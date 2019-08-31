#ifndef MANDA_WORKER_HPP
#define MANDA_WORKER_HPP
#include "module.hpp"
#include "task.hpp"
#include <list>
#include <memory>
#include <vector>

namespace manda::runtime {
class Worker {
private:
  std::vector<std::shared_ptr<Module>> modules;
  std::list<std::shared_ptr<Task>> tasks;
};
} // namespace manda::runtime

#endif
