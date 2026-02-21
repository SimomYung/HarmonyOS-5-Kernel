// SPDX-License-Identifier: GPL-2.0
/*
 * hotplug_thermal_cpu.c
 *
 * cpu thermal hotplug
 *
 * Copyright (C) 2017-2024 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "hotplug_thermal.h"
#ifdef CONFIG_DFX_SH
#include "dfx_sh.h"
#endif
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/topology.h>
#ifdef CONFIG_THERMAL_ISOLATE
#include <linux/delay.h>
#include <linux/core_ctl.h>
#endif

#define BIG_CLUSTER	2
#define MIDDLE_CLUSTER	1

#ifdef CONFIG_DFX_SH
#define LU_BIG_UP_THRESHOLD	80000
#define LU_BIG_DOWN_THRESHOLD	90000
static bool g_lu_controled = true;
#endif

#ifdef CONFIG_THERMAL_ISOLATE
#ifdef CONFIG_LIBLINUX
#define MAX_CPU_NUM 20
bool g_cpu_isolated_by_thermal[MAX_CPU_NUM] = {false};
#else
#define BIG_CLUSTER_ISOLATE_MASK 0xF0
bool g_thermel_iso_turn_off_core_ctl = true;
#endif
#endif
static u32 g_clusters = 2;
#define CAPACITY_OF_CLUSTER	10

#define CPU_HOTPLUG_NAME	"cpu"
static struct hotplug_ops g_cpuhp_ops;

#ifdef CONFIG_HOTPLUG_CPU_INFO
unsigned int g_cpuhp_map = 0x0;
#endif

#ifdef CONFIG_HOTPLUG_EMULATION
show_hotplug_attr(cpu, down_threshold);
store_hotplug_attr(cpu, down_threshold);
show_hotplug_attr(cpu, up_threshold);
store_hotplug_attr(cpu, up_threshold);
show_hotplug_attr(cpu, critical_down_threshold);
store_hotplug_attr(cpu, critical_down_threshold);
show_hotplug_attr(cpu, critical_up_threshold);
store_hotplug_attr(cpu, critical_up_threshold);
show_hotplug_attr(cpu, emul_temp);
store_hotplug_attr(cpu, emul_temp);

hotplug_attr_rw(cpu_down_threshold, cpu, down_threshold);
hotplug_attr_rw(cpu_up_threshold, cpu, up_threshold);
hotplug_attr_rw(critical_cpu_down_threshold, cpu, critical_down_threshold);
hotplug_attr_rw(critical_cpu_up_threshold, cpu, critical_up_threshold);
hotplug_attr_rw(hotplug_emul_temp, cpu, emul_temp);

static struct attribute *cpuhp_dev_attributes[] = {
	&dev_attr_hotplug_emul_temp.attr,
	&dev_attr_cpu_down_threshold.attr,
	&dev_attr_cpu_up_threshold.attr,
	&dev_attr_critical_cpu_down_threshold.attr,
	&dev_attr_critical_cpu_up_threshold.attr,
	NULL
};

static const struct attribute_group cpuhp_dev_attr_group = {
	.attrs = cpuhp_dev_attributes,
};

static int thermal_hotplug_create_cpu_node(struct device *dev)
{
	return device_add_group(dev, &cpuhp_dev_attr_group);
}

static void thermal_hotplug_remove_cpu_node(struct device *dev)
{
	device_remove_group(dev, &cpuhp_dev_attr_group);
}
#else
static int thermal_hotplug_create_cpu_node(struct device *dev __maybe_unused) { return 0; }

static void thermal_hotplug_remove_cpu_node(struct device *dev __maybe_unused) {}
#endif

#ifdef CONFIG_THERMAL_PERCORE
static u32 g_tdp_cpu_mask[HOTPLUG_STATUS_NR][MAX_TDP_LEVEL] = {0};
#endif

/*
 * This function is used to check cpu smt mode, hotplug mask should
 * use cpu_smt_mask when function returns true which means cpu is mt mode.
 */
static bool thermal_cpu_smt_check(void)
{
	int cpu;

	for_each_possible_cpu(cpu) {
		if (cpumask_weight(topology_sibling_cpumask(cpu)) >= 2)
			return true;
	}
	return false;
}

#if ((defined CONFIG_LIBLINUX) && (defined CONFIG_THERMAL_ISOLATE))
int liblinux_pal_set_cpu_isolate_null(int cpu, bool need_isolated)
{
	pr_err("hotplug isolate CPU%d, cur_state:%d, isolated:%d\n", cpu, g_cpu_isolated_by_thermal[cpu], need_isolated);
	return 0;
}
#endif
static void hotplug_handle_cpu(int cpu, bool action)
{
#ifdef CONFIG_LIBLINUX
#ifdef CONFIG_THERMAL_ISOLATE
	int ret;
	bool need_isolated = !action; // action: true means up; false means down.
	if (cpu >= MAX_CPU_NUM || cpu < 0) {
		pr_err("hotplug Unable to isolate CPU%d, config error\n", cpu);
		return;
	}
	// isolated and unisolated must be called in pairs, isolated cannot be called repeatedly.
	if (g_cpu_isolated_by_thermal[cpu] == need_isolated)
		return;
 
	ret = liblinux_pal_set_cpu_isolate_null(cpu, need_isolated);
	if (ret != 0)
		pr_err("hotplug Unable to isolate CPU%d, isolated:%d, ret:%d\n", cpu, need_isolated, ret);
	else
		g_cpu_isolated_by_thermal[cpu] = need_isolated;
#else
	int ret;
	int retry_time = 0;
	int (*cpu_action)(unsigned int) = NULL;

	if (action)
		cpu_action = liblinux_cpu_up;
	else
		cpu_action = liblinux_cpu_down;

	while (retry_time < 10) {
		ret = cpu_action((unsigned int)cpu);
		if (ret == 0)
			return;
		retry_time++;
	}
#endif
#elif defined(CONFIG_THERMAL_ISOLATE)
	int ret;
	int (*cpu_action)(int) = NULL;

	if (action)
		cpu_action = sched_unisolate_cpu;
	else
		cpu_action = sched_isolate_cpu;
	ret = cpu_action(cpu);
	if (ret != 0)
		pr_err("hotplug Unable to isolate CPU%d,%d\n", cpu, action);
#else
	struct device *cpu_dev = NULL;

	if (cpu_online(cpu) == action)
		return;

	cpu_dev = get_cpu_device((unsigned int)cpu);
	if (cpu_dev == NULL) {
		pr_err("cpu_dev%d invalid\n", cpu);
		return;
	}

	device_lock(cpu_dev);
	if (action) {
		cpu_device_up(cpu_dev);
		kobject_uevent(&cpu_dev->kobj, KOBJ_ONLINE);
		cpu_dev->offline = false;
	} else {
		cpu_device_down(cpu_dev);
		kobject_uevent(&cpu_dev->kobj, KOBJ_OFFLINE);
		cpu_dev->offline = true;
	}
	device_unlock(cpu_dev);
#endif
}

static void thermal_hotplug_handle_cpus(unsigned int target_mask, unsigned int range_mask)
{
	int cpu;
	unsigned int cpu_mask;
#if (!(defined CONFIG_LIBLINUX) && (defined CONFIG_THERMAL_ISOLATE))
	bool turn_off_core_ctl;

	/* Only work on single platform. Must be refactored. */
	turn_off_core_ctl = ((target_mask & BIG_CLUSTER_ISOLATE_MASK) == 0);
	if (turn_off_core_ctl != g_thermel_iso_turn_off_core_ctl) {
		g_thermel_iso_turn_off_core_ctl = turn_off_core_ctl;
		msleep(MSEC_PER_SEC / HZ);
	}
#endif

	for_each_possible_cpu(cpu) {
		cpu_mask = 1UL << (unsigned int)cpu;
		if ((cpu_mask & range_mask) == 0)
			continue;

		if ((cpu_mask & target_mask) == 0)
			hotplug_handle_cpu(cpu, true);
		else
			hotplug_handle_cpu(cpu, false);
	}
}

static void cpu_fsm_callback(enum HOTPLUG_STATUS status)
{
	u32 target_mask;
#ifdef CONFIG_DFX_SH
	if (g_lu_controled) {
		if (get_lu_sh_flag(sh_type_cluster_big)) {
			g_cpuhp_ops.down_threshold = LU_BIG_DOWN_THRESHOLD;
			g_cpuhp_ops.up_threshold = LU_BIG_UP_THRESHOLD;
			g_lu_controled = false;
			pr_err("SHLu thermal hotplug success\n");
		}
	}
#endif

	target_mask = g_cpuhp_ops.control_val[status];
#ifdef CONFIG_THERMAL_PERCORE
	u32 tdp_level = 0;
	if (is_tdp_hotplug_support(&tdp_level)) {
		if (tdp_level < MAX_TDP_LEVEL)
			target_mask = g_tdp_cpu_mask[status][tdp_level];
		else
			pr_err("cpu_fsm_callback: tdp level not right. %u\n", tdp_level);
	}
#endif
#ifdef CONFIG_HOTPLUG_CPU_INFO
	g_cpuhp_map = target_mask;
#endif

	pr_err("cpu hotplug target_mask %x, range_mask %x, current_temp:%d\n",
		target_mask, g_cpuhp_ops.control_val[HOTPLUG_CRITICAL], g_cpuhp_ops.current_temp);
	thermal_hotplug_handle_cpus(target_mask, g_cpuhp_ops.control_val[HOTPLUG_CRITICAL]);
}

#ifdef CONFIG_THERMAL_PERCORE
int get_cpu_hotplug_status(void)
{
	return g_cpuhp_ops.status;
}
#endif

static int get_cpu_temp(int *val)
{
	u32 id;
	int tmp, ret;

	ret = ipa_get_sensor_value(0, val);
	if (ret != 0)
		return ret;

	for (id = 1; id < g_clusters; id++) {
		ret = ipa_get_sensor_value(id, &tmp);
		if (ret != 0)
			return ret;
		if (tmp > *val)
			*val = tmp;
	}
	return 0;
}

/* assume cpu topology has 3 package */
static u32 get_package_bitmask(int package_id)
{
	int cpu;
	u32 mask = 0;

	for_each_possible_cpu(cpu) {
		if (topology_physical_package_id(cpu) == package_id)
			mask |= BIT((unsigned int)cpu);
	}

	return mask;
}

static int get_hotplug_bitmask(struct device_node *np, u32 *mask)
{
	int ret, i;
	u32 cluster_num = 0;
	u32 cpu_hotplug_cluster[CAPACITY_OF_CLUSTER] = {0};

	ret = of_property_read_u32(np, "ithermal,cpu_hotplug_cluster_num", &cluster_num);
	if (ret != 0) {
		pr_err("%s cpu_hotplug_cluster_num read err\n", __func__);
		return ret;
	}
	
	ret = of_property_read_u32_array(np, "ithermal,cpu_hotplug_cluster", cpu_hotplug_cluster, cluster_num);
	if (ret != 0) {
		pr_err("%s cpu_hotplug_cluster read err\n", __func__);
		return ret;
	}

	for (i = 0; i < cluster_num; i++)
		*mask |= get_package_bitmask(cpu_hotplug_cluster[i]);

	return 0;
}

static int get_critical_hotplug_bitmask(struct device_node *np, u32 *critical_mask)
{
	int ret, i;
	u32 critical_cluster_num = 0;
	u32 critical_hotplug_cluster[CAPACITY_OF_CLUSTER] = {0};

	ret = of_property_read_u32(np, "ithermal,critical_cpu_hotplug_cluster_num", &critical_cluster_num);
	if (ret != 0) {
		pr_err("%s critical_cluster_num read err\n", __func__);
		return ret;
	}
	
	ret = of_property_read_u32_array(np, "ithermal,critical_cpu_hotplug_cluster",
					 critical_hotplug_cluster, critical_cluster_num);
	if (ret != 0) {
		pr_err("%s critical_hotplug_cluster read err\n", __func__);
		return ret;
	}

	ret = get_hotplug_bitmask(np, critical_mask);
	if (ret != 0) {
		pr_err("%s get_hotplug_bitmask err\n", __func__);
		return ret;
	}
	for (i = 0; i < critical_cluster_num; i++)
		*critical_mask |= get_package_bitmask(critical_hotplug_cluster[i]);

	return 0;
}

#ifdef CONFIG_THERMAL_PERCORE
static void tdp_cpu_mask_calu(struct device_node *np)
{
	int ret;
	u32 mask = 0;
	ret = of_property_read_u32(np, "ithermal,tdp_cpu_hotplug_none_mask_low", &mask);
	if (ret != 0) {
		pr_err("%s tdp_cpu_hotplug_none_mask_low read err\n", __func__);
		return;
	}
	g_tdp_cpu_mask[HOTPLUG_NONE][0] = mask;
	ret = of_property_read_u32(np, "ithermal,tdp_cpu_hotplug_normal_mask_low", &mask);
	if (ret != 0) {
		pr_err("%s tdp_cpu_hotplug_normal_mask_low read err\n", __func__);
		return;
	}
	g_tdp_cpu_mask[HOTPLUG_NORMAL][0] = mask;
	ret = of_property_read_u32(np, "ithermal,tdp_cpu_hotplug_critical_mask_low", &mask);
	if (ret != 0) {
		pr_err("%s tdp_cpu_hotplug_critical_mask_low read err\n", __func__);
		return;
	}
	g_tdp_cpu_mask[HOTPLUG_CRITICAL][0] = mask;

	ret = of_property_read_u32(np, "ithermal,tdp_cpu_hotplug_none_mask_high", &mask);
	if (ret != 0) {
		pr_err("%s tdp_cpu_hotplug_none_mask_high read err\n", __func__);
		return;
	}
	g_tdp_cpu_mask[HOTPLUG_NONE][1] = mask;
	ret = of_property_read_u32(np, "ithermal,tdp_cpu_hotplug_normal_mask_high", &mask);
	if (ret != 0) {
		pr_err("%s tdp_cpu_hotplug_normal_mask_high read err\n", __func__);
		return;
	}
	g_tdp_cpu_mask[HOTPLUG_NORMAL][1] = mask;
	ret = of_property_read_u32(np, "ithermal,tdp_cpu_hotplug_critical_mask_high", &mask);
	if (ret != 0) {
		pr_err("%s tdp_cpu_hotplug_critical_mask_high read err\n", __func__);
		return;
	}
	g_tdp_cpu_mask[HOTPLUG_CRITICAL][1] = mask;
}
#endif

static int thermal_cpu_hotplug_dts_parse(struct device_node *np, struct hotplug_ops *ops)
{
	int ret;
	u32 mask = 0;
	u32 critical_mask = 0;

	ret = of_property_read_u32(np, "ithermal,down_threshold", (u32 *)&ops->down_threshold);
	if (ret != 0) {
		pr_err("%s hotplug up threshold read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,up_threshold", (u32 *)&ops->up_threshold);
	if (ret != 0) {
		pr_err("%s hotplug down threshold read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,critical_down_threshold",
				   (u32 *)&ops->critical_down_threshold);
	if (ret != 0)
		ops->critical_down_threshold = ops->down_threshold;

	ret = of_property_read_u32(np, "ithermal,critical_up_threshold",
				   (u32 *)&ops->critical_up_threshold);
	if (ret != 0)
		ops->critical_up_threshold = ops->up_threshold;

	if (thermal_cpu_smt_check()) {
		pr_err("%s cpu is mt mode, use cpu_smt_hotplug_mask\n", __func__);
		ret = of_property_read_u32(np, "ithermal,cpu_smt_hotplug_mask", &mask);
		if (ret != 0) {
			pr_err("%s hotplug mt maskd read err\n", __func__);
			return ret;
		}

		ret = of_property_read_u32(np, "ithermal,critical_cpu_smt_hotplug_mask", &critical_mask);
		if (ret != 0)
			critical_mask = mask;
	} else {
		ret = of_property_read_u32(np, "ithermal,cpu_hotplug_mask", &mask);
		if (ret != 0) {
			pr_err("%s hotplug maskd read err\n", __func__);
			return ret;
		}

		ret = of_property_read_u32(np, "ithermal,critical_cpu_hotplug_mask", &critical_mask);
		if (ret != 0)
			critical_mask = mask;
	}

	if (mask == 0){
		ret = get_hotplug_bitmask(np, &mask);
		if (ret != 0) {
			pr_err("%s get_hotplug_bitmask err\n", __func__);
			return ret;
		}
	}
		
	if (critical_mask == 0){
		ret = get_critical_hotplug_bitmask(np, &critical_mask);
		if (ret != 0) 
			critical_mask = mask;
	}
	g_cpuhp_ops.control_val[HOTPLUG_NORMAL] = mask;
	g_cpuhp_ops.control_val[HOTPLUG_CRITICAL] = critical_mask;
#ifdef CONFIG_THERMAL_PERCORE
	tdp_cpu_mask_calu(np);
#endif
	return 0;
}

static int thermal_cpu_hotplug_dts_parse_cluster(u32 *cluster_num)
{
	int ret;
	struct device_node *np = NULL;

	np = of_find_node_by_name(NULL, "ipa_sensors_info");
	if (np == NULL) {
		pr_err("ipa_sensors_info node not found\n");
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "ithermal,cluster_num", cluster_num);
	if (ret != 0) {
		pr_err("%s cluster_num read err\n", __func__);
		return ret;
	}

	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU_INFO
unsigned int get_cpuhp_map(void)
{
	return g_cpuhp_map;
}
#endif

#ifdef CONFIG_THERMAL_PERCORE
bool thermal_is_over_hotplug_temp(void)
{
	return (g_cpuhp_ops.status != HOTPLUG_NONE);
}
#endif

static int thermal_hotplug_cpu_init(struct device_node *np, struct device *dev)
{
	int ret;

	if (np == NULL || dev == NULL) {
		pr_err("%s fail\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	ret = thermal_cpu_hotplug_dts_parse_cluster(&g_clusters);
	if (ret != 0) {
		pr_err("%s parse dts fail\n", __func__);
		goto out;
	}

	ret = thermal_cpu_hotplug_dts_parse(np, &g_cpuhp_ops);
	if (ret != 0) {
		pr_err("%s parse dts fail\n", __func__);
		goto out;
	}

	ret = thermal_hotplug_create_cpu_node(dev);
	if (ret != 0) {
		pr_err("%s create_node fail\n", __func__);
		goto out;
	}
out:
	return ret;
}

static void thermal_hotplug_cpu_exit(struct device *dev)
{
	thermal_hotplug_remove_cpu_node(dev);
}

static struct hotplug_ops g_cpuhp_ops = {
	.name = CPU_HOTPLUG_NAME,
	.physical_status = HOTPLUG_NONE,
	.status = HOTPLUG_NONE,

	.get_temp = get_cpu_temp,
	.current_temp = 0,

	.fsm_callback = cpu_fsm_callback,
	.control_val = {0, 0, 0},

	.init = thermal_hotplug_cpu_init,
	.exit = thermal_hotplug_cpu_exit,
};

static int __init thermal_cpuhp_init(void)
{
	return hotplug_ops_register(&g_cpuhp_ops);
}

static void __exit thermal_cpuhp_exit(void)
{
	hotplug_ops_unregister(&g_cpuhp_ops);
}

module_init(thermal_cpuhp_init);
module_exit(thermal_cpuhp_exit);
