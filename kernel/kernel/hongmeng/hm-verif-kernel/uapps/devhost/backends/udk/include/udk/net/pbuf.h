/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Structures and defines of pbuf
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#ifndef __UDK_PBUF_H__
#define __UDK_PBUF_H__

#include <lib/dlist.h>
#include <lib/utils.h>
#include <udk/sync.h>
#include <udk/device.h>
#include <hmnet/hmnet_devices.h>

struct udk_net_device;

/*
 * pbuf->va points to the memory which is shared in apps.
 * pbuf->offset means the length to reserved.
 * pbuf->len means the real payload length.
 * (offset + len) should no longer than the share memory unit length.
 * |<-------------------share memory unit---------------------|
 * |<---offset---->|<-------------len---------------->|
 */
struct pkt_buf {
	struct dlist_node	list;
	struct udk_net_device	*dev;

	unsigned char		*va;
	unsigned int		offset;
	unsigned int		len;

	/* info of share mem to storage pay load */
	unsigned long long	unit_pa;
	unsigned int		shm_offset;
};

void udk_free_pbuf(struct pkt_buf *pbuf);

static inline void free_pbuf_list(struct dlist_node *head)
{
	struct pkt_buf *pbuf = NULL;
	struct pkt_buf *tmp = NULL;

	dlist_for_each_entry_safe(pbuf, tmp, head, struct pkt_buf, list) {
		dlist_delete(&pbuf->list);
		udk_free_pbuf(pbuf);
	}
}

static inline void pbuf_enqueue(struct dlist_node *head,
				struct pkt_buf *pbuf)
{
	dlist_insert_tail(head, &pbuf->list);
}

static inline void udk_pbuf_update_offset(struct pkt_buf *pbuf, unsigned int len)
{
	pbuf->offset += len;
}

static inline void udk_pbuf_update_len(struct pkt_buf *pbuf, unsigned int len)
{
	pbuf->len += len;
}

/* pbuf ops */
struct pkt_buf *udk_dev_alloc_pbuf(struct udk_net_device *dev);
void udk_dev_free_pbuf(struct pkt_buf *pbuf);

#endif /* __UDK_PBUF_H__ */
