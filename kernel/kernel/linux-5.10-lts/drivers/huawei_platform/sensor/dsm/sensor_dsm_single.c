// SPDX-License-Identifier: GPL-2.0
/*
 * sensor_dsm.c
 *
 * dsm for sensor module
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
#include <huawei_platform/sensor/sensor_dsm_single.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <dfx/hiview_hisysevent.h>
#include <dsm/dsm_pub.h>
#include <huawei_platform/log/log_exception.h>
#include <huawei_platform/log/hw_log.h>

#include <securec.h>

#define HWLOG_TAG sensor_dsm_single
HWLOG_REGIST();

static LIST_HEAD(dmd_map_list);
static DEFINE_SPINLOCK(g_sensor_dsm_list_lock);

static struct hisysevent_error_no_map sensor_dsm_map[] = {
	{ DSM_HALL_ERROR_NO, SENSOR_DSM_LEVEL_STATISTIC, "HALL", "SENSOR_HALL_ERROR", "CONTENT" },
	{ DSM_AP_ERR_COLORSENSOR_DETECT, SENSOR_DSM_LEVEL_STATISTIC, "COLOR", "SENSOR_COLOR_CONNET_ERROR", "CONTENT" },
	{ DSM_PMIC_VIBRATOR_IRQ_OCP_NO, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_OCP_ERR", "CONTENT" },
	{ DSM_PMIC_VIBRATOR_IRQ_OUT_NO, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_INIT_ERR", "CONTENT" },
	{ DSM_PMIC_VIBRATOR_IRQ_UNDERVOL_NO, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_UVP_ERR", "CONTENT" },
	{ DSM_PMIC_VIBRATOR_IRQ_OVERVOL_NO, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_OVP_ERR", "CONTENT" },
	{ DSM_PMIC_VIBRATOR_IRQ_ADC_NO, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_ADC_ERR", "CONTENT" },
	{ DSM_PMIC_VIBRATOR_FREQ_CHECK_NO, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_F0_ERR", "CONTENT" },
	{ DSM_SHB_VIBRATOR_OTP_ERROR, SENSOR_DSM_LEVEL_STATISTIC, "VIBRATOR", "SENSOR_HAPTIC_OVER_TEM_ERR", "CONTENT" },
};

static int sensor_dsm_single_domain_check(const char *domain)
{
	return (!strncmp(domain, "VIBRATOR", SENSOR_DSM_BUF_SIZE_0016) ||
		!strncmp(domain, "SENSOR", SENSOR_DSM_BUF_SIZE_0016) ||
		!strncmp(domain, "HALL", SENSOR_DSM_BUF_SIZE_0016) ||
		!strncmp(domain, "COLOR", SENSOR_DSM_BUF_SIZE_0016));
}

static struct sensor_dsm_err_map *sensor_dsm_single_map_register(const char *domain)
{
	struct sensor_dsm_err_map *node = NULL;
	struct sensor_dsm_err_map *temp = NULL;
	unsigned long flags;
	bool found = false;

	if (!domain)
		return NULL;

	if (!sensor_dsm_single_domain_check(domain)) {
		hwlog_err("unable to report other field's event:%s\n", domain);
		return NULL;
	}

	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if (node == NULL) {
		hwlog_err("unable to report other field's event:%s\n", domain);
		return NULL;
	}

	spin_lock_irqsave(&g_sensor_dsm_list_lock, flags);
	list_for_each_entry(temp, &dmd_map_list, map_node) {
		if (!strncmp(domain, temp->map[0].domain, SENSOR_DSM_BUF_SIZE_0016)) {
			hwlog_err("domain : %s is already registered\n", domain);
			found = true;
			goto out;
		}
	}

	list_add_tail(&node->map_node, &dmd_map_list);

	hwlog_err("domain : %s is registered\n", domain);
out:
	spin_unlock_irqrestore(&g_sensor_dsm_list_lock, flags);
	if (found) {
		kfree(node);
		node = NULL;
	}
	return node;
}

int sensor_dsm_single_report_hiview(const char *domain, const char *error_name, const char *key, const char *value, unsigned int err_level)
{
	int ret;
	struct hiview_hisysevent *hi_event = NULL;

	if (!error_name || !key || !value || !domain)
		return -EINVAL;

	hi_event = hisysevent_create(domain, error_name, err_level);
	if (!hi_event) {
		hwlog_err("%s create hievent fail\n", __func__);
		return -EPERM;
	}

	hisysevent_put_string(hi_event, key, value);
	ret = hisysevent_write(hi_event);
	if (ret < 0) {
		hwlog_err("%s hievent report fail\n", __func__);
		hisysevent_destroy(&hi_event);
		return -EPERM;
	}

	hisysevent_destroy(&hi_event);
	hwlog_info("%s domain=%s err_no=%s key=%s value=%s level=%d\n",
		__func__, domain, error_name, key, value, err_level);
	return SENSOR_DSM_OK;
}

int sensor_dsm_single_convert_dsm_to_hisysevent(int err_no, int *dsm_error_level, char *dsm_domain, char *name, char *key)
{
	int ret = SENSOR_DSM_ERR;
	int i;
	struct sensor_dsm_err_map *temp = NULL;
	unsigned long flags;

	spin_lock_irqsave(&g_sensor_dsm_list_lock, flags);
	list_for_each_entry(temp, &dmd_map_list, map_node) {
		for (i = 0; i < temp->event_num; i++) {
			if (err_no == temp->map[i].error_no) {
				*dsm_error_level = temp->map[i].level;
				ret = strncpy_s(name, SENSOR_DSM_BUF_SIZE_0032, temp->map[i].name, strlen(temp->map[i].name));
				ret += strncpy_s(dsm_domain, SENSOR_DSM_BUF_SIZE_0016, temp->map[i].domain, strlen(temp->map[i].domain));
				ret += strncpy_s(key, SENSOR_DSM_BUF_SIZE_0048, temp->map[i].key, strlen(temp->map[i].key));
				hwlog_err("%s event match: level %d name %s domain %s key %s,\
					 map level %d map name %s, map domain %s map key %s, ret %d\n", __func__,
					*dsm_error_level, name, dsm_domain, key,
					temp->map[i].level, temp->map[i].name, temp->map[i].domain, temp->map[i].key, ret);
				break;
			}
		}
	}
	spin_unlock_irqrestore(&g_sensor_dsm_list_lock, flags);
	return ret;
}

int sensor_dsm_report_hiview(unsigned int err_no, const char *key, const char *value)
{
	int dsm_error_level;
	char dsm_domain[SENSOR_DSM_BUF_SIZE_0016] = { '\0' };
	char event_name[SENSOR_DSM_BUF_SIZE_0032] = { '\0' };
	char event_key[SENSOR_DSM_BUF_SIZE_0048] = { '\0' };

	if (sensor_dsm_single_convert_dsm_to_hisysevent(err_no, &dsm_error_level, dsm_domain, event_name, event_key) == SENSOR_DSM_OK) {
		sensor_dsm_single_report_hiview(dsm_domain, event_name, key, value, dsm_error_level);
		return SENSOR_DSM_OK;
	}

	return SENSOR_DSM_ERR;
}

int sensor_dsm_single_register(char *domain)
{
	if (domain == NULL)
		return -1;
	struct sensor_dsm_err_map *sensor_err_map = NULL;
	int i;
	sensor_err_map = sensor_dsm_single_map_register(domain);
	if (sensor_err_map) {
		sensor_err_map->event_num = sizeof(sensor_dsm_map) / sizeof(sensor_dsm_map[0]);
		for (i = 0; i < sensor_err_map->event_num; i++) {
			sensor_err_map->map[i].error_no = sensor_dsm_map[i].error_no;
			sensor_err_map->map[i].level = sensor_dsm_map[i].level;
			(void)strncpy_s(sensor_err_map->map[i].domain, SENSOR_DSM_BUF_SIZE_0016,
				sensor_dsm_map[i].domain, strlen(sensor_dsm_map[i].domain));
			(void)strncpy_s(sensor_err_map->map[i].name, SENSOR_DSM_BUF_SIZE_0032,
				sensor_dsm_map[i].name, strlen(sensor_dsm_map[i].name));
			(void)strncpy_s(sensor_err_map->map[i].key, SENSOR_DSM_BUF_SIZE_0048,
				sensor_dsm_map[i].key, strlen(sensor_dsm_map[i].key));
		}
	}
	return 0;
}
EXPORT_SYMBOL(sensor_dsm_single_register);
