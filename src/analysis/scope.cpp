#include "scope.hpp"

using namespace manda::analysis;
using namespace std;

Scope::Scope(shared_ptr<const Scope> parent) : parent(move(parent)){};

const unordered_map<string, std::shared_ptr<Symbol>> &Scope::getSymbols() const {
  return symbols;
}

shared_ptr<Symbol> &Scope::add(const string &name, const Symbol &symbol,
                   bool allowRedefine) {
  auto it = symbols.find(name);
  if ((it != symbols.end()) && !allowRedefine) {
    return it->second;
  } else {
    return symbols[name] = make_shared<Symbol>(symbol);
  }
}

shared_ptr<Symbol> Scope::resolve(const string &name) const {
  for (auto &p : symbols) {
    if (p.first == name) {
      return p.second;
    }
  }

  if (parent) {
    return parent->resolve(name);
  } else {
    return nullptr;
  }
}

shared_ptr<Scope> Scope::createChild() const {
  auto child = enable_shared_from_this<Scope>::shared_from_this();
  return make_shared<Scope>(child);
}