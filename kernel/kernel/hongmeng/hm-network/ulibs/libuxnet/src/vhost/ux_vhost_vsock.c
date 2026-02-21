/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implements for vsock vhost backend
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 23 15:16:13 2021
 */

#include <net/if_arp.h>

#include "vhost/ux_vhost.h"
#include "vsock/ux_vsock_netdev.h"

#define VIRTIO_ID_VSOCK 		19
#define VIRTQ_VSOCK_SIZE_MAX		256

enum {
	VSOCK_VQ_RX,
	VSOCK_VQ_TX,
	VSOCK_VQ_EV,
	VSOCK_VQ_NUM
};

struct vsock_vhost_netdev {
	struct vsock_netdev dev;

	struct vhost_device_s *device;
};

static int vsock_handle_vq_ready(int32_t vq_index, struct vhost_device_s *d,
				 struct virtqueue_s *vq);
static int vsock_device_done(struct vhost_backend_s *b, struct vhost_device_s *d);
static void vsock_device_destroy(struct vhost_backend_s *b, struct vhost_device_s *d);

static struct vhost_backend_s vsock_backend = {
	.virtio_id = VIRTIO_ID_VSOCK,
	.vq_num = VSOCK_VQ_NUM,
	.handle_vq_ready = vsock_handle_vq_ready,
	.device_done = vsock_device_done,
	.device_destroy = vsock_device_destroy,
};

int ux_vhost_vsock_init(void)
{
	return ux_vhost_register_backend(&vsock_backend);
}

static int __copy_iov_to_vm_msg(const struct iovec *iov, size_t nr_iov,
				struct vm_msg *msg)
{
	const iov_copy_func_t copy_func = copy_data_from_iter;

	return vsock_copy_iov_to_vm_msg(iov, nr_iov, msg, copy_func);
}

static int vsock_recv_iov(unsigned int cid, struct iovec *iov, int nr)
{
	ux_netdev_t *vsock_netdev = vsock_get_netdev_by_cid(cid);
	struct vm_msg msg = { 0 };
	int ret;

	if (vsock_netdev == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = __copy_iov_to_vm_msg(iov, nr, &msg);
	}

	if (ret == E_HM_OK) {
		if (msg.hdr.src_cid != cid) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_netdev_receive_zero_copy(vsock_netdev, &msg,
						  sizeof(msg.hdr) + msg.len, 0, UXDEV_PKT_STRUCT);
	}

	/**
	 * if msg.data is not null, it means processing error, so we must free the data here.
	 */
	if (msg.data != NULL) {
		free(msg.data);
	}

	if (vsock_netdev != NULL) {
		ux_netdev_put(vsock_netdev);
	}

	return ret == E_HM_OK ? (sizeof(msg.hdr) + msg.hdr.len): ret;
}

static int vsock_handle_tq(struct virtqueue_s *vq, void *opaque)
{
	struct vhost_device_s *device = (struct vhost_device_s*)opaque;
	uint16_t flags[VIRTQ_VSOCK_SIZE_MAX];
	struct iovec iov[VIRTQ_VSOCK_SIZE_MAX];
	int total_updated = 0;
	int ret;

	while (virtq_has_desc(vq)) {
		struct virtq_buffer_s buff = {
			.desc_count = 0,
			.num_iov = VIRTQ_VSOCK_SIZE_MAX,
			.iov = iov,
			.flags = flags,
			.head_idx = 0
		};

		ret = virtq_get_descs(vq, &buff);
		if (ret == E_HM_AGAIN) {
			continue;
		} else if (ret != E_HM_OK) {
			net_warn(HMNET_VHOST, "get desc failed\n");
			break;
		}

		ret = vsock_recv_iov(device->device_id, buff.iov, buff.desc_count);
		if (ret > 0) {
			total_updated += ret;
			virtq_update_used(vq->used, vq->qsize,
					  buff.head_idx, (uint32_t)ret);
		}
	}

	if (total_updated > 0) {
		ret = vhost_send_irq(device);
		if (ret != E_HM_OK) {
			net_warn(HMNET_VHOST, "send irq failed ret = %s\n", hmstrerror(ret));
		}
	}

	return E_HM_OK;
}

static int vsock_handle_rq(struct virtqueue_s *vq, void *opaque)
{
	struct vhost_device_s *device = (struct vhost_device_s*)opaque;
	ux_netdev_t *netdev = vsock_get_netdev_by_cid(device->device_id);

	if (netdev != NULL) {
		struct vsock_netdev *vhost_dev = ux_netdev_priv(netdev);
		vsock_netdev_wakeup(vhost_dev);
	}

	vq->ready = true;
	return E_HM_OK;
}

static int vsock_handle_ev(struct virtqueue_s *vq, void *opaque)
{
	UNUSED(vq, opaque);
	net_warn(HMNET_VHOST, "!!! do not support vsock event now\n");
	return 0;
}

static int vsock_handle_vq_ready(int32_t vq_index, struct vhost_device_s *d,
				 struct virtqueue_s *vq)
{
	int ret = E_HM_OK;

	switch (vq_index) {
	case VSOCK_VQ_RX:
		vq->handle_kick = vsock_handle_rq;
		vq->opaque = d;
		break;
	case VSOCK_VQ_TX:
		vq->handle_kick = vsock_handle_tq;
		vq->opaque = d;
		break;
	case VSOCK_VQ_EV:
		vq->handle_kick = vsock_handle_ev;
		vq->opaque = d;
		break;
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

static ssize_t __send_msg_to_iov(struct vm_msg *msg, struct iovec *iov, int nr)
{
	struct iov_iter iter;
	ssize_t copied = 0;
	int ret;

	ret = iov_iter_init(&iter, iov, nr);
	if (ret == E_HM_OK) {
		/* header must be totally copied */
		ret = copy_data_to_iter(&iter, (void *)(&msg->hdr), sizeof(msg->hdr));
		if (ret != sizeof(msg->hdr)) {
			net_warn(HMNET_VHOST, "virtio header not totally copied\n");
			ret = E_HM_POSIX_FAULT;
		} else {
			copied += ret;
			ret = E_HM_OK;
		}
	}

	if (ret == E_HM_OK && msg->len > 0) {
		ret = copy_data_to_iter(&iter, msg->data, msg->len);
		if (ret <= 0) {
			net_warn(HMNET_VHOST, "copy buf data failed\n");
			ret = E_HM_POSIX_FAULT;
		} else {
			copied += ret;
			ret = E_HM_OK;
		}
	}

	return (ret == E_HM_OK) ? copied : ret;
}

static ssize_t __vhost_send_msg(struct vsock_vhost_netdev *vhost_dev, struct vm_msg *msg)
{
	struct vhost_device_s *device = vhost_dev->device;
	struct virtqueue_s *vq = &device->vqs[VSOCK_VQ_RX];
	uint16_t aflags[VIRTQ_VSOCK_SIZE_MAX];
	struct iovec iov[VIRTQ_VSOCK_SIZE_MAX];
	ssize_t copied;
	struct virtq_buffer_s buff = {
		.desc_count = 0,
		.num_iov = VIRTQ_VSOCK_SIZE_MAX,
		.iov = iov,
		.flags = aflags,
		.head_idx = 0
	};
	int ret = E_HM_AGAIN;

	while (virtq_has_desc(vq)) {
		ret = virtq_get_descs(vq, &buff);
		if (ret == E_HM_AGAIN) {
			continue;
		} else {
			break;
		}
	}

	if (ret == E_HM_OK) {
		copied = __send_msg_to_iov(msg, iov, buff.desc_count);
		if (copied < 0) {
			ret = (int)copied;
		}
	}

	if (ret == E_HM_OK) {
		virtq_update_used(vq->used, vq->qsize, buff.head_idx, (uint32_t)copied);
		if (vhost_send_irq(device) != E_HM_OK) {
			net_warn(HMNET_VHOST, "failed to send vhost virtqueue irq\n");
		}
	}

	return (ret == E_HM_OK) ? (copied - sizeof(msg->hdr)): ret;
}

static int vsock_vhost_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	struct vsock_vhost_netdev *vhost_netdev = ux_netdev_priv(dev);
	struct vm_msg *msg = (struct vm_msg*)data;
	int ret = E_HM_OK;

	if (dev == NULL || vhost_netdev == NULL ||
	    data == NULL || len != (sizeof(msg->hdr) + msg->hdr.len) ) {
		ret = E_HM_INVAL;
	} else {
		ret = __vhost_send_msg(vhost_netdev, msg);
	}

	return ret;
}

static ux_netdev_ops_t vsock_vhost_ops = {
	.ndo_init = NULL,
	.ndo_set_mtu = NULL,
	.ndo_ioctl = NULL,
	.ndo_xmit = vsock_vhost_xmit,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_set_mac = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

static int vsock_device_done(struct vhost_backend_s *backend,
			     struct vhost_device_s *device)
{
	unsigned int cid = device->device_id;
	ux_netdev_t *vsock_dev = NULL;
	int ret;

	ret = vsock_alloc_netdev(cid, &vsock_dev, sizeof(struct vsock_vhost_netdev));
	if (ret == E_HM_OK) {
		struct vsock_vhost_netdev *vhost_netdev = ux_netdev_priv(vsock_dev);

		vsock_dev->ops = &vsock_vhost_ops;
		vsock_dev->type = ARPHRD_VSOCKMON;

		vhost_netdev->dev.cid = cid;
		vhost_netdev->dev.uvmm_cnode_idx = device->cnode_idx;
		vhost_netdev->device = device;

		ret = ux_netdev_register(vsock_dev);
	}

	if (ret != E_HM_OK) {
		if (vsock_dev != NULL) {
			ux_netdev_free(vsock_dev);
		}
	}

	return ret;
}

static void vsock_device_destroy(struct vhost_backend_s *b, struct vhost_device_s *d)
{
	ux_netdev_t *vsock_dev = vsock_get_netdev_by_cnode_idx(d->cnode_idx);

	if (vsock_dev != NULL) {
		ux_netdev_unregister(vsock_dev);
		ux_netdev_put(vsock_dev);
	}
}
