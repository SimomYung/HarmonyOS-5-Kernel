/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of actv local area
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 18 10:06:03 2023
 */
#include <lib/trace/trace_misc.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmactv/actv.h>
#include <hmasm/current.h>
#include <hmkernel/capability.h>
#include <hmkernel/activation/sched.h>
#include <hmkernel/pac_func.h>
#include <hongmeng/types.h>
#include <libpreempt/preempt.h>
#include <libsysif/sysmgr/api.h>
#include <libpcpinfo/pcpinfo.h>
#include <hmasm/current.h>
#include <libhmtrace_ng/hmtrace.h>
/* for raw thread implement */
#include "../libhmsync/raw_tls.h"
#include <lib/pac.h>

#ifdef CONFIG_ACTV_LOCAL_AREA_K
struct actv_local_area_info_s {
	/*
	 * Record the current address of actv_local_k. Use it to check whether
	 * the TLS base was switched. If did, reset the actv local area.
	 */
	volatile struct __actv_local_area_k *p_actv_local_k;
	/*
	 * Record the kinds of the kernel information that would be
	 * synchronized to the actv local area.
	 */
	unsigned long flags;
};

static __thread volatile struct actv_local_area_info_s g_local_area_info = {
	.p_actv_local_k = NULL,
	.flags = 0U
};

/* used for storing info delivered from kernel to current actv */
static __thread volatile struct __actv_local_area_k actv_local_k = {
	.raising_hmsignals = {
		.__mask = 0ULL
	},
	.sched_info = {
		.inited = 0U,
		.state = 0U,
		.resched = 0U
	},
#ifdef CONFIG_SECCOMP
	.seccomp_info = {
		.scno = 0U,
		.arch = 0U,
		.instruction_pointer = 0U,
		.seccomp_args = {0U, 0U, 0U, 0U, 0U, 0U},
		.hscno = 0U,
		.inited = 0U,
		.seccomp_filter = 0U,
		.seccomp_filter_coidx = 0U,
		.need_check_thiscall = 0U,
	},
#endif
#ifdef CONFIG_ACTV_SCHED_STAT
	.actv_sched_info = {
		.ready_time = 0U,
		.block_time = 0U,
		.ready_cnt = 0U,
		.block_cnt = 0U,
		.need_reset = 0U
	},
#endif
	.iowait_info = {
		.iowait = 0U
	},
#ifdef CONFIG_ACTV_SCHED_LT
	.lt_info = {
		.inited = 0U,
		.group = 0U
	},
#endif
#ifdef CONFIG_ACTV_LOCAL_AREA_SERVICE_INFO
	.service_info = {
		.base_credential = 0ULL
	},
#endif
	.creds = {
		.cred = 0U,
		.xact_cred = 0U
	},
#ifdef CONFIG_ACTV_SCHED_FS
	.fs_info = {
		.saved_uctx = 0ULL,
		.flush_cb = 0ULL,
		.flush_stack = 0ULL
	},
#endif
};

void hm_actv_set_local_area(unsigned long flags)
{
	if (g_local_area_info.p_actv_local_k != &actv_local_k) {
		/*
		 * If the TLS base was switched, reset the actv local area.
		 */
		mem_zero_s(actv_local_k);

		NOFAIL(sysfast_actv_set_local_area(flags, (struct __actv_local_area_k *)&actv_local_k));
		g_local_area_info.flags = flags;
		g_local_area_info.p_actv_local_k = &actv_local_k;
	} else if (unlikely(____IS((flags & __ACTV_LOCAL_AREA_FLAGS_MASK) & (~g_local_area_info.flags)))) {
		NOFAIL(sysfast_actv_set_local_area(flags, (struct __actv_local_area_k *)&actv_local_k));
		g_local_area_info.flags |= flags;
	}
}


void hm_actv_local_reset(void)
{
	mem_zero_s(actv_local_k);
	mem_zero_s(g_local_area_info);
	preempt_process_lc_reset();
}

void hm_actv_unset_local_area(void)
{
	NOFAIL(sysfast_actv_set_local_area(0UL, NULL));
}

uint64_t hm_actv_local_get_raising_hmsignals(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_HMSIGNAL);
	return actv_local_k.raising_hmsignals.__mask;
}

struct __sched_info_s *hm_actv_local_sched_of(void)
{
	return (struct __sched_info_s *)&actv_local_k.sched_info;
}

void hm_actv_local_set_sched_inited(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_SCHED_INFO);
	actv_local_k.sched_info.inited = SCHED_MAGIC_VAL;
}

void hm_actv_local_set_iowait(void)
{
	/*
	 * Note that iowait is stored in actv_local_k, hm_actv_set_local_area
	 * may clear actv_local_k, so here assign actv_local_k a value, must
	 * be called after hm_actv_set_local_area.
	 */
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_IOWAIT_INFO);
	actv_local_k.iowait_info.iowait = 1;
}

void hm_actv_local_clr_iowait(void)
{
	actv_local_k.iowait_info.iowait = 0;
}

void hm_actv_local_set_block_info(unsigned long pc)
{
	if (hm_trace_is_tracing_block_enabled()) {
		hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_BLOCKED_INFO);
		hm_set_block_info((struct __block_info_s *)&actv_local_k.block_info, pc);
	}
}

void hm_actv_local_clr_block_info(void)
{
	if (hm_trace_is_tracing_block_enabled()) {
		mem_zero_s(actv_local_k.block_info);
	}
}
#else
void hm_actv_local_reset(void) { }

void hm_actv_unset_local_area(void) { }

void hm_actv_set_local_area(unsigned long flags)
{
	UNUSED(flags);
}

uint64_t hm_actv_local_get_raising_hmsignals(void)
{
	return 0;
}

void hm_actv_local_set_iowait(void) { }

void hm_actv_local_clr_iowait(void) { }

void hm_actv_local_set_block_info(void) {}

void hm_actv_local_clr_block_info(void) {}
#endif /* CONFIG_ACTV_LOCAL_AREA_K */

#ifdef CONFIG_ACTV_SCHED_LT
static inline void hm_actv_local_init_lt_info(void)
{
	mem_zero_s(actv_local_k.lt_info);
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_LT_INFO);
	actv_local_k.lt_info.inited = LT_MAGIC_VAL;
}

static inline bool hm_actv_local_is_lt_info_inited(void)
{
	return actv_local_k.lt_info.inited == LT_MAGIC_VAL;
}

unsigned int hm_actv_local_lt_group_of(void)
{
	if (unlikely(!hm_actv_local_is_lt_info_inited())) {
		hm_actv_local_init_lt_info();
	}
	return actv_local_k.lt_info.group;
}
#else
unsigned int hm_actv_local_lt_group_of(void)
{
	return 0;
}
#endif

#ifdef CONFIG_ACTV_SCHED_FS
__u64 hm_actv_local_saved_uctx_of(void)
{
	return actv_local_k.fs_info.saved_uctx;
}

void hm_actv_local_set_saved_uctx(__u64 uctx)
{
	actv_local_k.fs_info.saved_uctx = uctx;
}

__u64 hm_actv_local_flush_cb_of(void)
{
	if (actv_local_k.fs_info.flush_cb == 0) {
		return 0;
	}
	return __autdb(actv_local_k.fs_info.flush_cb, &(actv_local_k.fs_info.flush_cb));
}

void hm_actv_local_set_flush_cb(__u64 flush_cb)
{
	__u64 res = flush_cb;
	if (flush_cb != 0) {
		res = __pacdb(flush_cb, &(actv_lopcal_k.fs_info.flush_cb));
	}
	actv_local_k.fs_info.flush_cb = res;
}

__u64 hm_actv_local_flush_stack_of(void)
{
	if (actv_local_k.fs_info.flush_stack == 0) {
		return 0;
	}
	return __autdb(actv_local_k.fs_info.flush_stack, &(actv_local_k.fs_info.flush_stack));
}

void hm_actv_local_set_flush_stack(__u64 flush_stack)
{
	__u64 res = flush_stack;
	if (flush_stack != 0) {
		res = __pacdb(flush_stack, &(actv_local_k.fs_info.flush_stack));
	}
	actv_local_k.fs_info.flush_stack = res;
}

void hm_actv_local_set_fs_inited(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_FS_INFO);
}
#else
__u64 hm_actv_local_saved_uctx_of(void)
{
	return 0ULL;
}

void hm_actv_local_set_saved_uctx(__u64 uctx)
{
	UNUSED(uctx);
}

__u64 hm_actv_local_flush_cb_of(void)
{
	return 0ULL;
}

void hm_actv_local_set_flush_cb(__u64 flush_cb)
{
	UNUSED(flush_cb);
}

__u64 hm_actv_local_flush_stack_of(void)
{
	return 0ULL;
}

void hm_actv_local_set_flush_stack(__u64 flush_stack)
{
	UNUSED(flush_stack);
}

void hm_actv_local_set_fs_inited(void) { }
#endif

#ifdef CONFIG_SECCOMP
uint32_t hm_actv_local_seccomp_scno_of(void)
{
	return actv_local_k.seccomp_info.scno;
}

uint32_t hm_actv_local_seccomp_arch_of(void)
{
	return actv_local_k.seccomp_info.arch;
}

uint64_t hm_actv_local_seccomp_filter_of(void)
{
	hm_pacdb_data_chek(actv_local_k.seccomp_info.seccomp_filter,
			   (__u64)(uintptr_t)&actv_local_k.seccomp_info.seccomp_filter,
			   actv_local_k.seccomp_info.seccomp_filter_pacdb);
	return actv_local_k.seccomp_info.seccomp_filter;
}

uint64_t hm_actv_local_seccomp_cofilter_index_of(void)
{
	return actv_local_k.seccomp_info.seccomp_filter_coidx;
}

volatile struct __seccomp_info_s *hm_actv_local_seccomp_info_of(void)
{
	return &actv_local_k.seccomp_info;
}

uint64_t *hm_actv_local_seccomp_args_of(void)
{
	return (uint64_t *)(actv_local_k.seccomp_info.seccomp_args);
}

bool hm_actv_local_seccomp_need_check_thiscall(void)
{
	return (actv_local_k.seccomp_info.need_check_thiscall != 0);
}

void hm_actv_local_clear_seccomp_info(void)
{
	__u32 inited = actv_local_k.seccomp_info.inited;
	mem_zero_s(actv_local_k.seccomp_info);
	actv_local_k.seccomp_info.inited = inited;
}

void hm_actv_local_set_seccomp_info_inited(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_SECCOMP_INFO);
	actv_local_k.seccomp_info.inited = SECCOMP_MAGIC_VAL;
}

bool hm_actv_local_is_seccomp_info_inited(void)
{
	return actv_local_k.seccomp_info.inited == SECCOMP_MAGIC_VAL;
}
#else
void hm_actv_local_clear_seccomp_info(void) { }

bool hm_actv_local_seccomp_need_check_thiscall(void)
{
	return false;
}

void hm_actv_local_set_seccomp_info_inited(void) { }

bool hm_actv_local_is_seccomp_info_inited(void)
{
	return false;
}

uint64_t *hm_actv_local_seccomp_args_of(void)
{
	return NULL;
}

uint64_t hm_actv_local_seccomp_filter_of(void)
{
	return 0;
}

uint32_t hm_actv_local_seccomp_scno_of(void)
{
	return 0;
}

uint32_t hm_actv_local_seccomp_arch_of(void)
{
	return 0;
}

volatile struct __seccomp_info_s *hm_actv_local_seccomp_info_of(void)
{
	return NULL;
}
#endif /* CONFIG_SECCOMP */

static __thread char tname[HMCAP_THREAD_NAME_SIZE];
static struct __ctrlmem_pcpinfo_s *pcpinfo_addr = NULL;

static struct __ctrlmem_pcpinfo_s *get_pcpinfo_addr(void)
{
	if (pcpinfo_addr == NULL) {
#ifndef __HOST_LLT__
		struct __actvret_hmcall_proc_pcpinfo pret;
		mem_zero_s(pret);
		int err = actvcall_hmcall_proc_pcpinfo(&pret);
		if (err < 0) {
			hm_error("failed to get pcpinfo vaddr, %s\n", hmstrerror(err));
		}
		pcpinfo_addr = (struct __ctrlmem_pcpinfo_s *)(unsigned long)pret.pcpinfo_vaddr;
#else
		pcpinfo_addr = fake_pcpinfo_addr;
#endif
	}
	return pcpinfo_addr;
}

pid_t __hm_actv_local_thread_vtid_of(void)
{
#ifdef CONFIG_ACTV_LOCAL_AREA_THREAD_INFO
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_THREAD_INFO);
#endif
	return (__pcpinfo_current_thread_vtid(get_pcpinfo_addr()));
}

pid_t __hm_actv_local_thread_vpid_of(void)
{
	return (__pcpinfo_current_thread_vpid(get_pcpinfo_addr()));
}

pid_t __hm_actv_local_thread_tid_of(void)
{
#ifdef CONFIG_ACTV_LOCAL_AREA_THREAD_INFO
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_THREAD_INFO);
#endif
	return (__pcpinfo_current_thread_tid(get_pcpinfo_addr()));
}

pid_t __hm_actv_local_thread_pid_of(void)
{
	return (__pcpinfo_current_thread_pid(get_pcpinfo_addr()));
}

const char *__hm_actv_local_thread_name_of(void)
{
	__u64 gen;
	__u64 new_gen;
	unsigned int cpuid;
	unsigned int new_cpuid;
#ifdef CONFIG_ACTV_LOCAL_AREA_THREAD_INFO
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_THREAD_INFO);
#endif
	struct __ctrlmem_pcpinfo_s *pcpinfo = get_pcpinfo_addr();

	/* See Documentation/pcpinfo/pcpinfo_synchronization.md for more discussions */
	do {
		cpuid = __arch_current_cpuid();
		gen = __read_once(pcpinfo[cpuid].generation);
		(void)memcpy(tname, pcpinfo[cpuid].ptcb.name, HMCAP_THREAD_NAME_SIZE);
		new_cpuid = __arch_current_cpuid();
		new_gen = __read_once(pcpinfo[cpuid].generation);
	} while ((gen != new_gen) || (cpuid != new_cpuid));

	return (const char *)&tname[0];
}

uint32_t hm_actv_local_thread_cpu_of(void)
{
	return __arch_current_cpuid();
}

unsigned long long __hm_actv_local_thread_cref_of(void)
{
	return (__pcpinfo_current_thread_tcb_cref(get_pcpinfo_addr()));
}

unsigned long long __hm_actv_local_sched_thread_cref_of(void)
{
	return (__pcpinfo_current_thread_sched_tcb_cref(get_pcpinfo_addr()));
}

__u32 __hm_actv_local_thread_priority_of(void)
{
	return (__pcpinfo_current_thread_running_prio(get_pcpinfo_addr()));
}

__u32 __hm_actv_local_thread_kcov_idx_state(void)
{
	return (__pcpinfo_current_thread_kcov_idx_state(get_pcpinfo_addr()));
}

unsigned long long __hm_actv_local_thread_vskey_of(void)
{
	return (__pcpinfo_current_thread_vs_key(get_pcpinfo_addr()));
}

__u8 __hm_actv_local_thread_xdep_critical_of(void)
{
	return (__pcpinfo_current_thread_xdep_critical(get_pcpinfo_addr()));
}

#ifdef CONFIG_ACTV_SCHED_STAT
__u64 hm_actv_local_actv_ready_time_of(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_ACTV_INFO);
	return actv_local_k.actv_sched_info.ready_time;
}

__u64 hm_actv_local_actv_block_time_of(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_ACTV_INFO);
	return actv_local_k.actv_sched_info.block_time;
}

__u32 hm_actv_local_actv_ready_cnt_of(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_ACTV_INFO);
	return actv_local_k.actv_sched_info.ready_cnt;
}

__u32 hm_actv_local_actv_block_cnt_of(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_ACTV_INFO);
	return actv_local_k.actv_sched_info.block_cnt;
}

bool hm_actv_local_actv_is_need_reset(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_ACTV_INFO);
	return actv_local_k.actv_sched_info.need_reset == 1U;
}

void hm_actv_local_actv_set_need_reset(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_ACTV_INFO);
	actv_local_k.actv_sched_info.need_reset = 1U;
}

#else

__u64 hm_actv_local_actv_ready_time_of(void)
{
	return 0ULL;
}

__u64 hm_actv_local_actv_block_time_of(void)
{
	return 0ULL;
}

__u32 hm_actv_local_actv_ready_cnt_of(void)
{
	return 0U;
}

__u32 hm_actv_local_actv_block_cnt_of(void)
{
	return 0U;
}

bool hm_actv_local_actv_is_need_reset(void)
{
	return false;
}

void hm_actv_local_actv_set_need_reset(void) { }
#endif /* CONFIG_ACTV_SCHED_STAT */

#ifdef CONFIG_ACTV_LOCAL_AREA_SERVICE_INFO
uint64_t hm_actv_service_info(void)
{
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_SERVICE_INFO);
	struct __actv_local_area_k *temp_ptr = (struct __actv_local_area_k *)g_local_area_info.p_actv_local_k;

	return temp_ptr->service_info.base_credential;
}
#else
uint64_t hm_actv_service_info(void)
{
	return 0U;
}
#endif /* CONFIG_ACTV_LOCAL_AREA_SERVICE_INFO */

__u64 hm_actv_local_actv_cred_of(void)
{
	hm_pacdb_data_chek(actv_local_k.creds.cred, (__u64)(uintptr_t)&actv_local_k.creds.cred, actv_local_k.creds.pac_cred);
	return actv_local_k.creds.cred;
}

__u64 hm_actv_local_actv_xcred_of(void)
{
	hm_pacdb_data_chek(actv_local_k.creds.xact_cred, (__u64)(uintptr_t)&actv_local_k.creds.xact_cred,
		actv_local_k.creds.pac_xact);
	return actv_local_k.creds.xact_cred;
}

void *hm_actv_local_creds_addr_of(void)
{
	return (void *)&actv_local_k.creds;
}
