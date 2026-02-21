/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  BSP_TASK_QUEUE_REG_NSF_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  wushengtao/yanmengting/jieguangwen
 * Version       :  1.0
 * Date          :  2022/3/1
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1
 * History       :  wushengtao/yanmengting/jieguangwen 2023/05/06 11:31:26 Create file
 */

#ifndef __BSP_TASK_QUEUE_REG_NSF_C_UNION_DEFINE_H__
#define __BSP_TASK_QUEUE_REG_NSF_C_UNION_DEFINE_H__

/* Define the union U_TASK_QUEUE_PARA0_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task0_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task0_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task0_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task0_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task0_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task0_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task0_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task0_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA0_H;

/* Define the union U_TASK_QUEUE_PARA1_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task_addr_hi      : 4   ; /* [3..0] */
        // Comment of field: bsp_task_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task_lenth        : 14  ; /* [17..4] */
        // Comment of field: bsp_task_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task_flag         : 1   ; /* [18] */
        // Comment of field: bsp_task_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task_frm_no       : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA1_H;

/* Define the union U_TASK_QUEUE_PARA2_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task2_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task2_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task2_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task2_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task2_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task2_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task2_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task2_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA2_H;

/* Define the union U_TASK_QUEUE_PARA3_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task3_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task3_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task3_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task3_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task3_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task3_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task3_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task3_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA3_H;

/* Define the union U_TASK_QUEUE_PARA4_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task4_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task4_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task4_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task4_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task4_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task4_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task4_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task4_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA4_H;

/* Define the union U_TASK_QUEUE_PARA5_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task5_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task5_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task5_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task5_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task5_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task5_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task5_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task5_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA5_H;

/* Define the union U_TASK_QUEUE_PARA6_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task6_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task6_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task6_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task6_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task6_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task6_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task6_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task6_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA6_H;

/* Define the union U_TASK_QUEUE_PARA7_H */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_task7_addr_hi
        /*
         * BSP任务起始地址，DDR地址高4bit
         */
        unsigned int    bsp_task7_addr_hi     : 4   ; /* [3..0] */
        // Comment of field: bsp_task7_lenth
        /*
         * BSP任务长度（当前任务及所含子任务的总长度），32byte为单位，支持配置0~1023个子任务。
         */
        unsigned int    bsp_task7_lenth       : 14  ; /* [17..4] */
        // Comment of field: bsp_task7_flag
        /*
         * BSP任务是否待执行标记；1：待执行；0：已执行
         */
        unsigned int    bsp_task7_flag        : 1   ; /* [18] */
        // Comment of field: bsp_task7_frm_no
        /*
         * BSP任务的帧序号，0~63循环，备用。
         */
        unsigned int    bsp_task7_frm_no      : 6   ; /* [24..19] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 7   ; /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_TASK_QUEUE_PARA7_H;

/* Define the union U_BSP_INIT_PARA1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_ringbuff_res_addr_hi
        /*
         * 非安全模式：res ringbuffer起始地址，DDR地址高4bit
         */
        unsigned int    bsp_ringbuff_res_addr_hi : 4   ; /* [3..0] */
        // Comment of field: bsp_ringbuff_res_lenth
        /*
         * 非安全模式：res ringbuffer长度，单位4Kbyte，配置0表示无效，支持（64M-1K）B空间
         */
        unsigned int    bsp_ringbuff_res_lenth : 16  ; /* [19..4] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 12  ; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_PARA1;

/* Define the union U_BSP_INIT_PARA3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_ringbuff_info_addr_hi
        /*
         * 非安全模式：info ringbuffer起始地址，DDR地址高4bit
         */
        unsigned int    bsp_ringbuff_info_addr_hi : 4   ; /* [3..0] */
        // Comment of field: bsp_ringbuff_info_lenth
        /*
         * 非安全模式：info ringbuffer长度，单位4Kbyte，配置0表示无效，支持（64M-1K）B空间
         */
        unsigned int    bsp_ringbuff_info_lenth : 16  ; /* [19..4] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 12  ; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_PARA3;

/* Define the union U_BSP_INIT_PARA4 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_ringbuff_info_afull_th
        /*
         * 非安全模式：info ringbuffer进入反压的阈值，1Kbyte为单位，配置0表示无效，支持64MB阈值大小。
         */
        unsigned int    bsp_ringbuff_info_afull_th : 16  ; /* [15..0] */
        // Comment of field: bsp_ringbuff_res_afull_th
        /*
         * 非安全模式：res ringbuffer进入反压的阈值，1Kbyte为单位，配置0表示无效，支持64MB阈值大小。
         */
        unsigned int    bsp_ringbuff_res_afull_th : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_PARA4;

/* Define the union U_BSP_INIT_PARA5 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_ringbuff_info_wrap_th
        /*
         * 非安全模式：info ringbuffer绕回阈值，4byte为单位，配置0表示无效，支持256KB阈值大小
         */
        unsigned int    bsp_ringbuff_info_wrap_th : 16  ; /* [15..0] */
        // Comment of field: bsp_ringbuff_res_wrap_th
        /*
         * 非安全模式：res ringbuffer绕回阈值，4byte为单位，配置0表示无效，支持256KB阈值大小
         */
        unsigned int    bsp_ringbuff_res_wrap_th : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_PARA5;

/* Define the union U_BSP_INIT_PARA6 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_ringbuff_info_aempty_th
        /*
         * 非安全模式：info ringbuffer退出反压的阈值，1Kbyte为单位，配置0表示无效，支持64MB阈值大小。
         */
        unsigned int    bsp_ringbuff_info_aempty_th : 16  ; /* [15..0] */
        // Comment of field: bsp_ringbuff_res_aempty_th
        /*
         * 非安全模式：res ringbuffer退出反压的阈值，1Kbyte为单位，配置0表示无效，支持64MB阈值大小。
         */
        unsigned int    bsp_ringbuff_res_aempty_th : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_PARA6;

/* Define the union U_BSP_INIT_PARA7 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: bsp_ringbuff_info_rptr_en_th
        /*
         * 非安全模式：info ringbuffer使能PXPC推送读指针的阈值，1Kbyte为单位，配置0表示无效，支持64MB阈值大小。
         */
        unsigned int    bsp_ringbuff_info_rptr_en_th : 16  ; /* [15..0] */
        // Comment of field: bsp_ringbuff_res_rptr_en_th
        /*
         * 非安全模式：res ringbuffer使能PXPC推送读指针的阈值，1Kbyte为单位，配置0表示无效，支持64MB阈值大小。
         */
        unsigned int    bsp_ringbuff_res_rptr_en_th : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_BSP_INIT_PARA7;

/* Define the union U_RPT_DATA0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: curr_proc_subtask_cnt
        /*
         * 当前执行的是task里的第几个subtask
         */
        unsigned int    curr_proc_subtask_cnt : 16  ; /* [15..0] */
        // Comment of field: curr_proc_ctu_cnt
        /*
         * 当前subtask正在解码第几个ctu
         */
        unsigned int    curr_proc_ctu_cnt     : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_RPT_DATA0;

/* Define the union U_RPT_DATA1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ctu_x_cur
        /*
         * 当前解码ctu的x坐标，单位像素
         */
        unsigned int    ctu_x_cur             : 16  ; /* [15..0] */
        // Comment of field: ctu_y_cur
        /*
         * 当前解码ctu的y坐标，单位像素
         */
        unsigned int    ctu_y_cur             : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_RPT_DATA1;

/* Define the union U_RPT_DATA2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: info_wptr_h
        /*
         * info上报写指针高4bit
         */
        unsigned int    info_wptr_h           : 4   ; /* [3..0] */
        // Comment of field: res_wptr_h
        /*
         * res上报写指针高4bit
         */
        unsigned int    res_wptr_h            : 4   ; /* [7..4] */
        // Comment of field: ctu_row_dec_done_cnt
        /*
         * 当前subtask已解码完成的ctu_row计数
         */
        unsigned int    ctu_row_dec_done_cnt  : 16  ; /* [23..8] */
        // Comment of field: reserved_0
        /*
         * reserved
         */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_RPT_DATA2;

typedef struct {
    unsigned int           TASK_QUEUE_PARA0_L;
    U_TASK_QUEUE_PARA0_H   TASK_QUEUE_PARA0_H;
    unsigned int           TASK_QUEUE_PARA1_L;
    U_TASK_QUEUE_PARA1_H   TASK_QUEUE_PARA1_H;
    unsigned int           TASK_QUEUE_PARA2_L;
    U_TASK_QUEUE_PARA2_H   TASK_QUEUE_PARA2_H;
    unsigned int           TASK_QUEUE_PARA3_L;
    U_TASK_QUEUE_PARA3_H   TASK_QUEUE_PARA3_H;
    unsigned int           TASK_QUEUE_PARA4_L;
    U_TASK_QUEUE_PARA4_H   TASK_QUEUE_PARA4_H;
    unsigned int           TASK_QUEUE_PARA5_L;
    U_TASK_QUEUE_PARA5_H   TASK_QUEUE_PARA5_H;
    unsigned int           TASK_QUEUE_PARA6_L;
    U_TASK_QUEUE_PARA6_H   TASK_QUEUE_PARA6_H;
    unsigned int           TASK_QUEUE_PARA7_L;
    U_TASK_QUEUE_PARA7_H   TASK_QUEUE_PARA7_H;
} S_BSP_TASK_QUEUE_REG_NSF_REGS_TASK_0_TYPE;

typedef struct {
    unsigned int           BSP_INIT_PARA0;
    U_BSP_INIT_PARA1       BSP_INIT_PARA1;
    unsigned int           BSP_INIT_PARA2;
    U_BSP_INIT_PARA3       BSP_INIT_PARA3;
    U_BSP_INIT_PARA4       BSP_INIT_PARA4;
    U_BSP_INIT_PARA5       BSP_INIT_PARA5;
    U_BSP_INIT_PARA6       BSP_INIT_PARA6;
    U_BSP_INIT_PARA7       BSP_INIT_PARA7;
    unsigned int           BSP_RENEW_WPTR0;
    unsigned int           BSP_RENEW_WPTR1;
    unsigned int           RSV2;
    unsigned int           RSV3;
    U_RPT_DATA0            RPT_DATA0;
    U_RPT_DATA1            RPT_DATA1;
    U_RPT_DATA2            RPT_DATA2;
    unsigned int           RPT_DATA3;
    unsigned int           RPT_DATA4;
    unsigned int           RPT_RSV0;
    unsigned int           RPT_RSV1;
    unsigned int           RPT_RSV2;
} S_BSP_TASK_QUEUE_REG_NSF_REGS_BSP_80_TYPE;

//==============================================================================
// 不要用自动生成的这个, 用下边手工生成的那个
/* Define the global struct  */
//typedef struct {
//    S_BSP_TASK_QUEUE_REG_NSF_REGS_TASK_0_TYPE   TASK_REG[4]                      ; /* 0x0~0x300 */
//    S_BSP_TASK_QUEUE_REG_NSF_REGS_BSP_80_TYPE   BSP_REG[4]                       ; /* 0x80~0x380 */
//} S_BSP_TASK_QUEUE_REG_NSF_REGS_TYPE;


//==============================================================================
/* Define the union of all registers */
typedef union {
    unsigned int    u32;

    U_TASK_QUEUE_PARA0_H   TASK_QUEUE_PARA0_H;
    U_TASK_QUEUE_PARA1_H   TASK_QUEUE_PARA1_H;
    U_TASK_QUEUE_PARA2_H   TASK_QUEUE_PARA2_H;
    U_TASK_QUEUE_PARA3_H   TASK_QUEUE_PARA3_H;
    U_TASK_QUEUE_PARA4_H   TASK_QUEUE_PARA4_H;
    U_TASK_QUEUE_PARA5_H   TASK_QUEUE_PARA5_H;
    U_TASK_QUEUE_PARA6_H   TASK_QUEUE_PARA6_H;
    U_TASK_QUEUE_PARA7_H   TASK_QUEUE_PARA7_H;
    U_BSP_INIT_PARA1       BSP_INIT_PARA1;
    U_BSP_INIT_PARA3       BSP_INIT_PARA3;
    U_BSP_INIT_PARA4       BSP_INIT_PARA4;
    U_BSP_INIT_PARA5       BSP_INIT_PARA5;
    U_BSP_INIT_PARA6       BSP_INIT_PARA6;
    U_BSP_INIT_PARA7       BSP_INIT_PARA7;
    U_RPT_DATA0            RPT_DATA0;
    U_RPT_DATA1            RPT_DATA1;
    U_RPT_DATA2            RPT_DATA2;
} U_BSP_TASK_QUEUE_REG_NSF_REGS_TYPE;

// 注意!! 自动生成的头文件有问题, 下边这一段是手工添加, 不要覆盖
typedef struct {
	unsigned int           TASK_QUEUE_PARA0_L;
	U_TASK_QUEUE_PARA0_H   TASK_QUEUE_PARA0_H;
} S_TASK_QUEUE_PARA;

typedef struct {
	volatile unsigned int    info_wptr_l_nsafe;
	volatile unsigned int    res_wptr_l_nsafe;
} S_RPT_PTR_NSAFE;

typedef struct {
	S_TASK_QUEUE_PARA 		TASK_QUEUE_PARA[8]; /* 0x00~0x3C */
	S_RPT_PTR_NSAFE  		RPT_PTR_NSAFE[8];   /* 0x40~0x7C */
	unsigned int           BSP_INIT_PARA0;		/* 0x80 */
	U_BSP_INIT_PARA1       BSP_INIT_PARA1;		/* 0x84 */
	unsigned int           BSP_INIT_PARA2;		/* 0x88 */
	U_BSP_INIT_PARA3       BSP_INIT_PARA3;		/* 0x8C */
	U_BSP_INIT_PARA4       BSP_INIT_PARA4;		/* 0x90 */
	U_BSP_INIT_PARA5       BSP_INIT_PARA5;		/* 0x94 */
	U_BSP_INIT_PARA6       BSP_INIT_PARA6;		/* 0x98 */
	U_BSP_INIT_PARA7       BSP_INIT_PARA7;		/* 0x9C */
    unsigned int           BSP_RENEW_WPTR0;
    unsigned int           BSP_RENEW_WPTR1;
	unsigned int           RSV2;
	unsigned int           RSV3;
	U_RPT_DATA0            RPT_DATA0;		/* 0x00B0*/
	U_RPT_DATA1            RPT_DATA1;		/* 0x00B4*/
	U_RPT_DATA2            RPT_DATA2;		/* 0x00B8*/
	unsigned int           RPT_DATA3;		/* 0x00BC*/
	unsigned int           RPT_DATA4;		/* 0x00C0*/
	unsigned int           RPT_RSV0;		/* 0x00C4*/
	unsigned int           RPT_RSV1;		/* 0x00C8*/
	unsigned int           RPT_RSV2;		/* 0x00CC*/
	unsigned int  			RSV4[12];
} S_ONE_BSP_TASK_QUEUE_REGS_TYPE;

typedef struct {
	S_ONE_BSP_TASK_QUEUE_REGS_TYPE BSP[4];
} S_BSP_TASK_QUEUE_REG_NSF_REGS_TYPE;

#endif /* __BSP_TASK_QUEUE_REG_NSF_C_UNION_DEFINE_H__ */
