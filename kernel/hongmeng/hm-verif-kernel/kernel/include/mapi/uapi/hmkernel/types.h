/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Types
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifndef UAPI_HMKERNEL_TYPES_H
#define UAPI_HMKERNEL_TYPES_H

#include <hmasm/types.h>

#ifndef __ASSEMBLY__
#define __ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

typedef _Bool __bool;
#define __true (_Bool)(1 == 1)
#define __false (_Bool)(1 != 1)

#define __NULL	((void *)(0ul))

#endif /* __ASSEMBLY__ */
#endif
