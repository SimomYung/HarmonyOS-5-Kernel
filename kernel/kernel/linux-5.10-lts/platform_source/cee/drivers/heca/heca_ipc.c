/*
 * heca_ipc.c
 *
 * hisilicon efficinecy control algorithm ipc communication.
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
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
#include "heca_ipc.h"

#include <linux/slab.h>

#include <securec.h>
#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <platform_include/basicplatform/linux/ipc_msg.h>

#include "heca_worker.h"

struct heca_ipc_worker_msg {
	struct kthread_work work;
	u32 data[MAX_MAIL_SIZE];
};

static void heca_ipc_send_data_async_callback(struct kthread_work *work)
{
	int ret;
	struct heca_ipc_worker_msg *msg = container_of(work, struct heca_ipc_worker_msg, work);

	ret = RPROC_ASYNC_SEND(IPC_ACPU_LPM3_MBX_4, (mbox_msg_t *) msg->data, MAX_MAIL_SIZE);
	if (ret)
		pr_err("heca ipc: failed to send async %d %d\n", msg->data[1], ret);

	kfree(msg);
}

static int heca_ipc_send_data_async(u32 id, void *data, u32 len, u32 max_len)
{
	struct heca_ipc_worker_msg *msg = kzalloc(sizeof(struct heca_ipc_worker_msg), GFP_KERNEL);
	if (IS_ERR_OR_NULL(msg)) {
		pr_err("heca ipc: failed to send async %d\n", id);
		return -ENOMEM;
	}

	kthread_init_work(&msg->work, heca_ipc_send_data_async_callback);

	msg->data[0] = IPC_CMD(OBJ_AP, OBJ_HECA, CMD_SETTING, TYPE_DNLIMIT);
	msg->data[1] = id;

	if (data)
		memcpy_s(&msg->data[2], max_len, data, len);

	heca_worker_queue_work(&msg->work);

	return 0;
}

static int heca_ipc_send_data_sync(u32 id, void *data, u32 len, u32 max_len)
{
	mbox_msg_t msg[MAX_MAIL_SIZE] = { 0 };
	mbox_msg_t ack[2] = { 0 };
	int ret;

	msg[0] = IPC_CMD(OBJ_AP, OBJ_HECA, CMD_SETTING, TYPE_DNLIMIT);
	msg[1] = id;

	if (data)
		memcpy_s(&msg[2], max_len, data, len);

	ret = RPROC_SYNC_SEND(IPC_ACPU_LPM3_MBX_4, (mbox_msg_t *)msg, MAX_MAIL_SIZE, ack, ARRAY_SIZE(ack));
	if (ret || ack[0] != msg[0] || ack[1] != msg[1]) {
		pr_err("heca ipc: failed to send sync %d %d %d %d\n", id, ret, ack[0], ack[1]);
		ret = -EIO;
	}

	return ret;
}

int heca_ipc_send_data(u32 id, void *data, u32 len, enum heca_ipc_commu_type type)
{
	int ret;
	int max_len = (MAX_MAIL_SIZE - 2) * sizeof(u32);

	if (id >= HECA_IPC_MSG_MAX || len > max_len || type >= HECA_IPC_MAX) {
		pr_err("heca ipc: invalid ipc param %d %d %d\n", id, len, type);
		return -EINVAL;
	}

	switch (type)
	{
	case HECA_IPC_SYNC:
		ret = heca_ipc_send_data_sync(id, data, len, max_len);
		break;
	case HECA_IPC_ASYNC:
		ret = heca_ipc_send_data_async(id, data, len, max_len);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
