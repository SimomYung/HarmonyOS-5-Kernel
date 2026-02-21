// SPDX-License-Identifier: GPL-2.0
/*
 * hotplug_thermal_gpu.c
 *
 * gpu thermal hotplug
 *
 * Copyright (C) 2017-2022 Huawei Technologies Co., Ltd.
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

#define GPU_HOTPLUG_NAME	"gpu"
static struct hotplug_ops g_gpuhp_ops;

struct gpu_hotplug_ops_t {
	gpu_hotplug_t gpu_hotplug;
};
#ifdef CONFIG_GPU_CORE_HOTPLUG
struct gpu_hotplug_ops_t g_gpu_hotplug_ops = { .gpu_hotplug = gpu_thermal_cores_control, };
#else
struct gpu_hotplug_ops_t g_gpu_hotplug_ops = { .gpu_hotplug = NULL, };
#endif

#ifdef CONFIG_HOTPLUG_EMULATION
show_hotplug_attr(gpu, down_threshold);
store_hotplug_attr(gpu, down_threshold);
show_hotplug_attr(gpu, up_threshold);
store_hotplug_attr(gpu, up_threshold);
show_hotplug_attr(gpu, emul_temp);
store_hotplug_attr(gpu, emul_temp);

hotplug_attr_rw(gpu_down_threshold, gpu, down_threshold);
hotplug_attr_rw(gpu_up_threshold, gpu, up_threshold);
hotplug_attr_rw(gpu_hotplug_emul_temp, gpu, emul_temp);

static struct attribute *gpuhp_dev_attributes[] = {
	&dev_attr_gpu_hotplug_emul_temp.attr,
	&dev_attr_gpu_down_threshold.attr,
	&dev_attr_gpu_up_threshold.attr,
	NULL
};

static const struct attribute_group gpuhp_dev_attr_group = {
	.attrs = gpuhp_dev_attributes,
};

static int thermal_hotplug_create_gpu_node(struct device *dev)
{
	return device_add_group(dev, &gpuhp_dev_attr_group);
}

static void thermal_hotplug_remove_gpu_node(struct device *dev)
{
	device_remove_group(dev, &gpuhp_dev_attr_group);
}
#else
static int thermal_hotplug_create_gpu_node(struct device *dev __maybe_unused) { return 0; }

static void thermal_hotplug_remove_gpu_node(struct device *dev __maybe_unused) {}
#endif

void gpu_hotplug_callback_register(gpu_hotplug_t gpu_hotplug)
{
	if (gpu_hotplug == NULL) {
		pr_info("gpu hotplug callback register failed!\n");
		return;
	}
	g_gpu_hotplug_ops.gpu_hotplug = gpu_hotplug;
}
EXPORT_SYMBOL(gpu_hotplug_callback_register);

static void thermal_hotplug_handle_gpu(unsigned int target_cores)
{
	if (g_gpu_hotplug_ops.gpu_hotplug == NULL) {
		pr_err("%s: invalid callback\n", __func__);
		return;
	}

	g_gpu_hotplug_ops.gpu_hotplug(target_cores);
}

static void gpu_fsm_callback(enum HOTPLUG_STATUS status)
{
	pr_err("gpu to %u cores, current_temp:%d\n",
		g_gpuhp_ops.control_val[status], g_gpuhp_ops.current_temp);
	thermal_hotplug_handle_gpu(g_gpuhp_ops.control_val[status]);
}

static int get_gpu_temp(int *val)
{
	return ipa_get_dev_temp(GPU_HOTPLUG_NAME, val);
}

static int thermal_gpu_hotplug_dts_parse(struct device_node *np, struct hotplug_ops *ops)
{
	int ret;

	ret = of_property_read_u32(np, "ithermal,gpu_down_threshold", (u32 *)&ops->down_threshold);
	if (ret != 0)
		return ret;

	ret = of_property_read_u32(np, "ithermal,gpu_up_threshold", (u32 *)&ops->up_threshold);
	if (ret != 0)
		return ret;

	ret = of_property_read_u32(np, "ithermal,gpu_total_cores", ops->control_val + HOTPLUG_NONE);
	if (ret != 0) {
		pr_err("%s gpu_total_cores read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,gpu_limit_cores", ops->control_val + HOTPLUG_NORMAL);
	if (ret != 0) {
		pr_err("%s gpu_limit_cores read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,critical_gpu_down_threshold",
				   (u32 *)&ops->critical_down_threshold);
	if (ret != 0)
		ops->critical_down_threshold = ops->down_threshold;

	ret = of_property_read_u32(np, "ithermal,critical_gpu_up_threshold",
				   (u32 *)&ops->critical_up_threshold);
	if (ret != 0)
		ops->critical_up_threshold = ops->up_threshold;

	ret = of_property_read_u32(np, "ithermal,critical_gpu_limit_cores",
				   ops->control_val + HOTPLUG_CRITICAL);
	if (ret != 0)
		ops->control_val[HOTPLUG_CRITICAL] = ops->control_val[HOTPLUG_NORMAL];

	return 0;
}

static int thermal_hotplug_gpu_init(struct device_node *np, struct device *dev)
{
	int ret;

	if (np == NULL || dev == NULL) {
		pr_err("%s fail\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	ret = thermal_gpu_hotplug_dts_parse(np, &g_gpuhp_ops);
	if (ret != 0) {
		pr_err("%s parse dts fail\n", __func__);
		goto out;
	}

	ret = thermal_hotplug_create_gpu_node(dev);
	if (ret != 0) {
		pr_err("%s create_node fail\n", __func__);
		goto out;
	}
out:
	return ret;
}

static void thermal_hotplug_gpu_exit(struct device *dev)
{
	thermal_hotplug_remove_gpu_node(dev);
}

static struct hotplug_ops g_gpuhp_ops = {
	.name = GPU_HOTPLUG_NAME,
	.physical_status = HOTPLUG_NONE,
	.status = HOTPLUG_NONE,

	.get_temp = get_gpu_temp,
	.current_temp = 0,

	.fsm_callback = gpu_fsm_callback,
	.control_val = {0},

	.init = thermal_hotplug_gpu_init,
	.exit = thermal_hotplug_gpu_exit,
};

static int __init thermal_gpuhp_init(void)
{
	return hotplug_ops_register(&g_gpuhp_ops);
}

static void __exit thermal_gpuhp_exit(void)
{
	hotplug_ops_unregister(&g_gpuhp_ops);
}

module_init(thermal_gpuhp_init);
module_exit(thermal_gpuhp_exit);
