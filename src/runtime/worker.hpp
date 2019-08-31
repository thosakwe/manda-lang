#ifndef MANDA_WORKER_HPP
#define MANDA_WORKER_HPP
#include "task.hpp"
#include <list>
#include <memory>

namespace manda {
namespace runtime {
class Worker {
private:
  std::list<std::shared_ptr<Task>> tasks;
};
} // namespace runtime
} // namespace manda

#endif
