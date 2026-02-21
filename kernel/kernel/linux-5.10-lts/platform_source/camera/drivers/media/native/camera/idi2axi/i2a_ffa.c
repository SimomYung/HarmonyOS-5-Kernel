/*	1
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: hidi2axi ffa message
 * Create: 2025/4/14
*/

#include <linux/module.h>
#include <linux/arm_ffa.h>
#include <linux/errno.h>
#include "i2a_ffa.h"
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>

#define FUNC_ARGS_NO_USE                (0)
#define FUNC_EXEC_SUCC                  (0)
#define FUNC_EXEC_FAIL                  (-1)
#define IDI2AXI_INSTANCES 2u


int ffa_idi2axi_enable(uint32_t id)
{
	if (id >= IDI2AXI_INSTANCES) {
		pr_err("idi2axi instance:%u is not supported", id);
		return -EINVAL;
	}

    int ret;
	struct ffa_send_direct_data args = {
		.data0 = FID_IDI2AXI_ENABLE,
		.data1 = id,
		.data2 = FUNC_ARGS_NO_USE,
		.data3 = FUNC_ARGS_NO_USE,
		.data4 = FUNC_ARGS_NO_USE,
	};
	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		pr_err("%s: %d, send ffa msg to tzsp fail.\n", __func__, __LINE__);
		return ret;
	}
	if (args.data4 != FUNC_EXEC_SUCC) {
        pr_err("%s :tzsp func fail errcode.0x%llx\n", __func__, args.data4);
        return -1;
    }
	pr_info("ffa_tzsp_idi2axi_enable sucess");
    return ret;
}

int ffa_idi2axi_disable(uint32_t id)
{
	if (id >= IDI2AXI_INSTANCES) {
		pr_err("idi2axi instance:%u is not supported", id);
		return -EINVAL;
	}
	int ret;
	struct ffa_send_direct_data args = {
		.data0 = FID_IDI2AXI_DISABLE,
		.data1 = id,
		.data2 = FUNC_ARGS_NO_USE,
		.data3 = FUNC_ARGS_NO_USE,
		.data4 = FUNC_ARGS_NO_USE,
	};
	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		pr_err("%s :failed to send ecc msg\n", __func__);
        return ret;
	}
	if (args.data4 != FUNC_EXEC_SUCC) {
        pr_err("%s :tzsp func fail errcode.0x%llx\n", __func__, args.data4);
        return -1;
    }
	pr_info("ffa_tzsp_idi2axi_disable sucess");
    return ret;		 
}