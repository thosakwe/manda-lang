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
  if (ptr == nullptr)
    return;
  marked[ptr] = true;
  sweep();
  //  marked.insert(ptr);
  //  auto it = refCounts.find(ptr);
  //  if (it != refCounts.end()) {
  //    it->second++;
  //  } else {
  //    refCounts[ptr] = 0;
  //  }
}

void GarbageCollector::decref(void *ptr) {
  if (ptr == nullptr)
    return;
  marked.erase(ptr);
  //  auto it = refCounts.find(ptr);
  //  if (it != refCounts.end()) {
  //    if (it->second <= 0) {
  //      refCounts.erase(it);
  //      free(it->first);
  //    } else {
  //      it->second--;
  //    }
  //  }
}

void GarbageCollector::sweep() {
  // TODO: Fix sweeping
  return;
  if (marked.size() > maxAllocationsBeforeSweep) {
    maxAllocationsBeforeSweep *= 2;
  }
  if (marked.size() >= maxAllocationsBeforeSweep) {
    // Sweep empty cells.
    auto it = marked.begin();
    while (it != marked.end()) {
      if (it->second) {
        // Unmark.
        it->second = false;
        it++;
      } else {
        // Delete.
        // TODO: Destructors???
        free(it->first);
        marked.erase(it++);
      }
    }
  }
}

void GarbageCollector::static_incref(GarbageCollector *gc, void *ptr) {
  gc->incref(ptr);
}

void GarbageCollector::static_decref(GarbageCollector *gc, void *ptr) {
  gc->decref(ptr);
}

unsigned long GarbageCollector::refCount(void *ptr) const {
  auto it = marked.find(ptr);
  if (it == marked.end()) {
    return 0;
  } else {
    return 1;
  }
  //  auto it = refCounts.find(ptr);
  //  if (it == refCounts.find(ptr)) {
  //    return 0;
  //  } else {
  //    return it->second;
  //  }
}
