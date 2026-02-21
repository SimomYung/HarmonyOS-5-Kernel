/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_ufs_sysctrl_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_UFS_SYSCTRL_INTERFACE_H__
#define __SOC_UFS_SYSCTRL_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) reg_define
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：CRG_UFS_CFG寄存器0。
   位域定义UNION结构:  SOC_UFS_Sysctrl_CRG_UFS_CFG_UNION */
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_ADDR(base)                ((base) + (0x000UL))

/* 寄存器说明：CRG_UFS_CFG寄存器1。
   位域定义UNION结构:  SOC_UFS_Sysctrl_CRG_UFS_CFG1_UNION */
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_ADDR(base)               ((base) + (0x004UL))

/* 寄存器说明：复位控制寄存器0。
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_RESET_CTRL_UNION */
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ADDR(base)             ((base) + (0x008UL))

/* 寄存器说明：UFSAXI_W_QOS_LMTR
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_UNION */
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ADDR(base)          ((base) + (0x00CUL))

/* 寄存器说明：UFSAXI_R_QOS_LMTR
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_UNION */
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ADDR(base)          ((base) + (0x010UL))

/* 寄存器说明：CRG_UFS_STAT 状态寄存器。
   位域定义UNION结构:  SOC_UFS_Sysctrl_CRG_UFS_STAT_UNION */
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_ADDR(base)               ((base) + (0x014UL))

/* 寄存器说明：UFS AXI口控制信号
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_AXI_CTRL_UNION */
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_ADDR(base)               ((base) + (0x018UL))

/* 寄存器说明：memory ctrol signal for UFS internal Memory.
   位域定义UNION结构:  SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_UNION */
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ADDR(base)          ((base) + (0x01CUL))

/* 寄存器说明：ufs_clkrst_bypass
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_UNION */
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ADDR(base)          ((base) + (0x020UL))

/* 寄存器说明：memory ctrol signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_AO_MEMORY_CTRL_UNION */
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ADDR(base)             ((base) + (0x024UL))

/* 寄存器说明：memory ctrol signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_MEMORY_CTRL_UNION */
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ADDR(base)                ((base) + (0x028UL))

/* 寄存器说明：hiufs mphy cfg
   位域定义UNION结构:  SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_UNION */
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_ADDR(base)             ((base) + (0x02CUL))

/* 寄存器说明：debug signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_HIUFS_DEBUG_UNION */
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_ADDR(base)                ((base) + (0x030UL))

/* 寄存器说明：mcu test point signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_UNION */
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_ADDR(base)         ((base) + (0x034UL))

/* 寄存器说明：tp ctrl signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_TESTPOINT_CTRL_UNION */
#define SOC_UFS_Sysctrl_TESTPOINT_CTRL_ADDR(base)             ((base) + (0x038UL))

/* 寄存器说明：enhance属性控制信号
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_UNION */
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_ADDR(base)           ((base) + (0xF00UL))


#else


/* 寄存器说明：CRG_UFS_CFG寄存器0。
   位域定义UNION结构:  SOC_UFS_Sysctrl_CRG_UFS_CFG_UNION */
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_ADDR(base)                ((base) + (0x000))

/* 寄存器说明：CRG_UFS_CFG寄存器1。
   位域定义UNION结构:  SOC_UFS_Sysctrl_CRG_UFS_CFG1_UNION */
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_ADDR(base)               ((base) + (0x004))

/* 寄存器说明：复位控制寄存器0。
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_RESET_CTRL_UNION */
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ADDR(base)             ((base) + (0x008))

/* 寄存器说明：UFSAXI_W_QOS_LMTR
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_UNION */
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ADDR(base)          ((base) + (0x00C))

/* 寄存器说明：UFSAXI_R_QOS_LMTR
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_UNION */
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ADDR(base)          ((base) + (0x010))

/* 寄存器说明：CRG_UFS_STAT 状态寄存器。
   位域定义UNION结构:  SOC_UFS_Sysctrl_CRG_UFS_STAT_UNION */
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_ADDR(base)               ((base) + (0x014))

/* 寄存器说明：UFS AXI口控制信号
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_AXI_CTRL_UNION */
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_ADDR(base)               ((base) + (0x018))

/* 寄存器说明：memory ctrol signal for UFS internal Memory.
   位域定义UNION结构:  SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_UNION */
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ADDR(base)          ((base) + (0x01C))

/* 寄存器说明：ufs_clkrst_bypass
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_UNION */
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ADDR(base)          ((base) + (0x020))

/* 寄存器说明：memory ctrol signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_AO_MEMORY_CTRL_UNION */
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ADDR(base)             ((base) + (0x024))

/* 寄存器说明：memory ctrol signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_MEMORY_CTRL_UNION */
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ADDR(base)                ((base) + (0x028))

/* 寄存器说明：hiufs mphy cfg
   位域定义UNION结构:  SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_UNION */
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_ADDR(base)             ((base) + (0x02C))

/* 寄存器说明：debug signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_HIUFS_DEBUG_UNION */
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_ADDR(base)                ((base) + (0x030))

/* 寄存器说明：mcu test point signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_UNION */
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_ADDR(base)         ((base) + (0x034))

/* 寄存器说明：tp ctrl signal
   位域定义UNION结构:  SOC_UFS_Sysctrl_TESTPOINT_CTRL_UNION */
#define SOC_UFS_Sysctrl_TESTPOINT_CTRL_ADDR(base)             ((base) + (0x038))

/* 寄存器说明：enhance属性控制信号
   位域定义UNION结构:  SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_UNION */
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_ADDR(base)           ((base) + (0xF00))


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
                     (1/1) reg_define
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_CRG_UFS_CFG_UNION
 结构说明  : CRG_UFS_CFG 寄存器结构定义。地址偏移量:0x000，初值:0x00000027，宽度:32
 寄存器说明: CRG_UFS_CFG寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  div_ufs_tick  : 6;  /* bit[0-5]  :  */
        unsigned int  reserved      : 10; /* bit[6-15] :  */
        unsigned int  clkdivmasken1 : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                         只有当clkdivmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。Clkdivmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_UFS_Sysctrl_CRG_UFS_CFG_UNION;
#endif
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_div_ufs_tick_START   (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_div_ufs_tick_END     (5)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_clkdivmasken1_START  (16)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_clkdivmasken1_END    (31)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_CRG_UFS_CFG1_UNION
 结构说明  : CRG_UFS_CFG1 寄存器结构定义。地址偏移量:0x004，初值:0x000007FF，宽度:32
 寄存器说明: CRG_UFS_CFG寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gt_clk_ufs             : 1;  /* bit[0]    :  */
        unsigned int  gt_hclk_ufs_top        : 1;  /* bit[1]    :  */
        unsigned int  gt_pclk_ufs_mphy_apb   : 1;  /* bit[2]    :  */
        unsigned int  gt_clk_ufsphy_cfg      : 1;  /* bit[3]    :  */
        unsigned int  gt_clk_ufs_mphymcu_bus : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_ufs_mphymcu     : 1;  /* bit[5]    :  */
        unsigned int  gt_pclk_ufs_debug      : 1;  /* bit[6]    :  */
        unsigned int  gt_pclk_ufs_djtag_s    : 1;  /* bit[7]    :  */
        unsigned int  gt_clk_ufs_32k         : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_ufs_tick        : 1;  /* bit[9]    :  */
        unsigned int  sc_gt_clk_ufs_tick_div : 1;  /* bit[10]   :  */
        unsigned int  sc_gt_debug_clk        : 1;  /* bit[11]   : 0 default debug clock not output */
        unsigned int  reserved               : 4;  /* bit[12-15]:  */
        unsigned int  clkdivmasken2          : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                  只有当clkdivmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。Clkdivmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_UFS_Sysctrl_CRG_UFS_CFG1_UNION;
#endif
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_START              (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_END                (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_hclk_ufs_top_START         (1)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_hclk_ufs_top_END           (1)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_mphy_apb_START    (2)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_mphy_apb_END      (2)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufsphy_cfg_START       (3)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufsphy_cfg_END         (3)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_bus_START  (4)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_bus_END    (4)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_START      (5)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_END        (5)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_debug_START       (6)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_debug_END         (6)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_djtag_s_START     (7)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_djtag_s_END       (7)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_32k_START          (8)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_32k_END            (8)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_tick_START         (9)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_tick_END           (9)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_clk_ufs_tick_div_START  (10)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_clk_ufs_tick_div_END    (10)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_debug_clk_START         (11)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_debug_clk_END           (11)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_clkdivmasken2_START           (16)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_clkdivmasken2_END             (31)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_UFS_RESET_CTRL_UNION
 结构说明  : UFS_RESET_CTRL 寄存器结构定义。地址偏移量:0x008，初值:0x00008000，宽度:32
 寄存器说明: 复位控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ip_rst_ufs          : 1;  /* bit[0]    :  */
        unsigned int  ip_arst_ufs         : 1;  /* bit[1]    :  */
        unsigned int  ip_prst_ufs_debug   : 1;  /* bit[2]    :  */
        unsigned int  ip_prst_dgtag_s     : 1;  /* bit[3]    :  */
        unsigned int  ip_rst_mphymcu_bus  : 1;  /* bit[4]    :  */
        unsigned int  ip_rst_mphymcu_arc4 : 1;  /* bit[5]    :  */
        unsigned int  ip_rst_mphymcu_19p2 : 1;  /* bit[6]    :  */
        unsigned int  ip_prst_mphy_apb    : 1;  /* bit[7]    :  */
        unsigned int  ip_hrst_ufs_dcdr    : 1;  /* bit[8]    :  */
        unsigned int  reserved            : 6;  /* bit[9-14] :  */
        unsigned int  ufs_device_rst_n    : 1;  /* bit[15]   : 该bit没有使用，使用AO区寄存器控制device reset */
        unsigned int  clkdivmasken        : 16; /* bit[16-31]: 信号每个比特位的使能位：
                                                               只有当clkdivmasken对应的比特位为1'b1，相应的比特位才起作用。clkdivmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_UFS_Sysctrl_UFS_RESET_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_ufs_START           (0)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_ufs_END             (0)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_arst_ufs_START          (1)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_arst_ufs_END            (1)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_ufs_debug_START    (2)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_ufs_debug_END      (2)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_dgtag_s_START      (3)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_dgtag_s_END        (3)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_bus_START   (4)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_bus_END     (4)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_arc4_START  (5)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_arc4_END    (5)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_19p2_START  (6)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_19p2_END    (6)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_mphy_apb_START     (7)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_mphy_apb_END       (7)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_hrst_ufs_dcdr_START     (8)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_hrst_ufs_dcdr_END       (8)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ufs_device_rst_n_START     (15)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ufs_device_rst_n_END       (15)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_clkdivmasken_START         (16)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_clkdivmasken_END           (31)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_UNION
 结构说明  : UFSAXI_W_QOS_LMTR 寄存器结构定义。地址偏移量:0x00C，初值:0x00000000，宽度:32
 寄存器说明: UFSAXI_W_QOS_LMTR
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ufsaxi_w_lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                                    配置值=带宽容限(B)/16；
                                                                    说明：
                                                                    可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                                    例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                                    注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0               : 2;  /* bit[14-15]: 保留。 */
        unsigned int  ufsaxi_w_lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                                    配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                                    例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                                    说明：
                                                                    1.乘256的目的是保留允许平均带宽的高8位小数；
                                                                    2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                                    3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1               : 2;  /* bit[29-30]: 保留。 */
        unsigned int  ufsaxi_w_lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                                    0：禁止；
                                                                    1：使能。 */
    } reg;
} SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_UNION;
#endif
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_saturation_START  (0)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_saturation_END    (13)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_bandwidth_START   (16)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_bandwidth_END     (28)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_en_START          (31)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_UNION
 结构说明  : UFSAXI_R_QOS_LMTR 寄存器结构定义。地址偏移量:0x010，初值:0x00000000，宽度:32
 寄存器说明: UFSAXI_R_QOS_LMTR
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ufsaxi_r_lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                                    配置值=带宽容限(B)/16；
                                                                    说明：
                                                                    可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                                    例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                                    注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0               : 2;  /* bit[14-15]: 保留。 */
        unsigned int  ufsaxi_r_lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                                    配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                                    例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                                    说明：
                                                                    1.乘256的目的是保留允许平均带宽的高8位小数；
                                                                    2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                                    3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1               : 2;  /* bit[29-30]: 保留。 */
        unsigned int  ufsaxi_r_lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                                    0：禁止；
                                                                    1：使能。 */
    } reg;
} SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_UNION;
#endif
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_saturation_START  (0)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_saturation_END    (13)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_bandwidth_START   (16)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_bandwidth_END     (28)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_en_START          (31)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_CRG_UFS_STAT_UNION
 结构说明  : CRG_UFS_STAT 寄存器结构定义。地址偏移量:0x014，初值:0x000003FF，宽度:32
 寄存器说明: CRG_UFS_STAT 状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  crg_ufs_stat : 16; /* bit[0-15] : crg_ufs_stat[15:10]: reserved
                                                        crg_ufs_stat[9]: st_clk_ufs_tick
                                                        crg_ufs_stat[8]: st_clk_ufs_32k
                                                        crg_ufs_stat[7]: st_pclk_ufs_debug
                                                        crg_ufs_stat[6]: st_pclk_ufs_djtag_s
                                                        crg_ufs_stat[5]: st_clk_ufs_mphymcu_bus
                                                        crg_ufs_stat[4]: st_clk_ufs_mphymcu
                                                        crg_ufs_stat[3]: st_pclk_ufs_mphy_apb
                                                        crg_ufs_stat[2]: st_clk_ufsphy_cfg
                                                        crg_ufs_stat[1]: st_hclk_ufs_top
                                                        crg_ufs_stat[0]: st_clk_ufs */
        unsigned int  reserved     : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_UFS_Sysctrl_CRG_UFS_STAT_UNION;
#endif
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_crg_ufs_stat_START  (0)
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_crg_ufs_stat_END    (15)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_UFS_AXI_CTRL_UNION
 结构说明  : UFS_AXI_CTRL 寄存器结构定义。地址偏移量:0x018，初值:0x00000003，宽度:32
 寄存器说明: UFS AXI口控制信号
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_axcache_mask : 2;  /* bit[0-1] : axi cache信号的选择。
                                                          bit1：如果bit0为0，此位才有意义
                                                          0：访问L3 cache
                                                          1：访问system cache
                                                          bit0：
                                                          0：走cache通路，选择访问L3cache 或者system cache
                                                          1：接HCI控制器原始cache信号，默认全0，非cacheable */
        unsigned int  reserved        : 30; /* bit[2-31]:  */
    } reg;
} SOC_UFS_Sysctrl_UFS_AXI_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_sc_axcache_mask_START  (0)
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_sc_axcache_mask_END    (1)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_UNION
 结构说明  : MEMORY_CTRL_D1W2R 寄存器结构定义。地址偏移量:0x01C，初值:0x00000050，宽度:32
 寄存器说明: memory ctrol signal for UFS internal Memory.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ufs_mem_ctrl_d1w2r : 16; /* bit[0-15] : memory ctrol signal for UFS internal Memory. */
        unsigned int  sc_ufspsw_mem_sdo  : 1;  /* bit[16-16]:  */
        unsigned int  reserved           : 15; /* bit[17-31]: -- */
    } reg;
} SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_UNION;
#endif
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ufs_mem_ctrl_d1w2r_START  (0)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ufs_mem_ctrl_d1w2r_END    (15)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_sc_ufspsw_mem_sdo_START   (16)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_sc_ufspsw_mem_sdo_END     (16)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_UNION
 结构说明  : UFS_CLKRST_BYPASS 寄存器结构定义。地址偏移量:0x020，初值:0x00000000，宽度:32
 寄存器说明: ufs_clkrst_bypass
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ufs_ufshc_unipro_cfg_clkrst_bypass : 1;  /* bit[0]   : BP memory ctrol signal for UFS internal Memory. */
        unsigned int  ufs_mphy_cfg_clkrst_bypass         : 1;  /* bit[1]   :  */
        unsigned int  ufs_ahbcfg_clkrst_bypass           : 1;  /* bit[2]   :  */
        unsigned int  ufs_trace_clkrst_bypass            : 1;  /* bit[3]   :  */
        unsigned int  reserved                           : 28; /* bit[4-31]: -- */
    } reg;
} SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ufshc_unipro_cfg_clkrst_bypass_START  (0)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ufshc_unipro_cfg_clkrst_bypass_END    (0)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_mphy_cfg_clkrst_bypass_START          (1)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_mphy_cfg_clkrst_bypass_END            (1)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ahbcfg_clkrst_bypass_START            (2)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ahbcfg_clkrst_bypass_END              (2)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_trace_clkrst_bypass_START             (3)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_trace_clkrst_bypass_END               (3)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_AO_MEMORY_CTRL_UNION
 结构说明  : AO_MEMORY_CTRL 寄存器结构定义。地址偏移量:0x024，初值:0x00005858，宽度:32
 寄存器说明: memory ctrol signal
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ufs_ao_mem_ctrl : 26; /* bit[0-25] :  */
        unsigned int  reserved        : 6;  /* bit[26-31]:  */
    } reg;
} SOC_UFS_Sysctrl_AO_MEMORY_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ufs_ao_mem_ctrl_START  (0)
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ufs_ao_mem_ctrl_END    (25)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_MEMORY_CTRL_UNION
 结构说明  : MEMORY_CTRL 寄存器结构定义。地址偏移量:0x028，初值:0x00005858，宽度:32
 寄存器说明: memory ctrol signal
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ufs_mem_ctrl : 26; /* bit[0-25] :  */
        unsigned int  reserved     : 6;  /* bit[26-31]:  */
    } reg;
} SOC_UFS_Sysctrl_MEMORY_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ufs_mem_ctrl_START  (0)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ufs_mem_ctrl_END    (25)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_UNION
 结构说明  : HIUFS_MPHY_CFG 寄存器结构定义。地址偏移量:0x02C，初值:0x00000002，宽度:32
 寄存器说明: hiufs mphy cfg
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mphy_reg_bypass_40to20 : 1;  /* bit[0]   : 0 default 40bit */
        unsigned int  mphy_ufs_en            : 1;  /* bit[1]   :  */
        unsigned int  mphy_i2c_sel           : 1;  /* bit[2]   : 0 default disable i2c */
        unsigned int  mphymcu_jtag_sel       : 1;  /* bit[3]   :  */
        unsigned int  reserved               : 28; /* bit[4-31]:  */
    } reg;
} SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_UNION;
#endif
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_reg_bypass_40to20_START  (0)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_reg_bypass_40to20_END    (0)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_ufs_en_START             (1)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_ufs_en_END               (1)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_i2c_sel_START            (2)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_i2c_sel_END              (2)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphymcu_jtag_sel_START        (3)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphymcu_jtag_sel_END          (3)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_HIUFS_DEBUG_UNION
 结构说明  : HIUFS_DEBUG 寄存器结构定义。地址偏移量:0x030，初值:0x00000000，宽度:32
 寄存器说明: debug signal
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_remote_hibernate_exit : 1;  /* bit[0]   :  */
        unsigned int  mphy_pll_lock              : 1;  /* bit[1]   :  */
        unsigned int  reserved                   : 30; /* bit[2-31]:  */
    } reg;
} SOC_UFS_Sysctrl_HIUFS_DEBUG_UNION;
#endif
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_intr_remote_hibernate_exit_START  (0)
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_intr_remote_hibernate_exit_END    (0)
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_mphy_pll_lock_START               (1)
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_mphy_pll_lock_END                 (1)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_UNION
 结构说明  : MPHYMCU_TEST_POINT 寄存器结构定义。地址偏移量:0x034，初值:0x00000000，宽度:32
 寄存器说明: mcu test point signal
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mphymcu_test_point : 8;  /* bit[0-7] :  */
        unsigned int  reserved           : 24; /* bit[8-31]:  */
    } reg;
} SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_UNION;
#endif
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_mphymcu_test_point_START  (0)
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_mphymcu_test_point_END    (7)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_TESTPOINT_CTRL_UNION
 结构说明  : TESTPOINT_CTRL 寄存器结构定义。地址偏移量:0x038，初值:0x00000000，宽度:32
 寄存器说明: tp ctrl signal
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_tp_observe_ctrl : 6;  /* bit[0-5] :  */
        unsigned int  reserved           : 26; /* bit[6-31]:  */
    } reg;
} SOC_UFS_Sysctrl_TESTPOINT_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_TESTPOINT_CTRL_sc_tp_observe_ctrl_START  (0)
#define SOC_UFS_Sysctrl_TESTPOINT_CTRL_sc_tp_observe_ctrl_END    (5)


/*****************************************************************************
 结构名    : SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_UNION
 结构说明  : UFS_ENHANCE_CTRL 寄存器结构定义。地址偏移量:0xF00，初值:0x00000000，宽度:32
 寄存器说明: enhance属性控制信号
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_enhanced_op_en : 1;  /* bit[0-0] : enhance属性控制信号，默认关闭 */
        unsigned int  reserved          : 31; /* bit[1-31]:  */
    } reg;
} SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_sc_enhanced_op_en_START  (0)
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_sc_enhanced_op_en_END    (0)






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

#endif /* end of soc_ufs_sysctrl_interface.h */
