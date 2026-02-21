#include <time.h>
#include <limits.h>
#include "pthread_impl.h"

int timer_delete(timer_t t)
{
	int r = 0;
	if ((intptr_t)t < 0) {
		pthread_t td = (void *)((uintptr_t)t << 1);
		if (td->timer_id >= 0) {
			r = syscall(SYS_timer_delete, (unsigned int)td->timer_id & (unsigned int)INT_MAX);
			/* set td->timer_id to an invalid id */
			a_store(&td->timer_id, INT_MIN);
			__syscall(SYS_tkill, td->tid, SIGTIMER);
		}
		return r;
	}
	return syscall(SYS_timer_delete, t);
}
