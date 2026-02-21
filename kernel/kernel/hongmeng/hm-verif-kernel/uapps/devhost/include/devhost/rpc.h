/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for cross devhost RPC APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_RPC_H__
#define __DEVHOST_API_RPC_H__

#if defined(MODULE)
#include <linux/types.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <udk/stdint.h>
#include <stddef.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

/*
 * The interface for devhost rpc response
 */
struct devhost_rpc_resp {
	/*
	 * Write to response
	 *
	 * @param   resp    [I,O] The response object to write
	 * @param   data    [I] The data to write
	 * @param   size    [I] The size of the data to write
	 *
	 * @return  E_HM_OK if sucess, error code if fail
	 */
	int (*write)(struct devhost_rpc_resp *resp, const char *data, size_t size);
	void (*acquire_buf)(struct devhost_rpc_resp *resp, uint64_t *buf, uint64_t *size);
	unsigned long resp_size; /* The response size */
};

struct devhost_rpc_req {
	int (*read)(struct devhost_rpc_req *req, const char *data, size_t size);
	void (*acquire_buf)(struct devhost_rpc_req *req, uint64_t *buf, uint64_t *size);
	unsigned long req_size; /* The request size */
};

struct devhost_rpc_ops {
	int (*invoke)(int devid, int cmd, struct devhost_rpc_req *req,
		      struct devhost_rpc_resp *resp);
};
int devhost_register_rpc(int devid, struct devhost_rpc_ops *ops);

struct rpc_ctx {
	void *buf;
	unsigned long size;
};

int devhost_rpc_invoke(uint64_t rref, int devid, int cmd,
		       const struct rpc_ctx *req,
		       const struct rpc_ctx *resp,
		       unsigned long *resp_size_out,
		       bool is_xact);
void devhost_svrp_release(uint64_t rref);

/* for timer service nanosleep only */
int devhost_timer_accept_grant(uint64_t target_cref, uint64_t uref,
			       unsigned long flags, uint64_t *result);

#endif /* __DEVHOST_API_RPC_H__ */
