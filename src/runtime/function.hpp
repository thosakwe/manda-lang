#ifndef MANDA_FUNCTION_HPP
#define MANDA_FUNCTION_HPP
#include "../analysis/location.hpp"
#include "interpreter.hpp"
#include <string>
#include <vector>

namespace manda::runtime {
struct Parameter {
  manda::analysis::Location location;
  std::string name;
  std::shared_ptr<Type> type;
};

class FunctionType : public Type {
public:
  FunctionType(std::vector<Parameter> parameters,
               std::shared_ptr<Type> returnType);
  [[nodiscard]] std::string getName() const override;
  [[nodiscard]] const std::vector<Parameter> &getParameters() const;
  [[nodiscard]] const std::shared_ptr<Type> &getReturnType() const;

private:
  std::vector<Parameter> parameters;
  std::shared_ptr<Type> returnType;
};
} // namespace manda::runtime

#endif
