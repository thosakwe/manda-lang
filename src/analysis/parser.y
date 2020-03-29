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
%token ARROW

%defines
%locations
%define api.location.type {manda::analysis::Location}
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
  TypeCtx* tval;
  ParamCtx* pval;
  ParamList* plistval;
}

%initial-action
{
  @$.filename = parser->filename;
};

%code requires {
  #include <ast.hpp>
  /* #include <parser.hpp> */
  typedef union YYSTYPE YYSTYPE;
  namespace manda::analysis {
    class Parser;
    int yylex(YYSTYPE*, Location*, Parser*);
    void yyerror(Parser*, Location*, const char*);
  }
  using namespace manda::analysis;
  using DeclList = AstList<DeclCtx>;
  using ExprList = AstList<ExprCtx>;
  using StringPartList = AstList<StringPartCtx>;
  using ParamList = AstList<ParamCtx>;
  #define tok parser->lastToken
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
%type <idval> id_opt
%type <strpartval> str_part
%type <strpartlistval> str_part_list
%type <tval> type
%type <pval> param
%type <plistval> param_list
%type <tval> return_type

%left COMMA
%right EQUALS LPAREN

%%

compilation_unit:
  decl_list
    {
      @$ = @1;
      $$ = new CompilationUnitCtx;
      toVector($1, $$->declarations);
      parser->result = shared_ptr<CompilationUnitCtx>($$);
    }
;

decl:
  expr { $$ = new ExprDeclCtx($1); @$ = @1; }
  /* TODO: Type parameters, location */
  | TYPE id EQUALS type { @$ = @2; $$ = new TypeDeclCtx($2->name, $4); delete $2; }
;

decl_list:
  %empty { $$ = nullptr; }
  | decl { $$ = new DeclList($1); @$ = @1; }
  | decl_list decl
    {
      auto *v = new DeclList($2);
      if ($1 == nullptr) {
        $$ = v;
         @$ = @2;
      } else {
        $$ = $1->add(v);
         @$ = @1;
      }
    }

id: ID { $$ = new IdExprCtx{@1, txt}; @$ = @1; };
id_opt:
  %empty { $$ = nullptr; }
  | id { $$ = $1; @$ = @1; }

type:
  ID { $$ = new TypeRefCtx{@1, txt}; @$ = @1; }

expr:
  ID { $$ = new IdExprCtx{@1, txt}; @$ = @1; }
  | NUMBER { $$ = new NumberLiteralCtx{@1, stod(txt)}; @$ = @1; }
  | TRUE { $$ = new BoolLiteralCtx(true); @$ = @1; }
  | VOID { $$ = new VoidLiteralCtx; @$ = @1; }
  | FALSE { $$ = new BoolLiteralCtx(false); @$ = @1; }
  | SINGLE_QUOTE str_part SINGLE_QUOTE
    {
      @$ = @1;
      auto *ctx = new StringLiteralCtx(true);
      ctx->location = $2->location;
      ctx->value = $2->convert(true);
      delete $2;
      $$ = ctx;
    }
  | DOUBLE_QUOTE str_part_list DOUBLE_QUOTE
    {
      @$ = @1;
      auto *ctx = new StringLiteralCtx(false);
      toVector($2, ctx->parts);
      $$ = ctx;
    }
  | LCURLY expr_list RCURLY
    {
      @$ = @1;
      auto *ctx = new BlockExprCtx;
      toVector($2, ctx->body);
      $$ = ctx;
    }
  | tuple_expr_list { $$ = $1; @$ = @1; }
  | expr LPAREN arg_list RPAREN
    {
      @$ = @1;
      auto *ctx = new CallExprCtx($1);
      toVector($3, ctx->arguments);
      $$ = ctx;
    }
  | FINAL id EQUALS expr
    {
      @$ = @2;
      $$ = new VarExprCtx(true, $2->name, $4);
      delete $2;
    }
  | VAR id EQUALS expr
    {
      @$ = @2;
      $$ = new VarExprCtx(false, $2->name, $4);
      delete $2;
    }
  | FN id_opt LPAREN param_list RPAREN return_type arrow expr
    {
      // TODO: Locations
      // TODO: Delete unused stuff
      @$ = @2;
      auto *v = new FnDeclExprCtx;
      v->body = shared_ptr<ExprCtx>($8);
      if ($2) v->name = $2->name;
      if ($6) v->returnType = shared_ptr<TypeCtx>($6);
      toVector($4, v->params);
      $$ = v;
      delete $2;
    }
  | FN id_opt return_type arrow expr
    {
      // TODO: Locations
      // TODO: Delete unused stuff
      @$ = @2;
      auto *v = new FnDeclExprCtx;
      v->body = shared_ptr<ExprCtx>($5);
      if ($2) v->name = $2->name;
      if ($3) v->returnType = shared_ptr<TypeCtx>($3);
      $$ = v;
      delete $2;
    }
  | LPAREN expr RPAREN { @$ = @1; $$ = new ParenExprCtx($2); }
;

semi: %empty | SEMI;

expr_list:
  %empty { $$ = nullptr; }
  | expr { @$ = @1; $$ = new ExprList($1); }
  | expr_list semi expr
    {
      auto *v = new ExprList($3);
      if ($1 == nullptr) {
        $$ = v;
        @$ = @2;
      } else {
        $$ = $1->add(v);
        @$ = @1;
      }
    }

tuple_expr_list:
  expr COMMA expr
    {
      auto *tup = dynamic_cast<TupleExprCtx*>($1);
      @$ = @1;
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
      @$ = @1;
      $1->items.emplace_back($2);
    }

arg_list:
  %empty { $$ = nullptr; }
  | expr { @$ = @1; $$ = new ExprList($1); }
  | arg_list COMMA expr
    {
      auto *v = new ExprList($3);
      if ($1 == nullptr) {
        $$ = v;
        @$ = @2;
      } else {
        $$ = $1->add(v);
        @$ = @1;
      }
    }

str_part_list:
  %empty { $$ = nullptr; }
  | str_part { @$ = @1; $$ = new StringPartList($1); }
  | str_part_list str_part
    {
      auto *v = new StringPartList($2);
      if ($1 == nullptr) {
        $$ = v;
        @$ = @2;
      } else {
        $$ = $1->add(v);
        @$ = @1;
      }
    }
;

str_part:
  TEXT { @$ = @1; $$ = new TextStringPartCtx(@1, txt); }
  | HEX_ESCAPE { @$ = @1; $$ = new HexEscapeStringPartCtx(@1, txt); }
  | QUOTE_ESCAPE { @$ = @1; $$ = new QuoteEscapeStringPartCtx(@1); }
;

arrow:
  %empty
  | ARROW
;

return_type:
  %empty { $$ = nullptr; }
  | COLON type { $$ = $2; @$ = @2; }

param:
  id
    {
      @$ = @1;
      $$ = new ParamCtx{$1->location, $1->name, nullptr, nullptr};
      delete $1;
    }
  | id COLON type
    {
      @$ = @1;
      $$ = new ParamCtx{$1->location, $1->name,
        shared_ptr<TypeCtx>($3), nullptr};
      delete $1;
    }
  | id EQUALS expr
    {
      @$ = @1;
      $$ = new ParamCtx{$1->location, $1->name, nullptr,
        shared_ptr<ExprCtx>($3)};
      delete $1;
    }
  | id COLON type EQUALS expr
    {
      @$ = @1;
      $$ = new ParamCtx{$1->location, $1->name,
        shared_ptr<TypeCtx>($3),
        shared_ptr<ExprCtx>($5)};
      delete $1;
    }
;

param_list:
  %empty { $$ = nullptr; }
  | param { $$ = new ParamList($1); @$ = @1; }
  | param_list COMMA param
    {
      auto *v = new ParamList($3);
      if ($1 == nullptr) {
        $$ = v;
        @$ = @3;
      } else {
        $$ = $1->add(v);
        @$ = @1;
      }
    }