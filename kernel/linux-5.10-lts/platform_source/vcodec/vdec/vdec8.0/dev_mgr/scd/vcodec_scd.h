/*
 * vcodec_scd.h
 *
 * Headers define of vcodec scd device
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
#ifndef VCODEC_SCD_H
#define VCODEC_SCD_H

#include "dev_mgr.h"
#include "vfmw_ext.h"
#include "vcodec_osal.h"
#include "vdec/VDE_SUB_CTRL_NS_c_union_define.h"
#include "dev_create.h"

#define SCD_INVALID_CMD_ID     0xFFFFFFFFU

enum vcodec_scd_status {
	VCODEC_SCD_STATUS_POWER_OFF,
	VCODEC_SCD_STATUS_POWER_ON,
	VCODEC_SCD_STATUS_SUSPEND,
	VCODEC_SCD_STATUS_FAULT,
	VCODEC_SCD_STATUS_BUTT
};

struct vcodec_dev_scd {
	struct vcodec_dev dev;                   // interfaces
	enum vcodec_dev_type type;               // VCODEC_DEV_SCD
	size_t size;                             // sizeof(struct vcodec_dev_scd)
	struct device *device;                   // parent device

	enum vcodec_scd_status status;           // power status
	uint32_t poll_irq_enable;                // enable poll irq
	uint32_t perf_enable;                    // enable perf print
	scd_reg reg;

	UADDR reg_phy;
	uint32_t reg_size;
	uint8_t *reg_base_vir_addr;
	S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl_vir_addr;
	uint8_t *scd_reg_base_vir_addr;                // only used in fpga version

	int32_t event;
	int32_t sema;
};

struct vcodec_scd_control_proc_tbl {
	uint32_t cmd;
	uint32_t msg_size;
	uint32_t validate_scd_status;
	int32_t (*proc)(struct vcodec_dev_scd *dev, void *msg);
};

int32_t vcodec_scd_init_dev(struct vcodec_dev_scd *dev, uint8_t *reg_base);
void vcodec_scd_deinit_dev(struct vcodec_dev_scd *dev);
int32_t vcodec_scd_reset_dev(struct vcodec_dev_scd *dev);
void vcodec_scd_set_irq_mask(struct vcodec_dev_scd* dev);
irqreturn_t vcodec_scd_interrupt_handling(struct vcodec_dev_scd* dev);
const struct vcodec_scd_control_proc_tbl *vcodec_scd_get_control_proc_tbl(void);

#endif
