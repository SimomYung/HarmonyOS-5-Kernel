/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implements for network vsock actv netdev
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 2 14:21:46 2021
 */

#include <net/if_arp.h>
#include <ux_netlog.h>
#include <libhmsrv_sys/hm_vm.h>
#include <libhmsrv_sys/hm_path.h>

#include "ux_vsock_core.h"
#include "vsock/ux_vsock_netdev.h"
#include "vsock/ux_vsock_actv_netdev.h"

/**
 * netdev registered by actv-calls
 */
struct vsock_actv_netdev {
	struct vsock_netdev dev;

	char uvmm_path[PATHMGR_PATH_NAME_MAX];
	rref_t uvmm_rref;

	ssize_t (*send_msg)(struct vsock_actv_netdev *dev, struct vm_msg *msg);
};

static ssize_t __init_and_send_msg(struct vsock_actv_netdev *actv_netdev, struct vm_msg *msg);
static void __release_actv_netdev(struct vsock_actv_netdev *actv_netdev);

static void __release_actv_netdev(struct vsock_actv_netdev *actv_netdev)
{
	if (actv_netdev != NULL) {
		if (actv_netdev->uvmm_rref != 0) {
			(void)hm_path_release(actv_netdev->uvmm_path, actv_netdev->uvmm_rref);
			actv_netdev->uvmm_rref = 0;
		}
	}
}

static ssize_t __send_msg(struct vsock_actv_netdev *actv_netdev, struct vm_msg *msg)
{
	hm_actv_set_accessible_mem_merge_2_range(ptr_to_ulong(&(msg->hdr)), VIRTIO_VSOCK_HDR_LEN,
						 ptr_to_ulong(msg->data), msg->len);
	return hm_vsock_vm_rx(actv_netdev->uvmm_rref, (void *)&(msg->hdr),
			      VIRTIO_VSOCK_HDR_LEN, msg->data, msg->len);
}

static int __get_uvmm_rref(struct vsock_actv_netdev *actv_netdev)
{
	return hm_path_acquire(actv_netdev->uvmm_path, &actv_netdev->uvmm_rref);
}

static ssize_t __init_and_send_msg(struct vsock_actv_netdev *actv_netdev, struct vm_msg *msg)
{
	ssize_t ret = (ssize_t)__get_uvmm_rref(actv_netdev);
	if (ret == E_HM_OK) {
		actv_netdev->send_msg = __send_msg;

		ret = __send_msg(actv_netdev, msg);
	}

	return ret;
}

static int vsock_actv_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	struct vsock_actv_netdev *actv_netdev = NULL;
	struct vm_msg *msg = NULL;
	int ret;

	UNUSED(flags);
	if (dev == NULL || data == NULL) {
		return E_HM_INVAL;
	}

	actv_netdev = ux_netdev_priv(dev);
	msg = (struct vm_msg*)data;
	if (actv_netdev == NULL || len != (int)(sizeof(msg->hdr) + msg->hdr.len)) {
		ret = E_HM_INVAL;
	} else {
		ret = (int)actv_netdev->send_msg(actv_netdev, msg);
	}

	return ret;
}

static ux_netdev_ops_t vsock_actv_ops = {
	.ndo_init = NULL,
	.ndo_set_mtu = NULL,
	.ndo_ioctl = NULL,
	.ndo_xmit = vsock_actv_xmit,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_set_mac = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

int vsock_register_actv_netdev(unsigned int cid, unsigned int dev_group_id,
			       uint32_t uvmm_cnode_idx)
{
	ux_netdev_t *vsock_dev = NULL;
	int ret;

	if (!vsock_is_cid_valid(cid)) {
		ret = E_HM_INVAL;
	} else {
		ret = vsock_alloc_netdev(cid, &vsock_dev, sizeof(struct vsock_actv_netdev));
	}

	if (ret == E_HM_OK) {
		struct vsock_actv_netdev *actv_netdev = ux_netdev_priv(vsock_dev);

		ret = sprintf_s(actv_netdev->uvmm_path, sizeof(actv_netdev->uvmm_path),
				 "uvmm-ap-path-%u", dev_group_id);
		if (ret > 0) {
			vsock_dev->ops = &vsock_actv_ops;
			vsock_dev->type = ARPHRD_VSOCKMON;

			actv_netdev->dev.cid = cid;
			actv_netdev->dev.uvmm_cnode_idx = uvmm_cnode_idx;
			actv_netdev->send_msg = __init_and_send_msg;

			ret = ux_netdev_register(vsock_dev);
		} else {
			ret = E_HM_RESFAULT;
		}
	}

	if (ret != E_HM_OK) {
		if (vsock_dev != NULL) {
			ux_netdev_free(vsock_dev);
		}
	}

	return ret;
}

static void __unregister_actv_netdev(ux_netdev_t *vsock_dev, uint32_t uvmm_cnode_idx)
{
	struct vsock_actv_netdev *actv_netdev = ux_netdev_priv(vsock_dev);

	if (actv_netdev->dev.uvmm_cnode_idx == uvmm_cnode_idx) {
		ux_netdev_unregister(vsock_dev);
		__release_actv_netdev(actv_netdev);
	}
}

void vsock_unregister_actv_netdev(uint32_t uvmm_cnode_idx, unsigned int cid)
{
	ux_netdev_t *vsock_dev = NULL;

	vsock_dev = vsock_get_netdev_by_cid(cid);
	if (vsock_dev != NULL && vsock_dev->type == ARPHRD_VSOCKMON) {
		__unregister_actv_netdev(vsock_dev, uvmm_cnode_idx);
	}

	if (vsock_dev != NULL) {
		ux_netdev_put(vsock_dev);
	}
}

static int __convert_iov_to_vm_msg(const struct iovec *caller_iov, size_t nr_iov,
				   struct vm_msg *msg)
{
	struct iovec iov[VSOCK_PACKET_MAX_IOV_NUM];
	int ret;

	if (vsock_is_iov_num_invalid(nr_iov)) {
		ret = E_HM_INVAL;
	} else {
		ret = hm_actv_read_caller_vm(iov, caller_iov,
					     nr_iov * sizeof(struct iovec));
		if (ret != E_HM_OK) {
			net_warn(HMNET_VSOCK, "read caller iov failed ret = %s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		iov_copy_func_t copy_func = copy_data_from_caller_iter;

		ret = vsock_copy_iov_to_vm_msg(iov, nr_iov, msg, copy_func);
	}

	return ret;
}

int vsock_actv_netdev_tx(uint32_t uvmm_cnode_idx, unsigned int cid,
			 const struct iovec *caller_iov, size_t nr_iov)
{
	struct vsock_netdev *actv_netdev = NULL;
	ux_netdev_t *vsock_dev = NULL;
	struct vm_msg msg = { 0 };
	int ret = E_HM_OK;

	vsock_dev = vsock_get_netdev_by_cid(cid);
	if (vsock_dev == NULL) {
		net_warn(HMNET_VSOCK, "can not find netdev with cid = %u\n", cid);
		ret = E_HM_INVAL;
	} else {
		actv_netdev = ux_netdev_priv(vsock_dev);
		/* to avoid fake uvmm cnode indexs */
		if (actv_netdev->uvmm_cnode_idx != uvmm_cnode_idx) {
			net_warn(HMNET_VSOCK, "invalid uvmm cnode idx\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = __convert_iov_to_vm_msg(caller_iov, nr_iov, &msg);
	}

	if (ret == E_HM_OK) {
		if (msg.hdr.src_cid != actv_netdev->cid) {
			net_warn(HMNET_VSOCK, "invalid src_cid = %u\n", (unsigned int)(msg.hdr.src_cid));
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_netdev_receive_zero_copy(vsock_dev, &msg,
						  (int)(sizeof(msg.hdr) + msg.len), 0, UXDEV_PKT_STRUCT);
	}

	/**
	 * if msg.data is not null, it means processing error, so we must free the data here.
	 */
	if (msg.data != NULL) {
		free(msg.data);
	}

	if (vsock_dev != NULL) {
		ux_netdev_put(vsock_dev);
	}

	return ret == E_HM_OK ? (int)(sizeof(msg.hdr) + msg.hdr.len): ret;
}

int vsock_actv_netdev_vq_ready(uint32_t uvmm_cnode_idx, unsigned int cid)
{
	ux_netdev_t *vsock_netdev = NULL;
	int ret = E_HM_OK;

	vsock_netdev = vsock_get_netdev_by_cid(cid);
	if (vsock_netdev == NULL) {
		net_warn(HMNET_VSOCK, "can not find netdev with cid = %u\n", cid);
		ret = E_HM_INVAL;
	} else {
		struct vsock_netdev *actv_netdev = ux_netdev_priv(vsock_netdev);

		/* to avoid fake uvmm cnode indexs  */
		if (actv_netdev->uvmm_cnode_idx != uvmm_cnode_idx) {
			net_warn(HMNET_VSOCK, "invalid uvmm cnode idx\n");
			ret = E_HM_INVAL;
		} else {
			vsock_netdev_wakeup(actv_netdev);
		}
	}

	if (vsock_netdev != NULL) {
		ux_netdev_put(vsock_netdev);
	}

	return ret;
}

void vsock_actv_netdev_revoke(uint32_t cnode_idx)
{
	ux_netdev_t *vsock_dev = vsock_get_netdev_by_cnode_idx(cnode_idx);

	if (vsock_dev != NULL && vsock_dev->type == ARPHRD_VSOCKMON) {
		__unregister_actv_netdev(vsock_dev, cnode_idx);
	}

	if (vsock_dev != NULL) {
		ux_netdev_put(vsock_dev);
	}
}
