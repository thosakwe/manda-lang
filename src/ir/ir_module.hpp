#ifndef MANDA_IR_MODULE_HPP
#define MANDA_IR_MODULE_HPP
#include "ir_function.hpp"
#include <memory>
#include <string>
#include <vector>

namespace manda::ir {
struct IRModule {
  std::string name;
  std::shared_ptr<IRModule> parent;
  std::vector<std::shared_ptr<IRFunction>> functions;
};
} // namespace manda::ir

#endif
