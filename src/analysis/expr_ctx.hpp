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
  virtual void accept(ExprVisitor &visitor) const = 0;
};

template <typename T> struct AstList {
  T *value;
  AstList *next;
  explicit AstList(T *v) : value(v) { next = nullptr; }
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
  bool isPublic = false;
  std::string name;
};

struct VarExprCtx : public TopLevelExprCtx {
  // TODO: Set location
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
  void accept(ExprVisitor &visitor) const override;
};

struct VoidLiteralCtx : public ExprCtx {
  // TODO: Set location
  void accept(ExprVisitor &visitor) const override;
};

struct IdExprCtx : public ExprCtx {
  std::string name;
  // TODO: Set location
  IdExprCtx(const Location &l, std::string n) : name(std::move(n)) {
    location = l;
  }
  void accept(ExprVisitor &visitor) const override;
};

class NumberLiteralCtx : public ExprCtx {
public:
  // TODO: Set location
  double value;
  NumberLiteralCtx(const Location &l, double v) : value(v) { location = l; }
  void accept(ExprVisitor &visitor) const override;
};

class StringPartCtx {
public:
  // TODO: Set location
  Location location;
  [[nodiscard]] virtual std::string convert(bool singleQuote) const = 0;
  StringPartCtx() = default;
  StringPartCtx(const StringPartCtx &) = default;
  StringPartCtx(StringPartCtx &&) = default;
  StringPartCtx &operator=(const StringPartCtx &) = default;
  StringPartCtx &operator=(StringPartCtx &&) = default;
  virtual ~StringPartCtx() = default;
};

class TextStringPartCtx : public StringPartCtx {
public:
  std::string text;
  [[nodiscard]] std::string convert(bool singleQuote) const override;

  TextStringPartCtx(const Location &l, std::string t) : text(std::move(t)) {
    location = l;
  }
};

class HexEscapeStringPartCtx : public StringPartCtx {
public:
  std::string text;
  [[nodiscard]] std::string convert(bool singleQuote) const override;

  HexEscapeStringPartCtx(const Location &l, std::string t)
      : text(std::move(t)) {
    location = l;
  }
};

class QuoteEscapeStringPartCtx : public StringPartCtx {
public:
  [[nodiscard]] std::string convert(bool singleQuote) const override;
  explicit QuoteEscapeStringPartCtx(const Location &l) { location = l; }
};

class StringLiteralCtx : public ExprCtx {
public:
  // TODO: Location
  explicit StringLiteralCtx(bool sq) : singleQuote(sq) {}
  void accept(ExprVisitor &visitor) const override;
  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] bool isChar() const;

public:
  bool singleQuote;
  std::string value;
  std::vector<std::unique_ptr<StringPartCtx>> parts;
};

struct BoolLiteralCtx : public ExprCtx {
  // TODO: Set location
  bool value;
  explicit BoolLiteralCtx(bool v) : value(v) {}
  void accept(ExprVisitor &visitor) const override;
};

struct BlockExprCtx : public ExprCtx {
  // TODO: Set location
  void accept(ExprVisitor &visitor) const override;
  std::vector<std::unique_ptr<ExprCtx>> body;
};

struct TupleExprCtx : public ExprCtx {
  // TODO: Set location
  void accept(ExprVisitor &visitor) const override;
  std::vector<std::unique_ptr<ExprCtx>> items;
};

struct CastExprCtx : public ExprCtx {
  void accept(ExprVisitor &visitor) const override;
  std::unique_ptr<ExprCtx> value;
  std::unique_ptr<TypeCtx> type;
};

struct CallExprCtx : public ExprCtx {
  explicit CallExprCtx(ExprCtx *tgt) : target(tgt) {
    location = target->location;
  }
  void accept(ExprVisitor &visitor) const override;
  std::unique_ptr<ExprCtx> target;
  std::vector<std::unique_ptr<ExprCtx>> arguments;
};

struct ParenExprCtx : public ExprCtx {
  // TODO: Location
  std::unique_ptr<ExprCtx> inner;
  explicit ParenExprCtx(ExprCtx *i) : inner(i) {}
  void accept(ExprVisitor &visitor) const override;
};

class ExprVisitor {
public:
  virtual void visitVarExpr(const VarExprCtx &ctx) = 0;
  virtual void visitFnDeclExpr(const FnDeclExprCtx &ctx) = 0;
  virtual void visitVoidLiteral(const VoidLiteralCtx &ctx) = 0;
  virtual void visitIdExpr(const IdExprCtx &ctx) = 0;
  virtual void visitNumberLiteral(const NumberLiteralCtx &ctx) = 0;
  virtual void visitStringLiteral(const StringLiteralCtx &ctx) = 0;
  virtual void visitBoolLiteral(const BoolLiteralCtx &ctx) = 0;
  virtual void visitBlockExpr(const BlockExprCtx &ctx) = 0;
  virtual void visitTupleExpr(const TupleExprCtx &ctx) = 0;
  virtual void visitCastExpr(const CastExprCtx &ctx) = 0;
  virtual void visitCallExpr(const CallExprCtx &ctx) = 0;
  virtual void visitParenExpr(const ParenExprCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
