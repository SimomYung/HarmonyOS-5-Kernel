/*
 * npu_ipc.c
 *
 * about npu ipc
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
#include "npu_ipc.h"
#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <securec.h>
#include "npu_log.h"

#define NPU_IPC_RETRY_MAX_TIMES     4
#define IPCDRV_RPROC_MSG_LENGTH		8
#define IPCDRV_MSG_MAX_LENGTH		128
#define IPCDRV_MSG_LENGTH			28

struct ipcdrv_msg_header {
	u32 msg_type : 1;
	u32 cmd_type : 7;
	u32 sync_type : 1;
	u32 reserved : 1;
	u32 msg_length : 14;
	u32 msg_index : 8;
};

struct ipcdrv_message {
	struct ipcdrv_msg_header ipc_msg_header;
	u8 ipcdrv_payload[IPCDRV_MSG_LENGTH];
};

enum ipc_msg_type {
	MSGTYPE_DRIVER_SEND = 0,
	MSGTYPE_DRIVER_RECEIVE = 1
};

static int send_ipc_msg_to_ts_core(rproc_id_t rproc_id, rproc_msg_t *msg,
	rproc_msg_len_t len, enum ipc_send_ack_type ack_type)
{
	int ret;

	if (ack_type == IPCDRV_SEND_WITH_ACK) {
		rproc_msg_t ack_buff = 0;
		rproc_msg_len_t ack_buffer_len = 1;
		ret = ipc_rproc_xfer_sync(rproc_id, msg, len, &ack_buff, ack_buffer_len);
	} else {
		/* send IPC message synchronously without ACK interrupts, avoiding access to the power-off area */
		ret = ipc_rproc_xfer_sync_no_ack_npu(rproc_id, msg, len);
	}

	return ret;
}

static int send_ipc_msg_to_ts_common(rproc_id_t rproc_id, rproc_msg_t *msg,
	rproc_msg_len_t len, enum ipc_send_ack_type ack_type)
{
	int ret;
	int retry_time;

	for (retry_time = 0; retry_time < NPU_IPC_RETRY_MAX_TIMES; retry_time++) {
		ret = send_ipc_msg_to_ts_core(rproc_id, msg, len, ack_type);
		if (ret == 0)
			break;
		npu_drv_err("send ipc %d times fail: %d", retry_time, ret);
	}

	return ret;
}

static int send_ipc_msg_to_ts_without_payload(
    u32 cmd_type, enum ipc_msg_sync_type msg_sync_type, enum ipc_send_ack_type ack_type)
{
	rproc_msg_t msg[IPCDRV_RPROC_MSG_LENGTH] = {0};
	struct ipcdrv_message *ipc_msg = NULL;
	int ret;

	ipc_msg = (struct ipcdrv_message *)msg;
	ipc_msg->ipc_msg_header.msg_type = MSGTYPE_DRIVER_SEND;
	ipc_msg->ipc_msg_header.cmd_type = cmd_type;
	ipc_msg->ipc_msg_header.sync_type = msg_sync_type;
	ipc_msg->ipc_msg_header.reserved = 0;
	ipc_msg->ipc_msg_header.msg_length = 0;
	ipc_msg->ipc_msg_header.msg_index = 0;
	npu_drv_info("cmd_type = %u msg_sync_type = %d\n", cmd_type, msg_sync_type);
	ret = send_ipc_msg_to_ts_common(IPC_NPU_ACPU_NPU_MBX3, msg,
		IPCDRV_RPROC_MSG_LENGTH, ack_type);
	if (ret != 0) {
		npu_drv_err("send ipc msg to ts common failed\n");
		return -1;
	}
	return 0;
}

int send_ipc_msg_to_ts(u32 cmd_type, enum ipc_msg_sync_type msg_sync_type,
	const u8 *send_msg, u32 send_len, enum ipc_send_ack_type ack_type)
{
	rproc_msg_t msg[IPCDRV_RPROC_MSG_LENGTH] = {0};
	struct ipcdrv_message *ipc_msg = NULL;
	int ret;
	u32 index;
	u32 msg_cnt;
	u32 left_msg_size;
	u32 msg_length = 0;

	cond_return_error(send_len > IPCDRV_MSG_MAX_LENGTH, -1,
		"send_len = %u is error\n", send_len);

	if (send_msg == NULL || send_len == 0)
		return send_ipc_msg_to_ts_without_payload(cmd_type, msg_sync_type, ack_type);

	ipc_msg = (struct ipcdrv_message *)msg;
	ipc_msg->ipc_msg_header.msg_type = MSGTYPE_DRIVER_SEND;
	ipc_msg->ipc_msg_header.cmd_type = cmd_type;
	ipc_msg->ipc_msg_header.sync_type = msg_sync_type;
	ipc_msg->ipc_msg_header.reserved = 0;
	ipc_msg->ipc_msg_header.msg_length = send_len;

	msg_cnt = ((send_len - 1) / (u32)IPCDRV_MSG_LENGTH) + 1;
	left_msg_size = send_len;
	npu_drv_info("send_len = %u msg_cnt = %u\n", send_len, msg_cnt);
	for (index = 0; index < msg_cnt; index++) {
		msg_length = (left_msg_size > IPCDRV_MSG_LENGTH) ?
			(u32)IPCDRV_MSG_LENGTH : left_msg_size;
		ipc_msg->ipc_msg_header.msg_index = index;
		ret = memcpy_s(ipc_msg->ipcdrv_payload, msg_length,
				send_msg + index * (u32)IPCDRV_MSG_LENGTH, msg_length);
		cond_return_error(ret != 0, -1,"memcpy failed, index:%u, msg_length:0x%x\n",
			index, msg_length);

		npu_drv_debug(
			"msg_cnt:%u, left_msg_size:0x%x index = %u, msg_index = %u\n",
			msg_cnt, left_msg_size, index, ipc_msg->ipc_msg_header.msg_index);
		ret = send_ipc_msg_to_ts_common(IPC_NPU_ACPU_NPU_MBX3, msg,
			IPCDRV_RPROC_MSG_LENGTH, ack_type);
		if (ret != 0) {
			npu_drv_err("send ipc msg to ts common failed\n");
			return -1;
		}

		if (left_msg_size >= IPCDRV_MSG_LENGTH)
			left_msg_size -= (u32)IPCDRV_MSG_LENGTH;
	}

	return 0;
}
EXPORT_SYMBOL(send_ipc_msg_to_ts);