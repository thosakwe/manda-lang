#ifndef MANDA_LOCATION_HPP
#define MANDA_LOCATION_HPP
#include <ostream>
#include <string>

namespace manda::analysis {
struct Location {
  std::string filename;
  unsigned long line, column;
  friend std::ostream &operator<<(std::ostream &out, const Location &a);
};
} // namespace manda::analysis

#endif