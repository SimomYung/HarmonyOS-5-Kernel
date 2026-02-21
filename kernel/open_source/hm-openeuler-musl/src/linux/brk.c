#define _BSD_SOURCE
#include <unistd.h>
#include <errno.h>
#include "syscall.h"

int __brk(void *end)
{
	long ret;

	ret = __syscall(SYS_brk, end);
	if (ret < 0) {
		errno = ENOMEM;
		return -1;
	}
	return 0;
}

weak_alias(__brk, brk);
