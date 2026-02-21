#include <sched.h>
#include "pthread_impl.h"

static int check_sched_priority(int prio, int policy)
{
	int prio_min, prio_max;
	prio_min = sched_get_priority_min(policy);
	prio_max = sched_get_priority_max(policy);

	if (prio_min >= 0 && prio_max >= 0 && prio >= prio_min && prio <= prio_max)
		return 0;
	return EINVAL;
}

int pthread_attr_setschedparam(pthread_attr_t *restrict a, const struct sched_param *restrict param)
{
	int check = check_sched_priority(param->sched_priority, a->_a_policy);
	if (check != 0u)
		return check;
	a->_a_prio = param->sched_priority;
	return 0;
}
