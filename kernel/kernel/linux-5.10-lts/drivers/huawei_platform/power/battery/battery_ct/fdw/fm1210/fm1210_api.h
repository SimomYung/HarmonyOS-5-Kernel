/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * fm1210_api.h
 *
 * fm1210_api.c head file
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

#ifndef __FM1210_API_H__
#define __FM1210_API_H__

#include "fm1210.h"
#include "../../batt_aut_checker.h"

/* ow command time delay para */
#define FM1210_POWER_ON_DELAY                        10

/* ow reset time delay para */
#define FM1210_DEFAULT_RESET_START_DELAY             2500
#define FM1210_DEFAULT_RESET_END_DELAY               1000
#define FM1210_DEFAULT_RESET_SAMPLE_DELAY            10           // todo

/* result bytes */
#define FM1210_RESULT_SUCCESS                        0x00

/* USER API COMMAND TIME DELAY */
#define FM1210_CMD_READ_BLOCK_DELAY                  1
#define FM1210_CMD_WRITE_BLOCK_DELAY                 10
#define FM1210_CMD_EE_PUMP_DELAY                     1
#define FM1210_CMD_LOCK_BLOCK_DELAY                  10
#define FM1210_CMD_READ_BIN_DELAY                    10
#define FM1210_CMD_ECC_SIGN_DELAY                    100
#define FM1210_CMD_GAP_DELAY                         1
#define FM1210_CMD_RETRY_DELAY                       500

/* the following values of delay time are not precise and should be adjusted according to the waveform.  */
/* ow write time delay para */
#define FM1210_DEFAULT_WRITE_START_DELAY             2             // todo
#define FM1210_DEFAULT_WRITE_LOW_DELAY               140
#define FM1210_DEFAULT_WRITE_HIGH_DELAY              4
#define FM1210_DEFAULT_WRITE_END_DELAY               10
#define FM1210_OW_WRITE_BYTE_DELAY                   150

/* ow read time delay para */
#define FM1210_DEFAULT_READ_START_DELAY              4
#define FM1210_DEFAULT_READ_SAMPLE_DELAY             10
#define FM1210_DEFAULT_READ_END_DELAY                140
#define FM1210_OW_READ_BYTE_DELAY                    150

/* PAGE SIZE */
#define FM1210_READ_UID_SIZE                         8
#define FM1210_DEV_CERT_SIZE                         144
#define FM1210_DEV_CERT_OFS                          (FM1210_DEV_CERT_SIZE << 1)
#define FM1210_ECC_SIG_SIZE                          64
#define FM1210_ECC_SIG_OFS                           (FM1210_ECC_SIG_SIZE << 1)
#define FM1210_RES_DATA_OFS                          1

#define FM1210_CMD_BLOCKNUM                          1
#define FM1210_RES_STATE_LEN                         1
#define FM1210_CRC_LEN                               2
#define FM1210_CMD_READ_LEN                          2
#define FM1210_CMD_WRITE_LEN                         18
#define FM1210_CMD_WRITE_CODE_LEN                    2
#define FM1210_CMD_EE_LEN                            2
#define FM1210_CMD_LOCK_STA_LEN                      2
#define FM1210_CMD_LOCK_LEN                          2
#define FM1210_CMD_CERT_LEN                          1
#define FM1210_CMD_RAND_LEN                          1
#define FM1210_CMD_ECC_LEN                           34
#define FM1210_ECC_MSG_LEN                           32
#define FM1210_ECC_MSG_OFFSET                        2


/* CRC ALG */
#define FM1210_CRC_IN_VAL                            0xc6c6
#define FM1210_CRC_CHAR_OFFSET                       8
#define FM1210_CRC_POLY_VAL                          0x1021
#define FM1210_CRC_EDGE_VAL                          0x8000
#define FM1210_CRC_GETLOW_VAL                        0xFF

/* COMM PROTOCOL */
#define FM1210_MAX_RETRY_COUNT                       6
#define FM1210_READ                                  0
#define FM1210_WRITE                                 1
#define FM1210_LOCK                                  2
#define FM1210_LOCK_STATUS                           3

#define FM1210_UID_BLOCKNUM                          0
#define FM1210_UID_BLOCKLEN                          1
#define FM1210_ACT_BLOCKNUM                          0x01
#define FM1210_ACT_BLOCKLEN                          4
#define FM1210_BATTTYPE_BLOCKNUM                     0x05
#define FM1210_BATTTYPE_BLOCKLEN                     1
#define FM1210_SN_BLOCKNUM                           0x06
#define FM1210_SN_BLOCKLEN                           2
#define FM1210_GROUPSN_BLOCKNUM                      0x08
#define FM1210_GROUPSN_BLOCKLEN                      1
#define FM1210_LOCK_BLOCKNUM                         0x0C
#define FM1210_LOCK_BLOCKLEN                         1
#define FM1210_BATTSTORAGE_BLOCKNUM                  0x0D
#define FM1210_BATTSTORAGE_BLOCKLEN                  1

#define FM1210_GET_CERT                              0x40
#define FM1210_GET_RAND                              0x44
#define FM1210_GET_ECC                               0x41
#define FM1210_GET_ECC_TYPE                          0x80
#define FM1210_READ_CMD                              0x30
#define FM1210_WRITE_CMD                             0x31
#define FM1210_ACTIVATE_EE_CMD                       0x15
#define FM1210_EE_PUMP_ACTIVE                        0x5A
#define FM1210_EE_PUMP_CLOSE                         0
#define FM1210_LOCK_CMD                              0x39
#define FM1210_LOCKSTA_CMD                           0x32

#define FM1210_TRANSCEIVE_LEN                        258
#define FM1210_PAGE_BLOCK_SIZE                       16
#define FM1210_READ_RECV_LEN                         16
#define FM1210_READ_BLOCK_RECV_LEN                   128
#define FM1210_READ_FILE_RECV_LEN                    256
#define FM1210_WRITE_RECV_LEN                        1
#define FM1210_EE_RECV_LEN                           1
#define FM1210_LOCK_RECV_LEN                         1
#define FM1210_LOCKSTA_RECV_LEN                      2
#define FM1210_ROMID_RECV_LEN                        10
#define FM1210_CERT_RECV_LEN                         146
#define FM1210_CERT_HEAD                             16
#define FM1210_TRND_LEN                              8
#define FM1210_COMB_TABLE_QBUF_LEN                   1920
#define FM1210_DEV_INFO_LEN                          300
#define FM1210_CERT_LEN                              80
#define FM1210_ECC_RECV_LEN                          128

#define FM1210_READ_LEN_EDGE                         0xFD
#define FM1210_COMM_ERR                              0xA1
#define FM1210_READ_LEN_ERR                          0xA3
#define FM1210_CRC_ERR                               0xA2

#define FM1210_ADDR_OFFSET_BIT4                      16
#define FM1210_ASCII_NUMBER_START                    48
#define FM1210_ASCII_NUMBER_END                      57
#define FM1210_OFFSET_LETTER                         7
#define FM1210_ASCII_LOWERCASE_START                 97
#define FM1210_ASCII_LOWERCASE_END                   122
#define FM1210_ASCII_TO_UPPERCASE                    32
#define FM1210_STR_TO_HEX_OFFSET                     0x30
#define FM1210_DEC_TO_HEX_TIMES                      16
#define FM1210_HEX_NUMBERS_NUM                       9
#define FM1210_HEX_LETTERS_NUM                       7
#define FM1210_PRINT_DATA_LEN                        5

enum transceive_cmd_index {
	CMD_LEN_INDEX = 0,
	CMD_INDEX,
	CMD_SEND_APDU_INDEX,
};

enum ecc_sign_mode {
	ECC_NORMAL = 0,
	ECC_EX,
	ECC_INVALID
};


int fm1210_read_romid(struct fm1210_dev *di, char *buf, int times,
	int delay_ms);
int fm1210_ic_ck(struct fm1210_dev *di);
int fm1210_operate_file(struct fm1210_dev *di, int blockNum, int operation,
	unsigned char *buf, int buf_len);
int fm1210_do_authentication(struct fm1210_dev *di, struct power_genl_attr *key_res,
	enum key_cr *result, enum ecc_sign_mode mode);
void fm1210_hex_to_str(char *pdest, const char *psrc, int ilen);
void fm1210_str_to_hex(char *pdest, const char *psrc, int ilen);
void fm1210_gpio_dir_input(int onewire_gpio);
void fm1210_gpio_dir_output(int onewire_gpio, uint8_t value);
void fm1210_gpio_set_value(int onewire_gpio, uint8_t value);
int fm1210_gpio_get_value(int onewire_gpio);
char* fm1210_print_data(u8* buf, size_t buf_length);
#endif /* __FM1210_API_H__ */
