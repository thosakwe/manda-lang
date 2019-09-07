#include "object_resolver.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

ObjectResolver::ObjectResolver(Interpreter &interpreter,
                               shared_ptr<SymbolTable> scope)
    : interpreter(interpreter), scope(move(scope)) {}

const shared_ptr<Object> &ObjectResolver::getLastObject() const {
  return lastObject;
}
