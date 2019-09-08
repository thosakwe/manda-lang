#include "char.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include <ostream>
#include <sstream>

using namespace manda::runtime;
using namespace std;

Char::Char(char ch) : value(ch) {}

void Char::print(ostream &out, bool ansiSupported) const {
  ostringstream oss;
  if (!ansiSupported) {
    oss << value;
  } else {
    oss << "'" << value << "'";
  }
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << cyan(oss.str());
  }
}

string CharType::getName() const { return "Char"; }

shared_ptr<Type> Char::getType(Interpreter &interpreter) const {
  return interpreter.getCoreLibrary().charType;
}

jit_type_t CharType::toJitType() const { return jit_type_sys_char; }

shared_ptr<manda::runtime::Object>
CharType::deserialize(Interpreter &interpreter, void *ptr) {
  auto *asUbyte = (jit_ubyte *)ptr;
  return make_shared<Char>((char)*asUbyte);
}

// shared_ptr<manda::runtime::Object>
// CharType::applyJitFunction(std::vector<void *> &args, jit_function &func) {
//  char result;
//  func.apply(args.data(), &result);
//  return make_shared<Char>(result);
//}
