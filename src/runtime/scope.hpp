#ifndef MANDA_SCOPE_HPP
#define MANDA_SCOPE_HPP
#include "symbol.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace manda::runtime {
class Scope : public std::enable_shared_from_this<Scope> {
private:
  std::shared_ptr<const Scope> parent;
  std::unordered_map<std::string, Symbol> symbols;

public:
  Scope() = default;

  explicit Scope(std::shared_ptr<const Scope> parent);

  const std::unordered_map<std::string, Symbol> &getSymbols() const;

public:
  Symbol &add(const std::string &name, const Symbol &symbol,
              bool allowRedefine = false);

  std::optional<Symbol> resolve(const std::string &name) const;

  std::shared_ptr<Scope> createChild() const;
};
} // namespace manda::runtime

#endif
