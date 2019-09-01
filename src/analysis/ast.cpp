#include "ast.hpp"

void manda::analysis::ExprDeclCtx::accept(
    manda::analysis::DeclVisitor &visitor) {
  visitor.visitExprDecl(*this);
}

void manda::analysis::TypeDeclCtx::accept(
    manda::analysis::DeclVisitor &visitor) {
  visitor.visitTypeDecl(*this);
}
