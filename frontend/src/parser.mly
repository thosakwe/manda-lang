%token <Ast.comment> COMMENT
%token EOF

%%

comment:
  | EOF { None }
  | x = COMMENT { Some x }

comments: x = list(comment) { x }