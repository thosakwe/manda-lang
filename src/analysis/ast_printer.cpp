#include "ast_printer.hpp"

using namespace manda::analysis;
using namespace std;

AstPrinter::AstPrinter(ostream &out) : IndentingPrinter(out) {}

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

void AstPrinter::visitIfClause(const IfClauseCtx &ctx) {
  print() << "if" << endl;
  indent();
  ctx.condition->accept(*this);
  indent();
  print() << "then" << endl;
  indent();
  ctx.body->accept(*this);
  outdent();
  outdent();
  outdent();
}

void AstPrinter::visitIfExpr(const IfExprCtx &ctx) {
  print() << "IfExpr" << endl;
  indent();
  visitIfClause(*ctx.ifClause);
  for (auto &clause : ctx.elseIfClauses) {
    visitIfClause(*clause);
  }
  if (ctx.elseClause) {
    print() << "else" << endl;
    indent();
    ctx.elseClause->accept(*this);
    outdent();
  }
  outdent();
}

void AstPrinter::visitFnDeclExpr(const FnDeclExprCtx &ctx) {
  // TODO: Print
  print() << "FnDeclExpr(" << (ctx.name.empty() ? "<anonymous>" : ctx.name)
          << ")" << endl;
  indent();
  for (auto &param : ctx.params) {
    // TODO: Print type, default value
    print() << "Param " << param->name << endl;
  }
  if (!ctx.body) {
    print() << "<no body>" << endl;
  } else {
    ctx.body->accept(*this);
  }
  outdent();
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

void AstPrinter::visitStringLiteral(const StringLiteralCtx &ctx) {
  if (ctx.isChar()) {
    print() << "CharLiteral(" << ctx.getValue() << ")" << endl;
  } else {
    print() << "StringLiteral, len=" << ctx.getValue().length() << " ("
            << ctx.getValue() << ")" << endl;
  }
}

void AstPrinter::visitBoolLiteral(const BoolLiteralCtx &ctx) {
  print() << "BoolLiteral(" << (ctx.value ? "true" : "false") << ")" << endl;
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

void AstPrinter::visitListExpr(const ListExprCtx &ctx) {
  print() << "ListExpr [" << endl;
  indent();
  for (auto &node : ctx.items) {
    node->accept(*this);
  }
  outdent();
  print() << "]" << endl;
}

void AstPrinter::visitCastExpr(const CastExprCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitCallExpr(const CallExprCtx &ctx) {
  print() << "CallExpr(" << ctx.arguments.size() << " argument(s))" << endl;
  indent();
  ctx.target->accept(*this);
  if (!ctx.arguments.empty()) {
    indent();
    print() << "Arguments:" << endl;
    indent();
    for (auto &node : ctx.arguments) {
      node->accept(*this);
    }
    outdent();
    outdent();
  }
  outdent();
}

void AstPrinter::visitParenExpr(const ParenExprCtx &ctx) {
  print() << "ParenExpr" << endl;
  indent();
  ctx.inner->accept(*this);
  outdent();
}
