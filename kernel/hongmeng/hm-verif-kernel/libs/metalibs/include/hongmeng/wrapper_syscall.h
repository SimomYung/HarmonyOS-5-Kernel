/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2025. All rights reserved.
 * Description: wrapper for real syscall
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 30 22:38:58 2022
 */

#ifndef ULIBS_HONGMENG_ARCH_UNITE_SYSCALL_H
#define ULIBS_HONGMENG_ARCH_UNITE_SYSCALL_H

#include <hmkernel/types.h>

static inline int
sysfast_cnode_stat_rref(struct sysarg_cnode_statrref *presult,
			rref_t rref, unsigned long flags)
{
	__u64 v = (__u64)rref;
#if (__SIZEOF_LONG__ == 8)
	return sysfast___cnode_stat_rref64(presult, (unsigned long)(v), flags);
#else
	unsigned long v_hi = (unsigned long)((v >> 32u) & 0xffffffffull);
	unsigned long v_lo = (unsigned long)(v & 0xffffffffull);
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	return sysfast___cnode_stat_rref32(presult, v_hi, v_lo, flags);
#else
	return sysfast___cnode_stat_rref32(presult, v_lo, v_hi, flags);
#endif
#endif
}

#endif
