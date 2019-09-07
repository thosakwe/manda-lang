#ifndef MANDA_CORE_LIBRARY_HPP
#define MANDA_CORE_LIBRARY_HPP
#include "any_type.hpp"
#include "interpreter.hpp"
#include "symbol_table.hpp"

namespace manda::runtime {
class CoreLibrary {
public:
  CoreLibrary();

  std::shared_ptr<AnyType> anyType;

  void install(SymbolTable &scope);

  static std::shared_ptr<Object>
  printFn(manda::runtime::Interpreter &, const manda::analysis::Location &,
          std::shared_ptr<Object> &,
          const std::vector<std::shared_ptr<Object>> &);
};
} // namespace manda::runtime

#endif
