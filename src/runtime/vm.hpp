#ifndef MANDA_VM_HPP
#define MANDA_VM_HPP
#include "vm_options.hpp"
#include "worker.hpp"
#include <list>
#include <memory>

namespace manda::runtime {
class VM {
public:
  explicit VM(VMOptions options);
  void addWorker(const std::shared_ptr<Worker> &worker);
  int run();

private:
  VMOptions options;
  std::list<std::shared_ptr<Worker>> workers;
};
} // namespace manda::runtime

#endif
