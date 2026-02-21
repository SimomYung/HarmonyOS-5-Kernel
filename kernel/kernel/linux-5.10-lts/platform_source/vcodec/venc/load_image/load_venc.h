/*
 * load_venc_ca.h
 *
 * head of load_venc_ca.c
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#ifndef __LOAD_VENC_H__
#define __LOAD_VENC_H__

#include <linux/types.h>

#define LOAD_VENC_OK                              0
#define LOAD_VENC_ERROR                           1 

#define SEC_AUTH_NAME                             "sec_auth"

#define load_venc_min(a, b)                        ((a) < (b) ? (a) : (b))

#define SEC_AUTH_SEND_MAX_SIZE                    0x100000  /* 1M */

int load_verify_venc_image(void *image_buffer, size_t image_size);
int load_venc_image(void *image_buffer, size_t image_size);

#endif /* __LOAD_VENC_H__ */