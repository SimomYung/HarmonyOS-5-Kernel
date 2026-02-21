#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

int tcsetattr(int fd, int act, const struct termios *tio)
{
	if (!isatty(fd)) {
		return -1;
	}

	if (act < 0 || act > 2) {
		errno = EINVAL;
		return -1;
	}

	return ioctl(fd, TCSETS + act, tio, sizeof(struct termios));
}
