#ifndef MANDA_PARSER_HPP
#define MANDA_PARSER_HPP
#include "ast.hpp"
#include "manda_exception.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include <queue>
#include <vector>

// TODO: Pass an Interpreter instance, and use reportError
namespace manda::analysis {
class Parser : public MandaErrorEmitter {
public:
  explicit Parser(Scanner &scanner);
  bool next(Token::TokenType type);
  std::shared_ptr<CompilationUnitCtx> parseCompilationUnit();
  std::shared_ptr<DeclCtx> parseDecl();
  std::shared_ptr<TypeDeclCtx> parseTypeDecl();

  std::shared_ptr<ExprDeclCtx> parseExprDecl();
  std::shared_ptr<ExprCtx> parseExpr();
  std::shared_ptr<ExprCtx> parsePrimaryExpr();
  std::shared_ptr<ExprCtx> parseClimbingExpr(std::shared_ptr<ExprCtx> &lhs,
                                             int minPrecedence);
  std::shared_ptr<VarExprCtx> parseVarExpr(const Token &token);
  std::shared_ptr<FnDeclExprCtx> parseFnDeclExpr(const Token &token);
  std::shared_ptr<ParamCtx> parseParam();
  std::shared_ptr<StringLiteralCtx> parseStringLiteral(const Token &token, bool isSingleQuote);
  std::shared_ptr<BlockExprCtx> parseBlockExpr(const Token &token);
  std::shared_ptr<ListExprCtx> parseListExpr(const Token &token);
  std::shared_ptr<ExprCtx> parseParenExpr(const Token &token);
  std::shared_ptr<IfExprCtx> parseIfExpr(const Token &token);
  std::shared_ptr<IfClauseCtx> parseIfClause(const Token &token);

  std::shared_ptr<TypeCtx> parseType();

  std::shared_ptr<IdExprCtx> parseIdentifier();
  std::shared_ptr<StringPartCtx> parseStringPart(bool isSingleQuote);
  Token peek();

private:
  Token current;
  Scanner &scanner;
  std::queue<Token> queue_;
};
} // namespace manda::analysis

#endif
