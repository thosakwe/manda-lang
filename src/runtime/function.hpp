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

// TODO: Return types
class Function : public Object {
public:
  [[nodiscard]] virtual const std::string &getName() const = 0;
  [[nodiscard]] virtual const std::vector<Parameter> &getParameters() const = 0;
  [[nodiscard]] virtual std::shared_ptr<Object>
  invoke(Interpreter &interpreter, const manda::analysis::Location &location,
         std::shared_ptr<Object> &thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const = 0;
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;
  virtual std::shared_ptr<Type>
  getReturnType(Interpreter &interpreter) const = 0;
};

class FunctionType : public Type {
public:
  FunctionType(std::vector<Parameter> parameters,
               std::shared_ptr<Type> returnType);
  std::string getName() const override;
  jit_type_t toJitType() const override;

private:
  std::vector<Parameter> parameters;
  std::shared_ptr<Type> returnType;
};
} // namespace manda::runtime

#endif
