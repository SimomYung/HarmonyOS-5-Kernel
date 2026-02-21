/*
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/types.h>
#include <linux/arm-smccc.h>
#include "platform_include/see/bl31_smc.h"

static noinline u64 lb_sec_ops(u64 fid, u32 x1, u32 x2, u32 x3)
{
	struct arm_smccc_res res;

	arm_smccc_smc(fid, x1, x2, x3, 0, 0, 0, 0, &res);
	return (int)res.a0;
}

u32 get_lb_efuse(void)
{
	return lb_sec_ops(LB_FID_GET_EFUSE, 0, 0, 0);
}

u32 lb_set_sec_pgid(u32 pgid, u32 mst_cfg, u32 drv_cfg)
{
	return lb_sec_ops(LB_FID_SET_PGID, pgid, mst_cfg, drv_cfg);
}

u32 lb_set_sec_cgid(u32 cgid, u32 way_allc, u32 cgid_cfg)
{
	return lb_sec_ops(LB_FID_SET_CGID, cgid, way_allc, cgid_cfg);
}

u32 lb_exclusive_enable(void)
{
	return lb_sec_ops(LB_FID_EXCLUSIVE_ENABLE, 0, 0, 0);
}

u32 lb_exclusive_bypass(void)
{
	return lb_sec_ops(LB_FID_EXCLUSIVE_BYPASS, 0, 0, 0);
}

u32 lb_pgid_request(u32 pgid)
{
	return lb_sec_ops(LB_FID_PGID_REQUEST, pgid, 0, 0);
}

u32 lb_pgid_release(u32 pgid)
{
	return lb_sec_ops(LB_FID_PGID_RELEASE, pgid, 0, 0);
}

u32 lb_mpam_way_enable(u32 mpam_id, u32 way)
{
	return lb_sec_ops(LB_FID_MPAM_WAY_ENABLE, mpam_id, way, 0);
}

#ifdef CONFIG_MM_LB_SEC_REG_TEST
u32 lb_powerup(void)
{
	return lb_sec_ops(LB_FID_POWERUP, 0, 0, 0);
}

u32 lb_powerdown(void)
{
	return lb_sec_ops(LB_FID_POWERDOWN, 0, 0, 0);
}

u32 lb_part_powerup(void)
{
	return lb_sec_ops(LB_FID_PARTPOWERUP, 0, 0, 0);
}

u32 lb_part_powerdown(void)
{
	return lb_sec_ops(LB_FID_PARTPOWERDOWN, 0, 0, 0);
}

u32 lb_lp_powerup(void)
{
	return lb_sec_ops(LB_FID_LPPOWERUP, 0, 0, 0);
}

u32 lb_lp_powerdown(void)
{
	return lb_sec_ops(LB_FID_LPPOWERDOWN, 0, 0, 0);
}

u32 sec_read(u32 addr)
{
	return lb_sec_ops(LB_FID_READ_TEST, addr, 0, 0);
}

u32 sec_write(u32 value, u32 addr)
{
	return lb_sec_ops(LB_FID_WRITE_TEST, value, addr, 0);
}
#endif
