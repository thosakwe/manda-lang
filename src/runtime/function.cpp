#include "function.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

void Function::print(ostream &out, bool ansiSupported) const {
  auto &name = getName();
  auto &params = getParameters();
  ostringstream oss;
  oss << "[Function";
  if (!name.empty()) {
    oss << ": " << name;
  }
  if (!params.empty()) {
    oss << "(";
    for (unsigned long i = 0; i < params.size(); i++) {
      auto &param = params[i];
      if (i > 0) {
        oss << ", ";
      }
      oss << param.name;
      // TODO: Add print to Type, so they can printed here
    }
    oss << ")";
  }
  oss << "]";
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << cyan(oss.str());
  }
}
