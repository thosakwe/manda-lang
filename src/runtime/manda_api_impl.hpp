#ifndef MANDA_MANDA_API_IMPL_HPP
#define MANDA_MANDA_API_IMPL_HPP
#include "interpreter.hpp"
#include <manda_api.h>
#include <optional>
#include <vector>

// struct manda_jit_any {
//    void *data;
//    unsigned long long type_id;
// };

extern "C" struct _manda_object {
  manda::runtime::Object *object;
  unsigned long long typeId;
}; // namespace manda::runtime

extern "C" struct _manda_context {
  manda::runtime::Interpreter &interpreter;
  std::vector<_manda_object> arguments;
  std::optional<_manda_object> returnValue;
};

#endif
