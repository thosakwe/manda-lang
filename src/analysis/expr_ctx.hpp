#ifndef MANDA_EXPR_CTX_HPP
#define MANDA_EXPR_CTX_HPP
#include "location.hpp"
#include "type_ctx.hpp"
#include <memory>
#include <string>
#include <vector>

namespace manda::analysis {
class ExprCtx {
public:
  Location location;
};

class TopLevelExprCtx : public ExprCtx {
  // Visibility?
  bool isPublic;
  std::string name;
};

struct VarExprCtx : public TopLevelExprCtx {
  bool isFinal;
  std::shared_ptr<ExprCtx> value;
};

struct ParamCtx {
  Location location;
  std::string name;
  std::shared_ptr<TypeCtx> type;
  std::shared_ptr<ExprCtx> defaultValue;
};

struct FnDeclCtx : public TopLevelExprCtx {
  std::vector<std::shared_ptr<ParamCtx>> params;
  std::shared_ptr<TypeCtx> returnType;
  std::shared_ptr<ExprCtx> body;
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
  explicit BoolLiteralCtx(bool value);
  bool value;
};

struct BlockExprCtx : public ExprCtx {
  std::vector<std::shared_ptr<ExprCtx>> body;
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
} // namespace manda::analysis

#endif
