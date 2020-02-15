#ifndef MANDA_WORKER_HPP
#define MANDA_WORKER_HPP

#include "../analysis/ast.hpp"
#include "module.hpp"
#include "task.hpp"
#include "vm_options.hpp"
#include <list>
#include <memory>
#include <vector>

namespace manda::runtime {
class Worker : public std::enable_shared_from_this<Worker> {
public:
  explicit Worker(VMOptions options);
  void executeProgram(std::shared_ptr<analysis::CompilationUnitCtx> &ctx);
  [[nodiscard]] int getExitCode() const;

  bool isAlive() const;

  const std::shared_ptr<Object> &getResult() const;

  static void runWorker(const std::shared_ptr<Worker> &worker,
                        const VMOptions &options);

private:
  int exitCode = 0;
  bool dead;
  std::vector<std::shared_ptr<Module>> modules;
  VMOptions options;
  std::list<std::unique_ptr<Task>> tasks;
  std::shared_ptr<Object> result;
};
} // namespace manda::runtime

#endif
