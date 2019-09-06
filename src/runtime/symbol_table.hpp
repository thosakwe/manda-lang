#ifndef MANDA_SYMBOL_TABLE_HPP
#define MANDA_SYMBOL_TABLE_HPP
#include "object.hpp"
#include "type.hpp"
#include <string>
#include <unordered_map>
#include <variant>

namespace manda::runtime {
using Symbol = std::variant<std::monostate, std::shared_ptr<Object>,
                            std::shared_ptr<Type>>;

class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
public:
  SymbolTable() = default;
  explicit SymbolTable(std::shared_ptr<const SymbolTable> &parent);
  const Symbol resolve(std::string name) const;
  Symbol add(std::string &name, const Symbol &value,
             bool allowRedefine = false);
  std::shared_ptr<SymbolTable> createChild() const;
  const std::unordered_map<std::string, Symbol> &getSymbols();

private:
  std::shared_ptr<const SymbolTable> parent;
  std::unordered_map<std::string, Symbol> symbols;
};
} // namespace manda::runtime

#endif
