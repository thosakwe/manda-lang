#ifndef MANDA_SYMBOL_HPP
#define MANDA_SYMBOL_HPP
#include "expr_ctx.hpp"
#include "type.hpp"
#include <optional>

namespace manda::analysis {
enum Visibility { pub, priv };

struct SymbolUsage {
  enum SymbolUsageType { get, set };
  Location location;
  SymbolUsageType type = get;
};

struct Symbol {
  Location location;
  std::shared_ptr<Type> type;
  Visibility visibility = priv;

  std::shared_ptr<ExprCtx> exprCtx;

  std::shared_ptr<TypeCtx> typeCtx;

  std::vector<SymbolUsage> usages;

  std::optional<bool> boolValue;
  std::optional<char> charValue;
  std::optional<int64_t> intValue;
  std::optional<std::string> stringValue;
  std::shared_ptr<Type> typeValue;
};
} // namespace manda::analysis

#endif
