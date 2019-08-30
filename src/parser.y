%{
  #include <w_lexer.hpp>
  void wLangerror(const char* msg);
%}

%define api.prefix {wLang}
%token ID "identifier"
%token EQUALS "="
%token TYPE "type"

%code requires {
  #include <type_reference.hpp>
}

%union {
  char* strval;
  w_lang::Type* typeval;
}

%%

type_decl:
  TYPE ID EQUALS type
;

type:
  ID
;
