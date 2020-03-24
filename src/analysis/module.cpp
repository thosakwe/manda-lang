#include <utility>

#include "module.hpp"

using namespace manda::analysis;
using namespace manda::analysis;
using namespace std;

Module::Module(string name) : name(move(name)) {
  symbolTable = make_shared<RuntimeScope>();
}

// const string &Module::getName() const { return name; }

// void Module::setName(string value) { name = std::move(value); }

shared_ptr<RuntimeScope> Module::getSymbolTable() { return symbolTable; }

vector<unique_ptr<ExprCtx>> &Module::getTopLevelExpressions() {
  return topLevelExpressions;
}

const string &Module::getName() const { return name; }

// shared_ptr<const RuntimeScope> Module::getSymbolTable() const {
//  return symbolTable;
//}
