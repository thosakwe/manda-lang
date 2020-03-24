#include "any_type.hpp"
#include "object.hpp"
#include <vector>

using namespace manda::analysis;
using namespace std;

string AnyType::getName() const { return "Any"; }

jit_type_t AnyType::toJitType() const {
  // Because the type is unknown at runtime, we can only return a pointer,
  // with some type information attached.
  // struct manda_jit_any {
  //    void *data;
  //    unsigned long long type_id;
  // };
  vector<jit_type_t> fields;
  fields.push_back(jit_type_void_ptr);
  fields.push_back(jit_type_sys_ulonglong);
  return jit_type_create_struct(fields.data(), fields.size(), 0);
}

shared_ptr<manda::analysis::Object>
AnyType::deserialize(Interpreter &interpreter, void *ptr) {
  auto *obj = (Object **)ptr;
  return shared_ptr<Object>(*obj);
}

jit_value AnyType::boxRawValue(JitCompiledFunction &fn,
                               const jit_value &rawValue) {
  // Any objects are already in their final state.
  return rawValue;
}
