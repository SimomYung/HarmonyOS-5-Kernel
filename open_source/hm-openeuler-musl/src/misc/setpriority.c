#include <sys/resource.h>
#include "syscall.h"
#include <errno.h>

int setpriority(int which, id_t who, int prio)
{
	return syscall(SYS_setpriority, which, who, prio);
}
