#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "syscall.h"

int tcdrain(int fd)
{
	if (!isatty(fd)) {
		return -1;
	}

	return syscall_cp(SYS_ioctl, fd, TCSBRK, 1);
}
