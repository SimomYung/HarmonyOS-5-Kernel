/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  BSP_TOP_SCTRL_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  wushengtao/yanmengting/jieguangwen
 * Version       :  1.0
 * Date          :  2022/3/1
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  wushengtao/yanmengting/jieguangwen 2023/11/14 14:13:57 Create file
 */

#ifndef __BSP_TOP_SCTRL_C_UNION_DEFINE_H__
#define __BSP_TOP_SCTRL_C_UNION_DEFINE_H__

/* Define the union U_BSP_CRG_EN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_clk_en
        /*
         * bit0：BSP_ACC0的时钟使能。
         * bit1：BSP_ACC1的时钟使能。
         * bit2：BSP_ACC2的时钟使能。（当前版本保留）
         * bit3：BSP_ACC3的时钟使能。（当前版本保留）
         * Bit4：BSP_CORE_SUBSYS的时钟使能。
         * Bit5：BSP_CORE的时钟使能。
         * Bit6：BSP_DMA的时钟使能
         * bit[7]：保留。
         */
        unsigned int    bsp_clk_en            : 8   ; /* [7..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_CRG_EN;

/* Define the union U_BSP_CRG_DIS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_clk_dis
        /*
         * bit0：BSP_ACC0的时钟去使能。
         * bit1：BSP_ACC1的时钟去使能。
         * bit2：BSP_ACC2的时钟去使能。（当前版本保留）
         * bit3：BSP_ACC3的时钟去使能。（当前版本保留）
         * Bit4：BSP_CORE_SUBSYS的时钟去使能。
         * Bit5：BSP_CORE的时钟去使能。
         * Bit6：BSP_DMA的时钟去使能
         * bit[7]：保留。
         */
        unsigned int    bsp_clk_dis           : 8   ; /* [7..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_CRG_DIS;

/* Define the union U_BSP_CRG_ST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_clk_stat
        /*
         * bit0：BSP_ACC0的时钟状态。
         * bit1：BSP_ACC1的时钟状态。
         * bit2：BSP_ACC2的时钟状态。（当前版本保留）
         * bit3：BSP_ACC3的时钟状态。（当前版本保留）
         * Bit4：BSP_CORE_SUBSYS的时钟状态。
         * Bit5：BSP_CORE的时钟状态。
         * Bit6：BSP_DMA的时钟状态。
         * bit[7]：保留。
         */
        unsigned int    bsp_clk_stat          : 8   ; /* [7..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_CRG_ST;

/* Define the union U_BSP_RST_EN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_rst_en
        /*
         * bit0：BSP_ACC0的复位使能。
         * bit1：BSP_ACC1的复位使能。
         * bit2：BSP_ACC2的复位使能。（当前版本保留）
         * bit3：BSP_ACC3的复位使能。（当前版本保留）
         * Bit4：BSP_CORE_SUBSYS的复位使能。
         * Bit5：BSP_CORE的复位使能。
         * Bit6：BSP_DMA的复位使能。
         * bit[7]：保留。
         */
        unsigned int    bsp_rst_en            : 8   ; /* [7..0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_RST_EN;

/* Define the union U_BSP_RST_DIS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_rst_dis
        /*
         * bit0：BSP_ACC0的复位去使能。
         * bit1：BSP_ACC1的复位去使能。
         * bit2：BSP_ACC2的复位去使能。（当前版本保留）
         * bit3：BSP_ACC3的复位去使能。（当前版本保留）
         * Bit4：BSP_CORE_SUBSYS的复位去使能。
         * Bit5：BSP_CORE的复位去使能。
         * Bit6：BSP_DMA的复位去使能。
         * bit[7]：保留。
         */
        unsigned int    bsp_rst_dis           : 8   ; /* [7..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_RST_DIS;

/* Define the union U_BSP_RST_ST */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_rst_stat
        /*
         * bit0：BSP_ACC0的复位状态。
         * bit1：BSP_ACC1的复位状态。
         * bit2：BSP_ACC2的复位状态。（当前版本保留）
         * bit3：BSP_ACC3的复位状态。（当前版本保留）
         * Bit4：BSP_CORE_SUBSYS的复位状态。
         * Bit5：BSP_CORE的复位状态。
         * Bit6：BSP_DMA的复位状态。
         * bit[7]：保留。
         */
        unsigned int    bsp_rst_stat          : 8   ; /* [7..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_RST_ST;

/* Define the union U_BSP_CKG_BYPASS */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_acc0_ckg_bypass
        /*
         * acc0自动门控是否bypass。仅限静态配置。
         * 默认配置0.
         * 0:模块时钟自动门控功能有效，硬件可自动开关时钟 
         * 1:模块时钟自动门控功能无效，硬件不进行时钟控制
         */
        unsigned int    bsp_acc0_ckg_bypass   : 1   ; /* [0] */
        // Comment of field: bsp_acc1_ckg_bypass
        /*
         * acc1自动门控是否bypass。仅限静态配置。
         * 默认配置0.
         * 0:模块时钟自动门控功能有效，硬件可自动开关时钟 
         * 1:模块时钟自动门控功能无效，硬件不进行时钟控制
         */
        unsigned int    bsp_acc1_ckg_bypass   : 1   ; /* [1] */
        // Comment of field: bsp_acc2_ckg_bypass
        /*
         * acc2自动门控是否bypass。仅限静态配置。
         * 默认配置0.
         * 0:模块时钟自动门控功能有效，硬件可自动开关时钟 
         * 1:模块时钟自动门控功能无效，硬件不进行时钟控制
         */
        unsigned int    bsp_acc2_ckg_bypass   : 1   ; /* [2] */
        // Comment of field: bsp_acc3_ckg_bypass
        /*
         * acc3自动门控是否bypass。仅限静态配置。
         * 默认配置0.
         * 0:模块时钟自动门控功能有效，硬件可自动开关时钟 
         * 1:模块时钟自动门控功能无效，硬件不进行时钟控制
         */
        unsigned int    bsp_acc3_ckg_bypass   : 1   ; /* [3] */
        // Comment of field: hac2qsp_ckg_bypass
        /*
         * HAC2QSP自动门控是否bypass。仅限静态配置。
         * 默认配置0.
         * 0:模块时钟自动门控功能有效，硬件可自动开关时钟 
         * 1:模块时钟自动门控功能无效，硬件不进行时钟控制
         */
        unsigned int    hac2qsp_ckg_bypass    : 1   ; /* [4] */
        // Comment of field: bsp_lbus_ckg_bypass
        /*
         * BSP_LBUS自动门控是否bypass。仅限静态配置。
         * 默认配置0.
         * 0:模块时钟自动门控功能有效，硬件可自动开关时钟 
         * 1:模块时钟自动门控功能无效，硬件不进行时钟控制
         */
        unsigned int    bsp_lbus_ckg_bypass   : 1   ; /* [5] */
        // Comment of field: reserved_0
        /*
         * reserved，待补充
         */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_CKG_BYPASS;

/* Define the union U_BSP_INIT_MODE_2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_sw_init_addr_hi_sf
        /*
         * BSP动态镜像加载的起始地址，安全加载源地址的高4bit。BSP安全SW初始化值的DDR起始地址，高4bit
         */
        unsigned int    bsp_sw_init_addr_hi_sf : 4   ; /* [3..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 27  ; /* [30..4] */
        // Comment of field: reserved_1
        /*
         * 保留
         */
        unsigned int    reserved_1            : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_MODE_2;

/* Define the union U_BSP_CORE_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_halt_ena
        /*
         * BSP halt置位
         */
        unsigned int    bsp_halt_ena          : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 15  ; /* [15..1] */
        // Comment of field: bsp_halt_clr
        /*
         * BSP 解halt
         */
        unsigned int    bsp_halt_clr          : 1   ; /* [16] */
        // Comment of field: reserved_1
        /*
         * Reserved.
         */
        unsigned int    reserved_1            : 15  ; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_CORE_CFG;

/* Define the union U_BSP_CORE_STAT_0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_halt_sta
        /*
         * BSP_CORE外置halt投票结果
         */
        unsigned int    bsp_halt_sta          : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 1   ; /* [1] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 2   ; /* [3..2] */
        // Comment of field: bsp_wfi_sta
        /*
         * BSP_CORE状态指示
         */
        unsigned int    bsp_wfi_sta           : 1   ; /* [4] */
        // Comment of field: reserved_2
        /*
         * reserved
         */
        unsigned int    reserved_2            : 1   ; /* [5] */
        // Comment of field: reserved_3
        /*
         * reserved
         */
        unsigned int    reserved_3            : 2   ; /* [7..6] */
        // Comment of field: bsp_core_subsys_busy
        /*
         * BSP_CORE_SUBSYS总线桥工作状态指示:1.忙 0.空闲
         */
        unsigned int    bsp_core_subsys_busy  : 1   ; /* [8] */
        // Comment of field: reserved_4
        /*
         * reserved
         */
        unsigned int    reserved_4            : 1   ; /* [9] */
        // Comment of field: reserved_5
        /*
         * reserved
         */
        unsigned int    reserved_5            : 2   ; /* [11..10] */
        // Comment of field: reserved_6
        /*
         * reserved
         */
        unsigned int    reserved_6            : 1   ; /* [12] */
        // Comment of field: reserved_7
        /*
         * Reserved.
         */
        unsigned int    reserved_7            : 19  ; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_CORE_STAT_0;

/* Define the union U_BSP_NMI_INT */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_nmi_int_ena
        /*
         * 中断使能：BSP_NMI_INT使能
         */
        unsigned int    bsp_nmi_int_ena       : 1   ; /* [0] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 3   ; /* [3..1] */
        // Comment of field: bsp_nmi_int_clr
        /*
         * 清中断
         */
        unsigned int    bsp_nmi_int_clr       : 1   ; /* [4] */
        // Comment of field: reserved_1
        /*
         * reserved
         */
        unsigned int    reserved_1            : 3   ; /* [7..5] */
        // Comment of field: bsp_nmi_int_sta
        /*
         * BSP NMI中断状态
         */
        unsigned int    bsp_nmi_int_sta       : 1   ; /* [8] */
        unsigned int    reserved_2            : 23  ; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_NMI_INT;

/* Define the union U_BSP_INIT_MODE_0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_sw_init_addr_hi
        /*
         * BSP动态镜像加载的起始地址，非安全加载源地址的高4bit。BSP安全SW初始化值的DDR起始地址，高4bit
         */
        unsigned int    bsp_sw_init_addr_hi   : 4   ; /* [3..0] */
        // Comment of field: bsp_retention_en
        /*
         * BSP上电模式指示，默认为非retention上电
         * 0：BSP为非retention上电
         * 1：BSP为retention上电
         * 仅支持在BSP系统上电初始化阶段静态配置，不支持在工作过程中动态配置切换模式。
         */
        unsigned int    bsp_retention_en      : 1   ; /* [4] */
        // Comment of field: bsp_ringbuff_mode_flag
        /*
         * 残差和INFO使用RINGBUFFER存储标志；
         * 1：使用RINGBUFFER模式
         * 0：使用非RINGBUFFER模式。
         * 仅支持在BSP系统上电初始化阶段静态配置，不支持在工作过程中动态配置切换模式。
         */
        unsigned int    bsp_ringbuff_mode_flag : 1   ; /* [5] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_MODE_0;

//==============================================================================
/* Define the global struct */
typedef struct {
    U_BSP_CRG_EN           BSP_CRG_EN                       ; /* 0x0 */
    U_BSP_CRG_DIS          BSP_CRG_DIS                      ; /* 0x4 */
    unsigned int           BSP_CRG_2                        ; /* 0x8 */
    U_BSP_CRG_ST           BSP_CRG_ST                       ; /* 0xc */
    U_BSP_RST_EN           BSP_RST_EN                       ; /* 0x10 */
    U_BSP_RST_DIS          BSP_RST_DIS                      ; /* 0x14 */
    unsigned int           BSP_CRG_6                        ; /* 0x18 */
    U_BSP_RST_ST           BSP_RST_ST                       ; /* 0x1c */
    U_BSP_CKG_BYPASS       BSP_CKG_BYPASS                   ; /* 0x20 */
    unsigned int           reserved_0                       ; /* 0x24 */
    unsigned int           reserved_1[1014]                 ; /* 0x28~0xffc */
    U_BSP_CORE_CFG         BSP_CORE_CFG                     ; /* 0x1000 */
    U_BSP_CORE_STAT_0      BSP_CORE_STAT_0                  ; /* 0x1004 */
    unsigned int           BSP_CORE_STAT_1                  ; /* 0x1008 */
    U_BSP_NMI_INT          BSP_NMI_INT                      ; /* 0x100c */
    unsigned int           BSP_NMI_MASKW_REG                ; /* 0x1010 */
    unsigned int           SUBCTRL_IPC_ADDR_0               ; /* 0x1014 */
    unsigned int           SUBCTRL_IPC_ADDR_1               ; /* 0x1018 */
    unsigned int           reserved_2                       ; /* 0x101c */
    U_BSP_INIT_MODE_0      BSP_INIT_MODE_0                  ; /* 0x1020 */
    unsigned int           BSP_INIT_MODE_1                  ; /* 0x1024 */
    U_BSP_INIT_MODE_2      BSP_INIT_MODE_2                  ; /* 0x1028 */
    unsigned int           BSP_INIT_MODE_3                  ; /* 0x102c */
} S_BSP_TOP_SCTRL_REGS_TYPE;

//==============================================================================
/* Define the union of all registers */
typedef union {
    unsigned int    u32;

    U_BSP_CRG_EN           BSP_CRG_EN;
    U_BSP_CRG_DIS          BSP_CRG_DIS;
    U_BSP_CRG_ST           BSP_CRG_ST;
    U_BSP_RST_EN           BSP_RST_EN;
    U_BSP_RST_DIS          BSP_RST_DIS;
    U_BSP_RST_ST           BSP_RST_ST;
    U_BSP_CKG_BYPASS       BSP_CKG_BYPASS;
    U_BSP_INIT_MODE_2      BSP_INIT_MODE_2;
    U_BSP_CORE_CFG         BSP_CORE_CFG;
    U_BSP_CORE_STAT_0      BSP_CORE_STAT_0;
    U_BSP_NMI_INT          BSP_NMI_INT;
    U_BSP_INIT_MODE_0      BSP_INIT_MODE_0;
} U_BSP_TOP_SCTRL_REGS_TYPE;

#endif /* __BSP_TOP_SCTRL_C_UNION_DEFINE_H__ */
