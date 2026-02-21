/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  BSPS_SC_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  wushengtao/yanmengting/jieguangwen
 * Version       :  1.0
 * Date          :  2022/3/1
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  wushengtao/yanmengting/jieguangwen 2023/11/14 14:14:10 Create file
 */

#ifndef __BSPS_SC_C_UNION_DEFINE_H__
#define __BSPS_SC_C_UNION_DEFINE_H__

/* Define the union U_SC_CTRL_EN0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
         * BSP系统外不可配置。
         */
        unsigned int    reserved_0            : 13  ; /* [12..0] */
        // Comment of field: acc0_pxpc_int_en
        /*
         * [13]：pxpc回读ctu info/res反推读指针后反推中断，即对该比特写'b1，安全模式
         * [14]：pxpc回读ctu info/res反推读指针后反推中断，即对该比特写'b1，非安全模式
         */
        unsigned int    acc0_pxpc_int_en      : 2   ; /* [14..13] */
        // Comment of field: reserved_1
        /*
         * BSP系统外不可配置。
         */
        unsigned int    reserved_1            : 17  ; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SC_CTRL_EN0;

/* Define the union U_SC_CTRL_EN1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
         * BSP系统外不可配置。
         */
        unsigned int    reserved_0            : 13  ; /* [12..0] */
        // Comment of field: acc1_pxpc_int_en
        /*
         * [13]：pxpc回读ctu info/res反推读指针后反推中断，即对该比特写'b1，安全模式
         * [14]：pxpc回读ctu info/res反推读指针后反推中断，即对该比特写'b1，非安全模式
         */
        unsigned int    acc1_pxpc_int_en      : 2   ; /* [14..13] */
        // Comment of field: reserved_1
        /*
         * BSP系统外不可配置。
         */
        unsigned int    reserved_1            : 17  ; /* [31..15] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SC_CTRL_EN1;

/* Define the union U_SC_CTRL_EN4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: riscv2bsp_cmd_acc0_nsf
        /*
         * 给BSP任务下发后的任务通知中断。非安全模式下。
         */
        unsigned int    riscv2bsp_cmd_acc0_nsf : 1   ; /* [0] */
        // Comment of field: riscv2bsp_cmd_acc1_nsf
        /*
         * 给BSP任务下发后的任务通知中断。非安全模式下。
         */
        unsigned int    riscv2bsp_cmd_acc1_nsf : 1   ; /* [1] */
        // Comment of field: riscv2bsp_cmd_acc2_nsf
        /*
         * 给BSP任务下发后的任务通知中断。非安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_cmd_acc2_nsf : 1   ; /* [2] */
        // Comment of field: riscv2bsp_cmd_acc3_nsf
        /*
         * 给BSP任务下发后的任务通知中断。非安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_cmd_acc3_nsf : 1   ; /* [3] */
        // Comment of field: riscv2bsp_flush_acc0_nsf
        /*
         * 通知BSP_ACC0进行清场的中断。非安全模式下。
         */
        unsigned int    riscv2bsp_flush_acc0_nsf : 1   ; /* [4] */
        // Comment of field: riscv2bsp_flush_acc1_nsf
        /*
         * 通知BSP_ACC1进行清场的中断。非安全模式下。
         */
        unsigned int    riscv2bsp_flush_acc1_nsf : 1   ; /* [5] */
        // Comment of field: riscv2bsp_flush_acc2_nsf
        /*
         * 通知BSP_ACC2进行清场的中断。非安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_flush_acc2_nsf : 1   ; /* [6] */
        // Comment of field: riscv2bsp_flush_acc3_nsf
        /*
         * 通知BSP_ACC3进行清场的中断。非安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_flush_acc3_nsf : 1   ; /* [7] */
        // Comment of field: reserved_0
        /*
         * BSP系统外不可配置。
         */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SC_CTRL_EN4;

/* Define the union U_SC_CTRL_EN5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: riscv2bsp_cmd_acc0_sf
        /*
         * 给BSP任务下发后的任务通知中断。安全模式下。
         */
        unsigned int    riscv2bsp_cmd_acc0_sf : 1   ; /* [0] */
        // Comment of field: riscv2bsp_cmd_acc1_sf
        /*
         * 给BSP任务下发后的任务通知中断。安全模式下。
         */
        unsigned int    riscv2bsp_cmd_acc1_sf : 1   ; /* [1] */
        // Comment of field: riscv2bsp_cmd_acc2_sf
        /*
         * 给BSP任务下发后的任务通知中断。安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_cmd_acc2_sf : 1   ; /* [2] */
        // Comment of field: riscv2bsp_cmd_acc3_sf
        /*
         * 给BSP任务下发后的任务通知中断。安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_cmd_acc3_sf : 1   ; /* [3] */
        // Comment of field: riscv2bsp_flush_acc0_sf
        /*
         * 通知BSP_ACC0进行清场的中断。安全模式下。
         */
        unsigned int    riscv2bsp_flush_acc0_sf : 1   ; /* [4] */
        // Comment of field: riscv2bsp_flush_acc1_sf
        /*
         * 通知BSP_ACC1进行清场的中断。安全模式下。
         */
        unsigned int    riscv2bsp_flush_acc1_sf : 1   ; /* [5] */
        // Comment of field: riscv2bsp_flush_acc2_sf
        /*
         * 通知BSP_ACC2进行清场的中断。安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_flush_acc2_sf : 1   ; /* [6] */
        // Comment of field: riscv2bsp_flush_acc3_sf
        /*
         * 通知BSP_ACC3进行清场的中断。安全模式下。（当前版本保留）
         */
        unsigned int    riscv2bsp_flush_acc3_sf : 1   ; /* [7] */
        // Comment of field: riscv2bsp_powerdown
        /*
         * 通知BSP进入下电流程的中断。安全模式下。
         */
        unsigned int    riscv2bsp_powerdown   : 1   ; /* [8] */
        // Comment of field: reserved_0
        /*
         * BSP系统外不可配置。
         */
        unsigned int    reserved_0            : 23  ; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SC_CTRL_EN5;

//==============================================================================
/* Define the global struct */
typedef struct {
    unsigned int           reserved_0[384]                  ; /* 0x0~0x5fc */
    U_SC_CTRL_EN0          SC_CTRL_EN0                      ; /* 0x600 */
    unsigned int           reserved_1[7]                    ; /* 0x604~0x61c */
    U_SC_CTRL_EN1          SC_CTRL_EN1                      ; /* 0x620 */
    unsigned int           reserved_2[7]                    ; /* 0x624~0x63c */
    unsigned int           SC_CTRL_EN2                      ; /* 0x640 */
    unsigned int           reserved_3[7]                    ; /* 0x644~0x65c */
    unsigned int           SC_CTRL_EN3                      ; /* 0x660 */
    unsigned int           reserved_4[7]                    ; /* 0x664~0x67c */
    U_SC_CTRL_EN4          SC_CTRL_EN4                      ; /* 0x680 */
    unsigned int           reserved_5[7]                    ; /* 0x684~0x69c */
    U_SC_CTRL_EN5          SC_CTRL_EN5                      ; /* 0x6a0 */
} S_BSPS_SC_REGS_TYPE;

//==============================================================================
/* Define the union of all registers */
typedef union {
    unsigned int    u32;

    U_SC_CTRL_EN0          SC_CTRL_EN0;
    U_SC_CTRL_EN1          SC_CTRL_EN1;
    U_SC_CTRL_EN4          SC_CTRL_EN4;
    U_SC_CTRL_EN5          SC_CTRL_EN5;
} U_BSPS_SC_REGS_TYPE;

#endif /* __BSPS_SC_C_UNION_DEFINE_H__ */
