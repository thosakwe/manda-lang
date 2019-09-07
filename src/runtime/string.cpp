#include "string.hpp"
#include "ansi_printer.hpp"
#include <iostream>
#include <sstream>

using namespace manda::runtime;
using namespace std;

String::String(std::string s) : value(move(s)) {}

void String::print(ostream &out, bool ansiSupported) const {
  ostringstream oss;
  oss << "\"" << value << "\"";
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << green(oss.str());
  }
}
