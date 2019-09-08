#include "type.hpp"
#include <exception>

using namespace manda::runtime;
using namespace std;

bool Type::isExactly(const Type &other) { return &other == this; }

bool Type::isAssignableTo(const Type &other) { return isExactly(other); }

shared_ptr<Object> Type::applyJitFunction(void **args, jit_function &func) {
  throw logic_error("applyJitFunction not implemented");
}
