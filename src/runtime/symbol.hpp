#ifndef MANDA_SYMBOL_HPP
#define MANDA_SYMBOL_HPP
#include "../analysis/expr_ctx.hpp"
#include "type.hpp"

namespace manda::runtime {
enum Visibility { pub, priv };

struct Symbol {
  std::shared_ptr<manda::analysis::ExprCtx> node;
  std::shared_ptr<Type> type;
  Visibility visibility = priv;
};
} // namespace manda::runtime

#endif
