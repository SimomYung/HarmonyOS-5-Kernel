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
#include <lib/dlist.h>
#include <libmem/flags.h>

#define DMABUF_HIGH_ORDER_FLAGS (IOFAST_ZERO | IOFAST_NO_SLOW | IOFAST_NOWARN)
#define DMABUF_LOW_ORDER_FLAGS (IOFAST_ZERO)

struct mem_raw_ranges;

int udk_vmap(struct mem_raw_ranges *pfn_array,
	     int array_len, unsigned int prot, unsigned int flags, void *rvaddr);
int udk_vunmap(unsigned long va, unsigned long size);
int udk_iomem_vmap(struct mem_raw_ranges *pfn_array, size_t cnt,
		   unsigned int prot, unsigned int flags, uintptr_t *ret_vaddr);
int udk_iomem_vunmap(uintptr_t vaddr, size_t size);
uint64_t udk_alloc_pages(uint32_t flags, uint32_t order);
uint64_t udk_alloc_pages_batch(uint32_t flags, uint32_t order,
			       unsigned long size, void *pa_array,
			       unsigned int *pg_num);
void udk_free_pages(uint64_t pa);

void udk_add_to_pglist(uint64_t pa, struct dlist_node *pglist);
void udk_delete_from_pglist(uint64_t pa);
void udk_traverse_for_pglist(struct dlist_node *pglist, uint64_t *pa, size_t *size);
uint32_t udk_fetch_page_order(uint64_t pa);

int udk_page_set_private_data(uint64_t pa, unsigned long data);
unsigned long udk_page_private_data_of(uint64_t pa);

#endif /* __UDK_MM_FUSION_H__ */
