#ifndef MANDA_LOCATION_HPP
#define MANDA_LOCATION_HPP
#include <ostream>
#include <string>

namespace manda::analysis {
struct Position {
  std::string filename;
  unsigned long offset, line, column;
  friend std::ostream &operator<<(std::ostream &out, const Position &a);
};

struct Location {
  Position begin, end;
  friend std::ostream &operator<<(std::ostream &out, const Location &a);
  friend Location operator+(const Location &a, const Location &b);
};
} // namespace manda::analysis

#endif