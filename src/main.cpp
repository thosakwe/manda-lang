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
  std::string filename(argv[1]);
  std::ifstream ifs(filename);
  std::shared_ptr<manda::analysis::CompilationUnitCtx> compilationUnit;

  if (!ifs) {
    std::cerr << "Error: File \"" << filename << "\" does not exist."
              << std::endl;
    return 1;
  }

  std::string contents = {std::istreambuf_iterator<char>(ifs),
                          std::istreambuf_iterator<char>()};
  manda::analysis::Scanner scanner(filename, contents);
  manda::analysis::Parser parser(scanner);
  scanner.scan();
  compilationUnit = parser.parseCompilationUnit();

  // TODO: Dump any errors...

  manda::runtime::VM vm;
  manda::runtime::Worker mainWorker;
  mainWorker.loadCompilationUnit(compilationUnit);
  vm.addWorker(mainWorker);
  return vm.run();
}

int runRepl(int argc, char **argv) {
  // TODO: Run from file...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (true) {
    std::string line(readline("manda> "));
    add_history(line.c_str());
  }
#pragma clang diagnostic pop
}