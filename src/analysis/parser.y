%token EQUALS
%token LT GT LTE GTE
%token LCURLY RCURLY LPAREN RPAREN
%token COLON
%token LBRACKET RBRACKET
%token TWO_DOTS THREE_DOTS DOT
%token PLUS MINUS TIMES DIV MOD
%token XOR OR AND BOOL_OR BOOL_AND BOOL_EQUALS
%token SEMI
%token AS ELSE EXPORT FOR FINAL FN FROM IF IMPORT
%token MATCH PUB SUM THEN TYPE WITH VAR
%token ID NUMBER DOUBLE_QUOTE SINGLE_QUOTE TEXT
%token HEX_ESCAPE UNICODE_ESCAPE

%define api.pure full
%param {manda::analysis::Parser* parser}

%union {
  ExprCtx* exprval;
}

%code requires {
  #include <ast.hpp>
  /* #include <parser.hpp> */
  typedef union YYSTYPE YYSTYPE;
  namespace manda::analysis {
    class Parser;
    int yylex(YYSTYPE*, Parser*);
    void yyerror(Parser*, const char*);
  }
  using namespace manda::analysis;
  #define tok parser->lastToken
  #define l tok.location
  #define txt tok.text
}

%{
  #include <parser.hpp>
%}

%start expr
%type <exprval> expr

%%

expr:
  ID { $$ = new IdExprCtx{l, txt}; }
  | NUMBER { $$ = new NumberLiteralCtx{l, stod(txt)}; }
  | LPAREN expr RPAREN { $$ = $2; }
;
