#include "token.hpp"

using namespace manda::analysis;
using namespace std;

ostream &manda::analysis::operator<<(ostream &out, const Token &a) {
  out << a.location.begin << ": \"" << a.text << "\" => " << a.type;
  return out;
}

bool Token::isEOF() const { return type == END_OF_FILE; }

// TODO: Binary operators

bool Token::isBinaryOp() const { return false; }

int Token::getPrecedence() const { return -1; }

bool Token::isRightAssociative() const { return false; }

ostream &manda::analysis::operator<<(ostream &out,
                                     const Token::TokenType &type) {
  switch (type) {
  case Token::END_OF_FILE:
    out << "END_OF_FILE";
    break;
  case Token::EQUALS:
    out << "EQUALS";
    break;
  case Token::LT:
    out << "LT";
    break;
  case Token::GT:
    out << "GT";
    break;
  case Token::LTE:
    out << "LTE";
    break;
  case Token::GTE:
    out << "GTE";
    break;
  case Token::LCURLY:
    out << "LCURLY";
    break;
  case Token::RCURLY:
    out << "RCURLY";
    break;
  case Token::LPAREN:
    out << "LPAREN";
    break;
  case Token::RPAREN:
    out << "RPAREN";
    break;
  case Token::COLON:
    out << "COLON";
    break;
  case Token::COMMA:
    out << "COMMA";
    break;
  case Token::LBRACKET:
    out << "LBRACKET";
    break;
  case Token::RBRACKET:
    out << "RBRACKET";
    break;
  case Token::TWO_DOTS:
    out << "TWO_DOTS";
    break;
  case Token::THREE_DOTS:
    out << "THREE_DOTS";
    break;
  case Token::DOT:
    out << "DOT";
    break;
  case Token::PLUS:
    out << "PLUS";
    break;
  case Token::MINUS:
    out << "MINUS";
    break;
  case Token::TIMES:
    out << "TIMES";
    break;
  case Token::DIV:
    out << "DIV";
    break;
  case Token::MOD:
    out << "MOD";
    break;
  case Token::XOR:
    out << "XOR";
    break;
  case Token::OR:
    out << "OR";
    break;
  case Token::AND:
    out << "AND";
    break;
  case Token::BOOL_OR:
    out << "BOOL_OR";
    break;
  case Token::BOOL_AND:
    out << "BOOL_AND";
    break;
  case Token::BOOL_EQUALS:
    out << "BOOL_EQUALS";
    break;
  case Token::SEMI:
    out << "SEMI";
    break;
  case Token::AS:
    out << "AS";
    break;
  case Token::ELSE:
    out << "ELSE";
    break;
  case Token::EXPORT:
    out << "EXPORT";
    break;
  case Token::FALSE:
    out << "FALSE";
    break;
  case Token::FOR:
    out << "FOR";
    break;
  case Token::FINAL:
    out << "FINAL";
    break;
  case Token::FN:
    out << "FN";
    break;
  case Token::FROM:
    out << "FROM";
    break;
  case Token::IF:
    out << "IF";
    break;
  case Token::IMPORT:
    out << "IMPORT";
    break;
  case Token::MATCH:
    out << "MATCH";
    break;
  case Token::PUB:
    out << "PUB";
    break;
  case Token::SUM:
    out << "SUM";
    break;
  case Token::THEN:
    out << "THEN";
    break;
  case Token::TRUE:
    out << "TRUE";
    break;
  case Token::TYPE:
    out << "TYPE";
    break;
  case Token::WITH:
    out << "WITH";
    break;
  case Token::VAR:
    out << "VAR";
    break;
  case Token::VOID:
    out << "VOID";
    break;
  case Token::ID:
    out << "ID";
    break;
  case Token::NUMBER:
    out << "NUMBER";
    break;
  case Token::DOUBLE_QUOTE:
    out << "DOUBLE_QUOTE";
    break;
  case Token::SINGLE_QUOTE:
    out << "SINGLE_QUOTE";
    break;
  case Token::TEXT:
    out << "TEXT";
    break;
  case Token::HEX_ESCAPE:
    out << "HEX_ESCAPE";
    break;
  case Token::UNICODE_ESCAPE:
    out << "UNICODE_ESCAPE";
    break;
  case Token::QUOTE_ESCAPE:
    out << "QUOTE_ESCAPE";
    break;
  case Token::ARROW:
    out << "ARROW";
    break;
  }
  return out;
}
