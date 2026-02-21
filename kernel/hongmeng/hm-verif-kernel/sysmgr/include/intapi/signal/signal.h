/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2023. All rights reserved.
 * Description: Apis for signal.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 10 20:33:34 2018
 */
#ifndef SYSMGR_INTAPI_SIGNAL_SIGNAL_H
#define SYSMGR_INTAPI_SIGNAL_SIGNAL_H

/* include this header first to use hm_signal_defs.h instead of signal.h */
#include <libhmsrv_sys/hm_signal.h>

#include <lib/dlist.h>
#include <lib/sdlist.h>
#include <intapi/thread/thread_ops.h>
#include <intapi/signal/sigset.h>
#include <hmkernel/ipc/signal.h>
#include <hmkernel/ipc/hmsignal.h>
#include <libstrict/strict.h>
#include <libhmsync/bitops/generic.h>
#include <libhwsecurec/securec.h>
#include <sec/pac.h>

struct process_s;
struct thread_s;

#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
union sigval_compat32 {
	int32_t sival_int;
	uint32_t sival_ptr;
};

#define __SI_PAD_COMPAT32_SIZE (128 - 3 * sizeof(int))
struct siginfo_compat32 {
	int32_t si_signo;
	int32_t si_errno;
	int32_t si_code;
	union {
		char __pad[__SI_PAD_COMPAT32_SIZE];
		struct {
			pid_t _si_pid;
			uid_t _si_uid;
			union {
				union sigval_compat32 _si_value;
				int32_t _si_status;
			};
		};
		uint32_t _si_addr;
	};
};
#endif

static inline bool
is_st_signum(unsigned int signum)
{
	return (signum > 0U) && (signum <= SIGNAL_MAX_STANDARD);
}

static inline bool
is_rt_signum(unsigned int signum)
{
	return (signum >= SIGNAL_MIN_RT) && (signum <= SIGNAL_MAX_RT);
}

/* signum is 1-64, that __SIGIDX_OF(signum) can be used by arrays */
static inline bool
is_valid_signum(unsigned int signum)
{
	return (signum > 0U) && (signum <= SIGNAL_MAX_RT);
}

struct rt_siginfo_s {
	struct sdlist_node node;
	struct __siginfo siginfo;
	bool pre_alloc;
};

/*
 * Caller must ensure is_rt_signum(siginfo->signum)
 */
static inline void
rt_siginfo_init(struct rt_siginfo_s *rt_siginfo, const struct __siginfo *siginfo)
{
	mem_assign_s(rt_siginfo->siginfo, *siginfo);
	sdlist_node_init(&(rt_siginfo->node), (int)siginfo->signum);
}

static inline struct rt_siginfo_s *
rtsiginfo_of_dnode(struct dlist_node *dnode)
{
	struct sdlist_node *sdnode = container_of(dnode, struct sdlist_node, dnode);
	return container_of(sdnode, struct rt_siginfo_s, node);
}

static inline struct dlist_node *
rtsiginfo_dnode_of(struct rt_siginfo_s *rt_siginfo)
{
	return &(rt_siginfo->node.dnode);
}

/*
 * pending_siginfo_set is a set that includes the siginfo
 * of pending standard and real time signals.
 *
 * The pending signals in struct thread_s will be unified besed on
 * pending_siginfo_set and the related functions will be refactored in the future.
 */
struct pending_siginfo_set {
	uint64_t sigset_pending;
	struct __siginfo siginfo[SIGNAL_NR_STANDARD];
	struct sdlist_node rt_pending_list;
};

static inline void
pending_siginfo_set_init(struct pending_siginfo_set *set)
{
	set->sigset_pending = SIGSET_EMPTY;
	mem_zero_a(set->siginfo);
	sdlist_init(&(set->rt_pending_list));
}

/*
 * If the signum is a st signal, siginfo cannot be empty, and
 * if the signum is a rt signal, link cannot be empty.
 */
static inline void
pending_siginfo_set_insert(struct pending_siginfo_set *set, unsigned int signum,
			   struct sdlist_node *node, struct __siginfo *siginfo)
{
	set->sigset_pending = sigset_set(set->sigset_pending, signum);
	if (is_st_signum(signum)) {
		BUG_ON(siginfo == NULL);
		mem_assign_s(set->siginfo[__SIGIDX_OF(signum)], *siginfo);
	} else if (is_rt_signum(signum)) {
		BUG_ON(node == NULL);
		sdlist_insert(&(set->rt_pending_list), node);
	} else {
		/* No action required */
	}
}

struct process_signal_pool {
	struct rt_siginfo_s *head;
	struct dlist_node freelist;
};

struct int_sigaction {
	unsigned long handler;
	unsigned long flags;
	unsigned long restorer;
	uint64_t mask;
};

struct process_signal_s {
	/* Signal processing entry function address */
	__sec_data(sysmgr, usr_ptr) unsigned long handler_entry;
	struct pending_siginfo_set shared_pending;
	struct process_signal_pool pool;
	unsigned int special_signal;
	/* True if first thread waked up in the same group */
	bool is_resumed;
	struct int_sigaction actions[_NSIG - 1U];
};

#define THREAD_SIGNAL_STATE_CONTINUE  0
#define THREAD_SIGNAL_STATE_STOPPING  1
#define THREAD_SIGNAL_STATE_STOPPED   2
#define THREAD_SIGNAL_STATE_SUSPENDED 3

struct thread_signal_s {
	uint64_t sigmask;
	/* Used for saving signal mask in rt_sigsuspend */
	uint64_t saved_sigmask;
	/* The preserved signal mask during sigtimedwait */
	uint64_t preserved_mask;
	/* Used for saving prev sigmask when handling SIGSTOP */
	uint64_t backup_mask;
	struct pending_siginfo_set pending;
	/* signal alternative stack */
	unsigned long sas_sp;
	unsigned long sas_size;
	unsigned int sas_flags;
	/* Used for saving the input sigset of sigtimedwait */
	uint64_t wait_sigset;
	int wait_lock;
	/* Record state signaled by SIGSTOP/SIGCONT */
	int state;

	/* Backup hmsignal mask in vsyscall context for silk */
	struct __hmsigmask bak_hmsigmask;
	bool in_vsyscall_ctx;

	bool need_restore_sigmask;
	/* Whether the HMSIGNAL-interrupted syscall is restartable */
	unsigned int restartable_flags;
	/* Used for appending hmsignals within one upcall */
	unsigned long long pending_hmsignals;
};

int signalmgr_kill_process(struct process_s *target, unsigned int signum,
			   struct process_s *source, const struct __siginfo *siginfo);
int signalmgr_kill_process_nolock(struct process_s *target,
				  struct thread_s *target_thread,
				  unsigned int signum, struct process_s *source,
				  const struct __siginfo *siginfo);
int signalmgr_force_kill_process(struct process_s *target, unsigned int signum,
				 struct process_s *source, const struct __siginfo *siginfo);
int signalmgr_kill_pid(int pid, unsigned int signum, struct process_s *src_process,
		       struct thread_s *real_sender, const struct __siginfo *siginfo);
int signalmgr_kill_thread(struct thread_s *target, unsigned int signum,
			  struct process_s *source, const struct __siginfo *siginfo);
int signalmgr_force_kill_thread(struct thread_s *target, unsigned int signum,
				struct process_s *source, const struct __siginfo *siginfo);
int signalmgr_kill_tid(int tid, unsigned int signum, struct process_s *src_process,
		       struct thread_s *real_sender, const struct __siginfo *siginfo);
int signalmgr_tgkill(int tgid, int tid, unsigned int signum,
		     struct process_s *src_process, struct thread_s *real_sender);
int signalmgr_sigqueue(int tgid, unsigned int signum, struct process_s *src_process,
		       struct thread_s *real_sender, const struct __siginfo *siginfo);
int signalmgr_pthread_sigqueue(struct thread_s *target, unsigned int signum,
			       struct process_s *src_process,
			       struct thread_s *real_sender,
			       const struct __siginfo *siginfo);
#ifdef CONFIG_SIGNALFD_SUPPORT
ssize_t signalmgr_read_signalfd(struct thread_s *thread, __u64 sigmask,
				unsigned int flags, void *buf, size_t buf_size);
int signalmgr_read_signalevents(struct thread_s *thread, __u64 sigmask, __u32 *revents);
int signalmgr_epoll_ctrl(struct process_s *process, int sigfd, int op, __u64 sigmask);
#endif

/*
 * Caller should hold process->signal_lock before calling these 3 functions.
 */
int __signalmgr_update_sigpending(struct thread_s *thread, bool has_thread_alive);
int __signalmgr_check_signal_handler(struct process_s *target,
				     bool same_origin,
				     unsigned int signum);
int __signalmgr_dequeue_specific(struct thread_s *t, unsigned int signum, struct __siginfo *info);

int signalmgr_drop_pending_signal(struct thread_s *thread, uint64_t sigmask,
				  struct __siginfo *out);
void signalmgr_resume_thread(struct thread_s *thread);
int signalmgr_timedwait(struct thread_s *thread, uint64_t sigset,
			struct __siginfo *out, const struct timespec *timeout);

int signalmgr_get_print_fatal_signals(void);
void signalmgr_set_print_fatal_signals(int val);
int signalmgr_do_sigprocmask(struct thread_s *t, int how, uint64_t nset, uint64_t *oset);
int signalmgr_do_rt_sigprocmask(struct thread_s *thread, int how, void *nset,
			     void *oset, unsigned long size);
int signalmgr_do_rt_sigreturn(unsigned long credential, void *frame);

#ifdef CONFIG_SIGNALFD_SUPPORT
void __signalfd_notify(struct thread_s *thread, unsigned int signum);
#else
static inline void __signalfd_notify(const struct thread_s *thread, unsigned int signum)
{
	UNUSED(thread, signum);
}
#endif

static inline bool __thread_is_on_sigaltstack(const struct thread_signal_s *t, unsigned long sp)
{
	return ((t->sas_flags & SIGSTACK_FLAG_SS_AUTODISARM) == 0 &&
		sp > t->sas_sp && sp - t->sas_sp <= t->sas_size);
}

#if (__SIZEOF_POINTER__ == 8u)
#define STACK_ALIGN_BYTE	16UL
#else
#define STACK_ALIGN_BYTE	8UL
#endif

struct thread_sig_ops_s {
	bool (*select_proxy_thread_bypass)(void);
	bool (*select_sigtarget_thread_bypass)(const struct thread_s *thread);
	int (*deliver_hmsignal)(struct thread_s *thread, bool has_thread_alive, int hmsignum);
	void (*suspend_thread)(struct thread_s *thread);
	int (*try_wake_futex)(struct thread_s *thread);
	bool (*is_thread_active)(const struct thread_s *t, rref_t cnode);
};

extern const thread_sig_ops_t klt_thread_sig_ops;
bool signalmgr_ops_select_proxy_thread_bypass(const struct thread_s *thread);
bool signalmgr_ops_select_sigtarget_thread_bypass(const struct thread_s *thread);
int signalmgr_ops_deliver_hmsignal(struct thread_s *thread, bool has_thread_alive, int hmsignum);
bool signalmgr_ops_is_thread_active(const struct thread_s *thread, rref_t cnode);

typedef int (*deliver_func)(struct thread_s *thread, rref_t cnode_rref, cref_t target_cref,
	bool has_thread_alive, int hmsignum);
int __signalmgr_deliver_hmsignal(struct thread_s *thread, cref_t tcb_cref, bool has_thread_alive, int hmsignum,
				 deliver_func deliver_hmsignal);

typedef struct signal_select_info_s {
	const struct process_s *process;
	rref_t cnode;
	unsigned long userdata;
} signal_select_info_t;

struct thread_s *signalmgr_thread_select(
	signal_select_info_t *info,
	bool (*bypass)(const struct thread_s *t),
	bool (*pred)(const struct thread_s *t,
	unsigned long userdata),
	bool (*post)(const struct thread_s *t, rref_t cnode));
int signalmgr_store_siginfo(struct process_s *p, struct __siginfo *info, siginfo_t *uaddr);
int signalmgr_load_sigset(struct process_s *p, sigset_t *uaddr, uint64_t *sigset);

#endif
