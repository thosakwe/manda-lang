type <T> MyOption = sum
  None;
  Some(&T)

fn timesTwo (n: MyOption<i32>) ->
  match n with
    None -> 0
    Some(value) -> value * 2