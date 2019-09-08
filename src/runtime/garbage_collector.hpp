#ifndef MANDA_GARBAGE_COLLECTOR_HPP
#define MANDA_GARBAGE_COLLECTOR_HPP
#include <jit/jit.h>
#include <unordered_map>

namespace manda::runtime {
class GarbageCollector {
public:
  void *allocate(jit_uint size);
  void incref(void *ptr);
  void decref(void *ptr);

  template <typename T, class... Args> T *make(Args &&... args) {
    auto *out = new T(args...);
    incref((void *)out);
    return out;
  }

private:
  std::unordered_map<void *, unsigned long> refCounts;
};
} // namespace manda::runtime

#endif
