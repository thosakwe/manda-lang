#include "array.hpp"

using namespace manda::runtime;
using namespace std;

Array::Array(shared_ptr<Type> innerType) : innerType(move(innerType)) {}

const shared_ptr<Type> &Array::getInnerType() const { return innerType; }

const vector<shared_ptr<Object>> &Array::getItems() const { return items; }

vector<shared_ptr<Object>> &Array::getItems() { return items; }
