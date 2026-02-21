
#include "syscall.h"
#include "pthread_impl.h"

unsigned long pthread_gettaskcpuusage(int type, pthread_t t, int cpuMode)
{
    return __syscall(__NR_getcpuusage, type, t->tid, cpuMode);
}

