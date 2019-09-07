#include "ast_function.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

AstFunction::AstFunction(const FnDeclExprCtx &node)
    : name(node.name), node(node) {}

const string &AstFunction::getName() const { return name; }

const FnDeclExprCtx &AstFunction::getNode() { return node; }
