

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_SCTRL_INTERFACE_H__
#define __SOC_SCTRL_INTERFACE_H__

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


/* Register description: 系统控制器寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCTRL_UNION */
#define SOC_SCTRL_SCCTRL_ADDR(base)                           ((base) + 0xF00UL)

/* Register description: 系统控制器寄存器
   Bit domain definition UNION:  SOC_SCTRL_STD2LS_CTRL_UNION */
#define SOC_SCTRL_STD2LS_CTRL_ADDR(base)                      ((base) + 0xFB0UL)

/* Register description: 系统控制器寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_CTRL_UNION */
#define SOC_SCTRL_DOZE_CTRL_ADDR(base)                        ((base) + 0xFC8UL)

/* Register description: 系统状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSYSSTAT_UNION */
#define SOC_SCTRL_SCSYSSTAT_ADDR(base)                        ((base) + 0xF04UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_0_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_0_ADDR(base)                  ((base) + 0xF14UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_1_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_1_ADDR(base)                  ((base) + 0xF18UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_2_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_2_ADDR(base)                  ((base) + 0xF1CUL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_3_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_3_ADDR(base)                  ((base) + 0xFB4UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_4_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_4_ADDR(base)                  ((base) + 0xF10UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_5_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_5_ADDR(base)                  ((base) + 0xF20UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_6_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_6_ADDR(base)                  ((base) + 0xF24UL)

/* Register description: PMU控制寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SC_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_ADDR(base)                 ((base) + 0xF90UL)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_ADDR(base)       ((base) + 0xE60UL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_ADDR(base)       ((base) + 0xE64UL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_ADDR(base)       ((base) + 0xE68UL)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_ADDR(base)      ((base) + 0xE6CUL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_ADDR(base)      ((base) + 0xE70UL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_ADDR(base)      ((base) + 0xE74UL)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_ADDR(base)         ((base) + 0xE78UL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_ADDR(base)         ((base) + 0xE7CUL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_ADDR(base)         ((base) + 0xE80UL)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_ADDR(base)          ((base) + 0xE84UL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_ADDR(base)          ((base) + 0xE88UL)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_ADDR(base)          ((base) + 0xE8CUL)

/* Register description: PMU Power Sequence 状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_PMU_PS_STAT_0_UNION */
#define SOC_SCTRL_SC_PMU_PS_STAT_0_ADDR(base)                 ((base) + 0xF9CUL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_SC_STAT_0_UNION */
#define SOC_SCTRL_SYS_SC_STAT_0_ADDR(base)                    ((base) + 0xF2CUL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_SC_STAT_1_UNION */
#define SOC_SCTRL_SYS_SC_STAT_1_ADDR(base)                    ((base) + 0xFE4UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_SLOW_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_ADDR(base)               ((base) + 0xE90UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_ADDR(base)          ((base) + 0xE94UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_ADDR(base)         ((base) + 0xE98UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_STANDBY_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_ADDR(base)            ((base) + 0xE9CUL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LS2STD_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_ADDR(base)             ((base) + 0xEA0UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_DOZE1_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_ADDR(base)              ((base) + 0xFC0UL)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_DOZE2_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_ADDR(base)              ((base) + 0xFC4UL)

/* Register description: DOZE1下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_ADDR(base)           ((base) + 0xF94UL)

/* Register description: DOZE1下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_ADDR(base)           ((base) + 0xFD4UL)

/* Register description: DOZE1下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_ADDR(base)           ((base) + 0xFD8UL)

/* Register description: DOZE2下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_ADDR(base)           ((base) + 0xFBCUL)

/* Register description: DOZE2下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_ADDR(base)           ((base) + 0xFDCUL)

/* Register description: DOZE2下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_ADDR(base)           ((base) + 0xFE0UL)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE0_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE0_ADDR(base)                   ((base) + 0x028UL)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE1_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE1_ADDR(base)                   ((base) + 0x02CUL)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE2_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE2_ADDR(base)                   ((base) + 0x9A0UL)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE3_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE3_ADDR(base)                   ((base) + 0x9A4UL)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE0_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE0_ADDR(base)                   ((base) + 0x040UL)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE1_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE1_ADDR(base)                   ((base) + 0x044UL)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE2_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE2_ADDR(base)                   ((base) + 0x9A8UL)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE3_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE3_ADDR(base)                   ((base) + 0x9ACUL)

/* Register description: DOZE控制寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SW_INDICATORS_MASK0_UNION */
#define SOC_SCTRL_SW_INDICATORS_MASK0_ADDR(base)              ((base) + 0xF40UL)

/* Register description: DOZE控制寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SW_INDICATORS_MASK1_UNION */
#define SOC_SCTRL_SW_INDICATORS_MASK1_ADDR(base)              ((base) + 0xFE8UL)

/* Register description: DOZE控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SW_DDR_IDLE_MASK_UNION */
#define SOC_SCTRL_SW_DDR_IDLE_MASK_ADDR(base)                 ((base) + 0xF44UL)

/* Register description: DOZE超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_COUNT_CTRL0_UNION */
#define SOC_SCTRL_DOZE_COUNT_CTRL0_ADDR(base)                 ((base) + 0xF48UL)

/* Register description: DOZE超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_COUNT_CTRL1_UNION */
#define SOC_SCTRL_DOZE_COUNT_CTRL1_ADDR(base)                 ((base) + 0xFCCUL)

/* Register description: DOZE1滤波控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_UNION */
#define SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_ADDR(base)              ((base) + 0xE04UL)

/* Register description: DOZE2滤波控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_UNION */
#define SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_ADDR(base)              ((base) + 0xE08UL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_0_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_0_ADDR(base)                  ((base) + 0xF98UL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_1_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_1_ADDR(base)                  ((base) + 0xFA0UL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_2_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_2_ADDR(base)                  ((base) + 0xFA4UL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_3_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_3_ADDR(base)                  ((base) + 0xFA8UL)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_4_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_4_ADDR(base)                  ((base) + 0xFACUL)

/* Register description: SYSBUS低功耗控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYSBUS_PWRD_CTRL_UNION */
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_ADDR(base)                 ((base) + 0xFB8UL)

/* Register description: 时钟切换控制
   Bit domain definition UNION:  SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_UNION */
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_ADDR(base)              ((base) + 0xF50UL)

/* Register description: TCXO控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_TCXO_CTRL_UNION */
#define SOC_SCTRL_SC_TCXO_CTRL_ADDR(base)                     ((base) + 0xF08UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_STAT_UNION */
#define SOC_SCTRL_SC_PERI_POWER_STAT_ADDR(base)               ((base) + 0xF58UL)

/* Register description: PERI_ISO BP寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERIISOBYPASS_UNION */
#define SOC_SCTRL_SCPERIISOBYPASS_ADDR(base)                  ((base) + 0xF5CUL)

/* Register description: DOZE2状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE2_FLAG_STAT_UNION */
#define SOC_SCTRL_DOZE2_FLAG_STAT_ADDR(base)                  ((base) + 0xE0CUL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_CTRL_0_UNION */
#define SOC_SCTRL_SC_PERI_POWER_CTRL_0_ADDR(base)             ((base) + 0xF70UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_CTRL_1_UNION */
#define SOC_SCTRL_SC_PERI_POWER_CTRL_1_ADDR(base)             ((base) + 0xF74UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_CTRL_2_UNION */
#define SOC_SCTRL_SC_PERI_POWER_CTRL_2_ADDR(base)             ((base) + 0xF78UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_UNION */
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_ADDR(base)          ((base) + 0xF80UL)

/* Register description: 用于指示系统曾经进入过低功耗的状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_STAT_UNION */
#define SOC_SCTRL_SC_LP_STAT_ADDR(base)                       ((base) + 0x008UL)

/* Register description: 用于清除指示系统曾经进入过低功耗的状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_STAT_CLR_UNION */
#define SOC_SCTRL_SC_LP_STAT_CLR_ADDR(base)                   ((base) + 0x01CUL)

/* Register description: 状态记录寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_FLAG_UNION */
#define SOC_SCTRL_SC_LP_FLAG_ADDR(base)                       ((base) + 0x018UL)

/* Register description: 用于指示系统曾经进入过深度睡眠的状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_INTR_WAKEUP_CTRL_UNION */
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_ADDR(base)              ((base) + 0x014UL)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA2_UNION */
#define SOC_SCTRL_SCEFUSEDATA2_ADDR(base)                     ((base) + 0x00CUL)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA3_UNION */
#define SOC_SCTRL_SCEFUSEDATA3_ADDR(base)                     ((base) + 0x010UL)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA8_UNION */
#define SOC_SCTRL_SCEFUSEDATA8_ADDR(base)                     ((base) + 0x060UL)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA9_UNION */
#define SOC_SCTRL_SCEFUSEDATA9_ADDR(base)                     ((base) + 0x064UL)

/* Register description: 系统低功耗软件寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_POR_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_POR_ADDR(base)             ((base) + 0xE50UL)

/* Register description: 系统低功耗软件安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_ADDR(base)           ((base) + 0xE54UL)

/* Register description: 系统低功耗软件安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_ADDR(base)           ((base) + 0xE58UL)

/* Register description: 系统低功耗软件非安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_ADDR(base)         ((base) + 0x024UL)

/* Register description: 系统低功耗软件非安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_ADDR(base)         ((base) + 0x034UL)

/* Register description: SC数据记录寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_DATA_UNION */
#define SOC_SCTRL_SC_LP_DATA_ADDR(base)                       ((base) + 0x020UL)

/* Register description: 低功耗配置状态
   Bit domain definition UNION:  SOC_SCTRL_SCLPSTATCFG_UNION */
#define SOC_SCTRL_SCLPSTATCFG_ADDR(base)                      ((base) + 0x038UL)

/* Register description: ASP上下电配置寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SC_ASP_PWRCFG0_UNION */
#define SOC_SCTRL_SC_ASP_PWRCFG0_ADDR(base)                   ((base) + 0x080UL)

/* Register description: ASP上下电配置寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SC_ASP_PWRCFG1_UNION */
#define SOC_SCTRL_SC_ASP_PWRCFG1_ADDR(base)                   ((base) + 0x084UL)

/* Register description: 聚合后的唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_GATHER_STAT_UNION */
#define SOC_SCTRL_SCINT_GATHER_STAT_ADDR(base)                ((base) + 0x0A0UL)

/* Register description: intr_ao_wakeup0唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK_UNION */
#define SOC_SCTRL_SCINT_MASK_ADDR(base)                       ((base) + 0x0A4UL)

/* Register description: intr_ao_wakeup0原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT_UNION */
#define SOC_SCTRL_SCINT_STAT_ADDR(base)                       ((base) + 0x0A8UL)

/* Register description: DRX唤醒中断分配配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCDRX_INT_CFG_UNION */
#define SOC_SCTRL_SCDRX_INT_CFG_ADDR(base)                    ((base) + 0x0ACUL)

/* Register description: LPMCU WFI唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUWFI_INT_UNION */
#define SOC_SCTRL_SCLPMCUWFI_INT_ADDR(base)                   ((base) + 0x0B0UL)

/* Register description: intr_ao_wakeup1唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK1_UNION */
#define SOC_SCTRL_SCINT_MASK1_ADDR(base)                      ((base) + 0x0B4UL)

/* Register description: intr_ao_wakeup1原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT1_UNION */
#define SOC_SCTRL_SCINT_STAT1_ADDR(base)                      ((base) + 0x0B8UL)

/* Register description: intr_peri_wakeup唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK2_UNION */
#define SOC_SCTRL_SCINT_MASK2_ADDR(base)                      ((base) + 0x0BCUL)

/* Register description: intr_peri_wakeup原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT2_UNION */
#define SOC_SCTRL_SCINT_STAT2_ADDR(base)                      ((base) + 0x0C0UL)

/* Register description: intr_peri_wakeup0唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK3_UNION */
#define SOC_SCTRL_SCINT_MASK3_ADDR(base)                      ((base) + 0x0C4UL)

/* Register description: intr_peri_wakeup0原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT3_UNION */
#define SOC_SCTRL_SCINT_STAT3_ADDR(base)                      ((base) + 0x0C8UL)

/* Register description: intr_drx_wakeup唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK4_UNION */
#define SOC_SCTRL_SCINT_MASK4_ADDR(base)                      ((base) + 0x090UL)

/* Register description: intr_drx_wakeup原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT4_UNION */
#define SOC_SCTRL_SCINT_STAT4_ADDR(base)                      ((base) + 0x094UL)

/* Register description: intr_ao_wakeup2唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK5_UNION */
#define SOC_SCTRL_SCINT_MASK5_ADDR(base)                      ((base) + 0x098UL)

/* Register description: intr_ao_wakeup2原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT5_UNION */
#define SOC_SCTRL_SCINT_STAT5_ADDR(base)                      ((base) + 0x09CUL)

/* Register description: 唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_INT_STD2LS_MASK_UNION */
#define SOC_SCTRL_SCINT_INT_STD2LS_MASK_ADDR(base)            ((base) + 0x0D0UL)

/* Register description: 唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_INT_LS2STD_MASK_UNION */
#define SOC_SCTRL_SCINT_INT_LS2STD_MASK_ADDR(base)            ((base) + 0x0D4UL)

/* Register description: 外设控制寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL0_UNION */
#define SOC_SCTRL_SCPERCTRL0_ADDR(base)                       ((base) + 0x300UL)

/* Register description: 外设控制寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL1_UNION */
#define SOC_SCTRL_SCPERCTRL1_ADDR(base)                       ((base) + 0x304UL)

/* Register description: 外设控制寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL2_UNION */
#define SOC_SCTRL_SCPERCTRL2_ADDR(base)                       ((base) + 0x308UL)

/* Register description: 外设控制寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL3_UNION */
#define SOC_SCTRL_SCPERCTRL3_ADDR(base)                       ((base) + 0x30CUL)

/* Register description: 外设控制寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL4_UNION */
#define SOC_SCTRL_SCPERCTRL4_ADDR(base)                       ((base) + 0x310UL)

/* Register description: 外设控制寄存器5
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL5_UNION */
#define SOC_SCTRL_SCPERCTRL5_ADDR(base)                       ((base) + 0x314UL)

/* Register description: 外设控制寄存器6
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL6_UNION */
#define SOC_SCTRL_SCPERCTRL6_ADDR(base)                       ((base) + 0x318UL)

/* Register description: 外设控制寄存器7
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL7_UNION */
#define SOC_SCTRL_SCPERCTRL7_ADDR(base)                       ((base) + 0x31CUL)

/* Register description: 外设控制寄存器9
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL9_UNION */
#define SOC_SCTRL_SCPERCTRL9_ADDR(base)                       ((base) + 0x324UL)

/* Register description: 外设控制寄存器10
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL10_UNION */
#define SOC_SCTRL_SCPERCTRL10_ADDR(base)                      ((base) + 0x328UL)

/* Register description: 外设控制寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL11_UNION */
#define SOC_SCTRL_SCPERCTRL11_ADDR(base)                      ((base) + 0x32CUL)

/* Register description: 外设控制寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL12_UNION */
#define SOC_SCTRL_SCPERCTRL12_ADDR(base)                      ((base) + 0x330UL)

/* Register description: 外设控制寄存器13
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL13_UNION */
#define SOC_SCTRL_SCPERCTRL13_ADDR(base)                      ((base) + 0x334UL)

/* Register description: 外设控制寄存器14
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL14_UNION */
#define SOC_SCTRL_SCPERCTRL14_ADDR(base)                      ((base) + 0x338UL)

/* Register description: 外设控制寄存器15
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL15_UNION */
#define SOC_SCTRL_SCPERCTRL15_ADDR(base)                      ((base) + 0x33CUL)

/* Register description: 外设控制寄存器8，安全寄存器，控制信号由SCTRL内部逻辑产生的状态输出控制
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL16_UNION */
#define SOC_SCTRL_SCPERCTRL16_ADDR(base)                      ((base) + 0x340UL)

/* Register description: 外设控制寄存器8，安全寄存器，控制信号由SCTRL内部逻辑产生的状态输出控制
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL17_UNION */
#define SOC_SCTRL_SCPERCTRL17_ADDR(base)                      ((base) + 0x344UL)

/* Register description: 外设状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS0_UNION */
#define SOC_SCTRL_SCPERSTATUS0_ADDR(base)                     ((base) + 0x360UL)

/* Register description: 外设状态寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS1_UNION */
#define SOC_SCTRL_SCPERSTATUS1_ADDR(base)                     ((base) + 0x364UL)

/* Register description: 外设状态寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS2_UNION */
#define SOC_SCTRL_SCPERSTATUS2_ADDR(base)                     ((base) + 0x368UL)

/* Register description: 外设状态寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS3_UNION */
#define SOC_SCTRL_SCPERSTATUS3_ADDR(base)                     ((base) + 0x36CUL)

/* Register description: 外设状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS4_UNION */
#define SOC_SCTRL_SCPERSTATUS4_ADDR(base)                     ((base) + 0x370UL)

/* Register description: 外设状态寄存器5
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS5_UNION */
#define SOC_SCTRL_SCPERSTATUS5_ADDR(base)                     ((base) + 0x374UL)

/* Register description: 外设状态寄存器6
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS6_UNION */
#define SOC_SCTRL_SCPERSTATUS6_ADDR(base)                     ((base) + 0x378UL)

/* Register description: 外设状态寄存器7
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS7_UNION */
#define SOC_SCTRL_SCPERSTATUS7_ADDR(base)                     ((base) + 0x37CUL)

/* Register description: 外设状态寄存器8
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS8_UNION */
#define SOC_SCTRL_SCPERSTATUS8_ADDR(base)                     ((base) + 0x380UL)

/* Register description: 外设状态寄存器9
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS9_UNION */
#define SOC_SCTRL_SCPERSTATUS9_ADDR(base)                     ((base) + 0x384UL)

/* Register description: 外设状态寄存器10
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS10_UNION */
#define SOC_SCTRL_SCPERSTATUS10_ADDR(base)                    ((base) + 0x388UL)

/* Register description: 外设状态寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS11_UNION */
#define SOC_SCTRL_SCPERSTATUS11_ADDR(base)                    ((base) + 0x38CUL)

/* Register description: 外设状态寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS12_UNION */
#define SOC_SCTRL_SCPERSTATUS12_ADDR(base)                    ((base) + 0x390UL)

/* Register description: 外设状态寄存器13
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS13_UNION */
#define SOC_SCTRL_SCPERSTATUS13_ADDR(base)                    ((base) + 0x394UL)

/* Register description: 外设状态寄存器14
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS14_UNION */
#define SOC_SCTRL_SCPERSTATUS14_ADDR(base)                    ((base) + 0x398UL)

/* Register description: 外设状态寄存器15
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS15_UNION */
#define SOC_SCTRL_SCPERSTATUS15_ADDR(base)                    ((base) + 0x39CUL)

/* Register description: 内部状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINNERSTAT_UNION */
#define SOC_SCTRL_SCINNERSTAT_ADDR(base)                      ((base) + 0x3A0UL)

/* Register description: 内部使用的配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINNERCTRL_UNION */
#define SOC_SCTRL_SCINNERCTRL_ADDR(base)                      ((base) + 0x3A4UL)

/* Register description: MRB EFUSE BUSY状态指示寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMRBBUSYSTAT_UNION */
#define SOC_SCTRL_SCMRBBUSYSTAT_ADDR(base)                    ((base) + 0x3FCUL)

/* Register description: 用于保存软件入口地址的寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSWADDR_UNION */
#define SOC_SCTRL_SCSWADDR_ADDR(base)                         ((base) + 0x400UL)

/* Register description: 用于保留DDR训练数据的地址的寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCDDRADDR_UNION */
#define SOC_SCTRL_SCDDRADDR_ADDR(base)                        ((base) + 0x404UL)

/* Register description: 用于保存DDR训练数据的寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCDDRDATA_UNION */
#define SOC_SCTRL_SCDDRDATA_ADDR(base)                        ((base) + 0x408UL)

/* Register description: 用于软件使用的存储数据寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA0_UNION */
#define SOC_SCTRL_SCBAKDATA0_ADDR(base)                       ((base) + 0x40CUL)

/* Register description: 用于软件使用的存储数据寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA1_UNION */
#define SOC_SCTRL_SCBAKDATA1_ADDR(base)                       ((base) + 0x410UL)

/* Register description: 用于软件使用的存储数据寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA2_UNION */
#define SOC_SCTRL_SCBAKDATA2_ADDR(base)                       ((base) + 0x414UL)

/* Register description: 用于软件使用的存储数据寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA3_UNION */
#define SOC_SCTRL_SCBAKDATA3_ADDR(base)                       ((base) + 0x418UL)

/* Register description: 用于软件使用的存储数据寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA4_UNION */
#define SOC_SCTRL_SCBAKDATA4_ADDR(base)                       ((base) + 0x41CUL)

/* Register description: 用于软件使用的存储数据寄存器5
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA5_UNION */
#define SOC_SCTRL_SCBAKDATA5_ADDR(base)                       ((base) + 0x420UL)

/* Register description: 用于软件使用的存储数据寄存器6
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA6_UNION */
#define SOC_SCTRL_SCBAKDATA6_ADDR(base)                       ((base) + 0x424UL)

/* Register description: 用于软件使用的存储数据寄存器7
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA7_UNION */
#define SOC_SCTRL_SCBAKDATA7_ADDR(base)                       ((base) + 0x428UL)

/* Register description: 用于软件使用的存储数据寄存器8
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA8_UNION */
#define SOC_SCTRL_SCBAKDATA8_ADDR(base)                       ((base) + 0x42CUL)

/* Register description: 用于软件使用的存储数据寄存器9
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA9_UNION */
#define SOC_SCTRL_SCBAKDATA9_ADDR(base)                       ((base) + 0x430UL)

/* Register description: 用于软件使用的存储数据寄存器10
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA10_UNION */
#define SOC_SCTRL_SCBAKDATA10_ADDR(base)                      ((base) + 0x434UL)

/* Register description: 用于软件使用的存储数据寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA11_UNION */
#define SOC_SCTRL_SCBAKDATA11_ADDR(base)                      ((base) + 0x438UL)

/* Register description: 用于软件使用的存储数据寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA12_UNION */
#define SOC_SCTRL_SCBAKDATA12_ADDR(base)                      ((base) + 0x43CUL)

/* Register description: 用于软件使用的存储数据寄存器13
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA13_UNION */
#define SOC_SCTRL_SCBAKDATA13_ADDR(base)                      ((base) + 0x440UL)

/* Register description: 用于软件使用的存储数据寄存器14
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA14_UNION */
#define SOC_SCTRL_SCBAKDATA14_ADDR(base)                      ((base) + 0x444UL)

/* Register description: 用于软件使用的存储数据寄存器15
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA15_UNION */
#define SOC_SCTRL_SCBAKDATA15_ADDR(base)                      ((base) + 0x448UL)

/* Register description: 用于软件使用的存储数据寄存器16
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA16_UNION */
#define SOC_SCTRL_SCBAKDATA16_ADDR(base)                      ((base) + 0x44CUL)

/* Register description: 用于软件使用的存储数据寄存器17
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA17_UNION */
#define SOC_SCTRL_SCBAKDATA17_ADDR(base)                      ((base) + 0x450UL)

/* Register description: 用于软件使用的存储数据寄存器18
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA18_UNION */
#define SOC_SCTRL_SCBAKDATA18_ADDR(base)                      ((base) + 0x454UL)

/* Register description: 用于软件使用的存储数据寄存器19
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA19_UNION */
#define SOC_SCTRL_SCBAKDATA19_ADDR(base)                      ((base) + 0x458UL)

/* Register description: 用于软件使用的存储数据寄存器20
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA20_UNION */
#define SOC_SCTRL_SCBAKDATA20_ADDR(base)                      ((base) + 0x45CUL)

/* Register description: 用于软件使用的存储数据寄存器21
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA21_UNION */
#define SOC_SCTRL_SCBAKDATA21_ADDR(base)                      ((base) + 0x460UL)

/* Register description: 用于软件使用的存储数据寄存器22
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA22_UNION */
#define SOC_SCTRL_SCBAKDATA22_ADDR(base)                      ((base) + 0x464UL)

/* Register description: 用于软件使用的存储数据寄存器23
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA23_UNION */
#define SOC_SCTRL_SCBAKDATA23_ADDR(base)                      ((base) + 0x468UL)

/* Register description: 用于软件使用的存储数据寄存器24
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA24_UNION */
#define SOC_SCTRL_SCBAKDATA24_ADDR(base)                      ((base) + 0x46CUL)

/* Register description: 用于软件使用的存储数据寄存器25
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA25_UNION */
#define SOC_SCTRL_SCBAKDATA25_ADDR(base)                      ((base) + 0x470UL)

/* Register description: 用于软件使用的存储数据寄存器26
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA26_UNION */
#define SOC_SCTRL_SCBAKDATA26_ADDR(base)                      ((base) + 0x474UL)

/* Register description: 用于软件使用的存储数据寄存器27
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA27_UNION */
#define SOC_SCTRL_SCBAKDATA27_ADDR(base)                      ((base) + 0x478UL)

/* Register description: 用于软件使用的存储数据寄存器28
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA28_UNION */
#define SOC_SCTRL_SCBAKDATA28_ADDR(base)                      ((base) + 0x47CUL)

/* Register description: 外设状态非复位保留寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS0_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS0_ADDR(base)       ((base) + 0x480UL)

/* Register description: 外设状态非复位保留寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS1_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS1_ADDR(base)       ((base) + 0x484UL)

/* Register description: 外设状态非复位保留寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS2_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS2_ADDR(base)       ((base) + 0x488UL)

/* Register description: 外设状态非复位保留寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS3_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS3_ADDR(base)       ((base) + 0x48CUL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA0_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA0_MSK_ADDR(base)                   ((base) + 0x4C0UL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA1_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA1_MSK_ADDR(base)                   ((base) + 0x4C4UL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA2_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA2_MSK_ADDR(base)                   ((base) + 0x4C8UL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA3_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA3_MSK_ADDR(base)                   ((base) + 0x4CCUL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA4_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA4_MSK_ADDR(base)                   ((base) + 0x4D0UL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA5_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA5_MSK_ADDR(base)                   ((base) + 0x4D4UL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA6_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA6_MSK_ADDR(base)                   ((base) + 0x4D8UL)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA7_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA7_MSK_ADDR(base)                   ((base) + 0x4DCUL)

/* Register description: LPMCU子系统时钟使能寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCLKEN_UNION */
#define SOC_SCTRL_SCLPMCUCLKEN_ADDR(base)                     ((base) + 0x960UL)

/* Register description: LPMCU子系统软时钟关闭寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCLKDIS_UNION */
#define SOC_SCTRL_SCLPMCUCLKDIS_ADDR(base)                    ((base) + 0x964UL)

/* Register description: LPMCU子系统软时钟状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCLKSTAT_UNION */
#define SOC_SCTRL_SCLPMCUCLKSTAT_ADDR(base)                   ((base) + 0x968UL)

/* Register description: LPMCU子系统软复位使能寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURSTEN_UNION */
#define SOC_SCTRL_SCLPMCURSTEN_ADDR(base)                     ((base) + 0x970UL)

/* Register description: LPMCU子系统软复位撤离寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURSTDIS_UNION */
#define SOC_SCTRL_SCLPMCURSTDIS_ADDR(base)                    ((base) + 0x974UL)

/* Register description: LPMCU子系统软复位使能状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURSTSTAT_UNION */
#define SOC_SCTRL_SCLPMCURSTSTAT_ADDR(base)                   ((base) + 0x978UL)

/* Register description: LPMCU子系统控制配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCTRL_UNION */
#define SOC_SCTRL_SCLPMCUCTRL_ADDR(base)                      ((base) + 0x980UL)

/* Register description: LPMCU子系统状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUSTAT_UNION */
#define SOC_SCTRL_SCLPMCUSTAT_ADDR(base)                      ((base) + 0x984UL)

/* Register description: LPMCU RAM 控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURAMCTRL_UNION */
#define SOC_SCTRL_SCLPMCURAMCTRL_ADDR(base)                   ((base) + 0x988UL)

/* Register description: BBPDRX子系统状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT0_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT0_ADDR(base)                    ((base) + 0x530UL)

/* Register description: BBPDRX子系统状态寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT1_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT1_ADDR(base)                    ((base) + 0x534UL)

/* Register description: BBPDRX子系统状态寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT2_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT2_ADDR(base)                    ((base) + 0x538UL)

/* Register description: BBPDRX子系统状态寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT3_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT3_ADDR(base)                    ((base) + 0x53CUL)

/* Register description: BBPDRX子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT4_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT4_ADDR(base)                    ((base) + 0x540UL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCA53_EVENT_MASK_UNION */
#define SOC_SCTRL_SCA53_EVENT_MASK_ADDR(base)                 ((base) + 0x550UL)

/* Register description: A57 EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCA57_EVENT_MASK_UNION */
#define SOC_SCTRL_SCA57_EVENT_MASK_ADDR(base)                 ((base) + 0x554UL)

/* Register description: IOMCU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_ADDR(base)               ((base) + 0x558UL)

/* Register description: LPMCU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_ADDR(base)               ((base) + 0x55CUL)

/* Register description: MCPU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMCPU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCMCPU_EVENT_MASK_ADDR(base)                ((base) + 0x560UL)

/* Register description: MCPU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCISPA7_EVENT_MASK_UNION */
#define SOC_SCTRL_SCISPA7_EVENT_MASK_ADDR(base)               ((base) + 0x568UL)

/* Register description: MCPU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCHIFD_UL_EVENT_MASK_UNION */
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_ADDR(base)             ((base) + 0x56CUL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_ADDR(base)           ((base) + 0x570UL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_ADDR(base)           ((base) + 0x574UL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_UNION */
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_ADDR(base)           ((base) + 0x578UL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSILS_EVENT_MASK_UNION */
#define SOC_SCTRL_SCSILS_EVENT_MASK_ADDR(base)                ((base) + 0x97CUL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_UNION */
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_ADDR(base)         ((base) + 0x544UL)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_UNION */
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_ADDR(base)        ((base) + 0x548UL)

/* Register description: EVENT状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEVENT_STAT_UNION */
#define SOC_SCTRL_SCEVENT_STAT_ADDR(base)                     ((base) + 0x564UL)

/* Register description: IOMCU时钟控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUCLKCTRL_UNION */
#define SOC_SCTRL_SCIOMCUCLKCTRL_ADDR(base)                   ((base) + 0x880UL)

/* Register description: IOMCU时钟控制信号状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUCLKSTAT_UNION */
#define SOC_SCTRL_SCIOMCUCLKSTAT_ADDR(base)                   ((base) + 0x584UL)

/* Register description: IOMCU子系统控制配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUCTRL_UNION */
#define SOC_SCTRL_SCIOMCUCTRL_ADDR(base)                      ((base) + 0x588UL)

/* Register description: IOMCU子系统状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUSTAT_UNION */
#define SOC_SCTRL_SCIOMCUSTAT_ADDR(base)                      ((base) + 0x58CUL)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_ADDR(base)              ((base) + 0x500UL)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_ADDR(base)            ((base) + 0x504UL)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_ADDR(base)             ((base) + 0x508UL)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_ADDR(base)              ((base) + 0x50CUL)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_ADDR(base)            ((base) + 0x510UL)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_ADDR(base)             ((base) + 0x514UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR0_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR0_NONSEC_ADDR(base)                ((base) + 0x700UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR1_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR1_NONSEC_ADDR(base)                ((base) + 0x704UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR2_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR2_NONSEC_ADDR(base)                ((base) + 0x708UL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR3_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR3_NONSEC_ADDR(base)                ((base) + 0x70CUL)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_SEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_ADDR(base)                ((base) + 0x8E8UL)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_ADDR(base)              ((base) + 0x8ECUL)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_SEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_ADDR(base)               ((base) + 0x8F0UL)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_SEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_ADDR(base)                ((base) + 0x8F4UL)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_ADDR(base)              ((base) + 0x8F8UL)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_SEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_ADDR(base)               ((base) + 0x8FCUL)

/* Register description: JTAG选择控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAG_SEL_UNION */
#define SOC_SCTRL_SCJTAG_SEL_ADDR(base)                       ((base) + 0x800UL)

/* Register description: DJTAG控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCFG_DJTAG_UNION */
#define SOC_SCTRL_SCCFG_DJTAG_ADDR(base)                      ((base) + 0x814UL)

/* Register description: CP15DISABLE控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCP15_DISABLE_UNION */
#define SOC_SCTRL_SCCP15_DISABLE_ADDR(base)                   ((base) + 0x818UL)

/* Register description: ARM debug en配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCFG_ARM_DBGEN_UNION */
#define SOC_SCTRL_SCCFG_ARM_DBGEN_ADDR(base)                  ((base) + 0x82CUL)

/* Register description: ARM debug en状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBGEN_STAT_UNION */
#define SOC_SCTRL_SCARM_DBGEN_STAT_ADDR(base)                 ((base) + 0x838UL)

/* Register description: EFUSE PAD烧写控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSECTRL_UNION */
#define SOC_SCTRL_SCEFUSECTRL_ADDR(base)                      ((base) + 0x83CUL)

/* Register description: SECS时钟门控寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSESEL_UNION */
#define SOC_SCTRL_SCEFUSESEL_ADDR(base)                       ((base) + 0x840UL)

/* Register description: EFUSE CHIP_ID0寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCHIP_ID0_UNION */
#define SOC_SCTRL_SCCHIP_ID0_ADDR(base)                       ((base) + 0x848UL)

/* Register description: EFUSE CHIP_ID1寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCHIP_ID1_UNION */
#define SOC_SCTRL_SCCHIP_ID1_ADDR(base)                       ((base) + 0x84CUL)

/* Register description: CPU安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCPUSECCTRL_UNION */
#define SOC_SCTRL_SCCPUSECCTRL_ADDR(base)                     ((base) + 0x850UL)

/* Register description: ISP安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCISPSECCTRL_UNION */
#define SOC_SCTRL_SCISPSECCTRL_ADDR(base)                     ((base) + 0x830UL)

/* Register description: JTAG_SD SWITCH选择寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAGSD_SW_SEL_UNION */
#define SOC_SCTRL_SCJTAGSD_SW_SEL_ADDR(base)                  ((base) + 0x854UL)

/* Register description: JTAG_SYS SWITCH选择寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAGSYS_SW_SEL_UNION */
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_ADDR(base)                 ((base) + 0x858UL)

/* Register description: JTAG回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAG_SD_SWD2jtag_dfx_UNION */
#define SOC_SCTRL_SCJTAG_SD_SWD2jtag_dfx_ADDR(base)           ((base) + 0x870UL)

/* Register description: JTAG回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAG_SYS_SWD2jtag_dfx_UNION */
#define SOC_SCTRL_SCJTAG_SYS_SWD2jtag_dfx_ADDR(base)          ((base) + 0x874UL)

/* Register description: syscache中断控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_INTR_SYSCACHE_SEC_UNION */
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_ADDR(base)             ((base) + 0x85CUL)

/* Register description: ARM debug key0配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY0_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY0_ADDR(base)                   ((base) + 0x860UL)

/* Register description: ARM debug key1配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY1_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY1_ADDR(base)                   ((base) + 0x864UL)

/* Register description: ARM debug key2配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY2_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY2_ADDR(base)                   ((base) + 0x868UL)

/* Register description: ARM debug key3配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY3_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY3_ADDR(base)                   ((base) + 0x86CUL)

/* Register description: ARM debug key4配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY4_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY4_ADDR(base)                   ((base) + 0x884UL)

/* Register description: ARM debug key5配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY5_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY5_ADDR(base)                   ((base) + 0x888UL)

/* Register description: ARM debug key6配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY6_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY6_ADDR(base)                   ((base) + 0x88CUL)

/* Register description: ARM debug key7配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY7_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY7_ADDR(base)                   ((base) + 0x890UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR0_UNION */
#define SOC_SCTRL_SCSPMIADDR0_ADDR(base)                      ((base) + 0x920UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR1_UNION */
#define SOC_SCTRL_SCSPMIADDR1_ADDR(base)                      ((base) + 0x924UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR2_UNION */
#define SOC_SCTRL_SCSPMIADDR2_ADDR(base)                      ((base) + 0x928UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR3_UNION */
#define SOC_SCTRL_SCSPMIADDR3_ADDR(base)                      ((base) + 0x92CUL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR4_UNION */
#define SOC_SCTRL_SCSPMIADDR4_ADDR(base)                      ((base) + 0x930UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR5_UNION */
#define SOC_SCTRL_SCSPMIADDR5_ADDR(base)                      ((base) + 0x934UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR6_UNION */
#define SOC_SCTRL_SCSPMIADDR6_ADDR(base)                      ((base) + 0x938UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR7_UNION */
#define SOC_SCTRL_SCSPMIADDR7_ADDR(base)                      ((base) + 0x93CUL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR8_UNION */
#define SOC_SCTRL_SCSPMIADDR8_ADDR(base)                      ((base) + 0x940UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR9_UNION */
#define SOC_SCTRL_SCSPMIADDR9_ADDR(base)                      ((base) + 0x944UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR10_UNION */
#define SOC_SCTRL_SCSPMIADDR10_ADDR(base)                     ((base) + 0x948UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR11_UNION */
#define SOC_SCTRL_SCSPMIADDR11_ADDR(base)                     ((base) + 0x94CUL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR12_UNION */
#define SOC_SCTRL_SCSPMIADDR12_ADDR(base)                     ((base) + 0x950UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR13_UNION */
#define SOC_SCTRL_SCSPMIADDR13_ADDR(base)                     ((base) + 0x954UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR14_UNION */
#define SOC_SCTRL_SCSPMIADDR14_ADDR(base)                     ((base) + 0x958UL)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR15_UNION */
#define SOC_SCTRL_SCSPMIADDR15_ADDR(base)                     ((base) + 0x95CUL)

/* Register description: HISEE子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCHISEEGPIOLOCK_UNION */
#define SOC_SCTRL_SCHISEEGPIOLOCK_ADDR(base)                  ((base) + 0x894UL)

/* Register description: HISEE子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCHISEESPILOCK_UNION */
#define SOC_SCTRL_SCHISEESPILOCK_ADDR(base)                   ((base) + 0x898UL)

/* Register description: HISEE子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCHISEEI2CLOCK_UNION */
#define SOC_SCTRL_SCHISEEI2CLOCK_ADDR(base)                   ((base) + 0x89CUL)

/* Register description: HISEE IO选择回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCHISEEIOSEL_UNION */
#define SOC_SCTRL_SCHISEEIOSEL_ADDR(base)                     ((base) + 0x8A0UL)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL0_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL0_SEC_ADDR(base)                   ((base) + 0x8A4UL)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL1_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL1_SEC_ADDR(base)                   ((base) + 0x8B0UL)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL2_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL2_SEC_ADDR(base)                   ((base) + 0x8B4UL)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL3_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL3_SEC_ADDR(base)                   ((base) + 0x8B8UL)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL4_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL4_SEC_ADDR(base)                   ((base) + 0x8BCUL)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL5_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL5_SEC_ADDR(base)                   ((base) + 0x8E0UL)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS0_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS0_SEC_ADDR(base)                 ((base) + 0x8C0UL)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS1_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS1_SEC_ADDR(base)                 ((base) + 0x8C4UL)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS2_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS2_SEC_ADDR(base)                 ((base) + 0x8C8UL)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS3_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS3_SEC_ADDR(base)                 ((base) + 0x8CCUL)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ_0_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_ADDR(base)                ((base) + 0x8D0UL)

/* Register description: SC_TCP_VOTE_STAT
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_STAT_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_STAT_ADDR(base)                 ((base) + 0x8D4UL)

/* Register description: SC_TCP_VOTE_CTRL
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_CTRL0_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_ADDR(base)                ((base) + 0x8D8UL)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ_1_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_ADDR(base)                ((base) + 0x8DCUL)

/* Register description: SC_TCP_VOTE_CTRL
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_CTRL1_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_ADDR(base)                ((base) + 0x8E4UL)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_ADDR(base)            ((base) + 0x808UL)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_ADDR(base)            ((base) + 0x80CUL)

/* Register description: SOCID寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCSOCID0_UNION */
#define SOC_SCTRL_SCSOCID0_ADDR(base)                         ((base) + 0xE00UL)

/* Register description: 外设状态非复位保留寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED0_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED0_ADDR(base)          ((base) + 0xE10UL)

/* Register description: 外设状态非复位保留寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED1_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED1_ADDR(base)          ((base) + 0xE14UL)

/* Register description: 外设状态非复位保留寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED2_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED2_ADDR(base)          ((base) + 0xE18UL)

/* Register description: 外设状态非复位保留寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED3_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED3_ADDR(base)          ((base) + 0xE1CUL)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL0_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL0_SEC_ADDR(base)               ((base) + 0xE20UL)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL1_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL1_SEC_ADDR(base)               ((base) + 0xE24UL)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL2_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL2_SEC_ADDR(base)               ((base) + 0xE28UL)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL3_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL3_SEC_ADDR(base)               ((base) + 0xE2CUL)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL4_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL4_SEC_ADDR(base)               ((base) + 0xE30UL)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG0_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG0_ADDR(base)                 ((base) + 0xE40UL)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG1_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG1_ADDR(base)                 ((base) + 0xE44UL)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG2_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG2_ADDR(base)                 ((base) + 0xE48UL)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG3_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG3_ADDR(base)                 ((base) + 0xE4CUL)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_EMU_VERSION_UNION */
#define SOC_SCTRL_SC_EMU_VERSION_ADDR(base)                   ((base) + 0xE34UL)


#else


/* Register description: 系统控制器寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCTRL_UNION */
#define SOC_SCTRL_SCCTRL_ADDR(base)                           ((base) + 0xF00)

/* Register description: 系统控制器寄存器
   Bit domain definition UNION:  SOC_SCTRL_STD2LS_CTRL_UNION */
#define SOC_SCTRL_STD2LS_CTRL_ADDR(base)                      ((base) + 0xFB0)

/* Register description: 系统控制器寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_CTRL_UNION */
#define SOC_SCTRL_DOZE_CTRL_ADDR(base)                        ((base) + 0xFC8)

/* Register description: 系统状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSYSSTAT_UNION */
#define SOC_SCTRL_SCSYSSTAT_ADDR(base)                        ((base) + 0xF04)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_0_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_0_ADDR(base)                  ((base) + 0xF14)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_1_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_1_ADDR(base)                  ((base) + 0xF18)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_2_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_2_ADDR(base)                  ((base) + 0xF1C)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_3_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_3_ADDR(base)                  ((base) + 0xFB4)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_4_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_4_ADDR(base)                  ((base) + 0xF10)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_5_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_5_ADDR(base)                  ((base) + 0xF20)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_CTRL_6_UNION */
#define SOC_SCTRL_SYS_STAT_CTRL_6_ADDR(base)                  ((base) + 0xF24)

/* Register description: PMU控制寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SC_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_ADDR(base)                 ((base) + 0xF90)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_ADDR(base)       ((base) + 0xE60)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_ADDR(base)       ((base) + 0xE64)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_ADDR(base)       ((base) + 0xE68)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_ADDR(base)      ((base) + 0xE6C)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_ADDR(base)      ((base) + 0xE70)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_ADDR(base)      ((base) + 0xE74)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_ADDR(base)         ((base) + 0xE78)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_ADDR(base)         ((base) + 0xE7C)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_ADDR(base)         ((base) + 0xE80)

/* Register description: PMU PS控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_ADDR(base)          ((base) + 0xE84)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_ADDR(base)          ((base) + 0xE88)

/* Register description: PMU PS超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_ADDR(base)          ((base) + 0xE8C)

/* Register description: PMU Power Sequence 状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_PMU_PS_STAT_0_UNION */
#define SOC_SCTRL_SC_PMU_PS_STAT_0_ADDR(base)                 ((base) + 0xF9C)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_SC_STAT_0_UNION */
#define SOC_SCTRL_SYS_SC_STAT_0_ADDR(base)                    ((base) + 0xF2C)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_SC_STAT_1_UNION */
#define SOC_SCTRL_SYS_SC_STAT_1_ADDR(base)                    ((base) + 0xFE4)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_SLOW_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_ADDR(base)               ((base) + 0xE90)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_ADDR(base)          ((base) + 0xE94)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_ADDR(base)         ((base) + 0xE98)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_STANDBY_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_ADDR(base)            ((base) + 0xE9C)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LS2STD_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_ADDR(base)             ((base) + 0xEA0)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_DOZE1_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_ADDR(base)              ((base) + 0xFC0)

/* Register description: 系统状态控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_DOZE2_CTRL_UNION */
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_ADDR(base)              ((base) + 0xFC4)

/* Register description: DOZE1下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_ADDR(base)           ((base) + 0xF94)

/* Register description: DOZE1下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_ADDR(base)           ((base) + 0xFD4)

/* Register description: DOZE1下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_ADDR(base)           ((base) + 0xFD8)

/* Register description: DOZE2下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_UNION */
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_ADDR(base)           ((base) + 0xFBC)

/* Register description: DOZE2下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_UNION */
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_ADDR(base)           ((base) + 0xFDC)

/* Register description: DOZE2下PMU控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_UNION */
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_ADDR(base)           ((base) + 0xFE0)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE0_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE0_ADDR(base)                   ((base) + 0x028)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE1_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE1_ADDR(base)                   ((base) + 0x02C)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE2_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE2_ADDR(base)                   ((base) + 0x9A0)

/* Register description: DOZE1软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE1_VOTE3_UNION */
#define SOC_SCTRL_SC_DOZE1_VOTE3_ADDR(base)                   ((base) + 0x9A4)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE0_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE0_ADDR(base)                   ((base) + 0x040)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE1_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE1_ADDR(base)                   ((base) + 0x044)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE2_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE2_ADDR(base)                   ((base) + 0x9A8)

/* Register description: DOZE2软投票寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_DOZE2_VOTE3_UNION */
#define SOC_SCTRL_SC_DOZE2_VOTE3_ADDR(base)                   ((base) + 0x9AC)

/* Register description: DOZE控制寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SW_INDICATORS_MASK0_UNION */
#define SOC_SCTRL_SW_INDICATORS_MASK0_ADDR(base)              ((base) + 0xF40)

/* Register description: DOZE控制寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SW_INDICATORS_MASK1_UNION */
#define SOC_SCTRL_SW_INDICATORS_MASK1_ADDR(base)              ((base) + 0xFE8)

/* Register description: DOZE控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SW_DDR_IDLE_MASK_UNION */
#define SOC_SCTRL_SW_DDR_IDLE_MASK_ADDR(base)                 ((base) + 0xF44)

/* Register description: DOZE超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_COUNT_CTRL0_UNION */
#define SOC_SCTRL_DOZE_COUNT_CTRL0_ADDR(base)                 ((base) + 0xF48)

/* Register description: DOZE超时控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_COUNT_CTRL1_UNION */
#define SOC_SCTRL_DOZE_COUNT_CTRL1_ADDR(base)                 ((base) + 0xFCC)

/* Register description: DOZE1滤波控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_UNION */
#define SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_ADDR(base)              ((base) + 0xE04)

/* Register description: DOZE2滤波控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_UNION */
#define SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_ADDR(base)              ((base) + 0xE08)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_0_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_0_ADDR(base)                  ((base) + 0xF98)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_1_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_1_ADDR(base)                  ((base) + 0xFA0)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_2_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_2_ADDR(base)                  ((base) + 0xFA4)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_3_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_3_ADDR(base)                  ((base) + 0xFA8)

/* Register description: 系统状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE_ARB_STAT_4_UNION */
#define SOC_SCTRL_DOZE_ARB_STAT_4_ADDR(base)                  ((base) + 0xFAC)

/* Register description: SYSBUS低功耗控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYSBUS_PWRD_CTRL_UNION */
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_ADDR(base)                 ((base) + 0xFB8)

/* Register description: 时钟切换控制
   Bit domain definition UNION:  SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_UNION */
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_ADDR(base)              ((base) + 0xF50)

/* Register description: TCXO控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_TCXO_CTRL_UNION */
#define SOC_SCTRL_SC_TCXO_CTRL_ADDR(base)                     ((base) + 0xF08)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_STAT_UNION */
#define SOC_SCTRL_SC_PERI_POWER_STAT_ADDR(base)               ((base) + 0xF58)

/* Register description: PERI_ISO BP寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERIISOBYPASS_UNION */
#define SOC_SCTRL_SCPERIISOBYPASS_ADDR(base)                  ((base) + 0xF5C)

/* Register description: DOZE2状态回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_DOZE2_FLAG_STAT_UNION */
#define SOC_SCTRL_DOZE2_FLAG_STAT_ADDR(base)                  ((base) + 0xE0C)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_CTRL_0_UNION */
#define SOC_SCTRL_SC_PERI_POWER_CTRL_0_ADDR(base)             ((base) + 0xF70)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_CTRL_1_UNION */
#define SOC_SCTRL_SC_PERI_POWER_CTRL_1_ADDR(base)             ((base) + 0xF74)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_PERI_POWER_CTRL_2_UNION */
#define SOC_SCTRL_SC_PERI_POWER_CTRL_2_ADDR(base)             ((base) + 0xF78)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_UNION */
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_ADDR(base)          ((base) + 0xF80)

/* Register description: 用于指示系统曾经进入过低功耗的状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_STAT_UNION */
#define SOC_SCTRL_SC_LP_STAT_ADDR(base)                       ((base) + 0x008)

/* Register description: 用于清除指示系统曾经进入过低功耗的状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_STAT_CLR_UNION */
#define SOC_SCTRL_SC_LP_STAT_CLR_ADDR(base)                   ((base) + 0x01C)

/* Register description: 状态记录寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_FLAG_UNION */
#define SOC_SCTRL_SC_LP_FLAG_ADDR(base)                       ((base) + 0x018)

/* Register description: 用于指示系统曾经进入过深度睡眠的状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_INTR_WAKEUP_CTRL_UNION */
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_ADDR(base)              ((base) + 0x014)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA2_UNION */
#define SOC_SCTRL_SCEFUSEDATA2_ADDR(base)                     ((base) + 0x00C)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA3_UNION */
#define SOC_SCTRL_SCEFUSEDATA3_ADDR(base)                     ((base) + 0x010)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA8_UNION */
#define SOC_SCTRL_SCEFUSEDATA8_ADDR(base)                     ((base) + 0x060)

/* Register description: EFUSEDATA寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSEDATA9_UNION */
#define SOC_SCTRL_SCEFUSEDATA9_ADDR(base)                     ((base) + 0x064)

/* Register description: 系统低功耗软件寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_POR_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_POR_ADDR(base)             ((base) + 0xE50)

/* Register description: 系统低功耗软件安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_ADDR(base)           ((base) + 0xE54)

/* Register description: 系统低功耗软件安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_ADDR(base)           ((base) + 0xE58)

/* Register description: 系统低功耗软件非安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_ADDR(base)         ((base) + 0x024)

/* Register description: 系统低功耗软件非安全寄存器
   Bit domain definition UNION:  SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_UNION */
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_ADDR(base)         ((base) + 0x034)

/* Register description: SC数据记录寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_LP_DATA_UNION */
#define SOC_SCTRL_SC_LP_DATA_ADDR(base)                       ((base) + 0x020)

/* Register description: 低功耗配置状态
   Bit domain definition UNION:  SOC_SCTRL_SCLPSTATCFG_UNION */
#define SOC_SCTRL_SCLPSTATCFG_ADDR(base)                      ((base) + 0x038)

/* Register description: ASP上下电配置寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SC_ASP_PWRCFG0_UNION */
#define SOC_SCTRL_SC_ASP_PWRCFG0_ADDR(base)                   ((base) + 0x080)

/* Register description: ASP上下电配置寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SC_ASP_PWRCFG1_UNION */
#define SOC_SCTRL_SC_ASP_PWRCFG1_ADDR(base)                   ((base) + 0x084)

/* Register description: 聚合后的唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_GATHER_STAT_UNION */
#define SOC_SCTRL_SCINT_GATHER_STAT_ADDR(base)                ((base) + 0x0A0)

/* Register description: intr_ao_wakeup0唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK_UNION */
#define SOC_SCTRL_SCINT_MASK_ADDR(base)                       ((base) + 0x0A4)

/* Register description: intr_ao_wakeup0原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT_UNION */
#define SOC_SCTRL_SCINT_STAT_ADDR(base)                       ((base) + 0x0A8)

/* Register description: DRX唤醒中断分配配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCDRX_INT_CFG_UNION */
#define SOC_SCTRL_SCDRX_INT_CFG_ADDR(base)                    ((base) + 0x0AC)

/* Register description: LPMCU WFI唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUWFI_INT_UNION */
#define SOC_SCTRL_SCLPMCUWFI_INT_ADDR(base)                   ((base) + 0x0B0)

/* Register description: intr_ao_wakeup1唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK1_UNION */
#define SOC_SCTRL_SCINT_MASK1_ADDR(base)                      ((base) + 0x0B4)

/* Register description: intr_ao_wakeup1原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT1_UNION */
#define SOC_SCTRL_SCINT_STAT1_ADDR(base)                      ((base) + 0x0B8)

/* Register description: intr_peri_wakeup唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK2_UNION */
#define SOC_SCTRL_SCINT_MASK2_ADDR(base)                      ((base) + 0x0BC)

/* Register description: intr_peri_wakeup原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT2_UNION */
#define SOC_SCTRL_SCINT_STAT2_ADDR(base)                      ((base) + 0x0C0)

/* Register description: intr_peri_wakeup0唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK3_UNION */
#define SOC_SCTRL_SCINT_MASK3_ADDR(base)                      ((base) + 0x0C4)

/* Register description: intr_peri_wakeup0原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT3_UNION */
#define SOC_SCTRL_SCINT_STAT3_ADDR(base)                      ((base) + 0x0C8)

/* Register description: intr_drx_wakeup唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK4_UNION */
#define SOC_SCTRL_SCINT_MASK4_ADDR(base)                      ((base) + 0x090)

/* Register description: intr_drx_wakeup原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT4_UNION */
#define SOC_SCTRL_SCINT_STAT4_ADDR(base)                      ((base) + 0x094)

/* Register description: intr_ao_wakeup2唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_MASK5_UNION */
#define SOC_SCTRL_SCINT_MASK5_ADDR(base)                      ((base) + 0x098)

/* Register description: intr_ao_wakeup2原始唤醒中断状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_STAT5_UNION */
#define SOC_SCTRL_SCINT_STAT5_ADDR(base)                      ((base) + 0x09C)

/* Register description: 唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_INT_STD2LS_MASK_UNION */
#define SOC_SCTRL_SCINT_INT_STD2LS_MASK_ADDR(base)            ((base) + 0x0D0)

/* Register description: 唤醒中断屏蔽配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINT_INT_LS2STD_MASK_UNION */
#define SOC_SCTRL_SCINT_INT_LS2STD_MASK_ADDR(base)            ((base) + 0x0D4)

/* Register description: 外设控制寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL0_UNION */
#define SOC_SCTRL_SCPERCTRL0_ADDR(base)                       ((base) + 0x300)

/* Register description: 外设控制寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL1_UNION */
#define SOC_SCTRL_SCPERCTRL1_ADDR(base)                       ((base) + 0x304)

/* Register description: 外设控制寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL2_UNION */
#define SOC_SCTRL_SCPERCTRL2_ADDR(base)                       ((base) + 0x308)

/* Register description: 外设控制寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL3_UNION */
#define SOC_SCTRL_SCPERCTRL3_ADDR(base)                       ((base) + 0x30C)

/* Register description: 外设控制寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL4_UNION */
#define SOC_SCTRL_SCPERCTRL4_ADDR(base)                       ((base) + 0x310)

/* Register description: 外设控制寄存器5
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL5_UNION */
#define SOC_SCTRL_SCPERCTRL5_ADDR(base)                       ((base) + 0x314)

/* Register description: 外设控制寄存器6
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL6_UNION */
#define SOC_SCTRL_SCPERCTRL6_ADDR(base)                       ((base) + 0x318)

/* Register description: 外设控制寄存器7
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL7_UNION */
#define SOC_SCTRL_SCPERCTRL7_ADDR(base)                       ((base) + 0x31C)

/* Register description: 外设控制寄存器9
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL9_UNION */
#define SOC_SCTRL_SCPERCTRL9_ADDR(base)                       ((base) + 0x324)

/* Register description: 外设控制寄存器10
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL10_UNION */
#define SOC_SCTRL_SCPERCTRL10_ADDR(base)                      ((base) + 0x328)

/* Register description: 外设控制寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL11_UNION */
#define SOC_SCTRL_SCPERCTRL11_ADDR(base)                      ((base) + 0x32C)

/* Register description: 外设控制寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL12_UNION */
#define SOC_SCTRL_SCPERCTRL12_ADDR(base)                      ((base) + 0x330)

/* Register description: 外设控制寄存器13
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL13_UNION */
#define SOC_SCTRL_SCPERCTRL13_ADDR(base)                      ((base) + 0x334)

/* Register description: 外设控制寄存器14
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL14_UNION */
#define SOC_SCTRL_SCPERCTRL14_ADDR(base)                      ((base) + 0x338)

/* Register description: 外设控制寄存器15
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL15_UNION */
#define SOC_SCTRL_SCPERCTRL15_ADDR(base)                      ((base) + 0x33C)

/* Register description: 外设控制寄存器8，安全寄存器，控制信号由SCTRL内部逻辑产生的状态输出控制
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL16_UNION */
#define SOC_SCTRL_SCPERCTRL16_ADDR(base)                      ((base) + 0x340)

/* Register description: 外设控制寄存器8，安全寄存器，控制信号由SCTRL内部逻辑产生的状态输出控制
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL17_UNION */
#define SOC_SCTRL_SCPERCTRL17_ADDR(base)                      ((base) + 0x344)

/* Register description: 外设状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS0_UNION */
#define SOC_SCTRL_SCPERSTATUS0_ADDR(base)                     ((base) + 0x360)

/* Register description: 外设状态寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS1_UNION */
#define SOC_SCTRL_SCPERSTATUS1_ADDR(base)                     ((base) + 0x364)

/* Register description: 外设状态寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS2_UNION */
#define SOC_SCTRL_SCPERSTATUS2_ADDR(base)                     ((base) + 0x368)

/* Register description: 外设状态寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS3_UNION */
#define SOC_SCTRL_SCPERSTATUS3_ADDR(base)                     ((base) + 0x36C)

/* Register description: 外设状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS4_UNION */
#define SOC_SCTRL_SCPERSTATUS4_ADDR(base)                     ((base) + 0x370)

/* Register description: 外设状态寄存器5
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS5_UNION */
#define SOC_SCTRL_SCPERSTATUS5_ADDR(base)                     ((base) + 0x374)

/* Register description: 外设状态寄存器6
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS6_UNION */
#define SOC_SCTRL_SCPERSTATUS6_ADDR(base)                     ((base) + 0x378)

/* Register description: 外设状态寄存器7
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS7_UNION */
#define SOC_SCTRL_SCPERSTATUS7_ADDR(base)                     ((base) + 0x37C)

/* Register description: 外设状态寄存器8
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS8_UNION */
#define SOC_SCTRL_SCPERSTATUS8_ADDR(base)                     ((base) + 0x380)

/* Register description: 外设状态寄存器9
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS9_UNION */
#define SOC_SCTRL_SCPERSTATUS9_ADDR(base)                     ((base) + 0x384)

/* Register description: 外设状态寄存器10
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS10_UNION */
#define SOC_SCTRL_SCPERSTATUS10_ADDR(base)                    ((base) + 0x388)

/* Register description: 外设状态寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS11_UNION */
#define SOC_SCTRL_SCPERSTATUS11_ADDR(base)                    ((base) + 0x38C)

/* Register description: 外设状态寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS12_UNION */
#define SOC_SCTRL_SCPERSTATUS12_ADDR(base)                    ((base) + 0x390)

/* Register description: 外设状态寄存器13
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS13_UNION */
#define SOC_SCTRL_SCPERSTATUS13_ADDR(base)                    ((base) + 0x394)

/* Register description: 外设状态寄存器14
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS14_UNION */
#define SOC_SCTRL_SCPERSTATUS14_ADDR(base)                    ((base) + 0x398)

/* Register description: 外设状态寄存器15
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS15_UNION */
#define SOC_SCTRL_SCPERSTATUS15_ADDR(base)                    ((base) + 0x39C)

/* Register description: 内部状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINNERSTAT_UNION */
#define SOC_SCTRL_SCINNERSTAT_ADDR(base)                      ((base) + 0x3A0)

/* Register description: 内部使用的配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCINNERCTRL_UNION */
#define SOC_SCTRL_SCINNERCTRL_ADDR(base)                      ((base) + 0x3A4)

/* Register description: MRB EFUSE BUSY状态指示寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMRBBUSYSTAT_UNION */
#define SOC_SCTRL_SCMRBBUSYSTAT_ADDR(base)                    ((base) + 0x3FC)

/* Register description: 用于保存软件入口地址的寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSWADDR_UNION */
#define SOC_SCTRL_SCSWADDR_ADDR(base)                         ((base) + 0x400)

/* Register description: 用于保留DDR训练数据的地址的寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCDDRADDR_UNION */
#define SOC_SCTRL_SCDDRADDR_ADDR(base)                        ((base) + 0x404)

/* Register description: 用于保存DDR训练数据的寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCDDRDATA_UNION */
#define SOC_SCTRL_SCDDRDATA_ADDR(base)                        ((base) + 0x408)

/* Register description: 用于软件使用的存储数据寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA0_UNION */
#define SOC_SCTRL_SCBAKDATA0_ADDR(base)                       ((base) + 0x40C)

/* Register description: 用于软件使用的存储数据寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA1_UNION */
#define SOC_SCTRL_SCBAKDATA1_ADDR(base)                       ((base) + 0x410)

/* Register description: 用于软件使用的存储数据寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA2_UNION */
#define SOC_SCTRL_SCBAKDATA2_ADDR(base)                       ((base) + 0x414)

/* Register description: 用于软件使用的存储数据寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA3_UNION */
#define SOC_SCTRL_SCBAKDATA3_ADDR(base)                       ((base) + 0x418)

/* Register description: 用于软件使用的存储数据寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA4_UNION */
#define SOC_SCTRL_SCBAKDATA4_ADDR(base)                       ((base) + 0x41C)

/* Register description: 用于软件使用的存储数据寄存器5
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA5_UNION */
#define SOC_SCTRL_SCBAKDATA5_ADDR(base)                       ((base) + 0x420)

/* Register description: 用于软件使用的存储数据寄存器6
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA6_UNION */
#define SOC_SCTRL_SCBAKDATA6_ADDR(base)                       ((base) + 0x424)

/* Register description: 用于软件使用的存储数据寄存器7
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA7_UNION */
#define SOC_SCTRL_SCBAKDATA7_ADDR(base)                       ((base) + 0x428)

/* Register description: 用于软件使用的存储数据寄存器8
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA8_UNION */
#define SOC_SCTRL_SCBAKDATA8_ADDR(base)                       ((base) + 0x42C)

/* Register description: 用于软件使用的存储数据寄存器9
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA9_UNION */
#define SOC_SCTRL_SCBAKDATA9_ADDR(base)                       ((base) + 0x430)

/* Register description: 用于软件使用的存储数据寄存器10
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA10_UNION */
#define SOC_SCTRL_SCBAKDATA10_ADDR(base)                      ((base) + 0x434)

/* Register description: 用于软件使用的存储数据寄存器11
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA11_UNION */
#define SOC_SCTRL_SCBAKDATA11_ADDR(base)                      ((base) + 0x438)

/* Register description: 用于软件使用的存储数据寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA12_UNION */
#define SOC_SCTRL_SCBAKDATA12_ADDR(base)                      ((base) + 0x43C)

/* Register description: 用于软件使用的存储数据寄存器13
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA13_UNION */
#define SOC_SCTRL_SCBAKDATA13_ADDR(base)                      ((base) + 0x440)

/* Register description: 用于软件使用的存储数据寄存器14
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA14_UNION */
#define SOC_SCTRL_SCBAKDATA14_ADDR(base)                      ((base) + 0x444)

/* Register description: 用于软件使用的存储数据寄存器15
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA15_UNION */
#define SOC_SCTRL_SCBAKDATA15_ADDR(base)                      ((base) + 0x448)

/* Register description: 用于软件使用的存储数据寄存器16
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA16_UNION */
#define SOC_SCTRL_SCBAKDATA16_ADDR(base)                      ((base) + 0x44C)

/* Register description: 用于软件使用的存储数据寄存器17
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA17_UNION */
#define SOC_SCTRL_SCBAKDATA17_ADDR(base)                      ((base) + 0x450)

/* Register description: 用于软件使用的存储数据寄存器18
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA18_UNION */
#define SOC_SCTRL_SCBAKDATA18_ADDR(base)                      ((base) + 0x454)

/* Register description: 用于软件使用的存储数据寄存器19
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA19_UNION */
#define SOC_SCTRL_SCBAKDATA19_ADDR(base)                      ((base) + 0x458)

/* Register description: 用于软件使用的存储数据寄存器20
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA20_UNION */
#define SOC_SCTRL_SCBAKDATA20_ADDR(base)                      ((base) + 0x45C)

/* Register description: 用于软件使用的存储数据寄存器21
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA21_UNION */
#define SOC_SCTRL_SCBAKDATA21_ADDR(base)                      ((base) + 0x460)

/* Register description: 用于软件使用的存储数据寄存器22
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA22_UNION */
#define SOC_SCTRL_SCBAKDATA22_ADDR(base)                      ((base) + 0x464)

/* Register description: 用于软件使用的存储数据寄存器23
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA23_UNION */
#define SOC_SCTRL_SCBAKDATA23_ADDR(base)                      ((base) + 0x468)

/* Register description: 用于软件使用的存储数据寄存器24
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA24_UNION */
#define SOC_SCTRL_SCBAKDATA24_ADDR(base)                      ((base) + 0x46C)

/* Register description: 用于软件使用的存储数据寄存器25
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA25_UNION */
#define SOC_SCTRL_SCBAKDATA25_ADDR(base)                      ((base) + 0x470)

/* Register description: 用于软件使用的存储数据寄存器26
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA26_UNION */
#define SOC_SCTRL_SCBAKDATA26_ADDR(base)                      ((base) + 0x474)

/* Register description: 用于软件使用的存储数据寄存器27
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA27_UNION */
#define SOC_SCTRL_SCBAKDATA27_ADDR(base)                      ((base) + 0x478)

/* Register description: 用于软件使用的存储数据寄存器28
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA28_UNION */
#define SOC_SCTRL_SCBAKDATA28_ADDR(base)                      ((base) + 0x47C)

/* Register description: 外设状态非复位保留寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS0_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS0_ADDR(base)       ((base) + 0x480)

/* Register description: 外设状态非复位保留寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS1_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS1_ADDR(base)       ((base) + 0x484)

/* Register description: 外设状态非复位保留寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS2_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS2_ADDR(base)       ((base) + 0x488)

/* Register description: 外设状态非复位保留寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS3_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS3_ADDR(base)       ((base) + 0x48C)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA0_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA0_MSK_ADDR(base)                   ((base) + 0x4C0)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA1_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA1_MSK_ADDR(base)                   ((base) + 0x4C4)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA2_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA2_MSK_ADDR(base)                   ((base) + 0x4C8)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA3_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA3_MSK_ADDR(base)                   ((base) + 0x4CC)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA4_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA4_MSK_ADDR(base)                   ((base) + 0x4D0)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA5_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA5_MSK_ADDR(base)                   ((base) + 0x4D4)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA6_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA6_MSK_ADDR(base)                   ((base) + 0x4D8)

/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA7_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA7_MSK_ADDR(base)                   ((base) + 0x4DC)

/* Register description: LPMCU子系统时钟使能寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCLKEN_UNION */
#define SOC_SCTRL_SCLPMCUCLKEN_ADDR(base)                     ((base) + 0x960)

/* Register description: LPMCU子系统软时钟关闭寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCLKDIS_UNION */
#define SOC_SCTRL_SCLPMCUCLKDIS_ADDR(base)                    ((base) + 0x964)

/* Register description: LPMCU子系统软时钟状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCLKSTAT_UNION */
#define SOC_SCTRL_SCLPMCUCLKSTAT_ADDR(base)                   ((base) + 0x968)

/* Register description: LPMCU子系统软复位使能寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURSTEN_UNION */
#define SOC_SCTRL_SCLPMCURSTEN_ADDR(base)                     ((base) + 0x970)

/* Register description: LPMCU子系统软复位撤离寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURSTDIS_UNION */
#define SOC_SCTRL_SCLPMCURSTDIS_ADDR(base)                    ((base) + 0x974)

/* Register description: LPMCU子系统软复位使能状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURSTSTAT_UNION */
#define SOC_SCTRL_SCLPMCURSTSTAT_ADDR(base)                   ((base) + 0x978)

/* Register description: LPMCU子系统控制配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUCTRL_UNION */
#define SOC_SCTRL_SCLPMCUCTRL_ADDR(base)                      ((base) + 0x980)

/* Register description: LPMCU子系统状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCUSTAT_UNION */
#define SOC_SCTRL_SCLPMCUSTAT_ADDR(base)                      ((base) + 0x984)

/* Register description: LPMCU RAM 控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCURAMCTRL_UNION */
#define SOC_SCTRL_SCLPMCURAMCTRL_ADDR(base)                   ((base) + 0x988)

/* Register description: BBPDRX子系统状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT0_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT0_ADDR(base)                    ((base) + 0x530)

/* Register description: BBPDRX子系统状态寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT1_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT1_ADDR(base)                    ((base) + 0x534)

/* Register description: BBPDRX子系统状态寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT2_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT2_ADDR(base)                    ((base) + 0x538)

/* Register description: BBPDRX子系统状态寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT3_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT3_ADDR(base)                    ((base) + 0x53C)

/* Register description: BBPDRX子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCBBPDRXSTAT4_UNION */
#define SOC_SCTRL_SCBBPDRXSTAT4_ADDR(base)                    ((base) + 0x540)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCA53_EVENT_MASK_UNION */
#define SOC_SCTRL_SCA53_EVENT_MASK_ADDR(base)                 ((base) + 0x550)

/* Register description: A57 EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCA57_EVENT_MASK_UNION */
#define SOC_SCTRL_SCA57_EVENT_MASK_ADDR(base)                 ((base) + 0x554)

/* Register description: IOMCU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_ADDR(base)               ((base) + 0x558)

/* Register description: LPMCU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCLPMCU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_ADDR(base)               ((base) + 0x55C)

/* Register description: MCPU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMCPU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCMCPU_EVENT_MASK_ADDR(base)                ((base) + 0x560)

/* Register description: MCPU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCISPA7_EVENT_MASK_UNION */
#define SOC_SCTRL_SCISPA7_EVENT_MASK_ADDR(base)               ((base) + 0x568)

/* Register description: MCPU EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCHIFD_UL_EVENT_MASK_UNION */
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_ADDR(base)             ((base) + 0x56C)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_ADDR(base)           ((base) + 0x570)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_UNION */
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_ADDR(base)           ((base) + 0x574)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_UNION */
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_ADDR(base)           ((base) + 0x578)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSILS_EVENT_MASK_UNION */
#define SOC_SCTRL_SCSILS_EVENT_MASK_ADDR(base)                ((base) + 0x97C)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_UNION */
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_ADDR(base)         ((base) + 0x544)

/* Register description: EVENT屏蔽寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_UNION */
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_ADDR(base)        ((base) + 0x548)

/* Register description: EVENT状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEVENT_STAT_UNION */
#define SOC_SCTRL_SCEVENT_STAT_ADDR(base)                     ((base) + 0x564)

/* Register description: IOMCU时钟控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUCLKCTRL_UNION */
#define SOC_SCTRL_SCIOMCUCLKCTRL_ADDR(base)                   ((base) + 0x880)

/* Register description: IOMCU时钟控制信号状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUCLKSTAT_UNION */
#define SOC_SCTRL_SCIOMCUCLKSTAT_ADDR(base)                   ((base) + 0x584)

/* Register description: IOMCU子系统控制配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUCTRL_UNION */
#define SOC_SCTRL_SCIOMCUCTRL_ADDR(base)                      ((base) + 0x588)

/* Register description: IOMCU子系统状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCIOMCUSTAT_UNION */
#define SOC_SCTRL_SCIOMCUSTAT_ADDR(base)                      ((base) + 0x58C)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_ADDR(base)              ((base) + 0x500)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_ADDR(base)            ((base) + 0x504)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_ADDR(base)             ((base) + 0x508)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_ADDR(base)              ((base) + 0x50C)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_ADDR(base)            ((base) + 0x510)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_ADDR(base)             ((base) + 0x514)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR0_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR0_NONSEC_ADDR(base)                ((base) + 0x700)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR1_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR1_NONSEC_ADDR(base)                ((base) + 0x704)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR2_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR2_NONSEC_ADDR(base)                ((base) + 0x708)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SCMDMADDR3_NONSEC_UNION */
#define SOC_SCTRL_SCMDMADDR3_NONSEC_ADDR(base)                ((base) + 0x70C)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_SEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_ADDR(base)                ((base) + 0x8E8)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_ADDR(base)              ((base) + 0x8EC)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_SEC_0_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_ADDR(base)               ((base) + 0x8F0)

/* Register description: 通用资源锁定寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_CFG_SEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_ADDR(base)                ((base) + 0x8F4)

/* Register description: 通用资源解锁寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_UNION */
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_ADDR(base)              ((base) + 0x8F8)

/* Register description: 通用资源锁状态寄存器。
   Bit domain definition UNION:  SOC_SCTRL_SC_LOCK_STAT_SEC_1_UNION */
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_ADDR(base)               ((base) + 0x8FC)

/* Register description: JTAG选择控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAG_SEL_UNION */
#define SOC_SCTRL_SCJTAG_SEL_ADDR(base)                       ((base) + 0x800)

/* Register description: DJTAG控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCFG_DJTAG_UNION */
#define SOC_SCTRL_SCCFG_DJTAG_ADDR(base)                      ((base) + 0x814)

/* Register description: CP15DISABLE控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCP15_DISABLE_UNION */
#define SOC_SCTRL_SCCP15_DISABLE_ADDR(base)                   ((base) + 0x818)

/* Register description: ARM debug en配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCFG_ARM_DBGEN_UNION */
#define SOC_SCTRL_SCCFG_ARM_DBGEN_ADDR(base)                  ((base) + 0x82C)

/* Register description: ARM debug en状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBGEN_STAT_UNION */
#define SOC_SCTRL_SCARM_DBGEN_STAT_ADDR(base)                 ((base) + 0x838)

/* Register description: EFUSE PAD烧写控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSECTRL_UNION */
#define SOC_SCTRL_SCEFUSECTRL_ADDR(base)                      ((base) + 0x83C)

/* Register description: SECS时钟门控寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCEFUSESEL_UNION */
#define SOC_SCTRL_SCEFUSESEL_ADDR(base)                       ((base) + 0x840)

/* Register description: EFUSE CHIP_ID0寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCHIP_ID0_UNION */
#define SOC_SCTRL_SCCHIP_ID0_ADDR(base)                       ((base) + 0x848)

/* Register description: EFUSE CHIP_ID1寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCHIP_ID1_UNION */
#define SOC_SCTRL_SCCHIP_ID1_ADDR(base)                       ((base) + 0x84C)

/* Register description: CPU安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCCPUSECCTRL_UNION */
#define SOC_SCTRL_SCCPUSECCTRL_ADDR(base)                     ((base) + 0x850)

/* Register description: ISP安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCISPSECCTRL_UNION */
#define SOC_SCTRL_SCISPSECCTRL_ADDR(base)                     ((base) + 0x830)

/* Register description: JTAG_SD SWITCH选择寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAGSD_SW_SEL_UNION */
#define SOC_SCTRL_SCJTAGSD_SW_SEL_ADDR(base)                  ((base) + 0x854)

/* Register description: JTAG_SYS SWITCH选择寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAGSYS_SW_SEL_UNION */
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_ADDR(base)                 ((base) + 0x858)

/* Register description: JTAG回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAG_SD_SWD2jtag_dfx_UNION */
#define SOC_SCTRL_SCJTAG_SD_SWD2jtag_dfx_ADDR(base)           ((base) + 0x870)

/* Register description: JTAG回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCJTAG_SYS_SWD2jtag_dfx_UNION */
#define SOC_SCTRL_SCJTAG_SYS_SWD2jtag_dfx_ADDR(base)          ((base) + 0x874)

/* Register description: syscache中断控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_INTR_SYSCACHE_SEC_UNION */
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_ADDR(base)             ((base) + 0x85C)

/* Register description: ARM debug key0配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY0_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY0_ADDR(base)                   ((base) + 0x860)

/* Register description: ARM debug key1配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY1_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY1_ADDR(base)                   ((base) + 0x864)

/* Register description: ARM debug key2配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY2_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY2_ADDR(base)                   ((base) + 0x868)

/* Register description: ARM debug key3配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY3_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY3_ADDR(base)                   ((base) + 0x86C)

/* Register description: ARM debug key4配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY4_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY4_ADDR(base)                   ((base) + 0x884)

/* Register description: ARM debug key5配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY5_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY5_ADDR(base)                   ((base) + 0x888)

/* Register description: ARM debug key6配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY6_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY6_ADDR(base)                   ((base) + 0x88C)

/* Register description: ARM debug key7配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCARM_DBG_KEY7_UNION */
#define SOC_SCTRL_SCARM_DBG_KEY7_ADDR(base)                   ((base) + 0x890)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR0_UNION */
#define SOC_SCTRL_SCSPMIADDR0_ADDR(base)                      ((base) + 0x920)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR1_UNION */
#define SOC_SCTRL_SCSPMIADDR1_ADDR(base)                      ((base) + 0x924)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR2_UNION */
#define SOC_SCTRL_SCSPMIADDR2_ADDR(base)                      ((base) + 0x928)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR3_UNION */
#define SOC_SCTRL_SCSPMIADDR3_ADDR(base)                      ((base) + 0x92C)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR4_UNION */
#define SOC_SCTRL_SCSPMIADDR4_ADDR(base)                      ((base) + 0x930)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR5_UNION */
#define SOC_SCTRL_SCSPMIADDR5_ADDR(base)                      ((base) + 0x934)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR6_UNION */
#define SOC_SCTRL_SCSPMIADDR6_ADDR(base)                      ((base) + 0x938)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR7_UNION */
#define SOC_SCTRL_SCSPMIADDR7_ADDR(base)                      ((base) + 0x93C)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR8_UNION */
#define SOC_SCTRL_SCSPMIADDR8_ADDR(base)                      ((base) + 0x940)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR9_UNION */
#define SOC_SCTRL_SCSPMIADDR9_ADDR(base)                      ((base) + 0x944)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR10_UNION */
#define SOC_SCTRL_SCSPMIADDR10_ADDR(base)                     ((base) + 0x948)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR11_UNION */
#define SOC_SCTRL_SCSPMIADDR11_ADDR(base)                     ((base) + 0x94C)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR12_UNION */
#define SOC_SCTRL_SCSPMIADDR12_ADDR(base)                     ((base) + 0x950)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR13_UNION */
#define SOC_SCTRL_SCSPMIADDR13_ADDR(base)                     ((base) + 0x954)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR14_UNION */
#define SOC_SCTRL_SCSPMIADDR14_ADDR(base)                     ((base) + 0x958)

/* Register description: SPMI地址配置寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCSPMIADDR15_UNION */
#define SOC_SCTRL_SCSPMIADDR15_ADDR(base)                     ((base) + 0x95C)

/* Register description: HISEE子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCHISEEGPIOLOCK_UNION */
#define SOC_SCTRL_SCHISEEGPIOLOCK_ADDR(base)                  ((base) + 0x894)

/* Register description: HISEE子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCHISEESPILOCK_UNION */
#define SOC_SCTRL_SCHISEESPILOCK_ADDR(base)                   ((base) + 0x898)

/* Register description: HISEE子系统状态寄存器4
   Bit domain definition UNION:  SOC_SCTRL_SCHISEEI2CLOCK_UNION */
#define SOC_SCTRL_SCHISEEI2CLOCK_ADDR(base)                   ((base) + 0x89C)

/* Register description: HISEE IO选择回读寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCHISEEIOSEL_UNION */
#define SOC_SCTRL_SCHISEEIOSEL_ADDR(base)                     ((base) + 0x8A0)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL0_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL0_SEC_ADDR(base)                   ((base) + 0x8A4)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL1_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL1_SEC_ADDR(base)                   ((base) + 0x8B0)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL2_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL2_SEC_ADDR(base)                   ((base) + 0x8B4)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL3_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL3_SEC_ADDR(base)                   ((base) + 0x8B8)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL4_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL4_SEC_ADDR(base)                   ((base) + 0x8BC)

/* Register description: SC安全控制寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERCTRL5_SEC_UNION */
#define SOC_SCTRL_SCPERCTRL5_SEC_ADDR(base)                   ((base) + 0x8E0)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS0_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS0_SEC_ADDR(base)                 ((base) + 0x8C0)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS1_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS1_SEC_ADDR(base)                 ((base) + 0x8C4)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS2_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS2_SEC_ADDR(base)                 ((base) + 0x8C8)

/* Register description: SC安全状态寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTATUS3_SEC_UNION */
#define SOC_SCTRL_SCPERSTATUS3_SEC_ADDR(base)                 ((base) + 0x8CC)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ_0_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_ADDR(base)                ((base) + 0x8D0)

/* Register description: SC_TCP_VOTE_STAT
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_STAT_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_STAT_ADDR(base)                 ((base) + 0x8D4)

/* Register description: SC_TCP_VOTE_CTRL
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_CTRL0_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_ADDR(base)                ((base) + 0x8D8)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ_1_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_ADDR(base)                ((base) + 0x8DC)

/* Register description: SC_TCP_VOTE_CTRL
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_CTRL1_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_ADDR(base)                ((base) + 0x8E4)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_ADDR(base)            ((base) + 0x808)

/* Register description: SC_TCP_VOTE_REQ
   Bit domain definition UNION:  SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_UNION */
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_ADDR(base)            ((base) + 0x80C)

/* Register description: SOCID寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCSOCID0_UNION */
#define SOC_SCTRL_SCSOCID0_ADDR(base)                         ((base) + 0xE00)

/* Register description: 外设状态非复位保留寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED0_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED0_ADDR(base)          ((base) + 0xE10)

/* Register description: 外设状态非复位保留寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED1_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED1_ADDR(base)          ((base) + 0xE14)

/* Register description: 外设状态非复位保留寄存器2
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED2_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED2_ADDR(base)          ((base) + 0xE18)

/* Register description: 外设状态非复位保留寄存器3
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT_POR_RESERVED3_UNION */
#define SOC_SCTRL_SCPERSTAT_POR_RESERVED3_ADDR(base)          ((base) + 0xE1C)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL0_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL0_SEC_ADDR(base)               ((base) + 0xE20)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL1_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL1_SEC_ADDR(base)               ((base) + 0xE24)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL2_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL2_SEC_ADDR(base)               ((base) + 0xE28)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL3_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL3_SEC_ADDR(base)               ((base) + 0xE2C)

/* Register description: 低功耗数据寄存器
   Bit domain definition UNION:  SOC_SCTRL_SC_NPU_LPCTRL4_SEC_UNION */
#define SOC_SCTRL_SC_NPU_LPCTRL4_SEC_ADDR(base)               ((base) + 0xE30)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG0_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG0_ADDR(base)                 ((base) + 0xE40)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG1_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG1_ADDR(base)                 ((base) + 0xE44)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG2_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG2_ADDR(base)                 ((base) + 0xE48)

/* Register description: 轨迹记录
   Bit domain definition UNION:  SOC_SCTRL_SC_TRACKING_LOG3_UNION */
#define SOC_SCTRL_SC_TRACKING_LOG3_ADDR(base)                 ((base) + 0xE4C)

/* Register description: 
   Bit domain definition UNION:  SOC_SCTRL_SC_EMU_VERSION_UNION */
#define SOC_SCTRL_SC_EMU_VERSION_ADDR(base)                   ((base) + 0xE34)


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
 struct               : SOC_SCTRL_SCCTRL_UNION
 struct description   : SCCTRL Register structure definition
                        Address Offset:0xF00 Initial:0x0 Width:32
 register description : 系统控制器寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mode_ctrl_soft : 4;  /* bit[0-3]  : 系统模式控制。这里定义了系统期望的工作模式。
                                                          0000：DEEPSLEEP；
                                                          0001：LIGHTSLEEP；
                                                          001X：STANDBY；
                                                          01XX：SLOW；
                                                          1XXX：NORMAL */
        unsigned int  reserved_0     : 3;  /* bit[4-6]  :  */
        unsigned int  reserved_1     : 1;  /* bit[7]    :  */
        unsigned int  sys_mode       : 4;  /* bit[8-11] : 指示系统控制器系统状态机当前工作模式。
                                                          0000：DEEPSLEEP；
                                                          0001：DOZE1；
                                                          0010：SLOW；
                                                          0011：DOZE2
                                                          0100: NORMAL
                                                          0110：STANDBY
                                                          0101：LIGHTSLEEP */
        unsigned int  reserved_2     : 13; /* bit[12-24]: reserved */
        unsigned int  reserved_3     : 1;  /* bit[25]   :  */
        unsigned int  reserved_4     : 2;  /* bit[26-27]:  */
        unsigned int  reserved_5     : 1;  /* bit[28]   :  */
        unsigned int  reserved_6     : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCCTRL_UNION;
#endif
#define SOC_SCTRL_SCCTRL_mode_ctrl_soft_START  (0)
#define SOC_SCTRL_SCCTRL_mode_ctrl_soft_END    (3)
#define SOC_SCTRL_SCCTRL_sys_mode_START        (8)
#define SOC_SCTRL_SCCTRL_sys_mode_END          (11)


/*****************************************************************************
 struct               : SOC_SCTRL_STD2LS_CTRL_UNION
 struct description   : STD2LS_CTRL Register structure definition
                        Address Offset:0xFB0 Initial:0x00000000 Width:32
 register description : 系统控制器寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lightsleep_to_standby_req : 1;  /* bit[0]   : 系统在LIGHTSLEEP状态时，此bit配置为1，系统状态切换到STANDBY状态。（如果此时有唤醒中断，优先响应唤醒中断，切换到SLOW状态） */
        unsigned int  standby_to_lightsleep_req : 1;  /* bit[1]   : 系统在STANDBY状态时，此bit配置为1，系统状态切换到LIGHTSLEEP状态。（如果此时有唤醒中断，优先响应唤醒中断，切换到SLOW状态） */
        unsigned int  reserved                  : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_SCTRL_STD2LS_CTRL_UNION;
#endif
#define SOC_SCTRL_STD2LS_CTRL_lightsleep_to_standby_req_START  (0)
#define SOC_SCTRL_STD2LS_CTRL_lightsleep_to_standby_req_END    (0)
#define SOC_SCTRL_STD2LS_CTRL_standby_to_lightsleep_req_START  (1)
#define SOC_SCTRL_STD2LS_CTRL_standby_to_lightsleep_req_END    (1)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_CTRL_UNION
 struct description   : DOZE_CTRL Register structure definition
                        Address Offset:0xFC8 Initial:0x00000000 Width:32
 register description : 系统控制器寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sw_doze1_req : 1;  /* bit[0]   : NORMAL状态时，此bit配置为1，切换DOZE1状态请求。 */
        unsigned int  sw_doze2_req : 1;  /* bit[1]   : NORMAL状态时，此bit配置为1，切换DOZE2状态请求。 */
        unsigned int  reserved     : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_SCTRL_DOZE_CTRL_UNION;
#endif
#define SOC_SCTRL_DOZE_CTRL_sw_doze1_req_START  (0)
#define SOC_SCTRL_DOZE_CTRL_sw_doze1_req_END    (0)
#define SOC_SCTRL_DOZE_CTRL_sw_doze2_req_START  (1)
#define SOC_SCTRL_DOZE_CTRL_sw_doze2_req_END    (1)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSYSSTAT_UNION
 struct description   : SCSYSSTAT Register structure definition
                        Address Offset:0xF04 Initial:0x0 Width:32
 register description : 系统状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 31; /* bit[0-30]: 读：回读无意义。相关状态寄存器参考AO_CRG.SC_RST_SRC寄存器 */
        unsigned int  reserved_1: 1;  /* bit[31]  : SYS_MISC有逻辑处理，注意修改
                                                    写：往该寄存器写入任意值会触发系统软复位。
                                                    读：回读无意义。相关状态寄存器参考AO_CRG.SC_RST_SRC寄存器 */
    } reg;
} SOC_SCTRL_SCSYSSTAT_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_0_UNION
 struct description   : SYS_STAT_CTRL_0 Register structure definition
                        Address Offset:0xF14 Initial:0x0 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                              : 1;  /* bit[0] :  */
        unsigned int  mfc_sw_sft_req                          : 1;  /* bit[1] : {hfc_sw_sft_req , mfc_sw_sft_req} bit[0]与bit[1]一起配置
                                                                                {0,0} ：19.2M分支，默认态
                                                                                {0,1} ：ULPPLL分支
                                                                                {1,0} ：PLL分支
                                                                                {1,1} ：forbidden */
        unsigned int  hfc_sw_sft_req                          : 1;  /* bit[2] : {hfc_sw_sft_req , mfc_sw_sft_req} bit[0]与bit[1]一起配置
                                                                                {0,0} ：19.2M分支，默认态
                                                                                {0,1} ：ULPPLL分支
                                                                                {1,0} ：PLL分支
                                                                                {1,1} ：forbidden */
        unsigned int  mfc_hfc_sw_sft_ctrl_en                  : 1;  /* bit[3] : 中频时钟和高频时钟软件控制使能。
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  pll_sft_en                              : 1;  /* bit[4] : 高频HFC时钟SPLL软件开关。
                                                                                1：软件开启PLL，
                                                                                0：硬件关闭PLL */
        unsigned int  hfc_pll_sft_ctrl_en                     : 1;  /* bit[5] : 高频SPLL开关软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  ulppll_fsm_hw_en_sft                    : 1;  /* bit[6] : ULPPLL软件使能。
                                                                                1：软件开启ULPPLL，
                                                                                0：硬件关闭ULPPLL */
        unsigned int  ulppll_fsm_hw_en_sft_ctrl_en            : 1;  /* bit[7] : ULPPLL开关软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  reserved_1                              : 1;  /* bit[8] :  */
        unsigned int  bus_pwrd_sft_req                        : 1;  /* bit[9] : bus的低功耗软件请求信号。
                                                                                0：请求退出低功耗模式；
                                                                                1：请求进入低功耗模式。 */
        unsigned int  bus_idle_sft_ctrl_en                    : 1;  /* bit[10]: bus进退低功耗软件控制使能。
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  reserved_2                              : 1;  /* bit[11]:  */
        unsigned int  sys_idle_gt_flag_sft_ctrl               : 1;  /* bit[12]: ao->peri时钟软门控。
                                                                                1：开钟
                                                                                0：关钟 */
        unsigned int  ao2peri_clk_gt_sft_ctrl_en              : 1;  /* bit[13]: ao->peri时钟门控软件控制使能。
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  sys_idle_gt_fll_src_peri_sft            : 1;  /* bit[14]: ao->peri FLL时钟软门控。
                                                                                1：开钟
                                                                                0：关钟 */
        unsigned int  ao2peri_clk_gt_fll_src_peri_sft_ctrl_en : 1;  /* bit[15]: ao->peri FLL时钟门控软件控制使能。
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  reserved_3                              : 1;  /* bit[16]:  */
        unsigned int  peri_pwr_up_dn_sft_ctrl                 : 1;  /* bit[17]: peri上下电软件控制 */
        unsigned int  peri_pwr_pdc_sft_ctrl_en                : 1;  /* bit[18]: peri上下电软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  reserved_4                              : 1;  /* bit[19]:  */
        unsigned int  reserved_5                              : 1;  /* bit[20]:  */
        unsigned int  sc_sclk_sw2fll_sft_req                  : 1;  /* bit[21]: sclk的时钟切换软件控制
                                                                                1：切换到FLL时钟通路
                                                                                0：切换到32K时钟通路 */
        unsigned int  sclk_sw2fll_sft_ctrl_en                 : 1;  /* bit[22]: sclk的时钟切换软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  reserved_6                              : 1;  /* bit[23]:  */
        unsigned int  reserved_7                              : 1;  /* bit[24]:  */
        unsigned int  bus_clk_sw_sft_inuse                    : 1;  /* bit[25]: aobus的时钟切换软件控制
                                                                                1：切换到ULPPLL时钟通路
                                                                                0：与sysbus同频 */
        unsigned int  bus_clk_sw_sft_ctrl_en                  : 1;  /* bit[26]: aobus的时钟切换软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  reserved_8                              : 1;  /* bit[27]:  */
        unsigned int  sc_gt_clk_ulppll_sft                    : 1;  /* bit[28]: ULPPLL的时钟门控软件控制
                                                                                1：门控开；
                                                                                0：门控关 */
        unsigned int  gt_clk_ulppll_sft_ctrl_en               : 1;  /* bit[29]: ULPPLL的时钟门控软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
        unsigned int  mfc_clk_sft_gt                          : 1;  /* bit[30]: MFC CLK的时钟门控软件控制
                                                                                1：门控开；
                                                                                0：门控关 */
        unsigned int  mfc_clk_gt_sft_ctrl_en                  : 1;  /* bit[31]: MFC CLK的时钟门控软件控制使能
                                                                                1：使能；
                                                                                0：不使能 */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_sw_sft_req_START                           (1)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_sw_sft_req_END                             (1)
#define SOC_SCTRL_SYS_STAT_CTRL_0_hfc_sw_sft_req_START                           (2)
#define SOC_SCTRL_SYS_STAT_CTRL_0_hfc_sw_sft_req_END                             (2)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_hfc_sw_sft_ctrl_en_START                   (3)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_hfc_sw_sft_ctrl_en_END                     (3)
#define SOC_SCTRL_SYS_STAT_CTRL_0_pll_sft_en_START                               (4)
#define SOC_SCTRL_SYS_STAT_CTRL_0_pll_sft_en_END                                 (4)
#define SOC_SCTRL_SYS_STAT_CTRL_0_hfc_pll_sft_ctrl_en_START                      (5)
#define SOC_SCTRL_SYS_STAT_CTRL_0_hfc_pll_sft_ctrl_en_END                        (5)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ulppll_fsm_hw_en_sft_START                     (6)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ulppll_fsm_hw_en_sft_END                       (6)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ulppll_fsm_hw_en_sft_ctrl_en_START             (7)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ulppll_fsm_hw_en_sft_ctrl_en_END               (7)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_pwrd_sft_req_START                         (9)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_pwrd_sft_req_END                           (9)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_idle_sft_ctrl_en_START                     (10)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_idle_sft_ctrl_en_END                       (10)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sys_idle_gt_flag_sft_ctrl_START                (12)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sys_idle_gt_flag_sft_ctrl_END                  (12)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ao2peri_clk_gt_sft_ctrl_en_START               (13)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ao2peri_clk_gt_sft_ctrl_en_END                 (13)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sys_idle_gt_fll_src_peri_sft_START             (14)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sys_idle_gt_fll_src_peri_sft_END               (14)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ao2peri_clk_gt_fll_src_peri_sft_ctrl_en_START  (15)
#define SOC_SCTRL_SYS_STAT_CTRL_0_ao2peri_clk_gt_fll_src_peri_sft_ctrl_en_END    (15)
#define SOC_SCTRL_SYS_STAT_CTRL_0_peri_pwr_up_dn_sft_ctrl_START                  (17)
#define SOC_SCTRL_SYS_STAT_CTRL_0_peri_pwr_up_dn_sft_ctrl_END                    (17)
#define SOC_SCTRL_SYS_STAT_CTRL_0_peri_pwr_pdc_sft_ctrl_en_START                 (18)
#define SOC_SCTRL_SYS_STAT_CTRL_0_peri_pwr_pdc_sft_ctrl_en_END                   (18)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sc_sclk_sw2fll_sft_req_START                   (21)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sc_sclk_sw2fll_sft_req_END                     (21)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sclk_sw2fll_sft_ctrl_en_START                  (22)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sclk_sw2fll_sft_ctrl_en_END                    (22)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_clk_sw_sft_inuse_START                     (25)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_clk_sw_sft_inuse_END                       (25)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_clk_sw_sft_ctrl_en_START                   (26)
#define SOC_SCTRL_SYS_STAT_CTRL_0_bus_clk_sw_sft_ctrl_en_END                     (26)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sc_gt_clk_ulppll_sft_START                     (28)
#define SOC_SCTRL_SYS_STAT_CTRL_0_sc_gt_clk_ulppll_sft_END                       (28)
#define SOC_SCTRL_SYS_STAT_CTRL_0_gt_clk_ulppll_sft_ctrl_en_START                (29)
#define SOC_SCTRL_SYS_STAT_CTRL_0_gt_clk_ulppll_sft_ctrl_en_END                  (29)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_clk_sft_gt_START                           (30)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_clk_sft_gt_END                             (30)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_clk_gt_sft_ctrl_en_START                   (31)
#define SOC_SCTRL_SYS_STAT_CTRL_0_mfc_clk_gt_sft_ctrl_en_END                     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_1_UNION
 struct description   : SYS_STAT_CTRL_1 Register structure definition
                        Address Offset:0xF18 Initial:0x05 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_sft_sel_clk_sys_pre         : 1;  /* bit[0]    : 19.2M时钟源头切换FLL软件切换信号。
                                                                          1：ULPPLL分频 
                                                                          0：PMU 38.4M时钟2分频， */
        unsigned int  sft_sel_clk_sys_ctrl_en        : 1;  /* bit[1]    : 19.2M时钟源头切换FLL软件控制使能。
                                                                          1：使能；
                                                                          0：不使能 */
        unsigned int  sc_sft_clk_sys_gt_ctrl         : 1;  /* bit[2]    : 19.2M时钟源头门控软件切换信号。
                                                                          1：打开
                                                                          0：关闭 */
        unsigned int  sft_clk_sys_gt_ctrl_en         : 1;  /* bit[3]    : 19.2M时钟源头门控软件控制使能。
                                                                          1：使能；
                                                                          0：不使能 */
        unsigned int  tsen_disable_sft_req           : 1;  /* bit[4]    : tsensor关闭软件控制。
                                                                          1：关闭
                                                                          0：使能 */
        unsigned int  tsen_disable_sft_ctrl_en       : 1;  /* bit[5]    : tsensor软件控制使能。
                                                                          1：使能；
                                                                          0：不使能 */
        unsigned int  sel_sys_idle_gt_clksys         : 1;  /* bit[6]    : 0:peri区clksys时钟在DOZE1常开
                                                                          1：peri区clksys时钟在DOZE1关闭 */
        unsigned int  sel_sys_idle_gt_32k            : 1;  /* bit[7]    : 0:peri区32k时钟在DOZE1常开
                                                                          1：peri区32k时钟在DOZE1关闭 */
        unsigned int  cpu_cluster_dvfs_ctrl_bypass   : 7;  /* bit[8-14] : 大中小核fcm的时钟切换控制bypass
                                                                          0：CPUL 1：CPUM 2：FCM 3：CPUB
                                                                          4：reserved 5：reserved 6：reserved */
        unsigned int  ulppll_en_dis_timeout_sel      : 1;  /* bit[15]   : ulppll开关超时使能控制
                                                                          0：使用ULPPLL状态机的指示ULPPLL的开关
                                                                          1：使用SCTRL内部超时完成ULPPLL的开关 */
        unsigned int  cpu_dvfs_sft_req               : 1;  /* bit[16]   : 置1发起CPU时钟切换DVFS */
        unsigned int  cpu_dvfs_sft_ctrl_en           : 1;  /* bit[17]   : CPU时钟切换DVFS软件控制使能。
                                                                          1：使能；
                                                                          0：不使能 */
        unsigned int  clk_sys_ctrl_sel               : 1;  /* bit[18]   : 19.2M时钟源头控制选择。
                                                                          1：19.2M时钟源头切换FLL；
                                                                          0：19.2M时钟源头门控 */
        unsigned int  reserved                       : 1;  /* bit[19]   :  */
        unsigned int  peri_sram_sft_ds               : 1;  /* bit[20]   : tsensor关闭软件控制。
                                                                          1：关闭
                                                                          0：使能 */
        unsigned int  peri_sram_ds_sft_ctrl_en       : 1;  /* bit[21]   : PERI SRAM DS软件控制使能。
                                                                          1：使能；
                                                                          0：不使能 */
        unsigned int  usbphy_ref_clk_gt_sft          : 1;  /* bit[22]   :  */
        unsigned int  usbphy_ref_clk_gt_sft_ctrl_en  : 1;  /* bit[23]   : USB参考时钟控制选择。
                                                                          1：软件控制；
                                                                          0：硬件使能 */
        unsigned int  periph_cpu_clk_sft_sw          : 1;  /* bit[24]   :  */
        unsigned int  sel_periph_cpu_clk_sft_ctrl_en : 1;  /* bit[25]   :  */
        unsigned int  sram_ds_exit_time              : 6;  /* bit[26-31]:  */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_1_sc_sft_sel_clk_sys_pre_START          (0)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sc_sft_sel_clk_sys_pre_END            (0)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sft_sel_clk_sys_ctrl_en_START         (1)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sft_sel_clk_sys_ctrl_en_END           (1)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sc_sft_clk_sys_gt_ctrl_START          (2)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sc_sft_clk_sys_gt_ctrl_END            (2)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sft_clk_sys_gt_ctrl_en_START          (3)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sft_clk_sys_gt_ctrl_en_END            (3)
#define SOC_SCTRL_SYS_STAT_CTRL_1_tsen_disable_sft_req_START            (4)
#define SOC_SCTRL_SYS_STAT_CTRL_1_tsen_disable_sft_req_END              (4)
#define SOC_SCTRL_SYS_STAT_CTRL_1_tsen_disable_sft_ctrl_en_START        (5)
#define SOC_SCTRL_SYS_STAT_CTRL_1_tsen_disable_sft_ctrl_en_END          (5)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sel_sys_idle_gt_clksys_START          (6)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sel_sys_idle_gt_clksys_END            (6)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sel_sys_idle_gt_32k_START             (7)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sel_sys_idle_gt_32k_END               (7)
#define SOC_SCTRL_SYS_STAT_CTRL_1_cpu_cluster_dvfs_ctrl_bypass_START    (8)
#define SOC_SCTRL_SYS_STAT_CTRL_1_cpu_cluster_dvfs_ctrl_bypass_END      (14)
#define SOC_SCTRL_SYS_STAT_CTRL_1_ulppll_en_dis_timeout_sel_START       (15)
#define SOC_SCTRL_SYS_STAT_CTRL_1_ulppll_en_dis_timeout_sel_END         (15)
#define SOC_SCTRL_SYS_STAT_CTRL_1_cpu_dvfs_sft_req_START                (16)
#define SOC_SCTRL_SYS_STAT_CTRL_1_cpu_dvfs_sft_req_END                  (16)
#define SOC_SCTRL_SYS_STAT_CTRL_1_cpu_dvfs_sft_ctrl_en_START            (17)
#define SOC_SCTRL_SYS_STAT_CTRL_1_cpu_dvfs_sft_ctrl_en_END              (17)
#define SOC_SCTRL_SYS_STAT_CTRL_1_clk_sys_ctrl_sel_START                (18)
#define SOC_SCTRL_SYS_STAT_CTRL_1_clk_sys_ctrl_sel_END                  (18)
#define SOC_SCTRL_SYS_STAT_CTRL_1_peri_sram_sft_ds_START                (20)
#define SOC_SCTRL_SYS_STAT_CTRL_1_peri_sram_sft_ds_END                  (20)
#define SOC_SCTRL_SYS_STAT_CTRL_1_peri_sram_ds_sft_ctrl_en_START        (21)
#define SOC_SCTRL_SYS_STAT_CTRL_1_peri_sram_ds_sft_ctrl_en_END          (21)
#define SOC_SCTRL_SYS_STAT_CTRL_1_usbphy_ref_clk_gt_sft_START           (22)
#define SOC_SCTRL_SYS_STAT_CTRL_1_usbphy_ref_clk_gt_sft_END             (22)
#define SOC_SCTRL_SYS_STAT_CTRL_1_usbphy_ref_clk_gt_sft_ctrl_en_START   (23)
#define SOC_SCTRL_SYS_STAT_CTRL_1_usbphy_ref_clk_gt_sft_ctrl_en_END     (23)
#define SOC_SCTRL_SYS_STAT_CTRL_1_periph_cpu_clk_sft_sw_START           (24)
#define SOC_SCTRL_SYS_STAT_CTRL_1_periph_cpu_clk_sft_sw_END             (24)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sel_periph_cpu_clk_sft_ctrl_en_START  (25)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sel_periph_cpu_clk_sft_ctrl_en_END    (25)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sram_ds_exit_time_START               (26)
#define SOC_SCTRL_SYS_STAT_CTRL_1_sram_ds_exit_time_END                 (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_2_UNION
 struct description   : SYS_STAT_CTRL_2 Register structure definition
                        Address Offset:0xF1C Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ao2peri_clk_gt_on_time  : 16; /* bit[0-15] : 门控开启时间 */
        unsigned int  ao2peri_clk_gt_off_time : 16; /* bit[16-31]: 门控关闭时间 */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_2_ao2peri_clk_gt_on_time_START   (0)
#define SOC_SCTRL_SYS_STAT_CTRL_2_ao2peri_clk_gt_on_time_END     (15)
#define SOC_SCTRL_SYS_STAT_CTRL_2_ao2peri_clk_gt_off_time_START  (16)
#define SOC_SCTRL_SYS_STAT_CTRL_2_ao2peri_clk_gt_off_time_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_3_UNION
 struct description   : SYS_STAT_CTRL_3 Register structure definition
                        Address Offset:0xFB4 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mfc_clk_gt_on_time  : 16; /* bit[0-15] : 门控开启时间 */
        unsigned int  mfc_clk_gt_off_time : 16; /* bit[16-31]: 门控关闭时间 */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_3_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_3_mfc_clk_gt_on_time_START   (0)
#define SOC_SCTRL_SYS_STAT_CTRL_3_mfc_clk_gt_on_time_END     (15)
#define SOC_SCTRL_SYS_STAT_CTRL_3_mfc_clk_gt_off_time_START  (16)
#define SOC_SCTRL_SYS_STAT_CTRL_3_mfc_clk_gt_off_time_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_4_UNION
 struct description   : SYS_STAT_CTRL_4 Register structure definition
                        Address Offset:0xF10 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bus_idle_enter_time : 16; /* bit[0-15] : SYSBUS到AOBUS进入power disconnect时间 */
        unsigned int  bus_idle_exit_time  : 16; /* bit[16-31]: SYSBUS到AOBUS退出power disconnect时间 */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_4_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_4_bus_idle_enter_time_START  (0)
#define SOC_SCTRL_SYS_STAT_CTRL_4_bus_idle_enter_time_END    (15)
#define SOC_SCTRL_SYS_STAT_CTRL_4_bus_idle_exit_time_START   (16)
#define SOC_SCTRL_SYS_STAT_CTRL_4_bus_idle_exit_time_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_5_UNION
 struct description   : SYS_STAT_CTRL_5 Register structure definition
                        Address Offset:0xF20 Initial:0x00 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_dis_time    : 8;  /* bit[0-7]  : ulppll关闭时间 */
        unsigned int  ulppll_en_time     : 8;  /* bit[8-15] : ulppll开启时间 */
        unsigned int  ulppll_gt_on_time  : 8;  /* bit[16-23]: ulppll门控开启时间 */
        unsigned int  ulppll_gt_off_time : 8;  /* bit[24-31]: ulppll门控关闭时间 */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_5_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_dis_time_START     (0)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_dis_time_END       (7)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_en_time_START      (8)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_en_time_END        (15)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_gt_on_time_START   (16)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_gt_on_time_END     (23)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_gt_off_time_START  (24)
#define SOC_SCTRL_SYS_STAT_CTRL_5_ulppll_gt_off_time_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_CTRL_6_UNION
 struct description   : SYS_STAT_CTRL_6 Register structure definition
                        Address Offset:0xF24 Initial:0x00 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  clksys_gt_off_time : 8;  /* bit[0-7]  : clksys门控关闭时间 */
        unsigned int  clksys_gt_on_time  : 8;  /* bit[8-15] : clksys门控开启时间 */
        unsigned int  reserved_0         : 8;  /* bit[16-23]:  */
        unsigned int  reserved_1         : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SYS_STAT_CTRL_6_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_CTRL_6_clksys_gt_off_time_START  (0)
#define SOC_SCTRL_SYS_STAT_CTRL_6_clksys_gt_off_time_END    (7)
#define SOC_SCTRL_SYS_STAT_CTRL_6_clksys_gt_on_time_START   (8)
#define SOC_SCTRL_SYS_STAT_CTRL_6_clksys_gt_on_time_END     (15)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_PMU_PS_CTRL_0_UNION
 struct description   : SC_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xF90 Initial:0x00 Width:32
 register description : PMU控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0               : 1;  /* bit[0]    :  */
        unsigned int  pmu_ps_lp_sft_ctrl_en    : 1;  /* bit[1]    : PMU power sequence软件控制使能。
                                                                    1：使能；
                                                                    0：不使能 */
        unsigned int  doze1_ps_skip_en         : 1;  /* bit[2]    :  */
        unsigned int  doze2_ps_skip_en         : 1;  /* bit[3]    :  */
        unsigned int  slv_pmu_ps_sft_idx       : 8;  /* bit[4-11] : 软件控制下，往slv_pmu 发送的power sequence码字 */
        unsigned int  slv_pmu_ps_sft_chg       : 1;  /* bit[12]   : 软件控制下，往slv_pmu 发送的power sequence的使能 */
        unsigned int  reserved_1               : 1;  /* bit[13]   :  */
        unsigned int  reserved_2               : 1;  /* bit[14]   :  */
        unsigned int  reserved_3               : 1;  /* bit[15]   :  */
        unsigned int  mst_pmu_ps_sft_idx       : 8;  /* bit[16-23]: 软件控制下，往mst_pmu 发送的power sequence码字 */
        unsigned int  mst_pmu_ps_sft_chg       : 1;  /* bit[24]   : 软件控制下，往mst_pmu 发送的power sequence的使能 */
        unsigned int  fer_mst_pmu_psok_sft_req : 1;  /* bit[25]   : 软件控制下，给FER发送的查询mst_pmu的power sequence是否完成的查询请求 */
        unsigned int  fer_mst_pmu_psok_bypass  : 1;  /* bit[26]   : MST PMU power sequence完成信号是否等待FER完成信号。 
                                                                    1：等待超时； 
                                                                    0：等待fer */
        unsigned int  reserved_4               : 5;  /* bit[27-31]:  */
    } reg;
} SOC_SCTRL_SC_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_pmu_ps_lp_sft_ctrl_en_START     (1)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_pmu_ps_lp_sft_ctrl_en_END       (1)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_doze1_ps_skip_en_START          (2)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_doze1_ps_skip_en_END            (2)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_doze2_ps_skip_en_START          (3)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_doze2_ps_skip_en_END            (3)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_slv_pmu_ps_sft_idx_START        (4)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_slv_pmu_ps_sft_idx_END          (11)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_slv_pmu_ps_sft_chg_START        (12)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_slv_pmu_ps_sft_chg_END          (12)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_mst_pmu_ps_sft_idx_START        (16)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_mst_pmu_ps_sft_idx_END          (23)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_mst_pmu_ps_sft_chg_START        (24)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_mst_pmu_ps_sft_chg_END          (24)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_fer_mst_pmu_psok_sft_req_START  (25)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_fer_mst_pmu_psok_sft_req_END    (25)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_fer_mst_pmu_psok_bypass_START   (26)
#define SOC_SCTRL_SC_PMU_PS_CTRL_0_fer_mst_pmu_psok_bypass_END     (26)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_UNION
 struct description   : SC_DEEPSLEEP_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xE60 Initial:0x00 Width:32
 register description : PMU PS控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_lp_enter_deepsleep_hrd_idx : 8;  /* bit[0-7]  : 进入低功耗状态deepsleep前，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_enter_deepsleep_hrd_idx : 8;  /* bit[8-15] : 进入低功耗状态deepsleep前，向mst_pmu发送退出低功耗的power sequence码字 */
        unsigned int  slv_pmu_ps_lp_exit_deepsleep_hrd_idx  : 8;  /* bit[16-23]: 退出低功耗状态deepsleep后，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_exit_deepsleep_hrd_idx  : 8;  /* bit[24-31]: 退出低功耗状态deepsleep后，向mst_pmu发送退出低功耗的power sequence码字 */
    } reg;
} SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_deepsleep_hrd_idx_START  (0)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_deepsleep_hrd_idx_END    (7)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_deepsleep_hrd_idx_START  (8)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_deepsleep_hrd_idx_END    (15)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_deepsleep_hrd_idx_START   (16)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_deepsleep_hrd_idx_END     (23)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_deepsleep_hrd_idx_START   (24)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_deepsleep_hrd_idx_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_UNION
 struct description   : SC_DEEPSLEEP_PMU_PS_CTRL_1 Register structure definition
                        Address Offset:0xE64 Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_exit_deepsleep_time : 24; /* bit[0-23] : PMU退出deepsleep低功耗的时间 */
        unsigned int  reserved                      : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_pmu_ps_lp_exit_deepsleep_time_START  (0)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_1_pmu_ps_lp_exit_deepsleep_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_UNION
 struct description   : SC_DEEPSLEEP_PMU_PS_CTRL_2 Register structure definition
                        Address Offset:0xE68 Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_enter_deepsleep_time : 24; /* bit[0-23] : PMU进入deepsleep低功耗的时间 */
        unsigned int  reserved                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_pmu_ps_lp_enter_deepsleep_time_START  (0)
#define SOC_SCTRL_SC_DEEPSLEEP_PMU_PS_CTRL_2_pmu_ps_lp_enter_deepsleep_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_UNION
 struct description   : SC_LIGHTSLEEP_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xE6C Initial:0x00 Width:32
 register description : PMU PS控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_lp_enter_lightsleep_hrd_idx : 8;  /* bit[0-7]  : 进入低功耗状态lightsleep前，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_enter_lightsleep_hrd_idx : 8;  /* bit[8-15] : 进入低功耗状态lightsleep前，向mst_pmu发送退出低功耗的power sequence码字 */
        unsigned int  slv_pmu_ps_lp_exit_lightsleep_hrd_idx  : 8;  /* bit[16-23]: 退出低功耗状态lightsleep后，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_exit_lightsleep_hrd_idx  : 8;  /* bit[24-31]: 退出低功耗状态lightsleep后，向mst_pmu发送退出低功耗的power sequence码字 */
    } reg;
} SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_lightsleep_hrd_idx_START  (0)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_lightsleep_hrd_idx_END    (7)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_lightsleep_hrd_idx_START  (8)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_lightsleep_hrd_idx_END    (15)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_lightsleep_hrd_idx_START   (16)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_lightsleep_hrd_idx_END     (23)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_lightsleep_hrd_idx_START   (24)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_lightsleep_hrd_idx_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_UNION
 struct description   : SC_LIGHTSLEEP_PMU_PS_CTRL_1 Register structure definition
                        Address Offset:0xE70 Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_exit_lightsleep_time : 24; /* bit[0-23] : PMU退出lightsleep低功耗的时间 */
        unsigned int  reserved                       : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_pmu_ps_lp_exit_lightsleep_time_START  (0)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_1_pmu_ps_lp_exit_lightsleep_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_UNION
 struct description   : SC_LIGHTSLEEP_PMU_PS_CTRL_2 Register structure definition
                        Address Offset:0xE74 Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_enter_lightsleep_time : 24; /* bit[0-23] : PMU进入lightsleep低功耗的时间 */
        unsigned int  reserved                        : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_pmu_ps_lp_enter_lightsleep_time_START  (0)
#define SOC_SCTRL_SC_LIGHTSLEEP_PMU_PS_CTRL_2_pmu_ps_lp_enter_lightsleep_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_UNION
 struct description   : SC_STANDBY_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xE78 Initial:0x00 Width:32
 register description : PMU PS控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_lp_enter_standby_hrd_idx : 8;  /* bit[0-7]  : 进入低功耗状态standby前，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_enter_standby_hrd_idx : 8;  /* bit[8-15] : 进入低功耗状态standby前，向mst_pmu发送退出低功耗的power sequence码字 */
        unsigned int  slv_pmu_ps_lp_exit_standby_hrd_idx  : 8;  /* bit[16-23]: 退出低功耗状态standby后，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_exit_standby_hrd_idx  : 8;  /* bit[24-31]: 退出低功耗状态standby后，向mst_pmu发送退出低功耗的power sequence码字 */
    } reg;
} SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_standby_hrd_idx_START  (0)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_standby_hrd_idx_END    (7)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_standby_hrd_idx_START  (8)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_standby_hrd_idx_END    (15)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_standby_hrd_idx_START   (16)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_standby_hrd_idx_END     (23)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_standby_hrd_idx_START   (24)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_standby_hrd_idx_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_UNION
 struct description   : SC_STANDBY_PMU_PS_CTRL_1 Register structure definition
                        Address Offset:0xE7C Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_exit_standby_time : 24; /* bit[0-23] : PMU退出standby低功耗的时间 */
        unsigned int  reserved                    : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_pmu_ps_lp_exit_standby_time_START  (0)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_1_pmu_ps_lp_exit_standby_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_UNION
 struct description   : SC_STANDBY_PMU_PS_CTRL_2 Register structure definition
                        Address Offset:0xE80 Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_enter_standby_time : 24; /* bit[0-23] : PMU进入standby低功耗的时间 */
        unsigned int  reserved                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_pmu_ps_lp_enter_standby_time_START  (0)
#define SOC_SCTRL_SC_STANDBY_PMU_PS_CTRL_2_pmu_ps_lp_enter_standby_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_UNION
 struct description   : SC_STD2LS_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xE84 Initial:0x00 Width:32
 register description : PMU PS控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_lp_enter_std2ls_hrd_idx : 8;  /* bit[0-7]  : standby切换到lightsleep，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_enter_std2ls_hrd_idx : 8;  /* bit[8-15] : standby切换到lightsleep，向mst_pmu发送退出低功耗的power sequence码字 */
        unsigned int  slv_pmu_ps_lp_exit_ls2std_hrd_idx  : 8;  /* bit[16-23]: lightsleep切换到standby，向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_exit_ls2std_hrd_idx  : 8;  /* bit[24-31]: lightsleep切换到standby，向mst_pmu发送退出低功耗的power sequence码字 */
    } reg;
} SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_std2ls_hrd_idx_START  (0)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_std2ls_hrd_idx_END    (7)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_std2ls_hrd_idx_START  (8)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_std2ls_hrd_idx_END    (15)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_ls2std_hrd_idx_START   (16)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_ls2std_hrd_idx_END     (23)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_ls2std_hrd_idx_START   (24)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_ls2std_hrd_idx_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_UNION
 struct description   : SC_STD2LS_PMU_PS_CTRL_1 Register structure definition
                        Address Offset:0xE88 Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_exit_ls2std_time : 24; /* bit[0-23] : PMU从lightsleep切换到standby低功耗的时间 */
        unsigned int  reserved                   : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_pmu_ps_lp_exit_ls2std_time_START  (0)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_1_pmu_ps_lp_exit_ls2std_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_UNION
 struct description   : SC_STD2LS_PMU_PS_CTRL_2 Register structure definition
                        Address Offset:0xE8C Initial:0x00 Width:32
 register description : PMU PS超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_enter_std2ls_time : 24; /* bit[0-23] : PMU从standby切换到lightsleep低功耗的时间 */
        unsigned int  reserved                    : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_pmu_ps_lp_enter_std2ls_time_START  (0)
#define SOC_SCTRL_SC_STD2LS_PMU_PS_CTRL_2_pmu_ps_lp_enter_std2ls_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_PMU_PS_STAT_0_UNION
 struct description   : SC_PMU_PS_STAT_0 Register structure definition
                        Address Offset:0xF9C Initial:0x0 Width:32
 register description : PMU Power Sequence 状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_idx_spmi                 : 8;  /* bit[0-7]  :  */
        unsigned int  slv_pmu_ps_chg_spmi                 : 1;  /* bit[8]    :  */
        unsigned int  slv_pmu_ps_ack_spmi                 : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                          : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                          : 1;  /* bit[11]   :  */
        unsigned int  mst_pmu_ps_idx_spmi                 : 8;  /* bit[12-19]: 往mst_pmu 发送的power sequence码字 */
        unsigned int  mst_pmu_ps_chg_spmi                 : 1;  /* bit[20]   : 往mst_pmu 发送的power sequence的使能 */
        unsigned int  mst_pmu_ps_ack_spmi                 : 1;  /* bit[21]   : spmi发送完成信号 */
        unsigned int  fer_mst_pmu_psok_req                : 1;  /* bit[22]   :  */
        unsigned int  fer_mst_pmu_psok_ack                : 1;  /* bit[23]   :  */
        unsigned int  fer_pmu_ps_lp_exit_finish_flag      : 1;  /* bit[24]   :  */
        unsigned int  fer_pmu_ps_lp_enter_finish_flag     : 1;  /* bit[25]   :  */
        unsigned int  reserved_2                          : 1;  /* bit[26]   :  */
        unsigned int  reserved_3                          : 1;  /* bit[27]   :  */
        unsigned int  fer_mst_pmu_ps_lp_enter_finish_flag : 1;  /* bit[28]   :  */
        unsigned int  fer_mst_pmu_ps_lp_exit_finish_flag  : 1;  /* bit[29]   :  */
        unsigned int  fer_err_flag                        : 1;  /* bit[30]   : FER未及时回复错误指示 */
        unsigned int  fer_err_flag_clr                    : 1;  /* bit[31]   : FER未及时回复错误指示清除寄存器 */
    } reg;
} SOC_SCTRL_SC_PMU_PS_STAT_0_UNION;
#endif
#define SOC_SCTRL_SC_PMU_PS_STAT_0_slv_pmu_ps_idx_spmi_START                  (0)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_slv_pmu_ps_idx_spmi_END                    (7)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_slv_pmu_ps_chg_spmi_START                  (8)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_slv_pmu_ps_chg_spmi_END                    (8)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_slv_pmu_ps_ack_spmi_START                  (9)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_slv_pmu_ps_ack_spmi_END                    (9)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_mst_pmu_ps_idx_spmi_START                  (12)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_mst_pmu_ps_idx_spmi_END                    (19)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_mst_pmu_ps_chg_spmi_START                  (20)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_mst_pmu_ps_chg_spmi_END                    (20)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_mst_pmu_ps_ack_spmi_START                  (21)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_mst_pmu_ps_ack_spmi_END                    (21)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_psok_req_START                 (22)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_psok_req_END                   (22)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_psok_ack_START                 (23)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_psok_ack_END                   (23)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_pmu_ps_lp_exit_finish_flag_START       (24)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_pmu_ps_lp_exit_finish_flag_END         (24)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_pmu_ps_lp_enter_finish_flag_START      (25)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_pmu_ps_lp_enter_finish_flag_END        (25)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_ps_lp_enter_finish_flag_START  (28)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_ps_lp_enter_finish_flag_END    (28)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_ps_lp_exit_finish_flag_START   (29)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_mst_pmu_ps_lp_exit_finish_flag_END     (29)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_err_flag_START                         (30)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_err_flag_END                           (30)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_err_flag_clr_START                     (31)
#define SOC_SCTRL_SC_PMU_PS_STAT_0_fer_err_flag_clr_END                       (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_SC_STAT_0_UNION
 struct description   : SYS_SC_STAT_0 Register structure definition
                        Address Offset:0xF2C Initial:0x0 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cur_sc_mode         : 5;  /* bit[0-4]  :  */
        unsigned int  nxt_sc_mode         : 5;  /* bit[5-9]  :  */
        unsigned int  sc_gt_clk_mfc       : 1;  /* bit[10]   :  */
        unsigned int  mfc_sw_req          : 1;  /* bit[11]   :  */
        unsigned int  mfc_sw_ack          : 1;  /* bit[12]   :  */
        unsigned int  hfc_sw_req          : 1;  /* bit[13]   :  */
        unsigned int  hfc_sw_ack          : 1;  /* bit[14]   :  */
        unsigned int  sc_bus_clk_sw_inuse : 1;  /* bit[15]   :  */
        unsigned int  sw_ack_clk_aonoc_sw : 2;  /* bit[16-17]:  */
        unsigned int  pll_en              : 1;  /* bit[18]   :  */
        unsigned int  spll_fsm_hw_stat    : 1;  /* bit[19]   :  */
        unsigned int  sys_sc_cnt_timeout  : 1;  /* bit[20]   :  */
        unsigned int  bus_pwrd_req        : 1;  /* bit[21]   :  */
        unsigned int  bus_pwrd_enter_ack  : 1;  /* bit[22]   :  */
        unsigned int  bus_pwrd_exit_ack   : 1;  /* bit[23]   :  */
        unsigned int  reserved            : 1;  /* bit[24]   :  */
        unsigned int  sys_idle_gt_flag    : 1;  /* bit[25]   :  */
        unsigned int  peri_pwr_up_dn      : 1;  /* bit[26]   :  */
        unsigned int  sc_io_ret           : 1;  /* bit[27]   :  */
        unsigned int  sc_sel_clk_sys_pre  : 1;  /* bit[28]   :  */
        unsigned int  sw_ack_clk_sys_pre  : 2;  /* bit[29-30]:  */
        unsigned int  hw_tsen_disable_req : 1;  /* bit[31]   :  */
    } reg;
} SOC_SCTRL_SYS_SC_STAT_0_UNION;
#endif
#define SOC_SCTRL_SYS_SC_STAT_0_cur_sc_mode_START          (0)
#define SOC_SCTRL_SYS_SC_STAT_0_cur_sc_mode_END            (4)
#define SOC_SCTRL_SYS_SC_STAT_0_nxt_sc_mode_START          (5)
#define SOC_SCTRL_SYS_SC_STAT_0_nxt_sc_mode_END            (9)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_gt_clk_mfc_START        (10)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_gt_clk_mfc_END          (10)
#define SOC_SCTRL_SYS_SC_STAT_0_mfc_sw_req_START           (11)
#define SOC_SCTRL_SYS_SC_STAT_0_mfc_sw_req_END             (11)
#define SOC_SCTRL_SYS_SC_STAT_0_mfc_sw_ack_START           (12)
#define SOC_SCTRL_SYS_SC_STAT_0_mfc_sw_ack_END             (12)
#define SOC_SCTRL_SYS_SC_STAT_0_hfc_sw_req_START           (13)
#define SOC_SCTRL_SYS_SC_STAT_0_hfc_sw_req_END             (13)
#define SOC_SCTRL_SYS_SC_STAT_0_hfc_sw_ack_START           (14)
#define SOC_SCTRL_SYS_SC_STAT_0_hfc_sw_ack_END             (14)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_bus_clk_sw_inuse_START  (15)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_bus_clk_sw_inuse_END    (15)
#define SOC_SCTRL_SYS_SC_STAT_0_sw_ack_clk_aonoc_sw_START  (16)
#define SOC_SCTRL_SYS_SC_STAT_0_sw_ack_clk_aonoc_sw_END    (17)
#define SOC_SCTRL_SYS_SC_STAT_0_pll_en_START               (18)
#define SOC_SCTRL_SYS_SC_STAT_0_pll_en_END                 (18)
#define SOC_SCTRL_SYS_SC_STAT_0_spll_fsm_hw_stat_START     (19)
#define SOC_SCTRL_SYS_SC_STAT_0_spll_fsm_hw_stat_END       (19)
#define SOC_SCTRL_SYS_SC_STAT_0_sys_sc_cnt_timeout_START   (20)
#define SOC_SCTRL_SYS_SC_STAT_0_sys_sc_cnt_timeout_END     (20)
#define SOC_SCTRL_SYS_SC_STAT_0_bus_pwrd_req_START         (21)
#define SOC_SCTRL_SYS_SC_STAT_0_bus_pwrd_req_END           (21)
#define SOC_SCTRL_SYS_SC_STAT_0_bus_pwrd_enter_ack_START   (22)
#define SOC_SCTRL_SYS_SC_STAT_0_bus_pwrd_enter_ack_END     (22)
#define SOC_SCTRL_SYS_SC_STAT_0_bus_pwrd_exit_ack_START    (23)
#define SOC_SCTRL_SYS_SC_STAT_0_bus_pwrd_exit_ack_END      (23)
#define SOC_SCTRL_SYS_SC_STAT_0_sys_idle_gt_flag_START     (25)
#define SOC_SCTRL_SYS_SC_STAT_0_sys_idle_gt_flag_END       (25)
#define SOC_SCTRL_SYS_SC_STAT_0_peri_pwr_up_dn_START       (26)
#define SOC_SCTRL_SYS_SC_STAT_0_peri_pwr_up_dn_END         (26)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_io_ret_START            (27)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_io_ret_END              (27)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_sel_clk_sys_pre_START   (28)
#define SOC_SCTRL_SYS_SC_STAT_0_sc_sel_clk_sys_pre_END     (28)
#define SOC_SCTRL_SYS_SC_STAT_0_sw_ack_clk_sys_pre_START   (29)
#define SOC_SCTRL_SYS_SC_STAT_0_sw_ack_clk_sys_pre_END     (30)
#define SOC_SCTRL_SYS_SC_STAT_0_hw_tsen_disable_req_START  (31)
#define SOC_SCTRL_SYS_SC_STAT_0_hw_tsen_disable_req_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_SC_STAT_1_UNION
 struct description   : SYS_SC_STAT_1 Register structure definition
                        Address Offset:0xFE4 Initial:0x0 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_peri_sram_ctrl_ds         : 1;  /* bit[0]    :  */
        unsigned int  cpu_dvfs_clk_sw_req          : 1;  /* bit[1]    :  */
        unsigned int  cpu_dvfs_clk_sw_finish_flag  : 1;  /* bit[2]    :  */
        unsigned int  cpu_dvfs_sc_doze_flag        : 1;  /* bit[3]    :  */
        unsigned int  gt_clk_usbphy_ref            : 1;  /* bit[4]    :  */
        unsigned int  sys_idle_gt_fll_src_peri     : 1;  /* bit[5]    :  */
        unsigned int  bus_idle_ack_timeout_err     : 1;  /* bit[6]    :  */
        unsigned int  bus_idle_ack_timeout_err_clr : 1;  /* bit[7]    :  */
        unsigned int  spll_fsm_unlock_intr         : 1;  /* bit[8]    :  */
        unsigned int  enter_doze1_add_cnt          : 10; /* bit[9-18] :  */
        unsigned int  enter_doze2_add_cnt          : 10; /* bit[19-28]:  */
        unsigned int  enter_doze_cnt_clr           : 1;  /* bit[29]   :  */
        unsigned int  sw_ack_clk_periph_cpu_sw     : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_SCTRL_SYS_SC_STAT_1_UNION;
#endif
#define SOC_SCTRL_SYS_SC_STAT_1_sc_peri_sram_ctrl_ds_START          (0)
#define SOC_SCTRL_SYS_SC_STAT_1_sc_peri_sram_ctrl_ds_END            (0)
#define SOC_SCTRL_SYS_SC_STAT_1_cpu_dvfs_clk_sw_req_START           (1)
#define SOC_SCTRL_SYS_SC_STAT_1_cpu_dvfs_clk_sw_req_END             (1)
#define SOC_SCTRL_SYS_SC_STAT_1_cpu_dvfs_clk_sw_finish_flag_START   (2)
#define SOC_SCTRL_SYS_SC_STAT_1_cpu_dvfs_clk_sw_finish_flag_END     (2)
#define SOC_SCTRL_SYS_SC_STAT_1_cpu_dvfs_sc_doze_flag_START         (3)
#define SOC_SCTRL_SYS_SC_STAT_1_cpu_dvfs_sc_doze_flag_END           (3)
#define SOC_SCTRL_SYS_SC_STAT_1_gt_clk_usbphy_ref_START             (4)
#define SOC_SCTRL_SYS_SC_STAT_1_gt_clk_usbphy_ref_END               (4)
#define SOC_SCTRL_SYS_SC_STAT_1_sys_idle_gt_fll_src_peri_START      (5)
#define SOC_SCTRL_SYS_SC_STAT_1_sys_idle_gt_fll_src_peri_END        (5)
#define SOC_SCTRL_SYS_SC_STAT_1_bus_idle_ack_timeout_err_START      (6)
#define SOC_SCTRL_SYS_SC_STAT_1_bus_idle_ack_timeout_err_END        (6)
#define SOC_SCTRL_SYS_SC_STAT_1_bus_idle_ack_timeout_err_clr_START  (7)
#define SOC_SCTRL_SYS_SC_STAT_1_bus_idle_ack_timeout_err_clr_END    (7)
#define SOC_SCTRL_SYS_SC_STAT_1_spll_fsm_unlock_intr_START          (8)
#define SOC_SCTRL_SYS_SC_STAT_1_spll_fsm_unlock_intr_END            (8)
#define SOC_SCTRL_SYS_SC_STAT_1_enter_doze1_add_cnt_START           (9)
#define SOC_SCTRL_SYS_SC_STAT_1_enter_doze1_add_cnt_END             (18)
#define SOC_SCTRL_SYS_SC_STAT_1_enter_doze2_add_cnt_START           (19)
#define SOC_SCTRL_SYS_SC_STAT_1_enter_doze2_add_cnt_END             (28)
#define SOC_SCTRL_SYS_SC_STAT_1_enter_doze_cnt_clr_START            (29)
#define SOC_SCTRL_SYS_SC_STAT_1_enter_doze_cnt_clr_END              (29)
#define SOC_SCTRL_SYS_SC_STAT_1_sw_ack_clk_periph_cpu_sw_START      (30)
#define SOC_SCTRL_SYS_SC_STAT_1_sw_ack_clk_periph_cpu_sw_END        (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_SLOW_CTRL_UNION
 struct description   : SYS_STAT_SLOW_CTRL Register structure definition
                        Address Offset:0xE90 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mfc_clk_gt_en          : 1;  /* bit[0]    : 进退SLOW的中频时钟门控开关硬件控制使能：
                                                                  1：控制；
                                                                  0：不控制 */
        unsigned int  hfc_pll_ctrl_en        : 1;  /* bit[1]    : 进退SLOW时，高频PLL开关的硬件控制使能。
                                                                  1：控制；
                                                                  0：不控制 */
        unsigned int  hfc2mfc_sw_en          : 1;  /* bit[2]    : 进退SLOW的高频中频时钟切换硬件控制使能。
                                                                  1：控制；
                                                                  0：不控制 */
        unsigned int  reserved               : 13; /* bit[3-15] :  */
        unsigned int  sys_stat_slow_ctrl_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_SLOW_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_mfc_clk_gt_en_START           (0)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_mfc_clk_gt_en_END             (0)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_hfc_pll_ctrl_en_START         (1)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_hfc_pll_ctrl_en_END           (1)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_hfc2mfc_sw_en_START           (2)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_hfc2mfc_sw_en_END             (2)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_sys_stat_slow_ctrl_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_SLOW_CTRL_sys_stat_slow_ctrl_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_UNION
 struct description   : SYS_STAT_DEEPSLEEP_CTRL Register structure definition
                        Address Offset:0xE94 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bus_idle_ctrl_en            : 1;  /* bit[0]    : 进退deepsleep的bus进退低功耗硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  bus_clk_sw_en               : 1;  /* bit[1]    : 进退deepsleep时，AONOC时钟切换硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  ao2peri_clk_gt_en           : 1;  /* bit[2]    : 进退deepsleep的ao->peri时钟门控开关硬件控制使能：
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  pmu_ps_lp_en                : 1;  /* bit[3]    : 进退deepsleep时，PMU power sequence进退低功耗的硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  peri_pwr_ctrl_en            : 1;  /* bit[4]    : 进退deepsleep时，PERI上下电硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  tcxo_ctrl_en                : 1;  /* bit[5]    : 进退deepsleep时，clk_sys切换或者门控硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  io_ret_ctrl_en              : 1;  /* bit[6]    : 进退deepsleep时，IO retention硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  tsen_ctrl_en                : 1;  /* bit[7]    : 进退deepsleep时，TSENSOR关闭硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  sram_ds_ctrl_en             : 1;  /* bit[8]    : 进退deepsleep时，SRAM进退deepsleep硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  peri_sw_to_aoclk_ctrl_en    : 1;  /* bit[9]    : 进退deepsleep时，doze1 cpu 中断唤醒逻辑时钟切换控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  cpu_dvfs_doze1_ctrl_en      : 1;  /* bit[10]   : 进退deepsleep时，CPU DVFS硬件控制使能。
                                                                       1：控制；
                                                                       0：不控制 */
        unsigned int  fll_clk_off_en              : 1;  /* bit[11]   : 进退deepsleep时，FLL时钟开关或门控控制使能。
                                                                       1：控制，系统切到32k
                                                                       0：不控制，系统不切到32K */
        unsigned int  ulppll_dis_en               : 1;  /* bit[12]   : 进退deepsleep时，ULPPLL开关硬件控制使能。
                                                                       1：控制开关，不控制门控
                                                                       0：不控制开关，控制门控 */
        unsigned int  reserved                    : 3;  /* bit[13-15]:  */
        unsigned int  sys_stat_deepsleep_ctrl_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_bus_idle_ctrl_en_START             (0)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_bus_idle_ctrl_en_END               (0)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_bus_clk_sw_en_START                (1)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_bus_clk_sw_en_END                  (1)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_ao2peri_clk_gt_en_START            (2)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_ao2peri_clk_gt_en_END              (2)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_pmu_ps_lp_en_START                 (3)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_pmu_ps_lp_en_END                   (3)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_peri_pwr_ctrl_en_START             (4)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_peri_pwr_ctrl_en_END               (4)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_tcxo_ctrl_en_START                 (5)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_tcxo_ctrl_en_END                   (5)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_io_ret_ctrl_en_START               (6)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_io_ret_ctrl_en_END                 (6)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_tsen_ctrl_en_START                 (7)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_tsen_ctrl_en_END                   (7)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_sram_ds_ctrl_en_START              (8)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_sram_ds_ctrl_en_END                (8)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_peri_sw_to_aoclk_ctrl_en_START     (9)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_peri_sw_to_aoclk_ctrl_en_END       (9)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_cpu_dvfs_doze1_ctrl_en_START       (10)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_cpu_dvfs_doze1_ctrl_en_END         (10)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_fll_clk_off_en_START               (11)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_fll_clk_off_en_END                 (11)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_ulppll_dis_en_START                (12)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_ulppll_dis_en_END                  (12)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_sys_stat_deepsleep_ctrl_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_DEEPSLEEP_CTRL_sys_stat_deepsleep_ctrl_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_UNION
 struct description   : SYS_STAT_LIGHTSLEEP_CTRL Register structure definition
                        Address Offset:0xE98 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bus_idle_ctrl_en             : 1;  /* bit[0]    : 进退lightsleep的bus进退低功耗硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  bus_clk_sw_en                : 1;  /* bit[1]    : 进退lightsleep时，AONOC时钟切换硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  ao2peri_clk_gt_en            : 1;  /* bit[2]    : 进退lightsleep的ao->peri时钟门控开关硬件控制使能：
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  pmu_ps_lp_en                 : 1;  /* bit[3]    : 进退lightsleep时，PMU power sequence进退低功耗的硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  peri_pwr_ctrl_en             : 1;  /* bit[4]    : 进退lightsleep时，PERI上下电硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  tcxo_ctrl_en                 : 1;  /* bit[5]    : 进退lightsleep时，clk_sys切换或者门控硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  io_ret_ctrl_en               : 1;  /* bit[6]    : 进退lightsleep时，IO retention硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  tsen_ctrl_en                 : 1;  /* bit[7]    : 进退lightsleep时，TSENSOR关闭硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  sram_ds_ctrl_en              : 1;  /* bit[8]    : 进退lightsleep时，SRAM进退deepsleep硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  peri_sw_to_aoclk_ctrl_en     : 1;  /* bit[9]    : 进退lightsleep时，doze1 cpu 中断唤醒逻辑时钟切换控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  cpu_dvfs_doze1_ctrl_en       : 1;  /* bit[10]   : 进退lightsleep时，CPU DVFS硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  fll_clk_off_en               : 1;  /* bit[11]   : 进退lightsleep时，FLL时钟开关或门控控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  ulppll_dis_en                : 1;  /* bit[12]   : 进退lightsleep时，ULPPLL开关硬件控制使能。
                                                                        1：控制；
                                                                        0：不控制 */
        unsigned int  reserved                     : 3;  /* bit[13-15]:  */
        unsigned int  sys_stat_lightsleep_ctrl_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_bus_idle_ctrl_en_START              (0)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_bus_idle_ctrl_en_END                (0)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_bus_clk_sw_en_START                 (1)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_bus_clk_sw_en_END                   (1)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_ao2peri_clk_gt_en_START             (2)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_ao2peri_clk_gt_en_END               (2)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_pmu_ps_lp_en_START                  (3)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_pmu_ps_lp_en_END                    (3)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_peri_pwr_ctrl_en_START              (4)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_peri_pwr_ctrl_en_END                (4)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_tcxo_ctrl_en_START                  (5)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_tcxo_ctrl_en_END                    (5)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_io_ret_ctrl_en_START                (6)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_io_ret_ctrl_en_END                  (6)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_tsen_ctrl_en_START                  (7)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_tsen_ctrl_en_END                    (7)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_sram_ds_ctrl_en_START               (8)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_sram_ds_ctrl_en_END                 (8)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_peri_sw_to_aoclk_ctrl_en_START      (9)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_peri_sw_to_aoclk_ctrl_en_END        (9)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_cpu_dvfs_doze1_ctrl_en_START        (10)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_cpu_dvfs_doze1_ctrl_en_END          (10)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_fll_clk_off_en_START                (11)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_fll_clk_off_en_END                  (11)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_ulppll_dis_en_START                 (12)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_ulppll_dis_en_END                   (12)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_sys_stat_lightsleep_ctrl_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_LIGHTSLEEP_CTRL_sys_stat_lightsleep_ctrl_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_STANDBY_CTRL_UNION
 struct description   : SYS_STAT_STANDBY_CTRL Register structure definition
                        Address Offset:0xE9C Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bus_idle_ctrl_en          : 1;  /* bit[0]    : 进退standby的bus进退低功耗硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  bus_clk_sw_en             : 1;  /* bit[1]    : 进退standby时，AONOC时钟切换硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  ao2peri_clk_gt_en         : 1;  /* bit[2]    : 进退standby的ao->peri时钟门控开关硬件控制使能：
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  pmu_ps_lp_en              : 1;  /* bit[3]    : 进退standby时，PMU power sequence进退低功耗的硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  peri_pwr_ctrl_en          : 1;  /* bit[4]    : 进退standby时，PERI上下电硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  tcxo_ctrl_en              : 1;  /* bit[5]    : 进退standby时，clk_sys切换或者门控硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  io_ret_ctrl_en            : 1;  /* bit[6]    : 进退standby时，IO retention硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  tsen_ctrl_en              : 1;  /* bit[7]    : 进退standby时，TSENSOR关闭硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  sram_ds_ctrl_en           : 1;  /* bit[8]    : 进退standby时，SRAM进退deepsleep硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  peri_sw_to_aoclk_ctrl_en  : 1;  /* bit[9]    : 进退standby时，doze1 cpu 中断唤醒逻辑时钟切换控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  cpu_dvfs_doze1_ctrl_en    : 1;  /* bit[10]   : 进退standby时，CPU DVFS硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  fll_clk_off_en            : 1;  /* bit[11]   : 进退standby时，FLL时钟开关或门控控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  ulppll_dis_en             : 1;  /* bit[12]   : 进退standby时，ULPPLL开关硬件控制使能。
                                                                     1：控制；
                                                                     0：不控制 */
        unsigned int  reserved                  : 3;  /* bit[13-15]:  */
        unsigned int  sys_stat_standby_ctrl_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_STANDBY_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_bus_idle_ctrl_en_START           (0)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_bus_idle_ctrl_en_END             (0)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_bus_clk_sw_en_START              (1)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_bus_clk_sw_en_END                (1)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_ao2peri_clk_gt_en_START          (2)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_ao2peri_clk_gt_en_END            (2)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_pmu_ps_lp_en_START               (3)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_pmu_ps_lp_en_END                 (3)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_peri_pwr_ctrl_en_START           (4)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_peri_pwr_ctrl_en_END             (4)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_tcxo_ctrl_en_START               (5)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_tcxo_ctrl_en_END                 (5)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_io_ret_ctrl_en_START             (6)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_io_ret_ctrl_en_END               (6)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_tsen_ctrl_en_START               (7)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_tsen_ctrl_en_END                 (7)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_sram_ds_ctrl_en_START            (8)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_sram_ds_ctrl_en_END              (8)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_peri_sw_to_aoclk_ctrl_en_START   (9)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_peri_sw_to_aoclk_ctrl_en_END     (9)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_cpu_dvfs_doze1_ctrl_en_START     (10)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_cpu_dvfs_doze1_ctrl_en_END       (10)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_fll_clk_off_en_START             (11)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_fll_clk_off_en_END               (11)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_ulppll_dis_en_START              (12)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_ulppll_dis_en_END                (12)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_sys_stat_standby_ctrl_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_STANDBY_CTRL_sys_stat_standby_ctrl_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LS2STD_CTRL_UNION
 struct description   : SYS_STAT_LS2STD_CTRL Register structure definition
                        Address Offset:0xEA0 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bus_idle_ctrl_en         : 1;  /* bit[0]    : lightlseep和standby切换时，bus进退低功耗硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  bus_clk_sw_en            : 1;  /* bit[1]    : lightlseep和standby切换时，AONOC时钟切换硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  ao2peri_clk_gt_en        : 1;  /* bit[2]    : lightlseep和standby切换时的ao->peri时钟门控开关硬件控制使能：
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  pmu_ps_lp_en             : 1;  /* bit[3]    : lightlseep和standby切换时，PMU power sequence进退低功耗的硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  peri_pwr_ctrl_en         : 1;  /* bit[4]    : lightlseep和standby切换时，PERI上下电硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  tcxo_ctrl_en             : 1;  /* bit[5]    : lightlseep和standby切换时，clk_sys切换或者门控硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  io_ret_ctrl_en           : 1;  /* bit[6]    : lightlseep和standby切换时，IO retention硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  tsen_ctrl_en             : 1;  /* bit[7]    : lightlseep和standby切换时，TSENSOR关闭硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  sram_ds_ctrl_en          : 1;  /* bit[8]    : lightlseep和standby切换时，SRAM进退deepsleep硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  peri_sw_to_aoclk_ctrl_en : 1;  /* bit[9]    : lightlseep和standby切换时，doze1 cpu 中断唤醒逻辑时钟切换控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  cpu_dvfs_doze1_ctrl_en   : 1;  /* bit[10]   : lightlseep和standby切换时，CPU DVFS硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  fll_clk_off_en           : 1;  /* bit[11]   : lightlseep和standby切换时，FLL时钟开关或门控控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  ulppll_dis_en            : 1;  /* bit[12]   : lightlseep和standby切换时，ULPPLL开关硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  reserved                 : 3;  /* bit[13-15]:  */
        unsigned int  sys_stat_ls2std_ctrl_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_LS2STD_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_bus_idle_ctrl_en_START          (0)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_bus_idle_ctrl_en_END            (0)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_bus_clk_sw_en_START             (1)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_bus_clk_sw_en_END               (1)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_ao2peri_clk_gt_en_START         (2)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_ao2peri_clk_gt_en_END           (2)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_pmu_ps_lp_en_START              (3)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_pmu_ps_lp_en_END                (3)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_peri_pwr_ctrl_en_START          (4)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_peri_pwr_ctrl_en_END            (4)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_tcxo_ctrl_en_START              (5)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_tcxo_ctrl_en_END                (5)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_io_ret_ctrl_en_START            (6)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_io_ret_ctrl_en_END              (6)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_tsen_ctrl_en_START              (7)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_tsen_ctrl_en_END                (7)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_sram_ds_ctrl_en_START           (8)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_sram_ds_ctrl_en_END             (8)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_peri_sw_to_aoclk_ctrl_en_START  (9)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_peri_sw_to_aoclk_ctrl_en_END    (9)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_cpu_dvfs_doze1_ctrl_en_START    (10)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_cpu_dvfs_doze1_ctrl_en_END      (10)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_fll_clk_off_en_START            (11)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_fll_clk_off_en_END              (11)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_ulppll_dis_en_START             (12)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_ulppll_dis_en_END               (12)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_sys_stat_ls2std_ctrl_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_LS2STD_CTRL_sys_stat_ls2std_ctrl_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_DOZE1_CTRL_UNION
 struct description   : SYS_STAT_DOZE1_CTRL Register structure definition
                        Address Offset:0xFC0 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mfc_clk_gt_en            : 1;  /* bit[0]    : 进退doze1的中频时钟门控开关硬件控制使能：
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  hfc_pll_ctrl_en          : 1;  /* bit[1]    : 进退doze1时，中频PLL开关的硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  hfc2mfc_sw_en            : 1;  /* bit[2]    : 进退doze1的高频中频时钟切换硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  bus_idle_ctrl_en         : 1;  /* bit[3]    : 进退doze1的bus进退低功耗硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  bus_clk_sw_en            : 1;  /* bit[4]    : 进退doze1时，AONOC时钟切换硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  ao2peri_clk_gt_en        : 1;  /* bit[5]    : 进退doze1的ao->peri时钟门控开关硬件控制使能：
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  pmu_ps_lp_en             : 1;  /* bit[6]    : 进退doze1时，PMU power sequence进退低功耗的硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  peri_pwr_ctrl_en         : 1;  /* bit[7]    : 进退doze1时，PERI上下电硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  tcxo_ctrl_en             : 1;  /* bit[8]    : 进退doze1时，clk_sys切换或者门控硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  io_ret_ctrl_en           : 1;  /* bit[9]    : 进退doze1时，IO retention硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  tsen_ctrl_en             : 1;  /* bit[10]   : 进退doze1时，TSENSOR关闭硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  sram_ds_ctrl_en          : 1;  /* bit[11]   : 进退doze1时，SRAM进退deepsleep硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  peri_sw_to_aoclk_ctrl_en : 1;  /* bit[12]   : 进退doze1时，doze1 cpu 中断唤醒逻辑时钟切换控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  cpu_dvfs_doze1_ctrl_en   : 1;  /* bit[13]   : 进退doze1时，CPU DVFS硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  fll_clk_off_en           : 1;  /* bit[14]   : 进退doze1时，FLL时钟开关或门控控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  ulppll_dis_en            : 1;  /* bit[15]   : 进退doze1时，ULPPLL开关硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  sys_stat_doze1_ctrl_msk  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_DOZE1_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_mfc_clk_gt_en_START             (0)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_mfc_clk_gt_en_END               (0)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_hfc_pll_ctrl_en_START           (1)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_hfc_pll_ctrl_en_END             (1)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_hfc2mfc_sw_en_START             (2)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_hfc2mfc_sw_en_END               (2)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_bus_idle_ctrl_en_START          (3)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_bus_idle_ctrl_en_END            (3)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_bus_clk_sw_en_START             (4)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_bus_clk_sw_en_END               (4)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_ao2peri_clk_gt_en_START         (5)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_ao2peri_clk_gt_en_END           (5)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_pmu_ps_lp_en_START              (6)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_pmu_ps_lp_en_END                (6)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_peri_pwr_ctrl_en_START          (7)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_peri_pwr_ctrl_en_END            (7)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_tcxo_ctrl_en_START              (8)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_tcxo_ctrl_en_END                (8)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_io_ret_ctrl_en_START            (9)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_io_ret_ctrl_en_END              (9)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_tsen_ctrl_en_START              (10)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_tsen_ctrl_en_END                (10)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_sram_ds_ctrl_en_START           (11)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_sram_ds_ctrl_en_END             (11)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_peri_sw_to_aoclk_ctrl_en_START  (12)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_peri_sw_to_aoclk_ctrl_en_END    (12)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_cpu_dvfs_doze1_ctrl_en_START    (13)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_cpu_dvfs_doze1_ctrl_en_END      (13)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_fll_clk_off_en_START            (14)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_fll_clk_off_en_END              (14)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_ulppll_dis_en_START             (15)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_ulppll_dis_en_END               (15)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_sys_stat_doze1_ctrl_msk_START   (16)
#define SOC_SCTRL_SYS_STAT_DOZE1_CTRL_sys_stat_doze1_ctrl_msk_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_DOZE2_CTRL_UNION
 struct description   : SYS_STAT_DOZE2_CTRL Register structure definition
                        Address Offset:0xFC4 Initial:0x0000 Width:32
 register description : 系统状态控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mfc_clk_gt_en            : 1;  /* bit[0]    : 进退doze2的中频时钟门控开关硬件控制使能：
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  hfc_pll_ctrl_en          : 1;  /* bit[1]    : 进退doze2时，中频PLL开关的硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  hfc2mfc_sw_en            : 1;  /* bit[2]    : 进退doze2的高频中频时钟切换硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  bus_idle_ctrl_en         : 1;  /* bit[3]    : 进退doze2的bus进退低功耗硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  bus_clk_sw_en            : 1;  /* bit[4]    : 进退doze2时，AONOC时钟切换硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  ao2peri_clk_gt_en        : 1;  /* bit[5]    : 进退doze2的ao->peri时钟门控开关硬件控制使能：
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  pmu_ps_lp_en             : 1;  /* bit[6]    : 进退doze2时，PMU power sequence进退低功耗的硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  peri_pwr_ctrl_en         : 1;  /* bit[7]    : 进退doze2时，PERI上下电硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  tcxo_ctrl_en             : 1;  /* bit[8]    : 进退doze2时，clk_sys切换或者门控硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  io_ret_ctrl_en           : 1;  /* bit[9]    : 进退doze2时，IO retention硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  tsen_ctrl_en             : 1;  /* bit[10]   : 进退doze2时，TSENSOR关闭硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  sram_ds_ctrl_en          : 1;  /* bit[11]   : 进退doze2时，SRAM进退deepsleep硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  peri_sw_to_aoclk_ctrl_en : 1;  /* bit[12]   : 进退doze2时，doze1 cpu 中断唤醒逻辑时钟切换控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  cpu_dvfs_doze1_ctrl_en   : 1;  /* bit[13]   : 进退doze2时，CPU DVFS硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  fll_clk_off_en           : 1;  /* bit[14]   : 进退doze2时，FLL时钟开关或门控控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  ulppll_dis_en            : 1;  /* bit[15]   : 进退doze2时，ULPPLL开关硬件控制使能。
                                                                    1：控制；
                                                                    0：不控制 */
        unsigned int  sys_stat_doze2_ctrl_msk  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_DOZE2_CTRL_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_mfc_clk_gt_en_START             (0)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_mfc_clk_gt_en_END               (0)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_hfc_pll_ctrl_en_START           (1)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_hfc_pll_ctrl_en_END             (1)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_hfc2mfc_sw_en_START             (2)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_hfc2mfc_sw_en_END               (2)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_bus_idle_ctrl_en_START          (3)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_bus_idle_ctrl_en_END            (3)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_bus_clk_sw_en_START             (4)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_bus_clk_sw_en_END               (4)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_ao2peri_clk_gt_en_START         (5)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_ao2peri_clk_gt_en_END           (5)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_pmu_ps_lp_en_START              (6)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_pmu_ps_lp_en_END                (6)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_peri_pwr_ctrl_en_START          (7)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_peri_pwr_ctrl_en_END            (7)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_tcxo_ctrl_en_START              (8)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_tcxo_ctrl_en_END                (8)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_io_ret_ctrl_en_START            (9)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_io_ret_ctrl_en_END              (9)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_tsen_ctrl_en_START              (10)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_tsen_ctrl_en_END                (10)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_sram_ds_ctrl_en_START           (11)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_sram_ds_ctrl_en_END             (11)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_peri_sw_to_aoclk_ctrl_en_START  (12)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_peri_sw_to_aoclk_ctrl_en_END    (12)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_cpu_dvfs_doze1_ctrl_en_START    (13)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_cpu_dvfs_doze1_ctrl_en_END      (13)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_fll_clk_off_en_START            (14)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_fll_clk_off_en_END              (14)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_ulppll_dis_en_START             (15)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_ulppll_dis_en_END               (15)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_sys_stat_doze2_ctrl_msk_START   (16)
#define SOC_SCTRL_SYS_STAT_DOZE2_CTRL_sys_stat_doze2_ctrl_msk_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_UNION
 struct description   : SC_DOZE1_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xF94 Initial:0x00 Width:32
 register description : DOZE1下PMU控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_lp_enter_doze1_hrd_idx : 8;  /* bit[0-7]  : 进入doze1前向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_enter_doze1_hrd_idx : 8;  /* bit[8-15] : 进入doze1前向mst_pmu发送退出低功耗的power sequence码字 */
        unsigned int  slv_pmu_ps_lp_exit_doze1_hrd_idx  : 8;  /* bit[16-23]: 退出doze1后向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_exit_doze1_hrd_idx  : 8;  /* bit[24-31]: 退出doze1后向mst_pmu发送退出低功耗的power sequence码字 */
    } reg;
} SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_doze1_hrd_idx_START  (0)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_doze1_hrd_idx_END    (7)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_doze1_hrd_idx_START  (8)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_doze1_hrd_idx_END    (15)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_doze1_hrd_idx_START   (16)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_doze1_hrd_idx_END     (23)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_doze1_hrd_idx_START   (24)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_doze1_hrd_idx_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_UNION
 struct description   : SC_DOZE1_PMU_PS_CTRL_1 Register structure definition
                        Address Offset:0xFD4 Initial:0x00 Width:32
 register description : DOZE1下PMU控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_exit_doze1_time : 24; /* bit[0-23] : PMU退出DOZE1低功耗的时间 */
        unsigned int  reserved_0                : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                : 7;  /* bit[25-31]:  */
    } reg;
} SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_pmu_ps_lp_exit_doze1_time_START  (0)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_1_pmu_ps_lp_exit_doze1_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_UNION
 struct description   : SC_DOZE1_PMU_PS_CTRL_2 Register structure definition
                        Address Offset:0xFD8 Initial:0x00 Width:32
 register description : DOZE1下PMU控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_enter_doze1_time : 24; /* bit[0-23] : PMU进入DOZE1低功耗的时间 */
        unsigned int  reserved_0                 : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                 : 7;  /* bit[25-31]:  */
    } reg;
} SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_pmu_ps_lp_enter_doze1_time_START  (0)
#define SOC_SCTRL_SC_DOZE1_PMU_PS_CTRL_2_pmu_ps_lp_enter_doze1_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_UNION
 struct description   : SC_DOZE2_PMU_PS_CTRL_0 Register structure definition
                        Address Offset:0xFBC Initial:0x00 Width:32
 register description : DOZE2下PMU控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  slv_pmu_ps_lp_enter_doze2_hrd_idx : 8;  /* bit[0-7]  : 进入doze2前向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_enter_doze2_hrd_idx : 8;  /* bit[8-15] : 进入doze2前向mst_pmu发送退出低功耗的power sequence码字 */
        unsigned int  slv_pmu_ps_lp_exit_doze2_hrd_idx  : 8;  /* bit[16-23]: 退出doze2后向slv_pmu发送退出低功耗的power sequence码字 */
        unsigned int  mst_pmu_ps_lp_exit_doze2_hrd_idx  : 8;  /* bit[24-31]: 退出doze2后向mst_pmu发送退出低功耗的power sequence码字 */
    } reg;
} SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_doze2_hrd_idx_START  (0)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_slv_pmu_ps_lp_enter_doze2_hrd_idx_END    (7)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_doze2_hrd_idx_START  (8)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_mst_pmu_ps_lp_enter_doze2_hrd_idx_END    (15)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_doze2_hrd_idx_START   (16)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_slv_pmu_ps_lp_exit_doze2_hrd_idx_END     (23)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_doze2_hrd_idx_START   (24)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_0_mst_pmu_ps_lp_exit_doze2_hrd_idx_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_UNION
 struct description   : SC_DOZE2_PMU_PS_CTRL_1 Register structure definition
                        Address Offset:0xFDC Initial:0x00 Width:32
 register description : DOZE2下PMU控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_exit_doze2_time : 24; /* bit[0-23] : PMU退出DOZE2低功耗的时间 */
        unsigned int  reserved_0                : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                : 7;  /* bit[25-31]:  */
    } reg;
} SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_pmu_ps_lp_exit_doze2_time_START  (0)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_1_pmu_ps_lp_exit_doze2_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_UNION
 struct description   : SC_DOZE2_PMU_PS_CTRL_2 Register structure definition
                        Address Offset:0xFE0 Initial:0x00 Width:32
 register description : DOZE2下PMU控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pmu_ps_lp_enter_doze2_time : 24; /* bit[0-23] : PMU进入DOZE2低功耗的时间 */
        unsigned int  reserved_0                 : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                 : 7;  /* bit[25-31]:  */
    } reg;
} SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_pmu_ps_lp_enter_doze2_time_START  (0)
#define SOC_SCTRL_SC_DOZE2_PMU_PS_CTRL_2_pmu_ps_lp_enter_doze2_time_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_VOTE0_UNION
 struct description   : SC_DOZE1_VOTE0 Register structure definition
                        Address Offset:0x028 Initial:0x0000 Width:32
 register description : DOZE1软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze1_vote_reg     : 16; /* bit[0-15] :  */
        unsigned int  sc_doze1_vote0_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE1_VOTE0_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_VOTE0_doze1_vote_reg_START      (0)
#define SOC_SCTRL_SC_DOZE1_VOTE0_doze1_vote_reg_END        (15)
#define SOC_SCTRL_SC_DOZE1_VOTE0_sc_doze1_vote0_msk_START  (16)
#define SOC_SCTRL_SC_DOZE1_VOTE0_sc_doze1_vote0_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_VOTE1_UNION
 struct description   : SC_DOZE1_VOTE1 Register structure definition
                        Address Offset:0x02C Initial:0x0000 Width:32
 register description : DOZE1软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze1_vote_reg     : 16; /* bit[0-15] :  */
        unsigned int  sc_doze1_vote1_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE1_VOTE1_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_VOTE1_doze1_vote_reg_START      (0)
#define SOC_SCTRL_SC_DOZE1_VOTE1_doze1_vote_reg_END        (15)
#define SOC_SCTRL_SC_DOZE1_VOTE1_sc_doze1_vote1_msk_START  (16)
#define SOC_SCTRL_SC_DOZE1_VOTE1_sc_doze1_vote1_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_VOTE2_UNION
 struct description   : SC_DOZE1_VOTE2 Register structure definition
                        Address Offset:0x9A0 Initial:0x0000 Width:32
 register description : DOZE1软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dss_doze1_vote_reg   : 1;  /* bit[0]    : DSS投票DOZE1专用 */
        unsigned int  modem_doze1_vote_reg : 1;  /* bit[1]    : MODEM投票DOZE1专用 */
        unsigned int  doze1_vote_reg       : 14; /* bit[2-15] :  */
        unsigned int  sc_doze1_vote2_msk   : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE1_VOTE2_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_VOTE2_dss_doze1_vote_reg_START    (0)
#define SOC_SCTRL_SC_DOZE1_VOTE2_dss_doze1_vote_reg_END      (0)
#define SOC_SCTRL_SC_DOZE1_VOTE2_modem_doze1_vote_reg_START  (1)
#define SOC_SCTRL_SC_DOZE1_VOTE2_modem_doze1_vote_reg_END    (1)
#define SOC_SCTRL_SC_DOZE1_VOTE2_doze1_vote_reg_START        (2)
#define SOC_SCTRL_SC_DOZE1_VOTE2_doze1_vote_reg_END          (15)
#define SOC_SCTRL_SC_DOZE1_VOTE2_sc_doze1_vote2_msk_START    (16)
#define SOC_SCTRL_SC_DOZE1_VOTE2_sc_doze1_vote2_msk_END      (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE1_VOTE3_UNION
 struct description   : SC_DOZE1_VOTE3 Register structure definition
                        Address Offset:0x9A4 Initial:0x0000 Width:32
 register description : DOZE1软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze1_vote_reg     : 16; /* bit[0-15] :  */
        unsigned int  sc_doze1_vote3_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE1_VOTE3_UNION;
#endif
#define SOC_SCTRL_SC_DOZE1_VOTE3_doze1_vote_reg_START      (0)
#define SOC_SCTRL_SC_DOZE1_VOTE3_doze1_vote_reg_END        (15)
#define SOC_SCTRL_SC_DOZE1_VOTE3_sc_doze1_vote3_msk_START  (16)
#define SOC_SCTRL_SC_DOZE1_VOTE3_sc_doze1_vote3_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_VOTE0_UNION
 struct description   : SC_DOZE2_VOTE0 Register structure definition
                        Address Offset:0x040 Initial:0x0000 Width:32
 register description : DOZE2软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze2_vote_reg     : 16; /* bit[0-15] :  */
        unsigned int  sc_doze2_vote0_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE2_VOTE0_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_VOTE0_doze2_vote_reg_START      (0)
#define SOC_SCTRL_SC_DOZE2_VOTE0_doze2_vote_reg_END        (15)
#define SOC_SCTRL_SC_DOZE2_VOTE0_sc_doze2_vote0_msk_START  (16)
#define SOC_SCTRL_SC_DOZE2_VOTE0_sc_doze2_vote0_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_VOTE1_UNION
 struct description   : SC_DOZE2_VOTE1 Register structure definition
                        Address Offset:0x044 Initial:0x0000 Width:32
 register description : DOZE2软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze2_vote_reg     : 16; /* bit[0-15] :  */
        unsigned int  sc_doze2_vote1_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE2_VOTE1_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_VOTE1_doze2_vote_reg_START      (0)
#define SOC_SCTRL_SC_DOZE2_VOTE1_doze2_vote_reg_END        (15)
#define SOC_SCTRL_SC_DOZE2_VOTE1_sc_doze2_vote1_msk_START  (16)
#define SOC_SCTRL_SC_DOZE2_VOTE1_sc_doze2_vote1_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_VOTE2_UNION
 struct description   : SC_DOZE2_VOTE2 Register structure definition
                        Address Offset:0x9A8 Initial:0x0000 Width:32
 register description : DOZE2软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dss_doze2_vote_reg   : 1;  /* bit[0]    : DSS投票DOZE2专用 */
        unsigned int  modem_doze2_vote_reg : 1;  /* bit[1]    : MODEM投票DOZE2专用 */
        unsigned int  doze2_vote_reg       : 14; /* bit[2-15] :  */
        unsigned int  sc_doze2_vote2_msk   : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE2_VOTE2_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_VOTE2_dss_doze2_vote_reg_START    (0)
#define SOC_SCTRL_SC_DOZE2_VOTE2_dss_doze2_vote_reg_END      (0)
#define SOC_SCTRL_SC_DOZE2_VOTE2_modem_doze2_vote_reg_START  (1)
#define SOC_SCTRL_SC_DOZE2_VOTE2_modem_doze2_vote_reg_END    (1)
#define SOC_SCTRL_SC_DOZE2_VOTE2_doze2_vote_reg_START        (2)
#define SOC_SCTRL_SC_DOZE2_VOTE2_doze2_vote_reg_END          (15)
#define SOC_SCTRL_SC_DOZE2_VOTE2_sc_doze2_vote2_msk_START    (16)
#define SOC_SCTRL_SC_DOZE2_VOTE2_sc_doze2_vote2_msk_END      (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_DOZE2_VOTE3_UNION
 struct description   : SC_DOZE2_VOTE3 Register structure definition
                        Address Offset:0x9AC Initial:0x0000 Width:32
 register description : DOZE2软投票寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze2_vote_reg     : 16; /* bit[0-15] :  */
        unsigned int  sc_doze2_vote3_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_DOZE2_VOTE3_UNION;
#endif
#define SOC_SCTRL_SC_DOZE2_VOTE3_doze2_vote_reg_START      (0)
#define SOC_SCTRL_SC_DOZE2_VOTE3_doze2_vote_reg_END        (15)
#define SOC_SCTRL_SC_DOZE2_VOTE3_sc_doze2_vote3_msk_START  (16)
#define SOC_SCTRL_SC_DOZE2_VOTE3_sc_doze2_vote3_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SW_INDICATORS_MASK0_UNION
 struct description   : SW_INDICATORS_MASK0 Register structure definition
                        Address Offset:0xF40 Initial:0x1 Width:32
 register description : DOZE控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cpu_lp_ctrl_idle_flag_mask          : 1;  /* bit[0] : 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  gpu_lp_ctrl_idle_flag_mask          : 1;  /* bit[1] : 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  ddr_lp_ctrl_idle_flag_mask          : 1;  /* bit[2] : 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  lpmcu_sleep_status_mask             : 1;  /* bit[3] : 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_0                          : 1;  /* bit[4] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_1                          : 1;  /* bit[5] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_2                          : 1;  /* bit[6] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_3                          : 1;  /* bit[7] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  modem_peribus_nopending_comb_mask   : 1;  /* bit[8] : 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  cfgbus_nopending_comb_mask          : 1;  /* bit[9] : 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  center_crossbar_nopending_comb_mask : 1;  /* bit[10]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  hsdt0bus_nopending_comb_mask        : 1;  /* bit[11]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  hsdt1bus_nopending_comb_mask        : 1;  /* bit[12]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  fcmbus_nopending_comb_mask          : 1;  /* bit[13]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  m1qic_nopending_comb_mask           : 1;  /* bit[14]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  qice_wrap_main_nopending_comb_mask  : 1;  /* bit[15]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  qice_wrap_sc_nopending_comb_mask    : 1;  /* bit[16]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_4                          : 1;  /* bit[17]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_5                          : 1;  /* bit[18]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_6                          : 1;  /* bit[19]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  ufs_ldm_st_h8                       : 1;  /* bit[20]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  ffts_idle                           : 1;  /* bit[21]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  ffts_taskempty                      : 1;  /* bit[22]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  dfa_doze                            : 1;  /* bit[23]: 状态的屏蔽控制
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_7                          : 1;  /* bit[24]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_8                          : 1;  /* bit[25]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_9                          : 1;  /* bit[26]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_10                         : 1;  /* bit[27]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_11                         : 1;  /* bit[28]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_12                         : 1;  /* bit[29]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_13                         : 1;  /* bit[30]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_14                         : 1;  /* bit[31]: 系统IDLE 状态的投票DOZE状态的屏蔽位；(不使用bit必须保持为1.不可配为0!)
                                                                            1：屏蔽 0：不屏蔽 */
    } reg;
} SOC_SCTRL_SW_INDICATORS_MASK0_UNION;
#endif
#define SOC_SCTRL_SW_INDICATORS_MASK0_cpu_lp_ctrl_idle_flag_mask_START           (0)
#define SOC_SCTRL_SW_INDICATORS_MASK0_cpu_lp_ctrl_idle_flag_mask_END             (0)
#define SOC_SCTRL_SW_INDICATORS_MASK0_gpu_lp_ctrl_idle_flag_mask_START           (1)
#define SOC_SCTRL_SW_INDICATORS_MASK0_gpu_lp_ctrl_idle_flag_mask_END             (1)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ddr_lp_ctrl_idle_flag_mask_START           (2)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ddr_lp_ctrl_idle_flag_mask_END             (2)
#define SOC_SCTRL_SW_INDICATORS_MASK0_lpmcu_sleep_status_mask_START              (3)
#define SOC_SCTRL_SW_INDICATORS_MASK0_lpmcu_sleep_status_mask_END                (3)
#define SOC_SCTRL_SW_INDICATORS_MASK0_modem_peribus_nopending_comb_mask_START    (8)
#define SOC_SCTRL_SW_INDICATORS_MASK0_modem_peribus_nopending_comb_mask_END      (8)
#define SOC_SCTRL_SW_INDICATORS_MASK0_cfgbus_nopending_comb_mask_START           (9)
#define SOC_SCTRL_SW_INDICATORS_MASK0_cfgbus_nopending_comb_mask_END             (9)
#define SOC_SCTRL_SW_INDICATORS_MASK0_center_crossbar_nopending_comb_mask_START  (10)
#define SOC_SCTRL_SW_INDICATORS_MASK0_center_crossbar_nopending_comb_mask_END    (10)
#define SOC_SCTRL_SW_INDICATORS_MASK0_hsdt0bus_nopending_comb_mask_START         (11)
#define SOC_SCTRL_SW_INDICATORS_MASK0_hsdt0bus_nopending_comb_mask_END           (11)
#define SOC_SCTRL_SW_INDICATORS_MASK0_hsdt1bus_nopending_comb_mask_START         (12)
#define SOC_SCTRL_SW_INDICATORS_MASK0_hsdt1bus_nopending_comb_mask_END           (12)
#define SOC_SCTRL_SW_INDICATORS_MASK0_fcmbus_nopending_comb_mask_START           (13)
#define SOC_SCTRL_SW_INDICATORS_MASK0_fcmbus_nopending_comb_mask_END             (13)
#define SOC_SCTRL_SW_INDICATORS_MASK0_m1qic_nopending_comb_mask_START            (14)
#define SOC_SCTRL_SW_INDICATORS_MASK0_m1qic_nopending_comb_mask_END              (14)
#define SOC_SCTRL_SW_INDICATORS_MASK0_qice_wrap_main_nopending_comb_mask_START   (15)
#define SOC_SCTRL_SW_INDICATORS_MASK0_qice_wrap_main_nopending_comb_mask_END     (15)
#define SOC_SCTRL_SW_INDICATORS_MASK0_qice_wrap_sc_nopending_comb_mask_START     (16)
#define SOC_SCTRL_SW_INDICATORS_MASK0_qice_wrap_sc_nopending_comb_mask_END       (16)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ufs_ldm_st_h8_START                        (20)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ufs_ldm_st_h8_END                          (20)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ffts_idle_START                            (21)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ffts_idle_END                              (21)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ffts_taskempty_START                       (22)
#define SOC_SCTRL_SW_INDICATORS_MASK0_ffts_taskempty_END                         (22)
#define SOC_SCTRL_SW_INDICATORS_MASK0_dfa_doze_START                             (23)
#define SOC_SCTRL_SW_INDICATORS_MASK0_dfa_doze_END                               (23)


/*****************************************************************************
 struct               : SOC_SCTRL_SW_INDICATORS_MASK1_UNION
 struct description   : SW_INDICATORS_MASK1 Register structure definition
                        Address Offset:0xFE8 Initial:0x1 Width:32
 register description : DOZE控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  media1_mtcmos_ack            : 1;  /* bit[0] : 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  dss_doze1_idle_indicator     : 1;  /* bit[1] : 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_0                   : 1;  /* bit[2] : 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  venc_mtcmos_ack              : 1;  /* bit[3] : 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_1                   : 1;  /* bit[4] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_2                   : 1;  /* bit[5] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  npu_top_iso_en               : 1;  /* bit[6] : 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  gpu_top_iso_en               : 1;  /* bit[7] : 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_3                   : 1;  /* bit[8] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_4                   : 1;  /* bit[9] : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_5                   : 1;  /* bit[10]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  media2_mtcmos_ack            : 1;  /* bit[11]: 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_6                   : 1;  /* bit[12]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_7                   : 1;  /* bit[13]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_8                   : 1;  /* bit[14]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_9                   : 1;  /* bit[15]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  acpu_pc1_stat_mask           : 1;  /* bit[16]: 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  acpu_pc3_stat_mask           : 1;  /* bit[17]: 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  peri_dvs_profile_status_mask : 1;  /* bit[18]: 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  m1_dvs_profile_status_mask   : 1;  /* bit[19]: 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  m2_dvs_profile_status_mask   : 1;  /* bit[20]: 状态的屏蔽控制
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  hw_dss_dvfs_doze1_vote_mask  : 1;  /* bit[21]: 状态的屏蔽控制
                                                                     1：屏蔽 1：不屏蔽 */
        unsigned int  reserved_10                  : 1;  /* bit[22]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_11                  : 1;  /* bit[23]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_12                  : 1;  /* bit[24]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_13                  : 1;  /* bit[25]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_14                  : 1;  /* bit[26]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_15                  : 1;  /* bit[27]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_16                  : 1;  /* bit[28]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_17                  : 1;  /* bit[29]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_18                  : 1;  /* bit[30]: 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_19                  : 1;  /* bit[31]: 系统IDLE 状态的投票DOZE状态的屏蔽位；(不使用bit必须保持为1.不可配为0!)
                                                                     1：屏蔽 0：不屏蔽 */
    } reg;
} SOC_SCTRL_SW_INDICATORS_MASK1_UNION;
#endif
#define SOC_SCTRL_SW_INDICATORS_MASK1_media1_mtcmos_ack_START             (0)
#define SOC_SCTRL_SW_INDICATORS_MASK1_media1_mtcmos_ack_END               (0)
#define SOC_SCTRL_SW_INDICATORS_MASK1_dss_doze1_idle_indicator_START      (1)
#define SOC_SCTRL_SW_INDICATORS_MASK1_dss_doze1_idle_indicator_END        (1)
#define SOC_SCTRL_SW_INDICATORS_MASK1_venc_mtcmos_ack_START               (3)
#define SOC_SCTRL_SW_INDICATORS_MASK1_venc_mtcmos_ack_END                 (3)
#define SOC_SCTRL_SW_INDICATORS_MASK1_npu_top_iso_en_START                (6)
#define SOC_SCTRL_SW_INDICATORS_MASK1_npu_top_iso_en_END                  (6)
#define SOC_SCTRL_SW_INDICATORS_MASK1_gpu_top_iso_en_START                (7)
#define SOC_SCTRL_SW_INDICATORS_MASK1_gpu_top_iso_en_END                  (7)
#define SOC_SCTRL_SW_INDICATORS_MASK1_media2_mtcmos_ack_START             (11)
#define SOC_SCTRL_SW_INDICATORS_MASK1_media2_mtcmos_ack_END               (11)
#define SOC_SCTRL_SW_INDICATORS_MASK1_acpu_pc1_stat_mask_START            (16)
#define SOC_SCTRL_SW_INDICATORS_MASK1_acpu_pc1_stat_mask_END              (16)
#define SOC_SCTRL_SW_INDICATORS_MASK1_acpu_pc3_stat_mask_START            (17)
#define SOC_SCTRL_SW_INDICATORS_MASK1_acpu_pc3_stat_mask_END              (17)
#define SOC_SCTRL_SW_INDICATORS_MASK1_peri_dvs_profile_status_mask_START  (18)
#define SOC_SCTRL_SW_INDICATORS_MASK1_peri_dvs_profile_status_mask_END    (18)
#define SOC_SCTRL_SW_INDICATORS_MASK1_m1_dvs_profile_status_mask_START    (19)
#define SOC_SCTRL_SW_INDICATORS_MASK1_m1_dvs_profile_status_mask_END      (19)
#define SOC_SCTRL_SW_INDICATORS_MASK1_m2_dvs_profile_status_mask_START    (20)
#define SOC_SCTRL_SW_INDICATORS_MASK1_m2_dvs_profile_status_mask_END      (20)
#define SOC_SCTRL_SW_INDICATORS_MASK1_hw_dss_dvfs_doze1_vote_mask_START   (21)
#define SOC_SCTRL_SW_INDICATORS_MASK1_hw_dss_dvfs_doze1_vote_mask_END     (21)


/*****************************************************************************
 struct               : SOC_SCTRL_SW_DDR_IDLE_MASK_UNION
 struct description   : SW_DDR_IDLE_MASK Register structure definition
                        Address Offset:0xF44 Initial:0xFFFF Width:32
 register description : DOZE控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  adb_dmc_a_ddrc_byp_stat_mask   : 1;  /* bit[0]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  adb_dmc_b_ddrc_byp_stat_mask   : 1;  /* bit[1]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_0                     : 1;  /* bit[2]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_1                     : 1;  /* bit[3]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  adb_dmc_a_ddrc_asref_stat_mask : 1;  /* bit[4]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  adb_dmc_b_ddrc_asref_stat_mask : 1;  /* bit[5]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_2                     : 1;  /* bit[6]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_3                     : 1;  /* bit[7]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_4                     : 1;  /* bit[8]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_5                     : 1;  /* bit[9]    : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_6                     : 1;  /* bit[10]   : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_7                     : 1;  /* bit[11]   : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_8                     : 1;  /* bit[12]   : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_9                     : 1;  /* bit[13]   : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_10                    : 1;  /* bit[14]   : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_11                    : 1;  /* bit[15]   : 状态的屏蔽控制
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_12                    : 16; /* bit[16-31]: DDR 的IDLE 状态的投票DOZE状态的屏蔽位；
                                                                          1：屏蔽 0：不屏蔽 */
    } reg;
} SOC_SCTRL_SW_DDR_IDLE_MASK_UNION;
#endif
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_a_ddrc_byp_stat_mask_START    (0)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_a_ddrc_byp_stat_mask_END      (0)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_b_ddrc_byp_stat_mask_START    (1)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_b_ddrc_byp_stat_mask_END      (1)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_a_ddrc_asref_stat_mask_START  (4)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_a_ddrc_asref_stat_mask_END    (4)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_b_ddrc_asref_stat_mask_START  (5)
#define SOC_SCTRL_SW_DDR_IDLE_MASK_adb_dmc_b_ddrc_asref_stat_mask_END    (5)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_COUNT_CTRL0_UNION
 struct description   : DOZE_COUNT_CTRL0 Register structure definition
                        Address Offset:0xF48 Initial:0x00 Width:32
 register description : DOZE超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sw_acpu_pc1_filter : 12; /* bit[0-11] :  */
        unsigned int  sw_acpu_pc3_filter : 12; /* bit[12-23]:  */
        unsigned int  reserved           : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_DOZE_COUNT_CTRL0_UNION;
#endif
#define SOC_SCTRL_DOZE_COUNT_CTRL0_sw_acpu_pc1_filter_START  (0)
#define SOC_SCTRL_DOZE_COUNT_CTRL0_sw_acpu_pc1_filter_END    (11)
#define SOC_SCTRL_DOZE_COUNT_CTRL0_sw_acpu_pc3_filter_START  (12)
#define SOC_SCTRL_DOZE_COUNT_CTRL0_sw_acpu_pc3_filter_END    (23)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_COUNT_CTRL1_UNION
 struct description   : DOZE_COUNT_CTRL1 Register structure definition
                        Address Offset:0xFCC Initial:0x00 Width:32
 register description : DOZE超时控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  normal_enter_time                  : 20; /* bit[0-19] :  */
        unsigned int  doze_entry_pre_clkon_bypass        : 1;  /* bit[20]   : 进出doze态控制XPU_LP_CTRL和CPU的门控信号的bypass */
        unsigned int  doze_ao2peri_clkgt_off_done_bypass : 1;  /* bit[21]   : 退出doze态时打开XPU_LP_CTRL和CPU的门控信号的bypass
                                                                              0：退出doze态时打开XPU_LP_CTRL和CPU的门控信号
                                                                              1：回到slow态时打开XPU_LP_CTRL和CPU的门控信号 */
        unsigned int  modem_doze1_hw_vote_bypass         : 1;  /* bit[22]   : modem doze1硬件投票bypass */
        unsigned int  dss_doze1_hw_vote_bypass           : 1;  /* bit[23]   : dss doze1硬件投票bypass */
        unsigned int  doze2_req_mask                     : 1;  /* bit[24]   : mask doze2的请求，系统不会进DOZE2 */
        unsigned int  doze1_req_mask                     : 1;  /* bit[25]   : mask doze1的请求，系统不会进DOZE1 */
        unsigned int  reserved                           : 6;  /* bit[26-31]:  */
    } reg;
} SOC_SCTRL_DOZE_COUNT_CTRL1_UNION;
#endif
#define SOC_SCTRL_DOZE_COUNT_CTRL1_normal_enter_time_START                   (0)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_normal_enter_time_END                     (19)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze_entry_pre_clkon_bypass_START         (20)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze_entry_pre_clkon_bypass_END           (20)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze_ao2peri_clkgt_off_done_bypass_START  (21)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze_ao2peri_clkgt_off_done_bypass_END    (21)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_modem_doze1_hw_vote_bypass_START          (22)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_modem_doze1_hw_vote_bypass_END            (22)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_dss_doze1_hw_vote_bypass_START            (23)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_dss_doze1_hw_vote_bypass_END              (23)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze2_req_mask_START                      (24)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze2_req_mask_END                        (24)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze1_req_mask_START                      (25)
#define SOC_SCTRL_DOZE_COUNT_CTRL1_doze1_req_mask_END                        (25)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_UNION
 struct description   : DOZE1_DEBOUNCE_CTRL Register structure definition
                        Address Offset:0xE04 Initial:0x000 Width:32
 register description : DOZE1滤波控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze1_req_waitcfg_in  : 10; /* bit[0-9]  :  */
        unsigned int  doze1_req_waitcfg_out : 10; /* bit[10-19]:  */
        unsigned int  reserved              : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_UNION;
#endif
#define SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_doze1_req_waitcfg_in_START   (0)
#define SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_doze1_req_waitcfg_in_END     (9)
#define SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_doze1_req_waitcfg_out_START  (10)
#define SOC_SCTRL_DOZE1_DEBOUNCE_CTRL_doze1_req_waitcfg_out_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_UNION
 struct description   : DOZE2_DEBOUNCE_CTRL Register structure definition
                        Address Offset:0xE08 Initial:0x000 Width:32
 register description : DOZE2滤波控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze2_req_waitcfg_in  : 10; /* bit[0-9]  :  */
        unsigned int  doze2_req_waitcfg_out : 10; /* bit[10-19]:  */
        unsigned int  reserved              : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_UNION;
#endif
#define SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_doze2_req_waitcfg_in_START   (0)
#define SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_doze2_req_waitcfg_in_END     (9)
#define SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_doze2_req_waitcfg_out_START  (10)
#define SOC_SCTRL_DOZE2_DEBOUNCE_CTRL_doze2_req_waitcfg_out_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_ARB_STAT_0_UNION
 struct description   : DOZE_ARB_STAT_0 Register structure definition
                        Address Offset:0xF98 Initial:0x000 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bus_idle_valid    : 12; /* bit[0-11] :  */
        unsigned int  master_idle_valid : 8;  /* bit[12-19]:  */
        unsigned int  ip_idle_valid     : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_DOZE_ARB_STAT_0_UNION;
#endif
#define SOC_SCTRL_DOZE_ARB_STAT_0_bus_idle_valid_START     (0)
#define SOC_SCTRL_DOZE_ARB_STAT_0_bus_idle_valid_END       (11)
#define SOC_SCTRL_DOZE_ARB_STAT_0_master_idle_valid_START  (12)
#define SOC_SCTRL_DOZE_ARB_STAT_0_master_idle_valid_END    (19)
#define SOC_SCTRL_DOZE_ARB_STAT_0_ip_idle_valid_START      (20)
#define SOC_SCTRL_DOZE_ARB_STAT_0_ip_idle_valid_END        (31)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_ARB_STAT_1_UNION
 struct description   : DOZE_ARB_STAT_1 Register structure definition
                        Address Offset:0xFA0 Initial:0x0000 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  doze1_req_aft       : 1;  /* bit[0]    :  */
        unsigned int  doze2_req_aft       : 1;  /* bit[1]    :  */
        unsigned int  doze1_req           : 1;  /* bit[2]    :  */
        unsigned int  doze2_req           : 1;  /* bit[3]    :  */
        unsigned int  sw_doze1_vote_valid : 1;  /* bit[4]    :  */
        unsigned int  sw_doze2_vote_valid : 1;  /* bit[5]    :  */
        unsigned int  acpu_doze1_ok       : 1;  /* bit[6]    :  */
        unsigned int  acpu_doze2_ok       : 1;  /* bit[7]    :  */
        unsigned int  peri_profile_ok     : 1;  /* bit[8]    :  */
        unsigned int  others_pwroff_ok    : 1;  /* bit[9]    :  */
        unsigned int  media_doze1_ok      : 1;  /* bit[10]   :  */
        unsigned int  media_doze2_ok      : 1;  /* bit[11]   :  */
        unsigned int  modem_doze1_ok      : 1;  /* bit[12]   :  */
        unsigned int  modem_doze2_ok      : 1;  /* bit[13]   :  */
        unsigned int  reserved_0          : 1;  /* bit[14]   :  */
        unsigned int  reserved_1          : 1;  /* bit[15]   :  */
        unsigned int  ddr_idle_valid      : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_DOZE_ARB_STAT_1_UNION;
#endif
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze1_req_aft_START        (0)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze1_req_aft_END          (0)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze2_req_aft_START        (1)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze2_req_aft_END          (1)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze1_req_START            (2)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze1_req_END              (2)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze2_req_START            (3)
#define SOC_SCTRL_DOZE_ARB_STAT_1_doze2_req_END              (3)
#define SOC_SCTRL_DOZE_ARB_STAT_1_sw_doze1_vote_valid_START  (4)
#define SOC_SCTRL_DOZE_ARB_STAT_1_sw_doze1_vote_valid_END    (4)
#define SOC_SCTRL_DOZE_ARB_STAT_1_sw_doze2_vote_valid_START  (5)
#define SOC_SCTRL_DOZE_ARB_STAT_1_sw_doze2_vote_valid_END    (5)
#define SOC_SCTRL_DOZE_ARB_STAT_1_acpu_doze1_ok_START        (6)
#define SOC_SCTRL_DOZE_ARB_STAT_1_acpu_doze1_ok_END          (6)
#define SOC_SCTRL_DOZE_ARB_STAT_1_acpu_doze2_ok_START        (7)
#define SOC_SCTRL_DOZE_ARB_STAT_1_acpu_doze2_ok_END          (7)
#define SOC_SCTRL_DOZE_ARB_STAT_1_peri_profile_ok_START      (8)
#define SOC_SCTRL_DOZE_ARB_STAT_1_peri_profile_ok_END        (8)
#define SOC_SCTRL_DOZE_ARB_STAT_1_others_pwroff_ok_START     (9)
#define SOC_SCTRL_DOZE_ARB_STAT_1_others_pwroff_ok_END       (9)
#define SOC_SCTRL_DOZE_ARB_STAT_1_media_doze1_ok_START       (10)
#define SOC_SCTRL_DOZE_ARB_STAT_1_media_doze1_ok_END         (10)
#define SOC_SCTRL_DOZE_ARB_STAT_1_media_doze2_ok_START       (11)
#define SOC_SCTRL_DOZE_ARB_STAT_1_media_doze2_ok_END         (11)
#define SOC_SCTRL_DOZE_ARB_STAT_1_modem_doze1_ok_START       (12)
#define SOC_SCTRL_DOZE_ARB_STAT_1_modem_doze1_ok_END         (12)
#define SOC_SCTRL_DOZE_ARB_STAT_1_modem_doze2_ok_START       (13)
#define SOC_SCTRL_DOZE_ARB_STAT_1_modem_doze2_ok_END         (13)
#define SOC_SCTRL_DOZE_ARB_STAT_1_ddr_idle_valid_START       (16)
#define SOC_SCTRL_DOZE_ARB_STAT_1_ddr_idle_valid_END         (31)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_ARB_STAT_2_UNION
 struct description   : DOZE_ARB_STAT_2 Register structure definition
                        Address Offset:0xFA4 Initial:0x00 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cpu_package_pwrmod_trans_req  : 1;  /* bit[0]    :  */
        unsigned int  cpu_package_pwrmod_trans_ack  : 1;  /* bit[1]    :  */
        unsigned int  cpu_package_pwrmod_trans_stat : 4;  /* bit[2-5]  :  */
        unsigned int  masters_idle_indicators       : 8;  /* bit[6-13] :  */
        unsigned int  bus_idle_status               : 12; /* bit[14-25]:  */
        unsigned int  reserved                      : 6;  /* bit[26-31]:  */
    } reg;
} SOC_SCTRL_DOZE_ARB_STAT_2_UNION;
#endif
#define SOC_SCTRL_DOZE_ARB_STAT_2_cpu_package_pwrmod_trans_req_START   (0)
#define SOC_SCTRL_DOZE_ARB_STAT_2_cpu_package_pwrmod_trans_req_END     (0)
#define SOC_SCTRL_DOZE_ARB_STAT_2_cpu_package_pwrmod_trans_ack_START   (1)
#define SOC_SCTRL_DOZE_ARB_STAT_2_cpu_package_pwrmod_trans_ack_END     (1)
#define SOC_SCTRL_DOZE_ARB_STAT_2_cpu_package_pwrmod_trans_stat_START  (2)
#define SOC_SCTRL_DOZE_ARB_STAT_2_cpu_package_pwrmod_trans_stat_END    (5)
#define SOC_SCTRL_DOZE_ARB_STAT_2_masters_idle_indicators_START        (6)
#define SOC_SCTRL_DOZE_ARB_STAT_2_masters_idle_indicators_END          (13)
#define SOC_SCTRL_DOZE_ARB_STAT_2_bus_idle_status_START                (14)
#define SOC_SCTRL_DOZE_ARB_STAT_2_bus_idle_status_END                  (25)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_ARB_STAT_3_UNION
 struct description   : DOZE_ARB_STAT_3 Register structure definition
                        Address Offset:0xFA8 Initial:0x0 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_idle_indicators   : 12; /* bit[0-11] :  */
        unsigned int  ddr_idle_indicators  : 16; /* bit[12-27]:  */
        unsigned int  normal_enter_timeout : 1;  /* bit[28]   :  */
        unsigned int  reserved             : 3;  /* bit[29-31]:  */
    } reg;
} SOC_SCTRL_DOZE_ARB_STAT_3_UNION;
#endif
#define SOC_SCTRL_DOZE_ARB_STAT_3_ip_idle_indicators_START    (0)
#define SOC_SCTRL_DOZE_ARB_STAT_3_ip_idle_indicators_END      (11)
#define SOC_SCTRL_DOZE_ARB_STAT_3_ddr_idle_indicators_START   (12)
#define SOC_SCTRL_DOZE_ARB_STAT_3_ddr_idle_indicators_END     (27)
#define SOC_SCTRL_DOZE_ARB_STAT_3_normal_enter_timeout_START  (28)
#define SOC_SCTRL_DOZE_ARB_STAT_3_normal_enter_timeout_END    (28)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE_ARB_STAT_4_UNION
 struct description   : DOZE_ARB_STAT_4 Register structure definition
                        Address Offset:0xFAC Initial:0x0 Width:32
 register description : 系统状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pwroff_indicators                : 5;  /* bit[0-4]  :  */
        unsigned int  pwron_indicators                 : 5;  /* bit[5-9]  :  */
        unsigned int  media_pwron_indicator            : 1;  /* bit[10]   :  */
        unsigned int  dss_doze1_idle_indicator         : 1;  /* bit[11]   :  */
        unsigned int  media_others_pwron_indicator     : 4;  /* bit[12-15]:  */
        unsigned int  little_cluster_pwrmod_trans_stat : 4;  /* bit[16-19]:  */
        unsigned int  middle_cluster_pwrmod_trans_stat : 4;  /* bit[20-23]:  */
        unsigned int  big_cluster_pwrmod_trans_stat    : 4;  /* bit[24-27]:  */
        unsigned int  sw_ack_clk_periph_cpu_lit_sw     : 2;  /* bit[28-29]:  */
        unsigned int  sw_ack_clk_periph_cpu_hc_sw      : 2;  /* bit[30-31]:  */
    } reg;
} SOC_SCTRL_DOZE_ARB_STAT_4_UNION;
#endif
#define SOC_SCTRL_DOZE_ARB_STAT_4_pwroff_indicators_START                 (0)
#define SOC_SCTRL_DOZE_ARB_STAT_4_pwroff_indicators_END                   (4)
#define SOC_SCTRL_DOZE_ARB_STAT_4_pwron_indicators_START                  (5)
#define SOC_SCTRL_DOZE_ARB_STAT_4_pwron_indicators_END                    (9)
#define SOC_SCTRL_DOZE_ARB_STAT_4_media_pwron_indicator_START             (10)
#define SOC_SCTRL_DOZE_ARB_STAT_4_media_pwron_indicator_END               (10)
#define SOC_SCTRL_DOZE_ARB_STAT_4_dss_doze1_idle_indicator_START          (11)
#define SOC_SCTRL_DOZE_ARB_STAT_4_dss_doze1_idle_indicator_END            (11)
#define SOC_SCTRL_DOZE_ARB_STAT_4_media_others_pwron_indicator_START      (12)
#define SOC_SCTRL_DOZE_ARB_STAT_4_media_others_pwron_indicator_END        (15)
#define SOC_SCTRL_DOZE_ARB_STAT_4_little_cluster_pwrmod_trans_stat_START  (16)
#define SOC_SCTRL_DOZE_ARB_STAT_4_little_cluster_pwrmod_trans_stat_END    (19)
#define SOC_SCTRL_DOZE_ARB_STAT_4_middle_cluster_pwrmod_trans_stat_START  (20)
#define SOC_SCTRL_DOZE_ARB_STAT_4_middle_cluster_pwrmod_trans_stat_END    (23)
#define SOC_SCTRL_DOZE_ARB_STAT_4_big_cluster_pwrmod_trans_stat_START     (24)
#define SOC_SCTRL_DOZE_ARB_STAT_4_big_cluster_pwrmod_trans_stat_END       (27)
#define SOC_SCTRL_DOZE_ARB_STAT_4_sw_ack_clk_periph_cpu_lit_sw_START      (28)
#define SOC_SCTRL_DOZE_ARB_STAT_4_sw_ack_clk_periph_cpu_lit_sw_END        (29)
#define SOC_SCTRL_DOZE_ARB_STAT_4_sw_ack_clk_periph_cpu_hc_sw_START       (30)
#define SOC_SCTRL_DOZE_ARB_STAT_4_sw_ack_clk_periph_cpu_hc_sw_END         (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYSBUS_PWRD_CTRL_UNION
 struct description   : SYSBUS_PWRD_CTRL Register structure definition
                        Address Offset:0xFB8 Initial:0x0000000 Width:32
 register description : SYSBUS低功耗控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sysbus_pwrd_sft_req           : 1;  /* bit[0]   :  */
        unsigned int  sysbus_pwrd_sft_ctrl_en       : 1;  /* bit[1]   :  */
        unsigned int  sysbus_pwrd_ack               : 1;  /* bit[2]   :  */
        unsigned int  aobus2sysbus_pwrd_sft_req     : 1;  /* bit[3]   :  */
        unsigned int  aobus2sysbus_pwrd_sft_ctrl_en : 1;  /* bit[4]   :  */
        unsigned int  aobus2sysbus_pwrd_ack         : 1;  /* bit[5]   :  */
        unsigned int  reserved                      : 26; /* bit[6-31]: 保留。 */
    } reg;
} SOC_SCTRL_SYSBUS_PWRD_CTRL_UNION;
#endif
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_sysbus_pwrd_sft_req_START            (0)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_sysbus_pwrd_sft_req_END              (0)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_sysbus_pwrd_sft_ctrl_en_START        (1)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_sysbus_pwrd_sft_ctrl_en_END          (1)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_sysbus_pwrd_ack_START                (2)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_sysbus_pwrd_ack_END                  (2)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_aobus2sysbus_pwrd_sft_req_START      (3)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_aobus2sysbus_pwrd_sft_req_END        (3)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_aobus2sysbus_pwrd_sft_ctrl_en_START  (4)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_aobus2sysbus_pwrd_sft_ctrl_en_END    (4)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_aobus2sysbus_pwrd_ack_START          (5)
#define SOC_SCTRL_SYSBUS_PWRD_CTRL_aobus2sysbus_pwrd_ack_END            (5)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_UNION
 struct description   : SC_SCLK_SW2FLL_CTRL Register structure definition
                        Address Offset:0xF50 Initial:0x0000 Width:32
 register description : 时钟切换控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_sft_sw2fll_req              : 2;  /* bit[0-1]  : 1'b0:软件请求切换到32K, 1'b1: 软件请求切换到FLL */
        unsigned int  sc_sw2fll_req_msk              : 5;  /* bit[2-6]  : 1'b0: 不屏蔽，1'b1: 屏蔽
                                                                          bit[4]: tcp切换请求屏蔽控制
                                                                          bit[3]: iomcu切换请求屏蔽控制
                                                                          bit[2]: asp切换请求屏蔽控制
                                                                          bit[1:0]: sc_sft_sw2fll_req[1:0]屏蔽控制 */
        unsigned int  reserved_0                     : 1;  /* bit[7]    :  */
        unsigned int  lfc_mode_fll_to_32k_sft_req    : 1;  /* bit[8]    : 32k和FLL时钟强制切换状态机请求
                                                                          1'b0: 强切32K
                                                                          1'b1: 强切FLL */
        unsigned int  lfc_mode_fll_to_32k_sft_req_en : 1;  /* bit[9]    : 32k和FLL时钟强制切换状态机请求使能
                                                                          1'b0: 不使能
                                                                          1'b1: 使能 */
        unsigned int  reserved_1                     : 2;  /* bit[10-11]:  */
        unsigned int  modem_doze1_mask               : 1;  /* bit[12]   : 状态的屏蔽控制(不使用bit必须保持为1.不可配为0!)
                                                                          1：屏蔽 0：不屏蔽 */
        unsigned int  reserved_2                     : 3;  /* bit[13-15]:  */
        unsigned int  sc_sclk_sw2fll_ctrl_msk        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_UNION;
#endif
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_sc_sft_sw2fll_req_START               (0)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_sc_sft_sw2fll_req_END                 (1)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_sc_sw2fll_req_msk_START               (2)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_sc_sw2fll_req_msk_END                 (6)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_lfc_mode_fll_to_32k_sft_req_START     (8)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_lfc_mode_fll_to_32k_sft_req_END       (8)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_lfc_mode_fll_to_32k_sft_req_en_START  (9)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_lfc_mode_fll_to_32k_sft_req_en_END    (9)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_modem_doze1_mask_START                (12)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_modem_doze1_mask_END                  (12)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_sc_sclk_sw2fll_ctrl_msk_START         (16)
#define SOC_SCTRL_SC_SCLK_SW2FLL_CTRL_sc_sclk_sw2fll_ctrl_msk_END           (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCXO_CTRL_UNION
 struct description   : SC_TCXO_CTRL Register structure definition
                        Address Offset:0xF08 Initial:0x00 Width:32
 register description : TCXO控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tcxo_on_time    : 24; /* bit[0-23] : tcxo开启时间 */
        unsigned int  tcxo_enable_sel : 1;  /* bit[24]   : TCXO开启判断选择 1：跟随状态机tcxo开启，0：通过tcxo_on_time超时判断 */
        unsigned int  reserved        : 7;  /* bit[25-31]: 门控关闭时间 */
    } reg;
} SOC_SCTRL_SC_TCXO_CTRL_UNION;
#endif
#define SOC_SCTRL_SC_TCXO_CTRL_tcxo_on_time_START     (0)
#define SOC_SCTRL_SC_TCXO_CTRL_tcxo_on_time_END       (23)
#define SOC_SCTRL_SC_TCXO_CTRL_tcxo_enable_sel_START  (24)
#define SOC_SCTRL_SC_TCXO_CTRL_tcxo_enable_sel_END    (24)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_PERI_POWER_STAT_UNION
 struct description   : SC_PERI_POWER_STAT Register structure definition
                        Address Offset:0xF58 Initial:0x0 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pw_nxt_stat : 4;  /* bit[0-3]  :  */
        unsigned int  pw_cur_stat : 4;  /* bit[4-7]  :  */
        unsigned int  peri_iso_en : 1;  /* bit[8]    :  */
        unsigned int  reserved_0  : 1;  /* bit[9]    :  */
        unsigned int  reserved_1  : 10; /* bit[10-19]:  */
        unsigned int  reserved_2  : 11; /* bit[20-30]:  */
        unsigned int  reserved_3  : 1;  /* bit[31]   :  */
    } reg;
} SOC_SCTRL_SC_PERI_POWER_STAT_UNION;
#endif
#define SOC_SCTRL_SC_PERI_POWER_STAT_pw_nxt_stat_START  (0)
#define SOC_SCTRL_SC_PERI_POWER_STAT_pw_nxt_stat_END    (3)
#define SOC_SCTRL_SC_PERI_POWER_STAT_pw_cur_stat_START  (4)
#define SOC_SCTRL_SC_PERI_POWER_STAT_pw_cur_stat_END    (7)
#define SOC_SCTRL_SC_PERI_POWER_STAT_peri_iso_en_START  (8)
#define SOC_SCTRL_SC_PERI_POWER_STAT_peri_iso_en_END    (8)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERIISOBYPASS_UNION
 struct description   : SCPERIISOBYPASS Register structure definition
                        Address Offset:0xF5C Initial:0x000000 Width:32
 register description : PERI_ISO BP寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_iso_dynamic_ctrl_bypass : 1;  /* bit[0]   : 1'b1: PW FSM不控制PERI变压区ISO功能 */
        unsigned int  reserved_0                   : 3;  /* bit[1-3] :  */
        unsigned int  peri_rst_dynamic_ctrl_bypass : 1;  /* bit[4]   : 1'b1: PW FSM不控制PERI变压区RST功能 */
        unsigned int  reserved_1                   : 3;  /* bit[5-7] :  */
        unsigned int  peri_urst_flag_ctrl_bypass   : 1;  /* bit[8]   : 1'b1: PW FSM解复位只看超时 */
        unsigned int  reserved_2                   : 23; /* bit[9-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCPERIISOBYPASS_UNION;
#endif
#define SOC_SCTRL_SCPERIISOBYPASS_peri_iso_dynamic_ctrl_bypass_START  (0)
#define SOC_SCTRL_SCPERIISOBYPASS_peri_iso_dynamic_ctrl_bypass_END    (0)
#define SOC_SCTRL_SCPERIISOBYPASS_peri_rst_dynamic_ctrl_bypass_START  (4)
#define SOC_SCTRL_SCPERIISOBYPASS_peri_rst_dynamic_ctrl_bypass_END    (4)
#define SOC_SCTRL_SCPERIISOBYPASS_peri_urst_flag_ctrl_bypass_START    (8)
#define SOC_SCTRL_SCPERIISOBYPASS_peri_urst_flag_ctrl_bypass_END      (8)


/*****************************************************************************
 struct               : SOC_SCTRL_DOZE2_FLAG_STAT_UNION
 struct description   : DOZE2_FLAG_STAT Register structure definition
                        Address Offset:0xE0C Initial:0x0 Width:32
 register description : DOZE2状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  been_doze2_flag : 1;  /* bit[0]    : 1表示系统进入过DOZE2状态，软件读清0 */
        unsigned int  reserved_0      : 27; /* bit[1-27] :  */
        unsigned int  reserved_1      : 1;  /* bit[28]   :  */
        unsigned int  reserved_2      : 3;  /* bit[29-31]: 注意SYS_MISC有特殊处理 */
    } reg;
} SOC_SCTRL_DOZE2_FLAG_STAT_UNION;
#endif
#define SOC_SCTRL_DOZE2_FLAG_STAT_been_doze2_flag_START  (0)
#define SOC_SCTRL_DOZE2_FLAG_STAT_been_doze2_flag_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_PERI_POWER_CTRL_0_UNION
 struct description   : SC_PERI_POWER_CTRL_0 Register structure definition
                        Address Offset:0xF70 Initial:0x0003 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_clk_on_time  : 16; /* bit[0-15] :  */
        unsigned int  peri_clk_off_time : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SC_PERI_POWER_CTRL_0_UNION;
#endif
#define SOC_SCTRL_SC_PERI_POWER_CTRL_0_peri_clk_on_time_START   (0)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_0_peri_clk_on_time_END     (15)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_0_peri_clk_off_time_START  (16)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_0_peri_clk_off_time_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_PERI_POWER_CTRL_1_UNION
 struct description   : SC_PERI_POWER_CTRL_1 Register structure definition
                        Address Offset:0xF74 Initial:0x0002 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_rst_on_time  : 16; /* bit[0-15] :  */
        unsigned int  peri_rst_off_time : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SC_PERI_POWER_CTRL_1_UNION;
#endif
#define SOC_SCTRL_SC_PERI_POWER_CTRL_1_peri_rst_on_time_START   (0)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_1_peri_rst_on_time_END     (15)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_1_peri_rst_off_time_START  (16)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_1_peri_rst_off_time_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_PERI_POWER_CTRL_2_UNION
 struct description   : SC_PERI_POWER_CTRL_2 Register structure definition
                        Address Offset:0xF78 Initial:0x0002 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_iso_dis_time : 16; /* bit[0-15] :  */
        unsigned int  peri_iso_en_time  : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SC_PERI_POWER_CTRL_2_UNION;
#endif
#define SOC_SCTRL_SC_PERI_POWER_CTRL_2_peri_iso_dis_time_START  (0)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_2_peri_iso_dis_time_END    (15)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_2_peri_iso_en_time_START   (16)
#define SOC_SCTRL_SC_PERI_POWER_CTRL_2_peri_iso_en_time_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_UNION
 struct description   : SC_RESUME_WATCHDOG_CTRL Register structure definition
                        Address Offset:0xF80 Initial:0x0 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resume_watchdog_time       : 20; /* bit[0-19] : resume维测时间，32K时钟计时 */
        unsigned int  resume_watchdog_bypass     : 1;  /* bit[20]   :  */
        unsigned int  resume_watchdog_stop_pulse : 1;  /* bit[21]   :  */
        unsigned int  reserved_0                 : 2;  /* bit[22-23]:  */
        unsigned int  resume_watchdog_en         : 1;  /* bit[24]   :  */
        unsigned int  reserved_1                 : 3;  /* bit[25-27]:  */
        unsigned int  sys_ctrl_wd_rst_req        : 1;  /* bit[28]   :  */
        unsigned int  reserved_2                 : 1;  /* bit[29]   :  */
        unsigned int  reserved_3                 : 2;  /* bit[30-31]:  */
    } reg;
} SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_UNION;
#endif
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_time_START        (0)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_time_END          (19)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_bypass_START      (20)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_bypass_END        (20)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_stop_pulse_START  (21)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_stop_pulse_END    (21)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_en_START          (24)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_resume_watchdog_en_END            (24)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_sys_ctrl_wd_rst_req_START         (28)
#define SOC_SCTRL_SC_RESUME_WATCHDOG_CTRL_sys_ctrl_wd_rst_req_END           (28)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LP_STAT_UNION
 struct description   : SC_LP_STAT Register structure definition
                        Address Offset:0x008 Initial:0x0 Width:32
 register description : 用于指示系统曾经进入过低功耗的状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  deepsleeped     : 1;  /* bit[0]    : 系统是否曾经进入过掉电状态机的DEEP SLEEP模式指示位。
                                                           0：系统没有进入过DEEP SLEEP状态；
                                                           1：系统曾经进入过DEEP SLEEP状态且没有被读清。
                                                           （对该比特位写1清除该位的状态到复位值）。 */
        unsigned int  lightsleeped    : 1;  /* bit[1]    : 系统是否曾经进入过掉电状态机的LIGHT SLEEP模式指示位。
                                                           0：系统没有进入过LIGHT SLEEP状态；
                                                           1：系统曾经进入过LIGHT SLEEP状态且没有被读清。
                                                           （对该比特位写1清除该位的状态到复位值）。 */
        unsigned int  standbyed       : 1;  /* bit[2]    : 系统是否曾经进入过掉电状态机的STANDBY模式指示位。
                                                           0：系统没有进入过STANDBY状态；
                                                           1：系统曾经进入过STANDBY状态且没有被读清。
                                                           （对该比特位写1清除该位的状态到复位值）。 */
        unsigned int  doze1ed         : 1;  /* bit[3]    : 系统是否曾经进入过系统状态机的DOZE1模式指示位。
                                                           0：系统没有进入过DOZE1状态；
                                                           1：系统曾经进入过DOZE1状态且没有被读清。
                                                           （对该比特位写1清除该位的状态到复位值）。 */
        unsigned int  doze2ed         : 1;  /* bit[4]    : 系统是否曾经进入过系统状态机的DOZE2模式指示位。
                                                           0：系统没有进入过DOZE2状态；
                                                           1：系统曾经进入过DOZE2状态且没有被读清。
                                                           （对该比特位写1清除该位的状态到复位值）。 */
        unsigned int  reserved_0      : 15; /* bit[5-19] : 详见《EFUSE信息排布》。 */
        unsigned int  reserved_1      : 11; /* bit[20-30]: 详见《EFUSE信息排布》。 */
        unsigned int  reserved_2      : 1;  /* bit[31]   : SYS_MISC有逻辑处理，注意修改 */
    } reg;
} SOC_SCTRL_SC_LP_STAT_UNION;
#endif
#define SOC_SCTRL_SC_LP_STAT_deepsleeped_START      (0)
#define SOC_SCTRL_SC_LP_STAT_deepsleeped_END        (0)
#define SOC_SCTRL_SC_LP_STAT_lightsleeped_START     (1)
#define SOC_SCTRL_SC_LP_STAT_lightsleeped_END       (1)
#define SOC_SCTRL_SC_LP_STAT_standbyed_START        (2)
#define SOC_SCTRL_SC_LP_STAT_standbyed_END          (2)
#define SOC_SCTRL_SC_LP_STAT_doze1ed_START          (3)
#define SOC_SCTRL_SC_LP_STAT_doze1ed_END            (3)
#define SOC_SCTRL_SC_LP_STAT_doze2ed_START          (4)
#define SOC_SCTRL_SC_LP_STAT_doze2ed_END            (4)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LP_STAT_CLR_UNION
 struct description   : SC_LP_STAT_CLR Register structure definition
                        Address Offset:0x01C Initial:0x0000000 Width:32
 register description : 用于清除指示系统曾经进入过低功耗的状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  deepsleeped_clr  : 1;  /* bit[0]   : 对该比特位写1清除SC_LP_STAT寄存器deepsleep模式指示位到复位值 */
        unsigned int  lightsleeped_clr : 1;  /* bit[1]   : 对该比特位写1清除SC_LP_STAT寄存器lightsleep模式指示位到复位值 */
        unsigned int  standbyed_clr    : 1;  /* bit[2]   : 对该比特位写1清除SC_LP_STAT寄存器standby模式指示位到复位值 */
        unsigned int  doze1ed_clr      : 1;  /* bit[3]   : 对该比特位写1清除SC_LP_STAT寄存器DOZE1模式指示位到复位值 */
        unsigned int  doze2ed_clr      : 1;  /* bit[4]   : 对该比特位写1清除SC_LP_STAT寄存器DOZE2模式指示位到复位值 */
        unsigned int  reserved         : 27; /* bit[5-31]: reserved */
    } reg;
} SOC_SCTRL_SC_LP_STAT_CLR_UNION;
#endif
#define SOC_SCTRL_SC_LP_STAT_CLR_deepsleeped_clr_START   (0)
#define SOC_SCTRL_SC_LP_STAT_CLR_deepsleeped_clr_END     (0)
#define SOC_SCTRL_SC_LP_STAT_CLR_lightsleeped_clr_START  (1)
#define SOC_SCTRL_SC_LP_STAT_CLR_lightsleeped_clr_END    (1)
#define SOC_SCTRL_SC_LP_STAT_CLR_standbyed_clr_START     (2)
#define SOC_SCTRL_SC_LP_STAT_CLR_standbyed_clr_END       (2)
#define SOC_SCTRL_SC_LP_STAT_CLR_doze1ed_clr_START       (3)
#define SOC_SCTRL_SC_LP_STAT_CLR_doze1ed_clr_END         (3)
#define SOC_SCTRL_SC_LP_STAT_CLR_doze2ed_clr_START       (4)
#define SOC_SCTRL_SC_LP_STAT_CLR_doze2ed_clr_END         (4)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LP_FLAG_UNION
 struct description   : SC_LP_FLAG Register structure definition
                        Address Offset:0x018 Initial:0x0000 Width:32
 register description : 状态记录寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  system_stat_flag : 5;  /* bit[0-4]  :  */
        unsigned int  reserved_0       : 5;  /* bit[5-9]  :  */
        unsigned int  reserved_1       : 5;  /* bit[10-14]:  */
        unsigned int  reserved_2       : 1;  /* bit[15]   :  */
        unsigned int  sc_lp_flag_msk   : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_LP_FLAG_UNION;
#endif
#define SOC_SCTRL_SC_LP_FLAG_system_stat_flag_START  (0)
#define SOC_SCTRL_SC_LP_FLAG_system_stat_flag_END    (4)
#define SOC_SCTRL_SC_LP_FLAG_sc_lp_flag_msk_START    (16)
#define SOC_SCTRL_SC_LP_FLAG_sc_lp_flag_msk_END      (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_INTR_WAKEUP_CTRL_UNION
 struct description   : SC_INTR_WAKEUP_CTRL Register structure definition
                        Address Offset:0x014 Initial:0x0 Width:32
 register description : 用于指示系统曾经进入过深度睡眠的状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_wakeup_sctrl2m3_stat           : 1;  /* bit[0]   : 送给LPM3的唤醒中断状态:bit[0] = [1]&[2]&[8] */
        unsigned int  intr_wakeup_sys_ff2                 : 1;  /* bit[1]   : 汇聚后的唤醒中断状态，送给SYS_FSM和PW_FSM */
        unsigned int  intr_wakeup_sctrl2m3_en_at_suspend  : 1;  /* bit[2]   : SUSPEND时，是否将汇聚后的唤醒中断送给LPM3（软件配置bit4去使能，当硬件升压开钟后使能，SR期间的使能） */
        unsigned int  reserved_0                          : 1;  /* bit[3]   :  */
        unsigned int  intr_wakeup_sctrl2m3_clr_at_suspend : 1;  /* bit[4]   : SUSPEND时清除送给LPM3的汇聚后唤醒中断（系统状态机看到的唤醒中断不会清除） */
        unsigned int  reserved_1                          : 3;  /* bit[5-7] :  */
        unsigned int  intr_wakeup_sctrl2m3_en             : 1;  /* bit[8]   : 汇聚后唤醒中断是否送给LPM3，总是能 */
        unsigned int  reserved_2                          : 22; /* bit[9-30]:  */
        unsigned int  reserved_3                          : 1;  /* bit[31]  :  */
    } reg;
} SOC_SCTRL_SC_INTR_WAKEUP_CTRL_UNION;
#endif
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_stat_START            (0)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_stat_END              (0)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sys_ff2_START                  (1)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sys_ff2_END                    (1)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_en_at_suspend_START   (2)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_en_at_suspend_END     (2)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_clr_at_suspend_START  (4)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_clr_at_suspend_END    (4)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_en_START              (8)
#define SOC_SCTRL_SC_INTR_WAKEUP_CTRL_intr_wakeup_sctrl2m3_en_END                (8)


/*****************************************************************************
 struct               : SOC_SCTRL_SCEFUSEDATA2_UNION
 struct description   : SCEFUSEDATA2 Register structure definition
                        Address Offset:0x00C Initial:0x00000000 Width:32
 register description : EFUSEDATA寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved        : 32; /* bit[0-31]: 详见《EFUSE信息排布》 */
    } reg;
} SOC_SCTRL_SCEFUSEDATA2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCEFUSEDATA3_UNION
 struct description   : SCEFUSEDATA3 Register structure definition
                        Address Offset:0x010 Initial:0x00000000 Width:32
 register description : EFUSEDATA寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved        : 32; /* bit[0-31]: 详见《EFUSE信息排布》 */
    } reg;
} SOC_SCTRL_SCEFUSEDATA3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCEFUSEDATA8_UNION
 struct description   : SCEFUSEDATA8 Register structure definition
                        Address Offset:0x060 Initial:0x00000000 Width:32
 register description : EFUSEDATA寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved        : 32; /* bit[0-31]: 详见《EFUSE信息排布》 */
    } reg;
} SOC_SCTRL_SCEFUSEDATA8_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCEFUSEDATA9_UNION
 struct description   : SCEFUSEDATA9 Register structure definition
                        Address Offset:0x064 Initial:0x00000000 Width:32
 register description : EFUSEDATA寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved        : 32; /* bit[0-31]: 详见《EFUSE信息排布》 */
    } reg;
} SOC_SCTRL_SCEFUSEDATA9_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LP_FLAG_POR_UNION
 struct description   : SYS_STAT_LP_FLAG_POR Register structure definition
                        Address Offset:0xE50 Initial:0x00000000 Width:32
 register description : 系统低功耗软件寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件S5_M使用。 */
    } reg;
} SOC_SCTRL_SYS_STAT_LP_FLAG_POR_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_UNION
 struct description   : SYS_STAT_LP_FLAG_0_SEC Register structure definition
                        Address Offset:0xE54 Initial:0x0000 Width:32
 register description : 系统低功耗软件安全寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sys_stat_lp_flag_0_sec     : 16; /* bit[0-15] : 保留给软件S5_M使用。 */
        unsigned int  sys_stat_lp_flag_0_sec_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_sys_stat_lp_flag_0_sec_START      (0)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_sys_stat_lp_flag_0_sec_END        (15)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_sys_stat_lp_flag_0_sec_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_SEC_sys_stat_lp_flag_0_sec_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_UNION
 struct description   : SYS_STAT_LP_FLAG_1_SEC Register structure definition
                        Address Offset:0xE58 Initial:0x0000 Width:32
 register description : 系统低功耗软件安全寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sys_stat_lp_flag_1_sec     : 16; /* bit[0-15] : 保留给软件S5_M使用。 */
        unsigned int  sys_stat_lp_flag_1_sec_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_sys_stat_lp_flag_1_sec_START      (0)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_sys_stat_lp_flag_1_sec_END        (15)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_sys_stat_lp_flag_1_sec_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_SEC_sys_stat_lp_flag_1_sec_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_UNION
 struct description   : SYS_STAT_LP_FLAG_0_UNSEC Register structure definition
                        Address Offset:0x024 Initial:0x0000 Width:32
 register description : 系统低功耗软件非安全寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sys_stat_lp_flag_0_unsec     : 16; /* bit[0-15] : 保留给软件S5_M使用。 */
        unsigned int  sys_stat_lp_flag_0_unsec_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_sys_stat_lp_flag_0_unsec_START      (0)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_sys_stat_lp_flag_0_unsec_END        (15)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_sys_stat_lp_flag_0_unsec_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_0_UNSEC_sys_stat_lp_flag_0_unsec_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_UNION
 struct description   : SYS_STAT_LP_FLAG_1_UNSEC Register structure definition
                        Address Offset:0x034 Initial:0x0000 Width:32
 register description : 系统低功耗软件非安全寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sys_stat_lp_flag_1_unsec     : 16; /* bit[0-15] : 保留给软件S5_M使用。 */
        unsigned int  sys_stat_lp_flag_1_unsec_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_UNION;
#endif
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_sys_stat_lp_flag_1_unsec_START      (0)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_sys_stat_lp_flag_1_unsec_END        (15)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_sys_stat_lp_flag_1_unsec_msk_START  (16)
#define SOC_SCTRL_SYS_STAT_LP_FLAG_1_UNSEC_sys_stat_lp_flag_1_unsec_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LP_DATA_UNION
 struct description   : SC_LP_DATA Register structure definition
                        Address Offset:0x020 Initial:0x00000000 Width:32
 register description : SC数据记录寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_lp_data : 32; /* bit[0-31]: SR数据存储 */
    } reg;
} SOC_SCTRL_SC_LP_DATA_UNION;
#endif
#define SOC_SCTRL_SC_LP_DATA_sc_lp_data_START  (0)
#define SOC_SCTRL_SC_LP_DATA_sc_lp_data_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPSTATCFG_UNION
 struct description   : SCLPSTATCFG Register structure definition
                        Address Offset:0x038 Initial:0x0 Width:32
 register description : 低功耗配置状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lp_stat_cfg_s0 : 1;  /* bit[0]   : 送给ao_loadmonitor的低功耗状态配置信号，s0~s4为one-hot类型。 */
        unsigned int  lp_stat_cfg_s1 : 1;  /* bit[1]   : 同bit0。 */
        unsigned int  lp_stat_cfg_s2 : 1;  /* bit[2]   : 同bit0。 */
        unsigned int  lp_stat_cfg_s3 : 1;  /* bit[3]   : 同bit0。 */
        unsigned int  lp_stat_cfg_s4 : 1;  /* bit[4]   : 同bit0。 */
        unsigned int  reserved_0     : 26; /* bit[5-30]: 保留。 */
        unsigned int  reserved_1     : 1;  /* bit[31]  : 保留。 */
    } reg;
} SOC_SCTRL_SCLPSTATCFG_UNION;
#endif
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s0_START  (0)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s0_END    (0)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s1_START  (1)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s1_END    (1)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s2_START  (2)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s2_END    (2)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s3_START  (3)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s3_END    (3)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s4_START  (4)
#define SOC_SCTRL_SCLPSTATCFG_lp_stat_cfg_s4_END    (4)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_ASP_PWRCFG0_UNION
 struct description   : SC_ASP_PWRCFG0 Register structure definition
                        Address Offset:0x080 Initial:0x0000 Width:32
 register description : ASP上下电配置寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_asp_powerdn_msk      : 1;  /* bit[0]    : ASP下电投票中断屏蔽（不使用）。 */
        unsigned int  intr_asp_powerup_msk      : 1;  /* bit[1]    : ASP上电投票中断屏蔽（不使用）。 */
        unsigned int  vote_asp_powerupdn_bypass : 1;  /* bit[2]    : ASP上下电投票功能bypass。 */
        unsigned int  reserved_0                : 1;  /* bit[3]    :  */
        unsigned int  asp_power_req_lpmcu       : 2;  /* bit[4-5]  : LPMCU发起的ASP上下电投票请求：
                                                                     1'b0: 下电； 1'b1: 上电。
                                                                     LPMCU只投上电，只检测上升沿。 */
        unsigned int  reserved_1                : 2;  /* bit[6-7]  :  */
        unsigned int  asp_power_req_sft0        : 1;  /* bit[8]    : 软件发起的ASP上下电投票请求：
                                                                     1'b0: 下电； 1'b1: 上电。 */
        unsigned int  asp_power_req_sft1        : 1;  /* bit[9]    : 同bit[8] */
        unsigned int  asp_power_req_sft2        : 1;  /* bit[10]   : 同bit[8] */
        unsigned int  asp_power_req_sft3        : 1;  /* bit[11]   : 同bit[8] */
        unsigned int  reserved_2                : 4;  /* bit[12-15]:  */
        unsigned int  sc_asp_pwrcfg0_msk        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_ASP_PWRCFG0_UNION;
#endif
#define SOC_SCTRL_SC_ASP_PWRCFG0_intr_asp_powerdn_msk_START       (0)
#define SOC_SCTRL_SC_ASP_PWRCFG0_intr_asp_powerdn_msk_END         (0)
#define SOC_SCTRL_SC_ASP_PWRCFG0_intr_asp_powerup_msk_START       (1)
#define SOC_SCTRL_SC_ASP_PWRCFG0_intr_asp_powerup_msk_END         (1)
#define SOC_SCTRL_SC_ASP_PWRCFG0_vote_asp_powerupdn_bypass_START  (2)
#define SOC_SCTRL_SC_ASP_PWRCFG0_vote_asp_powerupdn_bypass_END    (2)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_lpmcu_START        (4)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_lpmcu_END          (5)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft0_START         (8)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft0_END           (8)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft1_START         (9)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft1_END           (9)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft2_START         (10)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft2_END           (10)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft3_START         (11)
#define SOC_SCTRL_SC_ASP_PWRCFG0_asp_power_req_sft3_END           (11)
#define SOC_SCTRL_SC_ASP_PWRCFG0_sc_asp_pwrcfg0_msk_START         (16)
#define SOC_SCTRL_SC_ASP_PWRCFG0_sc_asp_pwrcfg0_msk_END           (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_ASP_PWRCFG1_UNION
 struct description   : SC_ASP_PWRCFG1 Register structure definition
                        Address Offset:0x084 Initial:0x0 Width:32
 register description : ASP上下电配置寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_asp_powerdn_clr       : 1;  /* bit[0]    : ASP下电投票中断清除。 */
        unsigned int  intr_asp_powerup_clr       : 1;  /* bit[1]    : ASP上电投票中断清除。 */
        unsigned int  reserved_0                 : 2;  /* bit[2-3]  : 保留。 */
        unsigned int  intr_asp_powerdn_raw       : 1;  /* bit[4]    : ASP下电投票中断(原始中断)。 */
        unsigned int  intr_asp_powerup_raw       : 1;  /* bit[5]    : ASP上电投票中断(原始中断)。 */
        unsigned int  reserved_1                 : 2;  /* bit[6-7]  :  */
        unsigned int  intr_asp_powerdn           : 1;  /* bit[8]    : ASP下电投票中断(上报中断)。 */
        unsigned int  intr_asp_powerup           : 1;  /* bit[9]    : ASP上电投票中断(上报中断)。 */
        unsigned int  reserved_2                 : 2;  /* bit[10-11]:  */
        unsigned int  asp_power_req_hifi_sync    : 2;  /* bit[12-13]: HIFI发起的ASP上下电投票请求：
                                                                      1'b0: 下电； 1'b1: 上电。
                                                                      HIFI只投下电，只检测下降沿。 */
        unsigned int  reserved_3                 : 2;  /* bit[14-15]:  */
        unsigned int  asp_power_req_hifi_stat    : 2;  /* bit[16-17]: LPMCU和HIFI发起的ASP上下电投票请求：LPMCU投上电，HIFI投下电。 */
        unsigned int  reserved_4                 : 2;  /* bit[18-19]:  */
        unsigned int  asp_power_req_cpu_lit_sync : 2;  /* bit[20-21]: CPU发起的ASP上下电投票请求：
                                                                      1'b0: 下电； 1'b1: 上电。 */
        unsigned int  asp_power_req_cpu_hc_sync  : 2;  /* bit[22-23]: CPU发起的ASP上下电投票请求：
                                                                      1'b0: 下电； 1'b1: 上电。 */
        unsigned int  asp_power_req              : 1;  /* bit[24]   : ASP上下电最终请求。 */
        unsigned int  reserved_5                 : 3;  /* bit[25-27]:  */
        unsigned int  curr_asp_power_stat        : 2;  /* bit[28-29]: ASP上下电状态：
                                                                      2'b00: 下电，默认值；
                                                                      2'b01: 正在上电；
                                                                      2'b10: 上电；
                                                                      2'b11: 正在下电。 */
        unsigned int  reserved_6                 : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_SCTRL_SC_ASP_PWRCFG1_UNION;
#endif
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerdn_clr_START        (0)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerdn_clr_END          (0)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerup_clr_START        (1)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerup_clr_END          (1)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerdn_raw_START        (4)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerdn_raw_END          (4)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerup_raw_START        (5)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerup_raw_END          (5)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerdn_START            (8)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerdn_END              (8)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerup_START            (9)
#define SOC_SCTRL_SC_ASP_PWRCFG1_intr_asp_powerup_END              (9)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_hifi_sync_START     (12)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_hifi_sync_END       (13)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_hifi_stat_START     (16)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_hifi_stat_END       (17)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_cpu_lit_sync_START  (20)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_cpu_lit_sync_END    (21)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_cpu_hc_sync_START   (22)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_cpu_hc_sync_END     (23)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_START               (24)
#define SOC_SCTRL_SC_ASP_PWRCFG1_asp_power_req_END                 (24)
#define SOC_SCTRL_SC_ASP_PWRCFG1_curr_asp_power_stat_START         (28)
#define SOC_SCTRL_SC_ASP_PWRCFG1_curr_asp_power_stat_END           (29)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_GATHER_STAT_UNION
 struct description   : SCINT_GATHER_STAT Register structure definition
                        Address Offset:0x0A0 Initial:0x0000000 Width:32
 register description : 聚合后的唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  int_wakeup_sys : 1;  /* bit[0]   : 1：中断有效；
                                                         0：中断无效；
                                                         注：该中断状态是已经考虑中断屏蔽配置之后的状态。 */
        unsigned int  int_ls2std_req : 1;  /* bit[1]   :  */
        unsigned int  int_std2ls_req : 1;  /* bit[2]   :  */
        unsigned int  reserved_0     : 1;  /* bit[3]   :  */
        unsigned int  reserved_1     : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCINT_GATHER_STAT_UNION;
#endif
#define SOC_SCTRL_SCINT_GATHER_STAT_int_wakeup_sys_START  (0)
#define SOC_SCTRL_SCINT_GATHER_STAT_int_wakeup_sys_END    (0)
#define SOC_SCTRL_SCINT_GATHER_STAT_int_ls2std_req_START  (1)
#define SOC_SCTRL_SCINT_GATHER_STAT_int_ls2std_req_END    (1)
#define SOC_SCTRL_SCINT_GATHER_STAT_int_std2ls_req_START  (2)
#define SOC_SCTRL_SCINT_GATHER_STAT_int_std2ls_req_END    (2)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_MASK_UNION
 struct description   : SCINT_MASK Register structure definition
                        Address Offset:0x0A4 Initial:0x0 Width:32
 register description : intr_ao_wakeup0唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 1;  /* bit[0] : 中断屏蔽寄存器。
                                                             0：屏蔽有效；
                                                             1：屏蔽无效。 */
        unsigned int  reserved_1           : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_2           : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  intr_fer0_pmu_gic    : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  intr_fer1_pmu_gic    : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  intr_fer2_pmu_gic    : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  rtc0_int             : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  rtc1_int             : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  timer00_int          : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  timer01_int          : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  timer10_int          : 1;  /* bit[10]: 含义同bit0。 */
        unsigned int  timer11_int          : 1;  /* bit[11]: 含义同bit0。 */
        unsigned int  timer20_int          : 1;  /* bit[12]: 含义同bit0。 */
        unsigned int  timer21_int          : 1;  /* bit[13]: 含义同bit0。 */
        unsigned int  timer30_int          : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  timer31_int          : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  timer40_int          : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  timer41_int          : 1;  /* bit[17]: 含义同bit0。 */
        unsigned int  timer50_int          : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  timer51_int          : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  timer60_int          : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  timer61_int          : 1;  /* bit[21]: 含义同bit0。 */
        unsigned int  timer70_int          : 1;  /* bit[22]: 含义同bit0。 */
        unsigned int  timer71_int          : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  timer80_int          : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  timer81_int          : 1;  /* bit[25]: 含义同bit0。 */
        unsigned int  intr_ipc_arm         : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  intr_asp_watchdog    : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  intr_iomcu_wdog      : 1;  /* bit[28]: 含义同bit0。 */
        unsigned int  intr_iomcu_gpio_comb : 1;  /* bit[29]: 屏蔽IOMCU GPIO中断唤醒晶振和系统。 */
        unsigned int  intr_wakeup_iomcu    : 1;  /* bit[30]: 接集成intr_ao_wakeup0
                                                             IOMCU 软件唤醒中断的屏蔽位； */
        unsigned int  reserved_3           : 1;  /* bit[31]:  */
    } reg;
} SOC_SCTRL_SCINT_MASK_UNION;
#endif
#define SOC_SCTRL_SCINT_MASK_intr_fer0_pmu_gic_START     (3)
#define SOC_SCTRL_SCINT_MASK_intr_fer0_pmu_gic_END       (3)
#define SOC_SCTRL_SCINT_MASK_intr_fer1_pmu_gic_START     (4)
#define SOC_SCTRL_SCINT_MASK_intr_fer1_pmu_gic_END       (4)
#define SOC_SCTRL_SCINT_MASK_intr_fer2_pmu_gic_START     (5)
#define SOC_SCTRL_SCINT_MASK_intr_fer2_pmu_gic_END       (5)
#define SOC_SCTRL_SCINT_MASK_rtc0_int_START              (6)
#define SOC_SCTRL_SCINT_MASK_rtc0_int_END                (6)
#define SOC_SCTRL_SCINT_MASK_rtc1_int_START              (7)
#define SOC_SCTRL_SCINT_MASK_rtc1_int_END                (7)
#define SOC_SCTRL_SCINT_MASK_timer00_int_START           (8)
#define SOC_SCTRL_SCINT_MASK_timer00_int_END             (8)
#define SOC_SCTRL_SCINT_MASK_timer01_int_START           (9)
#define SOC_SCTRL_SCINT_MASK_timer01_int_END             (9)
#define SOC_SCTRL_SCINT_MASK_timer10_int_START           (10)
#define SOC_SCTRL_SCINT_MASK_timer10_int_END             (10)
#define SOC_SCTRL_SCINT_MASK_timer11_int_START           (11)
#define SOC_SCTRL_SCINT_MASK_timer11_int_END             (11)
#define SOC_SCTRL_SCINT_MASK_timer20_int_START           (12)
#define SOC_SCTRL_SCINT_MASK_timer20_int_END             (12)
#define SOC_SCTRL_SCINT_MASK_timer21_int_START           (13)
#define SOC_SCTRL_SCINT_MASK_timer21_int_END             (13)
#define SOC_SCTRL_SCINT_MASK_timer30_int_START           (14)
#define SOC_SCTRL_SCINT_MASK_timer30_int_END             (14)
#define SOC_SCTRL_SCINT_MASK_timer31_int_START           (15)
#define SOC_SCTRL_SCINT_MASK_timer31_int_END             (15)
#define SOC_SCTRL_SCINT_MASK_timer40_int_START           (16)
#define SOC_SCTRL_SCINT_MASK_timer40_int_END             (16)
#define SOC_SCTRL_SCINT_MASK_timer41_int_START           (17)
#define SOC_SCTRL_SCINT_MASK_timer41_int_END             (17)
#define SOC_SCTRL_SCINT_MASK_timer50_int_START           (18)
#define SOC_SCTRL_SCINT_MASK_timer50_int_END             (18)
#define SOC_SCTRL_SCINT_MASK_timer51_int_START           (19)
#define SOC_SCTRL_SCINT_MASK_timer51_int_END             (19)
#define SOC_SCTRL_SCINT_MASK_timer60_int_START           (20)
#define SOC_SCTRL_SCINT_MASK_timer60_int_END             (20)
#define SOC_SCTRL_SCINT_MASK_timer61_int_START           (21)
#define SOC_SCTRL_SCINT_MASK_timer61_int_END             (21)
#define SOC_SCTRL_SCINT_MASK_timer70_int_START           (22)
#define SOC_SCTRL_SCINT_MASK_timer70_int_END             (22)
#define SOC_SCTRL_SCINT_MASK_timer71_int_START           (23)
#define SOC_SCTRL_SCINT_MASK_timer71_int_END             (23)
#define SOC_SCTRL_SCINT_MASK_timer80_int_START           (24)
#define SOC_SCTRL_SCINT_MASK_timer80_int_END             (24)
#define SOC_SCTRL_SCINT_MASK_timer81_int_START           (25)
#define SOC_SCTRL_SCINT_MASK_timer81_int_END             (25)
#define SOC_SCTRL_SCINT_MASK_intr_ipc_arm_START          (26)
#define SOC_SCTRL_SCINT_MASK_intr_ipc_arm_END            (26)
#define SOC_SCTRL_SCINT_MASK_intr_asp_watchdog_START     (27)
#define SOC_SCTRL_SCINT_MASK_intr_asp_watchdog_END       (27)
#define SOC_SCTRL_SCINT_MASK_intr_iomcu_wdog_START       (28)
#define SOC_SCTRL_SCINT_MASK_intr_iomcu_wdog_END         (28)
#define SOC_SCTRL_SCINT_MASK_intr_iomcu_gpio_comb_START  (29)
#define SOC_SCTRL_SCINT_MASK_intr_iomcu_gpio_comb_END    (29)
#define SOC_SCTRL_SCINT_MASK_intr_wakeup_iomcu_START     (30)
#define SOC_SCTRL_SCINT_MASK_intr_wakeup_iomcu_END       (30)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_STAT_UNION
 struct description   : SCINT_STAT Register structure definition
                        Address Offset:0x0A8 Initial:0x0 Width:32
 register description : intr_ao_wakeup0原始唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 1;  /* bit[0] : 中断状态寄存器。
                                                             0：中断无效；
                                                             1：中断有效。 */
        unsigned int  reserved_1           : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_2           : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  intr_fer0_pmu_gic    : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  intr_fer1_pmu_gic    : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  intr_fer2_pmu_gic    : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  rtc0_int             : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  rtc1_int             : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  timer00_int          : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  timer01_int          : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  timer10_int          : 1;  /* bit[10]: 含义同bit0。 */
        unsigned int  timer11_int          : 1;  /* bit[11]: 含义同bit0。 */
        unsigned int  timer20_int          : 1;  /* bit[12]: 含义同bit0。 */
        unsigned int  timer21_int          : 1;  /* bit[13]: 含义同bit0。 */
        unsigned int  timer30_int          : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  timer31_int          : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  timer40_int          : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  timer41_int          : 1;  /* bit[17]: 含义同bit0。 */
        unsigned int  timer50_int          : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  timer51_int          : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  timer60_int          : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  timer61_int          : 1;  /* bit[21]: 含义同bit0。 */
        unsigned int  timer70_int          : 1;  /* bit[22]: 含义同bit0。 */
        unsigned int  timer71_int          : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  timer80_int          : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  timer81_int          : 1;  /* bit[25]: 含义同bit0。 */
        unsigned int  intr_ipc_arm         : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  intr_asp_watchdog    : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  intr_iomcu_wdog      : 1;  /* bit[28]: 含义同bit0。 */
        unsigned int  intr_iomcu_gpio_comb : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  intr_wakeup_iomcu    : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_3           : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_STAT_UNION;
#endif
#define SOC_SCTRL_SCINT_STAT_intr_fer0_pmu_gic_START     (3)
#define SOC_SCTRL_SCINT_STAT_intr_fer0_pmu_gic_END       (3)
#define SOC_SCTRL_SCINT_STAT_intr_fer1_pmu_gic_START     (4)
#define SOC_SCTRL_SCINT_STAT_intr_fer1_pmu_gic_END       (4)
#define SOC_SCTRL_SCINT_STAT_intr_fer2_pmu_gic_START     (5)
#define SOC_SCTRL_SCINT_STAT_intr_fer2_pmu_gic_END       (5)
#define SOC_SCTRL_SCINT_STAT_rtc0_int_START              (6)
#define SOC_SCTRL_SCINT_STAT_rtc0_int_END                (6)
#define SOC_SCTRL_SCINT_STAT_rtc1_int_START              (7)
#define SOC_SCTRL_SCINT_STAT_rtc1_int_END                (7)
#define SOC_SCTRL_SCINT_STAT_timer00_int_START           (8)
#define SOC_SCTRL_SCINT_STAT_timer00_int_END             (8)
#define SOC_SCTRL_SCINT_STAT_timer01_int_START           (9)
#define SOC_SCTRL_SCINT_STAT_timer01_int_END             (9)
#define SOC_SCTRL_SCINT_STAT_timer10_int_START           (10)
#define SOC_SCTRL_SCINT_STAT_timer10_int_END             (10)
#define SOC_SCTRL_SCINT_STAT_timer11_int_START           (11)
#define SOC_SCTRL_SCINT_STAT_timer11_int_END             (11)
#define SOC_SCTRL_SCINT_STAT_timer20_int_START           (12)
#define SOC_SCTRL_SCINT_STAT_timer20_int_END             (12)
#define SOC_SCTRL_SCINT_STAT_timer21_int_START           (13)
#define SOC_SCTRL_SCINT_STAT_timer21_int_END             (13)
#define SOC_SCTRL_SCINT_STAT_timer30_int_START           (14)
#define SOC_SCTRL_SCINT_STAT_timer30_int_END             (14)
#define SOC_SCTRL_SCINT_STAT_timer31_int_START           (15)
#define SOC_SCTRL_SCINT_STAT_timer31_int_END             (15)
#define SOC_SCTRL_SCINT_STAT_timer40_int_START           (16)
#define SOC_SCTRL_SCINT_STAT_timer40_int_END             (16)
#define SOC_SCTRL_SCINT_STAT_timer41_int_START           (17)
#define SOC_SCTRL_SCINT_STAT_timer41_int_END             (17)
#define SOC_SCTRL_SCINT_STAT_timer50_int_START           (18)
#define SOC_SCTRL_SCINT_STAT_timer50_int_END             (18)
#define SOC_SCTRL_SCINT_STAT_timer51_int_START           (19)
#define SOC_SCTRL_SCINT_STAT_timer51_int_END             (19)
#define SOC_SCTRL_SCINT_STAT_timer60_int_START           (20)
#define SOC_SCTRL_SCINT_STAT_timer60_int_END             (20)
#define SOC_SCTRL_SCINT_STAT_timer61_int_START           (21)
#define SOC_SCTRL_SCINT_STAT_timer61_int_END             (21)
#define SOC_SCTRL_SCINT_STAT_timer70_int_START           (22)
#define SOC_SCTRL_SCINT_STAT_timer70_int_END             (22)
#define SOC_SCTRL_SCINT_STAT_timer71_int_START           (23)
#define SOC_SCTRL_SCINT_STAT_timer71_int_END             (23)
#define SOC_SCTRL_SCINT_STAT_timer80_int_START           (24)
#define SOC_SCTRL_SCINT_STAT_timer80_int_END             (24)
#define SOC_SCTRL_SCINT_STAT_timer81_int_START           (25)
#define SOC_SCTRL_SCINT_STAT_timer81_int_END             (25)
#define SOC_SCTRL_SCINT_STAT_intr_ipc_arm_START          (26)
#define SOC_SCTRL_SCINT_STAT_intr_ipc_arm_END            (26)
#define SOC_SCTRL_SCINT_STAT_intr_asp_watchdog_START     (27)
#define SOC_SCTRL_SCINT_STAT_intr_asp_watchdog_END       (27)
#define SOC_SCTRL_SCINT_STAT_intr_iomcu_wdog_START       (28)
#define SOC_SCTRL_SCINT_STAT_intr_iomcu_wdog_END         (28)
#define SOC_SCTRL_SCINT_STAT_intr_iomcu_gpio_comb_START  (29)
#define SOC_SCTRL_SCINT_STAT_intr_iomcu_gpio_comb_END    (29)
#define SOC_SCTRL_SCINT_STAT_intr_wakeup_iomcu_START     (30)
#define SOC_SCTRL_SCINT_STAT_intr_wakeup_iomcu_END       (30)


/*****************************************************************************
 struct               : SOC_SCTRL_SCDRX_INT_CFG_UNION
 struct description   : SCDRX_INT_CFG Register structure definition
                        Address Offset:0x0AC Initial:0x0 Width:32
 register description : DRX唤醒中断分配配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  drx0_int             : 1;  /* bit[0]    : ldrx2armlte_drx_arm_wakeup_int
                                                                中断配置寄存器。
                                                                0：属于DRX0的中断；
                                                                1：属于DRX1的中断。 */
        unsigned int  drx1_int             : 1;  /* bit[1]    : tdrx2armtds_drx_arm_wakeup_int，含义同bit0。 */
        unsigned int  drx2_int             : 1;  /* bit[2]    : g1_int_bbp_to_cpu_on，含义同bit0。 */
        unsigned int  drx3_int             : 1;  /* bit[3]    : g2_int_bbp_to_cpu_on，含义同bit0 */
        unsigned int  drx4_int             : 1;  /* bit[4]    : intr_wbbp_arm_int02，含义同bit0。 */
        unsigned int  reserved_0           : 1;  /* bit[5]    :  */
        unsigned int  drx5_int             : 1;  /* bit[6]    : intr_cbbp_int01，含义同bit0。 */
        unsigned int  drx6_int             : 1;  /* bit[7]    : g3_int_bbp_to_cpu_on，含义同bit0 */
        unsigned int  drx7_int             : 1;  /* bit[8]    : intr_wbbp_arm_int02_2，含义同bit0。 */
        unsigned int  drx8_int             : 1;  /* bit[9]    : lte2_drx_arm_wakeup_int，含义同bit0。 */
        unsigned int  reserved_1           : 21; /* bit[10-30]:  */
        unsigned int  intr_iomcu_gpio_comb : 1;  /* bit[31]   :  */
    } reg;
} SOC_SCTRL_SCDRX_INT_CFG_UNION;
#endif
#define SOC_SCTRL_SCDRX_INT_CFG_drx0_int_START              (0)
#define SOC_SCTRL_SCDRX_INT_CFG_drx0_int_END                (0)
#define SOC_SCTRL_SCDRX_INT_CFG_drx1_int_START              (1)
#define SOC_SCTRL_SCDRX_INT_CFG_drx1_int_END                (1)
#define SOC_SCTRL_SCDRX_INT_CFG_drx2_int_START              (2)
#define SOC_SCTRL_SCDRX_INT_CFG_drx2_int_END                (2)
#define SOC_SCTRL_SCDRX_INT_CFG_drx3_int_START              (3)
#define SOC_SCTRL_SCDRX_INT_CFG_drx3_int_END                (3)
#define SOC_SCTRL_SCDRX_INT_CFG_drx4_int_START              (4)
#define SOC_SCTRL_SCDRX_INT_CFG_drx4_int_END                (4)
#define SOC_SCTRL_SCDRX_INT_CFG_drx5_int_START              (6)
#define SOC_SCTRL_SCDRX_INT_CFG_drx5_int_END                (6)
#define SOC_SCTRL_SCDRX_INT_CFG_drx6_int_START              (7)
#define SOC_SCTRL_SCDRX_INT_CFG_drx6_int_END                (7)
#define SOC_SCTRL_SCDRX_INT_CFG_drx7_int_START              (8)
#define SOC_SCTRL_SCDRX_INT_CFG_drx7_int_END                (8)
#define SOC_SCTRL_SCDRX_INT_CFG_drx8_int_START              (9)
#define SOC_SCTRL_SCDRX_INT_CFG_drx8_int_END                (9)
#define SOC_SCTRL_SCDRX_INT_CFG_intr_iomcu_gpio_comb_START  (31)
#define SOC_SCTRL_SCDRX_INT_CFG_intr_iomcu_gpio_comb_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCUWFI_INT_UNION
 struct description   : SCLPMCUWFI_INT Register structure definition
                        Address Offset:0x0B0 Initial:0x00000000 Width:32
 register description : LPMCU WFI唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpmcu_wfi_int : 1;  /* bit[0]   : 写操作清除LPMCU_wfi_int中断；
                                                        读操作返回LPMCU_wfi_int中断状态。 */
        unsigned int  reserved      : 31; /* bit[1-31]: SYS_MISC有逻辑处理，注意修改 */
    } reg;
} SOC_SCTRL_SCLPMCUWFI_INT_UNION;
#endif
#define SOC_SCTRL_SCLPMCUWFI_INT_lpmcu_wfi_int_START  (0)
#define SOC_SCTRL_SCLPMCUWFI_INT_lpmcu_wfi_int_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_MASK1_UNION
 struct description   : SCINT_MASK1 Register structure definition
                        Address Offset:0x0B4 Initial:0x0 Width:32
 register description : intr_ao_wakeup1唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                0：屏蔽有效；
                                                                1：屏蔽无效。 */
        unsigned int  reserved_1           : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  reserved_2           : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  reserved_3           : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  reserved_4           : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  reserved_5           : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_6           : 1;  /* bit[6]    :  */
        unsigned int  reserved_7           : 1;  /* bit[7]    :  */
        unsigned int  reserved_8           : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_9           : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_10          : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_mspc  : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_iomcu : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_asp   : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_ao    : 1;  /* bit[14]   :  */
        unsigned int  ao_ipc_mdm           : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  ao_ipc_gic           : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  ao_ipc_lpmcu         : 1;  /* bit[17]   : 含义同bit0。 */
        unsigned int  reserved_11          : 1;  /* bit[18]   : 含义同bit0。 */
        unsigned int  ao_ipc_asp           : 1;  /* bit[19]   : 含义同bit0。 */
        unsigned int  reserved_12          : 1;  /* bit[20]   : 含义同bit0。 */
        unsigned int  ao_ipc_iomcu         : 1;  /* bit[21]   : 含义同bit0。 */
        unsigned int  reserved_13          : 1;  /* bit[22]   : 含义同bit0。 */
        unsigned int  reserved_14          : 1;  /* bit[23]   :  */
        unsigned int  reserved_15          : 1;  /* bit[24]   :  */
        unsigned int  reserved_16          : 1;  /* bit[25]   :  */
        unsigned int  reserved_17          : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  reserved_18          : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  ao_ioc               : 1;  /* bit[28-28]: 含义同bit0。 */
        unsigned int  se_gpio1             : 1;  /* bit[29-29]: 含义同bit0。 */
        unsigned int  mad_int              : 1;  /* bit[30-30]:  */
        unsigned int  intr_ao_wd           : 1;  /* bit[31-31]: 接集成intr_ao_wakeup1
                                                                含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_MASK1_UNION;
#endif
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_mspc_START   (11)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_mspc_END     (11)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_iomcu_START  (12)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_iomcu_END    (12)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_asp_START    (13)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_asp_END      (13)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_ao_START     (14)
#define SOC_SCTRL_SCINT_MASK1_intr_qic_probe_ao_END       (14)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_mdm_START            (15)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_mdm_END              (15)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_gic_START            (16)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_gic_END              (16)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_lpmcu_START          (17)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_lpmcu_END            (17)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_asp_START            (19)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_asp_END              (19)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_iomcu_START          (21)
#define SOC_SCTRL_SCINT_MASK1_ao_ipc_iomcu_END            (21)
#define SOC_SCTRL_SCINT_MASK1_ao_ioc_START                (28)
#define SOC_SCTRL_SCINT_MASK1_ao_ioc_END                  (28)
#define SOC_SCTRL_SCINT_MASK1_se_gpio1_START              (29)
#define SOC_SCTRL_SCINT_MASK1_se_gpio1_END                (29)
#define SOC_SCTRL_SCINT_MASK1_mad_int_START               (30)
#define SOC_SCTRL_SCINT_MASK1_mad_int_END                 (30)
#define SOC_SCTRL_SCINT_MASK1_intr_ao_wd_START            (31)
#define SOC_SCTRL_SCINT_MASK1_intr_ao_wd_END              (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_STAT1_UNION
 struct description   : SCINT_STAT1 Register structure definition
                        Address Offset:0x0B8 Initial:0x0 Width:32
 register description : intr_ao_wakeup1原始唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                0：屏蔽有效；
                                                                1：屏蔽无效。 */
        unsigned int  reserved_1           : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  reserved_2           : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  reserved_3           : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  reserved_4           : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  reserved_5           : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_6           : 1;  /* bit[6]    :  */
        unsigned int  reserved_7           : 1;  /* bit[7]    :  */
        unsigned int  reserved_8           : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_9           : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_10          : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_mspc  : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_iomcu : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_asp   : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  intr_qic_probe_ao    : 1;  /* bit[14]   :  */
        unsigned int  ao_ipc_mdm           : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  ao_ipc_gic           : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  ao_ipc_lpmcu         : 1;  /* bit[17]   : 含义同bit0。 */
        unsigned int  reserved_11          : 1;  /* bit[18]   : 含义同bit0。 */
        unsigned int  ao_ipc_asp           : 1;  /* bit[19]   : 含义同bit0。 */
        unsigned int  reserved_12          : 1;  /* bit[20]   : 含义同bit0。 */
        unsigned int  ao_ipc_iomcu         : 1;  /* bit[21]   : 含义同bit0。 */
        unsigned int  reserved_13          : 1;  /* bit[22]   : 含义同bit0。 */
        unsigned int  reserved_14          : 1;  /* bit[23]   :  */
        unsigned int  reserved_15          : 1;  /* bit[24]   :  */
        unsigned int  reserved_16          : 1;  /* bit[25]   :  */
        unsigned int  reserved_17          : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  reserved_18          : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  ao_ioc               : 1;  /* bit[28-28]: 含义同bit0。 */
        unsigned int  se_gpio1             : 1;  /* bit[29-29]: 含义同bit0。 */
        unsigned int  mad_int              : 1;  /* bit[30-30]: 含义同bit0。 */
        unsigned int  intr_ao_wd           : 1;  /* bit[31-31]: 含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_STAT1_UNION;
#endif
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_mspc_START   (11)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_mspc_END     (11)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_iomcu_START  (12)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_iomcu_END    (12)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_asp_START    (13)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_asp_END      (13)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_ao_START     (14)
#define SOC_SCTRL_SCINT_STAT1_intr_qic_probe_ao_END       (14)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_mdm_START            (15)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_mdm_END              (15)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_gic_START            (16)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_gic_END              (16)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_lpmcu_START          (17)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_lpmcu_END            (17)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_asp_START            (19)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_asp_END              (19)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_iomcu_START          (21)
#define SOC_SCTRL_SCINT_STAT1_ao_ipc_iomcu_END            (21)
#define SOC_SCTRL_SCINT_STAT1_ao_ioc_START                (28)
#define SOC_SCTRL_SCINT_STAT1_ao_ioc_END                  (28)
#define SOC_SCTRL_SCINT_STAT1_se_gpio1_START              (29)
#define SOC_SCTRL_SCINT_STAT1_se_gpio1_END                (29)
#define SOC_SCTRL_SCINT_STAT1_mad_int_START               (30)
#define SOC_SCTRL_SCINT_STAT1_mad_int_END                 (30)
#define SOC_SCTRL_SCINT_STAT1_intr_ao_wd_START            (31)
#define SOC_SCTRL_SCINT_STAT1_intr_ao_wd_END              (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_MASK2_UNION
 struct description   : SCINT_MASK2 Register structure definition
                        Address Offset:0x0BC Initial:0x000000 Width:32
 register description : intr_peri_wakeup唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gic_irq_int0    : 1;  /* bit[0]   : 中断屏蔽寄存器。
                                                          0：屏蔽有效；
                                                          1：屏蔽无效。 */
        unsigned int  gic_irq_int1    : 1;  /* bit[1]   : 含义同bit0。 */
        unsigned int  gic_irq_int2    : 1;  /* bit[2]   : 含义同bit0。 */
        unsigned int  intr_tsp        : 1;  /* bit[3]   : 含义同bit0。 */
        unsigned int  intr_lpmcu_wdog : 1;  /* bit[4]   : 含义同bit0。 */
        unsigned int  lpmcu_wfi_int   : 1;  /* bit[5]   : 含义同bit0。 */
        unsigned int  acpu_ppi_wakeup : 1;  /* bit[6]   : 含义同bit0。 */
        unsigned int  reserved_0      : 1;  /* bit[7]   : 含义同bit0。 */
        unsigned int  reserved_1      : 24; /* bit[8-31]: 接集成intr_peri_wakeup
                                                          保留。 */
    } reg;
} SOC_SCTRL_SCINT_MASK2_UNION;
#endif
#define SOC_SCTRL_SCINT_MASK2_gic_irq_int0_START     (0)
#define SOC_SCTRL_SCINT_MASK2_gic_irq_int0_END       (0)
#define SOC_SCTRL_SCINT_MASK2_gic_irq_int1_START     (1)
#define SOC_SCTRL_SCINT_MASK2_gic_irq_int1_END       (1)
#define SOC_SCTRL_SCINT_MASK2_gic_irq_int2_START     (2)
#define SOC_SCTRL_SCINT_MASK2_gic_irq_int2_END       (2)
#define SOC_SCTRL_SCINT_MASK2_intr_tsp_START         (3)
#define SOC_SCTRL_SCINT_MASK2_intr_tsp_END           (3)
#define SOC_SCTRL_SCINT_MASK2_intr_lpmcu_wdog_START  (4)
#define SOC_SCTRL_SCINT_MASK2_intr_lpmcu_wdog_END    (4)
#define SOC_SCTRL_SCINT_MASK2_lpmcu_wfi_int_START    (5)
#define SOC_SCTRL_SCINT_MASK2_lpmcu_wfi_int_END      (5)
#define SOC_SCTRL_SCINT_MASK2_acpu_ppi_wakeup_START  (6)
#define SOC_SCTRL_SCINT_MASK2_acpu_ppi_wakeup_END    (6)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_STAT2_UNION
 struct description   : SCINT_STAT2 Register structure definition
                        Address Offset:0x0C0 Initial:0x000000 Width:32
 register description : intr_peri_wakeup原始唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gic_irq_int0    : 1;  /* bit[0]   : 中断状态寄存器。
                                                          0：中断无效；
                                                          1：中断有效。 */
        unsigned int  gic_irq_int1    : 1;  /* bit[1]   : 含义同bit0。 */
        unsigned int  gic_irq_int2    : 1;  /* bit[2]   : 含义同bit0。 */
        unsigned int  intr_tsp        : 1;  /* bit[3]   : 含义同bit0。 */
        unsigned int  intr_lpmcu_wdog : 1;  /* bit[4]   : 含义同bit0。 */
        unsigned int  reserved_0      : 1;  /* bit[5]   : 含义同bit0。 */
        unsigned int  acpu_ppi_wakeup : 1;  /* bit[6]   : 含义同bit0。 */
        unsigned int  reserved_1      : 1;  /* bit[7]   : 含义同bit0。 */
        unsigned int  reserved_2      : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCINT_STAT2_UNION;
#endif
#define SOC_SCTRL_SCINT_STAT2_gic_irq_int0_START     (0)
#define SOC_SCTRL_SCINT_STAT2_gic_irq_int0_END       (0)
#define SOC_SCTRL_SCINT_STAT2_gic_irq_int1_START     (1)
#define SOC_SCTRL_SCINT_STAT2_gic_irq_int1_END       (1)
#define SOC_SCTRL_SCINT_STAT2_gic_irq_int2_START     (2)
#define SOC_SCTRL_SCINT_STAT2_gic_irq_int2_END       (2)
#define SOC_SCTRL_SCINT_STAT2_intr_tsp_START         (3)
#define SOC_SCTRL_SCINT_STAT2_intr_tsp_END           (3)
#define SOC_SCTRL_SCINT_STAT2_intr_lpmcu_wdog_START  (4)
#define SOC_SCTRL_SCINT_STAT2_intr_lpmcu_wdog_END    (4)
#define SOC_SCTRL_SCINT_STAT2_acpu_ppi_wakeup_START  (6)
#define SOC_SCTRL_SCINT_STAT2_acpu_ppi_wakeup_END    (6)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_MASK3_UNION
 struct description   : SCINT_MASK3 Register structure definition
                        Address Offset:0x0C4 Initial:0x0 Width:32
 register description : intr_peri_wakeup0唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_peri_gpio_wakeup     : 5;  /* bit[0-4]  : 中断屏蔽寄存器。
                                                                     0：屏蔽有效；
                                                                     1：屏蔽无效。 */
        unsigned int  intr_ddrc_fatal_gic_lpmcu : 4;  /* bit[5-8]  : 含义同bit0。 */
        unsigned int  reserved_0                : 4;  /* bit[9-12] : 含义同bit0。 */
        unsigned int  reserved_1                : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  reserved_2                : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  reserved_3                : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  reserved_4                : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  reserved_5                : 2;  /* bit[17-18]: 含义同bit0。 */
        unsigned int  reserved_6                : 2;  /* bit[19-20]: 含义同bit0。 */
        unsigned int  reserved_7                : 2;  /* bit[21-22]: 含义同bit0。 */
        unsigned int  reserved_8                : 1;  /* bit[23]   : 含义同bit0。 */
        unsigned int  vad_int                   : 1;  /* bit[24]   : 含义同bit0。 */
        unsigned int  reserved_9                : 1;  /* bit[25]   : 含义同bit0。 */
        unsigned int  reserved_10               : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  reserved_12               : 1;  /* bit[28]   : 含义同bit0。 */
        unsigned int  reserved_13               : 1;  /* bit[29]   : 含义同bit0。 */
        unsigned int  reserved_14               : 1;  /* bit[30]   : 含义同bit0。 */
        unsigned int  reserved_15               : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_SCTRL_SCINT_MASK3_UNION;
#endif
#define SOC_SCTRL_SCINT_MASK3_intr_peri_gpio_wakeup_START      (0)
#define SOC_SCTRL_SCINT_MASK3_intr_peri_gpio_wakeup_END        (4)
#define SOC_SCTRL_SCINT_MASK3_intr_ddrc_fatal_gic_lpmcu_START  (5)
#define SOC_SCTRL_SCINT_MASK3_intr_ddrc_fatal_gic_lpmcu_END    (8)
#define SOC_SCTRL_SCINT_MASK3_vad_int_START                    (24)
#define SOC_SCTRL_SCINT_MASK3_vad_int_END                      (24)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_STAT3_UNION
 struct description   : SCINT_STAT3 Register structure definition
                        Address Offset:0x0C8 Initial:0x0 Width:32
 register description : intr_peri_wakeup0原始唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_peri_gpio_wakeup     : 5;  /* bit[0-4]  : 中断状态寄存器。
                                                                     0：中断无效；
                                                                     1：中断有效。 */
        unsigned int  intr_ddrc_fatal_gic_lpmcu : 4;  /* bit[5-8]  : 含义同bit0。bit[8:7]指示ddr_dc/d无用； */
        unsigned int  reserved_0                : 4;  /* bit[9-12] : 含义同bit0。 */
        unsigned int  reserved_1                : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  reserved_2                : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  reserved_3                : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  reserved_4                : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  reserved_5                : 2;  /* bit[17-18]: 含义同bit0。 */
        unsigned int  reserved_6                : 2;  /* bit[19-20]: 含义同bit0。 */
        unsigned int  reserved_7                : 2;  /* bit[21-22]: 含义同bit0。 */
        unsigned int  reserved_8                : 1;  /* bit[23]   : 含义同bit0。 */
        unsigned int  vad_int                   : 1;  /* bit[24]   : 含义同bit0。 */
        unsigned int  reserved_9                : 1;  /* bit[25]   : 含义同bit0。 */
        unsigned int  reserved_10               : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  reserved_12               : 1;  /* bit[28]   : 含义同bit0。 */
        unsigned int  reserved_13               : 1;  /* bit[29]   : 含义同bit0。 */
        unsigned int  reserved_14               : 1;  /* bit[30]   : 含义同bit0。 */
        unsigned int  reserved_15               : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_SCTRL_SCINT_STAT3_UNION;
#endif
#define SOC_SCTRL_SCINT_STAT3_intr_peri_gpio_wakeup_START      (0)
#define SOC_SCTRL_SCINT_STAT3_intr_peri_gpio_wakeup_END        (4)
#define SOC_SCTRL_SCINT_STAT3_intr_ddrc_fatal_gic_lpmcu_START  (5)
#define SOC_SCTRL_SCINT_STAT3_intr_ddrc_fatal_gic_lpmcu_END    (8)
#define SOC_SCTRL_SCINT_STAT3_vad_int_START                    (24)
#define SOC_SCTRL_SCINT_STAT3_vad_int_END                      (24)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_MASK4_UNION
 struct description   : SCINT_MASK4 Register structure definition
                        Address Offset:0x090 Initial:0x0 Width:32
 register description : intr_drx_wakeup唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lte_drx_arm_wakeup_int  : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                   0：屏蔽有效；
                                                                   1：屏蔽无效。 */
        unsigned int  nrv_drx_arm_wakeup_int  : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  g1_int_gbbp_to_cpu_on   : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  g2_int_gbbp_to_cpu_on   : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  w_cpu_int02             : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  intr_sdrx_wakeup        : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  g3_int_gbbp_to_cpu_on   : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  w_cpu_int02_2           : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  lte2_drx_arm_wakeup_int : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  ltev_drx_arm_wakeup_int : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  nr_drx_arm_wakeup_int   : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  nr2_drx_arm_wakeup_int  : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  scg_drx_arm_wakeup_int  : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  tsp0_wakeup_intr        : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  modem_doze1_wake        : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  pdea_wakeup_intr_0      : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  pdea_wakeup_intr_1      : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  reserved_0              : 1;  /* bit[17]   : 含义同bit0。 */
        unsigned int  reserved_1              : 1;  /* bit[18]   : 含义同bit0。 */
        unsigned int  reserved_2              : 1;  /* bit[19]   : 含义同bit0。 */
        unsigned int  reserved_3              : 1;  /* bit[20]   : 含义同bit0。 */
        unsigned int  reserved_4              : 1;  /* bit[21]   : 含义同bit0。 */
        unsigned int  reserved_5              : 1;  /* bit[22]   : 含义同bit0。 */
        unsigned int  reserved_6              : 1;  /* bit[23]   :  */
        unsigned int  reserved_7              : 1;  /* bit[24]   :  */
        unsigned int  reserved_8              : 1;  /* bit[25]   :  */
        unsigned int  reserved_9              : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  reserved_10             : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  reserved_11             : 1;  /* bit[28-28]: 含义同bit0。 */
        unsigned int  reserved_12             : 1;  /* bit[29-29]: 含义同bit0。 */
        unsigned int  reserved_13             : 1;  /* bit[30-30]:  */
        unsigned int  reserved_14             : 1;  /* bit[31-31]: 接集成intr_drx_wakeup[30:0], bit[31]不使用。
                                                                   含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_MASK4_UNION;
#endif
#define SOC_SCTRL_SCINT_MASK4_lte_drx_arm_wakeup_int_START   (0)
#define SOC_SCTRL_SCINT_MASK4_lte_drx_arm_wakeup_int_END     (0)
#define SOC_SCTRL_SCINT_MASK4_nrv_drx_arm_wakeup_int_START   (1)
#define SOC_SCTRL_SCINT_MASK4_nrv_drx_arm_wakeup_int_END     (1)
#define SOC_SCTRL_SCINT_MASK4_g1_int_gbbp_to_cpu_on_START    (2)
#define SOC_SCTRL_SCINT_MASK4_g1_int_gbbp_to_cpu_on_END      (2)
#define SOC_SCTRL_SCINT_MASK4_g2_int_gbbp_to_cpu_on_START    (3)
#define SOC_SCTRL_SCINT_MASK4_g2_int_gbbp_to_cpu_on_END      (3)
#define SOC_SCTRL_SCINT_MASK4_w_cpu_int02_START              (4)
#define SOC_SCTRL_SCINT_MASK4_w_cpu_int02_END                (4)
#define SOC_SCTRL_SCINT_MASK4_intr_sdrx_wakeup_START         (5)
#define SOC_SCTRL_SCINT_MASK4_intr_sdrx_wakeup_END           (5)
#define SOC_SCTRL_SCINT_MASK4_g3_int_gbbp_to_cpu_on_START    (6)
#define SOC_SCTRL_SCINT_MASK4_g3_int_gbbp_to_cpu_on_END      (6)
#define SOC_SCTRL_SCINT_MASK4_w_cpu_int02_2_START            (7)
#define SOC_SCTRL_SCINT_MASK4_w_cpu_int02_2_END              (7)
#define SOC_SCTRL_SCINT_MASK4_lte2_drx_arm_wakeup_int_START  (8)
#define SOC_SCTRL_SCINT_MASK4_lte2_drx_arm_wakeup_int_END    (8)
#define SOC_SCTRL_SCINT_MASK4_ltev_drx_arm_wakeup_int_START  (9)
#define SOC_SCTRL_SCINT_MASK4_ltev_drx_arm_wakeup_int_END    (9)
#define SOC_SCTRL_SCINT_MASK4_nr_drx_arm_wakeup_int_START    (10)
#define SOC_SCTRL_SCINT_MASK4_nr_drx_arm_wakeup_int_END      (10)
#define SOC_SCTRL_SCINT_MASK4_nr2_drx_arm_wakeup_int_START   (11)
#define SOC_SCTRL_SCINT_MASK4_nr2_drx_arm_wakeup_int_END     (11)
#define SOC_SCTRL_SCINT_MASK4_scg_drx_arm_wakeup_int_START   (12)
#define SOC_SCTRL_SCINT_MASK4_scg_drx_arm_wakeup_int_END     (12)
#define SOC_SCTRL_SCINT_MASK4_tsp0_wakeup_intr_START         (13)
#define SOC_SCTRL_SCINT_MASK4_tsp0_wakeup_intr_END           (13)
#define SOC_SCTRL_SCINT_MASK4_modem_doze1_wake_START         (14)
#define SOC_SCTRL_SCINT_MASK4_modem_doze1_wake_END           (14)
#define SOC_SCTRL_SCINT_MASK4_pdea_wakeup_intr_0_START       (15)
#define SOC_SCTRL_SCINT_MASK4_pdea_wakeup_intr_0_END         (15)
#define SOC_SCTRL_SCINT_MASK4_pdea_wakeup_intr_1_START       (16)
#define SOC_SCTRL_SCINT_MASK4_pdea_wakeup_intr_1_END         (16)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_STAT4_UNION
 struct description   : SCINT_STAT4 Register structure definition
                        Address Offset:0x094 Initial:0x0 Width:32
 register description : intr_drx_wakeup原始唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lte_drx_arm_wakeup_int  : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                   0：屏蔽有效；
                                                                   1：屏蔽无效。 */
        unsigned int  nrv_drx_arm_wakeup_int  : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  g1_int_gbbp_to_cpu_on   : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  g2_int_gbbp_to_cpu_on   : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  w_cpu_int02             : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  intr_sdrx_wakeup        : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  g3_int_gbbp_to_cpu_on   : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  w_cpu_int02_2           : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  lte2_drx_arm_wakeup_int : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  ltev_drx_arm_wakeup_int : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  nr_drx_arm_wakeup_int   : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  nr2_drx_arm_wakeup_int  : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  scg_drx_arm_wakeup_int  : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  tsp0_wakeup_intr        : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  modem_doze1_wake        : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  pdea_wakeup_intr_0      : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  pdea_wakeup_intr_1      : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  reserved_0              : 1;  /* bit[17]   : 含义同bit0。 */
        unsigned int  reserved_1              : 1;  /* bit[18]   : 含义同bit0。 */
        unsigned int  reserved_2              : 1;  /* bit[19]   : 含义同bit0。 */
        unsigned int  reserved_3              : 1;  /* bit[20]   : 含义同bit0。 */
        unsigned int  reserved_4              : 1;  /* bit[21]   : 含义同bit0。 */
        unsigned int  reserved_5              : 1;  /* bit[22]   : 含义同bit0。 */
        unsigned int  reserved_6              : 1;  /* bit[23]   :  */
        unsigned int  reserved_7              : 1;  /* bit[24]   :  */
        unsigned int  reserved_8              : 1;  /* bit[25]   :  */
        unsigned int  reserved_9              : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  reserved_10             : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  reserved_11             : 1;  /* bit[28-28]: 含义同bit0。 */
        unsigned int  reserved_12             : 1;  /* bit[29-29]: 含义同bit0。 */
        unsigned int  reserved_13             : 1;  /* bit[30-30]: 含义同bit0。 */
        unsigned int  reserved_14             : 1;  /* bit[31-31]: 含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_STAT4_UNION;
#endif
#define SOC_SCTRL_SCINT_STAT4_lte_drx_arm_wakeup_int_START   (0)
#define SOC_SCTRL_SCINT_STAT4_lte_drx_arm_wakeup_int_END     (0)
#define SOC_SCTRL_SCINT_STAT4_nrv_drx_arm_wakeup_int_START   (1)
#define SOC_SCTRL_SCINT_STAT4_nrv_drx_arm_wakeup_int_END     (1)
#define SOC_SCTRL_SCINT_STAT4_g1_int_gbbp_to_cpu_on_START    (2)
#define SOC_SCTRL_SCINT_STAT4_g1_int_gbbp_to_cpu_on_END      (2)
#define SOC_SCTRL_SCINT_STAT4_g2_int_gbbp_to_cpu_on_START    (3)
#define SOC_SCTRL_SCINT_STAT4_g2_int_gbbp_to_cpu_on_END      (3)
#define SOC_SCTRL_SCINT_STAT4_w_cpu_int02_START              (4)
#define SOC_SCTRL_SCINT_STAT4_w_cpu_int02_END                (4)
#define SOC_SCTRL_SCINT_STAT4_intr_sdrx_wakeup_START         (5)
#define SOC_SCTRL_SCINT_STAT4_intr_sdrx_wakeup_END           (5)
#define SOC_SCTRL_SCINT_STAT4_g3_int_gbbp_to_cpu_on_START    (6)
#define SOC_SCTRL_SCINT_STAT4_g3_int_gbbp_to_cpu_on_END      (6)
#define SOC_SCTRL_SCINT_STAT4_w_cpu_int02_2_START            (7)
#define SOC_SCTRL_SCINT_STAT4_w_cpu_int02_2_END              (7)
#define SOC_SCTRL_SCINT_STAT4_lte2_drx_arm_wakeup_int_START  (8)
#define SOC_SCTRL_SCINT_STAT4_lte2_drx_arm_wakeup_int_END    (8)
#define SOC_SCTRL_SCINT_STAT4_ltev_drx_arm_wakeup_int_START  (9)
#define SOC_SCTRL_SCINT_STAT4_ltev_drx_arm_wakeup_int_END    (9)
#define SOC_SCTRL_SCINT_STAT4_nr_drx_arm_wakeup_int_START    (10)
#define SOC_SCTRL_SCINT_STAT4_nr_drx_arm_wakeup_int_END      (10)
#define SOC_SCTRL_SCINT_STAT4_nr2_drx_arm_wakeup_int_START   (11)
#define SOC_SCTRL_SCINT_STAT4_nr2_drx_arm_wakeup_int_END     (11)
#define SOC_SCTRL_SCINT_STAT4_scg_drx_arm_wakeup_int_START   (12)
#define SOC_SCTRL_SCINT_STAT4_scg_drx_arm_wakeup_int_END     (12)
#define SOC_SCTRL_SCINT_STAT4_tsp0_wakeup_intr_START         (13)
#define SOC_SCTRL_SCINT_STAT4_tsp0_wakeup_intr_END           (13)
#define SOC_SCTRL_SCINT_STAT4_modem_doze1_wake_START         (14)
#define SOC_SCTRL_SCINT_STAT4_modem_doze1_wake_END           (14)
#define SOC_SCTRL_SCINT_STAT4_pdea_wakeup_intr_0_START       (15)
#define SOC_SCTRL_SCINT_STAT4_pdea_wakeup_intr_0_END         (15)
#define SOC_SCTRL_SCINT_STAT4_pdea_wakeup_intr_1_START       (16)
#define SOC_SCTRL_SCINT_STAT4_pdea_wakeup_intr_1_END         (16)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_MASK5_UNION
 struct description   : SCINT_MASK5 Register structure definition
                        Address Offset:0x098 Initial:0x0 Width:32
 register description : intr_ao_wakeup2唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_timer130                         : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                                 0：屏蔽有效；
                                                                                 1：屏蔽无效。 */
        unsigned int  intr_timer131                         : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  intr_timer140                         : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  intr_timer141                         : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  intr_timer150                         : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  intr_timer151                         : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  intr_timer160                         : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  intr_timer161                         : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  intr_timer170                         : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  intr_timer171                         : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  intr_pcie0_0_l12_wake_gic_lpmcu       : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  intr_ao_wd1                           : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  intr_wakeup_swing_enter_req           : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  intr_wakeup_swing_exit_req            : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  intr_lfc_mode_quit_to_sctrl_and_lpmcu : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  intr_usb3_sys_error_to_ao_wakeup      : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  reserved_0                            : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  intr_m7_ahb_dbg_timeout_lpmcu         : 1;  /* bit[17]   : 含义同bit0。 */
        unsigned int  intr_pcie0_1_l12_wake_gic_lpmcu       : 1;  /* bit[18]   : 含义同bit0。 */
        unsigned int  intr_pcie0_0_l2_wake_gic_lpmcu        : 1;  /* bit[19]   : 含义同bit0。 */
        unsigned int  intr_pcie0_1_l2_wake_gic_lpmcu        : 1;  /* bit[20]   : 含义同bit0。 */
        unsigned int  reserved_1                            : 1;  /* bit[21]   : 含义同bit0。 */
        unsigned int  dss_doze1_wake                        : 1;  /* bit[22]   : 含义同bit0。 */
        unsigned int  dss_doze1_wd_wake                     : 1;  /* bit[23]   : 含义同bit0。 */
        unsigned int  freq_abnormal_int_0                   : 1;  /* bit[24]   : 含义同bit0。 */
        unsigned int  mdm_pll_unlock_intr_lpmcu             : 1;  /* bit[25]   : 含义同bit0。 */
        unsigned int  mdm_wdt_intr_merge_acpu_lpmcu         : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  pdea_wdt_int                          : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  modem_bus_err                         : 1;  /* bit[28-28]: 含义同bit0。 */
        unsigned int  reserved_2                            : 1;  /* bit[29-29]: 含义同bit0。 */
        unsigned int  reserved_3                            : 1;  /* bit[30-30]: 含义同bit0。 */
        unsigned int  reserved_4                            : 1;  /* bit[31-31]: 接集成intr_ao_wakeup2[31:0]
                                                                                 含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_MASK5_UNION;
#endif
#define SOC_SCTRL_SCINT_MASK5_intr_timer130_START                          (0)
#define SOC_SCTRL_SCINT_MASK5_intr_timer130_END                            (0)
#define SOC_SCTRL_SCINT_MASK5_intr_timer131_START                          (1)
#define SOC_SCTRL_SCINT_MASK5_intr_timer131_END                            (1)
#define SOC_SCTRL_SCINT_MASK5_intr_timer140_START                          (2)
#define SOC_SCTRL_SCINT_MASK5_intr_timer140_END                            (2)
#define SOC_SCTRL_SCINT_MASK5_intr_timer141_START                          (3)
#define SOC_SCTRL_SCINT_MASK5_intr_timer141_END                            (3)
#define SOC_SCTRL_SCINT_MASK5_intr_timer150_START                          (4)
#define SOC_SCTRL_SCINT_MASK5_intr_timer150_END                            (4)
#define SOC_SCTRL_SCINT_MASK5_intr_timer151_START                          (5)
#define SOC_SCTRL_SCINT_MASK5_intr_timer151_END                            (5)
#define SOC_SCTRL_SCINT_MASK5_intr_timer160_START                          (6)
#define SOC_SCTRL_SCINT_MASK5_intr_timer160_END                            (6)
#define SOC_SCTRL_SCINT_MASK5_intr_timer161_START                          (7)
#define SOC_SCTRL_SCINT_MASK5_intr_timer161_END                            (7)
#define SOC_SCTRL_SCINT_MASK5_intr_timer170_START                          (8)
#define SOC_SCTRL_SCINT_MASK5_intr_timer170_END                            (8)
#define SOC_SCTRL_SCINT_MASK5_intr_timer171_START                          (9)
#define SOC_SCTRL_SCINT_MASK5_intr_timer171_END                            (9)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_0_l12_wake_gic_lpmcu_START        (10)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_0_l12_wake_gic_lpmcu_END          (10)
#define SOC_SCTRL_SCINT_MASK5_intr_ao_wd1_START                            (11)
#define SOC_SCTRL_SCINT_MASK5_intr_ao_wd1_END                              (11)
#define SOC_SCTRL_SCINT_MASK5_intr_wakeup_swing_enter_req_START            (12)
#define SOC_SCTRL_SCINT_MASK5_intr_wakeup_swing_enter_req_END              (12)
#define SOC_SCTRL_SCINT_MASK5_intr_wakeup_swing_exit_req_START             (13)
#define SOC_SCTRL_SCINT_MASK5_intr_wakeup_swing_exit_req_END               (13)
#define SOC_SCTRL_SCINT_MASK5_intr_lfc_mode_quit_to_sctrl_and_lpmcu_START  (14)
#define SOC_SCTRL_SCINT_MASK5_intr_lfc_mode_quit_to_sctrl_and_lpmcu_END    (14)
#define SOC_SCTRL_SCINT_MASK5_intr_usb3_sys_error_to_ao_wakeup_START       (15)
#define SOC_SCTRL_SCINT_MASK5_intr_usb3_sys_error_to_ao_wakeup_END         (15)
#define SOC_SCTRL_SCINT_MASK5_intr_m7_ahb_dbg_timeout_lpmcu_START          (17)
#define SOC_SCTRL_SCINT_MASK5_intr_m7_ahb_dbg_timeout_lpmcu_END            (17)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_1_l12_wake_gic_lpmcu_START        (18)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_1_l12_wake_gic_lpmcu_END          (18)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_0_l2_wake_gic_lpmcu_START         (19)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_0_l2_wake_gic_lpmcu_END           (19)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_1_l2_wake_gic_lpmcu_START         (20)
#define SOC_SCTRL_SCINT_MASK5_intr_pcie0_1_l2_wake_gic_lpmcu_END           (20)
#define SOC_SCTRL_SCINT_MASK5_dss_doze1_wake_START                         (22)
#define SOC_SCTRL_SCINT_MASK5_dss_doze1_wake_END                           (22)
#define SOC_SCTRL_SCINT_MASK5_dss_doze1_wd_wake_START                      (23)
#define SOC_SCTRL_SCINT_MASK5_dss_doze1_wd_wake_END                        (23)
#define SOC_SCTRL_SCINT_MASK5_freq_abnormal_int_0_START                    (24)
#define SOC_SCTRL_SCINT_MASK5_freq_abnormal_int_0_END                      (24)
#define SOC_SCTRL_SCINT_MASK5_mdm_pll_unlock_intr_lpmcu_START              (25)
#define SOC_SCTRL_SCINT_MASK5_mdm_pll_unlock_intr_lpmcu_END                (25)
#define SOC_SCTRL_SCINT_MASK5_mdm_wdt_intr_merge_acpu_lpmcu_START          (26)
#define SOC_SCTRL_SCINT_MASK5_mdm_wdt_intr_merge_acpu_lpmcu_END            (26)
#define SOC_SCTRL_SCINT_MASK5_pdea_wdt_int_START                           (27)
#define SOC_SCTRL_SCINT_MASK5_pdea_wdt_int_END                             (27)
#define SOC_SCTRL_SCINT_MASK5_modem_bus_err_START                          (28)
#define SOC_SCTRL_SCINT_MASK5_modem_bus_err_END                            (28)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_STAT5_UNION
 struct description   : SCINT_STAT5 Register structure definition
                        Address Offset:0x09C Initial:0x0 Width:32
 register description : intr_ao_wakeup2原始唤醒中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_timer130                         : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                                 0：屏蔽有效；
                                                                                 1：屏蔽无效。 */
        unsigned int  intr_timer131                         : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  intr_timer140                         : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  intr_timer141                         : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  intr_timer150                         : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  intr_timer151                         : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  intr_timer160                         : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  intr_timer161                         : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  intr_timer170                         : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  intr_timer171                         : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  intr_pcie0_0_l12_wake_gic_lpmcu       : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  intr_ao_wd1                           : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  intr_wakeup_swing_enter_req           : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  intr_wakeup_swing_exit_req            : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  intr_lfc_mode_quit_to_sctrl_and_lpmcu : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  intr_usb3_sys_error_to_ao_wakeup      : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  reserved_0                            : 1;  /* bit[16]   : 含义同bit0。 */
        unsigned int  intr_m7_ahb_dbg_timeout_lpmcu         : 1;  /* bit[17]   : 含义同bit0。 */
        unsigned int  intr_pcie0_1_l12_wake_gic_lpmcu       : 1;  /* bit[18]   : 含义同bit0。 */
        unsigned int  intr_pcie0_0_l2_wake_gic_lpmcu        : 1;  /* bit[19]   : 含义同bit0。 */
        unsigned int  intr_pcie0_1_l2_wake_gic_lpmcu        : 1;  /* bit[20]   : 含义同bit0。 */
        unsigned int  reserved_1                            : 1;  /* bit[21]   : 含义同bit0。 */
        unsigned int  dss_doze1_wake                        : 1;  /* bit[22]   : 含义同bit0。 */
        unsigned int  dss_doze1_wd_wake                     : 1;  /* bit[23]   : 含义同bit0。 */
        unsigned int  freq_abnormal_int_0                   : 1;  /* bit[24]   : 含义同bit0。 */
        unsigned int  mdm_pll_unlock_intr_lpmcu             : 1;  /* bit[25]   : 含义同bit0。 */
        unsigned int  mdm_wdt_intr_merge_acpu_lpmcu         : 1;  /* bit[26]   : 含义同bit0。 */
        unsigned int  pdea_wdt_int                          : 1;  /* bit[27]   : 含义同bit0。 */
        unsigned int  modem_bus_err                         : 1;  /* bit[28-28]: 含义同bit0。 */
        unsigned int  reserved_2                            : 1;  /* bit[29-29]: 含义同bit0。 */
        unsigned int  reserved_3                            : 1;  /* bit[30-30]: 含义同bit0。 */
        unsigned int  reserved_4                            : 1;  /* bit[31-31]: 含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_STAT5_UNION;
#endif
#define SOC_SCTRL_SCINT_STAT5_intr_timer130_START                          (0)
#define SOC_SCTRL_SCINT_STAT5_intr_timer130_END                            (0)
#define SOC_SCTRL_SCINT_STAT5_intr_timer131_START                          (1)
#define SOC_SCTRL_SCINT_STAT5_intr_timer131_END                            (1)
#define SOC_SCTRL_SCINT_STAT5_intr_timer140_START                          (2)
#define SOC_SCTRL_SCINT_STAT5_intr_timer140_END                            (2)
#define SOC_SCTRL_SCINT_STAT5_intr_timer141_START                          (3)
#define SOC_SCTRL_SCINT_STAT5_intr_timer141_END                            (3)
#define SOC_SCTRL_SCINT_STAT5_intr_timer150_START                          (4)
#define SOC_SCTRL_SCINT_STAT5_intr_timer150_END                            (4)
#define SOC_SCTRL_SCINT_STAT5_intr_timer151_START                          (5)
#define SOC_SCTRL_SCINT_STAT5_intr_timer151_END                            (5)
#define SOC_SCTRL_SCINT_STAT5_intr_timer160_START                          (6)
#define SOC_SCTRL_SCINT_STAT5_intr_timer160_END                            (6)
#define SOC_SCTRL_SCINT_STAT5_intr_timer161_START                          (7)
#define SOC_SCTRL_SCINT_STAT5_intr_timer161_END                            (7)
#define SOC_SCTRL_SCINT_STAT5_intr_timer170_START                          (8)
#define SOC_SCTRL_SCINT_STAT5_intr_timer170_END                            (8)
#define SOC_SCTRL_SCINT_STAT5_intr_timer171_START                          (9)
#define SOC_SCTRL_SCINT_STAT5_intr_timer171_END                            (9)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_0_l12_wake_gic_lpmcu_START        (10)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_0_l12_wake_gic_lpmcu_END          (10)
#define SOC_SCTRL_SCINT_STAT5_intr_ao_wd1_START                            (11)
#define SOC_SCTRL_SCINT_STAT5_intr_ao_wd1_END                              (11)
#define SOC_SCTRL_SCINT_STAT5_intr_wakeup_swing_enter_req_START            (12)
#define SOC_SCTRL_SCINT_STAT5_intr_wakeup_swing_enter_req_END              (12)
#define SOC_SCTRL_SCINT_STAT5_intr_wakeup_swing_exit_req_START             (13)
#define SOC_SCTRL_SCINT_STAT5_intr_wakeup_swing_exit_req_END               (13)
#define SOC_SCTRL_SCINT_STAT5_intr_lfc_mode_quit_to_sctrl_and_lpmcu_START  (14)
#define SOC_SCTRL_SCINT_STAT5_intr_lfc_mode_quit_to_sctrl_and_lpmcu_END    (14)
#define SOC_SCTRL_SCINT_STAT5_intr_usb3_sys_error_to_ao_wakeup_START       (15)
#define SOC_SCTRL_SCINT_STAT5_intr_usb3_sys_error_to_ao_wakeup_END         (15)
#define SOC_SCTRL_SCINT_STAT5_intr_m7_ahb_dbg_timeout_lpmcu_START          (17)
#define SOC_SCTRL_SCINT_STAT5_intr_m7_ahb_dbg_timeout_lpmcu_END            (17)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_1_l12_wake_gic_lpmcu_START        (18)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_1_l12_wake_gic_lpmcu_END          (18)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_0_l2_wake_gic_lpmcu_START         (19)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_0_l2_wake_gic_lpmcu_END           (19)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_1_l2_wake_gic_lpmcu_START         (20)
#define SOC_SCTRL_SCINT_STAT5_intr_pcie0_1_l2_wake_gic_lpmcu_END           (20)
#define SOC_SCTRL_SCINT_STAT5_dss_doze1_wake_START                         (22)
#define SOC_SCTRL_SCINT_STAT5_dss_doze1_wake_END                           (22)
#define SOC_SCTRL_SCINT_STAT5_dss_doze1_wd_wake_START                      (23)
#define SOC_SCTRL_SCINT_STAT5_dss_doze1_wd_wake_END                        (23)
#define SOC_SCTRL_SCINT_STAT5_freq_abnormal_int_0_START                    (24)
#define SOC_SCTRL_SCINT_STAT5_freq_abnormal_int_0_END                      (24)
#define SOC_SCTRL_SCINT_STAT5_mdm_pll_unlock_intr_lpmcu_START              (25)
#define SOC_SCTRL_SCINT_STAT5_mdm_pll_unlock_intr_lpmcu_END                (25)
#define SOC_SCTRL_SCINT_STAT5_mdm_wdt_intr_merge_acpu_lpmcu_START          (26)
#define SOC_SCTRL_SCINT_STAT5_mdm_wdt_intr_merge_acpu_lpmcu_END            (26)
#define SOC_SCTRL_SCINT_STAT5_pdea_wdt_int_START                           (27)
#define SOC_SCTRL_SCINT_STAT5_pdea_wdt_int_END                             (27)
#define SOC_SCTRL_SCINT_STAT5_modem_bus_err_START                          (28)
#define SOC_SCTRL_SCINT_STAT5_modem_bus_err_END                            (28)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_INT_STD2LS_MASK_UNION
 struct description   : SCINT_INT_STD2LS_MASK Register structure definition
                        Address Offset:0x0D0 Initial:0x00000 Width:32
 register description : 唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_wakeup_swing_exit_req : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                      0：屏蔽有效；
                                                                      1：屏蔽无效。 */
        unsigned int  reserved_0                 : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  reserved_1                 : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  reserved_2                 : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  reserved_3                 : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  reserved_4                 : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_5                 : 1;  /* bit[6]    :  */
        unsigned int  reserved_6                 : 1;  /* bit[7]    :  */
        unsigned int  reserved_7                 : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_8                 : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_9                 : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  reserved_10                : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  reserved_11                : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  reserved_12                : 19; /* bit[13-31]: 
                                                                      含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_INT_STD2LS_MASK_UNION;
#endif
#define SOC_SCTRL_SCINT_INT_STD2LS_MASK_intr_wakeup_swing_exit_req_START  (0)
#define SOC_SCTRL_SCINT_INT_STD2LS_MASK_intr_wakeup_swing_exit_req_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINT_INT_LS2STD_MASK_UNION
 struct description   : SCINT_INT_LS2STD_MASK Register structure definition
                        Address Offset:0x0D4 Initial:0x00000 Width:32
 register description : 唤醒中断屏蔽配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_wakeup_swing_enter_req : 1;  /* bit[0]    : 中断屏蔽寄存器。
                                                                       0：屏蔽有效；
                                                                       1：屏蔽无效。 */
        unsigned int  reserved_0                  : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  reserved_1                  : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  reserved_2                  : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  reserved_3                  : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  reserved_4                  : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_5                  : 1;  /* bit[6]    :  */
        unsigned int  reserved_6                  : 1;  /* bit[7]    :  */
        unsigned int  reserved_7                  : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_8                  : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_9                  : 1;  /* bit[10]   : 含义同bit0。 */
        unsigned int  reserved_10                 : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  reserved_11                 : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  reserved_12                 : 19; /* bit[13-31]: 
                                                                       含义同bit0。 */
    } reg;
} SOC_SCTRL_SCINT_INT_LS2STD_MASK_UNION;
#endif
#define SOC_SCTRL_SCINT_INT_LS2STD_MASK_intr_wakeup_swing_enter_req_START  (0)
#define SOC_SCTRL_SCINT_INT_LS2STD_MASK_intr_wakeup_swing_enter_req_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL0_UNION
 struct description   : SCPERCTRL0 Register structure definition
                        Address Offset:0x300 Initial:0x00 Width:32
 register description : 外设控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_efusec_mem_ctrl : 26; /* bit[0-25] : bit[5:3]:011;
                                                              bit[7:6]:01
                                                              bit[10:8]:000
                                                              bit[13:11]:011
                                                              bit[15:!4]: 01
                                                              bit[18:16]:000
                                                              其他为0，子系统分配。 */
        unsigned int  reserved_0         : 1;  /* bit[26]   :  */
        unsigned int  reserved_1         : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_SCTRL_SCPERCTRL0_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL0_sc_efusec_mem_ctrl_START  (0)
#define SOC_SCTRL_SCPERCTRL0_sc_efusec_mem_ctrl_END    (25)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL1_UNION
 struct description   : SCPERCTRL1 Register structure definition
                        Address Offset:0x304 Initial:0xFFFFFFFF Width:32
 register description : 外设控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL1_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL2_UNION
 struct description   : SCPERCTRL2 Register structure definition
                        Address Offset:0x308 Initial:0x00 Width:32
 register description : 外设控制寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 8;  /* bit[0-7]  : 不使用 */
        unsigned int  reserved_1      : 1;  /* bit[8]    :  */
        unsigned int  reserved_2      : 1;  /* bit[9]    :  */
        unsigned int  txp_jtag_bypass : 1;  /* bit[10]   :  */
        unsigned int  reserved_3      : 5;  /* bit[11-15]:  */
        unsigned int  reserved_4      : 1;  /* bit[16]   :  */
        unsigned int  reserved_5      : 1;  /* bit[17]   :  */
        unsigned int  reserved_6      : 1;  /* bit[18]   :  */
        unsigned int  reserved_7      : 1;  /* bit[19]   :  */
        unsigned int  reserved_8      : 1;  /* bit[20]   :  */
        unsigned int  reserved_9      : 4;  /* bit[21-24]: 保留。 */
        unsigned int  reserved_10     : 1;  /* bit[25]   : 保留。 */
        unsigned int  reserved_11     : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCPERCTRL2_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL2_txp_jtag_bypass_START  (10)
#define SOC_SCTRL_SCPERCTRL2_txp_jtag_bypass_END    (10)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL3_UNION
 struct description   : SCPERCTRL3 Register structure definition
                        Address Offset:0x30C Initial:0x0000 Width:32
 register description : 外设控制寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  efusec2_timeout_bypass   : 1;  /* bit[0]    :  */
        unsigned int  reserved_0               : 1;  /* bit[1]    :  */
        unsigned int  reserved_1               : 1;  /* bit[2]    :  */
        unsigned int  reserved_2               : 1;  /* bit[3]    :  */
        unsigned int  reserved_3               : 1;  /* bit[4]    :  */
        unsigned int  reserved_4               : 3;  /* bit[5-7]  :  */
        unsigned int  efusec_timeout_bypass    : 1;  /* bit[8]    : efusec timeout bypass（暂未使用）。 */
        unsigned int  bbdrx_timeout_bypass     : 1;  /* bit[9]    : bbdrx timeout bypass
                                                                    0:no bypass 1:bypass。 */
        unsigned int  sleep_mode_cfgbus_bypass : 1;  /* bit[10]   : PHX ES不使用
                                                                    1'b0:sleepmode有效时关时钟
                                                                    1'b1:sleepmode有效时不关时钟。 */
        unsigned int  sleep_mode_lpmcu_bypass  : 1;  /* bit[11]   : PHX ES不使用
                                                                    1'b0:sleepmode有效时关时钟
                                                                    1'b1:sleepmode有效时不关时钟。 */
        unsigned int  first_power_on_flag      : 1;  /* bit[12]   : CS修改--》上电后软件配置为1'b0。 */
        unsigned int  sleep_mode_aobus_bypass  : 1;  /* bit[13]   : 1'b0:sleepmode有效时关时钟
                                                                    1'b1:sleepmode有效时不关时钟。 */
        unsigned int  reserved_5               : 1;  /* bit[14]   :  */
        unsigned int  reserved_6               : 1;  /* bit[15]   :  */
        unsigned int  scperctrl3_msk           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCPERCTRL3_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL3_efusec2_timeout_bypass_START    (0)
#define SOC_SCTRL_SCPERCTRL3_efusec2_timeout_bypass_END      (0)
#define SOC_SCTRL_SCPERCTRL3_efusec_timeout_bypass_START     (8)
#define SOC_SCTRL_SCPERCTRL3_efusec_timeout_bypass_END       (8)
#define SOC_SCTRL_SCPERCTRL3_bbdrx_timeout_bypass_START      (9)
#define SOC_SCTRL_SCPERCTRL3_bbdrx_timeout_bypass_END        (9)
#define SOC_SCTRL_SCPERCTRL3_sleep_mode_cfgbus_bypass_START  (10)
#define SOC_SCTRL_SCPERCTRL3_sleep_mode_cfgbus_bypass_END    (10)
#define SOC_SCTRL_SCPERCTRL3_sleep_mode_lpmcu_bypass_START   (11)
#define SOC_SCTRL_SCPERCTRL3_sleep_mode_lpmcu_bypass_END     (11)
#define SOC_SCTRL_SCPERCTRL3_first_power_on_flag_START       (12)
#define SOC_SCTRL_SCPERCTRL3_first_power_on_flag_END         (12)
#define SOC_SCTRL_SCPERCTRL3_sleep_mode_aobus_bypass_START   (13)
#define SOC_SCTRL_SCPERCTRL3_sleep_mode_aobus_bypass_END     (13)
#define SOC_SCTRL_SCPERCTRL3_scperctrl3_msk_START            (16)
#define SOC_SCTRL_SCPERCTRL3_scperctrl3_msk_END              (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL4_UNION
 struct description   : SCPERCTRL4 Register structure definition
                        Address Offset:0x310 Initial:0x0 Width:32
 register description : 外设控制寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddr_cha_test_ctrl : 1;  /* bit[0-0]  : NSV不使用。
                                                             DDRPHY Channel A是否启动可维可测功能；
                                                             1'b0：不启动。1'b1：启动。 */
        unsigned int  ddr_chb_test_ctrl : 1;  /* bit[1-1]  : NSV不使用。
                                                             DDRPHY Channel B是否启动可维可测功能；
                                                             1'b0：不启动。1'b1：启动。 */
        unsigned int  ddr_chc_test_ctrl : 1;  /* bit[2-2]  : NSV不使用。
                                                             DDRPHY Channel C是否启动可维可测功能；
                                                             1'b0：不启动。1'b1：启动。 */
        unsigned int  ddr_chd_test_ctrl : 1;  /* bit[3-3]  : NSV不使用。
                                                             DDRPHY Channel D是否启动可维可测功能；
                                                             1'b0：不启动。1'b1：启动。 */
        unsigned int  xo_mode_a2d       : 1;  /* bit[4-4]  : 32K时钟源头
                                                             1'b0: 19.2/586
                                                             1'b1: 32768 */
        unsigned int  reserved_0        : 3;  /* bit[5-7]  : 保留。 */
        unsigned int  crc_value         : 21; /* bit[8-28] : RTC原理：将32K RC时钟同步到19.2MHz时钟域，在连续1024个RC时钟计数窗口内，统计19.2MHz的时钟个数，得到数值A。
                                                             可知A=N*1024+B，其中N和B为整数，用A可以推算出商N和余数B。
                                                             b) 32K来源为19.2MHz/586 时，N为586， B为0；
                                                             c) 32K来源为32768Hz 时， N为585， B为960；
                                                             高11bit 用来配置N；低10bit 用来配置B。 */
        unsigned int  reserved_1        : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCPERCTRL4_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL4_ddr_cha_test_ctrl_START  (0)
#define SOC_SCTRL_SCPERCTRL4_ddr_cha_test_ctrl_END    (0)
#define SOC_SCTRL_SCPERCTRL4_ddr_chb_test_ctrl_START  (1)
#define SOC_SCTRL_SCPERCTRL4_ddr_chb_test_ctrl_END    (1)
#define SOC_SCTRL_SCPERCTRL4_ddr_chc_test_ctrl_START  (2)
#define SOC_SCTRL_SCPERCTRL4_ddr_chc_test_ctrl_END    (2)
#define SOC_SCTRL_SCPERCTRL4_ddr_chd_test_ctrl_START  (3)
#define SOC_SCTRL_SCPERCTRL4_ddr_chd_test_ctrl_END    (3)
#define SOC_SCTRL_SCPERCTRL4_xo_mode_a2d_START        (4)
#define SOC_SCTRL_SCPERCTRL4_xo_mode_a2d_END          (4)
#define SOC_SCTRL_SCPERCTRL4_crc_value_START          (8)
#define SOC_SCTRL_SCPERCTRL4_crc_value_END            (28)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL5_UNION
 struct description   : SCPERCTRL5 Register structure definition
                        Address Offset:0x314 Initial:0x0000 Width:32
 register description : 外设控制寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 4;  /* bit[0-3]  :  */
        unsigned int  ao_hpm_clk_div     : 6;  /* bit[4-9]  : ao HPM时钟分频器。配置值要求大于1；分频比=配置值+1。 */
        unsigned int  ao_hpm_en          : 1;  /* bit[10]   : ao HPM使能。
                                                              0:disable 1:enable。 */
        unsigned int  ao_hpmx_en         : 1;  /* bit[11]   : ao HPMX使能。
                                                              0:disable 1:enable。 */
        unsigned int  ao_hpmx_opc_min_en : 1;  /* bit[12]   :  */
        unsigned int  ao_hpm_opc_min_en  : 1;  /* bit[13]   :  */
        unsigned int  reserved_1         : 1;  /* bit[14]   :  */
        unsigned int  reserved_2         : 1;  /* bit[15]   :  */
        unsigned int  scperctrl5_msk     : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCPERCTRL5_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL5_ao_hpm_clk_div_START      (4)
#define SOC_SCTRL_SCPERCTRL5_ao_hpm_clk_div_END        (9)
#define SOC_SCTRL_SCPERCTRL5_ao_hpm_en_START           (10)
#define SOC_SCTRL_SCPERCTRL5_ao_hpm_en_END             (10)
#define SOC_SCTRL_SCPERCTRL5_ao_hpmx_en_START          (11)
#define SOC_SCTRL_SCPERCTRL5_ao_hpmx_en_END            (11)
#define SOC_SCTRL_SCPERCTRL5_ao_hpmx_opc_min_en_START  (12)
#define SOC_SCTRL_SCPERCTRL5_ao_hpmx_opc_min_en_END    (12)
#define SOC_SCTRL_SCPERCTRL5_ao_hpm_opc_min_en_START   (13)
#define SOC_SCTRL_SCPERCTRL5_ao_hpm_opc_min_en_END     (13)
#define SOC_SCTRL_SCPERCTRL5_scperctrl5_msk_START      (16)
#define SOC_SCTRL_SCPERCTRL5_scperctrl5_msk_END        (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL6_UNION
 struct description   : SCPERCTRL6 Register structure definition
                        Address Offset:0x318 Initial:0x00 Width:32
 register description : 外设控制寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_fix_io_ret      : 1;  /* bit[0-0]  : PERI_FIX IO Retetion
                                                                0：Retetion撤销；
                                                                1：Retetion使能。 */
        unsigned int  reserved_0           : 1;  /* bit[1]    :  */
        unsigned int  reserved_1           : 2;  /* bit[2-3]  :  */
        unsigned int  peri_bootio_ret      : 1;  /* bit[4-4]  : 外设区启动相关IO Retetion：
                                                                0：Retetion撤销；
                                                                1：Retetion使能。 */
        unsigned int  peri_bootio_ret_mode : 1;  /* bit[5-5]  : 外设区启动相关IO Retetion控制模式：
                                                                0：由比特4的配置值控制；
                                                                1：由硬件自动控制。 */
        unsigned int  reserved_2           : 2;  /* bit[6-7]  :  */
        unsigned int  reserved_3           : 2;  /* bit[8-9]  :  */
        unsigned int  reserved_4           : 2;  /* bit[10-11]:  */
        unsigned int  reserved_5           : 1;  /* bit[12]   :  */
        unsigned int  reserved_6           : 7;  /* bit[13-19]: 保留。 */
        unsigned int  reserved_7           : 1;  /* bit[20]   : 保留。 */
        unsigned int  reserved_8           : 1;  /* bit[21]   : 保留。 */
        unsigned int  reserved_9           : 1;  /* bit[22]   : 保留。 */
        unsigned int  reserved_10          : 1;  /* bit[23]   : 保留。 */
        unsigned int  reserved_11          : 8;  /* bit[24-31]: SYS_MISC有逻辑处理，注意修改！！！ */
    } reg;
} SOC_SCTRL_SCPERCTRL6_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL6_peri_fix_io_ret_START       (0)
#define SOC_SCTRL_SCPERCTRL6_peri_fix_io_ret_END         (0)
#define SOC_SCTRL_SCPERCTRL6_peri_bootio_ret_START       (4)
#define SOC_SCTRL_SCPERCTRL6_peri_bootio_ret_END         (4)
#define SOC_SCTRL_SCPERCTRL6_peri_bootio_ret_mode_START  (5)
#define SOC_SCTRL_SCPERCTRL6_peri_bootio_ret_mode_END    (5)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL7_UNION
 struct description   : SCPERCTRL7 Register structure definition
                        Address Offset:0x31C Initial:0x0000 Width:32
 register description : 外设控制寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sctrl2lm_hard_volt           : 1;  /* bit[0]    :  */
        unsigned int  reserved_0                   : 1;  /* bit[1]    :  */
        unsigned int  reserved_1                   : 1;  /* bit[2]    :  */
        unsigned int  reserved_2                   : 1;  /* bit[3]    :  */
        unsigned int  reserved_3                   : 1;  /* bit[4]    :  */
        unsigned int  reserved_4                   : 1;  /* bit[5]    :  */
        unsigned int  reserved_5                   : 1;  /* bit[6]    :  */
        unsigned int  reserved_6                   : 1;  /* bit[7]    :  */
        unsigned int  sel_suspend_ao2syscache      : 1;  /* bit[8]    : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  reserved_7                   : 1;  /* bit[9]    :  */
        unsigned int  sleep_mode_to_peri_bypass    : 1;  /* bit[10]   :  */
        unsigned int  sleep_mode_to_hsdt_bypass    : 1;  /* bit[11]   :  */
        unsigned int  sleep_mode_to_cpu_crg_bypass : 1;  /* bit[12]   :  */
        unsigned int  peri_nonbootio_ret_mode      : 1;  /* bit[13]   : 外设区非启动相关IO Retetion控制模式：
                                                                        0：由比特14的配置值控制；
                                                                        1：由硬件自动控制。 */
        unsigned int  sc_peri_nonboot_io_ret       : 1;  /* bit[14]   : PERI 非BOOT IO Ret */
        unsigned int  pmu_powerhold_protect        : 1;  /* bit[15]   :  */
        unsigned int  scperctrl7_msk               : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效，SYS_MISC有逻辑处理，注意修改！！！ */
    } reg;
} SOC_SCTRL_SCPERCTRL7_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL7_sctrl2lm_hard_volt_START            (0)
#define SOC_SCTRL_SCPERCTRL7_sctrl2lm_hard_volt_END              (0)
#define SOC_SCTRL_SCPERCTRL7_sel_suspend_ao2syscache_START       (8)
#define SOC_SCTRL_SCPERCTRL7_sel_suspend_ao2syscache_END         (8)
#define SOC_SCTRL_SCPERCTRL7_sleep_mode_to_peri_bypass_START     (10)
#define SOC_SCTRL_SCPERCTRL7_sleep_mode_to_peri_bypass_END       (10)
#define SOC_SCTRL_SCPERCTRL7_sleep_mode_to_hsdt_bypass_START     (11)
#define SOC_SCTRL_SCPERCTRL7_sleep_mode_to_hsdt_bypass_END       (11)
#define SOC_SCTRL_SCPERCTRL7_sleep_mode_to_cpu_crg_bypass_START  (12)
#define SOC_SCTRL_SCPERCTRL7_sleep_mode_to_cpu_crg_bypass_END    (12)
#define SOC_SCTRL_SCPERCTRL7_peri_nonbootio_ret_mode_START       (13)
#define SOC_SCTRL_SCPERCTRL7_peri_nonbootio_ret_mode_END         (13)
#define SOC_SCTRL_SCPERCTRL7_sc_peri_nonboot_io_ret_START        (14)
#define SOC_SCTRL_SCPERCTRL7_sc_peri_nonboot_io_ret_END          (14)
#define SOC_SCTRL_SCPERCTRL7_pmu_powerhold_protect_START         (15)
#define SOC_SCTRL_SCPERCTRL7_pmu_powerhold_protect_END           (15)
#define SOC_SCTRL_SCPERCTRL7_scperctrl7_msk_START                (16)
#define SOC_SCTRL_SCPERCTRL7_scperctrl7_msk_END                  (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL9_UNION
 struct description   : SCPERCTRL9 Register structure definition
                        Address Offset:0x324 Initial:0x0000000 Width:32
 register description : 外设控制寄存器9
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  rst_apb_bbp_on    : 1;  /* bit[0]   : bbp on区apb时钟域的软复位信号
                                                            1'b1:复位
                                                            1'b0:解除复位 */
        unsigned int  rst_19m_bbp_on    : 1;  /* bit[1]   : bbp on区19.2M时钟域的软复位信号
                                                            1'b1:复位
                                                            1'b0:解除复位 */
        unsigned int  rst_32k_bbp_on    : 1;  /* bit[2]   : bbp on区32K时钟域的软复位信号
                                                            1'b1:复位
                                                            1'b0:解除复位 */
        unsigned int  reserved_0        : 1;  /* bit[3]   :  */
        unsigned int  rst_bbp_dbg_timer : 1;  /* bit[4]   : bbp on区32K计数器(串行32k格式时戳)的软复位信号
                                                            1'b1:复位
                                                            1'b0:解除复位 */
        unsigned int  rst_bbp_guc_timer : 1;  /* bit[5]   : bbp on区32K计数器(格雷码32k时戳，sctrl使用)的软复位信号
                                                            1'b1:复位
                                                            1'b0:解除复位 */
        unsigned int  reserved_1        : 1;  /* bit[6]   :  */
        unsigned int  bbp_on_ctrl       : 25; /* bit[7-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL9_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL9_rst_apb_bbp_on_START     (0)
#define SOC_SCTRL_SCPERCTRL9_rst_apb_bbp_on_END       (0)
#define SOC_SCTRL_SCPERCTRL9_rst_19m_bbp_on_START     (1)
#define SOC_SCTRL_SCPERCTRL9_rst_19m_bbp_on_END       (1)
#define SOC_SCTRL_SCPERCTRL9_rst_32k_bbp_on_START     (2)
#define SOC_SCTRL_SCPERCTRL9_rst_32k_bbp_on_END       (2)
#define SOC_SCTRL_SCPERCTRL9_rst_bbp_dbg_timer_START  (4)
#define SOC_SCTRL_SCPERCTRL9_rst_bbp_dbg_timer_END    (4)
#define SOC_SCTRL_SCPERCTRL9_rst_bbp_guc_timer_START  (5)
#define SOC_SCTRL_SCPERCTRL9_rst_bbp_guc_timer_END    (5)
#define SOC_SCTRL_SCPERCTRL9_bbp_on_ctrl_START        (7)
#define SOC_SCTRL_SCPERCTRL9_bbp_on_ctrl_END          (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL10_UNION
 struct description   : SCPERCTRL10 Register structure definition
                        Address Offset:0x328 Initial:0x0 Width:32
 register description : 外设控制寄存器10
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddra_acctl_ioctrl_henl_in : 1;  /* bit[0]    :  */
        unsigned int  ddrb_acctl_ioctrl_henl_in : 1;  /* bit[1]    :  */
        unsigned int  ddrc_acctl_ioctrl_henl_in : 1;  /* bit[2]    :  */
        unsigned int  ddrd_acctl_ioctrl_henl_in : 1;  /* bit[3]    :  */
        unsigned int  ddra_pll_en_lat           : 1;  /* bit[4]    :  */
        unsigned int  ddrb_pll_en_lat           : 1;  /* bit[5]    :  */
        unsigned int  ddrc_pll_en_lat           : 1;  /* bit[6]    :  */
        unsigned int  ddrd_pll_en_lat           : 1;  /* bit[7]    :  */
        unsigned int  ddrac_ioctl_ocldo_lhen_in : 1;  /* bit[8]    :  */
        unsigned int  ddrbd_ioctl_ocldo_lhen_in : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                : 8;  /* bit[10-17]:  */
        unsigned int  reserved_1                : 2;  /* bit[18-19]:  */
        unsigned int  reserved_2                : 4;  /* bit[20-23]:  */
        unsigned int  reserved_3                : 2;  /* bit[24-25]:  */
        unsigned int  reserved_4                : 2;  /* bit[26-27]:  */
        unsigned int  reserved_5                : 4;  /* bit[28-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL10_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL10_ddra_acctl_ioctrl_henl_in_START  (0)
#define SOC_SCTRL_SCPERCTRL10_ddra_acctl_ioctrl_henl_in_END    (0)
#define SOC_SCTRL_SCPERCTRL10_ddrb_acctl_ioctrl_henl_in_START  (1)
#define SOC_SCTRL_SCPERCTRL10_ddrb_acctl_ioctrl_henl_in_END    (1)
#define SOC_SCTRL_SCPERCTRL10_ddrc_acctl_ioctrl_henl_in_START  (2)
#define SOC_SCTRL_SCPERCTRL10_ddrc_acctl_ioctrl_henl_in_END    (2)
#define SOC_SCTRL_SCPERCTRL10_ddrd_acctl_ioctrl_henl_in_START  (3)
#define SOC_SCTRL_SCPERCTRL10_ddrd_acctl_ioctrl_henl_in_END    (3)
#define SOC_SCTRL_SCPERCTRL10_ddra_pll_en_lat_START            (4)
#define SOC_SCTRL_SCPERCTRL10_ddra_pll_en_lat_END              (4)
#define SOC_SCTRL_SCPERCTRL10_ddrb_pll_en_lat_START            (5)
#define SOC_SCTRL_SCPERCTRL10_ddrb_pll_en_lat_END              (5)
#define SOC_SCTRL_SCPERCTRL10_ddrc_pll_en_lat_START            (6)
#define SOC_SCTRL_SCPERCTRL10_ddrc_pll_en_lat_END              (6)
#define SOC_SCTRL_SCPERCTRL10_ddrd_pll_en_lat_START            (7)
#define SOC_SCTRL_SCPERCTRL10_ddrd_pll_en_lat_END              (7)
#define SOC_SCTRL_SCPERCTRL10_ddrac_ioctl_ocldo_lhen_in_START  (8)
#define SOC_SCTRL_SCPERCTRL10_ddrac_ioctl_ocldo_lhen_in_END    (8)
#define SOC_SCTRL_SCPERCTRL10_ddrbd_ioctl_ocldo_lhen_in_START  (9)
#define SOC_SCTRL_SCPERCTRL10_ddrbd_ioctl_ocldo_lhen_in_END    (9)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL11_UNION
 struct description   : SCPERCTRL11 Register structure definition
                        Address Offset:0x32C Initial:0x00000000 Width:32
 register description : 外设控制寄存器11
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL11_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL12_UNION
 struct description   : SCPERCTRL12 Register structure definition
                        Address Offset:0x330 Initial:0x1FFFFF Width:32
 register description : 外设控制寄存器12
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  :  */
        unsigned int  reserved_1: 2;  /* bit[6-7]  :  */
        unsigned int  reserved_2: 3;  /* bit[8-10] :  */
        unsigned int  reserved_3: 21; /* bit[11-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL12_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL13_UNION
 struct description   : SCPERCTRL13 Register structure definition
                        Address Offset:0x334 Initial:0x3FFFFFFF Width:32
 register description : 外设控制寄存器13
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 1;  /* bit[1]   :  */
        unsigned int  reserved_2: 30; /* bit[2-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL13_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL14_UNION
 struct description   : SCPERCTRL14 Register structure definition
                        Address Offset:0x338 Initial:0x00000000 Width:32
 register description : 外设控制寄存器14
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL14_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL15_UNION
 struct description   : SCPERCTRL15 Register structure definition
                        Address Offset:0x33C Initial:0x00000000 Width:32
 register description : 外设控制寄存器15
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL15_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL16_UNION
 struct description   : SCPERCTRL16 Register structure definition
                        Address Offset:0x340 Initial:0x0000 Width:32
 register description : 外设控制寄存器8，安全寄存器，控制信号由SCTRL内部逻辑产生的状态输出控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  avs_en0         : 1;  /* bit[0]    :  */
        unsigned int  avs_en1         : 1;  /* bit[1]    :  */
        unsigned int  avs_en2         : 1;  /* bit[2]    :  */
        unsigned int  avs_en3         : 1;  /* bit[3]    :  */
        unsigned int  avs_en4         : 1;  /* bit[4]    :  */
        unsigned int  avs_en5         : 1;  /* bit[5]    :  */
        unsigned int  avs_en6         : 1;  /* bit[6]    :  */
        unsigned int  avs_en7         : 1;  /* bit[7]    :  */
        unsigned int  avs_en8         : 1;  /* bit[8]    :  */
        unsigned int  avs_en9         : 1;  /* bit[9]    :  */
        unsigned int  avs_en10        : 1;  /* bit[10]   :  */
        unsigned int  avs_en11        : 1;  /* bit[11]   :  */
        unsigned int  avs_en12        : 1;  /* bit[12]   :  */
        unsigned int  avs_en13        : 1;  /* bit[13]   :  */
        unsigned int  avs_en14        : 1;  /* bit[14]   :  */
        unsigned int  avs_en15        : 1;  /* bit[15]   :  */
        unsigned int  scperctrl16_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCPERCTRL16_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL16_avs_en0_START          (0)
#define SOC_SCTRL_SCPERCTRL16_avs_en0_END            (0)
#define SOC_SCTRL_SCPERCTRL16_avs_en1_START          (1)
#define SOC_SCTRL_SCPERCTRL16_avs_en1_END            (1)
#define SOC_SCTRL_SCPERCTRL16_avs_en2_START          (2)
#define SOC_SCTRL_SCPERCTRL16_avs_en2_END            (2)
#define SOC_SCTRL_SCPERCTRL16_avs_en3_START          (3)
#define SOC_SCTRL_SCPERCTRL16_avs_en3_END            (3)
#define SOC_SCTRL_SCPERCTRL16_avs_en4_START          (4)
#define SOC_SCTRL_SCPERCTRL16_avs_en4_END            (4)
#define SOC_SCTRL_SCPERCTRL16_avs_en5_START          (5)
#define SOC_SCTRL_SCPERCTRL16_avs_en5_END            (5)
#define SOC_SCTRL_SCPERCTRL16_avs_en6_START          (6)
#define SOC_SCTRL_SCPERCTRL16_avs_en6_END            (6)
#define SOC_SCTRL_SCPERCTRL16_avs_en7_START          (7)
#define SOC_SCTRL_SCPERCTRL16_avs_en7_END            (7)
#define SOC_SCTRL_SCPERCTRL16_avs_en8_START          (8)
#define SOC_SCTRL_SCPERCTRL16_avs_en8_END            (8)
#define SOC_SCTRL_SCPERCTRL16_avs_en9_START          (9)
#define SOC_SCTRL_SCPERCTRL16_avs_en9_END            (9)
#define SOC_SCTRL_SCPERCTRL16_avs_en10_START         (10)
#define SOC_SCTRL_SCPERCTRL16_avs_en10_END           (10)
#define SOC_SCTRL_SCPERCTRL16_avs_en11_START         (11)
#define SOC_SCTRL_SCPERCTRL16_avs_en11_END           (11)
#define SOC_SCTRL_SCPERCTRL16_avs_en12_START         (12)
#define SOC_SCTRL_SCPERCTRL16_avs_en12_END           (12)
#define SOC_SCTRL_SCPERCTRL16_avs_en13_START         (13)
#define SOC_SCTRL_SCPERCTRL16_avs_en13_END           (13)
#define SOC_SCTRL_SCPERCTRL16_avs_en14_START         (14)
#define SOC_SCTRL_SCPERCTRL16_avs_en14_END           (14)
#define SOC_SCTRL_SCPERCTRL16_avs_en15_START         (15)
#define SOC_SCTRL_SCPERCTRL16_avs_en15_END           (15)
#define SOC_SCTRL_SCPERCTRL16_scperctrl16_msk_START  (16)
#define SOC_SCTRL_SCPERCTRL16_scperctrl16_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL17_UNION
 struct description   : SCPERCTRL17 Register structure definition
                        Address Offset:0x344 Initial:0x0000 Width:32
 register description : 外设控制寄存器8，安全寄存器，控制信号由SCTRL内部逻辑产生的状态输出控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                                    : 4;  /* bit[0-3]  :  */
        unsigned int  reserved_1                                    : 1;  /* bit[4]    :  */
        unsigned int  reserved_2                                    : 1;  /* bit[5]    :  */
        unsigned int  sel_suspend_ao2syscache_core_crg_apb_inuse    : 1;  /* bit[6]    : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  sel_suspend_ao2syscache_lpmcu_invar_inuse     : 1;  /* bit[7]    : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  sel_suspend_ao2syscache_ddr_inuse             : 1;  /* bit[8]    : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  sel_suspend_ao2syscache_sysbus_inuse          : 1;  /* bit[9]    : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  sel_suspend_ao2syscache_hsdt_subsys_inuse     : 1;  /* bit[10]   : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  sel_suspend_ao2syscache_center_crossbar_inuse : 1;  /* bit[11]   : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  sel_suspend_ao1syscache_cpu_gic_inuse         : 1;  /* bit[12]   : 0是选择32k时钟源，1是选择fll时钟源 */
        unsigned int  mddrc_syscache_retention_flag                 : 1;  /* bit[13]   : MDDRC和SYSCACHE retention标志，软件标记使用(软件标记，ao_sig_misc不分配)
                                                                                         1'b1：进入Retention；1'b0：不进入retention */
        unsigned int  hifi_load_image_flag                          : 1;  /* bit[14]   : 1'b0: 标志HIFI镜像加载;1'b1:不加载(软件标记，ao_sig_misc不分配) */
        unsigned int  asp_imgld_flag                                : 1;  /* bit[15]   : 软件标记位，ASP上电时是否run HIFI(软件标记，ao_sig_misc不分配)
                                                                                         1'b1: run HIFI；1'b0: 不run HIFI */
        unsigned int  scperctrl17_msk                               : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCPERCTRL17_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_core_crg_apb_inuse_START     (6)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_core_crg_apb_inuse_END       (6)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_lpmcu_invar_inuse_START      (7)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_lpmcu_invar_inuse_END        (7)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_ddr_inuse_START              (8)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_ddr_inuse_END                (8)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_sysbus_inuse_START           (9)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_sysbus_inuse_END             (9)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_hsdt_subsys_inuse_START      (10)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_hsdt_subsys_inuse_END        (10)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_center_crossbar_inuse_START  (11)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao2syscache_center_crossbar_inuse_END    (11)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao1syscache_cpu_gic_inuse_START          (12)
#define SOC_SCTRL_SCPERCTRL17_sel_suspend_ao1syscache_cpu_gic_inuse_END            (12)
#define SOC_SCTRL_SCPERCTRL17_mddrc_syscache_retention_flag_START                  (13)
#define SOC_SCTRL_SCPERCTRL17_mddrc_syscache_retention_flag_END                    (13)
#define SOC_SCTRL_SCPERCTRL17_hifi_load_image_flag_START                           (14)
#define SOC_SCTRL_SCPERCTRL17_hifi_load_image_flag_END                             (14)
#define SOC_SCTRL_SCPERCTRL17_asp_imgld_flag_START                                 (15)
#define SOC_SCTRL_SCPERCTRL17_asp_imgld_flag_END                                   (15)
#define SOC_SCTRL_SCPERCTRL17_scperctrl17_msk_START                                (16)
#define SOC_SCTRL_SCPERCTRL17_scperctrl17_msk_END                                  (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS0_UNION
 struct description   : SCPERSTATUS0 Register structure definition
                        Address Offset:0x360 Initial:0x0 Width:32
 register description : 外设状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 1;  /* bit[0]    :  */
        unsigned int  reserved_1           : 4;  /* bit[1-4]  :  */
        unsigned int  reserved_2           : 3;  /* bit[5-7]  :  */
        unsigned int  reserved_3           : 3;  /* bit[8-10] :  */
        unsigned int  reserved_4           : 10; /* bit[11-20]:  */
        unsigned int  reserved_5           : 10; /* bit[21-30]:  */
        unsigned int  bbp_apb_bus_slv_stat : 1;  /* bit[31]   : bbp_apb_bus_slv_stat
                                                                apb时钟域的复位状态
                                                                1'b1:处于解复位状态
                                                                1'b0：处于复位状态。 */
    } reg;
} SOC_SCTRL_SCPERSTATUS0_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS0_bbp_apb_bus_slv_stat_START  (31)
#define SOC_SCTRL_SCPERSTATUS0_bbp_apb_bus_slv_stat_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS1_UNION
 struct description   : SCPERSTATUS1 Register structure definition
                        Address Offset:0x364 Initial:0x00000 Width:32
 register description : 外设状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1: 4;  /* bit[8-11] : 保留。 */
        unsigned int  reserved_2: 20; /* bit[12-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCPERSTATUS1_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS2_UNION
 struct description   : SCPERSTATUS2 Register structure definition
                        Address Offset:0x368 Initial:0x0 Width:32
 register description : 外设状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 10; /* bit[0-9]  :  */
        unsigned int  reserved_1: 10; /* bit[10-19]:  */
        unsigned int  reserved_2: 10; /* bit[20-29]:  */
        unsigned int  reserved_3: 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCPERSTATUS2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS3_UNION
 struct description   : SCPERSTATUS3 Register structure definition
                        Address Offset:0x36C Initial:0x00000000 Width:32
 register description : 外设状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS4_UNION
 struct description   : SCPERSTATUS4 Register structure definition
                        Address Offset:0x370 Initial:0x00000000 Width:32
 register description : 外设状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bbdrx_timeout_dbg_info : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS4_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS4_bbdrx_timeout_dbg_info_START  (0)
#define SOC_SCTRL_SCPERSTATUS4_bbdrx_timeout_dbg_info_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS5_UNION
 struct description   : SCPERSTATUS5 Register structure definition
                        Address Offset:0x374 Initial:0x0 Width:32
 register description : 外设状态寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 10; /* bit[0-9]  :  */
        unsigned int  reserved_1         : 10; /* bit[10-19]:  */
        unsigned int  reserved_2         : 1;  /* bit[20]   :  */
        unsigned int  reserved_3         : 1;  /* bit[21]   :  */
        unsigned int  reserved_4         : 1;  /* bit[22]   :  */
        unsigned int  reserved_5         : 1;  /* bit[23]   :  */
        unsigned int  mdm_tsp_dbg_ack    : 1;  /* bit[24]   :  */
        unsigned int  reserved_6         : 1;  /* bit[25]   :  */
        unsigned int  repair_finish      : 1;  /* bit[26]   : 指示该次修复过程是否完成：
                                                              0：该次修复过程没有结束；
                                                              1：该次修复过程结束。 */
        unsigned int  reserved_7         : 1;  /* bit[27]   :  */
        unsigned int  cfg_ispa7_dbgen_dx : 4;  /* bit[28-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS5_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS5_mdm_tsp_dbg_ack_START     (24)
#define SOC_SCTRL_SCPERSTATUS5_mdm_tsp_dbg_ack_END       (24)
#define SOC_SCTRL_SCPERSTATUS5_repair_finish_START       (26)
#define SOC_SCTRL_SCPERSTATUS5_repair_finish_END         (26)
#define SOC_SCTRL_SCPERSTATUS5_cfg_ispa7_dbgen_dx_START  (28)
#define SOC_SCTRL_SCPERSTATUS5_cfg_ispa7_dbgen_dx_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS6_UNION
 struct description   : SCPERSTATUS6 Register structure definition
                        Address Offset:0x378 Initial:0x00 Width:32
 register description : 外设状态寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                : 1;  /* bit[0]    :  */
        unsigned int  reserved_1                : 1;  /* bit[1]    :  */
        unsigned int  reserved_2                : 1;  /* bit[2]    :  */
        unsigned int  reserved_3                : 1;  /* bit[3]    :  */
        unsigned int  reserved_4                : 1;  /* bit[4]    :  */
        unsigned int  reserved_5                : 1;  /* bit[5]    :  */
        unsigned int  reserved_6                : 1;  /* bit[6]    :  */
        unsigned int  reserved_7                : 1;  /* bit[7]    :  */
        unsigned int  reserved_8                : 1;  /* bit[8]    :  */
        unsigned int  reserved_9                : 1;  /* bit[9]    :  */
        unsigned int  reserved_10               : 1;  /* bit[10]   :  */
        unsigned int  reserved_11               : 1;  /* bit[11]   :  */
        unsigned int  reserved_12               : 1;  /* bit[12]   :  */
        unsigned int  reserved_13               : 1;  /* bit[13]   :  */
        unsigned int  reserved_14               : 1;  /* bit[14]   :  */
        unsigned int  reserved_15               : 1;  /* bit[15]   :  */
        unsigned int  sc_cfg_arm_dbgen_dx       : 4;  /* bit[16-19]: {spniden,spiden,niden,dbgen}
                                                                     0:disable 1:enable */
        unsigned int  sc_cfg_mcu_dbgen_dx       : 4;  /* bit[20-23]: {spniden,spiden,niden,dbgen}
                                                                     0:disable 1:enable */
        unsigned int  audio_mmbuf_ctrl_to_sctrl : 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS6_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS6_sc_cfg_arm_dbgen_dx_START        (16)
#define SOC_SCTRL_SCPERSTATUS6_sc_cfg_arm_dbgen_dx_END          (19)
#define SOC_SCTRL_SCPERSTATUS6_sc_cfg_mcu_dbgen_dx_START        (20)
#define SOC_SCTRL_SCPERSTATUS6_sc_cfg_mcu_dbgen_dx_END          (23)
#define SOC_SCTRL_SCPERSTATUS6_audio_mmbuf_ctrl_to_sctrl_START  (24)
#define SOC_SCTRL_SCPERSTATUS6_audio_mmbuf_ctrl_to_sctrl_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS7_UNION
 struct description   : SCPERSTATUS7 Register structure definition
                        Address Offset:0x37C Initial:0x00 Width:32
 register description : 外设状态寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    :  */
        unsigned int  reserved_1: 1;  /* bit[1]    :  */
        unsigned int  reserved_2: 2;  /* bit[2-3]  :  */
        unsigned int  reserved_3: 2;  /* bit[4-5]  :  */
        unsigned int  reserved_4: 1;  /* bit[6]    :  */
        unsigned int  reserved_5: 1;  /* bit[7]    :  */
        unsigned int  reserved_6: 1;  /* bit[8]    :  */
        unsigned int  reserved_7: 1;  /* bit[9]    :  */
        unsigned int  reserved_8: 1;  /* bit[10]   :  */
        unsigned int  reserved_9: 1;  /* bit[11]   :  */
        unsigned int  reserved_10: 1;  /* bit[12]   :  */
        unsigned int  reserved_11: 1;  /* bit[13]   :  */
        unsigned int  reserved_12: 1;  /* bit[14]   :  */
        unsigned int  reserved_13: 1;  /* bit[15]   :  */
        unsigned int  reserved_14: 1;  /* bit[16]   :  */
        unsigned int  reserved_15: 1;  /* bit[17]   :  */
        unsigned int  reserved_16: 1;  /* bit[18]   :  */
        unsigned int  reserved_17: 1;  /* bit[19]   :  */
        unsigned int  reserved_18: 1;  /* bit[20]   :  */
        unsigned int  reserved_19: 1;  /* bit[21]   :  */
        unsigned int  reserved_20: 1;  /* bit[22]   :  */
        unsigned int  reserved_21: 1;  /* bit[23]   :  */
        unsigned int  reserved_22: 8;  /* bit[24-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS7_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS8_UNION
 struct description   : SCPERSTATUS8 Register structure definition
                        Address Offset:0x380 Initial:0x0000 Width:32
 register description : 外设状态寄存器8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    :  */
        unsigned int  reserved_1: 1;  /* bit[1]    :  */
        unsigned int  reserved_2: 1;  /* bit[2]    :  */
        unsigned int  reserved_3: 1;  /* bit[3]    :  */
        unsigned int  reserved_4: 1;  /* bit[4]    :  */
        unsigned int  reserved_5: 1;  /* bit[5]    :  */
        unsigned int  reserved_6: 1;  /* bit[6]    :  */
        unsigned int  reserved_7: 1;  /* bit[7]    :  */
        unsigned int  reserved_8: 1;  /* bit[8]    :  */
        unsigned int  reserved_9: 6;  /* bit[9-14] :  */
        unsigned int  reserved_10: 1;  /* bit[15]   :  */
        unsigned int  reserved_11: 1;  /* bit[16]   :  */
        unsigned int  reserved_12: 1;  /* bit[17]   :  */
        unsigned int  reserved_13: 14; /* bit[18-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS8_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS9_UNION
 struct description   : SCPERSTATUS9 Register structure definition
                        Address Offset:0x384 Initial:0x00 Width:32
 register description : 外设状态寄存器9
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    :  */
        unsigned int  reserved_1: 1;  /* bit[1]    :  */
        unsigned int  reserved_2: 1;  /* bit[2]    :  */
        unsigned int  reserved_3: 1;  /* bit[3]    :  */
        unsigned int  reserved_4: 1;  /* bit[4]    :  */
        unsigned int  reserved_5: 1;  /* bit[5]    :  */
        unsigned int  reserved_6: 1;  /* bit[6]    :  */
        unsigned int  reserved_7: 1;  /* bit[7]    :  */
        unsigned int  reserved_8: 1;  /* bit[8]    :  */
        unsigned int  reserved_9: 1;  /* bit[9]    :  */
        unsigned int  reserved_10: 1;  /* bit[10]   :  */
        unsigned int  reserved_11: 1;  /* bit[11]   :  */
        unsigned int  reserved_12: 1;  /* bit[12]   :  */
        unsigned int  reserved_13: 1;  /* bit[13]   :  */
        unsigned int  reserved_14: 1;  /* bit[14]   :  */
        unsigned int  reserved_15: 1;  /* bit[15]   :  */
        unsigned int  reserved_16: 1;  /* bit[16]   :  */
        unsigned int  reserved_17: 1;  /* bit[17]   :  */
        unsigned int  reserved_18: 1;  /* bit[18]   :  */
        unsigned int  reserved_19: 1;  /* bit[19]   :  */
        unsigned int  reserved_20: 1;  /* bit[20]   :  */
        unsigned int  reserved_21: 1;  /* bit[21]   :  */
        unsigned int  reserved_22: 1;  /* bit[22]   :  */
        unsigned int  reserved_23: 1;  /* bit[23]   :  */
        unsigned int  reserved_24: 1;  /* bit[24]   :  */
        unsigned int  reserved_25: 7;  /* bit[25-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS9_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS10_UNION
 struct description   : SCPERSTATUS10 Register structure definition
                        Address Offset:0x388 Initial:0x000 Width:32
 register description : 外设状态寄存器10
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  iomcu_arc_watchdog_reset      : 1;  /* bit[0]    : PHX ES: IOMCU内部接1'b0 */
        unsigned int  pcie0_100ms_idle_timeout_intr : 1;  /* bit[1]    :  */
        unsigned int  pcie1_100ms_idle_timeout_intr : 1;  /* bit[2]    :  */
        unsigned int  reserved_0                    : 1;  /* bit[3]    :  */
        unsigned int  reserved_1                    : 1;  /* bit[4]    :  */
        unsigned int  reserved_2                    : 1;  /* bit[5]    :  */
        unsigned int  reserved_3                    : 1;  /* bit[6]    :  */
        unsigned int  iomcu_core_stalled            : 1;  /* bit[7]    :  */
        unsigned int  iomcu_sys_tf_half_r           : 1;  /* bit[8]    :  */
        unsigned int  reserved_4                    : 13; /* bit[9-21] :  */
        unsigned int  reserved_5                    : 10; /* bit[22-31]: 测试管脚 */
    } reg;
} SOC_SCTRL_SCPERSTATUS10_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS10_iomcu_arc_watchdog_reset_START       (0)
#define SOC_SCTRL_SCPERSTATUS10_iomcu_arc_watchdog_reset_END         (0)
#define SOC_SCTRL_SCPERSTATUS10_pcie0_100ms_idle_timeout_intr_START  (1)
#define SOC_SCTRL_SCPERSTATUS10_pcie0_100ms_idle_timeout_intr_END    (1)
#define SOC_SCTRL_SCPERSTATUS10_pcie1_100ms_idle_timeout_intr_START  (2)
#define SOC_SCTRL_SCPERSTATUS10_pcie1_100ms_idle_timeout_intr_END    (2)
#define SOC_SCTRL_SCPERSTATUS10_iomcu_core_stalled_START             (7)
#define SOC_SCTRL_SCPERSTATUS10_iomcu_core_stalled_END               (7)
#define SOC_SCTRL_SCPERSTATUS10_iomcu_sys_tf_half_r_START            (8)
#define SOC_SCTRL_SCPERSTATUS10_iomcu_sys_tf_half_r_END              (8)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS11_UNION
 struct description   : SCPERSTATUS11 Register structure definition
                        Address Offset:0x38C Initial:0x0 Width:32
 register description : 外设状态寄存器11
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 10; /* bit[0-9]  :  */
        unsigned int  reserved_1: 10; /* bit[10-19]:  */
        unsigned int  reserved_2: 10; /* bit[20-29]:  */
        unsigned int  reserved_3: 2;  /* bit[30-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS11_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS12_UNION
 struct description   : SCPERSTATUS12 Register structure definition
                        Address Offset:0x390 Initial:0x0000000 Width:32
 register description : 外设状态寄存器12
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                  : 1;  /* bit[0]   :  */
        unsigned int  reserved_1                  : 1;  /* bit[1]   :  */
        unsigned int  reserved_2                  : 1;  /* bit[2]   :  */
        unsigned int  reserved_3                  : 1;  /* bit[3]   :  */
        unsigned int  intr_wakeup_swing_enter_req : 1;  /* bit[4]   :  */
        unsigned int  intr_wakeup_swing_exit_req  : 1;  /* bit[5]   :  */
        unsigned int  reserved_4                  : 26; /* bit[6-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS12_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS12_intr_wakeup_swing_enter_req_START  (4)
#define SOC_SCTRL_SCPERSTATUS12_intr_wakeup_swing_enter_req_END    (4)
#define SOC_SCTRL_SCPERSTATUS12_intr_wakeup_swing_exit_req_START   (5)
#define SOC_SCTRL_SCPERSTATUS12_intr_wakeup_swing_exit_req_END     (5)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS13_UNION
 struct description   : SCPERSTATUS13 Register structure definition
                        Address Offset:0x394 Initial:0x000 Width:32
 register description : 外设状态寄存器13
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ao_hpm_opc      : 10; /* bit[0-9]  : ao HPM原始性能码。 */
        unsigned int  ao_hpm_opc_vld  : 1;  /* bit[10]   : ao HPM原始性能码有效的状态指示。
                                                           0:not valid 1:valid。 */
        unsigned int  reserved_0      : 1;  /* bit[11]   : 保留。 */
        unsigned int  ao_hpmx_opc     : 10; /* bit[12-21]: ao HPMX原始性能码。 */
        unsigned int  ao_hpmx_opc_vld : 1;  /* bit[22]   : ao HPMX原始性能码有效的状态指示。
                                                           0:not valid 1:valid。 */
        unsigned int  reserved_1      : 9;  /* bit[23-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS13_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS13_ao_hpm_opc_START       (0)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpm_opc_END         (9)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpm_opc_vld_START   (10)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpm_opc_vld_END     (10)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpmx_opc_START      (12)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpmx_opc_END        (21)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpmx_opc_vld_START  (22)
#define SOC_SCTRL_SCPERSTATUS13_ao_hpmx_opc_vld_END    (22)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS14_UNION
 struct description   : SCPERSTATUS14 Register structure definition
                        Address Offset:0x398 Initial:0x0 Width:32
 register description : 外设状态寄存器14
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 10; /* bit[0-9]  :  */
        unsigned int  ao_hpm_opc_min  : 10; /* bit[10-19]:  */
        unsigned int  ao_hpmx_opc_min : 10; /* bit[20-29]:  */
        unsigned int  reserved_1      : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCPERSTATUS14_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS14_ao_hpm_opc_min_START   (10)
#define SOC_SCTRL_SCPERSTATUS14_ao_hpm_opc_min_END     (19)
#define SOC_SCTRL_SCPERSTATUS14_ao_hpmx_opc_min_START  (20)
#define SOC_SCTRL_SCPERSTATUS14_ao_hpmx_opc_min_END    (29)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS15_UNION
 struct description   : SCPERSTATUS15 Register structure definition
                        Address Offset:0x39C Initial:0x0000 Width:32
 register description : 外设状态寄存器15
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddra_reset_state : 1;  /* bit[0]    : 1'b0:处在复位态；
                                                            1'b1:不处在复位态； */
        unsigned int  ddrb_reset_state : 1;  /* bit[1]    : 1'b0:处在复位态；
                                                            1'b1:不处在复位态； */
        unsigned int  ddrc_reset_state : 1;  /* bit[2]    : 1'b0:处在复位态；
                                                            1'b1:不处在复位态； */
        unsigned int  ddrd_reset_state : 1;  /* bit[3]    : 1'b0:处在复位态；
                                                            1'b1:不处在复位态； */
        unsigned int  reserved_0       : 1;  /* bit[4]    :  */
        unsigned int  reserved_1       : 1;  /* bit[5]    :  */
        unsigned int  reserved_2       : 1;  /* bit[6]    :  */
        unsigned int  reserved_3       : 1;  /* bit[7]    :  */
        unsigned int  reserved_4       : 1;  /* bit[8]    :  */
        unsigned int  reserved_5       : 1;  /* bit[9]    :  */
        unsigned int  reserved_6       : 6;  /* bit[10-15]:  */
        unsigned int  reserved_7       : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS15_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS15_ddra_reset_state_START  (0)
#define SOC_SCTRL_SCPERSTATUS15_ddra_reset_state_END    (0)
#define SOC_SCTRL_SCPERSTATUS15_ddrb_reset_state_START  (1)
#define SOC_SCTRL_SCPERSTATUS15_ddrb_reset_state_END    (1)
#define SOC_SCTRL_SCPERSTATUS15_ddrc_reset_state_START  (2)
#define SOC_SCTRL_SCPERSTATUS15_ddrc_reset_state_END    (2)
#define SOC_SCTRL_SCPERSTATUS15_ddrd_reset_state_START  (3)
#define SOC_SCTRL_SCPERSTATUS15_ddrd_reset_state_END    (3)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINNERSTAT_UNION
 struct description   : SCINNERSTAT Register structure definition
                        Address Offset:0x3A0 Initial:0x0 Width:32
 register description : 内部状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  memory_repair_bypass : 1;  /* bit[0]    : Memory repair bypass信号状态，来自efuse。 */
        unsigned int  hardw_ctrl_sel0      : 1;  /* bit[1]    : test_mode0信号经锁存后的状态。 */
        unsigned int  boot_mode            : 2;  /* bit[2-3]  : 只看高bit，低bit不使用 */
        unsigned int  ufs_qspi_sel_pre     : 2;  /* bit[4-5]  : 不使用 */
        unsigned int  reserved_0           : 1;  /* bit[6]    :  */
        unsigned int  reserved_1           : 7;  /* bit[7-13] :  */
        unsigned int  reserved_2           : 1;  /* bit[14]   :  */
        unsigned int  ufs_qspi_sel         : 1;  /* bit[15]   : 不使用 */
        unsigned int  ate_mode             : 5;  /* bit[16-20]: 5'B0_0000:FAC_TEST_SEL;
                                                                5'B0_0010:BISR_EFUSE_SEL;
                                                                5'B0_0100:MDM_GPU_TSENSOR_SEL
                                                                5'B0_0110:HISEE_TESENSOR_SEL
                                                                5'B0_1001:ABB_TEST_SEL
                                                                5'B0_1010:ODIO_PLOOP_SEL
                                                                5'B0_1011:ODIO_NLOOP_SEL
                                                                5'B0_1100:TSENSOR_TEST_SEL
                                                                5'B0_1101:SYS_EFUSE_SEL
                                                                5'B0_1110:MTCMOS_TEST_SEL
                                                                5'B0_1111:OTP_TEST_SEL
                                                                5'B0_0001:AUDIO_TEST_SEL */
        unsigned int  reserved_3           : 1;  /* bit[21]   :  */
        unsigned int  reserved_4           : 6;  /* bit[22-27]:  */
        unsigned int  reserved_5           : 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCINNERSTAT_UNION;
#endif
#define SOC_SCTRL_SCINNERSTAT_memory_repair_bypass_START  (0)
#define SOC_SCTRL_SCINNERSTAT_memory_repair_bypass_END    (0)
#define SOC_SCTRL_SCINNERSTAT_hardw_ctrl_sel0_START       (1)
#define SOC_SCTRL_SCINNERSTAT_hardw_ctrl_sel0_END         (1)
#define SOC_SCTRL_SCINNERSTAT_boot_mode_START             (2)
#define SOC_SCTRL_SCINNERSTAT_boot_mode_END               (3)
#define SOC_SCTRL_SCINNERSTAT_ufs_qspi_sel_pre_START      (4)
#define SOC_SCTRL_SCINNERSTAT_ufs_qspi_sel_pre_END        (5)
#define SOC_SCTRL_SCINNERSTAT_ufs_qspi_sel_START          (15)
#define SOC_SCTRL_SCINNERSTAT_ufs_qspi_sel_END            (15)
#define SOC_SCTRL_SCINNERSTAT_ate_mode_START              (16)
#define SOC_SCTRL_SCINNERSTAT_ate_mode_END                (20)


/*****************************************************************************
 struct               : SOC_SCTRL_SCINNERCTRL_UNION
 struct description   : SCINNERCTRL Register structure definition
                        Address Offset:0x3A4 Initial:0x00000000 Width:32
 register description : 内部使用的配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mali_dbgack_byp : 1;  /* bit[0]   : 0：bypass无效；
                                                          1：bypass有效。 */
        unsigned int  reserved        : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCINNERCTRL_UNION;
#endif
#define SOC_SCTRL_SCINNERCTRL_mali_dbgack_byp_START  (0)
#define SOC_SCTRL_SCINNERCTRL_mali_dbgack_byp_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCMRBBUSYSTAT_UNION
 struct description   : SCMRBBUSYSTAT Register structure definition
                        Address Offset:0x3FC Initial:0x00000000 Width:32
 register description : MRB EFUSE BUSY状态指示寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mrb_busy_flag : 32; /* bit[0-31]: CS删除 */
    } reg;
} SOC_SCTRL_SCMRBBUSYSTAT_UNION;
#endif
#define SOC_SCTRL_SCMRBBUSYSTAT_mrb_busy_flag_START  (0)
#define SOC_SCTRL_SCMRBBUSYSTAT_mrb_busy_flag_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSWADDR_UNION
 struct description   : SCSWADDR Register structure definition
                        Address Offset:0x400 Initial:0x00000000 Width:32
 register description : 用于保存软件入口地址的寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  swaddr : 32; /* bit[0-31]: 保存软件的入口地址。 */
    } reg;
} SOC_SCTRL_SCSWADDR_UNION;
#endif
#define SOC_SCTRL_SCSWADDR_swaddr_START  (0)
#define SOC_SCTRL_SCSWADDR_swaddr_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCDDRADDR_UNION
 struct description   : SCDDRADDR Register structure definition
                        Address Offset:0x404 Initial:0x00000000 Width:32
 register description : 用于保留DDR训练数据的地址的寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddrtrainaddr : 32; /* bit[0-31]: 保存DDR训练数据的地址。 */
    } reg;
} SOC_SCTRL_SCDDRADDR_UNION;
#endif
#define SOC_SCTRL_SCDDRADDR_ddrtrainaddr_START  (0)
#define SOC_SCTRL_SCDDRADDR_ddrtrainaddr_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCDDRDATA_UNION
 struct description   : SCDDRDATA Register structure definition
                        Address Offset:0x408 Initial:0x00000000 Width:32
 register description : 用于保存DDR训练数据的寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddrtraindata : 32; /* bit[0-31]: 保存DDR的训练数据。 */
    } reg;
} SOC_SCTRL_SCDDRDATA_UNION;
#endif
#define SOC_SCTRL_SCDDRDATA_ddrtraindata_START  (0)
#define SOC_SCTRL_SCDDRDATA_ddrtraindata_END    (31)


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
 struct               : SOC_SCTRL_SCBAKDATA1_UNION
 struct description   : SCBAKDATA1 Register structure definition
                        Address Offset:0x410 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器1，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA1_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA1_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA1_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA2_UNION
 struct description   : SCBAKDATA2 Register structure definition
                        Address Offset:0x414 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器2，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA2_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA2_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA2_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA3_UNION
 struct description   : SCBAKDATA3 Register structure definition
                        Address Offset:0x418 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器3，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA3_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA3_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA3_bakdata_END    (31)


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
 struct               : SOC_SCTRL_SCBAKDATA5_UNION
 struct description   : SCBAKDATA5 Register structure definition
                        Address Offset:0x420 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器5，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA5_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA5_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA5_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA6_UNION
 struct description   : SCBAKDATA6 Register structure definition
                        Address Offset:0x424 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器6，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA6_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA6_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA6_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA7_UNION
 struct description   : SCBAKDATA7 Register structure definition
                        Address Offset:0x428 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器7，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA7_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA7_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA7_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA8_UNION
 struct description   : SCBAKDATA8 Register structure definition
                        Address Offset:0x42C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器8，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA8_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA8_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA8_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA9_UNION
 struct description   : SCBAKDATA9 Register structure definition
                        Address Offset:0x430 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器9
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器9，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA9_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA9_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA9_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA10_UNION
 struct description   : SCBAKDATA10 Register structure definition
                        Address Offset:0x434 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器10
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器10，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA10_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA10_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA10_bakdata_END    (31)


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
 struct               : SOC_SCTRL_SCBAKDATA12_UNION
 struct description   : SCBAKDATA12 Register structure definition
                        Address Offset:0x43C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器12
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器12，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA12_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA12_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA12_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA13_UNION
 struct description   : SCBAKDATA13 Register structure definition
                        Address Offset:0x440 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器13
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器13，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA13_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA13_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA13_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA14_UNION
 struct description   : SCBAKDATA14 Register structure definition
                        Address Offset:0x444 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器14
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器14，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA14_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA14_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA14_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA15_UNION
 struct description   : SCBAKDATA15 Register structure definition
                        Address Offset:0x448 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器15
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器15，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA15_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA15_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA15_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA16_UNION
 struct description   : SCBAKDATA16 Register structure definition
                        Address Offset:0x44C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器16
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器16，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA16_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA16_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA16_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA17_UNION
 struct description   : SCBAKDATA17 Register structure definition
                        Address Offset:0x450 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器17
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器17，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA17_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA17_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA17_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA18_UNION
 struct description   : SCBAKDATA18 Register structure definition
                        Address Offset:0x454 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器18
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器18，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA18_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA18_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA18_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA19_UNION
 struct description   : SCBAKDATA19 Register structure definition
                        Address Offset:0x458 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器19
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器19，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA19_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA19_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA19_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA20_UNION
 struct description   : SCBAKDATA20 Register structure definition
                        Address Offset:0x45C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器20
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器20，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA20_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA20_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA20_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA21_UNION
 struct description   : SCBAKDATA21 Register structure definition
                        Address Offset:0x460 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器21
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器21，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA21_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA21_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA21_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA22_UNION
 struct description   : SCBAKDATA22 Register structure definition
                        Address Offset:0x464 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器22
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器22，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA22_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA22_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA22_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA23_UNION
 struct description   : SCBAKDATA23 Register structure definition
                        Address Offset:0x468 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器23
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器23，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA23_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA23_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA23_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA24_UNION
 struct description   : SCBAKDATA24 Register structure definition
                        Address Offset:0x46C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器24
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器24，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA24_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA24_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA24_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA25_UNION
 struct description   : SCBAKDATA25 Register structure definition
                        Address Offset:0x470 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器25
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器25，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA25_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA25_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA25_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA26_UNION
 struct description   : SCBAKDATA26 Register structure definition
                        Address Offset:0x474 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器26
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器26，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA26_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA26_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA26_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA27_UNION
 struct description   : SCBAKDATA27 Register structure definition
                        Address Offset:0x478 Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器27
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器27，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA27_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA27_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA27_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA28_UNION
 struct description   : SCBAKDATA28 Register structure definition
                        Address Offset:0x47C Initial:0x00000000 Width:32
 register description : 用于软件使用的存储数据寄存器28
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bakdata : 32; /* bit[0-31]: 给软件预留的数据寄存器28，用于临时数据的缓存。 */
    } reg;
} SOC_SCTRL_SCBAKDATA28_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA28_bakdata_START  (0)
#define SOC_SCTRL_SCBAKDATA28_bakdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS0_UNION
 struct description   : SCPERSTAT_POR_RESERVED_NS0 Register structure definition
                        Address Offset:0x480 Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS1_UNION
 struct description   : SCPERSTAT_POR_RESERVED_NS1 Register structure definition
                        Address Offset:0x484 Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS1_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS2_UNION
 struct description   : SCPERSTAT_POR_RESERVED_NS2 Register structure definition
                        Address Offset:0x488 Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS3_UNION
 struct description   : SCPERSTAT_POR_RESERVED_NS3 Register structure definition
                        Address Offset:0x48C Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED_NS3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA0_MSK_UNION
 struct description   : SCBAKDATA0_MSK Register structure definition
                        Address Offset:0x4C0 Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata0_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA0_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA0_MSK_scbakdata0_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA0_MSK_scbakdata0_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA1_MSK_UNION
 struct description   : SCBAKDATA1_MSK Register structure definition
                        Address Offset:0x4C4 Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata1_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA1_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA1_MSK_scbakdata1_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA1_MSK_scbakdata1_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA2_MSK_UNION
 struct description   : SCBAKDATA2_MSK Register structure definition
                        Address Offset:0x4C8 Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata2_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA2_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA2_MSK_scbakdata2_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA2_MSK_scbakdata2_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA3_MSK_UNION
 struct description   : SCBAKDATA3_MSK Register structure definition
                        Address Offset:0x4CC Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata3_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA3_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA3_MSK_scbakdata3_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA3_MSK_scbakdata3_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA4_MSK_UNION
 struct description   : SCBAKDATA4_MSK Register structure definition
                        Address Offset:0x4D0 Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata4_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA4_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA4_MSK_scbakdata4_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA4_MSK_scbakdata4_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA5_MSK_UNION
 struct description   : SCBAKDATA5_MSK Register structure definition
                        Address Offset:0x4D4 Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata5_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA5_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA5_MSK_scbakdata5_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA5_MSK_scbakdata5_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA6_MSK_UNION
 struct description   : SCBAKDATA6_MSK Register structure definition
                        Address Offset:0x4D8 Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata6_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA6_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA6_MSK_scbakdata6_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA6_MSK_scbakdata6_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBAKDATA7_MSK_UNION
 struct description   : SCBAKDATA7_MSK Register structure definition
                        Address Offset:0x4DC Initial:0x0000 Width:32
 register description : 备份寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved           : 16; /* bit[0-15] :  */
        unsigned int  scbakdata7_msk_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCBAKDATA7_MSK_UNION;
#endif
#define SOC_SCTRL_SCBAKDATA7_MSK_scbakdata7_msk_msk_START  (16)
#define SOC_SCTRL_SCBAKDATA7_MSK_scbakdata7_msk_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCUCLKEN_UNION
 struct description   : SCLPMCUCLKEN Register structure definition
                        Address Offset:0x960 Initial:0x0 Width:32
 register description : LPMCU子系统时钟使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dapclkoff_sys_n            : 1;  /* bit[0] :  */
        unsigned int  auto_clkgt_h2p_lpperi_en   : 1;  /* bit[1] :  */
        unsigned int  tcm_clk_clkoff_sys_n       : 1;  /* bit[2] :  */
        unsigned int  ram_clk_clkoff_sys_n       : 1;  /* bit[3] :  */
        unsigned int  aon_clk_clkoff_sys_n       : 1;  /* bit[4] :  */
        unsigned int  reserved_0                 : 1;  /* bit[5] :  */
        unsigned int  tsen_clk_clkoff_sys_n      : 1;  /* bit[6] :  */
        unsigned int  pmc_clk_clkoff_sys_n       : 1;  /* bit[7] :  */
        unsigned int  uart_clk_clkoff_sys_n      : 1;  /* bit[8] :  */
        unsigned int  funcmbist_clk_clkoff_sys_n : 1;  /* bit[9] :  */
        unsigned int  reserved_1                 : 1;  /* bit[10]:  */
        unsigned int  pcrg_clk_clkoff_sys_n      : 1;  /* bit[11]:  */
        unsigned int  reserved_2                 : 1;  /* bit[12]:  */
        unsigned int  reserved_3                 : 1;  /* bit[13]:  */
        unsigned int  tim_clk_clkoff_sys_n       : 1;  /* bit[14]:  */
        unsigned int  cssys_clk_clkoff_sys_n     : 1;  /* bit[15]:  */
        unsigned int  spi_clk_clkoff_sys_n       : 1;  /* bit[16]:  */
        unsigned int  reserved_4                 : 1;  /* bit[17]:  */
        unsigned int  reserved_5                 : 1;  /* bit[18]:  */
        unsigned int  reserved_6                 : 1;  /* bit[19]:  */
        unsigned int  reserved_7                 : 1;  /* bit[20]:  */
        unsigned int  reserved_8                 : 1;  /* bit[21]:  */
        unsigned int  reserved_9                 : 1;  /* bit[22]:  */
        unsigned int  reserved_10                : 1;  /* bit[23]:  */
        unsigned int  reserved_11                : 1;  /* bit[24]:  */
        unsigned int  reserved_12                : 1;  /* bit[25]:  */
        unsigned int  reserved_13                : 1;  /* bit[26]:  */
        unsigned int  reserved_14                : 1;  /* bit[27]:  */
        unsigned int  reserved_15                : 1;  /* bit[28]:  */
        unsigned int  reserved_16                : 1;  /* bit[29]:  */
        unsigned int  reserved_17                : 1;  /* bit[30]:  */
        unsigned int  reserved_18                : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCUCLKEN_UNION;
#endif
#define SOC_SCTRL_SCLPMCUCLKEN_dapclkoff_sys_n_START             (0)
#define SOC_SCTRL_SCLPMCUCLKEN_dapclkoff_sys_n_END               (0)
#define SOC_SCTRL_SCLPMCUCLKEN_auto_clkgt_h2p_lpperi_en_START    (1)
#define SOC_SCTRL_SCLPMCUCLKEN_auto_clkgt_h2p_lpperi_en_END      (1)
#define SOC_SCTRL_SCLPMCUCLKEN_tcm_clk_clkoff_sys_n_START        (2)
#define SOC_SCTRL_SCLPMCUCLKEN_tcm_clk_clkoff_sys_n_END          (2)
#define SOC_SCTRL_SCLPMCUCLKEN_ram_clk_clkoff_sys_n_START        (3)
#define SOC_SCTRL_SCLPMCUCLKEN_ram_clk_clkoff_sys_n_END          (3)
#define SOC_SCTRL_SCLPMCUCLKEN_aon_clk_clkoff_sys_n_START        (4)
#define SOC_SCTRL_SCLPMCUCLKEN_aon_clk_clkoff_sys_n_END          (4)
#define SOC_SCTRL_SCLPMCUCLKEN_tsen_clk_clkoff_sys_n_START       (6)
#define SOC_SCTRL_SCLPMCUCLKEN_tsen_clk_clkoff_sys_n_END         (6)
#define SOC_SCTRL_SCLPMCUCLKEN_pmc_clk_clkoff_sys_n_START        (7)
#define SOC_SCTRL_SCLPMCUCLKEN_pmc_clk_clkoff_sys_n_END          (7)
#define SOC_SCTRL_SCLPMCUCLKEN_uart_clk_clkoff_sys_n_START       (8)
#define SOC_SCTRL_SCLPMCUCLKEN_uart_clk_clkoff_sys_n_END         (8)
#define SOC_SCTRL_SCLPMCUCLKEN_funcmbist_clk_clkoff_sys_n_START  (9)
#define SOC_SCTRL_SCLPMCUCLKEN_funcmbist_clk_clkoff_sys_n_END    (9)
#define SOC_SCTRL_SCLPMCUCLKEN_pcrg_clk_clkoff_sys_n_START       (11)
#define SOC_SCTRL_SCLPMCUCLKEN_pcrg_clk_clkoff_sys_n_END         (11)
#define SOC_SCTRL_SCLPMCUCLKEN_tim_clk_clkoff_sys_n_START        (14)
#define SOC_SCTRL_SCLPMCUCLKEN_tim_clk_clkoff_sys_n_END          (14)
#define SOC_SCTRL_SCLPMCUCLKEN_cssys_clk_clkoff_sys_n_START      (15)
#define SOC_SCTRL_SCLPMCUCLKEN_cssys_clk_clkoff_sys_n_END        (15)
#define SOC_SCTRL_SCLPMCUCLKEN_spi_clk_clkoff_sys_n_START        (16)
#define SOC_SCTRL_SCLPMCUCLKEN_spi_clk_clkoff_sys_n_END          (16)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCUCLKDIS_UNION
 struct description   : SCLPMCUCLKDIS Register structure definition
                        Address Offset:0x964 Initial:0x0 Width:32
 register description : LPMCU子系统软时钟关闭寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dapclkoff_sys_n            : 1;  /* bit[0] :  */
        unsigned int  auto_clkgt_h2p_lpperi_en   : 1;  /* bit[1] :  */
        unsigned int  tcm_clk_clkoff_sys_n       : 1;  /* bit[2] :  */
        unsigned int  ram_clk_clkoff_sys_n       : 1;  /* bit[3] :  */
        unsigned int  aon_clk_clkoff_sys_n       : 1;  /* bit[4] :  */
        unsigned int  reserved_0                 : 1;  /* bit[5] :  */
        unsigned int  tsen_clk_clkoff_sys_n      : 1;  /* bit[6] :  */
        unsigned int  pmc_clk_clkoff_sys_n       : 1;  /* bit[7] :  */
        unsigned int  uart_clk_clkoff_sys_n      : 1;  /* bit[8] :  */
        unsigned int  funcmbist_clk_clkoff_sys_n : 1;  /* bit[9] :  */
        unsigned int  reserved_1                 : 1;  /* bit[10]:  */
        unsigned int  pcrg_clk_clkoff_sys_n      : 1;  /* bit[11]:  */
        unsigned int  reserved_2                 : 1;  /* bit[12]:  */
        unsigned int  reserved_3                 : 1;  /* bit[13]:  */
        unsigned int  tim_clk_clkoff_sys_n       : 1;  /* bit[14]:  */
        unsigned int  cssys_clk_clkoff_sys_n     : 1;  /* bit[15]:  */
        unsigned int  spi_clk_clkoff_sys_n       : 1;  /* bit[16]:  */
        unsigned int  reserved_4                 : 1;  /* bit[17]:  */
        unsigned int  reserved_5                 : 1;  /* bit[18]:  */
        unsigned int  reserved_6                 : 1;  /* bit[19]:  */
        unsigned int  reserved_7                 : 1;  /* bit[20]:  */
        unsigned int  reserved_8                 : 1;  /* bit[21]:  */
        unsigned int  reserved_9                 : 1;  /* bit[22]:  */
        unsigned int  reserved_10                : 1;  /* bit[23]:  */
        unsigned int  reserved_11                : 1;  /* bit[24]:  */
        unsigned int  reserved_12                : 1;  /* bit[25]:  */
        unsigned int  reserved_13                : 1;  /* bit[26]:  */
        unsigned int  reserved_14                : 1;  /* bit[27]:  */
        unsigned int  reserved_15                : 1;  /* bit[28]:  */
        unsigned int  reserved_16                : 1;  /* bit[29]:  */
        unsigned int  reserved_17                : 1;  /* bit[30]:  */
        unsigned int  reserved_18                : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCUCLKDIS_UNION;
#endif
#define SOC_SCTRL_SCLPMCUCLKDIS_dapclkoff_sys_n_START             (0)
#define SOC_SCTRL_SCLPMCUCLKDIS_dapclkoff_sys_n_END               (0)
#define SOC_SCTRL_SCLPMCUCLKDIS_auto_clkgt_h2p_lpperi_en_START    (1)
#define SOC_SCTRL_SCLPMCUCLKDIS_auto_clkgt_h2p_lpperi_en_END      (1)
#define SOC_SCTRL_SCLPMCUCLKDIS_tcm_clk_clkoff_sys_n_START        (2)
#define SOC_SCTRL_SCLPMCUCLKDIS_tcm_clk_clkoff_sys_n_END          (2)
#define SOC_SCTRL_SCLPMCUCLKDIS_ram_clk_clkoff_sys_n_START        (3)
#define SOC_SCTRL_SCLPMCUCLKDIS_ram_clk_clkoff_sys_n_END          (3)
#define SOC_SCTRL_SCLPMCUCLKDIS_aon_clk_clkoff_sys_n_START        (4)
#define SOC_SCTRL_SCLPMCUCLKDIS_aon_clk_clkoff_sys_n_END          (4)
#define SOC_SCTRL_SCLPMCUCLKDIS_tsen_clk_clkoff_sys_n_START       (6)
#define SOC_SCTRL_SCLPMCUCLKDIS_tsen_clk_clkoff_sys_n_END         (6)
#define SOC_SCTRL_SCLPMCUCLKDIS_pmc_clk_clkoff_sys_n_START        (7)
#define SOC_SCTRL_SCLPMCUCLKDIS_pmc_clk_clkoff_sys_n_END          (7)
#define SOC_SCTRL_SCLPMCUCLKDIS_uart_clk_clkoff_sys_n_START       (8)
#define SOC_SCTRL_SCLPMCUCLKDIS_uart_clk_clkoff_sys_n_END         (8)
#define SOC_SCTRL_SCLPMCUCLKDIS_funcmbist_clk_clkoff_sys_n_START  (9)
#define SOC_SCTRL_SCLPMCUCLKDIS_funcmbist_clk_clkoff_sys_n_END    (9)
#define SOC_SCTRL_SCLPMCUCLKDIS_pcrg_clk_clkoff_sys_n_START       (11)
#define SOC_SCTRL_SCLPMCUCLKDIS_pcrg_clk_clkoff_sys_n_END         (11)
#define SOC_SCTRL_SCLPMCUCLKDIS_tim_clk_clkoff_sys_n_START        (14)
#define SOC_SCTRL_SCLPMCUCLKDIS_tim_clk_clkoff_sys_n_END          (14)
#define SOC_SCTRL_SCLPMCUCLKDIS_cssys_clk_clkoff_sys_n_START      (15)
#define SOC_SCTRL_SCLPMCUCLKDIS_cssys_clk_clkoff_sys_n_END        (15)
#define SOC_SCTRL_SCLPMCUCLKDIS_spi_clk_clkoff_sys_n_START        (16)
#define SOC_SCTRL_SCLPMCUCLKDIS_spi_clk_clkoff_sys_n_END          (16)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCUCLKSTAT_UNION
 struct description   : SCLPMCUCLKSTAT Register structure definition
                        Address Offset:0x968 Initial:0x0 Width:32
 register description : LPMCU子系统软时钟状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dapclkoff_sys_n            : 1;  /* bit[0] : M3 DAPCLK时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  auto_clkgt_h2p_lpperi_en   : 1;  /* bit[1] : Lpmcu apb slv h2p桥时钟自动门控控制
                                                                   0:无自动时钟门控逻辑，时钟常开
                                                                   1:有自动时钟门控逻辑
                                                                   软件配置该寄存器为1 时需要在初始化时，确保任何操作均没有发到桥上； */
        unsigned int  tcm_clk_clkoff_sys_n       : 1;  /* bit[2] :  */
        unsigned int  ram_clk_clkoff_sys_n       : 1;  /* bit[3] :  */
        unsigned int  aon_clk_clkoff_sys_n       : 1;  /* bit[4] : AON_SUBSYS访问接口的时钟门控系统控制。0：无效；1：开启时钟。不使用. */
        unsigned int  reserved_0                 : 1;  /* bit[5] : DIG TSENSORC的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  tsen_clk_clkoff_sys_n      : 1;  /* bit[6] : TSENSORC的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  pmc_clk_clkoff_sys_n       : 1;  /* bit[7] : PMC的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  uart_clk_clkoff_sys_n      : 1;  /* bit[8] : UART的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  funcmbist_clk_clkoff_sys_n : 1;  /* bit[9] : PMU_I2C0的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  reserved_1                 : 1;  /* bit[10]:  */
        unsigned int  pcrg_clk_clkoff_sys_n      : 1;  /* bit[11]: 外设CRG配置接口的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  reserved_2                 : 1;  /* bit[12]: 保留。 */
        unsigned int  reserved_3                 : 1;  /* bit[13]: LP_WDG的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  tim_clk_clkoff_sys_n       : 1;  /* bit[14]: LP_TIMER的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  cssys_clk_clkoff_sys_n     : 1;  /* bit[15]: CSSYS访问接口的时钟门控系统控制。0：无效；1：开启时钟。 */
        unsigned int  spi_clk_clkoff_sys_n       : 1;  /* bit[16]:  */
        unsigned int  reserved_4                 : 1;  /* bit[17]:  */
        unsigned int  reserved_5                 : 1;  /* bit[18]:  */
        unsigned int  reserved_6                 : 1;  /* bit[19]:  */
        unsigned int  reserved_7                 : 1;  /* bit[20]:  */
        unsigned int  reserved_8                 : 1;  /* bit[21]:  */
        unsigned int  reserved_9                 : 1;  /* bit[22]:  */
        unsigned int  reserved_10                : 1;  /* bit[23]:  */
        unsigned int  reserved_11                : 1;  /* bit[24]:  */
        unsigned int  reserved_12                : 1;  /* bit[25]:  */
        unsigned int  reserved_13                : 1;  /* bit[26]:  */
        unsigned int  reserved_14                : 1;  /* bit[27]:  */
        unsigned int  reserved_15                : 1;  /* bit[28]:  */
        unsigned int  reserved_16                : 1;  /* bit[29]:  */
        unsigned int  reserved_17                : 1;  /* bit[30]:  */
        unsigned int  reserved_18                : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCUCLKSTAT_UNION;
#endif
#define SOC_SCTRL_SCLPMCUCLKSTAT_dapclkoff_sys_n_START             (0)
#define SOC_SCTRL_SCLPMCUCLKSTAT_dapclkoff_sys_n_END               (0)
#define SOC_SCTRL_SCLPMCUCLKSTAT_auto_clkgt_h2p_lpperi_en_START    (1)
#define SOC_SCTRL_SCLPMCUCLKSTAT_auto_clkgt_h2p_lpperi_en_END      (1)
#define SOC_SCTRL_SCLPMCUCLKSTAT_tcm_clk_clkoff_sys_n_START        (2)
#define SOC_SCTRL_SCLPMCUCLKSTAT_tcm_clk_clkoff_sys_n_END          (2)
#define SOC_SCTRL_SCLPMCUCLKSTAT_ram_clk_clkoff_sys_n_START        (3)
#define SOC_SCTRL_SCLPMCUCLKSTAT_ram_clk_clkoff_sys_n_END          (3)
#define SOC_SCTRL_SCLPMCUCLKSTAT_aon_clk_clkoff_sys_n_START        (4)
#define SOC_SCTRL_SCLPMCUCLKSTAT_aon_clk_clkoff_sys_n_END          (4)
#define SOC_SCTRL_SCLPMCUCLKSTAT_tsen_clk_clkoff_sys_n_START       (6)
#define SOC_SCTRL_SCLPMCUCLKSTAT_tsen_clk_clkoff_sys_n_END         (6)
#define SOC_SCTRL_SCLPMCUCLKSTAT_pmc_clk_clkoff_sys_n_START        (7)
#define SOC_SCTRL_SCLPMCUCLKSTAT_pmc_clk_clkoff_sys_n_END          (7)
#define SOC_SCTRL_SCLPMCUCLKSTAT_uart_clk_clkoff_sys_n_START       (8)
#define SOC_SCTRL_SCLPMCUCLKSTAT_uart_clk_clkoff_sys_n_END         (8)
#define SOC_SCTRL_SCLPMCUCLKSTAT_funcmbist_clk_clkoff_sys_n_START  (9)
#define SOC_SCTRL_SCLPMCUCLKSTAT_funcmbist_clk_clkoff_sys_n_END    (9)
#define SOC_SCTRL_SCLPMCUCLKSTAT_pcrg_clk_clkoff_sys_n_START       (11)
#define SOC_SCTRL_SCLPMCUCLKSTAT_pcrg_clk_clkoff_sys_n_END         (11)
#define SOC_SCTRL_SCLPMCUCLKSTAT_tim_clk_clkoff_sys_n_START        (14)
#define SOC_SCTRL_SCLPMCUCLKSTAT_tim_clk_clkoff_sys_n_END          (14)
#define SOC_SCTRL_SCLPMCUCLKSTAT_cssys_clk_clkoff_sys_n_START      (15)
#define SOC_SCTRL_SCLPMCUCLKSTAT_cssys_clk_clkoff_sys_n_END        (15)
#define SOC_SCTRL_SCLPMCUCLKSTAT_spi_clk_clkoff_sys_n_START        (16)
#define SOC_SCTRL_SCLPMCUCLKSTAT_spi_clk_clkoff_sys_n_END          (16)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCURSTEN_UNION
 struct description   : SCLPMCURSTEN Register structure definition
                        Address Offset:0x970 Initial:0x0 Width:32
 register description : LPMCU子系统软复位使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  rst_software_req       : 1;  /* bit[0] :  */
        unsigned int  coresight_soft_rst_req : 1;  /* bit[1] :  */
        unsigned int  reserved_0             : 1;  /* bit[2] :  */
        unsigned int  tcm_soft_rst_req       : 1;  /* bit[3] :  */
        unsigned int  ram_soft_rst_req       : 1;  /* bit[4] :  */
        unsigned int  aon_soft_rst_req       : 1;  /* bit[5] :  */
        unsigned int  ccrg_soft_rst_req      : 1;  /* bit[6] :  */
        unsigned int  tsen_soft_rst_req      : 1;  /* bit[7] :  */
        unsigned int  pmc_soft_rst_req       : 1;  /* bit[8] :  */
        unsigned int  uart_soft_rst_req      : 1;  /* bit[9] :  */
        unsigned int  funcmbist_soft_rst_req : 1;  /* bit[10]:  */
        unsigned int  reserved_1             : 1;  /* bit[11]:  */
        unsigned int  pcrg_soft_rst_req      : 1;  /* bit[12]:  */
        unsigned int  reserved_2             : 1;  /* bit[13]:  */
        unsigned int  reserved_3             : 1;  /* bit[14]:  */
        unsigned int  tim_soft_rst_req       : 1;  /* bit[15]:  */
        unsigned int  cssys_soft_rst_req     : 1;  /* bit[16]:  */
        unsigned int  spi_soft_rst_req       : 1;  /* bit[17]:  */
        unsigned int  reserved_4             : 1;  /* bit[18]:  */
        unsigned int  reserved_5             : 1;  /* bit[19]:  */
        unsigned int  reserved_6             : 1;  /* bit[20]:  */
        unsigned int  reserved_7             : 1;  /* bit[21]:  */
        unsigned int  reserved_8             : 1;  /* bit[22]:  */
        unsigned int  reserved_9             : 1;  /* bit[23]:  */
        unsigned int  reserved_10            : 1;  /* bit[24]:  */
        unsigned int  reserved_11            : 1;  /* bit[25]:  */
        unsigned int  reserved_12            : 1;  /* bit[26]:  */
        unsigned int  reserved_13            : 1;  /* bit[27]:  */
        unsigned int  reserved_14            : 1;  /* bit[28]:  */
        unsigned int  reserved_15            : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCURSTEN_UNION;
#endif
#define SOC_SCTRL_SCLPMCURSTEN_rst_software_req_START        (0)
#define SOC_SCTRL_SCLPMCURSTEN_rst_software_req_END          (0)
#define SOC_SCTRL_SCLPMCURSTEN_coresight_soft_rst_req_START  (1)
#define SOC_SCTRL_SCLPMCURSTEN_coresight_soft_rst_req_END    (1)
#define SOC_SCTRL_SCLPMCURSTEN_tcm_soft_rst_req_START        (3)
#define SOC_SCTRL_SCLPMCURSTEN_tcm_soft_rst_req_END          (3)
#define SOC_SCTRL_SCLPMCURSTEN_ram_soft_rst_req_START        (4)
#define SOC_SCTRL_SCLPMCURSTEN_ram_soft_rst_req_END          (4)
#define SOC_SCTRL_SCLPMCURSTEN_aon_soft_rst_req_START        (5)
#define SOC_SCTRL_SCLPMCURSTEN_aon_soft_rst_req_END          (5)
#define SOC_SCTRL_SCLPMCURSTEN_ccrg_soft_rst_req_START       (6)
#define SOC_SCTRL_SCLPMCURSTEN_ccrg_soft_rst_req_END         (6)
#define SOC_SCTRL_SCLPMCURSTEN_tsen_soft_rst_req_START       (7)
#define SOC_SCTRL_SCLPMCURSTEN_tsen_soft_rst_req_END         (7)
#define SOC_SCTRL_SCLPMCURSTEN_pmc_soft_rst_req_START        (8)
#define SOC_SCTRL_SCLPMCURSTEN_pmc_soft_rst_req_END          (8)
#define SOC_SCTRL_SCLPMCURSTEN_uart_soft_rst_req_START       (9)
#define SOC_SCTRL_SCLPMCURSTEN_uart_soft_rst_req_END         (9)
#define SOC_SCTRL_SCLPMCURSTEN_funcmbist_soft_rst_req_START  (10)
#define SOC_SCTRL_SCLPMCURSTEN_funcmbist_soft_rst_req_END    (10)
#define SOC_SCTRL_SCLPMCURSTEN_pcrg_soft_rst_req_START       (12)
#define SOC_SCTRL_SCLPMCURSTEN_pcrg_soft_rst_req_END         (12)
#define SOC_SCTRL_SCLPMCURSTEN_tim_soft_rst_req_START        (15)
#define SOC_SCTRL_SCLPMCURSTEN_tim_soft_rst_req_END          (15)
#define SOC_SCTRL_SCLPMCURSTEN_cssys_soft_rst_req_START      (16)
#define SOC_SCTRL_SCLPMCURSTEN_cssys_soft_rst_req_END        (16)
#define SOC_SCTRL_SCLPMCURSTEN_spi_soft_rst_req_START        (17)
#define SOC_SCTRL_SCLPMCURSTEN_spi_soft_rst_req_END          (17)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCURSTDIS_UNION
 struct description   : SCLPMCURSTDIS Register structure definition
                        Address Offset:0x974 Initial:0x0 Width:32
 register description : LPMCU子系统软复位撤离寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  rst_software_req       : 1;  /* bit[0] :  */
        unsigned int  coresight_soft_rst_req : 1;  /* bit[1] :  */
        unsigned int  reserved_0             : 1;  /* bit[2] :  */
        unsigned int  tcm_soft_rst_req       : 1;  /* bit[3] :  */
        unsigned int  ram_soft_rst_req       : 1;  /* bit[4] :  */
        unsigned int  aon_soft_rst_req       : 1;  /* bit[5] :  */
        unsigned int  ccrg_soft_rst_req      : 1;  /* bit[6] :  */
        unsigned int  tsen_soft_rst_req      : 1;  /* bit[7] :  */
        unsigned int  pmc_soft_rst_req       : 1;  /* bit[8] :  */
        unsigned int  uart_soft_rst_req      : 1;  /* bit[9] :  */
        unsigned int  funcmbist_soft_rst_req : 1;  /* bit[10]:  */
        unsigned int  reserved_1             : 1;  /* bit[11]:  */
        unsigned int  pcrg_soft_rst_req      : 1;  /* bit[12]:  */
        unsigned int  reserved_2             : 1;  /* bit[13]:  */
        unsigned int  reserved_3             : 1;  /* bit[14]:  */
        unsigned int  tim_soft_rst_req       : 1;  /* bit[15]:  */
        unsigned int  cssys_soft_rst_req     : 1;  /* bit[16]:  */
        unsigned int  spi_soft_rst_req       : 1;  /* bit[17]:  */
        unsigned int  reserved_4             : 1;  /* bit[18]:  */
        unsigned int  reserved_5             : 1;  /* bit[19]:  */
        unsigned int  reserved_6             : 1;  /* bit[20]:  */
        unsigned int  reserved_7             : 1;  /* bit[21]:  */
        unsigned int  reserved_8             : 1;  /* bit[22]:  */
        unsigned int  reserved_9             : 1;  /* bit[23]:  */
        unsigned int  reserved_10            : 1;  /* bit[24]:  */
        unsigned int  reserved_11            : 1;  /* bit[25]:  */
        unsigned int  reserved_12            : 1;  /* bit[26]:  */
        unsigned int  reserved_13            : 1;  /* bit[27]:  */
        unsigned int  reserved_14            : 1;  /* bit[28]:  */
        unsigned int  reserved_15            : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCURSTDIS_UNION;
#endif
#define SOC_SCTRL_SCLPMCURSTDIS_rst_software_req_START        (0)
#define SOC_SCTRL_SCLPMCURSTDIS_rst_software_req_END          (0)
#define SOC_SCTRL_SCLPMCURSTDIS_coresight_soft_rst_req_START  (1)
#define SOC_SCTRL_SCLPMCURSTDIS_coresight_soft_rst_req_END    (1)
#define SOC_SCTRL_SCLPMCURSTDIS_tcm_soft_rst_req_START        (3)
#define SOC_SCTRL_SCLPMCURSTDIS_tcm_soft_rst_req_END          (3)
#define SOC_SCTRL_SCLPMCURSTDIS_ram_soft_rst_req_START        (4)
#define SOC_SCTRL_SCLPMCURSTDIS_ram_soft_rst_req_END          (4)
#define SOC_SCTRL_SCLPMCURSTDIS_aon_soft_rst_req_START        (5)
#define SOC_SCTRL_SCLPMCURSTDIS_aon_soft_rst_req_END          (5)
#define SOC_SCTRL_SCLPMCURSTDIS_ccrg_soft_rst_req_START       (6)
#define SOC_SCTRL_SCLPMCURSTDIS_ccrg_soft_rst_req_END         (6)
#define SOC_SCTRL_SCLPMCURSTDIS_tsen_soft_rst_req_START       (7)
#define SOC_SCTRL_SCLPMCURSTDIS_tsen_soft_rst_req_END         (7)
#define SOC_SCTRL_SCLPMCURSTDIS_pmc_soft_rst_req_START        (8)
#define SOC_SCTRL_SCLPMCURSTDIS_pmc_soft_rst_req_END          (8)
#define SOC_SCTRL_SCLPMCURSTDIS_uart_soft_rst_req_START       (9)
#define SOC_SCTRL_SCLPMCURSTDIS_uart_soft_rst_req_END         (9)
#define SOC_SCTRL_SCLPMCURSTDIS_funcmbist_soft_rst_req_START  (10)
#define SOC_SCTRL_SCLPMCURSTDIS_funcmbist_soft_rst_req_END    (10)
#define SOC_SCTRL_SCLPMCURSTDIS_pcrg_soft_rst_req_START       (12)
#define SOC_SCTRL_SCLPMCURSTDIS_pcrg_soft_rst_req_END         (12)
#define SOC_SCTRL_SCLPMCURSTDIS_tim_soft_rst_req_START        (15)
#define SOC_SCTRL_SCLPMCURSTDIS_tim_soft_rst_req_END          (15)
#define SOC_SCTRL_SCLPMCURSTDIS_cssys_soft_rst_req_START      (16)
#define SOC_SCTRL_SCLPMCURSTDIS_cssys_soft_rst_req_END        (16)
#define SOC_SCTRL_SCLPMCURSTDIS_spi_soft_rst_req_START        (17)
#define SOC_SCTRL_SCLPMCURSTDIS_spi_soft_rst_req_END          (17)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCURSTSTAT_UNION
 struct description   : SCLPMCURSTSTAT Register structure definition
                        Address Offset:0x978 Initial:0x0 Width:32
 register description : LPMCU子系统软复位使能状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  rst_software_req       : 1;  /* bit[0] : 0:无变化 1:SCLPMCURSTSTAT对应bit置1。 */
        unsigned int  coresight_soft_rst_req : 1;  /* bit[1] : bit含义参考bit0。 */
        unsigned int  reserved_0             : 1;  /* bit[2] : bit含义参考bit0。 */
        unsigned int  tcm_soft_rst_req       : 1;  /* bit[3] : Baltimore不使用 */
        unsigned int  ram_soft_rst_req       : 1;  /* bit[4] : bit含义参考bit0。 */
        unsigned int  aon_soft_rst_req       : 1;  /* bit[5] : 不使用 */
        unsigned int  ccrg_soft_rst_req      : 1;  /* bit[6] :  */
        unsigned int  tsen_soft_rst_req      : 1;  /* bit[7] : bit含义参考bit0。 */
        unsigned int  pmc_soft_rst_req       : 1;  /* bit[8] : bit含义参考bit0。 */
        unsigned int  uart_soft_rst_req      : 1;  /* bit[9] : bit含义参考bit0。 */
        unsigned int  funcmbist_soft_rst_req : 1;  /* bit[10]: bit含义参考bit0。 */
        unsigned int  reserved_1             : 1;  /* bit[11]: bit含义参考bit0。 */
        unsigned int  pcrg_soft_rst_req      : 1;  /* bit[12]: bit含义参考bit0。 */
        unsigned int  reserved_2             : 1;  /* bit[13]: bit含义参考bit0。 */
        unsigned int  reserved_3             : 1;  /* bit[14]: bit含义参考bit0。 */
        unsigned int  tim_soft_rst_req       : 1;  /* bit[15]: bit含义参考bit0。 */
        unsigned int  cssys_soft_rst_req     : 1;  /* bit[16]: bit含义参考bit0。 */
        unsigned int  spi_soft_rst_req       : 1;  /* bit[17]: bit含义参考bit0。 */
        unsigned int  reserved_4             : 1;  /* bit[18]: bit含义参考bit0。 */
        unsigned int  reserved_5             : 1;  /* bit[19]:  */
        unsigned int  reserved_6             : 1;  /* bit[20]:  */
        unsigned int  reserved_7             : 1;  /* bit[21]:  */
        unsigned int  reserved_8             : 1;  /* bit[22]:  */
        unsigned int  reserved_9             : 1;  /* bit[23]:  */
        unsigned int  reserved_10            : 1;  /* bit[24]:  */
        unsigned int  reserved_11            : 1;  /* bit[25]:  */
        unsigned int  reserved_12            : 1;  /* bit[26]:  */
        unsigned int  reserved_13            : 1;  /* bit[27]:  */
        unsigned int  reserved_14            : 1;  /* bit[28]:  */
        unsigned int  reserved_15            : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCURSTSTAT_UNION;
#endif
#define SOC_SCTRL_SCLPMCURSTSTAT_rst_software_req_START        (0)
#define SOC_SCTRL_SCLPMCURSTSTAT_rst_software_req_END          (0)
#define SOC_SCTRL_SCLPMCURSTSTAT_coresight_soft_rst_req_START  (1)
#define SOC_SCTRL_SCLPMCURSTSTAT_coresight_soft_rst_req_END    (1)
#define SOC_SCTRL_SCLPMCURSTSTAT_tcm_soft_rst_req_START        (3)
#define SOC_SCTRL_SCLPMCURSTSTAT_tcm_soft_rst_req_END          (3)
#define SOC_SCTRL_SCLPMCURSTSTAT_ram_soft_rst_req_START        (4)
#define SOC_SCTRL_SCLPMCURSTSTAT_ram_soft_rst_req_END          (4)
#define SOC_SCTRL_SCLPMCURSTSTAT_aon_soft_rst_req_START        (5)
#define SOC_SCTRL_SCLPMCURSTSTAT_aon_soft_rst_req_END          (5)
#define SOC_SCTRL_SCLPMCURSTSTAT_ccrg_soft_rst_req_START       (6)
#define SOC_SCTRL_SCLPMCURSTSTAT_ccrg_soft_rst_req_END         (6)
#define SOC_SCTRL_SCLPMCURSTSTAT_tsen_soft_rst_req_START       (7)
#define SOC_SCTRL_SCLPMCURSTSTAT_tsen_soft_rst_req_END         (7)
#define SOC_SCTRL_SCLPMCURSTSTAT_pmc_soft_rst_req_START        (8)
#define SOC_SCTRL_SCLPMCURSTSTAT_pmc_soft_rst_req_END          (8)
#define SOC_SCTRL_SCLPMCURSTSTAT_uart_soft_rst_req_START       (9)
#define SOC_SCTRL_SCLPMCURSTSTAT_uart_soft_rst_req_END         (9)
#define SOC_SCTRL_SCLPMCURSTSTAT_funcmbist_soft_rst_req_START  (10)
#define SOC_SCTRL_SCLPMCURSTSTAT_funcmbist_soft_rst_req_END    (10)
#define SOC_SCTRL_SCLPMCURSTSTAT_pcrg_soft_rst_req_START       (12)
#define SOC_SCTRL_SCLPMCURSTSTAT_pcrg_soft_rst_req_END         (12)
#define SOC_SCTRL_SCLPMCURSTSTAT_tim_soft_rst_req_START        (15)
#define SOC_SCTRL_SCLPMCURSTSTAT_tim_soft_rst_req_END          (15)
#define SOC_SCTRL_SCLPMCURSTSTAT_cssys_soft_rst_req_START      (16)
#define SOC_SCTRL_SCLPMCURSTSTAT_cssys_soft_rst_req_END        (16)
#define SOC_SCTRL_SCLPMCURSTSTAT_spi_soft_rst_req_START        (17)
#define SOC_SCTRL_SCLPMCURSTSTAT_spi_soft_rst_req_END          (17)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCUCTRL_UNION
 struct description   : SCLPMCUCTRL Register structure definition
                        Address Offset:0x980 Initial:0x00 Width:32
 register description : LPMCU子系统控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 1;  /* bit[0]    : 保留。 */
        unsigned int  mpudisable      : 1;  /* bit[1]    : M3内存保护单元除能信号
                                                           0:enable 1:disable。 (Charlotte 不使用) */
        unsigned int  nmi_in          : 1;  /* bit[2]    : 外部不可屏蔽中断输入
                                                           0：no int 1:int occur。 */
        unsigned int  bus_div_clk_sel : 2;  /* bit[3-4]  : bus_div_clk分频比选择信号
                                                           00：bus_clk的1分频
                                                           01：bus_clk的2分频
                                                           10：bus_clk的3分频
                                                           11：bus_clk的4分频。 */
        unsigned int  sysresetreq_en  : 1;  /* bit[5]    : (Charlotte 不使用) */
        unsigned int  reserved_1      : 5;  /* bit[6-10] : 保留。 */
        unsigned int  reserved_2      : 1;  /* bit[11]   : 系统状态机LPMCU IDLE判断BYPASS控制：
                                                           1'b1：bypass有效
                                                           1'b0：bypass无效。 */
        unsigned int  reserved_3      : 12; /* bit[12-23]: 保留。 */
        unsigned int  reserved_4      : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCUCTRL_UNION;
#endif
#define SOC_SCTRL_SCLPMCUCTRL_mpudisable_START       (1)
#define SOC_SCTRL_SCLPMCUCTRL_mpudisable_END         (1)
#define SOC_SCTRL_SCLPMCUCTRL_nmi_in_START           (2)
#define SOC_SCTRL_SCLPMCUCTRL_nmi_in_END             (2)
#define SOC_SCTRL_SCLPMCUCTRL_bus_div_clk_sel_START  (3)
#define SOC_SCTRL_SCLPMCUCTRL_bus_div_clk_sel_END    (4)
#define SOC_SCTRL_SCLPMCUCTRL_sysresetreq_en_START   (5)
#define SOC_SCTRL_SCLPMCUCTRL_sysresetreq_en_END     (5)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCUSTAT_UNION
 struct description   : SCLPMCUSTAT Register structure definition
                        Address Offset:0x984 Initial:0x00000 Width:32
 register description : LPMCU子系统状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sysresetreq_sign_out_stat : 1;  /* bit[0]    : M3 SYSRESETREQ复位事件标志输出
                                                                     读清 0:no rst 1:rst。 (Charlotte 不使用) */
        unsigned int  sleeping                  : 1;  /* bit[1]    : 休眠状态指示信M3 休眠状态指示信号
                                                                     0:no sleeping 1:sleeping。 */
        unsigned int  lockup                    : 1;  /* bit[2]    : M3 core锁死指示信号lockup
                                                                     0: no lockup 1:lockup。 */
        unsigned int  halted                    : 1;  /* bit[3]    : M3 core停机指示信号（debug状态）
                                                                     0：no halt 1:halted。 */
        unsigned int  m3_lp_bus_slv_idle_flag   : 1;  /* bit[4]    : lpmcu core进入hard fault状态指示信号，1进入 */
        unsigned int  in_nmi_hdlr               : 1;  /* bit[5]    : lpmcu core进入nmi handler指示信号，1进入 */
        unsigned int  hard_fault_mode           : 1;  /* bit[6]    : lpmcu core进入hardfault状态指示信号，1进入 */
        unsigned int  lpmcu_lp_h2p_idle         : 1;  /* bit[7]    : lpmcu lp h2p桥idle状态指示信号；
                                                                     1:idle */
        unsigned int  lpmcu_bus_idle            : 1;  /* bit[8]    : lpmcu ahb matrix idle状态指示信号；
                                                                     1:idle */
        unsigned int  reserved_0                : 2;  /* bit[9-10] :  */
        unsigned int  reserved_1                : 1;  /* bit[11]   :  */
        unsigned int  reserved_2                : 1;  /* bit[12]   :  */
        unsigned int  reserved_3                : 19; /* bit[13-31]: SYS_MISC内部做逻辑，注意匹配修改 */
    } reg;
} SOC_SCTRL_SCLPMCUSTAT_UNION;
#endif
#define SOC_SCTRL_SCLPMCUSTAT_sysresetreq_sign_out_stat_START  (0)
#define SOC_SCTRL_SCLPMCUSTAT_sysresetreq_sign_out_stat_END    (0)
#define SOC_SCTRL_SCLPMCUSTAT_sleeping_START                   (1)
#define SOC_SCTRL_SCLPMCUSTAT_sleeping_END                     (1)
#define SOC_SCTRL_SCLPMCUSTAT_lockup_START                     (2)
#define SOC_SCTRL_SCLPMCUSTAT_lockup_END                       (2)
#define SOC_SCTRL_SCLPMCUSTAT_halted_START                     (3)
#define SOC_SCTRL_SCLPMCUSTAT_halted_END                       (3)
#define SOC_SCTRL_SCLPMCUSTAT_m3_lp_bus_slv_idle_flag_START    (4)
#define SOC_SCTRL_SCLPMCUSTAT_m3_lp_bus_slv_idle_flag_END      (4)
#define SOC_SCTRL_SCLPMCUSTAT_in_nmi_hdlr_START                (5)
#define SOC_SCTRL_SCLPMCUSTAT_in_nmi_hdlr_END                  (5)
#define SOC_SCTRL_SCLPMCUSTAT_hard_fault_mode_START            (6)
#define SOC_SCTRL_SCLPMCUSTAT_hard_fault_mode_END              (6)
#define SOC_SCTRL_SCLPMCUSTAT_lpmcu_lp_h2p_idle_START          (7)
#define SOC_SCTRL_SCLPMCUSTAT_lpmcu_lp_h2p_idle_END            (7)
#define SOC_SCTRL_SCLPMCUSTAT_lpmcu_bus_idle_START             (8)
#define SOC_SCTRL_SCLPMCUSTAT_lpmcu_bus_idle_END               (8)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCURAMCTRL_UNION
 struct description   : SCLPMCURAMCTRL Register structure definition
                        Address Offset:0x988 Initial:0x00 Width:32
 register description : LPMCU RAM 控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpramctrl_sel       : 1;  /* bit[0]    : LPMCU ram低功耗模式控制选择：
                                                               1：软件控制
                                                               0：状态机控制。 */
        unsigned int  lpram0_mod          : 2;  /* bit[1-2]  : LPMCU ram0低功耗模式配置
                                                               (Boston CS只支持normal和dslp状态)
                                                               00;normal
                                                               01:sleep
                                                               10:deepsleep
                                                               11:power-down。 */
        unsigned int  lpram1_mod          : 2;  /* bit[3-4]  : LPMCU ram1低功耗模式配置：
                                                               (Boston CS不使用)
                                                               00;normal
                                                               01:sleep
                                                               10:deepsleep
                                                               11:power-down。 */
        unsigned int  lpram2_mod          : 2;  /* bit[5-6]  : LPMCU ram2低功耗模式配置：
                                                               (Boston CS不使用)
                                                               00;normal
                                                               01:sleep
                                                               10:deepsleep
                                                               11:power-down。 */
        unsigned int  lpram3_mod          : 2;  /* bit[7-8]  : LPMCU ram3低功耗模式配置：
                                                               (Boston CS不使用)
                                                               00;normal
                                                               01:sleep
                                                               10:deepsleep
                                                               11:power-down。 */
        unsigned int  lpram0ctrl_soft     : 3;  /* bit[9-11] : LPMCU ram0低功耗控制信号软件配置：
                                                               bit9:slp
                                                               bit10:dslp
                                                               bit11:sd
                                                               000:normal
                                                               001:sleep
                                                               010:deepsleep
                                                               100:powerdown。 */
        unsigned int  lpram1ctrl_soft     : 3;  /* bit[12-14]: LPMCU ram1低功耗控制信号软件配置：
                                                               (Boston CS不使用)
                                                               bit12:slp
                                                               bit13:dslp
                                                               bit14:sd
                                                               000:normal
                                                               001:sleep
                                                               010:deepsleep
                                                               100:powerdown。 */
        unsigned int  lpram2ctrl_soft     : 3;  /* bit[15-17]: LPMCU ram2低功耗控制信号软件配置：
                                                               (Boston CS不使用)
                                                               bit15:slp
                                                               bit16:dslp
                                                               bit17:sd
                                                               000:normal
                                                               001:sleep
                                                               010:deepsleep
                                                               100:powerdown。 */
        unsigned int  lpram3ctrl_soft     : 3;  /* bit[18-20]: LPMCU ram3低功耗控制信号软件配置：
                                                               (Boston CS不使用)
                                                               bit18:slp
                                                               bit19:dslp
                                                               bit20:sd
                                                               000:normal
                                                               001:sleep
                                                               010:deepsleep
                                                               100:powerdown。 */
        unsigned int  memrep_ram_mod      : 2;  /* bit[21-22]: memrep ram低功耗模式配置（状态机控制）
                                                               00:normal
                                                               01:sleep
                                                               10:deepsleep
                                                               11:power-down。 */
        unsigned int  memrep_ramctrl_soft : 3;  /* bit[23-25]: memrep ram低功耗控制信号软件配置：
                                                               bit23:slp
                                                               bit24:dslp
                                                               bit25:sd
                                                               000:normal
                                                               001:sleep
                                                               010:deepsleep
                                                               100:powerdown。 */
        unsigned int  reserved            : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCLPMCURAMCTRL_UNION;
#endif
#define SOC_SCTRL_SCLPMCURAMCTRL_lpramctrl_sel_START        (0)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpramctrl_sel_END          (0)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram0_mod_START           (1)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram0_mod_END             (2)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram1_mod_START           (3)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram1_mod_END             (4)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram2_mod_START           (5)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram2_mod_END             (6)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram3_mod_START           (7)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram3_mod_END             (8)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram0ctrl_soft_START      (9)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram0ctrl_soft_END        (11)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram1ctrl_soft_START      (12)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram1ctrl_soft_END        (14)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram2ctrl_soft_START      (15)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram2ctrl_soft_END        (17)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram3ctrl_soft_START      (18)
#define SOC_SCTRL_SCLPMCURAMCTRL_lpram3ctrl_soft_END        (20)
#define SOC_SCTRL_SCLPMCURAMCTRL_memrep_ram_mod_START       (21)
#define SOC_SCTRL_SCLPMCURAMCTRL_memrep_ram_mod_END         (22)
#define SOC_SCTRL_SCLPMCURAMCTRL_memrep_ramctrl_soft_START  (23)
#define SOC_SCTRL_SCLPMCURAMCTRL_memrep_ramctrl_soft_END    (25)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBBPDRXSTAT0_UNION
 struct description   : SCBBPDRXSTAT0 Register structure definition
                        Address Offset:0x530 Initial:0x0000000 Width:32
 register description : BBPDRX子系统状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  g2_gdrx_depsleep_flag    : 1;  /* bit[0]   : 状态指示信号，高有效。 */
        unsigned int  g1_gdrx_depsleep_flag    : 1;  /* bit[1]   : 状态指示信号，高有效。 */
        unsigned int  wdrx_deepsleep_flag      : 1;  /* bit[2]   : 状态指示信号，高有效。 */
        unsigned int  cdrx_deepsleep_flag_1x   : 1;  /* bit[3]   : 状态指示信号，高有效。 */
        unsigned int  cdrx_deepsleep_flag_evdo : 1;  /* bit[4]   : 状态指示信号，高有效。 */
        unsigned int  reserved                 : 27; /* bit[5-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCBBPDRXSTAT0_UNION;
#endif
#define SOC_SCTRL_SCBBPDRXSTAT0_g2_gdrx_depsleep_flag_START     (0)
#define SOC_SCTRL_SCBBPDRXSTAT0_g2_gdrx_depsleep_flag_END       (0)
#define SOC_SCTRL_SCBBPDRXSTAT0_g1_gdrx_depsleep_flag_START     (1)
#define SOC_SCTRL_SCBBPDRXSTAT0_g1_gdrx_depsleep_flag_END       (1)
#define SOC_SCTRL_SCBBPDRXSTAT0_wdrx_deepsleep_flag_START       (2)
#define SOC_SCTRL_SCBBPDRXSTAT0_wdrx_deepsleep_flag_END         (2)
#define SOC_SCTRL_SCBBPDRXSTAT0_cdrx_deepsleep_flag_1x_START    (3)
#define SOC_SCTRL_SCBBPDRXSTAT0_cdrx_deepsleep_flag_1x_END      (3)
#define SOC_SCTRL_SCBBPDRXSTAT0_cdrx_deepsleep_flag_evdo_START  (4)
#define SOC_SCTRL_SCBBPDRXSTAT0_cdrx_deepsleep_flag_evdo_END    (4)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBBPDRXSTAT1_UNION
 struct description   : SCBBPDRXSTAT1 Register structure definition
                        Address Offset:0x534 Initial:0x00000000 Width:32
 register description : BBPDRX子系统状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ldrx2dbg_abs_timer1 : 32; /* bit[0-31]: ldrx2dbg_abs_timer的低32bit。(32KHz时钟域的计数)。 */
    } reg;
} SOC_SCTRL_SCBBPDRXSTAT1_UNION;
#endif
#define SOC_SCTRL_SCBBPDRXSTAT1_ldrx2dbg_abs_timer1_START  (0)
#define SOC_SCTRL_SCBBPDRXSTAT1_ldrx2dbg_abs_timer1_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBBPDRXSTAT2_UNION
 struct description   : SCBBPDRXSTAT2 Register structure definition
                        Address Offset:0x538 Initial:0x00000000 Width:32
 register description : BBPDRX子系统状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ldrx2dbg_abs_timer2 : 32; /* bit[0-31]: ldrx2dbg_abs_timer的高32bit。(32KHz时钟域的计数)。 */
    } reg;
} SOC_SCTRL_SCBBPDRXSTAT2_UNION;
#endif
#define SOC_SCTRL_SCBBPDRXSTAT2_ldrx2dbg_abs_timer2_START  (0)
#define SOC_SCTRL_SCBBPDRXSTAT2_ldrx2dbg_abs_timer2_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBBPDRXSTAT3_UNION
 struct description   : SCBBPDRXSTAT3 Register structure definition
                        Address Offset:0x53C Initial:0x00000000 Width:32
 register description : BBPDRX子系统状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ldrx2dbg_abs_timer_sleep1 : 32; /* bit[0-31]: 最近一次进入sleep时ldrx2dbg_abs_timer的低32bit。(32KHz时钟域的计数)。 */
    } reg;
} SOC_SCTRL_SCBBPDRXSTAT3_UNION;
#endif
#define SOC_SCTRL_SCBBPDRXSTAT3_ldrx2dbg_abs_timer_sleep1_START  (0)
#define SOC_SCTRL_SCBBPDRXSTAT3_ldrx2dbg_abs_timer_sleep1_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCBBPDRXSTAT4_UNION
 struct description   : SCBBPDRXSTAT4 Register structure definition
                        Address Offset:0x540 Initial:0x00000000 Width:32
 register description : BBPDRX子系统状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ldrx2dbg_abs_timer_sleep2 : 32; /* bit[0-31]: 最近一次进入sleep时ldrx2dbg_abs_timer的高32bit。(32KHz时钟域的计数)。 */
    } reg;
} SOC_SCTRL_SCBBPDRXSTAT4_UNION;
#endif
#define SOC_SCTRL_SCBBPDRXSTAT4_ldrx2dbg_abs_timer_sleep2_START  (0)
#define SOC_SCTRL_SCBBPDRXSTAT4_ldrx2dbg_abs_timer_sleep2_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCA53_EVENT_MASK_UNION
 struct description   : SCA53_EVENT_MASK Register structure definition
                        Address Offset:0x550 Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCA53_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCA53_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCA53_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCA53_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCA53_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCA53_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCA53_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCA53_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCA53_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCA53_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCA53_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCA53_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCA53_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCA53_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCA53_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCA53_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCA53_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCA53_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCA53_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCA53_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCA53_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCA53_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCA53_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCA53_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCA53_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCA53_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCA53_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCA57_EVENT_MASK_UNION
 struct description   : SCA57_EVENT_MASK Register structure definition
                        Address Offset:0x554 Initial:0x0 Width:32
 register description : A57 EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 不使用 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : 不使用 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 不使用 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 不使用 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 不使用 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 不使用 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] : 不使用 */
        unsigned int  sils_event_mask         : 1;  /* bit[7] : 不使用 */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : 不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] : 不使用 */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]: 不使用 */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]:  */
    } reg;
} SOC_SCTRL_SCA57_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCA57_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCA57_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCA57_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCA57_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCA57_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCA57_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCA57_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCA57_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCA57_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCA57_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCA57_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCA57_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCA57_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCA57_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCA57_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCA57_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCA57_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCA57_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCA57_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCA57_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCA57_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCA57_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCA57_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCA57_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCA57_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCA57_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCIOMCU_EVENT_MASK_UNION
 struct description   : SCIOMCU_EVENT_MASK Register structure definition
                        Address Offset:0x558 Initial:0x0 Width:32
 register description : IOMCU EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]:  */
    } reg;
} SOC_SCTRL_SCIOMCU_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCIOMCU_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCLPMCU_EVENT_MASK_UNION
 struct description   : SCLPMCU_EVENT_MASK Register structure definition
                        Address Offset:0x55C Initial:0x0 Width:32
 register description : LPMCU EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]:  */
    } reg;
} SOC_SCTRL_SCLPMCU_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCLPMCU_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCMCPU_EVENT_MASK_UNION
 struct description   : SCMCPU_EVENT_MASK Register structure definition
                        Address Offset:0x560 Initial:0x0 Width:32
 register description : MCPU EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCMCPU_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCMCPU_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCMCPU_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCISPA7_EVENT_MASK_UNION
 struct description   : SCISPA7_EVENT_MASK Register structure definition
                        Address Offset:0x568 Initial:0x0 Width:32
 register description : MCPU EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCISPA7_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCISPA7_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCISPA7_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCHIFD_UL_EVENT_MASK_UNION
 struct description   : SCHIFD_UL_EVENT_MASK Register structure definition
                        Address Offset:0x56C Initial:0x0 Width:32
 register description : MCPU EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCHIFD_UL_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCHIFD_UL_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_UNION
 struct description   : SCNPU_AICPU_EVENT_MASK Register structure definition
                        Address Offset:0x570 Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCNPU_AICPU_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_UNION
 struct description   : SCNPU_TSCPU_EVENT_MASK Register structure definition
                        Address Offset:0x574 Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCNPU_TSCPU_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_UNION
 struct description   : SCNPU_ARCHS_EVENT_MASK Register structure definition
                        Address Offset:0x578 Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCNPU_ARCHS_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSILS_EVENT_MASK_UNION
 struct description   : SCSILS_EVENT_MASK Register structure definition
                        Address Offset:0x97C Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCSILS_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCSILS_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCSILS_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCSILS_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCSILS_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCSILS_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCSILS_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCSILS_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCSILS_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCSILS_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCSILS_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCSILS_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCSILS_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCSILS_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCSILS_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCSILS_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCSILS_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCSILS_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCSILS_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCSILS_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCSILS_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCSILS_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCSILS_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCSILS_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCSILS_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCSILS_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCSILS_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_UNION
 struct description   : SCMDM5G_L2HAC_EVENT_MASK Register structure definition
                        Address Offset:0x544 Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCMDM5G_L2HAC_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_UNION
 struct description   : SCMDM5G_CCPU5G_EVENT_MASK Register structure definition
                        Address Offset:0x548 Initial:0x0 Width:32
 register description : EVENT屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event_mask          : 1;  /* bit[0] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  a57_event_mask          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event_mask        : 1;  /* bit[2] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  lpmcu_event_mask        : 1;  /* bit[3] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  mcpu_event_mask         : 1;  /* bit[4] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  ispa7_event_mask        : 1;  /* bit[5] : 1：屏蔽无效；
                                                                0：屏蔽有效。 */
        unsigned int  hifd_ul_event_mask      : 1;  /* bit[6] :  */
        unsigned int  sils_event_mask         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event_mask    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event_mask    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event_mask    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event_mask  : 1;  /* bit[11]:  */
        unsigned int  mdm5g_ccpu5g_event_mask : 1;  /* bit[12]:  */
        unsigned int  reserved_0              : 1;  /* bit[13]:  */
        unsigned int  reserved_1              : 1;  /* bit[14]:  */
        unsigned int  reserved_2              : 1;  /* bit[15]:  */
        unsigned int  reserved_3              : 1;  /* bit[16]:  */
        unsigned int  reserved_4              : 1;  /* bit[17]:  */
        unsigned int  reserved_5              : 1;  /* bit[18]:  */
        unsigned int  reserved_6              : 1;  /* bit[19]:  */
        unsigned int  reserved_7              : 1;  /* bit[20]:  */
        unsigned int  reserved_8              : 1;  /* bit[21]:  */
        unsigned int  reserved_9              : 1;  /* bit[22]:  */
        unsigned int  reserved_10             : 1;  /* bit[23]:  */
        unsigned int  reserved_11             : 1;  /* bit[24]:  */
        unsigned int  reserved_12             : 1;  /* bit[25]:  */
        unsigned int  reserved_13             : 1;  /* bit[26]:  */
        unsigned int  reserved_14             : 1;  /* bit[27]:  */
        unsigned int  reserved_15             : 1;  /* bit[28]:  */
        unsigned int  reserved_16             : 1;  /* bit[29]:  */
        unsigned int  reserved_17             : 1;  /* bit[30]:  */
        unsigned int  reserved_18             : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_UNION;
#endif
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_a53_event_mask_START           (0)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_a53_event_mask_END             (0)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_a57_event_mask_START           (1)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_a57_event_mask_END             (1)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_iomcu_event_mask_START         (2)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_iomcu_event_mask_END           (2)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_lpmcu_event_mask_START         (3)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_lpmcu_event_mask_END           (3)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_mcpu_event_mask_START          (4)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_mcpu_event_mask_END            (4)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_ispa7_event_mask_START         (5)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_ispa7_event_mask_END           (5)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_hifd_ul_event_mask_START       (6)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_hifd_ul_event_mask_END         (6)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_sils_event_mask_START          (7)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_sils_event_mask_END            (7)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_npu_archs_event_mask_START     (8)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_npu_archs_event_mask_END       (8)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_npu_tscpu_event_mask_START     (9)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_npu_tscpu_event_mask_END       (9)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_npu_aicpu_event_mask_START     (10)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_npu_aicpu_event_mask_END       (10)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_mdm5g_l2hac_event_mask_START   (11)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_mdm5g_l2hac_event_mask_END     (11)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_mdm5g_ccpu5g_event_mask_START  (12)
#define SOC_SCTRL_SCMDM5G_CCPU5G_EVENT_MASK_mdm5g_ccpu5g_event_mask_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCEVENT_STAT_UNION
 struct description   : SCEVENT_STAT Register structure definition
                        Address Offset:0x564 Initial:0x0 Width:32
 register description : EVENT状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_event          : 1;  /* bit[0] : 1：event有效；
                                                           0：event无效。 */
        unsigned int  a57_event          : 1;  /* bit[1] : CPU和FCM共用a53_event,此信号不使用。 */
        unsigned int  iomcu_event        : 1;  /* bit[2] : 1：event有效；
                                                           0：event无效。 */
        unsigned int  lpmcu_event        : 1;  /* bit[3] : 1：event有效；
                                                           0：event无效。 */
        unsigned int  mcpu_event         : 1;  /* bit[4] : MDM不使用 */
        unsigned int  ispa7_event        : 1;  /* bit[5] : 1：event有效；
                                                           0：event无效。 */
        unsigned int  hifd_ul_event      : 1;  /* bit[6] :  */
        unsigned int  sils_event         : 1;  /* bit[7] :  */
        unsigned int  npu_archs_event    : 1;  /* bit[8] : PHX ES不使用 */
        unsigned int  npu_tscpu_event    : 1;  /* bit[9] :  */
        unsigned int  npu_aicpu_event    : 1;  /* bit[10]:  */
        unsigned int  mdm5g_l2hac_event  : 1;  /* bit[11]: MDM不使用 */
        unsigned int  mdm5g_ccpu5g_event : 1;  /* bit[12]: MDM不使用 */
        unsigned int  reserved_0         : 1;  /* bit[13]:  */
        unsigned int  reserved_1         : 1;  /* bit[14]:  */
        unsigned int  reserved_2         : 1;  /* bit[15]:  */
        unsigned int  reserved_3         : 1;  /* bit[16]:  */
        unsigned int  reserved_4         : 1;  /* bit[17]:  */
        unsigned int  reserved_5         : 1;  /* bit[18]:  */
        unsigned int  reserved_6         : 1;  /* bit[19]:  */
        unsigned int  reserved_7         : 1;  /* bit[20]:  */
        unsigned int  reserved_8         : 1;  /* bit[21]:  */
        unsigned int  reserved_9         : 1;  /* bit[22]:  */
        unsigned int  reserved_10        : 1;  /* bit[23]:  */
        unsigned int  reserved_11        : 1;  /* bit[24]:  */
        unsigned int  reserved_12        : 1;  /* bit[25]:  */
        unsigned int  reserved_13        : 1;  /* bit[26]:  */
        unsigned int  reserved_14        : 1;  /* bit[27]:  */
        unsigned int  reserved_15        : 1;  /* bit[28]:  */
        unsigned int  reserved_16        : 1;  /* bit[29]:  */
        unsigned int  reserved_17        : 1;  /* bit[30]:  */
        unsigned int  reserved_18        : 1;  /* bit[31]: SYS_MISC有逻辑处理，注意修改 */
    } reg;
} SOC_SCTRL_SCEVENT_STAT_UNION;
#endif
#define SOC_SCTRL_SCEVENT_STAT_a53_event_START           (0)
#define SOC_SCTRL_SCEVENT_STAT_a53_event_END             (0)
#define SOC_SCTRL_SCEVENT_STAT_a57_event_START           (1)
#define SOC_SCTRL_SCEVENT_STAT_a57_event_END             (1)
#define SOC_SCTRL_SCEVENT_STAT_iomcu_event_START         (2)
#define SOC_SCTRL_SCEVENT_STAT_iomcu_event_END           (2)
#define SOC_SCTRL_SCEVENT_STAT_lpmcu_event_START         (3)
#define SOC_SCTRL_SCEVENT_STAT_lpmcu_event_END           (3)
#define SOC_SCTRL_SCEVENT_STAT_mcpu_event_START          (4)
#define SOC_SCTRL_SCEVENT_STAT_mcpu_event_END            (4)
#define SOC_SCTRL_SCEVENT_STAT_ispa7_event_START         (5)
#define SOC_SCTRL_SCEVENT_STAT_ispa7_event_END           (5)
#define SOC_SCTRL_SCEVENT_STAT_hifd_ul_event_START       (6)
#define SOC_SCTRL_SCEVENT_STAT_hifd_ul_event_END         (6)
#define SOC_SCTRL_SCEVENT_STAT_sils_event_START          (7)
#define SOC_SCTRL_SCEVENT_STAT_sils_event_END            (7)
#define SOC_SCTRL_SCEVENT_STAT_npu_archs_event_START     (8)
#define SOC_SCTRL_SCEVENT_STAT_npu_archs_event_END       (8)
#define SOC_SCTRL_SCEVENT_STAT_npu_tscpu_event_START     (9)
#define SOC_SCTRL_SCEVENT_STAT_npu_tscpu_event_END       (9)
#define SOC_SCTRL_SCEVENT_STAT_npu_aicpu_event_START     (10)
#define SOC_SCTRL_SCEVENT_STAT_npu_aicpu_event_END       (10)
#define SOC_SCTRL_SCEVENT_STAT_mdm5g_l2hac_event_START   (11)
#define SOC_SCTRL_SCEVENT_STAT_mdm5g_l2hac_event_END     (11)
#define SOC_SCTRL_SCEVENT_STAT_mdm5g_ccpu5g_event_START  (12)
#define SOC_SCTRL_SCEVENT_STAT_mdm5g_ccpu5g_event_END    (12)


/*****************************************************************************
 struct               : SOC_SCTRL_SCIOMCUCLKCTRL_UNION
 struct description   : SCIOMCUCLKCTRL Register structure definition
                        Address Offset:0x880 Initial:0x0000 Width:32
 register description : IOMCU时钟控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_iomcu_pll_div    : 1;  /* bit[0]    : IOMCU PLL分频器使能：
                                                               0：使能关闭
                                                               1：使能打开
                                                               最终的使能信号由该配置信号与IOMCU_pll_div_ctrl做或。 */
        unsigned int  gt_iomcu_sys_div    : 1;  /* bit[1]    : IOMCU SYS分频器使能：
                                                               0：使能关闭
                                                               1：使能打开
                                                               最终的使能信号由该配置信号与IOMCU_sys_div_ctrl做或。 */
        unsigned int  gt_clk_qcr_iomcubus : 1;  /* bit[2]    :  */
        unsigned int  reserved            : 13; /* bit[3-15] : 保留。 */
        unsigned int  sciomcuclkctrl_msk  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCIOMCUCLKCTRL_UNION;
#endif
#define SOC_SCTRL_SCIOMCUCLKCTRL_gt_iomcu_pll_div_START     (0)
#define SOC_SCTRL_SCIOMCUCLKCTRL_gt_iomcu_pll_div_END       (0)
#define SOC_SCTRL_SCIOMCUCLKCTRL_gt_iomcu_sys_div_START     (1)
#define SOC_SCTRL_SCIOMCUCLKCTRL_gt_iomcu_sys_div_END       (1)
#define SOC_SCTRL_SCIOMCUCLKCTRL_gt_clk_qcr_iomcubus_START  (2)
#define SOC_SCTRL_SCIOMCUCLKCTRL_gt_clk_qcr_iomcubus_END    (2)
#define SOC_SCTRL_SCIOMCUCLKCTRL_sciomcuclkctrl_msk_START   (16)
#define SOC_SCTRL_SCIOMCUCLKCTRL_sciomcuclkctrl_msk_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCIOMCUCLKSTAT_UNION
 struct description   : SCIOMCUCLKSTAT Register structure definition
                        Address Offset:0x584 Initial:0x0000000 Width:32
 register description : IOMCU时钟控制信号状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  iomcu_pll_ctrl        : 1;  /* bit[0]   : IOMCU侧控制PLL使能状态：
                                                                0：使能关闭
                                                                1：使能打开。 */
        unsigned int  iomcu_pll_div_ctrl    : 1;  /* bit[1]   : IOMCU侧控制PLL DIV使能状态：
                                                                0：使能关闭
                                                                1：使能打开。 */
        unsigned int  iomcu_sys_div_ctrl    : 1;  /* bit[2]   : IOMCU侧控制SYS DIV使能状态：
                                                                0：使能关闭
                                                                1：使能打开。 */
        unsigned int  gt_iomcu_pll_div_stat : 1;  /* bit[3]   : IOMCU的PLL DIV使能最终状态（gt_IOMCU_pll_div与IOMCU_pll_div_ctrl或）：
                                                                0：使能关闭
                                                                1：使能打开。 */
        unsigned int  gt_iomcu_sys_div_stat : 1;  /* bit[4]   : IOMCU的SYS DIV使能最终状态（gt_IOMCU_sys_div与IOMCU_sys_div_ctrl或）：
                                                                0：使能关闭
                                                                1：使能打开。 */
        unsigned int  reserved              : 27; /* bit[5-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCIOMCUCLKSTAT_UNION;
#endif
#define SOC_SCTRL_SCIOMCUCLKSTAT_iomcu_pll_ctrl_START         (0)
#define SOC_SCTRL_SCIOMCUCLKSTAT_iomcu_pll_ctrl_END           (0)
#define SOC_SCTRL_SCIOMCUCLKSTAT_iomcu_pll_div_ctrl_START     (1)
#define SOC_SCTRL_SCIOMCUCLKSTAT_iomcu_pll_div_ctrl_END       (1)
#define SOC_SCTRL_SCIOMCUCLKSTAT_iomcu_sys_div_ctrl_START     (2)
#define SOC_SCTRL_SCIOMCUCLKSTAT_iomcu_sys_div_ctrl_END       (2)
#define SOC_SCTRL_SCIOMCUCLKSTAT_gt_iomcu_pll_div_stat_START  (3)
#define SOC_SCTRL_SCIOMCUCLKSTAT_gt_iomcu_pll_div_stat_END    (3)
#define SOC_SCTRL_SCIOMCUCLKSTAT_gt_iomcu_sys_div_stat_START  (4)
#define SOC_SCTRL_SCIOMCUCLKSTAT_gt_iomcu_sys_div_stat_END    (4)


/*****************************************************************************
 struct               : SOC_SCTRL_SCIOMCUCTRL_UNION
 struct description   : SCIOMCUCTRL Register structure definition
                        Address Offset:0x588 Initial:0x00000000 Width:32
 register description : IOMCU子系统控制配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   : 保留。 */
        unsigned int  nmi_in   : 1;  /* bit[1]   : 外部不可屏蔽中断输入
                                                   0:no nmi 1:nmi int。 */
        unsigned int  reserved_1: 1;  /* bit[2]   : 保留。 */
        unsigned int  reserved_2: 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCIOMCUCTRL_UNION;
#endif
#define SOC_SCTRL_SCIOMCUCTRL_nmi_in_START    (1)
#define SOC_SCTRL_SCIOMCUCTRL_nmi_in_END      (1)


/*****************************************************************************
 struct               : SOC_SCTRL_SCIOMCUSTAT_UNION
 struct description   : SCIOMCUSTAT Register structure definition
                        Address Offset:0x58C Initial:0x000 Width:32
 register description : IOMCU子系统状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  iomcu_sysresetreq : 1;  /* bit[0]    : iomcu_sysresetreq，读清
                                                             0:normal 1:rst have occurred。 */
        unsigned int  iomcu_m7_state    : 3;  /* bit[1-3]  : [3]sleeping
                                                             0:no sleeping 1:sleeping
                                                             [2]lockup
                                                             0:no lockup 1:lockup
                                                             [1]halted
                                                             0:no halted 1:halted。 */
        unsigned int  reserved_0        : 1;  /* bit[4]    :  */
        unsigned int  reserved_1        : 1;  /* bit[5]    :  */
        unsigned int  reserved_2        : 1;  /* bit[6]    :  */
        unsigned int  m7_sc_stat        : 9;  /* bit[7-15] : m7_sc_stat[7:0] */
        unsigned int  reserved_3        : 1;  /* bit[16]   :  */
        unsigned int  reserved_4        : 1;  /* bit[17]   :  */
        unsigned int  reserved_5        : 1;  /* bit[18]   :  */
        unsigned int  reserved_6        : 1;  /* bit[19]   :  */
        unsigned int  reserved_7        : 1;  /* bit[20]   :  */
        unsigned int  reserved_8        : 1;  /* bit[21]   :  */
        unsigned int  iomcu_stat        : 10; /* bit[22-31]: SYS_MISC有逻辑处理，注意修改
                                                             others:reserved
                                                             [31:30]AO_BUS的时钟切换 iomcu_aobus_clk_sw_req，
                                                             [29:28]MMBUF的时钟切换 iomcu_mmbuf_clk_sw_req，
                                                             [27]AO_BUS的时钟门控 iomcu_aobus_clk_en_req，1bit,1：打开 0：关闭
                                                             [26]MMBUF的时钟门控 iomcu_mmbuf_clk_en_req，1bit,1：打开 0：关闭
                                                             [25]FLL的开关控制 iomcu_fpll_en_req，1bit,1：打开 0：关闭
                                                             [24:22]reserved。 */
    } reg;
} SOC_SCTRL_SCIOMCUSTAT_UNION;
#endif
#define SOC_SCTRL_SCIOMCUSTAT_iomcu_sysresetreq_START  (0)
#define SOC_SCTRL_SCIOMCUSTAT_iomcu_sysresetreq_END    (0)
#define SOC_SCTRL_SCIOMCUSTAT_iomcu_m7_state_START     (1)
#define SOC_SCTRL_SCIOMCUSTAT_iomcu_m7_state_END       (3)
#define SOC_SCTRL_SCIOMCUSTAT_m7_sc_stat_START         (7)
#define SOC_SCTRL_SCIOMCUSTAT_m7_sc_stat_END           (15)
#define SOC_SCTRL_SCIOMCUSTAT_iomcu_stat_START         (22)
#define SOC_SCTRL_SCIOMCUSTAT_iomcu_stat_END           (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_UNION
 struct description   : SC_LOCK_CFG_UNSEC_0 Register structure definition
                        Address Offset:0x500 Initial:0x0 Width:32
 register description : 通用资源锁定寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的锁定master ID号0。 */
        unsigned int  resource0lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的锁定master ID号1。 */
        unsigned int  resource0lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id2  : 3;  /* bit[9-11] : 通用资源锁0的锁定master ID号2。 */
        unsigned int  resource0lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id3  : 3;  /* bit[13-15]: 通用资源锁0的锁定master ID号3。 */
        unsigned int  resource0lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id4  : 3;  /* bit[17-19]: 通用资源锁0的锁定master ID号4。 */
        unsigned int  resource0lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id5  : 3;  /* bit[21-23]: 通用资源锁0的锁定master ID号5。 */
        unsigned int  resource0lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id6  : 3;  /* bit[25-27]: 通用资源锁0的锁定master ID号6。 */
        unsigned int  resource0lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                              通用资源锁0的锁定master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd0_START  (0)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd0_END    (0)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id0_START   (1)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id0_END     (3)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd1_START  (4)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd1_END    (4)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id1_START   (5)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id1_END     (7)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd2_START  (8)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd2_END    (8)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id2_START   (9)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id2_END     (11)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd3_START  (12)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd3_END    (12)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id3_START   (13)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id3_END     (15)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd4_START  (16)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd4_END    (16)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id4_START   (17)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id4_END     (19)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd5_START  (20)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd5_END    (20)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id5_START   (21)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id5_END     (23)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd6_START  (24)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd6_END    (24)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id6_START   (25)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id6_END     (27)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd7_START  (28)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_cmd7_END    (28)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id7_START   (29)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_0_resource0lock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_UNION
 struct description   : SC_UNLOCK_CFG_UNSEC_0 Register structure definition
                        Address Offset:0x504 Initial:0x0 Width:32
 register description : 通用资源解锁寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的解锁master ID号0。 */
        unsigned int  resource0unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的解锁master ID号1。 */
        unsigned int  resource0unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id2  : 3;  /* bit[9-11] : 通用资源锁0的解锁master ID号2。 */
        unsigned int  resource0unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id3  : 3;  /* bit[13-15]: 通用资源锁0的解锁master ID号3。 */
        unsigned int  resource0unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id4  : 3;  /* bit[17-19]: 通用资源锁0的解锁master ID号4。 */
        unsigned int  resource0unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id5  : 3;  /* bit[21-23]: 通用资源锁0的解锁master ID号5。 */
        unsigned int  resource0unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id6  : 3;  /* bit[25-27]: 通用资源锁0的解锁master ID号6。 */
        unsigned int  resource0unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的解锁master ID号7。 */
    } reg;
} SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_UNION;
#endif
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd0_START  (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd0_END    (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id0_START   (1)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id0_END     (3)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd1_START  (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd1_END    (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id1_START   (5)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id1_END     (7)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd2_START  (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd2_END    (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id2_START   (9)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id2_END     (11)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd3_START  (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd3_END    (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id3_START   (13)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id3_END     (15)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd4_START  (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd4_END    (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id4_START   (17)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id4_END     (19)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd5_START  (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd5_END    (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id5_START   (21)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id5_END     (23)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd6_START  (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd6_END    (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id6_START   (25)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id6_END     (27)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd7_START  (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_cmd7_END    (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id7_START   (29)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_0_resource0unlock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_UNION
 struct description   : SC_LOCK_STAT_UNSEC_0 Register structure definition
                        Address Offset:0x508 Initial:0x0 Width:32
 register description : 通用资源锁状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_st0    : 1;  /* bit[0-0]  : 通用资源锁0的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁0的锁定状态master ID号0。 */
        unsigned int  resource0lock_st1    : 1;  /* bit[4-4]  : 通用资源锁0的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁0的锁定状态master ID号1。 */
        unsigned int  resource0lock_st2    : 1;  /* bit[8-8]  : 通用资源锁0的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁0的锁定状态master ID号2。 */
        unsigned int  resource0lock_st3    : 1;  /* bit[12-12]: 通用资源锁0的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁0的锁定状态master ID号3。 */
        unsigned int  resource0lock_st4    : 1;  /* bit[16-16]: 通用资源锁0的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁0的锁定状态master ID号4。 */
        unsigned int  resource0lock_st5    : 1;  /* bit[20-20]: 通用资源锁0的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁0的锁定状态master ID号5。 */
        unsigned int  resource0lock_st6    : 1;  /* bit[24-24]: 通用资源锁0的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁0的锁定状态master ID号6。 */
        unsigned int  resource0lock_st7    : 1;  /* bit[28-28]: 通用资源锁0的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id7 : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的锁定状态master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st0_START     (0)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st0_END       (0)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id0_START  (1)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id0_END    (3)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st1_START     (4)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st1_END       (4)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id1_START  (5)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id1_END    (7)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st2_START     (8)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st2_END       (8)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id2_START  (9)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id2_END    (11)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st3_START     (12)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st3_END       (12)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id3_START  (13)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id3_END    (15)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st4_START     (16)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st4_END       (16)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id4_START  (17)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id4_END    (19)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st5_START     (20)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st5_END       (20)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id5_START  (21)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id5_END    (23)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st6_START     (24)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st6_END       (24)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id6_START  (25)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id6_END    (27)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st7_START     (28)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st7_END       (28)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id7_START  (29)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_0_resource0lock_st_id7_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_UNION
 struct description   : SC_LOCK_CFG_UNSEC_1 Register structure definition
                        Address Offset:0x50C Initial:0x0 Width:32
 register description : 通用资源锁定寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的锁定master ID号0。 */
        unsigned int  resource0lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的锁定master ID号1。 */
        unsigned int  resource0lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id2  : 3;  /* bit[9-11] : 通用资源锁0的锁定master ID号2。 */
        unsigned int  resource0lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id3  : 3;  /* bit[13-15]: 通用资源锁0的锁定master ID号3。 */
        unsigned int  resource0lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id4  : 3;  /* bit[17-19]: 通用资源锁0的锁定master ID号4。 */
        unsigned int  resource0lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id5  : 3;  /* bit[21-23]: 通用资源锁0的锁定master ID号5。 */
        unsigned int  resource0lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id6  : 3;  /* bit[25-27]: 通用资源锁0的锁定master ID号6。 */
        unsigned int  resource0lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                              通用资源锁0的锁定master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd0_START  (0)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd0_END    (0)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id0_START   (1)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id0_END     (3)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd1_START  (4)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd1_END    (4)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id1_START   (5)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id1_END     (7)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd2_START  (8)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd2_END    (8)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id2_START   (9)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id2_END     (11)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd3_START  (12)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd3_END    (12)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id3_START   (13)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id3_END     (15)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd4_START  (16)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd4_END    (16)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id4_START   (17)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id4_END     (19)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd5_START  (20)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd5_END    (20)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id5_START   (21)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id5_END     (23)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd6_START  (24)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd6_END    (24)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id6_START   (25)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id6_END     (27)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd7_START  (28)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_cmd7_END    (28)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id7_START   (29)
#define SOC_SCTRL_SC_LOCK_CFG_UNSEC_1_resource0lock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_UNION
 struct description   : SC_UNLOCK_CFG_UNSEC_1 Register structure definition
                        Address Offset:0x510 Initial:0x0 Width:32
 register description : 通用资源解锁寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的解锁master ID号0。 */
        unsigned int  resource0unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的解锁master ID号1。 */
        unsigned int  resource0unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id2  : 3;  /* bit[9-11] : 通用资源锁0的解锁master ID号2。 */
        unsigned int  resource0unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id3  : 3;  /* bit[13-15]: 通用资源锁0的解锁master ID号3。 */
        unsigned int  resource0unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id4  : 3;  /* bit[17-19]: 通用资源锁0的解锁master ID号4。 */
        unsigned int  resource0unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id5  : 3;  /* bit[21-23]: 通用资源锁0的解锁master ID号5。 */
        unsigned int  resource0unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id6  : 3;  /* bit[25-27]: 通用资源锁0的解锁master ID号6。 */
        unsigned int  resource0unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的解锁master ID号7。 */
    } reg;
} SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_UNION;
#endif
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd0_START  (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd0_END    (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id0_START   (1)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id0_END     (3)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd1_START  (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd1_END    (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id1_START   (5)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id1_END     (7)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd2_START  (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd2_END    (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id2_START   (9)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id2_END     (11)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd3_START  (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd3_END    (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id3_START   (13)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id3_END     (15)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd4_START  (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd4_END    (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id4_START   (17)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id4_END     (19)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd5_START  (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd5_END    (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id5_START   (21)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id5_END     (23)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd6_START  (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd6_END    (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id6_START   (25)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id6_END     (27)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd7_START  (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_cmd7_END    (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id7_START   (29)
#define SOC_SCTRL_SC_UNLOCK_CFG_UNSEC_1_resource0unlock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_UNION
 struct description   : SC_LOCK_STAT_UNSEC_1 Register structure definition
                        Address Offset:0x514 Initial:0x0 Width:32
 register description : 通用资源锁状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_st0    : 1;  /* bit[0-0]  : 通用资源锁0的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁0的锁定状态master ID号0。 */
        unsigned int  resource0lock_st1    : 1;  /* bit[4-4]  : 通用资源锁0的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁0的锁定状态master ID号1。 */
        unsigned int  resource0lock_st2    : 1;  /* bit[8-8]  : 通用资源锁0的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁0的锁定状态master ID号2。 */
        unsigned int  resource0lock_st3    : 1;  /* bit[12-12]: 通用资源锁0的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁0的锁定状态master ID号3。 */
        unsigned int  resource0lock_st4    : 1;  /* bit[16-16]: 通用资源锁0的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁0的锁定状态master ID号4。 */
        unsigned int  resource0lock_st5    : 1;  /* bit[20-20]: 通用资源锁0的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁0的锁定状态master ID号5。 */
        unsigned int  resource0lock_st6    : 1;  /* bit[24-24]: 通用资源锁0的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁0的锁定状态master ID号6。 */
        unsigned int  resource0lock_st7    : 1;  /* bit[28-28]: 通用资源锁0的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id7 : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的锁定状态master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st0_START     (0)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st0_END       (0)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id0_START  (1)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id0_END    (3)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st1_START     (4)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st1_END       (4)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id1_START  (5)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id1_END    (7)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st2_START     (8)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st2_END       (8)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id2_START  (9)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id2_END    (11)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st3_START     (12)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st3_END       (12)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id3_START  (13)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id3_END    (15)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st4_START     (16)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st4_END       (16)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id4_START  (17)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id4_END    (19)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st5_START     (20)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st5_END       (20)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id5_START  (21)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id5_END    (23)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st6_START     (24)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st6_END       (24)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id6_START  (25)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id6_END    (27)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st7_START     (28)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st7_END       (28)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id7_START  (29)
#define SOC_SCTRL_SC_LOCK_STAT_UNSEC_1_resource0lock_st_id7_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCMDMADDR0_NONSEC_UNION
 struct description   : SCMDMADDR0_NONSEC Register structure definition
                        Address Offset:0x700 Initial:0x0 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 31; /* bit[0-30]:  */
        unsigned int  reserved_1: 1;  /* bit[31]  :  */
    } reg;
} SOC_SCTRL_SCMDMADDR0_NONSEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCMDMADDR1_NONSEC_UNION
 struct description   : SCMDMADDR1_NONSEC Register structure definition
                        Address Offset:0x704 Initial:0x0 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 31; /* bit[0-30]:  */
        unsigned int  reserved_1: 1;  /* bit[31]  :  */
    } reg;
} SOC_SCTRL_SCMDMADDR1_NONSEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCMDMADDR2_NONSEC_UNION
 struct description   : SCMDMADDR2_NONSEC Register structure definition
                        Address Offset:0x708 Initial:0x0 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 31; /* bit[0-30]:  */
        unsigned int  reserved_1: 1;  /* bit[31]  :  */
    } reg;
} SOC_SCTRL_SCMDMADDR2_NONSEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCMDMADDR3_NONSEC_UNION
 struct description   : SCMDMADDR3_NONSEC Register structure definition
                        Address Offset:0x70C Initial:0x0 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 31; /* bit[0-30]:  */
        unsigned int  reserved_1: 1;  /* bit[31]  :  */
    } reg;
} SOC_SCTRL_SCMDMADDR3_NONSEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_CFG_SEC_0_UNION
 struct description   : SC_LOCK_CFG_SEC_0 Register structure definition
                        Address Offset:0x8E8 Initial:0x0 Width:32
 register description : 通用资源锁定寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的锁定master ID号0。 */
        unsigned int  resource0lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的锁定master ID号1。 */
        unsigned int  resource0lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id2  : 3;  /* bit[9-11] : 通用资源锁0的锁定master ID号2。 */
        unsigned int  resource0lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id3  : 3;  /* bit[13-15]: 通用资源锁0的锁定master ID号3。 */
        unsigned int  resource0lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id4  : 3;  /* bit[17-19]: 通用资源锁0的锁定master ID号4。 */
        unsigned int  resource0lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id5  : 3;  /* bit[21-23]: 通用资源锁0的锁定master ID号5。 */
        unsigned int  resource0lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id6  : 3;  /* bit[25-27]: 通用资源锁0的锁定master ID号6。 */
        unsigned int  resource0lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                              通用资源锁0的锁定master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_CFG_SEC_0_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd0_START  (0)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd0_END    (0)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id0_START   (1)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id0_END     (3)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd1_START  (4)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd1_END    (4)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id1_START   (5)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id1_END     (7)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd2_START  (8)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd2_END    (8)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id2_START   (9)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id2_END     (11)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd3_START  (12)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd3_END    (12)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id3_START   (13)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id3_END     (15)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd4_START  (16)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd4_END    (16)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id4_START   (17)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id4_END     (19)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd5_START  (20)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd5_END    (20)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id5_START   (21)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id5_END     (23)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd6_START  (24)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd6_END    (24)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id6_START   (25)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id6_END     (27)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd7_START  (28)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_cmd7_END    (28)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id7_START   (29)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_0_resource0lock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_UNION
 struct description   : SC_UNLOCK_CFG_SEC_0 Register structure definition
                        Address Offset:0x8EC Initial:0x0 Width:32
 register description : 通用资源解锁寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的解锁master ID号0。 */
        unsigned int  resource0unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的解锁master ID号1。 */
        unsigned int  resource0unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id2  : 3;  /* bit[9-11] : 通用资源锁0的解锁master ID号2。 */
        unsigned int  resource0unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id3  : 3;  /* bit[13-15]: 通用资源锁0的解锁master ID号3。 */
        unsigned int  resource0unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id4  : 3;  /* bit[17-19]: 通用资源锁0的解锁master ID号4。 */
        unsigned int  resource0unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id5  : 3;  /* bit[21-23]: 通用资源锁0的解锁master ID号5。 */
        unsigned int  resource0unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id6  : 3;  /* bit[25-27]: 通用资源锁0的解锁master ID号6。 */
        unsigned int  resource0unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的解锁master ID号7。 */
    } reg;
} SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_UNION;
#endif
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd0_START  (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd0_END    (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id0_START   (1)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id0_END     (3)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd1_START  (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd1_END    (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id1_START   (5)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id1_END     (7)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd2_START  (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd2_END    (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id2_START   (9)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id2_END     (11)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd3_START  (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd3_END    (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id3_START   (13)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id3_END     (15)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd4_START  (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd4_END    (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id4_START   (17)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id4_END     (19)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd5_START  (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd5_END    (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id5_START   (21)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id5_END     (23)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd6_START  (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd6_END    (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id6_START   (25)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id6_END     (27)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd7_START  (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_cmd7_END    (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id7_START   (29)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_0_resource0unlock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_STAT_SEC_0_UNION
 struct description   : SC_LOCK_STAT_SEC_0 Register structure definition
                        Address Offset:0x8F0 Initial:0x0 Width:32
 register description : 通用资源锁状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_st0    : 1;  /* bit[0-0]  : 通用资源锁0的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁0的锁定状态master ID号0。 */
        unsigned int  resource0lock_st1    : 1;  /* bit[4-4]  : 通用资源锁0的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁0的锁定状态master ID号1。 */
        unsigned int  resource0lock_st2    : 1;  /* bit[8-8]  : 通用资源锁0的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁0的锁定状态master ID号2。 */
        unsigned int  resource0lock_st3    : 1;  /* bit[12-12]: 通用资源锁0的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁0的锁定状态master ID号3。 */
        unsigned int  resource0lock_st4    : 1;  /* bit[16-16]: 通用资源锁0的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁0的锁定状态master ID号4。 */
        unsigned int  resource0lock_st5    : 1;  /* bit[20-20]: 通用资源锁0的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁0的锁定状态master ID号5。 */
        unsigned int  resource0lock_st6    : 1;  /* bit[24-24]: 通用资源锁0的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁0的锁定状态master ID号6。 */
        unsigned int  resource0lock_st7    : 1;  /* bit[28-28]: 通用资源锁0的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id7 : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的锁定状态master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_STAT_SEC_0_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st0_START     (0)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st0_END       (0)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id0_START  (1)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id0_END    (3)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st1_START     (4)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st1_END       (4)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id1_START  (5)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id1_END    (7)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st2_START     (8)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st2_END       (8)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id2_START  (9)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id2_END    (11)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st3_START     (12)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st3_END       (12)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id3_START  (13)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id3_END    (15)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st4_START     (16)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st4_END       (16)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id4_START  (17)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id4_END    (19)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st5_START     (20)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st5_END       (20)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id5_START  (21)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id5_END    (23)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st6_START     (24)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st6_END       (24)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id6_START  (25)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id6_END    (27)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st7_START     (28)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st7_END       (28)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id7_START  (29)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_0_resource0lock_st_id7_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_CFG_SEC_1_UNION
 struct description   : SC_LOCK_CFG_SEC_1 Register structure definition
                        Address Offset:0x8F4 Initial:0x0 Width:32
 register description : 通用资源锁定寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的锁定master ID号0。 */
        unsigned int  resource0lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的锁定master ID号1。 */
        unsigned int  resource0lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id2  : 3;  /* bit[9-11] : 通用资源锁0的锁定master ID号2。 */
        unsigned int  resource0lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id3  : 3;  /* bit[13-15]: 通用资源锁0的锁定master ID号3。 */
        unsigned int  resource0lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id4  : 3;  /* bit[17-19]: 通用资源锁0的锁定master ID号4。 */
        unsigned int  resource0lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id5  : 3;  /* bit[21-23]: 通用资源锁0的锁定master ID号5。 */
        unsigned int  resource0lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id6  : 3;  /* bit[25-27]: 通用资源锁0的锁定master ID号6。 */
        unsigned int  resource0lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                              通用资源锁0的锁定master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_CFG_SEC_1_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd0_START  (0)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd0_END    (0)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id0_START   (1)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id0_END     (3)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd1_START  (4)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd1_END    (4)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id1_START   (5)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id1_END     (7)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd2_START  (8)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd2_END    (8)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id2_START   (9)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id2_END     (11)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd3_START  (12)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd3_END    (12)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id3_START   (13)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id3_END     (15)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd4_START  (16)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd4_END    (16)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id4_START   (17)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id4_END     (19)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd5_START  (20)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd5_END    (20)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id5_START   (21)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id5_END     (23)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd6_START  (24)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd6_END    (24)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id6_START   (25)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id6_END     (27)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd7_START  (28)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_cmd7_END    (28)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id7_START   (29)
#define SOC_SCTRL_SC_LOCK_CFG_SEC_1_resource0lock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_UNION
 struct description   : SC_UNLOCK_CFG_SEC_1 Register structure definition
                        Address Offset:0x8F8 Initial:0x0 Width:32
 register description : 通用资源解锁寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的解锁master ID号0。 */
        unsigned int  resource0unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的解锁master ID号1。 */
        unsigned int  resource0unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id2  : 3;  /* bit[9-11] : 通用资源锁0的解锁master ID号2。 */
        unsigned int  resource0unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id3  : 3;  /* bit[13-15]: 通用资源锁0的解锁master ID号3。 */
        unsigned int  resource0unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id4  : 3;  /* bit[17-19]: 通用资源锁0的解锁master ID号4。 */
        unsigned int  resource0unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id5  : 3;  /* bit[21-23]: 通用资源锁0的解锁master ID号5。 */
        unsigned int  resource0unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id6  : 3;  /* bit[25-27]: 通用资源锁0的解锁master ID号6。 */
        unsigned int  resource0unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id7  : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的解锁master ID号7。 */
    } reg;
} SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_UNION;
#endif
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd0_START  (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd0_END    (0)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id0_START   (1)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id0_END     (3)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd1_START  (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd1_END    (4)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id1_START   (5)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id1_END     (7)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd2_START  (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd2_END    (8)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id2_START   (9)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id2_END     (11)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd3_START  (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd3_END    (12)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id3_START   (13)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id3_END     (15)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd4_START  (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd4_END    (16)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id4_START   (17)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id4_END     (19)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd5_START  (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd5_END    (20)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id5_START   (21)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id5_END     (23)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd6_START  (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd6_END    (24)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id6_START   (25)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id6_END     (27)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd7_START  (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_cmd7_END    (28)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id7_START   (29)
#define SOC_SCTRL_SC_UNLOCK_CFG_SEC_1_resource0unlock_id7_END     (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_LOCK_STAT_SEC_1_UNION
 struct description   : SC_LOCK_STAT_SEC_1 Register structure definition
                        Address Offset:0x8FC Initial:0x0 Width:32
 register description : 通用资源锁状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  resource0lock_st0    : 1;  /* bit[0-0]  : 通用资源锁0的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁0的锁定状态master ID号0。 */
        unsigned int  resource0lock_st1    : 1;  /* bit[4-4]  : 通用资源锁0的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁0的锁定状态master ID号1。 */
        unsigned int  resource0lock_st2    : 1;  /* bit[8-8]  : 通用资源锁0的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁0的锁定状态master ID号2。 */
        unsigned int  resource0lock_st3    : 1;  /* bit[12-12]: 通用资源锁0的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁0的锁定状态master ID号3。 */
        unsigned int  resource0lock_st4    : 1;  /* bit[16-16]: 通用资源锁0的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁0的锁定状态master ID号4。 */
        unsigned int  resource0lock_st5    : 1;  /* bit[20-20]: 通用资源锁0的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁0的锁定状态master ID号5。 */
        unsigned int  resource0lock_st6    : 1;  /* bit[24-24]: 通用资源锁0的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁0的锁定状态master ID号6。 */
        unsigned int  resource0lock_st7    : 1;  /* bit[28-28]: 通用资源锁0的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id7 : 3;  /* bit[29-31]: SYS_MISC逻辑处理，注意修改
                                                                通用资源锁0的锁定状态master ID号7。 */
    } reg;
} SOC_SCTRL_SC_LOCK_STAT_SEC_1_UNION;
#endif
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st0_START     (0)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st0_END       (0)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id0_START  (1)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id0_END    (3)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st1_START     (4)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st1_END       (4)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id1_START  (5)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id1_END    (7)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st2_START     (8)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st2_END       (8)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id2_START  (9)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id2_END    (11)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st3_START     (12)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st3_END       (12)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id3_START  (13)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id3_END    (15)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st4_START     (16)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st4_END       (16)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id4_START  (17)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id4_END    (19)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st5_START     (20)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st5_END       (20)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id5_START  (21)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id5_END    (23)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st6_START     (24)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st6_END       (24)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id6_START  (25)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id6_END    (27)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st7_START     (28)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st7_END       (28)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id7_START  (29)
#define SOC_SCTRL_SC_LOCK_STAT_SEC_1_resource0lock_st_id7_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCJTAG_SEL_UNION
 struct description   : SCJTAG_SEL Register structure definition
                        Address Offset:0x800 Initial:0x00000000 Width:32
 register description : JTAG选择控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 2;  /* bit[1-2] :  */
        unsigned int  reserved_2: 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCJTAG_SEL_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCCFG_DJTAG_UNION
 struct description   : SCCFG_DJTAG Register structure definition
                        Address Offset:0x814 Initial:0x00000000 Width:32
 register description : DJTAG控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_djtag : 1;  /* bit[0]   : 1：djtag disable
                                                    0：normal
                                                    写一次锁定，密码认证通过后数据才能写入并且只有当efuse_djtag_ctrl为2'b01时，cfg_djtag才输出到djtag_disable。 */
        unsigned int  reserved  : 31; /* bit[1-31]: SYS_MISC逻辑处理，注意修改 */
    } reg;
} SOC_SCTRL_SCCFG_DJTAG_UNION;
#endif
#define SOC_SCTRL_SCCFG_DJTAG_cfg_djtag_START  (0)
#define SOC_SCTRL_SCCFG_DJTAG_cfg_djtag_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCCP15_DISABLE_UNION
 struct description   : SCCP15_DISABLE Register structure definition
                        Address Offset:0x818 Initial:0x000000 Width:32
 register description : CP15DISABLE控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_cp15disable0 : 1;  /* bit[0]    : A53 CPU0的控制bit，写1锁定，CPU0复位上升沿解锁。 */
        unsigned int  a53_cp15disable1 : 1;  /* bit[1]    : A53 CPU1的控制bit，写1锁定，CPU1复位上升沿解锁。 */
        unsigned int  a53_cp15disable2 : 1;  /* bit[2]    : A53 CPU2的控制bit，写1锁定，CPU2复位上升沿解锁。 */
        unsigned int  a53_cp15disable3 : 1;  /* bit[3]    : A53 CPU3的控制bit，写1锁定，CPU3复位上升沿解锁。 */
        unsigned int  a57_cp15disable0 : 1;  /* bit[4]    : A57 CPU0的控制bit，写1锁定，CPU0复位上升沿解锁。 */
        unsigned int  a57_cp15disable1 : 1;  /* bit[5]    : A57 CPU1的控制bit，写1锁定，CPU1复位上升沿解锁。 */
        unsigned int  a57_cp15disable2 : 1;  /* bit[6]    : A57 CPU2的控制bit，写1锁定，CPU2复位上升沿解锁。 */
        unsigned int  a57_cp15disable3 : 1;  /* bit[7]    : A57 CPU3的控制bit，写1锁定，CPU3复位上升沿解锁。 */
        unsigned int  ispa7cp15disable : 1;  /* bit[8]    : ISPA7 CPU的控制bit，写1锁定，CPU复位上升沿解锁。不使用 */
        unsigned int  gicdisable       : 1;  /* bit[9]    : GIC的控制bit，写1锁定，GIC复位上升沿解锁。 */
        unsigned int  reserved         : 22; /* bit[10-31]: SYS_MISC逻辑处理，注意修改 */
    } reg;
} SOC_SCTRL_SCCP15_DISABLE_UNION;
#endif
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable0_START  (0)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable0_END    (0)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable1_START  (1)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable1_END    (1)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable2_START  (2)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable2_END    (2)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable3_START  (3)
#define SOC_SCTRL_SCCP15_DISABLE_a53_cp15disable3_END    (3)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable0_START  (4)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable0_END    (4)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable1_START  (5)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable1_END    (5)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable2_START  (6)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable2_END    (6)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable3_START  (7)
#define SOC_SCTRL_SCCP15_DISABLE_a57_cp15disable3_END    (7)
#define SOC_SCTRL_SCCP15_DISABLE_ispa7cp15disable_START  (8)
#define SOC_SCTRL_SCCP15_DISABLE_ispa7cp15disable_END    (8)
#define SOC_SCTRL_SCCP15_DISABLE_gicdisable_START        (9)
#define SOC_SCTRL_SCCP15_DISABLE_gicdisable_END          (9)


/*****************************************************************************
 struct               : SOC_SCTRL_SCCFG_ARM_DBGEN_UNION
 struct description   : SCCFG_ARM_DBGEN Register structure definition
                        Address Offset:0x82C Initial:0x0000000 Width:32
 register description : ARM debug en配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_arm_dbgen : 4;  /* bit[0-3] : ARM debugen配置。（由系统寄存器逻辑保护每次上电复位之后此值只可写一次。）
                                                        [3:0]={spniden,spiden,niden,dbgen}
                                                        0:disable 1:enable。 */
        unsigned int  reserved      : 28; /* bit[4-31]: SYS_MISC逻辑处理，注意修改 */
    } reg;
} SOC_SCTRL_SCCFG_ARM_DBGEN_UNION;
#endif
#define SOC_SCTRL_SCCFG_ARM_DBGEN_cfg_arm_dbgen_START  (0)
#define SOC_SCTRL_SCCFG_ARM_DBGEN_cfg_arm_dbgen_END    (3)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBGEN_STAT_UNION
 struct description   : SCARM_DBGEN_STAT Register structure definition
                        Address Offset:0x838 Initial:0x00000000 Width:32
 register description : ARM debug en状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbgen_wr : 1;  /* bit[0]   : ARM debugen wr状态寄存器。
                                                       0：当前不可写cfg_arm_dbgen寄存器。1：当前可写cfg_arm_dbgen寄存器。 */
        unsigned int  reserved     : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCARM_DBGEN_STAT_UNION;
#endif
#define SOC_SCTRL_SCARM_DBGEN_STAT_arm_dbgen_wr_START  (0)
#define SOC_SCTRL_SCARM_DBGEN_STAT_arm_dbgen_wr_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCEFUSECTRL_UNION
 struct description   : SCEFUSECTRL Register structure definition
                        Address Offset:0x83C Initial:0x00000000 Width:32
 register description : EFUSE PAD烧写控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sys_efuse_pad_sel : 1;  /* bit[0]   : 0：PAD烧写；
                                                            1：软件烧写。 */
        unsigned int  reserved          : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCEFUSECTRL_UNION;
#endif
#define SOC_SCTRL_SCEFUSECTRL_sys_efuse_pad_sel_START  (0)
#define SOC_SCTRL_SCEFUSECTRL_sys_efuse_pad_sel_END    (0)


/*****************************************************************************
 struct               : SOC_SCTRL_SCEFUSESEL_UNION
 struct description   : SCEFUSESEL Register structure definition
                        Address Offset:0x840 Initial:0x00000000 Width:32
 register description : SECS时钟门控寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   : 不使用此寄存器 */
        unsigned int  reserved_1: 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCEFUSESEL_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCCHIP_ID0_UNION
 struct description   : SCCHIP_ID0 Register structure definition
                        Address Offset:0x848 Initial:0x00000000 Width:32
 register description : EFUSE CHIP_ID0寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  CHIP_ID0 : 32; /* bit[0-31]: CHIP_ID低32bit */
    } reg;
} SOC_SCTRL_SCCHIP_ID0_UNION;
#endif
#define SOC_SCTRL_SCCHIP_ID0_CHIP_ID0_START  (0)
#define SOC_SCTRL_SCCHIP_ID0_CHIP_ID0_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCCHIP_ID1_UNION
 struct description   : SCCHIP_ID1 Register structure definition
                        Address Offset:0x84C Initial:0x00000000 Width:32
 register description : EFUSE CHIP_ID1寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  CHIP_ID1 : 32; /* bit[0-31]: CHIP_ID高32bit */
    } reg;
} SOC_SCTRL_SCCHIP_ID1_UNION;
#endif
#define SOC_SCTRL_SCCHIP_ID1_CHIP_ID1_START  (0)
#define SOC_SCTRL_SCCHIP_ID1_CHIP_ID1_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCCPUSECCTRL_UNION
 struct description   : SCCPUSECCTRL Register structure definition
                        Address Offset:0x850 Initial:0x000000 Width:32
 register description : CPU安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  a53_cryptodisable : 3;  /* bit[0-2] : ATL：大核、中核、小核加密引擎disable
                                                            0：Enable the Cryptography engine；
                                                            1：Disable the Cryptography engine。 */
        unsigned int  a53_sc_cfg        : 5;  /* bit[3-7] : 预留，送给CPU/FCM的配置位。 */
        unsigned int  reserved          : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCCPUSECCTRL_UNION;
#endif
#define SOC_SCTRL_SCCPUSECCTRL_a53_cryptodisable_START  (0)
#define SOC_SCTRL_SCCPUSECCTRL_a53_cryptodisable_END    (2)
#define SOC_SCTRL_SCCPUSECCTRL_a53_sc_cfg_START         (3)
#define SOC_SCTRL_SCCPUSECCTRL_a53_sc_cfg_END           (7)


/*****************************************************************************
 struct               : SOC_SCTRL_SCISPSECCTRL_UNION
 struct description   : SCISPSECCTRL Register structure definition
                        Address Offset:0x830 Initial:0x000000 Width:32
 register description : ISP安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_cryptodisable : 3;  /* bit[0-2] : ATL：加密引擎disable
                                                            0：Enable the Cryptography engine；
                                                            1：Disable the Cryptography engine。 */
        unsigned int  isp_sc_cfg        : 5;  /* bit[3-7] : 预留，送给ISP的配置位。 */
        unsigned int  reserved          : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_SCTRL_SCISPSECCTRL_UNION;
#endif
#define SOC_SCTRL_SCISPSECCTRL_isp_cryptodisable_START  (0)
#define SOC_SCTRL_SCISPSECCTRL_isp_cryptodisable_END    (2)
#define SOC_SCTRL_SCISPSECCTRL_isp_sc_cfg_START         (3)
#define SOC_SCTRL_SCISPSECCTRL_isp_sc_cfg_END           (7)


/*****************************************************************************
 struct               : SOC_SCTRL_SCJTAGSD_SW_SEL_UNION
 struct description   : SCJTAGSD_SW_SEL Register structure definition
                        Address Offset:0x854 Initial:0x0 Width:32
 register description : JTAG_SD SWITCH选择寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  jtagsd_sw_sel            : 8;  /* bit[0-7]  : jtag_mux选择信号，具体含义，请参见Chicago JTAG_MUX Case Allocation Feature List.xlsm */
        unsigned int  auth_pass                : 1;  /* bit[8]    : 1'b1:efuse_key和chip id中0的个数跟efuse值相等；
                                                                    1'b0:efuse_key和chip id中0的个数跟efuse值不相等； */
        unsigned int  arm_dbgen_wr1            : 1;  /* bit[9]    : ARM debugen wr状态寄存器。
                                                                    0：当前不可写cfg_arm_dbgen寄存器。1：当前可写cfg_arm_dbgen寄存器。
                                                                    bit含义同0x838 bit0； */
        unsigned int  jtag_sd_swd_sel          : 1;  /* bit[10]   : 软件配置支路选择信号，0为BYPASS，1为SWD2JTAG。默认值为0。 */
        unsigned int  jtag_sd_watcher_bypass   : 1;  /* bit[11]   : watcher bypass信号，1为bypass。默认值为0。 */
        unsigned int  jtag_sd_watcher_tms_tie1 : 1;  /* bit[12]   : watcher tms输入tie1信号，1为tms tie1，0为tms从IOC输入。默认值为0. */
        unsigned int  reserved                 : 12; /* bit[13-24]: 保留。 */
        unsigned int  jtag_sim_hwselmd         : 2;  /* bit[25-26]: SIM JTAG IO硬线切换使能。
                                                                    2’b0x:屏蔽IO的硬线切换，
                                                                    2’b10:使能IO的硬线切换，且IO低电平表示使能调试；
                                                                    2’b11:使能IO的硬线切换，且IO高电平表示使能调试；
                                                                    注：用SIM卡卡托检测脚的GPIO做复用的硬线切换：卡托在位则为SIM卡，卡托不在位将SIM卡切换为JTAG；给IOC的选择条件逻辑为:
                                                                    jtag_sim_mode_ioc = （jtag_sim_mode | ((gpio_sim_detect ^~ jtag_sim_hwselmd[0] ) & jtag_sim_hwselmd[1])） & （~jtag_sd_mode_ioc）  */
        unsigned int  jtag_sd_hwselmd          : 2;  /* bit[27-28]: 无SD卡
                                                                    SD JTAG IO硬线切换使能模式
                                                                    2’b0x:屏蔽IO的硬线切换，
                                                                    2’b10:使能IO的硬线切换，且IO低电平表示使能调试；
                                                                    2’b11:使能IO的硬线切换，且IO高电平表示使能调试；
                                                                    注：用SD卡卡托检测脚的GPIO做复用的硬线切换：卡托在位则为SIM卡，卡托不在位将SIM卡切换为JTAG；给IOC的选择条件逻辑为:
                                                                    jtag_sd_mode_ioc = (jtag_sd_mode | ((gpio_sd_detect ^~ jtag_sd_hwselmd[0] ) & jtag_sd_hwselmd[1])) */
        unsigned int  gpio_sd_sim_sel          : 1;  /* bit[29]   : assign gpio_sim_detect = ~gpio_sd_sim_sel ? gpio_29_in[2] : 1’b1 ;
                                                                    assign gpio_sd_detect = gpio_sd_sim_sel ? gpio_29_in[2] : 1’b1: */
        unsigned int  jtag_sim_mode            : 1;  /* bit[30]   : 送给IOC的控制信号
                                                                    1'b0：sim卡IO不是能jtag调节模式，芯片保持正常功能模式；
                                                                    1'b1：sim卡IO使能jtag调试功能。 */
        unsigned int  jtag_sd_mode             : 1;  /* bit[31-31]: 无SD卡
                                                                    送给JTAG MUX的控制信号
                                                                    0: 不使能jtag调试功能，芯片保持为正常功能模式
                                                                    1：使能jtag调试功能。 */
    } reg;
} SOC_SCTRL_SCJTAGSD_SW_SEL_UNION;
#endif
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtagsd_sw_sel_START             (0)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtagsd_sw_sel_END               (7)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_auth_pass_START                 (8)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_auth_pass_END                   (8)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_arm_dbgen_wr1_START             (9)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_arm_dbgen_wr1_END               (9)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_swd_sel_START           (10)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_swd_sel_END             (10)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_watcher_bypass_START    (11)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_watcher_bypass_END      (11)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_watcher_tms_tie1_START  (12)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_watcher_tms_tie1_END    (12)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sim_hwselmd_START          (25)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sim_hwselmd_END            (26)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_hwselmd_START           (27)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_hwselmd_END             (28)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_gpio_sd_sim_sel_START           (29)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_gpio_sd_sim_sel_END             (29)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sim_mode_START             (30)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sim_mode_END               (30)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_mode_START              (31)
#define SOC_SCTRL_SCJTAGSD_SW_SEL_jtag_sd_mode_END                (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCJTAGSYS_SW_SEL_UNION
 struct description   : SCJTAGSYS_SW_SEL Register structure definition
                        Address Offset:0x858 Initial:0x0000 Width:32
 register description : JTAG_SYS SWITCH选择寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  jtagsys_sw_sel            : 8;  /* bit[0-7]  : jtag_mux选择信号，具体含义，请参见Chicago JTAG_MUX Case Allocation Feature List.xlsm */
        unsigned int  jtag_sys_swd_sel          : 1;  /* bit[8]    : 软件配置支路选择信号，0为BYPASS，1为SWD2JTAG。默认值为0。 */
        unsigned int  jtag_sys_watcher_bypass   : 1;  /* bit[9]    : watcher bypass信号，1为bypass。默认值为0。 */
        unsigned int  jtag_sys_watcher_tms_tie1 : 1;  /* bit[10]   : watcher tms输入tie1信号，1为tms tie1，0为tms从IOC输入。默认值为0. */
        unsigned int  reserved_0                : 5;  /* bit[11-15]: 保留。 */
        unsigned int  reserved_1                : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SCJTAGSYS_SW_SEL_UNION;
#endif
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtagsys_sw_sel_START             (0)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtagsys_sw_sel_END               (7)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtag_sys_swd_sel_START           (8)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtag_sys_swd_sel_END             (8)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtag_sys_watcher_bypass_START    (9)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtag_sys_watcher_bypass_END      (9)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtag_sys_watcher_tms_tie1_START  (10)
#define SOC_SCTRL_SCJTAGSYS_SW_SEL_jtag_sys_watcher_tms_tie1_END    (10)


/*****************************************************************************
 struct               : SOC_SCTRL_SCJTAG_SD_SWD2jtag_dfx_UNION
 struct description   : SCJTAG_SD_SWD2jtag_dfx Register structure definition
                        Address Offset:0x870 Initial:0x00000000 Width:32
 register description : JTAG回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: JTAG_SD swd2jtag DFX信号 */
    } reg;
} SOC_SCTRL_SCJTAG_SD_SWD2jtag_dfx_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCJTAG_SYS_SWD2jtag_dfx_UNION
 struct description   : SCJTAG_SYS_SWD2jtag_dfx Register structure definition
                        Address Offset:0x874 Initial:0x00000000 Width:32
 register description : JTAG回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: JTAG_SYS swd2jtag DFX信号 */
    } reg;
} SOC_SCTRL_SCJTAG_SYS_SWD2jtag_dfx_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_INTR_SYSCACHE_SEC_UNION
 struct description   : SC_INTR_SYSCACHE_SEC Register structure definition
                        Address Offset:0x85C Initial:0x0000 Width:32
 register description : syscache中断控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_iomcu_enable_syscache_gen   : 1;  /* bit[0]    : iomcu_enable_syscache中断产生 */
        unsigned int  intr_iomcu_enable_syscache_clr   : 1;  /* bit[1]    : iomcu_enable_syscache中断清除 */
        unsigned int  intr_iomcu_enable_syscache_msk   : 1;  /* bit[2]    : iomcu_enable_syscache中断屏蔽: 1屏蔽；0不屏蔽。 */
        unsigned int  reserved_0                       : 1;  /* bit[3]    :  */
        unsigned int  intr_iomcu_enable_syscache_raw   : 1;  /* bit[4]    : iomcu_enable_syscache原始中断（屏蔽前中断） */
        unsigned int  intr_iomcu_enable_syscache_stat  : 1;  /* bit[5]    : iomcu_enable_syscache中断状态（屏蔽后中断，送给LPM3） */
        unsigned int  reserved_1                       : 2;  /* bit[6-7]  :  */
        unsigned int  intr_iomcu_disable_syscache_gen  : 1;  /* bit[8]    : iomcu_disable_syscache中断产生 */
        unsigned int  intr_iomcu_disable_syscache_clr  : 1;  /* bit[9]    : iomcu_disable_syscache中断清除 */
        unsigned int  intr_iomcu_disable_syscache_msk  : 1;  /* bit[10]   : iomcu_disable_syscache中断屏蔽: 1屏蔽；0不屏蔽。 */
        unsigned int  reserved_2                       : 1;  /* bit[11]   :  */
        unsigned int  intr_iomcu_disable_syscache_raw  : 1;  /* bit[12]   : iomcu_disable_syscache原始中断（屏蔽前中断） */
        unsigned int  intr_iomcu_disable_syscache_stat : 1;  /* bit[13]   : iomcu_disable_syscache中断状态（屏蔽后中断，送给LPM3） */
        unsigned int  reserved_3                       : 2;  /* bit[14-15]:  */
        unsigned int  reserved_4                       : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SC_INTR_SYSCACHE_SEC_UNION;
#endif
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_gen_START    (0)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_gen_END      (0)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_clr_START    (1)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_clr_END      (1)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_msk_START    (2)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_msk_END      (2)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_raw_START    (4)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_raw_END      (4)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_stat_START   (5)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_enable_syscache_stat_END     (5)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_gen_START   (8)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_gen_END     (8)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_clr_START   (9)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_clr_END     (9)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_msk_START   (10)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_msk_END     (10)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_raw_START   (12)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_raw_END     (12)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_stat_START  (13)
#define SOC_SCTRL_SC_INTR_SYSCACHE_SEC_intr_iomcu_disable_syscache_stat_END    (13)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY0_UNION
 struct description   : SCARM_DBG_KEY0 Register structure definition
                        Address Offset:0x860 Initial:0x00000000 Width:32
 register description : ARM debug key0配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key0 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[31:0]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY0_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY0_arm_dbg_key0_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY0_arm_dbg_key0_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY1_UNION
 struct description   : SCARM_DBG_KEY1 Register structure definition
                        Address Offset:0x864 Initial:0x00000000 Width:32
 register description : ARM debug key1配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key1 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[63:31]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY1_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY1_arm_dbg_key1_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY1_arm_dbg_key1_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY2_UNION
 struct description   : SCARM_DBG_KEY2 Register structure definition
                        Address Offset:0x868 Initial:0x00000000 Width:32
 register description : ARM debug key2配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key2 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[95:64]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY2_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY2_arm_dbg_key2_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY2_arm_dbg_key2_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY3_UNION
 struct description   : SCARM_DBG_KEY3 Register structure definition
                        Address Offset:0x86C Initial:0x00000000 Width:32
 register description : ARM debug key3配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key3 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[127:96]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY3_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY3_arm_dbg_key3_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY3_arm_dbg_key3_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY4_UNION
 struct description   : SCARM_DBG_KEY4 Register structure definition
                        Address Offset:0x884 Initial:0x00000000 Width:32
 register description : ARM debug key4配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key4 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[159:128]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY4_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY4_arm_dbg_key4_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY4_arm_dbg_key4_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY5_UNION
 struct description   : SCARM_DBG_KEY5 Register structure definition
                        Address Offset:0x888 Initial:0x00000000 Width:32
 register description : ARM debug key5配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key5 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[191:160]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY5_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY5_arm_dbg_key5_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY5_arm_dbg_key5_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY6_UNION
 struct description   : SCARM_DBG_KEY6 Register structure definition
                        Address Offset:0x88C Initial:0x00000000 Width:32
 register description : ARM debug key6配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key6 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[223:192]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY6_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY6_arm_dbg_key6_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY6_arm_dbg_key6_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCARM_DBG_KEY7_UNION
 struct description   : SCARM_DBG_KEY7 Register structure definition
                        Address Offset:0x890 Initial:0x00000000 Width:32
 register description : ARM debug key7配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arm_dbg_key7 : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                       arm_dbg_key bit[255:224]。
                                                       （由系统寄存器逻辑保护每次上电复位之后此值只可写3次）。 */
    } reg;
} SOC_SCTRL_SCARM_DBG_KEY7_UNION;
#endif
#define SOC_SCTRL_SCARM_DBG_KEY7_arm_dbg_key7_START  (0)
#define SOC_SCTRL_SCARM_DBG_KEY7_arm_dbg_key7_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR0_UNION
 struct description   : SCSPMIADDR0 Register structure definition
                        Address Offset:0x920 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr0 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR0_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR0_spmi_avs_addr0_START  (0)
#define SOC_SCTRL_SCSPMIADDR0_spmi_avs_addr0_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR1_UNION
 struct description   : SCSPMIADDR1 Register structure definition
                        Address Offset:0x924 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr1 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR1_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR1_spmi_avs_addr1_START  (0)
#define SOC_SCTRL_SCSPMIADDR1_spmi_avs_addr1_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR2_UNION
 struct description   : SCSPMIADDR2 Register structure definition
                        Address Offset:0x928 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr2 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR2_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR2_spmi_avs_addr2_START  (0)
#define SOC_SCTRL_SCSPMIADDR2_spmi_avs_addr2_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR3_UNION
 struct description   : SCSPMIADDR3 Register structure definition
                        Address Offset:0x92C Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr3 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR3_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR3_spmi_avs_addr3_START  (0)
#define SOC_SCTRL_SCSPMIADDR3_spmi_avs_addr3_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR4_UNION
 struct description   : SCSPMIADDR4 Register structure definition
                        Address Offset:0x930 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr4 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR4_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR4_spmi_avs_addr4_START  (0)
#define SOC_SCTRL_SCSPMIADDR4_spmi_avs_addr4_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR5_UNION
 struct description   : SCSPMIADDR5 Register structure definition
                        Address Offset:0x934 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr5 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR5_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR5_spmi_avs_addr5_START  (0)
#define SOC_SCTRL_SCSPMIADDR5_spmi_avs_addr5_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR6_UNION
 struct description   : SCSPMIADDR6 Register structure definition
                        Address Offset:0x938 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr6 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR6_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR6_spmi_avs_addr6_START  (0)
#define SOC_SCTRL_SCSPMIADDR6_spmi_avs_addr6_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR7_UNION
 struct description   : SCSPMIADDR7 Register structure definition
                        Address Offset:0x93C Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr7 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR7_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR7_spmi_avs_addr7_START  (0)
#define SOC_SCTRL_SCSPMIADDR7_spmi_avs_addr7_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR8_UNION
 struct description   : SCSPMIADDR8 Register structure definition
                        Address Offset:0x940 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr8 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR8_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR8_spmi_avs_addr8_START  (0)
#define SOC_SCTRL_SCSPMIADDR8_spmi_avs_addr8_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR9_UNION
 struct description   : SCSPMIADDR9 Register structure definition
                        Address Offset:0x944 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr9 : 20; /* bit[0-19] :  */
        unsigned int  reserved       : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR9_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR9_spmi_avs_addr9_START  (0)
#define SOC_SCTRL_SCSPMIADDR9_spmi_avs_addr9_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR10_UNION
 struct description   : SCSPMIADDR10 Register structure definition
                        Address Offset:0x948 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr10 : 20; /* bit[0-19] :  */
        unsigned int  reserved        : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR10_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR10_spmi_avs_addr10_START  (0)
#define SOC_SCTRL_SCSPMIADDR10_spmi_avs_addr10_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR11_UNION
 struct description   : SCSPMIADDR11 Register structure definition
                        Address Offset:0x94C Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr11 : 20; /* bit[0-19] :  */
        unsigned int  reserved        : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR11_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR11_spmi_avs_addr11_START  (0)
#define SOC_SCTRL_SCSPMIADDR11_spmi_avs_addr11_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR12_UNION
 struct description   : SCSPMIADDR12 Register structure definition
                        Address Offset:0x950 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr12 : 20; /* bit[0-19] :  */
        unsigned int  reserved        : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR12_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR12_spmi_avs_addr12_START  (0)
#define SOC_SCTRL_SCSPMIADDR12_spmi_avs_addr12_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR13_UNION
 struct description   : SCSPMIADDR13 Register structure definition
                        Address Offset:0x954 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr13 : 20; /* bit[0-19] :  */
        unsigned int  reserved        : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR13_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR13_spmi_avs_addr13_START  (0)
#define SOC_SCTRL_SCSPMIADDR13_spmi_avs_addr13_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR14_UNION
 struct description   : SCSPMIADDR14 Register structure definition
                        Address Offset:0x958 Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr14 : 20; /* bit[0-19] :  */
        unsigned int  reserved        : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR14_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR14_spmi_avs_addr14_START  (0)
#define SOC_SCTRL_SCSPMIADDR14_spmi_avs_addr14_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSPMIADDR15_UNION
 struct description   : SCSPMIADDR15 Register structure definition
                        Address Offset:0x95C Initial:0x000 Width:32
 register description : SPMI地址配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi_avs_addr15 : 20; /* bit[0-19] :  */
        unsigned int  reserved        : 12; /* bit[20-31]:  */
    } reg;
} SOC_SCTRL_SCSPMIADDR15_UNION;
#endif
#define SOC_SCTRL_SCSPMIADDR15_spmi_avs_addr15_START  (0)
#define SOC_SCTRL_SCSPMIADDR15_spmi_avs_addr15_END    (19)


/*****************************************************************************
 struct               : SOC_SCTRL_SCHISEEGPIOLOCK_UNION
 struct description   : SCHISEEGPIOLOCK Register structure definition
                        Address Offset:0x894 Initial:0x00000000 Width:32
 register description : HISEE子系统状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                   gpio1/2/3解锁配置寄存器，hisee_gpio_lock，只能写不能读。 */
    } reg;
} SOC_SCTRL_SCHISEEGPIOLOCK_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCHISEESPILOCK_UNION
 struct description   : SCHISEESPILOCK Register structure definition
                        Address Offset:0x898 Initial:0x00000000 Width:32
 register description : HISEE子系统状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                   spi解锁配置寄存器，hisee_spi_lock，只能写不能读。 */
    } reg;
} SOC_SCTRL_SCHISEESPILOCK_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCHISEEI2CLOCK_UNION
 struct description   : SCHISEEI2CLOCK Register structure definition
                        Address Offset:0x89C Initial:0x00000000 Width:32
 register description : HISEE子系统状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: SYS_MISC逻辑处理，注意修改
                                                   i2c解锁配置寄存器，hisee_i2c_lock，只能写不能读。 */
    } reg;
} SOC_SCTRL_SCHISEEI2CLOCK_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCHISEEIOSEL_UNION
 struct description   : SCHISEEIOSEL Register structure definition
                        Address Offset:0x8A0 Initial:0x0 Width:32
 register description : HISEE IO选择回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hisee_gpio1_io_sel : 1;  /* bit[0]   :  */
        unsigned int  hisee_gpio2_io_sel : 1;  /* bit[1]   :  */
        unsigned int  hisee_gpio3_io_sel : 1;  /* bit[2]   :  */
        unsigned int  hisee_spi_io_sel   : 1;  /* bit[3]   :  */
        unsigned int  hisee_i2c_io_sel   : 1;  /* bit[4]   :  */
        unsigned int  reserved_0         : 26; /* bit[5-30]:  */
        unsigned int  reserved_1         : 1;  /* bit[31]  :  */
    } reg;
} SOC_SCTRL_SCHISEEIOSEL_UNION;
#endif
#define SOC_SCTRL_SCHISEEIOSEL_hisee_gpio1_io_sel_START  (0)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_gpio1_io_sel_END    (0)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_gpio2_io_sel_START  (1)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_gpio2_io_sel_END    (1)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_gpio3_io_sel_START  (2)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_gpio3_io_sel_END    (2)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_spi_io_sel_START    (3)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_spi_io_sel_END      (3)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_i2c_io_sel_START    (4)
#define SOC_SCTRL_SCHISEEIOSEL_hisee_i2c_io_sel_END      (4)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL0_SEC_UNION
 struct description   : SCPERCTRL0_SEC Register structure definition
                        Address Offset:0x8A4 Initial:0xF Width:32
 register description : SC安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    :  */
        unsigned int  reserved_1: 1;  /* bit[1]    :  */
        unsigned int  reserved_2: 2;  /* bit[2-3]  :  */
        unsigned int  reserved_3: 1;  /* bit[4]    :  */
        unsigned int  reserved_4: 2;  /* bit[5-6]  :  */
        unsigned int  reserved_5: 13; /* bit[7-19] :  */
        unsigned int  reserved_6: 1;  /* bit[20]   :  */
        unsigned int  reserved_7: 1;  /* bit[21]   :  */
        unsigned int  reserved_8: 2;  /* bit[22-23]:  */
        unsigned int  reserved_9: 4;  /* bit[24-27]:  */
        unsigned int  reserved_10: 4;  /* bit[28-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL0_SEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL1_SEC_UNION
 struct description   : SCPERCTRL1_SEC Register structure definition
                        Address Offset:0x8B0 Initial:0x00 Width:32
 register description : SC安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  : 保留。 */
        unsigned int  reserved_1: 19; /* bit[6-24] : 保留。 */
        unsigned int  reserved_2: 7;  /* bit[25-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL1_SEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL2_SEC_UNION
 struct description   : SCPERCTRL2_SEC Register structure definition
                        Address Offset:0x8B4 Initial:0x0000 Width:32
 register description : SC安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpmcu_arc_dbg_port_sel : 1;  /* bit[0]    : M3核不使用 */
        unsigned int  lpmcu_arc_halt_req_a   : 1;  /* bit[1]    : M3核不使用 */
        unsigned int  lpmcu_arc_run_req_a    : 1;  /* bit[2]    : M3核不使用 */
        unsigned int  reserved_0             : 3;  /* bit[3-5]  :  */
        unsigned int  reserved_1             : 2;  /* bit[6-7]  :  */
        unsigned int  reserved_2             : 8;  /* bit[8-15] :  */
        unsigned int  scperctrl2_sec_msk     : 16; /* bit[16-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL2_SEC_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL2_SEC_lpmcu_arc_dbg_port_sel_START  (0)
#define SOC_SCTRL_SCPERCTRL2_SEC_lpmcu_arc_dbg_port_sel_END    (0)
#define SOC_SCTRL_SCPERCTRL2_SEC_lpmcu_arc_halt_req_a_START    (1)
#define SOC_SCTRL_SCPERCTRL2_SEC_lpmcu_arc_halt_req_a_END      (1)
#define SOC_SCTRL_SCPERCTRL2_SEC_lpmcu_arc_run_req_a_START     (2)
#define SOC_SCTRL_SCPERCTRL2_SEC_lpmcu_arc_run_req_a_END       (2)
#define SOC_SCTRL_SCPERCTRL2_SEC_scperctrl2_sec_msk_START      (16)
#define SOC_SCTRL_SCPERCTRL2_SEC_scperctrl2_sec_msk_END        (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL3_SEC_UNION
 struct description   : SCPERCTRL3_SEC Register structure definition
                        Address Offset:0x8B8 Initial:0x0000000 Width:32
 register description : SC安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 3;  /* bit[0-2] :  */
        unsigned int  reserved_1: 1;  /* bit[3]   :  */
        unsigned int  reserved_2: 28; /* bit[4-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL3_SEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL4_SEC_UNION
 struct description   : SCPERCTRL4_SEC Register structure definition
                        Address Offset:0x8BC Initial:0x0FFFF Width:32
 register description : SC安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hisee_gpio1_ack : 4;  /* bit[0-3]  : 用于hisec的otp trim 端口，不使用 */
        unsigned int  hisee_gpio2_ack : 4;  /* bit[4-7]  : 用于hisec的otp trim 端口，不使用 */
        unsigned int  hisee_gpio3_ack : 4;  /* bit[8-11] : 用于hisec的otp trim 端口，不使用 */
        unsigned int  reserved        : 20; /* bit[12-31]:  */
    } reg;
} SOC_SCTRL_SCPERCTRL4_SEC_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL4_SEC_hisee_gpio1_ack_START  (0)
#define SOC_SCTRL_SCPERCTRL4_SEC_hisee_gpio1_ack_END    (3)
#define SOC_SCTRL_SCPERCTRL4_SEC_hisee_gpio2_ack_START  (4)
#define SOC_SCTRL_SCPERCTRL4_SEC_hisee_gpio2_ack_END    (7)
#define SOC_SCTRL_SCPERCTRL4_SEC_hisee_gpio3_ack_START  (8)
#define SOC_SCTRL_SCPERCTRL4_SEC_hisee_gpio3_ack_END    (11)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERCTRL5_SEC_UNION
 struct description   : SCPERCTRL5_SEC Register structure definition
                        Address Offset:0x8E0 Initial:0x0000 Width:32
 register description : SC安全控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 8;  /* bit[0-7]  :  */
        unsigned int  reserved_1         : 1;  /* bit[8]    :  */
        unsigned int  reserved_2         : 1;  /* bit[9]    :  */
        unsigned int  reserved_3         : 1;  /* bit[10]   :  */
        unsigned int  reserved_4         : 4;  /* bit[11-14]:  */
        unsigned int  iomcu_debug_sel    : 1;  /* bit[15]   :  */
        unsigned int  scperctrl5_sec_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SCPERCTRL5_SEC_UNION;
#endif
#define SOC_SCTRL_SCPERCTRL5_SEC_iomcu_debug_sel_START     (15)
#define SOC_SCTRL_SCPERCTRL5_SEC_iomcu_debug_sel_END       (15)
#define SOC_SCTRL_SCPERCTRL5_SEC_scperctrl5_sec_msk_START  (16)
#define SOC_SCTRL_SCPERCTRL5_SEC_scperctrl5_sec_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS0_SEC_UNION
 struct description   : SCPERSTATUS0_SEC Register structure definition
                        Address Offset:0x8C0 Initial:0x0 Width:32
 register description : SC安全状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  arc_run_ack  : 1;  /* bit[0]    :  */
        unsigned int  arc_halt_ack : 1;  /* bit[1]    :  */
        unsigned int  reserved_0   : 1;  /* bit[2]    :  */
        unsigned int  reserved_1   : 1;  /* bit[3]    :  */
        unsigned int  reserved_2   : 1;  /* bit[4]    :  */
        unsigned int  reserved_3   : 3;  /* bit[5-7]  :  */
        unsigned int  reserved_4   : 8;  /* bit[8-15] :  */
        unsigned int  reserved_5   : 1;  /* bit[16]   :  */
        unsigned int  reserved_6   : 1;  /* bit[17]   :  */
        unsigned int  reserved_7   : 1;  /* bit[18]   :  */
        unsigned int  reserved_8   : 1;  /* bit[19]   :  */
        unsigned int  reserved_9   : 1;  /* bit[20]   :  */
        unsigned int  reserved_10  : 1;  /* bit[21]   :  */
        unsigned int  reserved_11  : 9;  /* bit[22-30]:  */
        unsigned int  reserved_12  : 1;  /* bit[31]   :  */
    } reg;
} SOC_SCTRL_SCPERSTATUS0_SEC_UNION;
#endif
#define SOC_SCTRL_SCPERSTATUS0_SEC_arc_run_ack_START   (0)
#define SOC_SCTRL_SCPERSTATUS0_SEC_arc_run_ack_END     (0)
#define SOC_SCTRL_SCPERSTATUS0_SEC_arc_halt_ack_START  (1)
#define SOC_SCTRL_SCPERSTATUS0_SEC_arc_halt_ack_END    (1)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS1_SEC_UNION
 struct description   : SCPERSTATUS1_SEC Register structure definition
                        Address Offset:0x8C4 Initial:0x00000000 Width:32
 register description : SC安全状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS1_SEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS2_SEC_UNION
 struct description   : SCPERSTATUS2_SEC Register structure definition
                        Address Offset:0x8C8 Initial:0x00000000 Width:32
 register description : SC安全状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS2_SEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTATUS3_SEC_UNION
 struct description   : SCPERSTATUS3_SEC Register structure definition
                        Address Offset:0x8CC Initial:0x00000000 Width:32
 register description : SC安全状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_SCTRL_SCPERSTATUS3_SEC_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_REQ_0_UNION
 struct description   : SC_TCP_VOTE_REQ_0 Register structure definition
                        Address Offset:0x8D0 Initial:0x0000 Width:32
 register description : SC_TCP_VOTE_REQ
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vote_req_0            : 7;  /* bit[0-6]  : bit[0]: LPM3 WFI投票使能配置（1'b0:不使能，1'b1:使能）
                                                                 bit[6:1]：软件投票，每bit对应不同的投票源
                                                                 bit[1]: lpmcu;bit[2]: iomcu; bit[3]: asp. */
        unsigned int  vote_en_0             : 1;  /* bit[7]    : 投票使能配置 */
        unsigned int  vote_req_1            : 7;  /* bit[8-14] : bit[8]: 系统唤醒源投票使能配置（1'b0:不使能，1'b1:使能）
                                                                 bit[14:9]：软件投票，每bit对应不同的投票源
                                                                 bit[9]:lpmcu, bit[10]:iomcu, bit[11]: asp */
        unsigned int  vote_en_1             : 1;  /* bit[15]   : 投票使能配置 */
        unsigned int  sc_tcp_vote_req_0_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_REQ_0_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_req_0_START             (0)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_req_0_END               (6)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_en_0_START              (7)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_en_0_END                (7)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_req_1_START             (8)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_req_1_END               (14)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_en_1_START              (15)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_vote_en_1_END                (15)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_sc_tcp_vote_req_0_msk_START  (16)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_0_sc_tcp_vote_req_0_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_STAT_UNION
 struct description   : SC_TCP_VOTE_STAT Register structure definition
                        Address Offset:0x8D4 Initial:0x0 Width:32
 register description : SC_TCP_VOTE_STAT
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vote_close_req_0      : 1;  /* bit[0]    : VOTE_MNG FSM发出的关闭请求 */
        unsigned int  vote_open_req_0       : 1;  /* bit[1]    : VOTE_MNG FSM发出的打开请求 */
        unsigned int  vote_close_ack_sync_0 : 1;  /* bit[2]    : VOTE_MNG FSM接收的关闭响应 */
        unsigned int  vote_open_ack_sync_0  : 1;  /* bit[3]    : VOTE_MNG FSM接收的打开响应 */
        unsigned int  reserved_0            : 1;  /* bit[4]    :  */
        unsigned int  cur_path_stat_0       : 3;  /* bit[5-7]  : VOTE_MNG FSM的状态机状态
                                                                 3'b000: PATH_OPEN, 表示资源可访问
                                                                 3'b001：WAIT_CLOSE_ACK
                                                                 3'b100: WAIT_CLOSE_CMPL
                                                                 3'b010: PATH_CLOSE, 表示资源不可访问
                                                                 3'b011: WAIT_OPEN_ACK
                                                                 3'b101: WAIT_OPEN_CMPL */
        unsigned int  vote_close_req_1      : 1;  /* bit[8]    : VOTE_MNG FSM发出的关闭请求 */
        unsigned int  vote_open_req_1       : 1;  /* bit[9]    : VOTE_MNG FSM发出的打开请求 */
        unsigned int  vote_close_ack_sync_1 : 1;  /* bit[10]   : VOTE_MNG FSM接收的关闭响应 */
        unsigned int  vote_open_ack_sync_1  : 1;  /* bit[11]   : VOTE_MNG FSM接收的打开响应 */
        unsigned int  reserved_1            : 1;  /* bit[12]   :  */
        unsigned int  cur_path_stat_1       : 3;  /* bit[13-15]: VOTE_MNG FSM的状态机状态,同stat_0 */
        unsigned int  vote_close_req_2      : 1;  /* bit[16]   : VOTE_MNG FSM发出的关闭请求 */
        unsigned int  vote_open_req_2       : 1;  /* bit[17]   : VOTE_MNG FSM发出的打开请求 */
        unsigned int  vote_close_ack_sync_2 : 1;  /* bit[18]   : VOTE_MNG FSM接收的关闭响应 */
        unsigned int  vote_open_ack_sync_2  : 1;  /* bit[19]   : VOTE_MNG FSM接收的打开响应 */
        unsigned int  reserved_2            : 1;  /* bit[20]   :  */
        unsigned int  cur_path_stat_2       : 3;  /* bit[21-23]: VOTE_MNG FSM的状态机状态，同stat_0 */
        unsigned int  vote_close_req_3      : 1;  /* bit[24]   : VOTE_MNG FSM发出的关闭请求 */
        unsigned int  vote_open_req_3       : 1;  /* bit[25]   : VOTE_MNG FSM发出的打开请求 */
        unsigned int  vote_close_ack_sync_3 : 1;  /* bit[26]   : VOTE_MNG FSM接收的关闭响应 */
        unsigned int  vote_open_ack_sync_3  : 1;  /* bit[27]   : VOTE_MNG FSM接收的打开响应 */
        unsigned int  reserved_3            : 1;  /* bit[28]   :  */
        unsigned int  cur_path_stat_3       : 3;  /* bit[29-31]: VOTE_MNG FSM的状态机状态，同stat_0 */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_STAT_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_0_START       (0)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_0_END         (0)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_0_START        (1)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_0_END          (1)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_0_START  (2)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_0_END    (2)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_0_START   (3)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_0_END     (3)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_0_START        (5)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_0_END          (7)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_1_START       (8)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_1_END         (8)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_1_START        (9)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_1_END          (9)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_1_START  (10)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_1_END    (10)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_1_START   (11)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_1_END     (11)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_1_START        (13)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_1_END          (15)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_2_START       (16)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_2_END         (16)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_2_START        (17)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_2_END          (17)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_2_START  (18)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_2_END    (18)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_2_START   (19)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_2_END     (19)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_2_START        (21)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_2_END          (23)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_3_START       (24)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_req_3_END         (24)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_3_START        (25)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_req_3_END          (25)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_3_START  (26)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_close_ack_sync_3_END    (26)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_3_START   (27)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_vote_open_ack_sync_3_END     (27)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_3_START        (29)
#define SOC_SCTRL_SC_TCP_VOTE_STAT_cur_path_stat_3_END          (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_CTRL0_UNION
 struct description   : SC_TCP_VOTE_CTRL0 Register structure definition
                        Address Offset:0x8D8 Initial:0x0 Width:32
 register description : SC_TCP_VOTE_CTRL
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]    :  */
        unsigned int  reserved_1         : 1;  /* bit[1]    :  */
        unsigned int  reserved_2         : 1;  /* bit[2]    : TCP BUSY状态切换FLL使能配置 */
        unsigned int  reserved_3         : 1;  /* bit[3]    : TCP切换FLL请求 */
        unsigned int  tcp_busy_stat_sync : 1;  /* bit[4]    : TCP BUSY状态 */
        unsigned int  reserved_4         : 1;  /* bit[5]    : ASP切换FLL请求 */
        unsigned int  reserved_5         : 1;  /* bit[6]    : IOMCU切换FLL请求 */
        unsigned int  int_wakeup_sys_ff2 : 1;  /* bit[7]    : 唤醒中断 */
        unsigned int  reserved_6         : 1;  /* bit[8]    : 汇聚后的切换FLL请求 */
        unsigned int  mad_int_sync       : 1;  /* bit[9]    :  */
        unsigned int  reserved_7         : 21; /* bit[10-30]:  */
        unsigned int  reserved_8         : 1;  /* bit[31]   :  */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_CTRL0_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_tcp_busy_stat_sync_START  (4)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_tcp_busy_stat_sync_END    (4)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_int_wakeup_sys_ff2_START  (7)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_int_wakeup_sys_ff2_END    (7)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_mad_int_sync_START        (9)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL0_mad_int_sync_END          (9)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_REQ_1_UNION
 struct description   : SC_TCP_VOTE_REQ_1 Register structure definition
                        Address Offset:0x8DC Initial:0x0000 Width:32
 register description : SC_TCP_VOTE_REQ
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vote_req_2            : 7;  /* bit[0-6]  : bit[0]: mad_int投票使能配置（1'b0:不使能，1'b1:使能）
                                                                 bit[6:1]：软件投票，每bit对应不同的投票源：1'b1表示上电请求；1'b0表示下电请求。其中bit[1]为LPM3软件投票位 */
        unsigned int  vote_en_2             : 1;  /* bit[7]    : 投票使能配置 */
        unsigned int  vote_req_3            : 7;  /* bit[8-14] : 软件投票，每bit对应不同的投票源 */
        unsigned int  vote_en_3             : 1;  /* bit[15]   : 投票使能配置 */
        unsigned int  sc_tcp_vote_req_1_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_REQ_1_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_req_2_START             (0)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_req_2_END               (6)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_en_2_START              (7)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_en_2_END                (7)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_req_3_START             (8)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_req_3_END               (14)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_en_3_START              (15)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_vote_en_3_END                (15)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_sc_tcp_vote_req_1_msk_START  (16)
#define SOC_SCTRL_SC_TCP_VOTE_REQ_1_sc_tcp_vote_req_1_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_CTRL1_UNION
 struct description   : SC_TCP_VOTE_CTRL1 Register structure definition
                        Address Offset:0x8E4 Initial:0x000000 Width:32
 register description : SC_TCP_VOTE_CTRL
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vote_mng_sw2_path_open_req_0  : 1;  /* bit[0]   :  */
        unsigned int  vote_mng_sw2_path_close_req_0 : 1;  /* bit[1]   :  */
        unsigned int  vote_mng_sw2_path_open_req_1  : 1;  /* bit[2]   :  */
        unsigned int  vote_mng_sw2_path_close_req_1 : 1;  /* bit[3]   :  */
        unsigned int  vote_mng_sw2_path_open_req_2  : 1;  /* bit[4]   :  */
        unsigned int  vote_mng_sw2_path_close_req_2 : 1;  /* bit[5]   :  */
        unsigned int  vote_mng_sw2_path_open_req_3  : 1;  /* bit[6]   :  */
        unsigned int  vote_mng_sw2_path_close_req_3 : 1;  /* bit[7]   :  */
        unsigned int  reserved                      : 24; /* bit[8-31]:  */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_CTRL1_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_0_START   (0)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_0_END     (0)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_0_START  (1)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_0_END    (1)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_1_START   (2)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_1_END     (2)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_1_START  (3)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_1_END    (3)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_2_START   (4)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_2_END     (4)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_2_START  (5)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_2_END    (5)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_3_START   (6)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_open_req_3_END     (6)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_3_START  (7)
#define SOC_SCTRL_SC_TCP_VOTE_CTRL1_vote_mng_sw2_path_close_req_3_END    (7)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_UNION
 struct description   : SC_TCP_VOTE_REQ3_ADD0 Register structure definition
                        Address Offset:0x808 Initial:0x0000 Width:32
 register description : SC_TCP_VOTE_REQ
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vote_req_3_add0           : 16; /* bit[0-15] : vote_mng3投票位 */
        unsigned int  sc_tcp_vote_req3_add0_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_vote_req_3_add0_START            (0)
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_vote_req_3_add0_END              (15)
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_sc_tcp_vote_req3_add0_msk_START  (16)
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD0_sc_tcp_vote_req3_add0_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_UNION
 struct description   : SC_TCP_VOTE_REQ3_ADD1 Register structure definition
                        Address Offset:0x80C Initial:0x0000 Width:32
 register description : SC_TCP_VOTE_REQ
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vote_req_3_add1           : 16; /* bit[0-15] : vote_mng3投票位 */
        unsigned int  sc_tcp_vote_req3_add1_msk : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_UNION;
#endif
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_vote_req_3_add1_START            (0)
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_vote_req_3_add1_END              (15)
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_sc_tcp_vote_req3_add1_msk_START  (16)
#define SOC_SCTRL_SC_TCP_VOTE_REQ3_ADD1_sc_tcp_vote_req3_add1_msk_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCSOCID0_UNION
 struct description   : SCSOCID0 Register structure definition
                        Address Offset:0xE00 Initial:0x62B0 Width:32
 register description : SOCID寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  version_code : 16; /* bit[0-15] : ASIC使用：0x1100是芯片的版本号。
                                                        FPGA使用： FPGA_XILINX ，FPGA自己维护。
                                                        emulator使用：EMULATOR_VERSION ，emu自己维护。 */
        unsigned int  chip_code    : 16; /* bit[16-31]: 0x36C0是芯片的编号。 */
    } reg;
} SOC_SCTRL_SCSOCID0_UNION;
#endif
#define SOC_SCTRL_SCSOCID0_version_code_START  (0)
#define SOC_SCTRL_SCSOCID0_version_code_END    (15)
#define SOC_SCTRL_SCSOCID0_chip_code_START     (16)
#define SOC_SCTRL_SCSOCID0_chip_code_END       (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED0_UNION
 struct description   : SCPERSTAT_POR_RESERVED0 Register structure definition
                        Address Offset:0xE10 Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED1_UNION
 struct description   : SCPERSTAT_POR_RESERVED1 Register structure definition
                        Address Offset:0xE14 Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED1_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED2_UNION
 struct description   : SCPERSTAT_POR_RESERVED2 Register structure definition
                        Address Offset:0xE18 Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SCPERSTAT_POR_RESERVED3_UNION
 struct description   : SCPERSTAT_POR_RESERVED3 Register structure definition
                        Address Offset:0xE1C Initial:0x00000000 Width:32
 register description : 外设状态非复位保留寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 非下电寄存器，保留给软件使用。 */
    } reg;
} SOC_SCTRL_SCPERSTAT_POR_RESERVED3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_NPU_LPCTRL0_SEC_UNION
 struct description   : SC_NPU_LPCTRL0_SEC Register structure definition
                        Address Offset:0xE20 Initial:0x00000000 Width:32
 register description : 低功耗数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpdata : 32; /* bit[0-31]: 给软件预留，用于低功耗数据的缓存。 */
    } reg;
} SOC_SCTRL_SC_NPU_LPCTRL0_SEC_UNION;
#endif
#define SOC_SCTRL_SC_NPU_LPCTRL0_SEC_lpdata_START  (0)
#define SOC_SCTRL_SC_NPU_LPCTRL0_SEC_lpdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_NPU_LPCTRL1_SEC_UNION
 struct description   : SC_NPU_LPCTRL1_SEC Register structure definition
                        Address Offset:0xE24 Initial:0x00000000 Width:32
 register description : 低功耗数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpdata : 32; /* bit[0-31]: 给软件预留，用于低功耗数据的缓存。 */
    } reg;
} SOC_SCTRL_SC_NPU_LPCTRL1_SEC_UNION;
#endif
#define SOC_SCTRL_SC_NPU_LPCTRL1_SEC_lpdata_START  (0)
#define SOC_SCTRL_SC_NPU_LPCTRL1_SEC_lpdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_NPU_LPCTRL2_SEC_UNION
 struct description   : SC_NPU_LPCTRL2_SEC Register structure definition
                        Address Offset:0xE28 Initial:0x00000000 Width:32
 register description : 低功耗数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpdata : 32; /* bit[0-31]: 给软件预留，用于低功耗数据的缓存。 */
    } reg;
} SOC_SCTRL_SC_NPU_LPCTRL2_SEC_UNION;
#endif
#define SOC_SCTRL_SC_NPU_LPCTRL2_SEC_lpdata_START  (0)
#define SOC_SCTRL_SC_NPU_LPCTRL2_SEC_lpdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_NPU_LPCTRL3_SEC_UNION
 struct description   : SC_NPU_LPCTRL3_SEC Register structure definition
                        Address Offset:0xE2C Initial:0x00000000 Width:32
 register description : 低功耗数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpdata : 32; /* bit[0-31]: 给软件预留，用于低功耗数据的缓存。 */
    } reg;
} SOC_SCTRL_SC_NPU_LPCTRL3_SEC_UNION;
#endif
#define SOC_SCTRL_SC_NPU_LPCTRL3_SEC_lpdata_START  (0)
#define SOC_SCTRL_SC_NPU_LPCTRL3_SEC_lpdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_NPU_LPCTRL4_SEC_UNION
 struct description   : SC_NPU_LPCTRL4_SEC Register structure definition
                        Address Offset:0xE30 Initial:0x00000000 Width:32
 register description : 低功耗数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lpdata : 32; /* bit[0-31]: 给软件预留，用于低功耗数据的缓存。 */
    } reg;
} SOC_SCTRL_SC_NPU_LPCTRL4_SEC_UNION;
#endif
#define SOC_SCTRL_SC_NPU_LPCTRL4_SEC_lpdata_START  (0)
#define SOC_SCTRL_SC_NPU_LPCTRL4_SEC_lpdata_END    (31)


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TRACKING_LOG0_UNION
 struct description   : SC_TRACKING_LOG0 Register structure definition
                        Address Offset:0xE40 Initial:0x00000000 Width:32
 register description : 轨迹记录
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 专用，请勿分配 */
    } reg;
} SOC_SCTRL_SC_TRACKING_LOG0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TRACKING_LOG1_UNION
 struct description   : SC_TRACKING_LOG1 Register structure definition
                        Address Offset:0xE44 Initial:0x00000000 Width:32
 register description : 轨迹记录
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 专用，请勿分配 */
    } reg;
} SOC_SCTRL_SC_TRACKING_LOG1_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TRACKING_LOG2_UNION
 struct description   : SC_TRACKING_LOG2 Register structure definition
                        Address Offset:0xE48 Initial:0x00000000 Width:32
 register description : 轨迹记录
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 专用，请勿分配 */
    } reg;
} SOC_SCTRL_SC_TRACKING_LOG2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_TRACKING_LOG3_UNION
 struct description   : SC_TRACKING_LOG3 Register structure definition
                        Address Offset:0xE4C Initial:0x00000000 Width:32
 register description : 轨迹记录
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 专用，请勿分配 */
    } reg;
} SOC_SCTRL_SC_TRACKING_LOG3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_SCTRL_SC_EMU_VERSION_UNION
 struct description   : SC_EMU_VERSION Register structure definition
                        Address Offset:0xE34 Initial:0x00000000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_emu_version : 32; /* bit[0-31]: EMU版本号专用 */
    } reg;
} SOC_SCTRL_SC_EMU_VERSION_UNION;
#endif
#define SOC_SCTRL_SC_EMU_VERSION_sc_emu_version_START  (0)
#define SOC_SCTRL_SC_EMU_VERSION_sc_emu_version_END    (31)






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

#endif /* end of soc_sctrl_interface.h */
