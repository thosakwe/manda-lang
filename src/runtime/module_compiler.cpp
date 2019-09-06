#include "module_compiler.hpp"

manda::runtime::ModuleCompiler::ModuleCompiler() : module{""} {}

manda::runtime::Module &manda::runtime::ModuleCompiler::getModule() {
  return module;
}

void manda::runtime::ModuleCompiler::visitCompilationUnit(
    manda::analysis::CompilationUnitCtx &ctx) {
  for (auto &node : ctx.declarations) {
    node->accept(*this);
  }
}

void manda::runtime::ModuleCompiler::visitExprDecl(
    manda::analysis::ExprDeclCtx &ctx) {
  using namespace manda::analysis;
  auto *topLevel = dynamic_cast<TopLevelExprCtx *>(ctx.value.get());
  if (!topLevel) {
    // TODO: Throw error
  } else {
    // TODO: Visit top-level
    topLevel->accept(*this);
  }
}

void manda::runtime::ModuleCompiler::visitTypeDecl(
    manda::analysis::TypeDeclCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitVarExpr(
    manda::analysis::VarExprCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitFnDeclExpr(
    manda::analysis::FnDeclExprCtx &ctx) {
  // Emit an "empty" function object, that simply points to the fn decl.
}

void manda::runtime::ModuleCompiler::visitVoidLiteral(
    manda::analysis::VoidLiteralCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitIdExpr(
    manda::analysis::IdExprCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitNumberLiteral(
    manda::analysis::NumberLiteralCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitStringLiteral(
    manda::analysis::StringLiteralCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitBoolLiteral(
    manda::analysis::BoolLiteralCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitBlockExpr(
    manda::analysis::BlockExprCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitTupleExpr(
    manda::analysis::TupleExprCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitCastExpr(
    manda::analysis::CastExprCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitCallExpr(
    manda::analysis::CallExprCtx &ctx) {}

void manda::runtime::ModuleCompiler::visitParenExpr(
    manda::analysis::ParenExprCtx &ctx) {}
