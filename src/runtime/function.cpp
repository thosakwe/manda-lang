#include "function.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

FunctionType::FunctionType(std::vector<Parameter> parameters,
                           std::shared_ptr<Type> returnType)
    : parameters(move(parameters)), returnType(move(returnType)) {}

string FunctionType::getName() const {
  // TODO: Return "Function(x, y) -> z"
  return "Function";
}

const vector<Parameter> &FunctionType::getParameters() const {
  return parameters;
}

const shared_ptr<Type> &FunctionType::getReturnType() const {
  return returnType;
}