#include "type.hpp"

using namespace manda::runtime;
using namespace std;

bool Type::isExactly(const Type &other) { return &other == this; }

bool Type::isAssignableTo(const Type &other) { return isExactly(other); }

bool Type::isAssignableTo(const std::shared_ptr<Type> &other) {
  return isAssignableTo(*other);
}

std::shared_ptr<Type> Type::getParent() const { return nullptr; }

unsigned long Type::getLevel() const { return 0; }