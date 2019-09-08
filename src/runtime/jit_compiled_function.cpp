#include "jit_compiled_function.hpp"
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

void JitCompiledFunction::publicBuild() { build(); }

void JitCompiledFunction::build() {
  // Compile the body, and return it.
  // TODO: Type checking...
  astFunction.getNode()->body->accept(*this);
  if (!lastValue) {
    // TODO: Come up with a better error message.
    interpreter.reportError(astFunction.getNode()->location,
                            "JIT compilation failed.");
    fail();
    return;
  } else {
    // Return the value.
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

void JitCompiledFunction::visitCallExpr(const CallExprCtx &ctx) {}

void JitCompiledFunction::visitParenExpr(const ParenExprCtx &ctx) {}
