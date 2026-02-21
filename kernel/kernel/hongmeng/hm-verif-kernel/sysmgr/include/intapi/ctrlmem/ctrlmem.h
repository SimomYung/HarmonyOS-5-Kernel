/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: intapi of sysmgr ctrlmem
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 13 16:19:27 2022
 */

#ifndef INTAPI_CTRLMEM_CTRLMEM_H
#define INTAPI_CTRLMEM_CTRLMEM_H

#include <hongmeng/errno.h>
#include <sysmgr/module_ctx.h>

int ctrlmem_init_pre_linear(const struct sysmgr_module_ctx *ctx);

int ctrlmem_init_after_linear(const struct sysmgr_module_ctx *ctx);

struct page_s;
struct page_s *ctrlmem_alloc_page(uint32_t order, uint32_t ori_paf);
void ctrlmem_free_page(struct page_s *page);

#endif
