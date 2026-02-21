/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: pool-page management in memmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 07 19:28:06 2023
 */

#ifndef SYSMGR_MEM_POOL_PAGE_H
#define SYSMGR_MEM_POOL_PAGE_H

#include <stdlib.h>
#include <libmem/utils.h>

#include <hmkernel/page_pool/page_pool.h>
/*
 * Alloc page form page pool
 * @param           unsigned long long sender
 *                  unsigned long credential
 * @return          E_HM_OK if succeed,
 *                  other if failed.
 */
int mem_handle_pool_page_alloc(unsigned long long sender,
			       unsigned long credential);

/*
 * Free page
 * @param pfn      [I] pfn number of allocated page
 * @note            User should ensure the page is not in used
 */
void mem_handle_pool_page_free(uint64_t pfn);

int hm_pool_page_alloc_and_insert(void);

#endif /* SYSMGR_MEM_POOL_PAGE_H */
