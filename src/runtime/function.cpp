#include "function.hpp"
manda::runtime::Function::Function(std::string name,
                                   const manda::analysis::FnDeclExprCtx &node)
    : name(std::move(name)), node(node) {}

const std::string &manda::runtime::Function::getName() const { return name; }
const manda::analysis::FnDeclExprCtx &manda::runtime::Function::getNode() {
  return node;
}
