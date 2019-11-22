#include "array.hpp"

using namespace manda::runtime;
using namespace std;

Array::Array(shared_ptr<Type> innerType) : innerType(std::move(innerType)) {}

const shared_ptr<Type> &Array::getInnerType() const { return innerType; }

const shared_ptr<Object> &Array::getItems() const { return items; }

std::shared_ptr<Object> &Array::getItems() { return items; }
