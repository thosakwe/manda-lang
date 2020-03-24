#include "analysis/ast_printer.hpp"
#include "analysis/parser.hpp"
#include "analysis/scanner.hpp"
#include "defs.hpp"
#include "runtime/ansi_printer.hpp"
#include "runtime/core_library.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/module_compiler.hpp"
#include "runtime/object_resolver.hpp"
#include "runtime/vm.hpp"
#include "runtime/vm_options.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <readline/history.h>
#include <readline/readline.h>
#include <sstream>
#include <string>
#include <sysexits.h>
#define tab "  "

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

int runFile(const VMOptions &options);
void printHelp(ostream &out);

int main(int argc, const char **argv) {
  VMOptions options;
  options.parse(argc, argv);
  if (options.developerMode) {
    cout << options << endl;
  }

  if (options.showHelp) {
    printHelp(cout);
    return 0;
  } else if (options.showVersion) {
    cout << "Manda " << MANDA_VERSION << endl;
    return 0;
  } else if (options.inputFile.empty()) {
    cerr << "Error: No input file was given." << endl;
    printHelp(cerr);
    return EX_USAGE;
  } else if (!options.parseErrors.empty()) {
    for (auto &err : options.parseErrors) {
      cerr << "Error: " << err << endl;
    }
    printHelp(cerr);
    return EX_USAGE;
  } else {
    return runFile(options);
  }
}

void printHelp(ostream &out) {
  out << "Usage: manda [<options>] [<input_file>] [--] [<input_file_options>]"
      << endl;
  out << endl
      << "Interprets the program in <input_file>, forwarding any "
      << "<input_file_options> to said program." << endl;
  out << "If no <input_file> is given, a REPL will begin." << endl;
  out << endl << "Options:" << endl;
  out << "--help or -h" << endl;
  out << tab << "Print this help information." << endl;
  out << "--devel" << endl; // TODO: Only in `DEBUG` mode.
  out << tab << "Enable (extremely verbose) debugging information." << endl;
  out << "-DNAME[=VALUE] or --define NAME=VALUE" << endl;
  out << tab << "Define NAME to be VALUE, or empty." << endl;
  out << "--version or -v" << endl;
  out << tab << "Print the version of the installed runtime." << endl;
}

int runFile(const VMOptions &options) {
  auto &filename = options.inputFile;
  ifstream ifs(filename);
  if (!ifs) {
    cerr << "Error: File \"" << filename << "\" does not exist." << endl;
    return 1;
  }

  string contents = {istreambuf_iterator<char>(ifs),
                     istreambuf_iterator<char>()};
  VM vm(options);
  Scanner scanner(filename, contents);
  Parser parser(scanner);
  auto compilationUnit = parser.parseCompilationUnit();
  if (compilationUnit == nullptr || parser.hasErrors()) {
    for (auto &error : parser.getErrors()) {
      ostringstream oss;
      oss << error;
      cerr << red(oss.str()) << endl;
    }
    return 1;
  } else {
    if (options.developerMode) {
      AstPrinter printer(cout);
      compilationUnit->accept(printer);
    }

    // TODO: Proper error handling, etc.
    auto mainWorker = make_shared<Worker>(options);
    mainWorker->executeProgram(compilationUnit);
    vm.addWorker(mainWorker);
    return vm.run();
  }
}

// This *must* be global because of the way GNU Readline works. :(
shared_ptr<Module> replModule;
char **manda_repl_completer(const char *text, int start, int end);
char *manda_completion_generator(const char *text, int state);