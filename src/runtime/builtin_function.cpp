#include "builtin_function.hpp"
#include "ansi_printer.hpp"
#include <sstream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

const string &BuiltinFunction::getName() const { return name; }

const vector<Parameter> &BuiltinFunction::getParameters() const {
  return parameters;
}

shared_ptr<Object>
BuiltinFunction::invoke(Interpreter &interpreter, const Location &location,
                        shared_ptr<Object> &thisObject,
                        const vector<shared_ptr<Object>> &args) const {
  return fn(interpreter, location, thisObject, args);
}

BuiltinFunction::BuiltinFunction(
    string name, vector<Parameter> parameters,
    function<shared_ptr<Object>(Interpreter &, const Location,
                                shared_ptr<Object> &,
                                const vector<shared_ptr<Object>> &)>
        fn)
    : name(move(name)), parameters(move(parameters)), fn(move(fn)) {}

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
