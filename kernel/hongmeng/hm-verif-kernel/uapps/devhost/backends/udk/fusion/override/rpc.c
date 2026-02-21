/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Override rpc interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thur Jul 6 15:50:54 2023
 */

#include <libsysif/devhost/api.h>
#include <libhmactv/actv.h>
#include <devhost/rpc.h>

int devhost_rpc_invoke(uint64_t rref, int devid, int cmd,
		       const struct rpc_ctx *req,
		       const struct rpc_ctx *resp,
		       unsigned long *resp_size_out,
		       bool is_xact)
{
	struct __actvret_drvcall_devhost_rpc_invoke pret;
	int ret = E_HM_OK;

	mem_zero_s(pret);
	if (resp->buf != NULL) {
		hm_actv_set_accessible_mem_range(ptr_to_ulong(resp->buf), (unsigned long)resp->size);
	}
	if (is_xact) {
		ret = actvxactcapcall_drvcall_devhost_rpc_invoke(true, true,
						 rref, devid, cmd,
						 ptr_to_ulong(req->buf), req->size,
						 ptr_to_ulong(resp->buf), resp->size,
						 &pret);
	} else {
		ret = actvcapcall_drvcall_devhost_rpc_invoke(rref, devid, cmd,
						 ptr_to_ulong(req->buf), req->size,
						 ptr_to_ulong(resp->buf), resp->size,
						 &pret);
	}
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	if (resp_size_out != NULL) {
		*resp_size_out = (unsigned long)pret.resp_size;
	}
	return ret;
}
