#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "analyzer.hpp"
#include "ast.hpp"
#include "module.hpp"
#include "scope.hpp"
#include <optional>
#include <stack>

namespace manda::analysis {
class ModuleCompiler : public manda::analysis::CompilationUnitVisitor,
                       public manda::analysis::DeclVisitor {
public:
  Analyzer &analyzer;
  std::shared_ptr<Module> module;

  explicit ModuleCompiler(Analyzer &analyzer);
  ModuleCompiler(Analyzer &analyzer, std::shared_ptr<Module> &module);
  void visitCompilationUnit(analysis::CompilationUnitCtx &ctx) override;
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;

  void visitFnDecl(analysis::FnDeclExprCtx &ctx,
                   const std::shared_ptr<Scope> &scope);
};
} // namespace manda::analysis

#endif
