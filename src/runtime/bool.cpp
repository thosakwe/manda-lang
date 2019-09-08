#include "bool.hpp"
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

const shared_ptr<Type> &Bool::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().boolType;
}

string BoolType::getName() const { return "Bool"; }
