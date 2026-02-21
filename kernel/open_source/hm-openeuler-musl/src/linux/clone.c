#define _GNU_SOURCE
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include "pthread_impl.h"
#include "syscall.h"

struct start_args {
	int (*start_func)(void *);
	void *start_arg;
};

#define ALIGN_UP(x, y) ((x)+(y)-1 & -(y))

static int clone_entry(void *p)
{
	int ret;
	struct start_args *args = p;
	int (*start)(void *) = (int(*)(void *))args->start_func;
#ifdef CONFIG_SHADOW_STACK
	void *sstack = __mmap_sstack();
	if (sstack == NULL) {
		errno = ENOMEM;
		return -1;
	}
	set_shadow_stack(shadow_stack_start_addr((uintptr_t)sstack));
#endif
	ret = (int)start(args->start_arg);
#ifdef CONFIG_SHADOW_STACK
	__munmap(sstack, SHADOW_STACK_TOTAL_SIZE);
#endif

	__syscall(SYS_exit, ret);
	__builtin_unreachable();
}

int clone(int (*func)(void *), void *stack, int flags, void *arg, ...)
{
	va_list ap;
	pid_t *ptid, *ctid;
	void  *tls;

	if ((func == NULL) || (stack == NULL)) {
		errno = EINVAL;
		return -1;
	}

	va_start(ap, arg);
	ptid = va_arg(ap, pid_t *);
	tls  = va_arg(ap, void *);
	ctid = va_arg(ap, pid_t *);
	va_end(ap);

	if ((unsigned int)flags & CLONE_THREAD) {
		uintptr_t stack_tmp = (uintptr_t)stack;
		stack_tmp -= stack_tmp % 16;
		stack_tmp -= ALIGN_UP(sizeof(struct start_args), 16);
		struct start_args *args = (void *)stack_tmp;
		args->start_func = func;
		args->start_arg = arg;
		return __syscall_ret(__clone(clone_entry, (void *)stack_tmp, flags, args, ptid, tls, ctid));
	} else {
		return __syscall_ret(__clone(func, stack, flags, arg, ptid, tls, ctid));
	}
}
