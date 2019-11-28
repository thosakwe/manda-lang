%token <Ast.comment> COMMENT
%token EOF

%start <Ast.comment option> comment
%%

comment:
  | EOF { None }
  | x = COMMENT { Some x }