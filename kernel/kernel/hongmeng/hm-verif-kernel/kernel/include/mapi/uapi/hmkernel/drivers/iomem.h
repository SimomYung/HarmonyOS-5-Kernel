/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Extensions for operating the IO mapping memory
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 09 20:11:10 2021
 */
#ifndef MAPI_UAPI_HMKERNEL_DRIVERS_IOMEM_H
#define MAPI_UAPI_HMKERNEL_DRIVERS_IOMEM_H

/* Operate the IO mapping memory atomically. */
#define __IOMEM_ATOMIC_OP_32_READ	0
#define __IOMEM_ATOMIC_OP_32_AND	1
#define __IOMEM_ATOMIC_OP_32_OR		2
#define __IOMEM_ATOMIC_OP_32_XOR	3

#define __EXT_FASTCALL_GLOBAL_IOMEM_ATOMIC_OP_32	12

#endif /* !MAPI_UAPI_HMKERNEL_DRIVERS_IOMEM_H */
