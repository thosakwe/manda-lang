%option noyywrap reentrant stack
%option prefix="manda"
%x DQ SQ

%{
// TODO: Comments
#include <scanner.hpp>
using TT = manda::analysis::Token::TokenType;
%}

WS [ \r\t]
NUM -?[0-9]+(\.[0-9]+)?
HEX_ESC \\x[A-Fa-f0-9][A-Fa-f0-9]
TEXT [^\n]
ID [A-Za-z_][A-Za-z0-9_]*

%%

\n+     lines(yyleng);
{WS}+   columns(yyleng);
=       return TT::EQUALS;
"->"    return TT::ARROW;
"<"     return TT::LT;
">"     return TT::GT;
"<="    return TT::LT;
">="    return TT::GT;
"{"     return TT::LCURLY;
"}"     return TT::RCURLY;
"("     return TT::LPAREN;
")"     return TT::RPAREN;
:       return TT::COLON;
,       return TT::COMMA;
"["     return TT::LBRACKET;
"]"     return TT::RBRACKET;
"."     return TT::DOT;
".."    return TT::TWO_DOTS;
"..."   return TT::THREE_DOTS;
"+"     return TT::PLUS;
"/"     return TT::DIV;
%       return TT::MOD;
"^"     return TT::XOR;
&       return TT::AND;
"||"    return TT::BOOL_OR;
&&      return TT::BOOL_AND;
==      return TT::BOOL_EQUALS;
;       return TT::SEMI;
as      return TT::AS;
else    return TT::ELSE;
export  return TT::EXPORT;
false   return TT::FALSE;
for     return TT::FOR;
final   return TT::FINAL;
fn      return TT::FN;
from    return TT::FROM;
if      return TT::IF;
import  return TT::IMPORT;
match   return TT::MATCH;
pub     return TT::PUB;
sum     return TT::SUM;
then    return TT::THEN;
true    return TT::TRUE;
type    return TT::TYPE;
with    return TT::WITH;
var     return TT::VAR;
void    return TT::VOID;
{NUM}   return TT::NUMBER;
"\""    { return TT::DOUBLE_QUOTE; yy_push_state(DQ, yyscanner); }
"'"     { return TT::SINGLE_QUOTE; yy_push_state(SQ, yyscanner); }
{ID}    return TT::ID;

<DQ>"\\\""        return TT::QUOTE_ESCAPE;
<SQ>"\\'"         return TT::QUOTE_ESCAPE;
<DQ>"\""          { return TT::DOUBLE_QUOTE; yy_pop_state(yyscanner); }
<SQ>"'"           { return TT::SINGLE_QUOTE; yy_pop_state(yyscanner); }
<DQ,SQ>{HEX_ESC}  return TT::HEX_ESCAPE;
<DQ,SQ>{TEXT}     return TT::TEXT;
