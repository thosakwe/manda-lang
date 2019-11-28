type compilation_unit_ctx = (directive list)
and comment = Lexing.position * string
and node_info = (comment list) * Lexing.position
and directive =
  | ImportAll of node_info * string
  | ImportSome of node_info * string * (string list)
and decl =
  | TypeDecl of node_info * string * typ
  | ExprDecl of expr
and typ =
  | TypeRef of node_info * string
and expr =
  | ConstInt of node_info * int
  | ConstFloat of node_info * float
  | ConstBool of node_info * bool
  | ConstChar of node_info * char
  | BinOp of expr * (node_info * string) * expr
and string_part =
  | Text of node_info * string

let node_info_of_expr = function
  | ConstInt (info, _) -> info
  | ConstFloat (info, _) -> info
  | ConstBool (info, _) -> info
  | ConstChar (info, _) -> info
  | BinOp (_, (info, _), _) -> info

let position_of_expr exp = 
  let (_, pos) = node_info_of_expr exp in
  pos