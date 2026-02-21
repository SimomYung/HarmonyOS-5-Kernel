#include "pthread_impl.h"

#ifdef __aarch64__
#undef DEFAULT_STACK_SIZE
#define DEFAULT_STACK_SIZE (8 * 1024 * 1024)
#endif

unsigned __default_stacksize = DEFAULT_STACK_SIZE;
unsigned __default_guardsize = DEFAULT_GUARD_SIZE;
