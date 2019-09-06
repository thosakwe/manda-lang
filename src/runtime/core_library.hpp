#ifndef MANDA_CORE_LIBRARY_HPP
#define MANDA_CORE_LIBRARY_HPP
#include "interpreter.hpp"
#include "symbol_table.hpp"

namespace manda::runtime {
class CoreLibrary {
public:
  static std::shared_ptr<Object>
  printFn(manda::runtime::Interpreter &, const manda::analysis::Location &,
          std::shared_ptr<Object> &,
          const std::vector<std::shared_ptr<Object>> &);
  static void install(SymbolTable &scope);
};
} // namespace manda::runtime

#endif
