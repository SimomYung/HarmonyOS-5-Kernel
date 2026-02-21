/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  PXPC_TASK_QUEUE_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  wushengtao/yanmengting/jieguangwen
 * Version       :  1.0
 * Date          :  2022/3/1
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1
 * History       :  wushengtao/yanmengting/jieguangwen 2023/05/06 11:34:35 Create file
 */

#ifndef __PXPC_TASK_QUEUE_C_UNION_DEFINE_H__
#define __PXPC_TASK_QUEUE_C_UNION_DEFINE_H__

/* Define the union U_TASK_QUEUE_PARA1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_task_addr_hi
        /*
         * PXPC任务起始地址，DDR地址高4bit
         */
        unsigned int    pxpc_task_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: pxpc_task_lenth
        /*
         * PXPC任务长度（当前任务及所含子任务的总长度），16byte为单位，可支持4096个子任务。
         */
        unsigned int    pxpc_task_lenth       : 14  ; /* [17..4] */
        // Comment of field: pxpc_task_destroy_res_ptr_lo
        /*
         * 销毁任务需要回写的RES读指针低位。RES读指针代表RES循环Buffer读地址的bit[35:12]
         */
        unsigned int    pxpc_task_destroy_res_ptr_lo : 14  ; /* [31..18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA1;

/* Define the union U_TASK_QUEUE_PARA2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_task_destroy_res_ptr_hi
        /*
         * 销毁任务需要回写的RES读指针高位
         */
        unsigned int    pxpc_task_destroy_res_ptr_hi : 10  ; /* [9..0] */
        // Comment of field: pxpc_task_destroy_syntax_ptr_lo
        /*
         * 销毁任务需要回写的Syntax读指针低位。Syntax读指针代表Syntax循环Buffer读地址的bit[35:12]
         */
        unsigned int    pxpc_task_destroy_syntax_ptr_lo : 22  ; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA2;

/* Define the union U_TASK_QUEUE_PARA3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: pxpc_task_destroy_syntax_ptr_hi
        /*
         * 销毁任务需要回写的Syntax读指针高位
         */
        unsigned int    pxpc_task_destroy_syntax_ptr_hi : 2   ; /* [1..0] */
        // Comment of field: pxpc_task_destroy_bsp_num
        /*
         * 销毁任务需要回写的BSP序号
         */
        unsigned int    pxpc_task_destroy_bsp_num : 3   ; /* [4..2] */
        // Comment of field: pxpc_frm_num
        /*
         * 帧序号，0-63循环计数
         */
        unsigned int    pxpc_frm_num          : 6   ; /* [10..5] */
        // Comment of field: pxpc_task_first_ind
        /*
         * 帧内第一个任务指示，1代表帧内第一个任务，0代表非第一个任务
         */
        unsigned int    pxpc_task_first_ind   : 1   ; /* [11] */
        // Comment of field: pxpc_task_destroy_last_ind
        /*
         * 销毁任务为帧内最后一个任务指示，1代表帧内最后1个任务，0代表非帧内最后一个任务
         */
        unsigned int    pxpc_task_destroy_last_ind : 1   ; /* [12] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 17  ; /* [29..13] */
        // Comment of field: pxpc_task_exe_flag
        /*
         * 任务是否执行Flag，1代表任务未执行，0代表任务已执行；---RISC-V固定写1
         */
        unsigned int    pxpc_task_exe_flag    : 1   ; /* [30] */
        // Comment of field: pxpc_task_type
        /*
         * 任务类型，1为销毁任务，0为正常任务
         */
        unsigned int    pxpc_task_type        : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA3;

typedef struct {
    unsigned int           TASK_QUEUE_PARA0;
    U_TASK_QUEUE_PARA1     TASK_QUEUE_PARA1;
    U_TASK_QUEUE_PARA2     TASK_QUEUE_PARA2;
    U_TASK_QUEUE_PARA3     TASK_QUEUE_PARA3;
} S_PXPC_TASK_QUEUE_REGS_TASK_0_TYPE;

//==============================================================================
/* Define the global struct */
typedef struct {
    S_PXPC_TASK_QUEUE_REGS_TASK_0_TYPE   TASK_REG[8]                      ; /* 0x0~0x70 */
} S_PXPC_TASK_QUEUE_REGS_TYPE;

//==============================================================================
/* Define the union of all registers */
typedef union {
    unsigned int    u32;

    U_TASK_QUEUE_PARA1     TASK_QUEUE_PARA1;
    U_TASK_QUEUE_PARA2     TASK_QUEUE_PARA2;
    U_TASK_QUEUE_PARA3     TASK_QUEUE_PARA3;
} U_PXPC_TASK_QUEUE_REGS_TYPE;

#endif /* __PXPC_TASK_QUEUE_C_UNION_DEFINE_H__ */
