/*
 * lcd_kit_factory_hs.c
 *
 * lcdkit hisi factory function for lcd driver
 *
 * Copyright (c) 2019-2019 Huawei Technologies Co., Ltd.
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
#include "lcd_kit_factory.h"
#include "lcd_kit_power.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_parse.h"
#include "hisi_fb.h"
#include "lcd_kit_common.h"
#include "lcd_kit_ext_disp.h"
#include "lcd_kit_factory_utils.h"
#include <linux/jiffies.h>
#if defined(CONFIG_LPM_HKADC)
#include <platform_include/cee/linux/adc.h>
#endif

static ssize_t lcd_inversion_mode_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return lcd_kit_inversion_get_mode(LCD_ACTIVE_PANEL, buf);
}

static ssize_t lcd_inversion_mode_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long val;
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("lcd inversion mode store buf NULL Pointer!\n");
		return LCD_KIT_FAIL;
	}
	val = simple_strtoul(buf, NULL, 0);
	down(&hisifd->blank_sem);
	if (hisifd->panel_power_on) {
		hisifb_activate_vsync(hisifd);
		ret = lcd_kit_inversion_set_mode(LCD_ACTIVE_PANEL, hisifd, val);
		if (ret)
			LCD_KIT_ERR("inversion mode set failed\n");
		hisifb_deactivate_vsync(hisifd);
	}
	up(&hisifd->blank_sem);
	return count;
}

static ssize_t lcd_scan_mode_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return lcd_kit_scan_get_mode(LCD_ACTIVE_PANEL, buf);
}

static ssize_t lcd_scan_mode_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long val;
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("lcd scan mode store buf NULL Pointer!\n");
		return LCD_KIT_FAIL;
	}
	val = simple_strtoul(buf, NULL, 0);
	down(&hisifd->blank_sem);
	if (hisifd->panel_power_on) {
		hisifb_activate_vsync(hisifd);
		ret = lcd_kit_scan_set_mode(LCD_ACTIVE_PANEL, hisifd, val);
		if (ret)
			LCD_KIT_ERR("scan mode set failed\n");
		hisifb_deactivate_vsync(hisifd);
	}
	up(&hisifd->blank_sem);
	return count;
}

static ssize_t lcd_check_reg_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_panel_info *pinfo = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo = &(hisifd->panel_info);
	down(&hisifd->blank_sem);
	if (hisifd->panel_power_on) {
		if (pinfo->fps_updt != pinfo->fps && pinfo->fps_updt != 0) {
			ret = snprintf(buf, PAGE_SIZE, "OK\n");
			LCD_KIT_INFO("check_reg result:%s\n", buf);
			up(&hisifd->blank_sem);
			return ret;
		}
		hisifb_activate_vsync(hisifd);
		ret = lcd_kit_check_reg(LCD_ACTIVE_PANEL, hisifd, buf);
		hisifb_deactivate_vsync(hisifd);
	}
	up(&hisifd->blank_sem);
	return ret;
}

static ssize_t lcd_gram_check_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	int checksum_result;
	struct hisi_fb_data_type *hisifd = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (FACT_INFO->checksum.support) {
		down(&hisifd->blank_sem);
		if (hisifd->panel_power_on) {
			hisifb_activate_vsync(hisifd);
			checksum_result = lcd_kit_checksum_check(hisifd);
			hisifb_deactivate_vsync(hisifd);
			ret = snprintf(buf, PAGE_SIZE, "%d\n", checksum_result);
		}
		up(&hisifd->blank_sem);
		/* disable checksum stress test, restore mipi clk and vdd */
		if (FACT_INFO->checksum.status == LCD_KIT_CHECKSUM_END) {
			checksum_stress_disable(hisifd);
			if (FACT_INFO->checksum.clear_sram) {
				lcd_kit_recovery_display(hisifd);
				LCD_KIT_INFO("checksum recovery display\n");
			}
		}
	}
	return ret;
}

static ssize_t lcd_gram_check_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;
	unsigned long val = 0;
	int index;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	ret = kstrtoul(buf, 10, &val);
	if (ret)
		return ret;
	LCD_KIT_INFO("val=%ld\n", val);
	if (FACT_INFO->checksum.support) {
		/* enable checksum stress test, promote mipi clk and vdd */
		if (FACT_INFO->checksum.status == LCD_KIT_CHECKSUM_END)
			checksum_stress_enable(hisifd);
		down(&hisifd->blank_sem);
		if (hisifd->panel_power_on) {
			hisifb_activate_vsync(hisifd);
			index = val - INDEX_START;
			ret = lcd_kit_checksum_set(hisifd, index);
			hisifb_deactivate_vsync(hisifd);
		}
		up(&hisifd->blank_sem);
	}
	return count;
}

static ssize_t lcd_sleep_ctrl_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return lcd_kit_get_sleep_mode(LCD_ACTIVE_PANEL, buf);
}

static ssize_t lcd_sleep_ctrl_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned long val = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	ret = kstrtoul(buf, 0, &val);
	if (ret) {
		LCD_KIT_ERR("invalid parameter!\n");
		return ret;
	}
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is power off!\n");
		return LCD_KIT_FAIL;
	}
	ret = lcd_kit_set_sleep_mode(LCD_ACTIVE_PANEL, val);
	if (ret)
		LCD_KIT_ERR("set sleep mode fail!\n");
	return count;
}

static ssize_t lcd_poweric_det_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	ssize_t ret = LCD_KIT_OK;
	int i;
	char str[LCD_PMIC_LENGTH_MAX] = {0};
	char tmp[LCD_PMIC_LENGTH_MAX] = {0};
	struct hisi_fb_data_type *hisifd = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd)
		return LCD_KIT_FAIL;

	if (!hisifd->panel_info.poweric_num_length ||
		!FACT_INFO->poweric_detect.support) {
		ret += snprintf(tmp, sizeof(tmp), "poweric%d:%d", 1, 1);
		strncat(str, tmp, strlen(tmp));
		LCD_KIT_INFO("poweric_detect not support, default pass\n");
	} else {
		LCD_KIT_INFO("poweric num = %d\n", hisifd->panel_info.poweric_num_length);
		for (i = 0; i < hisifd->panel_info.poweric_num_length; i++) {
			LCD_KIT_INFO("i = %d, poweric_status = %d\n",
				i, hisifd->panel_info.poweric_status[i]);
			ret += snprintf(tmp, sizeof(tmp), "poweric%d:%d",
				(i + 1), hisifd->panel_info.poweric_status[i]);
			strncat(str, tmp, strlen(tmp));
			if (i != hisifd->panel_info.poweric_num_length - 1)
				strncat(str, "\r\n", strlen("\r\n"));
		}
	}
	LCD_KIT_INFO("%s\n", str);
	ret = snprintf(buf, PAGE_SIZE, "%s\n", str);
	return ret;
}

static ssize_t lcd_poweric_det_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisi_fb_panel_data *pdata = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_OK;
	}
	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (!pdata) {
		LCD_KIT_ERR("lcd poweric det store pdata NULL Pointer\n");
		return LCD_KIT_OK;
	}
	if (!buf) {
		LCD_KIT_ERR("lcd poweric det store buf NULL Pointer\n");
		return LCD_KIT_OK;
	}
	hisifd->panel_info.poweric_num_length = 0;
	if (FACT_INFO->poweric_detect.support) {
		LCD_KIT_INFO("poweric detect is support\n");
		poweric_gpio_detect_dbc(hisifd);
	} else {
		LCD_KIT_ERR("poweric detect is not support\n");
	}
	return count;
}

static ssize_t lcd_amoled_pcd_errflag_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = PCD_ERRFLAG_SUCCESS;

	if (buf == NULL) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	if (disp_info->pcd_errflag.pcd_support ||
		disp_info->pcd_errflag.errflag_support)
		ret = lcd_amoled_cmds_pcd_errflag(dev, attr, buf);
	else
		ret = lcd_amoled_gpio_pcd_errflag(dev, attr, buf);
	return ret;
}

static ssize_t lcd_amoled_pcd_errflag_store(struct device *dev,
	struct device_attribute *attr, const char *buf)
{
	int ret;
	unsigned long val = 0;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	ret = kstrtoul(buf, 0, &val);
	if (ret) {
		LCD_KIT_ERR("invalid parameter\n");
		return ret;
	}
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
	return lcd_kit_pcd_detect(hisifd, val);
}

static ssize_t lcd_test_config_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	ret = lcd_kit_get_test_config(LCD_ACTIVE_PANEL, buf);
	if (ret < 0) {
		LCD_KIT_ERR("not find test item\n");
		return ret;
	}
	down(&hisifd->blank_sem);
	if (hisifd->panel_power_on) {
		if (buf) {
			if (!strncmp(buf, "OVER_CURRENT_DETECTION", strlen("OVER_CURRENT_DETECTION"))) {
				hisifb_activate_vsync(hisifd);
				ret = lcd_kit_current_det(hisifd);
				hisifb_deactivate_vsync(hisifd);
				up(&hisifd->blank_sem);
				return snprintf(buf, PAGE_SIZE, "%d", ret);
			}
			if (!strncmp(buf, "OVER_VOLTAGE_DETECTION", strlen("OVER_VOLTAGE_DETECTION"))) {
				hisifb_activate_vsync(hisifd);
				ret = lcd_kit_lv_det(hisifd);
				hisifb_deactivate_vsync(hisifd);
				up(&hisifd->blank_sem);
				return snprintf(buf, PAGE_SIZE, "%d", ret);
			}
		}
	}
	up(&hisifd->blank_sem);
	return ret;
}

static ssize_t lcd_test_config_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	if (lcd_kit_set_test_config(LCD_ACTIVE_PANEL, buf) < 0)
		LCD_KIT_ERR("set_test_config failed\n");
	return count;
}

static ssize_t lcd_lv_detect_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	ret = lcd_kit_lv_det(hisifd);
	return snprintf(buf, PAGE_SIZE, "%d", ret);
}

static ssize_t lcd_current_detect_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	ret = lcd_kit_current_det(hisifd);
	return snprintf(buf, PAGE_SIZE, "%d", ret);
}

static ssize_t lcd_ldo_check_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int i;
	int j;
	int cur_val = -1; // init invalid value
	int sum_current;
	int len = sizeof(struct lcd_ldo);
	int temp_max_value;
	struct hisi_fb_data_type *hisifd = NULL;
	struct lcd_ldo ldo_result = {0};
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!FACT_INFO->ldo_check.support) {
		LCD_KIT_INFO("ldo check not support\n");
		return len;
	}
	down(&hisifd->blank_sem);
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is power off\n");
		up(&hisifd->blank_sem);
		return LCD_KIT_FAIL;
	}
	for (i = 0; i < FACT_INFO->ldo_check.ldo_num; i++) {
		sum_current = 0;
		temp_max_value = 0;
		for (j = 0; j < LDO_CHECK_COUNT; j++) {
#if defined(CONFIG_LPM_HKADC)
			cur_val = lpm_adc_get_current(
				FACT_INFO->ldo_check.ldo_channel[i]);
#endif
			if (cur_val < 0) {
				sum_current = -1;
				break;
			}
			sum_current = sum_current + cur_val;
			if (temp_max_value < cur_val)
				temp_max_value = cur_val;
		}
		if (sum_current == -1)
			FACT_INFO->ldo_check.ldo_current[i] = -1;
		else
			FACT_INFO->ldo_check.ldo_current[i] =
				(sum_current - temp_max_value) / (LDO_CHECK_COUNT - 1);
	}
	up(&hisifd->blank_sem);
	ldo_transform(&ldo_result);
	memcpy(buf, &ldo_result, (u32)len);
	LCD_KIT_INFO("ldo check finished\n");
	return len;
}

static ssize_t lcd_general_test_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	struct hisi_fb_data_type *hisifd = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (FACT_INFO->hor_line.support)
		ret = lcd_hor_line_test(hisifd);

	if (ret == 0)
		ret = snprintf(buf, PAGE_SIZE, "OK\n");
	else
		ret = snprintf(buf, PAGE_SIZE, "FAIL\n");

	return ret;
}

static ssize_t lcd_vertical_line_test_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!FACT_INFO->vtc_line.support) {
		ret = snprintf(buf, PAGE_SIZE, "NOT_SUPPORT\n");
		return ret;
	}
	ret = snprintf(buf, PAGE_SIZE, "OK\n");
	return ret;
}

static ssize_t lcd_vertical_line_test_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned long index = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	if (dev == NULL) {
		LCD_KIT_ERR("NULL Pointer!\n");
		return LCD_KIT_FAIL;
	}
	if (buf == NULL) {
		LCD_KIT_ERR("NULL Pointer!\n");
		return LCD_KIT_FAIL;
	}
	hisifd = lcd_kit_get_hisifd(dev);
	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	ret = kstrtoul(buf, 10, &index);
	if (ret) {
		LCD_KIT_ERR("kstrtoul fail!\n");
		return ret;
	}
	LCD_KIT_INFO("index=%ld\n", index);
	if (FACT_INFO->vtc_line.support) {
		ret = lcd_vtc_line_test(hisifd, index);
		if (ret)
			LCD_KIT_ERR("vtc line test fail\n");
	}
	return count;
}

static ssize_t lcd_bl_self_test_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_bl_ops *bl_ops = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	bl_ops = lcd_kit_get_bl_ops();
	if (!bl_ops) {
		LCD_KIT_ERR("bl_ops is NULL!\n");
		return LCD_KIT_FAIL;
	}

	if (FACT_INFO->bl_open_short_support) {
		if (bl_ops->bl_self_test)
			ret = bl_ops->bl_self_test();
	}
	return snprintf(buf, PAGE_SIZE, "%d\n", ret);
}

static ssize_t lcd_hkadc_debug_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	if (FACT_INFO->hkadc.support)
		ret = snprintf(buf, PAGE_SIZE, "%d\n", FACT_INFO->hkadc.value);

	return ret;
}

static ssize_t lcd_hkadc_debug_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	int channel = 0;
	int panel_id = LCD_ACTIVE_PANEL;

	if (buf == NULL) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	if (FACT_INFO->hkadc.support) {
		ret = sscanf(buf, "%u", &channel);
		if (ret) {
			LCD_KIT_ERR("ivalid parameter!\n");
			return ret;
		}
#if defined(CONFIG_LPM_HKADC)
		FACT_INFO->hkadc.value = lpm_adc_get_value(channel);
#endif
	}
	return count;
}

static ssize_t lcd_avdd_detect_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int gpio_avdd_ret;
	ssize_t ret;
	int vb_average;
	int panel_id = LCD_ACTIVE_PANEL;

	if (buf == NULL) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}

	if (!FACT_INFO->avdd_detect.support) {
		LCD_KIT_ERR("avdd detect is not support\n");
		ret = snprintf(buf, PAGE_SIZE, "%d\n", 0);
		LCD_KIT_ERR("ret is %d\n", ret);
		return LCD_KIT_OK;
	}

	if (FACT_INFO->avdd_detect.gpio_grp_ctrl) {
		gpio_avdd_ret = lcd_get_avdd_gpio_grp_det_result(buf);
		return snprintf(buf, PAGE_SIZE, "%d\n", gpio_avdd_ret);
	}

	vb_average = process_vb_value();
#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)) && defined(CONFIG_HUAWEI_DATA_ACQUISITION)
	lcd_avdd_detect_data_report(vb_average);
#endif
	if (vb_average < FACT_INFO->avdd_detect.low_threshold ||
		vb_average > FACT_INFO->avdd_detect.high_threshold) {
		LCD_KIT_ERR("avdd exceeds the range, value = %d\n", vb_average);
		/* 1 is returned upon failure */
		ret = snprintf(buf, PAGE_SIZE, "%d\n", 1);
		return ret;
	}
	LCD_KIT_INFO("vb_average = %d, avdd detect success\n", vb_average);
	/* 0 is returned upon success */
	ret = snprintf(buf, PAGE_SIZE, "%d\n", 0);
	return ret;
}

static ssize_t lcd_oneside_mode_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	if (FACT_INFO->oneside_mode.support ||
		FACT_INFO->color_aod_det.support)
		ret = snprintf(buf, PAGE_SIZE, "%d\n",
			FACT_INFO->oneside_mode.mode);

	return ret;
}

static ssize_t lcd_oneside_mode_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	ssize_t ret;
	struct hisi_fb_data_type *hisifd = NULL;
	unsigned int mode = 0;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("oneside setting store buf null!\n");
		return LCD_KIT_FAIL;
	}
	if (strlen(buf) >= MAX_BUF) {
		LCD_KIT_ERR("buf overflow!\n");
		return LCD_KIT_FAIL;
	}
	ret = sscanf(buf, "%u", &mode);
	if (!ret) {
		LCD_KIT_ERR("sscanf return invaild:%zd\n", ret);
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("oneside set %d\n", mode);
	if (FACT_INFO->oneside_mode.support)
		lcd_kit_oneside_setting(hisifd, mode);
	if (FACT_INFO->color_aod_det.support)
		lcd_kit_aod_detect_setting(hisifd, mode);
	return count;
}

#define LCD_REG_LENG_MAX 1000
#define MAX_BUF 60

static ssize_t lcd_fps_cmd_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;
	char str[LCD_REG_LENG_MAX] = {0};
	int panel_id = LCD_ACTIVE_PANEL;

	if (disp_info->fps.support)
		ret = snprintf(str, sizeof(str),
			"FPS60:%s;FPS90:%s;FPS1:%s;FPS45:%s;FPS40:%s;FPS30:%s;FPS24:%s;FPS20:%s;FPS18:%s;FPS15:%s;FPS10:%s;FPS120:%s;",
			disp_info->fps.fps_60_cmd, disp_info->fps.fps_90_cmd,
			disp_info->fps.fps_1_cmd, disp_info->fps.fps_45_cmd,
			disp_info->fps.fps_40_cmd, disp_info->fps.fps_30_cmd,
			disp_info->fps.fps_24_cmd, disp_info->fps.fps_20_cmd,
			disp_info->fps.fps_18_cmd, disp_info->fps.fps_15_cmd,
			disp_info->fps.fps_10_cmd, disp_info->fps.fps_120_cmd);
	ret = snprintf(buf, PAGE_SIZE, "%s\n", str);
	LCD_KIT_INFO("%s\n", str);
	return ret;
}

static ssize_t lcd_fps_cmd_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int val = 0;
	int ret;
	int panel_id = LCD_ACTIVE_PANEL;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (!hisifd)
		return LCD_KIT_FAIL;
	if (!buf) {
		LCD_KIT_ERR("buf is null!\n");
		return LCD_KIT_FAIL;
	}
	if (strlen(buf) >= MAX_BUF) {
		LCD_KIT_ERR("buf overflow!\n");
		return LCD_KIT_FAIL;
	}
	ret = kstrtouint(buf, 10, &val);
	if (ret) {
		LCD_KIT_ERR("invalid parameter!\n");
		return ret;
	}
	LCD_KIT_INFO("val is %u\n", val);
	if (hisifd->panel_power_on)
		send_fps_lock_cmd(panel_id, hisifd, val);
	return count;
}

static ssize_t lcd_check_sum2_detect_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int result;
	ssize_t ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;
	struct hisi_fb_data_type *hisifd = NULL;

	LCD_KIT_INFO("enter \n");
	hisifd = dev_get_hisifd(dev);
	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
	if (FACT_INFO->check_sum2_detect.support) {
		lcd_check_sum2_detect_set(panel_id, hisifd,
			FACT_INFO->check_sum2_detect.loop_index);
		result = lcd_check_sum2_detect(panel_id, hisifd);
		ret = snprintf(buf, PAGE_SIZE, "%d\n", result);
	}
	/* disable esd */
	lcd_esd_enable(hisifd, 1);
	return ret;
}

static ssize_t lcd_check_sum2_detect_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	ssize_t ret = LCD_KIT_OK;
	unsigned int index = 0;
	struct hisi_fb_data_type *hisifd = NULL;
	int panel_id = LCD_ACTIVE_PANEL;

	hisifd = dev_get_hisifd(dev);
	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("enter\n");
	if (!FACT_INFO->check_sum2_detect.support) {
		LCD_KIT_ERR("check sum2 detect not support\n");
		return LCD_KIT_FAIL;
	}
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
	if (!dev) {
		LCD_KIT_ERR("dev is null\n");
		return LCD_KIT_FAIL;
	}
	if (!buf) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	/* disable esd */
	lcd_esd_enable(hisifd, 0);
	/* decimal  parsing */
	ret = kstrtouint(buf, 10, &index);
	if (ret) {
		LCD_KIT_ERR("str to int fail\n");
		return ret;
	}
	LCD_KIT_INFO("pic index = %u\n", index);
	if (index == 0) {
		FACT_INFO->check_sum2_detect.loop_index = INVALID_INDEX;
		return LCD_KIT_FAIL;
	}
	FACT_INFO->check_sum2_detect.loop_index = index - MIPI_DETECT_BASE;
	return ret;
}

static ssize_t lcd_mbist_detect_test_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int result;
	ssize_t ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!dev) {
		LCD_KIT_ERR("buf or dev is null\n");
		return LCD_KIT_FAIL;
	}
	hisifd = dev_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (FACT_INFO->mbist_detect.support) {
		hisifb_activate_vsync(hisifd);
		if (hisifd->panel_power_on) {
			result = lcd_mbist_detect_test(panel_id, hisifd);
			ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", result);
		}
		hisifb_deactivate_vsync(hisifd);
	}
	/* enable esd */
	lcd_esd_enable(hisifd, ENABLE);
	return ret;
}

static ssize_t lcd_mbist_detect_test_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	ssize_t ret = LCD_KIT_OK;
	unsigned int index = 0;
	int panel_id = LCD_ACTIVE_PANEL;
	struct hisi_fb_data_type *hisifd = NULL;

	if (!buf || !dev) {
		LCD_KIT_ERR("buf or dev is null\n");
		return LCD_KIT_FAIL;
	}
	hisifd = dev_get_hisifd(dev);
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	/* disable esd */
	lcd_esd_enable(hisifd, DISABLE);
	ret = kstrtouint(buf, 10, &index);
	if (ret) {
		LCD_KIT_ERR("invalid parameter\n");
		return ret;
	}

	LCD_KIT_INFO("picture index = %u\n", index);
	if (hisifd->panel_power_on)
		ret = lcd_mbist_detect_set(panel_id, hisifd, index);

	return ret;
}

struct lcd_fact_ops g_fact_ops = {
	.inversion_mode_show = lcd_inversion_mode_show,
	.inversion_mode_store = lcd_inversion_mode_store,
	.scan_mode_show = lcd_scan_mode_show,
	.scan_mode_store = lcd_scan_mode_store,
	.check_reg_show = lcd_check_reg_show,
	.gram_check_show = lcd_gram_check_show,
	.gram_check_store = lcd_gram_check_store,
	.sleep_ctrl_show = lcd_sleep_ctrl_show,
	.sleep_ctrl_store = lcd_sleep_ctrl_store,
	.poweric_det_show = lcd_poweric_det_show,
	.poweric_det_store = lcd_poweric_det_store,
	.pcd_errflag_check_show = lcd_amoled_pcd_errflag_show,
	.pcd_errflag_check_store = lcd_amoled_pcd_errflag_store,
	.test_config_show = lcd_test_config_show,
	.test_config_store = lcd_test_config_store,
	.lv_detect_show = lcd_lv_detect_show,
	.current_detect_show = lcd_current_detect_show,
	.ldo_check_show = lcd_ldo_check_show,
	.general_test_show = lcd_general_test_show,
	.vtc_line_test_show = lcd_vertical_line_test_show,
	.vtc_line_test_store = lcd_vertical_line_test_store,
	.hkadc_debug_show = lcd_hkadc_debug_show,
	.hkadc_debug_store = lcd_hkadc_debug_store,
	.avdd_detect_show = lcd_avdd_detect_show,
	.oneside_mode_show = lcd_oneside_mode_show,
	.oneside_mode_store = lcd_oneside_mode_store,
	.fps_cmd_show = lcd_fps_cmd_show,
	.fps_cmd_store = lcd_fps_cmd_store,
	.check_sum2_detect_show = lcd_check_sum2_detect_show,
	.check_sum2_detect_store = lcd_check_sum2_detect_store,
	.mbist_detect_show = lcd_mbist_detect_test_show,
	.mbist_detect_store = lcd_mbist_detect_test_store,
};

static void parse_dt_poweric_det(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* poweric detect */
	lcd_kit_parse_u32(np, "lcd-kit,poweric-detect-support",
		&FACT_INFO->poweric_detect.support, 0);
	if (FACT_INFO->poweric_detect.support) {
		lcd_kit_parse_u32(np, "lcd-kit,poweric-number",
			&FACT_INFO->poweric_detect.poweric_num, 0);
		lcd_kit_parse_u32(np, "lcd-kit,poweric-detect-value",
			&FACT_INFO->poweric_detect.detect_val, 0);
		lcd_kit_parse_array_data(np, "lcd-kit,poweric-gpio-enable-list",
			&FACT_INFO->poweric_detect.gpio_list);
		lcd_kit_parse_array_data(np, "lcd-kit,poweric-gpio-enable-val",
			&FACT_INFO->poweric_detect.gpio_val);
		lcd_kit_parse_array_data(np, "lcd-kit,poweric-gpio-enable-number-list",
			&FACT_INFO->poweric_detect.gpio_num_list);
		FACT_INFO->poweric_detect.detect_gpio =
			power_hdl->lcd_elvdd_gpio.buf[0];
	}
}

static void parse_dt_mbist_detect(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* mbist detect test */
	lcd_kit_parse_u32(np, "lcd-kit,mbist-det-support",
		&FACT_INFO->mbist_detect.support, 0);
	if (FACT_INFO->mbist_detect.support) {
		FACT_INFO->mbist_detect.pic_index = INVALID_INDEX;
		lcd_kit_parse_u32(np, "lcd-kit,mbist-detect-threshold",
			&FACT_INFO->mbist_detect.threshold, 0);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-rd1-cmds",
			"lcd-kit,mbist-det-rd1-cmds-state",
			&FACT_INFO->mbist_detect.rd_cmds[DET1_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-rd2-cmds",
			"lcd-kit,mbist-det-rd2-cmds-state",
			&FACT_INFO->mbist_detect.rd_cmds[DET2_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-rd3-cmds",
			"lcd-kit,mbist-det-rd3-cmds-state",
			&FACT_INFO->mbist_detect.rd_cmds[DET3_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-rd4-cmds",
			"lcd-kit,mbist-det-rd4-cmds-state",
			&FACT_INFO->mbist_detect.rd_cmds[DET4_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-enter1-cmds",
			"lcd-kit,mbist-det-enter1-cmds-state",
			&FACT_INFO->mbist_detect.enter_cmds[DET1_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-enter2-cmds",
			"lcd-kit,mbist-det-enter2-cmds-state",
			&FACT_INFO->mbist_detect.enter_cmds[DET2_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-enter3-cmds",
			"lcd-kit,mbist-det-enter3-cmds-state",
			&FACT_INFO->mbist_detect.enter_cmds[DET3_INDEX]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,mbist-det-enter4-cmds",
			"lcd-kit,mbist-det-enter4-cmds-state",
			&FACT_INFO->mbist_detect.enter_cmds[DET4_INDEX]);
		lcd_kit_parse_array_data(np, "lcd-kit,mbist-det-value1",
			&FACT_INFO->mbist_detect.value[DET1_INDEX]);
		lcd_kit_parse_array_data(np, "lcd-kit,mbist-det-value2",
			&FACT_INFO->mbist_detect.value[DET2_INDEX]);
		lcd_kit_parse_array_data(np, "lcd-kit,mbist-det-value3",
			&FACT_INFO->mbist_detect.value[DET3_INDEX]);
		lcd_kit_parse_array_data(np, "lcd-kit,mbist-det-value4",
			&FACT_INFO->mbist_detect.value[DET4_INDEX]);
		lcd_kit_parse_u32(np, "lcd-kit,mbist-det-dual-dsi-support",
			&FACT_INFO->mbist_detect.dual_dsi_support, 0);
	}
}

static void parse_dt_pcd_errflag(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* pcd errflag check */
	lcd_kit_parse_u32(np, "lcd-kit,pcd-errflag-check-support",
		&FACT_INFO->pcd_errflag_check.pcd_errflag_check_support, 0);
	if (FACT_INFO->pcd_errflag_check.pcd_errflag_check_support) {
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,pcd-detect-open-cmds",
			"lcd-kit,pcd-read-cmds-state",
			&FACT_INFO->pcd_errflag_check.pcd_detect_open_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,pcd-detect-close-cmds",
			"lcd-kit,pcd-read-cmds-state",
			&FACT_INFO->pcd_errflag_check.pcd_detect_close_cmds);
	}
}

static void parse_dt_check_sum2_detect(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* check sum2 detect */
	lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-support",
		&FACT_INFO->check_sum2_detect.support, 0);

	if (FACT_INFO->check_sum2_detect.support) {
		FACT_INFO->check_sum2_detect.loop_index = INVALID_INDEX;
		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-threshold",
			&FACT_INFO->check_sum2_detect.threshold, 0);
		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-delay",
			&FACT_INFO->check_sum2_detect.delay, 0);

		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-time-index1",
			&FACT_INFO->check_sum2_detect.time[TEST_LOOP_0], 0);
		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-time-index2",
			&FACT_INFO->check_sum2_detect.time[TEST_LOOP_1], 0);
		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-time-index3",
			&FACT_INFO->check_sum2_detect.time[TEST_LOOP_2], 0);

		lcd_kit_parse_dcs_cmds(np, "lcd-kit,check-sum2-detect-enter1-cmds",
			"lcd-kit,check-sum2-detect-enter1-cmds-state",
			&FACT_INFO->check_sum2_detect.enter_cmds[TEST_LOOP_0]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,check-sum2-detect-rd1-cmds",
			"lcd-kit,check-sum2-detect-rd1-cmds-state",
			&FACT_INFO->check_sum2_detect.rd_cmds[TEST_LOOP_0]);
		lcd_kit_parse_array_data(np, "lcd-kit,check-sum2-detect-value1",
			&FACT_INFO->check_sum2_detect.value[TEST_LOOP_0]);

		lcd_kit_parse_dcs_cmds(np, "lcd-kit,check-sum2-detect-enter2-cmds",
			"lcd-kit,check-sum2-detect-enter2-cmds-state",
			&FACT_INFO->check_sum2_detect.enter_cmds[TEST_LOOP_1]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,check-sum2-detect-rd2-cmds",
			"lcd-kit,check-sum2-detect-rd2-cmds-state",
			&FACT_INFO->check_sum2_detect.rd_cmds[TEST_LOOP_1]);
		lcd_kit_parse_array_data(np, "lcd-kit,check-sum2-detect-value2",
			&FACT_INFO->check_sum2_detect.value[TEST_LOOP_1]);

		lcd_kit_parse_dcs_cmds(np, "lcd-kit,check-sum2-detect-enter3-cmds",
			"lcd-kit,check-sum2-detect-enter1-cmds-state",
			&FACT_INFO->check_sum2_detect.enter_cmds[TEST_LOOP_2]);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,check-sum2-detect-rd3-cmds",
			"lcd-kit,check-sum2-detect-rd3-cmds-state",
			&FACT_INFO->check_sum2_detect.rd_cmds[TEST_LOOP_2]);
		lcd_kit_parse_array_data(np, "lcd-kit,check-sum2-detect-value3",
			&FACT_INFO->check_sum2_detect.value[TEST_LOOP_2]);

		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-detect-dual-dsi-support",
			&FACT_INFO->check_sum2_detect.dual_dsi_support, 0);
		lcd_kit_parse_u32(np, "lcd-kit,check-sum2-model-switch",
			&FACT_INFO->check_sum2_detect.model_switch, 0);
	}
}

static void parse_dt_bl_open_short(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* backlight open short test */
	lcd_kit_parse_u32(np, "lcd-kit,bkl-open-short-support",
		&FACT_INFO->bl_open_short_support, 0);
}

static void parse_dt_checksum(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* check sum */
	lcd_kit_parse_u32(np, "lcd-kit,checksum-support",
		&FACT_INFO->checksum.support, 0);
	if (FACT_INFO->checksum.support) {
		lcd_kit_parse_u32(np,
			"lcd-kit,checksum-special-support",
			&FACT_INFO->checksum.special_support, 0);
		lcd_kit_parse_u32(np, "lcd-kit,checksum-need-clear-sram",
			&FACT_INFO->checksum.clear_sram, 0);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,checksum-enable-cmds",
			"lcd-kit,checksum-enable-cmds-state",
			&FACT_INFO->checksum.enable_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,checksum-disable-cmds",
			"lcd-kit,checksum-disable-cmds-state",
			&FACT_INFO->checksum.disable_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,checksum-cmds",
			"lcd-kit,checksum-cmds-state",
			&FACT_INFO->checksum.checksum_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,checksum-clear-sram-cmds",
			"lcd-kit,checksum-clear-sram-cmds-state",
			&FACT_INFO->checksum.clear_sram_cmds);
		lcd_kit_parse_arrays_data(np, "lcd-kit,checksum-value",
			&FACT_INFO->checksum.value, CHECKSUM_VALUE_SIZE);
		lcd_kit_parse_arrays_data(np, "lcd-kit,checksum-dsi1-value",
			&FACT_INFO->checksum.dsi1_value, CHECKSUM_VALUE_SIZE);
		/* checksum stress test */
		lcd_kit_parse_u32(np,
			"lcd-kit,checksum-stress-test-support",
			&FACT_INFO->checksum.stress_test_support, 0);
		if (FACT_INFO->checksum.stress_test_support) {
			lcd_kit_parse_u32(np,
				"lcd-kit,checksum-stress-test-vdd",
				&FACT_INFO->checksum.vdd, 0);
			lcd_kit_parse_u32(np,
				"lcd-kit,checksum-stress-test-mipi",
				&FACT_INFO->checksum.mipi_clk, 0);
		}
	}
}

static void parse_dt_hkadc(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* hkadc */
	lcd_kit_parse_u32(np, "lcd-kit,hkadc-support",
		&FACT_INFO->hkadc.support, 0);
	if (FACT_INFO->hkadc.support)
		lcd_kit_parse_u32(np, "lcd-kit,hkadc-value",
			&FACT_INFO->hkadc.value, 0);
}

static void parse_dt_curr_det(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* current detect */
	lcd_kit_parse_u32(np, "lcd-kit,current-det-support",
		&FACT_INFO->current_det.support, 0);
	if (FACT_INFO->current_det.support) {
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,current-det-cmds",
			"lcd-kit,current-det-cmds-state",
			&FACT_INFO->current_det.detect_cmds);
		lcd_kit_parse_array_data(np, "lcd-kit,current-det-value",
			&FACT_INFO->current_det.value);
	}
}

static void parse_dt_ldo_check(struct device_node *np)
{
	int ret;
	char *name[LDO_NUM_MAX] = {NULL};
	int i;
	int panel_id = LCD_ACTIVE_PANEL;

	/* ldo check */
	lcd_kit_parse_u32(np, "lcd-kit,ldo-check-support",
		&FACT_INFO->ldo_check.support, 0);
	if (FACT_INFO->ldo_check.support) {
		FACT_INFO->ldo_check.ldo_num = of_property_count_elems_of_size(
			np, "lcd-kit,ldo-check-channel", sizeof(u32));
		if (FACT_INFO->ldo_check.ldo_num > 0) {
			ret = of_property_read_u32_array(np,
				"lcd-kit,ldo-check-channel",
				FACT_INFO->ldo_check.ldo_channel,
				FACT_INFO->ldo_check.ldo_num);
			if (ret < 0)
				LCD_KIT_ERR("parse ldo channel fail\n");
			ret = of_property_read_u32_array(np,
				"lcd-kit,ldo-check-threshold",
				FACT_INFO->ldo_check.curr_threshold,
				FACT_INFO->ldo_check.ldo_num);
			if (ret < 0)
				LCD_KIT_ERR("parse current threshold fail\n");
			ret = of_property_read_string_array(np,
				"lcd-kit,ldo-check-name",
				(const char **)&name[0],
				FACT_INFO->ldo_check.ldo_num);
			if (ret < 0)
				LCD_KIT_ERR("parse ldo name fail\n");
			for (i = 0; i < (int)FACT_INFO->ldo_check.ldo_num; i++)
				strncpy(FACT_INFO->ldo_check.ldo_name[i],
					name[i], LDO_NAME_LEN_MAX - 1);
		}
	}
}

static void parse_dt_avdd_det(struct device_node *np)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* ddic low voltage detect test */
	lcd_kit_parse_u32(np, "lcd-kit,avdd-det-support",
		&FACT_INFO->avdd_detect.support, 0);
	if (FACT_INFO->avdd_detect.support) {
		lcd_kit_parse_u32(np, "lcd-kit,avdd-det-vb-channel",
			&FACT_INFO->avdd_detect.vb_channel, 0);
		lcd_kit_parse_u32(np, "lcd-kit,avdd-det-low-threshold",
			&FACT_INFO->avdd_detect.low_threshold, 0);
		lcd_kit_parse_u32(np, "lcd-kit,avdd-det-high-threshold",
			&FACT_INFO->avdd_detect.high_threshold, 0);
		lcd_kit_parse_u32(np, "lcd-kit,avdd-det-gpio-grp-ctrl",
			&FACT_INFO->avdd_detect.gpio_grp_ctrl, 0);
		if (FACT_INFO->avdd_detect.gpio_grp_ctrl) {
			(void)lcd_kit_parse_array_data(np,
				"lcd-kit,avdd-det-gpio-grp",
				&FACT_INFO->avdd_detect.gpio_grp);
			(void)lcd_kit_parse_array_data(np,
				"lcd-kit,avdd-det-gpio-ctrl-a",
				&FACT_INFO->avdd_detect.gpio_ctrl_a);
			(void)lcd_kit_parse_array_data(np,
				"lcd-kit,avdd-det-gpio-ctrl-b",
				&FACT_INFO->avdd_detect.gpio_ctrl_b);
		}
	}
}

static void parse_dt(struct device_node *np)
{
	/* parse pcd errflag */
	parse_dt_pcd_errflag(np);
	/* check sum2 detect parse */
	parse_dt_check_sum2_detect(np);
	/* parse mbist */
	parse_dt_mbist_detect(np);
	/* parse backlight open short */
	parse_dt_bl_open_short(np);
	/* parse checksum */
	parse_dt_checksum(np);
	/* parse hkadc */
	parse_dt_hkadc(np);
	/* parse current detect */
	parse_dt_curr_det(np);
	/* parse ldo check */
	parse_dt_ldo_check(np);
	/* parse poweric detect */
	parse_dt_poweric_det(np);
	/* parse avdd detect */
	parse_dt_avdd_det(np);
}

int lcd_factory_init(struct device_node *np)
{
	struct hisi_fb_data_type *hisifd = NULL;
	int ret;

	lcd_kit_fact_init(LCD_ACTIVE_PANEL, np);
	parse_dt(np);

	hisifd = hisifd_list[PRIMARY_PANEL_IDX];
	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	lcd_fact_ops_register(&g_fact_ops);
	ret = lcd_create_fact_sysfs(&hisifd->fbi->dev->kobj);
	if (ret)
		LCD_KIT_ERR("create factory sysfs fail\n");
	return LCD_KIT_OK;
}

