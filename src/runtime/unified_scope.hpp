#ifndef MANDA_RUNTIME_UNIFIED_SCOPE_HPP
#define MANDA_RUNTIME_UNIFIED_SCOPE_HPP
#include "generic_scope.hpp"
#include "object.hpp"
#include "symbol_table.hpp"
#include "type.hpp"
#include <jit/jit.h>

namespace manda::runtime {
struct UnifiedScope {
  std::shared_ptr<RuntimeScope> runtimeScope;
  std::shared_ptr<JitScope> jitScope;
  std::shared_ptr<TypeScope> typeScope;

  UnifiedScope();
  UnifiedScope(std::shared_ptr<RuntimeScope> runtimeScope,
               std::shared_ptr<JitScope> jitScope,
               std::shared_ptr<TypeScope> typeScope);
  [[nodiscard]] UnifiedScope createChild() const;
  void addType(const std::string &name, const std::shared_ptr<Type> &type,
               bool allowRedefine = false);
};
} // namespace manda::runtime

#endif
