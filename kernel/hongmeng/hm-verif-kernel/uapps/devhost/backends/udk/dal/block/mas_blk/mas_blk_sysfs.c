/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: mas blk sysfs implement
 */

#include <udk/block/mas_blk/mas_blk_busy_idle_interface.h>
#include <udk/block/mas_blk/mas_blk_latency_interface.h>
#include <udk/block/mas_blk/mas_blk_core_interface.h>
#include <udk/block/mas_blk/mas_blk_iosched_interface.h>
#ifdef CONFIG_MAS_BKOPS
#include <udk/block/mas_blk/mas_bkops_core.h>
#endif
#include <udk/sysfs.h>

#if defined(CONFIG_MAS_DEBUG_FS)
static struct udk_sysfs_fops __cfi_mas_queue_status_entry = {
	.read = __cfi_mas_queue_status_show,
	.write = NULL,
};

static struct udk_sysfs_fops __mas_queue_io_latency_warning_threshold_entry = {
	.read = NULL,
	.write = __mas_queue_io_latency_warning_threshold_store,
};

static struct udk_sysfs_fops __cfi_mas_queue_busyidle_enable_entry = {
	.read = NULL,
	.write = __cfi_mas_queue_busyidle_enable_store,
};

static struct udk_sysfs_fops __cfi_mas_queue_busyidle_statistic_reset_entry = {
	.read = NULL,
	.write = __cfi_mas_queue_busyidle_statistic_reset_store,
};

static struct udk_sysfs_fops __cfi_mas_queue_busyidle_statistic_entry = {
	.read = __cfi_mas_queue_busyidle_statistic_show,
	.write = NULL,
};

static struct udk_sysfs_fops __cfi_mas_queue_idle_state_entry = {
	.read = __cfi_mas_queue_idle_state_show,
	.write = NULL,
};

static struct udk_sysfs_fops mas_queue_cp_enabled_entry = {
	.read = mas_queue_cp_enabled_show,
	.write = mas_queue_cp_enabled_store,
};

static struct udk_sysfs_fops mas_queue_cp_debug_en_entry = {
	.read = mas_queue_cp_debug_en_show,
	.write = mas_queue_cp_debug_en_store,
};

static struct udk_sysfs_fops mas_queue_cp_limit_entry = {
	.read = mas_queue_cp_limit_show,
	.write = mas_queue_cp_limit_store,
};

#ifdef CONFIG_MAS_BKOPS
static struct udk_sysfs_fops mas_queue_bkops_enable_entry = {
	.read = mas_queue_bkops_enable_show,
	.write = NULL,
};
#endif
#endif /* CONFIG_MAS_DEBUG_FS */

void mas_blk_sysfs_block_init(const char *prefix_name, void *ctx)
{
#if defined(CONFIG_MAS_DEBUG_FS)
	(void)udk_sysfs_create_mas_blk(prefix_name, "mas_queue_feature_status",
		&__cfi_mas_queue_status_entry, ctx);
	(void)udk_sysfs_create_mas_blk(prefix_name, "io_latency_warning_threshold",
		&__mas_queue_io_latency_warning_threshold_entry, ctx);
	(void)udk_sysfs_create_mas_blk(prefix_name, "busy_idle_enable",
		&__cfi_mas_queue_busyidle_enable_entry, ctx);
	(void)udk_sysfs_create_mas_blk(prefix_name, "busy_idle_statistic_reset",
		&__cfi_mas_queue_busyidle_statistic_reset_entry, ctx);
	(void)udk_sysfs_create_mas_blk(prefix_name, "busy_idle_statistic",
		&__cfi_mas_queue_busyidle_statistic_entry, ctx);
		(void)udk_sysfs_create_mas_blk(prefix_name, "idle_state",
		&__cfi_mas_queue_idle_state_entry, ctx);
#ifndef CONFIG_MAS_BLK_NVME
	(void)udk_sysfs_create_mas_blk(prefix_name, "cp_enabled",
		&mas_queue_cp_enabled_entry, ctx);
	(void)udk_sysfs_create_mas_blk(prefix_name, "cp_debug_en",
		&mas_queue_cp_debug_en_entry, ctx);
	(void)udk_sysfs_create_mas_blk(prefix_name, "cp_limit",
		&mas_queue_cp_limit_entry, ctx);
#endif
#ifdef CONFIG_MAS_BKOPS
	(void)udk_sysfs_create_mas_blk(prefix_name, "bkops_enable",
		&mas_queue_bkops_enable_entry, ctx);
#endif
#endif
}
