#include "worker.hpp"
#include "function.hpp"
#include "interpreter.hpp"
#include "module_compiler.hpp"
#include "number.hpp"
#include "string.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Worker::Worker(VMOptions options) : options(std::move(options)) {}

int Worker::getExitCode() const { return exitCode; }

void Worker::executeProgram(shared_ptr<analysis::CompilationUnitCtx> &ctx) {
  // Validate that there is a main...
  auto module = make_shared<Module>(options.inputFile);
  Interpreter interpreter(options, module);
  ModuleCompiler compiler(interpreter, module);
  ctx->accept(compiler);

  auto main = module->getSymbolTable()->resolve("main");
  Location startLocation = {module->getName(), 1, 1};

  if (!main || !holds_alternative<shared_ptr<Object>>(*main)) {
    // TODO: Throw error if no main function was found.
    interpreter.reportError(
        startLocation,
        "No function named \"main\" was defined in the top-level context.");
    dead = true;
    exitCode = 1;
    return;
  } else {
    auto *mainMethod =
        dynamic_cast<Function *>(get<shared_ptr<Object>>(*main).get());
    if (!mainMethod) {
      interpreter.reportError(startLocation,
                              "A symbol \"main\" was defined in the top-level "
                              "context, but it was not a function.");
      dead = true;
      exitCode = 1;
      return;
    } else {
      // TODO: Pass arguments to main, only if it requests them
      shared_ptr<Object> thisObject;
      // Forward command line args
      // TODO: This should be a list
      vector<shared_ptr<Object>> args;
      //      for (auto &s : options.rest) {
      //        args.push_back(make_shared<String>(s));
      //      }

      // Create a new task, that will eventually be started.
      auto task = make_unique<Task>(*this, ctx, interpreter, startLocation,
                                    mainMethod, thisObject, args);
      tasks.push_back(task);
    }
  }
}

bool Worker::isAlive() const { return !dead; }

const std::shared_ptr<Object> &Worker::getResult() const { return result; }

void Worker::runWorker(const std::shared_ptr<Worker> &worker,
                       const VMOptions &options) {
  // TODO: Allow tasks to yield.
  while (!worker->tasks.empty()) {
    auto it = worker->tasks.begin();
    while (it != worker->tasks.end()) {
      auto &task = *it;
      if (task->isAlive()) {
        task->resume(options);
        it++;
      } else {
        // TODO: Multiple results?
        worker->result = task->getResult();
        worker->tasks.erase(it++);
      }
    }
  }
}

//
// void Worker::executeProgram(shared_ptr<CompilationUnitCtx> &ctx) {
//  // TODO: Worker exit codes???
//  // TODO: Do the logic...
//  auto module = make_shared<Module>(options.inputFile);
//  Interpreter interpreter(options, module);
//  ModuleCompiler compiler(interpreter, module);
//  ctx->accept(compiler);
//
//  auto main = module->getSymbolTable()->resolve("main");
//  Location startLocation = {module->getName(), 1, 1};
//
//  if (!main || !holds_alternative<shared_ptr<Object>>(*main)) {
//    // TODO: Throw error if no main function was found.
//    interpreter.reportError(
//        startLocation,
//        "No function named \"main\" was defined in the top-level context.");
//  } else {
//    auto *mainMethod =
//        dynamic_cast<Function *>(get<shared_ptr<Object>>(*main).get());
//    if (!mainMethod) {
//      interpreter.reportError(startLocation,
//                              "A symbol \"main\" was defined in the top-level
//                              " "context, but it was not a function.");
//    } else {
//      // TODO: Pass arguments to main, only if it requests them
//      shared_ptr<Object> thisObject;
//      // Forward command line args
//      vector<shared_ptr<Object>> args;
//      for (auto &s : options.rest) {
//        args.push_back(make_shared<String>(s));
//      }
//      // Set the exit code, if result was a number.
//      auto result =
//          mainMethod->invoke(interpreter, startLocation, thisObject, args);
//      if (options.developerMode) {
//        if (result) {
//          cout << "Result: ";
//          result->print(cout, true);
//          cout << endl;
//        } else {
//          cout << "Result produced null pointer." << endl;
//        }
//      }
//      auto *resultAsNumber = dynamic_cast<Number *>(result.get());
//      if (resultAsNumber) {
//        exitCode = (int)resultAsNumber->getValue();
//      }
//    }
//  }
//}
