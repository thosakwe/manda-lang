#ifndef MANDA_GARBAGE_COLLECTOR_HPP
#define MANDA_GARBAGE_COLLECTOR_HPP
#include <jit/jit.h>
#include <unordered_map>

namespace manda::runtime {
template <typename T> class GCPointer;
class GarbageCollector {
public:
  void *allocate(jit_uint size);
  void incref(void *ptr);
  void decref(void *ptr);
  unsigned long refCount(void *ptr) const;
  static void static_incref(GarbageCollector *gc, void *ptr);
  static void static_decref(GarbageCollector *gc, void *ptr);

  template <typename T, class... Args> T *make(Args &&... args) {
    auto *out = new T(args...);
    incref((void *)out);
    return out;
  }

private:
  std::unordered_map<void *, unsigned long> refCounts;
};

template <typename T> class GCPointer {
public:
  GCPointer(GarbageCollector &gc, T *ptr) : gc(gc), ptr(ptr) { gc.incref(ptr); }
  GCPointer(const GCPointer<T> &other) : GCPointer(other.gc, other.ptr) {}
  GCPointer(GCPointer<T> &&other) noexcept : GCPointer(other.gc, other.ptr) {}
  GCPointer<T> &operator=(const GCPointer<T> &other) {
    gc.decref(ptr);
    gc = other.gc;
    ptr = other.ptr;
    return *this;
  }
  GCPointer<T> &operator=(GCPointer<T> &&other) noexcept {
    gc.decref(ptr);
    gc = other.gc;
    ptr = other.ptr;
    return *this;
  }
  ~GCPointer() { gc.decref(ptr); }
  // TODO: Prevent access to dead pointers...
  T *operator->() { return ptr; }
  const T *operator->() const { return ptr; }
  explicit operator bool() const {
    return ptr != nullptr && gc.refCount(ptr) > 0;
  }
  [[nodiscard]] unsigned long refCount() const { return gc.refCount(ptr); }
  const T &operator*() const { return *ptr; }
  T &operator*() { return *ptr; }

private:
  GarbageCollector &gc;
  T *ptr;
};

template <typename T, class... Args>
GCPointer<T> make_gc(GarbageCollector &gc, Args &&... args) {
  auto *ptr = new T(args...);
  return GCPointer<T>(gc, ptr);
}
} // namespace manda::runtime

#endif
