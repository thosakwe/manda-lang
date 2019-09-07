#include "analysis/ast_printer.hpp"
#include "analysis/parser.hpp"
#include "analysis/scanner.hpp"
#include "defs.hpp"
#include "runtime/core_library.hpp"
#include "runtime/interpreter.hpp"
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
#define tab "  "

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
  } else if (options.showVersion) {
    cout << "Manda " << MANDA_VERSION << endl;
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
  scanner.scan();
  auto compilationUnit = parser.parseCompilationUnit();
  if (compilationUnit == nullptr) {
    cout << "NULL" << endl;
    return 1;
  } else {
    if (options.developerMode) {
      AstPrinter printer(cout);
      compilationUnit->accept(printer);
    }
    Worker mainWorker(options);
    mainWorker.executeProgram(compilationUnit);
    //  vm.addWorker(mainWorker.shared_from_this());
    return vm.run();
  }
}

// This *must* be global because of the way GNU Readline works. :(
shared_ptr<Module> replModule;
char **manda_repl_completer(const char *text, int start, int end);
char *manda_completion_generator(const char *text, int state);

int runREPL(const VMOptions &options) {
  // TODO: Put version information, etc. in a consistent location.
  // TODO: Include general usage information, like commands, etc.
  cout << "Welcome to Manda " << MANDA_VERSION << "!" << endl;
  cout << "Type \".help\" for usage information." << endl;
  VM vm(options);
  auto module = replModule = make_shared<Module>("<stdin>");
  CoreLibrary::install(*(module->getSymbolTable()));
  rl_attempted_completion_function = manda_repl_completer;
  while (true) {
    string line(readline("> "));
    add_history(line.c_str());

    if (line == ".quit" || line == ".q") {
      break;
    } else if (line == ".help") {
      cout << "Commands:" << endl;
      cout << tab << ".help     Print this help information." << endl;
      cout << tab << ".quit,.q  Exit the Manda REPL." << endl;
      continue;
    }

    Scanner scanner("<stdin>", line);
    Parser parser(scanner);
    scanner.scan();
    auto compilationUnit = parser.parseCompilationUnit();
    if (compilationUnit == nullptr) {
      cout << "NULL" << endl;
      return 1;
    } else {
      if (options.developerMode) {
        AstPrinter printer(cout);
        compilationUnit->accept(printer);
      }
      module->getTopLevelExpressions().clear();
      ModuleCompiler compiler(options, module);
      compilationUnit->accept(compiler);
      for (auto &node : module->getTopLevelExpressions()) {
        Interpreter interpreter(options, module);
        node->accept(interpreter);
        auto result = interpreter.getLastObject();
        if (result) {
          auto ptr = *result;
          if (ptr) {
            ptr->print(cout, true);
          }
        }
        cout << endl;
      }
    }
  }
  return 0;
}

char **manda_repl_completer(const char *text, int start, int end) {
  rl_attempted_completion_over = 1;
  return rl_completion_matches(text, manda_completion_generator);
}

vector<string> manda_completion_matches;
size_t manda_completion_match_index = 0;

char *manda_completion_generator(const char *text, int state) {
  if (state == 0) {
    string search(text);
    manda_completion_match_index = 0;
    manda_completion_matches.clear();
    for (auto &p : replModule->getSymbolTable()->getSymbols()) {
      if (p.first.compare(0, search.length(), search) == 0) {
        manda_completion_matches.push_back(p.first);
      }
    }
  }
  if (manda_completion_match_index >= manda_completion_matches.size()) {
    return nullptr;
  } else {
    return strdup(
        manda_completion_matches[manda_completion_match_index++].c_str());
  }
}