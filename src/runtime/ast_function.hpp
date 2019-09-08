#ifndef MANDA_AST_FUNCTION_HPP
#define MANDA_AST_FUNCTION_HPP
#include "../analysis/expr_ctx.hpp"
#include "function.hpp"
#include "object.hpp"
#include "symbol_table.hpp"
#include <memory>

namespace manda::runtime {
class AstFunction : public Function {
public:
  // Copy the original node...
  AstFunction(const manda::analysis::FnDeclExprCtx &node,
              std::shared_ptr<SymbolTable> scope,
              std::shared_ptr<Type> returnType);
  AstFunction(const manda::analysis::FnDeclExprCtx &node,
              std::shared_ptr<SymbolTable> scope,
              std::vector<Parameter> parameters,
              std::shared_ptr<Type> returnType);

  [[nodiscard]] const std::string &getName() const override;

  [[nodiscard]] const std::vector<Parameter> &getParameters() const override;

  std::shared_ptr<Type> getReturnType(Interpreter &interpreter) const override;

  const std::shared_ptr<SymbolTable> &getScope() const;

  const std::unique_ptr<manda::analysis::FnDeclExprCtx> &getNode() const;

  std::shared_ptr<Object>
  invoke(Interpreter &interpreter, const manda::analysis::Location &location,
         std::shared_ptr<Object> &thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const override;

  jit_value acceptForJitCall(JitCompiledFunction &fn,
                        std::vector<jit_value> &arguments) override;

private:
  std::string name;
  std::unique_ptr<manda::analysis::FnDeclExprCtx> node;
  // TODO: Get parameters from actual node
  std::vector<Parameter> parameters;
  std::shared_ptr<Type> returnType;
  std::shared_ptr<SymbolTable> scope;
};
} // namespace manda::runtime

#endif
