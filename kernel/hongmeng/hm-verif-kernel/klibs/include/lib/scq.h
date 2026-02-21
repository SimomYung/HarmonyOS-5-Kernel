/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of scalable circuit queue
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 11 10:12:12 2022
 */

#ifndef KLIBS_SCQ_H
#define KLIBS_SCQ_H

#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif

#include <hmkernel/types.h>

#define MAX_DATA_WIDTH 20

struct scq_config_s {
	vuint32_t count;
	vuint16_t count_order;
	vuint16_t data_width;
};

/* XXX: cache line alignment */
struct scq_ring_s {
	struct scq_config_s cfg;
	vatomic64_t head;
	vatomic64_t tail;
	vatomic64_t array[];
};

struct scq_check_s {
	vuint32_t count;
};

struct scq_s {
	/* scq ringbuffer memory pointer. it is the real ringbuffer. */
	struct scq_ring_s *ring;

	/*
	 * Use another memory, to check if the ringbuffer index is overflow.
	 * As the ringbuffer is overlapped, can't trust any data in ringbuffer, so here
	 * provided another memory to save this.
	 */
	struct scq_check_s checker;
};

int scq_mpsc_init_empty(struct scq_s *scq, vuint32_t count_order, vuint32_t data_width);
int scq_mpsc_enqueue(struct scq_s *scq, vuint32_t data);
int scq_mpsc_dequeue(struct scq_s *scq);
#endif
