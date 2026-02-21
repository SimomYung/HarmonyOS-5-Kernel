/*
 * vcodec_pxp.h
 *
 * Headers define of vcodec pxp device
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
#ifndef VCODEC_PXP_H
#define VCODEC_PXP_H
#include "dev_mgr.h"
#include "vcodec_osal.h"
#include "vcodec_vdec_memory.h"
#include "vdec/VDE_SUB_CTRL_NS_c_union_define.h"
#include "vdec/PXPC_SC_c_union_define.h"
#include "vdec/PXPC_TASK_QUEUE_c_union_define.h"
#include "vdec/PXPC_DMA_c_union_define.h"
#include "dev_create.h"

#define PXP_INVALID_CMD_ID     0xFFFFFFFFU
#define PXP_ACK_TIME_OUT       100
#define VDEC_PXP_IMAGE_PATH    "/vendor/firmware/sec_pxpc_eng.bin"
#define VDEC_CS_PXP_IMAGE_PATH "/vendor/firmware/sec_pxpc.bin"
#define VDEC_CERT_SIZE         (8U * 1024U)
#define PXP_IMG_DATA_SIZE      (1024 * 1024)

enum vcodec_pxp_status {
	VCODEC_PXP_STATUS_POWER_OFF,
	VCODEC_PXP_STATUS_POWER_ON,
	VCODEC_PXP_STATUS_SUSPEND,
	VCODEC_PXP_STATUS_FAULT,
	VCODEC_PXP_STATUS_BUTT
};

enum pxp_isr_type
{
	PXPC_EOP = 0,
	PXPC_TASK_DONE = 1,
	PXPH_TASK_DONE = 2,
	PXP_RSV3 = 3,
	PXP_RSV4 = 4,
	PXP_RSV5 = 5,
	PXP_RSV6 = 6,
	PXP_RSV7 = 7,
	PXP_RSV8 = 8,
	PXP_RSV9 = 9,
	PXP_RSV10 = 10,
	PXPH_TIME_OUT = 11,
	PXPC_WARNING = 12,
	PXPC_NMI_ACK = 13,
	PXPC_INIT_DONE = 14,
	PXPC_PWR_DOWN_ACK = 15,
};

struct vcodec_pxp_buffer_info {
	UADDR syscache_start_addr;
	uint32_t syscache_len;
};

struct vcodec_dev_pxp {
	struct vcodec_dev dev;                   // interfaces
	enum vcodec_dev_type type;               // VCODEC_DEV_PXP
	size_t size;                             // sizeof(struct vcodec_dev_pxp)
	struct device *device;                   // parent device

	void *image_buffer;                      // pxp image buffer
	size_t image_size;                       // pxp image buffer size
	enum vcodec_pxp_status status;           // power status
	uint32_t multi_core_enable;              // whether enable multi-core, default is 1
	uint32_t syscache_enable;                // whether enable syscache, default is 1
	uint32_t is_sc_allocated;                // whether pxp dev has already alloced syscache
	uint32_t reset_flag;                     // whether in resetting, default is 0

	struct vdec_hw_hal_mem_info pxp_sc;         // syscache memory
	struct vcodec_pxp_buffer_info buffer_info;  // buffer information

	uint8_t *reg_base_vir_addr;
	S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl_vir_addr;
	S_PXPC_SC_REGS_TYPE *pxpc_subctrl_vir_addr;
	S_PXPC_DMA_REGS_TYPE *pxpc_dma_vir_addr;              // only used in fpga version
	S_PXPC_TASK_QUEUE_REGS_TYPE *pxpc_taskqueue_vir_addr; // only used in fpga version	
	struct vdec_fifo_buffer *pxp_decode_result_queue;     // only used in fpga version

	uint32_t pxp_power_on_ack;
	uint32_t pxp_power_off_ack;
};

struct vcodec_pxp_control_proc_tbl {
	uint32_t cmd;
	uint32_t msg_size;
	uint32_t validate_pxp_status;
	int32_t (*proc)(struct vcodec_dev_pxp *dev, void *msg);
};

struct pxp_reset_info {
	uint32_t chan_id;
	uint32_t frm_num;
};

int32_t vcodec_pxp_init_dev(struct vcodec_dev_pxp *dev, uint8_t *reg_base);
void vcodec_pxp_deinit_dev(struct vcodec_dev_pxp *dev);
int32_t vcodec_pxp_open_dev(struct vcodec_dev_pxp* dev);
int32_t vcodec_pxp_close_dev(struct vcodec_dev_pxp* dev);
int32_t vcodec_pxp_reset_dev(struct vcodec_dev_pxp* dev);
int32_t vcodec_pxp_load_image(struct vcodec_dev_pxp* dev);
void vcodec_pxp_free_image(struct vcodec_dev_pxp* dev);

const struct vcodec_pxp_control_proc_tbl *vcodec_pxp_get_control_proc_tbl(void);
irqreturn_t vcodec_pxp_interrupt_handling(struct vcodec_dev_pxp* dev);
void vcodec_pxp_set_irq_mask(struct vcodec_dev_pxp* dev);

int32_t vcodec_pxp_alloc_syscache(struct vcodec_dev_pxp* dev);
void vcodec_pxp_free_syscache(struct vcodec_dev_pxp* dev);
void vcodec_pxp_config_sc(struct vcodec_dev_pxp* dev);

#endif
