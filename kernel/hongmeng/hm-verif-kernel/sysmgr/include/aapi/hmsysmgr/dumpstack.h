/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definitions about dumpstack shared for sysmgr and other uapp
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 29 15:24:17 2019
 */
#ifndef SYSMGR_INCLUDE_AAPI_HMSYSMGR_DUMPSTACK_H
#define SYSMGR_INCLUDE_AAPI_HMSYSMGR_DUMPSTACK_H

#include <hongmeng/macro.h>

enum {
	/* dump registers of the thread */
	DUMPSTACK_FLAGS_REGISTERS,
	/* dump minimal registers of the thread */
	DUMPSTACK_FLAGS_REGISTERS_MINI,
	/* dump registers of the fpsimd state */
	DUMPSTACK_FLAGS_FPSIMD_REGISTERS,
	/* dump function call stack with full symbol information */
	DUMPSTACK_FLAGS_STACK_BACKTRACE,
	/* dump lock trace of thread */
	DUMPSTACK_FLAGS_LOCK_TRACE,
	/* dump load information of elf file in a process */
	DUMPSTACK_FLAGS_ELF_LOAD_INFO,
	/* dump the last 'bl' instruction */
	DUMPSTACK_FLAGS_LAST_BL_DATA,
	/* dump several data around the address in registers */
	DUMPSTACK_FLAGS_REGS_MEM_REGION,
	/* dump contents of stack memory */
	DUMPSTACK_FLAGS_STACK_CONTENTS,
	DUMPSTACK_FLAGS_ACTION_MAX,

	/* flags for symbols */
	/* do not show symbols for stack dump */
	DUMPSTACK_FLAGS_SYMBOL_NO_SYMBOL,
	/* do not load symbols from files */
	DUMPSTACK_FLAGS_SYMBOL_NO_LOAD,
	/* do not trigger OOM when preparing symbols */
	DUMPSTACK_FLAGS_SYMBOL_NO_OOM,
	/* dump info for sysrq w in user */
	DUMPSTACK_FLAGS_SYMBOL_SERVICE_NO_ADDR,
	/* show address relative to binary file but not the absolute running pc */
	DUMPSTACK_FLAGS_SYMBOL_RELATIVE_ADDR,
	/* do not show frame pointer address */
	DUMPSTACK_FLAGS_SYMBOL_NO_FRAMEPOINTER,
	DUMPSTACK_FLAGS_MAX,
};

#define dumpstack_flag_mask_of(f)	(((f) == 0) ? (1U) : (1U << ((unsigned int)(f))))
#define dumpstack_is_flag_set(x, f)	(((x) & dumpstack_flag_mask_of(f)) != 0U)
#define dumpstack_is_flag_clear(x, f)	(((x) & dumpstack_flag_mask_of(f)) == 0U)
#define dumpstack_set_flag(x, f)	((x) |= dumpstack_flag_mask_of(f))
#define dumpstack_clear_flag(x, f)	((x) &= ~dumpstack_flag_mask_of(f))

#define build_dumpstack_flags0 0
#define build_dumpstack_flags1(f)  dumpstack_flag_mask_of(f)
#define build_dumpstack_flags2(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags1(__VA_ARGS__)
#define build_dumpstack_flags3(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags2(__VA_ARGS__)
#define build_dumpstack_flags4(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags3(__VA_ARGS__)
#define build_dumpstack_flags5(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags4(__VA_ARGS__)
#define build_dumpstack_flags6(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags5(__VA_ARGS__)
#define build_dumpstack_flags7(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags6(__VA_ARGS__)
#define build_dumpstack_flags8(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags7(__VA_ARGS__)
#define build_dumpstack_flags9(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags8(__VA_ARGS__)
#define build_dumpstack_flags10(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags9(__VA_ARGS__)
#define build_dumpstack_flags11(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags10(__VA_ARGS__)
#define build_dumpstack_flags12(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags11(__VA_ARGS__)
#define build_dumpstack_flags13(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags12(__VA_ARGS__)
#define build_dumpstack_flags14(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags13(__VA_ARGS__)
#define build_dumpstack_flags15(f, ...) dumpstack_flag_mask_of(f) | build_dumpstack_flags14(__VA_ARGS__)

#define __build_dumpstack_flags(n, ...) (build_dumpstack_flags##n(__VA_ARGS__))
#define build_dumpstack_flags(n, ...) __build_dumpstack_flags(n, __VA_ARGS__)
#define dumpstack_build_flags(...) build_dumpstack_flags(VA_ARGS_NUM(__VA_ARGS__), __VA_ARGS__)

#endif
