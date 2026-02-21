/*
 * thermal_dmd.c
 *
 * trans temp from bbox to dmd
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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

#include <securec.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/string.h>
#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
#include <dmd_kernel_map.h>
#else
#include <lpmcu_runtime.h>
#endif
#include <m3_rdr_ddr_map.h>
#include <platform_include/cee/linux/thermal_dmd.h>
#include <../../../basicplatform/drivers/mntn/blackbox/rdr_field.h>

#define TEMP_VAL_OFFSET		(M3_RDR_SYS_CONTEXT_RUNTIME_VAR_OFFSET + \
				(RUNTIME_TMP_CUR_ADDR(0) - RUNTIME_VAR_BASE))
#define CAPACITY_DMD_TEMP_ARRAY	6

static unsigned int g_dmd_temp_num;
static const char *g_dmd_temp_name[CAPACITY_DMD_TEMP_ARRAY];
static bool g_dmd_temp_para_is_read;

static u64 get_tempinfo_offset(u64 *offset)
{
	int ret;
	struct rdr_register_module_result rdr_area_ap_info;
	struct rdr_register_module_result rdr_area_lpm3_info;

	ret = rdr_get_areainfo(RDR_AREA_AP, &rdr_area_ap_info);
	if (ret != 0) {
		pr_err("rdr_get_areainfo of RDR_AREA_AP failed.\n");
		return -EINVAL;
	}

	ret = rdr_get_areainfo(RDR_AREA_LPM3, &rdr_area_lpm3_info);
	if (ret != 0) {
		pr_err("rdr_get_areainfo of RDR_AREA_LPM3 failed.\n");
		return -EINVAL;
	}

	if (rdr_area_lpm3_info.log_addr < rdr_area_ap_info.log_addr) {
		pr_err("log_addr is invalid\n");
		return -EINVAL;
	}

	*offset = rdr_area_lpm3_info.log_addr - rdr_area_ap_info.log_addr +
		  RDR_BASEINFO_SIZE + TEMP_VAL_OFFSET;
	return ret;
}

static int save_temp_info_to_buf(u64 temp_log_addr, char *out_buf, u32 buf_size)
{
	int i, ret = 0;
	int offset = 0;

	for (i = 0; i < g_dmd_temp_num; i++) {
		if (i == 0) {
			ret = sprintf_s(out_buf + offset, sizeof(char) * buf_size - offset,
					"0x%02x", *((u8 volatile *)(uintptr_t)(temp_log_addr + i)));
		} else {
			ret = sprintf_s(out_buf + offset, sizeof(char) * buf_size - offset,
					",0x%02x", *((u8 volatile *)(uintptr_t)(temp_log_addr + i)));
		}
		if (ret < 0) {
			pr_err("%s sprint_s fail!\n", __func__);
			return ret;
		}
		offset += ret;
	}

	return offset;
}

int parse_tempinfo_from_rdr(const void *bbox_base, size_t size, char *out_buf, size_t buf_size)
{
	int ret = 0;
	u64 temp_log_addr;
	u64 temp_log_offset;

	if (!g_dmd_temp_para_is_read) {
		pr_err("dmd_temp paras read err for %s\n", __func__);
		return -EINVAL;
	}

	if (bbox_base == NULL || out_buf == NULL) {
		pr_err("%s addr is NULL\n", __func__);
		return -EINVAL;
	}

	ret = get_tempinfo_offset(&temp_log_offset);
	if (ret != 0) {
		pr_err("get temp_log_offset failed\n");
		return -EINVAL;
	}

	if (size < g_dmd_temp_num || (size - g_dmd_temp_num) < temp_log_offset) {
		pr_err("size of bbox_base is invalid.\n");
		return -EINVAL;
	}

	temp_log_addr = (void __iomem *)(bbox_base + temp_log_offset);

	ret = save_temp_info_to_buf(temp_log_addr, out_buf, buf_size);
	if (ret < 0)
		pr_err("save temp info format err: %s, ret:%d\n", out_buf, ret);

	return ret;
}

static void reset_dmd_temp_info(void)
{
	int i;

	for (i = 0; i < CAPACITY_DMD_TEMP_ARRAY; i++)
		g_dmd_temp_name[i] = NULL;
	g_dmd_temp_num = 0;
	g_dmd_temp_para_is_read = false;
}

void parse_temp_dmd_parameter(const struct device_node *np)
{
	int i, ret = 0;

	ret = of_property_read_s32(np, "ithermal,dmd_temp_num", &g_dmd_temp_num);
	if (ret != 0 || g_dmd_temp_num == 0 || g_dmd_temp_num > CAPACITY_DMD_TEMP_ARRAY) {
		pr_err("missing dmd_temp number\n");
		reset_dmd_temp_info();
		return;
	}

	for (i = 0; i < g_dmd_temp_num; i++) {
		ret = of_property_read_string_index(np, "ithermal,dmd_temp_name",
						    i, &g_dmd_temp_name[i]);
		if (ret != 0) {
			pr_err("%s g_dmd_temp_name %d read err\n", __func__, i);
			reset_dmd_temp_info();
			return;
		}
	}

	/* read dtsi ok, set true */
	g_dmd_temp_para_is_read = true;
}
