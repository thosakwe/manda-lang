#ifndef MANDA_MANDA_API_IMPL_HPP
#define MANDA_MANDA_API_IMPL_HPP
#include "interpreter.hpp"
#include <manda_api.h>
#include <vector>

namespace manda::runtime {

// struct manda_jit_any {
//    void *data;
//    unsigned long long type_id;
// };

struct MandaAny {
  Object *object;
  unsigned long long typeId;
};

extern "C" struct _manda_context {
  Interpreter &interpreter;
  unsigned long argumentCount;
  MandaAny *arguments;
  MandaAny returnValue;
};
} // namespace manda::runtime

#endif
