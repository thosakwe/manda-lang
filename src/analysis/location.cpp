#include "location.hpp"

std::ostream &manda::analysis::operator<<(std::ostream &out,
                                          const manda::analysis::Position &a) {
  out << a.line << ':' << a.column;
  return out;
}

std::ostream &manda::analysis::operator<<(std::ostream &out,
                                          const manda::analysis::Location &a) {
  //  out << a.filename << ':' << a.line << ':' << a.column;
  out << a.filename << ":" << a.begin;
  return out;
}
