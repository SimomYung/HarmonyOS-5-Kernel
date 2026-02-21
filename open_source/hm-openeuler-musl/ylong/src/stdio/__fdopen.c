#include "stdio_impl.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "libc.h"

FILE *__fdopen(int fd, const char *mode)
{
	FILE *f;
	struct winsize wsz;

	/* Check for valid initial mode character */
	if (!strchr("rwa", *mode)) {
		errno = EINVAL;
		return 0;
	}

	int flags = __syscall(SYS_fcntl, fd, F_GETFL);
	/* hm-fix: Return EBADF if fd is invalid */
	if (flags == -EBADF) {
		errno = -flags;
		return 0;
	}

	/* Allocate FILE+buffer or fail */
	if (!(f=malloc(sizeof *f + UNGET + BUFSIZ))) return 0;

	/* Zero-fill only the struct, not the buffer */
	memset(f, 0, sizeof *f);

	/* Impose mode restrictions */
	if (!strchr(mode, '+')) f->flags = (*mode == 'r') ? F_NOWR : F_NORD;

	/* hm-fix:
	 * Check whether mode is compatible with fd mode.
	 * If fd is O_RDONLY, f->flags must with F_NOWR.
	 * If fd is O_WRONLY, f->flags must with F_NORD.
	 */
	if (((flags & O_ACCMODE) == O_RDONLY && !(f->flags & F_NOWR)) ||
	    ((flags & O_ACCMODE) == O_WRONLY && !(f->flags & F_NORD))) {
		free(f);
		errno = EINVAL;
		return 0;
	}

	/* Apply close-on-exec flag */
	if (strchr(mode, 'e')) __syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);

	/* Set append mode on fd if opened for append */
	if (*mode == 'a') {
		if (!(flags & O_APPEND))
			__syscall(SYS_fcntl, fd, F_SETFL, flags | O_APPEND);
		f->flags |= F_APP;
	}

	f->fd = fd;
	f->buf = (unsigned char *)f + sizeof *f + UNGET;
	f->buf_size = BUFSIZ;

	/* Activate line buffered mode for terminals */
	f->lbf = EOF;
	if (!(f->flags & F_NOWR) && !__syscall(SYS_ioctl, fd, TIOCGWINSZ, &wsz))
		f->lbf = '\n';

	/* Initialize op ptrs. No problem if some are unneeded. */
	f->read = __stdio_read;
	f->write = __stdio_write;
	f->seek = __stdio_seek;
	f->close = __stdio_close;

	if (!libc.threaded) f->lock = -1;

	/* Add new FILE to open file list */
	return __ofl_add(f);
}

weak_alias(__fdopen, fdopen);
