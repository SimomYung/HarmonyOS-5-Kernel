/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  PXPC_SC_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  wushengtao/yanmengting/jieguangwen
 * Version       :  1.0
 * Date          :  2022/3/1
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  wushengtao/yanmengting/jieguangwen 2023/11/14 14:29:09 Create file
 */

#ifndef __PXPC_SC_C_UNION_DEFINE_H__
#define __PXPC_SC_C_UNION_DEFINE_H__

/* Define the union U_PXPC_NMI_INT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_core0_nmi_int_ena
        /*
         * 写中断使能: PXPC core0 NMI使能；
         */
        unsigned int    pxpc_core0_nmi_int_ena : 1   ; /* [0] */
        // Comment of field: pxpc_core1_nmi_int_ena
        /*
         * 写中断使能: PXPC core1 NMI使能；
         */
        unsigned int    pxpc_core1_nmi_int_ena : 1   ; /* [1] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 2   ; /* [3..2] */
        // Comment of field: pxpc_core0_nmi_int_clr
        /*
         * 清中断：PXPC core0 NMI清除；RV不需配置
         */
        unsigned int    pxpc_core0_nmi_int_clr : 1   ; /* [4] */
        // Comment of field: pxpc_core1_nmi_int_clr
        /*
         * 清中断：PXPC core1 NMI清除；RV不需配置
         */
        unsigned int    pxpc_core1_nmi_int_clr : 1   ; /* [5] */
        // Comment of field: reserved_1
        /*
         * 保留
         */
        unsigned int    reserved_1            : 2   ; /* [7..6] */
        // Comment of field: pxpc_core0_nmi_int_sta
        /*
         * PXPC core0 nmi中断状态上报
         */
        unsigned int    pxpc_core0_nmi_int_sta : 1   ; /* [8] */
        // Comment of field: pxpc_core1_nmi_int_sta
        /*
         * PXPC core1 nmi中断状态上报
         */
        unsigned int    pxpc_core1_nmi_int_sta : 1   ; /* [9] */
        unsigned int    reserved_2            : 22  ; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_PXPC_NMI_INT;

/* Define the union U_PXPC_NMI_MASKW_REG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_nmi_maskw_reg
        /*
         * RV不需配置。
         * 【15】:CORE1 NMI中断mask，默认为1；
         * 【14】:CORE0 NMI中断mask，默认为1；
         * 【13:0】:备份状态，可读写;
         */
        unsigned int    pxpc_nmi_maskw_reg    : 16  ; /* [15..0] */
        // Comment of field: pxpc_nmi_maskw_bitsel
        /*
         * 为[15:0]每个bit的使能位，只有bitmask对应的bit为为1，[15:0]对应的bit才会生效。
         */
        unsigned int    pxpc_nmi_maskw_bitsel : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_PXPC_NMI_MASKW_REG;

/* Define the union U_PXPC_CORE_HALT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_core0_halt_ena
        /*
         * PXPC core0 halt
         */
        unsigned int    pxpc_core0_halt_ena   : 1   ; /* [0] */
        // Comment of field: pxpc_core1_halt_ena
        /*
         * PXPC core1 halt，不允许RV配置
         */
        unsigned int    pxpc_core1_halt_ena   : 1   ; /* [1] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 2   ; /* [3..2] */
        // Comment of field: pxpc_core0_halt_clr
        /*
         * PXPC core0 解halt
         */
        unsigned int    pxpc_core0_halt_clr   : 1   ; /* [4] */
        // Comment of field: pxpc_core1_halt_clr
        /*
         * PXPC core1 解halt,不允许RV配置
         */
        unsigned int    pxpc_core1_halt_clr   : 1   ; /* [5] */
        // Comment of field: reserved_1
        /*
         * 保留
         */
        unsigned int    reserved_1            : 2   ; /* [7..6] */
        // Comment of field: pxpc_core0_halt_sta
        /*
         * PXPC core0 halt状态上报，默认halt
         */
        unsigned int    pxpc_core0_halt_sta   : 1   ; /* [8] */
        // Comment of field: pxpc_core1_halt_sta
        /*
         * PXPC core1 halt状态上报，默认halt
         */
        unsigned int    pxpc_core1_halt_sta   : 1   ; /* [9] */
        unsigned int    reserved_2            : 22  ; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_PXPC_CORE_HALT;

/* Define the union U_CRG_CTRL1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: core0_clk_ctrl
        /*
         * Core0 时钟控制
         * 1 - 打开；0 - 关闭。
         */
        unsigned int    core0_clk_ctrl        : 1   ; /* [0] */
        // Comment of field: core1_clk_ctrl
        /*
         * Core1 时钟控制
         * 1 - 打开；0 - 关闭。
         */
        unsigned int    core1_clk_ctrl        : 1   ; /* [1] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 1   ; /* [2] */
        // Comment of field: dmc_auto_icg_byp
        /*
         * 1 - 自动门控不使能；
         * 0 - 自动门控使能
         */
        unsigned int    dmc_auto_icg_byp      : 1   ; /* [3] */
        // Comment of field: scdbg_clk_en
        /*
         * scdbg时钟控制。
         * 1 - 打开；0 - 关闭。
         */
        unsigned int    scdbg_clk_en          : 1   ; /* [4] */
        // Comment of field: mpu_clk_en
        /*
         * mpu时钟控制。
         * 1 - 打开；0 - 关闭。
         */
        unsigned int    mpu_clk_en            : 1   ; /* [5] */
        // Comment of field: pdma_if_auto_icg_byp
        /*
         * PDMA到mem_subsys中间通路的自动门控：
         * '1 - 自动门控不使能；
         * 0 - 自动门控使能
         */
        unsigned int    pdma_if_auto_icg_byp  : 1   ; /* [6] */
        // Comment of field: core0_srst_req
        /*
         * Core0软复位控制。
         * 1 - 复位请求有效；0 - 复位请求无效；
         */
        unsigned int    core0_srst_req        : 1   ; /* [7] */
        // Comment of field: core1_srst_req
        /*
         * Core1软复位控制。
         * 1 - 复位请求有效；0 - 复位请求无效；
         */
        unsigned int    core1_srst_req        : 1   ; /* [8] */
        // Comment of field: pdma_srst_req
        /*
         * PXP DMA的软复位。
         * 1 - 复位请求有效；0 - 复位请求无效；
         * 需要在PXP DMA所有模块全部IDLE才可以使能
         */
        unsigned int    pdma_srst_req         : 1   ; /* [9] */
        // Comment of field: reserved_1
        /*
         * 保留
         */
        unsigned int    reserved_1            : 1   ; /* [10] */
        // Comment of field: core0_ic_clk_en
        /*
         * core0 ictl时钟控制。
         * 1 - 打开；0 - 关闭。
         */
        unsigned int    core0_ic_clk_en       : 1   ; /* [11] */
        // Comment of field: core1_ic_clk_en
        /*
         * core1 ictl时钟控制。
         * 1 - 打开；0 - 关闭。
         */
        unsigned int    core1_ic_clk_en       : 1   ; /* [12] */
        // Comment of field: reserved_2
        /*
         * 保留
         */
        unsigned int    reserved_2            : 3   ; /* [15..13] */
        // Comment of field: crg_ctrl1_mask
        /*
         * 为[15:0]每个bit的使能位，只有bitmask对应的bit为为1，[15:0]对应的bit才会生效。
         */
        unsigned int    crg_ctrl1_mask        : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_CRG_CTRL1;

/* Define the union U_SYS_IDLE_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_dma_ch_idle
        /*
         * pxpc_dma_ch0/1_idle 和 dma lbus idle
         */
        unsigned int    pxpc_dma_ch_idle      : 1   ; /* [0] */
        unsigned int    hdma_idle             : 1   ; /* [1] */
        // Comment of field: pxpc_dma_ch0_idle
        /*
         * DMA channel 0(rd/wr) 空闲指示
         */
        unsigned int    pxpc_dma_ch0_idle     : 1   ; /* [2] */
        // Comment of field: pxpc_dma_ch1_idle
        /*
         * DMA channel 1(rd/wr) 空闲指示
         */
        unsigned int    pxpc_dma_ch1_idle     : 1   ; /* [3] */
        // Comment of field: qsp2lbus_busy
        /*
         * 配置口转换桥busy信号，访问pxpc内部空间会拉高
         */
        unsigned int    qsp2lbus_busy         : 1   ; /* [4] */
        unsigned int    lbus_idle             : 1   ; /* [5] */
        unsigned int    io2qspm_idle          : 1   ; /* [6] */
        // Comment of field: busy
        /*
         * core0 & core1 & axim & lbus & pdma 子系统busy状态
         */
        unsigned int    busy                  : 1   ; /* [7] */
        // Comment of field: core0_wfi
        /*
         * core0 wfi状态
         */
        unsigned int    core0_wfi             : 1   ; /* [8] */
        // Comment of field: core1_wfi
        /*
         * core1 wfi状态
         */
        unsigned int    core1_wfi             : 1   ; /* [9] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 22  ; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SYS_IDLE_STAT;

//==============================================================================
/* Define the global struct */
typedef struct {
    U_PXPC_NMI_INT         PXPC_NMI_INT                     ; /* 0x0 */
    U_PXPC_NMI_MASKW_REG   PXPC_NMI_MASKW_REG               ; /* 0x4 */
    U_PXPC_CORE_HALT       PXPC_CORE_HALT                   ; /* 0x8 */
    unsigned int           reserved_0                       ; /* 0xc */
    unsigned int           SC_CTRL_EN_S                     ; /* 0x10 */
    unsigned int           SC_CTRL_DIS_S                    ; /* 0x14 */
    unsigned int           SC_CTRL_STAT_S                   ; /* 0x18 */
    unsigned int           SC_CTRL_MASK_S                   ; /* 0x1c */
    unsigned int           SC_CTRL_STAT_MSK_S               ; /* 0x20 */
    unsigned int           RV_TASK_QUENE_IDX                ; /* 0x24 */
    unsigned int           reserved_1[2]                    ; /* 0x28~0x2c */
    unsigned int           RV_CFG_S_0                       ; /* 0x30 */
    unsigned int           reserved_2[244]                  ; /* 0x34~0x400 */
    U_CRG_CTRL1            CRG_CTRL1                        ; /* 0x404 */
    unsigned int           reserved_3[10]                   ; /* 0x408~0x42c */
    U_SYS_IDLE_STAT        SYS_IDLE_STAT                    ; /* 0x430 */
    unsigned int           reserved_4[499]                  ; /* 0x434~0xbfc */
    unsigned int           SC_CTRL_EN_NS                    ; /* 0xc00 */
    unsigned int           SC_CTRL_DIS_NS                   ; /* 0xc04 */
    unsigned int           SC_CTRL_STAT_NS                  ; /* 0xc08 */
    unsigned int           SC_CTRL_MASK_NS                  ; /* 0xc0c */
    unsigned int           SC_CTRL_STAT_MSK_NS              ; /* 0xc10 */
    unsigned int           RV_TASK_QUENE_IDX_NS             ; /* 0xc14 */
    unsigned int           reserved_5[2]                    ; /* 0xc18~0xc1c */
    unsigned int           RV_CFG_NS_0                      ; /* 0xc20 */
    unsigned int           RV_CFG_NS_1                      ; /* 0xc24 */
} S_PXPC_SC_REGS_TYPE;

//==============================================================================
/* Define the union of all registers */
typedef union {
    unsigned int    u32;

    U_PXPC_NMI_INT         PXPC_NMI_INT;
    U_PXPC_NMI_MASKW_REG   PXPC_NMI_MASKW_REG;
    U_PXPC_CORE_HALT       PXPC_CORE_HALT;
    U_CRG_CTRL1            CRG_CTRL1;
    U_SYS_IDLE_STAT        SYS_IDLE_STAT;
} U_PXPC_SC_REGS_TYPE;

#endif /* __PXPC_SC_C_UNION_DEFINE_H__ */
