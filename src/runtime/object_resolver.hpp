#ifndef MANDA_OBJECT_RESOLVER_HPP
#define MANDA_OBJECT_RESOLVER_HPP
#include "../analysis/expr_ctx.hpp"
#include "interpreter.hpp"
#include "object.hpp"
#include "symbol_table.hpp"
#include <memory>

namespace manda::runtime {
class ObjectResolver {
public:
  ObjectResolver(Interpreter &interpreter, std::shared_ptr<SymbolTable> scope);
  [[nodiscard]] const std::shared_ptr<Object> &getLastObject() const;

private:
  Interpreter &interpreter;
  std::shared_ptr<SymbolTable> scope;
  std::shared_ptr<Object> lastObject;
};
} // namespace manda::runtime

#endif
