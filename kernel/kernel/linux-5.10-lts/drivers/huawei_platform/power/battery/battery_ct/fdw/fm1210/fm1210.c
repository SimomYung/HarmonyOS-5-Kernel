// SPDX-License-Identifier: GPL-2.0+
/*
 * fm1210.c
 *
 * fm1210 driver
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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

#include "fm1210.h"
#include <securec.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <uapi/linux/sched/types.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/compat.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/ctype.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/pm_qos.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/battery/battery_type_identify.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_devices_info.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <huawei_platform/power/power_mesg_srv.h>
#include "../../batt_aut_checker.h"
#include "../core/fm_ecdsa_api.h"
#include "fm1210_api.h"
#include "fm1210_swi.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG battct_fm1210
HWLOG_REGIST();

static bool fm1210_is_locked(unsigned char *content)
{
	if (!content || strstr(content, "unlocked")) {
		return false;
	}
	return true;
}

static int fm1210_cyclk_set(struct fm1210_dev *di)
{
	int ret;
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };

	ret = fm1210_operate_file(di, FM1210_LOCK_BLOCKNUM, FM1210_LOCK, NULL, 0);
	if (ret) {
		hwlog_err("[%s] lock fail or is locked, ic_index:%u\n", __func__, di->ic_index);
		return -EAGAIN;
	}
	ret = fm1210_operate_file(di, FM1210_LOCK_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check batt state fail, ic_index:%u\n", __func__, di->ic_index);
		return -EAGAIN;
	}

	if (!fm1210_is_locked(buf)) {
		hwlog_err("[%s] lock fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return 0;
}

static enum batt_ic_type fm1210_get_ic_type(void)
{
	hwlog_info("[%s] read type succ, type data is %d\n", __func__, FDW_FM1210_TYPE);
	return FDW_FM1210_TYPE;
}

static int fm1210_get_uid(struct platform_device *pdev,
	const unsigned char **uuid, unsigned int *uuid_len)
{
	struct fm1210_dev *di = NULL;
	int ret;

	if (!uuid || !uuid_len || !pdev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	di = platform_get_drvdata(pdev);
	if (!di)
		return -ENODEV;

	ret = fm1210_read_romid(di, di->mem.uid, FM1210_MAX_RETRY_COUNT, FM1210_CMD_RETRY_DELAY);
	if (ret) {
		hwlog_err("[%s] read uid error\n", __func__);
		return -EINVAL;
	}

	*uuid = di->mem.uid;
	*uuid_len = FM1210_UID_LEN;
	hwlog_info("[%s] read uid succ\n", __func__);

#ifdef ONEWIRE_STABILITY_DEBUG
	char* result = fm1210_print_data(di->mem.uid, FM1210_UID_LEN);
	if (result) {
		hwlog_info("[%s] read uid succ, uid data is %s\n", __func__, result);
		kfree(result);
	} else {
		hwlog_info("[%s] fm1210 print uid fail: %s\n", __func__);
	}
#endif
	return 0;
}

static int fm1210_get_batt_type(struct platform_device *pdev,
	const unsigned char **type, unsigned int *type_len)
{
	struct fm1210_dev *di = NULL;
	// type_temp stores the codes of the battery factory and cell factory
	unsigned char type_temp[FM1210_BATT_TYPE_LEN] = { 0 };

	if (!pdev || !type || !type_len) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	di = platform_get_drvdata(pdev);
	if (!di)
		return -ENODEV;

	if (fm1210_operate_file(di, FM1210_BATTTYPE_BLOCKNUM, FM1210_READ, type_temp, FM1210_BATTTYP_LEN)) {
		hwlog_err("[%s] read battery type err\n", __func__);
		return -EINVAL;
	}

	di->mem.batt_type[0] = type_temp[1];
	di->mem.batt_type[1] = type_temp[0];
	*type = di->mem.batt_type;
	*type_len = FM1210_BATTTYP_LEN;
	hwlog_info("[%s] read batttype succ ,Btp0:0x%x; Btp1:0x%x, ic_index:%u\n", __func__,
	di->mem.batt_type[0], di->mem.batt_type[1], di->ic_index);
	return 0;
}

static int fm1210_get_batt_sn(struct platform_device *pdev,
	struct power_genl_attr *res, const unsigned char **sn, unsigned int *sn_size)
{
	struct fm1210_dev *di = NULL;
	int ret;

	if (!pdev || !sn || !sn_size) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	(void)res;
	di = platform_get_drvdata(pdev);
	if (!di)
		return -EINVAL;
	if (!di->mem.sn_ready) {
		hwlog_info("[%s] read sn\n", __func__);
		memset_s(di->mem.sn, FM1210_SN_ASC_LEN, 0, FM1210_SN_ASC_LEN);
		ret = fm1210_operate_file(di, FM1210_SN_BLOCKNUM, FM1210_READ, di->mem.sn, FM1210_SN_ASC_LEN);
		if (ret) {
			hwlog_info("[%s] read sn error\n", __func__);
			di->mem.sn_ready = false;
			return -EINVAL;
		}
		di->mem.sn_ready = true;
	}
	hwlog_info("[%s] read sn succ\n", __func__);
#ifdef ONEWIRE_STABILITY_DEBUG
	hwlog_info("[%s] read sn succ, cache data is %x %x\n", __func__,
			di->mem.sn[0], di->mem.sn[1]);
#endif
	*sn = di->mem.sn;
	*sn_size = FM1210_SN_ASC_LEN;
	return 0;
}

static int fm1210_certification(struct platform_device *pdev,
	struct power_genl_attr *key_res, enum key_cr *result)
{
	int ret;
	struct fm1210_dev *di = NULL;

	if (!pdev || !key_res || !result) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -ENODEV;

	ret = fm1210_do_authentication(di, key_res, result, ECC_NORMAL);
	if (ret) {
		hwlog_err("[%s] ecce fail, ic_index:%u\n", __func__, di->ic_index);
		*result = KEY_FAIL_UNMATCH;
		return 0;
	}
	*result = KEY_PASS;
	return 0;
}

static int fm1210_ct_read(struct fm1210_dev *di)
{
	return fm1210_read_romid(di, di->mem.uid, FM1210_MAX_RETRY_COUNT, FM1210_CMD_RETRY_DELAY);
}

static int fm1210_act_read(struct fm1210_dev *di)
{
	int ret;
	uint16_t crc_act_read;
	uint16_t crc_act_cal;

	if (fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_READ, di->mem.act_sign, FM1210_ACT_LEN)) {
		hwlog_info("[%s] act_sig read error, ic_index:%u\n", __func__, di->ic_index);
		return -EAGAIN;
	}

	ret = memcpy_s((u8 *)&crc_act_read, FM1210_ACT_CRC_LEN, &di->mem.act_sign[FM1210_ACT_CRC_BYT0],
		FM1210_ACT_CRC_LEN);
	if (ret != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EAGAIN;
	}
	crc16_cal(di->mem.act_sign, (int)(di->mem.act_sign[0] + 1), &crc_act_cal);
	ret = (crc_act_cal != crc_act_read);
	if (ret) {
		hwlog_info("[%s] act_sig crc error, ic_index:%u\n", __func__, di->ic_index);
		return -EAGAIN;
	}
	hwlog_info("[%s] read act succ\n", __func__);
	return ret;
}

#ifndef BATTBD_FORCE_MATCH
static int fm1210_set_act_signature(struct platform_device *pdev,
	enum res_type type, const struct power_genl_attr *res)
{
	(void)pdev;
	(void)type;
	(void)res;
	hwlog_info("[%s] operation banned in user mode\n", __func__);
	return 0;
}
#else

static int fm1210_set_act_signature(struct platform_device *pdev,
	enum res_type type, const struct power_genl_attr *res)
{
	int ret;
	uint16_t crc_act;
	uint8_t act[FM1210_ACT_LEN] = { 0 };
	struct fm1210_dev *di = NULL;
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };

	if (!pdev || !res) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -ENODEV;
	if (res->len > FM1210_ACT_LEN) {
		hwlog_err("[%s] input act_sig oversize, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check lock state fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	if (fm1210_is_locked(buf)) {
		hwlog_err("[%s] cert locked, act set abandon, ic_index:%u\n", __func__, di->ic_index);
		return 0;
	}
	hwlog_info("[%s] start write act, ic_index:%u\n", __func__, di->ic_index);
	if ((memcpy_s(act, FM1210_ACT_LEN, res->data, res->len))!= EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	crc16_cal(act, res->len, &crc_act);
	if ((memcpy_s(act + sizeof(act) - sizeof(crc_act), sizeof(crc_act), (uint8_t *)&crc_act, sizeof(crc_act))) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	switch (type) {
	case RES_ACT:
		ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_WRITE, act, FM1210_ACT_LEN);
		if (ret) {
			hwlog_err("[%s] act write fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return 0;
}
#endif /* BATTBD_FORCE_MATCH */

static int fm1210_prepare(struct platform_device *pdev, enum res_type type,
	struct power_genl_attr *res)
{
	int ret;
	struct fm1210_dev *di = NULL;

	if (!pdev || !res) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	di = platform_get_drvdata(pdev);
	if (!di)
		return -ENODEV;

	switch (type) {
	case RES_CT:
		ret = fm1210_ct_read(di);
		if (ret) {
			hwlog_err("[%s] res_ct read fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		res->data = (const unsigned char *)di->mem.uid;
		res->len = FM1210_UID_LEN;
		break;
	case RES_ACT:
		ret = fm1210_act_read(di);
		if (ret) {
			hwlog_err("[%s] res_act read fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		res->data = (const unsigned char *)di->mem.act_sign;
		res->len = FM1210_ACT_LEN;
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
		res->data = NULL;
		res->len = 0;
	}
	return 0;
}

#ifndef BATTBD_FORCE_MATCH
static int fm1210_set_cert_ready(struct fm1210_dev *di)
{
	hwlog_info("[%s] operation banned in user mode, ic_index:%u\n", __func__, di->ic_index);
	return 0;
}
#else
static int fm1210_set_cert_ready(struct fm1210_dev *di)
{
	int ret;
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };

	ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check cert lock state fail, ic_index:%u\n", __func__, di->ic_index);
		return ret;
	}
	if (fm1210_is_locked(buf)) {
		hwlog_err("[%s] already set cert ready, ic_index:%u\n", __func__, di->ic_index);
		di->mem.cet_rdy = CERT_READY;
		return ret;
	}

	ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_LOCK, NULL, 0);
	if (ret) {
		hwlog_err("[%s] lock fail\n", __func__);
		return ret;
	}
	ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check cert lock state fail, ic_index:%u\n", __func__, di->ic_index);
		return ret;
	}
	if (!fm1210_is_locked(buf)) {
		hwlog_err("[%s] set_cert_ready fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	} else {
		di->mem.cet_rdy = CERT_READY;
		hwlog_err("[%s] set_cert_ready suc, ic_index:%u\n", __func__, di->ic_index);
	}

	return ret;
}
#endif /* BATTBD_FORCE_MATCH */

static int fm1210_set_batt_as_org(struct fm1210_dev *di)
{
	int ret;

	ret = fm1210_cyclk_set(di);
	if (ret)
		hwlog_err("[%s] set_batt_as_org fail, ic_index:%u\n", __func__, di->ic_index);
	else {
		di->mem.source = BATTERY_ORIGINAL;
		hwlog_info("[%s] set_batt_as_org suc, ic_index:%u\n", __func__, di->ic_index);
	}
	return ret;
}

static void fm1210_u64_to_byte_array(uint64_t data, uint8_t *arr)
{
	int i;

	for (i = 0; i < FM1210_BYTE_COUNT_PER_U64; ++i)
		arr[i] = (data >> (i * FM1210_BIT_COUNT_PER_BYTE));
}

/*
 * Note: arr length must be 8
 */
static void fm1210_byte_array_to_u64(uint64_t *data, uint8_t *arr)
{
	int i;

	if (!arr || !data) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return;
	}
	*data = 0;
	for (i = 0; i < FM1210_BYTE_COUNT_PER_U64; ++i)
		*data += (uint64_t)arr[i] << (i * FM1210_BIT_COUNT_PER_BYTE);
}

/*
 * Note: arr length must not be smaller than 16
 */
static int fm1210_hex_array_to_u64(uint64_t *data, uint8_t *arr)
{
	uint64_t val;
	int i;

	if (!arr || !data) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	*data = 0;
	for (i = 0; i < FM1210_HEX_COUNT_PER_U64; ++i) {
		if (arr[i] >= '0' && arr[i] <= '9') { /* number */
			val = arr[i] - '0';
		} else if (arr[i] >= 'a' && arr[i] <= 'f') { /* lowercase */
			val = arr[i] - 'a' + FM1210_HEX_NUMBER_BASE;
		} else if (arr[i] >= 'A' && arr[i] <= 'F') { /* uppercase */
			val = arr[i] - 'A' + FM1210_HEX_NUMBER_BASE;
		} else {
			hwlog_err("[%s] failed int arr[%d]=%d\n",
				__func__, i, arr[i]);
			*data = 0;
			return -EINVAL;
		}
		*data += val << ((FM1210_HEX_COUNT_PER_U64 - 1 - i) *
			FM1210_BIT_COUNT_PER_HEX);
	}
	return 0;
}

static int fm1210_write_group_sn(struct fm1210_dev *di, void *value)
{
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };
	uint8_t arr[FM1210_IC_GROUP_SN_LENGTH];
	int ret;

	if (!di || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check batt state fail, ic_index:%u\n", __func__, di->ic_index);
		return ret;
	}
	if (fm1210_is_locked(buf)) {
		hwlog_err("[%s] group sn already locked, ic_index:%u\n", __func__, di->ic_index);
		return 0;
	}

	fm1210_u64_to_byte_array(*((uint64_t *)value), arr);
	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_WRITE, arr, FM1210_IC_GROUP_SN_LENGTH);
	if (ret) {
		hwlog_err("[%s] write group sn fail, ic_index:%u\n", __func__, di->ic_index);
		return ret;
	}
	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_LOCK, NULL, 0);
	if (ret) {
		hwlog_err("[%s] lock fail or is locked, ic_index:%u\n", __func__, di->ic_index);
		return ret;
	}
	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check batt state fail, ic_index:%u\n", __func__, di->ic_index);
		return ret;
	}
	if (!fm1210_is_locked(buf)) {
		hwlog_err("[%s] lock fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	hwlog_info("[%s] write group_sn suc, ic_index:%u\n", __func__, di->ic_index);
	return 0;
}

static int fm1210_set_batt_org(struct fm1210_dev *di, void *value)
{
	int ret;
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };

	if (!di || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	ret = fm1210_operate_file(di, FM1210_LOCK_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check batt state fail, ic_index:%u\n", __func__, di->ic_index);
		return 0;
	}

	if (fm1210_is_locked(buf)) {
		hwlog_info("[%s] has been org(locked), quit work, ic_index:%u\n", __func__, di->ic_index);
		return 0;
	}

	if (*((enum batt_source *)value) == BATTERY_ORIGINAL) {
		ret = fm1210_set_batt_as_org(di);
		if (ret) {
			hwlog_err("[%s] set_batt_as_org fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
	}
	return 0;
}

static int fm1210_set_batt_safe_info(struct platform_device *pdev,
	enum batt_safe_info_t type, void *value)
{
	int ret;
	struct fm1210_dev *di = NULL;

	if (!pdev || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -EINVAL;

	switch (type) {
	case BATT_CHARGE_CYCLES:
		break;
	case BATT_SPARE_PART:
		ret = fm1210_set_batt_org(di, value);
		if (ret) {
			hwlog_err("[%s] set batt org fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_CERT_READY:
		ret = fm1210_set_cert_ready(di);
		if (ret) {
			hwlog_err("[%s] set_cert_ready fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_MATCH_INFO:
		ret = fm1210_write_group_sn(di, value);
		if (ret) {
			hwlog_err("[%s write group sn fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return 0;
}

static int get_batt_spare_part(struct fm1210_dev *di, void *value)
{
	int ret;
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };

	if (!di || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	ret = fm1210_operate_file(di, FM1210_LOCK_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check batt state fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}

	if (fm1210_is_locked(buf))
		*(enum batt_source *)value = BATTERY_ORIGINAL;
	else
		*(enum batt_source *)value = BATTERY_SPARE_PART;
	hwlog_info("[%s]batt source value is %d\n", __func__, *(enum batt_source *)value);
	return 0;
}

static int get_batt_cert_ready(struct fm1210_dev *di, void *value)
{
	int ret;
	unsigned char buf[FM1210_LOCK_BUF_LEN] = { 0 };

	if (!di || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_LOCK_STATUS, buf, FM1210_LOCK_BUF_LEN);
	if (ret) {
		hwlog_err("[%s] check cert lock state fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}

	if (!fm1210_is_locked(buf))
		hwlog_err("[%s] cert unready, ic_index:%u\n", __func__, di->ic_index);
	*(enum batt_cert_state *)value = CERT_READY;
	hwlog_info("[%s] cert ready, ic_index:%u\n", __func__, di->ic_index);
	return 0;
}

static int get_batt_match_info(struct fm1210_dev *di, void *value)
{
	int ret;
	uint64_t hash_group_sn = 0;
	uint8_t byte_group_sn[FM1210_IC_GROUP_SN_LENGTH] = { 0 };

	if (!di || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_READ, byte_group_sn, FM1210_IC_GROUP_SN_LENGTH);
	if (ret) {
		hwlog_err("[%s] read group sn fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}

	fm1210_byte_array_to_u64(&hash_group_sn, byte_group_sn);

	if (!hash_group_sn) {
		hwlog_err("[%s] hash_group_sn is null, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	ret = snprintf_s(di->mem.group_sn, FM1210_SN_LEN + 1, FM1210_SN_LEN,
		"%016llX", hash_group_sn);
	if (ret < 0) {
		hwlog_err("[%s] snprintf_s fail\n", __func__);
		return -EINVAL;
	}
	*(uint8_t **)value = di->mem.group_sn;
	hwlog_info("[%s] get group sn suc\n", __func__);
#ifdef ONEWIRE_STABILITY_DEBUG
	char* result = fm1210_print_data(di->mem.group_sn, FM1210_SN_LEN + 1);
	if (result) {
		hwlog_info("[%s] di->mem.group_sn is: %s\n", __func__, result);
		kfree(result);
	} else {
		hwlog_info("[%s] fm1210 print group_sn fail: %s\n", __func__);
	}
#endif
	return 0;
}

static int fm1210_get_batt_safe_info(struct platform_device *pdev,
	enum batt_safe_info_t type, void *value)
{
	int ret;
	struct fm1210_dev *di = NULL;

	if (!pdev || !value) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -EINVAL;

	switch (type) {
	case BATT_CHARGE_CYCLES:
		*(int *)value = BATT_INVALID_CYCLES;
		break;
	case BATT_SPARE_PART:
		ret = get_batt_spare_part(di, value);
		if (ret) {
			hwlog_err("[%s] check batt state fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_CERT_READY:
		ret = get_batt_cert_ready(di, value);
		if (ret) {
			hwlog_err("[%s] check cert lock state fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_MATCH_INFO:
		ret = get_batt_match_info(di, value);
		if (ret) {
			hwlog_err("[%s] read group sn fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return 0;
}

static int fm1210_get_batt_all_storage_info(struct fm1210_dev *di, struct batt_storage_info_data *data)
{
	int ret;
	int cycle_first_index;
	int cycle_second_index;
	u8 buf[BATT_STORAGE_INFO_LEN] = { 0 };

	if (!data || !data->activation || !data->cycle || !data->real_soh || !data->display_soh) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = fm1210_operate_file(di, FM1210_BATTSTORAGE_BLOCKNUM, FM1210_READ, buf, BATT_STORAGE_INFO_LEN);
	if (ret) {
		hwlog_err("[%s] read batt all state info from ic fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}

	cycle_first_index = BATT_CYCLE_OFFSET + BUF_FIRST_INDEX;
	cycle_second_index = BATT_CYCLE_OFFSET + BUF_SECOND_INDEX;
	data->activation->year = buf[BUF_FIRST_INDEX];
	data->activation->month = buf[BUF_SECOND_INDEX];
	*(data->cycle) = ((u16)buf[cycle_first_index] << BITS_PER_BYTE) | buf[cycle_second_index];
	*(data->real_soh) = buf[BATT_REAL_SOH_OFFSET + BUF_FIRST_INDEX];
	*(data->display_soh) = buf[BATT_DISPLAY_SOH_OFFSET + BUF_FIRST_INDEX];
	hwlog_info("[%s] read all_storage_info is year = %d  month = %d cycle = %d  realsoh = %d display_soh = %d",
		__func__, data->activation->year, data->activation->month, *(data->cycle),*(data->real_soh),
	*(data->display_soh));
	return 0;
}

static int fm1210_get_batt_storage_info(struct platform_device *pdev,
	struct batt_storage_info_data *data, enum batt_storage_info_type type)
{
	struct fm1210_dev *di = NULL;

	if (!pdev || !data) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -EINVAL;

	switch (type) {
	case BATT_ACTIVATION:
	case BATT_CYCLE:
	case BATT_REAL_SOH:
	case BATT_DISPLAY_SOH:
	case BATT_ALL_STORAGE_INFO:
		if (fm1210_get_batt_all_storage_info(di, data)) {
			hwlog_err("[%s] read %d fail, ic_index:%u\n", __func__, type, di->ic_index);
			return -EINVAL;
		}
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return 0;
}

static int fm1210_set_batt_all_storage_info(struct fm1210_dev *di, struct batt_storage_info_data *data)
{
	int ret;
	int cycle_first_index;
	int cycle_second_index;
	// buf[0] year buf[1] month buf[2-3] cycle buf[4] real_soh buf[5] display_soh others 0
	u8 buf[FM1210_PAGE_BLOCK_SIZE] = { 0 };

	if (!data || !data->activation || !data->cycle || !data->real_soh || !data->display_soh) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}

	cycle_first_index = BATT_CYCLE_OFFSET + BUF_FIRST_INDEX;
	cycle_second_index = BATT_CYCLE_OFFSET + BUF_SECOND_INDEX;
	buf[BUF_FIRST_INDEX] = data->activation->year;
	buf[BUF_SECOND_INDEX] = data->activation->month;
	buf[cycle_second_index] = (u8)(*(data->cycle) & BYTE_MASK);
	buf[cycle_first_index] = (u8)((*(data->cycle) >> BITS_PER_BYTE) & BYTE_MASK);
	buf[BATT_REAL_SOH_OFFSET + BUF_FIRST_INDEX] = *(data->real_soh);
	buf[BATT_DISPLAY_SOH_OFFSET + BUF_FIRST_INDEX] = *(data->display_soh);

	ret = fm1210_operate_file(di, FM1210_BATTSTORAGE_BLOCKNUM, FM1210_WRITE, buf, FM1210_PAGE_BLOCK_SIZE);
	if (ret) {
		hwlog_err("[%s] set_batt_all_storage from ic fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	hwlog_info("[%s] set all_storage_info is year=%d  month=%d cycleHighByte=%d cycleLowByte=%d "
	"realsoh=%d display_soh=%d",__func__,data->activation->year, data->activation->month,
	buf[cycle_first_index], buf[cycle_second_index], *(data->real_soh), *(data->display_soh));
	return 0;
}

// Callback function updates specific battery information fields
typedef void (*update_field_func)(struct batt_storage_info_data *data_read, struct batt_storage_info_data *data);

static int fm1210_set_batt_info_data(struct fm1210_dev *di, struct batt_storage_info_data *data,
								update_field_func update_field)
{
	int ret = 1;

	if (!data) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}

	struct batt_storage_info_data data_read = {
		.activation = kzalloc(sizeof(*data_read.activation), GFP_KERNEL),
		.cycle = kzalloc(sizeof(*data_read.cycle), GFP_KERNEL),
		.real_soh = kzalloc(sizeof(*data_read.real_soh), GFP_KERNEL),
		.display_soh = kzalloc(sizeof(*data_read.display_soh), GFP_KERNEL),
	};
	if (!data_read.activation || !data_read.cycle || !data_read.real_soh || !data_read.display_soh) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		goto err_free;
	}

	ret = fm1210_get_batt_all_storage_info(di, &data_read);
	if (ret) {
		hwlog_err("[%s] get_batt_all_storage from ic fail, ic_index:%u\n", __func__, di->ic_index);
		goto err_free;
	}

	update_field(&data_read, data);

	ret = fm1210_set_batt_all_storage_info(di, &data_read);
	if (ret) {
		hwlog_err("[%s] set info from ic fail, ic_index:%u\n", __func__, di->ic_index);
		goto err_free;
	}

err_free:
	kfree(data_read.activation);
	kfree(data_read.cycle);
	kfree(data_read.real_soh);
	kfree(data_read.display_soh);
	return ret;
}

static void update_activation_info(struct batt_storage_info_data *data_read, struct batt_storage_info_data *data)
{
	data_read->activation->year = data->activation->year;
	data_read->activation->month = data->activation->month;
}

static void update_cycle_info(struct batt_storage_info_data *data_read, struct batt_storage_info_data *data)
{
	*(data_read->cycle) = *(data->cycle);
}

static void update_real_soh_info(struct batt_storage_info_data *data_read, struct batt_storage_info_data *data)
{
	*(data_read->real_soh) = *(data->real_soh);
}

static void update_display_soh_info(struct batt_storage_info_data *data_read, struct batt_storage_info_data *data)
{
	*(data_read->display_soh) = *(data->display_soh);
}


static int fm1210_set_batt_activation(struct fm1210_dev *di, struct batt_storage_info_data *data)
{
	if (!data || !data->activation) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}
	return fm1210_set_batt_info_data(di, data, update_activation_info);
}

static int fm1210_set_batt_cycle(struct fm1210_dev *di, struct batt_storage_info_data *data)
{
	if (!data || !data->cycle) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}
	return fm1210_set_batt_info_data(di, data, update_cycle_info);
}

static int fm1210_set_batt_real_soh(struct fm1210_dev *di, struct batt_storage_info_data *data)
{
	if (!data || !data->real_soh) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}
	return fm1210_set_batt_info_data(di, data, update_real_soh_info);
}

static int fm1210_set_batt_display_soh(struct fm1210_dev *di, struct batt_storage_info_data *data)
{
	if (!data || !data->display_soh) {
		hwlog_err("[%s] data pointer NULL\n", __func__);
		return -EINVAL;
	}
	return fm1210_set_batt_info_data(di, data, update_display_soh_info);
}

static int fm1210_set_batt_storage_info(struct platform_device *pdev,
	struct batt_storage_info_data *data, enum batt_storage_info_type type)
{
	struct fm1210_dev *di = NULL;

	if (!pdev || !data) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -EINVAL;
	switch (type) {
	case BATT_ACTIVATION:
		if (fm1210_set_batt_activation(di, data)) {
			hwlog_err("[%s] write batt activation fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_CYCLE:
		if (fm1210_set_batt_cycle(di, data)) {
			hwlog_err("[%s] write batt cycle fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_REAL_SOH:
		if (fm1210_set_batt_real_soh(di, data)) {
			hwlog_err("[%s] write batt real soh fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_DISPLAY_SOH:
		if (fm1210_set_batt_display_soh(di, data)) {
			hwlog_err("[%s] write batt display soh fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	case BATT_ALL_STORAGE_INFO:
		if (fm1210_set_batt_all_storage_info(di, data)) {
			hwlog_err("[%s] write batt_all_storage_info fail, ic_index:%u\n", __func__, di->ic_index);
			return -EINVAL;
		}
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}

	return 0;
}

static void fm1210_set_ic_status(struct platform_device *pdev, int ic_status)
{
	struct fm1210_dev *di = NULL;

	if (!pdev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return;
	}
	di = platform_get_drvdata(pdev);
	if (!di) {
		hwlog_err("[%s] di pointer NULL\n", __func__);
		return;
	}
	switch (ic_status) {
	case BAT_TYPE_DEV_IN:
		fm1210_gpio_dir_input(di->onewire_gpio);
		break;
	case BAT_TYPE_DEV_LOW:
		fm1210_gpio_dir_output(di->onewire_gpio, LOW);
		break;
	default:
		hwlog_err("[%s] invalid option, ic_index:%u\n", __func__, di->ic_index);
	}
}

#ifdef BATTBD_FORCE_MATCH
static int fm1210_ecc_ex_check(struct platform_device *pdev,
	struct power_genl_attr *key_res)
{
	struct fm1210_dev *di = NULL;
	enum key_cr result = KEY_UNREADY;

	if (!pdev || !key_res) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	di = platform_get_drvdata(pdev);
	if (!di) {
		hwlog_err("[%s] di pointer NULL\n", __func__);
		return -ENODEV;
	}

	return fm1210_do_authentication(di, key_res, &result, ECC_EX);
}
#else
static int fm1210_ecc_ex_check(struct platform_device *pdev,
	struct power_genl_attr *key_res)
{
	hwlog_info("user mode prohibit this ops\n", __func__);
	return 0;
}
#endif /* BATTBD_FORCE_MATCH */

static int fm1210_ct_ops_register(struct platform_device *pdev,
	struct batt_ct_ops *bco)
{
	int ret;
	struct fm1210_dev *di = NULL;

	if (!bco || !pdev) {
		hwlog_err("[%s] : bco/pdev NULL\n", __func__);
		return -ENXIO;
	}
	di = platform_get_drvdata(pdev);
	if (!di)
		return -EINVAL;

	ret = fm1210_ic_ck(di);
	if (ret) {
		hwlog_err("[%s] : ic unmatch, ic_index:%u\n", __func__, di->ic_index);
		return -ENXIO;
	}

	hwlog_info("[%s] ic matched, ic_index:%u\n", __func__, di->ic_index);

	bco->get_ic_type = fm1210_get_ic_type;
	bco->get_ic_uuid = fm1210_get_uid;
	bco->get_batt_type = fm1210_get_batt_type;
	bco->get_batt_sn = fm1210_get_batt_sn;
	bco->certification = fm1210_certification;
	bco->set_act_signature = fm1210_set_act_signature;
	bco->prepare = fm1210_prepare;
	bco->set_batt_safe_info = fm1210_set_batt_safe_info;
	bco->get_batt_safe_info = fm1210_get_batt_safe_info;
	bco->set_batt_storage_info = fm1210_set_batt_storage_info;
	bco->get_batt_storage_info = fm1210_get_batt_storage_info;
	bco->set_ic_status = fm1210_set_ic_status;
	bco->rt_check = fm1210_ecc_ex_check;
	bco->power_down = NULL;
	return 0;
}

static int fm1210_get_group_sn(struct platform_device *pdev, uint8_t *group_sn)
{
	int ret;
	struct fm1210_dev *di = platform_get_drvdata(pdev);
	uint64_t hash_group_sn = 0;
	uint8_t byte_group_sn[FM1210_IC_GROUP_SN_LENGTH] = { 0 };

	if (!di)
		return -ENODEV;

	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_READ, byte_group_sn, FM1210_IC_GROUP_SN_LENGTH);
	if (ret) {
		hwlog_err("[%s] read group sn fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	fm1210_byte_array_to_u64(&hash_group_sn, byte_group_sn);
	ret = snprintf_s(group_sn, FM1210_SN_LEN + 1, FM1210_SN_LEN,
		"%016llX", hash_group_sn);
	if (ret < 0) {
		hwlog_err("[%s] snprintf_s fail\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static int fm1210_group_sn_write(struct platform_device *pdev,
	uint8_t *group_sn)
{
	int ret;

	if (!group_sn || !pdev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -ENODEV;
	}
	struct fm1210_dev *di = platform_get_drvdata(pdev);
	int8_t buf[FM1210_IC_GROUP_SN_LENGTH];
	uint64_t val = 0;
	int i;

	if (!di)
		return -ENODEV;

	ret = fm1210_hex_array_to_u64(&val, group_sn);
	if (ret) {
		hwlog_err("[%s] hex to u64 fail\n", __func__);
		return -EINVAL;
	}

	hwlog_info("[%s] val = %016llX\n", __func__, val);
	fm1210_u64_to_byte_array(val, buf);
	for (i = 0; i < FM1210_IC_GROUP_SN_LENGTH; ++i)
		hwlog_info("[%s] buf[%d] = %u\n", __func__, i, buf[i]);

	ret = fm1210_operate_file(di, FM1210_GROUPSN_BLOCKNUM, FM1210_WRITE, buf, FM1210_IC_GROUP_SN_LENGTH);
	if (ret) {
		hwlog_err("[%s] write group sn fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return 0;
}

static ssize_t ic_type_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	if (!buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "ic type: %d", fm1210_get_ic_type());
}

static ssize_t uid_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct fm1210_dev *di = NULL;
	int ret;
	uint8_t uid[FM1210_UID_LEN * 2 + 1] = { 0 };
	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}
	dev_get_drv_data(di, dev);
	if (!di)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_read_romid(di, di->mem.uid, FM1210_MAX_RETRY_COUNT, FM1210_CMD_RETRY_DELAY);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] read uid error\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read uid error");
	}
	fm1210_hex_to_str(uid, di->mem.uid, FM1210_UID_LEN);
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%s", uid);
}

static ssize_t batt_type_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct fm1210_dev *di = NULL;
	int ret;
	uint8_t type[FM1210_BATTTYP_LEN] = { 0 };
	uint8_t type_temp[FM1210_BATTTYP_LEN] = { 0 };

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	dev_get_drv_data(di, dev);
	if (!di)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_operate_file(di, FM1210_BATTTYPE_BLOCKNUM, FM1210_READ, type_temp, FM1210_BATTTYP_LEN);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] read battery type err\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read batt type error");
	}
	type[0] = type_temp[1];
	type[1] = type_temp[0];

	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "[%s] Btp0:0x%x; Btp1:0x%x, ic_index:%u\n",
			__func__, type[FM1210_PKVED_IND], type[FM1210_CELVED_IND], di->ic_index);
}

static ssize_t sn_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct fm1210_dev *di = NULL;
	int ret;
	uint8_t sn[FM1210_SN_ASC_LEN] = { 0 };

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	dev_get_drv_data(di, dev);
	if (!di)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_operate_file(di, FM1210_SN_BLOCKNUM, FM1210_READ, sn, FM1210_SN_ASC_LEN);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] batt type iqr fail\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "read sn error");
	}
	unsigned char sn_string[FM1210_SN_ASC_LEN + 1] = {0};
	for (int i = 0; i < FM1210_SN_ASC_LEN; i++) {
		sn_string[i] = sn[i];
	}
	sn_string[FM1210_SN_ASC_LEN] = '\0';
	hwlog_info("[%s] SN[ltoh]:%s, ic_index:%u\n", __func__, sn_string, di->ic_index);
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "SN [ltoh]: %s\n", sn_string);
}

static ssize_t group_sn_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct fm1210_dev *di = NULL;
	int ret;
	uint8_t group_sn[FM1210_SN_LEN + 1] = { 0 };
	struct platform_device *pdev = NULL;

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_get_group_sn(pdev, group_sn);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] get group sn fail\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "get group sn  failed");
	}
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "group sn: %s", group_sn);
}

static ssize_t group_sn_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	struct fm1210_dev *di;
	uint8_t byte_group_sn[FM1210_IC_GROUP_SN_LENGTH] = { 0 };
	struct platform_device *pdev = NULL;

	if (!dev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev || count > FM1210_IC_GROUP_SN_LENGTH)
		return -EINVAL;

	ret = memcpy_s(byte_group_sn, FM1210_IC_GROUP_SN_LENGTH, buf, count);
	if (ret != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_group_sn_write(pdev, byte_group_sn);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] write group sn fail\n", __func__);
		return -EINVAL;
	}
	return count;
}

static ssize_t actsig_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int ret;
	struct fm1210_dev *di = NULL;
	uint8_t act[FM1210_ACT_LEN * 2 + 1];

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	dev_get_drv_data(di, dev);
	if (!di)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_act_read(di);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] res_act read fail, ic_index:%u\n", __func__, di->ic_index);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "get actsig failed");
	}
	fm1210_hex_to_str(act, di->mem.act_sign, FM1210_ACT_LEN);
	return scnprintf(buf, PAGE_SIZE, "Act = %s\n", act);
}

static ssize_t actsig_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	uint8_t act_data[FM1210_ACT_LEN];
	struct power_genl_attr res;
	enum res_type type = RES_ACT;
	struct fm1210_dev *di;
	struct platform_device *pdev = NULL;

	if (!dev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev || count > FM1210_ACT_LEN) {
		hwlog_err("[%s] data error\n", __func__);
		return -EINVAL;
	}

	ret = memcpy_s(act_data, count, buf, count);
	if (ret != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	res.data = act_data;
	res.len = count;
	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_set_act_signature(pdev, type, &res);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] act set fail\n", __func__);
		return -EINVAL;
	}
	return count;
}

static ssize_t battcyc_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int ret;
	enum batt_safe_info_t type = BATT_CHARGE_CYCLES;
	int batt_cyc;
	struct fm1210_dev *di;
	struct platform_device *pdev = NULL;

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev)
		return -EINVAL;

	ret = fm1210_get_batt_safe_info(pdev, type, (void *)&batt_cyc);
	if (ret) {
		hwlog_err("[%s] batt_cyc get fail\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "get battcyc failed");
	}

	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "battcyc: %d", batt_cyc);
}

static ssize_t battcyc_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	enum batt_safe_info_t type = BATT_CHARGE_CYCLES;
	long cyc_dcr;
	struct fm1210_dev *di;
	struct platform_device *pdev = NULL;

	if (!dev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev)
		return -EINVAL;

	if (kstrtol(buf, POWER_BASE_DEC, &cyc_dcr) < 0 ||
			cyc_dcr < 0 || cyc_dcr > FM1210_CYC_MAX) {
		hwlog_err("[%s] : val is not valid!, ic_index:%u\n",
				__func__, di->ic_index);
		return -EINVAL;
	}
	ret = fm1210_set_batt_safe_info(pdev, type, (void *)&cyc_dcr);
	if (ret) {
		hwlog_err("[%s] batt_cyc dcr %d fail\n", __func__, cyc_dcr);
		return  -EINVAL;
	}
	return count;
}

static ssize_t org_spar_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int ret;
	enum batt_safe_info_t type = BATT_SPARE_PART;
	enum batt_source batt_spar;
	struct fm1210_dev *di;
	struct platform_device *pdev = NULL;

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "di or pdev is null");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_get_batt_safe_info(pdev, type, (void *)&batt_spar);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] batt spar check %d fail\n", __func__, ret);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "batt spar check failed");
	}
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "batt: %d", batt_spar);
}

static ssize_t org_spar_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	long val;
	enum batt_safe_info_t type = BATT_SPARE_PART;
	struct fm1210_dev *di;
	struct platform_device *pdev = NULL;

	if (!dev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev)
		return -EINVAL;

	if (kstrtol(buf, POWER_BASE_DEC, &val) < 0) {
		hwlog_err("[%s] : val is not valid!, ic_index:%u\n",
			__func__, di->ic_index);
		return -EINVAL;
	}
	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_set_batt_safe_info(pdev, type, (void *)&val);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] org set %d fail\n", __func__, ret);
		return -EINVAL;
	}
	return count;
}

static ssize_t cert_status_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int ret;
	struct fm1210_dev *di;
	uint8_t cert_status_buf[FM1210_LOCK_BUF_LEN] = { 0 };

	if (!dev || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "Error data");
	}

	dev_get_drv_data(di, dev);
	if (!di)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "di is null");

	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_operate_file(di, FM1210_ACT_BLOCKNUM, FM1210_LOCK_STATUS, cert_status_buf, FM1210_LOCK_BUF_LEN);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] check lock state fail, ic_index:%u\n", __func__, di->ic_index);
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "check lock state fail");
	}
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "cert status: %s", cert_status_buf);
}

static ssize_t cert_status_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	long val = 0;
	struct fm1210_dev *di;
	enum batt_safe_info_t type = BATT_CERT_READY;
	struct platform_device *pdev = NULL;

	if (!dev) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	pdev = container_of(dev, struct platform_device, dev);
	dev_get_drv_data(di, dev);
	if (!di || !pdev)
		return -EINVAL;

	if (kstrtol(buf, POWER_BASE_DEC, &val) < 0 || val != 1) {
			hwlog_err("[%s] : val is not valid!, ic_index:%u\n",
			__func__, di->ic_index);
			return -EINVAL;
	}
	bat_type_apply_mode(BAT_ID_SN);
	ret = fm1210_set_batt_safe_info(pdev, type, (void *)&val);
	bat_type_release_mode(true);
	if (ret) {
		hwlog_err("[%s] lock cert fail, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	return count;
}

static const DEVICE_ATTR_RO(ic_type);
static const DEVICE_ATTR_RO(uid);
static const DEVICE_ATTR_RO(batt_type);
static const DEVICE_ATTR_RO(sn);

#ifdef ONEWIRE_STABILITY_DEBUG
static const DEVICE_ATTR_RW(actsig);
static const DEVICE_ATTR_RW(battcyc);
static const DEVICE_ATTR_RW(org_spar);
static const DEVICE_ATTR_RW(cert_status);
static const DEVICE_ATTR_RW(group_sn);
#endif /* BATTERY_LIMIT_DEBUG */

static const struct attribute *batt_checker_attrs[] = {
	&dev_attr_ic_type.attr,
	&dev_attr_uid.attr,
	&dev_attr_batt_type.attr,
	&dev_attr_sn.attr,
#ifdef ONEWIRE_STABILITY_DEBUG
	&dev_attr_actsig.attr,
	&dev_attr_battcyc.attr,
	&dev_attr_org_spar.attr,
	&dev_attr_cert_status.attr,
	&dev_attr_group_sn.attr,
#endif /* BATTERY_LIMIT_DEBUG */
	NULL, /* sysfs_create_files need last one be NULL */
};

static int fm1210_sysfs_create(struct platform_device *pdev)
{
	if (!pdev) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}
	if (sysfs_create_files(&pdev->dev.kobj, batt_checker_attrs)) {
		hwlog_err("[%s] Can't create all expected nodes under %s\n",
			  __func__, pdev->dev.kobj.name);
		return BATTERY_DRIVER_FAIL;
	}

	return BATTERY_DRIVER_SUCCESS;
}

static void fm1210_parse_protocol(struct fm1210_dev *di, struct device_node *np)
{
	if (!di || !np) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return;
	}
	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_reset_start_delay", &di->fm1210_swi.ow_reset_start_delay, FM1210_DEFAULT_RESET_START_DELAY))
		hwlog_err("[%s] : ow_reset_start_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_reset_sample_delay", &di->fm1210_swi.ow_reset_sample_delay, FM1210_DEFAULT_RESET_SAMPLE_DELAY))
		hwlog_err("[%s] : ow_reset_sample_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_reset_end_delay", &di->fm1210_swi.ow_reset_end_delay, FM1210_DEFAULT_RESET_END_DELAY))
		hwlog_err("[%s] : ow_reset_end_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_write_start_delay", &di->fm1210_swi.ow_write_start_delay, FM1210_DEFAULT_WRITE_START_DELAY))
		hwlog_err("[%s] : oow_write_start_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_write_low_delay", &di->fm1210_swi.ow_write_low_delay, FM1210_DEFAULT_WRITE_LOW_DELAY))
		hwlog_err("[%s] : ow_write_low_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_write_high_delay", &di->fm1210_swi.ow_write_high_delay, FM1210_DEFAULT_WRITE_HIGH_DELAY))
		hwlog_err("[%s] : ow_write_high_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_write_end_delay", &di->fm1210_swi.ow_write_end_delay, FM1210_DEFAULT_WRITE_END_DELAY))
		hwlog_err("[%s] : ow_write_end_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_read_start_delay", &di->fm1210_swi.ow_read_start_delay, FM1210_DEFAULT_READ_START_DELAY))
		hwlog_err("[%s] : ow_read_start_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_read_sample_delay", &di->fm1210_swi.ow_read_sample_delay, FM1210_DEFAULT_READ_SAMPLE_DELAY))
		hwlog_err("[%s] : ow_read_sample_delay not given in dts\n", __func__);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ow_read_end_delay", &di->fm1210_swi.ow_read_end_delay, FM1210_DEFAULT_READ_END_DELAY))
		hwlog_err("[%s] : ow_read_end_delay not given in dts\n", __func__);
}

static int fm1210_parse_dts(struct fm1210_dev *di, struct device_node *np)
{
	int ret;
	if (!di || !np) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "ic_index", &di->ic_index);
	if (ret) {
		hwlog_err("[%s] : ic_index not given in dts\n", __func__);
		di->ic_index = 0;
	}
	hwlog_info("[%s] ic_index = 0x%x\n", __func__, di->ic_index);

	ret = of_property_read_u16(np, "cyc_full", &di->cyc_full);
	if (ret) {
		hwlog_err("[%s] : cyc_full not given in dts, ic_index:%u\n",
			__func__, di->ic_index);
		di->cyc_full = 0;
	}
	hwlog_info("[%s] cyc_full = 0x%x, ic_index:%u\n", __func__,
		di->cyc_full, di->ic_index);

	fm1210_parse_protocol(di, np);
	return 0;
}

static int fm1210_gpio_init(struct platform_device *pdev,
	struct fm1210_dev *di)
{
	int ret;
	if (!pdev || !di) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	di->onewire_gpio = of_get_named_gpio(pdev->dev.of_node,
		"onewire-gpio", 0);

	if (!gpio_is_valid(di->onewire_gpio)) {
		hwlog_err("[%s] onewire_gpio is not valid, ic_index:%u\n", __func__, di->ic_index);
		return -EINVAL;
	}
	/* get the gpio */
	ret = devm_gpio_request(&pdev->dev, di->onewire_gpio, "onewire_fm1210");
	if (ret) {
		hwlog_err("[%s] gpio request failed %d, ic_index:%u\n",
			__func__, di->onewire_gpio, di->ic_index);
		return ret;
	}
	gpio_direction_input(di->onewire_gpio);

	return 0;
}

static int fm1210_ic_check(struct fm1210_dev *di)
{
	int retry;
	int ret;

	if (!di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	bat_type_apply_mode(BAT_ID_SN);
	for (retry = 0; retry < FM1210_MAX_RETRY_COUNT; retry++) {
		ret = fm1210_ic_ck(di);
		if (!ret)
			break;
	}
	bat_type_release_mode(true);
	hwlog_info("[%s] ic check ret = %d, ic_index:%u\n", __func__, ret, di->ic_index);
	return ret;
}

static int fm1210_probe(struct platform_device *pdev)
{
	int ret;
	struct fm1210_dev *di = NULL;
	struct device_node *np = NULL;
	struct power_devices_info_data *power_dev_info = NULL;

	hwlog_info("[%s] probe begin\n", __func__);

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	di->dev = &pdev->dev;
	np = pdev->dev.of_node;

	ret = fm1210_parse_dts(di, np);
	if (ret) {
		hwlog_err("[%s] dts parse fail, ic_index:%u\n", __func__, di->ic_index);
		goto probe_fail;
	}

	ret = fm1210_gpio_init(pdev, di);
	if (ret) {
		hwlog_err("[%s] gpio init fail, ic_index:%u\n", __func__, di->ic_index);
		goto probe_fail;
	}

	ret = fm1210_ic_check(di);
	if (ret)
		goto ic_ck_fail;

	di->reg_node.ic_memory_release = NULL;
	di->reg_node.ic_dev = pdev;
	di->reg_node.ct_ops_register = fm1210_ct_ops_register;
	add_to_aut_ic_list(&di->reg_node);

	ret = fm1210_sysfs_create(pdev);
	if (ret)
		hwlog_err("[%s] : sysfs create fail, ic_index:%u\n", __func__, di->ic_index);

	power_dev_info = power_devices_info_register();
	if (power_dev_info) {
		power_dev_info->dev_name = pdev->dev.driver->name;
		power_dev_info->dev_id = 0;
		power_dev_info->ver_id = 0;
	}

	platform_set_drvdata(pdev, di);

	hwlog_info("[%s] success!\n", __func__);
	return 0;

ic_ck_fail:
	devm_gpio_free(&pdev->dev, di->onewire_gpio);
probe_fail:
	hwlog_info("[%s] probe_fail\n", __func__);
	kfree(di);
	return ret;
}

static int fm1210_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id ow_dt_match[] = {
	{
		.compatible = "fdw,fm1210",
		.data = NULL,
	},
	{},
};

static struct platform_driver fm1210_driver = {
	.probe			= fm1210_probe,
	.remove			= fm1210_remove,
	.driver			= {
		.owner		= THIS_MODULE,
		.name		= "fm1210",
		.of_match_table = ow_dt_match,
	},
};

static int __init fm1210_init(void)
{
	return platform_driver_register(&fm1210_driver);
}

static void __exit fm1210_exit(void)
{
	platform_driver_unregister(&fm1210_driver);
}

subsys_initcall_sync(fm1210_init);
module_exit(fm1210_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("fm1210 ic");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
