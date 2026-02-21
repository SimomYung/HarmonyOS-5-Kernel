#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int tcsendbreak(int fd, int dur)
{
	if (!isatty(fd)) {
		return -1;
	}

	/* nonzero duration is implementation-defined, so ignore it */
	return ioctl(fd, TCSBRK, 0);
}
