%token <string> COMMENT
%token <bool> BOOL
%token <int> DECIMAL
%token <float> FLOAT
%token <int> HEX
%token <string> ID

%token EQUALS
%token TYPE

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
      Ast.TypeDecl  ((comments, $loc), name, value)
    }

expr:
  | comments = list(comment); value = BOOL { Ast.ConstBool ((comments, $loc), value) }
  | comments = list(comment); value = DECIMAL { Ast.ConstInt ((comments, $loc), value) }
  | comments = list(comment); value = HEX { Ast.ConstInt ((comments, $loc), value) }
  | comments = list(comment); value = FLOAT { Ast.ConstFloat ((comments, $loc), value) }

typ:
  | comments = list(comment); name = ID { Ast.TypeRef ((comments, $loc), name) }
