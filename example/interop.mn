// The `opaque` type represents a void*, but cannot
// be converted to W types.
extern malloc(size: u64) : opaque
extern free(ptr: opaque) : void

fn main {
  var ptr = malloc(32)
  free(ptr)
}