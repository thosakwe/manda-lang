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
        oss << "Unexpected token '" << tok.text << "' (";
        oss << tok.type << ").";
        emitError(tok.location, oss.str());
      }
    }
  }
  return ptr;
}

Token Parser::peek() {
  if (!queue_.empty()) {
    return queue_.front();
  } else if (!scanner.isDone()) {
    auto tok = scanner.nextToken();
    queue_.push(tok);
    return tok;
  } else {
    auto tok = Token();
    tok.type = Token::END_OF_FILE;
    return tok;
  }
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

unique_ptr<DeclCtx> Parser::parseDecl() {
  auto expr = parseExprDecl();
  if (expr)
    return expr;
  return parseTypeDecl();
}

unique_ptr<ExprDeclCtx> Parser::parseExprDecl() {
  auto expr = parseExpr();
  if (!expr)
    return nullptr;
  return make_unique<ExprDeclCtx>(expr);
}

unique_ptr<TypeDeclCtx> Parser::parseTypeDecl() { return nullptr; }

unique_ptr<ExprCtx> Parser::parseExpr() {
  auto lhs = parsePrimaryExpr();
  if (!lhs) {
    return nullptr;
  } else if (next(Token::LPAREN)) {
    auto lastLocation = current.location;
    auto ptr = make_unique<CallExprCtx>(lhs);
    auto arg = parseExpr();
    while (arg) {
      lastLocation = arg->location;
      ptr->arguments.push_back(move(arg));
      if (next(Token::COMMA)) {
        arg = parseExpr();
      } else {
        break;
      }
    }
    if (!next(Token::RPAREN)) {
      emitError(lastLocation, "Missing ')' after argument list.");
      return nullptr;
    }
    return ptr;
  } else {
    return parseClimbingExpr(lhs, 0);
  }
}

unique_ptr<ExprCtx> Parser::parsePrimaryExpr() {
  if (next(Token::ID)) {
    return make_unique<IdExprCtx>(current);
  } else if (next(Token::NUMBER)) {
    return make_unique<NumberLiteralCtx>(current);
  } else if (next(Token::TRUE) || next(Token::FALSE)) {
    return make_unique<BoolLiteralCtx>(current);
  } else if (next(Token::SINGLE_QUOTE)) {
    return parseStringLiteral(current, true);
  } else if (next(Token::DOUBLE_QUOTE)) {
    return parseStringLiteral(current, false);
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

unique_ptr<ExprCtx> Parser::parseClimbingExpr(unique_ptr<ExprCtx> &lhs,
                                              int minPrecedence) {
  if (!lhs)
    return nullptr;
  auto lookahead = peek();
  if (lookahead.isBinaryOp() && lookahead.getPrecedence() >= minPrecedence) {
    auto op = lookahead;
    queue_.pop();
    auto rhs = parsePrimaryExpr();
    if (!rhs) {
      emitError(op.location, "Missing expression after operator.");
      return nullptr;
    }
    lookahead = peek();
    while (lookahead.isBinaryOp() &&
           (lookahead.getPrecedence() > op.getPrecedence() ||
            lookahead.isRightAssociative())) {
      rhs = parseClimbingExpr(rhs, lookahead.getPrecedence());
      if (!rhs) {
        // TODO: Report missing expr
        return nullptr;
      }
      lookahead = peek();
    }
    // TODO: Apply the binary operation...
    //    switch (op.type) {
    //    default: {
    //      return nullptr;
    //    }
    //    }
  }
  return move(lhs);
}

unique_ptr<BlockExprCtx> Parser::parseBlockExpr(const Token &token) {
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

unique_ptr<ExprCtx> Parser::parseParenExpr(const Token &token) {
  auto ptr = make_unique<ParenExprCtx>(token.location);
  auto lastLocation = token.location;
  auto expr = parseExpr();
  if (!expr) {
    emitError(lastLocation, "Missing expression after '('.");
    return nullptr;
  } else if (!next(Token::COMMA)) {
    lastLocation = expr->location;
    ptr->inner = move(expr);
    if (!next(Token::RPAREN)) {
      emitError(lastLocation, "Missing ')'.");
      return nullptr;
    }
    return ptr;
  } else {
    auto tuple = make_unique<TupleExprCtx>(token.location);
    tuple->items.push_back(move(expr));
    expr = parseExpr();
    while (expr) {
      lastLocation = expr->location;
      tuple->items.push_back(move(expr));
      if (next(Token::COMMA)) {
        lastLocation = current.location;
        expr = parseExpr();
      } else {
        break;
      }
    }
    if (!next(Token::RPAREN)) {
      emitError(lastLocation, "Missing ')'.");
      return nullptr;
    }
    return tuple;
  }
}

unique_ptr<VarExprCtx> Parser::parseVarExpr(const Token &token) {
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

unique_ptr<FnDeclExprCtx> Parser::parseFnDeclExpr(const Token &token) {
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

unique_ptr<ParamCtx> Parser::parseParam() {
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

unique_ptr<StringLiteralCtx> Parser::parseStringLiteral(const Token &token,
                                                        bool isSingleQuote) {
  auto ptr = make_unique<StringLiteralCtx>(token);
  auto part = parseStringPart(isSingleQuote);
  auto lastLocation = ptr->location;
  auto closingType = isSingleQuote ? Token::SINGLE_QUOTE : Token::DOUBLE_QUOTE;
  ptr->singleQuote = isSingleQuote;
  if (isSingleQuote) {
    if (!part) {
      emitError(lastLocation, "Character literals cannot be empty.");
      return nullptr;
    }
    ptr->value = part->convert(true);
  } else {
    while (part) {
      lastLocation = part->location;
      ptr->parts.push_back(move(part));
      part = parseStringPart(false);
    }
  }
  if (!next(closingType)) {
    string quote =
        isSingleQuote ? "single quote (\"'\")" : "double quote ('\"')";
    ostringstream oss;
    oss << "Missing " << quote << ".";
    emitError(lastLocation, oss.str());
    return nullptr;
  }
  return ptr;
}

unique_ptr<StringPartCtx> Parser::parseStringPart(bool isSingleQuote) {
  if (next(Token::TEXT)) {
    return make_unique<TextStringPartCtx>(current);
  } else if (next(Token::HEX_ESCAPE)) {
    return make_unique<HexEscapeStringPartCtx>(current);
  } else if (isSingleQuote && next(Token::SINGLE_QUOTE_ESCAPE)) {
    return make_unique<QuoteEscapeStringPartCtx>(current.location);
  } else if (!isSingleQuote && next(Token::DOUBLE_QUOTE_ESCAPE)) {
    return make_unique<QuoteEscapeStringPartCtx>(current.location);
  }
  return nullptr;
}

unique_ptr<TypeCtx> Parser::parseType() {
  if (next(Token::ID)) {
    return make_unique<TypeRefCtx>(current);
  } else {
    return nullptr;
  }
}

unique_ptr<IdExprCtx> Parser::parseIdentifier() {
  if (!next(Token::ID)) {
    return nullptr;
  } else {
    return make_unique<IdExprCtx>(current);
  }
}
