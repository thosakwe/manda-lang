#include "builtin_function.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

const string &BuiltinFunction::getName() const { return name; }

const vector<Parameter> &BuiltinFunction::getParameters() const {
  return parameters;
}

std::shared_ptr<Object> BuiltinFunction::invoke(
    Interpreter &interpreter, const manda::analysis::Location &location,
    std::shared_ptr<Object> &thisObject,
    const std::vector<std::shared_ptr<Object>> &args) const {
  return fn(interpreter, location, thisObject, args);
}

void BuiltinFunction::print(ostream &out, bool ansiSupported) const {
  ostringstream oss;
  oss << "[Function";
  if (!name.empty()) {
    oss << ": " << name;
  }
  oss << "]";
  if (!ansiSupported) {
    out << oss.str();
  } else {
    out << cyan(oss.str());
  }
}
