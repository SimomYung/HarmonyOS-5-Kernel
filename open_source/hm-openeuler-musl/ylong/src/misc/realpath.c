#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "syscall.h"

char *realpath(const char *filename, char *resolved)
{
    int ret = 0;
    char tmp[PATH_MAX] = { 0 };

    if (filename == NULL) {
        errno = EINVAL;
        return NULL;
    }

    ret = syscall(SYS_realpath, filename, tmp);
    if (ret < 0) {
        return NULL;
    }

    return resolved ? strcpy(resolved, tmp) : strdup(tmp);
}
