/*
 * npu_smartisp_message.h
 *
 * about npu mailbox
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#ifndef __NPU_ISPNN_MESSAGE_H
#define __NPU_ISPNN_MESSAGE_H

#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <npu_platform.h>

#include "npu_common.h"

#define NPU_SMARTISP_IPC_ACK_TIMEOUT_MS 1000

enum npu_smartisp_flag {
	NPU_SMARTISP_FLAG_DISABLE = 0,
	NPU_SMARTISP_FLAG_ENABLE
};

int npu_smartisp_ack_irq_init(struct npu_platform_info *plat_info);
void npu_smartisp_ack_irq_destroy(void);
int npu_smartisp_ipc_msg_process(uint8_t smartisp_flag);

#endif