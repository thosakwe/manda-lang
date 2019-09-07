#include "ast_function.hpp"
#include "void.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

AstFunction::AstFunction(const FnDeclExprCtx &node)
    : name(node.name), node(node) {}

AstFunction::AstFunction(const manda::analysis::FnDeclExprCtx &node,
                         std::vector<Parameter> parameters)
    : name(node.name), node(node), parameters(move(parameters)) {}

const string &AstFunction::getName() const { return name; }

const FnDeclExprCtx &AstFunction::getNode() { return node; }

const vector<Parameter> &AstFunction::getParameters() const {
  return parameters;
}

shared_ptr<Object>
AstFunction::invoke(Interpreter &interpreter, const Location &location,
                    shared_ptr<Object> &thisObject,
                    const vector<shared_ptr<Object>> &args) const {
  // TODO: Invoke AstFunction
  return make_shared<Void>();
}
