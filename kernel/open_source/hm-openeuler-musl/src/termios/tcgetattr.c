#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int tcgetattr(int fd, struct termios *tio)
{
	if (!isatty(fd)) {
		return -1;
	}

	if (ioctl(fd, TCGETS, tio, sizeof(struct termios)))
		return -1;
	return 0;
}
