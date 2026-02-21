#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

pid_t tcgetsid(int fd)
{
	int sid;

	if (!isatty(fd)) {
		return -1;
	}

	if (ioctl(fd, TIOCGSID, &sid) < 0)
		return -1;
	return sid;
}
