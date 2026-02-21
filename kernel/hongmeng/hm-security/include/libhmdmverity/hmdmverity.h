/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: This function offer the dm-verity table signature verification.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 12 10:01:24 2022
 */

#ifndef ULIBS_LIBHMDMVERITY_HMDMVERITY_H
#define ULIBS_LIBHMDMVERITY_HMDMVERITY_H

int dm_sig_verity(char *pubkey_path, uint8_t *data, uint32_t dataLen, uint8_t *sign, uint32_t signLen);

#endif
