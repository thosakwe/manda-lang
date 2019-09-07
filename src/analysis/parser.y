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
%token HEX_ESCAPE UNICODE_ESCAPE QUOTE_ESCAPE

%define api.pure full
%param {manda::analysis::Parser* parser}

%union {
  CompilationUnitCtx* cunitval;
  DeclCtx* declval;
  DeclList* decllistval;
  ExprCtx* exprval;
  ExprList* elistval;
  IdExprCtx* idval;
  TupleExprCtx* tupval;
  StringPartCtx* strpartval;
  StringPartList* strpartlistval;
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
  using StringPartList = AstList<StringPartCtx>;
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
%type <tupval> tuple_expr_list
%type <elistval> arg_list
%type <idval> id
%type <strpartval> str_part
%type <strpartlistval> str_part_list

%left COMMA
%right EQUALS LPAREN

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
  | SINGLE_QUOTE str_part SINGLE_QUOTE
    {
      auto *ctx = new StringLiteralCtx(true);
      ctx->location = $2->location;
      ctx->value = $2->convert(true);
      delete $2;
      $$ = ctx;
    }
  | DOUBLE_QUOTE str_part_list DOUBLE_QUOTE
    {
      auto *ctx = new StringLiteralCtx(false);
      toVector($2, ctx->parts);
      $$ = ctx;
    }
  | LCURLY expr_list RCURLY
    {
      auto *ctx = new BlockExprCtx;
      toVector($2, ctx->body);
      $$ = ctx;
    }
  | tuple_expr_list { $$ = $1; }
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
  | LPAREN expr RPAREN { $$ = new ParenExprCtx($2); }
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

tuple_expr_list:
  expr COMMA expr
    {
      auto *tup = dynamic_cast<TupleExprCtx*>($1);
      if (tup != nullptr) {
        tup->items.emplace_back($3);
        $$ = tup;
      } else {
        $$ = new TupleExprCtx;
        $$->location = $1->location;
        $$->items.emplace_back($1);
        $$->items.emplace_back($3);
      }
    }
  | tuple_expr_list expr
    {
      $$ = $1;
      $1->items.emplace_back($2);
    }

arg_list:
  %empty { $$ = nullptr; }
  | expr { $$ = new ExprList($1); }
  | arg_list COMMA expr
    {
      auto *v = new ExprList($3);
      if ($1 == nullptr) {
        $$ = v;
      } else {
        $$ = $$->add(v);
      }
    }

str_part_list:
  %empty { $$ = nullptr; }
  | str_part { $$ = new StringPartList($1); }
  | str_part_list str_part
    {
      auto *v = new StringPartList($2);
      if ($1 == nullptr) {
        $$ = v;
      } else {
        $$ = $$->add(v);
      }
    }
;

str_part:
  TEXT { $$ = new TextStringPartCtx(l, txt); }
  | HEX_ESCAPE { $$ = new HexEscapeStringPartCtx(l, txt); }
  | QUOTE_ESCAPE { $$ = new QuoteEscapeStringPartCtx(l); }
;