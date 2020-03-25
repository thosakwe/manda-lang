#ifndef MANDA_RUNTIME_BASE_RESOLVER_HPP
#define MANDA_RUNTIME_BASE_RESOLVER_HPP
#include "scope.hpp"
#include <stack>

namespace manda::analysis {
class BaseResolver {
public:
  BaseResolver();
  explicit BaseResolver(const std::shared_ptr<Scope> &existingScope);

protected:
  std::stack<std::shared_ptr<Scope>> scopeStack;
  std::shared_ptr<Scope> &getCurrentScope();
  void pushScope();
  void popScope();
};
} // namespace manda::analysis

#endif
