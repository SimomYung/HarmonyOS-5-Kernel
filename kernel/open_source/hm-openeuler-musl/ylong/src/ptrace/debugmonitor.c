

#include "syscall.h"
#include "sys/debugmonitor.h"

long debugmonitor(int request, int id, void *addr, void *data)
{
    return __syscall(SYS_debugmonitor, request, id, addr, data);
}
