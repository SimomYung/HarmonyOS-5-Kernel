/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Framework of ctrlmem - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 13 17:49:42 2022
 */

#ifndef UAPI_HMKERNEL_CTRLMEM_H
#define UAPI_HMKERNEL_CTRLMEM_H

#include <hmkernel/types.h>

/*
 * Use a u64 for ctrlmem page to identify itself.
 * Currently use 5 bits for its type and 32 bits
 * for its size (in bytes)
 */
#define __ctrlmem_meta_type_of(meta)	(((meta) >> 56u) & (__u64)(0xffu))
#define __ctrlmem_meta_is_last(meta)	(((meta) >> 32u) & (__u64)(0x1u))
#define __ctrlmem_meta_size_of(meta)	((meta) & (__u64)(0xffffffffu))

#define __CTRLMEM_META_TYPE_VSPACE	((__u64)(1u))
#define __CTRLMEM_META_TYPE_SYSCONF	((__u64)(2u))
#define __CTRLMEM_META_TYPE_BKOBJCTNR	((__u64)(3u))
#define __CTRLMEM_META_TYPE_PCPINFO	((__u64)(4u))

#endif
