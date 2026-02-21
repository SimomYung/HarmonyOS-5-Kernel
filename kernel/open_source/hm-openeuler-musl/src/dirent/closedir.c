#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "__dirent.h"

int closedir(DIR *dir)
{
	if (dir == NULL || safe_copy(dir, dir, sizeof(DIR)) < 0) {
		errno = EINVAL;
		return -1;
	}
	int ret = close(dir->fd);
	free(dir);
	return ret;
}
