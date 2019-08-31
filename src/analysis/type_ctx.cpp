#include "type_ctx.hpp"

manda::analysis::TypeReferenceCtx::TypeReferenceCtx(std::string &name)
    : name(name) {}

const std::string &manda::analysis::TypeReferenceCtx::getName() const {
  return name;
}
