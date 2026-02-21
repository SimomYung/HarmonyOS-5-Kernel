/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definition of [u]random device ioctl
 * Author: Huawei OS Kernel Lab
 * Create:  Tue 26 11:18:58 2023
 */

#ifndef CRYPTO_IOCTL_H
#define CRYPTO_IOCTL_H

#include <sys/ioctl.h>

#define RND_IOCTL_BASE 'R'
#define RNDGETENTCNT	_IOR(RND_IOCTL_BASE, 0x00, int)
#define RNDADDTOENTCNT	_IOW(RND_IOCTL_BASE, 0x01, int)
#define RNDGETPOOL	_IOR(RND_IOCTL_BASE, 0x02, int [2])
#define RNDADDENTROPY	_IOW(RND_IOCTL_BASE, 0x03, int [2])
#define RNDZAPENTCNT	_IO(RND_IOCTL_BASE,  0x04)
#define RNDCLEARPOOL	_IO(RND_IOCTL_BASE,  0x06)
#define RNDRESEEDCRNG	_IO(RND_IOCTL_BASE,  0x07)

#endif
