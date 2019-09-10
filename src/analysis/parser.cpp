#include "parser.hpp"
#include <iostream>
#include <manda_flex_parser.hpp>

using namespace manda::analysis;
using namespace std;

int yyparse(Parser *parser);

Parser::Parser(const Scanner &scanner)
    : tokens(scanner.getTokens()), it(scanner.getTokens().begin()),
      filename(scanner.getFilename()) {}

int manda::analysis::yylex(YYSTYPE *lvalp, Location *location, Parser *parser) {
  return parser->yylex(lvalp, location);
}

int Parser::yylex(YYSTYPE *lvalp, Location *location) {
  if (it >= tokens.end()) {
    return 0;
  }
  auto token = lastToken = *(it++);
  // TODO: Set last text
  return (token.type - Token::EQUALS) + EQUALS;
}

void manda::analysis::yyerror(Location *location, Parser *parser,
                              const char *message) {
  parser->yyerror(message, location);
}

void Parser::yyerror(const std::string &message, Location *location) {
  // TODO: Add to an error buffer.
  cerr << "error: " << (*location) << ": " << message << endl;
}

shared_ptr<CompilationUnitCtx> Parser::parseCompilationUnit() {
  // TODO: Implement this...
  it = tokens.begin();
  yyparse(this);
  return result;
}
