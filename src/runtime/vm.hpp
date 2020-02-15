#ifndef MANDA_VM_HPP
#define MANDA_VM_HPP
#include "vm_options.hpp"
#include "worker.hpp"
#include <list>
#include <memory>
#include <thread>

namespace manda::runtime {
class VM {
public:
  explicit VM(VMOptions options);
  void addWorker(const std::shared_ptr<Worker> &worker);
  int run();

private:
  VMOptions options;
  std::list<std::shared_ptr<Worker>> workers;
  std::vector<std::thread> workerThreads;
  bool started = false;
  void startWorker(const std::shared_ptr<Worker> &worker);
};
} // namespace manda::runtime

#endif
