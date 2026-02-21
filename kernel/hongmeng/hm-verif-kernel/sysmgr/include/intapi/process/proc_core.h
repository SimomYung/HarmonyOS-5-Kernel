/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition for process core structure and api
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 13 17:20:47 2020
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROC_CORE_H
#define SYSMGR_INTAPI_PROCESS_PROC_CORE_H

#ifndef SYSMGR_INTAPI_PROCESS_PROCESS_H
# error "Not for directly included by C file. Include intapi/process/process.h instead."
#endif

#include <libalgo/id_generator.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_mqueue.h>
#include <libhmsrv_sys/hm_ipc_sem.h>
#include <libhmucap/kobjpool.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_counter.h>
#include <libmem/slab_utils.h>
#include <lib/dlist.h>
#include <lib/sdlist.h>
#include <libhmelf/elf.h>
#include <limits.h>
#include <libsysif/fs/api.h>

#include <intapi/ctrlmem/kobjs/process.h>
#include <intapi/inspector/die_catch/die_catch.h>
#include <intapi/inspector/hmtrace/uprobe_process.h>
#include <intapi/process/proc_type.h>
#include <intapi/process/procpool.h>
#include <intapi/process/pid.h>
#include <intapi/process/process_rlimit.h>
#include <intapi/process/namespace.h>
#include <intapi/process/debug.h>
#include <intapi/process/symbol.h>
#include <intapi/process/binfmt.h>
#include <intapi/process/user.h>
#include <intapi/process/rusage.h>
#include <intapi/thread/threadpool.h>
#include <intapi/activation/actv_binder.h>
#include <intapi/mem/pmem_area.h>
#include <intapi/mem/extable.h>
#include <intapi/mem/vspace.h>
#include <intapi/mem/mcache.h>
#include <intapi/signal/signal.h>
#include <sec/security.h>
#include <sec/pac.h>
#include <hmasm/param.h>
#include <hmasm/privilege_layout.h>
#include <libhmsync/waitqueue.h>
#ifdef CONFIG_SYSMGR_PERF
#include <intapi/inspector/perf/perf.h>
#endif
#ifdef CONFIG_HMPSF
#include <intapi/hmpsf/hmpsf_proc.h>
#endif
#include <intapi/hmprobe/hmprobe_proc.h>

#ifdef CONFIG_ULWT
#include <intapi/ulwt/ulwt.h>
#endif

#ifdef CONFIG_PROCFSMGR_FFTS_SCHED
#include "intapi/process/proc_ffts_sched.h"
#endif

DECLARE_MCACHE(process)
DECLARE_MCACHE(process_signal)

/* see WCOREDUMP define */
static inline int coredump_exit_code(unsigned int signum)
{
	unsigned int code = 0x80U | (signum & 0x7fU);

	return (int)code;
}

#define nsec_to_jiffies(t)	\
	(((t) + NSEC_PER_SEC / __UL(HZ) - 1UL) / (NSEC_PER_SEC / __UL(HZ)))

#define INVALID_CNODE_RREF			0xFFFFFFFFFFFFFFFF
/* RLIMIT_NPROC exceeded in setuid */
#define PROCESS_NPROC_EXCEEDED				0x1
#define PROCESS_MEM_REAPING				0x2
#define PROCESS_VFORK_USE_PARENT_VS			0x4
#define PROCESS_FORK_NOTICE_FS				0x8
#ifdef CONFIG_RESMGR_PIDS_FORK_CONTROL
#define PROCESS_FORK_FROM_DFX				0x40000000
#define PROCESS_FORK_FROM_WEBDFX			0x80000000
#endif

#define ADDR_SIZE_MODEL_32			4
#define ADDR_SIZE_MODEL_64			8
#define CLD_PROC_ACTIVE				(1u)
#define CLD_PROC_STOP				(2u)
#define CLD_PROC_CONT				(4u)
#define CLD_PROC_ZOMBIE				(8u)
#define cld_proc_consume(s, state)		((s) | ((state) << 16))
#define cld_proc_consumed(s, state)		((s) & ((state) << 16))

#define CLD_PROC_STOP_CODE			((SIGSTOP << 8) | 0x7f)
#define CLD_PROC_CONT_CODE			(0xffff)

#define IS_CHILD_SUBREAPER			(1U)
#define HAS_CHILD_SUBREAPER			(2U)
#define THREAD_GROUP_EXIT			0x0001U
#define THREAD_GROUP_EXEC			0x0002U

#define PROCESS_AUXV_SECUREEXEC			0x0001U

#define DEFAULT_CSIGNAL SIGCHLD
/*
 * process state transition:
 *
 *                          ACTIVE
 *                             |
 *                             | (exit)
 *                             |
 *                          ZOMBIE
 *                             |
 *                             | (be waited)
 *                     ________|
 *                    |        |
 *                    |        V
 *                    |       GTD
 *    (group leader or|        |
 *    session leader) |        | (removed from the procpool)
 *                    |        V
 *                    |----->UNUSED
 *
 */
enum process_state {
	PROCESS_UNUSED,
	PROCESS_CREATING,
	PROCESS_ACTIVE,
	PROCESS_ZOMBIE,
	PROCESS_GTD,
	PROCESS_BLOCKED,
};

enum compat_model {
	NON_COMPAT,
	AARCH64_AARCH32,
	AARCH64_ILP32,
};

struct executable {
	char *name;
	uintptr_t entry;
};

struct process_reaper {
	struct thread_s *link_thread;
	struct dlist_node node;
};

struct process_threads {
	int nr_threads;
	int exit_status;
	__u64 exit_tock;
	unsigned int flags;
	struct raw_mutex mutex;
	struct dlist_node head;
};

struct process_siblings {
	struct raw_mutex mutex;
	struct thread_s *parent;
	struct dlist_node node;
	unsigned int cld_state;
};

struct pid_link_s {
	struct pid_s *pid[PIDTYPE_MAX];
	struct dlist_node pid_links[PIDTYPE_MAX];
	struct raw_mutex mutex;
};

struct process_auxv {
	uint16_t phdr_size;
	uint16_t phdr_num;
	uint16_t flags;
	uintptr_t phdr_addr;
	uintptr_t base_addr;
	uintptr_t entry_addr;
	uintptr_t vdso_base;
};

struct process_coredump_attr {
	uint64_t force;
	uint64_t stacksize;
	uint32_t filter;
	uint8_t  filter_type;
	/* Protect the variables above */
	struct raw_mutex mutex;

#ifdef CONFIG_COREDUMP_MEMORY_KDUMP
	bool skip_dump_vr_content;
#endif

	struct thread_s **threads;
	uint32_t nr_threads;
	/* the value checked by futex_wait */
	int suspend_flag;
	/* the number of threads in futex_wait state */
	raw_atomic_int_t suspended_threads_num;
	/* Set if the process_coredump is deferred */
	raw_atomic_int_t deferred_flag;
	raw_atomic_int_t dumping_flag;
	raw_atomic_int_t on_exit;

	struct {
		struct process_s *target_process;
		int wait_flag;
	} pipe_wait;

#ifdef CONFIG_COREDUMP_DEVHOST_ACTV
	/* futex_wait for the exception actv of devhost */
	int exception_actv_futex;
#endif
};

struct process_hangtask_attr {
	unsigned int detect_hang_of_ready_task;
	unsigned int detect_hang_of_lock_task;
};

struct process_uapp_pmems {
	struct raw_mutex mutex;
	struct rbt_tree tree;
};

struct process_table_pmems {
	struct raw_mutex mutex;
	struct dlist_node head;
};

struct itimer {
	cref_t cref;
	struct pmem_area_s *mem;
};

struct fdtable_s {
	struct kobj_self_mng obj;
};

struct posix_timer_s {
	cref_t cref;
	int id;
	struct __siginfo siginfo;
	cref_t target_cref;
	int to_caller;
	struct dlist_node p_node;
	struct dlist_node t_node;
	struct raw_mutex mutex;
#ifdef CONFIG_ULWT
	int target_ulwt_tid;
#endif
};

struct process_posix_timer {
	int nr_timers;
	struct raw_mutex mutex;
	struct dlist_node head;
};

struct process_mqueue {
	int opened_mq_num;
	int cur_mqd;
	/* record the mqdes of opened mqueues */
	DEFINE_RAW_BITMAP(mqueue_opened_bitmap, __UL(MQUEUE_MAX_OPEN_NUM));
	struct raw_mutex lock;
};

struct process_scheduler_s {
	struct dlist_node node;
	rref_t scheduler_rref;
	uref_t scheduler_uref;
	struct raw_refcnt refcnt;
};

struct process_sem {
	struct sem_undo_list *undo_list;
};

#ifdef CONFIG_HYPERVISOR
#ifdef CONFIG_VMEXIT_WITH_THREADS
struct process_actvpool_vmexit_s {
	cref_t pool_cref;
	struct pmem_area_s *actvpool_pmem;
};
#endif

struct process_vm_s {
	struct process_s *process;
	struct vspace_s *vm_vspace;
	struct dlist_node vcpuctxs;
	struct raw_mutex mutex;
	struct raw_refcnt refcnt;
	cref_t vm_cref;
	struct pmem_area_s *vm_pmem;
#ifdef CONFIG_VMEXIT_WITH_THREADS
	struct process_actvpool_vmexit_s actvpool_vmexit;
#endif
#ifdef CONFIG_SYSMGR_PERF
	struct vperfctx_s *vm_vperfctx;
#endif
#ifdef CONFIG_VM_SMMU
	uint64_t pgd_smmu;
#endif
};

static inline struct process_s *vm_process_of(struct process_vm_s *vm)
{
	BUG_ON(vm->process == NULL);
	return vm->process;
}
#endif /* end of CONFIG_HYPERVISOR */

struct fids {
	struct raw_mutex mutex;
	struct rbt_tree fid_tree;
};

struct process_resource {
	struct raw_refcnt_nr refcnt_nr;
	__sec_data(sysmgr, obj_handler) rref_t cnode_rref;
	__sec_data(sysmgr, ptr) struct vspace_s *vspace;
	struct fids fids;
#ifdef CONFIG_HYPERVISOR
	struct process_vm_s *vm;
#endif
#ifdef CONFIG_VSMMU
	void *vsmmu;
#endif
	struct raw_work work;
	struct process_mqueue mq_resource;
#ifdef CONFIG_ACTV_BINDER
	/* Manage the resources allocated for the actv binder */
	struct process_actv_binder_s actv_binder;
#endif
	/*
	 * schedulers resource
	 * Process's child threads may run in different schedulers,
	 * and shedulers' ref are recorded
	 */
	struct dlist_node schedulers_list;
	struct process_sem sem_resource;
	struct fdtable_s fdtable;
};

struct fid_holder {
	unsigned long fid;
	rref_t rref;
	struct raw_refcnt_nr refcnt;
	struct rbt_node node;
};

struct process_loader {
	struct executable target;
	char *interp_name;
	enum process_model model;
	struct process_auxv auxv;
	struct elf_generic_header elf_header;
	struct loader_file_cred loader_cred;
	bool is_pie;
	size_t load_align;
	struct rlimit rlimit_stack;
#ifdef CONFIG_SCHED_MISMATCHED_32BIT
	bool is_arm32_compatible;
#endif
	struct process_s *process;
	/*
	 * Those APPs only use lsyscall don't need g1 and g2 bind,
	 * use this to distinguish them.
	 */
	bool native_apps;
};

struct epoll_signalfd_ctx {
	struct dlist_node list;
	int sigfd;
	int count;
	__u64 sigmask;
};

struct process_signalfd_ctxs {
	struct dlist_node head;
	int fd_cnt; /* total fd count in dlist */
};

struct process_cnode_ref {
	rref_t cnode_rref;
	struct process_resource *resource;
};

struct process_vspace_ref {
	struct vspace_s *vspace;
	struct process_resource *resource;
};

struct cnode_spawn_pmems {
	struct pmem_area_s *cnode;
	struct pmem_area_s *ctable;
	struct pmem_area_s *utable;
	struct pmem_area_s *rtable;
};

/*
 * struct wait_info - wait info structure
 * @pid:	wait pid
 * @ret:	return value
 * @options:	flag from waitpid/waitid posix interface
 * @rusage:	rusage of wait process
 */
struct wait_info {
	pid_t pid;
	int ret;
	int wstatus;
	unsigned int options;
	struct rusage_compat *ru;
	siginfo_t *siginfo;
};

struct process_events_s {
	struct raw_mutex mutex;
	struct dlist_node head;
};

#define DUMP_SIGNAL_SIGMASK_BITLEN	SIGNAL_MAX_STANDARD
#define DUMP_SIGNAL_DUMP_ONCE_BITLEN	1U
#define DUMP_SIGNAL_DUMP_MAIN_BITLEN	1U

struct dump_on_signal_s {
	uint64_t sigmask   : DUMP_SIGNAL_SIGMASK_BITLEN;
	uint64_t dump_once : DUMP_SIGNAL_DUMP_ONCE_BITLEN;
	bool dump_main_thread_only : DUMP_SIGNAL_DUMP_MAIN_BITLEN;
};
struct tp_section;

struct exit_thread_cputime_s {
	vatomic64_t total;
	vatomic64_t run_usr;
	vatomic64_t run_sys;
	vatomic64_t ready;
};

struct process_time_info_s {
	uint64_t start_time;
	vatomic64_t cutime;
	vatomic64_t cstime;
	struct exit_thread_cputime_s exit_thread_cputime;
};

#ifdef CONFIG_THREAD_FINE_LOAD
enum proc_auth_type {
	PROC_QOS_AUTH = 0,
	PROC_DDL_AUTH,
	PROC_AUTH_NUM,
};

struct proc_qos_s {
	int value;
	vatomic32_t auth_flag[PROC_AUTH_NUM];
	struct raw_mutex mutex;
};

enum proc_qos_type {
	PROC_QOS_RESERVE = 0,
	PROC_QOS_DEFAULT,
	PROC_QOS_SYSTEM,
	PROC_QOS_FOREGROUND,
	PROC_QOS_BACKGROUND,
	PROC_QOS_FOCUS,
	PROC_QOS_NUM,
};
#endif

/*
 * Thread & Process relationship:
 * 1. When a process created, set its refcnt to 1;
 * 2. When we create a new thread for the process, will increase the process refcnt;
 * 3. When a thread exit, will decrease the process refcnt:
 *    - actually decrease the process refcnt after thread released(in thread_delete)
 * 3. When the process create a child, will increase the process refcnt;
 * 4. When a child delete from the process, or the child reparent to other process, will
 *    decrease the process refcnt;
 * 5. When the process become a session leader, the group leader will increase the refcnt;
 * 6. When the group leave the session, will decrease the process refcnt;
 * 7. When the process become a group leader, increase the member of the group will increase
 *    the process refcnt;
 * 8. When the member of the group leave the group, decrease the refcnt of the process;
 * 9. When process exit, will terminate all its threads, when thread refcnt decrease to
 *    zero, will free thread, and decrease the process refcnt.
 *
 * So we can promise that:
 *  1. the lifecycle of the process is longer than that of all its threads.
 *  2. we can use process pointer in thread while in the whole thread lifecycle.
 */
struct thread_s;
struct execve_info;
struct spawn_info;
struct __ctrlmem_kobj_process_s;
struct process_s {
	struct pid_link_s pid_link;
	raw_atomic_int_t oom_score_adj;
	raw_atomic_int_t oom_score_adj_min;
#ifdef CONFIG_MMAP_SWXN
	raw_atomic_int_t swxn_attr;
#endif
	__sec_data(sysmgr, cntx) uint32_t cnode_idx;
	/* credential type */
	__sec_data(sysmgr, cntx) uint32_t cred_type;

#ifdef CONFIG_FLO_CTRL
	struct flo_data *flo_data;
#endif

	raw_atomic_int_t nr_threads_num;

	/*
	 * privilege number.
	 * > 0: privilege service (run in EL1)
	 * = 0: normal process (run in EL0)
	 */
	__sec_data(sysmgr, cntx) uint32_t privilege_num;
	uint32_t exit_signal;
	uint64_t sequence_id;
	raw_atomic_ullong_t pmem_size;

	/* whether process already required sched module */
	bool ref_sched_required;

	bool timer_id_gen_inited;
	struct id_generator *timer_id_gen;

#ifdef CONFIG_SUBREAPER
	/*
	 * bit 1 indicates whether the process is a child_subreaper process
	 * bit 2 indicates whether a child_subreaper process exists in the ancestor process
	 */
	unsigned int child_subreaper;
#endif
	/* count of process has executed exec */
	raw_atomic_int_t exec_cnt;

	uref_t module_sched_uref;
	__sec_data(sysmgr, obj_handler) rref_t module_sched_rref;
	struct itimer it_real;
	struct process_posix_timer posix_timers;
	struct process_resource resource;

	/*
	 * This lock protects signal-related fields in struct process_s & thread.
	 *
	 * It is used in signalmgr context and released before going outside
	 * signalmgr to avoid deadlock.
	 *
	 * Lock order hints:
	 * process->group.mutex   --> process->signal_lock
	 * process->session.mutex --> process->signal_lock
	 * process->signal_lock   --> process->threads.mutex
	 */
	struct raw_mutex signal_lock;
	struct process_signal_s *signal;
	struct process_signalfd_ctxs signalfd_ctxs;
	struct cnode_spawn_pmems spawn_pmems;
	struct process_siblings siblings;
	struct procpool_nodes pool_nodes;
	/*
	 * cred lock to ensure that cred_update and get_cred_by_id are exclusive
	 * and to protect the use of user.
	 */
	struct raw_rwlock cred_lock;
#ifdef CONFIG_DAC
	struct user_res *user; /* real user info */
#endif
#ifdef CONFIG_ENCAPS
	/* encaps lock to avoid compitition of encaps updating in process and thread */
	struct raw_rwlock encaps_lock;
#endif
	struct security_s security;
	struct process_threads threads;
#ifdef CONFIG_THREAD_FINE_LOAD
	struct proc_qos_s proc_qos;
#endif

#ifdef CONFIG_PROCFSMGR_FFTS_SCHED
	struct ffrt_proc *ffrt_proc;
#endif

#ifdef CONFIG_TSM_COMMON
	struct process_ulwtinfo_s *ulwtinfo;
#endif
	/*
	 * Record the pmems allocated for uapp by memmgr_alloc_pmem
	 * for sysmgr: maybe 0, because sysmgr will not call memmgr_alloc_pmem
	 * for other process: including memmgr_alloc_pmem and extend R/U table pmem
	 */
	struct process_uapp_pmems uapp_pmems;
	/*
	 * Record the pmems allocated for uapp which won't be free
	 * until process exit, including extend c table pmem.
	 */
	struct process_table_pmems table_pmems;

	/* Wait list contains threads of current process */
	struct wait_queue_head wait_list;

	struct process_loader loader;

	/* process name lock to ensure process_name_of and process_exec_flush_name are exclusive */
	struct raw_rwlock pname_lock;

	struct rusage_compat *process_rusage;
	/*
	 * record resource usage statistics for all children of
	 * the calling process that have terminated and been waited for.
	 */
	struct rusage_compat *children_rusage;
	/*
	 * use rusage_mutex to protect process_rusage and children_rusage,
	 * prevent concurrent initialization or modification of the rusage value during access.
	 */
	struct raw_mutex rusage_mutex;

	/* process state, protected by process mutex */
	enum process_state state;

#ifdef CONFIG_MM_AMA_POLICY
	uint32_t il_prev;
#endif

#ifdef CONFIG_RLIMIT
	struct rlimit_ctrl prlimctl;
#endif

	struct raw_mutex mutex;
	struct raw_mutex ctable_lock;
	struct raw_refcnt refcnt;
#ifdef CONFIG_PDEBUG
	struct pdebug pdebug;
#endif
#ifdef CONFIG_PROCESS_COREDUMP
	struct process_coredump_attr coredump;
#endif
#ifdef CONFIG_DUMP_EXCEPTION_ON_SIGNAL
	struct dump_on_signal_s dump_on_signal;
#endif
	struct process_hangtask_attr hangtask;
	struct sysmgr_except_table extable;
	/* Record symbol info of loaded dynamic shared objects */
	uint64_t dso_sym_info;
#ifdef CONFIG_RESMGR
#ifdef CONFIG_RESMGR_COMPAT_V1
	struct res_group *rgrp_list[MAX_RES_COUNT];
#else
	struct res_group *rgrp;
#endif
	struct raw_rwlock rgrp_lock;
#endif
	uint32_t flags;
#ifdef CONFIG_SYSMGR_PM_SUSPEND
	enum proc_pm_attr pm_attr;
#endif
	struct raw_counter threads_cnter;

#ifdef CONFIG_SYSMGR_PERF
	struct perf_event_s *perf_event;
#endif

#ifdef CONFIG_UPROBE
	struct uprobe_process_s uprobe_process;
#endif

#ifdef CONFIG_PROCESS_EVENT
	struct process_events_s *events;
#endif
#ifdef CONFIG_HMPSF
	struct hmpsf_proc_s hmpsf_proc;
#endif
#ifdef CONFIG_HMPROBE
	struct hmprobe_proc_s hmprobe_proc;
#endif

	__cpuset_t default_affinity;
	/*
	 * affinity of threads in curr process are limited in default_affinity without
	 * the flag of privileged_affinity
	 * */
	bool is_privileged_affinity;
	vatomic8_t is_oom_killed;
	rref_t sysctrl_rref;

	/* Number of paths added by the process */
	raw_atomic_int_t path_cnt;
	/* The vspaceid of the coobj */
#ifdef CONFIG_COOBJ
	int coobj_vspaceid;
#endif
#ifdef CONFIG_DFX_DIE_CATCH
	struct die_catch_s die_catch;
#endif
	struct process_time_info_s time_info;
#ifdef CONFIG_YAMA
	pid_t yama_pdebugger;
#endif
#ifdef CONFIG_VFS_FUSION
	unsigned long vfs_data;
#endif
	struct __ctrlmem_kobj_process_s *kobj_process;
	uintptr_t trace_shm_addr;
	uintptr_t trace_shm_snapshot_addr;
	uintptr_t trace_shm_saved_taskinfo_addr;
	struct tp_section *tp_section;
	struct dlist_node oom_select;
};

/* If this check failed, reduce MAX_DLIST_NODE */
raw_static_assert(sizeof(struct process_s) < SLAB_MAX_LOWER_BOUND, process_size_is_too_big);

#ifdef CONFIG_DUMP_EXCEPTION_ON_SIGNAL
int process_set_signal_dump(struct process_s *process, uint32_t signum,
			    bool dump_once, bool dump_main_thread_only);
bool process_is_dump_signal(struct process_s *process, uint32_t signum);
#endif

#define SYMBOL_POOL_ENTRY_COUNT_MAX 10U
struct process_symbol_entry_s {
	__u32 cnode_idx;
	struct symbol_list_s symbol_list;
};
struct process_symbol_pool_s {
	unsigned int used;
	struct process_symbol_entry_s entries[SYMBOL_POOL_ENTRY_COUNT_MAX];
};


#ifdef CONFIG_THREAD_FINE_LOAD
#define QOS_AUTH_BIT(nr)	(1UL << (unsigned int)(nr))
static inline
struct proc_qos_s *proc_qos_of(struct process_s *process)
{
	return &process->proc_qos;
}

int proc_qos_apply(struct process_s *process, int new_qos);
void proc_qos_auth_register(enum proc_auth_type type, unsigned int flag);
void proc_qos_auth_update(struct proc_qos_s *qos, enum proc_auth_type type, unsigned int flag);
bool proc_qos_auth_allowed(struct proc_qos_s *qos, enum proc_auth_type type, unsigned int func_id);
void proc_qos_init(struct process_s *process);
#endif

enum process_model process_loader_model_of(struct process_s *process);
const char *process_loader_model_name(enum process_model model);
int process_get_name(char *name, size_t len, uint32_t cnode_idx);
const char *process_name_get_last(const char *src_name, size_t dst_len);
int process_new_self(struct process_s *process, const char *name);
void process_add_symbol(struct process_s *process, struct symbol_list_s *symbol_list);
void process_add_symbol_with_vspace(struct process_s *process,
				    struct symbol_list_s *symbol_list,
				    struct vspace_s *vspace);
void process_vm_add_symbol(struct process_s *process,
			   struct symbol_list_s *symbol_list);
void process_remove_symbol(struct symbol_list_s *symbol_list);
struct symbol_list_s *process_symbol_pool_add(struct process_symbol_pool_s *pool,
		struct process_s *process, struct vspace_s *vspace, bool emergency);
void process_symbol_pool_release(struct process_symbol_pool_s *pool);
struct symbol_list_s *process_symbol_pool_find(struct process_symbol_pool_s *pool,
		struct process_s *process);
int process_real_load_symbol(struct symbol_desc *sym_desc,
			     struct vspace_s *vspace, unsigned int sym_flags);
void process_real_unload_symbol(struct symbol_desc *sym_desc);
int process_spawn_with_parent(pid_t fixed_pid, uint32_t cnode_idx_for_root,
			      struct thread_s *parent,
			      struct spawn_info *spawn_info,
			      struct thread_s **child_thread);
int procmgr_spawn_core(struct thread_s *thread,
		       struct hm_spawn_req spawn_req,
		       pid_t *pid_ret, unsigned int *child_cnode_ret);
int process_fork_with_thread(struct thread_attr *attr,
			     uint64_t clone_flags,
			     struct thread_s **child);

int process_execve(struct thread_s *thread, struct execve_info *exec_info);

int process_add_thread(struct thread_s *thread);
int process_add_thread_nolock(struct thread_s *thread);
void process_remove_thread(struct thread_s *thread);
void process_del_from_parent(struct process_s *process);
void process_ref_remove(struct process_s *process);
void process_refcnt_release(struct raw_refcnt *ref);
int process_require_cap(struct process_s *process, cref_t cap_ref,
			uint8_t rights, cref_t *out_rref, cref_t *out_uref);

void process_leave_orgs(struct process_s *process);
void process_delete_from_ref(struct raw_refcnt *ref);

void process_dlist_init(struct process_s *process);
void process_raw_mutex_init(struct process_s *process);
void process_hangtask_attr_init(struct process_s *process, const struct process_s *parent);
void process_hangtask_attr_update(struct process_s *process);

rref_t process_acquire_cnode_rref(struct process_resource *resource);
struct vspace_s *process_acquire_vspace(struct process_s *process);
struct process_vm_s *process_get_vm(struct process_s *process);
void process_put_vm(struct process_vm_s *vm);
void process_resource_put(struct process_resource *resource);
struct thread_s *process_parent_get(struct process_s *process);
struct thread_s *process_parent_get_nolock(struct process_s *process);
pid_t process_ppid_in_pidns(struct process_s *process, struct pid_ns *ns);
unsigned long process_total_forks_of(void);
unsigned long process_total_spawns_of(void);
#ifdef __HOST_LLT__
int process_add_to_parent(struct process_s *process, struct thread_s *parent);
void process_add_to_parent_orgs(struct process_s *process, struct process_s *parent);
#endif
void process_attach_pid(struct process_s *process, int type, struct pid_s *pid);
void process_detach_pid(struct process_s *process, int type);
void process_change_pid(struct process_s *process, int type, struct pid_s *pid);
struct pid_s *process_pgid_s_with_lock(struct process_s *process);
struct pid_s *process_sid_s_with_lock(struct process_s *process);
void unlock_pidlink_and_sid(struct process_s *process, struct pid_s *sid);
void process_notify_parent(struct process_s *process, unsigned int mode);
void process_fill_notify_parent_siginfo(struct process_s *process,
					struct process_s *parent,
					struct __siginfo *siginfo,
					unsigned int signum,
					unsigned int mode);
struct process_s *find_get_process_by_pid_s(struct pid_s *pid);
__cpuset_t process_system_service_chk_affinity(const struct process_s *caller,
					       __cpuset_t affinity);
struct thread_s *process_get_main_thread(struct process_s *process);
void process_main_thread_name_of(struct process_s *process, char *buf, size_t size);

static inline struct process_s *process_of_cred_type(const unsigned int *type)
{
	return container_of(type, struct process_s, cred_type);
}

#ifdef CONFIG_RLIMIT
void process_rlimit_init(struct process_s *process, const struct process_s *parent);
#else
static inline void
process_rlimit_init(struct process_s *process, const struct process_s *parent)
{
	UNUSED(process, parent);
}
#endif

void process_info_to_parent(struct process_s *process, struct process_s *parent);
int proc_count_all_threads_nr(void);
int process_sched_stat_transfer(struct process_s *process, struct proc_sched_stat_s *stat, int num);
int process_all_process_sched_stat(struct proc_sched_stat_s *stat, int num);
#ifdef CONFIG_YAMA
static inline int process_set_yama_debugger(struct process_s *process, pid_t pid)
{
	process->yama_pdebugger = pid;
	return E_HM_OK;
}

static inline pid_t process_get_yama_debugger(struct process_s *process)
{
	return process->yama_pdebugger;
}
#endif

static inline void exit_thread_cputime_accumulate(struct __tcb_time *total_time,
					     struct exit_thread_cputime_s *exit_thread_cputime)
{
	__u64 exit_thread_cputime_total = vatomic64_read_rlx(&exit_thread_cputime->total);
	if (exit_thread_cputime_total > total_time->total) {
		total_time->total = exit_thread_cputime_total;
	}
	total_time->run_sys += vatomic64_read_rlx(&exit_thread_cputime->run_sys);
	total_time->run_usr += vatomic64_read_rlx(&exit_thread_cputime->run_usr);
	total_time->ready += vatomic64_read_rlx(&exit_thread_cputime->ready);
}

#ifdef CONFIG_PROCESS_KMEM_STAT
void process_add_km_size(struct process_s *process, enum km_type stat_type, int64_t value);
void process_sub_km_size(pid_t pid, enum km_type stat_type, int64_t value);
void current_km_pid(pid_t *rpid);
#endif
#endif
