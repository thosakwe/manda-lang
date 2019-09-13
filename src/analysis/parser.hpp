#ifndef MANDA_PARSER_HPP
#define MANDA_PARSER_HPP
#include "ast.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include <queue>
#include <vector>

// TODO: Pass an Interpreter instance, and use reportError
namespace manda::analysis {
class Parser {
public:
  explicit Parser(Scanner &scanner);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();
  bool next(Token::TokenType type);

private:
  Token current;
  Scanner &scanner;
  std::queue<Token> queue_;
};
} // namespace manda::analysis

#endif
