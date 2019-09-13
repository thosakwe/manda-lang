#include "parser.hpp"
#include <iostream>
#include <manda_flex_parser.hpp>

using namespace manda::analysis;
using namespace std;

Parser::Parser(const Scanner &scanner) : scanner(scanner) {}

shared_ptr<CompilationUnitCtx> Parser::parseCompilationUnit() {
  // TODO: Implement this...
  return nullptr;
}
