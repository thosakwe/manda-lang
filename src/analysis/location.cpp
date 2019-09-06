#include "location.hpp"

std::ostream &manda::analysis::operator<<(std::ostream &out,
                                          const manda::analysis::Location &a) {
  out << a.filename << ':' << a.line << ':' << a.column;
  return out;
}
