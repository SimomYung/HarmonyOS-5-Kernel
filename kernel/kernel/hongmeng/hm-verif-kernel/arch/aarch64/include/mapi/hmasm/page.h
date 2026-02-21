/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: page.h
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 18 18:59:59 2022
 */

#ifndef AARCH64_MAPI_HMASM_PAGE_H
#define AARCH64_MAPI_HMASM_PAGE_H

#include <uapi/hmasm/page.h>

#define PAGE_SIZE   __PAGE_SIZE
#define PAGE_SHIFT  __PAGE_SHIFT
#define PAGE_MASK   __PAGE_MASK

#ifndef CONFIG_AARCH64_PAGE_4K
# error "not support 16K and 64K page size"
#endif

#endif
