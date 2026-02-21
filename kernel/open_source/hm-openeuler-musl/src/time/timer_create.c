#include <time.h>
#include <setjmp.h>
#include <limits.h>
#include "pthread_impl.h"
#include "atomic.h"

struct ksigevent {
	union sigval sigev_value;
	int sigev_signo;
	int sigev_notify;
	int sigev_tid;
};

struct start_args {
	volatile int b;
	struct sigevent *sev;
};

static void dummy_0()
{
}
weak_alias(dummy_0, __pthread_tsd_run_dtors);

static void cleanup_fromsig(void *p)
{
	pthread_t self = __pthread_self();
	__pthread_tsd_run_dtors();
	self->cancel = 0;
	self->cancelbuf = 0;
	self->canceldisable = 0;
	self->cancelasync = 0;
	__reset_tls();
	longjmp(p, 1);
}

/*
 * child thread synchronize at a barrier
 * barrier val:
 *     0:  init state
 *     2:  child  thread arrive, update to be 2
 *     3:  parent thread arrive, update to be 3
 *     5:  child thread done
 * When child thread arrives, if the old barrier is 0,
 * it means the parent hasn't arrived yet, then wait,
 * otherwise wake
 */
static void __child_sync(volatile int *barrier)
{
	if (a_swap(barrier, 2) == 0) {
		__wait(barrier, 0, 2, 0);
	} else {
		__wake(barrier, 1, 0);
	}

	a_swap(barrier, 5);
	__wake(barrier, 1, 0);
}

/*
 * parent thread synchronize at a barrier
 * barrier val:
 *     0:  init state
 *     2:  child  thread arrive
 *     3:  parent thread arrive
 *     4:  parent thread should be wait
 *     5:  child thread done
 * When parent thread arrives, if the old barrier is 2,
 * it means the child is in wait state, so go to wake,
 * otherwize it will wait.
 */
static void __parent_sync(volatile int *barrier)
{
	if (a_swap(barrier, 3) == 2) {
		__wake(barrier, 1, 0);
	} else {
		__wait(barrier, 0, 3, 0);
	}

	if (a_swap(barrier, 4) != 5) {
		__wait(barrier, 0, 4, 0);
	}
}

static void *start(void *arg)
{
	pthread_t self = __pthread_self();
	struct start_args *args = arg;
	jmp_buf jb;

	void (*notify)(union sigval) = args->sev->sigev_notify_function;
	union sigval val = args->sev->sigev_value;

	__child_sync(&args->b);

	if (self->timer_id < 0)
		return 0;

	for (;;) {
		siginfo_t si;
		while (sigwaitinfo(SIGTIMER_SET, &si) < 0);
		if (si.si_code == SI_TIMER && !setjmp(jb)) {
			pthread_cleanup_push(cleanup_fromsig, jb);
			notify(val);
			pthread_cleanup_pop(1);
		}
		if (self->timer_id < 0) break;
	}
	return 0;
}

int timer_create(clockid_t clk, struct sigevent *restrict evp, timer_t *restrict res)
{
	volatile static int init = 0;
	pthread_t td;
	pthread_attr_t attr;
	int r;
	struct start_args args;
	struct ksigevent ksev, *ksevp=0;
	int timerid;
	sigset_t set;

	switch (evp ? evp->sigev_notify : SIGEV_SIGNAL) {
	case SIGEV_NONE:
	case SIGEV_SIGNAL:
	case SIGEV_THREAD_ID:
		if (evp) {
			ksev.sigev_value = evp->sigev_value;
			ksev.sigev_signo = evp->sigev_signo;
			ksev.sigev_notify = evp->sigev_notify;
			if (evp->sigev_notify == SIGEV_THREAD_ID)
				ksev.sigev_tid = evp->sigev_notify_thread_id;
			else
				ksev.sigev_tid = 0;
			ksevp = &ksev;
		}
		if (syscall(SYS_timer_create, clk, ksevp, &timerid) < 0)
			return -1;
		*res = (void *)(intptr_t)timerid;
		break;
	case SIGEV_THREAD:
		if (!init) {
			struct sigaction sa = { .sa_handler = SIG_DFL };
			__libc_sigaction(SIGTIMER, &sa, 0);
			a_store(&init, 1);
		}
		if (evp->sigev_notify_attributes)
			attr = *evp->sigev_notify_attributes;
		else
			pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		a_store(&args.b, 0);
		args.sev = evp;

		__block_app_sigs(&set);
		__syscall(SYS_rt_sigprocmask, SIG_BLOCK, SIGTIMER_SET, 0, _NSIG/8);
		r = pthread_create(&td, &attr, start, &args);
		__restore_sigs(&set);
		if (r) {
			errno = r;
			return -1;
		}

		ksev.sigev_value.sival_ptr = 0;
		ksev.sigev_signo = SIGTIMER;
		ksev.sigev_notify = SIGEV_THREAD_ID;
		ksev.sigev_tid = td->tid;
		if (syscall(SYS_timer_create, clk, &ksev, &timerid) < 0)
			timerid = -1;
		td->timer_id = timerid;
		__parent_sync(&args.b);
		if (timerid < 0) return -1;
		*res = (void *)(INTPTR_MIN | (uintptr_t)td>>1);
		break;
	default:
		errno = EINVAL;
		return -1;
	}

	return 0;
}
