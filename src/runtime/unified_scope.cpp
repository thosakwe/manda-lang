#include "unified_scope.hpp"

using namespace manda::runtime;
using namespace std;

UnifiedScope::UnifiedScope() {
  runtimeScope = make_shared<GenericScope<Symbol>>();
  jitScope = make_shared<GenericScope<jit_value>>();
  typeScope = make_shared<GenericScope<shared_ptr<Type>>>();
}

UnifiedScope::UnifiedScope(shared_ptr<GenericScope<Symbol>> runtimeScope,
                           shared_ptr<GenericScope<jit_value>> jitScope,
                           shared_ptr<GenericScope<shared_ptr<Type>>> typeScope)
    : runtimeScope(move(runtimeScope)), jitScope(move(jitScope)),
      typeScope(move(typeScope)) {}

UnifiedScope UnifiedScope::createChild() const {
  return {runtimeScope->createChild(), jitScope->createChild(),
          typeScope->createChild()};
}
