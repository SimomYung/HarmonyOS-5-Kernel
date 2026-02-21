/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: Implement of atf interfaces
 *
 * This file is released under the GPLv2.
 */
#include "i2a_atf.h"
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/arm-smccc.h>
#include <platform_include/see/bl31_smc.h>

#define IDI2AXI_INSTANCES 2u
int atf_idi2axi_enable(uint32_t id)
{
	struct arm_smccc_res res;

	if (id >= IDI2AXI_INSTANCES) {
		pr_err("idi2axi instance:%u is not supported", id);
		return -EINVAL;
	}
	arm_smccc_1_1_smc(FID_IDI2AXI_ENABLE, id, &res);
	pr_info("atf_idi2axi_enable ret = %d", __func__, (int32_t)res.a0);
	return (int32_t)res.a0;
}

int atf_idi2axi_disable(uint32_t id)
{
	struct arm_smccc_res res;

	if (id >= IDI2AXI_INSTANCES) {
		pr_err("idi2axi instance:%u is not supported", id);
		return -EINVAL;
	}
	arm_smccc_1_1_smc(FID_IDI2AXI_DISABLE, id, &res);
	return (int32_t)res.a0;
}
