#include "bool_type.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"

using namespace manda::runtime;
using namespace std;

Bool::Bool(bool value) : value(value) {}

bool Bool::getValue() const { return value; }

void Bool::print(ostream &out, bool ansiSupported) const {
  auto v = value ? "true" : "false";
  if (!ansiSupported) {
    out << v;
  } else {
    out << magenta(v);
  }
}

std::shared_ptr<Type> Bool::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().boolType;
}

string BoolType::getName() const { return "Bool"; }

jit_type_t BoolType::toJitType() const { return jit_type_sys_bool; }

shared_ptr<manda::runtime::Object>
BoolType::deserialize(Interpreter &interpreter, void *ptr) {
  auto asUbyte = *((jit_ubyte *)ptr);
  return make_shared<Bool>(asUbyte == 0x1);
}

jit_value BoolType::boxRawValue(JitCompiledFunction &fn,
                                const jit_value &rawValue) {

  return invokeStaticBoxFunction("manda_box_bool", (void *)&box,
                                 jit_type_sys_bool, fn, rawValue);
}

Bool *BoolType::box(bool value) { return new Bool(value); }
