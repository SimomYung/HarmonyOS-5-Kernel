

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_HTS_INTERFACE_H__
#define __SOC_HTS_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/6) GLB_REG
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: EVENT_OUT对外发出的hts_src_id寄存器
   Bit domain definition UNION:  SOC_HTS_SRC_ID_UNION */
#define SOC_HTS_SRC_ID_ADDR(base)                             (base)

/* Register description: EVENT_OUT对外发出的hts_uid寄存器
   Bit domain definition UNION:  SOC_HTS_UID_UNION */
#define SOC_HTS_UID_ADDR(base)                                ((base) + 0x004UL)

/* Register description: 缓存store特性bypass控制寄存器
   Bit domain definition UNION:  SOC_HTS_STORE_BYPASS_MODE_UNION */
#define SOC_HTS_STORE_BYPASS_MODE_ADDR(base)                  ((base) + 0x010UL)

/* Register description: EVENT_IN的check table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_ADDR(base)            ((base) + 0x070UL)

/* Register description: EVENT_IN的IBUF的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_ADDR(base)             ((base) + 0x074UL)

/* Register description: EVENT_PROC的mapping table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_ADDR(base)        ((base) + 0x078UL)

/* Register description: EVENT_PROC的rule table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_ADDR(base)           ((base) + 0x07cUL)

/* Register description: EVENT_PROC的status table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_ADDR(base)         ((base) + 0x080UL)

/* Register description: EVENT_OUT的OBUF的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_ADDR(base)            ((base) + 0x084UL)

/* Register description: 异常处理封锁event的lock控制寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_LOCK_UNION */
#define SOC_HTS_EVENT_LOCK_ADDR(base, m)                      (base) + 0x100 + 0x004 * (m)

/* Register description: 异常处理封锁IBUF中event cnt的clear控制寄存器
   Bit domain definition UNION:  SOC_HTS_IBUF_EVENT_CNT_CLR_UNION */
#define SOC_HTS_IBUF_EVENT_CNT_CLR_ADDR(base, n)              (base) + 0x200 + 0x004 * (n)

/* Register description: HTS中断指示寄存器
   Bit domain definition UNION:  SOC_HTS_INT_UNION */
#define SOC_HTS_INT_ADDR(base)                                ((base) + 0x300UL)

/* Register description: HTS中断屏蔽寄存器
   Bit domain definition UNION:  SOC_HTS_INT_MASK_UNION */
#define SOC_HTS_INT_MASK_ADDR(base)                           ((base) + 0x304UL)

/* Register description: HTS DBG回读信息及cnt使能寄存器
   Bit domain definition UNION:  SOC_HTS_DBG_EN_UNION */
#define SOC_HTS_DBG_EN_ADDR(base)                             ((base) + 0x400UL)

/* Register description: EVENT_IN的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_0_ADDR(base)               ((base) + 0x410UL)

/* Register description: EVENT_IN的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_1_ADDR(base)               ((base) + 0x414UL)

/* Register description: EVENT_IN的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_2_ADDR(base)               ((base) + 0x418UL)

/* Register description: EVENT_IN的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_3_ADDR(base)               ((base) + 0x41cUL)

/* Register description: EVENT_IN的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_4_ADDR(base)               ((base) + 0x420UL)

/* Register description: EVENT_IN的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_5_ADDR(base)               ((base) + 0x424UL)

/* Register description: EVENT_PROC MAPPING的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_ADDR(base)     ((base) + 0x430UL)

/* Register description: EVENT_PROC MAPPING的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_ADDR(base)     ((base) + 0x434UL)

/* Register description: EVENT_PROC FETCH的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_ADDR(base)       ((base) + 0x440UL)

/* Register description: EVENT_PROC FETCH的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_ADDR(base)       ((base) + 0x444UL)

/* Register description: EVENT_PROC FETCH的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_ADDR(base)       ((base) + 0x448UL)

/* Register description: EVENT_PROC PROCESS的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_ADDR(base)     ((base) + 0x450UL)

/* Register description: EVENT_PROC PROCESS的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_ADDR(base)     ((base) + 0x454UL)

/* Register description: EVENT_PROC PROCESS的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_ADDR(base)     ((base) + 0x458UL)

/* Register description: EVENT_PROC PROCESS的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_ADDR(base)     ((base) + 0x45cUL)

/* Register description: EVENT_PROC PROCESS的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_ADDR(base)     ((base) + 0x460UL)

/* Register description: EVENT_PROC PROCESS的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_ADDR(base)     ((base) + 0x464UL)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_ADDR(base)     ((base) + 0x470UL)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_ADDR(base)     ((base) + 0x474UL)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_ADDR(base)     ((base) + 0x478UL)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_ADDR(base)     ((base) + 0x47cUL)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_ADDR(base)     ((base) + 0x480UL)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_ADDR(base)     ((base) + 0x484UL)

/* Register description: EVENT_OUT_OBUF的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_ADDR(base)         ((base) + 0x490UL)

/* Register description: EVENT_OUT_OBUF的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_ADDR(base)         ((base) + 0x494UL)

/* Register description: EVENT_OUT_OBUF的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_ADDR(base)         ((base) + 0x49cUL)

/* Register description: EVENT_OUT_OBUF的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_ADDR(base)         ((base) + 0x4a0UL)

/* Register description: EVENT_OUT_OBUF的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_ADDR(base)         ((base) + 0x4a4UL)

/* Register description: EVENT_OUT_OBUF的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_ADDR(base)         ((base) + 0x4a8UL)

/* Register description: EVENT_OUT_OBUF的回读寄存器6
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_ADDR(base)         ((base) + 0x4acUL)

/* Register description: EVENT_OUT_OBUF的回读寄存器7
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_ADDR(base)         ((base) + 0x4b0UL)

/* Register description: EVENT_IN的异常状态回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_ADDR(base)         ((base) + 0x500UL)

/* Register description: EVENT_IN的异常状态回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_ADDR(base)         ((base) + 0x504UL)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_ADDR(base) ((base) + 0x510UL)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_ADDR(base) ((base) + 0x514UL)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_ADDR(base) ((base) + 0x518UL)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_ADDR(base) ((base) + 0x51cUL)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_ADDR(base) ((base) + 0x520UL)

/* Register description: EVENT_OUT的异常状态回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_ADDR(base)        ((base) + 0x524UL)

/* Register description: 缓存event的store控制寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_STORE_UNION */
#define SOC_HTS_EVENT_STORE_ADDR(base, e)                     (base) + 0x600 + 0x004 * (e)

/* Register description: OBUF中event cnt的clear控制寄存器
   Bit domain definition UNION:  SOC_HTS_OBUF_EVENT_CNT_CLR_UNION */
#define SOC_HTS_OBUF_EVENT_CNT_CLR_ADDR(base, f)              (base) + 0x700 + 0x004 * (f)


#else


/* Register description: EVENT_OUT对外发出的hts_src_id寄存器
   Bit domain definition UNION:  SOC_HTS_SRC_ID_UNION */
#define SOC_HTS_SRC_ID_ADDR(base)                             (base)

/* Register description: EVENT_OUT对外发出的hts_uid寄存器
   Bit domain definition UNION:  SOC_HTS_UID_UNION */
#define SOC_HTS_UID_ADDR(base)                                ((base) + 0x004)

/* Register description: 缓存store特性bypass控制寄存器
   Bit domain definition UNION:  SOC_HTS_STORE_BYPASS_MODE_UNION */
#define SOC_HTS_STORE_BYPASS_MODE_ADDR(base)                  ((base) + 0x010)

/* Register description: EVENT_IN的check table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_ADDR(base)            ((base) + 0x070)

/* Register description: EVENT_IN的IBUF的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_ADDR(base)             ((base) + 0x074)

/* Register description: EVENT_PROC的mapping table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_ADDR(base)        ((base) + 0x078)

/* Register description: EVENT_PROC的rule table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_ADDR(base)           ((base) + 0x07c)

/* Register description: EVENT_PROC的status table的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_ADDR(base)         ((base) + 0x080)

/* Register description: EVENT_OUT的OBUF的mem_ctrl寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_ADDR(base)            ((base) + 0x084)

/* Register description: 异常处理封锁event的lock控制寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_LOCK_UNION */
#define SOC_HTS_EVENT_LOCK_ADDR(base, m)                      (base) + 0x100 + 0x004 * (m)

/* Register description: 异常处理封锁IBUF中event cnt的clear控制寄存器
   Bit domain definition UNION:  SOC_HTS_IBUF_EVENT_CNT_CLR_UNION */
#define SOC_HTS_IBUF_EVENT_CNT_CLR_ADDR(base, n)              (base) + 0x200 + 0x004 * (n)

/* Register description: HTS中断指示寄存器
   Bit domain definition UNION:  SOC_HTS_INT_UNION */
#define SOC_HTS_INT_ADDR(base)                                ((base) + 0x300)

/* Register description: HTS中断屏蔽寄存器
   Bit domain definition UNION:  SOC_HTS_INT_MASK_UNION */
#define SOC_HTS_INT_MASK_ADDR(base)                           ((base) + 0x304)

/* Register description: HTS DBG回读信息及cnt使能寄存器
   Bit domain definition UNION:  SOC_HTS_DBG_EN_UNION */
#define SOC_HTS_DBG_EN_ADDR(base)                             ((base) + 0x400)

/* Register description: EVENT_IN的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_0_ADDR(base)               ((base) + 0x410)

/* Register description: EVENT_IN的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_1_ADDR(base)               ((base) + 0x414)

/* Register description: EVENT_IN的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_2_ADDR(base)               ((base) + 0x418)

/* Register description: EVENT_IN的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_3_ADDR(base)               ((base) + 0x41c)

/* Register description: EVENT_IN的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_4_ADDR(base)               ((base) + 0x420)

/* Register description: EVENT_IN的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_IN_READ_BACK_5_ADDR(base)               ((base) + 0x424)

/* Register description: EVENT_PROC MAPPING的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_ADDR(base)     ((base) + 0x430)

/* Register description: EVENT_PROC MAPPING的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_ADDR(base)     ((base) + 0x434)

/* Register description: EVENT_PROC FETCH的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_ADDR(base)       ((base) + 0x440)

/* Register description: EVENT_PROC FETCH的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_ADDR(base)       ((base) + 0x444)

/* Register description: EVENT_PROC FETCH的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_ADDR(base)       ((base) + 0x448)

/* Register description: EVENT_PROC PROCESS的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_ADDR(base)     ((base) + 0x450)

/* Register description: EVENT_PROC PROCESS的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_ADDR(base)     ((base) + 0x454)

/* Register description: EVENT_PROC PROCESS的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_ADDR(base)     ((base) + 0x458)

/* Register description: EVENT_PROC PROCESS的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_ADDR(base)     ((base) + 0x45c)

/* Register description: EVENT_PROC PROCESS的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_ADDR(base)     ((base) + 0x460)

/* Register description: EVENT_PROC PROCESS的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_ADDR(base)     ((base) + 0x464)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_ADDR(base)     ((base) + 0x470)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_ADDR(base)     ((base) + 0x474)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_ADDR(base)     ((base) + 0x478)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_ADDR(base)     ((base) + 0x47c)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_ADDR(base)     ((base) + 0x480)

/* Register description: EVENT_OUT_DISPATCH的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_ADDR(base)     ((base) + 0x484)

/* Register description: EVENT_OUT_OBUF的回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_ADDR(base)         ((base) + 0x490)

/* Register description: EVENT_OUT_OBUF的回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_ADDR(base)         ((base) + 0x494)

/* Register description: EVENT_OUT_OBUF的回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_ADDR(base)         ((base) + 0x49c)

/* Register description: EVENT_OUT_OBUF的回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_ADDR(base)         ((base) + 0x4a0)

/* Register description: EVENT_OUT_OBUF的回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_ADDR(base)         ((base) + 0x4a4)

/* Register description: EVENT_OUT_OBUF的回读寄存器5
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_ADDR(base)         ((base) + 0x4a8)

/* Register description: EVENT_OUT_OBUF的回读寄存器6
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_ADDR(base)         ((base) + 0x4ac)

/* Register description: EVENT_OUT_OBUF的回读寄存器7
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_UNION */
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_ADDR(base)         ((base) + 0x4b0)

/* Register description: EVENT_IN的异常状态回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_ADDR(base)         ((base) + 0x500)

/* Register description: EVENT_IN的异常状态回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_ADDR(base)         ((base) + 0x504)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_ADDR(base) ((base) + 0x510)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器1
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_ADDR(base) ((base) + 0x514)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器2
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_ADDR(base) ((base) + 0x518)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器3
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_ADDR(base) ((base) + 0x51c)

/* Register description: EVENT_PROC的PROCESS异常状态回读寄存器4
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_UNION */
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_ADDR(base) ((base) + 0x520)

/* Register description: EVENT_OUT的异常状态回读寄存器0
   Bit domain definition UNION:  SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_UNION */
#define SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_ADDR(base)        ((base) + 0x524)

/* Register description: 缓存event的store控制寄存器
   Bit domain definition UNION:  SOC_HTS_EVENT_STORE_UNION */
#define SOC_HTS_EVENT_STORE_ADDR(base, e)                     (base) + 0x600 + 0x004 * (e)

/* Register description: OBUF中event cnt的clear控制寄存器
   Bit domain definition UNION:  SOC_HTS_OBUF_EVENT_CNT_CLR_UNION */
#define SOC_HTS_OBUF_EVENT_CNT_CLR_ADDR(base, f)              (base) + 0x700 + 0x004 * (f)


#endif


/****************************************************************************
                     (2/6) CHECK_TABLE
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: EVENT_IN_CHECK的event_i到uid的映射表
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_CHECK_UNION */
#define SOC_HTS_EVENT_IN_CHECK_ADDR(base, p)                  (base) + 0x0004 * (p)


#else


/* Register description: EVENT_IN_CHECK的event_i到uid的映射表
   Bit domain definition UNION:  SOC_HTS_EVENT_IN_CHECK_UNION */
#define SOC_HTS_EVENT_IN_CHECK_ADDR(base, p)                  (base) + 0x0004 * (p)


#endif


/****************************************************************************
                     (3/6) MAPPING_TABLE
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: EVENT_PROC_MAPPING的event_i到rule_addr的映射表
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_IDX_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_IDX_ADDR(base, i)          (base) + 0x0004 * (i)


#else


/* Register description: EVENT_PROC_MAPPING的event_i到rule_addr的映射表
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_MAPPING_IDX_UNION */
#define SOC_HTS_EVENT_PROC_MAPPING_IDX_ADDR(base, i)          (base) + 0x0004 * (i)


#endif


/****************************************************************************
                     (4/6) RULE_TABLE
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: EVENT_PROC_MAPPING的event到rule_addr的映射表
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_RULE_UNION */
#define SOC_HTS_EVENT_PROC_RULE_ADDR(base, j)                 (base) + 0x0004 * (j)


#else


/* Register description: EVENT_PROC_MAPPING的event到rule_addr的映射表
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_RULE_UNION */
#define SOC_HTS_EVENT_PROC_RULE_ADDR(base, j)                 (base) + 0x0004 * (j)


#endif


/****************************************************************************
                     (5/6) STATUS_TABLE
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: EVENT_PROC的rule_status回读
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_STATUS_UNION */
#define SOC_HTS_EVENT_PROC_STATUS_ADDR(base, k)               (base) + 0x0004 * (k)


#else


/* Register description: EVENT_PROC的rule_status回读
   Bit domain definition UNION:  SOC_HTS_EVENT_PROC_STATUS_UNION */
#define SOC_HTS_EVENT_PROC_STATUS_ADDR(base, k)               (base) + 0x0004 * (k)


#endif


/****************************************************************************
                     (6/6) HTS_WRAP
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: HTS_WRAP发出outstanding的维测回读寄存器0
   Bit domain definition UNION:  SOC_HTS_WRAP_OST_CNT_0_UNION */
#define SOC_HTS_WRAP_OST_CNT_0_ADDR(base)                     (base)

/* Register description: HTS及HTS_WRAP的idle的使能控制
   Bit domain definition UNION:  SOC_HTS_IDLE_EN_UNION */
#define SOC_HTS_IDLE_EN_ADDR(base)                            ((base) + 0x010UL)

/* Register description: HTS及HTS_WRAP的idle状态回读
   Bit domain definition UNION:  SOC_HTS_IDLE_READ_BACK_UNION */
#define SOC_HTS_IDLE_READ_BACK_ADDR(base)                     ((base) + 0x014UL)

/* Register description: HTS_WRAP的总线不反压计数器启动的请求寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_ADDR(base)  ((base) + 0x100UL)

/* Register description: HTS_WRAP的总线不反压计数器完成的状态寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_ADDR(base)  ((base) + 0x104UL)

/* Register description: HTS_WRAP的总线不反压计数器的预期计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_ADDR(base) ((base) + 0x108UL)

/* Register description: HTS_WRAP的总线不反压计数器的实际计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ADDR(base)      ((base) + 0x10cUL)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器启动的请求寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_ADDR(base)  ((base) + 0x200UL)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器完成的状态寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_ADDR(base)  ((base) + 0x204UL)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器的预期计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_ADDR(base) ((base) + 0x208UL)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器的实际计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ADDR(base)      ((base) + 0x20cUL)


#else


/* Register description: HTS_WRAP发出outstanding的维测回读寄存器0
   Bit domain definition UNION:  SOC_HTS_WRAP_OST_CNT_0_UNION */
#define SOC_HTS_WRAP_OST_CNT_0_ADDR(base)                     (base)

/* Register description: HTS及HTS_WRAP的idle的使能控制
   Bit domain definition UNION:  SOC_HTS_IDLE_EN_UNION */
#define SOC_HTS_IDLE_EN_ADDR(base)                            ((base) + 0x010)

/* Register description: HTS及HTS_WRAP的idle状态回读
   Bit domain definition UNION:  SOC_HTS_IDLE_READ_BACK_UNION */
#define SOC_HTS_IDLE_READ_BACK_ADDR(base)                     ((base) + 0x014)

/* Register description: HTS_WRAP的总线不反压计数器启动的请求寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_ADDR(base)  ((base) + 0x100)

/* Register description: HTS_WRAP的总线不反压计数器完成的状态寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_ADDR(base)  ((base) + 0x104)

/* Register description: HTS_WRAP的总线不反压计数器的预期计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_ADDR(base) ((base) + 0x108)

/* Register description: HTS_WRAP的总线不反压计数器的实际计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_UNION */
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ADDR(base)      ((base) + 0x10c)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器启动的请求寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_ADDR(base)  ((base) + 0x200)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器完成的状态寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_ADDR(base)  ((base) + 0x204)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器的预期计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_ADDR(base) ((base) + 0x208)

/* Register description: HTS_WRAP缓存功能的总线不反压计数器的实际计数值寄存器
   Bit domain definition UNION:  SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_UNION */
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ADDR(base)      ((base) + 0x20c)


#endif




/*****************************************************************************
  3 枚举定义
*****************************************************************************/



/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/



/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/



/*****************************************************************************
  7 UNION定义
*****************************************************************************/

/****************************************************************************
                     (1/6) GLB_REG
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_HTS_SRC_ID_UNION
 struct description   : SRC_ID Register structure definition
                        Address Offset:0x000 Initial:0x0000 Width:32
 register description : EVENT_OUT对外发出的hts_src_id寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_src_id : 6;  /* bit[0-5]  : EVENT_OUT对外发出的hts_src_id寄存器 */
        unsigned int  reserved_0 : 10; /* bit[6-15] : reserved */
        unsigned int  reserved_1 : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_SRC_ID_UNION;
#endif
#define SOC_HTS_SRC_ID_hts_src_id_START  (0)
#define SOC_HTS_SRC_ID_hts_src_id_END    (5)


/*****************************************************************************
 struct               : SOC_HTS_UID_UNION
 struct description   : UID Register structure definition
                        Address Offset:0x004 Initial:0x0000 Width:32
 register description : EVENT_OUT对外发出的hts_uid寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_uid  : 12; /* bit[0-11] : EVENT_OUT对外发出的hts_uid寄存器 */
        unsigned int  reserved_0: 4;  /* bit[12-15]: reserved */
        unsigned int  reserved_1: 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_UID_UNION;
#endif
#define SOC_HTS_UID_hts_uid_START   (0)
#define SOC_HTS_UID_hts_uid_END     (11)


/*****************************************************************************
 struct               : SOC_HTS_STORE_BYPASS_MODE_UNION
 struct description   : STORE_BYPASS_MODE Register structure definition
                        Address Offset:0x010 Initial:0x0000 Width:32
 register description : 缓存store特性bypass控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_store_bypass_mode : 1;  /* bit[0]    : 缓存store特性bypass控制寄存器（输出缓存CCB）
                                                                 1：bypass输出缓存特性
                                                                 0：enable输出缓存特性 */
        unsigned int  reserved_0            : 15; /* bit[1-15] : reserved */
        unsigned int  reserved_1            : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_STORE_BYPASS_MODE_UNION;
#endif
#define SOC_HTS_STORE_BYPASS_MODE_hts_store_bypass_mode_START  (0)
#define SOC_HTS_STORE_BYPASS_MODE_hts_store_bypass_mode_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_UNION
 struct description   : EVENT_IN_CHECK_MEM_CTRL Register structure definition
                        Address Offset:0x070 Initial:0x00000850 Width:32
 register description : EVENT_IN的check table的mem_ctrl寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mem_ctrl_event_in_check : 32; /* bit[0-31]: mem_ctrl_event_in_check
                                                                  bit[0]：LS，Light sleep mode,active high
                                                                  bit[1]：DS，Deep sleep mode,active high
                                                                  bit[2]：SD reserved，mem_sd由外部统一控制
                                                                  bit[3]：reserved
                                                                  bit[5:4]：TSELR，调节读，默认值01
                                                                  bit[7:6]：TSELW，调节写，默认值01
                                                                  bit[10:8]：TEST，测试pin，默认值000
                                                                  bit[12:11]：TRA，读assist，仅针对DR类型mem，默认值01 */
    } reg;
} SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_UNION;
#endif
#define SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_mem_ctrl_event_in_check_START  (0)
#define SOC_HTS_EVENT_IN_CHECK_MEM_CTRL_mem_ctrl_event_in_check_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_UNION
 struct description   : EVENT_IN_IBUF_MEM_CTRL Register structure definition
                        Address Offset:0x074 Initial:0x00000850 Width:32
 register description : EVENT_IN的IBUF的mem_ctrl寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mem_ctrl_event_in_ibuf : 32; /* bit[0-31]: mem_ctrl_event_in_ibuf
                                                                 bit[0]：LS，Light sleep mode,active high
                                                                 bit[1]：DS，Deep sleep mode,active high
                                                                 bit[2]：SD reserved，mem_sd由外部统一控制
                                                                 bit[3]：reserved
                                                                 bit[5:4]：TSELR，调节读，默认值01
                                                                 bit[7:6]：TSELW，调节写，默认值01
                                                                 bit[10:8]：TEST，测试pin，默认值000
                                                                 bit[12:11]：TRA，读assist，仅针对DR类型mem，默认值01 */
    } reg;
} SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_UNION;
#endif
#define SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_mem_ctrl_event_in_ibuf_START  (0)
#define SOC_HTS_EVENT_IN_IBUF_MEM_CTRL_mem_ctrl_event_in_ibuf_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_UNION
 struct description   : EVENT_PROC_MAPPING_MEM_CTRL Register structure definition
                        Address Offset:0x078 Initial:0x00000850 Width:32
 register description : EVENT_PROC的mapping table的mem_ctrl寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mem_ctrl_mapping : 32; /* bit[0-31]: mem_ctrl_mapping
                                                           bit[0]：LS，Light sleep mode,active high
                                                           bit[1]：DS，Deep sleep mode,active high
                                                           bit[2]：SD reserved，mem_sd由外部统一控制
                                                           bit[3]：reserved
                                                           bit[5:4]：TSELR，调节读，默认值01
                                                           bit[7:6]：TSELW，调节写，默认值01
                                                           bit[10:8]：TEST，测试pin，默认值000
                                                           bit[12:11]：TRA，读assist，仅针对DR类型mem，默认值01 */
    } reg;
} SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_UNION;
#endif
#define SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_mem_ctrl_mapping_START  (0)
#define SOC_HTS_EVENT_PROC_MAPPING_MEM_CTRL_mem_ctrl_mapping_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_UNION
 struct description   : EVENT_PROC_RULE_MEM_CTRL Register structure definition
                        Address Offset:0x07c Initial:0x00000850 Width:32
 register description : EVENT_PROC的rule table的mem_ctrl寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mem_ctrl_rule : 32; /* bit[0-31]: mem_ctrl_rule
                                                        bit[0]：LS，Light sleep mode,active high
                                                        bit[1]：DS，Deep sleep mode,active high
                                                        bit[2]：SD reserved，mem_sd由外部统一控制
                                                        bit[3]：reserved
                                                        bit[5:4]：TSELR，调节读，默认值01
                                                        bit[7:6]：TSELW，调节写，默认值01
                                                        bit[10:8]：TEST，测试pin，默认值000
                                                        bit[12:11]：TRA，读assist，仅针对DR类型mem，默认值01 */
    } reg;
} SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_UNION;
#endif
#define SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_mem_ctrl_rule_START  (0)
#define SOC_HTS_EVENT_PROC_RULE_MEM_CTRL_mem_ctrl_rule_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_UNION
 struct description   : EVENT_PROC_STATUS_MEM_CTRL Register structure definition
                        Address Offset:0x080 Initial:0x00000850 Width:32
 register description : EVENT_PROC的status table的mem_ctrl寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mem_ctrl_status : 32; /* bit[0-31]: mem_ctrl_status
                                                          bit[0]：LS，Light sleep mode,active high
                                                          bit[1]：DS，Deep sleep mode,active high
                                                          bit[2]：SD reserved，mem_sd由外部统一控制
                                                          bit[3]：reserved
                                                          bit[5:4]：TSELR，调节读，默认值01
                                                          bit[7:6]：TSELW，调节写，默认值01
                                                          bit[10:8]：TEST，测试pin，默认值000
                                                          bit[12:11]：TRA，读assist，仅针对DR类型mem，默认值01 */
    } reg;
} SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_UNION;
#endif
#define SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_mem_ctrl_status_START  (0)
#define SOC_HTS_EVENT_PROC_STATUS_MEM_CTRL_mem_ctrl_status_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_UNION
 struct description   : EVENT_OUT_OBUF_MEM_CTRL Register structure definition
                        Address Offset:0x084 Initial:0x00000850 Width:32
 register description : EVENT_OUT的OBUF的mem_ctrl寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mem_ctrl_event_out_obuf : 32; /* bit[0-31]: mem_ctrl_event_out_obuf（输出缓存CCB）
                                                                  bit[0]：LS，Light sleep mode,active high
                                                                  bit[1]：DS，Deep sleep mode,active high
                                                                  bit[2]：SD reserved，mem_sd由外部统一控制
                                                                  bit[3]：reserved
                                                                  bit[5:4]：TSELR，调节读，默认值01
                                                                  bit[7:6]：TSELW，调节写，默认值01
                                                                  bit[10:8]：TEST，测试pin，默认值000
                                                                  bit[12:11]：TRA，读assist，仅针对DR类型mem，默认值01 */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_mem_ctrl_event_out_obuf_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_MEM_CTRL_mem_ctrl_event_out_obuf_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_LOCK_UNION
 struct description   : EVENT_LOCK Register structure definition
                        Address Offset:0x100+0x004*(m) Initial:0x0000 Width:32
 register description : 异常处理封锁event的lock控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_event_lock : 16; /* bit[0-15] : 异常处理封锁event的lock控制寄存器，m范围[0-63]，例如m=0表示lock[1023:0]寄存器中的lock[15:0]，其中lock[0]冗余 */
        unsigned int  bitmasken      : 16; /* bit[16-31]: 每个比特位的使能位：
                                                          只有当bitmasken对应的比特位为1'b1，相应的比特位才起作用。bitmasken[0]就是bit[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HTS_EVENT_LOCK_UNION;
#endif
#define SOC_HTS_EVENT_LOCK_hts_event_lock_START  (0)
#define SOC_HTS_EVENT_LOCK_hts_event_lock_END    (15)
#define SOC_HTS_EVENT_LOCK_bitmasken_START       (16)
#define SOC_HTS_EVENT_LOCK_bitmasken_END         (31)


/*****************************************************************************
 struct               : SOC_HTS_IBUF_EVENT_CNT_CLR_UNION
 struct description   : IBUF_EVENT_CNT_CLR Register structure definition
                        Address Offset:0x200+0x004*(n) Initial:0x00000000 Width:32
 register description : 异常处理封锁IBUF中event cnt的clear控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_ibuf_event_cnt_clr : 32; /* bit[0-31]: 异常处理封锁IBUF中event cnt的clear控制寄存器，n范围[0-31]，例如n=0表示clear[1023:0]寄存器中的clear[31:0]，其中clear[0]冗余
                                                                 写1清，写0无作用 */
    } reg;
} SOC_HTS_IBUF_EVENT_CNT_CLR_UNION;
#endif
#define SOC_HTS_IBUF_EVENT_CNT_CLR_hts_ibuf_event_cnt_clr_START  (0)
#define SOC_HTS_IBUF_EVENT_CNT_CLR_hts_ibuf_event_cnt_clr_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_INT_UNION
 struct description   : INT Register structure definition
                        Address Offset:0x300 Initial:0x0000000 Width:32
 register description : HTS中断指示寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  int_hts_event_in_event_zero_error_pulse             : 1;  /* bit[0]   : EVENT_IN模块的脉冲异常中断，收到event为0 */
        unsigned int  int_hts_event_in_check_error_pulse                  : 1;  /* bit[1]   : EVENT_IN模块的脉冲异常中断，收到event对应的uid和预期uid不符，或uid为0 */
        unsigned int  int_hts_event_in_ibuf_overflow_error_pulse          : 1;  /* bit[2]   : EVENT_IN模块的脉冲异常中断，表示ibuf的cnt有溢出 */
        unsigned int  int_hts_event_proc_no_rule_error_pulse              : 1;  /* bit[3]   : EVENT_PROC模块的脉冲异常中断，表示收到的event没有对应的rule */
        unsigned int  int_hts_event_proc_rule_illegal_error_pulse         : 1;  /* bit[4]   : EVENT_PROC模块的脉冲异常中断，表示rule配置有非法值，或输入event和rule中描述的event不匹配 */
        unsigned int  int_hts_event_proc_process_cnt_overflow_error_pulse : 1;  /* bit[5]   : EVENT_PROC模块的脉冲异常中断，表示1to1 src repeat或多to1时，cnt有溢出 */
        unsigned int  int_hts_event_out_obuf_overflow_error_pulse         : 1;  /* bit[6]   : EVENT_OUT模块的脉冲异常中断，表示obuf的cnt有溢出（输出缓存CCB） */
        unsigned int  reserved                                            : 25; /* bit[7-31]: reserved */
    } reg;
} SOC_HTS_INT_UNION;
#endif
#define SOC_HTS_INT_int_hts_event_in_event_zero_error_pulse_START              (0)
#define SOC_HTS_INT_int_hts_event_in_event_zero_error_pulse_END                (0)
#define SOC_HTS_INT_int_hts_event_in_check_error_pulse_START                   (1)
#define SOC_HTS_INT_int_hts_event_in_check_error_pulse_END                     (1)
#define SOC_HTS_INT_int_hts_event_in_ibuf_overflow_error_pulse_START           (2)
#define SOC_HTS_INT_int_hts_event_in_ibuf_overflow_error_pulse_END             (2)
#define SOC_HTS_INT_int_hts_event_proc_no_rule_error_pulse_START               (3)
#define SOC_HTS_INT_int_hts_event_proc_no_rule_error_pulse_END                 (3)
#define SOC_HTS_INT_int_hts_event_proc_rule_illegal_error_pulse_START          (4)
#define SOC_HTS_INT_int_hts_event_proc_rule_illegal_error_pulse_END            (4)
#define SOC_HTS_INT_int_hts_event_proc_process_cnt_overflow_error_pulse_START  (5)
#define SOC_HTS_INT_int_hts_event_proc_process_cnt_overflow_error_pulse_END    (5)
#define SOC_HTS_INT_int_hts_event_out_obuf_overflow_error_pulse_START          (6)
#define SOC_HTS_INT_int_hts_event_out_obuf_overflow_error_pulse_END            (6)


/*****************************************************************************
 struct               : SOC_HTS_INT_MASK_UNION
 struct description   : INT_MASK Register structure definition
                        Address Offset:0x304 Initial:0x0000000 Width:32
 register description : HTS中断屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  int_hts_event_in_event_zero_error_mask             : 1;  /* bit[0]   : EVENT_IN模块的脉冲异常中断，收到event为0的中断mask */
        unsigned int  int_hts_event_in_check_error_mask                  : 1;  /* bit[1]   : EVENT_IN模块的脉冲异常中断，收到event对应的uid和预期uid不符，或uid为0的中断mask */
        unsigned int  int_hts_event_in_ibuf_overflow_error_mask          : 1;  /* bit[2]   : EVENT_IN模块的脉冲异常中断，表示ibuf的cnt有溢出的中断mask */
        unsigned int  int_hts_event_proc_no_rule_error_mask              : 1;  /* bit[3]   : EVENT_PROC模块的脉冲异常中断，表示收到的event没有对应的rule的中断mask */
        unsigned int  int_hts_event_proc_rule_illegal_error_mask         : 1;  /* bit[4]   : EVENT_PROC模块的脉冲异常中断，表示rule配置有非法值，或输入event和rule中描述的event不匹配的中断mask */
        unsigned int  int_hts_event_proc_process_cnt_overflow_error_mask : 1;  /* bit[5]   : EVENT_PROC模块的脉冲异常中断，表示1to1 src repeat或多to1时，cnt有溢出的中断mask */
        unsigned int  int_hts_event_out_obuf_overflow_error_mask         : 1;  /* bit[6]   : EVENT_OUT模块的脉冲异常中断，表示obuf的cnt有溢出的中断mask（输出缓存CCB） */
        unsigned int  reserved                                           : 25; /* bit[7-31]: reserved */
    } reg;
} SOC_HTS_INT_MASK_UNION;
#endif
#define SOC_HTS_INT_MASK_int_hts_event_in_event_zero_error_mask_START              (0)
#define SOC_HTS_INT_MASK_int_hts_event_in_event_zero_error_mask_END                (0)
#define SOC_HTS_INT_MASK_int_hts_event_in_check_error_mask_START                   (1)
#define SOC_HTS_INT_MASK_int_hts_event_in_check_error_mask_END                     (1)
#define SOC_HTS_INT_MASK_int_hts_event_in_ibuf_overflow_error_mask_START           (2)
#define SOC_HTS_INT_MASK_int_hts_event_in_ibuf_overflow_error_mask_END             (2)
#define SOC_HTS_INT_MASK_int_hts_event_proc_no_rule_error_mask_START               (3)
#define SOC_HTS_INT_MASK_int_hts_event_proc_no_rule_error_mask_END                 (3)
#define SOC_HTS_INT_MASK_int_hts_event_proc_rule_illegal_error_mask_START          (4)
#define SOC_HTS_INT_MASK_int_hts_event_proc_rule_illegal_error_mask_END            (4)
#define SOC_HTS_INT_MASK_int_hts_event_proc_process_cnt_overflow_error_mask_START  (5)
#define SOC_HTS_INT_MASK_int_hts_event_proc_process_cnt_overflow_error_mask_END    (5)
#define SOC_HTS_INT_MASK_int_hts_event_out_obuf_overflow_error_mask_START          (6)
#define SOC_HTS_INT_MASK_int_hts_event_out_obuf_overflow_error_mask_END            (6)


/*****************************************************************************
 struct               : SOC_HTS_DBG_EN_UNION
 struct description   : DBG_EN Register structure definition
                        Address Offset:0x400 Initial:0x0000 Width:32
 register description : HTS DBG回读信息及cnt使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_dbg_en : 1;  /* bit[0]    : HTS DBG回读信息及cnt使能寄存器 */
        unsigned int  reserved_0 : 15; /* bit[1-15] : reserved */
        unsigned int  reserved_1 : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_DBG_EN_UNION;
#endif
#define SOC_HTS_DBG_EN_hts_dbg_en_START  (0)
#define SOC_HTS_DBG_EN_hts_dbg_en_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_READ_BACK_0_UNION
 struct description   : EVENT_IN_READ_BACK_0 Register structure definition
                        Address Offset:0x410 Initial:0x0 Width:32
 register description : EVENT_IN的回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_last_rcv_event  : 10; /* bit[0-9]  : EVENT_IN模块入口处最晚接收到的event */
        unsigned int  event_in_last_rcv_src_id : 6;  /* bit[10-15]: EVENT_IN模块入口处最晚接收到的src_id */
        unsigned int  event_in_last_rcv_uid    : 12; /* bit[16-27]: EVENT_IN模块入口处最晚接收到的uid */
        unsigned int  reserved                 : 4;  /* bit[28-31]: reserved */
    } reg;
} SOC_HTS_EVENT_IN_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_IN_READ_BACK_0_event_in_last_rcv_event_START   (0)
#define SOC_HTS_EVENT_IN_READ_BACK_0_event_in_last_rcv_event_END     (9)
#define SOC_HTS_EVENT_IN_READ_BACK_0_event_in_last_rcv_src_id_START  (10)
#define SOC_HTS_EVENT_IN_READ_BACK_0_event_in_last_rcv_src_id_END    (15)
#define SOC_HTS_EVENT_IN_READ_BACK_0_event_in_last_rcv_uid_START     (16)
#define SOC_HTS_EVENT_IN_READ_BACK_0_event_in_last_rcv_uid_END       (27)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_READ_BACK_1_UNION
 struct description   : EVENT_IN_READ_BACK_1 Register structure definition
                        Address Offset:0x414 Initial:0x0000 Width:32
 register description : EVENT_IN的回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_rcv_event_cnt : 16; /* bit[0-15] : EVENT_IN模块从前级接收到的event的计数器 */
        unsigned int  reserved               : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_IN_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_IN_READ_BACK_1_event_in_rcv_event_cnt_START  (0)
#define SOC_HTS_EVENT_IN_READ_BACK_1_event_in_rcv_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_READ_BACK_2_UNION
 struct description   : EVENT_IN_READ_BACK_2 Register structure definition
                        Address Offset:0x418 Initial:0x0 Width:32
 register description : EVENT_IN的回读寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_ibuf_last_rcv_event    : 10; /* bit[0-9]  : EVENT_IN模块最晚接收到的event */
        unsigned int  reserved_0                      : 6;  /* bit[10-15]: reserved */
        unsigned int  event_in_ibuf_last_rcv_vp_event : 12; /* bit[16-27]: EVENT_IN模块最晚接收到的虚拟event */
        unsigned int  reserved_1                      : 4;  /* bit[28-31]: reserved */
    } reg;
} SOC_HTS_EVENT_IN_READ_BACK_2_UNION;
#endif
#define SOC_HTS_EVENT_IN_READ_BACK_2_event_in_ibuf_last_rcv_event_START     (0)
#define SOC_HTS_EVENT_IN_READ_BACK_2_event_in_ibuf_last_rcv_event_END       (9)
#define SOC_HTS_EVENT_IN_READ_BACK_2_event_in_ibuf_last_rcv_vp_event_START  (16)
#define SOC_HTS_EVENT_IN_READ_BACK_2_event_in_ibuf_last_rcv_vp_event_END    (27)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_READ_BACK_3_UNION
 struct description   : EVENT_IN_READ_BACK_3 Register structure definition
                        Address Offset:0x41c Initial:0x0000 Width:32
 register description : EVENT_IN的回读寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_ibuf_rcv_event_cnt    : 16; /* bit[0-15] : EVENT_IN模块中ibuf从前级接收到的event的计数器 */
        unsigned int  event_in_ibuf_rcv_vp_event_cnt : 16; /* bit[16-31]: EVENT_IN模块中ibuf从前级接收到的虚拟event的计数器 */
    } reg;
} SOC_HTS_EVENT_IN_READ_BACK_3_UNION;
#endif
#define SOC_HTS_EVENT_IN_READ_BACK_3_event_in_ibuf_rcv_event_cnt_START     (0)
#define SOC_HTS_EVENT_IN_READ_BACK_3_event_in_ibuf_rcv_event_cnt_END       (15)
#define SOC_HTS_EVENT_IN_READ_BACK_3_event_in_ibuf_rcv_vp_event_cnt_START  (16)
#define SOC_HTS_EVENT_IN_READ_BACK_3_event_in_ibuf_rcv_vp_event_cnt_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_READ_BACK_4_UNION
 struct description   : EVENT_IN_READ_BACK_4 Register structure definition
                        Address Offset:0x420 Initial:0x0000 Width:32
 register description : EVENT_IN的回读寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_ibuf_bitmap_rd_back_sel : 5;  /* bit[0-4]  : EVENT_IN模块ibuf的bitmap回读选择寄存器，选择回读bitmap的第event_in_ibuf_bitmap_rd_back_sel+1个32bit，例如
                                                                            event_in_ibuf_bitmap_rd_back_sel=0时，回读bitmap[31:0]
                                                                            event_in_ibuf_bitmap_rd_back_sel=31时，回读bitmap[1023:992]
                                                                            不受hts_dbg_en控制 */
        unsigned int  reserved_0                       : 11; /* bit[5-15] : reserved */
        unsigned int  reserved_1                       : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_IN_READ_BACK_4_UNION;
#endif
#define SOC_HTS_EVENT_IN_READ_BACK_4_event_in_ibuf_bitmap_rd_back_sel_START  (0)
#define SOC_HTS_EVENT_IN_READ_BACK_4_event_in_ibuf_bitmap_rd_back_sel_END    (4)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_READ_BACK_5_UNION
 struct description   : EVENT_IN_READ_BACK_5 Register structure definition
                        Address Offset:0x424 Initial:0x00000000 Width:32
 register description : EVENT_IN的回读寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_ibuf_bitmap_rd_back : 32; /* bit[0-31]: EVENT_IN模块ibuf的bitmap回读寄存器，选择回读bitmap的第event_in_ibuf_bitmap_rd_back_sel+1个32bit，例如
                                                                       event_in_ibuf_bitmap_rd_back_sel=0时，回读bitmap[31:0]
                                                                       event_in_ibuf_bitmap_rd_back_sel=31时，回读bitmap[1023:992]
                                                                       不受hts_dbg_en控制 */
    } reg;
} SOC_HTS_EVENT_IN_READ_BACK_5_UNION;
#endif
#define SOC_HTS_EVENT_IN_READ_BACK_5_event_in_ibuf_bitmap_rd_back_START  (0)
#define SOC_HTS_EVENT_IN_READ_BACK_5_event_in_ibuf_bitmap_rd_back_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_UNION
 struct description   : EVENT_PROC_MAPPING_READ_BACK_0 Register structure definition
                        Address Offset:0x430 Initial:0x0000 Width:32
 register description : EVENT_PROC MAPPING的回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_mapping_last_rcv_event : 10; /* bit[0-9]  : EVENT_PROC mapping模块最晚接收到的event */
        unsigned int  reserved_0                        : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_event_proc_mapping_last_rcv_event_START  (0)
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_0_event_proc_mapping_last_rcv_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_UNION
 struct description   : EVENT_PROC_MAPPING_READ_BACK_1 Register structure definition
                        Address Offset:0x434 Initial:0x0000 Width:32
 register description : EVENT_PROC MAPPING的回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_mapping_rcv_event_cnt : 16; /* bit[0-15] : EVENT_PROC mapping模块接收到的event的计数器 */
        unsigned int  reserved                         : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_event_proc_mapping_rcv_event_cnt_START  (0)
#define SOC_HTS_EVENT_PROC_MAPPING_READ_BACK_1_event_proc_mapping_rcv_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_UNION
 struct description   : EVENT_PROC_FETCH_READ_BACK_0 Register structure definition
                        Address Offset:0x440 Initial:0x0000 Width:32
 register description : EVENT_PROC FETCH的回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_fetch_last_rcv_event : 10; /* bit[0-9]  : EVENT_PROC fetch模块最晚接收到的event */
        unsigned int  reserved_0                      : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                      : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_event_proc_fetch_last_rcv_event_START  (0)
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_0_event_proc_fetch_last_rcv_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_UNION
 struct description   : EVENT_PROC_FETCH_READ_BACK_1 Register structure definition
                        Address Offset:0x444 Initial:0x0000 Width:32
 register description : EVENT_PROC FETCH的回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_fetch_last_rcv_rule_addr : 10; /* bit[0-9]  : EVENT_PROC fetch模块最晚接收到的rule_addr */
        unsigned int  reserved_0                          : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                          : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_event_proc_fetch_last_rcv_rule_addr_START  (0)
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_1_event_proc_fetch_last_rcv_rule_addr_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_UNION
 struct description   : EVENT_PROC_FETCH_READ_BACK_2 Register structure definition
                        Address Offset:0x448 Initial:0x0000 Width:32
 register description : EVENT_PROC FETCH的回读寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_fetch_rcv_event_cnt : 16; /* bit[0-15] : EVENT_PROC fetch模块接收到的event的计数器 */
        unsigned int  reserved                       : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_UNION;
#endif
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_event_proc_fetch_rcv_event_cnt_START  (0)
#define SOC_HTS_EVENT_PROC_FETCH_READ_BACK_2_event_proc_fetch_rcv_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_UNION
 struct description   : EVENT_PROC_PROCESS_READ_BACK_0 Register structure definition
                        Address Offset:0x450 Initial:0x0000 Width:32
 register description : EVENT_PROC PROCESS的回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_last_rcv_event : 10; /* bit[0-9]  : EVENT_PROC process模块最晚接收到的event */
        unsigned int  reserved_0                        : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_event_proc_process_last_rcv_event_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_0_event_proc_process_last_rcv_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_UNION
 struct description   : EVENT_PROC_PROCESS_READ_BACK_1 Register structure definition
                        Address Offset:0x454 Initial:0x00000000 Width:32
 register description : EVENT_PROC PROCESS的回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_last_rcv_rule_h : 32; /* bit[0-31]: EVENT_PROC process模块最晚接收到的rule的高32bit */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_event_proc_process_last_rcv_rule_h_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_1_event_proc_process_last_rcv_rule_h_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_UNION
 struct description   : EVENT_PROC_PROCESS_READ_BACK_2 Register structure definition
                        Address Offset:0x458 Initial:0x00000000 Width:32
 register description : EVENT_PROC PROCESS的回读寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_last_rcv_rule_l : 32; /* bit[0-31]: EVENT_PROC process模块最晚接收到的rule的低32bit */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_event_proc_process_last_rcv_rule_l_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_2_event_proc_process_last_rcv_rule_l_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_UNION
 struct description   : EVENT_PROC_PROCESS_READ_BACK_3 Register structure definition
                        Address Offset:0x45c Initial:0x0000 Width:32
 register description : EVENT_PROC PROCESS的回读寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_last_rcv_status : 14; /* bit[0-13] : EVENT_PROC process模块最晚接收到的status */
        unsigned int  reserved_0                         : 2;  /* bit[14-15]: reserved */
        unsigned int  reserved_1                         : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_event_proc_process_last_rcv_status_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_3_event_proc_process_last_rcv_status_END    (13)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_UNION
 struct description   : EVENT_PROC_PROCESS_READ_BACK_4 Register structure definition
                        Address Offset:0x460 Initial:0x0000 Width:32
 register description : EVENT_PROC PROCESS的回读寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_last_snd_status : 14; /* bit[0-13] : EVENT_PROC process模块最晚发送到的status */
        unsigned int  reserved_0                         : 2;  /* bit[14-15]: reserved */
        unsigned int  reserved_1                         : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_event_proc_process_last_snd_status_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_4_event_proc_process_last_snd_status_END    (13)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_UNION
 struct description   : EVENT_PROC_PROCESS_READ_BACK_5 Register structure definition
                        Address Offset:0x464 Initial:0x0000 Width:32
 register description : EVENT_PROC PROCESS的回读寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_proces_rcv_event_cnt : 16; /* bit[0-15] : EVENT_PROC process模块接收到的event的计数器 */
        unsigned int  reserved                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_event_proc_proces_rcv_event_cnt_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_READ_BACK_5_event_proc_proces_rcv_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_UNION
 struct description   : EVENT_OUT_DISPATCH_READ_BACK_0 Register structure definition
                        Address Offset:0x470 Initial:0x0000 Width:32
 register description : EVENT_OUT_DISPATCH的回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_dispatch_last_rcv_event : 10; /* bit[0-9]  : EVENT_OUT dispatch模块最晚接收到的event */
        unsigned int  reserved_0                        : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_event_out_dispatch_last_rcv_event_START  (0)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_0_event_out_dispatch_last_rcv_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_UNION
 struct description   : EVENT_OUT_DISPATCH_READ_BACK_1 Register structure definition
                        Address Offset:0x474 Initial:0x0000 Width:32
 register description : EVENT_OUT_DISPATCH的回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_dispatch_rcv_event_cnt : 16; /* bit[0-15] : EVENT_OUT dispatch模块接收到的event的计数器 */
        unsigned int  reserved                         : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_event_out_dispatch_rcv_event_cnt_START  (0)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_1_event_out_dispatch_rcv_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_UNION
 struct description   : EVENT_OUT_DISPATCH_READ_BACK_2 Register structure definition
                        Address Offset:0x478 Initial:0x00 Width:32
 register description : EVENT_OUT_DISPATCH的回读寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_dispatch_last_snd_event    : 10; /* bit[0-9]  : EVENT_OUT dispatch模块最晚发送的event */
        unsigned int  reserved_0                           : 6;  /* bit[10-15]: reserved */
        unsigned int  event_out_dispatch_last_snd_vp_event : 10; /* bit[16-25]: EVENT_OUT dispatch模块最晚发送的虚拟event */
        unsigned int  reserved_1                           : 6;  /* bit[26-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_UNION;
#endif
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_event_out_dispatch_last_snd_event_START     (0)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_event_out_dispatch_last_snd_event_END       (9)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_event_out_dispatch_last_snd_vp_event_START  (16)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_2_event_out_dispatch_last_snd_vp_event_END    (25)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_UNION
 struct description   : EVENT_OUT_DISPATCH_READ_BACK_3 Register structure definition
                        Address Offset:0x47c Initial:0x0000 Width:32
 register description : EVENT_OUT_DISPATCH的回读寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_dispatch_snd_event_cnt    : 16; /* bit[0-15] : EVENT_OUT dispatch模块发送的event的计数器 */
        unsigned int  event_out_dispatch_snd_vp_event_cnt : 16; /* bit[16-31]: EVENT_OUT dispatch模块发送的虚拟event的计数器 */
    } reg;
} SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_UNION;
#endif
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_event_out_dispatch_snd_event_cnt_START     (0)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_event_out_dispatch_snd_event_cnt_END       (15)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_event_out_dispatch_snd_vp_event_cnt_START  (16)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_3_event_out_dispatch_snd_vp_event_cnt_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_UNION
 struct description   : EVENT_OUT_DISPATCH_READ_BACK_4 Register structure definition
                        Address Offset:0x480 Initial:0x0000 Width:32
 register description : EVENT_OUT_DISPATCH的回读寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_lock_last_snd_event : 10; /* bit[0-9]  : EVENT_OUT lock模块最晚发送的event */
        unsigned int  reserved_0                    : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                    : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_UNION;
#endif
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_event_out_lock_last_snd_event_START  (0)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_4_event_out_lock_last_snd_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_UNION
 struct description   : EVENT_OUT_DISPATCH_READ_BACK_5 Register structure definition
                        Address Offset:0x484 Initial:0x0000 Width:32
 register description : EVENT_OUT_DISPATCH的回读寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_lock_snd_event_cnt : 16; /* bit[0-15] : EVENT_OUT lock模块发送的event的计数器 */
        unsigned int  reserved                     : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_UNION;
#endif
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_event_out_lock_snd_event_cnt_START  (0)
#define SOC_HTS_EVENT_OUT_DISPATCH_READ_BACK_5_event_out_lock_snd_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_0 Register structure definition
                        Address Offset:0x490 Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_bitmap_rd_back_sel : 5;  /* bit[0-4]  : EVENT_OUT模块obuf的bitmap回读选择寄存器，选择回读bitmap的第event_out_obuf_bitmap_rd_back_sel+1个32bit，例如
                                                                             event_out_obuf_bitmap_rd_back_sel=0时，回读bitmap[31:0]
                                                                             event_out_obuf_bitmap_rd_back_sel=31时，回读bitmap[1023:992]
                                                                             不受hts_dbg_en控制（输出缓存CCB） */
        unsigned int  reserved_0                        : 11; /* bit[5-15] : reserved */
        unsigned int  reserved_1                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_event_out_obuf_bitmap_rd_back_sel_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_0_event_out_obuf_bitmap_rd_back_sel_END    (4)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_1 Register structure definition
                        Address Offset:0x494 Initial:0x00000000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_bitmap_rd_back : 32; /* bit[0-31]: EVENT_OUT模块obuf的bitmap回读寄存器，选择回读bitmap的第event_out_obuf_bitmap_rd_back_sel+1个32bit，例如
                                                                        event_out_obuf_bitmap_rd_back_sel=0时，回读bitmap[31:0]
                                                                        event_out_obuf_bitmap_rd_back_sel=31时，回读bitmap[1023:992]
                                                                        不受hts_dbg_en控制（输出缓存CCB） */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_event_out_obuf_bitmap_rd_back_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_1_event_out_obuf_bitmap_rd_back_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_2 Register structure definition
                        Address Offset:0x49c Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_mux_last_snd_event : 10; /* bit[0-9]  : EVENT_OUT obuf mux模块发送给obuf dispatch模块的event（输出缓存CCB） */
        unsigned int  reserved_0                        : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_event_out_obuf_mux_last_snd_event_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_2_event_out_obuf_mux_last_snd_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_3 Register structure definition
                        Address Offset:0x4a0 Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_mux_snd_event_cnt : 16; /* bit[0-15] : EVENT_OUT obuf mux模块发送给obuf dispatch模块的event的计数器（输出缓存CCB） */
        unsigned int  reserved                         : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_event_out_obuf_mux_snd_event_cnt_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_3_event_out_obuf_mux_snd_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_4 Register structure definition
                        Address Offset:0x4a4 Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_dispatch_last_snd_event : 10; /* bit[0-9]  : EVENT_OUT obuf dispatch模块发送给obuf的event（输出缓存CCB） */
        unsigned int  reserved_0                             : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                             : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_event_out_obuf_dispatch_last_snd_event_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_4_event_out_obuf_dispatch_last_snd_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_5 Register structure definition
                        Address Offset:0x4a8 Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_dispatch_snd_event_cnt : 16; /* bit[0-15] : EVENT_OUT obuf dispatch模块发送给obuf的event的计数器（输出缓存CCB） */
        unsigned int  reserved                              : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_event_out_obuf_dispatch_snd_event_cnt_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_5_event_out_obuf_dispatch_snd_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_6 Register structure definition
                        Address Offset:0x4ac Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_arb_last_snd_event : 10; /* bit[0-9]  : EVENT_OUT obuf arb模块最晚发送的event（输出缓存CCB） */
        unsigned int  reserved_0                        : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                        : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_event_out_obuf_arb_last_snd_event_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_6_event_out_obuf_arb_last_snd_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_UNION
 struct description   : EVENT_OUT_OBUF_READ_BACK_7 Register structure definition
                        Address Offset:0x4b0 Initial:0x0000 Width:32
 register description : EVENT_OUT_OBUF的回读寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_arb_snd_event_cnt : 16; /* bit[0-15] : EVENT_OUT obuf arb模块发送的event的计数器（输出缓存CCB） */
        unsigned int  reserved                         : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_UNION;
#endif
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_event_out_obuf_arb_snd_event_cnt_START  (0)
#define SOC_HTS_EVENT_OUT_OBUF_READ_BACK_7_event_out_obuf_arb_snd_event_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_UNION
 struct description   : EVENT_IN_ERROR_READ_BACK_0 Register structure definition
                        Address Offset:0x500 Initial:0x0 Width:32
 register description : EVENT_IN的异常状态回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_check_error_last_event  : 10; /* bit[0-9]  : EVENT_IN模块最晚接收到的uid check不通过，或uid为0对应的event */
        unsigned int  event_in_check_error_last_src_id : 6;  /* bit[10-15]: EVENT_IN模块最晚接收到的uid check不通过，或uid为0对应的src_id */
        unsigned int  event_in_check_error_last_uid    : 12; /* bit[16-27]: EVENT_IN模块最晚接收到的uid check不通过，或uid为0对应的uid */
        unsigned int  reserved                         : 4;  /* bit[28-31]: reserved */
    } reg;
} SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_event_in_check_error_last_event_START   (0)
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_event_in_check_error_last_event_END     (9)
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_event_in_check_error_last_src_id_START  (10)
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_event_in_check_error_last_src_id_END    (15)
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_event_in_check_error_last_uid_START     (16)
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_0_event_in_check_error_last_uid_END       (27)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_UNION
 struct description   : EVENT_IN_ERROR_READ_BACK_1 Register structure definition
                        Address Offset:0x504 Initial:0x0000 Width:32
 register description : EVENT_IN的异常状态回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_in_ibuf_overflow_error_last_event : 10; /* bit[0-9]  : EVENT_IN模块最晚一个cnt溢出对应的event */
        unsigned int  reserved_0                              : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                              : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_event_in_ibuf_overflow_error_last_event_START  (0)
#define SOC_HTS_EVENT_IN_ERROR_READ_BACK_1_event_in_ibuf_overflow_error_last_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_UNION
 struct description   : EVENT_PROC_PROCESS_ERROR_READ_BACK_0 Register structure definition
                        Address Offset:0x510 Initial:0x0000 Width:32
 register description : EVENT_PROC的PROCESS异常状态回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_mapping_no_rule_error_last_event : 10; /* bit[0-9]  : EVENT_PROC模块的mapping阶段没有对应的rule时，对应的event */
        unsigned int  reserved_0                                  : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                                  : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_event_proc_mapping_no_rule_error_last_event_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_0_event_proc_mapping_no_rule_error_last_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_UNION
 struct description   : EVENT_PROC_PROCESS_ERROR_READ_BACK_1 Register structure definition
                        Address Offset:0x514 Initial:0x00000000 Width:32
 register description : EVENT_PROC的PROCESS异常状态回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_illegal_rule_error_last_rule_h : 32; /* bit[0-31]: EVENT_PROC模块process过程中发现rule配置有非法值，或输入event和rule中描述的event不匹配，对应的rule的高32bit */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_event_proc_process_illegal_rule_error_last_rule_h_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_1_event_proc_process_illegal_rule_error_last_rule_h_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_UNION
 struct description   : EVENT_PROC_PROCESS_ERROR_READ_BACK_2 Register structure definition
                        Address Offset:0x518 Initial:0x00000000 Width:32
 register description : EVENT_PROC的PROCESS异常状态回读寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_illegal_rule_error_last_rule_l : 32; /* bit[0-31]: EVENT_PROC模块process过程中发现rule配置有非法值，或输入event和rule中描述的event不匹配，对应的rule的低32bit */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_event_proc_process_illegal_rule_error_last_rule_l_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_2_event_proc_process_illegal_rule_error_last_rule_l_END    (31)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_UNION
 struct description   : EVENT_PROC_PROCESS_ERROR_READ_BACK_3 Register structure definition
                        Address Offset:0x51c Initial:0x0000 Width:32
 register description : EVENT_PROC的PROCESS异常状态回读寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_illegal_rule_error_last_event : 10; /* bit[0-9]  : EVENT_PROC模块process过程中发现rule配置有非法值，或输入event和rule中描述的event不匹配，对应的event */
        unsigned int  reserved_0                                       : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                                       : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_event_proc_process_illegal_rule_error_last_event_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_3_event_proc_process_illegal_rule_error_last_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_UNION
 struct description   : EVENT_PROC_PROCESS_ERROR_READ_BACK_4 Register structure definition
                        Address Offset:0x520 Initial:0x0000 Width:32
 register description : EVENT_PROC的PROCESS异常状态回读寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_proc_process_status_overflow_error_last_event : 10; /* bit[0-9]  : EVENT_PROC模块的process过程中发现status cnt有溢出时对应的event */
        unsigned int  reserved_0                                          : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                                          : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_UNION;
#endif
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_event_proc_process_status_overflow_error_last_event_START  (0)
#define SOC_HTS_EVENT_PROC_PROCESS_ERROR_READ_BACK_4_event_proc_process_status_overflow_error_last_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_UNION
 struct description   : EVENT_OUT_ERROR_READ_BACK_0 Register structure definition
                        Address Offset:0x524 Initial:0x0000 Width:32
 register description : EVENT_OUT的异常状态回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  event_out_obuf_overflow_error_last_event : 10; /* bit[0-9]  : EVENT_OUT模块最晚一个cnt溢出对应的event（输出缓存CCB） */
        unsigned int  reserved_0                               : 6;  /* bit[10-15]: reserved */
        unsigned int  reserved_1                               : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_UNION;
#endif
#define SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_event_out_obuf_overflow_error_last_event_START  (0)
#define SOC_HTS_EVENT_OUT_ERROR_READ_BACK_0_event_out_obuf_overflow_error_last_event_END    (9)


/*****************************************************************************
 struct               : SOC_HTS_EVENT_STORE_UNION
 struct description   : EVENT_STORE Register structure definition
                        Address Offset:0x600+0x004*(e) Initial:0x0000 Width:32
 register description : 缓存event的store控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_event_store : 16; /* bit[0-15] : 缓存event的store控制寄存器，e范围[0-63]，例如e=0表示store[1023:0]寄存器中的lock[15:0]，其中store[0]冗余（输出缓存CCB） */
        unsigned int  bitmasken       : 16; /* bit[16-31]: 每个比特位的使能位：
                                                           只有当bitmasken对应的比特位为1'b1，相应的比特位才起作用。bitmasken[0]就是bit[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HTS_EVENT_STORE_UNION;
#endif
#define SOC_HTS_EVENT_STORE_hts_event_store_START  (0)
#define SOC_HTS_EVENT_STORE_hts_event_store_END    (15)
#define SOC_HTS_EVENT_STORE_bitmasken_START        (16)
#define SOC_HTS_EVENT_STORE_bitmasken_END          (31)


/*****************************************************************************
 struct               : SOC_HTS_OBUF_EVENT_CNT_CLR_UNION
 struct description   : OBUF_EVENT_CNT_CLR Register structure definition
                        Address Offset:0x700+0x004*(f) Initial:0x00000000 Width:32
 register description : OBUF中event cnt的clear控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_obuf_event_cnt_clr : 32; /* bit[0-31]: OBUF中event cnt的clear控制寄存器，f范围[0-31]，例如f=0表示clear[1023:0]寄存器中的clear[31:0]，其中clear[0]冗余（输出缓存CCB）
                                                                 写1清，写0无作用 */
    } reg;
} SOC_HTS_OBUF_EVENT_CNT_CLR_UNION;
#endif
#define SOC_HTS_OBUF_EVENT_CNT_CLR_hts_obuf_event_cnt_clr_START  (0)
#define SOC_HTS_OBUF_EVENT_CNT_CLR_hts_obuf_event_cnt_clr_END    (31)




/****************************************************************************
                     (2/6) CHECK_TABLE
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_HTS_EVENT_IN_CHECK_UNION
 struct description   : EVENT_IN_CHECK Register structure definition
                        Address Offset:0x0004*(p) Initial:0x00000000 Width:32
 register description : EVENT_IN_CHECK的event_i到uid的映射表
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_mapping_idx : 32; /* bit[0-31]: CHECK模块中event到uid的查找表，p范围[1-1023]
                                                          bit[31:12]：reserved
                                                          bit[11:0]：event_p对应的uid
                                                          该寄存器为ram实现，默认值为x态 */
    } reg;
} SOC_HTS_EVENT_IN_CHECK_UNION;
#endif
#define SOC_HTS_EVENT_IN_CHECK_cfg_mapping_idx_START  (0)
#define SOC_HTS_EVENT_IN_CHECK_cfg_mapping_idx_END    (31)




/****************************************************************************
                     (3/6) MAPPING_TABLE
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_MAPPING_IDX_UNION
 struct description   : EVENT_PROC_MAPPING_IDX Register structure definition
                        Address Offset:0x0004*(i) Initial:0x00000000 Width:32
 register description : EVENT_PROC_MAPPING的event_i到rule_addr的映射表
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_mapping_idx : 32; /* bit[0-31]: MAPPING模块中event到rule_addr的查找表，i范围[1-1023]
                                                          bit[31:10]：reserved
                                                          bit[9:0]：eventi对应的rule_addr
                                                          该寄存器为ram实现，默认值为x态 */
    } reg;
} SOC_HTS_EVENT_PROC_MAPPING_IDX_UNION;
#endif
#define SOC_HTS_EVENT_PROC_MAPPING_IDX_cfg_mapping_idx_START  (0)
#define SOC_HTS_EVENT_PROC_MAPPING_IDX_cfg_mapping_idx_END    (31)




/****************************************************************************
                     (4/6) RULE_TABLE
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_RULE_UNION
 struct description   : EVENT_PROC_RULE Register structure definition
                        Address Offset:0x0004*(j) Initial:0x00000000 Width:32
 register description : EVENT_PROC_MAPPING的event到rule_addr的映射表
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_rule : 32; /* bit[0-31]: EVENT_PROC_FETCH的rule，每条rule64bit，j范围[2-2047]
                                                   j为偶数，表示第（j模2）条rule的低32bit
                                                   j为奇数，表示第（j模2）条rule的高32bit
                                                   例如：
                                                   rule_addr1对应的rule的低32bit为j=2
                                                   rule_addr1对应的rule的高32bit为j=3
                                                   该寄存器为ram实现，默认值为x态 */
    } reg;
} SOC_HTS_EVENT_PROC_RULE_UNION;
#endif
#define SOC_HTS_EVENT_PROC_RULE_cfg_rule_START  (0)
#define SOC_HTS_EVENT_PROC_RULE_cfg_rule_END    (31)




/****************************************************************************
                     (5/6) STATUS_TABLE
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_HTS_EVENT_PROC_STATUS_UNION
 struct description   : EVENT_PROC_STATUS Register structure definition
                        Address Offset:0x0004*(k) Initial:0x00000000 Width:32
 register description : EVENT_PROC的rule_status回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_status_read_back : 32; /* bit[0-31]: EVENT_PROC的rule_status回读，k范围[1-1023]
                                                               cmd_code=0或1时有效
                                                               cmd_code=0（1to1 repeat，dst_repeat_time=1）：
                                                               bit[31:6]：reserved
                                                               bit[5:0]：src_event0_cnt 
                                                               cmd_code=1（3to1）：
                                                               bit[31:14]：reserved
                                                               bit[13:10]：src_event2_cnt
                                                               bit[9:6]：src_event1_cnt
                                                               bit[5:0]：src_event0_cnt
                                                               cmd_code=1（2to1）：
                                                               bit[31:10]：reserved
                                                               bit[9:6]：src_event1_cnt
                                                               bit[5:0]：src_event0_cnt
                                                               该寄存器为ram实现，默认值为x态 */
    } reg;
} SOC_HTS_EVENT_PROC_STATUS_UNION;
#endif
#define SOC_HTS_EVENT_PROC_STATUS_cfg_status_read_back_START  (0)
#define SOC_HTS_EVENT_PROC_STATUS_cfg_status_read_back_END    (31)




/****************************************************************************
                     (6/6) HTS_WRAP
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_HTS_WRAP_OST_CNT_0_UNION
 struct description   : WRAP_OST_CNT_0 Register structure definition
                        Address Offset:0x000 Initial:0x0 Width:32
 register description : HTS_WRAP发出outstanding的维测回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  total_ost_cnt     : 5;  /* bit[0-4]  : HTS向消息总线发出的所有未返回的outstanding计数，最大值是16 */
        unsigned int  reserved_0        : 3;  /* bit[5-7]  : reserved */
        unsigned int  gpu_ost_cnt       : 5;  /* bit[8-12] : HTS向消息总线发出的以gpu为目的地且未返回的outstanding计数，最大值是16 */
        unsigned int  reserved_1        : 3;  /* bit[13-15]: reserved */
        unsigned int  npu_hwts_ost_cnt  : 5;  /* bit[16-20]: HTS向消息总线发出的以npu_hwts为目的地且未返回的outstanding计数，最大值是16 */
        unsigned int  reserved_2        : 3;  /* bit[21-23]: reserved */
        unsigned int  npu_tscpu_ost_cnt : 5;  /* bit[24-28]: HTS向消息总线发出的以npu_tscpu为目的地且未返回的outstanding计数，最大值是16 */
        unsigned int  reserved_3        : 3;  /* bit[29-31]: reserved */
    } reg;
} SOC_HTS_WRAP_OST_CNT_0_UNION;
#endif
#define SOC_HTS_WRAP_OST_CNT_0_total_ost_cnt_START      (0)
#define SOC_HTS_WRAP_OST_CNT_0_total_ost_cnt_END        (4)
#define SOC_HTS_WRAP_OST_CNT_0_gpu_ost_cnt_START        (8)
#define SOC_HTS_WRAP_OST_CNT_0_gpu_ost_cnt_END          (12)
#define SOC_HTS_WRAP_OST_CNT_0_npu_hwts_ost_cnt_START   (16)
#define SOC_HTS_WRAP_OST_CNT_0_npu_hwts_ost_cnt_END     (20)
#define SOC_HTS_WRAP_OST_CNT_0_npu_tscpu_ost_cnt_START  (24)
#define SOC_HTS_WRAP_OST_CNT_0_npu_tscpu_ost_cnt_END    (28)


/*****************************************************************************
 struct               : SOC_HTS_IDLE_EN_UNION
 struct description   : IDLE_EN Register structure definition
                        Address Offset:0x010 Initial:0x00000000 Width:32
 register description : HTS及HTS_WRAP的idle的使能控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_wrap_idle_en : 1;  /* bit[0]   : HTS的硬件idle使能控制，1表示使用硬件的idle逻辑，0表示hts对外永远非idle */
        unsigned int  reserved         : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_HTS_IDLE_EN_UNION;
#endif
#define SOC_HTS_IDLE_EN_hts_wrap_idle_en_START  (0)
#define SOC_HTS_IDLE_EN_hts_wrap_idle_en_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_IDLE_READ_BACK_UNION
 struct description   : IDLE_READ_BACK Register structure definition
                        Address Offset:0x014 Initial:0x0000000 Width:32
 register description : HTS及HTS_WRAP的idle状态回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hts_idle                 : 1;  /* bit[0]   : HTS的idle指示信号回读寄存器，1表示idle */
        unsigned int  hts_wrap_mst_to_qsp_idle : 1;  /* bit[1]   : HTS_WRAP中的MST_TO_QSP模块的idle指示信号回读寄存器，1表示idle */
        unsigned int  hts_wrap_apb_dec_idle    : 1;  /* bit[2]   : HTS_WRAP中的APB_DEC模块的idle指示信号回读寄存器，1表示idle。该寄存器回读时在有apb访问时一定是0，因此寄存器回读值永远是0 */
        unsigned int  hts_wrap_idle            : 1;  /* bit[3]   : HTS_WRAP整体的idle指示信号回读寄存器，只有hts_idle、hts_wrap_apb_dec_idle和hts_wrap_mst_to_qsp_idle都为idle状态时才idle，1表示idle。该寄存器回读时由于hts_wrap_apb_dec_idle在有apb访问时一定是0，因此寄存器回读值永远是0 */
        unsigned int  reserved                 : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_HTS_IDLE_READ_BACK_UNION;
#endif
#define SOC_HTS_IDLE_READ_BACK_hts_idle_START                  (0)
#define SOC_HTS_IDLE_READ_BACK_hts_idle_END                    (0)
#define SOC_HTS_IDLE_READ_BACK_hts_wrap_mst_to_qsp_idle_START  (1)
#define SOC_HTS_IDLE_READ_BACK_hts_wrap_mst_to_qsp_idle_END    (1)
#define SOC_HTS_IDLE_READ_BACK_hts_wrap_apb_dec_idle_START     (2)
#define SOC_HTS_IDLE_READ_BACK_hts_wrap_apb_dec_idle_END       (2)
#define SOC_HTS_IDLE_READ_BACK_hts_wrap_idle_START             (3)
#define SOC_HTS_IDLE_READ_BACK_hts_wrap_idle_END               (3)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION
 struct description   : WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ Register structure definition
                        Address Offset:0x100 Initial:0x00000000 Width:32
 register description : HTS_WRAP的总线不反压计数器启动的请求寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ibuf_clr_bus_nopress_cnt_req : 1;  /* bit[0]   : HTS封锁清除流程中，软件清除所有要销毁的event对应的ibuf缓存后，要写1配置本寄存器，使能总线无反压计数器，写0无效 */
        unsigned int  reserved                     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION;
#endif
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_ibuf_clr_bus_nopress_cnt_req_START  (0)
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_REQ_ibuf_clr_bus_nopress_cnt_req_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION
 struct description   : WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK Register structure definition
                        Address Offset:0x104 Initial:0x00000000 Width:32
 register description : HTS_WRAP的总线不反压计数器完成的状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ibuf_clr_bus_nopress_cnt_ack : 1;  /* bit[0]   : HTS封锁清除流程中，软件使能总线无反压计数器后，查询本寄存器为1表示计数器计数到预期值，可以继续封锁清除流程，为0表示计数器未计数到预期值，需要继续查询指导查到1才能继续封锁清除流程 */
        unsigned int  reserved                     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION;
#endif
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_ibuf_clr_bus_nopress_cnt_ack_START  (0)
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ACK_ibuf_clr_bus_nopress_cnt_ack_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION
 struct description   : WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE Register structure definition
                        Address Offset:0x108 Initial:0x0000 Width:32
 register description : HTS_WRAP的总线不反压计数器的预期计数值寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ibuf_clr_bus_nopress_cnt_value : 16; /* bit[0-15] : 总线无反压计数器的预期计数值，计数到达预期计数值后拉高ibuf_clr_bus_nopress_cnt_ack。默认值为700 */
        unsigned int  reserved                       : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION;
#endif
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_ibuf_clr_bus_nopress_cnt_value_START  (0)
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_VALUE_ibuf_clr_bus_nopress_cnt_value_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_UNION
 struct description   : WRAP_IBUF_CLR_BUS_NOPRESS_CNT Register structure definition
                        Address Offset:0x10c Initial:0x0000 Width:32
 register description : HTS_WRAP的总线不反压计数器的实际计数值寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ibuf_clr_bus_nopress_cnt : 16; /* bit[0-15] : 总线无反压计数器的实际计数值 */
        unsigned int  reserved                 : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_UNION;
#endif
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ibuf_clr_bus_nopress_cnt_START  (0)
#define SOC_HTS_WRAP_IBUF_CLR_BUS_NOPRESS_CNT_ibuf_clr_bus_nopress_cnt_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION
 struct description   : WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ Register structure definition
                        Address Offset:0x200 Initial:0x00000000 Width:32
 register description : HTS_WRAP缓存功能的总线不反压计数器启动的请求寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  obuf_clr_bus_nopress_cnt_req : 1;  /* bit[0]   : HTS缓存流程中，软件配置所要缓存的event对应的obuf后，要写1配置本寄存器，使能总线无反压计数器，写0无效 */
        unsigned int  reserved                     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_UNION;
#endif
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_obuf_clr_bus_nopress_cnt_req_START  (0)
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_REQ_obuf_clr_bus_nopress_cnt_req_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION
 struct description   : WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK Register structure definition
                        Address Offset:0x204 Initial:0x00000000 Width:32
 register description : HTS_WRAP缓存功能的总线不反压计数器完成的状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  obuf_clr_bus_nopress_cnt_ack : 1;  /* bit[0]   : HTS缓存流程中，软件使能缓存功能的总线无反压计数器后，查询本寄存器为1表示计数器计数到预期值，可以继续缓存流程，为0表示计数器未计数到预期值，需要继续查询指导查到1才能继续缓存流程 */
        unsigned int  reserved                     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_UNION;
#endif
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_obuf_clr_bus_nopress_cnt_ack_START  (0)
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_ACK_obuf_clr_bus_nopress_cnt_ack_END    (0)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION
 struct description   : WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE Register structure definition
                        Address Offset:0x208 Initial:0x0000 Width:32
 register description : HTS_WRAP缓存功能的总线不反压计数器的预期计数值寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  obuf_clr_bus_nopress_cnt_value : 16; /* bit[0-15] : 缓存功能的总线无反压计数器的预期计数值，计数到达预期计数值后拉高obuf_clr_bus_nopress_cnt_ack。默认值为10 */
        unsigned int  reserved                       : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_UNION;
#endif
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_obuf_clr_bus_nopress_cnt_value_START  (0)
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_VALUE_obuf_clr_bus_nopress_cnt_value_END    (15)


/*****************************************************************************
 struct               : SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_UNION
 struct description   : WRAP_OBUF_CLR_BUS_NOPRESS_CNT Register structure definition
                        Address Offset:0x20c Initial:0x0000 Width:32
 register description : HTS_WRAP缓存功能的总线不反压计数器的实际计数值寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  obuf_clr_bus_nopress_cnt : 16; /* bit[0-15] : 缓存功能的总线无反压计数器的实际计数值 */
        unsigned int  reserved                 : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_UNION;
#endif
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_obuf_clr_bus_nopress_cnt_START  (0)
#define SOC_HTS_WRAP_OBUF_CLR_BUS_NOPRESS_CNT_obuf_clr_bus_nopress_cnt_END    (15)






/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of soc_hts_interface.h */
