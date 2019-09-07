#ifndef MANDA_BUILTIN_FUNCTION_HPP
#define MANDA_BUILTIN_FUNCTION_HPP
#include "function.hpp"
#include <functional>

namespace manda::runtime {
struct BuiltinFunction : public Function {
  BuiltinFunction(std::string name, std::vector<Parameter> parameters,
                  std::function<std::shared_ptr<Object>(
                      Interpreter &, const manda::analysis::Location,
                      std::shared_ptr<Object> &,
                      const std::vector<std::shared_ptr<Object>> &)>
                      fn);
  std::string name;
  std::vector<Parameter> parameters;
  std::function<std::shared_ptr<Object>(
      Interpreter &, const manda::analysis::Location, std::shared_ptr<Object> &,
      const std::vector<std::shared_ptr<Object>> &)>
      fn;
  [[nodiscard]] const std::string &getName() const override;
  [[nodiscard]] const std::vector<Parameter> &getParameters() const override;
  std::shared_ptr<Object>
  invoke(Interpreter &interpreter, const manda::analysis::Location &location,
         std::shared_ptr<Object> &thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const override;
};
} // namespace manda::runtime

#endif
