#define _GNU_SOURCE
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "pthread_impl.h"

int pthread_getname_np(pthread_t thread, const char *name, unsigned int isProcess)
{
    return prctl(PR_GET_TNAME, (unsigned long)name, thread->tid, isProcess, 0UL) ? errno : 0;
}
