#include "task.hpp"
#include "function.hpp"
#include <utility>

using namespace manda::runtime;
using namespace std;

Task::Task(Worker &worker,
           shared_ptr<manda::analysis::CompilationUnitCtx> compilationUnit,
           Interpreter &interpreter, manda::analysis::Location startLocation,
           shared_ptr<Object> entryPoint, shared_ptr<Object> thisObject,
           vector<shared_ptr<Object>> args)
    : worker(worker), interpreter(move(interpreter)),
      compilationUnit(move(compilationUnit)),
      startLocation(move(startLocation)), entryPoint(move(entryPoint)),
      thisObject(move(thisObject)), args(move(args)) {}

bool manda::runtime::Task::isAlive() const { return !dead; }

const shared_ptr<Object> &manda::runtime::Task::getResult() const {
  return result;
}

void manda::runtime::Task::begin(const manda::runtime::VMOptions &options) {
  // Fire off the entry point.
  // TODO: Handle yields...
  auto *entryFn = dynamic_cast<Function *>(entryPoint.get());
  result = entryFn->invoke(interpreter, startLocation, thisObject, args);
}

void manda::runtime::Task::resume(const manda::runtime::VMOptions &options) {
  if (!started) {
    begin(options);
  } else {
    // TODO: Allow resuming after a yield. Perhaps save state?
  }
}
