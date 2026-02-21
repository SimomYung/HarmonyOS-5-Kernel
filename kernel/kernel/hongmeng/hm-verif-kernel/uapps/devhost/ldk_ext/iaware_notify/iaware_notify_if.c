/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Handler for iaware notify operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 28 12:15:54 2023
 */

#include <libsysif/devhost/server.h>

#include <libcrt/hmcrt.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libdevhost/devhost.h>
#include <libdevhost/procfs.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include "iaware_notify.h"
#include "iaware_notify_api.h"
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <devhost/log.h>
#include "iaware_notify_policy.h"

static uref_t dh_iaware_notify_uref;

uref_t dh_iaware_notify_uref_of(void)
{
	return dh_iaware_notify_uref;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_iaware_notify_thread_msg, dh_iaware_notify_thread_msg_ext,
		 unsigned long long, sender, unsigned long, credential,
		 struct evtcb_thread_msg_s, msg)
{
	UNUSED(sender, credential);
	int ret;

	ret = thread_event_handler(&msg);
	if (ret != E_HM_OK) {
		hm_error("dh_iaware_notify: handle thread event failed,%s\n", hmstrerror(ret));
	}

	return ret;
}

int devhost_regist_handler_to_iaware(void)
{
	int ret;

	dh_iaware_notify_uref = libdh_ap_export(hmsrv_sysmgr_cnode_idx());
	ret = hm_event_register_event(dh_iaware_notify_uref, EVTMGR_ETYPE_THREAD_CLONE,
				      __devhost_method_iaware_notify_thread_msg);
	if (ret != E_HM_OK) {
		hm_error("iaware_notify regist_clone_handler_to_eventmgr failed,%s\n", hmstrerror(ret));
		return ret;
	}
	ret = hm_event_register_event(dh_iaware_notify_uref, EVTMGR_ETYPE_THREAD_SET_NAME,
				      __devhost_method_iaware_notify_thread_msg);
	if (ret != E_HM_OK) {
		hm_error("iaware_notify regist_set_name_handler_to_eventmgr failed,%s\n", hmstrerror(ret));
	}

	return ret;
}

/* load iaware_notify module */
static int iaware_notify_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("iaware_notify.ko");
	if (ret < 0) {
		hm_error("lnxbase: load iaware_notify.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, iaware_notify_load_module, NULL);

/*
 * net sysif module, support network devices operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_iaware_notify_thread_msg`
 *
 * dependence: NULL
 */
DEFINE_DSO_PLUGIN(iaware_notify, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_iaware_notify_thread_msg, dh_iaware_notify_thread_msg_ext),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
