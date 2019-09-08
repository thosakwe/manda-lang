#include "builtin_function.hpp"
#include "jit_compiled_function.hpp"
#include "manda_api_impl.hpp"
#include <sstream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

BuiltinFunction::BuiltinFunction(string name, vector<Parameter> parameters,
                                 std::shared_ptr<Type> returnType,
                                 MandaNativeFunction nativeFunction)
    : name(move(name)), parameters(move(parameters)),
      returnType(move(returnType)), nativeFunction(nativeFunction) {}

const string &BuiltinFunction::getName() const { return name; }

const vector<Parameter> &BuiltinFunction::getParameters() const {
  return parameters;
}

void BuiltinFunction::acceptForJitCall(JitCompiledFunction &function,
                                       vector<jit_value> &arguments) {
  // We were given a pointer to a C API function... Call it.
  // TODO: Support retrieving the value from a C API function.

  // The C API expects: void my_func(struct _manda_context*).
  jit_type_t apiSignatureParams[1] = {jit_type_void_ptr};
  jit_type_t apiSignature = jit_type_create_signature(
      jit_abi_cdecl, jit_type_void, apiSignatureParams, 1, 0);

  // Create a _manda_context.
  // TODO: Make sure this pointer gets freed.
  // TODO: Figure out how to pass parameters here.
  auto *context = new _manda_context{function.getInterpreter()};
  jit_value_t apiArgs[1] = {
      function.new_constant((void *)context, jit_type_void_ptr).raw()};
  function.insn_call_native(name.c_str(), (void *)nativeFunction, apiSignature,
                            apiArgs, 1, JIT_CALL_TAIL);
}

shared_ptr<Object>
BuiltinFunction::invoke(Interpreter &interpreter, const Location &location,
                        shared_ptr<Object> &thisObject,
                        const vector<shared_ptr<Object>> &args) const {
  // TODO: Allow invoking built-in functions outside of JIT???
  return nullptr;
}

shared_ptr<Type>
BuiltinFunction::getReturnType(Interpreter &interpreter) const {
  return returnType;
}
