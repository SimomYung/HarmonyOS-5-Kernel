#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int tcsetpgrp(int fd, pid_t pgrp)
{
	int pgrp_int = pgrp;

	if (!isatty(fd)) {
		return -1;
	}

	return ioctl(fd, TIOCSPGRP, &pgrp_int, sizeof(pgrp_int));
}
