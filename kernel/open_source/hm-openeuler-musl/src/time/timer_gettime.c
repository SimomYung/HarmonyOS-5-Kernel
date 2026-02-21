#include "timer_impl.h"
#include "pthread_impl.h"

int timer_gettime(timer_t t, struct itimerspec *val)
{
	if ((intptr_t)t < 0) {
		int ret = __check_timer_id(&t);
		if (ret != 0) {
			return __syscall_ret(ret);
		}
	}
#ifdef SYS_timer_gettime64
	int r = -ENOSYS;
	if (sizeof(time_t) > 4)
		r = __syscall(SYS_timer_gettime64, t, val);
	if (SYS_timer_gettime == SYS_timer_gettime64 || r!=-ENOSYS)
		return __syscall_ret(r);
	long val32[4];
	r = __syscall(SYS_timer_gettime, t, val32);
	if (!r) {
		val->it_interval.tv_sec = val32[0];
		val->it_interval.tv_nsec = val32[1];
		val->it_value.tv_sec = val32[2];
		val->it_value.tv_nsec = val32[3];
	}
	return __syscall_ret(r);
#endif
	return syscall(SYS_timer_gettime, t, val);
}
