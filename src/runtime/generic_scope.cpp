#include "generic_scope.hpp"

using namespace manda::runtime;
using namespace std;

template <typename T> GenericScope<T>::GenericScope() = default;

template <typename T>
const unordered_map<std::string, GenericSymbol<T>> &
GenericScope<T>::getSymbols() {
  return symbols;
}

template <typename T>
optional<T> GenericScope<T>::resolve(const std::string &name) const {
  auto symbol = resolveSymbol(name);
  if (!symbol) {
    return nullopt;
  } else {
    return symbol->value;
  }
}

template <typename T>
optional<GenericSymbol<T>>
GenericScope<T>::resolveSymbol(const std::string &name) const {
  for (auto &p : symbols) {
    if (p.first == name) {
      return p.second;
    }
  }

  if (parent) {
    return parent->resolveSymbol(name);
  } else {
    return nullopt;
  }
}

template <typename T>
GenericScope<T>::GenericScope(const GenericScope<T> &parent) : parent(parent) {}

template <typename T>
GenericSymbol<T> &GenericScope<T>::add(const std::string &name, const T &value,
                                       bool allowRedefine) {
  return addSymbol(name, {name, value}, allowRedefine);
}

template <typename T>
GenericSymbol<T> &GenericScope<T>::addSymbol(const std::string &name,
                                             const GenericSymbol<T> &symbol,
                                             bool allowRedefine) {
  auto it = symbols.find(name);
  if ((it != symbols.end()) && !allowRedefine) {
    return it->second;
  } else {
    return symbols[name] = symbol;
  }
}
