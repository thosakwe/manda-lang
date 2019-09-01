#ifndef MANDA_ANALYSIS_AST_HPP
#define MANDA_ANALYSIS_AST_HPP
#include "expr_ctx.hpp"

namespace manda::analysis {
class DeclVisitor;

struct DeclCtx {
  Location location;
  virtual void accept(DeclVisitor &visitor) = 0;
};

struct ExprDeclCtx : public DeclCtx {
  void accept(DeclVisitor &visitor) override;
  std::shared_ptr<ExprCtx> value;
};

struct TypeDeclCtx : public DeclCtx {
  void accept(DeclVisitor &visitor) override;
  std::string name;
  std::vector<std::string> typeParameters;
  std::shared_ptr<TypeCtx> type;
};

class DeclVisitor {
public:
  virtual void visitExprDecl(ExprDeclCtx &ctx) = 0;
  virtual void visitTypeDecl(TypeDeclCtx &ctx) = 0;
};

struct CompilationUnitCtx {
  Location location;
  std::vector<std::shared_ptr<DeclCtx>> declarations;
};
} // namespace manda::analysis

#endif
