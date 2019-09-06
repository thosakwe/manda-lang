#include "symbol_table.hpp"

using namespace manda::runtime;
using namespace std;

const unordered_map<string, Symbol> &SymbolTable::getSymbols() {
  return symbols;
}

const Symbol SymbolTable::resolve(const string name) const {
  auto it = symbols.find(name);
  if (it != symbols.end()) {
    return it->second;
  } else if (parent) {
    return parent->resolve(name);
  } else {
    return monostate();
  }
}

Symbol SymbolTable::add(const string &name, const Symbol &value, bool allowRedefine) {
  auto it = symbols.find(name);
  if ((it != symbols.end() && !allowRedefine) ||
      holds_alternative<monostate>(value)) {
    // The symbol cannot be redefined...
    return monostate();
  } else {
    return symbols[name] = value;
  }
}

shared_ptr<SymbolTable> SymbolTable::createChild() const {
  auto child = shared_from_this();
  return make_shared<SymbolTable>(child);
}

SymbolTable::SymbolTable(shared_ptr<const SymbolTable> &parent)
    : parent(parent) {}
