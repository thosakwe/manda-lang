type compilation_unit = (directive list) * (decl list)
and span = Lexing.position * Lexing.position
and comment = span * string
and directive =
  | ImportAll of span * string
  | ImportSome of span * string * (string list)
and decl =
  | TypeDecl of (comment list) * span * string * typ
  | ExprDecl of expr
and typ =
  | TypeRef of span * string
  | OptionalType of span * typ
and expr =
  | ConstInt of span * int
  | ConstFloat of span * float
  | ConstBool of span * bool
  | ConstChar of span * char
  | Block of span * (expr list)
  | BinOp of span * expr * string * expr
  | Call of span * expr * (expr list)
  | Fn of span * (comment list) * string * (param list) * (expr option)
  | If of span * if_clause * (if_clause list) * (expr option)
  | Var of span * bool * string * expr
  | NullCheck of span * expr
and if_clause = span * expr * expr
and param = span * string * (typ option)
and string_part =
  | Text of span * string

let span_of_expr = function
  | ConstInt (info, _) -> info
  | ConstFloat (info, _) -> info
  | ConstBool (info, _) -> info
  | ConstChar (info, _) -> info
  | Block (info, _) -> info
  | BinOp (info, _, _, _) -> info
  | Call (span, _, _) -> span
  | Fn (span, _, _, _, _) -> span
  | If (span, _, _, _) -> span
  | Var (span, _,  _, _) -> span
  | NullCheck (span, _) -> span