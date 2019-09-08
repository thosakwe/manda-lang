#include "char.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include <ostream>
#include <sstream>

using namespace manda::runtime;
using namespace std;

Char::Char(char ch) : value(ch) {}

void Char::print(ostream &out, bool ansiSupported) const {
  ostringstream oss;
  oss << "'" << value << "'";
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << cyan(oss.str());
  }
}

string CharType::getName() const { return "Char"; }

shared_ptr<Type> Char::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().charType;
}

jit_type_t CharType::toJitType() const { return jit_type_sys_char; }
