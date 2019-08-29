pub type <T> MyOption = sum
  None;
  Some(&T)

pub fn timesTwo (n: MyOption<i32>) ->
  match n with
    None -> 0;
    Some(value) -> value * 2