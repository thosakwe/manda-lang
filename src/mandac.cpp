#include "analysis/ast_printer.hpp"
#include "analysis/parser.hpp"
#include "analysis/scanner.hpp"
#include "compiler/manda_compiler.hpp"
#include "defs.hpp"
#include "ir/module_text_printer.hpp"
#include "runtime/ansi_printer.hpp"
#include "runtime/vm_options.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <sysexits.h>
#define tab "  "

using namespace manda::analysis;
using namespace manda::compiler;
using namespace manda::ir;
using namespace manda::runtime;
using namespace std;

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
    auto &filename = options.inputFile;
    ifstream ifs(filename);
    if (!ifs) {
      cerr << "Error: File \"" << filename << "\" does not exist." << endl;
      return 1;
    }

    string contents = {istreambuf_iterator<char>(ifs),
                       istreambuf_iterator<char>()};
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

      // Compile it
      MandaCompiler compiler;
      compiler.compile(*compilationUnit);
      ModuleTextPrinter(cout).dump(*compiler.getModule());
    }
  }
}

void printHelp(ostream &out) {
  out << "Usage: mandac [<options>] [<input_file>]" << endl;
  out << endl << "Compile the program in <input_file> into bytecode." << endl;
  out << endl << "Options:" << endl;
  out << "--help or -h" << endl;
  out << tab << "Print this help information." << endl;
  out << "--devel" << endl; // TODO: Only in `DEBUG` mode.
  out << tab << "Enable (extremely verbose) debugging information." << endl;
  out << "-DNAME[=VALUE] or --define NAME=VALUE" << endl;
  out << tab << "Define NAME to be VALUE, or empty." << endl;
  out << "--version or -v" << endl;
  out << tab << "Print the version of the compiler." << endl;
}