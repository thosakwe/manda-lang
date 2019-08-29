// IMPORTANT: Pointers in W cannot be converted to/from numbers;
// therefore, there is no null pointer.
fn main {
  var i = 13
  var pi = &i
  *pi = 23
  i
}
