#include "function_type.hpp"
#include <sstream>

using namespace manda::analysis;
using namespace std;

FunctionType::FunctionType(std::vector<Parameter> parameters,
                           std::shared_ptr<Type> returnType)
    : parameters(move(parameters)), returnType(move(returnType)) {}

string FunctionType::getName() const {
  // Return "fn (x, y) -> z"
  ostringstream oss;
  oss << "fn (";
  for (unsigned long i = 0; i < getParameters().size(); i++) {
    if (i > 0) {
      oss << ", ";
    }
    oss << getParameters()[i].name;
  }
  oss << ") -> ";
  oss << getReturnType()->getName();
  return oss.str();
}

const vector<Parameter> &FunctionType::getParameters() const {
  return parameters;
}

const shared_ptr<Type> &FunctionType::getReturnType() const {
  return returnType;
}