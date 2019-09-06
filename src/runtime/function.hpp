#ifndef MANDA_FUNCTION_HPP
#define MANDA_FUNCTION_HPP
#include "../analysis/expr_ctx.hpp"
#include "object.hpp"

namespace manda::runtime {
class Function : public Object {
public:
  Function(std::string name, const manda::analysis::FnDeclExprCtx &node);
  const std::string &getName() const;
  const manda::analysis::FnDeclExprCtx &getNode();

private:
  std::string name;
  const manda::analysis::FnDeclExprCtx &node;
};
} // namespace manda::runtime

#endif
