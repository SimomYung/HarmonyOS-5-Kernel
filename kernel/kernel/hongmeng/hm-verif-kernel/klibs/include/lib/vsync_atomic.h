/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Klib atomic wrapper for libvsync
 * Author: Huawei OS Kernel Lab
 * Create: Jul 28 09:58 2021
 */

#ifndef KLIBS_VSYNC_ATOMIC_H
#define KLIBS_VSYNC_ATOMIC_H

#define VSYNC_FREESTANDING

/* Libvsync's WFE maybe can't wake up in time, close it */
#define VSYNC_NO_WFE

#include <vsync/atomic.h>
#endif /* !KLIBS_VSYNC_ATOMIC_H */
