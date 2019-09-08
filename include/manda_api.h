#ifndef MANDA_API_H
#define MANDA_API_H

// TODO: Documentaton

#ifdef __cplusplus
#define MANDA_EXTERN_C extern "C"
#else
#define MANDA_EXTERN_C
#endif

typedef struct _manda_context *manda_context_t;
typedef struct _manda_object *manda_object_t;
typedef enum { MANDA_RESULT_OK, MANDA_RESULT_ERROR } manda_result_t;

MANDA_EXTERN_C manda_result_t manda_return(manda_context_t context,
                                           manda_object_t value);

MANDA_EXTERN_C manda_result_t manda_return_void(manda_context_t context);

MANDA_EXTERN_C manda_result_t manda_to_string(manda_context_t context,
                                              manda_object_t value,
                                              const char **ptr);

#endif