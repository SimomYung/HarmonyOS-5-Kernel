/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Headfiles and defines inherited from udk
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 11 09:27:48 2020
 */

#ifndef TTY_WRAP_H
#define TTY_WRAP_H

#if defined(__LIBLINUX_TTY__)
#include "liblinux/tty_wrap.h"
#elif defined(__UDK_TTY__)
#include "udk/tty/tty_wrap.h"
#else
#error "bad build env for tty"
#endif

#endif
