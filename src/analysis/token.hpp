#ifndef MANDA_TOKEN_HPP
#define MANDA_TOKEN_HPP
#include "location.hpp"
#include <cstdint>
#include <ostream>
#include <regex>
#include <string>

namespace manda::analysis {
struct Token {
  enum TokenType {
    EQUALS,
    LT,
    GT,
    LTE,
    GTE,
    LCURLY,
    RCURLY,
    LPAREN,
    RPAREN,
    COLON,
    LBRACKET,
    RBRACKET,
    TWO_DOTS,
    THREE_DOTS,
    DOT,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    MOD,
    XOR,
    OR,
    AND,
    BOOL_OR,
    BOOL_AND,
    BOOL_EQUALS,
    SEMI,
    AS,
    ELSE,
    EXPORT,
    FOR,
    FINAL,
    FN,
    FROM,
    IF,
    IMPORT,
    MATCH,
    PUB,
    SUM,
    THEN,
    TYPE,
    WITH,
    VAR,
    ID,
    NUMBER,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    TEXT,
    HEX_ESCAPE,
    UNICODE_ESCAPE
  };

  TokenType type;
  Location location;
  std::smatch match;
  std::string text;
};

bool tokenIsLonger(const Token &a, const Token &b);
} // namespace manda::analysis

#endif
