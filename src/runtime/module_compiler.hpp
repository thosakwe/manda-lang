#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "../analysis/ast.hpp"
#include "interpreter.hpp"
#include "module.hpp"
#include "symbol_table.hpp"
#include "vm_options.hpp"
#include <optional>
#include <stack>

namespace manda::runtime {
class ModuleCompiler : public manda::analysis::CompilationUnitVisitor,
                       public manda::analysis::DeclVisitor {
public:
  explicit ModuleCompiler(Interpreter &interpreter);
  ModuleCompiler(Interpreter &interpreter, std::shared_ptr<Module> &module);
  std::shared_ptr<Module> &getModule();
  void visitCompilationUnit(analysis::CompilationUnitCtx &ctx) override;
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;

private:
  Interpreter &interpreter;
  std::shared_ptr<Module> module;
  std::optional<std::shared_ptr<Object>> lastObject;
  std::stack<std::shared_ptr<SymbolTable>> scopeStack;
};
} // namespace manda::runtime

#endif
