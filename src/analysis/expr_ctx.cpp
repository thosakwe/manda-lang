#include "expr_ctx.hpp"

void manda::analysis::FnDeclExprCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitFnDeclExpr(*this);
}

void manda::analysis::VarExprCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitVarExpr(*this);
}

void manda::analysis::StringLiteralCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitStringLiteral(*this);
}

void manda::analysis::IdExprCtx::accept(manda::analysis::ExprVisitor &visitor) {
  visitor.visitIdExpr(*this);
}

void manda::analysis::CallExprCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitCallExpr(*this);
}

void manda::analysis::VoidLiteralCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitVoidLiteral(*this);
}

void manda::analysis::CastExprCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitCastExpr(*this);
}

void manda::analysis::NumberLiteralCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitNumberLiteral(*this);
}

void manda::analysis::BlockExprCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitBlockExpr(*this);
}

void manda::analysis::TupleExprCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitTupleExpr(*this);
}

void manda::analysis::BoolLiteralCtx::accept(
    manda::analysis::ExprVisitor &visitor) {
  visitor.visitBoolLiteral(*this);
}
