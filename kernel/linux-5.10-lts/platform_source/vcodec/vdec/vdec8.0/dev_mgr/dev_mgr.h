/*
 * dev_mgr.h
 *
 * Headers define of vcodec sub device manager module
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#ifndef VCODEC_DEV_MGR_H
#define VCODEC_DEV_MGR_H
#include <linux/device.h>
#include <linux/interrupt.h>
#include "vcodec_types.h"
#ifdef VFMW_PROC_SUPPORT
#include "vcodec_proc.h"
#endif

enum vcodec_dev_type {
	VCODEC_DEV_SMMU,
	VCODEC_DEV_REGULATOR,
	VCODEC_DEV_SCD,
	VCODEC_DEV_MCU,
	VCODEC_DEV_BSP,
	VCODEC_DEV_PXP,
	VCODEC_DEV_MAX,
};

enum vcodec_dev_event_notify {
	VCODEC_DEV_MCU_EVENT_ALLOC_ION_DONE,
	VCODEC_DEV_MCU_EVENT_TRANSFER_LOG,
	VCODEC_DEV_MCU_RESET_PXP,
	VCODEC_DEV_MCU_RESET_BSP,
	VCODEC_DEV_MCU_RESET_GLOBAL,
	VCODEC_DEV_MCU_EVENT_REPORT,
};

enum vcodec_dev_msg {
	VCODEC_DEV_MCU_SET_EVENT_NOTIFY,
	VCODEC_DEV_MCU_SEND_CTRL_MSG,
	VCODEC_DEV_MCU_SEND_STREAM_MSG,
	VCODEC_DEV_MCU_GET_EVENT,
	VCODEC_DEV_MCU_SEND_IMAGE_MSG,
	VCODEC_DEV_MCU_SEND_PROC_MSG,
	VCODEC_DEV_MCU_DUMP_REALTIME_LOG,
	VCODEC_DEV_MCU_GET_REALTIME_LOG,
	VCODEC_DEV_MCU_SET_LOG_TRANSFER,
	VCODEC_DEV_MCU_SEND_LOCK_SCD_MSG,
	VCODEC_DEV_MCU_SEND_UNLOCK_SCD_MSG,
	VCODEC_DEV_MCU_SEND_DESTORY_MSG,
	VCODEC_DEV_MCU_INIT_CHAN_OUTPUT_EVENT_QUEUE,

	VCODEC_DEV_SCD_SET_POLL_IRQ_FLAG,
	VCODEC_DEV_SCD_SET_PERF_FLAG,	
	VCODEC_DEV_SCD_CONFIG_TASK, 
	VCODEC_DEV_SCD_CANCEL_TASK,

	VCODEC_DEV_BSP_SET_RING_BUFFER_FLAG,
	VCODEC_DEV_BSP_SET_RETENTION_FLAG,
	VCODEC_DEV_BSP_CLEAR,
	VCODEC_DEV_BSP_CONFIG_DECODE_TASK,
	VCODEC_DEV_BSP_GET_DECODE_RESULT_BSP0,
	VCODEC_DEV_BSP_GET_DECODE_RESULT_BSP1,

	VCODEC_DEV_PXP_SET_MULTI_CORE_FLAG,
	VCODEC_DEV_PXP_SET_SYSCACHE_FLAG,
	VCODEC_DEV_PXP_CONFIG_DECODE_TASK,
	VCODEC_DEV_PXP_GET_DECODE_RESULT,
};

struct vcodec_dev {
	int32_t (*open)(struct vcodec_dev* dev, uint8_t *reg_base);   // open the device
	int32_t (*control)(struct vcodec_dev* dev, 
		uint32_t msg_id, void *msg, uint32_t msg_size);           // control the device
	int32_t (*close)(struct vcodec_dev* dev);                     // close the device
	void (*destroy)(struct vcodec_dev* dev);                      // deinit and destroy the device
	int32_t (*suspend)(struct vcodec_dev* dev);                   // suspend the device
	int32_t (*resume)(struct vcodec_dev* dev);                    // resume the device
	int32_t (*reset)(struct vcodec_dev* dev, void *reset_info);   // reset the device
	irqreturn_t (*irq_proc)(struct vcodec_dev* dev);              // irq process
	void (*proc_read)(struct vcodec_dev* dev, void* file);        // proc read the device
};

// create and init device
struct vcodec_dev* create_vcodec_dev(enum vcodec_dev_type type, struct device* device);
void destroy_vcodec_dev(struct vcodec_dev* dev);

#endif