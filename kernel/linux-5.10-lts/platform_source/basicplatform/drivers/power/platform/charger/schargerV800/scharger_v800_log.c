/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: charger log for schargerV800.
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/regmap.h>
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_sysfs.h>

#include "scharger_v800_buck.h"
#include "scharger_v800_dc.h"
#include "scharger_v800_adc.h"

#define REG_PD_NUM          0x7E
#define REG_PD_BASE_ADDR    0x0

#define REG_GLB_NUM         0xB7
#define REG_GLB_BASE_ADDR   0x300
/*
 * REG_GLB range: 0x00-0x05, 0x10-0x1B, 0x20-0x3C, 0x60-0x6E,
 * 0x71-0x83, 0x8E-0x91, 0x9E-0x9F, 0xB0-0xB7
 */
static const int reg_glb_range[][2] = {{0x00, 0x05}, {0x10, 0x1B}, {0x20, 0x3C},
									   {0x60, 0x6E}, {0x71, 0x83}, {0x8E, 0x91},
									   {0x9E, 0x9F}, {0xB0, 0xB7}};

#define REG_IRQ_NUM         0x61
#define REG_IRQ_BASE_ADDR   0x800
/* REG_IRQ range: 0x0-0x0F, 0x20-0x2F, 0x40-0x4E, 0x60-0x61 */
static const int reg_irq_range[][2] = {{0x00, 0x0F}, {0x20, 0x2F}, {0x40, 0x4E},
									   {0x60, 0x61}};

#define REG_ANA_NUM         0xFD
#define REG_ANA_BASE_ADDR   0x900
/*
 * REG_ANA range: 0x00-0x16, 0x20, 0x30-0x50, 0x56-0x7E,
 * 0x90-0x99, 0x9C-0xAE, 0xB0-0xB7, 0xB9-0xCF, 0xD4-0xD9,
 * 0xDA-0xEB, 0xF0-0xF8, 0xFA-0xFD
 */
static const int reg_ana_range[][2] = {{0x00, 0x16}, {0x20, 0x20}, {0x30, 0x50},
									   {0x56, 0x7E}, {0x90, 0x99}, {0x9C, 0xAE},
									   {0xB0, 0xB7}, {0xB9, 0xCF}, {0xD4, 0xD9},
									   {0xDA, 0xEB}, {0xF0, 0xF8}, {0xFA, 0xFD}};

struct schargerlog_adc_data {
	int ibus;
	int vbus;
	int ibat;
	int vusb;
	int vbat;
	int tdie;
	int tsbat;
	int vpsw;
	int vsysh;
};

struct schargerlog_data {
	struct schargerlog_adc_data adc_data;
	enum charger_mode chg_mode;
};

static void show_reg_head(char *buf, size_t str_len)
{
	int ret = 0;
	int i, j, start, end, num;
	size_t len = 0;

	num = sizeof(reg_glb_range) / sizeof(reg_glb_range[0]);
	for (i = 0; i < num; i++) {
		start = reg_glb_range[i][0];
		end = reg_glb_range[i][1];
		for (j = start; j <= end; j++) {
			len = strlen(buf);
			ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%x  ",
					 REG_GLB_BASE_ADDR + j);
			if (ret < 0) {
				scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
				return;
			}
		}
	}

	num = sizeof(reg_irq_range) / sizeof(reg_irq_range[0]);
	for (i = 0; i < num; i++) {
		start = reg_irq_range[i][0];
		end = reg_irq_range[i][1];
		for (j = start; j <= end; j++) {
			len = strlen(buf);
			ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%x  ",
					 REG_IRQ_BASE_ADDR + j);
			if (ret < 0) {
				scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
				return;
			}
		}
	}

	num = sizeof(reg_ana_range) / sizeof(reg_ana_range[0]);
	for (i = 0; i < num; i++) {
		start = reg_ana_range[i][0];
		end = reg_ana_range[i][1];
		for (j = start; j <= end; j++) {
			len = strlen(buf);
			ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%x  ",
					 REG_ANA_BASE_ADDR + j);
			if (ret < 0) {
				scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
				return;
			}
		}
	}

	for (i = 0; i <= REG_PD_NUM; i++) {
		len = strlen(buf);
		ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%-3x  ",
				 REG_PD_BASE_ADDR + i);
		if (ret < 0) {
			scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
			return;
		}
	}
}

static ssize_t get_scharger_head(char *buf, int str_len)
{
	int ret;

	ret = snprintf_s(buf, str_len, str_len - 1, "%s",
		 "Mode     Ibus   Vbus   Ibat   Vusb   Vbat  Tdie  TsBat  Vpsw  Vsysh  ");
	if (ret < 0) {
		scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
		return 0;
	}

	show_reg_head(buf, str_len);

	return strlen(buf);
}

static enum charger_mode get_charge_mode(void)
{
	struct current_charger_mode *charger_mode;

	charger_mode = scharger_v800_get_current_charge_mode();
	if(charger_mode == NULL) {
		scharger_err("%s charger_mode is NULL!\n", __func__);
		return -1;
	}
	return charger_mode->chg_mode;
}

static void get_adc_data(struct schargerlog_adc_data *adc_data)
{
	adc_data->ibus = scharger_v800_get_ibus();
	adc_data->vbus = scharger_v800_get_vbus();
	adc_data->ibat = scharger_v800_get_ibat();
	adc_data->vusb = scharger_v800_get_vusb();
	adc_data->vbat = scharger_v800_get_vbat();
	adc_data->tdie = scharger_v800_get_tdie();
	adc_data->tsbat = scharger_v800_get_tsbat();
	adc_data->vpsw = scharger_v800_get_vpsw();
	adc_data->vsysh = scharger_v800_get_vsysh();
}

static void show_chg_mode_schargerlog(char *buf, int str_len,
	enum charger_mode chg_mode)
{
	size_t len = strlen(buf);
	int ret;

	if (chg_mode == BUCK5V)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "BUCK5V   ");
	else if (chg_mode == BUCK9V)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "BUCK9V   ");
	else if (chg_mode == SC11)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "SC11     ");
	else if (chg_mode == SC21)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "SC21     ");
	else if (chg_mode == SC41)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "SC41     ");
	else if (chg_mode == RSC11)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "RSC11    ");
	else if (chg_mode == RSC12)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "RSC12    ");
	else if (chg_mode == RSC14)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "RSC14    ");
	else if (chg_mode == OTG5V)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "OTG5V    ");
	else if (chg_mode == OTG9V)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "OTG9V    ");
	else if (chg_mode == WDRC12)
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "WDRC12   ");
	else
		ret = snprintf_s(buf + len, str_len - len, str_len - len - 1, "UNKNOWN  ");
	if (ret <= 0)
		scharger_err("[%s]snprintf_s fail, %s\n", __func__, buf);
}

static void show_adc_schargerlog(char *buf, int str_len, struct schargerlog_adc_data *adc_data)
{
	size_t len = strlen(buf);
	int ret;

	ret = snprintf_s(buf + len, str_len - len, str_len -len - 1,
		"%-5d  %-5d  %-5d  %-5d  %-4d  %-4d  %-5d  %-4d  %-5d  ",
		 adc_data->ibus, adc_data->vbus, adc_data->ibat, adc_data->vusb,
		 adc_data->vbat, adc_data->tdie, adc_data->tsbat, adc_data->vpsw, adc_data->vsysh);
	if (ret < 0) {
		scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
		return;
	}
}

static void _show_schargerlog(char *buf, int str_len, struct schargerlog_data *chglog_data)
{
	show_chg_mode_schargerlog(buf, str_len, chglog_data->chg_mode);
	show_adc_schargerlog(buf, str_len, &chglog_data->adc_data);
}

static void show_reg_value_schargerlog(char *buf, int str_len, struct scharger_buck *di)
{
	int ret = 0;
	int i, j, start, end, num;
	size_t len = 0;
	unsigned int val = 0;

	num = sizeof(reg_glb_range) / sizeof(reg_glb_range[0]);
	for (i = 0; i < num; i++) {
		start = reg_glb_range[i][0];
		end = reg_glb_range[i][1];
		for (j = start; j <= end; j++) {
			regmap_read(di->regmap, REG_GLB_BASE_ADDR + j, &val);
			len = strlen(buf);
			ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%-3x  ", val);
			if (ret < 0) {
				scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
				return;
			}
		}
	}

	num = sizeof(reg_irq_range) / sizeof(reg_irq_range[0]);
	for (i = 0; i < num; i++) {
		start = reg_irq_range[i][0];
		end = reg_irq_range[i][1];
		for (j = start; j <= end; j++) {
			regmap_read(di->regmap, REG_IRQ_BASE_ADDR + j, &val);
			len = strlen(buf);
			ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%-3x  ", val);
			if (ret < 0) {
				scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
				return;
			}
		}
	}

	num = sizeof(reg_ana_range) / sizeof(reg_ana_range[0]);
	for (i = 0; i < num; i++) {
		start = reg_ana_range[i][0];
		end = reg_ana_range[i][1];
		for (j = start; j <= end; j++) {
			regmap_read(di->regmap, REG_ANA_BASE_ADDR + j, &val);
			len = strlen(buf);
			ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%-3x  ", val);
			if (ret < 0) {
				scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
				return;
			}
		}
	}

	for (i = 0; i <= REG_PD_NUM; i++) {
		regmap_read(di->regmap, REG_PD_BASE_ADDR + i, &val);
		len = strlen(buf);
		ret = snprintf_s(buf + len, str_len - len, str_len - len -1, "0x%-3x  ", val);
		if (ret < 0) {
			scharger_err("(%s)snprintf_s fail ret = %d\n", __func__, ret);
			return;
		}
	}
}

static ssize_t show_schargerlog(char *buf, int size, struct scharger_buck *di)
{
	struct schargerlog_data chglog_data = {0};
	int str_len = size - 1;

	get_adc_data(&chglog_data.adc_data);
	chglog_data.chg_mode = get_charge_mode();

	_show_schargerlog(buf, str_len, &chglog_data);

	show_reg_value_schargerlog(buf, str_len, di);

	return strlen(buf);
}

int scharger_v800_get_register_head(char *buf, int size, void *dev_data)
{
	int str_len = size - 1;
	int err;

	err = memset_s(buf, size, 0, size);
	if (err != EOK)
		scharger_err("[%s]memset_s fail, err=%d\n", __func__, err);
	get_scharger_head(buf, str_len);

	return 0;
}

int scharger_v800_dump_register(char *buf, int size, void *dev_data)
{
	int err;

	err = memset_s(buf, size, 0, size);
	if (err != EOK)
		scharger_err("[%s]memset_s fail, err=%d\n", __func__, err);
	show_schargerlog(buf, size, dev_data);

	return 0;
}