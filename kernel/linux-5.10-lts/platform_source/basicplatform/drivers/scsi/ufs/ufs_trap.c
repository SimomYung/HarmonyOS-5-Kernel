/*
 * ufs_trap.c
 *
 * ufs trap to tzsp/bl31
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

#include <linux/of.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/arm-smccc.h>
#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>

int ufs_ffa_send(u64 arg0, u64 arg1, u64 arg2, u64 arg3)
{
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
	int ret;
	struct ffa_send_direct_data args = {
		.data0 = arg0,
		.data1 = arg1,
		.data2 = arg2,
		.data3 = arg3,
		.data4 = 0,
	};

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		pr_err("%s:ufs:ffa send fail:%d\n", __func__, ret);
		return ret;
	}

	return (int)args.data1;
#else
	return 0;
#endif
}

u32 atfd_ufs_smc(u64 _function_id, u64 _arg0, u64 _arg1, u64 _arg2)
{
#ifdef CONFIG_UFS_FFA_SUPPORT
	return ufs_ffa_send(_function_id, _arg0, _arg1, _arg2);
#else
	struct arm_smccc_res res;

	arm_smccc_smc(_function_id, _arg0, _arg1, _arg2,
		0, 0, 0, 0, &res);
	return (u32)res.a0;
#endif
}

