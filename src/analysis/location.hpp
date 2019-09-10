#ifndef MANDA_LOCATION_HPP
#define MANDA_LOCATION_HPP
#include <ostream>
#include <string>

namespace manda::analysis {
struct Position {
  unsigned long offset, line, column;
  friend std::ostream &operator<<(std::ostream &out, const Position &a);
};

struct Location {
  std::string filename;
  Position begin, end;
  friend std::ostream &operator<<(std::ostream &out, const Location &a);
};
} // namespace manda::analysis

#endif