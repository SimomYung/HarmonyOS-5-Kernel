/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of operation on thread in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 4 10:24:35 2019
 */

#ifndef H_SYSMGR_INCLUDE_THREAD_THREAD_H
#define H_SYSMGR_INCLUDE_THREAD_THREAD_H

#include <unistd.h>

#include <intapi/thread/thread_ops.h>
#include <sec/security.h>
#include <activation/actv_pool.h>
#include <lib/sdlist.h>
#include <libalgo/rbtree.h>
#include <libhmsync/thread.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_mutex_guard.h>
#include <hmkernel/capability.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmsrv_sys/hm_signal.h>
#include <intapi/signal/sigset.h>
#include <intapi/signal/signal.h>
#include <intapi/process/pid.h>
#include <intapi/process/debug.h>
#include <intapi/process/rusage.h>
#include <intapi/nsmgr/ns_pid.h>
#include <libhmsync/waitqueue.h>
#ifdef CONFIG_SYSMGR_PERF
#include <intapi/inspector/perf/perf.h>
#endif
#include <intapi/mem/fault.h>
#include <intapi/mem/vspace.h>
#include <intapi/mem/copgtable.h>
#ifdef CONFIG_ULWT
#include <intapi/ulwt/ulwt.h>
#endif
#ifdef CONFIG_RESMGR
#include <intapi/res/res_info.h>
#endif
#ifdef CONFIG_SECCOMP
#include <sec/seccomp.h>
#endif
#ifdef CONFIG_THREAD_FINE_LOAD
#include <intapi/thread/thread_qos.h>
#endif
#ifdef CONFIG_XACCT
#include <intapi/inspector/xacct/xacct.h>
#endif

#include <hmasm/registers.h>
#include <sec/pac.h>

#include "robust_futex.h"

struct process_s;
struct ffrt_thd;

#define BIND_ENTRY_REF_CNT 2

/* threads bind for sysmgr */
struct actvpool_info;
struct bind_entry {
	rref_t rref[BIND_ENTRY_REF_CNT];
	uref_t uref[BIND_ENTRY_REF_CNT];
	struct actvpool_info *info;
};

struct thread_bind {
	unsigned int    nr_binds;
	struct bind_entry *binds;
};

/*
 * special flags for thread clone
 */
#define CLONE_MAIN_THREAD	0x01U
#define CLONE_FIXED_TID		0x02U
#define CLONE_FROM_PROCESS	0x04U
struct thread_attr {
	const char *name;
	/*
	 * this flag is different from hm_flags in hm_clone_req,
	 * it is used in sysmgr, and hm_flags come from ulibs,
	 * when we support execution separated from tcb, we will
	 * remove hm_flags.
	 */
	__u32 flags;
	pid_t fixed_pid;
	cref_t vspace_cref;

	/*
	 * we support three ways to create threads:
	 * 1. create from base actv and create a thread for
	 *    the base actv.
	 * 2. create from non-base actv and create a thread
	 *    for the non-base actv.
	 * 3. create from selfcall, don't care the base actv,
	 *    just create a thread for sysmgr.
	 *
	 * in the first case, caller is same with parent,
	 * and caller is different with the parent in other cases.
	 *
	 * in the future we will use parent to clone child for kernel,
	 * and parent is always vaild, caller is not important in 2&3 cases.
	 */
	struct thread_s *caller;
	struct thread_s *parent;
	/* new thread belongs to */
	struct process_s *process;
	struct hm_clone_req req;
	uintptr_t vfork_done;
	bool async_recycle;
#ifdef CONFIG_PIDFD_SUPPORT
	/* record the pidfd at this clone progress, used at rollback of clone failed */
	int pidfd;
#endif
};

/*
 * Posix-timers linked to this thread.
 * When thread destroy, the linked timers must be all canceled
 * to decrease cap_thread refcnt to 1.
 */
struct thread_posix_timer {
	struct raw_mutex mutex;
	struct dlist_node head;
};

/*
 * struct wait_op - wait return value structure
 * @options:	flag from waitpid/waitid posix interface
 * @ret:	points to activation return value
 * @wait_pid:	pid of wait process
 * @wait_pgid:	pgid of wait process
 */
struct wait_op {
	unsigned int options;
	int *ret;
	int *wstatus;
	union {
		pid_t wait_pid;
		pid_t wait_pgid;
	};
	struct rusage_compat *rusage;
	siginfo_t *siginfo;
};

struct process_children {
	struct raw_mutex mutex;
	struct dlist_node children_head;
};

#ifdef CONFIG_SIGNALFD_SUPPORT
struct thread_signalfd {
	/* Reading signalfd will wait on futex until signal available */
	int signalfd_futex;
	/* the set of signals accepted via signalfd */
	__u64 signalfd_mask;
};
#endif

#define THREAD_KILLED			0x0001U
#define THREAD_FREEZING			0x0002U
#define THREAD_FROZEN			0x0004U
#define THREAD_FROZEN_FAILED		0x0008U
#define THREAD_RESET_ON_FORK		0x0010U
#define THREAD_IN_THREAD_GROUP		0x0020U
#define THREAD_IN_THREAD_POOL		0x0040U
#define THREAD_G1_BINDED		0x0080U
#define THREAD_G2_BINDED		0x0100U
#define THREAD_RESMGR_PRE_ATTACHED	0x0200U
#define THREAD_IN_RESMGR		0x0400U
#define THREAD_CNTER_INCREASED		0x0800U
#define THREAD_IN_COREDUMP		0x1000U
#define THREAD_VFORK_PUT_RESOURCE	0x2000U
#define THREAD_TRIGER_DETACH		0x4000U
#define THREAD_BIND_PRE_ACTV		0x8000U
#define THREAD_SYSCALL_TRACE		0x10000U /* report syscall_{enter,exit} */
#define THREAD_TIME_STATISTICED		0x20000U
#define THREAD_HMSIG_KILL_HANDLED	0x40000U
#define THREAD_HMSIG_CRED_SYNC		0x80000U
#define THREAD_INVALID_MAIN_THREAD	0x100000U
#define THREAD_SYS_FREEZABLE		0x200000U
#ifdef CONFIG_RESMGR_PIDS_FORK_CONTROL
#define THREAD_MAIN_THREAD		0x400000U
#define THREAD_LAST_THREAD		0x800000U
#endif
#define THREAD_MEMALLOC			0x1000000U
#define THREAD_NEED_CPU_ATTACH		0x2000000U

#ifdef CONFIG_FREEZER_USER_HANDLER
#define THREAD_IN_USERHANDLER		0x4000000U /* thread is in user handler */
#define THREAD_REIN_USERHANDLER		0x8000000U /* thread reined user handler */
#endif

#define THREAD_PDEBUG_HOLD_REFCNT	0x10000000U
#define THREAD_PANIC_TRAPPED		0x20000000U

#define THREAD_GET_BY_BINDER		0x40000000U

#define THREAD_FREEZE_SKIP		0x80000000U
#define THREAD_DETACHED_DUE_TO_EXIT	0x100000000ULL
#define THREAD_EXCEPTION_DUMPED		0x200000000ULL

/* flags in thread->frozen_reason */
#define FROZEN_NONE		0x0U
#define FROZEN_POWER		0x1U
#define FROZEN_RESMGR		0x2U
#define FROZEN_USER		0x4U
#define FROZEN_REASONS_MASK	0xFU
#define FROZEN_FLAG_SET		0x1000U /* represent thread->flags has THREAD_FROZEN set */
#define FROZEN_MEM_ALIVE	0x2000U /* represent thread not oom victim */

#define PF_EXT_WSCG_MONITOR	0x1U

struct thread_core_ops_s {
	const char *(*name)(void);
	int (*destroy_cap)(struct thread_s *thread);
	int (*terminate)(struct thread_s *thread);
	int (*fork)(struct thread_s *thread, struct process_s *process, const struct sysarg_thread_config *config);
	unsigned long (*read_timerslack)(struct thread_s *thread);
	int (*set_timerslack)(struct thread_s *thread, unsigned long current_ns);
	int (*threadpool_insert)(struct thread_s *thread);
	int (*fork_bind_thread)(struct thread_s *new, struct thread_s *parent, struct process_s *process,
				rref_t child_cnode_rref, const struct thread_attr *attr);
	int (*clone_failed)(struct thread_s *thread);
	int (*state_of)(const struct thread_s *thread, unsigned long stat_flags, struct sysarg_thread_stat *stat,
			bool background);
	int (*schedattr_of)(const struct thread_s *thread, struct sysarg_thread_stat *stat);
	int (*time_statistics)(const struct thread_s *thread);
	int (*clear_tid_address)(struct thread_s *thread);
	struct thread_s *(*get_curr_thread)(struct thread_s *thread);
	void (*put_curr_thread)(struct thread_s *thread);
	int (*freeze)(struct thread_s *thread, unsigned int frozen_reason);
	int (*suspend)(const struct thread_s *thread, const unsigned int *addr, unsigned int val);
	int (*resume)(const struct thread_s *thread);
	int (*set_name)(struct process_s *process, struct thread_s *thread, const char *name, bool set_tcb_name);
	bool (*can_be_parent)(struct thread_s *thread);
	int (*write_cred_miscdata)(struct thread_s *new, struct process_s *process);
	bool (*is_current_thread)(struct thread_s *thread);
	cref_t (*curr_tcb_cref_of)(struct thread_s *thread);
	void (*set_pre_actv_local_creds)(struct thread_s *thread);
	void (*update_vec_idx)(struct thread_s *thread);
	void (*write_trace_thread_info)(struct thread_s *thread);
	void (*update_nr_threads)(struct process_s *process, struct thread_s *thread, bool op);
};

struct thread_s *threadmgr_ops_get_curr_thread(struct thread_s *thread);
int threadmgr_ops_terminate(struct thread_s *thread);
void threadmgr_ops_put_curr_thread(struct thread_s *thread);
const char *threadmgr_ops_name_of(struct thread_s *thread);
int threadmgr_ops_freeze(struct thread_s *thread, unsigned int frozen_reason);
int threadmgr_ops_write_cred_miscdata(struct thread_s *new, struct process_s *proc);
bool threadmgr_ops_is_current_thread(struct thread_s *thread);
cref_t threadmgr_ops_curr_tcb_cref_of(struct thread_s *thread);
void threadmgr_ops_set_pre_actv_local_creds(struct thread_s *thread);
int threadmgr_ops_state_of(const struct thread_s *thread, unsigned long stat_flags,
	struct sysarg_thread_stat *stat, bool background);
int threadmgr_ops_time_statistics(struct thread_s *thread);
int threadmgr_ops_set_timerslack(struct thread_s *thread, unsigned long current_ns);
unsigned long threadmgr_ops_read_timerslack(struct thread_s *thread);
void threadmgr_ops_write_trace_thread_info(struct thread_s *thread);
/* op : true is add , false is substrct */
void threadmgr_ops_update_nr_threads(struct thread_s *thread, bool op);
/*
 * Thread & Process relationship:
 * 1. When a process created, set its refcnt to 1;
 * 2. When we create a new thread for the process, will increase the process refcnt;
 * 3. When a thread exit, will decrease the process refcnt:
 *    - actually decrease the process refcnt after thread released(in thread_delete)
 * 4. When process exit, will terminate all its threads, when thread refcnt decrease to
 *    zero, will free thread, and decrease the process refcnt.
 *
 * So we can promise that:
 *  1. the lifecycle of the process is longer than that of all its threads.
 *  2. we can use process pointer in thread while in the whole thread lifecycle.
 *
 * Thread refcnt:
 * 1. When a thread created, set its refcnt to 1;
 * 2. When a thread exit, decrease the thread refcnt;
 * 3. When created a raw_thread, increase the thread refcnt and
 *    decrease the thread refcnt when join the raw thread.
 */
struct perf_event_context;
#ifdef CONFIG_SCHED_RT_PRIO_EXTEND
struct ex_rtprio_thread_s {
	int ex_rtprio;
	int usr_rtprio;
};
#endif

int thread_ex_rtprio_of(const struct thread_s *thread);
int thread_usr_rtprio_of(const struct thread_s *thread);
void thread_set_ex_rtprio(struct thread_s *thread, int ex_rtprio);
void thread_set_usr_rtprio(struct thread_s *thread, int usr_rtprio);

struct thread_s {
	const thread_ops_t *ops;
	/* protected by thread mutex */
	char name[HMCAP_THREAD_NAME_SIZE];

	/* thread flags, protected by thread->mutex */
	uint64_t flags;
	/* credential type */
	unsigned int cred_type;

	/* the signum which this thread get when its parent thread dead */
	int pdeath_signal;

	/* initialized when creating, read-only after created */
	struct tid_s *tid;
	__sec_data(sysmgr, obj_handler) cref_t tcb_cref;

	struct pmem_area_s *tcb_mem;

	struct actvpool_bind_thread bind;
	__sec_data(sysmgr, ptr) struct process_s *process;

	/* dlist of the process, protected by process->threads.mutex */
	struct dlist_node list;

	/* children process */
	struct process_children children;

	struct wait_queue_node *waiter;

	int *tid_addr;
	void *stack_vaddr;

	/* per-thread robust futex list head */
	struct thread_robust_list *robust_list;
#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
	struct compat_thread_robust_list *compat_robust_list;
#endif

	/*
	 * rbtree of tid and cref
	 * protected by threadpool lock
	 */
	struct rbt_node tid_node;
	struct rbt_node cref_node;

	struct raw_mutex mutex;
	struct raw_refcnt refcnt;

	/*
	 * cred lock of thread, to eusure get cred and update cred
	 * exclusively
	 */
	struct raw_rwlock cred_lock;
	struct raw_rwlock exec_lock;

	struct raw_mutex sched_mutex;
	struct {
		bool synced;
		unsigned int rq;
#ifdef CONFIG_MODULE_SCHED_CLASS
		/* userspace backup of kernel scheduling attr */
		int policy;
		int nice;
		int priority;
#endif
#ifdef CONFIG_THREAD_FINE_LOAD
		int sched_domain;
#endif
#ifdef CONFIG_CORE_SCHEDULING
		int smt_type;
#endif
	} sched_attr;

	struct page_fault_count_s page_fault_count;

#ifdef CONFIG_PDEBUG
	struct pdebug_obj_s pdebug_obj;
#endif

	struct thread_posix_timer posix_timers;
#ifdef CONFIG_SIGNALFD_SUPPORT
	struct thread_signalfd signalfd;
#endif

	/* Protected by process->signal_lock */
	struct thread_signal_s signal;

	__sec_data(sysmgr, ptr) struct vspace_s *vspace;

	/* thread bind actv server info */
	struct thread_bind binds;

	/* thread security context */
	struct sec_thread_s sec_thread;
#ifdef CONFIG_SECCOMP
	struct seccomp_s seccomp;
#endif
#ifdef CONFIG_NO_NEW_PRIVS
	bool no_new_privs;
#endif
#ifdef CONFIG_SECURITY_JITFORT
	bool use_copgtable;
#endif

	unsigned int personality;

#ifdef CONFIG_RESMGR
	/*
	 * Same as process->rgrp, we use process->rgrp_lock to protect @rgrp.
	 */
#ifdef CONFIG_RESMGR_COMPAT_V1
	struct res_group *rgrp_list[MAX_RES_COUNT];
	struct dlist_node rg_node_list[MAX_RES_COUNT];
#else
	struct res_group *rgrp;
	struct dlist_node rg_node;
#endif
#ifdef CONFIG_RESMGR_WORKINGSET
	unsigned int ext_flags;
#endif
	bool iter_procs;
#endif
#ifdef CONFIG_FREEZER
	unsigned int frozen_reason;
#endif
#ifdef CONFIG_RESMGR_THREADED_INHERIT_MAIN
    unsigned int resmgr_inherit_main;
#endif
	uint64_t seq;
	uint64_t start_time;

#ifdef CONFIG_SYSMGR_PERF
	struct perf_cpuctx_s perf_cpuctx;
#endif
#ifdef CONFIG_HMPERF_NG
	struct perf_event_context *perf_event_ctx;
#endif

#ifdef CONFIG_UPROBE
	struct uprobe_thread *cur_uprobe;
	uint32_t uprobe_depth;
#endif
	__sec_data(sysmgr, ptr) struct ns_group_s *ns_group;
#ifdef CONFIG_ULWT
	struct thread_ulwt_s ulwt;
#endif

#ifdef CONFIG_ACCESS_TOKEN
	__sec_data(sysmgr, cntx) uint64_t token;
	__sec_data(sysmgr, cntx) uint64_t ftoken;
#endif
	int *vfork_done;
#ifdef CONFIG_THREAD_FINE_LOAD
	bool in_ddl_grp;
	int thread_qos;
#endif
#ifdef CONFIG_KOBJ_THREAD
	struct ctrlmem_kobj_thread_wrapper_s *kobj_thread;
#endif
#ifdef CONFIG_THREAD_JOURNAL_INFO
	void *journal_info;
#endif
#ifdef CONFIG_SCHED_RT_PRIO_EXTEND
	struct ex_rtprio_thread_s ex_thread;
#endif
#ifdef CONFIG_SCHED_MISMATCHED_32BIT
	bool is_compat;
#endif
#ifdef CONFIG_SCHED_HIDE_REAL_PRIO
	bool hide_rt_policy;
#endif
	unsigned long local_creds_addr;
#ifdef CONFIG_XACCT
	struct xacct *xacct;
#endif
#ifdef CONFIG_BINDER_GET_THREAD
	uintptr_t binder_threads;
#endif

#ifdef CONFIG_HISI_FFRT_FFTS_SCHED
	struct ffrt_thd *ffrt_thd;
	int ffts_flag;
#endif

#ifdef CONFIG_HISI_GP_CTRL
	void *gp_info;
#endif
#ifdef CONFIG_KCOV
	void *kcov_private;
#endif
#ifdef CONFIG_SUPPORT_UAPP_HWASAN
	unsigned int tagged_flag;
#endif
#ifdef CONFIG_RESMGR_CPU
	cref_t scheduler_cref;
	unsigned int cpu_migrate_cnt;
#endif
};

static inline void thread_set_ops(struct thread_s *thread, const thread_ops_t *ops)
{
	thread->ops = ops;
}

static inline const thread_ops_t *thread_ops_of(const struct thread_s *thread)
{
	return thread->ops;
}

static inline uint64_t thread_token_of(const struct thread_s *thread)
{
#ifdef CONFIG_ACCESS_TOKEN
	return thread->token;
#else
	return 0;
#endif
}

#ifdef CONFIG_ULWT
static inline struct thread_ulwt_s *thread_ulwt_of(struct thread_s *thread)
{
	return &thread->ulwt;
}
#endif

static inline struct ns_group_s *thread_ns_group_of(const struct thread_s *thread)
{
	return thread->ns_group;
}

static inline struct pid_ns *thread_curr_active_pidns_of(const struct thread_s *thread)
{
	struct tid_s *tid = thread->tid;
	return tid->numbers[tid->level].ns;
}

static inline struct raw_refcnt *thread_refcnt_of(struct thread_s *thread)
{
	return &thread->refcnt;
}

static inline struct thread_s *thread_of_refcnt(struct raw_refcnt *refcnt)
{
	return container_of(refcnt, struct thread_s, refcnt);
}

static inline int thread_get(struct thread_s *thread)
{
	return raw_refcnt_get(&thread->refcnt);
}

static inline void thread_put(struct thread_s *thread)
{
	(void)raw_refcnt_put(&thread->refcnt);
}

/*
 * 1. thread_tid_of(): Get a global TID of thread in root pid ns.
 * 2. thread_vtid_of(): Get a virtual TID of thread in the ns where the thread is located.
 * 3. thread_tid_in_pidns(): Get a TID of thread in a specified ns.
 */
static inline pid_t thread_tid_of(struct thread_s *thread)
{
	RAW_MUTEX_GUARD(_, &thread->mutex);
	return tid_s_nr_of_level(thread->tid, root_pid_ns()->level);
}

static inline pid_t thread_vtid_of(struct thread_s *thread)
{
	RAW_MUTEX_GUARD(_, &thread->mutex);
	return tid_s_nr_of(thread->tid);
}

/* fetch tid without lock of thread_>mutex */
static inline pid_t thread_tid_of_nolock(struct thread_s *thread)
{
	return tid_s_nr_of_level(thread->tid, root_pid_ns()->level);
}

/* fetch vtid without lock of thread_>mutex */
static inline pid_t thread_vtid_of_nolock(struct thread_s *thread)
{
	return tid_s_nr_of(thread->tid);
}

/*
 * Do not lock thread mutex , so must make sure that the caller will not
 * be concurrent with execve, as exec may change tid of thread, which make
 * the tid pointer not safe.
 */
static inline struct tid_s *thread_tid_s_of(const struct thread_s *thread)
{
	return thread->tid;
}

static inline const char *thread_name_of(const struct thread_s *thread)
{
	return thread->name;
}

static inline cref_t thread_tcb_cref_of(const struct thread_s *thread)
{
	return thread->tcb_cref;
}

#ifdef CONFIG_KOBJ_THREAD
static inline struct ctrlmem_kobj_thread_wrapper_s *thread_kobj_of(const struct thread_s *thread)
{
	return thread->kobj_thread;
}
#else
static inline struct ctrlmem_kobj_thread_wrapper_s *thread_kobj_of(const struct thread_s *thread)
{
	UNUSED(thread);
	return NULL;
}
#endif

static inline struct actvpool_bind_thread *thread_bind_of(struct thread_s *thread)
{
	return &thread->bind;
}

static inline void *thread_stack_vaddr_of(struct thread_s *thread)
{
	return thread->stack_vaddr;
}

static inline void thread_set_stack(struct thread_s *thread, uintptr_t stack)
{
	/* only updated by execute operation, no competition. */
	thread->stack_vaddr = (void *)stack;
}

static inline void thread_set_sec_thread(struct thread_s *thread, struct sec_thread_s *sec_thread)
{
	thread->sec_thread.exec_ctx.type_id = sec_thread->exec_ctx.type_id;
}

static inline void thread_set_personality(struct thread_s *thread, unsigned int personality)
{
	thread->personality = personality;
}

static inline unsigned int thread_personality_of(struct thread_s *thread)
{
	return thread->personality;
}

static inline uint64_t thread_seq_of(struct thread_s *thread)
{
	return thread->seq;
}

static inline struct vspace_s *thread_vspace_of(struct thread_s *thread)
{
	return thread->vspace;
}

static inline void thread_set_and_get_vspace(struct thread_s *thread, struct vspace_s *vspace)
{
	int cnt;
	thread->vspace = vspace;
	cnt = vspace_get(vspace);
	BUG_ON(cnt <= 0);
}

static inline void thread_set_thread_flag(struct thread_s *thread, uint64_t flag)
{
	RAW_MUTEX_GUARD(_, &thread->mutex);
	thread->flags |= flag;
}

static inline void thread_clear_thread_flag(struct thread_s *thread, uint64_t flag)
{
	RAW_MUTEX_GUARD(_, &thread->mutex);
	thread->flags &= (~flag);
}

static inline bool thread_test_thread_flag(struct thread_s *thread, uint64_t flag)
{
	RAW_MUTEX_GUARD(_, &thread->mutex);
	return ((thread->flags & flag) != 0U);
}

static inline void thread_set_thread_flag_nolock(struct thread_s *thread, uint64_t flag)
{
	thread->flags |= flag;
}

static inline bool thread_test_thread_flag_nolock(struct thread_s *thread, uint64_t flag)
{
	return ((thread->flags & flag) != 0U);
}

struct thread_s *main_thread_of_sysmgr(void);
struct tid_s *thread_alloc_tid(const struct thread_s *thread, __u32 flags, pid_t fixed_pid);
void thread_attach_tid(struct thread_s *thread, struct tid_s *tid);
struct thread_s *thread_new(struct process_s *process);
int thread_set_tickless(struct thread_s *thread, bool tickless);
int thread_set_name_by_cref(cref_t tcb_cref, const char *name);
int thread_set_name(struct thread_s *thread, const char *name, bool set_tcb_name);
void thread_set_stack_addr(struct thread_s *thread, void *stack_vaddr);
int thread_terminate(struct thread_s *thread);
int thread_suspend(const struct thread_s *thread, const unsigned int *addr, unsigned int cond);
int thread_resume(const struct thread_s *thread);
void thread_triggerdetach(const struct thread_s *thread);
int thread_state_and_schedattr_of(const struct thread_s *thread, unsigned long stat_flags,
				  struct sysarg_thread_stat *stat);
void thread_try_set_process_exit_code(struct thread_s *thread, int exitcode);
bool thread_check_and_set_kill_flag(struct thread_s *thread);

#ifdef CONFIG_FREEZER
static inline bool thread_is_frozen_failed(struct thread_s *thread)
{
	bool rc = false;
	RAW_MUTEX_GUARD(_, &thread->mutex);
	if ((thread->flags & THREAD_FROZEN_FAILED) != 0U) {
		rc = true;
	}
	return rc;
}

int klt_freeze(struct thread_s *thread, unsigned int frozen_reason);
static inline int thread_freeze(struct thread_s *thread, unsigned int frozen_reason)
{
	return threadmgr_ops_freeze(thread, frozen_reason);
}

bool thread_is_freezing(struct thread_s *thread);
bool thread_is_frozen(struct thread_s *thread);
int thread_sync_status(struct thread_s *thread, const unsigned int frozen_reason);
int thread_thaw(struct thread_s *thread, unsigned int frozen_reason);
static inline void thread_set_freezable(struct thread_s *thread)
{
	RAW_MUTEX_GUARD(_, &thread->mutex);
	thread->flags |= THREAD_SYS_FREEZABLE;
}

int thread_freeze_self(struct thread_s *thread, unsigned int frozen_reason);
void thread_freezer_do_not_count(struct thread_s *thread);
void thread_freezer_count(struct thread_s *thread);
bool thread_freezer_should_skip(struct thread_s *thread);
#else
static inline bool thread_is_freezing(struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}

static inline bool thread_is_frozen(struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}

static inline bool thread_is_frozen_failed(struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}

static inline int klt_freeze(struct thread_s *thread, unsigned int frozen_reason)
{
	UNUSED(thread, frozen_reason);
	return E_HM_NOSYS;
}

static inline int thread_freeze(struct thread_s *thread, unsigned int frozen_reason)
{
	UNUSED(thread, frozen_reason);
	return E_HM_NOSYS;
}

static inline int thread_sync_status(const struct thread_s *thread, const unsigned int frozen_reason)
{
	UNUSED(thread, frozen_reason);
	return E_HM_NOSYS;
}

static inline int thread_thaw(struct thread_s *thread, unsigned int frozen_reason)
{
	UNUSED(thread, frozen_reason);
	return E_HM_NOSYS;
}

static inline int thread_set_freezable(struct thread_s *thread)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}

static inline int thread_freeze_self(struct thread_s *thread, unsigned int frozen_reason)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}

static inline void thread_freezer_do_not_count(struct thread_s *thread)
{
	UNUSED(thread);
}

static inline void thread_freezer_count(struct thread_s *thread)
{
	UNUSED(thread);
}

static inline bool thread_freezer_should_skip(struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}
#endif

pid_t thread_tid_in_pidns(struct thread_s *thread, struct pid_ns *ns);
struct thread_s *find_get_thread_by_tid_s(struct tid_s *tid);
void thread_delete_from_ref(struct raw_refcnt *refcnt);

void thread_set_tid_address(struct thread_s *thread, int *tid_addr);
void thread_clear_tid_address(struct thread_s *thread);


void thread_set_robust_list(struct thread_s *thread, struct thread_robust_list *list_head);
struct thread_robust_list *thread_get_robust_list(struct thread_s *thread);
#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
void thread_set_compat_robust_list(struct thread_s *thread,
				   struct compat_thread_robust_list *list_head);
struct compat_thread_robust_list *thread_get_compat_robust_list(struct thread_s *thread);
#endif

#ifdef CONFIG_NO_NEW_PRIVS
static inline bool thread_no_new_privs_of(const struct thread_s *thread)
{
	return thread->no_new_privs;
}

static inline void thread_set_no_new_privs(struct thread_s *thread)
{
	thread->no_new_privs = true;
}
#else
static inline bool thread_no_new_privs_of(__maybe_unused const struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}

static inline void thread_set_no_new_privs(__maybe_unused struct thread_s *thread)
{
	UNUSED(thread);
}
#endif

bool thread_is_main(struct thread_s *thread);

struct process_s *thread_process_of(const struct thread_s *thread);

#ifdef CONFIG_SECURITY_JITFORT
static inline bool thread_using_copgtable(const struct thread_s *thread)
{
	return thread->use_copgtable;
}
#else
static inline bool thread_using_copgtable(const struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}
#endif

struct thread_s *thread_of_cred_type(const unsigned int *type);
int thread_alloc_bind(struct thread_s *thread, unsigned int nr_binds);
void thread_free_bind(struct thread_s *thread);
struct bind_entry *thread_search_bind_info(struct thread_s *thread, const char *name);

struct thread_s *thread_alive_parent_get(struct process_s *proc, const struct thread_s *curr);
int thread_clone(struct thread_attr *attr, struct thread_s **child);

struct thread_s *main_thread_of_sysmgr(void);
void set_sysmgr_main_thread(struct thread_s *thread);

void thread_init_ops(struct thread_s *thread);
void thread_delete_klt_specific_res(struct thread_s *thread);
void thread_wait_for_vfork(struct thread_s *thread, int *vfork_done);
void thread_complete_vfork(struct thread_s *thread, bool is_exec);
int thread_kexec_load_files(struct vspace_s *vspace, struct softreboot_info files);
int threadmgr_sched_trans_if(unsigned long credential ,cref_t src_cref, cref_t dst_cref, bool restore);
int threadmgr_do_thread_exit(unsigned long credential, int exitcode);
pid_t threadmgr_sysif_cred_to_tid(unsigned long credential);
int threadmgr_do_thread_futex(unsigned long credential,
			   int *uaddr1, int futex_op,
			   int val1, unsigned long ts_or_val2,
			   int *uaddr2, int val3);
void thread_futex_cleanup(struct thread_s *thread);
#endif
