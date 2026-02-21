#include <sys/resource.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"

#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define FIX(x) do{ if ((x)>=SYSCALL_RLIM_INFINITY) (x)=RLIM_INFINITY; }while(0)

int setrlimit(int resource, const struct rlimit *rlim)
{
	unsigned long k_rlim[2];
	struct rlimit tmp;
	if (SYSCALL_RLIM_INFINITY != RLIM_INFINITY) {
		tmp = *rlim;
		FIX(tmp.rlim_cur);
		FIX(tmp.rlim_max);
		rlim = &tmp;
	}
	int ret = syscall(SYS_prlimit64, 0, resource, rlim, 0);
	if (ret != -ENOSYS) return ret;
	k_rlim[0] = MIN(rlim->rlim_cur, MIN(-1UL, SYSCALL_RLIM_INFINITY));
	k_rlim[1] = MIN(rlim->rlim_max, MIN(-1UL, SYSCALL_RLIM_INFINITY));

	return syscall(SYS_setrlimit, resource, k_rlim);
}

weak_alias(setrlimit, setrlimit64);
