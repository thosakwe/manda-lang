#include "expr_ctx.hpp"
#include <sstream>
#include <string>

using namespace manda::analysis;
using namespace std;

NumberLiteralCtx::NumberLiteralCtx(const Location &location, double value)
    : value(value), ExprCtx(location) {}

IdExprCtx::IdExprCtx(const Location &location, std::string name)
    : name(move(name)), ExprCtx(location) {}

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

std::unique_ptr<ExprCtx> ExprCtx::cloneToUniquePointer() const {
  return unique_ptr<ExprCtx>(clone());
}

ParamCtx *ParamCtx::clone() const {
  return new ParamCtx{
      location, name,
      type == nullptr ? nullptr : unique_ptr<TypeCtx>(type->clone()),
      defaultValue == nullptr ? nullptr : defaultValue->cloneToUniquePointer()};
}

unique_ptr<ParamCtx> ParamCtx::cloneToUniquePointer() const {
  return unique_ptr<ParamCtx>(clone());
}

CallExprCtx *CallExprCtx::clone() const {
  auto *out = new CallExprCtx();
  out->location = location;
  out->target = target->cloneToUniquePointer();
  for (auto &ptr : arguments) {
    out->arguments.push_back(ptr->cloneToUniquePointer());
  }
  return out;
}

FnDeclExprCtx *FnDeclExprCtx::clone() const {
  auto *out = new FnDeclExprCtx;
  out->location = location;
  out->isPublic = isPublic;
  out->name = name;
  out->body = body == nullptr ? nullptr : body->cloneToUniquePointer();
  out->returnType =
      returnType == nullptr ? nullptr : returnType->cloneToUniquePointer();
  for (auto &ptr : params) {
    out->params.push_back(ptr->cloneToUniquePointer());
  }
  return out;
}

CastExprCtx *CastExprCtx::clone() const {
  auto *out = new CastExprCtx;
  out->location = location;
  out->value = value->cloneToUniquePointer();
  out->type = type->cloneToUniquePointer();
  return out;
}

NumberLiteralCtx *NumberLiteralCtx::clone() const {
  return new NumberLiteralCtx(location, value);
}

BoolLiteralCtx *BoolLiteralCtx::clone() const {
  auto *out = new BoolLiteralCtx;
  out->location = location;
  out->value = value;
  return out;
}

ParenExprCtx *ParenExprCtx::clone() const {
  auto *out = new ParenExprCtx();
  out->location = location;
  out->inner = inner->cloneToUniquePointer();
  return out;
}

StringLiteralCtx *StringLiteralCtx::clone() const {
  auto *out = new StringLiteralCtx;
  out->location = location;
  out->singleQuote = singleQuote;
  out->value = value;
  for (auto &ptr : parts) {
    out->parts.push_back(ptr->cloneToUniquePointer());
  }
  return out;
}

std::unique_ptr<StringPartCtx> StringPartCtx::cloneToUniquePointer() const {
  return unique_ptr<StringPartCtx>(clone());
}

QuoteEscapeStringPartCtx *QuoteEscapeStringPartCtx::clone() const {
  return new QuoteEscapeStringPartCtx;
}

HexEscapeStringPartCtx *HexEscapeStringPartCtx::clone() const {
  return new HexEscapeStringPartCtx(location, text);
}

TextStringPartCtx *TextStringPartCtx::clone() const {
  return new TextStringPartCtx(location, text);
}

VarExprCtx *VarExprCtx::clone() const {
  auto *out = new VarExprCtx;
  out->location = location;
  out->name = name;
  out->value = value->cloneToUniquePointer();
  out->isFinal = isFinal;
  out->isPublic = isPublic;
  return out;
}

BlockExprCtx *BlockExprCtx::clone() const {
  auto *out = new BlockExprCtx(location);
  out->location = location;
  for (auto &ptr : body) {
    out->body.push_back(ptr->cloneToUniquePointer());
  }
  return out;
}

VoidLiteralCtx *VoidLiteralCtx::clone() const {
  auto *out = new VoidLiteralCtx();
  out->location = location;
  return out;
}

TupleExprCtx *TupleExprCtx::clone() const {
  auto *out = new TupleExprCtx();
  out->location = location;
  for (auto &ptr : items) {
    out->items.push_back(ptr->cloneToUniquePointer());
  }
  return out;
}

IdExprCtx *IdExprCtx::clone() const { return new IdExprCtx(location, name); }
