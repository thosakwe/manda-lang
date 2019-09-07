#ifndef MANDA_AST_FUNCTION_HPP
#define MANDA_AST_FUNCTION_HPP
#include "../analysis/expr_ctx.hpp"
#include "function.hpp"
#include "object.hpp"

namespace manda::runtime {
class AstFunction : public Function {
public:
  // Copy the original node...
  // TODO: Should this be a reference?
  explicit AstFunction(manda::analysis::FnDeclExprCtx node);
  const manda::analysis::FnDeclExprCtx &getNode();

  [[nodiscard]] const std::string &getName() const override;

  [[nodiscard]] const std::vector<Parameter> &getParameters() const override;

  std::shared_ptr<Object>
  invoke(Interpreter &interpreter, const manda::analysis::Location &location,
         std::shared_ptr<Object> &thisObject,
         const std::vector<std::shared_ptr<Object>> &args) const override;

private:
  std::string name;
  // TODO: Should this instead be a shared_ptr???
  const manda::analysis::FnDeclExprCtx &node;
  // TODO: Get parameters from actual node
  std::vector<Parameter> parameters;
};
} // namespace manda::runtime

#endif
