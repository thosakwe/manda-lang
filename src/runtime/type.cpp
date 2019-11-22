#include "type.hpp"
#include "jit_compiled_function.hpp"
#include <cstdlib>
#include <exception>
#include <sstream>

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

jit_value Type::invokeStaticBoxFunction(const char *name, void *funcPtr,
                                        const jit_type_t &inType,
                                        JitCompiledFunction &fn,
                                        const jit_value &rawValue) {
  // void* box(rawValue)
  jit_type_t params[1] = {inType};
  auto sig =
      jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, params, 1, 0);
  jit_value_t args[1] = {rawValue.raw()};
  return fn.insn_call_native(name, funcPtr, sig, args, 1, 0);
}

std::shared_ptr<Type> Type::getParent() const { return nullptr; }

unsigned long Type::getLevel() const { return 0; }

//
// jit_value Type::boxRawValue(JitCompiledFunction &fn,
//                            const jit_value &rawValue) {
//  ostringstream oss;
//  oss << "boxRawValue not implemented for type ";
//  oss << getName();
//  throw logic_error(oss.str());
//}

//
// std::shared_ptr<manda::runtime::Object> Type::deserialize(void *ptr) {
//  ostringstream oss;
//  oss << "deserialize not implemented for type ";
//  oss << getName();
//  throw logic_error(oss.str());
//}
