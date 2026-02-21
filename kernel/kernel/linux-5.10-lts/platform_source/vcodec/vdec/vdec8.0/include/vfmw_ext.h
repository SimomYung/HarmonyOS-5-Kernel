/*
 * vfmw_ext.h
 *
 * This is for vfmw.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef VFMW_EXT_H
#define VFMW_EXT_H

#include "vcodec_types.h"

/* keep value the same with mcu */
#ifndef MAX_FRAME_NUM
#define MAX_FRAME_NUM 32
#endif

typedef struct {
	UADDR reg_phy_addr;
	uint32_t reg_range;
} vfmw_module_reg_info;

typedef enum {
	IPC_INT0_IRQ = 0,
	IPC_MBX0_IRQ,
	WATCH_DOG_IRQ,
	BSP_NORM_IRQ,
	PXPC_NORM_IRQ,
	SCD_NORM_IRQ,
	VDEC_NORM_IRQ,
	VDEC_HTS_IRQ,
	MAX_VDEC_NORM_IRQ,
} vdec_norm_irq_num;

typedef enum {
	BSP_SAFE_IRQ = 8,
	PXPC_SAFE_IRQ,
	SCD_SAFE_IRQ,
	VDEC_SAFE_IRQ,
	MAX_VDEC_SAFE_IRQ,
} vdec_safe_irq_num;

typedef enum {
	MFDE_MODULE,
	SCD_MOUDLE,
	CRG_MOUDLE,
	MMU_SID_MOUDLE,
	DP_MONIT_MOUDLE,
	MMU_TBU_MODULE,
	MAX_INNER_MODULE,
} vfmw_inner_module;

typedef struct {
	uint32_t irq_norm[MAX_VDEC_NORM_IRQ];
	uint32_t irq_safe[MAX_VDEC_SAFE_IRQ];
	uint32_t is_es_plat;
	uint32_t is_fpga;
	uint32_t pcie_mmap_host_start_reg;
	vfmw_module_reg_info module_reg[MAX_INNER_MODULE];
    vfmw_module_reg_info reg_base_info;
    uint32_t vdec_reserved_iova_start;
    uint32_t vdec_reserved_iova_size;
    struct device *dev;
} vdec_dts;

typedef struct {
	uint8_t int_mask;
	uint8_t scd_start;
	uint8_t scd_init_clr;
	uint8_t slice_check;
	uint8_t safe_flag;
	uint8_t scd_over;
	int32_t buffer_share_fd;
	UADDR buffer_first;
	UADDR buffer_last;
	UADDR buffer_init;
	UADDR roll_addr;
	UADDR next_addr;
	int32_t msg_share_fd;
	UADDR dn_msg_phy;
	UADDR up_msg_phy;
	uint32_t up_len;
	uint32_t up_step;
	uint32_t std_type;
	uint32_t pre_lsb;
	uint32_t pre_msb;
	uint32_t byte_valid;
	uint32_t short_num;
	uint32_t start_num;
	uint32_t src_eaten;
	uint32_t cfg_unid;
	uint32_t int_unid;
	int32_t ret_errno;
} scd_reg;

typedef struct {
	int32_t scd_id;
	scd_reg reg;
} scd_reg_ioctl;

struct scd_cfg {
	void *fd;
	scd_reg_ioctl scd_reg_cfg;
	scd_reg scd_state_reg;
};

struct bsp_clear_info {
	uint32_t bsp_index;
	uint32_t reg_index;
};

#ifndef VDH_BSP_NUM
#define VDH_BSP_NUM 2
#endif

typedef struct {
	// destination bsp of current task
	uint32_t bsp_id; 

	// task parameters
	UADDR bsp_hw_task_addr;
	uint32_t bsp_task_frm_no;
	uint32_t bsp_hw_task_len;
} bsp_task;
typedef struct {
	uint32_t chan_id;
	uint32_t work_mode;
	uint64_t cur_time;
	uint32_t task_num;
	bsp_task task[VDH_BSP_NUM];
} bsp_decode;

typedef struct {
    uint32_t chan_id;
    uint32_t early_end_cnt;
	uint32_t bs_over_cnt;
	uint32_t first_slice_lost;
    uint32_t frm_num;
} bsp_error;

typedef struct {
	UADDR ctu_info_addr[VDH_BSP_NUM];
	UADDR ctu_res_addr[VDH_BSP_NUM];
	uint32_t using_reg_index[VDH_BSP_NUM];
} bsp_ringbuffer_cfg;

typedef enum {
	BSP_POWERON_NORMAL = 0,
	BSP_POWERON_RESET,
	BSP_POWERON_SR,
} bsp_poweron_mode;

typedef struct {
	UADDR nsf_sw_addr;
	uint32_t use_ringbuffer_flag;
	bsp_ringbuffer_cfg ringbuffer_info_nsf;
	uint32_t ctu_info_len;
	uint32_t ctu_res_len;
	bsp_poweron_mode poweron_mode;
} bsp_powerup_cfg;

typedef struct {
	UADDR syscache_start_addr;
	uint32_t syscache_len;
	uint8_t muti_core_en;
	uint8_t is_reset;
	uint8_t is_sc_allocated;
	uint8_t reserved;
} pxp_powerup_cfg;

typedef struct {
	uint32_t chan_id;
	uint32_t int_reg;
	uint32_t int_flag;
	uint32_t int_unid;
	uint32_t err_flag;
	uint32_t rd_slc_msg;
	uint32_t slice_num;
	uint32_t ctb_num;
	uint32_t bsp_cycle;
	uint32_t is_need_wait;
	int32_t ret_errno;
} bsp_decode_done;
typedef struct {
	uint32_t task_lenth;
	UADDR task_addr;
	uint32_t frm_num;
} pxp_task;

typedef struct {
	uint32_t chan_id;
	uint32_t work_mode;
	uint64_t cur_time;

	uint32_t task_num; // 多slice最多用到2个task
	uint32_t reset_pass_frm; // consistent with hal pxp_decode struct
	pxp_task task[VDH_BSP_NUM];
} pxp_decode;

typedef struct {
	uint32_t chan_id;
	uint32_t int_reg;
	uint32_t int_flag;
	uint32_t int_unid;
	uint32_t err_flag;
	uint32_t rd_slc_msg;
	uint32_t slice_num;
	uint32_t ctb_num;
	uint32_t pxp_cycle;
	uint32_t is_need_wait;
	int32_t ret_errno;
} pxp_decode_done;

typedef enum {
	DEC_ASYNC_MODE,
	DEC_SYNC_MODE,
} work_mode;

/* keep value the same with mcu */
typedef enum {
	VFMW_CID_CREATE_CHAN,            /* create channel */
	VFMW_CID_DESTROY_CHAN,           /* destroy channel */
	VFMW_CID_CFG_CHAN,               /* config the decode channel */
	VFMW_CID_START_CHAN,             /* start channel */
	VFMW_CID_STOP_CHAN,              /* stop channel */
	VFMW_CID_SET_STREAM_INTF,        /* set the stream access interface for the decode channel */
	VFMW_CID_RELEASE_STREAM,         /* reset scd to release stream buffers */
	VFMW_CID_RESET_CHAN_WITH_OPTION, /* reset channel with options to keep some characters of the channel */
	VFMW_CID_EXTRA_WAKEUP_THREAD,   /* bvt set wake up vfmw thread */
	VFMW_CID_ALLOC_MEM_TO_CHANNEL,
	VFMW_CID_SET_FRAME_MEM,
	VFMW_CID_FLUSH_CHAN,
	VFMW_CID_SYNC_EXT_BUFFER,
	VFMW_CID_ACTIVATE_CHANNEL,
	VFMW_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION,
	VFMW_CMD_IPC_SUSPEND,
	VFMW_CMD_IPC_RESUME,
	VFMW_CID_MCORE_SUSPEND,
	VFMW_CID_MCORE_RESUME,
	VFMW_CID_MAX,
} vfmw_cid;

typedef enum {
	VFMW_CHAN_STATE_NULL = 0,
	VFMW_CHAN_STATE_CREATE,
} vfmw_chan_state;

typedef enum {
	FIRMWARE_UP_MSG_NEW_IMAGE = 0,
	FIRMWARE_UP_MSG_NEED_ARRANGE = 1,
	FIRMWARE_UP_MSG_H264_FIELD = 2,
	FIRMWARE_UP_MSG_COLOR_ASPECTS_CHG = 3,
	FIRMWARE_UP_MSG_HDR_SEI_CHG = 4,
	FIRMWARE_UP_MSG_LAST_FRAME = 5,
	FIRMWARE_UP_MSG_UNSUPPORT = 6,
	FIRMWARE_UP_MSG_RELEASE_STREAM = 7,
	FIRMWARE_UP_MSG_IMAGE_FLUSH_DONE = 8,
	FIRMWARE_UP_MSG_GLOBAL_RESET = 9,
	FIRMWARE_UP_MSG_LOAD_OVERTHRESHOLD = 10,
	FIRMWARE_DOWN_MSG_CONFIG_STREAM_BUFFER = 11,
	FIRMWARE_DOWN_MSG_RELEASE_IMAGE = 12,
} FIRMWARE_MSG_TYPE;

struct mcu_log_get_msg {
	uint32_t mcu_log_read_offset;
	void __user *usr_buf;
	uint32_t buf_size;
	uint32_t clr_sign;
	uint32_t read_size;
};

typedef struct
{
	uint32_t reserved;
} decode_done_info;

struct mcu_real_time_log {
	int8_t *buffer;
	uint32_t size;
};
#endif
