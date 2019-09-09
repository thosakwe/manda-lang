#include "base_resolver.hpp"

using namespace manda::runtime;
using namespace std;

BaseResolver::BaseResolver() { scopeStack.push(UnifiedScope()); }

BaseResolver::BaseResolver(const UnifiedScope &existingScope) {
  scopeStack.push(existingScope);
}

UnifiedScope &BaseResolver::getCurrentScope() { return scopeStack.top(); }

void BaseResolver::pushScope() {
  scopeStack.push(getCurrentScope().createChild());
}

void BaseResolver::popScope() { scopeStack.pop(); }

std::shared_ptr<GenericScope<std::shared_ptr<Type>>> &
BaseResolver::getTypeScope() {
  return getCurrentScope().typeScope;
}

std::shared_ptr<GenericScope<ObjectOrType>> &BaseResolver::getRuntimeScope() {
  return getCurrentScope().runtimeScope;
}

std::shared_ptr<GenericScope<jit_value>> &BaseResolver::getJitScope() {
  return getCurrentScope().jitScope;
}
