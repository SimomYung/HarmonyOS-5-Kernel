

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_INTR_HUB_INTERFACE_H__
#define __SOC_INTR_HUB_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) to_acpu
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: 中断安全属性寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_SEC_INFO_n_UNION */
#define SOC_INTR_HUB_L2_SEC_INFO_n_ADDR(base, n)              ((base) + (n) * 0x10UL)

/* Register description: 中断MASK设置寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_INTR_MASKSET_S_n_UNION */
#define SOC_INTR_HUB_L2_INTR_MASKSET_S_n_ADDR(base, n)        ((base) + (n) * 0x10UL + 0x4UL)

/* Register description: 中断MASK清除寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_UNION */
#define SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_ADDR(base, n)        ((base) + (n) * 0x10UL + 0x8UL)

/* Register description: 中断状态寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_INTR_STATUS_S_n_UNION */
#define SOC_INTR_HUB_L2_INTR_STATUS_S_n_ADDR(base, n)         ((base) + (n) * 0x10UL + 0xCUL)

/* Register description: INTR_HUB CONTROL Register
   Bit domain definition UNION:  SOC_INTR_HUB_CTRL_UNION */
#define SOC_INTR_HUB_CTRL_ADDR(base)                          ((base) + 0x03fcUL)


#else


/* Register description: 中断安全属性寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_SEC_INFO_n_UNION */
#define SOC_INTR_HUB_L2_SEC_INFO_n_ADDR(base, n)              ((base) + (n) * 0x10)

/* Register description: 中断MASK设置寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_INTR_MASKSET_S_n_UNION */
#define SOC_INTR_HUB_L2_INTR_MASKSET_S_n_ADDR(base, n)        ((base) + (n) * 0x10 + 0x4)

/* Register description: 中断MASK清除寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_UNION */
#define SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_ADDR(base, n)        ((base) + (n) * 0x10 + 0x8)

/* Register description: 中断状态寄存器
   Bit domain definition UNION:  SOC_INTR_HUB_L2_INTR_STATUS_S_n_UNION */
#define SOC_INTR_HUB_L2_INTR_STATUS_S_n_ADDR(base, n)         ((base) + (n) * 0x10 + 0xC)

/* Register description: INTR_HUB CONTROL Register
   Bit domain definition UNION:  SOC_INTR_HUB_CTRL_UNION */
#define SOC_INTR_HUB_CTRL_ADDR(base)                          ((base) + 0x03fc)


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
                     (1/1) to_acpu
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_INTR_HUB_L2_SEC_INFO_n_UNION
 struct description   : L2_SEC_INFO_n Register structure definition
                        Address Offset:(n)*0x10+0x0 Initial:0x00000000 Width:32
 register description : 中断安全属性寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  level2_reg_sec_info : 1;  /* bit[0]   : 设置第n组MASK设置，MASK清除，中断状态寄存器的访问权限：
                                                              值为0：安全非安全均可读可写
                                                              值为1：仅安全可读可写
                                                              
                                                              (送往ACPU的中断复位值为0，其他为1) */
        unsigned int  reserved            : 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_INTR_HUB_L2_SEC_INFO_n_UNION;
#endif
#define SOC_INTR_HUB_L2_SEC_INFO_n_level2_reg_sec_info_START  (0)
#define SOC_INTR_HUB_L2_SEC_INFO_n_level2_reg_sec_info_END    (0)


/*****************************************************************************
 struct               : SOC_INTR_HUB_L2_INTR_MASKSET_S_n_UNION
 struct description   : L2_INTR_MASKSET_S_n Register structure definition
                        Address Offset:(n)*0x10+0x4 Initial:0xFFFFFFFF Width:32
 register description : 中断MASK设置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  level2_intr_maskset : 32; /* bit[0-31]: 设置中断的中断屏蔽，每个bit对应一根输入中断
                                                              对应bit位为1：开启中断
                                                              对应bit位为0：屏蔽中断
                                                              n=N寄存器的第Mbit 代表 中断需求汇总中L2_N第Mbit的中断。
                                                              
                                                              读写该寄存器的效果如下：
                                                              读该寄存器：获取当前中断的MASK位，1为开启，0为关闭
                                                              写该寄存器：对应比特为0表示不改变MASK为，对应比特为1表示设置该MASK比特为1，即开启中断.
                                                              
                                                              (送往ACPU的中断复位值为0xffffffff，其他为0) */
    } reg;
} SOC_INTR_HUB_L2_INTR_MASKSET_S_n_UNION;
#endif
#define SOC_INTR_HUB_L2_INTR_MASKSET_S_n_level2_intr_maskset_START  (0)
#define SOC_INTR_HUB_L2_INTR_MASKSET_S_n_level2_intr_maskset_END    (31)


/*****************************************************************************
 struct               : SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_UNION
 struct description   : L2_INTR_MASKCLR_S_n Register structure definition
                        Address Offset:(n)*0x10+0x8 Initial:0xFFFFFFFF Width:32
 register description : 中断MASK清除寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  level2_intr_maskclr : 32; /* bit[0-31]: 设置中断的中断屏蔽，每个bit对应一根输入中断
                                                              对应bit位为1：开启中断
                                                              对应bit位为0：屏蔽中断
                                                              n=N寄存器的第Mbit 代表 中断需求汇总中L2_N第Mbit的中断。
                                                              
                                                              读写该寄存器的效果如下：
                                                              读该寄存器：获取当前中断的MASK位，1为开启，0为关闭
                                                              写该寄存器：对应比特为0表示不改变MASK为，对应比特为1表示设置该MASK比特为0，即屏蔽中断.
                                                              
                                                              (送往ACPU的中断复位值为0xffffffff，其他为0) */
    } reg;
} SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_UNION;
#endif
#define SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_level2_intr_maskclr_START  (0)
#define SOC_INTR_HUB_L2_INTR_MASKCLR_S_n_level2_intr_maskclr_END    (31)


/*****************************************************************************
 struct               : SOC_INTR_HUB_L2_INTR_STATUS_S_n_UNION
 struct description   : L2_INTR_STATUS_S_n Register structure definition
                        Address Offset:(n)*0x10+0xC Initial:0x00000000 Width:32
 register description : 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  level2_intr_status : 32; /* bit[0-31]: LEVEL2中断状态寄存器，表示MASK后的中断状态，每个bit对应一根输入中断
                                                             对应bit位为1表示发生中断，为0表示没有发生中断或中断被屏蔽。
                                                             
                                                             n=N寄存器的第Mbit 代表 中断需求汇总中L2_N第Mbit的中断。 */
    } reg;
} SOC_INTR_HUB_L2_INTR_STATUS_S_n_UNION;
#endif
#define SOC_INTR_HUB_L2_INTR_STATUS_S_n_level2_intr_status_START  (0)
#define SOC_INTR_HUB_L2_INTR_STATUS_S_n_level2_intr_status_END    (31)


/*****************************************************************************
 struct               : SOC_INTR_HUB_CTRL_UNION
 struct description   : CTRL Register structure definition
                        Address Offset:0x03fc Initial:0x00000000 Width:32
 register description : INTR_HUB CONTROL Register
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  clk_gt_ctrl : 2;  /* bit[0-1] : Auto clock gating control.
                                                      0x0/0x1: auto clock by hardware;
                                                      0x2/0x3: clock on by software; */
        unsigned int  reserved    : 30; /* bit[2-31]: 保留 */
    } reg;
} SOC_INTR_HUB_CTRL_UNION;
#endif
#define SOC_INTR_HUB_CTRL_clk_gt_ctrl_START  (0)
#define SOC_INTR_HUB_CTRL_clk_gt_ctrl_END    (1)






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

#endif /* end of soc_intr_hub_interface.h */
