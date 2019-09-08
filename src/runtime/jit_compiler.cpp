#include "jit_compiler.hpp"
#include <vector>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

JitCompiler::JitCompiler(Interpreter &interpreter, const AstFunction &fn)
    : interpreter(interpreter), astFunction(fn) {
  // Create the JIT function.
  auto signature = astFunction.getType(interpreter)->toJitType();
  jitFunction = jit_function(interpreter.getJitContext(), signature);
}

jit_function JitCompiler::getJitFunction() { return *jitFunction; }

bool JitCompiler::compile() {
  // Compile the body, and return it.
  // TODO: Type checking...
  astFunction.getNode()->body->accept(*this);
  if (!lastValue) {
    // TODO: Come up with a better error message.
    interpreter.reportError(astFunction.getNode()->location,
                            "JIT compilation failed.");
    return false;
  } else {
    // Return the value.
    jitFunction->insn_return(*lastValue);
  }
  // Always emit a default_return().
  // TODO: Prevent this if the function does not return Void or Any.
  jitFunction->insn_default_return();
  // Compile the function.
  interpreter.getJitContext().build_end();
  jitFunction->compile();
  interpreter.getJitContext().build_start();
  return true;
}

void JitCompiler::visitVarExpr(const VarExprCtx &ctx) {}

void JitCompiler::visitFnDeclExpr(const FnDeclExprCtx &ctx) {}

void JitCompiler::visitVoidLiteral(const VoidLiteralCtx &ctx) {}

void JitCompiler::visitIdExpr(const IdExprCtx &ctx) {}

void JitCompiler::visitNumberLiteral(const NumberLiteralCtx &ctx) {
  // Manda Numbers are sys doubles.
  auto jitNumberType = interpreter.getCoreLibrary().numberType->toJitType();
  lastValue = jit_value_create_float64_constant(jitFunction->raw(),
                                                jitNumberType, ctx.value);
}

void JitCompiler::visitStringLiteral(const StringLiteralCtx &ctx) {}

void JitCompiler::visitBoolLiteral(const BoolLiteralCtx &ctx) {}

void JitCompiler::visitBlockExpr(const BlockExprCtx &ctx) {}

void JitCompiler::visitTupleExpr(const TupleExprCtx &ctx) {}

void JitCompiler::visitCastExpr(const CastExprCtx &ctx) {}

void JitCompiler::visitCallExpr(const CallExprCtx &ctx) {}

void JitCompiler::visitParenExpr(const ParenExprCtx &ctx) {}
