/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vnet vhost backend impl
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 23 17:02:49 2021
 */
#include <net/if_arp.h>

#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <libhmiov/iov_iter.h>
#include <libvhost_user/vhost_server.h>

#include "vhost/ux_vhost.h"
#include "ux_netdev.h"
#include "hm_socket.h"

#define VIRTQUEUE_NET_MAX	256
#define VIRTIO_NET_HDR		12
#define VIRTIO_ID_NET		1

enum {
	VNET_QUEUE_RX,
	VNET_QUEUE_TX,
	VNET_QUEUE_MAX,
};

#define vq_buff_init(vq_buff, _iov, iovcnt, _flags) \
do {\
	(vq_buff)->desc_count = 0; \
	(vq_buff)->num_iov = (iovcnt); \
	(vq_buff)->iov = (_iov); \
	(vq_buff)->flags = (_flags); \
	(vq_buff)->head_idx = 0; \
} while (0)

struct vnet_device_s {
	ux_netdev_t *ndev;
	struct vhost_device_s *vhost_device;
};

static inline void *ux_netdev_priv_vnet(const ux_netdev_t *dev)
{
	return (char *)dev + sizeof(ux_netdev_t) + UX_NETDEV_PRIV_DEFAULT;
}

static int vnet_netdev_recv_from_iov(struct vnet_device_s *vnet_device,
				     struct iovec *iov, int iovcnt)
{
	int ret;
	size_t size;
	size_t len;
	char *data = NULL;
	struct iov_iter iter = {0};

	ret = iov_iter_init(&iter, iov, (unsigned long)(unsigned)iovcnt);
	if (ret == E_HM_OK) {
		size = iov_iter_left(&iter);
		if (size > MAX_MSG_SIZE) {
			ret = E_HM_2BIG;
		}
		if (size == 0) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		data = calloc(1, size);
		if (data == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		len = (size_t)copy_data_from_iter(&iter, data, size);
		if (len <= VIRTIO_NET_HDR) {
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_netdev_receive(vnet_device->ndev,
					data + VIRTIO_NET_HDR, size - VIRTIO_NET_HDR, UXDEV_PKT_RAW);
		if (ret > 0) {
			ret = size;
		}
	}

	if (data != NULL) {
		free(data);
	}

	return ret;
}

static int vnet_handle_rq(struct virtqueue_s *vq, void *opaque)
{
	UNUSED(vq);
	UNUSED(opaque);

	return E_HM_OK;
}

static inline struct vnet_device_s *vnet_device_of(struct vhost_device_s *device)
{
	if (device == NULL) {
		return NULL;
	}
	return (struct vnet_device_s *)device->priv;
}

static int vnet_handle_tq(struct virtqueue_s *vq, void *opaque)
{
	int ret;
	uint16_t flags[VIRTQUEUE_NET_MAX];
	struct virtq_buffer_s vq_buf;
	struct iovec iov[VIRTQUEUE_NET_MAX];
	struct vhost_device_s *device = (struct vhost_device_s*)opaque;
	struct vnet_device_s *vnet_device = vnet_device_of(device);

	if (vnet_device == NULL) {
		return E_HM_INVAL;
	}

	while (virtq_has_desc(vq)) {
		vq_buff_init(&vq_buf, iov, VIRTQUEUE_NET_MAX, flags);

		ret = virtq_get_descs(vq, &vq_buf);
		if (ret == E_HM_AGAIN) {
			continue;
		}
		if (ret != E_HM_OK) {
			net_error(HMNET_VHOST, "get desc failed\n");
			break;
		}

		ret = vnet_netdev_recv_from_iov(vnet_device,
						vq_buf.iov, vq_buf.desc_count);
		if (ret < 0) {
			virtq_update_used(vq->used, vq->qsize, vq_buf.head_idx, 0);
			break;
		}

		virtq_update_used(vq->used, vq->qsize, vq_buf.head_idx, ret);
	}

	ret = vhost_send_irq(device);
	if (ret != E_HM_OK) {
		net_warn(HMNET_VHOST, "send vhost virtqueue irq failed\n");
	}

	return ret;
}

static int vnet_handle_vq_ready(int32_t vq_index, struct vhost_device_s *d,
				 struct virtqueue_s *vq)
{
	int ret = E_HM_OK;

	switch (vq_index) {
	case VNET_QUEUE_RX:
		vq->handle_kick = vnet_handle_rq;
		vq->opaque = d;
		break;
	case VNET_QUEUE_TX:
		vq->handle_kick = vnet_handle_tq;
		vq->opaque = d;
		break;
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

static int vnet_netdev_send_to_iov(struct iovec *iov, int iovcnt, char *buf, int size)
{
	int ret = E_HM_OK;
	struct iov_iter iter = {0};
	char hdrbuf[VIRTIO_NET_HDR] = {0};
	size_t copylen;
	size_t hdr = VIRTIO_NET_HDR;
	ssize_t len;

	ret = iov_iter_init(&iter, iov, iovcnt);
	if (ret == E_HM_OK) {
		if (iov_iter_left(&iter) < VIRTIO_NET_HDR) {
			net_error(HMNET_VHOST, "iov length is too small\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		/* fill hdr bytes zero */
		len = copy_data_to_iter(&iter, hdrbuf, hdr);
		if (len != (ssize_t)hdr) {
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		copylen = iov_iter_left(&iter);
		if (copylen > (size_t)(unsigned)size) {
			copylen = (size_t)(unsigned)size;
		}
		ret = copy_data_to_iter(&iter, buf, copylen);
		if (ret != (int)(unsigned)copylen) {
			ret = E_HM_POSIX_FAULT;
		} else {
			ret += (int)len;
		}
	}

	return ret;
}

static int __vnet_ndo_xmit(struct vnet_device_s *vnet_device,
			    struct virtqueue_s *vq, int len, char *vmbuf)
{
	int ret;
	char *tmpbuf = vmbuf;
	uint16_t vq_flags[VIRTQUEUE_NET_MAX];
	struct iovec iov[VIRTQUEUE_NET_MAX];
	struct virtq_buffer_s vq_buf;

	if (!virtq_has_desc(vq)) {
		return E_HM_OVERFLOW;
	}

	vq_buff_init(&vq_buf, iov, VIRTQUEUE_NET_MAX, vq_flags);
	ret = virtq_get_descs(vq, &vq_buf);
	if (ret != E_HM_OK) {
		net_error(HMNET_VHOST, "failed to get virtqueue descriptor\n");
		return ret;
	}

	ret = vnet_netdev_send_to_iov(vq_buf.iov, vq_buf.desc_count, tmpbuf, len);
	if (ret < 0) {
		virtq_update_used(vq->used, vq->qsize, vq_buf.head_idx, 0);
	} else {
		virtq_update_used(vq->used, vq->qsize, vq_buf.head_idx, ret);
		ret = E_HM_OK;
	}

	if (vhost_send_irq(vnet_device->vhost_device) != E_HM_OK) {
		net_warn(HMNET_VHOST, "send vhost virtqueue irq failed\n");
	}

	return ret;
}

static int vnet_netdev_ndo_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	int ret = E_HM_OK;
	char *vmbuf = NULL;
	ux_copy_data_args_t args;
	struct virtqueue_s *vq = NULL;
	struct vnet_device_s *vnet_device = NULL;

	vnet_device = ux_netdev_priv_vnet(dev);
	if (vnet_device == NULL || vnet_device->vhost_device == NULL) {
		ret = E_HM_INVAL;
	}
	if (len > MAX_MSG_SIZE) {
		ret = E_HM_2BIG;
	}
	if (ret == E_HM_OK) {
		vq = &(vnet_device->vhost_device->vqs[VNET_QUEUE_RX]);
		/*
		 * NOTE: Copy data to vmbuf, then call iov interface
		 * to put buf into iov now; for optimizing performance,
		 * we will eliminate memory allocation later
		 */
		vmbuf = malloc(len);
		if (vmbuf == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		args.dst = (void *)vmbuf;
		args.dstlen = len;
		args.src = (const void *)data;
		args.count = len;
		args.offset = 0;
		args.flags = flags;
		ret = ux_netdev_copy_data(dev, &args);
		if (ret <= 0) {
			ret = E_HM_POSIX_FAULT;
		} else {
			ret = __vnet_ndo_xmit(vnet_device, vq, ret, vmbuf);
		}
	}

	if (vmbuf != NULL) {
		free(vmbuf);
	}

	return ret;
}

static ux_netdev_ops_t vnet_ops = {
	.ndo_init = NULL,
	.ndo_xmit = vnet_netdev_ndo_xmit,
	.ndo_set_mtu = NULL,
	.ndo_set_mac = NULL,
	.ndo_ioctl = NULL,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

static int vnet_device_done(struct vhost_backend_s *bend, struct vhost_device_s *dev)
{
	int ret = E_HM_OK;
	ux_netdev_t *netdev = NULL;
	struct vnet_device_s *vnet_device = NULL;

	if (bend == NULL || dev == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		netdev = ux_netdev_alloc(dev->dev_name,
					 sizeof(struct vhost_device_s) + UX_NETDEV_PRIV_DEFAULT);
		if (netdev == NULL) {
			ret = E_HM_NOMEM;
		} else {
			netdev->type = ARPHRD_ETHER;
			netdev->ops = &vnet_ops;
			vnet_device = ux_netdev_priv_vnet(netdev);
			dev->priv = vnet_device;
			vnet_device->vhost_device = dev;
			vnet_device->ndev = netdev;

			ux_rtnl_lock();
			ret = ux_netdev_register(netdev);
			ux_rtnl_unlock();
			if (ret != E_HM_OK) {
				ux_netdev_free(netdev);
			}
		}
	}

	return ret;
}

static void vnet_device_destroy(struct vhost_backend_s *bend, struct vhost_device_s *dev)
{
	ux_netdev_t *netdev = NULL;
	struct vnet_device_s *vnet_device = NULL;

	if (bend == NULL || dev == NULL || dev->priv == NULL) {
		return;
	}

	vnet_device = dev->priv;
	netdev = vnet_device->ndev;
	if (netdev != NULL) {
		ux_netdev_unregister(netdev);
	}
}

struct vhost_backend_s vnet_backend = {
	.virtio_id = VIRTIO_ID_NET,
	.vq_num = VNET_QUEUE_MAX,
	.handle_vq_ready = vnet_handle_vq_ready,
	.device_done = vnet_device_done,
	.device_destroy = vnet_device_destroy,
};

int ux_vhost_vnet_init(void)
{
	int ret;

	ret = ux_vhost_register_backend(&vnet_backend);
	if (ret != E_HM_OK) {
		hm_warn("vnet vhost_user backend register failed\n");
	}

	return ret;
}
