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
  std::unique_ptr<DeclCtx> parseDecl();
  std::unique_ptr<TypeDeclCtx> parseTypeDecl();

  std::unique_ptr<ExprDeclCtx> parseExprDecl();
  std::unique_ptr<ExprCtx> parseExpr();
  std::unique_ptr<ExprCtx> parsePrimaryExpr();
  std::unique_ptr<ExprCtx> parseClimbingExpr(std::unique_ptr<ExprCtx> &lhs,
                                             int minPrecedence);
  std::unique_ptr<VarExprCtx> parseVarExpr(const Token &token);
  std::unique_ptr<FnDeclExprCtx> parseFnDeclExpr(const Token &token);
  std::unique_ptr<ParamCtx> parseParam();
  std::unique_ptr<StringLiteralCtx> parseStringLiteral(const Token &token);
  std::unique_ptr<BlockExprCtx> parseBlockExpr(const Token &token);
  std::unique_ptr<ExprCtx> parseParenExpr(const Token &token);

  std::unique_ptr<TypeCtx> parseType();

  std::unique_ptr<IdExprCtx> parseIdentifier();
  Token peek();

private:
  Token current;
  Scanner &scanner;
  std::queue<Token> queue_;
};
} // namespace manda::analysis

#endif
