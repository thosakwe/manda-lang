#ifndef MANDA_ANALYSIS_AST_HPP
#define MANDA_ANALYSIS_AST_HPP
#include "expr_ctx.hpp"

namespace manda::analysis {
class CompilationUnitVisitor;
class DeclVisitor;

struct DeclCtx {
  Location location;
  DeclCtx() = default;
  DeclCtx(const DeclCtx &) = default;
  DeclCtx(DeclCtx &&) = default;
  DeclCtx &operator=(const DeclCtx &) = default;
  DeclCtx &operator=(DeclCtx &&) = default;
  virtual ~DeclCtx() = default;
  virtual void accept(DeclVisitor &visitor) = 0;
};

struct ExprDeclCtx : public DeclCtx {
  std::shared_ptr<ExprCtx> value;
  ExprDeclCtx() = default;
  explicit ExprDeclCtx(std::shared_ptr<ExprCtx> &v);
  void accept(DeclVisitor &visitor) override;
};

struct TypeDeclCtx : public DeclCtx {
  std::string name;
  std::vector<std::string> typeParameters;
  std::shared_ptr<TypeCtx> type;
  TypeDeclCtx() = default;
  TypeDeclCtx(std::string n, TypeCtx *v);
  void accept(DeclVisitor &visitor) override;
};

class DeclVisitor {
public:
  virtual void visitExprDecl(ExprDeclCtx &ctx) = 0;
  virtual void visitTypeDecl(TypeDeclCtx &ctx) = 0;
};

struct CompilationUnitCtx {
  // TODO: Location
  Location location;
  std::vector<std::shared_ptr<DeclCtx>> declarations;
  void accept(CompilationUnitVisitor &visitor);
};

class CompilationUnitVisitor {
public:
  virtual void visitCompilationUnit(CompilationUnitCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
