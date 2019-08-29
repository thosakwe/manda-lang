// Import specific symbols
import {OS, printOS} from sum_type

// Import all symbols from module
import generics

fn main {
  var os = generics.Some(Windows)
  printOptionalOS(os)
}

fn printOptionalOS(os: generics.MyOption<OS>) ->
  match os with
    None -> void;
    Some(value) -> printOS(value)