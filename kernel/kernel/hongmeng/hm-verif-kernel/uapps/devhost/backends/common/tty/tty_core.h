/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define head file of tty core functions
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:42:56 2019
 */

#ifndef TTY_CORE_H
#define TTY_CORE_H

#include "tty.h"
#include "tty_wrap.h"

struct tty_struct *find_tty_by_devt(dev_t devt);

struct tty_struct *find_tty_by_sid(pid_t sid);

struct tty_driver *current_tty_driver(void);

#endif
