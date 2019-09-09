#ifndef MANDA_RUNTIME_UNIFIED_SCOPE_HPP
#define MANDA_RUNTIME_UNIFIED_SCOPE_HPP
#include "generic_scope.hpp"
#include "object.hpp"
#include "symbol_table.hpp"
#include "type.hpp"
#include <jit/jit.h>

namespace manda::runtime {
struct UnifiedScope {
  std::shared_ptr<GenericScope<Symbol>> runtimeScope;
  std::shared_ptr<GenericScope<jit_value>> jitScope;
  std::shared_ptr<GenericScope<std::shared_ptr<Type>>> typeScope;

  UnifiedScope();
  UnifiedScope(std::shared_ptr<GenericScope<Symbol>> runtimeScope,
               std::shared_ptr<GenericScope<jit_value>> jitScope,
               std::shared_ptr<GenericScope<std::shared_ptr<Type>>> typeScope);
  UnifiedScope createChild() const;
};
} // namespace manda::runtime

#endif
