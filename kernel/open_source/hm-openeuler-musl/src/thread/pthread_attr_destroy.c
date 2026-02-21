#include "pthread_impl.h"

int pthread_attr_destroy(pthread_attr_t *a)
{
	if (a->_a_cpuset != 0)
		free((void *)(uintptr_t)a->_a_cpuset);
	return 0;
}
