#include "core_library.hpp"
#include "void_type.hpp"
#include <memory>

using namespace manda::analysis;
using namespace manda::analysis;
using namespace std;

CoreLibrary::CoreLibrary() {
  anyType = make_shared<AnyType>();
  boolType = make_shared<BoolType>();
  charType = make_shared<CharType>();
  numberType = make_shared<NumberType>();
  stringType = make_shared<StringType>();
  voidType = make_shared<VoidType>();
}

void CoreLibrary::install(Scope &scope) {
  // TODO: Find the core library file, and load the types directly from there.
  Location loc;
  scope.add("Any", {loc, anyType});
  scope.add("Bool", {loc, boolType});
  scope.add("Char", {loc, charType});
  scope.add("Number", {loc, numberType});
  scope.add("String", {loc, stringType});
  scope.add("Void", {loc, voidType});
}
