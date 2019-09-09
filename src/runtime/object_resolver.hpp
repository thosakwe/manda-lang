#ifndef MANDA_OBJECT_RESOLVER_HPP
#define MANDA_OBJECT_RESOLVER_HPP
#include "../analysis/expr_ctx.hpp"
#include "base_resolver.hpp"
#include "interpreter.hpp"
#include "object.hpp"
#include "symbol_table.hpp"
#include <memory>

namespace manda::runtime {
class ObjectResolver : public manda::analysis::ExprVisitor,
                       public BaseResolver {
public:
  ObjectResolver(Interpreter &interpreter, const UnifiedScope &scope);
  [[nodiscard]] const std::shared_ptr<Object> &getLastObject() const;
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
  std::shared_ptr<Object> lastObject;
};
} // namespace manda::runtime

#endif
