/*
 * vcodec_mcu.h
 *
 * Headers define of vcodec mcu device
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
#ifndef VCODEC_MCU_H
#define VCODEC_MCU_H
#include "dev_mgr.h"
#include "vcodec_osal.h"
#include "vcodec_vdec_memory.h"
#include "vcodec_mcu_ddr_map.h"
#include "vdec/VDE_SUB_CTRL_NS_c_union_define.h"
#include "dev_create.h"

#define VDEC_MCORE_IMG  "/vendor/firmware/vdec_mcore.img"
#define MCU_SUSPEND_WAIT_CNT  (50)
#define MCU_INVALID_CMD_ID    (0xFFFFFFFFU)

enum vcodec_mcu_status {
	VCODEC_MCU_STATUS_POWER_OFF,
	VCODEC_MCU_STATUS_POWER_ON,
	VCODEC_MCU_STATUS_SUSPEND,
	VCODEC_MCU_STATUS_FAULT,
	VCODEC_MCU_STATUS_BUTT
};

struct vcodec_dev_mcu {
	struct vcodec_dev dev;                     // interfaces
	enum vcodec_dev_type type;                 // VCODEC_DEV_MCU
	size_t size;                               // sizeof(struct vcodec_dev_mcu)
	struct device *device;                     // parent device

	uint32_t mcu_heap_iova;
	struct vdec_hw_hal_mem_info mcu_exec_mem;
	struct vdec_hw_hal_mem_info msg_pool_info;
	enum vcodec_mcu_status status;             // power status

	uint8_t *reg_base_vir_addr;
	S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl_vir_addr;

	struct completion mcu_startup_done;
	void (*event_notify)(enum vcodec_dev_event_notify, void *);
	void (*transfer_log)(uint8_t* buffer, uint32_t buff_len);
};

struct vcodec_mcu_control_proc_tbl {
	uint32_t cmd;
	uint32_t msg_size;
	uint32_t validate_mcu_status;
	int32_t (*proc)(struct vcodec_dev_mcu *dev, void *msg);
};

void vcodec_mcu_dev_init(struct vcodec_dev_mcu* dev);
void vcodec_mcu_dev_deinit(struct vcodec_dev_mcu* dev);
int32_t vcodec_mcu_dev_open(struct vcodec_dev_mcu* dev, uint8_t *reg_base);
int32_t vcodec_mcu_dev_suspend(struct vcodec_dev_mcu* dev);
int32_t vcodec_mcu_dev_resume(struct vcodec_dev_mcu* dev);
void vcodec_mcu_dev_close(struct vcodec_dev_mcu* dev);
const struct vcodec_mcu_control_proc_tbl *vcodec_mcu_dev_get_control_proc_tbl(void);

#endif
