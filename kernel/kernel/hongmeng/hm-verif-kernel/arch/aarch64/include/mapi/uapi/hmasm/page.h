/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: page.h
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 18 18:57:20 2022
 */

#ifndef AARCH64_MAPI_UAPI_HMASM_PAGE_H
#define AARCH64_MAPI_UAPI_HMASM_PAGE_H

#define __PAGE_SHIFT 12
#define __PAGE_SIZE 4096
#define __PAGE_MASK (~((unsigned long)__PAGE_SIZE - 1ul))

/*
 * NOTE: PAGE_SIZE will conflict with libc, resulting in namespace pollution.
 * Now, the fs and kernel compilation depend on PAGE_SIZE PAGE_SHIFT and PAGE_SHIFT, which
 * will be deleted after the rectification is completed.
 * TODO: When supporting multiple page sizes,the dynamic detection scheme should be used.
 */
#ifndef __KERNEL__
# define PAGE_SIZE   __PAGE_SIZE
# define PAGE_SHIFT  __PAGE_SHIFT
#endif

/*
 * Definitions for hybrid huge page. The size of hybrid huge page should be a multiple of
 * __PAGE_SIZE of MMU granularity. The multiple should be power of 2.
 */
#define HHP_PAGE_ORDER 2
#define HHP_PAGE_COUNT (1 << HHP_PAGE_ORDER)
#define HHP_PAGE_SIZE  (1 << (HHP_PAGE_ORDER + __PAGE_SHIFT))

#endif
