/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of kstate_if
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 21:53:50 CST 2023
 */

#include <libsysif/devhost/server.h>

#include <hongmeng/errno.h>
#include <libmem/umem.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include "kstate.h"
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <devhost/log.h>

#define MAX_ANCO_THAW_REASON_LENGTH 32

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_kstate_binderinfo_notify,
			 dh_kstate_binderinfo_notify_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, calling_pid, int, called_pid)
{
	UNUSED(sender, credential);
	int ret;

	ret = devhost_kstate_binderinfo_notify(calling_pid, called_pid);
	if (ret != E_HM_OK) {
		hm_error("devhost kstate binderinfo notify failed\n");
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_kstate_killinfo_notify,
			 dh_kstate_killinfo_notify_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, pid, int, seg)
{
	UNUSED(sender, credential);
	int ret;

	ret = devhost_kstate_killinfo_notify(pid, seg);
	/* this notify is called twice when a process exits, hwkillinfo would
	 * filter the second notify and return -1 as posix error code */
	if (ret != E_HM_OK && ret != posix2hmerrno(1)) {
		hm_error("devhost kstate killinfo notify failed\n");
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_kstate_freeze_binder_info_notify,
			 dh_kstate_freeze_binder_info_notify_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct kstate_binder_info *, info)
{
	UNUSED(sender, credential);
	int ret = E_HM_OK;
	struct kstate_binder_info kstate_info;

	if (info == NULL) {
		hm_error("invalid info\n");
		return E_HM_INVAL;
	}
	mem_zero_s(kstate_info);
	ret = hm_copy_from_user(&kstate_info, info, sizeof(struct kstate_binder_info));
	if (ret != E_HM_OK) {
		hm_error("copy from user failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = devhost_kstate_freeze_binder_info_notify(&kstate_info);
	if (ret != E_HM_OK) {
		hm_error("devhost kstate freeze_binder_info notify failed\n");
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_kstate_request_anco_thaw,
			 dh_kstate_request_anco_thaw_ext,
			 unsigned long long, sender, unsigned long, credential,
			 char *, reason, unsigned int, size)
{
	UNUSED(sender, credential);
	if (unlikely(size == 0 || size > MAX_ANCO_THAW_REASON_LENGTH)) {
		hm_error("invaild size\n");
		return E_HM_INVAL;
	}

	int ret = E_HM_OK;
	char thaw_rason[MAX_ANCO_THAW_REASON_LENGTH];
	mem_zero_a(thaw_rason);
	ret = hm_copy_from_user(thaw_rason, reason, size);
	if (ret != E_HM_OK) {
		hm_error("copy from user failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = devhost_kstate_request_anco_thaw(thaw_rason, size);
	if (ret != E_HM_OK) {
		hm_error("devhost kstate request anco thaw failed\n");
	}
	return ret;
}

/* load kstate module */
static int kstate_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("kstate.ko");
	if (ret < 0) {
		hm_error("lnxbase: load kstate.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, kstate_load_module, NULL);

/*
 * kstate sysif module, support huawei kstate operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_iaware_notify_thread_msg
 *
 * dependence: NULL
 */
DEFINE_DSO_PLUGIN(kstate, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_kstate_binderinfo_notify,
			       dh_kstate_binderinfo_notify_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_kstate_killinfo_notify,
			       dh_kstate_killinfo_notify_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_kstate_freeze_binder_info_notify,
			       dh_kstate_freeze_binder_info_notify_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_kstate_request_anco_thaw,
			       dh_kstate_request_anco_thaw_ext),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
