#include "tuple.hpp"
#include "ansi_printer.hpp"
#include "jit_compiled_function.hpp"
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

jit_value TupleType::boxRawValue(JitCompiledFunction &fn,
                                 const jit_value &rawValue) {
  // TODO: Dispose of the existing object??
  // We eventually call Tuple* box(Object** items, unsigned long size);
  // So, we allocate a region of memory big enough for all the Object*
  // pointers.
  auto jitType = toJitType();
  auto voidPtrSize = jit_type_get_size(jit_type_void_ptr);
  auto blobSize = items.size() * voidPtrSize;
  auto blob = fn.insn_malloc(blobSize);
  auto numFields = jit_type_num_fields(jitType);
  // TODO: Should the child pointers be GC-marked?
  // Next, for each child, call the corresponding boxRawValue, to get a
  // pointer.
  for (unsigned int i = 0; i < numFields; i++) {
    // Load the value at the offset.
    auto offset = jit_type_get_offset(jitType, i);
    auto itemType = items[i]->toJitType();
    auto value = fn.insn_load_relative(rawValue, offset, itemType);
    // Box the value into a pointer.
    auto boxedValue = items[i]->boxRawValue(fn, value);
    // Next, just write that pointer to the correct offset.
    fn.insn_store_relative(blob, i * voidPtrSize, boxedValue);
  }
  // Now that all the pointers have been copied, we invoke Tuple::box.
  jit_type_t params[2] = {jit_type_void_ptr, jit_type_sys_ulong};
  auto sig =
      jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, params, 2, 0);
  jit_value_t args[2] = {
      blob.raw(),
      fn.new_constant((jit_ulong)items.size(), jit_type_sys_ulong).raw()};
  return fn.insn_call_native("manda_box_tuple", (void *)&box, sig, args, 2, 0);
}

Tuple *TupleType::box(Object **items, unsigned long size) {
  auto out = new Tuple;
  for (unsigned long i = 0; i < size; i++) {
    out->getItems().emplace_back(items[i]);
  }
  return out;
}
