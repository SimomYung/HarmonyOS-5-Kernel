/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_corecrg_l_l3_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_CORECRG_L_L3_INTERFACE_H__
#define __SOC_CORECRG_L_L3_INTERFACE_H__

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


/* 寄存器说明：外设时钟使能寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PEREN0_UNION */
#define SOC_CORECRG_L_L3_PEREN0_ADDR(base)                    ((base) + (0x000UL))

/* 寄存器说明：外设时钟禁止寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERDIS0_UNION */
#define SOC_CORECRG_L_L3_PERDIS0_ADDR(base)                   ((base) + (0x004UL))

/* 寄存器说明：外设时钟使能状态寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERCLKEN0_UNION */
#define SOC_CORECRG_L_L3_PERCLKEN0_ADDR(base)                 ((base) + (0x008UL))

/* 寄存器说明：外设时钟最终状态寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERSTAT0_UNION */
#define SOC_CORECRG_L_L3_PERSTAT0_ADDR(base)                  ((base) + (0x00CUL))

/* 寄存器说明：外设状态寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERI_STAT0_UNION */
#define SOC_CORECRG_L_L3_PERI_STAT0_ADDR(base)                ((base) + (0x060UL))

/* 寄存器说明：时钟分频比控制寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_CLKDIV0_UNION */
#define SOC_CORECRG_L_L3_CLKDIV0_ADDR(base)                   ((base) + (0x080UL))

/* 寄存器说明：Vsensor控制使能寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_ADDR(base)         ((base) + (0x100UL))

/* 寄存器说明：Vsensor bypass寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_ADDR(base)     ((base) + (0x104UL))

/* 寄存器说明：Vsensor控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_ADDR(base)            ((base) + (0x108UL))

/* 寄存器说明：SVFD配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_ADDR(base)      ((base) + (0x10CUL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_ADDR(base)      ((base) + (0x110UL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_ADDR(base)      ((base) + (0x114UL))

/* 寄存器说明：SVFD状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_ADDR(base)   ((base) + (0x118UL))

/* 寄存器说明：SVFD测试状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_ADDR(base)       ((base) + (0x11CUL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_ADDR(base)      ((base) + (0x120UL))

/* 寄存器说明：Vsensor控制使能寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_EN_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_ADDR(base)             ((base) + (0x130UL))

/* 寄存器说明：Vsensor bypass寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_ADDR(base)         ((base) + (0x134UL))

/* 寄存器说明：Vsensor控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_L3_ADDR(base)                ((base) + (0x138UL))

/* 寄存器说明：SVFD配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_ADDR(base)          ((base) + (0x13CUL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_ADDR(base)          ((base) + (0x140UL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_ADDR(base)          ((base) + (0x144UL))

/* 寄存器说明：SVFD状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_ADDR(base)       ((base) + (0x148UL))

/* 寄存器说明：SVFD测试状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_TEST_STAT_L3_UNION */
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_ADDR(base)           ((base) + (0x14CUL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_ADDR(base)          ((base) + (0x150UL))

/* 寄存器说明：APLL0 SSC控制寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0SSCCTRL_UNION */
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_ADDR(base)              ((base) + (0x1E0UL))

/* 寄存器说明：APLL1 SSC控制寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1SSCCTRL_UNION */
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_ADDR(base)              ((base) + (0x1E4UL))

/* 寄存器说明：APLL0系统控制寄存器0寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG0_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG0_ADDR(base)                ((base) + (0x1E8UL))

/* 寄存器说明：APLL0系统控制寄存器1寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG1_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG1_ADDR(base)                ((base) + (0x1ECUL))

/* 寄存器说明：APLL0系统控制寄存器2寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG2_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG2_ADDR(base)                ((base) + (0x1F0UL))

/* 寄存器说明：APLL1系统控制寄存器0寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG0_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG0_ADDR(base)                ((base) + (0x1F4UL))

/* 寄存器说明：APLL1系统控制寄存器1寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG1_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG1_ADDR(base)                ((base) + (0x1F8UL))

/* 寄存器说明：APLL1系统控制寄存器2寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG2_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG2_ADDR(base)                ((base) + (0x1FCUL))

/* 寄存器说明：FCM子系统时钟使能寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_FCM_CLKEN_UNION */
#define SOC_CORECRG_L_L3_FCM_CLKEN_ADDR(base)                 ((base) + (0x200UL))

/* 寄存器说明：APLL0系统状态寄存器0
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_STAT_UNION */
#define SOC_CORECRG_L_L3_APLL0_STAT_ADDR(base)                ((base) + (0x250UL))

/* 寄存器说明：APLL1系统状态寄存器0
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_STAT_UNION */
#define SOC_CORECRG_L_L3_APLL1_STAT_ADDR(base)                ((base) + (0x254UL))

/* 寄存器说明：APLL0系统控制寄存器3寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG3_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG3_ADDR(base)                ((base) + (0x258UL))

/* 寄存器说明：APLL1系统控制寄存器3寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG3_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG3_ADDR(base)                ((base) + (0x25CUL))


#else


/* 寄存器说明：外设时钟使能寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PEREN0_UNION */
#define SOC_CORECRG_L_L3_PEREN0_ADDR(base)                    ((base) + (0x000))

/* 寄存器说明：外设时钟禁止寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERDIS0_UNION */
#define SOC_CORECRG_L_L3_PERDIS0_ADDR(base)                   ((base) + (0x004))

/* 寄存器说明：外设时钟使能状态寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERCLKEN0_UNION */
#define SOC_CORECRG_L_L3_PERCLKEN0_ADDR(base)                 ((base) + (0x008))

/* 寄存器说明：外设时钟最终状态寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERSTAT0_UNION */
#define SOC_CORECRG_L_L3_PERSTAT0_ADDR(base)                  ((base) + (0x00C))

/* 寄存器说明：外设状态寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_PERI_STAT0_UNION */
#define SOC_CORECRG_L_L3_PERI_STAT0_ADDR(base)                ((base) + (0x060))

/* 寄存器说明：时钟分频比控制寄存器0。
   位域定义UNION结构:  SOC_CORECRG_L_L3_CLKDIV0_UNION */
#define SOC_CORECRG_L_L3_CLKDIV0_ADDR(base)                   ((base) + (0x080))

/* 寄存器说明：Vsensor控制使能寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_ADDR(base)         ((base) + (0x100))

/* 寄存器说明：Vsensor bypass寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_ADDR(base)     ((base) + (0x104))

/* 寄存器说明：Vsensor控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_ADDR(base)            ((base) + (0x108))

/* 寄存器说明：SVFD配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_ADDR(base)      ((base) + (0x10C))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_ADDR(base)      ((base) + (0x110))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_ADDR(base)      ((base) + (0x114))

/* 寄存器说明：SVFD状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_ADDR(base)   ((base) + (0x118))

/* 寄存器说明：SVFD测试状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_ADDR(base)       ((base) + (0x11C))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_ADDR(base)      ((base) + (0x120))

/* 寄存器说明：Vsensor控制使能寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_EN_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_ADDR(base)             ((base) + (0x130))

/* 寄存器说明：Vsensor bypass寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_ADDR(base)         ((base) + (0x134))

/* 寄存器说明：Vsensor控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CTRL_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CTRL_L3_ADDR(base)                ((base) + (0x138))

/* 寄存器说明：SVFD配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_ADDR(base)          ((base) + (0x13C))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_ADDR(base)          ((base) + (0x140))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_ADDR(base)          ((base) + (0x144))

/* 寄存器说明：SVFD状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_UNION */
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_ADDR(base)       ((base) + (0x148))

/* 寄存器说明：SVFD测试状态寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_TEST_STAT_L3_UNION */
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_ADDR(base)           ((base) + (0x14C))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_UNION */
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_ADDR(base)          ((base) + (0x150))

/* 寄存器说明：APLL0 SSC控制寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0SSCCTRL_UNION */
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_ADDR(base)              ((base) + (0x1E0))

/* 寄存器说明：APLL1 SSC控制寄存器
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1SSCCTRL_UNION */
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_ADDR(base)              ((base) + (0x1E4))

/* 寄存器说明：APLL0系统控制寄存器0寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG0_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG0_ADDR(base)                ((base) + (0x1E8))

/* 寄存器说明：APLL0系统控制寄存器1寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG1_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG1_ADDR(base)                ((base) + (0x1EC))

/* 寄存器说明：APLL0系统控制寄存器2寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG2_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG2_ADDR(base)                ((base) + (0x1F0))

/* 寄存器说明：APLL1系统控制寄存器0寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG0_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG0_ADDR(base)                ((base) + (0x1F4))

/* 寄存器说明：APLL1系统控制寄存器1寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG1_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG1_ADDR(base)                ((base) + (0x1F8))

/* 寄存器说明：APLL1系统控制寄存器2寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG2_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG2_ADDR(base)                ((base) + (0x1FC))

/* 寄存器说明：FCM子系统时钟使能寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_FCM_CLKEN_UNION */
#define SOC_CORECRG_L_L3_FCM_CLKEN_ADDR(base)                 ((base) + (0x200))

/* 寄存器说明：APLL0系统状态寄存器0
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_STAT_UNION */
#define SOC_CORECRG_L_L3_APLL0_STAT_ADDR(base)                ((base) + (0x250))

/* 寄存器说明：APLL1系统状态寄存器0
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_STAT_UNION */
#define SOC_CORECRG_L_L3_APLL1_STAT_ADDR(base)                ((base) + (0x254))

/* 寄存器说明：APLL0系统控制寄存器3寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL0_CFG3_UNION */
#define SOC_CORECRG_L_L3_APLL0_CFG3_ADDR(base)                ((base) + (0x258))

/* 寄存器说明：APLL1系统控制寄存器3寄存器。
   位域定义UNION结构:  SOC_CORECRG_L_L3_APLL1_CFG3_UNION */
#define SOC_CORECRG_L_L3_APLL1_CFG3_ADDR(base)                ((base) + (0x25C))


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
 结构名    : SOC_CORECRG_L_L3_PEREN0_UNION
 结构说明  : PEREN0 寄存器结构定义。地址偏移量:0x000，初值:0x00000000，宽度:32
 寄存器说明: 外设时钟使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gt_clk_apll0_sscg : 1;  /* bit[0]    :  */
        unsigned int  gt_clk_apll1_sscg : 1;  /* bit[1]    :  */
        unsigned int  reserved_0        : 1;  /* bit[2]    :  */
        unsigned int  reserved_1        : 1;  /* bit[3]    :  */
        unsigned int  reserved_2        : 1;  /* bit[4]    :  */
        unsigned int  reserved_3        : 1;  /* bit[5]    :  */
        unsigned int  reserved_4        : 1;  /* bit[6]    :  */
        unsigned int  reserved_5        : 1;  /* bit[7]    :  */
        unsigned int  reserved_6        : 1;  /* bit[8]    :  */
        unsigned int  reserved_7        : 1;  /* bit[9]    :  */
        unsigned int  reserved_8        : 1;  /* bit[10]   :  */
        unsigned int  reserved_9        : 1;  /* bit[11]   :  */
        unsigned int  reserved_10       : 1;  /* bit[12]   :  */
        unsigned int  reserved_11       : 1;  /* bit[13]   :  */
        unsigned int  reserved_12       : 1;  /* bit[14]   :  */
        unsigned int  reserved_13       : 1;  /* bit[15]   :  */
        unsigned int  reserved_14       : 1;  /* bit[16]   :  */
        unsigned int  reserved_15       : 1;  /* bit[17]   :  */
        unsigned int  reserved_16       : 1;  /* bit[18]   :  */
        unsigned int  reserved_17       : 1;  /* bit[19]   :  */
        unsigned int  reserved_18       : 1;  /* bit[20]   :  */
        unsigned int  reserved_19       : 1;  /* bit[21]   :  */
        unsigned int  reserved_20       : 1;  /* bit[22]   :  */
        unsigned int  reserved_21       : 1;  /* bit[23]   :  */
        unsigned int  reserved_22       : 1;  /* bit[24]   :  */
        unsigned int  reserved_23       : 7;  /* bit[25-31]: 外设时钟使能控制：
                                                             0：写0无效果；
                                                             1：使能IP时钟。 */
    } reg;
} SOC_CORECRG_L_L3_PEREN0_UNION;
#endif
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll0_sscg_START  (0)
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll0_sscg_END    (0)
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll1_sscg_START  (1)
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll1_sscg_END    (1)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_PERDIS0_UNION
 结构说明  : PERDIS0 寄存器结构定义。地址偏移量:0x004，初值:0x00000000，宽度:32
 寄存器说明: 外设时钟禁止寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gt_clk_apll0_sscg : 1;  /* bit[0]    :  */
        unsigned int  gt_clk_apll1_sscg : 1;  /* bit[1]    :  */
        unsigned int  reserved_0        : 1;  /* bit[2]    :  */
        unsigned int  reserved_1        : 1;  /* bit[3]    :  */
        unsigned int  reserved_2        : 1;  /* bit[4]    :  */
        unsigned int  reserved_3        : 1;  /* bit[5]    :  */
        unsigned int  reserved_4        : 1;  /* bit[6]    :  */
        unsigned int  reserved_5        : 1;  /* bit[7]    :  */
        unsigned int  reserved_6        : 1;  /* bit[8]    :  */
        unsigned int  reserved_7        : 1;  /* bit[9]    :  */
        unsigned int  reserved_8        : 1;  /* bit[10]   :  */
        unsigned int  reserved_9        : 1;  /* bit[11]   :  */
        unsigned int  reserved_10       : 1;  /* bit[12]   :  */
        unsigned int  reserved_11       : 1;  /* bit[13]   :  */
        unsigned int  reserved_12       : 1;  /* bit[14]   :  */
        unsigned int  reserved_13       : 1;  /* bit[15]   :  */
        unsigned int  reserved_14       : 1;  /* bit[16]   :  */
        unsigned int  reserved_15       : 1;  /* bit[17]   :  */
        unsigned int  reserved_16       : 1;  /* bit[18]   :  */
        unsigned int  reserved_17       : 1;  /* bit[19]   :  */
        unsigned int  reserved_18       : 1;  /* bit[20]   :  */
        unsigned int  reserved_19       : 1;  /* bit[21]   :  */
        unsigned int  reserved_20       : 1;  /* bit[22]   :  */
        unsigned int  reserved_21       : 1;  /* bit[23]   :  */
        unsigned int  reserved_22       : 1;  /* bit[24]   :  */
        unsigned int  reserved_23       : 7;  /* bit[25-31]: 外设时钟禁止控制：
                                                             0：写0无效果；
                                                             1：禁止IP时钟。 */
    } reg;
} SOC_CORECRG_L_L3_PERDIS0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll0_sscg_START  (0)
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll0_sscg_END    (0)
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll1_sscg_START  (1)
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll1_sscg_END    (1)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_PERCLKEN0_UNION
 结构说明  : PERCLKEN0 寄存器结构定义。地址偏移量:0x008，初值:0x00000003，宽度:32
 寄存器说明: 外设时钟使能状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gt_clk_apll0_sscg : 1;  /* bit[0]    :  */
        unsigned int  gt_clk_apll1_sscg : 1;  /* bit[1]    :  */
        unsigned int  reserved_0        : 1;  /* bit[2]    :  */
        unsigned int  reserved_1        : 1;  /* bit[3]    :  */
        unsigned int  reserved_2        : 1;  /* bit[4]    :  */
        unsigned int  reserved_3        : 1;  /* bit[5]    :  */
        unsigned int  reserved_4        : 1;  /* bit[6]    :  */
        unsigned int  reserved_5        : 1;  /* bit[7]    :  */
        unsigned int  reserved_6        : 1;  /* bit[8]    :  */
        unsigned int  reserved_7        : 1;  /* bit[9]    :  */
        unsigned int  reserved_8        : 1;  /* bit[10]   :  */
        unsigned int  reserved_9        : 1;  /* bit[11]   :  */
        unsigned int  reserved_10       : 1;  /* bit[12]   :  */
        unsigned int  reserved_11       : 1;  /* bit[13]   :  */
        unsigned int  reserved_12       : 1;  /* bit[14]   :  */
        unsigned int  reserved_13       : 1;  /* bit[15]   :  */
        unsigned int  reserved_14       : 1;  /* bit[16]   :  */
        unsigned int  reserved_15       : 1;  /* bit[17]   :  */
        unsigned int  reserved_16       : 1;  /* bit[18]   :  */
        unsigned int  reserved_17       : 1;  /* bit[19]   :  */
        unsigned int  reserved_18       : 1;  /* bit[20]   :  */
        unsigned int  reserved_19       : 1;  /* bit[21]   :  */
        unsigned int  reserved_20       : 1;  /* bit[22]   :  */
        unsigned int  reserved_21       : 1;  /* bit[23]   :  */
        unsigned int  reserved_22       : 1;  /* bit[24]   :  */
        unsigned int  reserved_23       : 7;  /* bit[25-31]: 外设时钟使能状态：
                                                             0：IP时钟使能撤销状态；
                                                             1：IP时钟使能状态。 */
    } reg;
} SOC_CORECRG_L_L3_PERCLKEN0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll0_sscg_START  (0)
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll0_sscg_END    (0)
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll1_sscg_START  (1)
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll1_sscg_END    (1)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_PERSTAT0_UNION
 结构说明  : PERSTAT0 寄存器结构定义。地址偏移量:0x00C，初值:0x00000003，宽度:32
 寄存器说明: 外设时钟最终状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  st_clk_apll0_sscg : 1;  /* bit[0]    :  */
        unsigned int  st_clk_apll1_sscg : 1;  /* bit[1]    :  */
        unsigned int  reserved_0        : 1;  /* bit[2]    :  */
        unsigned int  reserved_1        : 1;  /* bit[3]    :  */
        unsigned int  reserved_2        : 1;  /* bit[4]    :  */
        unsigned int  reserved_3        : 1;  /* bit[5]    :  */
        unsigned int  reserved_4        : 1;  /* bit[6]    :  */
        unsigned int  reserved_5        : 1;  /* bit[7]    :  */
        unsigned int  reserved_6        : 1;  /* bit[8]    :  */
        unsigned int  reserved_7        : 1;  /* bit[9]    :  */
        unsigned int  reserved_8        : 1;  /* bit[10]   :  */
        unsigned int  reserved_9        : 1;  /* bit[11]   :  */
        unsigned int  reserved_10       : 1;  /* bit[12]   :  */
        unsigned int  reserved_11       : 1;  /* bit[13]   :  */
        unsigned int  reserved_12       : 1;  /* bit[14]   :  */
        unsigned int  reserved_13       : 1;  /* bit[15]   :  */
        unsigned int  reserved_14       : 1;  /* bit[16]   :  */
        unsigned int  reserved_15       : 1;  /* bit[17]   :  */
        unsigned int  reserved_16       : 1;  /* bit[18]   :  */
        unsigned int  reserved_17       : 1;  /* bit[19]   :  */
        unsigned int  reserved_18       : 1;  /* bit[20]   :  */
        unsigned int  reserved_19       : 1;  /* bit[21]   :  */
        unsigned int  reserved_20       : 1;  /* bit[22]   :  */
        unsigned int  reserved_21       : 1;  /* bit[23]   :  */
        unsigned int  reserved_22       : 1;  /* bit[24]   :  */
        unsigned int  reserved_23       : 7;  /* bit[25-31]: 外设时钟最终状态：
                                                             0：IP时钟禁止状态；
                                                             1：IP时钟使能状态。 */
    } reg;
} SOC_CORECRG_L_L3_PERSTAT0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll0_sscg_START  (0)
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll0_sscg_END    (0)
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll1_sscg_START  (1)
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll1_sscg_END    (1)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_PERI_STAT0_UNION
 结构说明  : PERI_STAT0 寄存器结构定义。地址偏移量:0x060，初值:0x00000014，宽度:32
 寄存器说明: 外设状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  st_clk_cpu_little      : 1;  /* bit[0]   :  */
        unsigned int  st_clk_cpu_l3          : 1;  /* bit[1]   :  */
        unsigned int  sw_ack_clk_cpul_pll_sw : 2;  /* bit[2-3] : CPUL时钟源选择状态指示：
                                                                 01：PPLL0 分频时钟
                                                                 10：APLL0 */
        unsigned int  sw_ack_clk_l3_pll_sw   : 2;  /* bit[4-5] : L3时钟源选择状态指示：
                                                                 01：PPLL0分频时钟
                                                                 10：APLL1 */
        unsigned int  reserved               : 26; /* bit[6-31]:  */
    } reg;
} SOC_CORECRG_L_L3_PERI_STAT0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_little_START       (0)
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_little_END         (0)
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_l3_START           (1)
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_l3_END             (1)
#define SOC_CORECRG_L_L3_PERI_STAT0_sw_ack_clk_cpul_pll_sw_START  (2)
#define SOC_CORECRG_L_L3_PERI_STAT0_sw_ack_clk_cpul_pll_sw_END    (3)
#define SOC_CORECRG_L_L3_PERI_STAT0_sw_ack_clk_l3_pll_sw_START    (4)
#define SOC_CORECRG_L_L3_PERI_STAT0_sw_ack_clk_l3_pll_sw_END      (5)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_CLKDIV0_UNION
 结构说明  : CLKDIV0 寄存器结构定义。地址偏移量:0x080，初值:0x00002500，宽度:32
 寄存器说明: 时钟分频比控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dvfs_clkdiv_little : 2;  /* bit[0-1]  : 小核vdm时钟分频比：
                                                              2'h0:1分频；
                                                              2'h1:2分频；
                                                              …
                                                              2'h3：4分频； */
        unsigned int  dvfs_clkdiv_l3     : 2;  /* bit[2-3]  : l3 vdm时钟分频比：
                                                              2'h0:1分频；
                                                              2'h1:2分频；
                                                              …
                                                              2'h3：4分频； */
        unsigned int  reserved_0         : 1;  /* bit[4]    :  */
        unsigned int  reserved_1         : 5;  /* bit[5-9]  :  */
        unsigned int  reserved_2         : 4;  /* bit[10-13]:  */
        unsigned int  reserved_3         : 2;  /* bit[14-15]:  */
        unsigned int  bitmasken          : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                              只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CORECRG_L_L3_CLKDIV0_UNION;
#endif
#define SOC_CORECRG_L_L3_CLKDIV0_dvfs_clkdiv_little_START  (0)
#define SOC_CORECRG_L_L3_CLKDIV0_dvfs_clkdiv_little_END    (1)
#define SOC_CORECRG_L_L3_CLKDIV0_dvfs_clkdiv_l3_START      (2)
#define SOC_CORECRG_L_L3_CLKDIV0_dvfs_clkdiv_l3_END        (3)
#define SOC_CORECRG_L_L3_CLKDIV0_bitmasken_START           (16)
#define SOC_CORECRG_L_L3_CLKDIV0_bitmasken_END             (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_UNION
 结构说明  : VS_CTRL_EN_LITTLE 寄存器结构定义。地址偏移量:0x100，初值:0x00000000，宽度:32
 寄存器说明: Vsensor控制使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vs_ctrl_en_little : 1;  /* bit[0]   : CPU_LITTLE Vsensor控制使能。
                                                            0：Vsensor不使能；
                                                            1：Vsensor使能。 */
        unsigned int  reserved          : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_vs_ctrl_en_little_START  (0)
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_vs_ctrl_en_little_END    (0)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_UNION
 结构说明  : VS_CTRL_BYPASS_LITTLE 寄存器结构定义。地址偏移量:0x104，初值:0x00000001，宽度:32
 寄存器说明: Vsensor bypass寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vs_ctrl_bypass_little : 1;  /* bit[0]   : CPU_BIG Vsensor bypass信号。(不使用)
                                                                0：Vsensor控制逻辑输出门控信号控制ICG；
                                                                1：Vsensor控制逻辑bypass，门控信号一直为1。 */
        unsigned int  reserved              : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_vs_ctrl_bypass_little_START  (0)
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_vs_ctrl_bypass_little_END    (0)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CTRL_LITTLE_UNION
 结构说明  : VS_CTRL_LITTLE 寄存器结构定义。地址偏移量:0x108，初值:0x08000000，宽度:32
 寄存器说明: Vsensor控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                       : 1;  /* bit[0]    :  */
        unsigned int  little_cpu0_wfi_wfe_bypass       : 1;  /* bit[1]    : CPU0 WFI/WFE主动防御的bypass。
                                                                            1'b1:bypass;
                                                                            1'b0:使能。 */
        unsigned int  little_cpu1_wfi_wfe_bypass       : 1;  /* bit[2]    : CPU1 WFI/WFE主动防御的bypass。
                                                                            1'b1:bypass;
                                                                            1'b0:使能。 */
        unsigned int  little_cpu2_wfi_wfe_bypass       : 1;  /* bit[3]    : CPU2 WFI/WFE主动防御的bypass。
                                                                            1'b1:bypass;
                                                                            1'b0:使能。 */
        unsigned int  little_cpu3_wfi_wfe_bypass       : 1;  /* bit[4]    : CPU3 WFI/WFE主动防御的bypass。
                                                                            1'b1:bypass;
                                                                            1'b0:使能。 */
        unsigned int  little_l2_idle_div_mod           : 2;  /* bit[5-6]  : l2 idle自动降频分频模式。
                                                                            00:4分频；
                                                                            01:8分频；
                                                                            10:16分频；
                                                                            11:32分频。 */
        unsigned int  little_cfg_cnt_cpu_wake_quit     : 16; /* bit[7-22] : 主动防御退出超时；计数周期为VDM工作时钟。 */
        unsigned int  little_cpu_wake_up_mode          : 2;  /* bit[23-24]: CPU_LITTLE WFI/WFE主动防御模式。
                                                                            2'b00:一个或一个以上退出WFI/WFE,进入主动防御;
                                                                            2'b01:两个或两个以上退出WFI/WFE,进入主动防御;
                                                                            2'b10:三个或三个以上退出WFI/WFE,进入主动防御;
                                                                            2'b11:四个同时退出WFI/WFE,进入主动防御。 */
        unsigned int  little_cpu_l2_idle_switch_bypass : 1;  /* bit[25]   : CPU_LITTLE 退出L2主动防御功能bypass。
                                                                            1'b1:bypass;
                                                                            1'b0:使能。 */
        unsigned int  little_cpu_l2_idle_gt_en         : 1;  /* bit[26]   : CPU_LITTLE 进入L2 IDLE后自动降频功能使能信号。
                                                                            1'b0:不使能;
                                                                            1'b1:使能，进入L2 IDLE后，主频根据自动降频模式进行分频。 */
        unsigned int  little_dvfs_div_en               : 1;  /* bit[27]   : CPU_LITTLE是否采用DVFS分频比。
                                                                            1'b1: 选择；1'b0：不选择。
                                                                            （静态配置，默认为1） */
        unsigned int  reserved_1                       : 1;  /* bit[28]   :  */
        unsigned int  sel_ckmux_little_icg             : 1;  /* bit[29]   : 时钟选择源头。
                                                                            1'b0：CRG直接输出的时钟；
                                                                            1'b1：SVFD输出的时钟。 */
        unsigned int  cpu_clk_div_cfg_little           : 2;  /* bit[30-31]: DFT测试使用分频比信号 */
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu0_wfi_wfe_bypass_START        (1)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu0_wfi_wfe_bypass_END          (1)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu1_wfi_wfe_bypass_START        (2)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu1_wfi_wfe_bypass_END          (2)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu2_wfi_wfe_bypass_START        (3)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu2_wfi_wfe_bypass_END          (3)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu3_wfi_wfe_bypass_START        (4)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu3_wfi_wfe_bypass_END          (4)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_l2_idle_div_mod_START            (5)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_l2_idle_div_mod_END              (6)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cfg_cnt_cpu_wake_quit_START      (7)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cfg_cnt_cpu_wake_quit_END        (22)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_wake_up_mode_START           (23)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_wake_up_mode_END             (24)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_switch_bypass_START  (25)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_switch_bypass_END    (25)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_gt_en_START          (26)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_gt_en_END            (26)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_dvfs_div_en_START                (27)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_dvfs_div_en_END                  (27)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_sel_ckmux_little_icg_START              (29)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_sel_ckmux_little_icg_END                (29)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_cpu_clk_div_cfg_little_START            (30)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_cpu_clk_div_cfg_little_END              (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_UNION
 结构说明  : VS_SVFD_CTRL0_LITTLE 寄存器结构定义。地址偏移量:0x10C，初值:0x00000000，宽度:32
 寄存器说明: SVFD配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_data_limit_e_little    : 1;  /* bit[0]    : 极限值输出状态下，使能信号。
                                                                       1'b0：清除前一次求值；
                                                                       1'b1：使能极限求值。 */
        unsigned int  reserved_0                  : 1;  /* bit[1]    : 逐级升频模式下，间隔选择：
                                                                       0: 64个cycle
                                                                       1: 128个cycle。 */
        unsigned int  svfd_pulse_width_sel_little : 1;  /* bit[2]    : CPM内部复位脉宽调节。 */
        unsigned int  reserved_1                  : 1;  /* bit[3]    : 预留测试输入。 */
        unsigned int  svfd_ulvt_ll_little         : 4;  /* bit[4-7]  : ULVT长线级联，级数配置PIN。 */
        unsigned int  svfd_ulvt_sl_little         : 4;  /* bit[8-11] : ULVT短线级联，级数配置PIN。 */
        unsigned int  svfd_lvt_ll_little          : 4;  /* bit[12-15]: LVT长线级联，级数配置PIN。 */
        unsigned int  svfd_lvt_sl_little          : 4;  /* bit[16-19]: LVT短线级联，级数配置PIN。 */
        unsigned int  svfd_trim_little            : 2;  /* bit[20-21]: trim信号。
                                                                       Charge pump电流大小配置。 */
        unsigned int  svfd_d_rate1_little         : 2;  /* bit[22-23]: 频率降低比率。
                                                                       00 : 10% 
                                                                       01 : 18.18%
                                                                       10 : 25%
                                                                       11 : reserve。 */
        unsigned int  svfd_d_rate_little          : 2;  /* bit[24-25]: CPU_LITTLE DLL调相速率。
                                                                       00 : 10% 
                                                                       01 : 18.18%
                                                                       10 : 25%
                                                                       11 : reserve。 */
        unsigned int  svfd_off_mode_little        : 1;  /* bit[26]   : svfd降频模式。
                                                                       0：直接升频；
                                                                       1：逐级升频。 */
        unsigned int  svfd_div64_en_little        : 1;  /* bit[27]   : 使能信号，工作模式不翻转。 */
        unsigned int  svfd_cpm_period_little      : 1;  /* bit[28]   : 电压检测采样周期。
                                                                       1'b0：1cycle；
                                                                       1'b1：2cycle。 */
        unsigned int  svfd_edge_sel_little        : 1;  /* bit[29]   : svdf边沿选择。
                                                                       1'b0：上升沿检测； 
                                                                       1'b1：双沿检测（上升沿+下降沿）。 */
        unsigned int  svfd_cmp_data_mode_little   : 2;  /* bit[30-31]: CPM码字的输出模式。
                                                                       00：直接输出；
                                                                       01：最小值输出；
                                                                       10：最大值输出；
                                                                       11：不翻转。 */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_data_limit_e_little_START     (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_data_limit_e_little_END       (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_pulse_width_sel_little_START  (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_pulse_width_sel_little_END    (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_ll_little_START          (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_ll_little_END            (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_sl_little_START          (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_sl_little_END            (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_ll_little_START           (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_ll_little_END             (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_sl_little_START           (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_sl_little_END             (19)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_trim_little_START             (20)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_trim_little_END               (21)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_d_rate1_little_START          (22)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_d_rate1_little_END            (23)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_d_rate_little_START           (24)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_d_rate_little_END             (25)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_off_mode_little_START         (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_off_mode_little_END           (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_div64_en_little_START         (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_div64_en_little_END           (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cpm_period_little_START       (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cpm_period_little_END         (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_edge_sel_little_START         (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_edge_sel_little_END           (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cmp_data_mode_little_START    (30)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cmp_data_mode_little_END      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_UNION
 结构说明  : VS_SVFD_CTRL1_LITTLE 寄存器结构定义。地址偏移量:0x110，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_glitch_test_little   : 1;  /* bit[0]    : Glitch测试使能。 */
        unsigned int  reserved_0                : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  svfd_test_ffs_little      : 8;  /* bit[4-11] : TEST_FFS<1:0> 
                                                                     降频模块预留测试pin */
        unsigned int  svfd_test_cpm_little      : 8;  /* bit[12-19]: TEST_CPM<6:4> CPM DFT筛选预留信号
                                                                      
                                                                     TEST_CPM<3> 
                                                                     Noise_print时钟控制信号
                                                                     0：128分频；
                                                                     1：2分频；
                                                                     
                                                                     
                                                                     TEST_CPM<2>:保留
                                                                     TEST_CPM<1>:CPM DFT 筛片jitter测试时能信号
                                                                     0：不使能
                                                                     1：使能
                                                                     TEST_CPM<0>:CPM DFT筛片时钟选择信号：
                                                                     0：选原时钟
                                                                     1：选降频时钟 */
        unsigned int  svfd_off_time_step_little : 7;  /* bit[20-26]: SVFD逐级升频，每级代表细粒度配置间隔时间。 */
        unsigned int  reserved_1                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_glitch_test_little_START    (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_glitch_test_little_END      (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_ffs_little_START       (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_ffs_little_END         (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_cpm_little_START       (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_cpm_little_END         (19)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_off_time_step_little_START  (20)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_off_time_step_little_END    (26)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_UNION
 结构说明  : VS_SVFD_CTRL2_LITTLE 寄存器结构定义。地址偏移量:0x114，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_enalbe_mode_little     : 2;  /* bit[0-1]  : 电压检测模式选择。
                                                                       00 :reserve；
                                                                       01 : IP内部电压检测(CPM)；
                                                                       10 : IP外部电压检测(VDM)；
                                                                       11 :VDM+CPM。 */
        unsigned int  svfs_cpm_threshold_r_little : 6;  /* bit[2-7]  : CPM上升沿监测BANK的阈值。 */
        unsigned int  svfs_cpm_threshold_f_little : 6;  /* bit[8-13] : CPM下降沿监测BANK的阈值。 */
        unsigned int  vol_drop_en_little          : 1;  /* bit[14]   : CPU_LITTLE 低温低压使能分频控制信号 */
        unsigned int  svfd_cmp_data_clr_little    : 1;  /* bit[15]   : cmp码字复位信号。
                                                                       1'b0：保存正常码字。
                                                                       1'b1：把码字置全1。 */
        unsigned int  bitmasken                   : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfd_enalbe_mode_little_START      (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfd_enalbe_mode_little_END        (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_r_little_START  (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_r_little_END    (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_f_little_START  (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_f_little_END    (13)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_vol_drop_en_little_START           (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_vol_drop_en_little_END             (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfd_cmp_data_clr_little_START     (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfd_cmp_data_clr_little_END       (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_bitmasken_START                    (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_bitmasken_END                      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_UNION
 结构说明  : VS_CPM_DATA_STAT_LITTLE 寄存器结构定义。地址偏移量:0x118，初值:0x00000000，宽度:32
 寄存器说明: SVFD状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_cpm_data_r_little     : 8;  /* bit[0-7]  : 上升沿检测编码输出，由128分频时钟寄存器输出。 */
        unsigned int  svfd_cpm_data_f_little     : 8;  /* bit[8-15] : 下降沿检测编码输出，由128分频时钟寄存器输出。 */
        unsigned int  svfd_dll_lock_little       : 1;  /* bit[16]   : DLL lock指示寄存器。 */
        unsigned int  svfd_cpm_data_norst_little : 6;  /* bit[17-22]: 异常复位时保存的最后一个cpm码字。 */
        unsigned int  reserved                   : 9;  /* bit[23-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_r_little_START      (0)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_r_little_END        (7)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_f_little_START      (8)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_f_little_END        (15)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_dll_lock_little_START        (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_dll_lock_little_END          (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_norst_little_START  (17)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_norst_little_END    (22)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_UNION
 结构说明  : VS_TEST_STAT_LITTLE 寄存器结构定义。地址偏移量:0x11C，初值:0x00000000，宽度:32
 寄存器说明: SVFD测试状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_test_out_ffs_little     : 4;  /* bit[0-3]  : bit[3]:预留测试输出pin;
                                                                        bit[2]:降频指示信号。
                                                                        bit[1]:DLL再锁定时处于降频状态;
                                                                        bit[0]:DLL再锁定后有失锁标志位。 */
        unsigned int  svfd_test_out_cpm_little     : 4;  /* bit[4-7]  : 预留测试输出PIN。 */
        unsigned int  svfd_glitch_result_little    : 1;  /* bit[8]    : Glitch检测结果，0为无毛刺，通过。 */
        unsigned int  idle_low_freq_en_little      : 1;  /* bit[9]    : 小核IDLE使能分频信号 */
        unsigned int  vbat_drop_protect_ind_little : 1;  /* bit[10]   : 小核使能低温低压分频状态信号 */
        unsigned int  reserved                     : 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_ffs_little_START      (0)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_ffs_little_END        (3)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_cpm_little_START      (4)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_cpm_little_END        (7)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_glitch_result_little_START     (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_glitch_result_little_END       (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_idle_low_freq_en_little_START       (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_idle_low_freq_en_little_END         (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_vbat_drop_protect_ind_little_START  (10)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_vbat_drop_protect_ind_little_END    (10)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_UNION
 结构说明  : VS_SVFD_CTRL3_LITTLE 寄存器结构定义。地址偏移量:0x120，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_svfd_freq_down_en_little  : 1;  /* bit[0]   : 软件触发svfd降频使能信号。
                                                                       1'b0：不降频。
                                                                       1'b1：降频。 */
        unsigned int  sel_svfd_freq_down_en_little : 1;  /* bit[1]   : 选择vdm使用的freq_down_en来源。
                                                                       1'b0：使用PMC输出的freq_down_en。
                                                                       1'b1：使用寄存器输出的freq_down_en。 */
        unsigned int  reserved                     : 30; /* bit[2-31]:  */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_sw_svfd_freq_down_en_little_START   (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_sw_svfd_freq_down_en_little_END     (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_sel_svfd_freq_down_en_little_START  (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_LITTLE_sel_svfd_freq_down_en_little_END    (1)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CTRL_EN_L3_UNION
 结构说明  : VS_CTRL_EN_L3 寄存器结构定义。地址偏移量:0x130，初值:0x00000000，宽度:32
 寄存器说明: Vsensor控制使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vs_ctrl_en_l3 : 1;  /* bit[0]   : L3(FCM) Vsensor控制使能。
                                                        0：Vsensor不使能；
                                                        1：Vsensor使能。 */
        unsigned int  reserved      : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_EN_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_vs_ctrl_en_l3_START  (0)
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_vs_ctrl_en_l3_END    (0)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_UNION
 结构说明  : VS_CTRL_BYPASS_L3 寄存器结构定义。地址偏移量:0x134，初值:0x00000001，宽度:32
 寄存器说明: Vsensor bypass寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vs_ctrl_bypass_l3 : 1;  /* bit[0]   : CPU_BIG Vsensor bypass信号。(不使用)
                                                            0：Vsensor控制逻辑输出门控信号控制ICG；
                                                            1：Vsensor控制逻辑bypass，门控信号一直为1。 */
        unsigned int  reserved          : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_vs_ctrl_bypass_l3_START  (0)
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_vs_ctrl_bypass_l3_END    (0)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CTRL_L3_UNION
 结构说明  : VS_CTRL_L3 寄存器结构定义。地址偏移量:0x138，初值:0x08000000，宽度:32
 寄存器说明: Vsensor控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                   : 1;  /* bit[0]    :  */
        unsigned int  l3_cpu0_wfi_wfe_bypass       : 1;  /* bit[1]    : CPU0 WFI/WFE主动防御的bypass。
                                                                        1'b1:bypass;
                                                                        1'b0:使能。 */
        unsigned int  l3_cpu1_wfi_wfe_bypass       : 1;  /* bit[2]    : CPU1 WFI/WFE主动防御的bypass。
                                                                        1'b1:bypass;
                                                                        1'b0:使能。 */
        unsigned int  l3_cpu2_wfi_wfe_bypass       : 1;  /* bit[3]    : CPU2 WFI/WFE主动防御的bypass。
                                                                        1'b1:bypass;
                                                                        1'b0:使能。 */
        unsigned int  l3_cpu3_wfi_wfe_bypass       : 1;  /* bit[4]    : CPU3 WFI/WFE主动防御的bypass。
                                                                        1'b1:bypass;
                                                                        1'b0:使能。 */
        unsigned int  l3_l2_idle_div_mod           : 2;  /* bit[5-6]  : l2 idle自动降频分频模式。
                                                                        00:4分频；
                                                                        01:8分频；
                                                                        10:16分频；
                                                                        11:32分频。 */
        unsigned int  l3_cfg_cnt_cpu_wake_quit     : 16; /* bit[7-22] : 主动防御退出超时；计数周期为VDM工作时钟。 */
        unsigned int  l3_cpu_wake_up_mode          : 2;  /* bit[23-24]: L3(FCM) WFI/WFE主动防御模式。
                                                                        2'b00:一个或一个以上退出WFI/WFE,进入主动防御;
                                                                        2'b01:两个或两个以上退出WFI/WFE,进入主动防御;
                                                                        2'b10:三个或三个以上退出WFI/WFE,进入主动防御;
                                                                        2'b11:四个同时退出WFI/WFE,进入主动防御。 */
        unsigned int  l3_cpu_l2_idle_switch_bypass : 1;  /* bit[25]   : L3(FCM) 退出L2主动防御功能bypass。
                                                                        1'b1:bypass;
                                                                        1'b0:使能。 */
        unsigned int  l3_cpu_l2_idle_gt_en         : 1;  /* bit[26]   : L3(FCM) 进入L2 IDLE后自动降频功能使能信号。
                                                                        1'b0:不使能;
                                                                        1'b1:使能，进入L2 IDLE后，主频根据自动降频模式进行分频。 */
        unsigned int  l3_dvfs_div_en               : 1;  /* bit[27]   : L3(FCM)是否采用DVFS分频比。
                                                                        1'b1: 选择；1'b0：不选择。
                                                                        （静态配置，默认为1） */
        unsigned int  reserved_1                   : 1;  /* bit[28]   :  */
        unsigned int  sel_ckmux_l3_icg             : 1;  /* bit[29]   : 时钟选择源头。
                                                                        1'b0：CRG直接输出的时钟；
                                                                        1'b1：SVFD输出的时钟。 */
        unsigned int  cpu_clk_div_cfg_l3           : 2;  /* bit[30-31]: DFT测试使用分频比信号 */
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu0_wfi_wfe_bypass_START        (1)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu0_wfi_wfe_bypass_END          (1)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu1_wfi_wfe_bypass_START        (2)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu1_wfi_wfe_bypass_END          (2)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu2_wfi_wfe_bypass_START        (3)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu2_wfi_wfe_bypass_END          (3)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu3_wfi_wfe_bypass_START        (4)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu3_wfi_wfe_bypass_END          (4)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_l2_idle_div_mod_START            (5)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_l2_idle_div_mod_END              (6)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cfg_cnt_cpu_wake_quit_START      (7)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cfg_cnt_cpu_wake_quit_END        (22)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_wake_up_mode_START           (23)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_wake_up_mode_END             (24)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_switch_bypass_START  (25)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_switch_bypass_END    (25)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_gt_en_START          (26)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_gt_en_END            (26)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_dvfs_div_en_START                (27)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_dvfs_div_en_END                  (27)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_sel_ckmux_l3_icg_START              (29)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_sel_ckmux_l3_icg_END                (29)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_cpu_clk_div_cfg_l3_START            (30)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_cpu_clk_div_cfg_l3_END              (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_UNION
 结构说明  : VS_SVFD_CTRL0_L3 寄存器结构定义。地址偏移量:0x13C，初值:0x00000000，宽度:32
 寄存器说明: SVFD配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_data_limit_e_l3    : 1;  /* bit[0]    : 极限值输出状态下，使能信号。
                                                                   1'b0：清除前一次求值；
                                                                   1'b1：使能极限求值。 */
        unsigned int  reserved_0              : 1;  /* bit[1]    : 逐级升频模式下，间隔选择：
                                                                   0: 64个cycle
                                                                   1: 128个cycle。 */
        unsigned int  svfd_pulse_width_sel_l3 : 1;  /* bit[2]    : CPM内部复位脉宽调节。 */
        unsigned int  reserved_1              : 1;  /* bit[3]    : 预留测试输入。 */
        unsigned int  svfd_ulvt_ll_l3         : 4;  /* bit[4-7]  : ULVT长线级联，级数配置PIN。 */
        unsigned int  svfd_ulvt_sl_l3         : 4;  /* bit[8-11] : ULVT短线级联，级数配置PIN。 */
        unsigned int  svfd_lvt_ll_l3          : 4;  /* bit[12-15]: LVT长线级联，级数配置PIN。 */
        unsigned int  svfd_lvt_sl_l3          : 4;  /* bit[16-19]: LVT短线级联，级数配置PIN。 */
        unsigned int  svfd_trim_l3            : 2;  /* bit[20-21]: trim信号。
                                                                   Charge pump电流大小配置。 */
        unsigned int  svfd_d_rate1_l3         : 2;  /* bit[22-23]: 频率降低比率。
                                                                   00 : 10% 
                                                                   01 : 18.18%
                                                                   10 : 25%
                                                                   11 : reserve。 */
        unsigned int  svfd_d_rate_l3          : 2;  /* bit[24-25]: L3(FCM) DLL调相速率。
                                                                   00 : 10% 
                                                                   01 : 18.18%
                                                                   10 : 25%
                                                                   11 : reserve。 */
        unsigned int  svfd_off_mode_l3        : 1;  /* bit[26]   : svfd降频模式。
                                                                   0：直接升频；
                                                                   1：逐级升频。 */
        unsigned int  svfd_div64_en_l3        : 1;  /* bit[27]   : 使能信号，工作模式不翻转。 */
        unsigned int  svfd_cpm_period_l3      : 1;  /* bit[28]   : 电压检测采样周期。
                                                                   1'b0：1cycle；
                                                                   1'b1：2cycle。 */
        unsigned int  svfd_edge_sel_l3        : 1;  /* bit[29]   : svdf边沿选择。
                                                                   1'b0：上升沿检测； 
                                                                   1'b1：双沿检测（上升沿+下降沿）。 */
        unsigned int  svfd_cmp_data_mode_l3   : 2;  /* bit[30-31]: CPM码字的输出模式。
                                                                   00：直接输出；
                                                                   01：最小值输出；
                                                                   10：最大值输出；
                                                                   11：不翻转。 */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_data_limit_e_l3_START     (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_data_limit_e_l3_END       (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_pulse_width_sel_l3_START  (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_pulse_width_sel_l3_END    (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_ll_l3_START          (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_ll_l3_END            (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_sl_l3_START          (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_sl_l3_END            (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_ll_l3_START           (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_ll_l3_END             (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_sl_l3_START           (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_sl_l3_END             (19)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_trim_l3_START             (20)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_trim_l3_END               (21)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_d_rate1_l3_START          (22)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_d_rate1_l3_END            (23)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_d_rate_l3_START           (24)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_d_rate_l3_END             (25)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_off_mode_l3_START         (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_off_mode_l3_END           (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_div64_en_l3_START         (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_div64_en_l3_END           (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cpm_period_l3_START       (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cpm_period_l3_END         (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_edge_sel_l3_START         (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_edge_sel_l3_END           (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cmp_data_mode_l3_START    (30)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cmp_data_mode_l3_END      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_UNION
 结构说明  : VS_SVFD_CTRL1_L3 寄存器结构定义。地址偏移量:0x140，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_glitch_test_little : 1;  /* bit[0]    : Glitch测试使能。 */
        unsigned int  reserved_0              : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  svfd_test_ffs_little    : 8;  /* bit[4-11] : TEST_FFS<1:0> 
                                                                   降频模块预留测试pin */
        unsigned int  svfd_test_cpm_little    : 8;  /* bit[12-19]: TEST_CPM<6:4> CPM DFT筛选预留信号
                                                                    
                                                                   TEST_CPM<3> 
                                                                   Noise_print时钟控制信号
                                                                   0：128分频；
                                                                   1：2分频；
                                                                   
                                                                   
                                                                   TEST_CPM<2>:保留
                                                                   TEST_CPM<1>:CPM DFT 筛片jitter测试时能信号
                                                                   0：不使能
                                                                   1：使能
                                                                   TEST_CPM<0>:CPM DFT筛片时钟选择信号：
                                                                   0：选原时钟
                                                                   1：选降频时钟 */
        unsigned int  svfd_off_time_step_l3   : 7;  /* bit[20-26]: SVFD逐级升频，每级代表细粒度配置间隔时间。 */
        unsigned int  reserved_1              : 5;  /* bit[27-31]:  */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_glitch_test_little_START  (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_glitch_test_little_END    (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_ffs_little_START     (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_ffs_little_END       (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_cpm_little_START     (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_cpm_little_END       (19)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_off_time_step_l3_START    (20)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_off_time_step_l3_END      (26)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_UNION
 结构说明  : VS_SVFD_CTRL2_L3 寄存器结构定义。地址偏移量:0x144，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_enalbe_mode_l3     : 2;  /* bit[0-1]  : 电压检测模式选择。
                                                                   00 :reserve；
                                                                   01 : IP内部电压检测(CPM)；
                                                                   10 : IP外部电压检测(VDM)；
                                                                   11 :VDM+CPM。 */
        unsigned int  svfs_cpm_threshold_r_l3 : 6;  /* bit[2-7]  : CPM上升沿监测BANK的阈值。 */
        unsigned int  svfs_cpm_threshold_f_l3 : 6;  /* bit[8-13] : CPM下降沿监测BANK的阈值。 */
        unsigned int  vol_drop_en_l3          : 1;  /* bit[14]   : CPU_L3 低温低压使能分频控制信号 */
        unsigned int  svfd_cmp_data_clr_l3    : 1;  /* bit[15]   : cmp码字复位信号。
                                                                   1'b0：保存正常码字。
                                                                   1'b1：把码字置全1。 */
        unsigned int  bitmasken               : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfd_enalbe_mode_l3_START      (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfd_enalbe_mode_l3_END        (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_r_l3_START  (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_r_l3_END    (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_f_l3_START  (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_f_l3_END    (13)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_vol_drop_en_l3_START           (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_vol_drop_en_l3_END             (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfd_cmp_data_clr_l3_START     (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfd_cmp_data_clr_l3_END       (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_bitmasken_START                (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_bitmasken_END                  (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_UNION
 结构说明  : VS_CPM_DATA_STAT_L3 寄存器结构定义。地址偏移量:0x148，初值:0x00000000，宽度:32
 寄存器说明: SVFD状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_cpm_data_r_l3     : 8;  /* bit[0-7]  : 上升沿检测编码输出，由128分频时钟寄存器输出。 */
        unsigned int  svfd_cpm_data_f_l3     : 8;  /* bit[8-15] : 下降沿检测编码输出，由128分频时钟寄存器输出。 */
        unsigned int  svfd_dll_lock_l3       : 1;  /* bit[16]   : DLL lock指示寄存器。 */
        unsigned int  svfd_cpm_data_norst_l3 : 6;  /* bit[17-22]: 异常复位时保存的最后一个cpm码字。 */
        unsigned int  reserved               : 9;  /* bit[23-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_r_l3_START      (0)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_r_l3_END        (7)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_f_l3_START      (8)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_f_l3_END        (15)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_dll_lock_l3_START        (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_dll_lock_l3_END          (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_norst_l3_START  (17)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_norst_l3_END    (22)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_TEST_STAT_L3_UNION
 结构说明  : VS_TEST_STAT_L3 寄存器结构定义。地址偏移量:0x14C，初值:0x00000000，宽度:32
 寄存器说明: SVFD测试状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_test_out_ffs_l3     : 4;  /* bit[0-3]  : bit[3]:预留测试输出pin;
                                                                    bit[2]:降频指示信号。
                                                                    bit[1]:DLL再锁定时处于降频状态;
                                                                    bit[0]:DLL再锁定后有失锁标志位。 */
        unsigned int  svfd_test_out_cpm_l3     : 4;  /* bit[4-7]  : 预留测试输出PIN。 */
        unsigned int  svfd_glitch_result_l3    : 1;  /* bit[8]    : Glitch检测结果，0为无毛刺，通过。 */
        unsigned int  idle_low_freq_en_l3      : 1;  /* bit[9]    : L3IDLE使能分频信号 */
        unsigned int  vbat_drop_protect_ind_l3 : 1;  /* bit[10]   : L3使能低温低压分频状态信号 */
        unsigned int  reserved                 : 21; /* bit[11-31]:  */
    } reg;
} SOC_CORECRG_L_L3_VS_TEST_STAT_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_ffs_l3_START      (0)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_ffs_l3_END        (3)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_cpm_l3_START      (4)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_cpm_l3_END        (7)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_glitch_result_l3_START     (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_glitch_result_l3_END       (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_idle_low_freq_en_l3_START       (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_idle_low_freq_en_l3_END         (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_vbat_drop_protect_ind_l3_START  (10)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_vbat_drop_protect_ind_l3_END    (10)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_UNION
 结构说明  : VS_SVFD_CTRL3_L3 寄存器结构定义。地址偏移量:0x150，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_svfd_freq_down_en_l3  : 1;  /* bit[0]   : 软件触发svfd降频使能信号。
                                                                   1'b0：不降频。
                                                                   1'b1：降频。 */
        unsigned int  sel_svfd_freq_down_en_l3 : 1;  /* bit[1]   : 选择vdm使用的freq_down_en来源。
                                                                   1'b0：使用PMC输出的freq_down_en。
                                                                   1'b1：使用寄存器输出的freq_down_en。 */
        unsigned int  reserved                 : 30; /* bit[2-31]:  */
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_sw_svfd_freq_down_en_l3_START   (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_sw_svfd_freq_down_en_l3_END     (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_sel_svfd_freq_down_en_l3_START  (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL3_L3_sel_svfd_freq_down_en_l3_END    (1)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL0SSCCTRL_UNION
 结构说明  : APLL0SSCCTRL 寄存器结构定义。地址偏移量:0x1E0，初值:0x0000019F，宽度:32
 寄存器说明: APLL0 SSC控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_ssc_reset                 : 1;  /* bit[0]    : 复位信号，高有效。
                                                                           0：复位取消；
                                                                           1：复位有效。
                                                                           注：不能在PLL工作过程中复位，否则会导致PLL失锁。 */
        unsigned int  apll0_ssc_disable               : 1;  /* bit[1]    : Bypass the modulator，高有效。
                                                                           0：正常工作；
                                                                           1：bypass。 */
        unsigned int  apll0_ssc_downspread            : 1;  /* bit[2]    : 选择center spread或down spread。
                                                                           0：Center Spread；
                                                                           1：Down Spread。 */
        unsigned int  apll0_ssc_spread                : 3;  /* bit[3-5]  : 设置调制深度(spread%)。
                                                                           3'b000 = 0；
                                                                           3'b001 = 0.049%；
                                                                           3'b010 = 0.098%；
                                                                           3'b011 = 0.195%；
                                                                           3'b100 = 0.391%；
                                                                           3'b101 = 0.781%；
                                                                           3'b110 = 1.563%；
                                                                           3'b111 = 3.125%。 */
        unsigned int  apll0_ssc_divval                : 4;  /* bit[6-9]  : 设置频谱调制的频率，一般约为32KHz；
                                                                           freq=(CLKSSCG / (DIVVAL * #points))，其中points为128；当REFDIV=1时，CLKSSCG=FREF。 */
        unsigned int  apll1_ssc_bypass_ssmod_dynammic : 1;  /* bit[10]   : bypass ssmod 动态切换
                                                                           0：打开动态切换；
                                                                           1：关闭动态切换。 */
        unsigned int  reserved                        : 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_APLL0SSCCTRL_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_reset_START                  (0)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_reset_END                    (0)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_disable_START                (1)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_disable_END                  (1)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_downspread_START             (2)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_downspread_END               (2)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_spread_START                 (3)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_spread_END                   (5)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_divval_START                 (6)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_divval_END                   (9)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll1_ssc_bypass_ssmod_dynammic_START  (10)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll1_ssc_bypass_ssmod_dynammic_END    (10)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL1SSCCTRL_UNION
 结构说明  : APLL1SSCCTRL 寄存器结构定义。地址偏移量:0x1E4，初值:0x0000019F，宽度:32
 寄存器说明: APLL1 SSC控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_ssc_reset                 : 1;  /* bit[0]    : 复位信号，高有效。
                                                                           0：复位取消；
                                                                           1：复位有效。
                                                                           注：不能在PLL工作过程中复位，否则会导致PLL失锁。 */
        unsigned int  apll1_ssc_disable               : 1;  /* bit[1]    : Bypass the modulator，高有效。
                                                                           0：正常工作；
                                                                           1：bypass。 */
        unsigned int  apll1_ssc_downspread            : 1;  /* bit[2]    : 选择center spread或down spread。
                                                                           0：Center Spread；
                                                                           1：Down Spread。 */
        unsigned int  apll1_ssc_spread                : 3;  /* bit[3-5]  : 设置调制深度(spread%)。
                                                                           3'b000 = 0；
                                                                           3'b001 = 0.049%；
                                                                           3'b010 = 0.098%；
                                                                           3'b011 = 0.195%；
                                                                           3'b100 = 0.391%；
                                                                           3'b101 = 0.781%；
                                                                           3'b110 = 1.563%；
                                                                           3'b111 = 3.125%。 */
        unsigned int  apll1_ssc_divval                : 4;  /* bit[6-9]  : 设置频谱调制的频率，一般约为32KHz；
                                                                           freq=(CLKSSCG / (DIVVAL * #points))，其中points为128；当REFDIV=1时，CLKSSCG=FREF。 */
        unsigned int  apll1_ssc_bypass_ssmod_dynammic : 1;  /* bit[10]   : bypass ssmod 动态切换
                                                                           0：打开动态切换；
                                                                           1：关闭动态切换。 */
        unsigned int  reserved                        : 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_CORECRG_L_L3_APLL1SSCCTRL_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_reset_START                  (0)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_reset_END                    (0)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_disable_START                (1)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_disable_END                  (1)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_downspread_START             (2)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_downspread_END               (2)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_spread_START                 (3)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_spread_END                   (5)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_divval_START                 (6)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_divval_END                   (9)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_bypass_ssmod_dynammic_START  (10)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_bypass_ssmod_dynammic_END    (10)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL0_CFG0_UNION
 结构说明  : APLL0_CFG0 寄存器结构定义。地址偏移量:0x1E8，初值:0x8C011022，宽度:32
 寄存器说明: APLL0系统控制寄存器0寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sta_reg_sel      : 1;  /* bit[0]    : 寄存器回读选择信号，选择当前读的是哪组状态寄存器：
                                                            0：选择读寄存器STATE0【默认】
                                                            1：选择读寄存器STATE1 */
        unsigned int  read_en          : 1;  /* bit[1]    : fnpll测试输出寄存器读取使能，input_cfg_en_fnpll=1时有效，read_en上升沿更新测试数据，所以更新数据时需要先低后高跳变：
                                                            0：保持测试输出寄存器数据
                                                            1：更新测试输出寄存器数据【默认】 */
        unsigned int  input_cfg_en     : 1;  /* bit[2]    : fnpll小数分频器参数配置使能：
                                                            0：使用内部默认配置【默认】
                                                            1：使用外部配置 */
        unsigned int  test_mode        : 1;  /* bit[3]    : fnpll测试模式使能，当input_cfg_en_fnpll=1时test_mode_fnpll才有效，
                                                            0：关闭测试使能【默认】
                                                            1：开启测试模式 */
        unsigned int  unlock_clear     : 1;  /* bit[4]    : 清零历史失锁指示信号unlock_flag以及历史失锁统计次数，只作为testchip测试使用：
                                                            0：不清零unlock_flag以及历史失锁统计次数；【默认】
                                                            1：清零unlock_flag以及历史失锁统计次数（上升沿触发） */
        unsigned int  dll_en           : 1;  /* bit[5]    : FNPLL dll控制使能：
                                                            0：关闭dll
                                                            1：dll正常工作【默认】 */
        unsigned int  pll_lock_ate_sel : 2;  /* bit[6-7]  : fnpll ATE测试输出信号复用的选择信号，选择ATE测试输出哪组信号，默认为0：
                                                            00：输出lock；【默认】
                                                            01：输出bbpd_calib_success；
                                                            10：输出dtc_calib_satu；
                                                            11：输出dtc_calib_state； */
        unsigned int  test_data_sel    : 4;  /* bit[8-11] : fnpll输出测试数据复用的选择信号，选择测试输出哪组数据，默认为0
                                                            0001：test_data={1'b0,c_out,mux_sel1_test[2:0],mux_sel0_test[2:0]},需要read_en上升沿更新数据;
                                                            0010：test_data={2'b0,k_gain},需要read_en上升沿更新数据;
                                                            0011：test_data={2'b0,dtc_m_bin},需要read_en上升沿更新数据;
                                                            0100：test_data={2'b0,dtc_c_bin},需要read_en上升沿更新数据;
                                                            0101：test_data={2'b0,dtc_o_bin},需要read_en上升沿更新数据;
                                                            0110：test_data={4'b0,coder_out},需要read_en上升沿更新数据;
                                                            0111：test_data={3'b0,phe_out},需要read_en上升沿更新数据;
                                                            其他：test_data=0 */
        unsigned int  bias_ctrl        : 2;  /* bit[12-13]: FNPLL不同电源电压下bias电压档位选择信号 
                                                            00-reserved；【禁配】
                                                            01-电源电压1.2V；
                                                            10-reserved；【禁配】
                                                            11-reserved；【禁配】 */
        unsigned int  icp_ctrl         : 2;  /* bit[14-15]: fnpll的ICP偏置电流选择信号
                                                            3：reserved【禁配】
                                                            2：reserved【禁配】
                                                            1：选择1/4 CP偏置电流
                                                            0：选择1/2 CP偏置电流【默认】 */
        unsigned int  updn_sel         : 1;  /* bit[16]   : fnpll的CP路数选择信号
                                                            1：打开1路CP【默认】
                                                            0：打开2路CP */
        unsigned int  reserved         : 2;  /* bit[17-18]: 保留 */
        unsigned int  int_mask         : 3;  /* bit[19-21]: 中断屏蔽位，为1时中断被屏蔽。中断通过或逻辑连到pll_int。
                                                            xx1：屏蔽unlock_flag；
                                                            x1x：屏蔽bbpd_calib_fail；
                                                            1xx：屏蔽dtc_calib_satu。
                                                            000：全部不屏蔽。【默认】 */
        unsigned int  pfd_div_ratio    : 4;  /* bit[22-25]: dc_ac模块内pfd分频比设置,默认不分频。建议不要配置。
                                                            4'b0000: 1分频（固定默认值）
                                                            4'b0001: 1分频
                                                            4'b0010: 2分频
                                                            ….
                                                            4'b1111: 15分频 */
        unsigned int  freq_threshold   : 6;  /* bit[26-31]: 判断振荡器输出时钟频率的阈值，（0/1禁配）,freq_threshold=pfd_div_ratio*5.5GHz/[4*freq(pfd_clk)]-1；
                                                            (当计算值大于63时取63即可，向上取整）。
                                                            0: （禁配）
                                                            1: （禁配）
                                                            2：振荡频率超过4*（2+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            3：振荡频率超过4*（3+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            4：振荡频率超过4*（4+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            ……
                                                            31:振荡频率超过4*（31+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            ...
                                                            63:振荡频率超过4*(63+1)*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            备注：pfd_div_ratio固定为1分频 */
    } reg;
} SOC_CORECRG_L_L3_APLL0_CFG0_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0_CFG0_sta_reg_sel_START       (0)
#define SOC_CORECRG_L_L3_APLL0_CFG0_sta_reg_sel_END         (0)
#define SOC_CORECRG_L_L3_APLL0_CFG0_read_en_START           (1)
#define SOC_CORECRG_L_L3_APLL0_CFG0_read_en_END             (1)
#define SOC_CORECRG_L_L3_APLL0_CFG0_input_cfg_en_START      (2)
#define SOC_CORECRG_L_L3_APLL0_CFG0_input_cfg_en_END        (2)
#define SOC_CORECRG_L_L3_APLL0_CFG0_test_mode_START         (3)
#define SOC_CORECRG_L_L3_APLL0_CFG0_test_mode_END           (3)
#define SOC_CORECRG_L_L3_APLL0_CFG0_unlock_clear_START      (4)
#define SOC_CORECRG_L_L3_APLL0_CFG0_unlock_clear_END        (4)
#define SOC_CORECRG_L_L3_APLL0_CFG0_dll_en_START            (5)
#define SOC_CORECRG_L_L3_APLL0_CFG0_dll_en_END              (5)
#define SOC_CORECRG_L_L3_APLL0_CFG0_pll_lock_ate_sel_START  (6)
#define SOC_CORECRG_L_L3_APLL0_CFG0_pll_lock_ate_sel_END    (7)
#define SOC_CORECRG_L_L3_APLL0_CFG0_test_data_sel_START     (8)
#define SOC_CORECRG_L_L3_APLL0_CFG0_test_data_sel_END       (11)
#define SOC_CORECRG_L_L3_APLL0_CFG0_bias_ctrl_START         (12)
#define SOC_CORECRG_L_L3_APLL0_CFG0_bias_ctrl_END           (13)
#define SOC_CORECRG_L_L3_APLL0_CFG0_icp_ctrl_START          (14)
#define SOC_CORECRG_L_L3_APLL0_CFG0_icp_ctrl_END            (15)
#define SOC_CORECRG_L_L3_APLL0_CFG0_updn_sel_START          (16)
#define SOC_CORECRG_L_L3_APLL0_CFG0_updn_sel_END            (16)
#define SOC_CORECRG_L_L3_APLL0_CFG0_int_mask_START          (19)
#define SOC_CORECRG_L_L3_APLL0_CFG0_int_mask_END            (21)
#define SOC_CORECRG_L_L3_APLL0_CFG0_pfd_div_ratio_START     (22)
#define SOC_CORECRG_L_L3_APLL0_CFG0_pfd_div_ratio_END       (25)
#define SOC_CORECRG_L_L3_APLL0_CFG0_freq_threshold_START    (26)
#define SOC_CORECRG_L_L3_APLL0_CFG0_freq_threshold_END      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL0_CFG1_UNION
 结构说明  : APLL0_CFG1 寄存器结构定义。地址偏移量:0x1EC，初值:0x0000005F，宽度:32
 寄存器说明: APLL0系统控制寄存器1寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctinue_lock_num : 6;  /* bit[0-5]  : fnpll锁定判断条件，连续多少个8倍 pfd clk周期计数值在锁定阈值内，默认为31，input_cfg_en=1时有效  */
        unsigned int  lock_thr        : 2;  /* bit[6-7]  : fnpll锁定阈值配置，input_cfg_en_fnpll=1时有效 2'b00:锁定阈值为0
                                                           2'b01:锁定阈值为1【默认】
                                                           2'b10:锁定阈值为2 
                                                           2'b11:锁定阈值为3
                                                           （1）整数模式，fbdiv<64，vco锁定精度为(lock_thr+1)/2个pfd频率，vco失锁精度为(lock_thr+2)/2个pfd频率。
                                                           （2）整数模式，fbdiv>=64，vco锁定精度为(lock_thr+2)/2个pfd频率，vco失锁精度为(lock_thr+3)/2个pfd频率。
                                                           （3）小数模式，fbdiv<64，vco上频偏锁定精度为(lock_thr+2)/2个pfd频率，下频偏锁定精度为(lock_thr+1)/2个pfd；vco上频偏失锁精度为(lock_thr+3)/2个pfd频率, 下频偏失锁精度为(lock_thr+2)/2个pfd频率。
                                                           （4）小数模式，fbdiv>=64，vco上频偏锁定精度为(lock_thr+3)/2个pfd频率，下频偏锁定精度为(lock_thr+2)/2个pfd；vco上频偏失锁精度为(lock_thr+4)/2个pfd频率, 下频偏失锁精度为(lock_thr+3)/2个pfd频率 */
        unsigned int  dc_ac_clk_en    : 1;  /* bit[8]    : 1：不使能dc_ac模块的时钟自动门控功能，时钟常开；
                                                           0：使能dc_ac模块的时钟自动门控功能（默认）
                                                           切换到ac 通路后，硬件自动关闭数字内部的dc时钟。 */
        unsigned int  dtc_ctrl_inv    : 1;  /* bit[9]    : fnpll中dtc控制字取反控制信号，input_cfg_en_fnpll=1时有效：
                                                           0：控制字正常输出【默认】
                                                           1：控制字取反 */
        unsigned int  reserved        : 6;  /* bit[10-15]: 保留 */
        unsigned int  ana_cfg_fnpll   : 16; /* bit[16-31]: <15:13>：浮空；
                                                           <12> ：IBIAS_TEST_EN: EN=1时opa输出端口电流可测
                                                           <11>：IDAC_TEST_EN:EN=1时dac输出端口电流可测；
                                                           <10:4>：
                                                           10xxxx1：FL_VIN1_TEST端口输出AC_BUF_PD; <10>和<9>不能同时设置为1
                                                           01xxxx1：FL_VIN1_TEST端口输出FL_VIN1;<10>和<9>不能同时设置为1
                                                           xx10xx1：DC_CLK_TEST 端口输出VCO_DC_CLK;<8>和<7>不能同时设置为1
                                                           xx01xx1：DC_CLK_TEST 端口输出PFD_CLK;<8>和<7>不能同时设置为1
                                                           xxxx101: VX_TEST端口输出DC_BUF_PD信号;<6>和<5>不能同时设置为1
                                                           xxxx011: VX_TEST端口输出FB_OUT_CLK信号;<6>和<5>不能同时设置为1
                                                           <3>:浮空
                                                           <2>:INIT_DIV2_EN:vbp_initial输入是否经过二分频，默认配置为0
                                                           <1:0>:PFD_DELAY_CTRL: 00为默认档位
                                                           01-00-10-11 pfd输入零相差下的输出脉宽逐级增大 */
    } reg;
} SOC_CORECRG_L_L3_APLL0_CFG1_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0_CFG1_ctinue_lock_num_START  (0)
#define SOC_CORECRG_L_L3_APLL0_CFG1_ctinue_lock_num_END    (5)
#define SOC_CORECRG_L_L3_APLL0_CFG1_lock_thr_START         (6)
#define SOC_CORECRG_L_L3_APLL0_CFG1_lock_thr_END           (7)
#define SOC_CORECRG_L_L3_APLL0_CFG1_dc_ac_clk_en_START     (8)
#define SOC_CORECRG_L_L3_APLL0_CFG1_dc_ac_clk_en_END       (8)
#define SOC_CORECRG_L_L3_APLL0_CFG1_dtc_ctrl_inv_START     (9)
#define SOC_CORECRG_L_L3_APLL0_CFG1_dtc_ctrl_inv_END       (9)
#define SOC_CORECRG_L_L3_APLL0_CFG1_ana_cfg_fnpll_START    (16)
#define SOC_CORECRG_L_L3_APLL0_CFG1_ana_cfg_fnpll_END      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL0_CFG2_UNION
 结构说明  : APLL0_CFG2 寄存器结构定义。地址偏移量:0x1F0，初值:0x800180A6，宽度:32
 寄存器说明: APLL0系统控制寄存器2寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  div2_pd        : 1;  /* bit[0]    : fnpll测试模式下VCO二分频器powerdown信号，test_mode_fnpll=1且input_cfg_en_fnpll=1时有效，否则正常工作模式使用数字内部时序产生值：
                                                          0：正常分频【默认】
                                                          1：Power down */
        unsigned int  fbdiv_rst_n    : 1;  /* bit[1]    : fnpll测试模式下反馈分频器复位输入，test_mode=1且input_cfg_en=1时有效，否则正常工作模式使用数字内部时序产生值：
                                                          0：反馈分频器复位
                                                          1：反馈分频器正常工作【默认】 */
        unsigned int  refdiv_rst_n   : 1;  /* bit[2]    : fnpll测试模式下参考分频器复位输入，test_mode_fnpll=1且input_cfg_en_fnpll=1时有效，否则正常工作模式使用数字内部时序产生值：
                                                          0：复位
                                                          1：正常工作【默认】 */
        unsigned int  dll_force_en   : 1;  /* bit[3]    : fnpll中dll状态强制使能，input_cfg_en_fnpll=1时有效：（DLL进入到DTC_CALIB状态需要pll_lock=1，在强制使能状态，不需要pll_lock=1这个条件。）
                                                          0：不强制进入DTC校正状态【默认】
                                                          1：强制进入DTC校正状态 */
        unsigned int  phe_code_a     : 2;  /* bit[4-5]  : fnpll中phe的coder调节系数a，input_cfg_en_fnpll=1时有效 ： 
                                                          2'b00: 0
                                                          2'b01: 1
                                                          2'b10: 2(默认)
                                                          2'b11: 3
                                                          要求小数模式时phe_code_a和phe_code_b不能同时配0，同时配0影响DTC_O校正，RMS jitter不稳 */
        unsigned int  phe_code_b     : 2;  /* bit[6-7]  : fnpll中phe的coder调节系数b，input_cfg_en_fnpll=1时有效： 2'b00: 0
                                                          2'b01: 1
                                                          2'b10: 2(默认)
                                                          2'b11: 3
                                                          要求小数模式时phe_code_a和phe_code_b不能同时配0，同时配0影响DTC_O校正，RMS jitter不稳 */
        unsigned int  bbpd_calib_byp : 1;  /* bit[8]    : fnpll的bbpd校正旁路信号，input_cfg_en_fnpll=1时有效，使能时直接跳过bbpd校正功能：
                                                          0：进行正常校正【默认】
                                                          1：bbpd校正bypass */
        unsigned int  k_gain_cfg_en  : 1;  /* bit[9]    : fnpll中dtc校正时k_gain外部配置使能，input_cfg_en_fnpll=1时有效：
                                                          0：k_gain内部产生【默认】
                                                          1：k_gain外部配置 */
        unsigned int  k_gain_cfg     : 6;  /* bit[10-15]: fnpll中dtc校正时k_gain的外部配置值，为1/2 FOUT2X 时钟周期对应dtc延时step的个数，input_cfg_en=1时有效，默认为32 */
        unsigned int  k_gain_av_thr  : 3;  /* bit[16-18]: fnpll中bbpd校正时k_gain瞬时值与平均值允许的差值，2bit整数1bit小数，input_cfg_en_fnpll=1时有效：
                                                          000:k_gain允许差值为0（小数模式时禁配，会影响DTC_O校正，RMSjitter不稳）
                                                          001:k_gain允许差值为0.5【默认】
                                                          010:k_gain允许差值为1.0
                                                          011:k_gain允许差值为1.5
                                                          100:k_gain允许差值为2.0
                                                          …
                                                          …
                                                          111:k_gain允许差值为3.5  */
        unsigned int  dtc_test       : 1;  /* bit[19]   : fnpll的dtc_m测试使能：
                                                          0：关闭dtc_m链测试模式【默认】
                                                          1：进入dtc_m链测试模式，调相功能时必须为1，要求同时满足input_cfg_en_fnpll=1 */
        unsigned int  dtc_m_cfg      : 6;  /* bit[20-25]: fnpll中dtc_m测试模式下dtc_m控制字的配置值，默认6'd0,调相功能时作为细链DTC控制字，要求同时满足input_cfg_en_fnpll=1 */
        unsigned int  dtc_o_cfg      : 6;  /* bit[26-31]: fnpll中bbpd校正bypass或者bbpd校正失败时dtc_o控制字的配置值，input_cfg_en_fnpll=1时有效，默认32 */
    } reg;
} SOC_CORECRG_L_L3_APLL0_CFG2_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0_CFG2_div2_pd_START         (0)
#define SOC_CORECRG_L_L3_APLL0_CFG2_div2_pd_END           (0)
#define SOC_CORECRG_L_L3_APLL0_CFG2_fbdiv_rst_n_START     (1)
#define SOC_CORECRG_L_L3_APLL0_CFG2_fbdiv_rst_n_END       (1)
#define SOC_CORECRG_L_L3_APLL0_CFG2_refdiv_rst_n_START    (2)
#define SOC_CORECRG_L_L3_APLL0_CFG2_refdiv_rst_n_END      (2)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dll_force_en_START    (3)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dll_force_en_END      (3)
#define SOC_CORECRG_L_L3_APLL0_CFG2_phe_code_a_START      (4)
#define SOC_CORECRG_L_L3_APLL0_CFG2_phe_code_a_END        (5)
#define SOC_CORECRG_L_L3_APLL0_CFG2_phe_code_b_START      (6)
#define SOC_CORECRG_L_L3_APLL0_CFG2_phe_code_b_END        (7)
#define SOC_CORECRG_L_L3_APLL0_CFG2_bbpd_calib_byp_START  (8)
#define SOC_CORECRG_L_L3_APLL0_CFG2_bbpd_calib_byp_END    (8)
#define SOC_CORECRG_L_L3_APLL0_CFG2_k_gain_cfg_en_START   (9)
#define SOC_CORECRG_L_L3_APLL0_CFG2_k_gain_cfg_en_END     (9)
#define SOC_CORECRG_L_L3_APLL0_CFG2_k_gain_cfg_START      (10)
#define SOC_CORECRG_L_L3_APLL0_CFG2_k_gain_cfg_END        (15)
#define SOC_CORECRG_L_L3_APLL0_CFG2_k_gain_av_thr_START   (16)
#define SOC_CORECRG_L_L3_APLL0_CFG2_k_gain_av_thr_END     (18)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dtc_test_START        (19)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dtc_test_END          (19)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dtc_m_cfg_START       (20)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dtc_m_cfg_END         (25)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dtc_o_cfg_START       (26)
#define SOC_CORECRG_L_L3_APLL0_CFG2_dtc_o_cfg_END         (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL1_CFG0_UNION
 结构说明  : APLL1_CFG0 寄存器结构定义。地址偏移量:0x1F4，初值:0x8C011022，宽度:32
 寄存器说明: APLL1系统控制寄存器0寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sta_reg_sel      : 1;  /* bit[0]    : 寄存器回读选择信号，选择当前读的是哪组状态寄存器：
                                                            0：选择读寄存器STATE0【默认】
                                                            1：选择读寄存器STATE1 */
        unsigned int  read_en          : 1;  /* bit[1]    : fnpll测试输出寄存器读取使能，input_cfg_en_fnpll=1时有效，read_en上升沿更新测试数据，所以更新数据时需要先低后高跳变：
                                                            0：保持测试输出寄存器数据
                                                            1：更新测试输出寄存器数据【默认】 */
        unsigned int  input_cfg_en     : 1;  /* bit[2]    : fnpll小数分频器参数配置使能：
                                                            0：使用内部默认配置【默认】
                                                            1：使用外部配置 */
        unsigned int  test_mode        : 1;  /* bit[3]    : fnpll测试模式使能，当input_cfg_en_fnpll=1时test_mode_fnpll才有效，
                                                            0：关闭测试使能【默认】
                                                            1：开启测试模式 */
        unsigned int  unlock_clear     : 1;  /* bit[4]    : 清零历史失锁指示信号unlock_flag以及历史失锁统计次数，只作为testchip测试使用：
                                                            0：不清零unlock_flag以及历史失锁统计次数；【默认】
                                                            1：清零unlock_flag以及历史失锁统计次数（上升沿触发） */
        unsigned int  dll_en           : 1;  /* bit[5]    : FNPLL dll控制使能：
                                                            0：关闭dll
                                                            1：dll正常工作【默认】 */
        unsigned int  pll_lock_ate_sel : 2;  /* bit[6-7]  : fnpll ATE测试输出信号复用的选择信号，选择ATE测试输出哪组信号，默认为0：
                                                            00：输出lock；【默认】
                                                            01：输出bbpd_calib_success；
                                                            10：输出dtc_calib_satu；
                                                            11：输出dtc_calib_state； */
        unsigned int  test_data_sel    : 4;  /* bit[8-11] : fnpll输出测试数据复用的选择信号，选择测试输出哪组数据，默认为0
                                                            0001：test_data={1'b0,c_out,mux_sel1_test[2:0],mux_sel0_test[2:0]},需要read_en上升沿更新数据;
                                                            0010：test_data={2'b0,k_gain},需要read_en上升沿更新数据;
                                                            0011：test_data={2'b0,dtc_m_bin},需要read_en上升沿更新数据;
                                                            0100：test_data={2'b0,dtc_c_bin},需要read_en上升沿更新数据;
                                                            0101：test_data={2'b0,dtc_o_bin},需要read_en上升沿更新数据;
                                                            0110：test_data={4'b0,coder_out},需要read_en上升沿更新数据;
                                                            0111：test_data={3'b0,phe_out},需要read_en上升沿更新数据;
                                                            其他：test_data=0 */
        unsigned int  bias_ctrl        : 2;  /* bit[12-13]: FNPLL不同电源电压下bias电压档位选择信号 
                                                            00-reserved；【禁配】
                                                            01-电源电压1.2V；
                                                            10-reserved；【禁配】
                                                            11-reserved；【禁配】 */
        unsigned int  icp_ctrl         : 2;  /* bit[14-15]: fnpll的ICP偏置电流选择信号
                                                            3：reserved【禁配】
                                                            2：reserved【禁配】
                                                            1：选择1/4 CP偏置电流
                                                            0：选择1/2 CP偏置电流【默认】 */
        unsigned int  updn_sel         : 1;  /* bit[16]   : fnpll的CP路数选择信号
                                                            1：打开1路CP【默认】
                                                            0：打开2路CP */
        unsigned int  reserved         : 2;  /* bit[17-18]: 保留 */
        unsigned int  int_mask         : 3;  /* bit[19-21]: 中断屏蔽位，为1时中断被屏蔽。中断通过或逻辑连到pll_int。
                                                            xx1：屏蔽unlock_flag；
                                                            x1x：屏蔽bbpd_calib_fail；
                                                            1xx：屏蔽dtc_calib_satu。
                                                            000：全部不屏蔽。【默认】 */
        unsigned int  pfd_div_ratio    : 4;  /* bit[22-25]: dc_ac模块内pfd分频比设置,默认不分频。建议不要配置。
                                                            4'b0000: 1分频（固定默认值）
                                                            4'b0001: 1分频
                                                            4'b0010: 2分频
                                                            ….
                                                            4'b1111: 15分频 */
        unsigned int  freq_threshold   : 6;  /* bit[26-31]: 判断振荡器输出时钟频率的阈值，（0/1禁配）,freq_threshold=pfd_div_ratio*5.5GHz/[4*freq(pfd_clk)]-1；
                                                            (当计算值大于63时取63即可，向上取整）。
                                                            0: （禁配）
                                                            1: （禁配）
                                                            2：振荡频率超过4*（2+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            3：振荡频率超过4*（3+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            4：振荡频率超过4*（4+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            ……
                                                            31:振荡频率超过4*（31+1）*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            ...
                                                            63:振荡频率超过4*(63+1)*freq(pfd_clk)/pfd_div_ratio后开启AC环路
                                                            备注：pfd_div_ratio固定为1分频 */
    } reg;
} SOC_CORECRG_L_L3_APLL1_CFG0_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1_CFG0_sta_reg_sel_START       (0)
#define SOC_CORECRG_L_L3_APLL1_CFG0_sta_reg_sel_END         (0)
#define SOC_CORECRG_L_L3_APLL1_CFG0_read_en_START           (1)
#define SOC_CORECRG_L_L3_APLL1_CFG0_read_en_END             (1)
#define SOC_CORECRG_L_L3_APLL1_CFG0_input_cfg_en_START      (2)
#define SOC_CORECRG_L_L3_APLL1_CFG0_input_cfg_en_END        (2)
#define SOC_CORECRG_L_L3_APLL1_CFG0_test_mode_START         (3)
#define SOC_CORECRG_L_L3_APLL1_CFG0_test_mode_END           (3)
#define SOC_CORECRG_L_L3_APLL1_CFG0_unlock_clear_START      (4)
#define SOC_CORECRG_L_L3_APLL1_CFG0_unlock_clear_END        (4)
#define SOC_CORECRG_L_L3_APLL1_CFG0_dll_en_START            (5)
#define SOC_CORECRG_L_L3_APLL1_CFG0_dll_en_END              (5)
#define SOC_CORECRG_L_L3_APLL1_CFG0_pll_lock_ate_sel_START  (6)
#define SOC_CORECRG_L_L3_APLL1_CFG0_pll_lock_ate_sel_END    (7)
#define SOC_CORECRG_L_L3_APLL1_CFG0_test_data_sel_START     (8)
#define SOC_CORECRG_L_L3_APLL1_CFG0_test_data_sel_END       (11)
#define SOC_CORECRG_L_L3_APLL1_CFG0_bias_ctrl_START         (12)
#define SOC_CORECRG_L_L3_APLL1_CFG0_bias_ctrl_END           (13)
#define SOC_CORECRG_L_L3_APLL1_CFG0_icp_ctrl_START          (14)
#define SOC_CORECRG_L_L3_APLL1_CFG0_icp_ctrl_END            (15)
#define SOC_CORECRG_L_L3_APLL1_CFG0_updn_sel_START          (16)
#define SOC_CORECRG_L_L3_APLL1_CFG0_updn_sel_END            (16)
#define SOC_CORECRG_L_L3_APLL1_CFG0_int_mask_START          (19)
#define SOC_CORECRG_L_L3_APLL1_CFG0_int_mask_END            (21)
#define SOC_CORECRG_L_L3_APLL1_CFG0_pfd_div_ratio_START     (22)
#define SOC_CORECRG_L_L3_APLL1_CFG0_pfd_div_ratio_END       (25)
#define SOC_CORECRG_L_L3_APLL1_CFG0_freq_threshold_START    (26)
#define SOC_CORECRG_L_L3_APLL1_CFG0_freq_threshold_END      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL1_CFG1_UNION
 结构说明  : APLL1_CFG1 寄存器结构定义。地址偏移量:0x1F8，初值:0x0000005F，宽度:32
 寄存器说明: APLL1系统控制寄存器1寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctinue_lock_num : 6;  /* bit[0-5]  : fnpll锁定判断条件，连续多少个8倍 pfd clk周期计数值在锁定阈值内，默认为31，input_cfg_en=1时有效  */
        unsigned int  lock_thr        : 2;  /* bit[6-7]  : fnpll锁定阈值配置，input_cfg_en_fnpll=1时有效 2'b00:锁定阈值为0
                                                           2'b01:锁定阈值为1【默认】
                                                           2'b10:锁定阈值为2 
                                                           2'b11:锁定阈值为3
                                                           （1）整数模式，fbdiv<64，vco锁定精度为(lock_thr+1)/2个pfd频率，vco失锁精度为(lock_thr+2)/2个pfd频率。
                                                           （2）整数模式，fbdiv>=64，vco锁定精度为(lock_thr+2)/2个pfd频率，vco失锁精度为(lock_thr+3)/2个pfd频率。
                                                           （3）小数模式，fbdiv<64，vco上频偏锁定精度为(lock_thr+2)/2个pfd频率，下频偏锁定精度为(lock_thr+1)/2个pfd；vco上频偏失锁精度为(lock_thr+3)/2个pfd频率, 下频偏失锁精度为(lock_thr+2)/2个pfd频率。
                                                           （4）小数模式，fbdiv>=64，vco上频偏锁定精度为(lock_thr+3)/2个pfd频率，下频偏锁定精度为(lock_thr+2)/2个pfd；vco上频偏失锁精度为(lock_thr+4)/2个pfd频率, 下频偏失锁精度为(lock_thr+3)/2个pfd频率 */
        unsigned int  dc_ac_clk_en    : 1;  /* bit[8]    : 1：不使能dc_ac模块的时钟自动门控功能，时钟常开；
                                                           0：使能dc_ac模块的时钟自动门控功能（默认）
                                                           切换到ac 通路后，硬件自动关闭数字内部的dc时钟。 */
        unsigned int  dtc_ctrl_inv    : 1;  /* bit[9]    : fnpll中dtc控制字取反控制信号，input_cfg_en_fnpll=1时有效：
                                                           0：控制字正常输出【默认】
                                                           1：控制字取反 */
        unsigned int  reserved        : 6;  /* bit[10-15]: 保留 */
        unsigned int  ana_cfg_fnpll   : 16; /* bit[16-31]: <15:13>：浮空；
                                                           <12> ：IBIAS_TEST_EN: EN=1时opa输出端口电流可测
                                                           <11>：IDAC_TEST_EN:EN=1时dac输出端口电流可测；
                                                           <10:4>：
                                                           10xxxx1：FL_VIN1_TEST端口输出AC_BUF_PD; <10>和<9>不能同时设置为1
                                                           01xxxx1：FL_VIN1_TEST端口输出FL_VIN1;<10>和<9>不能同时设置为1
                                                           xx10xx1：DC_CLK_TEST 端口输出VCO_DC_CLK;<8>和<7>不能同时设置为1
                                                           xx01xx1：DC_CLK_TEST 端口输出PFD_CLK;<8>和<7>不能同时设置为1
                                                           xxxx101: VX_TEST端口输出DC_BUF_PD信号;<6>和<5>不能同时设置为1
                                                           xxxx011: VX_TEST端口输出FB_OUT_CLK信号;<6>和<5>不能同时设置为1
                                                           <3>:浮空
                                                           <2>:INIT_DIV2_EN:vbp_initial输入是否经过二分频，默认配置为0
                                                           <1:0>:PFD_DELAY_CTRL: 00为默认档位
                                                           01-00-10-11 pfd输入零相差下的输出脉宽逐级增大 */
    } reg;
} SOC_CORECRG_L_L3_APLL1_CFG1_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1_CFG1_ctinue_lock_num_START  (0)
#define SOC_CORECRG_L_L3_APLL1_CFG1_ctinue_lock_num_END    (5)
#define SOC_CORECRG_L_L3_APLL1_CFG1_lock_thr_START         (6)
#define SOC_CORECRG_L_L3_APLL1_CFG1_lock_thr_END           (7)
#define SOC_CORECRG_L_L3_APLL1_CFG1_dc_ac_clk_en_START     (8)
#define SOC_CORECRG_L_L3_APLL1_CFG1_dc_ac_clk_en_END       (8)
#define SOC_CORECRG_L_L3_APLL1_CFG1_dtc_ctrl_inv_START     (9)
#define SOC_CORECRG_L_L3_APLL1_CFG1_dtc_ctrl_inv_END       (9)
#define SOC_CORECRG_L_L3_APLL1_CFG1_ana_cfg_fnpll_START    (16)
#define SOC_CORECRG_L_L3_APLL1_CFG1_ana_cfg_fnpll_END      (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL1_CFG2_UNION
 结构说明  : APLL1_CFG2 寄存器结构定义。地址偏移量:0x1FC，初值:0x800180A6，宽度:32
 寄存器说明: APLL1系统控制寄存器2寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  div2_pd        : 1;  /* bit[0]    : fnpll测试模式下VCO二分频器powerdown信号，test_mode_fnpll=1且input_cfg_en_fnpll=1时有效，否则正常工作模式使用数字内部时序产生值：
                                                          0：正常分频【默认】
                                                          1：Power down */
        unsigned int  fbdiv_rst_n    : 1;  /* bit[1]    : fnpll测试模式下反馈分频器复位输入，test_mode=1且input_cfg_en=1时有效，否则正常工作模式使用数字内部时序产生值：
                                                          0：反馈分频器复位
                                                          1：反馈分频器正常工作【默认】 */
        unsigned int  refdiv_rst_n   : 1;  /* bit[2]    : fnpll测试模式下参考分频器复位输入，test_mode_fnpll=1且input_cfg_en_fnpll=1时有效，否则正常工作模式使用数字内部时序产生值：
                                                          0：复位
                                                          1：正常工作【默认】 */
        unsigned int  dll_force_en   : 1;  /* bit[3]    : fnpll中dll状态强制使能，input_cfg_en_fnpll=1时有效：（DLL进入到DTC_CALIB状态需要pll_lock=1，在强制使能状态，不需要pll_lock=1这个条件。）
                                                          0：不强制进入DTC校正状态【默认】
                                                          1：强制进入DTC校正状态 */
        unsigned int  phe_code_a     : 2;  /* bit[4-5]  : fnpll中phe的coder调节系数a，input_cfg_en_fnpll=1时有效 ： 
                                                          2'b00: 0
                                                          2'b01: 1
                                                          2'b10: 2(默认)
                                                          2'b11: 3
                                                          要求小数模式时phe_code_a和phe_code_b不能同时配0，同时配0影响DTC_O校正，RMS jitter不稳 */
        unsigned int  phe_code_b     : 2;  /* bit[6-7]  : fnpll中phe的coder调节系数b，input_cfg_en_fnpll=1时有效： 2'b00: 0
                                                          2'b01: 1
                                                          2'b10: 2(默认)
                                                          2'b11: 3
                                                          要求小数模式时phe_code_a和phe_code_b不能同时配0，同时配0影响DTC_O校正，RMS jitter不稳 */
        unsigned int  bbpd_calib_byp : 1;  /* bit[8]    : fnpll的bbpd校正旁路信号，input_cfg_en_fnpll=1时有效，使能时直接跳过bbpd校正功能：
                                                          0：进行正常校正【默认】
                                                          1：bbpd校正bypass */
        unsigned int  k_gain_cfg_en  : 1;  /* bit[9]    : fnpll中dtc校正时k_gain外部配置使能，input_cfg_en_fnpll=1时有效：
                                                          0：k_gain内部产生【默认】
                                                          1：k_gain外部配置 */
        unsigned int  k_gain_cfg     : 6;  /* bit[10-15]: fnpll中dtc校正时k_gain的外部配置值，为1/2 FOUT2X 时钟周期对应dtc延时step的个数，input_cfg_en=1时有效，默认为32 */
        unsigned int  k_gain_av_thr  : 3;  /* bit[16-18]: fnpll中bbpd校正时k_gain瞬时值与平均值允许的差值，2bit整数1bit小数，input_cfg_en_fnpll=1时有效：
                                                          000:k_gain允许差值为0（小数模式时禁配，会影响DTC_O校正，RMSjitter不稳）
                                                          001:k_gain允许差值为0.5【默认】
                                                          010:k_gain允许差值为1.0
                                                          011:k_gain允许差值为1.5
                                                          100:k_gain允许差值为2.0
                                                          …
                                                          …
                                                          111:k_gain允许差值为3.5  */
        unsigned int  dtc_test       : 1;  /* bit[19]   : fnpll的dtc_m测试使能：
                                                          0：关闭dtc_m链测试模式【默认】
                                                          1：进入dtc_m链测试模式，调相功能时必须为1，要求同时满足input_cfg_en_fnpll=1 */
        unsigned int  dtc_m_cfg      : 6;  /* bit[20-25]: fnpll中dtc_m测试模式下dtc_m控制字的配置值，默认6'd0,调相功能时作为细链DTC控制字，要求同时满足input_cfg_en_fnpll=1 */
        unsigned int  dtc_o_cfg      : 6;  /* bit[26-31]: fnpll中bbpd校正bypass或者bbpd校正失败时dtc_o控制字的配置值，input_cfg_en_fnpll=1时有效，默认32 */
    } reg;
} SOC_CORECRG_L_L3_APLL1_CFG2_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1_CFG2_div2_pd_START         (0)
#define SOC_CORECRG_L_L3_APLL1_CFG2_div2_pd_END           (0)
#define SOC_CORECRG_L_L3_APLL1_CFG2_fbdiv_rst_n_START     (1)
#define SOC_CORECRG_L_L3_APLL1_CFG2_fbdiv_rst_n_END       (1)
#define SOC_CORECRG_L_L3_APLL1_CFG2_refdiv_rst_n_START    (2)
#define SOC_CORECRG_L_L3_APLL1_CFG2_refdiv_rst_n_END      (2)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dll_force_en_START    (3)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dll_force_en_END      (3)
#define SOC_CORECRG_L_L3_APLL1_CFG2_phe_code_a_START      (4)
#define SOC_CORECRG_L_L3_APLL1_CFG2_phe_code_a_END        (5)
#define SOC_CORECRG_L_L3_APLL1_CFG2_phe_code_b_START      (6)
#define SOC_CORECRG_L_L3_APLL1_CFG2_phe_code_b_END        (7)
#define SOC_CORECRG_L_L3_APLL1_CFG2_bbpd_calib_byp_START  (8)
#define SOC_CORECRG_L_L3_APLL1_CFG2_bbpd_calib_byp_END    (8)
#define SOC_CORECRG_L_L3_APLL1_CFG2_k_gain_cfg_en_START   (9)
#define SOC_CORECRG_L_L3_APLL1_CFG2_k_gain_cfg_en_END     (9)
#define SOC_CORECRG_L_L3_APLL1_CFG2_k_gain_cfg_START      (10)
#define SOC_CORECRG_L_L3_APLL1_CFG2_k_gain_cfg_END        (15)
#define SOC_CORECRG_L_L3_APLL1_CFG2_k_gain_av_thr_START   (16)
#define SOC_CORECRG_L_L3_APLL1_CFG2_k_gain_av_thr_END     (18)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dtc_test_START        (19)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dtc_test_END          (19)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dtc_m_cfg_START       (20)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dtc_m_cfg_END         (25)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dtc_o_cfg_START       (26)
#define SOC_CORECRG_L_L3_APLL1_CFG2_dtc_o_cfg_END         (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_FCM_CLKEN_UNION
 结构说明  : FCM_CLKEN 寄存器结构定义。地址偏移量:0x200，初值:0x00000000，宽度:32
 寄存器说明: FCM子系统时钟使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gt_clk_cpu_l3     : 1;  /* bit[0]    : CPU L3子系统整体时钟关断使能：
                                                             0：时钟关断；
                                                             1：时钟打开。 */
        unsigned int  gt_clk_cpu_little : 1;  /* bit[1]    : CPU LITTLE子系统整体时钟关断使能：
                                                             0：时钟关断；
                                                             1：时钟打开。 */
        unsigned int  reserved_0        : 1;  /* bit[2]    :  */
        unsigned int  reserved_1        : 13; /* bit[3-15] :  */
        unsigned int  bitmasken         : 16; /* bit[16-31]: clken每个比特位的使能位：
                                                             只有当bitmasken对应的比特位为1'b1，clken相应的比特位才起作用。bitmasken[0]就是clken[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CORECRG_L_L3_FCM_CLKEN_UNION;
#endif
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_l3_START      (0)
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_l3_END        (0)
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_little_START  (1)
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_little_END    (1)
#define SOC_CORECRG_L_L3_FCM_CLKEN_bitmasken_START          (16)
#define SOC_CORECRG_L_L3_FCM_CLKEN_bitmasken_END            (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL0_STAT_UNION
 结构说明  : APLL0_STAT 寄存器结构定义。地址偏移量:0x250，初值:0x00000000，宽度:32
 寄存器说明: APLL0系统状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 根据pll对应的cfg0信号中sta_reg_sel的值，选择输出pll对应的stat信号；stat信号详细描述见“寄存器复用说明”页
                                                   0：选择读寄存器FNPLL_STATE0【默认】
                                                   1：选择读寄存器FNPLL_STATE1 */
    } reg;
} SOC_CORECRG_L_L3_APLL0_STAT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL1_STAT_UNION
 结构说明  : APLL1_STAT 寄存器结构定义。地址偏移量:0x254，初值:0x00000000，宽度:32
 寄存器说明: APLL1系统状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 根据pll对应的cfg0信号中sta_reg_sel的值，选择输出pll对应的stat信号；stat信号详细描述见“寄存器复用说明”页
                                                   0：选择读寄存器FNPLL_STATE0【默认】
                                                   1：选择读寄存器FNPLL_STATE1 */
    } reg;
} SOC_CORECRG_L_L3_APLL1_STAT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL0_CFG3_UNION
 结构说明  : APLL0_CFG3 寄存器结构定义。地址偏移量:0x258，初值:0x00206040，宽度:32
 寄存器说明: APLL0系统控制寄存器3寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_cfg3 : 32; /* bit[0-31]: 具体含义待后续刷新，当前配置为默认值即可正常工作 */
    } reg;
} SOC_CORECRG_L_L3_APLL0_CFG3_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0_CFG3_apll0_cfg3_START  (0)
#define SOC_CORECRG_L_L3_APLL0_CFG3_apll0_cfg3_END    (31)


/*****************************************************************************
 结构名    : SOC_CORECRG_L_L3_APLL1_CFG3_UNION
 结构说明  : APLL1_CFG3 寄存器结构定义。地址偏移量:0x25C，初值:0x00206040，宽度:32
 寄存器说明: APLL1系统控制寄存器3寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_cfg3 : 32; /* bit[0-31]: 具体含义待后续刷新，当前配置为默认值即可正常工作 */
    } reg;
} SOC_CORECRG_L_L3_APLL1_CFG3_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1_CFG3_apll1_cfg3_START  (0)
#define SOC_CORECRG_L_L3_APLL1_CFG3_apll1_cfg3_END    (31)






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

#endif /* end of soc_corecrg_l_l3_interface.h */
