#ifndef MANDA_FUNCTION_TYPE_HPP
#define MANDA_FUNCTION_TYPE_HPP
#include "location.hpp"
#include "symbol.hpp"
#include "type.hpp"
#include <string>
#include <vector>

namespace manda::analysis {
struct Parameter {
  manda::analysis::Location location;
  std::string name;
  std::shared_ptr<Type> type;
  std::optional<Symbol> defaultValue;
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
} // namespace manda::analysis

#endif
