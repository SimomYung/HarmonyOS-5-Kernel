/*
 * Huawei hi1162 update code
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
#include <securec.h>
#include <crypto/hash.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/completion.h>
#include <linux/syscalls.h>
#include "hi116x_fwupdate.h"

#define HI1162_UPDATE_RETRY 5
#define HI1162_REPAIR_FF_RETRY 2
#define arry_size(a) (sizeof(a) / sizeof((a)[0]))

#define HI1162_UART_PORT "ttyHW5"
#define HI1162_UART_BAUD 115200
#define HI1162_UART_BUF_LEN 8192
#define HI1162_UART_BAUD_MAX 500000

#define HI1162_SSB_PATH "sparklink/hi1162SSB.bin"
#define HI1162_NV_PATH "sparklink/hi1162NV.bin"
#define HI1162_BT_PATH "sparklink/hi1162BT.bin"
#define HI1162_APP_PATH "sparklink/hi1162APP.bin"
#define HI1162_SSBEFC_PATH "sparklink/hi1162SSBEFC.bin"
#define HI1162_CKFF_PATH "sparklink/hi1162CKFF.bin"
#define HI1162_OTA_PATH "sparklink/hi1162OTA.bin"
#define HI1162_FW_MAX 7

#define HI1162_BT_CORE_TYPE 0
#define HI1162_DSP_CORE_TYPE 5
#define HI1162_APP_CORE_TYPE 2
#define HI1162_NV_CORE_TYPE 4
#define HI1162_SSB_CORE_TYPE 0x10 // virtual id, not use in cmd
#define HI1162_SSBEFC_CORE_TYPE 0x11
#define HI1162_CKFF_CORE_TYPE 0x12
#define HI1162_OTA_CORE_TYPE 0x13

#define HI1162_BT_RAM_SIZE (320 * 1024)
#define HI1162_DSP_RAM_SIZE (20 * 1024)
#define HI1162_APP_RAM_SIZE (528 * 1024)
#define HI1162_NV_RAM_SIZE (20 * 1024)
#define HI1162_CKFF_RAM_SIZE (0x8c400000)
#define HI1162_OTA_RAM_SIZE (0x8c501000)

#define HI1162_BT_FLASH_SIZE (320 * 1024)
#define HI1162_DSP_FLASH_SIZE (20 * 1024)
#define HI1162_APP_FLASH_SIZE (528 * 1024)
#define HI1162_NV_FLASH_SIZE (20 * 1024)
#define HI1162_CKFF_FLASH_SIZE (96 * 1024)
#define HI1162_OTA_FLASH_SIZE (702 * 1024)
#define HI1162_DLOAD_PACKET_SIZE 4096

#define HI1162_ROMLOADER_TYPE 0
#define HI1162_CODELOADER_TYPE 1
#define HI1162_TYPE_ERROR 2
#define HI1162_HASH_LEN_NEED 32

#define GET_ENTRY_FLAG_MAX_TIME 1000

#define HI1162_CMD_DELAY 5000
#define HI1162_CMD_FFCK_DELAY 50000
#define HI1162_HASH_CMD_DELAY 5050
#define HI1162_CMD_RESET_DELAY 2500

#define HI1162_NORMAL_CMD 0
#define HI1162_HASH_CMD 1
#define HI1162_NV_OTA_CMD 2
#define HI1162_NO_NEED_CHECK 3

#define HI1162_CMD_MAX_LEN 40

#define HI1162_CMD 0
#define HI1162_DATA 1

#define RESPONSE_LEN 9
#define RESPONSE_CODETYPE_LEN 37 /* less than code buffer size 42 */
#define RESPONSE_ROMTYPE_LEN 34 /* less than rom buffer size 39 */
#define INIT_CMD_LEN 20 /* demo 20 bytes:5C 73 01 02 00 01 01 01 01 00 00 00 00 63 6F 64 65 0E 5C 65 */
#define SSB_VER_LEN 80 /* 80: demo len 83 , >= 80 will ok */

#define TYPE_START 4 /* 4: begin of type str */
#define MIN_RECV_MSG_LEN 6 /* 5: at least 5 bytes required to check recv val */

#define HI1162_MSG_HEADER_1 0x5C
#define HI1162_MSG_HEADER_2 0x73
#define HI1162_MSG_TAIL_1 0x5C
#define HI1162_MSG_TAIL_2 0x65

#define HEAD_INVALID (-1)

#define HI1162_DUPLICATE_PADDING 0x00

#define WAIT_CHIP_PROD_MS 50000

typedef struct _hi1162_cmd {
	const u8 id;
	const u8 cmd_type;
	const int len;
	u8 cmdbuf[HI1162_CMD_MAX_LEN];
	const struct firmware *firmware;
	const int time_out;
	const int check_type;
	const int response_len;
} hi1162_cmd;

typedef struct _mode_buf {
	u8 buffer[SPARKLINK_UART_BUFF_LEN];
	size_t size;
} mode_buf;

static const char *g_hi1162_romloader = "romloader";
static const char *g_hi1162_codeloader = "codeloader";
static u8 g_hi1162_img_hash_out[HI1162_HASH_LEN_NEED] = {0};
static u8 g_hi1162_trans_buffer[HI1162_UART_BUF_LEN] = {0};
static u8 g_hi1162_data_buf[5 + HI1162_DLOAD_PACKET_SIZE] = {0};
static u8 g_serial_num = 0;
static const char *g_hi1162_uart_port = HI1162_UART_PORT;
static long g_hi1162_uart_default_baud = HI1162_UART_BAUD;
static long g_hi1162_uart_max_baud = HI1162_UART_BAUD_MAX;
static mode_buf g_uart_buf = {
	.buffer = {0},
	.size = 0,
};
static u8 g_tmp_mode = HI1162_TYPE_ERROR;
static u8 g_loadflg = 0;
static struct completion g_recv_complete;
static spinlock_t g_uart_lock;

static hi1162_cmd g_hi1162_init_command = {0x01, HI1162_CMD, 0,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, INIT_CMD_LEN};

static hi1162_cmd g_hi1162_check_sum_command = {0x02, HI1162_CMD, 33,
	{0}, NULL, HI1162_HASH_CMD_DELAY, HI1162_HASH_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_start_page_command = {0x03, HI1162_CMD, 3,
	{0x00, 0x00, 0x00}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_firmware_command = {0x04, HI1162_DATA, 0,
	{0x00}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_end_flash_command = {0x05, HI1162_CMD, 0,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_mem_config_command = {0x06, HI1162_CMD, 9,
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_flash_erase_command = {0x07, HI1162_CMD, 1,
	{0x00}, NULL, 50000, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_exit_ssb_command = {0x0A, HI1162_CMD, 0,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_exit_ssb_no_check_command = {0x0A, HI1162_CMD, 0,
	{0}, NULL, 3000, HI1162_NO_NEED_CHECK, RESPONSE_LEN};

static hi1162_cmd g_hi1162_reset_command = {0x0C, HI1162_CMD, 4,
	{0x00, 0x00, 0x00, 0x00}, NULL, HI1162_CMD_RESET_DELAY, HI1162_NO_NEED_CHECK,
	RESPONSE_LEN};

static hi1162_cmd g_hi1162_speed_command = {0x0D, HI1162_CMD, 8,
	{0x00, 0x07, 0xA1, 0x20, 0x00, 0x00, 0x70, 0x00},
	NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_erase_ssb_command = {0x0F, HI1162_CMD, 0,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_flash_limit_command = {0x11, HI1162_CMD, 8,
	{0x00, 0x07, 0xA1, 0x20, 0x00, 0x00, 0x70, 0x00},
	NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_signature_command = {0x15, HI1162_CMD, 36,
	{0x02, 0x00, 0x00, 0x20}, NULL, HI1162_HASH_CMD_DELAY, HI1162_HASH_CMD,
	RESPONSE_LEN};

static hi1162_cmd g_hi1162_read_ssbversion_command = {0x25, HI1162_CMD, 0,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, SSB_VER_LEN};

static hi1162_cmd g_hi1162_write_bin_command = {0x36, HI1162_CMD, 9,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_sha_bin_command = {0x37, HI1162_CMD, 40,
	{0}, NULL, HI1162_CMD_FFCK_DELAY, HI1162_NV_OTA_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_check_ssb_command = {0xFE, HI1162_CMD, 0,
	{0}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd g_hi1162_ssb_firmware_command = {0xFC, HI1162_DATA, 4,
	{0x00, 0x00, 0x00, 0x00}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD,
	RESPONSE_LEN};

static hi1162_cmd g_hi1162_ssb_sha_command = {0xFD, HI1162_CMD, 36,
	{0x00}, NULL, HI1162_CMD_DELAY, HI1162_NORMAL_CMD, RESPONSE_LEN};

static hi1162_cmd *g_hi1162_app_update_cmd_list[] = {
	&g_hi1162_init_command,
	&g_hi1162_speed_command,
	&g_hi1162_flash_limit_command,
	&g_hi1162_flash_erase_command,
	&g_hi1162_start_page_command,
	&g_hi1162_firmware_command,
	&g_hi1162_end_flash_command,
	&g_hi1162_mem_config_command,
	&g_hi1162_check_sum_command,
	&g_hi1162_signature_command,
	&g_hi1162_reset_command,
};

static hi1162_cmd *g_hi1162_ssb_erase_cmd_list[] = {
	&g_hi1162_init_command,
	&g_hi1162_erase_ssb_command,
	&g_hi1162_exit_ssb_command,
};

static hi1162_cmd *g_hi1162_ssb_update_cmd_list[] = {
	&g_hi1162_init_command,
	&g_hi1162_read_ssbversion_command,
	&g_hi1162_ssb_firmware_command,
	&g_hi1162_ssb_sha_command,
	&g_hi1162_check_ssb_command,
	&g_hi1162_exit_ssb_command,
	&g_hi1162_exit_ssb_no_check_command,
};

static hi1162_cmd *g_hi1162_bin_update_cmd_list[] = {
	&g_hi1162_init_command,
	&g_hi1162_speed_command,
	&g_hi1162_write_bin_command,
	&g_hi1162_firmware_command,
	&g_hi1162_end_flash_command,
	&g_hi1162_sha_bin_command,
	&g_hi1162_reset_command,
};

typedef struct _hi1162_update_info {
	int index;
	const char *fw_path;
	u32 ram_size;
	int flash_size;
	bool need_erase_ssb;
	int entry_check;
	hi1162_cmd **list;
	int list_len;
	bool need_check_id;
	bool need_delay;
} hi1162_update_info;

static hi1162_update_info g_hi1162_update_info_data[] = {
	{HI1162_SSBEFC_CORE_TYPE, HI1162_SSBEFC_PATH, 0,
	0, true, HI1162_ROMLOADER_TYPE,
	g_hi1162_ssb_update_cmd_list, arry_size(g_hi1162_ssb_update_cmd_list), false, true},

	{HI1162_CKFF_CORE_TYPE, HI1162_CKFF_PATH, HI1162_CKFF_RAM_SIZE,
	HI1162_CKFF_FLASH_SIZE, false, HI1162_CODELOADER_TYPE,
	g_hi1162_bin_update_cmd_list, arry_size(g_hi1162_bin_update_cmd_list), false, false},

	{HI1162_SSB_CORE_TYPE, HI1162_SSB_PATH,	0,
	0, true, HI1162_ROMLOADER_TYPE,
	g_hi1162_ssb_update_cmd_list, arry_size(g_hi1162_ssb_update_cmd_list), false, true},

	{HI1162_NV_CORE_TYPE, HI1162_NV_PATH, HI1162_NV_RAM_SIZE,
	HI1162_NV_FLASH_SIZE, false, HI1162_CODELOADER_TYPE,
	g_hi1162_bin_update_cmd_list, arry_size(g_hi1162_bin_update_cmd_list), false, false},

	{HI1162_BT_CORE_TYPE, HI1162_BT_PATH, HI1162_BT_RAM_SIZE,
	HI1162_BT_FLASH_SIZE, false, HI1162_CODELOADER_TYPE,
	g_hi1162_app_update_cmd_list, arry_size(g_hi1162_app_update_cmd_list), false, false},

	{HI1162_APP_CORE_TYPE, HI1162_APP_PATH, HI1162_DSP_RAM_SIZE,
	HI1162_APP_FLASH_SIZE, false, HI1162_CODELOADER_TYPE,
	g_hi1162_app_update_cmd_list, arry_size(g_hi1162_app_update_cmd_list), false, false},

	{HI1162_OTA_CORE_TYPE, HI1162_OTA_PATH, HI1162_OTA_RAM_SIZE,
	HI1162_OTA_FLASH_SIZE, false, HI1162_CODELOADER_TYPE,
	g_hi1162_bin_update_cmd_list, arry_size(g_hi1162_bin_update_cmd_list), false, false},
};


// sparklink uart control
static int hi1162_uart_init(struct sparklink_uart_ops *uart_ops)
{
	int ret;

	if (!uart_ops)
		return -EINVAL;

	init_completion(&g_recv_complete);
	ret = uart_ops->uart_init(g_hi1162_uart_port, g_hi1162_uart_default_baud);
	if (ret < 0) {
		sparklink_errmsg("uart init error %d\n", ret);
		return ret;
	}
	return 0;
}

static int hi1162_uart_deinit(struct sparklink_uart_ops *uart_ops)
{
	int ret;

	if (!uart_ops)
		return -EINVAL;

	ret = uart_ops->uart_deinit();
	if (ret < 0) {
		sparklink_errmsg("uart uart_deinit error %d\n", ret);
		return ret;
	}
	return 0;
}

static int search_mode_in_str(mode_buf *uartbuf, int head)
{
	int i;
	if (!uartbuf)
		return HI1162_TYPE_ERROR;

	if (strstr(uartbuf->buffer + head + TYPE_START, g_hi1162_codeloader)) {
		for (i = head; i < uartbuf->size - 1; i++) {
			if ((uartbuf->buffer[i] == HI1162_MSG_TAIL_1 && uartbuf->buffer[i + 1] == HI1162_MSG_TAIL_2)
				|| i - head >= RESPONSE_CODETYPE_LEN) {
					complete(&g_recv_complete);
					return HI1162_CODELOADER_TYPE;
				}
		}
	} else if (strstr(uartbuf->buffer + head + TYPE_START, g_hi1162_romloader)) {
		for (i = head; i < uartbuf->size - 1; i++) {
			if ((uartbuf->buffer[i] == HI1162_MSG_TAIL_1 && uartbuf->buffer[i + 1] == HI1162_MSG_TAIL_2)
				|| i - head >= RESPONSE_ROMTYPE_LEN) {
					complete(&g_recv_complete);
					return HI1162_ROMLOADER_TYPE;
				}
		}
	}

	return HI1162_TYPE_ERROR;
}

static int find_type_by_str(mode_buf *uartbuf)
{
	int i;
	int head = HEAD_INVALID;
	int tmpmode = HI1162_TYPE_ERROR;

	if (!uartbuf)
		return HI1162_TYPE_ERROR;

	for (i = 0; i < uartbuf->size - 1; i++) {
		if (uartbuf->buffer[i] == HI1162_MSG_HEADER_1 && uartbuf->buffer[i + 1] == HI1162_MSG_HEADER_2) {
			head = i;
			break;
		}
	}
	/* uart buffer wait mode buffer end */
	if (head == HEAD_INVALID || (int)uartbuf->size < head + RESPONSE_ROMTYPE_LEN)
		return HI1162_TYPE_ERROR;

	tmpmode = search_mode_in_str(uartbuf, head);

	return tmpmode;
}

static int find_mode_callback(const unsigned char *buf, int len)
{
	int i;
	if (!buf || len < 0)
		return -EINVAL;

	if (g_uart_buf.size >= SPARKLINK_UART_BUFF_LEN) {
		complete(&g_recv_complete);
		return 0;
	}

	spin_lock_bh(&g_uart_lock);

	/* optimize the read buff size */
	if (g_uart_buf.size + len > SPARKLINK_UART_BUFF_LEN)
		len = SPARKLINK_UART_BUFF_LEN - g_uart_buf.size;

	if (g_loadflg == 0) {
		for (i = 0; i < len; i++) {
			if (buf[i] == HI1162_MSG_HEADER_1) {
				g_loadflg = 1;
				g_uart_buf.size = 0;
				break;
			}
		}
	}
	if (g_loadflg == 0) {
		spin_unlock_bh(&g_uart_lock);
		return 0;
	}

	for (i = 0; i < len; i++) {
		g_uart_buf.buffer[g_uart_buf.size] = buf[i];
		g_uart_buf.size++;
	}

	g_tmp_mode = find_type_by_str(&g_uart_buf);
	spin_unlock_bh(&g_uart_lock);
	return 0;
}

static int judge_mode_type(struct sparklink_uart_ops *uart_ops, u8 *entrytype)
{
	int ret;

	if (!uart_ops || !entrytype)
		return -EINVAL;

	g_loadflg = 0;
	g_uart_buf.size = 0;
	g_tmp_mode = HI1162_TYPE_ERROR;

	ret = uart_ops->uart_read_start(find_mode_callback);
	if (ret < 0) {
		sparklink_errmsg("uart read err ret = %d\n", ret);
		return -EIO;
	}

	if (!wait_for_completion_timeout(&g_recv_complete, msecs_to_jiffies(GET_ENTRY_FLAG_MAX_TIME))) {
		uart_ops->uart_read_end();
		sparklink_errmsg("uart read timeout\n");
		return -EIO;
	}

	if (g_tmp_mode == HI1162_CODELOADER_TYPE)
		sparklink_infomsg("size = %d type is HI1162_CODELOADER_TYPE\n", g_uart_buf.size);
	else if (g_tmp_mode == HI1162_ROMLOADER_TYPE)
		sparklink_infomsg("size = %d type is HI1162_ROMLOADER_TYPE\n", g_uart_buf.size);
	*entrytype = g_tmp_mode;

	uart_ops->uart_read_end();
	if (g_tmp_mode != HI1162_CODELOADER_TYPE && g_tmp_mode != HI1162_ROMLOADER_TYPE)
		return -1;

	return 0;
}

static int _hi1162_device_get_mode(struct sparklink_uart_ops *uart_ops, u8 *entrytype)
{
	int ret;

	if (!uart_ops || !entrytype)
		return -EINVAL;

	g_serial_num = 0;
	hi1162_device_set_mode(SPARKLINK_STATE_RESET);
	uart_ops->reconfig_baudrate(g_hi1162_uart_default_baud);
	uart_ops->uart_clear();
	spin_lock_init(&g_uart_lock);

	ret = judge_mode_type(uart_ops, entrytype);
	/* record 24 byte date of uart rcvBuf when fail,
	8: index of sec U64, 16: index of 3rd U64 ... */
	if (ret < 0) {
		sparklink_errmsg("fail rxBuf: size = %d 0x%016llx 0x%016llx 0x%016llx 0x%016llx 0x%016llx 0x%016llx\n",
			g_uart_buf.size, *(u64 *)g_uart_buf.buffer, *(u64 *)(g_uart_buf.buffer + 8),
			*(u64 *)(g_uart_buf.buffer + 16), *(u64 *)(g_uart_buf.buffer + 24),
			*(u64 *)(g_uart_buf.buffer + 32), *(u64 *)(g_uart_buf.buffer + 40));
		return -EINVAL;
	}
	return 0;
}

static int hi1162_device_get_mode(struct sparklink_uart_ops *uart_ops, u8 *entrytype)
{
	int i;
	int ret = 0;

	if (!uart_ops || !entrytype)
		return -EINVAL;

	for (i = 0; i < HI1162_UPDATE_RETRY; i++) {
		ret = _hi1162_device_get_mode(uart_ops, entrytype);
		if (ret == 0) {
			sparklink_infomsg("get mode ok\n");
			return ret;
		} else {
			sparklink_errmsg("get mode error\n");
		}
	}
	return ret;
}

static u8 crc_calc(u8 crcin, const u8 *buffer, size_t len)
{
	u8 crc = crcin;
	size_t i;

	if (!buffer) {
		sparklink_errmsg("buffer is null\n");
		return crc;
	}

	for (i = 0; i < len; i++)
		crc ^= buffer[i];

	return crc;
}


static int hi1162_img_hash(const struct firmware *img)
{
	struct crypto_shash *alg = NULL;
	struct shash_desc *sdesc = NULL;
	char *alg_name = "sha256";
	unsigned int len;
	size_t size;
	int ret;

	if (!img)
		return -EINVAL;

	len = img->size;
	alg = crypto_alloc_shash(alg_name, 0, 0);
	if (IS_ERR(alg)) {
		sparklink_errmsg("can't alloc alg %s\n", alg_name);
		return -ENOMEM;
	}

	size = sizeof(struct shash_desc) + crypto_shash_descsize(alg);
	sdesc = (struct shash_desc *)kmalloc(size, GFP_KERNEL);
	if (!sdesc) {
		sparklink_errmsg("can't alloc sdesc fail\n");
		crypto_free_shash(alg);
		alg = NULL;
		return -ENOMEM;
	}

	sdesc->tfm = alg;
	ret = crypto_shash_digest(sdesc, img->data, len, g_hi1162_img_hash_out);
	kfree(sdesc);
	crypto_free_shash(alg);
	sdesc = NULL;
	alg = NULL;
	return ret;
}

static inline void word_copy(u8 *buf, u32 word)
{
	if (!buf)
		return;

	/* copy data, buf[3]:word bit[0-7], buf[2]:word bit[8-15],
	buf[1]:word bit[16-23], buf[0]:word bit[24-31] */
	buf[3] = (u8)(word >> 0);
	buf[2] = (u8)(word >> 8);
	buf[1] = (u8)(word >> 16);
	buf[0] = (u8)(word >> 24);
}

static void remove_duplicates(u8 *buffer, size_t *len, u8 value)
{
	int i;
	int j = 0, flag = 0, del_num = 0;

	if (!buffer || !len)
		return;

	for (i = 1; i < *len; i++) {
		if ((buffer[j] == buffer[i]) && (buffer[i] == value)) {
			if (flag == 1) {
				buffer[++j] = buffer[i];
				flag = 0;
				continue;
			}
			flag++;
			del_num++;
		}

		if ((buffer[j] == buffer[i]) && (buffer[i] != value)) {
			buffer[++j] = buffer[i];
			flag = 0;
		}

		if (buffer[j] != buffer[i]) {
			buffer[++j] = buffer[i];
			flag = 0;
		}
	}

	for (i = 0; i < del_num; i++)
		buffer[*len - i - 1] = HI1162_DUPLICATE_PADDING;
	*len -= del_num;
}

// check the device return value is ok
static int hi1162_recv_check(u8 *buffer, unsigned int check_flag, size_t *len)
{
	int i;
	if (!buffer || !len)
		return -EINVAL;

	/* 0x5c:remove repeat 0x5c */
	remove_duplicates(buffer, len, 0x5c);

	for (i = 0; i < *len - 1; i++) {
		if (buffer[i] == HI1162_MSG_HEADER_1 && buffer[i + 1] == HI1162_MSG_HEADER_2)
			break;
	}

	if (i > *len - MIN_RECV_MSG_LEN)
		return -EIO;
	buffer += i;
	*len -= i;

	/* we need check: 3rd result byte:0x02
	5th return code byte: 0x01, if serialNum is 5c,
	every byte turn to next */
	if (check_flag == HI1162_NV_OTA_CMD) {
		/* badflash 0x02-0xff or good flash 0x02-0x01/0x02-0x80 */
		/* --------------  REG DEFINE  --------------------- */
		/* index  0          1         2        */
		/* byte   header     header    MSG_NUM  */
		if ((buffer[3] == 0x02) && (buffer[5] == 0x01)) {
			sparklink_errmsg(" good flash or flash ok,bf5:0x%x\n", buffer[5]);
			return 0;
		} else if ((buffer[3] == 0x02) && (buffer[5] == 0x80)) {
			sparklink_errmsg(" repair 1162flash ok\n");
			return 0;
		} else if ((buffer[3] == 0x02) && (buffer[5] == 0xFF)) {
			hi1162_device_set_state(SPARKLINK_STATE_BADFLASH);
			sparklink_errmsg(" repair 1162flash fail\n");
			return -1; /* it will interrupt fw update */
		}
	} else {
		if ((buffer[3] == 0x02) && (buffer[5] == 0x01))
			return 0;
	}
	return -EIO;
}

static int hi1162_return_check(struct sparklink_uart_ops *uart_ops,
	int time_out, int check_flag, int response_len)
{
	int ret;
	size_t len = 0;
	u8 buffer[SPARKLINK_UART_BUFF_LEN] = {0};

	if (!uart_ops)
		return -EINVAL;

	ret = uart_ops->uart_read(buffer, &len, time_out, response_len);
	if (ret < 0) {
		sparklink_errmsg("uart read err ret = %d\n", ret);
		return ret;
	}
	ret = hi1162_recv_check(buffer, check_flag, &len);
	if (ret < 0) {
		/* record 24 byte date of uart transBuf/rcvBuf
		when fail, 8: index of sec U64, 16: index of 3rd U64 */
		sparklink_errmsg("rxBuf:size = %d 0x%016llx 0x%016llx 0x%016llx\n", len,
			*(u64 *)buffer, *(u64 *)(buffer + 8), *(u64 *)(buffer + 16));
		return ret;
	}
	return 0;
}

static int hi1162_send_data(struct sparklink_uart_ops *uart_ops, u8 *buf, size_t size)
{
	int ret;
	u8 crc;
	int offset = 0;
	int len = 0;

	if (!uart_ops || !buf)
		return -EINVAL;

	crc = g_serial_num;
	crc = crc_calc(crc, buf, size);
	/* add protocol header 0x5c 0x73 */
	g_hi1162_trans_buffer[len++] = HI1162_MSG_HEADER_1;
	g_hi1162_trans_buffer[len++] = HI1162_MSG_HEADER_2;
	/* add serialNum and change the 0x5c to 0x5c 0x5c */
	g_hi1162_trans_buffer[len++] = g_serial_num;
	if (g_serial_num == HI1162_MSG_HEADER_1)
		g_hi1162_trans_buffer[len++] = g_serial_num;

	/* change all the 0x5c to 0x5c 0x5c */
	while (offset < size && len < HI1162_UART_BUF_LEN) {
		if (buf[offset] == HI1162_MSG_HEADER_1)
			g_hi1162_trans_buffer[len++] = HI1162_MSG_HEADER_1;

		g_hi1162_trans_buffer[len++] = buf[offset++];
	}
	/* add crc to the buf end */
	g_hi1162_trans_buffer[len++] = crc;
	if (crc == 0x5c)
		g_hi1162_trans_buffer[len++] = crc;

	g_hi1162_trans_buffer[len++] = HI1162_MSG_TAIL_1;
	g_hi1162_trans_buffer[len++] = HI1162_MSG_TAIL_2;
	ret = uart_ops->uart_write(g_hi1162_trans_buffer, len);
	g_serial_num++;
	return ret;
}

static int hi1162_send_data_check(struct sparklink_uart_ops *uart_ops,
	u8 *buf, size_t size, int cmd_flag, int time_out, int response_len)
{
	int ret;

	if (!uart_ops || !buf)
		return -EINVAL;

	ret = hi1162_send_data(uart_ops, buf, size);
	if (ret < 0)
		return ret;

	if (cmd_flag == HI1162_NO_NEED_CHECK) {
		msleep(time_out);
		return ret;
	}

	ret = hi1162_return_check(uart_ops, time_out, cmd_flag, response_len);

	return ret;
}

static int hi1162_process_command(struct sparklink_uart_ops *uart_ops, hi1162_cmd *command)
{
	u8 cmd_buf[HI1162_CMD_MAX_LEN + 1] = {0};
	u8 cmd_len;
	int ret;
	long baud;

	if (!uart_ops || !command)
		return -EINVAL;

	cmd_buf[0] = command->id;
	cmd_len = command->len + 1;
	ret = memcpy_s(&cmd_buf[1], HI1162_CMD_MAX_LEN, command->cmdbuf, command->len);
	if (ret != EOK) {
		sparklink_errmsg("memcpy_s err ret = %d\n", ret);
		return ret;
	}

	ret = hi1162_send_data_check(uart_ops, cmd_buf, cmd_len,
		command->check_type, command->time_out, command->response_len);
	if (ret < 0) {
		sparklink_errmsg("hi1162_send_data err id = 0x%x, ret = %d\n", command->id, ret);
		return ret;
	}

	/* baud comes from speed cmd, 24:b[24-31], 16:b[16-23], 8:b[8-15], 0:b[0-7] */
	if (command->id == g_hi1162_speed_command.id) {
		baud = ((long)g_hi1162_speed_command.cmdbuf[0] << 24)
		+ ((long)g_hi1162_speed_command.cmdbuf[1] << 16)
		+ ((long)g_hi1162_speed_command.cmdbuf[2] << 8)
		+ ((long)g_hi1162_speed_command.cmdbuf[3]);
		uart_ops->reconfig_baudrate(baud);
	}
	return ret;
}

static int hi1162_process_data(struct sparklink_uart_ops *uart_ops, hi1162_cmd *command)
{
	int ret, remain;
	u32 addr = 0;
	u32 index = 0;
	size_t send_size;
	u8 *buffer = NULL;
	u32 packet_size = HI1162_DLOAD_PACKET_SIZE + 1;

	if (!uart_ops || !command || !command->firmware)
		return -EINVAL;

	remain = command->firmware->size;
	while (remain > 0) {
		ret = memset_s(g_hi1162_data_buf, sizeof(g_hi1162_data_buf), 0xFF, sizeof(g_hi1162_data_buf));
		if (ret != 0) {
			sparklink_errmsg("memset_s err ret = %d\n", ret);
			return ret;
		}
		buffer = &g_hi1162_data_buf[0];
		if (!buffer)
			return -1;

		buffer[0] = command->id;
		buffer++;
		/* 4:cmd length */
		if (command->len == 4) {
			ret = memcpy_s(buffer, HI1162_DLOAD_PACKET_SIZE, &index, sizeof(index));
			if (ret != EOK) {
				sparklink_errmsg("memcpy_s err ret = %d\n", ret);
				return ret;
			}
			buffer = buffer + sizeof(index);
			/* 5:index length + 1 */
			packet_size = HI1162_DLOAD_PACKET_SIZE + 5;
		}
		send_size = HI1162_DLOAD_PACKET_SIZE > remain ? remain : HI1162_DLOAD_PACKET_SIZE;
		ret = memcpy_s(buffer, HI1162_DLOAD_PACKET_SIZE,
			&(command->firmware->data[addr]), send_size);
		if (ret != EOK) {
			sparklink_errmsg("memcpy_s err ret = %d\n", ret);
			return ret;
		}
		addr += send_size;
		remain -= send_size;
		index++;
		sparklink_infomsg("data packet index = %d\n", index);
		ret = hi1162_send_data_check(uart_ops, g_hi1162_data_buf, packet_size,
			command->check_type, command->time_out, command->response_len);
		if (ret < 0) {
			sparklink_errmsg("hi1162_send_data err ret = %d\n", ret);
			return ret;
		}
	}
	return 0;
}

static int hi1162_process_cmd_data(struct sparklink_uart_ops *uart_ops, hi1162_cmd *command)
{
	int ret = -1;

	if (!uart_ops || !command)
		return -EINVAL;

	if (command->cmd_type == HI1162_CMD)
		ret = hi1162_process_command(uart_ops, command);
	else if (command->cmd_type == HI1162_DATA)
		ret = hi1162_process_data(uart_ops, command);

	return ret;
}

static int hi1162_command_prepare_bt_app(const struct firmware *img, u8 img_type, u32 ram_size)
{
	int ret;

	if (!img)
		return -EINVAL;

	/* set img_type to cmd pos 0 */
	g_hi1162_flash_erase_command.cmdbuf[0] = img_type;
	g_hi1162_start_page_command.cmdbuf[0] = img_type;
	g_hi1162_mem_config_command.cmdbuf[0] = img_type;
	g_hi1162_check_sum_command.cmdbuf[0] = img_type;
	g_hi1162_signature_command.cmdbuf[0] = img_type;
	/* add image size to mem_config cmd pos 1 */
	word_copy(&g_hi1162_mem_config_command.cmdbuf[1], img->size);
	/* add image size to mem_config cmd pos 5 */
	word_copy(&g_hi1162_mem_config_command.cmdbuf[5], ram_size);
	/* add hash message to check sum cmd pos 1 */
	ret = memcpy_s(g_hi1162_check_sum_command.cmdbuf + 1,
		HI1162_CMD_MAX_LEN - 1, g_hi1162_img_hash_out, HI1162_HASH_LEN_NEED);
	if (ret != EOK) {
		sparklink_errmsg("memcpy_s error\n");
		return ret;
	}
	/* add hash message to signature sum cmd pos 4 */
	ret = memcpy_s(g_hi1162_signature_command.cmdbuf + 4,
		HI1162_CMD_MAX_LEN - 4, g_hi1162_img_hash_out, HI1162_HASH_LEN_NEED);
	if (ret != EOK) {
		sparklink_errmsg("memcpy_s error\n");
		return ret;
	}
	g_hi1162_firmware_command.firmware = img;
	return 0;
}

static int hi1162_command_prepare_nv(const struct firmware *img, u8 img_type, u32 ram_size)
{
	int ret;

	if (!img)
		return -EINVAL;

	// cmd:0x36 content: b[0]--0 flash b[1:4] start_addr b[5-8] length
	g_hi1162_write_bin_command.cmdbuf[0] = (u8)0;
	word_copy(&g_hi1162_write_bin_command.cmdbuf[1], ram_size);
	word_copy(&g_hi1162_write_bin_command.cmdbuf[5], img->size);
	sparklink_infomsg("nv startaddr:0x%x img-size:0x%x\n", ram_size, img->size);
	word_copy(g_hi1162_sha_bin_command.cmdbuf, ram_size);
	word_copy(g_hi1162_sha_bin_command.cmdbuf + 4, img->size);
	ret = memcpy_s(g_hi1162_sha_bin_command.cmdbuf + 8,
		HI1162_CMD_MAX_LEN - 8, g_hi1162_img_hash_out, HI1162_HASH_LEN_NEED);
	if (ret != EOK) {
		sparklink_errmsg("memcpy_s error\n");
		return ret;
	}
	g_hi1162_firmware_command.firmware = img;
	return 0;
}

static int hi1162_command_prepare(const struct firmware *img, u8 img_type, u32 ram_size)
{
	int ret;

	if (!img)
		return -EINVAL;

	ret = hi1162_img_hash(img);
	if (ret < 0) {
		sparklink_errmsg("hi1162_img_hash failed\n");
		return ret;
	}

	switch (img_type) {
	case HI1162_SSBEFC_CORE_TYPE:
	case HI1162_SSB_CORE_TYPE:
		/* imag size pos:0 */
		word_copy(g_hi1162_ssb_sha_command.cmdbuf, img->size);
		/* hash data pos:4 and don't need check return value */
		ret = memcpy_s(g_hi1162_ssb_sha_command.cmdbuf + 4,
			HI1162_CMD_MAX_LEN - 4, g_hi1162_img_hash_out, HI1162_HASH_LEN_NEED);
		if (ret != EOK) {
			sparklink_errmsg("memcpy_s error\n");
			return ret;
		}
		g_hi1162_ssb_firmware_command.firmware = img;
		break;
	case HI1162_BT_CORE_TYPE:
	case HI1162_APP_CORE_TYPE:
		ret = hi1162_command_prepare_bt_app(img, img_type, ram_size);
		if (ret < 0) {
			sparklink_errmsg("prepare_bt_app error\n");
			return ret;
		}
		break;
	case HI1162_CKFF_CORE_TYPE:
	case HI1162_NV_CORE_TYPE:
	case HI1162_OTA_CORE_TYPE:
		ret = hi1162_command_prepare_nv(img, img_type, ram_size);
		if (ret < 0) {
			sparklink_errmsg("prepare_nv error\n");
			return ret;
		}
		break;
	default:
		return -EIO;
	}

	return 0;
}

static hi1162_update_info *hi1162_get_fwinfo(int index)
{
	int i;
	for (i = 0; i < arry_size(g_hi1162_update_info_data); i++) {
		if (index == g_hi1162_update_info_data[i].index)
			return &g_hi1162_update_info_data[i];
	}
	return NULL;
}

static int hi1162_process_list(struct sparklink_uart_ops *uart_ops,
	hi1162_cmd **process_list, int process_len)
{
	int ret, i;

	if ((uart_ops == NULL) || (process_list == NULL) || (*process_list == NULL))
		return -EINVAL;

	// retry the first command to avoid init fail
	for (i = 0; i < HI1162_UPDATE_RETRY; i++) {
		ret = hi1162_process_cmd_data(uart_ops, process_list[0]);
		if (ret < 0)
			continue;
		else
			break;
	}
	if (ret < 0) {
		sparklink_errmsg("process cmd-0x1 error 3 times\n");
		return ret;
	}
	for (i = 1; i < process_len; i++) {
		ret = hi1162_process_cmd_data(uart_ops, process_list[i]);
		if (ret < 0) {
			sparklink_errmsg("process cmd error process_list i = %d\n", i);
			return ret;
		}
	}
	return 0;
}

static int hi1162_device_check_mode(struct sparklink_uart_ops *ops,
	hi1162_update_info *update_info)
{
	int ret;
	u8 entry_type = HI1162_TYPE_ERROR;

	if (!ops || !update_info)
		return -EINVAL;

	ret = hi1162_device_get_mode(ops, &entry_type);
	if (ret < 0) {
		sparklink_errmsg("hi1162_device_get_mode error\n");
		return ret;
	}

	if (entry_type != update_info->entry_check) {
		if (update_info->need_erase_ssb) {
			ret = hi1162_process_list(ops, g_hi1162_ssb_erase_cmd_list,
				arry_size(g_hi1162_ssb_erase_cmd_list));
			if (ret < 0)
				sparklink_errmsg("ssb erase fail\n");
			else
				sparklink_errmsg("ssb erase ok\n");
			return ret;
		} else {
			sparklink_errmsg("update_info->need_erase_ssb is NULL\n");
			return -EIO;
		}
	}
	return 0;
}

static int hi1162_process_update(struct sparklink_uart_ops *ops,
	struct hi1162_device_info *di, hi1162_update_info *update_info)
{
	int ret;

	if (!ops || !di || !update_info)
		return -EINVAL;

	ret = hi1162_device_check_mode(ops, update_info);
	if (ret < 0) {
		sparklink_errmsg("device entry check fail\n");
		return ret;
	}

	ret = hi1162_process_list(ops, update_info->list, update_info->list_len);
	if (ret < 0) {
		sparklink_errmsg("hi1162_process_list fail ret = %d\n", ret);
		return ret;
	}
	if (update_info->need_check_id) {
		ret = hi1162_check_id(di);
		if (ret < 0) {
			sparklink_errmsg("check id fail\n");
			return ret;
		}
	}

	/* make sure the hash verifycaton finished after flash ssbefc ssb */
	if (update_info->need_delay)
		msleep(3000); /* sleep 3s */

	return 0;
}

/* chip_prod waiting for firmware is 10s-60s*/
static void wait_chip_prod_mount(int timeout)
{
	int wait_time;
	unsigned long old_fs;
	const int schedule_time = 200;
	const char *chip_prod_partition = "/chip_prod/etc/firmware/sparklink";
	const char *odm_prod_partition = "/odm/etc/firmware/sparklink";

	old_fs = get_fs();

	for (wait_time = 0; wait_time < timeout; wait_time += schedule_time) {
		if (!ksys_access(chip_prod_partition, 0) || !ksys_access(odm_prod_partition, 0)) {
			sparklink_errmsg("wait_time is: %d ms\n", wait_time);
			break;
		}

		msleep(schedule_time);
	}
	if (wait_time == timeout)
		sparklink_errmsg("access fs timeout\n");

	set_fs(old_fs);
}

/* the caller must be ensure that the ops and di is not null */
static int hi1162_fwupdate_image(struct sparklink_uart_ops *ops,
	struct hi1162_device_info *di, int index)
{
	int ret, retry;
	const struct firmware *firmware = NULL;
	int step = 0;
	hi1162_update_info *update_info = NULL;

	if (!ops || !di)
		return -EINVAL;

	update_info = hi1162_get_fwinfo(index);
	if (!update_info) {
		sparklink_errmsg("hi1162_fwupdate error id=%d\n", index);
		return -EINVAL;
	}
	step++;

	wait_chip_prod_mount(WAIT_CHIP_PROD_MS);

	ret = request_firmware(&firmware, update_info->fw_path, di->dev);
	if (ret < 0) {
		sparklink_errmsg("request firmware fail %d\n", ret);
		return ret;
	}
	step++;

	/* get img hash and flash  command generation */
	ret = hi1162_command_prepare(firmware, update_info->index, update_info->ram_size);
	if (ret < 0)
		goto REALSE_FIRMWARE;
	step++;

	ret = hi1162_uart_init(ops);
	if (ret < 0)
		goto REALSE_FIRMWARE;
	step++;

	for (retry = 0; retry < HI1162_UPDATE_RETRY; retry++) {
		ret = hi1162_process_update(ops, di, update_info);
		if (ret < 0) {
			sparklink_errmsg("process_update err ret=%d, retry=%d\n", ret, retry);
			/* do not retry download for hi1162CKFF.bin in here */
			if (index == HI1162_CKFF_CORE_TYPE) {
				sparklink_errmsg("ckff download fail and goto downlaoder ssbefc\n");
				break;
			}
		} else {
			break;
		}
	}
	step++;

	if (index == HI1162_OTA_CORE_TYPE)
		msleep(10000); /* it will delay 10s after updating ota for 1162 */

	hi1162_uart_deinit(ops);
REALSE_FIRMWARE:
	release_firmware(firmware);
	sparklink_errmsg("exit step = %d\n", step);
	return ret;
}

static int hi1162_init_fw_info(struct hi1162_device_info *di)
{
	int i;

	if (!di) {
		sparklink_errmsg("input invalid error\n");
		return -EINVAL;
	}

	for (i = 0; i < HI1162_FW_INFO_MAX; i++) {
		g_hi1162_update_info_data[i].index = di->fw_info[i].addr;
		g_hi1162_update_info_data[i].ram_size = di->fw_info[i].size;
		g_hi1162_update_info_data[i].flash_size = di->fw_info[i].value;
		g_hi1162_update_info_data[i].fw_path = di->fw_path[i];
	}

	g_hi1162_uart_port = di->tty_name;
	g_hi1162_uart_default_baud = di->default_baud;
	g_hi1162_uart_max_baud = di->max_baud;
	/* speed cmd comes from max baud, 24:b[24-31], 16:b[16-23], 8:b[8-15], 0:b[0-7] */
	g_hi1162_speed_command.cmdbuf[0] = (u8)(g_hi1162_uart_max_baud >> 24);
	g_hi1162_speed_command.cmdbuf[1] = (u8)(g_hi1162_uart_max_baud >> 16);
	g_hi1162_speed_command.cmdbuf[2] = (u8)(g_hi1162_uart_max_baud >> 8);
	g_hi1162_speed_command.cmdbuf[3] = (u8)(g_hi1162_uart_max_baud);

	return 0;
}

static int hi1162_bad_flash_check(struct sparklink_uart_ops *ops, struct hi1162_device_info *di)
{
	int ret, i;
	int j = 0;
	u8 entry_type = HI1162_TYPE_ERROR;

	if (!ops || !di) {
		sparklink_errmsg("input invalid error\n");
		return -EINVAL;
	}

	ret = hi1162_uart_init(ops);
	if (ret < 0)
		return ret;

	hi1162_device_get_mode(ops, &entry_type);
	/* if it is codeloader_type; it will not do flash-check */
	if (entry_type != HI1162_ROMLOADER_TYPE) {
		sparklink_errmsg("go out bad_flash_check because of codelaoder or other type!\n");
		hi1162_uart_deinit(ops);
		return 0;
	}
	/* adapt for the previous hi1162_uart_init */
	hi1162_uart_deinit(ops);

	for (i = HI1162_SSBEFC_FW_INFO; i < HI1162_SSB_FW_INFO; i++) {
		if (di->need_update[i]) {
			ret = hi1162_fwupdate_image(ops, di, di->fw_info[i].addr);
			if (ret >= 0) {
				sparklink_errmsg("flash-check fwupdate sucess %s\n", di->fw_path[i]);
				continue;
			}

			/* if download hi1162CKFF.bin fail,it will downlaod from hi1162SSBEFC.bin */
			if (i == HI1162_CKFF_FW_INFO && j < HI1162_REPAIR_FF_RETRY) {
				i = -1; /* retry from ssbefc */
				j++;
				continue;
			}
			sparklink_errmsg("flash-check fwupdate_image fail %s\n", di->fw_path[i]);
			return ret;
		}
	}
	sparklink_errmsg("flash check ff done!\n");
	return 0;
}

static int romloader_type_fwupdate(struct sparklink_uart_ops *ops, struct hi1162_device_info *di)
{
	int i, ret;

	if (!ops || !di) {
		sparklink_errmsg("input invalid error\n");
		return -EINVAL;
	}

	/* after repair check,the 1162 is romloader */
	for (i = HI1162_SSB_FW_INFO; i < HI1162_OTA_FW_INFO; i++) {
		if (di->need_update[i]) {
			ret = hi1162_fwupdate_image(ops, di, di->fw_info[i].addr);
			if (ret < 0) {
				sparklink_errmsg("fwupdate_image fail %s\n", di->fw_path[i]);
				return ret;
			}
			sparklink_infomsg("fwupdate sucess %s\n", di->fw_path[i]);
		}
	}

	return 0;
}

static int codeloader_type_fwupdate(struct sparklink_uart_ops *ops, struct hi1162_device_info *di)
{
	int ret;

	if (!ops || !di) {
		sparklink_errmsg("input invalid error\n");
		return -EINVAL;
	}

	/* when nv update fail, make sure that next boot can update nv */
	/* nv can only be updated in factory version */
#ifdef CONFIG_FACTORY_MODE
	if (di->need_update[HI1162_NV_FW_INFO] == true) {
		sparklink_errmsg("it will nv upgrade in codeloader\n");
		/* it will do nv update,because of codeloader */
		ret = hi1162_fwupdate_image(ops, di, di->fw_info[HI1162_NV_FW_INFO].addr);
		if (ret < 0) {
			sparklink_errmsg("fwupdate_image fail %s\n", di->fw_path[HI1162_NV_FW_INFO]);
			return ret;
		}
		sparklink_infomsg("fwupdate sucess %s\n", di->fw_path[HI1162_NV_FW_INFO]);
	} else {
		sparklink_errmsg("codeloader and no need nv\n");
	}
#endif

	if (di->need_update[HI1162_OTA_FW_INFO] == true) {
		sparklink_errmsg("it will ota upgrade in codeloader\n");
		/* it will do ota update,because of codeloader */
		ret = hi1162_fwupdate_image(ops, di, di->fw_info[HI1162_OTA_FW_INFO].addr);
		if (ret < 0) {
			sparklink_errmsg("fwupdate_image fail %s\n", di->fw_path[HI1162_OTA_FW_INFO]);
			return ret;
		}
		sparklink_infomsg("fwupdate sucess %s\n", di->fw_path[HI1162_OTA_FW_INFO]);
	} else {
		sparklink_errmsg("codeloader and no need ota\n");
	}

	return 0;
}

static int fw_update_by_type(u8 entry_type, struct sparklink_uart_ops *ops, struct hi1162_device_info *di)
{
	int ret = 0;

	if (!ops || !di) {
		sparklink_errmsg("input invalid error\n");
		return -EINVAL;
	}

	switch (entry_type) {
	case HI1162_ROMLOADER_TYPE:
		ret = romloader_type_fwupdate(ops, di);
		break;

	case HI1162_CODELOADER_TYPE:
		ret = codeloader_type_fwupdate(ops, di);
		break;

	default:
		/* 1162 is neither romloader nor codeloader */
		sparklink_errmsg("type-rom-or-code is unknow\n");
		ret = -EINVAL;
		break;
	}

	return ret;
}

int hi1162_fwupdate(struct sparklink_uart_ops *ops, struct hi1162_device_info *di)
{
	int ret;
	u8 entry_type = HI1162_TYPE_ERROR;

	if (!ops || !di)
		return -EINVAL;

	ret = hi1162_init_fw_info(di);
	if (ret < 0)
		return ret;

	if (di->need_update[HI1162_SSBEFC_FW_INFO]) {
		ret = hi1162_bad_flash_check(ops, di);
		if (ret < 0) {
			sparklink_errmsg("hi1162_check flash fail\n");
			return ret;
		}
	}

	if (hi1162_device_get_chipid_state() == SPARKLINK_CHIPID_SUCCESS) {
		entry_type = HI1162_CODELOADER_TYPE;
	} else {
		sparklink_errmsg("read chipid fail and it will get rom or code\n");
		/* codeloader--ssb nv bt app; romloader--ota upgrade */
		ret = hi1162_uart_init(ops);
		if (ret < 0)
			return ret;

		hi1162_device_get_mode(ops, &entry_type);
		sparklink_errmsg("the entry_type is %d\n", entry_type);
		hi1162_uart_deinit(ops);
	}

	ret = fw_update_by_type(entry_type, ops, di);
	if (ret < 0)
		return ret;

	sparklink_infomsg("hi1162 fwupdate sucess\n");
	return 0;
}
