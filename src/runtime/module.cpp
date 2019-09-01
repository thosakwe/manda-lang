#include "module.hpp"

manda::runtime::Module::Module(std::string &name) : name(name) {}

const std::string &manda::runtime::Module::getName() const { return name; }

manda::runtime::SymbolTable &manda::runtime::Module::getSymbolTable() {
  return symbolTable;
}

const manda::runtime::SymbolTable &
manda::runtime::Module::getSymbolTable() const {
  return symbolTable;
}
