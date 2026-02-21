#define _BSD_SOURCE
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include "syscall.h"

void *sbrk(intptr_t inc)
{
	char *oldbrk;

	oldbrk = (char *)__syscall(SYS_brk, 0);
	if ((long)oldbrk < 0) {
		errno = ENOMEM;
		return (void *)-1;
	}
	if (inc == 0) {
		return (void *)oldbrk;
	}
	if ((inc > 0 && (uintptr_t)oldbrk + (uintptr_t)inc < (uintptr_t)oldbrk) ||
	    (inc < 0 && (uintptr_t)oldbrk < (uintptr_t)-inc)) {
		errno = ENOMEM;
		return (void *)-1;
	}
	if (__brk(oldbrk + inc) != 0) {
		errno = ENOMEM;
		return (void *)-1;
	}
	return (void *)oldbrk;
}
