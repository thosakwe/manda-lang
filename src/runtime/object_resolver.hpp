//#ifndef MANDA_OBJECT_RESOLVER_HPP
//#define MANDA_OBJECT_RESOLVER_HPP
//#include "../analysis/expr_ctx.hpp"
//#include "base_resolver.hpp"
//#include "interpreter.hpp"
//#include "object.hpp"
//#include "symbol_table.hpp"
//#include <memory>
//
//namespace manda::runtime {
//class ObjectResolver : public manda::analysis::ExprVisitor,
//                       public BaseResolver {
//public:
//  ObjectResolver(Interpreter &interpreter, const UnifiedScope &scope);
////  [[nodiscard]] const std::shared_ptr<Object> &getLastObject() const;
//  void visitVarExpr(analysis::VarExprCtx &ctx) override;
//  void visitFnDeclExpr(analysis::FnDeclExprCtx &ctx) override;
//  void visitVoidLiteral(analysis::VoidLiteralCtx &ctx) override;
//  void visitIdExpr(analysis::IdExprCtx &ctx) override;
//  void visitNumberLiteral(analysis::NumberLiteralCtx &ctx) override;
//  void visitStringLiteral(analysis::StringLiteralCtx &ctx) override;
//  void visitBoolLiteral(analysis::BoolLiteralCtx &ctx) override;
//  void visitBlockExpr(analysis::BlockExprCtx &ctx) override;
//  void visitTupleExpr(analysis::TupleExprCtx &ctx) override;
//  void visitListExpr(analysis::ListExprCtx &ctx) override;
//  void visitCastExpr(analysis::CastExprCtx &ctx) override;
//  void visitCallExpr(analysis::CallExprCtx &ctx) override;
//  void visitParenExpr(analysis::ParenExprCtx &ctx) override;
//  void visitIfExpr(analysis::IfExprCtx &ctx) override;
//
//  /**
//   * Tries to evaluate the value of a clause, in the given scope.
//   * @param ctx
//   * @return Returns nullptr if the condition was false.
//   */
//  std::shared_ptr<Object> visitIfClause(const analysis::IfClauseCtx & ctx);
//
//private:
//  Interpreter &interpreter;
////  std::shared_ptr<Object> lastObject;
//};
//} // namespace manda::runtime
//
//#endif
