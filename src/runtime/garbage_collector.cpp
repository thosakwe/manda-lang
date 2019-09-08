#include "garbage_collector.hpp"
#include <cstdlib>

using namespace manda::runtime;
using namespace std;

void *GarbageCollector::allocate(jit_uint size) {
  // TODO: Handle out-of-memory?
  auto *ptr = malloc(size);
  incref(ptr);
  return ptr;
}

void GarbageCollector::incref(void *ptr) {
  auto it = refCounts.find(ptr);
  if (it != refCounts.end()) {
    it->second++;
  } else {
    refCounts[ptr] = 1;
  }
}

void GarbageCollector::decref(void *ptr) {
  auto it = refCounts.find(ptr);
  if (it != refCounts.end()) {
    if ((--it->second) <= 0) {
      refCounts.erase(it);
      free(it->first);
    }
  }
}
