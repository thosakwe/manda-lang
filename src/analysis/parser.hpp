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
  explicit Parser(const Scanner &scanner);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();
  int yylex(YYSTYPE *, Location *location);
  void yyerror(const std::string &message, Location *location);
  friend int yylex(YYSTYPE *, Location *, Parser *parser);
  friend void yyerror(Location *, Parser *, const char *);
  Token lastToken;
  std::shared_ptr<CompilationUnitCtx> result;
  std::string filename;

private:
  std::vector<Token>::const_iterator it;
  const std::vector<Token> &tokens;
};
} // namespace manda::analysis

#endif
