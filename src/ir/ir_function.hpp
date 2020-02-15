#ifndef MANDA_IR_FUNCTION_HPP
#define MANDA_IR_FUNCTION_HPP
#include "../runtime/type.hpp"
#include "ir_instruction.hpp"
#include "ir_parameter.hpp"
#include <memory>
#include <string>
#include <vector>

namespace manda::ir {
struct IRBlock {
  std::string name;
  std::vector<std::unique_ptr<IRInstruction>> instructions;
};

struct IRFunction {
  std::string name;
  std::vector<std::unique_ptr<IRParameter>> parameters;
  std::vector<std::unique_ptr<IRParameter>> locals;
  std::vector<std::shared_ptr<IRBlock>> blocks;
  std::shared_ptr<manda::runtime::Type> returnType;
};
} // namespace manda::ir

#endif
