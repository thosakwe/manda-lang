#ifndef MANDA_TYPE_RESOLVER_HPP
#define MANDA_TYPE_RESOLVER_HPP
#include "../analysis/type_ctx.hpp"
#include "interpreter.hpp"
#include "symbol_table.hpp"
#include "type.hpp"
#include <memory>

namespace manda::runtime {
class TypeResolver : public manda::analysis::TypeVisitor {
public:
  TypeResolver(Interpreter &interpreter, std::shared_ptr<SymbolTable> scope);
  void visitTypeRef(const analysis::TypeRefCtx &ctx) override;
  [[nodiscard]] const std::shared_ptr<Type> &getLastType() const;

private:
  Interpreter &interpreter;
  std::shared_ptr<SymbolTable> scope;
  std::shared_ptr<Type> lastType;
};
} // namespace manda::runtime

#endif
