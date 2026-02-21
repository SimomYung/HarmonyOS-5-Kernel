/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2021-2021. All rights reserved.
 * Description: soc_ipp_cvdr_interface.h
 * Create: 2021-05-15
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_IPP_CVDR_INTERFACE_H__
#define __SOC_IPP_CVDR_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) CVDR_IPP
 ****************************************************************************/
/* 寄存器说明：CVDR config register.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_ADDR(base)     ((base) + (0x0))

/* 寄存器说明：AXI Write QOS/Pressure configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_ADDR(base) ((base) + (0x4))

/* 寄存器说明：AXI Read QOS/Pressure configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_ADDR(base) ((base) + (0x8))

/* 寄存器说明：Force clock ON
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_ADDR(base)    ((base) + (0xc))

/* 寄存器说明：CVDR debug register enable.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_ADDR(base) ((base) + (0x10))

/* 寄存器说明：CVDR debug register.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_ADDR(base)   ((base) + (0x14))

/* 寄存器说明：DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_DEBUG_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_DEBUG_ADDR(base)        ((base) + (0x18))

/* 寄存器说明：Spare Other RO.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_OTHER_RO_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_OTHER_RO_ADDR(base)     ((base) + (0x1c))

/* 寄存器说明：Spare Other RW.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_OTHER_RW_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_OTHER_RW_ADDR(base)     ((base) + (0x20))

/* 寄存器说明：Initiator read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_ADDR(base)  ((base) + (0x40))

/* 寄存器说明：NR RD Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_ADDR(base) ((base) + (0x44))

/* 寄存器说明：Non-Raster Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_2_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_2_ADDR(base) ((base) + (0x48))

/* 寄存器说明：Initiator read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_ADDR(base)  ((base) + (0x4c))

/* 寄存器说明：NR RD Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_ADDR(base) ((base) + (0x50))

/* 寄存器说明：Non-Raster Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_3_ADDR(base) ((base) + (0x54))

/* 寄存器说明：Initiator read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_ADDR(base)  ((base) + (0x58))

/* 寄存器说明：NR RD Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_ADDR(base) ((base) + (0x5c))

/* 寄存器说明：Non-Raster Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_4_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_4_ADDR(base) ((base) + (0x60))

/* 寄存器说明：Initiator read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_ADDR(base)  ((base) + (0x7c))

/* 寄存器说明：NR RD Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_ADDR(base) ((base) + (0x80))

/* 寄存器说明：Non-Raster Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_7_ADDR(base) ((base) + (0x84))

/* 寄存器说明：Initiator read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_ADDR(base)  ((base) + (0x88))

/* 寄存器说明：NR RD Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_ADDR(base) ((base) + (0x8c))

/* 寄存器说明：Non-Raster Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_8_ADDR(base) ((base) + (0x90))

/* 寄存器说明：Initiator write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_ADDR(base)  ((base) + (0xdc))

/* 寄存器说明：NR WR Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_ADDR(base) ((base) + (0xe0))

/* 寄存器说明：Non-Raster Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_6_ADDR(base) ((base) + (0xe4))

/* 寄存器说明：Initiator write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_ADDR(base)  ((base) + (0xe8))

/* 寄存器说明：NR WR Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_ADDR(base) ((base) + (0xec))

/* 寄存器说明：Non-Raster Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_7_ADDR(base) ((base) + (0xf0))

/* 寄存器说明：Initiator write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_ADDR(base)  ((base) + (0xf4))

/* 寄存器说明：NR WR Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_ADDR(base) ((base) + (0xf8))

/* 寄存器说明：Non-Raster Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_8_ADDR(base) ((base) + (0xfc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_ADDR(base)  ((base) + (0x100))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_ADDR(base)  ((base) + (0x104))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_ADDR(base)  ((base) + (0x108))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_0_ADDR(base) ((base) + (0x10c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_ADDR(base) ((base) + (0x110))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_ADDR(base) ((base) + (0x114))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_ADDR(base) ((base) + (0x118))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_0_ADDR(base) ((base) + (0x11c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_ADDR(base)  ((base) + (0x1c0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_ADDR(base)  ((base) + (0x1c4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_ADDR(base)  ((base) + (0x1c8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_6_ADDR(base) ((base) + (0x1cc))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_ADDR(base) ((base) + (0x1d0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_ADDR(base) ((base) + (0x1d4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_ADDR(base) ((base) + (0x1d8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_6_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_6_ADDR(base) ((base) + (0x1dc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_ADDR(base)  ((base) + (0x1e0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_ADDR(base)  ((base) + (0x1e4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_ADDR(base)  ((base) + (0x1e8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_7_ADDR(base) ((base) + (0x1ec))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_ADDR(base) ((base) + (0x1f0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_ADDR(base) ((base) + (0x1f4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_ADDR(base) ((base) + (0x1f8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_7_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_7_ADDR(base) ((base) + (0x1fc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_ADDR(base)  ((base) + (0x220))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_ADDR(base)  ((base) + (0x224))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_ADDR(base)  ((base) + (0x228))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_9_ADDR(base) ((base) + (0x22c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_ADDR(base) ((base) + (0x230))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_ADDR(base) ((base) + (0x234))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_ADDR(base) ((base) + (0x238))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_9_ADDR(base) ((base) + (0x23c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_ADDR(base) ((base) + (0x240))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_ADDR(base) ((base) + (0x244))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_ADDR(base) ((base) + (0x248))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_10_ADDR(base) ((base) + (0x24c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_ADDR(base) ((base) + (0x250))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_ADDR(base) ((base) + (0x254))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_ADDR(base) ((base) + (0x258))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_10_ADDR(base) ((base) + (0x25c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_ADDR(base) ((base) + (0x280))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_ADDR(base) ((base) + (0x284))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_ADDR(base) ((base) + (0x288))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_12_ADDR(base) ((base) + (0x28c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_ADDR(base) ((base) + (0x290))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_ADDR(base) ((base) + (0x294))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_ADDR(base) ((base) + (0x298))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_12_ADDR(base) ((base) + (0x29c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_ADDR(base) ((base) + (0x2a0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_ADDR(base) ((base) + (0x2a4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_ADDR(base) ((base) + (0x2a8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_13_ADDR(base) ((base) + (0x2ac))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_ADDR(base) ((base) + (0x2b0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_ADDR(base) ((base) + (0x2b4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_ADDR(base) ((base) + (0x2b8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_13_ADDR(base) ((base) + (0x2bc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_ADDR(base) ((base) + (0x2c0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_ADDR(base) ((base) + (0x2c4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_ADDR(base) ((base) + (0x2c8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_14_ADDR(base) ((base) + (0x2cc))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_ADDR(base) ((base) + (0x2d0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_ADDR(base) ((base) + (0x2d4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_ADDR(base) ((base) + (0x2d8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_14_ADDR(base) ((base) + (0x2dc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_ADDR(base) ((base) + (0x2e0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_ADDR(base) ((base) + (0x2e4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_ADDR(base) ((base) + (0x2e8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_15_ADDR(base) ((base) + (0x2ec))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_ADDR(base) ((base) + (0x2f0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_ADDR(base) ((base) + (0x2f4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_ADDR(base) ((base) + (0x2f8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_15_ADDR(base) ((base) + (0x2fc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_ADDR(base) ((base) + (0x300))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_ADDR(base) ((base) + (0x304))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_ADDR(base) ((base) + (0x308))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_16_ADDR(base) ((base) + (0x30c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_ADDR(base) ((base) + (0x310))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_ADDR(base) ((base) + (0x314))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_ADDR(base) ((base) + (0x318))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_16_ADDR(base) ((base) + (0x31c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_ADDR(base) ((base) + (0x320))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_ADDR(base) ((base) + (0x324))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_ADDR(base) ((base) + (0x328))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_17_ADDR(base) ((base) + (0x32c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_ADDR(base) ((base) + (0x330))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_ADDR(base) ((base) + (0x334))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_ADDR(base) ((base) + (0x338))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_17_ADDR(base) ((base) + (0x33c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_ADDR(base) ((base) + (0x340))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_ADDR(base) ((base) + (0x344))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_ADDR(base) ((base) + (0x348))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_18_ADDR(base) ((base) + (0x34c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_ADDR(base) ((base) + (0x350))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_ADDR(base) ((base) + (0x354))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_ADDR(base) ((base) + (0x358))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_18_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_18_ADDR(base) ((base) + (0x35c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_ADDR(base) ((base) + (0x360))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_ADDR(base) ((base) + (0x364))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_ADDR(base) ((base) + (0x368))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_19_ADDR(base) ((base) + (0x36c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_ADDR(base) ((base) + (0x370))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_ADDR(base) ((base) + (0x374))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_ADDR(base) ((base) + (0x378))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_19_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_19_ADDR(base) ((base) + (0x37c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_ADDR(base) ((base) + (0x380))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_ADDR(base) ((base) + (0x384))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_ADDR(base) ((base) + (0x388))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_20_ADDR(base) ((base) + (0x38c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_ADDR(base) ((base) + (0x390))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_ADDR(base) ((base) + (0x394))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_ADDR(base) ((base) + (0x398))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_20_ADDR(base) ((base) + (0x39c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_ADDR(base) ((base) + (0x3a0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_ADDR(base) ((base) + (0x3a4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_ADDR(base) ((base) + (0x3a8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_21_ADDR(base) ((base) + (0x3ac))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_ADDR(base) ((base) + (0x3b0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_ADDR(base) ((base) + (0x3b4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_ADDR(base) ((base) + (0x3b8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_21_ADDR(base) ((base) + (0x3bc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_ADDR(base) ((base) + (0x3c0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_ADDR(base) ((base) + (0x3c4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_ADDR(base) ((base) + (0x3c8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_22_ADDR(base) ((base) + (0x3cc))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_ADDR(base) ((base) + (0x3d0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_ADDR(base) ((base) + (0x3d4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_ADDR(base) ((base) + (0x3d8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_22_ADDR(base) ((base) + (0x3dc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_ADDR(base) ((base) + (0x3e0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_ADDR(base) ((base) + (0x3e4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_ADDR(base) ((base) + (0x3e8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_23_ADDR(base) ((base) + (0x3ec))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_ADDR(base) ((base) + (0x3f0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_ADDR(base) ((base) + (0x3f4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_ADDR(base) ((base) + (0x3f8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_23_ADDR(base) ((base) + (0x3fc))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_ADDR(base) ((base) + (0x400))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_ADDR(base) ((base) + (0x404))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_ADDR(base) ((base) + (0x408))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_24_ADDR(base) ((base) + (0x40c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_ADDR(base) ((base) + (0x410))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_ADDR(base) ((base) + (0x414))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_ADDR(base) ((base) + (0x418))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_24_ADDR(base) ((base) + (0x41c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_ADDR(base) ((base) + (0x420))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_ADDR(base) ((base) + (0x424))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_ADDR(base) ((base) + (0x428))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_25_ADDR(base) ((base) + (0x42c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_ADDR(base) ((base) + (0x430))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_ADDR(base) ((base) + (0x434))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_ADDR(base) ((base) + (0x438))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_25_ADDR(base) ((base) + (0x43c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_ADDR(base) ((base) + (0x440))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_ADDR(base) ((base) + (0x444))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_ADDR(base) ((base) + (0x448))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_26_ADDR(base) ((base) + (0x44c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_ADDR(base) ((base) + (0x450))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_ADDR(base) ((base) + (0x454))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_ADDR(base) ((base) + (0x458))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_26_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_26_ADDR(base) ((base) + (0x45c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_ADDR(base) ((base) + (0x460))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_ADDR(base) ((base) + (0x464))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_ADDR(base) ((base) + (0x468))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_27_ADDR(base) ((base) + (0x46c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_ADDR(base) ((base) + (0x470))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_ADDR(base) ((base) + (0x474))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_ADDR(base) ((base) + (0x478))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_27_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_27_ADDR(base) ((base) + (0x47c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_ADDR(base) ((base) + (0x480))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_ADDR(base) ((base) + (0x484))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_ADDR(base) ((base) + (0x488))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_28_ADDR(base) ((base) + (0x48c))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_ADDR(base) ((base) + (0x490))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_ADDR(base) ((base) + (0x494))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_ADDR(base) ((base) + (0x498))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_28_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_28_ADDR(base) ((base) + (0x49c))

/* 寄存器说明：Video port read Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_ADDR(base) ((base) + (0x4a0))

/* 寄存器说明：Line width generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_ADDR(base) ((base) + (0x4a4))

/* 寄存器说明：Frame height generation.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_ADDR(base) ((base) + (0x4a8))

/* 寄存器说明：AXI frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_29_ADDR(base) ((base) + (0x4ac))

/* 寄存器说明：Line Wrap definition.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_ADDR(base) ((base) + (0x4b0))

/* 寄存器说明：Video port read interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_ADDR(base) ((base) + (0x4b4))

/* 寄存器说明：Video port read Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_ADDR(base) ((base) + (0x4b8))

/* 寄存器说明：Video Port Read DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_29_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_29_ADDR(base) ((base) + (0x4bc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_ADDR(base) ((base) + (0x500))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_ADDR(base) ((base) + (0x504))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_ADDR(base) ((base) + (0x50c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0_ADDR(base) ((base) + (0x510))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_ADDR(base) ((base) + (0x514))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0_ADDR(base) ((base) + (0x518))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_0_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_0_ADDR(base) ((base) + (0x51c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_ADDR(base) ((base) + (0x560))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_ADDR(base) ((base) + (0x564))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_ADDR(base) ((base) + (0x56c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3_ADDR(base) ((base) + (0x570))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_ADDR(base) ((base) + (0x574))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3_ADDR(base) ((base) + (0x578))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_3_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_3_ADDR(base) ((base) + (0x57c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_ADDR(base) ((base) + (0x600))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_ADDR(base) ((base) + (0x604))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_ADDR(base) ((base) + (0x60c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8_ADDR(base) ((base) + (0x610))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_ADDR(base) ((base) + (0x614))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8_ADDR(base) ((base) + (0x618))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_8_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_8_ADDR(base) ((base) + (0x61c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_ADDR(base) ((base) + (0x620))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_ADDR(base) ((base) + (0x624))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_ADDR(base) ((base) + (0x62c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9_ADDR(base) ((base) + (0x630))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_ADDR(base) ((base) + (0x634))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9_ADDR(base) ((base) + (0x638))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_9_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_9_ADDR(base) ((base) + (0x63c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_ADDR(base) ((base) + (0x640))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_ADDR(base) ((base) + (0x644))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_ADDR(base) ((base) + (0x64c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10_ADDR(base) ((base) + (0x650))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_ADDR(base) ((base) + (0x654))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10_ADDR(base) ((base) + (0x658))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_10_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_10_ADDR(base) ((base) + (0x65c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_ADDR(base) ((base) + (0x660))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_ADDR(base) ((base) + (0x664))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_ADDR(base) ((base) + (0x66c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11_ADDR(base) ((base) + (0x670))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_ADDR(base) ((base) + (0x674))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11_ADDR(base) ((base) + (0x678))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_11_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_11_ADDR(base) ((base) + (0x67c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_ADDR(base) ((base) + (0x680))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_ADDR(base) ((base) + (0x684))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_ADDR(base) ((base) + (0x68c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12_ADDR(base) ((base) + (0x690))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_ADDR(base) ((base) + (0x694))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12_ADDR(base) ((base) + (0x698))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_12_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_12_ADDR(base) ((base) + (0x69c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_ADDR(base) ((base) + (0x6a0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_ADDR(base) ((base) + (0x6a4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_ADDR(base) ((base) + (0x6ac))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13_ADDR(base) ((base) + (0x6b0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_ADDR(base) ((base) + (0x6b4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13_ADDR(base) ((base) + (0x6b8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_13_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_13_ADDR(base) ((base) + (0x6bc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_ADDR(base) ((base) + (0x6c0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_ADDR(base) ((base) + (0x6c4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_ADDR(base) ((base) + (0x6cc))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14_ADDR(base) ((base) + (0x6d0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_ADDR(base) ((base) + (0x6d4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14_ADDR(base) ((base) + (0x6d8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_14_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_14_ADDR(base) ((base) + (0x6dc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_ADDR(base) ((base) + (0x6e0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_ADDR(base) ((base) + (0x6e4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_ADDR(base) ((base) + (0x6ec))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15_ADDR(base) ((base) + (0x6f0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_ADDR(base) ((base) + (0x6f4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15_ADDR(base) ((base) + (0x6f8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_15_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_15_ADDR(base) ((base) + (0x6fc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_ADDR(base) ((base) + (0x700))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_ADDR(base) ((base) + (0x704))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_ADDR(base) ((base) + (0x70c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16_ADDR(base) ((base) + (0x710))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_ADDR(base) ((base) + (0x714))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16_ADDR(base) ((base) + (0x718))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_16_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_16_ADDR(base) ((base) + (0x71c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_ADDR(base) ((base) + (0x720))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_ADDR(base) ((base) + (0x724))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_ADDR(base) ((base) + (0x72c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17_ADDR(base) ((base) + (0x730))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_ADDR(base) ((base) + (0x734))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17_ADDR(base) ((base) + (0x738))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_17_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_17_ADDR(base) ((base) + (0x73c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_ADDR(base) ((base) + (0x780))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_ADDR(base) ((base) + (0x784))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_ADDR(base) ((base) + (0x78c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20_ADDR(base) ((base) + (0x790))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_ADDR(base) ((base) + (0x794))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20_ADDR(base) ((base) + (0x798))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_20_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_20_ADDR(base) ((base) + (0x79c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_ADDR(base) ((base) + (0x7a0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_ADDR(base) ((base) + (0x7a4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_ADDR(base) ((base) + (0x7ac))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21_ADDR(base) ((base) + (0x7b0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_ADDR(base) ((base) + (0x7b4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21_ADDR(base) ((base) + (0x7b8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_21_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_21_ADDR(base) ((base) + (0x7bc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_ADDR(base) ((base) + (0x7c0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_ADDR(base) ((base) + (0x7c4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_ADDR(base) ((base) + (0x7cc))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22_ADDR(base) ((base) + (0x7d0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_ADDR(base) ((base) + (0x7d4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22_ADDR(base) ((base) + (0x7d8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_22_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_22_ADDR(base) ((base) + (0x7dc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_ADDR(base) ((base) + (0x7e0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_ADDR(base) ((base) + (0x7e4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_ADDR(base) ((base) + (0x7ec))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23_ADDR(base) ((base) + (0x7f0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_ADDR(base) ((base) + (0x7f4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23_ADDR(base) ((base) + (0x7f8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_23_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_23_ADDR(base) ((base) + (0x7fc))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_ADDR(base) ((base) + (0x800))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_ADDR(base) ((base) + (0x804))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_ADDR(base) ((base) + (0x80c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24_ADDR(base) ((base) + (0x810))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_ADDR(base) ((base) + (0x814))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24_ADDR(base) ((base) + (0x818))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_24_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_24_ADDR(base) ((base) + (0x81c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_ADDR(base) ((base) + (0x820))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_ADDR(base) ((base) + (0x824))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_ADDR(base) ((base) + (0x82c))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25_ADDR(base) ((base) + (0x830))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_ADDR(base) ((base) + (0x834))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25_ADDR(base) ((base) + (0x838))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_25_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_25_ADDR(base) ((base) + (0x83c))

/* 寄存器说明：[] Video port write interface configuration: prefetch or reset or stall capability.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_ADDR(base) ((base) + (0x8c0))

/* 寄存器说明：[] Video port write Access limiter.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_ADDR(base) ((base) + (0x8c4))

/* 寄存器说明：[] AXI line wrap and line stride.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_ADDR(base) ((base) + (0x8cc))

/* 寄存器说明：[] Video port write interface configuration for stripe size (added on V300) - used for big buffers
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30_ADDR(base) ((base) + (0x8d0))

/* 寄存器说明：[] Video port write Configuration.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_ADDR(base) ((base) + (0x8d4))

/* 寄存器说明：[] AXI address Frame start.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_ADDR(base) ((base) + (0x8d8))

/* 寄存器说明：[] Video Port Write DEBUG information.
   位域定义UNION结构:  SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_UNION */
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_ADDR(base) ((base) + (0x8dc))





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
                     (1/1) CVDR_IPP
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_UNION
 结构说明  : CVDR_IPP_CVDR_CFG 寄存器结构定义。地址偏移量:0x0，初值:0x1F3F2000，宽度:32
 寄存器说明: CVDR config register.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  axiwrite_du_threshold  : 6;  /* bit[0-5]  : DataUnit write threshold (from 0 to 64) - AXI burst mode
                                                                  This registers enables to define when the AXI write commands must be sent.
                                                                  If the number of valid DUs is higher than the du_write_threshold

                                                                   then all valid DUs are sent to AXI write interface until no valid DUs are available.. */
        unsigned int  reserved_0             : 2;  /* bit[6-7]  :  */
        unsigned int  du_threshold_reached   : 8;  /* bit[8-15] : configuration register used to limit the impact of some initiators when the system is performing refresh or any other stall on the DRAM. Once number_busy_write_DUs > du_threshold_reached then the VP/NR read or write ports can be stopped (this is replacing the back pressure signal)
                                                                  This register can be used to optimize the CVDR DU usage and reserve some DUs to some initiators. For configuration

                                                                  Default value: 32
                                                                  [note] du_write_threshold shall be set to a value lower than du_threshold_reached - nbr of initiators. [end]. */
        unsigned int  max_axiread_id         : 6;  /* bit[16-21]: Maximum value allowed on the ARID to access the memory.
                                                                  0: only value 0x0 is allowed (1 transaction)
                                                                  1: value 0 and 1 are allowed
                                                                  and so on
                                                                  31: value 0

                                                                  1

                                                                   up to 31 are allowed (32 transactions)
                                                                  ..
                                                                  63: up to 64 AXI RD outstandings -> Max for IPP/ISP
                                                                  [note] ISP supports a maximum of 32 outstandings

                                                                   while IPP supports up to 64.[end]. */
        unsigned int  axi_wr_buffer_behavior : 2;  /* bit[22-23]: Specifies the Axi write buffer behavior (added on v300) when available.
                                                                  0x0: buffer bypassed
                                                                  0x1: buffer always used
                                                                  0x2: buffer used when amount of current outstanding AXI transactions is 1/2 of max_axiwrite_id
                                                                  0x3: buffer used when amount of current outstanding AXI transactions is reaching max_axiwrite_id
                                                                  . */
        unsigned int  max_axiwrite_id        : 6;  /* bit[24-29]: max number of AXI write transactions
                                                                  0: 1 transaction at a time maximum
                                                                  1: 2 transaction at a time maximum
                                                                  and so on
                                                                  64: 64 transactions maximum at a time (ordered). */
        unsigned int  reserved_1             : 2;  /* bit[30-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_axiwrite_du_threshold_START   (0)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_axiwrite_du_threshold_END     (5)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_du_threshold_reached_START    (8)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_du_threshold_reached_END      (15)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_max_axiread_id_START          (16)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_max_axiread_id_END            (21)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_axi_wr_buffer_behavior_START  (22)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_axi_wr_buffer_behavior_END    (23)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_max_axiwrite_id_START         (24)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_max_axiwrite_id_END           (29)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_UNION
 结构说明  : CVDR_IPP_CVDR_WR_QOS_CFG 寄存器结构定义。地址偏移量:0x4，初值:0xF0765432，宽度:32
 寄存器说明: AXI Write QOS/Pressure configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_qos_threshold_01_stop  : 4;  /* bit[0-3]  : same behavior as threshold_11. */
        unsigned int  wr_qos_threshold_01_start : 4;  /* bit[4-7]  : same behavior as threshold_11. */
        unsigned int  wr_qos_threshold_10_stop  : 4;  /* bit[8-11] : same behavior as threshold_11. */
        unsigned int  wr_qos_threshold_10_start : 4;  /* bit[12-15]: same behavior as threshold_11. */
        unsigned int  wr_qos_threshold_11_stop  : 4;  /* bit[16-19]: stop condition for internal Qos 11. */
        unsigned int  wr_qos_threshold_11_start : 4;  /* bit[20-23]: Threshold level used for Qos generation (based on number of DUs in use). Real value = wr_threshold_qos * 8. Once reached

                                                                      then QoS value of 11 is assumed until DU number is bellow wr_qos_threshold_11_stop * 8.
                                                                     When 0: The condition is always true
                                                                     1 : 8 DUs
                                                                     and so on
                                                                     Values from 11 to 15: are disabling the condition since current implementation has 80 DUs possible. */
        unsigned int  reserved                  : 2;  /* bit[24-25]:  */
        unsigned int  wr_qos_min                : 2;  /* bit[26-27]: Minimum QoS value to be generated
                                                                     [note] For CVDR_RT

                                                                      value to program shall be 0x2. For CVDR_SRT

                                                                      value to program shall be 0x0. [end]. */
        unsigned int  wr_qos_max                : 2;  /* bit[28-29]: Maximum QoS value to be generated
                                                                     [note] For CVDR_RT

                                                                      value to progrom shall be 0x3. For CVDR_SRT

                                                                      value to program shall be 0x1. [end]. */
        unsigned int  wr_qos_sr                 : 2;  /* bit[30-31]: 2 bits register to configure the qos value to generate when pressure from SR is asserted. Note that the qos generated will be the highest value possible between the wr_qos_sr and the qos generated based on bumber of DUs in use . */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_01_stop_START   (0)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_01_stop_END     (3)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_01_start_START  (4)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_01_start_END    (7)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_10_stop_START   (8)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_10_stop_END     (11)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_10_start_START  (12)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_10_start_END    (15)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_11_stop_START   (16)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_11_stop_END     (19)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_11_start_START  (20)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_threshold_11_start_END    (23)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_min_START                 (26)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_min_END                   (27)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_max_START                 (28)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_max_END                   (29)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_sr_START                  (30)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_wr_qos_sr_END                    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_UNION
 结构说明  : CVDR_IPP_CVDR_RD_QOS_CFG 寄存器结构定义。地址偏移量:0x8，初值:0xF0122334，宽度:32
 寄存器说明: AXI Read QOS/Pressure configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_qos_threshold_01_stop  : 4;  /* bit[0-3]  : same behavior as threshold_11. */
        unsigned int  rd_qos_threshold_01_start : 4;  /* bit[4-7]  : same behavior as threshold_11. */
        unsigned int  rd_qos_threshold_10_stop  : 4;  /* bit[8-11] : same behavior as threshold_11. */
        unsigned int  rd_qos_threshold_10_start : 4;  /* bit[12-15]: same behavior as threshold_11. */
        unsigned int  rd_qos_threshold_11_stop  : 4;  /* bit[16-19]: stop condition for internal Qos 11. */
        unsigned int  rd_qos_threshold_11_start : 4;  /* bit[20-23]: QOS=11 generated if number of DUs in use is less than the programmed value * 8.
                                                                     When 0: the threshold check is disabled
                                                                     When 7: maximum number of DUs 56. */
        unsigned int  reserved                  : 2;  /* bit[24-25]:  */
        unsigned int  rd_qos_min                : 2;  /* bit[26-27]: Minimum QoS value to be generated
                                                                     [note] For CVDR_RT

                                                                      value to program shall be 0x2. For CVDR_SRT

                                                                      value to program shall be 0x0. [end]. */
        unsigned int  rd_qos_max                : 2;  /* bit[28-29]: Maximum QoS value to be generated
                                                                     [note] For CVDR_RT

                                                                      value to progrom shall be 0x3. For CVDR_SRT

                                                                      value to program shall be 0x1. [end]. */
        unsigned int  rd_qos_sr                 : 2;  /* bit[30-31]: 2 bits register to configure the qos value to generate when pressure from SR is asserted. Note that the qos generated will be the highest value possible between the rd_qos_sr and the qos generated based on bumber of DUs in use . */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_01_stop_START   (0)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_01_stop_END     (3)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_01_start_START  (4)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_01_start_END    (7)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_10_stop_START   (8)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_10_stop_END     (11)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_10_start_START  (12)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_10_start_END    (15)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_11_stop_START   (16)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_11_stop_END     (19)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_11_start_START  (20)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_threshold_11_start_END    (23)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_min_START                 (26)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_min_END                   (27)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_max_START                 (28)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_max_END                   (29)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_sr_START                  (30)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_rd_qos_sr_END                    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_UNION
 结构说明  : CVDR_IPP_FORCE_CLK 寄存器结构定义。地址偏移量:0xc，初值:0x00000000，宽度:32
 寄存器说明: Force clock ON
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  force_vprd_clk_on   : 1;  /* bit[0-0] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_vpwr_clk_on   : 1;  /* bit[1-1] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_nrrd_clk_on   : 1;  /* bit[2-2] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_nrwr_clk_on   : 1;  /* bit[3-3] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_axi_rd_clk_on : 1;  /* bit[4-4] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_axi_wr_clk_on : 1;  /* bit[5-5] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_du_rd_clk_on  : 1;  /* bit[6-6] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_du_wr_clk_on  : 1;  /* bit[7-7] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  force_cfg_clk_on    : 1;  /* bit[8-8] : clock control
                                                              0: local clock gating is controlled by the hardware
                                                              1: local clock gating is disabled (clock is ON). */
        unsigned int  reserved            : 23; /* bit[9-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_vprd_clk_on_START    (0)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_vprd_clk_on_END      (0)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_vpwr_clk_on_START    (1)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_vpwr_clk_on_END      (1)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_nrrd_clk_on_START    (2)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_nrrd_clk_on_END      (2)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_nrwr_clk_on_START    (3)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_nrwr_clk_on_END      (3)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_axi_rd_clk_on_START  (4)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_axi_rd_clk_on_END    (4)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_axi_wr_clk_on_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_axi_wr_clk_on_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_du_rd_clk_on_START   (6)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_du_rd_clk_on_END     (6)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_du_wr_clk_on_START   (7)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_du_wr_clk_on_END     (7)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_cfg_clk_on_START     (8)
#define SOC_IPP_CVDR_CVDR_IPP_FORCE_CLK_force_cfg_clk_on_END       (8)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_UNION
 结构说明  : CVDR_IPP_CVDR_DEBUG_EN 寄存器结构定义。地址偏移量:0x10，初值:0x00000000，宽度:32
 寄存器说明: CVDR debug register enable.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_peak_en : 1;  /* bit[0-0] : Enable the FIFO peak functionality over the write port. */
        unsigned int  reserved_0 : 7;  /* bit[1-7] :  */
        unsigned int  rd_peak_en : 1;  /* bit[8-8] : Enable the FIFO peak functionality over the read port. */
        unsigned int  reserved_1 : 23; /* bit[9-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_wr_peak_en_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_wr_peak_en_END    (0)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_rd_peak_en_START  (8)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_EN_rd_peak_en_END    (8)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_UNION
 结构说明  : CVDR_IPP_CVDR_DEBUG 寄存器结构定义。地址偏移量:0x14，初值:0x00000000，宽度:32
 寄存器说明: CVDR debug register.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_peak  : 8;  /* bit[0-7]  : Peak number of Data Units used for the write functionality. */
        unsigned int  rd_peak  : 8;  /* bit[8-15] : Peak number of Data Units used for the read functionality. */
        unsigned int  reserved : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_wr_peak_START   (0)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_wr_peak_END     (7)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_rd_peak_START   (8)
#define SOC_IPP_CVDR_CVDR_IPP_CVDR_DEBUG_rd_peak_END     (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_DEBUG_UNION
 结构说明  : CVDR_IPP_DEBUG 寄存器结构定义。地址偏移量:0x18，初值:0x00000000，宽度:32
 寄存器说明: DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  debug : 32; /* bit[0-31]: DEBUG information. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_DEBUG_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_DEBUG_debug_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_DEBUG_debug_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_OTHER_RO_UNION
 结构说明  : CVDR_IPP_OTHER_RO 寄存器结构定义。地址偏移量:0x1c，初值:0x00000000，宽度:32
 寄存器说明: Spare Other RO.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  other_ro : 32; /* bit[0-31]: Bit 31:1: unused
                                                   Bit 0: used for AXI flush mechanism (in conjonction with bit 0 in OTHER_RW). */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_OTHER_RO_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_OTHER_RO_other_ro_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_OTHER_RO_other_ro_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_OTHER_RW_UNION
 结构说明  : CVDR_IPP_OTHER_RW 寄存器结构定义。地址偏移量:0x20，初值:0x00000000，宽度:32
 寄存器说明: Spare Other RW.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  other_rw : 32; /* bit[0-31]: Bit 31:1: unused
                                                   Bit 0: used for AXI flush mechanism (in conjonction with bit 0 in OTHER_RO). */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_OTHER_RW_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_OTHER_RW_other_rw_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_OTHER_RW_other_rw_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_UNION
 结构说明  : CVDR_IPP_NR_RD_CFG_2 寄存器结构定义。地址偏移量:0x40，初值:0x00000000，宽度:32
 寄存器说明: Initiator read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 5;  /* bit[0-4]  :  */
        unsigned int  nrrd_allocated_du_2                      : 5;  /* bit[5-9]  : Number of allocated DUs
                                                                                    A maximum of 32 DUs can be allocated per video port. */
        unsigned int  reserved_1                               : 6;  /* bit[10-15]:  */
        unsigned int  nr_rd_stop_enable_du_threshold_reached_2 : 1;  /* bit[16-16]: Please review CVDR_RD_CFG.vprd_stop_enable_du_threshold_reached definition. */
        unsigned int  nr_rd_stop_enable_flux_ctrl_2            : 1;  /* bit[17-17]: Please review CVDR_RD_CFG.vprd_stop_enable_flux_ctrl definition. */
        unsigned int  nr_rd_stop_enable_pressure_2             : 1;  /* bit[18-18]: Please review CVDR_RD_CFG.vprd_stop_enable_pressure definition. */
        unsigned int  reserved_2                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_rd_stop_ok_2                          : 1;  /* bit[24-24]: Status bit relative to the stop sequence.
                                                                                    shall be 0 when nr_wr_stop is 0.
                                                                                    . */
        unsigned int  nr_rd_stop_2                             : 1;  /* bit[25-25]: stop bit

                                                                                     used to stop the interface.
                                                                                    0: the port is not stopped
                                                                                    1: the port is required to be stopped. */
        unsigned int  reserved_3                               : 5;  /* bit[26-30]:  */
        unsigned int  nrrd_enable_2                            : 1;  /* bit[31-31]: Enable or stop the initiator acquisition (at any time). */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nrrd_allocated_du_2_START                       (5)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nrrd_allocated_du_2_END                         (9)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_enable_du_threshold_reached_2_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_enable_du_threshold_reached_2_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_enable_flux_ctrl_2_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_enable_flux_ctrl_2_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_enable_pressure_2_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_enable_pressure_2_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_ok_2_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_ok_2_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_2_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nr_rd_stop_2_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nrrd_enable_2_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_2_nrrd_enable_2_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_UNION
 结构说明  : CVDR_IPP_NR_RD_LIMITER_2 寄存器结构定义。地址偏移量:0x44，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR RD Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrrd_access_limiter_0_2      : 4;  /* bit[0-3]  : CVDR RAM access limiter: maximum number of read 128 bit accesses allowed on a 16 clock cycle window
                                                                        0: 0 access allowed (unexpected in regular SW)
                                                                        1: 1 access maximum allowed
                                                                        and so on
                                                                        15: 15 accesses maximum allowed. */
        unsigned int  nrrd_access_limiter_1_2      : 4;  /* bit[4-7]  : see description of access_limiter_0. */
        unsigned int  nrrd_access_limiter_2_2      : 4;  /* bit[8-11] : see description of access_limiter_0. */
        unsigned int  nrrd_access_limiter_3_2      : 4;  /* bit[12-15]: see description of access_limiter_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrrd_access_limiter_reload_2 : 4;  /* bit[24-27]: Access limiter reload functionality: controls the throughput available for a given initiator over 128 cycles.
                                                                        When 0xF

                                                                         access_limiter value is reloaded every 16 cycles.
                                                                        Bit 0: enables access limiter 0 value to be reloaded on first 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 1: enables access limiter 1 value to be reloaded on second 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 2: enables access limiter 2 value to be reloaded on third 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 3: enables access limiter 3 value to be reloaded on fourth 16 cycles simulation time (modulo 128 cycles). */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_0_2_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_0_2_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_1_2_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_1_2_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_2_2_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_2_2_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_3_2_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_3_2_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_reload_2_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_2_nrrd_access_limiter_reload_2_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_2_UNION
 结构说明  : CVDR_IPP_NR_RD_DEBUG_2 寄存器结构定义。地址偏移量:0x48，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_rd_debug_2 : 32; /* bit[0-31]: Non-Raster Read DEBUG information. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_2_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_2_nr_rd_debug_2_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_2_nr_rd_debug_2_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_UNION
 结构说明  : CVDR_IPP_NR_RD_CFG_3 寄存器结构定义。地址偏移量:0x4c，初值:0x00000000，宽度:32
 寄存器说明: Initiator read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 5;  /* bit[0-4]  :  */
        unsigned int  nrrd_allocated_du_3                      : 5;  /* bit[5-9]  : See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_1                               : 6;  /* bit[10-15]:  */
        unsigned int  nr_rd_stop_enable_du_threshold_reached_3 : 1;  /* bit[16-16]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_flux_ctrl_3            : 1;  /* bit[17-17]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_pressure_3             : 1;  /* bit[18-18]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_2                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_rd_stop_ok_3                          : 1;  /* bit[24-24]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_3                             : 1;  /* bit[25-25]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_3                               : 5;  /* bit[26-30]:  */
        unsigned int  nrrd_enable_3                            : 1;  /* bit[31-31]: See previous definition on NR_RD_CFG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nrrd_allocated_du_3_START                       (5)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nrrd_allocated_du_3_END                         (9)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_enable_du_threshold_reached_3_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_enable_du_threshold_reached_3_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_enable_flux_ctrl_3_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_enable_flux_ctrl_3_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_enable_pressure_3_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_enable_pressure_3_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_ok_3_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_ok_3_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_3_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nr_rd_stop_3_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nrrd_enable_3_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_3_nrrd_enable_3_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_UNION
 结构说明  : CVDR_IPP_NR_RD_LIMITER_3 寄存器结构定义。地址偏移量:0x50，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR RD Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrrd_access_limiter_0_3      : 4;  /* bit[0-3]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_1_3      : 4;  /* bit[4-7]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_2_3      : 4;  /* bit[8-11] : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_3_3      : 4;  /* bit[12-15]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrrd_access_limiter_reload_3 : 4;  /* bit[24-27]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_0_3_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_0_3_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_1_3_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_1_3_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_2_3_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_2_3_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_3_3_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_3_3_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_reload_3_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_3_nrrd_access_limiter_reload_3_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_3_UNION
 结构说明  : CVDR_IPP_NR_RD_DEBUG_3 寄存器结构定义。地址偏移量:0x54，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_rd_debug_3 : 32; /* bit[0-31]: See previous definition on NR_RD_DEBUG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_3_nr_rd_debug_3_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_3_nr_rd_debug_3_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_UNION
 结构说明  : CVDR_IPP_NR_RD_CFG_4 寄存器结构定义。地址偏移量:0x58，初值:0x00000000，宽度:32
 寄存器说明: Initiator read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 5;  /* bit[0-4]  :  */
        unsigned int  nrrd_allocated_du_4                      : 5;  /* bit[5-9]  : See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_1                               : 6;  /* bit[10-15]:  */
        unsigned int  nr_rd_stop_enable_du_threshold_reached_4 : 1;  /* bit[16-16]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_flux_ctrl_4            : 1;  /* bit[17-17]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_pressure_4             : 1;  /* bit[18-18]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_2                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_rd_stop_ok_4                          : 1;  /* bit[24-24]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_4                             : 1;  /* bit[25-25]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_3                               : 5;  /* bit[26-30]:  */
        unsigned int  nrrd_enable_4                            : 1;  /* bit[31-31]: See previous definition on NR_RD_CFG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nrrd_allocated_du_4_START                       (5)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nrrd_allocated_du_4_END                         (9)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_enable_du_threshold_reached_4_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_enable_du_threshold_reached_4_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_enable_flux_ctrl_4_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_enable_flux_ctrl_4_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_enable_pressure_4_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_enable_pressure_4_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_ok_4_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_ok_4_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_4_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nr_rd_stop_4_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nrrd_enable_4_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_nrrd_enable_4_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_UNION
 结构说明  : CVDR_IPP_NR_RD_LIMITER_4 寄存器结构定义。地址偏移量:0x5c，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR RD Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrrd_access_limiter_0_4      : 4;  /* bit[0-3]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_1_4      : 4;  /* bit[4-7]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_2_4      : 4;  /* bit[8-11] : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_3_4      : 4;  /* bit[12-15]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrrd_access_limiter_reload_4 : 4;  /* bit[24-27]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_0_4_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_0_4_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_1_4_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_1_4_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_2_4_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_2_4_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_3_4_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_3_4_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_reload_4_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_nrrd_access_limiter_reload_4_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_4_UNION
 结构说明  : CVDR_IPP_NR_RD_DEBUG_4 寄存器结构定义。地址偏移量:0x60，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_rd_debug_4 : 32; /* bit[0-31]: See previous definition on NR_RD_DEBUG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_4_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_4_nr_rd_debug_4_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_4_nr_rd_debug_4_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_UNION
 结构说明  : CVDR_IPP_NR_RD_CFG_7 寄存器结构定义。地址偏移量:0x7c，初值:0x00000000，宽度:32
 寄存器说明: Initiator read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 5;  /* bit[0-4]  :  */
        unsigned int  nrrd_allocated_du_7                      : 5;  /* bit[5-9]  : See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_1                               : 6;  /* bit[10-15]:  */
        unsigned int  nr_rd_stop_enable_du_threshold_reached_7 : 1;  /* bit[16-16]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_flux_ctrl_7            : 1;  /* bit[17-17]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_pressure_7             : 1;  /* bit[18-18]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_2                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_rd_stop_ok_7                          : 1;  /* bit[24-24]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_7                             : 1;  /* bit[25-25]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_3                               : 5;  /* bit[26-30]:  */
        unsigned int  nrrd_enable_7                            : 1;  /* bit[31-31]: See previous definition on NR_RD_CFG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nrrd_allocated_du_7_START                       (5)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nrrd_allocated_du_7_END                         (9)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_enable_du_threshold_reached_7_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_enable_du_threshold_reached_7_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_enable_flux_ctrl_7_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_enable_flux_ctrl_7_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_enable_pressure_7_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_enable_pressure_7_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_ok_7_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_ok_7_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_7_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nr_rd_stop_7_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nrrd_enable_7_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_7_nrrd_enable_7_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_UNION
 结构说明  : CVDR_IPP_NR_RD_LIMITER_7 寄存器结构定义。地址偏移量:0x80，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR RD Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrrd_access_limiter_0_7      : 4;  /* bit[0-3]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_1_7      : 4;  /* bit[4-7]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_2_7      : 4;  /* bit[8-11] : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_3_7      : 4;  /* bit[12-15]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrrd_access_limiter_reload_7 : 4;  /* bit[24-27]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_0_7_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_0_7_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_1_7_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_1_7_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_2_7_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_2_7_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_3_7_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_3_7_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_reload_7_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_7_nrrd_access_limiter_reload_7_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_7_UNION
 结构说明  : CVDR_IPP_NR_RD_DEBUG_7 寄存器结构定义。地址偏移量:0x84，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_rd_debug_7 : 32; /* bit[0-31]: See previous definition on NR_RD_DEBUG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_7_nr_rd_debug_7_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_7_nr_rd_debug_7_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_UNION
 结构说明  : CVDR_IPP_NR_RD_CFG_8 寄存器结构定义。地址偏移量:0x88，初值:0x00000000，宽度:32
 寄存器说明: Initiator read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 5;  /* bit[0-4]  :  */
        unsigned int  nrrd_allocated_du_8                      : 5;  /* bit[5-9]  : See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_1                               : 6;  /* bit[10-15]:  */
        unsigned int  nr_rd_stop_enable_du_threshold_reached_8 : 1;  /* bit[16-16]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_flux_ctrl_8            : 1;  /* bit[17-17]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_enable_pressure_8             : 1;  /* bit[18-18]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_2                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_rd_stop_ok_8                          : 1;  /* bit[24-24]: See previous definition on NR_RD_CFG_2. */
        unsigned int  nr_rd_stop_8                             : 1;  /* bit[25-25]: See previous definition on NR_RD_CFG_2. */
        unsigned int  reserved_3                               : 5;  /* bit[26-30]:  */
        unsigned int  nrrd_enable_8                            : 1;  /* bit[31-31]: See previous definition on NR_RD_CFG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nrrd_allocated_du_8_START                       (5)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nrrd_allocated_du_8_END                         (9)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_enable_du_threshold_reached_8_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_enable_du_threshold_reached_8_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_enable_flux_ctrl_8_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_enable_flux_ctrl_8_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_enable_pressure_8_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_enable_pressure_8_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_ok_8_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_ok_8_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_8_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nr_rd_stop_8_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nrrd_enable_8_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_8_nrrd_enable_8_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_UNION
 结构说明  : CVDR_IPP_NR_RD_LIMITER_8 寄存器结构定义。地址偏移量:0x8c，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR RD Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrrd_access_limiter_0_8      : 4;  /* bit[0-3]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_1_8      : 4;  /* bit[4-7]  : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_2_8      : 4;  /* bit[8-11] : See previous definition on NR_RD_LIMITER_2. */
        unsigned int  nrrd_access_limiter_3_8      : 4;  /* bit[12-15]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrrd_access_limiter_reload_8 : 4;  /* bit[24-27]: See previous definition on NR_RD_LIMITER_2. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_0_8_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_0_8_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_1_8_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_1_8_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_2_8_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_2_8_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_3_8_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_3_8_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_reload_8_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_8_nrrd_access_limiter_reload_8_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_8_UNION
 结构说明  : CVDR_IPP_NR_RD_DEBUG_8 寄存器结构定义。地址偏移量:0x90，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_rd_debug_8 : 32; /* bit[0-31]: See previous definition on NR_RD_DEBUG_2. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_8_nr_rd_debug_8_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_RD_DEBUG_8_nr_rd_debug_8_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_UNION
 结构说明  : CVDR_IPP_NR_WR_CFG_6 寄存器结构定义。地址偏移量:0xdc，初值:0x00000000，宽度:32
 寄存器说明: Initiator write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  nr_wr_stop_enable_du_threshold_reached_6 : 1;  /* bit[16-16]: Please review CVDR_WR_CFG.vpwr_stop_enable_du_threshold_reached definition. */
        unsigned int  nr_wr_stop_enable_flux_ctrl_6            : 1;  /* bit[17-17]: Please review CVDR_WR_CFG.vpwr_stop_enable_flux_ctrl definition. */
        unsigned int  nr_wr_stop_enable_pressure_6             : 1;  /* bit[18-18]: Please review CVDR_WR_CFG.vpwr_stop_enable_pressure definition. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_wr_stop_ok_6                          : 1;  /* bit[24-24]: Status bit relative to the stop sequence.
                                                                                    shall be 0 when nr_wr_stop is 0.
                                                                                    . */
        unsigned int  nr_wr_stop_6                             : 1;  /* bit[25-25]: stop bit

                                                                                     used to stop the interface.
                                                                                    0: the port is not stopped
                                                                                    1: the port is required to be stopped. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  nrwr_enable_6                            : 1;  /* bit[31-31]: Enable or stop the initiator acquisition (at any time). */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_enable_du_threshold_reached_6_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_enable_du_threshold_reached_6_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_enable_flux_ctrl_6_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_enable_flux_ctrl_6_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_enable_pressure_6_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_enable_pressure_6_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_ok_6_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_ok_6_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_6_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nr_wr_stop_6_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nrwr_enable_6_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_6_nrwr_enable_6_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_UNION
 结构说明  : CVDR_IPP_NR_WR_LIMITER_6 寄存器结构定义。地址偏移量:0xe0，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR WR Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrwr_access_limiter_0_6      : 4;  /* bit[0-3]  : CVDR RAM access limiter: maximum number of write 128 bit accesses allowed on a 16 clock cycle window
                                                                        0: no access limiter (all requests are accepted - unexpected in regular SW) [this value is only authorized if the reload register is set to 1]
                                                                        1: 1 access maximum allowed
                                                                        and so on
                                                                        15: 15 accesses maximum allowed. */
        unsigned int  nrwr_access_limiter_1_6      : 4;  /* bit[4-7]  : see description of access_limiter_0. */
        unsigned int  nrwr_access_limiter_2_6      : 4;  /* bit[8-11] : see description of access_limiter_0. */
        unsigned int  nrwr_access_limiter_3_6      : 4;  /* bit[12-15]: see description of access_limiter_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrwr_access_limiter_reload_6 : 4;  /* bit[24-27]: Access limiter reload functionality: controls the throughput available for a given initiator over 128 cycles.
                                                                        When 0xF

                                                                         access_limiter value is reloaded every 16 cycles.
                                                                        Bit 0: enables access limiter 0 value to be reloaded on first 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 1: enables access limiter 1 value to be reloaded on second 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 2: enables access limiter 2 value to be reloaded on third 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 3: enables access limiter 3 value to be reloaded on fourth 16 cycles simulation time (modulo 128 cycles). */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_0_6_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_0_6_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_1_6_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_1_6_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_2_6_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_2_6_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_3_6_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_3_6_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_reload_6_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_6_nrwr_access_limiter_reload_6_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_6_UNION
 结构说明  : CVDR_IPP_NR_WR_DEBUG_6 寄存器结构定义。地址偏移量:0xe4，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_wr_debug_6 : 32; /* bit[0-31]: Non-Raster Write DEBUG information. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_6_nr_wr_debug_6_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_6_nr_wr_debug_6_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_UNION
 结构说明  : CVDR_IPP_NR_WR_CFG_7 寄存器结构定义。地址偏移量:0xe8，初值:0x00000000，宽度:32
 寄存器说明: Initiator write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  nr_wr_stop_enable_du_threshold_reached_7 : 1;  /* bit[16-16]: See previous definition on NR_WR_CFG_6. */
        unsigned int  nr_wr_stop_enable_flux_ctrl_7            : 1;  /* bit[17-17]: See previous definition on NR_WR_CFG_6. */
        unsigned int  nr_wr_stop_enable_pressure_7             : 1;  /* bit[18-18]: See previous definition on NR_WR_CFG_6. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_wr_stop_ok_7                          : 1;  /* bit[24-24]: See previous definition on NR_WR_CFG_6. */
        unsigned int  nr_wr_stop_7                             : 1;  /* bit[25-25]: See previous definition on NR_WR_CFG_6. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  nrwr_enable_7                            : 1;  /* bit[31-31]: See previous definition on NR_WR_CFG_6. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_enable_du_threshold_reached_7_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_enable_du_threshold_reached_7_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_enable_flux_ctrl_7_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_enable_flux_ctrl_7_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_enable_pressure_7_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_enable_pressure_7_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_ok_7_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_ok_7_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_7_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nr_wr_stop_7_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nrwr_enable_7_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_7_nrwr_enable_7_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_UNION
 结构说明  : CVDR_IPP_NR_WR_LIMITER_7 寄存器结构定义。地址偏移量:0xec，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR WR Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrwr_access_limiter_0_7      : 4;  /* bit[0-3]  : See previous definition on NR_WR_LIMITER_6. */
        unsigned int  nrwr_access_limiter_1_7      : 4;  /* bit[4-7]  : See previous definition on NR_WR_LIMITER_6. */
        unsigned int  nrwr_access_limiter_2_7      : 4;  /* bit[8-11] : See previous definition on NR_WR_LIMITER_6. */
        unsigned int  nrwr_access_limiter_3_7      : 4;  /* bit[12-15]: See previous definition on NR_WR_LIMITER_6. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrwr_access_limiter_reload_7 : 4;  /* bit[24-27]: See previous definition on NR_WR_LIMITER_6. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_0_7_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_0_7_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_1_7_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_1_7_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_2_7_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_2_7_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_3_7_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_3_7_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_reload_7_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_7_nrwr_access_limiter_reload_7_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_7_UNION
 结构说明  : CVDR_IPP_NR_WR_DEBUG_7 寄存器结构定义。地址偏移量:0xf0，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_wr_debug_7 : 32; /* bit[0-31]: See previous definition on NR_WR_DEBUG_6. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_7_nr_wr_debug_7_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_7_nr_wr_debug_7_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_UNION
 结构说明  : CVDR_IPP_NR_WR_CFG_8 寄存器结构定义。地址偏移量:0xf4，初值:0x00000000，宽度:32
 寄存器说明: Initiator write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  nr_wr_stop_enable_du_threshold_reached_8 : 1;  /* bit[16-16]: See previous definition on NR_WR_CFG_6. */
        unsigned int  nr_wr_stop_enable_flux_ctrl_8            : 1;  /* bit[17-17]: See previous definition on NR_WR_CFG_6. */
        unsigned int  nr_wr_stop_enable_pressure_8             : 1;  /* bit[18-18]: See previous definition on NR_WR_CFG_6. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  nr_wr_stop_ok_8                          : 1;  /* bit[24-24]: See previous definition on NR_WR_CFG_6. */
        unsigned int  nr_wr_stop_8                             : 1;  /* bit[25-25]: See previous definition on NR_WR_CFG_6. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  nrwr_enable_8                            : 1;  /* bit[31-31]: See previous definition on NR_WR_CFG_6. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_enable_du_threshold_reached_8_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_enable_du_threshold_reached_8_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_enable_flux_ctrl_8_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_enable_flux_ctrl_8_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_enable_pressure_8_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_enable_pressure_8_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_ok_8_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_ok_8_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_8_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nr_wr_stop_8_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nrwr_enable_8_START                             (31)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_CFG_8_nrwr_enable_8_END                               (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_UNION
 结构说明  : CVDR_IPP_NR_WR_LIMITER_8 寄存器结构定义。地址偏移量:0xf8，初值:0x0F00FFFF，宽度:32
 寄存器说明: NR WR Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nrwr_access_limiter_0_8      : 4;  /* bit[0-3]  : See previous definition on NR_WR_LIMITER_6. */
        unsigned int  nrwr_access_limiter_1_8      : 4;  /* bit[4-7]  : See previous definition on NR_WR_LIMITER_6. */
        unsigned int  nrwr_access_limiter_2_8      : 4;  /* bit[8-11] : See previous definition on NR_WR_LIMITER_6. */
        unsigned int  nrwr_access_limiter_3_8      : 4;  /* bit[12-15]: See previous definition on NR_WR_LIMITER_6. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  nrwr_access_limiter_reload_8 : 4;  /* bit[24-27]: See previous definition on NR_WR_LIMITER_6. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_0_8_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_0_8_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_1_8_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_1_8_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_2_8_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_2_8_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_3_8_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_3_8_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_reload_8_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_LIMITER_8_nrwr_access_limiter_reload_8_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_8_UNION
 结构说明  : CVDR_IPP_NR_WR_DEBUG_8 寄存器结构定义。地址偏移量:0xfc，初值:0x00000000，宽度:32
 寄存器说明: Non-Raster Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nr_wr_debug_8 : 32; /* bit[0-31]: See previous definition on NR_WR_DEBUG_6. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_8_nr_wr_debug_8_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_NR_WR_DEBUG_8_nr_wr_debug_8_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_0 寄存器结构定义。地址偏移量:0x100，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_0    : 5;  /* bit[0-4]  : Pixel formats supported (see ref. [1])
                                                                   0:3 : 1/2/4/8 PF8
                                                                   4:7 : 1/2/4/8 PF10
                                                                   8:11 : 1/2/4/8 PF12
                                                                   12:15 : 1/2/4/8 PF14
                                                                   16:24 : NA
                                                                   25 : 3PF8
                                                                   26 : 6PF12 (72 bits)
                                                                   27 : 6PF16 (96 bits)
                                                                   28 : D32 (or 2PF16)
                                                                   29 : D48
                                                                   30 : D64
                                                                   31 : D128. */
        unsigned int  vprd_pixel_expansion_0 : 1;  /* bit[5-5]  : Pixel expansion can be used (see ref. [1]). */
        unsigned int  vprd_allocated_du_0    : 5;  /* bit[6-10] : Number of allocated DUs
                                                                  A maximum of 32 DUs can be allocated per video port. */
        unsigned int  reserved               : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_0       : 19; /* bit[13-31]: last 32KB page reserved to read the data. This page is not necessarely prefetched but is the last page reserved by the Software.
                                                                  The CVDR must prefetch the page used by the frame start (back to initial - in case of continuous mode) once the last page is equal to vprd_last_page.
                                                                  This info must be aligned with the frame size (SW info) and the vprd_line_wrap info (SW and HW) to be aligned with buffer size reserved by the SW.
                                                                  [note] change compared to V160 is that ISP interface is now 34 bits while was 32 bits before. 2 bits added on that register.
                                                                  In wrap mode

                                                                   the value is frame_start_address + line_wraps*line_stride (Used for SystemCache)[end]. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_pixel_format_0_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_pixel_format_0_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_pixel_expansion_0_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_pixel_expansion_0_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_allocated_du_0_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_allocated_du_0_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_last_page_0_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_0_vprd_last_page_0_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_0 寄存器结构定义。地址偏移量:0x104，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_0           : 15; /* bit[0-14] : number of pixels per line (up to 32K pixels). */
        unsigned int  reserved_0                 : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_0 : 8;  /* bit[16-23]: Horizontal blanking (number of clock cycles between 2 lines)
                                                                      up to 256 clock cycles. */
        unsigned int  reserved_1                 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_vprd_line_size_0_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_vprd_line_size_0_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_vprd_horizontal_blanking_0_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_0_vprd_horizontal_blanking_0_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_0 寄存器结构定义。地址偏移量:0x108，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_0        : 15; /* bit[0-14] : number of lines per frame (up to 32K lines). */
        unsigned int  reserved_0               : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_0 : 8;  /* bit[16-23]: Vertical blanking (number of dummy lines between 2 frames)
                                                                    up to 256 lines. */
        unsigned int  reserved_1               : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_vprd_frame_size_0_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_vprd_frame_size_0_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_vprd_vertical_blanking_0_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_0_vprd_vertical_blanking_0_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_0_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_0 寄存器结构定义。地址偏移量:0x10c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved               : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_0 : 30; /* bit[2-31]: Start address of the frame (in 16 bytes boundary).
                                                                 Since we are 128-bits

                                                                  the 4 lowest bits on the ISP AXI address are always 0.
                                                                 vprd_axi_frame_start is aligned to MSB of ISP AXI address.
                                                                 [note] change compared to V160 is that ISP interface is now 34 bits while was 32 bits before. 2 bits added on that register.[end]. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_0_vprd_axi_frame_start_0_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_0_vprd_axi_frame_start_0_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_0 寄存器结构定义。地址偏移量:0x110，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_0 : 14; /* bit[0-13] : Stride to use on line starts (aligned to a 16 byte boundary: Max stride of 256 KBytes)
                                                              0: line stride disabled (continuous) (continuous mode : next line will start on a 128 bytes boundary)
                                                              1: 2*16 bytes
                                                              and so on
                                                              16383: 16384*16 bytes. */
        unsigned int  reserved           : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_0   : 14; /* bit[18-31]: Line wrap: wrap to frame start location (14 bits)
                                                              0: wrap every line
                                                              1: wrap every 2 lines
                                                              and so on
                                                              0x3FFE: wrap every 16K-1 lines
                                                              0x3FFF: no wrap. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_vprd_line_stride_0_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_vprd_line_stride_0_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_vprd_line_wrap_0_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_0_vprd_line_wrap_0_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_0 寄存器结构定义。地址偏移量:0x114，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_0 : 1;  /* bit[16-16]: Please review CVDR_WR_CFG.vpwr_stop_enable_du_trheshold_reached definition. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_0            : 1;  /* bit[17-17]: Enable bit used to enable flux_ctrl_r to be taken into account.
                                                                                    0: the port can not be stopped based on flux_ctrl_r
                                                                                    1: the port will be stopped when flux_ctrl_r = 1.
                                                                                    [note] 1 for Non-real time port. [end]. */
        unsigned int  vp_rd_stop_enable_pressure_0             : 1;  /* bit[18-18]: Please review CVDR_WR_CFG.vp_wr_stop_enable_pressure definition. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_0                          : 1;  /* bit[24-24]: Status bit relative to the stop sequence.
                                                                                    shall be 0 when nr_wr_stop is 0.
                                                                                    . */
        unsigned int  vp_rd_stop_0                             : 1;  /* bit[25-25]: stop bit

                                                                                     used to stop the interface.
                                                                                    0: the port is not stopped
                                                                                    1: the port is required to be stopped. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_0                   : 1;  /* bit[31-31]: Enables to bypass the prefetch mechanism.
                                                                                    0: prefetch functionality is enabled
                                                                                    1: prefetch functionality is bypassed (Index ID is always vprd_index_id0). */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_enable_du_threshold_reached_0_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_enable_du_threshold_reached_0_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_enable_flux_ctrl_0_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_enable_flux_ctrl_0_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_enable_pressure_0_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_enable_pressure_0_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_ok_0_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_ok_0_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_0_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vp_rd_stop_0_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vprd_prefetch_bypass_0_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_0_vprd_prefetch_bypass_0_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_0 寄存器结构定义。地址偏移量:0x118，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_0      : 4;  /* bit[0-3]  : CVDR RAM access limiter: maximum number of read 128 bit accesses allowed on a 16 clock cycle window
                                                                        0: 0 access allowed (unexpected in regular SW)
                                                                        1: 1 access maximum allowed
                                                                        and so on
                                                                        15: 15 accesses maximum allowed. */
        unsigned int  vprd_access_limiter_1_0      : 4;  /* bit[4-7]  : see description of access_limiter_0. */
        unsigned int  vprd_access_limiter_2_0      : 4;  /* bit[8-11] : see description of access_limiter_0. */
        unsigned int  vprd_access_limiter_3_0      : 4;  /* bit[12-15]: see description of access_limiter_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_0 : 4;  /* bit[24-27]: Access limiter reload functionality: controls the throughput available for a given initiator over 128 cycles.
                                                                        When 0xF

                                                                         access_limiter value is reloaded every 16 cycles.
                                                                        Bit 0: enables access limiter 0 value to be reloaded on first 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 1: enables access limiter 1 value to be reloaded on second 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 2: enables access limiter 2 value to be reloaded on third 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 3: enables access limiter 3 value to be reloaded on fourth 16 cycles simulation time (modulo 128 cycles)
                                                                        To bypass limiters

                                                                         reload must be 0xF

                                                                         and all limiters at 0x0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_0_0_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_0_0_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_1_0_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_1_0_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_2_0_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_2_0_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_3_0_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_3_0_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_reload_0_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_0_vprd_access_limiter_reload_0_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_0_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_0 寄存器结构定义。地址偏移量:0x11c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_0 : 32; /* bit[0-31]: Video Port Read DEBUG information. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_0_vp_rd_debug_0_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_0_vp_rd_debug_0_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_6 寄存器结构定义。地址偏移量:0x1c0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_6    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_6 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_6    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved               : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_6       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_pixel_format_6_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_pixel_format_6_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_pixel_expansion_6_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_pixel_expansion_6_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_allocated_du_6_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_allocated_du_6_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_last_page_6_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_6_vprd_last_page_6_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_6 寄存器结构定义。地址偏移量:0x1c4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_6           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                 : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_6 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_vprd_line_size_6_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_vprd_line_size_6_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_vprd_horizontal_blanking_6_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_6_vprd_horizontal_blanking_6_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_6 寄存器结构定义。地址偏移量:0x1c8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_6        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0               : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_6 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1               : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_vprd_frame_size_6_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_vprd_frame_size_6_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_vprd_vertical_blanking_6_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_6_vprd_vertical_blanking_6_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_6_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_6 寄存器结构定义。地址偏移量:0x1cc，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved               : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_6 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_6_vprd_axi_frame_start_6_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_6_vprd_axi_frame_start_6_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_6 寄存器结构定义。地址偏移量:0x1d0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_6 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved           : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_6   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_vprd_line_stride_6_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_vprd_line_stride_6_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_vprd_line_wrap_6_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_6_vprd_line_wrap_6_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_6 寄存器结构定义。地址偏移量:0x1d4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_6 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_6            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_6             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_6                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_6                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_6                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_enable_du_threshold_reached_6_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_enable_du_threshold_reached_6_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_enable_flux_ctrl_6_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_enable_flux_ctrl_6_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_enable_pressure_6_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_enable_pressure_6_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_ok_6_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_ok_6_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_6_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vp_rd_stop_6_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vprd_prefetch_bypass_6_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_6_vprd_prefetch_bypass_6_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_6 寄存器结构定义。地址偏移量:0x1d8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_6      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_6      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_6      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_6      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_6 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_0_6_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_0_6_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_1_6_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_1_6_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_2_6_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_2_6_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_3_6_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_3_6_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_reload_6_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_6_vprd_access_limiter_reload_6_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_6_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_6 寄存器结构定义。地址偏移量:0x1dc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_6 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_6_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_6_vp_rd_debug_6_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_6_vp_rd_debug_6_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_7 寄存器结构定义。地址偏移量:0x1e0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_7    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_7 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_7    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved               : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_7       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_pixel_format_7_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_pixel_format_7_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_pixel_expansion_7_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_pixel_expansion_7_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_allocated_du_7_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_allocated_du_7_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_last_page_7_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_7_vprd_last_page_7_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_7 寄存器结构定义。地址偏移量:0x1e4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_7           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                 : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_7 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_vprd_line_size_7_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_vprd_line_size_7_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_vprd_horizontal_blanking_7_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_7_vprd_horizontal_blanking_7_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_7 寄存器结构定义。地址偏移量:0x1e8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_7        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0               : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_7 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1               : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_vprd_frame_size_7_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_vprd_frame_size_7_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_vprd_vertical_blanking_7_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_7_vprd_vertical_blanking_7_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_7_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_7 寄存器结构定义。地址偏移量:0x1ec，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved               : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_7 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_7_vprd_axi_frame_start_7_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_7_vprd_axi_frame_start_7_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_7 寄存器结构定义。地址偏移量:0x1f0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_7 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved           : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_7   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_vprd_line_stride_7_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_vprd_line_stride_7_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_vprd_line_wrap_7_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_7_vprd_line_wrap_7_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_7 寄存器结构定义。地址偏移量:0x1f4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_7 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_7            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_7             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_7                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_7                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_7                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_enable_du_threshold_reached_7_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_enable_du_threshold_reached_7_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_enable_flux_ctrl_7_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_enable_flux_ctrl_7_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_enable_pressure_7_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_enable_pressure_7_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_ok_7_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_ok_7_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_7_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vp_rd_stop_7_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vprd_prefetch_bypass_7_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_7_vprd_prefetch_bypass_7_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_7 寄存器结构定义。地址偏移量:0x1f8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_7      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_7      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_7      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_7      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_7 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_0_7_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_0_7_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_1_7_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_1_7_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_2_7_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_2_7_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_3_7_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_3_7_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_reload_7_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_7_vprd_access_limiter_reload_7_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_7_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_7 寄存器结构定义。地址偏移量:0x1fc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_7 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_7_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_7_vp_rd_debug_7_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_7_vp_rd_debug_7_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_9 寄存器结构定义。地址偏移量:0x220，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_9    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_9 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_9    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved               : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_9       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_pixel_format_9_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_pixel_format_9_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_pixel_expansion_9_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_pixel_expansion_9_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_allocated_du_9_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_allocated_du_9_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_last_page_9_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_9_vprd_last_page_9_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_9 寄存器结构定义。地址偏移量:0x224，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_9           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                 : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_9 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_vprd_line_size_9_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_vprd_line_size_9_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_vprd_horizontal_blanking_9_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_9_vprd_horizontal_blanking_9_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_9 寄存器结构定义。地址偏移量:0x228，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_9        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0               : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_9 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1               : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_vprd_frame_size_9_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_vprd_frame_size_9_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_vprd_vertical_blanking_9_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_9_vprd_vertical_blanking_9_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_9_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_9 寄存器结构定义。地址偏移量:0x22c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved               : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_9 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_9_vprd_axi_frame_start_9_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_9_vprd_axi_frame_start_9_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_9 寄存器结构定义。地址偏移量:0x230，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_9 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved           : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_9   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_vprd_line_stride_9_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_vprd_line_stride_9_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_vprd_line_wrap_9_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_9_vprd_line_wrap_9_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_9 寄存器结构定义。地址偏移量:0x234，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_9 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_9            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_9             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_9                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_9                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_9                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_enable_du_threshold_reached_9_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_enable_du_threshold_reached_9_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_enable_flux_ctrl_9_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_enable_flux_ctrl_9_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_enable_pressure_9_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_enable_pressure_9_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_ok_9_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_ok_9_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_9_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vp_rd_stop_9_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vprd_prefetch_bypass_9_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_9_vprd_prefetch_bypass_9_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_9 寄存器结构定义。地址偏移量:0x238，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_9      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_9      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_9      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_9      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_9 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_0_9_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_0_9_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_1_9_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_1_9_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_2_9_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_2_9_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_3_9_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_3_9_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_reload_9_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_9_vprd_access_limiter_reload_9_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_9_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_9 寄存器结构定义。地址偏移量:0x23c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_9 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_9_vp_rd_debug_9_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_9_vp_rd_debug_9_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_10 寄存器结构定义。地址偏移量:0x240，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_10    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_10 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_10    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_10       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_pixel_format_10_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_pixel_format_10_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_pixel_expansion_10_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_pixel_expansion_10_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_allocated_du_10_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_allocated_du_10_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_last_page_10_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_10_vprd_last_page_10_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_10 寄存器结构定义。地址偏移量:0x244，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_10           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_10 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_vprd_line_size_10_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_vprd_line_size_10_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_vprd_horizontal_blanking_10_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_10_vprd_horizontal_blanking_10_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_10 寄存器结构定义。地址偏移量:0x248，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_10        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_10 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_vprd_frame_size_10_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_vprd_frame_size_10_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_vprd_vertical_blanking_10_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_10_vprd_vertical_blanking_10_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_10_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_10 寄存器结构定义。地址偏移量:0x24c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_10 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_10_vprd_axi_frame_start_10_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_10_vprd_axi_frame_start_10_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_10 寄存器结构定义。地址偏移量:0x250，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_10 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_10   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_vprd_line_stride_10_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_vprd_line_stride_10_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_vprd_line_wrap_10_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_10_vprd_line_wrap_10_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_10 寄存器结构定义。地址偏移量:0x254，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_10 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_10            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_10             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_10                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_10                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_10                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_enable_du_threshold_reached_10_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_enable_du_threshold_reached_10_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_enable_flux_ctrl_10_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_enable_flux_ctrl_10_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_enable_pressure_10_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_enable_pressure_10_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_ok_10_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_ok_10_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_10_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vp_rd_stop_10_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vprd_prefetch_bypass_10_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_10_vprd_prefetch_bypass_10_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_10 寄存器结构定义。地址偏移量:0x258，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_10      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_10      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_10      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_10      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_10 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_0_10_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_0_10_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_1_10_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_1_10_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_2_10_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_2_10_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_3_10_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_3_10_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_reload_10_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_10_vprd_access_limiter_reload_10_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_10_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_10 寄存器结构定义。地址偏移量:0x25c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_10 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_10_vp_rd_debug_10_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_10_vp_rd_debug_10_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_12 寄存器结构定义。地址偏移量:0x280，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_12    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_12 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_12    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_12       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_pixel_format_12_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_pixel_format_12_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_pixel_expansion_12_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_pixel_expansion_12_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_allocated_du_12_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_allocated_du_12_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_last_page_12_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_12_vprd_last_page_12_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_12 寄存器结构定义。地址偏移量:0x284，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_12           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_12 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_vprd_line_size_12_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_vprd_line_size_12_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_vprd_horizontal_blanking_12_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_12_vprd_horizontal_blanking_12_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_12 寄存器结构定义。地址偏移量:0x288，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_12        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_12 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_vprd_frame_size_12_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_vprd_frame_size_12_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_vprd_vertical_blanking_12_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_12_vprd_vertical_blanking_12_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_12_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_12 寄存器结构定义。地址偏移量:0x28c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_12 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_12_vprd_axi_frame_start_12_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_12_vprd_axi_frame_start_12_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_12 寄存器结构定义。地址偏移量:0x290，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_12 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_12   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_vprd_line_stride_12_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_vprd_line_stride_12_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_vprd_line_wrap_12_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_12_vprd_line_wrap_12_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_12 寄存器结构定义。地址偏移量:0x294，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_12 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_12            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_12             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_12                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_12                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_12                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_enable_du_threshold_reached_12_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_enable_du_threshold_reached_12_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_enable_flux_ctrl_12_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_enable_flux_ctrl_12_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_enable_pressure_12_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_enable_pressure_12_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_ok_12_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_ok_12_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_12_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vp_rd_stop_12_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vprd_prefetch_bypass_12_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_12_vprd_prefetch_bypass_12_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_12 寄存器结构定义。地址偏移量:0x298，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_12      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_12      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_12      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_12      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_12 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_0_12_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_0_12_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_1_12_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_1_12_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_2_12_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_2_12_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_3_12_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_3_12_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_reload_12_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_12_vprd_access_limiter_reload_12_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_12_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_12 寄存器结构定义。地址偏移量:0x29c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_12 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_12_vp_rd_debug_12_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_12_vp_rd_debug_12_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_13 寄存器结构定义。地址偏移量:0x2a0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_13    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_13 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_13    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_13       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_pixel_format_13_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_pixel_format_13_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_pixel_expansion_13_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_pixel_expansion_13_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_allocated_du_13_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_allocated_du_13_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_last_page_13_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_13_vprd_last_page_13_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_13 寄存器结构定义。地址偏移量:0x2a4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_13           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_13 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_vprd_line_size_13_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_vprd_line_size_13_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_vprd_horizontal_blanking_13_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_13_vprd_horizontal_blanking_13_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_13 寄存器结构定义。地址偏移量:0x2a8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_13        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_13 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_vprd_frame_size_13_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_vprd_frame_size_13_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_vprd_vertical_blanking_13_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_13_vprd_vertical_blanking_13_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_13_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_13 寄存器结构定义。地址偏移量:0x2ac，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_13 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_13_vprd_axi_frame_start_13_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_13_vprd_axi_frame_start_13_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_13 寄存器结构定义。地址偏移量:0x2b0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_13 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_13   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_vprd_line_stride_13_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_vprd_line_stride_13_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_vprd_line_wrap_13_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_13_vprd_line_wrap_13_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_13 寄存器结构定义。地址偏移量:0x2b4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_13 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_13            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_13             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_13                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_13                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_13                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_enable_du_threshold_reached_13_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_enable_du_threshold_reached_13_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_enable_flux_ctrl_13_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_enable_flux_ctrl_13_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_enable_pressure_13_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_enable_pressure_13_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_ok_13_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_ok_13_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_13_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vp_rd_stop_13_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vprd_prefetch_bypass_13_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_13_vprd_prefetch_bypass_13_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_13 寄存器结构定义。地址偏移量:0x2b8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_13      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_13      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_13      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_13      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_13 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_0_13_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_0_13_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_1_13_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_1_13_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_2_13_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_2_13_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_3_13_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_3_13_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_reload_13_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_13_vprd_access_limiter_reload_13_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_13_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_13 寄存器结构定义。地址偏移量:0x2bc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_13 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_13_vp_rd_debug_13_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_13_vp_rd_debug_13_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_14 寄存器结构定义。地址偏移量:0x2c0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_14    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_14 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_14    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_14       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_pixel_format_14_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_pixel_format_14_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_pixel_expansion_14_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_pixel_expansion_14_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_allocated_du_14_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_allocated_du_14_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_last_page_14_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_14_vprd_last_page_14_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_14 寄存器结构定义。地址偏移量:0x2c4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_14           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_14 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_vprd_line_size_14_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_vprd_line_size_14_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_vprd_horizontal_blanking_14_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_14_vprd_horizontal_blanking_14_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_14 寄存器结构定义。地址偏移量:0x2c8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_14        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_14 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_vprd_frame_size_14_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_vprd_frame_size_14_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_vprd_vertical_blanking_14_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_14_vprd_vertical_blanking_14_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_14_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_14 寄存器结构定义。地址偏移量:0x2cc，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_14 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_14_vprd_axi_frame_start_14_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_14_vprd_axi_frame_start_14_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_14 寄存器结构定义。地址偏移量:0x2d0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_14 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_14   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_vprd_line_stride_14_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_vprd_line_stride_14_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_vprd_line_wrap_14_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_14_vprd_line_wrap_14_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_14 寄存器结构定义。地址偏移量:0x2d4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_14 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_14            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_14             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_14                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_14                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_14                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_enable_du_threshold_reached_14_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_enable_du_threshold_reached_14_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_enable_flux_ctrl_14_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_enable_flux_ctrl_14_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_enable_pressure_14_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_enable_pressure_14_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_ok_14_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_ok_14_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_14_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vp_rd_stop_14_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vprd_prefetch_bypass_14_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_14_vprd_prefetch_bypass_14_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_14 寄存器结构定义。地址偏移量:0x2d8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_14      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_14      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_14      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_14      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_14 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_0_14_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_0_14_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_1_14_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_1_14_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_2_14_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_2_14_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_3_14_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_3_14_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_reload_14_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_14_vprd_access_limiter_reload_14_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_14_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_14 寄存器结构定义。地址偏移量:0x2dc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_14 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_14_vp_rd_debug_14_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_14_vp_rd_debug_14_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_15 寄存器结构定义。地址偏移量:0x2e0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_15    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_15 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_15    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_15       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_pixel_format_15_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_pixel_format_15_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_pixel_expansion_15_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_pixel_expansion_15_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_allocated_du_15_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_allocated_du_15_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_last_page_15_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_15_vprd_last_page_15_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_15 寄存器结构定义。地址偏移量:0x2e4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_15           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_15 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_vprd_line_size_15_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_vprd_line_size_15_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_vprd_horizontal_blanking_15_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_15_vprd_horizontal_blanking_15_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_15 寄存器结构定义。地址偏移量:0x2e8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_15        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_15 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_vprd_frame_size_15_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_vprd_frame_size_15_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_vprd_vertical_blanking_15_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_15_vprd_vertical_blanking_15_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_15_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_15 寄存器结构定义。地址偏移量:0x2ec，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_15 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_15_vprd_axi_frame_start_15_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_15_vprd_axi_frame_start_15_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_15 寄存器结构定义。地址偏移量:0x2f0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_15 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_15   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_vprd_line_stride_15_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_vprd_line_stride_15_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_vprd_line_wrap_15_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_15_vprd_line_wrap_15_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_15 寄存器结构定义。地址偏移量:0x2f4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_15 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_15            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_15             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_15                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_15                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_15                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_enable_du_threshold_reached_15_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_enable_du_threshold_reached_15_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_enable_flux_ctrl_15_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_enable_flux_ctrl_15_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_enable_pressure_15_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_enable_pressure_15_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_ok_15_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_ok_15_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_15_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vp_rd_stop_15_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vprd_prefetch_bypass_15_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_15_vprd_prefetch_bypass_15_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_15 寄存器结构定义。地址偏移量:0x2f8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_15      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_15      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_15      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_15      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_15 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_0_15_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_0_15_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_1_15_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_1_15_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_2_15_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_2_15_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_3_15_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_3_15_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_reload_15_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_15_vprd_access_limiter_reload_15_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_15_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_15 寄存器结构定义。地址偏移量:0x2fc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_15 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_15_vp_rd_debug_15_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_15_vp_rd_debug_15_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_16 寄存器结构定义。地址偏移量:0x300，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_16    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_16 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_16    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_16       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_pixel_format_16_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_pixel_format_16_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_pixel_expansion_16_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_pixel_expansion_16_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_allocated_du_16_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_allocated_du_16_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_last_page_16_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_16_vprd_last_page_16_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_16 寄存器结构定义。地址偏移量:0x304，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_16           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_16 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_vprd_line_size_16_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_vprd_line_size_16_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_vprd_horizontal_blanking_16_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_16_vprd_horizontal_blanking_16_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_16 寄存器结构定义。地址偏移量:0x308，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_16        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_16 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_vprd_frame_size_16_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_vprd_frame_size_16_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_vprd_vertical_blanking_16_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_16_vprd_vertical_blanking_16_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_16_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_16 寄存器结构定义。地址偏移量:0x30c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_16 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_16_vprd_axi_frame_start_16_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_16_vprd_axi_frame_start_16_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_16 寄存器结构定义。地址偏移量:0x310，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_16 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_16   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_vprd_line_stride_16_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_vprd_line_stride_16_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_vprd_line_wrap_16_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_16_vprd_line_wrap_16_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_16 寄存器结构定义。地址偏移量:0x314，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_16 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_16            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_16             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_16                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_16                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_16                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_enable_du_threshold_reached_16_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_enable_du_threshold_reached_16_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_enable_flux_ctrl_16_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_enable_flux_ctrl_16_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_enable_pressure_16_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_enable_pressure_16_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_ok_16_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_ok_16_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_16_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vp_rd_stop_16_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vprd_prefetch_bypass_16_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_16_vprd_prefetch_bypass_16_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_16 寄存器结构定义。地址偏移量:0x318，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_16      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_16      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_16      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_16      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_16 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_0_16_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_0_16_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_1_16_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_1_16_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_2_16_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_2_16_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_3_16_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_3_16_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_reload_16_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_16_vprd_access_limiter_reload_16_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_16_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_16 寄存器结构定义。地址偏移量:0x31c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_16 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_16_vp_rd_debug_16_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_16_vp_rd_debug_16_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_17 寄存器结构定义。地址偏移量:0x320，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_17    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_17 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_17    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_17       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_pixel_format_17_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_pixel_format_17_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_pixel_expansion_17_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_pixel_expansion_17_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_allocated_du_17_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_allocated_du_17_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_last_page_17_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_17_vprd_last_page_17_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_17 寄存器结构定义。地址偏移量:0x324，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_17           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_17 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_vprd_line_size_17_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_vprd_line_size_17_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_vprd_horizontal_blanking_17_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_17_vprd_horizontal_blanking_17_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_17 寄存器结构定义。地址偏移量:0x328，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_17        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_17 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_vprd_frame_size_17_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_vprd_frame_size_17_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_vprd_vertical_blanking_17_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_17_vprd_vertical_blanking_17_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_17_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_17 寄存器结构定义。地址偏移量:0x32c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_17 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_17_vprd_axi_frame_start_17_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_17_vprd_axi_frame_start_17_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_17 寄存器结构定义。地址偏移量:0x330，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_17 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_17   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_vprd_line_stride_17_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_vprd_line_stride_17_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_vprd_line_wrap_17_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_17_vprd_line_wrap_17_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_17 寄存器结构定义。地址偏移量:0x334，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_17 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_17            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_17             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_17                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_17                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_17                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_enable_du_threshold_reached_17_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_enable_du_threshold_reached_17_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_enable_flux_ctrl_17_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_enable_flux_ctrl_17_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_enable_pressure_17_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_enable_pressure_17_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_ok_17_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_ok_17_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_17_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vp_rd_stop_17_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vprd_prefetch_bypass_17_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_17_vprd_prefetch_bypass_17_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_17 寄存器结构定义。地址偏移量:0x338，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_17      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_17      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_17      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_17      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_17 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_0_17_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_0_17_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_1_17_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_1_17_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_2_17_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_2_17_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_3_17_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_3_17_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_reload_17_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_17_vprd_access_limiter_reload_17_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_17_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_17 寄存器结构定义。地址偏移量:0x33c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_17 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_17_vp_rd_debug_17_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_17_vp_rd_debug_17_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_18 寄存器结构定义。地址偏移量:0x340，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_18    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_18 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_18    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_18       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_pixel_format_18_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_pixel_format_18_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_pixel_expansion_18_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_pixel_expansion_18_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_allocated_du_18_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_allocated_du_18_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_last_page_18_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_18_vprd_last_page_18_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_18 寄存器结构定义。地址偏移量:0x344，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_18           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_18 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_vprd_line_size_18_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_vprd_line_size_18_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_vprd_horizontal_blanking_18_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_18_vprd_horizontal_blanking_18_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_18 寄存器结构定义。地址偏移量:0x348，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_18        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_18 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_vprd_frame_size_18_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_vprd_frame_size_18_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_vprd_vertical_blanking_18_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_18_vprd_vertical_blanking_18_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_18_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_18 寄存器结构定义。地址偏移量:0x34c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_18 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_18_vprd_axi_frame_start_18_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_18_vprd_axi_frame_start_18_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_18 寄存器结构定义。地址偏移量:0x350，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_18 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_18   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_vprd_line_stride_18_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_vprd_line_stride_18_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_vprd_line_wrap_18_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_18_vprd_line_wrap_18_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_18 寄存器结构定义。地址偏移量:0x354，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_18 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_18            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_18             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_18                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_18                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_18                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_enable_du_threshold_reached_18_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_enable_du_threshold_reached_18_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_enable_flux_ctrl_18_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_enable_flux_ctrl_18_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_enable_pressure_18_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_enable_pressure_18_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_ok_18_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_ok_18_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_18_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vp_rd_stop_18_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vprd_prefetch_bypass_18_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_18_vprd_prefetch_bypass_18_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_18 寄存器结构定义。地址偏移量:0x358，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_18      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_18      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_18      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_18      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_18 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_0_18_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_0_18_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_1_18_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_1_18_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_2_18_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_2_18_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_3_18_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_3_18_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_reload_18_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_18_vprd_access_limiter_reload_18_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_18_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_18 寄存器结构定义。地址偏移量:0x35c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_18 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_18_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_18_vp_rd_debug_18_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_18_vp_rd_debug_18_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_19 寄存器结构定义。地址偏移量:0x360，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_19    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_19 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_19    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_19       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_pixel_format_19_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_pixel_format_19_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_pixel_expansion_19_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_pixel_expansion_19_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_allocated_du_19_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_allocated_du_19_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_last_page_19_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_19_vprd_last_page_19_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_19 寄存器结构定义。地址偏移量:0x364，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_19           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_19 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_vprd_line_size_19_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_vprd_line_size_19_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_vprd_horizontal_blanking_19_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_19_vprd_horizontal_blanking_19_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_19 寄存器结构定义。地址偏移量:0x368，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_19        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_19 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_vprd_frame_size_19_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_vprd_frame_size_19_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_vprd_vertical_blanking_19_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_19_vprd_vertical_blanking_19_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_19_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_19 寄存器结构定义。地址偏移量:0x36c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_19 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_19_vprd_axi_frame_start_19_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_19_vprd_axi_frame_start_19_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_19 寄存器结构定义。地址偏移量:0x370，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_19 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_19   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_vprd_line_stride_19_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_vprd_line_stride_19_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_vprd_line_wrap_19_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_19_vprd_line_wrap_19_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_19 寄存器结构定义。地址偏移量:0x374，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_19 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_19            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_19             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_19                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_19                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_19                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_enable_du_threshold_reached_19_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_enable_du_threshold_reached_19_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_enable_flux_ctrl_19_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_enable_flux_ctrl_19_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_enable_pressure_19_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_enable_pressure_19_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_ok_19_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_ok_19_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_19_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vp_rd_stop_19_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vprd_prefetch_bypass_19_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_19_vprd_prefetch_bypass_19_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_19 寄存器结构定义。地址偏移量:0x378，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_19      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_19      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_19      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_19      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_19 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_0_19_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_0_19_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_1_19_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_1_19_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_2_19_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_2_19_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_3_19_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_3_19_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_reload_19_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_19_vprd_access_limiter_reload_19_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_19_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_19 寄存器结构定义。地址偏移量:0x37c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_19 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_19_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_19_vp_rd_debug_19_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_19_vp_rd_debug_19_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_20 寄存器结构定义。地址偏移量:0x380，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_20    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_20 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_20    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_20       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_pixel_format_20_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_pixel_format_20_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_pixel_expansion_20_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_pixel_expansion_20_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_allocated_du_20_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_allocated_du_20_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_last_page_20_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_20_vprd_last_page_20_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_20 寄存器结构定义。地址偏移量:0x384，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_20           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_20 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_vprd_line_size_20_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_vprd_line_size_20_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_vprd_horizontal_blanking_20_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_20_vprd_horizontal_blanking_20_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_20 寄存器结构定义。地址偏移量:0x388，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_20        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_20 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_vprd_frame_size_20_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_vprd_frame_size_20_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_vprd_vertical_blanking_20_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_20_vprd_vertical_blanking_20_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_20_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_20 寄存器结构定义。地址偏移量:0x38c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_20 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_20_vprd_axi_frame_start_20_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_20_vprd_axi_frame_start_20_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_20 寄存器结构定义。地址偏移量:0x390，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_20 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_20   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_vprd_line_stride_20_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_vprd_line_stride_20_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_vprd_line_wrap_20_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_20_vprd_line_wrap_20_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_20 寄存器结构定义。地址偏移量:0x394，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_20 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_20            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_20             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_20                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_20                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_20                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_enable_du_threshold_reached_20_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_enable_du_threshold_reached_20_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_enable_flux_ctrl_20_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_enable_flux_ctrl_20_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_enable_pressure_20_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_enable_pressure_20_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_ok_20_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_ok_20_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_20_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vp_rd_stop_20_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vprd_prefetch_bypass_20_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_20_vprd_prefetch_bypass_20_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_20 寄存器结构定义。地址偏移量:0x398，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_20      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_20      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_20      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_20      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_20 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_0_20_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_0_20_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_1_20_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_1_20_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_2_20_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_2_20_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_3_20_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_3_20_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_reload_20_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_20_vprd_access_limiter_reload_20_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_20_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_20 寄存器结构定义。地址偏移量:0x39c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_20 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_20_vp_rd_debug_20_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_20_vp_rd_debug_20_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_21 寄存器结构定义。地址偏移量:0x3a0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_21    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_21 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_21    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_21       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_pixel_format_21_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_pixel_format_21_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_pixel_expansion_21_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_pixel_expansion_21_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_allocated_du_21_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_allocated_du_21_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_last_page_21_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_21_vprd_last_page_21_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_21 寄存器结构定义。地址偏移量:0x3a4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_21           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_21 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_vprd_line_size_21_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_vprd_line_size_21_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_vprd_horizontal_blanking_21_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_21_vprd_horizontal_blanking_21_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_21 寄存器结构定义。地址偏移量:0x3a8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_21        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_21 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_vprd_frame_size_21_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_vprd_frame_size_21_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_vprd_vertical_blanking_21_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_21_vprd_vertical_blanking_21_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_21_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_21 寄存器结构定义。地址偏移量:0x3ac，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_21 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_21_vprd_axi_frame_start_21_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_21_vprd_axi_frame_start_21_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_21 寄存器结构定义。地址偏移量:0x3b0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_21 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_21   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_vprd_line_stride_21_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_vprd_line_stride_21_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_vprd_line_wrap_21_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_21_vprd_line_wrap_21_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_21 寄存器结构定义。地址偏移量:0x3b4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_21 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_21            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_21             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_21                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_21                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_21                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_enable_du_threshold_reached_21_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_enable_du_threshold_reached_21_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_enable_flux_ctrl_21_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_enable_flux_ctrl_21_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_enable_pressure_21_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_enable_pressure_21_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_ok_21_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_ok_21_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_21_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vp_rd_stop_21_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vprd_prefetch_bypass_21_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_21_vprd_prefetch_bypass_21_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_21 寄存器结构定义。地址偏移量:0x3b8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_21      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_21      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_21      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_21      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_21 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_0_21_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_0_21_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_1_21_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_1_21_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_2_21_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_2_21_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_3_21_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_3_21_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_reload_21_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_21_vprd_access_limiter_reload_21_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_21_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_21 寄存器结构定义。地址偏移量:0x3bc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_21 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_21_vp_rd_debug_21_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_21_vp_rd_debug_21_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_22 寄存器结构定义。地址偏移量:0x3c0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_22    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_22 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_22    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_22       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_pixel_format_22_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_pixel_format_22_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_pixel_expansion_22_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_pixel_expansion_22_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_allocated_du_22_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_allocated_du_22_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_last_page_22_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_22_vprd_last_page_22_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_22 寄存器结构定义。地址偏移量:0x3c4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_22           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_22 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_vprd_line_size_22_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_vprd_line_size_22_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_vprd_horizontal_blanking_22_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_22_vprd_horizontal_blanking_22_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_22 寄存器结构定义。地址偏移量:0x3c8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_22        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_22 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_vprd_frame_size_22_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_vprd_frame_size_22_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_vprd_vertical_blanking_22_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_22_vprd_vertical_blanking_22_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_22_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_22 寄存器结构定义。地址偏移量:0x3cc，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_22 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_22_vprd_axi_frame_start_22_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_22_vprd_axi_frame_start_22_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_22 寄存器结构定义。地址偏移量:0x3d0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_22 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_22   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_vprd_line_stride_22_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_vprd_line_stride_22_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_vprd_line_wrap_22_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_22_vprd_line_wrap_22_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_22 寄存器结构定义。地址偏移量:0x3d4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_22 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_22            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_22             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_22                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_22                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_22                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_enable_du_threshold_reached_22_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_enable_du_threshold_reached_22_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_enable_flux_ctrl_22_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_enable_flux_ctrl_22_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_enable_pressure_22_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_enable_pressure_22_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_ok_22_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_ok_22_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_22_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vp_rd_stop_22_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vprd_prefetch_bypass_22_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_22_vprd_prefetch_bypass_22_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_22 寄存器结构定义。地址偏移量:0x3d8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_22      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_22      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_22      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_22      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_22 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_0_22_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_0_22_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_1_22_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_1_22_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_2_22_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_2_22_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_3_22_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_3_22_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_reload_22_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_22_vprd_access_limiter_reload_22_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_22_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_22 寄存器结构定义。地址偏移量:0x3dc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_22 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_22_vp_rd_debug_22_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_22_vp_rd_debug_22_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_23 寄存器结构定义。地址偏移量:0x3e0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_23    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_23 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_23    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_23       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_pixel_format_23_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_pixel_format_23_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_pixel_expansion_23_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_pixel_expansion_23_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_allocated_du_23_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_allocated_du_23_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_last_page_23_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_23_vprd_last_page_23_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_23 寄存器结构定义。地址偏移量:0x3e4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_23           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_23 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_vprd_line_size_23_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_vprd_line_size_23_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_vprd_horizontal_blanking_23_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_23_vprd_horizontal_blanking_23_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_23 寄存器结构定义。地址偏移量:0x3e8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_23        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_23 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_vprd_frame_size_23_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_vprd_frame_size_23_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_vprd_vertical_blanking_23_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_23_vprd_vertical_blanking_23_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_23_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_23 寄存器结构定义。地址偏移量:0x3ec，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_23 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_23_vprd_axi_frame_start_23_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_23_vprd_axi_frame_start_23_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_23 寄存器结构定义。地址偏移量:0x3f0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_23 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_23   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_vprd_line_stride_23_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_vprd_line_stride_23_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_vprd_line_wrap_23_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_23_vprd_line_wrap_23_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_23 寄存器结构定义。地址偏移量:0x3f4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_23 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_23            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_23             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_23                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_23                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_23                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_enable_du_threshold_reached_23_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_enable_du_threshold_reached_23_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_enable_flux_ctrl_23_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_enable_flux_ctrl_23_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_enable_pressure_23_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_enable_pressure_23_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_ok_23_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_ok_23_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_23_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vp_rd_stop_23_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vprd_prefetch_bypass_23_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_23_vprd_prefetch_bypass_23_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_23 寄存器结构定义。地址偏移量:0x3f8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_23      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_23      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_23      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_23      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_23 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_0_23_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_0_23_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_1_23_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_1_23_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_2_23_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_2_23_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_3_23_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_3_23_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_reload_23_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_23_vprd_access_limiter_reload_23_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_23_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_23 寄存器结构定义。地址偏移量:0x3fc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_23 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_23_vp_rd_debug_23_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_23_vp_rd_debug_23_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_24 寄存器结构定义。地址偏移量:0x400，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_24    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_24 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_24    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_24       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_pixel_format_24_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_pixel_format_24_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_pixel_expansion_24_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_pixel_expansion_24_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_allocated_du_24_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_allocated_du_24_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_last_page_24_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_24_vprd_last_page_24_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_24 寄存器结构定义。地址偏移量:0x404，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_24           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_24 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_vprd_line_size_24_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_vprd_line_size_24_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_vprd_horizontal_blanking_24_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_24_vprd_horizontal_blanking_24_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_24 寄存器结构定义。地址偏移量:0x408，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_24        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_24 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_vprd_frame_size_24_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_vprd_frame_size_24_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_vprd_vertical_blanking_24_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_24_vprd_vertical_blanking_24_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_24_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_24 寄存器结构定义。地址偏移量:0x40c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_24 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_24_vprd_axi_frame_start_24_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_24_vprd_axi_frame_start_24_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_24 寄存器结构定义。地址偏移量:0x410，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_24 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_24   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_vprd_line_stride_24_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_vprd_line_stride_24_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_vprd_line_wrap_24_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_24_vprd_line_wrap_24_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_24 寄存器结构定义。地址偏移量:0x414，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_24 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_24            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_24             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_24                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_24                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_24                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_enable_du_threshold_reached_24_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_enable_du_threshold_reached_24_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_enable_flux_ctrl_24_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_enable_flux_ctrl_24_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_enable_pressure_24_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_enable_pressure_24_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_ok_24_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_ok_24_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_24_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vp_rd_stop_24_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vprd_prefetch_bypass_24_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_24_vprd_prefetch_bypass_24_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_24 寄存器结构定义。地址偏移量:0x418，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_24      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_24      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_24      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_24      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_24 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_0_24_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_0_24_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_1_24_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_1_24_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_2_24_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_2_24_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_3_24_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_3_24_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_reload_24_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_24_vprd_access_limiter_reload_24_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_24_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_24 寄存器结构定义。地址偏移量:0x41c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_24 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_24_vp_rd_debug_24_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_24_vp_rd_debug_24_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_25 寄存器结构定义。地址偏移量:0x420，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_25    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_25 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_25    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_25       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_pixel_format_25_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_pixel_format_25_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_pixel_expansion_25_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_pixel_expansion_25_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_allocated_du_25_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_allocated_du_25_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_last_page_25_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_25_vprd_last_page_25_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_25 寄存器结构定义。地址偏移量:0x424，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_25           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_25 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_vprd_line_size_25_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_vprd_line_size_25_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_vprd_horizontal_blanking_25_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_25_vprd_horizontal_blanking_25_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_25 寄存器结构定义。地址偏移量:0x428，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_25        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_25 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_vprd_frame_size_25_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_vprd_frame_size_25_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_vprd_vertical_blanking_25_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_25_vprd_vertical_blanking_25_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_25_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_25 寄存器结构定义。地址偏移量:0x42c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_25 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_25_vprd_axi_frame_start_25_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_25_vprd_axi_frame_start_25_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_25 寄存器结构定义。地址偏移量:0x430，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_25 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_25   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_vprd_line_stride_25_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_vprd_line_stride_25_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_vprd_line_wrap_25_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_25_vprd_line_wrap_25_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_25 寄存器结构定义。地址偏移量:0x434，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_25 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_25            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_25             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_25                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_25                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_25                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_enable_du_threshold_reached_25_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_enable_du_threshold_reached_25_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_enable_flux_ctrl_25_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_enable_flux_ctrl_25_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_enable_pressure_25_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_enable_pressure_25_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_ok_25_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_ok_25_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_25_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vp_rd_stop_25_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vprd_prefetch_bypass_25_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_25_vprd_prefetch_bypass_25_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_25 寄存器结构定义。地址偏移量:0x438，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_25      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_25      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_25      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_25      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_25 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_0_25_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_0_25_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_1_25_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_1_25_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_2_25_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_2_25_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_3_25_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_3_25_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_reload_25_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_25_vprd_access_limiter_reload_25_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_25_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_25 寄存器结构定义。地址偏移量:0x43c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_25 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_25_vp_rd_debug_25_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_25_vp_rd_debug_25_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_26 寄存器结构定义。地址偏移量:0x440，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_26    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_26 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_26    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_26       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_pixel_format_26_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_pixel_format_26_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_pixel_expansion_26_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_pixel_expansion_26_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_allocated_du_26_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_allocated_du_26_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_last_page_26_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_26_vprd_last_page_26_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_26 寄存器结构定义。地址偏移量:0x444，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_26           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_26 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_vprd_line_size_26_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_vprd_line_size_26_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_vprd_horizontal_blanking_26_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_26_vprd_horizontal_blanking_26_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_26 寄存器结构定义。地址偏移量:0x448，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_26        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_26 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_vprd_frame_size_26_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_vprd_frame_size_26_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_vprd_vertical_blanking_26_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_26_vprd_vertical_blanking_26_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_26_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_26 寄存器结构定义。地址偏移量:0x44c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_26 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_26_vprd_axi_frame_start_26_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_26_vprd_axi_frame_start_26_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_26 寄存器结构定义。地址偏移量:0x450，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_26 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_26   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_vprd_line_stride_26_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_vprd_line_stride_26_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_vprd_line_wrap_26_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_26_vprd_line_wrap_26_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_26 寄存器结构定义。地址偏移量:0x454，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_26 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_26            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_26             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_26                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_26                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_26                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_enable_du_threshold_reached_26_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_enable_du_threshold_reached_26_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_enable_flux_ctrl_26_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_enable_flux_ctrl_26_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_enable_pressure_26_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_enable_pressure_26_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_ok_26_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_ok_26_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_26_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vp_rd_stop_26_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vprd_prefetch_bypass_26_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_26_vprd_prefetch_bypass_26_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_26 寄存器结构定义。地址偏移量:0x458，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_26      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_26      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_26      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_26      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_26 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_0_26_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_0_26_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_1_26_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_1_26_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_2_26_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_2_26_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_3_26_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_3_26_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_reload_26_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_26_vprd_access_limiter_reload_26_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_26_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_26 寄存器结构定义。地址偏移量:0x45c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_26 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_26_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_26_vp_rd_debug_26_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_26_vp_rd_debug_26_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_27 寄存器结构定义。地址偏移量:0x460，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_27    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_27 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_27    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_27       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_pixel_format_27_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_pixel_format_27_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_pixel_expansion_27_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_pixel_expansion_27_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_allocated_du_27_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_allocated_du_27_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_last_page_27_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_27_vprd_last_page_27_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_27 寄存器结构定义。地址偏移量:0x464，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_27           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_27 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_vprd_line_size_27_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_vprd_line_size_27_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_vprd_horizontal_blanking_27_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_27_vprd_horizontal_blanking_27_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_27 寄存器结构定义。地址偏移量:0x468，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_27        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_27 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_vprd_frame_size_27_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_vprd_frame_size_27_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_vprd_vertical_blanking_27_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_27_vprd_vertical_blanking_27_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_27_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_27 寄存器结构定义。地址偏移量:0x46c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_27 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_27_vprd_axi_frame_start_27_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_27_vprd_axi_frame_start_27_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_27 寄存器结构定义。地址偏移量:0x470，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_27 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_27   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_vprd_line_stride_27_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_vprd_line_stride_27_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_vprd_line_wrap_27_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_27_vprd_line_wrap_27_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_27 寄存器结构定义。地址偏移量:0x474，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_27 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_27            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_27             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_27                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_27                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_27                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_enable_du_threshold_reached_27_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_enable_du_threshold_reached_27_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_enable_flux_ctrl_27_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_enable_flux_ctrl_27_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_enable_pressure_27_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_enable_pressure_27_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_ok_27_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_ok_27_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_27_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vp_rd_stop_27_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vprd_prefetch_bypass_27_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_27_vprd_prefetch_bypass_27_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_27 寄存器结构定义。地址偏移量:0x478，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_27      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_27      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_27      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_27      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_27 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_0_27_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_0_27_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_1_27_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_1_27_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_2_27_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_2_27_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_3_27_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_3_27_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_reload_27_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_27_vprd_access_limiter_reload_27_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_27_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_27 寄存器结构定义。地址偏移量:0x47c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_27 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_27_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_27_vp_rd_debug_27_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_27_vp_rd_debug_27_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_28 寄存器结构定义。地址偏移量:0x480，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_28    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_28 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_28    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_28       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_pixel_format_28_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_pixel_format_28_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_pixel_expansion_28_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_pixel_expansion_28_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_allocated_du_28_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_allocated_du_28_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_last_page_28_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_28_vprd_last_page_28_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_28 寄存器结构定义。地址偏移量:0x484，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_28           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_28 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_vprd_line_size_28_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_vprd_line_size_28_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_vprd_horizontal_blanking_28_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_28_vprd_horizontal_blanking_28_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_28 寄存器结构定义。地址偏移量:0x488，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_28        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_28 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_vprd_frame_size_28_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_vprd_frame_size_28_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_vprd_vertical_blanking_28_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_28_vprd_vertical_blanking_28_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_28_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_28 寄存器结构定义。地址偏移量:0x48c，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_28 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_28_vprd_axi_frame_start_28_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_28_vprd_axi_frame_start_28_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_28 寄存器结构定义。地址偏移量:0x490，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_28 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_28   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_vprd_line_stride_28_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_vprd_line_stride_28_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_vprd_line_wrap_28_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_28_vprd_line_wrap_28_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_28 寄存器结构定义。地址偏移量:0x494，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_28 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_28            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_28             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_28                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_28                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_28                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_enable_du_threshold_reached_28_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_enable_du_threshold_reached_28_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_enable_flux_ctrl_28_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_enable_flux_ctrl_28_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_enable_pressure_28_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_enable_pressure_28_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_ok_28_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_ok_28_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_28_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vp_rd_stop_28_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vprd_prefetch_bypass_28_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_28_vprd_prefetch_bypass_28_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_28 寄存器结构定义。地址偏移量:0x498，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_28      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_28      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_28      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_28      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_28 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_0_28_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_0_28_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_1_28_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_1_28_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_2_28_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_2_28_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_3_28_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_3_28_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_reload_28_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_28_vprd_access_limiter_reload_28_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_28_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_28 寄存器结构定义。地址偏移量:0x49c，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_28 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_28_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_28_vp_rd_debug_28_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_28_vp_rd_debug_28_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_UNION
 结构说明  : CVDR_IPP_VP_RD_CFG_29 寄存器结构定义。地址偏移量:0x4a0，初值:0x00000000，宽度:32
 寄存器说明: Video port read Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_pixel_format_29    : 5;  /* bit[0-4]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_pixel_expansion_29 : 1;  /* bit[5-5]  : See previous definition on VP_RD_CFG_0. */
        unsigned int  vprd_allocated_du_29    : 5;  /* bit[6-10] : See previous definition on VP_RD_CFG_0. */
        unsigned int  reserved                : 2;  /* bit[11-12]:  */
        unsigned int  vprd_last_page_29       : 19; /* bit[13-31]: See previous definition on VP_RD_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_pixel_format_29_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_pixel_format_29_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_pixel_expansion_29_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_pixel_expansion_29_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_allocated_du_29_START     (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_allocated_du_29_END       (10)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_last_page_29_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_CFG_29_vprd_last_page_29_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_UNION
 结构说明  : CVDR_IPP_VP_RD_LWG_29 寄存器结构定义。地址偏移量:0x4a4，初值:0x00000000，宽度:32
 寄存器说明: Line width generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_size_29           : 15; /* bit[0-14] : See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_0                  : 1;  /* bit[15-15]:  */
        unsigned int  vprd_horizontal_blanking_29 : 8;  /* bit[16-23]: See previous definition on VP_RD_LWG_0. */
        unsigned int  reserved_1                  : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_vprd_line_size_29_START            (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_vprd_line_size_29_END              (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_vprd_horizontal_blanking_29_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LWG_29_vprd_horizontal_blanking_29_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_UNION
 结构说明  : CVDR_IPP_VP_RD_FHG_29 寄存器结构定义。地址偏移量:0x4a8，初值:0x00000000，宽度:32
 寄存器说明: Frame height generation.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_frame_size_29        : 15; /* bit[0-14] : See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_0                : 1;  /* bit[15-15]:  */
        unsigned int  vprd_vertical_blanking_29 : 8;  /* bit[16-23]: See previous definition on VP_RD_FHG_0. */
        unsigned int  reserved_1                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_vprd_frame_size_29_START         (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_vprd_frame_size_29_END           (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_vprd_vertical_blanking_29_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_FHG_29_vprd_vertical_blanking_29_END    (23)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_29_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_FS_29 寄存器结构定义。地址偏移量:0x4ac，初值:0x00000000，宽度:32
 寄存器说明: AXI frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                : 2;  /* bit[0-1] :  */
        unsigned int  vprd_axi_frame_start_29 : 30; /* bit[2-31]: See previous definition on VP_RD_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_29_vprd_axi_frame_start_29_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_FS_29_vprd_axi_frame_start_29_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_UNION
 结构说明  : CVDR_IPP_VP_RD_AXI_LINE_29 寄存器结构定义。地址偏移量:0x4b0，初值:0xFFFC0000，宽度:32
 寄存器说明: Line Wrap definition.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_line_stride_29 : 14; /* bit[0-13] : See previous definition on VP_RD_AXI_LINE_0. */
        unsigned int  reserved            : 4;  /* bit[14-17]:  */
        unsigned int  vprd_line_wrap_29   : 14; /* bit[18-31]: See previous definition on VP_RD_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_vprd_line_stride_29_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_vprd_line_stride_29_END    (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_vprd_line_wrap_29_START    (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_AXI_LINE_29_vprd_line_wrap_29_END      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_UNION
 结构说明  : CVDR_IPP_VP_RD_IF_CFG_29 寄存器结构定义。地址偏移量:0x4b4，初值:0x00000000，宽度:32
 寄存器说明: Video port read interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_rd_stop_enable_du_threshold_reached_29 : 1;  /* bit[16-16]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_flux_ctrl_29            : 1;  /* bit[17-17]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_enable_pressure_29             : 1;  /* bit[18-18]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_rd_stop_ok_29                          : 1;  /* bit[24-24]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  vp_rd_stop_29                             : 1;  /* bit[25-25]: See previous definition on VP_RD_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vprd_prefetch_bypass_29                   : 1;  /* bit[31-31]: See previous definition on VP_RD_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_enable_du_threshold_reached_29_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_enable_du_threshold_reached_29_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_enable_flux_ctrl_29_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_enable_flux_ctrl_29_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_enable_pressure_29_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_enable_pressure_29_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_ok_29_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_ok_29_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_29_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vp_rd_stop_29_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vprd_prefetch_bypass_29_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_IF_CFG_29_vprd_prefetch_bypass_29_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_UNION
 结构说明  : CVDR_IPP_VP_RD_LIMITER_29 寄存器结构定义。地址偏移量:0x4b8，初值:0x0F00FFFF，宽度:32
 寄存器说明: Video port read Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vprd_access_limiter_0_29      : 4;  /* bit[0-3]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_1_29      : 4;  /* bit[4-7]  : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_2_29      : 4;  /* bit[8-11] : See previous definition on VP_RD_LIMITER_0. */
        unsigned int  vprd_access_limiter_3_29      : 4;  /* bit[12-15]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vprd_access_limiter_reload_29 : 4;  /* bit[24-27]: See previous definition on VP_RD_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_0_29_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_0_29_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_1_29_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_1_29_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_2_29_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_2_29_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_3_29_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_3_29_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_reload_29_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_LIMITER_29_vprd_access_limiter_reload_29_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_29_UNION
 结构说明  : CVDR_IPP_VP_RD_DEBUG_29 寄存器结构定义。地址偏移量:0x4bc，初值:0x00000000，宽度:32
 寄存器说明: Video Port Read DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_rd_debug_29 : 32; /* bit[0-31]: See previous definition on VP_RD_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_29_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_29_vp_rd_debug_29_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_RD_DEBUG_29_vp_rd_debug_29_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_0 寄存器结构定义。地址偏移量:0x500，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_0 : 1;  /* bit[16-16]: Enable bit used to enable CVDR_CFG.du_threshold_reached to be taken into account.
                                                                                    0: the port can not be stopped based on CVDR_CFG.du_threshold_reached
                                                                                    1: the port will be stopped when CVDR_CFG.du_threshold_reached condition is met
                                                                                    [note] 1 for Non-real time port. [end]. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_0            : 1;  /* bit[17-17]: Enable bit used to enable flux_ctrl_w to be taken into account.
                                                                                    0: the port can not be stopped based on flux_ctrl_w
                                                                                    1: the port will be stopped when flux_ctrl_w = 1.
                                                                                    [note] 1 for Non-real time port. [end]. */
        unsigned int  vp_wr_stop_enable_pressure_0             : 1;  /* bit[18-18]: Enable bit used to enable pressure_bit to be taken into account.
                                                                                    0: the port can not be stopped based on pressure_bit
                                                                                    1: the port will be stopped when pressure_bit = 1.
                                                                                    [note] 1 for Non-real time port. [end]. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_0                          : 1;  /* bit[24-24]: Status bit relative to the stop sequence.
                                                                                    shall be 0 when nr_wr_stop is 0.
                                                                                    . */
        unsigned int  vp_wr_stop_0                             : 1;  /* bit[25-25]: stop bit

                                                                                     used to stop the interface.
                                                                                    0: the port is not stopped
                                                                                    1: the port is required to be stopped. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_0                   : 1;  /* bit[31-31]: Enables to bypass the prefetch mechanism.
                                                                                    0: prefetch functionality is enabled
                                                                                    1: prefetch functionality is bypassed (Index ID is always vpwr_index_id0). */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_enable_du_threshold_reached_0_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_enable_du_threshold_reached_0_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_enable_flux_ctrl_0_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_enable_flux_ctrl_0_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_enable_pressure_0_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_enable_pressure_0_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_ok_0_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_ok_0_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_0_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vp_wr_stop_0_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vpwr_prefetch_bypass_0_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_0_vpwr_prefetch_bypass_0_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_0 寄存器结构定义。地址偏移量:0x504，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_0      : 4;  /* bit[0-3]  : CVDR RAM access limiter: maximum number of 128 bit write accesses allowed on a 16 clock cycle window
                                                                        0: 0 access allowed (unexpected in regular SW)
                                                                        1: 1 access maximum allowed
                                                                        and so on
                                                                        15: 15 accesses maximum allowed. */
        unsigned int  vpwr_access_limiter_1_0      : 4;  /* bit[4-7]  : see description of access_limiter_0. */
        unsigned int  vpwr_access_limiter_2_0      : 4;  /* bit[8-11] : see description of access_limiter_0. */
        unsigned int  vpwr_access_limiter_3_0      : 4;  /* bit[12-15]: see description of access_limiter_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_0 : 4;  /* bit[24-27]: Access limiter reload functionality: controls the throughput available for a given initiator over 128 cycles.
                                                                        When 0xF

                                                                         access_limiter value is reloaded every 16 cycles.
                                                                        Bit 0: enables access limiter 0 value to be reloaded on first 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 1: enables access limiter 1 value to be reloaded on second 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 2: enables access limiter 2 value to be reloaded on third 16 cycles simulation time (modulo 128 cycles)
                                                                        Bit 3: enables access limiter 3 value to be reloaded on fourth 16 cycles simulation time (modulo 128 cycles). */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_0_0_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_0_0_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_1_0_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_1_0_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_2_0_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_2_0_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_3_0_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_3_0_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_reload_0_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_0_vpwr_access_limiter_reload_0_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0 寄存器结构定义。地址偏移量:0x50c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_0      : 14; /* bit[0-13] : Stride to use on line starts (aligned to a 16 byte boundary: Max stride of 256 KBytes)
                                                                        0: line stride disabled (continuous) (continuous mode : next line will start on a 128 bytes boundary)
                                                                        1: 2*16 bytes
                                                                        and so on
                                                                        16383 : 16384*16 bytes. */
        unsigned int  vpwr_line_start_wstrb_sid0_0 : 4;  /* bit[14-17]: Video port write start strobe to use on a start of a line.
                                                                        0xF: 16 bytes are valid - packer started at 0 (default)
                                                                        0xE: 15 bytes are valid (MSB) - packer starts at 8 bits and lowest 8 bits are don't care.
                                                                        ..
                                                                        0x0: 1 byte valid (MSB) - packers starts at 120bits and lowest 120 bits are don't care.. */
        unsigned int  vpwr_line_wrap_sid0_0        : 14; /* bit[18-31]: Line wrap: wrap to frame start location (14 bits)
                                                                        0: wrap every line
                                                                        1: wrap every 2 lines
                                                                        and so on
                                                                        0x3FFE: wrap every 16K-1 lines
                                                                        0x3FFF: no wrap (continuous writing)
                                                                        In case of YUV420

                                                                         UV data is bigger than needed

                                                                         1 additional line is received

                                                                         and this register must be configured accordingly.
                                                                        [note] when a buffer is wrapped (rolling buffer)

                                                                         the last_page is expected be aligned with the last available 32K Page.[end]. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_vpwr_line_stride_sid0_0_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_vpwr_line_stride_sid0_0_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_vpwr_line_start_wstrb_sid0_0_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_vpwr_line_start_wstrb_sid0_0_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_vpwr_line_wrap_sid0_0_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_0_vpwr_line_wrap_sid0_0_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0 寄存器结构定义。地址偏移量:0x510，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_0 : 16; /* bit[0-15] : Byte granularity used for prefetch generation (same as line_stride) - added to avoid design/verif corner cases/risk.
                                                                          0: 1 byte
                                                                          1: 2 byte
                                                                          ..
                                                                          65535: 65536 bytes (32K*16bits)
                                                                          Examples: VRAW with RGB16 * 4608 = 1728 128b

                                                                           FE with 32K*16 bits = 4096 128b
                                                                          [note] needs to be configured to stripe_size*pixel_size(in bytes) - 1 [end]. */
        unsigned int  reserved                       : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0_vp_wr_stripe_size_bytes_sid0_0_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_0_vp_wr_stripe_size_bytes_sid0_0_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_0 寄存器结构定义。地址偏移量:0x514，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_0    : 5;  /* bit[0-4]  : Pixel formats supported (see ref. [1])
                                                                        0:3 : 1/2/4/8 PF8
                                                                        4:7 : 1/2/4/8 PF10
                                                                        8:11 : 1/2/4/8 PF12
                                                                        12:15 : 1/2/4/8 PF14
                                                                        16:24 : NA
                                                                        25 : 3PF8
                                                                        26 : 6PF12 (72 bits)
                                                                        27 : 6PF16 (96 bits)
                                                                        28 : D32 (or 2PF16)
                                                                        29 : D48
                                                                        30 : D64
                                                                        31 : D128. */
        unsigned int  vpwr_pixel_expansion_sid0_0 : 1;  /* bit[5-5]  : Pixel expansion can be used (see ref. [1]). */
        unsigned int  vpwr_4pf_enable_sid0_0      : 1;  /* bit[6-6]  : enables 4PF to 2PF conversion /slowdown. This conversion shall only be enabled when 1PF or 2PF formats are configured into vpwr_pixel_format
                                                                       1: enabled
                                                                       0: disabled. */
        unsigned int  reserved                    : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_0       : 19; /* bit[13-31]: last 32KB page reserved to write the data. This page is not necessarely prefetched but is the last page reserved by the Software. In Wrap Mode

                                                                        this address is used as the max available address.
                                                                       The CVDR must prefetch the page used by the frame start (back to initial - in case of continuous mode) once the last page is equal to vpwr_last_page.
                                                                       This info must be aligned with the frame size (SW info) and the vpwr_line_wrap info (SW and HW) to be aligned with buffer size reserved by the SW.
                                                                       Specific case: long lines in 1 frame are possible (Jpeg case

                                                                        3A/DIS

                                                                        debug): vpwr_line_wrap=0 and vpwr_last_page will be different from the page used by vpwr_address_frame_start. In this case

                                                                        the module must be able to prefetch continuously until the vpwr_last_page and get back to initial page to prepare continuous write operation.
                                                                       In case of YUV420

                                                                        UV data is bigger than needed

                                                                        1 additional line is received

                                                                        and this register must be configured accordingly.
                                                                       [note] change compared to V160 is that ISP interface is now 34 bits while was 32 bits before. 2 bits added on that register.[end]. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_pixel_format_sid0_0_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_pixel_format_sid0_0_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_pixel_expansion_sid0_0_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_pixel_expansion_sid0_0_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_4pf_enable_sid0_0_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_4pf_enable_sid0_0_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_last_page_sid0_0_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_0_vpwr_last_page_sid0_0_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0 寄存器结构定义。地址偏移量:0x518，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                        : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_0 : 30; /* bit[2-31]: Start address of the frame (in 16 bytes boundary). ISP address is now 33:0 and was 31:0 on v160.
                                                                          Since we are 128-bits

                                                                           the 4 lowest bits on the AXI address are always 0 at ISP boundary.
                                                                          The vpwr_address_frame_start matches the upper bits of the AXI address available at ISp level (33:4).
                                                                          [note] change compared to V160 is that ISP interface is now 34 bits while was 32 bits before. 2 bits added on that register.[end]. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0_vpwr_address_frame_start_sid0_0_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_0_vpwr_address_frame_start_sid0_0_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_0_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_0 寄存器结构定义。地址偏移量:0x51c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_0 : 32; /* bit[0-31]: Video Port Write DEBUG information. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_0_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_0_vp_wr_debug_sid0_0_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_0_vp_wr_debug_sid0_0_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_3 寄存器结构定义。地址偏移量:0x560，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_3 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_3            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_3             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_3                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_3                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_3                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_enable_du_threshold_reached_3_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_enable_du_threshold_reached_3_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_enable_flux_ctrl_3_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_enable_flux_ctrl_3_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_enable_pressure_3_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_enable_pressure_3_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_ok_3_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_ok_3_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_3_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vp_wr_stop_3_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vpwr_prefetch_bypass_3_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_3_vpwr_prefetch_bypass_3_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_3 寄存器结构定义。地址偏移量:0x564，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_3      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_3      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_3      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_3      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_3 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_0_3_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_0_3_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_1_3_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_1_3_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_2_3_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_2_3_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_3_3_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_3_3_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_reload_3_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_3_vpwr_access_limiter_reload_3_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3 寄存器结构定义。地址偏移量:0x56c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_3      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_3 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_3        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_vpwr_line_stride_sid0_3_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_vpwr_line_stride_sid0_3_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_vpwr_line_start_wstrb_sid0_3_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_vpwr_line_start_wstrb_sid0_3_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_vpwr_line_wrap_sid0_3_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_3_vpwr_line_wrap_sid0_3_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3 寄存器结构定义。地址偏移量:0x570，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_3 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                       : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3_vp_wr_stripe_size_bytes_sid0_3_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_3_vp_wr_stripe_size_bytes_sid0_3_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_3 寄存器结构定义。地址偏移量:0x574，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_3    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_3 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_3      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                    : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_3       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_pixel_format_sid0_3_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_pixel_format_sid0_3_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_pixel_expansion_sid0_3_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_pixel_expansion_sid0_3_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_4pf_enable_sid0_3_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_4pf_enable_sid0_3_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_last_page_sid0_3_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_3_vpwr_last_page_sid0_3_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3 寄存器结构定义。地址偏移量:0x578，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                        : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_3 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3_vpwr_address_frame_start_sid0_3_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_3_vpwr_address_frame_start_sid0_3_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_3_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_3 寄存器结构定义。地址偏移量:0x57c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_3 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_3_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_3_vp_wr_debug_sid0_3_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_3_vp_wr_debug_sid0_3_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_8 寄存器结构定义。地址偏移量:0x600，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_8 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_8            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_8             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_8                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_8                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_8                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_enable_du_threshold_reached_8_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_enable_du_threshold_reached_8_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_enable_flux_ctrl_8_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_enable_flux_ctrl_8_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_enable_pressure_8_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_enable_pressure_8_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_ok_8_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_ok_8_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_8_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vp_wr_stop_8_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vpwr_prefetch_bypass_8_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_8_vpwr_prefetch_bypass_8_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_8 寄存器结构定义。地址偏移量:0x604，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_8      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_8      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_8      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_8      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_8 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_0_8_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_0_8_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_1_8_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_1_8_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_2_8_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_2_8_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_3_8_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_3_8_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_reload_8_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_8_vpwr_access_limiter_reload_8_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8 寄存器结构定义。地址偏移量:0x60c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_8      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_8 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_8        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_vpwr_line_stride_sid0_8_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_vpwr_line_stride_sid0_8_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_vpwr_line_start_wstrb_sid0_8_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_vpwr_line_start_wstrb_sid0_8_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_vpwr_line_wrap_sid0_8_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_8_vpwr_line_wrap_sid0_8_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8 寄存器结构定义。地址偏移量:0x610，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_8 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                       : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8_vp_wr_stripe_size_bytes_sid0_8_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_8_vp_wr_stripe_size_bytes_sid0_8_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_8 寄存器结构定义。地址偏移量:0x614，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_8    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_8 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_8      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                    : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_8       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_pixel_format_sid0_8_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_pixel_format_sid0_8_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_pixel_expansion_sid0_8_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_pixel_expansion_sid0_8_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_4pf_enable_sid0_8_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_4pf_enable_sid0_8_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_last_page_sid0_8_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_8_vpwr_last_page_sid0_8_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8 寄存器结构定义。地址偏移量:0x618，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                        : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_8 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8_vpwr_address_frame_start_sid0_8_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_8_vpwr_address_frame_start_sid0_8_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_8_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_8 寄存器结构定义。地址偏移量:0x61c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_8 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_8_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_8_vp_wr_debug_sid0_8_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_8_vp_wr_debug_sid0_8_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_9 寄存器结构定义。地址偏移量:0x620，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                               : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_9 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_9            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_9             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                               : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_9                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_9                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                               : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_9                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_enable_du_threshold_reached_9_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_enable_du_threshold_reached_9_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_enable_flux_ctrl_9_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_enable_flux_ctrl_9_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_enable_pressure_9_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_enable_pressure_9_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_ok_9_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_ok_9_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_9_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vp_wr_stop_9_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vpwr_prefetch_bypass_9_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_9_vpwr_prefetch_bypass_9_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_9 寄存器结构定义。地址偏移量:0x624，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_9      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_9      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_9      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_9      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                   : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_9 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_0_9_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_0_9_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_1_9_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_1_9_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_2_9_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_2_9_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_3_9_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_3_9_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_reload_9_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_9_vpwr_access_limiter_reload_9_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9 寄存器结构定义。地址偏移量:0x62c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_9      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_9 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_9        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_vpwr_line_stride_sid0_9_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_vpwr_line_stride_sid0_9_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_vpwr_line_start_wstrb_sid0_9_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_vpwr_line_start_wstrb_sid0_9_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_vpwr_line_wrap_sid0_9_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_9_vpwr_line_wrap_sid0_9_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9 寄存器结构定义。地址偏移量:0x630，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_9 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                       : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9_vp_wr_stripe_size_bytes_sid0_9_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_9_vp_wr_stripe_size_bytes_sid0_9_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_9 寄存器结构定义。地址偏移量:0x634，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_9    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_9 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_9      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                    : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_9       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_pixel_format_sid0_9_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_pixel_format_sid0_9_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_pixel_expansion_sid0_9_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_pixel_expansion_sid0_9_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_4pf_enable_sid0_9_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_4pf_enable_sid0_9_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_last_page_sid0_9_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_9_vpwr_last_page_sid0_9_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9 寄存器结构定义。地址偏移量:0x638，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                        : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_9 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9_vpwr_address_frame_start_sid0_9_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_9_vpwr_address_frame_start_sid0_9_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_9_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_9 寄存器结构定义。地址偏移量:0x63c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_9 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_9_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_9_vp_wr_debug_sid0_9_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_9_vp_wr_debug_sid0_9_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_10 寄存器结构定义。地址偏移量:0x640，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_10 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_10            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_10             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_10                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_10                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_10                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_enable_du_threshold_reached_10_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_enable_du_threshold_reached_10_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_enable_flux_ctrl_10_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_enable_flux_ctrl_10_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_enable_pressure_10_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_enable_pressure_10_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_ok_10_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_ok_10_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_10_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vp_wr_stop_10_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vpwr_prefetch_bypass_10_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_10_vpwr_prefetch_bypass_10_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_10 寄存器结构定义。地址偏移量:0x644，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_10      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_10      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_10      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_10      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_10 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_0_10_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_0_10_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_1_10_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_1_10_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_2_10_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_2_10_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_3_10_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_3_10_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_reload_10_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_10_vpwr_access_limiter_reload_10_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10 寄存器结构定义。地址偏移量:0x64c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_10      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_10 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_10        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_vpwr_line_stride_sid0_10_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_vpwr_line_stride_sid0_10_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_vpwr_line_start_wstrb_sid0_10_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_vpwr_line_start_wstrb_sid0_10_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_vpwr_line_wrap_sid0_10_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_10_vpwr_line_wrap_sid0_10_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10 寄存器结构定义。地址偏移量:0x650，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_10 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10_vp_wr_stripe_size_bytes_sid0_10_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_10_vp_wr_stripe_size_bytes_sid0_10_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_10 寄存器结构定义。地址偏移量:0x654，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_10    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_10 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_10      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_10       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_pixel_format_sid0_10_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_pixel_format_sid0_10_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_pixel_expansion_sid0_10_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_pixel_expansion_sid0_10_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_4pf_enable_sid0_10_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_4pf_enable_sid0_10_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_last_page_sid0_10_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_10_vpwr_last_page_sid0_10_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10 寄存器结构定义。地址偏移量:0x658，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_10 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10_vpwr_address_frame_start_sid0_10_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_10_vpwr_address_frame_start_sid0_10_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_10_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_10 寄存器结构定义。地址偏移量:0x65c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_10 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_10_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_10_vp_wr_debug_sid0_10_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_10_vp_wr_debug_sid0_10_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_11 寄存器结构定义。地址偏移量:0x660，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_11 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_11            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_11             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_11                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_11                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_11                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_enable_du_threshold_reached_11_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_enable_du_threshold_reached_11_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_enable_flux_ctrl_11_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_enable_flux_ctrl_11_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_enable_pressure_11_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_enable_pressure_11_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_ok_11_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_ok_11_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_11_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vp_wr_stop_11_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vpwr_prefetch_bypass_11_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_11_vpwr_prefetch_bypass_11_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_11 寄存器结构定义。地址偏移量:0x664，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_11      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_11      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_11      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_11      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_11 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_0_11_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_0_11_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_1_11_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_1_11_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_2_11_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_2_11_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_3_11_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_3_11_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_reload_11_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_11_vpwr_access_limiter_reload_11_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11 寄存器结构定义。地址偏移量:0x66c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_11      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_11 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_11        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_vpwr_line_stride_sid0_11_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_vpwr_line_stride_sid0_11_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_vpwr_line_start_wstrb_sid0_11_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_vpwr_line_start_wstrb_sid0_11_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_vpwr_line_wrap_sid0_11_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_11_vpwr_line_wrap_sid0_11_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11 寄存器结构定义。地址偏移量:0x670，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_11 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11_vp_wr_stripe_size_bytes_sid0_11_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_11_vp_wr_stripe_size_bytes_sid0_11_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_11 寄存器结构定义。地址偏移量:0x674，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_11    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_11 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_11      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_11       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_pixel_format_sid0_11_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_pixel_format_sid0_11_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_pixel_expansion_sid0_11_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_pixel_expansion_sid0_11_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_4pf_enable_sid0_11_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_4pf_enable_sid0_11_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_last_page_sid0_11_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_11_vpwr_last_page_sid0_11_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11 寄存器结构定义。地址偏移量:0x678，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_11 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11_vpwr_address_frame_start_sid0_11_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_11_vpwr_address_frame_start_sid0_11_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_11_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_11 寄存器结构定义。地址偏移量:0x67c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_11 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_11_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_11_vp_wr_debug_sid0_11_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_11_vp_wr_debug_sid0_11_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_12 寄存器结构定义。地址偏移量:0x680，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_12 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_12            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_12             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_12                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_12                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_12                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_enable_du_threshold_reached_12_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_enable_du_threshold_reached_12_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_enable_flux_ctrl_12_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_enable_flux_ctrl_12_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_enable_pressure_12_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_enable_pressure_12_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_ok_12_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_ok_12_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_12_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vp_wr_stop_12_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vpwr_prefetch_bypass_12_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_12_vpwr_prefetch_bypass_12_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_12 寄存器结构定义。地址偏移量:0x684，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_12      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_12      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_12      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_12      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_12 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_0_12_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_0_12_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_1_12_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_1_12_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_2_12_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_2_12_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_3_12_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_3_12_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_reload_12_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_12_vpwr_access_limiter_reload_12_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12 寄存器结构定义。地址偏移量:0x68c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_12      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_12 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_12        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_vpwr_line_stride_sid0_12_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_vpwr_line_stride_sid0_12_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_vpwr_line_start_wstrb_sid0_12_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_vpwr_line_start_wstrb_sid0_12_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_vpwr_line_wrap_sid0_12_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_12_vpwr_line_wrap_sid0_12_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12 寄存器结构定义。地址偏移量:0x690，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_12 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12_vp_wr_stripe_size_bytes_sid0_12_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_12_vp_wr_stripe_size_bytes_sid0_12_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_12 寄存器结构定义。地址偏移量:0x694，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_12    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_12 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_12      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_12       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_pixel_format_sid0_12_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_pixel_format_sid0_12_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_pixel_expansion_sid0_12_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_pixel_expansion_sid0_12_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_4pf_enable_sid0_12_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_4pf_enable_sid0_12_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_last_page_sid0_12_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_12_vpwr_last_page_sid0_12_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12 寄存器结构定义。地址偏移量:0x698，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_12 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12_vpwr_address_frame_start_sid0_12_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_12_vpwr_address_frame_start_sid0_12_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_12_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_12 寄存器结构定义。地址偏移量:0x69c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_12 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_12_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_12_vp_wr_debug_sid0_12_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_12_vp_wr_debug_sid0_12_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_13 寄存器结构定义。地址偏移量:0x6a0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_13 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_13            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_13             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_13                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_13                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_13                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_enable_du_threshold_reached_13_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_enable_du_threshold_reached_13_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_enable_flux_ctrl_13_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_enable_flux_ctrl_13_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_enable_pressure_13_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_enable_pressure_13_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_ok_13_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_ok_13_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_13_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vp_wr_stop_13_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vpwr_prefetch_bypass_13_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_13_vpwr_prefetch_bypass_13_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_13 寄存器结构定义。地址偏移量:0x6a4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_13      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_13      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_13      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_13      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_13 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_0_13_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_0_13_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_1_13_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_1_13_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_2_13_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_2_13_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_3_13_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_3_13_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_reload_13_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_13_vpwr_access_limiter_reload_13_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13 寄存器结构定义。地址偏移量:0x6ac，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_13      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_13 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_13        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_vpwr_line_stride_sid0_13_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_vpwr_line_stride_sid0_13_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_vpwr_line_start_wstrb_sid0_13_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_vpwr_line_start_wstrb_sid0_13_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_vpwr_line_wrap_sid0_13_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_13_vpwr_line_wrap_sid0_13_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13 寄存器结构定义。地址偏移量:0x6b0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_13 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13_vp_wr_stripe_size_bytes_sid0_13_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_13_vp_wr_stripe_size_bytes_sid0_13_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_13 寄存器结构定义。地址偏移量:0x6b4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_13    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_13 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_13      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_13       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_pixel_format_sid0_13_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_pixel_format_sid0_13_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_pixel_expansion_sid0_13_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_pixel_expansion_sid0_13_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_4pf_enable_sid0_13_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_4pf_enable_sid0_13_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_last_page_sid0_13_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_13_vpwr_last_page_sid0_13_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13 寄存器结构定义。地址偏移量:0x6b8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_13 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13_vpwr_address_frame_start_sid0_13_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_13_vpwr_address_frame_start_sid0_13_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_13_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_13 寄存器结构定义。地址偏移量:0x6bc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_13 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_13_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_13_vp_wr_debug_sid0_13_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_13_vp_wr_debug_sid0_13_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_14 寄存器结构定义。地址偏移量:0x6c0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_14 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_14            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_14             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_14                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_14                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_14                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_enable_du_threshold_reached_14_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_enable_du_threshold_reached_14_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_enable_flux_ctrl_14_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_enable_flux_ctrl_14_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_enable_pressure_14_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_enable_pressure_14_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_ok_14_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_ok_14_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_14_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vp_wr_stop_14_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vpwr_prefetch_bypass_14_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_14_vpwr_prefetch_bypass_14_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_14 寄存器结构定义。地址偏移量:0x6c4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_14      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_14      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_14      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_14      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_14 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_0_14_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_0_14_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_1_14_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_1_14_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_2_14_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_2_14_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_3_14_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_3_14_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_reload_14_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_14_vpwr_access_limiter_reload_14_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14 寄存器结构定义。地址偏移量:0x6cc，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_14      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_14 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_14        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_vpwr_line_stride_sid0_14_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_vpwr_line_stride_sid0_14_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_vpwr_line_start_wstrb_sid0_14_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_vpwr_line_start_wstrb_sid0_14_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_vpwr_line_wrap_sid0_14_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_14_vpwr_line_wrap_sid0_14_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14 寄存器结构定义。地址偏移量:0x6d0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_14 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14_vp_wr_stripe_size_bytes_sid0_14_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_14_vp_wr_stripe_size_bytes_sid0_14_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_14 寄存器结构定义。地址偏移量:0x6d4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_14    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_14 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_14      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_14       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_pixel_format_sid0_14_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_pixel_format_sid0_14_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_pixel_expansion_sid0_14_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_pixel_expansion_sid0_14_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_4pf_enable_sid0_14_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_4pf_enable_sid0_14_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_last_page_sid0_14_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_14_vpwr_last_page_sid0_14_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14 寄存器结构定义。地址偏移量:0x6d8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_14 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14_vpwr_address_frame_start_sid0_14_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_14_vpwr_address_frame_start_sid0_14_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_14_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_14 寄存器结构定义。地址偏移量:0x6dc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_14 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_14_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_14_vp_wr_debug_sid0_14_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_14_vp_wr_debug_sid0_14_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_15 寄存器结构定义。地址偏移量:0x6e0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_15 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_15            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_15             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_15                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_15                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_15                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_enable_du_threshold_reached_15_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_enable_du_threshold_reached_15_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_enable_flux_ctrl_15_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_enable_flux_ctrl_15_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_enable_pressure_15_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_enable_pressure_15_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_ok_15_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_ok_15_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_15_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vp_wr_stop_15_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vpwr_prefetch_bypass_15_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_15_vpwr_prefetch_bypass_15_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_15 寄存器结构定义。地址偏移量:0x6e4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_15      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_15      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_15      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_15      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_15 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_0_15_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_0_15_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_1_15_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_1_15_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_2_15_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_2_15_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_3_15_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_3_15_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_reload_15_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_15_vpwr_access_limiter_reload_15_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15 寄存器结构定义。地址偏移量:0x6ec，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_15      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_15 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_15        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_vpwr_line_stride_sid0_15_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_vpwr_line_stride_sid0_15_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_vpwr_line_start_wstrb_sid0_15_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_vpwr_line_start_wstrb_sid0_15_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_vpwr_line_wrap_sid0_15_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_15_vpwr_line_wrap_sid0_15_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15 寄存器结构定义。地址偏移量:0x6f0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_15 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15_vp_wr_stripe_size_bytes_sid0_15_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_15_vp_wr_stripe_size_bytes_sid0_15_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_15 寄存器结构定义。地址偏移量:0x6f4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_15    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_15 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_15      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_15       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_pixel_format_sid0_15_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_pixel_format_sid0_15_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_pixel_expansion_sid0_15_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_pixel_expansion_sid0_15_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_4pf_enable_sid0_15_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_4pf_enable_sid0_15_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_last_page_sid0_15_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_15_vpwr_last_page_sid0_15_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15 寄存器结构定义。地址偏移量:0x6f8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_15 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15_vpwr_address_frame_start_sid0_15_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_15_vpwr_address_frame_start_sid0_15_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_15_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_15 寄存器结构定义。地址偏移量:0x6fc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_15 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_15_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_15_vp_wr_debug_sid0_15_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_15_vp_wr_debug_sid0_15_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_16 寄存器结构定义。地址偏移量:0x700，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_16 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_16            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_16             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_16                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_16                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_16                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_enable_du_threshold_reached_16_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_enable_du_threshold_reached_16_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_enable_flux_ctrl_16_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_enable_flux_ctrl_16_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_enable_pressure_16_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_enable_pressure_16_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_ok_16_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_ok_16_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_16_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vp_wr_stop_16_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vpwr_prefetch_bypass_16_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_16_vpwr_prefetch_bypass_16_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_16 寄存器结构定义。地址偏移量:0x704，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_16      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_16      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_16      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_16      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_16 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_0_16_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_0_16_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_1_16_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_1_16_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_2_16_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_2_16_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_3_16_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_3_16_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_reload_16_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_16_vpwr_access_limiter_reload_16_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16 寄存器结构定义。地址偏移量:0x70c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_16      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_16 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_16        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_vpwr_line_stride_sid0_16_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_vpwr_line_stride_sid0_16_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_vpwr_line_start_wstrb_sid0_16_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_vpwr_line_start_wstrb_sid0_16_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_vpwr_line_wrap_sid0_16_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_16_vpwr_line_wrap_sid0_16_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16 寄存器结构定义。地址偏移量:0x710，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_16 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16_vp_wr_stripe_size_bytes_sid0_16_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_16_vp_wr_stripe_size_bytes_sid0_16_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_16 寄存器结构定义。地址偏移量:0x714，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_16    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_16 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_16      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_16       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_pixel_format_sid0_16_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_pixel_format_sid0_16_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_pixel_expansion_sid0_16_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_pixel_expansion_sid0_16_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_4pf_enable_sid0_16_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_4pf_enable_sid0_16_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_last_page_sid0_16_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_16_vpwr_last_page_sid0_16_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16 寄存器结构定义。地址偏移量:0x718，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_16 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16_vpwr_address_frame_start_sid0_16_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_16_vpwr_address_frame_start_sid0_16_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_16_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_16 寄存器结构定义。地址偏移量:0x71c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_16 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_16_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_16_vp_wr_debug_sid0_16_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_16_vp_wr_debug_sid0_16_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_17 寄存器结构定义。地址偏移量:0x720，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_17 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_17            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_17             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_17                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_17                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_17                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_enable_du_threshold_reached_17_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_enable_du_threshold_reached_17_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_enable_flux_ctrl_17_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_enable_flux_ctrl_17_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_enable_pressure_17_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_enable_pressure_17_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_ok_17_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_ok_17_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_17_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vp_wr_stop_17_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vpwr_prefetch_bypass_17_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_17_vpwr_prefetch_bypass_17_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_17 寄存器结构定义。地址偏移量:0x724，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_17      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_17      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_17      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_17      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_17 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_0_17_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_0_17_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_1_17_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_1_17_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_2_17_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_2_17_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_3_17_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_3_17_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_reload_17_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_17_vpwr_access_limiter_reload_17_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17 寄存器结构定义。地址偏移量:0x72c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_17      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_17 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_17        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_vpwr_line_stride_sid0_17_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_vpwr_line_stride_sid0_17_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_vpwr_line_start_wstrb_sid0_17_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_vpwr_line_start_wstrb_sid0_17_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_vpwr_line_wrap_sid0_17_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_17_vpwr_line_wrap_sid0_17_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17 寄存器结构定义。地址偏移量:0x730，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_17 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17_vp_wr_stripe_size_bytes_sid0_17_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_17_vp_wr_stripe_size_bytes_sid0_17_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_17 寄存器结构定义。地址偏移量:0x734，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_17    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_17 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_17      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_17       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_pixel_format_sid0_17_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_pixel_format_sid0_17_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_pixel_expansion_sid0_17_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_pixel_expansion_sid0_17_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_4pf_enable_sid0_17_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_4pf_enable_sid0_17_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_last_page_sid0_17_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_17_vpwr_last_page_sid0_17_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17 寄存器结构定义。地址偏移量:0x738，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_17 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17_vpwr_address_frame_start_sid0_17_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_17_vpwr_address_frame_start_sid0_17_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_17_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_17 寄存器结构定义。地址偏移量:0x73c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_17 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_17_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_17_vp_wr_debug_sid0_17_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_17_vp_wr_debug_sid0_17_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_20 寄存器结构定义。地址偏移量:0x780，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_20 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_20            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_20             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_20                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_20                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_20                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_enable_du_threshold_reached_20_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_enable_du_threshold_reached_20_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_enable_flux_ctrl_20_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_enable_flux_ctrl_20_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_enable_pressure_20_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_enable_pressure_20_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_ok_20_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_ok_20_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_20_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vp_wr_stop_20_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vpwr_prefetch_bypass_20_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_20_vpwr_prefetch_bypass_20_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_20 寄存器结构定义。地址偏移量:0x784，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_20      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_20      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_20      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_20      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_20 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_0_20_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_0_20_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_1_20_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_1_20_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_2_20_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_2_20_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_3_20_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_3_20_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_reload_20_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_20_vpwr_access_limiter_reload_20_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20 寄存器结构定义。地址偏移量:0x78c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_20      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_20 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_20        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_vpwr_line_stride_sid0_20_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_vpwr_line_stride_sid0_20_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_vpwr_line_start_wstrb_sid0_20_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_vpwr_line_start_wstrb_sid0_20_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_vpwr_line_wrap_sid0_20_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_20_vpwr_line_wrap_sid0_20_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20 寄存器结构定义。地址偏移量:0x790，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_20 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20_vp_wr_stripe_size_bytes_sid0_20_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_20_vp_wr_stripe_size_bytes_sid0_20_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_20 寄存器结构定义。地址偏移量:0x794，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_20    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_20 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_20      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_20       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_pixel_format_sid0_20_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_pixel_format_sid0_20_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_pixel_expansion_sid0_20_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_pixel_expansion_sid0_20_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_4pf_enable_sid0_20_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_4pf_enable_sid0_20_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_last_page_sid0_20_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_20_vpwr_last_page_sid0_20_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20 寄存器结构定义。地址偏移量:0x798，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_20 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20_vpwr_address_frame_start_sid0_20_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_20_vpwr_address_frame_start_sid0_20_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_20_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_20 寄存器结构定义。地址偏移量:0x79c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_20 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_20_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_20_vp_wr_debug_sid0_20_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_20_vp_wr_debug_sid0_20_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_21 寄存器结构定义。地址偏移量:0x7a0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_21 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_21            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_21             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_21                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_21                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_21                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_enable_du_threshold_reached_21_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_enable_du_threshold_reached_21_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_enable_flux_ctrl_21_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_enable_flux_ctrl_21_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_enable_pressure_21_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_enable_pressure_21_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_ok_21_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_ok_21_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_21_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vp_wr_stop_21_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vpwr_prefetch_bypass_21_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_21_vpwr_prefetch_bypass_21_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_21 寄存器结构定义。地址偏移量:0x7a4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_21      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_21      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_21      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_21      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_21 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_0_21_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_0_21_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_1_21_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_1_21_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_2_21_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_2_21_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_3_21_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_3_21_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_reload_21_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_21_vpwr_access_limiter_reload_21_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21 寄存器结构定义。地址偏移量:0x7ac，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_21      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_21 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_21        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_vpwr_line_stride_sid0_21_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_vpwr_line_stride_sid0_21_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_vpwr_line_start_wstrb_sid0_21_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_vpwr_line_start_wstrb_sid0_21_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_vpwr_line_wrap_sid0_21_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_21_vpwr_line_wrap_sid0_21_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21 寄存器结构定义。地址偏移量:0x7b0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_21 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21_vp_wr_stripe_size_bytes_sid0_21_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_21_vp_wr_stripe_size_bytes_sid0_21_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_21 寄存器结构定义。地址偏移量:0x7b4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_21    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_21 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_21      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_21       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_pixel_format_sid0_21_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_pixel_format_sid0_21_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_pixel_expansion_sid0_21_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_pixel_expansion_sid0_21_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_4pf_enable_sid0_21_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_4pf_enable_sid0_21_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_last_page_sid0_21_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_21_vpwr_last_page_sid0_21_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21 寄存器结构定义。地址偏移量:0x7b8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_21 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21_vpwr_address_frame_start_sid0_21_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_21_vpwr_address_frame_start_sid0_21_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_21_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_21 寄存器结构定义。地址偏移量:0x7bc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_21 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_21_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_21_vp_wr_debug_sid0_21_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_21_vp_wr_debug_sid0_21_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_22 寄存器结构定义。地址偏移量:0x7c0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_22 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_22            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_22             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_22                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_22                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_22                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_enable_du_threshold_reached_22_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_enable_du_threshold_reached_22_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_enable_flux_ctrl_22_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_enable_flux_ctrl_22_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_enable_pressure_22_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_enable_pressure_22_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_ok_22_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_ok_22_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_22_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vp_wr_stop_22_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vpwr_prefetch_bypass_22_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_22_vpwr_prefetch_bypass_22_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_22 寄存器结构定义。地址偏移量:0x7c4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_22      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_22      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_22      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_22      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_22 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_0_22_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_0_22_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_1_22_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_1_22_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_2_22_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_2_22_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_3_22_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_3_22_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_reload_22_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_22_vpwr_access_limiter_reload_22_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22 寄存器结构定义。地址偏移量:0x7cc，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_22      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_22 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_22        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_vpwr_line_stride_sid0_22_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_vpwr_line_stride_sid0_22_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_vpwr_line_start_wstrb_sid0_22_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_vpwr_line_start_wstrb_sid0_22_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_vpwr_line_wrap_sid0_22_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_22_vpwr_line_wrap_sid0_22_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22 寄存器结构定义。地址偏移量:0x7d0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_22 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22_vp_wr_stripe_size_bytes_sid0_22_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_22_vp_wr_stripe_size_bytes_sid0_22_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_22 寄存器结构定义。地址偏移量:0x7d4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_22    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_22 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_22      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_22       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_pixel_format_sid0_22_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_pixel_format_sid0_22_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_pixel_expansion_sid0_22_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_pixel_expansion_sid0_22_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_4pf_enable_sid0_22_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_4pf_enable_sid0_22_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_last_page_sid0_22_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_22_vpwr_last_page_sid0_22_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22 寄存器结构定义。地址偏移量:0x7d8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_22 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22_vpwr_address_frame_start_sid0_22_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_22_vpwr_address_frame_start_sid0_22_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_22_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_22 寄存器结构定义。地址偏移量:0x7dc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_22 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_22_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_22_vp_wr_debug_sid0_22_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_22_vp_wr_debug_sid0_22_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_23 寄存器结构定义。地址偏移量:0x7e0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_23 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_23            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_23             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_23                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_23                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_23                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_enable_du_threshold_reached_23_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_enable_du_threshold_reached_23_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_enable_flux_ctrl_23_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_enable_flux_ctrl_23_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_enable_pressure_23_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_enable_pressure_23_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_ok_23_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_ok_23_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_23_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vp_wr_stop_23_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vpwr_prefetch_bypass_23_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_23_vpwr_prefetch_bypass_23_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_23 寄存器结构定义。地址偏移量:0x7e4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_23      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_23      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_23      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_23      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_23 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_0_23_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_0_23_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_1_23_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_1_23_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_2_23_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_2_23_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_3_23_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_3_23_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_reload_23_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_23_vpwr_access_limiter_reload_23_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23 寄存器结构定义。地址偏移量:0x7ec，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_23      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_23 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_23        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_vpwr_line_stride_sid0_23_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_vpwr_line_stride_sid0_23_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_vpwr_line_start_wstrb_sid0_23_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_vpwr_line_start_wstrb_sid0_23_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_vpwr_line_wrap_sid0_23_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_23_vpwr_line_wrap_sid0_23_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23 寄存器结构定义。地址偏移量:0x7f0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_23 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23_vp_wr_stripe_size_bytes_sid0_23_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_23_vp_wr_stripe_size_bytes_sid0_23_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_23 寄存器结构定义。地址偏移量:0x7f4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_23    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_23 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_23      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_23       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_pixel_format_sid0_23_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_pixel_format_sid0_23_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_pixel_expansion_sid0_23_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_pixel_expansion_sid0_23_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_4pf_enable_sid0_23_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_4pf_enable_sid0_23_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_last_page_sid0_23_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_23_vpwr_last_page_sid0_23_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23 寄存器结构定义。地址偏移量:0x7f8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_23 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23_vpwr_address_frame_start_sid0_23_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_23_vpwr_address_frame_start_sid0_23_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_23_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_23 寄存器结构定义。地址偏移量:0x7fc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_23 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_23_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_23_vp_wr_debug_sid0_23_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_23_vp_wr_debug_sid0_23_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_24 寄存器结构定义。地址偏移量:0x800，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_24 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_24            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_24             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_24                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_24                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_24                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_enable_du_threshold_reached_24_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_enable_du_threshold_reached_24_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_enable_flux_ctrl_24_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_enable_flux_ctrl_24_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_enable_pressure_24_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_enable_pressure_24_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_ok_24_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_ok_24_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_24_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vp_wr_stop_24_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vpwr_prefetch_bypass_24_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_24_vpwr_prefetch_bypass_24_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_24 寄存器结构定义。地址偏移量:0x804，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_24      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_24      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_24      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_24      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_24 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_0_24_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_0_24_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_1_24_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_1_24_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_2_24_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_2_24_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_3_24_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_3_24_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_reload_24_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_24_vpwr_access_limiter_reload_24_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24 寄存器结构定义。地址偏移量:0x80c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_24      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_24 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_24        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_vpwr_line_stride_sid0_24_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_vpwr_line_stride_sid0_24_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_vpwr_line_start_wstrb_sid0_24_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_vpwr_line_start_wstrb_sid0_24_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_vpwr_line_wrap_sid0_24_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_24_vpwr_line_wrap_sid0_24_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24 寄存器结构定义。地址偏移量:0x810，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_24 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24_vp_wr_stripe_size_bytes_sid0_24_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_24_vp_wr_stripe_size_bytes_sid0_24_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_24 寄存器结构定义。地址偏移量:0x814，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_24    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_24 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_24      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_24       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_pixel_format_sid0_24_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_pixel_format_sid0_24_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_pixel_expansion_sid0_24_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_pixel_expansion_sid0_24_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_4pf_enable_sid0_24_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_4pf_enable_sid0_24_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_last_page_sid0_24_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_24_vpwr_last_page_sid0_24_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24 寄存器结构定义。地址偏移量:0x818，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_24 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24_vpwr_address_frame_start_sid0_24_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_24_vpwr_address_frame_start_sid0_24_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_24_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_24 寄存器结构定义。地址偏移量:0x81c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_24 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_24_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_24_vp_wr_debug_sid0_24_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_24_vp_wr_debug_sid0_24_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_25 寄存器结构定义。地址偏移量:0x820，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_25 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_25            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_25             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_25                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_25                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_25                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_enable_du_threshold_reached_25_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_enable_du_threshold_reached_25_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_enable_flux_ctrl_25_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_enable_flux_ctrl_25_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_enable_pressure_25_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_enable_pressure_25_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_ok_25_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_ok_25_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_25_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vp_wr_stop_25_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vpwr_prefetch_bypass_25_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_25_vpwr_prefetch_bypass_25_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_25 寄存器结构定义。地址偏移量:0x824，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_25      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_25      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_25      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_25      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_25 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_0_25_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_0_25_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_1_25_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_1_25_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_2_25_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_2_25_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_3_25_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_3_25_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_reload_25_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_25_vpwr_access_limiter_reload_25_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25 寄存器结构定义。地址偏移量:0x82c，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_25      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_25 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_25        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_vpwr_line_stride_sid0_25_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_vpwr_line_stride_sid0_25_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_vpwr_line_start_wstrb_sid0_25_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_vpwr_line_start_wstrb_sid0_25_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_vpwr_line_wrap_sid0_25_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_25_vpwr_line_wrap_sid0_25_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25 寄存器结构定义。地址偏移量:0x830，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_25 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25_vp_wr_stripe_size_bytes_sid0_25_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_25_vp_wr_stripe_size_bytes_sid0_25_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_25 寄存器结构定义。地址偏移量:0x834，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_25    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_25 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_25      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_25       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_pixel_format_sid0_25_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_pixel_format_sid0_25_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_pixel_expansion_sid0_25_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_pixel_expansion_sid0_25_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_4pf_enable_sid0_25_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_4pf_enable_sid0_25_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_last_page_sid0_25_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_25_vpwr_last_page_sid0_25_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25 寄存器结构定义。地址偏移量:0x838，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_25 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25_vpwr_address_frame_start_sid0_25_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_25_vpwr_address_frame_start_sid0_25_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_25_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_25 寄存器结构定义。地址偏移量:0x83c，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_25 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_25_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_25_vp_wr_debug_sid0_25_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_25_vp_wr_debug_sid0_25_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_UNION
 结构说明  : CVDR_IPP_VP_WR_IF_CFG_30 寄存器结构定义。地址偏移量:0x8c0，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write interface configuration: prefetch or reset or stall capability.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                                : 16; /* bit[0-15] :  */
        unsigned int  vp_wr_stop_enable_du_threshold_reached_30 : 1;  /* bit[16-16]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_flux_ctrl_30            : 1;  /* bit[17-17]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_enable_pressure_30             : 1;  /* bit[18-18]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_1                                : 5;  /* bit[19-23]:  */
        unsigned int  vp_wr_stop_ok_30                          : 1;  /* bit[24-24]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  vp_wr_stop_30                             : 1;  /* bit[25-25]: See previous definition on VP_WR_IF_CFG_0. */
        unsigned int  reserved_2                                : 5;  /* bit[26-30]:  */
        unsigned int  vpwr_prefetch_bypass_30                   : 1;  /* bit[31-31]: See previous definition on VP_WR_IF_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_enable_du_threshold_reached_30_START  (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_enable_du_threshold_reached_30_END    (16)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_enable_flux_ctrl_30_START             (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_enable_flux_ctrl_30_END               (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_enable_pressure_30_START              (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_enable_pressure_30_END                (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_ok_30_START                           (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_ok_30_END                             (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_30_START                              (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vp_wr_stop_30_END                                (25)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vpwr_prefetch_bypass_30_START                    (31)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_vpwr_prefetch_bypass_30_END                      (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_UNION
 结构说明  : CVDR_IPP_VP_WR_LIMITER_30 寄存器结构定义。地址偏移量:0x8c4，初值:0x0F00FFFF，宽度:32
 寄存器说明: [] Video port write Access limiter.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_access_limiter_0_30      : 4;  /* bit[0-3]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_1_30      : 4;  /* bit[4-7]  : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_2_30      : 4;  /* bit[8-11] : See previous definition on VP_WR_LIMITER_0. */
        unsigned int  vpwr_access_limiter_3_30      : 4;  /* bit[12-15]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  vpwr_access_limiter_reload_30 : 4;  /* bit[24-27]: See previous definition on VP_WR_LIMITER_0. */
        unsigned int  reserved_1                    : 4;  /* bit[28-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_0_30_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_0_30_END         (3)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_1_30_START       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_1_30_END         (7)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_2_30_START       (8)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_2_30_END         (11)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_3_30_START       (12)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_3_30_END         (15)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_reload_30_START  (24)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_vpwr_access_limiter_reload_30_END    (27)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30 寄存器结构定义。地址偏移量:0x8cc，初值:0xFFFFC000，宽度:32
 寄存器说明: [] AXI line wrap and line stride.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_line_stride_sid0_30      : 14; /* bit[0-13] : See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_start_wstrb_sid0_30 : 4;  /* bit[14-17]: See previous definition on VP_WR_SID_AXI_LINE_0. */
        unsigned int  vpwr_line_wrap_sid0_30        : 14; /* bit[18-31]: See previous definition on VP_WR_SID_AXI_LINE_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_vpwr_line_stride_sid0_30_START       (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_vpwr_line_stride_sid0_30_END         (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_vpwr_line_start_wstrb_sid0_30_START  (14)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_vpwr_line_start_wstrb_sid0_30_END    (17)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_vpwr_line_wrap_sid0_30_START         (18)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_vpwr_line_wrap_sid0_30_END           (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30 寄存器结构定义。地址偏移量:0x8d0，初值:0x0000FFFF，宽度:32
 寄存器说明: [] Video port write interface configuration for stripe size (added on V300) - used for big buffers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_stripe_size_bytes_sid0_30 : 16; /* bit[0-15] : See previous definition on VP_WR_SID_PREFETCH_0. */
        unsigned int  reserved                        : 16; /* bit[16-31]:  */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30_vp_wr_stripe_size_bytes_sid0_30_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_PREFETCH_sid0_30_vp_wr_stripe_size_bytes_sid0_30_END    (15)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_CFG_sid0_30 寄存器结构定义。地址偏移量:0x8d4，初值:0x00000000，宽度:32
 寄存器说明: [] Video port write Configuration.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vpwr_pixel_format_sid0_30    : 5;  /* bit[0-4]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_pixel_expansion_sid0_30 : 1;  /* bit[5-5]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  vpwr_4pf_enable_sid0_30      : 1;  /* bit[6-6]  : See previous definition on VP_WR_SID_CFG_0. */
        unsigned int  reserved                     : 6;  /* bit[7-12] :  */
        unsigned int  vpwr_last_page_sid0_30       : 19; /* bit[13-31]: See previous definition on VP_WR_SID_CFG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_pixel_format_sid0_30_START     (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_pixel_format_sid0_30_END       (4)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_pixel_expansion_sid0_30_START  (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_pixel_expansion_sid0_30_END    (5)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_4pf_enable_sid0_30_START       (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_4pf_enable_sid0_30_END         (6)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_last_page_sid0_30_START        (13)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_vpwr_last_page_sid0_30_END          (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30 寄存器结构定义。地址偏移量:0x8d8，初值:0x00000000，宽度:32
 寄存器说明: [] AXI address Frame start.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved                         : 2;  /* bit[0-1] :  */
        unsigned int  vpwr_address_frame_start_sid0_30 : 30; /* bit[2-31]: See previous definition on VP_WR_SID_AXI_FS_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_vpwr_address_frame_start_sid0_30_START  (2)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_vpwr_address_frame_start_sid0_30_END    (31)


/*****************************************************************************
 结构名    : SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_UNION
 结构说明  : CVDR_IPP_VP_WR_SID_DEBUG_sid0_30 寄存器结构定义。地址偏移量:0x8dc，初值:0x00000000，宽度:32
 寄存器说明: [] Video Port Write DEBUG information.
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vp_wr_debug_sid0_30 : 32; /* bit[0-31]: See previous definition on VP_WR_SID_DEBUG_0. */
    } reg;
} SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_UNION;
#endif
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_vp_wr_debug_sid0_30_START  (0)
#define SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_vp_wr_debug_sid0_30_END    (31)






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

#endif /* end of soc_ipp_cvdr_interface.h */
