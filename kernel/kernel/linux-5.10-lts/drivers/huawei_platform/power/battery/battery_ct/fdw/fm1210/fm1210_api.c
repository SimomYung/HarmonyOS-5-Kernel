// SPDX-License-Identifier: GPL-2.0+
/*
 * fm1210_api.c
 *
 * interface for fm1210.c
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

#include "fm1210_api.h"
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
#include <huawei_platform/power/power_mesg_srv.h>
#include "../../batt_aut_checker.h"
#include "../core/fm_ecdsa_api.h"
#include "fm1210.h"
#include "fm1210_swi.h"

#define HASH_LENGTH             32
#define SIG_LENGTH              64
#define ACCESS_ALLOWED	        0x01
#define ENC_CLEARTEXT	        0x01
/* time measurement setting */

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG fm1210_API
HWLOG_REGIST();

static u8 g_ecc_sig_res[FM1210_ECC_SIG_SIZE];

struct fm1210_file {
	uint8_t file_block;
	uint8_t block_num;
};

static const struct fm1210_file g_file_block[] = {
	{FM1210_UID_BLOCKNUM, FM1210_UID_BLOCKLEN},
	{FM1210_ACT_BLOCKNUM, FM1210_ACT_BLOCKLEN},
	{FM1210_BATTTYPE_BLOCKNUM, FM1210_BATTTYPE_BLOCKLEN},
	{FM1210_SN_BLOCKNUM, FM1210_SN_BLOCKLEN},
	{FM1210_GROUPSN_BLOCKNUM, FM1210_GROUPSN_BLOCKLEN},
	{FM1210_LOCK_BLOCKNUM, FM1210_LOCK_BLOCKLEN},
	{FM1210_BATTSTORAGE_BLOCKNUM, FM1210_BATTSTORAGE_BLOCKLEN},
};

struct fm1210_comm {
	u8 comm_cmd;
	u16 delay_ms;
};

static const struct fm1210_comm g_cmd_comm[] = {
	{FM1210_GET_CERT, FM1210_CMD_READ_BIN_DELAY},
	{FM1210_GET_RAND, FM1210_CMD_READ_BIN_DELAY},
	{FM1210_GET_ECC, FM1210_CMD_ECC_SIGN_DELAY},
	{FM1210_READ_CMD, FM1210_CMD_READ_BLOCK_DELAY},
	{FM1210_WRITE_CMD, FM1210_CMD_WRITE_BLOCK_DELAY},
	{FM1210_ACTIVATE_EE_CMD, FM1210_CMD_EE_PUMP_DELAY},
	{FM1210_LOCKSTA_CMD, FM1210_CMD_READ_BLOCK_DELAY},
	{FM1210_LOCK_CMD, FM1210_CMD_LOCK_BLOCK_DELAY},
};

struct lock_status {
	u8	read_access : 2;    // '01'=可读 | 其他值 = 不可读
	u8	write_access : 2;   // '01'=可写 | 其他值 = 不可写
	u8	enc_type : 2;       // '01'=支持普通或安全访问模式 | 其他值 = 只支持安全访问模式
	u8	key_type : 2;       // '11'=通用加密 | '01'=用户指定加密 | '00' = 制造商加密
};

static void set_sched_affinity_to_current(void)
{
	long retval;
	int current_cpu;

	preempt_disable();
	current_cpu = smp_processor_id();
	preempt_enable();
	retval = sched_setaffinity(0, cpumask_of(current_cpu));
	if (retval)
		hwlog_info("[%s] Set cpu af to current cpu failed %ld\n", __func__, retval);
	else
		hwlog_info("[%s] Set cpu af to current cpu %d\n", __func__, current_cpu);
}

static void set_sched_affinity_to_all(void)
{
	long retval;
	cpumask_t dstp;

	cpumask_setall(&dstp);
	retval = sched_setaffinity(0, &dstp);
	if (retval)
		hwlog_info("[%s] Set cpu af to all valid cpus failed %ld\n", __func__, retval);
	else
		hwlog_info("[%s] Set cpu af to all valid cpus\n", __func__);
}

void fm1210_power_on(struct fm1210_dev *di)
{
	if (!di) {
		hwlog_err("[%s] di is null\n", __func__);
		return;
	}
	gpio_direction_output(di->onewire_gpio, FM1210_HIGH_VOLTAGE);
	mdelay(FM1210_POWER_ON_DELAY);
	gpio_set_value(di->onewire_gpio, FM1210_HIGH_VOLTAGE);
}

void fm1210_power_off(struct fm1210_dev *di)
{
	if (!di) {
		hwlog_err("[%s] di is null\n", __func__);
		return;
	}
	gpio_direction_input(di->onewire_gpio);
}

static void fm1210_dev_on(struct fm1210_dev *di)
{
	if (!di) {
		hwlog_err("[%s] di is null\n", __func__);
		return;
	}
	set_sched_affinity_to_current();
	local_irq_save(di->irq_flags);
	fm1210_power_on(di);
	fm1210_ow_dev_reset(di);
}

static void fm1210_dev_off(struct fm1210_dev *di)
{
	if (!di) {
		hwlog_err("[%s] di is null\n", __func__);
		return;
	}
	fm1210_power_off(di);
	local_irq_restore(di->irq_flags);
	set_sched_affinity_to_all();
}

static bool is_all_zero(char *buf, int len)
{
	int i;

	if (!buf) {
		hwlog_err("[%s] buf is null\n", __func__);
		return false;
	}
	for (i = 0; i < len; i++) {
		if (buf[i] != 0)
			return false;
	}

	return true;
}

static unsigned short crc16_ccitt_false(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = FM1210_CRC_IN_VAL;
	unsigned short wCPoly = FM1210_CRC_POLY_VAL;
	unsigned char wChar;
	int i;

	if (!puchMsg) {
		hwlog_err("[%s] puchMsg is null\n", __func__);
		return -EINVAL;
	}

	while (usDataLen--) {
		wChar = *(puchMsg++);
		wCRCin ^= (wChar << FM1210_CRC_CHAR_OFFSET);

		for (i = 0; i < FM1210_CRC_CHAR_OFFSET; i++) {
			if (wCRCin & FM1210_CRC_EDGE_VAL)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return wCRCin;
}

static int get_cmd_delay(u8 comm_cmd, u16 *delay_ms)
{
	if (!delay_ms) {
		hwlog_err("[%s]  delay_ms is null\n", __func__);
		return -EINVAL;
	}

	for (uint8_t i = 0; i < ARRAY_SIZE(g_cmd_comm); i++) {
		if (comm_cmd == g_cmd_comm[i].comm_cmd) {
			*delay_ms = g_cmd_comm[i].delay_ms;
			return 0;
		}
	}
	hwlog_err("[%s] fail\n", __func__);
	return -EINVAL;
}

char* fm1210_print_data(u8* buf, size_t buf_length)
{
	uint8_t m;
	size_t total_len = buf_length * FM1210_PRINT_DATA_LEN + FM1210_PRINT_DATA_LEN;
	size_t remaining = total_len;

	if (buf_length == 0)
		return NULL;

	char* result = (char*)kmalloc(total_len, GFP_KERNEL);
	if (!result)
		return NULL;

	char* p = result;
	for (m = 0; m < buf_length; m++) {
		int len = sprintf_s(p, remaining, "%02x ", buf[m]);
		if (len < 0) {
			hwlog_err("[%s] sprintf_s error \n", __func__);
			kfree(result);
			return NULL;
		}

		if (len > remaining) {
			hwlog_err("[%s] buffer overflow detected\n", __func__);
			kfree(result);
			return NULL;
		}
		p += len;
		remaining -= len;
	}
	return result;
}

static void fm1210_print_write_data(u8* buf, size_t buf_length)
{
	char* result = fm1210_print_data(buf, buf_length);
	if (result) {
		hwlog_info("[%s] f1210_write_data: %s\n", __func__, result);
		kfree(result);
	} else {
		hwlog_info("[%s] f1210 print write data fail: %s\n", __func__);
	}
}

static void fm1210_print_read_data(u8* buf, size_t buf_length)
{
	char* result = fm1210_print_data(buf, buf_length);
	if (result) {
		hwlog_info("[%s] f1210_read_data: %s\n", __func__, result);
		kfree(result);
	} else {
		hwlog_info("[%s] fm1210 print read data fail: %s\n", __func__);
	}
}

static u8 owse_transceive(struct fm1210_dev *di, u8 *write_buf, u16 write_len,
	u8 *read_buf, u16 *read_len)
{
	u8 buf[FM1210_TRANSCEIVE_LEN];
	u16 i, buf_len = 0, delay_ms = 0;
	u16 crc, crc_m, crc_s;

	if (!di || !write_buf || !read_buf || !read_len) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	if (get_cmd_delay(write_buf[0], &delay_ms))
		return -EINVAL;

	buf[buf_len++] = write_len;
	if ((memcpy_s(buf + buf_len, write_len, write_buf, write_len)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}

	buf_len += write_len;
	crc = crc16_ccitt_false(write_buf, write_len);
	buf[buf_len++] = crc >> FM1210_CRC_CHAR_OFFSET;
	buf[buf_len++] = crc & FM1210_CRC_GETLOW_VAL;

	for (i = 0; i < buf_len; i++)
		fm1210_write_byte_8m(di, buf[i]);

	fm1210_print_write_data(buf, buf_len);

	if (delay_ms > 0)
		mdelay(delay_ms);

	buf[0] = fm1210_read_byte_8m(di);
	*read_len = buf[0];
	if (*read_len < 1)
		return FM1210_COMM_ERR;
	if (*read_len > FM1210_READ_LEN_EDGE)
		return FM1210_READ_LEN_ERR;
	for (i = 0; i < *read_len + FM1210_CRC_LEN; i++) {
		buf[i] = fm1210_read_byte_8m(di);
	}

	fm1210_print_read_data(buf, *read_len + FM1210_CRC_LEN);

	crc_s = (buf[*read_len] << 8) | buf[*read_len + 1];
	crc_m = crc16_ccitt_false(buf, *read_len);
	if (crc_m != crc_s)
		return FM1210_CRC_ERR;
	mdelay(FM1210_CMD_GAP_DELAY);

	if (buf[0])
		return buf[0];
	*read_len -= FM1210_RES_STATE_LEN;
	if (*read_len) {
		if ((memcpy_s(read_buf, *read_len, buf + FM1210_RES_DATA_OFS, *read_len)) != EOK) {
			hwlog_err("[%s] memcpy_s failed\n", __func__);
			return -EINVAL;
		}
	}
	return FM1210_RESULT_SUCCESS;
}

static u8 ow_apdu_xfer_once(struct fm1210_dev *di, u8 *write_buf, u16 write_len,
	u8 *read_buf, u16 *read_len)
{
	unsigned char ret = 0;

	if (!di || !write_buf || !read_buf || !read_len) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	fm1210_dev_on(di);
	ret = owse_transceive(di, write_buf, write_len, read_buf, read_len);
	fm1210_dev_off(di);
	if (ret)
		return ret;
	return FM1210_RESULT_SUCCESS;
}

/* apdu process */
static u8 ow_apdu_xfer(struct fm1210_dev *di, u8 *write_buf, u16 write_len, u8 *read_buf, u16 *read_len)
{
	unsigned char ret = 0;
	unsigned char loop = 0;

	if (!di || !write_buf || !read_buf || !read_len) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	for (loop = 0; loop < FM1210_MAX_RETRY_COUNT; loop++) {
		ret = ow_apdu_xfer_once(di, write_buf, write_len, read_buf, read_len);
		if (ret)
			hwlog_err("[%s] owse_transceive fail ret : %u\n", __func__, ret);
		else
			return FM1210_RESULT_SUCCESS;
		msleep(FM1210_CMD_RETRY_DELAY);
	}
	hwlog_err("[%s] apdu xfer loop fail: %d\n", __func__, ret);
	return ret;
}

static int fm1210_get_dev_cert(struct fm1210_dev *di, char *buf)
{
	u8 cmdbuf[FM1210_CMD_CERT_LEN] = { FM1210_GET_CERT };
	u8 rbuf[FM1210_CERT_RECV_LEN];
	u16 cmdlen = FM1210_CMD_CERT_LEN;
	u16 rlen;

	if (!di || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	char *retbuf = buf;

	if (ow_apdu_xfer(di, cmdbuf, cmdlen, rbuf, &rlen)) {
		hwlog_err("[%s] read certificate err\n", __func__);
		return -EINVAL;
	}
	fm1210_hex_to_str(retbuf, rbuf, FM1210_DEV_CERT_SIZE);
	retbuf += FM1210_DEV_CERT_OFS;
	if ((snprintf_s(retbuf, FM1210_DEV_INFO_LEN - FM1210_DEV_CERT_OFS,
		(FM1210_DEV_INFO_LEN - FM1210_DEV_CERT_OFS - 1), "\n")) < 0) {
		hwlog_err("[%s] snprintf_s is failed\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static void fm1210_get_sig_res(char *buf, int len)
{
	if (!buf) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return;
	}
	fm1210_hex_to_str(buf, g_ecc_sig_res, FM1210_ECC_SIG_SIZE);
	if ((snprintf_s(buf + FM1210_ECC_SIG_OFS, len - FM1210_ECC_SIG_OFS,
		(len - FM1210_ECC_SIG_OFS - 1), "\n")) < 0)
		hwlog_err("[%s] snprintf_s is failed\n", __func__);
	memset_s(g_ecc_sig_res, sizeof(g_ecc_sig_res), 0, sizeof(g_ecc_sig_res));
}

static int gen_rand(struct fm1210_dev *di, unsigned char *buf, u16 rand_len)
{
	u8 cmdbuf[FM1210_CMD_RAND_LEN] = { FM1210_GET_RAND };
	u8 rbuf[FM1210_TRND_LEN];
	u16 cmdlen = FM1210_CMD_RAND_LEN;
	u16 rlen;

	if (!di || !buf) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	if (ow_apdu_xfer(di, cmdbuf, cmdlen, rbuf, &rlen)) {
		hwlog_err("[%s] err\n", __func__);
		return -EINVAL;
	}
	if ((memcpy_s(buf, rand_len, rbuf, rand_len)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	};
	return 0;
}

void fm1210_hex_to_str(char *pdest, const char *psrc, int ilen)
{
	char ddl, ddh;
	int i;

	if (!pdest || !psrc) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return;
	}

	for (i = 0; i < ilen; i++) {
		ddh = FM1210_ASCII_NUMBER_START + (psrc[i] / FM1210_ADDR_OFFSET_BIT4);
		ddl = FM1210_ASCII_NUMBER_START + (psrc[i] % FM1210_ADDR_OFFSET_BIT4);
		if (ddh > FM1210_ASCII_NUMBER_END)
			ddh = ddh + FM1210_OFFSET_LETTER;
		if (ddl > FM1210_ASCII_NUMBER_END)
			ddl = ddl + FM1210_OFFSET_LETTER;
		pdest[i * 2] = ddh;
		pdest[i * 2 + 1] = ddl;
	}
	pdest[ilen * 2] = '\0';
}

char fm1210_to_upper(char c)
{
	if ((c >= FM1210_ASCII_LOWERCASE_START) && (c <= FM1210_ASCII_LOWERCASE_END))
		return (c - FM1210_ASCII_TO_UPPERCASE);
	return c;
}

void fm1210_str_to_hex(char *pdest, const char *psrc, int ilen)
{
	char h1, h2;
	char s1, s2;
	int i;

	if (!pdest || !psrc) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return;
	}

	for (i = 0; i < ilen; i++) {
		h1 = psrc[2 * i];
		h2 = psrc[2 * i + 1];
		s1 = (fm1210_to_upper(h1) - FM1210_STR_TO_HEX_OFFSET);
		if (s1 > FM1210_HEX_NUMBERS_NUM)
			s1 -= FM1210_HEX_LETTERS_NUM;
		s2 = (fm1210_to_upper(h2) - FM1210_STR_TO_HEX_OFFSET);
		if (s2 > FM1210_HEX_NUMBERS_NUM)
			s2 -= FM1210_HEX_LETTERS_NUM;
		pdest[i] = (s1 * FM1210_DEC_TO_HEX_TIMES) + s2;
	}
}

static int get_dev_info(struct fm1210_dev *di, struct power_genl_attr *key_res, uint8_t *dev_info)
{
	int ret;
	char buffer[FM1210_DEV_INFO_LEN];
	char cert_hash[HASH_LENGTH];
	uint8_t comb_table_qbuf[FM1210_COMB_TABLE_QBUF_LEN];

	if (!di || !key_res || !dev_info) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}
	/* init fmecsda */
	fm_ecdsa_precombpoint(comb_table_qbuf, FM1210_COMB_TABLE_QBUF_LEN, key_res->data, key_res->len);
	uint8_t m;
	uint8_t len =  key_res->len;
	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	ret = fm1210_read_romid(di, buffer, FM1210_MAX_RETRY_COUNT, FM1210_CMD_RETRY_DELAY);
	if (ret) {
		hwlog_info("[%s] UID read failed\n", __func__);
		return -EINVAL;
	}
	fm1210_str_to_hex(dev_info, buffer, FM1210_READ_UID_SIZE);

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	ret = fm1210_get_dev_cert(di, buffer);
	if (ret)
		return -EINVAL;

	fm1210_str_to_hex(dev_info + FM1210_READ_UID_SIZE, buffer, FM1210_DEV_CERT_SIZE);
	hwlog_debug("[%s] dev_info:\n", __func__);
	for (m = 0; m < 152; m++) {
		hwlog_debug("%02X \n", dev_info[m]);
	}
	fm_sha256(dev_info + FM1210_READ_UID_SIZE, FM1210_CERT_LEN, cert_hash);
	ret = fm_ecdsa_verify(comb_table_qbuf, FM1210_COMB_TABLE_QBUF_LEN, key_res->data, key_res->len,
		cert_hash, HASH_LENGTH, dev_info + FM1210_CERT_LEN + FM1210_READ_UID_SIZE, SIG_LENGTH);
	hwlog_info("[%s] AP verify cert result: %d\n", __func__, ret); /* 0: success */

	return ret;
}

static int fm1210_ecc_signature(struct fm1210_dev *di, uint8_t *msg_hash,
	uint8_t *sig_res, enum ecc_sign_mode mode)
{
	char buffer[FM1210_DEV_INFO_LEN]; /* msg_hash */
	u8 cmdbuf[FM1210_CMD_ECC_LEN] = { FM1210_GET_ECC, FM1210_GET_ECC_TYPE };
	u8 rbuf[FM1210_ECC_RECV_LEN];
	u8 cmdlen = FM1210_CMD_ECC_LEN;
	u16 rlen;
	u8 msg[FM1210_ECC_MSG_LEN] = { 0 };

	if (!di || !msg_hash || !sig_res) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	fm1210_hex_to_str(buffer, msg_hash, HASH_LENGTH);

	fm1210_str_to_hex(msg, buffer, FM1210_ECC_MSG_LEN);

	if ((memcpy_s(cmdbuf + FM1210_ECC_MSG_OFFSET, FM1210_ECC_MSG_LEN, msg, FM1210_ECC_MSG_LEN)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	if (ow_apdu_xfer(di, cmdbuf, cmdlen, rbuf, &rlen))
		return -EINVAL;
	if ((memcpy_s(g_ecc_sig_res, SIG_LENGTH, rbuf, rlen)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	fm1210_get_sig_res(buffer, FM1210_DEV_INFO_LEN);
	fm1210_str_to_hex(sig_res, buffer, SIG_LENGTH);
	return 0;
}

static int verify_device(struct fm1210_dev *di, uint8_t *dev_cert, enum ecc_sign_mode mode)
{
	int ret;
	unsigned char trnd[FM1210_TRND_LEN];
	unsigned char trnd_hash[HASH_LENGTH];
	unsigned char sig[SIG_LENGTH];
	uint8_t comb_table_qbuf[FM1210_COMB_TABLE_QBUF_LEN];

	if (!di || !dev_cert) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	/* init fmecsda */
	fm_ecdsa_precombpoint(comb_table_qbuf, FM1210_COMB_TABLE_QBUF_LEN, dev_cert + FM1210_CERT_HEAD, SIG_LENGTH);

	memset_s(trnd_hash, sizeof(trnd_hash), 0, sizeof(trnd_hash));
	memset_s(sig, sizeof(sig), 0, sizeof(sig));

	/* AP gen rand */
	gen_rand(di, trnd, FM1210_TRND_LEN);

	/* AP calculate SHA256 */
	fm_sha256(trnd, FM1210_TRND_LEN, trnd_hash);

	/* SE calculate ECC signature */
	ret = fm1210_ecc_signature(di, trnd_hash, sig, mode);
	if (ret) {
		hwlog_err("[%s] ecc_signature err : %d\n", __func__, ret);
		return ret;
	}

	ret = fm_ecdsa_verify(comb_table_qbuf, FM1210_COMB_TABLE_QBUF_LEN, dev_cert + FM1210_CERT_HEAD, SIG_LENGTH,
		trnd_hash, HASH_LENGTH, sig, SIG_LENGTH);
	hwlog_info("[%s] AP verify signature result: %d\n", __func__, ret);

	return ret;
}

int fm1210_do_authentication(struct fm1210_dev *di, struct power_genl_attr *key_res,
	enum key_cr *result, enum ecc_sign_mode mode)
{
	int ret;
	unsigned char cert[FM1210_DEV_CERT_SIZE];
	unsigned char romid[FM1210_READ_UID_SIZE];
	uint8_t dev_info[FM1210_READ_UID_SIZE + FM1210_DEV_CERT_SIZE];

	if (!di || !key_res || !result) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	memset_s(cert, sizeof(cert), 0, sizeof(cert));
	memset_s(romid, sizeof(romid), 0, sizeof(romid));
	memset_s(dev_info, sizeof(dev_info), 0, sizeof(dev_info));

	ret = get_dev_info(di, key_res, dev_info);
	if (ret) {
		hwlog_err("[%s] get_dev_info err: %d\n", __func__, ret);
		*result = KEY_FAIL_UNMATCH;
		return -EAGAIN;
	}
	ret = memcpy_s(romid, FM1210_READ_UID_SIZE, dev_info, FM1210_READ_UID_SIZE);
	if (ret != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	ret = memcpy_s(cert, FM1210_DEV_CERT_SIZE, dev_info + FM1210_READ_UID_SIZE, FM1210_DEV_CERT_SIZE);
	if (ret != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}

	ret = verify_device(di, cert, mode);
	if (ret) {
		hwlog_err("[%s] verify_device err: %d\n", __func__, ret);
		*result = KEY_FAIL_UNMATCH;
		return -EINVAL;
	}

	*result = KEY_PASS;
	return 0;
}

static int fm1210_ee_pump(struct fm1210_dev *di, int flag)
{
	u8 cmdbuf[FM1210_CMD_EE_LEN] = { FM1210_ACTIVATE_EE_CMD, flag };
	u8 rbuf[FM1210_EE_RECV_LEN];
	u8 cmdlen = FM1210_CMD_EE_LEN;
	u16 rlen;
	unsigned char loop = 0;

	if (!di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	for (loop = 0; loop < FM1210_MAX_RETRY_COUNT; loop++) {
		fm1210_dev_on(di);
		if (owse_transceive(di, cmdbuf, cmdlen, rbuf, &rlen)) {
			fm1210_dev_off(di);
			msleep(FM1210_CMD_RETRY_DELAY);
		} else {
			hwlog_info("[%s] fm1210_ee_pump active succ\n", __func__);
			return FM1210_RESULT_SUCCESS;
		}
	}
	return -EINVAL;
}

static u8 fm1210_write_block_retry(struct fm1210_dev *di, u8 *write_buf, u16 write_len, u8 *read_buf, u16 *read_len)
{
	unsigned char ret = 0;
	unsigned char loop = 0;

	if (!di || !write_buf || !read_buf || !read_len) {
		hwlog_err("[%s] pointer NULL\n", __func__);
		return -EINVAL;
	}

	for (loop = 0; loop < FM1210_MAX_RETRY_COUNT; loop++) {
		ret = owse_transceive(di, write_buf, write_len, read_buf, read_len);
		if (ret) {
			fm1210_dev_off(di);
			msleep(FM1210_CMD_RETRY_DELAY);
			if (fm1210_ee_pump(di, FM1210_EE_PUMP_ACTIVE)) {
				hwlog_err("[%s] fm1210_ee_pump active failed\n", __func__);
				return -EINVAL;
			}
		} else
			return FM1210_RESULT_SUCCESS;
	}
	return ret;
}

static int fm1210_read_block(struct fm1210_dev *di, int blockNum, unsigned char *buf)
{
	u8 cmdbuf[FM1210_CMD_READ_LEN] = { FM1210_READ_CMD, blockNum }; /* read cmd */
	u8 rbuf[FM1210_READ_RECV_LEN]; /* data + sw */
	u16 cmdlen = FM1210_CMD_READ_LEN;
	u16 rlen;
	int ret;

	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	if (ow_apdu_xfer(di, cmdbuf, cmdlen, rbuf, &rlen))
		return -EINVAL;
	hwlog_info("[%s] memcpy_s rlen: %d\n", __func__, rlen);
	ret = memcpy_s(buf, FM1210_READ_RECV_LEN, rbuf, rlen);
	if (ret != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static int fm1210_write_block(struct fm1210_dev *di, int blockNum, unsigned char *buf, int size)
{
	u8 cmdbuf[FM1210_CMD_WRITE_LEN] = { FM1210_WRITE_CMD, blockNum }; /* write cmd */
	u8 rbuf[FM1210_WRITE_RECV_LEN];
	u16 cmdlen = FM1210_CMD_WRITE_CODE_LEN;
	u16 rlen;

	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	if ((memcpy_s(cmdbuf + cmdlen, FM1210_PAGE_BLOCK_SIZE, buf, FM1210_PAGE_BLOCK_SIZE)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	cmdlen += FM1210_PAGE_BLOCK_SIZE;
	if (fm1210_write_block_retry(di, cmdbuf, cmdlen, rbuf, &rlen))
		return -EAGAIN;
	return 0;
}

static int get_block_len(int block, uint8_t *block_len)
{
	if (!block_len) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}
	for (uint8_t i = 0; i < ARRAY_SIZE(g_file_block); i++) {
		if (block == g_file_block[i].file_block) {
			*block_len = g_file_block[i].block_num;
			return 0;
		}
	}
	hwlog_err("[%s] fail\n", __func__);
	return -EINVAL;
}

static int fm1210_read_file(struct fm1210_dev *di, int blockNum, unsigned char *buf, int size)
{
	int ret;
	int i;
	u8 rbuf[FM1210_READ_BLOCK_RECV_LEN];
	u8 tbuf[FM1210_READ_FILE_RECV_LEN];
	u8 ofs = 0;
	uint8_t block_len = 0;

	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = get_block_len(blockNum, &block_len);
	if (ret)
		return -EINVAL;

	for (i = 0; i < block_len; i++) {
		ret = fm1210_read_block(di, blockNum + i, rbuf);
		if (ret) {
			hwlog_err("[%s] fail : %d\n", __func__, ret);
			return ret;
		}
		if ((memcpy_s(tbuf + ofs, FM1210_PAGE_BLOCK_SIZE, rbuf, FM1210_PAGE_BLOCK_SIZE)) != EOK) {
			hwlog_err("[%s] memcpy_s failed\n", __func__);
			return -EINVAL;
		}
		ofs += FM1210_PAGE_BLOCK_SIZE;
	}
	if ((memcpy_s(buf, size, tbuf, size)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static int fm1210_write_file(struct fm1210_dev *di, int blockNum, unsigned char *buf, int size)
{
	int ret;
	int i;
	u8 rbuf[FM1210_PAGE_BLOCK_SIZE];
	u8 ofs = 0;
	uint8_t block_len = 0;

	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = get_block_len(blockNum, &block_len);
	if (ret)
		return -EINVAL;

	if (fm1210_ee_pump(di, FM1210_EE_PUMP_ACTIVE)) {
		hwlog_err("[%s] fm1210_ee_pump active failed\n", __func__);
		return -EINVAL;
	}
	for (i = 0; i < block_len; i++) {
		if ((memcpy_s(rbuf, FM1210_PAGE_BLOCK_SIZE, buf + ofs, FM1210_PAGE_BLOCK_SIZE)) != EOK) {
			hwlog_err("[%s] memcpy_s failed\n", __func__);
			fm1210_dev_off(di);
			return -EINVAL;
		}
		ret = fm1210_write_block(di, blockNum + i, rbuf, FM1210_PAGE_BLOCK_SIZE);
		if (ret) {
			hwlog_err("[%s] fail : %d\n", __func__, ret);
			return ret;
		}
		ofs += FM1210_PAGE_BLOCK_SIZE;
		memset_s(rbuf, sizeof(rbuf), 0, sizeof(rbuf));
	}
	fm1210_dev_off(di);
	return 0;
}

static int fm1210_lock_file(struct fm1210_dev *di, int blockNum)
{
	u8 cmdbuf[FM1210_CMD_LOCK_LEN] = { FM1210_LOCK_CMD, blockNum }; /* lock cmd */
	u8 rbuf[FM1210_LOCK_RECV_LEN];
	u16 cmdlen = FM1210_CMD_LOCK_LEN;
	u16 rlen;
	u8 ret = 0;
	uint8_t block_len = 0;
	int i;

	if (!di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = get_block_len(blockNum, &block_len);
	if (ret)
		return -EINVAL;

	if (fm1210_ee_pump(di, FM1210_EE_PUMP_ACTIVE)) {
		hwlog_err("[%s] fm1210_ee_pump active failed\n", __func__);
		return -EINVAL;
	}
	for (i = 0; i < block_len; i++) {
		cmdbuf[FM1210_CMD_BLOCKNUM] = blockNum + i;
		ret = fm1210_write_block_retry(di, cmdbuf, cmdlen, rbuf, &rlen);
		if (ret) {
			hwlog_err("[%s] fail : %d\n", __func__, ret);
			return ret;
		}
	}
	fm1210_dev_off(di);
	return ret;
}

static int fm1210_file_lock_sta(struct fm1210_dev *di, int blockNum, unsigned char *buf, int buf_len)
{
	u8 cmdbuf[FM1210_CMD_LOCK_STA_LEN] = { FM1210_LOCKSTA_CMD, blockNum };
	u8 rbuf[FM1210_LOCKSTA_RECV_LEN];
	u16 rlen;
	u8 ret;
	uint8_t block_len = 0;
	struct lock_status lock_sta;
	int lock_block = 0;

	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}

	ret = get_block_len(blockNum, &block_len);
	if (ret)
		return -EINVAL;

	for (int i = 0; i < block_len; i++) {
		cmdbuf[FM1210_CMD_BLOCKNUM] = blockNum + i;
		ret = ow_apdu_xfer(di, cmdbuf, FM1210_CMD_LOCK_STA_LEN, rbuf, &rlen);
		if (ret) {
			hwlog_err("[%s] fail : %d\n", __func__, ret);
			return ret;
		}
		if ((memcpy_s((u8 *)&lock_sta, sizeof(lock_sta), rbuf, sizeof(lock_sta))) != EOK) {
			hwlog_err("[%s] memcpy_s failed\n", __func__);
			return -EINVAL;
		}
		if ((lock_sta.write_access != ACCESS_ALLOWED) && (lock_sta.read_access == ACCESS_ALLOWED) &&
		(lock_sta.enc_type == ENC_CLEARTEXT))
			lock_block++;
		memset_s(rbuf, sizeof(rbuf), 0, sizeof(rbuf));
	}

	if (block_len == lock_block) {
		ret = snprintf_s(buf, buf_len, buf_len - 1, "locked\n");
		if (ret < 0)
			hwlog_err("[%s] lock_sta snprintf_s is failed\n", __func__);
		else
			hwlog_info("[%s] lock_sta:locked\n", __func__);
	} else {
		ret = snprintf_s(buf, buf_len, buf_len - 1, "unlocked\n");
		if (ret < 0)
			hwlog_err("[%s]lock_sta snprintf_s is failed\n", __func__);
		else
			hwlog_info("[%s] lock_sta:unlocked\n", __func__);
	}
	return 0;
}

static int fm1210_read_raw_romid(struct fm1210_dev *di, char *buf)
{
	u8 cmdbuf[FM1210_CMD_READ_LEN] = { FM1210_READ_CMD, FM1210_UID_BLOCKNUM}; /* read cmd */
	u8 rbuf[FM1210_READ_RECV_LEN]; /* data + sw */
	u16 cmdlen = FM1210_CMD_READ_LEN;
	u16 rlen;
	int ret;

	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return -EINVAL;
	}
	ret = ow_apdu_xfer_once(di, cmdbuf, cmdlen, rbuf, &rlen);
	if (ret) {
		hwlog_err("[%s] owse_transceive fail ret : %u\n", __func__, ret);
		return -EINVAL;
	}
#ifdef ONEWIRE_STABILITY_DEBUG
	for (uint8_t i = 0; i < FM1210_READ_RECV_LEN; i++)
		hwlog_debug("[%s] rbuf[%d] = %x\n", __func__, i, rbuf[i]);
#endif
	if (is_all_zero(rbuf, FM1210_READ_UID_SIZE)) {
		hwlog_err("[%s] uid all zero\n", __func__);
		return -EINVAL;
	}
	if ((memcpy_s(buf, FM1210_READ_UID_SIZE, rbuf, FM1210_READ_UID_SIZE)) != EOK) {
		hwlog_err("[%s] memcpy_s failed\n", __func__);
		return -EINVAL;
	}
	return 0;
}

int fm1210_ic_ck(struct fm1210_dev *di)
{
	if (!di)
		return -ENODEV;

	return fm1210_read_romid(di, di->mem.uid, 1, 0);
}

static bool get_uid_cache(struct fm1210_dev *di, char *buf)
{
	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return false;
	}
	if (di->mem.uid_ready)
		if ((memmove_s(buf, FM1210_READ_UID_SIZE, di->mem.uid, FM1210_READ_UID_SIZE)) != EOK) {
			hwlog_err("[%s] memmove_s failed\n", __func__);
			return false;
		}
	return di->mem.uid_ready;
}

static void set_uid_cache(struct fm1210_dev *di, char *buf)
{
	if (!buf || !di) {
		hwlog_err("[%s] : pointer NULL\n", __func__);
		return;
	}
	if (di->mem.uid_ready)
		return;
	if ((memmove_s(di->mem.uid, FM1210_READ_UID_SIZE, buf, FM1210_READ_UID_SIZE)) != EOK) {
		hwlog_err("[%s] memmove_s failed\n", __func__);
		return;
	}
	di->mem.uid_ready = true;
}

int fm1210_read_romid(struct fm1210_dev *di, char *buf, int times, int delay_ms)
{
	int loop;
	int ret;
	char uid_buffer[FM1210_READ_UID_SIZE];

	if (!di || !buf || (times <= 0) || (delay_ms < 0))
		return -ENODEV;

	if (get_uid_cache(di, buf)) {
		hwlog_info("[%s] read uid_cache suc\n", __func__);
		return 0;
	}

	for (loop = 0; loop < times; loop++) {
		ret = fm1210_read_raw_romid(di, uid_buffer);
		if (!ret)
			break;
		if (delay_ms)
			msleep(delay_ms);
	}
	if (!ret) {
		if ((memcpy_s(buf, FM1210_READ_UID_SIZE, uid_buffer, FM1210_READ_UID_SIZE)) != EOK) {
			hwlog_err("[%s] memcpy_s failed\n", __func__);
			return -EINVAL;
		}
		set_uid_cache(di, uid_buffer);
	}
	hwlog_info("[%s] read uid suc\n", __func__);
	return ret;
}

int fm1210_operate_file(struct fm1210_dev *di, int blockNum, int operation, unsigned char *buf, int buf_len)
{
	int ret;

	switch (operation) {
	case FM1210_READ:
		ret = fm1210_read_file(di, blockNum, buf, buf_len);
		break;
	case FM1210_WRITE:
		ret = fm1210_write_file(di, blockNum, buf, buf_len);
		break;
	case FM1210_LOCK:
		ret = fm1210_lock_file(di, blockNum);
		break;
	case FM1210_LOCK_STATUS:
		ret = fm1210_file_lock_sta(di, blockNum, buf, buf_len);
		break;
	default:
		hwlog_err("[%s] no operetion\n", __func__);
		return -EINVAL;
	}

	return ret;
}
