/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of pdebug (intapi)
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 1 9:30:16 2019
 */
#ifndef SYSMGR_INTAPI_PROCESS_DEBUG_H
#define SYSMGR_INTAPI_PROCESS_DEBUG_H

#include <lib/dlist.h>
#include <hmasm/compat.h>
#include <hmkernel/capability.h>
#include <hmkernel/debug.h>
#include <hmkernel/ipc/signal.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsrv_sys/hm_procmgr.h>

struct process_s;
struct thread_s;
struct wait_info;

struct pdebug_act {
	struct dlist_node node;
	enum pdebug_act_type type;
	bool aborted;
	int futex_waiter;
	__u32 deferred_actions;
	void *data;
};

/* type for pdebug_act.data */
struct pdebug_act_thread_sigstop_arg {
	bool need_check_hmsignal;
};

#define PDEBUG_ACT_ALWAYS_SUSPEND	0x1
#define PDEBUG_ACT_DEPENDS_SUSPEND	0x2
typedef int (*act_handler)(struct thread_s *debugger_thread, struct thread_s *thread, struct pdebug_act *act);
struct pdebug_act_handler {
	act_handler handler;
	int flag;
};

struct pdebug {
	/* the following is for target process */
	/* lock for set or clear debugger thread of target */
	struct raw_mutex debugger_mutex;
	struct thread_s *debugger_thread;
	raw_atomic_int_t thread_count;

	/* the following is for debugger process */
	struct raw_mutex mutex;
	struct dlist_node threads;
	/* for pdebug wait status */
	int futex_wstatus;
	/* differentiate between pdebug (for gdb) and ptrace, for debugger */
	bool from_ptrace;
};

/* for pdebug_obj.state */
#define PDEBUG_ATTACHED		1
#define PDEBUG_STOPPED		(1 << 1)
/* thread enters the state below when it is suspended by SIGSTOP */
#define PDEBUG_SIGSTOP_STOPPED	(1 << 2)
/* thread exited and called detach_phrase_one */
#define PDEBUG_THREAD_EXITED	(1 << 3)
#define PDEBUG_SEIZED		(1 << 4)
#define PDEBUG_LISTENING	(1 << 5)
#define PDEBUG_WAKING		(1 << 6)

/* for internal futex wait */
#define PDEBUG_FUTEX_INACTIVE	0
#define PDEBUG_FUTEX_ACTIVE	1
/*
 * for pdebug_obj.resume_sig, enen if signum==0
 * is valid; low 8 bits for signal num
 */
#define VALID_RESUME_SIG	0x100

#define bit64(nr)	((__u64)1u << (__u64)(nr))

struct pdebug_obj_s {
	struct dlist_node node;
	uref_t thread_uref;
	cref_t debug_ctx;
	struct pmem_area_s *mem;
	int futex_val;
	unsigned int resume_sig;
	/* needed by supporting debugging signal */
	struct __siginfo last_siginfo;
	struct pdebug_new_thr_info new_threadinfo;
	unsigned int state;
	struct pdebug_event event;
	unsigned long last_event_msg;
	/* for setoptions */
	__u64 event_bitmap;
	bool deferred_notify_cldstop;
	struct __siginfo sigchld_info;
	/* signals to be resent to thread when wake from suspending */
	struct dlist_node pending_signals;
	__u64 pending_signal_bits;
	struct dlist_node pending_acts;
	__u64 pending_act_bits;
};

struct uctx_regs_info_s {
	struct arch_uctx uctx;
	struct aarch32_uctx uctx32;
	/* hold the final regs */
	void *regs;
	size_t regs_size;
	bool use_compat;
};

#define pdebug_obj_of(thread)			((thread)->pdebug_obj)
#define pdebug_obj_event_of(thread)		(pdebug_obj_of(thread).event)
#define pdebug_obj_event_type_of(thread)	(pdebug_obj_event_of(thread).type)

#ifdef CONFIG_PDEBUG
bool pdebug_thread_attached(const struct thread_s *current_thread, struct thread_s *thread);
void pdebug_resume_thread(struct thread_s *thread, unsigned int sig,
			  unsigned int resume_flag, __u32 *deferred_actions);
int ptrace_listen(struct thread_s *debugger_thread, struct thread_s *thread);
int pdebug_get_uctx(struct thread_s *thread, struct arch_uctx *uctx);
void pdebug_process_init(struct process_s *process);
void pdebug_thread_exit(struct thread_s *debugger_thread, bool need_log);
int pdebug_thread(struct thread_s *current_thread, enum pdebug_thread_req req, pid_t tid, void *arg);
int pdebug_wait_status(struct process_s *process, pid_t tid,
		       struct pdebug_event *msg, int option);
int pdebug_call_act(enum pdebug_act_type type, struct thread_s *thread, void *arg);
int pdebug_vspace_read(struct thread_s *debugger_thread, pid_t tid,
		  uintptr_t addr, void *data, size_t bytes);
int pdebug_vspace_write(struct thread_s *debugger_thread, pid_t tid,
		  uintptr_t addr, const void *data, size_t bytes);
int pdebug_get_hwbkpt_si_errno(struct thread_s *thread, __u64 addr);
int pdebug_handle_hmsignal(struct thread_s *thread);
int pdebug_signal_hook(const struct process_s *source, struct thread_s *thread,
		       const struct __siginfo *siginfo);
int pdebug_und_insn_hook(const struct thread_s *thread, uint32_t cnode_idx, uintptr_t pc);
bool pdebug_is_thread_attached(const struct thread_s *thread);
bool pdebug_is_thread_stopped(const struct thread_s *thread);
bool pdebug_is_thread_traced_and_stopped(struct thread_s *debugger_thread, struct thread_s *thread);
int pdebug_get_thread_status(struct process_s *debugger, struct wait_info *winfo,
			     struct thread_s **thread_to_put);
void pdebug_tracee_thread_put(struct thread_s *thread);
int ptrace_syscall_trace_set(struct thread_s *thread);
int ptrace_syscall_trace_clear(struct thread_s *thread);
struct thread_s *debugger_thread_get(const struct thread_s *thread);
void debugger_thread_put(struct thread_s *debugger_thread);
int pdebug_may_access(struct process_s *debugger, struct thread_s *thread, int type);

int __pdebug_create_cpuctx(struct thread_s *thread);
int __pdebug_config_cpuctx(rref_t cnode_rref, struct thread_s *thread,
			   enum __debug_config_cmd req, struct __debug_config_arg *const arg,
			   struct __debug_config_result *result);
int procmgr_ops_pdebug_config_debugctx(struct thread_s *debugger_thread, struct thread_s *thread,
				       enum __debug_config_cmd req, struct __debug_config_arg *const arg,
				       struct __debug_config_result *result);

struct procmgr_pdebug_ops_s {
	int (*config_debugctx)(rref_t cnode_rref, struct thread_s *thread,
			       enum __debug_config_cmd req, struct __debug_config_arg *const arg,
			       struct __debug_config_result *result);
};
void pdebug_handle_thread_deferred_actions(struct thread_s *thread, __u32 deferred_actions);

#else
static inline bool pdebug_thread_attached(const struct thread_s *current_thread, struct thread_s *thread)
{
	UNUSED(current_thread, thread);
	return false;
}
static inline void pdebug_resume_thread(struct thread_s *thread, unsigned int sig,
					unsigned int resume_flag,
					__u32 *deferred_actions) {}
static inline int ptrace_listen(struct thread_s *debugger_thread, struct thread_s *thread)
{
	UNUSED(debugger_thread, thread);
	return E_HM_NOSYS;
}
static inline int pdebug_get_uctx(struct thread_s *thread, struct arch_uctx *uctx)
{
	return E_HM_NOSYS;
}
static inline void pdebug_process_init(struct process_s *process) {}
static inline int pdebug_thread(struct thread_s *current_thread,
				enum pdebug_thread_req req, pid_t tid, void *arg)
{
	UNUSED(current_thread, req, tid, arg);
	return E_HM_NOSYS;
}
static inline void pdebug_thread_exit(struct thread_s *debugger_thread, bool need_log)
{
	UNUSED(debugger_thread, need_log);
}
static inline int pdebug_wait_status(struct process_s *process, pid_t tid,
				     struct pdebug_event *msg, int option)
{
	return E_HM_NOSYS;
}
static inline int
pdebug_call_act(__maybe_unused enum pdebug_act_type type,
		__maybe_unused struct thread_s *thread, __maybe_unused void *arg) {
	UNUSED(type, thread, arg);
	return E_HM_NOSYS;
}

static inline int
pdebug_vspace_read(struct thread_s *debugger_thread, pid_t tid,
		  uintptr_t addr, void *data, size_t bytes)
{
	UNUSED(debugger_thread, tid, addr, data, bytes);
	return E_HM_NOSYS;
}

static inline int
pdebug_vspace_write(struct thread_s *debugger_thread, pid_t tid,
		  uintptr_t addr, const void *data, size_t bytes)
{
	UNUSED(debugger_thread, tid, addr, data, bytes);
	return E_HM_NOSYS;
}
static inline int
pdebug_get_hwbkpt_si_errno(struct thread_s *thread, __u64 addr)
{
	UNUSED(thread, addr);
	return 0;
}
static inline int
pdebug_signal_hook(const struct process_s *source, struct thread_s *thread,
		   const struct __siginfo *siginfo)
{
	return E_HM_NOSYS;
}
static inline int
pdebug_handle_hmsignal(struct thread_s *thread)
{
	return E_HM_NOSYS;
}
static inline int
pdebug_und_insn_hook(const struct thread_s *thread, uint32_t cnode_idx, uintptr_t pc)
{
	return E_HM_NOSYS;
}
static inline bool
pdebug_is_thread_attached(const struct thread_s *thread)
{
	return false;
}
static inline bool
pdebug_is_thread_stopped(const struct thread_s *thread)
{
	return false;
}
static inline bool
pdebug_is_thread_traced_and_stopped(__maybe_unused struct thread_s *debugger_thread,
				    __maybe_unused struct thread_s *thread)
{
	UNUSED(debugger_thread, thread);
	return false;
}
static inline int pdebug_get_thread_status(__maybe_unused struct process_s *debugger,
					   __maybe_unused struct wait_info *winfo,
					   __maybe_unused struct thread_s **thread_to_put)
{
	return E_HM_NOSYS;
}
static inline void pdebug_tracee_thread_put(__maybe_unused struct thread_s *thread)
{
	UNUSED(thread);
}
static inline int
ptrace_write_regs(struct thread_s *debugger_thread, struct thread_s *thread, struct arch_regs *regs,
		  struct raw_mutex *mutex)
{
	return E_HM_NOSYS;
}
static inline int ptrace_syscall_trace_set(struct thread_s *thread)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}
static inline int ptrace_syscall_trace_clear(struct thread_s *thread)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}
static inline struct thread_s *debugger_thread_get(const struct thread_s *thread)
{
	UNUSED(thread);
	return NULL;
}
static inline void debugger_thread_put(struct thread_s *debugger_thread)
{
	UNUSED(debugger_thread);
}
static inline int pdebug_may_access(struct process_s *debugger, struct thread_s *thread, int type)
{
	UNUSED(debugger, thread, type);
	return E_HM_NOSYS;
}

static inline int __pdebug_create_cpuctx(struct thread_s *thread)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}
static inline int __pdebug_config_cpuctx(rref_t cnode_rref, struct thread_s *thread,
					 enum __debug_config_cmd req, struct __debug_config_arg *const arg,
					 struct __debug_config_result *result)
{
	UNUSED(cnode_rref, thread, req, arg, result);
	return E_HM_NOSYS;
}

static inline int procmgr_ops_pdebug_config_debugctx(struct thread_s *debugger_thread, struct thread_s *thread,
						     enum __debug_config_cmd req, struct __debug_config_arg *const arg,
						     struct __debug_config_result *result)
{
	UNUSED(debugger_thread, thread, req, arg, result);
	return E_HM_NOSYS;
}

static inline void pdebug_handle_thread_deferred_actions(struct thread_s *thread, __u32 deferred_actions)
{
	UNUSED(thread, deferred_actions);
}
#endif

#endif
