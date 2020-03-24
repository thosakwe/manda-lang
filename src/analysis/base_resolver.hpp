#ifndef MANDA_RUNTIME_BASE_RESOLVER_HPP
#define MANDA_RUNTIME_BASE_RESOLVER_HPP
#include "scope.hpp"
#include <stack>

namespace manda::analysis {
class BaseResolver {
public:
  BaseResolver();
  explicit BaseResolver(const Scope &existingScope);

protected:
  std::stack<Scope> scopeStack;
  Scope &getCurrentScope();
  void pushScope();
  void popScope();
};
} // namespace manda::analysis

#endif
