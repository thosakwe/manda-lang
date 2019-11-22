#include "array_type.hpp"

using namespace manda::runtime;
using namespace std;

ArrayType::ArrayType(shared_ptr<Type> innerType) : innerType(move(innerType)) {}

const shared_ptr<Type> &ArrayType::getInnerType() const { return innerType; }

string ArrayType::getName() const { return innerType->getName() + "[]"; }

jit_type_t ArrayType::toJitType() const {
  // A Manda array is really a struct like this:
  // {
  //    uint64_t length;
  //    T* data = nullptr;
  // }
  jit_type_t fields[2];
  fields[0] = jit_type_sys_ulonglong;
  fields[1] = jit_type_create_pointer(innerType->toJitType(), 0);
  return jit_type_create_struct(fields, 2, 0);
}

shared_ptr<Object> ArrayType::deserialize(Interpreter &interpreter, void *ptr) {

}
