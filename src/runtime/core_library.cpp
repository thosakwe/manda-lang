#include "core_library.hpp"
#include "builtin_function.hpp"
#include "interpreter.hpp"
#include "void.hpp"
#include <iostream>
#include <memory>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

CoreLibrary::CoreLibrary() {
  anyType = make_shared<AnyType>();
  boolType = make_shared<BoolType>();
  charType = make_shared<CharType>();
  numberType = make_shared<NumberType>();
  stringType = make_shared<StringType>();
  voidType = make_shared<VoidType>();
}

// shared_ptr<Object>
// CoreLibrary::printFn(Interpreter &i, const Location &l, shared_ptr<Object> &,
//                     const vector<shared_ptr<Object>> &args) {
//  if (i.ensureArgumentCount(l, args, 1)) {
//    args[0]->print(cout, false);
//    cout << endl;
//    return make_shared<Void>();
//  } else {
//    // Return a nullptr here to signify failure.
//    return nullptr;
//  }
//}

void CoreLibrary::printFn(manda_context_t context) {
  // TODO: Actually print the object.
  cout << "Hello from the API!" << endl;
}

void CoreLibrary::install(SymbolTable &scope) {
  // TODO: Why does make_shared not work here???
  // TODO: Add required parameters?
  scope.add("Any", anyType);
  scope.add("Bool", boolType);
  scope.add("Char", charType);
  scope.add("Number", numberType);
  scope.add("String", stringType);
  scope.add("Void", voidType);
  scope.add("print", shared_ptr<Object>(
                         new BuiltinFunction("print", {}, voidType, printFn)));
}
