#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include "syscall.h"

long pathconf(const char *path, int name)
{
	if ((path == NULL) || (access(path, F_OK) != 0)) {
		return __syscall_ret((unsigned long)-ENOENT);
	}

	if (access(path, X_OK) != 0) {
		return __syscall_ret((unsigned long)-EACCES);
	}

	if (name == _PC_NAME_MAX) {
		struct statvfs sv;
		if (statvfs(path, &sv) < 0) {
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
		if ((stat(path, &st) < 0) ||
		    ((!S_ISREG(st.st_mode)) && (!S_ISBLK(st.st_mode))))
			return -1;
		else
			return 1;
	}

	return fpathconf(0, name);
}
