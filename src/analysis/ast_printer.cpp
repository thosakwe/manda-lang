#include "ast_printer.hpp"

using namespace manda::analysis;
using namespace std;

AstPrinter::AstPrinter(ostream &out) : out(out) { level = 0; }

void AstPrinter::indent() { level++; }

void AstPrinter::outdent() {
  if (level > 0)
    level--;
}

ostream &AstPrinter::print() {
  for (unsigned long i = 0; i < level; i++) {
    out << "  ";
  }
  out << "- ";
  return out;
}

void AstPrinter::visitCompilationUnit(const CompilationUnitCtx &ctx) {
  print() << "CompilationUnit" << endl;
  indent();
  for (auto &node : ctx.declarations) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitExprDecl(const ExprDeclCtx &ctx) {
  print() << "ExprDecl" << endl;
  indent();
  ctx.value->accept(*this);
  outdent();
}

void AstPrinter::visitTypeDecl(const TypeDeclCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitVarExpr(const VarExprCtx &ctx) {
  print() << "VarExpr(" << ctx.name << ")" << endl;
  indent();
  ctx.value->accept(*this);
  outdent();
}

void AstPrinter::visitFnDeclExpr(const FnDeclExprCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitVoidLiteral(const VoidLiteralCtx &ctx) {
  print() << "VoidLiteral" << endl;
}

void AstPrinter::visitIdExpr(const IdExprCtx &ctx) {
  print() << "IdExpr(" << ctx.name << ")" << endl;
}

void AstPrinter::visitNumberLiteral(const NumberLiteralCtx &ctx) {
  print() << "NumberLiteral(" << ctx.value << ")" << endl;
}

void AstPrinter::visitStringLiteral(const StringLiteralCtx &ctx) {}

void AstPrinter::visitBoolLiteral(const BoolLiteralCtx &ctx) {
  print() << "BoolLiteral(" << ctx.value << ")" << endl;
}

void AstPrinter::visitBlockExpr(const BlockExprCtx &ctx) {
  print() << "BlockExpr" << endl;
  indent();
  for (auto &node : ctx.body) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitTupleExpr(const TupleExprCtx &ctx) {
  print() << "TupleExpr" << endl;
  indent();
  for (auto &node : ctx.items) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitCastExpr(const CastExprCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitCallExpr(const CallExprCtx &ctx) {
  print() << "CallExpr" << endl;
  indent();
  ctx.target->accept(*this);
  for (auto &node : ctx.arguments) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitParenExpr(const ParenExprCtx &ctx) {
  print() << "ParenExpr" << endl;
  indent();
  ctx.inner->accept(*this);
  outdent();
}
