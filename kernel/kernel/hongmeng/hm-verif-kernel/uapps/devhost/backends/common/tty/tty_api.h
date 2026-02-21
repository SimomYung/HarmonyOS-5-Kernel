/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define tty-APIs for low level drivers
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 04 15:42:56 2019
 */

#ifndef TTY_API_H
#define TTY_API_H

#include "tty.h"

/* basic tty init api */
struct tty_struct *tty_struct_register(void *backend_drv,
				       const struct tty_backend_operations *bops,
				       unsigned int tty_opt,
				       const char *tty_name);

void tty_struct_unregister(struct tty_struct *tty);

void tty_struct_reset(struct tty_struct *tty);

bool tty_struct_try_recycle(struct tty_struct *tty);

void tty_struct_set_removed(struct tty_struct *tty);

int tty_struct_device_reinit(struct tty_struct *tty);

bool tty_struct_is_using(struct tty_struct *tty);

/* tty echo func */
int tty_echo(struct tty_struct *tty, const char *echo_buf, int size);

/* tty ioctl func for stream master */
int tty_stream_master_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);

#endif
