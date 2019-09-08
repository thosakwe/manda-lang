#ifndef MANDA_GENERIC_SCOPE_HPP
#define MANDA_GENERIC_SCOPE_HPP
#include <jit/jit-plus.h>
#include <optional>
#include <string>
#include <unordered_map>

namespace manda::runtime {
enum Visibility { pub, priv };

template <typename T> class GenericSymbol {
  std::string name;
  T value;
  Visibility visibility = priv;
};

template <typename T> class GenericScope {
public:
  GenericScope();
  const std::unordered_map<std::string, GenericSymbol<T>> &getSymbols();
  GenericSymbol<T> &add(const std::string &name, const T &value,
                        bool allowRedefine = false);
  GenericSymbol<T> &addSymbol(const std::string &name,
                              const GenericSymbol<T> &symbol,
                              bool allowRedefine);
  std::optional<T> resolve(const std::string &name) const;
  std::optional<GenericSymbol<T>> resolveSymbol(const std::string &name) const;

private:
  explicit GenericScope(const GenericScope<T> &parent);
  std::optional<GenericScope<T>> parent;
  std::unordered_map<std::string, GenericSymbol<T>> symbols;
};
} // namespace manda::runtime

#endif
