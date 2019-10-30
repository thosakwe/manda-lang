#ifndef MANDA_INDENTING_PRINTER_HPP
#define MANDA_INDENTING_PRINTER_HPP
#include <ostream>

namespace manda::analysis {
class IndentingPrinter {
public:
  explicit IndentingPrinter(std::ostream &out);
protected:
  std::ostream &out;
  unsigned long level;
  void indent();
  void outdent();
  std::ostream &print();
};
} // namespace manda::analysis

#endif
