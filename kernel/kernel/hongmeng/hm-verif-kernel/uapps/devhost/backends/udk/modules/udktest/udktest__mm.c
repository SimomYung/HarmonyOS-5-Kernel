/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Desciption: Udk mm releated interface test
 * Author: Huawei OS Kernel Lab
 * Create: Thr Mar 10 10:42:07 2022
 */
#include "udktests.h"

#include <string.h>
#include <udk/mm.h>
#include <udk/log.h>

#define TEST_SIZE	0x1000UL
#define TEST_ALIGN	0x10UL

static int test_va_pa_interconversion(const void *vaddr,
				      unsigned long long paddr)
{
	void *test_vaddr = NULL;
	unsigned long long test_paddr = 0ULL;

	test_paddr = udk_dma_va_to_pa(vaddr);
	if (test_paddr != paddr) {
		udk_error("udk dma va to pa failed\n");
		return -EPERM;
	}

	test_vaddr = udk_dma_pa_to_va(paddr);
	if (test_vaddr != vaddr) {
		udk_error("udk dma pa to va failed\n");
		return -EPERM;
	}

	return 0;
}

static int test_udk_dma_alloc(void)
{
	int ret;
	void *vaddr = NULL;
	unsigned long long paddr = 0ULL;

	vaddr = udk_dma_alloc(TEST_SIZE, DMA_ALLOC_COHERENT, &paddr);
	if (vaddr == NULL || paddr == 0ULL) {
		udk_error("udk dma alloc failed\n");
		return -ENOMEM;
	}

	ret = test_va_pa_interconversion(vaddr, paddr);
	if (ret < 0) {
		udk_error("test va pa interconversion failed with ret: %s\n",
			  strerror(-ret));
		return ret;
	}

	ret = udk_dma_free(vaddr);
	if (ret < 0) {
		udk_error("udk dma free failed with ret: %s\n", strerror(-ret));
		return ret;
	}

	return 0;
}

static int test_udk_dma_alloc_aligned(void)
{
	int ret;
	void *vaddr = NULL;
	unsigned long long paddr = 0ULL;

	vaddr = udk_dma_alloc_aligned(TEST_SIZE, DMA_ALLOC_COHERENT, &paddr,
				      TEST_ALIGN);
	if (vaddr == NULL || paddr == 0ULL) {
		udk_error("udk dma alloc aligned failed\n");
		return -ENOMEM;
	}

	ret = udk_dma_free(vaddr);
	if (ret < 0) {
		udk_error("udk dma free failed with ret: %s\n", strerror(-ret));
		return ret;
	}

	return 0;
}

int udktest__mm(void)
{
	int ret;

	ret = test_udk_dma_alloc();
	if (ret < 0) {
		return ret;
	}

	ret = test_udk_dma_alloc_aligned();
	if (ret < 0) {
		return ret;
	}

	return 0;
}
