/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: npu intr hub control
 * Author: liushuailong
 * Create: 2020-08-05
 */

#ifndef __NPU_SOC_DEFINES_H__
#define __NPU_SOC_DEFINES_H__

#ifndef CONFIG_NPU_OHOS
#include "soc_npu_hwts_interface.h"
#endif

#define SOC_NPU_HWTS_LITE_MID               0x20
#define SOC_NPU_HWTS_TINY_MID               0x21
#define SOC_NPU_TS_CPU_MID                  0x22
#define SOC_NPU_SMMU_TCU_MID                0x23
#define SOC_NPU_LITE_CORE0_MID              0x24
#define SOC_NPU_TINY_CORE0_MID              0x25
#define SOC_NPU_LITE_CORE1_MID              0x26
#define SOC_NPU_TINY_CORE1_MID              0x27
#define SOC_NPU_LITE_AIV_MID                0x28
#define SOC_NPU_TINY_AIV_MID                0x29

#define SOC_MID_NPU_HWTS_LITE               32
#define SOC_MID_NPU_HWTS_TINY               33
#define SOC_MID_NPU_TS_CPU                  34
#define SOC_MID_NPU_SMMU_TCU                35
#define SOC_MID_NPU_LITE_CORE0              36
#define SOC_MID_NPU_TINY_CORE0              37
#define SOC_MID_NPU_LITE_CORE1              38
#define SOC_MID_NPU_TINY_CORE1              39

#define SOC_MID_IOMCU                       12
#define SOC_MID_IOMCU_DMA                   26
#define SOC_MID_ACPU_FCM_PP_QICE            46

#define HWTS_DEFAULT_MID_VALUES (((SOC_MID_IOMCU) << (SOC_NPU_HWTS_HWTS_HOST_MID_CFG_hwts_sensorhub_mid_START)) | \
    ((SOC_MID_ACPU_FCM_PP_QICE) << (SOC_NPU_HWTS_HWTS_HOST_MID_CFG_hwts_acpu_mid_START)) | \
    ((SOC_MID_NPU_TS_CPU) << (SOC_NPU_HWTS_HWTS_HOST_MID_CFG_hwts_tscpu_mid_START)))

#define SOC_SMMU_SID_TSCPU                  12
#define SOC_SMMU_SID_AICORE_NONSEC          13
#define SOC_SMMU_SID_AICORE_SEC             16

#define NPU_TZPC_SEC                        0
#define NPU_TZPC_NONSEC                     1
#define NPU_EASC_SEC                        0
#define NPU_EASC_NONSEC                     1
#define NPU_EASC_UNLOCKED                   0
#define NPU_EASC_LOCKED                     1
#define NPU_EASC_TYPE_RD                    1
#define NPU_EASC_TYPE_WR                    2
#define NPU_EASC_TYPE_WR_RD                 3
#define NPU_EASC_REGION_OFFSET              0x24
#define NPU_EASC_REGION_MAX_COUNT           8
#define HWTS_SEC_EN_NONSEC_RW_AVAILABLE     0

#define AICORE_UNLOCK_MAGICNUM              0x19101920
#define AICORE_INTERLEAVE_GRANULARITY       0 /* 0:128Byte */
#define AICORE_INIT_TIMEOUT                 30000

#define HWTS_PREFETCH_HEAD_OFFSET           0xE800
#define HWTS_PREFETCH_HEAD_PER_SIZE         64

#define NPU_HWTS_RTSQ_TINY_BITMAP       0x0000000000000003ULL
#define NPU_HWTS_RTSQ_LITE_SEC_BITMAP   0x000000000000000CULL
#define NPU_HWTS_RTSQ_FFRT_BITMAP       0x0000000000000FF0ULL
#define NPU_HWTS_RTSQ_LITE_UNSEC_BITMAP 0x0000007FFFFFF000ULL
#define NPU_HWTS_RTSQ_ISPNN_BITMAP      0x03FFFF8000000000ULL
#define NPU_HWTS_RTSQ_HTS_BITMAP        0x7C00000000000000ULL
#define NPU_HWTS_RTSQ_RESERVED          0x8000000000000000ULL

#define HWTS_SQ_SLOT_SIZE 64

/* must be 64B & and cannot be modified */
#ifndef CONFIG_NPU_OHOS
struct npu_hwts_static_swap_buff {
    SOC_NPU_HWTS_HWTS_RTSQ_BASE_ADDR0_UNION sq_base_addr0;
    SOC_NPU_HWTS_HWTS_RTSQ_BASE_ADDR1_UNION sq_base_addr1;
    SOC_NPU_HWTS_HWTS_RTSQ_CFG0_UNION sq_cfg0;
    SOC_NPU_HWTS_HWTS_RTSQ_CFG1_UNION sq_cfg1;
    SOC_NPU_HWTS_HWTS_RTSQ_CFG2_UNION sq_cfg2;
    SOC_NPU_HWTS_HWTS_RTSQ_CFG3_UNION sq_cfg3;
    SOC_NPU_HWTS_HWTS_RTSQ_CFG4_UNION sq_cfg4;
    SOC_NPU_HWTS_HWTS_RTCQ_CFG0_UNION cq_cfg0;
    SOC_NPU_HWTS_HWTS_RTCQ_BASE_ADDR0_UNION cq_base_addr0;
    SOC_NPU_HWTS_HWTS_RTCQ_BASE_ADDR1_UNION cq_base_addr1;
    SOC_NPU_HWTS_HWTS_RTSQ_CONTEXT_SWITCH_BUF_CTRL0_UNION sq_context_switch_ctrl0;
    SOC_NPU_HWTS_HWTS_RTSQ_CONTEXT_SWITCH_BUF_CTRL1_UNION sq_context_switch_ctrl1;
    unsigned int warning_status_l;
    unsigned int warning_status_h;
    unsigned int kickstart_blkid          :16;
    unsigned int context_switch_blkid0    :16;
    unsigned int res0;
};
#endif
struct npu_ffrt_model_buff {
    unsigned int err_mode   :1;
    unsigned int res0   :3;
    unsigned int va   :1;
    unsigned int pmu_id     :2;
    unsigned int res1   :9;
    unsigned int ssid   :12;
    unsigned int res2   :4; // end 0
    unsigned int r0_vld  :1;
    unsigned int p0_pri  :3;
    unsigned int res3  :12;
    unsigned int r1_vld  :1;
    unsigned int p1_pri  :3;
    unsigned int res4  :12; // end 1
    unsigned int r0_head  :16;
    unsigned int r0_tail  :16; // end 2
    unsigned int r0_swapbuf_addr_l; // end 3
    unsigned int r0_swapbuf_addr_h :16;
    unsigned int res5  :16; // end 4
    unsigned int r1_head  :16;
    unsigned int r1_tail  :16; // end 5
    unsigned int r1_swapbuf_addr_l; // end 6
    unsigned int r1_swapbuf_addr_h :16;
    unsigned int res6  :16; // end 7
};

enum npu_hwts_sqe_type {
    NPU_HWTS_SQE_AICORE = 0, // rt 0
    NPU_HWTS_SQE_VECTOR_CORE = 2,
    NPU_HWTS_SQE_PLACE_HOLDER = 3, // rt 20 21 22
    NPU_HWTS_SQE_NOTIFY_RECORD = 6, // rt 15
    NPU_HWTS_SQE_NOTIFY_WAIT = 7, // rt 14
    NPU_HWTS_SQE_WRITE_VALUE = 8,
    NPU_HWTS_SQE_AICORE_EFF = 10,
    NPU_HWTS_SQE_HTS_EVENT_RECORD = 11,
    NPU_HWTS_SQE_HTS_EVENT_WAIT = 12,
    NPU_HWTS_SQE_TYPE_RESV,
};

enum npu_hwts_ph_task_type {
    NPU_HWTS_PH_SQE_NORMAL = 0, // rt 20
    NPU_HWTS_PH_SQE_LABEL_SWITCH = 1, // rt 21
    NPU_HWTS_PH_SQE_LABEL_GOTO = 2, // rt 21
    NPU_HWTS_PH_SQE_TYPE_RESV,
};

struct hwts_sqe_head {
    unsigned int type          : 6;
    unsigned int l1_lock       : 1;
    unsigned int l1_unlock     : 1;
    unsigned int ie            : 1;
    unsigned int pre_p         : 1;
    unsigned int post_p        : 1;
    unsigned int wr_cqe        : 1;
    unsigned int rd_cond       : 1;
    unsigned int res0          : 3;
    unsigned int block_dim     : 16;
    unsigned int stream_id     : 16;
    unsigned int task_id       : 16;
    unsigned int res1;
    unsigned int res2          : 16;
    unsigned int kernel_credit : 8;
    unsigned int res3          : 8;
};

struct hwts_kernel_sqe {
    unsigned int type                : 6;
    unsigned int l1_lock             : 1;
    unsigned int l1_unlock           : 1;
    unsigned int ie                  : 1;
    unsigned int pre_p               : 1;
    unsigned int post_p              : 1;
    unsigned int wr_cqe              : 1;
    unsigned int rd_cond             : 1;
    unsigned int res0                : 3;
    unsigned int block_dim           : 16;
    unsigned int stream_id           : 16;
    unsigned int task_id             : 16;
    unsigned int res1;
    unsigned int res2                : 16;
    unsigned int kernel_credit       : 8;
    unsigned int res3                : 8;

    unsigned int schem               : 2;
    unsigned int res4                : 1;
    unsigned int icache_prefetch_cnt : 5;
    unsigned int wrr_ratio_wr        : 2;
    unsigned int wrr_ratio_rd        : 2;
    unsigned int res5                : 20;
    unsigned int res6;
    unsigned int stack_phy_base_low;
    unsigned int stack_phy_base_high;

    unsigned int task_start_pc_low;
    unsigned int task_start_pc_high  :16;
    unsigned int res7                :16;
    unsigned int task_param_ptr_low;
    unsigned int task_param_ptr_high;

    unsigned int res8;
    unsigned int res9;
    unsigned int res10;
    unsigned int res11;
};

struct hwts_label_switch_sqe {
    unsigned long long right;
    unsigned short true_label_idx;
    unsigned char condition;
    unsigned char res0[5];
};

struct hwts_label_goto_sqe {
    unsigned short label_idx;
    unsigned short res0[7];
};

struct hwts_ph_sqe {
    unsigned int type          : 6;
    unsigned int l1_lock       : 1;
    unsigned int l1_unlock     : 1;
    unsigned int ie            : 1;
    unsigned int pre_p         : 1;
    unsigned int post_p        : 1;
    unsigned int wr_cqe        : 1;
    unsigned int res0          : 4;
    unsigned int task_type     : 16;
    unsigned int stream_id     : 16;
    unsigned int task_id       : 16;
    union {
        struct hwts_label_switch_sqe label_switch;
        struct hwts_label_goto_sqe   label_goto;
    } u;
    unsigned int res1[10];
};

struct hwts_notify_sqe {
    unsigned int type          : 6;
    unsigned int res           : 2;
    unsigned int ie            : 1;
    unsigned int pre_p         : 1;
    unsigned int post_p        : 1;
    unsigned int wr_cqe        : 1;
    unsigned int res0          : 20;
    unsigned int stream_id     : 16;
    unsigned int task_id       : 16;
    unsigned int res1;
    unsigned int swapout_credit : 8;
    unsigned int res2          : 24;

    unsigned int notify_id     : 9;
    unsigned int res4          : 23;
    unsigned int res5[11];
};

struct hwts_hts_event_sqe {
    unsigned int type          : 6;
    unsigned int res0          : 2;
    unsigned int ie            : 1;
    unsigned int pre_p         : 1;
    unsigned int post_p        : 1;
    unsigned int wr_cqe        : 1;
    unsigned int res1          : 20;
    unsigned int stream_id     : 16;
    unsigned int task_id       : 16;
    unsigned int res2;
    unsigned int swapout_credit : 8;
    unsigned int res3          : 24;

    unsigned int event_id      : 10;
    unsigned int dst_id        : 6;
    unsigned int uid           : 12;
    unsigned int res4          : 4;
    unsigned int res5[11];
};

struct hwts_write_val_sqe {
    unsigned int type          : 6;
    unsigned int l1_lock       : 1;
    unsigned int l1_unlock     : 1;
    unsigned int ie            : 1;
    unsigned int pre_p         : 1;
    unsigned int post_p        : 1;
    unsigned int wr_cqe        : 1;
    unsigned int res0          : 20;
    unsigned int stream_id     : 16;
    unsigned int task_id       : 16;
    unsigned int res1;
    unsigned int res2          : 16;
    unsigned int kernel_credit : 8;
    unsigned int res3          : 8;

    unsigned int write_addr_low;
    unsigned short write_addr_high;
    unsigned short awsize       : 3; /* 0:8 bits.1:16bits 2:32bits 3:64 bits.4-7:reserved */
    unsigned short snoop        : 1;
    unsigned short res4         : 4;
    unsigned short awcache      : 4; /* cache property of destnation */
    unsigned short awprot       : 3; /* secure setting,should be setted 2 */
    unsigned short va           : 1;
    unsigned int res5[2];

    unsigned int write_value_part0;
    unsigned int write_value_part1;
    unsigned int write_value_part2;
    unsigned int write_value_part3;
    unsigned int write_value_part4;
    unsigned int write_value_part5;
    unsigned int write_value_part6;
    unsigned int write_value_part7;
};

struct hwts_cqe {
    volatile unsigned short p     : 1;
    volatile unsigned short w     : 1;
    volatile unsigned short evt   : 1;
    volatile unsigned short e     : 1;
    volatile unsigned short sq_id : 10;
    volatile unsigned short res1  : 2;
    volatile unsigned short sq_head;
    volatile unsigned short stream_id;
    volatile unsigned short task_id;
    volatile unsigned int syscnt_low; // warn status/error status
    volatile unsigned int syscnt_high;
};

enum intr_hub_l2_0_index {
    INTR_NPU2ACPU_PCR_IRQ = 0,
    INTR_NPU2ACPU_TS_WDOG_INT,
    INTR_NPU2ACPU_TS_WDOG_RES,
    INTR_NPU2ACPU_HWTS_DFX_AF_INT,
    INTR_NPU2ACPU_HWTS_DEBUG_NS,
    INTR_NPU2ACPU_EASC,
    NPU_INTR_HUB_L2_0_COUNT,
};

enum intr_hub_l2_1_index {
    INTR_NPU2ACPU_HWTS_DEBUG_S = 0,
    NPU_INTR_HUB_L2_1_COUNT,
};

enum intr_hub_l2_2_index {
    INTR_NPU2ACPU_TINY_HWTS_DFX_AF_INT,
    INTR_NPU2ACPU_TINY_HWTS_DEBUG,
    INTR_NPU2ACPU_DMMU,
    NPU_INTR_HUB_L2_2_COUNT,
};

enum intr_hub_l2_3_index {
    INTR_NPU2ACPU_HTS_SLV_ADAPTER_ERROR,
    NPU_INTR_HUB_L2_3_COUNT,
};

struct npu_easc_region_cfg {
    unsigned int start_addr_low;
    unsigned int start_addr_high;
    unsigned int end_addr_low;
    unsigned int end_addr_high;
    unsigned int mid0   : 8;
    unsigned int mid1   : 8;
    unsigned int mid2   : 8;
    unsigned int mid3   : 8;
    unsigned int secure : 1;
    unsigned int wr_rd_type : 2;
};

struct npu_error_info {
	unsigned int type;
	unsigned int value;
};

enum npu_error_info_type {
    NPU_NOSEC_ERROR = 1,
    NPU_SEC_ERROR,
    NPU_TINY_ERROR,
};
 
enum npu_error_info_value {
    BUS_ERROR = 1,
    POOL_CONFLICT,
    SQE_ERROR,
    TASK_ERROR,
};

/* abnormal address of bus error */
#define NPU_DEBUG_BUS_ERROR_ADDR 0xE5800000

#define AICORE_BASE_ADDR_SHIFT      0x14

#endif
