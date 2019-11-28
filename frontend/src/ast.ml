type compilation_unit_ctx = (directive list)
and comments = string list
and node_info = comments * Lexing.position
and directive =
  | ImportAll of node_info * string
  | ImportSome of node_info * string * (string list)
and decl =
  | TypeDecl of node_info * string * typ
and typ =
  | TypeRef of node_info * string
and expr =
  | ConstInt of node_info * int
  | ConstFloat of node_info * float
  | ConstBool of node_info * bool
  | ConstChar of node_info * char
and string_part =
  | Text of node_info * string