#include "type.hpp"

const w_lang::Type* w_lang::Type::getInnermost() const {
  return this;
}
