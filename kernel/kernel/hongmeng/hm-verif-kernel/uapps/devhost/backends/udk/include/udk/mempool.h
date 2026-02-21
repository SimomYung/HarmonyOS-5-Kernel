/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Mempool routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 21 09:43:46 2020
 */
#ifndef	__UDK_MEMPOOL_H__
#define	__UDK_MEMPOOL_H__

#include <inttypes.h>
#include <libmem/mem_pool.h>

struct udk_mempool;

struct udk_mempool *udk_mempool_create(unsigned long align);
void udk_mempool_set_insert_behavior(struct udk_mempool *pool, mempool_insert_t func);
void udk_mempool_destroy(struct udk_mempool *pool);
int udk_mempool_insert(struct udk_mempool *pool, const void *vaddr, uint64_t paddr, unsigned long size);
int udk_mempool_remove(struct udk_mempool *pool, const void *vaddr, unsigned long size);
void *udk_mempool_mem_alloc(struct udk_mempool *pool, unsigned long size);
void udk_mempool_mem_free(struct udk_mempool *pool, const void *addr, unsigned long size);
int udk_mempool_paddr_from_vaddr(struct udk_mempool *pool, const void *vaddr, uint64_t *paddr);

#endif /* __UDK_MEMPOOL_H__ */
