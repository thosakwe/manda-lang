#ifndef MANDA_AST_FUNCTION_HPP
#define MANDA_AST_FUNCTION_HPP
#include "../analysis/expr_ctx.hpp"
#include "object.hpp"

namespace manda::runtime {
class AstFunction : public Object {
public:
  AstFunction(std::string name, const manda::analysis::FnDeclExprCtx &node);
  [[nodiscard]] const std::string &getName() const;
  const manda::analysis::FnDeclExprCtx &getNode();

private:
  std::string name;
  // TODO: Should this instead be a shared_ptr???
  const manda::analysis::FnDeclExprCtx &node;
};
} // namespace manda::runtime

#endif
