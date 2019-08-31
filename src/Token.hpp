#ifndef MANDA_TOKEN_HPP
#define MANDA_TOKEN_HPP

#include <cstdint>
#include <ostream>
#include <string>

namespace w {
struct SourceLocation {
  std::string filename;
  uint64_t line, col;

  friend std::ostream &operator<<(std::ostream &stream,
                                  const SourceLocation &location);
};

struct Token {
  enum TokenType {
    EQUALS,
    ID,
    TYPE
  };

  TokenType type;
  SourceLocation location;
  std::string text;
};
}

#endif
