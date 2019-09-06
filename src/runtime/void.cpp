#include "void.hpp"
#include "ansi_printer.hpp"

using namespace manda::runtime;
using namespace std;

void Void::print(ostream &out, bool ansiSupported) const {
  if (!ansiSupported) {
    out << "<void>";
  } else {
    out << gray("<void>");
  }
}
