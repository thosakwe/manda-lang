#ifndef MANDA_VM_HPP
#define MANDA_VM_HPP
#include "worker.hpp"
#include <list>
#include <memory>

namespace manda::runtime {
class VM {
public:
  void addWorker(const std::shared_ptr<Worker> &worker);
  int run();

private:
  std::list<std::shared_ptr<Worker>> workers;
};
} // namespace manda::runtime

#endif
