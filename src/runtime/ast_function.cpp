#include "ast_function.hpp"
manda::runtime::AstFunction::AstFunction(std::string name,
                                   const manda::analysis::FnDeclExprCtx &node)
    : name(std::move(name)), node(node) {}

const std::string &manda::runtime::AstFunction::getName() const { return name; }
const manda::analysis::FnDeclExprCtx &manda::runtime::AstFunction::getNode() {
  return node;
}
