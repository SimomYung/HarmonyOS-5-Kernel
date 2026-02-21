// SPDX-License-Identifier: GPL-2.0-only
#include <linux/sched/debug.h>
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/sched/user.h>
#include <linux/init.h>
#include <linux/kmemleak.h>

#define SYNCHRONOUS_MASK \
	(sigmask(SIGSEGV) | sigmask(SIGBUS) | sigmask(SIGILL) | \
	 sigmask(SIGTRAP) | sigmask(SIGFPE) | sigmask(SIGSYS))

struct __sighand_struct {
	spinlock_t siglock;
	refcount_t count;
};

void __init_sighand(struct task_struct *tsk)
{
	static __thread struct __sighand_struct tls_sig __aligned(8);
	struct sighand_struct *sig = (struct sighand_struct *)&tls_sig;

	RCU_INIT_POINTER(tsk->sighand, sig);
	refcount_set(&sig->count, 1);
	spin_lock_init(&sig->siglock);
	kmemleak_alloc(sig, sizeof(struct __sighand_struct), 0, GFP_KERNEL);
}

int __init_sighand_kthread(struct task_struct *tsk)
{
	struct sighand_struct *sig;

	sig = kmem_cache_alloc(sighand_cachep, GFP_KERNEL);
	RCU_INIT_POINTER(tsk->sighand, sig);
	if (!sig)
		return -ENOMEM;

	refcount_set(&sig->count, 1);

	/* deep copy */
	spin_lock_irq(&current->sighand->siglock);
	memcpy(sig->action, current->sighand->action, sizeof(sig->action));
	spin_unlock_irq(&current->sighand->siglock);

	return 0;
}

void __cleanup_sighand(struct sighand_struct *sighand)
{
	if (!refcount_dec_and_test(&sighand->count))
		panic("liblinux: %s: sighand_struct refcount abnormal\n", __func__);
	kmem_cache_free(sighand_cachep, sighand);
}

int next_signal(struct sigpending *sig_pending, sigset_t *mask)
{
	unsigned long i, *s, *m, tmp;
	int sig_v = 0;

	s = sig_pending->signal.sig;
	m = mask->sig;

	tmp = *s & ~*m;
	if (tmp) {
		if (tmp & SYNCHRONOUS_MASK)
			tmp &= SYNCHRONOUS_MASK;
		sig_v = ffz(~tmp) + 1;
		return sig_v;
	}

	switch (_NSIG_WORDS) {
	case 1:
		/* Nothing to do */
		break;
	case 2:
		tmp = s[1] & ~m[1];
		if (!tmp)
			break;
		sig_v = ffz(~tmp) + _NSIG_BPW + 1;
		break;
	default:
		for (i = 1; i < _NSIG_WORDS; ++i) {
			tmp = *++s & ~*++m;
			if (!tmp)
				continue;
			sig_v = ffz(~tmp) + i * _NSIG_BPW + 1;
			break;
		}
		break;
	}

	return sig_v;
}

static void collect_signal(int sig, struct sigpending *list, kernel_siginfo_t *info,
			   bool *resched_timer)
{
	sigdelset(&list->signal, sig);
}

void recalc_sigpending(void)
{
	clear_thread_flag(TIF_SIGPENDING);
}
EXPORT_SYMBOL(recalc_sigpending);

int dequeue_signal(struct task_struct *tsk, sigset_t *mask, kernel_siginfo_t *info)
{
	bool resched_timer = false;
	int signr;

	signr = next_signal(&tsk->pending, mask);
	if (signr)
		collect_signal(signr, &tsk->pending, info, &resched_timer);

	recalc_sigpending();
	if (!signr)
		return 0;

	return signr;
}
EXPORT_SYMBOL_GPL(dequeue_signal);

void signal_wake_up_state(struct task_struct *t, unsigned int state)
{
	set_tsk_thread_flag(t, TIF_SIGPENDING);
	wake_up_state(t, state | TASK_INTERRUPTIBLE);
}

static inline bool legacy_queue(struct sigpending *signals, int sig)
{
	return (sig < SIGRTMIN) && sigismember(&signals->signal, sig);
}

static void complete_signal(int sig, struct task_struct *p, enum pid_type type)
{
	struct task_struct *t = p;

	signal_wake_up(t, sig == SIGKILL);
	return;
}

static int __send_signal(int sig, struct kernel_siginfo *info, struct task_struct *t,
			 enum pid_type type, bool force)
{
	struct sigpending *pending;
	int ret = 0;

	if (type != PIDTYPE_PID) {
		pr_warn("liblinux: %s: only support PIDTYPE_PID\n", __func__);
		return -EINVAL;
	}

	pending = &t->pending;

	if (legacy_queue(pending, sig))
		goto ret;

	if (!(t->flags & PF_KTHREAD)) {
		pr_warn("liblinux: %s: only support kthread\n", __func__);
		return -EINVAL;
	}

	sigaddset(&pending->signal, sig);
	complete_signal(sig, t, type);
ret:
	return ret;
}

struct sighand_struct *__lock_task_sighand(struct task_struct *task,
					   unsigned long *flags)
{
	struct sighand_struct *sighand;

	rcu_read_lock();
	for (;;) {
		sighand = rcu_dereference(task->sighand);
		if (unlikely(sighand == NULL))
			break;

		spin_lock_irqsave(&sighand->siglock, *flags);
		if (likely(sighand == rcu_access_pointer(task->sighand)))
			break;
		spin_unlock_irqrestore(&sighand->siglock, *flags);
	}
	rcu_read_unlock();

	return sighand;
}

static int send_signal(int sig, struct kernel_siginfo *info, struct task_struct *t,
		       enum pid_type type)
{
	bool force = false;

	if (info == SEND_SIG_PRIV) {
		force = true;
	} else {
		pr_warn("liblinux: %s: only support SEND_SIG_PRIV\n", __func__);
		return -EINVAL;
	}

	return __send_signal(sig, info, t, type, force);
}

int do_send_sig_info(int sig, struct kernel_siginfo *info, struct task_struct *p,
		     enum pid_type type)
{
	int ret = -ESRCH;
	unsigned long flag;

	if (lock_task_sighand(p, &flag)) {
		ret = send_signal(sig, info, p, type);
		unlock_task_sighand(p, &flag);
	}

	return ret;
}

int send_sig_info(int sig, struct kernel_siginfo *info, struct task_struct *p)
{
	if (!valid_signal(sig))
		return -EINVAL;

	if (sig != SIGUSR1) {
		pr_warn("liblinux: %s: only support SIGUSR1\n", __func__);
		return -EINVAL;
	}

	return do_send_sig_info(sig, info, p, PIDTYPE_PID);
}
EXPORT_SYMBOL(send_sig_info);
