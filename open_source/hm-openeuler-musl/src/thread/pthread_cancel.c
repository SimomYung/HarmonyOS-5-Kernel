#define _GNU_SOURCE
#include <string.h>
#include "pthread_impl.h"
#include "syscall.h"
#include "lock.h"

hidden long __cancel(), __syscall_cp_c();

long __cancel()
{
	pthread_t self = __pthread_self();
	if (self->canceldisable == PTHREAD_CANCEL_ENABLE || self->cancelasync)
		pthread_exit(PTHREAD_CANCELED);
	self->canceldisable = PTHREAD_CANCEL_DISABLE;
	return -ECANCELED;
}

long __syscall_cp_c(syscall_arg_t nr,
                    syscall_arg_t u, syscall_arg_t v, syscall_arg_t w,
                    syscall_arg_t x, syscall_arg_t y, syscall_arg_t z)
{
	pthread_t self;
	long r;
	int st;

	if ((st=(self=__pthread_self())->canceldisable)
	    && (st==PTHREAD_CANCEL_DISABLE || nr==SYS_close))
		return __syscall(nr, u, v, w, x, y, z);

	if (self->cancel != 0) {
		return __cancel();
	} else {
		a_store(&self->do_syscall, 1);
		r = __syscall(nr, u, v, w, x, y, z);
		a_store(&self->do_syscall, 0);
		if (r==-EINTR && nr!=SYS_close && self->cancel &&
		    self->canceldisable != PTHREAD_CANCEL_DISABLE)
			r = __cancel();
		return r;
	}
}

static void _sigaddset(sigset_t *set, int sig)
{
	unsigned s = sig-1;
	set->__bits[s/8/sizeof *set->__bits] |= 1UL<<(s&8*sizeof *set->__bits-1);
}

static void cancel_handler(int sig, siginfo_t *si, void *ctx)
{
	pthread_t self = __pthread_self();
	ucontext_t *uc = ctx;

	a_barrier();
	if (!self->cancel || self->canceldisable == PTHREAD_CANCEL_DISABLE) return;

	_sigaddset(&uc->uc_sigmask, SIGCANCEL);

	if (self->cancelasync || self->do_syscall) {
		uc->uc_mcontext.MC_PC = (uintptr_t)__cancel;
#ifdef CANCEL_GOT
		uc->uc_mcontext.MC_GOT = CANCEL_GOT;
#endif
		return;
	}

	__syscall(SYS_tkill, self->tid, SIGCANCEL);
}

void __testcancel()
{
	pthread_t self = __pthread_self();
	if (self->cancel && !self->canceldisable)
		__cancel();
}

static void init_cancellation()
{
	struct sigaction sa = {
		.sa_flags = SA_SIGINFO | SA_RESTART,
		.sa_sigaction = cancel_handler
	};
	memset(&sa.sa_mask, -1, _NSIG/8);
	__libc_sigaction(SIGCANCEL, &sa, 0);
}

int pthread_cancel(pthread_t t)
{
	static int init;
	if (!is_pthread_valid(t)) {
		return ESRCH;
	}
	if (!init) {
		init_cancellation();
		init = 1;
	}
	if (t == pthread_self()) {
		a_store(&t->cancel, 1);
		if (t->canceldisable == PTHREAD_CANCEL_ENABLE && t->cancelasync)
			pthread_exit(PTHREAD_CANCELED);
		return 0;
	}

	int r;
	sigset_t set;
	/* Block not just app signals, but internal ones too,
	 * since pthread_cancel must be async-cancel-safe. */
	__block_all_sigs(&set);
	LOCK(t->killlock);

	a_store(&t->cancel, 1);

	r = (t->tid != 0) ? (int)(-__syscall(SYS_tkill, t->tid, SIGCANCEL)) : 0;
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}
