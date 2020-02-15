#ifndef MANDA_TASK_HPP
#define MANDA_TASK_HPP
#include "../analysis/ast.hpp"
#include "interpreter.hpp"
#include "object.hpp"
#include "vm_options.hpp"

namespace manda::runtime {
class Interpreter;
class Worker;

class Task {
public:
  Task(Worker &worker,
       std::shared_ptr<manda::analysis::CompilationUnitCtx> compilationUnit,
       std::shared_ptr<Interpreter> interpreter,
       manda::analysis::Location startLocation,
       std::shared_ptr<Object> entryPoint, std::shared_ptr<Object> thisObject,
       std::vector<std::shared_ptr<Object>> args);
  [[nodiscard]] bool isAlive() const;
  [[nodiscard]] const std::shared_ptr<Object> &getResult() const;
  void begin(const VMOptions &options);
  void resume(const VMOptions &options);

private:
  Worker &worker;
  std::shared_ptr<manda::analysis::CompilationUnitCtx> compilationUnit;
  std::shared_ptr<Object> result;
  std::shared_ptr<Interpreter> interpreter;
  manda::analysis::Location startLocation;
  std::shared_ptr<Object> entryPoint;
  std::shared_ptr<Object> thisObject;
  std::vector<std::shared_ptr<Object>> args;
  bool dead = false, started = false;
};
} // namespace manda::runtime

#endif
