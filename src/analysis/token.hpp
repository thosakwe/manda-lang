#ifndef MANDA_TOKEN_HPP
#define MANDA_TOKEN_HPP
#include "location.hpp"
#include <cstdint>
#include <ostream>
#include <string>

namespace manda::analysis {
struct Token {
  enum TokenType { EQUALS, ID, TYPE };

  TokenType type;
  Location location;
  std::string text;
};
} // namespace manda::analysis

#endif
