/*
 * vcodec_bsp.h
 *
 * Headers define of vcodec bsp device
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
#ifndef VCODEC_BSP_H
#define VCODEC_BSP_H
#include "dev_mgr.h"
#include "vcodec_osal.h"
#include "vcodec_vdec_memory.h"
#include "vdec/VDE_SUB_CTRL_NS_c_union_define.h"
#include "vdec/BSP_TOP_SCTRL_c_union_define.h"
#include "vdec/BSPS_SC_c_union_define.h"
#include "vdec/BSP_TASK_QUEUE_REG_NSF_c_union_define.h"
#include "dev_create.h"

#define BSP_INVALID_CMD_ID     0xFFFFFFFFU

#define BSP_ACK_TIME_OUT      100
#define BSP_IMG_DATA_SIZE     (100 * 1024)
 
#define VDEC_CS_BSP_IMAGE_PATH "/vendor/firmware/sec_bsps.bin"
#define VDEC_BSP_IMAGE_PATH    "/vendor/firmware/sec_bsps_eng.bin"
#define VDEC_CERT_SIZE         (8U * 1024U)
#define BSP_SW_DATA_SIZE       (14U * 1024U)

enum bsp_isr_type
{
	BSP_SUBTASK_DONE_NSF0 = 0,
	BSP_SUBTASK_DONE_NSF1 = 1,
	BSP_SUBTASK_DONE_NSF2 = 2,
	BSP_SUBTASK_DONE_NSF3 = 3,
	BSP_TASK_DONE_NSF0 = 4,
	BSP_TASK_DONE_NSF1 = 5,
	BSP_TASK_DONE_NSF2 = 6,
	BSP_TASK_DONE_NSF3 = 7,
	BSP_CTUROW_DONE_NSF0 = 8,
	BSP_CTUROW_DONE_NSF1 = 9,
	BSP_CTUROW_DONE_NSF2 = 10,
	BSP_CTUROW_DONE_NSF3 = 11,
	BSP_CTU_BP_NSF0 = 12,
	BSP_CTU_BP_NSF1 = 13,
	BSP_CTU_BP_NSF2 = 14,
	BSP_CTU_BP_NSF3 = 15,
	BSP_ABN_WARN_INT0 = 16,
	BSP_ABN_WARN_INT1 = 17,
	BSP_ABN_WARN_INT2 = 18,
	BSP_ABN_WARN_INT3 = 19,
	BSP_CLR_ACK0 = 20,
	BSP_CLR_ACK1 = 21,
	BSP_CLR_ACK2 = 22,
	BSP_CLR_ACK3 = 23,
	BSP_NMI_ACK = 24,
	BSP_POWERDOWN_ACK = 25,
	BSP_INIT_DONE = 26,
	BSP_CORE_WARM = 27,
};

struct bsp_buffer_cfg {
	uint32_t ctu_info_len;
	uint32_t ctu_res_len;
	UADDR ctu_info_addr[VDH_BSP_NUM];
	UADDR ctu_res_addr[VDH_BSP_NUM];
	uint32_t using_reg_index[VDH_BSP_NUM];
	UADDR sw_addr;
};

enum vcodec_bsp_status {
	VCODEC_BSP_STATUS_POWER_OFF,
	VCODEC_BSP_STATUS_POWER_ON,
	VCODEC_BSP_STATUS_SUSPEND,
	VCODEC_BSP_STATUS_FAULT,
	VCODEC_BSP_STATUS_BUTT
};

struct vcodec_dev_bsp {
	struct vcodec_dev dev;                     // interfaces
	enum vcodec_dev_type type;                 // VCODEC_DEV_BSP
	size_t size;                               // sizeof(struct vcodec_dev_bsp)
	struct device *device;                     // parent device

	void *image_buffer;                        // bsp image buffer
	size_t image_size;                         // bsp image buffer size
	uint32_t use_ring_buffer_flag;             // whether use ring buffer, default is 1
	uint32_t retention_flag;                   // whether work in retention mode, default is 1;
	uint32_t reset_flag;                       // whether in resetting, default is 0
	struct vdec_hw_hal_mem_info ring_buffer;   // bsp unsafe ring buffer
	struct vdec_hw_hal_mem_info sw_buffer;     // bsp unsafe sw buffer
	struct bsp_buffer_cfg  buffer_info;        // bsp buffer info
	enum vcodec_bsp_status status;             // power status

	S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl_vir_addr;
	S_BSP_TOP_SCTRL_REGS_TYPE *bsp_top_sctrl_vir_addr;          // only used in fpga version
	S_BSP_TASK_QUEUE_REG_NSF_REGS_TYPE *bsp_taskqueue_vir_addr; // only used in fpga version
	S_BSPS_SC_REGS_TYPE *bsp_sctrl_vir_addr;                    // only used in fpga version
	uint32_t next_idle_reg[VDH_BSP_NUM];                        // only used in fpga version
	uint8_t *reg_base_vir_addr;                                 // only used in fpga version
	struct vdec_fifo_buffer *bsp0_decode_result_queue;          // only used in fpga version
	struct vdec_fifo_buffer *bsp1_decode_result_queue;          // only used in fpga version

	uint32_t bsp_power_on_ack;
	uint32_t bsp_power_off_ack;
	uint32_t bsp0_clear_ack;
	uint32_t bsp1_clear_ack;
};

struct vcodec_bsp_control_proc_tbl {
	uint32_t cmd;
	uint32_t msg_size;
	uint32_t validate_bsp_status;
	int32_t (*proc)(struct vcodec_dev_bsp *dev, void *msg);
};

struct bsp_reset_info {
	uint32_t chan_id;
	uint32_t frm_num;
	uint32_t bsp_index;
	uint32_t bsp_reg_decoding_index[VDH_BSP_NUM];
};

int32_t vcodec_bsp_init_dev(struct vcodec_dev_bsp* dev, uint8_t *reg_base);
void vcodec_bsp_deinit_dev(struct vcodec_dev_bsp* dev);
int32_t vcodec_bsp_open_dev(struct vcodec_dev_bsp* dev);
int32_t vcodec_bsp_close_dev(struct vcodec_dev_bsp* dev);
int32_t vcodec_bsp_reset_dev(struct vcodec_dev_bsp* dev);
const uint8_t *vcodec_bsp_sw_data(void);

int32_t vcodec_bsp_load_image(struct vcodec_dev_bsp* dev);
void vcodec_bsp_free_image(struct vcodec_dev_bsp *dev);
int32_t vcodec_bsp_alloc_buffer(struct vcodec_dev_bsp* dev);
void vcodec_bsp_free_buffer(struct vcodec_dev_bsp* dev);

const struct vcodec_bsp_control_proc_tbl *vcodec_bsp_get_control_proc_tbl(void);
irqreturn_t vcodec_bsp_interrupt_handling(struct vcodec_dev_bsp* dev);
void vcodec_bsp_set_irq_mask(struct vcodec_dev_bsp* dev);

#endif
