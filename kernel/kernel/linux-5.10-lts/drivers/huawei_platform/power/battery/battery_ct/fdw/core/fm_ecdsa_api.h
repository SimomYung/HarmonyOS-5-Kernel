/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * fm_ecdsa_api.c
 *
 * interface for fm_api.c
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

#ifndef FM_ECSDA_API_H
#define FM_ECSDA_API_H

#define CRC16_INIT_VAL        0xffff
#define BIT_P_BYT             8
#define ODD_MASK              0x0001

int fm_ecdsa_precombpoint(const unsigned char *combtable, size_t tablen,
	const unsigned char *pa, size_t palen);

int fm_ecdsa_verify(const unsigned char *combtable, size_t tablen,
	const unsigned char *pa, size_t palen, const unsigned char *hash,
	size_t hlen, const unsigned char *sig, size_t slen);

int fm_sha256(const unsigned char *msg, size_t msglen,
	unsigned char *outhash);

void crc16_cal(uint8_t *msg, int len, uint16_t *crc);

#endif /* END FM_ECSDA_API_H */
