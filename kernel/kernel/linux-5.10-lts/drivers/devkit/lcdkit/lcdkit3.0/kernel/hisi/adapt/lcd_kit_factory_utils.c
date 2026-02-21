/*
 * lcd_kit_factory_utils.c
 *
 * lcdkit hisi factory function for lcd driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#include <linux/jiffies.h>
#include "lcd_kit_factory_utils.h"
#if defined(CONFIG_LPM_HKADC)
#include <platform_include/cee/linux/adc.h>
#endif

#define CHECK_SUM2_LOOP_TIME 60
#define CHECK_SUM2_LOOP_DELAY 500

#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
extern struct dsm_client *lcd_dclient;
#endif

static void checksum_set_mipi_clk(struct hisi_fb_data_type *hisifd,
	u32 mipi_clk)
{
	hisifd->panel_info.mipi.dsi_bit_clk_upt = mipi_clk;
}

static void checksum_set_vdd(u32 vdd)
{
	int panel_id = LCD_ACTIVE_PANEL;

	power_hdl->lcd_vdd.buf[POWER_VOL] = vdd;
}

void checksum_stress_enable(struct hisi_fb_data_type *hisifd)
{
	int ret;
	u32 mipi_clk;
	u32 lcd_vdd;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!FACT_INFO->checksum.stress_test_support) {
		LCD_KIT_ERR("not support checksum stress test\n");
		return;
	}
	if (FACT_INFO->checksum.mipi_clk) {
		mipi_clk = hisifd->panel_info.mipi.dsi_bit_clk_upt;
		FACT_INFO->checksum.rec_mipi_clk = mipi_clk;
		checksum_set_mipi_clk(hisifd, FACT_INFO->checksum.mipi_clk);
	}
	if (FACT_INFO->checksum.vdd) {
		if (power_hdl->lcd_vdd.buf == NULL) {
			LCD_KIT_ERR("lcd vdd buf is null\n");
			return;
		}
		lcd_vdd = power_hdl->lcd_vdd.buf[POWER_VOL];
		FACT_INFO->checksum.rec_vdd = lcd_vdd;
		checksum_set_vdd(FACT_INFO->checksum.vdd);
		ret = lcd_power_set_vol(LCD_KIT_VDD);
		if (ret)
			LCD_KIT_ERR("set voltage error\n");
	}
	lcd_kit_recovery_display(hisifd);
}

void checksum_stress_disable(struct hisi_fb_data_type *hisifd)
{
	int ret;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!FACT_INFO->checksum.stress_test_support) {
		LCD_KIT_ERR("not support checksum stress test\n");
		return;
	}
	if (FACT_INFO->checksum.mipi_clk)
		checksum_set_mipi_clk(hisifd,
			FACT_INFO->checksum.rec_mipi_clk);
	if (FACT_INFO->checksum.vdd) {
		if (power_hdl->lcd_vdd.buf == NULL) {
			LCD_KIT_ERR("lcd vdd buf is null\n");
			return;
		}
		checksum_set_vdd(FACT_INFO->checksum.rec_vdd);
		ret = lcd_power_set_vol(LCD_KIT_VDD);
		if (ret)
			LCD_KIT_ERR("set voltage error\n");
	}
	lcd_kit_recovery_display(hisifd);
}

static void lcd_enable_checksum(struct hisi_fb_data_type *hisifd)
{
	int panel_id = LCD_ACTIVE_PANEL;

	/* disable esd */
	lcd_esd_enable(hisifd, 0);
	FACT_INFO->checksum.status = LCD_KIT_CHECKSUM_START;
	if (!FACT_INFO->checksum.special_support) {
		if (FACT_INFO->checksum.clear_sram) {
			LCD_KIT_INFO("before checksum, clear sram\n");
			lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
				&FACT_INFO->checksum.clear_sram_cmds);
		} else {
			lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
				&FACT_INFO->checksum.enable_cmds);
		}
	}
	FACT_INFO->checksum.pic_index = INVALID_INDEX;
	LCD_KIT_INFO("Enable checksum\n");
}

int lcd_kit_checksum_set(struct hisi_fb_data_type *hisifd,
	int pic_index)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!hisifd) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (FACT_INFO->checksum.status == LCD_KIT_CHECKSUM_END) {
		if (pic_index == TEST_PIC_0) {
			LCD_KIT_INFO("start gram checksum\n");
			lcd_enable_checksum(hisifd);
			return ret;
		}
		LCD_KIT_INFO("pic index error\n");
		return LCD_KIT_FAIL;
	}
	if (pic_index == TEST_PIC_2)
		FACT_INFO->checksum.check_count++;
	if (FACT_INFO->checksum.check_count == CHECKSUM_CHECKCOUNT) {
		LCD_KIT_INFO("check 5 times, set checksum end\n");
		FACT_INFO->checksum.status = LCD_KIT_CHECKSUM_END;
		FACT_INFO->checksum.check_count = 0;
	}
	switch (pic_index) {
	case TEST_PIC_0:
	case TEST_PIC_1:
	case TEST_PIC_2:
		if (FACT_INFO->checksum.clear_sram)
			lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
				&FACT_INFO->checksum.enable_cmds);
		if (FACT_INFO->checksum.special_support)
			lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
				&FACT_INFO->checksum.enable_cmds);
		LCD_KIT_INFO("set pic [%d]\n", pic_index);
		FACT_INFO->checksum.pic_index = pic_index;
		break;
	default:
		LCD_KIT_INFO("set pic [%d] unknown\n", pic_index);
		FACT_INFO->checksum.pic_index = INVALID_INDEX;
		break;
	}
	return ret;
}

static int lcd_checksum_compare(uint8_t *read_value, uint32_t *value,
	int len)
{
	int i;
	int err_no = 0;
	uint8_t tmp;

	for (i = 0; i < len; i++) {
		tmp = (uint8_t)value[i];
		if (read_value[i] != tmp) {
			LCD_KIT_ERR("gram check error\n");
			LCD_KIT_ERR("read_value[%d]:0x%x\n", i, read_value[i]);
			LCD_KIT_ERR("expect_value[%d]:0x%x\n", i, tmp);
			err_no++;
		}
	}
	return err_no;
}

static int lcd_check_checksum(struct hisi_fb_data_type *hisifd)
{
	int ret;
	int err_cnt;
	int check_cnt;
	uint8_t read_value[LCD_KIT_CHECKSUM_SIZE + 1] = {0};
	uint32_t *checksum = NULL;
	uint32_t pic_index;
	int panel_id = LCD_ACTIVE_PANEL;

	if (FACT_INFO->checksum.value.arry_data == NULL) {
		LCD_KIT_ERR("null pointer\n");
		return LCD_KIT_FAIL;
	}
	pic_index = FACT_INFO->checksum.pic_index;
	ret = lcd_kit_dsi_cmds_rx(LCD_ACTIVE_PANEL, hisifd, read_value,
		LCD_KIT_CHECKSUM_SIZE, &FACT_INFO->checksum.checksum_cmds);
	if (ret) {
		LCD_KIT_ERR("checksum read dsi0 error\n");
		return LCD_KIT_FAIL;
	}
	check_cnt = FACT_INFO->checksum.value.arry_data[pic_index].cnt;
	if (check_cnt > LCD_KIT_CHECKSUM_SIZE) {
		LCD_KIT_ERR("checksum count is larger than checksum size\n");
		return LCD_KIT_FAIL;
	}
	checksum = FACT_INFO->checksum.value.arry_data[pic_index].buf;
	err_cnt = lcd_checksum_compare(read_value, checksum, check_cnt);
	if (err_cnt) {
		LCD_KIT_ERR("err_cnt:%d\n", err_cnt);
		ret = LCD_KIT_FAIL;
	}
	return ret;
}

static int lcd_check_dsi1_checksum(struct hisi_fb_data_type *hisifd)
{
	int ret;
	int err_cnt;
	int check_cnt;
	uint8_t read_value[LCD_KIT_CHECKSUM_SIZE + 1] = {0};
	uint32_t *checksum = NULL;
	uint32_t pic_index;
	int panel_id = LCD_ACTIVE_PANEL;

	if (FACT_INFO->checksum.dsi1_value.arry_data == NULL) {
		LCD_KIT_ERR("null pointer\n");
		return LCD_KIT_FAIL;
	}
	pic_index = FACT_INFO->checksum.pic_index;
	ret = lcd_kit_dsi1_cmds_rx(hisifd, read_value, LCD_KIT_CHECKSUM_SIZE,
		&FACT_INFO->checksum.checksum_cmds);
	if (ret) {
		LCD_KIT_ERR("checksum read dsi1 error\n");
		return LCD_KIT_FAIL;
	}
	check_cnt = FACT_INFO->checksum.dsi1_value.arry_data[pic_index].cnt;
	if (check_cnt > LCD_KIT_CHECKSUM_SIZE) {
		LCD_KIT_ERR("checksum count is larger than checksum size\n");
		return LCD_KIT_FAIL;
	}
	checksum = FACT_INFO->checksum.dsi1_value.arry_data[pic_index].buf;
	err_cnt = lcd_checksum_compare(read_value, checksum, check_cnt);
	if (err_cnt) {
		LCD_KIT_ERR("err_cnt:%d\n", err_cnt);
		ret = LCD_KIT_FAIL;
	}
	return ret;
}

int lcd_kit_current_det(struct hisi_fb_data_type *hisifd)
{
	int current_check_result;
	uint8_t rd = 0;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!FACT_INFO->current_det.support) {
		LCD_KIT_INFO("current detect is not support! return!\n");
		return LCD_KIT_OK;
	}
	lcd_kit_dsi_cmds_rx(LCD_ACTIVE_PANEL, hisifd, &rd,
		1, &FACT_INFO->current_det.detect_cmds);
	LCD_KIT_INFO("current detect, read value = 0x%x\n", rd);
	/* buf[0] means current detect result */
	if ((rd & FACT_INFO->current_det.value.buf[0]) == LCD_KIT_OK) {
		current_check_result = LCD_KIT_OK;
		LCD_KIT_ERR("no current over\n");
	} else {
		current_check_result = LCD_KIT_FAIL;
		LCD_KIT_ERR("current over:0x%x\n", rd);
	}
	return current_check_result;
}

int lcd_kit_lv_det(struct hisi_fb_data_type *hisifd)
{
	int lv_check_result;
	uint8_t rd = 0;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!FACT_INFO->lv_det.support) {
		LCD_KIT_INFO("current detect is not support! return!\n");
		return LCD_KIT_OK;
	}
	lcd_kit_dsi_cmds_rx(LCD_ACTIVE_PANEL, hisifd, &rd,
		1, &FACT_INFO->lv_det.detect_cmds);
	LCD_KIT_INFO("current detect, read value = 0x%x\n", rd);
	if ((rd & FACT_INFO->lv_det.value.buf[0]) == 0) {
		lv_check_result = LCD_KIT_OK;
		LCD_KIT_ERR("no current over\n");
	} else {
		lv_check_result = LCD_KIT_FAIL;
		LCD_KIT_ERR("current over:0x%x\n", rd);
	}
	return lv_check_result;
}

int lcd_hor_line_test(struct hisi_fb_data_type *hisifd)
{
	int ret = LCD_KIT_OK;
	struct hisi_panel_info *pinfo = NULL;
	int count = HOR_LINE_TEST_TIMES;
	int panel_id = LCD_ACTIVE_PANEL;

	pinfo = &(hisifd->panel_info);
	if (pinfo == NULL) {
		LCD_KIT_ERR("pinfo is NULL!\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("horizontal line test start\n");
	LCD_KIT_INFO("FACT_INFO->hor_line.duration = %d\n",
			FACT_INFO->hor_line.duration);
	/* disable esd check */
	lcd_esd_enable(hisifd, 0);
	/* disable elvdd detect */
	disp_info->elvdd_detect.support = 0;
	while (count--) {
		/* hardware reset */
		if (!FACT_INFO->hor_line.hl_no_reset)
			lcd_hardware_reset(LCD_ACTIVE_PANEL);
		mdelay(30);
		/* test avdd */
		down(&hisifd->blank_sem);
		if (!hisifd->panel_power_on) {
			LCD_KIT_ERR("panel is power off\n");
			up(&hisifd->blank_sem);
			return LCD_KIT_FAIL;
		}
		hisifb_activate_vsync(hisifd);
		if (FACT_INFO->hor_line.hl_cmds.cmds != NULL) {
			ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
				&FACT_INFO->hor_line.hl_cmds);
			if (ret)
				LCD_KIT_ERR("send avdd cmd error\n");
		}
		hisifb_deactivate_vsync(hisifd);
		up(&hisifd->blank_sem);
		msleep(FACT_INFO->hor_line.duration * MILLISEC_TIME);
		/* recovery display */
		lcd_kit_recovery_display(hisifd);
	}
	/* enable esd */
	lcd_esd_enable(hisifd, 1);
	LCD_KIT_INFO("horizontal line test end\n");
	return ret;
}

int lcd_kit_checksum_check(struct hisi_fb_data_type *hisifd)
{
	int ret;
	uint32_t pic_index;
	int panel_id = LCD_ACTIVE_PANEL;

	pic_index = FACT_INFO->checksum.pic_index;
	LCD_KIT_INFO("checksum pic num:%d\n", pic_index);
	if (pic_index > TEST_PIC_2) {
		LCD_KIT_ERR("checksum pic num unknown:%d\n", pic_index);
		return LCD_KIT_FAIL;
	}
	ret = lcd_check_checksum(hisifd);
	if (ret)
		LCD_KIT_ERR("checksum error\n");
	if (is_dual_mipi_panel(hisifd)) {
		ret = lcd_check_dsi1_checksum(hisifd);
		if (ret)
			LCD_KIT_ERR("dsi1 checksum error\n");
	}
	if (ret && FACT_INFO->checksum.pic_index == TEST_PIC_2)
		FACT_INFO->checksum.status = LCD_KIT_CHECKSUM_END;

	if (FACT_INFO->checksum.status == LCD_KIT_CHECKSUM_END) {
		lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->checksum.disable_cmds);
		LCD_KIT_INFO("gram checksum end, disable checksum\n");
		/* enable esd */
		lcd_esd_enable(hisifd, 1);
	}
	return ret;
}

static void poweric_gpio_ctl(int num, int pull_type)
{
	int gpio_temp;
	int panel_id = LCD_ACTIVE_PANEL;

	if (num >= FACT_INFO->poweric_detect.gpio_list.cnt) {
		LCD_KIT_ERR("num is out bound\n");
		return;
	}
	gpio_temp = FACT_INFO->poweric_detect.gpio_list.buf[num];
	if (pull_type) {
		if (FACT_INFO->poweric_detect.gpio_val.buf[num])
			lcd_poweric_gpio_operator(gpio_temp, GPIO_HIGH);
		else
			lcd_poweric_gpio_operator(gpio_temp, GPIO_LOW);
	} else {
		if (FACT_INFO->poweric_detect.gpio_val.buf[num])
			lcd_poweric_gpio_operator(gpio_temp, GPIO_LOW);
		else
			lcd_poweric_gpio_operator(gpio_temp, GPIO_HIGH);
	}
}

static int poweric_get_elvdd(struct hisi_fb_data_type *hisifd, int i)
{
	int ret;
	int32_t gpio_value;
	int panel_id = LCD_ACTIVE_PANEL;

	lcd_poweric_gpio_operator(FACT_INFO->poweric_detect.detect_gpio,
		GPIO_REQ);
	ret = gpio_direction_input(FACT_INFO->poweric_detect.detect_gpio);
	if (ret) {
		gpio_free(FACT_INFO->poweric_detect.detect_gpio);
		LCD_KIT_ERR("poweric_gpio[%d] direction set fail\n",
			FACT_INFO->poweric_detect.detect_gpio);
		return LCD_KIT_FAIL;
	}
	gpio_value = gpio_get_value(FACT_INFO->poweric_detect.detect_gpio);
	LCD_KIT_INFO("poweric detect elvdd value is %d\n", gpio_value);

	if (i >= ARRAY_SIZE(hisifd->panel_info.poweric_status)) {
		LCD_KIT_ERR("i is out of bounds");
		return LCD_KIT_FAIL;
	}
	if (gpio_value != FACT_INFO->poweric_detect.detect_val)
		hisifd->panel_info.poweric_status[i] = POWERIC_ERR;
	else
		hisifd->panel_info.poweric_status[i] = POWERIC_NOR;
	return LCD_KIT_OK;
}

static int poweric_set_elvdd(struct hisi_fb_data_type *hisifd,
	int start, int end, int i)
{
	int j, k, ret, temp;
	int gpio_flag = 0;
	int panel_id = LCD_ACTIVE_PANEL;

	if (end >= FACT_INFO->poweric_detect.gpio_list.cnt) {
		LCD_KIT_ERR("end out of bound\n");
		return LCD_KIT_FAIL;
	}
	if (i >= ARRAY_SIZE(hisifd->panel_info.poweric_status)) {
		LCD_KIT_ERR("i is out of bounds");
		return LCD_KIT_FAIL;
	}
	for (j = start; j < end; j++) {
		lcd_poweric_gpio_operator(FACT_INFO->poweric_detect.gpio_list.buf[j],
			GPIO_REQ);
		ret = gpio_direction_output(FACT_INFO->poweric_detect.gpio_list.buf[j],
			OUTPUT_TYPE);
		if (ret) {
			gpio_flag = POWERIC_NOR;
			for (k = start; k <= j; k++) {
				temp = FACT_INFO->poweric_detect.gpio_list.buf[k];
				lcd_poweric_gpio_operator(temp, GPIO_FREE);
			}
			LCD_KIT_ERR("poweric_gpio[%d] direction set fail\n",
				FACT_INFO->poweric_detect.gpio_list.buf[j]);
			hisifd->panel_info.poweric_status[i] = POWERIC_ERR;
			break;
		}
		LCD_KIT_INFO("disp_info->poweric_detect.gpio_list.buf[j] = %d\n",
			FACT_INFO->poweric_detect.gpio_list.buf[j]);
		/* enable gpio */
		poweric_gpio_ctl(j, PULL_TYPE_NOR);
		/* enable delay between gpio */
		msleep(10);
	}
	return gpio_flag;
}

void poweric_reverse_gpio(int start, int end)
{
	int j;
	for (j = start; j < end; j++) {
		poweric_gpio_ctl(j, PULL_TYPE_REV);
		/* disable delay between gpio */
		msleep(10);
	}
}

void poweric_free_gpio(int start, int end)
{
	int j;
	int panel_id = LCD_ACTIVE_PANEL;

	for (j = start; j < end; j++)
		lcd_poweric_gpio_operator(
			FACT_INFO->poweric_detect.gpio_list.buf[j],
			GPIO_FREE);
}

void poweric_gpio_detect_dbc(struct hisi_fb_data_type *hisifd)
{
	int i, gpio_temp;
	int32_t gpio_value;
	int gpio_pos = 0;
	int gpio_flag, temp;
	int panel_id = LCD_ACTIVE_PANEL;

	temp = FACT_INFO->poweric_detect.poweric_num;
	hisifd->panel_info.poweric_num_length = temp;
	LCD_KIT_INFO("poweric_num = %d\n", FACT_INFO->poweric_detect.poweric_num);
	if (FACT_INFO->poweric_detect.poweric_num >= POWERIC_NUM_MAX) {
		LCD_KIT_ERR("poweric_num out bound\n");
		return;
	}
	for (i = 0; i < FACT_INFO->poweric_detect.poweric_num; i++) {
		temp = FACT_INFO->poweric_detect.detect_gpio;
		gpio_temp = gpio_pos;
		gpio_pos += FACT_INFO->poweric_detect.gpio_num_list.buf[i];
		/* operate gpio to detect elvdd */
		gpio_flag = poweric_set_elvdd(hisifd, gpio_temp, gpio_pos, i);
		if (!gpio_flag) {
			LCD_KIT_INFO("detect_gpio = %d\n", temp);
			if (poweric_get_elvdd(hisifd, i))
				LCD_KIT_INFO("get ELVDD fail %d\n", temp);
			/* reverse operate gpio to recovery elvdd */
			poweric_reverse_gpio(gpio_temp, gpio_pos);
			/* delay before obtaining ELVDD */
			msleep(1000);
			gpio_value = gpio_get_value(
				FACT_INFO->poweric_detect.detect_gpio);
			lcd_poweric_gpio_operator(
				FACT_INFO->poweric_detect.detect_gpio,
				GPIO_FREE);
			/* free gpio */
			poweric_free_gpio(gpio_temp, gpio_pos);
			LCD_KIT_INFO("second detect elvdd value is %d\n", gpio_value);
		} else {
			gpio_flag = POWERIC_ERR;
		}
	}
}

ssize_t lcd_amoled_gpio_pcd_errflag(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;

	if (buf == NULL) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}

	ret = lcd_kit_gpio_pcd_errflag_check();
	LCD_KIT_INFO("pcd_errflag result_value = %d\n", ret);
	ret = snprintf(buf, PAGE_SIZE, "%d\n", ret);
	return (ssize_t)ret;
}

ssize_t lcd_amoled_cmds_pcd_errflag(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = LCD_KIT_OK;
	int check_result;
	struct hisi_fb_data_type *hisifd = NULL;

	hisifd = lcd_kit_get_hisifd(dev);
	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	if (buf == NULL) {
		LCD_KIT_ERR("buf is null\n");
		return LCD_KIT_FAIL;
	}
	if (hisifd->panel_power_on) {
		if (disp_info->pcd_errflag.pcd_support ||
			disp_info->pcd_errflag.errflag_support) {
			check_result = lcd_kit_check_pcd_errflag_check(hisifd);
			ret = snprintf(buf, PAGE_SIZE, "%d\n", check_result);
			LCD_KIT_INFO("pcd_errflag, the check_result = %d\n",
				check_result);
		}
	}
	return ret;
}

int lcd_kit_pcd_detect(struct hisi_fb_data_type *hisifd, uint32_t val)
{
	int ret = LCD_KIT_OK;
	static uint32_t pcd_mode;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!hisifd) {
		LCD_KIT_ERR("hisifd is NULL\n");
		return LCD_KIT_FAIL;
	}

	if (!FACT_INFO->pcd_errflag_check.pcd_errflag_check_support) {
		LCD_KIT_ERR("pcd errflag not support!\n");
		return LCD_KIT_OK;
	}

	if (pcd_mode == val) {
		LCD_KIT_ERR("pcd detect already\n");
		return LCD_KIT_OK;
	}

	pcd_mode = val;
	if (pcd_mode == LCD_KIT_PCD_DETECT_OPEN)
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->pcd_errflag_check.pcd_detect_open_cmds);
	else if (pcd_mode == LCD_KIT_PCD_DETECT_CLOSE)
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->pcd_errflag_check.pcd_detect_close_cmds);
	LCD_KIT_INFO("pcd_mode %d, ret %d\n", pcd_mode, ret);
	return ret;
}

void ldo_transform(struct lcd_ldo *ldo)
{
	int i;
	int panel_id = LCD_ACTIVE_PANEL;

	ldo->lcd_ldo_num = FACT_INFO->ldo_check.ldo_num;
	memcpy(ldo->lcd_ldo_name, FACT_INFO->ldo_check.ldo_name,
		sizeof(FACT_INFO->ldo_check.ldo_name));
	memcpy(ldo->lcd_ldo_channel, FACT_INFO->ldo_check.ldo_channel,
		sizeof(FACT_INFO->ldo_check.ldo_channel));
	memcpy(ldo->lcd_ldo_current, FACT_INFO->ldo_check.ldo_current,
		sizeof(FACT_INFO->ldo_check.ldo_current));
	memcpy(ldo->lcd_ldo_threshold, FACT_INFO->ldo_check.curr_threshold,
		sizeof(FACT_INFO->ldo_check.curr_threshold));
	for (i = 0; i < ldo->lcd_ldo_num; i++) {
		LCD_KIT_INFO("ldo[%d]:\n", i);
		LCD_KIT_INFO("name=%s,current=%d,channel=%d,threshold=%d!\n",
			ldo->lcd_ldo_name[i],
			ldo->lcd_ldo_current[i],
			ldo->lcd_ldo_channel[i],
			ldo->lcd_ldo_threshold[i]);
	}
}

static void lcd_vtc_line_set_bias_voltage(struct hisi_fb_data_type *hisifd)
{
	struct lcd_kit_bias_ops *bias_ops = NULL;
	int ret;
	int panel_id = LCD_ACTIVE_PANEL;

	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return;
	}

	bias_ops = lcd_kit_get_bias_ops();
	if (bias_ops == NULL) {
		LCD_KIT_ERR("can not get bias_ops\n");
		return;
	}
	/* set bias voltage */
	if ((bias_ops->set_vtc_bias_voltage) &&
		(FACT_INFO->vtc_line.vtc_vsp > 0) &&
		(FACT_INFO->vtc_line.vtc_vsn > 0)) {
		ret = bias_ops->set_vtc_bias_voltage(FACT_INFO->vtc_line.vtc_vsp,
			FACT_INFO->vtc_line.vtc_vsn, true);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("set vtc bias voltage error\n");
		if (bias_ops->set_bias_is_need_disable() == true)
			lcd_kit_recovery_display(hisifd);
	}
}

static void lcd_vtc_line_resume_bias_voltage(struct hisi_fb_data_type *hisifd)
{
	struct lcd_kit_bias_ops *bias_ops = NULL;
	int ret;
	int panel_id = LCD_ACTIVE_PANEL;

	if (hisifd == NULL) {
		LCD_KIT_ERR("hisifd is null\n");
		return;
	}

	bias_ops = lcd_kit_get_bias_ops();
	if (bias_ops == NULL) {
		LCD_KIT_ERR("can not get bias_ops\n");
		return;
	}
	/* set bias voltage */
	if ((bias_ops->set_vtc_bias_voltage) &&
		(bias_ops->set_bias_is_need_disable() == true)) {
		/* buf[2]:set bias voltage value */
		ret = bias_ops->set_vtc_bias_voltage(power_hdl->lcd_vsp.buf[2],
			power_hdl->lcd_vsn.buf[2], false);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("set vtc bias voltage error\n");
	}
}

int lcd_vtc_line_test(struct hisi_fb_data_type *hisifd,
	unsigned long pic_index)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	switch (pic_index) {
	case PIC1_INDEX:
		/* disable esd */
		lcd_esd_enable(hisifd, 0);
		/* disable elvdd detect */
		disp_info->elvdd_detect.support = 0;
		/* lcd panel set bias */
		lcd_vtc_line_set_bias_voltage(hisifd);
		/* hardware reset */
		if (!FACT_INFO->vtc_line.vtc_no_reset)
			lcd_hardware_reset(LCD_ACTIVE_PANEL);
		mdelay(20);
		if (FACT_INFO->vtc_line.vtc_cmds.cmds != NULL) {
			ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
				&FACT_INFO->vtc_line.vtc_cmds);
			if (ret)
				LCD_KIT_ERR("send vtc cmd error\n");
		}
		break;
	case PIC2_INDEX:
	case PIC3_INDEX:
	case PIC4_INDEX:
		LCD_KIT_INFO("picture:%lu display\n", pic_index);
		break;
	case PIC5_INDEX:
		/* lcd panel resume bias */
		lcd_vtc_line_resume_bias_voltage(hisifd);
		lcd_kit_recovery_display(hisifd);
		/* enable esd */
		lcd_esd_enable(hisifd, 1);
		break;
	default:
		LCD_KIT_ERR("pic number not support\n");
		break;
	}
	return ret;
}

#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)) && defined(CONFIG_HUAWEI_DATA_ACQUISITION)

#define AVDD_PRODUCT_CODE     703026001

static int lcd_data_report_err(int err_no, const struct message *msg)
{
	int ret = LCD_KIT_OK;

	if (!msg) {
		LCD_KIT_ERR("msg is NULL Pointer\n");
		return LCD_KIT_FAIL;
	}

	if (lcd_dclient && !dsm_client_ocuppy(lcd_dclient)) {
		ret = dsm_client_copy_ext(lcd_dclient, msg,
			sizeof(struct message));
		if (ret > 0)
			dsm_client_notify(lcd_dclient, err_no);
		else
			LCD_KIT_ERR("dsm_client_notify failed");
	}
	return ret;
}

static int data_message_report(const struct event *event)
{
	int ret;
	struct message *msg = NULL;

	msg = kzalloc(sizeof(struct message), GFP_KERNEL);
	if (!msg) {
		LCD_KIT_ERR("alloc message failed\n");
		return -1;
	}

	msg->version = 1;
	msg->num_events = 1;
	msg->data_source = DATA_FROM_KERNEL;
	memcpy(&(msg->events[0]), event, sizeof(struct event));
	ret = lcd_data_report_err(DA_LCD_AVDD_ERROR_NO, msg);
	kfree(msg);
	msg = NULL;
	return ret;
}

void lcd_avdd_detect_data_report(uint16_t value)
{
	int ret;
	u32 min;
	u32 max;
	struct event event;
	int panel_id = LCD_ACTIVE_PANEL;

	min = FACT_INFO->avdd_detect.low_threshold;
	max = FACT_INFO->avdd_detect.high_threshold;
	memset(&event, 0, sizeof(struct event));
	event.item_id = AVDD_PRODUCT_CODE;
	memcpy(event.station, "NA", strlen("NA") + 1);
	memcpy(event.bsn, "NA", strlen("NA") + 1);
	memcpy(event.firmware, "NA", strlen("NA") + 1);
	memcpy(event.description, "NA", strlen("NA") + 1);
	memcpy(event.device_name, "LCD_DDIC", strlen("LCD_DDIC") + 1);
	memcpy(event.test_name, "AVDD_DETECT", strlen("AVDD_DETECT") + 1);
	if (value < min || value > max)
		memcpy(event.result, "fail", strlen("fail") + 1);
	else
		memcpy(event.result, "pass", strlen("pass") + 1);
	ret = snprintf(event.value, MAX_VAL_LEN, "%u", value);
	if (ret < 0) {
		LCD_KIT_ERR("snprintf error\n");
		return;
	}
	ret = snprintf(event.min_threshold, MAX_VAL_LEN, "%u", min);
	if (ret < 0) {
		LCD_KIT_ERR("snprintf error\n");
		return;
	}
	ret = snprintf(event.max_threshold, MAX_VAL_LEN, "%u", max);
	if (ret < 0) {
		LCD_KIT_ERR("snprintf error\n");
		return;
	}
	ret = data_message_report(&event);
	if (ret > 0)
		LCD_KIT_INFO("avdd data report succ\n");
	else
		LCD_KIT_ERR("avdd data report failed\n");
}
#endif
int process_vb_value(void)
{
	int i;
	int j;
	int temp;
	int average;
	int vb_channel;
	int vb_arr[ADC_READ_TIMES] = {0};
	int panel_id = LCD_ACTIVE_PANEL;

	vb_channel = FACT_INFO->avdd_detect.vb_channel;
#if defined(CONFIG_LPM_HKADC)
	for (i = 0; i < ADC_READ_TIMES; i++) {
		vb_arr[i] = lpm_adc_get_value(vb_channel);
		LCD_KIT_ERR("vb_arr[%d] = %d\n", i, vb_arr[i]);
		mdelay(1);
	}
#else
	LCD_KIT_ERR("hisi hkadc is not support\n");
	return LCD_KIT_FAIL;
#endif
	/* array sort */
	for (i = 1; i < ADC_READ_TIMES; i++) {
		temp = vb_arr[i];
		for (j = i; j > 0 && vb_arr[j - 1] > temp; j--)
			vb_arr[j] = vb_arr[j - 1];
		vb_arr[j] = temp;
	}
	temp = 0;
	/* remove the max and min values, then calculate the average value */
	for (i = 1; i < ADC_READ_TIMES - 1; i++)
		temp += vb_arr[i];
	average = temp / (ADC_READ_TIMES - 2);
	LCD_KIT_ERR("average = %d\n", average);
	return average;
}
static void lcd_avdd_gpio_free(
	struct lcd_kit_array_data gpio_grp, int gpio_cnt)
{
	int i;

	for (i = 0; i < gpio_cnt; i++) {
		poweric_gpio = gpio_grp.buf[i];
		(void)lcd_kit_gpio_tx(LCD_KIT_POWERIC_DET_DBC,
			GPIO_FREE);
	}
}
static int lcd_avdd_gpio_config(
	struct lcd_kit_array_data gpio_grp,
	struct lcd_kit_array_data gpio_ctrl)
{
	int i;
	int ret = LCD_KIT_OK;

	for (i = 0; i < gpio_grp.cnt; i++) {
		poweric_gpio = gpio_grp.buf[i];
		ret = lcd_kit_gpio_tx(LCD_KIT_POWERIC_DET_DBC,
			gpio_ctrl.buf[i]);
		if (ret < 0) {
			(void)lcd_avdd_gpio_free(gpio_grp, gpio_grp.cnt);
			return ret;
		}
	}
	return ret;
}
static int lcd_avdd_gpio_request(struct lcd_kit_array_data gpio_grp)
{
	int i;
	int ret = LCD_KIT_OK;

	for (i = 0; i < gpio_grp.cnt; i++) {
		poweric_gpio = gpio_grp.buf[i];
		ret = lcd_kit_gpio_tx(LCD_KIT_POWERIC_DET_DBC,
			GPIO_REQ);
		if (ret < 0) {
			(void)lcd_avdd_gpio_free(gpio_grp, i);
			return ret;
		}
	}
	return ret;
}
static int lcd_get_gpio_adc_val(
	struct lcd_kit_array_data gpio_grp,
	struct lcd_kit_array_data gpio_ctrl)
{
	int ret;
	int adc_value;

	if (!gpio_grp.buf || !gpio_ctrl.buf) {
		LCD_KIT_ERR("buff is null.\n");
		return 0;
	}

	if ((gpio_grp.cnt != gpio_ctrl.cnt) ||
		(gpio_grp.cnt == 0)) {
		LCD_KIT_ERR("gpio_cnt %d, a_cnt %d, b_cnt %d.\n",
			gpio_grp.cnt, gpio_ctrl.cnt, gpio_ctrl.cnt);
		return 0;
	}

	ret = lcd_avdd_gpio_request(gpio_grp);
	if (ret < 0) {
		LCD_KIT_ERR("gpio requst fail.\n");
		return ret;
	}

	ret = lcd_avdd_gpio_config(gpio_grp, gpio_ctrl);
	if (ret < 0) {
		LCD_KIT_ERR("gpio requst fail.\n");
		return ret;
	}

	adc_value = process_vb_value();

	(void)lcd_avdd_gpio_free(gpio_grp, gpio_grp.cnt);

	return adc_value;
}
int lcd_get_avdd_gpio_grp_det_result(char *buf)
{
	int det_ret = LCD_KIT_OK;
	int gpio_det_result;
	int gpio_adc_val_a;
	int gpio_adc_val_b;
	u32 low_threshold;
	u32 high_threshold;
	int panel_id = LCD_ACTIVE_PANEL;

	gpio_adc_val_a = lcd_get_gpio_adc_val(
		FACT_INFO->avdd_detect.gpio_grp,
		FACT_INFO->avdd_detect.gpio_ctrl_a);
	gpio_adc_val_b = lcd_get_gpio_adc_val(
		FACT_INFO->avdd_detect.gpio_grp,
		FACT_INFO->avdd_detect.gpio_ctrl_b);
	if ((gpio_adc_val_a < 0) ||
		(gpio_adc_val_b < 0) ||
		(gpio_adc_val_a < gpio_adc_val_b)) {
		LCD_KIT_INFO("avdd det err, val_a = %d, val_b = %d.\n",
			gpio_adc_val_a,
			gpio_adc_val_b);
		return det_ret;
	}
	gpio_det_result = gpio_adc_val_a - gpio_adc_val_b;
	low_threshold = FACT_INFO->avdd_detect.low_threshold;
	high_threshold = FACT_INFO->avdd_detect.high_threshold;
	if (gpio_det_result < low_threshold ||
		gpio_det_result > high_threshold) {
		LCD_KIT_ERR("avdd det err, value = %d, threshold : %d ~ %d.\n",
			gpio_det_result,
			low_threshold,
			high_threshold);
		/* err code is current panel number */
		return 1 << LCD_ACTIVE_PANEL;
	}
	LCD_KIT_INFO("panel %d: val_a %d, val_b %d, det_ret %d\n",
		LCD_ACTIVE_PANEL, gpio_adc_val_a, gpio_adc_val_b, det_ret);
	return det_ret;
}

int lcd_kit_oneside_setting(struct hisi_fb_data_type *hisifd,
	uint32_t mode)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is null\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("oneside driver mode is %d\n", mode);
	if ((mode >= ONESIDE_DRIVER_LEFT) && (mode <= ONESIDE_MODE_EXIT))
		FACT_INFO->oneside_mode.mode = (int)mode;
	switch (mode) {
	case ONESIDE_DRIVER_LEFT:
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->oneside_mode.left_cmds);
		break;
	case ONESIDE_DRIVER_RIGHT:
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->oneside_mode.right_cmds);
		break;
	case ONESIDE_MODE_EXIT:
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->oneside_mode.exit_cmds);
		break;
	default:
		break;
	}
	return ret;
}

int lcd_kit_aod_detect_setting(struct hisi_fb_data_type *hisifd,
	uint32_t mode)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	LCD_KIT_INFO("color aod detect mode is %d\n", mode);
	if ((mode >= COLOR_AOD_DETECT_ENTER) &&
		(mode <= COLOR_AOD_DETECT_EXIT))
		FACT_INFO->color_aod_det.mode = (int)mode;
	if (mode == COLOR_AOD_DETECT_ENTER)
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->color_aod_det.enter_cmds);
	else if (mode == COLOR_AOD_DETECT_EXIT)
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hisifd,
			&FACT_INFO->color_aod_det.exit_cmds);
	else
		HISI_FB_ERR("color aod detect mode not support\n");
	return ret;
}

int lcd_kit_avdd_mipi_ctrl(void *hld, int enable)
{
	int ret = LCD_KIT_OK;
	int panel_id = LCD_ACTIVE_PANEL;

	if (hld == NULL) {
		LCD_KIT_ERR("NULL Pointer\n");
		return LCD_KIT_FAIL;
	}
	if (!lcd_kit_is_enter_sleep_mode(LCD_ACTIVE_PANEL)) {
		LCD_KIT_INFO("PT mode is not support\n");
		return LCD_KIT_OK;
	}
	if (enable && (FACT_INFO->pt.avdd_disable_cmds.cmds != NULL)) {
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hld,
			&FACT_INFO->pt.avdd_disable_cmds);
		if (ret)
			LCD_KIT_ERR("avdd disable cmd error\n");
		LCD_KIT_INFO("pt avdd disable\n");
	} else if (!enable && (FACT_INFO->pt.avdd_enable_cmds.cmds != NULL)) {
		ret = lcd_kit_dsi_cmds_tx(LCD_ACTIVE_PANEL, hld,
			&FACT_INFO->pt.avdd_enable_cmds);
		if (ret)
			LCD_KIT_ERR("avdd enable cmd error\n");
		LCD_KIT_INFO("pt avdd enable\n");
	}
	return ret;
}

static int fps_cmd_send(int panel_id, struct hisi_fb_data_type *hisifd,
	struct lcd_kit_dsi_panel_cmds *cmds)
{
	return lcd_kit_dsi_cmds_tx(panel_id, hisifd, cmds);
}

void send_fps_lock_cmd(int panel_id, struct hisi_fb_data_type *hisifd,
	unsigned int val)
{
	int ret = LCD_KIT_OK;
	int table_num;
	int i;
	struct fps_cmd_table fps_lock_cmd[] = {
		{ FPS_120, &disp_info->fps.fps_to_120_cmds },
		{ FPS_90, &disp_info->fps.fps_to_90_cmds },
		{ FPS_60, &disp_info->fps.fps_to_60_cmds },
		{ FPS_45, &disp_info->fps.fps_to_45_cmds },
		{ FPS_40, &disp_info->fps.fps_to_40_cmds },
		{ FPS_30, &disp_info->fps.fps_to_30_cmds },
		{ FPS_24, &disp_info->fps.fps_to_24_cmds },
		{ FPS_20, &disp_info->fps.fps_to_20_cmds },
		{ FPS_18, &disp_info->fps.fps_to_18_cmds },
		{ FPS_15, &disp_info->fps.fps_to_15_cmds },
		{ FPS_10, &disp_info->fps.fps_to_10_cmds },
		{ FPS_1, &disp_info->fps.fps_to_1_cmds },
	};

	table_num = sizeof(fps_lock_cmd) / sizeof(fps_lock_cmd[0]);
	for (i = 0; i < table_num; i++) {
		if (val == fps_lock_cmd[i].val)
			ret = fps_cmd_send(panel_id, hisifd, fps_lock_cmd[i].cmds);
	}
	disp_info->fps.current_fps = val;
	LCD_KIT_INFO("ret = %d", ret);
}

int lcd_check_sum2_detect_set(uint32_t panel_id, struct hisi_fb_data_type *hisifd,
	unsigned int loop_num)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_dsi_panel_cmds *enter_cmds = NULL;

	LCD_KIT_INFO("enter\n");
	enter_cmds = FACT_INFO->check_sum2_detect.enter_cmds;
	if (loop_num >= CHECK_SUM2_NUM) {
		LCD_KIT_ERR("set loop : %u unknown\n", loop_num);
		FACT_INFO->check_sum2_detect.loop_index = INVALID_INDEX;
		return ret;
	}
	if (!enter_cmds[loop_num].cmds) {
		LCD_KIT_ERR("loop %u enter cmds is null\n", loop_num);
		return LCD_KIT_FAIL;
	}
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is not power on!\n");
		return LCD_KIT_FAIL;
	}
	ret = lcd_kit_dsi_cmds_tx(panel_id, hisifd, &enter_cmds[loop_num]);
	if (ret) {
		LCD_KIT_ERR("send check sum2 enter cmds failed\n");
		return ret;
	}
	LCD_KIT_INFO("set picture : %u\n", loop_num);
	return ret;
}

static int checksum2_judge(unsigned char *read_value, unsigned int *value, int len)
{
	int i;
	int error_count = 0;
	char expect_value, judge_type;

	if (!read_value || !value) {
		LCD_KIT_ERR("read_value or value is null\n");
		return LCD_KIT_FAIL;
	}

	for (i = 0; i < len && i < CHECKSUM2_READ_COUNT; i++) {
		judge_type = (value[i] >> 8) & 0xFF;
		expect_value = value[i] & 0xFF;

		LCD_KIT_INFO("judge_type[%d]:0x%x\n", i, judge_type);

		switch (judge_type) {
		case CHECKSUM2_EQUAL:
			if (read_value[i] != expect_value) {
				LCD_KIT_ERR("lcd checksum error:"
					"read_value[%d] = 0x%x, but expect_value[%d] = 0x%x\n",
					i, read_value[i], i, expect_value);
				error_count++;
			}
			break;
		case CHECKSUM2_UNEQUAL:
			if (read_value[i] == expect_value) {
				LCD_KIT_ERR("lcd checksum error:"
					"read_value[%d] = 0x%x, expect_value[%d] = 0x%x, value equal is error\n",
					i, read_value[i], i, expect_value);
				error_count++;
			}
			break;
		default:
			LCD_KIT_ERR("lcd checksum judge_type invalid\n");
			error_count++;
			break;
		}
	}
	return error_count;
}

static void checksum2_compare(uint32_t panel_id, char expect_value[CHECKSUM2_VAL_NUM],
	unsigned char *read_value, int *count, int loop_num)
{
	if (!expect_value || !read_value || !count) {
		LCD_KIT_ERR("%s:invalid input param\n", __func__);
		return;
	}

	if (read_value[1] == expect_value[1]) {
		LCD_KIT_ERR("[WARING]loop: %u, read_value0 = 0x%x, except_value0 = 0x%x",
		loop_num, read_value[0], expect_value[0]);
		LCD_KIT_ERR("[WARING]loop: %u, read_value1 = 0x%x, except_value1 = 0x%x",
		loop_num, read_value[1], expect_value[1]);
		return;
	}
	if (read_value[0] == expect_value[0]) {
		LCD_KIT_INFO("loop: %u, read_value0 = 0x%x, except_value0 = 0x%x",
		loop_num, read_value[0], expect_value[0]);
		LCD_KIT_INFO("loop: %u, read_value1 = 0x%x, except_value1 = 0x%x",
		loop_num, read_value[1], expect_value[1]);
	} else {
		(*count)++;
		if (loop_num >= ARRAY_SIZE(FACT_INFO->check_sum2_detect.reg_val)) {
			LCD_KIT_ERR("loop_num is out of bounds");
			return;
		}
		FACT_INFO->check_sum2_detect.reg_val[loop_num][0] = read_value[0];
		FACT_INFO->check_sum2_detect.reg_val[loop_num][1] = read_value[1];
		LCD_KIT_ERR("[ERROR]loop: %u, read_value0 = 0x%x, except_value0 = 0x%x",
		loop_num, read_value[0], expect_value[0]);
		LCD_KIT_ERR("[ERROR]loop: %u, read_value1 = 0x%x, except_value1 = 0x%x",
		loop_num, read_value[1], expect_value[1]);
	}
}

static void check_read_check_sum2_reg_val(uint32_t panel_id,
	unsigned char *read_value, int *error_count, int loop_num)
{
	int i;
	unsigned int *detect_value = NULL;
	int detect_value_cnt;
	unsigned char expect_value[CHECKSUM2_VAL_NUM] = {0};
	struct lcd_kit_array_data config_data;

	if (loop_num >= ARRAY_SIZE(FACT_INFO->check_sum2_detect.value)) {
		LCD_KIT_ERR("loop_num is out of bounds");
		return;
	}
	config_data = FACT_INFO->check_sum2_detect.value[loop_num];
	detect_value = config_data.buf;
	detect_value_cnt = config_data.cnt;

	for (i = 0; i < CHECKSUM2_VAL_NUM && i < detect_value_cnt; i++)
		expect_value[i] = detect_value[i] & 0xFF;

 	if (FACT_INFO->check_sum2_detect.model_switch)
 		(*error_count) += checksum2_judge(read_value, config_data.buf, config_data.cnt);
 	else
		checksum2_compare(panel_id, expect_value, read_value, error_count, loop_num);
}

static int lcd_judge_check_sum2_detect(uint32_t panel_id, struct hisi_fb_data_type *hisifd,
	unsigned int loop_num, int *count)
{
	int ret;
	unsigned char read_value[CHECKSUM2_READ_COUNT + 1] = {0};
	unsigned long timeout;

	LCD_KIT_INFO("enter\n");
	if (loop_num >= ARRAY_SIZE(FACT_INFO->check_sum2_detect.time)) {
		LCD_KIT_ERR("loop_num is out of bounds");
		return LCD_KIT_FAIL;
	}
	timeout = jiffies + (CHECK_SUM2_LOOP_TIME * FACT_INFO->check_sum2_detect.time[loop_num] * HZ);
	if (!FACT_INFO->check_sum2_detect.rd_cmds[loop_num].cmds) {
		LCD_KIT_INFO("check sum2 detect read cmd is NULL\n");
		return LCD_KIT_FAIL;
	}
	msleep(FACT_INFO->check_sum2_detect.delay);
	do {
		if (!hisifd->panel_power_on) {
			LCD_KIT_ERR("panel is not power on!\n");
			return LCD_KIT_FAIL;
		}
		ret = lcd_kit_dsi_cmds_rx(panel_id, hisifd, read_value,
			CHECKSUM2_READ_COUNT, &FACT_INFO->check_sum2_detect.rd_cmds[loop_num]);
		if (ret) {
			LCD_KIT_ERR("checksum read dsi0 error\n");
			return LCD_KIT_FAIL;
		}
		check_read_check_sum2_reg_val(panel_id, read_value, count, loop_num);
		if (is_dual_mipi_panel(hisifd))
			LCD_KIT_INFO("dual_panel\n");
		msleep(CHECK_SUM2_LOOP_DELAY);
	} while (time_before(jiffies, timeout));

	return ret;
}

int lcd_check_sum2_detect(uint32_t panel_id, struct hisi_fb_data_type *hisifd)
{
	int ret;
	int count = 0;
	unsigned int loop_num;

	LCD_KIT_INFO("enter \n");
	loop_num = FACT_INFO->check_sum2_detect.loop_index;
	if (loop_num >= CHECK_SUM2_NUM) {
		LCD_KIT_ERR("pic index out of bound");
		return LCD_KIT_FAIL;
	}
	ret = lcd_judge_check_sum2_detect(panel_id, hisifd, loop_num, &count);
	if (ret)
		LCD_KIT_ERR("check sum2 detect judge fail\n");

	LCD_KIT_INFO("loop_num = %d, error_count = %d\n", loop_num, count);
	if (count >= FACT_INFO->check_sum2_detect.threshold) {
		ret = LCD_KIT_FAIL;
		lcd_kit_check_sum2_dmd_report(FACT_INFO->check_sum2_detect.reg_val,
			panel_id);
		/* set initial value */
		count = 0;
	}
	LCD_KIT_INFO("exit\n");
	return ret;
}

static int check_read_mbist_detect_reg_val(uint32_t panel_id, unsigned char *read_value,
	unsigned int pic_index, int loop)
{
	int i;
	unsigned char expect_value;
	unsigned int *detect_value = NULL;
	struct lcd_kit_array_data config_data;

	config_data = FACT_INFO->mbist_detect.value[pic_index];
	detect_value = config_data.buf;
	for (i = 0; i < config_data.cnt; i++) {
		/* obtains the value of the first byte */
		expect_value = detect_value[i] & 0xFF;
		LCD_KIT_INFO("pic: %u, read_val:%d = 0x%x, expect_val = 0x%x\n",
			pic_index, i, read_value[i], expect_value);
		if ((i < VAL_NUM) && (loop < MBIST_LOOPS))
			FACT_INFO->mbist_detect.reg_val[loop][pic_index][i] =
				read_value[i];
		if (read_value[i] != expect_value) {
			FACT_INFO->mbist_detect.err_flag[pic_index]++;
			LCD_KIT_ERR("[ERROR]pic: %u, read_val:%d = 0x%x, but expect_val = 0x%x\n",
				pic_index, i, read_value[i], expect_value);
			return LCD_KIT_FAIL;
		}
	}
	return LCD_KIT_OK;
}

static int lcd_judge_mbist_detect(int panel_id, struct hisi_fb_data_type *hisifd, unsigned int pic_index, int loop)
{
	int ret;
	unsigned char read_value[MAX_REG_READ_COUNT] = { 0 };
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	if (FACT_INFO->mbist_detect.rd_cmds[pic_index].cmds == NULL) {
		LCD_KIT_INFO("read mbist detect cmd is NULL\n");
		return LCD_KIT_FAIL;
	}
	/* delay 2s, or mbist reg value invalid */
	ssleep(2);
	if (!hisifd->panel_power_on) {
		LCD_KIT_ERR("panel is not power on!\n");
		return LCD_KIT_FAIL;
	}
	down(&hisifd->blank_sem);
	hisifb_activate_vsync(hisifd);
	ret = lcd_kit_dsi_cmds_rx(panel_id, hisifd, read_value,
		MAX_REG_READ_COUNT - 1, &FACT_INFO->mbist_detect.rd_cmds[pic_index]);
	hisifb_deactivate_vsync(hisifd);
	up(&hisifd->blank_sem);
	if (ret) {
		LCD_KIT_ERR("mbist detect read error\n");
		return ret;
	}
	ret = check_read_mbist_detect_reg_val(panel_id, read_value, pic_index, loop);
	return ret;
}

int lcd_mbist_detect_test(int panel_id, struct hisi_fb_data_type *hisifd)
{
	int i;
	int ret;
	static int count = 0;
	static int loop_num = 0;
	int err_record = 0;
	unsigned int pic_index;
	unsigned int *error_flag = NULL;

	pic_index = FACT_INFO->mbist_detect.pic_index;
	error_flag = FACT_INFO->mbist_detect.err_flag;
	if (pic_index >= DETECT_NUM) {
		LCD_KIT_ERR("pic number not support\n");
		return LCD_KIT_FAIL;
	}
	ret = lcd_judge_mbist_detect(panel_id, hisifd, pic_index, loop_num);
	if (ret)
		LCD_KIT_ERR("mbist detect judge fail\n");
	count++;
	if (count >= DETECT_NUM) {
		loop_num++;
		/* set initial value */
		count = 0;
	}
	LCD_KIT_INFO("count = %d, loop_num = %d\n", count, loop_num);
	if (loop_num >= MBIST_LOOPS) {
		for (i = 0; i < DETECT_NUM; i++) {
			LCD_KIT_INFO("pic : %d, err_num = %d\n",
				i, error_flag[i]);
			if (error_flag[i] >= FACT_INFO->mbist_detect.threshold)
				err_record++;
			/* set initial value */
			FACT_INFO->mbist_detect.err_flag[i] = 0;
		}
		if (err_record)
			lcd_kit_mbist_detect_dmd_report(FACT_INFO->mbist_detect.reg_val, panel_id);
		/* set initial value */
		loop_num = 0;
	}
	return ret;
}

int lcd_mbist_detect_set(int panel_id, struct hisi_fb_data_type *hisifd, unsigned int pic_index)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_dsi_panel_cmds *enter_cmds = NULL;

	pic_index = pic_index - DET_START;
	enter_cmds = FACT_INFO->mbist_detect.enter_cmds;
	if ((pic_index >= DET1_INDEX) && (pic_index < DETECT_NUM)) {
		if (enter_cmds[pic_index].cmds == NULL) {
			LCD_KIT_ERR("send mbist detect cmd is NULL\n");
			return LCD_KIT_FAIL;
		}
		down(&hisifd->blank_sem);
		hisifb_activate_vsync(hisifd);
		ret = lcd_kit_dsi_cmds_tx(panel_id, hisifd, &enter_cmds[pic_index]);
		hisifb_deactivate_vsync(hisifd);
		up(&hisifd->blank_sem);
		if (ret) {
			LCD_KIT_ERR("send mbist detect cmd error\n");
			return ret;
		}
		LCD_KIT_INFO("set picture : %u\n", pic_index);
		FACT_INFO->mbist_detect.pic_index = pic_index;
	} else {
		LCD_KIT_INFO("set picture : %u unknown\n", pic_index);
		FACT_INFO->mbist_detect.pic_index = INVALID_INDEX;
	}
	return ret;
}
