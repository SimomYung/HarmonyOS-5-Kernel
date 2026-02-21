/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Handler for clock operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 29 16:11:40 2020
 */
#include <libsysif/devhost/server.h>

#include <devhost/plugin.h>
#include <libstrict/strict.h>

#include "types.h"
#include <vfs.h>
#include <devhost/log.h>
#include <devhost/file.h>
#include <devhost/device.h>
#include <libdevhost/devhost.h>
#include <libsysif/fs/api.h>
#include "devhost.h"

static int (*g_set_poweroff_alarm)(long, bool) = NULL;

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_clock_settime, dh_clock_settime_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long, tp, unsigned long long, file_id)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  0, (unsigned int)file_id);
	if (node == NULL) {
		dh_error("can't find file_node by file_id to settime\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid");
		return E_HM_INVAL;
	}

	if ((devinfo->p->clkops == NULL) || (devinfo->p->clkops->clock_settime == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devinfo->p->clkops->clock_settime(file_id,
						ulong_to_ptr(tp, const struct timespec));

	return posix2hmerrno(-ret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_clock_gettime, dh_clock_gettime_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long, tp, unsigned long long, file_id)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  0, (unsigned int)file_id);
	if (node == NULL) {
		dh_error("can't find file node by fileid to gettime\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid");
		return E_HM_INVAL;
	}

	if ((devinfo->p->clkops == NULL) || (devinfo->p->clkops->clock_gettime == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devinfo->p->clkops->clock_gettime(file_id,
						ulong_to_ptr(tp, struct timespec));

	return posix2hmerrno(-ret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_clock_getres, dh_clock_getres_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long, tp, unsigned long long, file_id)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  0, (unsigned int)file_id);
	if (node == NULL) {
		dh_error("can't find file_node by file_id to getres\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid");
		return E_HM_INVAL;
	}

	if ((devinfo->p->clkops == NULL) || (devinfo->p->clkops->clock_getres == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devinfo->p->clkops->clock_getres(file_id,
					       ulong_to_ptr(tp, struct timespec));

	return posix2hmerrno(-ret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_clock_adjtime, dh_clock_adjtime_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long, tx, unsigned long long, file_id)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  0, (unsigned int)file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid");
		return E_HM_INVAL;
	}

	if ((devinfo->p->clkops == NULL) || (devinfo->p->clkops->clock_adjtime == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devinfo->p->clkops->clock_adjtime(file_id,
						ulong_to_ptr(tx, struct timex));

	return posix2hmerrno(-ret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_clock_poweroff_alarm, dh_clock_poweroff_alarm_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, tm)
{
	int ret = E_HM_OK;

	UNUSED(credential);
	UNUSED(sender);

	if (g_set_poweroff_alarm == NULL) {
		dh_warn("timer: devhost does not support set_poweroff_alarm\n");
		return E_HM_NOSYS;
	}

	ret = g_set_poweroff_alarm(tm, true);
	if (ret != E_HM_OK) {
		dh_warn("timer: set poweroff alarm fail ret=%d\n", ret);
	}

	return ret;
}

PUBLIC_SYMBOL
int devhost_register_rtc_ops(set_power_on_alarm_t set_power_on_alarm)
{
	if (set_power_on_alarm == NULL) {
		dh_error("clock_if: set_power_on_alarm is NULL\n");
		return E_HM_INVAL;
	}

	g_set_poweroff_alarm = set_power_on_alarm;

	return E_HM_OK;
}

static int devhost_clock_register(void)
{
	int ret;
	uref_t dh_uref;
	rref_t fsmgr_rref = 0ULL;
	struct sysarg_cnode_statrref stat = { 0 };

	ret = hm_path_acquire(FSMGR_PATH_ACTV, &fsmgr_rref);
	if (ret != E_HM_OK) {
		dh_error("acquire fsmgr path failed\n");
		return ret;
	}

	ret = sysfast_cnode_stat_rref(&stat, fsmgr_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret != E_HM_OK) {
		(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
		dh_error("stat fsmgr rref failed: %s\n", hmstrerror(ret));
		return ret;
	}

	dh_uref = libdh_ap_export(stat.src_cnode_idx);
	if (IS_REF_ERR(dh_uref)) {
		ret = REF_TO_ERR(dh_uref);
		(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
		dh_error("export actv pool to fsmgr failed: %s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvxactcapcall_fscall_vfs_timerfd_register_devhost(
				true, true, fsmgr_rref, dh_uref);
	if (ret != E_HM_OK) {
		dh_error("export uref to fsmgr failed: %s\n", hmstrerror(ret));
		(void)hm_ucap_revoke(0, dh_uref);
		(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
		return ret;
	}

	return E_HM_OK;
}

static int dh_clock_postinit(void *arg)
{
	UNUSED(arg);
	int err = devhost_clock_register();

	return (err == E_HM_NOSYS) ? E_HM_OK : err;
}

static int dh_clock_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	int ret = E_HM_OK;

	UNUSED(plugin);

	ret = devhost_postinit(dh_clock_postinit, NULL);
	if (ret < 0) {
		dh_error("devhost clock postinit failed: %s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

/*
 * clock sysif module, supports clock related operations,
 * including getting and setting the clock time
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_clock_settime`
 *  - target sysif-ep: `drvhandler_devhost_clock_gettime`
 *  - target sysif-ep: `drvhandler_devhost_clock_getres`
 *  - target sysif-ep: `drvhandler_devhost_clock_adjtime`
 *  - target sysif-ep: `drvhandler_devhost_clock_poweroff_alarm`
 *
 * export symbol:
 *  - func: `devhost_register_rtc_ops`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(clock, dh_clock_plugin_init,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_clock_settime, dh_clock_settime_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_clock_gettime, dh_clock_gettime_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_clock_getres, dh_clock_getres_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_clock_adjtime, dh_clock_adjtime_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_clock_poweroff_alarm, dh_clock_poweroff_alarm_ext)
);
