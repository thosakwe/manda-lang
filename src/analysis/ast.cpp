#include "ast.hpp"

using namespace manda::analysis;
using namespace std;

ExprDeclCtx::ExprDeclCtx(std::unique_ptr<ExprCtx> &v) : value(std::move(v)) {
  location = value->location;
}

TypeDeclCtx::TypeDeclCtx(string n, TypeCtx *v) : name(move(n)), type(v) {
  location = type->location;
}

std::unique_ptr<DeclCtx> DeclCtx::cloneToUniquePointer() const {
  return unique_ptr<DeclCtx>(clone());
}

CompilationUnitCtx *CompilationUnitCtx::clone() const {
  auto *out = new CompilationUnitCtx;
  out->location = location;
  for (auto &ptr : declarations) {
    out->declarations.push_back(ptr->cloneToUniquePointer());
  }
  return out;
}

unique_ptr<CompilationUnitCtx>
CompilationUnitCtx::cloneToUniquePointer() const {
  return unique_ptr<CompilationUnitCtx>(clone());
}

ExprDeclCtx *ExprDeclCtx::clone() const {
  auto *out = new ExprDeclCtx;
  out->location = location;
  out->value = value->cloneToUniquePointer();
  return out;
}

TypeDeclCtx *TypeDeclCtx::clone() const {
  auto *out = new TypeDeclCtx;
  out->location = location;
  out->name = name;
  out->type = type->cloneToUniquePointer();
  out->typeParameters = vector<string>(typeParameters);
  return out;
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
