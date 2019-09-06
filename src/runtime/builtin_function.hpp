#ifndef MANDA_BUILTIN_FUNCTION_HPP
#define MANDA_BUILTIN_FUNCTION_HPP
#include "function.hpp"
#include <functional>

namespace manda::runtime {
struct BuiltinFunction : public Function {
  std::string name;
  std::vector<Parameter> parameters;
  std::function<std::shared_ptr<Object>(
      Interpreter &, std::shared_ptr<Object> &,
      const std::vector<std::shared_ptr<Object>> &)>
      fn;
  const std::string &getName() const override;
  const std::vector<Parameter> &getParameters() const override;
  std::shared_ptr<Object>
  invoke(Interpreter &interpreter, std::shared_ptr<Object> &thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const override;
  void print(std::ostream &out, bool ansiSupported) const override;
};
} // namespace manda::runtime

#endif
