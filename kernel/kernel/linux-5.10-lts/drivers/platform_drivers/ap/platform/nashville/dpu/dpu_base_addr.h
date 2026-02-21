/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2023-2023. All rights reserved.
 * Description: dpu_base_addr.h
 * Create: 2023-12-12
 */
#ifndef DPU_BASE_ADDR_H
#define DPU_BASE_ADDR_H

/****************************************************************************
                            REG_REDEFINE
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__

/* Register description: 用于软件使用的存储数据寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA0_UNION */
#define SOC_SCTRL_SCBAKDATA0_ADDR(base)                       ((base) + (0x40CUL))

/* Register description: 用于软件使用的存储数据寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA4_UNION */
#define SOC_SCTRL_SCBAKDATA4_ADDR(base)                       ((base) + (0x41CUL))

/* Register description: 用于软件使用的存储数据寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA11_UNION */
#define SOC_SCTRL_SCBAKDATA11_ADDR(base)                      ((base) + (0x438UL))

 /* Register description: PERI投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_ADDR(base)           ((base) + 0x01CUL)

/* Register description: DSS SW初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_ADDR(base)          ((base) + 0x030UL)

/* Register description: DSS DIV初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_ADDR(base)         ((base) + 0x034UL)

/* Register description: DSS PLL EN以及GT初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_UNION */
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_ADDR(base)            ((base) + 0x038UL)

/* Register description: DSS 电压频点初始化触发配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_ADDR(base)          ((base) + 0x040UL)

/* Register description: DSS DVFS 软通道指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_APB_SF_INSTR_LIST_UNION */
#define SOC_PMCTRL_APB_SF_INSTR_LIST_ADDR(base, sf)           ((base) + 0x070UL + (sf) * 4)

/* Register description: DSS DVFS 硬通道指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_APB_HW_INSTR_LIST_UNION */
#define SOC_PMCTRL_APB_HW_INSTR_LIST_ADDR(base, hw)           ((base) + 0x100UL + (hw) * 4)

/* Register description: DSS DVFS CNT相关初始化配置1
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_CNT_CFG1_UNION */
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_ADDR(base)               ((base) + 0x044UL)

/* Register description: DSS DVFS CNT相关初始化配置2
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_CNT_CFG2_UNION */
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_ADDR(base)               ((base) + 0x048UL)

/* Register description: DSS DVFS CNT相关初始化配置3
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_CNT_CFG3_UNION */
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG3_ADDR(base)               ((base) + 0x04CUL)

/* Register description: DSS DVFS 中断相关初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_INTR_CFG_UNION */
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_ADDR(base)               ((base) + 0x050UL)

/* Register description: DSS软通道DVFS相关配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_SF_CH_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_SF_CH_ADDR(base)               ((base) + 0x054UL)

/* Register description: DSS软通道DVFS相关回读
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_SF_RD_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_ADDR(base)               ((base) + 0x058UL)

/* Register description: DSS硬通道DVFS相关配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_HW_CH_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_ADDR(base)               ((base) + 0x05CUL)

/* Register description: DSS硬通道DVFS相关回读
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_HW_RD_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_ADDR(base)               ((base) + 0x060UL)

/* Register description: DSS硬通道DVFS使能相关配置
   Bit domain definition UNION:  SOC_PMCTRL_APB_DSS_DVFS_HW_EN_UNION */
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_ADDR(base)              ((base) + 0x064UL)

/* Register description: DSS接口相关信号debug配置
   Bit domain definition UNION:  SOC_PMCTRL_APB_DEBUG_CFG_UNION */
#define SOC_PMCTRL_APB_DEBUG_CFG_ADDR(base)                   ((base) + 0x1D0UL)

/* Register description: PERI_M1 DVS 状态回读
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M1_DVS_STAT3_UNION */
#define SOC_PMCTRL_PERI_M1_DVS_STAT3_ADDR(base)               ((base) + 0x334UL)

/* Register description: PERI_M1 DVS 状态回读
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M1_DVS_STAT7_UNION */
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_ADDR(base)               ((base) + 0x348UL)

/* Register description: 外设通用控制寄存器5
   Bit domain definition UNION:  SOC_PMCTRL_PERI_CTRL5_UNION */
#define SOC_PMCTRL_PERI_CTRL5_ADDR(base)                      ((base) + 0x354UL)

/* Register description: BUCK5投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_ADDR(base)          ((base) + 0x3A8UL)

/* Register description: VDDC_MEM投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_ADDR(base)       ((base) + 0x3E4UL)

/* Register description: PERI_M1投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_ADDR(base)        ((base) + 0x524UL)

/* Register description: PERI_M2投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_ADDR(base)        ((base) + 0x558UL)

/* Register description: PERI DSS AVS电压参数0
   Bit domain definition UNION:  SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_UNION */
#define SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_ADDR(base)          ((base) + 0xA40UL)

/* Register description: 外设时钟使能状态寄存器2。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_PERCLKEN1_UNION */
#define SOC_MEDIA1_CRG_PERCLKEN1_ADDR(base)                   ((base) + 0x018UL)

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_PERRSTSTAT0_UNION */
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ADDR(base)                 ((base) + 0x038UL)

/* Register description: 时钟分频比控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_CLKDIV0_UNION */
#define SOC_MEDIA1_CRG_CLKDIV0_ADDR(base)                     ((base) + 0x060UL)

/* Register description: 时钟分频比控制寄存器2。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_CLKDIV2_UNION */
#define SOC_MEDIA1_CRG_CLKDIV2_ADDR(base)                     ((base) + 0x068UL)

/* Register description: dss dvfs使能寄存器
   Bit domain definition UNION:  SOC_MEDIA1_CRG_DSS_DVFS_UNION */
#define SOC_MEDIA1_CRG_DSS_DVFS_ADDR(base)                    ((base) + 0x100UL)

/* Register description: dss dvfs状态回读寄存器
   Bit domain definition UNION:  SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_UNION */
#define SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_ADDR(base)            ((base) + 0x104UL)

/* Register description: 外设软复位使能寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERRSTEN0_UNION */
#define SOC_CRGPERIPH_PERRSTEN0_ADDR(base)                    ((base) + (0x060UL))

 /* Register description: 外设软复位撤离寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERRSTDIS0_UNION */
#define SOC_CRGPERIPH_PERRSTDIS0_ADDR(base)                   ((base) + (0x064UL))

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERRSTSTAT0_UNION */
#define SOC_CRGPERIPH_PERRSTSTAT0_ADDR(base)                  ((base) + (0x068UL))

/* Register description: 外设时钟使能寄存器12。
   Bit domain definition UNION:  SOC_CRGPERIPH_PEREN12_UNION */
#define SOC_CRGPERIPH_PEREN12_ADDR(base)                      ((base) + (0x470UL))

/* Register description: 外设时钟禁止寄存器12。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERDIS12_UNION */
#define SOC_CRGPERIPH_PERDIS12_ADDR(base)                     ((base) + (0x474UL))

/* Register description: DSS DVFS状态机维测状态寄存器
   Bit domain definition UNION:  SOC_CRGPERIPH_HW_DSS_DVFS_STAT_UNION */
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_ADDR(base)             ((base) + (0x668UL))

/* Register description: PLL状态机非安全投票寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_UNION */
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ADDR(base)             ((base) + (0x950UL))

/* Register description: PLL状态机非安全投票寄存器1。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_UNION */
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ADDR(base)             ((base) + (0x954UL))

/* Register description: PPLL2_B控制寄存器0
   Bit domain definition UNION:  SOC_CRGPERIPH_PPLL2_BCTRL0_UNION */
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ADDR(base)                 ((base) + (0xA84UL))

/* Register description: PPLL2_B控制寄存器1
   Bit domain definition UNION:  SOC_CRGPERIPH_PPLL2_BCTRL1_UNION */
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ADDR(base)                 ((base) + (0xA88UL))

/* Register description: PLL_FSM控制寄存器
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_CTRL1_UNION */
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_ADDR(base)                ((base) + (0xC44UL))

/* Register description: PLL_FSM状态回读寄存器。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_STAT0_UNION */
#define SOC_CRGPERIPH_PLL_FSM_STAT0_ADDR(base)                ((base) + (0xC48UL))

/* Register description: PLL_FSM状态回读寄存器。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_STAT1_UNION */
#define SOC_CRGPERIPH_PLL_FSM_STAT1_ADDR(base)                ((base) + (0xC4CUL))

/* Register description: PLL投票状态回读寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_VOTE_STAT0_UNION */
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ADDR(base)               ((base) + (0xC50UL))

/* Register description: PLL使能投票寄存器1。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_UNION */
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ADDR(base)            ((base) + (0xC60UL))

/* Register description: PLL使能投票寄存器1。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_UNION */
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ADDR(base)            ((base) + (0xC6CUL))

#else

/* Register description: 用于软件使用的存储数据寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA0_UNION */
#define SOC_SCTRL_SCBAKDATA0_ADDR(base)                       ((base) + (0x40C))

/* Register description: 用于软件使用的存储数据寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA4_UNION */
#define SOC_SCTRL_SCBAKDATA4_ADDR(base)                       ((base) + (0x41C))

/* Register description: 用于软件使用的存储数据寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA11_UNION */
#define SOC_SCTRL_SCBAKDATA11_ADDR(base)                      ((base) + (0x438))

 /* Register description: PERI投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_ADDR(base)           ((base) + 0x01C)

/* Register description: DSS SW初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_ADDR(base)          ((base) + 0x030)

/* Register description: DSS DIV初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_ADDR(base)         ((base) + 0x034)

/* Register description: DSS PLL EN以及GT初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_UNION */
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_ADDR(base)            ((base) + 0x038)

/* Register description: DSS 电压频点初始化触发配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_ADDR(base)          ((base) + 0x040)

/* Register description: DSS DVFS 软通道指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_APB_SF_INSTR_LIST_UNION */
#define SOC_PMCTRL_APB_SF_INSTR_LIST_ADDR(base, sf)           ((base) + 0x070 + (sf) * 4)

/* Register description: DSS DVFS 硬通道指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_APB_HW_INSTR_LIST_UNION */
#define SOC_PMCTRL_APB_HW_INSTR_LIST_ADDR(base, hw)           ((base) + 0x100 + (hw) * 4)

/* Register description: DSS DVFS CNT相关初始化配置1
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_CNT_CFG1_UNION */
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_ADDR(base)               ((base) + 0x044)

/* Register description: DSS DVFS CNT相关初始化配置2
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_CNT_CFG2_UNION */
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_ADDR(base)               ((base) + 0x048)

/* Register description: DSS DVFS CNT相关初始化配置3
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_CNT_CFG3_UNION */
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG3_ADDR(base)               ((base) + 0x04C)

/* Register description: DSS DVFS 中断相关初始化配置
   Bit domain definition UNION:  SOC_PMCTRL_DSS_DVFS_INTR_CFG_UNION */
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_ADDR(base)               ((base) + 0x050)

/* Register description: DSS软通道DVFS相关配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_SF_CH_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_SF_CH_ADDR(base)               ((base) + 0x054)

/* Register description: DSS软通道DVFS相关回读
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_SF_RD_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_ADDR(base)               ((base) + 0x058)

/* Register description: DSS硬通道DVFS相关配置
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_HW_CH_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_ADDR(base)               ((base) + 0x05C)

/* Register description: DSS硬通道DVFS相关回读
   Bit domain definition UNION:  SOC_PMCTRL_HW_DSS_DVFS_HW_RD_UNION */
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_ADDR(base)               ((base) + 0x060)

/* Register description: DSS硬通道DVFS使能相关配置
   Bit domain definition UNION:  SOC_PMCTRL_APB_DSS_DVFS_HW_EN_UNION */
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_ADDR(base)              ((base) + 0x064)

/* Register description: DSS接口相关信号debug配置
   Bit domain definition UNION:  SOC_PMCTRL_APB_DEBUG_CFG_UNION */
#define SOC_PMCTRL_APB_DEBUG_CFG_ADDR(base)                   ((base) + 0x1D0)

/* Register description: PERI_M1 DVS 状态回读
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M1_DVS_STAT3_UNION */
#define SOC_PMCTRL_PERI_M1_DVS_STAT3_ADDR(base)               ((base) + 0x334)

/* Register description: PERI_M1 DVS 状态回读
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M1_DVS_STAT7_UNION */
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_ADDR(base)               ((base) + 0x348)

/* Register description: 外设通用控制寄存器5
   Bit domain definition UNION:  SOC_PMCTRL_PERI_CTRL5_UNION */
#define SOC_PMCTRL_PERI_CTRL5_ADDR(base)                      ((base) + 0x354)

/* Register description: BUCK5投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_ADDR(base)          ((base) + 0x3A8)

/* Register description: VDDC_MEM投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_ADDR(base)       ((base) + 0x3E4)

/* Register description: PERI_M1投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_ADDR(base)        ((base) + 0x524)

/* Register description: PERI_M2投票源软件调压完成中断MSK
   Bit domain definition UNION:  SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_UNION */
#define SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_ADDR(base)        ((base) + 0x558)

/* Register description: PERI DSS AVS电压参数0
   Bit domain definition UNION:  SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_UNION */
#define SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_ADDR(base)          ((base) + 0xA40)

/* Register description: 外设时钟使能状态寄存器2。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_PERCLKEN1_UNION */
#define SOC_MEDIA1_CRG_PERCLKEN1_ADDR(base)                   ((base) + 0x018)

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_PERRSTSTAT0_UNION */
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ADDR(base)                 ((base) + 0x038)

/* Register description: 时钟分频比控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_CLKDIV0_UNION */
#define SOC_MEDIA1_CRG_CLKDIV0_ADDR(base)                     ((base) + 0x060)

/* Register description: 时钟分频比控制寄存器2。
   Bit domain definition UNION:  SOC_MEDIA1_CRG_CLKDIV2_UNION */
#define SOC_MEDIA1_CRG_CLKDIV2_ADDR(base)                     ((base) + 0x068)

/* Register description: dss dvfs使能寄存器
   Bit domain definition UNION:  SOC_MEDIA1_CRG_DSS_DVFS_UNION */
#define SOC_MEDIA1_CRG_DSS_DVFS_ADDR(base)                    ((base) + 0x100)

/* Register description: dss dvfs状态回读寄存器
   Bit domain definition UNION:  SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_UNION */
#define SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_ADDR(base)            ((base) + 0x104)

/* Register description: 外设软复位使能寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERRSTEN0_UNION */
#define SOC_CRGPERIPH_PERRSTEN0_ADDR(base)                    ((base) + (0x060))

 /* Register description: 外设软复位撤离寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERRSTDIS0_UNION */
#define SOC_CRGPERIPH_PERRSTDIS0_ADDR(base)                   ((base) + (0x064))

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERRSTSTAT0_UNION */
#define SOC_CRGPERIPH_PERRSTSTAT0_ADDR(base)                  ((base) + (0x068))

/* Register description: 外设时钟使能寄存器12。
   Bit domain definition UNION:  SOC_CRGPERIPH_PEREN12_UNION */
#define SOC_CRGPERIPH_PEREN12_ADDR(base)                      ((base) + (0x470))

/* Register description: 外设时钟禁止寄存器12。
   Bit domain definition UNION:  SOC_CRGPERIPH_PERDIS12_UNION */
#define SOC_CRGPERIPH_PERDIS12_ADDR(base)                     ((base) + (0x474))

/* Register description: DSS DVFS状态机维测状态寄存器
   Bit domain definition UNION:  SOC_CRGPERIPH_HW_DSS_DVFS_STAT_UNION */
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_ADDR(base)             ((base) + (0x668))

/* Register description: PLL状态机非安全投票寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_UNION */
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ADDR(base)             ((base) + (0x950))

/* Register description: PLL状态机非安全投票寄存器1。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_UNION */
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ADDR(base)             ((base) + (0x954))

/* Register description: PPLL2_B控制寄存器0
   Bit domain definition UNION:  SOC_CRGPERIPH_PPLL2_BCTRL0_UNION */
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ADDR(base)                 ((base) + (0xA84))

/* Register description: PPLL2_B控制寄存器1
   Bit domain definition UNION:  SOC_CRGPERIPH_PPLL2_BCTRL1_UNION */
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ADDR(base)                 ((base) + (0xA88))

/* Register description: PLL_FSM控制寄存器
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_CTRL1_UNION */
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_ADDR(base)                ((base) + (0xC44))

/* Register description: PLL_FSM状态回读寄存器。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_STAT0_UNION */
#define SOC_CRGPERIPH_PLL_FSM_STAT0_ADDR(base)                ((base) + (0xC48))

/* Register description: PLL_FSM状态回读寄存器。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_FSM_STAT1_UNION */
#define SOC_CRGPERIPH_PLL_FSM_STAT1_ADDR(base)                ((base) + (0xC4C))

/* Register description: PLL投票状态回读寄存器0。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_VOTE_STAT0_UNION */
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ADDR(base)               ((base) + (0xC50))

/* Register description: PLL使能投票寄存器1。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_UNION */
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ADDR(base)            ((base) + (0xC60))

/* Register description: PLL使能投票寄存器1。
   Bit domain definition UNION:  SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_UNION */
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ADDR(base)            ((base) + (0xC6C))

#endif /*__SOC_H_FOR_ASM__*/

/* BL31 & Kernel share this 64K memory*/
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE     0x10941000
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE     (0x10000)


/****************************************************************************
                            UNION_REDEFINE
 ****************************************************************************/\

/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA0_UNION
 struct description   : SCBAKDATA0 Register structure definition
                        Address Offset:0x40C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器0，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA0_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA0_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA0_bakdata_END    (31)

/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA4_UNION
 struct description   : SCBAKDATA4 Register structure definition
                        Address Offset:0x41C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器4，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA4_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA4_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA4_bakdata_END    (31)

/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA11_UNION
 struct description   : SCBAKDATA11 Register structure definition
                        Address Offset:0x438 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器11
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器11，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA11_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA11_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA11_bakdata_END    (31)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION
 struct description   : PERI_INTR_M3ORCPU_MSK Register structure definition
                        Address Offset:0x01C Initial:0x0000 Width:32
 register description : PERI投票源软件调压完成中断MSK
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_intr_lpm3_msk : 16; /* bit[0-15] : bit0表示投票源0送给LPMCU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
        unsigned int  peri_intr_cpu_msk  : 16; /* bit[16-31]: bit0表示投票源0送给ACPU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
    } reg;
} SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_UNION;
#endif
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_lpm3_msk_START  (0)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_lpm3_msk_END    (15)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_cpu_msk_START   (16)
#define SOC_PMCTRL_PERI_INTR_M3ORCPU_MSK_peri_intr_cpu_msk_END     (31)

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_UNION
 struct description   : HW_DSS_DVFS_APB_SW_CFG Register structure definition
                        Address Offset:0x030 Initial:0x0000000 Width:32
 register description : DSS SW初始化配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_sw      : 4;  /* bit[0-3] : sw 初始化配置 */
        unsigned int  apb_sw_init : 1;  /* bit[4]   : sw init请求，脉冲有效，1有效 */
        unsigned int  reserved    : 27; /* bit[5-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_UNION;
#endif
#define SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_apb_sw_START       (0)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_apb_sw_END         (3)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_apb_sw_init_START  (4)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_SW_CFG_apb_sw_init_END    (4)

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_UNION
 struct description   : HW_DSS_DVFS_APB_DIV_CFG Register structure definition
                        Address Offset:0x034 Initial:0x0000000 Width:32
 register description : DSS DIV初始化配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_div      : 6;  /* bit[0-5] : div 初始化配置 */
        unsigned int  apb_div_init : 1;  /* bit[6]   : div init请求，脉冲有效，1有效 */
        unsigned int  reserved     : 25; /* bit[7-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_UNION;
#endif
#define SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_apb_div_START       (0)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_apb_div_END         (5)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_apb_div_init_START  (6)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_DIV_CFG_apb_div_init_END    (6)

/*****************************************************************************
 struct               : SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_UNION
 struct description   : APB_DSS_DVFS_PLL_CFG Register structure definition
                        Address Offset:0x038 Initial:0x0000 Width:32
 register description : DSS PLL EN以及GT初始化配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_pll_en0 : 1;  /* bit[0]    : pll en0,1有效 */
        unsigned int  apb_pll_en1 : 1;  /* bit[1]    : pll en1,1有效 */
        unsigned int  apb_pll_en2 : 1;  /* bit[2]    : pll en2,1有效 */
        unsigned int  apb_pll_en3 : 1;  /* bit[3]    : pll en3,1有效 */
        unsigned int  apb_pll_gt0 : 1;  /* bit[4]    : pll gt0,1有效 */
        unsigned int  apb_pll_gt1 : 1;  /* bit[5]    : pll gt1,1有效 */
        unsigned int  apb_pll_gt2 : 1;  /* bit[6]    : pll gt2,1有效 */
        unsigned int  apb_pll_gt3 : 1;  /* bit[7]    : pll gt3,1有效 */
        unsigned int  reserved_0  : 8;  /* bit[8-15] :  */
        unsigned int  reserved_1  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_UNION;
#endif
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en0_START  (0)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en0_END    (0)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en1_START  (1)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en1_END    (1)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en2_START  (2)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en2_END    (2)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en3_START  (3)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_en3_END    (3)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt0_START  (4)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt0_END    (4)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt1_START  (5)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt1_END    (5)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt2_START  (6)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt2_END    (6)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt3_START  (7)
#define SOC_PMCTRL_APB_DSS_DVFS_PLL_CFG_apb_pll_gt3_END    (7)

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_UNION
 struct description   : HW_DSS_DVFS_APB_VF_CFG Register structure definition
                        Address Offset:0x040 Initial:0x000000 Width:32
 register description : DSS 电压频点初始化触发配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_vol_chg_init_pulse : 1;  /* bit[0]    : vol chg init请求，脉冲有效，1有效，delay div init配置 */
        unsigned int  apb_div_chg_init_pulse : 1;  /* bit[1]    : div chg init请求，脉冲有效，1有效，delay div init配置 */
        unsigned int  apb_sw_chg_init_pulse  : 1;  /* bit[2]    : sw chg init请求，脉冲有效，1有效，delay sw init配置 */
        unsigned int  apb_pll_en3_init       : 1;  /* bit[3]    : pll en3，脉冲有效，1有效 */
        unsigned int  apb_pll_en2_init       : 1;  /* bit[4]    : pll en2，脉冲有效，1有效 */
        unsigned int  apb_pll_en1_init       : 1;  /* bit[5]    : pll en1，脉冲有效，1有效 */
        unsigned int  apb_pll_en0_init       : 1;  /* bit[6]    : pll en0，脉冲有效，1有效 */
        unsigned int  apb_pll_gt3_init       : 1;  /* bit[7]    : pll gt3，脉冲有效，1有效 */
        unsigned int  apb_pll_gt2_init       : 1;  /* bit[8]    : pll gt2，脉冲有效，1有效 */
        unsigned int  apb_pll_gt1_init       : 1;  /* bit[9]    : pll gt1，脉冲有效，1有效 */
        unsigned int  apb_pll_gt0_init       : 1;  /* bit[10]   : pll gt0，脉冲有效，1有效 */
        unsigned int  reserved               : 21; /* bit[11-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_UNION;
#endif
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_vol_chg_init_pulse_START  (0)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_vol_chg_init_pulse_END    (0)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_div_chg_init_pulse_START  (1)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_div_chg_init_pulse_END    (1)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_sw_chg_init_pulse_START   (2)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_sw_chg_init_pulse_END     (2)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en3_init_START        (3)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en3_init_END          (3)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en2_init_START        (4)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en2_init_END          (4)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en1_init_START        (5)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en1_init_END          (5)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en0_init_START        (6)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_en0_init_END          (6)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt3_init_START        (7)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt3_init_END          (7)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt2_init_START        (8)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt2_init_END          (8)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt1_init_START        (9)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt1_init_END          (9)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt0_init_START        (10)
#define SOC_PMCTRL_HW_DSS_DVFS_APB_VF_CFG_apb_pll_gt0_init_END          (10)

/*****************************************************************************
 struct               : SOC_PMCTRL_APB_SF_INSTR_LIST_UNION
 struct description   : APB_SF_INSTR_LIST Register structure definition
                        Address Offset:0x070+ (sf)*4 Initial:0x00000000 Width:32
 register description : DSS DVFS 软通道指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: sf配置范围0~5 */
    } reg;
} SOC_PMCTRL_APB_SF_INSTR_LIST_UNION;
#endif

/*****************************************************************************
 struct               : SOC_PMCTRL_APB_HW_INSTR_LIST_UNION
 struct description   : APB_HW_INSTR_LIST Register structure definition
                        Address Offset:0x100+ (hw)*4 Initial:0x00000000 Width:32
 register description : DSS DVFS 硬通道指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: hw配置范围0~20 */
    } reg;
} SOC_PMCTRL_APB_HW_INSTR_LIST_UNION;
#endif

/*****************************************************************************
 struct               : SOC_PMCTRL_DSS_DVFS_CNT_CFG1_UNION
 struct description   : DSS_DVFS_CNT_CFG1 Register structure definition
                        Address Offset:0x044 Initial:0x0 Width:32
 register description : DSS DVFS CNT相关初始化配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_pclk_cnt_us    : 10; /* bit[0-9]  : 使用默认值 */
        unsigned int  apb_vol_stable_cnt : 4;  /* bit[10-13]: 支持可配范围0x1~0xF */
        unsigned int  apb_sw_stable_cnt  : 4;  /* bit[14-17]: 支持可配范围0x1~0xE */
        unsigned int  apb_sw_ack_cnt     : 8;  /* bit[18-25]: 支持可配范围0x6~0xff */
        unsigned int  apb_div_stable_cnt : 4;  /* bit[26-29]: 支持可配范围0x1~0xE */
        unsigned int  reserved           : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_DSS_DVFS_CNT_CFG1_UNION;
#endif
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_pclk_cnt_us_START     (0)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_pclk_cnt_us_END       (9)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_vol_stable_cnt_START  (10)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_vol_stable_cnt_END    (13)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_sw_stable_cnt_START   (14)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_sw_stable_cnt_END     (17)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_sw_ack_cnt_START      (18)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_sw_ack_cnt_END        (25)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_div_stable_cnt_START  (26)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG1_apb_div_stable_cnt_END    (29)

/*****************************************************************************
 struct               : SOC_PMCTRL_DSS_DVFS_CNT_CFG2_UNION
 struct description   : DSS_DVFS_CNT_CFG2 Register structure definition
                        Address Offset:0x048 Initial:0x0 Width:32
 register description : DSS DVFS CNT相关初始化配置2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_div_ack_cnt       : 8;  /* bit[0-7]  : 支持可配范围0x6~0xff */
        unsigned int  apb_pll_lock_mode     : 1;  /* bit[8]    : 1等待pll lock信号，0等待lock time */
        unsigned int  apb_lock_time         : 13; /* bit[9-21] : 默认600ns左右 */
        unsigned int  apb_pll_gt_stable_cnt : 4;  /* bit[22-25]: 支持可配范围0x1~0xF */
        unsigned int  reserved_0            : 4;  /* bit[26-29]:  */
        unsigned int  reserved_1            : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_DSS_DVFS_CNT_CFG2_UNION;
#endif
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_div_ack_cnt_START        (0)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_div_ack_cnt_END          (7)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_pll_lock_mode_START      (8)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_pll_lock_mode_END        (8)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_lock_time_START          (9)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_lock_time_END            (21)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_pll_gt_stable_cnt_START  (22)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG2_apb_pll_gt_stable_cnt_END    (25)

/*****************************************************************************
 struct               : SOC_PMCTRL_DSS_DVFS_CNT_CFG3_UNION
 struct description   : DSS_DVFS_CNT_CFG3 Register structure definition
                        Address Offset:0x04C Initial:0x000 Width:32
 register description : DSS DVFS CNT相关初始化配置3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_reload_instr_stable_cnt : 16; /* bit[0-15] : 默认361ns左右 */
        unsigned int  reserved_0                  : 1;  /* bit[16]   :  */
        unsigned int  reserved_1                  : 1;  /* bit[17]   :  */
        unsigned int  reserved_2                  : 1;  /* bit[18]   :  */
        unsigned int  reserved_3                  : 1;  /* bit[19]   :  */
        unsigned int  reserved_4                  : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_DSS_DVFS_CNT_CFG3_UNION;
#endif
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG3_apb_reload_instr_stable_cnt_START  (0)
#define SOC_PMCTRL_DSS_DVFS_CNT_CFG3_apb_reload_instr_stable_cnt_END    (15)

/*****************************************************************************
 struct               : SOC_PMCTRL_DSS_DVFS_INTR_CFG_UNION
 struct description   : DSS_DVFS_INTR_CFG Register structure definition
                        Address Offset:0x050 Initial:0x0000000 Width:32
 register description : DSS DVFS 中断相关初始化配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_intr_sf_dvfs_bypass      : 1;  /* bit[0]   : 1 中断屏蔽，采用查询方式回读单次DVFS状态机完成状态，
                                                                       0 采用中断方式 */
        unsigned int  apb_intr_sf_dvfs_m3orcpu_msk : 1;  /* bit[1]   : 1 中断送给m3,0中断送给Acpu */
        unsigned int  apb_intr_hw_dvfs_bypass      : 1;  /* bit[2]   : 1 中断屏蔽，采用查询方式回读单次DVFS状态机完成状态，
                                                                       0 采用中断方式 */
        unsigned int  apb_intr_hw_dvfs_m3orcpu_msk : 1;  /* bit[3]   : 1 中断送给m3,0中断送给Acpu */
        unsigned int  apb_inter_instr_bypass       : 1;  /* bit[4]   : 1 中断屏蔽，查询方式回读中断指令产生的中断，
                                                                       0 中断打开，查询方式回读中断指令产生的中断 */
        unsigned int  reserved                     : 27; /* bit[5-31]:  */
    } reg;
} SOC_PMCTRL_DSS_DVFS_INTR_CFG_UNION;
#endif
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_sf_dvfs_bypass_START       (0)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_sf_dvfs_bypass_END         (0)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_sf_dvfs_m3orcpu_msk_START  (1)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_sf_dvfs_m3orcpu_msk_END    (1)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_hw_dvfs_bypass_START       (2)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_hw_dvfs_bypass_END         (2)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_hw_dvfs_m3orcpu_msk_START  (3)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_intr_hw_dvfs_m3orcpu_msk_END    (3)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_inter_instr_bypass_START        (4)
#define SOC_PMCTRL_DSS_DVFS_INTR_CFG_apb_inter_instr_bypass_END          (4)

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_SF_CH_UNION
 struct description   : HW_DSS_DVFS_SF_CH Register structure definition
                        Address Offset:0x054 Initial:0x0000000 Width:32
 register description : DSS软通道DVFS相关配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_intr_sf_dvfs_clr : 1;  /* bit[0]   : 软通道中断清除配置，1有效 */
        unsigned int  apb_sf_escape        : 1;  /* bit[1]   : 软通道指令退出配置，1有效 */
        unsigned int  apb_sf_next_step     : 1;  /* bit[2]   : 软通道halt指令，下一步执行配置，1有效 */
        unsigned int  apb_sf_dvfs_en       : 1;  /* bit[3]   : 软通道DVFS状态机使能，1有效 */
        unsigned int  apb_sf_instr_clr     : 1;  /* bit[4]   : 软通道指令清除，1有效 */
        unsigned int  reserved             : 27; /* bit[5-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_SF_CH_UNION;
#endif

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_SF_RD_UNION
 struct description   : HW_DSS_DVFS_SF_RD Register structure definition
                        Address Offset:0x058 Initial:0x0000 Width:32
 register description : DSS软通道DVFS相关回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_rd_sf_cur_instr_num      : 4;  /* bit[0-3]  : 软通道DVFS状态机当前指令number */
        unsigned int  apb_rd_sf_cur_stat           : 4;  /* bit[4-7]  : 软通道DVFS状态机当前状态 */
        unsigned int  apb_rd_sf_ch_vol_idx         : 8;  /* bit[8-15] : 软通道DVFS状态机当前取到AVS电压 */
        unsigned int  apb_rd_intr_sf_dvfs_raw_stat : 1;  /* bit[16]   : 软通道DVFS状态机，单次完成原始状态（中断屏蔽前） */
        unsigned int  apb_rd_intr_sf_dvfs_stat     : 1;  /* bit[17]   : 软通道DVFS状态机，单次完成状态（中断屏蔽后） */
        unsigned int  apb_rd_sf_ch_dvfs_busy       : 1;  /* bit[18]   : 软通道DVFS状态机busy状态回读 */
        unsigned int  reserved                     : 13; /* bit[19-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_SF_RD_UNION;
#endif
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_cur_instr_num_START       (0)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_cur_instr_num_END         (3)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_cur_stat_START            (4)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_cur_stat_END              (7)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_ch_vol_idx_START          (8)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_ch_vol_idx_END            (15)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_intr_sf_dvfs_raw_stat_START  (16)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_intr_sf_dvfs_raw_stat_END    (16)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_intr_sf_dvfs_stat_START      (17)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_intr_sf_dvfs_stat_END        (17)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_ch_dvfs_busy_START        (18)
#define SOC_PMCTRL_HW_DSS_DVFS_SF_RD_apb_rd_sf_ch_dvfs_busy_END          (18)

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_HW_CH_UNION
 struct description   : HW_DSS_DVFS_HW_CH Register structure definition
                        Address Offset:0x05C Initial:0x000 Width:32
 register description : DSS硬通道DVFS相关配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_intr_hw_dvfs_clr           : 1;  /* bit[0]    : 硬通道中断清除配置，1有效 */
        unsigned int  apb_hw_escape                  : 1;  /* bit[1]    : 硬通道指令退出配置，1有效 */
        unsigned int  apb_hw_next_step               : 1;  /* bit[2]    : 硬通道halt指令，下一步执行配置，1有效 */
        unsigned int  apb_hw_flush_pls_init          : 1;  /* bit[3]    : 硬通道DVFS状态机enable第一次写完指令配置，将影子指令空间load进执行指令空间 */
        unsigned int  apb_hw_instr_clr               : 1;  /* bit[4]    : 硬通道指令清除，1有效 */
        unsigned int  reserved_0                     : 1;  /* bit[5]    :  */
        unsigned int  apb_hw_dss2ddr_dfs_intr_clr    : 1;  /* bit[6]    : dss2ddr硬通道中断清除配置，1有效 */
        unsigned int  apb_hw_dss2ddr_escape          : 1;  /* bit[7]    : dss2ddr硬通道指令退出配置，1有效 */
        unsigned int  apb_hw_dss2ddr_next_step       : 1;  /* bit[8]    : dss2ddr硬通道halt指令，下一步执行配置，1有效 */
        unsigned int  apb_hw_dss2ddr_flush_pls_init  : 1;  /* bit[9]    : dss2ddr硬通道DVFS状态机enable第一次写完指令配置，将影子指令空间load进执行指令空间 */
        unsigned int  apb_dss2ddr_instr_clr          : 1;  /* bit[10]   : dss2ddr硬通道指令清除，1有效 */
        unsigned int  reserved_1                     : 1;  /* bit[11]   :  */
        unsigned int  apb_hw_dss2qice_dfs_intr_clr   : 1;  /* bit[12]   : dss2qice硬通道中断清除配置，1有效 */
        unsigned int  apb_hw_dss2qice_escape         : 1;  /* bit[13]   : dss2qice硬通道指令退出配置，1有效 */
        unsigned int  apb_hw_dss2qice_next_step      : 1;  /* bit[14]   : dss2qice硬通道halt指令，下一步执行配置，1有效 */
        unsigned int  apb_hw_dss2qice_flush_pls_init : 1;  /* bit[15]   : dss2qice硬通道DVFS状态机enable第一次写完指令配置，将影子指令空间load进执行指令空间 */
        unsigned int  apb_dss2qice_instr_clr         : 1;  /* bit[16]   : dss2qice硬通道指令清除，1有效 */
        unsigned int  reserved_2                     : 1;  /* bit[17]   :  */
        unsigned int  reserved_3                     : 1;  /* bit[18]   :  */
        unsigned int  reserved_4                     : 1;  /* bit[19]   :  */
        unsigned int  reserved_5                     : 12; /* bit[20-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_HW_CH_UNION;
#endif
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_intr_hw_dvfs_clr_START            (0)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_intr_hw_dvfs_clr_END              (0)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_escape_START                   (1)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_escape_END                     (1)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_next_step_START                (2)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_next_step_END                  (2)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_flush_pls_init_START           (3)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_flush_pls_init_END             (3)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_instr_clr_START                (4)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_instr_clr_END                  (4)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_dfs_intr_clr_START     (6)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_dfs_intr_clr_END       (6)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_escape_START           (7)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_escape_END             (7)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_next_step_START        (8)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_next_step_END          (8)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_flush_pls_init_START   (9)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2ddr_flush_pls_init_END     (9)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_dss2ddr_instr_clr_START           (10)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_dss2ddr_instr_clr_END             (10)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_dfs_intr_clr_START    (12)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_dfs_intr_clr_END      (12)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_escape_START          (13)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_escape_END            (13)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_next_step_START       (14)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_next_step_END         (14)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_flush_pls_init_START  (15)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_hw_dss2qice_flush_pls_init_END    (15)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_dss2qice_instr_clr_START          (16)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_CH_apb_dss2qice_instr_clr_END            (16)

/*****************************************************************************
 struct               : SOC_PMCTRL_HW_DSS_DVFS_HW_RD_UNION
 struct description   : HW_DSS_DVFS_HW_RD Register structure definition
                        Address Offset:0x060 Initial:0x000 Width:32
 register description : DSS硬通道DVFS相关回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_rd_hw_cur_instr_num      : 6;  /* bit[0-5]  : 硬通道DVFS状态机当前指令number */
        unsigned int  apb_rd_hw_cur_stat           : 4;  /* bit[6-9]  : 硬通道DVFS状态机当前状态 */
        unsigned int  apb_rd_hw_ch_vol_idx         : 8;  /* bit[10-17]: 硬通道DVFS状态机当前取到AVS电压 */
        unsigned int  apb_rd_intr_hw_dvfs_raw_stat : 1;  /* bit[18]   : 硬通道DVFS状态机，单次完成原始状态（中断屏蔽前） */
        unsigned int  apb_rd_intr_hw_dvfs_stat     : 1;  /* bit[19]   : 硬通道DVFS状态机，单次完成状态（中断屏蔽后） */
        unsigned int  apb_rd_hw_ch_dvfs_busy       : 1;  /* bit[20]   : 硬通道DVFS状态机busy状态回读 */
        unsigned int  apb_rd_inter_instr_raw_stat  : 1;  /* bit[21]   : 中断指令，产的中断（中断屏蔽前） */
        unsigned int  apb_rd_inter_instr_stat      : 1;  /* bit[22]   : 中断指令，产的中断（中断屏蔽后） */
        unsigned int  reserved                     : 9;  /* bit[23-31]:  */
    } reg;
} SOC_PMCTRL_HW_DSS_DVFS_HW_RD_UNION;
#endif
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_cur_instr_num_START       (0)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_cur_instr_num_END         (5)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_cur_stat_START            (6)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_cur_stat_END              (9)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_ch_vol_idx_START          (10)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_ch_vol_idx_END            (17)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_intr_hw_dvfs_raw_stat_START  (18)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_intr_hw_dvfs_raw_stat_END    (18)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_intr_hw_dvfs_stat_START      (19)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_intr_hw_dvfs_stat_END        (19)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_ch_dvfs_busy_START        (20)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_hw_ch_dvfs_busy_END          (20)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_inter_instr_raw_stat_START   (21)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_inter_instr_raw_stat_END     (21)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_inter_instr_stat_START       (22)
#define SOC_PMCTRL_HW_DSS_DVFS_HW_RD_apb_rd_inter_instr_stat_END         (22)

/*****************************************************************************
 struct               : SOC_PMCTRL_APB_DSS_DVFS_HW_EN_UNION
 struct description   : APB_DSS_DVFS_HW_EN Register structure definition
                        Address Offset:0x064 Initial:0x0000 Width:32
 register description : DSS硬通道DVFS使能相关配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_hw_dvfs_enable                : 1;  /* bit[0]    : 硬通道DVFS使能，默认不使能 */
        unsigned int  apb_hw_instr_lock                 : 1;  /* bit[1]    : 硬通道指令lock，防误写，刷新指令步骤，先配置0 unlock指令，再配置上面0x05c清指令，再写指令，再配置1 lock指令 */
        unsigned int  reserved_0                        : 1;  /* bit[2]    :  */
        unsigned int  apb_hw_dss2ddr_dfs_enable         : 1;  /* bit[3]    : dss2ddr硬通道DVFS使能，默认不使能 */
        unsigned int  apb_dss2ddr_instr_lock            : 1;  /* bit[4]    : dss2ddr硬通道指令lock，防误写，刷新指令步骤，先配置0 unlock指令，再配置上面0x05c清指令，再写指令，再配置1 lock指令 */
        unsigned int  reserved_1                        : 1;  /* bit[5]    :  */
        unsigned int  apb_hw_dss2qice_dfs_enable        : 1;  /* bit[6]    : dss2qice硬通道DVFS使能，默认不使能 */
        unsigned int  apb_dss2qice_instr_lock           : 1;  /* bit[7]    : dss2qice硬通道指令lock，防误写，刷新指令步骤，先配置0 unlock指令，再配置上面0x05c清指令，再写指令，再配置1 lock指令 */
        unsigned int  reserved_2                        : 1;  /* bit[8]    :  */
        unsigned int  apb_hw_ch_fps_cnt                 : 3;  /* bit[9-11] : DSS DVFS和DSS2DDR以及DSS2QICE，SF ch/ HW ch flush_en使能一次后hw ch连续几帧不生效，默认1帧不生效，过渡1帧 */
        unsigned int  apb_hw_ch_fps_cnt_dss_bypass      : 1;  /* bit[12]   : DSS DVFS bypass掉过渡帧，1生效，0代表执行下方配置的过渡帧 */
        unsigned int  apb_hw_ch_fps_cnt_dss2qice_bypass : 1;  /* bit[13]   : DSS2QICE DFS bypass掉过渡帧,1生效，0代表执行下方过渡帧 */
        unsigned int  apb_hw_ch_fps_cnt_dss2ddr_bypass  : 1;  /* bit[14]   : DSS2DDR DFS bypass掉过渡帧,1生效，0代表执行下方过渡帧 */
        unsigned int  reserved_3                        : 1;  /* bit[15]   :  */
        unsigned int  reserved_4                        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_APB_DSS_DVFS_HW_EN_UNION;
#endif
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_dvfs_enable_START                 (0)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_dvfs_enable_END                   (0)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_instr_lock_START                  (1)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_instr_lock_END                    (1)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_dss2ddr_dfs_enable_START          (3)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_dss2ddr_dfs_enable_END            (3)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_dss2ddr_instr_lock_START             (4)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_dss2ddr_instr_lock_END               (4)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_dss2qice_dfs_enable_START         (6)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_dss2qice_dfs_enable_END           (6)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_dss2qice_instr_lock_START            (7)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_dss2qice_instr_lock_END              (7)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_START                  (9)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_END                    (11)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_dss_bypass_START       (12)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_dss_bypass_END         (12)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_dss2qice_bypass_START  (13)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_dss2qice_bypass_END    (13)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_dss2ddr_bypass_START   (14)
#define SOC_PMCTRL_APB_DSS_DVFS_HW_EN_apb_hw_ch_fps_cnt_dss2ddr_bypass_END     (14)

/*****************************************************************************
 struct               : SOC_PMCTRL_APB_DEBUG_CFG_UNION
 struct description   : APB_DEBUG_CFG Register structure definition
                        Address Offset:0x1D0 Initial:0x00000 Width:32
 register description : DSS接口相关信号debug配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apb_dss2dq_debug_en             : 1;  /* bit[0]    : DSS2DQ debug使能位，1生效，默认为0 */
        unsigned int  apb_dss2dq_valid_cnt_clr        : 1;  /* bit[1]    : dss2dq_chg_valid cnt 清0 */
        unsigned int  apb_dss2dq_chg_cnt_clr          : 1;  /* bit[2]    : dss2dq_chg cnt 清0 */
        unsigned int  apb_flush_cnt_clr               : 1;  /* bit[3]    : flush_cnt 清0 */
        unsigned int  apb_vsync_cnt_clr               : 1;  /* bit[4]    : vsync cnt 清0 */
        unsigned int  reserved_0                      : 1;  /* bit[5]    : DSS2DDR 软通道频率请求上调，1代表上调，0代表下调 */
        unsigned int  apb_dss_dvfs_debug_en           : 1;  /* bit[6]    : DSS DVFS debug使能位，1生效，默认为0 */
        unsigned int  apb_dss_dvfs_valid_cnt_clr      : 1;  /* bit[7]    : dss_dvfs_chg_valid cnt 清0 */
        unsigned int  apb_dss_dvfs_chg_cnt_clr        : 1;  /* bit[8]    : dss_dvfs_chg cnt 清0 */
        unsigned int  apb_dss_doze1_wake_chg_cnt_clr  : 1;  /* bit[9]    : dss_doze1_wake_chg cnt 清0 */
        unsigned int  apb_dss_doze1_enter_chg_cnt_clr : 1;  /* bit[10]   : dss_doze1_enter_chg cnt 清0 */
        unsigned int  apb_inter_instr_clr             : 1;  /* bit[11]   : 中断指令inter cnt 清0 */
        unsigned int  reserved_1                      : 20; /* bit[12-31]:  */
    } reg;
} SOC_PMCTRL_APB_DEBUG_CFG_UNION;
#endif
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss2dq_debug_en_START              (0)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss2dq_debug_en_END                (0)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss2dq_valid_cnt_clr_START         (1)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss2dq_valid_cnt_clr_END           (1)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss2dq_chg_cnt_clr_START           (2)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss2dq_chg_cnt_clr_END             (2)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_flush_cnt_clr_START                (3)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_flush_cnt_clr_END                  (3)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_vsync_cnt_clr_START                (4)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_vsync_cnt_clr_END                  (4)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_dvfs_debug_en_START            (6)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_dvfs_debug_en_END              (6)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_dvfs_valid_cnt_clr_START       (7)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_dvfs_valid_cnt_clr_END         (7)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_dvfs_chg_cnt_clr_START         (8)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_dvfs_chg_cnt_clr_END           (8)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_doze1_wake_chg_cnt_clr_START   (9)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_doze1_wake_chg_cnt_clr_END     (9)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_doze1_enter_chg_cnt_clr_START  (10)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_dss_doze1_enter_chg_cnt_clr_END    (10)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_inter_instr_clr_START              (11)
#define SOC_PMCTRL_APB_DEBUG_CFG_apb_inter_instr_clr_END                (11)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_M1_DVS_STAT3_UNION
 struct description   : PERI_M1_DVS_STAT3 Register structure definition
                        Address Offset:0x334 Initial:0x00 Width:32
 register description : PERI_M1 DVS 状态回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_m1_buck_ctrl_cur_vol_idx : 8;  /* bit[0-7]  :  */
        unsigned int  peri_m1_buck_ctrl_nxt_vol_idx : 8;  /* bit[8-15] :  */
        unsigned int  reserved_0                    : 8;  /* bit[16-23]:  */
        unsigned int  reserved_1                    : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_PERI_M1_DVS_STAT3_UNION;
#endif
#define SOC_PMCTRL_PERI_M1_DVS_STAT3_peri_m1_buck_ctrl_cur_vol_idx_START  (0)
#define SOC_PMCTRL_PERI_M1_DVS_STAT3_peri_m1_buck_ctrl_cur_vol_idx_END    (7)
#define SOC_PMCTRL_PERI_M1_DVS_STAT3_peri_m1_buck_ctrl_nxt_vol_idx_START  (8)
#define SOC_PMCTRL_PERI_M1_DVS_STAT3_peri_m1_buck_ctrl_nxt_vol_idx_END    (15)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_M1_DVS_STAT7_UNION
 struct description   : PERI_M1_DVS_STAT7 Register structure definition
                        Address Offset:0x348 Initial:0x00 Width:32
 register description : PERI_M1 DVS 状态回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_m1_valid_vol_vote_idx_12_vau_3 : 8;  /* bit[0-7]  :  */
        unsigned int  peri_m1_valid_vol_vote_idx_13_vau_3 : 8;  /* bit[8-15] :  */
        unsigned int  peri_m1_valid_vol_vote_idx_14_vau_3 : 8;  /* bit[16-23]:  */
        unsigned int  peri_m1_valid_vol_vote_idx_15_vau_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_PERI_M1_DVS_STAT7_UNION;
#endif
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_12_vau_3_START  (0)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_12_vau_3_END    (7)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_13_vau_3_START  (8)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_13_vau_3_END    (15)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_14_vau_3_START  (16)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_14_vau_3_END    (23)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_15_vau_3_START  (24)
#define SOC_PMCTRL_PERI_M1_DVS_STAT7_peri_m1_valid_vol_vote_idx_15_vau_3_END    (31)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_CTRL5_UNION
 struct description   : PERI_CTRL5 Register structure definition
                        Address Offset:0x354 Initial:0x00000000 Width:32
 register description : 外设通用控制寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 外设区降压投票寄存器，软件专用。 */
    } reg;
} SOC_PMCTRL_PERI_CTRL5_UNION;
#endif

/*****************************************************************************
 struct               : SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_UNION
 struct description   : BUCK5_INTR_M3ORCPU_MSK Register structure definition
                        Address Offset:0x3A8 Initial:0x000000 Width:32
 register description : BUCK5投票源软件调压完成中断MSK
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  buck5_intr_lpm3_msk : 4;  /* bit[0-3] : bit0表示投票源0送给LPMCU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
        unsigned int  buck5_intr_acpu_msk : 4;  /* bit[4-7] : bit0表示投票源0送给LPMCU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
        unsigned int  reserved            : 24; /* bit[8-31]:  */
    } reg;
} SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_UNION;
#endif
#define SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_buck5_intr_lpm3_msk_START  (0)
#define SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_buck5_intr_lpm3_msk_END    (3)
#define SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_buck5_intr_acpu_msk_START  (4)
#define SOC_PMCTRL_BUCK5_INTR_M3ORCPU_MSK_buck5_intr_acpu_msk_END    (7)

/*****************************************************************************
 struct               : SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_UNION
 struct description   : VDDC_MEM_INTR_M3ORCPU_MSK Register structure definition
                        Address Offset:0x3E4 Initial:0x0000 Width:32
 register description : VDDC_MEM投票源软件调压完成中断MSK
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vddc_mem_intr_lpm3_msk : 16; /* bit[0-15] : bit0表示投票源0送给LPMCU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
        unsigned int  vddc_mem_intr_cpu_msk  : 16; /* bit[16-31]: bit0表示投票源0送给ACPU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
    } reg;
} SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_UNION;
#endif
#define SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_vddc_mem_intr_lpm3_msk_START  (0)
#define SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_vddc_mem_intr_lpm3_msk_END    (15)
#define SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_vddc_mem_intr_cpu_msk_START   (16)
#define SOC_PMCTRL_VDDC_MEM_INTR_M3ORCPU_MSK_vddc_mem_intr_cpu_msk_END     (31)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_UNION
 struct description   : PERI_M1_INTR_M3ORCPU_MSK Register structure definition
                        Address Offset:0x524 Initial:0x0000 Width:32
 register description : PERI_M1投票源软件调压完成中断MSK
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_m1_intr_lpm3_msk : 16; /* bit[0-15] : bit0表示投票源0送给LPMCU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
        unsigned int  peri_m1_intr_cpu_msk  : 16; /* bit[16-31]: bit0表示投票源0送给ACPU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
    } reg;
} SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_UNION;
#endif
#define SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_peri_m1_intr_lpm3_msk_START  (0)
#define SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_peri_m1_intr_lpm3_msk_END    (15)
#define SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_peri_m1_intr_cpu_msk_START   (16)
#define SOC_PMCTRL_PERI_M1_INTR_M3ORCPU_MSK_peri_m1_intr_cpu_msk_END     (31)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_UNION
 struct description   : PERI_M2_INTR_M3ORCPU_MSK Register structure definition
                        Address Offset:0x558 Initial:0x0000 Width:32
 register description : PERI_M2投票源软件调压完成中断MSK
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_m2_intr_lpm3_msk : 16; /* bit[0-15] : bit0表示投票源0送给LPMCU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
        unsigned int  peri_m2_intr_cpu_msk  : 16; /* bit[16-31]: bit0表示投票源0送给ACPU的中断mask，1：投票源0的中断被屏蔽，0：投票源0的中断打开。其他bit依次类推 */
    } reg;
} SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_UNION;
#endif
#define SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_peri_m2_intr_lpm3_msk_START  (0)
#define SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_peri_m2_intr_lpm3_msk_END    (15)
#define SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_peri_m2_intr_cpu_msk_START   (16)
#define SOC_PMCTRL_PERI_M2_INTR_M3ORCPU_MSK_peri_m2_intr_cpu_msk_END     (31)

/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_UNION
 struct description   : PERI_DSS_AVS_VOL_IDX_0 Register structure definition
                        Address Offset:0xA40 Initial:0x00000000 Width:32
 register description : PERI DSS AVS电压参数0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_dss_avs_vol_idx_0 : 32; /* bit[0-31]: 电压参数分为四档：
                                                                 bit[7:0] 对应 0.6 v profile档位；
                                                                 bit[15:8] 对应 0.65v profile档位；
                                                                 bit[23:16] 对应 0.7 v profile档位；
                                                                 bit[31:24] 对应 0.8 v profile档位； */
    } reg;
} SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_UNION;
#endif
#define SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_peri_dss_avs_vol_idx_0_START  (0)
#define SOC_PMCTRL_PERI_DSS_AVS_VOL_IDX_0_peri_dss_avs_vol_idx_0_END    (31)

/*****************************************************************************
 struct               : SOC_MEDIA1_CRG_PERCLKEN1_UNION
 struct description   : PERCLKEN1 Register structure definition
                        Address Offset:0x018 Initial:0x00000 Width:32
 register description : 外设时钟使能状态寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_hpm                      : 1;  /* bit[0]    :  */
        unsigned int  gt_pclk_autodiv_aclk_vcodec_tcu : 1;  /* bit[1]    :  */
        unsigned int  gt_pclk_autodiv_vdec            : 1;  /* bit[2]    :  */
        unsigned int  gt_pclk_autodiv_venc            : 1;  /* bit[3]    :  */
        unsigned int  gt_pclk_autodiv_dss             : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                      : 1;  /* bit[5]    :  */
        unsigned int  reserved_1                      : 1;  /* bit[6]    :  */
        unsigned int  reserved_2                      : 1;  /* bit[7]    :  */
        unsigned int  reserved_3                      : 1;  /* bit[8]    :  */
        unsigned int  reserved_4                      : 1;  /* bit[9]    :  */
        unsigned int  reserved_5                      : 1;  /* bit[10]   :  */
        unsigned int  reserved_6                      : 1;  /* bit[11]   :  */
        unsigned int  reserved_7                      : 1;  /* bit[12]   :  */
        unsigned int  reserved_8                      : 1;  /* bit[13]   :  */
        unsigned int  reserved_9                      : 18; /* bit[14-31]: 外设时钟使能状态：
                                                                           0：IP时钟使能撤销状态；
                                                                           1：IP时钟使能状态。 */
    } reg;
} SOC_MEDIA1_CRG_PERCLKEN1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_hpm_START                       (0)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_hpm_END                         (0)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_aclk_vcodec_tcu_START  (1)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_aclk_vcodec_tcu_END    (1)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_vdec_START             (2)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_vdec_END               (2)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_venc_START             (3)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_venc_END               (3)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_dss_START              (4)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_dss_END                (4)

/*****************************************************************************
 struct               : SOC_MEDIA1_CRG_PERRSTSTAT0_UNION
 struct description   : PERRSTSTAT0 Register structure definition
                        Address Offset:0x038 Initial:0x00 Width:32
 register description : 外设软复位状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_dss_crg          : 1;  /* bit[0]    :  */
        unsigned int  ip_rst_dss              : 1;  /* bit[1]    :  */
        unsigned int  ip_prst_atgm_dss        : 1;  /* bit[2]    :  */
        unsigned int  ip_prst_atgm_vdec       : 1;  /* bit[3]    :  */
        unsigned int  ip_rst_tcu_m1           : 1;  /* bit[4]    :  */
        unsigned int  ip_prst_apb_brg_tcu     : 1;  /* bit[5]    :  */
        unsigned int  ip_rst_vdec_pxpc        : 1;  /* bit[6]    :  */
        unsigned int  ip_rst_vdec             : 1;  /* bit[7]    :  */
        unsigned int  ip_arst_vdec            : 1;  /* bit[8]    :  */
        unsigned int  ip_prst_vdec            : 1;  /* bit[9]    :  */
        unsigned int  ip_prst_atgm_venc       : 1;  /* bit[10]   :  */
        unsigned int  ip_rst_dss_disp_ch1_crg : 1;  /* bit[11]   :  */
        unsigned int  ip_rst_dss_disp_ch1     : 1;  /* bit[12]   :  */
        unsigned int  ip_rst_media_qic_data   : 1;  /* bit[13]   :  */
        unsigned int  ip_rst_media_qice_data  : 1;  /* bit[14]   :  */
        unsigned int  ip_rst_cs_vdec          : 1;  /* bit[15]   :  */
        unsigned int  ip_rst_cs_venc          : 1;  /* bit[16]   :  */
        unsigned int  ip_rst_venc             : 1;  /* bit[17]   :  */
        unsigned int  ip_prst_venc            : 1;  /* bit[18]   :  */
        unsigned int  ip_arst_venc            : 1;  /* bit[19]   :  */
        unsigned int  ip_rst_hpm              : 1;  /* bit[20]   :  */
        unsigned int  ip_rst_dbg_cs           : 1;  /* bit[21]   :  */
        unsigned int  ip_rst_qic_dss          : 1;  /* bit[22]   :  */
        unsigned int  ip_rst_media1_wrap_crg  : 1;  /* bit[23]   :  */
        unsigned int  reserved_0              : 1;  /* bit[24]   :  */
        unsigned int  reserved_1              : 1;  /* bit[25]   :  */
        unsigned int  reserved_2              : 1;  /* bit[26]   :  */
        unsigned int  reserved_3              : 5;  /* bit[27-31]: IP软复位使能状态：
                                                                   0：IP处于复位撤离状态；
                                                                   1：IP处于软复位使能状态。 */
    } reg;
} SOC_MEDIA1_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_crg_START           (0)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_crg_END             (0)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_START               (1)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_END                 (1)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_dss_START         (2)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_dss_END           (2)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_vdec_START        (3)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_vdec_END          (3)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_tcu_m1_START            (4)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_tcu_m1_END              (4)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_apb_brg_tcu_START      (5)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_apb_brg_tcu_END        (5)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_vdec_pxpc_START         (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_vdec_pxpc_END           (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_vdec_START              (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_vdec_END                (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_arst_vdec_START             (8)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_arst_vdec_END               (8)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_vdec_START             (9)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_vdec_END               (9)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_venc_START        (10)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_venc_END          (10)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_disp_ch1_crg_START  (11)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_disp_ch1_crg_END    (11)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_disp_ch1_START      (12)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_disp_ch1_END        (12)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media_qic_data_START    (13)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media_qic_data_END      (13)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media_qice_data_START   (14)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media_qice_data_END     (14)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_cs_vdec_START           (15)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_cs_vdec_END             (15)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_cs_venc_START           (16)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_cs_venc_END             (16)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_venc_START              (17)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_venc_END                (17)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_venc_START             (18)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_venc_END               (18)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_arst_venc_START             (19)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_arst_venc_END               (19)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_hpm_START               (20)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_hpm_END                 (20)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dbg_cs_START            (21)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dbg_cs_END              (21)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_qic_dss_START           (22)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_qic_dss_END             (22)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media1_wrap_crg_START   (23)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media1_wrap_crg_END     (23)

/*****************************************************************************
 struct               : SOC_MEDIA1_CRG_CLKDIV0_UNION
 struct description   : CLKDIV0 Register structure definition
                        Address Offset:0x060 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sel_dss_pll        : 4;  /* bit[0-3]  : clk_dss的PLL时钟源选择：
                                                              4'b0001:PPLL2_b_MEDIA；
                                                              4'b0010:PPLL0_MEDIA；
                                                              4'b0100:PPLL1_MEDIA；
                                                              4'b1000:ULPPLL_MEDIA； */
        unsigned int  sel_vdec_pll       : 4;  /* bit[4-7]  : clk_vdec的PLL时钟源选择：
                                                              4'b0001:PPLL2_b_MEDIA；
                                                              4'b0010:PPLL0_MEDIA；
                                                              4'b0100:PPLL1_MEDIA；
                                                              4'b1000:PPLL2_MEDIA； */
        unsigned int  sel_vcodec_axi_pll : 4;  /* bit[8-11] : vcodec_axi的PLL时钟源选择：
                                                              4'b0001:PPLL2_b_MEDIA；
                                                              4'b0010:PPLL0_MEDIA；
                                                              4'b0100:PPLL1_MEDIA；
                                                              4'b1000:PPLL2_MEDIA； */
        unsigned int  reserved           : 4;  /* bit[12-15]:  */
        unsigned int  bitmasken          : 16; /* bit[16-31]: bitmasken位，为clkdiv每个比特位的使能位：
                                                              只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA1_CRG_CLKDIV0_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV0_sel_dss_pll_START         (0)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_dss_pll_END           (3)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_vdec_pll_START        (4)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_vdec_pll_END          (7)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_vcodec_axi_pll_START  (8)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_vcodec_axi_pll_END    (11)
#define SOC_MEDIA1_CRG_CLKDIV0_bitmasken_START           (16)
#define SOC_MEDIA1_CRG_CLKDIV0_bitmasken_END             (31)

/*****************************************************************************
 struct               : SOC_MEDIA1_CRG_CLKDIV2_UNION
 struct description   : CLKDIV2 Register structure definition
                        Address Offset:0x068 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_dss   : 6;  /* bit[0-5]  : clk_dss时钟分频比选择。
                                                     6'h0:1；
                                                     6'h1:2；
                                                     …
                                                     6'h3F:64； */
        unsigned int  reserved_0: 2;  /* bit[6-7]  :  */
        unsigned int  div_vdec  : 6;  /* bit[8-13] : clk_vdec时钟分频比选择。
                                                     6'h0:1；
                                                     6'h1:2；
                                                     …
                                                     6'h3F:64； */
        unsigned int  reserved_1: 2;  /* bit[14-15]:  */
        unsigned int  bitmasken : 16; /* bit[16-31]: bitmasken位，为clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA1_CRG_CLKDIV2_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV2_div_dss_START    (0)
#define SOC_MEDIA1_CRG_CLKDIV2_div_dss_END      (5)
#define SOC_MEDIA1_CRG_CLKDIV2_div_vdec_START   (8)
#define SOC_MEDIA1_CRG_CLKDIV2_div_vdec_END     (13)
#define SOC_MEDIA1_CRG_CLKDIV2_bitmasken_START  (16)
#define SOC_MEDIA1_CRG_CLKDIV2_bitmasken_END    (31)

/*****************************************************************************
 struct               : SOC_MEDIA1_CRG_DSS_DVFS_UNION
 struct description   : DSS_DVFS Register structure definition
                        Address Offset:0x100 Initial:0x0000 Width:32
 register description : dss dvfs使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hw_dvfs_en : 1;  /* bit[0]    : hw_dvfs使能信号 100 07新增 */
        unsigned int  reserved_0 : 15; /* bit[1-15] :  */
        unsigned int  reserved_1 : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_MEDIA1_CRG_DSS_DVFS_UNION;
#endif
#define SOC_MEDIA1_CRG_DSS_DVFS_hw_dvfs_en_START  (0)
#define SOC_MEDIA1_CRG_DSS_DVFS_hw_dvfs_en_END    (0)

/*****************************************************************************
 struct               : SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_UNION
 struct description   : DSS_DVFS_AUTODIV Register structure definition
                        Address Offset:0x104 Initial:0x000000 Width:32
 register description : dss dvfs状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hw_sel_dss_pll : 4;  /* bit[0-3]  : pll选择回读 */
        unsigned int  hw_div_dss     : 6;  /* bit[4-9]  : div回读 */
        unsigned int  reserved       : 22; /* bit[10-31]:  */
    } reg;
} SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_UNION;
#endif
#define SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_hw_sel_dss_pll_START  (0)
#define SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_hw_sel_dss_pll_END    (3)
#define SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_hw_div_dss_START      (4)
#define SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_hw_div_dss_END        (9)


/*****************************************************************************
 struct               : SOC_CRGPERIPH_PERRSTEN0_UNION
 struct description   : PERRSTEN0 Register structure definition
                        Address Offset:0x060 Initial:0x00000000 Width:32
 register description : 外设软复位使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_dpctrl_pixel  : 1;  /* bit[0] : IP软复位使能：
                                                             0：IP软复位使能状态不变；
                                                             1：IP软复位使能。 */
        unsigned int  ip_rst_time_stamp    : 1;  /* bit[1] :  */
        unsigned int  reserved_0           : 1;  /* bit[2] :  */
        unsigned int  ip_rst_pasensor_peri : 1;  /* bit[3] :  */
        unsigned int  ip_rst_resource_lock : 1;  /* bit[4] :  */
        unsigned int  reserved_1           : 1;  /* bit[5] :  */
        unsigned int  reserved_2           : 1;  /* bit[6] :  */
        unsigned int  ip_rst_qic_qcr_mdm   : 1;  /* bit[7] :  */
        unsigned int  reserved_3           : 1;  /* bit[8] :  */
        unsigned int  reserved_4           : 1;  /* bit[9] :  */
        unsigned int  reserved_5           : 1;  /* bit[10]:  */
        unsigned int  reserved_6           : 1;  /* bit[11]:  */
        unsigned int  reserved_7           : 1;  /* bit[12]:  */
        unsigned int  reserved_8           : 1;  /* bit[13]:  */
        unsigned int  ip_rst_i2c7          : 1;  /* bit[14]:  */
        unsigned int  reserved_9           : 1;  /* bit[15]:  */
        unsigned int  reserved_10          : 1;  /* bit[16]:  */
        unsigned int  reserved_11          : 1;  /* bit[17]:  */
        unsigned int  ip_rst_perf_stat     : 1;  /* bit[18]:  */
        unsigned int  ip_rst_i2c6          : 1;  /* bit[19]:  */
        unsigned int  reserved_12          : 1;  /* bit[20]:  */
        unsigned int  reserved_13          : 1;  /* bit[21]:  */
        unsigned int  reserved_14          : 1;  /* bit[22]:  */
        unsigned int  ip_rst_noc_mdm_cfg   : 1;  /* bit[23]:  */
        unsigned int  ip_rst_power_stat    : 1;  /* bit[24]:  */
        unsigned int  reserved_15          : 1;  /* bit[25]:  */
        unsigned int  reserved_16          : 1;  /* bit[26]:  */
        unsigned int  reserved_17          : 1;  /* bit[27]:  */
        unsigned int  ip_rst_ufs_dfa_cpu   : 1;  /* bit[28]:  */
        unsigned int  ip_rst_ufs_dfa_sys   : 1;  /* bit[29]:  */
        unsigned int  reserved_18          : 1;  /* bit[30]:  */
        unsigned int  ip_rst_bist_sync     : 1;  /* bit[31]: IP软复位使能：
                                                             0：IP软复位使能状态不变；
                                                             1：IP软复位使能。 */
    } reg;
} SOC_CRGPERIPH_PERRSTEN0_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_dpctrl_pixel_START   (0)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_dpctrl_pixel_END     (0)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_time_stamp_START     (1)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_time_stamp_END       (1)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_pasensor_peri_START  (3)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_pasensor_peri_END    (3)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_resource_lock_START  (4)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_resource_lock_END    (4)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_qic_qcr_mdm_START    (7)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_qic_qcr_mdm_END      (7)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c7_START           (14)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c7_END             (14)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_perf_stat_START      (18)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_perf_stat_END        (18)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c6_START           (19)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_i2c6_END             (19)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_mdm_cfg_START    (23)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_noc_mdm_cfg_END      (23)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_power_stat_START     (24)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_power_stat_END       (24)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_ufs_dfa_cpu_START    (28)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_ufs_dfa_cpu_END      (28)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_ufs_dfa_sys_START    (29)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_ufs_dfa_sys_END      (29)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_bist_sync_START      (31)
#define SOC_CRGPERIPH_PERRSTEN0_ip_rst_bist_sync_END        (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PERRSTDIS0_UNION
 struct description   : PERRSTDIS0 Register structure definition
                        Address Offset:0x064 Initial:0x00000000 Width:32
 register description : 外设软复位撤离寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_dpctrl_pixel  : 1;  /* bit[0] : IP软复位撤离：
                                                             0：IP软复位使能状态不变；
                                                             1：IP软复位撤离。 */
        unsigned int  ip_rst_time_stamp    : 1;  /* bit[1] :  */
        unsigned int  reserved_0           : 1;  /* bit[2] :  */
        unsigned int  ip_rst_pasensor_peri : 1;  /* bit[3] :  */
        unsigned int  ip_rst_resource_lock : 1;  /* bit[4] :  */
        unsigned int  reserved_1           : 1;  /* bit[5] :  */
        unsigned int  reserved_2           : 1;  /* bit[6] :  */
        unsigned int  ip_rst_qic_qcr_mdm   : 1;  /* bit[7] :  */
        unsigned int  reserved_3           : 1;  /* bit[8] :  */
        unsigned int  reserved_4           : 1;  /* bit[9] :  */
        unsigned int  reserved_5           : 1;  /* bit[10]:  */
        unsigned int  reserved_6           : 1;  /* bit[11]:  */
        unsigned int  reserved_7           : 1;  /* bit[12]:  */
        unsigned int  reserved_8           : 1;  /* bit[13]:  */
        unsigned int  ip_rst_i2c7          : 1;  /* bit[14]:  */
        unsigned int  reserved_9           : 1;  /* bit[15]:  */
        unsigned int  reserved_10          : 1;  /* bit[16]:  */
        unsigned int  reserved_11          : 1;  /* bit[17]:  */
        unsigned int  ip_rst_perf_stat     : 1;  /* bit[18]:  */
        unsigned int  ip_rst_i2c6          : 1;  /* bit[19]:  */
        unsigned int  reserved_12          : 1;  /* bit[20]:  */
        unsigned int  reserved_13          : 1;  /* bit[21]:  */
        unsigned int  reserved_14          : 1;  /* bit[22]:  */
        unsigned int  ip_rst_noc_mdm_cfg   : 1;  /* bit[23]:  */
        unsigned int  ip_rst_power_stat    : 1;  /* bit[24]:  */
        unsigned int  reserved_15          : 1;  /* bit[25]:  */
        unsigned int  reserved_16          : 1;  /* bit[26]:  */
        unsigned int  reserved_17          : 1;  /* bit[27]:  */
        unsigned int  ip_rst_ufs_dfa_cpu   : 1;  /* bit[28]:  */
        unsigned int  ip_rst_ufs_dfa_sys   : 1;  /* bit[29]:  */
        unsigned int  reserved_18          : 1;  /* bit[30]:  */
        unsigned int  ip_rst_bist_sync     : 1;  /* bit[31]: IP软复位撤离：
                                                             0：IP软复位使能状态不变；
                                                             1：IP软复位撤离。 */
    } reg;
} SOC_CRGPERIPH_PERRSTDIS0_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_dpctrl_pixel_START   (0)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_dpctrl_pixel_END     (0)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_time_stamp_START     (1)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_time_stamp_END       (1)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_pasensor_peri_START  (3)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_pasensor_peri_END    (3)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_resource_lock_START  (4)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_resource_lock_END    (4)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_qic_qcr_mdm_START    (7)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_qic_qcr_mdm_END      (7)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c7_START           (14)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c7_END             (14)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_perf_stat_START      (18)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_perf_stat_END        (18)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c6_START           (19)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_i2c6_END             (19)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_mdm_cfg_START    (23)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_noc_mdm_cfg_END      (23)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_power_stat_START     (24)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_power_stat_END       (24)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_ufs_dfa_cpu_START    (28)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_ufs_dfa_cpu_END      (28)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_ufs_dfa_sys_START    (29)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_ufs_dfa_sys_END      (29)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_bist_sync_START      (31)
#define SOC_CRGPERIPH_PERRSTDIS0_ip_rst_bist_sync_END        (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PERRSTSTAT0_UNION
 struct description   : PERRSTSTAT0 Register structure definition
                        Address Offset:0x068 Initial:0xFF8710A9 Width:32
 register description : 外设软复位状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_dpctrl_pixel  : 1;  /* bit[0] : 外设软复位使能状态：
                                                             0：IP软复位使能撤销；
                                                             1：IP软复位使能； */
        unsigned int  ip_rst_time_stamp    : 1;  /* bit[1] :  */
        unsigned int  reserved_0           : 1;  /* bit[2] :  */
        unsigned int  ip_rst_pasensor_peri : 1;  /* bit[3] :  */
        unsigned int  ip_rst_resource_lock : 1;  /* bit[4] :  */
        unsigned int  reserved_1           : 1;  /* bit[5] :  */
        unsigned int  reserved_2           : 1;  /* bit[6] :  */
        unsigned int  ip_rst_qic_qcr_mdm   : 1;  /* bit[7] :  */
        unsigned int  reserved_3           : 1;  /* bit[8] : 这个复位其实是ip_rst_qice_gm_gs，不与mdm绑定 */
        unsigned int  reserved_4           : 1;  /* bit[9] :  */
        unsigned int  reserved_5           : 1;  /* bit[10]:  */
        unsigned int  reserved_6           : 1;  /* bit[11]:  */
        unsigned int  reserved_7           : 1;  /* bit[12]:  */
        unsigned int  reserved_8           : 1;  /* bit[13]:  */
        unsigned int  ip_rst_i2c7          : 1;  /* bit[14]:  */
        unsigned int  reserved_9           : 1;  /* bit[15]:  */
        unsigned int  reserved_10          : 1;  /* bit[16]:  */
        unsigned int  reserved_11          : 1;  /* bit[17]:  */
        unsigned int  ip_rst_perf_stat     : 1;  /* bit[18]:  */
        unsigned int  ip_rst_i2c6          : 1;  /* bit[19]:  */
        unsigned int  reserved_12          : 1;  /* bit[20]:  */
        unsigned int  reserved_13          : 1;  /* bit[21]:  */
        unsigned int  reserved_14          : 1;  /* bit[22]:  */
        unsigned int  ip_rst_noc_mdm_cfg   : 1;  /* bit[23]:  */
        unsigned int  ip_rst_power_stat    : 1;  /* bit[24]:  */
        unsigned int  reserved_15          : 1;  /* bit[25]:  */
        unsigned int  reserved_16          : 1;  /* bit[26]:  */
        unsigned int  reserved_17          : 1;  /* bit[27]:  */
        unsigned int  ip_rst_ufs_dfa_cpu   : 1;  /* bit[28]:  */
        unsigned int  ip_rst_ufs_dfa_sys   : 1;  /* bit[29]:  */
        unsigned int  reserved_18          : 1;  /* bit[30]:  */
        unsigned int  ip_rst_bist_sync     : 1;  /* bit[31]: IP软复位使能状态：
                                                             0：IP处于复位撤离状态；
                                                             1：IP处于软复位使能状态。 */
    } reg;
} SOC_CRGPERIPH_PERRSTSTAT0_UNION;
#endif
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_dpctrl_pixel_START   (0)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_dpctrl_pixel_END     (0)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_time_stamp_START     (1)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_time_stamp_END       (1)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_pasensor_peri_START  (3)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_pasensor_peri_END    (3)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_resource_lock_START  (4)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_resource_lock_END    (4)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_qic_qcr_mdm_START    (7)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_qic_qcr_mdm_END      (7)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c7_START           (14)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c7_END             (14)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_perf_stat_START      (18)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_perf_stat_END        (18)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c6_START           (19)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_i2c6_END             (19)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_mdm_cfg_START    (23)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_noc_mdm_cfg_END      (23)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_power_stat_START     (24)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_power_stat_END       (24)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_ufs_dfa_cpu_START    (28)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_ufs_dfa_cpu_END      (28)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_ufs_dfa_sys_START    (29)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_ufs_dfa_sys_END      (29)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_bist_sync_START      (31)
#define SOC_CRGPERIPH_PERRSTSTAT0_ip_rst_bist_sync_END        (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PEREN12_UNION
 struct description   : PEREN12 Register structure definition
                        Address Offset:0x470 Initial:0x00000000 Width:32
 register description : 外设时钟使能寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_i3c4             : 1;  /* bit[0]    : 外设时钟使能控制：
                                                                   0：写0无效果；
                                                                   1：使能IP时钟。 */
        unsigned int  reserved_0              : 1;  /* bit[1]    :  */
        unsigned int  reserved_1              : 1;  /* bit[2]    :  */
        unsigned int  gt_pclk_atgs_sys        : 1;  /* bit[3]    :  */
        unsigned int  reserved_2              : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_mdm_drx2dbg      : 1;  /* bit[5]    :  */
        unsigned int  reserved_3              : 1;  /* bit[6]    :  */
        unsigned int  reserved_4              : 1;  /* bit[7]    :  */
        unsigned int  gt_clk_intr_hub         : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_cpu_pctrl        : 1;  /* bit[9]    :  */
        unsigned int  gt_clk_npu_pctrl        : 1;  /* bit[10]   :  */
        unsigned int  gt_clk_media2_pctrl     : 1;  /* bit[11]   :  */
        unsigned int  gt_clk_media1_pctrl     : 1;  /* bit[12]   :  */
        unsigned int  gt_clk_hsdt0_pcmp_xctrl : 1;  /* bit[13]   :  */
        unsigned int  gt_clk_hsdt1_pcmp_xctrl : 1;  /* bit[14]   :  */
        unsigned int  gt_clk_spe_ref          : 1;  /* bit[15]   :  */
        unsigned int  gt_hclk_spe             : 1;  /* bit[16]   :  */
        unsigned int  gt_clk_spe              : 1;  /* bit[17]   :  */
        unsigned int  reserved_5              : 1;  /* bit[18]   :  */
        unsigned int  reserved_6              : 1;  /* bit[19]   :  */
        unsigned int  gt_aclk_maa             : 1;  /* bit[20]   :  */
        unsigned int  gt_clk_dpctrl_16m       : 1;  /* bit[21]   :  */
        unsigned int  gt_clk_dpctrl_pixel     : 1;  /* bit[22]   :  */
        unsigned int  reserved_7              : 1;  /* bit[23]   :  */
        unsigned int  reserved_8              : 8;  /* bit[24-31]: 外设时钟使能控制：
                                                                   0：写0无效果；
                                                                   1：使能IP时钟。 */
    } reg;
} SOC_CRGPERIPH_PEREN12_UNION;
#endif
#define SOC_CRGPERIPH_PEREN12_gt_clk_i3c4_START              (0)
#define SOC_CRGPERIPH_PEREN12_gt_clk_i3c4_END                (0)
#define SOC_CRGPERIPH_PEREN12_gt_pclk_atgs_sys_START         (3)
#define SOC_CRGPERIPH_PEREN12_gt_pclk_atgs_sys_END           (3)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_drx2dbg_START       (5)
#define SOC_CRGPERIPH_PEREN12_gt_clk_mdm_drx2dbg_END         (5)
#define SOC_CRGPERIPH_PEREN12_gt_clk_intr_hub_START          (8)
#define SOC_CRGPERIPH_PEREN12_gt_clk_intr_hub_END            (8)
#define SOC_CRGPERIPH_PEREN12_gt_clk_cpu_pctrl_START         (9)
#define SOC_CRGPERIPH_PEREN12_gt_clk_cpu_pctrl_END           (9)
#define SOC_CRGPERIPH_PEREN12_gt_clk_npu_pctrl_START         (10)
#define SOC_CRGPERIPH_PEREN12_gt_clk_npu_pctrl_END           (10)
#define SOC_CRGPERIPH_PEREN12_gt_clk_media2_pctrl_START      (11)
#define SOC_CRGPERIPH_PEREN12_gt_clk_media2_pctrl_END        (11)
#define SOC_CRGPERIPH_PEREN12_gt_clk_media1_pctrl_START      (12)
#define SOC_CRGPERIPH_PEREN12_gt_clk_media1_pctrl_END        (12)
#define SOC_CRGPERIPH_PEREN12_gt_clk_hsdt0_pcmp_xctrl_START  (13)
#define SOC_CRGPERIPH_PEREN12_gt_clk_hsdt0_pcmp_xctrl_END    (13)
#define SOC_CRGPERIPH_PEREN12_gt_clk_hsdt1_pcmp_xctrl_START  (14)
#define SOC_CRGPERIPH_PEREN12_gt_clk_hsdt1_pcmp_xctrl_END    (14)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_ref_START           (15)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_ref_END             (15)
#define SOC_CRGPERIPH_PEREN12_gt_hclk_spe_START              (16)
#define SOC_CRGPERIPH_PEREN12_gt_hclk_spe_END                (16)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_START               (17)
#define SOC_CRGPERIPH_PEREN12_gt_clk_spe_END                 (17)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_maa_START              (20)
#define SOC_CRGPERIPH_PEREN12_gt_aclk_maa_END                (20)
#define SOC_CRGPERIPH_PEREN12_gt_clk_dpctrl_16m_START        (21)
#define SOC_CRGPERIPH_PEREN12_gt_clk_dpctrl_16m_END          (21)
#define SOC_CRGPERIPH_PEREN12_gt_clk_dpctrl_pixel_START      (22)
#define SOC_CRGPERIPH_PEREN12_gt_clk_dpctrl_pixel_END        (22)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PERDIS12_UNION
 struct description   : PERDIS12 Register structure definition
                        Address Offset:0x474 Initial:0x00000000 Width:32
 register description : 外设时钟禁止寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_i3c4             : 1;  /* bit[0]    : 外设时钟禁止控制：
                                                                   0：写0无效果；
                                                                   1：禁止IP时钟。 */
        unsigned int  reserved_0              : 1;  /* bit[1]    :  */
        unsigned int  reserved_1              : 1;  /* bit[2]    :  */
        unsigned int  gt_pclk_atgs_sys        : 1;  /* bit[3]    :  */
        unsigned int  reserved_2              : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_mdm_drx2dbg      : 1;  /* bit[5]    :  */
        unsigned int  reserved_3              : 1;  /* bit[6]    :  */
        unsigned int  reserved_4              : 1;  /* bit[7]    :  */
        unsigned int  gt_clk_intr_hub         : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_cpu_pctrl        : 1;  /* bit[9]    :  */
        unsigned int  gt_clk_npu_pctrl        : 1;  /* bit[10]   :  */
        unsigned int  gt_clk_media2_pctrl     : 1;  /* bit[11]   :  */
        unsigned int  gt_clk_media1_pctrl     : 1;  /* bit[12]   :  */
        unsigned int  gt_clk_hsdt0_pcmp_xctrl : 1;  /* bit[13]   :  */
        unsigned int  gt_clk_hsdt1_pcmp_xctrl : 1;  /* bit[14]   :  */
        unsigned int  gt_clk_spe_ref          : 1;  /* bit[15]   :  */
        unsigned int  gt_hclk_spe             : 1;  /* bit[16]   :  */
        unsigned int  gt_clk_spe              : 1;  /* bit[17]   :  */
        unsigned int  reserved_5              : 1;  /* bit[18]   :  */
        unsigned int  reserved_6              : 1;  /* bit[19]   :  */
        unsigned int  gt_aclk_maa             : 1;  /* bit[20]   :  */
        unsigned int  gt_clk_dpctrl_16m       : 1;  /* bit[21]   :  */
        unsigned int  gt_clk_dpctrl_pixel     : 1;  /* bit[22]   :  */
        unsigned int  reserved_7              : 1;  /* bit[23]   :  */
        unsigned int  reserved_8              : 8;  /* bit[24-31]: 外设时钟禁止控制：
                                                                   0：写0无效果；
                                                                   1：禁止IP时钟。 */
    } reg;
} SOC_CRGPERIPH_PERDIS12_UNION;
#endif
#define SOC_CRGPERIPH_PERDIS12_gt_clk_i3c4_START              (0)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_i3c4_END                (0)
#define SOC_CRGPERIPH_PERDIS12_gt_pclk_atgs_sys_START         (3)
#define SOC_CRGPERIPH_PERDIS12_gt_pclk_atgs_sys_END           (3)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_drx2dbg_START       (5)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_mdm_drx2dbg_END         (5)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_intr_hub_START          (8)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_intr_hub_END            (8)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_cpu_pctrl_START         (9)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_cpu_pctrl_END           (9)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_npu_pctrl_START         (10)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_npu_pctrl_END           (10)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_media2_pctrl_START      (11)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_media2_pctrl_END        (11)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_media1_pctrl_START      (12)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_media1_pctrl_END        (12)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_hsdt0_pcmp_xctrl_START  (13)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_hsdt0_pcmp_xctrl_END    (13)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_hsdt1_pcmp_xctrl_START  (14)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_hsdt1_pcmp_xctrl_END    (14)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_ref_START           (15)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_ref_END             (15)
#define SOC_CRGPERIPH_PERDIS12_gt_hclk_spe_START              (16)
#define SOC_CRGPERIPH_PERDIS12_gt_hclk_spe_END                (16)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_START               (17)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_spe_END                 (17)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_maa_START              (20)
#define SOC_CRGPERIPH_PERDIS12_gt_aclk_maa_END                (20)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_dpctrl_16m_START        (21)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_dpctrl_16m_END          (21)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_dpctrl_pixel_START      (22)
#define SOC_CRGPERIPH_PERDIS12_gt_clk_dpctrl_pixel_END        (22)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_HW_DSS_DVFS_STAT_UNION
 struct description   : HW_DSS_DVFS_STAT Register structure definition
                        Address Offset:0x668 Initial:0x00000000 Width:32
 register description : DSS DVFS状态机维测状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hw_dss_dvfs_pll_gt0   : 1;  /* bit[0]    :  */
        unsigned int  hw_dss_dvfs_pll_gt1   : 1;  /* bit[1]    :  */
        unsigned int  hw_dss_dvfs_pll_gt2   : 1;  /* bit[2]    :  */
        unsigned int  hw_dss_dvfs_pll_gt3   : 1;  /* bit[3]    :  */
        unsigned int  hw_dss_dvfs_pll_en0   : 1;  /* bit[4]    :  */
        unsigned int  reserved_0            : 1;  /* bit[5]    :  */
        unsigned int  hw_dss_dvfs_pll_en2   : 1;  /* bit[6]    :  */
        unsigned int  hw_dss_dvfs_pll_en3   : 1;  /* bit[7]    :  */
        unsigned int  hw_dss_dvfs_pll_lock0 : 1;  /* bit[8]    :  */
        unsigned int  reserved_1            : 1;  /* bit[9]    :  */
        unsigned int  hw_dss_dvfs_pll_lock2 : 1;  /* bit[10]   :  */
        unsigned int  hw_dss_dvfs_pll_lock3 : 1;  /* bit[11]   :  */
        unsigned int  reserved_2            : 1;  /* bit[12]   :  */
        unsigned int  reserved_3            : 1;  /* bit[13]   :  */
        unsigned int  reserved_4            : 1;  /* bit[14]   :  */
        unsigned int  reserved_5            : 1;  /* bit[15]   :  */
        unsigned int  reserved_6            : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                 只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CRGPERIPH_HW_DSS_DVFS_STAT_UNION;
#endif
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt0_START    (0)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt0_END      (0)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt1_START    (1)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt1_END      (1)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt2_START    (2)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt2_END      (2)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt3_START    (3)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_gt3_END      (3)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_en0_START    (4)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_en0_END      (4)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_en2_START    (6)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_en2_END      (6)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_en3_START    (7)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_en3_END      (7)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_lock0_START  (8)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_lock0_END    (8)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_lock2_START  (10)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_lock2_END    (10)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_lock3_START  (11)
#define SOC_CRGPERIPH_HW_DSS_DVFS_STAT_hw_dss_dvfs_pll_lock3_END    (11)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_UNION
 struct description   : PLL_FSM_NS_VOTE0 Register structure definition
                        Address Offset:0x950 Initial:0x00000000 Width:32
 register description : PLL状态机非安全投票寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ppll1_en_fsm_vote : 5;  /* bit[0-4]  : 投票位仅在pll_fsm使能之后有效 */
        unsigned int  ppll2_en_fsm_vote : 5;  /* bit[5-9]  :  */
        unsigned int  ppll3_en_fsm_vote : 5;  /* bit[10-14]:  */
        unsigned int  reserved          : 1;  /* bit[15]   :  */
        unsigned int  bitmasken         : 16; /* bit[16-31]: pll_en_vote_ctrl0每个比特位的使能位：
                                                             只有当bitmasken对应的比特位为1'b1，pll_en_vote_ctrl0相应的比特位才起作用。bitmasken[0]就是peri_fnpll_ctrl[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ppll1_en_fsm_vote_START  (0)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ppll1_en_fsm_vote_END    (4)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ppll2_en_fsm_vote_START  (5)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ppll2_en_fsm_vote_END    (9)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ppll3_en_fsm_vote_START  (10)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ppll3_en_fsm_vote_END    (14)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_bitmasken_START          (16)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_bitmasken_END            (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_UNION
 struct description   : PLL_FSM_NS_VOTE1 Register structure definition
                        Address Offset:0x954 Initial:0x00000000 Width:32
 register description : PLL状态机非安全投票寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ppll7_en_fsm_vote   : 5;  /* bit[0-4]  : 投票位仅在pll_fsm使能之后有效 */
        unsigned int  ppll2_b_en_fsm_vote : 5;  /* bit[5-9]  :  */
        unsigned int  ppll6_en_fsm_vote   : 5;  /* bit[10-14]:  */
        unsigned int  reserved            : 1;  /* bit[15]   :  */
        unsigned int  bitmasken           : 16; /* bit[16-31]: pll_en_vote_ctrl0每个比特位的使能位：
                                                               只有当bitmasken对应的比特位为1'b1，pll_en_vote_ctrl0相应的比特位才起作用。bitmasken[0]就是peri_fnpll_ctrl[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ppll7_en_fsm_vote_START    (0)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ppll7_en_fsm_vote_END      (4)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ppll2_b_en_fsm_vote_START  (5)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ppll2_b_en_fsm_vote_END    (9)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ppll6_en_fsm_vote_START    (10)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ppll6_en_fsm_vote_END      (14)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_bitmasken_START            (16)
#define SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_bitmasken_END              (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PPLL2_BCTRL0_UNION
 struct description   : PPLL2_BCTRL0 Register structure definition
                        Address Offset:0xA84 Initial:0x00905306 Width:32
 register description : PPLL2_B控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ppll2_b_en       : 1;  /* bit[0-0]  : PPLL2_B PowerDown控制。
                                                            0：关闭；
                                                            1：使能。 */
        unsigned int  ppll2_b_bp       : 1;  /* bit[1-1]  : PPLL2_B Bypass控制。
                                                            0：正常工作；
                                                            1：Bypass。 */
        unsigned int  ppll2_b_refdiv   : 6;  /* bit[2-7]  : PPLL2_B输入时钟分频器。 */
        unsigned int  ppll2_b_fbdiv    : 12; /* bit[8-19] : PPLL2_B反馈时钟整数分频器。 */
        unsigned int  ppll2_b_postdiv1 : 3;  /* bit[20-22]: PPLL2_B输出时钟分频器1。 */
        unsigned int  ppll2_b_postdiv2 : 3;  /* bit[23-25]: PPLL2_B输出时钟分频器2。 */
        unsigned int  ppll2_b_lock     : 1;  /* bit[26-26]: PPLL2_B Lock指示。
                                                            0：无效；
                                                            1：PPLL2_B Lock。 */
        unsigned int  reserved         : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_CRGPERIPH_PPLL2_BCTRL0_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_en_START        (0)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_en_END          (0)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_bp_START        (1)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_bp_END          (1)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_refdiv_START    (2)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_refdiv_END      (7)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_fbdiv_START     (8)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_fbdiv_END       (19)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_postdiv1_START  (20)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_postdiv1_END    (22)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_postdiv2_START  (23)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_postdiv2_END    (25)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_lock_START      (26)
#define SOC_CRGPERIPH_PPLL2_BCTRL0_ppll2_b_lock_END        (26)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PPLL2_BCTRL1_UNION
 struct description   : PPLL2_BCTRL1 Register structure definition
                        Address Offset:0xA88 Initial:0x06555555 Width:32
 register description : PPLL2_B控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ppll2_b_fracdiv : 24; /* bit[0-23] : PPLL2_B反馈时钟分数分频器。 */
        unsigned int  ppll2_b_int_mod : 1;  /* bit[24-24]: PPLL2_B整数分频模式。
                                                           0：分数分频模式；
                                                           1：整数分频模式。
                                                           注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  ppll2_b_cfg_vld : 1;  /* bit[25-25]: PPLL2_B配置有效标志。
                                                           0：配置无效；
                                                           1：配置有效。 */
        unsigned int  gt_clk_ppll2_b  : 1;  /* bit[26-26]: PPLL2_B门控信号。
                                                           0：PPLL2_B输出时钟门控；
                                                           1：PPLL2_B输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved        : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_CRGPERIPH_PPLL2_BCTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ppll2_b_fracdiv_START  (0)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ppll2_b_fracdiv_END    (23)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ppll2_b_int_mod_START  (24)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ppll2_b_int_mod_END    (24)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ppll2_b_cfg_vld_START  (25)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_ppll2_b_cfg_vld_END    (25)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_gt_clk_ppll2_b_START   (26)
#define SOC_CRGPERIPH_PPLL2_BCTRL1_gt_clk_ppll2_b_END     (26)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_FSM_CTRL1_UNION
 struct description   : PLL_FSM_CTRL1 Register structure definition
                        Address Offset:0xC44 Initial:0x001E053F Width:32
 register description : PLL_FSM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pll_lock_dbc_en : 1;  /* bit[0]    : PLL投票状态机是否会读第二次lock作为锁定判断依据：
                                                           0：关闭
                                                           1：使能 */
        unsigned int  pll_fsm_intr_en : 1;  /* bit[1]    : PLL投票状态机中断控制使能：
                                                           0：关闭
                                                           1：使能 */
        unsigned int  pll_wait_en     : 1;  /* bit[2]    : PLL投票状态机EN使能至LOCK判断等待使能：
                                                           0：关闭
                                                           1：使能 */
        unsigned int  pll_timeout_en  : 1;  /* bit[3]    : PLL投票状态超时判断使能：
                                                           0：关闭
                                                           1：使能 */
        unsigned int  pll_intr_lmt    : 3;  /* bit[4-6]  : PLL状态机触发中断的异常累计次数（不可配置为0）：
                                                           3'd1：1次
                                                           ...
                                                           3'd7：7次 */
        unsigned int  pll_wait_lmt    : 8;  /* bit[7-14] : PLL投票状态机EN使能至LOCK判断等待的cycle数目，默认按照10us左右计算（@1.2MHz） */
        unsigned int  pll_timeout_lmt : 8;  /* bit[15-22]: PLL投票状态机EN使能至LOCK判断等待的cycle数目，默认按照50us左右计算（@1.2MHz） */
        unsigned int  reserved        : 9;  /* bit[23-31]:  */
    } reg;
} SOC_CRGPERIPH_PLL_FSM_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_lock_dbc_en_START  (0)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_lock_dbc_en_END    (0)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_fsm_intr_en_START  (1)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_fsm_intr_en_END    (1)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_wait_en_START      (2)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_wait_en_END        (2)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_timeout_en_START   (3)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_timeout_en_END     (3)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_intr_lmt_START     (4)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_intr_lmt_END       (6)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_wait_lmt_START     (7)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_wait_lmt_END       (14)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_timeout_lmt_START  (15)
#define SOC_CRGPERIPH_PLL_FSM_CTRL1_pll_timeout_lmt_END    (22)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_FSM_STAT0_UNION
 struct description   : PLL_FSM_STAT0 Register structure definition
                        Address Offset:0xC48 Initial:0x00000000 Width:32
 register description : PLL_FSM状态回读寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pll1_intr_cnt    : 3;  /* bit[0-2]  : pll_fsm触发中断的异常累计次数状态，用于debug */
        unsigned int  pll2_intr_cnt    : 3;  /* bit[3-5]  :  */
        unsigned int  pll2_b_intr_cnt  : 3;  /* bit[6-8]  :  */
        unsigned int  pll3_intr_cnt    : 3;  /* bit[9-11] :  */
        unsigned int  pll7_intr_cnt    : 3;  /* bit[12-14]:  */
        unsigned int  pll6_intr_cnt    : 3;  /* bit[15-17]:  */
        unsigned int  reserved         : 5;  /* bit[18-22]:  */
        unsigned int  pll6_fsm_intr    : 1;  /* bit[23]   :  */
        unsigned int  pll1_fsm_intr    : 1;  /* bit[24]   : pll_fsm中断指示信号，高有效 */
        unsigned int  pll2_fsm_intr    : 1;  /* bit[25]   :  */
        unsigned int  ppll2_b_fsm_intr : 1;  /* bit[26]   :  */
        unsigned int  pll3_fsm_intr    : 1;  /* bit[27]   :  */
        unsigned int  pll7_fsm_intr    : 1;  /* bit[28]   :  */
        unsigned int  aupll_fsm_intr   : 1;  /* bit[29]   :  */
        unsigned int  pll5_fsm_intr    : 1;  /* bit[30]   :  */
        unsigned int  intr_pll_fsm     : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_CRGPERIPH_PLL_FSM_STAT0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll1_intr_cnt_START     (0)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll1_intr_cnt_END       (2)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll2_intr_cnt_START     (3)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll2_intr_cnt_END       (5)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll2_b_intr_cnt_START   (6)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll2_b_intr_cnt_END     (8)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll3_intr_cnt_START     (9)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll3_intr_cnt_END       (11)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll7_intr_cnt_START     (12)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll7_intr_cnt_END       (14)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll6_intr_cnt_START     (15)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll6_intr_cnt_END       (17)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll6_fsm_intr_START     (23)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll6_fsm_intr_END       (23)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll1_fsm_intr_START     (24)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll1_fsm_intr_END       (24)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll2_fsm_intr_START     (25)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll2_fsm_intr_END       (25)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_ppll2_b_fsm_intr_START  (26)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_ppll2_b_fsm_intr_END    (26)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll3_fsm_intr_START     (27)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll3_fsm_intr_END       (27)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll7_fsm_intr_START     (28)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll7_fsm_intr_END       (28)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_aupll_fsm_intr_START    (29)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_aupll_fsm_intr_END      (29)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll5_fsm_intr_START     (30)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_pll5_fsm_intr_END       (30)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_intr_pll_fsm_START      (31)
#define SOC_CRGPERIPH_PLL_FSM_STAT0_intr_pll_fsm_END        (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_FSM_STAT1_UNION
 struct description   : PLL_FSM_STAT1 Register structure definition
                        Address Offset:0xC4C Initial:0x00555555 Width:32
 register description : PLL_FSM状态回读寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pll1_curr_stat   : 4;  /* bit[0-3]  : pll_fsm状态指示信号，用于debug */
        unsigned int  pll2_curr_stat   : 4;  /* bit[4-7]  :  */
        unsigned int  pll2_b_curr_stat : 4;  /* bit[8-11] :  */
        unsigned int  pll3_curr_stat   : 4;  /* bit[12-15]:  */
        unsigned int  pll7_curr_stat   : 4;  /* bit[16-19]:  */
        unsigned int  pll6_curr_stat   : 4;  /* bit[20-23]:  */
        unsigned int  reserved         : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_CRGPERIPH_PLL_FSM_STAT1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll1_curr_stat_START    (0)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll1_curr_stat_END      (3)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll2_curr_stat_START    (4)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll2_curr_stat_END      (7)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll2_b_curr_stat_START  (8)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll2_b_curr_stat_END    (11)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll3_curr_stat_START    (12)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll3_curr_stat_END      (15)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll7_curr_stat_START    (16)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll7_curr_stat_END      (19)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll6_curr_stat_START    (20)
#define SOC_CRGPERIPH_PLL_FSM_STAT1_pll6_curr_stat_END      (23)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_VOTE_STAT0_UNION
 struct description   : PLL_VOTE_STAT0 Register structure definition
                        Address Offset:0xC50 Initial:0x0003FFFF Width:32
 register description : PLL投票状态回读寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 1;  /* bit[0]    :  */
        unsigned int  ppll1_gt_stat   : 1;  /* bit[1]    :  */
        unsigned int  ppll2_gt_stat   : 1;  /* bit[2]    :  */
        unsigned int  ppll2_b_gt_stat : 1;  /* bit[3]    :  */
        unsigned int  ppll3_gt_stat   : 1;  /* bit[4]    :  */
        unsigned int  reserved_1      : 1;  /* bit[5]    :  */
        unsigned int  reserved_2      : 1;  /* bit[6]    :  */
        unsigned int  ppll6_gt_stat   : 1;  /* bit[7]    :  */
        unsigned int  ppll7_gt_stat   : 1;  /* bit[8]    :  */
        unsigned int  reserved_3      : 1;  /* bit[9]    :  */
        unsigned int  ppll1_bypass    : 1;  /* bit[10]   :  */
        unsigned int  ppll2_bypass    : 1;  /* bit[11]   :  */
        unsigned int  ppll2_b_bypass  : 1;  /* bit[12]   :  */
        unsigned int  ppll3_bypass    : 1;  /* bit[13]   :  */
        unsigned int  reserved_4      : 1;  /* bit[14]   :  */
        unsigned int  reserved_5      : 1;  /* bit[15]   :  */
        unsigned int  ppll6_bypass    : 1;  /* bit[16]   :  */
        unsigned int  ppll7_bypass    : 1;  /* bit[17]   :  */
        unsigned int  reserved_6      : 1;  /* bit[18]   :  */
        unsigned int  ppll1_en_stat   : 1;  /* bit[19]   :  */
        unsigned int  ppll2_en_stat   : 1;  /* bit[20]   :  */
        unsigned int  ppll2_b_en_stat : 1;  /* bit[21]   :  */
        unsigned int  ppll3_en_stat   : 1;  /* bit[22]   :  */
        unsigned int  reserved_7      : 1;  /* bit[23]   :  */
        unsigned int  reserved_8      : 1;  /* bit[24]   :  */
        unsigned int  ppll6_en_stat   : 1;  /* bit[25]   :  */
        unsigned int  ppll7_en_stat   : 1;  /* bit[26]   :  */
        unsigned int  reserved_9      : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_CRGPERIPH_PLL_VOTE_STAT0_UNION;
#endif
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_gt_stat_START    (1)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_gt_stat_END      (1)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_gt_stat_START    (2)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_gt_stat_END      (2)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_gt_stat_START  (3)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_gt_stat_END    (3)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_gt_stat_START    (4)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_gt_stat_END      (4)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll6_gt_stat_START    (7)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll6_gt_stat_END      (7)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_gt_stat_START    (8)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_gt_stat_END      (8)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_bypass_START     (10)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_bypass_END       (10)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_bypass_START     (11)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_bypass_END       (11)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_bypass_START   (12)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_bypass_END     (12)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_bypass_START     (13)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_bypass_END       (13)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll6_bypass_START     (16)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll6_bypass_END       (16)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_bypass_START     (17)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_bypass_END       (17)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_en_stat_START    (19)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll1_en_stat_END      (19)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_en_stat_START    (20)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_en_stat_END      (20)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_en_stat_START  (21)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll2_b_en_stat_END    (21)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_en_stat_START    (22)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll3_en_stat_END      (22)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll6_en_stat_START    (25)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll6_en_stat_END      (25)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_en_stat_START    (26)
#define SOC_CRGPERIPH_PLL_VOTE_STAT0_ppll7_en_stat_END      (26)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_UNION
 struct description   : PLL_EN_VOTE_CTRL1 Register structure definition
                        Address Offset:0xC60 Initial:0x00000000 Width:32
 register description : PLL使能投票寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0    : 5;  /* bit[0-4]  :  */
        unsigned int  ppll6_en_vote : 5;  /* bit[5-9]  :  */
        unsigned int  ppll7_en_vote : 5;  /* bit[10-14]:  */
        unsigned int  reserved_1    : 1;  /* bit[15]   :  */
        unsigned int  bitmasken     : 16; /* bit[16-31]: pll_en_vote_ctrl1每个比特位的使能位：
                                                         只有当bitmasken对应的比特位为1'b1，pll_en_vote_ctrl1相应的比特位才起作用。bitmasken[0]就是peri_fnpll_ctrl[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_UNION;
#endif
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ppll6_en_vote_START  (5)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ppll6_en_vote_END    (9)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ppll7_en_vote_START  (10)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ppll7_en_vote_END    (14)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_bitmasken_START      (16)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_bitmasken_END        (31)

/*****************************************************************************
 struct               : SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_UNION
 struct description   : PLL_EN_VOTE_CTRL2 Register structure definition
                        Address Offset:0xC6C Initial:0x00000000 Width:32
 register description : PLL使能投票寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 5;  /* bit[0-4]  :  */
        unsigned int  reserved_1      : 5;  /* bit[5-9]  :  */
        unsigned int  ppll2_b_en_vote : 5;  /* bit[10-14]:  */
        unsigned int  reserved_2      : 1;  /* bit[15]   :  */
        unsigned int  bitmasken       : 16; /* bit[16-31]: pll_en_vote_ctrl1每个比特位的使能位：
                                                           只有当bitmasken对应的比特位为1'b1，pll_en_vote_ctrl1相应的比特位才起作用。bitmasken[0]就是peri_fnpll_ctrl[0]的mask使能位。写1有效。 */
    } reg;
} SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_UNION;
#endif
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ppll2_b_en_vote_START  (10)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ppll2_b_en_vote_END    (14)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_bitmasken_START        (16)
#define SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_bitmasken_END          (31)

#endif