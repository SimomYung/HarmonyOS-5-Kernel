/*
 * discharge_boost.c
 *
 * discharge boost driver
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "dischg_boost.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <securec.h>
#include <chipset_common/hwpower/hardware_ic/buck_boost.h>
#include <chipset_common/hwpower/hardware_ic/boost_5v.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_debug.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_sysfs.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include <huawei_platform/log/hw_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG dischg_boost
HWLOG_REGIST();

#define VSYS_SWITCH_SCENE_NORMAL  0x1
#define VSYS_SWITCH_SCENE_BOOST   0x2
#define VSYS_SWITCH_SCENE_PENDING 0x4 // usually as a buffer zone bewteen normal and boost

#define DISCHG_BOOST_SWITCH_BOOST_VOUT  3600
#define DISCHG_BOOST_MAX_PATH           10

typedef int (*switch_state_handler)(struct dischg_boost_context *ctx);
 
typedef int (*switch_path_handler)(struct dischg_boost_config *ctx);

enum {
	DIS_BST_NO_ENFORCE,
	DIS_BST_ENFORCE_ON,
	DIS_BST_ENFORCE_OFF
};

enum {
	DISCHG_BOOST_SWITCH_MODE_GPIO,
	DISCHG_BOOST_SWITCH_MODE_BUCKBOOST,
	DISCHG_BOOST_SWITCH_MODE_BOOST_5V,
};

enum dischg_boost_sysfs_type {
	DISCHG_BOOST_SYSFS_BEGIN = 0,
	DISCHG_BOOST_SYSFS_ENFORCE_STS = DISCHG_BOOST_SYSFS_BEGIN,
	DISCHG_BOOST_SYSFS_END,
};

struct switch_cond {
	int prev_scene;
	int next_scene;
};

struct switch_state_item {
	struct switch_cond cond;
	switch_state_handler handler;
};

struct switch_path {
	int switch_mode;
	switch_path_handler boost_handler;
	switch_path_handler normal_handler;
};

static int dischg_boost_switch_to_boost_by_gpio(struct dischg_boost_config *cfg)
{
	if (gpio_direction_output(cfg->normal_gpio, 0))
		return -EPERM;
	power_usleep(cfg->switch_boost_delay_ms * DT_USLEEP_1MS);
	if (gpio_direction_output(cfg->boost_gpio, 1))
		return gpio_direction_output(cfg->normal_gpio, 1);
	return 0;
}

static int dischg_boost_switch_to_normal_by_gpio(struct dischg_boost_config *cfg)
{
	if (gpio_direction_output(cfg->boost_gpio, 0))
		return -EPERM;
	power_usleep(cfg->switch_normal_delay_ms * DT_USLEEP_1MS);
	if (gpio_direction_output(cfg->normal_gpio, 1))
		return gpio_direction_output(cfg->boost_gpio, 1);
	return 0;
}

static int dischg_boost_switch_to_boost_by_buckboost(struct dischg_boost_config *cfg)
{
	if (!buckboost_set_enable(1, BBST_USER_BOOST_DISCHG, cfg->index))
		return -EPERM;
	if (buckboost_set_vout(cfg->switch_boost_vout, BBST_USER_BOOST_DISCHG, cfg->index))
		return buckboost_set_enable(0, BBST_USER_BOOST_DISCHG, cfg->index) ? 0 : -EPERM;
	return 0;
}

static int dischg_boost_switch_to_normal_by_buckboost(struct dischg_boost_config *cfg)
{
	return buckboost_set_enable(0, BBST_USER_BOOST_DISCHG, cfg->index);
}

static int dischg_boost_switch_to_boost_by_boost_5v(struct dischg_boost_config *cfg)
{
	return boost_5v_enable(true, BOOST_CTRL_DISCHG_BOOST);
}

static int dischg_boost_switch_to_normal_by_boost_5v(struct dischg_boost_config *cfg)
{
	return boost_5v_enable(false, BOOST_CTRL_DISCHG_BOOST);
}

static const struct switch_path path_handler_map[] = {
	{
		.switch_mode = DISCHG_BOOST_SWITCH_MODE_GPIO,
		.boost_handler = dischg_boost_switch_to_boost_by_gpio,
		.normal_handler = dischg_boost_switch_to_normal_by_gpio,
	},
	{
		.switch_mode = DISCHG_BOOST_SWITCH_MODE_BUCKBOOST,
		.boost_handler = dischg_boost_switch_to_boost_by_buckboost,
		.normal_handler = dischg_boost_switch_to_normal_by_buckboost,
	},
	{
		.switch_mode = DISCHG_BOOST_SWITCH_MODE_BOOST_5V,
		.boost_handler = dischg_boost_switch_to_boost_by_boost_5v,
		.normal_handler = dischg_boost_switch_to_normal_by_boost_5v,
	},
};

static inline bool dischg_boost_match_mode(int cur_mode, int handler_mode)
{
	return cur_mode == handler_mode;
}

static int dischg_boost_switch_to_boost(struct dischg_boost_context *ctx)
{
	int ret = -EPERM;
	size_t i;
	size_t size = ARRAY_SIZE(path_handler_map);

	if (!ctx || !ctx->dischg_dev) {
		hwlog_info("config or dischg_dev is null\n");
		return -EPERM;
	}

	hwlog_info("switch_to_boost begin\n");
	power_wakeup_lock(ctx->dischg_dev->wakelock, false);
	for (i = 0; i < size; ++i) {
		if (dischg_boost_match_mode(ctx->dischg_cfg.switch_mode, path_handler_map[i].switch_mode)) {
			ret = path_handler_map[i].boost_handler(&ctx->dischg_cfg);
			break;
		}
	}
	power_wakeup_unlock(ctx->dischg_dev->wakelock, false);
	hwlog_info("switch_to_boost end, ret=%d\n", ret);
	return ret;
}

static int dischg_boost_switch_to_normal(struct dischg_boost_context *ctx)
{
	int ret = -EPERM;
	size_t i;
	size_t size = ARRAY_SIZE(path_handler_map);

	if (!ctx || !ctx->dischg_dev) {
		hwlog_info("config or dischg_dev is null\n");
		return -EPERM;
	}

	hwlog_info("switch_to_normal begin\n");
	power_wakeup_lock(ctx->dischg_dev->wakelock, false);
	for (i = 0; i < size; ++i) {
		if (dischg_boost_match_mode(ctx->dischg_cfg.switch_mode, path_handler_map[i].switch_mode)) {
			ret = path_handler_map[i].normal_handler(&ctx->dischg_cfg);
			break;
		}
	}
	power_wakeup_unlock(ctx->dischg_dev->wakelock, false);
	hwlog_info("switch_to_normal end, ret=%d\n", ret);
	return ret;
}

static const struct switch_state_item state_handler_map[] = {
	{
		.cond = {
			.prev_scene = VSYS_SWITCH_SCENE_BOOST | VSYS_SWITCH_SCENE_PENDING,
			.next_scene = VSYS_SWITCH_SCENE_NORMAL,
		},
		.handler = dischg_boost_switch_to_normal,
	},
	{
		.cond = {
			.prev_scene = VSYS_SWITCH_SCENE_NORMAL | VSYS_SWITCH_SCENE_PENDING,
			.next_scene = VSYS_SWITCH_SCENE_BOOST,
		},
		.handler = dischg_boost_switch_to_boost,
	},
};

static inline int dischg_boost_get_scene_by_soc(
	const struct dischg_boost_config *cfg, int soc)
{
	if (soc <= cfg->switch_boost_soc)
		return VSYS_SWITCH_SCENE_BOOST;
	else if (soc > cfg->switch_normal_soc)
		return VSYS_SWITCH_SCENE_NORMAL;
	else
		return VSYS_SWITCH_SCENE_PENDING;
}

static inline bool dischg_boost_match_scene(int scene, int scenes)
{
	return !!(scene & scenes);
}

static inline bool dischg_boost_match_cond(const struct switch_cond *cur,
	const struct switch_cond *item)
{
	return dischg_boost_match_scene(cur->prev_scene, item->prev_scene) &&
		dischg_boost_match_scene(cur->next_scene, item->next_scene);
}

static inline switch_state_handler dischg_boost_get_switch_handler(const struct switch_cond *cur_cond)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(state_handler_map); ++i) {
		if (dischg_boost_match_cond(cur_cond, &state_handler_map[i].cond))
			return state_handler_map[i].handler;
	}
	return NULL;
}

static inline bool dischg_boost_has_switch_handler(const struct switch_cond *cur_cond)
{
	return !!dischg_boost_get_switch_handler(cur_cond);
}

#ifdef CONFIG_HUAWEI_POWER_DEBUG
static ssize_t dischg_boost_dbg_get_scene_store(void *dev_data,
	const char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	int soc = 0;
	int index = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	if (sscanf(buf, "%d %d", &index, &soc) != 2) {
		hwlog_err("buf does not contain two integers\n");
		return -EINVAL;
	}

	hwlog_info("soc=%d, scene=%d\n", soc,
		dischg_boost_get_scene_by_soc(&di->dischg_context[index].dischg_cfg, soc));
	return size;
}

static ssize_t dischg_boost_dbg_exec_handler_store(void *dev_data,
	const char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	int prev_soc = 0;
	int next_soc = 0;
	int index = 0;
	struct switch_cond cond = { 0 };
	switch_state_handler handler = NULL;
	struct dischg_boost_config *cfg = NULL;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	/* 3: three parameters */
	if (sscanf(buf, "%d %d %d", &index, &prev_soc, &next_soc) != 3) {
		hwlog_err("buf does not contain three integers\n");
		return -EINVAL;
	}

	cfg = &di->dischg_context[index].dischg_cfg;
	cond.prev_scene = dischg_boost_get_scene_by_soc(cfg, prev_soc);
	cond.next_scene = dischg_boost_get_scene_by_soc(cfg, next_soc);
	hwlog_info("prev_soc=%d, prev_scene=%d, next_soc=%d, next_scene=%d\n",
		prev_soc, cond.prev_scene, next_soc, cond.next_scene);

	handler = dischg_boost_get_switch_handler(&cond);
	if (!handler) {
		hwlog_info("handler is null\n");
		return -EINVAL;
	}
	handler(&di->dischg_context[index]);
	return size;
}

static ssize_t dischg_boost_dbg_dischg_soc_cfg_show(void *dev_data,
	char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	struct dischg_boost_config *cfg = NULL;
	int i;
	int len;
	int ret = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	for (i = 0; i < di->path_num; i++) {
		cfg = &di->dischg_context[i].dischg_cfg;
		len = scnprintf(buf + ret, size, "boost_soc=%d, normal_soc=%d\n",
			cfg->switch_boost_soc, cfg->switch_normal_soc);
		if (len < 0)
			return ret;
		ret += len;
	}
	return ret;
}

static ssize_t dischg_boost_dbg_dischg_soc_cfg_store(void *dev_data,
	const char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	struct dischg_boost_config *cfg = NULL;
	int boost_soc = 0;
	int normal_soc = 0;
	int index = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	/* 3: three parameters */
	if (sscanf(buf, "%d %d %d", &index, &boost_soc, &normal_soc) != 3) {
		hwlog_err("buf does not contain three integers\n");
		return -EINVAL;
	}

	cfg = &di->dischg_context[index].dischg_cfg;
	cfg->switch_boost_soc = boost_soc;
	cfg->switch_normal_soc = normal_soc;
	hwlog_info("boost_soc=%d, normal_soc=%d\n", boost_soc, normal_soc);
	return size;
}

static ssize_t dischg_boost_dbg_switch_delay_ms_show(void *dev_data,
	char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	struct dischg_boost_config *cfg = NULL;
	int i;
	int len;
	int ret = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	for (i = 0; i < di->path_num; i++) {
		cfg = &di->dischg_context[i].dischg_cfg;
		len = scnprintf(buf + ret, size, "boost_delay_ms=%d, normal_delay_ms=%d\n",
			cfg->switch_boost_delay_ms, cfg->switch_normal_delay_ms);
		if (len < 0)
			return ret;
		ret += len;
	}

	return ret;
}

static ssize_t dischg_boost_dbg_switch_delay_ms_store(void *dev_data,
	const char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	struct dischg_boost_config *cfg = NULL;
	int boost_delay_ms = 0;
	int normal_delay_ms = 0;
	int index = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	/* 3: three parameters */
	if (sscanf(buf, "%d %d %d", &index, &boost_delay_ms, &normal_delay_ms) != 3) {
		hwlog_err("buf does not contain three integers\n");
		return -EINVAL;
	}

	cfg = &di->dischg_context[index].dischg_cfg;
	cfg->switch_boost_delay_ms = boost_delay_ms;
	cfg->switch_normal_delay_ms = normal_delay_ms;
	hwlog_info("boost_delay_ms=%d, normal_delay_ms=%d\n",
		boost_delay_ms, normal_delay_ms);
	return size;
}

static ssize_t dischg_boost_dbg_switch_boost_vout_show(void *dev_data,
	char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	struct dischg_boost_config *cfg = NULL;
	int i;
	int len;
	int ret = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	for (i = 0; i < di->path_num; i++) {
		cfg = &di->dischg_context[i].dischg_cfg;
		len = scnprintf(buf + ret, size, "boost_vout=%d\n",
			cfg->switch_boost_vout);
		if (len < 0)
			return ret;
		ret += len;
	}

	return ret;
}

static ssize_t dischg_boost_dbg_switch_boost_vout_store(void *dev_data,
	const char *buf, size_t size)
{
	struct dischg_boost_dev *di = dev_data;
	struct dischg_boost_config *cfg = NULL;
	int boost_vout = 0;
	int index = 0;

	if (!buf || !di || !di->dischg_context) {
		hwlog_err("buf or di or context is null\n");
		return -EINVAL;
	}

	/* 2: two parameters */
	if (sscanf(buf, "%d %d", &index, &boost_vout) != 2) {
		hwlog_err("buf does not contain two integers\n");
		return -EINVAL;
	}

	cfg = &di->dischg_context[index].dischg_cfg;
	cfg->switch_boost_vout = boost_vout;
	hwlog_info("boost_vout=%d\n", boost_vout);
	return size;
}
#endif /* CONFIG_HUAWEI_POWER_DEBUG */

static inline int dischg_boost_get_current_by_enforce(int enforce_sts, int *scene)
{
	switch (enforce_sts) {
	case DIS_BST_ENFORCE_ON:
		*scene = VSYS_SWITCH_SCENE_BOOST;
		return 0;
	case DIS_BST_ENFORCE_OFF:
		*scene = VSYS_SWITCH_SCENE_NORMAL;
		return 0;
	default:
		return -EINVAL;
	}
}

static inline int dischg_boost_get_current_by_real_soc(
	const struct dischg_boost_config *cfg)
{
	int soc = 0;

	(void)power_supply_get_int_property_value(POWER_PLATFORM_BAT_PSY_NAME,
		POWER_SUPPLY_PROP_CAPACITY, &soc);
	return dischg_boost_get_scene_by_soc(cfg, soc);
}

static inline int dischg_boost_get_current_scene(const struct dischg_boost_context *ctx)
{
	int cur_scene = 0;

	if (!dischg_boost_get_current_by_enforce(ctx->enforce_sts, &cur_scene))
		return cur_scene;

	return dischg_boost_get_current_by_real_soc(&ctx->dischg_cfg);
}

static void dischg_boost_work(struct work_struct *work)
{
	int current_scene;
	int previous_scene;
	struct dischg_boost_context *ctx = NULL;
	switch_state_handler handler = NULL;
	struct switch_cond cur_cond = { 0 };

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	ctx= container_of(work, struct dischg_boost_context, switch_work);
	if (!ctx) {
		hwlog_err("context is null\n");
		return;
	}

	previous_scene = atomic_read(&ctx->prev_scene);
	current_scene = dischg_boost_get_current_scene(ctx);
	if (current_scene == previous_scene) {
		hwlog_err("scene:%d has not been changed\n", current_scene);
		return;
	}

	cur_cond.next_scene = current_scene;
	cur_cond.prev_scene = previous_scene;
	handler = dischg_boost_get_switch_handler(&cur_cond);
	if (!handler) {
		hwlog_err("handler is null\n");
		return;
	}

	hwlog_info("dischg_boost_work execute handler\n");
	handler(ctx);
	atomic_set(&ctx->prev_scene, current_scene);
}

static void dischg_boost_trigger_switch_by_path(struct dischg_boost_context *ctx)
{
	int cur_scene;
	struct switch_cond cur_cond = { 0 };
	int prev_scene;

	if (!ctx)
		return;

	prev_scene = atomic_read(&ctx->prev_scene);
	cur_scene = dischg_boost_get_current_scene(ctx);
	if (prev_scene == cur_scene)
		return;

	cur_cond.next_scene = cur_scene;
	cur_cond.prev_scene = prev_scene;

	if (!dischg_boost_has_switch_handler(&cur_cond))
		return;

	if (work_busy(&ctx->switch_work)) {
		hwlog_info("switch work was just triggered, ignore\n");
		return;
	}

	hwlog_info("trigger switch work\n");
	schedule_work(&ctx->switch_work);
}

static void dischg_boost_trigger_switch(struct dischg_boost_dev *di)
{
	int i;

	if (!di || !di->dischg_context)
		return;

	for (i = 0; i < di->path_num; i++)
		dischg_boost_trigger_switch_by_path(&di->dischg_context[i]);
}

static int dischg_boost_psy_change_cb(
	struct notifier_block *nb, unsigned long event, void *data)
{
	struct dischg_boost_dev *di = NULL;
	struct power_supply *psy = data;

	if (!nb || !data) {
		hwlog_err("nb or data is null\n");
		return NOTIFY_OK;
	}

	di = container_of(nb, struct dischg_boost_dev, soc_change_nb);
	if (!di) {
		hwlog_err("di is null\n");
		return NOTIFY_OK;
	}

	if (strcmp(psy->desc->name, POWER_PLATFORM_BAT_PSY_NAME)) {
		hwlog_info("psy %s is not target, ignore\n", POWER_PLATFORM_BAT_PSY_NAME);
		return NOTIFY_OK;
	}

	dischg_boost_trigger_switch(di);
	return NOTIFY_OK;
}

static void dischg_boost_parse_config(struct dischg_boost_config *cfg,
	struct device_node *np, int index)
{
	(void)power_dts_read_u32_index_dflt(power_dts_tag(HWLOG_TAG), np,
		"switch_boost_soc", index, &cfg->switch_boost_soc, 20); // 20: 20% soc
	(void)power_dts_read_u32_index_dflt(power_dts_tag(HWLOG_TAG), np,
		"switch_normal_soc", index, &cfg->switch_normal_soc, 25); // 25: 25% soc
	(void)power_dts_read_u32_index_dflt(power_dts_tag(HWLOG_TAG), np,
		"switch_mode", index, &cfg->switch_mode, DISCHG_BOOST_SWITCH_MODE_GPIO);
	(void)power_dts_read_u32_index_dflt(power_dts_tag(HWLOG_TAG), np,
		"switch_boost_delay_ms", index, &cfg->switch_boost_delay_ms, 1); // 1: 1ms
	(void)power_dts_read_u32_index_dflt(power_dts_tag(HWLOG_TAG), np,
		"switch_normal_delay_ms", index, &cfg->switch_normal_delay_ms, 4); // 4: 4ms
	(void)power_dts_read_u32_index_dflt(power_dts_tag(HWLOG_TAG), np,
		"switch_boost_vout", index, &cfg->switch_boost_vout, DISCHG_BOOST_SWITCH_BOOST_VOUT);
}

static int dischg_boost_init_gpio(struct dischg_boost_config *cfg, struct device_node *np, int index)
{
	int ret = 0;

	ret += power_gpio_config_output_index(np, "normal_gpio",
		"dischg_normal_gpio", index, &cfg->normal_gpio, 1);
	ret += power_gpio_config_output_index(np, "boost_gpio",
		"dischg_boost_gpio", index, &cfg->boost_gpio, 0);

	return ret;
}

static int dischg_boost_init_discharge_context(struct dischg_boost_dev *di, struct device_node *np)
{
	int i;
	struct dischg_boost_context *ctx = NULL;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"path_num", &di->path_num, 1);

	if ((di->path_num <= 0) || (di->path_num > DISCHG_BOOST_MAX_PATH)) {
		hwlog_info("path num is out of range!\n");
		return -EPERM;
	}

	di->dischg_context = kzalloc(sizeof(*di->dischg_context) * di->path_num, GFP_KERNEL);
	if (!di->dischg_context)
		return -ENOMEM;

	for (i = 0; i < di->path_num; i++) {
		ctx = &di->dischg_context[i];
		ctx->dischg_cfg.index = i;
		ctx->dischg_dev = di;
		INIT_WORK(&ctx->switch_work, dischg_boost_work);
		dischg_boost_parse_config(&ctx->dischg_cfg, np, i);
		if (ctx->dischg_cfg.switch_mode == DISCHG_BOOST_SWITCH_MODE_GPIO) {
			if (dischg_boost_init_gpio(&ctx->dischg_cfg, np, i))
				return -EPERM;
		}
		// we assume that previous scene is NORMAL, and the gpios will be inited as NORMAL scene
		atomic_set(&ctx->prev_scene, VSYS_SWITCH_SCENE_NORMAL);
	}

	return 0;
}

static inline void dischg_boost_register_power_debug(struct dischg_boost_dev *di)
{
#ifdef CONFIG_HUAWEI_POWER_DEBUG
	power_dbg_ops_register("dischg_boost", "get_scene", di,
		NULL,
		dischg_boost_dbg_get_scene_store);
	power_dbg_ops_register("dischg_boost", "exec_handler", di,
		NULL,
		dischg_boost_dbg_exec_handler_store);
	power_dbg_ops_register("dischg_boost", "dischg_soc_cfg", di,
		dischg_boost_dbg_dischg_soc_cfg_show,
		dischg_boost_dbg_dischg_soc_cfg_store);
	power_dbg_ops_register("dischg_boost", "switch_delay_ms", di,
		dischg_boost_dbg_switch_delay_ms_show,
		dischg_boost_dbg_switch_delay_ms_store);
	power_dbg_ops_register("dischg_boost", "switch_boost_vout", di,
		dischg_boost_dbg_switch_boost_vout_show,
		dischg_boost_dbg_switch_boost_vout_store);
#endif /* CONFIG_HUAWEI_POWER_DEBUG */
}

#ifdef CONFIG_SYSFS
static ssize_t dischg_boost_sysfs_show(struct device *device,
	struct device_attribute *attr, char *buf);
static ssize_t dischg_boost_sysfs_store(struct device *device,
	struct device_attribute *attr, const char *buf, size_t count);

static struct power_sysfs_attr_info dischg_boost_sysfs_field_tbl[] = {
	power_sysfs_attr_rw(dischg_boost, 0660,
		DISCHG_BOOST_SYSFS_ENFORCE_STS, enforce_status),
};

#define DISCHG_BOOST_SYSFS_ATTRS_SIZE ARRAY_SIZE(dischg_boost_sysfs_field_tbl)

static struct attribute *dischg_boost_sysfs_attrs[DISCHG_BOOST_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group dischg_boost_sysfs_attr_group = {
	.attrs = dischg_boost_sysfs_attrs,
};

static ssize_t dischg_boost_sysfs_show(struct device *device,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct dischg_boost_dev *di = NULL;
	int i;
	int len;
	int ret = 0;

	if (!device) {
		hwlog_err("%s device is null\n", __func__);
		return -ENODEV;
	}

	di = dev_get_drvdata(device);
	if (!di || !di->dischg_context) {
		hwlog_err("%s di or context is null\n", __func__);
		return -ENODEV;
	}

	info = power_sysfs_lookup_attr(attr->attr.name,
		dischg_boost_sysfs_field_tbl, DISCHG_BOOST_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case DISCHG_BOOST_SYSFS_ENFORCE_STS:
		for (i = 0; i < di->path_num; i++) {
			len = snprintf_s(buf + ret, PAGE_SIZE, PAGE_SIZE - ret -1,
				"enforce_sts=%d\n", di->dischg_context[i].enforce_sts);
			if (len < 0)
				return ret;
			ret += len;
		}
		return ret;
	default:
		return 0;
	}
}

static ssize_t dischg_boost_sysfs_store(struct device *device,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct power_sysfs_attr_info *info = NULL;
	struct dischg_boost_dev *di = NULL;
	int value = 0;
	int i;

	if (!device) {
		hwlog_err("%s device is null\n", __func__);
		return -ENODEV;
	}

	di = dev_get_drvdata(device);
	if (!di || !di->dischg_context) {
		hwlog_err("%s di or config is null\n", __func__);
		return -ENODEV;
	}

	info = power_sysfs_lookup_attr(attr->attr.name,
		dischg_boost_sysfs_field_tbl, DISCHG_BOOST_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	if (kstrtoint(buf, POWER_BASE_DEC, &value) < 0) {
		hwlog_err("kstrtoint value %s error\n", buf);
		return -EINVAL;
	}

	switch (info->name) {
	case DISCHG_BOOST_SYSFS_ENFORCE_STS:
		if ((value > DIS_BST_ENFORCE_OFF) || (value < DIS_BST_NO_ENFORCE))
			return -EINVAL;
		for (i = 0; i < di->path_num; i++)
			di->dischg_context[i].enforce_sts = value;
		dischg_boost_trigger_switch(di);
		break;
	default:
		break;
	}

	return count;
}

static struct device * dischg_boost_sysfs_create_group(void)
{
	power_sysfs_init_attrs(dischg_boost_sysfs_attrs,
		dischg_boost_sysfs_field_tbl, DISCHG_BOOST_SYSFS_ATTRS_SIZE);
	return power_sysfs_create_group("hw_power", "dischg_boost",
		&dischg_boost_sysfs_attr_group);
}

static void dischg_boost_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_group(dev, &dischg_boost_sysfs_attr_group);
}
#else
static inline struct device * dischg_boost_sysfs_create_group(void)
{
	return NULL;
}

static inline void dischg_boost_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int dischg_boost_probe(struct platform_device *pdev)
{
	int ret;
	struct dischg_boost_dev *di = NULL;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = devm_kzalloc(&pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	ret = dischg_boost_init_discharge_context(di, pdev->dev.of_node);
	if (ret) {
		hwlog_err("init context fail, ret:%d\n", ret);
		goto free_mem;
	}

	di->soc_change_nb.notifier_call = dischg_boost_psy_change_cb;
	ret = power_supply_reg_notifier(&di->soc_change_nb);
	if (ret) {
		hwlog_err("register soc_change_nb failed, ret:%d\n", ret);
		goto free_mem;
	}
	di->dev = dischg_boost_sysfs_create_group();
	dischg_boost_register_power_debug(di);
	di->wakelock = power_wakeup_source_register(&pdev->dev, "dischg_boost_wakelock");
	dischg_boost_trigger_switch(di);

	platform_set_drvdata(pdev, di);
	dev_set_drvdata(di->dev, di);
	return 0;

free_mem:
	devm_kfree(&pdev->dev, di);
	platform_set_drvdata(pdev, NULL);
	return ret;
}

static int dischg_boost_remove(struct platform_device *pdev)
{
	struct dischg_boost_dev *di = platform_get_drvdata(pdev);

	if (!di) {
		hwlog_err("di NULL\n");
		return -ENODEV;
	}
	kfree(di->dischg_context);
	power_supply_unreg_notifier(&di->soc_change_nb);
	power_wakeup_source_unregister(di->wakelock);
	dischg_boost_sysfs_remove_group(di->dev);
	return 0;
}

static void dischg_boost_shutdown(struct platform_device *pdev)
{
	struct dischg_boost_dev *di = platform_get_drvdata(pdev);

	if (!di) {
		hwlog_err("di NULL\n");
		return;
	}
	kfree(di->dischg_context);
	power_supply_unreg_notifier(&di->soc_change_nb);
	power_wakeup_source_unregister(di->wakelock);
}

#ifdef CONFIG_PM
static int dischg_boost_resume(struct platform_device *pdev)
{
	struct dischg_boost_dev *di = platform_get_drvdata(pdev);

	if (!di) {
		hwlog_err("di NULL\n");
		return -ENODEV;
	}
	dischg_boost_trigger_switch(di);
	return 0;
}
#endif /* CONFIG_PM */

static const struct of_device_id dischg_boost_match_table[] = {
	{
		.compatible = "huawei,dischg_boost",
		.data = NULL,
	},
	{},
};

static struct platform_driver dischg_boost_driver = {
	.probe = dischg_boost_probe,
	.remove = dischg_boost_remove,
#ifdef CONFIG_PM
	.resume = dischg_boost_resume,
#endif /* CONFIG_PM */
	.shutdown = dischg_boost_shutdown,
	.driver = {
		.name = "huawei,dischg_boost",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(dischg_boost_match_table),
	},
};

static int __init dischg_boost_init(void)
{
	return platform_driver_register(&dischg_boost_driver);
}

static void __exit dischg_boost_exit(void)
{
	platform_driver_unregister(&dischg_boost_driver);
}

late_initcall(dischg_boost_init);
module_exit(dischg_boost_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("discharge boost module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
