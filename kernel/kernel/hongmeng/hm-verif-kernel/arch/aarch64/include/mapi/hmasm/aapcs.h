/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Definition of aapcs and apcs for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 10:12:37 2021
 */
#ifndef MAPI_AARCH64_AAPCS_H
#define MAPI_AARCH64_AAPCS_H

#include <hmkernel/types.h>
#include <hmkernel/uaccess.h>

#include <hmasm/ucontext.h>

/* Process AAPCS */
static inline void
aarch64_aapcs_fetch_int(void *parg, size_t argsz, u64 reg)
{
	switch (argsz) {
	case 1:
		ptr_from_void(parg, u8)[0] = (u8)(reg & 0xffU);
		break;
	case 2:
		ptr_from_void(parg, u16)[0] = (u16)(reg & 0xffffU);
		break;
	case 4:
		ptr_from_void(parg, u32)[0] = (u32)(reg & 0xffffffffU);
		break;
	case 8:
		ptr_from_void(parg, u64)[0] = reg;
		break;
	default:
		panic("invalid argno size:%zu\n", argsz);
		break;
	}
}

/* Process apcs for aarch32 */
static inline void
aarch32_apcs_fetch_int(void *parg, size_t argsz, u32 reg_hi, u32 reg_lo)
{
	/* note that parg is u64 pointer */
	switch (argsz) {
	case 1:
		((u8 *)(parg))[0] = (u8)(reg_lo & 0xffU);
		break;
	case 2:
		((u16 *)(parg))[0] = (u16)(reg_lo & 0xffffU);
		break;
	case 4:
		((u32 *)(parg))[0] = (u32)(reg_lo & 0xffffffffU);
		break;
	case 8:
		((u64 *)(parg))[0] = ((u64)(reg_hi) << 32) + (u64)(reg_lo);
		break;
	default:
		panic("invalid argno size:%zu\n", argsz);
		break;
	}
}

static inline void
ilp32_aapcs_fetch_int(void *parg, size_t argsz, u64 reg,
		      bool is_signed, bool is_fixed64)
{
	/*
	 * For aarch64ilp32 client, we have to process long, ulong and pointer type
	 * because these three types have different bits on aarch64 and aarch64ilp32.
	 *
	 * There are two possible exceptional cases:
	 *
	 * 1. Undefined value
	 * register or stack has 64bit, but parameter only occupies lower 32bit or
	 * higher 32bit which depends on byte order. The left 32bit value is undefined,
	 * so we have to cast it to make sure it's zero.
	 *
	 * 2. Negative number
	 * only happens when parameter is long type
	 * for example, 0xFFFFFFFF means -1 on aarch64ilp32.
	 * if byte order is big endian, it's 0x00000000FFFFFFFF on aarch64
	 * it means 4294967295
	 *
	 */
	if ((argsz == 8UL) && !is_fixed64) {
		if (is_signed) {
			ptr_from_void(parg, s64)[0] = (s64)(s32)reg;
		} else {
			ptr_from_void(parg, u64)[0] = (u64)(u32)reg;
		}
	} else {
		aarch64_aapcs_fetch_int(parg, argsz, reg);
	}
}
#endif
