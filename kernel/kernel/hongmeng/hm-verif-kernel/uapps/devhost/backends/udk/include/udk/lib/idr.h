/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: IDR routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 15 17:10:09 2019
 */
#ifndef __UDK_IDR_H__
#define __UDK_IDR_H__

#ifndef _STDLIB_H
#define _STDLIB_H
#define __UNDEF_STDLIB_H
#endif

#if defined(UDK_MODULE)
/* Rename functions used in idr.h */
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

/* This path only works in the verif repo */
#include <libalgo/idr.h>

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

#endif /* __UDK_IDR_H__ */
