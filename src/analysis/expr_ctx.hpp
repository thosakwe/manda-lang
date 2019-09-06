#ifndef MANDA_EXPR_CTX_HPP
#define MANDA_EXPR_CTX_HPP
#include "location.hpp"
#include "type_ctx.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace manda::analysis {
class ExprVisitor;

class ExprCtx {
public:
  Location location;
  ExprCtx() = default;
  ExprCtx(const ExprCtx &) = default;
  ExprCtx(ExprCtx &&) = default;
  ExprCtx &operator=(const ExprCtx &) = default;
  ExprCtx &operator=(ExprCtx &&) = default;
  virtual ~ExprCtx() = default;
  virtual void accept(ExprVisitor &visitor) = 0;
};

template <typename T> struct AstList {
  ExprCtx *value;
  AstList *next;
  explicit AstList(ExprCtx *v) : value(v) { next = nullptr; }
  AstList(const AstList &) = delete;
  AstList(AstList &&) = delete;
  AstList &operator=(const AstList &) = delete;
  AstList &operator=(AstList &&) = delete;
  ~AstList() {
    if (next != nullptr) {
      // Don't delete *value, we passed ownership to unique_ptr.
      delete next;
      next = nullptr;
    }
  }
  AstList<T> *add(AstList<T> *other) {
    if (other != nullptr) {
      auto *current = this;
      while (current->next != nullptr) {
        current = current->next;
      }
      current->next = other;
    }
    return this;
  }
};

template <typename T>
void toVector(AstList<T> *head, std::vector<std::unique_ptr<T>> &out) {
  auto *current = head;
  while (current != nullptr) {
    out.emplace_back(current->value);
    current = current->next;
  }
  delete head; // Destroy it!!!
}

class TopLevelExprCtx : public ExprCtx {
public:
  TopLevelExprCtx() = default;
  TopLevelExprCtx(const TopLevelExprCtx &) = default;
  TopLevelExprCtx(TopLevelExprCtx &&) = default;
  TopLevelExprCtx &operator=(const TopLevelExprCtx &) = default;
  TopLevelExprCtx &operator=(TopLevelExprCtx &&) = default;

private:
  // Visibility?
  bool isPublic;
  std::string name;
};

struct VarExprCtx : public TopLevelExprCtx {
  bool isFinal;
  std::string name;
  std::unique_ptr<ExprCtx> value;
  VarExprCtx(bool f, std::string n, ExprCtx *v)
      : isFinal(f), name(std::move(n)), value(v) {}
  void accept(ExprVisitor &visitor) override;
};

struct ParamCtx {
  Location location;
  std::string name;
  std::unique_ptr<TypeCtx> type;
  std::unique_ptr<ExprCtx> defaultValue;
};

struct FnDeclExprCtx : public TopLevelExprCtx {
  std::vector<std::unique_ptr<ParamCtx>> params;
  std::unique_ptr<TypeCtx> returnType;
  std::unique_ptr<ExprCtx> body;
  void accept(ExprVisitor &visitor) override;
};

struct VoidLiteralCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
};

struct IdExprCtx : public ExprCtx {
  std::string name;
  IdExprCtx(const Location &l, std::string n) : name(std::move(n)) {
    location = l;
  }
  void accept(ExprVisitor &visitor) override;
};

class NumberLiteralCtx : public ExprCtx {
public:
  double value;
  NumberLiteralCtx(const Location &l, double v) : value(v) { location = l; }
  void accept(ExprVisitor &visitor) override;
};

class StringLiteralCtx : public ExprCtx {
public:
  void accept(ExprVisitor &visitor) override;

public:
  std::string value;
};

struct BoolLiteralCtx : public ExprCtx {
  bool value;
  explicit BoolLiteralCtx(bool v) : value(v) {}
  void accept(ExprVisitor &visitor) override;
};

struct BlockExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::vector<std::unique_ptr<ExprCtx>> body;
};

struct TupleExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::vector<std::unique_ptr<ExprCtx>> items;
};

struct CastExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) override;
  std::unique_ptr<ExprCtx> value;
  std::unique_ptr<TypeCtx> type;
};

struct CallExprCtx : public ExprCtx {
  explicit CallExprCtx(ExprCtx *tgt) : target(tgt) {}
  void accept(ExprVisitor &visitor) override;
  std::unique_ptr<ExprCtx> target;
  std::vector<std::unique_ptr<ExprCtx>> arguments;
};

class ExprVisitor {
public:
  virtual void visitVarExpr(VarExprCtx &ctx) = 0;
  virtual void visitFnDeclExpr(FnDeclExprCtx &ctx) = 0;
  virtual void visitVoidLiteral(VoidLiteralCtx &ctx) = 0;
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
