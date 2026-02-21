/*
 * batt_info_interface.c
 *
 * battery information interface
 *
 * Copyright (c) 2012-2025 Huawei Technologies Co., Ltd.
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

#include "batt_info_interface.h"
#include <huawei_platform/power/batt_info_pub.h>
#include "securec.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG batt_info_interface
HWLOG_REGIST();

int get_battery_sn_by_id(unsigned int index, char *sn_buf, unsigned int buf_size, unsigned int *sn_len)
{
	struct batt_chk_data *checker_data = NULL;

	if (!sn_buf || !sn_len)
		return -ENXIO;

	if (get_checker_data_by_id(index, &checker_data) || !checker_data ||
		!checker_data->bco.get_batt_sn) {
		hwlog_err("[%s] get checker_data fail\n", __func__);
		return -ENOENT;
	}

	if (!checker_data->sn) {
		if (checker_data->bco.get_batt_sn(checker_data, 0,
				&checker_data->sn, &checker_data->sn_len)) {
			hwlog_err("get battery sn failed in %s\n", __func__);
			return -EINVAL;
		}
	}

	if (buf_size < checker_data->sn_len) {
		hwlog_err("[%s] buff is too small\n", __func__);
		return -EOVERFLOW;
	}

	if (memcpy_s(sn_buf, buf_size, checker_data->sn, checker_data->sn_len))
		return -ENXIO;

	*sn_len = checker_data->sn_len;

	return 0;
}

int get_battery_identifier(char *buf, int buf_size, int identifier_index, int identifier_len)
{
	struct batt_checker_entry *temp = NULL;
	struct platform_device *pdev = NULL;
	struct batt_chk_data *checker_data = NULL;
	struct list_head *batt_checkers_head = NULL;
	int len = 0;
	int buf_size_temp = buf_size;
	errno_t rc;

	if (!buf || identifier_index < 0 || identifier_len <= 0)
		return -ENXIO;

	batt_checkers_head = get_batt_checkers_head();
	if (!batt_checkers_head) {
		hwlog_err("%s: batt_checkers_head is NULL\n", __func__);
		return -ENOTSUPP;
	}

	list_for_each_entry(temp, batt_checkers_head, node) {
		pdev = temp->pdev;
		checker_data = platform_get_drvdata(pdev);
		if (!checker_data->sn) {
			if (!checker_data->bco.get_batt_sn) {
				hwlog_err("%s: bco.get_batt_sn NULL\n", __func__);
				return -ENOTSUPP;
			}
			if (checker_data->bco.get_batt_sn(checker_data, 0,
				&checker_data->sn, &checker_data->sn_len)) {
				hwlog_err("get battery sn failed in %s\n", __func__);
				return -EINVAL;
			}
		}

		if ((identifier_index + identifier_len) > checker_data->sn_len) {
			hwlog_err("battery identifier index is too big in %s\n",  __func__);
			return -EOVERFLOW;
		}

		/* cur_len + identifier_len + '\n' + '\0' */
		if ((len + identifier_len + 2) > buf_size_temp) {
			hwlog_err("[%s] buff is too small\n", __func__);
			return -EOVERFLOW;
		}

		rc = memcpy_s(buf + len, buf_size_temp, checker_data->sn + identifier_index, identifier_len);
		if (rc != EOK)
			return -EINVAL;

		buf[identifier_len + len] = '\n';
		len += identifier_len + 1;
		buf_size_temp -= len;
	}

	if (!len) {
		hwlog_err("[%s] batt_ct is not supported\n", __func__);
		return -EUNATCH;
	}

	return 0;
}

int get_battery_backup_id(char *buf, int buf_size)
{
	/* content len + '\n' + '\0' */
	if (get_battery_identifier(buf, buf_size, BACKUP_ID_OFFSET, 1)) {
		hwlog_err("%s get sn backup id failed.\n", __func__);
		return -EINVAL;
	}

	return 0;
}

int get_battery_type(unsigned char *name, unsigned int name_size)
{
	struct batt_checker_entry *temp = NULL;
	struct platform_device *pdev = NULL;
	struct batt_chk_data *checker_data = NULL;
	struct list_head *batt_checkers_head = NULL;
	const unsigned char *batt_type = NULL;
	unsigned int len = 0;

	batt_checkers_head = get_batt_checkers_head();
	if (!batt_checkers_head) {
		hwlog_err("%s: batt_checkers_head is NULL\n", __func__);
		return -ENOTSUPP;
	}

	list_for_each_entry(temp, batt_checkers_head, node) {
		pdev = temp->pdev;
		checker_data = platform_get_drvdata(pdev);
		if (checker_data->bco.get_batt_type) {
			if (checker_data->bco.get_batt_type(checker_data,
				&batt_type, &len)) {
				hwlog_err("Get battery type error in %s\n",
					__func__);
				return -ENOTSUPP;
			}
			if (len >= name_size) {
				hwlog_err("buffer is too small found in %s\n",
					__func__);
				return -EINVAL;
			}
			if (memcpy_s(name, name_size, batt_type, len)) {
				hwlog_err("[%s] memcpy_s failed\n", __func__);
				return -EINVAL;
			}
			name[len] = 0;
			hwlog_info("battery type is %s\n", name);
			return 0;
		}
		return -EINVAL;
	}

	return -EINVAL;
}

int get_battery_type_by_id(unsigned int index, char *name, unsigned int name_size)
{
	struct batt_chk_data *checker_data = NULL;
	const char *batt_type = NULL;
	unsigned int batt_type_len = 0;
	int ret;

	if (!name)
		return -ENXIO;

	if (get_checker_data_by_id(index, &checker_data) || !checker_data ||
		!checker_data->bco.get_batt_type_extend) {
		hwlog_err("[%s] get checker_data fail\n", __func__);
		return -ENOENT;
	}

	if (checker_data->bco.get_batt_type_extend(checker_data,
		&batt_type, &batt_type_len)) {
		hwlog_err("%s get extend battery type error\n", __func__);
			return -EINVAL;
	}

	if (batt_type_len >= name_size) {
		hwlog_err("%s buffer is too small\n", __func__);
		return -EINVAL;
	}

	ret = memcpy_s(name, name_size, batt_type, batt_type_len);
	if (ret != EOK) {
		hwlog_err("%s memcpy_s failed\n", __func__);
		return -ENXIO;
	}
	name[batt_type_len] = 0;

	hwlog_info("%s battery type is %s\n", __func__, name);
	return 0;
}

int get_battery_activation_info_by_id(unsigned int index, struct activation_info *activation)
{
	struct batt_chk_data *checker_data = NULL;

	if (!activation)
		return -EINVAL;

	if (get_checker_data_by_id(index, &checker_data) || !checker_data ||
		!checker_data->bco.get_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (!checker_data->storage_info.activation.initialized) {
		if (checker_data->bco.get_batt_storage_info(checker_data, &(checker_data->storage_info.data),
			BATT_ACTIVATION)) {
			hwlog_err("[%s] get batt activation info failed\n", __func__);
			return -EINVAL;
		}

		checker_data->storage_info.activation.initialized = true;
		hwlog_info("[%s] get storage info from IC\n", __func__);
	}

	activation->year = checker_data->storage_info.activation.data.year;
	activation->month = checker_data->storage_info.activation.data.month;

	return 0;
}

int set_battery_activation_info_by_id(unsigned int index, struct activation_info activation)
{
	struct batt_chk_data *checker_data = NULL;

	if (get_checker_data_by_id(index, &checker_data) || !checker_data ||
		!checker_data->bco.set_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (activation.year > BATT_ACTIVATION_YEAR_MAX || activation.month < BATT_ACTIVATION_MONTH_MIN ||
		activation.month > BATT_ACTIVATION_MONTH_MAX) {
		hwlog_err("[%s] year:%hhu or month:%hhu is illegal\n",
			__func__, activation.year, activation.month);
		return -EINVAL;
	}

	checker_data->storage_info.activation.data.year = activation.year;
	checker_data->storage_info.activation.data.month = activation.month;
	if (checker_data->bco.set_batt_storage_info(checker_data, &(checker_data->storage_info.data),
		BATT_ACTIVATION)) {
		hwlog_err("[%s] set batt activation info failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

int get_battery_cycle_by_id(unsigned int index, u16 *cycle)
{
	struct batt_chk_data *checker_data = NULL;

	if (!cycle)
		return -EINVAL;

	if (get_checker_data_by_id(index, &checker_data) || !checker_data ||
		!checker_data->bco.get_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (!checker_data->storage_info.cycle.initialized) {
		if (checker_data->bco.get_batt_storage_info(checker_data, &(checker_data->storage_info.data),
			BATT_CYCLE)) {
			hwlog_err("[%s] get batt cycle failed\n", __func__);
			return -EINVAL;
		}
		checker_data->storage_info.cycle.initialized = true;
		hwlog_info("[%s] get storage info from IC\n", __func__);
	}

	*cycle = checker_data->storage_info.cycle.data;

	return 0;
}

static int set_batt_storage_cycle(struct batt_chk_data *checker_data)
{
	if (!checker_data || !checker_data->bco.set_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (checker_data->bco.set_batt_storage_info(checker_data, &(checker_data->storage_info.data),
		BATT_CYCLE)) {
		hwlog_err("[%s] set batt cycle failed\n", __func__);
		return -EINVAL;
	}

	hwlog_info("[%s] set batt cycle:%hu to IC\n", __func__, checker_data->storage_info.cycle.data);
	return 0;
}

void update_battery_cycle(struct batt_chk_data *checker_data)
{
	if (!checker_data) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return;
	}

	mutex_lock(&(checker_data->storage_info.cycle.update_lock));
	checker_data->storage_info.cycle.data = checker_data->storage_info.cycle.update_data;
	if (set_batt_storage_cycle(checker_data)) {
		mutex_unlock(&(checker_data->storage_info.cycle.update_lock));
		hwlog_err("[%s] update battery cycle failed\n", __func__);
		return;
	}

	checker_data->storage_info.cycle.update_flag = false;
	mutex_unlock(&(checker_data->storage_info.cycle.update_lock));
}

int set_battery_cycle_by_id(unsigned int index, u16 cycle)
{
	struct batt_chk_data *checker_data = NULL;
	struct batt_info *drv_data = get_batt_info();

	if (!drv_data) {
		hwlog_err("[%s] drv_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (cycle > BATT_CYCLE_MAX) {
		hwlog_err("[%s] cycle:%hu is illegal\n", __func__, cycle);
		return -EINVAL;
	}

	if (get_checker_data_by_id(index, &checker_data) || !checker_data) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (cycle == checker_data->storage_info.cycle.data) {
		hwlog_info("[%s] cycle:%hu is equal to storaged cycle\n", __func__, cycle);
		return 0;
	}

	if (drv_data->delayed_update_enabled) {
		mutex_lock(&(checker_data->storage_info.cycle.update_lock));
		checker_data->storage_info.cycle.update_data = cycle;
		checker_data->storage_info.cycle.update_flag = true;
		mutex_unlock(&(checker_data->storage_info.cycle.update_lock));
		schedule_delayed_work(&drv_data->delayed_update_work,
			msecs_to_jiffies(BLANK_UPDATE_WAIT_INTERVAL));
		return 0;
	}

	checker_data->storage_info.cycle.data = cycle;
	if (set_batt_storage_cycle(checker_data)) {
		hwlog_err("[%s] set batt storaged cycle failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

int get_battery_real_soh_by_id(unsigned int index, u8 *real_soh)
{
	struct batt_chk_data *checker_data = NULL;

	if (!real_soh)
		return -EINVAL;

	if (get_checker_data_by_id(index, &checker_data) || !checker_data ||
		!checker_data->bco.get_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (!checker_data->storage_info.real_soh.initialized) {
		if (checker_data->bco.get_batt_storage_info(checker_data,
			&(checker_data->storage_info.data), BATT_REAL_SOH)) {
			hwlog_err("[%s] get batt real soh failed\n", __func__);
			return -EINVAL;
		}
		checker_data->storage_info.real_soh.initialized = true;
		hwlog_info("[%s] get storage info from IC\n", __func__);
	}

	*real_soh = checker_data->storage_info.real_soh.data;

	return 0;
}

static int set_batt_storage_real_soh(struct batt_chk_data *checker_data)
{
	if (!checker_data || !checker_data->bco.set_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (checker_data->bco.set_batt_storage_info(checker_data, &(checker_data->storage_info.data),
		BATT_REAL_SOH)) {
		hwlog_err("[%s] set batt real soh failed\n", __func__);
		return -EINVAL;
	}

	hwlog_info("[%s] set batt real soh:%hhu to IC\n", __func__,
		checker_data->storage_info.real_soh.data);
	return 0;
}

void update_battery_real_soh(struct batt_chk_data *checker_data)
{
	if (!checker_data) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return;
	}

	mutex_lock(&(checker_data->storage_info.real_soh.update_lock));
	checker_data->storage_info.real_soh.data = checker_data->storage_info.real_soh.update_data;
	if (set_batt_storage_real_soh(checker_data)) {
		mutex_unlock(&(checker_data->storage_info.real_soh.update_lock));
		hwlog_err("[%s] update battery real soh failed\n", __func__);
		return;
	}

	checker_data->storage_info.real_soh.update_flag = false;
	mutex_unlock(&checker_data->storage_info.real_soh.update_lock);
}

int set_battery_real_soh_by_id(unsigned int index, u8 real_soh)
{
	struct batt_chk_data *checker_data = NULL;
	struct batt_info *drv_data = get_batt_info();

	if (!drv_data) {
		hwlog_err("[%s] drv_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (real_soh > BATT_SOH_MAX) {
		hwlog_err("[%s] real_soh:%hhu is illegal\n", __func__, real_soh);
		return -EINVAL;
	}

	if (get_checker_data_by_id(index, &checker_data) || !checker_data) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (drv_data->delayed_update_enabled) {
		mutex_lock(&(checker_data->storage_info.real_soh.update_lock));
		checker_data->storage_info.real_soh.update_data = real_soh;
		checker_data->storage_info.real_soh.update_flag = true;
		mutex_unlock(&(checker_data->storage_info.real_soh.update_lock));
		schedule_delayed_work(&drv_data->delayed_update_work,
			msecs_to_jiffies(BLANK_UPDATE_WAIT_INTERVAL));
		return 0;
	}
	
	checker_data->storage_info.real_soh.data = real_soh;
	if (set_batt_storage_real_soh(checker_data)) {
		hwlog_err("[%s] set batt storaged real_soh failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

int get_battery_display_soh(u8 *display_soh)
{
	struct batt_chk_data *checker_data = NULL;

	if (!display_soh)
		return -EINVAL;

	if (get_checker_data_by_id(0, &checker_data) || !checker_data ||
		!checker_data->bco.get_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (!checker_data->storage_info.display_soh.initialized) {
		if (checker_data->bco.get_batt_storage_info(checker_data,
			&(checker_data->storage_info.data), BATT_DISPLAY_SOH)) {
			hwlog_err("[%s] get batt display soh failed\n", __func__);
			return -EINVAL;
		}
		checker_data->storage_info.display_soh.initialized = true;
		hwlog_info("[%s] get storage info from IC\n", __func__);
	}

	*display_soh = checker_data->storage_info.display_soh.data;

	return 0;
}

static int set_batt_storage_display_soh(struct batt_chk_data *checker_data)
{
	if (!checker_data || !checker_data->bco.set_batt_storage_info) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (checker_data->bco.set_batt_storage_info(checker_data, &(checker_data->storage_info.data),
		BATT_DISPLAY_SOH)) {
		hwlog_err("[%s] set batt display soh failed\n", __func__);
		return -EINVAL;
	}

	hwlog_info("[%s] set batt display soh:%hhu to IC\n", __func__,
		checker_data->storage_info.display_soh.data);
	return 0;
}

void update_battery_display_soh(struct batt_chk_data *checker_data)
{
	if (!checker_data) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return;
	}

	mutex_lock(&(checker_data->storage_info.display_soh.update_lock));
	checker_data->storage_info.display_soh.data = checker_data->storage_info.display_soh.update_data;
	if (set_batt_storage_display_soh(checker_data)) {
		mutex_unlock(&(checker_data->storage_info.display_soh.update_lock));
		hwlog_err("[%s] update battery display soh failed\n", __func__);
		return;
	}

	checker_data->storage_info.display_soh.update_flag = false;
	mutex_unlock(&(checker_data->storage_info.display_soh.update_lock));
}

int set_battery_display_soh(u8 display_soh)
{
	struct batt_chk_data *checker_data = NULL;
	struct batt_info *drv_data = get_batt_info();

	if (!drv_data) {
		hwlog_err("[%s] drv_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (display_soh > BATT_SOH_MAX) {
		hwlog_err("[%s] display_soh:%hhu is illegal\n", __func__, display_soh);
		return -EINVAL;
	}

	if (get_checker_data_by_id(0, &checker_data) || !checker_data) {
		hwlog_err("[%s] checker_data is NULL\n", __func__);
		return -ENOENT;
	}

	if (drv_data->delayed_update_enabled) {
		mutex_lock(&(checker_data->storage_info.display_soh.update_lock));
		checker_data->storage_info.display_soh.update_data = display_soh;
		checker_data->storage_info.display_soh.update_flag = true;
		mutex_unlock(&(checker_data->storage_info.display_soh.update_lock));
		schedule_delayed_work(&drv_data->delayed_update_work,
			msecs_to_jiffies(BLANK_UPDATE_WAIT_INTERVAL));
		return 0;
	}

	checker_data->storage_info.display_soh.data = display_soh;
	if (set_batt_storage_display_soh(checker_data)) {
		hwlog_err("[%s] set batt storaged display_soh failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}
