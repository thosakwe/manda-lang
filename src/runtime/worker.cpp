#include "worker.hpp"
#include "interpreter.hpp"
#include "module_compiler.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Worker::Worker(VMOptions options) : options(std::move(options)) {}

void Worker::executeProgram(shared_ptr<CompilationUnitCtx> &ctx) {
  // TODO: Do the logic...
  auto module = make_shared<Module>("TODO: File name or something");
  Interpreter interpreter(options, module);
  ModuleCompiler compiler(interpreter);
  ctx->accept(compiler);

  auto main = module->getSymbolTable()->resolve("main");

  if (!holds_alternative<shared_ptr<Object>>(main)) {
    // TODO: Throw error if no main function was found.
    cout << "No main :/" << endl;
  } else {
    auto mainMethod = get<shared_ptr<Object>>(main).get();
    // TODO: Dynamic cast to function, and execute
    cout << "Found main! :)" << endl;
  }
}
