/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for power management support
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 12 21:10:32 2019
 */

#ifndef __DEVHOST_PM_H__
#define __DEVHOST_PM_H__

#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/api.h>
#include <libhmpm/wl_stat.h>

#include <devhost/device.h>
#include <libhmpm/dubai.h>

enum {
	DH_UREF_NORMAL,       /* default elastic pool */
	DH_UREF_POPULATED,    /* non-elastic pool */
	DH_UREF_TYPE_NUM
};

cref_t dh_pm_ap_cref_of(void);
int devhost_pm_add_device(struct device_info *dev);
int devhost_pm_remove_device(const struct device_info *dev);
int devhost_pm_action(unsigned int action);
int devhost_pm_prepare(unsigned int type, char *cmd);
int devhost_plat_power_off(void);
int devhost_plat_reboot(const char *cmd);
bool is_pwrmgr_ready(void);
int devhost_pm_register(void);
int devhost_pm_cpuhp_action(unsigned int cpu, unsigned int action);
int devhost_pm_dubai_hwlog(struct pm_dubai_ctrl_log *log_info);
int devhost_pmqos_request_add(int type, int value, int *id);
int devhost_pmqos_request_update(int type, int id, int value);
int devhost_pmqos_request_remove(int type, int id);
int devhost_pm_wl_stat(struct wl_stat_queue_info *queue_info, size_t cnt,
		       size_t *enqueue_cnt);
int devhost_pm_set_min_cpufreq(unsigned int cpuid, unsigned int target_freq, int reason);
int devhost_pm_set_max_cpufreq(unsigned int cpuid, unsigned int target_freq, int reason);
int devhost_pm_thermal_action(unsigned int action, struct thermal_action_arg *arg, int *action_ret);

#endif
