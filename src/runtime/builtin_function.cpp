#include "builtin_function.hpp"
#include <sstream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

BuiltinFunction::BuiltinFunction(
    string name, vector<Parameter> parameters, std::shared_ptr<Type> returnType,
    function<shared_ptr<Object>(Interpreter &, const Location,
                                shared_ptr<Object> &,
                                const vector<shared_ptr<Object>> &)>
        fn)
    : name(move(name)), parameters(move(parameters)),
      returnType(move(returnType)), fn(move(fn)) {}

const string &BuiltinFunction::getName() const { return name; }

const vector<Parameter> &BuiltinFunction::getParameters() const {
  return parameters;
}

void BuiltinFunction::acceptForJitCall(JitCompiledFunction &function,
                                       std::vector<jit_value> &arguments) {
  // TODO: C API here?
}

shared_ptr<Object>
BuiltinFunction::invoke(Interpreter &interpreter, const Location &location,
                        shared_ptr<Object> &thisObject,
                        const vector<shared_ptr<Object>> &args) const {
  return fn(interpreter, location, thisObject, args);
}

shared_ptr<Type>
BuiltinFunction::getReturnType(Interpreter &interpreter) const {
  return returnType;
}
