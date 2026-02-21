/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost power mgmt APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_PM_H__
#define __DEVHOST_API_PM_H__

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */
#include <stdbool.h>

#define DH_PM_EVENT_PREPARE			0U
#define DH_PM_EVENT_POST			1U
#define DH_PM_EVENT_RESTORE_PREPARE		2U
#define DH_PM_EVENT_POST_RESTORE		3U
#define DH_PM_EVENT_FREEZE			4U
#define DH_PM_EVENT_QUIESCE			5U
#define DH_PM_EVENT_THAW			6U
#define DH_PM_EVENT_RECOVER			7U
#define DH_PM_EVENT_RESTORE			8U
#define DH_PM_EVENT_SUSPEND_PREPARE_NORMAL	9U
#define DH_PM_EVENT_SUSPEND_PREPARE_FORCE	10U
#define DH_PM_EVENT_SUSPEND			11U
#define DH_PM_EVENT_SUSPEND_LATE		12U
#define DH_PM_EVENT_RESUME_EARLY		13U
#define DH_PM_EVENT_RESUME			14U
#define DH_PM_EVENT_RESUME_FINISH		15U
#define DH_PM_EVENT_ULSR_ENTER			16U
#define DH_PM_EVENT_ULSR_LEAVE			17U

struct devhost_plat_pm_ops {
	int (*prepare)(unsigned int type, char *cmd);
	int (*power_off)(void);
	int (*reboot)(const char *cmd);
	void (*syscore_shutdown)(void);
	int (*wq_freeze)(void);
	void (*wq_thaw)(void);
	int (*suspend)(unsigned int action);
	int (*cpu_add)(unsigned int cpu);
	int (*cpu_remove)(unsigned int cpu);
	int (*wakelock_stat)(void *args);
	int (*idle_enter)(void);
	int (*idle_exit)(void);
	int (*hibernate)(unsigned int action);
	int (*dubai_hwlog)(unsigned int log_lv, char *tag, char *msg);
	int (*thermal_device_acquire)(const char *type, int *id);
	int (*thermal_device_temp_read)(int id, int *temp);
	void (*preempt_bypass)(bool bypass);
};

int devhost_register_plat_pm(const struct devhost_plat_pm_ops *plat_pm_ops);
int devhost_pm_init(const struct devhost_plat_pm_ops *plat_pm_ops);

/* for shutdown hm */
int devhost_shutdown(void);
int devhost_snapshot(void);

int devhost_wakelock_acquire(void);
int devhost_wakelock_release(void);
struct wl_stat;
int devhost_wakelock_stat_enqueue(const struct wl_stat *stat, void *args);

int devhost_pmqos_request_add(int type, int value, int *id);
int devhost_pmqos_request_update(int type, int id, int value);
int devhost_pmqos_request_remove(int type, int id);

int devhost_freq_driver_init_done(void);
int devhost_pm_cpufreq_query_cur(unsigned int cpuid, unsigned int *freq);
int devhost_pm_cpufreq_query_max(unsigned int cpuid, unsigned int *freq);
int devhost_pm_cpufreq_query_hw_max(unsigned int cpuid, unsigned int *freq);
unsigned long long devhost_pm_get_cpu_idle_time(unsigned int cpu, unsigned long long *wall, int io_busy);
int devhost_pm_set_min_cpufreq(unsigned int cpuid, unsigned int target_freq, int reason);
int devhost_pm_set_max_cpufreq(unsigned int cpuid, unsigned int target_freq, int reason);

bool devhost_preempt_bypassed(void);
#endif /* __DEVHOST_API_PM_H__ */
