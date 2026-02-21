/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exported net APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 03 16:59:44 2021
 */
#include <devhost_api.h>
#include <hmnet/hmnet_devices.h>
#include <libalgo/idr.h>
#include <libsysif/devmgr/api.h>

#include "types.h"
#include "devhost.h"
#include <devhost/log.h>

PUBLIC_SYMBOL
int devhost_netcall_nic_rx(unsigned int pool_offset, int shm_len,
			   int rx_len, int offset, int ifindex)
{
	int ret = E_HM_OK;

	ret = hm_netdev_rx(pool_offset, shm_len, rx_len, offset, ifindex);
	if (ret < 0) {
		dh_error("hm_netdev_rx failed, ifindex=%d, err=%s\n",
			 ifindex, hmstrerror(ret));
		return ret;
	}

	return 0;
}

PUBLIC_SYMBOL
int devhost_netcall_nic_rx_ex(int num, void *data, int data_size)
{
	int ret = E_HM_OK;

	ret = hm_netdev_rx_ex(num, data, data_size);
	if (ret < 0) {
		dh_error("hm_netdev_rx failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return 0;
}

PUBLIC_SYMBOL
void devhost_netdev_update_features(int ifindex, uint64_t features)
{
	int err = E_HM_OK;

	err = hm_netdev_update_features(ifindex, features);
	if (err < 0) {
		dh_warn("hm_netdev_update_features failed, err=%s\n",
			hmstrerror(err));
	}
}

static DEFINE_IDR(g_net_devid_idr);
static struct raw_mutex __netdev_mutex = RAW_MUTEX_INITIALIZER;

PUBLIC_SYMBOL
int devhost_acquire_netdev(int ifindex, struct device_info **pdev)
{
	void *ptr = NULL;

	if (pdev == NULL) {
		return E_HM_INVAL;
	}
	RAW_MUTEX_GUARD(_, &__netdev_mutex);
	ptr = idr_find(&g_net_devid_idr, (unsigned int)ifindex);
	if (ptr == NULL) {
		return E_HM_NOOBJ;
	}
	/* transfer refcnt to caller */
	*pdev =  devinfo_get((struct device_info *)ptr);

	return 0;
}

PUBLIC_SYMBOL
int devhost_netdev_alloc_ifindex(int ifindex)
{
	int ret = E_HM_OK;
	struct __actvret_drvcall_devmgr_net_alloc_ifindex drvcall_ret;

	mem_zero_s(drvcall_ret);
	ret = actvcapcall_drvcall_devmgr_net_alloc_ifindex(
			libdh_get_devmgr_ap(),
			ifindex,
			&drvcall_ret);
	if (ret < 0) {
		return ret;
	}
	return drvcall_ret.ifindex;
}

PUBLIC_SYMBOL
int devhost_register_netdev(struct device_info *net_dev,
			    const char *name,
			    uint64_t features,
			    int *ifindex_in_out)
{
	int ret = E_HM_OK;
	struct hm_actv_buf devname;
	struct __actvret_drvcall_devmgr_net_register drvcall_ret;

	if ((net_dev == NULL) || (name == NULL) || (ifindex_in_out == NULL)) {
		return -EINVAL;
	}

	hm_actv_buf_init(&devname, name, strlen(name) + 1);
	mem_zero_s(drvcall_ret);
	raw_mutex_lock(&__netdev_mutex);
	ret = actvcapcall_drvcall_devmgr_net_register(
			libdh_get_devmgr_ap(),
			devname, *ifindex_in_out,
			features,
			&drvcall_ret);
	if (ret != E_HM_OK) {
		ret = -hmerrno2posix(ret);
		goto err_out;
	}

	ret = idr_set(&g_net_devid_idr, net_dev,
		      (unsigned int)drvcall_ret.ifindex);
	if (ret < 0) {
		(void)actvcapcall_drvcall_devmgr_net_unregister(
				libdh_get_devmgr_ap(), drvcall_ret.ifindex);
		if (ret == E_HM_OBJEXIST) {
			ret = E_HM_POSIX_EXIST;
		}
		ret = -hmerrno2posix(ret);
		goto err_out;
	}

	*ifindex_in_out = drvcall_ret.ifindex;

err_out:
	raw_mutex_unlock(&__netdev_mutex);
	return ret;
}

PUBLIC_SYMBOL
void devhost_unregister_netdev(int ifindex)
{
	int ret = E_HM_OK;

	ret = actvcapcall_drvcall_devmgr_net_unregister(libdh_get_devmgr_ap(), ifindex);
	if (ret < 0) {
		dh_debug("devmgr unregister net device failed, index=%d\n", ifindex);
		return;
	}

	RAW_MUTEX_GUARD(_, &__netdev_mutex);
	(void)idr_remove(&g_net_devid_idr, (unsigned)ifindex);
}
