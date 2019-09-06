#include "number.hpp"
#include "ansi_printer.hpp"
#include <iomanip>
#include <string>

using namespace std;

manda::runtime::Number::Number(double value) : value(value) {}

double manda::runtime::Number::getValue() const { return value; }

void manda::runtime::Number::print(ostream &out, bool ansiSupported) const {
  // Remove trailing zeros.
  auto str = to_string(value);
  str.erase(str.find_last_not_of('0') + 1, string::npos);
  out << fixed;
  if (!ansiSupported) {
    out << str;
  } else {
    out << yellow(str);
  }
  out << scientific;
}
