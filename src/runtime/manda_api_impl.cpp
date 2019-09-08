#include "manda_api_impl.hpp"
#include <jit/jit-util.h>
#include <sstream>

using namespace manda::runtime;
using namespace std;

MANDA_EXTERN_C unsigned long manda_get_argument_count(manda_context_t context) {
  if (!context) {
    return 0;
  } else {
    return context->argumentCount;
  }
}

MANDA_EXTERN_C manda_object_t manda_get_argument(manda_context_t context,
                                                 unsigned long index) {
  if (!context || (index >= context->argumentCount)) {
    return nullptr;
  } else {
    return &context->arguments[index];
  }
}

MANDA_EXTERN_C manda_result_t manda_return(manda_context_t context,
                                           manda_object_t value);

MANDA_EXTERN_C manda_result_t manda_return_void(manda_context_t context);

MANDA_EXTERN_C manda_result_t manda_to_string(manda_context_t context,
                                              manda_object_t value,
                                              const char **ptr) {
  if (!context || !value || !(value->object)) {
    return MANDA_RESULT_ERROR;
  } else {
    ostringstream oss;
    value->object->print(oss, false);
    auto str = oss.str();
    *ptr = str.c_str();
    return MANDA_RESULT_OK;
  }
}