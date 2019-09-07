#ifndef MANDA_TOKEN_HPP
#define MANDA_TOKEN_HPP
#include "location.hpp"
#include <ostream>
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
    COMMA,
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
    FALSE,
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
    TRUE,
    TYPE,
    WITH,
    VAR,
    VOID,
    ID,
    NUMBER,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    TEXT,
    HEX_ESCAPE,
    UNICODE_ESCAPE,
    QUOTE_ESCAPE,
    ARROW
  };

  TokenType type;
  Location location;
  std::string text;
  friend std::ostream &operator<<(std::ostream &out, const Token &a);
};

bool tokenIsLonger(const Token &a, const Token &b);
} // namespace manda::analysis

#endif
