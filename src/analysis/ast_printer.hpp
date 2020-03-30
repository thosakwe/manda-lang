#ifndef MANDA_AST_PRINTER_HPP
#define MANDA_AST_PRINTER_HPP
#include "ast.hpp"
#include "indenting_printer.hpp"
#include <ostream>

namespace manda::analysis {
class AstPrinter : public CompilationUnitVisitor,
                   public DeclVisitor,
                   public ExprVisitor,
                   public TypeVisitor,
                   public IndentingPrinter {
public:
  explicit AstPrinter(std::ostream &out);
  void visitCompilationUnit(CompilationUnitCtx &ctx) override;
  void visitExprDecl(ExprDeclCtx &ctx) override;
  void visitTypeDecl(TypeDeclCtx &ctx) override;
  void visitVarExpr(VarExprCtx &ctx) override;
  void visitFnDeclExpr(FnDeclExprCtx &ctx) override;
  void visitIfClause(IfClauseCtx &ctx);
  void visitVoidLiteral(VoidLiteralCtx &ctx) override;
  void visitIdExpr(IdExprCtx &ctx) override;
  void visitNumberLiteral(NumberLiteralCtx &ctx) override;
  void visitStringLiteral(StringLiteralCtx &ctx) override;
  void visitBoolLiteral(BoolLiteralCtx &ctx) override;
  void visitBlockExpr(BlockExprCtx &ctx) override;
  void visitTupleExpr(TupleExprCtx &ctx) override;
  void visitListExpr(ListExprCtx &ctx) override;
  void visitCastExpr(CastExprCtx &ctx) override;
  void visitCallExpr(CallExprCtx &ctx) override;
  void visitParenExpr(ParenExprCtx &ctx) override;
  void visitIfExpr(IfExprCtx &ctx) override;
  void visitTypeRef(TypeRefCtx &ctx) override;
};
} // namespace manda::analysis

#endif
