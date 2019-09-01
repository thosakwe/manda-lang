#include "module_compiler.hpp"

manda::runtime::ModuleCompiler::ModuleCompiler() : module{""} {}

manda::runtime::Module &manda::runtime::ModuleCompiler::getModule() {
  return module;
}

void manda::runtime::ModuleCompiler::visitExprDecl(
    manda::analysis::ExprDeclCtx &ctx) {
  using namespace manda::analysis;
  auto *topLevel = dynamic_cast<TopLevelExprCtx *>(ctx.value.get());
  if (!topLevel) {
    // TODO: Throw error
  } else {
    // TODO: Visit top-level
  }
}

void manda::runtime::ModuleCompiler::visitTypeDecl(
    manda::analysis::TypeDeclCtx &ctx) {}
