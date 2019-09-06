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

void AstPrinter::visitCompilationUnit(CompilationUnitCtx &ctx) {
  print() << "CompilationUnit" << endl;
  indent();
  for (auto &node : ctx.declarations) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitExprDecl(ExprDeclCtx &ctx) {
  print() << "ExprDecl" << endl;
  indent();
  ctx.value->accept(*this);
  outdent();
}

void AstPrinter::visitTypeDecl(TypeDeclCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitVarExpr(VarExprCtx &ctx) {
  print() << "VarExpr(" << ctx.name << ")" << endl;
  indent();
  ctx.value->accept(*this);
  outdent();
}

void AstPrinter::visitFnDeclExpr(FnDeclExprCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitVoidLiteral(VoidLiteralCtx &ctx) {
  print() << "VoidLiteral" << endl;
}

void AstPrinter::visitIdExpr(IdExprCtx &ctx) {
  print() << "IdExpr(" << ctx.name << ")" << endl;
}

void AstPrinter::visitNumberLiteral(NumberLiteralCtx &ctx) {
  print() << "NumberLiteral(" << ctx.value << ")" << endl;
}

void AstPrinter::visitStringLiteral(StringLiteralCtx &ctx) {}

void AstPrinter::visitBoolLiteral(BoolLiteralCtx &ctx) {
  print() << "BoolLiteral(" << ctx.value << ")" << endl;
}

void AstPrinter::visitBlockExpr(BlockExprCtx &ctx) {
  print() << "BlockExpr" << endl;
  indent();
  for (auto &node : ctx.body) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitTupleExpr(TupleExprCtx &ctx) {
  print() << "TupleExpr" << endl;
  indent();
  for (auto &node : ctx.items) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitCastExpr(CastExprCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitCallExpr(CallExprCtx &ctx) {
  print() << "CallExpr" << endl;
  indent();
  ctx.target->accept(*this);
  for (auto &node : ctx.arguments) {
    node->accept(*this);
  }
  outdent();
}

void AstPrinter::visitParenExpr(ParenExprCtx &ctx) {
  print() << "ParenExpr" << endl;
  indent();
  ctx.inner->accept(*this);
  outdent();
}
