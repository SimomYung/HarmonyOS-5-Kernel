/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Declarations of verification methods of cms
 * Author: Huawei OS Kernel Lab
 * Create: Mon June 28 10:51:16 2021
 */
#ifndef LIBHMSIG_CMS_SIG_VERIFY_H
#define LIBHMSIG_CMS_SIG_VERIFY_H
#include <stdint.h>
#include <libhmsig/hmsig_crl.h>

int cms_verify_sig(struct hmsig_shm *shm, const void *data, size_t size, const void *sig, size_t sig_size);

int cms_check_crl(const void *old_crl, size_t old_sz, const void *new_crl, size_t new_sz);

int cms_check_crl_format(const void *crl, size_t sz);

#endif /* ifndef LIBHMSIG_CMS_SIG_VERIFY_H */
