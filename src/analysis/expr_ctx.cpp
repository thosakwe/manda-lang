#include "expr_ctx.hpp"
#include <string>

using namespace manda::analysis;
using namespace std;

void FnDeclExprCtx::accept(ExprVisitor &visitor) {
  visitor.visitFnDeclExpr(*this);
}

void VarExprCtx::accept(ExprVisitor &visitor) { visitor.visitVarExpr(*this); }

void StringLiteralCtx::accept(ExprVisitor &visitor) {
  visitor.visitStringLiteral(*this);
}

void IdExprCtx::accept(ExprVisitor &visitor) { visitor.visitIdExpr(*this); }

void CallExprCtx::accept(ExprVisitor &visitor) { visitor.visitCallExpr(*this); }

void VoidLiteralCtx::accept(ExprVisitor &visitor) {
  visitor.visitVoidLiteral(*this);
}

void CastExprCtx::accept(ExprVisitor &visitor) { visitor.visitCastExpr(*this); }

void NumberLiteralCtx::accept(ExprVisitor &visitor) {
  visitor.visitNumberLiteral(*this);
}

void BlockExprCtx::accept(ExprVisitor &visitor) {
  visitor.visitBlockExpr(*this);
}

void TupleExprCtx::accept(ExprVisitor &visitor) {
  visitor.visitTupleExpr(*this);
}

void BoolLiteralCtx::accept(ExprVisitor &visitor) {
  visitor.visitBoolLiteral(*this);
}

void ParenExprCtx::accept(ExprVisitor &visitor) {
  visitor.visitParenExpr(*this);
}

string QuoteEscapeStringPartCtx::convert(bool singleQuote) const {
  return singleQuote ? "'" : "\"";
}
string HexEscapeStringPartCtx::convert(bool singleQuote) const {
  string s;
  s += (char)stol(text.substr(2), nullptr, 16);
  return s;
}

string TextStringPartCtx::convert(bool singleQuote) const { return text; }
