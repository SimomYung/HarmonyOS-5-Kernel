#define _GNU_SOURCE
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "pthread_impl.h"

int pthread_setname_np(pthread_t thread, const char *name)
{
    size_t len;
    if ((len = strnlen(name, 16)) > 15) {
        return ERANGE;
    }

    return prctl(PR_SET_TNAME, (unsigned long)name, thread->tid, 0UL, 0UL) ? errno : 0;
}
