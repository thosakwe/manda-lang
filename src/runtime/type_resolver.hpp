#ifndef MANDA_TYPE_RESOLVER_HPP
#define MANDA_TYPE_RESOLVER_HPP
#include "../analysis/type_ctx.hpp"
#include "base_resolver.hpp"
#include "interpreter.hpp"
#include "symbol_table.hpp"
#include "type.hpp"
#include <memory>
#include <vector>

namespace manda::runtime {
class TypeResolver : public manda::analysis::TypeVisitor,
//                     public manda::analysis::ExprVisitor,
                     public BaseResolver {
public:
  TypeResolver(Interpreter &interpreter, const UnifiedScope &scope);
//  [[nodiscard]] const std::shared_ptr<Type> &getLastType() const;
//  void visitTypeRef(analysis::TypeRefCtx &ctx) override;
//  void visitVarExpr(const analysis::VarExprCtx &ctx) override;
//  void visitFnDeclExpr(const analysis::FnDeclExprCtx &ctx) override;
//  void visitVoidLiteral(const analysis::VoidLiteralCtx &ctx) override;
//  void visitIdExpr(const analysis::IdExprCtx &ctx) override;
//  void visitNumberLiteral(const analysis::NumberLiteralCtx &ctx) override;
//  void visitStringLiteral(const analysis::StringLiteralCtx &ctx) override;
//  void visitBoolLiteral(const analysis::BoolLiteralCtx &ctx) override;
//  void visitBlockExpr(const analysis::BlockExprCtx &ctx) override;
//  void visitTupleExpr(const analysis::TupleExprCtx &ctx) override;
//  void visitListExpr(const analysis::ListExprCtx &ctx) override;
//  void visitCastExpr(const analysis::CastExprCtx &ctx) override;
//  void visitCallExpr(const analysis::CallExprCtx &ctx) override;
//  void visitParenExpr(const analysis::ParenExprCtx &ctx) override;
//  void visitIfExpr(const analysis::IfExprCtx &ctx) override;

  /**
   * Ensures that the expression in the clause is a boolean, and returns the
   * resolution of the body.
   * @return Returns the resolved body type, or nullptr if something went wrong.
   */
  std::shared_ptr<Type> visitIfClause(const analysis::IfClauseCtx &ctx);

  /**
   * Attempts to find the common ancestor of the two given types.
   * @return Returns the common ancestor, or Any.
   */
  std::shared_ptr<Type> findCommonAncestor(const std::shared_ptr<Type> &left,
                                           const std::shared_ptr<Type> &right);

  /**
   * Computes the type hierarchy, up to a given type.
   * @param type
   * @return Returns a vector containing the given type and all of its parents, in order. The last will be the root type.
   */
  std::vector<std::shared_ptr<Type>> findPathToRoot(const std::shared_ptr<Type> &type);

private:
  Interpreter &interpreter;
  //  std::shared_ptr<RuntimeScope> scope;
  //  std::stack<std::shared_ptr<TypeScope>> typeScopeStack;
  std::shared_ptr<Type> lastType;
};
} // namespace manda::runtime

#endif
