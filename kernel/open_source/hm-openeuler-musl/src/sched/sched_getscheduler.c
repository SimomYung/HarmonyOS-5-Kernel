#include <sched.h>
#include "syscall.h"

int sched_getscheduler(pid_t pid)
{
	return (int)syscall((long)SYS_sched_getscheduler, pid);
}
