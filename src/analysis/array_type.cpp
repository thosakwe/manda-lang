#include "array_type.hpp"

using namespace manda::analysis;
using namespace std;

ArrayType::ArrayType(shared_ptr<Type> innerType) : innerType(move(innerType)) {}

const shared_ptr<Type> &ArrayType::getInnerType() const { return innerType; }

string ArrayType::getName() const { return innerType->getName() + "[]"; }
