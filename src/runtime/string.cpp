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
