#include "vm.hpp"

using namespace manda::runtime;
using namespace std;

VM::VM(VMOptions options) : options(std::move(options)) {}

void VM::addWorker(const shared_ptr<Worker> &worker) {
  // TODO: Implement this
}

int VM::run() {
  // TODO: Implement this
  return 0;
}
