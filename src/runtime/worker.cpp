#include "worker.hpp"
#include "function.hpp"
#include "interpreter.hpp"
#include "module_compiler.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Worker::Worker(VMOptions options) : options(std::move(options)) {}

void Worker::executeProgram(shared_ptr<CompilationUnitCtx> &ctx) {
  // TODO: Worker exit codes???
  // TODO: Do the logic...
  auto module = make_shared<Module>("TODO: File name or something");
  Interpreter interpreter(options, module);
  ModuleCompiler compiler(interpreter, module);
  ctx->accept(compiler);

  auto main = module->getSymbolTable()->resolve("main");
  Location startLocation = {module->getName(), 1, 1};

  if (!holds_alternative<shared_ptr<Object>>(main)) {
    // TODO: Throw error if no main function was found.
    interpreter.reportError(
        startLocation,
        "No function named \"main\" was defined in the top-level context.");
  } else {
    auto *mainMethod =
        dynamic_cast<Function *>(get<shared_ptr<Object>>(main).get());
    if (!mainMethod) {
      interpreter.reportError(startLocation,
                              "A symbol \"main\" was defined in the top-level "
                              "context, but it was not a function.");
    } else {
      // TODO: Dynamic cast to function, and execute
      cout << "Found main! :)" << endl;
    }
  }
}
