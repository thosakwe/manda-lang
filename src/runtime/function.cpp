#include "function.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

void Function::print(ostream &out, bool ansiSupported) const {
  auto name = getName();
  ostringstream oss;
  oss << "[Function";
  if (!name.empty()) {
    oss << ": " << name;
  }
  oss << "]";
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << cyan(oss.str());
  }
}
