/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ufs trap to tzsp/bl31
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __UFS_TRAP_H__
#define __UFS_TRAP_H__

u32 atfd_ufs_smc(u64 _function_id, u64 _arg0, u64 _arg1, u64 _arg2);

#endif /* __UFS_TRAP_H__ */
