#include "array.hpp"
#include "array_type.hpp"
#include <iostream>

using namespace manda::runtime;
using namespace std;

Array::Array(shared_ptr<Type> innerType) : innerType(move(innerType)) {}

const shared_ptr<Type> &Array::getInnerType() const { return innerType; }

const vector<shared_ptr<Object>> &Array::getItems() const { return items; }

vector<shared_ptr<Object>> &Array::getItems() { return items; }

void Array::print(ostream &out, bool ansiSupported) const {
  out << "[";
  for (unsigned long i = 0; i < items.size(); i++) {
    if (i > 0) {
      out << ", ";
    }
    items[i]->print(out, ansiSupported);
  }
  out << "]";
}

shared_ptr<Type> Array::getType(Interpreter &interpreter) const {
  return make_shared<ArrayType>(innerType);
}
