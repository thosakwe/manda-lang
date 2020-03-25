#include "base_resolver.hpp"

using namespace manda::analysis;
using namespace std;

BaseResolver::BaseResolver() { scopeStack.push(make_shared<Scope>()); }

BaseResolver::BaseResolver(const std::shared_ptr<Scope> &existingScope) {
  scopeStack.push(existingScope);
}

std::shared_ptr<Scope> &BaseResolver::getCurrentScope() {
  return scopeStack.top();
}

void BaseResolver::pushScope() {
  scopeStack.push(getCurrentScope()->createChild());
}

void BaseResolver::popScope() { scopeStack.pop(); }