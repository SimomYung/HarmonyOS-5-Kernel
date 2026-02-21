#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "syscall.h"

int posix_openpt(int flags)
{
	int r = open("/dev/ptmx", flags);
	if (r < 0 && errno == ENOSPC) errno = EAGAIN;
	return r;
}

int grantpt(int fd)
{
	return 0;
}

int unlockpt(int fd)
{
	int unlock = 0;
	return ioctl(fd, TIOCSPTLCK, &unlock);
}

int __ptsname_r(int fd, char *buf, size_t len)
{
	int pty, err;

	if (!isatty(fd)) {
		return errno;
	}

	if (buf == NULL) {
		errno = EINVAL;
		return errno;
	}

	if ((err = __syscall(SYS_ioctl, fd, TIOCGPTN, &pty))) return -err;
	if ((size_t)snprintf(buf, len, "/dev/pts/%d", pty) >= len) {
		errno = ERANGE;
		return errno;
	}
	return 0;
}

weak_alias(__ptsname_r, ptsname_r);
