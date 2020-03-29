#include "../analysis/ast_printer.hpp"
#include "../analysis/module_compiler.hpp"
#include "../analysis/type_resolver.hpp"
#include "../analysis/parser.hpp"
#include "../analysis/scanner.hpp"
#include "../runtime/ansi_printer.hpp"
#include "defs.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
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

    // The ModuleCompiler will initialize all symbols to "unresolved."
    Analyzer analyzer(options);
    ModuleCompiler moduleCompiler(analyzer);
    moduleCompiler.visitCompilationUnit(*compilationUnit);

    // Once we have collected all names, it is now time to find "main," and analyze it,
    // so that it can be executed.
    auto &module = moduleCompiler.module;
    auto main = module->getSymbolTable()->resolve("main");
    if (!main) {
      ostringstream oss;
      oss << "Error: File \"" << filename << R"(" does not expose a "main" function.)";
      cerr << red(oss.str()) << endl;
      return 1;
    } else if (!main->exprCtx) {
      ostringstream oss;
      oss << "The top-level \"main\" symbol must be an expression.";
      cerr << red(oss.str()) << endl;
      return 1;
    }

    TypeResolver mainResolver(analyzer, module->getSymbolTable());
    main->exprCtx->accept(mainResolver);
    main->type = main->exprCtx->runtimeType;

    // Just dump everything.
   for (auto &pair : module->getSymbolTable()->getSymbols()) {
     cout << "  * " << pair.first << ": " << pair.second->type->getName() << endl;
   }

    return 0;
  }
}