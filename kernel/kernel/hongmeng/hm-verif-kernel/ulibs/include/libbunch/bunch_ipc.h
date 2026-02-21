/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for bunch ipc api
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 15 21:23:14 2019
 */
#ifndef ULIBS_LIBBUNCH_BUNCH_IPC_H
#define ULIBS_LIBBUNCH_BUNCH_IPC_H

#include <hmkernel/capability.h>
#include <libbunch/bunch.h>

#define BUNCH_IPC_DEFAULT_SIZE 4096U

/* attr as bunch private data */
struct bunch_ipc_attr {
	/* key for ashm, client transfer it to server */
	uint64_t key;
	/* shm_id of ashm, have different local value between client and server */
	int32_t shm_id;
	/* shm size, should be same between client and server,
	 * compatible with lib32 use fixed size uint32_t not size_t */
	uint32_t size;
	/* cnode_idx of key owner, used by writer to check */
	uint32_t owner_idx;
};

typedef int (*ipc_call)(struct bunch_ipc_attr *ipc_attr, void *priv_data);
struct bunch_ipc_reader_ctx {
	void *private_data;
	ipc_call ipc;
	uint64_t target_info;
};

/* low level interfaces */

int bunch_ipc_inner_alloc(struct bunch_ipc_attr *attr, size_t data_size);
int bunch_ipc_inner_free(struct bunch_ipc_attr *attr);
int bunch_ipc_inner_fetch(struct bunch_ipc_attr *attr, struct bunch_ipc_reader_ctx *ctx);
int bunch_ipc_inner_tryfetch(struct bunch_ipc_attr *attr, struct bunch_ipc_reader_ctx *ctx);
int bunch_ipc_inner_fill(struct bunch_ipc_attr *attr, struct bunch *bunch);
int bunch_ipc_inner_drain(struct bunch *bunch);

/* high level interfaces */

/* client create and destroy ipc,
* double destroy is unexpected, after destroy set bunch NULL is perfered */
struct bunch *bunch_ipc_create(size_t size);
static inline void bunch_ipc_ctx_init(struct bunch_ipc_reader_ctx *ctx,
				      uint64_t target_info, ipc_call ipc, void *data)
{
	if (ctx != NULL) {
		/* ensure initialize all members */
		ctx->target_info = target_info;
		ctx->ipc = ipc;
		ctx->private_data = data;
	}
}
/* client try request memory data, can be called many times between create and destroy */
int bunch_ipc_request(struct bunch *bunch, struct bunch_ipc_reader_ctx *ctx);
/* bunch is NULL is also safe, but not double destroy */
void bunch_ipc_destroy(struct bunch *bunch);
/* server get and put ipc,
 * double close is unexpected, after close set bunch NULL is perfered */
struct bunch *bunch_ipc_open(struct bunch_ipc_attr *attr);
/* bunch is NULL is also safe, but not double close */
void bunch_ipc_close(struct bunch *bunch);

#endif
