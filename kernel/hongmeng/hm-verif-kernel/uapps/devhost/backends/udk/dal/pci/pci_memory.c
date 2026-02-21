/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: PCI memory type functions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 30 11:48:39 2021
 */
#include "pci_memory.h"

#include <lib/utils.h>
#include <udk/pci.h>
#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/bitops.h>
#include <udk/securec.h>

struct pci_mempool {
	uint64_t start;
	uint64_t size;
};

struct pci_allocator {
	struct pci_mempool mempools[(int)UDK_PCI_MEM_TYPE_NR];
};

static const uint32_t mem_type_table[(int)UDK_PCI_MEM_TYPE_NR] = {
	BAR_SPACE_IO,
	BAR_TYPE_32BIT | BAR_TYPE_64BIT,
	BAR_TYPE_32BIT | BAR_PREFETCHABLE,
	BAR_TYPE_64BIT | BAR_PREFETCHABLE,
};

int mem_flag_to_type(uint32_t flag)
{
	uint32_t type = UDK_MASK(flag, MEM_FLAG_TYPE_MASK);
	int index;

	if (type == BAR_TYPE_32BIT || type == BAR_TYPE_64BIT) {
		return (int)UDK_PCI_MEM_NORMAL;
	}
	for (index = 0; index < (int)UDK_PCI_MEM_TYPE_NR; index++) {
		if (mem_type_table[index] == type) {
			return index;
		}
	}

	return -EINVAL;
}

uint32_t mem_type_to_flag(uint32_t type)
{
	if (type >= (uint32_t)UDK_PCI_MEM_TYPE_NR) {
		return 0;
	}

	return mem_type_table[type];
}

bool is_mem_type_flag_match(uint32_t type, uint32_t flag)
{
	uint32_t actual_type = (uint32_t)mem_flag_to_type(flag);

	return type == actual_type;
}

struct pci_allocator *pci_allocator_alloc(void)
{
	struct pci_allocator *allocator = NULL;

	allocator = udk_malloc(sizeof(*allocator));
	if (allocator == NULL) {
		return NULL;
	}
	mem_zero_s(*allocator);

	return allocator;
}

int pci_allocator_insert(struct pci_allocator *allocator, uint64_t addr,
			 uint64_t size, uint32_t type)
{
	struct pci_mempool *pool = NULL;

	if (allocator == NULL) {
		udk_info("allocator is null\n");
		return -EINVAL;
	}
	if (addr == 0 || size == 0 || type >= (unsigned int)UDK_PCI_MEM_TYPE_NR) {
		udk_warn("invalid addr, size=0x%llx or type=%u\n",
			 (unsigned long long)size, type);
		return -EINVAL;
	}
	pool = &allocator->mempools[type];
	if (pool->start != 0) {
		udk_error("type=%u has been inserted\n", type);
		return -EINVAL;
	}
	pool->start = addr;
	pool->size = size;

	return 0;
}

static uint64_t mempool_alloc(struct pci_mempool *src, uint64_t size, uint64_t align)
{
	uint64_t aligned_start, aligned_end, aligned_size;
	uint64_t start = src->start;

	aligned_start = ALIGN_UP(src->start, align);
	aligned_end = ALIGN_DOWN(src->start + src->size, align);
	aligned_size = aligned_end - aligned_start;
	if (aligned_size < size) {
		return 0;
	}
	src->start = aligned_start + size;
	src->size -= (src->start - start);

	return aligned_start;
}

uint64_t pci_allocator_mem_alloc(struct pci_allocator *allocator,
				 uint64_t size, uint64_t align,
				 uint32_t type)
{
	if (allocator == NULL) {
		udk_error("allocator is null\n");
		return 0;
	}
	if (size == 0 || type >= (uint32_t)UDK_PCI_MEM_TYPE_NR) {
		udk_error("invalid size=0x%llx or type=%u\n",
			  (unsigned long long)size, type);
		return 0;
	}

	return mempool_alloc(&allocator->mempools[type], size, align);
}

void pci_allocator_destroy(struct pci_allocator *allocator)
{
	if (allocator != NULL) {
		udk_free(allocator);
	}
}
