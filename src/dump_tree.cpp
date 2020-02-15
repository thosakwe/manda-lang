#include "analysis/ast_printer.hpp"
#include "analysis/parser.hpp"
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>

using namespace manda::analysis;
using namespace std;

int main() {
  cout << "Enter any Manda text to have it parsed." << endl;
  cout << "Type QUIT to exit." << endl;

  while (true) {
    string line(readline("dump_tree> "));
    if (line == "QUIT") {
      break;
    } else {
      add_history(line.c_str());
      Scanner scanner("stdin", line);
      Parser parser(scanner);
      auto ctx = parser.parseCompilationUnit();
      if (!ctx) {
        cout << "No compilation unit..." << endl;
      } else {
        AstPrinter printer(cout);
        ctx->accept(printer);
      }
    }
  }
}