#include <sched.h>
#include "syscall.h"

int sched_setscheduler(pid_t pid, int sched, const struct sched_param *param)
{
	return (int)syscall((long)SYS_sched_setscheduler, pid, sched, param);
}
