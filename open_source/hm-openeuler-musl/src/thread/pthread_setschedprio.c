#include "pthread_impl.h"
#include "lock.h"

int pthread_setschedprio(pthread_t t, int prio)
{
	int r;
	if (!is_pthread_valid(t)) {
		return ESRCH;
	}
	sigset_t set;
	__block_app_sigs(&set);
	LOCK(t->killlock);
	r = !t->tid ? ESRCH : -__syscall(SYS_sched_setparam, t->tid, &prio);
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}
