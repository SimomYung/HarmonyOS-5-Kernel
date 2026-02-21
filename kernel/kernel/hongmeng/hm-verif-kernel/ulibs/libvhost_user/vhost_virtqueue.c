/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user virtqueue implements
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#include "libvhost_user/vhost_virtqueue.h"

#include <hmasm/barrier.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hmkernel/compiler.h>
#include <libhmlog/hmlog.h>
#include <libsecure/bounds_check.h>
#include <libvhost_user/vhost_server.h>

#define VIRTQ_DESC_FLAG_NEXT		1U
#define VIRTQ_DESC_FLAG_INDIRECT	4U
#define VIRTIO_FEATURE_INDIRECT_DESC    28U
#define VIRTIO_FEATURE_EVENT_IDX        29U

static uint16_t get_head_idx(const struct virtqueue_s *virtq)
{
	uint16_t head, idx;

	/* Get the head index of descriptors list */
	idx = virtq->last_avail & (virtq->qsize - 1U);
	head = virtq->avail->ring[idx];

	return head;
}

static volatile vq_desc_t *
get_next_desc(volatile vq_desc_t *table, uint16_t size, uint16_t next)
{
	volatile vq_desc_t *desc = NULL;

	if (next < size) {
		next = array_index_fixup(next, size);

		desc = &table[next];
	}

	return desc;
}

static int map_desc_to_iov(struct virtqueue_s *virtq,
			   const volatile vq_desc_t *desc,struct iovec *iov,
			   uint16_t *flags)
{
	uint64_t hva = 0U;
	int ret;
	uint32_t desc_len;

	if (virtq == NULL || desc == NULL || iov == NULL) {
		ret = E_HM_INVAL;
	} else {
		desc_len = desc->len;
		ret = virtq->convert_gpa_to_hva(virtq->device, desc->addr, desc_len, &hva);
	}

	if (ret == E_HM_OK) {
		iov->iov_base = u64_to_ptr(hva, void);
		iov->iov_len = desc_len;

		if (flags != NULL) {
			*flags = desc->flags;
		}
	}

	return ret;
}

static void clear_iov(int count, struct iovec *iov, uint16_t *flags)
{
	int i;

	for (i = 0; i < count; i++) {
		iov[i].iov_base = NULL;
		iov[i].iov_len = 0;

		if (flags != NULL) {
			flags[i] = 0;
		}
	}
}

static bool virtq_has_feature(const struct virtqueue_s *virtq, uint32_t bit)
{
	uint64_t features = ((struct vhost_device_s *)virtq->device)->backend->features;

	return (features & (1ULL << bit)) != 0ULL;
}

static void set_avail_event(struct virtqueue_s *virtq)
{
	volatile vq_used_elem_t *used_elem = &virtq->used->ring[virtq->qsize];
	volatile uint16_t *event_idx = ulong_to_ptr(ptr_to_ulong(used_elem), uint16_t);

	UNUSED(used_elem);

	if (virtq_has_feature(virtq, VIRTIO_FEATURE_EVENT_IDX)) {
		*event_idx = virtq->last_avail;
		mb();
	}
}

static int vhost_switch_to_indir(struct virtqueue_s *virtq,
				 volatile vq_desc_t **desc,
				 volatile vq_desc_t **table,
				 uint16_t *size)
{
	int ret;
	uint64_t hva;
	uint16_t num;
	volatile vq_desc_t *indir = *desc;
	uint32_t indir_len = indir->len;

	if (!virtq_has_feature(virtq, VIRTIO_FEATURE_INDIRECT_DESC)) {
		hm_error("indir desc support is not negotiated\n");
		return E_HM_BADR;
	}

	num = (uint16_t)(indir_len / sizeof(vq_desc_t));
	if ((indir_len & 0xfU) != 0 || (num == 0)) {
		hm_error("indir desc table length is incorrect\n");
		return E_HM_BADR;
	}

	ret = virtq->convert_gpa_to_hva(virtq->device, indir->addr, indir_len, &hva);
	if (ret != E_HM_OK) {
		hm_error("indir desc table addr is incorrect\n");
		return E_HM_BADR;
	}
	*desc = *table = u64_to_ptr(hva, volatile vq_desc_t);
	*size = num;

	return E_HM_OK;
}

int virtq_get_descs(struct virtqueue_s *virtq, struct virtq_buffer_s *buf)
{
	uint16_t next, table_size;
	int count, ret;
	volatile vq_desc_t *desc = NULL;
	volatile vq_desc_t *table = NULL;

	table = virtq->desc;
	table_size = virtq->qsize;
	rmb();
	buf->head_idx = next = get_head_idx(virtq);
	/* Get descriptors */
	for (count = 0; count < buf->num_iov; count++) {
		desc = get_next_desc(table, table_size, next);
		if (desc == NULL) {
			hm_error("get next desc failed\n");
			break;
		}
		if ((desc->flags & VIRTQ_DESC_FLAG_INDIRECT) != 0) {
			ret = vhost_switch_to_indir(virtq, &desc, &table, &table_size);
			if (ret != E_HM_OK) {
				hm_warn("get indirect desc to iov failed\n");
				break;
			}
		}

		ret = map_desc_to_iov(virtq, desc, &buf->iov[count],
				      &buf->flags[count]);
		if (ret != E_HM_OK) {
			hm_warn("map desc to iov failed\n");
			break;
		}
		if ((desc->flags & VIRTQ_DESC_FLAG_NEXT) == 0) {
			virtq->last_avail++;
			set_avail_event(virtq);
			buf->desc_count = count + 1;
			return E_HM_OK;
		}
		next = desc->next;
	}
	/*
	 * We stop converting if the iovec is not large enough, and clear all
	 * processed iov. So the caller need to input a larger array to get the
	 * descriptors again
	 */
	if (count >= buf->num_iov) {
		hm_error("iov is too small for the desc list\n");
	}

	clear_iov(count, buf->iov, buf->flags);
	/*
	 * Discard the failed virtqueue descriptor. Otherwise the desc can not
	 * be released in the virtqueue.
	 */
	virtq->last_avail++;
	set_avail_event(virtq);
	return E_HM_AGAIN;
}

void virtq_update_used(volatile vq_used_t *vq_used, uint16_t qsize,
		       uint16_t idx, uint32_t io_len)
{
	volatile vq_used_elem_t *used_elem = NULL;
	uint16_t uidx;

	/* Get the last available position in used ring */
	uidx = vq_used->idx;
	used_elem = &vq_used->ring[uidx & (qsize - 1U)];
	uidx++;

	used_elem->id = idx;
	used_elem->len = io_len;
	wmb();

	vq_used->idx = uidx;
}

void virtq_revert_descs(struct virtqueue_s *virtq)
{
	if (virtq != NULL) {
		virtq->last_avail--;
		set_avail_event(virtq);
	}
}

static uint16_t get_used_event(struct virtqueue_s *virtq)
{
	uint16_t event = virtq->qsize;

	return virtq->avail->ring[event];
}

static __pure bool need_event_trigger(uint16_t event, uint16_t old, uint16_t new)
{
	if ((new - old) > ((new - event) - (uint16_t)1)) {
		return true;
	}

	return false;
}

static bool need_irq(struct virtqueue_s *virtq)
{
	bool ret = true;
	uint16_t new_idx, old_idx, event_idx;

	mb();
	old_idx = virtq->saved_used;
	virtq->saved_used = new_idx = virtq->used->idx;

	if (virtq_has_feature(virtq, VIRTIO_FEATURE_EVENT_IDX)) {
		event_idx = get_used_event(virtq);
		if (!need_event_trigger(event_idx, old_idx, new_idx)) {
			ret = false;
		}
	}

	return ret;
}

int virtq_send_irq(struct virtqueue_s *virtq)
{
	int ret = E_HM_OK;

	if (virtq == NULL) {
		ret = E_HM_INVAL;
	}

	if ((ret == E_HM_OK) && need_irq(virtq)) {
		ret = vhost_send_irq(virtq->device);
	}

	return ret;
}

void virtq_dump(const struct virtqueue_s *virtq)
{
	volatile uint16_t *avail_flags = NULL;
	volatile uint16_t *avail_idx = NULL;
	volatile uint16_t *used_flags = NULL;
	volatile uint16_t *used_idx = NULL;
	volatile vq_used_elem_t *used_elem = NULL;
	volatile uint16_t *event_idx = NULL;

	if ((virtq == NULL) || (virtq->desc == NULL) || (virtq->avail == NULL) ||
	    (virtq->used == NULL)) {
		return;
	}

	rmb();
	avail_flags = &virtq->avail->flags;
	avail_idx = &virtq->avail->idx;
	used_flags = &virtq->used->flags;
	used_idx = &virtq->used->idx;
	used_elem = &virtq->used->ring[virtq->qsize];
	event_idx = ulong_to_ptr(ptr_to_ulong(used_elem), uint16_t);

	hm_info("vhost virtq last avail: %u, saved used: %u\n",
		virtq->last_avail, virtq->saved_used);
	hm_info("vhost virtq avail flags: %u, avail idx: %u\n",
		*avail_flags, *avail_idx);
	hm_info("vhost virtq used flags: %u, used idx: %u, event idx: %u\n",
		*used_flags, *used_idx, *event_idx);
}
