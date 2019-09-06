#ifndef MANDA_MODULE_COMPILER_HPP
#define MANDA_MODULE_COMPILER_HPP
#include "../analysis/ast.hpp"
#include "module.hpp"
#include "symbol_table.hpp"
#include <optional>
#include <stack>

namespace manda::runtime {
class ModuleCompiler : public manda::analysis::CompilationUnitVisitor,
                       public manda::analysis::DeclVisitor,
                       public manda::analysis::ExprVisitor {
public:
  ModuleCompiler();
  Module &getModule();
  void visitCompilationUnit(analysis::CompilationUnitCtx &ctx) override;
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;
  void visitVarExpr(analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(analysis::FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(analysis::VoidLiteralCtx &ctx) override;
  void visitIdExpr(analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(analysis::TupleExprCtx &ctx) override;
  void visitCastExpr(analysis::CastExprCtx &ctx) override;
  void visitCallExpr(analysis::CallExprCtx &ctx) override;
  void visitParenExpr(analysis::ParenExprCtx &ctx) override;

private:
  Module module;
  std::optional<std::shared_ptr<Object>> lastObject;
  std::stack<std::shared_ptr<SymbolTable>> scopeStack;
};
} // namespace manda::runtime

#endif
