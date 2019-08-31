#ifndef MANDA_EXPR_CTX_HPP
#define MANDA_EXPR_CTX_HPP
#include "location.hpp"
#include "type_ctx.hpp"
#include <memory>
#include <string>
#include <vector>

namespace manda {
namespace analysis {
class ExprCtx {
public:
  Location location;
};

struct VoidExprCtx : public ExprCtx {};

class NumberLiteralCtx : public ExprCtx {
public:
  explicit NumberLiteralCtx(double value);
  double value;
};

class StringLiteralCtx : public ExprCtx {
public:
  explicit StringLiteralCtx(std::string &value);
  std::string value;
};

class BoolLiteralCtx : public ExprCtx {
public:
  explicit BoolLiteralCtx(bool &value);
  bool value;
};

struct BlockExprCtx : public ExprCtx {
  std::vector<std::shared_ptr<ExprCtx>> body;
};

struct VarExprCtx : public ExprCtx {
  bool isFinal;
  std::string name;
  std::shared_ptr<ExprCtx> value;
};

struct TupleExprCtx : public ExprCtx {
  std::vector<std::shared_ptr<ExprCtx>> items;
};

struct CastExprCtx : public ExprCtx {
  std::shared_ptr<ExprCtx> value;
  std::shared_ptr<TypeCtx> type;
};

struct CallExprCtx : public ExprCtx {
  std::shared_ptr<ExprCtx> target;
  std::vector<std::shared_ptr<ExprCtx>> arguments;
};
} // namespace analysis
} // namespace manda

#endif
