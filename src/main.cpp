#include "analysis/ast_printer.hpp"
#include "analysis/parser.hpp"
#include "analysis/scanner.hpp"
#include "runtime/module_compiler.hpp"
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

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

int runFile(const VMOptions &options);
int runREPL(const VMOptions &options);
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
  } else if (options.isREPL()) {
    return runREPL(options);
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
  auto tab = "  ";
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
  scanner.scan();
  auto compilationUnit = parser.parseCompilationUnit();
  if (compilationUnit == nullptr) {
    cout << "NULL" << endl;
    return 1;
  } else {
    AstPrinter printer(cout);
    compilationUnit->accept(printer);
    Worker mainWorker(options);
    mainWorker.executeProgram(compilationUnit);
    //  vm.addWorker(mainWorker.shared_from_this());
    return vm.run();
  }
}

int runREPL(const VMOptions &options) {
  // TODO: Run from file...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  VM vm(options);
  auto module = make_shared<Module>("<stdin>");
  while (true) {
    string line(readline("manda> "));
    add_history(line.c_str());
    Scanner scanner("<stdin>", line);
    Parser parser(scanner);
    scanner.scan();
    auto compilationUnit = parser.parseCompilationUnit();
    if (compilationUnit == nullptr) {
      cout << "NULL" << endl;
      return 1;
    } else {
      AstPrinter printer(cout);
      compilationUnit->accept(printer);
      module->getTopLevelExpressions().clear();
      ModuleCompiler compiler(options, module);
      compilationUnit->accept(compiler);
      for (auto &node : module->getTopLevelExpressions()) {
        cout << "Found top-level" << endl;
        // TODO: Actually execute
      }
    }
  }
#pragma clang diagnostic pop
}
