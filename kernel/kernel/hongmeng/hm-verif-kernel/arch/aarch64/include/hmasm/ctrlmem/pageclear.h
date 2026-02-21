/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Page clear header
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 05 11:35:49 2023
 */
#ifndef AARCH64_ASM_CTRLMEM_PAGECLEAR_H
#define AARCH64_ASM_CTRLMEM_PAGECLEAR_H

#ifdef __HOST_LLT__
#include <hmkernel/types.h>
#include <hmkernel/panic.h>
#include <hmasm/page.h>
#include <lib/string.h>
static inline void pageclear(void *addr, unsigned long len)
{
	BUG_ON(!IS_ALIGNED(len, PAGE_SIZE));
	BUG_ON(!IS_ALIGNED(ptr_to_u64(addr), PAGE_SIZE));
	bzero(addr, len);
}
#else
void pageclear(void *addr, unsigned long len);
#endif

#endif /* AARCH64_ASM_CTRLMEM_PAGECLEAR_H */
