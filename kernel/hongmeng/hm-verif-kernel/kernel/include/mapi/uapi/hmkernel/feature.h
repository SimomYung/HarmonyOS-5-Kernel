/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Feature export for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 14 13:02:08 2020
 */
#ifndef UAPI_HMKERNEL_FEATURE_H
#define UAPI_HMKERNEL_FEATURE_H

/*
 *	host llt		arch			__UAPI_COMPAT__
 *	   Y			arm				N
 *	   Y			aarch64				Y
 *	   Y			x86				Y
 *	   N			arm				N
 *	   N			aarch64				Y
 */
#ifdef __KERNEL__
#ifdef __HOST_LLT__
#ifndef CONFIG_ARM
#define __UAPI_COMPAT__
#endif

#else

#ifdef __aarch64__
#define __UAPI_COMPAT__
#endif
#endif /* !__HOST_LLT__ */

#else

#ifdef __aarch64__
#define __UAPI_COMPAT__
#endif
#endif /* !__KERNEL__ */

#endif
