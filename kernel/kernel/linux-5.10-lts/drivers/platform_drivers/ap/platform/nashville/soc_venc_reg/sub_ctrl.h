// ******************************************************************************
// Copyright     :  Copyright (C) 2020, Hisilicon Technologies Co. Ltd.
// File name     :  sub_ctrl.h
// Project line  :
// Department    :  K5
// Author        :  AnthonySixta
// Version       :  1.0
// Date          :  2013/5/31
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2
// History       :  AnthonySixta 2020/06/02 09:29:01 Create file
// ******************************************************************************

#ifndef __SUB_CTRL_H__
#define __SUB_CTRL_H__

/* Define the union U_SUBCTRL_CRG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_por_rst           : 1   ; /* [0]  */
        unsigned int    mcu_wdt_rst           : 1   ; /* [1]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    mcu_core_wait         : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_CRG0;

/* Define the union U_SUBCTRL_CRG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_clk_en            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_CRG1;

/* Define the union U_SUBCTRL_CRG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0] */
        unsigned int    ipc_soft_rst          : 1   ; /* [1]  */
        unsigned int    tim_soft_rst          : 1   ; /* [2]  */
        unsigned int    uart_soft_rst         : 1   ; /* [3]  */
        unsigned int    reserved_1            : 12  ; /* [15..4] */
        unsigned int    vedu_core_soft_rst    : 1   ; /* [16]  */
        unsigned int    axi_core_soft_rst     : 1   ; /* [17]  */
        unsigned int    reserved_2            : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_CRG2;

/* Define the union U_SUBCTRL_CRG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0] */
        unsigned int    ipc_clk_en            : 1   ; /* [1]  */
        unsigned int    tim_clk_en            : 1   ; /* [2]  */
        unsigned int    uart_clk_en           : 1   ; /* [3]  */
        unsigned int    reserved_1            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_CRG3;

/* Define the union U_SUBCTRL_MCU_NMI */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_soft_nmi          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_MCU_NMI;

/* Define the union U_SUBCTRL_HW_BUF_LEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    core_sleep_mode       : 1   ; /* [0]  */
        unsigned int    core_debug_mode       : 1   ; /* [1]  */
        unsigned int    core_hard_fault_mode  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    venc_mcu_dbgen        : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_HW_BUF_LEN;

/* Define the union U_SUBCTRL_AHB_MATRIX_PRI */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ahb_mst_pri           : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    reserved_1            : 1   ; /* [8] */
        unsigned int    reserved_2            : 1   ; /* [9] */
        unsigned int    reserved_3            : 1   ; /* [10] */
        unsigned int    reserved_4            : 21  ; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SUBCTRL_AHB_MATRIX_PRI;

/* Define the union U_MCU_LCU_LATENCY_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_lcu_latency_cnt   : 10  ; /* [9..0] */
        unsigned int    reserved_0            : 22  ; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_MCU_LCU_LATENCY_CNT;
//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_SUBCTRL_CRG0         SUBCTRL_CRG0;
    volatile U_SUBCTRL_CRG1         SUBCTRL_CRG1;
    volatile U_SUBCTRL_CRG2         SUBCTRL_CRG2;
    volatile U_SUBCTRL_CRG3         SUBCTRL_CRG3;
    volatile unsigned int           SUBCTRL_MCU_POR_PC;
    volatile unsigned int           SUBCTRL_RESERVED_0;
    volatile U_SUBCTRL_MCU_NMI      SUBCTRL_MCU_NMI;
    volatile unsigned int           SUBCTRL_RESERVED_1[9];
    volatile U_SUBCTRL_HW_BUF_LEN   SUBCTRL_HW_BUF_LEN;
    volatile unsigned int           SUBCTRL_RESERVED_2[3];
    volatile U_SUBCTRL_AHB_MATRIX_PRI SUBCTRL_AHB_MATRIX_PRI;
    volatile unsigned int           reserved_3[7]                    ; /* 0x54~0x6c */
    volatile U_MCU_LCU_LATENCY_CNT   MCU_LCU_LATENCY_CNT              ; /* 0x70 */
    volatile unsigned int           reserved_4[995];
} S_VE_SUB_CTRL_REGS_TYPE;

#endif /* __SUB_CTRL_H__ */
