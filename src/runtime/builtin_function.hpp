#ifndef MANDA_BUILTIN_FUNCTION_HPP
#define MANDA_BUILTIN_FUNCTION_HPP
#include "function.hpp"
#include <functional>
#include <manda_api.h>

namespace manda::runtime {
struct BuiltinFunction : public Function {
  std::string name;
  std::vector<Parameter> parameters;
  std::shared_ptr<Type> returnType;
  MandaNativeFunction nativeFunction;

  BuiltinFunction(std::string name, std::vector<Parameter> parameters,
                  std::shared_ptr<Type> returnType,
                  MandaNativeFunction nativeFunction);

  [[nodiscard]] const std::string &getName() const override;

  [[nodiscard]] const std::vector<Parameter> &getParameters() const override;

  std::shared_ptr<Type> getReturnType(Interpreter &interpreter) const override;

  std::shared_ptr<Object>
  invoke(Interpreter &interpreter, const manda::analysis::Location &location,
         std::shared_ptr<Object> &thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const override;

  void acceptForJitCall(JitCompiledFunction &function,
                        std::vector<jit_value> &arguments) override;
};
} // namespace manda::runtime

#endif
