/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of scalable circuit queue
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 11 10:12:12 2022
 */

#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <lib/scq.h>

#ifndef NULL
#define NULL (void *)0
#endif

#define CONSUMED_BIT   1

#ifdef __KERNEL__
struct scq_cycles {
	s64 e_cycle;
	s64 g_cycle;
};
#else
struct scq_cycles {
	int64_t e_cycle;
	int64_t g_cycle;
};
#endif

/*
 * Lockless ring queues must meet the following condition:
 * 1. Queue is long enough to ensure that the queue is not full.
 * 2. The maximum number of records in the queue is 2 ^ MAX_DATA_WIDTH.
 * 3. Only one consumer can invoke the scq_mpsc_dequeue function.
 */
int scq_mpsc_init_empty(struct scq_s *scq, vuint32_t count_order, vuint32_t data_width)
{
	int ret = E_HM_OK;
	struct scq_ring_s *ring = NULL;

	if (scq == NULL || scq->ring == NULL || data_width > MAX_DATA_WIDTH) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ring = scq->ring;
		/* count is maximum queue length */
		ring->cfg.count = 1U << count_order;
		ring->cfg.count_order =(vuint16_t)count_order;
		ring->cfg.data_width = (vuint16_t)data_width;
		for (unsigned int i = 0; i < ring->cfg.count; i++) {
			vatomic64_init(&ring->array[i], (vuint64_t)(-1));
		}
		vatomic64_init(&ring->head, 0);
		vatomic64_init(&ring->tail, 0);
		scq->checker.count = ring->cfg.count;
	}

	return ret;
}

#ifdef __KERNEL__
static s64 get_cycles(vuint64_t data, vuint16_t order)
{
	s64 ret = -1;

	if((s64)data != -1) {
		/* Initial cycles is -1, -1 here is valid value, nor an error */
		ret = (s64)(data >> order);
	}
	return ret;
}
#else
static int64_t get_cycles(vuint64_t data, vuint16_t order)
{
	int64_t ret = -1;

	if((int64_t)data != -1) {
		/* Initial cycles is -1, -1 here is valid value, nor an error */
		ret = (int64_t)(data >> order);
	}
	return ret;
}
#endif

static inline vuint64_t set_consumed(vuint16_t data_width)
{
	return (1ULL << (data_width));
}
static inline vuint64_t is_consumed(vuint64_t data, vuint16_t data_width)
{
	return (data >> data_width) & 0x1;
}
static inline vuint64_t entry_data(vuint64_t e_cycles, vuint32_t data,
	vuint16_t data_width)
{
	return (e_cycles << (data_width + CONSUMED_BIT)) | data;
}

static int do_scq_mpsc_enqueue(struct scq_s *scq, vuint32_t data)
{
	int ret = E_HM_OK;
	struct scq_ring_s *ring = scq->ring;
	struct scq_config_s *cfg = &ring->cfg;
	vuint64_t index, ent, new_ent, tail;
	struct scq_cycles cycles;

	while(1) {
		tail  = vatomic64_get_inc(&ring->tail);
		index = tail & (cfg->count - 1);
		if (index >= scq->checker.count) {
			ret = E_HM_OVERFLOW;
			break;
		}

		ent   = vatomic64_read(&ring->array[index]);
		/* one bit for indidate consumed state */
		cycles.e_cycle = get_cycles(ent, cfg->data_width + CONSUMED_BIT);
		cycles.g_cycle = get_cycles(tail, cfg->count_order);
		if ((cycles.e_cycle < cycles.g_cycle) && is_consumed(ent, cfg->data_width)) {
			new_ent = entry_data((vuint64_t)cycles.g_cycle, data, cfg->data_width);
			if (vatomic64_cmpxchg(&ring->array[index], ent, new_ent) == ent) {
				break;
			}
		}
	}

	return ret;
}

/* The queue should never FULL, enqueue always success */
int scq_mpsc_enqueue(struct scq_s *scq, vuint32_t data)
{
	int ret = E_HM_OK;

	if (scq == NULL || scq->ring == NULL ||
	    data > ((1U << scq->ring->cfg.data_width) - 1)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = do_scq_mpsc_enqueue(scq, data);
	}
	return ret;
}

static int do_scq_mpsc_dequeue(struct scq_s *scq)
{
	struct scq_ring_s *ring  = scq->ring;
	struct scq_config_s *cfg = &ring->cfg;
	struct scq_cycles cycles;
	vuint64_t index, ent, new_ent;
	vuint64_t head, tail;
	int ret;

	while(1) {
		head  = vatomic64_read(&ring->head);
		tail  = vatomic64_read(&ring->tail);
		if (head == tail) {
			ret = E_HM_NOOBJ;
			break;
		}

		index = head & (cfg->count - 1);
		if (index >= scq->checker.count) {
			ret = E_HM_OVERFLOW;
			break;
		}
		ent   = vatomic64_read(&ring->array[index]);
		/* one bit for indidate consumed state */
		cycles.e_cycle = get_cycles(ent, cfg->data_width + CONSUMED_BIT);
		cycles.g_cycle = get_cycles(head, cfg->count_order);
		if (cycles.e_cycle == cycles.g_cycle) {
			ent = vatomic64_get_or(&ring->array[index], set_consumed(cfg->data_width));
			vatomic64_write(&ring->head, head + 1);
			ret = (int)(ent & ((1U << cfg->data_width) - 1));
			break;
		}
		if (head + 1 == tail) {
			ret = E_HM_NOOBJ;
			break;
		}
		if (cycles.e_cycle < cycles.g_cycle) {
			new_ent = (((vuint64_t)cycles.g_cycle) << (cfg->data_width + CONSUMED_BIT)) |
				set_consumed(cfg->data_width);
			if (vatomic64_cmpxchg(&ring->array[index], ent, new_ent) != ent) {
				continue;
			}
		}
		vatomic64_write(&ring->head, head + 1);
	}
	return ret;
}

int scq_mpsc_dequeue(struct scq_s *scq)
{
	int ret = E_HM_OK;

	if (scq == NULL || scq->ring == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = do_scq_mpsc_dequeue(scq);
	}

	return ret;
}
