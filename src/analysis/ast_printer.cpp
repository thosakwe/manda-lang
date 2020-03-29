#include "ast_printer.hpp"

using namespace manda::analysis;
using namespace std;

AstPrinter::AstPrinter(ostream &out) : IndentingPrinter(out) {}

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

void AstPrinter::visitIfClause(IfClauseCtx &ctx) {
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

void AstPrinter::visitIfExpr(IfExprCtx &ctx) {
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

void AstPrinter::visitFnDeclExpr(FnDeclExprCtx &ctx) {
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

void AstPrinter::visitVoidLiteral(VoidLiteralCtx &ctx) {
  print() << "VoidLiteral" << endl;
}

void AstPrinter::visitIdExpr(IdExprCtx &ctx) {
  print() << "IdExpr(" << ctx.name << ")" << endl;
}

void AstPrinter::visitNumberLiteral(NumberLiteralCtx &ctx) {
  print() << "NumberLiteral(" << ctx.value << ")" << endl;
}

void AstPrinter::visitStringLiteral(StringLiteralCtx &ctx) {
  if (ctx.isChar()) {
    print() << "CharLiteral(" << ctx.getValue() << ")" << endl;
  } else {
    print() << "StringLiteral, len=" << ctx.getValue().length() << " ("
            << ctx.getValue() << ")" << endl;
  }
}

void AstPrinter::visitBoolLiteral(BoolLiteralCtx &ctx) {
  print() << "BoolLiteral(" << (ctx.value ? "true" : "false") << ")" << endl;
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

void AstPrinter::visitListExpr(ListExprCtx &ctx) {
  print() << "ListExpr [" << endl;
  indent();
  for (auto &node : ctx.items) {
    node->accept(*this);
  }
  outdent();
  print() << "]" << endl;
}

void AstPrinter::visitCastExpr(CastExprCtx &ctx) {
  // TODO: Print
}

void AstPrinter::visitCallExpr(CallExprCtx &ctx) {
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

void AstPrinter::visitParenExpr(ParenExprCtx &ctx) {
  print() << "ParenExpr" << endl;
  indent();
  ctx.inner->accept(*this);
  outdent();
}
