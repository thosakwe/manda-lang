#include "parser.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace std;

Parser::Parser(Scanner &scanner) : scanner(scanner) {}

shared_ptr<CompilationUnitCtx> Parser::parseCompilationUnit() {
  auto ptr = make_shared<CompilationUnitCtx>();
  while (!scanner.isDone()) {
    auto decl = parseDecl();
    if (decl) {
      ptr->declarations.push_back(move(decl));
    } else {
      // TODO: Handle errors here...
      scanner.nextToken();
    }
  }
  return ptr;
}

bool Parser::next(Token::TokenType type) {
  if (!queue_.empty()) {
    auto tok = queue_.front();
    if (tok.type == type) {
      current = tok;
      queue_.pop();
      return true;
    } else {
      return false;
    }
  } else {
    auto tok = scanner.nextToken();
    if (tok.type == type) {
      current = tok;
      return true;
    } else {
      queue_.push(tok);
      return false;
    }
  }
}

std::unique_ptr<DeclCtx> Parser::parseDecl() {
  auto expr = parseExprDecl();
  if (expr)
    return expr;
  return parseTypeDecl();
}

std::unique_ptr<ExprDeclCtx> Parser::parseExprDecl() {
  auto expr = parseExpr();
  if (!expr)
    return nullptr;
  return make_unique<ExprDeclCtx>(expr);
}

std::unique_ptr<TypeDeclCtx> Parser::parseTypeDecl() { return nullptr; }

std::unique_ptr<ExprCtx> Parser::parseExpr() {
  // TODO: Infix
  return parsePrefixExpr();
}

std::unique_ptr<ExprCtx> Parser::parsePrefixExpr() {
  if (next(Token::ID)) {
    return make_unique<IdExprCtx>(current);
  } else if (next(Token::NUMBER)) {
    return make_unique<NumberLiteralCtx>(current);
  } else if (next(Token::TRUE) || next(Token::FALSE)) {
    return make_unique<BoolLiteralCtx>(current);
  } else if (next(Token::VOID)) {
    return make_unique<VoidLiteralCtx>();
  } else if (next(Token::VAR) || next(Token::FINAL)) {
    return nullptr;
    //    return parseVarExpr(current);
  } else if (next(Token::LCURLY)) {
    return parseBlockExpr(current);
  }

  // TODO: Other exprs

  else {
    return nullptr;
  }
}

std::unique_ptr<BlockExprCtx> Parser::parseBlockExpr(const Token &token) {
  auto ptr = make_unique<BlockExprCtx>(token.location);
  auto expr = parseExpr();
  while (expr) {
    ptr->body.push_back(expr);
    expr = parseExpr();
  }
  if (!next(Token::RCURLY)) {
    emitError(lastLocation, "Missing '}'.");
    return nullptr;
  }
  return ptr;
}
