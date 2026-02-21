/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Captype thread
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 19 05:24:19 2018
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(Thread, 2, CAPCOARSEGRANT(Thread)
		   CAPNOGRANTMOVE(Thread)
		   CAPDEFAULTSIZE(Thread),
	/* Methods that only have capcall (called by other threads) */
	CAPMETHOD(Thread, Resume)
	CAPMETHOD(Thread, Wake)
	CAPMETHOD(Thread, KillTcbSilently)
	CAPMETHOD(Thread, HMSignalKill)
	CAPMETHOD(Thread, UpdateVecIdx)
	/* Methods that have both capcall and fastcall */
	CAPMETHOD(Thread, VSpaceOf)
	CAPMETHOD(Thread, SetCpu)
	CAPMETHOD(Thread, GetCpu)
	CAPMETHOD(Thread, Suspend)
	CAPMETHOD(Thread, AttachCpuContext)
	CAPMETHOD(Thread, DetachCpuContext)
	CAPMETHOD(Thread, Stat)
	CAPMETHOD(Thread, SetName)
	CAPMETHOD(Thread, TriggerDetachOne)
	CAPMETHOD(Thread, HMSignalMask)

	CAPMETHOD(Thread, GetSecureCookie)
	CAPMETHOD(Thread, SetSecureCookie)
	CAPMETHOD(Thread, GetPreferCookie)
	CAPMETHOD(Thread, SetPreferCookie)
	CAPMETHOD(Thread, SetSmtExpeller)
	CAPMETHOD(Thread, CpuHwpCtrl)
	CAPMETHOD(Thread, L3cPartCtrl)
	CAPMETHOD(Thread, FloInit)
	CAPMETHOD(Thread, FloOnoff)
	CAPMETHOD(Thread, PerfCtrl)
	/* Methods that only have fastcall (called by itself) */
	/* thread_cref
	 * thread_hmsignal_unset
	 * thread_hmsignal_pending
	 */
	/*
	 * thread own miscdata accessor
	 */
	CAPMETHOD(Thread, MiscdataRead)
	CAPMETHOD(Thread, MiscdataWrite)
	CAPMETHOD(Thread, SeccompKernelCache)
	CAPMETHOD(Thread, MiscdataReadv)
	CAPMETHOD(Thread, MiscdataWritev)
	CAPMETHOD(Thread, MiscdataReadvCas)
	CAPMETHOD(Thread, MiscdataWritevCas)
	CAPMETHOD(Thread, ExtFastcallBind)
	CAPMETHOD(Thread, ExtFastcallUnbind)
	CAPMETHOD(Thread, AvailableExtFastcall)

	/* Methods that only have capcall */
	CAPMETHOD(Thread, SetRq)
	CAPMETHOD(Thread, SetTickLess)
	CAPMETHOD(Thread, KvicSourceBind)
	CAPMETHOD(Thread, KvicSourceUnbind)
	CAPMETHOD(Thread, KvicKevSourceBind)
	CAPMETHOD(Thread, KvicKevSourceUnbind)
	CAPMETHOD(Thread, SchedSetAttr)
	CAPMETHOD(Thread, SchedGetAttr)
	CAPMETHOD(Thread, PacCtrl)
	CAPMETHOD(Thread, PtraceSet)
	CAPMETHOD(Thread, RtFutexExit)

	CAPERRNO(Thread, EHMERROR, THREAD_STATE,		"E_HM_THREAD_STATE")
	CAPERRNO(Thread, EHMERROR, THREAD_NOEXSP,		"E_HM_THREAD_NOEXSP")
	CAPERRNO(Thread, EHMERROR, THREAD_HMSIGNAL_PENDING,	"E_HM_THREAD_HMSIGNAL_PENDING")
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_THREAD_API_EXPORTED
#define CAP_THREAD_API_EXPORTED

#include <hmkernel/cpuset.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hmkernel/ipc/hmsignal.h>
#include <hmasm/ucontext.h>

#define HMCAP_THREAD_NAME_SIZE	16
#define THREAD_STATE_ATTR_SIZE  10
#define THREAD_SCHED_INFO_SIZE	256

#define __CAP_TCB_OBJ_SIZE  PAGE_SIZE

/* the target tcb is created with a vspace in caller cnode of the syscall */
#define THREAD_CONFIG_FLAGS_MASK_CALLER_VSPACE		(0x1ULL << 32)
/* the target tcb is created with a suspend state */
#define THREAD_CONFIG_FLAGS_MASK_IS_SUSPEND		(0x2ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_IS_CLONE_UCTX		(0x4ULL << 32)
/* the target tcb is created with a pre actv */
#define THREAD_CONFIG_FLAGS_MASK_CREATE_PRE_ACTV	(0x8ULL << 32)
/* the two bits occupied by throutine thread, see THREAD_CONFIG_FLAGS_MASK_IS_ULWT/XSTREAM */
#define THREAD_CONFIG_FLAGS_MASK_FOR_SOFTWARE_USE	(0x30ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_IS_VFORK		(0x40ULL << 32)

#define THREAD_CONFIG_FLAGS_MASK_IS_ULWT		(0x80ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_IS_WORKER		(0x100ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_IS_LSYSCALL		(0x200ULL << 32)
/* (0x400ULL << 32) reserved */
#define THREAD_CONFIG_FLAGS_MASK_WRITE_CRED		(0x800ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_IS_HELPER		(0x1000ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_RESET_ON_FORK		(0x2000ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_NEED_RESTORE_PRIO	(0x4000ULL << 32)
#define THREAD_CONFIG_FLAGS_MASK_SERVICES_CREATE	(0x8000ULL << 32)

#define thread_config_flags_caller_vspace(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_CALLER_VSPACE)		!= (typeof(f))0)
#define thread_config_flags_is_suspend(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_IS_SUSPEND)		!= (typeof(f))0)
#define thread_config_flags_is_clone_uctx(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_IS_CLONE_UCTX)		!= (typeof(f))0)
#define thread_config_flags_create_pre_actv(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_CREATE_PRE_ACTV)	!= (typeof(f))0)
#define thread_config_flags_is_vfork(f)		(((f) & THREAD_CONFIG_FLAGS_MASK_IS_VFORK) != (typeof(f))0)
#define thread_config_flags_is_ulwt(f)		(((f) & THREAD_CONFIG_FLAGS_MASK_IS_ULWT)		!= (typeof(f))0)
#define thread_config_flags_is_worker(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_IS_WORKER)		!= (typeof(f))0)
#define thread_config_flags_is_lsyscall(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_IS_LSYSCALL)		!= (typeof(f))0)
#define thread_config_flags_is_helper(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_IS_HELPER)		!= (typeof(f))0)
#define thread_config_flags_write_cred(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_WRITE_CRED)		!= (typeof(f))0)
#define thread_config_flags_reset_on_fork(f)		(((f) & THREAD_CONFIG_FLAGS_MASK_RESET_ON_FORK)		!= (typeof(f))0)
#define thread_config_flags_need_restore_prio(f)	(((f) & THREAD_CONFIG_FLAGS_MASK_NEED_RESTORE_PRIO)	!= (typeof(f))0)
#define thread_config_flags_services_create(f)		(((f) & THREAD_CONFIG_FLAGS_MASK_SERVICES_CREATE)	!= (typeof(f))0)

#define THREAD_STAT_STATE		0x01UL
#define THREAD_STAT_SCHED		0x02UL
#define THREAD_STAT_UCTX		0x04UL
#define THREAD_STAT_PENDING_SIGNALS	0x08UL
#define THREAD_STAT_TIME		0x10UL
#define THREAD_STAT_RQ			0x20UL
#define THREAD_STAT_BLOCK		0x40UL
#define THREAD_STAT_CPU 		0x80UL
#define THREAD_STAT_SIGNAL_MASK 	0x100UL
#define THREAD_STAT_NVCSW		0x200UL
#define THREAD_STAT_NIVCSW		0x400UL
#define THREAD_STAT_CUR_UCTX		0x800UL
#define THREAD_STAT_HMSIGNAL 		0x1000UL
#define THREAD_STAT_MIGRATING		0x2000UL
#define THREAD_STAT_TIMER_CREF		0x4000UL
#define THREAD_STAT_EXTRA_SCHED_INFO	0x8000UL
#define THREAD_STAT_YIELD_TIME		0x10000UL
#define THREAD_STAT_FPSIMD		0x20000UL
#define THREAD_STAT_GLOBAL_PREEMPT	0x40000UL
#define THREAD_LOCK_ADDR		0x80000UL

#define THREAD_PTRACE_SET	0x1UL
#define THREAD_PTRACE_CLEAR	0x2UL
#define THREAD_PTRACE_SET_FPSIMD	0x3UL
#define THREAD_PTRACE_SET_GPREGS	0x4UL

enum {
	CRIGHT_THREAD_STAT_BIT,
	CRIGHT_THREAD_SET_EX_SP_BIT,
	CRIGHT_THREAD_MISCDATA_BIT,
	CRIGHT_THREAD_MAX
};

enum thread_perf_ctrl_type {
	PERF_CTRL_SET_MICROARCH,
	PERF_CTRL_MAX
};

#define CRIGHT_THREAD_STAT		(1U << CRIGHT_THREAD_STAT_BIT)
#define CRIGHT_THREAD_SET_EX_SP		(1U << CRIGHT_THREAD_SET_EX_SP_BIT)
#define CRIGHT_THREAD_MISCDATA		(1U << CRIGHT_THREAD_MISCDATA_BIT)

 /* flags in sysarg_thread_config are only used when creating thread */
struct sysarg_thread_config {
	char *name;
	cref_t vspace_cref;
	cref_t scheduler_cref;
	__u64 flags;
	int retval;
	int vpid;
	int vtid;
	int pid;
	int tid;
	unsigned int uid;

	unsigned long cred_idx;
	/* only used for !callinfo->user_call(sysproc_create_sysmgr sysproc_create_idle) */
	struct arch_uctx *uctx;

	struct arch_clone_regs clone_regs;
	struct __hmsigmask mask;
	__u64 kobj_thread;
	int saved_prio;
};

enum {
	TCB_RQ_TYPE_CUR,
	TCB_RQ_TYPE_USER,
	TCB_RQ_TYPE_NR,
};

struct __tcb_time {
	__u64 total;
	__u64 run_usr;
	__u64 run_sys;
	__u64 ready;
	__u64 suspend;
	__u64 sctime; /* state change time */
};

struct __tcb_hmsignal_info {
	struct __hmsigmask curr_actv_hmsigmask;
	struct __hmsigmask base_actv_hmsigmask;
	struct __hmsigmask pending_hmsignals;
};

struct sysarg_thread_stat {
	__u32 tcb_state;
	__u32 tcb_rq[TCB_RQ_TYPE_NR];
	__u32 tcb_block;
	__u32 tcb_futex_flags;
	__u32 tcb_running_cpu;
	__u32 tcb_cnode_idx;
	__u16 tcb_is_migrating;
	__u16 tcb_is_global_preempt;
	__u64 tcb_block_futex_vaddr;
	__u64 tcb_pending_signals;
	__u64 tcb_signal_mask;
	__u64 tcb_sched;
	__u64 tcb_nvcsw;
	__u64 tcb_nivcsw;
	__u32 tcb_yield_time;
	__u64 tcb_secure_cookie;
	__u64 tcb_prefer_cookie;
	__u32 tcb_smt_expeller;
	__u32 tcb_check_cpu;
	__u32 tcb_check_result;
	__u64 tcb_check_time;
	__s32 tcb_attr[THREAD_STATE_ATTR_SIZE];
	/* thread time, unit: tock */
	struct __tcb_time tcb_time;
	struct __tcb_hmsignal_info hmsiginfo;
	cref_t prev_tcb_cref;
	cref_t preempt_tcb_cref;
	/* timer which has sent HMSIGNAL_ALARM */
	cref_t tcb_timer_cref;
	char sched_info[THREAD_SCHED_INFO_SIZE];
	/*
	 * 1. the size of struct arch_uctx is not fixed while 32bit-uapp
	 * calls 64bit-kernel, so the uctx should be the last variable of
	 * the struct.
	 * 2. the size of all the other variables above should be fixed.
	 * 3. In aarch64 with compat opened, arch_uctx in kernel is 16 bytes
	 * aligned. However, it's aligned to 8 bytes for 32-bit uapp, so it
	 * need be aligned to 16 bytes here so that the 32-bit uapp can get
	 * correct offset.
	 */
	struct arch_uctx uctx __aligned(16);
};

struct sysarg_execve_config {
	char name[HMCAP_THREAD_NAME_SIZE];
	cref_t vspace_cref;
	struct arch_uctx uctx;
	int retval;
	int tid;
	unsigned int uid;
	__bool set_cnode_flags;
};

#define THREAD_RT_FUTEX_EXIT_VALID_ADDR			1U
#define THREAD_RT_FUTEX_EXIT_INVAL_ADDR			2U

struct sysarg_thread_rt_futex_exit_info {
	__u64 rt_futex_addr;
	__u32 flags;
};

#endif
#endif
