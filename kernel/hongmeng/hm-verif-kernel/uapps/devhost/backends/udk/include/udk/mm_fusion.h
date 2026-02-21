/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Memory mapping routines of UDK only enabled under fusion mode
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 11 10:27:05 2024
 */
#ifndef __UDK_MM_FUSION_H__
#define __UDK_MM_FUSION_H__

#include <stdint.h>
#include <stddef.h>

uint64_t udk_alloc_pages(uint32_t flags, uint32_t order);
void udk_free_pages(uint64_t pa);

#endif /* __UDK_MM_FUSION_H__ */
