#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

pid_t tcgetpgrp(int fd)
{
	int pgrp;

	if (!isatty(fd)) {
		return -1;
	}

	if (ioctl(fd, TIOCGPGRP, &pgrp, sizeof(pgrp)) < 0)
		return -1;
	return pgrp;
}
