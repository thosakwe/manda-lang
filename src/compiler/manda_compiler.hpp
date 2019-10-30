#ifndef MANDA_MANDA_ANALYZER_HPP
#define MANDA_MANDA_ANALYZER_HPP

#include "../analysis/ast.hpp"
#include "../analysis/expr_ctx.hpp"
#include "../ir/ir_module.hpp"
#include "../runtime/base_resolver.hpp"
#include "../runtime/type_resolver.hpp"

namespace manda::compiler {
/**
 * A class that is capable of resolving the types of every expression
 * in a Manda source file, and producing IR from it.
 */
class MandaCompiler : public analysis::CompilationUnitVisitor,
                      public analysis::ExprVisitor,
                      public analysis::TypeVisitor,
                      public analysis::DeclVisitor,
                      public runtime::BaseResolver {
public:
  const std::shared_ptr<manda::ir::IRModule> &getModule() const;
  void visitExprDecl(const analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(const analysis::TypeDeclCtx &ctx) override;
  void visitCompilationUnit(const analysis::CompilationUnitCtx &ctx) override;
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
  void visitTypeRef(const analysis::TypeRefCtx &ctx) override;

private:
  std::shared_ptr<manda::ir::IRModule> module;
};
} // namespace manda::compiler

#endif
