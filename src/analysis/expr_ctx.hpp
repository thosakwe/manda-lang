#ifndef MANDA_EXPR_CTX_HPP
#define MANDA_EXPR_CTX_HPP
#include "location.hpp"
#include "type_ctx.hpp"
#include <memory>
#include <string>
#include <vector>

namespace manda::analysis {
class ExprVisitor;

class ExprCtx {
public:
  Location location;
  ExprCtx(const ExprCtx &) = default;
  ExprCtx(ExprCtx &&) = default;
  ExprCtx &operator=(const ExprCtx &) = default;
  ExprCtx &operator=(ExprCtx &&) = default;
  virtual ~ExprCtx() = default;
  virtual void accept(ExprVisitor &visitor) = 0;
};

class TopLevelExprCtx : public ExprCtx {
public:
  void accept(ExprVisitor &visitor) override;
private:
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

struct FnDeclExprCtx : public TopLevelExprCtx {
  std::vector<std::shared_ptr<ParamCtx>> params;
  std::shared_ptr<TypeCtx> returnType;
  std::shared_ptr<ExprCtx> body;
};

struct VoidExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
};

struct IdExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::string name;
};

class NumberLiteralCtx : public ExprCtx {
public:
  void accept(ExprVisitor &visitor) override;

public:
  double value;
};

class StringLiteralCtx : public ExprCtx {
public:
  void accept(ExprVisitor &visitor) override;

public:
  std::string value;
};

class BoolLiteralCtx : public ExprCtx {
public:
  void accept(ExprVisitor &visitor) override;

public:
  bool value;
};

struct BlockExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::vector<std::shared_ptr<ExprCtx>> body;
};

struct TupleExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::vector<std::shared_ptr<ExprCtx>> items;
};

struct CastExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::shared_ptr<ExprCtx> value;
  std::shared_ptr<TypeCtx> type;
};

struct CallExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::shared_ptr<ExprCtx> target;
  std::vector<std::shared_ptr<ExprCtx>> arguments;
};

class ExprVisitor {
public:
  virtual void visitVarExpr(VarExprCtx &ctx) = 0;
  virtual void visitFnDeclExpr(FnDeclExprCtx &ctx) = 0;
  virtual void visitVoidExpr(VoidExprCtx &ctx) = 0;
  virtual void visitIdExpr(IdExprCtx &ctx) = 0;
  virtual void visitNumberLiteral(NumberLiteralCtx &ctx) = 0;
  virtual void visitStringLiteral(StringLiteralCtx &ctx) = 0;
  virtual void visitBoolLiteral(BoolLiteralCtx &ctx) = 0;
  virtual void visitBlockExpr(BlockExprCtx &ctx) = 0;
  virtual void visitTupleExpr(TupleExprCtx &ctx) = 0;
  virtual void visitCastExpr(CastExprCtx &ctx) = 0;
  virtual void visitCallExpr(CallExprCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
