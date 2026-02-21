#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include "syscall.h"

pid_t vfork(void)
{
/**
* Hongmeng kernel realize vfork directly invokes syscall, so child process vforked may duplicate a locked mutex.
* The child will trap in dead-lock if try lock again.
* The modification should be reverted if Hongmeng kernel support the true vfork.
*/
#ifdef CONFIG_VFORK
	return fork();
#else
#ifdef SYS_fork
	return (pid_t)syscall(SYS_fork);
#else
	return (pid_t)syscall(SYS_clone, SIGCHLD, NULL, NULL, NULL, NULL, NULL, NULL);
#endif
#endif
}
