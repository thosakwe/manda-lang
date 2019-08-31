#ifndef MANDA_LOCATION_HPP
#define MANDA_LOCATION_HPP
#include <string>

namespace manda::analysis {
struct Location {
  std::string filename;
  unsigned long line, column;
};
} // namespace manda::analysis

#endif
