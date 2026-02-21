/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Header of hm proccess management
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 10:59:31 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_PROCMGR_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_PROCMGR_H

#include <unistd.h>
#include <spawn.h>
#include <sys/resource.h>
#include <hongmeng/errno.h>
#include <libbunch/bunch.h>
#include <hmkernel/debug.h>
#include <hmkernel/ipc/signal.h>
#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <limits.h>
#include <sys/wait.h>
#undef _GNU_SOURCE
#else
#include <limits.h>
#include <sys/wait.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* the process is created with a suspend state */
#define SPAWN_CONFIG_FLAGS_MASK_SUSPEND			0x1U
#define spawn_config_flags_is_suspend(f)	(((f) & SPAWN_CONFIG_FLAGS_MASK_SUSPEND) != 0U)
#define spawn_config_flags_set_suspend(f)	((f) |= SPAWN_CONFIG_FLAGS_MASK_SUSPEND)
/* pgid of new process is made the same as its pid */
#define SPAWN_CONFIG_FLAGS_MASK_SETPGROUP		0x2U
#define spawn_config_flags_is_setpgroup(f)	((f) & SPAWN_CONFIG_FLAGS_MASK_SETPGROUP)

#define SPAWN_CONFIG_FLAGS_MASK_SETUID			0x2U
#define spawn_config_flags_is_setuid(f)		((f) & SPAWN_CONFIG_FLAGS_MASK_SETUID)

/* the process will be created with the specified affinity */
#define SPAWN_CONFIG_FLAGS_MASK_SETAFFINITY 		0x4U
/* the thread created by the process will be set with specified default affinity */
#define SPAWN_CONFIG_FLAGS_MASK_SETDEFAULTAFFINITY 	0x8U
/* posix_spawn_file_actions_t.__flags POISX used 1~128 */
#define SPAWN_CLONE_FS				256
#define EXEC_CLONE_FS				512

/* waitid status mask */
#define STATUS_MASK  0x7fU
#define STATUS_LOW_BITS 8U
#define CALLER_PIDNSID UINT_MAX

struct rusage_compat {
	/* user CPU time used */
	long hm_ru_utime_tv_sec;
	long hm_ru_utime_tv_usec;
	/* system CPU time used */
	long hm_ru_stime_tv_sec;
	long hm_ru_stime_tv_usec;

	long hm_ru_maxrss;
	long hm_ru_ixrss;
	long hm_ru_idrss;
	long hm_ru_isrss;
	long hm_ru_minflt;
	long hm_ru_majflt;
	long hm_ru_nswap;
	long hm_ru_inblock;
	long hm_ru_oublock;
	long hm_ru_msgsnd;
	long hm_ru_msgrcv;
	long hm_ru_nsignals;
	long hm_ru_nvcsw;
	long hm_ru_nivcsw;
};

struct rusage_compat64 {
	/* user CPU time used */
	uint64_t hm_ru_utime_tv_sec;
	uint64_t hm_ru_utime_tv_usec;
	/* system CPU time used */
	uint64_t hm_ru_stime_tv_sec;
	uint64_t hm_ru_stime_tv_usec;

	uint64_t hm_ru_maxrss;
	uint64_t hm_ru_ixrss;
	uint64_t hm_ru_idrss;
	uint64_t hm_ru_isrss;
	uint64_t hm_ru_minflt;
	uint64_t hm_ru_majflt;
	uint64_t hm_ru_nswap;
	uint64_t hm_ru_inblock;
	uint64_t hm_ru_oublock;
	uint64_t hm_ru_msgsnd;
	uint64_t hm_ru_msgrcv;
	uint64_t hm_ru_nsignals;
	uint64_t hm_ru_nvcsw;
	uint64_t hm_ru_nivcsw;
};

struct process_affinity {
	/* affinity that will be set for the main thread of process */
	__cpuset_t affinity;
	/*
	 * affinity that will be set for the child thread&process
	 * that created by curr process
	 * */
	__cpuset_t default_affinity;
	bool is_privileged_affinity;
};

struct hm_spawn_attr {
	const char *path;
	const char *const *argv;
	const char *const *envp;
	const posix_spawn_file_actions_t *file_action;
	const posix_spawnattr_t *attrp;
	unsigned int flags;
	unsigned long layout_end;
	struct process_affinity process_affinity;
};

struct hm_execve_req {
	uint64_t layout_end;
	uint64_t path;
	uint64_t argv;
	uint64_t envp;
};

struct hm_spawn_req {
	unsigned int flags;
	struct hm_execve_req exec_req;
	struct process_affinity process_affinity;
};

struct hm_clone_req {
	uint64_t clone_flags;
	uint64_t child_stack;
	uint64_t ptid;
	uint64_t newtls;
	uint64_t ctid;
	uint64_t entry;
	uint64_t args;
	uint64_t wu_desc_addr;
};

struct proc_sched_stat_s {
	pid_t tid;
	pid_t pid;
	__u64 stime;
	__u64 utime;
	__u64 cstime;
	__u64 cutime;
};

#define PROCESS_EXIT_UVMM_EVENT 0U
#define PROCESS_EVENT_MAX	1U

#define EVENT_PATH_LEN 64
struct event_args_s {
	char name[EVENT_PATH_LEN];
};

#define MAX_CA_PATH_LEN 256
struct get_tid_path_s {
	char path[MAX_CA_PATH_LEN];
};

enum coredump_request {
	COREDUMP_INVAL,
	COREDUMP_FORCE,
	COREDUMP_FILTER_TYPE,
	COREDUMP_FILTER,
	COREDUMP_STACKSIZE,
	COREDUMP_MAX,
	COREDUMP_ON_EXIT,
};

union coredump_attr {
	uint64_t stacksize;
	uint64_t force;
	uint32_t filter;
	uint8_t  filter_type;
	int on_exit;
};

struct hm_post_fork_hook {
	void (*func)(void);
	struct hm_post_fork_hook *link;
};

enum pdebug_thread_req {
	PDEBUG_THREAD_ATTACH,
	PDEBUG_THREAD_SEIZE,
	PDEBUG_THREAD_TRACEME,
	PDEBUG_THREAD_DETACH,
	PDEBUG_THREAD_SUSPEND,
	PDEBUG_THREAD_INTERRUPT,
	PDEBUG_THREAD_RESUME,
	PDEBUG_THREAD_READ_REGS,
	PDEBUG_THREAD_WRITE_REGS,
	PDEBUG_THREAD_GET_SIGINFO,
	PDEBUG_THREAD_GET_NEWTID,
	PDEBUG_THREAD_SET_EVENT_OPTION,
	PDEBUG_THREAD_GET_EVENT_MSG,
	PDEBUG_THREAD_GET_UCTX_32BIT,
};

enum pdebug_vspace_req {
	PDEBUG_VSPACE_READ,
	PDEBUG_VSPACE_WRITE,
};

/* sig_code types */
#define PDEBUG_SIG_NORMAL		0
#define PDEBUG_SIG_HW_WATCHPOINT	1
#define PDEBUG_SIG_HW_BREAKPOINT	2

enum pdebug_event_type {
	PDEBUG_EVENT_NONE = 0,
	PDEBUG_EVENT_THREAD_CREATE,
	PDEBUG_EVENT_THREAD_EXIT,
	PDEBUG_EVENT_THREAD_ATTACHED,
	PDEBUG_EVENT_THREAD_STOPPED,
	PDEBUG_EVENT_STOP,
	PDEBUG_EVENT_THREAD_SIGNALED,
	PDEBUG_EVENT_PROCESS_EXIT,
	PDEBUG_EVENT_PROCESS_EXEC,
	PDEBUG_EVENT_PROCESS_FORK,
	PDEBUG_EVENT_PROCESS_CLONE,
	PDEBUG_EVENT_PROCESS_VFORK,
	PDEBUG_EVENT_PROCESS_VFORK_DONE,
	PDEBUG_EVENT_THREAD_TERMINATE,
	PDEBUG_EVENT_SYSCALL_TRACE,
	PDEBUG_EVENT_SECCOMP_TRACE,
};

struct pdebug_create_arg {
	pid_t new_pid;
	rref_t new_rref;
};

struct pdebug_fork_arg {
	pid_t child_pid;
	rref_t child_thread_rref;
};

struct pdebug_new_thr_info {
	pid_t new_pid;
	rref_t new_rref;
};

struct pdebug_exit_arg {
	unsigned int exit_status;
};

struct pdebug_signal_arg {
	struct __siginfo siginfo;
};

struct pdebug_strace_arg {
	unsigned int stop_signal;
};

struct pdebug_event {
	enum pdebug_event_type type;
	pid_t pid;
	union {
		struct pdebug_create_arg create_arg;
		struct pdebug_exit_arg exit_arg;
		struct pdebug_signal_arg signal_arg;
		struct pdebug_fork_arg fork_arg;
		struct pdebug_strace_arg strace_arg;
	};
};

enum pdebug_act_type {
	PDEBUG_ACT_ATTACH = 0,
	PDEBUG_ACT_SUSPEND,
	PDEBUG_ACT_EVENT_STOP,
	PDEBUG_ACT_SIGNAL,
	PDEBUG_ACT_THREAD_CREATE,
	PDEBUG_ACT_THREAD_EXIT,
	PDEBUG_ACT_THREAD_SIGSTOP,
	PDEBUG_ACT_PROCESS_EXEC_DETACH,
	PDEBUG_ACT_PROCESS_EXEC,
	PDEBUG_ACT_PROCESS_EXEC_ATTACH,
	PDEBUG_ACT_WRITE_REGS,
	PDEBUG_ACT_PROCESS_FORK,
	PDEBUG_ACT_PROCESS_CLONE,
	PDEBUG_ACT_PROCESS_VFORK,
	PDEBUG_ACT_PROCESS_VFORK_DONE,
	PDEBUG_ACT_THREAD_TERMINATE,
	PDEBUG_ACT_EXITKILL,
	PDEBUG_ACT_SYSCALL_TRACE,
	PDEBUG_ACT_TRACESYSGOOD,
	PDEBUG_ACT_SIGNAL_HOOK,
	PDEBUG_ACT_SECCOMP_TRACE,
	PDEBUG_ACT_TYPE_INVAL,
};

#define PDEBUG_WOPT_NOHANG				1
#define PDEBUG_WOPT_NOFETCH				(1 << 2)

#define CPUCONTEXT_NAME_LEN	16

struct cpucontext_name {
	char string[CPUCONTEXT_NAME_LEN];
};

enum proc_pm_attr {
	PM_ATTR_NONE = -1,
	PM_ATTR_NORMAL,
	PM_ATTR_SYSTEM,
	PM_ATTR_MAX,
};

#define PM_CONFIG_NAME_LEN	64

struct proc_pm_config {
	char name[PM_CONFIG_NAME_LEN];
	enum proc_pm_attr attr;
};

#define DORMANCY_INFO_LEN 256

struct proc_pm_dormancy_info {
	char info[DORMANCY_INFO_LEN];
};

struct __actvret_hmcall_proc_spawn;

#define DYN_SHM_MAGIC_NUM (0x44594e) /* DYN */
#define DYN_LIBS_SHM_NAME  "vfs_dyn_libs_shm"

struct shm_file_entry {
	char file_name[NAME_MAX];
	uint64_t file_offset;
	uint64_t file_size;
};

struct shm_files_hdr {
	int magic_num;
	unsigned int num_of_files;
};

struct ttyinfo_s {
	unsigned int devno; /* compatable with linux dev_t */
	char name[TTY_NAME_MAX];
};

/* compatible with data structure in LDK */
#define MAX_PID_NUM		5
#define DUBAI_CMDLINE_LEN	128
#define TASK_COMM_LEN		16
#define TASK_PREFIX_LEN		32

struct uid_info {
	uid_t uid;
	char name[DUBAI_CMDLINE_LEN];
} __attribute__((packed));

struct uid_pid_set {
	pid_t pids[MAX_PID_NUM];
	int pid_num;
} __attribute__((packed));

struct decomposed_proc_info {
	uid_t uid;
	char comm[TASK_COMM_LEN];
	char prefix[TASK_PREFIX_LEN];
} __attribute__((packed));

struct binder_proc {
	uid_t uid;
	pid_t pid;
	char cmdline[DUBAI_CMDLINE_LEN];
}__attribute__((packed));

struct binder_stat_info {
	struct binder_proc server;
	struct binder_proc client;
	int count;
}__attribute__((packed));

#define TASK_CPUSET_LEN		128
#define TASK_HIGHLOAD_MAX_CNT	10
struct abnormal_task_load_entry {
	uid_t uid;
	pid_t pid;
	pid_t tid;
	char task_name[TASK_COMM_LEN];
	unsigned int cpuload_ratio;
	int qos;
	char cpuset[TASK_CPUSET_LEN];
	unsigned int load_usage;
};

struct abnormal_task_load_stat {
	bool abnormal;
	unsigned int count;
	struct abnormal_task_load_entry task_entry[TASK_HIGHLOAD_MAX_CNT];
	/* count of tasks that still in g_abnormal_tasks */
	unsigned int history_count;
} __attribute__((packed));

enum load_chk_type {
	LOAD_CHK_ALL_TASK,
	LOAD_CHK_HISTORY_ABNORMAL_TASK,
	LOAD_CHK_TYPE_NR,
};
struct task_load_chk_args {
	unsigned int interval;
	unsigned int up_ratio;
	unsigned int down_ratio;
	unsigned int cpu_window_idx;
	unsigned int cpu_window_load;
	unsigned int usage_threshold;
	enum load_chk_type type;
};

int hm_spawn(pid_t *pid, const char *path,
	     const posix_spawn_file_actions_t *file_action,
	     const posix_spawnattr_t *attrp,
	     const char *const argv[], const char *const envp[]);
int hm_spawn_ex(const struct hm_spawn_attr *attr,
		struct __actvret_hmcall_proc_spawn *pret);
void hm_spawn_attr_init(struct hm_spawn_attr *attr);
void hm_spawn_set_affinity(struct hm_spawn_attr *attr,
			   const struct process_affinity *process_affinity,
			   unsigned int flags);
void hm_spawn_set_suspend_state(struct hm_spawn_attr *attr);
int hm_spawn_with_attr(pid_t *pid, const char *path,
		       const posix_spawn_file_actions_t *file_action,
		       const posix_spawnattr_t *attrp,
		       const char *const argv[], const char *const envp[],
		       struct hm_spawn_attr *attr);
int hm_waitpid(pid_t pid, unsigned int options, int *wstatus);
int hm_exit(int wstatus);
pid_t hm_fork(void);
pid_t hm_getpid(void);
uid_t hm_getuid(void);
pid_t hm_getppid(void);
int hm_setresuid(uid_t ruid, uid_t euid, uid_t suid);
int hm_setresgid(gid_t rgid, gid_t egid, gid_t sgid);
int hm_setgroups(uint32_t data_size, const gid_t *groups);
int hm_get_cnode_idx(pid_t pid, uint32_t *cnode_idx);
int hm_get_cnode_idx_in_pidns(pid_t pid, uint32_t nsid, uint32_t *cnode_idx);
int hm_get_cnode_idx_in_caller_pidns(pid_t pid, uint32_t *cnode_idx);
pid_t hm_pid_of_cnode_idx(uint32_t cnode_idx);
pid_t hm_pid_of_cnode_idx_in_ns(uint32_t cnode_idx, uint32_t nsid);
int hm_proc_stat(pid_t pid, unsigned int flags, struct bunch *bunch);
int hm_dubai_binder_stats_request(void *stat_info, int *count, int max_count);
int hm_dubai_cpustat_request(void *stat, int *count, size_t size, int type);
int hm_dubai_proc_cmdline_request(uid_t uid, pid_t pid, char *cmdline, size_t size, char *comm, size_t comm_size);
int hm_dubai_pids_of_uid_request(struct uid_info *info, size_t uid_info_size,
				 struct uid_pid_set *pid_set, size_t pid_set_size);
int hm_dubai_decomposed_procs_update(struct decomposed_proc_info *proc_info, int count);
int hm_abnormal_task_load_stat_request(void *task_load_stat, size_t stat_size,
				       void *load_chk_args, size_t args_size);
int hm_server_proc_stat(rref_t server, pid_t pid, unsigned int flags,
			struct bunch *bunch);
pid_t hm_getpgid(pid_t pid);
pid_t hm_getsid(pid_t pid);
int hm_setfgpgrp(pid_t sid, pid_t pgid);
pid_t hm_getfgpgrp(pid_t sid);
pid_t hm_mysid(void);
int hm_get_sid_pgid(pid_t *sid, pid_t *pgid);
int hm_signal_fgpgrp(pid_t sid, int signum);
int hm_signal_check_ignore(pid_t pid, int signum);
int hm_session_set_ttyinfo(pid_t sid, const struct ttyinfo_s *info);
int hm_session_get_ttyinfo(struct ttyinfo_s *info, bool fwd);
int hm_get_coredump(pid_t pid, enum coredump_request request,
		    union coredump_attr *result);
int hm_set_coredump(pid_t pid, enum coredump_request request,
		    union coredump_attr value);
int hm_nsid_of_cnode_idx(uint32_t cnode_idx, int flag, uint32_t *id);
int hm_pdebug_thread_attach(pid_t tid);
int hm_pdebug_thread_seize(pid_t tid);
int hm_pdebug_thread_detach(pid_t tid);
int hm_pdebug_thread_suspend(pid_t tid);
int hm_pdebug_thread_interrupt(pid_t tid);
int hm_pdebug_thread_read_regs(pid_t tid, struct arch_regs *tregs);
int hm_pdebug_thread_write_regs(pid_t tid, struct arch_regs *tregs);
int hm_pdebug_thread_resume(pid_t tid, unsigned int sig);
int hm_pdebug_wait_status(pid_t tid, struct pdebug_event *event, int option);
int hm_pdebug_thread_get_siginfo(pid_t tid, struct __siginfo *siginfo);
int hm_pdebug_thread_get_new_threadinfo(pid_t tid, struct pdebug_new_thr_info *newthread);
int hm_pdebug_vspace_read(pid_t tid, unsigned long addr, unsigned long * const data);
int hm_pdebug_vspace_write(pid_t tid, unsigned long addr, unsigned long data);
int hm_pdebug_set_event_options(pid_t tid, uint64_t options);
int hm_pdebug_get_eventmsg(pid_t tid, unsigned long *out);
int hm_pdebug_thread_get_uctx(int tid);
int hm_pdebug_config_ss_disable(pid_t tid);
int hm_pdebug_config_ss_enable(pid_t tid);
int hm_pdebug_get_debug_info(pid_t tid, struct __debug_config_result *result);
int hm_pdebug_config_wp_reg(pid_t tid, struct __debug_config_arg *arg);
int hm_pdebug_config_bp_reg(pid_t tid, struct __debug_config_arg *arg);
int hm_pdebug_seccomp_event(void *arg);
int hm_seccomp_refresh_data(void *data, unsigned long data_size);
/* RLIM_INFINITY in musl is always 64-bit, we need 32-bit in arm32 */
#define RLIM_INFINITY_32 (~0UL)

#if defined(__aarch64__)
# define HM_RLIM_INFINITY RLIM_INFINITY
#elif defined(__arm__)
# define HM_RLIM_INFINITY RLIM_INFINITY_32
#elif defined(__x86_64__)
# define HM_RLIM_INFINITY RLIM_INFINITY
#elif defined(__i686__)
# define HM_RLIM_INFINITY RLIM_INFINITY_32
#else
# error "Unknown architecture"
#endif

#define HM_RLIM_FILE_OPEN_CUR 1024

int hm_prlimit_by_pid(pid_t pid, unsigned int type,
		      const struct rlimit *new_rlim,
		      struct rlimit *old_rlim);
int hm_rlimit_by_cnode_idx(uint32_t cnode_idx, unsigned int type,
			   const struct rlimit *new_rlim,
			   struct rlimit *old_rlim);

int hm_proc_elf_load_info(pid_t pid, struct bunch *bunch);
int hm_proc_show_pte(const void *vaddr);
int hm_proc_register_event(unsigned int type,
			   const struct event_args_s *args,
			   bool fwd);
int hm_proc_getdumpable(void);
int hm_proc_setdumpable(unsigned long dumpable);

#define CHECK_REG_PC 0u
#define CHECK_REG_SP 1u
#define CHECK_MATCH_FAILED  0
#define CHECK_MATCH_SUCCESS 1
/*
 * hm_proc_regs_check - Check whether SP or PC is in specified address range
 * @check_which: which register to check
 * @addr: the start address of the check range
 * @size: the size of the check range
 *
 * Return 0 when @check_which is NOT in the check range
 * Return 1 when @check_which is matched
 * Return E_HM_xx when an error occurred
 */
int hm_proc_regs_check(unsigned int check_which, unsigned long addr, size_t size);

int hm_proc_set_child_subreaper(unsigned long child_subreaper);
int hm_proc_get_child_subreaper(int *const child_subreaper);
int hm_proc_set_privs(unsigned long arg2, unsigned long arg3,
		      unsigned long arg4, unsigned long arg5);
int hm_proc_get_privs(unsigned long arg2, unsigned long arg3,
		      unsigned long arg4, unsigned long arg5);
/*
 * hm_proc_current_pgrp_isorphaned - Check whether current process belongs to an orphaned process group
 * Return 0 means current process don`t belong to an orphaned process group
 * Return 1 means current process belongs to an orphaned process group
 * Return <0 when an error occurred
 */
int hm_proc_current_pgrp_isorphaned(void);

#define HM_PROC_TYPE_PROCESS		0
#define HM_PROC_TYPE_THREAD		1
/*
 * hm_proc_get_processed_nr - Get number of processes/threads in the system
 * @type:
 * 	type = HM_PROC_TYPE_PROCESS: get processes numbers in the system
 *  	type = HM_PROC_TYPE_THREAD: get threads number for process
 * @tid: tid only works when type = HM_PROC_TYPE_THREAD
 *	pid = -1: get all threads num in the system
 *	pid = 0: get threads num for curr process
 *	pid > 0: means get threads num for specific process
 * return thread/process number if sunccess
 * return < 0 if an error occurred
 */
int hm_proc_get_nr(int type, int pid);

/*
 * hm_proc_sched_stat - Get sched_stat for processes/threads in the system
 * @type:
 * 	type = HM_PROC_TYPE_PROCESS: get processes sched stat in the system
 *  	type = HM_PROC_TYPE_THREAD: get threads sched stat in the system
 * @id: tid only works when type = HM_PROC_TYPE_THREAD
 *	id = -1: get all thread/process sched stat
 *	id = 0: get sched stat for curr thread/process
 *	pid > 0: get sched stat for specific thread/process
 * @sched_stat: memory malloced by user
 * @num: size of array sched_stat
 * return filled buffer size if success
 * return < 0 when an orror occurred
 */
int hm_proc_sched_stat(int type, pid_t id, struct proc_sched_stat_s *sched_stat, int num);
int hm_proc_sync_cred_for_ap(cref_t ap_cref);
int hm_proc_get_cred_by_id(void *cred, size_t len,
			   unsigned long long proc_id, int flags);
int hm_proc_get_cred_by_id_in_pidns(void *cred, size_t len,
				    unsigned long long proc_id, unsigned int nsid, int flags);
int hm_proc_current_exec_path(char *buf, size_t len);

int hm_proc_vtid_to_root_tid(pid_t pid);
#ifdef __cplusplus
}
#endif

#endif
