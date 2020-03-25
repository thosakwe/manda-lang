#ifndef MANDA_TYPE_RESOLVER_HPP
#define MANDA_TYPE_RESOLVER_HPP
#include "analyzer.hpp"
#include "base_resolver.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "type_ctx.hpp"
#include <memory>
#include <vector>

namespace manda::analysis {
class TypeResolver : public manda::analysis::TypeVisitor,
                     public manda::analysis::ExprVisitor,
                     public BaseResolver {
public:
  TypeResolver(Analyzer &analyzer, const std::shared_ptr<Scope> &scope);
  //  [[nodiscard]] const std::shared_ptr<Type> &getLastType() const;
  void visitTypeRef(analysis::TypeRefCtx &ctx) override;
  void visitVarExpr(analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(analysis::FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(analysis::VoidLiteralCtx &ctx) override;
  void visitIdExpr(analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(analysis::TupleExprCtx &ctx) override;
  void visitListExpr(analysis::ListExprCtx &ctx) override;
  void visitCastExpr(analysis::CastExprCtx &ctx) override;
  void visitCallExpr(analysis::CallExprCtx &ctx) override;
  void visitParenExpr(analysis::ParenExprCtx &ctx) override;
  void visitIfExpr(analysis::IfExprCtx &ctx) override;

  /**
   * Ensures that the expression in the clause is a boolean, and returns the
   * resolution of the body.
   * @return Returns the resolved body type, or nullptr if something went wrong.
   */
  std::shared_ptr<Type> visitIfClause(analysis::IfClauseCtx &ctx);

  /**
   * Attempts to find the common ancestor of the two given types.
   * @return Returns the common ancestor, or Any.
   */
  std::shared_ptr<Type> findCommonAncestor(const std::shared_ptr<Type> &left,
                                           const std::shared_ptr<Type> &right);

  /**
   * Computes the type hierarchy, up to a given type.
   * @param type
   * @return Returns a vector containing the given type and all of its parents,
   * in order. The last will be the root type.
   */
  std::vector<std::shared_ptr<Type>>
  findPathToRoot(const std::shared_ptr<Type> &type);

private:
  Analyzer &analyzer;
  //  std::shared_ptr<RuntimeScope> scope;
  //  std::stack<std::shared_ptr<TypeScope>> typeScopeStack;
  //  std::shared_ptr<Type> lastType;
};
} // namespace manda::analysis

#endif
