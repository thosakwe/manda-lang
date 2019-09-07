#include "expr_ctx.hpp"
#include <sstream>
#include <string>

using namespace manda::analysis;
using namespace std;

void FnDeclExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitFnDeclExpr(*this);
}

void VarExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitVarExpr(*this);
}

void StringLiteralCtx::accept(ExprVisitor &visitor) const {
  visitor.visitStringLiteral(*this);
}

string StringLiteralCtx::getValue() const {
  if (singleQuote) {
    return value;
  } else {
    ostringstream oss;
    for (auto &part : parts) {
      oss << part->convert(false);
    }
    return oss.str();
  }
}

bool StringLiteralCtx::isChar() const { return singleQuote; }

void IdExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitIdExpr(*this);
}

void CallExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitCallExpr(*this);
}

void VoidLiteralCtx::accept(ExprVisitor &visitor) const {
  visitor.visitVoidLiteral(*this);
}

void CastExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitCastExpr(*this);
}

void NumberLiteralCtx::accept(ExprVisitor &visitor) const {
  visitor.visitNumberLiteral(*this);
}

void BlockExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitBlockExpr(*this);
}

void TupleExprCtx::accept(ExprVisitor &visitor) const {
  visitor.visitTupleExpr(*this);
}

void BoolLiteralCtx::accept(ExprVisitor &visitor) const {
  visitor.visitBoolLiteral(*this);
}

void ParenExprCtx::accept(ExprVisitor &visitor) const {
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
