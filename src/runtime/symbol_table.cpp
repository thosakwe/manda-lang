#include "symbol_table.hpp"
//
//using namespace manda::runtime;
//using namespace std;
//
//const unordered_map<string, ObjectOrType> &RuntimeScope::getSymbols() {
//  return symbols;
//}
//
//const ObjectOrType RuntimeScope::resolve(const string name) const {
//  auto it = symbols.find(name);
//  if (it != symbols.end()) {
//    return it->second;
//  } else if (parent) {
//    return parent->resolve(name);
//  } else {
//    return monostate();
//  }
//}
//
//ObjectOrType RuntimeScope::add(const string &name, const ObjectOrType &value, bool allowRedefine) {
//  auto it = symbols.find(name);
//  if ((it != symbols.end() && !allowRedefine) ||
//      holds_alternative<monostate>(value)) {
//    // The symbol cannot be redefined...
//    return monostate();
//  } else {
//    return symbols[name] = value;
//  }
//}
//
//std::shared_ptr<RuntimeScope> RuntimeScope::createChild() const {
//  auto child = shared_from_this();
//  return make_shared<RuntimeScope>(child);
//}
//
//RuntimeScope::RuntimeScope(shared_ptr<const RuntimeScope> &parent)
//    : parent(parent) {}
