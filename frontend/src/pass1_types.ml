type typ =
  | IntType of bool * int
  | FloatType of int
  | BoolType
  | OptionalType of typ
  | UnresolvedType of Ast.span * string
and obj =
  | ConstInt of Ast.span * int
  | ConstFloat of Ast.span * float
  | ConstBool of Ast.span * bool
  | Cast of Ast.span * typ * obj
  | Block of Ast.span * typ * (obj list)
  | BinOp of Ast.span * obj * string * obj
  | Call of Ast.span * typ * obj * (obj list)
  | Fn of Ast.span * string * (param list) * obj
  | If of Ast.span * if_clause * (if_clause list) *  (obj option)
  | Var of Ast.span * string * obj
  | NullCheck of Ast.span * obj
  | Reference of Ast.span * string
and if_clause = Ast.span * obj * obj
and param = Ast.span * string * typ
and symbol =
  | Object of obj
  | Type of typ
and scope =
  {
    parent: scope option;
    variables: symbol Map.Make(String).t;
  }