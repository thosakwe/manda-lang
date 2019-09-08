#include "number.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include <iomanip>
#include <string>

using namespace std;

manda::runtime::Number::Number(double value) : value(value) {}

double manda::runtime::Number::getValue() const { return value; }

void manda::runtime::Number::print(ostream &out, bool ansiSupported) const {
  // Remove trailing zeros.
  auto str = to_string(value);
  str.erase(str.find_last_not_of('0') + 1, string::npos);
  out << fixed;
  if (!ansiSupported) {
    out << str;
  } else {
    out << yellow(str);
  }
  out << scientific;
}

shared_ptr<manda::runtime::Type> manda::runtime::Number::getType(
    manda::runtime::Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().numberType;
}

string manda::runtime::NumberType::getName() const { return "Number"; }

jit_type_t manda::runtime::NumberType::toJitType() const {
  // Manda numbers are system doubles.
  // TODO: Should these be long doubles?
  return jit_type_sys_double;
}

shared_ptr<manda::runtime::Object>
manda::runtime::NumberType::applyJitFunction(void **args, jit_function &func) {
  // TODO: Process result of jit_function_apply
  jit_float64 result;
  func.apply(args, &result);
  return make_shared<Number>(result);
}
