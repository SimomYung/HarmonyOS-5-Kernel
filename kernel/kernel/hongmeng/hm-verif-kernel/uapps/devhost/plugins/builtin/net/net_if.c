/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for net device operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#include <libsysif/devhost/server.h>

#include <libhmsrv_io/io_net_sring.h>
#include <libcrt/hmcrt.h>
#include <devhost/plugin.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_ashm.h>

#include "types.h"
#include "devhost.h"
#include "devhost_api.h"
#include <devhost/log.h>
#include "devhost_netdev.h"

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_prepare, dh_net_prepare_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	uint64_t key;
	int ret = E_HM_OK;
	int pool_shm_id;
#ifdef CONFIG_NET_IO_URING
	struct io_uring *uring = NULL;
#endif
	uint64_t rx_ring_key = 0ULL;
	struct __actvret_drvcall_devhost_net_prepare *pret =
			actvhdlr_drvhandler_devhost_net_prepare_prepare_ret(sender, credential);

	dh_debug("pool prepare start\n");

#ifdef CONFIG_NET_IO_URING
	uring = prepare_rx_uring_once();
	if (uring == NULL) {
		dh_error("prepare_rx_uring_once failed\n");
		return E_HM_NOMEM;
	}
	ret = io_uring_grant_to_caller(uring, &rx_ring_key);
	if (ret < 0) {
		prepare_rx_uring_cleanup();
		return ret;
	}
#endif

	ret = prepare_cell_pool_once(&pool_shm_id);
	if (ret < 0) {
		dh_error("prepare_cell_pool_once failed\n");
		prepare_rx_uring_cleanup();
		return ret;
	}

	if (sysif_actv_src_cnode_idx() != hm_ucap_self_cnode_idx()) {
		ret = hm_ashm_grant_to_caller(pool_shm_id, SHM_GRANT_RDWR, &key);
		if (ret < 0) {
			dh_error("hm_ashm_grant_to_caller failed, err=%s\n",
				hmstrerror(ret));
			prepare_rx_uring_cleanup();
			prepare_cell_pool_cleanup();
			return ret;
		}
	} else {
		/* libnet mode
		 * hmnetd and devhost belongs to the same process.
		 */
		key = (uint64_t)pool_offset2va(0);
	}

	pret->shm_id = key;
	pret->rx_uring_shm_id = rx_ring_key;
	pret->pool_size = NET_CELL_POOL_LEN;
	pret->unit_size = NET_CELL_POOL_UNIT_SIZE;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_init, dh_net_init_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, ifindex, rref_t, tgt_rref)
{
	uref_t uref_ap;

	UNUSED(sender, credential);
	UNUSED(ifindex);

	uref_ap = libdh_ap_grant(tgt_rref);
	if (IS_REF_ERR(uref_ap)) {
		return REF_TO_ERR(uref_ap);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_send, dh_net_send_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, ifindex,
			 unsigned int, offset,
			 unsigned int, size,
			 unsigned int, payload_len,
			 unsigned long, flags)
{
	int ret = E_HM_OK;
	struct device_info *info = NULL;

	UNUSED(sender, credential);

	devhost_netstat_inc_tx();

	if (offset > (NET_CELL_POOL_LEN - NET_CELL_POOL_UNIT_SIZE) ||
	    payload_len > NET_CELL_POOL_UNIT_SIZE) {
		return E_HM_INVAL;
	}
	ret = devhost_acquire_netdev(ifindex, &info);
	if (ret != 0) {
		return ret;
	}
	if ((info->p->netops == NULL) || (info->p->netops->net_tx == NULL)) {
		devinfo_put(info);
		return E_HM_NOSYS;
	}

	ret = info->p->netops->net_tx(flags, payload_len, size, offset, info);
	devinfo_put(info);

	return posix2hmerrno(-ret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_ioctl, dh_net_ioctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, ifindex,
			 int, request, unsigned long, args)
{
	struct device_info *info = NULL;
	int ret = E_HM_OK;

	UNUSED(sender, credential);

	dh_debug("net_ioctl ifindex=%d\n", ifindex);

	ret = devhost_acquire_netdev(ifindex, &info);
	if (ret != 0) {
		return ret;
	}
	if (info->p->netops == NULL || info->p->netops->driver_ioctl == NULL) {
		return E_HM_NOSYS;
	}

	ret = info->p->netops->driver_ioctl(info, request, args);
	if (ret != 0) {
		return posix2hmerrno(-ret);
	}
	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_status, dh_net_status_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, ifindex)
{
	struct __actvret_drvcall_devhost_net_status *pret =
		actvhdlr_drvhandler_devhost_net_status_prepare_ret(sender, credential);
	int ret = E_HM_OK;
	struct device_info *info = NULL;
	struct netdev_status *pnet_status = &pret->status;

	ret = devhost_acquire_netdev(ifindex, &info);
	if (ret != 0) {
		return ret;
	}

	if ((info->p->netops == NULL) || (info->p->netops->show_status == NULL)) {
		devinfo_put(info);
		return E_HM_NOSYS;
	}

	devhost_netstat_get(pnet_status);

	ret = info->p->netops->show_status(info, pnet_status);
	devinfo_put(info);

	return posix2hmerrno(-ret);
}

/*
 * net sysif module, support network devices operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_net_prepare`
 *  - target sysif-ep: `drvhandler_devhost_net_init`
 *  - target sysif-ep: `drvhandler_devhost_net_send`
 *  - target sysif-ep: `drvhandler_devhost_net_ioctl`
 *  - target sysif-ep: `drvhandler_devhost_net_status`
 *
 * export symbol:
 *  - func: `devhost_native_net_rx`
 *  - func: `devhost_native_net_rx_ex`
 *  - func: `pool_va2offset`
 *  - func: `pool_offset2va`
 *  - func: `pool_offset2pa`
 *  - func: `devhost_dma_pool_alloc`
 *  - func: `devhost_dma_pool_alloc_ex`
 *  - func: `devhost_dma_pool_free`
 *  - func: `devhost_netcall_nic_rx`
 *  - func: `devhost_netcall_nic_rx_ex`
 *  - func: `devhost_netdev_update_features`
 *  - func: `devhost_acquire_netdev`
 *  - func: `devhost_netdev_alloc_ifindex`
 *  - func: `devhost_register_netdev`
 *  - func: `devhost_unregister_netdev`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(net, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_prepare, dh_net_prepare_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_init, dh_net_init_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_send, dh_net_send_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_ioctl, dh_net_ioctl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_status, dh_net_status_ext)
)
