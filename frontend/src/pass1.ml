(* let resolve_expr = function
  | Ast.ConstInt (span, value) -> Pass1_types.ConstInt (span, value)
  | Ast.ConstFloat (span, value) -> Pass1_types.ConstFloat (span, value)
  | Ast.ConstBool (span, value) -> Pass1_types.ConstBool (span, value)
  | Ast.ConstChar (span, value) -> begin
    let u8 = Pass1_types.IntType (false, 1) in
    let obj = Pass1_types.ConstInt (span, (Char.code value)) in
    Pass1_types.Cast (span, u8, obj)
    end
  | _ -> Pass1_types.ConstInt ({}, 2) *)