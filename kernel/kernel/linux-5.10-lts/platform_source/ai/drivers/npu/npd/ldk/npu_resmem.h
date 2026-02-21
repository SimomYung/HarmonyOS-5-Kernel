/*
 * npu_resmem.h
 *
 * about npu resmem
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
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
#ifndef __NPU_RESMEM_H__
#define __NPU_RESMEM_H__

#include <linux/types.h>
#include "npu_platform.h"
#include "npu_ddr_map.h"

int npu_plat_set_resmem(struct npu_platform_info *plat_info);

#ifdef CONFIG_NPU_ISPNN_INFO
struct hiail_svm_info {
	uint16_t ssid;
	uint64_t ttbr;
	uint64_t tcr;
};
 
struct hiail_para_info {
    struct hiail_svm_info svm_info;
	uint64_t frame_buffer_iova_addr;
	uint64_t frame_buffer_size;
	uint64_t fake_frame_buffer_addr;
};
#endif // for CONFIG_NPU_ISPNN_INFO

#endif // for __NPU_RESMEM_H__
