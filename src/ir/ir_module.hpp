#ifndef MANDA_IR_MODULE_HPP
#define MANDA_IR_MODULE_HPP
#include <memory>
#include <string>

namespace manda::ir {
class IRModule {
public:
  explicit IRModule(std::string name);
  IRModule(std::string name, std::shared_ptr<IRModule> parent);
  const std::shared_ptr<IRModule> &getParent() const;
  const std::string &getName() const;

private:
  std::shared_ptr<IRModule> parent;
  std::string name;
};
} // namespace manda::ir

#endif
