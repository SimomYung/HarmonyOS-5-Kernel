/*
 * Thp driver code for chipone
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <securec.h>
#include "huawei_pen_clk.h"
#include "huawei_thp.h"
#include "bus_adapter/huawei_bus_adapter.h"
#include "bus_adapter/huawei_spi_bus_platform.h"
#include "bus_adapter/huawei_spi_bus_config_parse.h"

#define MOVE_0BIT 0
#define MOVE_8BIT 8
#define MOVE_16BIT 16
#define MOVE_24BIT 24
#define MASK_8BIT 0xFF
#define OFFSET_0 0
#define OFFSET_1 1
#define OFFSET_2 2
#define OFFSET_3 3
#define OFFSET_4 4
#define OFFSET_5 5
#define SINGLE_CLICK 0x0001
#define DOUBLE_CLICK 0x0002
#define BOTH_CLICK 0x0003
#define SPEED_HZ_DRW (8 * 1000 * 1000)
#define SPEED_HZ_TRS (6 * 1000 * 1000)
#define SPEED_HZ_DEFAULT 0
#define TOUCH_GSTR_SIZE 112
#define TI_UN_CON 1000
#define DETECT_RETRY_TIME 3
#define DETECT_RETRY_DELAY_MS 6
#define DRW_MODE_RETRY_DELAY_MS 6
#define SPI_DELAY_MS 5
#define POWER_DELAY_5MS 5
#define SPI_RETRY_TIMES 5
#define SET_GESTURE_MODE_RETRY_TIMES 3
#define TOUCH_DRV_VERSION "v1.0"
#define TOUCH_IC_NAME "chipone"
#define TOUCH_DEV_NODE_NAME "chipone_thp"
#define TOUCH_PROJECT_ID_LEN 10

/* commands */
#define TOUCH_DRW_RD_CMD 0x61
#define TOUCH_DRW_WR_CMD 0x60
#define TOUCH_NORM_RD_CMD 0xF1
#define TOUCH_NORM_WR_CMD 0xF0
/* hwid */
#define TOUCH_ICNL9268_HWID 0x00009268
#define TOUCH_ICNL9922C_HWID 0x99C220
#define TOUCH_ICNL9951R_HWID 0x991510
#define TOUCH_ICNL9952_HWID  0x990520
#define TOUCH_IC_HWID_MASK 0x0000FFFF
#define TDDI_HWID_MASK 0xFFFFF0

/* registers */
#define HW_REG_HWID_9922 0x70000
#define TOUCH_REG_HWID_9288 0x870000
#define TOUCH_REG_CMD_SEL 0x74000
#define TOUCH_REG_FLASH_ADDR 0x74004
#define TOUCH_REG_SRAM_ADDR 0x74008
#define TOUCH_REG_DATA_LENGTH 0x7400C
#define TOUCH_REG_START_DEXC 0x74010
#define TOUCH_REG_BUSY 0x74024
#define TOUCH_PROJECT_ID_ADDR 0x0000000E
#define TOUCH_RST_FLASH_VOL_REG1 0x71034
#define TOUCH_RST_FLASH_VOL_REG2 0x74084

/* buffer size */
#define CHIPONE_MAX_LOG_SIZE 128
#define TOUCH_SPI_BUF_SIZE(x) ((x) - CHIPONE_MAX_LOG_SIZE)
#define TOUCH_TCS_HEAD_SIZE sizeof(struct touch_tcs_head)
#define TOUCH_TCS_TAIL_SIZE sizeof(struct touch_cspi_tail)
#define TOUCH_FRAME_MAX_SIZE(x) ((x) - 4 * ONE_PACKET_FRAME_SIZE)
#define TOUCH_DUAL_MAX_FRAME_SIZE(x) (TOUCH_SPI_BUF_SIZE((x)) - TOUCH_TCS_TAIL_SIZE)
#define TOUCH_FRAME_MIN_SIZE (TOUCH_TCS_HEAD_SIZE + TOUCH_TCS_TAIL_SIZE)

/* retries count */
#define TOUCH_ENTER_DRW_RETRIES 3
#define TOUCH_WAIT_BUS_RETRIES 3
#define THP_RESET_WAIT_TIME 10
#define TOUCH_FLASH_BUSY_DELAY 1

#define TOUCH_SWITCH_INT_DELAY_TIME_MS 2

/* log buf */
# define STR_START (g_logbuf + offset)
# define STR_SIZ ((sizeof(g_logbuf) - offset))

/* gesture flag */
#define CMD_BUF_SIZE 64
#define TCS_ERR_CODE_OK 0
#define BOOT_MODE_SRAM 0x03
#define TARGET_IC_MASTER 0x81
#define SWITCH_IC_RETRY_TIMES 2
#define GESTURE_INFO_LEN 112
#define GESTURE_D_TAP_VAL 0x50
#define GESTURE_PEN_S_TAP_VAL 0x60
#define ENABLE_SINGLE_CLICK 0x01
#define ENABLE_DOUBLE_CLICK 0x02
#define ENABLE_STYLUS_CLICK 0x04
#define ENTER_PROG_MODE_RETRY_TIMES 5
#define ENTER_PROG_MODE_RETRY_DELAY 10

#define STYLUS_CONNECT (1 << 1)
#define SEND_BT_CONNECT_STATUS_AFTER_RESET_DELAY 200
#define TOUCH_SEND_CMD_RETRIES 3
#define TOUCH_ENTER_STYLUS_CHECK_LEN 5
#define TOUCH_ENTER_STYLUS_DELAY 1
#define ENTER_STYLUS_ERR_CHECK 0x0
#define ENTER_STYLUS_CMD1_CHECK 0x5e
#define ENTER_STYLUS_CMD2_CHECK 0x22
#define STYLUS_ADSORPTION_STATUS_ON 0
#define STYLUS_A63_CONNECT  0x40
#define ENTER_STYLUS_NO_TDDI_CMD1_CHECK 0x20
#define ENTER_STYLUS_NO_TDDI_CMD2_CHECK 0x31
#define ENTER_STYLUS_NO_TDDI_CMD3_CHECK 0xa6
#define ENTER_STYLUS_NO_TDDI_CMD4_CHECK 0xc0

enum touch_ic_type {
	IC_TYPE_ICNL9922C = 2,   /* 0x99C220 */
	IC_TYPE_ICNL9951R = 3,   /* 0x991510 */
	IC_TYPE_ICNL9952 = 4,   /* 0x990520 */
};

static uint8_t g_cmd_rbuf[CMD_BUF_SIZE];

#if defined(CONFIG_HUAWEI_THP_QCOM)
static u8 g_lowpower_mode;
#endif
static u32 g_touch_frame_size;

/* structs for communication */
#pragma pack(push, 1)
struct touch_drw_head {
	uint8_t rwcmd;
	uint8_t addr[3];
	uint8_t len[3];
	uint8_t crc[2];
	uint8_t wait[4];
};

struct touch_drw_tail {
	uint8_t crc[2];
	uint8_t wait[1];
	uint8_t ack[2];
};

struct touch_tcs_head {
	uint8_t  dummy[8];
	uint16_t curr_size;
	uint16_t next_size;
};

struct touch_cspi_tail {
	uint8_t ecode;
	uint8_t cmd[2];
	uint8_t crc[2];
};
#pragma pack(pop)

static uint8_t g_logbuf[CHIPONE_MAX_LOG_SIZE];

/* Get frame data command */
static uint8_t get_frame_cmd[] = {
	TOUCH_NORM_RD_CMD, 0x05, 0x51, 0x70, 0x00, 0xAA, 0x4A
};
/* Get frame data command */
static uint8_t get_frame_cmd_tddi[] = {
		TOUCH_NORM_RD_CMD, 0x22, 0x41, 0xD4, 0x04, 0xF4, 0x7F
};
/* Enable single click gesture */
static uint8_t single_click_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x28, 0x23, 0x04, 0x00, 0x33, 0x90,
	0x00, 0x00, 0x80, 0x00, 0x09, 0x80
};
/* Enable double click gesture */
static uint8_t double_click_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x28, 0x23, 0x04, 0x00, 0x33, 0x90,
	0x01, 0x00, 0x00, 0x00, 0x03, 0x94
};
/* Enable both click gesture */
static uint8_t both_click_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x28, 0x23, 0x04, 0x00, 0x90, 0x33,
	0x01, 0x00, 0x80, 0x00, 0x0A, 0x14
};
/* Enter sleep mode, disable gesture */
static uint8_t sleep_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x04, 0x22, 0x01, 0x00, 0x28, 0x7E,
	0x02, 0x0F, 0x80
};
/* Set gesture report format */
static uint8_t gesture_format_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x40, 0x22, 0x01, 0x00, 0x36, 0x2E,
	0x00, 0x00, 0x00
};
#if defined(CONFIG_HUAWEI_THP_QCOM)
/* Confirm gesture report received */
static uint8_t gesture_confirm_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x1E, 0x23, 0x01, 0x00, 0x39, 0x36,
	0x00, 0x00, 0x00
};
#endif
/* Workmode cmd */
static uint8_t get_workmode_cmd_tddi[] = {
	TOUCH_NORM_RD_CMD, 0x01, 0x40, 0x01, 0x00, 0xF8, 0x3D
};
#if defined(CONFIG_HUAWEI_THP_QCOM)
/* Get gesture data command */
static uint8_t get_gesture_cmd_tddi[] = {
	TOUCH_NORM_RD_CMD, 0x03, 0x41, 0x70, 0x00, 0xE9, 0xB3
};
#endif
/* Enter gesture mode */
static uint8_t gesture_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x04, 0x22, 0x01, 0x00, 0x28, 0x7E,
	0x03, 0x0A, 0x00
};
/*ICNL9952 enter gesture mode cmd*/
static uint8_t enter_gesture_mode[] = {
	TOUCH_NORM_WR_CMD, 0x04, 0x22, 0x10, 0x00, 0x28, 0x18,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0A, 0x0A
};

static uint8_t stylus_enable_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x5e, 0x22, 0x01, 0x0, 0x33, 0x36,
	0x01, 0x05, 0x80
};

static uint8_t stylus_disable_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x5e, 0x22, 0x01, 0x0, 0x33, 0x36,
	0x0, 0x0, 0x0
};

static uint8_t stylus_adsorption_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x5e, 0x22, 0x01, 0x0, 0x33, 0x36,
	0x02, 0x0f, 0x80
};

static uint8_t stylus_a63_enable_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x5e, 0x22, 0x01, 0x0, 0x33, 0x36,
	0x05, 0x1e, 0x0
};

static uint8_t stylus_a63_disable_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x5e, 0x22, 0x01, 0x0, 0x33, 0x36,
	0x04, 0x1b, 0x80
};

static uint8_t stylus_a63_adsorption_cmd_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x5e, 0x22, 0x01, 0x0, 0x33, 0x36,
	0x06, 0x14, 0x0
};

static uint8_t stylus_enable_cmd_no_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x20, 0x31, 0x01, 0x00, 0x58, 0xaf,
	0x01, 0x05, 0x80, 0x00, 0x00
};

static uint8_t stylus_disable_cmd_no_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x20, 0x31, 0x01, 0x00, 0x58, 0xaf,
	0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t stylus_adsorption_cmd_no_tddi[] = {
	TOUCH_NORM_WR_CMD, 0x20, 0x31, 0x01, 0x00, 0x58, 0xaf,
	0x01, 0x05, 0x80, 0x00, 0x00
};

//double click & stylus gesture
static uint8_t double_click_stylus_gesture_enable_cmd[] = {
	TOUCH_NORM_WR_CMD, 0x31, 0x23, 0x01, 0x00, 0x35, 0x7A,
	0x01, 0x05, 0x80
};

struct ts_hw_id_table_entry {
	uint32_t hw_id;
	uint32_t mask;
};

struct stylus_status_command_config {
	const uint8_t* enable_cmd;
	size_t enable_size;
	const uint8_t* adsorp_cmd;
	size_t adsorp_size;
	const uint8_t* disable_cmd;
	size_t disable_size;
};

static const struct stylus_status_command_config a63_config = {
	.enable_cmd = stylus_a63_enable_cmd_tddi,
	.enable_size = sizeof(stylus_a63_enable_cmd_tddi),
	.adsorp_cmd = stylus_a63_adsorption_cmd_tddi,
	.adsorp_size = sizeof(stylus_a63_adsorption_cmd_tddi),
	.disable_cmd = stylus_a63_disable_cmd_tddi,
	.disable_size = sizeof(stylus_a63_disable_cmd_tddi),
};

static const struct stylus_status_command_config default_config = {
	.enable_cmd = stylus_enable_cmd_tddi,
	.enable_size = sizeof(stylus_enable_cmd_tddi),
	.adsorp_cmd = stylus_adsorption_cmd_tddi,
	.adsorp_size = sizeof(stylus_adsorption_cmd_tddi),
	.disable_cmd = stylus_disable_cmd_tddi,
	.disable_size = sizeof(stylus_disable_cmd_tddi),
};

static const struct stylus_status_command_config no_tddi_config = {
	.enable_cmd = stylus_enable_cmd_no_tddi,
	.enable_size = sizeof(stylus_enable_cmd_no_tddi),
	.adsorp_cmd = stylus_adsorption_cmd_no_tddi,
	.adsorp_size = sizeof(stylus_adsorption_cmd_no_tddi),
	.disable_cmd = stylus_disable_cmd_no_tddi,
	.disable_size = sizeof(stylus_disable_cmd_no_tddi),
};

static const struct ts_hw_id_table_entry hw_id_table[] = {
    {
        .hw_id = TOUCH_ICNL9268_HWID,
        .mask = TOUCH_IC_HWID_MASK
	},
    {
        .hw_id = TOUCH_ICNL9922C_HWID,
        .mask = TDDI_HWID_MASK
    },
    {
        .hw_id = TOUCH_ICNL9951R_HWID,
        .mask = TDDI_HWID_MASK
    },
    {
        .hw_id = TOUCH_ICNL9952_HWID,
        .mask = TDDI_HWID_MASK
    },
};

static void touch_hw_reset(struct thp_device *tdev);
#ifdef CONFIG_HUAWEI_SHB_THP
static int touch_driver_switch_int_sh(struct thp_device *tdev);
static int touch_driver_switch_int_ap(struct thp_device *tdev);
static void touch_driver_poweroff_status(struct thp_device *tdev);
#endif

static inline void put_unaligned_be16(void *p, uint16_t v)
{
	uint8_t *pu8 = (uint8_t *)p;

	pu8[OFFSET_0] = (v >> MOVE_8BIT) & MASK_8BIT;
	pu8[OFFSET_1] = (v >> MOVE_0BIT) & MASK_8BIT;
}

static inline void put_unaligned_be24(void *p, uint32_t v)
{
	uint8_t *pu8 = (uint8_t *)p;

	pu8[OFFSET_0] = (v >> MOVE_16BIT) & MASK_8BIT;
	pu8[OFFSET_1] = (v >> MOVE_8BIT) & MASK_8BIT;
	pu8[OFFSET_2] = (v >> MOVE_0BIT) & MASK_8BIT;
}

static inline uint16_t get_unaligned_be16(const void *p)
{
	const uint8_t *pu8 = (const uint8_t *)p;

	return ((pu8[OFFSET_0] << MOVE_8BIT) | pu8[OFFSET_1]);
}

static inline uint16_t get_unaligned_le16(const void *p)
{
	const uint8_t *pu8 = (const uint8_t *)p;

	return (pu8[OFFSET_0] | (pu8[OFFSET_1] << MOVE_8BIT));
}

static inline uint32_t get_unaligned_le32(const void *p)
{
	const uint8_t *pu8 = (const uint8_t *)p;

	return (pu8[OFFSET_0] | (pu8[OFFSET_1] << MOVE_8BIT) |
		(pu8[OFFSET_2] << MOVE_16BIT) | (pu8[OFFSET_3] << MOVE_24BIT));
}

static inline long tvdiff2ms(struct timeval *start, struct timeval *end)
{
	return (end->tv_sec - start->tv_sec) * TI_UN_CON +
			(end->tv_usec - start->tv_usec) / TI_UN_CON;
}

/* calculate 16bit crc for data */
const static uint16_t crc16_table[] = {
	0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
	0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
	0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
	0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
	0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
	0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
	0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
	0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
	0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
	0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
	0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
	0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
	0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
	0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
	0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
	0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
	0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
	0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
	0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
	0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
	0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
	0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
	0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
	0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
	0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
	0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
	0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
	0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
	0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
	0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
	0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
	0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202,
};

static int g_thp_udfp_status;
static unsigned int chipone_stylus_status;
static unsigned int chipone_ring_setting_switch;
static unsigned int g_aod_touch_status;

static void touch_driver_log_buf(char *prefix, uint8_t *data, size_t datalen)
{
	int offset = 0;
	unsigned int i;

	offset += snprintf_s(STR_START, STR_SIZ, STR_SIZ - 1, "%s", prefix);
	if (data && datalen) {
		for (i = 0; i < datalen && offset < sizeof(g_logbuf); i++)
			offset += snprintf_s(STR_START, STR_SIZ, STR_SIZ - 1, " %02x", data[i]);
	}

	thp_log_info("%s", g_logbuf);
}

static uint16_t touch_driver_crc16(const uint8_t *data, size_t len)
{
	uint16_t crc16 = 0;
	uint8_t index;

	while (len) {
		index = (((crc16 >> MOVE_8BIT) ^ *data) & MASK_8BIT);
		crc16 = (crc16 << MOVE_8BIT) ^ crc16_table[index];
		data++;
		len--;
	}

	return crc16;
}

/* power api */
static int touch_driver_power_init(u32 panel_id)
{
	int ret;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (cd->power_cfg.power_supply_mode)
		return NO_ERR;

	ret = thp_power_supply_get(THP_VCC, panel_id);
	if (ret)
		thp_log_err("%s: fail to get vcc power\n", __func__);
	ret = thp_power_supply_get(THP_IOVDD, panel_id);
	if (ret)
		thp_log_err("%s: fail to get power\n", __func__);
	return NO_ERR;
}

static int touch_driver_power_release(u32 panel_id)
{
	int ret;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (cd->power_cfg.power_supply_mode)
		return NO_ERR;

	ret = thp_power_supply_put(THP_VCC, panel_id);
	if (ret)
		thp_log_err("%s: fail to release vcc power\n", __func__);
	ret = thp_power_supply_put(THP_IOVDD, panel_id);
	if (ret)
		thp_log_err("%s: fail to release power\n", __func__);
	return ret;
}

static int touch_driver_power_on(struct thp_device *tdev)
{
	int ret;
	struct thp_core_data *cd = thp_get_core_data(tdev->thp_core->panel_id);

	if (cd->power_cfg.power_supply_mode)
		return NO_ERR;
	thp_log_info("%s:called\n", __func__);

	gpio_direction_input(tdev->gpios->irq_gpio);
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
	ret = thp_power_supply_ctrl(THP_VCC, THP_POWER_ON, 1, cd->panel_id); /* delay 1ms */
	if (ret)
		thp_log_err("%s:power ctrl fail\n", __func__);
	ret = thp_power_supply_ctrl(THP_IOVDD, THP_POWER_ON,
		cd->iovdd_power_on_delay_ms, cd->panel_id);
	if (ret)
		thp_log_err("%s:power ctrl vddio fail\n", __func__);
	gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);

	touch_hw_reset(tdev);

	return ret;
}

static int touch_driver_power_off(struct thp_device *tdev)
{
	int ret;
	struct thp_core_data *cd = thp_get_core_data(tdev->thp_core->panel_id);

	if (cd->power_cfg.power_supply_mode)
		return NO_ERR;
	thp_log_debug("%s: in\n", __func__);

	gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
	mdelay(tdev->timing_config.boot_reset_low_delay_ms);
	gpio_set_value(tdev->gpios->cs_gpio, GPIO_LOW);

	ret = thp_power_supply_ctrl(THP_IOVDD, THP_POWER_OFF, POWER_DELAY_5MS, cd->panel_id);
	if (ret)
		thp_log_err("%s:power ctrl fail\n", __func__);
	ret = thp_power_supply_ctrl(THP_VCC, THP_POWER_OFF, POWER_DELAY_5MS, cd->panel_id);
	if (ret)
		thp_log_err("%s:power ctrl vcc fail\n", __func__);

	return ret;
}

/* transfer spi data */
static int touch_driver_spi_transfer(struct thp_device *tdev,
	void *tbuf, void *rbuf, size_t len, uint32_t speed)
{
	struct spi_transfer xfer = {
		.tx_buf = tbuf,
		.rx_buf = rbuf,
		.len = len,
		.speed_hz = speed,
	};
	struct spi_message msg;
	int retry = 0;
	int rc;

	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);
	while (retry < SPI_RETRY_TIMES) {
		rc = thp_spi_sync(tdev->sdev, &msg);
		if (rc) {
			thp_log_err("%s: spi_sync failed: rc=%d, retry=%d\n",
				__func__, rc, retry);
			retry++;
			mdelay(SPI_DELAY_MS);
			continue;
		} else {
			break;
		}
	}

	return rc;
}

static int touch_driver_write_dual_protocals_cmd(struct thp_device *tdev,
	void *cmd, size_t cmd_len)
{
	int ret = NO_ERR;

	if (tdev == NULL) {
		thp_log_err("%s: tdev is null\n", __func__);
		return -EINVAL;
	}
	if (memcpy_s(tdev->tx_buff, TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(tdev->thp_core)),
		cmd, cmd_len) != EOK) {
		thp_log_err("%s: copy cmd to tx_buf failed: ret = %d\n", __func__, ret);
		return -EINVAL;
	}
	if (memcpy_s(tdev->rx_buff, TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(tdev->thp_core)),
		g_cmd_rbuf, cmd_len) != EOK) {
		thp_log_err("%s: copy g_cmd_rbuf to rx_buff failed: ret = %d\n", __func__, ret);
		return -EINVAL;
	}
	ret = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff, cmd_len, SPEED_HZ_DEFAULT);
	if (ret)
		thp_log_err("%s: cmd send failed: ret=%d\n", __func__, ret);

	return ret;
}

static int touch_driver_read_dual_protocals_checkvalue(struct thp_device *tdev)
{
	int ret = NO_ERR;

	ret = memset_s(tdev->rx_buff, TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(tdev->thp_core)),
		0, CMD_BUF_SIZE);
	if (ret) {
		thp_log_err("%s: init rx_buff failed: ret = %d\n", __func__, ret);
		return ret;
	}
	ret = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		TOUCH_TCS_TAIL_SIZE, SPEED_HZ_DEFAULT);
	if (ret) {
		thp_log_err("%s: read check value failed: ret = %d\n", __func__, ret);
		return ret;
	}
	ret = memcpy_s(g_cmd_rbuf, CMD_BUF_SIZE, tdev->rx_buff, TOUCH_TCS_TAIL_SIZE);
	if (ret)
		thp_log_err("%s: copy check value to g_cmd_rbuf failed: ret = %d\n",
			__func__, ret);

	return ret;
}

static int touch_driver_check_dual_protocals(uint8_t *buf)
{
	struct touch_cspi_tail *ptail = NULL;
	size_t crc_offset = offsetof(struct touch_cspi_tail, crc);
	uint16_t crc16_calc;
	uint16_t crc16_recv;

	/* get_workmode_cmd no need data crc check */
	if (buf == get_workmode_cmd_tddi) {
		thp_log_info("%s: no need data crc check in workmode\n", __func__);
		return NO_ERR;
	}
	ptail = (struct touch_cspi_tail *)g_cmd_rbuf;
	crc16_calc = touch_driver_crc16((uint8_t *)ptail, crc_offset);
	crc16_recv = ptail->crc[0] | (ptail->crc[1] << MOVE_8BIT);
	if (crc16_calc != crc16_recv) {
		thp_log_err("%s:crc error: calc %#06x != %#06x recv\n",
			__func__, crc16_calc, crc16_recv);
		return -EINVAL;
	} else {
		if (ptail->ecode) {
			thp_log_err("%s:cspi recv errno: %#04x\n", __func__, ptail->ecode);
			return -EINVAL;
        /* Check if the sent gesture commands was received correctly */
		} else if ((ptail->cmd[0] != buf[1]) || (ptail->cmd[1] != buf[2])) {
			thp_log_err("%s:cmd error: recv %#04x, %#04x !=  %#04x, %#04x\n", __func__,
				ptail->cmd[1], ptail->cmd[0], buf[2], buf[1]);
			return -EINVAL;
		} else {
			return NO_ERR;
		}
	}
}

/* Dual protocols communication interface */
static int touch_dual_protocols_spi_transfer(struct thp_device *tdev,
	void *tbuf, size_t tlen)
{
	int rc;
	struct thp_core_data *cd = NULL;

	if ((!tdev) || (!tdev->thp_core)) {
		thp_log_err("%s: tdev or thp_core is null\n", __func__);
		return -EINVAL;
	}
	rc = touch_driver_write_dual_protocals_cmd(tdev, tbuf, tlen);
	if (rc) {
		thp_log_err("%s: cspi_transfer send failed: rc=%d\n", __func__, rc);
		return rc;
	}
	cd = tdev->thp_core;
	/*delay 50us between tx/rx cmds*/
	if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952)
		udelay(50);
	rc = touch_driver_read_dual_protocals_checkvalue(tdev);
	if (rc) {
		thp_log_err("%s: cspi_transfer recv failed: rc = %d\n", __func__, rc);
		return rc;
	}
	rc = touch_driver_check_dual_protocals(tbuf);
	if (rc)
		thp_log_err("%s: cspi dual protocals spi_transfer failed: rc = %d\n",
			__func__, rc);
    /*delay 100us between cmds*/
	if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952)
		udelay(100);

	return rc;
}

/* hardware reset */
static void touch_hw_reset(struct thp_device *tdev)
{
	thp_log_info("%s: called\n", __func__);
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_HIGH);
	thp_time_delay(tdev->timing_config.boot_reset_hi_delay_ms);
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
	thp_time_delay(tdev->timing_config.boot_reset_low_delay_ms);
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_HIGH);
	thp_time_delay(tdev->timing_config.boot_reset_hi_delay_ms);
}

/* enter drw mode */
static int touch_enter_drw_mode(struct thp_device *tdev)
{
	uint8_t enter_drw_cmd[] = { 0xCC, 0x33, 0x55, 0x5A };
	int cmd_size = sizeof(enter_drw_cmd);
	int retries = TOUCH_ENTER_DRW_RETRIES;
	int rc;

	thp_log_info("%s: called\n", __func__);
	thp_bus_lock(tdev);
	memcpy(tdev->tx_buff, enter_drw_cmd, cmd_size);
	while (retries--) {
		rc = touch_driver_spi_transfer(tdev,
			tdev->tx_buff, tdev->rx_buff, cmd_size, SPEED_HZ_DRW);
		if (!rc)
			break;
		thp_log_err("%s: Enter drw mode failed: rc=%d, retry %d\n",
			__func__, rc, retries);
		thp_time_delay(tdev->thp_core->send_cmd_retry_delay_ms);
	}
	thp_bus_unlock(tdev);
	if (rc)
		thp_log_err("%s: Enter drw mode failed: rc=%d\n", __func__, rc);

	return rc;
}

/* read reg value in drw mode */
static int touch_drw_read_reg(struct thp_device *tdev,
	uint32_t reg_addr, size_t rlen)
{
	struct touch_drw_head *psend = (struct touch_drw_head *)tdev->tx_buff;
	uint8_t *pdata = tdev->rx_buff + sizeof(struct touch_drw_head);
	struct thp_core_data *cd = tdev->thp_core;
	uint16_t crc16_calc;
	uint16_t crc16_recv;
	int rc;
	uint16_t total_len;

	total_len = rlen + sizeof(struct touch_drw_head) + sizeof(struct touch_drw_tail);
	thp_log_info("%s: called\n", __func__);
	if (!rlen || (total_len > TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(cd)))) {
		thp_log_err("%s: Invalid read size %ld\n", __func__, rlen);
		return -EINVAL;
	}

	thp_bus_lock(tdev);
	memset(tdev->tx_buff, 0, total_len);
	psend->rwcmd = TOUCH_DRW_RD_CMD;
	put_unaligned_be24(&psend->addr[0], reg_addr);
	put_unaligned_be24(&psend->len[0], rlen);
	crc16_calc = touch_driver_crc16(&psend->rwcmd,
		offsetof(struct touch_drw_head, crc));
	put_unaligned_be16(&psend->crc[0], (uint16_t)~crc16_calc);

	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff,
		tdev->rx_buff, total_len, SPEED_HZ_DRW);
	if (rc) {
		thp_log_err("%s:Drw read reg %#07x failed: rc=%d\n",
			__func__, reg_addr, rc);
	} else {
		crc16_calc = touch_driver_crc16(pdata, rlen);
		crc16_recv = ~get_unaligned_be16(pdata + rlen);
		if (crc16_calc != crc16_recv) {
			thp_log_err("%s: crc error: calc %#06x != %#06x recv\n",
				__func__, crc16_calc, crc16_recv);
			rc = -EINVAL;
		}
	}
	thp_bus_unlock(tdev);

	return rc;
}

/* write reg value in drw mode */
static int touch_driver_drw_write_reg(struct thp_device *tdev,
	uint32_t reg_addr, uint8_t *wdata, size_t wlen)
{
	struct touch_drw_head *psend = (struct touch_drw_head *)tdev->tx_buff;
	uint8_t *pdata = tdev->tx_buff + sizeof(struct touch_drw_head);
	uint16_t total_len = wlen + sizeof(struct touch_drw_head) + sizeof(struct touch_drw_tail);
	uint16_t crc16_calc;
	int rc;

	thp_log_info("%s", __func__);
	if (!wlen || (total_len > TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(tdev->thp_core)))) {
		thp_log_err("Invalid write size %ld\n", wlen);
		return -EINVAL;
	}

	thp_bus_lock(tdev);
	rc = memset_s(tdev->tx_buff, total_len, 0, total_len);
	if (rc) {
		thp_log_err("%s: memset fail\n", __func__);
		thp_bus_unlock(tdev);
		return rc;
	}
	psend->rwcmd = TOUCH_DRW_WR_CMD;
	put_unaligned_be24(&psend->addr[0], reg_addr);
	put_unaligned_be24(&psend->len[0], wlen);
	crc16_calc = touch_driver_crc16(&psend->rwcmd, offsetof(struct touch_drw_head, crc));
	put_unaligned_be16(&psend->crc[0], (uint16_t)~crc16_calc);
	rc = memcpy_s(pdata, wlen, wdata, wlen);
	if (rc) {
		thp_log_err("memcpy failed, rc = %d\n", rc);
		thp_bus_unlock(tdev);
		return rc;
	}
	crc16_calc = touch_driver_crc16(wdata, wlen);
	put_unaligned_be16(pdata + wlen, (uint16_t)~crc16_calc);
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		total_len, SPEED_HZ_DRW);
	if (rc)
		thp_log_err("Drw write reg %#07x failed: rc=%d\n", reg_addr, rc);
	thp_bus_unlock(tdev);

	return rc;
}

static int touch_driver_get_hwid(struct thp_device *tdev, uint32_t *hwid)
{
	uint32_t ret_hwid;
	int rc;
	struct thp_core_data *cd = tdev->thp_core;

	thp_log_info("%s", __func__);
	rc = touch_enter_drw_mode(tdev);
	if (rc) {
		thp_log_err("Enter drw mode failed: rc=%d\n", rc);
		return rc;
	}
	if (cd->self_control_power)
		rc = touch_drw_read_reg(tdev, TOUCH_REG_HWID_9288, sizeof(uint32_t));
	else
		rc = touch_drw_read_reg(tdev, HW_REG_HWID_9922, sizeof(uint32_t));
	if (rc) {
		thp_log_err("Read hwid failed: rc=%d\n", rc);
		return rc;
	}
	ret_hwid = get_unaligned_le32(tdev->rx_buff + sizeof(struct touch_drw_head));
	*hwid = ret_hwid;
	return NO_ERR;
}

/* free tdev and buffers */
static int touch_driver_dev_free(struct thp_device *tdev)
{
	thp_log_info("%s: called\n", __func__);
	if (tdev) {
		kfree(tdev->rx_buff);
		tdev->rx_buff = NULL;
		kfree(tdev->tx_buff);
		tdev->tx_buff = NULL;
		kfree(tdev);
		tdev = NULL;
	}

	return NO_ERR;
}

/* malloc tdev and buffers */
static struct thp_device *touch_driver_dev_malloc(struct thp_core_data *cd)
{
	struct thp_device *tdev = NULL;
	uint32_t touch_driver_buf_size =
		TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(cd));

	thp_log_info("%s: called\n", __func__);
	tdev = kzalloc(sizeof(*tdev), GFP_KERNEL);
	if (!tdev)
		goto err_touch_driver_dev_free;
	tdev->tx_buff = kzalloc(touch_driver_buf_size, GFP_KERNEL);
	if (!tdev->tx_buff)
		goto err_touch_driver_dev_free;
	tdev->rx_buff = kzalloc(touch_driver_buf_size, GFP_KERNEL);
	if (!tdev->rx_buff)
		goto err_touch_driver_dev_free;

	return tdev;

err_touch_driver_dev_free:
	touch_driver_dev_free(tdev);
	thp_log_err("%s: Malloc thp_device failed\n", __func__);

	return NULL;
}

static void thp_parse_feature_ic_config(struct device_node *thp_node,
	struct thp_core_data *cd)
{
	thp_log_info("%s: called\n", __func__);
	thp_parse_dts_u32(thp_node, "aod_support_on_tddi",
		&cd->aod_support_on_tddi, __func__);

	thp_parse_dts_u32(thp_node, "touch_frame_size",
		&g_touch_frame_size, __func__);
}

/* thp callback, init */
static int touch_driver_init(struct thp_device *tdev)
{
	struct thp_core_data *cd = tdev->thp_core;
	struct device_node *touch_node;
	int rc;

	thp_log_info("%s", __func__);
	touch_node = of_get_child_by_name(cd->thp_node, TOUCH_DEV_NODE_NAME);
	if (!touch_node) {
		thp_log_err("%s node NOT found in dts", TOUCH_DEV_NODE_NAME);
		return -ENODEV;
	}
	if (tdev->sdev->master->flags & SPI_MASTER_HALF_DUPLEX) {
		thp_log_err("Full duplex not supported by master");
		return -EIO;
	}
	rc = thp_parse_spi_config(touch_node, cd);
	if (rc)
		thp_log_err("Spi config parse failed: rc=%d", rc);

	rc = thp_parse_timing_config(touch_node, &tdev->timing_config);
	if (rc)
		thp_log_err("Timing config parse failed: rc=%d", rc);

	rc = thp_parse_feature_config(touch_node, cd);
	if (rc)
		thp_log_err("%s: feature_config fail\n", __func__);

	cd->send_cmd_retry_delay_ms = 0;
	thp_parse_dts_u32(touch_node, "send_cmd_retry_delay_ms",
		&cd->send_cmd_retry_delay_ms, __func__);

    cd->support_fw_xtal_switch = 0;
	thp_parse_dts_u32(touch_node, "support_fw_xtal_switch",
		&cd->support_fw_xtal_switch, __func__);

	cd->need_extra_system_clk = 0;
	thp_parse_dts_u32(touch_node, "need_extra_system_clk",
		&cd->need_extra_system_clk, __func__);
	thp_log_info("%s: need_extra_system_clk %u\n",
		__func__, cd->need_extra_system_clk);

	thp_parse_feature_ic_config(touch_node, cd);
	return NO_ERR;
}

static void touch_driver_detect_for_power(struct thp_device *tdev)
{
	int rc;

	thp_log_info("%s", __func__);
	rc = touch_driver_power_init(tdev->thp_core->panel_id);
	if (rc)
		thp_log_err("%s: power init failed\n", __func__);
	rc = touch_driver_power_on(tdev);
	if (rc)
		thp_log_err("%s: power on failed\n", __func__);
}

static int touch_driver_detect_for_err(struct thp_device *tdev)
{
	int rc;

	thp_log_info("%s", __func__);
	if (tdev->thp_core->self_control_power) {
		rc = touch_driver_power_off(tdev);
		if (rc)
			thp_log_err("%s: power off failed\n", __func__);
		rc = touch_driver_power_release(tdev->thp_core->panel_id);
		if (rc < 0)
			thp_log_err("%s: power ctrl Failed\n", __func__);
	}
	if (tdev->thp_core->fast_booting_solution)
		touch_driver_dev_free(tdev);
	return -ENODEV;
}

static void touch_driver_config_gpio_for_tddi(struct thp_device *tdev)
{
	struct thp_core_data *cd = NULL;
	thp_log_info("%s:called\n", __func__);

	if ((!tdev) || (!tdev->thp_core)) {
		thp_log_err("%s: tdev or thp_core is null\n", __func__);
		return;
	}
	cd = tdev->thp_core;
	if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952) {
		gpio_direction_input(tdev->gpios->irq_gpio);
		gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);
		touch_hw_reset(tdev);
	}
}

static bool touch_driver_hw_id_check(uint32_t *hwid)
{
	int list = 0;
	bool hwid_flag = false;
	uint32_t hwid_tmp;

	for (list = 0; list < ARRAY_SIZE(hw_id_table); list++) {
		hwid_tmp = *hwid;
		hwid_tmp &= hw_id_table[list].mask;
		thp_log_info("%s: *hwid=%x, hwid_tmp=%x, mask=%x\n",
				__func__, *hwid, hwid_tmp, hw_id_table[list].mask);

		if (hwid_tmp == hw_id_table[list].hw_id) {
			thp_log_info("%s: hwid match success, list=%d\n", __func__, list);
			hwid_flag = true;
			break;
		}
	}

	return hwid_flag;
}

/* thp callback, detect */
static int touch_driver_detect(struct thp_device *tdev)
{
	int rc;
	uint32_t hwid = 0;

	thp_log_info("%s: called", __func__);
	if ((!tdev) || (!tdev->thp_core)) {
		thp_log_err("%s: tdev or thp_core is null\n", __func__);
		return -EINVAL;
	}

	if (tdev->thp_core->self_control_power)
		touch_driver_detect_for_power(tdev);
	else
		touch_driver_config_gpio_for_tddi(tdev);

	thp_time_delay(tdev->timing_config.boot_reset_after_delay_ms);
	rc = touch_driver_get_hwid(tdev, &hwid);
	if (rc) {
		thp_log_err("Get hwid failed: rc=%d\n", rc);
		goto err;
	}

	if (!touch_driver_hw_id_check(&hwid)) {
		thp_log_err("Mismatch hwid, got %#06x while %#06x expected\n",
				hwid, hwid);
		goto err;
	}
	thp_log_info("%s: detect success, hwid=%06x\n", __func__, hwid);
	pen_clk_control(CLK_ENABLE, tdev->thp_core->panel_id);

	return NO_ERR;
err:
	return touch_driver_detect_for_err(tdev);
}

static int touch_driver_frame_size_check(struct thp_device *tdev,
	uint16_t curr_len,uint16_t next_len, unsigned int len)
{
	uint16_t touch_max_frame_size;
	struct thp_core_data *cd = tdev->thp_core;

	touch_max_frame_size =
		TOUCH_FRAME_MAX_SIZE(thp_get_max_frame_size(cd));
	if (cd->support_dual_chip_arch)
		touch_max_frame_size =
			TOUCH_DUAL_MAX_FRAME_SIZE(thp_get_max_frame_size(cd));
	if ((curr_len < TOUCH_FRAME_MIN_SIZE) ||
			(curr_len > touch_max_frame_size)) {
		thp_log_err("Invalid current frame size: %u\n", curr_len);
		return -1;
	}
	if ((next_len < TOUCH_FRAME_MIN_SIZE) ||
			(next_len > touch_max_frame_size)) {
		thp_log_err("Invalid next frame size: %u\n", next_len);
		return -1;
	}
	if ((curr_len + TOUCH_TCS_TAIL_SIZE) > len) {
		thp_log_err("Mismatched curr_size %u, read size %u\n",
			curr_len, len);
		return -1;
	}
	return NO_ERR;
}

static int touch_driver_check_frame(struct thp_device *tdev,
	uint8_t *frame, unsigned int frame_size)
{
	struct touch_tcs_head *head;
	struct touch_cspi_tail *tail;
	uint16_t curr_size, next_size;
	uint16_t crc16_calc, crc16_recv;

	head = (struct touch_tcs_head *)frame;
	curr_size = head->curr_size;
	next_size = head->next_size;
	if (touch_driver_frame_size_check(tdev,
		curr_size, next_size, frame_size) != 0) {
		thp_log_err("Some frame size is invalid\n");
		return -1;
	}
	tail = (struct touch_cspi_tail *)(frame + curr_size);
	crc16_calc = touch_driver_crc16(frame,
		curr_size + offsetof(struct touch_cspi_tail, crc));
	crc16_recv = get_unaligned_le16((uint16_t *)tail->crc);
	if (crc16_calc != crc16_recv) {
		thp_log_err("Crc error: calc %#06x != %#06x recv",
			crc16_calc, crc16_recv);
		return -1;
	}
	if ((tail->cmd[0] != 0x05) || (tail->cmd[1] != 0x51)) {
		thp_log_err("Mismatched reply! cmd[2] = %02x,%02x\n",
			tail->cmd[0], tail->cmd[1]);
		return -1;
	}

	return NO_ERR;
}

static int touch_driver_get_frame_data(struct thp_device *tdev, char *buf, unsigned int len)
{
	unsigned int frame_len = 0;
	unsigned int copy_len;
	static unsigned int read_len;
	struct touch_tcs_head *head;
	int rc;
	struct thp_core_data *cd = tdev->thp_core;

	read_len = TOUCH_FRAME_MAX_SIZE(thp_get_max_frame_size(cd)) +
		sizeof(struct touch_cspi_tail);
	memset(tdev->tx_buff, 0, TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(cd)));
	memset(tdev->rx_buff, 0, TOUCH_SPI_BUF_SIZE(thp_get_max_frame_size(cd)));
	if (cd->self_control_power) {
		if (cd->support_dual_chip_arch)
			read_len = TOUCH_DUAL_MAX_FRAME_SIZE(thp_get_max_frame_size(cd)) +
				sizeof(struct touch_cspi_tail);
		memcpy(tdev->tx_buff, get_frame_cmd, sizeof(get_frame_cmd));
	} else {
		read_len = g_touch_frame_size;
		frame_len = g_touch_frame_size;
		copy_len = len < g_touch_frame_size ? len : g_touch_frame_size;
		memcpy(tdev->tx_buff, get_frame_cmd_tddi, sizeof(get_frame_cmd_tddi));
	}
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
			read_len, SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("%s: Check frame spi transfer failed: rc=%d\n", __func__, rc);
		return rc;
	}
	if (cd->self_control_power) {
		rc = touch_driver_check_frame(tdev, tdev->rx_buff, read_len);
		if (rc) {
			thp_log_err("%s: Check frame failed: rc=%d\n", __func__, rc);
			read_len = TOUCH_FRAME_MAX_SIZE(thp_get_max_frame_size(cd)) +
				sizeof(struct touch_cspi_tail);
			return rc;
		}
		head = (struct touch_tcs_head *)(tdev->rx_buff);
		read_len = head->next_size + sizeof(struct touch_cspi_tail);
		frame_len = head->curr_size + sizeof(struct touch_cspi_tail);
		copy_len = frame_len > len ? len : frame_len;
	}
	memcpy(buf, tdev->rx_buff, copy_len);
	if (frame_len < len)
		memset(buf + frame_len, 0, len - frame_len);
	return rc;
}

/* thp callback, get_frame */
static int touch_driver_get_frame(struct thp_device *tdev,
	char *buf, unsigned int len)
{
	int rc;
	bool flag;

	flag = (!tdev) || (!tdev->tx_buff) || (!tdev->rx_buff);
	if (flag) {
		thp_log_err("%s: input dev null\n", __func__);
		return -EINVAL;
	}
	if (!buf) {
		thp_log_err("%s: input buf null\n", __func__);
		return -EINVAL;
	}
	if ((!len) || (len > thp_get_max_frame_size(tdev->thp_core))) {
		thp_log_err("%s: read len: %u illegal\n", __func__, len);
		return -EINVAL;
	}

	thp_bus_lock(tdev);
	rc = touch_driver_get_frame_data(tdev, buf, len);
	if (rc)
		thp_log_err("%s: Get frame failed: rc=%d\n", __func__, rc);

	thp_bus_unlock(tdev);
	return rc;
}

static int touch_driver_write_rst(struct thp_device *tdev)
{
	int rc;
	uint8_t rst_reg[] = { 0x00 };

	rc = touch_driver_drw_write_reg(tdev, TOUCH_RST_FLASH_VOL_REG1,
		rst_reg, sizeof(rst_reg));
	if (rc) {
		thp_log_err("Reset flash voltage reg1(0x%x) failed rc=%d\n",
			TOUCH_RST_FLASH_VOL_REG1, rc);
		return rc;
	}
	rc = touch_driver_drw_write_reg(tdev, TOUCH_RST_FLASH_VOL_REG2,
		rst_reg, sizeof(rst_reg));
	if (rc) {
		thp_log_err("Reset flash voltage reg2(0x%x) failed rc=%d\n",
			TOUCH_RST_FLASH_VOL_REG2, rc);
		return rc;
	}
	thp_time_delay(THP_RESET_WAIT_TIME);
	return rc;
}

static int touch_driver_write_project_id_cmd(struct thp_device *tdev)
{
	int rc;
	uint8_t cmd_sel[] = { 0x01 };
	uint8_t flash_addr[] = { 0x00, 0x14, 0x0F, 0x00 };
	uint8_t sram_addr[] = { 0x00, 0x00, 0x00, 0x00 };
	uint8_t data_length[] = { 0x00, 0x02, 0x00, 0x00 };
	uint8_t start_dexc[] = { 0x01 };

	rc = touch_driver_drw_write_reg(tdev,
		TOUCH_REG_CMD_SEL, cmd_sel, sizeof(cmd_sel));
	if (rc) {
		thp_log_err("Set mass read failed: rc=%d", rc);
		return rc;
	}
	rc = touch_driver_drw_write_reg(tdev,
		TOUCH_REG_FLASH_ADDR, flash_addr, sizeof(flash_addr));
	if (rc) {
		thp_log_err("Set flash addr failed: rc=%d\n", rc);
		return rc;
	}
	rc = touch_driver_drw_write_reg(tdev,
		TOUCH_REG_SRAM_ADDR, sram_addr, sizeof(sram_addr));
	if (rc) {
		thp_log_err("Set sram addr failed: rc=%d\n", rc);
		return rc;
	}
	rc = touch_driver_drw_write_reg(tdev,
		TOUCH_REG_DATA_LENGTH, data_length, sizeof(data_length));
	if (rc) {
		thp_log_err("Set data length failed: rc=%d\n", rc);
		return rc;
	}
	rc = touch_driver_drw_write_reg(tdev,
		TOUCH_REG_START_DEXC, start_dexc, sizeof(start_dexc));
	if (rc) {
		thp_log_err("Start copy flash data failed: rc=%d\n", rc);
		return rc;
	}
	return rc;
}

/* thp callback, get_project id */
static int touch_driver_get_project_id(struct thp_device *tdev,
	char *buf, unsigned int len)
{
	uint32_t proj_id_addr = TOUCH_PROJECT_ID_ADDR;
	int retries = TOUCH_WAIT_BUS_RETRIES;
	uint8_t *proj_id_ptr;
	uint32_t flash_busy;
	int copy_len;
	int rc;

	thp_log_info("[%s]", __func__);
	if (!buf || !len) {
		thp_log_err("Invalid buf or len for project id");
		return -EINVAL;
	}

	touch_driver_write_rst(tdev);
	touch_driver_write_project_id_cmd(tdev);

	while (retries--) {
		rc = touch_drw_read_reg(tdev, TOUCH_REG_BUSY, sizeof(uint32_t));
		if (rc) {
			thp_log_err("Check flash busy failed: rc=%d, retries=%d", rc, retries);
			return rc;
		}
		flash_busy = get_unaligned_le32(tdev->rx_buff + sizeof(struct touch_drw_head));
		if (flash_busy)
			mdelay(TOUCH_FLASH_BUSY_DELAY);
		else
			break;
	}
	if (flash_busy) {
		thp_log_err("Check flash busy failed\n");
		return -EINVAL;
	}
	rc = touch_drw_read_reg(tdev, proj_id_addr, TOUCH_PROJECT_ID_LEN);
	if (rc) {
		thp_log_err("Read sram data failed: rc=%d\n", rc);
		return rc;
	}
	proj_id_ptr = tdev->rx_buff + sizeof(struct touch_drw_head);
	touch_driver_log_buf("PROJID", proj_id_ptr, TOUCH_PROJECT_ID_LEN);

	memset_s(buf, len, 0, len);
	if (len >= TOUCH_PROJECT_ID_LEN)
		copy_len = TOUCH_PROJECT_ID_LEN;
	else
		copy_len = (int)len;

	rc = memcpy_s(buf, copy_len, proj_id_ptr, copy_len);
	if (rc) {
		thp_log_err("copy project id fail, ret = %d\n", rc);
		return rc;
	}

	return NO_ERR;
}

/* thp callback, resume */
static int touch_driver_resume_tddi(struct thp_device *tdev)
{
	gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);
	touch_hw_reset(tdev);
#if defined(CONFIG_HUAWEI_THP_QCOM)
	if (tdev->thp_core->aod_support_on_tddi) {
		tdev->thp_core->tp_ud_lowpower_status = 0;
		g_lowpower_mode = 0; /* clear lowpower status */
	}
#endif
	return NO_ERR;
}

/* thp callback, resume */
static int touch_driver_resume_no_tddi(struct thp_device *tdev)
{
	int ret = NO_ERR;
	bool no_power_off_flag;

	thp_log_info("%s: called\n", __func__);
	pen_clk_control(CLK_ENABLE, tdev->thp_core->panel_id);
	no_power_off_flag = g_thp_udfp_status || chipone_stylus_status ||
		(tdev->thp_core->easy_wakeup_info.sleep_mode == TS_GESTURE_MODE) ||
		g_aod_touch_status || chipone_ring_setting_switch ||
		tdev->thp_core->power_cfg.power_supply_mode;

	if (no_power_off_flag) {
		if (tdev->thp_core->support_shb_thp) {
#ifdef CONFIG_HUAWEI_SHB_THP
			ret = touch_driver_switch_int_ap(tdev);
			thp_log_info("%s: switch to ap\n", __func__);

#endif
		} else {
			thp_log_info("%s:gesture support without shb\n", __func__);
		}
		touch_hw_reset(tdev);
	} else {
		ret = touch_driver_power_on(tdev);
	}
	thp_time_delay(tdev->timing_config.resume_reset_after_delay_ms);

	return ret;
}

/* thp callback, resume */
static int touch_driver_resume(struct thp_device *tdev)
{
	bool flag;
	struct thp_core_data *cd = tdev->thp_core;

	thp_log_info("%s: called\n", __func__);
	flag = !tdev || !tdev->thp_core || !tdev->thp_core->sdev;
	if (flag) {
		thp_log_err("%s: Invalid tdev\n", __func__);
		return -EINVAL;
	}

	if (cd->self_control_power)
		return touch_driver_resume_no_tddi(tdev);
	else
		return touch_driver_resume_tddi(tdev);
}

/* Set gesture mode command,three commands need to be set consecutively */
static int touch_driver_set_gesture_mode(struct thp_device *tdev,
	uint8_t *cmd, size_t cmd_len)
{
	int rc;

	thp_log_info("%s: called\n", __func__);
	/* Data format for notifying the IC of preparing to send a gesture */
	rc = thp_bus_lock(tdev);
	if (rc < 0) {
		thp_log_err("%s:get lock fail\n", __func__);
		return -EINVAL;
	}
	rc = touch_dual_protocols_spi_transfer(tdev, gesture_format_cmd_tddi,
		sizeof(gesture_format_cmd_tddi));
	if (rc) {
		thp_log_err("%s:Send gesture format cmd failed: rc=%d\n", __func__, rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}

	/* Send current gesture mode command */
	rc = touch_dual_protocols_spi_transfer(tdev, cmd, cmd_len);
	if (rc) {
		thp_log_err("%s:Send supported gesture mode cmd failed: rc=%d\n",
			__func__, rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}

	/* Toggle gesture command mode (IC scans in gesture command mode) */
	rc = touch_dual_protocols_spi_transfer(tdev,
		gesture_cmd_tddi, sizeof(gesture_cmd_tddi));
	if (rc) {
		thp_log_err("%s:Enter gesutre mode failed: rc=%d\n", __func__, rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	thp_bus_unlock(tdev);

	return NO_ERR;
}

/* 9922c Set gesture cmd */
static int touch_driver_set_gesture_cmd(struct thp_device *tdev, uint8_t **cmd,
	size_t *cmd_len, uint16_t param)
{
	struct thp_core_data *cd = NULL;

	if ((!tdev) || (!tdev->thp_core)) {
		thp_log_err("%s: tdev or thp_core is null\n", __func__);
		return -EINVAL;
	}

	cd = tdev->thp_core;
	switch (param) {
	case SINGLE_CLICK:
		*cmd = single_click_cmd_tddi;
		*cmd_len = sizeof(single_click_cmd_tddi);
		break;
	case DOUBLE_CLICK:
		if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952) {
			*cmd = double_click_stylus_gesture_enable_cmd;
			*cmd_len = sizeof(double_click_stylus_gesture_enable_cmd);
		} else {
			*cmd = double_click_cmd_tddi;
			*cmd_len = sizeof(double_click_cmd_tddi);
		}
		break;
	case BOTH_CLICK:
		*cmd = both_click_cmd_tddi;
		*cmd_len = sizeof(both_click_cmd_tddi);
		break;
	default:
		thp_log_err("%s:BUG Invalid gesture setting: %#06x\n", __func__, param);
		return -EINVAL;
	}

	return NO_ERR;
}

/* enter gesture mode */
static int touch_driver_enter_gesture_mode(struct thp_device *tdev, uint16_t param)
{
	int rc;
	uint8_t *cmd = NULL;
	size_t cmd_len;
	int retry = 0;

	thp_log_info("%s: called\n", __func__);
	rc = touch_driver_set_gesture_cmd(tdev, &cmd, &cmd_len, param);
	if (rc) {
		thp_log_err("%s:set gesture cmd failed: rc=%d\n",
			__func__, rc);
		return -EIO;
	}

	while (retry < SET_GESTURE_MODE_RETRY_TIMES) {
		rc = touch_driver_set_gesture_mode(tdev, cmd, cmd_len);
		if (rc) {
			thp_log_err("%s:Send supported gesture mode cmd failed: rc=%d, retries=%d\n",
				__func__, rc, retry);
			retry++;
			continue;
		} else {
			thp_log_err("%s:Send supported gesture mode cmd success\n", __func__);
			break;
		}
	}

	return rc;
}

static int touch_driver_config_suspend_timing(struct thp_device *tdev)
{
	struct thp_core_data *cd = NULL;

	if (!tdev) {
		thp_log_err("%s: tdev is null\n", __func__);
		return -EINVAL;
	}

	cd = tdev->thp_core;
	if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952) {
		thp_spi_cs_set(GPIO_LOW);
		thp_do_time_delay(tdev->timing_config.suspend_reset_after_delay_ms);
		gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
	}

	return NO_ERR;
}

static int touch_driver_enter_sleep_mode(struct thp_device *tdev)
{
	int rc;

	thp_log_info("%s: called\n", __func__);
	rc = thp_bus_lock(tdev);
	if (rc < 0) {
		thp_log_err("%s:get lock fail\n", __func__);
		return -EINVAL;
	}
	rc = touch_dual_protocols_spi_transfer(tdev, sleep_cmd_tddi, sizeof(sleep_cmd_tddi));
	if (rc)
		thp_log_err("%s:Set sleep mode failed: rc=%d\n", __func__, rc);

	touch_driver_config_suspend_timing(tdev);
	thp_bus_unlock(tdev);

	return rc;
}

/* set gesture param */
static void touch_driver_set_param(uint16_t *param,
	struct thp_device *tdev)
{
	struct thp_core_data *cd = tdev->thp_core;

	if (g_aod_touch_status)
		*param |= ENABLE_SINGLE_CLICK;
	if (cd->sleep_mode == TS_GESTURE_MODE)
		*param |= ENABLE_DOUBLE_CLICK;
}

#if defined(CONFIG_HUAWEI_THP_QCOM)
static void touch_read_workmode(struct thp_device *tdev)
{
	int rc;

	thp_log_info("%s: called\n", __func__);
	thp_bus_lock(tdev);
	rc = touch_dual_protocols_spi_transfer(tdev,
		get_workmode_cmd_tddi, sizeof(get_workmode_cmd_tddi));
	if (rc)
		thp_log_err("%s: failed: rc=%d\n", __func__, rc);
	thp_bus_unlock(tdev);
}
#endif

/* thp callback, suspend */
static int touch_driver_suspend_tddi(struct thp_device *tdev)
{
	uint16_t param = 0;
	struct thp_core_data *cd = NULL;
	int rc;
	bool flag;

	thp_log_info("%s: called\n", __func__);
	flag = !tdev || !tdev->thp_core || !tdev->thp_core->sdev;
	if (flag) {
		thp_log_err("%s: tdev or thp_core or sdev is null\n", __func__);
		return -EINVAL;
	}
	cd = tdev->thp_core;
	/* set gesture param */
	touch_driver_set_param(&param, tdev);
	if (is_pt_test_mode(tdev) || !param)
		return touch_driver_enter_sleep_mode(tdev);

	rc = touch_driver_enter_gesture_mode(tdev, param);
	if (rc) {
		thp_log_err("%s:Enter gesture mode failed: rc=%d\n", __func__, rc);
		return rc;
	}
#if defined(CONFIG_HUAWEI_THP_QCOM)
	if (cd->aod_support_on_tddi && cd->tp_ud_lowpower_status) {
		touch_read_workmode(tdev);
		rc = touch_driver_enter_sleep_mode(tdev);
		g_lowpower_mode = cd->tp_ud_lowpower_status;
	}
#endif

	if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952) {
		thp_spi_cs_set(GPIO_HIGH);
		thp_do_time_delay(tdev->timing_config.suspend_reset_after_delay_ms);
	}

	return rc;
}

static int touch_driver_suspend_no_tddi(struct thp_device *tdev)
{
	int rc = NO_ERR;
	bool no_power_off_flag;
	struct thp_core_data *cd = tdev->thp_core;

	g_thp_udfp_status = thp_get_status(THP_STATUS_UDFP, cd->panel_id);
	chipone_stylus_status = !!(thp_get_status(THP_STATUS_STYLUS, cd->panel_id)) |
		(cd->suspend_stylus3_status & (cd->support_quick_note ? cd->quick_note : 1));
	g_aod_touch_status = cd->aod_touch_status;
	chipone_ring_setting_switch = cd->ring_setting_switch;
	thp_log_info("%s:gesture_status:%u,finger_status:%d\n",
		__func__, cd->easy_wakeup_info.sleep_mode, g_thp_udfp_status);
	thp_log_info("%s:ring_support:%u,ring_switch:%u,phone_status:%u,ring_setting_switch:%u\n",
		__func__, cd->support_ring_feature, cd->ring_switch,
		cd->phone_status, chipone_ring_setting_switch);
	thp_log_info("%s:stylus_status:%u,aod_touch_status:%u\n", __func__,
		chipone_stylus_status, cd->aod_touch_status);

	no_power_off_flag = g_thp_udfp_status || chipone_ring_setting_switch ||
		(cd->easy_wakeup_info.sleep_mode == TS_GESTURE_MODE) ||
		chipone_stylus_status || cd->aod_touch_status;
	if (no_power_off_flag) {
		if (tdev->thp_core->support_shb_thp) {
#ifdef CONFIG_HUAWEI_SHB_THP
			touch_driver_poweroff_status(tdev);
			rc = touch_driver_switch_int_sh(tdev);
#endif
		} else {
			thp_log_info("%s:gesture support without shb\n", __func__);
		}
	} else {
		if (tdev->thp_core->support_shb_thp)
			/* 0:all function was closed */
			tdev->thp_core->poweroff_function_status = 0;
		rc = touch_driver_power_off(tdev);
	}
	mdelay(tdev->timing_config.before_pen_clk_disable_delay_ms);
	pen_clk_control(CLK_DISABLE, tdev->thp_core->panel_id);

	return rc;
}

/* thp callback, suspend */
static int touch_driver_suspend(struct thp_device *tdev)
{
	bool flag;
	struct thp_core_data *cd = tdev->thp_core;

	thp_log_info("%s: called\n", __func__);
	flag = !tdev || !tdev->thp_core || !tdev->thp_core->sdev;
	if (flag) {
		thp_log_err("%s: Invalid tdev\n", __func__);
		return -EINVAL;
	}

	if (cd->self_control_power)
		return touch_driver_suspend_no_tddi(tdev);
	else
		return touch_driver_suspend_tddi(tdev);
}

/* thp callback, exit */
static void touch_driver_exit(struct thp_device *tdev)
{
	thp_log_info("%s: called\n", __func__);
	touch_driver_dev_free(tdev);
}

#if defined(CONFIG_HUAWEI_THP_QCOM)
/* check gesture,  NOTICE: thp bus is locked */
static int touch_driver_check_gesture(uint8_t *gesture,
	uint32_t gesture_size)
{
	struct touch_cspi_tail *tail =
		(struct touch_cspi_tail *) (gesture + gesture_size);
	uint16_t crc16_calc;
	uint16_t crc16_recv;
	size_t crc_offset = offsetof(struct touch_cspi_tail, crc);

	thp_log_info("%s: called\n", __func__);
	crc16_calc = touch_driver_crc16(gesture, (gesture_size + crc_offset));
	crc16_recv = (gesture[gesture_size + OFFSET_4] << MOVE_8BIT) |
		gesture[gesture_size + OFFSET_3];
	if (crc16_calc != crc16_recv) {
		thp_log_err("%s:Crc error: calc %#06x != %#06x recv\n",
			__func__, crc16_calc, crc16_recv);
		return -EINVAL;
	}
	if ((tail->cmd[OFFSET_0] != get_gesture_cmd_tddi[OFFSET_1]) ||
		(tail->cmd[OFFSET_1] != get_gesture_cmd_tddi[OFFSET_2])) {
		thp_log_err("%s:Mismatched reply! cmd[2] = %02x,%02x\n",
			__func__, tail->cmd[OFFSET_0], tail->cmd[OFFSET_1]);
		return -EINVAL;
	}

	return NO_ERR;
}

/* thp callback, parse_event_info */
static int touch_driver_parse_event_info(struct thp_device *tdev,
	struct thp_udfp_data *udfp_data)
{
	unsigned int read_len = TOUCH_GSTR_SIZE;
	int rc;

	thp_log_info("%s: called\n", __func__);
	rc = thp_bus_lock(tdev);
	if (rc < 0) {
		thp_log_err("%s:get lock fail\n", __func__);
		return -EINVAL;
	}
	memcpy(tdev->tx_buff, get_gesture_cmd_tddi, sizeof(get_gesture_cmd_tddi));
	/* tx_buff and rx_buff is for irq use only */
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		read_len + TOUCH_TCS_TAIL_SIZE, SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("%s:Get gesture failed: rc=%d\n", __func__, rc);
		goto get_gesture_info_failed;
	}
	rc = touch_driver_check_gesture(tdev->rx_buff, read_len);
	if (rc) {
		thp_log_err("%s:Check gesture failed\n", __func__);
		goto get_gesture_info_failed;
	}
	memcpy(udfp_data, tdev->rx_buff, sizeof(struct thp_udfp_data));

get_gesture_info_failed:
	/*
	 * Indicates the ready flag of the clear gesture. This cspi must be issued
	 * to clear the ready flag of the data regardless of whether the SPI
	 * transmission is successful
	 */
	rc = touch_dual_protocols_spi_transfer(tdev, gesture_confirm_cmd_tddi,
		sizeof(gesture_confirm_cmd_tddi));
	if (rc)
		thp_log_err("%s:Clear gesture status failed: rc=%d\n", __func__, rc);
	thp_bus_unlock(tdev);

	return rc;
}

static int touch_driver_set_lowpower(struct thp_device *tdev, u8 state)
{
	uint16_t param = 0;

	thp_log_info("%s: state %u\n", __func__, state);
	/*
	 * gesture_mode and sleep_mode need to be woken
	 * up before switching between each other
	 */
	touch_read_workmode(tdev);
	if (state)
		return touch_driver_enter_sleep_mode(tdev);
	/* set gesture param */
	touch_driver_set_param(&param, tdev);
	if (!param)
		/* invalid gesture param */
		return touch_driver_enter_sleep_mode(tdev);

	return touch_driver_enter_gesture_mode(tdev, param);
}

static int touch_driver_set_lowpower_state(struct thp_device *tdev,
	u8 state)
{
	bool flag;
	struct thp_core_data *cd = NULL;
	int rc;

	thp_log_info("%s: called state = %u\n", __func__, state);
	flag = !tdev || !tdev->thp_core;
	if (flag) {
		thp_log_err("%s: tdev or thp_core is null\n", __func__);
		return -EINVAL;
	}
	cd = tdev->thp_core;
	if (!cd->aod_support_on_tddi) {
		thp_log_info("%s: aod not supported\n", __func__);
		return NO_ERR;
	}
	/* cd->aod_state_flag = 1 means that the phone is in aod state */
	thp_log_info("%s : aod_state_flag value %u\n", __func__, cd->aod_state_flag);
	if (!cd->aod_state_flag && cd->work_status != SUSPEND_DONE) {
		thp_log_info("%s: resumed, not handle lp\n", __func__);
		return NO_ERR;
	}
	if (g_lowpower_mode == state) {
		thp_log_info("%s:don't repeat old state %u\n",
			__func__, state);
		return NO_ERR;
	}
	rc = touch_driver_set_lowpower(tdev, state);
	if (rc)
		thp_log_err("%s: failed: rc=%d", __func__, rc);
	g_lowpower_mode = state;

	return rc;
}
#endif

/* 9951r enter gesture mode */
static int touch_check_enter_gesture(struct thp_device *tdev)
{
	int rc;
	uint16_t crc16_calc;
	uint16_t crc16_recv;
	uint8_t errcode;

	if (!tdev) {
		thp_log_err("%s: tdev is null\n", __func__);
		return -EINVAL;
	}
	rc = memcpy_s(tdev->tx_buff, sizeof(enter_gesture_mode),
		enter_gesture_mode, sizeof(enter_gesture_mode));
	if (rc != EOK) {
		thp_log_err("%s: copy cmd to tx_buf failed: rc = %d\n", __func__, rc);
		return -EINVAL;
	}
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		sizeof(enter_gesture_mode), SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("%s: write failed: rc=%d", __func__, rc);
		return -EIO;
	}
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		OFFSET_3 + sizeof(crc16_recv), SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("%s: read failed: rc=%d", __func__, rc);
		return -EIO;
	}

	errcode = tdev->rx_buff[0];
	if (errcode != TCS_ERR_CODE_OK) {
		thp_log_err("%s: errcode=%d", __func__, errcode);
		return -EINVAL;
	}
	if (tdev->rx_buff[1] != 0x04 && tdev->rx_buff[2] != 0x22) {
		thp_log_err("%s: cmd mismatch %d,%d", __func__,
			tdev->rx_buff[1], tdev->rx_buff[2]);
		return -EINVAL;
	}
	crc16_recv = tdev->rx_buff[3] | (tdev->rx_buff[4] << MOVE_8BIT);
	crc16_calc = touch_driver_crc16(tdev->rx_buff, OFFSET_3);
	if (crc16_calc != crc16_recv) {
		thp_log_err("%s: crc mismatch: calc %04x != %04x recv", __func__,
			crc16_calc, crc16_recv);
		return -EINVAL;
	}

	return rc;
}

/* 9951r gesture check */
static int touch_driver_gesture_check(struct thp_device *tdev)
{
	uint16_t crc16_calc;
	uint16_t crc16_recv;
	uint8_t errcode;
	int rc;

	errcode = tdev->rx_buff[GESTURE_INFO_LEN];
	if (errcode != TCS_ERR_CODE_OK) {
		thp_log_err("%s: errcode=%d", __func__, errcode);
		rc = -EINVAL;
		return rc;
	}
	if (tdev->rx_buff[GESTURE_INFO_LEN + OFFSET_1] != 0x03 &&
		tdev->rx_buff[GESTURE_INFO_LEN + OFFSET_2] != 0x41) {
		thp_log_err("%s: cmd mismatch %d,%d", __func__,
			tdev->rx_buff[GESTURE_INFO_LEN + OFFSET_1],
			tdev->rx_buff[GESTURE_INFO_LEN + OFFSET_2]);
		rc = -EINVAL;
		return rc;
	}
	crc16_recv = tdev->rx_buff[GESTURE_INFO_LEN + OFFSET_3] |
			(tdev->rx_buff[GESTURE_INFO_LEN + OFFSET_4] << MOVE_8BIT);
	crc16_calc = touch_driver_crc16(tdev->rx_buff, GESTURE_INFO_LEN + OFFSET_3);
	if (crc16_calc != crc16_recv) {
		thp_log_err("%s: crc mismatch: calc %04x != %04x recv", __func__,
			crc16_calc, crc16_recv);
		rc = -EINVAL;
		return rc;
	}
	return NO_ERR;
}
/* 9951r gesture report */
static int touch_driver_gesture_report(struct thp_device *tdev,
	unsigned int *gesture_wakeup_value)
{
	int rc = -EIO;

	static uint8_t get_gesture_cmd[] = {
		TOUCH_NORM_RD_CMD, 0x03, 0x41, 0x70, 0x00, 0xe9, 0xb3
	};

	thp_log_info("%s, gesture trigger\n", __func__);
	mutex_lock(&tdev->thp_core->thp_wrong_touch_lock);
	if (tdev->thp_core->easy_wakeup_info.off_motion_on == true) {
		memcpy(tdev->tx_buff, get_gesture_cmd, sizeof(get_gesture_cmd));
		rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
			GESTURE_INFO_LEN + OFFSET_5, SPEED_HZ_DEFAULT);
		if (rc) {
			thp_log_err("%s: write failed: rc=%d", __func__, rc);
			rc = -EIO;
			goto err_unlock;
		}

		rc = touch_driver_gesture_check(tdev);
		if (rc) {
			thp_log_err("%s: gesture check failed: rc=%d", __func__, rc);
			rc = -EIO;
			goto err_unlock;
		}

		if (tdev->rx_buff[0] == GESTURE_D_TAP_VAL) {
			*gesture_wakeup_value = TS_DOUBLE_CLICK;
			thp_log_info("%s, double click report\n", __func__);
			rc = NO_ERR;
			goto err_enter_gesture;
		} else if (tdev->rx_buff[0] == GESTURE_PEN_S_TAP_VAL) {
			*gesture_wakeup_value = TS_STYLUS_WAKEUP_TO_MEMO;
			thp_log_info("%s, stylus click report\n", __func__);
			rc = NO_ERR;
			goto err_enter_gesture;
		} else {
			thp_log_err("%s, unknown gesture id %#04x",
				__func__, tdev->rx_buff[0]);
			rc = -EINVAL;
			goto err_enter_gesture;
		}
	}

err_enter_gesture:
	if (tdev->thp_core->support_vendor_ic_type == IC_TYPE_ICNL9952)
		touch_check_enter_gesture(tdev);
	else
		touch_driver_enter_gesture_mode(tdev, DOUBLE_CLICK);
	tdev->thp_core->easy_wakeup_info.off_motion_on = true;
err_unlock:
	mutex_unlock(&tdev->thp_core->thp_wrong_touch_lock);

	return rc;
}


#ifdef CONFIG_HUAWEI_SHB_THP
#define INPUTHUB_POWER_SWITCH_START_BIT 9
#define INPUTHUB_POWER_SWITCH_START_OFFSET 1
static void touch_driver_poweroff_status(struct thp_device *tdev)
{
	struct thp_core_data *cd = thp_get_core_data(tdev->thp_core->panel_id);
	unsigned int finger_status = !!(thp_get_status(THP_STATUS_UDFP, cd->panel_id));
	unsigned int stylus_status = !!(thp_get_status(THP_STATUS_STYLUS, cd->panel_id)) |
		!!(thp_get_status(THP_STATUS_STYLUS3, cd->panel_id));

	cd->poweroff_function_status =
		(cd->double_click_switch << THP_DOUBLE_CLICK_ON) |
		(finger_status << THP_TPUD_ON) |
		(cd->ring_setting_switch << THP_RING_SUPPORT) |
		(cd->ring_switch << THP_RING_ON) |
		(stylus_status << THP_PEN_MODE_ON) |
		(cd->phone_status << THP_PHONE_STATUS) |
		(cd->single_click_switch << THP_SINGLE_CLICK_ON) |
		(cd->volume_side_status << THP_VOLUME_SIDE_STATUS_LEFT);
	if (cd->aod_display_support)
		cd->poweroff_function_status |=
		(cd->aod_touch_status << THP_AOD_TOUCH_STATUS);
	if ((cd->power_switch >= POWER_KEY_OFF_CTRL) &&
		(cd->power_switch < POWER_MAX_STATUS))
	/*
	 * cd->poweroff_function_status 0~8 bit saved function flag
	 * eg:double_click, finger_status, ring_switch,and so on.
	 * cd->poweroff_function_status 9~16 bit saved screen-on-off reason flag
	 * cd->power_switch is a value(1~8) which stand for screen-on-off reason
	 */
		cd->poweroff_function_status |=
			(1 << (INPUTHUB_POWER_SWITCH_START_BIT +
			cd->power_switch - INPUTHUB_POWER_SWITCH_START_OFFSET));
}

static int touch_driver_switch_int_sh(struct thp_device *tdev)
{
	uint8_t cmd_enable[] = {
		TOUCH_NORM_WR_CMD, 0x22, 0x31, 0x01, 0x00, 0x5B, 0x07,
		0x01, 0x05, 0x80
	};
	uint8_t *cmd;
	size_t cmd_len;
	struct touch_cspi_tail *tail;
	uint16_t crc16_calc, crc16_recv;
	int rc;

	cmd = cmd_enable;
	cmd_len = sizeof(cmd_enable);

	if (tdev->thp_core->use_ap_sh_common_int && tdev->thp_core->support_pinctrl)
		thp_pinctrl_select_lowpower(tdev->thp_core->thp_dev);

	thp_bus_lock(tdev);
	memcpy(tdev->tx_buff, cmd, cmd_len);
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff, cmd_len, SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("%s:send cmd failed: rc=%d\n", __func__, rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	thp_time_delay(TOUCH_SWITCH_INT_DELAY_TIME_MS);
	memset(tdev->tx_buff, 0, TOUCH_TCS_TAIL_SIZE);
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		TOUCH_TCS_TAIL_SIZE, SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("%s:recv cmd failed: rc=%d\n", __func__, rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	tail = (struct touch_cspi_tail *)tdev->rx_buff;
	if (tail->ecode || (tail->cmd[OFFSET_0] != cmd[OFFSET_1]) || (tail->cmd[OFFSET_1] != cmd[OFFSET_2])) {
		thp_log_err("Tcs check failed: ecode=%d, cmd[2]={%02x,%02x}\n",
			tail->ecode, tail->cmd[OFFSET_0], tail->cmd[OFFSET_1]);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	crc16_calc = touch_driver_crc16((uint8_t *)tail, offsetof(struct touch_cspi_tail, crc));
	crc16_recv = tail->crc[OFFSET_0] | (tail->crc[OFFSET_1] << MOVE_8BIT);
	if (crc16_calc != crc16_recv) {
		thp_log_err("crc error: calc %#06x != %#06x recv\n", crc16_calc, crc16_recv);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	thp_bus_unlock(tdev);
	thp_log_info("%s: send: switch int sh\n", __func__);
	return NO_ERR;
}

static int touch_driver_switch_int_ap(struct thp_device *tdev)
{
	uint8_t cmd_disable[] = {
		TOUCH_NORM_WR_CMD, 0x22, 0x31, 0x01, 0x00, 0x5B, 0x07,
		0x00, 0x00, 0x00
	};
	uint8_t *cmd;
	size_t cmd_len;
	struct touch_cspi_tail *tail;
	uint16_t crc16_calc, crc16_recv;
	int rc;

	cmd = cmd_disable;
	cmd_len = sizeof(cmd_disable);

	if (tdev->thp_core->use_ap_sh_common_int && tdev->thp_core->support_pinctrl)
		thp_pinctrl_select_normal(tdev->thp_core->thp_dev);

	thp_bus_lock(tdev);
	memcpy(tdev->tx_buff, cmd, cmd_len);
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff, cmd_len, SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("Send cmd failed: rc=%d\n", rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	thp_time_delay(TOUCH_SWITCH_INT_DELAY_TIME_MS);
	memset(tdev->tx_buff, 0, TOUCH_TCS_TAIL_SIZE);
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff, TOUCH_TCS_TAIL_SIZE, SPEED_HZ_DEFAULT);
	if (rc) {
		thp_log_err("Recv cmd failed: rc=%d\n", rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	tail = (struct touch_cspi_tail *)tdev->rx_buff;
	if (tail->ecode || (tail->cmd[OFFSET_0] != cmd[OFFSET_1]) ||
		(tail->cmd[OFFSET_1] != cmd[OFFSET_2])) {
		thp_log_err("Tcs check failed: ecode=%d, cmd[2]={%02x,%02x}\n",
			tail->ecode, tail->cmd[OFFSET_0], tail->cmd[OFFSET_1]);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	crc16_calc = touch_driver_crc16((uint8_t *)tail, offsetof(struct touch_cspi_tail, crc));
	crc16_recv = tail->crc[OFFSET_0] | (tail->crc[OFFSET_1] << MOVE_8BIT);
	if (crc16_calc != crc16_recv) {
		thp_log_err("crc error: calc %#06x != %#06x recv",
			crc16_calc, crc16_recv);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	thp_bus_unlock(tdev);
	thp_log_info("%s: send: switch int ap\n", __func__);
	return NO_ERR;
}
#endif

static int touch_driver_check_stylus_cmd(struct thp_device *tdev, int cmd_size)
{
	int rc = NO_ERR;
	bool flag;
	bool check_cmd_flag;
	struct thp_core_data *cd = NULL;

	flag = (!tdev) || (!tdev->tx_buff) || (!tdev->rx_buff) || (!tdev->thp_core);
	if (flag) {
		thp_log_err("%s: tdev or tdev->tx_buff is null\n",
			__func__);
		return -EINVAL;
	}
	cd = tdev->thp_core;
	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff,
		tdev->rx_buff, cmd_size, SPEED_HZ_DEFAULT);
	if (rc)
		thp_log_err("%s: write failed: rc=%d", __func__, rc);

	mdelay(TOUCH_ENTER_STYLUS_DELAY);
	if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952) {
		rc = memset_s(tdev->tx_buff, cmd_size, 0, cmd_size);
		if (rc) {
			thp_log_err("%s: memset tx_buff fail\n", __func__);
			return rc;
		}
		rc = memset_s(tdev->rx_buff, TOUCH_ENTER_STYLUS_CHECK_LEN, 0, TOUCH_ENTER_STYLUS_CHECK_LEN);
		if (rc) {
			thp_log_err("%s: memset rx_buff fail\n", __func__);
			return rc;
		}
	}

	rc = touch_driver_spi_transfer(tdev, tdev->tx_buff, tdev->rx_buff,
		TOUCH_ENTER_STYLUS_CHECK_LEN, SPEED_HZ_DEFAULT);
	if (rc)
		thp_log_err("%s: read failed: rc=%d", __func__, rc);

	if (tdev->thp_core->self_control_power)
		check_cmd_flag = (tdev->rx_buff[OFFSET_0] == ENTER_STYLUS_ERR_CHECK) &&
			(tdev->rx_buff[OFFSET_1] == ENTER_STYLUS_NO_TDDI_CMD1_CHECK) &&
			(tdev->rx_buff[OFFSET_2] == ENTER_STYLUS_NO_TDDI_CMD2_CHECK) &&
			(tdev->rx_buff[OFFSET_3] == ENTER_STYLUS_NO_TDDI_CMD3_CHECK) &&
			(tdev->rx_buff[OFFSET_4] == ENTER_STYLUS_NO_TDDI_CMD4_CHECK);
	else
		check_cmd_flag = (tdev->rx_buff[OFFSET_0] == ENTER_STYLUS_ERR_CHECK) &&
			(tdev->rx_buff[OFFSET_1] == ENTER_STYLUS_CMD1_CHECK) &&
			(tdev->rx_buff[OFFSET_2] == ENTER_STYLUS_CMD2_CHECK);
	if (check_cmd_flag) {
		thp_log_info("%s: check stylus cmd success\n", __func__);
		return NO_ERR;
	}

	return -EIO;
}

static int stylus3_status_cmddata_set(struct thp_device *tdev, bool stylus3_uplink_switch,
	int stylus3_adsorption_status, int stylus3_lite_status, int *cmd_size)
{
	const struct stylus_status_command_config *config = NULL;
	const uint8_t *selected_cmd = NULL;

	if ((!tdev) || (!tdev->tx_buff) || (!cmd_size)) {
		thp_log_err("%s: invalid params\n",__func__);
		return -EINVAL;
	}

	if (tdev->thp_core->self_control_power)
		config = &no_tddi_config;
	else
		config = (stylus3_lite_status == STYLUS_A63_CONNECT) ? &a63_config : &default_config;
	if (stylus3_uplink_switch) {
		if (stylus3_adsorption_status == STYLUS_ADSORPTION_STATUS_ON) {
			selected_cmd = config->enable_cmd;
			*cmd_size = config->enable_size;
		} else {
			selected_cmd = config->adsorp_cmd;
			*cmd_size = config->adsorp_size;
		}
	} else {
		selected_cmd = config->disable_cmd;
		*cmd_size = config->disable_size;
	}

	return memcpy_s(tdev->tx_buff, *cmd_size, selected_cmd, *cmd_size);
}

static int touch_driver_send_stylus_cmd(struct thp_device *tdev, bool stylus3_uplink_switch,
	int stylus3_adsorption_status, int stylus3_lite_status)
{
	int i = 0;
	int rc;
	int cmd_size;

	thp_log_info("%s: called\n", __func__);

	thp_bus_lock(tdev);
	rc = stylus3_status_cmddata_set(tdev, stylus3_uplink_switch,
		stylus3_adsorption_status, stylus3_lite_status, &cmd_size);
	if (rc != EOK) {
		thp_log_err("%s: memcpy_s fail:%d\n", __func__, rc);
		thp_bus_unlock(tdev);
		return -EIO;
	}
	for (i = 0; i < TOUCH_SEND_CMD_RETRIES; i++) {
		rc = touch_driver_check_stylus_cmd(tdev, cmd_size);
		if (rc == NO_ERR) {
			thp_bus_unlock(tdev);
			return NO_ERR;
		}
		mdelay(SPI_DELAY_MS);
	}
	thp_log_err("%s: send stylus cmd fail\n", __func__);
	thp_bus_unlock(tdev);

	return -EIO;
}

static int touch_driver_get_power_status(struct thp_device *tdev)
{
	enum ts_sleep_mode gesture_status;
	enum thp_status_type power_status = THP_STATUS_POWER;

	gesture_status = tdev->thp_core->easy_wakeup_info.sleep_mode;
	if (gesture_status != TS_GESTURE_MODE) {
		power_status = thp_get_status(power_status, tdev->thp_core->panel_id);
		if (power_status == THP_STATUS_POWER) {
			thp_log_err("ic power off, not send cmd to fw\n");
			return -EINVAL;
		}
	}
	return NO_ERR;
}

static int touch_driver_bt_handler(struct thp_device *tdev,
	bool delay_enable)
{
	struct thp_core_data *cd = NULL;
	int stylus3_status;
	int stylus3_adsorption_status;
	int stylus3_lite_status;
	int current_stylus3_work_mode;
	bool current_stylus3_status;
	int rc = NO_ERR;

	if ((!tdev) || (!tdev->thp_core)) {
		thp_log_err("%s: tdev or thp_core is null\n", __func__);
		return -EINVAL;
	}

	cd = tdev->thp_core;
	if (!cd->send_bt_status_to_fw)
		return NO_ERR;

	rc = touch_driver_get_power_status(tdev);
	if (rc < 0)
		return NO_ERR;

	if (delay_enable) {
		if (cd->support_vendor_ic_type == IC_TYPE_ICNL9952) {
			thp_log_info("%s: ic do not need delay 200ms when reset\n", __func__);
			return rc;
		} else {
			msleep(SEND_BT_CONNECT_STATUS_AFTER_RESET_DELAY);
		}
	}

	stylus3_status =
		atomic_read(&tdev->thp_core->last_stylus3_status);
	stylus3_adsorption_status =
		atomic_read(&tdev->thp_core->last_stylus_adsorption_status);
	stylus3_lite_status =
		atomic_read(&tdev->thp_core->last_stylus_lite_status);
	current_stylus3_work_mode =
		atomic_read(&tdev->thp_core->last_stylus3_work_mode);
	thp_log_info("%s:get stylus3 status:%u, stylus3_current_work_mode:%u\n",
		__func__, stylus3_status, current_stylus3_work_mode);
	thp_log_info("%s:stylus3_adsorption_status:%u, stylus3_lite_status:%u\n",
		__func__, stylus3_adsorption_status, stylus3_lite_status);
	current_stylus3_status = ((stylus3_status & STYLUS_CONNECT) > 0) &&
		(current_stylus3_work_mode == 0);
	rc = touch_driver_send_stylus_cmd(tdev, current_stylus3_status, stylus3_adsorption_status, stylus3_lite_status);
	if (rc < 0)
		thp_log_err("%s: send stylus status fail\n", __func__);
	else
		thp_log_info("%s: stylus_state:%u, send to fw success\n",
			__func__, stylus3_status);
	return rc;
}

/* thp ops */
static struct thp_device_ops cts_dev_ops = {
	.init = touch_driver_init,
	.detect = touch_driver_detect,
	.get_frame = touch_driver_get_frame,
	.get_project_id = touch_driver_get_project_id,
	.resume = touch_driver_resume,
	.suspend = touch_driver_suspend,
	.exit = touch_driver_exit,
	.chip_gesture_report = touch_driver_gesture_report,
	.bt_handler = touch_driver_bt_handler,
#if defined(CONFIG_HUAWEI_THP_QCOM)
	.get_event_info = touch_driver_parse_event_info,
	.tp_lowpower_ctrl = touch_driver_set_lowpower_state,
#endif
#ifdef CONFIG_HUAWEI_SHB_THP
	.switch_int_sh = touch_driver_switch_int_sh,
	.switch_int_ap = touch_driver_switch_int_ap,
#endif
};

/* module init */
static int thp_module_init(u32 panel_id)
{
	struct thp_device *tdev;
	int rc;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	thp_log_info("%s: THP dirver " TOUCH_DRV_VERSION " for IC", __func__);
	if (!cd) {
		thp_log_err("%s: cd is null\n", __func__);
		return -EINVAL;;
	}
	tdev = touch_driver_dev_malloc(cd);
	if (!tdev) {
		rc = -ENOMEM;
		thp_log_err("%s: Malloc for thp device failed: rc=%d\n",
			__func__, rc);
		return rc;
	}
	tdev->ic_name = TOUCH_IC_NAME;
	tdev->dev_node_name = TOUCH_DEV_NODE_NAME;
	tdev->ops = &cts_dev_ops;
	if (cd->fast_booting_solution) {
		thp_send_detect_cmd(tdev, panel_id, NO_SYNC_TIMEOUT);
		/*
		 * The thp_register_dev will be called later to complete
		 * the real detect action.If it fails, the detect function will
		 * release the resources requested here.
		 */
		return NO_ERR;
	}
	rc = thp_register_dev(tdev, panel_id);
	if (rc) {
		rc = -EFAULT;
		thp_log_err("%s: Register thp device failed: rc=%d\n",
			__func__, rc);
		touch_driver_dev_free(tdev);
		return rc;
	}
	thp_log_info("%s: THP dirver registered\n", __func__);

	return NO_ERR;
}

static int __init touch_driver_module_init(void)
{
	int rc = NO_ERR;
	struct device_node *dn = NULL;
	u32 i;

	thp_log_info("%s: called\n", __func__);
	for (i = 0; i < TOTAL_PANEL_NUM; i++) {
		dn = get_thp_node(i);
		if (dn && of_get_child_by_name(dn, TOUCH_DEV_NODE_NAME))
			rc = thp_module_init(i);
	}
	return rc;
}

/* module exit */
static void __exit touch_driver_module_exit(void)
{
	thp_log_info("%s: called, do nothing", __func__);
}

#ifdef CONFIG_HUAWEI_THP_QCOM
late_initcall(touch_driver_module_init);
#else
module_init(touch_driver_module_init);
#endif
module_exit(touch_driver_module_exit);
MODULE_AUTHOR("huawei");
MODULE_DESCRIPTION("huawei driver");
MODULE_LICENSE("GPL");
