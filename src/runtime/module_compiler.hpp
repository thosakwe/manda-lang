#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "../analysis/ast.hpp"
#include "module.hpp"

namespace manda::runtime {
class ModuleCompiler {
public:
  ModuleCompiler();
  Module &getModule();

private:
  Module module;
};
} // namespace manda::runtime

#endif
