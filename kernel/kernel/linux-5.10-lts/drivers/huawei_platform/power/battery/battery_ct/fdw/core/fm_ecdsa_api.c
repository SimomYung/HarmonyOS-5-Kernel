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

#include "fl_cryptolib.h"
#include "fm_ecdsa_api.h"
#include <linux/slab.h>

static void comm_get_u32_be(uint32_t *pt_n, uint8_t *b, uint32_t offset)
{
	*pt_n = (((uint32_t) b[offset]) << 24) |
		(((uint32_t) b[offset + 1]) << 16) |
		(((uint32_t) b[offset + 2]) <<  8) |
		(((uint32_t) b[offset + 3]));
}

int fm_ecdsa_precombpoint(const unsigned char *combtable, size_t tablen,
		const unsigned char *pa, size_t palen)
{
	int ret = 0;
	int i;
	struct str_pae *pae_ctx1 = NULL;
	struct str_ecc_key key;
	uint32_t pa_buf[0x40];

	pae_ctx1 = (struct str_pae *)kzalloc(sizeof(struct str_pae), GFP_KERNEL);
	if (pae_ctx1 == NULL)
		return 1;
	ecc_init_p256k1(&key, pae_ctx1);
	key.q = (struct str_affpoint *)pa_buf;
	for (i = 0; i < 0x10; i++)
		comm_get_u32_be(&pa_buf[i], (unsigned char *)pa, i * 4);
	key.comb_table_q = (uint32_t *)combtable;
	ret = ecc_precombpoint(&key);
	kfree(pae_ctx1);
	return ret;
}

int fm_ecdsa_verify(const unsigned char *combtable, size_t tablen,
		const unsigned char *pa, size_t palen,
		const unsigned char *hash, size_t hlen,
		const unsigned char *sig, size_t slen)
{
	int ret = 0;
	int i;
	uint32_t pa_buf[0x40];
	struct str_pae *pae_ctx1;
	struct str_ecc_key key;

	pae_ctx1 = (struct str_pae *)kzalloc(sizeof(struct str_pae), GFP_KERNEL);
	if (pae_ctx1 == NULL)
		return 1;

	ecc_init_p256k1(&key, pae_ctx1);
	key.q = (struct str_affpoint *)pa_buf;
	for (i = 0; i < 0x10; i++)
		comm_get_u32_be(&pa_buf[i], (unsigned char *)pa, i * 4);
	key.comb_table_q = (uint32_t *)combtable;
	ret = ecdsa_verifyhash(&key, (unsigned char *)hash, (unsigned char *)sig);
	kfree(pae_ctx1);
	return (ret == RESULT_OK ? 0 : 1);
}

int fm_sha256(const unsigned char *msg, size_t msglen,
		unsigned char *outhash)
{
	int ret = 0;
	struct str_sha256_context ctx;

	fl_sha256_init(&ctx);
	fl_sha256_update(&ctx, msg, msglen);
	fl_sha256_final(&ctx, outhash);
	return ret;
}

void crc16_cal(uint8_t *msg, int len, uint16_t *crc)
{
	int i, j;
	uint16_t crc_mul = 0xA001; /* G(x) = x ^ 16 + x ^ 15 + x ^ 2 + 1 */

	*crc = CRC16_INIT_VAL;
	for (i = 0; i < len; i++) {
		*crc ^= *(msg++);
		for (j = 0; j < BIT_P_BYT; j++) {
			if (*crc & ODD_MASK)
				*crc = (*crc >> 1) ^ crc_mul;
			else
				*crc >>= 1;
		}
	}
}

