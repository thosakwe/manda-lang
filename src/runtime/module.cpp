#include <utility>

#include "module.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Module::Module(string name) : name(move(name)) {
  symbolTable = make_shared<SymbolTable>();
}

// const string &Module::getName() const { return name; }

// void Module::setName(string value) { name = std::move(value); }

shared_ptr<SymbolTable> Module::getSymbolTable() { return symbolTable; }

vector<unique_ptr<ExprCtx>> &Module::getTopLevelExpressions() {
  return topLevelExpressions;
}

// shared_ptr<const SymbolTable> Module::getSymbolTable() const {
//  return symbolTable;
//}
