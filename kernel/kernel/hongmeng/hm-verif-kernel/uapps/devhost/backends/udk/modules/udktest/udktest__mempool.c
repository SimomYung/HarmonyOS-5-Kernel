/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: UDK mempool related interface test
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 23 20:40:28 2022
 */
#include "udktests.h"

#include <limits.h>
#include <errno.h>
#include <udk/log.h>
#include <udk/mempool.h>

#define TEST_MEMPOOL_INVAL_ALIGN0		0U
#define TEST_MEMPOOL_INVAL_ALIGN1		3U
#define TEST_MEMPOOL_INVAL_ALIGN2		4097U

#define TEST_MEMPOOL_VALID_ALIGN0		32U

#define TEST_MEMPOOL_VADDR			0x100000ULL
#define TEST_MEMPOOL_PADDR			0x10000ULL
#define TEST_MEMPOOL_SIZE			0x10000U

static int test_mempool_create(void)
{
	struct udk_mempool *mempool = NULL;

	mempool = udk_mempool_create(TEST_MEMPOOL_INVAL_ALIGN0);
	if (mempool != NULL) {
		udk_error("expect udk mempool create failed\n");
		return -EFAULT;
	}

	mempool = udk_mempool_create(TEST_MEMPOOL_INVAL_ALIGN1);
	if (mempool != NULL) {
		udk_error("expect udk mempool create failed\n");
		return -EFAULT;
	}

	mempool = udk_mempool_create(TEST_MEMPOOL_INVAL_ALIGN2);
	if (mempool != NULL) {
		udk_error("expect udk mempool create failed\n");
		return -EFAULT;
	}

	mempool = udk_mempool_create(ULONG_MAX);
	if (mempool != NULL) {
		udk_error("expect udk mempool create failed\n");
		return -EFAULT;
	}

	mempool = udk_mempool_create(TEST_MEMPOOL_VALID_ALIGN0);
	if (mempool == NULL) {
		udk_error("udk mempool create failed\n");
		return -EFAULT;
	}
	udk_mempool_destroy(mempool);

	return 0;
}

static int test_mempool_mem_alloc(void)
{
	struct udk_mempool *mempool = NULL;
	void *va = NULL;
	uint64_t pa = 0U;
	int ret;

	mempool = udk_mempool_create(TEST_MEMPOOL_VALID_ALIGN0);
	if (mempool == NULL) {
		udk_error("udk mempool create failed\n");
		return -EFAULT;
	}
	ret = udk_mempool_insert(mempool, (void *)(uintptr_t)(TEST_MEMPOOL_VADDR),
				 TEST_MEMPOOL_PADDR, TEST_MEMPOOL_SIZE);
	if (ret < 0) {
		udk_error("udk mempool insert failed\n");
		udk_mempool_destroy(mempool);
		return ret;
	}

	va = udk_mempool_mem_alloc(mempool, TEST_MEMPOOL_VALID_ALIGN0 - 1);
	if (va != NULL) {
		udk_error("expect udk mempool mem alloc failed\n");
		udk_mempool_destroy(mempool);
		return -EFAULT;
	}
	va = udk_mempool_mem_alloc(mempool, TEST_MEMPOOL_VALID_ALIGN0 >> 1);
	if (va != NULL) {
		udk_error("expect udk mempool mem alloc failed\n");
		udk_mempool_destroy(mempool);
		return -EFAULT;
	}

	va = udk_mempool_mem_alloc(mempool, TEST_MEMPOOL_VALID_ALIGN0);
	if (va == NULL) {
		udk_error("udk mempool mem alloc failed\n");
		udk_mempool_destroy(mempool);
		return -EFAULT;
	}
	ret = udk_mempool_paddr_from_vaddr(mempool, va, &pa);
	if (ret < 0 || ((uintptr_t)va - TEST_MEMPOOL_VADDR) != (pa - TEST_MEMPOOL_PADDR)) {
		udk_error("udk mempool paddr to vaddr failed\n");
		return ret;
	}

	udk_mempool_mem_free(mempool, va, TEST_MEMPOOL_VALID_ALIGN0);
	ret = udk_mempool_remove(mempool, (void *)(uintptr_t)TEST_MEMPOOL_VADDR, TEST_MEMPOOL_SIZE);
	if (ret != 0) {
		udk_error("udk mempool remove failed\n");
		return -EFAULT;
	}
	udk_mempool_destroy(mempool);

	return 0;
}


int udktest__mempool(void)
{
	int ret;

	ret = test_mempool_create();
	if (ret < 0) {
		return ret;
	}

	ret = test_mempool_mem_alloc();
	if (ret < 0) {
		return ret;
	}

	return 0;
}
