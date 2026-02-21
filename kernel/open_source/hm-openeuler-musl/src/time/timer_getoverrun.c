#include "timer_impl.h"
#include "pthread_impl.h"

int timer_getoverrun(timer_t t)
{
	if ((intptr_t)t < 0) {
		int ret = __check_timer_id(&t);
		if (ret != 0) {
			return __syscall_ret(ret);
		}
	}
	return syscall(SYS_timer_getoverrun, t);
}
