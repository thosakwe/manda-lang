#ifndef MANDA_PARSER_HPP
#define MANDA_PARSER_HPP
#include "ast.hpp"
#include "token.hpp"
#include <vector>

namespace manda::analysis {
class Parser {
public:
  explicit Parser(const std::vector<Token> &tokens);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();

private:
  std::vector<Token>::iterator it;
  std::vector<Token> tokens;
};
} // namespace manda::analysis

#endif
