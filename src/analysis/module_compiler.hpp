#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "ast.hpp"
#include "interpreter.hpp"
#include "module.hpp"
#include "symbol_table.hpp"
#include "vm_options.hpp"
#include <optional>
#include <stack>

namespace manda::analysis {
class ModuleCompiler : public manda::analysis::CompilationUnitVisitor,
                       public manda::analysis::DeclVisitor {
public:
  explicit ModuleCompiler(Interpreter &interpreter);
  ModuleCompiler(Interpreter &interpreter, std::shared_ptr<Module> &module);
  std::shared_ptr<Module> &getModule();
  void visitCompilationUnit(analysis::CompilationUnitCtx &ctx) override;
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;

  void visitFnDecl(analysis::FnDeclExprCtx &ctx, UnifiedScope& scope);

private:
  Interpreter &interpreter;
  std::shared_ptr<Module> module;
  std::optional<std::shared_ptr<Object>> lastObject;
  std::stack<std::shared_ptr<RuntimeScope>> scopeStack;
};
} // namespace manda::analysis

#endif
