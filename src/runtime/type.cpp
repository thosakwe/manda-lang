#include "type.hpp"
#include <exception>
#include <sstream>
#include <stdlib.h>

using namespace manda::runtime;
using namespace std;

bool Type::isExactly(const Type &other) { return &other == this; }

bool Type::isAssignableTo(const Type &other) { return isExactly(other); }

bool Type::isAssignableTo(const std::shared_ptr<Type> &other) {
  return isAssignableTo(*other);
}

std::shared_ptr<manda::runtime::Object>
Type::applyJitFunction(Interpreter &interpreter, std::vector<void *> &args,
                       jit_function &func) {
  auto size = jit_type_get_size(toJitType());
  auto *ptr = malloc(size);
  func.apply(args.data(), ptr);
  auto result = deserialize(interpreter, ptr);
  free(ptr);
  return result;
  //  ostringstream oss;
  //  oss << "applyJitFunction not implemented for type ";
  //  oss << getName();
  //  throw logic_error(oss.str());
}

jit_value Type::boxRawValue(JitCompiledFunction &fn,
                            const jit_value &rawValue) {
  ostringstream oss;
  oss << "boxRawValue not implemented for type ";
  oss << getName();
  throw logic_error(oss.str());
}

//
// std::shared_ptr<manda::runtime::Object> Type::deserialize(void *ptr) {
//  ostringstream oss;
//  oss << "deserialize not implemented for type ";
//  oss << getName();
//  throw logic_error(oss.str());
//}
