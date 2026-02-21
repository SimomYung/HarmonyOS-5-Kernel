#include <sched.h>
#include "syscall.h"

int sched_getparam(pid_t pid, struct sched_param *param)
{
	return (int)syscall((long)SYS_sched_getparam, pid, param);
}
