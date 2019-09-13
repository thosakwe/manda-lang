#ifndef MANDA_PARSER_HPP
#define MANDA_PARSER_HPP
#include "ast.hpp"
#include "scanner.hpp"
#include "token.hpp"
typedef union YYSTYPE YYSTYPE;
#include <vector>

// TODO: Pass an Interpreter instance, and use reportError
namespace manda::analysis {
class Parser {
public:
  explicit Parser(const Scanner &scanner);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();

private:
  Scanner &scanner;
};
} // namespace manda::analysis

#endif
