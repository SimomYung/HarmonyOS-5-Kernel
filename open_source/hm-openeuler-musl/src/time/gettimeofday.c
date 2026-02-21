#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include "syscall.h"
#include "atomic.h"

#ifdef VDSO_GETTIME_OF_DAY_SYM

static void *volatile vdso_func;

static int gettimeofday_init(struct timeval *tv)
{
	void *p = __vdsosym(VDSO_GETTIME_OF_DAY_VER, VDSO_GETTIME_OF_DAY_SYM);
	int (*f)(struct timeval *tv) =
		(int (*)(struct timeval *))p;
	a_cas_p(&vdso_func, (void *)gettimeofday_init, p);
	return f ? f(tv) : -ENOSYS;
}

static void *volatile vdso_func = (void *)gettimeofday_init;

#endif

int gettimeofday(struct timeval *restrict tv, void *restrict tz)
{
	int r;
	if (!tv) return 0;
#ifdef VDSO_GETTIME_OF_DAY_SYM
	int (*f)(struct timeval *) =
		(int (*)(struct timeval *))vdso_func;
	if (f) {
		r = f(tv);
		if (!r) return r;
	}
#endif

	r = __syscall(SYS_gettimeofday, tv, tz);

	return __syscall_ret(r);
}
