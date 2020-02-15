#include "unified_scope.hpp"

using namespace manda::runtime;
using namespace std;

UnifiedScope::UnifiedScope() {
  runtimeScope = make_shared<RuntimeScope>();
  jitScope = make_shared<JitScope>();
  typeScope = make_shared<TypeScope>();
}

UnifiedScope::UnifiedScope(shared_ptr<RuntimeScope> runtimeScope,
                           shared_ptr<JitScope> jitScope,
                           shared_ptr<TypeScope> typeScope)
    : runtimeScope(move(runtimeScope)), jitScope(move(jitScope)),
      typeScope(move(typeScope)) {}

UnifiedScope UnifiedScope::createChild() const {
  return {runtimeScope->createChild(), jitScope->createChild(),
          typeScope->createChild()};
}

void UnifiedScope::addType(const std::string &name,
                           const std::shared_ptr<Type> &type,
                           bool allowRedefine) {
  runtimeScope->add(name, type, allowRedefine);
  typeScope->add(name, type, allowRedefine);
}
