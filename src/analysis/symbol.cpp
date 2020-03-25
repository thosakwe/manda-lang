#include "symbol.hpp"

using namespace manda::analysis;
using namespace std;

shared_ptr<ExprCtx> Symbol::nodeAsExpr() const {
  return static_pointer_cast<ExprCtx>(node);
}

shared_ptr<TypeCtx> Symbol::nodeAsType() const {
  return static_pointer_cast<TypeCtx>(node);
}
