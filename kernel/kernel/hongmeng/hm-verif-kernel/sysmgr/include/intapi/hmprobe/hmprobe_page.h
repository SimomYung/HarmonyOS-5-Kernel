/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 14:45:40 2023
 */
#ifndef H_SYSMGR_INCLUDE_HMPROBE_PAGE_H
#define H_SYSMGR_INCLUDE_HMPROBE_PAGE_H

int hmprobe_page_alloc(uint64_t *pfn, uint32_t page_order);
void hmprobe_page_free(uint64_t pfn);

#endif
