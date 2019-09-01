#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "../analysis/ast.hpp"
#include "module.hpp"

namespace manda::runtime {
class ModuleCompiler : public manda::analysis::DeclVisitor {
public:
  ModuleCompiler();
  Module &getModule();
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;

private:
  Module module;
};
} // namespace manda::runtime

#endif
