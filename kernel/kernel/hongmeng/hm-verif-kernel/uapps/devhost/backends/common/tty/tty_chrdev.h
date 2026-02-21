/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define structures and prototypes of char-device tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 19:14:01 2019
 */

#ifndef TTY_CHRDEV_H
#define TTY_CHRDEV_H

#if defined(__LIBLINUX_TTY__)
#include "liblinux/tty_chrdev.h"
#elif defined(__UDK_TTY__)
#include "udk/tty/tty_chrdev.h"
#else
#error "bad build env for tty"
#endif

#endif
