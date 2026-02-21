#include <sched.h>
#include "syscall.h"

int sched_setparam(pid_t pid, const struct sched_param *param)
{
	return (int)syscall((long)SYS_sched_setparam, pid, param);
}
