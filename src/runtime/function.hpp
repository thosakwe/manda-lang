#ifndef MANDA_FUNCTION_HPP
#define MANDA_FUNCTION_HPP
#include "../analysis/location.hpp"
#include "interpreter.hpp"
#include "object.hpp"
#include <string>
#include <vector>

namespace manda::runtime {
struct Parameter {
  manda::analysis::Location location;
  std::string name;
  std::shared_ptr<Type> type;
};

class Function : public Object {
public:
  [[nodiscard]] virtual const std::string &getName() const = 0;
  [[nodiscard]] virtual const std::vector<Parameter> &getParameters() const = 0;
  [[nodiscard]] virtual std::shared_ptr<Object>
  invoke(Interpreter &interpreter, std::shared_ptr<Object> thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const = 0;
};
} // namespace manda::runtime

#endif
