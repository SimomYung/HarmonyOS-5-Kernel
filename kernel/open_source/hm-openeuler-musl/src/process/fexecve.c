#define _GNU_SOURCE
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"

int fexecve(int fd, char *const argv[], char *const envp[])
{
	if (fd < 0 || argv == NULL || envp == NULL) {
		errno = EINVAL;
		return -1;
	}

	char tmp[PATH_MAX];
	int r = __syscall(SYS_execveat, fd, "", argv, envp, AT_EMPTY_PATH);
	if (r != -ENOSYS) return __syscall_ret(r);
	char buf[15 + 3*sizeof(int)];
	__procfdname(buf, fd);
	r = readlink(buf, tmp, sizeof tmp - 1);
	if (r < 0) {
		return -1;
	}
	tmp[r] = 0;
	execve(tmp, argv, envp);
	if (errno == ENOENT) errno = EBADF;
	return -1;
}
