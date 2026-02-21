/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: base64 decoding
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 06 12:25:51 2022
 */
#ifndef ULIBS_HMBASE64_H
#define ULIBS_HMBASE64_H

#include <stdio.h>
#include <stdint.h>

/* include \0 */
#define base64_encode_out_size(ori_size) ((((ori_size) + 2) / 3) * 4 + 1)
#define base64_decode_out_size(ori_size) (((ori_size) / 4) * 3)

/*
 * encode the src using base64.
 * caller should make sure dst size is enough.
 * on success the function return out_size(the number of encode bytes) and return E_HM_OK,
 * return E_HM_FAULT if encode failed.
 */
int base64_encode(const uint8_t *src, size_t len, char *dst, size_t *out_size);
/*
 * decode the base64 string with space ignored.
 * is safe to src and dst point to same memory.
 * on success the function will set out_size(the number of decoded bytes) and return E_HM_OK,
 * return E_HM_FAULT if decode failed.
 */
int base64_decode(const char *src, size_t len, uint8_t *dst, size_t *out_size);

#endif
