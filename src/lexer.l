%option prefix="w"
%option reentrant noyywrap nounput noinput batch

ID [A-Za-z_][A-Za-z0-9_]*
HEX 0x[A-Fa-f0-9]+
INT -?[0-9]+
NEWLINE \n
WHITESPACE [ \r\t]

%{
  #include <w_parser.hpp>
  #define YY_DECL int w_lang::Lexer::wlex(void& yyscanner);
  using T = w_lang::Token::TokenType;
%}

%%

{NEWLINE}     newline();
{WHITESPACE}  whitespace();
=             emit(T::EQUALS);
type          emit(T::TYPE);
{ID}          emit_identifier();
.             emit_error();
