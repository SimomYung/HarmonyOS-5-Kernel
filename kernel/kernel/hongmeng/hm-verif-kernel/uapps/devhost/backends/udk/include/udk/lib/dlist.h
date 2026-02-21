/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Doubly linked list for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 04 00:56:34 2019
 */
#ifndef __UDK_DLIST_H__
#define __UDK_DLIST_H__

/* Used in native udk driver, not including linux headers */
#if defined(UDK_MODULE)

#include <udk/types.h>

#define _ASM_TYPES_H /* Avoid include linux/asm/types.h by lib/utils.h */
#define _ASM_GENERIC_TYPES_H
#include <hmasm/types.h>
typedef uintptr_t uptr_t;
#include <lib/dlist.h>
#undef _ASM_TYPES_H
#undef _ASM_GENERIC_TYPES_H

#else /* !UDK_MODULE */

/* Remove this when drivers are no longer built against linux */
/* Used in driver */
#if defined(MODULE)

/* Build compatible linux driver (genuio), use linux definitions */
#define _KLIBS_UTILS_
#include <lib/dlist.h>
#undef _KLIBS_UTILS_

#else /* !MODULE */

#include <lib/dlist.h>

#endif /* MODULE */

#endif /* UDK_MODULE */

#endif /* __UDK_DLIST_H__ */
