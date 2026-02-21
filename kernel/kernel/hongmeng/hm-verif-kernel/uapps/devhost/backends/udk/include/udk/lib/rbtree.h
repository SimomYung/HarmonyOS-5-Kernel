/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: This is the header of udk rbtree interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 21 16:06:35 2023
 */

#ifndef __UDK_RBTREE_H__
#define __UDK_RBTREE_H__

#ifndef _STDLIB_H
#define _STDLIB_H
#define __UNDEF_STDLIB_H
#endif

#if defined(UDK_MODULE)
/* Rename functions used in rbtree.h */
#define malloc(...) udk_malloc(__VA_ARGS__)
#define free(...)   udk_free(__VA_ARGS__)
#define wctomb(...) udk_wctomb(__VA_ARGS__)

/* Drivers use POSIX errno */
#define E_HM_INVAL (-EINVAL)
#define E_HM_NOMEM (-ENOMEM)
#define E_HM_POSIX_EXIST (-EEXIST)
#define E_HM_OBJEXIST (-EEXIST)
#define E_HM_POSIX_NOENT (-ENOENT)
#define E_HM_NOOBJ (-ENOENT)
#endif /* UDK_MODULE */

#include <libalgo/rbtree.h>

#if defined(UDK_MODULE)
#undef malloc
#undef free
#undef wctomb
#undef E_HM_INVAL
#undef E_HM_NOMEM
#undef E_HM_POSIX_EXIST
#undef E_HM_OBJEXIST
#undef E_HM_POSIX_NOENT
#undef E_HM_NOOBJ
#endif

#ifdef __UNDEF_STDLIB_H
#undef _STDLIB_H
#undef __UNDEF_STDLIB_H
#endif

#endif /* __DEVHOST_UDK_RBTREE_H__ */
