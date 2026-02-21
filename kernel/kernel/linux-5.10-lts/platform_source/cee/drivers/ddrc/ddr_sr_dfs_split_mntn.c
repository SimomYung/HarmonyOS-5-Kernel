/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/types.h>
#include <platform_include/cee/linux/hisi_ddr.h>
#include "securec.h"
#include <m3_rdr_ddr_map.h>
#include "../pm/pm.h"
#include "../pm/helper/register/register_ops.h"
#include <ddr/ddr_define_kernel.h>
#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
#include <lpm_kernel_map.h>
#else
#include <m3_sram_map.h>
#endif

#define MNTN_LOG_BUFFER_SIZE 256
#define BYTES_PER_TICK_POS 4
#define _ddr_runtime_read(offset) \
	runtime_read(M3_RDR_SYS_CONTEXT_LPMCU_STAT_OFFSET + LPMCU_TELE_MNTN_DATA_STRUCT_ACTUAL_SIZE + (offset))
#define read_ddr_sr_dfs_tickmark(offset) _ddr_runtime_read((offset) * BYTES_PER_TICK_POS)

static int ddr_sr_dfs_tickmark_show(void)
{
	if (is_runtime_base_inited() != 0) {
		pr_info("ddr lpmcu runtime base not init");
		return -EINVAL;
	}
	int offset = 0;
	unsigned char buf[MNTN_LOG_BUFFER_SIZE] = {'\0'};

	for (unsigned int i = 0; i < TICK_VOTE_USED_MAX; i++) {
		offset += sprintf_s(buf + offset, MNTN_LOG_BUFFER_SIZE, "%x ", read_ddr_sr_dfs_tickmark(i));
		if (offset < 0) {
			pr_err("ddr sr dfs mntn buf over\n");
			return -EINVAL;
		}
	}
	pr_info("ddr sr dfs record:%s\n", buf);
	(void)memset_s(buf, sizeof(buf), '\0', MNTN_LOG_BUFFER_SIZE);
	return 0;
}

static int ddr_sr_dfs_tickmark_suspend(void)
{
	return 0;
}
static int ddr_sr_dfs_tickmark_resume(void)
{
	return ddr_sr_dfs_tickmark_show();
}

static struct sr_mntn g_ddr_sr_dfs_mntn_tickmark = {
	.owner = "ddr_sr_dfs_tickmark",
	.enable = true,
	.suspend = ddr_sr_dfs_tickmark_suspend,
	.resume = ddr_sr_dfs_tickmark_resume,
};

static __init int init_ddr_sr_dfs_mntn_tickmark(void)
{
	int ret;

	ret = register_sr_mntn(&g_ddr_sr_dfs_mntn_tickmark, SR_MNTN_PRIO_L);
	if (ret != 0) {
		pr_err("register ddr sr dfs mntn module failed");
		return ret;
	}

	return 0;
}

late_initcall(init_ddr_sr_dfs_mntn_tickmark);
