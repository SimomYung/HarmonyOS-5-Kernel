/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define headfile of tty_ioctl
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:42:56 2019
 */

#ifndef TTY_IOCTL_H
#define TTY_IOCTL_H

#include "tty.h"

typedef int (*tty_ioctl_handler_t)(struct tty_struct *tty, unsigned long arg,
				   bool is_master);

struct tty_ioctl_table {
	unsigned int cmd;
	tty_ioctl_handler_t handler;
};

int tty_ldisc_ioctl(struct tty_context *ctx, unsigned int cmd,
		    unsigned long arg);

int tty_set_control_terminal(struct tty_struct *tty, int arg);

/*
 * As same as the posix standard, tty_set_control_terminal will allow
 * the process which has capability CAP_SYS_ADMIN to preempt the terminal
 * with arg equals to 1
 */
#define TTY_TERMINAL_PREEMPTED_BY_ROOT 1
#define TTY_TERMINAL_NOT_PREEMPTED 0

#endif
