/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: function for rpmb
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "rpmb_driver.h"
#include "agent_rpmb/core/agent_rpmb.h"
#include "tc_ns_log.h"

int rpmb_ioctl_cmd(enum func_id id, enum rpmb_op_type operation,
	struct storage_blk_ioc_rpmb_data *storage_data)
{
    uint64_t time_start, time_end, time_cost;
    int ret;

	if (storage_data == NULL)
		return -1;

    time_start = dfx_getcurtime();
    ret = vendor_rpmb_ioctl_cmd(id, operation, storage_data);
    time_end = dfx_getcurtime();

    time_cost = (time_end - time_start) / NANOS_PER_MILLISECOND;
    if (time_cost > IOCTL_TIMEOUT_MILLISECOND) {
        tlogw("vendor_rpmb_ioctl_cmd cost %llu ms!!\n", time_cost);
    }
    return ret;
}
