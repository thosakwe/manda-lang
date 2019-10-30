#ifndef MANDA_IR_PARAMETER_HPP
#define MANDA_IR_PARAMETER_HPP
#include "../runtime/type.hpp"
#include <string>

namespace manda::ir {
class IRParameter {
  std::string name;
  std::shared_ptr<manda::runtime::Type> type;
};
} // namespace manda::ir

#endif
