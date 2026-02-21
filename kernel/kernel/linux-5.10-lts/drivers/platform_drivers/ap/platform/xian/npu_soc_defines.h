/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: npu intr hub control
 * Author: liushuailong
 * Create: 2020-08-05
 */

#ifndef __NPU_SOC_DEFINES_H__
#define __NPU_SOC_DEFINES_H__

#include <stddef.h>
#ifndef CONFIG_NPU_OHOS
#include "soc_npu_hwts_interface.h"
#endif

#define SOC_MID_NPU_HWTS_LITE               32
#define SOC_MID_NPU_HWTS_TINY               33
#define SOC_MID_NPU_TS_CPU                  34
#define SOC_MID_NPU_SMMU_TCU                35
#define SOC_MID_NPU_LITE_CORE0              36
#define SOC_MID_NPU_TINY_CORE0              37

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

typedef enum {
    HWTS_PRIORITY_GRP_PLATFORM0 = 0,
    HWTS_PRIORITY_GRP_PLATFORM1,
    HWTS_PRIORITY_GRP_PLATFORM2,
    HWTS_PRIORITY_GRP_HIGHEST,
    HWTS_PRIORITY_GRP_HIGHER,
    HWTS_PRIORITY_GRP_HIGH,
    HWTS_PRIORITY_GRP_MIDDLE,
    HWTS_PRIORITY_GRP_LOW,
    HWTS_PRIORITY_GRP_MAX
} hwts_priority_grp_t;

typedef enum {
    SMART_ISP_TYPE_MIN,
    SMART_ISP_TYPE_SMART_PALETTE = SMART_ISP_TYPE_MIN,
    SMART_ISP_TYPE_SMART_ENHANCER,
    SMART_ISP_TYPE_AIAF,
    SMART_ISP_TYPE_AIAWB,
    SMART_ISP_TYPE_SMART_MSC,
    SMART_ISP_TYPE_MAX
} smartisp_services_type_t;

typedef enum {
    HWTS_RTSQ_TYPE_NS_AIC = 0,
    HWTS_RTSQ_TYPE_NS_AIV,
    HWTS_RTSQ_TYPE_TINY,
    HWTS_RTSQ_TYPE_SMARTISP,
    HWTS_RTSQ_TYPE_SEC,
    HWTS_RTSQ_TYPE_FFRT,
    HWTS_RTSQ_TYPE_HTS,
    HWTS_RTSQ_TYPE_NS_EFFAIC,
    HWTS_RTSQ_TYPE_RSV,
    HWTS_RTSQ_TYPE_MAX
} hwts_rtsq_type_t;

typedef enum {
    HWTS_HOST_TYPE_ISP = 0,
    HWTS_HOST_TYPE_ACPU = 1,
    HWTS_HOST_TYPE_SENSORHUB = 2,
    HWTS_HOST_TYPE_TSCPU = 3,
    HWTS_HOST_TYPE_RSV
} hwts_host_type_t;

typedef struct hwts_rtsq_config_item {
    hwts_rtsq_type_t rtsq_type;
    hwts_host_type_t host_type;
    hwts_priority_grp_t priority;
    unsigned short int num;
    void * sub_config_item;
    unsigned long long bitmap;
} hwts_rtsq_config_item_t;

typedef struct smart_isp_rtsq_config_table {
    smartisp_services_type_t service_type;
    unsigned short int num;
    unsigned short int reuse_flag;
} smart_isp_rtsq_config_table_t;

typedef struct lite_unsec_rtsq_config_table {
    hwts_priority_grp_t priority;
    unsigned short int num;
} lite_unsec_rtsq_config_table_t;

 
static smart_isp_rtsq_config_table_t g_smart_isp_sq_table[] = {
    {SMART_ISP_TYPE_SMART_PALETTE, 2, 0},
    {SMART_ISP_TYPE_SMART_ENHANCER, 2, 0},
    {SMART_ISP_TYPE_AIAF, 2, 0},
    {SMART_ISP_TYPE_AIAWB, 2, 0},
    {SMART_ISP_TYPE_SMART_MSC, 2, 0},
};

static lite_unsec_rtsq_config_table_t g_ns_aic_sq_table[] = {
    {HWTS_PRIORITY_GRP_PLATFORM0, 0},
    {HWTS_PRIORITY_GRP_PLATFORM1, 0},
    {HWTS_PRIORITY_GRP_PLATFORM2, 4},
    {HWTS_PRIORITY_GRP_HIGHEST, 4},
    {HWTS_PRIORITY_GRP_HIGHER, 4},
    {HWTS_PRIORITY_GRP_HIGH, 4},
    {HWTS_PRIORITY_GRP_MIDDLE, 4},
    {HWTS_PRIORITY_GRP_LOW, 4},
};

static lite_unsec_rtsq_config_table_t g_ns_aiv_sq_table[] = {
    {HWTS_PRIORITY_GRP_PLATFORM0, 0},
    {HWTS_PRIORITY_GRP_PLATFORM1, 0},
    {HWTS_PRIORITY_GRP_PLATFORM2, 2},
    {HWTS_PRIORITY_GRP_HIGHEST, 2},
    {HWTS_PRIORITY_GRP_HIGHER, 2},
    {HWTS_PRIORITY_GRP_HIGH, 1},
    {HWTS_PRIORITY_GRP_MIDDLE, 1},
    {HWTS_PRIORITY_GRP_LOW, 1},
};

static hwts_rtsq_config_item_t g_hwts_config_table[] = {
    /* tiny configure hwts sq */
    {HWTS_RTSQ_TYPE_TINY, HWTS_HOST_TYPE_SENSORHUB, HWTS_PRIORITY_GRP_HIGHER, 2, NULL, 0},
    /* sec lite configure hwts sq */
    {HWTS_RTSQ_TYPE_SEC, HWTS_HOST_TYPE_ACPU, HWTS_PRIORITY_GRP_PLATFORM1, 2, NULL, 0},
    /* non-sec ispnn configure hwts sq */
    {HWTS_RTSQ_TYPE_SMARTISP, HWTS_HOST_TYPE_TSCPU, HWTS_PRIORITY_GRP_MAX, 10, (void *)g_smart_isp_sq_table, 0},
    /* non-sec rsv configure hwts sq */
    {HWTS_RTSQ_TYPE_RSV, HWTS_HOST_TYPE_RSV, HWTS_PRIORITY_GRP_PLATFORM2, 2, NULL, 0},
    /* non-sec ffrt configure hwts sq */
    {HWTS_RTSQ_TYPE_FFRT, HWTS_HOST_TYPE_ACPU, HWTS_PRIORITY_GRP_PLATFORM2, 8, NULL, 0},
    /* non-sec aic configure hwts sq */
    {HWTS_RTSQ_TYPE_NS_AIC, HWTS_HOST_TYPE_TSCPU, HWTS_PRIORITY_GRP_MAX, 24, (void *)g_ns_aic_sq_table, 0},
    /* non-sec aiv configure hwts sq */
    {HWTS_RTSQ_TYPE_NS_AIV, HWTS_HOST_TYPE_TSCPU, HWTS_PRIORITY_GRP_MAX, 9, (void *)g_ns_aiv_sq_table, 0},
    /* non-sec hts configure hwts sq: DLSS/AudioNN */
    {HWTS_RTSQ_TYPE_HTS, HWTS_HOST_TYPE_TSCPU, HWTS_PRIORITY_GRP_PLATFORM2, 5, NULL, 0},
};

#define array_num(array_name) (sizeof((array_name)) / sizeof((array_name[0])))


static inline hwts_rtsq_config_item_t *get_hwts_config(unsigned short int idx)
{
    return &g_hwts_config_table[idx];
}

static inline unsigned int get_hwts_config_num(void)
{
    return array_num(g_hwts_config_table);
}

static inline unsigned int get_hwts_sub_config_num(hwts_rtsq_type_t rtsq_type)
{
    if (rtsq_type == HWTS_RTSQ_TYPE_SMARTISP) {
        return array_num(g_smart_isp_sq_table);
    }
    if (rtsq_type == HWTS_RTSQ_TYPE_NS_AIC) {
        return array_num(g_ns_aic_sq_table);
    }
    if (rtsq_type == HWTS_RTSQ_TYPE_NS_AIV) {
        return array_num(g_ns_aiv_sq_table);
    }
    return -1;
}

/* 0~63 bit RTSQ bitmap */
#define GEN_RTSQ_BITMAP(start, end) (((end) % 64 == 63) ? (((uint64_t)0xFFFFFFFFFFFFFFFFULL) << (start) % 64) : ((1ULL << ((end) % 64 + 1)) - (1ULL << (start) % 64)))

static unsigned long long g_rtsq_bitmap[HWTS_RTSQ_TYPE_MAX];

#define HWTS_RTSQ_TABLE_BITMAP_INIT() \
    do { \
        unsigned int start_num = 0; \
        unsigned int end_num = 0; \
        unsigned int i = 0; \
        for (; i < get_hwts_config_num(); i++) { \
            hwts_rtsq_config_item_t* item = get_hwts_config(i); \
            if (item->num > 0) { \
                end_num = start_num + item->num - 1; \
            } else { \
                end_num = start_num; \
            } \
            item->bitmap = GEN_RTSQ_BITMAP(start_num, end_num); \
            g_rtsq_bitmap[item->rtsq_type] = item->bitmap; \
            start_num = end_num + 1; \
        } \
    } while (0)

#define NPU_HWTS_RTSQ_TINY_BITMAP         (g_rtsq_bitmap[HWTS_RTSQ_TYPE_TINY])
#define NPU_HWTS_RTSQ_LITE_UNSEC_BITMAP   ((g_rtsq_bitmap[HWTS_RTSQ_TYPE_NS_AIC]) | (g_rtsq_bitmap[HWTS_RTSQ_TYPE_NS_AIV]))
#define NPU_HWTS_RTSQ_FFRT_BITMAP         (g_rtsq_bitmap[HWTS_RTSQ_TYPE_FFRT])
#define NPU_HWTS_RTSQ_HTS_BITMAP          (g_rtsq_bitmap[HWTS_RTSQ_TYPE_HTS])
#define NPU_HWTS_RTSQ_SMARTISP_BITMAP     (g_rtsq_bitmap[HWTS_RTSQ_TYPE_SMARTISP])
#define NPU_HWTS_RTSQ_LITE_SEC_BITMAP     (g_rtsq_bitmap[HWTS_RTSQ_TYPE_SEC])

#define HWTS_PREFETCH_HEAD_OFFSET           0xE800
#define HWTS_PREFETCH_HEAD_PER_SIZE         64

#define HWTS_SQ_SLOT_SIZE 64

static inline hwts_rtsq_type_t get_hwts_rtsq_type(unsigned short int rtsq_id)
{
    if (rtsq_id >= HWTS_SQ_SLOT_SIZE) {
        return HWTS_RTSQ_TYPE_MAX;
    }

    unsigned int start_num = 0;
    unsigned int end_num = 0;
    unsigned int i = 0;
    for (; i < get_hwts_config_num(); i++) {
        hwts_rtsq_config_item_t* item = get_hwts_config(i);
        if (item->num > 0) {
            end_num = start_num + item->num - 1;
        } else {
            end_num = start_num;
        }
        if (rtsq_id >= start_num && rtsq_id <= end_num) {
            return item->rtsq_type;
        }
        start_num = end_num + 1;
    }
    return HWTS_RTSQ_TYPE_MAX;
}

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

#define AICORE_BASE_ADDR_SHIFT      0x14

#endif
