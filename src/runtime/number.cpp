#include "number.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include "jit_compiled_function.hpp"
#include <exception>
#include <iomanip>
#include <iostream>
#include <jit/jit-dump.h>
#include <sstream>
#include <string>

using namespace manda::runtime;
using namespace std;

Number::Number(double value) : value(value) {}

double Number::getValue() const { return value; }

void Number::print(ostream &out, bool ansiSupported) const {
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

shared_ptr<Type> Number::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().numberType;
}

string NumberType::getName() const { return "Number"; }

jit_type_t NumberType::toJitType() const {
  // Manda numbers are system doubles.
  // TODO: Should these be long doubles?
  return jit_type_float64;
}

jit_value NumberType::boxRawValue(JitCompiledFunction &fn,
                                  const jit_value &rawValue) {
  // void* box(f64)
  jit_type_t params[1] = {jit_type_float64};
  auto sig =
      jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, params, 1, 0);
  jit_value_t args[1] = {rawValue.raw()};
  return fn.insn_call_native("manda_box_number", (void *)&box, sig, args, 1, 0);
}

Number *NumberType::box(jit_float64 value) { return new Number(value); }

typedef jit_float64 (*HackExec)();

shared_ptr<Object> NumberType::deserialize(Interpreter &interpreter,
                                           void *ptr) {
  //  auto *asFloat64 = (jit_float64 **)ptr;
  //  auto *first = asFloat64[0];
  //  cout << "HMM: " << (*first) << endl;
  //  return make_shared<Number>(*first);
  auto *asFloat64 = (jit_float64 *)ptr;
  //  throw logic_error("aaa");
  return make_shared<Number>(*asFloat64);

  //  // See the explanation in applyJitFunction for why we are using this hack.
  //  auto &context = interpreter.getJitContext();
  //  context.build_start();
  //  auto sig =
  //      jit_type_create_signature(jit_abi_cdecl, jit_type_float64, nullptr, 0,
  //      0);
  //  auto hack = jit_function_create(context.raw(), sig);
  //  jit_constant_t constant = {jit_type_void_ptr, ptr};
  //  auto jitPtr = jit_value_create_constant(hack, &constant);
  //  auto returnValue = jit_insn_load_relative(hack, jitPtr, 0,
  //  jit_type_float64); jit_insn_return(hack, returnValue);
  //  jit_function_compile(hack);
  //  context.build_end();
  //
  //  if (interpreter.getOptions().developerMode) {
  //    jit_dump_function(stdout, hack, "number_deserialize_hack");
  //  }
  //
  //  auto exec = (HackExec)jit_function_to_closure(hack);
  //  auto hackResult = exec();
  //  return make_shared<Number>(hackResult);
}

shared_ptr<Object> NumberType::applyJitFunction(Interpreter &interpreter,
                                                std::vector<void *> &args,
                                                jit_function &func) {
  // This is an extreme hack, but for some reason, reading the
  // void* as a double simply does not work.
  // However, jit_function_to_closure DOES.
  // So what we'll do is create a JIT function that simply returns
  // the double...
  auto &context = interpreter.getJitContext();
  context.build_start();
  auto sig =
      jit_type_create_signature(jit_abi_cdecl, jit_type_float64, nullptr, 0, 0);
  auto hack = jit_function_create(context.raw(), sig);
  vector<jit_value_t> jitArgs;
  for (void *arg : args) {
    jit_constant_t constant = {jit_type_void_ptr, arg};
    jitArgs.push_back(jit_value_create_constant(hack, &constant));
  }
  auto returnValue =
      jit_insn_call(hack, "number_hack_caller", func.raw(), func.signature(),
                    jitArgs.data(), jitArgs.size(), 0);
  jit_insn_return(hack, returnValue);
  jit_function_compile(hack);
  context.build_end();

  if (interpreter.getOptions().developerMode) {
    jit_dump_function(stdout, hack, "number_apply_hack");
  }

  auto exec = (HackExec)jit_function_to_closure(hack);
  auto result = exec();
  return make_shared<Number>(result);

  // TODO: Process result of jit_function_apply
  // TODO: Why does this return bogus information with zero args?
  //  if (args.empty()) {
  //    typedef jit_float64 (*HackExec)();
  //    auto exec = (HackExec)func.closure();
  //    auto result = exec();
  //    return make_shared<Number>(result);
  //  } else {
  //    return Type::applyJitFunction(args, func);
  //    //    jit_float64 result;
  //    //    func.apply(args.data(), &result);
  //    //    return make_shared<Number>(result);
  //  }
}
