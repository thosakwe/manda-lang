#ifndef MANDA_SCOPE_HPP
#define MANDA_SCOPE_HPP
#include "symbol.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace manda::analysis {
class Scope : public std::enable_shared_from_this<Scope> {
private:
  std::shared_ptr<const Scope> parent;
  std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;

public:
  Scope() = default;

  explicit Scope(std::shared_ptr<const Scope> parent);

  const std::unordered_map<std::string, std::shared_ptr<Symbol>> &getSymbols() const;

public:
  std::shared_ptr<Symbol> &add(const std::string &name, const Symbol &symbol,
              bool allowRedefine = false);

  std::shared_ptr<Symbol> resolve(const std::string &name) const;

  std::shared_ptr<Scope> createChild() const;
};
} // namespace manda::analysis

#endif
