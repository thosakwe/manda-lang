#include "any_type.hpp"
#include <vector>

using namespace manda::runtime;
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
