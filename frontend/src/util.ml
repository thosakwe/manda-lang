let string_of_position (pos:Lexing.position) =
  pos.pos_fname
  ^ ":" ^ (string_of_int pos.pos_lnum)
  ^ ":" ^ (string_of_int pos.pos_cnum)