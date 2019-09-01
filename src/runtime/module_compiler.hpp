#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "../analysis/ast.hpp"
#include "module.hpp"

namespace manda::runtime {
class ModuleCompiler : public manda::analysis::DeclVisitor,
                       public manda::analysis::ExprVisitor {
public:
  ModuleCompiler();
  Module &getModule();
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;
  void visitVarExpr(analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(analysis::FnDeclExprCtx &ctx) override;
  void visitVoidExpr(analysis::VoidExprCtx &ctx) override;
  void visitIdExpr(analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(analysis::TupleExprCtx &ctx) override;
  void visitCastExpr(analysis::CastExprCtx &ctx) override;
  void visitCallExpr(analysis::CallExprCtx &ctx) override;

private:
  Module module;
};
} // namespace manda::runtime

#endif
