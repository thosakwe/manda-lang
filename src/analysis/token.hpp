#ifndef MANDA_TOKEN_HPP
#define MANDA_TOKEN_HPP
#include "location.hpp"
#include <cstdint>
#include <ostream>
#include <regex>
#include <string>

namespace manda::analysis {
struct Token {
  enum TokenType { EQUALS, ID, TYPE };

  TokenType type;
  Location location;
  std::smatch match;
  std::string text;
};

bool tokenIsLonger(const Token &a, const Token &b);
} // namespace manda::analysis

#endif
