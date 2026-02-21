/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define headfile of tty_io
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:42:56 2019
 */

#ifndef TTY_IO_H
#define TTY_IO_H

#include "tty.h"

struct tty_operations *current_tty_operations(void);

#endif
