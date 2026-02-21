#include <sys/stat.h>
#include "syscall.h"

int futimens(int fd, const struct timespec times[2])
{
	return syscall(SYS_utimensat, fd, 0, times, 0);
}
