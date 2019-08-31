#include "symbol_table.hpp"

const manda::runtime::Symbol
manda::runtime::SymbolTable::resolve(std::string &name) const {
  auto it = symbols.find(name);
  if (it != symbols.end()) {
    return it->second;
  } else if (parent) {
    return parent->resolve(name);
  } else {
    return std::monostate();
  }
}

manda::runtime::Symbol
manda::runtime::SymbolTable::add(std::string &name,
                                 manda::runtime::Symbol &value) {
  auto it = symbols.find(name);
  if (it != symbols.end() || std::holds_alternative<std::monostate>(value)) {
    // The symbol cannot be redefined...
    return std::monostate();
  } else {
    return symbols[name] = value;
  }
}

std::shared_ptr<manda::runtime::SymbolTable>
manda::runtime::SymbolTable::createChild() const {
  return std::make_shared<SymbolTable>(shared_from_this());
}

manda::runtime::SymbolTable::SymbolTable(
    std::shared_ptr<manda::runtime::SymbolTable> &parent)
    : parent(parent) {}
