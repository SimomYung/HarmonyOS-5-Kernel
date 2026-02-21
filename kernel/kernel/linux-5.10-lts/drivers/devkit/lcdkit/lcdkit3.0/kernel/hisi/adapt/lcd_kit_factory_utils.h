/*
 * lcd_kit_factory_utils.h
 *
 * lcdkit header file of the production line test item
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

#ifndef __LCD_KIT_FACTORY_UTILS_H_
#define __LCD_KIT_FACTORY_UTILS_H_
#include "lcd_kit_factory.h"
#include "lcd_kit_power.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_parse.h"
#include "hisi_fb.h"
#include "lcd_kit_common.h"
#include "lcd_kit_ext_disp.h"

void checksum_stress_disable(struct hisi_fb_data_type *hisifd);
void checksum_stress_enable(struct hisi_fb_data_type *hisifd);
int lcd_kit_checksum_set(struct hisi_fb_data_type *hisifd, int pic_index);
void poweric_gpio_detect_dbc(struct hisi_fb_data_type *hisifd);
ssize_t lcd_amoled_cmds_pcd_errflag(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t lcd_amoled_gpio_pcd_errflag(struct device *dev, struct device_attribute *attr, char *buf);
int lcd_kit_pcd_detect(struct hisi_fb_data_type *hisifd, uint32_t val);
int lcd_kit_current_det(struct hisi_fb_data_type *hisifd);
int lcd_kit_lv_det(struct hisi_fb_data_type *hisifd);
int lcd_kit_current_det(struct hisi_fb_data_type *hisifd);
void ldo_transform(struct lcd_ldo *ldo);
int lcd_hor_line_test(struct hisi_fb_data_type *hisifd);
int lcd_vtc_line_test(struct hisi_fb_data_type *hisifd, unsigned long pic_index);
int lcd_get_avdd_gpio_grp_det_result(char *buf);
int process_vb_value(void);
void lcd_avdd_detect_data_report(uint16_t value);
int lcd_kit_oneside_setting(struct hisi_fb_data_type *hisifd, uint32_t mode);
int lcd_kit_aod_detect_setting(struct hisi_fb_data_type *hisifd, uint32_t mode);
void send_fps_lock_cmd(int panel_id, struct hisi_fb_data_type *hisifd, unsigned int val);
int lcd_check_sum2_detect_set(uint32_t panel_id, struct hisi_fb_data_type *hisifd, unsigned int loop_num);
int lcd_check_sum2_detect(uint32_t panel_id, struct hisi_fb_data_type *hisifd);
int lcd_mbist_detect_set(int panel_id, struct hisi_fb_data_type *hisifd, unsigned int pic_index);
int lcd_kit_checksum_check(struct hisi_fb_data_type *hisifd);
int lcd_mbist_detect_test(int panel_id, struct hisi_fb_data_type *hisifd);
#endif

