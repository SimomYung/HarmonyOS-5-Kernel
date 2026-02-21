#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "syscall.h"

long fpathconf(int fd, int name)
{
	if (__syscall(SYS_fcntl, fd, F_GETFD) < 0) {
		return __syscall_ret((unsigned long)-EBADF);
	}

	if ((name > _PC_2_SYMLINKS) || (name < _PC_LINK_MAX)) {
		return __syscall_ret((unsigned long)-EINVAL);
	}

	if (name == _PC_NAME_MAX) {
		struct statvfs sv;
		if (fstatvfs(fd, &sv) < 0) {
#ifdef NAME_MAX
			return NAME_MAX;
#else
			return -1;
#endif
		} else {
			return (long)sv.f_namemax;
		}
	}

	if (name == _PC_ASYNC_IO) {
		struct stat st;
		if ((fstat(fd, &st) < 0) || ((!S_ISREG(st.st_mode)) && (!S_ISBLK(st.st_mode))))
			return -1;
		else
			return 1;
	}

	static const short values[] = {
#ifdef LINK_MAX
		[_PC_LINK_MAX] = LINK_MAX,
#else
		[_PC_LINK_MAX] = _POSIX_LINK_MAX,
#endif

#ifdef MAX_CANON
		[_PC_MAX_CANON] = MAX_CANON,
#else
		[_PC_MAX_CANON] = _POSIX_MAX_CANON,
#endif

#ifdef  MAX_INPUT
		[_PC_MAX_INPUT] = MAX_INPUT,
#else
		[_PC_MAX_INPUT] = _POSIX_MAX_INPUT,
#endif
		[_PC_NAME_MAX] = 0,
#ifdef PATH_MAX
		[_PC_PATH_MAX] = PATH_MAX,
#else
		[_PC_PATH_MAX] = -1,
#endif

#ifdef PIPE_BUF
		[_PC_PIPE_BUF] = PIPE_BUF,
#else
		[_PC_PIPE_BUF] = -1,
#endif

#ifdef _POSIX_CHOWN_RESTRICTED
		[_PC_CHOWN_RESTRICTED] = _POSIX_CHOWN_RESTRICTED,
#else
		[_PC_CHOWN_RESTRICTED] = -1,
#endif

#ifdef _POSIX_NO_TRUNC
		[_PC_NO_TRUNC] = _POSIX_NO_TRUNC,
#else
		[_PC_NO_TRUNC] = -1,
#endif

#ifdef _POSIX_VDISABLE
		[_PC_VDISABLE] = _POSIX_VDISABLE,
#else
		[_PC_VDISABLE] = -1,
#endif

#ifdef _POSIX_SYNC_IO
		[_PC_SYNC_IO] = _POSIX_SYNC_IO,
#else
		[_PC_SYNC_IO] = -1,
#endif
		[_PC_ASYNC_IO] = 0,
#ifdef _POSIX_PRIO_IO
		[_PC_PRIO_IO] = _POSIX_PRIO_IO1,
#else
		[_PC_PRIO_IO] =-1,
#endif

#ifdef SOCK_MAXBUF
		[_PC_SOCK_MAXBUF] = SOCK_MAXBUF,
#else
		[_PC_SOCK_MAXBUF] = -1,
#endif

#ifdef FILESIZEBITS
		[_PC_FILESIZEBITS] = FILESIZEBITS,
#else
		[_PC_FILESIZEBITS] = 32,
#endif

		[_PC_REC_INCR_XFER_SIZE] = 4096,
		[_PC_REC_MAX_XFER_SIZE] = 4096,
		[_PC_REC_MIN_XFER_SIZE] = 4096,
		[_PC_REC_XFER_ALIGN] = 4096,
		[_PC_ALLOC_SIZE_MIN] = 4096,
		[_PC_SYMLINK_MAX] = -1,
		[_PC_2_SYMLINKS] = 1
	};
	return values[name];
}
