#ifndef MANDA_WORKER_HPP
#define MANDA_WORKER_HPP

#include "../analysis/ast.hpp"
#include "module.hpp"
#include "task.hpp"
#include <list>
#include <memory>
#include <vector>

namespace manda::runtime {
class Worker : public std::enable_shared_from_this<Worker> {
public:
  void loadCompilationUnit(std::shared_ptr<analysis::CompilationUnitCtx> &ctx);

private:
  std::vector<std::shared_ptr<Module>> modules;
  std::list<std::shared_ptr<Task>> tasks;
};
} // namespace manda::runtime

#endif
