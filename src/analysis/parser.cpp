#include "parser.hpp"
#include <iostream>
#include <sstream>

using namespace manda::analysis;
using namespace std;

Parser::Parser(Scanner &scanner) : scanner(scanner) {
  scanner.pipe(*this);
  //  auto tok = scanner.nextToken();
  //  while (!tok.isEOF()) {
  //    queue_.push(tok);
  //    tok = scanner.nextToken();
  //  }
}

shared_ptr<CompilationUnitCtx> Parser::parseCompilationUnit() {
  auto ptr = make_shared<CompilationUnitCtx>();
  while (true) {
    auto decl = parseDecl();
    if (decl) {
      ptr->declarations.push_back(move(decl));
    } else {
      Token tok;
      if (!queue_.empty()) {
        tok = queue_.front();
        queue_.pop();
      } else {
        tok = scanner.nextToken();
      }
      //      if (!tok.text.empty()) {
      if (tok.isEOF()) {
        break;
      } else {
        ostringstream oss;
        oss << "Unexpected text '" << tok.text << "'.";
        emitError(tok.location, oss.str());
      }
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
    return parseVarExpr(current);
  } else if (next(Token::FN)) {
    return parseFnDeclExpr(current);
  } else if (next(Token::LCURLY)) {
    return parseBlockExpr(current);
  }

  // Keep this last...
  else if (next(Token::LPAREN)) {
    return parseParenExpr(current);
  }

  // TODO: Other exprs

  else {
    return nullptr;
  }
}

std::unique_ptr<BlockExprCtx> Parser::parseBlockExpr(const Token &token) {
  auto ptr = make_unique<BlockExprCtx>(token.location);
  auto lastLocation = token.location;
  auto expr = parseExpr();
  while (expr) {
    lastLocation = expr->location;
    ptr->body.push_back(move(expr));
    expr = parseExpr();
  }
  if (!next(Token::RCURLY)) {
    emitError(lastLocation, "Missing '}'.");
    return nullptr;
  }
  return ptr;
}

std::unique_ptr<ParenExprCtx> Parser::parseParenExpr(const Token &token) {
  auto ptr = make_unique<ParenExprCtx>(token.location);
  auto lastLocation = token.location;
  auto expr = parseExpr();
  if (!expr) {
    emitError(lastLocation, "Missing expression after '('.");
    return nullptr;
  } else {
    lastLocation = expr->location;
    ptr->inner = move(expr);
  }
  if (!next(Token::RPAREN)) {
    emitError(lastLocation, "Missing ')'.");
    return nullptr;
  }
  return ptr;
}

std::unique_ptr<VarExprCtx> Parser::parseVarExpr(const Token &token) {
  auto ptr =
      make_unique<VarExprCtx>(token.location, token.type == Token::FINAL);
  auto lastLocation = token.location;
  auto name = parseIdentifier();
  if (!name) {
    emitError(lastLocation, "Missing identifier after 'var' or 'final'.");
    return nullptr;
  }
  lastLocation = name->location;
  if (!next(Token::EQUALS)) {
    emitError(lastLocation, "Missing '=' after identifier.");
    return nullptr;
  }
  lastLocation = current.location;
  auto expr = parseExpr();
  if (!expr) {
    emitError(lastLocation, "Missing expression after '='.");
    return nullptr;
  }
  ptr->name = name->name;
  ptr->value = move(expr);
  return ptr;
}

std::unique_ptr<FnDeclExprCtx> Parser::parseFnDeclExpr(const Token &token) {
  auto ptr = make_unique<FnDeclExprCtx>(token.location);
  auto lastLocation = ptr->location;
  // TODO: Make name optional
  if (!next(Token::ID)) {
    emitError(lastLocation, "Missing name for function.");
    return nullptr;
  }
  lastLocation = current.location;
  ptr->name = current.text;
  if (next(Token::LPAREN)) {
    lastLocation = current.location;
    auto param = parseParam();
    while (param) {
      lastLocation = param->location;
      ptr->params.push_back(move(param));
      if (!next(Token::COMMA)) {
        break;
      } else {
        param = parseParam();
      }
    }
    if (!next(Token::RPAREN)) {
      emitError(lastLocation, "Missing ')' after parameter list.");
      return nullptr;
    }
  }
  // TODO: Parse return type
  if (next(Token::ARROW)) {
    lastLocation = current.location;
  }
  auto body = parseExpr();
  if (!body) {
    ostringstream oss;
    oss << "Missing body in function '";
    oss << ptr->name;
    oss << "'.";
    emitError(lastLocation, oss.str());
    return nullptr;
  }
  ptr->body = move(body);
  return ptr;
}

std::unique_ptr<ParamCtx> Parser::parseParam() {
  auto ptr = make_unique<ParamCtx>();
  auto name = parseIdentifier();
  if (!name) {
    return nullptr;
  }

  ptr->location = name->location;
  ptr->name = name->name;

  if (next(Token::COLON)) {
    // TODO: Parse type associated with a param
  }

  // TODO: Parse default values
  return ptr;
}

std::unique_ptr<TypeCtx> Parser::parseType() {
  if (next(Token::ID)) {
    return make_unique<TypeRefCtx>(current);
  } else {
    return nullptr;
  }
}

std::unique_ptr<IdExprCtx> Parser::parseIdentifier() {
  if (!next(Token::ID)) {
    return nullptr;
  } else {
    return make_unique<IdExprCtx>(current);
  }
}
