%token <string> COMMENT
%token <bool> BOOL
%token <int> DECIMAL
%token <float> FLOAT
%token <int> HEX
%token <string> ID

%token ARROW
%token COLON
%token COMMA
%token ELSE
%token EQUALS
%token FINAL
%token FN
%token IF
%token LBRACKET RBRACKET
%token LCURLY RCURLY
%token LPAREN RPAREN
%token QUESTION
%token THEN
%token TYPE
%token VAR

%token EOF

%start <Ast.compilation_unit> compilation_unit

%%

comment: text = COMMENT { ($loc, text) }

compilation_unit: decls = list(decl); EOF { ([], decls) }

decl:
  | value = expr { Ast.ExprDecl value }
  | comments = list(comment);
    TYPE;
    list(comment);
    name = ID;
    list(comment);
    EQUALS;
    value = typ
    { 
      Ast.TypeDecl (comments, $loc, name, value)
    }

expr:
  | list(comment); value = BOOL { Ast.ConstBool ($loc, value) }
  | list(comment); value = DECIMAL { Ast.ConstInt ($loc, value) }
  | list(comment); value = HEX { Ast.ConstInt ($loc, value) }
  | list(comment); value = FLOAT { Ast.ConstFloat ($loc, value) }
  | list(comment); LCURLY; exprs = list(expr); list(comment); RCURLY { Ast.Block($loc, exprs) }
  | target = expr; LPAREN; args = separated_list(COMMA, expr); RPAREN { Ast.Call($loc, target, args) }
  | main = if_clause; list(comment); else_if = list(else_if_clause); else_ = option(else_clause)
    {
      Ast.If ($loc, main, else_if, else_)
    }
  |
    list(comment);
    final = var_mod; 
    list(comment);
    name = ID;
    EQUALS;
    value = expr
    {
      Ast.Var($loc, final, name, value)
    }
  |
    comments = list(comment);
    FN;
    list(comment);
    name = ID;
    params = option(param_list);
    body = option(fn_body)
    {
      let p = match params with
        | Some lst -> lst
        | None -> []
      in
      Ast.Fn($loc, comments, name, p, body)
    }
  | list(comment); QUESTION; value = expr { Ast.NullCheck($loc, value) }
  | list(comment); LPAREN; value = expr; list(comment); RPAREN { value }

fn_body: 
  | list(comment); ARROW; value = expr { value }
  | value = expr { value }

param_list: list(comment); LPAREN; params = separated_list(COMMA, param); list(comment); RPAREN { params }
param: list(comment); name = ID; typ = option(param_type) { ($loc, name, typ) }
param_type: list(comment); COLON; value = typ { value }

if_clause: list(comment); IF; condition = expr; option(THEN); body = expr { ($loc, condition, body) }
else_if_clause: ELSE; value = if_clause { value }
else_clause: list(comment); ELSE; value = expr { value }

var_mod:
  | FINAL { true }
  | VAR { false }

typ:
  | list(comment); name = ID { Ast.TypeRef ($loc, name) }
  | inner = typ; QUESTION { Ast.OptionalType($loc, inner) }