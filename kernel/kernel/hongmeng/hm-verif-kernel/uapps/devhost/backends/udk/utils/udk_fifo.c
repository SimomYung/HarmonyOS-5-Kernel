/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the implementation of udk_fifo related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:12:05 2019
 */
#include <udk/fifo.h>

#include <libhwsecurec/securec.h>

int udk_fifo_init(struct udk_fifo *fifo, size_t size)
{
	if (fifo == NULL || size == 0) {
		return -EINVAL;
	}
	fifo->head = 0;
	fifo->len = 0;
	fifo->buf = (unsigned char *)malloc(size);
	if (fifo->buf == NULL) {
		hm_error("fifo init malloc failed\n");
		return -ENOMEM;
	}
	fifo->size = size;
	udk_mutex_init(&fifo->lock);
	return 0;
}

void udk_fifo_free(struct udk_fifo *fifo)
{
	if (fifo == NULL) {
		return;
	}
	udk_mutex_lock(&fifo->lock);
	fifo->head = 0;
	fifo->len = 0;
	fifo->size = 0;
	if (fifo->buf != NULL) {
		free(fifo->buf);
		fifo->buf = NULL;
	}
	udk_mutex_unlock(&fifo->lock);
}

int udk_fifo_in(struct udk_fifo *fifo, const unsigned char *buf, size_t size)
{
	size_t tail;
	size_t num_copied;

	if (fifo == NULL) {
		return -EINVAL;
	}

	udk_mutex_lock(&fifo->lock);
	if (fifo->buf == NULL ||
	    buf == NULL ||
	    (fifo->size - fifo->len < size)) {
		udk_mutex_unlock(&fifo->lock);
		return -EINVAL;
	}

	/* Use memcpy_s to copy data in two step
	 * We ensure that head will not be overrun by checking remaining space
	 * 1. copy from tail to fifo->size
	 * 2. wrap tail and copy the remaining data
	 */
	tail = (fifo->head + fifo->len) % fifo->size;
	num_copied = fifo->size - tail > size ? size : fifo->size - tail;
	/* copy first part */
	NOFAIL(memcpy_s(fifo->buf + tail, fifo->size - tail, buf, num_copied));
	/* copy second part */
	if (num_copied < size) {
		NOFAIL(memcpy_s(fifo->buf, fifo->head, buf + num_copied, size - num_copied));
	}
	/* update len */
	fifo->len += size;
	udk_mutex_unlock(&fifo->lock);

	return (int)size;
}

int udk_fifo_out(struct udk_fifo *fifo, unsigned char *buf, size_t size)
{
	size_t num_copied;

	if (fifo == NULL) {
		return -EINVAL;
	}

	udk_mutex_lock(&fifo->lock);
	if (fifo->buf == NULL ||
	    buf == NULL ||
	    fifo->len < size) {
		udk_mutex_unlock(&fifo->lock);
		return -EINVAL;
	}

	/* Use memcpy_s to copy data in two step
	 * 1. copy from head to fifo->size
	 * 2. wrap head and copy the remaining data
	 */
	num_copied = fifo->size - fifo->head > size ? size : fifo->size - fifo->head;
	/* copy first part */
	NOFAIL(memcpy_s(buf, size, fifo->buf + fifo->head, num_copied));
	/* copy second part */
	if (num_copied < size) {
		NOFAIL(memcpy_s(buf + num_copied, size - num_copied, fifo->buf, size - num_copied));
	}
	/* update len and head */
	fifo->len -= size;
	fifo->head = (fifo->head + size) % fifo->size;
	udk_mutex_unlock(&fifo->lock);

	return (int)size;
}

size_t udk_fifo_len(struct udk_fifo *fifo)
{
	size_t len;

	if (fifo == NULL) {
		return 0U;
	}
	udk_mutex_lock(&fifo->lock);
	len = fifo->len;
	udk_mutex_unlock(&fifo->lock);
	return len;
}
