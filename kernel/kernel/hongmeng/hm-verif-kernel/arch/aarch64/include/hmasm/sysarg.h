/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 16 05:28:48 2019
 */

#ifndef AARCH64_ASM_SYSARG_H
#define AARCH64_ASM_SYSARG_H

#include <hmasm/aapcs.h>
#include <hmkernel/strict.h>
#include <hmkernel/uaccess.h>
#include <mapi/hmasm/sysarg.h>

static inline void
aarch32_sysarg_capcall_ref(u64 *pref, const struct arch_uctx *uctx)
{
	u32 hi, lo;
#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
	lo = (u32)REG_NORMAL(&uctx->regs, 0);
	hi = (u32)REG_NORMAL(&uctx->regs, 1);
#else
	hi = (u32)REG_NORMAL(&uctx->regs, 0);
	lo = (u32)REG_NORMAL(&uctx->regs, 1);
#endif
	aarch32_apcs_fetch_int(pref, sizeof(u64), hi, lo);
}

static inline int
arch_sysarg_capcall_ref(u64 *pref, const struct arch_uctx *uctx)
{
	if (((uctx->flags & ARCH_UCONTEXT_DATA_32BIT) != 0ULL) &&
	   ((uctx->flags & ARCH_UCONTEXT_EXEC_32BIT) != 0ULL)) {
		aarch32_sysarg_capcall_ref(pref, uctx);
	} else {
		/* we use arg0 for capref */
		((u64 *)(pref))[0] = uctx->regs.__regs[0];
	}
	return 0;
}

static inline int
aarch32_sysarg_capcall_fixed64(void *parg, size_t argsz, unsigned int pos,
			       const struct arch_uctx *uctx, bool is_fastpath,
			       struct arch_cpu_local_s *cpu_local)
{
	int err;
	u32 val1, val2;

	BUG_ON(argsz != 8UL);

	err = aarch32_apcs_pull_u32(&val1, pos, uctx,
				    is_fastpath, cpu_local);
	if (err == 0) {
		err = aarch32_apcs_pull_u32(&val2, pos + 1U, uctx,
					    is_fastpath, cpu_local);
	}

	if (err == 0) {
		/* combine two val to real arg */
#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
		aarch32_apcs_fetch_int(parg, argsz, val2, val1);
#else
		aarch32_apcs_fetch_int(parg, argsz, val1, val2);
#endif
	}

	return err;
}

static inline int
aarch32_sysarg_capcall_normal(void *parg, unsigned long argsz, unsigned int pos,
			      const struct arch_uctx *uctx,
			      bool is_signed, bool is_fastpath,
			      struct arch_cpu_local_s *cpu_local)
{
	int err;
	u32 val;

	err = aarch32_apcs_pull_u32(&val, pos, uctx,
				    is_fastpath, cpu_local);
	if (err == 0) {
		if (argsz == 8UL) {
			/* need to extend aarch32 args */
			if (is_signed) {
				ptr_from_void(parg, s64)[0] = (s64)(s32)val;
			} else {
				ptr_from_void(parg, u64)[0] = (u64)(u32)val;
			}
		} else {
			/* fetch <= 4 size arg */
			aarch32_apcs_fetch_int(parg, argsz, 0, val);
		}
	}

	return err;
}

static inline __always_inline int
aarch32_sysarg_capcall(void *parg, const unsigned long argsz[],
		       unsigned long *pstate,
		       const struct arch_uctx *uctx,
		       unsigned int argno, bool is_signed,
		       bool is_fixed64, bool is_fastpath,
		       struct arch_cpu_local_s *cpu_local)
{
	/* aarch32, 2 args for capref */
	unsigned int pos = (unsigned int)(*pstate + 2UL);
	size_t sz = (size_t)argsz[argno];
	int err = 0;

	if (is_fixed64) {
		/* according to aapcs, u64 arg is aligned to 8 */
		if ((pos & 1U) == 1U) {
			pos += 1U;
			*pstate += 1UL;
		}

		err = aarch32_sysarg_capcall_fixed64(parg, sz, pos, uctx,
						     is_fastpath, cpu_local);
		/* consume 2 u32 */
		*pstate += 2UL;
	} else {
		/* consume 1 u32 */
		err = aarch32_sysarg_capcall_normal(parg, sz, pos, uctx, is_signed,
						    is_fastpath, cpu_local);
		*pstate += 1UL;
	}

	return err;
}

static inline __always_inline int
aarch64_sysarg_capcall(void *parg, unsigned long argsz[], unsigned long *pstate,
		       const struct arch_uctx *uctx, unsigned int argno)
{
	/* aarch64, 1 arg for capref */
	unsigned long call_argno = *pstate + 1UL;
	u64 __user *stack_ptr = NULL;
	int err = 0;
	u64 reg;

	if (call_argno < 8UL) {
		reg = uctx->regs.__regs[call_argno];
		aarch64_aapcs_fetch_int(parg, argsz[argno], reg);
	} else {
		stack_ptr = u64_to_ptr(REG_SP(&uctx->regs) + (call_argno - 8UL) * sizeof(u64),
				       u64 __user);
		err = copy_from_user(&reg, stack_ptr, sizeof(u64));
		if (err == 0) {
			aarch64_aapcs_fetch_int(parg, argsz[argno], reg);
		}
	}

	(*pstate)++;

	return err;
}

static inline __always_inline int
ilp32_sysarg_capcall(void *parg, unsigned long argsz[], unsigned long *pstate,
		     const struct arch_uctx *uctx, unsigned int argno,
		     bool is_signed, bool is_fixed64)
{
	unsigned long call_argno = *pstate + 1UL;
	u64 __user *stack_ptr = NULL;
	int err = 0;
	u64 reg;

	if (call_argno < 8UL) {
		reg = uctx->regs.__regs[call_argno];
		ilp32_aapcs_fetch_int(parg, argsz[argno], reg,
				      is_signed, is_fixed64);
	} else {
		stack_ptr = u64_to_ptr(REG_SP(&uctx->regs) + (call_argno - 8UL) * sizeof(u64),
				       u64 __user);
		err = copy_from_user(&reg, stack_ptr, sizeof(u64));
		if (err == 0) {
			ilp32_aapcs_fetch_int(parg, argsz[argno], reg,
					      is_signed, is_fixed64);
		}
	}

	(*pstate)++;

	return err;
}

#if !defined(CONFIG_COMPAT) && !defined(CONFIG_ILP32)
static inline int
arch_sysarg_capcall(void *parg, unsigned long argsz[],
		    unsigned long *pstate,
		    const struct arch_uctx *uctx,
		    unsigned int argno, bool is_signed,
		    bool is_fixed64, bool is_fastpath,
		    struct arch_cpu_local_s *cpu_local)
{
	UNUSED(is_signed, is_fixed64, is_fastpath, cpu_local);

	return aarch64_sysarg_capcall(parg, argsz, pstate, uctx, argno);
}
#else
static inline int
arch_sysarg_capcall(void *parg, unsigned long argsz[],
		    unsigned long *pstate,
		    const struct arch_uctx *uctx,
		    unsigned int argno, bool is_signed,
		    bool is_fixed64, bool is_fastpath,
		    struct arch_cpu_local_s *cpu_local)
{
	int err = 0;
	bool is_data32 = ((uctx->flags & ARCH_UCONTEXT_DATA_32BIT) != 0ULL);
	bool is_exec32 = ((uctx->flags & ARCH_UCONTEXT_EXEC_32BIT) != 0ULL);

	if (is_data32 && is_exec32) {
		/* user context is aarch32 */
		err = aarch32_sysarg_capcall(parg, argsz, pstate, uctx, argno,
					     is_signed, is_fixed64,
					     is_fastpath, cpu_local);
	} else if (is_data32 && !is_exec32) {
		/* user context is ilp32 */
		err = ilp32_sysarg_capcall(parg, argsz, pstate, uctx, argno,
					   is_signed, is_fixed64);
	} else {
		/* user context is aarch64 default */
		err = aarch64_sysarg_capcall(parg, argsz, pstate, uctx, argno);
	}

	return err;
}
#endif

#endif
