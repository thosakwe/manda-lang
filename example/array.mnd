type <T> MyVec =
  class {
    var array = [] as T[]
    u64 length = 0

    pub fn getLength() -> length

    pub fn add(value: T) {
      if length == 0 {
        array = [value]
      } else if length == array.length {
        // Double the size of the array
        var newarr = alloc<T>(array.length * 2)
        for (var i : [0 .. array.length]) {
          newarr[i] = array[i]
        }
        newarr[length] = value
        array = newarr
      } else {
        array[length] = value
      }

      length++
    }
  }

fn main {
  var ints = MyVec<int>()
  for (var i : [0 .. 100]) {
    ints.add(i)
  }
  ints.getLength()
}