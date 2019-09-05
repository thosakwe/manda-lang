#ifndef MANDA_PARSER_HPP
#define MANDA_PARSER_HPP
#include <manda_flex_parser.hpp>
#include "ast.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include <vector>

namespace manda::analysis {
class Parser {
public:
  explicit Parser(const Scanner &tokens);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();
  friend int yylex(YYSTYPE*, Parser* parser);

private:
  bool next(Token::TokenType type);
  bool anyNext(const std::vector<Token::TokenType>& types);
  std::vector<Token>::iterator it;
  const std::vector<Token> &tokens;
};
} // namespace manda::analysis

#endif
