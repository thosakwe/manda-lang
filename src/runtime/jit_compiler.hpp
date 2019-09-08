#ifndef MANDA_JIT_COMPILER_HPP
#define MANDA_JIT_COMPILER_HPP
#include "ast_function.hpp"
#include "symbol_table.hpp"
#include <optional>

namespace manda::runtime {
class Interpreter;
class JitCompiler : public manda::analysis::ExprVisitor {
public:
  explicit JitCompiler(Interpreter &interpreter, const AstFunction &fn);
  jit_function getJitFunction();
  bool compile();
  void visitVarExpr(const analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(const analysis::FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(const analysis::VoidLiteralCtx &ctx) override;
  void visitIdExpr(const analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(const analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(const analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(const analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(const analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(const analysis::TupleExprCtx &ctx) override;
  void visitCastExpr(const analysis::CastExprCtx &ctx) override;
  void visitCallExpr(const analysis::CallExprCtx &ctx) override;
  void visitParenExpr(const analysis::ParenExprCtx &ctx) override;

private:
  Interpreter &interpreter;
  const AstFunction &astFunction;
  std::optional<jit_function> jitFunction;
  std::optional<jit_value> lastValue;
};
} // namespace manda::runtime

#endif
