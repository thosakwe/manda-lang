#include "vm.hpp"
#include "number.hpp"
#include <iostream>
#include <vector>

using namespace manda::runtime;
using namespace std;

VM::VM(VMOptions options) : options(std::move(options)) {}

void VM::addWorker(const shared_ptr<Worker> &worker) {
  workers.push_back(worker);
  if (started) {
    startWorker(worker);
  }
}

void VM::startWorker(const std::shared_ptr<Worker> &worker) {
  thread workerThread(Worker::runWorker, worker, options);
  workerThread.detach();
  workerThreads.push_back(move(workerThread));
}

int VM::run() {
  started = true;
  // Start all workers.
  if (options.developerMode) {
    cout << "Workers: " << workers.size() << endl;
  }

  for (auto &worker : workers) {
    startWorker(worker);
  }
  // Loop while all workers are alive.
  std::shared_ptr<Object> result;
  while (!workers.empty()) {
    auto it = workers.begin();
    while (it != workers.end()) {
      auto &worker = *it;
      if (worker->isAlive()) {
        it++;
      } else {
        // TODO: Only allow the result to be set once.
        result = worker->getResult();
        workers.erase(it++);
      }
    }
  }

  // Next, return an exit code, potentially.
  // Set the exit code, if result was a number.
  if (options.developerMode) {
    if (result) {
      cout << "Result: ";
      result->print(cout, true);
      cout << endl;
    } else {
      cout << "Result produced null pointer." << endl;
    }
  }
  auto *resultAsNumber = dynamic_cast<Number *>(result.get());
  if (resultAsNumber) {
    return (int)resultAsNumber->getValue();
  }
  return 0;
}
