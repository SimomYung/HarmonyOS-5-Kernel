/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define interface of fileguard_if
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */

#include <libsysif/devhost/server.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include <libmem/umem.h>

#include "fileguard_hdr.h"
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <devhost/log.h>

#define FG_CHR_WRITE_SIZE		(512 * 1024)

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fileguard_message_notify,
			 dh_fileguard_message_notify_ext,
			 unsigned long long, sender, unsigned long, credential,
			 void *, message, size_t, len)
{
	UNUSED(sender, credential);
	int ret;
	void *data;

	if (message == NULL || len == 0 || len > FG_CHR_WRITE_SIZE) {
		return E_HM_INVAL;
	}

	data = malloc(len);
	if (data == NULL) {
		hm_error("[fg] malloc fail\n");
		return E_HM_NOMEM;
	} else {
		ret = hm_copy_from_user(data, message, len);
	}

	ret = devhost_fileguard_msg_notify(data, len);
	if (ret != E_HM_OK) {
		hm_error("devhost fileguard binderinfo notify failed\n");
	}

	free(data);
	return ret;
}

/* load fileguard module */
static int fileguard_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("fileguard.ko");
	if (ret < 0) {
		hm_error("lnxbase: load fileguard.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, fileguard_load_module, NULL);

/*
 * fileguard sysif module, support huawei fileguard operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_iaware_notify_thread_msg
 *
 * dependence: NULL
 */
DEFINE_DSO_PLUGIN(fileguard, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fileguard_message_notify,
			       dh_fileguard_message_notify_ext),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
