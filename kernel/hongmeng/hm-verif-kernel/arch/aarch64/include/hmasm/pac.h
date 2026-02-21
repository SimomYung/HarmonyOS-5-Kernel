/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header about PAC
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 29 06:35:02 2023
 */
#ifndef A64_ASM_PAC_H
#define A64_ASM_PAC_H

#include <mapi/hmasm/ucontext.h>
#include <hmasm/cpu/boot.h>
#include <hmasm/memory.h>
#include <mapi/hmasm/pac.h>

#define PAC_CTX_OFFSET 0x130 /* sizeof(struct arch_uctx) */
#define PAC_CPU_LOCAL_OFFSET 0x20

#define PAC_CTRL_KEY_IA __U(0)
#define PAC_CTRL_KEY_IB __U(1)
#define PAC_CTRL_KEY_DA __U(2)
#define PAC_CTRL_KEY_DB __U(3)
#define PAC_CTRL_KEY_GA __U(4)

#define PAC_CTRL_KEY_IA_BIT (__U(1) << PAC_CTRL_KEY_IA)
#define PAC_CTRL_KEY_IB_BIT (__U(1) << PAC_CTRL_KEY_IB)
#define PAC_CTRL_KEY_DA_BIT (__U(1) << PAC_CTRL_KEY_DA)
#define PAC_CTRL_KEY_DB_BIT (__U(1) << PAC_CTRL_KEY_DB)
#define PAC_CTRL_KEY_GA_BIT (__U(1) << PAC_CTRL_KEY_GA)

#define PAC_CTRL_EN_KEYS (PAC_CTRL_KEY_IA_BIT | PAC_CTRL_KEY_IB_BIT | \
			  PAC_CTRL_KEY_DA_BIT | PAC_CTRL_KEY_DB_BIT)
#define PAC_CTRL_KEYS (PAC_CTRL_EN_KEYS | PAC_CTRL_KEY_GA_BIT)

#define PAC_KEYS_KERNEL_IA __U(0)
#define PAC_KEYS_KERNEL_DA __U(1)
#define PAC_KEYS_KERNEL_GA __U(2)
#ifdef CONFIG_PAC_SAME_KEY
#define PAC_KEYS_KERNEL_DB __U(3)
#define PAC_KEYS_KERNEL_IB __U(4)
#else
#define PAC_KEYS_KERNEL_IB __U(3)
#endif

#ifdef __ASSEMBLY__

#ifdef CONFIG_PAC
.macro __switch_stack_key_lasp
/* Do NOT switch IB key when all CPUs share the same one */
#ifndef CONFIG_KDP_DATA_INVISIBLE
	/* IB key is per cpu */
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_IB)]
	msr APIBKeyLo_EL1, x2
	msr APIBKeyHi_EL1, x3
#endif
.endm

.macro __switch_stack_key_normal
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_IB)]
	msr APIBKeyLo_EL1, x2
	msr APIBKeyHi_EL1, x3
.endm

.macro __switch_keys_lasp
/* switch keys when privileged services DO NOT share the same keys */
#ifndef CONFIG_PAC_SAME_KEY
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_IA)]
	msr APIAKeyLo_EL1, x2
	msr APIAKeyHi_EL1, x3
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_DA)]
	msr APDAKeyLo_EL1, x2
	msr APDAKeyHi_EL1, x3
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_GA)]
	msr APGAKeyLo_EL1, x2
	msr APGAKeyHi_EL1, x3
#endif
.endm

.macro __switch_keys_normal
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_IA)]
	msr APIAKeyLo_EL1, x2
	msr APIAKeyHi_EL1, x3
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_DA)]
	msr APDAKeyLo_EL1, x2
	msr APDAKeyHi_EL1, x3
	ldp x2, x3, [sp, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_GA)]
	msr APGAKeyLo_EL1, x2
	msr APGAKeyHi_EL1, x3
.endm

.macro pac_cntx_switch_to_user suffix
	/* x21: el1, sp: uctx/actv */
#ifdef CONFIG_PAC_SAME_KEY
	/* el1 switch to el1 and privileged services share the same keys,
	 * only switch IB key on stack */
	tbnz x21, #55, switch_stack_key_\suffix
#else
# if defined(CONFIG_PRIVILEGED_SERVICE) && !defined(CONFIG_PAC_ALWAYS_ENABLE)
	/* el1 switch to el1 and privileged services DO NOT share the same keys,
	 * switch all keys */
	tbnz x21, #55, switch_keys_\suffix
# endif
#endif
#ifndef CONFIG_PAC_ALWAYS_ENABLE
	ldr x2, [sp, (PAC_CTX_OFFSET + 16 * 5)]
	mov x3, #0x2000
	movk x3, #0xc800, lsl #16
	and x2, x2, x3
	cmp x2, x3
	bne switch_sctlr_\suffix
#endif
switch_keys_\suffix:
	__switch_keys_\suffix
switch_stack_key_\suffix:
	__switch_stack_key_\suffix
	/* DB is not used by kernel, DB are switched in actv_cpuctx_sched_in */
#ifndef CONFIG_PAC_ALWAYS_ENABLE
	b switch_end_\suffix
switch_sctlr_\suffix:
	mvn x3, x3
	mrs x4, sctlr_el1
	and x4, x4, x3
	orr x4, x4, x2
	msr sctlr_el1, x4
	b switch_keys_\suffix
#endif
switch_end_\suffix:
.endm

.macro pac_cntx_switch_to_kernel el
.ifc \el, el0
	bl pac_cntx_user_to_kernel
.else
#ifndef CONFIG_PAC_SAME_KEY
	bl pac_cntx_user_to_kernel
#endif
.endif
.endm

.macro pac_cntx_switch_to_sysmgr
#ifndef CONFIG_PAC_SAME_KEY
	ldp x4, x5, [x1, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_IA)]
	msr APIAKeyLo_EL1, x4
	msr APIAKeyHi_EL1, x5
#endif
#ifndef CONFIG_KDP_DATA_INVISIBLE
	/* Do NOT switch IB key when all CPUs share the same one */
	ldp x4, x5, [x1, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_IB)]
	msr APIBKeyLo_EL1, x4
	msr APIBKeyHi_EL1, x5
#endif
#ifndef CONFIG_PAC_SAME_KEY
	ldp x4, x5, [x1, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_DA)]
	msr APDAKeyLo_EL1, x4
	msr APDAKeyHi_EL1, x5
	ldp x4, x5, [x1, (PAC_CTX_OFFSET + 16 * __ARCH_REGS_PAUTH_GA)]
	msr APGAKeyLo_EL1, x4
	msr APGAKeyHi_EL1, x5
#endif
	/* DB is not used by kernel, DB are switched in actv_cpuctx_sched_in */
	isb
.endm

#ifndef CONFIG_KDP_DATA_INVISIBLE
.macro pac_cntx_cpu_restore
	/* x0 cpuinitctx saved registers */
	ldp x3, x4, [x0, (CPUINITCTX_SYS_REGS_APIAKEYLO_EL1 * 8)]
	msr APIAKeyLo_EL1, x3
	msr APIAKeyHi_EL1, x4
	ldp x3, x4, [x0, (CPUINITCTX_SYS_REGS_APIBKEYLO_EL1 * 8)]
	msr APIBKeyLo_EL1, x3
	msr APIBKeyHi_EL1, x4
	ldp x3, x4, [x0, (CPUINITCTX_SYS_REGS_APDAKEYLO_EL1 * 8)]
	msr APDAKeyLo_EL1, x3
	msr APDAKeyHi_EL1, x4
	ldp x3, x4, [x0, (CPUINITCTX_SYS_REGS_APDBKEYLO_EL1 * 8)]
	msr APDBKeyLo_EL1, x3
	msr APDBKeyHi_EL1, x4
	ldp x3, x4, [x0, (CPUINITCTX_SYS_REGS_APGAKEYLO_EL1 * 8)]
	msr APGAKeyLo_EL1, x3
	msr APGAKeyHi_EL1, x4
	/* caller use ISB */
.endm
#else
.macro pac_cntx_cpu_restore
	adrp x0, keys_kernel
	add x0, x0, #:lo12:keys_kernel
	/* x0: keys_kernel */

	ldp x3, x4, [x0, #16 * PAC_KEYS_KERNEL_IA]
	msr APIAKeyLo_EL1, x3
	msr APIAKeyHi_EL1, x4

	ldp x3, x4, [x0, #16 * PAC_KEYS_KERNEL_DA]
	msr APDAKeyLo_EL1, x3
	msr APDAKeyHi_EL1, x4

	ldp x3, x4, [x0, #16 * PAC_KEYS_KERNEL_GA]
	msr APGAKeyLo_EL1, x3
	msr APGAKeyHi_EL1, x4

	ldp x3, x4, [x0, #16 * PAC_KEYS_KERNEL_IB]
	msr APIBKeyLo_EL1, x3
	msr APIBKeyHi_EL1, x4

	/* PAC_SAME_KEY must be y */
	ldp x3, x4, [x0, #16 * PAC_KEYS_KERNEL_DB]
	msr APDBKeyLo_EL1, x3
	msr APDBKeyHi_EL1, x4
.endm
#endif

.macro pac_cntx_cpu_snapshot
#ifndef CONFIG_KDP_DATA_INVISIBLE
	/* x0 cpuinitctx system registers base */
	mrs x1, APIAKeyLo_EL1
	mrs x2, APIAKeyHi_EL1
	stp x1, x2, [x0, ((CPUINITCTX_SYS_REGS_APIAKEYLO_EL1 - CPUINITCTX_SYS_REGS_BASE) * 8)]
	mrs x1, APIBKeyLo_EL1
	mrs x2, APIBKeyHi_EL1
	stp x1, x2, [x0, ((CPUINITCTX_SYS_REGS_APIBKEYLO_EL1 - CPUINITCTX_SYS_REGS_BASE) * 8)]
	mrs x1, APDAKeyLo_EL1
	mrs x2, APDAKeyHi_EL1
	stp x1, x2, [x0, ((CPUINITCTX_SYS_REGS_APDAKEYLO_EL1 - CPUINITCTX_SYS_REGS_BASE) * 8)]
	mrs x1, APDBKeyLo_EL1
	mrs x2, APDBKeyHi_EL1
	stp x1, x2, [x0, ((CPUINITCTX_SYS_REGS_APDBKEYLO_EL1 - CPUINITCTX_SYS_REGS_BASE) * 8)]
	mrs x1, APGAKeyLo_EL1
	mrs x2, APGAKeyHi_EL1
	stp x1, x2, [x0, ((CPUINITCTX_SYS_REGS_APGAKEYLO_EL1 - CPUINITCTX_SYS_REGS_BASE) * 8)]
#endif
.endm

.macro prefetch_regs
	prfm pldl1keep, [sp, #64 * 0]
	prfm pldl1keep, [sp, #64 * 1]
	prfm pldl1keep, [sp, #64 * 2]
	prfm pldl1keep, [sp, #64 * 3]
.endm

#else /* CONFIG_PAC */

.macro pac_cntx_switch_to_kernel el
.endm
.macro pac_cntx_switch_to_user suffix
.endm
.macro pac_cntx_switch_to_sysmgr
.endm
.macro pac_cntx_cpu_restore
.endm
.macro pac_cntx_cpu_snapshot
.endm
.macro prefetch_regs
.endm

#endif /* CONFIG_PAC */

#else /* __ASSEMBLY__ */

#include <hmasm/types.h>
#include <hmkernel/types.h>

struct CNode_s;
struct tcb_s;
struct actv_s;
struct actv_pool_s;

int pac_thread_ctrl(struct tcb_s *tcb, unsigned int command, __u64 arg, struct CNode_s *cnode);
int pac_actv_ctrl(struct actv_s *actv, unsigned int command, __u64 arg, struct CNode_s *cnode);
int pac_actv_pool_ctrl(struct actv_pool_s *pool, unsigned int command, __u64 arg, struct CNode_s *cnode);
void pac_uctx_copy(struct arch_secret_uctx *to, const struct arch_secret_uctx *from);

struct ptrauth_key {
	u64 lo, hi;
};

/* IA/DA/GA keys are global, and we give each cpu its own IB. */
struct pac_keys_kernel {
	struct ptrauth_key apia;
	struct ptrauth_key apda;
	struct ptrauth_key apga;
#ifdef CONFIG_PAC_SAME_KEY
	struct ptrauth_key apdb;
#endif
#ifdef CONFIG_KDP_DATA_INVISIBLE
	/* all CPUs share the same IB key when it's stored in kdp invisible zone */
	struct ptrauth_key apib;
#else
	struct ptrauth_key apibs[CONFIG_NR_CPUS];
#endif
};
static_assert(__builtin_offsetof(struct pac_keys_kernel, apia) ==
		PAC_KEYS_KERNEL_IA * sizeof(struct ptrauth_key), ia_key_is_0);
static_assert(__builtin_offsetof(struct pac_keys_kernel, apda) ==
		PAC_KEYS_KERNEL_DA * sizeof(struct ptrauth_key), da_key_is_1);
static_assert(__builtin_offsetof(struct pac_keys_kernel, apga) ==
		PAC_KEYS_KERNEL_GA * sizeof(struct ptrauth_key), ga_key_is_2);
#ifdef CONFIG_KDP_DATA_INVISIBLE
static_assert(__builtin_offsetof(struct pac_keys_kernel, apib) ==
		PAC_KEYS_KERNEL_IB * sizeof(struct ptrauth_key), ib_key_is_3);
#endif

struct pac_keys_cpu {
	struct ptrauth_key apib;
};

extern struct pac_keys_kernel keys_kernel;

void pac_setup(void);
void pac_enable_cpu_keys(bool completed);
struct arch_cpu_local_s;
void pac_cpu_key_init(struct arch_cpu_local_s *cpu_local);
void pac_thread_inherit_kernel(struct tcb_s *tcb);
void pac_ctx_switch(const struct actv_s *actv);

#define __stringify(x) #x
#define __ptrauth_op(o, k) __stringify(o##k)

#endif /* __ASSEMBLY__ */
#endif
