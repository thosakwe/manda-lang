#ifndef MANDA_EXPR_CTX_HPP
#define MANDA_EXPR_CTX_HPP
#include "base_ctx.hpp"
#include "location.hpp"
#include "token.hpp"
#include "type.hpp"
#include "type_ctx.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace manda::analysis {
class ExprVisitor;

class ExprCtx : public BaseCtx {
public:
  //  std::shared_ptr<manda::analysis::Object> constantValue;
  std::shared_ptr<manda::analysis::Type> runtimeType;
  ExprCtx() = default;
  explicit ExprCtx(const Location &location) : BaseCtx(location) {}
  ExprCtx(const ExprCtx &) = default;
  ExprCtx(ExprCtx &&) = default;
  ExprCtx &operator=(const ExprCtx &) = default;
  ExprCtx &operator=(ExprCtx &&) = default;
  ~ExprCtx() override = default;
  virtual void accept(ExprVisitor &visitor) = 0;
};

class TopLevelExprCtx : public ExprCtx {
public:
  bool isPublic = false;
  std::string name;
  TopLevelExprCtx() = default;
  explicit TopLevelExprCtx(const Location &location) : ExprCtx(location) {}
  TopLevelExprCtx(const TopLevelExprCtx &) = default;
  TopLevelExprCtx(TopLevelExprCtx &&) = default;
  TopLevelExprCtx &operator=(const TopLevelExprCtx &) = default;
  TopLevelExprCtx &operator=(TopLevelExprCtx &&) = default;
};

struct VarExprCtx : public TopLevelExprCtx {
  // TODO: Set location
  bool isFinal = false;
  std::string name;
  std::shared_ptr<ExprCtx> value;
  VarExprCtx(const Location &location, bool isFinal)
      : isFinal(isFinal), TopLevelExprCtx(location) {}
  void accept(ExprVisitor &visitor) override;
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
  explicit FnDeclExprCtx(const Location &location)
      : TopLevelExprCtx(location) {}
  void accept(ExprVisitor &visitor) override;
};

struct IfClauseCtx {
  Location location;
  std::shared_ptr<ExprCtx> condition, body;
};

struct IfExprCtx : public ExprCtx {
  std::shared_ptr<IfClauseCtx> ifClause;
  std::vector<std::shared_ptr<IfClauseCtx>> elseIfClauses;
  std::shared_ptr<ExprCtx> elseClause;
  explicit IfExprCtx(std::shared_ptr<IfClauseCtx> ifClause)
      : ExprCtx(ifClause->location), ifClause(move(ifClause)) {}
  void accept(ExprVisitor &visitor) override;
};

struct VoidLiteralCtx : public ExprCtx {
  // TODO: Set location
  void accept(ExprVisitor &visitor) override;
};

struct IdExprCtx : public ExprCtx {
  std::string name;
  // TODO: Set location
  IdExprCtx(const Location &location, std::string name);
  explicit IdExprCtx(const Token &token)
      : name(token.text), ExprCtx(token.location) {}
  void accept(ExprVisitor &visitor) override;
};

class NumberLiteralCtx : public ExprCtx {
public:
  // TODO: Set location
  double value = 0.0;
  NumberLiteralCtx(const Location &location, double value);
  explicit NumberLiteralCtx(const Token &token)
      : value(std::stod(token.text)), ExprCtx(token.location) {}
  void accept(ExprVisitor &visitor) override;
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
};

class TextStringPartCtx : public StringPartCtx {
public:
  std::string text;
  [[nodiscard]] std::string convert(bool singleQuote) const override;
  TextStringPartCtx() = default;
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
  HexEscapeStringPartCtx(const Location &location, std::string text)
      : text(std::move(text)), StringPartCtx(location) {}
  explicit HexEscapeStringPartCtx(const Token &token)
      : text(token.text), StringPartCtx(token.location) {}
};

class QuoteEscapeStringPartCtx : public StringPartCtx {
public:
  [[nodiscard]] std::string convert(bool singleQuote) const override;
  QuoteEscapeStringPartCtx() = default;
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
  void accept(ExprVisitor &visitor) override;
  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] bool isChar() const;

public:
  bool singleQuote = false;
  std::string value;
  std::vector<std::shared_ptr<StringPartCtx>> parts;
};

struct BoolLiteralCtx : public ExprCtx {
  // TODO: Set location
  bool value = false;
  BoolLiteralCtx() = default;
  explicit BoolLiteralCtx(const Token &token)
      : value(token.text == "true"), ExprCtx(token.location) {}
  void accept(ExprVisitor &visitor) override;
};

struct BlockExprCtx : public ExprCtx {
  explicit BlockExprCtx(const Location &location) : ExprCtx(location){};
  void accept(ExprVisitor &visitor) override;
  std::vector<std::shared_ptr<ExprCtx>> body;
};

struct TupleExprCtx : public ExprCtx {
  explicit TupleExprCtx(const Location &location) : ExprCtx(location){};
  void accept(ExprVisitor &visitor) override;
  std::vector<std::shared_ptr<ExprCtx>> items;
};

struct ListExprCtx : public ExprCtx {
  explicit ListExprCtx(const Location &location) : ExprCtx(location) {}
  void accept(ExprVisitor &visitor) override;
  std::vector<std::shared_ptr<ExprCtx>> items;
};

struct CastExprCtx : public ExprCtx {
  CastExprCtx() = default;
  void accept(ExprVisitor &visitor) override;
  std::shared_ptr<ExprCtx> value;
  std::shared_ptr<TypeCtx> type;
};

struct CallExprCtx : public ExprCtx {
  CallExprCtx() = default;
  explicit CallExprCtx(std::shared_ptr<ExprCtx> &target)
      : target(move(target)) {
    location = this->target->location;
  }
  void accept(ExprVisitor &visitor) override;
  std::shared_ptr<ExprCtx> target;
  std::vector<std::shared_ptr<ExprCtx>> arguments;
};

struct ParenExprCtx : public ExprCtx {
  std::shared_ptr<ExprCtx> inner;
  explicit ParenExprCtx(const Location &location) : ExprCtx(location) {}
  void accept(ExprVisitor &visitor) override;
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
