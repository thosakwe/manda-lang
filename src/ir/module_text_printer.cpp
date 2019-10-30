#include "module_text_printer.hpp"

using namespace manda::ir;
using namespace std;

ModuleTextPrinter::ModuleTextPrinter(ostream &out) : IndentingPrinter(out) {}

void ModuleTextPrinter::dump(const IRModule &module) {
  print() << "Module " << (module.name.empty() ? "<anonymous>" : module.name)
          << endl;
  indent();
  for (auto &func : module.functions) {
    print() << "Function <" << func->name << ">" << endl;
    indent();
    for (auto &block : func->blocks) {
      print() << "Block <" << block->name << ">" << endl;
      indent();
      for (auto &instr : block->instructions) {
        print() << *instr << endl;
      }
      outdent();
    }
    outdent();
  }
  outdent();
}
