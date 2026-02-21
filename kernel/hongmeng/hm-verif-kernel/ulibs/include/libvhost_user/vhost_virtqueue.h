/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user virtqueue headers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#ifndef __VHOST_VIRTQUEUE_H__
#define __VHOST_VIRTQUEUE_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/uio.h>

typedef struct vq_desc {
	uint64_t addr;
	uint32_t len;
	uint16_t flags;
	uint16_t next;
} vq_desc_t;

typedef struct vq_avail {
	uint16_t flags;
	uint16_t idx;
	uint16_t ring[];
} vq_avail_t;

typedef struct vq_used_elem {
	uint32_t id;
	uint32_t len;
} vq_used_elem_t;

typedef struct vq_used {
	uint16_t flags;
	uint16_t idx;
	vq_used_elem_t ring[];
} vq_used_t;

/*
 * Reference from section 2.6 Split Virtqueues
 * in the Virtual I/O Device (VIRTIO) Version 1.1
 * A ring of available descriptor heads with free-running index.
 * struct vq_avail {
 *	uint16_t flags;
 *	uint16_t idx;
 *	uint16_t ring[vq_size];
 *	uint16_t used_event_idx; // Only if VIRTIO_F_EVENT_IDX
 * }
 *
 * A ring of used descriptor heads with free-running index.
 * struct vq_used {
 *	uint16_t flags;
 *	uint16_t idx;
 *	struct vq_used_elem ring[vq_size];
 *	uint16_t avail_event_idx; // Only if VIRTIO_F_EVENT_IDX
 * }
 */
#define DESC_SIZE(vq_size) (16 * (vq_size))
#define AVAIL_SIZE(vq_size) (6 + 2 * (vq_size))
#define USED_SIZE(vq_size) (6 + 8 * (vq_size))

struct virtqueue_s {
	vq_desc_t *desc;
	vq_avail_t *avail;
	vq_used_t *used;

	uint16_t qsize;

	uint16_t last_avail;
	uint16_t saved_used;

	bool ready;

	void *opaque;
	int (*handle_kick)(struct virtqueue_s *vq, void *opaque);

	void *device;
	int (*convert_gpa_to_hva)(void *device, uint64_t gpa, uint64_t len, uint64_t *hva);
};

struct virtq_buffer_s {
	int desc_count;
	int num_iov;
	struct iovec *iov;
	uint16_t *flags;
	uint16_t head_idx;
};

static inline bool virtq_has_desc(const struct virtqueue_s *virtq)
{
	return (virtq->last_avail != virtq->avail->idx);
}

int virtq_get_descs(struct virtqueue_s *virtq, struct virtq_buffer_s *buf);
void virtq_update_used(volatile vq_used_t *vq_used, uint16_t qsize, uint16_t idx,
			   uint32_t io_len);
void virtq_revert_descs(struct virtqueue_s *virtq);
int virtq_send_irq(struct virtqueue_s *virtq);
void virtq_dump(const struct virtqueue_s *virtq);
#endif
