#include "parser.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace std;

Parser::Parser(Scanner &scanner) : scanner(scanner) {}

shared_ptr<CompilationUnitCtx> Parser::parseCompilationUnit() {
  // TODO: Implement this...
  return nullptr;
}

bool Parser::next(Token::TokenType type) {
  if (!queue_.empty()) {
    auto tok = queue_.front();
    if (tok.type == type) {
      current = tok;
      queue_.pop();
      return true;
    } else {
      return false;
    }
  } else {
    auto tok = scanner.nextToken();
    if (tok.type == type) {
      current = tok;
      return true;
    } else {
      queue_.push(tok);
      return false;
    }
  }
}
