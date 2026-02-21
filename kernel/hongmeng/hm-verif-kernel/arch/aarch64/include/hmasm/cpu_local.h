/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Cpu local
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 25 12:16:22 2018
 */
#ifndef AARCH64_ASM_CPU_LOCAL_H
#define AARCH64_ASM_CPU_LOCAL_H

#include <hmasm/cpu/boot.h>
#include <hmasm/processor.h>
#include <hmasm/ucontext.h>
#include <hmasm/memory.h>
#include <hmasm/pac.h>
#include <hmasm/dabrt_hook.h>
#include <hmkernel/rq.h>
#include <hmkernel/types.h>
#include <hmkernel/kernel.h>
#include <hmkernel/lock_trace.h>

#include <hmkernel/mm/kstack.h>
#include <hmkernel/bitops/bitops.h>
#include <hmkernel/events/kev_trace_struct.h>

#include <hmkernel/atomic.h>
#include <hmkernel/krandom.h>
#include <hmkernel/interrupt/kvic_struct.h>
#include <lib/trace/trace_misc.h>

#include <hmkernel/mm/kstack.h>

/* IRQ bit only serves for state assertion and resched masking */
#define ARCH_RESTORE_FLAG_A64_IRQ_OFF	(1UL << 16UL)
#define ARCH_RESTORE_FLAG_A64_FIQ_OFF	(1UL << 17UL)
#define ARCH_RESTORE_FLAG_A64_SS_ON	(1UL << 18UL)

/* Get stack top address from cpu_local's address */
#define __cpu_local_to_sp(va_cpu_local) (ptr_add((va_cpu_local), KSTACK_REGION_OFFSET_MAIN_STACK + \
								 KSTACK_REGION_SIZE_MAIN_STACK))

/* Get exception irq stack top address from cpu_local's address */
#define __cpu_local_to_ex_irq_sp(va_cpu_local) (ptr_add((va_cpu_local), KSTACK_REGION_OFFSET_EXCEPTION_IRQ_STACK + \
								    KSTACK_REGION_SIZE_EXCEPTION_IRQ_STACK))

/* Get exception fiq stack top address from cpu_local's address */
#define __cpu_local_to_ex_fiq_sp(va_cpu_local) (ptr_add((va_cpu_local), KSTACK_REGION_OFFSET_EXCEPTION_FIQ_STACK + \
								    KSTACK_REGION_SIZE_EXCEPTION_FIQ_STACK))

/* Get exception fiq stack top address from cpu_local's address */
#define __cpu_local_to_ex_sp(va_cpu_local) (ptr_add((va_cpu_local), KSTACK_REGION_OFFSET_EXCEPTION_STACK + \
								    KSTACK_REGION_SIZE_EXCEPTION_STACK))

/*
 * 35 64-bit registers and 8 bytes padding,
 * registers contain x0~x30, spsr, pc, sp and tpidr
 */
#ifdef PAC_DFI_KERNEL
#define FRAME_SIZE sizeof(struct arch_uctx)
#else
#define FRAME_SIZE sizeof(struct arch_regs)
#endif

/*
 * the address of the first frame pushed into irq stack,
 * which starts at the middle of exception stack
 */
#define __cpu_local_to_irq_stack_fp(va_cpu_local) \
	(ptr_add((va_cpu_local), KSTACK_REGION_OFFSET_EXCEPTION_IRQ_STACK + \
				KSTACK_REGION_SIZE_EXCEPTION_IRQ_STACK - FRAME_SIZE))

struct actv_s;
struct ctrlmem_kobj_vspace_s;
/* first element must be kernel stack of current cpu */
struct arch_cpu_local_s {
	void* kstack;
	unsigned int cpu;
	struct actv_s *_curr_actv;
	struct tcb_s *curr_tcb;
#ifdef CONFIG_PAC
	struct pac_keys_cpu pac_key;
#endif
	unsigned int syscall_phase;
	u64 nr_ctx_switches;
	u64 nr_interrupts;

	/* store waker for kev_unblock */
	const char *waker;

	u64 idmap;
	struct cpuinitctx_s cpuinitctx;

#ifdef CONFIG_SMP
	vatomic32_t nr_iowaits;
	struct sched_rt_info_s sched_rt_info;
#else
	u32 nr_iowaits;
#endif
	/* store the curr_pool which is serving for actv push */
	struct actv_pool_s *curr_pool;

	unsigned long restore_action;

	/* indicate kev notify bitmap empty or not */
	bool kev_notify_action;
#ifdef CONFIG_VCPU_FPSIMD_LAZY
	struct tcb_s *fpu_owner;
#endif
	/* each bit indicates a kev rb notification */
	DEFINE_BITMAP(kev_rb_nofity, KEV_NR);

	/* indicate fire the kvic_trigger when user_context_switch */
	struct __kvic_postproc_data kvic_trigger_data;

	struct dabrt_hook dabrt_hook;
#ifdef CONFIG_PM_SCHED_INDICATOR_TRIGGER
	bool sched_ind_trigger_need_fire;
#endif

#ifdef CONFIG_HMPERF_NG
	struct dlist_node perf_wakeup_list;
#endif

#ifdef CONFIG_ARM_UNALIGNED_ACCESS_EMU
	u64 dabort_status;
#endif

#ifdef CONFIG_ACTV_POOL_WITH_THREADS
	/*
	 * Record tcbs that are used to provide Actv RPC services, and
	 * try to wake up and delete them from the actvpool.
	 */
	struct dlist_node actvpool_intr_tcb_list;
#endif

#ifdef CONFIG_TRACEMGR
	/* indicate trace notify action */
	bool trace_notify_action;
	DEFINE_BITMAP(trace_futex_notify, TRACE_RMQ_FUTEX_NR);
#endif

	struct random_state pcp_random_state;

	vatomicptr(struct ctrlmem_kobj_vspace_s *) curr_vspace_kpgf;

#ifdef CONFIG_KLOCK_TRACE
	/* make sure this is the last member of the arch_cpu_local_s */
	struct lock_buf_s lock_buf;
#endif
};

static_assert(CPU_LOCAL_SIZE >= sizeof(struct arch_cpu_local_s), size_of_arch_cpu_local_s_too_large);

extern void arch_cpu_local_setup(unsigned int cpu);
extern struct arch_cpu_local_s* __pure arch_cpu_local_load(void);
static inline struct arch_cpu_local_s* __pure arch_cpu_local(unsigned int cpu);
static inline struct arch_cpu_local_s* __pure arch_current_cpu_local(void);

#ifndef __HOST_LLT__

extern int __stack_top[];

static inline bool
arch_is_native_kernel_mode(void)
{
	return (read_sysreg("SPSel") & 0x01) == 0x01;
}

static inline __always_inline __pure struct arch_cpu_local_s *
arch_current_cpu_local(void)
{
	struct arch_cpu_local_s *cpu_local = NULL;
	/*
	 * Do not use VA_KSTACK_START because VA_KSTACK_START require a global
	 * variable access when KASLR is on.
	 */
	cpu_local = arch_cpu_local_load();
	return cpu_local;
}

static inline struct arch_cpu_local_s *
arch_cpu_local(unsigned int cpu)
{
	return u64_to_ptr((u64)(VA_KSTACK_START + (u64)(KERNEL_STACK_REGION_SIZE) * cpu),
			  struct arch_cpu_local_s);
}
#endif

JAM_INLINE struct actv_s *
tcb_curr_actv_of_nolock(const struct tcb_s *tcb);

static inline struct actv_s *
arch_actv_of_cpu_local(const struct arch_cpu_local_s *cpu_local)
{
	return tcb_curr_actv_of_nolock(cpu_local->curr_tcb);
}

static inline struct actv_s *
arch_raw_actv_of_cpu_local(const struct arch_cpu_local_s *cpu_local)
{
	return cpu_local->_curr_actv;
}

JAM_INLINE void arch_set_actv_of_cpu_local(struct arch_cpu_local_s *cpu_local,
					   struct actv_s *actv);

static inline struct actv_s *
arch_actv_of_current_cpu(void)
{
	struct arch_cpu_local_s *cpu_local = arch_current_cpu_local();

	return arch_actv_of_cpu_local(cpu_local);
}

static inline void
arch_set_actv_of_current_cpu(struct actv_s *actv)
{
	struct arch_cpu_local_s *cpu_local = arch_current_cpu_local();
	arch_set_actv_of_cpu_local(cpu_local, actv);
}

static inline void
arch_cpu_set_restore_action(struct arch_cpu_local_s *cpu_local, unsigned long action)
{
	cpu_local->restore_action |= action;
}

static inline void
arch_cpu_clr_restore_action(struct arch_cpu_local_s *cpu_local, unsigned long action)
{
	cpu_local->restore_action &= ~action;
}

static inline bool
arch_cpu_tst_restore_action(struct arch_cpu_local_s *cpu_local, unsigned long action)
{
	return __IS(cpu_local->restore_action & action);
}

static inline bool
arch_curr_user_irq_is_unmasked(void)
{
	return !arch_cpu_tst_restore_action(arch_current_cpu_local(),
					   ARCH_RESTORE_FLAG_A64_IRQ_OFF);
}

static inline void
arch_curr_switch_tcb_current(struct tcb_s *next)
{
	struct arch_cpu_local_s *cpu_local = arch_current_cpu_local();
#ifdef CONFIG_DEBUG_BUILD
	BUG_ON(arch_cpu_tst_restore_action(cpu_local, ARCH_RESTORE_FLAG_A64_IRQ_OFF));
#endif
	cpu_local->curr_tcb = next;
}

/*
 * To support usage of IRQ mask/unmask in privilege server, kernel does not change the state of
 * IRQ when returing to user state.
 */
static inline void
arch_restore_daif(struct arch_uctx *ctx, const struct arch_cpu_local_s *cpu_local)
{
	unsigned long action = cpu_local->restore_action;
	unsigned long spsr = REG_SPSR(&ctx->regs);

	if (unlikely((action & ARCH_RESTORE_FLAG_A64_SS_ON) != 0UL)) {
		spsr |= PSR_SS_BIT;
	} else {
		spsr &= ~PSR_SS_BIT;
	}

	REG_SPSR(&ctx->regs) = spsr;
	arch_uctx_pacga(ctx);
}

static inline void
arch_save_daif(const struct arch_uctx *ctx, struct arch_cpu_local_s *cpu_local)
{
	unsigned long spsr = REG_SPSR(&ctx->regs);
	if (unlikely(__IS(spsr & (PSR_I_BIT)))) {
		arch_cpu_set_restore_action(cpu_local,
				ARCH_RESTORE_FLAG_A64_IRQ_OFF);
	} else {
		arch_cpu_clr_restore_action(cpu_local,
				ARCH_RESTORE_FLAG_A64_IRQ_OFF);
	}
}

static inline struct actv_pool_s *
arch_curr_pool_of_curr_cpu(void)
{
	struct arch_cpu_local_s *cpu_local = arch_current_cpu_local();
	return cpu_local->curr_pool;
}

static inline void
arch_set_curr_pool_of_curr_cpu(struct actv_pool_s *pool, struct arch_cpu_local_s *cpu_local)
{
	cpu_local->curr_pool = pool;
}

unsigned int __arch_current_cpu_id(void);

static inline void
arch_inc_interrupt_of_cpu(unsigned int cpu)
{
	struct arch_cpu_local_s *cpu_local = arch_cpu_local(cpu);

	cpu_local->nr_interrupts += 1U;
}

static inline u64
arch_interrupt_of_cpu(unsigned int cpu)
{
	struct arch_cpu_local_s *cpu_local = arch_cpu_local(cpu);

	return cpu_local->nr_interrupts;
}

static inline void
arch_inc_ctx_switch_of_cpu_local(struct arch_cpu_local_s *cpu_local)
{
#define PER_CONTEXT_SWITCH 2U
	cpu_local->nr_ctx_switches += PER_CONTEXT_SWITCH;
#undef PER_CONTEXT_SWITCH
}

static inline void
arch_inc_ctx_switch_of_cpu(unsigned int cpu)
{
	struct arch_cpu_local_s *cpu_local = arch_cpu_local(cpu);
	arch_inc_ctx_switch_of_cpu_local(cpu_local);
}

static inline u64
arch_ctx_switch_of_cpu(unsigned int cpu)
{
	struct arch_cpu_local_s *cpu_local = arch_cpu_local(cpu);

	return cpu_local->nr_ctx_switches;
}

#ifdef CONFIG_ARM_UNALIGNED_ACCESS_EMU
static inline u64
arch_dabort_status_of_current_cpu(void)
{
	struct arch_cpu_local_s *cpu_local = arch_current_cpu_local();
	return cpu_local->dabort_status;
}

static inline void
arch_set_dabort_status_of_current_cpu(u64 dabort_status)
{
	struct arch_cpu_local_s *cpu_local = arch_current_cpu_local();
	cpu_local->dabort_status = dabort_status;
}
#else
static inline u64 arch_dabort_status_of_current_cpu(void)
{
	return 0;
}

static inline void
arch_set_dabort_status_of_current_cpu(u64 dabort_status)
{
	UNUSED(dabort_status);
}
#endif /* !CONFIG_ARM_UNALIGNED_ACCESS_EMU */

#ifdef CONFIG_VCPU_FPSIMD_LAZY
static inline void
arch_fpu_owner_set(unsigned int cpu, struct tcb_s *owner)
{
	struct arch_cpu_local_s *cpu_local = arch_cpu_local(cpu);
	cpu_local->fpu_owner = owner;
}

static inline struct tcb_s *
arch_fpu_owner_get(unsigned int cpu)
{
	struct arch_cpu_local_s *cpu_local = arch_cpu_local(cpu);
	return cpu_local->fpu_owner;
}
#endif

#endif
