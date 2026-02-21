#include <mqueue.h>
#include <hmsyscall.h>
#include "syscall.h"

int mq_close(mqd_t mqd)
{
	return syscall(SYS_mq_close, mqd);
}
