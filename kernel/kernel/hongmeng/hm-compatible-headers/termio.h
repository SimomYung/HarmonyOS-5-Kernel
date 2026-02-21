/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 07 15:04:16 2022
 */
#ifndef _GLIBC_INC_TERMIO_H
#define _GLIBC_INC_TERMIO_H

/*
 * To adapt to GLIBC,
 * for GLIBC includes the following headers when termio.h is included,
 * but native MUSL not.
 */
#include <termios.h>
#include <sys/ioctl.h>

#endif
