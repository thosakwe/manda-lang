#include "indenting_printer.hpp"

using namespace manda::analysis;
using namespace std;

IndentingPrinter::IndentingPrinter(ostream &out) : out(out) { level = 0; }

void IndentingPrinter::indent() { level++; }

void IndentingPrinter::outdent() {
  if (level > 0)
    level--;
}

ostream &IndentingPrinter::print() {
  for (unsigned long i = 0; i < level; i++) {
    out << "  ";
  }
  out << "- ";
  return out;
}
