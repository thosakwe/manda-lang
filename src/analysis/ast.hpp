#ifndef MANDA_ANALYSIS_AST_HPP
#define MANDA_ANALYSIS_AST_HPP
#include "expr_ctx.hpp"

namespace manda::analysis {
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
  std::unique_ptr<ExprCtx> value;
  explicit ExprDeclCtx(ExprCtx *v) : value(v) { location = value->location; }
  void accept(DeclVisitor &visitor) override;
};

struct TypeDeclCtx : public DeclCtx {
  void accept(DeclVisitor &visitor) override;
  std::string name;
  std::vector<std::string> typeParameters;
  std::unique_ptr<TypeCtx> type;
};

class DeclVisitor {
public:
  virtual void visitExprDecl(ExprDeclCtx &ctx) = 0;
  virtual void visitTypeDecl(TypeDeclCtx &ctx) = 0;
};

struct CompilationUnitCtx {
  // TODO: Location
  Location location;
  std::vector<std::unique_ptr<DeclCtx>> declarations;
};
} // namespace manda::analysis

#endif
