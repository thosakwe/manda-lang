#ifndef MANDA_GENERIC_SCOPE_HPP
#define MANDA_GENERIC_SCOPE_HPP
#include <jit/jit-plus.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace manda::runtime {
enum Visibility { pub, priv };

template <typename T> struct GenericSymbol {
  std::string name;
  T value;
  Visibility visibility = priv;
};

template <typename T>
class GenericScope : public std::enable_shared_from_this<GenericScope<T>> {
private:
  std::shared_ptr<const GenericScope<T>> parent;
  std::unordered_map<std::string, GenericSymbol<T>> symbols;

public:
  GenericScope() = default;

  explicit GenericScope(const std::shared_ptr<const GenericScope<T>> &parent)
      : parent(parent){};

  const std::unordered_map<std::string, GenericSymbol<T>> &getSymbols() {
    return symbols;
  }

public:
  GenericSymbol<T> &add(const std::string &name, const T &value,
                        bool allowRedefine = false) {
    return addSymbol(name, {name, value}, allowRedefine);
  }

  GenericSymbol<T> &addSymbol(const std::string &name,
                              const GenericSymbol<T> &symbol,
                              bool allowRedefine = false) {
    auto it = symbols.find(name);
    if ((it != symbols.end()) && !allowRedefine) {
      return it->second;
    } else {
      return symbols[name] = symbol;
    }
  }

  std::optional<GenericSymbol<T>> resolveSymbol(const std::string &name) const {
    for (auto &p : symbols) {
      if (p.first == name) {
        return p.second;
      }
    }

    if (parent) {
      return parent->resolveSymbol(name);
    } else {
      return std::nullopt;
    }
  }

  std::optional<T> resolve(const std::string &name) const {
    auto symbol = resolveSymbol(name);
    if (!symbol) {
      return std::nullopt;
    } else {
      return symbol->value;
    }
  }

  std::shared_ptr<GenericScope<T>> createChild() const {
    auto child =
        std::enable_shared_from_this<GenericScope<T>>::shared_from_this();
    return std::make_shared<GenericScope<T>>(child);
  }
};
} // namespace manda::runtime

#endif
