#ifndef MANDA_MANDA_API_IMPL_HPP
#define MANDA_MANDA_API_IMPL_HPP
#include "interpreter.hpp"
#include <manda_api.h>

namespace manda::runtime {
extern "C" struct _manda_context { Interpreter &interpreter; };
} // namespace manda::runtime

#endif
