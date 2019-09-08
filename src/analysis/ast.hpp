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
  virtual void accept(DeclVisitor &visitor) const = 0;
  virtual DeclCtx *clone() const = 0;
  std::unique_ptr<DeclCtx> cloneToUniquePointer() const;
};

struct ExprDeclCtx : public DeclCtx {
  std::unique_ptr<ExprCtx> value;
  ExprDeclCtx() = default;
  explicit ExprDeclCtx(ExprCtx *v);
  void accept(DeclVisitor &visitor) const override;
  ExprDeclCtx *clone() const override;
};

struct TypeDeclCtx : public DeclCtx {
  std::string name;
  std::vector<std::string> typeParameters;
  std::unique_ptr<TypeCtx> type;
  TypeDeclCtx() = default;
  TypeDeclCtx(std::string n, TypeCtx *v);
  void accept(DeclVisitor &visitor) const override;
  TypeDeclCtx *clone() const override;
};

class DeclVisitor {
public:
  virtual void visitExprDecl(const ExprDeclCtx &ctx) = 0;
  virtual void visitTypeDecl(const TypeDeclCtx &ctx) = 0;
};

struct CompilationUnitCtx {
  // TODO: Location
  Location location;
  std::vector<std::unique_ptr<DeclCtx>> declarations;
  void accept(CompilationUnitVisitor &visitor) const;
  CompilationUnitCtx *clone() const;
  std::unique_ptr<CompilationUnitCtx> cloneToUniquePointer() const;
};

class CompilationUnitVisitor {
public:
  virtual void visitCompilationUnit(const CompilationUnitCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
