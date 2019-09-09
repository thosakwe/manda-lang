#ifndef MANDA_SYMBOL_TABLE_HPP
#define MANDA_SYMBOL_TABLE_HPP
#include "generic_scope.hpp"
#include "object.hpp"
#include "type.hpp"
#include <jit/jit-plus.h>
#include <string>
#include <unordered_map>
#include <variant>

namespace manda::runtime {
typedef std::variant<std::shared_ptr<Object>, std::shared_ptr<Type>>
    ObjectOrType;
// using RuntimeScope = GenericScope<ObjectOrType>;
typedef GenericScope<jit_value> JitScope;
typedef GenericScope<ObjectOrType> RuntimeScope;
typedef GenericScope<std::shared_ptr<Type>> TypeScope;

// class RuntimeScope : public std::enable_shared_from_this<RuntimeScope> {
// public:
//  RuntimeScope() = default;
//  explicit RuntimeScope(std::shared_ptr<const RuntimeScope> &parent);
//  const ObjectOrType resolve(std::string name) const;
//  ObjectOrType add(const std::string &name, const ObjectOrType &value,
//             bool allowRedefine = false);
//  std::shared_ptr<RuntimeScope> createChild() const;
//  const std::unordered_map<std::string, ObjectOrType> &getSymbols();
//
// private:
//  std::shared_ptr<const RuntimeScope> parent;
//  std::unordered_map<std::string, ObjectOrType> symbols;
//};
} // namespace manda::runtime

#endif
