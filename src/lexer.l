%option bison-bridge
%option prefix="wLang"

ID [A-Za-z_][A-Za-z0-9_]*

%{
  #include <w_parser.hpp>
%}

%%
= { return EQUALS; }

type { return TYPE; }

{ID} { yylval.strval = strdup(yytext); return ID; }
