#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int tcflush(int fd, int queue)
{
	if (!isatty(fd)) {
		return -1;
	}

	return ioctl(fd, TCFLSH, queue, 0);
}
