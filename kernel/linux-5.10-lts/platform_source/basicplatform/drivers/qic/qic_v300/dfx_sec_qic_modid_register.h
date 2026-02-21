/*
 * QIC v300 modid register functions head file.
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
#ifndef __DFX_SEC_QIC_MODID_REGISTER_H
#define __DFX_SEC_QIC_MODID_REGISTER_H

#define QIC_REGISTER_LIST_MAX_LENGTH 512
#define QIC_MODID_NOT_EXIST 0x0
#define QIC_MODID_EXIST 0x1
#define QIC_MODID_NEGATIVE 0xFFFFFFFF
#define QIC_MODID_NUM_MAX 0x6

struct qic_modid_trans_s {
	struct list_head s_list;
	u32 mid_start;
	u32 mid_end;
	u32 bus;
	u32 modid;
};

u32 dfx_qic_modid_find(u32 qic_busid, u32 qic_mid);
#endif