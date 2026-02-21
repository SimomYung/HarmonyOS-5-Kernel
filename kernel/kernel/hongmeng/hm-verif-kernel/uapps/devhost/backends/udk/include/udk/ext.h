/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: This is the header of udk ext interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 12 10:12:53 2022
 */
#ifndef __UDK_EXT_H__
#define __UDK_EXT_H__

#include <udk/types.h>
#include <udk/errno.h>
#include <libstrict/strict.h>

int udk_ext_ctl(uint32_t cmd, void *cfg, uint32_t cfg_len,
		void *rlt, uint32_t rlt_len);

#endif /* __UDK_EXT_H__ */
