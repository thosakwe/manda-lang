#include "manda_api_impl.hpp"
#include <jit/jit-util.h>
#include <sstream>

using namespace manda::runtime;
using namespace std;

_manda_context::~_manda_context() {
  // Dereference all pointers.
  if (!managed) {
    for (auto &arg : arguments) {
      interpreter.getGarbageCollector().decref(arg.object);
    }
  }
}

void _manda_context::addArgument(const _manda_object &value) {
  if (!managed) {
    interpreter.getGarbageCollector().incref(value.object);
  }
  arguments.push_back(value);
}

MANDA_EXTERN_C unsigned long manda_get_argument_count(manda_context_t context) {
  if (!context) {
    return 0;
  } else {
    return context->arguments.size();
  }
}

MANDA_EXTERN_C manda_object_t manda_get_argument(manda_context_t context,
                                                 unsigned long index) {
  if (!context || (index >= context->arguments.size())) {
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
  if (!context || !value || (value->object == nullptr)) {
    return MANDA_RESULT_ERROR;
  } else {
    // TODO: Mark duplicate pointer in GC
    ostringstream oss;
    value->object->print(oss, false);
    auto str = oss.str();
    //    *ptr = str.c_str();
    *ptr = jit_strdup(str.c_str());
    return MANDA_RESULT_OK;
  }
}