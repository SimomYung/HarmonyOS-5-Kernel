/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of stream driver
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:42:56 2019
 */

#ifndef STREAM_CORE_H
#define STREAM_CORE_H

#include "stream.h"

int stream_register_tty(struct stream_device_pair *pair);

void stream_unregister_tty(struct stream_device_pair *pair);

int stream_ptmx_read(struct stream_context *ctx, char *buf, size_t size);

int stream_ptmx_write(struct stream_context *ctx, const char *buf, size_t size);

unsigned int stream_ptmx_poll(struct stream_context *ctx);

int stream_ptmx_ioctl(struct stream_context *ctx,
		      unsigned int cmd, unsigned long arg);

int stream_put_char(void *backend, char c);

int stream_flush(void *backend);

int stream_poll_notify(void *backend, unsigned int event);

/* Api about ptmx and pair create */
int stream_ptmx_init(struct stream_control_device *ptmx,
		      struct tty_chrdrv *chrdrv,
		      const struct tty_backend_operations *bops);

void stream_ptmx_release(struct stream_control_device *ptmx);

struct stream_device_pair *
stream_device_pair_create(struct stream_control_device *ptmx);

void stream_device_pair_release(struct stream_device_pair *pair);

#endif
