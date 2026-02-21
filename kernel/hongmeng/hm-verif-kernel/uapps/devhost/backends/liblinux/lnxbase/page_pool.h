/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page pool internal APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#ifndef __LNXBASE_PAGE_POOL_INTERNAL_H__
#define __LNXBASE_PAGE_POOL_INTERNAL_H__

#include <lnxbase/page_pool.h>

typedef int (*page_pool_extend_stat_fn)(const struct page_pool *pool,
					char *buf, size_t size, unsigned long *rlen);

void page_pool_init(struct page_pool *pool, unsigned int page_sizeorder,
		    struct pool_info *info, page_pool_setup_cb setup);
void page_pool_destroy(struct page_pool *pool);
void *page_pool_prepare_map(const struct page_pool *pool,
			    unsigned long long paddr, unsigned long size);
void page_pool_prepare_unmap(const struct page_pool *pool,
			     void *vaddr, unsigned long size);
int page_pool_procfs_register(struct page_pool *pool, page_pool_extend_stat_fn statfn);

struct page_pool *pool_creat_mem(struct pool_info *info, unsigned int page_sizeorder);

#endif /* __LNXBASE_PAGE_POOL_INTERNAL_H__ */
