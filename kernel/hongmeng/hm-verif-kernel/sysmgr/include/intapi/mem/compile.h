/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Invalid header file include check
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 09 11:23:17 2021
 */

/*
 * Some headers are internal for memmgr, which should
 * not be included by other modules of sysmgr. This
 * file provides one checker for invalid including of
 * memmgr internal headers when compiling.
 *
 * The memmgr internal headers should include the file
 * first to prevent invalid including by other modules
 * of sysmgr.
 */
#ifndef INTAPI_MEM_COMPILE_H
#define INTAPI_MEM_COMPILE_H

#ifndef __MEM_INTERNAL__
#error "Memory internal header should not be directly included."
#endif

#endif /* INTAPI_MEM_COMPILE_H */
