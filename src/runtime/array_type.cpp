#include "array_type.hpp"
#include "array.hpp"

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
  // This is probably pretty unsafe, lmao
  auto *p = (uint8_t*) ptr;
  auto length = *((uint64_t *)p);
  auto itemSize = jit_type_get_size(innerType->toJitType());
  auto out = make_shared<Array>(innerType);
  p += sizeof(length);
  for (uint64_t i = 0; i < length; i++) {
    auto object = innerType->deserialize(interpreter, p);
    out->getItems().push_back(object);
    p += itemSize;
  }
  return out;
}

jit_value ArrayType::boxRawValue(JitCompiledFunction &fn,
                                 const jit_value &rawValue) {
  // TODO: Box value!!!
  return jit_value();
}