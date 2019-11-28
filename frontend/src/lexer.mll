{
  open Lexing
  open Parser

  exception SyntaxError of string

  let next_line lexbuf =
  let pos = lexbuf.lex_curr_p in
  lexbuf.lex_curr_p <-
    { pos with pos_bol = lexbuf.lex_curr_pos;
               pos_lnum = pos.pos_lnum + 1
    }
}

let digit = ['0'-'9']
let frac = '.' digit*
let hex_digit = ['A'-'F' 'a'-'f' '0'-'9']+
let exp = ['e' 'E'] ['-' '+']? digit+
let decimal = digit*
let float = digit* frac? exp?
let hex = "0x" hex_digit+

let white = [' ' '\t']+
let newline = '\r' | '\n' | "\r\n"
let id = ['a'-'z' 'A'-'Z' '_'] ['a'-'z' 'A'-'Z' '0'-'9' '_']*

let comment = "//" [^'\n']+ '\n'

rule read = parse
  | white    { read lexbuf }
  | newline  { next_line lexbuf; read lexbuf }
  | decimal { DECIMAL (int_of_string (Lexing.lexeme lexbuf)) }
  | float { FLOAT (float_of_string (Lexing.lexeme lexbuf)) }
  | hex { HEX (int_of_string (Lexing.lexeme lexbuf)) }
  | "true" { BOOL true }
  | "false" { BOOL false }
  | id { ID (Lexing.lexeme lexbuf) }
  | eof { EOF }