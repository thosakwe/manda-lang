#include "ast_function.hpp"
#include "jit_compiled_function.hpp"
#include "object_resolver.hpp"
#include "void.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

AstFunction::AstFunction(const FnDeclExprCtx &node, UnifiedScope scope,
                         shared_ptr<Type> returnType)
    : name(node.name), node(node.clone()), returnType(move(returnType)),
      scope(move(scope)) {}

AstFunction::AstFunction(const manda::analysis::FnDeclExprCtx &node,
                         UnifiedScope scope, std::vector<Parameter> parameters,
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
  // Invoke the JIT function.
  JitCompiledFunction jitFunction(interpreter, *this);
  // TODO: Validate parameters, and inject parameters into scope.
  //  This is still to-do, as it will require writing parameters directly
  // into memory.
  vector<void *> jitArgs;
  return returnType->applyJitFunction(interpreter, jitArgs, jitFunction);
}

jit_value AstFunction::acceptForJitCall(JitCompiledFunction &fn,
                                        std::vector<jit_value> &arguments) {
  // TODO: Allow calling on-demand here
  return nullptr;
}

shared_ptr<Type> AstFunction::getReturnType(Interpreter &interpreter) const {
  return returnType;
}

const unique_ptr<FnDeclExprCtx> &AstFunction::getNode() const { return node; }

const UnifiedScope &AstFunction::getScope() const { return scope; }
