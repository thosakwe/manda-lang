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
