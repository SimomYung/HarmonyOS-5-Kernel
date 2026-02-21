/*
 * QIC V300 err probe functions head file.
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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
#ifndef __DFX_SEC_QIC_ERR_PROBE_H
#define __DFX_SEC_QIC_ERR_PROBE_H
#include "dfx_sec_qic.h"

#define MAX_QIC_DFX_INFO           4
#define OPC_MASK                   0xF
#define OPC_TYPE_MAX               0x6
#define ERR_TYPE_OFFSET            0xC
#define ERR_TYPE_MASK              0xF
#define ERR_TYPE_MAX               0xC
#define TBID_OFFSET                0x8
#define TBID_MASK                  0xFF
#define IBID_OFFSET                0x10
#define IBID_MASK                  0xFF
#define MMIDX_OFFSET               0
#define MMIDX_MASK                 0xFF
#define MASTER_ID_OFFSET           0
#define MASTER_ID_MASK             0xFF
#define SAFE_OFFSET                0
#define SAFE_MASK                  0x1
#define EM_NO_IRQ                  0xABCDABCDUL
#define INVALID_PARAM              0xDCBADCBAUL
#define EM_POWER_OFF               0xDCBAABCDUL
#define EM_POWER_ON                0xABCDDCBAUL
#define EM_IRQ_EXIT                0
#define EM_IRQ_NONE                (-1)
#define ADDR_ERR                   U64_MAX
#define ERRLOG_NUM                 8
#define POWER_STATUS_LEN           1
#define GET_POWER_STATUS           1
#define GET_ERRLOG_INFO            2

struct sec_qic_dfx_info {
	unsigned int opc_type;
	unsigned int err_type;
	unsigned int tbid;
	unsigned int ibid;
	unsigned int mmidx;
	unsigned int tid;
	unsigned int addr_low32;
	unsigned int addr_high32;
	unsigned int master_id;
	unsigned int safe_flag;
};

int dfx_sec_qic_get_errinfo(const struct dfx_sec_qic_device *qic_dev, bool from_irq);
void dfx_sec_qic_reset_handler(const struct dfx_sec_qic_device *qic_dev);
#endif

