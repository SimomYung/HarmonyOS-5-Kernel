/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  VDE_SUB_CTRL_NS_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  xxx
 * Version       :  1.0
 * Date          :  2013/3/10
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  xxx 2023/11/14 14:25:09 Create file
 */

#ifndef __VDE_SUB_CTRL_NS_C_UNION_DEFINE_H__
#define __VDE_SUB_CTRL_NS_C_UNION_DEFINE_H__

/* Define the union U_MCU_CRG0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_clk_en
        /*
         * MCU clk enable signal
         * 0: close MCU clock;
         * 1: enable MCU clock;
         */
        unsigned int    mcu_clk_en            : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 31  ; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_CRG0;

/* Define the union U_MCU_CRG1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 1   ; /* [0] */
        // Comment of field: ipc_clk_en
        /*
         * IPC clk enable signal
         * 0: close IPC clock;
         * 1: enable IPC clock;
         */
        unsigned int    ipc_clk_en            : 1   ; /* [1] */
        // Comment of field: tim_clk_en
        /*
         * Timer clk enable signal
         * 0: close Timer clock;
         * 1: enable Timer clock;
         */
        unsigned int    tim_clk_en            : 1   ; /* [2] */
        // Comment of field: uart_clk_en
        /*
         * Uart clk enable signal
         * 0: close Uart clock;
         * 1: enable Uart clock;
         */
        unsigned int    uart_clk_en           : 1   ; /* [3] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_CRG1;

/* Define the union U_VDEC_CRG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: scd_clk_en
        /*
         * scd clk enable signal
         * 0: close scd clock;
         * 1: enable scd clock;
         */
        unsigned int    scd_clk_en            : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 3   ; /* [3..1] */
        // Comment of field: bsp_clk_en
        /*
         * bsp clk enable signal
         * 0: close bsp clock;
         * 1: enable bsp clock;
         */
        unsigned int    bsp_clk_en            : 1   ; /* [4] */
        // Comment of field: reserved_1
        /*
         * 保留
         */
        unsigned int    reserved_1            : 3   ; /* [7..5] */
        // Comment of field: pxpc_clk_en
        /*
         * pxpc clk enable signal
         * 0: close pxpc clock;
         * 1: enable pxpc clock;
         */
        unsigned int    pxpc_clk_en           : 1   ; /* [8] */
        // Comment of field: pxpc_vdec_clk_en
        /*
         * pxpc vdec clk enable signal
         * 0: close pxpc vdec clock;
         * 1: enable pxpc vdec clock;
         */
        unsigned int    pxpc_vdec_clk_en      : 1   ; /* [9] */
        // Comment of field: reserved_2
        /*
         * 保留
         */
        unsigned int    reserved_2            : 2   ; /* [11..10] */
        // Comment of field: pxp_hw_clk_en
        /*
         * pxp_hw vdec clk enable signal
         * 0: close pxp_hw vdec clock;
         * 1: enable pxp_hw vdec clock;
         */
        unsigned int    pxp_hw_clk_en         : 1   ; /* [12] */
        // Comment of field: hw_qic_clk_axi_en
        /*
         * pxp_hw qic axi clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    hw_qic_clk_axi_en     : 1   ; /* [13] */
        // Comment of field: reserved_3
        /*
         * 保留
         */
        unsigned int    reserved_3            : 2   ; /* [15..14] */
        // Comment of field: core_qic_clk_vdec_en
        /*
         * vdec core_qic vdec clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    core_qic_clk_vdec_en  : 1   ; /* [16] */
        // Comment of field: core_qic_clk_axi_en
        /*
         * vdec core_qic axi clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    core_qic_clk_axi_en   : 1   ; /* [17] */
        // Comment of field: core_qic_clk_pxpc_en
        /*
         * vdec core_qic pxpc clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    core_qic_clk_pxpc_en  : 1   ; /* [18] */
        // Comment of field: core_qic_clk_pxphw_en
        /*
         * vdec core_qic pxphw clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    core_qic_clk_pxphw_en : 1   ; /* [19] */
        // Comment of field: cfg_qic_clk_apb_en
        /*
         * vdec cfg_qic apb clk enable signal，保留
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    cfg_qic_clk_apb_en    : 1   ; /* [20] */
        // Comment of field: cfg_qic_clk_vdec_en
        /*
         * vdec core_qic vdec clk enable signal，保留
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    cfg_qic_clk_vdec_en   : 1   ; /* [21] */
        // Comment of field: cfg_qic_clk_in_sys_en
        /*
         * vdec core_qic in sys clk enable signal，保留
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    cfg_qic_clk_in_sys_en : 1   ; /* [22] */
        // Comment of field: cfg_qic_clk_axi_en
        /*
         * vdec core_qic axi clk enable signal，保留
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    cfg_qic_clk_axi_en    : 1   ; /* [23] */
        // Comment of field: smmu_clk_axi_en
        /*
         * SMMU时钟使能配置。   1:时钟常开，0： 时钟关闭
         */
        unsigned int    smmu_clk_axi_en       : 1   ; /* [24] */
        // Comment of field: dpm_freq_sel
        /*
         * dpm_freq_sel，保留
         */
        unsigned int    dpm_freq_sel          : 3   ; /* [27..25] */
        // Comment of field: reserved_4
        /*
         * reserved
         */
        unsigned int    reserved_4            : 4   ; /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDEC_CRG;

/* Define the union U_VDEC_ROOT_CRG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: calc_vdec_clk_en
        /*
         * CALC TOP vdec clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    calc_vdec_clk_en      : 1   ; /* [0] */
        // Comment of field: calc_axi_clk_en
        /*
         * CALC TOP axi clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    calc_axi_clk_en       : 1   ; /* [1] */
        // Comment of field: calc_pxpc_clk_en
        /*
         * CALC TOP pxpc clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    calc_pxpc_clk_en      : 1   ; /* [2] */
        // Comment of field: calc_in_sys_clk_en
        /*
         * CALC TOP in_sys clk enable signal
         * 0: close clock;
         * 1: enable clock;
         */
        unsigned int    calc_in_sys_clk_en    : 1   ; /* [3] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDEC_ROOT_CRG;

/* Define the union U_MCU_RST0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_por_rst
        /*
         * MCU reset control (power on reset (POR) )
         * 0: force MCU exit reset state;
         * 1: force MCU enter reset state.
         */
        unsigned int    mcu_por_rst           : 1   ; /* [0] */
        // Comment of field: mcu_wdt_rst
        /*
         * MCU watchdog reset control
         * 0: force MCU exit watchdog reset state;
         * 1: force MCU enter watchdog reset state.
         */
        unsigned int    mcu_wdt_rst           : 1   ; /* [1] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 6   ; /* [7..2] */
        // Comment of field: mcu_core_wait
        /*
         * MCU core wait state control，保留，不使用
         * 0: force MCU exit  core wait state;
         * 1: force MCU enter  core wait state.
         */
        unsigned int    mcu_core_wait         : 1   ; /* [8] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 23  ; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_RST0;

/* Define the union U_MCU_RST1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 1   ; /* [0] */
        // Comment of field: ipc_soft_rst
        /*
         * IPC soft reset control 
         * 0: force IPC exit reset state;
         * 1: force IPC enter reset state.
         */
        unsigned int    ipc_soft_rst          : 1   ; /* [1] */
        // Comment of field: tim_soft_rst
        /*
         * timer soft reset control 
         * 0: force timer exit reset state;
         * 1: force timer enter reset state.
         */
        unsigned int    tim_soft_rst          : 1   ; /* [2] */
        // Comment of field: uart_soft_rst
        /*
         * Uart soft reset control 
         * 0: force Uart exit reset state;
         * 1: force Uart enter reset state.
         */
        unsigned int    uart_soft_rst         : 1   ; /* [3] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_RST1;

/* Define the union U_VDEC_RST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: scd_soft_rst
        /*
         * scd soft reset control scd_soft_rst
         * 0: force' scd exit reset state;
         * 1: force 'scd enter reset state.
         */
        unsigned int    scd_soft_rst          : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 3   ; /* [3..1] */
        // Comment of field: bsp_soft_rst
        /*
         * bsp soft reset control
         * 0: force' bsp exit reset state;
         * 1: force 'bsp enter reset state.
         */
        unsigned int    bsp_soft_rst          : 1   ; /* [4] */
        // Comment of field: reserved_1
        /*
         * 保留
         */
        unsigned int    reserved_1            : 3   ; /* [7..5] */
        // Comment of field: pxpc_soft_rst
        /*
         * pxpc soft reset control，对应pxpc rst_pxpc_dbg_n
         * 0: force pxpc exit reset state;
         * 1: force pxpc enter reset state.
         */
        unsigned int    pxpc_soft_rst         : 1   ; /* [8] */
        // Comment of field: pxpc_vdec_soft_rst
        /*
         * pxpc vdec clk soft reset control
         * 0: force pxpc vdec clk exit reset state;
         * 1: force pxpc vdec clk enter reset state.
         */
        unsigned int    pxpc_vdec_soft_rst    : 1   ; /* [9] */
        // Comment of field: pxpc_top_soft_rst
        /*
         * pxpc 时钟域soft reset control，对应pxpc rst_pxpc_n
         * 0: force pxpc exit reset state;
         * 1: force pxpc enter reset state.
         */
        unsigned int    pxpc_top_soft_rst     : 1   ; /* [10] */
        // Comment of field: reserved_2
        /*
         * 保留
         */
        unsigned int    reserved_2            : 1   ; /* [11] */
        // Comment of field: pxphw_soft_rst
        /*
         * pxphw soft reset control
         * 0: force' pxphw exit reset state;
         * 1: force 'pxphw enter reset state.
         */
        unsigned int    pxphw_soft_rst        : 1   ; /* [12] */
        // Comment of field: hw_qic_clk_axi_soft_rst
        /*
         * pxp_hw_qic axi clk soft reset control
         */
        unsigned int    hw_qic_clk_axi_soft_rst : 1   ; /* [13] */
        // Comment of field: reserved_3
        /*
         * 保留
         */
        unsigned int    reserved_3            : 2   ; /* [15..14] */
        // Comment of field: core_qic_clk_vdec_soft_rst
        /*
         * core_qic vdec clk soft reset control
         */
        unsigned int    core_qic_clk_vdec_soft_rst : 1   ; /* [16] */
        // Comment of field: core_qic_clk_axi_soft_rst
        /*
         * core_qic vdec axi soft reset control
         */
        unsigned int    core_qic_clk_axi_soft_rst : 1   ; /* [17] */
        // Comment of field: core_qic_clk_pxpc_soft_rst
        /*
         * core_qic pxpc clk soft reset control
         */
        unsigned int    core_qic_clk_pxpc_soft_rst : 1   ; /* [18] */
        // Comment of field: core_qic_clk_pxphw_soft_rst
        /*
         * core_qic pxp_hw clk soft reset control
         */
        unsigned int    core_qic_clk_pxphw_soft_rst : 1   ; /* [19] */
        // Comment of field: cfg_qic_clk_apb_soft_rst
        /*
         * cfg_qic apb clk soft reset control，保留
         */
        unsigned int    cfg_qic_clk_apb_soft_rst : 1   ; /* [20] */
        // Comment of field: cfg_qic_clk_vdec_soft_rst
        /*
         * cfg_qic vdec clk soft reset control，保留
         */
        unsigned int    cfg_qic_clk_vdec_soft_rst : 1   ; /* [21] */
        // Comment of field: cfg_qic_clk_in_sys_soft_rst
        /*
         * cfg_qic in sys clk soft reset control，保留
         */
        unsigned int    cfg_qic_clk_in_sys_soft_rst : 1   ; /* [22] */
        // Comment of field: cfg_qic_clk_axi_soft_rst
        /*
         * cfg_qic axi clk soft reset control，保留
         */
        unsigned int    cfg_qic_clk_axi_soft_rst : 1   ; /* [23] */
        // Comment of field: smmu_clk_axi_soft_rst
        /*
         * SMMU软复位。   1:复位，0： 不复位
         */
        unsigned int    smmu_clk_axi_soft_rst : 1   ; /* [24] */
        // Comment of field: hts_soft_rst
        /*
         * HTS软复位。   1:复位，0： 不复位
         */
        unsigned int    hts_soft_rst          : 1   ; /* [25] */
        // Comment of field: reserved_4
        /*
         * reserved
         */
        unsigned int    reserved_4            : 6   ; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDEC_RST;

/* Define the union U_VDEC_ROOT_RST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: calc_vdec_soft_rst
        /*
         * CALC TOP vdec reset control
         * 0: force exit reset state;
         * 1: force enter reset state.
         */
        unsigned int    calc_vdec_soft_rst    : 1   ; /* [0] */
        // Comment of field: calc_axi_soft_rst
        /*
         * CALC TOP vdec reset control
         * 0: force exit reset state;
         * 1: force enter reset state.
         */
        unsigned int    calc_axi_soft_rst     : 1   ; /* [1] */
        // Comment of field: calc_pxpc_soft_rst
        /*
         * CALC TOP vdec reset control
         * 0: force exit reset state;
         * 1: force enter reset state.
         */
        unsigned int    calc_pxpc_soft_rst    : 1   ; /* [2] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 29  ; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDEC_ROOT_RST;

/* Define the union U_VDE_CK_EN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxp_hw_clk_gt_en
        /*
         * pxp_hw输入工作时钟门控使能
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    pxp_hw_clk_gt_en      : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * pxpc输入工作时钟门控使能，预留
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    reserved_0            : 1   ; /* [1] */
        // Comment of field: reserved_1
        /*
         * bsp输入工作时钟门控使能，预留
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    reserved_1            : 1   ; /* [2] */
        // Comment of field: scd_clk_gt_en
        /*
         * scd输入工作时钟门控使能
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    scd_clk_gt_en         : 1   ; /* [3] */
        // Comment of field: reserved_2
        /*
         * reserved
         */
        unsigned int    reserved_2            : 12  ; /* [15..4] */
        // Comment of field: mem_clk_gt_en
        /*
         * VDEC MEM输入工作时钟门控使能
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    mem_clk_gt_en         : 1   ; /* [16] */
        // Comment of field: mem_clk_gt_en_bsp
        /*
         * BSP MEM输入工作时钟门控使能
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    mem_clk_gt_en_bsp     : 1   ; /* [17] */
        // Comment of field: mem_clk_gt_en_pxpc
        /*
         * PXPC MEM输入工作时钟门控使能
         * 0: 不做自动门控;
         * 1: 使能自动门控.
         */
        unsigned int    mem_clk_gt_en_pxpc    : 1   ; /* [18] */
        // Comment of field: reserved_3
        /*
         * reserved
         */
        unsigned int    reserved_3            : 13  ; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDE_CK_EN;

/* Define the union U_BSP_TASK_INDICATOR0_NS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: task_indicator_bsp0
        /*
         * BSP0未处理任务个数,任务队列最大值是8
         */
        unsigned int    task_indicator_bsp0   : 8   ; /* [7..0] */
        // Comment of field: task_thredth_bsp0
        /*
         * BSP0未处理任务阈值，用来判断RV下发任务是否超出阈值溢出
         */
        unsigned int    task_thredth_bsp0     : 8   ; /* [15..8] */
        // Comment of field: task_indicator_bsp1
        /*
         * BSP1未处理任务个数,任务队列最大值是8
         */
        unsigned int    task_indicator_bsp1   : 8   ; /* [23..16] */
        // Comment of field: task_thredth_bsp1
        /*
         * BSP1未处理任务阈值
         */
        unsigned int    task_thredth_bsp1     : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_TASK_INDICATOR0_NS;

/* Define the union U_PXPC_TASK_INDICATOR_NS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: task_indicator_pxpc
        /*
         * PXPC未处理任务个数
         */
        unsigned int    task_indicator_pxpc   : 8   ; /* [7..0] */
        // Comment of field: task_thredth_pxpc
        /*
         * PXPC未处理任务阈值
         */
        unsigned int    task_thredth_pxpc     : 8   ; /* [15..8] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_PXPC_TASK_INDICATOR_NS;

/* Define the union U_NEW_TASK_NS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: new_task_pause_bsp0
        /*
         * RISCV新配置给BSP0的任务标志，脉冲，每产生一个脉冲，task_pending_cnt_bsp0自增1
         */
        unsigned int    new_task_pause_bsp0   : 1   ; /* [0] */
        // Comment of field: new_task_pause_bsp1
        /*
         * RISCV新配置给BSP1的任务标志，脉冲，每产生一个脉冲，task_pending_cnt_bsp1自增1
         */
        unsigned int    new_task_pause_bsp1   : 1   ; /* [1] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 14  ; /* [15..2] */
        // Comment of field: new_task_pause_pxpc
        /*
         * RISCV新配置给BSP1的任务标志，脉冲，每产生一个脉冲，task_pending_cnt_pxpc自增1
         */
        unsigned int    new_task_pause_pxpc   : 1   ; /* [16] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 15  ; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_NEW_TASK_NS;

/* Define the union U_INTR_NS_PXPC_WR0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_pxpc_wr_ns
        /*
         * PXPC写中断，共16bit，含义参见文档：http://shasvn01-rd:6801/svn/HIS_K3_CBB_SVN/VCODEC/HiVcodecV800/03.Share/3.3 架构/02.架构方案/PXPC/VDECV800 PXPC_TOPS与RISC-V交互中断源分配.xlsx
         */
        unsigned int    intr_pxpc_wr_ns       : 16  ; /* [15..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_NS_PXPC_WR0;

/* Define the union U_INTR_NS_PXPC0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_pxpc_ns
        /*
         * PXPC写中断(到MCU)，
         * 前16bit含义参见文档：http://shasvn01-rd:6801/svn/HIS_K3_CBB_SVN/VCODEC/HiVcodecV800/03.Share/3.3 架构/02.架构方案/PXPC/VDECV800 PXPC_TOPS与RISC-V交互中断源分配.xlsx
         * bit16为pxpc_wdg_int硬中断；
         * bit17为pxpc_warn_int硬中断；
         */
        unsigned int    intr_pxpc_ns          : 18  ; /* [17..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_NS_PXPC0;

/* Define the union U_INTR_NS_PXPC1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_pxpc_acpu_ns
        /*
         * PXPC写中断(到ACPU)上报，默认屏蔽
         * bit0~15为pxpc软中断，每bit含义同intr_pxpc_ns；
         * bit16为pxpc_wdg_int硬中断；
         * bit17为pxpc_warn_int硬中断；
         */
        unsigned int    intr_pxpc_acpu_ns     : 18  ; /* [17..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_NS_PXPC1;

/* Define the union U_INTR_NS_BSP2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_bsp_ns1
        /*
         * BSP写硬中断(到MCU)
         * bit0:bsp_wtd_int
         * bit1:预留硬中断
         * bit2:bsp_core_err
         * bit3:预留硬中断
         */
        unsigned int    intr_bsp_ns1          : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_NS_BSP2;

/* Define the union U_INTR_NS_BSP3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_bsp_acpu_ns1
        /*
         * BSP写硬中断(到ACPU)，默认屏蔽
         * bit0:bsp_wtd_int
         * bit1:预留硬中断
         * bit2:bsp_core_err
         * bit3:预留硬中断
         */
        unsigned int    intr_bsp_acpu_ns1     : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_NS_BSP3;

/* Define the union U_INTR_MASK_NS_PXPC0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_pxpc_mask
        /*
         * PXPC中断屏蔽信号(到MCU)
         */
        unsigned int    intr_pxpc_mask        : 18  ; /* [17..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_MASK_NS_PXPC0;

/* Define the union U_INTR_MASK_NS_PXPC1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_pxpc_acpu_mask
        /*
         * PXPC中断屏蔽信号(到ACPU)，默认屏蔽中断
         */
        unsigned int    intr_pxpc_acpu_mask   : 18  ; /* [17..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_MASK_NS_PXPC1;

/* Define the union U_INTR_MASK_NS_BSP2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_bsp_mask1
        /*
         * BSP写硬中断(到MCU)屏蔽信号
         */
        unsigned int    intr_bsp_mask1        : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_MASK_NS_BSP2;

/* Define the union U_INTR_MASK_NS_BSP3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_bsp_acpu_mask1
        /*
         * BSP写硬中断(到ACPU)屏蔽信号
         */
        unsigned int    intr_bsp_acpu_mask1   : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_MASK_NS_BSP3;

/* Define the union U_INTR_CLR_NS_PXPC0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_pxpc_clr
        /*
         * PXPC中断清除信号(到MCU和ACPU)
         */
        unsigned int    intr_pxpc_clr         : 16  ; /* [15..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_CLR_NS_PXPC0;

/* Define the union U_INTR_SEL_NS_DSS_SYNC */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_120hz_sel_ns
        /*
         * 120hz中断选择(非安全)： 不再使用
         * 0：选择edp2vdec同步中断
         * 1：选择dsi02vdec同步中断
         * 2：选择dsi12vdec同步中断
         */
        unsigned int    intr_120hz_sel_ns     : 2   ; /* [1..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_SEL_NS_DSS_SYNC;

/* Define the union U_INTR_CNT_STAT0_NS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_subtask_intr0_cnt_ns
        /*
         * bsp core0 subtask中断计数器(非安全)
         */
        unsigned int    bsp_subtask_intr0_cnt_ns : 4   ; /* [3..0] */
        // Comment of field: bsp_subtask_intr1_cnt_ns
        /*
         * bsp core1 subtask中断计数器(非安全)
         */
        unsigned int    bsp_subtask_intr1_cnt_ns : 4   ; /* [7..4] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 8   ; /* [15..8] */
        // Comment of field: bsp_task_intr0_cnt_ns
        /*
         * bsp core0 task中断计数器(非安全)
         */
        unsigned int    bsp_task_intr0_cnt_ns : 4   ; /* [19..16] */
        // Comment of field: bsp_task_intr1_cnt_ns
        /*
         * bsp core1 task中断计数器(非安全)
         */
        unsigned int    bsp_task_intr1_cnt_ns : 4   ; /* [23..20] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_CNT_STAT0_NS;

/* Define the union U_INTR_CNT_STAT1_NS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dss_120hz_intr_cnt_ns
        /*
         * dss 120hz中断计数器(非安全)
         */
        unsigned int    dss_120hz_intr_cnt_ns : 4   ; /* [3..0] */
        // Comment of field: pxpc_frm_intr_cnt_ns
        /*
         * pxpc帧完成中断计数器(非安全)
         */
        unsigned int    pxpc_frm_intr_cnt_ns  : 4   ; /* [7..4] */
        // Comment of field: pxpc_task_intr_cnt_ns
        /*
         * pxpc任务完成中断计数器(非安全)
         */
        unsigned int    pxpc_task_intr_cnt_ns : 4   ; /* [11..8] */
        // Comment of field: pxpc_maintask_intr_cnt_ns
        /*
         * pxpc主任务完成中断计数器(非安全)
         */
        unsigned int    pxpc_maintask_intr_cnt_ns : 4   ; /* [15..12] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_CNT_STAT1_NS;

/* Define the union U_INTR_CNT_RSV_NS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: intr_cnt_rsv_ns
        /*
         * 预留中断计数器(非安全)
         */
        unsigned int    intr_cnt_rsv_ns       : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_INTR_CNT_RSV_NS;

/* Define the union U_SCD_SOFTRST_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: scd_srst_req
        /*
         * SCD软复位请求
         */
        unsigned int    scd_srst_req          : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 7   ; /* [7..1] */
        // Comment of field: scd_force_ack
        /*
         * SCD强制软复位ACK
         */
        unsigned int    scd_force_ack         : 1   ; /* [8] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 7   ; /* [15..9] */
        // Comment of field: scd_srst_ok
        /*
         * SCD软复位OK信号
         */
        unsigned int    scd_srst_ok           : 1   ; /* [16] */
        // Comment of field: reserved_2
        /*
         * reserved
         */
        unsigned int    reserved_2            : 15  ; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SCD_SOFTRST_CFG;

/* Define the union U_VDEC_IDLE_ST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vdec_idle
        /*
         * VDEC idle
         */
        unsigned int    vdec_idle             : 1   ; /* [0] */
        // Comment of field: vdec_scd_idle
        /*
         * VDEC scd模块idle
         */
        unsigned int    vdec_scd_idle         : 1   ; /* [1] */
        // Comment of field: vdec_bsp_idle
        /*
         * VDEC bsp模块idle
         */
        unsigned int    vdec_bsp_idle         : 1   ; /* [2] */
        // Comment of field: reserved_0
        /*
         * VDEC bsp core0 idle
         */
        unsigned int    reserved_0            : 1   ; /* [3] */
        // Comment of field: reserved_1
        /*
         * VDEC bsp core1 idle
         */
        unsigned int    reserved_1            : 1   ; /* [4] */
        // Comment of field: reserved_2
        /*
         * reserved
         */
        unsigned int    reserved_2            : 1   ; /* [5] */
        // Comment of field: reserved_3
        /*
         * reserved
         */
        unsigned int    reserved_3            : 1   ; /* [6] */
        // Comment of field: vdec_pxpc_idle
        /*
         * VDEC pxpc模块idle
         */
        unsigned int    vdec_pxpc_idle        : 1   ; /* [7] */
        // Comment of field: vdec_pxpc_bus_idle
        /*
         * VDEC pxp bus idle
         */
        unsigned int    vdec_pxpc_bus_idle    : 1   ; /* [8] */
        unsigned int    reserved_4            : 1   ; /* [9] */
        // Comment of field: vdec_pxp_hw_idle
        /*
         * VDEC pxp_hw模块idle
         */
        unsigned int    vdec_pxp_hw_idle      : 1   ; /* [10] */
        // Comment of field: vdec_core_data_qic_idle
        /*
         * vdec_core_data_qic模块idle
         */
        unsigned int    vdec_core_data_qic_idle : 1   ; /* [11] */
        // Comment of field: vdec_core_cfg_qic_idle
        /*
         * vdec_core_cfg_qic模块idle
         */
        unsigned int    vdec_core_cfg_qic_idle : 1   ; /* [12] */
        // Comment of field: vdec_cfg_qic_idle
        /*
         * vdec_cfg_qic模块idle
         */
        unsigned int    vdec_cfg_qic_idle     : 1   ; /* [13] */
        // Comment of field: tbu_idle
        /*
         * tbu模块idle
         */
        unsigned int    tbu_idle              : 1   ; /* [14] */
        unsigned int    reserved_5            : 1   ; /* [15] */
        // Comment of field: tbu_dti_rs_idle
        /*
         * TBU DTI RS模块idle
         */
        unsigned int    tbu_dti_rs_idle       : 1   ; /* [16] */
        // Comment of field: media1_vdec_qic_msg_idle
        /*
         * media1_vdec_qic模块HTS部分idle
         */
        unsigned int    media1_vdec_qic_msg_idle : 1   ; /* [17] */
        // Comment of field: reserved_6
        /*
         * reserved
         */
        unsigned int    reserved_6            : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDEC_IDLE_ST;

/* Define the union U_VDEC_IDLE_BY */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: vdec_idle_bp
        /*
         * vdec modules idle bypass control:
         * bit0:  scd idle bypass;
         * bit1:  bsp idle bypass;
         * bit2:  pxpc idle bypass;
         * bit3:  pxphw idle bypass;
         * bit4:  cfg_qic idle bypass;
         * bit5:  core_cfg_qic idle bypass;
         * bit6:  core_data_qic idle bypass;
         * bit7:  tbu idle bypass;
         * bit8:  tbu dti rs idle bypass;
         * bit9:  media1_vdec_qic_msg idle bypass;
         * other：reserved
         */
        unsigned int    vdec_idle_bp          : 24  ; /* [23..0] */
        // Comment of field: mcu_idle_cfg
        /*
         * mcu idle state config，0：busy；1：idle；
         * note: if set 0, the VDEC enters the busy state and the frequency is increased;
         */
        unsigned int    mcu_idle_cfg          : 1   ; /* [24] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VDEC_IDLE_BY;

/* Define the union U_CORE_DATA_QIC_PUSH */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: core_data_qic_riscv_awpush
        /*
         * core data qic riscv写命令push，只使用bit0~2
         */
        unsigned int    core_data_qic_riscv_awpush : 4   ; /* [3..0] */
        // Comment of field: core_data_qic_riscv_arpush
        /*
         * core data qic riscv读命令push，只使用bit0~2
         */
        unsigned int    core_data_qic_riscv_arpush : 4   ; /* [7..4] */
        // Comment of field: core_data_qic_scd_awpush
        /*
         * core data qic scd写命令push，只使用bit0~2
         */
        unsigned int    core_data_qic_scd_awpush : 4   ; /* [11..8] */
        // Comment of field: core_data_qic_scd_arpush
        /*
         * core data qic scd读命令push，只使用bit0~2
         */
        unsigned int    core_data_qic_scd_arpush : 4   ; /* [15..12] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_CORE_DATA_QIC_PUSH;

/* Define the union U_MCU_NMI */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_soft_nmi
        /*
         * "1" active; 
         * not masked interrupt;
         */
        unsigned int    mcu_soft_nmi          : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 31  ; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_NMI;

/* Define the union U_MCU_HW_BUF_LEN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
         * 'core_sleep_mode  indicate that MCU is in sleep mode;
         */
        unsigned int    reserved_0            : 1   ; /* [0] */
        // Comment of field: reserved_1
        /*
         * 'core_debug_mode  indicate that MCU is in debug  mode;
         */
        unsigned int    reserved_1            : 1   ; /* [1] */
        // Comment of field: reserved_2
        /*
         * 'core_hard_fault_mode  indicate that MCU is in hard fault status;
         */
        unsigned int    reserved_2            : 1   ; /* [2] */
        // Comment of field: reserved_3
        /*
         * reserved
         */
        unsigned int    reserved_3            : 1   ; /* [3] */
        // Comment of field: vdec_mcu_dbgen
        /*
         * MCU DBG path enable signal
         */
        unsigned int    vdec_mcu_dbgen        : 1   ; /* [4] */
        // Comment of field: reserved_4
        /*
         * reserved
         */
        unsigned int    reserved_4            : 27  ; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_HW_BUF_LEN;

/* Define the union U_MCU_ADDR_HIGH */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_ddr_addr_high4
        /*
         * MCU访问DDR地址bit[35:32]
         */
        unsigned int    mcu_ddr_addr_high4    : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_ADDR_HIGH;

/* Define the union U_MCU_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_instr_fetch_en
        /*
         * Stall instruction fetching w hen this signal asserts during power on Core will boot after reset release when this signal desserts.
         * 1’b0 Fetching instruction.
         * 1’b1 Stall fetching instruction.
         */
        unsigned int    mcu_instr_fetch_en    : 1   ; /* [0] */
        // Comment of field: mcu_hart_id
        /*
         * Hart ID. Constant value. 保留，不使用，RV接口tie0
         * SW can read out this Hart ID when read the hardware thread ID CSR (mhartid).
         */
        unsigned int    mcu_hart_id           : 4   ; /* [4..1] */
        // Comment of field: mcu_dm_clk_off_n
        /*
         * The signals is used to turn off debug module clock.
         * 1’b0:Turn off debug module clock.
         * 1’b1:Turn on debug module clock.
         * Note: This signal is a static configuration signal and needs to be stable before the processor starts running. It is not allowed to be changed during the core running.
         * The signal is aimed to reduce the processor power consumption.
         */
        unsigned int    mcu_dm_clk_off_n      : 1   ; /* [5] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_CTRL0;

/* Define the union U_MCU_DBG_SIGNAL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mcu_sleep_mode
        /*
         * Core is in sleep mode when this signal asserts
         */
        unsigned int    mcu_sleep_mode        : 1   ; /* [0] */
        // Comment of field: mcu_debug_mode
        /*
         * Core is in debug mode when this signal asserts
         */
        unsigned int    mcu_debug_mode        : 1   ; /* [1] */
        // Comment of field: mcu_in_nmi_hdlr
        /*
         * Core is handling non maskable interrupt when this signal drives high.
         */
        unsigned int    mcu_in_nmi_hdlr       : 1   ; /* [2] */
        // Comment of field: mcu_in_async_excp_hdlr
        /*
         * Core is handling asynchronous exception when this signal drives
         * high.
         */
        unsigned int    mcu_in_async_excp_hdlr : 1   ; /* [3] */
        // Comment of field: mcu_ahbs_store_idle
        /*
         * Indicates the core’s AHBS port write buffer is idle, which can be used to indicate AHBS pre loading is complete.
         */
        unsigned int    mcu_ahbs_store_idle   : 1   ; /* [4] */
        // Comment of field: mcu_dcsr_stoptime
        /*
         * This signal indicates that whether timer stops counting when the
         * processor is in debug mode.
         * 0:Increment timers as usual.
         * 1:Don't increment any hart-local timers while in Debug Mode.
         * In LinxCore170, timer is defined out of the processor, so this control signal is represented on the top interface of the processor.
         */
        unsigned int    mcu_dcsr_stoptime     : 1   ; /* [5] */
        // Comment of field: mcu_in_double_excp_hdlr
        /*
         * This signal indicates that the processor encounter another exception when the processor is in exception handler while this signal drives high.
         * In general, this signal driving high indicates the processor encountering a fatal error.
         */
        unsigned int    mcu_in_double_excp_hdlr : 1   ; /* [6] */
        // Comment of field: mcu_top_dfx_cmt_vld0
        /*
         * Indicates the core is committing one instruction of dual issue instructions. The index “0” represents the oldest one.
         */
        unsigned int    mcu_top_dfx_cmt_vld0  : 1   ; /* [7] */
        // Comment of field: mcu_top_dfx_cmt_vld1
        /*
         * Indicates the core is committing one instruction of dual issue instructions. The index “1” represents the newest one.
         */
        unsigned int    mcu_top_dfx_cmt_vld1  : 1   ; /* [8] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 3   ; /* [11..9] */
        // Comment of field: mcu_dfx_cmt_gpr_vld0
        /*
         * Indicates that the corresponding general register file updating valid signal of one instruction of dual issue instructions is committing. The index “0” represents the oldest one.
         */
        unsigned int    mcu_dfx_cmt_gpr_vld0  : 1   ; /* [12] */
        // Comment of field: mcu_dfx_cmt_gpr_vld1
        /*
         * Indicates that the corresponding general register file updating valid signal of one instruction of dual issue instructions is committing. The index “1” represents the newest one.
         */
        unsigned int    mcu_dfx_cmt_gpr_vld1  : 1   ; /* [13] */
        // Comment of field: mcu_dfx_cmt_gpr_idx0
        /*
         * Indicates that the corresponding general register file index of one instruction of dual issue instructions is committing. The index “0”
         * represents the oldest one.
         */
        unsigned int    mcu_dfx_cmt_gpr_idx0  : 5   ; /* [18..14] */
        // Comment of field: mcu_dfx_cmt_gpr_idx1
        /*
         * Indicates that the corresponding general register file index of one instruction of dual issue instructions is committing. The index “1”
         * represents the newest one.
         */
        unsigned int    mcu_dfx_cmt_gpr_idx1  : 5   ; /* [23..19] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_DBG_SIGNAL0;

/* Define the union U_CORE_DFS_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: div_num0
        /*
         * BSP的自动降频分频比例，默认64分频
         */
        unsigned int    div_num0              : 6   ; /* [5..0] */
        // Comment of field: dfs_force_busy0
        /*
         * BSP的强制busy退降频，默认不使用
         */
        unsigned int    dfs_force_busy0       : 1   ; /* [6] */
        // Comment of field: dfs_en0
        /*
         * BSP的自动降频使能，默认不使能
         */
        unsigned int    dfs_en0               : 1   ; /* [7] */
        // Comment of field: dfs_wait_cnt0
        /*
         * BSP进入自动降频的延迟，默认64cycle
         */
        unsigned int    dfs_wait_cnt0         : 8   ; /* [15..8] */
        // Comment of field: div_num1
        /*
         * PXPC的自动降频分频比例，默认64分频
         */
        unsigned int    div_num1              : 6   ; /* [21..16] */
        // Comment of field: dfs_force_busy1
        /*
         * PXPC的自动降频分频比例，默认64分频
         */
        unsigned int    dfs_force_busy1       : 1   ; /* [22] */
        // Comment of field: dfs_en1
        /*
         * PXPC的自动降频使能，默认不使能
         */
        unsigned int    dfs_en1               : 1   ; /* [23] */
        // Comment of field: dfs_wait_cnt1
        /*
         * PXPC进入自动降频的延迟，默认64cycle
         */
        unsigned int    dfs_wait_cnt1         : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_CORE_DFS_CFG;

//==============================================================================
/* Define the global struct */
typedef struct {
    unsigned int           VDE_VERSION                      ; /* 0x0 */
    unsigned int           reserved_0[3]                    ; /* 0x4~0xc */
    U_MCU_CRG0             MCU_CRG0                         ; /* 0x10 */
    U_MCU_CRG1             MCU_CRG1                         ; /* 0x14 */
    U_VDEC_CRG             VDEC_CRG                         ; /* 0x18 */
    U_VDEC_ROOT_CRG        VDEC_ROOT_CRG                    ; /* 0x1c */
    U_MCU_RST0             MCU_RST0                         ; /* 0x20 */
    U_MCU_RST1             MCU_RST1                         ; /* 0x24 */
    U_VDEC_RST             VDEC_RST                         ; /* 0x28 */
    U_VDEC_ROOT_RST        VDEC_ROOT_RST                    ; /* 0x2c */
    U_VDE_CK_EN            VDE_CK_EN                        ; /* 0x30 */
    unsigned int           reserved_1[3]                    ; /* 0x34~0x3c */
    U_BSP_TASK_INDICATOR0_NS   BSP_TASK_INDICATOR0_NS           ; /* 0x40 */
    unsigned int           BSP_TASK_INDICATOR1_NS           ; /* 0x44 */
    U_PXPC_TASK_INDICATOR_NS   PXPC_TASK_INDICATOR_NS           ; /* 0x48 */
    U_NEW_TASK_NS          NEW_TASK_NS                      ; /* 0x4c */
    unsigned int           reserved_2[4]                    ; /* 0x50~0x5c */
    U_INTR_NS_PXPC_WR0     INTR_NS_PXPC_WR0                 ; /* 0x60 */
    U_INTR_NS_PXPC0        INTR_NS_PXPC0                    ; /* 0x64 */
    U_INTR_NS_PXPC1        INTR_NS_PXPC1                    ; /* 0x68 */
    unsigned int           reserved_3                       ; /* 0x6c */
    unsigned int           INTR_NS_BSP_WR0                  ; /* 0x70 */
    unsigned int           INTR_NS_BSP0                     ; /* 0x74 */
    unsigned int           INTR_NS_BSP1                     ; /* 0x78 */
    U_INTR_NS_BSP2         INTR_NS_BSP2                     ; /* 0x7c */
    U_INTR_NS_BSP3         INTR_NS_BSP3                     ; /* 0x80 */
    unsigned int           INTR_NS_VDEC                     ; /* 0x84 */
    unsigned int           INTR_NS_VDEC1                    ; /* 0x88 */
    unsigned int           reserved_4                       ; /* 0x8c */
    U_INTR_MASK_NS_PXPC0   INTR_MASK_NS_PXPC0               ; /* 0x90 */
    U_INTR_MASK_NS_PXPC1   INTR_MASK_NS_PXPC1               ; /* 0x94 */
    unsigned int           reserved_5[2]                    ; /* 0x98~0x9c */
    unsigned int           INTR_MASK_NS_BSP0                ; /* 0xa0 */
    unsigned int           INTR_MASK_NS_BSP1                ; /* 0xa4 */
    U_INTR_MASK_NS_BSP2    INTR_MASK_NS_BSP2                ; /* 0xa8 */
    U_INTR_MASK_NS_BSP3    INTR_MASK_NS_BSP3                ; /* 0xac */
    unsigned int           INTR_MASK_NS_VDEC                ; /* 0xb0 */
    unsigned int           INTR_MASK_NS_VDEC1               ; /* 0xb4 */
    unsigned int           reserved_6[2]                    ; /* 0xb8~0xbc */
    U_INTR_CLR_NS_PXPC0    INTR_CLR_NS_PXPC0                ; /* 0xc0 */
    unsigned int           reserved_7[3]                    ; /* 0xc4~0xcc */
    unsigned int           INTR_CLR_NS_BSP0                 ; /* 0xd0 */
    unsigned int           reserved_8[3]                    ; /* 0xd4~0xdc */
    unsigned int           INTR_CLR_NS_VDEC                 ; /* 0xe0 */
    U_INTR_SEL_NS_DSS_SYNC   INTR_SEL_NS_DSS_SYNC             ; /* 0xe4 */
    unsigned int           reserved_9[6]                    ; /* 0xe8~0xfc */
    U_INTR_CNT_STAT0_NS    INTR_CNT_STAT0_NS                ; /* 0x100 */
    U_INTR_CNT_STAT1_NS    INTR_CNT_STAT1_NS                ; /* 0x104 */
    unsigned int           reserved_10[2]                   ; /* 0x108~0x10c */
    U_INTR_CNT_RSV_NS      INTR_CNT_RSV_NS[8]               ; /* 0x110~0x12c */
    unsigned int           reserved_11[4]                   ; /* 0x130~0x13c */
    U_SCD_SOFTRST_CFG      SCD_SOFTRST_CFG                  ; /* 0x140 */
    unsigned int           SOFTRST_STA0                     ; /* 0x144 */
    unsigned int           SOFTRST_STA1                     ; /* 0x148 */
    unsigned int           reserved_12                      ; /* 0x14c */
    U_VDEC_IDLE_ST         VDEC_IDLE_ST                     ; /* 0x150 */
    unsigned int           CFG_QIC_NO_PENDING_RD            ; /* 0x154 */
    unsigned int           CFG_QIC_NO_PENDING_WR            ; /* 0x158 */
    unsigned int           CFG_QIC_NO_PENDING_RT            ; /* 0x15c */
    unsigned int           CFG_QIC_NO_PENDING_TB            ; /* 0x160 */
    unsigned int           CORE_CFG_QIC_NO_PENDING_RD       ; /* 0x164 */
    unsigned int           CORE_CFG_QIC_NO_PENDING_WR       ; /* 0x168 */
    unsigned int           CORE_CFG_QIC_NO_PENDING_RT       ; /* 0x16c */
    unsigned int           CORE_CFG_QIC_NO_PENDING_TB       ; /* 0x170 */
    unsigned int           CORE_DATA_QIC_NO_PENDING         ; /* 0x174 */
    unsigned int           reserved_13[6]                   ; /* 0x178~0x18c */
    U_VDEC_IDLE_BY         VDEC_IDLE_BY                     ; /* 0x190 */
    unsigned int           reserved_14[3]                   ; /* 0x194~0x19c */
    unsigned int           VDEC_SPMEM_CTRL_0                ; /* 0x1a0 */
    unsigned int           VDEC_TPMEM_CTRL_0                ; /* 0x1a4 */
    unsigned int           VDEC_BPMEM_CTRL_0                ; /* 0x1a8 */
    unsigned int           VDEC_SPMEM_CTRL_1                ; /* 0x1ac */
    unsigned int           reserved_15[20]                  ; /* 0x1b0~0x1fc */
    U_CORE_DATA_QIC_PUSH   CORE_DATA_QIC_PUSH               ; /* 0x200 */
    unsigned int           reserved_16[67]                  ; /* 0x204~0x30c */
    unsigned int           MCU_POR_PC                       ; /* 0x310 */
    unsigned int           reserved_17                      ; /* 0x314 */
    U_MCU_NMI              MCU_NMI                          ; /* 0x318 */
    unsigned int           reserved_18[9]                   ; /* 0x31c~0x33c */
    U_MCU_HW_BUF_LEN       MCU_HW_BUF_LEN                   ; /* 0x340 */
    U_MCU_ADDR_HIGH        MCU_ADDR_HIGH                    ; /* 0x344 */
    unsigned int           reserved_19[18]                  ; /* 0x348~0x38c */
    U_MCU_CTRL0            MCU_CTRL0                        ; /* 0x390 */
    unsigned int           MCU_INSTR_MEM_CTRL0_LOW          ; /* 0x394 */
    unsigned int           MCU_INSTR_MEM_CTRL0_HIGH         ; /* 0x398 */
    unsigned int           MCU_INSTR_MEM_CTRL1_LOW          ; /* 0x39c */
    unsigned int           MCU_INSTR_MEM_CTRL1_HIGH         ; /* 0x3a0 */
    unsigned int           MCU_INSTR_MEM_CTRL2_LOW          ; /* 0x3a4 */
    unsigned int           MCU_INSTR_MEM_CTRL2_HIGH         ; /* 0x3a8 */
    unsigned int           reserved_20[21]                  ; /* 0x3ac~0x3fc */
    U_MCU_DBG_SIGNAL0      MCU_DBG_SIGNAL0                  ; /* 0x400 */
    unsigned int           MCU_DFX_CMT_PC0                  ; /* 0x404 */
    unsigned int           MCU_DFX_CMT_PC1                  ; /* 0x408 */
    unsigned int           MCU_DFX_CMT_GPR_DATA0            ; /* 0x40c */
    unsigned int           MCU_DFX_CMT_GPR_DATA1            ; /* 0x410 */
    unsigned int           MCU_DFX_GPR_DATA_0_31[32]        ; /* 0x414~0x490 */
    unsigned int           reserved_21[27]                  ; /* 0x494~0x4fc */
    U_CORE_DFS_CFG         CORE_DFS_CFG                     ; /* 0x500 */
} S_VDE_SUB_CTRL_NS_REGS_TYPE;

//==============================================================================
/* Define the union of all registers */
typedef union {
    unsigned int    u32;

    U_MCU_CRG0             MCU_CRG0;
    U_MCU_CRG1             MCU_CRG1;
    U_VDEC_CRG             VDEC_CRG;
    U_VDEC_ROOT_CRG        VDEC_ROOT_CRG;
    U_MCU_RST0             MCU_RST0;
    U_MCU_RST1             MCU_RST1;
    U_VDEC_RST             VDEC_RST;
    U_VDEC_ROOT_RST        VDEC_ROOT_RST;
    U_VDE_CK_EN            VDE_CK_EN;
    U_BSP_TASK_INDICATOR0_NS BSP_TASK_INDICATOR0_NS;
    U_PXPC_TASK_INDICATOR_NS PXPC_TASK_INDICATOR_NS;
    U_NEW_TASK_NS          NEW_TASK_NS;
    U_INTR_NS_PXPC_WR0     INTR_NS_PXPC_WR0;
    U_INTR_NS_PXPC0        INTR_NS_PXPC0;
    U_INTR_NS_PXPC1        INTR_NS_PXPC1;
    U_INTR_NS_BSP2         INTR_NS_BSP2;
    U_INTR_NS_BSP3         INTR_NS_BSP3;
    U_INTR_MASK_NS_PXPC0   INTR_MASK_NS_PXPC0;
    U_INTR_MASK_NS_PXPC1   INTR_MASK_NS_PXPC1;
    U_INTR_MASK_NS_BSP2    INTR_MASK_NS_BSP2;
    U_INTR_MASK_NS_BSP3    INTR_MASK_NS_BSP3;
    U_INTR_CLR_NS_PXPC0    INTR_CLR_NS_PXPC0;
    U_INTR_SEL_NS_DSS_SYNC INTR_SEL_NS_DSS_SYNC;
    U_INTR_CNT_STAT0_NS    INTR_CNT_STAT0_NS;
    U_INTR_CNT_STAT1_NS    INTR_CNT_STAT1_NS;
    U_INTR_CNT_RSV_NS      INTR_CNT_RSV_NS;
    U_SCD_SOFTRST_CFG      SCD_SOFTRST_CFG;
    U_VDEC_IDLE_ST         VDEC_IDLE_ST;
    U_VDEC_IDLE_BY         VDEC_IDLE_BY;
    U_CORE_DATA_QIC_PUSH   CORE_DATA_QIC_PUSH;
    U_MCU_NMI              MCU_NMI;
    U_MCU_HW_BUF_LEN       MCU_HW_BUF_LEN;
    U_MCU_ADDR_HIGH        MCU_ADDR_HIGH;
    U_MCU_CTRL0            MCU_CTRL0;
    U_MCU_DBG_SIGNAL0      MCU_DBG_SIGNAL0;
    U_CORE_DFS_CFG         CORE_DFS_CFG;
} U_VDE_SUB_CTRL_NS_REGS_TYPE;

#endif /* __VDE_SUB_CTRL_NS_C_UNION_DEFINE_H__ */
