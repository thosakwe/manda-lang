#include "ast.hpp"

using namespace manda::analysis;
using namespace std;

ExprDeclCtx::ExprDeclCtx(ExprCtx *v) : value(v) { location = value->location; }

TypeDeclCtx::TypeDeclCtx(string n, TypeCtx *v) : name(move(n)), type(v) {
  location = type->location;
}

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
