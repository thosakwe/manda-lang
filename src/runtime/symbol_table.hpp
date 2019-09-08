#ifndef MANDA_SYMBOL_TABLE_HPP
#define MANDA_SYMBOL_TABLE_HPP
#include "generic_scope.hpp"
#include "object.hpp"
#include "type.hpp"
#include <string>
#include <unordered_map>
#include <variant>

namespace manda::runtime {
typedef std::variant<std::shared_ptr<Object>, std::shared_ptr<Type>> Symbol;
// using SymbolTable = GenericScope<Symbol>;
typedef GenericScope<Symbol> SymbolTable;

// class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
// public:
//  SymbolTable() = default;
//  explicit SymbolTable(std::shared_ptr<const SymbolTable> &parent);
//  const Symbol resolve(std::string name) const;
//  Symbol add(const std::string &name, const Symbol &value,
//             bool allowRedefine = false);
//  std::shared_ptr<SymbolTable> createChild() const;
//  const std::unordered_map<std::string, Symbol> &getSymbols();
//
// private:
//  std::shared_ptr<const SymbolTable> parent;
//  std::unordered_map<std::string, Symbol> symbols;
//};
} // namespace manda::runtime

#endif
