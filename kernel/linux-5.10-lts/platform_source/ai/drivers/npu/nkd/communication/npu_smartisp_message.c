/*
 * npu_ispnn_message.c
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

#include "npu_smartisp_message.h"

#include <linux/irq.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <securec.h>
#include "npu_common.h"
#include "npu_log.h"
#include "npu_platform.h"
#include "npu_irq_adapter.h"
#include "npu_pm_internal.h"
#include "npu_ipc_msg.h"
#include "npu_ipc.h"

struct semaphore g_smartisp_ack_sem;

// run in irq top half
static irqreturn_t npu_smartisp_ack_irq(int irq, void *data)
{
	unused(irq);
	unused(data);

	npu_plat_handle_irq_tophalf(NPU_IRQ_SMARTISP_ACK);
	up(&g_smartisp_ack_sem);
	npu_drv_debug("hwts_irq_handler end");
	return IRQ_HANDLED;
}

int npu_smartisp_ack_irq_init(struct npu_platform_info *plat_info)
{
	int ret;
	cond_return_error(plat_info == NULL, -EINVAL, "get plat info fail\n");

	sema_init(&g_smartisp_ack_sem, 0);

	// register irq handler
	ret = request_irq((unsigned int)(plat_info->dts_info.irq_smartisp_ack),
		npu_smartisp_ack_irq, IRQF_TRIGGER_NONE, "smartisp-ack", NULL);
	cond_return_error(ret != 0, ret, "request_irq ack irq failed ret %d\n", ret);

	return 0;
}

void npu_smartisp_ack_irq_destroy(void)
{
	struct npu_platform_info *plat_info = NULL;
	plat_info = npu_plat_get_info();
	if (plat_info == NULL) {
		npu_drv_err("npu plat get info\n");
		return;
	}

	free_irq((unsigned int)(plat_info->dts_info.irq_smartisp_ack), NULL);
}

static int wait_smartisp_ipc_ack(void)
{
	int ret;
	u64 jiffy;
	jiffy = msecs_to_jiffies(NPU_SMARTISP_IPC_ACK_TIMEOUT_MS);
	ret = down_timeout(&g_smartisp_ack_sem, (long)jiffy);

	return ret;
}

int npu_smartisp_ipc_msg_process(uint8_t smartisp_flag)
{
    int ret;

	ret = send_ipc_msg_to_ts(IPCDRV_TS_SET_SMARTISP_FLAG, IPCDRV_MSG_ASYNC,
		&smartisp_flag, (u32)sizeof(smartisp_flag), IPCDRV_SEND_WITHOUT_ACK);
	cond_return_error(ret != 0, ret, "npu send smartisp flag %u failed\n", smartisp_flag);

	ret = wait_smartisp_ipc_ack();
	cond_return_error(ret != 0, ret, "wait smartisp flag %u ack failed\n", smartisp_flag);
    return ret;
}