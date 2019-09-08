#include "string.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <sstream>

using namespace manda::runtime;
using namespace std;

String::String(std::string s) : value(move(s)) {}

void String::print(ostream &out, bool ansiSupported) const {
  if (!ansiSupported) {
    out << value;
  } else {
    ostringstream oss;
    oss << "\"" << value << "\"";
    out << green(oss.str());
  }
}

shared_ptr<Type> String::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().stringType;
}

string StringType::getName() const { return "String"; }

jit_type_t StringType::toJitType() const {
  // Manda basic strings are just 8-bit lists, so char*
  return jit_type_create_pointer(jit_type_sys_char, 0);
}
