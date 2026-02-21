/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Const
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef UAPI_HMKERNEL_CONST_H
#define UAPI_HMKERNEL_CONST_H

#define ___KCAT(x, y)       (x##y)
#define __KCAT(x, y)	___KCAT(x, y)

#ifdef __ASSEMBLY__
#define __U(x) x
#define __UL(x) x
#define __ULL(x) x
#else
#define __U(x)		((unsigned int)__KCAT(x, U))
#define __UL(x)		((unsigned long)__KCAT(x, UL))
#define __ULL(x)	((unsigned long long)__KCAT(x, ULL))
#endif

#endif
