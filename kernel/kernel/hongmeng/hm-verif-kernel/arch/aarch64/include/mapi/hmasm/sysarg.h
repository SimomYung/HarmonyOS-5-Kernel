/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Definition of sysarg for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 10:06:52 2021
 */
#ifndef MAPI_AARCH64_ASM_SYSARG_H
#define MAPI_AARCH64_ASM_SYSARG_H

#include <hmasm/aapcs.h>
#include <hmasm/ucontext.h>
#include <hmasm/lsyscall_ucontext.h>
#include <hmkernel/uaccess.h>

struct arch_cpu_local_s;

static inline int
aarch32_sysarg_fast(void *parg, size_t argsz,
		    const struct arch_uctx *uctx,
		    unsigned long argno, bool is_signed,
		    bool is_fixed64, bool is_fastpath,
		    struct arch_cpu_local_s *cpu_local)
{
	int err = 0;
	u32 arg;

	/* fast syscall must not using fixed64 type,
	 * According to APCS:
	 */
	BUG_ON(is_fixed64);

	/* ARM32 Only R0~R3 Used for function parameter transfer */
	if (argno < 4 || arch_uctx_is_lsyscall(uctx)) { /* lsyscall always use regs to pass args */
		arg = (u32)REG_NORMAL(&uctx->regs, argno);
	} else if (!is_fastpath) {
		err = copy_from_user(&arg,
				     u64_to_uptr(REG_SP(&uctx->regs) + (argno - 4UL) * 4UL),
				     sizeof(arg));
	} else {
		void __user *uptr = u64_to_uptr(REG_SP(&uctx->regs) + (argno - 4UL) * sizeof(u32));
		u32 *ptr = ptr_from_void(uptr, u32);
		fastpath_check_user_access(actv_kobjvs_of(actv_of_uctx(uctx)), UACCESS_DIRECTION_U2K, cpu_local,
					   uptr, sizeof(u32));
		arg = *ptr;
		err = 0;
	}

	if (argsz == 8UL) {
		/* need to extend aarch32 args */
		if (is_signed) {
			ptr_from_void(parg, s64)[0] = (s64)(s32)arg;
		} else {
			ptr_from_void(parg, u64)[0] = (u64)(u32)arg;
		}
	} else {
		/* normal fetch int from aarch32 arg */
		aarch32_apcs_fetch_int(parg, argsz, 0, arg);
	}
	return err;
}

static inline int
aarch64_sysarg_fast(void *parg, size_t argsz, u64 reg)
{
	/* see aapcs, a64 case is very simple */
	aarch64_aapcs_fetch_int(parg, argsz, reg);
	return 0;
}

static inline int
ilp32_sysarg_fast(void *parg, size_t argsz, u64 reg,
		  bool is_signed, bool is_fixed64)
{
	ilp32_aapcs_fetch_int(parg, argsz, reg, is_signed, is_fixed64);
	return 0;
}

#if !defined(CONFIG_COMPAT) && !defined(CONFIG_ILP32)
static inline int
arch_sysarg_fast(void *parg, size_t argsz,
		 const struct arch_uctx *uctx,
		 unsigned long argno, bool is_signed,
		 bool is_fixed64, bool is_fastpath,
		 const struct arch_cpu_local_s *cpu_local)
{
	UNUSED(is_signed, is_fixed64, is_fastpath, cpu_local);

	return aarch64_sysarg_fast(parg, argsz, uctx->regs.__regs[argno]);
}
#else
static inline int
arch_sysarg_fast(void *parg, size_t argsz,
		 const struct arch_uctx *uctx,
		 unsigned long argno, bool is_signed,
		 bool is_fixed64, bool is_fastpath,
		 struct arch_cpu_local_s *cpu_local)
{
	BUG_ON(argno > 7UL);

	int err;
	bool is_data32 = ((uctx->flags & ARCH_UCONTEXT_DATA_32BIT) != 0ULL);
	bool is_exec32 = ((uctx->flags & ARCH_UCONTEXT_EXEC_32BIT) != 0ULL);

	if (is_data32 && is_exec32) {
		/* user context is aarch32 */
		err = aarch32_sysarg_fast(parg, argsz, uctx, argno, is_signed,
					  is_fixed64, is_fastpath, cpu_local);
	} else if (is_data32 && !is_exec32) {
		/* user context is ilp32 */
		err = ilp32_sysarg_fast(parg, argsz, uctx->regs.__regs[argno],
					is_signed, is_fixed64);
	} else {
		/* user context is aarch64 default */
		err = aarch64_sysarg_fast(parg, argsz, uctx->regs.__regs[argno]);
	}

	return err;
}
#endif

#endif
