/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Structures of kconsole backend device
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 05 07:08:29 2020
 */

#ifndef TTY_KCONSOLE_H
#define TTY_KCONSOLE_H

#include "../tty.h"
#include "../tty_wrap.h"

struct kconsole_device {
	struct tty_struct *tty;
	const struct tty_backend_operations *tty_bops;
	struct tty_line_buf *input_buf;
	struct tty_line_buf *output_buf;
	struct serial_icounter_struct icount;
	unsigned int irq;
	bool status;
	struct tty_irq_bottom_half bottom;
};

#define KCONSOLE_DEVICE_ENABLED true
#define KCONSOLE_DEVICE_DISABLED false

#endif
