/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declare interface of slab_utils
 * Author: Huawei OS Kernel Lab
 * Create: Thu April 11 15:33:20 UTC 2024
 */

#ifndef _ULIBS_LIBMEM_SLAB_POISON_H_
#define _ULIBS_LIBMEM_SLAB_POISON_H_

#ifdef CONFIG_DEBUG_SLAB_POISON
#define POISON_INUSE 0x55 /* for use-uninitialised poisoning 对应"U" */
#define POISON_FREE  0x46 /* for use-after-free poisoning 对应"F" */
#define POISON_END   0x45 /* end-byte of poisoning 对应"E" */
#endif /* CONFIG_DEBUG_SLAB_POISON */

#endif /* _ULIBS_LIBMEM_SLAB_POISON_H_ */
