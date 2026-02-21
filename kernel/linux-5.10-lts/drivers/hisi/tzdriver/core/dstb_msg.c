/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2024. All rights reserved.
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
#include "dstb_msg.h"
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/path.h>
#include <linux/mman.h>
#include <securec.h>
#include "teek_client_constants.h"
#include "teek_ns_client.h"
#include "smc_smp.h"
#include "mailbox_mempool.h"
#include "tc_client_driver.h"
#include "internal_functions.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"

static int tc_ns_send_dstb_global_msg(char *mb_devid, uint32_t devid_size, char *mb_msg, uint32_t size)
{
	struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
	int ret = 0;
	struct mb_cmd_pack *mb_pack = NULL;

	mb_pack = mailbox_alloc_cmd_pack();
	if (!mb_pack) {
		tloge("alloc mb pack failed\n");
		return -ENOMEM;
	}

	mb_pack->operation.paramtypes =
		teec_param_types(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE);
	mb_pack->operation.params[0].memref.buffer = mailbox_virt_to_phys((uintptr_t)mb_msg);
	mb_pack->operation.buffer_h_addr[0] = ((uint64_t)mailbox_virt_to_phys((uintptr_t)mb_msg)) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[0].memref.size = size;
	mb_pack->operation.params[1].memref.buffer = mailbox_virt_to_phys((uintptr_t)mb_devid);
	mb_pack->operation.buffer_h_addr[1] = ((uint64_t)mailbox_virt_to_phys((uintptr_t)mb_devid)) >> ADDR_TRANS_NUM;
	mb_pack->operation.params[1].memref.size = devid_size;

	smc_cmd.cmd_type = CMD_TYPE_GLOBAL;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_DSTB_SRV;
	smc_cmd.operation_phys = mailbox_virt_to_phys((uintptr_t)&mb_pack->operation);
	smc_cmd.operation_h_phys = (uint64_t)mailbox_virt_to_phys((uintptr_t)&mb_pack->operation) >> ADDR_TRANS_NUM;
	ret = tc_ns_smc(&smc_cmd);
	if (ret != 0) {
		tloge("send global msg failed, return error %d\n", ret);
		ret = -EFAULT;
	}
	mailbox_free(mb_pack);

	return ret;
}

int send_dstb_global_msg(const struct dstb_global_msg *msg)
{
	char *mb_msg = NULL;
	char *mb_devid = NULL;
	int ret = 0;
	struct dstb_global_msg global_msg = { 0 };

	if (!msg) {
		tloge("dstb msg is NULL input buffer\n");
		return -EINVAL;
	}
	if (copy_from_user(&global_msg, msg, sizeof(global_msg))) {
		tloge("copy from user failed\n");
		return -EFAULT;
	}
	if (global_msg.buffer == 0 || global_msg.size == 0 || global_msg.size > DSTB_MAX_MSG_SIZE ||
		global_msg.devid_buffer == 0 || global_msg.devid_size == 0 || global_msg.devid_size > DSTB_MAX_MSG_SIZE) {
		tloge("invalid global msg");
		return -EINVAL;
	}

	mb_msg = mailbox_alloc(global_msg.size, 0);
	if (!mb_msg) {
		tloge("alloc mb fail 1\n");
		return -ENOMEM;
	}
	mb_devid = mailbox_alloc(global_msg.devid_size, 0);
	if (!mb_devid) {
		tloge("alloc mb fail 2\n");
		mailbox_free(mb_msg);
		return -ENOMEM;
	}

	if (copy_from_user(mb_msg, global_msg.buffer, global_msg.size) != 0 ||
		copy_from_user(mb_devid, global_msg.devid_buffer, global_msg.devid_size) != 0) {
		tloge("copy global msg from user fail\n");
		(void)mailbox_free(mb_msg);
		(void)mailbox_free(mb_devid);
		return -EFAULT;
	}

	ret = tc_ns_send_dstb_global_msg(mb_devid, global_msg.devid_size, mb_msg, global_msg.size);
	if (ret != 0)
		tloge("tc ns send global msg failed, ret = 0x%x\n", ret);

	(void)mailbox_free(mb_msg);
	(void)mailbox_free(mb_devid);
	return ret;
}
