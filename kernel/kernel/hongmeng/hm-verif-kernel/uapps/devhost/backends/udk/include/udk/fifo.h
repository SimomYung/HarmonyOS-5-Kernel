/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the header of udk_fifo interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 04 16:46:16 2019
 */
#ifndef __DEVHOST_UDK_FIFO_H__
#define __DEVHOST_UDK_FIFO_H__

#include <udk/types.h>
#include <udk/sync.h>

struct udk_fifo {
	size_t head;
	size_t len;
	size_t size;
	unsigned char *buf;
	struct udk_mutex lock;
};

int udk_fifo_init(struct udk_fifo *fifo, size_t size);
void udk_fifo_free(struct udk_fifo *fifo);
int udk_fifo_in(struct udk_fifo *fifo, const unsigned char *buf, size_t size);
int udk_fifo_out(struct udk_fifo *fifo, unsigned char *buf, size_t size);
size_t udk_fifo_len(struct udk_fifo *fifo);

#endif /* __DEVHOST_UDK_FIFO_H__ */
