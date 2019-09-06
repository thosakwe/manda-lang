#include "analysis/ast_printer.hpp"
#include "analysis/parser.hpp"
#include "analysis/scanner.hpp"
#include "runtime/vm.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <readline/history.h>
#include <readline/readline.h>
#include <sstream>
#include <string>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

int runFile(int argc, char **argv);
int runRepl(int argc, char **argv);

int main(int argc, char **argv) {
  // TODO: Parse options.
  if (argc < 2) {
    return runRepl(argc, argv);
  } else {
    return runFile(argc, argv);
  }
}

int runFile(int argc, char **argv) {
  string filename(argv[1]);
  ifstream ifs(filename);
  shared_ptr<CompilationUnitCtx> compilationUnit;

  if (!ifs) {
    cerr << "Error: File \"" << filename << "\" does not exist." << endl;
    return 1;
  }

  string contents = {istreambuf_iterator<char>(ifs),
                     istreambuf_iterator<char>()};
  Scanner scanner(filename, contents);
  Parser parser(scanner);
  scanner.scan();
  compilationUnit = parser.parseCompilationUnit();
  if (compilationUnit == nullptr) {
    cout << "NULL" << endl;
  } else {
    AstPrinter printer(cout);
    compilationUnit->accept(printer);
  }
  //   // TODO: Dump any errors...

  VM vm;
  Worker mainWorker;
  mainWorker.loadCompilationUnit(compilationUnit);
//  vm.addWorker(mainWorker.shared_from_this());
  return vm.run();
}

int runRepl(int argc, char **argv) {
  // TODO: Run from file...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (true) {
    string line(readline("manda> "));
    add_history(line.c_str());
    Scanner scanner("", line);
    Parser parser(scanner);
    scanner.scan();
    auto compilationUnit = parser.parseCompilationUnit();
    if (compilationUnit == nullptr) {
      cout << "NULL" << endl;
    } else {
      AstPrinter printer(cout);
      compilationUnit->accept(printer);
    }
  }
#pragma clang diagnostic pop
}
