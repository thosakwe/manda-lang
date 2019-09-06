#ifndef MANDA_AST_PRINTER_HPP
#define MANDA_AST_PRINTER_HPP
#include "ast.hpp"
#include <ostream>

namespace manda::analysis {
class AstPrinter : public CompilationUnitVisitor, DeclVisitor, ExprVisitor {
public:
  explicit AstPrinter(std::ostream &out);
  void indent();
  void outdent();
  void visitCompilationUnit(CompilationUnitCtx &ctx) override;
private:
  void visitExprDecl(ExprDeclCtx &ctx) override;
  void visitTypeDecl(TypeDeclCtx &ctx) override;
  void visitVarExpr(VarExprCtx &ctx) override;
  void visitFnDeclExpr(FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(VoidLiteralCtx &ctx) override;
  void visitIdExpr(IdExprCtx &ctx) override;
  void visitNumberLiteral(NumberLiteralCtx &ctx) override;
  void visitStringLiteral(StringLiteralCtx &ctx) override;
  void visitBoolLiteral(BoolLiteralCtx &ctx) override;
  void visitBlockExpr(BlockExprCtx &ctx) override;
  void visitTupleExpr(TupleExprCtx &ctx) override;
  void visitCastExpr(CastExprCtx &ctx) override;
  void visitCallExpr(CallExprCtx &ctx) override;

private:
  std::ostream &out;
  unsigned long level;
  std::ostream &print();
  void visitParenExpr(ParenExprCtx &ctx) override;
};
} // namespace manda::analysis

#endif
