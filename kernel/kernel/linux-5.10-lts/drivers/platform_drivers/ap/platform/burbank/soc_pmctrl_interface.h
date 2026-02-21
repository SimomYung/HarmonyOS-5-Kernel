/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_pmctrl_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_PMCTRL_INTERFACE_H__
#define __SOC_PMCTRL_INTERFACE_H__

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


/* 寄存器说明：APLL硬件控制来源配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_APLL_CTRL_SRC_UNION */
#define SOC_PMCTRL_APLL_CTRL_SRC_ADDR(base)                   ((base) + (0x100UL))

/* 寄存器说明：子系统V和F控制来源配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_UNION */
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_ADDR(base)        ((base) + (0x104UL))

/* 寄存器说明：调压通道配置
   位域定义UNION结构:  SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_UNION */
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_ADDR(base)         ((base) + (0x108UL))

/* 寄存器说明：通用时间配置
   位域定义UNION结构:  SOC_PMCTRL_TIME_PARAMETER_CFG_0_UNION */
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_ADDR(base)            ((base) + (0x10CUL))

/* 寄存器说明：通用时间配置
   位域定义UNION结构:  SOC_PMCTRL_TIME_PARAMETER_CFG_1_UNION */
#define SOC_PMCTRL_TIME_PARAMETER_CFG_1_ADDR(base)            ((base) + (0x110UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUL_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_ADDR(base)              ((base) + (0x150UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUM_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_ADDR(base)              ((base) + (0x154UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUB_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_ADDR(base)              ((base) + (0x158UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_FCM_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_ADDR(base)               ((base) + (0x15CUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_ADDR(base)               ((base) + (0x160UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_ADDR(base)           ((base) + (0x164UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUL_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_ADDR(base)              ((base) + (0x170UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUM_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_ADDR(base)              ((base) + (0x174UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUB_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_ADDR(base)              ((base) + (0x178UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_FCM_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_ADDR(base)               ((base) + (0x17CUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_ADDR(base)          ((base) + (0x180UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_ADDR(base)           ((base) + (0x184UL))

/* 寄存器说明：GPU HPM时钟分频寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMCLKDIV_UNION */
#define SOC_PMCTRL_GPU_HPMCLKDIV_ADDR(base)                   ((base) + (0x120UL))

/* 寄存器说明：GPU HPM选择寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMSEL_UNION */
#define SOC_PMCTRL_GPU_HPMSEL_ADDR(base)                      ((base) + (0x124UL))

/* 寄存器说明：GPU HPM使能寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMEN_UNION */
#define SOC_PMCTRL_GPU_HPMEN_ADDR(base)                       ((base) + (0x128UL))

/* 寄存器说明：GPU HPM OPC回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMOPC_UNION */
#define SOC_PMCTRL_GPU_HPMOPC_ADDR(base)                      ((base) + (0x12CUL))

/* 寄存器说明：软件备份寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA0_UNION */
#define SOC_PMCTRL_PERIBAKDATA0_ADDR(base)                    ((base) + (0x130UL))

/* 寄存器说明：软件备份寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA1_UNION */
#define SOC_PMCTRL_PERIBAKDATA1_ADDR(base)                    ((base) + (0x134UL))

/* 寄存器说明：软件备份寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA2_UNION */
#define SOC_PMCTRL_PERIBAKDATA2_ADDR(base)                    ((base) + (0x138UL))

/* 寄存器说明：软件备份寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA3_UNION */
#define SOC_PMCTRL_PERIBAKDATA3_ADDR(base)                    ((base) + (0x13CUL))

/* 寄存器说明：SMPI电压控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SPMIVOLCFG_UNION */
#define SOC_PMCTRL_SPMIVOLCFG_ADDR(base)                      ((base) + (0x114UL))

/* 寄存器说明：SMPI电压控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SPMIVOLCFG1_UNION */
#define SOC_PMCTRL_SPMIVOLCFG1_ADDR(base)                     ((base) + (0x118UL))

/* 寄存器说明：G3D自动分频bypass控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_UNION */
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_ADDR(base)             ((base) + (0x1D8UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_A0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_A0_ADDR(base)                    ((base) + (0x200UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_A1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_A1_ADDR(base)                    ((base) + (0x204UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_B0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_B0_ADDR(base)                    ((base) + (0x208UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_B1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_B1_ADDR(base)                    ((base) + (0x20CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_C0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_C0_ADDR(base)                    ((base) + (0x210UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_C1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_C1_ADDR(base)                    ((base) + (0x214UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_D0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_D0_ADDR(base)                    ((base) + (0x218UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_D1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_D1_ADDR(base)                    ((base) + (0x21CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E0_ADDR(base)                    ((base) + (0x220UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E1_ADDR(base)                    ((base) + (0x224UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E2_ADDR(base)                    ((base) + (0x228UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E3_ADDR(base)                    ((base) + (0x22CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F0_ADDR(base)                    ((base) + (0x230UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F1_ADDR(base)                    ((base) + (0x234UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F2_ADDR(base)                    ((base) + (0x238UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F3_ADDR(base)                    ((base) + (0x23CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G0_ADDR(base)                    ((base) + (0x240UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G1_ADDR(base)                    ((base) + (0x244UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G2_ADDR(base)                    ((base) + (0x248UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G3_ADDR(base)                    ((base) + (0x24CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H0_ADDR(base)                    ((base) + (0x250UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H1_ADDR(base)                    ((base) + (0x254UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H2_ADDR(base)                    ((base) + (0x258UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H3_ADDR(base)                    ((base) + (0x25CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H4_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H4_ADDR(base)                    ((base) + (0x260UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H5_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H5_ADDR(base)                    ((base) + (0x264UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H6_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H6_ADDR(base)                    ((base) + (0x268UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H7_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H7_ADDR(base)                    ((base) + (0x26CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I0_ADDR(base)                    ((base) + (0x270UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I1_ADDR(base)                    ((base) + (0x274UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I2_ADDR(base)                    ((base) + (0x278UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I3_ADDR(base)                    ((base) + (0x27CUL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I4_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I4_ADDR(base)                    ((base) + (0x280UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I5_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I5_ADDR(base)                    ((base) + (0x284UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I6_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I6_ADDR(base)                    ((base) + (0x288UL))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I7_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I7_ADDR(base)                    ((base) + (0x28CUL))

/* 寄存器说明：投票通道数配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_CHANNEL_CFG_UNION */
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_ADDR(base)                ((base) + (0x2B0UL))

/* 寄存器说明：投票类型配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_MODE_CFG_0_UNION */
#define SOC_PMCTRL_VOTE_MODE_CFG_0_ADDR(base)                 ((base) + (0x2BCUL))

/* 寄存器说明：投票类型配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_MODE_CFG_1_UNION */
#define SOC_PMCTRL_VOTE_MODE_CFG_1_ADDR(base)                 ((base) + (0x2C0UL))

/* 寄存器说明：投票使能配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_START_EN_UNION */
#define SOC_PMCTRL_VOTE_START_EN_ADDR(base)                   ((base) + (0x2C4UL))

/* 寄存器说明：投票中断使能配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_EN_UNION */
#define SOC_PMCTRL_INT_VOTE_EN_ADDR(base)                     ((base) + (0x2C8UL))

/* 寄存器说明：投票中断清除配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_CLR_UNION */
#define SOC_PMCTRL_INT_VOTE_CLR_ADDR(base)                    ((base) + (0x2CCUL))

/* 寄存器说明：投票中断原始状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_RAW_STAT_UNION */
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_ADDR(base)               ((base) + (0x2D0UL))

/* 寄存器说明：投票中断最终状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_STAT_UNION */
#define SOC_PMCTRL_INT_VOTE_STAT_ADDR(base)                   ((base) + (0x2D4UL))

/* 寄存器说明：投票中断寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_FREQUENCY_VOTE_UNION */
#define SOC_PMCTRL_INT_FREQUENCY_VOTE_ADDR(base)              ((base) + (0x2D8UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_A_UNION */
#define SOC_PMCTRL_VOTE_RESULT_A_ADDR(base)                   ((base) + (0x2DCUL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_B_UNION */
#define SOC_PMCTRL_VOTE_RESULT_B_ADDR(base)                   ((base) + (0x2E0UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_C_UNION */
#define SOC_PMCTRL_VOTE_RESULT_C_ADDR(base)                   ((base) + (0x2E4UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_D_UNION */
#define SOC_PMCTRL_VOTE_RESULT_D_ADDR(base)                   ((base) + (0x2E8UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_E_UNION */
#define SOC_PMCTRL_VOTE_RESULT_E_ADDR(base)                   ((base) + (0x2ECUL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_F_UNION */
#define SOC_PMCTRL_VOTE_RESULT_F_ADDR(base)                   ((base) + (0x2F0UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_G_UNION */
#define SOC_PMCTRL_VOTE_RESULT_G_ADDR(base)                   ((base) + (0x2F4UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_H_UNION */
#define SOC_PMCTRL_VOTE_RESULT_H_ADDR(base)                   ((base) + (0x2F8UL))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_I_UNION */
#define SOC_PMCTRL_VOTE_RESULT_I_ADDR(base)                   ((base) + (0x2FCUL))

/* 寄存器说明：DDRC LP控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_DDRLPCTRL_UNION */
#define SOC_PMCTRL_DDRLPCTRL_ADDR(base)                       ((base) + (0x30CUL))

/* 寄存器说明：外设通用控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL0_UNION */
#define SOC_PMCTRL_PERI_CTRL0_ADDR(base)                      ((base) + (0x340UL))

/* 寄存器说明：外设通用控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL1_UNION */
#define SOC_PMCTRL_PERI_CTRL1_ADDR(base)                      ((base) + (0x344UL))

/* 寄存器说明：外设通用控制寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL2_UNION */
#define SOC_PMCTRL_PERI_CTRL2_ADDR(base)                      ((base) + (0x348UL))

/* 寄存器说明：外设通用控制寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL3_UNION */
#define SOC_PMCTRL_PERI_CTRL3_ADDR(base)                      ((base) + (0x34CUL))

/* 寄存器说明：外设通用控制寄存器4
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL4_UNION */
#define SOC_PMCTRL_PERI_CTRL4_ADDR(base)                      ((base) + (0x350UL))

/* 寄存器说明：外设通用控制寄存器5
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL5_UNION */
#define SOC_PMCTRL_PERI_CTRL5_ADDR(base)                      ((base) + (0x354UL))

/* 寄存器说明：外设通用控制寄存器6
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL6_UNION */
#define SOC_PMCTRL_PERI_CTRL6_ADDR(base)                      ((base) + (0x358UL))

/* 寄存器说明：外设通用控制寄存器7
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL7_UNION */
#define SOC_PMCTRL_PERI_CTRL7_ADDR(base)                      ((base) + (0x35CUL))

/* 寄存器说明：外设通用状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT0_UNION */
#define SOC_PMCTRL_PERI_STAT0_ADDR(base)                      ((base) + (0x360UL))

/* 寄存器说明：外设通用状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT1_UNION */
#define SOC_PMCTRL_PERI_STAT1_ADDR(base)                      ((base) + (0x364UL))

/* 寄存器说明：外设通用状态寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT2_UNION */
#define SOC_PMCTRL_PERI_STAT2_ADDR(base)                      ((base) + (0x368UL))

/* 寄存器说明：外设通用状态寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT3_UNION */
#define SOC_PMCTRL_PERI_STAT3_ADDR(base)                      ((base) + (0x36CUL))

/* 寄存器说明：外设通用状态寄存器4
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT4_UNION */
#define SOC_PMCTRL_PERI_STAT4_ADDR(base)                      ((base) + (0x370UL))

/* 寄存器说明：NOC低功耗请求信号寄存器0
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEREQ_0_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_ADDR(base)             ((base) + (0x380UL))

/* 寄存器说明：NOC低功耗响应寄存器0
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEACK_0_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_ADDR(base)             ((base) + (0x384UL))

/* 寄存器说明：NOC低功耗状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLE_0_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLE_0_ADDR(base)                ((base) + (0x388UL))

/* 寄存器说明：NOC低功耗请求信号寄存器1
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEREQ_1_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_ADDR(base)             ((base) + (0x38CUL))

/* 寄存器说明：NOC低功耗响应寄存器1
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEACK_1_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_ADDR(base)             ((base) + (0x390UL))

/* 寄存器说明：NOC低功耗状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLE_1_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLE_1_ADDR(base)                ((base) + (0x394UL))

/* 寄存器说明：NOC低功耗请求信号寄存器
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEREQ_2_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_2_ADDR(base)             ((base) + (0x398UL))

/* 寄存器说明：NOC低功耗响应寄存器
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEACK_2_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEACK_2_ADDR(base)             ((base) + (0x39CUL))

/* 寄存器说明：NOC低功耗状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLE_2_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLE_2_ADDR(base)                ((base) + (0x374UL))

/* 寄存器说明：外设中断屏蔽寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT0_MASK_UNION */
#define SOC_PMCTRL_PERI_INT0_MASK_ADDR(base)                  ((base) + (0x3A0UL))

/* 寄存器说明：外设中断状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT0_STAT_UNION */
#define SOC_PMCTRL_PERI_INT0_STAT_ADDR(base)                  ((base) + (0x3A4UL))

/* 寄存器说明：外设中断屏蔽寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT1_MASK_UNION */
#define SOC_PMCTRL_PERI_INT1_MASK_ADDR(base)                  ((base) + (0x3A8UL))

/* 寄存器说明：外设中断状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT1_STAT_UNION */
#define SOC_PMCTRL_PERI_INT1_STAT_ADDR(base)                  ((base) + (0x3ACUL))

/* 寄存器说明：外设中断屏蔽寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT2_MASK_UNION */
#define SOC_PMCTRL_PERI_INT2_MASK_ADDR(base)                  ((base) + (0x3B0UL))

/* 寄存器说明：外设中断状态寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT2_STAT_UNION */
#define SOC_PMCTRL_PERI_INT2_STAT_ADDR(base)                  ((base) + (0x3B4UL))

/* 寄存器说明：外设中断屏蔽寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT3_MASK_UNION */
#define SOC_PMCTRL_PERI_INT3_MASK_ADDR(base)                  ((base) + (0x3B8UL))

/* 寄存器说明：外设中断状态寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT3_STAT_UNION */
#define SOC_PMCTRL_PERI_INT3_STAT_ADDR(base)                  ((base) + (0x3BCUL))

/* 寄存器说明：Vsensor控制使能寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CTRL_EN_GPU_UNION */
#define SOC_PMCTRL_VS_CTRL_EN_GPU_ADDR(base)                  ((base) + (0x440UL))

/* 寄存器说明：Vsensor bypass寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CTRL_BYPASS_GPU_UNION */
#define SOC_PMCTRL_VS_CTRL_BYPASS_GPU_ADDR(base)              ((base) + (0x444UL))

/* 寄存器说明：Vsensor控制配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CTRL_GPU_UNION */
#define SOC_PMCTRL_VS_CTRL_GPU_ADDR(base)                     ((base) + (0x448UL))

/* 寄存器说明：SVFD配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_SVFD_CTRL0_GPU_UNION */
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_ADDR(base)               ((base) + (0x44CUL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_SVFD_CTRL1_GPU_UNION */
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_ADDR(base)               ((base) + (0x450UL))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_SVFD_CTRL2_GPU_UNION */
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_ADDR(base)               ((base) + (0x454UL))

/* 寄存器说明：SVFD状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_UNION */
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_ADDR(base)            ((base) + (0x458UL))

/* 寄存器说明：SVFD测试状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_TEST_STAT_GPU_UNION */
#define SOC_PMCTRL_VS_TEST_STAT_GPU_ADDR(base)                ((base) + (0x45CUL))

/* 寄存器说明：BOOTROM FLAG寄存器
   位域定义UNION结构:  SOC_PMCTRL_BOOTROMFLAG_UNION */
#define SOC_PMCTRL_BOOTROMFLAG_ADDR(base)                     ((base) + (0x460UL))

/* 寄存器说明：PERI HPM使能寄存器
   位域定义UNION结构:  SOC_PMCTRL_PERIHPMEN_UNION */
#define SOC_PMCTRL_PERIHPMEN_ADDR(base)                       ((base) + (0x464UL))

/* 寄存器说明：PERI HPM时钟分频寄存器
   位域定义UNION结构:  SOC_PMCTRL_PERIHPMCLKDIV_UNION */
#define SOC_PMCTRL_PERIHPMCLKDIV_ADDR(base)                   ((base) + (0x468UL))

/* 寄存器说明：PERI HPM OPC寄存器
   位域定义UNION结构:  SOC_PMCTRL_PERIHPMOPC_UNION */
#define SOC_PMCTRL_PERIHPMOPC_ADDR(base)                      ((base) + (0x46CUL))

/* 寄存器说明：外设通用状态寄存器8
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT8_UNION */
#define SOC_PMCTRL_PERI_STAT8_ADDR(base)                      ((base) + (0x4D0UL))

/* 寄存器说明：外设通用状态寄存器9
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT9_UNION */
#define SOC_PMCTRL_PERI_STAT9_ADDR(base)                      ((base) + (0x4D4UL))

/* 寄存器说明：外设通用状态寄存器10
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT10_UNION */
#define SOC_PMCTRL_PERI_STAT10_ADDR(base)                     ((base) + (0x4D8UL))

/* 寄存器说明：外设通用状态寄存器11
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT11_UNION */
#define SOC_PMCTRL_PERI_STAT11_ADDR(base)                     ((base) + (0x4DCUL))

/* 寄存器说明：外设通用状态寄存器12
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT12_UNION */
#define SOC_PMCTRL_PERI_STAT12_ADDR(base)                     ((base) + (0x4E0UL))

/* 寄存器说明：外设通用状态寄存器13
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT13_UNION */
#define SOC_PMCTRL_PERI_STAT13_ADDR(base)                     ((base) + (0x4E4UL))

/* 寄存器说明：外设通用状态寄存器14
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT14_UNION */
#define SOC_PMCTRL_PERI_STAT14_ADDR(base)                     ((base) + (0x4E8UL))

/* 寄存器说明：外设通用状态寄存器15
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT15_UNION */
#define SOC_PMCTRL_PERI_STAT15_ADDR(base)                     ((base) + (0x4ECUL))

/* 寄存器说明：APLL0控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL0_UNION */
#define SOC_PMCTRL_APLL0CTRL0_ADDR(base)                      ((base) + (0x500UL))

/* 寄存器说明：APLL0控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL1_UNION */
#define SOC_PMCTRL_APLL0CTRL1_ADDR(base)                      ((base) + (0x504UL))

/* 寄存器说明：APLL0控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL0CTRL0_STAT_ADDR(base)                 ((base) + (0x508UL))

/* 寄存器说明：APLL0控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL0CTRL1_STAT_ADDR(base)                 ((base) + (0x50CUL))

/* 寄存器说明：APLL1控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL0_UNION */
#define SOC_PMCTRL_APLL1CTRL0_ADDR(base)                      ((base) + (0x510UL))

/* 寄存器说明：APLL1控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL1_UNION */
#define SOC_PMCTRL_APLL1CTRL1_ADDR(base)                      ((base) + (0x514UL))

/* 寄存器说明：APLL1控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL1CTRL0_STAT_ADDR(base)                 ((base) + (0x518UL))

/* 寄存器说明：APLL1控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL1CTRL1_STAT_ADDR(base)                 ((base) + (0x51CUL))

/* 寄存器说明：APLL2控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL0_UNION */
#define SOC_PMCTRL_APLL2CTRL0_ADDR(base)                      ((base) + (0x520UL))

/* 寄存器说明：APLL2控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL1_UNION */
#define SOC_PMCTRL_APLL2CTRL1_ADDR(base)                      ((base) + (0x524UL))

/* 寄存器说明：APLL2控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL2CTRL0_STAT_ADDR(base)                 ((base) + (0x528UL))

/* 寄存器说明：APLL2控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL2CTRL1_STAT_ADDR(base)                 ((base) + (0x52CUL))

/* 寄存器说明：APLL3控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL0_UNION */
#define SOC_PMCTRL_APLL3CTRL0_ADDR(base)                      ((base) + (0x530UL))

/* 寄存器说明：APLL3控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL1_UNION */
#define SOC_PMCTRL_APLL3CTRL1_ADDR(base)                      ((base) + (0x534UL))

/* 寄存器说明：APLL3控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL3CTRL0_STAT_ADDR(base)                 ((base) + (0x538UL))

/* 寄存器说明：APLL3控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL3CTRL1_STAT_ADDR(base)                 ((base) + (0x53CUL))

/* 寄存器说明：APLL4控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL0_UNION */
#define SOC_PMCTRL_APLL4CTRL0_ADDR(base)                      ((base) + (0x540UL))

/* 寄存器说明：APLL4控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL1_UNION */
#define SOC_PMCTRL_APLL4CTRL1_ADDR(base)                      ((base) + (0x544UL))

/* 寄存器说明：APLL4控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL4CTRL0_STAT_ADDR(base)                 ((base) + (0x548UL))

/* 寄存器说明：APLL4控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL4CTRL1_STAT_ADDR(base)                 ((base) + (0x54CUL))

/* 寄存器说明：APLL5控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL0_UNION */
#define SOC_PMCTRL_APLL5CTRL0_ADDR(base)                      ((base) + (0x550UL))

/* 寄存器说明：APLL5控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL1_UNION */
#define SOC_PMCTRL_APLL5CTRL1_ADDR(base)                      ((base) + (0x554UL))

/* 寄存器说明：APLL5控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL5CTRL0_STAT_ADDR(base)                 ((base) + (0x558UL))

/* 寄存器说明：APLL5控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL5CTRL1_STAT_ADDR(base)                 ((base) + (0x55CUL))

/* 寄存器说明：APLL6控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL0_UNION */
#define SOC_PMCTRL_APLL6CTRL0_ADDR(base)                      ((base) + (0x560UL))

/* 寄存器说明：APLL6控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL1_UNION */
#define SOC_PMCTRL_APLL6CTRL1_ADDR(base)                      ((base) + (0x564UL))

/* 寄存器说明：APLL6控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL6CTRL0_STAT_ADDR(base)                 ((base) + (0x568UL))

/* 寄存器说明：APLL6控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL6CTRL1_STAT_ADDR(base)                 ((base) + (0x56CUL))

/* 寄存器说明：APLL7控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL0_UNION */
#define SOC_PMCTRL_APLL7CTRL0_ADDR(base)                      ((base) + (0x570UL))

/* 寄存器说明：APLL7控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL1_UNION */
#define SOC_PMCTRL_APLL7CTRL1_ADDR(base)                      ((base) + (0x574UL))

/* 寄存器说明：APLL7控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL7CTRL0_STAT_ADDR(base)                 ((base) + (0x578UL))

/* 寄存器说明：APLL7控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL7CTRL1_STAT_ADDR(base)                 ((base) + (0x57CUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL0_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL0_SW_INITIAL0_ADDR(base)               ((base) + (0x5E0UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL0_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL0_SW_INITIAL1_ADDR(base)               ((base) + (0x5E4UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL1_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL1_SW_INITIAL0_ADDR(base)               ((base) + (0x5E8UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL1_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL1_SW_INITIAL1_ADDR(base)               ((base) + (0x5ECUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL2_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL2_SW_INITIAL0_ADDR(base)               ((base) + (0x5F0UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL2_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL2_SW_INITIAL1_ADDR(base)               ((base) + (0x5F4UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL3_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL3_SW_INITIAL0_ADDR(base)               ((base) + (0x5F8UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL3_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL3_SW_INITIAL1_ADDR(base)               ((base) + (0x5FCUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL4_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL4_SW_INITIAL0_ADDR(base)               ((base) + (0x600UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL4_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL4_SW_INITIAL1_ADDR(base)               ((base) + (0x604UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL5_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL5_SW_INITIAL0_ADDR(base)               ((base) + (0x608UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL5_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL5_SW_INITIAL1_ADDR(base)               ((base) + (0x60CUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL6_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL6_SW_INITIAL0_ADDR(base)               ((base) + (0x610UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL6_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL6_SW_INITIAL1_ADDR(base)               ((base) + (0x614UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL7_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL7_SW_INITIAL0_ADDR(base)               ((base) + (0x618UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL7_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL7_SW_INITIAL1_ADDR(base)               ((base) + (0x61CUL))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_A_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_A_ADDR(base, ma)          ((base) + (0xC00 + (ma)*4UL))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_B_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_B_ADDR(base, mb)          ((base) + (0xC80 + (mb)*4UL))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_C_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_C_ADDR(base, mc)          ((base) + (0xD00 + (mc)*4UL))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_D_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_D_ADDR(base, md)          ((base) + (0xD80 + (md)*4UL))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_E_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_E_ADDR(base, me)          ((base) + (0xE00 + (me)*4UL))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_F_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_F_ADDR(base, mf)          ((base) + (0xE80 + (mf)*4UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_A_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_ADDR(base)               ((base) + (0x700UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_B_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_ADDR(base)               ((base) + (0x704UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_C_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_ADDR(base)               ((base) + (0x708UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_D_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_ADDR(base)               ((base) + (0x70CUL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_E_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_ADDR(base)               ((base) + (0x710UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_F_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_ADDR(base)               ((base) + (0x714UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_G_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_ADDR(base)               ((base) + (0x718UL))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_H_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_ADDR(base)               ((base) + (0x71CUL))

/* 寄存器说明：INSTR控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_UNION */
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_ADDR(base)           ((base) + (0x720UL))

/* 寄存器说明：INSTR控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_UNION */
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_ADDR(base)           ((base) + (0x724UL))

/* 寄存器说明：TASKCTRL中断控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_UNION */
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_ADDR(base)            ((base) + (0x730UL))

/* 寄存器说明：TASKCTRL中断控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_UNION */
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_ADDR(base)            ((base) + (0x734UL))

/* 寄存器说明：TASKCTRL中断控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_UNION */
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_ADDR(base)            ((base) + (0x738UL))

/* 寄存器说明：INSTR控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_UNION */
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_ADDR(base)            ((base) + (0x73CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_0_UNION */
#define SOC_PMCTRL_TASKCTRL_A_0_ADDR(base)                    ((base) + (0x740UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_1_UNION */
#define SOC_PMCTRL_TASKCTRL_A_1_ADDR(base)                    ((base) + (0x744UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_2_UNION */
#define SOC_PMCTRL_TASKCTRL_A_2_ADDR(base)                    ((base) + (0x748UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_3_UNION */
#define SOC_PMCTRL_TASKCTRL_A_3_ADDR(base)                    ((base) + (0x74CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_0_UNION */
#define SOC_PMCTRL_TASKCTRL_B_0_ADDR(base)                    ((base) + (0x750UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_1_UNION */
#define SOC_PMCTRL_TASKCTRL_B_1_ADDR(base)                    ((base) + (0x754UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_2_UNION */
#define SOC_PMCTRL_TASKCTRL_B_2_ADDR(base)                    ((base) + (0x758UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_3_UNION */
#define SOC_PMCTRL_TASKCTRL_B_3_ADDR(base)                    ((base) + (0x75CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_0_UNION */
#define SOC_PMCTRL_TASKCTRL_C_0_ADDR(base)                    ((base) + (0x760UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_1_UNION */
#define SOC_PMCTRL_TASKCTRL_C_1_ADDR(base)                    ((base) + (0x764UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_2_UNION */
#define SOC_PMCTRL_TASKCTRL_C_2_ADDR(base)                    ((base) + (0x768UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_3_UNION */
#define SOC_PMCTRL_TASKCTRL_C_3_ADDR(base)                    ((base) + (0x76CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_0_UNION */
#define SOC_PMCTRL_TASKCTRL_D_0_ADDR(base)                    ((base) + (0x770UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_1_UNION */
#define SOC_PMCTRL_TASKCTRL_D_1_ADDR(base)                    ((base) + (0x774UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_2_UNION */
#define SOC_PMCTRL_TASKCTRL_D_2_ADDR(base)                    ((base) + (0x778UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_3_UNION */
#define SOC_PMCTRL_TASKCTRL_D_3_ADDR(base)                    ((base) + (0x77CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_0_UNION */
#define SOC_PMCTRL_TASKCTRL_E_0_ADDR(base)                    ((base) + (0x780UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_1_UNION */
#define SOC_PMCTRL_TASKCTRL_E_1_ADDR(base)                    ((base) + (0x784UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_2_UNION */
#define SOC_PMCTRL_TASKCTRL_E_2_ADDR(base)                    ((base) + (0x788UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_3_UNION */
#define SOC_PMCTRL_TASKCTRL_E_3_ADDR(base)                    ((base) + (0x78CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_0_UNION */
#define SOC_PMCTRL_TASKCTRL_F_0_ADDR(base)                    ((base) + (0x790UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_1_UNION */
#define SOC_PMCTRL_TASKCTRL_F_1_ADDR(base)                    ((base) + (0x794UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_2_UNION */
#define SOC_PMCTRL_TASKCTRL_F_2_ADDR(base)                    ((base) + (0x798UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_3_UNION */
#define SOC_PMCTRL_TASKCTRL_F_3_ADDR(base)                    ((base) + (0x79CUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_0_UNION */
#define SOC_PMCTRL_TASKCTRL_G_0_ADDR(base)                    ((base) + (0x7A0UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_1_UNION */
#define SOC_PMCTRL_TASKCTRL_G_1_ADDR(base)                    ((base) + (0x7A4UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_2_UNION */
#define SOC_PMCTRL_TASKCTRL_G_2_ADDR(base)                    ((base) + (0x7A8UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_3_UNION */
#define SOC_PMCTRL_TASKCTRL_G_3_ADDR(base)                    ((base) + (0x7ACUL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_0_UNION */
#define SOC_PMCTRL_TASKCTRL_H_0_ADDR(base)                    ((base) + (0x7B0UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_1_UNION */
#define SOC_PMCTRL_TASKCTRL_H_1_ADDR(base)                    ((base) + (0x7B4UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_2_UNION */
#define SOC_PMCTRL_TASKCTRL_H_2_ADDR(base)                    ((base) + (0x7B8UL))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_3_UNION */
#define SOC_PMCTRL_TASKCTRL_H_3_ADDR(base)                    ((base) + (0x7BCUL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_ADDR(base)            ((base) + (0x7C0UL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_ADDR(base)            ((base) + (0x7C4UL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_ADDR(base)            ((base) + (0x7C8UL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_ADDR(base)            ((base) + (0x7CCUL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_ADDR(base)            ((base) + (0x7D0UL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_ADDR(base)            ((base) + (0x7D4UL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_ADDR(base)            ((base) + (0x7D8UL))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_ADDR(base)            ((base) + (0x7DCUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_UNION */
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_ADDR(base)             ((base) + (0x7E0UL))

/* 寄存器说明：COFSM VOL SENDOUT
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_SENDOUT_UNION */
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_ADDR(base)               ((base) + (0x7E4UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_ADDR(base)          ((base) + (0x800UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_ADDR(base)          ((base) + (0x804UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_ADDR(base)          ((base) + (0x808UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_ADDR(base)          ((base) + (0x80CUL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_ADDR(base)          ((base) + (0x810UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_ADDR(base)          ((base) + (0x814UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_ADDR(base)          ((base) + (0x818UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_ADDR(base)          ((base) + (0x81CUL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_ADDR(base)          ((base) + (0x820UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_ADDR(base)          ((base) + (0x824UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_ADDR(base)          ((base) + (0x828UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_ADDR(base)          ((base) + (0x82CUL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_ADDR(base)          ((base) + (0x830UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_ADDR(base)          ((base) + (0x834UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_ADDR(base)          ((base) + (0x838UL))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_ADDR(base)          ((base) + (0x83CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_ADDR(base)        ((base) + (0x840UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_ADDR(base)        ((base) + (0x844UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_ADDR(base)        ((base) + (0x848UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_ADDR(base)        ((base) + (0x84CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_ADDR(base)          ((base) + (0x850UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_ADDR(base)          ((base) + (0x854UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_ADDR(base)          ((base) + (0x858UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_ADDR(base)          ((base) + (0x85CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_ADDR(base)        ((base) + (0x860UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_ADDR(base)        ((base) + (0x864UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_ADDR(base)        ((base) + (0x868UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_ADDR(base)        ((base) + (0x86CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_ADDR(base)          ((base) + (0x870UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_ADDR(base)          ((base) + (0x874UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_ADDR(base)          ((base) + (0x878UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_ADDR(base)          ((base) + (0x87CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_ADDR(base)        ((base) + (0x880UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_ADDR(base)        ((base) + (0x884UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_ADDR(base)        ((base) + (0x888UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_ADDR(base)        ((base) + (0x88CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_ADDR(base)          ((base) + (0x890UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_ADDR(base)          ((base) + (0x894UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_ADDR(base)          ((base) + (0x898UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_ADDR(base)          ((base) + (0x89CUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_ADDR(base)        ((base) + (0x8A0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_ADDR(base)        ((base) + (0x8A4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_ADDR(base)        ((base) + (0x8A8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_ADDR(base)        ((base) + (0x8ACUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_ADDR(base)          ((base) + (0x8B0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_ADDR(base)          ((base) + (0x8B4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_ADDR(base)          ((base) + (0x8B8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_ADDR(base)          ((base) + (0x8BCUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_ADDR(base)        ((base) + (0x8C0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_ADDR(base)        ((base) + (0x8C4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_ADDR(base)        ((base) + (0x8C8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_ADDR(base)        ((base) + (0x8CCUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_ADDR(base)          ((base) + (0x8D0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_ADDR(base)          ((base) + (0x8D4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_ADDR(base)          ((base) + (0x8D8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_ADDR(base)          ((base) + (0x8DCUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_ADDR(base)        ((base) + (0x8E0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_ADDR(base)        ((base) + (0x8E4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_ADDR(base)        ((base) + (0x8E8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_ADDR(base)        ((base) + (0x8ECUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_ADDR(base)          ((base) + (0x8F0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_ADDR(base)          ((base) + (0x8F4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_ADDR(base)          ((base) + (0x8F8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_ADDR(base)          ((base) + (0x8FCUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_ADDR(base)        ((base) + (0x9A0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_ADDR(base)        ((base) + (0x9A4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_ADDR(base)        ((base) + (0x9A8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_ADDR(base)        ((base) + (0x9ACUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_ADDR(base)          ((base) + (0x9B0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_ADDR(base)          ((base) + (0x9B4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_ADDR(base)          ((base) + (0x9B8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_ADDR(base)          ((base) + (0x9BCUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_ADDR(base)        ((base) + (0x9C0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_ADDR(base)        ((base) + (0x9C4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_ADDR(base)        ((base) + (0x9C8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_ADDR(base)        ((base) + (0x9CCUL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_ADDR(base)          ((base) + (0x9D0UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_ADDR(base)          ((base) + (0x9D4UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_ADDR(base)          ((base) + (0x9D8UL))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_ADDR(base)          ((base) + (0x9DCUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_ADDR(base)        ((base) + (0x670UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_ADDR(base)        ((base) + (0x674UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_ADDR(base)          ((base) + (0x678UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_ADDR(base)          ((base) + (0x67CUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_ADDR(base)        ((base) + (0x680UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_ADDR(base)        ((base) + (0x684UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_ADDR(base)          ((base) + (0x688UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_ADDR(base)          ((base) + (0x68CUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_ADDR(base)        ((base) + (0x690UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_ADDR(base)        ((base) + (0x694UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_ADDR(base)          ((base) + (0x698UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_ADDR(base)          ((base) + (0x69CUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_ADDR(base)        ((base) + (0x6A0UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_ADDR(base)        ((base) + (0x6A4UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_ADDR(base)          ((base) + (0x6A8UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_ADDR(base)          ((base) + (0x6ACUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_ADDR(base)        ((base) + (0x6B0UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_ADDR(base)        ((base) + (0x6B4UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_ADDR(base)          ((base) + (0x6B8UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_ADDR(base)          ((base) + (0x6BCUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_ADDR(base)        ((base) + (0x6C0UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_ADDR(base)        ((base) + (0x6C4UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_ADDR(base)          ((base) + (0x6C8UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_ADDR(base)          ((base) + (0x6CCUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_ADDR(base)        ((base) + (0x6D0UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_ADDR(base)        ((base) + (0x6D4UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_ADDR(base)          ((base) + (0x6D8UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_ADDR(base)          ((base) + (0x6DCUL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_ADDR(base)        ((base) + (0x6E0UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_ADDR(base)        ((base) + (0x6E4UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_ADDR(base)          ((base) + (0x6E8UL))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_ADDR(base)          ((base) + (0x6ECUL))

/* 寄存器说明：PERI 投票源0电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_ADDR(base)             ((base) + (0xA00UL))

/* 寄存器说明：PERI 投票源1电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_ADDR(base)             ((base) + (0xA04UL))

/* 寄存器说明：PERI 投票源2电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_ADDR(base)             ((base) + (0xA08UL))

/* 寄存器说明：PERI 投票源3电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_ADDR(base)             ((base) + (0xA0CUL))

/* 寄存器说明：PERI 投票源4电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_ADDR(base)             ((base) + (0xA10UL))

/* 寄存器说明：PERI 投票源5电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_ADDR(base)             ((base) + (0xA14UL))

/* 寄存器说明：PERI 投票源6电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_ADDR(base)             ((base) + (0xA18UL))

/* 寄存器说明：PERI 投票源7电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_ADDR(base)             ((base) + (0xA1CUL))

/* 寄存器说明：PERI 投票源8电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_ADDR(base)             ((base) + (0xA20UL))

/* 寄存器说明：PERI 投票源9电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_ADDR(base)             ((base) + (0xA24UL))

/* 寄存器说明：PERI 投票源10电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_ADDR(base)            ((base) + (0xA28UL))

/* 寄存器说明：PERI 投票源11电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_ADDR(base)            ((base) + (0xA2CUL))

/* 寄存器说明：PERI 投票源12电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_ADDR(base)            ((base) + (0xA30UL))

/* 寄存器说明：PERI 投票源13电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_ADDR(base)            ((base) + (0xA34UL))

/* 寄存器说明：PERI 投票源14电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_ADDR(base)            ((base) + (0xA38UL))

/* 寄存器说明：PERI 投票源15电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_ADDR(base)            ((base) + (0xA3CUL))

/* 寄存器说明：PERI 投票源电压档位
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_ADDR(base)        ((base) + (0x000UL))

/* 寄存器说明：PERI 投票源电压档位
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_ADDR(base)        ((base) + (0x004UL))

/* 寄存器说明：PERI 投票源有效状态
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_ADDR(base)             ((base) + (0x008UL))

/* 寄存器说明：PERI 投票源软件调压请求
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_SFT_CHG_UNION */
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_ADDR(base)               ((base) + (0x00CUL))

/* 寄存器说明：PERI 投票源软件调压请求bypass
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_UNION */
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_ADDR(base)        ((base) + (0x010UL))

/* 寄存器说明：PERI升压等待时间
   位域定义UNION结构:  SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_UNION */
#define SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_ADDR(base)           ((base) + (0xA54UL))

/* 寄存器说明：PERI降压等待时间
   位域定义UNION结构:  SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_UNION */
#define SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_ADDR(base)           ((base) + (0xA58UL))

/* 寄存器说明：PERI投票源软件调压完成信号clear
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_SFT_CLR_UNION */
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_ADDR(base)                ((base) + (0x014UL))

/* 寄存器说明：PERI投票源调压完成信号
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_STABLE_UNION */
#define SOC_PMCTRL_PERI_VOL_STABLE_ADDR(base)                 ((base) + (0x018UL))

/* 寄存器说明：PERI DVS初始电压配置
   位域定义UNION结构:  SOC_PMCTRL_PERI_INIT_VOL_CONFIG_UNION */
#define SOC_PMCTRL_PERI_INIT_VOL_CONFIG_ADDR(base)            ((base) + (0xA64UL))

/* 寄存器说明：PERI DVS debug锁电压配置
   位域定义UNION结构:  SOC_PMCTRL_PERI_FIX_VOL_CONFIG_UNION */
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_ADDR(base)             ((base) + (0xA68UL))

/* 寄存器说明：PERI投票源软件调压完成中断MSK
   位域定义UNION结构:  SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_ADDR(base)           ((base) + (0x01CUL))

/* 寄存器说明：PERI电压回读
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_READ_UNION */
#define SOC_PMCTRL_PERI_VOL_READ_ADDR(base)                   ((base) + (0xA70UL))

/* 寄存器说明：PERI DVS 输出回读
   位域定义UNION结构:  SOC_PMCTRL_PERI_DVS_OUTPUT_READ_UNION */
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_ADDR(base)            ((base) + (0x020UL))

/* 寄存器说明：PERI电压档位值
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_RANK_IDX_0_UNION */
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_ADDR(base)             ((base) + (0xA78UL))

/* 寄存器说明：PERI电压档位值
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_RANK_IDX_1_UNION */
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_ADDR(base)             ((base) + (0xA7CUL))

/* 寄存器说明：PERI 4路硬件通道软件调压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_UNION */
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_ADDR(base)       ((base) + (0xA80UL))

/* 寄存器说明：PERI响应下一次调压请求hold时间
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT_TIME_HOLD_UNION */
#define SOC_PMCTRL_PERI_STAT_TIME_HOLD_ADDR(base)             ((base) + (0xA84UL))

/* 寄存器说明：PERI软件投票源调压完成中断bypass
   位域定义UNION结构:  SOC_PMCTRL_PERI_INTR_BYPASS_SFT_UNION */
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_ADDR(base)            ((base) + (0x024UL))

/* 寄存器说明：PERI硬件通道调压完成中断bypass，reserverd bit mask借用其他功能
   位域定义UNION结构:  SOC_PMCTRL_PERI_INTR_BYPASS_HRD_UNION */
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_ADDR(base)            ((base) + (0x028UL))

/* 寄存器说明：PERI 调压请求flag
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_CHG_FLAG_UNION */
#define SOC_PMCTRL_PERI_VOL_CHG_FLAG_ADDR(base)               ((base) + (0xA90UL))

/* 寄存器说明：硬件通道请求电压回读
   位域定义UNION结构:  SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_UNION */
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_ADDR(base)          ((base) + (0xA94UL))

/* 寄存器说明：SVFD/VDM使能状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_ADDR(base)             ((base) + (0x620UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_ADDR(base)            ((base) + (0x624UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_ADDR(base)            ((base) + (0x628UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_ADDR(base)            ((base) + (0x62CUL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_ADDR(base)            ((base) + (0x630UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_ADDR(base)            ((base) + (0x634UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_ADDR(base)            ((base) + (0x638UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_ADDR(base)            ((base) + (0x63CUL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_ADDR(base)            ((base) + (0x640UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_ADDR(base)         ((base) + (0x644UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_ADDR(base)         ((base) + (0x648UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_ADDR(base)         ((base) + (0x64CUL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_ADDR(base)         ((base) + (0x650UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_ADDR(base)         ((base) + (0x654UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_ADDR(base)         ((base) + (0x658UL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_ADDR(base)         ((base) + (0x65CUL))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_ADDR(base)         ((base) + (0x660UL))

/* 寄存器说明：SVFD/VDM软件bypass寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_ADDR(base)      ((base) + (0x664UL))


#else


/* 寄存器说明：APLL硬件控制来源配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_APLL_CTRL_SRC_UNION */
#define SOC_PMCTRL_APLL_CTRL_SRC_ADDR(base)                   ((base) + (0x100))

/* 寄存器说明：子系统V和F控制来源配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_UNION */
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_ADDR(base)        ((base) + (0x104))

/* 寄存器说明：调压通道配置
   位域定义UNION结构:  SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_UNION */
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_ADDR(base)         ((base) + (0x108))

/* 寄存器说明：通用时间配置
   位域定义UNION结构:  SOC_PMCTRL_TIME_PARAMETER_CFG_0_UNION */
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_ADDR(base)            ((base) + (0x10C))

/* 寄存器说明：通用时间配置
   位域定义UNION结构:  SOC_PMCTRL_TIME_PARAMETER_CFG_1_UNION */
#define SOC_PMCTRL_TIME_PARAMETER_CFG_1_ADDR(base)            ((base) + (0x110))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUL_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_ADDR(base)              ((base) + (0x150))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUM_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_ADDR(base)              ((base) + (0x154))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUB_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_ADDR(base)              ((base) + (0x158))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_FCM_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_ADDR(base)               ((base) + (0x15C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_ADDR(base)               ((base) + (0x160))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_UNION */
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_ADDR(base)           ((base) + (0x164))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUL_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_ADDR(base)              ((base) + (0x170))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUM_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_ADDR(base)              ((base) + (0x174))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_CPUB_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_ADDR(base)              ((base) + (0x178))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_FCM_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_ADDR(base)               ((base) + (0x17C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_ADDR(base)          ((base) + (0x180))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_UNION */
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_ADDR(base)           ((base) + (0x184))

/* 寄存器说明：GPU HPM时钟分频寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMCLKDIV_UNION */
#define SOC_PMCTRL_GPU_HPMCLKDIV_ADDR(base)                   ((base) + (0x120))

/* 寄存器说明：GPU HPM选择寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMSEL_UNION */
#define SOC_PMCTRL_GPU_HPMSEL_ADDR(base)                      ((base) + (0x124))

/* 寄存器说明：GPU HPM使能寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMEN_UNION */
#define SOC_PMCTRL_GPU_HPMEN_ADDR(base)                       ((base) + (0x128))

/* 寄存器说明：GPU HPM OPC回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_GPU_HPMOPC_UNION */
#define SOC_PMCTRL_GPU_HPMOPC_ADDR(base)                      ((base) + (0x12C))

/* 寄存器说明：软件备份寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA0_UNION */
#define SOC_PMCTRL_PERIBAKDATA0_ADDR(base)                    ((base) + (0x130))

/* 寄存器说明：软件备份寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA1_UNION */
#define SOC_PMCTRL_PERIBAKDATA1_ADDR(base)                    ((base) + (0x134))

/* 寄存器说明：软件备份寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA2_UNION */
#define SOC_PMCTRL_PERIBAKDATA2_ADDR(base)                    ((base) + (0x138))

/* 寄存器说明：软件备份寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERIBAKDATA3_UNION */
#define SOC_PMCTRL_PERIBAKDATA3_ADDR(base)                    ((base) + (0x13C))

/* 寄存器说明：SMPI电压控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SPMIVOLCFG_UNION */
#define SOC_PMCTRL_SPMIVOLCFG_ADDR(base)                      ((base) + (0x114))

/* 寄存器说明：SMPI电压控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SPMIVOLCFG1_UNION */
#define SOC_PMCTRL_SPMIVOLCFG1_ADDR(base)                     ((base) + (0x118))

/* 寄存器说明：G3D自动分频bypass控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_UNION */
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_ADDR(base)             ((base) + (0x1D8))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_A0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_A0_ADDR(base)                    ((base) + (0x200))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_A1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_A1_ADDR(base)                    ((base) + (0x204))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_B0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_B0_ADDR(base)                    ((base) + (0x208))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_B1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_B1_ADDR(base)                    ((base) + (0x20C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_C0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_C0_ADDR(base)                    ((base) + (0x210))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_C1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_C1_ADDR(base)                    ((base) + (0x214))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_D0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_D0_ADDR(base)                    ((base) + (0x218))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_D1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_D1_ADDR(base)                    ((base) + (0x21C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E0_ADDR(base)                    ((base) + (0x220))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E1_ADDR(base)                    ((base) + (0x224))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E2_ADDR(base)                    ((base) + (0x228))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_E3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_E3_ADDR(base)                    ((base) + (0x22C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F0_ADDR(base)                    ((base) + (0x230))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F1_ADDR(base)                    ((base) + (0x234))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F2_ADDR(base)                    ((base) + (0x238))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_F3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_F3_ADDR(base)                    ((base) + (0x23C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G0_ADDR(base)                    ((base) + (0x240))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G1_ADDR(base)                    ((base) + (0x244))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G2_ADDR(base)                    ((base) + (0x248))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_G3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_G3_ADDR(base)                    ((base) + (0x24C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H0_ADDR(base)                    ((base) + (0x250))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H1_ADDR(base)                    ((base) + (0x254))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H2_ADDR(base)                    ((base) + (0x258))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H3_ADDR(base)                    ((base) + (0x25C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H4_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H4_ADDR(base)                    ((base) + (0x260))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H5_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H5_ADDR(base)                    ((base) + (0x264))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H6_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H6_ADDR(base)                    ((base) + (0x268))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_H7_UNION */
#define SOC_PMCTRL_FREQ_VOTE_H7_ADDR(base)                    ((base) + (0x26C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I0_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I0_ADDR(base)                    ((base) + (0x270))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I1_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I1_ADDR(base)                    ((base) + (0x274))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I2_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I2_ADDR(base)                    ((base) + (0x278))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I3_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I3_ADDR(base)                    ((base) + (0x27C))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I4_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I4_ADDR(base)                    ((base) + (0x280))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I5_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I5_ADDR(base)                    ((base) + (0x284))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I6_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I6_ADDR(base)                    ((base) + (0x288))

/* 寄存器说明：频率投票寄存器
   位域定义UNION结构:  SOC_PMCTRL_FREQ_VOTE_I7_UNION */
#define SOC_PMCTRL_FREQ_VOTE_I7_ADDR(base)                    ((base) + (0x28C))

/* 寄存器说明：投票通道数配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_CHANNEL_CFG_UNION */
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_ADDR(base)                ((base) + (0x2B0))

/* 寄存器说明：投票类型配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_MODE_CFG_0_UNION */
#define SOC_PMCTRL_VOTE_MODE_CFG_0_ADDR(base)                 ((base) + (0x2BC))

/* 寄存器说明：投票类型配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_MODE_CFG_1_UNION */
#define SOC_PMCTRL_VOTE_MODE_CFG_1_ADDR(base)                 ((base) + (0x2C0))

/* 寄存器说明：投票使能配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_START_EN_UNION */
#define SOC_PMCTRL_VOTE_START_EN_ADDR(base)                   ((base) + (0x2C4))

/* 寄存器说明：投票中断使能配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_EN_UNION */
#define SOC_PMCTRL_INT_VOTE_EN_ADDR(base)                     ((base) + (0x2C8))

/* 寄存器说明：投票中断清除配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_CLR_UNION */
#define SOC_PMCTRL_INT_VOTE_CLR_ADDR(base)                    ((base) + (0x2CC))

/* 寄存器说明：投票中断原始状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_RAW_STAT_UNION */
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_ADDR(base)               ((base) + (0x2D0))

/* 寄存器说明：投票中断最终状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_VOTE_STAT_UNION */
#define SOC_PMCTRL_INT_VOTE_STAT_ADDR(base)                   ((base) + (0x2D4))

/* 寄存器说明：投票中断寄存器
   位域定义UNION结构:  SOC_PMCTRL_INT_FREQUENCY_VOTE_UNION */
#define SOC_PMCTRL_INT_FREQUENCY_VOTE_ADDR(base)              ((base) + (0x2D8))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_A_UNION */
#define SOC_PMCTRL_VOTE_RESULT_A_ADDR(base)                   ((base) + (0x2DC))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_B_UNION */
#define SOC_PMCTRL_VOTE_RESULT_B_ADDR(base)                   ((base) + (0x2E0))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_C_UNION */
#define SOC_PMCTRL_VOTE_RESULT_C_ADDR(base)                   ((base) + (0x2E4))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_D_UNION */
#define SOC_PMCTRL_VOTE_RESULT_D_ADDR(base)                   ((base) + (0x2E8))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_E_UNION */
#define SOC_PMCTRL_VOTE_RESULT_E_ADDR(base)                   ((base) + (0x2EC))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_F_UNION */
#define SOC_PMCTRL_VOTE_RESULT_F_ADDR(base)                   ((base) + (0x2F0))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_G_UNION */
#define SOC_PMCTRL_VOTE_RESULT_G_ADDR(base)                   ((base) + (0x2F4))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_H_UNION */
#define SOC_PMCTRL_VOTE_RESULT_H_ADDR(base)                   ((base) + (0x2F8))

/* 寄存器说明：投票结果回读寄存器
   位域定义UNION结构:  SOC_PMCTRL_VOTE_RESULT_I_UNION */
#define SOC_PMCTRL_VOTE_RESULT_I_ADDR(base)                   ((base) + (0x2FC))

/* 寄存器说明：DDRC LP控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_DDRLPCTRL_UNION */
#define SOC_PMCTRL_DDRLPCTRL_ADDR(base)                       ((base) + (0x30C))

/* 寄存器说明：外设通用控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL0_UNION */
#define SOC_PMCTRL_PERI_CTRL0_ADDR(base)                      ((base) + (0x340))

/* 寄存器说明：外设通用控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL1_UNION */
#define SOC_PMCTRL_PERI_CTRL1_ADDR(base)                      ((base) + (0x344))

/* 寄存器说明：外设通用控制寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL2_UNION */
#define SOC_PMCTRL_PERI_CTRL2_ADDR(base)                      ((base) + (0x348))

/* 寄存器说明：外设通用控制寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL3_UNION */
#define SOC_PMCTRL_PERI_CTRL3_ADDR(base)                      ((base) + (0x34C))

/* 寄存器说明：外设通用控制寄存器4
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL4_UNION */
#define SOC_PMCTRL_PERI_CTRL4_ADDR(base)                      ((base) + (0x350))

/* 寄存器说明：外设通用控制寄存器5
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL5_UNION */
#define SOC_PMCTRL_PERI_CTRL5_ADDR(base)                      ((base) + (0x354))

/* 寄存器说明：外设通用控制寄存器6
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL6_UNION */
#define SOC_PMCTRL_PERI_CTRL6_ADDR(base)                      ((base) + (0x358))

/* 寄存器说明：外设通用控制寄存器7
   位域定义UNION结构:  SOC_PMCTRL_PERI_CTRL7_UNION */
#define SOC_PMCTRL_PERI_CTRL7_ADDR(base)                      ((base) + (0x35C))

/* 寄存器说明：外设通用状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT0_UNION */
#define SOC_PMCTRL_PERI_STAT0_ADDR(base)                      ((base) + (0x360))

/* 寄存器说明：外设通用状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT1_UNION */
#define SOC_PMCTRL_PERI_STAT1_ADDR(base)                      ((base) + (0x364))

/* 寄存器说明：外设通用状态寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT2_UNION */
#define SOC_PMCTRL_PERI_STAT2_ADDR(base)                      ((base) + (0x368))

/* 寄存器说明：外设通用状态寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT3_UNION */
#define SOC_PMCTRL_PERI_STAT3_ADDR(base)                      ((base) + (0x36C))

/* 寄存器说明：外设通用状态寄存器4
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT4_UNION */
#define SOC_PMCTRL_PERI_STAT4_ADDR(base)                      ((base) + (0x370))

/* 寄存器说明：NOC低功耗请求信号寄存器0
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEREQ_0_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_ADDR(base)             ((base) + (0x380))

/* 寄存器说明：NOC低功耗响应寄存器0
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEACK_0_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_ADDR(base)             ((base) + (0x384))

/* 寄存器说明：NOC低功耗状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLE_0_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLE_0_ADDR(base)                ((base) + (0x388))

/* 寄存器说明：NOC低功耗请求信号寄存器1
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEREQ_1_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_ADDR(base)             ((base) + (0x38C))

/* 寄存器说明：NOC低功耗响应寄存器1
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEACK_1_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_ADDR(base)             ((base) + (0x390))

/* 寄存器说明：NOC低功耗状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLE_1_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLE_1_ADDR(base)                ((base) + (0x394))

/* 寄存器说明：NOC低功耗请求信号寄存器
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEREQ_2_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_2_ADDR(base)             ((base) + (0x398))

/* 寄存器说明：NOC低功耗响应寄存器
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLEACK_2_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLEACK_2_ADDR(base)             ((base) + (0x39C))

/* 寄存器说明：NOC低功耗状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_NOC_POWER_IDLE_2_UNION */
#define SOC_PMCTRL_NOC_POWER_IDLE_2_ADDR(base)                ((base) + (0x374))

/* 寄存器说明：外设中断屏蔽寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT0_MASK_UNION */
#define SOC_PMCTRL_PERI_INT0_MASK_ADDR(base)                  ((base) + (0x3A0))

/* 寄存器说明：外设中断状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT0_STAT_UNION */
#define SOC_PMCTRL_PERI_INT0_STAT_ADDR(base)                  ((base) + (0x3A4))

/* 寄存器说明：外设中断屏蔽寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT1_MASK_UNION */
#define SOC_PMCTRL_PERI_INT1_MASK_ADDR(base)                  ((base) + (0x3A8))

/* 寄存器说明：外设中断状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT1_STAT_UNION */
#define SOC_PMCTRL_PERI_INT1_STAT_ADDR(base)                  ((base) + (0x3AC))

/* 寄存器说明：外设中断屏蔽寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT2_MASK_UNION */
#define SOC_PMCTRL_PERI_INT2_MASK_ADDR(base)                  ((base) + (0x3B0))

/* 寄存器说明：外设中断状态寄存器2
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT2_STAT_UNION */
#define SOC_PMCTRL_PERI_INT2_STAT_ADDR(base)                  ((base) + (0x3B4))

/* 寄存器说明：外设中断屏蔽寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT3_MASK_UNION */
#define SOC_PMCTRL_PERI_INT3_MASK_ADDR(base)                  ((base) + (0x3B8))

/* 寄存器说明：外设中断状态寄存器3
   位域定义UNION结构:  SOC_PMCTRL_PERI_INT3_STAT_UNION */
#define SOC_PMCTRL_PERI_INT3_STAT_ADDR(base)                  ((base) + (0x3BC))

/* 寄存器说明：Vsensor控制使能寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CTRL_EN_GPU_UNION */
#define SOC_PMCTRL_VS_CTRL_EN_GPU_ADDR(base)                  ((base) + (0x440))

/* 寄存器说明：Vsensor bypass寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CTRL_BYPASS_GPU_UNION */
#define SOC_PMCTRL_VS_CTRL_BYPASS_GPU_ADDR(base)              ((base) + (0x444))

/* 寄存器说明：Vsensor控制配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CTRL_GPU_UNION */
#define SOC_PMCTRL_VS_CTRL_GPU_ADDR(base)                     ((base) + (0x448))

/* 寄存器说明：SVFD配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_SVFD_CTRL0_GPU_UNION */
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_ADDR(base)               ((base) + (0x44C))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_SVFD_CTRL1_GPU_UNION */
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_ADDR(base)               ((base) + (0x450))

/* 寄存器说明：SVFD控制配置寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_SVFD_CTRL2_GPU_UNION */
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_ADDR(base)               ((base) + (0x454))

/* 寄存器说明：SVFD状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_UNION */
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_ADDR(base)            ((base) + (0x458))

/* 寄存器说明：SVFD测试状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_VS_TEST_STAT_GPU_UNION */
#define SOC_PMCTRL_VS_TEST_STAT_GPU_ADDR(base)                ((base) + (0x45C))

/* 寄存器说明：BOOTROM FLAG寄存器
   位域定义UNION结构:  SOC_PMCTRL_BOOTROMFLAG_UNION */
#define SOC_PMCTRL_BOOTROMFLAG_ADDR(base)                     ((base) + (0x460))

/* 寄存器说明：PERI HPM使能寄存器
   位域定义UNION结构:  SOC_PMCTRL_PERIHPMEN_UNION */
#define SOC_PMCTRL_PERIHPMEN_ADDR(base)                       ((base) + (0x464))

/* 寄存器说明：PERI HPM时钟分频寄存器
   位域定义UNION结构:  SOC_PMCTRL_PERIHPMCLKDIV_UNION */
#define SOC_PMCTRL_PERIHPMCLKDIV_ADDR(base)                   ((base) + (0x468))

/* 寄存器说明：PERI HPM OPC寄存器
   位域定义UNION结构:  SOC_PMCTRL_PERIHPMOPC_UNION */
#define SOC_PMCTRL_PERIHPMOPC_ADDR(base)                      ((base) + (0x46C))

/* 寄存器说明：外设通用状态寄存器8
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT8_UNION */
#define SOC_PMCTRL_PERI_STAT8_ADDR(base)                      ((base) + (0x4D0))

/* 寄存器说明：外设通用状态寄存器9
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT9_UNION */
#define SOC_PMCTRL_PERI_STAT9_ADDR(base)                      ((base) + (0x4D4))

/* 寄存器说明：外设通用状态寄存器10
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT10_UNION */
#define SOC_PMCTRL_PERI_STAT10_ADDR(base)                     ((base) + (0x4D8))

/* 寄存器说明：外设通用状态寄存器11
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT11_UNION */
#define SOC_PMCTRL_PERI_STAT11_ADDR(base)                     ((base) + (0x4DC))

/* 寄存器说明：外设通用状态寄存器12
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT12_UNION */
#define SOC_PMCTRL_PERI_STAT12_ADDR(base)                     ((base) + (0x4E0))

/* 寄存器说明：外设通用状态寄存器13
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT13_UNION */
#define SOC_PMCTRL_PERI_STAT13_ADDR(base)                     ((base) + (0x4E4))

/* 寄存器说明：外设通用状态寄存器14
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT14_UNION */
#define SOC_PMCTRL_PERI_STAT14_ADDR(base)                     ((base) + (0x4E8))

/* 寄存器说明：外设通用状态寄存器15
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT15_UNION */
#define SOC_PMCTRL_PERI_STAT15_ADDR(base)                     ((base) + (0x4EC))

/* 寄存器说明：APLL0控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL0_UNION */
#define SOC_PMCTRL_APLL0CTRL0_ADDR(base)                      ((base) + (0x500))

/* 寄存器说明：APLL0控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL1_UNION */
#define SOC_PMCTRL_APLL0CTRL1_ADDR(base)                      ((base) + (0x504))

/* 寄存器说明：APLL0控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL0CTRL0_STAT_ADDR(base)                 ((base) + (0x508))

/* 寄存器说明：APLL0控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL0CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL0CTRL1_STAT_ADDR(base)                 ((base) + (0x50C))

/* 寄存器说明：APLL1控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL0_UNION */
#define SOC_PMCTRL_APLL1CTRL0_ADDR(base)                      ((base) + (0x510))

/* 寄存器说明：APLL1控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL1_UNION */
#define SOC_PMCTRL_APLL1CTRL1_ADDR(base)                      ((base) + (0x514))

/* 寄存器说明：APLL1控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL1CTRL0_STAT_ADDR(base)                 ((base) + (0x518))

/* 寄存器说明：APLL1控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL1CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL1CTRL1_STAT_ADDR(base)                 ((base) + (0x51C))

/* 寄存器说明：APLL2控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL0_UNION */
#define SOC_PMCTRL_APLL2CTRL0_ADDR(base)                      ((base) + (0x520))

/* 寄存器说明：APLL2控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL1_UNION */
#define SOC_PMCTRL_APLL2CTRL1_ADDR(base)                      ((base) + (0x524))

/* 寄存器说明：APLL2控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL2CTRL0_STAT_ADDR(base)                 ((base) + (0x528))

/* 寄存器说明：APLL2控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL2CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL2CTRL1_STAT_ADDR(base)                 ((base) + (0x52C))

/* 寄存器说明：APLL3控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL0_UNION */
#define SOC_PMCTRL_APLL3CTRL0_ADDR(base)                      ((base) + (0x530))

/* 寄存器说明：APLL3控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL1_UNION */
#define SOC_PMCTRL_APLL3CTRL1_ADDR(base)                      ((base) + (0x534))

/* 寄存器说明：APLL3控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL3CTRL0_STAT_ADDR(base)                 ((base) + (0x538))

/* 寄存器说明：APLL3控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL3CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL3CTRL1_STAT_ADDR(base)                 ((base) + (0x53C))

/* 寄存器说明：APLL4控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL0_UNION */
#define SOC_PMCTRL_APLL4CTRL0_ADDR(base)                      ((base) + (0x540))

/* 寄存器说明：APLL4控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL1_UNION */
#define SOC_PMCTRL_APLL4CTRL1_ADDR(base)                      ((base) + (0x544))

/* 寄存器说明：APLL4控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL4CTRL0_STAT_ADDR(base)                 ((base) + (0x548))

/* 寄存器说明：APLL4控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL4CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL4CTRL1_STAT_ADDR(base)                 ((base) + (0x54C))

/* 寄存器说明：APLL5控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL0_UNION */
#define SOC_PMCTRL_APLL5CTRL0_ADDR(base)                      ((base) + (0x550))

/* 寄存器说明：APLL5控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL1_UNION */
#define SOC_PMCTRL_APLL5CTRL1_ADDR(base)                      ((base) + (0x554))

/* 寄存器说明：APLL5控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL5CTRL0_STAT_ADDR(base)                 ((base) + (0x558))

/* 寄存器说明：APLL5控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL5CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL5CTRL1_STAT_ADDR(base)                 ((base) + (0x55C))

/* 寄存器说明：APLL6控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL0_UNION */
#define SOC_PMCTRL_APLL6CTRL0_ADDR(base)                      ((base) + (0x560))

/* 寄存器说明：APLL6控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL1_UNION */
#define SOC_PMCTRL_APLL6CTRL1_ADDR(base)                      ((base) + (0x564))

/* 寄存器说明：APLL6控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL6CTRL0_STAT_ADDR(base)                 ((base) + (0x568))

/* 寄存器说明：APLL6控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL6CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL6CTRL1_STAT_ADDR(base)                 ((base) + (0x56C))

/* 寄存器说明：APLL7控制寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL0_UNION */
#define SOC_PMCTRL_APLL7CTRL0_ADDR(base)                      ((base) + (0x570))

/* 寄存器说明：APLL7控制寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL1_UNION */
#define SOC_PMCTRL_APLL7CTRL1_ADDR(base)                      ((base) + (0x574))

/* 寄存器说明：APLL7控制状态寄存器0
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL0_STAT_UNION */
#define SOC_PMCTRL_APLL7CTRL0_STAT_ADDR(base)                 ((base) + (0x578))

/* 寄存器说明：APLL7控制状态寄存器1
   位域定义UNION结构:  SOC_PMCTRL_APLL7CTRL1_STAT_UNION */
#define SOC_PMCTRL_APLL7CTRL1_STAT_ADDR(base)                 ((base) + (0x57C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL0_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL0_SW_INITIAL0_ADDR(base)               ((base) + (0x5E0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL0_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL0_SW_INITIAL1_ADDR(base)               ((base) + (0x5E4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL1_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL1_SW_INITIAL0_ADDR(base)               ((base) + (0x5E8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL1_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL1_SW_INITIAL1_ADDR(base)               ((base) + (0x5EC))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL2_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL2_SW_INITIAL0_ADDR(base)               ((base) + (0x5F0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL2_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL2_SW_INITIAL1_ADDR(base)               ((base) + (0x5F4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL3_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL3_SW_INITIAL0_ADDR(base)               ((base) + (0x5F8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL3_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL3_SW_INITIAL1_ADDR(base)               ((base) + (0x5FC))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL4_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL4_SW_INITIAL0_ADDR(base)               ((base) + (0x600))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL4_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL4_SW_INITIAL1_ADDR(base)               ((base) + (0x604))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL5_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL5_SW_INITIAL0_ADDR(base)               ((base) + (0x608))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL5_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL5_SW_INITIAL1_ADDR(base)               ((base) + (0x60C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL6_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL6_SW_INITIAL0_ADDR(base)               ((base) + (0x610))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL6_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL6_SW_INITIAL1_ADDR(base)               ((base) + (0x614))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL7_SW_INITIAL0_UNION */
#define SOC_PMCTRL_APLL7_SW_INITIAL0_ADDR(base)               ((base) + (0x618))

/* 寄存器说明：
   位域定义UNION结构:  SOC_PMCTRL_APLL7_SW_INITIAL1_UNION */
#define SOC_PMCTRL_APLL7_SW_INITIAL1_ADDR(base)               ((base) + (0x61C))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_A_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_A_ADDR(base, ma)          ((base) + (0xC00 + (ma)*4))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_B_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_B_ADDR(base, mb)          ((base) + (0xC80 + (mb)*4))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_C_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_C_ADDR(base, mc)          ((base) + (0xD00 + (mc)*4))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_D_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_D_ADDR(base, md)          ((base) + (0xD80 + (md)*4))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_E_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_E_ADDR(base, me)          ((base) + (0xE00 + (me)*4))

/* 寄存器说明：DVFS指令寄存器
   位域定义UNION结构:  SOC_PMCTRL_PDVFS_INSTR_LIST_F_UNION */
#define SOC_PMCTRL_PDVFS_INSTR_LIST_F_ADDR(base, mf)          ((base) + (0xE80 + (mf)*4))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_A_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_ADDR(base)               ((base) + (0x700))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_B_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_ADDR(base)               ((base) + (0x704))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_C_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_ADDR(base)               ((base) + (0x708))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_D_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_ADDR(base)               ((base) + (0x70C))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_E_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_ADDR(base)               ((base) + (0x710))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_F_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_ADDR(base)               ((base) + (0x714))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_G_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_ADDR(base)               ((base) + (0x718))

/* 寄存器说明：INSTR地址寄存器
   位域定义UNION结构:  SOC_PMCTRL_INSTR_ADDR_CTRL_H_UNION */
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_ADDR(base)               ((base) + (0x71C))

/* 寄存器说明：INSTR控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_UNION */
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_ADDR(base)           ((base) + (0x720))

/* 寄存器说明：INSTR控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_UNION */
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_ADDR(base)           ((base) + (0x724))

/* 寄存器说明：TASKCTRL中断控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_UNION */
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_ADDR(base)            ((base) + (0x730))

/* 寄存器说明：TASKCTRL中断控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_UNION */
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_ADDR(base)            ((base) + (0x734))

/* 寄存器说明：TASKCTRL中断控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_UNION */
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_ADDR(base)            ((base) + (0x738))

/* 寄存器说明：INSTR控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_UNION */
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_ADDR(base)            ((base) + (0x73C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_0_UNION */
#define SOC_PMCTRL_TASKCTRL_A_0_ADDR(base)                    ((base) + (0x740))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_1_UNION */
#define SOC_PMCTRL_TASKCTRL_A_1_ADDR(base)                    ((base) + (0x744))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_2_UNION */
#define SOC_PMCTRL_TASKCTRL_A_2_ADDR(base)                    ((base) + (0x748))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_A_3_UNION */
#define SOC_PMCTRL_TASKCTRL_A_3_ADDR(base)                    ((base) + (0x74C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_0_UNION */
#define SOC_PMCTRL_TASKCTRL_B_0_ADDR(base)                    ((base) + (0x750))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_1_UNION */
#define SOC_PMCTRL_TASKCTRL_B_1_ADDR(base)                    ((base) + (0x754))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_2_UNION */
#define SOC_PMCTRL_TASKCTRL_B_2_ADDR(base)                    ((base) + (0x758))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_B_3_UNION */
#define SOC_PMCTRL_TASKCTRL_B_3_ADDR(base)                    ((base) + (0x75C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_0_UNION */
#define SOC_PMCTRL_TASKCTRL_C_0_ADDR(base)                    ((base) + (0x760))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_1_UNION */
#define SOC_PMCTRL_TASKCTRL_C_1_ADDR(base)                    ((base) + (0x764))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_2_UNION */
#define SOC_PMCTRL_TASKCTRL_C_2_ADDR(base)                    ((base) + (0x768))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_C_3_UNION */
#define SOC_PMCTRL_TASKCTRL_C_3_ADDR(base)                    ((base) + (0x76C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_0_UNION */
#define SOC_PMCTRL_TASKCTRL_D_0_ADDR(base)                    ((base) + (0x770))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_1_UNION */
#define SOC_PMCTRL_TASKCTRL_D_1_ADDR(base)                    ((base) + (0x774))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_2_UNION */
#define SOC_PMCTRL_TASKCTRL_D_2_ADDR(base)                    ((base) + (0x778))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_D_3_UNION */
#define SOC_PMCTRL_TASKCTRL_D_3_ADDR(base)                    ((base) + (0x77C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_0_UNION */
#define SOC_PMCTRL_TASKCTRL_E_0_ADDR(base)                    ((base) + (0x780))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_1_UNION */
#define SOC_PMCTRL_TASKCTRL_E_1_ADDR(base)                    ((base) + (0x784))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_2_UNION */
#define SOC_PMCTRL_TASKCTRL_E_2_ADDR(base)                    ((base) + (0x788))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_E_3_UNION */
#define SOC_PMCTRL_TASKCTRL_E_3_ADDR(base)                    ((base) + (0x78C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_0_UNION */
#define SOC_PMCTRL_TASKCTRL_F_0_ADDR(base)                    ((base) + (0x790))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_1_UNION */
#define SOC_PMCTRL_TASKCTRL_F_1_ADDR(base)                    ((base) + (0x794))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_2_UNION */
#define SOC_PMCTRL_TASKCTRL_F_2_ADDR(base)                    ((base) + (0x798))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_F_3_UNION */
#define SOC_PMCTRL_TASKCTRL_F_3_ADDR(base)                    ((base) + (0x79C))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_0_UNION */
#define SOC_PMCTRL_TASKCTRL_G_0_ADDR(base)                    ((base) + (0x7A0))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_1_UNION */
#define SOC_PMCTRL_TASKCTRL_G_1_ADDR(base)                    ((base) + (0x7A4))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_2_UNION */
#define SOC_PMCTRL_TASKCTRL_G_2_ADDR(base)                    ((base) + (0x7A8))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_G_3_UNION */
#define SOC_PMCTRL_TASKCTRL_G_3_ADDR(base)                    ((base) + (0x7AC))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_0_UNION */
#define SOC_PMCTRL_TASKCTRL_H_0_ADDR(base)                    ((base) + (0x7B0))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_1_UNION */
#define SOC_PMCTRL_TASKCTRL_H_1_ADDR(base)                    ((base) + (0x7B4))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_2_UNION */
#define SOC_PMCTRL_TASKCTRL_H_2_ADDR(base)                    ((base) + (0x7B8))

/* 寄存器说明：TASKCTRL存器
   位域定义UNION结构:  SOC_PMCTRL_TASKCTRL_H_3_UNION */
#define SOC_PMCTRL_TASKCTRL_H_3_ADDR(base)                    ((base) + (0x7BC))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_ADDR(base)            ((base) + (0x7C0))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_ADDR(base)            ((base) + (0x7C4))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_ADDR(base)            ((base) + (0x7C8))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_ADDR(base)            ((base) + (0x7CC))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_ADDR(base)            ((base) + (0x7D0))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_ADDR(base)            ((base) + (0x7D4))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_ADDR(base)            ((base) + (0x7D8))

/* 寄存器说明：COFSM VOL控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_UNION */
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_ADDR(base)            ((base) + (0x7DC))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_UNION */
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_ADDR(base)             ((base) + (0x7E0))

/* 寄存器说明：COFSM VOL SENDOUT
   位域定义UNION结构:  SOC_PMCTRL_COFSM_VOL_SENDOUT_UNION */
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_ADDR(base)               ((base) + (0x7E4))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_ADDR(base)          ((base) + (0x800))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_ADDR(base)          ((base) + (0x804))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_ADDR(base)          ((base) + (0x808))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_ADDR(base)          ((base) + (0x80C))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_ADDR(base)          ((base) + (0x810))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_ADDR(base)          ((base) + (0x814))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_ADDR(base)          ((base) + (0x818))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_ADDR(base)          ((base) + (0x81C))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_ADDR(base)          ((base) + (0x820))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_ADDR(base)          ((base) + (0x824))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_ADDR(base)          ((base) + (0x828))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_ADDR(base)          ((base) + (0x82C))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_ADDR(base)          ((base) + (0x830))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_ADDR(base)          ((base) + (0x834))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_ADDR(base)          ((base) + (0x838))

/* 寄存器说明：COFSM CLK_DIV控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_UNION */
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_ADDR(base)          ((base) + (0x83C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_ADDR(base)        ((base) + (0x840))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_ADDR(base)        ((base) + (0x844))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_ADDR(base)        ((base) + (0x848))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_ADDR(base)        ((base) + (0x84C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_ADDR(base)          ((base) + (0x850))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_ADDR(base)          ((base) + (0x854))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_ADDR(base)          ((base) + (0x858))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_ADDR(base)          ((base) + (0x85C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_ADDR(base)        ((base) + (0x860))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_ADDR(base)        ((base) + (0x864))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_ADDR(base)        ((base) + (0x868))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_ADDR(base)        ((base) + (0x86C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_ADDR(base)          ((base) + (0x870))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_ADDR(base)          ((base) + (0x874))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_ADDR(base)          ((base) + (0x878))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_ADDR(base)          ((base) + (0x87C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_ADDR(base)        ((base) + (0x880))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_ADDR(base)        ((base) + (0x884))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_ADDR(base)        ((base) + (0x888))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_ADDR(base)        ((base) + (0x88C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_ADDR(base)          ((base) + (0x890))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_ADDR(base)          ((base) + (0x894))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_ADDR(base)          ((base) + (0x898))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_ADDR(base)          ((base) + (0x89C))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_ADDR(base)        ((base) + (0x8A0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_ADDR(base)        ((base) + (0x8A4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_ADDR(base)        ((base) + (0x8A8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_ADDR(base)        ((base) + (0x8AC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_ADDR(base)          ((base) + (0x8B0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_ADDR(base)          ((base) + (0x8B4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_ADDR(base)          ((base) + (0x8B8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_ADDR(base)          ((base) + (0x8BC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_ADDR(base)        ((base) + (0x8C0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_ADDR(base)        ((base) + (0x8C4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_ADDR(base)        ((base) + (0x8C8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_ADDR(base)        ((base) + (0x8CC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_ADDR(base)          ((base) + (0x8D0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_ADDR(base)          ((base) + (0x8D4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_ADDR(base)          ((base) + (0x8D8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_ADDR(base)          ((base) + (0x8DC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_ADDR(base)        ((base) + (0x8E0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_ADDR(base)        ((base) + (0x8E4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_ADDR(base)        ((base) + (0x8E8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_ADDR(base)        ((base) + (0x8EC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_ADDR(base)          ((base) + (0x8F0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_ADDR(base)          ((base) + (0x8F4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_ADDR(base)          ((base) + (0x8F8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_ADDR(base)          ((base) + (0x8FC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_ADDR(base)        ((base) + (0x9A0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_ADDR(base)        ((base) + (0x9A4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_ADDR(base)        ((base) + (0x9A8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_ADDR(base)        ((base) + (0x9AC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_ADDR(base)          ((base) + (0x9B0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_ADDR(base)          ((base) + (0x9B4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_ADDR(base)          ((base) + (0x9B8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_ADDR(base)          ((base) + (0x9BC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_ADDR(base)        ((base) + (0x9C0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_ADDR(base)        ((base) + (0x9C4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_ADDR(base)        ((base) + (0x9C8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_UNION */
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_ADDR(base)        ((base) + (0x9CC))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_ADDR(base)          ((base) + (0x9D0))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_ADDR(base)          ((base) + (0x9D4))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_ADDR(base)          ((base) + (0x9D8))

/* 寄存器说明：COFSM VOL控制
   位域定义UNION结构:  SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_UNION */
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_ADDR(base)          ((base) + (0x9DC))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_ADDR(base)        ((base) + (0x670))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_ADDR(base)        ((base) + (0x674))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_ADDR(base)          ((base) + (0x678))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_ADDR(base)          ((base) + (0x67C))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_ADDR(base)        ((base) + (0x680))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_ADDR(base)        ((base) + (0x684))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_ADDR(base)          ((base) + (0x688))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_ADDR(base)          ((base) + (0x68C))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_ADDR(base)        ((base) + (0x690))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_ADDR(base)        ((base) + (0x694))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_ADDR(base)          ((base) + (0x698))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_ADDR(base)          ((base) + (0x69C))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_ADDR(base)        ((base) + (0x6A0))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_ADDR(base)        ((base) + (0x6A4))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_ADDR(base)          ((base) + (0x6A8))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_ADDR(base)          ((base) + (0x6AC))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_ADDR(base)        ((base) + (0x6B0))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_ADDR(base)        ((base) + (0x6B4))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_ADDR(base)          ((base) + (0x6B8))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_ADDR(base)          ((base) + (0x6BC))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_ADDR(base)        ((base) + (0x6C0))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_ADDR(base)        ((base) + (0x6C4))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_ADDR(base)          ((base) + (0x6C8))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_ADDR(base)          ((base) + (0x6CC))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_ADDR(base)        ((base) + (0x6D0))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_ADDR(base)        ((base) + (0x6D4))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_ADDR(base)          ((base) + (0x6D8))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_ADDR(base)          ((base) + (0x6DC))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_ADDR(base)        ((base) + (0x6E0))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_ADDR(base)        ((base) + (0x6E4))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_ADDR(base)          ((base) + (0x6E8))

/* 寄存器说明：COFSM FREQ控制来源配
   位域定义UNION结构:  SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_UNION */
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_ADDR(base)          ((base) + (0x6EC))

/* 寄存器说明：PERI 投票源0电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_ADDR(base)             ((base) + (0xA00))

/* 寄存器说明：PERI 投票源1电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_ADDR(base)             ((base) + (0xA04))

/* 寄存器说明：PERI 投票源2电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_ADDR(base)             ((base) + (0xA08))

/* 寄存器说明：PERI 投票源3电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_ADDR(base)             ((base) + (0xA0C))

/* 寄存器说明：PERI 投票源4电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_ADDR(base)             ((base) + (0xA10))

/* 寄存器说明：PERI 投票源5电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_ADDR(base)             ((base) + (0xA14))

/* 寄存器说明：PERI 投票源6电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_ADDR(base)             ((base) + (0xA18))

/* 寄存器说明：PERI 投票源7电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_ADDR(base)             ((base) + (0xA1C))

/* 寄存器说明：PERI 投票源8电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_ADDR(base)             ((base) + (0xA20))

/* 寄存器说明：PERI 投票源9电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_ADDR(base)             ((base) + (0xA24))

/* 寄存器说明：PERI 投票源10电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_ADDR(base)            ((base) + (0xA28))

/* 寄存器说明：PERI 投票源11电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_ADDR(base)            ((base) + (0xA2C))

/* 寄存器说明：PERI 投票源12电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_ADDR(base)            ((base) + (0xA30))

/* 寄存器说明：PERI 投票源13电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_ADDR(base)            ((base) + (0xA34))

/* 寄存器说明：PERI 投票源14电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_ADDR(base)            ((base) + (0xA38))

/* 寄存器说明：PERI 投票源15电压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_ADDR(base)            ((base) + (0xA3C))

/* 寄存器说明：PERI 投票源电压档位
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_ADDR(base)        ((base) + (0x000))

/* 寄存器说明：PERI 投票源电压档位
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_ADDR(base)        ((base) + (0x004))

/* 寄存器说明：PERI 投票源有效状态
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION */
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_ADDR(base)             ((base) + (0x008))

/* 寄存器说明：PERI 投票源软件调压请求
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_SFT_CHG_UNION */
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_ADDR(base)               ((base) + (0x00C))

/* 寄存器说明：PERI 投票源软件调压请求bypass
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_UNION */
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_ADDR(base)        ((base) + (0x010))

/* 寄存器说明：PERI升压等待时间
   位域定义UNION结构:  SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_UNION */
#define SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_ADDR(base)           ((base) + (0xA54))

/* 寄存器说明：PERI降压等待时间
   位域定义UNION结构:  SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_UNION */
#define SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_ADDR(base)           ((base) + (0xA58))

/* 寄存器说明：PERI投票源软件调压完成信号clear
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_SFT_CLR_UNION */
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_ADDR(base)                ((base) + (0x014))

/* 寄存器说明：PERI投票源调压完成信号
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_STABLE_UNION */
#define SOC_PMCTRL_PERI_VOL_STABLE_ADDR(base)                 ((base) + (0x018))

/* 寄存器说明：PERI DVS初始电压配置
   位域定义UNION结构:  SOC_PMCTRL_PERI_INIT_VOL_CONFIG_UNION */
#define SOC_PMCTRL_PERI_INIT_VOL_CONFIG_ADDR(base)            ((base) + (0xA64))

/* 寄存器说明：PERI DVS debug锁电压配置
   位域定义UNION结构:  SOC_PMCTRL_PERI_FIX_VOL_CONFIG_UNION */
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_ADDR(base)             ((base) + (0xA68))

/* 寄存器说明：PERI投票源软件调压完成中断MSK
   位域定义UNION结构:  SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_ADDR(base)           ((base) + (0x01C))

/* 寄存器说明：PERI电压回读
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_READ_UNION */
#define SOC_PMCTRL_PERI_VOL_READ_ADDR(base)                   ((base) + (0xA70))

/* 寄存器说明：PERI DVS 输出回读
   位域定义UNION结构:  SOC_PMCTRL_PERI_DVS_OUTPUT_READ_UNION */
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_ADDR(base)            ((base) + (0x020))

/* 寄存器说明：PERI电压档位值
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_RANK_IDX_0_UNION */
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_ADDR(base)             ((base) + (0xA78))

/* 寄存器说明：PERI电压档位值
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_RANK_IDX_1_UNION */
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_ADDR(base)             ((base) + (0xA7C))

/* 寄存器说明：PERI 4路硬件通道软件调压参数
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_UNION */
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_ADDR(base)       ((base) + (0xA80))

/* 寄存器说明：PERI响应下一次调压请求hold时间
   位域定义UNION结构:  SOC_PMCTRL_PERI_STAT_TIME_HOLD_UNION */
#define SOC_PMCTRL_PERI_STAT_TIME_HOLD_ADDR(base)             ((base) + (0xA84))

/* 寄存器说明：PERI软件投票源调压完成中断bypass
   位域定义UNION结构:  SOC_PMCTRL_PERI_INTR_BYPASS_SFT_UNION */
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_ADDR(base)            ((base) + (0x024))

/* 寄存器说明：PERI硬件通道调压完成中断bypass，reserverd bit mask借用其他功能
   位域定义UNION结构:  SOC_PMCTRL_PERI_INTR_BYPASS_HRD_UNION */
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_ADDR(base)            ((base) + (0x028))

/* 寄存器说明：PERI 调压请求flag
   位域定义UNION结构:  SOC_PMCTRL_PERI_VOL_CHG_FLAG_UNION */
#define SOC_PMCTRL_PERI_VOL_CHG_FLAG_ADDR(base)               ((base) + (0xA90))

/* 寄存器说明：硬件通道请求电压回读
   位域定义UNION结构:  SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_UNION */
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_ADDR(base)          ((base) + (0xA94))

/* 寄存器说明：SVFD/VDM使能状态寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_ADDR(base)             ((base) + (0x620))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_ADDR(base)            ((base) + (0x624))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_ADDR(base)            ((base) + (0x628))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_ADDR(base)            ((base) + (0x62C))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_ADDR(base)            ((base) + (0x630))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_ADDR(base)            ((base) + (0x634))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_ADDR(base)            ((base) + (0x638))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_ADDR(base)            ((base) + (0x63C))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_ADDR(base)            ((base) + (0x640))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_ADDR(base)         ((base) + (0x644))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_ADDR(base)         ((base) + (0x648))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_ADDR(base)         ((base) + (0x64C))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_ADDR(base)         ((base) + (0x650))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_ADDR(base)         ((base) + (0x654))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_ADDR(base)         ((base) + (0x658))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_ADDR(base)         ((base) + (0x65C))

/* 寄存器说明：SVFD/VDM控制寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_ADDR(base)         ((base) + (0x660))

/* 寄存器说明：SVFD/VDM软件bypass寄存器
   位域定义UNION结构:  SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_UNION */
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_ADDR(base)      ((base) + (0x664))


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
 结构名    : SOC_PMCTRL_APLL_CTRL_SRC_UNION
 结构说明  : APLL_CTRL_SRC 寄存器结构定义。地址偏移量:0x100，初值:0xFFFFFFFF，宽度:32
 寄存器说明: APLL硬件控制来源配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_ctrl_src : 4;  /* bit[0-3]  : APLL0的硬件控制来源：
                                                          0x0: 来源于COFSMA
                                                          0x1: 来源于COFSMB
                                                          0x2: 来源于COFSMC
                                                          0x3: 来源于COFSMD
                                                          0x4: 来源于COFSME
                                                          0x5: 来源于COFSMF
                                                          0x6: 来源于COFSMG
                                                          0x7: 来源于COFSMH
                                                          Others:不控制 */
        unsigned int  apll1_ctrl_src : 4;  /* bit[4-7]  :  */
        unsigned int  apll2_ctrl_src : 4;  /* bit[8-11] :  */
        unsigned int  apll3_ctrl_src : 4;  /* bit[12-15]:  */
        unsigned int  apll4_ctrl_src : 4;  /* bit[16-19]:  */
        unsigned int  apll5_ctrl_src : 4;  /* bit[20-23]:  */
        unsigned int  apll6_ctrl_src : 4;  /* bit[24-27]:  */
        unsigned int  apll7_ctrl_src : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_APLL_CTRL_SRC_UNION;
#endif
#define SOC_PMCTRL_APLL_CTRL_SRC_apll0_ctrl_src_START  (0)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll0_ctrl_src_END    (3)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll1_ctrl_src_START  (4)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll1_ctrl_src_END    (7)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll2_ctrl_src_START  (8)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll2_ctrl_src_END    (11)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll3_ctrl_src_START  (12)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll3_ctrl_src_END    (15)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll4_ctrl_src_START  (16)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll4_ctrl_src_END    (19)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll5_ctrl_src_START  (20)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll5_ctrl_src_END    (23)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll6_ctrl_src_START  (24)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll6_ctrl_src_END    (27)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll7_ctrl_src_START  (28)
#define SOC_PMCTRL_APLL_CTRL_SRC_apll7_ctrl_src_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_UNION
 结构说明  : SUBSYS_VOL_FREQ_CTRL_SRC 寄存器结构定义。地址偏移量:0x104，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 子系统V和F控制来源配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpul_vol_freq_ctrl_src     : 4;  /* bit[0-3]  : CPUL VOL和FREQ硬件控制来源：
                                                                      0x0: 来源于COFSMA
                                                                      0x1: 来源于COFSMB
                                                                      0x2: 来源于COFSMC
                                                                      0x3: 来源于COFSMD
                                                                      0x4: 来源于COFSME
                                                                      0x5: 来源于COFSMF
                                                                      0x6: 来源于COFSMG
                                                                      0x7: 来源于COFSMH */
        unsigned int  cpum_vol_freq_ctrl_src     : 4;  /* bit[4-7]  :  */
        unsigned int  cpub_vol_freq_ctrl_src     : 4;  /* bit[8-11] :  */
        unsigned int  fcm_vol_freq_ctrl_src      : 4;  /* bit[12-15]:  */
        unsigned int  gpu_core_vol_freq_ctrl_src : 4;  /* bit[16-19]:  */
        unsigned int  gpu_top_vol_freq_ctrl_src  : 4;  /* bit[20-23]:  */
        unsigned int  reserved_0                 : 4;  /* bit[24-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_UNION;
#endif
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_cpul_vol_freq_ctrl_src_START      (0)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_cpul_vol_freq_ctrl_src_END        (3)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_cpum_vol_freq_ctrl_src_START      (4)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_cpum_vol_freq_ctrl_src_END        (7)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_cpub_vol_freq_ctrl_src_START      (8)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_cpub_vol_freq_ctrl_src_END        (11)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_fcm_vol_freq_ctrl_src_START       (12)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_fcm_vol_freq_ctrl_src_END         (15)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_gpu_core_vol_freq_ctrl_src_START  (16)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_gpu_core_vol_freq_ctrl_src_END    (19)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_gpu_top_vol_freq_ctrl_src_START   (20)
#define SOC_PMCTRL_SUBSYS_VOL_FREQ_CTRL_SRC_gpu_top_vol_freq_ctrl_src_END     (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_UNION
 结构说明  : SUBSYS_PMU_CHANNEL_CTRL 寄存器结构定义。地址偏移量:0x108，初值:0x00AAAAAA，宽度:32
 寄存器说明: 调压通道配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpul_mem_vol_channel_sel       : 2;  /* bit[0-1]  : 2'b00: i2c; 2'b01: ssi; 2'b10:spmi; 2'b11: NA */
        unsigned int  cpul_logic_vol_channel_sel     : 2;  /* bit[2-3]  :  */
        unsigned int  cpum_mem_vol_channel_sel       : 2;  /* bit[4-5]  :  */
        unsigned int  cpum_logic_vol_channel_sel     : 2;  /* bit[6-7]  :  */
        unsigned int  cpub_mem_vol_channel_sel       : 2;  /* bit[8-9]  :  */
        unsigned int  cpub_logic_vol_channel_sel     : 2;  /* bit[10-11]:  */
        unsigned int  fcm_mem_vol_channel_sel        : 2;  /* bit[12-13]:  */
        unsigned int  fcm_logic_vol_channel_sel      : 2;  /* bit[14-15]:  */
        unsigned int  gpu_core_mem_vol_channel_sel   : 2;  /* bit[16-17]:  */
        unsigned int  gpu_core_logic_vol_channel_sel : 2;  /* bit[18-19]:  */
        unsigned int  gpu_top_mem_vol_channel_sel    : 2;  /* bit[20-21]:  */
        unsigned int  gpu_top_logic_vol_channel_sel  : 2;  /* bit[22-23]:  */
        unsigned int  reserved_0                     : 4;  /* bit[24-27]:  */
        unsigned int  reserved_1                     : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_UNION;
#endif
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpul_mem_vol_channel_sel_START        (0)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpul_mem_vol_channel_sel_END          (1)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpul_logic_vol_channel_sel_START      (2)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpul_logic_vol_channel_sel_END        (3)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpum_mem_vol_channel_sel_START        (4)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpum_mem_vol_channel_sel_END          (5)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpum_logic_vol_channel_sel_START      (6)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpum_logic_vol_channel_sel_END        (7)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpub_mem_vol_channel_sel_START        (8)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpub_mem_vol_channel_sel_END          (9)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpub_logic_vol_channel_sel_START      (10)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_cpub_logic_vol_channel_sel_END        (11)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_fcm_mem_vol_channel_sel_START         (12)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_fcm_mem_vol_channel_sel_END           (13)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_fcm_logic_vol_channel_sel_START       (14)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_fcm_logic_vol_channel_sel_END         (15)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_core_mem_vol_channel_sel_START    (16)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_core_mem_vol_channel_sel_END      (17)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_core_logic_vol_channel_sel_START  (18)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_core_logic_vol_channel_sel_END    (19)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_top_mem_vol_channel_sel_START     (20)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_top_mem_vol_channel_sel_END       (21)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_top_logic_vol_channel_sel_START   (22)
#define SOC_PMCTRL_SUBSYS_PMU_CHANNEL_CTRL_gpu_top_logic_vol_channel_sel_END     (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TIME_PARAMETER_CFG_0_UNION
 结构说明  : TIME_PARAMETER_CFG_0 寄存器结构定义。地址偏移量:0x10C，初值:0x00550F05，宽度:32
 寄存器说明: 通用时间配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll_cfg_time      : 8;  /* bit[0-7]  : APLL缓调频时间（Unit：us） */
        unsigned int  clk_cfg_wait_cycle : 8;  /* bit[8-15] : 硬件配置时钟分频的等待周期 */
        unsigned int  pclk_cycle_per_us  : 10; /* bit[16-25]: 1us所需的pclk周期 */
        unsigned int  reserved_0         : 2;  /* bit[26-27]:  */
        unsigned int  reserved_1         : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TIME_PARAMETER_CFG_0_UNION;
#endif
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_apll_cfg_time_START       (0)
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_apll_cfg_time_END         (7)
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_clk_cfg_wait_cycle_START  (8)
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_clk_cfg_wait_cycle_END    (15)
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_pclk_cycle_per_us_START   (16)
#define SOC_PMCTRL_TIME_PARAMETER_CFG_0_pclk_cycle_per_us_END     (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TIME_PARAMETER_CFG_1_UNION
 结构说明  : TIME_PARAMETER_CFG_1 寄存器结构定义。地址偏移量:0x110，初值:0x00000032，宽度:32
 寄存器说明: 通用时间配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll_lock_time : 12; /* bit[0-11] : APLL lock时间（Unit：us） */
        unsigned int  reserved_0     : 4;  /* bit[12-15]:  */
        unsigned int  reserved_1     : 10; /* bit[16-25]:  */
        unsigned int  reserved_2     : 2;  /* bit[26-27]:  */
        unsigned int  reserved_3     : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TIME_PARAMETER_CFG_1_UNION;
#endif
#define SOC_PMCTRL_TIME_PARAMETER_CFG_1_apll_lock_time_START  (0)
#define SOC_PMCTRL_TIME_PARAMETER_CFG_1_apll_lock_time_END    (11)


/*****************************************************************************
 结构名    : SOC_PMCTRL_CPUL_DVFS_CLK_STAT_UNION
 结构说明  : CPUL_DVFS_CLK_STAT 寄存器结构定义。地址偏移量:0x150，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpul_clk_div_cpu_stat     : 16; /* bit[0-15] :  */
        unsigned int  cpul_clk_div_vdm_stat     : 2;  /* bit[16-17]:  */
        unsigned int  reserved_0                : 2;  /* bit[18-19]:  */
        unsigned int  cpul_clk_div_ppll0_stat   : 4;  /* bit[20-23]:  */
        unsigned int  cpul_gt_clk_ppll0_gt_stat : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                : 3;  /* bit[25-27]:  */
        unsigned int  cpul_pll_sw_stat          : 1;  /* bit[28]   :  */
        unsigned int  cpul_pll_sw_ack_sync      : 1;  /* bit[29]   :  */
        unsigned int  reserved_2                : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_CPUL_DVFS_CLK_STAT_UNION;
#endif
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_clk_div_cpu_stat_START      (0)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_clk_div_cpu_stat_END        (15)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_clk_div_vdm_stat_START      (16)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_clk_div_vdm_stat_END        (17)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_clk_div_ppll0_stat_START    (20)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_clk_div_ppll0_stat_END      (23)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_gt_clk_ppll0_gt_stat_START  (24)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_gt_clk_ppll0_gt_stat_END    (24)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_pll_sw_stat_START           (28)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_pll_sw_stat_END             (28)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_pll_sw_ack_sync_START       (29)
#define SOC_PMCTRL_CPUL_DVFS_CLK_STAT_cpul_pll_sw_ack_sync_END         (29)


/*****************************************************************************
 结构名    : SOC_PMCTRL_CPUM_DVFS_CLK_STAT_UNION
 结构说明  : CPUM_DVFS_CLK_STAT 寄存器结构定义。地址偏移量:0x154，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpum_clk_div_cpu_stat     : 16; /* bit[0-15] :  */
        unsigned int  cpum_clk_div_vdm_stat     : 2;  /* bit[16-17]:  */
        unsigned int  reserved_0                : 2;  /* bit[18-19]:  */
        unsigned int  cpum_clk_div_ppll0_stat   : 4;  /* bit[20-23]:  */
        unsigned int  cpum_gt_clk_ppll0_gt_stat : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                : 3;  /* bit[25-27]:  */
        unsigned int  cpum_pll_sw_stat          : 1;  /* bit[28]   :  */
        unsigned int  cpum_pll_sw_ack_sync      : 1;  /* bit[29]   :  */
        unsigned int  reserved_2                : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_CPUM_DVFS_CLK_STAT_UNION;
#endif
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_clk_div_cpu_stat_START      (0)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_clk_div_cpu_stat_END        (15)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_clk_div_vdm_stat_START      (16)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_clk_div_vdm_stat_END        (17)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_clk_div_ppll0_stat_START    (20)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_clk_div_ppll0_stat_END      (23)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_gt_clk_ppll0_gt_stat_START  (24)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_gt_clk_ppll0_gt_stat_END    (24)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_pll_sw_stat_START           (28)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_pll_sw_stat_END             (28)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_pll_sw_ack_sync_START       (29)
#define SOC_PMCTRL_CPUM_DVFS_CLK_STAT_cpum_pll_sw_ack_sync_END         (29)


/*****************************************************************************
 结构名    : SOC_PMCTRL_CPUB_DVFS_CLK_STAT_UNION
 结构说明  : CPUB_DVFS_CLK_STAT 寄存器结构定义。地址偏移量:0x158，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpub_clk_div_cpu_stat     : 16; /* bit[0-15] :  */
        unsigned int  cpub_clk_div_vdm_stat     : 2;  /* bit[16-17]:  */
        unsigned int  reserved_0                : 2;  /* bit[18-19]:  */
        unsigned int  cpub_clk_div_ppll0_stat   : 4;  /* bit[20-23]:  */
        unsigned int  cpub_gt_clk_ppll0_gt_stat : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                : 3;  /* bit[25-27]:  */
        unsigned int  cpub_pll_sw_stat          : 1;  /* bit[28]   :  */
        unsigned int  cpub_pll_sw_ack_sync      : 1;  /* bit[29]   :  */
        unsigned int  reserved_2                : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_CPUB_DVFS_CLK_STAT_UNION;
#endif
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_clk_div_cpu_stat_START      (0)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_clk_div_cpu_stat_END        (15)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_clk_div_vdm_stat_START      (16)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_clk_div_vdm_stat_END        (17)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_clk_div_ppll0_stat_START    (20)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_clk_div_ppll0_stat_END      (23)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_gt_clk_ppll0_gt_stat_START  (24)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_gt_clk_ppll0_gt_stat_END    (24)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_pll_sw_stat_START           (28)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_pll_sw_stat_END             (28)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_pll_sw_ack_sync_START       (29)
#define SOC_PMCTRL_CPUB_DVFS_CLK_STAT_cpub_pll_sw_ack_sync_END         (29)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FCM_DVFS_CLK_STAT_UNION
 结构说明  : FCM_DVFS_CLK_STAT 寄存器结构定义。地址偏移量:0x15C，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  fcm_clk_div_cpu_stat     : 16; /* bit[0-15] :  */
        unsigned int  fcm_clk_div_vdm_stat     : 2;  /* bit[16-17]:  */
        unsigned int  reserved_0               : 2;  /* bit[18-19]:  */
        unsigned int  fcm_clk_div_ppll0_stat   : 4;  /* bit[20-23]:  */
        unsigned int  fcm_gt_clk_ppll0_gt_stat : 1;  /* bit[24]   :  */
        unsigned int  reserved_1               : 3;  /* bit[25-27]:  */
        unsigned int  fcm_pll_sw_stat          : 1;  /* bit[28]   :  */
        unsigned int  fcm_pll_sw_ack_sync      : 1;  /* bit[29]   :  */
        unsigned int  reserved_2               : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_FCM_DVFS_CLK_STAT_UNION;
#endif
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_clk_div_cpu_stat_START      (0)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_clk_div_cpu_stat_END        (15)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_clk_div_vdm_stat_START      (16)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_clk_div_vdm_stat_END        (17)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_clk_div_ppll0_stat_START    (20)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_clk_div_ppll0_stat_END      (23)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_gt_clk_ppll0_gt_stat_START  (24)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_gt_clk_ppll0_gt_stat_END    (24)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_pll_sw_stat_START           (28)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_pll_sw_stat_END             (28)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_pll_sw_ack_sync_START       (29)
#define SOC_PMCTRL_FCM_DVFS_CLK_STAT_fcm_pll_sw_ack_sync_END         (29)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_DVFS_CLK_STAT_UNION
 结构说明  : GPU_DVFS_CLK_STAT 寄存器结构定义。地址偏移量:0x160，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_clk_div_cpu_stat     : 16; /* bit[0-15] :  */
        unsigned int  gpu_clk_div_vdm_stat     : 2;  /* bit[16-17]:  */
        unsigned int  reserved_0               : 2;  /* bit[18-19]:  */
        unsigned int  gpu_clk_div_ppll0_stat   : 4;  /* bit[20-23]:  */
        unsigned int  gpu_gt_clk_ppll0_gt_stat : 1;  /* bit[24]   :  */
        unsigned int  reserved_1               : 3;  /* bit[25-27]:  */
        unsigned int  gpu_core_pll_sw_stat     : 1;  /* bit[28]   :  */
        unsigned int  gpu_core_pll_sw_ack_sync : 1;  /* bit[29]   :  */
        unsigned int  reserved_2               : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_GPU_DVFS_CLK_STAT_UNION;
#endif
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_clk_div_cpu_stat_START      (0)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_clk_div_cpu_stat_END        (15)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_clk_div_vdm_stat_START      (16)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_clk_div_vdm_stat_END        (17)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_clk_div_ppll0_stat_START    (20)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_clk_div_ppll0_stat_END      (23)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_gt_clk_ppll0_gt_stat_START  (24)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_gt_clk_ppll0_gt_stat_END    (24)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_core_pll_sw_stat_START      (28)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_core_pll_sw_stat_END        (28)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_core_pll_sw_ack_sync_START  (29)
#define SOC_PMCTRL_GPU_DVFS_CLK_STAT_gpu_core_pll_sw_ack_sync_END    (29)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_UNION
 结构说明  : GPU_TOP_DVFS_CLK_STAT 寄存器结构定义。地址偏移量:0x164，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_top_clk_div_cpu_stat     : 16; /* bit[0-15] :  */
        unsigned int  gpu_top_clk_div_vdm_stat     : 2;  /* bit[16-17]:  */
        unsigned int  reserved_0                   : 2;  /* bit[18-19]:  */
        unsigned int  gpu_top_clk_div_ppll0_stat   : 4;  /* bit[20-23]:  */
        unsigned int  gpu_top_gt_clk_ppll0_gt_stat : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                   : 3;  /* bit[25-27]:  */
        unsigned int  gpu_top_pll_sw_stat          : 1;  /* bit[28]   :  */
        unsigned int  gpu_top_pll_sw_ack_sync      : 1;  /* bit[29]   :  */
        unsigned int  reserved_2                   : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_UNION;
#endif
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_clk_div_cpu_stat_START      (0)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_clk_div_cpu_stat_END        (15)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_clk_div_vdm_stat_START      (16)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_clk_div_vdm_stat_END        (17)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_clk_div_ppll0_stat_START    (20)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_clk_div_ppll0_stat_END      (23)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_gt_clk_ppll0_gt_stat_START  (24)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_gt_clk_ppll0_gt_stat_END    (24)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_pll_sw_stat_START           (28)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_pll_sw_stat_END             (28)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_pll_sw_ack_sync_START       (29)
#define SOC_PMCTRL_GPU_TOP_DVFS_CLK_STAT_gpu_top_pll_sw_ack_sync_END         (29)


/*****************************************************************************
 结构名    : SOC_PMCTRL_CPUL_DVFS_VOL_STAT_UNION
 结构说明  : CPUL_DVFS_VOL_STAT 寄存器结构定义。地址偏移量:0x170，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpul_pmu_mem_vol_idx   : 8;  /* bit[0-7]  :  */
        unsigned int  cpul_pmu_mem_vol_chg   : 1;  /* bit[8]    :  */
        unsigned int  reserved_0             : 3;  /* bit[9-11] :  */
        unsigned int  cpul_pmu_logic_vol_idx : 8;  /* bit[12-19]:  */
        unsigned int  cpul_pmu_logic_vol_chg : 1;  /* bit[20]   :  */
        unsigned int  reserved_1             : 3;  /* bit[21-23]:  */
        unsigned int  reserved_2             : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_CPUL_DVFS_VOL_STAT_UNION;
#endif
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_mem_vol_idx_START    (0)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_mem_vol_idx_END      (7)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_mem_vol_chg_START    (8)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_mem_vol_chg_END      (8)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_logic_vol_idx_START  (12)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_logic_vol_idx_END    (19)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_logic_vol_chg_START  (20)
#define SOC_PMCTRL_CPUL_DVFS_VOL_STAT_cpul_pmu_logic_vol_chg_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_CPUM_DVFS_VOL_STAT_UNION
 结构说明  : CPUM_DVFS_VOL_STAT 寄存器结构定义。地址偏移量:0x174，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpum_pmu_mem_vol_idx   : 8;  /* bit[0-7]  :  */
        unsigned int  cpum_pmu_mem_vol_chg   : 1;  /* bit[8]    :  */
        unsigned int  reserved_0             : 3;  /* bit[9-11] :  */
        unsigned int  cpum_pmu_logic_vol_idx : 8;  /* bit[12-19]:  */
        unsigned int  cpum_pmu_logic_vol_chg : 1;  /* bit[20]   :  */
        unsigned int  reserved_1             : 3;  /* bit[21-23]:  */
        unsigned int  reserved_2             : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_CPUM_DVFS_VOL_STAT_UNION;
#endif
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_mem_vol_idx_START    (0)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_mem_vol_idx_END      (7)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_mem_vol_chg_START    (8)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_mem_vol_chg_END      (8)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_logic_vol_idx_START  (12)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_logic_vol_idx_END    (19)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_logic_vol_chg_START  (20)
#define SOC_PMCTRL_CPUM_DVFS_VOL_STAT_cpum_pmu_logic_vol_chg_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_CPUB_DVFS_VOL_STAT_UNION
 结构说明  : CPUB_DVFS_VOL_STAT 寄存器结构定义。地址偏移量:0x178，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cpub_pmu_mem_vol_idx   : 8;  /* bit[0-7]  :  */
        unsigned int  cpub_pmu_mem_vol_chg   : 1;  /* bit[8]    :  */
        unsigned int  reserved_0             : 3;  /* bit[9-11] :  */
        unsigned int  cpub_pmu_logic_vol_idx : 8;  /* bit[12-19]:  */
        unsigned int  cpub_pmu_logic_vol_chg : 1;  /* bit[20]   :  */
        unsigned int  reserved_1             : 3;  /* bit[21-23]:  */
        unsigned int  reserved_2             : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_CPUB_DVFS_VOL_STAT_UNION;
#endif
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_mem_vol_idx_START    (0)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_mem_vol_idx_END      (7)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_mem_vol_chg_START    (8)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_mem_vol_chg_END      (8)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_logic_vol_idx_START  (12)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_logic_vol_idx_END    (19)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_logic_vol_chg_START  (20)
#define SOC_PMCTRL_CPUB_DVFS_VOL_STAT_cpub_pmu_logic_vol_chg_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FCM_DVFS_VOL_STAT_UNION
 结构说明  : FCM_DVFS_VOL_STAT 寄存器结构定义。地址偏移量:0x17C，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  fcm_pmu_mem_vol_idx   : 8;  /* bit[0-7]  :  */
        unsigned int  fcm_pmu_mem_vol_chg   : 1;  /* bit[8]    :  */
        unsigned int  reserved_0            : 3;  /* bit[9-11] :  */
        unsigned int  fcm_pmu_logic_vol_idx : 8;  /* bit[12-19]:  */
        unsigned int  fcm_pmu_logic_vol_chg : 1;  /* bit[20]   :  */
        unsigned int  reserved_1            : 3;  /* bit[21-23]:  */
        unsigned int  reserved_2            : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_FCM_DVFS_VOL_STAT_UNION;
#endif
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_mem_vol_idx_START    (0)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_mem_vol_idx_END      (7)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_mem_vol_chg_START    (8)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_mem_vol_chg_END      (8)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_logic_vol_idx_START  (12)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_logic_vol_idx_END    (19)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_logic_vol_chg_START  (20)
#define SOC_PMCTRL_FCM_DVFS_VOL_STAT_fcm_pmu_logic_vol_chg_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_UNION
 结构说明  : GPU_CORE_DVFS_VOL_STAT 寄存器结构定义。地址偏移量:0x180，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_core_pmu_mem_vol_idx   : 8;  /* bit[0-7]  :  */
        unsigned int  gpu_core_pmu_mem_vol_chg   : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                 : 3;  /* bit[9-11] :  */
        unsigned int  gpu_core_pmu_logic_vol_idx : 8;  /* bit[12-19]:  */
        unsigned int  gpu_core_pmu_logic_vol_chg : 1;  /* bit[20]   :  */
        unsigned int  reserved_1                 : 3;  /* bit[21-23]:  */
        unsigned int  reserved_2                 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_UNION;
#endif
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_mem_vol_idx_START    (0)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_mem_vol_idx_END      (7)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_mem_vol_chg_START    (8)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_mem_vol_chg_END      (8)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_logic_vol_idx_START  (12)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_logic_vol_idx_END    (19)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_logic_vol_chg_START  (20)
#define SOC_PMCTRL_GPU_CORE_DVFS_VOL_STAT_gpu_core_pmu_logic_vol_chg_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_UNION
 结构说明  : GPU_TOP_DVFS_VOL_STAT 寄存器结构定义。地址偏移量:0x184，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_top_pmu_mem_vol_idx   : 8;  /* bit[0-7]  :  */
        unsigned int  gpu_top_pmu_mem_vol_chg   : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                : 3;  /* bit[9-11] :  */
        unsigned int  gpu_top_pmu_logic_vol_idx : 8;  /* bit[12-19]:  */
        unsigned int  gpu_top_pmu_logic_vol_chg : 1;  /* bit[20]   :  */
        unsigned int  reserved_1                : 3;  /* bit[21-23]:  */
        unsigned int  reserved_2                : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_UNION;
#endif
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_mem_vol_idx_START    (0)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_mem_vol_idx_END      (7)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_mem_vol_chg_START    (8)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_mem_vol_chg_END      (8)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_logic_vol_idx_START  (12)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_logic_vol_idx_END    (19)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_logic_vol_chg_START  (20)
#define SOC_PMCTRL_GPU_TOP_DVFS_VOL_STAT_gpu_top_pmu_logic_vol_chg_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_HPMCLKDIV_UNION
 结构说明  : GPU_HPMCLKDIV 寄存器结构定义。地址偏移量:0x120，初值:0x00000001，宽度:32
 寄存器说明: GPU HPM时钟分频寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_hpm_clk_div : 6;  /* bit[0-5] : GPU HPM时钟分频器。 */
        unsigned int  reserved        : 26; /* bit[6-31]: 保留。 */
    } reg;
} SOC_PMCTRL_GPU_HPMCLKDIV_UNION;
#endif
#define SOC_PMCTRL_GPU_HPMCLKDIV_gpu_hpm_clk_div_START  (0)
#define SOC_PMCTRL_GPU_HPMCLKDIV_gpu_hpm_clk_div_END    (5)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_HPMSEL_UNION
 结构说明  : GPU_HPMSEL 寄存器结构定义。地址偏移量:0x124，初值:0x00000001，宽度:32
 寄存器说明: GPU HPM选择寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_hpm_type_sel : 1;  /* bit[0]   : HPM类型选择：1'b1：hpm；1'b0：hpmx。 */
        unsigned int  reserved_0       : 3;  /* bit[1-3] : 保留。 */
        unsigned int  gpu_hpm_addr_sel : 5;  /* bit[4-8] : HPM位置选择（与GPU stack排布对其）：
                                                           5'b00000: stack0
                                                           5'b00001: stack2
                                                           5'b00010: stack4
                                                           5'b00011: stack6
                                                           5'b11111: GPU TOP
                                                           其他：保留。 */
        unsigned int  reserved_1       : 23; /* bit[9-31]: 保留。 */
    } reg;
} SOC_PMCTRL_GPU_HPMSEL_UNION;
#endif
#define SOC_PMCTRL_GPU_HPMSEL_gpu_hpm_type_sel_START  (0)
#define SOC_PMCTRL_GPU_HPMSEL_gpu_hpm_type_sel_END    (0)
#define SOC_PMCTRL_GPU_HPMSEL_gpu_hpm_addr_sel_START  (4)
#define SOC_PMCTRL_GPU_HPMSEL_gpu_hpm_addr_sel_END    (8)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_HPMEN_UNION
 结构说明  : GPU_HPMEN 寄存器结构定义。地址偏移量:0x128，初值:0x00000000，宽度:32
 寄存器说明: GPU HPM使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_hpm_en          : 1;  /* bit[0]    : GPU HPM使能。
                                                               根据GPU_HPMSEL使能不同位置/类型hpm。 */
        unsigned int  reserved_0          : 3;  /* bit[1-3]  :  */
        unsigned int  gpu_hpm_opc_min_en  : 1;  /* bit[4]    : burbank不使用 */
        unsigned int  reserved_1          : 3;  /* bit[5-7]  :  */
        unsigned int  gpu_hpmx_opc_min_en : 1;  /* bit[8]    : burbank不使用 */
        unsigned int  reserved_2          : 1;  /* bit[9]    :  */
        unsigned int  reserved_3          : 22; /* bit[10-31]: 保留。 */
    } reg;
} SOC_PMCTRL_GPU_HPMEN_UNION;
#endif
#define SOC_PMCTRL_GPU_HPMEN_gpu_hpm_en_START           (0)
#define SOC_PMCTRL_GPU_HPMEN_gpu_hpm_en_END             (0)
#define SOC_PMCTRL_GPU_HPMEN_gpu_hpm_opc_min_en_START   (4)
#define SOC_PMCTRL_GPU_HPMEN_gpu_hpm_opc_min_en_END     (4)
#define SOC_PMCTRL_GPU_HPMEN_gpu_hpmx_opc_min_en_START  (8)
#define SOC_PMCTRL_GPU_HPMEN_gpu_hpmx_opc_min_en_END    (8)


/*****************************************************************************
 结构名    : SOC_PMCTRL_GPU_HPMOPC_UNION
 结构说明  : GPU_HPMOPC 寄存器结构定义。地址偏移量:0x12C，初值:0x00000000，宽度:32
 寄存器说明: GPU HPM OPC回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpu_hpm_opc     : 10; /* bit[0-9]  : GPUHPM原始性能码。
                                                           根据GPU_HPMSEL回读不同位置/类型hpm。 */
        unsigned int  reserved_0      : 2;  /* bit[10-11]: 保留。 */
        unsigned int  gpu_hpm_opc_vld : 1;  /* bit[12]   : GPUHPM原始性能码有效信号。
                                                           根据GPU_HPMSEL回读不同位置/类型hpm。 */
        unsigned int  reserved_1      : 3;  /* bit[13-15]:  */
        unsigned int  gpu_hpm_opc_min : 10; /* bit[16-25]: burbank不使用 */
        unsigned int  reserved_2      : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_GPU_HPMOPC_UNION;
#endif
#define SOC_PMCTRL_GPU_HPMOPC_gpu_hpm_opc_START      (0)
#define SOC_PMCTRL_GPU_HPMOPC_gpu_hpm_opc_END        (9)
#define SOC_PMCTRL_GPU_HPMOPC_gpu_hpm_opc_vld_START  (12)
#define SOC_PMCTRL_GPU_HPMOPC_gpu_hpm_opc_vld_END    (12)
#define SOC_PMCTRL_GPU_HPMOPC_gpu_hpm_opc_min_START  (16)
#define SOC_PMCTRL_GPU_HPMOPC_gpu_hpm_opc_min_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIBAKDATA0_UNION
 结构说明  : PERIBAKDATA0 寄存器结构定义。地址偏移量:0x130，初值:0x00000000，宽度:32
 寄存器说明: 软件备份寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 备份寄存器，低功耗控制专用！！！ */
    } reg;
} SOC_PMCTRL_PERIBAKDATA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIBAKDATA1_UNION
 结构说明  : PERIBAKDATA1 寄存器结构定义。地址偏移量:0x134，初值:0x00000000，宽度:32
 寄存器说明: 软件备份寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 备份寄存器，低功耗控制专用！！！ */
    } reg;
} SOC_PMCTRL_PERIBAKDATA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIBAKDATA2_UNION
 结构说明  : PERIBAKDATA2 寄存器结构定义。地址偏移量:0x138，初值:0x00000000，宽度:32
 寄存器说明: 软件备份寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 备份寄存器，低功耗控制专用！！！ */
    } reg;
} SOC_PMCTRL_PERIBAKDATA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIBAKDATA3_UNION
 结构说明  : PERIBAKDATA3 寄存器结构定义。地址偏移量:0x13C，初值:0x00000000，宽度:32
 寄存器说明: 软件备份寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 备份寄存器，低功耗控制专用！！！ */
    } reg;
} SOC_PMCTRL_PERIBAKDATA3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_SPMIVOLCFG_UNION
 结构说明  : SPMIVOLCFG 寄存器结构定义。地址偏移量:0x114，初值:0x00000000，宽度:32
 寄存器说明: SMPI电压控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  spmi_vol_idx_sftcfg0 : 8;  /* bit[0-7]  : SPMI配置当前电压值。该值可以通过软件配置。如果配置该寄存器时bit 8不写1，则需要该寄存器值与PMU电压值相同。（先读后写，防误配） */
        unsigned int  spmi_vol_chg_sftreq0 : 1;  /* bit[8-8]  : SPMI电压改变软件请求信号。
                                                                写1：请求改变电压，电压值为bit 7:0值
                                                                写0：无效果。 */
        unsigned int  reserved_0           : 3;  /* bit[9-11] :  */
        unsigned int  spmi_vol_idx_sftcfg1 : 8;  /* bit[12-19]: SPMI配置当前电压值。该值可以通过软件配置。如果配置该寄存器时bit 20不写1，则需要该寄存器值与PMU电压值相同。（先读后写，防误配） */
        unsigned int  spmi_vol_chg_sftreq1 : 1;  /* bit[20]   : SPMI电压改变软件请求信号。
                                                                写1：请求改变电压，电压值为bit19:12值
                                                                写0：无效果。 */
        unsigned int  reserved_1           : 11; /* bit[21-31]: 保留。 */
    } reg;
} SOC_PMCTRL_SPMIVOLCFG_UNION;
#endif
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_idx_sftcfg0_START  (0)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_idx_sftcfg0_END    (7)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_chg_sftreq0_START  (8)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_chg_sftreq0_END    (8)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_idx_sftcfg1_START  (12)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_idx_sftcfg1_END    (19)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_chg_sftreq1_START  (20)
#define SOC_PMCTRL_SPMIVOLCFG_spmi_vol_chg_sftreq1_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SPMIVOLCFG1_UNION
 结构说明  : SPMIVOLCFG1 寄存器结构定义。地址偏移量:0x118，初值:0x00000000，宽度:32
 寄存器说明: SMPI电压控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  spmi_vol_idx_sftcfg2 : 8;  /* bit[0-7]  : SPMI配置当前电压值。该值可以通过软件配置。如果配置该寄存器时bit 8不写1，则需要该寄存器值与PMU电压值相同。（先读后写，防误配） */
        unsigned int  spmi_vol_chg_sftreq2 : 1;  /* bit[8-8]  : SPMI电压改变软件请求信号。
                                                                写1：请求改变电压，电压值为bit 7:0值
                                                                写0：无效果。 */
        unsigned int  reserved_0           : 3;  /* bit[9-11] :  */
        unsigned int  spmi_vol_idx_sftcfg3 : 8;  /* bit[12-19]: SPMI配置当前电压值。该值可以通过软件配置。如果配置该寄存器时bit 20不写1，则需要该寄存器值与PMU电压值相同。（先读后写，防误配） */
        unsigned int  spmi_vol_chg_sftreq3 : 1;  /* bit[20]   : SPMI电压改变软件请求信号。
                                                                写1：请求改变电压，电压值为bit19:12值
                                                                写0：无效果。 */
        unsigned int  reserved_1           : 11; /* bit[21-31]: 保留。 */
    } reg;
} SOC_PMCTRL_SPMIVOLCFG1_UNION;
#endif
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_idx_sftcfg2_START  (0)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_idx_sftcfg2_END    (7)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_chg_sftreq2_START  (8)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_chg_sftreq2_END    (8)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_idx_sftcfg3_START  (12)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_idx_sftcfg3_END    (19)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_chg_sftreq3_START  (20)
#define SOC_PMCTRL_SPMIVOLCFG1_spmi_vol_chg_sftreq3_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_UNION
 结构说明  : G3DAUTOCLKDIVBYPASS 寄存器结构定义。地址偏移量:0x1D8，初值:0x000C3F15，宽度:32
 寄存器说明: G3D自动分频bypass控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  g3d_auto_clkdiv_bypass : 1;  /* bit[0-0]  : G3D shader上下电时硬件自动分频功能bypass控制。
                                                                  0：G3D 自动分频功能开启；
                                                                  1：G3D 自动分频功能关闭。 */
        unsigned int  g3d_auto_clkdiv_time   : 7;  /* bit[1-7]  : G3D自动分频后稳定时间。
                                                                  计数单位是PMCTRL工作时钟。 */
        unsigned int  g3d_pwr_dly_cnt        : 8;  /* bit[8-15] : G3D pwr_updn_ack持续的周期数。
                                                                  计数单位是PMCTRL工作时钟。 */
        unsigned int  reserved_0             : 2;  /* bit[16-17]: 保留 */
        unsigned int  g3d_div_debounce       : 6;  /* bit[18-23]: G3D shader上下电时自动降频请求的防抖周期数。 */
        unsigned int  reserved_1             : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_UNION;
#endif
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_auto_clkdiv_bypass_START  (0)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_auto_clkdiv_bypass_END    (0)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_auto_clkdiv_time_START    (1)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_auto_clkdiv_time_END      (7)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_pwr_dly_cnt_START         (8)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_pwr_dly_cnt_END           (15)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_div_debounce_START        (18)
#define SOC_PMCTRL_G3DAUTOCLKDIVBYPASS_g3d_div_debounce_END          (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_A0_UNION
 结构说明  : FREQ_VOTE_A0 寄存器结构定义。地址偏移量:0x200，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_a_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_a0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_a_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_a0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_A0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_A0_freq_vote_a0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_A1_UNION
 结构说明  : FREQ_VOTE_A1 寄存器结构定义。地址偏移量:0x204，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_a_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_a1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_a_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_a1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_A1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_A1_freq_vote_a1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_B0_UNION
 结构说明  : FREQ_VOTE_B0 寄存器结构定义。地址偏移量:0x208，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_b_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_b0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_b_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_b0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_B0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_B0_freq_vote_b0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_B1_UNION
 结构说明  : FREQ_VOTE_B1 寄存器结构定义。地址偏移量:0x20C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_b_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_b1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_b_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_b1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_B1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_B1_freq_vote_b1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_C0_UNION
 结构说明  : FREQ_VOTE_C0 寄存器结构定义。地址偏移量:0x210，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_c_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_c0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_c_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_c0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_C0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_C0_freq_vote_c0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_C1_UNION
 结构说明  : FREQ_VOTE_C1 寄存器结构定义。地址偏移量:0x214，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_c_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_c1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_c_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_c1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_C1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_C1_freq_vote_c1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_D0_UNION
 结构说明  : FREQ_VOTE_D0 寄存器结构定义。地址偏移量:0x218，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_d_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_d0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_d_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_d0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_D0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_D0_freq_vote_d0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_D1_UNION
 结构说明  : FREQ_VOTE_D1 寄存器结构定义。地址偏移量:0x21C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_d_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_d1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_d_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_d1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_D1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_D1_freq_vote_d1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_E0_UNION
 结构说明  : FREQ_VOTE_E0 寄存器结构定义。地址偏移量:0x220，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_e_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_e0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_e_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_e0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_E0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_E0_freq_vote_e0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_E1_UNION
 结构说明  : FREQ_VOTE_E1 寄存器结构定义。地址偏移量:0x224，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_e_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_e1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_e_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_e1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_E1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_E1_freq_vote_e1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_E2_UNION
 结构说明  : FREQ_VOTE_E2 寄存器结构定义。地址偏移量:0x228，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_e_channel4 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_e2_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_e_channel5 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_e2_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_E2_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e_channel4_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e_channel4_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e2_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e2_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e_channel5_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e_channel5_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e2_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_E2_freq_vote_e2_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_E3_UNION
 结构说明  : FREQ_VOTE_E3 寄存器结构定义。地址偏移量:0x22C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_e_channel6 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_e3_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_e_channel7 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_e3_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_E3_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e_channel6_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e_channel6_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e3_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e3_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e_channel7_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e_channel7_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e3_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_E3_freq_vote_e3_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_F0_UNION
 结构说明  : FREQ_VOTE_F0 寄存器结构定义。地址偏移量:0x230，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_f_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_f0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_f_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_f0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_F0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_F0_freq_vote_f0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_F1_UNION
 结构说明  : FREQ_VOTE_F1 寄存器结构定义。地址偏移量:0x234，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_f_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_f1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_f_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_f1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_F1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_F1_freq_vote_f1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_F2_UNION
 结构说明  : FREQ_VOTE_F2 寄存器结构定义。地址偏移量:0x238，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_f_channel4 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_f2_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_f_channel5 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_f2_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_F2_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f_channel4_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f_channel4_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f2_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f2_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f_channel5_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f_channel5_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f2_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_F2_freq_vote_f2_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_F3_UNION
 结构说明  : FREQ_VOTE_F3 寄存器结构定义。地址偏移量:0x23C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_f_channel6 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_f3_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_f_channel7 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_f3_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_F3_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f_channel6_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f_channel6_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f3_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f3_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f_channel7_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f_channel7_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f3_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_F3_freq_vote_f3_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_G0_UNION
 结构说明  : FREQ_VOTE_G0 寄存器结构定义。地址偏移量:0x240，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_g_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_g0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_g_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_g0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_G0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_G0_freq_vote_g0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_G1_UNION
 结构说明  : FREQ_VOTE_G1 寄存器结构定义。地址偏移量:0x244，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_g_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_g1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_g_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_g1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_G1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_G1_freq_vote_g1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_G2_UNION
 结构说明  : FREQ_VOTE_G2 寄存器结构定义。地址偏移量:0x248，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_g_channel4 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_g2_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_g_channel5 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_g2_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_G2_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g_channel4_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g_channel4_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g2_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g2_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g_channel5_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g_channel5_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g2_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_G2_freq_vote_g2_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_G3_UNION
 结构说明  : FREQ_VOTE_G3 寄存器结构定义。地址偏移量:0x24C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_g_channel6 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_g3_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_g_channel7 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_g3_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_G3_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g_channel6_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g_channel6_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g3_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g3_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g_channel7_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g_channel7_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g3_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_G3_freq_vote_g3_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H0_UNION
 结构说明  : FREQ_VOTE_H0 寄存器结构定义。地址偏移量:0x250，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_H0_freq_vote_h0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H1_UNION
 结构说明  : FREQ_VOTE_H1 寄存器结构定义。地址偏移量:0x254，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_H1_freq_vote_h1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H2_UNION
 结构说明  : FREQ_VOTE_H2 寄存器结构定义。地址偏移量:0x258，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel4 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h2_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel5 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h2_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H2_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h_channel4_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h_channel4_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h2_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h2_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h_channel5_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h_channel5_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h2_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_H2_freq_vote_h2_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H3_UNION
 结构说明  : FREQ_VOTE_H3 寄存器结构定义。地址偏移量:0x25C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel6 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h3_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel7 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h3_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H3_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h_channel6_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h_channel6_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h3_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h3_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h_channel7_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h_channel7_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h3_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_H3_freq_vote_h3_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H4_UNION
 结构说明  : FREQ_VOTE_H4 寄存器结构定义。地址偏移量:0x260，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel8 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h4_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel9 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h4_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H4_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h_channel8_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h_channel8_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h4_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h4_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h_channel9_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h_channel9_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h4_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_H4_freq_vote_h4_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H5_UNION
 结构说明  : FREQ_VOTE_H5 寄存器结构定义。地址偏移量:0x264，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel10 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h5_msk0     : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel11 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h5_msk1     : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H5_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h_channel10_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h_channel10_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h5_msk0_START      (15)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h5_msk0_END        (15)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h_channel11_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h_channel11_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h5_msk1_START      (31)
#define SOC_PMCTRL_FREQ_VOTE_H5_freq_vote_h5_msk1_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H6_UNION
 结构说明  : FREQ_VOTE_H6 寄存器结构定义。地址偏移量:0x268，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel12 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h6_msk0     : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel13 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h6_msk1     : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H6_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h_channel12_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h_channel12_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h6_msk0_START      (15)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h6_msk0_END        (15)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h_channel13_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h_channel13_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h6_msk1_START      (31)
#define SOC_PMCTRL_FREQ_VOTE_H6_freq_vote_h6_msk1_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_H7_UNION
 结构说明  : FREQ_VOTE_H7 寄存器结构定义。地址偏移量:0x26C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_h_channel14 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_h7_msk0     : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_h_channel15 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_h7_msk1     : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_H7_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h_channel14_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h_channel14_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h7_msk0_START      (15)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h7_msk0_END        (15)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h_channel15_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h_channel15_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h7_msk1_START      (31)
#define SOC_PMCTRL_FREQ_VOTE_H7_freq_vote_h7_msk1_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I0_UNION
 结构说明  : FREQ_VOTE_I0 寄存器结构定义。地址偏移量:0x270，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel0 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i0_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel1 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i0_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I0_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i_channel0_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i_channel0_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i0_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i0_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i_channel1_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i_channel1_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i0_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_I0_freq_vote_i0_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I1_UNION
 结构说明  : FREQ_VOTE_I1 寄存器结构定义。地址偏移量:0x274，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel2 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i1_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel3 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i1_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I1_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i_channel2_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i_channel2_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i1_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i1_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i_channel3_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i_channel3_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i1_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_I1_freq_vote_i1_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I2_UNION
 结构说明  : FREQ_VOTE_I2 寄存器结构定义。地址偏移量:0x278，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel4 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i2_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel5 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i2_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I2_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i_channel4_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i_channel4_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i2_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i2_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i_channel5_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i_channel5_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i2_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_I2_freq_vote_i2_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I3_UNION
 结构说明  : FREQ_VOTE_I3 寄存器结构定义。地址偏移量:0x27C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel6 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i3_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel7 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i3_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I3_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i_channel6_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i_channel6_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i3_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i3_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i_channel7_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i_channel7_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i3_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_I3_freq_vote_i3_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I4_UNION
 结构说明  : FREQ_VOTE_I4 寄存器结构定义。地址偏移量:0x280，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel8 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i4_msk0    : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel9 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i4_msk1    : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I4_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i_channel8_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i_channel8_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i4_msk0_START     (15)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i4_msk0_END       (15)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i_channel9_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i_channel9_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i4_msk1_START     (31)
#define SOC_PMCTRL_FREQ_VOTE_I4_freq_vote_i4_msk1_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I5_UNION
 结构说明  : FREQ_VOTE_I5 寄存器结构定义。地址偏移量:0x284，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel10 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i5_msk0     : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel11 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i5_msk1     : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I5_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i_channel10_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i_channel10_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i5_msk0_START      (15)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i5_msk0_END        (15)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i_channel11_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i_channel11_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i5_msk1_START      (31)
#define SOC_PMCTRL_FREQ_VOTE_I5_freq_vote_i5_msk1_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I6_UNION
 结构说明  : FREQ_VOTE_I6 寄存器结构定义。地址偏移量:0x288，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel12 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i6_msk0     : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel13 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i6_msk1     : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I6_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i_channel12_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i_channel12_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i6_msk0_START      (15)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i6_msk0_END        (15)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i_channel13_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i_channel13_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i6_msk1_START      (31)
#define SOC_PMCTRL_FREQ_VOTE_I6_freq_vote_i6_msk1_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_FREQ_VOTE_I7_UNION
 结构说明  : FREQ_VOTE_I7 寄存器结构定义。地址偏移量:0x28C，初值:0x00000000，宽度:32
 寄存器说明: 频率投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_channel14 : 15; /* bit[0-14] :  */
        unsigned int  freq_vote_i7_msk0     : 1;  /* bit[15]   : 写mask bit，分别mask bit[14:0] */
        unsigned int  freq_vote_i_channel15 : 15; /* bit[16-30]:  */
        unsigned int  freq_vote_i7_msk1     : 1;  /* bit[31]   : 写mask bit，分别mask bit[30:16] */
    } reg;
} SOC_PMCTRL_FREQ_VOTE_I7_UNION;
#endif
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i_channel14_START  (0)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i_channel14_END    (14)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i7_msk0_START      (15)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i7_msk0_END        (15)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i_channel15_START  (16)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i_channel15_END    (30)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i7_msk1_START      (31)
#define SOC_PMCTRL_FREQ_VOTE_I7_freq_vote_i7_msk1_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_CHANNEL_CFG_UNION
 结构说明  : VOTE_CHANNEL_CFG 寄存器结构定义。地址偏移量:0x2B0，初值:0x0003EA55，宽度:32
 寄存器说明: 投票通道数配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_channel_num_a : 2;  /* bit[0-1]  : 频率投票通道数目：
                                                                   2'b00: 2通道； 2'b01: 4通道；
                                                                   2'b10: 8通道； 2'b11: 16通道。 */
        unsigned int  freq_vote_channel_num_b : 2;  /* bit[2-3]  :  */
        unsigned int  freq_vote_channel_num_c : 2;  /* bit[4-5]  :  */
        unsigned int  freq_vote_channel_num_d : 2;  /* bit[6-7]  :  */
        unsigned int  freq_vote_channel_num_e : 2;  /* bit[8-9]  :  */
        unsigned int  freq_vote_channel_num_f : 2;  /* bit[10-11]:  */
        unsigned int  freq_vote_channel_num_g : 2;  /* bit[12-13]:  */
        unsigned int  freq_vote_channel_num_h : 2;  /* bit[14-15]:  */
        unsigned int  freq_vote_channel_num_i : 2;  /* bit[16-17]:  */
        unsigned int  reserved                : 14; /* bit[18-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_CHANNEL_CFG_UNION;
#endif
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_a_START  (0)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_a_END    (1)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_b_START  (2)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_b_END    (3)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_c_START  (4)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_c_END    (5)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_d_START  (6)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_d_END    (7)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_e_START  (8)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_e_END    (9)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_f_START  (10)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_f_END    (11)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_g_START  (12)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_g_END    (13)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_h_START  (14)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_h_END    (15)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_i_START  (16)
#define SOC_PMCTRL_VOTE_CHANNEL_CFG_freq_vote_channel_num_i_END    (17)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_MODE_CFG_0_UNION
 结构说明  : VOTE_MODE_CFG_0 寄存器结构定义。地址偏移量:0x2BC，初值:0x00000000，宽度:32
 寄存器说明: 投票类型配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_a_type    : 2;  /* bit[0-1]  : 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               A：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_b_type    : 2;  /* bit[2-3]  : 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               B：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_c_type    : 2;  /* bit[4-5]  : 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               C：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_d_type    : 2;  /* bit[6-7]  : 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               D：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_e_type    : 2;  /* bit[8-9]  : 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               E：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_f_type    : 2;  /* bit[10-11]: 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               F：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_g_type    : 2;  /* bit[12-13]: 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               G：不支持求sum，只支持求min和max */
        unsigned int  freq_vote_h_type    : 2;  /* bit[14-15]: 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               H：支持求min、max和sum */
        unsigned int  vote_mode_cfg_0_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_VOTE_MODE_CFG_0_UNION;
#endif
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_a_type_START     (0)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_a_type_END       (1)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_b_type_START     (2)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_b_type_END       (3)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_c_type_START     (4)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_c_type_END       (5)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_d_type_START     (6)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_d_type_END       (7)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_e_type_START     (8)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_e_type_END       (9)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_f_type_START     (10)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_f_type_END       (11)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_g_type_START     (12)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_g_type_END       (13)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_h_type_START     (14)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_freq_vote_h_type_END       (15)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_vote_mode_cfg_0_msk_START  (16)
#define SOC_PMCTRL_VOTE_MODE_CFG_0_vote_mode_cfg_0_msk_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_MODE_CFG_1_UNION
 结构说明  : VOTE_MODE_CFG_1 寄存器结构定义。地址偏移量:0x2C0，初值:0x00000000，宽度:32
 寄存器说明: 投票类型配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_i_type    : 2;  /* bit[0-1]  : 投票类型配置：2'b1：求max;2'b0：求min, 2'b1x:求sum。
                                                               I：支持求min、max和sum */
        unsigned int  reserved_0          : 2;  /* bit[2-3]  :  */
        unsigned int  reserved_1          : 2;  /* bit[4-5]  :  */
        unsigned int  reserved_2          : 2;  /* bit[6-7]  :  */
        unsigned int  reserved_3          : 2;  /* bit[8-9]  :  */
        unsigned int  reserved_4          : 2;  /* bit[10-11]:  */
        unsigned int  reserved_5          : 2;  /* bit[12-13]:  */
        unsigned int  reserved_6          : 2;  /* bit[14-15]:  */
        unsigned int  vote_mode_cfg_1_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_VOTE_MODE_CFG_1_UNION;
#endif
#define SOC_PMCTRL_VOTE_MODE_CFG_1_freq_vote_i_type_START     (0)
#define SOC_PMCTRL_VOTE_MODE_CFG_1_freq_vote_i_type_END       (1)
#define SOC_PMCTRL_VOTE_MODE_CFG_1_vote_mode_cfg_1_msk_START  (16)
#define SOC_PMCTRL_VOTE_MODE_CFG_1_vote_mode_cfg_1_msk_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_START_EN_UNION
 结构说明  : VOTE_START_EN 寄存器结构定义。地址偏移量:0x2C4，初值:0x00000000，宽度:32
 寄存器说明: 投票使能配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_a_start_en : 1;  /* bit[0]    : 频率投票功能使能（防止初始化时，误触发中断，初始化后票配置为1'b1）。 */
        unsigned int  freq_vote_b_start_en : 1;  /* bit[1]    :  */
        unsigned int  freq_vote_c_start_en : 1;  /* bit[2]    :  */
        unsigned int  freq_vote_d_start_en : 1;  /* bit[3]    :  */
        unsigned int  freq_vote_e_start_en : 1;  /* bit[4]    :  */
        unsigned int  freq_vote_f_start_en : 1;  /* bit[5]    :  */
        unsigned int  freq_vote_g_start_en : 1;  /* bit[6]    :  */
        unsigned int  freq_vote_h_start_en : 1;  /* bit[7]    :  */
        unsigned int  freq_vote_i_start_en : 1;  /* bit[8]    :  */
        unsigned int  reserved             : 7;  /* bit[9-15] :  */
        unsigned int  vote_start_en_msk    : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_PMCTRL_VOTE_START_EN_UNION;
#endif
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_a_start_en_START  (0)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_a_start_en_END    (0)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_b_start_en_START  (1)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_b_start_en_END    (1)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_c_start_en_START  (2)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_c_start_en_END    (2)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_d_start_en_START  (3)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_d_start_en_END    (3)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_e_start_en_START  (4)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_e_start_en_END    (4)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_f_start_en_START  (5)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_f_start_en_END    (5)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_g_start_en_START  (6)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_g_start_en_END    (6)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_h_start_en_START  (7)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_h_start_en_END    (7)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_i_start_en_START  (8)
#define SOC_PMCTRL_VOTE_START_EN_freq_vote_i_start_en_END    (8)
#define SOC_PMCTRL_VOTE_START_EN_vote_start_en_msk_START     (16)
#define SOC_PMCTRL_VOTE_START_EN_vote_start_en_msk_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INT_VOTE_EN_UNION
 结构说明  : INT_VOTE_EN 寄存器结构定义。地址偏移量:0x2C8，初值:0x00000000，宽度:32
 寄存器说明: 投票中断使能配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_freq_vote_en_a : 1;  /* bit[0]    : 频率投票中断使能。 */
        unsigned int  int_freq_vote_en_b : 1;  /* bit[1]    :  */
        unsigned int  int_freq_vote_en_c : 1;  /* bit[2]    :  */
        unsigned int  int_freq_vote_en_d : 1;  /* bit[3]    :  */
        unsigned int  int_freq_vote_en_e : 1;  /* bit[4]    :  */
        unsigned int  int_freq_vote_en_f : 1;  /* bit[5]    :  */
        unsigned int  int_freq_vote_en_g : 1;  /* bit[6]    :  */
        unsigned int  int_freq_vote_en_h : 1;  /* bit[7]    :  */
        unsigned int  int_freq_vote_en_i : 1;  /* bit[8]    :  */
        unsigned int  reserved           : 7;  /* bit[9-15] :  */
        unsigned int  int_vote_en_msk    : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_PMCTRL_INT_VOTE_EN_UNION;
#endif
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_a_START  (0)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_a_END    (0)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_b_START  (1)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_b_END    (1)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_c_START  (2)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_c_END    (2)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_d_START  (3)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_d_END    (3)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_e_START  (4)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_e_END    (4)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_f_START  (5)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_f_END    (5)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_g_START  (6)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_g_END    (6)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_h_START  (7)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_h_END    (7)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_i_START  (8)
#define SOC_PMCTRL_INT_VOTE_EN_int_freq_vote_en_i_END    (8)
#define SOC_PMCTRL_INT_VOTE_EN_int_vote_en_msk_START     (16)
#define SOC_PMCTRL_INT_VOTE_EN_int_vote_en_msk_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INT_VOTE_CLR_UNION
 结构说明  : INT_VOTE_CLR 寄存器结构定义。地址偏移量:0x2CC，初值:0x00000000，宽度:32
 寄存器说明: 投票中断清除配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_freq_vote_a_clr : 1;  /* bit[0]    : 频率投票中断清除。 */
        unsigned int  int_freq_vote_b_clr : 1;  /* bit[1]    :  */
        unsigned int  int_freq_vote_c_clr : 1;  /* bit[2]    :  */
        unsigned int  int_freq_vote_d_clr : 1;  /* bit[3]    :  */
        unsigned int  int_freq_vote_e_clr : 1;  /* bit[4]    :  */
        unsigned int  int_freq_vote_f_clr : 1;  /* bit[5]    :  */
        unsigned int  int_freq_vote_g_clr : 1;  /* bit[6]    :  */
        unsigned int  int_freq_vote_h_clr : 1;  /* bit[7]    :  */
        unsigned int  int_freq_vote_i_clr : 1;  /* bit[8]    :  */
        unsigned int  reserved_0          : 7;  /* bit[9-15] :  */
        unsigned int  reserved_1          : 16; /* bit[16-31]:  */
    } reg;
} SOC_PMCTRL_INT_VOTE_CLR_UNION;
#endif
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_a_clr_START  (0)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_a_clr_END    (0)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_b_clr_START  (1)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_b_clr_END    (1)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_c_clr_START  (2)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_c_clr_END    (2)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_d_clr_START  (3)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_d_clr_END    (3)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_e_clr_START  (4)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_e_clr_END    (4)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_f_clr_START  (5)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_f_clr_END    (5)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_g_clr_START  (6)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_g_clr_END    (6)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_h_clr_START  (7)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_h_clr_END    (7)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_i_clr_START  (8)
#define SOC_PMCTRL_INT_VOTE_CLR_int_freq_vote_i_clr_END    (8)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INT_VOTE_RAW_STAT_UNION
 结构说明  : INT_VOTE_RAW_STAT 寄存器结构定义。地址偏移量:0x2D0，初值:0x00000000，宽度:32
 寄存器说明: 投票中断原始状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_freq_vote_raw_stat_a : 1;  /* bit[0]    : 频率投票原始中断状态。 */
        unsigned int  int_freq_vote_raw_stat_b : 1;  /* bit[1]    :  */
        unsigned int  int_freq_vote_raw_stat_c : 1;  /* bit[2]    :  */
        unsigned int  int_freq_vote_raw_stat_d : 1;  /* bit[3]    :  */
        unsigned int  int_freq_vote_raw_stat_e : 1;  /* bit[4]    :  */
        unsigned int  int_freq_vote_raw_stat_f : 1;  /* bit[5]    :  */
        unsigned int  int_freq_vote_raw_stat_g : 1;  /* bit[6]    :  */
        unsigned int  int_freq_vote_raw_stat_h : 1;  /* bit[7]    :  */
        unsigned int  int_freq_vote_raw_stat_i : 1;  /* bit[8]    :  */
        unsigned int  reserved_0               : 7;  /* bit[9-15] :  */
        unsigned int  reserved_1               : 16; /* bit[16-31]:  */
    } reg;
} SOC_PMCTRL_INT_VOTE_RAW_STAT_UNION;
#endif
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_a_START  (0)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_a_END    (0)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_b_START  (1)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_b_END    (1)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_c_START  (2)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_c_END    (2)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_d_START  (3)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_d_END    (3)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_e_START  (4)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_e_END    (4)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_f_START  (5)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_f_END    (5)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_g_START  (6)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_g_END    (6)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_h_START  (7)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_h_END    (7)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_i_START  (8)
#define SOC_PMCTRL_INT_VOTE_RAW_STAT_int_freq_vote_raw_stat_i_END    (8)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INT_VOTE_STAT_UNION
 结构说明  : INT_VOTE_STAT 寄存器结构定义。地址偏移量:0x2D4，初值:0x00000000，宽度:32
 寄存器说明: 投票中断最终状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_freq_vote_stat_a : 1;  /* bit[0]    : 频率投票中断状态。 */
        unsigned int  int_freq_vote_stat_b : 1;  /* bit[1]    :  */
        unsigned int  int_freq_vote_stat_c : 1;  /* bit[2]    :  */
        unsigned int  int_freq_vote_stat_d : 1;  /* bit[3]    :  */
        unsigned int  int_freq_vote_stat_e : 1;  /* bit[4]    :  */
        unsigned int  int_freq_vote_stat_f : 1;  /* bit[5]    :  */
        unsigned int  int_freq_vote_stat_g : 1;  /* bit[6]    :  */
        unsigned int  int_freq_vote_stat_h : 1;  /* bit[7]    :  */
        unsigned int  int_freq_vote_stat_i : 1;  /* bit[8]    :  */
        unsigned int  reserved_0           : 7;  /* bit[9-15] :  */
        unsigned int  reserved_1           : 16; /* bit[16-31]:  */
    } reg;
} SOC_PMCTRL_INT_VOTE_STAT_UNION;
#endif
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_a_START  (0)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_a_END    (0)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_b_START  (1)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_b_END    (1)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_c_START  (2)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_c_END    (2)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_d_START  (3)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_d_END    (3)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_e_START  (4)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_e_END    (4)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_f_START  (5)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_f_END    (5)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_g_START  (6)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_g_END    (6)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_h_START  (7)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_h_END    (7)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_i_START  (8)
#define SOC_PMCTRL_INT_VOTE_STAT_int_freq_vote_stat_i_END    (8)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INT_FREQUENCY_VOTE_UNION
 结构说明  : INT_FREQUENCY_VOTE 寄存器结构定义。地址偏移量:0x2D8，初值:0x00000000，宽度:32
 寄存器说明: 投票中断寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_frequency_vote : 1;  /* bit[0]   : 送给LPM3的投票中断。 */
        unsigned int  reserved           : 31; /* bit[1-31]:  */
    } reg;
} SOC_PMCTRL_INT_FREQUENCY_VOTE_UNION;
#endif
#define SOC_PMCTRL_INT_FREQUENCY_VOTE_int_frequency_vote_START  (0)
#define SOC_PMCTRL_INT_FREQUENCY_VOTE_int_frequency_vote_END    (0)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_A_UNION
 结构说明  : VOTE_RESULT_A 寄存器结构定义。地址偏移量:0x2DC，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_a : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_A_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_A_freq_vote_result_a_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_A_freq_vote_result_a_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_B_UNION
 结构说明  : VOTE_RESULT_B 寄存器结构定义。地址偏移量:0x2E0，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_b : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_B_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_B_freq_vote_result_b_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_B_freq_vote_result_b_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_C_UNION
 结构说明  : VOTE_RESULT_C 寄存器结构定义。地址偏移量:0x2E4，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_c : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_C_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_C_freq_vote_result_c_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_C_freq_vote_result_c_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_D_UNION
 结构说明  : VOTE_RESULT_D 寄存器结构定义。地址偏移量:0x2E8，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_d : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_D_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_D_freq_vote_result_d_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_D_freq_vote_result_d_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_E_UNION
 结构说明  : VOTE_RESULT_E 寄存器结构定义。地址偏移量:0x2EC，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_e : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_E_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_E_freq_vote_result_e_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_E_freq_vote_result_e_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_F_UNION
 结构说明  : VOTE_RESULT_F 寄存器结构定义。地址偏移量:0x2F0，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_f : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_F_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_F_freq_vote_result_f_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_F_freq_vote_result_f_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_G_UNION
 结构说明  : VOTE_RESULT_G 寄存器结构定义。地址偏移量:0x2F4，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_g : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_G_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_G_freq_vote_result_g_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_G_freq_vote_result_g_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_H_UNION
 结构说明  : VOTE_RESULT_H 寄存器结构定义。地址偏移量:0x2F8，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_h : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_H_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_H_freq_vote_result_h_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_H_freq_vote_result_h_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VOTE_RESULT_I_UNION
 结构说明  : VOTE_RESULT_I 寄存器结构定义。地址偏移量:0x2FC，初值:0x00000000，宽度:32
 寄存器说明: 投票结果回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  freq_vote_result_i : 20; /* bit[0-19] : 最终投票频率 */
        unsigned int  reserved           : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_VOTE_RESULT_I_UNION;
#endif
#define SOC_PMCTRL_VOTE_RESULT_I_freq_vote_result_i_START  (0)
#define SOC_PMCTRL_VOTE_RESULT_I_freq_vote_result_i_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_DDRLPCTRL_UNION
 结构说明  : DDRLPCTRL 寄存器结构定义。地址偏移量:0x30C，初值:0x00000F1F，宽度:32
 寄存器说明: DDRC LP控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ddrc_csysreq_cfg  : 4;  /* bit[0-3]  : 请求DDRC A通道进入低功耗模式。
                                                             0：请求DDRC进入低功耗模式；
                                                             1：请求DDRC退出低功耗模式。 */
        unsigned int  ddrc_csysack      : 4;  /* bit[4-7]  : DDRC A通道对于请求的响应。
                                                             0：对于请求DDRC进入低功耗模式的响应；
                                                             1：对于请求DDRC退出低功耗模式的响应。 */
        unsigned int  ddrc_csysreq_stat : 4;  /* bit[8-11] : 请求DDRC A通道进入低功耗模式。
                                                             0：请求DDRC进入低功耗模式；
                                                             1：请求DDRC退出低功耗模式。 */
        unsigned int  reserved          : 20; /* bit[12-31]: PMC_MISC生成逻辑，注意适配修改 */
    } reg;
} SOC_PMCTRL_DDRLPCTRL_UNION;
#endif
#define SOC_PMCTRL_DDRLPCTRL_ddrc_csysreq_cfg_START   (0)
#define SOC_PMCTRL_DDRLPCTRL_ddrc_csysreq_cfg_END     (3)
#define SOC_PMCTRL_DDRLPCTRL_ddrc_csysack_START       (4)
#define SOC_PMCTRL_DDRLPCTRL_ddrc_csysack_END         (7)
#define SOC_PMCTRL_DDRLPCTRL_ddrc_csysreq_stat_START  (8)
#define SOC_PMCTRL_DDRLPCTRL_ddrc_csysreq_stat_END    (11)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL0_UNION
 结构说明  : PERI_CTRL0 寄存器结构定义。地址偏移量:0x340，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_ctrl_core_crg : 1;  /* bit[0-0] :  */
        unsigned int  dbg_ctrl_cssys    : 1;  /* bit[1-1] :  */
        unsigned int  reserved_0        : 1;  /* bit[2-2] :  */
        unsigned int  reserved_1        : 1;  /* bit[3-3] :  */
        unsigned int  reserved_2        : 1;  /* bit[4-4] :  */
        unsigned int  reserved_3        : 1;  /* bit[5-5] :  */
        unsigned int  dbg_ctrl_pmc      : 1;  /* bit[6-6] :  */
        unsigned int  dbg_ctrl_crg      : 1;  /* bit[7-7] :  */
        unsigned int  reserved_4        : 24; /* bit[8-31]:  */
    } reg;
} SOC_PMCTRL_PERI_CTRL0_UNION;
#endif
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_core_crg_START  (0)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_core_crg_END    (0)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_cssys_START     (1)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_cssys_END       (1)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_pmc_START       (6)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_pmc_END         (6)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_crg_START       (7)
#define SOC_PMCTRL_PERI_CTRL0_dbg_ctrl_crg_END         (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL1_UNION
 结构说明  : PERI_CTRL1 寄存器结构定义。地址偏移量:0x344，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 10; /* bit[0-9]  : 保留。 */
        unsigned int  reserved_1: 6;  /* bit[10-15]: 保留。 */
        unsigned int  reserved_2: 10; /* bit[16-25]: 保留。 */
        unsigned int  reserved_3: 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL2_UNION
 结构说明  : PERI_CTRL2 寄存器结构定义。地址偏移量:0x348，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cluster0_vol_addr : 10; /* bit[0-9]  : PMUSSI配置地址，对应CLUSTER0调压操作。
                                                             PMUSSI通道0。(PHX不使用) */
        unsigned int  reserved_0        : 6;  /* bit[10-15]: 保留。 */
        unsigned int  reserved_1        : 10; /* bit[16-25]: 保留。 */
        unsigned int  reserved_2        : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL2_UNION;
#endif
#define SOC_PMCTRL_PERI_CTRL2_cluster0_vol_addr_START  (0)
#define SOC_PMCTRL_PERI_CTRL2_cluster0_vol_addr_END    (9)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL3_UNION
 结构说明  : PERI_CTRL3 寄存器结构定义。地址偏移量:0x34C，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  g3d_mem_vol_addr : 10; /* bit[0-9]  : PMUSSI配置地址，对应G3D MEM调压操作。
                                                            PMUSSI通道1。(PHX不使用) */
        unsigned int  reserved_0       : 6;  /* bit[10-15]: 保留。 */
        unsigned int  peri_vol_addr    : 10; /* bit[16-25]: PMUSSI配置地址，对应PERI调压操作。
                                                            PMUSSI通道2。(PHX不使用) */
        unsigned int  reserved_1       : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL3_UNION;
#endif
#define SOC_PMCTRL_PERI_CTRL3_g3d_mem_vol_addr_START  (0)
#define SOC_PMCTRL_PERI_CTRL3_g3d_mem_vol_addr_END    (9)
#define SOC_PMCTRL_PERI_CTRL3_peri_vol_addr_START     (16)
#define SOC_PMCTRL_PERI_CTRL3_peri_vol_addr_END       (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL4_UNION
 结构说明  : PERI_CTRL4 寄存器结构定义。地址偏移量:0x350，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 16; /* bit[0-15] : 外设区降压投票寄存器，软件专用。 */
        unsigned int  reserved_1: 16; /* bit[16-31]: 外设区降压投票寄存器，软件专用。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL5_UNION
 结构说明  : PERI_CTRL5 寄存器结构定义。地址偏移量:0x354，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 外设区降压投票寄存器，软件专用。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL5_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL6_UNION
 结构说明  : PERI_CTRL6 寄存器结构定义。地址偏移量:0x358，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 2;  /* bit[0-1] :  */
        unsigned int  reserved_1: 2;  /* bit[2-3] :  */
        unsigned int  reserved_2: 3;  /* bit[4-6] :  */
        unsigned int  reserved_3: 1;  /* bit[7-7] :  */
        unsigned int  reserved_4: 24; /* bit[8-31]:  */
    } reg;
} SOC_PMCTRL_PERI_CTRL6_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_CTRL7_UNION
 结构说明  : PERI_CTRL7 寄存器结构定义。地址偏移量:0x35C，初值:0x00000000，宽度:32
 寄存器说明: 外设通用控制寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ispdss2ddr_dfs_ongoing : 1;  /* bit[0-0] : 1'b0:dfs not start
                                                                 1'b1:dfs ongoing */
        unsigned int  reserved               : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL7_UNION;
#endif
#define SOC_PMCTRL_PERI_CTRL7_ispdss2ddr_dfs_ongoing_START  (0)
#define SOC_PMCTRL_PERI_CTRL7_ispdss2ddr_dfs_ongoing_END    (0)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT0_UNION
 结构说明  : PERI_STAT0 寄存器结构定义。地址偏移量:0x360，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_STAT0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT1_UNION
 结构说明  : PERI_STAT1 寄存器结构定义。地址偏移量:0x364，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_info_cssys : 32; /* bit[0-31]: debug状态。 */
    } reg;
} SOC_PMCTRL_PERI_STAT1_UNION;
#endif
#define SOC_PMCTRL_PERI_STAT1_dbg_info_cssys_START  (0)
#define SOC_PMCTRL_PERI_STAT1_dbg_info_cssys_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT2_UNION
 结构说明  : PERI_STAT2 寄存器结构定义。地址偏移量:0x368，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_STAT2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT3_UNION
 结构说明  : PERI_STAT3 寄存器结构定义。地址偏移量:0x36C，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_STAT3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT4_UNION
 结构说明  : PERI_STAT4 寄存器结构定义。地址偏移量:0x370，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_STAT4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLEREQ_0_UNION
 结构说明  : NOC_POWER_IDLEREQ_0 寄存器结构定义。地址偏移量:0x380，初值:0x0000FE46，宽度:32
 寄存器说明: NOC低功耗请求信号寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ddrphy_bypass_mode            : 1;  /* bit[0-0]  : DDRPHY工作模式。
                                                                         0：非bypass模式；
                                                                         1：bypass模式。 */
        unsigned int  noc_ivp_power_idlereq         : 1;  /* bit[1-1]  : noc低功耗请求信号。
                                                                         0：请求noc退出低功耗模式；
                                                                         1：请求noc进入低功耗模式。 */
        unsigned int  noc_vcodec_power_idlereq      : 1;  /* bit[2-2]  : 同bit[1]。 */
        unsigned int  noc_cfgbus_power_idlereq      : 1;  /* bit[3-3]  : 同bit[1]。 */
        unsigned int  reserved_0                    : 1;  /* bit[4-4]  :  */
        unsigned int  noc_hsdt_power_idlereq        : 1;  /* bit[5-5]  : 同bit[1]。 */
        unsigned int  noc_usb_power_idlereq         : 1;  /* bit[6-6]  : 同bit[1]。 */
        unsigned int  noc_dmadebugbus_power_idlereq : 1;  /* bit[7-7]  : 同bit[1]。 */
        unsigned int  noc_modem_power_idlereq       : 1;  /* bit[8-8]  : 同bit[1]。 */
        unsigned int  reserved_1                    : 1;  /* bit[9-9]  : 同bit[1]。 */
        unsigned int  noc_pcie0_power_idlereq       : 1;  /* bit[10-10]: 同bit[1]。 */
        unsigned int  noc_tcu_power_idlereq         : 1;  /* bit[11-11]: 同bit[1]。 */
        unsigned int  noc_isp_power_idlereq         : 1;  /* bit[12-12]: 同bit[1]。 */
        unsigned int  noc_dss_power_idlereq         : 1;  /* bit[13-13]: 同bit[1]。 */
        unsigned int  noc_vivo_power_idlereq        : 1;  /* bit[14-14]: 同bit[1]。 */
        unsigned int  noc_vdec_power_idlereq        : 1;  /* bit[15-15]: 同bit[1]。 */
        unsigned int  biten                         : 16; /* bit[16-31]: 每个比特位的使能位。
                                                                         只有当biten对应的比特位为1'b1，相应的比特位才起作用。biten[0]就是bit 0的使能位。写1有效。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLEREQ_0_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_ddrphy_bypass_mode_START             (0)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_ddrphy_bypass_mode_END               (0)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_ivp_power_idlereq_START          (1)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_ivp_power_idlereq_END            (1)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_vcodec_power_idlereq_START       (2)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_vcodec_power_idlereq_END         (2)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_cfgbus_power_idlereq_START       (3)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_cfgbus_power_idlereq_END         (3)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_hsdt_power_idlereq_START         (5)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_hsdt_power_idlereq_END           (5)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_usb_power_idlereq_START          (6)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_usb_power_idlereq_END            (6)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_dmadebugbus_power_idlereq_START  (7)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_dmadebugbus_power_idlereq_END    (7)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_modem_power_idlereq_START        (8)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_modem_power_idlereq_END          (8)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_pcie0_power_idlereq_START        (10)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_pcie0_power_idlereq_END          (10)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_tcu_power_idlereq_START          (11)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_tcu_power_idlereq_END            (11)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_isp_power_idlereq_START          (12)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_isp_power_idlereq_END            (12)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_dss_power_idlereq_START          (13)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_dss_power_idlereq_END            (13)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_vivo_power_idlereq_START         (14)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_vivo_power_idlereq_END           (14)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_vdec_power_idlereq_START         (15)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_noc_vdec_power_idlereq_END           (15)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_biten_START                          (16)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_0_biten_END                            (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLEACK_0_UNION
 结构说明  : NOC_POWER_IDLEACK_0 寄存器结构定义。地址偏移量:0x384，初值:0x00000000，宽度:32
 寄存器说明: NOC低功耗响应寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                    : 1;  /* bit[0-0]  : 保留。 */
        unsigned int  noc_ivp_power_idleack         : 1;  /* bit[1-1]  : 同bit[1]。 */
        unsigned int  noc_vcodec_power_idleack      : 1;  /* bit[2-2]  : 同bit[1]。 */
        unsigned int  noc_cfgbus_power_idleack      : 1;  /* bit[3-3]  : 同bit[1]。 */
        unsigned int  reserved_1                    : 1;  /* bit[4-4]  :  */
        unsigned int  noc_hsdt_power_idleack        : 1;  /* bit[5-5]  : 同bit[1]。 */
        unsigned int  noc_usb_power_idleack         : 1;  /* bit[6-6]  : 同bit[1]。 */
        unsigned int  noc_dmadebugbus_power_idleack : 1;  /* bit[7-7]  : 同bit[1]。 */
        unsigned int  noc_modem_power_idleack       : 1;  /* bit[8-8]  : 同bit[1]。 */
        unsigned int  reserved_2                    : 1;  /* bit[9-9]  : 同bit[1]。 */
        unsigned int  noc_pcie0_power_idleack       : 1;  /* bit[10-10]: 同bit[1]。 */
        unsigned int  noc_tcu_power_idleack         : 1;  /* bit[11-11]: 同bit[1]。 */
        unsigned int  noc_isp_power_idleack         : 1;  /* bit[12-12]: 同bit[1]。 */
        unsigned int  noc_dss_power_idleack         : 1;  /* bit[13-13]: 同bit[1]。 */
        unsigned int  noc_vivo_power_idleack        : 1;  /* bit[14-14]: 同bit[1]。 */
        unsigned int  noc_vdec_power_idleack        : 1;  /* bit[15-15]: 同bit[1]。 */
        unsigned int  reserved_3                    : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLEACK_0_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_ivp_power_idleack_START          (1)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_ivp_power_idleack_END            (1)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_vcodec_power_idleack_START       (2)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_vcodec_power_idleack_END         (2)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_cfgbus_power_idleack_START       (3)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_cfgbus_power_idleack_END         (3)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_hsdt_power_idleack_START         (5)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_hsdt_power_idleack_END           (5)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_usb_power_idleack_START          (6)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_usb_power_idleack_END            (6)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_dmadebugbus_power_idleack_START  (7)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_dmadebugbus_power_idleack_END    (7)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_modem_power_idleack_START        (8)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_modem_power_idleack_END          (8)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_pcie0_power_idleack_START        (10)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_pcie0_power_idleack_END          (10)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_tcu_power_idleack_START          (11)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_tcu_power_idleack_END            (11)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_isp_power_idleack_START          (12)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_isp_power_idleack_END            (12)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_dss_power_idleack_START          (13)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_dss_power_idleack_END            (13)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_vivo_power_idleack_START         (14)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_vivo_power_idleack_END           (14)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_vdec_power_idleack_START         (15)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_0_noc_vdec_power_idleack_END           (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLE_0_UNION
 结构说明  : NOC_POWER_IDLE_0 寄存器结构定义。地址偏移量:0x388，初值:0x00000000，宽度:32
 寄存器说明: NOC低功耗状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                 : 1;  /* bit[0-0]  : 保留。 */
        unsigned int  noc_ivp_power_idle         : 1;  /* bit[1-1]  : noc进入低功耗模式的状态指示。
                                                                      0：对于请求noc退出低功耗模式的状态指示；
                                                                      1：对于请求noc进入低功耗模式的状态指示。 */
        unsigned int  noc_vcodec_power_idle      : 1;  /* bit[2-2]  : 同bit[1]。 */
        unsigned int  noc_cfgbus_power_idle      : 1;  /* bit[3-3]  : 同bit[1]。 */
        unsigned int  reserved_1                 : 1;  /* bit[4-4]  :  */
        unsigned int  noc_hsdt_power_idle        : 1;  /* bit[5-5]  : 同bit[1]。 */
        unsigned int  noc_usb_power_idle         : 1;  /* bit[6-6]  : 同bit[1]。 */
        unsigned int  noc_dmadebugbus_power_idle : 1;  /* bit[7-7]  : 同bit[1]。 */
        unsigned int  noc_modem_power_idle       : 1;  /* bit[8-8]  : 同bit[1]。 */
        unsigned int  reserved_2                 : 1;  /* bit[9-9]  : 同bit[1]。 */
        unsigned int  noc_pcie0_power_idle       : 1;  /* bit[10-10]: 同bit[1]。 */
        unsigned int  noc_tcu_power_idle         : 1;  /* bit[11-11]: 同bit[1]。 */
        unsigned int  noc_isp_power_idle         : 1;  /* bit[12-12]: 同bit[1]。 */
        unsigned int  noc_dss_power_idle         : 1;  /* bit[13-13]: 同bit[1]。 */
        unsigned int  noc_vivo_power_idle        : 1;  /* bit[14-14]: 同bit[1]。 */
        unsigned int  noc_vdec_power_idle        : 1;  /* bit[15-15]: 同bit[1]。 */
        unsigned int  reserved_3                 : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLE_0_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_ivp_power_idle_START          (1)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_ivp_power_idle_END            (1)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_vcodec_power_idle_START       (2)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_vcodec_power_idle_END         (2)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_cfgbus_power_idle_START       (3)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_cfgbus_power_idle_END         (3)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_hsdt_power_idle_START         (5)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_hsdt_power_idle_END           (5)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_usb_power_idle_START          (6)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_usb_power_idle_END            (6)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_dmadebugbus_power_idle_START  (7)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_dmadebugbus_power_idle_END    (7)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_modem_power_idle_START        (8)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_modem_power_idle_END          (8)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_pcie0_power_idle_START        (10)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_pcie0_power_idle_END          (10)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_tcu_power_idle_START          (11)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_tcu_power_idle_END            (11)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_isp_power_idle_START          (12)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_isp_power_idle_END            (12)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_dss_power_idle_START          (13)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_dss_power_idle_END            (13)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_vivo_power_idle_START         (14)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_vivo_power_idle_END           (14)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_vdec_power_idle_START         (15)
#define SOC_PMCTRL_NOC_POWER_IDLE_0_noc_vdec_power_idle_END           (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLEREQ_1_UNION
 结构说明  : NOC_POWER_IDLEREQ_1 寄存器结构定义。地址偏移量:0x38C，初值:0x0000FFFF，宽度:32
 寄存器说明: NOC低功耗请求信号寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_venc_power_idlereq     : 1;  /* bit[0-0]  : noc低功耗请求信号。
                                                                      0：请求noc退出低功耗模式；
                                                                      1：请求noc进入低功耗模式。 */
        unsigned int  reserved_0                 : 1;  /* bit[1-1]  : 同bit0。 */
        unsigned int  noc_npubus_power_idlereq   : 1;  /* bit[2-2]  : 同bit0。 */
        unsigned int  noc_aicore_power_idlereq   : 1;  /* bit[3-3]  : 同bit0。 */
        unsigned int  noc_mdm5gbus_power_idlereq : 1;  /* bit[4-4]  : 同bit0。 */
        unsigned int  reserved_1                 : 1;  /* bit[5-5]  : 同bit0。 */
        unsigned int  reserved_2                 : 1;  /* bit[6-6]  : 同bit0。 */
        unsigned int  noc_npucpu_power_idlereq   : 1;  /* bit[7-7]  : 同bit0。 */
        unsigned int  reserved_3                 : 1;  /* bit[8-8]  : 同bit0。 */
        unsigned int  reserved_4                 : 1;  /* bit[9-9]  : 同bit0。 */
        unsigned int  noc_sysdma_power_idlereq   : 1;  /* bit[10-10]: 同bit0。 */
        unsigned int  noc_eps_power_idlereq      : 1;  /* bit[11-11]: 同bit0。 */
        unsigned int  reserved_5                 : 1;  /* bit[12-12]: 同bit0。 */
        unsigned int  reserved_6                 : 1;  /* bit[13-13]: 同bit0。 */
        unsigned int  reserved_7                 : 1;  /* bit[14-14]: 同bit0。 */
        unsigned int  reserved_8                 : 1;  /* bit[15-15]: 同bit0。 */
        unsigned int  biten                      : 16; /* bit[16-31]: 每个比特位的使能位。
                                                                      只有当biten对应的比特位为1'b1，相应的比特位才起作用。biten[0]就是bit 0的使能位。写1有效。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLEREQ_1_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_venc_power_idlereq_START      (0)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_venc_power_idlereq_END        (0)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_npubus_power_idlereq_START    (2)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_npubus_power_idlereq_END      (2)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_aicore_power_idlereq_START    (3)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_aicore_power_idlereq_END      (3)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_mdm5gbus_power_idlereq_START  (4)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_mdm5gbus_power_idlereq_END    (4)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_npucpu_power_idlereq_START    (7)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_npucpu_power_idlereq_END      (7)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_sysdma_power_idlereq_START    (10)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_sysdma_power_idlereq_END      (10)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_eps_power_idlereq_START       (11)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_noc_eps_power_idlereq_END         (11)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_biten_START                       (16)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_1_biten_END                         (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLEACK_1_UNION
 结构说明  : NOC_POWER_IDLEACK_1 寄存器结构定义。地址偏移量:0x390，初值:0x00000000，宽度:32
 寄存器说明: NOC低功耗响应寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_venc_power_idleack     : 1;  /* bit[0-0]  : noc对于低功耗请求的响应。
                                                                      0：对于请求noc退出低功耗模式的响应；
                                                                      1：对于请求noc进入低功耗模式的响应。 */
        unsigned int  reserved_0                 : 1;  /* bit[1-1]  : 同bit0。 */
        unsigned int  noc_npubus_power_idleack   : 1;  /* bit[2-2]  : 同bit0。 */
        unsigned int  noc_aicore_power_idleack   : 1;  /* bit[3-3]  : 同bit0。 */
        unsigned int  noc_mdm5gbus_power_idleack : 1;  /* bit[4-4]  : 同bit0。 */
        unsigned int  reserved_1                 : 1;  /* bit[5-5]  : 同bit0。 */
        unsigned int  reserved_2                 : 1;  /* bit[6-6]  : 同bit0。 */
        unsigned int  noc_npucpu_power_idleack   : 1;  /* bit[7-7]  : 同bit0。 */
        unsigned int  reserved_3                 : 1;  /* bit[8-8]  : 同bit0。 */
        unsigned int  reserved_4                 : 1;  /* bit[9-9]  : 同bit0。 */
        unsigned int  noc_sysdma_power_idleack   : 1;  /* bit[10-10]: 同bit0。 */
        unsigned int  noc_eps_power_idleack      : 1;  /* bit[11-11]: 同bit0。 */
        unsigned int  reserved_5                 : 1;  /* bit[12-12]: 同bit0。 */
        unsigned int  reserved_6                 : 1;  /* bit[13-13]: 同bit0。 */
        unsigned int  reserved_7                 : 1;  /* bit[14-14]: 同bit0。 */
        unsigned int  reserved_8                 : 1;  /* bit[15-15]: 同bit0。 */
        unsigned int  reserved_9                 : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLEACK_1_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_venc_power_idleack_START      (0)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_venc_power_idleack_END        (0)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_npubus_power_idleack_START    (2)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_npubus_power_idleack_END      (2)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_aicore_power_idleack_START    (3)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_aicore_power_idleack_END      (3)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_mdm5gbus_power_idleack_START  (4)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_mdm5gbus_power_idleack_END    (4)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_npucpu_power_idleack_START    (7)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_npucpu_power_idleack_END      (7)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_sysdma_power_idleack_START    (10)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_sysdma_power_idleack_END      (10)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_eps_power_idleack_START       (11)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_1_noc_eps_power_idleack_END         (11)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLE_1_UNION
 结构说明  : NOC_POWER_IDLE_1 寄存器结构定义。地址偏移量:0x394，初值:0x00000000，宽度:32
 寄存器说明: NOC低功耗状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_venc_power_idle     : 1;  /* bit[0-0]  : noc进入低功耗模式的状态指示。
                                                                   0：对于请求noc退出低功耗模式的状态指示；
                                                                   1：对于请求noc进入低功耗模式的状态指示。 */
        unsigned int  reserved_0              : 1;  /* bit[1-1]  : 同bit0。 */
        unsigned int  noc_npubus_power_idle   : 1;  /* bit[2-2]  : 同bit0。 */
        unsigned int  noc_aicore_power_idle   : 1;  /* bit[3-3]  : 同bit0。 */
        unsigned int  noc_mdm5gbus_power_idle : 1;  /* bit[4-4]  : 同bit0。 */
        unsigned int  reserved_1              : 1;  /* bit[5-5]  : 同bit0。 */
        unsigned int  reserved_2              : 1;  /* bit[6-6]  : 同bit0。 */
        unsigned int  noc_npucpu_power_idle   : 1;  /* bit[7-7]  : 同bit0。 */
        unsigned int  reserved_3              : 1;  /* bit[8-8]  : 同bit0。 */
        unsigned int  reserved_4              : 1;  /* bit[9-9]  : 同bit0。 */
        unsigned int  noc_sysdma_power_idle   : 1;  /* bit[10-10]: 同bit0。 */
        unsigned int  noc_eps_power_idle      : 1;  /* bit[11-11]: 同bit0。 */
        unsigned int  reserved_5              : 1;  /* bit[12-12]: 同bit0。 */
        unsigned int  reserved_6              : 1;  /* bit[13-13]: 同bit0。 */
        unsigned int  reserved_7              : 1;  /* bit[14-14]: 同bit0。 */
        unsigned int  reserved_8              : 1;  /* bit[15-15]: 同bit0。 */
        unsigned int  reserved_9              : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLE_1_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_venc_power_idle_START      (0)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_venc_power_idle_END        (0)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_npubus_power_idle_START    (2)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_npubus_power_idle_END      (2)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_aicore_power_idle_START    (3)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_aicore_power_idle_END      (3)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_mdm5gbus_power_idle_START  (4)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_mdm5gbus_power_idle_END    (4)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_npucpu_power_idle_START    (7)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_npucpu_power_idle_END      (7)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_sysdma_power_idle_START    (10)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_sysdma_power_idle_END      (10)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_eps_power_idle_START       (11)
#define SOC_PMCTRL_NOC_POWER_IDLE_1_noc_eps_power_idle_END         (11)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLEREQ_2_UNION
 结构说明  : NOC_POWER_IDLEREQ_2 寄存器结构定义。地址偏移量:0x398，初值:0x000003FF，宽度:32
 寄存器说明: NOC低功耗请求信号寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  : noc低功耗请求信号。
                                                                     0：请求noc退出低功耗模式；
                                                                     1：请求noc进入低功耗模式。 */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  : 同bit0。 */
        unsigned int  reserved_2                : 1;  /* bit[2-2]  : 同bit0。 */
        unsigned int  reserved_3                : 1;  /* bit[3-3]  : 同bit0。 */
        unsigned int  reserved_4                : 1;  /* bit[4-4]  : 同bit0。 */
        unsigned int  reserved_5                : 1;  /* bit[5-5]  : 同bit0。 */
        unsigned int  reserved_6                : 1;  /* bit[6-6]  : 同bit0。 */
        unsigned int  reserved_7                : 1;  /* bit[7-7]  : 同bit0。 */
        unsigned int  reserved_8                : 1;  /* bit[8-8]  : 同bit0。 */
        unsigned int  reserved_9                : 1;  /* bit[9-9]  : 同bit0。 */
        unsigned int  noc_mmc0noc_power_idlereq : 1;  /* bit[10-10]: 同bit0。 */
        unsigned int  reserved_10               : 1;  /* bit[11-11]: 同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[12-12]: 同bit0。 */
        unsigned int  reserved_12               : 1;  /* bit[13-13]: 同bit0。 */
        unsigned int  reserved_13               : 1;  /* bit[14-14]: 同bit0。 */
        unsigned int  reserved_14               : 1;  /* bit[15-15]: 同bit0。 */
        unsigned int  biten                     : 16; /* bit[16-31]: 每个比特位的使能位。
                                                                     只有当biten对应的比特位为1'b1，相应的比特位才起作用。biten[0]就是bit 0的使能位。写1有效。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLEREQ_2_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_2_noc_mmc0noc_power_idlereq_START  (10)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_2_noc_mmc0noc_power_idlereq_END    (10)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_2_biten_START                      (16)
#define SOC_PMCTRL_NOC_POWER_IDLEREQ_2_biten_END                        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLEACK_2_UNION
 结构说明  : NOC_POWER_IDLEACK_2 寄存器结构定义。地址偏移量:0x39C，初值:0x00000000，宽度:32
 寄存器说明: NOC低功耗响应寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  : noc对于低功耗请求的响应。
                                                                     0：对于请求noc退出低功耗模式的响应；
                                                                     1：对于请求noc进入低功耗模式的响应。 */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  : 同bit0。 */
        unsigned int  reserved_2                : 1;  /* bit[2-2]  : 同bit0。 */
        unsigned int  reserved_3                : 1;  /* bit[3-3]  : 同bit0。 */
        unsigned int  reserved_4                : 1;  /* bit[4-4]  : 同bit0。 */
        unsigned int  reserved_5                : 1;  /* bit[5-5]  : 同bit0。 */
        unsigned int  reserved_6                : 1;  /* bit[6-6]  : 同bit0。 */
        unsigned int  reserved_7                : 1;  /* bit[7-7]  : 同bit0。 */
        unsigned int  reserved_8                : 1;  /* bit[8-8]  : 同bit0。 */
        unsigned int  reserved_9                : 1;  /* bit[9-9]  : 同bit0。 */
        unsigned int  noc_mmc0noc_power_idleack : 1;  /* bit[10-10]: 同bit0。 */
        unsigned int  reserved_10               : 1;  /* bit[11-11]: 同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[12-12]: 同bit0。 */
        unsigned int  reserved_12               : 1;  /* bit[13-13]: 同bit0。 */
        unsigned int  reserved_13               : 1;  /* bit[14-14]: 同bit0。 */
        unsigned int  reserved_14               : 1;  /* bit[15-15]: 同bit0。 */
        unsigned int  reserved_15               : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLEACK_2_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLEACK_2_noc_mmc0noc_power_idleack_START  (10)
#define SOC_PMCTRL_NOC_POWER_IDLEACK_2_noc_mmc0noc_power_idleack_END    (10)


/*****************************************************************************
 结构名    : SOC_PMCTRL_NOC_POWER_IDLE_2_UNION
 结构说明  : NOC_POWER_IDLE_2 寄存器结构定义。地址偏移量:0x374，初值:0x00000000，宽度:32
 寄存器说明: NOC低功耗状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0             : 1;  /* bit[0-0]  : noc进入低功耗模式的状态指示。
                                                                  0：对于请求noc退出低功耗模式的状态指示；
                                                                  1：对于请求noc进入低功耗模式的状态指示。 */
        unsigned int  reserved_1             : 1;  /* bit[1-1]  : 同bit0。 */
        unsigned int  reserved_2             : 1;  /* bit[2-2]  : 同bit0。 */
        unsigned int  reserved_3             : 1;  /* bit[3-3]  : 同bit0。 */
        unsigned int  reserved_4             : 1;  /* bit[4-4]  : 同bit0。 */
        unsigned int  reserved_5             : 1;  /* bit[5-5]  : 同bit0。 */
        unsigned int  reserved_6             : 1;  /* bit[6-6]  : 同bit0。 */
        unsigned int  reserved_7             : 1;  /* bit[7-7]  : 同bit0。 */
        unsigned int  reserved_8             : 1;  /* bit[8-8]  : 同bit0。 */
        unsigned int  reserved_9             : 1;  /* bit[9-9]  : 同bit0。 */
        unsigned int  noc_mmc0noc_power_idle : 1;  /* bit[10-10]: 同bit0。 */
        unsigned int  reserved_10            : 1;  /* bit[11-11]: 同bit0。 */
        unsigned int  reserved_11            : 1;  /* bit[12-12]: 同bit0。 */
        unsigned int  reserved_12            : 1;  /* bit[13-13]: 同bit0。 */
        unsigned int  reserved_13            : 1;  /* bit[14-14]: 同bit0。 */
        unsigned int  reserved_14            : 1;  /* bit[15-15]: 同bit0。 */
        unsigned int  reserved_15            : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PMCTRL_NOC_POWER_IDLE_2_UNION;
#endif
#define SOC_PMCTRL_NOC_POWER_IDLE_2_noc_mmc0noc_power_idle_START  (10)
#define SOC_PMCTRL_NOC_POWER_IDLE_2_noc_mmc0noc_power_idle_END    (10)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT0_MASK_UNION
 结构说明  : PERI_INT0_MASK 寄存器结构定义。地址偏移量:0x3A0，初值:0x00000000，宽度:32
 寄存器说明: 外设中断屏蔽寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_npugic_cfg_t_maintimeout        : 1;  /* bit[0] : NOC target端口计数超时屏蔽信号：
                                                                            0：不屏蔽计数超时信号；
                                                                            1：屏蔽计数超时信号。 */
        unsigned int  noc_hkadc_ssi_t_maintimeout         : 1;  /* bit[1] : 同bit0。 */
        unsigned int  noc_gpu_cfg_t_maintimeout           : 1;  /* bit[2] : 同bit0。 */
        unsigned int  noc_lpm3_slv_t_maintimeout          : 1;  /* bit[3] : 同bit0。 */
        unsigned int  noc_sys_peri0_cfg_t_maintimeout     : 1;  /* bit[4] : 同bit0。 */
        unsigned int  noc_sys_peri1_cfg_t_maintimeout     : 1;  /* bit[5] : 同bit0。 */
        unsigned int  noc_sys_peri2_cfg_t_maintimeout     : 1;  /* bit[6] : 同bit0。 */
        unsigned int  noc_sys_peri3_cfg_t_maintimeout     : 1;  /* bit[7] : 同bit0。 */
        unsigned int  noc_fd_cfg_t_maintimeout            : 1;  /* bit[8] : 同bit0。 */
        unsigned int  noc_dmac_cfg_t_maintimeout          : 1;  /* bit[9] : 同bit0。 */
        unsigned int  reserved_0                          : 1;  /* bit[10]: 同bit0。 */
        unsigned int  noc_hisee_cfg_t_maintimeout         : 1;  /* bit[11]: 同bit0。 */
        unsigned int  reserved_1                          : 1;  /* bit[12]: 同bit0。 */
        unsigned int  noc_top_cssys_slv_cfg_t_maintimeout : 1;  /* bit[13]: 同bit0。 */
        unsigned int  noc_ao_tcp_cfg_t_maintimeout        : 1;  /* bit[14]: 同bit0。 */
        unsigned int  noc_modem_cfg_t_maintimeout         : 1;  /* bit[15]: 同bit0。 */
        unsigned int  noc_usb3otg_cfg_t_maintimeout       : 1;  /* bit[16]: 同bit0。 */
        unsigned int  noc_npucpu_cfg_t_maintimeout        : 1;  /* bit[17]: 同bit0。 */
        unsigned int  reserved_2                          : 1;  /* bit[18]: 同bit0。 */
        unsigned int  noc_vcodec_crg_cfg_t_maintimeout    : 1;  /* bit[19]: 同bit0。 */
        unsigned int  reserved_3                          : 1;  /* bit[20]: 同bit0。 */
        unsigned int  reserved_4                          : 1;  /* bit[21]: 同bit0。 */
        unsigned int  noc_pcie0_cfg_t_maintimeout         : 1;  /* bit[22]: 同bit0。 */
        unsigned int  noc_cfg2vcodec_t_maintimeout        : 1;  /* bit[23]: 同bit0。 */
        unsigned int  noc_cfg2vivo_t_maintimeout          : 1;  /* bit[24]: 同bit0。 */
        unsigned int  noc_dmss_apb_slv_t_maintimeout      : 1;  /* bit[25]: 同bit0。 */
        unsigned int  noc_npu2cfgbus_cfg_t_maitimeout     : 1;  /* bit[26]: 同bit0。 */
        unsigned int  noc_asp_cfg_t_maintimeout           : 1;  /* bit[27]: 同bit0。 */
        unsigned int  noc_npubus_cfg_t_maintimeout        : 1;  /* bit[28]: 同bit0。（Baltimore不使用） */
        unsigned int  noc_iomcu_ahb_slv_maintimeout       : 1;  /* bit[29]: 同bit0。 */
        unsigned int  noc_iomcu_apb_slv_maintimeout       : 1;  /* bit[30]: 同bit0。 */
        unsigned int  noc_aon_apb_slv_t_maintimeout       : 1;  /* bit[31]: 同bit0。 */
    } reg;
} SOC_PMCTRL_PERI_INT0_MASK_UNION;
#endif
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npugic_cfg_t_maintimeout_START         (0)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npugic_cfg_t_maintimeout_END           (0)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_hkadc_ssi_t_maintimeout_START          (1)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_hkadc_ssi_t_maintimeout_END            (1)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_gpu_cfg_t_maintimeout_START            (2)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_gpu_cfg_t_maintimeout_END              (2)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_lpm3_slv_t_maintimeout_START           (3)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_lpm3_slv_t_maintimeout_END             (3)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri0_cfg_t_maintimeout_START      (4)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri0_cfg_t_maintimeout_END        (4)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri1_cfg_t_maintimeout_START      (5)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri1_cfg_t_maintimeout_END        (5)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri2_cfg_t_maintimeout_START      (6)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri2_cfg_t_maintimeout_END        (6)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri3_cfg_t_maintimeout_START      (7)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_sys_peri3_cfg_t_maintimeout_END        (7)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_fd_cfg_t_maintimeout_START             (8)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_fd_cfg_t_maintimeout_END               (8)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_dmac_cfg_t_maintimeout_START           (9)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_dmac_cfg_t_maintimeout_END             (9)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_hisee_cfg_t_maintimeout_START          (11)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_hisee_cfg_t_maintimeout_END            (11)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_top_cssys_slv_cfg_t_maintimeout_START  (13)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_top_cssys_slv_cfg_t_maintimeout_END    (13)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_ao_tcp_cfg_t_maintimeout_START         (14)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_ao_tcp_cfg_t_maintimeout_END           (14)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_modem_cfg_t_maintimeout_START          (15)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_modem_cfg_t_maintimeout_END            (15)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_usb3otg_cfg_t_maintimeout_START        (16)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_usb3otg_cfg_t_maintimeout_END          (16)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npucpu_cfg_t_maintimeout_START         (17)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npucpu_cfg_t_maintimeout_END           (17)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_vcodec_crg_cfg_t_maintimeout_START     (19)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_vcodec_crg_cfg_t_maintimeout_END       (19)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_pcie0_cfg_t_maintimeout_START          (22)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_pcie0_cfg_t_maintimeout_END            (22)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_cfg2vcodec_t_maintimeout_START         (23)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_cfg2vcodec_t_maintimeout_END           (23)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_cfg2vivo_t_maintimeout_START           (24)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_cfg2vivo_t_maintimeout_END             (24)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_dmss_apb_slv_t_maintimeout_START       (25)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_dmss_apb_slv_t_maintimeout_END         (25)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npu2cfgbus_cfg_t_maitimeout_START      (26)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npu2cfgbus_cfg_t_maitimeout_END        (26)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_asp_cfg_t_maintimeout_START            (27)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_asp_cfg_t_maintimeout_END              (27)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npubus_cfg_t_maintimeout_START         (28)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_npubus_cfg_t_maintimeout_END           (28)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_iomcu_ahb_slv_maintimeout_START        (29)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_iomcu_ahb_slv_maintimeout_END          (29)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_iomcu_apb_slv_maintimeout_START        (30)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_iomcu_apb_slv_maintimeout_END          (30)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_aon_apb_slv_t_maintimeout_START        (31)
#define SOC_PMCTRL_PERI_INT0_MASK_noc_aon_apb_slv_t_maintimeout_END          (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT0_STAT_UNION
 结构说明  : PERI_INT0_STAT 寄存器结构定义。地址偏移量:0x3A4，初值:0x00000000，宽度:32
 寄存器说明: 外设中断状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_npugic_cfg_t_maintimeout        : 1;  /* bit[0] : NOC target端口计数超时信号，当超时信号为高时，表示master访问slaver没有应答，slaver已挂死。 */
        unsigned int  noc_hkadc_ssi_t_maintimeout         : 1;  /* bit[1] : 同bit0。 */
        unsigned int  noc_gpu_cfg_t_maintimeout           : 1;  /* bit[2] : 同bit0。 */
        unsigned int  noc_lpm3_slv_t_maintimeout          : 1;  /* bit[3] : 同bit0。 */
        unsigned int  noc_sys_peri0_cfg_t_maintimeout     : 1;  /* bit[4] : 同bit0。 */
        unsigned int  noc_sys_peri1_cfg_t_maintimeout     : 1;  /* bit[5] : 同bit0。 */
        unsigned int  noc_sys_peri2_cfg_t_maintimeout     : 1;  /* bit[6] : 同bit0。 */
        unsigned int  noc_sys_peri3_cfg_t_maintimeout     : 1;  /* bit[7] : 同bit0。 */
        unsigned int  noc_fd_cfg_t_maintimeout            : 1;  /* bit[8] : 同bit0。 */
        unsigned int  noc_dmac_cfg_t_maintimeout          : 1;  /* bit[9] : 同bit0。 */
        unsigned int  reserved_0                          : 1;  /* bit[10]: 同bit0。 */
        unsigned int  noc_hisee_cfg_t_maintimeout         : 1;  /* bit[11]: 同bit0。 */
        unsigned int  reserved_1                          : 1;  /* bit[12]: 同bit0。 */
        unsigned int  noc_top_cssys_slv_cfg_t_maintimeout : 1;  /* bit[13]: 同bit0。 */
        unsigned int  noc_ao_tcp_cfg_t_maintimeout        : 1;  /* bit[14]: 同bit0。 */
        unsigned int  noc_modem_cfg_t_maintimeout         : 1;  /* bit[15]: 同bit0。 */
        unsigned int  noc_usb3otg_cfg_t_maintimeout       : 1;  /* bit[16]: 同bit0。 */
        unsigned int  noc_npucpu_cfg_t_maintimeout        : 1;  /* bit[17]: 同bit0。 */
        unsigned int  reserved_2                          : 1;  /* bit[18]: 同bit0。 */
        unsigned int  noc_vcodec_crg_cfg_t_maintimeout    : 1;  /* bit[19]: 同bit0。 */
        unsigned int  reserved_3                          : 1;  /* bit[20]: 同bit0。 */
        unsigned int  reserved_4                          : 1;  /* bit[21]: 同bit0。 */
        unsigned int  noc_pcie0_cfg_t_maintimeout         : 1;  /* bit[22]: 同bit0。 */
        unsigned int  noc_cfg2vcodec_t_maintimeout        : 1;  /* bit[23]: 同bit0。 */
        unsigned int  noc_cfg2vivo_t_maintimeout          : 1;  /* bit[24]: 同bit0。 */
        unsigned int  noc_dmss_apb_slv_t_maintimeout      : 1;  /* bit[25]: 同bit0。 */
        unsigned int  noc_npu2cfgbus_cfg_t_maitimeout     : 1;  /* bit[26]: 同bit0。 */
        unsigned int  noc_asp_cfg_t_maintimeout           : 1;  /* bit[27]: 同bit0。 */
        unsigned int  noc_npubus_cfg_t_maintimeout        : 1;  /* bit[28]: 同bit0。（Baltimore不使用） */
        unsigned int  noc_iomcu_ahb_slv_maintimeout       : 1;  /* bit[29]: 同bit0。 */
        unsigned int  noc_iomcu_apb_slv_maintimeout       : 1;  /* bit[30]: 同bit0。 */
        unsigned int  noc_aon_apb_slv_t_maintimeout       : 1;  /* bit[31]: 同bit0。 */
    } reg;
} SOC_PMCTRL_PERI_INT0_STAT_UNION;
#endif
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npugic_cfg_t_maintimeout_START         (0)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npugic_cfg_t_maintimeout_END           (0)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_hkadc_ssi_t_maintimeout_START          (1)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_hkadc_ssi_t_maintimeout_END            (1)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_gpu_cfg_t_maintimeout_START            (2)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_gpu_cfg_t_maintimeout_END              (2)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_lpm3_slv_t_maintimeout_START           (3)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_lpm3_slv_t_maintimeout_END             (3)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri0_cfg_t_maintimeout_START      (4)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri0_cfg_t_maintimeout_END        (4)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri1_cfg_t_maintimeout_START      (5)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri1_cfg_t_maintimeout_END        (5)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri2_cfg_t_maintimeout_START      (6)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri2_cfg_t_maintimeout_END        (6)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri3_cfg_t_maintimeout_START      (7)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_sys_peri3_cfg_t_maintimeout_END        (7)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_fd_cfg_t_maintimeout_START             (8)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_fd_cfg_t_maintimeout_END               (8)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_dmac_cfg_t_maintimeout_START           (9)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_dmac_cfg_t_maintimeout_END             (9)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_hisee_cfg_t_maintimeout_START          (11)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_hisee_cfg_t_maintimeout_END            (11)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_top_cssys_slv_cfg_t_maintimeout_START  (13)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_top_cssys_slv_cfg_t_maintimeout_END    (13)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_ao_tcp_cfg_t_maintimeout_START         (14)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_ao_tcp_cfg_t_maintimeout_END           (14)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_modem_cfg_t_maintimeout_START          (15)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_modem_cfg_t_maintimeout_END            (15)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_usb3otg_cfg_t_maintimeout_START        (16)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_usb3otg_cfg_t_maintimeout_END          (16)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npucpu_cfg_t_maintimeout_START         (17)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npucpu_cfg_t_maintimeout_END           (17)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_vcodec_crg_cfg_t_maintimeout_START     (19)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_vcodec_crg_cfg_t_maintimeout_END       (19)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_pcie0_cfg_t_maintimeout_START          (22)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_pcie0_cfg_t_maintimeout_END            (22)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_cfg2vcodec_t_maintimeout_START         (23)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_cfg2vcodec_t_maintimeout_END           (23)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_cfg2vivo_t_maintimeout_START           (24)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_cfg2vivo_t_maintimeout_END             (24)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_dmss_apb_slv_t_maintimeout_START       (25)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_dmss_apb_slv_t_maintimeout_END         (25)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npu2cfgbus_cfg_t_maitimeout_START      (26)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npu2cfgbus_cfg_t_maitimeout_END        (26)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_asp_cfg_t_maintimeout_START            (27)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_asp_cfg_t_maintimeout_END              (27)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npubus_cfg_t_maintimeout_START         (28)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_npubus_cfg_t_maintimeout_END           (28)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_iomcu_ahb_slv_maintimeout_START        (29)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_iomcu_ahb_slv_maintimeout_END          (29)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_iomcu_apb_slv_maintimeout_START        (30)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_iomcu_apb_slv_maintimeout_END          (30)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_aon_apb_slv_t_maintimeout_START        (31)
#define SOC_PMCTRL_PERI_INT0_STAT_noc_aon_apb_slv_t_maintimeout_END          (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT1_MASK_UNION
 结构说明  : PERI_INT1_MASK 寄存器结构定义。地址偏移量:0x3A8，初值:0x00000000，宽度:32
 寄存器说明: 外设中断屏蔽寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_dss_cfg_t_maintimeout          : 1;  /* bit[0] : NOC target端口计数超时屏蔽信号：
                                                                           0：不屏蔽计数超时信号；
                                                                           1：屏蔽计数超时信号。 */
        unsigned int  noc_isp_cfg_t_maintimeout          : 1;  /* bit[1] : 同bit0。 */
        unsigned int  noc_ipp_cfg_t_maintimeout          : 1;  /* bit[2] : 同bit0。 */
        unsigned int  noc_vcodec2cfg_cfg_t_maintimeout   : 1;  /* bit[3] : 同bit0。 */
        unsigned int  noc_vivo_crg_cfg_t_maintimeout     : 1;  /* bit[4] : 同bit0。 */
        unsigned int  noc_ivp_cfg_t_maintimeout          : 1;  /* bit[5] : 同bit0。 */
        unsigned int  noc_dmca_ahb_slv_t_maintimeout     : 1;  /* bit[6] : 同bit0。 */
        unsigned int  noc_dmcb_ahb_slv_t_maintimeout     : 1;  /* bit[7] : 同bit0。 */
        unsigned int  reserved_0                         : 1;  /* bit[8] : 同bit0。 */
        unsigned int  reserved_1                         : 1;  /* bit[9] : 同bit0。 */
        unsigned int  reserved_2                         : 1;  /* bit[10]:  */
        unsigned int  noc_hsdtbus_apb_cfg_t_maintimeout  : 1;  /* bit[11]: （Baltimore不使用） */
        unsigned int  noc_mad_cfg_t_maintimeout          : 1;  /* bit[12]:  */
        unsigned int  noc_cfg2ipu_t_maintimeout          : 1;  /* bit[13]:  */
        unsigned int  noc_venc_cfg_t_maintimeout         : 1;  /* bit[14]:  */
        unsigned int  reserved_3                         : 1;  /* bit[15]:  */
        unsigned int  noc_vdec_cfg_t_maintimeout         : 1;  /* bit[16]:  */
        unsigned int  noc_vivo2cfg_cfg_t_maintimeout     : 1;  /* bit[17]:  */
        unsigned int  reserved_4                         : 1;  /* bit[18]:  */
        unsigned int  noc_aicore_cfg_t_maintimeout       : 1;  /* bit[19]:  */
        unsigned int  reserved_5                         : 1;  /* bit[20]:  */
        unsigned int  noc_mmc0bus_apb_cfg_t_maintimeout  : 1;  /* bit[21]:  */
        unsigned int  noc_l2buffer_slv0_rd_t_maintimeout : 1;  /* bit[22]:  */
        unsigned int  noc_l2buffer_slv0_wr_t_maintimeout : 1;  /* bit[23]:  */
        unsigned int  noc_l2buffer_slv1_rd_t_maintimeout : 1;  /* bit[24]:  */
        unsigned int  noc_l2buffer_slv1_wr_t_maintimeout : 1;  /* bit[25]:  */
        unsigned int  reserved_6                         : 1;  /* bit[26]:  */
        unsigned int  noc_bba_cfg_t_maintimeout          : 1;  /* bit[27]:  */
        unsigned int  noc_modem5g_cfg_t_maintimeout      : 1;  /* bit[28]:  */
        unsigned int  noc_maa_cfg_t_maintimeout          : 1;  /* bit[29]:  */
        unsigned int  noc_aximem_slv_cfg_t_maintimeout   : 1;  /* bit[30]:  */
        unsigned int  reserved_7                         : 1;  /* bit[31]: 同bit0。 */
    } reg;
} SOC_PMCTRL_PERI_INT1_MASK_UNION;
#endif
#define SOC_PMCTRL_PERI_INT1_MASK_noc_dss_cfg_t_maintimeout_START           (0)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_dss_cfg_t_maintimeout_END             (0)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_isp_cfg_t_maintimeout_START           (1)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_isp_cfg_t_maintimeout_END             (1)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_ipp_cfg_t_maintimeout_START           (2)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_ipp_cfg_t_maintimeout_END             (2)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vcodec2cfg_cfg_t_maintimeout_START    (3)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vcodec2cfg_cfg_t_maintimeout_END      (3)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vivo_crg_cfg_t_maintimeout_START      (4)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vivo_crg_cfg_t_maintimeout_END        (4)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_ivp_cfg_t_maintimeout_START           (5)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_ivp_cfg_t_maintimeout_END             (5)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_dmca_ahb_slv_t_maintimeout_START      (6)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_dmca_ahb_slv_t_maintimeout_END        (6)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_dmcb_ahb_slv_t_maintimeout_START      (7)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_dmcb_ahb_slv_t_maintimeout_END        (7)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_hsdtbus_apb_cfg_t_maintimeout_START   (11)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_hsdtbus_apb_cfg_t_maintimeout_END     (11)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_mad_cfg_t_maintimeout_START           (12)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_mad_cfg_t_maintimeout_END             (12)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_cfg2ipu_t_maintimeout_START           (13)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_cfg2ipu_t_maintimeout_END             (13)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_venc_cfg_t_maintimeout_START          (14)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_venc_cfg_t_maintimeout_END            (14)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vdec_cfg_t_maintimeout_START          (16)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vdec_cfg_t_maintimeout_END            (16)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vivo2cfg_cfg_t_maintimeout_START      (17)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_vivo2cfg_cfg_t_maintimeout_END        (17)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_aicore_cfg_t_maintimeout_START        (19)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_aicore_cfg_t_maintimeout_END          (19)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_mmc0bus_apb_cfg_t_maintimeout_START   (21)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_mmc0bus_apb_cfg_t_maintimeout_END     (21)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv0_rd_t_maintimeout_START  (22)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv0_rd_t_maintimeout_END    (22)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv0_wr_t_maintimeout_START  (23)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv0_wr_t_maintimeout_END    (23)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv1_rd_t_maintimeout_START  (24)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv1_rd_t_maintimeout_END    (24)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv1_wr_t_maintimeout_START  (25)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_l2buffer_slv1_wr_t_maintimeout_END    (25)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_bba_cfg_t_maintimeout_START           (27)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_bba_cfg_t_maintimeout_END             (27)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_modem5g_cfg_t_maintimeout_START       (28)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_modem5g_cfg_t_maintimeout_END         (28)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_maa_cfg_t_maintimeout_START           (29)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_maa_cfg_t_maintimeout_END             (29)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_aximem_slv_cfg_t_maintimeout_START    (30)
#define SOC_PMCTRL_PERI_INT1_MASK_noc_aximem_slv_cfg_t_maintimeout_END      (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT1_STAT_UNION
 结构说明  : PERI_INT1_STAT 寄存器结构定义。地址偏移量:0x3AC，初值:0x00000000，宽度:32
 寄存器说明: 外设中断状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_dss_cfg_t_maintimeout          : 1;  /* bit[0] : NOC target端口计数超时信号，当超时信号为高时，表示master访问slaver没有应答，slaver已挂死。 */
        unsigned int  noc_isp_cfg_t_maintimeout          : 1;  /* bit[1] : 同bit0。 */
        unsigned int  noc_ipp_cfg_t_maintimeout          : 1;  /* bit[2] : 同bit0。 */
        unsigned int  noc_vcodec2cfg_cfg_t_maintimeout   : 1;  /* bit[3] : 同bit0。 */
        unsigned int  noc_vivo_crg_cfg_t_maintimeout     : 1;  /* bit[4] : 同bit0。 */
        unsigned int  noc_ivp_cfg_t_maintimeout          : 1;  /* bit[5] : 同bit0。 */
        unsigned int  noc_dmca_ahb_slv_t_maintimeout     : 1;  /* bit[6] : 同bit0。 */
        unsigned int  noc_dmcb_ahb_slv_t_maintimeout     : 1;  /* bit[7] : 同bit0。 */
        unsigned int  reserved_0                         : 1;  /* bit[8] : 同bit0。 */
        unsigned int  reserved_1                         : 1;  /* bit[9] : 同bit0。 */
        unsigned int  reserved_2                         : 1;  /* bit[10]:  */
        unsigned int  noc_hsdtbus_apb_cfg_t_maintimeout  : 1;  /* bit[11]: （Baltimore不使用） */
        unsigned int  noc_mad_cfg_t_maintimeout          : 1;  /* bit[12]:  */
        unsigned int  noc_cfg2ipu_t_maintimeout          : 1;  /* bit[13]:  */
        unsigned int  noc_venc_cfg_t_maintimeout         : 1;  /* bit[14]:  */
        unsigned int  reserved_3                         : 1;  /* bit[15]:  */
        unsigned int  noc_vdec_cfg_t_maintimeout         : 1;  /* bit[16]:  */
        unsigned int  noc_vivo2cfg_cfg_t_maintimeout     : 1;  /* bit[17]:  */
        unsigned int  reserved_4                         : 1;  /* bit[18]:  */
        unsigned int  noc_aicore_cfg_t_maintimeout       : 1;  /* bit[19]:  */
        unsigned int  reserved_5                         : 1;  /* bit[20]:  */
        unsigned int  noc_mmc0bus_apb_cfg_t_maintimeout  : 1;  /* bit[21]:  */
        unsigned int  noc_l2buffer_slv0_rd_t_maintimeout : 1;  /* bit[22]:  */
        unsigned int  noc_l2buffer_slv0_wr_t_maintimeout : 1;  /* bit[23]:  */
        unsigned int  noc_l2buffer_slv1_rd_t_maintimeout : 1;  /* bit[24]:  */
        unsigned int  noc_l2buffer_slv1_wr_t_maintimeout : 1;  /* bit[25]:  */
        unsigned int  reserved_6                         : 1;  /* bit[26]:  */
        unsigned int  noc_bba_cfg_t_maintimeout          : 1;  /* bit[27]:  */
        unsigned int  noc_modem5g_cfg_t_maintimeout      : 1;  /* bit[28]:  */
        unsigned int  noc_maa_cfg_t_maintimeout          : 1;  /* bit[29]:  */
        unsigned int  noc_aximem_slv_cfg_t_maintimeout   : 1;  /* bit[30]:  */
        unsigned int  reserved_7                         : 1;  /* bit[31]: 同bit0。 */
    } reg;
} SOC_PMCTRL_PERI_INT1_STAT_UNION;
#endif
#define SOC_PMCTRL_PERI_INT1_STAT_noc_dss_cfg_t_maintimeout_START           (0)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_dss_cfg_t_maintimeout_END             (0)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_isp_cfg_t_maintimeout_START           (1)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_isp_cfg_t_maintimeout_END             (1)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_ipp_cfg_t_maintimeout_START           (2)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_ipp_cfg_t_maintimeout_END             (2)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vcodec2cfg_cfg_t_maintimeout_START    (3)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vcodec2cfg_cfg_t_maintimeout_END      (3)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vivo_crg_cfg_t_maintimeout_START      (4)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vivo_crg_cfg_t_maintimeout_END        (4)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_ivp_cfg_t_maintimeout_START           (5)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_ivp_cfg_t_maintimeout_END             (5)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_dmca_ahb_slv_t_maintimeout_START      (6)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_dmca_ahb_slv_t_maintimeout_END        (6)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_dmcb_ahb_slv_t_maintimeout_START      (7)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_dmcb_ahb_slv_t_maintimeout_END        (7)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_hsdtbus_apb_cfg_t_maintimeout_START   (11)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_hsdtbus_apb_cfg_t_maintimeout_END     (11)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_mad_cfg_t_maintimeout_START           (12)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_mad_cfg_t_maintimeout_END             (12)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_cfg2ipu_t_maintimeout_START           (13)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_cfg2ipu_t_maintimeout_END             (13)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_venc_cfg_t_maintimeout_START          (14)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_venc_cfg_t_maintimeout_END            (14)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vdec_cfg_t_maintimeout_START          (16)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vdec_cfg_t_maintimeout_END            (16)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vivo2cfg_cfg_t_maintimeout_START      (17)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_vivo2cfg_cfg_t_maintimeout_END        (17)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_aicore_cfg_t_maintimeout_START        (19)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_aicore_cfg_t_maintimeout_END          (19)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_mmc0bus_apb_cfg_t_maintimeout_START   (21)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_mmc0bus_apb_cfg_t_maintimeout_END     (21)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv0_rd_t_maintimeout_START  (22)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv0_rd_t_maintimeout_END    (22)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv0_wr_t_maintimeout_START  (23)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv0_wr_t_maintimeout_END    (23)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv1_rd_t_maintimeout_START  (24)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv1_rd_t_maintimeout_END    (24)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv1_wr_t_maintimeout_START  (25)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_l2buffer_slv1_wr_t_maintimeout_END    (25)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_bba_cfg_t_maintimeout_START           (27)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_bba_cfg_t_maintimeout_END             (27)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_modem5g_cfg_t_maintimeout_START       (28)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_modem5g_cfg_t_maintimeout_END         (28)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_maa_cfg_t_maintimeout_START           (29)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_maa_cfg_t_maintimeout_END             (29)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_aximem_slv_cfg_t_maintimeout_START    (30)
#define SOC_PMCTRL_PERI_INT1_STAT_noc_aximem_slv_cfg_t_maintimeout_END      (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT2_MASK_UNION
 结构说明  : PERI_INT2_MASK 寄存器结构定义。地址偏移量:0x3B0，初值:0x00000000，宽度:32
 寄存器说明: 外设中断屏蔽寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_ufs_cfg_t_maintimeout    : 1;  /* bit[0] :  */
        unsigned int  reserved_0                   : 1;  /* bit[1] :  */
        unsigned int  reserved_1                   : 1;  /* bit[2] :  */
        unsigned int  reserved_2                   : 1;  /* bit[3] :  */
        unsigned int  noc_npu2cfgbus_t_maintimeout : 1;  /* bit[4] :  */
        unsigned int  noc_fcm2sys_t_maintimeout    : 1;  /* bit[5] :  */
        unsigned int  noc_gic_cfg_t_maintimeout    : 1;  /* bit[6] : （Baltimore不使用） */
        unsigned int  noc_fcm_cfg_t_maintimeout    : 1;  /* bit[7] :  */
        unsigned int  reserved_3                   : 1;  /* bit[8] :  */
        unsigned int  reserved_4                   : 1;  /* bit[9] :  */
        unsigned int  reserved_5                   : 1;  /* bit[10]:  */
        unsigned int  reserved_6                   : 1;  /* bit[11]:  */
        unsigned int  reserved_7                   : 1;  /* bit[12]:  */
        unsigned int  reserved_8                   : 1;  /* bit[13]:  */
        unsigned int  reserved_9                   : 1;  /* bit[14]:  */
        unsigned int  reserved_10                  : 1;  /* bit[15]:  */
        unsigned int  reserved_11                  : 1;  /* bit[16]:  */
        unsigned int  reserved_12                  : 1;  /* bit[17]:  */
        unsigned int  reserved_13                  : 1;  /* bit[18]:  */
        unsigned int  reserved_14                  : 1;  /* bit[19]:  */
        unsigned int  reserved_15                  : 1;  /* bit[20]:  */
        unsigned int  reserved_16                  : 1;  /* bit[21]:  */
        unsigned int  reserved_17                  : 1;  /* bit[22]:  */
        unsigned int  reserved_18                  : 1;  /* bit[23]:  */
        unsigned int  reserved_19                  : 1;  /* bit[24]:  */
        unsigned int  reserved_20                  : 1;  /* bit[25]:  */
        unsigned int  reserved_21                  : 1;  /* bit[26]:  */
        unsigned int  reserved_22                  : 1;  /* bit[27]:  */
        unsigned int  reserved_23                  : 1;  /* bit[28]:  */
        unsigned int  reserved_24                  : 1;  /* bit[29]:  */
        unsigned int  reserved_25                  : 1;  /* bit[30]:  */
        unsigned int  reserved_26                  : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_INT2_MASK_UNION;
#endif
#define SOC_PMCTRL_PERI_INT2_MASK_noc_ufs_cfg_t_maintimeout_START     (0)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_ufs_cfg_t_maintimeout_END       (0)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_npu2cfgbus_t_maintimeout_START  (4)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_npu2cfgbus_t_maintimeout_END    (4)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_fcm2sys_t_maintimeout_START     (5)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_fcm2sys_t_maintimeout_END       (5)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_gic_cfg_t_maintimeout_START     (6)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_gic_cfg_t_maintimeout_END       (6)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_fcm_cfg_t_maintimeout_START     (7)
#define SOC_PMCTRL_PERI_INT2_MASK_noc_fcm_cfg_t_maintimeout_END       (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT2_STAT_UNION
 结构说明  : PERI_INT2_STAT 寄存器结构定义。地址偏移量:0x3B4，初值:0x00000000，宽度:32
 寄存器说明: 外设中断状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  noc_ufs_cfg_t_maintimeout    : 1;  /* bit[0] :  */
        unsigned int  reserved_0                   : 1;  /* bit[1] :  */
        unsigned int  reserved_1                   : 1;  /* bit[2] :  */
        unsigned int  reserved_2                   : 1;  /* bit[3] :  */
        unsigned int  noc_npu2cfgbus_t_maintimeout : 1;  /* bit[4] :  */
        unsigned int  noc_fcm2sys_t_maintimeout    : 1;  /* bit[5] :  */
        unsigned int  noc_gic_cfg_t_maintimeout    : 1;  /* bit[6] : （Baltimore不使用） */
        unsigned int  noc_fcm_cfg_t_maintimeout    : 1;  /* bit[7] :  */
        unsigned int  reserved_3                   : 1;  /* bit[8] :  */
        unsigned int  reserved_4                   : 1;  /* bit[9] :  */
        unsigned int  reserved_5                   : 1;  /* bit[10]:  */
        unsigned int  reserved_6                   : 1;  /* bit[11]:  */
        unsigned int  reserved_7                   : 1;  /* bit[12]:  */
        unsigned int  reserved_8                   : 1;  /* bit[13]:  */
        unsigned int  reserved_9                   : 1;  /* bit[14]:  */
        unsigned int  reserved_10                  : 1;  /* bit[15]:  */
        unsigned int  reserved_11                  : 1;  /* bit[16]:  */
        unsigned int  reserved_12                  : 1;  /* bit[17]:  */
        unsigned int  reserved_13                  : 1;  /* bit[18]:  */
        unsigned int  reserved_14                  : 1;  /* bit[19]:  */
        unsigned int  reserved_15                  : 1;  /* bit[20]:  */
        unsigned int  reserved_16                  : 1;  /* bit[21]:  */
        unsigned int  reserved_17                  : 1;  /* bit[22]:  */
        unsigned int  reserved_18                  : 1;  /* bit[23]:  */
        unsigned int  reserved_19                  : 1;  /* bit[24]:  */
        unsigned int  reserved_20                  : 1;  /* bit[25]:  */
        unsigned int  reserved_21                  : 1;  /* bit[26]:  */
        unsigned int  reserved_22                  : 1;  /* bit[27]:  */
        unsigned int  reserved_23                  : 1;  /* bit[28]:  */
        unsigned int  reserved_24                  : 1;  /* bit[29]:  */
        unsigned int  reserved_25                  : 1;  /* bit[30]:  */
        unsigned int  reserved_26                  : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_INT2_STAT_UNION;
#endif
#define SOC_PMCTRL_PERI_INT2_STAT_noc_ufs_cfg_t_maintimeout_START     (0)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_ufs_cfg_t_maintimeout_END       (0)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_npu2cfgbus_t_maintimeout_START  (4)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_npu2cfgbus_t_maintimeout_END    (4)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_fcm2sys_t_maintimeout_START     (5)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_fcm2sys_t_maintimeout_END       (5)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_gic_cfg_t_maintimeout_START     (6)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_gic_cfg_t_maintimeout_END       (6)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_fcm_cfg_t_maintimeout_START     (7)
#define SOC_PMCTRL_PERI_INT2_STAT_noc_fcm_cfg_t_maintimeout_END       (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT3_MASK_UNION
 结构说明  : PERI_INT3_MASK 寄存器结构定义。地址偏移量:0x3B8，初值:0x00000000，宽度:32
 寄存器说明: 外设中断屏蔽寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_INT3_MASK_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INT3_STAT_UNION
 结构说明  : PERI_INT3_STAT 寄存器结构定义。地址偏移量:0x3BC，初值:0x00000000，宽度:32
 寄存器说明: 外设中断状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_INT3_STAT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_CTRL_EN_GPU_UNION
 结构说明  : VS_CTRL_EN_GPU 寄存器结构定义。地址偏移量:0x440，初值:0x00000000，宽度:32
 寄存器说明: Vsensor控制使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vs_ctrl_en_gpu : 1;  /* bit[0-0] : GPU Vsensor控制使能。
                                                         0：Vsensor不使能；
                                                         1：Vsensor使能。 */
        unsigned int  reserved       : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PMCTRL_VS_CTRL_EN_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_CTRL_EN_GPU_vs_ctrl_en_gpu_START  (0)
#define SOC_PMCTRL_VS_CTRL_EN_GPU_vs_ctrl_en_gpu_END    (0)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_CTRL_BYPASS_GPU_UNION
 结构说明  : VS_CTRL_BYPASS_GPU 寄存器结构定义。地址偏移量:0x444，初值:0x00000001，宽度:32
 寄存器说明: Vsensor bypass寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vs_ctrl_bypass_gpu : 1;  /* bit[0-0] : GPU Vsensor bypass信号(Baltimore不使用)。
                                                             0：Vsensor控制逻辑输出门控信号控制ICG；
                                                             1：Vsensor控制逻辑bypass，门控信号一直为1。 */
        unsigned int  reserved           : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PMCTRL_VS_CTRL_BYPASS_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_CTRL_BYPASS_GPU_vs_ctrl_bypass_gpu_START  (0)
#define SOC_PMCTRL_VS_CTRL_BYPASS_GPU_vs_ctrl_bypass_gpu_END    (0)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_CTRL_GPU_UNION
 结构说明  : VS_CTRL_GPU 寄存器结构定义。地址偏移量:0x448，初值:0x00000000，宽度:32
 寄存器说明: Vsensor控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                    : 1;  /* bit[0-0]  :  */
        unsigned int  gpu_cpu0_wfi_wfe_bypass       : 1;  /* bit[1-1]  : CPU0 WFI/WFE主动防御的bypass。
                                                                         1'b1:bypass;
                                                                         1'b0:使能。 */
        unsigned int  gpu_cpu1_wfi_wfe_bypass       : 1;  /* bit[2-2]  : CPU1 WFI/WFE主动防御的bypass。
                                                                         1'b1:bypass;
                                                                         1'b0:使能。 */
        unsigned int  gpu_cpu2_wfi_wfe_bypass       : 1;  /* bit[3-3]  : CPU2 WFI/WFE主动防御的bypass。
                                                                         1'b1:bypass;
                                                                         1'b0:使能。 */
        unsigned int  gpu_cpu3_wfi_wfe_bypass       : 1;  /* bit[4-4]  : CPU3 WFI/WFE主动防御的bypass。
                                                                         1'b1:bypass;
                                                                         1'b0:使能。 */
        unsigned int  gpu_l2_idle_div_mod           : 2;  /* bit[5-6]  : l2 idle自动降频分频模式。
                                                                         00:4分频；
                                                                         01:8分频；
                                                                         10:16分频；
                                                                         11:32分频。 */
        unsigned int  gpu_cfg_cnt_cpu_l2_idle_quit  : 16; /* bit[7-22] : 主动防御退出超时；计数周期为VDM工作时钟。 */
        unsigned int  gpu_cpu_wake_up_mode          : 2;  /* bit[23-24]: GPU WFI/WFE主动防御模式。
                                                                         2'b00:一个或一个以上退出WFI/WFE,进入主动防御;
                                                                         2'b01:两个或两个以上退出WFI/WFE,进入主动防御;
                                                                         2'b10:三个或三个以上退出WFI/WFE,进入主动防御;
                                                                         2'b11:四个同时退出WFI/WFE,进入主动防御。 */
        unsigned int  gpu_cpu_l2_idle_switch_bypass : 1;  /* bit[25-25]: GPU 退出L2主动防御功能bypass。
                                                                         1'b1:bypass;
                                                                         1'b0:使能。 */
        unsigned int  gpu_cpu_l2_idle_gt_en         : 1;  /* bit[26-26]: GPU 进入L2 IDLE后自动降频功能使能信号。
                                                                         1'b0:不使能;
                                                                         1'b1:使能，进入L2 IDLE后，主频根据自动降频模式进行分频。 */
        unsigned int  gpu_dvfs_div_en               : 1;  /* bit[27]   : GPU是否采用DVFS分频比。
                                                                         1'b1: 选择；1'b0：不选择。
                                                                         （静态配置，默认为0） */
        unsigned int  reserved_1                    : 1;  /* bit[28]   :  */
        unsigned int  sel_ckmux_gpu_icg             : 1;  /* bit[29]   : 时钟选择源头。
                                                                         1'b0：CRG直接输出的时钟；
                                                                         1'b1：SVFD输出的时钟。 */
        unsigned int  gpu_clk_div_cfg               : 2;  /* bit[30-31]: GPU低温低压降频分频比配置信号，静态配置 */
    } reg;
} SOC_PMCTRL_VS_CTRL_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu0_wfi_wfe_bypass_START        (1)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu0_wfi_wfe_bypass_END          (1)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu1_wfi_wfe_bypass_START        (2)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu1_wfi_wfe_bypass_END          (2)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu2_wfi_wfe_bypass_START        (3)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu2_wfi_wfe_bypass_END          (3)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu3_wfi_wfe_bypass_START        (4)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu3_wfi_wfe_bypass_END          (4)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_l2_idle_div_mod_START            (5)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_l2_idle_div_mod_END              (6)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cfg_cnt_cpu_l2_idle_quit_START   (7)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cfg_cnt_cpu_l2_idle_quit_END     (22)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu_wake_up_mode_START           (23)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu_wake_up_mode_END             (24)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu_l2_idle_switch_bypass_START  (25)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu_l2_idle_switch_bypass_END    (25)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu_l2_idle_gt_en_START          (26)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_cpu_l2_idle_gt_en_END            (26)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_dvfs_div_en_START                (27)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_dvfs_div_en_END                  (27)
#define SOC_PMCTRL_VS_CTRL_GPU_sel_ckmux_gpu_icg_START              (29)
#define SOC_PMCTRL_VS_CTRL_GPU_sel_ckmux_gpu_icg_END                (29)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_clk_div_cfg_START                (30)
#define SOC_PMCTRL_VS_CTRL_GPU_gpu_clk_div_cfg_END                  (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_SVFD_CTRL0_GPU_UNION
 结构说明  : VS_SVFD_CTRL0_GPU 寄存器结构定义。地址偏移量:0x44C，初值:0x00000000，宽度:32
 寄存器说明: SVFD配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_data_limit_e_gpu    : 1;  /* bit[0]    : 极限值输出状态下，使能信号。
                                                                    1'b0：清除前一次求值；
                                                                    1'b1：使能极限求值。 */
        unsigned int  svfd_off_time_step_gpu   : 1;  /* bit[1]    : 逐级升频模式下，间隔选择：
                                                                    0: 64个cycle
                                                                    1: 128个cycle。 */
        unsigned int  svfd_pulse_width_sel_gpu : 1;  /* bit[2]    : CPM内部复位脉宽调节。 */
        unsigned int  reserved_0               : 1;  /* bit[3]    : 预留测试输入。 */
        unsigned int  svfd_ulvt_ll_gpu         : 4;  /* bit[4-7]  : ULVT长线级联，级数配置PIN。 */
        unsigned int  svfd_ulvt_sl_gpu         : 4;  /* bit[8-11] : ULVT短线级联，级数配置PIN。 */
        unsigned int  svfd_lvt_ll_gpu          : 4;  /* bit[12-15]: LVT长线级联，级数配置PIN。 */
        unsigned int  svfd_lvt_sl_gpu          : 4;  /* bit[16-19]: LVT短线级联，级数配置PIN。 */
        unsigned int  svfd_trim_gpu            : 2;  /* bit[20-21]: trim信号。
                                                                    Charge pump电流大小配置。 */
        unsigned int  svfd_cmp_data_clr_gpu    : 1;  /* bit[22]   : cmp码字复位信号。
                                                                    1'b0：保存正常码字。
                                                                    1'b1：把码字置全1。 */
        unsigned int  reserved_1               : 1;  /* bit[23]   : 保留。 */
        unsigned int  svfd_d_rate_gpu          : 2;  /* bit[24-25]: CLUSTER0 DLL调相速率。
                                                                    00 : 10% 
                                                                    01 : 18.18%
                                                                    10 : 25%
                                                                    11 : reserve。 */
        unsigned int  svfd_off_mode_gpu        : 1;  /* bit[26]   : svfd降频模式。
                                                                    0：直接升频；
                                                                    1：逐级升频。 */
        unsigned int  svfd_div64_en_gpu        : 1;  /* bit[27]   : 使能信号，工作模式不翻转。 */
        unsigned int  svfd_cpm_period_gpu      : 1;  /* bit[28]   : 电压检测采样周期。
                                                                    1'b0：1cycle；
                                                                    1'b1：2cycle。 */
        unsigned int  svfd_edge_sel_gpu        : 1;  /* bit[29]   : svdf边沿选择。
                                                                    1'b0：上升沿检测； 
                                                                    1'b1：双沿检测（上升沿+下降沿）。 */
        unsigned int  svfd_cmp_data_mode_gpu   : 2;  /* bit[30-31]: CPM码字的输出模式。
                                                                    00：直接输出；
                                                                    01：最小值输出；
                                                                    10：最大值输出；
                                                                    11：不翻转。 */
    } reg;
} SOC_PMCTRL_VS_SVFD_CTRL0_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_data_limit_e_gpu_START     (0)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_data_limit_e_gpu_END       (0)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_off_time_step_gpu_START    (1)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_off_time_step_gpu_END      (1)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_pulse_width_sel_gpu_START  (2)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_pulse_width_sel_gpu_END    (2)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_ulvt_ll_gpu_START          (4)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_ulvt_ll_gpu_END            (7)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_ulvt_sl_gpu_START          (8)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_ulvt_sl_gpu_END            (11)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_lvt_ll_gpu_START           (12)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_lvt_ll_gpu_END             (15)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_lvt_sl_gpu_START           (16)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_lvt_sl_gpu_END             (19)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_trim_gpu_START             (20)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_trim_gpu_END               (21)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_cmp_data_clr_gpu_START     (22)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_cmp_data_clr_gpu_END       (22)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_d_rate_gpu_START           (24)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_d_rate_gpu_END             (25)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_off_mode_gpu_START         (26)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_off_mode_gpu_END           (26)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_div64_en_gpu_START         (27)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_div64_en_gpu_END           (27)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_cpm_period_gpu_START       (28)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_cpm_period_gpu_END         (28)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_edge_sel_gpu_START         (29)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_edge_sel_gpu_END           (29)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_cmp_data_mode_gpu_START    (30)
#define SOC_PMCTRL_VS_SVFD_CTRL0_GPU_svfd_cmp_data_mode_gpu_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_SVFD_CTRL1_GPU_UNION
 结构说明  : VS_SVFD_CTRL1_GPU 寄存器结构定义。地址偏移量:0x450，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_glitch_test_gpu : 1;  /* bit[0]    : Glitch测试使能。 */
        unsigned int  reserved_0           : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  svfd_test_ffs_gpu    : 8;  /* bit[4-11] : TEST_FFS<7:4>:降频模块预留测试pin
                                                                TEST_FFS<3>:CPM分频时钟的频率控制
                                                                0：6分频
                                                                1：4分频
                                                                TEST_FFS<2>:给CPM的时钟mux控制信号
                                                                0：原始时钟
                                                                1：分频时钟
                                                                TEST_FFS<1:0>:降频模块预留测试pin */
        unsigned int  svfd_test_cpm_gpu    : 8;  /* bit[12-19]: TEST_CPM<7>:Noise_print时钟控制信号
                                                                0：128分频；
                                                                1：高速时钟（2/4/6可配置）；
                                                                TEST_CPM<6>:噪声斜率监测输出信号mask
                                                                0：不输出；
                                                                1：输出
                                                                TEST_CPM<5:4>:噪声斜率监测输出信号mask
                                                                0：不输出；
                                                                1：输出
                                                                TEST_CPM<3:2>:保留
                                                                TEST_CPM<1>:CPM DFT 筛片jitter测试时能信号
                                                                0：不使能
                                                                1：使能
                                                                TEST_CPM<0>:CPM DFT筛片时钟选择信号：
                                                                0：选原时钟
                                                                1：选降频时钟 */
        unsigned int  reserved_1           : 12; /* bit[20-31]: 保留。 */
    } reg;
} SOC_PMCTRL_VS_SVFD_CTRL1_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_svfd_glitch_test_gpu_START  (0)
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_svfd_glitch_test_gpu_END    (0)
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_svfd_test_ffs_gpu_START     (4)
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_svfd_test_ffs_gpu_END       (11)
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_svfd_test_cpm_gpu_START     (12)
#define SOC_PMCTRL_VS_SVFD_CTRL1_GPU_svfd_test_cpm_gpu_END       (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_SVFD_CTRL2_GPU_UNION
 结构说明  : VS_SVFD_CTRL2_GPU 寄存器结构定义。地址偏移量:0x454，初值:0x00000000，宽度:32
 寄存器说明: SVFD控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_enalbe_mode_gpu     : 2;  /* bit[0-1]  : 电压检测模式选择。
                                                                    00 :reserve；
                                                                    01 : IP内部电压检测(CPM)；
                                                                    10 : IP外部电压检测(VDM)；
                                                                    11 :VDM+CPM。 */
        unsigned int  svfs_cpm_threshold_r_gpu : 6;  /* bit[2-7]  : CPM上升沿监测BANK的阈值。 */
        unsigned int  svfs_cpm_threshold_f_gpu : 6;  /* bit[8-13] : CPM下降沿监测BANK的阈值。 */
        unsigned int  vol_drop_en_gpu          : 1;  /* bit[14]   : GPU 低温低压使能分频控制信号(Burbank不使用) */
        unsigned int  reserved                 : 1;  /* bit[15]   : 保留。 */
        unsigned int  vs_svfd_ctrl2_gpu_msk    : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_PMCTRL_VS_SVFD_CTRL2_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_svfd_enalbe_mode_gpu_START      (0)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_svfd_enalbe_mode_gpu_END        (1)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_svfs_cpm_threshold_r_gpu_START  (2)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_svfs_cpm_threshold_r_gpu_END    (7)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_svfs_cpm_threshold_f_gpu_START  (8)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_svfs_cpm_threshold_f_gpu_END    (13)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_vol_drop_en_gpu_START           (14)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_vol_drop_en_gpu_END             (14)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_vs_svfd_ctrl2_gpu_msk_START     (16)
#define SOC_PMCTRL_VS_SVFD_CTRL2_GPU_vs_svfd_ctrl2_gpu_msk_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_UNION
 结构说明  : VS_CPM_DATA_STAT_GPU 寄存器结构定义。地址偏移量:0x458，初值:0x00000000，宽度:32
 寄存器说明: SVFD状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_cpm_data_r_gpu     : 8;  /* bit[0-7]  : (PHX不使用)
                                                                   上升沿检测编码输出，由128分频时钟寄存器输出。 */
        unsigned int  svfd_cpm_data_f_gpu     : 8;  /* bit[8-15] : (PHX不使用)
                                                                   下降沿检测编码输出，由128分频时钟寄存器输出。 */
        unsigned int  svfd_dll_lock_gpu       : 1;  /* bit[16]   : (PHX不使用)
                                                                   DLL lock指示寄存器。 */
        unsigned int  svfd_cpm_data_norst_gpu : 6;  /* bit[17-22]: 异常复位时保存的最后一个cpm码字。 */
        unsigned int  reserved                : 9;  /* bit[23-31]: 保留。 */
    } reg;
} SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_cpm_data_r_gpu_START      (0)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_cpm_data_r_gpu_END        (7)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_cpm_data_f_gpu_START      (8)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_cpm_data_f_gpu_END        (15)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_dll_lock_gpu_START        (16)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_dll_lock_gpu_END          (16)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_cpm_data_norst_gpu_START  (17)
#define SOC_PMCTRL_VS_CPM_DATA_STAT_GPU_svfd_cpm_data_norst_gpu_END    (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_VS_TEST_STAT_GPU_UNION
 结构说明  : VS_TEST_STAT_GPU 寄存器结构定义。地址偏移量:0x45C，初值:0x00000000，宽度:32
 寄存器说明: SVFD测试状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  svfd_test_out_ffs_gpu     : 4;  /* bit[0-3]  : (PHX不使用)
                                                                     bit[3:2]:预留测试输出pin;
                                                                     bit[1]:DLL再锁定时处于降频状态;
                                                                     bit[0]:DLL再锁定后有失锁标志位。 */
        unsigned int  svfd_test_out_cpm_gpu     : 4;  /* bit[4-7]  : (PHX不使用)
                                                                     预留测试输出PIN。 */
        unsigned int  svfd_glitch_result_gpu    : 1;  /* bit[8]    : (PHX不使用)
                                                                     Glitch检测结果，0为无毛刺，通过。 */
        unsigned int  idle_low_freq_en_gpu      : 1;  /* bit[9]    : (PHX不使用)
                                                                     GPUIDLE使能分频信号 */
        unsigned int  vbat_drop_protect_ind_gpu : 1;  /* bit[10]   : (PHX不使用)
                                                                     GPU使能低温低压分频状态信号 */
        unsigned int  reserved                  : 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_PMCTRL_VS_TEST_STAT_GPU_UNION;
#endif
#define SOC_PMCTRL_VS_TEST_STAT_GPU_svfd_test_out_ffs_gpu_START      (0)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_svfd_test_out_ffs_gpu_END        (3)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_svfd_test_out_cpm_gpu_START      (4)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_svfd_test_out_cpm_gpu_END        (7)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_svfd_glitch_result_gpu_START     (8)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_svfd_glitch_result_gpu_END       (8)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_idle_low_freq_en_gpu_START       (9)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_idle_low_freq_en_gpu_END         (9)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_vbat_drop_protect_ind_gpu_START  (10)
#define SOC_PMCTRL_VS_TEST_STAT_GPU_vbat_drop_protect_ind_gpu_END    (10)


/*****************************************************************************
 结构名    : SOC_PMCTRL_BOOTROMFLAG_UNION
 结构说明  : BOOTROMFLAG 寄存器结构定义。地址偏移量:0x460，初值:0x00000000，宽度:32
 寄存器说明: BOOTROM FLAG寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bootrom_flag : 1;  /* bit[0-0] : BOOTROM启动时，每一次指令操作翻转该bit，用于test输出。 */
        unsigned int  reserved     : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PMCTRL_BOOTROMFLAG_UNION;
#endif
#define SOC_PMCTRL_BOOTROMFLAG_bootrom_flag_START  (0)
#define SOC_PMCTRL_BOOTROMFLAG_bootrom_flag_END    (0)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIHPMEN_UNION
 结构说明  : PERIHPMEN 寄存器结构定义。地址偏移量:0x464，初值:0x00000000，宽度:32
 寄存器说明: PERI HPM使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_hpm_en  : 1;  /* bit[0-0]  : PERI HPM使能。 */
        unsigned int  reserved_0   : 3;  /* bit[1-3]  :  */
        unsigned int  peri_hpmx_en : 1;  /* bit[4]    : PERI HPMX使能。 */
        unsigned int  reserved_1   : 3;  /* bit[5-7]  :  */
        unsigned int  reserved_2   : 1;  /* bit[8]    :  */
        unsigned int  reserved_3   : 3;  /* bit[9-11] :  */
        unsigned int  reserved_4   : 1;  /* bit[12]   :  */
        unsigned int  reserved_5   : 19; /* bit[13-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERIHPMEN_UNION;
#endif
#define SOC_PMCTRL_PERIHPMEN_peri_hpm_en_START   (0)
#define SOC_PMCTRL_PERIHPMEN_peri_hpm_en_END     (0)
#define SOC_PMCTRL_PERIHPMEN_peri_hpmx_en_START  (4)
#define SOC_PMCTRL_PERIHPMEN_peri_hpmx_en_END    (4)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIHPMCLKDIV_UNION
 结构说明  : PERIHPMCLKDIV 寄存器结构定义。地址偏移量:0x468，初值:0x00000001，宽度:32
 寄存器说明: PERI HPM时钟分频寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_hpm_clk_div : 6;  /* bit[0-5]  : PERI HPM时钟分频器。 */
        unsigned int  reserved_0       : 2;  /* bit[6-7]  :  */
        unsigned int  reserved_1       : 10; /* bit[8-17] :  */
        unsigned int  reserved_2       : 2;  /* bit[18-19]:  */
        unsigned int  reserved_3       : 10; /* bit[20-29]:  */
        unsigned int  reserved_4       : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERIHPMCLKDIV_UNION;
#endif
#define SOC_PMCTRL_PERIHPMCLKDIV_peri_hpm_clk_div_START  (0)
#define SOC_PMCTRL_PERIHPMCLKDIV_peri_hpm_clk_div_END    (5)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERIHPMOPC_UNION
 结构说明  : PERIHPMOPC 寄存器结构定义。地址偏移量:0x46C，初值:0x00000000，宽度:32
 寄存器说明: PERI HPM OPC寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_hpm_opc      : 10; /* bit[0-9]  : PERI HPM原始性能码。 */
        unsigned int  reserved_0        : 2;  /* bit[10-11]:  */
        unsigned int  peri_hpm_opc_vld  : 1;  /* bit[12]   : PERI HPM原始性能码有效的状态指示。 */
        unsigned int  reserved_1        : 3;  /* bit[13-15]:  */
        unsigned int  peri_hpmx_opc     : 10; /* bit[16-25]: PERI HPMX原始性能码。 */
        unsigned int  reserved_2        : 2;  /* bit[26-27]:  */
        unsigned int  peri_hpmx_opc_vld : 1;  /* bit[28]   : PERI HPMX原始性能码有效的状态指示。 */
        unsigned int  reserved_3        : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERIHPMOPC_UNION;
#endif
#define SOC_PMCTRL_PERIHPMOPC_peri_hpm_opc_START       (0)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpm_opc_END         (9)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpm_opc_vld_START   (12)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpm_opc_vld_END     (12)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpmx_opc_START      (16)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpmx_opc_END        (25)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpmx_opc_vld_START  (28)
#define SOC_PMCTRL_PERIHPMOPC_peri_hpmx_opc_vld_END    (28)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT8_UNION
 结构说明  : PERI_STAT8 寄存器结构定义。地址偏移量:0x4D0，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uce_prog_stat_d : 8;  /* bit[0-7]  :  */
        unsigned int  uce_prog_stat_c : 8;  /* bit[8-15] :  */
        unsigned int  uce_prog_stat_b : 8;  /* bit[16-23]:  */
        unsigned int  uce_prog_stat_a : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_PERI_STAT8_UNION;
#endif
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_d_START  (0)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_d_END    (7)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_c_START  (8)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_c_END    (15)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_b_START  (16)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_b_END    (23)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_a_START  (24)
#define SOC_PMCTRL_PERI_STAT8_uce_prog_stat_a_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT9_UNION
 结构说明  : PERI_STAT9 寄存器结构定义。地址偏移量:0x4D4，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器9
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_STAT9_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT10_UNION
 结构说明  : PERI_STAT10 寄存器结构定义。地址偏移量:0x4D8，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器10
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 30'h0,djtag_ctrl[1:0]。 */
    } reg;
} SOC_PMCTRL_PERI_STAT10_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT11_UNION
 结构说明  : PERI_STAT11 寄存器结构定义。地址偏移量:0x4DC，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器11
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_STAT11_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT12_UNION
 结构说明  : PERI_STAT12 寄存器结构定义。地址偏移量:0x4E0，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器12
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_info_pmc : 32; /* bit[0-31]: debug状态。 */
    } reg;
} SOC_PMCTRL_PERI_STAT12_UNION;
#endif
#define SOC_PMCTRL_PERI_STAT12_dbg_info_pmc_START  (0)
#define SOC_PMCTRL_PERI_STAT12_dbg_info_pmc_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT13_UNION
 结构说明  : PERI_STAT13 寄存器结构定义。地址偏移量:0x4E4，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器13
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_info_crg : 32; /* bit[0-31]: debug状态。 */
    } reg;
} SOC_PMCTRL_PERI_STAT13_UNION;
#endif
#define SOC_PMCTRL_PERI_STAT13_dbg_info_crg_START  (0)
#define SOC_PMCTRL_PERI_STAT13_dbg_info_crg_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT14_UNION
 结构说明  : PERI_STAT14 寄存器结构定义。地址偏移量:0x4E8，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器14
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_STAT14_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT15_UNION
 结构说明  : PERI_STAT15 寄存器结构定义。地址偏移量:0x4EC，初值:0x00000000，宽度:32
 寄存器说明: 外设通用状态寄存器15
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PMCTRL_PERI_STAT15_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL0CTRL0_UNION
 结构说明  : APLL0CTRL0 寄存器结构定义。地址偏移量:0x500，初值:0x00903E06，宽度:32
 寄存器说明: APLL0控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_en_cfg       : 1;  /* bit[0-0]  : APLL0 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll0_bp_cfg       : 1;  /* bit[1-1]  : APLL0 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll0_refdiv_cfg   : 6;  /* bit[2-7]  : APLL0输入时钟分频器。 */
        unsigned int  apll0_fbdiv_cfg    : 12; /* bit[8-19] : APLL0反馈时钟整数分频器。 */
        unsigned int  apll0_postdiv1_cfg : 3;  /* bit[20-22]: APLL0输出时钟分频器1。 */
        unsigned int  apll0_postdiv2_cfg : 3;  /* bit[23-25]: APLL0输出时钟分频器2。 */
        unsigned int  apll0_lock         : 1;  /* bit[26-26]: APLL0 Lock指示。
                                                              0：无效；
                                                              1：APLL0 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL0CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL0CTRL0_apll0_en_cfg_START        (0)
#define SOC_PMCTRL_APLL0CTRL0_apll0_en_cfg_END          (0)
#define SOC_PMCTRL_APLL0CTRL0_apll0_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL0CTRL0_apll0_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL0CTRL0_apll0_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL0CTRL0_apll0_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL0CTRL0_apll0_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL0CTRL0_apll0_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL0CTRL0_apll0_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL0CTRL0_apll0_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL0CTRL0_apll0_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL0CTRL0_apll0_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL0CTRL0_apll0_lock_START          (26)
#define SOC_PMCTRL_APLL0CTRL0_apll0_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL0CTRL1_UNION
 结构说明  : APLL0CTRL1 寄存器结构定义。地址偏移量:0x504，初值:0x06F80000，宽度:32
 寄存器说明: APLL0控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_fracdiv_cfg : 24; /* bit[0-23] : APLL0反馈时钟分数分频器。 */
        unsigned int  apll0_int_mod_cfg : 1;  /* bit[24-24]: APLL0整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll0_cfg_vld_cfg : 1;  /* bit[25-25]: APLL0配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll0_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL0门控信号。
                                                             0：APLL0输出时钟门控；
                                                             1：APLL0输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL0CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL0CTRL1_apll0_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL0CTRL1_apll0_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL0CTRL1_apll0_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL0CTRL1_apll0_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL0CTRL1_apll0_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL0CTRL1_apll0_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL0CTRL1_gt_clk_apll0_cfg_START   (26)
#define SOC_PMCTRL_APLL0CTRL1_gt_clk_apll0_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL0CTRL0_STAT_UNION
 结构说明  : APLL0CTRL0_STAT 寄存器结构定义。地址偏移量:0x508，初值:0x00903E06，宽度:32
 寄存器说明: APLL0控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_en_stat       : 1;  /* bit[0-0]  : APLL0 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll0_bp_stat       : 1;  /* bit[1-1]  : APLL0 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll0_refdiv_stat   : 6;  /* bit[2-7]  : APLL0输入时钟分频器。 */
        unsigned int  apll0_fbdiv_stat    : 12; /* bit[8-19] : APLL0反馈时钟整数分频器。 */
        unsigned int  apll0_postdiv1_stat : 3;  /* bit[20-22]: APLL0输出时钟分频器1。 */
        unsigned int  apll0_postdiv2_stat : 3;  /* bit[23-25]: APLL0输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL0CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_en_stat_START        (0)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_en_stat_END          (0)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_bp_stat_START        (1)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_bp_stat_END          (1)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL0CTRL0_STAT_apll0_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL0CTRL1_STAT_UNION
 结构说明  : APLL0CTRL1_STAT 寄存器结构定义。地址偏移量:0x50C，初值:0x06F80000，宽度:32
 寄存器说明: APLL0控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_fracdiv_stat : 24; /* bit[0-23] : APLL0反馈时钟分数分频器。 */
        unsigned int  apll0_int_mod_stat : 1;  /* bit[24-24]: APLL0整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll0_cfg_vld_stat : 1;  /* bit[25-25]: APLL0配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll0_stat  : 1;  /* bit[26-26]: APLL0门控信号。
                                                              0：APLL0输出时钟门控；
                                                              1：APLL0输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL0CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL0CTRL1_STAT_apll0_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL0CTRL1_STAT_apll0_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL0CTRL1_STAT_apll0_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL0CTRL1_STAT_apll0_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL0CTRL1_STAT_apll0_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL0CTRL1_STAT_apll0_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL0CTRL1_STAT_gt_clk_apll0_stat_START   (26)
#define SOC_PMCTRL_APLL0CTRL1_STAT_gt_clk_apll0_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL1CTRL0_UNION
 结构说明  : APLL1CTRL0 寄存器结构定义。地址偏移量:0x510，初值:0x00903E06，宽度:32
 寄存器说明: APLL1控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_en_cfg       : 1;  /* bit[0-0]  : APLL1 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll1_bp_cfg       : 1;  /* bit[1-1]  : APLL1 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll1_refdiv_cfg   : 6;  /* bit[2-7]  : APLL1输入时钟分频器。 */
        unsigned int  apll1_fbdiv_cfg    : 12; /* bit[8-19] : APLL1反馈时钟整数分频器。 */
        unsigned int  apll1_postdiv1_cfg : 3;  /* bit[20-22]: APLL1输出时钟分频器1。 */
        unsigned int  apll1_postdiv2_cfg : 3;  /* bit[23-25]: APLL1输出时钟分频器2。 */
        unsigned int  apll1_lock         : 1;  /* bit[26-26]: APLL1 Lock指示。
                                                              0：无效；
                                                              1：APLL1 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL1CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL1CTRL0_apll1_en_cfg_START        (0)
#define SOC_PMCTRL_APLL1CTRL0_apll1_en_cfg_END          (0)
#define SOC_PMCTRL_APLL1CTRL0_apll1_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL1CTRL0_apll1_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL1CTRL0_apll1_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL1CTRL0_apll1_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL1CTRL0_apll1_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL1CTRL0_apll1_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL1CTRL0_apll1_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL1CTRL0_apll1_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL1CTRL0_apll1_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL1CTRL0_apll1_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL1CTRL0_apll1_lock_START          (26)
#define SOC_PMCTRL_APLL1CTRL0_apll1_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL1CTRL1_UNION
 结构说明  : APLL1CTRL1 寄存器结构定义。地址偏移量:0x514，初值:0x06F80000，宽度:32
 寄存器说明: APLL1控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_fracdiv_cfg : 24; /* bit[0-23] : APLL1反馈时钟分数分频器。 */
        unsigned int  apll1_int_mod_cfg : 1;  /* bit[24-24]: APLL1整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll1_cfg_vld_cfg : 1;  /* bit[25-25]: APLL1配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll1_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL1门控信号。
                                                             0：APLL1输出时钟门控；
                                                             1：APLL1输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL1CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL1CTRL1_apll1_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL1CTRL1_apll1_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL1CTRL1_apll1_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL1CTRL1_apll1_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL1CTRL1_apll1_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL1CTRL1_apll1_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL1CTRL1_gt_clk_apll1_cfg_START   (26)
#define SOC_PMCTRL_APLL1CTRL1_gt_clk_apll1_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL1CTRL0_STAT_UNION
 结构说明  : APLL1CTRL0_STAT 寄存器结构定义。地址偏移量:0x518，初值:0x00903E06，宽度:32
 寄存器说明: APLL1控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_en_stat       : 1;  /* bit[0-0]  : APLL1 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll1_bp_stat       : 1;  /* bit[1-1]  : APLL1 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll1_refdiv_stat   : 6;  /* bit[2-7]  : APLL1输入时钟分频器。 */
        unsigned int  apll1_fbdiv_stat    : 12; /* bit[8-19] : APLL1反馈时钟整数分频器。 */
        unsigned int  apll1_postdiv1_stat : 3;  /* bit[20-22]: APLL1输出时钟分频器1。 */
        unsigned int  apll1_postdiv2_stat : 3;  /* bit[23-25]: APLL1输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL1CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_en_stat_START        (0)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_en_stat_END          (0)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_bp_stat_START        (1)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_bp_stat_END          (1)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL1CTRL0_STAT_apll1_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL1CTRL1_STAT_UNION
 结构说明  : APLL1CTRL1_STAT 寄存器结构定义。地址偏移量:0x51C，初值:0x06F80000，宽度:32
 寄存器说明: APLL1控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_fracdiv_stat : 24; /* bit[0-23] : APLL1反馈时钟分数分频器。 */
        unsigned int  apll1_int_mod_stat : 1;  /* bit[24-24]: APLL1整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll1_cfg_vld_stat : 1;  /* bit[25-25]: APLL1配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll1_stat  : 1;  /* bit[26-26]: APLL1门控信号。
                                                              0：APLL1输出时钟门控；
                                                              1：APLL1输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL1CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL1CTRL1_STAT_apll1_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL1CTRL1_STAT_apll1_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL1CTRL1_STAT_apll1_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL1CTRL1_STAT_apll1_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL1CTRL1_STAT_apll1_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL1CTRL1_STAT_apll1_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL1CTRL1_STAT_gt_clk_apll1_stat_START   (26)
#define SOC_PMCTRL_APLL1CTRL1_STAT_gt_clk_apll1_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL2CTRL0_UNION
 结构说明  : APLL2CTRL0 寄存器结构定义。地址偏移量:0x520，初值:0x00903E06，宽度:32
 寄存器说明: APLL2控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll2_en_cfg       : 1;  /* bit[0-0]  : APLL2 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll2_bp_cfg       : 1;  /* bit[1-1]  : APLL2 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll2_refdiv_cfg   : 6;  /* bit[2-7]  : APLL2输入时钟分频器。 */
        unsigned int  apll2_fbdiv_cfg    : 12; /* bit[8-19] : APLL2反馈时钟整数分频器。 */
        unsigned int  apll2_postdiv1_cfg : 3;  /* bit[20-22]: APLL2输出时钟分频器1。 */
        unsigned int  apll2_postdiv2_cfg : 3;  /* bit[23-25]: APLL2输出时钟分频器2。 */
        unsigned int  apll2_lock         : 1;  /* bit[26-26]: APLL2 Lock指示。
                                                              0：无效；
                                                              1：APLL2 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL2CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL2CTRL0_apll2_en_cfg_START        (0)
#define SOC_PMCTRL_APLL2CTRL0_apll2_en_cfg_END          (0)
#define SOC_PMCTRL_APLL2CTRL0_apll2_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL2CTRL0_apll2_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL2CTRL0_apll2_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL2CTRL0_apll2_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL2CTRL0_apll2_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL2CTRL0_apll2_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL2CTRL0_apll2_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL2CTRL0_apll2_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL2CTRL0_apll2_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL2CTRL0_apll2_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL2CTRL0_apll2_lock_START          (26)
#define SOC_PMCTRL_APLL2CTRL0_apll2_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL2CTRL1_UNION
 结构说明  : APLL2CTRL1 寄存器结构定义。地址偏移量:0x524，初值:0x06F80000，宽度:32
 寄存器说明: APLL2控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll2_fracdiv_cfg : 24; /* bit[0-23] : APLL2反馈时钟分数分频器。 */
        unsigned int  apll2_int_mod_cfg : 1;  /* bit[24-24]: APLL2整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll2_cfg_vld_cfg : 1;  /* bit[25-25]: APLL2配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll2_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL2门控信号。
                                                             0：APLL2输出时钟门控；
                                                             1：APLL2输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL2CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL2CTRL1_apll2_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL2CTRL1_apll2_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL2CTRL1_apll2_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL2CTRL1_apll2_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL2CTRL1_apll2_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL2CTRL1_apll2_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL2CTRL1_gt_clk_apll2_cfg_START   (26)
#define SOC_PMCTRL_APLL2CTRL1_gt_clk_apll2_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL2CTRL0_STAT_UNION
 结构说明  : APLL2CTRL0_STAT 寄存器结构定义。地址偏移量:0x528，初值:0x00903E06，宽度:32
 寄存器说明: APLL2控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll2_en_stat       : 1;  /* bit[0-0]  : APLL2 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll2_bp_stat       : 1;  /* bit[1-1]  : APLL2 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll2_refdiv_stat   : 6;  /* bit[2-7]  : APLL2输入时钟分频器。 */
        unsigned int  apll2_fbdiv_stat    : 12; /* bit[8-19] : APLL2反馈时钟整数分频器。 */
        unsigned int  apll2_postdiv1_stat : 3;  /* bit[20-22]: APLL2输出时钟分频器1。 */
        unsigned int  apll2_postdiv2_stat : 3;  /* bit[23-25]: APLL2输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL2CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_en_stat_START        (0)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_en_stat_END          (0)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_bp_stat_START        (1)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_bp_stat_END          (1)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL2CTRL0_STAT_apll2_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL2CTRL1_STAT_UNION
 结构说明  : APLL2CTRL1_STAT 寄存器结构定义。地址偏移量:0x52C，初值:0x06F80000，宽度:32
 寄存器说明: APLL2控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll2_fracdiv_stat : 24; /* bit[0-23] : APLL2反馈时钟分数分频器。 */
        unsigned int  apll2_int_mod_stat : 1;  /* bit[24-24]: APLL2整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll2_cfg_vld_stat : 1;  /* bit[25-25]: APLL2配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll2_stat  : 1;  /* bit[26-26]: APLL2门控信号。
                                                              0：APLL2输出时钟门控；
                                                              1：APLL2输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL2CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL2CTRL1_STAT_apll2_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL2CTRL1_STAT_apll2_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL2CTRL1_STAT_apll2_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL2CTRL1_STAT_apll2_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL2CTRL1_STAT_apll2_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL2CTRL1_STAT_apll2_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL2CTRL1_STAT_gt_clk_apll2_stat_START   (26)
#define SOC_PMCTRL_APLL2CTRL1_STAT_gt_clk_apll2_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL3CTRL0_UNION
 结构说明  : APLL3CTRL0 寄存器结构定义。地址偏移量:0x530，初值:0x00903E06，宽度:32
 寄存器说明: APLL3控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll3_en_cfg       : 1;  /* bit[0-0]  : APLL3 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll3_bp_cfg       : 1;  /* bit[1-1]  : APLL3 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll3_refdiv_cfg   : 6;  /* bit[2-7]  : APLL3输入时钟分频器。 */
        unsigned int  apll3_fbdiv_cfg    : 12; /* bit[8-19] : APLL3反馈时钟整数分频器。 */
        unsigned int  apll3_postdiv1_cfg : 3;  /* bit[20-22]: APLL3输出时钟分频器1。 */
        unsigned int  apll3_postdiv2_cfg : 3;  /* bit[23-25]: APLL3输出时钟分频器2。 */
        unsigned int  apll3_lock         : 1;  /* bit[26-26]: APLL3 Lock指示。
                                                              0：无效；
                                                              1：APLL3 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL3CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL3CTRL0_apll3_en_cfg_START        (0)
#define SOC_PMCTRL_APLL3CTRL0_apll3_en_cfg_END          (0)
#define SOC_PMCTRL_APLL3CTRL0_apll3_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL3CTRL0_apll3_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL3CTRL0_apll3_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL3CTRL0_apll3_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL3CTRL0_apll3_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL3CTRL0_apll3_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL3CTRL0_apll3_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL3CTRL0_apll3_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL3CTRL0_apll3_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL3CTRL0_apll3_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL3CTRL0_apll3_lock_START          (26)
#define SOC_PMCTRL_APLL3CTRL0_apll3_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL3CTRL1_UNION
 结构说明  : APLL3CTRL1 寄存器结构定义。地址偏移量:0x534，初值:0x06F80000，宽度:32
 寄存器说明: APLL3控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll3_fracdiv_cfg : 24; /* bit[0-23] : APLL3反馈时钟分数分频器。 */
        unsigned int  apll3_int_mod_cfg : 1;  /* bit[24-24]: APLL3整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll3_cfg_vld_cfg : 1;  /* bit[25-25]: APLL3配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll3_cfg  : 1;  /* bit[26-26]: APLL3门控信号。
                                                             0：APLL3输出时钟门控；
                                                             1：APLL3输出时钟不门控。（等待PLL稳定后才能配置该bit）。
                                                             Atlanta 大核采用FNPLL时，表示fnpll_dll_en：
                                                             1'b1: 使能;1'b0：去使能。
                                                             大核采用SCPLL时，不使用此bit。PLL选择寄存器位于PERI_CRG。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL3CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL3CTRL1_apll3_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL3CTRL1_apll3_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL3CTRL1_apll3_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL3CTRL1_apll3_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL3CTRL1_apll3_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL3CTRL1_apll3_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL3CTRL1_gt_clk_apll3_cfg_START   (26)
#define SOC_PMCTRL_APLL3CTRL1_gt_clk_apll3_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL3CTRL0_STAT_UNION
 结构说明  : APLL3CTRL0_STAT 寄存器结构定义。地址偏移量:0x538，初值:0x00903E06，宽度:32
 寄存器说明: APLL3控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll3_en_stat       : 1;  /* bit[0-0]  : APLL3 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll3_bp_stat       : 1;  /* bit[1-1]  : APLL3 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll3_refdiv_stat   : 6;  /* bit[2-7]  : APLL3输入时钟分频器。 */
        unsigned int  apll3_fbdiv_stat    : 12; /* bit[8-19] : APLL3反馈时钟整数分频器。 */
        unsigned int  apll3_postdiv1_stat : 3;  /* bit[20-22]: APLL3输出时钟分频器1。 */
        unsigned int  apll3_postdiv2_stat : 3;  /* bit[23-25]: APLL3输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL3CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_en_stat_START        (0)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_en_stat_END          (0)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_bp_stat_START        (1)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_bp_stat_END          (1)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL3CTRL0_STAT_apll3_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL3CTRL1_STAT_UNION
 结构说明  : APLL3CTRL1_STAT 寄存器结构定义。地址偏移量:0x53C，初值:0x06F80000，宽度:32
 寄存器说明: APLL3控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll3_fracdiv_stat : 24; /* bit[0-23] : APLL3反馈时钟分数分频器。 */
        unsigned int  apll3_int_mod_stat : 1;  /* bit[24-24]: APLL3整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll3_cfg_vld_stat : 1;  /* bit[25-25]: APLL3配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll3_stat  : 1;  /* bit[26-26]: APLL3门控信号。
                                                              0：APLL3输出时钟门控；
                                                              1：APLL3输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL3CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL3CTRL1_STAT_apll3_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL3CTRL1_STAT_apll3_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL3CTRL1_STAT_apll3_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL3CTRL1_STAT_apll3_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL3CTRL1_STAT_apll3_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL3CTRL1_STAT_apll3_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL3CTRL1_STAT_gt_clk_apll3_stat_START   (26)
#define SOC_PMCTRL_APLL3CTRL1_STAT_gt_clk_apll3_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL4CTRL0_UNION
 结构说明  : APLL4CTRL0 寄存器结构定义。地址偏移量:0x540，初值:0x00903E06，宽度:32
 寄存器说明: APLL4控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll4_en_cfg       : 1;  /* bit[0-0]  : APLL4 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll4_bp_cfg       : 1;  /* bit[1-1]  : APLL4 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll4_refdiv_cfg   : 6;  /* bit[2-7]  : APLL4输入时钟分频器。 */
        unsigned int  apll4_fbdiv_cfg    : 12; /* bit[8-19] : APLL4反馈时钟整数分频器。 */
        unsigned int  apll4_postdiv1_cfg : 3;  /* bit[20-22]: APLL4输出时钟分频器1。 */
        unsigned int  apll4_postdiv2_cfg : 3;  /* bit[23-25]: APLL4输出时钟分频器2。 */
        unsigned int  apll4_lock         : 1;  /* bit[26-26]: APLL4 Lock指示。
                                                              0：无效；
                                                              1：APLL4 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL4CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL4CTRL0_apll4_en_cfg_START        (0)
#define SOC_PMCTRL_APLL4CTRL0_apll4_en_cfg_END          (0)
#define SOC_PMCTRL_APLL4CTRL0_apll4_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL4CTRL0_apll4_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL4CTRL0_apll4_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL4CTRL0_apll4_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL4CTRL0_apll4_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL4CTRL0_apll4_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL4CTRL0_apll4_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL4CTRL0_apll4_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL4CTRL0_apll4_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL4CTRL0_apll4_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL4CTRL0_apll4_lock_START          (26)
#define SOC_PMCTRL_APLL4CTRL0_apll4_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL4CTRL1_UNION
 结构说明  : APLL4CTRL1 寄存器结构定义。地址偏移量:0x544，初值:0x06F80000，宽度:32
 寄存器说明: APLL4控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll4_fracdiv_cfg : 24; /* bit[0-23] : APLL4反馈时钟分数分频器。 */
        unsigned int  apll4_int_mod_cfg : 1;  /* bit[24-24]: APLL4整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll4_cfg_vld_cfg : 1;  /* bit[25-25]: APLL4配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll4_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL4门控信号。
                                                             0：APLL4输出时钟门控；
                                                             1：APLL4输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL4CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL4CTRL1_apll4_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL4CTRL1_apll4_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL4CTRL1_apll4_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL4CTRL1_apll4_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL4CTRL1_apll4_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL4CTRL1_apll4_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL4CTRL1_gt_clk_apll4_cfg_START   (26)
#define SOC_PMCTRL_APLL4CTRL1_gt_clk_apll4_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL4CTRL0_STAT_UNION
 结构说明  : APLL4CTRL0_STAT 寄存器结构定义。地址偏移量:0x548，初值:0x00903E06，宽度:32
 寄存器说明: APLL4控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll4_en_stat       : 1;  /* bit[0-0]  : APLL4 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll4_bp_stat       : 1;  /* bit[1-1]  : APLL4 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll4_refdiv_stat   : 6;  /* bit[2-7]  : APLL4输入时钟分频器。 */
        unsigned int  apll4_fbdiv_stat    : 12; /* bit[8-19] : APLL4反馈时钟整数分频器。 */
        unsigned int  apll4_postdiv1_stat : 3;  /* bit[20-22]: APLL4输出时钟分频器1。 */
        unsigned int  apll4_postdiv2_stat : 3;  /* bit[23-25]: APLL4输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL4CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_en_stat_START        (0)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_en_stat_END          (0)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_bp_stat_START        (1)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_bp_stat_END          (1)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL4CTRL0_STAT_apll4_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL4CTRL1_STAT_UNION
 结构说明  : APLL4CTRL1_STAT 寄存器结构定义。地址偏移量:0x54C，初值:0x06F80000，宽度:32
 寄存器说明: APLL4控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll4_fracdiv_stat : 24; /* bit[0-23] : APLL4反馈时钟分数分频器。 */
        unsigned int  apll4_int_mod_stat : 1;  /* bit[24-24]: APLL4整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll4_cfg_vld_stat : 1;  /* bit[25-25]: APLL4配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll4_stat  : 1;  /* bit[26-26]: APLL4门控信号。
                                                              0：APLL4输出时钟门控；
                                                              1：APLL4输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL4CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL4CTRL1_STAT_apll4_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL4CTRL1_STAT_apll4_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL4CTRL1_STAT_apll4_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL4CTRL1_STAT_apll4_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL4CTRL1_STAT_apll4_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL4CTRL1_STAT_apll4_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL4CTRL1_STAT_gt_clk_apll4_stat_START   (26)
#define SOC_PMCTRL_APLL4CTRL1_STAT_gt_clk_apll4_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL5CTRL0_UNION
 结构说明  : APLL5CTRL0 寄存器结构定义。地址偏移量:0x550，初值:0x00903E06，宽度:32
 寄存器说明: APLL5控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll5_en_cfg       : 1;  /* bit[0-0]  : APLL5 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll5_bp_cfg       : 1;  /* bit[1-1]  : APLL5 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll5_refdiv_cfg   : 6;  /* bit[2-7]  : APLL5输入时钟分频器。 */
        unsigned int  apll5_fbdiv_cfg    : 12; /* bit[8-19] : APLL5反馈时钟整数分频器。 */
        unsigned int  apll5_postdiv1_cfg : 3;  /* bit[20-22]: APLL5输出时钟分频器1。 */
        unsigned int  apll5_postdiv2_cfg : 3;  /* bit[23-25]: APLL5输出时钟分频器2。 */
        unsigned int  apll5_lock         : 1;  /* bit[26-26]: APLL5 Lock指示。
                                                              0：无效；
                                                              1：APLL5 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL5CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL5CTRL0_apll5_en_cfg_START        (0)
#define SOC_PMCTRL_APLL5CTRL0_apll5_en_cfg_END          (0)
#define SOC_PMCTRL_APLL5CTRL0_apll5_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL5CTRL0_apll5_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL5CTRL0_apll5_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL5CTRL0_apll5_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL5CTRL0_apll5_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL5CTRL0_apll5_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL5CTRL0_apll5_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL5CTRL0_apll5_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL5CTRL0_apll5_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL5CTRL0_apll5_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL5CTRL0_apll5_lock_START          (26)
#define SOC_PMCTRL_APLL5CTRL0_apll5_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL5CTRL1_UNION
 结构说明  : APLL5CTRL1 寄存器结构定义。地址偏移量:0x554，初值:0x06F80000，宽度:32
 寄存器说明: APLL5控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll5_fracdiv_cfg : 24; /* bit[0-23] : APLL5反馈时钟分数分频器。 */
        unsigned int  apll5_int_mod_cfg : 1;  /* bit[24-24]: APLL5整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll5_cfg_vld_cfg : 1;  /* bit[25-25]: APLL5配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll5_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL5门控信号。
                                                             0：APLL5输出时钟门控；
                                                             1：APLL5输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL5CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL5CTRL1_apll5_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL5CTRL1_apll5_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL5CTRL1_apll5_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL5CTRL1_apll5_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL5CTRL1_apll5_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL5CTRL1_apll5_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL5CTRL1_gt_clk_apll5_cfg_START   (26)
#define SOC_PMCTRL_APLL5CTRL1_gt_clk_apll5_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL5CTRL0_STAT_UNION
 结构说明  : APLL5CTRL0_STAT 寄存器结构定义。地址偏移量:0x558，初值:0x00903E06，宽度:32
 寄存器说明: APLL5控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll5_en_stat       : 1;  /* bit[0-0]  : APLL5 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll5_bp_stat       : 1;  /* bit[1-1]  : APLL5 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll5_refdiv_stat   : 6;  /* bit[2-7]  : APLL5输入时钟分频器。 */
        unsigned int  apll5_fbdiv_stat    : 12; /* bit[8-19] : APLL5反馈时钟整数分频器。 */
        unsigned int  apll5_postdiv1_stat : 3;  /* bit[20-22]: APLL5输出时钟分频器1。 */
        unsigned int  apll5_postdiv2_stat : 3;  /* bit[23-25]: APLL5输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL5CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_en_stat_START        (0)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_en_stat_END          (0)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_bp_stat_START        (1)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_bp_stat_END          (1)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL5CTRL0_STAT_apll5_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL5CTRL1_STAT_UNION
 结构说明  : APLL5CTRL1_STAT 寄存器结构定义。地址偏移量:0x55C，初值:0x06F80000，宽度:32
 寄存器说明: APLL5控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll5_fracdiv_stat : 24; /* bit[0-23] : APLL5反馈时钟分数分频器。 */
        unsigned int  apll5_int_mod_stat : 1;  /* bit[24-24]: APLL5整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll5_cfg_vld_stat : 1;  /* bit[25-25]: APLL5配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll5_stat  : 1;  /* bit[26-26]: APLL5门控信号。
                                                              0：APLL5输出时钟门控；
                                                              1：APLL5输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL5CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL5CTRL1_STAT_apll5_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL5CTRL1_STAT_apll5_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL5CTRL1_STAT_apll5_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL5CTRL1_STAT_apll5_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL5CTRL1_STAT_apll5_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL5CTRL1_STAT_apll5_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL5CTRL1_STAT_gt_clk_apll5_stat_START   (26)
#define SOC_PMCTRL_APLL5CTRL1_STAT_gt_clk_apll5_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL6CTRL0_UNION
 结构说明  : APLL6CTRL0 寄存器结构定义。地址偏移量:0x560，初值:0x00903D06，宽度:32
 寄存器说明: APLL6控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll6_en_cfg       : 1;  /* bit[0-0]  : APLL6 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll6_bp_cfg       : 1;  /* bit[1-1]  : APLL6 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll6_refdiv_cfg   : 6;  /* bit[2-7]  : APLL6输入时钟分频器。 */
        unsigned int  apll6_fbdiv_cfg    : 12; /* bit[8-19] : APLL6反馈时钟整数分频器。 */
        unsigned int  apll6_postdiv1_cfg : 3;  /* bit[20-22]: APLL6输出时钟分频器1。 */
        unsigned int  apll6_postdiv2_cfg : 3;  /* bit[23-25]: APLL6输出时钟分频器2。 */
        unsigned int  apll6_lock         : 1;  /* bit[26-26]: APLL6 Lock指示。
                                                              0：无效；
                                                              1：APLL6 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL6CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL6CTRL0_apll6_en_cfg_START        (0)
#define SOC_PMCTRL_APLL6CTRL0_apll6_en_cfg_END          (0)
#define SOC_PMCTRL_APLL6CTRL0_apll6_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL6CTRL0_apll6_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL6CTRL0_apll6_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL6CTRL0_apll6_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL6CTRL0_apll6_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL6CTRL0_apll6_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL6CTRL0_apll6_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL6CTRL0_apll6_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL6CTRL0_apll6_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL6CTRL0_apll6_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL6CTRL0_apll6_lock_START          (26)
#define SOC_PMCTRL_APLL6CTRL0_apll6_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL6CTRL1_UNION
 结构说明  : APLL6CTRL1 寄存器结构定义。地址偏移量:0x564，初值:0x0670A3D7，宽度:32
 寄存器说明: APLL6控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll6_fracdiv_cfg : 24; /* bit[0-23] : APLL6反馈时钟分数分频器。 */
        unsigned int  apll6_int_mod_cfg : 1;  /* bit[24-24]: APLL6整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll6_cfg_vld_cfg : 1;  /* bit[25-25]: APLL6配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll6_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL6门控信号。
                                                             0：APLL6输出时钟门控；
                                                             1：APLL6输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL6CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL6CTRL1_apll6_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL6CTRL1_apll6_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL6CTRL1_apll6_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL6CTRL1_apll6_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL6CTRL1_apll6_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL6CTRL1_apll6_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL6CTRL1_gt_clk_apll6_cfg_START   (26)
#define SOC_PMCTRL_APLL6CTRL1_gt_clk_apll6_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL6CTRL0_STAT_UNION
 结构说明  : APLL6CTRL0_STAT 寄存器结构定义。地址偏移量:0x568，初值:0x00903E06，宽度:32
 寄存器说明: APLL6控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll6_en_stat       : 1;  /* bit[0-0]  : APLL6 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll6_bp_stat       : 1;  /* bit[1-1]  : APLL6 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll6_refdiv_stat   : 6;  /* bit[2-7]  : APLL6输入时钟分频器。 */
        unsigned int  apll6_fbdiv_stat    : 12; /* bit[8-19] : APLL6反馈时钟整数分频器。 */
        unsigned int  apll6_postdiv1_stat : 3;  /* bit[20-22]: APLL6输出时钟分频器1。 */
        unsigned int  apll6_postdiv2_stat : 3;  /* bit[23-25]: APLL6输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL6CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_en_stat_START        (0)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_en_stat_END          (0)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_bp_stat_START        (1)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_bp_stat_END          (1)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL6CTRL0_STAT_apll6_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL6CTRL1_STAT_UNION
 结构说明  : APLL6CTRL1_STAT 寄存器结构定义。地址偏移量:0x56C，初值:0x06F80000，宽度:32
 寄存器说明: APLL6控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll6_fracdiv_stat : 24; /* bit[0-23] : APLL6反馈时钟分数分频器。 */
        unsigned int  apll6_int_mod_stat : 1;  /* bit[24-24]: APLL6整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll6_cfg_vld_stat : 1;  /* bit[25-25]: APLL6配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll6_stat  : 1;  /* bit[26-26]: APLL6门控信号。
                                                              0：APLL6输出时钟门控；
                                                              1：APLL6输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL6CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL6CTRL1_STAT_apll6_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL6CTRL1_STAT_apll6_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL6CTRL1_STAT_apll6_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL6CTRL1_STAT_apll6_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL6CTRL1_STAT_apll6_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL6CTRL1_STAT_apll6_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL6CTRL1_STAT_gt_clk_apll6_stat_START   (26)
#define SOC_PMCTRL_APLL6CTRL1_STAT_gt_clk_apll6_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL7CTRL0_UNION
 结构说明  : APLL7CTRL0 寄存器结构定义。地址偏移量:0x570，初值:0x00903D06，宽度:32
 寄存器说明: APLL7控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll7_en_cfg       : 1;  /* bit[0-0]  : APLL6 PowerDown控制。
                                                              0：关闭；
                                                              1：使能。 */
        unsigned int  apll7_bp_cfg       : 1;  /* bit[1-1]  : APLL6 Bypass控制。
                                                              0：正常工作；
                                                              1：Bypass。 */
        unsigned int  apll7_refdiv_cfg   : 6;  /* bit[2-7]  : APLL6输入时钟分频器。 */
        unsigned int  apll7_fbdiv_cfg    : 12; /* bit[8-19] : APLL6反馈时钟整数分频器。 */
        unsigned int  apll7_postdiv1_cfg : 3;  /* bit[20-22]: APLL6输出时钟分频器1。 */
        unsigned int  apll7_postdiv2_cfg : 3;  /* bit[23-25]: APLL6输出时钟分频器2。 */
        unsigned int  apll7_lock         : 1;  /* bit[26-26]: APLL6 Lock指示。
                                                              0：无效；
                                                              1：APLL6 Lock。 */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL7CTRL0_UNION;
#endif
#define SOC_PMCTRL_APLL7CTRL0_apll7_en_cfg_START        (0)
#define SOC_PMCTRL_APLL7CTRL0_apll7_en_cfg_END          (0)
#define SOC_PMCTRL_APLL7CTRL0_apll7_bp_cfg_START        (1)
#define SOC_PMCTRL_APLL7CTRL0_apll7_bp_cfg_END          (1)
#define SOC_PMCTRL_APLL7CTRL0_apll7_refdiv_cfg_START    (2)
#define SOC_PMCTRL_APLL7CTRL0_apll7_refdiv_cfg_END      (7)
#define SOC_PMCTRL_APLL7CTRL0_apll7_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_APLL7CTRL0_apll7_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_APLL7CTRL0_apll7_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_APLL7CTRL0_apll7_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_APLL7CTRL0_apll7_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_APLL7CTRL0_apll7_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_APLL7CTRL0_apll7_lock_START          (26)
#define SOC_PMCTRL_APLL7CTRL0_apll7_lock_END            (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL7CTRL1_UNION
 结构说明  : APLL7CTRL1 寄存器结构定义。地址偏移量:0x574，初值:0x0670A3D7，宽度:32
 寄存器说明: APLL7控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll7_fracdiv_cfg : 24; /* bit[0-23] : APLL6反馈时钟分数分频器。 */
        unsigned int  apll7_int_mod_cfg : 1;  /* bit[24-24]: APLL6整数分频模式。
                                                             0：分数分频模式；
                                                             1：整数分频模式。
                                                             注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll7_cfg_vld_cfg : 1;  /* bit[25-25]: APLL6配置有效标志。
                                                             0：配置无效；
                                                             1：配置有效。 */
        unsigned int  gt_clk_apll7_cfg  : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                             APLL6门控信号。
                                                             0：APLL6输出时钟门控；
                                                             1：APLL6输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved          : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL7CTRL1_UNION;
#endif
#define SOC_PMCTRL_APLL7CTRL1_apll7_fracdiv_cfg_START  (0)
#define SOC_PMCTRL_APLL7CTRL1_apll7_fracdiv_cfg_END    (23)
#define SOC_PMCTRL_APLL7CTRL1_apll7_int_mod_cfg_START  (24)
#define SOC_PMCTRL_APLL7CTRL1_apll7_int_mod_cfg_END    (24)
#define SOC_PMCTRL_APLL7CTRL1_apll7_cfg_vld_cfg_START  (25)
#define SOC_PMCTRL_APLL7CTRL1_apll7_cfg_vld_cfg_END    (25)
#define SOC_PMCTRL_APLL7CTRL1_gt_clk_apll7_cfg_START   (26)
#define SOC_PMCTRL_APLL7CTRL1_gt_clk_apll7_cfg_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL7CTRL0_STAT_UNION
 结构说明  : APLL7CTRL0_STAT 寄存器结构定义。地址偏移量:0x578，初值:0x00903E06，宽度:32
 寄存器说明: APLL7控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll7_en_stat       : 1;  /* bit[0-0]  : APLL6 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll7_bp_stat       : 1;  /* bit[1-1]  : APLL6 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll7_refdiv_stat   : 6;  /* bit[2-7]  : APLL6输入时钟分频器。 */
        unsigned int  apll7_fbdiv_stat    : 12; /* bit[8-19] : APLL6反馈时钟整数分频器。 */
        unsigned int  apll7_postdiv1_stat : 3;  /* bit[20-22]: APLL6输出时钟分频器1。 */
        unsigned int  apll7_postdiv2_stat : 3;  /* bit[23-25]: APLL6输出时钟分频器2。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL7CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_en_stat_START        (0)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_en_stat_END          (0)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_bp_stat_START        (1)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_bp_stat_END          (1)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_refdiv_stat_START    (2)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_refdiv_stat_END      (7)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_fbdiv_stat_START     (8)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_fbdiv_stat_END       (19)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_postdiv1_stat_START  (20)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_postdiv1_stat_END    (22)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_postdiv2_stat_START  (23)
#define SOC_PMCTRL_APLL7CTRL0_STAT_apll7_postdiv2_stat_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL7CTRL1_STAT_UNION
 结构说明  : APLL7CTRL1_STAT 寄存器结构定义。地址偏移量:0x57C，初值:0x06F80000，宽度:32
 寄存器说明: APLL7控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll7_fracdiv_stat : 24; /* bit[0-23] : APLL6反馈时钟分数分频器。 */
        unsigned int  apll7_int_mod_stat : 1;  /* bit[24-24]: APLL6整数分频模式。
                                                              0：分数分频模式；
                                                              1：整数分频模式。
                                                              注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll7_cfg_vld_stat : 1;  /* bit[25-25]: APLL6配置有效标志。
                                                              0：配置无效；
                                                              1：配置有效。 */
        unsigned int  gt_clk_apll7_stat  : 1;  /* bit[26-26]: APLL6门控信号。
                                                              0：APLL6输出时钟门控；
                                                              1：APLL6输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved           : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL7CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_APLL7CTRL1_STAT_apll7_fracdiv_stat_START  (0)
#define SOC_PMCTRL_APLL7CTRL1_STAT_apll7_fracdiv_stat_END    (23)
#define SOC_PMCTRL_APLL7CTRL1_STAT_apll7_int_mod_stat_START  (24)
#define SOC_PMCTRL_APLL7CTRL1_STAT_apll7_int_mod_stat_END    (24)
#define SOC_PMCTRL_APLL7CTRL1_STAT_apll7_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_APLL7CTRL1_STAT_apll7_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_APLL7CTRL1_STAT_gt_clk_apll7_stat_START   (26)
#define SOC_PMCTRL_APLL7CTRL1_STAT_gt_clk_apll7_stat_END     (26)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL0_SW_INITIAL0_UNION
 结构说明  : APLL0_SW_INITIAL0 寄存器结构定义。地址偏移量:0x5E0，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll0_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll0_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll0_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL0_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL0_SW_INITIAL0_apll0_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL0_SW_INITIAL0_apll0_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL0_SW_INITIAL0_apll0_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL0_SW_INITIAL0_apll0_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL0_SW_INITIAL0_apll0_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL0_SW_INITIAL0_apll0_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL0_SW_INITIAL1_UNION
 结构说明  : APLL0_SW_INITIAL1 寄存器结构定义。地址偏移量:0x5E4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll0_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL0_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL0_SW_INITIAL1_apll0_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL0_SW_INITIAL1_apll0_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL1_SW_INITIAL0_UNION
 结构说明  : APLL1_SW_INITIAL0 寄存器结构定义。地址偏移量:0x5E8，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll1_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll1_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll1_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL1_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL1_SW_INITIAL0_apll1_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL1_SW_INITIAL0_apll1_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL1_SW_INITIAL0_apll1_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL1_SW_INITIAL0_apll1_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL1_SW_INITIAL0_apll1_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL1_SW_INITIAL0_apll1_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL1_SW_INITIAL1_UNION
 结构说明  : APLL1_SW_INITIAL1 寄存器结构定义。地址偏移量:0x5EC，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll1_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL1_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL1_SW_INITIAL1_apll1_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL1_SW_INITIAL1_apll1_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL2_SW_INITIAL0_UNION
 结构说明  : APLL2_SW_INITIAL0 寄存器结构定义。地址偏移量:0x5F0，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll2_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll2_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll2_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL2_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL2_SW_INITIAL0_apll2_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL2_SW_INITIAL0_apll2_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL2_SW_INITIAL0_apll2_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL2_SW_INITIAL0_apll2_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL2_SW_INITIAL0_apll2_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL2_SW_INITIAL0_apll2_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL2_SW_INITIAL1_UNION
 结构说明  : APLL2_SW_INITIAL1 寄存器结构定义。地址偏移量:0x5F4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll2_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL2_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL2_SW_INITIAL1_apll2_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL2_SW_INITIAL1_apll2_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL3_SW_INITIAL0_UNION
 结构说明  : APLL3_SW_INITIAL0 寄存器结构定义。地址偏移量:0x5F8，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll3_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll3_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll3_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL3_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL3_SW_INITIAL0_apll3_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL3_SW_INITIAL0_apll3_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL3_SW_INITIAL0_apll3_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL3_SW_INITIAL0_apll3_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL3_SW_INITIAL0_apll3_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL3_SW_INITIAL0_apll3_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL3_SW_INITIAL1_UNION
 结构说明  : APLL3_SW_INITIAL1 寄存器结构定义。地址偏移量:0x5FC，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll3_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL3_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL3_SW_INITIAL1_apll3_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL3_SW_INITIAL1_apll3_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL4_SW_INITIAL0_UNION
 结构说明  : APLL4_SW_INITIAL0 寄存器结构定义。地址偏移量:0x600，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll4_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll4_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll4_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL4_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL4_SW_INITIAL0_apll4_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL4_SW_INITIAL0_apll4_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL4_SW_INITIAL0_apll4_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL4_SW_INITIAL0_apll4_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL4_SW_INITIAL0_apll4_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL4_SW_INITIAL0_apll4_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL4_SW_INITIAL1_UNION
 结构说明  : APLL4_SW_INITIAL1 寄存器结构定义。地址偏移量:0x604，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll4_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL4_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL4_SW_INITIAL1_apll4_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL4_SW_INITIAL1_apll4_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL5_SW_INITIAL0_UNION
 结构说明  : APLL5_SW_INITIAL0 寄存器结构定义。地址偏移量:0x608，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll5_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll5_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll5_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL5_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL5_SW_INITIAL0_apll5_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL5_SW_INITIAL0_apll5_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL5_SW_INITIAL0_apll5_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL5_SW_INITIAL0_apll5_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL5_SW_INITIAL0_apll5_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL5_SW_INITIAL0_apll5_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL5_SW_INITIAL1_UNION
 结构说明  : APLL5_SW_INITIAL1 寄存器结构定义。地址偏移量:0x60C，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll5_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL5_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL5_SW_INITIAL1_apll5_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL5_SW_INITIAL1_apll5_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL6_SW_INITIAL0_UNION
 结构说明  : APLL6_SW_INITIAL0 寄存器结构定义。地址偏移量:0x610，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll6_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll6_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll6_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]:  */
    } reg;
} SOC_PMCTRL_APLL6_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL6_SW_INITIAL0_apll6_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL6_SW_INITIAL0_apll6_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL6_SW_INITIAL0_apll6_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL6_SW_INITIAL0_apll6_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL6_SW_INITIAL0_apll6_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL6_SW_INITIAL0_apll6_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL6_SW_INITIAL1_UNION
 结构说明  : APLL6_SW_INITIAL1 寄存器结构定义。地址偏移量:0x614，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll6_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL6_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL6_SW_INITIAL1_apll6_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL6_SW_INITIAL1_apll6_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL7_SW_INITIAL0_UNION
 结构说明  : APLL7_SW_INITIAL0 寄存器结构定义。地址偏移量:0x618，初值:0x00901500，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0                : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                : 6;  /* bit[2-7]  :  */
        unsigned int  apll7_fbdiv_sw_initial    : 12; /* bit[8-19] :  */
        unsigned int  apll7_postdiv1_sw_initial : 3;  /* bit[20-22]:  */
        unsigned int  apll7_postdiv2_sw_initial : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                : 5;  /* bit[27-31]:  */
    } reg;
} SOC_PMCTRL_APLL7_SW_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_APLL7_SW_INITIAL0_apll7_fbdiv_sw_initial_START     (8)
#define SOC_PMCTRL_APLL7_SW_INITIAL0_apll7_fbdiv_sw_initial_END       (19)
#define SOC_PMCTRL_APLL7_SW_INITIAL0_apll7_postdiv1_sw_initial_START  (20)
#define SOC_PMCTRL_APLL7_SW_INITIAL0_apll7_postdiv1_sw_initial_END    (22)
#define SOC_PMCTRL_APLL7_SW_INITIAL0_apll7_postdiv2_sw_initial_START  (23)
#define SOC_PMCTRL_APLL7_SW_INITIAL0_apll7_postdiv2_sw_initial_END    (25)


/*****************************************************************************
 结构名    : SOC_PMCTRL_APLL7_SW_INITIAL1_UNION
 结构说明  : APLL7_SW_INITIAL1 寄存器结构定义。地址偏移量:0x61C，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  apll7_fracdiv_sw_initial : 24; /* bit[0-23] :  */
        unsigned int  reserved_0               : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1               : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2               : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3               : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_APLL7_SW_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_APLL7_SW_INITIAL1_apll7_fracdiv_sw_initial_START  (0)
#define SOC_PMCTRL_APLL7_SW_INITIAL1_apll7_fracdiv_sw_initial_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PDVFS_INSTR_LIST_A_UNION
 结构说明  : PDVFS_INSTR_LIST_A 寄存器结构定义。地址偏移量:0xC00 + (ma)*4，初值:0x00000000，宽度:32
 寄存器说明: DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PDVFS_INSTR_LIST_A_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PDVFS_INSTR_LIST_B_UNION
 结构说明  : PDVFS_INSTR_LIST_B 寄存器结构定义。地址偏移量:0xC80 + (mb)*4，初值:0x00000000，宽度:32
 寄存器说明: DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PDVFS_INSTR_LIST_B_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PDVFS_INSTR_LIST_C_UNION
 结构说明  : PDVFS_INSTR_LIST_C 寄存器结构定义。地址偏移量:0xD00 + (mc)*4，初值:0x00000000，宽度:32
 寄存器说明: DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PDVFS_INSTR_LIST_C_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PDVFS_INSTR_LIST_D_UNION
 结构说明  : PDVFS_INSTR_LIST_D 寄存器结构定义。地址偏移量:0xD80 + (md)*4，初值:0x00000000，宽度:32
 寄存器说明: DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PDVFS_INSTR_LIST_D_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PDVFS_INSTR_LIST_E_UNION
 结构说明  : PDVFS_INSTR_LIST_E 寄存器结构定义。地址偏移量:0xE00 + (me)*4，初值:0x00000000，宽度:32
 寄存器说明: DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PDVFS_INSTR_LIST_E_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_PDVFS_INSTR_LIST_F_UNION
 结构说明  : PDVFS_INSTR_LIST_F 寄存器结构定义。地址偏移量:0xE80 + (mf)*4，初值:0x00000000，宽度:32
 寄存器说明: DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PDVFS_INSTR_LIST_F_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_A_UNION
 结构说明  : INSTR_ADDR_CTRL_A 寄存器结构定义。地址偏移量:0x700，初值:0x00C7CC00，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrla_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrla_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_A_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_taskctrla_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_taskctrla_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_taskctrla_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_taskctrla_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_taskctrla_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_A_taskctrla_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_B_UNION
 结构说明  : INSTR_ADDR_CTRL_B 寄存器结构定义。地址偏移量:0x704，初值:0x00CFCC80，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlb_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrlb_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrlb_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_B_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_taskctrlb_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_taskctrlb_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_taskctrlb_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_taskctrlb_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_taskctrlb_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_B_taskctrlb_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_C_UNION
 结构说明  : INSTR_ADDR_CTRL_C 寄存器结构定义。地址偏移量:0x708，初值:0x00D7CD00，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlc_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrlc_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrlc_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_C_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_taskctrlc_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_taskctrlc_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_taskctrlc_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_taskctrlc_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_taskctrlc_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_C_taskctrlc_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_D_UNION
 结构说明  : INSTR_ADDR_CTRL_D 寄存器结构定义。地址偏移量:0x70C，初值:0x00DFCD80，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrld_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrld_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrld_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_D_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_taskctrld_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_taskctrld_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_taskctrld_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_taskctrld_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_taskctrld_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_D_taskctrld_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_E_UNION
 结构说明  : INSTR_ADDR_CTRL_E 寄存器结构定义。地址偏移量:0x710，初值:0x00E7CE00，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrle_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrle_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrle_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_E_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_taskctrle_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_taskctrle_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_taskctrle_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_taskctrle_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_taskctrle_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_E_taskctrle_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_F_UNION
 结构说明  : INSTR_ADDR_CTRL_F 寄存器结构定义。地址偏移量:0x714，初值:0x00EFCE80，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlf_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrlf_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrlf_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_F_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_taskctrlf_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_taskctrlf_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_taskctrlf_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_taskctrlf_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_taskctrlf_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_F_taskctrlf_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_G_UNION
 结构说明  : INSTR_ADDR_CTRL_G 寄存器结构定义。地址偏移量:0x718，初值:0x00F7CF00，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlg_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrlg_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrlg_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_G_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_taskctrlg_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_taskctrlg_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_taskctrlg_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_taskctrlg_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_taskctrlg_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_G_taskctrlg_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_INSTR_ADDR_CTRL_H_UNION
 结构说明  : INSTR_ADDR_CTRL_H 寄存器结构定义。地址偏移量:0x71C，初值:0x00FFCF80，宽度:32
 寄存器说明: INSTR地址寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlh_instr_start_addr : 12; /* bit[0-11] : 子系统（TASKCTRL）的INSTR起始地址 */
        unsigned int  taskctrlh_instr_stop_addr  : 12; /* bit[12-23]: 子系统（TASKCTRL）的INSTR结束地址 */
        unsigned int  taskctrlh_instr_addr_err   : 1;  /* bit[24]   : 地址配置错误 */
        unsigned int  reserved                   : 7;  /* bit[25-31]:  */
    } reg;
} SOC_PMCTRL_INSTR_ADDR_CTRL_H_UNION;
#endif
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_taskctrlh_instr_start_addr_START  (0)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_taskctrlh_instr_start_addr_END    (11)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_taskctrlh_instr_stop_addr_START   (12)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_taskctrlh_instr_stop_addr_END     (23)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_taskctrlh_instr_addr_err_START    (24)
#define SOC_PMCTRL_INSTR_ADDR_CTRL_H_taskctrlh_instr_addr_err_END      (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_UNION
 结构说明  : TASKCTRL_INSTR_CTRL_0 寄存器结构定义。地址偏移量:0x720，初值:0x00000000，宽度:32
 寄存器说明: INSTR控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_instr_clr : 1;  /* bit[0] :  */
        unsigned int  taskctrlb_instr_clr : 1;  /* bit[1] :  */
        unsigned int  taskctrlc_instr_clr : 1;  /* bit[2] :  */
        unsigned int  taskctrld_instr_clr : 1;  /* bit[3] :  */
        unsigned int  taskctrle_instr_clr : 1;  /* bit[4] :  */
        unsigned int  taskctrlf_instr_clr : 1;  /* bit[5] :  */
        unsigned int  taskctrlg_instr_clr : 1;  /* bit[6] :  */
        unsigned int  taskctrlh_instr_clr : 1;  /* bit[7] :  */
        unsigned int  reserved_0          : 1;  /* bit[8] :  */
        unsigned int  reserved_1          : 1;  /* bit[9] :  */
        unsigned int  reserved_2          : 1;  /* bit[10]:  */
        unsigned int  reserved_3          : 1;  /* bit[11]:  */
        unsigned int  reserved_4          : 1;  /* bit[12]:  */
        unsigned int  reserved_5          : 1;  /* bit[13]:  */
        unsigned int  reserved_6          : 1;  /* bit[14]:  */
        unsigned int  reserved_7          : 1;  /* bit[15]:  */
        unsigned int  reserved_8          : 1;  /* bit[16]:  */
        unsigned int  reserved_9          : 1;  /* bit[17]:  */
        unsigned int  reserved_10         : 1;  /* bit[18]:  */
        unsigned int  reserved_11         : 1;  /* bit[19]:  */
        unsigned int  reserved_12         : 1;  /* bit[20]:  */
        unsigned int  reserved_13         : 1;  /* bit[21]:  */
        unsigned int  reserved_14         : 1;  /* bit[22]:  */
        unsigned int  reserved_15         : 1;  /* bit[23]:  */
        unsigned int  reserved_16         : 1;  /* bit[24]:  */
        unsigned int  reserved_17         : 1;  /* bit[25]:  */
        unsigned int  reserved_18         : 1;  /* bit[26]:  */
        unsigned int  reserved_19         : 1;  /* bit[27]:  */
        unsigned int  reserved_20         : 1;  /* bit[28]:  */
        unsigned int  reserved_21         : 1;  /* bit[29]:  */
        unsigned int  reserved_22         : 1;  /* bit[30]:  */
        unsigned int  reserved_23         : 1;  /* bit[31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrla_instr_clr_START  (0)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrla_instr_clr_END    (0)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlb_instr_clr_START  (1)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlb_instr_clr_END    (1)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlc_instr_clr_START  (2)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlc_instr_clr_END    (2)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrld_instr_clr_START  (3)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrld_instr_clr_END    (3)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrle_instr_clr_START  (4)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrle_instr_clr_END    (4)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlf_instr_clr_START  (5)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlf_instr_clr_END    (5)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlg_instr_clr_START  (6)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlg_instr_clr_END    (6)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlh_instr_clr_START  (7)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_0_taskctrlh_instr_clr_END    (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_UNION
 结构说明  : TASKCTRL_INSTR_CTRL_1 寄存器结构定义。地址偏移量:0x724，初值:0x00000000，宽度:32
 寄存器说明: INSTR控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_instr_lock : 1;  /* bit[0] :  */
        unsigned int  taskctrlb_instr_lock : 1;  /* bit[1] :  */
        unsigned int  taskctrlc_instr_lock : 1;  /* bit[2] :  */
        unsigned int  taskctrld_instr_lock : 1;  /* bit[3] :  */
        unsigned int  taskctrle_instr_lock : 1;  /* bit[4] :  */
        unsigned int  taskctrlf_instr_lock : 1;  /* bit[5] :  */
        unsigned int  taskctrlg_instr_lock : 1;  /* bit[6] :  */
        unsigned int  taskctrlh_instr_lock : 1;  /* bit[7] :  */
        unsigned int  reserved_0           : 1;  /* bit[8] :  */
        unsigned int  reserved_1           : 1;  /* bit[9] :  */
        unsigned int  reserved_2           : 1;  /* bit[10]:  */
        unsigned int  reserved_3           : 1;  /* bit[11]:  */
        unsigned int  reserved_4           : 1;  /* bit[12]:  */
        unsigned int  reserved_5           : 1;  /* bit[13]:  */
        unsigned int  reserved_6           : 1;  /* bit[14]:  */
        unsigned int  reserved_7           : 1;  /* bit[15]:  */
        unsigned int  reserved_8           : 1;  /* bit[16]:  */
        unsigned int  reserved_9           : 1;  /* bit[17]:  */
        unsigned int  reserved_10          : 1;  /* bit[18]:  */
        unsigned int  reserved_11          : 1;  /* bit[19]:  */
        unsigned int  reserved_12          : 1;  /* bit[20]:  */
        unsigned int  reserved_13          : 1;  /* bit[21]:  */
        unsigned int  reserved_14          : 1;  /* bit[22]:  */
        unsigned int  reserved_15          : 1;  /* bit[23]:  */
        unsigned int  reserved_16          : 1;  /* bit[24]:  */
        unsigned int  reserved_17          : 1;  /* bit[25]:  */
        unsigned int  reserved_18          : 1;  /* bit[26]:  */
        unsigned int  reserved_19          : 1;  /* bit[27]:  */
        unsigned int  reserved_20          : 1;  /* bit[28]:  */
        unsigned int  reserved_21          : 1;  /* bit[29]:  */
        unsigned int  reserved_22          : 1;  /* bit[30]:  */
        unsigned int  reserved_23          : 1;  /* bit[31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrla_instr_lock_START  (0)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrla_instr_lock_END    (0)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlb_instr_lock_START  (1)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlb_instr_lock_END    (1)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlc_instr_lock_START  (2)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlc_instr_lock_END    (2)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrld_instr_lock_START  (3)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrld_instr_lock_END    (3)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrle_instr_lock_START  (4)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrle_instr_lock_END    (4)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlf_instr_lock_START  (5)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlf_instr_lock_END    (5)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlg_instr_lock_START  (6)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlg_instr_lock_END    (6)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlh_instr_lock_START  (7)
#define SOC_PMCTRL_TASKCTRL_INSTR_CTRL_1_taskctrlh_instr_lock_END    (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_UNION
 结构说明  : TASKCTRL_INTR_CTRL_0 寄存器结构定义。地址偏移量:0x730，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL中断控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_intr_dvfs_en : 1;  /* bit[0] :  */
        unsigned int  taskctrlb_intr_dvfs_en : 1;  /* bit[1] :  */
        unsigned int  taskctrlc_intr_dvfs_en : 1;  /* bit[2] :  */
        unsigned int  taskctrld_intr_dvfs_en : 1;  /* bit[3] :  */
        unsigned int  taskctrle_intr_dvfs_en : 1;  /* bit[4] :  */
        unsigned int  taskctrlf_intr_dvfs_en : 1;  /* bit[5] :  */
        unsigned int  taskctrlg_intr_dvfs_en : 1;  /* bit[6] :  */
        unsigned int  taskctrlh_intr_dvfs_en : 1;  /* bit[7] :  */
        unsigned int  reserved_0             : 1;  /* bit[8] :  */
        unsigned int  reserved_1             : 1;  /* bit[9] :  */
        unsigned int  reserved_2             : 1;  /* bit[10]:  */
        unsigned int  reserved_3             : 1;  /* bit[11]:  */
        unsigned int  reserved_4             : 1;  /* bit[12]:  */
        unsigned int  reserved_5             : 1;  /* bit[13]:  */
        unsigned int  reserved_6             : 1;  /* bit[14]:  */
        unsigned int  reserved_7             : 1;  /* bit[15]:  */
        unsigned int  reserved_8             : 1;  /* bit[16]:  */
        unsigned int  reserved_9             : 1;  /* bit[17]:  */
        unsigned int  reserved_10            : 1;  /* bit[18]:  */
        unsigned int  reserved_11            : 1;  /* bit[19]:  */
        unsigned int  reserved_12            : 1;  /* bit[20]:  */
        unsigned int  reserved_13            : 1;  /* bit[21]:  */
        unsigned int  reserved_14            : 1;  /* bit[22]:  */
        unsigned int  reserved_15            : 1;  /* bit[23]:  */
        unsigned int  reserved_16            : 1;  /* bit[24]:  */
        unsigned int  reserved_17            : 1;  /* bit[25]:  */
        unsigned int  reserved_18            : 1;  /* bit[26]:  */
        unsigned int  reserved_19            : 1;  /* bit[27]:  */
        unsigned int  reserved_20            : 1;  /* bit[28]:  */
        unsigned int  reserved_21            : 1;  /* bit[29]:  */
        unsigned int  reserved_22            : 1;  /* bit[30]:  */
        unsigned int  reserved_23            : 1;  /* bit[31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrla_intr_dvfs_en_START  (0)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrla_intr_dvfs_en_END    (0)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlb_intr_dvfs_en_START  (1)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlb_intr_dvfs_en_END    (1)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlc_intr_dvfs_en_START  (2)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlc_intr_dvfs_en_END    (2)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrld_intr_dvfs_en_START  (3)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrld_intr_dvfs_en_END    (3)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrle_intr_dvfs_en_START  (4)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrle_intr_dvfs_en_END    (4)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlf_intr_dvfs_en_START  (5)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlf_intr_dvfs_en_END    (5)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlg_intr_dvfs_en_START  (6)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlg_intr_dvfs_en_END    (6)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlh_intr_dvfs_en_START  (7)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_0_taskctrlh_intr_dvfs_en_END    (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_UNION
 结构说明  : TASKCTRL_INTR_CTRL_1 寄存器结构定义。地址偏移量:0x734，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL中断控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_intr_dvfs_clr      : 1;  /* bit[0] :  */
        unsigned int  taskctrlb_intr_dvfs_clr      : 1;  /* bit[1] :  */
        unsigned int  taskctrlc_intr_dvfs_clr      : 1;  /* bit[2] :  */
        unsigned int  taskctrld_intr_dvfs_clr      : 1;  /* bit[3] :  */
        unsigned int  taskctrle_intr_dvfs_clr      : 1;  /* bit[4] :  */
        unsigned int  taskctrlf_intr_dvfs_clr      : 1;  /* bit[5] :  */
        unsigned int  taskctrlg_intr_dvfs_clr      : 1;  /* bit[6] :  */
        unsigned int  taskctrlh_intr_dvfs_clr      : 1;  /* bit[7] :  */
        unsigned int  reserved_0                   : 1;  /* bit[8] :  */
        unsigned int  reserved_1                   : 1;  /* bit[9] :  */
        unsigned int  reserved_2                   : 1;  /* bit[10]:  */
        unsigned int  reserved_3                   : 1;  /* bit[11]:  */
        unsigned int  reserved_4                   : 1;  /* bit[12]:  */
        unsigned int  reserved_5                   : 1;  /* bit[13]:  */
        unsigned int  reserved_6                   : 1;  /* bit[14]:  */
        unsigned int  reserved_7                   : 1;  /* bit[15]:  */
        unsigned int  taskctrla_intr_dvfs_raw_stat : 1;  /* bit[16]:  */
        unsigned int  taskctrlb_intr_dvfs_raw_stat : 1;  /* bit[17]:  */
        unsigned int  taskctrlc_intr_dvfs_raw_stat : 1;  /* bit[18]:  */
        unsigned int  taskctrld_intr_dvfs_raw_stat : 1;  /* bit[19]:  */
        unsigned int  taskctrle_intr_dvfs_raw_stat : 1;  /* bit[20]:  */
        unsigned int  taskctrlf_intr_dvfs_raw_stat : 1;  /* bit[21]:  */
        unsigned int  taskctrlg_intr_dvfs_raw_stat : 1;  /* bit[22]:  */
        unsigned int  taskctrlh_intr_dvfs_raw_stat : 1;  /* bit[23]:  */
        unsigned int  taskctrla_intr_dvfs_stat     : 1;  /* bit[24]:  */
        unsigned int  taskctrlb_intr_dvfs_stat     : 1;  /* bit[25]:  */
        unsigned int  taskctrlc_intr_dvfs_stat     : 1;  /* bit[26]:  */
        unsigned int  taskctrld_intr_dvfs_stat     : 1;  /* bit[27]:  */
        unsigned int  taskctrle_intr_dvfs_stat     : 1;  /* bit[28]:  */
        unsigned int  taskctrlf_intr_dvfs_stat     : 1;  /* bit[29]:  */
        unsigned int  taskctrlg_intr_dvfs_stat     : 1;  /* bit[30]:  */
        unsigned int  taskctrlh_intr_dvfs_stat     : 1;  /* bit[31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrla_intr_dvfs_clr_START       (0)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrla_intr_dvfs_clr_END         (0)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlb_intr_dvfs_clr_START       (1)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlb_intr_dvfs_clr_END         (1)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlc_intr_dvfs_clr_START       (2)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlc_intr_dvfs_clr_END         (2)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrld_intr_dvfs_clr_START       (3)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrld_intr_dvfs_clr_END         (3)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrle_intr_dvfs_clr_START       (4)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrle_intr_dvfs_clr_END         (4)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlf_intr_dvfs_clr_START       (5)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlf_intr_dvfs_clr_END         (5)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlg_intr_dvfs_clr_START       (6)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlg_intr_dvfs_clr_END         (6)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlh_intr_dvfs_clr_START       (7)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlh_intr_dvfs_clr_END         (7)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrla_intr_dvfs_raw_stat_START  (16)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrla_intr_dvfs_raw_stat_END    (16)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlb_intr_dvfs_raw_stat_START  (17)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlb_intr_dvfs_raw_stat_END    (17)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlc_intr_dvfs_raw_stat_START  (18)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlc_intr_dvfs_raw_stat_END    (18)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrld_intr_dvfs_raw_stat_START  (19)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrld_intr_dvfs_raw_stat_END    (19)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrle_intr_dvfs_raw_stat_START  (20)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrle_intr_dvfs_raw_stat_END    (20)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlf_intr_dvfs_raw_stat_START  (21)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlf_intr_dvfs_raw_stat_END    (21)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlg_intr_dvfs_raw_stat_START  (22)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlg_intr_dvfs_raw_stat_END    (22)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlh_intr_dvfs_raw_stat_START  (23)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlh_intr_dvfs_raw_stat_END    (23)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrla_intr_dvfs_stat_START      (24)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrla_intr_dvfs_stat_END        (24)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlb_intr_dvfs_stat_START      (25)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlb_intr_dvfs_stat_END        (25)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlc_intr_dvfs_stat_START      (26)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlc_intr_dvfs_stat_END        (26)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrld_intr_dvfs_stat_START      (27)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrld_intr_dvfs_stat_END        (27)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrle_intr_dvfs_stat_START      (28)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrle_intr_dvfs_stat_END        (28)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlf_intr_dvfs_stat_START      (29)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlf_intr_dvfs_stat_END        (29)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlg_intr_dvfs_stat_START      (30)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlg_intr_dvfs_stat_END        (30)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlh_intr_dvfs_stat_START      (31)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_1_taskctrlh_intr_dvfs_stat_END        (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_UNION
 结构说明  : TASKCTRL_INTR_CTRL_2 寄存器结构定义。地址偏移量:0x738，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL中断控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_intr_dvfs_stat_converge_ctrl : 2;  /* bit[0-1]  : 0x0: 汇聚到intr_dvfs_stat_pmc2lpm3_0
                                                                                  0x1: 汇聚到intr_dvfs_stat_pmc2lpm3_1
                                                                                  0x2: 汇聚到intr_dvfs_stat_pmc2lpm3_2
                                                                                  0x3: 不上报到LPM3 */
        unsigned int  taskctrlb_intr_dvfs_stat_converge_ctrl : 2;  /* bit[2-3]  :  */
        unsigned int  taskctrlc_intr_dvfs_stat_converge_ctrl : 2;  /* bit[4-5]  :  */
        unsigned int  taskctrld_intr_dvfs_stat_converge_ctrl : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrle_intr_dvfs_stat_converge_ctrl : 2;  /* bit[8-9]  :  */
        unsigned int  taskctrlf_intr_dvfs_stat_converge_ctrl : 2;  /* bit[10-11]:  */
        unsigned int  taskctrlg_intr_dvfs_stat_converge_ctrl : 2;  /* bit[12-13]:  */
        unsigned int  taskctrlh_intr_dvfs_stat_converge_ctrl : 2;  /* bit[14-15]:  */
        unsigned int  intr_dvfs_stat_pmc2lpm3_0              : 1;  /* bit[16]   : 汇聚后的intr_dvfs_stat */
        unsigned int  reserved_0                             : 3;  /* bit[17-19]:  */
        unsigned int  intr_dvfs_stat_pmc2lpm3_1              : 1;  /* bit[20]   :  */
        unsigned int  reserved_1                             : 3;  /* bit[21-23]:  */
        unsigned int  intr_dvfs_stat_pmc2lpm3_2              : 1;  /* bit[24]   :  */
        unsigned int  reserved_2                             : 3;  /* bit[25-27]:  */
        unsigned int  reserved_3                             : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrla_intr_dvfs_stat_converge_ctrl_START  (0)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrla_intr_dvfs_stat_converge_ctrl_END    (1)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlb_intr_dvfs_stat_converge_ctrl_START  (2)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlb_intr_dvfs_stat_converge_ctrl_END    (3)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlc_intr_dvfs_stat_converge_ctrl_START  (4)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlc_intr_dvfs_stat_converge_ctrl_END    (5)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrld_intr_dvfs_stat_converge_ctrl_START  (6)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrld_intr_dvfs_stat_converge_ctrl_END    (7)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrle_intr_dvfs_stat_converge_ctrl_START  (8)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrle_intr_dvfs_stat_converge_ctrl_END    (9)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlf_intr_dvfs_stat_converge_ctrl_START  (10)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlf_intr_dvfs_stat_converge_ctrl_END    (11)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlg_intr_dvfs_stat_converge_ctrl_START  (12)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlg_intr_dvfs_stat_converge_ctrl_END    (13)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlh_intr_dvfs_stat_converge_ctrl_START  (14)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_taskctrlh_intr_dvfs_stat_converge_ctrl_END    (15)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_intr_dvfs_stat_pmc2lpm3_0_START               (16)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_intr_dvfs_stat_pmc2lpm3_0_END                 (16)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_intr_dvfs_stat_pmc2lpm3_1_START               (20)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_intr_dvfs_stat_pmc2lpm3_1_END                 (20)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_intr_dvfs_stat_pmc2lpm3_2_START               (24)
#define SOC_PMCTRL_TASKCTRL_INTR_CTRL_2_intr_dvfs_stat_pmc2lpm3_2_END                 (24)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_UNION
 结构说明  : TASKCTRL_ENABLE_CTRL 寄存器结构定义。地址偏移量:0x73C，初值:0x00000000，宽度:32
 寄存器说明: INSTR控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_enable : 1;  /* bit[0] :  */
        unsigned int  taskctrlb_enable : 1;  /* bit[1] :  */
        unsigned int  taskctrlc_enable : 1;  /* bit[2] :  */
        unsigned int  taskctrld_enable : 1;  /* bit[3] :  */
        unsigned int  taskctrle_enable : 1;  /* bit[4] :  */
        unsigned int  taskctrlf_enable : 1;  /* bit[5] :  */
        unsigned int  taskctrlg_enable : 1;  /* bit[6] :  */
        unsigned int  taskctrlh_enable : 1;  /* bit[7] :  */
        unsigned int  reserved_0       : 1;  /* bit[8] :  */
        unsigned int  reserved_1       : 1;  /* bit[9] :  */
        unsigned int  reserved_2       : 1;  /* bit[10]:  */
        unsigned int  reserved_3       : 1;  /* bit[11]:  */
        unsigned int  reserved_4       : 1;  /* bit[12]:  */
        unsigned int  reserved_5       : 1;  /* bit[13]:  */
        unsigned int  reserved_6       : 1;  /* bit[14]:  */
        unsigned int  reserved_7       : 1;  /* bit[15]:  */
        unsigned int  reserved_8       : 1;  /* bit[16]:  */
        unsigned int  reserved_9       : 1;  /* bit[17]:  */
        unsigned int  reserved_10      : 1;  /* bit[18]:  */
        unsigned int  reserved_11      : 1;  /* bit[19]:  */
        unsigned int  reserved_12      : 1;  /* bit[20]:  */
        unsigned int  reserved_13      : 1;  /* bit[21]:  */
        unsigned int  reserved_14      : 1;  /* bit[22]:  */
        unsigned int  reserved_15      : 1;  /* bit[23]:  */
        unsigned int  reserved_16      : 1;  /* bit[24]:  */
        unsigned int  reserved_17      : 1;  /* bit[25]:  */
        unsigned int  reserved_18      : 1;  /* bit[26]:  */
        unsigned int  reserved_19      : 1;  /* bit[27]:  */
        unsigned int  reserved_20      : 1;  /* bit[28]:  */
        unsigned int  reserved_21      : 1;  /* bit[29]:  */
        unsigned int  reserved_22      : 1;  /* bit[30]:  */
        unsigned int  reserved_23      : 1;  /* bit[31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrla_enable_START  (0)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrla_enable_END    (0)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlb_enable_START  (1)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlb_enable_END    (1)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlc_enable_START  (2)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlc_enable_END    (2)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrld_enable_START  (3)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrld_enable_END    (3)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrle_enable_START  (4)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrle_enable_END    (4)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlf_enable_START  (5)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlf_enable_END    (5)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlg_enable_START  (6)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlg_enable_END    (6)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlh_enable_START  (7)
#define SOC_PMCTRL_TASKCTRL_ENABLE_CTRL_taskctrlh_enable_END    (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_A_0_UNION
 结构说明  : TASKCTRL_A_0 寄存器结构定义。地址偏移量:0x740，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrla_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrla_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrla_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrla_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrla_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrla_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrla_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_A_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_A_0_taskctrla_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_A_1_UNION
 结构说明  : TASKCTRL_A_1 寄存器结构定义。地址偏移量:0x744，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrla_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrla_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_A_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_A_1_taskctrla_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_A_1_taskctrla_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_A_1_taskctrla_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_A_1_taskctrla_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_A_2_UNION
 结构说明  : TASKCTRL_A_2 寄存器结构定义。地址偏移量:0x748，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_A_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_A_3_UNION
 结构说明  : TASKCTRL_A_3 寄存器结构定义。地址偏移量:0x74C，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_A_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_B_0_UNION
 结构说明  : TASKCTRL_B_0 寄存器结构定义。地址偏移量:0x750，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlb_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrlb_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrlb_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrlb_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrlb_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrlb_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrlb_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrlb_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_B_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_B_0_taskctrlb_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_B_1_UNION
 结构说明  : TASKCTRL_B_1 寄存器结构定义。地址偏移量:0x754，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlb_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrlb_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_B_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_B_1_taskctrlb_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_B_1_taskctrlb_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_B_1_taskctrlb_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_B_1_taskctrlb_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_B_2_UNION
 结构说明  : TASKCTRL_B_2 寄存器结构定义。地址偏移量:0x758，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_B_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_B_3_UNION
 结构说明  : TASKCTRL_B_3 寄存器结构定义。地址偏移量:0x75C，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_B_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_C_0_UNION
 结构说明  : TASKCTRL_C_0 寄存器结构定义。地址偏移量:0x760，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlc_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrlc_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrlc_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrlc_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrlc_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrlc_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrlc_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrlc_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_C_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_C_0_taskctrlc_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_C_1_UNION
 结构说明  : TASKCTRL_C_1 寄存器结构定义。地址偏移量:0x764，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlc_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrlc_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_C_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_C_1_taskctrlc_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_C_1_taskctrlc_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_C_1_taskctrlc_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_C_1_taskctrlc_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_C_2_UNION
 结构说明  : TASKCTRL_C_2 寄存器结构定义。地址偏移量:0x768，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_C_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_C_3_UNION
 结构说明  : TASKCTRL_C_3 寄存器结构定义。地址偏移量:0x76C，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_C_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_D_0_UNION
 结构说明  : TASKCTRL_D_0 寄存器结构定义。地址偏移量:0x770，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrld_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrld_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrld_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrld_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrld_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrld_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrld_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrld_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_D_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_D_0_taskctrld_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_D_1_UNION
 结构说明  : TASKCTRL_D_1 寄存器结构定义。地址偏移量:0x774，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrld_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrld_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_D_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_D_1_taskctrld_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_D_1_taskctrld_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_D_1_taskctrld_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_D_1_taskctrld_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_D_2_UNION
 结构说明  : TASKCTRL_D_2 寄存器结构定义。地址偏移量:0x778，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_D_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_D_3_UNION
 结构说明  : TASKCTRL_D_3 寄存器结构定义。地址偏移量:0x77C，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_D_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_E_0_UNION
 结构说明  : TASKCTRL_E_0 寄存器结构定义。地址偏移量:0x780，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrle_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrle_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrle_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrle_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrle_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrle_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrle_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrle_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_E_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_E_0_taskctrle_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_E_1_UNION
 结构说明  : TASKCTRL_E_1 寄存器结构定义。地址偏移量:0x784，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrle_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrle_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_E_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_E_1_taskctrle_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_E_1_taskctrle_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_E_1_taskctrle_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_E_1_taskctrle_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_E_2_UNION
 结构说明  : TASKCTRL_E_2 寄存器结构定义。地址偏移量:0x788，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_E_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_E_3_UNION
 结构说明  : TASKCTRL_E_3 寄存器结构定义。地址偏移量:0x78C，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_E_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_F_0_UNION
 结构说明  : TASKCTRL_F_0 寄存器结构定义。地址偏移量:0x790，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlf_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrlf_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrlf_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrlf_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrlf_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrlf_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrlf_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrlf_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_F_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_F_0_taskctrlf_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_F_1_UNION
 结构说明  : TASKCTRL_F_1 寄存器结构定义。地址偏移量:0x794，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlf_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrlf_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_F_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_F_1_taskctrlf_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_F_1_taskctrlf_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_F_1_taskctrlf_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_F_1_taskctrlf_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_F_2_UNION
 结构说明  : TASKCTRL_F_2 寄存器结构定义。地址偏移量:0x798，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_F_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_F_3_UNION
 结构说明  : TASKCTRL_F_3 寄存器结构定义。地址偏移量:0x79C，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_F_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_G_0_UNION
 结构说明  : TASKCTRL_G_0 寄存器结构定义。地址偏移量:0x7A0，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlg_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrlg_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrlg_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrlg_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrlg_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrlg_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrlg_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrlg_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_G_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_G_0_taskctrlg_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_G_1_UNION
 结构说明  : TASKCTRL_G_1 寄存器结构定义。地址偏移量:0x7A4，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlg_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrlg_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_G_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_G_1_taskctrlg_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_G_1_taskctrlg_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_G_1_taskctrlg_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_G_1_taskctrlg_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_G_2_UNION
 结构说明  : TASKCTRL_G_2 寄存器结构定义。地址偏移量:0x7A8，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_G_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_G_3_UNION
 结构说明  : TASKCTRL_G_3 寄存器结构定义。地址偏移量:0x7AC，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_G_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_H_0_UNION
 结构说明  : TASKCTRL_H_0 寄存器结构定义。地址偏移量:0x7B0，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlh_dvfs_en                 : 1;  /* bit[0]    :  */
        unsigned int  taskctrlh_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrlh_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrlh_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrlh_invalid_instr_flag      : 1;  /* bit[4]    :  */
        unsigned int  taskctrlh_halt_flag               : 1;  /* bit[5]    :  */
        unsigned int  reserved_0                        : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrlh_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址 */
        unsigned int  taskctrlh_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                        : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_H_0_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_dvfs_en_START                  (0)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_dvfs_en_END                    (0)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_next_step_cfg_START            (1)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_next_step_cfg_END              (1)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_escape_cfg_START               (2)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_escape_cfg_END                 (2)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_halt_flag_START                (5)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_halt_flag_END                  (5)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_cur_fetch_num_START            (20)
#define SOC_PMCTRL_TASKCTRL_H_0_taskctrlh_cur_fetch_num_END              (27)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_H_1_UNION
 结构说明  : TASKCTRL_H_1 寄存器结构定义。地址偏移量:0x7B4，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  taskctrlh_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrlh_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                 : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_H_1_UNION;
#endif
#define SOC_PMCTRL_TASKCTRL_H_1_taskctrlh_cur_task_stat_START     (0)
#define SOC_PMCTRL_TASKCTRL_H_1_taskctrlh_cur_task_stat_END       (7)
#define SOC_PMCTRL_TASKCTRL_H_1_taskctrlh_num_of_end_instr_START  (8)
#define SOC_PMCTRL_TASKCTRL_H_1_taskctrlh_num_of_end_instr_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_H_2_UNION
 结构说明  : TASKCTRL_H_2 寄存器结构定义。地址偏移量:0x7B8，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_H_2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_TASKCTRL_H_3_UNION
 结构说明  : TASKCTRL_H_3 寄存器结构定义。地址偏移量:0x7BC，初值:0x00000000，宽度:32
 寄存器说明: TASKCTRL存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_TASKCTRL_H_3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_A 寄存器结构定义。地址偏移量:0x7C0，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsma_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsma_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsma_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsma_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsma_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsma_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsma_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_A_cofsma_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_B 寄存器结构定义。地址偏移量:0x7C4，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsmb_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmb_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsmb_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsmb_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsmb_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsmb_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsmb_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_B_cofsmb_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_C 寄存器结构定义。地址偏移量:0x7C8，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsmc_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmc_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsmc_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsmc_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsmc_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsmc_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsmc_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_C_cofsmc_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_D 寄存器结构定义。地址偏移量:0x7CC，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsmd_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmd_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsmd_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsmd_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsmd_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsmd_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsmd_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_D_cofsmd_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_E 寄存器结构定义。地址偏移量:0x7D0，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsme_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsme_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsme_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsme_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsme_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsme_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsme_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_E_cofsme_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_F 寄存器结构定义。地址偏移量:0x7D4，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsmf_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmf_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsmf_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsmf_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsmf_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsmf_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsmf_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_F_cofsmf_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_G 寄存器结构定义。地址偏移量:0x7D8，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsmg_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmg_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsmg_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsmg_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsmg_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsmg_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsmg_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_G_cofsmg_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_UNION
 结构说明  : COFSM_VOL_CTRL_SRC_H 寄存器结构定义。地址偏移量:0x7DC，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                       0x0: 来源于TASKCTRLA
                                                                       0x1: 来源于TASKCTRLB
                                                                       0x2: 来源于TASKCTRLC
                                                                       0x3: 来源于TASKCTRLD
                                                                       0x4: 来源于TASKCTRLE
                                                                       0x5: 来源于TASKCTRLF
                                                                       0x6: 来源于TASKCTRLG
                                                                       0x7: 来源于TASKCTRLH
                                                                       Others:不控制 */
        unsigned int  cofsmh_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmh_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsmh_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  cofsmh_mem_vol_ctrl_src_0   : 4;  /* bit[16-19]:  */
        unsigned int  cofsmh_mem_vol_ctrl_src_1   : 4;  /* bit[20-23]:  */
        unsigned int  cofsmh_mem_vol_ctrl_src_2   : 4;  /* bit[24-27]:  */
        unsigned int  cofsmh_mem_vol_ctrl_src_3   : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_logic_vol_ctrl_src_3_END    (15)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_0_START    (16)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_0_END      (19)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_1_START    (20)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_1_END      (23)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_2_START    (24)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_2_END      (27)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_3_START    (28)
#define SOC_PMCTRL_COFSM_VOL_CTRL_SRC_H_cofsmh_mem_vol_ctrl_src_3_END      (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_UNION
 结构说明  : COFSM_FREQ_CTRL_SRC 寄存器结构定义。地址偏移量:0x7E0，初值:0xFFFFFFFF，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_freq_ctrl_src : 4;  /* bit[0-3]  : FREQ的硬件控制来源：
                                                                0x0: 来源于TASKCTRLA
                                                                0x1: 来源于TASKCTRLB
                                                                0x2: 来源于TASKCTRLC
                                                                0x3: 来源于TASKCTRLD
                                                                0x4: 来源于TASKCTRLE
                                                                0x5: 来源于TASKCTRLF
                                                                0x6: 来源于TASKCTRLG
                                                                0x7: 来源于TASKCTRLH
                                                                Others:不控制 */
        unsigned int  cofsmb_freq_ctrl_src : 4;  /* bit[4-7]  :  */
        unsigned int  cofsmc_freq_ctrl_src : 4;  /* bit[8-11] :  */
        unsigned int  cofsmd_freq_ctrl_src : 4;  /* bit[12-15]:  */
        unsigned int  cofsme_freq_ctrl_src : 4;  /* bit[16-19]:  */
        unsigned int  cofsmf_freq_ctrl_src : 4;  /* bit[20-23]:  */
        unsigned int  cofsmg_freq_ctrl_src : 4;  /* bit[24-27]:  */
        unsigned int  cofsmh_freq_ctrl_src : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_UNION;
#endif
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsma_freq_ctrl_src_START  (0)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsma_freq_ctrl_src_END    (3)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmb_freq_ctrl_src_START  (4)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmb_freq_ctrl_src_END    (7)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmc_freq_ctrl_src_START  (8)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmc_freq_ctrl_src_END    (11)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmd_freq_ctrl_src_START  (12)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmd_freq_ctrl_src_END    (15)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsme_freq_ctrl_src_START  (16)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsme_freq_ctrl_src_END    (19)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmf_freq_ctrl_src_START  (20)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmf_freq_ctrl_src_END    (23)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmg_freq_ctrl_src_START  (24)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmg_freq_ctrl_src_END    (27)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmh_freq_ctrl_src_START  (28)
#define SOC_PMCTRL_COFSM_FREQ_CTRL_SRC_cofsmh_freq_ctrl_src_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_VOL_SENDOUT_UNION
 结构说明  : COFSM_VOL_SENDOUT 寄存器结构定义。地址偏移量:0x7E4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL SENDOUT
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0               : 1;  /* bit[0]    :  */
        unsigned int  reserved_1               : 1;  /* bit[1]    :  */
        unsigned int  reserved_2               : 1;  /* bit[2]    :  */
        unsigned int  reserved_3               : 1;  /* bit[3]    :  */
        unsigned int  reserved_4               : 1;  /* bit[4]    :  */
        unsigned int  reserved_5               : 1;  /* bit[5]    :  */
        unsigned int  reserved_6               : 1;  /* bit[6]    :  */
        unsigned int  reserved_7               : 1;  /* bit[7]    :  */
        unsigned int  cofsme_logic_vol_sendout : 1;  /* bit[8]    : 1'b0: COFSMA的logic调压使用COFSM自己的电压仲裁
                                                                    1'b1: COFSMA的logic调压不使用COFSMA的内部电压仲裁机制仲裁，使用外部逻辑仲裁。 */
        unsigned int  reserved_8               : 1;  /* bit[9]    :  */
        unsigned int  cofsmf_logic_vol_sendout : 1;  /* bit[10]   :  */
        unsigned int  reserved_9               : 1;  /* bit[11]   :  */
        unsigned int  cofsmg_logic_vol_sendout : 1;  /* bit[12]   :  */
        unsigned int  reserved_10              : 1;  /* bit[13]   :  */
        unsigned int  cofsmh_logic_vol_sendout : 1;  /* bit[14]   :  */
        unsigned int  reserved_11              : 1;  /* bit[15]   :  */
        unsigned int  reserved_12              : 16; /* bit[16-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_VOL_SENDOUT_UNION;
#endif
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsme_logic_vol_sendout_START  (8)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsme_logic_vol_sendout_END    (8)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsmf_logic_vol_sendout_START  (10)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsmf_logic_vol_sendout_END    (10)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsmg_logic_vol_sendout_START  (12)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsmg_logic_vol_sendout_END    (12)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsmh_logic_vol_sendout_START  (14)
#define SOC_PMCTRL_COFSM_VOL_SENDOUT_cofsmh_logic_vol_sendout_END    (14)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_A_0 寄存器结构定义。地址偏移量:0x800，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsma_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsma_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsma_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsma_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsma_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsma_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsma_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsma_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsma_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsma_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_0_cofsma_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_A_1 寄存器结构定义。地址偏移量:0x804，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsma_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsma_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsma_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsma_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsma_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_A_1_cofsma_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_B_0 寄存器结构定义。地址偏移量:0x808，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsmb_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsmb_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsmb_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsmb_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsmb_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsmb_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsmb_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsmb_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsmb_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsmb_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_0_cofsmb_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_B_1 寄存器结构定义。地址偏移量:0x80C，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsmb_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsmb_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsmb_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsmb_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsmb_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_B_1_cofsmb_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_C_0 寄存器结构定义。地址偏移量:0x810，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsmc_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsmc_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsmc_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsmc_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsmc_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsmc_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsmc_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsmc_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsmc_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsmc_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_0_cofsmc_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_C_1 寄存器结构定义。地址偏移量:0x814，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsmc_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsmc_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsmc_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsmc_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsmc_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_C_1_cofsmc_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_D_0 寄存器结构定义。地址偏移量:0x818，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsmd_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsmd_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsmd_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsmd_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsmd_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsmd_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsmd_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsmd_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsmd_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsmd_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_0_cofsmd_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_D_1 寄存器结构定义。地址偏移量:0x81C，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsmd_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsmd_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsmd_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsmd_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsmd_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_D_1_cofsmd_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_E_0 寄存器结构定义。地址偏移量:0x820，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsme_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsme_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsme_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsme_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsme_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsme_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsme_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsme_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsme_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsme_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_0_cofsme_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_E_1 寄存器结构定义。地址偏移量:0x824，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsme_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsme_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsme_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsme_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsme_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_E_1_cofsme_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_F_0 寄存器结构定义。地址偏移量:0x828，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsmf_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsmf_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsmf_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsmf_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsmf_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsmf_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsmf_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsmf_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsmf_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsmf_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_0_cofsmf_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_F_1 寄存器结构定义。地址偏移量:0x82C，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsmf_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsmf_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsmf_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsmf_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsmf_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_F_1_cofsmf_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_G_0 寄存器结构定义。地址偏移量:0x830，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsmg_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsmg_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsmg_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsmg_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsmg_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsmg_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsmg_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsmg_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsmg_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsmg_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_0_cofsmg_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_G_1 寄存器结构定义。地址偏移量:0x834，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsmg_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsmg_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsmg_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsmg_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsmg_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_G_1_cofsmg_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_H_0 寄存器结构定义。地址偏移量:0x838，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsmh_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsmh_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsmh_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsmh_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsmh_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsmh_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsmh_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsmh_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsmh_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsmh_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_sw2apll_req_START              (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_sw2apll_req_END                (18)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_pll_sw_stat_START              (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_pll_sw_stat_END                (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_0_cofsmh_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_UNION
 结构说明  : COFSM_CLK_DIV_CTRL_H_1 寄存器结构定义。地址偏移量:0x83C，初值:0x00000000，宽度:32
 寄存器说明: COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_cpu_div_stat       : 16; /* bit[0-15] :  */
        unsigned int  cofsmh_vdm_div_stat       : 4;  /* bit[16-19]:  */
        unsigned int  cofsmh_vdm_div_sft_idx    : 4;  /* bit[20-23]:  */
        unsigned int  cofsmh_vdm_div_sft_cfg    : 1;  /* bit[24]   :  */
        unsigned int  cofsmh_cur_clk_src_stat   : 3;  /* bit[25-27]:  */
        unsigned int  cofsmh_cur_apll_freq_stat : 3;  /* bit[28-30]:  */
        unsigned int  reserved                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_cpu_div_stat_START        (0)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_cpu_div_stat_END          (15)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_vdm_div_stat_START        (16)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_vdm_div_stat_END          (19)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_vdm_div_sft_idx_START     (20)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_vdm_div_sft_idx_END       (23)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_vdm_div_sft_cfg_START     (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_vdm_div_sft_cfg_END       (24)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_cur_clk_src_stat_START    (25)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_cur_clk_src_stat_END      (27)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_cur_apll_freq_stat_START  (28)
#define SOC_PMCTRL_COFSM_CLK_DIV_CTRL_H_1_cofsmh_cur_apll_freq_stat_END    (30)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_A_0 寄存器结构定义。地址偏移量:0x840，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsma_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsma_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_0_cofsma_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_A_1 寄存器结构定义。地址偏移量:0x844，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsma_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsma_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsma_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsma_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsma_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_1_cofsma_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_A_2 寄存器结构定义。地址偏移量:0x848，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0               : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1               : 1;  /* bit[8]    :  */
        unsigned int  reserved_2               : 1;  /* bit[9]    :  */
        unsigned int  reserved_3               : 2;  /* bit[10-11]:  */
        unsigned int  reserved_4               : 8;  /* bit[12-19]:  */
        unsigned int  reserved_5               : 1;  /* bit[20]   :  */
        unsigned int  reserved_6               : 1;  /* bit[21]   :  */
        unsigned int  reserved_7               : 2;  /* bit[22-23]:  */
        unsigned int  cofsma_logic_cur_max_vol : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_cofsma_logic_cur_max_vol_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_2_cofsma_logic_cur_max_vol_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_A_3 寄存器结构定义。地址偏移量:0x84C，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsma_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsma_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_A_3_cofsma_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_A_0 寄存器结构定义。地址偏移量:0x850，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsma_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsma_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_0_cofsma_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_A_1 寄存器结构定义。地址偏移量:0x854，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsma_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsma_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsma_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsma_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsma_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_1_cofsma_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_A_2 寄存器结构定义。地址偏移量:0x858，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsma_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsma_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsma_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsma_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsma_mem_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_soft_mem_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_mem_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_2_cofsma_mem_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_A_3 寄存器结构定义。地址偏移量:0x85C，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsma_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsma_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsma_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_A_3_cofsma_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_B_0 寄存器结构定义。地址偏移量:0x860，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmb_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsmb_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmb_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_0_cofsmb_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_B_1 寄存器结构定义。地址偏移量:0x864，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmb_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmb_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmb_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmb_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmb_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmb_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_1_cofsmb_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_B_2 寄存器结构定义。地址偏移量:0x868，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0               : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1               : 1;  /* bit[8]    :  */
        unsigned int  reserved_2               : 1;  /* bit[9]    :  */
        unsigned int  reserved_3               : 2;  /* bit[10-11]:  */
        unsigned int  reserved_4               : 8;  /* bit[12-19]:  */
        unsigned int  reserved_5               : 1;  /* bit[20]   :  */
        unsigned int  reserved_6               : 1;  /* bit[21]   :  */
        unsigned int  reserved_7               : 2;  /* bit[22-23]:  */
        unsigned int  cofsmb_logic_cur_max_vol : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_cofsmb_logic_cur_max_vol_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_2_cofsmb_logic_cur_max_vol_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_B_3 寄存器结构定义。地址偏移量:0x86C，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmb_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmb_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmb_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_B_3_cofsmb_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_B_0 寄存器结构定义。地址偏移量:0x870，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmb_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsmb_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmb_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_0_cofsmb_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_B_1 寄存器结构定义。地址偏移量:0x874，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmb_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmb_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmb_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmb_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmb_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmb_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_1_cofsmb_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_B_2 寄存器结构定义。地址偏移量:0x878，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0             : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1             : 1;  /* bit[8]    :  */
        unsigned int  reserved_2             : 1;  /* bit[9]    :  */
        unsigned int  reserved_3             : 2;  /* bit[10-11]:  */
        unsigned int  reserved_4             : 8;  /* bit[12-19]:  */
        unsigned int  reserved_5             : 1;  /* bit[20]   :  */
        unsigned int  reserved_6             : 1;  /* bit[21]   :  */
        unsigned int  reserved_7             : 2;  /* bit[22-23]:  */
        unsigned int  cofsmb_mem_cur_max_vol : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_cofsmb_mem_cur_max_vol_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_2_cofsmb_mem_cur_max_vol_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_B_3 寄存器结构定义。地址偏移量:0x87C，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmb_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmb_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmb_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_B_3_cofsmb_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_C_0 寄存器结构定义。地址偏移量:0x880，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsmc_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmc_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_0_cofsmc_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_C_1 寄存器结构定义。地址偏移量:0x884，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmc_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmc_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmc_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmc_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmc_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_1_cofsmc_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_C_2 寄存器结构定义。地址偏移量:0x888，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0               : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1               : 1;  /* bit[8]    :  */
        unsigned int  reserved_2               : 1;  /* bit[9]    :  */
        unsigned int  reserved_3               : 2;  /* bit[10-11]:  */
        unsigned int  reserved_4               : 8;  /* bit[12-19]:  */
        unsigned int  reserved_5               : 1;  /* bit[20]   :  */
        unsigned int  reserved_6               : 1;  /* bit[21]   :  */
        unsigned int  reserved_7               : 2;  /* bit[22-23]:  */
        unsigned int  cofsmc_logic_cur_max_vol : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_cofsmc_logic_cur_max_vol_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_2_cofsmc_logic_cur_max_vol_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_C_3 寄存器结构定义。地址偏移量:0x88C，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmc_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmc_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_C_3_cofsmc_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_C_0 寄存器结构定义。地址偏移量:0x890，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsmc_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmc_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_0_cofsmc_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_C_1 寄存器结构定义。地址偏移量:0x894，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmc_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmc_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmc_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmc_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmc_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_1_cofsmc_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_C_2 寄存器结构定义。地址偏移量:0x898，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmc_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmc_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmc_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmc_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmc_mem_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_soft_mem_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_mem_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_2_cofsmc_mem_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_C_3 寄存器结构定义。地址偏移量:0x89C，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmc_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmc_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmc_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_C_3_cofsmc_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_D_0 寄存器结构定义。地址偏移量:0x8A0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsmd_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmd_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_0_cofsmd_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_D_1 寄存器结构定义。地址偏移量:0x8A4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmd_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmd_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmd_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmd_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmd_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_1_cofsmd_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_D_2 寄存器结构定义。地址偏移量:0x8A8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_soft_logic_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_soft_logic_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmd_soft_logic_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmd_soft_logic_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmd_soft_logic_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmd_soft_logic_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmd_logic_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_soft_logic_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_logic_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_2_cofsmd_logic_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_D_3 寄存器结构定义。地址偏移量:0x8AC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmd_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmd_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_D_3_cofsmd_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_D_0 寄存器结构定义。地址偏移量:0x8B0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsmd_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmd_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_0_cofsmd_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_D_1 寄存器结构定义。地址偏移量:0x8B4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmd_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmd_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmd_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmd_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmd_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_1_cofsmd_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_D_2 寄存器结构定义。地址偏移量:0x8B8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmd_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmd_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmd_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmd_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmd_mem_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_soft_mem_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_mem_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_2_cofsmd_mem_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_D_3 寄存器结构定义。地址偏移量:0x8BC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmd_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmd_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmd_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_D_3_cofsmd_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_E_0 寄存器结构定义。地址偏移量:0x8C0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsme_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsme_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsme_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_0_cofsme_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_E_1 寄存器结构定义。地址偏移量:0x8C4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsme_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsme_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsme_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsme_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsme_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsme_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_1_cofsme_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_E_2 寄存器结构定义。地址偏移量:0x8C8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0               : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1               : 1;  /* bit[8]    :  */
        unsigned int  reserved_2               : 1;  /* bit[9]    :  */
        unsigned int  reserved_3               : 2;  /* bit[10-11]:  */
        unsigned int  reserved_4               : 8;  /* bit[12-19]:  */
        unsigned int  reserved_5               : 1;  /* bit[20]   :  */
        unsigned int  reserved_6               : 1;  /* bit[21]   :  */
        unsigned int  reserved_7               : 2;  /* bit[22-23]:  */
        unsigned int  cofsme_logic_cur_max_vol : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_cofsme_logic_cur_max_vol_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_2_cofsme_logic_cur_max_vol_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_E_3 寄存器结构定义。地址偏移量:0x8CC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsme_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsme_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsme_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_E_3_cofsme_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_E_0 寄存器结构定义。地址偏移量:0x8D0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsme_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsme_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsme_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_0_cofsme_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_E_1 寄存器结构定义。地址偏移量:0x8D4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsme_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsme_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsme_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsme_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsme_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsme_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_1_cofsme_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_E_2 寄存器结构定义。地址偏移量:0x8D8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0             : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1             : 1;  /* bit[8]    :  */
        unsigned int  reserved_2             : 1;  /* bit[9]    :  */
        unsigned int  reserved_3             : 2;  /* bit[10-11]:  */
        unsigned int  reserved_4             : 8;  /* bit[12-19]:  */
        unsigned int  reserved_5             : 1;  /* bit[20]   :  */
        unsigned int  reserved_6             : 1;  /* bit[21]   :  */
        unsigned int  reserved_7             : 2;  /* bit[22-23]:  */
        unsigned int  cofsme_mem_cur_max_vol : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_cofsme_mem_cur_max_vol_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_2_cofsme_mem_cur_max_vol_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_E_3 寄存器结构定义。地址偏移量:0x8DC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsme_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsme_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsme_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_E_3_cofsme_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_F_0 寄存器结构定义。地址偏移量:0x8E0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsmf_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmf_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_0_cofsmf_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_F_1 寄存器结构定义。地址偏移量:0x8E4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmf_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmf_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmf_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmf_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmf_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_1_cofsmf_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_F_2 寄存器结构定义。地址偏移量:0x8E8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_soft_logic_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_soft_logic_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmf_soft_logic_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmf_soft_logic_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmf_soft_logic_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmf_soft_logic_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmf_logic_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_soft_logic_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_logic_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_2_cofsmf_logic_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_F_3 寄存器结构定义。地址偏移量:0x8EC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmf_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmf_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_F_3_cofsmf_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_F_0 寄存器结构定义。地址偏移量:0x8F0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsmf_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmf_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_0_cofsmf_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_F_1 寄存器结构定义。地址偏移量:0x8F4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmf_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmf_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmf_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmf_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmf_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_1_cofsmf_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_F_2 寄存器结构定义。地址偏移量:0x8F8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmf_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmf_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmf_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmf_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmf_mem_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_soft_mem_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_mem_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_2_cofsmf_mem_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_F_3 寄存器结构定义。地址偏移量:0x8FC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmf_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmf_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmf_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_F_3_cofsmf_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_G_0 寄存器结构定义。地址偏移量:0x9A0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsmg_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmg_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_0_cofsmg_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_G_1 寄存器结构定义。地址偏移量:0x9A4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmg_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmg_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmg_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmg_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmg_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_1_cofsmg_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_G_2 寄存器结构定义。地址偏移量:0x9A8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_soft_logic_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_soft_logic_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmg_soft_logic_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmg_soft_logic_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmg_soft_logic_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmg_soft_logic_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmg_logic_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_soft_logic_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_logic_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_2_cofsmg_logic_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_G_3 寄存器结构定义。地址偏移量:0x9AC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmg_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmg_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_G_3_cofsmg_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_G_0 寄存器结构定义。地址偏移量:0x9B0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsmg_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmg_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_0_cofsmg_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_G_1 寄存器结构定义。地址偏移量:0x9B4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmg_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmg_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmg_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmg_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmg_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_1_cofsmg_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_G_2 寄存器结构定义。地址偏移量:0x9B8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmg_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmg_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmg_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmg_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmg_mem_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_soft_mem_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_mem_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_2_cofsmg_mem_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_G_3 寄存器结构定义。地址偏移量:0x9BC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmg_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmg_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmg_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_G_3_cofsmg_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_H_0 寄存器结构定义。地址偏移量:0x9C0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_initial_pmu_logic_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_initial_pmu_logic_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[9-11] :  */
        unsigned int  cofsmh_cur_pmu_logic_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmh_logic_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                       : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_initial_pmu_logic_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_initial_pmu_logic_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_initial_pmu_logic_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_initial_pmu_logic_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_cur_pmu_logic_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_cur_pmu_logic_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_logic_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_0_cofsmh_logic_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_H_1 寄存器结构定义。地址偏移量:0x9C4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmh_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmh_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmh_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmh_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmh_logic_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_soft_logic_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_logic_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_1_cofsmh_logic_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_H_2 寄存器结构定义。地址偏移量:0x9C8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_soft_logic_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_soft_logic_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmh_soft_logic_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                      : 2;  /* bit[10-11]:  */
        unsigned int  cofsmh_soft_logic_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmh_soft_logic_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmh_soft_logic_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 2;  /* bit[22-23]:  */
        unsigned int  cofsmh_logic_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_soft_logic_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_logic_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_2_cofsmh_logic_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_UNION
 结构说明  : COFSM_LOGIC_VOL_CTRL_H_3 寄存器结构定义。地址偏移量:0x9CC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_cur_pmu_logic_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_cur_pmu_logic_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmh_cur_pmu_logic_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmh_cur_pmu_logic_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_LOGIC_VOL_CTRL_H_3_cofsmh_cur_pmu_logic_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_H_0 寄存器结构定义。地址偏移量:0x9D0，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_initial_pmu_mem_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_initial_pmu_mem_vol_req : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                     : 3;  /* bit[9-11] :  */
        unsigned int  cofsmh_cur_pmu_mem_vol_stat    : 8;  /* bit[12-19]:  */
        unsigned int  cofsmh_mem_cur_vau_stat        : 3;  /* bit[20-22]:  */
        unsigned int  reserved_1                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_initial_pmu_mem_vol_idx_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_initial_pmu_mem_vol_idx_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_initial_pmu_mem_vol_req_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_initial_pmu_mem_vol_req_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_cur_pmu_mem_vol_stat_START     (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_cur_pmu_mem_vol_stat_END       (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_mem_cur_vau_stat_START         (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_0_cofsmh_mem_cur_vau_stat_END           (22)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_H_1 寄存器结构定义。地址偏移量:0x9D4，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsmh_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmh_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmh_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsmh_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmh_mem_nxt_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_soft_mem_vol_chg_pls_1_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_mem_nxt_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_1_cofsmh_mem_nxt_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_H_2 寄存器结构定义。地址偏移量:0x9D8，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsmh_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                    : 2;  /* bit[10-11]:  */
        unsigned int  cofsmh_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsmh_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsmh_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                    : 2;  /* bit[22-23]:  */
        unsigned int  cofsmh_mem_cur_max_vol        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_soft_mem_vol_chg_pls_3_END    (21)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_mem_cur_max_vol_START         (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_2_cofsmh_mem_cur_max_vol_END           (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_UNION
 结构说明  : COFSM_MEM_VOL_CTRL_H_3 寄存器结构定义。地址偏移量:0x9DC，初值:0x00000000，宽度:32
 寄存器说明: COFSM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_cur_pmu_mem_vol_req_stat_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsmh_cur_pmu_mem_vol_req_stat_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsmh_cur_pmu_mem_vol_req_stat_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsmh_cur_pmu_mem_vol_req_stat_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_UNION;
#endif
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_0_START  (0)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_0_END    (7)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_1_START  (8)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_1_END    (15)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_2_START  (16)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_2_END    (23)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_3_START  (24)
#define SOC_PMCTRL_COFSM_MEM_VOL_CTRL_H_3_cofsmh_cur_pmu_mem_vol_req_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_A_0 寄存器结构定义。地址偏移量:0x670，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsma_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk（hold_time和step_time不能配置为0） */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_cofsma_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_cofsma_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_cofsma_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_0_cofsma_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_A_1 寄存器结构定义。地址偏移量:0x674，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsma_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_cofsma_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_cofsma_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_cofsma_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_A_1_cofsma_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_A_0 寄存器结构定义。地址偏移量:0x678，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsma_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_cofsma_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_cofsma_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_cofsma_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_0_cofsma_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_A_1 寄存器结构定义。地址偏移量:0x67C，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsma_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_cofsma_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_cofsma_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_cofsma_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_A_1_cofsma_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_B_0 寄存器结构定义。地址偏移量:0x680，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmb_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_cofsmb_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_cofsmb_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_cofsmb_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_0_cofsmb_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_B_1 寄存器结构定义。地址偏移量:0x684，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmb_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_cofsmb_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_cofsmb_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_cofsmb_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_B_1_cofsmb_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_B_0 寄存器结构定义。地址偏移量:0x688，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmb_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_cofsmb_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_cofsmb_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_cofsmb_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_0_cofsmb_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_B_1 寄存器结构定义。地址偏移量:0x68C，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmb_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_cofsmb_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_cofsmb_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_cofsmb_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_B_1_cofsmb_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_C_0 寄存器结构定义。地址偏移量:0x690，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmc_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_cofsmc_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_cofsmc_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_cofsmc_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_0_cofsmc_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_C_1 寄存器结构定义。地址偏移量:0x694，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmc_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_cofsmc_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_cofsmc_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_cofsmc_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_C_1_cofsmc_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_C_0 寄存器结构定义。地址偏移量:0x698，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmc_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_cofsmc_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_cofsmc_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_cofsmc_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_0_cofsmc_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_C_1 寄存器结构定义。地址偏移量:0x69C，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmc_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_cofsmc_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_cofsmc_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_cofsmc_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_C_1_cofsmc_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_D_0 寄存器结构定义。地址偏移量:0x6A0，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmd_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_cofsmd_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_cofsmd_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_cofsmd_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_0_cofsmd_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_D_1 寄存器结构定义。地址偏移量:0x6A4，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmd_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_cofsmd_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_cofsmd_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_cofsmd_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_D_1_cofsmd_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_D_0 寄存器结构定义。地址偏移量:0x6A8，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmd_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_cofsmd_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_cofsmd_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_cofsmd_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_0_cofsmd_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_D_1 寄存器结构定义。地址偏移量:0x6AC，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmd_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_cofsmd_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_cofsmd_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_cofsmd_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_D_1_cofsmd_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_E_0 寄存器结构定义。地址偏移量:0x6B0，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsme_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_cofsme_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_cofsme_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_cofsme_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_0_cofsme_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_E_1 寄存器结构定义。地址偏移量:0x6B4，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsme_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_cofsme_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_cofsme_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_cofsme_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_E_1_cofsme_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_E_0 寄存器结构定义。地址偏移量:0x6B8，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsme_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_cofsme_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_cofsme_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_cofsme_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_0_cofsme_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_E_1 寄存器结构定义。地址偏移量:0x6BC，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsme_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_cofsme_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_cofsme_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_cofsme_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_E_1_cofsme_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_F_0 寄存器结构定义。地址偏移量:0x6C0，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmf_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_cofsmf_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_cofsmf_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_cofsmf_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_0_cofsmf_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_F_1 寄存器结构定义。地址偏移量:0x6C4，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmf_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_cofsmf_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_cofsmf_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_cofsmf_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_F_1_cofsmf_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_F_0 寄存器结构定义。地址偏移量:0x6C8，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmf_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_cofsmf_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_cofsmf_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_cofsmf_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_0_cofsmf_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_F_1 寄存器结构定义。地址偏移量:0x6CC，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmf_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_cofsmf_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_cofsmf_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_cofsmf_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_F_1_cofsmf_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_G_0 寄存器结构定义。地址偏移量:0x6D0，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmg_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_cofsmg_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_cofsmg_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_cofsmg_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_0_cofsmg_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_G_1 寄存器结构定义。地址偏移量:0x6D4，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmg_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_cofsmg_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_cofsmg_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_cofsmg_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_G_1_cofsmg_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_G_0 寄存器结构定义。地址偏移量:0x6D8，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmg_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_cofsmg_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_cofsmg_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_cofsmg_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_0_cofsmg_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_G_1 寄存器结构定义。地址偏移量:0x6DC，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmg_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_cofsmg_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_cofsmg_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_cofsmg_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_G_1_cofsmg_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_H_0 寄存器结构定义。地址偏移量:0x6E0，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_pmu_logic_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmh_pmu_logic_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_cofsmh_pmu_logic_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_cofsmh_pmu_logic_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_cofsmh_pmu_logic_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_0_cofsmh_pmu_logic_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_UNION
 结构说明  : COFSM_PMU_LOGIC_TIME_H_1 寄存器结构定义。地址偏移量:0x6E4，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_pmu_logic_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmh_pmu_logic_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_cofsmh_pmu_logic_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_cofsmh_pmu_logic_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_cofsmh_pmu_logic_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_LOGIC_TIME_H_1_cofsmh_pmu_logic_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_UNION
 结构说明  : COFSM_PMU_MEM_TIME_H_0 寄存器结构定义。地址偏移量:0x6E8，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_pmu_mem_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmh_pmu_mem_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_cofsmh_pmu_mem_up_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_cofsmh_pmu_mem_up_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_cofsmh_pmu_mem_up_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_0_cofsmh_pmu_mem_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_UNION
 结构说明  : COFSM_PMU_MEM_TIME_H_1 寄存器结构定义。地址偏移量:0x6EC，初值:0x055002A0，宽度:32
 寄存器说明: COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_pmu_mem_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsmh_pmu_mem_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_UNION;
#endif
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_cofsmh_pmu_mem_dn_hold_time_START  (0)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_cofsmh_pmu_mem_dn_hold_time_END    (19)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_cofsmh_pmu_mem_dn_step_time_START  (20)
#define SOC_PMCTRL_COFSM_PMU_MEM_TIME_H_1_cofsmh_pmu_mem_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_UNION
 结构说明  : PERI_VOTE_VOL_IDX_0 寄存器结构定义。地址偏移量:0xA00，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源0电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_0 : 32; /* bit[0-31]: 电压参数分为四档：
                                                              bit[7:0] 对应 0.6 v profile档位；
                                                              bit[15:8] 对应 0.65v profile档位；
                                                              bit[23:16] 对应 0.7 v profile档位；
                                                              bit[31:24] 对应 0.8 v profile档位； */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_peri_vote_vol_idx_0_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_0_peri_vote_vol_idx_0_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_UNION
 结构说明  : PERI_VOTE_VOL_IDX_1 寄存器结构定义。地址偏移量:0xA04，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源1电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_1 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_peri_vote_vol_idx_1_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_1_peri_vote_vol_idx_1_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_UNION
 结构说明  : PERI_VOTE_VOL_IDX_2 寄存器结构定义。地址偏移量:0xA08，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源2电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_2 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_peri_vote_vol_idx_2_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_2_peri_vote_vol_idx_2_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_UNION
 结构说明  : PERI_VOTE_VOL_IDX_3 寄存器结构定义。地址偏移量:0xA0C，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源3电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_3 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_peri_vote_vol_idx_3_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_3_peri_vote_vol_idx_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_UNION
 结构说明  : PERI_VOTE_VOL_IDX_4 寄存器结构定义。地址偏移量:0xA10，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源4电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_4 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_peri_vote_vol_idx_4_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_4_peri_vote_vol_idx_4_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_UNION
 结构说明  : PERI_VOTE_VOL_IDX_5 寄存器结构定义。地址偏移量:0xA14，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源5电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_5 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_peri_vote_vol_idx_5_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_5_peri_vote_vol_idx_5_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_UNION
 结构说明  : PERI_VOTE_VOL_IDX_6 寄存器结构定义。地址偏移量:0xA18，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源6电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_6 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_peri_vote_vol_idx_6_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_6_peri_vote_vol_idx_6_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_UNION
 结构说明  : PERI_VOTE_VOL_IDX_7 寄存器结构定义。地址偏移量:0xA1C，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源7电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_7 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_peri_vote_vol_idx_7_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_7_peri_vote_vol_idx_7_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_UNION
 结构说明  : PERI_VOTE_VOL_IDX_8 寄存器结构定义。地址偏移量:0xA20，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源8电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_8 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_peri_vote_vol_idx_8_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_8_peri_vote_vol_idx_8_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_UNION
 结构说明  : PERI_VOTE_VOL_IDX_9 寄存器结构定义。地址偏移量:0xA24，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源9电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_9 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_peri_vote_vol_idx_9_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_9_peri_vote_vol_idx_9_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_UNION
 结构说明  : PERI_VOTE_VOL_IDX_10 寄存器结构定义。地址偏移量:0xA28，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源10电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_10 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_peri_vote_vol_idx_10_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_10_peri_vote_vol_idx_10_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_UNION
 结构说明  : PERI_VOTE_VOL_IDX_11 寄存器结构定义。地址偏移量:0xA2C，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源11电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_11 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_peri_vote_vol_idx_11_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_11_peri_vote_vol_idx_11_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_UNION
 结构说明  : PERI_VOTE_VOL_IDX_12 寄存器结构定义。地址偏移量:0xA30，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源12电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_12 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_peri_vote_vol_idx_12_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_12_peri_vote_vol_idx_12_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_UNION
 结构说明  : PERI_VOTE_VOL_IDX_13 寄存器结构定义。地址偏移量:0xA34，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源13电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_13 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_peri_vote_vol_idx_13_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_13_peri_vote_vol_idx_13_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_UNION
 结构说明  : PERI_VOTE_VOL_IDX_14 寄存器结构定义。地址偏移量:0xA38，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源14电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_14 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_peri_vote_vol_idx_14_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_14_peri_vote_vol_idx_14_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_UNION
 结构说明  : PERI_VOTE_VOL_IDX_15 寄存器结构定义。地址偏移量:0xA3C，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源15电压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_idx_15 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_peri_vote_vol_idx_15_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_IDX_15_peri_vote_vol_idx_15_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION
 结构说明  : PERI_VOTE_VOL_RANK_SFT_0 寄存器结构定义。地址偏移量:0x000，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源电压档位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_rank_0 : 2;  /* bit[0-1]  : PERI 投票源0电压档位
                                                                b'00 对应 0.6 v profile档位；
                                                                b'01 对应 0.65v profile档位；
                                                                b'10 对应 0.7 v profile档位；
                                                                b'11 对应 0.8 v profile档位； */
        unsigned int  peri_vote_vol_rank_1 : 2;  /* bit[2-3]  :  */
        unsigned int  peri_vote_vol_rank_2 : 2;  /* bit[4-5]  :  */
        unsigned int  peri_vote_vol_rank_3 : 2;  /* bit[6-7]  :  */
        unsigned int  peri_vote_vol_rank_4 : 2;  /* bit[8-9]  :  */
        unsigned int  peri_vote_vol_rank_5 : 2;  /* bit[10-11]:  */
        unsigned int  peri_vote_vol_rank_6 : 2;  /* bit[12-13]:  */
        unsigned int  peri_vote_vol_rank_7 : 2;  /* bit[14-15]:  */
        unsigned int  reserved             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_0_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_0_END    (1)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_1_START  (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_1_END    (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_2_START  (4)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_2_END    (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_3_START  (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_3_END    (7)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_4_START  (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_4_END    (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_5_START  (10)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_5_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_6_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_6_END    (13)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_7_START  (14)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_7_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION
 结构说明  : PERI_VOTE_VOL_RANK_SFT_1 寄存器结构定义。地址偏移量:0x004，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源电压档位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_rank_8  : 2;  /* bit[0-1]  : PERI 投票源8电压档位
                                                                 b‘00 对应 0.6 v profile档位；
                                                                 b’01 对应 0.65v profile档位；
                                                                 b‘10 对应 0.7 v profile档位；
                                                                 b’11 对应 0.8 v profile档位； */
        unsigned int  peri_vote_vol_rank_9  : 2;  /* bit[2-3]  :  */
        unsigned int  peri_vote_vol_rank_10 : 2;  /* bit[4-5]  :  */
        unsigned int  peri_vote_vol_rank_11 : 2;  /* bit[6-7]  :  */
        unsigned int  peri_vote_vol_rank_12 : 2;  /* bit[8-9]  :  */
        unsigned int  peri_vote_vol_rank_13 : 2;  /* bit[10-11]:  */
        unsigned int  peri_vote_vol_rank_14 : 2;  /* bit[12-13]:  */
        unsigned int  peri_vote_vol_rank_15 : 2;  /* bit[14-15]:  */
        unsigned int  reserved              : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_8_START   (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_8_END     (1)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_9_START   (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_9_END     (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_10_START  (4)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_10_END    (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_11_START  (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_11_END    (7)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_12_START  (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_12_END    (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_13_START  (10)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_13_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_14_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_14_END    (13)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_15_START  (14)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_15_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION
 结构说明  : PERI_VOTE_VOL_VALID 寄存器结构定义。地址偏移量:0x008，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源有效状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_vol_valid_0  : 1;  /* bit[0]    : PERI 投票源0有效状态
                                                                  b‘1 参与投票有效
                                                                  b’0 不参与投票无效 */
        unsigned int  peri_vote_vol_valid_1  : 1;  /* bit[1]    :  */
        unsigned int  peri_vote_vol_valid_2  : 1;  /* bit[2]    :  */
        unsigned int  peri_vote_vol_valid_3  : 1;  /* bit[3]    :  */
        unsigned int  peri_vote_vol_valid_4  : 1;  /* bit[4]    :  */
        unsigned int  peri_vote_vol_valid_5  : 1;  /* bit[5]    :  */
        unsigned int  peri_vote_vol_valid_6  : 1;  /* bit[6]    :  */
        unsigned int  peri_vote_vol_valid_7  : 1;  /* bit[7]    :  */
        unsigned int  peri_vote_vol_valid_8  : 1;  /* bit[8]    :  */
        unsigned int  peri_vote_vol_valid_9  : 1;  /* bit[9]    :  */
        unsigned int  peri_vote_vol_valid_10 : 1;  /* bit[10]   :  */
        unsigned int  peri_vote_vol_valid_11 : 1;  /* bit[11]   :  */
        unsigned int  peri_vote_vol_valid_12 : 1;  /* bit[12]   :  */
        unsigned int  peri_vote_vol_valid_13 : 1;  /* bit[13]   :  */
        unsigned int  peri_vote_vol_valid_14 : 1;  /* bit[14]   :  */
        unsigned int  peri_vote_vol_valid_15 : 1;  /* bit[15]   :  */
        unsigned int  reserved               : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_0_START   (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_0_END     (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_1_START   (1)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_1_END     (1)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_2_START   (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_2_END     (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_3_START   (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_3_END     (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_4_START   (4)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_4_END     (4)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_5_START   (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_5_END     (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_6_START   (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_6_END     (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_7_START   (7)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_7_END     (7)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_8_START   (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_8_END     (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_9_START   (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_9_END     (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_10_START  (10)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_10_END    (10)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_11_START  (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_11_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_12_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_12_END    (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_13_START  (13)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_13_END    (13)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_14_START  (14)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_14_END    (14)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_15_START  (15)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_15_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_SFT_CHG_UNION
 结构说明  : PERI_VOTE_SFT_CHG 寄存器结构定义。地址偏移量:0x00C，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源软件调压请求
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_sft_chg_0     : 1;  /* bit[0]    : software 调压请求脉冲有效信号，投票源0 */
        unsigned int  peri_vote_sft_chg_1     : 1;  /* bit[1]    : software 调压请求脉冲有效信号，投票源1 */
        unsigned int  peri_vote_sft_chg_2     : 1;  /* bit[2]    : software 调压请求脉冲有效信号，投票源2 */
        unsigned int  peri_vote_sft_chg_3     : 1;  /* bit[3]    : software 调压请求脉冲有效信号，投票源3 */
        unsigned int  peri_vote_sft_chg_4     : 1;  /* bit[4]    : software 调压请求脉冲有效信号，投票源4 */
        unsigned int  peri_vote_sft_chg_5     : 1;  /* bit[5]    : software 调压请求脉冲有效信号，投票源5 */
        unsigned int  peri_vote_sft_chg_6     : 1;  /* bit[6]    : software 调压请求脉冲有效信号，投票源6 */
        unsigned int  peri_vote_sft_chg_7     : 1;  /* bit[7]    : software 调压请求脉冲有效信号，投票源7 */
        unsigned int  peri_vote_sft_chg_8     : 1;  /* bit[8]    : software 调压请求脉冲有效信号，投票源8 */
        unsigned int  peri_vote_sft_chg_9     : 1;  /* bit[9]    : software 调压请求脉冲有效信号，投票源9 */
        unsigned int  peri_vote_sft_chg_10    : 1;  /* bit[10]   : software 调压请求脉冲有效信号，投票源10 */
        unsigned int  peri_vote_sft_chg_11    : 1;  /* bit[11]   : software 调压请求脉冲有效信号，投票源11 */
        unsigned int  peri_vote_sft_chg_12    : 1;  /* bit[12]   : software 调压请求脉冲有效信号，投票源12 */
        unsigned int  peri_vote_sft_chg_13    : 1;  /* bit[13]   : software 调压请求脉冲有效信号，投票源13 */
        unsigned int  peri_vote_sft_chg_14    : 1;  /* bit[14]   : software 调压请求脉冲有效信号，投票源14 */
        unsigned int  peri_vote_sft_chg_15    : 1;  /* bit[15]   : software 调压请求脉冲有效信号，投票源15 */
        unsigned int  peri_vote_sft_hrd_chg_0 : 1;  /* bit[16]   : 硬件通道软件调压请求脉冲有效信号，硬件通道0 */
        unsigned int  peri_vote_sft_hrd_chg_1 : 1;  /* bit[17]   : 硬件通道软件调压请求脉冲有效信号，硬件通道1 */
        unsigned int  peri_vote_sft_hrd_chg_2 : 1;  /* bit[18]   : 硬件通道软件调压请求脉冲有效信号，硬件通道2 */
        unsigned int  peri_vote_sft_hrd_chg_3 : 1;  /* bit[19]   : 硬件通道软件调压请求脉冲有效信号，硬件通道3 */
        unsigned int  reserved                : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOTE_SFT_CHG_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_0_START      (0)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_0_END        (0)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_1_START      (1)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_1_END        (1)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_2_START      (2)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_2_END        (2)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_3_START      (3)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_3_END        (3)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_4_START      (4)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_4_END        (4)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_5_START      (5)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_5_END        (5)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_6_START      (6)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_6_END        (6)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_7_START      (7)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_7_END        (7)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_8_START      (8)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_8_END        (8)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_9_START      (9)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_9_END        (9)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_10_START     (10)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_10_END       (10)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_11_START     (11)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_11_END       (11)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_12_START     (12)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_12_END       (12)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_13_START     (13)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_13_END       (13)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_14_START     (14)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_14_END       (14)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_15_START     (15)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_chg_15_END       (15)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_0_START  (16)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_0_END    (16)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_1_START  (17)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_1_END    (17)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_2_START  (18)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_2_END    (18)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_3_START  (19)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_peri_vote_sft_hrd_chg_3_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_UNION
 结构说明  : PERI_VOTE_SFT_CHG_BYPASS 寄存器结构定义。地址偏移量:0x010，初值:0x00000000，宽度:32
 寄存器说明: PERI 投票源软件调压请求bypass
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_sft_chg_bypass_0     : 1;  /* bit[0]    : software 调压请求脉冲有效信号bypass，投票源0 */
        unsigned int  peri_vote_sft_chg_bypass_1     : 1;  /* bit[1]    : software 调压请求脉冲有效信号bypass，投票源1 */
        unsigned int  peri_vote_sft_chg_bypass_2     : 1;  /* bit[2]    : software 调压请求脉冲有效信号bypass，投票源2 */
        unsigned int  peri_vote_sft_chg_bypass_3     : 1;  /* bit[3]    : software 调压请求脉冲有效信号bypass，投票源3 */
        unsigned int  peri_vote_sft_chg_bypass_4     : 1;  /* bit[4]    : software 调压请求脉冲有效信号bypass，投票源4 */
        unsigned int  peri_vote_sft_chg_bypass_5     : 1;  /* bit[5]    : software 调压请求脉冲有效信号bypass，投票源5 */
        unsigned int  peri_vote_sft_chg_bypass_6     : 1;  /* bit[6]    : software 调压请求脉冲有效信号bypass，投票源6 */
        unsigned int  peri_vote_sft_chg_bypass_7     : 1;  /* bit[7]    : software 调压请求脉冲有效信号bypass，投票源7 */
        unsigned int  peri_vote_sft_chg_bypass_8     : 1;  /* bit[8]    : software 调压请求脉冲有效信号bypass，投票源8 */
        unsigned int  peri_vote_sft_chg_bypass_9     : 1;  /* bit[9]    : software 调压请求脉冲有效信号bypass，投票源9 */
        unsigned int  peri_vote_sft_chg_bypass_10    : 1;  /* bit[10]   : software 调压请求脉冲有效信号bypass，投票源10 */
        unsigned int  peri_vote_sft_chg_bypass_11    : 1;  /* bit[11]   : software 调压请求脉冲有效信号bypass，投票源11 */
        unsigned int  peri_vote_sft_chg_bypass_12    : 1;  /* bit[12]   : software 调压请求脉冲有效信号bypass，投票源12 */
        unsigned int  peri_vote_sft_chg_bypass_13    : 1;  /* bit[13]   : software 调压请求脉冲有效信号bypass，投票源13 */
        unsigned int  peri_vote_sft_chg_bypass_14    : 1;  /* bit[14]   : software 调压请求脉冲有效信号bypass，投票源14 */
        unsigned int  peri_vote_sft_chg_bypass_15    : 1;  /* bit[15]   : software 调压请求脉冲有效信号bypass，投票源15 */
        unsigned int  peri_vote_hrd_chg_bypass_0     : 1;  /* bit[16]   : 硬件调压请求脉冲有效信号bypass，投票源0 */
        unsigned int  peri_vote_hrd_chg_bypass_1     : 1;  /* bit[17]   : 硬件调压请求脉冲有效信号bypass，投票源1 */
        unsigned int  peri_vote_hrd_chg_bypass_2     : 1;  /* bit[18]   : 硬件调压请求脉冲有效信号bypass，投票源2 */
        unsigned int  peri_vote_hrd_chg_bypass_3     : 1;  /* bit[19]   : 硬件调压请求脉冲有效信号bypass，投票源3 */
        unsigned int  peri_vote_sft_hrd_chg_bypass_0 : 1;  /* bit[20]   : 硬件通道软件调压请求脉冲有效信号bypass，投票源0 */
        unsigned int  peri_vote_sft_hrd_chg_bypass_1 : 1;  /* bit[21]   : 硬件通道软件调压请求脉冲有效信号bypass，投票源1 */
        unsigned int  peri_vote_sft_hrd_chg_bypass_2 : 1;  /* bit[22]   : 硬件通道软件调压请求脉冲有效信号bypass，投票源2 */
        unsigned int  peri_vote_sft_hrd_chg_bypass_3 : 1;  /* bit[23]   : 硬件通道软件调压请求脉冲有效信号bypass，投票源3 */
        unsigned int  reserved                       : 8;  /* bit[24-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_0_START      (0)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_0_END        (0)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_1_START      (1)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_1_END        (1)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_2_START      (2)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_2_END        (2)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_3_START      (3)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_3_END        (3)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_4_START      (4)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_4_END        (4)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_5_START      (5)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_5_END        (5)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_6_START      (6)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_6_END        (6)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_7_START      (7)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_7_END        (7)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_8_START      (8)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_8_END        (8)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_9_START      (9)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_9_END        (9)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_10_START     (10)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_10_END       (10)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_11_START     (11)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_11_END       (11)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_12_START     (12)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_12_END       (12)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_13_START     (13)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_13_END       (13)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_14_START     (14)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_14_END       (14)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_15_START     (15)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_chg_bypass_15_END       (15)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_0_START      (16)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_0_END        (16)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_1_START      (17)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_1_END        (17)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_2_START      (18)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_2_END        (18)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_3_START      (19)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_hrd_chg_bypass_3_END        (19)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_0_START  (20)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_0_END    (20)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_1_START  (21)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_1_END    (21)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_2_START  (22)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_2_END    (22)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_3_START  (23)
#define SOC_PMCTRL_PERI_VOTE_SFT_CHG_BYPASS_peri_vote_sft_hrd_chg_bypass_3_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_UNION
 结构说明  : PERI_PMU_TRIM_UP_TIME 寄存器结构定义。地址偏移量:0xA54，初值:0x055002A0，宽度:32
 寄存器说明: PERI升压等待时间
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_pmu_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  peri_pmu_up_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_UNION;
#endif
#define SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_peri_pmu_up_hold_time_START  (0)
#define SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_peri_pmu_up_hold_time_END    (19)
#define SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_peri_pmu_up_step_time_START  (20)
#define SOC_PMCTRL_PERI_PMU_TRIM_UP_TIME_peri_pmu_up_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_UNION
 结构说明  : PERI_PMU_TRIM_DN_TTME 寄存器结构定义。地址偏移量:0xA58，初值:0x055002A0，宽度:32
 寄存器说明: PERI降压等待时间
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_pmu_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  peri_pmu_dn_step_time : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_UNION;
#endif
#define SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_peri_pmu_dn_hold_time_START  (0)
#define SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_peri_pmu_dn_hold_time_END    (19)
#define SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_peri_pmu_dn_step_time_START  (20)
#define SOC_PMCTRL_PERI_PMU_TRIM_DN_TTME_peri_pmu_dn_step_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOL_SFT_CLR_UNION
 结构说明  : PERI_VOL_SFT_CLR 寄存器结构定义。地址偏移量:0x014，初值:0x00000000，宽度:32
 寄存器说明: PERI投票源软件调压完成信号clear
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vol_sft_clr_0     : 1;  /* bit[0]    : software 软件调压完成信号clear,投票源0 */
        unsigned int  peri_vol_sft_clr_1     : 1;  /* bit[1]    : software 软件调压完成信号clear,投票源1 */
        unsigned int  peri_vol_sft_clr_2     : 1;  /* bit[2]    : software 软件调压完成信号clear,投票源2 */
        unsigned int  peri_vol_sft_clr_3     : 1;  /* bit[3]    : software 软件调压完成信号clear,投票源3 */
        unsigned int  peri_vol_sft_clr_4     : 1;  /* bit[4]    : software 软件调压完成信号clear,投票源4 */
        unsigned int  peri_vol_sft_clr_5     : 1;  /* bit[5]    : software 软件调压完成信号clear,投票源5 */
        unsigned int  peri_vol_sft_clr_6     : 1;  /* bit[6]    : software 软件调压完成信号clear,投票源6 */
        unsigned int  peri_vol_sft_clr_7     : 1;  /* bit[7]    : software 软件调压完成信号clear,投票源7 */
        unsigned int  peri_vol_sft_clr_8     : 1;  /* bit[8]    : software 软件调压完成信号clear,投票源8 */
        unsigned int  peri_vol_sft_clr_9     : 1;  /* bit[9]    : software 软件调压完成信号clear,投票源9 */
        unsigned int  peri_vol_sft_clr_10    : 1;  /* bit[10]   : software 软件调压完成信号clear,投票源10 */
        unsigned int  peri_vol_sft_clr_11    : 1;  /* bit[11]   : software 软件调压完成信号clear,投票源11 */
        unsigned int  peri_vol_sft_clr_12    : 1;  /* bit[12]   : software 软件调压完成信号clear,投票源12 */
        unsigned int  peri_vol_sft_clr_13    : 1;  /* bit[13]   : software 软件调压完成信号clear,投票源13 */
        unsigned int  peri_vol_sft_clr_14    : 1;  /* bit[14]   : software 软件调压完成信号clear,投票源14 */
        unsigned int  peri_vol_sft_clr_15    : 1;  /* bit[15]   : software 软件调压完成信号clear,投票源15 */
        unsigned int  peri_vol_sft_clr_hrd_0 : 1;  /* bit[16]   : 硬件调压完成信号clear,硬件源0 */
        unsigned int  peri_vol_sft_clr_hrd_1 : 1;  /* bit[17]   : 硬件调压完成信号clear,硬件源1 */
        unsigned int  peri_vol_sft_clr_hrd_2 : 1;  /* bit[18]   : 硬件调压完成信号clear,硬件源2 */
        unsigned int  peri_vol_sft_clr_hrd_3 : 1;  /* bit[19]   : 硬件调压完成信号clear,硬件源3 */
        unsigned int  reserved               : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOL_SFT_CLR_UNION;
#endif
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_0_START      (0)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_0_END        (0)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_1_START      (1)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_1_END        (1)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_2_START      (2)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_2_END        (2)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_3_START      (3)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_3_END        (3)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_4_START      (4)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_4_END        (4)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_5_START      (5)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_5_END        (5)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_6_START      (6)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_6_END        (6)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_7_START      (7)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_7_END        (7)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_8_START      (8)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_8_END        (8)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_9_START      (9)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_9_END        (9)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_10_START     (10)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_10_END       (10)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_11_START     (11)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_11_END       (11)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_12_START     (12)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_12_END       (12)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_13_START     (13)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_13_END       (13)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_14_START     (14)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_14_END       (14)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_15_START     (15)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_15_END       (15)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_0_START  (16)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_0_END    (16)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_1_START  (17)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_1_END    (17)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_2_START  (18)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_2_END    (18)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_3_START  (19)
#define SOC_PMCTRL_PERI_VOL_SFT_CLR_peri_vol_sft_clr_hrd_3_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOL_STABLE_UNION
 结构说明  : PERI_VOL_STABLE 寄存器结构定义。地址偏移量:0x018，初值:0x00000000，宽度:32
 寄存器说明: PERI投票源调压完成信号
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vol_stable_0     : 1;  /* bit[0]    : PERI投票源软件调压完成信号,投票源0 */
        unsigned int  peri_vol_stable_1     : 1;  /* bit[1]    : PERI投票源软件调压完成信号,投票源1 */
        unsigned int  peri_vol_stable_2     : 1;  /* bit[2]    : PERI投票源软件调压完成信号,投票源2 */
        unsigned int  peri_vol_stable_3     : 1;  /* bit[3]    : PERI投票源软件调压完成信号,投票源3 */
        unsigned int  peri_vol_stable_4     : 1;  /* bit[4]    : PERI投票源软件调压完成信号,投票源4 */
        unsigned int  peri_vol_stable_5     : 1;  /* bit[5]    : PERI投票源软件调压完成信号,投票源5 */
        unsigned int  peri_vol_stable_6     : 1;  /* bit[6]    : PERI投票源软件调压完成信号,投票源6 */
        unsigned int  peri_vol_stable_7     : 1;  /* bit[7]    : PERI投票源软件调压完成信号,投票源7 */
        unsigned int  peri_vol_stable_8     : 1;  /* bit[8]    : PERI投票源软件调压完成信号,投票源8 */
        unsigned int  peri_vol_stable_9     : 1;  /* bit[9]    : PERI投票源软件调压完成信号,投票源9 */
        unsigned int  peri_vol_stable_10    : 1;  /* bit[10]   : PERI投票源软件调压完成信号,投票源10 */
        unsigned int  peri_vol_stable_11    : 1;  /* bit[11]   : PERI投票源软件调压完成信号,投票源11 */
        unsigned int  peri_vol_stable_12    : 1;  /* bit[12]   : PERI投票源软件调压完成信号,投票源12 */
        unsigned int  peri_vol_stable_13    : 1;  /* bit[13]   : PERI投票源软件调压完成信号,投票源13 */
        unsigned int  peri_vol_stable_14    : 1;  /* bit[14]   : PERI投票源软件调压完成信号,投票源14 */
        unsigned int  peri_vol_stable_15    : 1;  /* bit[15]   : PERI投票源软件调压完成信号,投票源15 */
        unsigned int  peri_vol_hrd_stable_0 : 1;  /* bit[16]   : PERI投票源硬件调压完成信号,投票源0 */
        unsigned int  peri_vol_hrd_stable_1 : 1;  /* bit[17]   : PERI投票源硬件调压完成信号,投票源1 */
        unsigned int  peri_vol_hrd_stable_2 : 1;  /* bit[18]   : PERI投票源硬件调压完成信号,投票源2 */
        unsigned int  peri_vol_hrd_stable_3 : 1;  /* bit[19]   : PERI投票源硬件调压完成信号,投票源3 */
        unsigned int  reserved              : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOL_STABLE_UNION;
#endif
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_0_START      (0)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_0_END        (0)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_1_START      (1)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_1_END        (1)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_2_START      (2)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_2_END        (2)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_3_START      (3)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_3_END        (3)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_4_START      (4)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_4_END        (4)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_5_START      (5)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_5_END        (5)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_6_START      (6)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_6_END        (6)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_7_START      (7)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_7_END        (7)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_8_START      (8)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_8_END        (8)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_9_START      (9)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_9_END        (9)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_10_START     (10)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_10_END       (10)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_11_START     (11)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_11_END       (11)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_12_START     (12)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_12_END       (12)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_13_START     (13)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_13_END       (13)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_14_START     (14)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_14_END       (14)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_15_START     (15)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_stable_15_END       (15)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_0_START  (16)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_0_END    (16)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_1_START  (17)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_1_END    (17)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_2_START  (18)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_2_END    (18)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_3_START  (19)
#define SOC_PMCTRL_PERI_VOL_STABLE_peri_vol_hrd_stable_3_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INIT_VOL_CONFIG_UNION
 结构说明  : PERI_INIT_VOL_CONFIG 寄存器结构定义。地址偏移量:0xA64，初值:0x00000000，宽度:32
 寄存器说明: PERI DVS初始电压配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_init_vol_idx : 8;  /* bit[0-7] : PERI 初始电压值 */
        unsigned int  peri_init_vol_req : 1;  /* bit[8]   : PERO 初始电压请求 */
        unsigned int  reserved          : 23; /* bit[9-31]:  */
    } reg;
} SOC_PMCTRL_PERI_INIT_VOL_CONFIG_UNION;
#endif
#define SOC_PMCTRL_PERI_INIT_VOL_CONFIG_peri_init_vol_idx_START  (0)
#define SOC_PMCTRL_PERI_INIT_VOL_CONFIG_peri_init_vol_idx_END    (7)
#define SOC_PMCTRL_PERI_INIT_VOL_CONFIG_peri_init_vol_req_START  (8)
#define SOC_PMCTRL_PERI_INIT_VOL_CONFIG_peri_init_vol_req_END    (8)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_FIX_VOL_CONFIG_UNION
 结构说明  : PERI_FIX_VOL_CONFIG 寄存器结构定义。地址偏移量:0xA68，初值:0x00000000，宽度:32
 寄存器说明: PERI DVS debug锁电压配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_sft_fix_vol_idx : 8;  /* bit[0-7]  : PERI 软件锁压值 */
        unsigned int  peri_sft_fix_vol     : 1;  /* bit[8]    : PERI 软件锁压标志 */
        unsigned int  peri_sft_fix_vol_req : 1;  /* bit[9]    : PERI_锁压请求 */
        unsigned int  reserved             : 22; /* bit[10-31]:  */
    } reg;
} SOC_PMCTRL_PERI_FIX_VOL_CONFIG_UNION;
#endif
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_peri_sft_fix_vol_idx_START  (0)
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_peri_sft_fix_vol_idx_END    (7)
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_peri_sft_fix_vol_START      (8)
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_peri_sft_fix_vol_END        (8)
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_peri_sft_fix_vol_req_START  (9)
#define SOC_PMCTRL_PERI_FIX_VOL_CONFIG_peri_sft_fix_vol_req_END    (9)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION
 结构说明  : PERI_INTR_M3ORCPU_MSK 寄存器结构定义。地址偏移量:0x01C，初值:0x00000000，宽度:32
 寄存器说明: PERI投票源软件调压完成中断MSK
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_intr_m3orcpu_msk_0  : 1;  /* bit[0]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源0 */
        unsigned int  peri_intr_m3orcpu_msk_1  : 1;  /* bit[1]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源1 */
        unsigned int  peri_intr_m3orcpu_msk_2  : 1;  /* bit[2]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源2 */
        unsigned int  peri_intr_m3orcpu_msk_3  : 1;  /* bit[3]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源3 */
        unsigned int  peri_intr_m3orcpu_msk_4  : 1;  /* bit[4]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源4 */
        unsigned int  peri_intr_m3orcpu_msk_5  : 1;  /* bit[5]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源5 */
        unsigned int  peri_intr_m3orcpu_msk_6  : 1;  /* bit[6]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源6 */
        unsigned int  peri_intr_m3orcpu_msk_7  : 1;  /* bit[7]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源7 */
        unsigned int  peri_intr_m3orcpu_msk_8  : 1;  /* bit[8]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源8 */
        unsigned int  peri_intr_m3orcpu_msk_9  : 1;  /* bit[9]    : 1代表给M3发送中断、0代表给ACPU发送中断,投票源9 */
        unsigned int  peri_intr_m3orcpu_msk_10 : 1;  /* bit[10]   : 1代表给M3发送中断、0代表给ACPU发送中断,投票源10 */
        unsigned int  peri_intr_m3orcpu_msk_11 : 1;  /* bit[11]   : 1代表给M3发送中断、0代表给ACPU发送中断,投票源11 */
        unsigned int  peri_intr_m3orcpu_msk_12 : 1;  /* bit[12]   : 1代表给M3发送中断、0代表给ACPU发送中断,投票源12 */
        unsigned int  peri_intr_m3orcpu_msk_13 : 1;  /* bit[13]   : 1代表给M3发送中断、0代表给ACPU发送中断,投票源13 */
        unsigned int  peri_intr_m3orcpu_msk_14 : 1;  /* bit[14]   : 1代表给M3发送中断、0代表给ACPU发送中断,投票源14 */
        unsigned int  peri_intr_m3orcpu_msk_15 : 1;  /* bit[15]   : 1代表给M3发送中断、0代表给ACPU发送中断,投票源15 */
        unsigned int  peri_intr_m3orcpu_msk_16 : 1;  /* bit[16]   : 1代表给M3发送中断、0代表给ACPU发送中断,硬件通道投票源0 */
        unsigned int  peri_intr_m3orcpu_msk_17 : 1;  /* bit[17]   : 1代表给M3发送中断、0代表给ACPU发送中断,硬件通道投票源1 */
        unsigned int  peri_intr_m3orcpu_msk_18 : 1;  /* bit[18]   : 1代表给M3发送中断、0代表给ACPU发送中断,硬件通道投票源2 */
        unsigned int  peri_intr_m3orcpu_msk_19 : 1;  /* bit[19]   : 1代表给M3发送中断、0代表给ACPU发送中断,硬件通道投票源3 */
        unsigned int  reserved                 : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION;
#endif
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_0_START   (0)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_0_END     (0)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_1_START   (1)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_1_END     (1)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_2_START   (2)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_2_END     (2)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_3_START   (3)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_3_END     (3)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_4_START   (4)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_4_END     (4)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_5_START   (5)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_5_END     (5)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_6_START   (6)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_6_END     (6)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_7_START   (7)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_7_END     (7)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_8_START   (8)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_8_END     (8)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_9_START   (9)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_9_END     (9)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_10_START  (10)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_10_END    (10)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_11_START  (11)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_11_END    (11)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_12_START  (12)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_12_END    (12)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_13_START  (13)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_13_END    (13)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_14_START  (14)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_14_END    (14)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_15_START  (15)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_15_END    (15)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_16_START  (16)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_16_END    (16)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_17_START  (17)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_17_END    (17)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_18_START  (18)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_18_END    (18)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_19_START  (19)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_m3orcpu_msk_19_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOL_READ_UNION
 结构说明  : PERI_VOL_READ 寄存器结构定义。地址偏移量:0xA70，初值:0x00000000，宽度:32
 寄存器说明: PERI电压回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_cur_max_vol       : 8;  /* bit[0-7]  : PERI当前电压值 */
        unsigned int  peri_nxt_max_vol       : 8;  /* bit[8-15] : PERI下次电压值 */
        unsigned int  peri_cur_max_vol_hst_0 : 8;  /* bit[16-23]: PERI历史上一次电压值 */
        unsigned int  peri_cur_max_vol_hst_1 : 8;  /* bit[24-31]: PERI历史上两次电压值 */
    } reg;
} SOC_PMCTRL_PERI_VOL_READ_UNION;
#endif
#define SOC_PMCTRL_PERI_VOL_READ_peri_cur_max_vol_START        (0)
#define SOC_PMCTRL_PERI_VOL_READ_peri_cur_max_vol_END          (7)
#define SOC_PMCTRL_PERI_VOL_READ_peri_nxt_max_vol_START        (8)
#define SOC_PMCTRL_PERI_VOL_READ_peri_nxt_max_vol_END          (15)
#define SOC_PMCTRL_PERI_VOL_READ_peri_cur_max_vol_hst_0_START  (16)
#define SOC_PMCTRL_PERI_VOL_READ_peri_cur_max_vol_hst_0_END    (23)
#define SOC_PMCTRL_PERI_VOL_READ_peri_cur_max_vol_hst_1_START  (24)
#define SOC_PMCTRL_PERI_VOL_READ_peri_cur_max_vol_hst_1_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_DVS_OUTPUT_READ_UNION
 结构说明  : PERI_DVS_OUTPUT_READ 寄存器结构定义。地址偏移量:0x020，初值:0x00000000，宽度:32
 寄存器说明: PERI DVS 输出回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_pmu_vol_idx_spmi       : 8;  /* bit[0-7]  : SPMI调压通道值 */
        unsigned int  peri_pmu_vol_chg_spmi       : 1;  /* bit[8]    : 对接SPMI调压通道请求电平翻转信号 */
        unsigned int  intr_peri_dvs_stat_pmc2lpm3 : 1;  /* bit[9]    : M3调压完成中断信号 */
        unsigned int  intr_peri_dvs_stat_pmc2acpu : 1;  /* bit[10]   : ACPU调压完成中断信号 */
        unsigned int  peri_vol_rank_pmc2dpm       : 3;  /* bit[11-13]: 送给PERI区DPM模块 */
        unsigned int  peri_cur_vau_stat           : 3;  /* bit[14-16]: 状态机状态 */
        unsigned int  reserved                    : 15; /* bit[17-31]:  */
    } reg;
} SOC_PMCTRL_PERI_DVS_OUTPUT_READ_UNION;
#endif
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_pmu_vol_idx_spmi_START        (0)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_pmu_vol_idx_spmi_END          (7)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_pmu_vol_chg_spmi_START        (8)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_pmu_vol_chg_spmi_END          (8)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_intr_peri_dvs_stat_pmc2lpm3_START  (9)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_intr_peri_dvs_stat_pmc2lpm3_END    (9)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_intr_peri_dvs_stat_pmc2acpu_START  (10)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_intr_peri_dvs_stat_pmc2acpu_END    (10)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_vol_rank_pmc2dpm_START        (11)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_vol_rank_pmc2dpm_END          (13)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_cur_vau_stat_START            (14)
#define SOC_PMCTRL_PERI_DVS_OUTPUT_READ_peri_cur_vau_stat_END              (16)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOL_RANK_IDX_0_UNION
 结构说明  : PERI_VOL_RANK_IDX_0 寄存器结构定义。地址偏移量:0xA78，初值:0x00000000，宽度:32
 寄存器说明: PERI电压档位值
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vol_rank_idx_0 : 8;  /* bit[0-7]  : PERI 电压高于idx_6,rank输出3‘b000；
                                                               高于idx_5，rank输出3’b001； 
                                                               高于idx_4，rank输出3’b010；
                                                               高于idx_3，rank输出3’b011；
                                                               高于idx_2，rank输出3’b100；
                                                               高于idx_1，rank输出3’b101；
                                                               小于idx_1，rank输出3‘b111；  */
        unsigned int  peri_vol_rank_idx_1 : 8;  /* bit[8-15] :  */
        unsigned int  peri_vol_rank_idx_2 : 8;  /* bit[16-23]:  */
        unsigned int  peri_vol_rank_idx_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOL_RANK_IDX_0_UNION;
#endif
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_0_START  (0)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_0_END    (7)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_1_START  (8)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_1_END    (15)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_2_START  (16)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_2_END    (23)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_3_START  (24)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_0_peri_vol_rank_idx_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOL_RANK_IDX_1_UNION
 结构说明  : PERI_VOL_RANK_IDX_1 寄存器结构定义。地址偏移量:0xA7C，初值:0x00000000，宽度:32
 寄存器说明: PERI电压档位值
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vol_rank_idx_4 : 8;  /* bit[0-7]  :  */
        unsigned int  peri_vol_rank_idx_5 : 8;  /* bit[8-15] :  */
        unsigned int  peri_vol_rank_idx_6 : 8;  /* bit[16-23]:  */
        unsigned int  reserved            : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOL_RANK_IDX_1_UNION;
#endif
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_peri_vol_rank_idx_4_START  (0)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_peri_vol_rank_idx_4_END    (7)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_peri_vol_rank_idx_5_START  (8)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_peri_vol_rank_idx_5_END    (15)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_peri_vol_rank_idx_6_START  (16)
#define SOC_PMCTRL_PERI_VOL_RANK_IDX_1_peri_vol_rank_idx_6_END    (23)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_UNION
 结构说明  : PERI_VOTE_SFT_HRD_VOL_IDX 寄存器结构定义。地址偏移量:0xA80，初值:0x00000000，宽度:32
 寄存器说明: PERI 4路硬件通道软件调压参数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vote_sft_hrd_vol_idx_0 : 8;  /* bit[0-7]  : PERI 第0路硬件通道软件调压参数 */
        unsigned int  peri_vote_sft_hrd_vol_idx_1 : 8;  /* bit[8-15] : PERI 第1路硬件通道软件调压参数 */
        unsigned int  peri_vote_sft_hrd_vol_idx_2 : 8;  /* bit[16-23]: PERI 第2路硬件通道软件调压参数 */
        unsigned int  peri_vote_sft_hrd_vol_idx_3 : 8;  /* bit[24-31]: PERI 第3路硬件通道软件调压参数 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_0_START  (0)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_0_END    (7)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_1_START  (8)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_1_END    (15)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_2_START  (16)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_2_END    (23)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_3_START  (24)
#define SOC_PMCTRL_PERI_VOTE_SFT_HRD_VOL_IDX_peri_vote_sft_hrd_vol_idx_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_STAT_TIME_HOLD_UNION
 结构说明  : PERI_STAT_TIME_HOLD 寄存器结构定义。地址偏移量:0xA84，初值:0x00000001，宽度:32
 寄存器说明: PERI响应下一次调压请求hold时间
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_stat_time_hold : 20; /* bit[0-19] :  */
        unsigned int  reserved            : 12; /* bit[20-31]: Unit: pclk */
    } reg;
} SOC_PMCTRL_PERI_STAT_TIME_HOLD_UNION;
#endif
#define SOC_PMCTRL_PERI_STAT_TIME_HOLD_peri_stat_time_hold_START  (0)
#define SOC_PMCTRL_PERI_STAT_TIME_HOLD_peri_stat_time_hold_END    (19)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INTR_BYPASS_SFT_UNION
 结构说明  : PERI_INTR_BYPASS_SFT 寄存器结构定义。地址偏移量:0x024，初值:0x00000000，宽度:32
 寄存器说明: PERI软件投票源调压完成中断bypass
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_intr_bypass_0  : 1;  /* bit[0]    : 中断屏蔽bypass,1有效，投票源0 */
        unsigned int  peri_intr_bypass_1  : 1;  /* bit[1]    : 中断屏蔽bypass,1有效，投票源1 */
        unsigned int  peri_intr_bypass_2  : 1;  /* bit[2]    : 中断屏蔽bypass,1有效，投票源2 */
        unsigned int  peri_intr_bypass_3  : 1;  /* bit[3]    : 中断屏蔽bypass,1有效，投票源3 */
        unsigned int  peri_intr_bypass_4  : 1;  /* bit[4]    : 中断屏蔽bypass,1有效，投票源4 */
        unsigned int  peri_intr_bypass_5  : 1;  /* bit[5]    : 中断屏蔽bypass,1有效，投票源5 */
        unsigned int  peri_intr_bypass_6  : 1;  /* bit[6]    : 中断屏蔽bypass,1有效，投票源6 */
        unsigned int  peri_intr_bypass_7  : 1;  /* bit[7]    : 中断屏蔽bypass,1有效，投票源7 */
        unsigned int  peri_intr_bypass_8  : 1;  /* bit[8]    : 中断屏蔽bypass,1有效，投票源8 */
        unsigned int  peri_intr_bypass_9  : 1;  /* bit[9]    : 中断屏蔽bypass,1有效，投票源9 */
        unsigned int  peri_intr_bypass_10 : 1;  /* bit[10]   : 中断屏蔽bypass,1有效，投票源10 */
        unsigned int  peri_intr_bypass_11 : 1;  /* bit[11]   : 中断屏蔽bypass,1有效，投票源11 */
        unsigned int  peri_intr_bypass_12 : 1;  /* bit[12]   : 中断屏蔽bypass,1有效，投票源12 */
        unsigned int  peri_intr_bypass_13 : 1;  /* bit[13]   : 中断屏蔽bypass,1有效，投票源13 */
        unsigned int  peri_intr_bypass_14 : 1;  /* bit[14]   : 中断屏蔽bypass,1有效，投票源14 */
        unsigned int  peri_intr_bypass_15 : 1;  /* bit[15]   : 中断屏蔽bypass,1有效，投票源15 */
        unsigned int  reserved            : 16; /* bit[16-31]:  */
    } reg;
} SOC_PMCTRL_PERI_INTR_BYPASS_SFT_UNION;
#endif
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_0_START   (0)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_0_END     (0)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_1_START   (1)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_1_END     (1)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_2_START   (2)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_2_END     (2)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_3_START   (3)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_3_END     (3)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_4_START   (4)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_4_END     (4)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_5_START   (5)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_5_END     (5)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_6_START   (6)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_6_END     (6)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_7_START   (7)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_7_END     (7)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_8_START   (8)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_8_END     (8)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_9_START   (9)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_9_END     (9)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_10_START  (10)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_10_END    (10)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_11_START  (11)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_11_END    (11)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_12_START  (12)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_12_END    (12)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_13_START  (13)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_13_END    (13)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_14_START  (14)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_14_END    (14)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_15_START  (15)
#define SOC_PMCTRL_PERI_INTR_BYPASS_SFT_peri_intr_bypass_15_END    (15)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_INTR_BYPASS_HRD_UNION
 结构说明  : PERI_INTR_BYPASS_HRD 寄存器结构定义。地址偏移量:0x028，初值:0x00000000，宽度:32
 寄存器说明: PERI硬件通道调压完成中断bypass，reserverd bit mask借用其他功能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_intr_bypass_16       : 1;  /* bit[0]    : 硬件通道调压完成中断屏蔽bypass,1有效，硬件投票源0 */
        unsigned int  peri_intr_bypass_17       : 1;  /* bit[1]    : 硬件通道调压完成中断屏蔽bypass,1有效，硬件投票源1 */
        unsigned int  peri_intr_bypass_18       : 1;  /* bit[2]    : 硬件通道调压完成中断屏蔽bypass,1有效，硬件投票源2 */
        unsigned int  peri_intr_bypass_19       : 1;  /* bit[3]    : 硬件通道调压完成中断屏蔽bypass,1有效，硬件投票源3 */
        unsigned int  peri_vol_rank_sftorhrd    : 1;  /* bit[4]    : PERI 电压档位送DPM，1选择软件配置，0选择硬件配置  */
        unsigned int  peri_vol_rank_pmc2dpm_sft : 3;  /* bit[5-7]  : 软件配置PERI 电压档位送DPM */
        unsigned int  reserved_0                : 1;  /* bit[8]    :  */
        unsigned int  reserved_1                : 1;  /* bit[9]    :  */
        unsigned int  reserved_2                : 1;  /* bit[10]   :  */
        unsigned int  reserved_3                : 1;  /* bit[11]   :  */
        unsigned int  reserved_4                : 1;  /* bit[12]   :  */
        unsigned int  reserved_5                : 1;  /* bit[13]   :  */
        unsigned int  reserved_6                : 1;  /* bit[14]   :  */
        unsigned int  reserved_7                : 1;  /* bit[15]   :  */
        unsigned int  reserved_8                : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_INTR_BYPASS_HRD_UNION;
#endif
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_16_START        (0)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_16_END          (0)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_17_START        (1)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_17_END          (1)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_18_START        (2)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_18_END          (2)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_19_START        (3)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_intr_bypass_19_END          (3)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_vol_rank_sftorhrd_START     (4)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_vol_rank_sftorhrd_END       (4)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_vol_rank_pmc2dpm_sft_START  (5)
#define SOC_PMCTRL_PERI_INTR_BYPASS_HRD_peri_vol_rank_pmc2dpm_sft_END    (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VOL_CHG_FLAG_UNION
 结构说明  : PERI_VOL_CHG_FLAG 寄存器结构定义。地址偏移量:0xA90，初值:0x00000000，宽度:32
 寄存器说明: PERI 调压请求flag
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_vol_chg_flag : 21; /* bit[0-20] : peri调压请求flag */
        unsigned int  reserved_0        : 3;  /* bit[21-23]: 对接SPMI调压通道请求电平翻转信号 */
        unsigned int  reserved_1        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_PERI_VOL_CHG_FLAG_UNION;
#endif
#define SOC_PMCTRL_PERI_VOL_CHG_FLAG_peri_vol_chg_flag_START  (0)
#define SOC_PMCTRL_PERI_VOL_CHG_FLAG_peri_vol_chg_flag_END    (20)


/*****************************************************************************
 结构名    : SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_UNION
 结构说明  : PERI_VALID_HRD_VOL_IDX 寄存器结构定义。地址偏移量:0xA94，初值:0x00000000，宽度:32
 寄存器说明: 硬件通道请求电压回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  peri_valid_hrd_vol_idx_0 : 8;  /* bit[0-7]  : 硬件通道0软硬件请求电压 */
        unsigned int  peri_valid_hrd_vol_idx_1 : 8;  /* bit[8-15] : 硬件通道1软硬件请求电压 */
        unsigned int  peri_valid_hrd_vol_idx_2 : 8;  /* bit[16-23]: 硬件通道2软硬件请求电压 */
        unsigned int  peri_valid_hrd_vol_idx_3 : 8;  /* bit[24-31]: 硬件通道3软硬件请求电压 */
    } reg;
} SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_UNION;
#endif
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_0_START  (0)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_0_END    (7)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_1_START  (8)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_1_END    (15)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_2_START  (16)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_2_END    (23)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_3_START  (24)
#define SOC_PMCTRL_PERI_VALID_HRD_VOL_IDX_peri_valid_hrd_vol_idx_3_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_UNION
 结构说明  : SVFD_FREQ_DOWM_STAT 寄存器结构定义。地址偏移量:0x620，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM使能状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_freq_down_en_svfd : 1;  /* bit[0]   : cofsma的SVFD硬件降频使能 */
        unsigned int  cofsmb_freq_down_en_svfd : 1;  /* bit[1]   : cofsmb的SVFD硬件降频使能 */
        unsigned int  cofsmc_freq_down_en_svfd : 1;  /* bit[2]   : cofsmc的SVFD硬件降频使能 */
        unsigned int  cofsmd_freq_down_en_svfd : 1;  /* bit[3]   : cofsmd的SVFD硬件降频使能 */
        unsigned int  cofsme_freq_down_en_svfd : 1;  /* bit[4]   : cofsme的SVFD硬件降频使能 */
        unsigned int  cofsmf_freq_down_en_svfd : 1;  /* bit[5]   : cofsmf的SVFD硬件降频使能 */
        unsigned int  cofsmg_freq_down_en_svfd : 1;  /* bit[6]   : cofsmg的SVFD硬件降频使能 */
        unsigned int  cofsmh_freq_down_en_svfd : 1;  /* bit[7]   : cofsmh的SVFD硬件降频使能 */
        unsigned int  reserved                 : 24; /* bit[8-31]:  */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsma_freq_down_en_svfd_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsma_freq_down_en_svfd_END    (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmb_freq_down_en_svfd_START  (1)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmb_freq_down_en_svfd_END    (1)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmc_freq_down_en_svfd_START  (2)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmc_freq_down_en_svfd_END    (2)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmd_freq_down_en_svfd_START  (3)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmd_freq_down_en_svfd_END    (3)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsme_freq_down_en_svfd_START  (4)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsme_freq_down_en_svfd_END    (4)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmf_freq_down_en_svfd_START  (5)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmf_freq_down_en_svfd_END    (5)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmg_freq_down_en_svfd_START  (6)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmg_freq_down_en_svfd_END    (6)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmh_freq_down_en_svfd_START  (7)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_STAT_cofsmh_freq_down_en_svfd_END    (7)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL0 寄存器结构定义。地址偏移量:0x624，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_cofsma_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL0_cofsma_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL1 寄存器结构定义。地址偏移量:0x628，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_cofsmb_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL1_cofsmb_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL2 寄存器结构定义。地址偏移量:0x62C，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_cofsmc_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL2_cofsmc_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL3 寄存器结构定义。地址偏移量:0x630，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_cofsmd_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL3_cofsmd_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL4 寄存器结构定义。地址偏移量:0x634，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_cofsme_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL4_cofsme_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL5 寄存器结构定义。地址偏移量:0x638，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_cofsmf_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL5_cofsmf_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL6 寄存器结构定义。地址偏移量:0x63C，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_cofsmg_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL6_cofsmg_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL7 寄存器结构定义。地址偏移量:0x640，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_svfd_freq_down_step_ctrl : 32; /* bit[0-31]: SVFD硬件降频使能的档位 */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_cofsmh_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL7_cofsmh_svfd_freq_down_step_ctrl_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME0 寄存器结构定义。地址偏移量:0x644，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_cofsma_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME0_cofsma_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME1 寄存器结构定义。地址偏移量:0x648，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmb_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_cofsmb_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME1_cofsmb_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME2 寄存器结构定义。地址偏移量:0x64C，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmc_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_cofsmc_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME2_cofsmc_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME3 寄存器结构定义。地址偏移量:0x650，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmd_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_cofsmd_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME3_cofsmd_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME4 寄存器结构定义。地址偏移量:0x654，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsme_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_cofsme_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME4_cofsme_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME5 寄存器结构定义。地址偏移量:0x658，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmf_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_cofsmf_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME5_cofsmf_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME6 寄存器结构定义。地址偏移量:0x65C，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmg_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_cofsmg_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME6_cofsmg_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_UNION
 结构说明  : SVFD_FREQ_DN_WAIT_TIME7 寄存器结构定义。地址偏移量:0x660，初值:0x0000000F，宽度:32
 寄存器说明: SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsmh_svfd_freq_dn_wait_time : 32; /* bit[0-31]: SVFD进入硬件降频每一步的等待时间(不能配为0) */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_cofsmh_svfd_freq_dn_wait_time_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DN_WAIT_TIME7_cofsmh_svfd_freq_dn_wait_time_END    (31)


/*****************************************************************************
 结构名    : SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_UNION
 结构说明  : SVFD_FREQ_DOWM_CTRL_BYPASS 寄存器结构定义。地址偏移量:0x664，初值:0x00000000，宽度:32
 寄存器说明: SVFD/VDM软件bypass寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cofsma_svfd_freq_down_ctrl_bypass : 1;  /* bit[0]   : cofsma的SVFD硬件降频bypass */
        unsigned int  cofsmb_svfd_freq_down_ctrl_bypass : 1;  /* bit[1]   : cofsmb的SVFD硬件降频bypass */
        unsigned int  cofsmc_svfd_freq_down_ctrl_bypass : 1;  /* bit[2]   : cofsmc的SVFD硬件降频bypass */
        unsigned int  cofsmd_svfd_freq_down_ctrl_bypass : 1;  /* bit[3]   : cofsmd的SVFD硬件降频bypass,1:bypass使能 */
        unsigned int  cofsme_svfd_freq_down_ctrl_bypass : 1;  /* bit[4]   : cofsme的SVFD硬件降频bypass */
        unsigned int  cofsmf_svfd_freq_down_ctrl_bypass : 1;  /* bit[5]   : cofsmf的SVFD硬件降频bypass */
        unsigned int  cofsmg_svfd_freq_down_ctrl_bypass : 1;  /* bit[6]   : cofsmg的SVFD硬件降频bypass */
        unsigned int  cofsmh_svfd_freq_down_ctrl_bypass : 1;  /* bit[7]   : cofsmh的SVFD硬件降频bypass,1:bypass使能 */
        unsigned int  reserved                          : 24; /* bit[8-31]:  */
    } reg;
} SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_UNION;
#endif
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsma_svfd_freq_down_ctrl_bypass_START  (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsma_svfd_freq_down_ctrl_bypass_END    (0)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmb_svfd_freq_down_ctrl_bypass_START  (1)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmb_svfd_freq_down_ctrl_bypass_END    (1)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmc_svfd_freq_down_ctrl_bypass_START  (2)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmc_svfd_freq_down_ctrl_bypass_END    (2)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmd_svfd_freq_down_ctrl_bypass_START  (3)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmd_svfd_freq_down_ctrl_bypass_END    (3)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsme_svfd_freq_down_ctrl_bypass_START  (4)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsme_svfd_freq_down_ctrl_bypass_END    (4)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmf_svfd_freq_down_ctrl_bypass_START  (5)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmf_svfd_freq_down_ctrl_bypass_END    (5)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmg_svfd_freq_down_ctrl_bypass_START  (6)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmg_svfd_freq_down_ctrl_bypass_END    (6)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmh_svfd_freq_down_ctrl_bypass_START  (7)
#define SOC_PMCTRL_SVFD_FREQ_DOWM_CTRL_BYPASS_cofsmh_svfd_freq_down_ctrl_bypass_END    (7)






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

#endif /* end of soc_pmctrl_interface.h */
