#include "type.hpp"
#include <exception>
#include <sstream>

using namespace manda::runtime;
using namespace std;

bool Type::isExactly(const Type &other) { return &other == this; }

bool Type::isAssignableTo(const Type &other) { return isExactly(other); }

shared_ptr<Object> Type::applyJitFunction(std::vector<void *> &args,
                                          jit_function &func) {
  ostringstream oss;
  oss << "applyJitFunction not implemented for type ";
  oss << getName();
  throw logic_error(oss.str());
}
