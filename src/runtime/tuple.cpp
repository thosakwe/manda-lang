#include "tuple.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

vector<shared_ptr<Object>> &manda::runtime::Tuple::getItems() { return items; }

void Tuple::print(ostream &out, bool ansiSupported) const {
  ostringstream oss;
  oss << "(";
  for (unsigned long i = 0; i < items.size(); i++) {
    if (i > 0) {
      oss << ", ";
    }
    items[i]->print(oss, ansiSupported);
  }
  oss << ")";
  out << oss.str();
}
