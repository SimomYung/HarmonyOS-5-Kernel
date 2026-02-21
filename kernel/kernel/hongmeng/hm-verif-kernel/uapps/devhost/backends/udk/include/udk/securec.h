/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Secure C functions for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 20:35:11 2019
 */
#ifndef __UDK_SECUREC_H__
#define __UDK_SECUREC_H__

/*
 * _STDxx_H may already included, define
 * __UNDEF_STDxx_h to ensure that we
 * undef _STDxx_H at last only if it is defined by us
 */
#ifndef _STDIO_H
#define _STDIO_H
#define __UNDEF_STDIO_H
#endif

#ifndef _STDLIB_H
#define _STDLIB_H
#define __UNDEF_STDLIB_H
#endif

#ifndef _FEATURES_H
#define _FEATURES_H
#define __UNDEF_FEATURES_H
#endif

#ifndef _ERRNO_H
#define _ERRNO_H
#define __UNDEF_ERRNO_H
#endif

#ifndef _STDINT_H
#define _STDINT_H
#define __UNDEF_STDINT_H
#endif

#define __NEED_FILE
#include <bits/alltypes.h>
#undef __NEED_FILE

#include <udk/panic.h>
#include <udk/compiler.h>
#include <libhwsecurec/securec.h>

#ifdef __UNDEF_STDINT_H
#undef _STDINT_H
#undef __UNDEF_STDINT_H
#endif

#ifdef __UNDEF_ERRNO_H
#undef _ERRNO_H
#undef __UNDEF_ERRNO_H
#endif

#ifdef __UNDEF_FEATURES_H
#undef _FEATURES_H
#undef __UNDEF_FEATURES_H
#endif

#ifdef __UNDEF_STDLIB_H
#undef _STDLIB_H
#undef __UNDEF_STDLIB_H
#endif

#ifdef __UNDEF_STDIO_H
#undef _STDIO_H
#undef __UNDEF_STDIO_H
#endif

#endif /* __UDK_SECUREC_H__ */
