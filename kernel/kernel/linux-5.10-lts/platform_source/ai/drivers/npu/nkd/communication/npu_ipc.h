/*
 * npu_ipc.h
 *
 * about npu ipc
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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
#ifndef __DRV_IPC_H__
#define __DRV_IPC_H__

#include <linux/errno.h>
#include <linux/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

enum ipc_msg_sync_type {
	IPCDRV_MSG_SYNC = 0,
	IPCDRV_MSG_ASYNC = 1
};

enum ipc_send_ack_type {
	IPCDRV_SEND_WITH_ACK = 0,
	IPCDRV_SEND_WITHOUT_ACK = 1
};

int send_ipc_msg_to_ts(u32 cmd_type, enum ipc_msg_sync_type msg_sync_type,
	const u8 *send_msg, u32 send_len, enum ipc_send_ack_type ack_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __DRV_IPC_H__ */
