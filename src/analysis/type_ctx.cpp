#include "type_ctx.hpp"

using namespace manda::analysis;
using namespace std;

void TypeRefCtx::accept(TypeVisitor &visitor) { visitor.visitTypeRef(*this); }

unique_ptr<TypeCtx> TypeCtx::cloneToUniquePointer() const {
  return unique_ptr<TypeCtx>(clone());
}

TypeRefCtx *TypeRefCtx::clone() const { return new TypeRefCtx(location, name); }
