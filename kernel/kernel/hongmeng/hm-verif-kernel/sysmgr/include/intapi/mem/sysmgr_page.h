/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declare interface of page for udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 13 16:10:12 UTC 2024
 */

#ifndef __SYSMGR_INTAPI_MEM_SYSMGR_PAGE_H__
#define __SYSMGR_INTAPI_MEM_SYSMGR_PAGE_H__

#include <lib/dlist.h>

uint64_t sysmgr_alloc_pages(uint32_t flags, uint32_t order);
uint64_t sysmgr_alloc_pages_batch(uint32_t flags, uint32_t order,
				  unsigned long size, void *pa_array,
				  unsigned int *pg_num);
void sysmgr_free_pages(uint64_t pa);

void sysmgr_add_to_pglist(uint64_t pa, struct dlist_node *pglist);
void sysmgr_delete_from_pglist(uint64_t pa);
void sysmgr_traverse_for_pglist(struct dlist_node *pglist, uint64_t *pa, size_t *size);

uint32_t sysmgr_fetch_page_order(uint64_t pa);

int sysmgr_page_set_private_data(uint64_t pa, unsigned long data);
unsigned long sysmgr_page_private_data_of(uint64_t pa);

#endif /* __SYSMGR_INTAPI_MEM_SYSMGR_PAGE_H__ */
