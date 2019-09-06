%option noyywrap reentrant
%option prefix="manda"
%x DQ SQ

%{
// TODO: Comments
#include <scanner.hpp>
using TT = manda::analysis::Token::TokenType;
%}

WS [ \r\t]
NUM -?[0-9]+(\.[0-9]+)?
ID [A-Za-z_][A-Za-z0-9_]*

%%

\n      location.line++;
{WS}    { location.line++; location.column = 1; }
=       emit(TT::EQUALS);
"<"     emit(TT::LT);
">"     emit(TT::GT);
"<="    emit(TT::LT);
">="    emit(TT::GT);
"{"       emit(TT::LCURLY);
"}"       emit(TT::RCURLY);
"("     emit(TT::LPAREN);
")"     emit(TT::RPAREN);
:       emit(TT::COLON);
,       emit(TT::COMMA);
"["     emit(TT::LBRACKET);
"]"     emit(TT::RBRACKET);
"."     emit(TT::DOT);
".."    emit(TT::TWO_DOTS);
"..."   emit(TT::THREE_DOTS);
"+"     emit(TT::PLUS);
"/"     emit(TT::DIV);
%       emit(TT::MOD);
"^"     emit(TT::XOR);
&       emit(TT::AND);
"||"    emit(TT::BOOL_OR);
&&      emit(TT::BOOL_AND);
==      emit(TT::BOOL_EQUALS);
;       emit(TT::SEMI);
as      emit(TT::AS);
else    emit(TT::ELSE);
export  emit(TT::EXPORT);
false   emit(TT::FALSE);
for     emit(TT::FOR);
final   emit(TT::FINAL);
fn      emit(TT::FN);
from    emit(TT::FROM);
if      emit(TT::IF);
import  emit(TT::IMPORT);
match   emit(TT::MATCH);
pub     emit(TT::PUB);
sum     emit(TT::SUM);
then    emit(TT::THEN);
true    emit(TT::TRUE);
type    emit(TT::TYPE);
with    emit(TT::WITH);
var     emit(TT::VAR);
void    emit(TT::VOID);
{NUM}   emit(TT::NUMBER);
"\""    { emit(TT::DOUBLE_QUOTE); BEGIN(DQ); }
"'"     { emit(TT::SINGLE_QUOTE); BEGIN(SQ); }
{ID}    emit(TT::ID);
