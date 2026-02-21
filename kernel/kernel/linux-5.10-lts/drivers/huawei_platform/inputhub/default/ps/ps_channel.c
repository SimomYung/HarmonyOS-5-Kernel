/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ps channel source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */
#include "ps_channel.h"
#include "als_channel.h"
#include "als_tp_color.h"
#include <linux/err.h>
#include <linux/mtd/hisi_nve_interface.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <securec.h>

#include "contexthub_route.h"
#include "ps_sysfs.h"

#ifdef CONFIG_PROX_LCDTYPE
static int8_t g_ps_lcd = LCD_OTHER;
static int8_t g_ps_ddic = UNKNOWN_DDIC;

static lcd_t ps_lcd_info[] = {
	{"190", LCD_BOE},
	{"192", LCD_BOE2},
	{"310", LCD_VISIONOX},
	{"290", LCD_HUAXING},
	{"198", LCD_BOE}
};
static lcd_v_t ps_lcd_ver_info[] = {
	{LCD_BOE, "pla", LCD_BOE_418}
};
static ddic_t ps_ddic_info[] = {
	{"h01", DDIC_HISI},
	{"c10", DDIC_RUIDING},
	{"c08", DDIC_RUIDING692H5},
	{"c07", DDIC_RUIDING692H0},
	{"c06", DDIC_RUIDING},
	{"207", DDIC_NOVATEK},
	{"f01", DDIC_YUNYINGGU},
	{"d06", DDIC_JICHUANG},
	{"c12", DDIC_RUIDING},
	{"h03", DDIC_HISI},
	{"h05", DDIC_HISI05}
};

static void get_ps_lcd_ddic()
{
	uint8_t index, lcdlen;
	struct device_node *np = NULL;
	const char *lcd_model = NULL;
	struct als_platform_data *pf_data = NULL;

	pf_data = als_get_platform_data(TAG_ALS);
	np = of_find_compatible_node(NULL, NULL, "huawei,lcd_panel_type");
	if (!np) {
		hwlog_err("als not find lcd_panel_type node\n");
		return;
	}
	if (pf_data->als_phone_type == ALTB) {
		if (of_property_read_string(np, "sec_lcd_panel_type", &lcd_model)) {
			hwlog_err("ps not find sec_lcd_panel_type in dts\n");
			return;
		}
	} else {
		if (of_property_read_string(np, "lcd_panel_type", &lcd_model)) {
			hwlog_err("ps not find lcd_panel_type in dts\n");
			return;
		}
	}

	lcdlen = strnlen(lcd_model, 128) - 3;
	hwlog_info("lcdlen = %d, lcd_panel_type: %s\n", lcdlen, lcd_model);
	for (index = 0; index < ARRAY_SIZE(ps_ddic_info); index++) {
		if (!strncmp((lcd_model + DDIC_IDX), ps_ddic_info[index].lcd_model, strlen(ps_ddic_info[index].lcd_model))) {
			hwlog_info("%s: ddic = %d\n", __func__, ps_ddic_info[index].ddic);
			g_ps_ddic = ps_ddic_info[index].ddic;
			break;
		}
	}

	for (index = 0; index < ARRAY_SIZE(ps_lcd_info); index++) {
		if (!strncmp(lcd_model, ps_lcd_info[index].lcd_model, strlen(ps_lcd_info[index].lcd_model))) {
			hwlog_info("%s: lcd = %d\n", __func__, ps_lcd_info[index].lcd);
			g_ps_lcd = ps_lcd_info[index].lcd;
			break;
		}
	}
	for (index = 0; index < ARRAY_SIZE(ps_lcd_ver_info); index++) {
		if (g_ps_lcd == ps_lcd_ver_info[index].lcd &&
			!strncmp(lcd_model + lcdlen, ps_lcd_ver_info[index].lcd_model, strlen(ps_lcd_ver_info[index].lcd_model))) {
			hwlog_info("%s: lcdv = %d\n", __func__, ps_lcd_ver_info[index].lcdv);
			g_ps_lcd = ps_lcd_ver_info[index].lcdv;
		}
	}
	hwlog_info("%s: g_ps_lcd = %d, g_ps_ddic = %d\n", __func__, g_ps_lcd, g_ps_ddic);
}
#endif

static uint8_t als_get_phone_type(void)
{
	struct als_platform_data *pf_data = als_get_platform_data(TAG_ALS);

	if (pf_data == NULL) {
		hwlog_err("get als pf data null\n");
		return 0;
	}
	return pf_data->als_phone_type;
}

int send_ps_calibrate_data_to_mcu(void)
{
	bool is_normal = false;
	struct ps_device_info *dev_info = NULL;

	dev_info = ps_get_device_info(TAG_PS);
	if (dev_info == NULL)
		return -1;
	if (read_calibrate_data_from_nv(PS_CALIDATA_NV_NUM,
		PS_CALIDATA_NV_SIZE, "PSENSOR"))
		return -1;

	dev_info->ps_first_start_flag = 1;

	if (memcpy_s(ps_sensor_offset, sizeof(ps_sensor_offset),
		user_info.nv_data, sizeof(ps_sensor_offset)) != EOK)
		return -1;

	if (als_get_phone_type() == ALTB && ps_sensor_offset[0] != 0 &&
		(dev_info->chip_type == PS_CHIP_TCS3718 || dev_info->chip_type == PS_CHIP_SIP3623)) {
		if (dev_info->chip_type == PS_CHIP_TCS3718 &&
			ps_sensor_offset[0] > TCS3718_XTALK_THRE_L && ps_sensor_offset[0] < TCS3718_XTALK_THRE_H) {
			is_normal = true;
		} else if (dev_info->chip_type == PS_CHIP_SIP3623 &&
			ps_sensor_offset[0] > SIP3623_XTALK_THRE_L && ps_sensor_offset[0] < SIP3623_XTALK_THRE_H) {
			is_normal = true;
		}

		hwlog_info("%s ps offset is_normal:%d.\n", __func__, is_normal);
		if (!is_normal) {
			if (memset_s(&ps_sensor_offset, sizeof(ps_sensor_offset), 0, sizeof(ps_sensor_offset)) != EOK) {
				hwlog_err("%s memset_s ps_sensor_offset error\n", __func__);
			}
		}
	}

#ifdef CONFIG_PROX_LCDTYPE
	get_ps_lcd_ddic();
	ps_sensor_offset[4] = g_ps_lcd;
	ps_sensor_offset[5] = g_ps_ddic;
#endif

	hwlog_info("nve_direct_access read ps_offset %d,%d,%d, %d,%d,%d\n",
		ps_sensor_offset[0], ps_sensor_offset[1], ps_sensor_offset[2],
		ps_sensor_offset[3], ps_sensor_offset[4], ps_sensor_offset[5]);
	if (memcpy_s(&ps_sensor_calibrate_data,
		sizeof(ps_sensor_calibrate_data),
		ps_sensor_offset,
		(sizeof(ps_sensor_calibrate_data) < PS_CALIDATA_NV_SIZE) ?
		sizeof(ps_sensor_calibrate_data) : PS_CALIDATA_NV_SIZE) != EOK)
		return -1;

	if (send_calibrate_data_to_mcu(TAG_PS, SUB_CMD_SET_OFFSET_REQ,
		ps_sensor_offset, PS_CALIDATA_NV_SIZE, false))
		return -1;

	return 0;
}

void reset_ps_calibrate_data(void)
{
	struct ps_device_info *dev_info = NULL;

	dev_info = ps_get_device_info(TAG_PS);
	if (dev_info == NULL)
		return;
	if (dev_info->chip_type != PS_CHIP_NONE ||
		dev_info->ps_cali_supported == 1)
		send_calibrate_data_to_mcu(TAG_PS, SUB_CMD_SET_OFFSET_REQ,
			ps_sensor_calibrate_data, PS_CALIDATA_NV_SIZE, true);
}

int tp_event_to_ultrasonic(unsigned char event)
{
	struct write_info pkg_ap = { 0 };
	pkt_parameter_req_t cpkt;
	struct pkt_header *hd = (struct pkt_header *)&cpkt;
	int ret;

	hwlog_info("%s event =%d\n", __func__, event);

	ret = memset_s(&cpkt, sizeof(cpkt), 0, sizeof(cpkt));
	if (ret != EOK) {
		hwlog_err("send_tp_event_to_ultrasonic memset_s cpkt err\n");
		return -1;
	}
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.tag = TAG_PS;
	cpkt.subcmd = SUB_CMD_PS_TP_EVENT_REQ;
	pkg_ap.wr_buf = &hd[1];
	pkg_ap.wr_len = sizeof(unsigned char) + SUBCMD_LEN;
	ret = memcpy_s(cpkt.para, sizeof(cpkt.para), &event, sizeof(unsigned char));
	if (ret != EOK) {
		hwlog_err("send_tp_event_to_ultrasonic memset_s cpkt err\n");
		return -1;
	}

	ret = write_customize_cmd(&pkg_ap, NULL, false);
	if (ret != 0) {
		hwlog_err("send_tp_event_to_ultrasonic failed ret = %d\n", ret);
		return -1;
	}
	return 0;
}
