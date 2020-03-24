#include "void_type.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"

using namespace manda::analysis;
using namespace std;

void Void::print(ostream &out, bool ansiSupported) const {
  if (!ansiSupported) {
    out << "<void>";
  } else {
    out << gray("<void>");
  }
}

shared_ptr<Type> Void::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().voidType;
}

string VoidType::getName() const { return "Void"; }

jit_type_t VoidType::toJitType() const { return jit_type_void; }

shared_ptr<manda::analysis::Object>
VoidType::deserialize(Interpreter &interpreter, void *ptr) {
  return make_shared<Void>();
}

jit_value VoidType::boxRawValue(JitCompiledFunction &fn,
                                const jit_value &rawValue) {
  // TODO: This should logically never happen.
  return rawValue;
}
