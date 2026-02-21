
#include <stdint.h>
#ifndef _SYS_PTRACE_H
#define _SYS_PTRACE_H

/* Standard calls - Similar to linux/FreeBSD */
#define PTRACE_TRACEME      0       /* tracee declares it's being traced */
#define PTRACE_PEEKDATA     2       /* read word in tracee's vm */
#define PTRACE_PEEKTEXT     PTRACE_PEEKDATA
#define PTRACE_POKEDATA     5       /* write word in tracee's vm */
#define PTRACE_POKETEXT     PTRACE_POKEDATA
#define PTRACE_CONT         7       /* continue the tracee */
#define PTRACE_SINGLESTEP   9       /* single step the tracee */
#define PTRACE_ATTACH       16      /* trace a running process */
#define PTRACE_DETACH       17      /* stop tracing a process */
#define PTRACE_GETSIGINFO   0x4202  /* which signal has caused the stop */
#define PTRACE_GETREGSET    0x4204  /* get general-purpose registers */
#define PTRACE_SETREGSET    0x4205  /* get general-purpose registers */
#define PTRACE_GET_RSEQ_CONFIGURATION	0x420f
/* System call */
struct __ptrace_rseq_configuration {
	uint64_t rseq_abi_pointer;
	uint32_t rseq_abi_size;
	uint32_t signature;
	uint32_t flags;
	uint32_t pad;
};
long ptrace(int request, int pid, void *addr, void *data);

#endif
