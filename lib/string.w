type String =
  interface {
    fn getLength: u64
    fn substring(start: u64, end: Option<u64> = None): String
  }

type AsciiString =
  class implements String {
    new(var arr: u8[] = []);
    fn getLength -> arr.length

    fn substring(start: u64, end: Option<u64> = None) {
      if (start < 0 || end >= length) then
        throw OutOfBoundsError()
      else
        BasicString(arr[start:end])
    }
  }