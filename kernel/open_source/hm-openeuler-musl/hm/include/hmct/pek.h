/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Header file of pek
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 27 17:07:11 2022
 */

#ifndef HM_INCLUDE_HMCT_PEK_H
#define HM_INCLUDE_HMCT_PEK_H

#include <stdint.h>

uint64_t hm_pek_open(const char *module_name);
int hm_pek_close(uint64_t module_rref);
int hm_pek_ioctl(uint64_t module_rref, uint32_t cmd,
		 void *cfg, uint32_t cfg_len,
		 void *rlt, uint32_t rlt_len);

#endif
