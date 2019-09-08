#include "tuple.hpp"
#include "ansi_printer.hpp"
#include <exception>
#include <iostream>
#include <sstream>

using namespace manda::runtime;
using namespace std;

vector<shared_ptr<Object>> &manda::runtime::Tuple::getItems() { return items; }

void Tuple::print(ostream &out, bool ansiSupported) const {
  ostringstream oss;
  oss << "(";
  for (unsigned long i = 0; i < items.size(); i++) {
    if (i > 0) {
      oss << ", ";
    }
    items[i]->print(oss, ansiSupported);
  }
  oss << ")";
  out << oss.str();
}

shared_ptr<Type> Tuple::getType(Interpreter &interpreter) const {
  vector<shared_ptr<Type>> itemTypes;
  for (auto &item : items) {
    itemTypes.push_back(item->getType(interpreter));
  }
  return make_shared<TupleType>(itemTypes);
}

TupleType::TupleType(std::vector<std::shared_ptr<Type>> items)
    : items(move(items)) {}

string TupleType::getName() const {
  ostringstream oss;
  oss << "(";
  for (unsigned long i = 0; i < items.size(); i++) {
    if (i > 0) {
      oss << ", ";
    }
    oss << items[i]->getName();
  }
  oss << ")";
  return oss.str();
}

jit_type_t TupleType::toJitType() const {
  // A Manda tuple can be compiled to a simple struct,
  // where each field corresponds to a child's type.
  vector<jit_type_t> fields;
  for (auto &item : items) {
    fields.push_back(item->toJitType());
  }
  return jit_type_create_struct(fields.data(), items.size(), 0);
}

shared_ptr<manda::runtime::Object>
TupleType::deserialize(Interpreter &interpreter, void *ptr) {
  // Deserialize each object, in turn.
  auto *p = static_cast<uint8_t *>(ptr);
  auto out = make_shared<Tuple>();
  auto jitType = toJitType();
  unsigned int i = 0;
  for (auto &item : items) {
    auto *objPtr = p + (jit_type_get_offset(jitType, i++));
    //    auto *dbl = (jit_float64 *) objPtr;
    //    cout << "p: " << (void *)p << endl;
    //    cout << "offset (" << (i - 1) << "): " << (void *)objPtr << endl;
    //    cout << "dbl: " << *dbl << endl;
    out->getItems().push_back(item->deserialize(interpreter, (void *)objPtr));
    //    out->getItems().push_back(item->deserialize(interpreter, (void *)p));
    //    p += jit_type_get_size(item->toJitType());
  }
  return out;
}
