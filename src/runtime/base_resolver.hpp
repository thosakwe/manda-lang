#ifndef MANDA_RUNTIME_BASE_RESOLVER_HPP
#define MANDA_RUNTIME_BASE_RESOLVER_HPP
#include "unified_scope.hpp"
#include <stack>

namespace manda::runtime {
class BaseResolver {
public:
  BaseResolver();

protected:
  std::stack<UnifiedScope> scopeStack;
  UnifiedScope &getCurrentScope();
  std::shared_ptr<GenericScope<std::shared_ptr<Type>>> &getTypeScope();
  std::shared_ptr<GenericScope<Symbol>> &getRuntimeScope();
  std::shared_ptr<GenericScope<jit_value>> &getJitScope();
  void pushScope();
  void popScope();
};
} // namespace manda::runtime

#endif
