#include "type_ctx.hpp"

using namespace manda::analysis;
using namespace std;

TypeRefCtx::TypeRefCtx(const Location &l, std::string name)
    : name(std::move(name)) {
  location = l;
}

void TypeRefCtx::accept(TypeVisitor &visitor) const {
  visitor.visitTypeRef(*this);
}

unique_ptr<TypeCtx> TypeCtx::cloneToUniquePointer() const {
  return unique_ptr<TypeCtx>(clone());
}

TypeRefCtx *TypeRefCtx::clone() const { return new TypeRefCtx(location, name); }
