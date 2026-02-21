/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Page dup header
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 24 11:32:25 2023
 */
#ifndef AARCH64_ASM_CTRLMEM_PAGEDUP_H
#define AARCH64_ASM_CTRLMEM_PAGEDUP_H

#ifdef __HOST_LLT__
static inline void pagedup(void *dst, void *src, unsigned long len)
{
	mem_cpy_p_to_p(dst, len, src, len);
}
#else
void pagedup(void *dst, void *src, unsigned long len);
#endif

#endif /* AARCH64_ASM_CTRLMEM_PAGEDUP_H */
