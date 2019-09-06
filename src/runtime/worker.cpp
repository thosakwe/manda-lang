#include "worker.hpp"
#include "module_compiler.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

void Worker::loadCompilationUnit(shared_ptr<CompilationUnitCtx> &ctx) {
  // TODO: Do the logic...
  ModuleCompiler compiler;
  ctx->accept(compiler);

  auto &module = compiler.getModule();
  auto main = module.getSymbolTable()->resolve("main");

  if (!holds_alternative<shared_ptr<Object>>(main)) {
    // TODO: Throw error if no main function was found.
    cout << "No main :/" << endl;
  } else {
    auto mainMethod = get<shared_ptr<Object>>(main).get();
    // TODO: Dynamic cast to function, and execute
    cout << "Found main! :)" << endl;
  }
}
