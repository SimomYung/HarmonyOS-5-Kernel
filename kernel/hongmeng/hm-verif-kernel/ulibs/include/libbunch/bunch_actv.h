/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for bunch actvcall api
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 15 21:23:14 2019
 */
#ifndef ULIBS_LIBBUNCH_BUNCH_ACTV_H
#define ULIBS_LIBBUNCH_BUNCH_ACTV_H

#include <hmkernel/capability.h>
#include <libbunch/bunch.h>

/* Bunch actv is used in actvcall scenario, whose caller is base actv
 * in actvcall chain. Sever will directly write into caller buffers
 */
#define BUNCH_ACTV_DEFAULT_SIZE 4096U
/* max buffer is 4M, default 4K * 2^12 */
#define BUNCH_ACTV_FETCH_RETRY_MAX 12U
#define BUNCH_ACTV_MAX_SIZE (BUNCH_ACTV_DEFAULT_SIZE << BUNCH_ACTV_FETCH_RETRY_MAX)
struct bunch_actv_attr {
	void *buf;
	uint32_t size;
};

typedef int (*actv_call)(struct bunch_actv_attr *actv_attr, void *priv_data);
struct bunch_actv_reader_ctx {
	void *private_data;
	actv_call actvcall;
};

static inline void bunch_actv_ctx_init(struct bunch_actv_reader_ctx *ctx,
				       actv_call call, void *data)
{
	if (ctx != NULL) {
		ctx->actvcall = call;
		ctx->private_data = data;
	}
}

/* bunch_actv_create/destroy: Allocate/free memory for attr and bunch
 * bunch NULL is also safe for bunch destroy, will not double destroy
 */
struct bunch *bunch_actv_create(size_t size);
void bunch_actv_destroy(struct bunch *bunch);

/* bunch_actv_open/close: Create bunch and fill it using exist attr.
 * bunch_actv_close will return saved attr, NULL is safe and will return NULL
 */
struct bunch *bunch_actv_open(struct bunch_actv_attr *attr);
void *bunch_actv_close(struct bunch *bunch);

/* client try request data, can be called many times between create/destroy */
int bunch_actv_request(struct bunch *bunch, struct bunch_actv_reader_ctx *ctx);
/* server write to caller vm directly */
int bunch_actv_write_caller(struct bunch *bunch,
			    const struct bunch_actv_attr *caller_attr);

#endif
