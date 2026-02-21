/* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 2 and only version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*/

#include "hdcp_common.h"

#include <securec.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include "ukmd_utils.h"
#include "dkmd_log.h"
#include "dpu_config_utils.h"
#include "dpu/dpu_base_addr.h"
#include "dpu/soc_dpu_define.h"

#define HDCP_STATE_BIT 25

bool is_hdcp_busy(uint32_t port_id)
{
	uint32_t reg_val;
	uint32_t state;
	char __iomem * sctrl_base;

	if (port_id >= MAX_DP_PORT_NUM) {
		dpu_pr_err("[HDCP] invalid port id = %u", port_id);
        return false;
	}

	/* sctrl 0x438
	* bit[25]:dp port0 hdcp state 1:busy 0:idle
	* bit[26]:dp port1 hdcp state 1:busy 0:idle
	* bit[27]:dp port2 hdcp state 1:busy 0:idle
	* bit[28]:dp port3 hdcp state 1:busy 0:idle
	* bit[29]:dp port4 hdcp state 1:busy 0:idle
	*/

	sctrl_base = dpu_config_get_ip_base(DISP_IP_BASE_SCTRL);
	dpu_check_and_return(!sctrl_base, false, err, "[HDCP] sctrl_base is NULL");

	reg_val = inp32(SOC_SCTRL_SCBAKDATA11_ADDR(sctrl_base));
	state = (reg_val >> (HDCP_STATE_BIT + port_id)) & 0x01;

    dpu_pr_info("[HDCP] dp[%u] hdcp state = %u", port_id, state);
	return state > 0;
}
