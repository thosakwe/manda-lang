#include "location.hpp"

std::ostream &manda::analysis::operator<<(std::ostream &out,
                                          const manda::analysis::Position &a) {
  out << a.filename << a.line << ':' << a.column;
  return out;
}

std::ostream &manda::analysis::operator<<(std::ostream &out,
                                          const manda::analysis::Location &a) {
  return out << a.begin;
}
