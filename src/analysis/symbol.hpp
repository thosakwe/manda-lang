#ifndef MANDA_SYMBOL_HPP
#define MANDA_SYMBOL_HPP
#include "expr_ctx.hpp"
#include "type.hpp"
#include <optional>

namespace manda::analysis {
enum Visibility { pub, priv };

struct Symbol {
  std::shared_ptr<manda::analysis::BaseCtx> node;
  std::shared_ptr<Type> type;

  Visibility visibility = priv;

  std::optional<bool> boolValue;
  std::optional<char> charValue;
  std::optional<int64_t> intValue;
  std::optional<std::string> stringValue;
  std::shared_ptr<Type> typeValue;

  [[nodiscard]] std::shared_ptr<manda::analysis::ExprCtx> nodeAsExpr() const;
  [[nodiscard]] std::shared_ptr<manda::analysis::TypeCtx> nodeAsType() const;
};
} // namespace manda::analysis

#endif
