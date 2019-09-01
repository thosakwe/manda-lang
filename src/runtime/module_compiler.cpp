#include "module_compiler.hpp"

manda::runtime::ModuleCompiler::ModuleCompiler() : module{""} {}

manda::runtime::Module &manda::runtime::ModuleCompiler::getModule() {
  return module;
}
