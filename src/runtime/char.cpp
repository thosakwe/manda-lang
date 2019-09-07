#include "char.hpp"
#include "ansi_printer.hpp"
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
