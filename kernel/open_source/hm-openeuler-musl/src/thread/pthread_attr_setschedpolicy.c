#include "pthread_impl.h"

int pthread_attr_setschedpolicy(pthread_attr_t *a, int policy)
{
	if (a == NULL || !is_sched_policy_valid(policy))
		return EINVAL;
	a->_a_policy = policy;
	return 0;
}
