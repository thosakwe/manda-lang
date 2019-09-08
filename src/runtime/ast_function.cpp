#include "ast_function.hpp"
#include "object_resolver.hpp"
#include "void.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

AstFunction::AstFunction(const FnDeclExprCtx &node,
                         std::shared_ptr<SymbolTable> scope,
                         shared_ptr<Type> returnType)
    : name(node.name), node(node.clone()), returnType(move(returnType)),
      scope(move(scope)) {}

AstFunction::AstFunction(const manda::analysis::FnDeclExprCtx &node,
                         std::shared_ptr<SymbolTable> scope,
                         std::vector<Parameter> parameters,
                         shared_ptr<Type> returnType)
    : name(node.name), node(node.clone()), parameters(move(parameters)),
      returnType(move(returnType)), scope(move(scope)) {}

const string &AstFunction::getName() const { return name; }

const vector<Parameter> &AstFunction::getParameters() const {
  return parameters;
}

shared_ptr<Object>
AstFunction::invoke(Interpreter &interpreter, const Location &location,
                    shared_ptr<Object> &thisObject,
                    const vector<shared_ptr<Object>> &args) const {
  // TODO: Validate parameters
  // TODO: Inject parameters into scope
  auto childScope = scope->createChild();
  ObjectResolver resolver(interpreter, childScope);
  node->body->accept(resolver);
  return resolver.getLastObject();
}

shared_ptr<Type> AstFunction::getReturnType(Interpreter &interpreter) const {
  return returnType;
}

const unique_ptr<FnDeclExprCtx> &AstFunction::getNode() const { return node; }
