#include "worker.hpp"
#include "module_compiler.hpp"

void manda::runtime::Worker::loadCompilationUnit(
    std::shared_ptr<manda::analysis::CompilationUnitCtx> &ctx) {
  // TODO: Do the logic...
  return;
  ModuleCompiler compiler;
  for (auto &decl : ctx->declarations) {
    decl->accept(compiler);
  }

  auto &module = compiler.getModule();
  auto main = module.getSymbolTable().resolve("main");

  if (!std::holds_alternative<std::shared_ptr<Object>>(main)) {
    // TODO: Throw error if no main function was found.
  } else {
    auto mainMethod = std::get<std::shared_ptr<Object>>(main).get();
    // TODO: Dynamic cast to function, and execute
  }
}
