/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Mempool routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 21 09:33:05 2020
 */

#include <udk/mempool.h>

#include <libmem/mem_pool.h>
#include <libmem/utils.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <udk/log.h>
#include <udk/bitops.h>

struct udk_mempool *udk_mempool_create(unsigned long align)
{
	int order;

	order = udk_size_to_order(align);
	if (order == -EINVAL) {
		udk_error("invalid align: %lx\n", align);
		return NULL;
	}

	return (struct udk_mempool *)mem_pool_create((uint32_t)order);
}

void udk_mempool_destroy(struct udk_mempool *pool)
{
	(void)mem_pool_destroy((struct mem_pool_s *)pool);
}

int udk_mempool_insert(struct udk_mempool *pool, const void *vaddr, uint64_t paddr, unsigned long size)
{
	int ret;
	ret = mem_pool_insert((struct mem_pool_s *)pool, ptr_to_ulong(vaddr), paddr, size);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

int udk_mempool_remove(struct udk_mempool *pool, const void *vaddr, unsigned long size)
{
	int ret;
	ret = mem_pool_remove((struct mem_pool_s *)pool, ptr_to_ulong(vaddr), size);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

void *udk_mempool_mem_alloc(struct udk_mempool *pool, unsigned long size)
{
	return ulong_to_ptr(mem_pool_alloc((struct mem_pool_s *)pool, size), void);
}

void udk_mempool_mem_free(struct udk_mempool *pool, const void *addr, unsigned long size)
{
	(void)mem_pool_free((struct mem_pool_s *)pool, ptr_to_ulong(addr), size);
}

int udk_mempool_paddr_from_vaddr(struct udk_mempool *pool, const void *vaddr, uint64_t *paddr)
{
	int ret;
	ret = mem_pool_paddr_from_vaddr((struct mem_pool_s *)pool, ptr_to_ulong(vaddr), paddr);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return 0;
}
