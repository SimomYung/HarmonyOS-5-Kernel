/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for power management sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 21 20:41:33 2019
 */
#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/server.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libpreempt/preempt.h>

#include <devhost/plugin.h>
#include <libstrict/strict.h>

#include "devhost.h"
#include "devhost_pm.h"
#include "devhost_backend.h"
#include <devhost/log.h>

static cref_t g_devhost_pm_ap;

static union devhost_hdlrs_union g_devhost_pm_ap_table[__devhost_method_MAX] __ro_after_init;

#define DEVHOST_PM_ACTV_NAME		"pm-actv"
#define DEVHOST_PM_ACTV_STACK_ORDER	2UL /* 16K */
#define DEVHOST_PM_ACTV_STACK_SIZE	(1UL << (DEVHOST_PM_ACTV_STACK_ORDER + PAGE_SHIFT))
#define DEVHOST_PM_ACTV_MAX_NUM		1024
#define DEVHOST_PM_ACTV_TSD_SIZE	128U
#define DEVHOST_PM_ACTV_BUF_SIZE	2048UL

static struct actvpool_configs g_pm_ap_cfg = {
	.ap_name		= DEVHOST_PM_ACTV_NAME,
	.path_name		= "", /* set empty string, means do not add path */
	.actv_init_num		= 0,
	.actv_max_num		= DEVHOST_PM_ACTV_MAX_NUM,
	.recvbuf_size		= DEVHOST_PM_ACTV_BUF_SIZE,
	.tsd_size		= DEVHOST_PM_ACTV_TSD_SIZE,
	.stack_size		= DEVHOST_PM_ACTV_STACK_SIZE,
	.actvret_size		= sizeof(union __actvret_drvcall_devhost),
	.action_table		= g_devhost_pm_ap_table,
	.action_table_size	= sizeof(void *) * (size_t)__devhost_method_MAX,
	.p			= NULL
};

cref_t dh_pm_ap_cref_of(void)
{
	return g_devhost_pm_ap;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_action, dh_pm_action_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, action)
{
	UNUSED(sender, credential);
	return devhost_pm_action(action);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_dubai_hwlog, dh_pm_dubai_hwlog_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct pm_dubai_ctrl_log *, log_info)
{
	int ret;
	struct pm_dubai_ctrl_log __log_info;

	UNUSED(sender, credential);
	if (log_info == NULL) {
		dh_error("invalid log info\n");
		return E_HM_INVAL;
	}
	ret = hm_actv_read_caller_vm(&__log_info, log_info, sizeof(struct pm_dubai_ctrl_log));
	if (ret != E_HM_OK) {
		dh_error("failed to get log info, err=%s\n", hmstrerror(ret));
		return E_HM_INVAL;
	}
	ret = devhost_pm_dubai_hwlog(&__log_info);
	if (ret != E_HM_OK) {
		dh_error("dubai hwlog failed, err=%s\n", hmstrerror(ret));
	}

	return ret;
}

void devhost_pm_ap_table_init(void)
{
	union devhost_hdlrs_union *dh_table = &g_devhost_pm_ap_table[__devhost_method_pm_action];
	union devhost_hdlrs_union *dh_ori_table = &sysif_devhost_actvcapcallhdlr_table[__devhost_method_pm_action];
	NOFAIL(memcpy_s(dh_table, sizeof(*dh_table), dh_ori_table, sizeof(*dh_ori_table)));

	dh_table = &g_devhost_pm_ap_table[__devhost_method_pm_dubai_hwlog];
	dh_ori_table = &sysif_devhost_actvcapcallhdlr_table[__devhost_method_pm_dubai_hwlog];
	NOFAIL(memcpy_s(dh_table, sizeof(*dh_table), dh_ori_table, sizeof(*dh_ori_table)));
}

static int devhost_pm_actv_init(void)
{
	return backend_ctx_setup();
}

void devhost_prepare_pm_actvs(void)
{
	int err = E_HM_OK;
	int rc;
	unsigned long online_cpu_nr = 0UL;

	rc = hm_sysctrl_read_conf(__SYSCONF_ONLINE_CPU, &online_cpu_nr, sizeof(online_cpu_nr));
	if (rc < 0 || online_cpu_nr == 0UL) {
		dh_panic("get online cpu nr failed\n");
	}
	g_pm_ap_cfg.actv_init_num = online_cpu_nr;
	NOFAIL(hm_apconfig_private_init(&g_pm_ap_cfg));
	NOFAIL(hm_apconfig_private_set_actvinitfunc(&g_pm_ap_cfg, devhost_pm_actv_init));
	NOFAIL(hm_apconfig_private_enable_preinitentry(&g_pm_ap_cfg));
	if (preempt_process_feature_enabled()) {
		NOFAIL(hm_apconfig_private_enable_alak(&g_pm_ap_cfg));
	}

	err = hm_setup_actvpool(&g_pm_ap_cfg, &g_devhost_pm_ap);
	if (err < 0) {
		dh_panic("devhost setup pm actvpool failed, err=%s\n", hmstrerror(err));
		g_devhost_pm_ap = ERR_TO_REF(err);
	}

	dh_info("devhost prepare %lu actvs for pm :%s\n", online_cpu_nr, hmstrerror(err));
	/* prepare resource failed should panic, does not do roll_back func */
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_prepare, dh_pm_prepare_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, type, struct reboot_cmd, cmd)
{
	size_t len;

	UNUSED(sender, credential);

	len = strnlen(cmd.string, REBOOT_CMD_LEN);
	if (len == REBOOT_CMD_LEN) {
		dh_error("reboot cmd string is invalid\n");
		return E_HM_INVAL;
	}

	return devhost_pm_prepare(type, cmd.string);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_power_off, dh_pm_power_off_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	UNUSED(sender, credential);
	return devhost_plat_power_off();
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_reboot, dh_pm_reboot_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct reboot_cmd, cmd)
{
	size_t len;

	UNUSED(sender, credential);

	len = strnlen(cmd.string, REBOOT_CMD_LEN);
	if (len == REBOOT_CMD_LEN) {
		dh_error("reboot cmd string is invalid\n");
		return E_HM_INVAL;
	}

	return devhost_plat_reboot(cmd.string);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_cpuhp_action, dh_pm_cpuhp_action_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, cpu, unsigned int, action)
{
	UNUSED(sender, credential);
	return devhost_pm_cpuhp_action(cpu, action);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_wl_stat, dh_pm_wl_stat_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct wl_stat_queue_info, queue_info, size_t, cnt)
{
	int ret;
	struct __actvret_drvcall_devhost_pm_wl_stat *pret =
		actvhdlr_drvhandler_devhost_pm_wl_stat_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL, "pret is null");

	ret = devhost_pm_wl_stat(&queue_info, cnt, &pret->enqueue_cnt);
	if (ret != E_HM_OK) {
		dh_error("devhost get pm wakelock stat failed\n");
		return ret;
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_pm_thermal_action, dh_pm_thermal_action_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, action, struct thermal_action_arg, arg)
{
	struct __actvret_drvcall_devhost_pm_thermal_action *pret =
		actvhdlr_drvhandler_devhost_pm_thermal_action_prepare_ret(sender, credential);

	BUG_ON_D(pret == NULL, "pret is null");
	return devhost_pm_thermal_action(action, &arg, &pret->action_ret);
}

static int __around_pm_add_device(struct advice_ctx *ctx, struct device_info *info, int devid)
{
	int ret = E_HM_OK;

	UNUSED(devid);

	ret = devhost_pm_add_device(info);
	if (ret < 0) {
		dh_error("pm add device failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = ctx->call_next(ctx);
	if (ret < 0) {
		(void)devhost_pm_remove_device(info);
	}

	return ret;
}
DEFINE_ADVICE_EXTENSION(dh_register_device_ext, NULL, __around_pm_add_device, NULL);

static int __around_pm_remove_device(struct advice_ctx *ctx, struct device_info *info)
{
	int ret = E_HM_OK;

	ret = ctx->call_next(ctx);
	if (ret < 0) {
		return ret;
	}

	(void)devhost_pm_remove_device(info);

	return ret;
}
DEFINE_ADVICE_EXTENSION(devhost_unregister_device_ext, NULL, __around_pm_remove_device, NULL);

static int dh_pm_barrier(void *arg)
{
	bool is_ready = false;

	UNUSED(arg);

	do {
		(void)hm_thread_yield();
		is_ready = is_pwrmgr_ready();
	} while (!is_ready);

	return E_HM_OK;
}

static int dh_pm_postinit(void *arg)
{
	UNUSED(arg);
	int err = devhost_pm_register();

	return (err == E_HM_NOSYS) ? E_HM_OK : err;
}

static int dh_pm_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	int ret = E_HM_OK;

	UNUSED(plugin);

	ret = devhost_register_barrier(dh_pm_barrier, NULL, DEVHOST_BARRIER_LEVEL_CORE);
	if (ret < 0) {
		dh_error("register pm barrier failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = devhost_postinit(dh_pm_postinit, NULL);
	if (ret < 0) {
		dh_error("devhost pm postinit failed: %s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

/*
 * pm sysif module, support power management related operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif_ep: `drvhandler_devhost_pm_action`
 *  - target sysif_ep: `drvhandler_devhost_pm_prepare`
 *  - target sysif-ep: `drvhandler_devhost_pm_power_off`
 *  - target sysif-ep: `drvhandler_devhost_pm_reboot`
 *  - target sysif-ep: `drvhandler_devhost_pm_cpuhp_action`
 *  - target sysif-ep: `drvhandler_devhost_pm_wl_stat`
 *  - target sysif-ep: `drvhandler_devhost_pm_dubai_hwlog`
 *  - target sysif-ep: `drvhandler_devhost_pm_thermal_action`
 *  - target advice-ep: `devhost_register_device`
 *  - target advice-ep: `devhost_unregister_device`
 *
 * export symbol:
 *  - func: `devhost_register_plat_pm`
 *  - func: `devhost_pm_init`
 *  - func: `devhost_wakelock_acquire`
 *  - func: `devhost_wakelock_release`
 *  - func: `devhost_wakelock_stat_enqueue`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(pm, dh_pm_plugin_init,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_action, dh_pm_action_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_prepare, dh_pm_prepare_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_power_off, dh_pm_power_off_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_reboot, dh_pm_reboot_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_cpuhp_action, dh_pm_cpuhp_action_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_wl_stat, dh_pm_wl_stat_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_dubai_hwlog, dh_pm_dubai_hwlog_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_pm_thermal_action, dh_pm_thermal_action_ext),
	IMPORT_ADVICE_EXTENSION(devhost_register_device, dh_register_device_ext),
	IMPORT_ADVICE_EXTENSION(devhost_unregister_device, devhost_unregister_device_ext)
);
