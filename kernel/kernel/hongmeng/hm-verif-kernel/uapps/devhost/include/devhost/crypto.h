/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost crypto APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_CRYPTO_H__
#define __DEVHOST_API_CRYPTO_H__

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

/* for random */
int devhost_get_random_bytes(void *buf, int nbytes);
int devhost_get_random_u32(uint32_t *value);
int devhost_get_random_u64(uint64_t *value);
#ifdef CONFIG_DEVHOST_ENTROPY_COLLECT
void devhost_add_randomness(const void *data, unsigned int data_len);
#else
static inline void devhost_add_randomness(const void *data, unsigned int data_len)
{
	(void)data;
	(void)data_len;
}
#endif

#endif /* __DEVHOST_API_CRYPTO_H__ */
