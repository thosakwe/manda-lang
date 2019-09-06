/* Note: The order must be *exactly* the same as token.hpp */
%token EQUALS
%token LT GT LTE GTE
%token LCURLY RCURLY LPAREN RPAREN
%token COLON COMMA
%token LBRACKET RBRACKET
%token TWO_DOTS THREE_DOTS DOT
%token PLUS MINUS TIMES DIV MOD
%token XOR OR AND BOOL_OR BOOL_AND BOOL_EQUALS
%token SEMI
%token AS ELSE EXPORT FALSE FOR FINAL FN FROM IF IMPORT
%token MATCH PUB SUM THEN TRUE TYPE WITH VAR VOID
%token ID NUMBER DOUBLE_QUOTE SINGLE_QUOTE TEXT
%token HEX_ESCAPE UNICODE_ESCAPE

%define api.pure full
%param {manda::analysis::Parser* parser}

%union {
  CompilationUnitCtx* cunitval;
  DeclCtx* declval;
  DeclList* decllistval;
  ExprCtx* exprval;
  ExprList* elistval;
  IdExprCtx* idval;
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
  using DeclList = AstList<DeclCtx>;
  using ExprList = AstList<ExprCtx>;
  #define tok parser->lastToken
  #define l tok.location
  #define txt tok.text
}

%{
  #include <parser.hpp>
  using namespace std;
%}

%start compilation_unit
%type <cunitval> compilation_unit
%type <declval> decl
%type <decllistval> decl_list
%type <exprval> expr
%type <elistval> expr_list
%type <elistval> arg_list
%type <idval> id

%%

compilation_unit:
  decl_list
    {
      $$ = new CompilationUnitCtx;
      toVector($1, $$->declarations);
      parser->result = shared_ptr<CompilationUnitCtx>($$);
    }
;

decl:
  expr { $$ = new ExprDeclCtx($1); }
;

decl_list:
  %empty { $$ = nullptr; }
  | decl { $$ = new DeclList($1); }
  | decl_list decl
    {
      auto *v = new DeclList($2);
      if ($1 == nullptr) {
        $$ = v;
      } else {
        $$ = $$->add(v);
      }
    }

id: ID { $$ = new IdExprCtx{l, txt}; };

expr:
  ID { $$ = new IdExprCtx{l, txt}; }
  | NUMBER { $$ = new NumberLiteralCtx{l, stod(txt)}; }
  | TRUE { $$ = new BoolLiteralCtx(true); }
  | VOID { $$ = new VoidLiteralCtx; }
  | FALSE { $$ = new BoolLiteralCtx(false); }
  | LPAREN expr RPAREN { $$ = $2; }
  | LCURLY expr_list RCURLY
    {
      auto *ctx = new BlockExprCtx;
      toVector($2, ctx->body);
      $$ = ctx;
    }
  | LPAREN arg_list RPAREN
    {
      auto *ctx = new TupleExprCtx;
      toVector($2, ctx->items);
      $$ = ctx;
    }
  | expr LPAREN arg_list RPAREN
    {
      auto *ctx = new CallExprCtx($1);
      toVector($3, ctx->arguments);
      $$ = ctx;
    }
  | FINAL id EQUALS expr
    {
      $$ = new VarExprCtx(true, $2->name, $4);
      delete $2;
    }
  | VAR id EQUALS expr
    {
      $$ = new VarExprCtx(false, $2->name, $4);
      delete $2;
    }
;

expr_list:
  %empty { $$ = nullptr; }
  | expr { $$ = new ExprList($1); }
  | expr_list expr
    {
      auto *v = new ExprList($2);
      if ($1 == nullptr) {
        $$ = v;
      } else {
        $$ = $$->add(v);
      }
    }


arg_list:
  %empty { $$ = nullptr; }
  | expr { $$ = new ExprList($1); }
  | expr_list COMMA expr
    {
      auto *v = new ExprList($3);
      if ($1 == nullptr) {
        $$ = v;
      } else {
        $$ = $$->add(v);
      }
    }
