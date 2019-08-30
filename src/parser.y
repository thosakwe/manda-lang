%{
  #include <w_lexer.hpp>
%}

%define api.prefix {wLang}
%token ID "identifier"
%token EQUALS "="
%token TYPE "type"

%%

type_decl:
  TYPE ID EQUALS type
;

type:
  ID
;
