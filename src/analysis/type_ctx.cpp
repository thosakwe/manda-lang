#include "type_ctx.hpp"

using namespace manda::analysis;
using namespace std;

void TypeRefCtx::accept(TypeVisitor &visitor) const {
  visitor.visitTypeRef(*this);
}
