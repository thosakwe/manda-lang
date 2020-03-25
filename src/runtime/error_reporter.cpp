#include "error_reporter.hpp"
#include "ansi_printer.hpp"
#include <iostream>
#include <sstream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

void ErrorReporter::reportError(const Location &location,
                                const string &message) {
  // TODO: What about thrown errors?
  ostringstream oss;
  oss << "Error: " << location << ": " << message;
  cerr << red(oss.str()) << endl;
}