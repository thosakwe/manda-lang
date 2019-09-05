#include "parser.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace std;

Parser::Parser(const Scanner &scanner)
    : tokens(scanner.getTokens()), it(scanner.getTokens().begin()) {}

int manda::analysis::yylex(YYSTYPE *lvalp, Parser *parser) {
  auto token = *(parser->it++);
  // TODO: Set last text
  return (token.type - Token::EQUALS) + 1;
}

void manda::analysis::yyerror(Parser *parser, const char *message) {
  // TODO: Add to an error buffer.
  cerr << "error: " << message << endl;
}

shared_ptr<CompilationUnitCtx> Parser::parseCompilationUnit() {
  // TODO: Implement this...
  return nullptr;
}
