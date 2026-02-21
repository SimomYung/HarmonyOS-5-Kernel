// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2021 Huawei Technologies Co., Ltd. All rights reserved.
 */

#include <linux/blackbox.h>
#include <linux/module.h>
#include <linux/blackbox_common.h>
#include <linux/blackbox_storage.h>
#ifdef CONFIG_BLACKBOX_EXPAND_EVENT
#include <linux/blackbox_subsystem_def.h>
#endif

/* ---- local macroes ---- */

/* ---- local prototypes ---- */

/* ---- local function prototypes ---- */
static void dump(const char *log_dir, struct error_info *info);
static void reset(struct error_info *info);
static int get_last_log_info(struct error_info *info);
static int save_last_log(const char *log_dir, struct error_info *info);

/* ---- function definitions ---- */
static void dump(const char *log_dir, struct error_info *info)
{
	if (unlikely(!log_dir || !info)) {
		bbox_print_err("log_dir: %p, info: %p!\n", log_dir, info);
		return;
	}
}

static void reset(struct error_info *info)
{
	if (unlikely(!info)) {
		bbox_print_err("info: %p!\n", info);
		return;
	}
}

static int get_last_log_info(struct error_info *info)
{
	bbox_print_info("There's no valid fault log!\n");

	return -ENOMSG;
}

static int save_last_log(const char *log_dir, struct error_info *info)
{
	int ret = -1;

	if (unlikely(!log_dir || !info)) {
		bbox_print_err("log_dir: %p, info: %p!\n", log_dir, info);
		return -EINVAL;
	}

	bbox_print_info("save last fault log %s!\n",
			ret ? "failed" : "successfully");

	return ret;
}

static int __init blackbox_init(void)
{
	struct module_ops ops = {
		.module = ADAPTER_SYSTEM,
		.dump = dump,
		.reset = reset,
		.get_last_log_info = get_last_log_info,
		.save_last_log = save_last_log,
	};

	if (bbox_register_module_ops(&ops) != 0) {
		bbox_print_err("bbox_register_module_ops failed!\n");
		return -EINVAL;
	}

	return 0;
}

postcore_initcall(blackbox_init);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Blackbox for system");
MODULE_AUTHOR("OHOS");
