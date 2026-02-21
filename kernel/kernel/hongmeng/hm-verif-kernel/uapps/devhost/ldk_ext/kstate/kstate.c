/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of kstate
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 21:16:09 CST 2023
 */
#include "kstate.h"

#include <devhost/plugin.h>
#include <hongmeng/errno.h>
#include <libdevhost/devhost.h>
#include <libhmlog/hmlog.h>
#include <lnxbase/export.h>

static struct devhost_kstate_ops g_devhost_kstate_ops __read_mostly;

PUBLIC_SYMBOL
void devhost_kstate_ops_register(kstate_info_func binderinfo, kstate_info_func killinfo)
{
	if (g_devhost_kstate_ops.binderinfo == NULL) {
		g_devhost_kstate_ops.binderinfo = binderinfo;
	}
	if (g_devhost_kstate_ops.killinfo == NULL) {
		g_devhost_kstate_ops.killinfo = killinfo;
	}
}
EXPORT_SYMBOL(devhost_kstate_ops_register);

PUBLIC_SYMBOL
void devhost_freeze_binder_info_ops_register(freeze_binder_info_func freeze_binder_info)
{
	if (g_devhost_kstate_ops.freeze_binder_info == NULL) {
		g_devhost_kstate_ops.freeze_binder_info = freeze_binder_info;
	}
}
EXPORT_SYMBOL(devhost_freeze_binder_info_ops_register);

PUBLIC_SYMBOL
void devhost_request_anco_thaw_ops_register(request_anco_thaw_func request_anco_thaw)
{
	if (g_devhost_kstate_ops.request_anco_thaw == NULL) {
		g_devhost_kstate_ops.request_anco_thaw = request_anco_thaw;
	}
}
EXPORT_SYMBOL(devhost_request_anco_thaw_ops_register);

int devhost_kstate_binderinfo_notify(int calling_pid, int called_pid)
{
	int ret = E_HM_OK;

	if ((g_devhost_kstate_ops.binderinfo != NULL)) {
		ret = g_devhost_kstate_ops.binderinfo(calling_pid, called_pid);
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

int devhost_kstate_killinfo_notify(int pid, int seg)
{
	int ret = E_HM_OK;

	if ((g_devhost_kstate_ops.killinfo != NULL)) {
		ret = g_devhost_kstate_ops.killinfo(pid, seg);
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

int devhost_kstate_freeze_binder_info_notify(struct kstate_binder_info *info)
{
	int ret = E_HM_OK;

	if ((g_devhost_kstate_ops.freeze_binder_info != NULL)) {
		ret = g_devhost_kstate_ops.freeze_binder_info(info);
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

int devhost_kstate_request_anco_thaw(char *reason, unsigned int size)
{
	int ret = E_HM_OK;

	if ((g_devhost_kstate_ops.request_anco_thaw != NULL)) {
		ret = g_devhost_kstate_ops.request_anco_thaw(reason, size);
		ret = posix2hmerrno(-ret);
	}

	return ret;
}
