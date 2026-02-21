#include "pthread_impl.h"
#include <threads.h>

static void *__pthread_getspecific(pthread_key_t k)
{
    if (k >= PTHREAD_KEYS_MAX || __pthread_keys[k] == NULL) {
        return NULL;
    }
	struct pthread *self = __pthread_self();
	return self->tsd[k];
}

weak_alias(__pthread_getspecific, pthread_getspecific);
weak_alias(__pthread_getspecific, tss_get);
