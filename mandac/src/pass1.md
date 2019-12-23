# Pass 1
The first pass turns Manda source code into an IR that the VM understands.
It targets a stack machine, and can easily be turned into, say, `libjit`
or `libgccjit` IR.
Before the intricate explanations, here are some idealized examples:

```
Source:
fn main -> 0

IR:
module "/path/to/file.mn" {
  func main() : i32 {
    block @entry {
      Const.i32 0
      Return
    }
  }
}
```

```
Source:
fn fib(n) ->
  if n <= 1 then
    n
  else fib(n - 1) + fib(n - 2)

IR:
TODO: Work on how this will work for generics
module "/path/to/file.mn" {
  func fib() : i32 {
    params {
      i32
    }
    locals {
      i32
    }
    block @entry {
      Const.i32 1
      CompareLte
      BrIf @terminate
      GetParam 0
      Const.i32 1
      Sub
      TailCall
      # Use TailCall instead of `Call "/path/to/file.mn" "fib"`
      GetParam 0
      Const.i32 2
      Sub
      TailCall
      Add
      # Note: This would be peephole-optimized
      SetLocal 0
      GetLocal 0
      Return
    }
    block @terminate {
      # Peephole-optimized, instead of chained SetLocal/GetLocal
      GetParam 0
      Return
    }
  }
}
```

```
Source:
class TimesTwo {
  new(var value: i32)

  fn compute -> value * 2
}

fn main {
  var tt = TimesTwo(2)
  tt.compute()
}

IR:
module "/path/to/file.mn" {
  class "TimesTwo" {
    members {
      i32
    }

    func new() : void {
      params {
        i32
      }
      block @entry {
        GetLocal 0
        SetMember 0
        Return
      }
    }

    func compute() : i32 {
      GetMember 0
      Const.i32 2
      Mul
      Return
    }
  }

  func main() : i32 {
    Const.i32 2
    New "/path/to/file.mn" "TimesTwo"
    CallMember "/path/to/file.mn" "TimesTwo" "compute"
    Return
  }
}
```