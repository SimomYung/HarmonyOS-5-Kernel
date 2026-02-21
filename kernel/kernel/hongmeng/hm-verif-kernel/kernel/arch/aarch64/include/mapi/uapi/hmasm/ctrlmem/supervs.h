/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Supervs related definitions - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 23 16:32:21 2025
 */

#ifndef A64_UAPI_HMASM_CTRLMEM_SUPERVS_H
#define A64_UAPI_HMASM_CTRLMEM_SUPERVS_H

#define __CTRLMEM_SUPERVS_SUPERPOLICYDB_VA_START	((void *)(0x100000UL))
#define __CTRLMEM_SUPERVS_SUPERPOLICYDB_SLOTNO		(1U)

#define __CTRLMEM_SUPERVS_SUPERPOLICYDB_VA_OFFSET	((1UL << 39) * __CTRLMEM_SUPERVS_SUPERPOLICYDB_SLOTNO)
#define __CTRLMEM_SUPERVS_SUPERPOLICYDB_VA_ACCESS	((void *)(__CTRLMEM_SUPERVS_SUPERPOLICYDB_VA_OFFSET + \
								  (unsigned long) \
								  (__CTRLMEM_SUPERVS_SUPERPOLICYDB_VA_START)))

#endif
