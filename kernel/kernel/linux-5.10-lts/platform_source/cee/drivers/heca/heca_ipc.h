/*
 * heca_ipc.h
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
#ifndef _HECA_IPC_H_
#define _HECA_IPC_H_

#include <linux/types.h>

enum heca_ipc_commu_type {
	HECA_IPC_SYNC,
	HECA_IPC_ASYNC,
	HECA_IPC_MAX
};

enum heca_ipc_id {
	HECA_IPC_TYPE = 0,
	HECA_START,
	HECA_STOP,
	HECA_LIT_UPDATE_CFG,
	HECA_GAME_START,
	HECA_GAME_STOP,
	HECA_GAME_MID_FPS_CFG,
	HECA_GAME_BIG_FPS_CFG,
	HECA_GAME_UPDATE_CFG,
	HECA_GAME_GPU_FPS_CFG,
	HECA_GAME_GPU_UPDATE_CFG,
	HECA_FREQ_SET_MIN_MAX,
	HECA_FREQ_SET_BOOSTED,
	HECA_IPC_MSG_MAX,
};

int heca_ipc_send_data(u32 id, void *data, u32 len, enum heca_ipc_commu_type type);

#endif