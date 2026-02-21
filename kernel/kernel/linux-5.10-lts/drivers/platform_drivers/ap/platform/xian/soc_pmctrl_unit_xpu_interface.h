

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_PMCTRL_UNIT_XPU_INTERFACE_H__
#define __SOC_PMCTRL_UNIT_XPU_INTERFACE_H__

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


/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_PDVFS_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_PDVFS_INSTR_LIST_ADDR(base, ma)   ((base) + (ma) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_PWR_UP_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_PWR_UP_INSTR_LIST_ADDR(base, mb)  ((base) + 0x080UL + (mb) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_PWR_DN_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_PWR_DN_INSTR_LIST_ADDR(base, mb)  ((base) + 0x0C0UL + (mb) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SC_ENTER_DOZE_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_SC_ENTER_DOZE_INSTR_LIST_ADDR(base, mc) ((base) + 0x100UL + (mc) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SC_EXIT_DOZE_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_SC_EXIT_DOZE_INSTR_LIST_ADDR(base, mc) ((base) + 0x140UL + (mc) * 4)

/* Register description: 优先级配置寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_UNION */
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_ADDR(base)       ((base) + 0x180UL)

/* Register description: MASTER状态寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_MASTER_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_ADDR(base)            ((base) + 0x184UL)

/* Register description: 控制寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_ADDR(base)          ((base) + 0x18CUL)

/* Register description: XPU_LP_CTRL XPU_PMC自动门控bypass寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_UNION */
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_ADDR(base) ((base) + 0x200UL)

/* Register description: INSTR控制寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_ADDR(base)    ((base) + 0x204UL)

/* Register description: TASKCTRL寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_ADDR(base)             ((base) + 0x208UL)

/* Register description: TASKCTRL寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_ADDR(base)             ((base) + 0x20CUL)

/* Register description: TASKCTRL寄存器2
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_2_ADDR(base)             ((base) + 0x210UL)

/* Register description: TASKCTRL寄存器3
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_3_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_3_ADDR(base)             ((base) + 0x214UL)

/* Register description: 调压/频硬件自动等待使能寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_UNION */
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_ADDR(base)   ((base) + 0x218UL)

/* Register description: APLL_0控制寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_ADDR(base)           ((base) + 0x220UL)

/* Register description: APLL_0控制寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_ADDR(base)           ((base) + 0x224UL)

/* Register description: APLL_0控制状态寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_ADDR(base)      ((base) + 0x228UL)

/* Register description: APLL_0控制状态寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_ADDR(base)      ((base) + 0x22CUL)

/* Register description: APLL_0 DVFS初始化寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_ADDR(base) ((base) + 0x230UL)

/* Register description: APLL_0 DVFS初始化寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_ADDR(base) ((base) + 0x234UL)

/* Register description: APLL_0 PWRUPDN初始化寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_ADDR(base) ((base) + 0x238UL)

/* Register description: APLL_0 PWRUPDN初始化寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_ADDR(base) ((base) + 0x23CUL)

/* Register description: COFSM CLK_DIV控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_ADDR(base)   ((base) + 0x260UL)

/* Register description: COFSM CLK_DIV控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_ADDR(base)   ((base) + 0x264UL)

/* Register description: SVFD/VDM寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_ADDR(base)    ((base) + 0x268UL)

/* Register description: SVFD/VDM控制寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_ADDR(base)        ((base) + 0x26CUL)

/* Register description: 精细小数缓调频bypass寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_UNION */
#define SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_ADDR(base) ((base) + 0x270UL)

/* Register description: COFSM VOL控制来源配
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_ADDR(base)     ((base) + 0x300UL)

/* Register description: COFSM VOL_HS控制来源配
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_ADDR(base)  ((base) + 0x304UL)

/* Register description: COFSM FREQ控制来源配
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_ADDR(base)    ((base) + 0x308UL)

/* Register description: COFSM LOGIC VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_ADDR(base) ((base) + 0x310UL)

/* Register description: COFSM LOGIC VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_ADDR(base) ((base) + 0x314UL)

/* Register description: COFSM LOGIC VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_ADDR(base) ((base) + 0x318UL)

/* Register description: COFSM LOGIC VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_ADDR(base) ((base) + 0x31CUL)

/* Register description: COFSM LOGIC VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_ADDR(base) ((base) + 0x320UL)

/* Register description: COFSM LOGIC VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_ADDR(base) ((base) + 0x324UL)

/* Register description: COFSM LOGIC VOL TRIM状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_ADDR(base) ((base) + 0x3ACUL)

/* Register description: COFSM MEM VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_ADDR(base)   ((base) + 0x330UL)

/* Register description: COFSM MEM VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_ADDR(base)   ((base) + 0x334UL)

/* Register description: COFSM MEM VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_ADDR(base)   ((base) + 0x338UL)

/* Register description: COFSM MEM VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_ADDR(base)   ((base) + 0x33CUL)

/* Register description: COFSM MEM VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_ADDR(base)   ((base) + 0x340UL)

/* Register description: COFSM MEM VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_ADDR(base)   ((base) + 0x344UL)

/* Register description: COFSMA VOL_HS控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_ADDR(base)      ((base) + 0x370UL)

/* Register description: COFSMA VDM状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_ADDR(base)         ((base) + 0x374UL)

/* Register description: COFSM FDCS控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_ADDR(base)      ((base) + 0x240UL)

/* Register description: COFSM FDCS回读
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_ADDR(base)      ((base) + 0x244UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_ADDR(base) ((base) + 0x380UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_ADDR(base) ((base) + 0x384UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_ADDR(base) ((base) + 0x388UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_ADDR(base) ((base) + 0x38CUL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_ADDR(base)   ((base) + 0x390UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_ADDR(base)   ((base) + 0x394UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_ADDR(base)   ((base) + 0x398UL)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_ADDR(base)   ((base) + 0x39CUL)

/* Register description: 通用时间配置
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_ADDR(base)   ((base) + 0x3A4UL)

/* Register description: 通用时间配置
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_ADDR(base)   ((base) + 0x3A8UL)


#else


/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_PDVFS_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_PDVFS_INSTR_LIST_ADDR(base, ma)   ((base) + (ma) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_PWR_UP_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_PWR_UP_INSTR_LIST_ADDR(base, mb)  ((base) + 0x080 + (mb) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_PWR_DN_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_PWR_DN_INSTR_LIST_ADDR(base, mb)  ((base) + 0x0C0 + (mb) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SC_ENTER_DOZE_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_SC_ENTER_DOZE_INSTR_LIST_ADDR(base, mc) ((base) + 0x100 + (mc) * 4)

/* Register description: DVFS指令寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SC_EXIT_DOZE_INSTR_LIST_UNION */
#define SOC_PMCTRL_UNIT_XPU_SC_EXIT_DOZE_INSTR_LIST_ADDR(base, mc) ((base) + 0x140 + (mc) * 4)

/* Register description: 优先级配置寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_UNION */
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_ADDR(base)       ((base) + 0x180)

/* Register description: MASTER状态寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_MASTER_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_ADDR(base)            ((base) + 0x184)

/* Register description: 控制寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_ADDR(base)          ((base) + 0x18C)

/* Register description: XPU_LP_CTRL XPU_PMC自动门控bypass寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_UNION */
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_ADDR(base) ((base) + 0x200)

/* Register description: INSTR控制寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_ADDR(base)    ((base) + 0x204)

/* Register description: TASKCTRL寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_ADDR(base)             ((base) + 0x208)

/* Register description: TASKCTRL寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_ADDR(base)             ((base) + 0x20C)

/* Register description: TASKCTRL寄存器2
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_2_ADDR(base)             ((base) + 0x210)

/* Register description: TASKCTRL寄存器3
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TASKCTRL_3_UNION */
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_3_ADDR(base)             ((base) + 0x214)

/* Register description: 调压/频硬件自动等待使能寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_UNION */
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_ADDR(base)   ((base) + 0x218)

/* Register description: APLL_0控制寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_ADDR(base)           ((base) + 0x220)

/* Register description: APLL_0控制寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_ADDR(base)           ((base) + 0x224)

/* Register description: APLL_0控制状态寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_ADDR(base)      ((base) + 0x228)

/* Register description: APLL_0控制状态寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_ADDR(base)      ((base) + 0x22C)

/* Register description: APLL_0 DVFS初始化寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_ADDR(base) ((base) + 0x230)

/* Register description: APLL_0 DVFS初始化寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_ADDR(base) ((base) + 0x234)

/* Register description: APLL_0 PWRUPDN初始化寄存器0
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_ADDR(base) ((base) + 0x238)

/* Register description: APLL_0 PWRUPDN初始化寄存器1
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_UNION */
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_ADDR(base) ((base) + 0x23C)

/* Register description: COFSM CLK_DIV控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_ADDR(base)   ((base) + 0x260)

/* Register description: COFSM CLK_DIV控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_ADDR(base)   ((base) + 0x264)

/* Register description: SVFD/VDM寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_ADDR(base)    ((base) + 0x268)

/* Register description: SVFD/VDM控制寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_UNION */
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_ADDR(base)        ((base) + 0x26C)

/* Register description: 精细小数缓调频bypass寄存器
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_UNION */
#define SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_ADDR(base) ((base) + 0x270)

/* Register description: COFSM VOL控制来源配
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_ADDR(base)     ((base) + 0x300)

/* Register description: COFSM VOL_HS控制来源配
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_ADDR(base)  ((base) + 0x304)

/* Register description: COFSM FREQ控制来源配
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_ADDR(base)    ((base) + 0x308)

/* Register description: COFSM LOGIC VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_ADDR(base) ((base) + 0x310)

/* Register description: COFSM LOGIC VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_ADDR(base) ((base) + 0x314)

/* Register description: COFSM LOGIC VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_ADDR(base) ((base) + 0x318)

/* Register description: COFSM LOGIC VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_ADDR(base) ((base) + 0x31C)

/* Register description: COFSM LOGIC VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_ADDR(base) ((base) + 0x320)

/* Register description: COFSM LOGIC VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_ADDR(base) ((base) + 0x324)

/* Register description: COFSM LOGIC VOL TRIM状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_ADDR(base) ((base) + 0x3AC)

/* Register description: COFSM MEM VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_ADDR(base)   ((base) + 0x330)

/* Register description: COFSM MEM VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_ADDR(base)   ((base) + 0x334)

/* Register description: COFSM MEM VOL控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_ADDR(base)   ((base) + 0x338)

/* Register description: COFSM MEM VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_ADDR(base)   ((base) + 0x33C)

/* Register description: COFSM MEM VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_ADDR(base)   ((base) + 0x340)

/* Register description: COFSM MEM VOL状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_ADDR(base)   ((base) + 0x344)

/* Register description: COFSMA VOL_HS控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_ADDR(base)      ((base) + 0x370)

/* Register description: COFSMA VDM状态
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_ADDR(base)         ((base) + 0x374)

/* Register description: COFSM FDCS控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_ADDR(base)      ((base) + 0x240)

/* Register description: COFSM FDCS回读
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_ADDR(base)      ((base) + 0x244)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_ADDR(base) ((base) + 0x380)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_ADDR(base) ((base) + 0x384)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_ADDR(base) ((base) + 0x388)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_ADDR(base) ((base) + 0x38C)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_ADDR(base)   ((base) + 0x390)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_ADDR(base)   ((base) + 0x394)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_ADDR(base)   ((base) + 0x398)

/* Register description: COFSM 调压时间控制
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_UNION */
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_ADDR(base)   ((base) + 0x39C)

/* Register description: 通用时间配置
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_UNION */
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_ADDR(base)   ((base) + 0x3A4)

/* Register description: 通用时间配置
   Bit domain definition UNION:  SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_UNION */
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_ADDR(base)   ((base) + 0x3A8)


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
 struct               : SOC_PMCTRL_UNIT_XPU_PDVFS_INSTR_LIST_UNION
 struct description   : PDVFS_INSTR_LIST Register structure definition
                        Address Offset:0x000+(ma)*4 Initial:0x00000000 Width:32
 register description : DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_PDVFS_INSTR_LIST_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_PWR_UP_INSTR_LIST_UNION
 struct description   : PWR_UP_INSTR_LIST Register structure definition
                        Address Offset:0x080+(mb)*4 Initial:0x00000000 Width:32
 register description : DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_PWR_UP_INSTR_LIST_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_PWR_DN_INSTR_LIST_UNION
 struct description   : PWR_DN_INSTR_LIST Register structure definition
                        Address Offset:0x0C0+(mb)*4 Initial:0x00000000 Width:32
 register description : DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_PWR_DN_INSTR_LIST_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_SC_ENTER_DOZE_INSTR_LIST_UNION
 struct description   : SC_ENTER_DOZE_INSTR_LIST Register structure definition
                        Address Offset:0x100+(mc)*4 Initial:0x00000000 Width:32
 register description : DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_SC_ENTER_DOZE_INSTR_LIST_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_SC_EXIT_DOZE_INSTR_LIST_UNION
 struct description   : SC_EXIT_DOZE_INSTR_LIST Register structure definition
                        Address Offset:0x140+(mc)*4 Initial:0x00000000 Width:32
 register description : DVFS指令寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_SC_EXIT_DOZE_INSTR_LIST_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_UNION
 struct description   : MASTER_SOURCE_ID Register structure definition
                        Address Offset:0x180 Initial:0x00000 Width:32
 register description : 优先级配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pwr_updn_id     : 4;  /* bit[0-3]  : 资源优先级ID配置，数值越大优先级越高。不可配置为0，不可以和其他master的数值相同。 */
        unsigned int  sc_doze_dvfs_id : 4;  /* bit[4-7]  : 资源优先级ID配置，数值越大优先级越高。不可配置为0，不可以和其他master的数值相同。 */
        unsigned int  dvfs_sft_id     : 4;  /* bit[8-11] : 资源优先级ID配置，数值越大优先级越高。不可配置为0，不可以和其他master的数值相同。 */
        unsigned int  reserved        : 20; /* bit[12-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_pwr_updn_id_START      (0)
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_pwr_updn_id_END        (3)
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_sc_doze_dvfs_id_START  (4)
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_sc_doze_dvfs_id_END    (7)
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_dvfs_sft_id_START      (8)
#define SOC_PMCTRL_UNIT_XPU_MASTER_SOURCE_ID_dvfs_sft_id_END        (11)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_MASTER_STAT_UNION
 struct description   : MASTER_STAT Register structure definition
                        Address Offset:0x184 Initial:0x000000 Width:32
 register description : MASTER状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pwr_updn_flag_sync : 1;  /* bit[0]   :  */
        unsigned int  pwr_updn_ack       : 1;  /* bit[1]   :  */
        unsigned int  sc_doze_flag_sync  : 1;  /* bit[2]   :  */
        unsigned int  sc_doze_dvfs_ack   : 1;  /* bit[3]   :  */
        unsigned int  resource_lock_id   : 4;  /* bit[4-7] : 当前持锁ID */
        unsigned int  dvfs_skip_flag     : 1;  /* bit[8]   : 在下电状态发生软件DVFS空转结束 */
        unsigned int  reserved           : 23; /* bit[9-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_MASTER_STAT_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_pwr_updn_flag_sync_START  (0)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_pwr_updn_flag_sync_END    (0)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_pwr_updn_ack_START        (1)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_pwr_updn_ack_END          (1)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_sc_doze_flag_sync_START   (2)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_sc_doze_flag_sync_END     (2)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_sc_doze_dvfs_ack_START    (3)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_sc_doze_dvfs_ack_END      (3)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_resource_lock_id_START    (4)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_resource_lock_id_END      (7)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_dvfs_skip_flag_START      (8)
#define SOC_PMCTRL_UNIT_XPU_MASTER_STAT_dvfs_skip_flag_END        (8)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_UNION
 struct description   : TASKCTRL_CTRL Register structure definition
                        Address Offset:0x18C Initial:0x0000000 Width:32
 register description : 控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                  : 1;  /* bit[0]   :  */
        unsigned int  taskctrl_intr_dvfs_en       : 1;  /* bit[1]   :  */
        unsigned int  taskctrl_intr_dvfs_clr      : 1;  /* bit[2]   :  */
        unsigned int  taskctrl_intr_dvfs_raw_stat : 1;  /* bit[3]   :  */
        unsigned int  taskctrl_intr_dvfs_stat     : 1;  /* bit[4]   :  */
        unsigned int  pwr_up_sft_ini_pulse        : 1;  /* bit[5]   : 软件配置当前状态为下电 */
        unsigned int  pwr_dn_sft_ini_pulse        : 1;  /* bit[6]   : 软件配置当前状态为上电 */
        unsigned int  reserved_1                  : 25; /* bit[7-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_en_START        (1)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_en_END          (1)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_clr_START       (2)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_clr_END         (2)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_raw_stat_START  (3)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_raw_stat_END    (3)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_stat_START      (4)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_taskctrl_intr_dvfs_stat_END        (4)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_pwr_up_sft_ini_pulse_START         (5)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_pwr_up_sft_ini_pulse_END           (5)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_pwr_dn_sft_ini_pulse_START         (6)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_CTRL_pwr_dn_sft_ini_pulse_END           (6)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_UNION
 struct description   : XPU_PMC_AUTO_CLKGT_BYPASS Register structure definition
                        Address Offset:0x200 Initial:0x000000 Width:32
 register description : XPU_LP_CTRL XPU_PMC自动门控bypass寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  auto_clkgt_taslctrl_bypass        : 1;  /* bit[0]   :  */
        unsigned int  cofsm_auto_clkgt_vol_hs_bypass    : 1;  /* bit[1]   :  */
        unsigned int  cofsm_auto_clkgt_logic_dvs_bypass : 1;  /* bit[2]   :  */
        unsigned int  cofsm_auto_clkgt_mem_dvs_bypass   : 1;  /* bit[3]   :  */
        unsigned int  cofsm_auto_clkgt_apll_ctrl_bypass : 1;  /* bit[4]   :  */
        unsigned int  auto_clkgt_apll_cfg_bypass        : 1;  /* bit[5]   :  */
        unsigned int  reserved_0                        : 1;  /* bit[6]   :  */
        unsigned int  reserved_1                        : 1;  /* bit[7]   :  */
        unsigned int  reserved_2                        : 24; /* bit[8-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_auto_clkgt_taslctrl_bypass_START         (0)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_auto_clkgt_taslctrl_bypass_END           (0)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_vol_hs_bypass_START     (1)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_vol_hs_bypass_END       (1)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_logic_dvs_bypass_START  (2)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_logic_dvs_bypass_END    (2)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_mem_dvs_bypass_START    (3)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_mem_dvs_bypass_END      (3)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_apll_ctrl_bypass_START  (4)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_cofsm_auto_clkgt_apll_ctrl_bypass_END    (4)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_auto_clkgt_apll_cfg_bypass_START         (5)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_AUTO_CLKGT_BYPASS_auto_clkgt_apll_cfg_bypass_END           (5)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_UNION
 struct description   : TASKCTRL_INSTR_CTRL Register structure definition
                        Address Offset:0x204 Initial:0x00 Width:32
 register description : INSTR控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  taskctrl_enable               : 1;  /* bit[0]    :  */
        unsigned int  taskctrl_instr_clr            : 1;  /* bit[1]    :  */
        unsigned int  taskctrl_instr_lock           : 1;  /* bit[2]    :  */
        unsigned int  reserved_0                    : 1;  /* bit[3]    :  */
        unsigned int  pwr_dn_instr_clr              : 1;  /* bit[4]    :  */
        unsigned int  pwr_dn_instr_lock             : 1;  /* bit[5]    :  */
        unsigned int  pwr_up_instr_clr              : 1;  /* bit[6]    :  */
        unsigned int  pwr_up_instr_lock             : 1;  /* bit[7]    :  */
        unsigned int  instr_update_enable           : 1;  /* bit[8]    : 回读为1，表示可以更新INSTR　LIST */
        unsigned int  instr_update_req              : 1;  /* bit[9]    : 软件请求抢锁 */
        unsigned int  instr_update_revoke           : 1;  /* bit[10]   : 无功能 */
        unsigned int  sc_exit_doze_dvfs_instr_clr   : 1;  /* bit[11]   :  */
        unsigned int  sc_exit_doze_dvfs_instr_lock  : 1;  /* bit[12]   :  */
        unsigned int  sc_enter_doze_dvfs_instr_clr  : 1;  /* bit[13]   :  */
        unsigned int  sc_enter_doze_dvfs_instr_lock : 1;  /* bit[14]   :  */
        unsigned int  reserved_1                    : 1;  /* bit[15]   :  */
        unsigned int  reserved_2                    : 1;  /* bit[16]   :  */
        unsigned int  reserved_3                    : 1;  /* bit[17]   :  */
        unsigned int  reserved_4                    : 1;  /* bit[18]   :  */
        unsigned int  reserved_5                    : 1;  /* bit[19]   :  */
        unsigned int  reserved_6                    : 1;  /* bit[20]   :  */
        unsigned int  reserved_7                    : 1;  /* bit[21]   :  */
        unsigned int  reserved_8                    : 1;  /* bit[22]   :  */
        unsigned int  reserved_9                    : 1;  /* bit[23]   :  */
        unsigned int  reserved_10                   : 1;  /* bit[24]   :  */
        unsigned int  reserved_11                   : 1;  /* bit[25]   :  */
        unsigned int  reserved_12                   : 1;  /* bit[26]   :  */
        unsigned int  reserved_13                   : 5;  /* bit[27-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_taskctrl_enable_START                (0)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_taskctrl_enable_END                  (0)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_taskctrl_instr_clr_START             (1)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_taskctrl_instr_clr_END               (1)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_taskctrl_instr_lock_START            (2)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_taskctrl_instr_lock_END              (2)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_dn_instr_clr_START               (4)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_dn_instr_clr_END                 (4)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_dn_instr_lock_START              (5)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_dn_instr_lock_END                (5)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_up_instr_clr_START               (6)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_up_instr_clr_END                 (6)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_up_instr_lock_START              (7)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_pwr_up_instr_lock_END                (7)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_instr_update_enable_START            (8)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_instr_update_enable_END              (8)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_instr_update_req_START               (9)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_instr_update_req_END                 (9)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_instr_update_revoke_START            (10)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_instr_update_revoke_END              (10)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_exit_doze_dvfs_instr_clr_START    (11)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_exit_doze_dvfs_instr_clr_END      (11)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_exit_doze_dvfs_instr_lock_START   (12)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_exit_doze_dvfs_instr_lock_END     (12)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_enter_doze_dvfs_instr_clr_START   (13)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_enter_doze_dvfs_instr_clr_END     (13)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_enter_doze_dvfs_instr_lock_START  (14)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_INSTR_CTRL_sc_enter_doze_dvfs_instr_lock_END    (14)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_UNION
 struct description   : TASKCTRL_0 Register structure definition
                        Address Offset:0x208 Initial:0x0 Width:32
 register description : TASKCTRL寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  taskctrl_dvfs_sft_en             : 1;  /* bit[0]    :  */
        unsigned int  taskctrl_next_step_cfg           : 1;  /* bit[1]    :  */
        unsigned int  taskctrl_escape_cfg              : 1;  /* bit[2]    :  */
        unsigned int  taskctrl_invalid_instr_flag_clr  : 1;  /* bit[3]    :  */
        unsigned int  taskctrl_invalid_instr_flag      : 1;  /* bit[4]    : 非法指令标志 or 没有END指令 */
        unsigned int  taskctrl_halt_flag               : 1;  /* bit[5]    : 正在执行的指令是否halt */
        unsigned int  reserved_0                       : 2;  /* bit[6-7]  :  */
        unsigned int  taskctrl_cur_decoding_instr_addr : 12; /* bit[8-19] : TASKCTRL正在解码执行的指令地址  */
        unsigned int  taskctrl_cur_fetch_num           : 8;  /* bit[20-27]: TASKCTRL正在检测INSTR是否合法的指令编号 */
        unsigned int  reserved_1                       : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_dvfs_sft_en_START              (0)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_dvfs_sft_en_END                (0)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_next_step_cfg_START            (1)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_next_step_cfg_END              (1)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_escape_cfg_START               (2)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_escape_cfg_END                 (2)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_invalid_instr_flag_clr_START   (3)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_invalid_instr_flag_clr_END     (3)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_invalid_instr_flag_START       (4)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_invalid_instr_flag_END         (4)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_halt_flag_START                (5)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_halt_flag_END                  (5)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_cur_decoding_instr_addr_START  (8)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_cur_decoding_instr_addr_END    (19)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_cur_fetch_num_START            (20)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_0_taskctrl_cur_fetch_num_END              (27)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_UNION
 struct description   : TASKCTRL_1 Register structure definition
                        Address Offset:0x20C Initial:0x0 Width:32
 register description : TASKCTRL寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  taskctrl_cur_task_stat    : 8;  /* bit[0-7]  : TASKCTRL FSM状态 */
        unsigned int  taskctrl_num_of_end_instr : 8;  /* bit[8-15] : TASLCTRL END_INSTR的编号数 */
        unsigned int  reserved_0                : 12; /* bit[16-27]:  */
        unsigned int  reserved_1                : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_taskctrl_cur_task_stat_START     (0)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_taskctrl_cur_task_stat_END       (7)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_taskctrl_num_of_end_instr_START  (8)
#define SOC_PMCTRL_UNIT_XPU_TASKCTRL_1_taskctrl_num_of_end_instr_END    (15)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TASKCTRL_2_UNION
 struct description   : TASKCTRL_2 Register structure definition
                        Address Offset:0x210 Initial:0x0 Width:32
 register description : TASKCTRL寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TASKCTRL_2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TASKCTRL_3_UNION
 struct description   : TASKCTRL_3 Register structure definition
                        Address Offset:0x214 Initial:0x0 Width:32
 register description : TASKCTRL寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 8;  /* bit[8-15] :  */
        unsigned int  reserved_2: 12; /* bit[16-27]:  */
        unsigned int  reserved_3: 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TASKCTRL_3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_UNION
 struct description   : XPU_PMC_POLL_WAIT_EN Register structure definition
                        Address Offset:0x218 Initial:0x0000000 Width:32
 register description : 调压/频硬件自动等待使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  taskctrl_poll_wait_en : 1;  /* bit[0]   : 表示调压/频指令是否等待调压/频完成。1’b1表示等待调压/频完成，1’b0表示不等待调压/频完成。 */
        unsigned int  reserved_0            : 1;  /* bit[1]   :  */
        unsigned int  reserved_1            : 1;  /* bit[2]   :  */
        unsigned int  reserved_2            : 1;  /* bit[3]   :  */
        unsigned int  reserved_3            : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_taskctrl_poll_wait_en_START  (0)
#define SOC_PMCTRL_UNIT_XPU_XPU_PMC_POLL_WAIT_EN_taskctrl_poll_wait_en_END    (0)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_UNION
 struct description   : APLL_0_CTRL0 Register structure definition
                        Address Offset:0x220 Initial:0x0 Width:32
 register description : APLL_0控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_0_en_cfg       : 1;  /* bit[0-0]  : apll_0 PowerDown控制。
                                                               0：关闭；
                                                               1：使能。 */
        unsigned int  apll_0_bp_cfg       : 1;  /* bit[1-1]  : apll_0 Bypass控制。
                                                               0：正常工作；
                                                               1：Bypass。 */
        unsigned int  apll_0_refdiv_cfg   : 6;  /* bit[2-7]  : apll_0输入时钟分频器。 */
        unsigned int  apll_0_fbdiv_cfg    : 12; /* bit[8-19] : apll_0反馈时钟整数分频器。 */
        unsigned int  apll_0_postdiv1_cfg : 3;  /* bit[20-22]: apll_0输出时钟分频器1。 */
        unsigned int  apll_0_postdiv2_cfg : 3;  /* bit[23-25]: apll_0输出时钟分频器2。 */
        unsigned int  apll_0_lock         : 1;  /* bit[26]   : apll_0 Lock指示。
                                                               0：无效；
                                                               1：apll_0 Lock。 */
        unsigned int  fcg_lock            : 1;  /* bit[27]   : FDCS Lock指示。
                                                               0：无效；
                                                               1：fdcs Lock。 */
        unsigned int  fdcs_fast_mode      : 1;  /* bit[28]   : FDCS的快速调频和缓调频选择。
                                                               0：缓调频；
                                                               1：快速调频。 */
        unsigned int  apll_fdcs_ctrl_sel  : 1;  /* bit[29]   : 控制APLL和FDCS选择。
                                                               0：APLL_0；
                                                               1：FDCS 。 */
        unsigned int  reserved            : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_en_cfg_START        (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_en_cfg_END          (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_bp_cfg_START        (1)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_bp_cfg_END          (1)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_refdiv_cfg_START    (2)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_refdiv_cfg_END      (7)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_fbdiv_cfg_START     (8)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_fbdiv_cfg_END       (19)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_postdiv1_cfg_START  (20)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_postdiv1_cfg_END    (22)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_postdiv2_cfg_START  (23)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_postdiv2_cfg_END    (25)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_lock_START          (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_0_lock_END            (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_fcg_lock_START             (27)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_fcg_lock_END               (27)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_fdcs_fast_mode_START       (28)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_fdcs_fast_mode_END         (28)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_fdcs_ctrl_sel_START   (29)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_apll_fdcs_ctrl_sel_END     (29)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_UNION
 struct description   : APLL_0_CTRL1 Register structure definition
                        Address Offset:0x224 Initial:0x0 Width:32
 register description : APLL_0控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_0_fracdiv_cfg       : 24; /* bit[0-23] : apll_0反馈时钟分数分频器。 */
        unsigned int  apll_0_int_mod_cfg       : 1;  /* bit[24-24]: apll_0整数分频模式。
                                                                    0：分数分频模式；
                                                                    1：整数分频模式。
                                                                    注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll_0_cfg_vld_cfg       : 1;  /* bit[25-25]: apll_0配置有效标志。
                                                                    0：配置无效；
                                                                    1：配置有效。 */
        unsigned int  gt_clk_apll_0_cfg        : 1;  /* bit[26-26]: Atlanta不在使用此bit。
                                                                    apll_0门控信号。
                                                                    0：apll_0输出时钟门控；
                                                                    1：apll_0输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved_0               : 1;  /* bit[27]   :  */
        unsigned int  apll_0_bp_dis_req_bypass : 1;  /* bit[28]   : 硬件配置apll bypass 去使能的bypass位 */
        unsigned int  apll_0_bp_en_req_bypass  : 1;  /* bit[29]   : 硬件配置apll bypass 使能的bypass位 */
        unsigned int  reserved_1               : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_fracdiv_cfg_START        (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_fracdiv_cfg_END          (23)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_int_mod_cfg_START        (24)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_int_mod_cfg_END          (24)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_cfg_vld_cfg_START        (25)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_cfg_vld_cfg_END          (25)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_gt_clk_apll_0_cfg_START         (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_gt_clk_apll_0_cfg_END           (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_bp_dis_req_bypass_START  (28)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_bp_dis_req_bypass_END    (28)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_bp_en_req_bypass_START   (29)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_apll_0_bp_en_req_bypass_END     (29)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_UNION
 struct description   : APLL_0_CTRL0_STAT Register structure definition
                        Address Offset:0x228 Initial:0x0 Width:32
 register description : APLL_0控制状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_0_en_stat             : 1;  /* bit[0-0]  : apll_0 PowerDown控制。
                                                                      0：关闭；
                                                                      1：使能。 */
        unsigned int  apll_0_bp_stat             : 1;  /* bit[1-1]  : apll_0 Bypass控制。
                                                                      0：正常工作；
                                                                      1：Bypass。 */
        unsigned int  apll_0_refdiv_stat         : 6;  /* bit[2-7]  : apll_0输入时钟分频器。 */
        unsigned int  apll_0_fbdiv_stat          : 12; /* bit[8-19] : apll_0反馈时钟整数分频器。 */
        unsigned int  apll_0_postdiv1_stat       : 3;  /* bit[20-22]: apll_0输出时钟分频器1。 */
        unsigned int  apll_0_postdiv2_stat       : 3;  /* bit[23-25]: apll_0输出时钟分频器2。 */
        unsigned int  fdcs_fast_mode_stable      : 1;  /* bit[26]   : 上升沿表示FDCS快调结束，电平信号。 */
        unsigned int  reserved                   : 1;  /* bit[27]   :  */
        unsigned int  cofsm_cur_apll_0_freq_stat : 4;  /* bit[28-31]: APLL_CTRL FSM状态 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_en_stat_START              (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_en_stat_END                (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_bp_stat_START              (1)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_bp_stat_END                (1)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_refdiv_stat_START          (2)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_refdiv_stat_END            (7)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_fbdiv_stat_START           (8)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_fbdiv_stat_END             (19)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_postdiv1_stat_START        (20)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_postdiv1_stat_END          (22)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_postdiv2_stat_START        (23)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_apll_0_postdiv2_stat_END          (25)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_fdcs_fast_mode_stable_START       (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_fdcs_fast_mode_stable_END         (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_cofsm_cur_apll_0_freq_stat_START  (28)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL0_STAT_cofsm_cur_apll_0_freq_stat_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_UNION
 struct description   : APLL_0_CTRL1_STAT Register structure definition
                        Address Offset:0x22C Initial:0x00 Width:32
 register description : APLL_0控制状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_0_fracdiv_stat : 24; /* bit[0-23] : apll_0反馈时钟分数分频器。 */
        unsigned int  apll_0_int_mod_stat : 1;  /* bit[24-24]: apll_0整数分频模式。
                                                               0：分数分频模式；
                                                               1：整数分频模式。
                                                               注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  apll_0_cfg_vld_stat : 1;  /* bit[25-25]: apll_0配置有效标志。
                                                               0：配置无效；
                                                               1：配置有效。 */
        unsigned int  gt_clk_apll_0_stat  : 1;  /* bit[26-26]: apll_0门控信号。
                                                               0：apll_0输出时钟门控；
                                                               1：apll_0输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved            : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_apll_0_fracdiv_stat_START  (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_apll_0_fracdiv_stat_END    (23)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_apll_0_int_mod_stat_START  (24)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_apll_0_int_mod_stat_END    (24)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_apll_0_cfg_vld_stat_START  (25)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_apll_0_cfg_vld_stat_END    (25)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_gt_clk_apll_0_stat_START   (26)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_CTRL1_STAT_gt_clk_apll_0_stat_END     (26)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_UNION
 struct description   : APLL_0_SW_DVFS_INITIAL0 Register structure definition
                        Address Offset:0x230 Initial:0x00 Width:32
 register description : APLL_0 DVFS初始化寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                  : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                  : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                  : 6;  /* bit[2-7]  :  */
        unsigned int  apll_0_fbdiv_sw_initial0    : 12; /* bit[8-19] :  */
        unsigned int  apll_0_postdiv1_sw_initial0 : 3;  /* bit[20-22]:  */
        unsigned int  apll_0_postdiv2_sw_initial0 : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                  : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                  : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_apll_0_fbdiv_sw_initial0_START     (8)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_apll_0_fbdiv_sw_initial0_END       (19)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_apll_0_postdiv1_sw_initial0_START  (20)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_apll_0_postdiv1_sw_initial0_END    (22)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_apll_0_postdiv2_sw_initial0_START  (23)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL0_apll_0_postdiv2_sw_initial0_END    (25)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_UNION
 struct description   : APLL_0_SW_DVFS_INITIAL1 Register structure definition
                        Address Offset:0x234 Initial:0x00 Width:32
 register description : APLL_0 DVFS初始化寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_0_fracdiv_sw_initial0 : 24; /* bit[0-23] :  */
        unsigned int  reserved_0                 : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1                 : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2                 : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3                 : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_apll_0_fracdiv_sw_initial0_START  (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_DVFS_INITIAL1_apll_0_fracdiv_sw_initial0_END    (23)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_UNION
 struct description   : APLL_0_SW_PWRUPDN_INITIAL0 Register structure definition
                        Address Offset:0x238 Initial:0x00 Width:32
 register description : APLL_0 PWRUPDN初始化寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                  : 1;  /* bit[0-0]  :  */
        unsigned int  reserved_1                  : 1;  /* bit[1-1]  :  */
        unsigned int  reserved_2                  : 6;  /* bit[2-7]  :  */
        unsigned int  apll_0_fbdiv_sw_initial1    : 12; /* bit[8-19] :  */
        unsigned int  apll_0_postdiv1_sw_initial1 : 3;  /* bit[20-22]:  */
        unsigned int  apll_0_postdiv2_sw_initial1 : 3;  /* bit[23-25]:  */
        unsigned int  reserved_3                  : 1;  /* bit[26-26]:  */
        unsigned int  reserved_4                  : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_apll_0_fbdiv_sw_initial1_START     (8)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_apll_0_fbdiv_sw_initial1_END       (19)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_apll_0_postdiv1_sw_initial1_START  (20)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_apll_0_postdiv1_sw_initial1_END    (22)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_apll_0_postdiv2_sw_initial1_START  (23)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL0_apll_0_postdiv2_sw_initial1_END    (25)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_UNION
 struct description   : APLL_0_SW_PWRUPDN_INITIAL1 Register structure definition
                        Address Offset:0x23C Initial:0x00 Width:32
 register description : APLL_0 PWRUPDN初始化寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_0_fracdiv_sw_initial1 : 24; /* bit[0-23] :  */
        unsigned int  reserved_0                 : 1;  /* bit[24-24]:  */
        unsigned int  reserved_1                 : 1;  /* bit[25-25]:  */
        unsigned int  reserved_2                 : 1;  /* bit[26-26]:  */
        unsigned int  reserved_3                 : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_apll_0_fracdiv_sw_initial1_START  (0)
#define SOC_PMCTRL_UNIT_XPU_APLL_0_SW_PWRUPDN_INITIAL1_apll_0_fracdiv_sw_initial1_END    (23)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_UNION
 struct description   : COFSM_CLK_DIV_CTRL_0 Register structure definition
                        Address Offset:0x260 Initial:0x0 Width:32
 register description : COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_cpu_div_sft_idx         : 16; /* bit[0-15] :  */
        unsigned int  cofsm_cpu_div_sft_cfg         : 1;  /* bit[16]   :  */
        unsigned int  cofsm_sw2ppll0_req            : 1;  /* bit[17]   :  */
        unsigned int  cofsm_sw2apll_req             : 1;  /* bit[18]   :  */
        unsigned int  cofsm_pll_sw_stat             : 1;  /* bit[19]   : 切换请求状态。1'b1: APLL，1'b0: PPLL0 */
        unsigned int  cofsm_ppll0_div_stat          : 4;  /* bit[20-23]:  */
        unsigned int  cofsm_ppll0_div_sft_idx       : 4;  /* bit[24-27]:  */
        unsigned int  cofsm_ppll0_div_sft_cfg       : 1;  /* bit[28]   :  */
        unsigned int  cofsm_gt_clk_ppll0_gt_sft_set : 1;  /* bit[29]   : PPLL0_DIV_GT开时钟 */
        unsigned int  cofsm_gt_clk_ppll0_gt_sft_clr : 1;  /* bit[30]   : PPLL0_DIV_GT关时钟 */
        unsigned int  cofsm_gt_clk_ppll0_gt_stat    : 1;  /* bit[31]   : PPLL0_DIV_GT。1'b1: 开钟，1'b0: 关钟 */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_cpu_div_sft_idx_START          (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_cpu_div_sft_idx_END            (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_cpu_div_sft_cfg_START          (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_cpu_div_sft_cfg_END            (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_sw2ppll0_req_START             (17)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_sw2ppll0_req_END               (17)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_sw2apll_req_START              (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_sw2apll_req_END                (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_pll_sw_stat_START              (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_pll_sw_stat_END                (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_ppll0_div_stat_START           (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_ppll0_div_stat_END             (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_ppll0_div_sft_idx_START        (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_ppll0_div_sft_idx_END          (27)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_ppll0_div_sft_cfg_START        (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_ppll0_div_sft_cfg_END          (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_gt_clk_ppll0_gt_sft_set_START  (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_gt_clk_ppll0_gt_sft_set_END    (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_gt_clk_ppll0_gt_sft_clr_START  (30)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_gt_clk_ppll0_gt_sft_clr_END    (30)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_gt_clk_ppll0_gt_stat_START     (31)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_0_cofsm_gt_clk_ppll0_gt_stat_END       (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_UNION
 struct description   : COFSM_CLK_DIV_CTRL_1 Register structure definition
                        Address Offset:0x264 Initial:0x0 Width:32
 register description : COFSM CLK_DIV控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_cpu_div_stat         : 16; /* bit[0-15] :  */
        unsigned int  cofsm_vdm_div_stat         : 4;  /* bit[16-19]:  */
        unsigned int  cofsm_vdm_div_sft_idx      : 4;  /* bit[20-23]:  */
        unsigned int  cofsm_vdm_div_sft_cfg      : 1;  /* bit[24]   :  */
        unsigned int  cofsm_cur_clk_src_stat     : 3;  /* bit[25-27]: 时钟源切换FSM状态 */
        unsigned int  cofsm_pipeline_mgt_sft_cfg : 1;  /* bit[28]   :  */
        unsigned int  cofsm_pipeline_mgt_en      : 1;  /* bit[29]   :  */
        unsigned int  reserved                   : 1;  /* bit[30]   :  */
        unsigned int  cofsm_pll_sw_ack_sync      : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_cpu_div_stat_START          (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_cpu_div_stat_END            (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_vdm_div_stat_START          (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_vdm_div_stat_END            (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_vdm_div_sft_idx_START       (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_vdm_div_sft_idx_END         (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_vdm_div_sft_cfg_START       (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_vdm_div_sft_cfg_END         (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_cur_clk_src_stat_START      (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_cur_clk_src_stat_END        (27)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_pipeline_mgt_sft_cfg_START  (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_pipeline_mgt_sft_cfg_END    (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_pipeline_mgt_en_START       (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_pipeline_mgt_en_END         (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_pll_sw_ack_sync_START       (31)
#define SOC_PMCTRL_UNIT_XPU_COFSM_CLK_DIV_CTRL_1_cofsm_pll_sw_ack_sync_END         (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_UNION
 struct description   : SVFD_FREQ_DOWM_CTRL Register structure definition
                        Address Offset:0x268 Initial:0x0000000 Width:32
 register description : SVFD/VDM寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_freq_down_en_svfd          : 1;  /* bit[0]   : cofsm的SVFD硬件降频使能 */
        unsigned int  cofsm_svfd_freq_down_ctrl_bypass : 1;  /* bit[1]   : cofsm的SVFD硬件降频bypass,1:bypass使能 */
        unsigned int  reserved_0                       : 1;  /* bit[2]   :  */
        unsigned int  reserved_1                       : 1;  /* bit[3]   :  */
        unsigned int  reserved_2                       : 28; /* bit[4-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_cofsm_freq_down_en_svfd_START           (0)
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_cofsm_freq_down_en_svfd_END             (0)
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_cofsm_svfd_freq_down_ctrl_bypass_START  (1)
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_DOWM_CTRL_cofsm_svfd_freq_down_ctrl_bypass_END    (1)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_UNION
 struct description   : SVFD_FREQ_CTRL0 Register structure definition
                        Address Offset:0x26C Initial:0x00 Width:32
 register description : SVFD/VDM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_svfd_freq_down_step_ctrl : 12; /* bit[0-11] : SVFD硬件降频使能的档位 */
        unsigned int  cofsm_svfd_freq_dn_wait_time   : 12; /* bit[12-23]: SVFD硬件降频等待时间 */
        unsigned int  reserved                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_cofsm_svfd_freq_down_step_ctrl_START  (0)
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_cofsm_svfd_freq_down_step_ctrl_END    (11)
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_cofsm_svfd_freq_dn_wait_time_START    (12)
#define SOC_PMCTRL_UNIT_XPU_SVFD_FREQ_CTRL0_cofsm_svfd_freq_dn_wait_time_END      (23)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_UNION
 struct description   : FINE_STEP_FREQ_CTRL_BYPASS Register structure definition
                        Address Offset:0x270 Initial:0x00000000 Width:32
 register description : 精细小数缓调频bypass寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_fine_step_ctrl_bypass : 1;  /* bit[0]   : 1:bypass使能 */
        unsigned int  reserved                    : 31; /* bit[1-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_cofsm_fine_step_ctrl_bypass_START  (0)
#define SOC_PMCTRL_UNIT_XPU_FINE_STEP_FREQ_CTRL_BYPASS_cofsm_fine_step_ctrl_bypass_END    (0)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_UNION
 struct description   : COFSM_VOL_CTRL_SRC Register structure definition
                        Address Offset:0x300 Initial:0xF Width:32
 register description : COFSM VOL控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_logic_vol_ctrl_src_0 : 4;  /* bit[0-3]  : LOGIC_VOL_0的硬件控制来源：
                                                                      0x0: 来源于TASKCTRLA
                                                                      0x1: 来源于TASKCTRLB
                                                                      0x2: 来源于TASKCTRLC
                                                                      0x3: 来源于TASKCTRLD
                                                                      0x4: 来源于TASKCTRLE
                                                                      0x5: 来源于TASKCTRLF
                                                                      0x6: 来源于TASKCTRLG
                                                                      0x7: 来源于TASKCTRLH
                                                                      Others:不控制 */
        unsigned int  cofsm_logic_vol_ctrl_src_1 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsm_logic_vol_ctrl_src_2 : 4;  /* bit[8-11] :  */
        unsigned int  cofsm_logic_vol_ctrl_src_3 : 4;  /* bit[12-15]:  */
        unsigned int  reserved_0                 : 4;  /* bit[16-19]:  */
        unsigned int  reserved_1                 : 4;  /* bit[20-23]:  */
        unsigned int  reserved_2                 : 4;  /* bit[24-27]:  */
        unsigned int  reserved_3                 : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_0_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_0_END    (3)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_1_START  (4)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_1_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_2_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_2_END    (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_3_START  (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_CTRL_SRC_cofsm_logic_vol_ctrl_src_3_END    (15)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_UNION
 struct description   : COFSM_VOL_HS_CTRL_SRC Register structure definition
                        Address Offset:0x304 Initial:0x0 Width:32
 register description : COFSM VOL_HS控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_vol_hs_ctrl_src    : 4;  /* bit[0-3]  : VOL_HS硬件控制来源：
                                                                    0x0: 来源于TASKCTRLA
                                                                    0x1: 来源于TASKCTRLB
                                                                    0x2: 来源于TASKCTRLC
                                                                    0x3: 来源于TASKCTRLD
                                                                    0x4: 来源于TASKCTRLE
                                                                    0x5: 来源于TASKCTRLF
                                                                    0x6: 来源于TASKCTRLG
                                                                    0x7: 来源于TASKCTRLH
                                                                    Others:不控制 */
        unsigned int  cofsm_mem_vol_ctrl_src_0 : 4;  /* bit[4-7]  :  */
        unsigned int  cofsm_mem_vol_ctrl_src_1 : 4;  /* bit[8-11] :  */
        unsigned int  cofsm_mem_vol_ctrl_src_2 : 4;  /* bit[12-15]:  */
        unsigned int  cofsm_mem_vol_ctrl_src_3 : 4;  /* bit[16-19]:  */
        unsigned int  reserved_0               : 4;  /* bit[20-23]:  */
        unsigned int  reserved_1               : 4;  /* bit[24-27]:  */
        unsigned int  reserved_2               : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_vol_hs_ctrl_src_START     (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_vol_hs_ctrl_src_END       (3)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_0_START  (4)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_0_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_1_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_1_END    (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_2_START  (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_2_END    (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_3_START  (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_SRC_cofsm_mem_vol_ctrl_src_3_END    (19)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_UNION
 struct description   : COFSM_FREQ_CTRL_SRC Register structure definition
                        Address Offset:0x308 Initial:0x0 Width:32
 register description : COFSM FREQ控制来源配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_freq_ctrl_src : 4;  /* bit[0-3]  : FREQ的硬件控制来源：
                                                               0x0: 来源于TASKCTRLA
                                                               0x1: 来源于TASKCTRLB
                                                               0x2: 来源于TASKCTRLC
                                                               0x3: 来源于TASKCTRLD
                                                               0x4: 来源于TASKCTRLE
                                                               0x5: 来源于TASKCTRLF
                                                               0x6: 来源于TASKCTRLG
                                                               0x7: 来源于TASKCTRLH
                                                               Others:不控制 */
        unsigned int  reserved_0          : 4;  /* bit[4-7]  :  */
        unsigned int  reserved_1          : 4;  /* bit[8-11] :  */
        unsigned int  reserved_2          : 4;  /* bit[12-15]:  */
        unsigned int  reserved_3          : 4;  /* bit[16-19]:  */
        unsigned int  reserved_4          : 4;  /* bit[20-23]:  */
        unsigned int  reserved_5          : 4;  /* bit[24-27]:  */
        unsigned int  reserved_6          : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_cofsm_freq_ctrl_src_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FREQ_CTRL_SRC_cofsm_freq_ctrl_src_END    (3)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_UNION
 struct description   : COFSM_LOGIC_VOL_CTRL_0 Register structure definition
                        Address Offset:0x310 Initial:0x00 Width:32
 register description : COFSM LOGIC VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_logic_vau_init_vol_idx              : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_logic_vau_init_vol_req              : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                                : 3;  /* bit[9-11] :  */
        unsigned int  cofsm_sft_logic_buck_vol_idx              : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_sft_logic_buck_on_req               : 1;  /* bit[20]   :  */
        unsigned int  cofsm_sft_logic_buck_off_req              : 1;  /* bit[21]   :  */
        unsigned int  cofsm_sft_logic_buck_onoff_req_en         : 1;  /* bit[22]   :  */
        unsigned int  cofsm_logic_separate_vset_ven_ctrl_bypass : 1;  /* bit[23]   : bypass BUCK ON OFF功能，只发起调压 */
        unsigned int  cofsm_logic_fer_buck_pg_bypass            : 1;  /* bit[24]   : 屏蔽与FER的PG事件的握手 */
        unsigned int  cofsm_logic_fer_buck_vok_bypass           : 1;  /* bit[25]   : 屏蔽与FER的VOK事件的握手 */
        unsigned int  reserved_1                                : 6;  /* bit[26-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_vau_init_vol_idx_START               (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_vau_init_vol_idx_END                 (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_vau_init_vol_req_START               (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_vau_init_vol_req_END                 (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_vol_idx_START               (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_vol_idx_END                 (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_on_req_START                (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_on_req_END                  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_off_req_START               (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_off_req_END                 (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_onoff_req_en_START          (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_sft_logic_buck_onoff_req_en_END            (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_separate_vset_ven_ctrl_bypass_START  (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_separate_vset_ven_ctrl_bypass_END    (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_fer_buck_pg_bypass_START             (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_fer_buck_pg_bypass_END               (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_fer_buck_vok_bypass_START            (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_0_cofsm_logic_fer_buck_vok_bypass_END              (25)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_UNION
 struct description   : COFSM_LOGIC_VOL_CTRL_1 Register structure definition
                        Address Offset:0x314 Initial:0x00 Width:32
 register description : COFSM LOGIC VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_soft_logic_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_soft_logic_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsm_soft_logic_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                     : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                     : 1;  /* bit[11]   :  */
        unsigned int  cofsm_soft_logic_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_soft_logic_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsm_soft_logic_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_2                     : 1;  /* bit[22]   :  */
        unsigned int  reserved_3                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_4                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_1_cofsm_soft_logic_vol_chg_pls_1_END    (21)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_UNION
 struct description   : COFSM_LOGIC_VOL_CTRL_2 Register structure definition
                        Address Offset:0x318 Initial:0x00 Width:32
 register description : COFSM LOGIC VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_soft_logic_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_soft_logic_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsm_soft_logic_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                     : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                     : 1;  /* bit[11]   :  */
        unsigned int  cofsm_soft_logic_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_soft_logic_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsm_soft_logic_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_2                     : 1;  /* bit[22]   :  */
        unsigned int  reserved_3                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_4                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_CTRL_2_cofsm_soft_logic_vol_chg_pls_3_END    (21)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_UNION
 struct description   : COFSM_LOGIC_VOL_STAT_0 Register structure definition
                        Address Offset:0x31C Initial:0x00 Width:32
 register description : COFSM LOGIC VOL状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_logic_vau_nxt_max_vol  : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_logic_vau_cur_max_vol  : 8;  /* bit[8-15] :  */
        unsigned int  cofsm_logic_buck_nxt_vol_idx : 8;  /* bit[16-23]:  */
        unsigned int  cofsm_logic_buck_cur_vol_idx : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_vau_nxt_max_vol_START   (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_vau_nxt_max_vol_END     (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_vau_cur_max_vol_START   (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_vau_cur_max_vol_END     (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_buck_nxt_vol_idx_START  (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_buck_nxt_vol_idx_END    (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_buck_cur_vol_idx_START  (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_0_cofsm_logic_buck_cur_vol_idx_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_UNION
 struct description   : COFSM_LOGIC_VOL_STAT_1 Register structure definition
                        Address Offset:0x320 Initial:0x00 Width:32
 register description : COFSM LOGIC VOL状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_logic_valid_vol_vote_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_logic_valid_vol_vote_idx_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsm_logic_valid_vol_vote_idx_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsm_logic_valid_vol_vote_idx_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_0_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_0_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_1_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_1_END    (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_2_START  (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_2_END    (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_3_START  (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_1_cofsm_logic_valid_vol_vote_idx_3_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_UNION
 struct description   : COFSM_LOGIC_VOL_STAT_2 Register structure definition
                        Address Offset:0x324 Initial:0x0 Width:32
 register description : COFSM LOGIC VOL状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_logic_vol_idx_spmi            : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_pmu_logic_vol_chg_spmi            : 1;  /* bit[8]    :  */
        unsigned int  cofsm_pmu_logic_vol_ack_spmi            : 1;  /* bit[9]    :  */
        unsigned int  cofsm_vau_logic_vol_equal_flag          : 1;  /* bit[10]   : 电压nxt==cur不下发调压 */
        unsigned int  cofsm_vau_logic_vol_equal_flag_clr      : 1;  /* bit[11]   :  */
        unsigned int  cofsm_logic_vau_cur_stat                : 3;  /* bit[12-14]: VAU FSM状态 */
        unsigned int  reserved_0                              : 1;  /* bit[15]   :  */
        unsigned int  cofsm_logic_buck_ctrl_cur_stat          : 3;  /* bit[16-18]: BUCK_CTRL FSM状态 */
        unsigned int  reserved_1                              : 1;  /* bit[19]   :  */
        unsigned int  cofsm_logic_vol_trim_stable             : 1;  /* bit[20]   : 调压完成指示 */
        unsigned int  cofsm_logic_buck_onoff_stable           : 1;  /* bit[21]   : buck on off 完成指示 */
        unsigned int  cofsm_fer_logic_buck_pgvok_req          : 1;  /* bit[22]   :  */
        unsigned int  cofsm_fer_logic_buck_pgvok_ack          : 1;  /* bit[23]   :  */
        unsigned int  cofsm_logic_vol_vote_stable_0           : 1;  /* bit[24]   :  */
        unsigned int  cofsm_logic_vol_vote_stable_1           : 1;  /* bit[25]   :  */
        unsigned int  cofsm_logic_vol_vote_stable_2           : 1;  /* bit[26]   :  */
        unsigned int  cofsm_logic_vol_vote_stable_3           : 1;  /* bit[27]   :  */
        unsigned int  cofsm_logic_vol_spmi_hold_timeout_flag  : 1;  /* bit[28]   : 触发spmi hold timeout */
        unsigned int  cofsm_logic_vol_buck_onoff_timeout_flag : 1;  /* bit[29]   : 触发buck onoff timeout */
        unsigned int  cofsm_logic_vol_fer_err_flag            : 1;  /* bit[30]   : fer交互出错 */
        unsigned int  cofsm_logic_vol_timeout_flag_clr        : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_pmu_logic_vol_idx_spmi_START             (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_pmu_logic_vol_idx_spmi_END               (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_pmu_logic_vol_chg_spmi_START             (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_pmu_logic_vol_chg_spmi_END               (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_pmu_logic_vol_ack_spmi_START             (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_pmu_logic_vol_ack_spmi_END               (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_vau_logic_vol_equal_flag_START           (10)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_vau_logic_vol_equal_flag_END             (10)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_vau_logic_vol_equal_flag_clr_START       (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_vau_logic_vol_equal_flag_clr_END         (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vau_cur_stat_START                 (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vau_cur_stat_END                   (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_buck_ctrl_cur_stat_START           (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_buck_ctrl_cur_stat_END             (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_trim_stable_START              (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_trim_stable_END                (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_buck_onoff_stable_START            (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_buck_onoff_stable_END              (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_fer_logic_buck_pgvok_req_START           (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_fer_logic_buck_pgvok_req_END             (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_fer_logic_buck_pgvok_ack_START           (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_fer_logic_buck_pgvok_ack_END             (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_0_START            (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_0_END              (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_1_START            (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_1_END              (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_2_START            (26)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_2_END              (26)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_3_START            (27)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_vote_stable_3_END              (27)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_spmi_hold_timeout_flag_START   (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_spmi_hold_timeout_flag_END     (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_buck_onoff_timeout_flag_START  (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_buck_onoff_timeout_flag_END    (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_fer_err_flag_START             (30)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_fer_err_flag_END               (30)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_timeout_flag_clr_START         (31)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_STAT_2_cofsm_logic_vol_timeout_flag_clr_END           (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_UNION
 struct description   : COFSM_LOGIC_VOL_TRIM_STAT Register structure definition
                        Address Offset:0x3AC Initial:0x0 Width:32
 register description : COFSM LOGIC VOL TRIM状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_trim_logic_vol_vote_add_sub_idx        : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_trim_logic_vol_vote_add_sub_flag       : 1;  /* bit[8]    :  */
        unsigned int  cofsm_trim_logic_vol_vote_req                : 1;  /* bit[9]    :  */
        unsigned int  cofsm_trim_logic_vol_vote_ack                : 1;  /* bit[10]   :  */
        unsigned int  cofsm_trim_logic_vol_vote_stable             : 1;  /* bit[11]   :  */
        unsigned int  cofsm_valid_trim_logic_vol_vote_add_sub_idx  : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_valid_trim_logic_vol_vote_add_sub_flag : 1;  /* bit[20]   :  */
        unsigned int  reserved_0                                   : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                                   : 1;  /* bit[22]   :  */
        unsigned int  reserved_2                                   : 1;  /* bit[23]   :  */
        unsigned int  reserved_3                                   : 1;  /* bit[24]   :  */
        unsigned int  reserved_4                                   : 1;  /* bit[25]   :  */
        unsigned int  reserved_5                                   : 1;  /* bit[26]   :  */
        unsigned int  reserved_6                                   : 1;  /* bit[27]   :  */
        unsigned int  reserved_7                                   : 1;  /* bit[28]   :  */
        unsigned int  reserved_8                                   : 1;  /* bit[29]   :  */
        unsigned int  reserved_9                                   : 1;  /* bit[30]   :  */
        unsigned int  reserved_10                                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_add_sub_idx_START         (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_add_sub_idx_END           (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_add_sub_flag_START        (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_add_sub_flag_END          (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_req_START                 (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_req_END                   (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_ack_START                 (10)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_ack_END                   (10)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_stable_START              (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_trim_logic_vol_vote_stable_END                (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_valid_trim_logic_vol_vote_add_sub_idx_START   (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_valid_trim_logic_vol_vote_add_sub_idx_END     (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_valid_trim_logic_vol_vote_add_sub_flag_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_LOGIC_VOL_TRIM_STAT_cofsm_valid_trim_logic_vol_vote_add_sub_flag_END    (20)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_UNION
 struct description   : COFSM_MEM_VOL_CTRL_0 Register structure definition
                        Address Offset:0x330 Initial:0x00 Width:32
 register description : COFSM MEM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_mem_vau_init_vol_idx              : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_mem_vau_init_vol_req              : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                              : 3;  /* bit[9-11] :  */
        unsigned int  cofsm_sft_mem_buck_vol_idx              : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_sft_mem_buck_on_req               : 1;  /* bit[20]   :  */
        unsigned int  cofsm_sft_mem_buck_off_req              : 1;  /* bit[21]   :  */
        unsigned int  cofsm_sft_mem_buck_onoff_req_en         : 1;  /* bit[22]   :  */
        unsigned int  cofsm_mem_separate_vset_ven_ctrl_bypass : 1;  /* bit[23]   : bypass BUCK ON OFF功能，只发起调压 */
        unsigned int  cofsm_mem_fer_buck_pg_bypass            : 1;  /* bit[24]   : 屏蔽与FER的PG事件的握手 */
        unsigned int  cofsm_mem_fer_buck_vok_bypass           : 1;  /* bit[25]   : 屏蔽与FER的VOK事件的握手 */
        unsigned int  reserved_1                              : 6;  /* bit[26-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_vau_init_vol_idx_START               (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_vau_init_vol_idx_END                 (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_vau_init_vol_req_START               (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_vau_init_vol_req_END                 (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_vol_idx_START               (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_vol_idx_END                 (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_on_req_START                (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_on_req_END                  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_off_req_START               (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_off_req_END                 (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_onoff_req_en_START          (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_sft_mem_buck_onoff_req_en_END            (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_separate_vset_ven_ctrl_bypass_START  (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_separate_vset_ven_ctrl_bypass_END    (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_fer_buck_pg_bypass_START             (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_fer_buck_pg_bypass_END               (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_fer_buck_vok_bypass_START            (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_0_cofsm_mem_fer_buck_vok_bypass_END              (25)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_UNION
 struct description   : COFSM_MEM_VOL_CTRL_1 Register structure definition
                        Address Offset:0x334 Initial:0x00 Width:32
 register description : COFSM MEM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_soft_mem_vol_chg_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_soft_mem_vol_vote_en_0 : 1;  /* bit[8]    :  */
        unsigned int  cofsm_soft_mem_vol_chg_pls_0 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                   : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                   : 1;  /* bit[11]   :  */
        unsigned int  cofsm_soft_mem_vol_chg_idx_1 : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_soft_mem_vol_vote_en_1 : 1;  /* bit[20]   :  */
        unsigned int  cofsm_soft_mem_vol_chg_pls_1 : 1;  /* bit[21]   :  */
        unsigned int  reserved_2                   : 1;  /* bit[22]   :  */
        unsigned int  reserved_3                   : 1;  /* bit[23]   :  */
        unsigned int  reserved_4                   : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_idx_0_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_idx_0_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_vote_en_0_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_vote_en_0_END    (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_pls_0_START  (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_pls_0_END    (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_idx_1_START  (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_idx_1_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_vote_en_1_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_vote_en_1_END    (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_pls_1_START  (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_1_cofsm_soft_mem_vol_chg_pls_1_END    (21)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_UNION
 struct description   : COFSM_MEM_VOL_CTRL_2 Register structure definition
                        Address Offset:0x338 Initial:0x00 Width:32
 register description : COFSM MEM VOL控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_soft_mem_vol_chg_idx_2 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_soft_mem_vol_vote_en_2 : 1;  /* bit[8]    :  */
        unsigned int  cofsm_soft_mem_vol_chg_pls_2 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                   : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                   : 1;  /* bit[11]   :  */
        unsigned int  cofsm_soft_mem_vol_chg_idx_3 : 8;  /* bit[12-19]:  */
        unsigned int  cofsm_soft_mem_vol_vote_en_3 : 1;  /* bit[20]   :  */
        unsigned int  cofsm_soft_mem_vol_chg_pls_3 : 1;  /* bit[21]   :  */
        unsigned int  reserved_2                   : 1;  /* bit[22]   :  */
        unsigned int  reserved_3                   : 1;  /* bit[23]   :  */
        unsigned int  reserved_4                   : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_idx_2_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_idx_2_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_vote_en_2_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_vote_en_2_END    (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_pls_2_START  (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_pls_2_END    (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_idx_3_START  (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_idx_3_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_vote_en_3_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_vote_en_3_END    (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_pls_3_START  (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_CTRL_2_cofsm_soft_mem_vol_chg_pls_3_END    (21)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_UNION
 struct description   : COFSM_MEM_VOL_STAT_0 Register structure definition
                        Address Offset:0x33C Initial:0x00 Width:32
 register description : COFSM MEM VOL状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_mem_vau_nxt_max_vol  : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_mem_vau_cur_max_vol  : 8;  /* bit[8-15] :  */
        unsigned int  cofsm_mem_buck_nxt_vol_idx : 8;  /* bit[16-23]:  */
        unsigned int  cofsm_mem_buck_cur_vol_idx : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_vau_nxt_max_vol_START   (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_vau_nxt_max_vol_END     (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_vau_cur_max_vol_START   (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_vau_cur_max_vol_END     (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_buck_nxt_vol_idx_START  (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_buck_nxt_vol_idx_END    (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_buck_cur_vol_idx_START  (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_0_cofsm_mem_buck_cur_vol_idx_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_UNION
 struct description   : COFSM_MEM_VOL_STAT_1 Register structure definition
                        Address Offset:0x340 Initial:0x00 Width:32
 register description : COFSM MEM VOL状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_mem_valid_vol_vote_idx_0 : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_mem_valid_vol_vote_idx_1 : 8;  /* bit[8-15] :  */
        unsigned int  cofsm_mem_valid_vol_vote_idx_2 : 8;  /* bit[16-23]:  */
        unsigned int  cofsm_mem_valid_vol_vote_idx_3 : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_0_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_0_END    (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_1_START  (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_1_END    (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_2_START  (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_2_END    (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_3_START  (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_1_cofsm_mem_valid_vol_vote_idx_3_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_UNION
 struct description   : COFSM_MEM_VOL_STAT_2 Register structure definition
                        Address Offset:0x344 Initial:0x0 Width:32
 register description : COFSM MEM VOL状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_mem_vol_idx_spmi            : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_pmu_mem_vol_chg_spmi            : 1;  /* bit[8]    :  */
        unsigned int  cofsm_pmu_mem_vol_ack_spmi            : 1;  /* bit[9]    :  */
        unsigned int  cofsm_vau_mem_vol_equal_flag          : 1;  /* bit[10]   : 电压nxt==cur不下发调压 */
        unsigned int  cofsm_vau_mem_vol_equal_flag_clr      : 1;  /* bit[11]   :  */
        unsigned int  cofsm_mem_vau_cur_stat                : 3;  /* bit[12-14]: VAU FSM状态 */
        unsigned int  reserved_0                            : 1;  /* bit[15]   :  */
        unsigned int  cofsm_mem_buck_ctrl_cur_stat          : 3;  /* bit[16-18]: BUCK_CTRL FSM状态 */
        unsigned int  reserved_1                            : 1;  /* bit[19]   :  */
        unsigned int  cofsm_mem_vol_trim_stable             : 1;  /* bit[20]   : 调压完成指示 */
        unsigned int  cofsm_mem_buck_onoff_stable           : 1;  /* bit[21]   : buck on off 完成指示 */
        unsigned int  cofsm_fer_mem_buck_pgvok_req          : 1;  /* bit[22]   :  */
        unsigned int  cofsm_fer_mem_buck_pgvok_ack          : 1;  /* bit[23]   :  */
        unsigned int  cofsm_mem_vol_vote_stable_0           : 1;  /* bit[24]   :  */
        unsigned int  cofsm_mem_vol_vote_stable_1           : 1;  /* bit[25]   :  */
        unsigned int  cofsm_mem_vol_vote_stable_2           : 1;  /* bit[26]   :  */
        unsigned int  cofsm_mem_vol_vote_stable_3           : 1;  /* bit[27]   :  */
        unsigned int  cofsm_mem_vol_spmi_hold_timeout_flag  : 1;  /* bit[28]   : 触发spmi hold timeout */
        unsigned int  cofsm_mem_vol_buck_onoff_timeout_flag : 1;  /* bit[29]   : 触发buck onoff timeout */
        unsigned int  cofsm_mem_vol_fer_err_flag            : 1;  /* bit[30]   : fer交互出错 */
        unsigned int  cofsm_mem_vol_timeout_flag_clr        : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_pmu_mem_vol_idx_spmi_START             (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_pmu_mem_vol_idx_spmi_END               (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_pmu_mem_vol_chg_spmi_START             (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_pmu_mem_vol_chg_spmi_END               (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_pmu_mem_vol_ack_spmi_START             (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_pmu_mem_vol_ack_spmi_END               (9)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_vau_mem_vol_equal_flag_START           (10)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_vau_mem_vol_equal_flag_END             (10)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_vau_mem_vol_equal_flag_clr_START       (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_vau_mem_vol_equal_flag_clr_END         (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vau_cur_stat_START                 (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vau_cur_stat_END                   (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_buck_ctrl_cur_stat_START           (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_buck_ctrl_cur_stat_END             (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_trim_stable_START              (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_trim_stable_END                (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_buck_onoff_stable_START            (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_buck_onoff_stable_END              (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_fer_mem_buck_pgvok_req_START           (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_fer_mem_buck_pgvok_req_END             (22)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_fer_mem_buck_pgvok_ack_START           (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_fer_mem_buck_pgvok_ack_END             (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_0_START            (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_0_END              (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_1_START            (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_1_END              (25)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_2_START            (26)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_2_END              (26)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_3_START            (27)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_vote_stable_3_END              (27)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_spmi_hold_timeout_flag_START   (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_spmi_hold_timeout_flag_END     (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_buck_onoff_timeout_flag_START  (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_buck_onoff_timeout_flag_END    (29)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_fer_err_flag_START             (30)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_fer_err_flag_END               (30)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_timeout_flag_clr_START         (31)
#define SOC_PMCTRL_UNIT_XPU_COFSM_MEM_VOL_STAT_2_cofsm_mem_vol_timeout_flag_clr_END           (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_UNION
 struct description   : COFSM_VOL_HS_CTRL Register structure definition
                        Address Offset:0x370 Initial:0x0 Width:32
 register description : COFSMA VOL_HS控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_soft_vol_hs_cfg_idx  : 8;  /* bit[0-7]  :  */
        unsigned int  cofsm_soft_vol_hs_cfg_pls  : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                 : 3;  /* bit[9-11] :  */
        unsigned int  cofsm_soft_vol_hs_ack      : 1;  /* bit[12]   :  */
        unsigned int  cofsm_vol_hs_soft_cfg_mode : 1;  /* bit[13]   : 1:软件模式使能 */
        unsigned int  cofsm_dvfs_vol_hs_ack      : 1;  /* bit[14]   :  */
        unsigned int  reserved_1                 : 1;  /* bit[15]   :  */
        unsigned int  cofsm_vol_hs_idx           : 8;  /* bit[16-23]: cofsm的调压信号 */
        unsigned int  cofsm_vol_hs_req           : 1;  /* bit[24]   : cofsm的调压req信号 */
        unsigned int  reserved_2                 : 3;  /* bit[25-27]:  */
        unsigned int  cofsm_cur_hs_stat          : 2;  /* bit[28-29]: VOL_HS状态机状态回读 */
        unsigned int  reserved_3                 : 2;  /* bit[30-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_soft_vol_hs_cfg_idx_START   (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_soft_vol_hs_cfg_idx_END     (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_soft_vol_hs_cfg_pls_START   (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_soft_vol_hs_cfg_pls_END     (8)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_soft_vol_hs_ack_START       (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_soft_vol_hs_ack_END         (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_vol_hs_soft_cfg_mode_START  (13)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_vol_hs_soft_cfg_mode_END    (13)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_dvfs_vol_hs_ack_START       (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_dvfs_vol_hs_ack_END         (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_vol_hs_idx_START            (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_vol_hs_idx_END              (23)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_vol_hs_req_START            (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_vol_hs_req_END              (24)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_cur_hs_stat_START           (28)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VOL_HS_CTRL_cofsm_cur_hs_stat_END             (29)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_UNION
 struct description   : COFSM_VDM_STAT Register structure definition
                        Address Offset:0x374 Initial:0x00 Width:32
 register description : COFSMA VDM状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_vdm_data     : 15; /* bit[0-14] :  */
        unsigned int  reserved_0         : 1;  /* bit[15]   :  */
        unsigned int  cofsm_vdm_data_ack : 1;  /* bit[16]   :  */
        unsigned int  cofsm_vdm_data_req : 1;  /* bit[17]   :  */
        unsigned int  cofsm_vdm_en_ack   : 1;  /* bit[18]   :  */
        unsigned int  cofsm_vdm_en_req   : 1;  /* bit[19]   :  */
        unsigned int  cofsm_vdm_dis_ack  : 1;  /* bit[20]   :  */
        unsigned int  cofsm_vdm_dis_req  : 1;  /* bit[21]   :  */
        unsigned int  reserved_1         : 2;  /* bit[22-23]:  */
        unsigned int  reserved_2         : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_data_START      (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_data_END        (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_data_ack_START  (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_data_ack_END    (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_data_req_START  (17)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_data_req_END    (17)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_en_ack_START    (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_en_ack_END      (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_en_req_START    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_en_req_END      (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_dis_ack_START   (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_dis_ack_END     (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_dis_req_START   (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_VDM_STAT_cofsm_vdm_dis_req_END     (21)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_UNION
 struct description   : COFSM_FDCS_CTRL_0 Register structure definition
                        Address Offset:0x240 Initial:0x0 Width:32
 register description : COFSM FDCS控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pim_drop_sft_cfg         : 1;  /* bit[0]    :  */
        unsigned int  cofsm_pim_drop_sft_en          : 1;  /* bit[1]    :  */
        unsigned int  cofsm_peaklow_sft_cfg          : 1;  /* bit[2]    :  */
        unsigned int  cofsm_peaklow_sft_en           : 1;  /* bit[3]    :  */
        unsigned int  cofsm_hw_onsys_avs_sft_cfg     : 1;  /* bit[4]    :  */
        unsigned int  cofsm_hw_onsys_avs_sft_en      : 1;  /* bit[5]    :  */
        unsigned int  cofsm_pim_drop_thr_sft_cfg     : 1;  /* bit[6]    :  */
        unsigned int  cofsm_pim_drop_thr_sft         : 8;  /* bit[7-14] :  */
        unsigned int  cofsm_ack_sel_sft_cfg          : 1;  /* bit[15]   :  */
        unsigned int  cofsm_ack_sel_sft              : 3;  /* bit[16-18]:  */
        unsigned int  cofsm_fdcs_dcc_thr_vld_sft_cfg : 1;  /* bit[19]   :  */
        unsigned int  cofsm_fdcs_monitor_sft_cfg     : 1;  /* bit[20]   :  */
        unsigned int  cofsm_fdcs_monitor_sft_en      : 1;  /* bit[21]   :  */
        unsigned int  reserved_0                     : 9;  /* bit[22-30]:  */
        unsigned int  reserved_1                     : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_sft_cfg_START          (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_sft_cfg_END            (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_sft_en_START           (1)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_sft_en_END             (1)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_peaklow_sft_cfg_START           (2)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_peaklow_sft_cfg_END             (2)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_peaklow_sft_en_START            (3)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_peaklow_sft_en_END              (3)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_hw_onsys_avs_sft_cfg_START      (4)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_hw_onsys_avs_sft_cfg_END        (4)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_hw_onsys_avs_sft_en_START       (5)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_hw_onsys_avs_sft_en_END         (5)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_thr_sft_cfg_START      (6)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_thr_sft_cfg_END        (6)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_thr_sft_START          (7)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_pim_drop_thr_sft_END            (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_ack_sel_sft_cfg_START           (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_ack_sel_sft_cfg_END             (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_ack_sel_sft_START               (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_ack_sel_sft_END                 (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_fdcs_dcc_thr_vld_sft_cfg_START  (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_fdcs_dcc_thr_vld_sft_cfg_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_fdcs_monitor_sft_cfg_START      (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_fdcs_monitor_sft_cfg_END        (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_fdcs_monitor_sft_en_START       (21)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_CTRL_0_cofsm_fdcs_monitor_sft_en_END         (21)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_UNION
 struct description   : COFSM_FDCS_STAT_0 Register structure definition
                        Address Offset:0x244 Initial:0x0 Width:32
 register description : COFSM FDCS回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pim_drop_en               : 1;  /* bit[0]    :  */
        unsigned int  cofsm_peaklow_en                : 1;  /* bit[1]    :  */
        unsigned int  cofsm_hw_onsys_avs_en           : 1;  /* bit[2]    :  */
        unsigned int  cofsm_fdcs_monitor_en           : 1;  /* bit[3]    :  */
        unsigned int  cofsm_pim_drop_thr              : 8;  /* bit[4-11] :  */
        unsigned int  cofsm_ack_sel                   : 3;  /* bit[12-14]:  */
        unsigned int  cofsm_fdcs_dcc_thr_cfg_vld_flag : 1;  /* bit[15]   :  */
        unsigned int  cofsm_fdcs_cfg_finish           : 1;  /* bit[16]   :  */
        unsigned int  cofsm_fdcs_xpu_cfg_finish       : 1;  /* bit[17]   :  */
        unsigned int  cofsm_pipeline_mgt_finish       : 1;  /* bit[18]   :  */
        unsigned int  cofsm_fdcs_xpu_req              : 1;  /* bit[19]   :  */
        unsigned int  reserved_0                      : 11; /* bit[20-30]:  */
        unsigned int  reserved_1                      : 1;  /* bit[31]   :  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_pim_drop_en_START                (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_pim_drop_en_END                  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_peaklow_en_START                 (1)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_peaklow_en_END                   (1)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_hw_onsys_avs_en_START            (2)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_hw_onsys_avs_en_END              (2)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_monitor_en_START            (3)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_monitor_en_END              (3)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_pim_drop_thr_START               (4)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_pim_drop_thr_END                 (11)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_ack_sel_START                    (12)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_ack_sel_END                      (14)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_dcc_thr_cfg_vld_flag_START  (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_dcc_thr_cfg_vld_flag_END    (15)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_cfg_finish_START            (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_cfg_finish_END              (16)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_xpu_cfg_finish_START        (17)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_xpu_cfg_finish_END          (17)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_pipeline_mgt_finish_START        (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_pipeline_mgt_finish_END          (18)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_xpu_req_START               (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_FDCS_STAT_0_cofsm_fdcs_xpu_req_END                 (19)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_UNION
 struct description   : COFSM_PMU_LOGIC_TIME_0 Register structure definition
                        Address Offset:0x380 Initial:0x055 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_logic_vol_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsm_pmu_logic_vol_up_step_time : 12; /* bit[20-31]: Unit: pclk(调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_cofsm_pmu_logic_vol_up_hold_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_cofsm_pmu_logic_vol_up_hold_time_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_cofsm_pmu_logic_vol_up_step_time_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_0_cofsm_pmu_logic_vol_up_step_time_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_UNION
 struct description   : COFSM_PMU_LOGIC_TIME_1 Register structure definition
                        Address Offset:0x384 Initial:0x055 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_logic_vol_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsm_pmu_logic_vol_dn_step_time : 12; /* bit[20-31]: Unit: pclk调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_cofsm_pmu_logic_vol_dn_hold_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_cofsm_pmu_logic_vol_dn_hold_time_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_cofsm_pmu_logic_vol_dn_step_time_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_1_cofsm_pmu_logic_vol_dn_step_time_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_UNION
 struct description   : COFSM_PMU_LOGIC_TIME_2 Register structure definition
                        Address Offset:0x388 Initial:0x000 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_logic_buck_on_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  reserved                     : 12; /* bit[20-31]: Unit: pclk(调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_cofsm_pmu_logic_buck_on_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_2_cofsm_pmu_logic_buck_on_time_END    (19)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_UNION
 struct description   : COFSM_PMU_LOGIC_TIME_3 Register structure definition
                        Address Offset:0x38C Initial:0x000 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_logic_buck_off_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  reserved                      : 12; /* bit[20-31]: Unit: pclk(调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_cofsm_pmu_logic_buck_off_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_LOGIC_TIME_3_cofsm_pmu_logic_buck_off_time_END    (19)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_UNION
 struct description   : COFSM_PMU_MEM_TIME_0 Register structure definition
                        Address Offset:0x390 Initial:0x055 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_mem_vol_up_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsm_pmu_mem_vol_up_step_time : 12; /* bit[20-31]: Unit: pclk(调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_cofsm_pmu_mem_vol_up_hold_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_cofsm_pmu_mem_vol_up_hold_time_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_cofsm_pmu_mem_vol_up_step_time_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_0_cofsm_pmu_mem_vol_up_step_time_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_UNION
 struct description   : COFSM_PMU_MEM_TIME_1 Register structure definition
                        Address Offset:0x394 Initial:0x055 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_mem_vol_dn_hold_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  cofsm_pmu_mem_vol_dn_step_time : 12; /* bit[20-31]: Unit: pclk调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_cofsm_pmu_mem_vol_dn_hold_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_cofsm_pmu_mem_vol_dn_hold_time_END    (19)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_cofsm_pmu_mem_vol_dn_step_time_START  (20)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_1_cofsm_pmu_mem_vol_dn_step_time_END    (31)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_UNION
 struct description   : COFSM_PMU_MEM_TIME_2 Register structure definition
                        Address Offset:0x398 Initial:0x000 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_mem_buck_on_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  reserved                   : 12; /* bit[20-31]: Unit: pclk(调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_cofsm_pmu_mem_buck_on_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_2_cofsm_pmu_mem_buck_on_time_END    (19)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_UNION
 struct description   : COFSM_PMU_MEM_TIME_3 Register structure definition
                        Address Offset:0x39C Initial:0x000 Width:32
 register description : COFSM 调压时间控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cofsm_pmu_mem_buck_off_time : 20; /* bit[0-19] : Unit: pclk */
        unsigned int  reserved                    : 12; /* bit[20-31]: Unit: pclk(调压时间控制不能配为0，下同) */
    } reg;
} SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_cofsm_pmu_mem_buck_off_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_COFSM_PMU_MEM_TIME_3_cofsm_pmu_mem_buck_off_time_END    (19)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_UNION
 struct description   : TIME_PARAMETER_CFG_0 Register structure definition
                        Address Offset:0x3A4 Initial:0x0 Width:32
 register description : 通用时间配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_cfg_time      : 8;  /* bit[0-7]  : APLL缓调频时间（Unit：us） */
        unsigned int  clk_cfg_wait_cycle : 8;  /* bit[8-15] : 硬件配置时钟分频的等待周期 */
        unsigned int  pclk_cycle_per_us  : 10; /* bit[16-25]: 1us所需的pclk周期 */
        unsigned int  reserved_0         : 2;  /* bit[26-27]:  */
        unsigned int  reserved_1         : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_apll_cfg_time_START       (0)
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_apll_cfg_time_END         (7)
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_clk_cfg_wait_cycle_START  (8)
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_clk_cfg_wait_cycle_END    (15)
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_pclk_cycle_per_us_START   (16)
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_0_pclk_cycle_per_us_END     (25)


/*****************************************************************************
 struct               : SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_UNION
 struct description   : TIME_PARAMETER_CFG_1 Register structure definition
                        Address Offset:0x3A8 Initial:0x0 Width:32
 register description : 通用时间配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  apll_lock_time : 12; /* bit[0-11] : APLL lock时间（Unit：us） */
        unsigned int  reserved_0     : 4;  /* bit[12-15]:  */
        unsigned int  reserved_1     : 10; /* bit[16-25]:  */
        unsigned int  reserved_2     : 2;  /* bit[26-27]:  */
        unsigned int  reserved_3     : 4;  /* bit[28-31]:  */
    } reg;
} SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_UNION;
#endif
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_apll_lock_time_START  (0)
#define SOC_PMCTRL_UNIT_XPU_TIME_PARAMETER_CFG_1_apll_lock_time_END    (11)






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

#endif /* end of soc_pmctrl_unit_xpu_interface.h */
