/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for vsock netdev interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun  2 14:21:46 2021
 */

#ifndef __UX_VSOCK_NETDEV_H_
#define __UX_VSOCK_NETDEV_H_

#include <stdlib.h>
#include <securec.h>
#include <lib/dlist.h>

#include <vsync/atomic.h>
#include <libhmiov/iov_iter.h>
#include <libhmsync/raw_thread.h>

#include <ux_netdev.h>
#include <vsock/ux_vsock_common.h>

struct vsock_stat {
	vatomic64_t rx_pkts;
	vatomic64_t rx_octets;
	vatomic64_t tx_pkts;
	vatomic64_t tx_octets;
};

struct vsock_netdev {
	struct dlist_node node;

	/**
	 * @cid is the identifier of each uvmm and must be unique.
	 */
	unsigned int cid;
	uint32_t uvmm_cnode_idx;

	raw_thread_t send_thread;
	bool running;

	struct raw_sem sq_sem;
	struct raw_mutex sq_lock;
	struct dlist_node send_queue;

	struct vsock_stat stat;
};

#define VSOCK_PACKET_MAX_IOV_NUM	2
#define VSOCK_NETDEV_NAME_FORMAT	"vsock%u"

static inline int vsock_format_netdev_name(unsigned int cid, char *name, size_t len)
{
	int ret = sprintf_s(name, len, VSOCK_NETDEV_NAME_FORMAT, cid);
	return ret > 0 ? E_HM_OK : E_HM_RESFAULT;
}

/**
 * The virtqueue desc is a two buffer chain, first buffer is only
 * 44 bytes contains the virtio_vsock_hdr, and the second buffer
 * contains the real data and can be empty.
 * So here the iov can not be greater than 2.
 */
static inline bool vsock_is_iov_num_invalid(size_t nr)
{
	return nr > VSOCK_PACKET_MAX_IOV_NUM;
}

static inline void vsock_netdev_wakeup(struct vsock_netdev *dev)
{
	int ret = raw_sem_post(&dev->sq_sem);
	if (ret != E_HM_OK) {
		net_warn(HMNET_VSOCK, "wake up send thread failed ret = %s\n", hmstrerror(ret));
	}
}

/**
 * vhost or actv netdev both process iovec messages, the process sequence is very
 * similar but has little difference with iov iterator data copy,
 * so we abstract this function here to avoid duplicated codes.
 */
typedef ssize_t (*iov_copy_func_t)(struct iov_iter *iter, char *data, size_t len);
int vsock_copy_iov_to_vm_msg(const struct iovec *iov, size_t nr_iov,
			     struct vm_msg *msg, iov_copy_func_t copy_func);

int vsock_alloc_netdev(unsigned int cid, ux_netdev_t **rdev, size_t priv);
ux_netdev_t *vsock_get_netdev_by_cnode_idx(uint32_t cnode_idx);
ux_netdev_t *vsock_get_netdev_by_cid(unsigned int cid);

int vsock_netdev_init(cid_revoke_handler_t handler);
void vsock_netdev_exit(void);
#endif
