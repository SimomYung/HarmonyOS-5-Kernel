/*
 * QIC Master Id Info.
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#ifndef __DFX_SEC_QIC_MASTERID_INFO_H
#define __DFX_SEC_QIC_MASTERID_INFO_H

#include "dfx_sec_qic_init.h"
struct masterid_parse {
	char* master_name;
	u32 sub_reason;
};

u32 get_mid_info_sub_reason(const struct dfx_sec_qic_device *qic_dev, u32 mid);

#endif