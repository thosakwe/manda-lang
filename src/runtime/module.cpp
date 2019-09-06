#include <utility>

#include "module.hpp"

using namespace manda::runtime;
using namespace std;

Module::Module(string name) : name(move(name)) {}

const string &Module::getName() const { return name; }

void Module::setName(string value) { name = value; }

shared_ptr<SymbolTable> Module::getSymbolTable() {
  return symbolTable.shared_from_this();
}
shared_ptr<const SymbolTable> Module::getSymbolTable() const {
  return symbolTable.shared_from_this();
}
