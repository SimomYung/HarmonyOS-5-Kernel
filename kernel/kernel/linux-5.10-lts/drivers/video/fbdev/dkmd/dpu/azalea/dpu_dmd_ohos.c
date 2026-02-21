/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/hrtimer.h>
#include <linux/err.h>
#include <securec.h>
#include "dpu_fb_debug.h"
#include "dpu_fb_def.h"

// Converting error_no to character strings in the OHOS.
typedef struct error_no_map
{
	int32_t error_no;
	char *name;
	int32_t running_test_switch;
} error_no_map;

// Define local maps to save memory and reduce code modification conflicts.
static struct error_no_map lcd_error_no_map[] = {
	{922001000, "DSM_LCD_LDI_UNDERFLOW_NO", 0},
	{922001001, "DSM_LCD_TE_TIME_OUT_ERROR_NO", 1},
	{922001002, "DSM_LCD_STATUS_ERROR_NO", 1},
	{922001003, "DSM_LCD_POWER_STATUS_ERROR_NO", 0},
	{922001008, "DSM_LCD_OVP_ERROR_NO", 1},
	{922001018, "DSM_LCD_ESD_STATUS_ERROR_NO", 1},
	{922001033, "DSM_LCD_BTB_CHECK_ERROR_NO", 1},
	{922001034, "DSM_LCD_BACKLIGHT_OCP_ERROR_NO", 1},
	{922001035, "DSM_LCD_BACKLIGHT_TSD_ERROR_NO", 1},
	{922001036, "DSM_LCD_MIPI_TRANSMIT_ERROR_NO", 1},
	{922001038, "DSM_LCD_VACTIVE_TIMEOUT_ERROR_NO", 1},
	{922001040, "DSM_LCD_BACKLIGHT_I2C_ERROR_NO", 1},
	{922001041, "DSM_LCD_BIAS_I2C_ERROR_NO", 1},
	{922001042, "DSM_LCD_BL_FLICKER_ERROR_NO", 0},
	{922001501, "DSM_LCD_PANEL_CRACK_ERROR_NO", 1},
	{922001502, "DSM_LCD_CHECKSUM_ERROR_NO", 1},
	{922001503, "DSM_LCD_POWER_ABNOMAL_ERROR_NO", 1},
	{922001504, "DSM_LCD_DDIC_LV_DETECT_ERROR_NO", 0},
	{922001505, "DSM_AMOLED_POWER_IC_CHECK_ERR_NO", 0},
	{922001506, "DSM_LCD_UNDERRUN_ERROR_NO", 0},
	{922001508, "DSM_LCD_LVD_DETECT_ERROR_NO", 1},
	{922001509, "DSM_LCD_MBIST_ERROR_NO", 1},
	{922001510, "DSM_LCD_MIPI_CHECK_ERROR_NO", 1},
	{922001511, "DSM_LCD_SUB_PANEL_CRACK_ERROR_NO", 1},
	{922001512, "DSM_LCD_SUB_CHECKSUM_ERROR_NO", 1},
	{922001513, "DSM_LCD_SUB_LVD_DETECT_ERROR_NO", 1},
	{922001514, "DSM_LCD_SUB_MBIST_ERROR_NO", 1},
	{922001515, "DSM_LCD_SUB_MIPI_CHECK_ERROR_NO", 1},
	{925004311, "DSM_DSI_DETECT_ERROR_NO", 0},
};

static int32_t lcd_errorno_to_str(int32_t error_no, char *str, int32_t buff_len) {
	size_t i;
	int32_t ret;
	size_t lcd_error_no_map_len = sizeof(lcd_error_no_map) / sizeof(error_no_map);

	DPU_FB_INFO("error_no=%d, buff_len=%d\n", error_no, buff_len);

	if (buff_len <= 1) {
		DPU_FB_ERR("invalid buff_len while converting lcd error number to string");
		return -1;
	}

	for (i = 0; i < lcd_error_no_map_len; i++) {
		if (lcd_error_no_map[i].error_no == error_no) {
			/* buff_len is 33 */
			if (strlen(lcd_error_no_map[i].name) >= buff_len) {
				DPU_FB_ERR("buff len error");
				return -1;
			}

			ret = strncpy_s(str, buff_len, lcd_error_no_map[i].name, buff_len - 1);
			if (ret != EOK) {
				DPU_FB_ERR("strncpy_s error");
				return -1;
			}
			return 0;
		}
	}
	return -1;
}

static int lcd_errorno_get_running_test_switch(int error_no)
{
	size_t i;
	size_t lcd_error_no_map_len = sizeof(lcd_error_no_map) / sizeof(error_no_map);

	for (i = 0; i < lcd_error_no_map_len; i++) {
		if (lcd_error_no_map[i].error_no == error_no) {
			return lcd_error_no_map[i].running_test_switch;
		}
	}

	return 0;
}

struct dsm_client_ops hlcd_dsm_ops = {
	.poll_state = NULL,
	.dump_func = NULL,
	.errorno_to_str = lcd_errorno_to_str,
	.errorno_get_running_test_switch = lcd_errorno_get_running_test_switch,
};

static struct dsm_dev dsm_lcd_dev = {
	.name = "LCD",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = &hlcd_dsm_ops,
	.buff_size = 5120,
};

struct dsm_client *dsm_lcd_client;

struct dsm_client* dkmd_get_dmd_client(void)
{
	return dsm_lcd_client;
}

void dksm_dmd_register(const char *lcd_name)
{
	DPU_FB_INFO("+");

	if (!lcd_name) {
		DPU_FB_WARNING("lcd_name is null\n");
		lcd_name = "unknown_panel";
	}

	dsm_lcd_dev.module_name = lcd_name;
	dsm_lcd_client = dsm_register_client(&dsm_lcd_dev);
	if (dsm_lcd_client)
		DPU_FB_INFO("regist dmd client succ, module_name=%s\n", lcd_name);
	else
		DPU_FB_WARNING("regist dmd client fail, module_name=%s\n", lcd_name);
}
