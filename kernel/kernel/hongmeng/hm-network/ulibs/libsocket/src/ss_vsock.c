/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of vsock interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 2 14:21:46 2021
 */

#include <libhmlog/hmlog.h>
#include <hongmeng/types.h>
#include <libhmsrv_sys/hm_mem.h>

#include "hm_vsock.h"
#include "ss_channel.h"

int hm_vsock_register_vm(unsigned int cid, unsigned int dev_group_id)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_vsock_register_vm(rref,
						    cid, dev_group_id);
	if (ret < 0) {
		hm_error("vsock register vm failed ret = %s\n", hmstrerror(ret));
	}

	hm_net_put_rref(&rref);
	return ret;
}

int hm_vsock_vm_tx(unsigned int cid, void *iov, size_t nr_iov)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_vsock_vm_notify(rref, cid,
						  VM_NOTIFY_VM_TX, iov, nr_iov);
	if (ret < 0) {
		hm_error("vsock vm tx failed ret = %s\n", hmstrerror(ret));
	}

	return ret;
}

int hm_vsock_vq_ready(unsigned int cid)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_vsock_vm_notify(rref, cid,
						  VM_NOTIFY_VQ_READY, NULL, 0);
	if (ret < 0) {
		hm_error("vsock vm tx failed ret = %s\n", hmstrerror(ret));
	}

	hm_net_put_rref(&rref);
	return ret;
}

int hm_vsock_unregister_vm(unsigned int cid)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_vsock_unregister_vm(rref, cid);

	hm_net_put_rref(&rref);
	return ret;
}
