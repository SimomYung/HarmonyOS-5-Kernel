

#include "syscall.h"
#include "sys/ptrace.h"

long ptrace(int request, int pid, void *addr, void *data)
{
    return __syscall(SYS_ptrace, request, pid, addr, data);
}
