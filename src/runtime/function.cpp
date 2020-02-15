#include "function.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

void Function::print(ostream &out, bool ansiSupported) const {
  auto &name = getName();
  auto &params = getParameters();
  ostringstream oss;
  oss << "[Function";
  if (!name.empty()) {
    oss << ": " << name;
  }
  if (!params.empty()) {
    oss << "(";
    for (unsigned long i = 0; i < params.size(); i++) {
      auto &param = params[i];
      if (i > 0) {
        oss << ", ";
      }
      oss << param.name;
      // TODO: Add print to Type, so they can printed here
    }
    oss << ")";
  }
  oss << "]";
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << cyan(oss.str());
  }
}

shared_ptr<Type> Function::getType(Interpreter &interpreter) const {
  return make_shared<FunctionType>(getParameters(), getReturnType(interpreter));
}

FunctionType::FunctionType(std::vector<Parameter> parameters,
                           std::shared_ptr<Type> returnType)
    : parameters(move(parameters)), returnType(move(returnType)) {}

string FunctionType::getName() const {
  // TODO: Return "Function(x, y) -> z"
  return "Function";
}

jit_type_t FunctionType::toJitType() const {
  // A Manda Function maps directly to a JIT function.
  auto jitReturnType = returnType->toJitType();
  vector<jit_type_t> jitParams;
  for (auto &param : parameters) {
    jitParams.push_back(param.type->toJitType());
  }
  return jit_type_create_signature(jit_abi_cdecl, jitReturnType,
                                   jitParams.data(), jitParams.size(), 0);
}

shared_ptr<manda::runtime::Object>
FunctionType::deserialize(Interpreter &interpreter, void *ptr) {
  auto *object = (Function **)ptr;
  return shared_ptr<Function>(*object);
}

const vector<Parameter> &FunctionType::getParameters() const {
  return parameters;
}

const shared_ptr<Type> &FunctionType::getReturnType() const {
  return returnType;
}

jit_value FunctionType::boxRawValue(JitCompiledFunction &fn,
                                    const jit_value &rawValue) {
  // We are receiving, and sending out, a function pointer.
  return rawValue;
}
