#include "number.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

manda::runtime::Number::Number(double value) : value(value) {}

double manda::runtime::Number::getValue() const { return value; }

void manda::runtime::Number::print(ostream &out, bool ansiSupported) const {
  //  out << value;
  //  return;
  ostringstream oss;
  oss << fixed << value;
  auto str = oss.str();
  // Remove trailing zeros.
  str.erase(str.find_last_not_of('0') + 1, string::npos);
  if (!ansiSupported) {
    out << str;
  } else {
    out << yellow(str);
  }
}

shared_ptr<manda::runtime::Type> manda::runtime::Number::getType(
    manda::runtime::Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().numberType;
}

string manda::runtime::NumberType::getName() const { return "Number"; }

jit_type_t manda::runtime::NumberType::toJitType() const {
  // Manda numbers are system doubles.
  // TODO: Should these be long doubles?
  return jit_type_float64;
}

shared_ptr<manda::runtime::Object>
manda::runtime::NumberType::deserialize(void *ptr) {
  auto *asFloat64 = (jit_float64 *)ptr;
  return make_shared<Number>(*asFloat64);
}

shared_ptr<manda::runtime::Object>
manda::runtime::NumberType::applyJitFunction(std::vector<void *> &args,
                                             jit_function &func) {
  // TODO: Process result of jit_function_apply
  // TODO: Why does this return bogus information with zero args?
  if (args.empty()) {
    typedef jit_float64 (*Exec)();
    auto exec = (Exec)func.closure();
    auto result = exec();
    return make_shared<Number>(result);
  } else {
    return Type::applyJitFunction(args, func);
    //    jit_float64 result;
    //    func.apply(args.data(), &result);
    //    return make_shared<Number>(result);
  }
}
