#include "core_library.hpp"
#include "builtin_function.hpp"
#include "void.hpp"
#include <iostream>
#include <memory>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

shared_ptr<Object>
CoreLibrary::printFn(Interpreter &i, const Location &l, shared_ptr<Object> &,
                     const vector<shared_ptr<Object>> &args) {
  if (i.ensureArgumentCount(l, args, 1)) {
    args[0]->print(cout, false);
    cout << endl;
    return make_shared<Void>();
  } else {
    // Return a nullptr here to signify failure.
    return nullptr;
  }
}

void CoreLibrary::install(SymbolTable &scope) {
  // TODO: Why does make_shared not work here???
  // TODO: Add required parameters?
  scope.add("print",
            shared_ptr<Object>(new BuiltinFunction("print", {}, printFn)));
}
