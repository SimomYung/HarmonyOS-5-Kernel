/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: function definition for base hash operation
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef AUTH_BASE_IMPL_H
#define AUTH_BASE_IMPL_H

#ifdef CONFIG_CLIENT_AUTH
#include <linux/version.h>
#include <linux/err.h>
#include <crypto/hash.h>

#define SHA256_DIGEST_LENTH    32

struct sdesc {
	struct shash_desc shash;
	char ctx[];
};

int calc_path_hash(bool is_hidl_srvc, unsigned char *digest, unsigned int dig_len);
int calc_task_hash(unsigned char *digest, uint32_t dig_len,
	struct task_struct *cur_struct, uint32_t pub_key_len);

int tee_init_shash_handle(char *hash_type);
void free_shash_handle(void);
struct crypto_shash *get_shash_handle(void);

void init_crypto_hash_lock(void);
void mutex_crypto_hash_lock(void);
void mutex_crypto_hash_unlock(void);

#else

static inline void free_shash_handle(void)
{
	return;
}

static inline void init_crypto_hash_lock(void)
{
	return;
}

#endif /* CLIENT_AUTH || TEECD_AUTH */

#endif
