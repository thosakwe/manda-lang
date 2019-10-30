#ifndef MANDA_AST_PRINTER_HPP
#define MANDA_AST_PRINTER_HPP
#include "ast.hpp"
#include "indenting_printer.hpp"
#include <ostream>

namespace manda::analysis {
class AstPrinter : public CompilationUnitVisitor,
                   public DeclVisitor,
                   public ExprVisitor,
                   public IndentingPrinter {
public:
  explicit AstPrinter(std::ostream &out);
  void visitCompilationUnit(const CompilationUnitCtx &ctx) override;
  void visitExprDecl(const ExprDeclCtx &ctx) override;
  void visitTypeDecl(const TypeDeclCtx &ctx) override;
  void visitVarExpr(const VarExprCtx &ctx) override;
  void visitFnDeclExpr(const FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(const VoidLiteralCtx &ctx) override;
  void visitIdExpr(const IdExprCtx &ctx) override;
  void visitNumberLiteral(const NumberLiteralCtx &ctx) override;
  void visitStringLiteral(const StringLiteralCtx &ctx) override;
  void visitBoolLiteral(const BoolLiteralCtx &ctx) override;
  void visitBlockExpr(const BlockExprCtx &ctx) override;
  void visitTupleExpr(const TupleExprCtx &ctx) override;
  void visitCastExpr(const CastExprCtx &ctx) override;
  void visitCallExpr(const CallExprCtx &ctx) override;
  void visitParenExpr(const ParenExprCtx &ctx) override;
};
} // namespace manda::analysis

#endif
