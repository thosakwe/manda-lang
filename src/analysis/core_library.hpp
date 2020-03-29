#ifndef MANDA_CORE_LIBRARY_HPP
#define MANDA_CORE_LIBRARY_HPP
#include "any_type.hpp"
#include "bool_type.hpp"
#include "char_type.hpp"
#include "location.hpp"
#include "number_type.hpp"
#include "scope.hpp"
#include "string_type.hpp"
#include "unresolved.hpp"
#include "void_type.hpp"
#include <manda_api.h>

namespace manda::analysis {
class CoreLibrary {
public:
  CoreLibrary();

  std::shared_ptr<AnyType> anyType;
  std::shared_ptr<BoolType> boolType;
  std::shared_ptr<CharType> charType;
  std::shared_ptr<NumberType> numberType;
  std::shared_ptr<StringType> stringType;
  std::shared_ptr<UnresolvedType> unresolvedType;
  std::shared_ptr<VoidType> voidType;

  void install(Scope &scope);
};
} // namespace manda::analysis

#endif
