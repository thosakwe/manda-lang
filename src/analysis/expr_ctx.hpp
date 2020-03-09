#ifndef MANDA_EXPR_CTX_HPP
#define MANDA_EXPR_CTX_HPP
#include "../runtime/type.hpp"
#include "location.hpp"
#include "token.hpp"
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
  std::shared_ptr<manda::runtime::Type> runtimeType;
  ExprCtx() = default;
  explicit ExprCtx(Location location) : location(std::move(location)) {}
  ExprCtx(const ExprCtx &) = default;
  ExprCtx(ExprCtx &&) = default;
  ExprCtx &operator=(const ExprCtx &) = default;
  ExprCtx &operator=(ExprCtx &&) = default;
  virtual ~ExprCtx() = default;
  virtual void accept(ExprVisitor &visitor) const = 0;
  virtual ExprCtx *clone() const = 0;
  virtual std::unique_ptr<ExprCtx> cloneToUniquePointer() const;
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
  bool isPublic = false;
  std::string name;
  TopLevelExprCtx() = default;
  TopLevelExprCtx(const Location &location) : ExprCtx(location) {}
  TopLevelExprCtx(const TopLevelExprCtx &) = default;
  TopLevelExprCtx(TopLevelExprCtx &&) = default;
  TopLevelExprCtx &operator=(const TopLevelExprCtx &) = default;
  TopLevelExprCtx &operator=(TopLevelExprCtx &&) = default;
};

struct VarExprCtx : public TopLevelExprCtx {
  // TODO: Set location
  bool isFinal = false;
  std::string name;
  std::unique_ptr<ExprCtx> value;
  VarExprCtx(const Location &location, bool isFinal)
      : isFinal(isFinal), TopLevelExprCtx(location) {}
  void accept(ExprVisitor &visitor) const override;
  VarExprCtx *clone() const override;
};

struct ParamCtx {
  Location location;
  std::string name;
  std::unique_ptr<TypeCtx> type;
  std::unique_ptr<ExprCtx> defaultValue;
  ParamCtx *clone() const;
  std::unique_ptr<ParamCtx> cloneToUniquePointer() const;
};

struct FnDeclExprCtx : public TopLevelExprCtx {
  std::vector<std::unique_ptr<ParamCtx>> params;
  std::unique_ptr<TypeCtx> returnType;
  std::unique_ptr<ExprCtx> body;
  explicit FnDeclExprCtx(const Location &location)
      : TopLevelExprCtx(location) {}
  void accept(ExprVisitor &visitor) const override;
  FnDeclExprCtx *clone() const override;
};

struct IfClauseCtx {
  Location location;
  std::unique_ptr<ExprCtx> condition, body;
  IfClauseCtx *clone() const;
  std::unique_ptr<IfClauseCtx> cloneToUniquePointer() const;
};

struct IfExprCtx : public ExprCtx {
  std::unique_ptr<IfClauseCtx> ifClause;
  std::vector<std::unique_ptr<IfClauseCtx>> elseIfClauses;
  std::unique_ptr<ExprCtx> elseClause;
  explicit IfExprCtx(std::unique_ptr<IfClauseCtx> ifClause)
      : ExprCtx(ifClause->location), ifClause(move(ifClause)) {}
  void accept(ExprVisitor &visitor) const override;
  ExprCtx *clone() const override;
};

struct VoidLiteralCtx : public ExprCtx {
  // TODO: Set location
  void accept(ExprVisitor &visitor) const override;
  VoidLiteralCtx *clone() const override;
};

struct IdExprCtx : public ExprCtx {
  std::string name;
  // TODO: Set location
  IdExprCtx(const Location &location, std::string name);
  explicit IdExprCtx(const Token &token)
      : name(token.text), ExprCtx(token.location) {}
  void accept(ExprVisitor &visitor) const override;
  IdExprCtx *clone() const override;
};

class NumberLiteralCtx : public ExprCtx {
public:
  // TODO: Set location
  double value = 0.0;
  NumberLiteralCtx(const Location &location, double value);
  NumberLiteralCtx(const Token &token)
      : value(std::stod(token.text)), ExprCtx(token.location) {}
  void accept(ExprVisitor &visitor) const override;
  NumberLiteralCtx *clone() const override;
};

class StringPartCtx {
public:
  // TODO: Set location
  Location location;
  [[nodiscard]] virtual std::string convert(bool singleQuote) const = 0;
  StringPartCtx() = default;
  explicit StringPartCtx(Location location) : location(std::move(location)) {}
  StringPartCtx(const StringPartCtx &) = default;
  StringPartCtx(StringPartCtx &&) = default;
  StringPartCtx &operator=(const StringPartCtx &) = default;
  StringPartCtx &operator=(StringPartCtx &&) = default;
  virtual ~StringPartCtx() = default;
  virtual StringPartCtx *clone() const = 0;
  std::unique_ptr<StringPartCtx> cloneToUniquePointer() const;
};

class TextStringPartCtx : public StringPartCtx {
public:
  std::string text;
  [[nodiscard]] std::string convert(bool singleQuote) const override;
  TextStringPartCtx() = default;
  TextStringPartCtx *clone() const override;
  TextStringPartCtx(const Location &location, std::string text)
      : text(std::move(text)), StringPartCtx(location) {}
  explicit TextStringPartCtx(const Token &token)
      : text(token.text), StringPartCtx(token.location) {}
};

class HexEscapeStringPartCtx : public StringPartCtx {
public:
  std::string text;
  [[nodiscard]] std::string convert(bool singleQuote) const override;
  HexEscapeStringPartCtx() = default;
  HexEscapeStringPartCtx *clone() const override;
  HexEscapeStringPartCtx(const Location &location, std::string text)
      : text(std::move(text)), StringPartCtx(location) {}
  explicit HexEscapeStringPartCtx(const Token &token)
      : text(token.text), StringPartCtx(token.location) {}
};

class QuoteEscapeStringPartCtx : public StringPartCtx {
public:
  [[nodiscard]] std::string convert(bool singleQuote) const override;
  QuoteEscapeStringPartCtx() = default;
  QuoteEscapeStringPartCtx *clone() const override;
  explicit QuoteEscapeStringPartCtx(const Location &location)
      : StringPartCtx(location) {}
};

class StringLiteralCtx : public ExprCtx {
public:
  // TODO: Location
  StringLiteralCtx() = default;
  StringLiteralCtx(const Location &location, bool sq)
      : singleQuote(sq), ExprCtx(location) {}
  explicit StringLiteralCtx(const Token &token)
      : singleQuote(token.type == Token::SINGLE_QUOTE),
        ExprCtx(token.location) {}
  StringLiteralCtx *clone() const override;
  void accept(ExprVisitor &visitor) const override;
  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] bool isChar() const;

public:
  bool singleQuote = false;
  std::string value;
  std::vector<std::unique_ptr<StringPartCtx>> parts;
};

struct BoolLiteralCtx : public ExprCtx {
  // TODO: Set location
  bool value = false;
  BoolLiteralCtx() = default;
  explicit BoolLiteralCtx(const Token &token)
      : value(token.text == "true"), ExprCtx(token.location) {}
  void accept(ExprVisitor &visitor) const override;
  BoolLiteralCtx *clone() const override;
};

struct BlockExprCtx : public ExprCtx {
  BlockExprCtx(const Location &location) : ExprCtx(location){};
  BlockExprCtx *clone() const override;
  void accept(ExprVisitor &visitor) const override;
  std::vector<std::unique_ptr<ExprCtx>> body;
};

struct TupleExprCtx : public ExprCtx {
  TupleExprCtx(const Location &location) : ExprCtx(location) {}
  TupleExprCtx *clone() const override;
  void accept(ExprVisitor &visitor) const override;
  std::vector<std::unique_ptr<ExprCtx>> items;
};

struct ListExprCtx : public ExprCtx {
  ListExprCtx(const Location &location) : ExprCtx(location) {}
  ListExprCtx *clone() const override;
  void accept(ExprVisitor &visitor) const override;
  std::vector<std::unique_ptr<ExprCtx>> items;
};

struct CastExprCtx : public ExprCtx {
  CastExprCtx() = default;
  CastExprCtx *clone() const override;
  void accept(ExprVisitor &visitor) const override;
  std::unique_ptr<ExprCtx> value;
  std::unique_ptr<TypeCtx> type;
};

struct CallExprCtx : public ExprCtx {
  CallExprCtx() = default;
  CallExprCtx *clone() const override;
  explicit CallExprCtx(std::unique_ptr<ExprCtx> &target)
      : target(move(target)) {
    location = this->target->location;
  }
  void accept(ExprVisitor &visitor) const override;
  std::unique_ptr<ExprCtx> target;
  std::vector<std::unique_ptr<ExprCtx>> arguments;
};

struct ParenExprCtx : public ExprCtx {
  std::unique_ptr<ExprCtx> inner;
  ParenExprCtx(const Location &location) : ExprCtx(location) {}
  ParenExprCtx *clone() const override;
  void accept(ExprVisitor &visitor) const override;
};

class ExprVisitor {
public:
  virtual void visitVarExpr(VarExprCtx &ctx) = 0;
  virtual void visitFnDeclExpr(FnDeclExprCtx &ctx) = 0;
  virtual void visitIfExpr(IfExprCtx &ctx) = 0;
  virtual void visitVoidLiteral(VoidLiteralCtx &ctx) = 0;
  virtual void visitIdExpr(IdExprCtx &ctx) = 0;
  virtual void visitNumberLiteral(NumberLiteralCtx &ctx) = 0;
  virtual void visitStringLiteral(StringLiteralCtx &ctx) = 0;
  virtual void visitBoolLiteral(BoolLiteralCtx &ctx) = 0;
  virtual void visitBlockExpr(BlockExprCtx &ctx) = 0;
  virtual void visitTupleExpr(TupleExprCtx &ctx) = 0;
  virtual void visitListExpr(ListExprCtx &ctx) = 0;
  virtual void visitCastExpr(CastExprCtx &ctx) = 0;
  virtual void visitCallExpr(CallExprCtx &ctx) = 0;
  virtual void visitParenExpr(ParenExprCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
