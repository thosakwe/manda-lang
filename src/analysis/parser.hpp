#ifndef MANDA_PARSER_HPP
#define MANDA_PARSER_HPP
#include "ast.hpp"
#include "scanner.hpp"
#include "token.hpp"
typedef union YYSTYPE YYSTYPE;
#include <vector>

namespace manda::analysis {
class Parser {
public:
  explicit Parser(const Scanner &tokens);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();
  int yylex(YYSTYPE *);
  void yyerror(const std::string &message);
  friend int yylex(YYSTYPE *, Parser *parser);
  friend void yyerror(Parser *, const char *);
  Token lastToken;

private:
  bool next(Token::TokenType type);
  bool anyNext(const std::vector<Token::TokenType> &types);
  std::vector<Token>::const_iterator it;
  const std::vector<Token> &tokens;
};
} // namespace manda::analysis

#endif
