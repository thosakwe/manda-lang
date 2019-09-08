#include "jit_compiled_function.hpp"
#include "../analysis/ast_printer.hpp"
#include "ast_function.hpp"
#include "char.hpp"
#include "manda_api_impl.hpp"
#include "number.hpp"
#include "object_resolver.hpp"
#include "string.hpp"
#include "tuple.hpp"
#include "type_resolver.hpp"
#include <iostream>
#include <jit/jit-dump.h>
#include <sstream>
#include <vector>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

JitCompiledFunction::JitCompiledFunction(Interpreter &interpreter,
                                         const AstFunction &fn)
    : interpreter(interpreter), astFunction(fn),
      jit_function(interpreter.getJitContext()) {
  // Create the JIT function.
  create();
  set_recompilable();
  coerceToAny.push(false);
}

jit_type_t JitCompiledFunction::create_signature() {
  return astFunction.getType(interpreter)->toJitType();
}

jit_value JitCompiledFunction::insn_malloc(const jit_value &size) {
  // void* jit_malloc(unsigned int size)
  jit_type_t params[1] = {jit_type_sys_uint};
  auto sig =
      jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, params, 1, 0);
  jit_value_t args[1] = {size.raw()};
  return insn_call_native("jit_malloc", (void *)&jit_malloc, sig, args, 1, 0);
}

jit_value JitCompiledFunction::insn_malloc(jit_uint size) {
  return insn_malloc(new_constant(size, jit_type_sys_uint));
}

void JitCompiledFunction::build() {
  // Compile the body, and return it.
  // TODO: Type checking...
  AstPrinter printer(cout);
  if (interpreter.getOptions().developerMode) {
    cout << "Function body:" << endl;
    astFunction.getNode()->body->accept(printer);
  }
  astFunction.getNode()->body->accept(*this);

  if (!lastValue) {
    // The only way a function have no return is if it is
    // marked as returning void.
    // TODO: Should `Any` also be able to return void? I doubt it.
    auto returnType = astFunction.getReturnType(interpreter);
    auto voidType = interpreter.getCoreLibrary().voidType;
    if (!(returnType->isAssignableTo(voidType))) {
      // TODO: Come up with a better error message.
      interpreter.reportError(astFunction.getNode()->location,
                              "JIT compilation failed.");
      if (interpreter.getOptions().developerMode) {
        cout << "Function type: " << astFunction.getType(interpreter)->getName()
             << endl;
        cout << "Expected return type: "
             << astFunction.getReturnType(interpreter)->getName() << endl;
        cout << "The compilation of this function did not return a value."
             << endl;
      }
      fail();
      return;
    }
  } else {
    // Return the value.
    // TODO: Ensure that the return type matches.
    insn_return(*lastValue);
  }
  // Always emit a default_return().
  // TODO: Prevent this if the function does not return Void or Any.
  insn_default_return();

  if (interpreter.getOptions().developerMode) {
    jit_dump_function(stdout, raw(), astFunction.getName().c_str());
    cout << "Note: Execution is not actually performed via JIT yet." << endl;
  }
}

void JitCompiledFunction::visitVarExpr(const VarExprCtx &ctx) {}

void JitCompiledFunction::visitFnDeclExpr(const FnDeclExprCtx &ctx) {}

void JitCompiledFunction::visitVoidLiteral(const VoidLiteralCtx &ctx) {}

void JitCompiledFunction::visitIdExpr(const IdExprCtx &ctx) {}

void JitCompiledFunction::visitNumberLiteral(const NumberLiteralCtx &ctx) {
  if (!coerceToAny.top()) {
    // Manda Numbers are sys doubles.
    auto jitNumberType = interpreter.getCoreLibrary().numberType->toJitType();
    lastValue = new_constant((jit_float64)ctx.value, jitNumberType);
  } else {
    // TODO: Delete the pointer...
    auto *object = new Number(ctx.value);
    lastValue = new_constant((void *)object, jit_type_void_ptr);
  }
}

void JitCompiledFunction::visitStringLiteral(const StringLiteralCtx &ctx) {
  if (!coerceToAny.top()) {
    if (ctx.isChar()) {
      auto jitCharType = interpreter.getCoreLibrary().charType->toJitType();
      lastValue = new_constant((jit_ubyte)ctx.value[0], jitCharType);
    } else {
      // TODO: Delete the pointer
      auto *str = jit_strdup(ctx.getValue().c_str());
      auto jitStrType = interpreter.getCoreLibrary().stringType->toJitType();
      lastValue = new_constant((void *)str, jitStrType);
    }
  } else {
    // TODO: Delete the pointer...
    Object *object;
    if (ctx.isChar()) {
      object = new Char(ctx.value[0]);
    } else {
      object = new String(ctx.getValue());
    }
    lastValue = new_constant((void *)object, jit_type_void_ptr);
  }
}

void JitCompiledFunction::visitBoolLiteral(const BoolLiteralCtx &ctx) {
  jit_ubyte value = ctx.value ? 0x1 : 0x0;
  if (!coerceToAny.top()) {
    // Manda Booleans are either 0x0 or 0x1.
    auto jitBoolType = interpreter.getCoreLibrary().boolType->toJitType();
    lastValue = new_constant(value, jitBoolType);
  } else {
    // TODO: Delete the pointer...
    auto *object = new Bool(ctx.value);
    lastValue = new_constant((void *)object, jit_type_void_ptr);
  }
}

void JitCompiledFunction::visitBlockExpr(const BlockExprCtx &ctx) {}

void JitCompiledFunction::visitTupleExpr(const TupleExprCtx &ctx) {
  if (coerceToAny.top()) {
    // Create a new tuple instance, and return the pointer.
    //    auto *object = new Bool(ctx.value);
    //    lastValue = new_constant((void *)object, jit_type_void_ptr);
    // TODO: Delete the pointer
    ObjectResolver resolver(interpreter, astFunction.getScope());
    auto *object = new Tuple();
    for (auto &item : ctx.items) {
      item->accept(resolver);
      auto result = resolver.getLastObject();
      if (!result) {
        interpreter.reportError(item->location,
                                "Could not resolve every item in this tuple.");
        lastValue = nullopt;
        return;
      } else {
        object->getItems().push_back(result);
      }
    }
    lastValue = new_constant((void *)object, jit_type_void_ptr);
  } else {
    // Resolve this item to a tuple type.
    // Then, create a temporary structure value.
    // For each field, write into the current location.
    // Increment by the size of the field.
    TypeResolver resolver(interpreter, astFunction.getScope());
    ctx.accept(resolver);
    auto resultType = resolver.getLastType();
    if (!resultType) {
      // TODO: This should never happen, so there probably needs to be
      // a good error message here in the case that it does.
      interpreter.reportError(ctx.location,
                              "Could not resolve the type of this tuple.");
      lastValue = nullopt;
      return;
    }

    auto jitResultType = resultType->toJitType();
    if (!jit_type_is_struct(jitResultType)) {
      // TODO: Since this is an actual compiler error, there should probably
      // be a better dump/diagnostic here.
      interpreter.reportError(ctx.location,
                              "This tuple did not produce a structure type.");
      lastValue = nullopt;
      return;
    }

    // Allocate space for the tuple on the stack.
    // TODO: Delete the pointer.
    auto jitTuplePointer = insn_malloc(jit_type_get_size(jitResultType));
    auto numFields = jit_type_num_fields(jitResultType);
    jit_nint offset = 0;
    for (unsigned int i = 0; i < numFields; i++) {
      // Figure out the size.
      lastValue = nullopt;
      auto type = jit_type_get_field(jitResultType, 0);
      ctx.items[i]->accept(*this);
      if (!lastValue) {
        ostringstream oss;
        oss << "Failed to compile expression at index ";
        oss << i << " in tuple.";
        interpreter.reportError(ctx.location, oss.str());
        lastValue = nullopt;
        return;
      }
      insn_store_relative(jitTuplePointer, offset, *lastValue);
    }

    // Return the pointer.
    lastValue = jitTuplePointer;
  }
}

void JitCompiledFunction::visitCastExpr(const CastExprCtx &ctx) {}

void JitCompiledFunction::visitCallExpr(const CallExprCtx &ctx) {
  // Resolve the target first.
  ObjectResolver resolver(interpreter, astFunction.getScope());
  ctx.target->accept(resolver);
  auto target = resolver.getLastObject();
  if (!target) {
    // TODO: Better error message here?
    interpreter.reportError(ctx.target->location,
                            "Evaluation of this call target failed.");
    fail();
    return;
  }

  // Ensure that the target is a function.
  // TODO: What if we are instantiating a type?
  auto *targetFunction = dynamic_cast<Function *>(target.get());
  if (!targetFunction) {
    // TODO: Better error message here?
    interpreter.reportError(ctx.target->location,
                            "Only functions can be called.");
    fail();
    return;
  }

  // TODO: Match arguments to parameters
  vector<jit_value> arguments;
  coerceToAny.push(true);
  for (auto &arg : ctx.arguments) {
    lastValue = nullopt;
    arg->accept(*this);
    if (!lastValue) {
      // TODO: Better error message here?
      interpreter.reportError(ctx.target->location,
                              "Could not resolve all arguments for this call.");
      fail();
      coerceToAny.pop();
      return;
    }
    arguments.push_back(*lastValue);
  }

  // Compile the function call.
  lastValue = targetFunction->acceptForJitCall(*this, arguments);
  coerceToAny.pop();
}

void JitCompiledFunction::visitParenExpr(const ParenExprCtx &ctx) {
  ctx.inner->accept(*this);
}

Interpreter &JitCompiledFunction::getInterpreter() const { return interpreter; }
