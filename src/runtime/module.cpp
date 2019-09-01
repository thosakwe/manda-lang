#include <utility>

#include "module.hpp"

manda::runtime::Module::Module(std::string name) : name(std::move(name)) {}

const std::string &manda::runtime::Module::getName() const { return name; }

void manda::runtime::Module::setName(std::string value) { name = value; }

manda::runtime::SymbolTable &manda::runtime::Module::getSymbolTable() {
  return symbolTable;
}

const manda::runtime::SymbolTable &
manda::runtime::Module::getSymbolTable() const {
  return symbolTable;
}
