/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Declarations of libhmsig APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon June 28 10:51:16 2021
 */
#ifndef ULIBS_LIBHMSIG_HMSIG_CRL_H
#define ULIBS_LIBHMSIG_HMSIG_CRL_H
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <hmkernel/const.h>

#define CRL_RAW_SHM_NUM_1  1U
#define CRL_RAW_SHM_NUM_2  2U
#define CRL_RAW_SHM_NUM_MAX  3U
#define CRL_RAW_SHM_NAME_1  "default_crl_1"
#define CRL_RAW_SHM_NAME_2  "default_crl_2"

struct hmsig_shm {
	uintptr_t base;
	size_t size;
	int shm_fd;
};

struct hmsig_crl_read_ctx {
	void *dst;
	unsigned int dst_sz;
	unsigned int offset;
	unsigned int read_sz;
	unsigned int left_to_end;
	int (*copy)(void *dst, size_t dst_sz, const void *src, size_t src_sz);
};

int hmsig_crl_shm_create(const void *raw_data, size_t raw_sz);

int hmsig_crl_shm_init(void);

int hmsig_crl_verify_sig(const void *data, size_t size, const void *sig, size_t sig_size);

int hmsig_crl_shm_write(const void *raw_data, size_t raw_sz);

int hmsig_crl_shm_read(struct hmsig_crl_read_ctx *ctx);

int hmsig_crl_size(size_t *crl_sz);

#endif /* ifndef ULIBS_LIBHMSIG_HMSIG_CRL_H */
