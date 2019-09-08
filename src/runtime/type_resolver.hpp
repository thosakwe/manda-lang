#ifndef MANDA_TYPE_RESOLVER_HPP
#define MANDA_TYPE_RESOLVER_HPP
#include "../analysis/type_ctx.hpp"
#include "interpreter.hpp"
#include "symbol_table.hpp"
#include "type.hpp"
#include <memory>

namespace manda::runtime {
class TypeResolver : public manda::analysis::TypeVisitor,
                     public manda::analysis::ExprVisitor {
public:
  TypeResolver(Interpreter &interpreter, std::shared_ptr<SymbolTable> scope);
  void pushTypeScope(const std::shared_ptr<TypeScope>& scope);
  [[nodiscard]] const std::shared_ptr<Type> &getLastType() const;
  void visitTypeRef(const analysis::TypeRefCtx &ctx) override;
  void visitVarExpr(const analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(const analysis::FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(const analysis::VoidLiteralCtx &ctx) override;
  void visitIdExpr(const analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(const analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(const analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(const analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(const analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(const analysis::TupleExprCtx &ctx) override;
  void visitCastExpr(const analysis::CastExprCtx &ctx) override;
  void visitCallExpr(const analysis::CallExprCtx &ctx) override;
  void visitParenExpr(const analysis::ParenExprCtx &ctx) override;

private:
  Interpreter &interpreter;
  std::shared_ptr<SymbolTable> scope;
  std::stack<std::shared_ptr<TypeScope>> typeScopeStack;
  std::shared_ptr<Type> lastType;
};
} // namespace manda::runtime

#endif
