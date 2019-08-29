type OS = sum
  Windows;
  Mac;
  Linux(string);
  Other({name: string})

fn main {
  var ubuntu = Linux("Ubuntu")
  printOS(ubuntu)
}

fn printOS (os: OS) ->
  match os with
    Windows -> print("Windows");
    Mac -> print("Mac");
    Linux(variant) -> print("Linux - " + variant);
    Other({name}) -> print(name)
