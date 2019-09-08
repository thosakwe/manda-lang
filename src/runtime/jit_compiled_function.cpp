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
      gc(interpreter.getGarbageCollector()),
      jit_function(interpreter.getJitContext()) {
  // Create the JIT function.
  create();
  set_recompilable();
  coerceToAny.push(false);
  scope = make_shared<JitValueScope>();
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

jit_value JitCompiledFunction::insn_gc_ptr_callback(const std::string &name,
                                                    const jit_value &ptr,
                                                    void *func) {
  // void some_gc_func(GC*, void* ptr);
  jit_type_t params[2] = {jit_type_void_ptr, jit_type_void_ptr};
  auto sig =
      jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, params, 2, 0);
  jit_value_t args[2] = {new_constant((void *)&gc, jit_type_void_ptr).raw(),
                         ptr.raw()};
  return insn_call_native(name.c_str(), func, sig, args, 2, 0);
}

jit_value JitCompiledFunction::insn_gc_incref(const jit_value &ptr) {
  return insn_gc_ptr_callback("gc_incref", ptr,
                              (void *)&GarbageCollector::static_incref);
}

jit_value JitCompiledFunction::insn_gc_decref(const jit_value &ptr) {
  return insn_gc_ptr_callback("gc_decref", ptr,
                              (void *)&GarbageCollector::static_decref);
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

void JitCompiledFunction::visitIdExpr(const IdExprCtx &ctx) {
  // Search to see if we have already compiled the given symbol.
  // If not, compile it, and add it to the current scope.
  // TODO: Throw on redefines?
  auto jitSymbol = scope->resolve(ctx.name);
  if (jitSymbol) {
    // Load the variable.
    if (interpreter.getOptions().developerMode) {
      cout << "Emitting load of var \"" << ctx.name << "\"" << endl;
    }
    lastValue = insn_load(*jitSymbol);
  } else {
    // Look up the symbol.
    auto rtSymbol = astFunction.getScope()->resolve(ctx.name);
    if (!rtSymbol) {
      ostringstream oss;
      oss << "The name \"" << ctx.name << "\" does not exist in this context.";
      interpreter.reportError(ctx.location, oss.str());
      lastValue = nullopt;
    } else if (holds_alternative<shared_ptr<Type>>(*rtSymbol)) {
      // TODO: What if we are intending to resolve a type here?
      ostringstream oss;
      oss << "The value of \"" << ctx.name << "\" is a type, not an object.";
      interpreter.reportError(ctx.location, oss.str());
      lastValue = nullopt;
    } else if (holds_alternative<shared_ptr<Object>>(*rtSymbol)) {
      // TODO: This is probably not that safe...
      if (coerceToAny.top()) {
        auto object = get<shared_ptr<Object>>(*rtSymbol);
        auto jitPtr = new_constant((void *)object.get(), jit_type_void_ptr);
        auto variable = new_value(jit_type_void_ptr);
        jit_insn_store(raw(), variable.raw(), jitPtr.raw());
        if (interpreter.getOptions().developerMode) {
          cout << "Emitting new var-as-any \"" << ctx.name << "\"" << endl;
          cout << "Pointer="
               << jit_value_get_constant(jitPtr.raw()).un.ptr_value << endl;
        }
        lastValue = insn_load(variable);
      } else {
        // TODO: Add serialize() to Type...
        interpreter.reportError(ctx.location,
                                "TODO: Add serialize() to Type...");
        lastValue = nullopt;
      }
    } else {
      // TODO: Better message here.
      ostringstream oss;
      oss << "The value of \"" << ctx.name
          << "\" is unknown. This is a compiler error.";
      interpreter.reportError(ctx.location, oss.str());
      lastValue = nullopt;
    }
  }
}

void JitCompiledFunction::visitNumberLiteral(const NumberLiteralCtx &ctx) {
  if (!coerceToAny.top()) {
    // Manda Numbers are sys doubles.
    auto jitNumberType = interpreter.getCoreLibrary().numberType->toJitType();
    lastValue = new_constant((jit_float64)ctx.value, jitNumberType);
  } else {
    auto *object = gc.make<Number>(ctx.value);
    lastValue = new_constant((void *)object, jit_type_void_ptr);
  }
}

void JitCompiledFunction::visitStringLiteral(const StringLiteralCtx &ctx) {
  if (!coerceToAny.top()) {
    if (ctx.isChar()) {
      auto jitCharType = interpreter.getCoreLibrary().charType->toJitType();
      lastValue = new_constant((jit_ubyte)ctx.value[0], jitCharType);
    } else {
      auto *str = jit_strdup(ctx.getValue().c_str());
      gc.incref(str);
      auto jitStrType = interpreter.getCoreLibrary().stringType->toJitType();
      lastValue = new_constant((void *)str, jitStrType);
    }
  } else {
    Object *object;
    if (ctx.isChar()) {
      object = gc.make<Char>(ctx.value[0]);
    } else {
      object = gc.make<String>(ctx.getValue());
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
    auto *object = gc.make<Bool>(ctx.value);
    lastValue = new_constant((void *)object, jit_type_void_ptr);
  }
}

void JitCompiledFunction::visitBlockExpr(const BlockExprCtx &ctx) {
  // TODO: Create a new label, and introduce a new scope. Then, resolve
  // all expressions.
  // If there are no expressions, return void (a.k.a., do nothing here).
}

void JitCompiledFunction::visitTupleExpr(const TupleExprCtx &ctx) {
  if (coerceToAny.top()) {
    // Create a new tuple instance, and return the pointer.
    //    auto *object = new Bool(ctx.value);
    //    lastValue = new_constant((void *)object, jit_type_void_ptr);
    ObjectResolver resolver(interpreter, astFunction.getScope());
    auto *object = gc.make<Tuple>();
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

    // Allocate space for the tuple.
    auto jitTuplePointer = insn_malloc(jit_type_get_size(jitResultType));
    // Mark the object in the garbage collector.
    insn_gc_incref(jitTuplePointer);
    auto numFields = jit_type_num_fields(jitResultType);
    for (unsigned int i = 0; i < numFields; i++) {
      // Figure out the size.
      lastValue = nullopt;
      auto type = jit_type_get_field(jitResultType, i);
      auto offset = jit_type_get_offset(jitResultType, i);
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

    // Return *a read of the pointer*.
    lastValue = insn_load_relative(jitTuplePointer, 0, jitResultType);
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
