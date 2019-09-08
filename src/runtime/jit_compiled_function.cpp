#include "jit_compiled_function.hpp"
#include "ast_function.hpp"
#include "object_resolver.hpp"
#include <iostream>
#include <jit/jit-dump.h>
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
}

jit_type_t JitCompiledFunction::create_signature() {
  return astFunction.getType(interpreter)->toJitType();
}

void JitCompiledFunction::build() {
  // Compile the body, and return it.
  // TODO: Type checking...
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
  // Manda Numbers are sys doubles.
  auto jitNumberType = interpreter.getCoreLibrary().numberType->toJitType();
  lastValue = new_constant((jit_float64)ctx.value, jitNumberType);
}

void JitCompiledFunction::visitStringLiteral(const StringLiteralCtx &ctx) {}

void JitCompiledFunction::visitBoolLiteral(const BoolLiteralCtx &ctx) {}

void JitCompiledFunction::visitBlockExpr(const BlockExprCtx &ctx) {}

void JitCompiledFunction::visitTupleExpr(const TupleExprCtx &ctx) {}

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
  for (auto &arg : ctx.arguments) {
    lastValue = nullopt;
    arg->accept(*this);
    if (!lastValue) {
      // TODO: Better error message here?
      interpreter.reportError(ctx.target->location,
                              "Could not resolve all arguments for this call.");
      fail();
      return;
    }
    arguments.push_back(*lastValue);
  }

  // Compile the function call.
  lastValue = targetFunction->acceptForJitCall(*this, arguments);
}

void JitCompiledFunction::visitParenExpr(const ParenExprCtx &ctx) {}

Interpreter &JitCompiledFunction::getInterpreter() const { return interpreter; }
