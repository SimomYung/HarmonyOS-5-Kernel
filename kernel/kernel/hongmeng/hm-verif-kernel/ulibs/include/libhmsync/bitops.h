/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Bitops
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 19 15:00:03 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_BITOPS_H
#define ULIBS_INCLUDE_LIBHMSYNC_BITOPS_H

#if defined(ARCH_HAS_BITOPS)
#else
#include <libhmsync/bitops/generic.h>
#endif

#endif
