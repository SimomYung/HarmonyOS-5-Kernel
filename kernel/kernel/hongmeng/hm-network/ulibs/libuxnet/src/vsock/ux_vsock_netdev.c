/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implements for network vsock netdev management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 2 14:21:46 2021
 */

#include "vsock/ux_vsock_netdev.h"

#include <net/if_arp.h>
#include <lib/dlist.h>

#include <hm_ifaddrs.h>

#include "ux_netlog.h"
#include "ux_demux.h"
#include "ux_vsock_trans.h"
#include "vsock/ux_vsock_common.h"

static cid_revoke_handler_t cid_revoke_handler = NULL;

static DLIST_HEAD(vsock_netdevs);
static struct raw_mutex vsock_netdevs_lock = RAW_MUTEX_INITIALIZER;

static inline void __lock_vsock_netdevs(void)
{
	raw_mutex_lock(&vsock_netdevs_lock);
}

static inline void __unlock_vsock_netdevs(void)
{
	raw_mutex_unlock(&vsock_netdevs_lock);
}

static void __vsock_netdev_enqueue_msg(struct vsock_netdev *dev,
					      struct vm_msg *msg)
{
	raw_mutex_lock(&dev->sq_lock);
	dlist_insert_tail(&dev->send_queue, &msg->node);
	raw_mutex_unlock(&dev->sq_lock);

	vsock_netdev_wakeup(dev);
}

static void __vsock_netdev_putback_msg(struct vsock_netdev *dev,
					      struct vm_msg *msg)
{
	raw_mutex_lock(&dev->sq_lock);
	dlist_insert_head(&dev->send_queue, &msg->node);
	raw_mutex_unlock(&dev->sq_lock);
}

static struct vm_msg *__vsock_netdev_dequeue_msg(struct vsock_netdev *dev)
{
	struct dlist_node *node = NULL;
	struct vm_msg *msg = NULL;

	raw_mutex_lock(&dev->sq_lock);
	node = dlist_pop_first(&dev->send_queue);
	if (node != NULL) {
		msg = container_of(node, struct vm_msg, node);
	}
	raw_mutex_unlock(&dev->sq_lock);

	return msg;
}

static void __vsock_netdev_kill_send_thread(struct vsock_netdev *dev)
{
	if (dev->send_thread != NULL) {
		dev->running = false;
		vsock_netdev_wakeup(dev);
		(void)raw_thread_join(dev->send_thread, NULL);
		dev->send_thread = NULL;
	}
}

static void __vsock_netdev_clear_left_msgs(struct vsock_netdev *dev)
{
	struct dlist_node *node = NULL;

	raw_mutex_lock(&dev->sq_lock);

	do {
		node = dlist_pop_first(&dev->send_queue);
		if (node != NULL) {
			struct vm_msg *msg = container_of(node, struct vm_msg, node);
			free(msg);
		}
	} while (node != NULL);

	raw_mutex_unlock(&dev->sq_lock);
}

static bool __is_vsock_netdev_registered(const struct vsock_netdev *dev)
{
	bool found = false;
	struct vsock_netdev *iter = NULL;

	dlist_for_each_entry(iter, &vsock_netdevs, struct vsock_netdev, node) {
		if (iter->cid == dev->cid ||
		    iter->uvmm_cnode_idx == dev->uvmm_cnode_idx) {
			found = true;
			break;
		}
	}

	return found;
}

static int vsock_register_netdev(struct vsock_netdev *dev)
{
	int ret = E_HM_OK;

	__lock_vsock_netdevs();

	if (!vsock_is_cid_valid(dev->cid)) {
		ret = E_HM_INVAL;
	} else if (__is_vsock_netdev_registered(dev)) {
		ret = E_HM_OBJEXIST;
	} else {
		dlist_insert(&vsock_netdevs, &dev->node);
	}

	__unlock_vsock_netdevs();

	return ret;
}

static void vsock_unregister_netdev(struct vsock_netdev *dev)
{
	__lock_vsock_netdevs();

	dlist_delete(&dev->node);

	__unlock_vsock_netdevs();
}

static ssize_t __vsock_netdev_send_msg(struct vsock_netdev *vsock_dev,
				       const struct virtio_vsock_hdr *hdr,
				       const char *buf, size_t len)
{
	struct vm_msg *msg = malloc(sizeof(struct vm_msg) + len);
	ssize_t ret = (ssize_t)len;

	if (msg != NULL) {
		dlist_init(&msg->node);
		msg->len = len;
		msg->data = (char*)(msg + 1);

		NOFAIL(memcpy_s(&msg->hdr, sizeof(msg->hdr), hdr, sizeof(*hdr)));
		if (len > 0) {
			NOFAIL(memcpy_s(msg->data, len, buf, len));
		}

		__vsock_netdev_enqueue_msg(vsock_dev, msg);
	} else {
		net_warn(HMNET_VSOCK, "alloc vm_msg failed, no memory\n");
		ret = E_HM_NOMEM;
	}

	return ret;
}

ssize_t vsock_send_msg(unsigned int cid, const struct virtio_vsock_hdr *hdr,
		       const char *buf, size_t len)
{
	struct vsock_netdev *vv = NULL;
	ssize_t ret = E_HM_NODEV;

	__lock_vsock_netdevs();
	dlist_for_each_entry(vv, &vsock_netdevs, struct vsock_netdev, node) {
		if (vv->cid == cid) {
			ret = __vsock_netdev_send_msg(vv, hdr, buf, len);
			break;
		}
	}
	__unlock_vsock_netdevs();

	return ret;
}

int vsock_alloc_netdev(unsigned int cid, ux_netdev_t **rdev, size_t priv)
{
	char ifname[IF_NAMESIZE];
	ux_netdev_t *vsock_dev = NULL;
	int ret;

	if (rdev == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = vsock_format_netdev_name(cid, ifname, sizeof(ifname));
	}

	if (ret == E_HM_OK) {
		vsock_dev = ux_netdev_alloc(ifname, priv);
		if (vsock_dev == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		*rdev = vsock_dev;
	}

	return ret;
}

ux_netdev_t *vsock_get_netdev_by_cid(unsigned int cid)
{
	ux_netdev_t *vsock_dev = NULL;
	char ifname[IF_NAMESIZE];
	int ret;

	ret = vsock_format_netdev_name(cid, ifname, sizeof(ifname));
	if (ret == E_HM_OK) {
		vsock_dev = ux_netdev_get_by_name(ifname);
	}

	return vsock_dev;
}

/**
 * The vsock packet data length can not big than VSOCK_DEFAULT_RX_BUF_SIZE
 * @see the guest virtqueue fill logic
 */
static int vsock_check_packet_length(size_t iter_left, uint32_t hdr_len)
{
	const size_t size_limit = VSOCK_MAX_PKT_BUF_SIZE;
	int ret = E_HM_OK;

	if (iter_left > size_limit || (size_t)hdr_len > size_limit) {
		net_warn(HMNET_VSOCK, "invalid message left = %zu, len = %u\n", iter_left, hdr_len);
		ret = E_HM_INVAL;
	}

	return ret;
}

int vsock_copy_iov_to_vm_msg(const struct iovec *iov, size_t nr_iov,
			     struct vm_msg *msg, iov_copy_func_t copy_func)
{
	struct iov_iter iter;
	int ret;

	if (vsock_is_iov_num_invalid(nr_iov)) {
		net_warn(HMNET_VSOCK, "invalid nr_iov = %zu\n", nr_iov);
		ret = E_HM_INVAL;
	} else {
		ret = iov_iter_init(&iter, iov, nr_iov);
	}

	if (ret == E_HM_OK) {
		ssize_t copied = copy_func(&iter, (char*)&msg->hdr, sizeof(msg->hdr));
		if ((size_t)copied != sizeof(msg->hdr)) {
			net_warn(HMNET_VSOCK, "copy msg header failed copied = %d\n", (int)copied);
			ret = E_HM_RESFAULT;
		}
	}

	if (ret == E_HM_OK) {
		ret = vsock_check_packet_length(iov_iter_left(&iter), msg->hdr.len);
	}

	if (ret == E_HM_OK && msg->hdr.len > 0) {
		msg->data = malloc(msg->hdr.len);
		if (msg->data == NULL) {
			ret = E_HM_NOMEM;
		} else {
			ssize_t copied = copy_func(&iter, (char *)msg->data, msg->hdr.len);
			if ((uint32_t)copied != msg->hdr.len) {
				net_warn(HMNET_VSOCK, "copy data failed copied = %u\n", (uint32_t)copied);
				ret = E_HM_RESFAULT;
			} else {
				msg->len = msg->hdr.len;
			}
		}
	}

	if (ret != E_HM_OK) {
		if (msg->data != NULL) {
			free(msg->data);
			msg->data = NULL;
		}
	}

	return ret;
}

struct vsock_netdev_iter_args {
	uint32_t cnode_idx;
	ux_netdev_t *vsock_dev;
};

static int __vsock_netdev_iterator(const ux_netdev_t *dev, void *args)
{
	struct vsock_netdev_iter_args *iter_args = args;
	int ret = 0;

	if (dev->type == ARPHRD_VSOCKMON) {
		struct vsock_netdev *vsock_dev = ux_netdev_priv(dev);

		if (vsock_dev->uvmm_cnode_idx == iter_args->cnode_idx) {
			ux_netdev_get((ux_netdev_t*)dev);
			iter_args->vsock_dev = (ux_netdev_t*)dev;
			ret = 1;
		}
	}

	return ret;
}

ux_netdev_t *vsock_get_netdev_by_cnode_idx(uint32_t cnode_idx)
{
	struct vsock_netdev_iter_args args = { cnode_idx, NULL };

	(void)ux_netdev_for_each_device(__vsock_netdev_iterator, &args);

	return args.vsock_dev;
}

/**
 * @data is a vm_msg struct
 * @len is the real msg length not size of struct vm_msg
 *      len = msg->hdr.len + sizeof(msg->hdr)
 */
static int vsock_netdev_data_recv(ux_netdev_t *dev, const void *data, size_t len)
{
	struct vm_msg *msg = (struct vm_msg*)data;
	int ret = E_HM_OK;

	BUG_ON(dev == NULL || dev->type != ARPHRD_VSOCKMON);
	struct vsock_netdev *vdev = ux_netdev_priv(dev);

	if (msg == NULL || len != (msg->hdr.len + sizeof(msg->hdr))) {
		net_warn(HMNET_VSOCK, "invalid msg or msg->len = %zu\n", len);
		ret = E_HM_INVAL;
	} else {
		(void)ux_trans_recv_msg(msg);
		vatomic64_inc(&vdev->stat.rx_pkts);
		vatomic64_add(&vdev->stat.rx_octets, (uint64_t)msg->len);
	}

	return ret;
}

static int vsock_netdev_recv_zero_copy(ux_netdev_t *dev,
				       void *data, size_t len, size_t offset)
{
	UNUSED(offset);
	return vsock_netdev_data_recv(dev, data, len);
}

static int vsock_netdev_data_copy(ux_netdev_t *dev, const void *from, void *to,
				  size_t len, size_t offset)
{
	const struct vm_msg *msg = from;
	const size_t hdr_len = sizeof(msg->hdr);
	int ret;

	UNUSED(dev);

	ret = memcpy_s(((char*)to) + offset, len - offset, &msg->hdr, hdr_len);
	if (ret == EOK && msg->len > 0) {
		ret = memcpy_s((char*)to + offset + hdr_len, (len - offset) - hdr_len,
			       msg->data, msg->len);
	}

	return (ret == EOK) ? (int)(hdr_len + msg->len) : -1;
}

static void vsock_netdev_data_free(ux_netdev_t *dev, const void *data)
{
	UNUSED(dev, data);
}

static int vsock_netif_get_mac(ux_netdev_t *dev, unsigned char *mac, size_t len)
{
	char zero[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int ret = E_HM_OK;

	UNUSED(dev);

	if (safe_copy((char *)mac, zero, len) < 0) {
		ret = E_HM_POSIX_FAULT;
	}

	return ret;
}

static int vsock_netif_getifaddrs(ux_netdev_t *dev, char *buf, size_t size, int *cnt)
{
	struct ifaddrs_storage_internal *ifp = NULL;
	size_t ifaddrs_len = sizeof(struct ifaddrs_storage_internal);

	if (size < ifaddrs_len) {
		*cnt = 0;
		return 0;
	}

	ifp = (struct ifaddrs_storage_internal *)(void *)buf;
	mem_zero_s(*ifp);
	NOFAIL(memcpy_s(ifp->name, sizeof(ifp->name), dev->name, IFNAMSIZ));

	*cnt = 1;
	return (int)ifaddrs_len;
}

static int vsock_netif_get_mtu(ux_netdev_t *dev, int *mtu)
{
	*mtu = dev->_mtu;

	return 0;
}

static void vsock_netif_get_flags(ux_netdev_t *dev, short int *flags, const char *name)
{
	UNUSED(name);
	*flags = (short int)(dev->flags);
}

static void vsock_netif_get_ipaddr(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	UNUSED(dev, name);
	sockaddr->sin_family = AF_VSOCK;
}

static void vsock_netif_get_netmask(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	UNUSED(dev, name);
	sockaddr->sin_family = AF_VSOCK;
}

static void vsock_netif_get_dstaddr(ux_netdev_t *dev, struct sockaddr_in *sockaddr)
{
	UNUSED(dev);
	sockaddr->sin_family = AF_VSOCK;
}

static int vsock_netif_get_stats(ux_netdev_t *dev, int file_bytes, char *result)
{
	int err;
	struct vsock_netdev *vdev;

	BUG_ON(dev == NULL || dev->type != ARPHRD_VSOCKMON);
	vdev = ux_netdev_priv(dev);

	err = snprintf_s(result, (size_t)(uint32_t)file_bytes, (size_t)(uint32_t)(file_bytes - 1),
			 "%6s: %7llu %7llu %4u %4u %4u %5u %10u %9u "
			 "%8llu %7llu %4u %4u %4u %5u %7u %10u\n",
			  dev->name, vatomic64_read(&vdev->stat.rx_octets), vatomic64_read(&vdev->stat.rx_pkts),
			  0, 0, 0, 0, 0, 0,
			  vatomic64_read(&vdev->stat.tx_octets), vatomic64_read(&vdev->stat.tx_pkts),
			  0, 0, 0, 0, 0, 0);
	if (err <= 0) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int vsock_netif_get_ifconf(ux_netdev_t *dev, void *ifr_dst, int ifr_len, int *size)
{
	UNUSED(ifr_len);
	struct ifreq ifr = {0};

	if (ifr_dst == NULL) {
		*size = 1;
		return 0;
	}

	int ret = strncpy_s(ifr.ifr_ifrn.ifrn_name, IF_NAMESIZE,
			dev->name, IF_NAMESIZE - 1);
	if (ret != 0) {
		return E_HM_POSIX_FAULT;
	}

	struct sockaddr_vm *vaddr = (struct sockaddr_vm *)&ifr.ifr_ifru.ifru_addr;
	vaddr->svm_family = AF_VSOCK;

	*size = 1;
	return ux_netdev_ifr_copy_to_user(ifr_dst, &ifr, &ifr_dst);
}

static int vsock_netif_set_flags(ux_netdev_t *dev, short int flags, const char *name)
{
	UNUSED(name);
	if (((unsigned int)(int)flags & IFF_UP) != 0U) {
		dev->flags |= IFF_UP | IFF_RUNNING;
	} else {
		dev->flags &= ~(IFF_UP | IFF_RUNNING);
	}

	return 0;
}

static size_t vsock_get_packet_hdr_len(ux_netdev_t *dev, const void *data,
				    size_t len, int flags)
{
	UNUSED(dev, data, len, flags);
	return sizeof(struct ux_vsockmon_hdr);
}

static void __copy_cid_and_port(struct ux_vsockmon_hdr *vsockmon_hdr,
				const struct virtio_vsock_hdr *vsock_hdr)
{
	vsockmon_hdr->src_cid = vsock_hdr->src_cid;
	vsockmon_hdr->src_port = vsock_hdr->src_port;
	vsockmon_hdr->dst_cid = vsock_hdr->dst_cid;
	vsockmon_hdr->dst_port = vsock_hdr->dst_port;
}

static void __convert_header_ops(struct ux_vsockmon_hdr *vsockmon_hdr,
				 const struct virtio_vsock_hdr *vsock_hdr)
{
	switch (vsock_hdr->op) {
	case VIRTIO_VSOCK_OP_REQUEST:
	case VIRTIO_VSOCK_OP_RESPONSE:
		vsockmon_hdr->op = UX_VSOCK_OP_CONNECT;
		break;
	case VIRTIO_VSOCK_OP_RST:
	case VIRTIO_VSOCK_OP_SHUTDOWN:
		vsockmon_hdr->op = UX_VSOCK_OP_DISCONNECT;
		break;
	case VIRTIO_VSOCK_OP_RW:
		vsockmon_hdr->op = UX_VSOCK_OP_PAYLOAD;
		break;
	case VIRTIO_VSOCK_OP_CREDIT_UPDATE:
	case VIRTIO_VSOCK_OP_CREDIT_REQUEST:
		vsockmon_hdr->op = UX_VSOCK_OP_CONTROL;
		break;
	default:
		vsockmon_hdr->op = UX_VSOCK_OP_UNKNOWN;
		break;
	}
}

static int vsock_set_packet_hdr(ux_netdev_t *dev, void *hdr,
				const void *data, size_t len, int flags)
{
	struct vm_msg *msg = (struct vm_msg*)data;
	struct virtio_vsock_hdr *vsock_hdr = &msg->hdr;
	struct ux_vsockmon_hdr *vsockmon_hdr = (struct ux_vsockmon_hdr*)hdr;

	UNUSED(dev, flags);

	BUG_ON(len < VIRTIO_VSOCK_HDR_LEN);

	__copy_cid_and_port(vsockmon_hdr, vsock_hdr);
	vsockmon_hdr->transport = UX_VSOCK_TRANSPORT_VIRTIO;
	vsockmon_hdr->len = VIRTIO_VSOCK_HDR_LEN;
	NOFAIL(memset_s(vsockmon_hdr->reserved, sizeof(vsockmon_hdr->reserved),
			0, sizeof(vsockmon_hdr->reserved)));
	__convert_header_ops(vsockmon_hdr, vsock_hdr);

	return E_HM_OK;
}

static ux_netdev_ops_priv_t vsock_netdev_priv_ops = {
	.dump_stat = NULL,
	.get_mac = vsock_netif_get_mac,
	.getifaddrs = vsock_netif_getifaddrs,
	.get_mtu = vsock_netif_get_mtu,
	.get_flags = vsock_netif_get_flags,
	.get_ipaddr = vsock_netif_get_ipaddr,
	.get_netmask = vsock_netif_get_netmask,
	.get_dstaddr = vsock_netif_get_dstaddr,
	.get_stats = vsock_netif_get_stats,
	.get_ifconf = vsock_netif_get_ifconf,

	.set_mac = NULL,
	.set_ipaddr = NULL,
	.del_ipaddr = NULL,
	.set_netmask = NULL,
	.set_dstaddr = NULL,
	.set_flags = vsock_netif_set_flags,
	.set_mtu = NULL,
	.set_rxhwcsum = NULL,

	.recv = vsock_netdev_data_recv,
	.recv_zero_copy = vsock_netdev_recv_zero_copy,
	.copy = vsock_netdev_data_copy,
	.free = vsock_netdev_data_free,
	.add_header = NULL,
	.remove_header = NULL,

	.get_packet_hdr_len = vsock_get_packet_hdr_len,
	.set_packet_hdr = vsock_set_packet_hdr,
};

static void *vsock_send_thread(void *arg)
{
	struct vsock_netdev *vsock_dev = (struct vsock_netdev*)arg;
	int ret = E_HM_OK;

	while (vsock_dev->running) {
		struct vm_msg *msg = __vsock_netdev_dequeue_msg(vsock_dev);
		if (msg != NULL) {
			const int msg_len = (int)(sizeof(msg->hdr) + msg->hdr.len);
			ux_netdev_t *dev = ux_netdev_from_priv(vsock_dev);

			ret = ux_netdev_xmit(dev, msg, msg_len, 0);
			if (ret < 0) {
				__vsock_netdev_putback_msg(vsock_dev, msg);
			} else {
				vatomic64_inc(&vsock_dev->stat.tx_pkts);
				vatomic64_add(&vsock_dev->stat.tx_octets, (uint64_t)msg->len);
				free(msg);
			}
		}

		if (msg == NULL || ret == E_HM_AGAIN) {
			(void)ux_sem_cond_waiti(&vsock_dev->sq_sem, LONG_MAX);
		}
	}

	return NULL;
}

static int vsock_device_init(ux_netdev_t *dev)
{
	struct vsock_netdev *vsock_dev = NULL;
	int ret;

	if (dev != NULL) {
		vsock_dev = ux_netdev_priv(dev);
	}
	if (vsock_dev == NULL) {
		ret = E_HM_INVAL;
	} else {
		vsock_dev->send_thread = NULL;
		vsock_dev->running = true;
		dlist_init(&vsock_dev->node);
		dlist_init(&vsock_dev->send_queue);
		(void)ux_sem_init(&vsock_dev->sq_sem, 0);
		raw_mutex_init(&vsock_dev->sq_lock);

		ret = raw_thread_create(&vsock_dev->send_thread, NULL,
					vsock_send_thread, vsock_dev);
	}

	if (ret == E_HM_OK) {
		ret = vsock_register_netdev(vsock_dev);
	}

	if (ret == E_HM_OK) {
		dev->priv_ops = &vsock_netdev_priv_ops;
		dev->type = ARPHRD_VSOCKMON;
		dev->flags = IFF_NOARP | IFF_UP;
		dev->_mtu = 0;
	} else {
		if (vsock_dev != NULL) {
			__vsock_netdev_kill_send_thread(vsock_dev);
		}
	}

	return ret;
}

static void vsock_device_uninit(const ux_netdev_t *dev)
{
	struct vsock_netdev *vsock_dev = NULL;
	if (dev == NULL) {
		return;
	}
	vsock_dev = ux_netdev_priv(dev);
	if (vsock_dev != NULL) {
		vsock_unregister_netdev(vsock_dev);
		__vsock_netdev_kill_send_thread(vsock_dev);
		__vsock_netdev_clear_left_msgs(vsock_dev);

		if (cid_revoke_handler != NULL) {
			cid_revoke_handler(vsock_dev->cid);
		}
	}
}

static ux_packet_demux_t vsock_demux = {
	.head = DLIST_HEAD_INIT(vsock_demux.head),
	.type = ARPHRD_VSOCKMON,
	.init = vsock_device_init,
	.uninit = vsock_device_uninit,
	.func = NULL,
	.dev = NULL,

	.find_ip_reachable = NULL,
	.set_route_entry = NULL,
	.set_default = NULL,
};

int vsock_netdev_init(cid_revoke_handler_t handler)
{
	int ret;

	cid_revoke_handler = handler;
	ret = ux_register_packet_demux(&vsock_demux);
	if (ret != E_HM_OK) {
		net_error(HMNET_VSOCK, "vsock device register demux failed\n");
		return ret;
	}

	return 0;
}

void vsock_netdev_exit(void)
{
	ux_unregister_packet_demux(&vsock_demux);
}
