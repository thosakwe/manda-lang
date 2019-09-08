#include "ast.hpp"

void manda::analysis::ExprDeclCtx::accept(
    manda::analysis::DeclVisitor &visitor) const {
  visitor.visitExprDecl(*this);
}

void manda::analysis::TypeDeclCtx::accept(
    manda::analysis::DeclVisitor &visitor) const {
  visitor.visitTypeDecl(*this);
}

void manda::analysis::CompilationUnitCtx::accept(
    manda::analysis::CompilationUnitVisitor &visitor) const {
  visitor.visitCompilationUnit(*this);
}
