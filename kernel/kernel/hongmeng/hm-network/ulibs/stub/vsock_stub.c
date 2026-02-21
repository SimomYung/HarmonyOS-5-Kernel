/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Stub interface for network vsock
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 11 10:15:31 2022
 */

#include "hmnet/hm_vsock.h"

#include <hongmeng/errno.h>
#include <libstrict/strict.h>

int hm_vsock_register_vm(unsigned int cid, unsigned int dev_group_id)
{
	UNUSED(cid, dev_group_id);
	return E_HM_NOSYS;
}

int hm_vsock_vm_tx(unsigned int cid, void *iov, size_t nr_iov)
{
	UNUSED(cid, iov, nr_iov);
	return E_HM_NOSYS;
}

int hm_vsock_unregister_vm(unsigned int cid)
{
	UNUSED(cid);
	return E_HM_NOSYS;
}
