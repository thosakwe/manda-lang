#ifndef MANDA_CORE_LIBRARY_HPP
#define MANDA_CORE_LIBRARY_HPP
#include "../analysis/location.hpp"
#include "any_type.hpp"
#include "bool.hpp"
#include "char.hpp"
#include "number.hpp"
#include "string.hpp"
#include "symbol_table.hpp"
#include "void.hpp"
#include <manda_api.h>

namespace manda::runtime {
class Interpreter;
class CoreLibrary {
public:
  CoreLibrary();

  std::shared_ptr<AnyType> anyType;
  std::shared_ptr<BoolType> boolType;
  std::shared_ptr<CharType> charType;
  std::shared_ptr<NumberType> numberType;
  std::shared_ptr<StringType> stringType;
  std::shared_ptr<VoidType> voidType;

  void install(SymbolTable &scope);

  static void printFn(manda_context_t context);

  //  static std::shared_ptr<Object>
  //  printFn(manda::runtime::Interpreter &, const manda::analysis::Location &,
  //          std::shared_ptr<Object> &,
  //          const std::vector<std::shared_ptr<Object>> &);
};
} // namespace manda::runtime

#endif
