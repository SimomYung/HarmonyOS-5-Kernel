/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Some user level api of tty to export
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 07 12:11:57 2020
 */
#ifndef ULIBS_LIBTTY_TTY_H
#define ULIBS_LIBTTY_TTY_H

#include <unistd.h> /* for pid_t */

/* HONGMENG TTY defined ioctl cmd */
#define TTY_RESET_SETTING 0x1003
#define TTY_SERIAL_CONTROL 0x1004
#define TTY_SERIAL_OOFF 0x0
#define TTY_SERIAL_OON 0x1
#define TTY_SERIAL_IOFF 0x2
#define TTY_SERIAL_ION 0x3
#define TTY_STAT_INFO 0x1005

/*
 * TTY drivers may repeatedly define following data structures.
 * Ensure they keep the same.
 */
struct tty_line_buf_info_s {
	unsigned int data_len;
	unsigned int max_len;
	unsigned int read_sem_waiters;
	unsigned int write_sem_waiters;
};

struct tty_info_s {
	pid_t sid;
	pid_t pgrp;
	unsigned int flag;
	struct tty_line_buf_info_s i_line_buf_info;
	struct tty_line_buf_info_s o_line_buf_info;
};

#endif	/* ULIBS_LIBTTY_TTY_H */
