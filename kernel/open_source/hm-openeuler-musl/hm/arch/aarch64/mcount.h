/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thursday Aug 29, 2019 09:09:30 CST
 */

#ifndef HM_ARCH_AARCH64_MCOUNT_H
#define HM_ARCH_AARCH64_MCOUNT_H

#define MCOUNT_INTERNAL(frompc, selfpc)	\
static inline void mcount_internal(unsigned long (frompc), unsigned long (selfpc))

#define RETURN_ADDRESS(n)	\
	__builtin_extract_return_addr(__builtin_return_address(n))

/*
 * AARCH64 profiling entry point is '_mcount'.
 * Accept 'frompc' address as argument from the function that calls _mcount
 * for profiling. Use  __builtin_return_address (0) for the 'selfpc' address.
 */
#define MCOUNT_ENTRY	\
void _mcount(void *frompc);							\
void _mcount(void *frompc)							\
{										\
	mcount_internal((unsigned long)((unsigned long)frompc),			\
			(unsigned long)((unsigned long)RETURN_ADDRESS(0)));	\
}

#endif
