/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Interfaces for page table alloc
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 18 20:01:28 2022
 */
#ifndef INTAPI_MEM_ALLOC_H
#define INTAPI_MEM_ALLOC_H

#include <stdint.h>
#include <hongmeng/types.h>

/*
 * Allocate physical memory by page
 *
 * @param order [I] The order of the page to allocate
 *
 * @return      Pfn of the page if success, INVALID_PFN if fail
 *
 * @note        The user should call free_page to free
 *              the memory allocated by alloc_page. And
 *              it's not allowed to free part of the page.
 */
uint64_t alloc_pgtable(uint32_t order);

/*
 * Free the page by its pfn
 *
 * @param pfn [I] The start address of the page
 *
 * @note      The paramter @pa should be the start address
 *            of the page allocated by alloc_page.
 */
void free_pgtable(uint64_t pfn);

/*
 * Check the maximum slab size to allocate
 */
size_t malloc_slab_max_size(void);

#endif /* INTAPI_MEM_ALLOC_H */
