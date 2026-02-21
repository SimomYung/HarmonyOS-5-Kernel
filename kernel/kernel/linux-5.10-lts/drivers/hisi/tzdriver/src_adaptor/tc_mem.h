/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TC_MEM_H
#define TC_MEM_H

#include "teek_ns_client.h"

static inline void sync_user_cache_to_mem(uint64_t start, uint64_t end) {}
static inline void invalid_user_cache(uint64_t start, uint64_t end) {}

void *tz_alloc(size_t size);
void tz_free(void *addr);
void tc_mem_free(struct tc_ns_shared_mem *shared_mem);
int tz_alloc_sharemem(void **kaddr, size_t len);
unsigned long agent_buffer_map(unsigned long buffer, uint32_t size);
#endif