/*
 * sr_dmd.c
 *
 * trans tp from cmdline to dmd
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

#include <platform_include/cee/linux/sr_dmd.h>
#include "pm.h"
#include "pub_def.h"
#include <securec.h>
#include <linux/init.h>
#include <linux/string.h>
#include <helper/log/lowpm_log.h>

#define BUFFER_MAX_LENGTH                 (20 * 2)
#define GET_SR_INFO_ERR                   (-1)
#define GET_SR_INFO_STR_ERR               (-2)
#define EARLY_PARSE_PARAMS_ERR            (-1)
#define EARLY_PARSE_SET_ERR               (-2)

static u32 g_pmu_point;
static u32 g_tp_point;

int get_sr_info_for_dmd(const void *bbox_base, size_t size, char *out_buf, size_t buf_size)
{
	int ret;
	char t_buffer[BUFFER_MAX_LENGTH] = {0};
	size_t len;

	(void)bbox_base;
	(void)size;

	if (out_buf == NULL) {
		lp_err("invalid param");
		return GET_SR_INFO_ERR;
	}

	ret = sprintf_s(t_buffer, sizeof(char) * BUFFER_MAX_LENGTH,
					"sr_pmu=%x,sr_sctrl=%x", g_pmu_point, g_tp_point);
	if (ret < 0) {
		lp_err("get sr info format err: %s, ret: %d", t_buffer, ret);
		return GET_SR_INFO_STR_ERR;
	}

	len = strlen(t_buffer);
	if (buf_size < len + 1) {
		lp_err("out_buf size is too small: %u", buf_size);
		return GET_SR_INFO_ERR;
	}

	ret = strcat_s(out_buf, buf_size, t_buffer);
	if (ret != 0) {
		lp_err("get sr info strcat err %s, ret: %d", out_buf, ret);
		return GET_SR_INFO_STR_ERR;
	}
	return 0;
}

int get_last_sr_info(u32 *sr_point, u32 *sr_pmu)
{
	if (sr_point != NULL)
		*sr_point = g_tp_point;

	if (sr_pmu != NULL)
		*sr_pmu = g_pmu_point;
	return 0;
}

static int early_parse_get_info(char *p, u32 *point, char *src_key)
{
	int ret;
	int sscanf_success = 1;

	if (p == NULL || *p == '\0' || point == NULL || src_key == NULL) {
		lp_err("%s info: pointer of cmdline is NULL", src_key);
		return EARLY_PARSE_PARAMS_ERR;
	}

	ret = sscanf_s(p, "%x", point);
	if (ret != sscanf_success) {
		lp_err("copy %s from cmdline fail: %d", src_key, ret);
		return EARLY_PARSE_SET_ERR;
	}

	lp_info("cmdline is %s, %s is %x", p, src_key, *point);
	return 0;
}

static int __init early_parse_get_sr_info(char *p)
{
	char *src_key = "sr_point";

	return early_parse_get_info(p, &g_tp_point, src_key);
}
early_param("sr_sctrl", early_parse_get_sr_info);

static int __init early_parse_get_pmu_info(char *p)
{
	char *src_key = "sr_pmu";

	return early_parse_get_info(p, &g_pmu_point, src_key);
}
early_param("sr_pmu", early_parse_get_pmu_info);
