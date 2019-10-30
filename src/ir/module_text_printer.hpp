#ifndef MANDA_MODULE_TEXT_PRINTER_HPP
#define MANDA_MODULE_TEXT_PRINTER_HPP
#include "../analysis/indenting_printer.hpp"
#include "ir_module.hpp"

namespace manda::ir {
class ModuleTextPrinter : public manda::analysis::IndentingPrinter {
public:
  explicit ModuleTextPrinter(std::ostream &out);
  void dump(const IRModule &module);
};
} // namespace manda::ir

#endif
