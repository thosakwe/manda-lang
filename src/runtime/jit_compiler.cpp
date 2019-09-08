#include "jit_compiler.hpp"
#include <vector>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

JitCompiler::JitCompiler(Interpreter &interpreter, shared_ptr<AstFunction> fn)
    : interpreter(interpreter), astFunction(move(fn)) {
  // Create the JIT function.
  auto signature = astFunction->getType(interpreter)->toJitType();
  jitFunction = jit_function(interpreter.getJitContext(), signature);
}

jit_function JitCompiler::getJitFunction() { return *jitFunction; }

void JitCompiler::compile() {
  // Compile the body, and return it.
  // TODO: Type checking...
  astFunction->getNode()->body->accept(*this);
  if (!lastValue) {
    // TODO: Come up with a better error message.
    interpreter.reportError(astFunction->getNode()->location,
                            "JIT compilation failed.");
  } else {
    // Return the value.
    jitFunction->insn_return(*lastValue);
  }
}

void JitCompiler::visitVarExpr(const VarExprCtx &ctx) {}

void JitCompiler::visitFnDeclExpr(const FnDeclExprCtx &ctx) {}

void JitCompiler::visitVoidLiteral(const VoidLiteralCtx &ctx) {}

void JitCompiler::visitIdExpr(const IdExprCtx &ctx) {}

void JitCompiler::visitNumberLiteral(const NumberLiteralCtx &ctx) {}

void JitCompiler::visitStringLiteral(const StringLiteralCtx &ctx) {}

void JitCompiler::visitBoolLiteral(const BoolLiteralCtx &ctx) {}

void JitCompiler::visitBlockExpr(const BlockExprCtx &ctx) {}

void JitCompiler::visitTupleExpr(const TupleExprCtx &ctx) {}

void JitCompiler::visitCastExpr(const CastExprCtx &ctx) {}

void JitCompiler::visitCallExpr(const CallExprCtx &ctx) {}

void JitCompiler::visitParenExpr(const ParenExprCtx &ctx) {}
