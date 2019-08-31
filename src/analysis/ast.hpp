#ifndef MANDA_ANALYSIS_AST_HPP
#define MANDA_ANALYSIS_AST_HPP
#include "expr_ctx.hpp"

namespace manda {
namespace analysis {
struct DeclCtx {
  Location location;
};

struct ExprDeclCtx : public DeclCtx {
  std::shared_ptr<ExprCtx> value;
};

struct TypeDeclCtx : public DeclCtx {
  std::string name;
  std::vector<std::string> typeParameters;
  std::shared_ptr<TypeCtx> type;
};

struct CompilationUnitCtx {
  Location location;
  std::vector<std::shared_ptr<DeclCtx>> declarations;
};
} // namespace analysis
} // namespace manda

#endif
