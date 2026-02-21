

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_AO_CRG_INTERFACE_H__
#define __SOC_AO_CRG_INTERFACE_H__

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


/* Register description: 系统状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCSYSSTAT_UNION */
#define SOC_AO_CRG_SCSYSSTAT_ADDR(base)                       ((base) + 0x004UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_GT_CFG_UNION */
#define SOC_AO_CRG_SC_ULPPLL_GT_CFG_ADDR(base)                ((base) + 0x088UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_ADDR(base)              ((base) + 0x08CUL)

/* Register description: PLL状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_STAT_UNION */
#define SOC_AO_CRG_SC_PLL_STAT_ADDR(base)                     ((base) + 0x0CCUL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_UNION */
#define SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_ADDR(base)          ((base) + 0x0D0UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_EN_VOTE_1_UNION */
#define SOC_AO_CRG_SC_PLL_EN_VOTE_1_ADDR(base)                ((base) + 0x0D4UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_EN_VOTE_3_UNION */
#define SOC_AO_CRG_SC_PLL_EN_VOTE_3_ADDR(base)                ((base) + 0x0DCUL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_GT_VOTE_1_UNION */
#define SOC_AO_CRG_SC_PLL_GT_VOTE_1_ADDR(base)                ((base) + 0x0E0UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_GT_VOTE_3_UNION */
#define SOC_AO_CRG_SC_PLL_GT_VOTE_3_ADDR(base)                ((base) + 0x0E4UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_BP_VOTE_1_UNION */
#define SOC_AO_CRG_SC_PLL_BP_VOTE_1_ADDR(base)                ((base) + 0x0E8UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_BP_VOTE_3_UNION */
#define SOC_AO_CRG_SC_PLL_BP_VOTE_3_ADDR(base)                ((base) + 0x0ECUL)

/* Register description: ULPPLL_配置0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ADDR(base)                  ((base) + 0x0F0UL)

/* Register description: ULPPLL_配置1
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG1_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG1_ADDR(base)                  ((base) + 0x0F4UL)

/* Register description: ULPPLL_配置2
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG2_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG2_ADDR(base)                  ((base) + 0x0F8UL)

/* Register description: ULPPLL_配置3
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG3_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG3_ADDR(base)                  ((base) + 0x0FCUL)

/* Register description: ULPPLL_配置4
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG4_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG4_ADDR(base)                  ((base) + 0x100UL)

/* Register description: ULPPLL_配置5
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG5_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG5_ADDR(base)                  ((base) + 0x104UL)

/* Register description: ULPPLL_配置6
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG6_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG6_ADDR(base)                  ((base) + 0x108UL)

/* Register description: ULPPLL_配置7
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG7_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG7_ADDR(base)                  ((base) + 0x10CUL)

/* Register description: ULPPLL_配置8
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG8_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG8_ADDR(base)                  ((base) + 0x110UL)

/* Register description: ULPPLL_控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CTRL0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ADDR(base)                 ((base) + 0x11CUL)

/* Register description: ULPPLL_状态0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_STATE0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_STATE0_ADDR(base)                ((base) + 0x120UL)

/* Register description: ULPPLL_1_配置0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CFG0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ADDR(base)                ((base) + 0x140UL)

/* Register description: ULPPLL_1_配置1
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CFG1_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_ADDR(base)                ((base) + 0x144UL)

/* Register description: ULPPLL_1_配置2
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CFG2_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CFG2_ADDR(base)                ((base) + 0x148UL)

/* Register description: ULPPLL_1_控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CTRL0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ADDR(base)               ((base) + 0x14CUL)

/* Register description: ULPPLL_1_状态0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_STATE0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_STATE0_ADDR(base)              ((base) + 0x150UL)

/* Register description: 外设时钟使能寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN0_UNION */
#define SOC_AO_CRG_SCPEREN0_ADDR(base)                        ((base) + 0x160UL)

/* Register description: 外设时钟禁止寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS0_UNION */
#define SOC_AO_CRG_SCPERDIS0_ADDR(base)                       ((base) + 0x164UL)

/* Register description: 外设时钟使能状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN0_UNION */
#define SOC_AO_CRG_SCPERCLKEN0_ADDR(base)                     ((base) + 0x168UL)

/* Register description: 外设时钟最终状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT0_UNION */
#define SOC_AO_CRG_SCPERSTAT0_ADDR(base)                      ((base) + 0x16CUL)

/* Register description: 外设时钟使能寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN1_UNION */
#define SOC_AO_CRG_SCPEREN1_ADDR(base)                        ((base) + 0x170UL)

/* Register description: 外设时钟禁止寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS1_UNION */
#define SOC_AO_CRG_SCPERDIS1_ADDR(base)                       ((base) + 0x174UL)

/* Register description: 外设时钟使能状态寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN1_UNION */
#define SOC_AO_CRG_SCPERCLKEN1_ADDR(base)                     ((base) + 0x178UL)

/* Register description: 外设时钟最终状态寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT1_UNION */
#define SOC_AO_CRG_SCPERSTAT1_ADDR(base)                      ((base) + 0x17CUL)

/* Register description: AO_CRG状态
   Bit domain definition UNION:  SOC_AO_CRG_SC_AO_CRG_STA_SEP0_UNION */
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_ADDR(base)              ((base) + 0x188UL)

/* Register description: 自动降频状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_UNION */
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_ADDR(base)           ((base) + 0x18CUL)

/* Register description: 外设时钟使能寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN2_UNION */
#define SOC_AO_CRG_SCPEREN2_ADDR(base)                        ((base) + 0x190UL)

/* Register description: 外设时钟禁止寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS2_UNION */
#define SOC_AO_CRG_SCPERDIS2_ADDR(base)                       ((base) + 0x194UL)

/* Register description: 外设时钟使能状态寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN2_UNION */
#define SOC_AO_CRG_SCPERCLKEN2_ADDR(base)                     ((base) + 0x198UL)

/* Register description: 外设时钟最终状态寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT2_UNION */
#define SOC_AO_CRG_SCPERSTAT2_ADDR(base)                      ((base) + 0x19CUL)

/* Register description: 外设时钟使能寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN3_UNION */
#define SOC_AO_CRG_SCPEREN3_ADDR(base)                        ((base) + 0x1A0UL)

/* Register description: 外设时钟禁止寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS3_UNION */
#define SOC_AO_CRG_SCPERDIS3_ADDR(base)                       ((base) + 0x1A4UL)

/* Register description: 外设时钟使能状态寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN3_UNION */
#define SOC_AO_CRG_SCPERCLKEN3_ADDR(base)                     ((base) + 0x1A8UL)

/* Register description: 外设时钟最终状态寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT3_UNION */
#define SOC_AO_CRG_SCPERSTAT3_ADDR(base)                      ((base) + 0x1ACUL)

/* Register description: 外设时钟使能寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN4_UNION */
#define SOC_AO_CRG_SCPEREN4_ADDR(base)                        ((base) + 0x1B0UL)

/* Register description: 外设时钟禁止寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS4_UNION */
#define SOC_AO_CRG_SCPERDIS4_ADDR(base)                       ((base) + 0x1B4UL)

/* Register description: 外设时钟使能状态寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN4_UNION */
#define SOC_AO_CRG_SCPERCLKEN4_ADDR(base)                     ((base) + 0x1B8UL)

/* Register description: 外设时钟最终状态寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT4_UNION */
#define SOC_AO_CRG_SCPERSTAT4_ADDR(base)                      ((base) + 0x1BCUL)

/* Register description: 外设时钟使能寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN5_UNION */
#define SOC_AO_CRG_SCPEREN5_ADDR(base)                        ((base) + 0x1C0UL)

/* Register description: 外设时钟禁止寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS5_UNION */
#define SOC_AO_CRG_SCPERDIS5_ADDR(base)                       ((base) + 0x1C4UL)

/* Register description: 外设时钟使能状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN5_UNION */
#define SOC_AO_CRG_SCPERCLKEN5_ADDR(base)                     ((base) + 0x1C8UL)

/* Register description: 外设时钟最终状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT5_UNION */
#define SOC_AO_CRG_SCPERSTAT5_ADDR(base)                      ((base) + 0x1CCUL)

/* Register description: 外设时钟使能寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN6_UNION */
#define SOC_AO_CRG_SCPEREN6_ADDR(base)                        ((base) + 0x1D0UL)

/* Register description: 外设时钟禁止寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS6_UNION */
#define SOC_AO_CRG_SCPERDIS6_ADDR(base)                       ((base) + 0x1D4UL)

/* Register description: 外设时钟使能状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN6_UNION */
#define SOC_AO_CRG_SCPERCLKEN6_ADDR(base)                     ((base) + 0x1D8UL)

/* Register description: 外设时钟最终状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT6_UNION */
#define SOC_AO_CRG_SCPERSTAT6_ADDR(base)                      ((base) + 0x1DCUL)

/* Register description: fnpll状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_STATE0_UNION */
#define SOC_AO_CRG_FNPLL_STATE0_ADDR(base)                    ((base) + 0x1E8UL)

/* Register description: 外设软复位使能寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN0_UNION */
#define SOC_AO_CRG_SCPERRSTEN0_ADDR(base)                     ((base) + 0x200UL)

/* Register description: 外设软复位撤离寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS0_UNION */
#define SOC_AO_CRG_SCPERRSTDIS0_ADDR(base)                    ((base) + 0x204UL)

/* Register description: 外设软复位状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT0_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT0_ADDR(base)                   ((base) + 0x208UL)

/* Register description: 外设软复位使能寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN1_UNION */
#define SOC_AO_CRG_SCPERRSTEN1_ADDR(base)                     ((base) + 0x20CUL)

/* Register description: 外设软复位撤离寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS1_UNION */
#define SOC_AO_CRG_SCPERRSTDIS1_ADDR(base)                    ((base) + 0x210UL)

/* Register description: 外设软复位状态寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT1_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT1_ADDR(base)                   ((base) + 0x214UL)

/* Register description: 外设软复位使能寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN2_UNION */
#define SOC_AO_CRG_SCPERRSTEN2_ADDR(base)                     ((base) + 0x218UL)

/* Register description: 外设软复位撤离寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS2_UNION */
#define SOC_AO_CRG_SCPERRSTDIS2_ADDR(base)                    ((base) + 0x21CUL)

/* Register description: 外设软复位状态寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT2_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT2_ADDR(base)                   ((base) + 0x220UL)

/* Register description: 时钟分频比控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV0_UNION */
#define SOC_AO_CRG_SCCLKDIV0_ADDR(base)                       ((base) + 0x250UL)

/* Register description: 时钟分频比控制寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV1_UNION */
#define SOC_AO_CRG_SCCLKDIV1_ADDR(base)                       ((base) + 0x254UL)

/* Register description: 时钟分频比控制寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV2_UNION */
#define SOC_AO_CRG_SCCLKDIV2_ADDR(base)                       ((base) + 0x258UL)

/* Register description: 时钟分频比控制寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV3_UNION */
#define SOC_AO_CRG_SCCLKDIV3_ADDR(base)                       ((base) + 0x25CUL)

/* Register description: 时钟分频比控制寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV4_UNION */
#define SOC_AO_CRG_SCCLKDIV4_ADDR(base)                       ((base) + 0x260UL)

/* Register description: 时钟分频比控制寄存器5
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV5_UNION */
#define SOC_AO_CRG_SCCLKDIV5_ADDR(base)                       ((base) + 0x264UL)

/* Register description: 时钟分频比控制寄存器6
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV6_UNION */
#define SOC_AO_CRG_SCCLKDIV6_ADDR(base)                       ((base) + 0x268UL)

/* Register description: 时钟分频比控制寄存器7
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV7_UNION */
#define SOC_AO_CRG_SCCLKDIV7_ADDR(base)                       ((base) + 0x26CUL)

/* Register description: 时钟分频比控制寄存器8
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV8_UNION */
#define SOC_AO_CRG_SCCLKDIV8_ADDR(base)                       ((base) + 0x270UL)

/* Register description: 时钟分频比控制寄存器8
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV9_UNION */
#define SOC_AO_CRG_SCCLKDIV9_ADDR(base)                       ((base) + 0x274UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_UNION */
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_ADDR(base)             ((base) + 0x27CUL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV10_UNION */
#define SOC_AO_CRG_SCCLKDIV10_ADDR(base)                      ((base) + 0x280UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV11_UNION */
#define SOC_AO_CRG_SCCLKDIV11_ADDR(base)                      ((base) + 0x284UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV12_UNION */
#define SOC_AO_CRG_SCCLKDIV12_ADDR(base)                      ((base) + 0x288UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV13_UNION */
#define SOC_AO_CRG_SCCLKDIV13_ADDR(base)                      ((base) + 0x28CUL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV14_UNION */
#define SOC_AO_CRG_SCCLKDIV14_ADDR(base)                      ((base) + 0x290UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV15_UNION */
#define SOC_AO_CRG_SCCLKDIV15_ADDR(base)                      ((base) + 0x294UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV16_UNION */
#define SOC_AO_CRG_SCCLKDIV16_ADDR(base)                      ((base) + 0x298UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV17_UNION */
#define SOC_AO_CRG_SCCLKDIV17_ADDR(base)                      ((base) + 0x29CUL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV18_UNION */
#define SOC_AO_CRG_SCCLKDIV18_ADDR(base)                      ((base) + 0x2A0UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV19_UNION */
#define SOC_AO_CRG_SCCLKDIV19_ADDR(base)                      ((base) + 0x2A4UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV20_UNION */
#define SOC_AO_CRG_SCCLKDIV20_ADDR(base)                      ((base) + 0x2A8UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV21_UNION */
#define SOC_AO_CRG_SCCLKDIV21_ADDR(base)                      ((base) + 0x2ACUL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV22_UNION */
#define SOC_AO_CRG_SCCLKDIV22_ADDR(base)                      ((base) + 0x2B0UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV23_UNION */
#define SOC_AO_CRG_SCCLKDIV23_ADDR(base)                      ((base) + 0x2B8UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV24_UNION */
#define SOC_AO_CRG_SCCLKDIV24_ADDR(base)                      ((base) + 0x2BCUL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_UNION */
#define SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_ADDR(base)             ((base) + 0x2B4UL)

/* Register description: 外设状态寄存器18
   Bit domain definition UNION:  SOC_AO_CRG_SC_INTR_NOCBUS_STAT_UNION */
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_ADDR(base)             ((base) + 0x354UL)

/* Register description: 外设状态寄存器17
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTATUS17_UNION */
#define SOC_AO_CRG_SCPERSTATUS17_ADDR(base)                   ((base) + 0x358UL)

/* Register description: 外设状态寄存器16
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTATUS16_UNION */
#define SOC_AO_CRG_SCPERSTATUS16_ADDR(base)                   ((base) + 0x35CUL)

/* Register description: 外设状态寄存器18
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTATUS18_UNION */
#define SOC_AO_CRG_SCPERSTATUS18_ADDR(base)                   ((base) + 0x360UL)

/* Register description: SPLL控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCAUPLLCTRL0_UNION */
#define SOC_AO_CRG_SCAUPLLCTRL0_ADDR(base)                    ((base) + 0x490UL)

/* Register description: SPLL控制寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCAUPLLCTRL1_UNION */
#define SOC_AO_CRG_SCAUPLLCTRL1_ADDR(base)                    ((base) + 0x494UL)

/* Register description: SPLL SSC控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCAUPLLSSCCTRL_UNION */
#define SOC_AO_CRG_SCAUPLLSSCCTRL_ADDR(base)                  ((base) + 0x498UL)

/* Register description: fnpll(dp audio 专用)配置寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG0_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG0_ADDR(base)                    ((base) + 0x49CUL)

/* Register description: fnpll(dp audio 专用)配置寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG1_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG1_ADDR(base)                    ((base) + 0x4A0UL)

/* Register description: fnpll(dp audio 专用)配置寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG2_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG2_ADDR(base)                    ((base) + 0x4A4UL)

/* Register description: fnpll(dp audio 专用)配置寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG3_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG3_ADDR(base)                    ((base) + 0x4A8UL)

/* Register description: fnpll(dp audio 专用)状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_STATE0_UNION */
#define SOC_AO_CRG_AUFNPLL_STATE0_ADDR(base)                  ((base) + 0x4B4UL)

/* Register description: 自动降频控制寄存器2。
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV2_UNION */
#define SOC_AO_CRG_AO_AUTODIV2_ADDR(base)                     ((base) + 0x500UL)

/* Register description: 外设时钟使能寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN0_SEC_UNION */
#define SOC_AO_CRG_SCPEREN0_SEC_ADDR(base)                    ((base) + 0x900UL)

/* Register description: 外设时钟禁止寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS0_SEC_UNION */
#define SOC_AO_CRG_SCPERDIS0_SEC_ADDR(base)                   ((base) + 0x904UL)

/* Register description: 外设时钟使能状态寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN0_SEC_UNION */
#define SOC_AO_CRG_SCPERCLKEN0_SEC_ADDR(base)                 ((base) + 0x908UL)

/* Register description: 外设软复位使能寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN0_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ADDR(base)                 ((base) + 0x910UL)

/* Register description: 外设软复位撤离寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS0_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ADDR(base)                ((base) + 0x914UL)

/* Register description: 外设软复位状态寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT0_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ADDR(base)               ((base) + 0x918UL)

/* Register description: 外设软复位使能寄存器1（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN1_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ADDR(base)                 ((base) + 0x990UL)

/* Register description: 外设软复位撤离寄存器1（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS1_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ADDR(base)                ((base) + 0x994UL)

/* Register description: 外设软复位状态寄存器1（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT1_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ADDR(base)               ((base) + 0x998UL)

/* Register description: 外设软复位使能寄存器2（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN2_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTEN2_SEC_ADDR(base)                 ((base) + 0x9A0UL)

/* Register description: 外设软复位撤离寄存器2（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS2_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTDIS2_SEC_ADDR(base)                ((base) + 0x9A4UL)

/* Register description: 外设软复位状态寄存器2（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT2_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT2_SEC_ADDR(base)               ((base) + 0x9A8UL)

/* Register description: 外设时钟使能寄存器3（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN3_SEC_UNION */
#define SOC_AO_CRG_SCPEREN3_SEC_ADDR(base)                    ((base) + 0x9C0UL)

/* Register description: 外设时钟禁止寄存器3（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS3_SEC_UNION */
#define SOC_AO_CRG_SCPERDIS3_SEC_ADDR(base)                   ((base) + 0x9C4UL)

/* Register description: 外设时钟使能状态寄存器3（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN3_SEC_UNION */
#define SOC_AO_CRG_SCPERCLKEN3_SEC_ADDR(base)                 ((base) + 0x9C8UL)

/* Register description: 外设软复位使能寄存器4（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN4_SEC_UNION */
#define SOC_AO_CRG_SCPEREN4_SEC_ADDR(base)                    ((base) + 0x9D0UL)

/* Register description: 外设软复位撤离寄存器4（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS4_SEC_UNION */
#define SOC_AO_CRG_SCPERDIS4_SEC_ADDR(base)                   ((base) + 0x9D4UL)

/* Register description: 外设软复位状态寄存器4（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN4_SEC_UNION */
#define SOC_AO_CRG_SCPERCLKEN4_SEC_ADDR(base)                 ((base) + 0x9D8UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV0_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV0_SEC_ADDR(base)                   ((base) + 0x9E0UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV1_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV1_SEC_ADDR(base)                   ((base) + 0x9E4UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV2_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV2_SEC_ADDR(base)                   ((base) + 0x9E8UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV3_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV3_SEC_ADDR(base)                   ((base) + 0x9ECUL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV4_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV4_SEC_ADDR(base)                   ((base) + 0x9F0UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV5_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV5_SEC_ADDR(base)                   ((base) + 0x9F4UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV6_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV6_SEC_ADDR(base)                   ((base) + 0x9F8UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV7_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV7_SEC_ADDR(base)                   ((base) + 0x9FCUL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV8_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV8_SEC_ADDR(base)                   ((base) + 0xA00UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV9_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV9_SEC_ADDR(base)                   ((base) + 0xA04UL)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV10_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV10_SEC_ADDR(base)                  ((base) + 0xA08UL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV11_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV11_SEC_ADDR(base)                  ((base) + 0xA0CUL)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV12_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV12_SEC_ADDR(base)                  ((base) + 0xA1CUL)

/* Register description: IP防总线挂死控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_UNION */
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ADDR(base)         ((base) + 0xA84UL)

/* Register description: IP防总线挂死控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_UNION */
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ADDR(base)         ((base) + 0xA88UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_CLK_STAT0_SEC_UNION */
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_ADDR(base)                ((base) + 0xA8CUL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_CLK_STAT1_SEC_UNION */
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_ADDR(base)                ((base) + 0xA90UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_PLL_FSM_CTRL0_SEC_UNION */
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_ADDR(base)               ((base) + 0xA94UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_PLL_FSM_CTRL1_SEC_UNION */
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_ADDR(base)               ((base) + 0xA98UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_PLL_FSM_STAT0_SEC_UNION */
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_ADDR(base)               ((base) + 0xA9CUL)

/* Register description: 自动降频控制寄存器0。
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV0_UNION */
#define SOC_AO_CRG_AO_AUTODIV0_ADDR(base)                     ((base) + 0xB00UL)

/* Register description: 自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV1_UNION */
#define SOC_AO_CRG_AO_AUTODIV1_ADDR(base)                     ((base) + 0xB04UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_DDRC_CTRL_UNION */
#define SOC_AO_CRG_SC_DDRC_CTRL_ADDR(base)                    ((base) + 0xE38UL)

/* Register description: SPLL控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL0_UNION */
#define SOC_AO_CRG_SCSPLLCTRL0_ADDR(base)                     ((base) + 0xF14UL)

/* Register description: SPLL控制寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL1_UNION */
#define SOC_AO_CRG_SCSPLLCTRL1_ADDR(base)                     ((base) + 0xF18UL)

/* Register description: SPLL SSC控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLSSCCTRL_UNION */
#define SOC_AO_CRG_SCSPLLSSCCTRL_ADDR(base)                   ((base) + 0xF1CUL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL0_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLLCTRL0_LOCK_STAT_ADDR(base)           ((base) + 0xF20UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL1_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLLCTRL1_LOCK_STAT_ADDR(base)           ((base) + 0xF24UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_ADDR(base)         ((base) + 0xF28UL)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_ADDR(base)          ((base) + 0xF2CUL)

/* Register description: fnpll配置寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG0_UNION */
#define SOC_AO_CRG_FNPLL_CFG0_ADDR(base)                      ((base) + 0xF30UL)

/* Register description: fnpll配置寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG1_UNION */
#define SOC_AO_CRG_FNPLL_CFG1_ADDR(base)                      ((base) + 0xF34UL)

/* Register description: fnpll配置寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG2_UNION */
#define SOC_AO_CRG_FNPLL_CFG2_ADDR(base)                      ((base) + 0xF38UL)

/* Register description: fnpll配置寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG3_UNION */
#define SOC_AO_CRG_FNPLL_CFG3_ADDR(base)                      ((base) + 0xF3CUL)

/* Register description: fnpll配置寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG0_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG0_ADDR(base)                   ((base) + 0xF40UL)

/* Register description: fnpll配置寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG1_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG1_ADDR(base)                   ((base) + 0xF44UL)

/* Register description: fnpll配置寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG2_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG2_ADDR(base)                   ((base) + 0xF48UL)

/* Register description: fnpll配置寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG3_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG3_ADDR(base)                   ((base) + 0xF4CUL)


#else


/* Register description: 系统状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCSYSSTAT_UNION */
#define SOC_AO_CRG_SCSYSSTAT_ADDR(base)                       ((base) + 0x004)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_GT_CFG_UNION */
#define SOC_AO_CRG_SC_ULPPLL_GT_CFG_ADDR(base)                ((base) + 0x088)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_ADDR(base)              ((base) + 0x08C)

/* Register description: PLL状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_STAT_UNION */
#define SOC_AO_CRG_SC_PLL_STAT_ADDR(base)                     ((base) + 0x0CC)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_UNION */
#define SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_ADDR(base)          ((base) + 0x0D0)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_EN_VOTE_1_UNION */
#define SOC_AO_CRG_SC_PLL_EN_VOTE_1_ADDR(base)                ((base) + 0x0D4)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_EN_VOTE_3_UNION */
#define SOC_AO_CRG_SC_PLL_EN_VOTE_3_ADDR(base)                ((base) + 0x0DC)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_GT_VOTE_1_UNION */
#define SOC_AO_CRG_SC_PLL_GT_VOTE_1_ADDR(base)                ((base) + 0x0E0)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_GT_VOTE_3_UNION */
#define SOC_AO_CRG_SC_PLL_GT_VOTE_3_ADDR(base)                ((base) + 0x0E4)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_BP_VOTE_1_UNION */
#define SOC_AO_CRG_SC_PLL_BP_VOTE_1_ADDR(base)                ((base) + 0x0E8)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_BP_VOTE_3_UNION */
#define SOC_AO_CRG_SC_PLL_BP_VOTE_3_ADDR(base)                ((base) + 0x0EC)

/* Register description: ULPPLL_配置0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ADDR(base)                  ((base) + 0x0F0)

/* Register description: ULPPLL_配置1
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG1_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG1_ADDR(base)                  ((base) + 0x0F4)

/* Register description: ULPPLL_配置2
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG2_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG2_ADDR(base)                  ((base) + 0x0F8)

/* Register description: ULPPLL_配置3
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG3_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG3_ADDR(base)                  ((base) + 0x0FC)

/* Register description: ULPPLL_配置4
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG4_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG4_ADDR(base)                  ((base) + 0x100)

/* Register description: ULPPLL_配置5
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG5_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG5_ADDR(base)                  ((base) + 0x104)

/* Register description: ULPPLL_配置6
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG6_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG6_ADDR(base)                  ((base) + 0x108)

/* Register description: ULPPLL_配置7
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG7_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG7_ADDR(base)                  ((base) + 0x10C)

/* Register description: ULPPLL_配置8
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CFG8_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CFG8_ADDR(base)                  ((base) + 0x110)

/* Register description: ULPPLL_控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_CTRL0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ADDR(base)                 ((base) + 0x11C)

/* Register description: ULPPLL_状态0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_STATE0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_STATE0_ADDR(base)                ((base) + 0x120)

/* Register description: ULPPLL_1_配置0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CFG0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ADDR(base)                ((base) + 0x140)

/* Register description: ULPPLL_1_配置1
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CFG1_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_ADDR(base)                ((base) + 0x144)

/* Register description: ULPPLL_1_配置2
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CFG2_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CFG2_ADDR(base)                ((base) + 0x148)

/* Register description: ULPPLL_1_控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_CTRL0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ADDR(base)               ((base) + 0x14C)

/* Register description: ULPPLL_1_状态0
   Bit domain definition UNION:  SOC_AO_CRG_SC_ULPPLL_1_STATE0_UNION */
#define SOC_AO_CRG_SC_ULPPLL_1_STATE0_ADDR(base)              ((base) + 0x150)

/* Register description: 外设时钟使能寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN0_UNION */
#define SOC_AO_CRG_SCPEREN0_ADDR(base)                        ((base) + 0x160)

/* Register description: 外设时钟禁止寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS0_UNION */
#define SOC_AO_CRG_SCPERDIS0_ADDR(base)                       ((base) + 0x164)

/* Register description: 外设时钟使能状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN0_UNION */
#define SOC_AO_CRG_SCPERCLKEN0_ADDR(base)                     ((base) + 0x168)

/* Register description: 外设时钟最终状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT0_UNION */
#define SOC_AO_CRG_SCPERSTAT0_ADDR(base)                      ((base) + 0x16C)

/* Register description: 外设时钟使能寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN1_UNION */
#define SOC_AO_CRG_SCPEREN1_ADDR(base)                        ((base) + 0x170)

/* Register description: 外设时钟禁止寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS1_UNION */
#define SOC_AO_CRG_SCPERDIS1_ADDR(base)                       ((base) + 0x174)

/* Register description: 外设时钟使能状态寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN1_UNION */
#define SOC_AO_CRG_SCPERCLKEN1_ADDR(base)                     ((base) + 0x178)

/* Register description: 外设时钟最终状态寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT1_UNION */
#define SOC_AO_CRG_SCPERSTAT1_ADDR(base)                      ((base) + 0x17C)

/* Register description: AO_CRG状态
   Bit domain definition UNION:  SOC_AO_CRG_SC_AO_CRG_STA_SEP0_UNION */
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_ADDR(base)              ((base) + 0x188)

/* Register description: 自动降频状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_UNION */
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_ADDR(base)           ((base) + 0x18C)

/* Register description: 外设时钟使能寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN2_UNION */
#define SOC_AO_CRG_SCPEREN2_ADDR(base)                        ((base) + 0x190)

/* Register description: 外设时钟禁止寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS2_UNION */
#define SOC_AO_CRG_SCPERDIS2_ADDR(base)                       ((base) + 0x194)

/* Register description: 外设时钟使能状态寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN2_UNION */
#define SOC_AO_CRG_SCPERCLKEN2_ADDR(base)                     ((base) + 0x198)

/* Register description: 外设时钟最终状态寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT2_UNION */
#define SOC_AO_CRG_SCPERSTAT2_ADDR(base)                      ((base) + 0x19C)

/* Register description: 外设时钟使能寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN3_UNION */
#define SOC_AO_CRG_SCPEREN3_ADDR(base)                        ((base) + 0x1A0)

/* Register description: 外设时钟禁止寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS3_UNION */
#define SOC_AO_CRG_SCPERDIS3_ADDR(base)                       ((base) + 0x1A4)

/* Register description: 外设时钟使能状态寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN3_UNION */
#define SOC_AO_CRG_SCPERCLKEN3_ADDR(base)                     ((base) + 0x1A8)

/* Register description: 外设时钟最终状态寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT3_UNION */
#define SOC_AO_CRG_SCPERSTAT3_ADDR(base)                      ((base) + 0x1AC)

/* Register description: 外设时钟使能寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN4_UNION */
#define SOC_AO_CRG_SCPEREN4_ADDR(base)                        ((base) + 0x1B0)

/* Register description: 外设时钟禁止寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS4_UNION */
#define SOC_AO_CRG_SCPERDIS4_ADDR(base)                       ((base) + 0x1B4)

/* Register description: 外设时钟使能状态寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN4_UNION */
#define SOC_AO_CRG_SCPERCLKEN4_ADDR(base)                     ((base) + 0x1B8)

/* Register description: 外设时钟最终状态寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT4_UNION */
#define SOC_AO_CRG_SCPERSTAT4_ADDR(base)                      ((base) + 0x1BC)

/* Register description: 外设时钟使能寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN5_UNION */
#define SOC_AO_CRG_SCPEREN5_ADDR(base)                        ((base) + 0x1C0)

/* Register description: 外设时钟禁止寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS5_UNION */
#define SOC_AO_CRG_SCPERDIS5_ADDR(base)                       ((base) + 0x1C4)

/* Register description: 外设时钟使能状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN5_UNION */
#define SOC_AO_CRG_SCPERCLKEN5_ADDR(base)                     ((base) + 0x1C8)

/* Register description: 外设时钟最终状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT5_UNION */
#define SOC_AO_CRG_SCPERSTAT5_ADDR(base)                      ((base) + 0x1CC)

/* Register description: 外设时钟使能寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN6_UNION */
#define SOC_AO_CRG_SCPEREN6_ADDR(base)                        ((base) + 0x1D0)

/* Register description: 外设时钟禁止寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS6_UNION */
#define SOC_AO_CRG_SCPERDIS6_ADDR(base)                       ((base) + 0x1D4)

/* Register description: 外设时钟使能状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN6_UNION */
#define SOC_AO_CRG_SCPERCLKEN6_ADDR(base)                     ((base) + 0x1D8)

/* Register description: 外设时钟最终状态寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTAT6_UNION */
#define SOC_AO_CRG_SCPERSTAT6_ADDR(base)                      ((base) + 0x1DC)

/* Register description: fnpll状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_STATE0_UNION */
#define SOC_AO_CRG_FNPLL_STATE0_ADDR(base)                    ((base) + 0x1E8)

/* Register description: 外设软复位使能寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN0_UNION */
#define SOC_AO_CRG_SCPERRSTEN0_ADDR(base)                     ((base) + 0x200)

/* Register description: 外设软复位撤离寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS0_UNION */
#define SOC_AO_CRG_SCPERRSTDIS0_ADDR(base)                    ((base) + 0x204)

/* Register description: 外设软复位状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT0_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT0_ADDR(base)                   ((base) + 0x208)

/* Register description: 外设软复位使能寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN1_UNION */
#define SOC_AO_CRG_SCPERRSTEN1_ADDR(base)                     ((base) + 0x20C)

/* Register description: 外设软复位撤离寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS1_UNION */
#define SOC_AO_CRG_SCPERRSTDIS1_ADDR(base)                    ((base) + 0x210)

/* Register description: 外设软复位状态寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT1_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT1_ADDR(base)                   ((base) + 0x214)

/* Register description: 外设软复位使能寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN2_UNION */
#define SOC_AO_CRG_SCPERRSTEN2_ADDR(base)                     ((base) + 0x218)

/* Register description: 外设软复位撤离寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS2_UNION */
#define SOC_AO_CRG_SCPERRSTDIS2_ADDR(base)                    ((base) + 0x21C)

/* Register description: 外设软复位状态寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT2_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT2_ADDR(base)                   ((base) + 0x220)

/* Register description: 时钟分频比控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV0_UNION */
#define SOC_AO_CRG_SCCLKDIV0_ADDR(base)                       ((base) + 0x250)

/* Register description: 时钟分频比控制寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV1_UNION */
#define SOC_AO_CRG_SCCLKDIV1_ADDR(base)                       ((base) + 0x254)

/* Register description: 时钟分频比控制寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV2_UNION */
#define SOC_AO_CRG_SCCLKDIV2_ADDR(base)                       ((base) + 0x258)

/* Register description: 时钟分频比控制寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV3_UNION */
#define SOC_AO_CRG_SCCLKDIV3_ADDR(base)                       ((base) + 0x25C)

/* Register description: 时钟分频比控制寄存器4
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV4_UNION */
#define SOC_AO_CRG_SCCLKDIV4_ADDR(base)                       ((base) + 0x260)

/* Register description: 时钟分频比控制寄存器5
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV5_UNION */
#define SOC_AO_CRG_SCCLKDIV5_ADDR(base)                       ((base) + 0x264)

/* Register description: 时钟分频比控制寄存器6
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV6_UNION */
#define SOC_AO_CRG_SCCLKDIV6_ADDR(base)                       ((base) + 0x268)

/* Register description: 时钟分频比控制寄存器7
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV7_UNION */
#define SOC_AO_CRG_SCCLKDIV7_ADDR(base)                       ((base) + 0x26C)

/* Register description: 时钟分频比控制寄存器8
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV8_UNION */
#define SOC_AO_CRG_SCCLKDIV8_ADDR(base)                       ((base) + 0x270)

/* Register description: 时钟分频比控制寄存器8
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV9_UNION */
#define SOC_AO_CRG_SCCLKDIV9_ADDR(base)                       ((base) + 0x274)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_UNION */
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_ADDR(base)             ((base) + 0x27C)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV10_UNION */
#define SOC_AO_CRG_SCCLKDIV10_ADDR(base)                      ((base) + 0x280)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV11_UNION */
#define SOC_AO_CRG_SCCLKDIV11_ADDR(base)                      ((base) + 0x284)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV12_UNION */
#define SOC_AO_CRG_SCCLKDIV12_ADDR(base)                      ((base) + 0x288)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV13_UNION */
#define SOC_AO_CRG_SCCLKDIV13_ADDR(base)                      ((base) + 0x28C)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV14_UNION */
#define SOC_AO_CRG_SCCLKDIV14_ADDR(base)                      ((base) + 0x290)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV15_UNION */
#define SOC_AO_CRG_SCCLKDIV15_ADDR(base)                      ((base) + 0x294)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV16_UNION */
#define SOC_AO_CRG_SCCLKDIV16_ADDR(base)                      ((base) + 0x298)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV17_UNION */
#define SOC_AO_CRG_SCCLKDIV17_ADDR(base)                      ((base) + 0x29C)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV18_UNION */
#define SOC_AO_CRG_SCCLKDIV18_ADDR(base)                      ((base) + 0x2A0)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV19_UNION */
#define SOC_AO_CRG_SCCLKDIV19_ADDR(base)                      ((base) + 0x2A4)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV20_UNION */
#define SOC_AO_CRG_SCCLKDIV20_ADDR(base)                      ((base) + 0x2A8)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV21_UNION */
#define SOC_AO_CRG_SCCLKDIV21_ADDR(base)                      ((base) + 0x2AC)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV22_UNION */
#define SOC_AO_CRG_SCCLKDIV22_ADDR(base)                      ((base) + 0x2B0)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV23_UNION */
#define SOC_AO_CRG_SCCLKDIV23_ADDR(base)                      ((base) + 0x2B8)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV24_UNION */
#define SOC_AO_CRG_SCCLKDIV24_ADDR(base)                      ((base) + 0x2BC)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_UNION */
#define SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_ADDR(base)             ((base) + 0x2B4)

/* Register description: 外设状态寄存器18
   Bit domain definition UNION:  SOC_AO_CRG_SC_INTR_NOCBUS_STAT_UNION */
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_ADDR(base)             ((base) + 0x354)

/* Register description: 外设状态寄存器17
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTATUS17_UNION */
#define SOC_AO_CRG_SCPERSTATUS17_ADDR(base)                   ((base) + 0x358)

/* Register description: 外设状态寄存器16
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTATUS16_UNION */
#define SOC_AO_CRG_SCPERSTATUS16_ADDR(base)                   ((base) + 0x35C)

/* Register description: 外设状态寄存器18
   Bit domain definition UNION:  SOC_AO_CRG_SCPERSTATUS18_UNION */
#define SOC_AO_CRG_SCPERSTATUS18_ADDR(base)                   ((base) + 0x360)

/* Register description: SPLL控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCAUPLLCTRL0_UNION */
#define SOC_AO_CRG_SCAUPLLCTRL0_ADDR(base)                    ((base) + 0x490)

/* Register description: SPLL控制寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCAUPLLCTRL1_UNION */
#define SOC_AO_CRG_SCAUPLLCTRL1_ADDR(base)                    ((base) + 0x494)

/* Register description: SPLL SSC控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCAUPLLSSCCTRL_UNION */
#define SOC_AO_CRG_SCAUPLLSSCCTRL_ADDR(base)                  ((base) + 0x498)

/* Register description: fnpll(dp audio 专用)配置寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG0_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG0_ADDR(base)                    ((base) + 0x49C)

/* Register description: fnpll(dp audio 专用)配置寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG1_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG1_ADDR(base)                    ((base) + 0x4A0)

/* Register description: fnpll(dp audio 专用)配置寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG2_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG2_ADDR(base)                    ((base) + 0x4A4)

/* Register description: fnpll(dp audio 专用)配置寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_CFG3_UNION */
#define SOC_AO_CRG_AUFNPLL_CFG3_ADDR(base)                    ((base) + 0x4A8)

/* Register description: fnpll(dp audio 专用)状态寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_AUFNPLL_STATE0_UNION */
#define SOC_AO_CRG_AUFNPLL_STATE0_ADDR(base)                  ((base) + 0x4B4)

/* Register description: 自动降频控制寄存器2。
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV2_UNION */
#define SOC_AO_CRG_AO_AUTODIV2_ADDR(base)                     ((base) + 0x500)

/* Register description: 外设时钟使能寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN0_SEC_UNION */
#define SOC_AO_CRG_SCPEREN0_SEC_ADDR(base)                    ((base) + 0x900)

/* Register description: 外设时钟禁止寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS0_SEC_UNION */
#define SOC_AO_CRG_SCPERDIS0_SEC_ADDR(base)                   ((base) + 0x904)

/* Register description: 外设时钟使能状态寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN0_SEC_UNION */
#define SOC_AO_CRG_SCPERCLKEN0_SEC_ADDR(base)                 ((base) + 0x908)

/* Register description: 外设软复位使能寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN0_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ADDR(base)                 ((base) + 0x910)

/* Register description: 外设软复位撤离寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS0_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ADDR(base)                ((base) + 0x914)

/* Register description: 外设软复位状态寄存器0（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT0_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ADDR(base)               ((base) + 0x918)

/* Register description: 外设软复位使能寄存器1（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN1_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ADDR(base)                 ((base) + 0x990)

/* Register description: 外设软复位撤离寄存器1（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS1_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ADDR(base)                ((base) + 0x994)

/* Register description: 外设软复位状态寄存器1（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT1_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ADDR(base)               ((base) + 0x998)

/* Register description: 外设软复位使能寄存器2（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTEN2_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTEN2_SEC_ADDR(base)                 ((base) + 0x9A0)

/* Register description: 外设软复位撤离寄存器2（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTDIS2_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTDIS2_SEC_ADDR(base)                ((base) + 0x9A4)

/* Register description: 外设软复位状态寄存器2（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERRSTSTAT2_SEC_UNION */
#define SOC_AO_CRG_SCPERRSTSTAT2_SEC_ADDR(base)               ((base) + 0x9A8)

/* Register description: 外设时钟使能寄存器3（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN3_SEC_UNION */
#define SOC_AO_CRG_SCPEREN3_SEC_ADDR(base)                    ((base) + 0x9C0)

/* Register description: 外设时钟禁止寄存器3（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS3_SEC_UNION */
#define SOC_AO_CRG_SCPERDIS3_SEC_ADDR(base)                   ((base) + 0x9C4)

/* Register description: 外设时钟使能状态寄存器3（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN3_SEC_UNION */
#define SOC_AO_CRG_SCPERCLKEN3_SEC_ADDR(base)                 ((base) + 0x9C8)

/* Register description: 外设软复位使能寄存器4（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPEREN4_SEC_UNION */
#define SOC_AO_CRG_SCPEREN4_SEC_ADDR(base)                    ((base) + 0x9D0)

/* Register description: 外设软复位撤离寄存器4（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERDIS4_SEC_UNION */
#define SOC_AO_CRG_SCPERDIS4_SEC_ADDR(base)                   ((base) + 0x9D4)

/* Register description: 外设软复位状态寄存器4（安全）
   Bit domain definition UNION:  SOC_AO_CRG_SCPERCLKEN4_SEC_UNION */
#define SOC_AO_CRG_SCPERCLKEN4_SEC_ADDR(base)                 ((base) + 0x9D8)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV0_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV0_SEC_ADDR(base)                   ((base) + 0x9E0)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV1_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV1_SEC_ADDR(base)                   ((base) + 0x9E4)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV2_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV2_SEC_ADDR(base)                   ((base) + 0x9E8)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV3_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV3_SEC_ADDR(base)                   ((base) + 0x9EC)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV4_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV4_SEC_ADDR(base)                   ((base) + 0x9F0)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV5_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV5_SEC_ADDR(base)                   ((base) + 0x9F4)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV6_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV6_SEC_ADDR(base)                   ((base) + 0x9F8)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV7_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV7_SEC_ADDR(base)                   ((base) + 0x9FC)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV8_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV8_SEC_ADDR(base)                   ((base) + 0xA00)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV9_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV9_SEC_ADDR(base)                   ((base) + 0xA04)

/* Register description: 时钟分频比配置
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV10_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV10_SEC_ADDR(base)                  ((base) + 0xA08)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV11_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV11_SEC_ADDR(base)                  ((base) + 0xA0C)

/* Register description: 时钟分频比控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCCLKDIV12_SEC_UNION */
#define SOC_AO_CRG_SCCLKDIV12_SEC_ADDR(base)                  ((base) + 0xA1C)

/* Register description: IP防总线挂死控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_UNION */
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ADDR(base)         ((base) + 0xA84)

/* Register description: IP防总线挂死控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_UNION */
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ADDR(base)         ((base) + 0xA88)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_CLK_STAT0_SEC_UNION */
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_ADDR(base)                ((base) + 0xA8C)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_CLK_STAT1_SEC_UNION */
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_ADDR(base)                ((base) + 0xA90)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_PLL_FSM_CTRL0_SEC_UNION */
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_ADDR(base)               ((base) + 0xA94)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_PLL_FSM_CTRL1_SEC_UNION */
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_ADDR(base)               ((base) + 0xA98)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_PLL_FSM_STAT0_SEC_UNION */
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_ADDR(base)               ((base) + 0xA9C)

/* Register description: 自动降频控制寄存器0。
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV0_UNION */
#define SOC_AO_CRG_AO_AUTODIV0_ADDR(base)                     ((base) + 0xB00)

/* Register description: 自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_AO_CRG_AO_AUTODIV1_UNION */
#define SOC_AO_CRG_AO_AUTODIV1_ADDR(base)                     ((base) + 0xB04)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SC_DDRC_CTRL_UNION */
#define SOC_AO_CRG_SC_DDRC_CTRL_ADDR(base)                    ((base) + 0xE38)

/* Register description: SPLL控制寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL0_UNION */
#define SOC_AO_CRG_SCSPLLCTRL0_ADDR(base)                     ((base) + 0xF14)

/* Register description: SPLL控制寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL1_UNION */
#define SOC_AO_CRG_SCSPLLCTRL1_ADDR(base)                     ((base) + 0xF18)

/* Register description: SPLL SSC控制寄存器
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLSSCCTRL_UNION */
#define SOC_AO_CRG_SCSPLLSSCCTRL_ADDR(base)                   ((base) + 0xF1C)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL0_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLLCTRL0_LOCK_STAT_ADDR(base)           ((base) + 0xF20)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLCTRL1_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLLCTRL1_LOCK_STAT_ADDR(base)           ((base) + 0xF24)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_ADDR(base)         ((base) + 0xF28)

/* Register description: 
   Bit domain definition UNION:  SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_UNION */
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_ADDR(base)          ((base) + 0xF2C)

/* Register description: fnpll配置寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG0_UNION */
#define SOC_AO_CRG_FNPLL_CFG0_ADDR(base)                      ((base) + 0xF30)

/* Register description: fnpll配置寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG1_UNION */
#define SOC_AO_CRG_FNPLL_CFG1_ADDR(base)                      ((base) + 0xF34)

/* Register description: fnpll配置寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG2_UNION */
#define SOC_AO_CRG_FNPLL_CFG2_ADDR(base)                      ((base) + 0xF38)

/* Register description: fnpll配置寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_CFG3_UNION */
#define SOC_AO_CRG_FNPLL_CFG3_ADDR(base)                      ((base) + 0xF3C)

/* Register description: fnpll配置寄存器0
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG0_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG0_ADDR(base)                   ((base) + 0xF40)

/* Register description: fnpll配置寄存器1
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG1_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG1_ADDR(base)                   ((base) + 0xF44)

/* Register description: fnpll配置寄存器2
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG2_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG2_ADDR(base)                   ((base) + 0xF48)

/* Register description: fnpll配置寄存器3
   Bit domain definition UNION:  SOC_AO_CRG_FNPLL_LP_CFG3_UNION */
#define SOC_AO_CRG_FNPLL_LP_CFG3_ADDR(base)                   ((base) + 0xF4C)


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
 struct               : SOC_AO_CRG_SCSYSSTAT_UNION
 struct description   : SCSYSSTAT Register structure definition
                        Address Offset:0x004 Initial:0x0 Width:32
 register description : 系统状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  fcm_tsensor0_rst_req         : 1;  /* bit[0]    : Baltimore:大中核、FCM0、ARPP、IVP */
        unsigned int  fcm_tsensor1_rst_req         : 1;  /* bit[1]    : Baltimore:MDM */
        unsigned int  reserved_0                   : 1;  /* bit[2]    : Baltimore:GPU、NPU、ISP、FCM1 */
        unsigned int  soft_rst_req                 : 1;  /* bit[3]    :  */
        unsigned int  wd0_rst_req                  : 1;  /* bit[4]    :  */
        unsigned int  wd1_rst_req                  : 1;  /* bit[5]    :  */
        unsigned int  lpmcu_rst_req                : 1;  /* bit[6]    :  */
        unsigned int  reserved_1                   : 1;  /* bit[7]    : Baltimore不使用 */
        unsigned int  iomcu_rst_req                : 1;  /* bit[8]    :  */
        unsigned int  asp_subsys_wd_req            : 1;  /* bit[9]    :  */
        unsigned int  venc_wd_rst_req              : 1;  /* bit[10]   :  */
        unsigned int  modem_wd_rst_req             : 1;  /* bit[11]   :  */
        unsigned int  reserved_2                   : 1;  /* bit[12]   :  */
        unsigned int  isp_a7_wd_rst_req            : 1;  /* bit[13]   :  */
        unsigned int  wait_ddr_selfreflash_timeout : 1;  /* bit[14]   :  */
        unsigned int  ddr_fatal_intr               : 2;  /* bit[15-16]:  */
        unsigned int  reserved_3                   : 2;  /* bit[17-18]:  */
        unsigned int  ddr_uce_wd_rst_req           : 4;  /* bit[19-22]:  */
        unsigned int  ao_wd_rst_req                : 1;  /* bit[23]   :  */
        unsigned int  reserved_4                   : 1;  /* bit[24]   :  */
        unsigned int  ao_wd1_rst_req               : 1;  /* bit[25]   :  */
        unsigned int  ddr_lp_ctrl_wd_rst_req       : 1;  /* bit[26]   :  */
        unsigned int  sys_ctrl_wd_rst_req          : 1;  /* bit[27]   :  */
        unsigned int  gpu_lp_ctrl_wd_rst_req       : 1;  /* bit[28]   :  */
        unsigned int  cpu_lp_ctrl_wd_rst_req       : 1;  /* bit[29]   :  */
        unsigned int  reserved_5                   : 1;  /* bit[30]   :  */
        unsigned int  reserved_6                   : 1;  /* bit[31]   : SYS_MISC有逻辑处理，注意修改
                                                                        写：往该寄存器写入任意值会触发系统软复位。
                                                                        读：读该寄存器读出复位源。
                                                                        复位源指示：未说明的为保留bit。 */
    } reg;
} SOC_AO_CRG_SCSYSSTAT_UNION;
#endif
#define SOC_AO_CRG_SCSYSSTAT_fcm_tsensor0_rst_req_START          (0)
#define SOC_AO_CRG_SCSYSSTAT_fcm_tsensor0_rst_req_END            (0)
#define SOC_AO_CRG_SCSYSSTAT_fcm_tsensor1_rst_req_START          (1)
#define SOC_AO_CRG_SCSYSSTAT_fcm_tsensor1_rst_req_END            (1)
#define SOC_AO_CRG_SCSYSSTAT_soft_rst_req_START                  (3)
#define SOC_AO_CRG_SCSYSSTAT_soft_rst_req_END                    (3)
#define SOC_AO_CRG_SCSYSSTAT_wd0_rst_req_START                   (4)
#define SOC_AO_CRG_SCSYSSTAT_wd0_rst_req_END                     (4)
#define SOC_AO_CRG_SCSYSSTAT_wd1_rst_req_START                   (5)
#define SOC_AO_CRG_SCSYSSTAT_wd1_rst_req_END                     (5)
#define SOC_AO_CRG_SCSYSSTAT_lpmcu_rst_req_START                 (6)
#define SOC_AO_CRG_SCSYSSTAT_lpmcu_rst_req_END                   (6)
#define SOC_AO_CRG_SCSYSSTAT_iomcu_rst_req_START                 (8)
#define SOC_AO_CRG_SCSYSSTAT_iomcu_rst_req_END                   (8)
#define SOC_AO_CRG_SCSYSSTAT_asp_subsys_wd_req_START             (9)
#define SOC_AO_CRG_SCSYSSTAT_asp_subsys_wd_req_END               (9)
#define SOC_AO_CRG_SCSYSSTAT_venc_wd_rst_req_START               (10)
#define SOC_AO_CRG_SCSYSSTAT_venc_wd_rst_req_END                 (10)
#define SOC_AO_CRG_SCSYSSTAT_modem_wd_rst_req_START              (11)
#define SOC_AO_CRG_SCSYSSTAT_modem_wd_rst_req_END                (11)
#define SOC_AO_CRG_SCSYSSTAT_isp_a7_wd_rst_req_START             (13)
#define SOC_AO_CRG_SCSYSSTAT_isp_a7_wd_rst_req_END               (13)
#define SOC_AO_CRG_SCSYSSTAT_wait_ddr_selfreflash_timeout_START  (14)
#define SOC_AO_CRG_SCSYSSTAT_wait_ddr_selfreflash_timeout_END    (14)
#define SOC_AO_CRG_SCSYSSTAT_ddr_fatal_intr_START                (15)
#define SOC_AO_CRG_SCSYSSTAT_ddr_fatal_intr_END                  (16)
#define SOC_AO_CRG_SCSYSSTAT_ddr_uce_wd_rst_req_START            (19)
#define SOC_AO_CRG_SCSYSSTAT_ddr_uce_wd_rst_req_END              (22)
#define SOC_AO_CRG_SCSYSSTAT_ao_wd_rst_req_START                 (23)
#define SOC_AO_CRG_SCSYSSTAT_ao_wd_rst_req_END                   (23)
#define SOC_AO_CRG_SCSYSSTAT_ao_wd1_rst_req_START                (25)
#define SOC_AO_CRG_SCSYSSTAT_ao_wd1_rst_req_END                  (25)
#define SOC_AO_CRG_SCSYSSTAT_ddr_lp_ctrl_wd_rst_req_START        (26)
#define SOC_AO_CRG_SCSYSSTAT_ddr_lp_ctrl_wd_rst_req_END          (26)
#define SOC_AO_CRG_SCSYSSTAT_sys_ctrl_wd_rst_req_START           (27)
#define SOC_AO_CRG_SCSYSSTAT_sys_ctrl_wd_rst_req_END             (27)
#define SOC_AO_CRG_SCSYSSTAT_gpu_lp_ctrl_wd_rst_req_START        (28)
#define SOC_AO_CRG_SCSYSSTAT_gpu_lp_ctrl_wd_rst_req_END          (28)
#define SOC_AO_CRG_SCSYSSTAT_cpu_lp_ctrl_wd_rst_req_START        (29)
#define SOC_AO_CRG_SCSYSSTAT_cpu_lp_ctrl_wd_rst_req_END          (29)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_GT_CFG_UNION
 struct description   : SC_ULPPLL_GT_CFG Register structure definition
                        Address Offset:0x088 Initial:0x0000000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_gt_cfg : 1;  /* bit[0]   :  */
        unsigned int  reserved_0    : 3;  /* bit[1-3] :  */
        unsigned int  reserved_1    : 1;  /* bit[4]   :  */
        unsigned int  reserved_2    : 27; /* bit[5-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_GT_CFG_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_GT_CFG_ulppll_gt_cfg_START  (0)
#define SOC_AO_CRG_SC_ULPPLL_GT_CFG_ulppll_gt_cfg_END    (0)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_UNION
 struct description   : SC_ULPPLL_1_GT_CFG Register structure definition
                        Address Offset:0x08C Initial:0x0000000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_1_gt_cfg : 1;  /* bit[0]   :  */
        unsigned int  reserved_0      : 3;  /* bit[1-3] :  */
        unsigned int  reserved_1      : 1;  /* bit[4]   :  */
        unsigned int  reserved_2      : 27; /* bit[5-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_ulppll_1_gt_cfg_START  (0)
#define SOC_AO_CRG_SC_ULPPLL_1_GT_CFG_ulppll_1_gt_cfg_END    (0)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_STAT_UNION
 struct description   : SC_PLL_STAT Register structure definition
                        Address Offset:0x0CC Initial:0x0000 Width:32
 register description : PLL状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spll_lock           : 1;  /* bit[0]    : SPLL状态指示。
                                                               0：未稳定；
                                                               1：已稳定。 */
        unsigned int  spll_en_stat        : 1;  /* bit[1]    : SPLL最终使能状态指示（由SPLL_en与IOMCU_pll_ctrl、ppll_en_cpu、SPLL_en_modem或）。
                                                               0：去使能；
                                                               1：使能。 */
        unsigned int  spll_gt_stat        : 1;  /* bit[2]    : SPLL最终门控状态指示：
                                                               0：门控生效，无时钟；
                                                               1：门控无效，有时钟。 */
        unsigned int  spll_bypass_stat    : 1;  /* bit[3]    : SPLL最终bypass状态指示：
                                                               0：没有bypass，输出PLL时钟；
                                                               1：bypass，输出19.2Mhz时钟。 */
        unsigned int  ulppll_en_stat      : 1;  /* bit[4]    :  */
        unsigned int  ulppll_gt_stat      : 1;  /* bit[5]    :  */
        unsigned int  ulppll_bp_stat      : 1;  /* bit[6]    :  */
        unsigned int  reserved_0          : 1;  /* bit[7]    :  */
        unsigned int  reserved_1          : 1;  /* bit[8]    :  */
        unsigned int  reserved_2          : 1;  /* bit[9]    :  */
        unsigned int  reserved_3          : 1;  /* bit[10]   :  */
        unsigned int  ulppll_op_freq_flag : 1;  /* bit[11]   :  */
        unsigned int  ulppll_1_en_stat    : 1;  /* bit[12]   :  */
        unsigned int  ulppll_1_gt_stat    : 1;  /* bit[13]   :  */
        unsigned int  ulppll_1_bp_stat    : 1;  /* bit[14]   :  */
        unsigned int  ulppll_sleep_en     : 1;  /* bit[15]   :  */
        unsigned int  reserved_4          : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SC_PLL_STAT_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_STAT_spll_lock_START            (0)
#define SOC_AO_CRG_SC_PLL_STAT_spll_lock_END              (0)
#define SOC_AO_CRG_SC_PLL_STAT_spll_en_stat_START         (1)
#define SOC_AO_CRG_SC_PLL_STAT_spll_en_stat_END           (1)
#define SOC_AO_CRG_SC_PLL_STAT_spll_gt_stat_START         (2)
#define SOC_AO_CRG_SC_PLL_STAT_spll_gt_stat_END           (2)
#define SOC_AO_CRG_SC_PLL_STAT_spll_bypass_stat_START     (3)
#define SOC_AO_CRG_SC_PLL_STAT_spll_bypass_stat_END       (3)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_en_stat_START       (4)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_en_stat_END         (4)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_gt_stat_START       (5)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_gt_stat_END         (5)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_bp_stat_START       (6)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_bp_stat_END         (6)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_op_freq_flag_START  (11)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_op_freq_flag_END    (11)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_1_en_stat_START     (12)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_1_en_stat_END       (12)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_1_gt_stat_START     (13)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_1_gt_stat_END       (13)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_1_bp_stat_START     (14)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_1_bp_stat_END       (14)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_sleep_en_START      (15)
#define SOC_AO_CRG_SC_PLL_STAT_ulppll_sleep_en_END        (15)


/*****************************************************************************
 struct               : SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_UNION
 struct description   : ULPPLL_DT_WORK_EN_VOTE Register structure definition
                        Address Offset:0x0D0 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_dt_work_en_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken              : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_UNION;
#endif
#define SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_ulppll_dt_work_en_vote_START  (0)
#define SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_ulppll_dt_work_en_vote_END    (15)
#define SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_bitmasken_START               (16)
#define SOC_AO_CRG_ULPPLL_DT_WORK_EN_VOTE_bitmasken_END                 (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_EN_VOTE_1_UNION
 struct description   : SC_PLL_EN_VOTE_1 Register structure definition
                        Address Offset:0x0D4 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_en_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken      : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_EN_VOTE_1_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_EN_VOTE_1_ulppll_en_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_EN_VOTE_1_ulppll_en_vote_END    (15)
#define SOC_AO_CRG_SC_PLL_EN_VOTE_1_bitmasken_START       (16)
#define SOC_AO_CRG_SC_PLL_EN_VOTE_1_bitmasken_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_EN_VOTE_3_UNION
 struct description   : SC_PLL_EN_VOTE_3 Register structure definition
                        Address Offset:0x0DC Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_1_en_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_EN_VOTE_3_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_EN_VOTE_3_ulppll_1_en_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_EN_VOTE_3_ulppll_1_en_vote_END    (15)
#define SOC_AO_CRG_SC_PLL_EN_VOTE_3_bitmasken_START         (16)
#define SOC_AO_CRG_SC_PLL_EN_VOTE_3_bitmasken_END           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_GT_VOTE_1_UNION
 struct description   : SC_PLL_GT_VOTE_1 Register structure definition
                        Address Offset:0x0E0 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_gt_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken      : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_GT_VOTE_1_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_GT_VOTE_1_ulppll_gt_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_GT_VOTE_1_ulppll_gt_vote_END    (15)
#define SOC_AO_CRG_SC_PLL_GT_VOTE_1_bitmasken_START       (16)
#define SOC_AO_CRG_SC_PLL_GT_VOTE_1_bitmasken_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_GT_VOTE_3_UNION
 struct description   : SC_PLL_GT_VOTE_3 Register structure definition
                        Address Offset:0x0E4 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_1_gt_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_GT_VOTE_3_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_GT_VOTE_3_ulppll_1_gt_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_GT_VOTE_3_ulppll_1_gt_vote_END    (15)
#define SOC_AO_CRG_SC_PLL_GT_VOTE_3_bitmasken_START         (16)
#define SOC_AO_CRG_SC_PLL_GT_VOTE_3_bitmasken_END           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_BP_VOTE_1_UNION
 struct description   : SC_PLL_BP_VOTE_1 Register structure definition
                        Address Offset:0x0E8 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_bp_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken      : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_BP_VOTE_1_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_BP_VOTE_1_ulppll_bp_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_BP_VOTE_1_ulppll_bp_vote_END    (15)
#define SOC_AO_CRG_SC_PLL_BP_VOTE_1_bitmasken_START       (16)
#define SOC_AO_CRG_SC_PLL_BP_VOTE_1_bitmasken_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_BP_VOTE_3_UNION
 struct description   : SC_PLL_BP_VOTE_3 Register structure definition
                        Address Offset:0x0EC Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_1_bp_vote : 16; /* bit[0-15] :  */
        unsigned int  bitmasken        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_BP_VOTE_3_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_BP_VOTE_3_ulppll_1_bp_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_BP_VOTE_3_ulppll_1_bp_vote_END    (15)
#define SOC_AO_CRG_SC_PLL_BP_VOTE_3_bitmasken_START         (16)
#define SOC_AO_CRG_SC_PLL_BP_VOTE_3_bitmasken_END           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG0_UNION
 struct description   : SC_ULPPLL_CFG0 Register structure definition
                        Address Offset:0x0F0 Initial:0x4 Width:32
 register description : ULPPLL_配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  x2_en_cfg        : 1;  /* bit[0]    : 控制PLL是否为X2模式：
                                                            0：x1模式【默认】
                                                            1：x2模式 */
        unsigned int  reserved         : 1;  /* bit[1]    : reserved */
        unsigned int  ki_s             : 4;  /* bit[2-5]  : X1模式下,LPF积分路径因子，移位项 */
        unsigned int  ki_e             : 3;  /* bit[6-8]  : X1模式下,LPF积分路径因子，乘积项 */
        unsigned int  kp_s             : 3;  /* bit[9-11] : X1模式下,LPF比例路径因子，移位项 */
        unsigned int  kp_e             : 3;  /* bit[12-14]: X1模式下,LPF比例路径因子，乘积项 */
        unsigned int  ki_s_x2          : 4;  /* bit[15-18]: X2模式下，LPF积分路径因子，移位项，x2锁定 */
        unsigned int  ki_e_x2          : 3;  /* bit[19-21]: X2模式下，LPF积分路径因子，乘积项，x2锁定 */
        unsigned int  kp_s_x2          : 3;  /* bit[22-24]: X2模式下，LPF比例路径因子，移位项，x2锁定 */
        unsigned int  kp_e_x2          : 3;  /* bit[25-27]: X2模式下，LPF比例路径因子，乘积项，x2锁定 */
        unsigned int  kdco_ctrl_cfg_en : 1;  /* bit[28]   : kdco测试模式使能信号
                                                            0：kdco自动校正模式，使用KDCO自动校正码值【默认】
                                                            1：测试模式，直接使用软件配置码值kdco_ctrl_cfg */
        unsigned int  kdco_ctrl_cfg    : 3;  /* bit[29-31]: 电流基准档位配置，配置值越大电流档位越大
                                                            测试模式下，可直接软件配置的电流基准档位，0~7共八个档位;
                                                            非测试模式下，初始电流基准档位，默认为中间值，0~7可配
                                                            二进制和独热码关系
                                                            3'b000 : 7'b000_0000
                                                            3'b001 ： 7'b000_0001
                                                            3'b010 ： 7'b000_0010
                                                            3'b011 ： 7'b000_0100
                                                            3'b100 ： 7'b000_1000
                                                            3'b101 ： 7'b001_0000
                                                            3'b110 ： 7'b010_0000
                                                            3'b111 ： 7'b100_0000 */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG0_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_CFG0_x2_en_cfg_START         (0)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_x2_en_cfg_END           (0)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_s_START              (2)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_s_END                (5)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_e_START              (6)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_e_END                (8)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_s_START              (9)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_s_END                (11)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_e_START              (12)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_e_END                (14)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_s_x2_START           (15)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_s_x2_END             (18)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_e_x2_START           (19)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_ki_e_x2_END             (21)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_s_x2_START           (22)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_s_x2_END             (24)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_e_x2_START           (25)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kp_e_x2_END             (27)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kdco_ctrl_cfg_en_START  (28)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kdco_ctrl_cfg_en_END    (28)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kdco_ctrl_cfg_START     (29)
#define SOC_AO_CRG_SC_ULPPLL_CFG0_kdco_ctrl_cfg_END       (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG1_UNION
 struct description   : SC_ULPPLL_CFG1 Register structure definition
                        Address Offset:0x0F4 Initial:0x000 Width:32
 register description : ULPPLL_配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_vld           : 1;  /* bit[0]    : ULPPLL_CFG2配置是否有效
                                                             0：无效【默认】
                                                             1：有效 */
        unsigned int  cfg_sel           : 2;  /* bit[1-2]  : ULPPLL_CFG2配置选择(在rst_cfg_n=1 & cfg_vld=1时，才能刷新cfg_sel选择的寄存器)
                                                             2'b00:pll_cfg2_0【默认】
                                                             2'b01:pll_cfg2_1
                                                             2'b10:pll_cfg2_2
                                                             2'b11:pll_cfg2_3 */
        unsigned int  rst_cfg_n         : 1;  /* bit[3]    : 寄存器复用ULPPLL_CFG2_0~3配置的复位端
                                                             0：复位【默认】
                                                             1：解复位 */
        unsigned int  test_data_sel     : 4;  /* bit[4-7]  : test模式输出数据选择,从ULPPLL_STATE读出来
                                                             4'b0000:ulppll_state0_0【默认】
                                                             4'b0001:ulppll_state0_1
                                                             4'b0010:ulppll_state0_2
                                                             4'b0011:ulppll_state0_3
                                                             4'b0100:ulppll_state0_4
                                                             4'b0101:ulppll_cfg2_0
                                                             4'b0110:ulppll_cfg2_1
                                                             4'b0111:ulppll_cfg2_2
                                                             4'b1000:ulppll_cfg2_3
                                                             others:ulppll_state0_0 */
        unsigned int  read_en           : 1;  /* bit[8]    : 读寄存器使能信号，上升沿更新寄存器数据，其他情况下保持。使用时先配成0，再配成1 */
        unsigned int  test_ck_en        : 1;  /* bit[9]    : test数据回读模块时钟使能信号
                                                             0：不使能【默认】
                                                             1：使能 */
        unsigned int  filter_rhm_sw_pll : 1;  /* bit[10]   : 滤波器电阻开关
                                                             1：电阻接入【默认】
                                                             0：电阻断开
                                                             filter_byp、filter_rhm_sw_pll、swcap_en至少一个为1，否则环路断开；同时为1时，为bypass模式 */
        unsigned int  swcap_en          : 1;  /* bit[11]   : 开关电容使能信号：
                                                             1'b0 不使能【默认】
                                                             1'b1 使能 */
        unsigned int  delay_sel         : 4;  /* bit[12-15]: 调整不交叠时钟的不交叠区域(后仿 TT)：
                                                             0000：不交叠区域3.5ns
                                                             0001：不交叠区域50ns【默认】
                                                             0011：不交叠区域95ns
                                                             0111：不交叠区域140ns
                                                             1111：不交叠区域186ns
                                                             其他值禁止配置 */
        unsigned int  sel_d2a_swcap     : 2;  /* bit[16-17]: 选择输出到模拟的时钟ckv/div，用于产生两相不交叠时钟
                                                             2'b00: div = 2^(8+1)
                                                             2'b01: div = 2^(9+1)
                                                             2'b10: div = 2^(10+1)
                                                             2'b11: div = 2^(11+1) */
        unsigned int  ulppll_ana_cfg    : 5;  /* bit[18-22]: reserved */
        unsigned int  reserved          : 9;  /* bit[23-31]: reserved */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG1_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_CFG1_cfg_vld_START            (0)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_cfg_vld_END              (0)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_cfg_sel_START            (1)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_cfg_sel_END              (2)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_rst_cfg_n_START          (3)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_rst_cfg_n_END            (3)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_test_data_sel_START      (4)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_test_data_sel_END        (7)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_read_en_START            (8)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_read_en_END              (8)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_test_ck_en_START         (9)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_test_ck_en_END           (9)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_filter_rhm_sw_pll_START  (10)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_filter_rhm_sw_pll_END    (10)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_swcap_en_START           (11)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_swcap_en_END             (11)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_delay_sel_START          (12)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_delay_sel_END            (15)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_sel_d2a_swcap_START      (16)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_sel_d2a_swcap_END        (17)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_ulppll_ana_cfg_START     (18)
#define SOC_AO_CRG_SC_ULPPLL_CFG1_ulppll_ana_cfg_END       (22)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG2_UNION
 struct description   : SC_ULPPLL_CFG2 Register structure definition
                        Address Offset:0x0F8 Initial:0x10201020 Width:32
 register description : ULPPLL_配置2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 在rst_cfg_n=1 & cfg_vld=1时，cfg_sel选择的寄存器，对应寄存器详细描述见“寄存器复用详细说明”页
                                                   2'b00:SCFPLL_CFG2_0【默认】
                                                   2'b01:SCFPLL_CFG2_1
                                                   2'b10:SCFPLL_CFG2_2 */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG3_UNION
 struct description   : SC_ULPPLL_CFG3 Register structure definition
                        Address Offset:0x0FC Initial:0x00000000 Width:32
 register description : ULPPLL_配置3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG4_UNION
 struct description   : SC_ULPPLL_CFG4 Register structure definition
                        Address Offset:0x100 Initial:0x00000000 Width:32
 register description : ULPPLL_配置4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG4_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG5_UNION
 struct description   : SC_ULPPLL_CFG5 Register structure definition
                        Address Offset:0x104 Initial:0x00000000 Width:32
 register description : ULPPLL_配置5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG5_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG6_UNION
 struct description   : SC_ULPPLL_CFG6 Register structure definition
                        Address Offset:0x108 Initial:0x00000000 Width:32
 register description : ULPPLL_配置6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG6_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG7_UNION
 struct description   : SC_ULPPLL_CFG7 Register structure definition
                        Address Offset:0x10C Initial:0x00000000 Width:32
 register description : ULPPLL_配置7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG7_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CFG8_UNION
 struct description   : SC_ULPPLL_CFG8 Register structure definition
                        Address Offset:0x110 Initial:0x00000000 Width:32
 register description : ULPPLL_配置8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CFG8_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_CTRL0_UNION
 struct description   : SC_ULPPLL_CTRL0 Register structure definition
                        Address Offset:0x11C Initial:0x000 Width:32
 register description : ULPPLL_控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_en     : 1;  /* bit[0]    : ULPPLL PowerDown全局控制。
                                                         0：关闭PLL【默认】；
                                                         1：使能PLL。 */
        unsigned int  ulppll_bypass : 1;  /* bit[1]    : ULPPLL Bypass控制，动态切换会有毛刺。
                                                         0：关闭bypass通路，输出postdiv后的时钟；
                                                         1：打开bypass通路，直接输出参考时钟【默认】。 */
        unsigned int  reserved_0    : 2;  /* bit[2-3]  :  */
        unsigned int  reserved_1    : 3;  /* bit[4-6]  :  */
        unsigned int  reserved_2    : 1;  /* bit[7]    :  */
        unsigned int  ulppll_fbdiv  : 15; /* bit[8-22] : PLL配置整数分频比，支持频率范围为120M~322M */
        unsigned int  reserved_3    : 9;  /* bit[23-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_CTRL0_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ulppll_en_START      (0)
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ulppll_en_END        (0)
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ulppll_bypass_START  (1)
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ulppll_bypass_END    (1)
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ulppll_fbdiv_START   (8)
#define SOC_AO_CRG_SC_ULPPLL_CTRL0_ulppll_fbdiv_END     (22)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_STATE0_UNION
 struct description   : SC_ULPPLL_STATE0 Register structure definition
                        Address Offset:0x120 Initial:0x00000000 Width:32
 register description : ULPPLL_状态0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 根据test_data_sel的值，选择输出到ULPPLL_STATE0端口的值，对应寄存器详细描述见“寄存器复用详细说明”页
                                                   4'b0000:SCFPLLSTAT_0【默认】
                                                   4'b0001:SCFPLLSTAT_1
                                                   4'b0010:SCFPLLSTAT_2
                                                   4'b0011:SCFPLLSTAT_3
                                                   4'b0100:SCFPLLSTAT_4
                                                   4'b0101:SCFPLL_CFG2_0
                                                   4'b0110:SCFPLL_CFG2_1
                                                   4'b0111:SCFPLL_CFG2_2
                                                   4'b1000:SCFPLL_CFG2_3
                                                   others:SCFPLLSTAT_5 */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_STATE0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_1_CFG0_UNION
 struct description   : SC_ULPPLL_1_CFG0 Register structure definition
                        Address Offset:0x140 Initial:0x4 Width:32
 register description : ULPPLL_1_配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  x2_en_cfg        : 1;  /* bit[0]    : 控制PLL是否为X2模式：
                                                            0：x1模式【默认】
                                                            1：x2模式 */
        unsigned int  reserved         : 1;  /* bit[1]    : reserved */
        unsigned int  ki_s             : 4;  /* bit[2-5]  : X1模式下,LPF积分路径因子，移位项 */
        unsigned int  ki_e             : 3;  /* bit[6-8]  : X1模式下,LPF积分路径因子，乘积项 */
        unsigned int  kp_s             : 3;  /* bit[9-11] : X1模式下,LPF比例路径因子，移位项 */
        unsigned int  kp_e             : 3;  /* bit[12-14]: X1模式下,LPF比例路径因子，乘积项 */
        unsigned int  ki_s_x2          : 4;  /* bit[15-18]: X2模式下，LPF积分路径因子，移位项，x2锁定 */
        unsigned int  ki_e_x2          : 3;  /* bit[19-21]: X2模式下，LPF积分路径因子，乘积项，x2锁定 */
        unsigned int  kp_s_x2          : 3;  /* bit[22-24]: X2模式下，LPF比例路径因子，移位项，x2锁定 */
        unsigned int  kp_e_x2          : 3;  /* bit[25-27]: X2模式下，LPF比例路径因子，乘积项，x2锁定 */
        unsigned int  kdco_ctrl_cfg_en : 1;  /* bit[28]   : kdco测试模式使能信号
                                                            0：kdco自动校正模式，使用KDCO自动校正码值【默认】
                                                            1：测试模式，直接使用软件配置码值kdco_ctrl_cfg */
        unsigned int  kdco_ctrl_cfg    : 3;  /* bit[29-31]: 电流基准档位配置，配置值越大电流档位越大
                                                            测试模式下，可直接软件配置的电流基准档位，0~7共八个档位;
                                                            非测试模式下，初始电流基准档位，默认为中间值，0~7可配
                                                            二进制和独热码关系
                                                            3'b000 : 7'b000_0000
                                                            3'b001 ： 7'b000_0001
                                                            3'b010 ： 7'b000_0010
                                                            3'b011 ： 7'b000_0100
                                                            3'b100 ： 7'b000_1000
                                                            3'b101 ： 7'b001_0000
                                                            3'b110 ： 7'b010_0000
                                                            3'b111 ： 7'b100_0000 */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_1_CFG0_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_x2_en_cfg_START         (0)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_x2_en_cfg_END           (0)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_s_START              (2)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_s_END                (5)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_e_START              (6)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_e_END                (8)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_s_START              (9)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_s_END                (11)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_e_START              (12)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_e_END                (14)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_s_x2_START           (15)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_s_x2_END             (18)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_e_x2_START           (19)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_ki_e_x2_END             (21)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_s_x2_START           (22)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_s_x2_END             (24)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_e_x2_START           (25)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kp_e_x2_END             (27)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kdco_ctrl_cfg_en_START  (28)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kdco_ctrl_cfg_en_END    (28)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kdco_ctrl_cfg_START     (29)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG0_kdco_ctrl_cfg_END       (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_1_CFG1_UNION
 struct description   : SC_ULPPLL_1_CFG1 Register structure definition
                        Address Offset:0x144 Initial:0x000 Width:32
 register description : ULPPLL_1_配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_vld           : 1;  /* bit[0]    : ULPPLL_CFG2配置是否有效
                                                             0：无效【默认】
                                                             1：有效 */
        unsigned int  cfg_sel           : 2;  /* bit[1-2]  : ULPPLL_CFG2配置选择(在rst_cfg_n=1 & cfg_vld=1时，才能刷新cfg_sel选择的寄存器)
                                                             2'b00:pll_cfg2_0【默认】
                                                             2'b01:pll_cfg2_1
                                                             2'b10:pll_cfg2_2
                                                             2'b11:pll_cfg2_3 */
        unsigned int  rst_cfg_n         : 1;  /* bit[3]    : 寄存器复用ULPPLL_CFG2_0~3配置的复位端
                                                             0：复位【默认】
                                                             1：解复位 */
        unsigned int  test_data_sel     : 4;  /* bit[4-7]  : test模式输出数据选择,从ULPPLL_STATE读出来
                                                             4'b0000:ulppll_state0_0【默认】
                                                             4'b0001:ulppll_state0_1
                                                             4'b0010:ulppll_state0_2
                                                             4'b0011:ulppll_state0_3
                                                             4'b0100:ulppll_state0_4
                                                             4'b0101:ulppll_cfg2_0
                                                             4'b0110:ulppll_cfg2_1
                                                             4'b0111:ulppll_cfg2_2
                                                             4'b1000:ulppll_cfg2_3
                                                             others:ulppll_state0_0 */
        unsigned int  read_en           : 1;  /* bit[8]    : 读寄存器使能信号，上升沿更新寄存器数据，其他情况下保持。使用时先配成0，再配成1 */
        unsigned int  test_ck_en        : 1;  /* bit[9]    : test数据回读模块时钟使能信号
                                                             0：不使能【默认】
                                                             1：使能 */
        unsigned int  filter_rhm_sw_pll : 1;  /* bit[10]   : 滤波器电阻开关
                                                             1：电阻接入【默认】
                                                             0：电阻断开
                                                             filter_byp、filter_rhm_sw_pll、swcap_en至少一个为1，否则环路断开；同时为1时，为bypass模式 */
        unsigned int  swcap_en          : 1;  /* bit[11]   : 开关电容使能信号：
                                                             1'b0 不使能【默认】
                                                             1'b1 使能 */
        unsigned int  delay_sel         : 4;  /* bit[12-15]: 调整不交叠时钟的不交叠区域(后仿 TT)：
                                                             0000：不交叠区域3.5ns
                                                             0001：不交叠区域50ns【默认】
                                                             0011：不交叠区域95ns
                                                             0111：不交叠区域140ns
                                                             1111：不交叠区域186ns
                                                             其他值禁止配置 */
        unsigned int  sel_d2a_swcap     : 2;  /* bit[16-17]: 选择输出到模拟的时钟ckv/div，用于产生两相不交叠时钟
                                                             2'b00: div = 2^(8+1)
                                                             2'b01: div = 2^(9+1)
                                                             2'b10: div = 2^(10+1)
                                                             2'b11: div = 2^(11+1) */
        unsigned int  ulppll_ana_cfg    : 5;  /* bit[18-22]: reserved */
        unsigned int  reserved          : 9;  /* bit[23-31]: reserved */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_1_CFG1_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_cfg_vld_START            (0)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_cfg_vld_END              (0)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_cfg_sel_START            (1)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_cfg_sel_END              (2)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_rst_cfg_n_START          (3)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_rst_cfg_n_END            (3)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_test_data_sel_START      (4)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_test_data_sel_END        (7)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_read_en_START            (8)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_read_en_END              (8)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_test_ck_en_START         (9)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_test_ck_en_END           (9)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_filter_rhm_sw_pll_START  (10)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_filter_rhm_sw_pll_END    (10)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_swcap_en_START           (11)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_swcap_en_END             (11)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_delay_sel_START          (12)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_delay_sel_END            (15)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_sel_d2a_swcap_START      (16)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_sel_d2a_swcap_END        (17)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_ulppll_ana_cfg_START     (18)
#define SOC_AO_CRG_SC_ULPPLL_1_CFG1_ulppll_ana_cfg_END       (22)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_1_CFG2_UNION
 struct description   : SC_ULPPLL_1_CFG2 Register structure definition
                        Address Offset:0x148 Initial:0x10201020 Width:32
 register description : ULPPLL_1_配置2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 在rst_cfg_n=1 & cfg_vld=1时，cfg_sel选择的寄存器，对应寄存器详细描述见“寄存器复用详细说明”页
                                                   2'b00:SCFPLL_CFG2_0【默认】
                                                   2'b01:SCFPLL_CFG2_1
                                                   2'b10:SCFPLL_CFG2_2 */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_1_CFG2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_1_CTRL0_UNION
 struct description   : SC_ULPPLL_1_CTRL0 Register structure definition
                        Address Offset:0x14C Initial:0x000 Width:32
 register description : ULPPLL_1_控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ulppll_en      : 1;  /* bit[0]    : ULPPLL PowerDown全局控制。
                                                          0：关闭PLL【默认】；
                                                          1：使能PLL。 */
        unsigned int  ulppll_bypass  : 1;  /* bit[1]    : ULPPLL Bypass控制，动态切换会有毛刺。
                                                          0：关闭bypass通路，输出postdiv后的时钟；
                                                          1：打开bypass通路，直接输出参考时钟【默认】。 */
        unsigned int  reserved_0     : 2;  /* bit[2-3]  :  */
        unsigned int  ulppll_postdiv : 3;  /* bit[4-6]  : postdiv分频比选择信号：
                                                          0:1分频【默认】
                                                          1:2分频
                                                          2:4分频
                                                          3:8分频
                                                          4:16分频
                                                          其他：1分频 */
        unsigned int  reserved_1     : 1;  /* bit[7]    :  */
        unsigned int  ulppll_fbdiv   : 15; /* bit[8-22] : PLL配置整数分频比，支持频率范围为120M~322M */
        unsigned int  reserved_2     : 9;  /* bit[23-31]:  */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_1_CTRL0_UNION;
#endif
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_en_START       (0)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_en_END         (0)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_bypass_START   (1)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_bypass_END     (1)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_postdiv_START  (4)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_postdiv_END    (6)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_fbdiv_START    (8)
#define SOC_AO_CRG_SC_ULPPLL_1_CTRL0_ulppll_fbdiv_END      (22)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_ULPPLL_1_STATE0_UNION
 struct description   : SC_ULPPLL_1_STATE0 Register structure definition
                        Address Offset:0x150 Initial:0x00000000 Width:32
 register description : ULPPLL_1_状态0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 根据test_data_sel的值，选择输出到ULPPLL_STATE0端口的值，对应寄存器详细描述见“寄存器复用详细说明”页
                                                   4'b0000:SCFPLLSTAT_0【默认】
                                                   4'b0001:SCFPLLSTAT_1
                                                   4'b0010:SCFPLLSTAT_2
                                                   4'b0011:SCFPLLSTAT_3
                                                   4'b0100:SCFPLLSTAT_4
                                                   4'b0101:SCFPLL_CFG2_0
                                                   4'b0110:SCFPLL_CFG2_1
                                                   4'b0111:SCFPLL_CFG2_2
                                                   4'b1000:SCFPLL_CFG2_3
                                                   others:SCFPLLSTAT_5 */
    } reg;
} SOC_AO_CRG_SC_ULPPLL_1_STATE0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN0_UNION
 struct description   : SCPEREN0 Register structure definition
                        Address Offset:0x160 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0          : 1;  /* bit[0] : 外设时钟使能控制：
                                                            0：写0无效果；
                                                            1：使能IP时钟。 */
        unsigned int  gt_pclk_rtc         : 1;  /* bit[1] :  */
        unsigned int  reserved_1          : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_timer0      : 1;  /* bit[3] :  */
        unsigned int  reserved_2          : 1;  /* bit[4] :  */
        unsigned int  gt_clk_mad_32k      : 1;  /* bit[5] :  */
        unsigned int  gt_clk_fll_test_src : 1;  /* bit[6] :  */
        unsigned int  reserved_3          : 1;  /* bit[7] :  */
        unsigned int  reserved_4          : 1;  /* bit[8] :  */
        unsigned int  reserved_5          : 1;  /* bit[9] :  */
        unsigned int  reserved_6          : 1;  /* bit[10]:  */
        unsigned int  reserved_7          : 1;  /* bit[11]:  */
        unsigned int  reserved_8          : 1;  /* bit[12]:  */
        unsigned int  reserved_9          : 1;  /* bit[13]:  */
        unsigned int  reserved_10         : 1;  /* bit[14]:  */
        unsigned int  gt_pclk_ao_ioc      : 1;  /* bit[15]:  */
        unsigned int  gt_pclk_blpwm2      : 1;  /* bit[16]:  */
        unsigned int  gt_clk_mad_acpu     : 1;  /* bit[17]:  */
        unsigned int  gt_clk_jtag_auth    : 1;  /* bit[18]:  */
        unsigned int  gt_pclk_syscnt      : 1;  /* bit[19]:  */
        unsigned int  gt_clk_syscnt       : 1;  /* bit[20]:  */
        unsigned int  reserved_11         : 1;  /* bit[21]:  */
        unsigned int  reserved_12         : 1;  /* bit[22]:  */
        unsigned int  reserved_13         : 1;  /* bit[23]:  */
        unsigned int  reserved_14         : 1;  /* bit[24]:  */
        unsigned int  reserved_15         : 1;  /* bit[25]:  */
        unsigned int  reserved_16         : 1;  /* bit[26]:  */
        unsigned int  gt_clk_asp_tcxo     : 1;  /* bit[27]:  */
        unsigned int  gt_pclk_bbpdrx      : 1;  /* bit[28]:  */
        unsigned int  reserved_17         : 1;  /* bit[29]:  */
        unsigned int  reserved_18         : 1;  /* bit[30]:  */
        unsigned int  reserved_19         : 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SCPEREN0_UNION;
#endif
#define SOC_AO_CRG_SCPEREN0_gt_pclk_rtc_START          (1)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_rtc_END            (1)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_timer0_START       (3)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_timer0_END         (3)
#define SOC_AO_CRG_SCPEREN0_gt_clk_mad_32k_START       (5)
#define SOC_AO_CRG_SCPEREN0_gt_clk_mad_32k_END         (5)
#define SOC_AO_CRG_SCPEREN0_gt_clk_fll_test_src_START  (6)
#define SOC_AO_CRG_SCPEREN0_gt_clk_fll_test_src_END    (6)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_ao_ioc_START       (15)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_ao_ioc_END         (15)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_blpwm2_START       (16)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_blpwm2_END         (16)
#define SOC_AO_CRG_SCPEREN0_gt_clk_mad_acpu_START      (17)
#define SOC_AO_CRG_SCPEREN0_gt_clk_mad_acpu_END        (17)
#define SOC_AO_CRG_SCPEREN0_gt_clk_jtag_auth_START     (18)
#define SOC_AO_CRG_SCPEREN0_gt_clk_jtag_auth_END       (18)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_syscnt_START       (19)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_syscnt_END         (19)
#define SOC_AO_CRG_SCPEREN0_gt_clk_syscnt_START        (20)
#define SOC_AO_CRG_SCPEREN0_gt_clk_syscnt_END          (20)
#define SOC_AO_CRG_SCPEREN0_gt_clk_asp_tcxo_START      (27)
#define SOC_AO_CRG_SCPEREN0_gt_clk_asp_tcxo_END        (27)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_bbpdrx_START       (28)
#define SOC_AO_CRG_SCPEREN0_gt_pclk_bbpdrx_END         (28)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS0_UNION
 struct description   : SCPERDIS0 Register structure definition
                        Address Offset:0x164 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0          : 1;  /* bit[0] : 外设时钟禁止控制：
                                                            0：写0无效果；
                                                            1：禁止IP时钟。 */
        unsigned int  gt_pclk_rtc         : 1;  /* bit[1] :  */
        unsigned int  reserved_1          : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_timer0      : 1;  /* bit[3] :  */
        unsigned int  reserved_2          : 1;  /* bit[4] :  */
        unsigned int  gt_clk_mad_32k      : 1;  /* bit[5] :  */
        unsigned int  gt_clk_fll_test_src : 1;  /* bit[6] :  */
        unsigned int  reserved_3          : 1;  /* bit[7] :  */
        unsigned int  reserved_4          : 1;  /* bit[8] :  */
        unsigned int  reserved_5          : 1;  /* bit[9] :  */
        unsigned int  reserved_6          : 1;  /* bit[10]:  */
        unsigned int  reserved_7          : 1;  /* bit[11]:  */
        unsigned int  reserved_8          : 1;  /* bit[12]:  */
        unsigned int  reserved_9          : 1;  /* bit[13]:  */
        unsigned int  reserved_10         : 1;  /* bit[14]:  */
        unsigned int  gt_pclk_ao_ioc      : 1;  /* bit[15]:  */
        unsigned int  gt_pclk_blpwm2      : 1;  /* bit[16]:  */
        unsigned int  gt_clk_mad_acpu     : 1;  /* bit[17]:  */
        unsigned int  gt_clk_jtag_auth    : 1;  /* bit[18]:  */
        unsigned int  gt_pclk_syscnt      : 1;  /* bit[19]:  */
        unsigned int  gt_clk_syscnt       : 1;  /* bit[20]:  */
        unsigned int  reserved_11         : 1;  /* bit[21]:  */
        unsigned int  reserved_12         : 1;  /* bit[22]:  */
        unsigned int  reserved_13         : 1;  /* bit[23]:  */
        unsigned int  reserved_14         : 1;  /* bit[24]:  */
        unsigned int  reserved_15         : 1;  /* bit[25]:  */
        unsigned int  reserved_16         : 1;  /* bit[26]:  */
        unsigned int  gt_clk_asp_tcxo     : 1;  /* bit[27]:  */
        unsigned int  gt_pclk_bbpdrx      : 1;  /* bit[28]:  */
        unsigned int  reserved_17         : 1;  /* bit[29]:  */
        unsigned int  reserved_18         : 1;  /* bit[30]:  */
        unsigned int  reserved_19         : 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SCPERDIS0_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_rtc_START          (1)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_rtc_END            (1)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_timer0_START       (3)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_timer0_END         (3)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_mad_32k_START       (5)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_mad_32k_END         (5)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_fll_test_src_START  (6)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_fll_test_src_END    (6)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_ao_ioc_START       (15)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_ao_ioc_END         (15)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_blpwm2_START       (16)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_blpwm2_END         (16)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_mad_acpu_START      (17)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_mad_acpu_END        (17)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_jtag_auth_START     (18)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_jtag_auth_END       (18)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_syscnt_START       (19)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_syscnt_END         (19)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_syscnt_START        (20)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_syscnt_END          (20)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_asp_tcxo_START      (27)
#define SOC_AO_CRG_SCPERDIS0_gt_clk_asp_tcxo_END        (27)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_bbpdrx_START       (28)
#define SOC_AO_CRG_SCPERDIS0_gt_pclk_bbpdrx_END         (28)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN0_UNION
 struct description   : SCPERCLKEN0 Register structure definition
                        Address Offset:0x168 Initial:0x1 Width:32
 register description : 外设时钟使能状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0          : 1;  /* bit[0] : 外设时钟使能状态：
                                                            0：IP时钟使能撤销状态；
                                                            1：IP时钟使能状态。 */
        unsigned int  gt_pclk_rtc         : 1;  /* bit[1] :  */
        unsigned int  reserved_1          : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_timer0      : 1;  /* bit[3] :  */
        unsigned int  reserved_2          : 1;  /* bit[4] :  */
        unsigned int  gt_clk_mad_32k      : 1;  /* bit[5] :  */
        unsigned int  gt_clk_fll_test_src : 1;  /* bit[6] :  */
        unsigned int  reserved_3          : 1;  /* bit[7] :  */
        unsigned int  reserved_4          : 1;  /* bit[8] :  */
        unsigned int  reserved_5          : 1;  /* bit[9] :  */
        unsigned int  reserved_6          : 1;  /* bit[10]:  */
        unsigned int  reserved_7          : 1;  /* bit[11]:  */
        unsigned int  reserved_8          : 1;  /* bit[12]:  */
        unsigned int  reserved_9          : 1;  /* bit[13]:  */
        unsigned int  reserved_10         : 1;  /* bit[14]:  */
        unsigned int  gt_pclk_ao_ioc      : 1;  /* bit[15]:  */
        unsigned int  gt_pclk_blpwm2      : 1;  /* bit[16]:  */
        unsigned int  gt_clk_mad_acpu     : 1;  /* bit[17]:  */
        unsigned int  gt_clk_jtag_auth    : 1;  /* bit[18]:  */
        unsigned int  gt_pclk_syscnt      : 1;  /* bit[19]:  */
        unsigned int  gt_clk_syscnt       : 1;  /* bit[20]:  */
        unsigned int  reserved_11         : 1;  /* bit[21]:  */
        unsigned int  reserved_12         : 1;  /* bit[22]:  */
        unsigned int  reserved_13         : 1;  /* bit[23]:  */
        unsigned int  reserved_14         : 1;  /* bit[24]:  */
        unsigned int  reserved_15         : 1;  /* bit[25]:  */
        unsigned int  reserved_16         : 1;  /* bit[26]:  */
        unsigned int  gt_clk_asp_tcxo     : 1;  /* bit[27]:  */
        unsigned int  gt_pclk_bbpdrx      : 1;  /* bit[28]:  */
        unsigned int  reserved_17         : 1;  /* bit[29]:  */
        unsigned int  reserved_18         : 1;  /* bit[30]:  */
        unsigned int  reserved_19         : 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SCPERCLKEN0_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_rtc_START          (1)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_rtc_END            (1)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_timer0_START       (3)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_timer0_END         (3)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_mad_32k_START       (5)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_mad_32k_END         (5)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_fll_test_src_START  (6)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_fll_test_src_END    (6)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_ao_ioc_START       (15)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_ao_ioc_END         (15)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_blpwm2_START       (16)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_blpwm2_END         (16)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_mad_acpu_START      (17)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_mad_acpu_END        (17)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_jtag_auth_START     (18)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_jtag_auth_END       (18)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_syscnt_START       (19)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_syscnt_END         (19)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_syscnt_START        (20)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_syscnt_END          (20)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_asp_tcxo_START      (27)
#define SOC_AO_CRG_SCPERCLKEN0_gt_clk_asp_tcxo_END        (27)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_bbpdrx_START       (28)
#define SOC_AO_CRG_SCPERCLKEN0_gt_pclk_bbpdrx_END         (28)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT0_UNION
 struct description   : SCPERSTAT0 Register structure definition
                        Address Offset:0x16C Initial:0x1 Width:32
 register description : 外设时钟最终状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_ref_crc    : 1;  /* bit[0] : 外设时钟最终状态：
                                                          0：IP时钟禁止状态；
                                                          1：IP时钟使能状态。 */
        unsigned int  st_pclk_rtc       : 1;  /* bit[1] :  */
        unsigned int  st_pclk_rtc1      : 1;  /* bit[2] :  */
        unsigned int  st_pclk_timer0    : 1;  /* bit[3] :  */
        unsigned int  reserved_0        : 1;  /* bit[4] :  */
        unsigned int  st_pclk_timer1    : 1;  /* bit[5] :  */
        unsigned int  reserved_1        : 1;  /* bit[6] :  */
        unsigned int  st_pclk_timer2    : 1;  /* bit[7] :  */
        unsigned int  reserved_2        : 1;  /* bit[8] :  */
        unsigned int  st_pclk_timer3    : 1;  /* bit[9] :  */
        unsigned int  reserved_3        : 1;  /* bit[10]:  */
        unsigned int  aupll_sscg_idle   : 1;  /* bit[11]:  */
        unsigned int  spll_sscg_idle    : 1;  /* bit[12]:  */
        unsigned int  st_clk_pcie0_aux  : 1;  /* bit[13]:  */
        unsigned int  reserved_4        : 1;  /* bit[14]: st_clk_pcie1_aux */
        unsigned int  st_pclk_ao_ioc    : 1;  /* bit[15]:  */
        unsigned int  st_pclk_blpwm2    : 1;  /* bit[16]:  */
        unsigned int  st_clk_out0       : 1;  /* bit[17]:  */
        unsigned int  st_clk_jtag_auth  : 1;  /* bit[18]:  */
        unsigned int  st_pclk_syscnt    : 1;  /* bit[19]:  */
        unsigned int  st_clk_syscnt     : 1;  /* bit[20]:  */
        unsigned int  st_clk_out2       : 1;  /* bit[21]:  */
        unsigned int  reserved_5        : 1;  /* bit[22]:  */
        unsigned int  st_clk_sci0       : 1;  /* bit[23]:  */
        unsigned int  st_clk_sci1       : 1;  /* bit[24]:  */
        unsigned int  st_clk_mad        : 1;  /* bit[25]:  */
        unsigned int  st_clk_asp_subsys : 1;  /* bit[26]:  */
        unsigned int  st_clk_asp_tcxo   : 1;  /* bit[27]:  */
        unsigned int  st_pclk_bbpdrx    : 1;  /* bit[28]:  */
        unsigned int  reserved_6        : 1;  /* bit[29]:  */
        unsigned int  st_pclk_efusec    : 1;  /* bit[30]:  */
        unsigned int  st_clk_spll_sscg  : 1;  /* bit[31]: 整个寄存器的复位值由SCTRL的输入信号peri_clk_stat0[31:0]的复位值来定。 */
    } reg;
} SOC_AO_CRG_SCPERSTAT0_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT0_st_clk_ref_crc_START     (0)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_ref_crc_END       (0)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_rtc_START        (1)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_rtc_END          (1)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_rtc1_START       (2)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_rtc1_END         (2)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer0_START     (3)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer0_END       (3)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer1_START     (5)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer1_END       (5)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer2_START     (7)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer2_END       (7)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer3_START     (9)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_timer3_END       (9)
#define SOC_AO_CRG_SCPERSTAT0_aupll_sscg_idle_START    (11)
#define SOC_AO_CRG_SCPERSTAT0_aupll_sscg_idle_END      (11)
#define SOC_AO_CRG_SCPERSTAT0_spll_sscg_idle_START     (12)
#define SOC_AO_CRG_SCPERSTAT0_spll_sscg_idle_END       (12)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_pcie0_aux_START   (13)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_pcie0_aux_END     (13)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_ao_ioc_START     (15)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_ao_ioc_END       (15)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_blpwm2_START     (16)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_blpwm2_END       (16)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_out0_START        (17)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_out0_END          (17)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_jtag_auth_START   (18)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_jtag_auth_END     (18)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_syscnt_START     (19)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_syscnt_END       (19)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_syscnt_START      (20)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_syscnt_END        (20)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_out2_START        (21)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_out2_END          (21)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_sci0_START        (23)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_sci0_END          (23)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_sci1_START        (24)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_sci1_END          (24)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_mad_START         (25)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_mad_END           (25)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_asp_subsys_START  (26)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_asp_subsys_END    (26)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_asp_tcxo_START    (27)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_asp_tcxo_END      (27)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_bbpdrx_START     (28)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_bbpdrx_END       (28)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_efusec_START     (30)
#define SOC_AO_CRG_SCPERSTAT0_st_pclk_efusec_END       (30)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_spll_sscg_START   (31)
#define SOC_AO_CRG_SCPERSTAT0_st_clk_spll_sscg_END     (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN1_UNION
 struct description   : SCPEREN1 Register structure definition
                        Address Offset:0x170 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_asp_dw_axi                : 1;  /* bit[0] : 外设时钟使能控制：
                                                                         0：写0无效；
                                                                         1：写1使能IP时钟。 */
        unsigned int  gt_clk_asp_x2h                   : 1;  /* bit[1] :  */
        unsigned int  gt_clk_asp_h2p                   : 1;  /* bit[2] :  */
        unsigned int  gt_clk_asp_cfg                   : 1;  /* bit[3] :  */
        unsigned int  gt_clk_asp_subsys_acpu           : 1;  /* bit[4] :  */
        unsigned int  gt_clk_em_sync_top               : 1;  /* bit[5] :  */
        unsigned int  gt_clk_ao_camera                 : 1;  /* bit[6] :  */
        unsigned int  reserved_0                       : 1;  /* bit[7] :  */
        unsigned int  gt_clk_hsdt0_pcie_aux            : 1;  /* bit[8] :  */
        unsigned int  gt_clk_fll_src_tp                : 1;  /* bit[9] :  */
        unsigned int  gt_pclk_timer4                   : 1;  /* bit[10]:  */
        unsigned int  reserved_1                       : 1;  /* bit[11]:  */
        unsigned int  gt_pclk_timer5                   : 1;  /* bit[12]:  */
        unsigned int  reserved_2                       : 1;  /* bit[13]:  */
        unsigned int  gt_pclk_timer6                   : 1;  /* bit[14]:  */
        unsigned int  reserved_3                       : 1;  /* bit[15]:  */
        unsigned int  reserved_4                       : 1;  /* bit[16]:  */
        unsigned int  reserved_5                       : 1;  /* bit[17]:  */
        unsigned int  gt_pclk_timer8                   : 1;  /* bit[18]:  */
        unsigned int  gt_clk_pcie0_aux                 : 1;  /* bit[19]:  */
        unsigned int  reserved_6                       : 1;  /* bit[20]: gt_clk_pcie1_aux */
        unsigned int  reserved_7                       : 1;  /* bit[21]:  */
        unsigned int  reserved_8                       : 1;  /* bit[22]:  */
        unsigned int  reserved_9                       : 1;  /* bit[23]:  */
        unsigned int  gt_clk_noc_aobus2aspbus          : 1;  /* bit[24]:  */
        unsigned int  reserved_10                      : 1;  /* bit[25]:  */
        unsigned int  reserved_11                      : 1;  /* bit[26]:  */
        unsigned int  gt_clk_asp_codec_acpu            : 1;  /* bit[27]:  */
        unsigned int  reserved_12                      : 1;  /* bit[28]:  */
        unsigned int  reserved_13                      : 1;  /* bit[29]:  */
        unsigned int  reserved_14                      : 1;  /* bit[30]:  */
        unsigned int  wait_ddr_selfreflash_done_bypass : 1;  /* bit[31]: [31]:wait_ddr_selfreflash_done_bypass
                                                                         0：写0无效；
                                                                         1：写1 done bypass。 */
    } reg;
} SOC_AO_CRG_SCPEREN1_UNION;
#endif
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_dw_axi_START                 (0)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_dw_axi_END                   (0)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_x2h_START                    (1)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_x2h_END                      (1)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_h2p_START                    (2)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_h2p_END                      (2)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_cfg_START                    (3)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_cfg_END                      (3)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_subsys_acpu_START            (4)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_subsys_acpu_END              (4)
#define SOC_AO_CRG_SCPEREN1_gt_clk_em_sync_top_START                (5)
#define SOC_AO_CRG_SCPEREN1_gt_clk_em_sync_top_END                  (5)
#define SOC_AO_CRG_SCPEREN1_gt_clk_ao_camera_START                  (6)
#define SOC_AO_CRG_SCPEREN1_gt_clk_ao_camera_END                    (6)
#define SOC_AO_CRG_SCPEREN1_gt_clk_hsdt0_pcie_aux_START             (8)
#define SOC_AO_CRG_SCPEREN1_gt_clk_hsdt0_pcie_aux_END               (8)
#define SOC_AO_CRG_SCPEREN1_gt_clk_fll_src_tp_START                 (9)
#define SOC_AO_CRG_SCPEREN1_gt_clk_fll_src_tp_END                   (9)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer4_START                    (10)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer4_END                      (10)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer5_START                    (12)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer5_END                      (12)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer6_START                    (14)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer6_END                      (14)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer8_START                    (18)
#define SOC_AO_CRG_SCPEREN1_gt_pclk_timer8_END                      (18)
#define SOC_AO_CRG_SCPEREN1_gt_clk_pcie0_aux_START                  (19)
#define SOC_AO_CRG_SCPEREN1_gt_clk_pcie0_aux_END                    (19)
#define SOC_AO_CRG_SCPEREN1_gt_clk_noc_aobus2aspbus_START           (24)
#define SOC_AO_CRG_SCPEREN1_gt_clk_noc_aobus2aspbus_END             (24)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_codec_acpu_START             (27)
#define SOC_AO_CRG_SCPEREN1_gt_clk_asp_codec_acpu_END               (27)
#define SOC_AO_CRG_SCPEREN1_wait_ddr_selfreflash_done_bypass_START  (31)
#define SOC_AO_CRG_SCPEREN1_wait_ddr_selfreflash_done_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS1_UNION
 struct description   : SCPERDIS1 Register structure definition
                        Address Offset:0x174 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_asp_dw_axi                : 1;  /* bit[0] : 外设时钟禁止控制：
                                                                         0：写0无效；
                                                                         1：写1禁止IP时钟。 */
        unsigned int  gt_clk_asp_x2h                   : 1;  /* bit[1] :  */
        unsigned int  gt_clk_asp_h2p                   : 1;  /* bit[2] :  */
        unsigned int  gt_clk_asp_cfg                   : 1;  /* bit[3] :  */
        unsigned int  gt_clk_asp_subsys_acpu           : 1;  /* bit[4] :  */
        unsigned int  gt_clk_em_sync_top               : 1;  /* bit[5] :  */
        unsigned int  gt_clk_ao_camera                 : 1;  /* bit[6] :  */
        unsigned int  reserved_0                       : 1;  /* bit[7] :  */
        unsigned int  gt_clk_hsdt0_pcie_aux            : 1;  /* bit[8] :  */
        unsigned int  gt_clk_fll_src_tp                : 1;  /* bit[9] :  */
        unsigned int  gt_pclk_timer4                   : 1;  /* bit[10]:  */
        unsigned int  reserved_1                       : 1;  /* bit[11]:  */
        unsigned int  gt_pclk_timer5                   : 1;  /* bit[12]:  */
        unsigned int  reserved_2                       : 1;  /* bit[13]:  */
        unsigned int  gt_pclk_timer6                   : 1;  /* bit[14]:  */
        unsigned int  reserved_3                       : 1;  /* bit[15]:  */
        unsigned int  reserved_4                       : 1;  /* bit[16]:  */
        unsigned int  reserved_5                       : 1;  /* bit[17]:  */
        unsigned int  gt_pclk_timer8                   : 1;  /* bit[18]:  */
        unsigned int  gt_clk_pcie0_aux                 : 1;  /* bit[19]:  */
        unsigned int  reserved_6                       : 1;  /* bit[20]:  */
        unsigned int  reserved_7                       : 1;  /* bit[21]:  */
        unsigned int  reserved_8                       : 1;  /* bit[22]:  */
        unsigned int  reserved_9                       : 1;  /* bit[23]:  */
        unsigned int  gt_clk_noc_aobus2aspbus          : 1;  /* bit[24]:  */
        unsigned int  reserved_10                      : 1;  /* bit[25]:  */
        unsigned int  reserved_11                      : 1;  /* bit[26]:  */
        unsigned int  gt_clk_asp_codec_acpu            : 1;  /* bit[27]:  */
        unsigned int  reserved_12                      : 1;  /* bit[28]:  */
        unsigned int  reserved_13                      : 1;  /* bit[29]:  */
        unsigned int  reserved_14                      : 1;  /* bit[30]:  */
        unsigned int  wait_ddr_selfreflash_done_bypass : 1;  /* bit[31]: [31]:wait_ddr_selfreflash_done_bypass
                                                                         0：写0无效；
                                                                         1：写1 done not bypass。 */
    } reg;
} SOC_AO_CRG_SCPERDIS1_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_dw_axi_START                 (0)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_dw_axi_END                   (0)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_x2h_START                    (1)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_x2h_END                      (1)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_h2p_START                    (2)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_h2p_END                      (2)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_cfg_START                    (3)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_cfg_END                      (3)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_subsys_acpu_START            (4)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_subsys_acpu_END              (4)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_em_sync_top_START                (5)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_em_sync_top_END                  (5)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_ao_camera_START                  (6)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_ao_camera_END                    (6)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_hsdt0_pcie_aux_START             (8)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_hsdt0_pcie_aux_END               (8)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_fll_src_tp_START                 (9)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_fll_src_tp_END                   (9)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer4_START                    (10)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer4_END                      (10)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer5_START                    (12)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer5_END                      (12)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer6_START                    (14)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer6_END                      (14)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer8_START                    (18)
#define SOC_AO_CRG_SCPERDIS1_gt_pclk_timer8_END                      (18)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_pcie0_aux_START                  (19)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_pcie0_aux_END                    (19)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_noc_aobus2aspbus_START           (24)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_noc_aobus2aspbus_END             (24)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_codec_acpu_START             (27)
#define SOC_AO_CRG_SCPERDIS1_gt_clk_asp_codec_acpu_END               (27)
#define SOC_AO_CRG_SCPERDIS1_wait_ddr_selfreflash_done_bypass_START  (31)
#define SOC_AO_CRG_SCPERDIS1_wait_ddr_selfreflash_done_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN1_UNION
 struct description   : SCPERCLKEN1 Register structure definition
                        Address Offset:0x178 Initial:0x1 Width:32
 register description : 外设时钟使能状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_asp_dw_axi                : 1;  /* bit[0] : 外设时钟使能状态：
                                                                         0：IP时钟禁止状态；
                                                                         1：IP时钟使能状态。 */
        unsigned int  gt_clk_asp_x2h                   : 1;  /* bit[1] :  */
        unsigned int  gt_clk_asp_h2p                   : 1;  /* bit[2] :  */
        unsigned int  gt_clk_asp_cfg                   : 1;  /* bit[3] :  */
        unsigned int  gt_clk_asp_subsys_acpu           : 1;  /* bit[4] :  */
        unsigned int  gt_clk_em_sync_top               : 1;  /* bit[5] :  */
        unsigned int  gt_clk_ao_camera                 : 1;  /* bit[6] :  */
        unsigned int  reserved_0                       : 1;  /* bit[7] :  */
        unsigned int  gt_clk_hsdt0_pcie_aux            : 1;  /* bit[8] :  */
        unsigned int  gt_clk_fll_src_tp                : 1;  /* bit[9] :  */
        unsigned int  gt_pclk_timer4                   : 1;  /* bit[10]:  */
        unsigned int  reserved_1                       : 1;  /* bit[11]:  */
        unsigned int  gt_pclk_timer5                   : 1;  /* bit[12]:  */
        unsigned int  reserved_2                       : 1;  /* bit[13]:  */
        unsigned int  gt_pclk_timer6                   : 1;  /* bit[14]:  */
        unsigned int  reserved_3                       : 1;  /* bit[15]:  */
        unsigned int  reserved_4                       : 1;  /* bit[16]:  */
        unsigned int  reserved_5                       : 1;  /* bit[17]:  */
        unsigned int  gt_pclk_timer8                   : 1;  /* bit[18]:  */
        unsigned int  gt_clk_pcie0_aux                 : 1;  /* bit[19]:  */
        unsigned int  reserved_6                       : 1;  /* bit[20]:  */
        unsigned int  reserved_7                       : 1;  /* bit[21]:  */
        unsigned int  reserved_8                       : 1;  /* bit[22]:  */
        unsigned int  reserved_9                       : 1;  /* bit[23]:  */
        unsigned int  gt_clk_noc_aobus2aspbus          : 1;  /* bit[24]:  */
        unsigned int  reserved_10                      : 1;  /* bit[25]:  */
        unsigned int  reserved_11                      : 1;  /* bit[26]:  */
        unsigned int  gt_clk_asp_codec_acpu            : 1;  /* bit[27]:  */
        unsigned int  reserved_12                      : 1;  /* bit[28]:  */
        unsigned int  reserved_13                      : 1;  /* bit[29]:  */
        unsigned int  reserved_14                      : 1;  /* bit[30]:  */
        unsigned int  wait_ddr_selfreflash_done_bypass : 1;  /* bit[31]: [31]:wait_ddr_selfreflash_done_bypass
                                                                         0:no bypass
                                                                         1:bypass。 */
    } reg;
} SOC_AO_CRG_SCPERCLKEN1_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_dw_axi_START                 (0)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_dw_axi_END                   (0)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_x2h_START                    (1)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_x2h_END                      (1)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_h2p_START                    (2)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_h2p_END                      (2)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_cfg_START                    (3)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_cfg_END                      (3)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_subsys_acpu_START            (4)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_subsys_acpu_END              (4)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_em_sync_top_START                (5)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_em_sync_top_END                  (5)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_ao_camera_START                  (6)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_ao_camera_END                    (6)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_hsdt0_pcie_aux_START             (8)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_hsdt0_pcie_aux_END               (8)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_fll_src_tp_START                 (9)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_fll_src_tp_END                   (9)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer4_START                    (10)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer4_END                      (10)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer5_START                    (12)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer5_END                      (12)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer6_START                    (14)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer6_END                      (14)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer8_START                    (18)
#define SOC_AO_CRG_SCPERCLKEN1_gt_pclk_timer8_END                      (18)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_pcie0_aux_START                  (19)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_pcie0_aux_END                    (19)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_noc_aobus2aspbus_START           (24)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_noc_aobus2aspbus_END             (24)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_codec_acpu_START             (27)
#define SOC_AO_CRG_SCPERCLKEN1_gt_clk_asp_codec_acpu_END               (27)
#define SOC_AO_CRG_SCPERCLKEN1_wait_ddr_selfreflash_done_bypass_START  (31)
#define SOC_AO_CRG_SCPERCLKEN1_wait_ddr_selfreflash_done_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT1_UNION
 struct description   : SCPERSTAT1 Register structure definition
                        Address Offset:0x17C Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_asp_dw_axi            : 1;  /* bit[0]    : 外设时钟最终状态：
                                                                        0：IP时钟禁止状态；
                                                                        1：IP时钟使能状态。 */
        unsigned int  st_clk_asp_x2h               : 1;  /* bit[1]    :  */
        unsigned int  st_clk_asp_h2p               : 1;  /* bit[2]    :  */
        unsigned int  st_clk_asp_cfg               : 1;  /* bit[3]    :  */
        unsigned int  st_clk_mad_axi               : 1;  /* bit[4]    :  */
        unsigned int  st_clk_noc_aobus2sysbusddrc  : 1;  /* bit[5]    :  */
        unsigned int  st_clk_ao_camera             : 1;  /* bit[6]    :  */
        unsigned int  reserved_0                   : 1;  /* bit[7]    :  */
        unsigned int  st_clk_mad_32k               : 1;  /* bit[8]    :  */
        unsigned int  st_clk_fll_src_tp            : 1;  /* bit[9]    :  */
        unsigned int  st_pclk_timer4               : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                   : 1;  /* bit[11]   :  */
        unsigned int  st_pclk_timer5               : 1;  /* bit[12]   :  */
        unsigned int  reserved_2                   : 1;  /* bit[13]   :  */
        unsigned int  st_pclk_timer6               : 1;  /* bit[14]   :  */
        unsigned int  reserved_3                   : 1;  /* bit[15]   :  */
        unsigned int  st_pclk_timer7               : 1;  /* bit[16]   :  */
        unsigned int  reserved_4                   : 1;  /* bit[17]   :  */
        unsigned int  st_pclk_timer8               : 1;  /* bit[18]   :  */
        unsigned int  reserved_5                   : 1;  /* bit[19]   :  */
        unsigned int  st_timerclk_refh             : 1;  /* bit[20]   :  */
        unsigned int  st_clk_em_sync_top           : 1;  /* bit[21]   :  */
        unsigned int  swdone_clk_iomcu_fll_src_div : 1;  /* bit[22]   :  */
        unsigned int  swdone_clk_aobus_div         : 1;  /* bit[23]   : clk_aobus DIV分频比切换是否完成指示信号：
                                                                        0：未完成；
                                                                        1：完成。 */
        unsigned int  st_clk_noc_aobus2aspbus      : 1;  /* bit[24]   :  */
        unsigned int  st_clk_noc_aobus2iomcubus    : 1;  /* bit[25]   : 0：时钟关闭；
                                                                        1：时钟开启。 */
        unsigned int  st_clk_hsdt_subsys           : 1;  /* bit[26]   :  */
        unsigned int  swdone_clk_asp_subsys_div    : 1;  /* bit[27]   : clk_asp_subsysDIV分频比切换是否完成指示信号：
                                                                        0：未完成；
                                                                        1：完成。 */
        unsigned int  swdone_clk_sci_div           : 1;  /* bit[28]   : clk_sci DIV分频比切换是否完成指示信号：
                                                                        0：未完成；
                                                                        1：完成。 */
        unsigned int  sw_ack_clk_aobus_sw          : 3;  /* bit[29-31]: aobus当前时钟源选择指示。
                                                                        3'b001: 32k
                                                                        3'b010: clksys
                                                                        3'b100: ppll0 */
    } reg;
} SOC_AO_CRG_SCPERSTAT1_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_dw_axi_START             (0)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_dw_axi_END               (0)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_x2h_START                (1)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_x2h_END                  (1)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_h2p_START                (2)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_h2p_END                  (2)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_cfg_START                (3)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_asp_cfg_END                  (3)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_mad_axi_START                (4)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_mad_axi_END                  (4)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_noc_aobus2sysbusddrc_START   (5)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_noc_aobus2sysbusddrc_END     (5)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_ao_camera_START              (6)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_ao_camera_END                (6)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_mad_32k_START                (8)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_mad_32k_END                  (8)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_fll_src_tp_START             (9)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_fll_src_tp_END               (9)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer4_START                (10)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer4_END                  (10)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer5_START                (12)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer5_END                  (12)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer6_START                (14)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer6_END                  (14)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer7_START                (16)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer7_END                  (16)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer8_START                (18)
#define SOC_AO_CRG_SCPERSTAT1_st_pclk_timer8_END                  (18)
#define SOC_AO_CRG_SCPERSTAT1_st_timerclk_refh_START              (20)
#define SOC_AO_CRG_SCPERSTAT1_st_timerclk_refh_END                (20)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_em_sync_top_START            (21)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_em_sync_top_END              (21)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_iomcu_fll_src_div_START  (22)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_iomcu_fll_src_div_END    (22)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_aobus_div_START          (23)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_aobus_div_END            (23)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_noc_aobus2aspbus_START       (24)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_noc_aobus2aspbus_END         (24)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_noc_aobus2iomcubus_START     (25)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_noc_aobus2iomcubus_END       (25)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_hsdt_subsys_START            (26)
#define SOC_AO_CRG_SCPERSTAT1_st_clk_hsdt_subsys_END              (26)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_asp_subsys_div_START     (27)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_asp_subsys_div_END       (27)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_sci_div_START            (28)
#define SOC_AO_CRG_SCPERSTAT1_swdone_clk_sci_div_END              (28)
#define SOC_AO_CRG_SCPERSTAT1_sw_ack_clk_aobus_sw_START           (29)
#define SOC_AO_CRG_SCPERSTAT1_sw_ack_clk_aobus_sw_END             (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_AO_CRG_STA_SEP0_UNION
 struct description   : SC_AO_CRG_STA_SEP0 Register structure definition
                        Address Offset:0x188 Initial:0x0000000 Width:32
 register description : AO_CRG状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0        : 1;  /* bit[0]   :  */
        unsigned int  aupll_en_stat     : 1;  /* bit[1]   : DP AUDIO专用PLL 使能状态指示信号，SCTRL回读 */
        unsigned int  aupll_bypass_stat : 1;  /* bit[2]   : DP AUDIO专用PLL bypass指示信号，SCTRL回读 */
        unsigned int  aupll_gt_stat     : 1;  /* bit[3]   : DP AUDIO专用PLL 输出门控状态指示信号，SCTRL回读 */
        unsigned int  aupll_lock        : 1;  /* bit[4]   : DP AUDIO专用PLL lock指示信号，SCTRL回读 */
        unsigned int  reserved_1        : 27; /* bit[5-31]:  */
    } reg;
} SOC_AO_CRG_SC_AO_CRG_STA_SEP0_UNION;
#endif
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_en_stat_START      (1)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_en_stat_END        (1)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_bypass_stat_START  (2)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_bypass_stat_END    (2)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_gt_stat_START      (3)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_gt_stat_END        (3)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_lock_START         (4)
#define SOC_AO_CRG_SC_AO_CRG_STA_SEP0_aupll_lock_END           (4)


/*****************************************************************************
 struct               : SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_UNION
 struct description   : AO_AUTODIV_INUSE_STAT Register structure definition
                        Address Offset:0x18C Initial:0x000 Width:32
 register description : 自动降频状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aoqic_sys_idle_flag : 1;  /* bit[0]    : 维测用 */
        unsigned int  aoqic_cc_idle_flag  : 1;  /* bit[1]    :  */
        unsigned int  autodiv_pcie_stat   : 1;  /* bit[2]    :  */
        unsigned int  reserved_0          : 1;  /* bit[3]    :  */
        unsigned int  reserved_1          : 1;  /* bit[4]    :  */
        unsigned int  reserved_2          : 1;  /* bit[5]    :  */
        unsigned int  reserved_3          : 1;  /* bit[6]    :  */
        unsigned int  reserved_4          : 1;  /* bit[7]    :  */
        unsigned int  reserved_5          : 1;  /* bit[8]    :  */
        unsigned int  reserved_6          : 1;  /* bit[9]    :  */
        unsigned int  reserved_7          : 1;  /* bit[10]   :  */
        unsigned int  reserved_8          : 1;  /* bit[11]   :  */
        unsigned int  reserved_9          : 1;  /* bit[12]   :  */
        unsigned int  reserved_10         : 1;  /* bit[13]   :  */
        unsigned int  reserved_11         : 1;  /* bit[14]   :  */
        unsigned int  reserved_12         : 1;  /* bit[15]   :  */
        unsigned int  reserved_13         : 1;  /* bit[16]   :  */
        unsigned int  reserved_14         : 1;  /* bit[17]   :  */
        unsigned int  reserved_15         : 1;  /* bit[18]   :  */
        unsigned int  reserved_16         : 1;  /* bit[19]   :  */
        unsigned int  reserved_17         : 1;  /* bit[20]   :  */
        unsigned int  reserved_18         : 1;  /* bit[21]   :  */
        unsigned int  reserved_19         : 10; /* bit[22-31]: 保留 */
    } reg;
} SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_UNION;
#endif
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_aoqic_sys_idle_flag_START  (0)
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_aoqic_sys_idle_flag_END    (0)
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_aoqic_cc_idle_flag_START   (1)
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_aoqic_cc_idle_flag_END     (1)
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_autodiv_pcie_stat_START    (2)
#define SOC_AO_CRG_AO_AUTODIV_INUSE_STAT_autodiv_pcie_stat_END      (2)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN2_UNION
 struct description   : SCPEREN2 Register structure definition
                        Address Offset:0x190 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]    : 外设时钟禁止控制：
                                                              0：写0无效；1：写1使能IP时钟。 */
        unsigned int  reserved_1         : 1;  /* bit[1]    :  */
        unsigned int  reserved_2         : 1;  /* bit[2]    :  */
        unsigned int  reserved_3         : 1;  /* bit[3]    :  */
        unsigned int  reserved_4         : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_isp_snclk3  : 1;  /* bit[5]    : xian新增 */
        unsigned int  gt_clk_isp_snclk2  : 1;  /* bit[6]    : xian新增 */
        unsigned int  gt_clk_isp_snclk0  : 1;  /* bit[7]    : xian新增 */
        unsigned int  reserved_5         : 1;  /* bit[8]    :  */
        unsigned int  reserved_6         : 1;  /* bit[9]    :  */
        unsigned int  reserved_7         : 1;  /* bit[10]   :  */
        unsigned int  gt_clk_hsdt_subsys : 6;  /* bit[11-16]:  */
        unsigned int  reserved_8         : 1;  /* bit[17]   :  */
        unsigned int  reserved_9         : 1;  /* bit[18]   :  */
        unsigned int  reserved_10        : 1;  /* bit[19]   :  */
        unsigned int  reserved_11        : 1;  /* bit[20]   :  */
        unsigned int  reserved_12        : 1;  /* bit[21]   :  */
        unsigned int  reserved_13        : 1;  /* bit[22]   :  */
        unsigned int  reserved_14        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15        : 1;  /* bit[24]   :  */
        unsigned int  reserved_16        : 1;  /* bit[25]   :  */
        unsigned int  reserved_17        : 1;  /* bit[26]   : gt_clk_ao_tof */
        unsigned int  reserved_18        : 1;  /* bit[27]   :  */
        unsigned int  reserved_19        : 1;  /* bit[28]   :  */
        unsigned int  reserved_20        : 1;  /* bit[29]   :  */
        unsigned int  reserved_21        : 1;  /* bit[30]   :  */
        unsigned int  gt_clk_mad_axi     : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPEREN2_UNION;
#endif
#define SOC_AO_CRG_SCPEREN2_gt_clk_isp_snclk3_START   (5)
#define SOC_AO_CRG_SCPEREN2_gt_clk_isp_snclk3_END     (5)
#define SOC_AO_CRG_SCPEREN2_gt_clk_isp_snclk2_START   (6)
#define SOC_AO_CRG_SCPEREN2_gt_clk_isp_snclk2_END     (6)
#define SOC_AO_CRG_SCPEREN2_gt_clk_isp_snclk0_START   (7)
#define SOC_AO_CRG_SCPEREN2_gt_clk_isp_snclk0_END     (7)
#define SOC_AO_CRG_SCPEREN2_gt_clk_hsdt_subsys_START  (11)
#define SOC_AO_CRG_SCPEREN2_gt_clk_hsdt_subsys_END    (16)
#define SOC_AO_CRG_SCPEREN2_gt_clk_mad_axi_START      (31)
#define SOC_AO_CRG_SCPEREN2_gt_clk_mad_axi_END        (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS2_UNION
 struct description   : SCPERDIS2 Register structure definition
                        Address Offset:0x194 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]    : 外设时钟禁止控制：
                                                              0：写0无效；
                                                              1：写1关闭IP时钟。 */
        unsigned int  reserved_1         : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  reserved_2         : 1;  /* bit[2]    :  */
        unsigned int  reserved_3         : 1;  /* bit[3]    :  */
        unsigned int  reserved_4         : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_isp_snclk3  : 1;  /* bit[5]    :  */
        unsigned int  gt_clk_isp_snclk2  : 1;  /* bit[6]    :  */
        unsigned int  gt_clk_isp_snclk0  : 1;  /* bit[7]    :  */
        unsigned int  reserved_5         : 1;  /* bit[8]    :  */
        unsigned int  reserved_6         : 1;  /* bit[9]    :  */
        unsigned int  reserved_7         : 1;  /* bit[10]   :  */
        unsigned int  gt_clk_hsdt_subsys : 6;  /* bit[11-16]:  */
        unsigned int  reserved_8         : 1;  /* bit[17]   :  */
        unsigned int  reserved_9         : 1;  /* bit[18]   :  */
        unsigned int  reserved_10        : 1;  /* bit[19]   :  */
        unsigned int  reserved_11        : 1;  /* bit[20]   :  */
        unsigned int  reserved_12        : 1;  /* bit[21]   :  */
        unsigned int  reserved_13        : 1;  /* bit[22]   :  */
        unsigned int  reserved_14        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15        : 1;  /* bit[24]   :  */
        unsigned int  reserved_16        : 1;  /* bit[25]   :  */
        unsigned int  reserved_17        : 1;  /* bit[26]   :  */
        unsigned int  reserved_18        : 1;  /* bit[27]   :  */
        unsigned int  reserved_19        : 1;  /* bit[28]   :  */
        unsigned int  reserved_20        : 1;  /* bit[29]   :  */
        unsigned int  reserved_21        : 1;  /* bit[30]   :  */
        unsigned int  gt_clk_mad_axi     : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERDIS2_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS2_gt_clk_isp_snclk3_START   (5)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_isp_snclk3_END     (5)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_isp_snclk2_START   (6)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_isp_snclk2_END     (6)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_isp_snclk0_START   (7)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_isp_snclk0_END     (7)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_hsdt_subsys_START  (11)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_hsdt_subsys_END    (16)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_mad_axi_START      (31)
#define SOC_AO_CRG_SCPERDIS2_gt_clk_mad_axi_END        (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN2_UNION
 struct description   : SCPERCLKEN2 Register structure definition
                        Address Offset:0x198 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]    : 外设时钟状态：
                                                              0：时钟禁止；
                                                              1：时钟使能。 */
        unsigned int  reserved_1         : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  reserved_2         : 1;  /* bit[2]    :  */
        unsigned int  reserved_3         : 1;  /* bit[3]    :  */
        unsigned int  reserved_4         : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_isp_snclk3  : 1;  /* bit[5]    :  */
        unsigned int  gt_clk_isp_snclk2  : 1;  /* bit[6]    :  */
        unsigned int  gt_clk_isp_snclk0  : 1;  /* bit[7]    :  */
        unsigned int  reserved_5         : 1;  /* bit[8]    :  */
        unsigned int  reserved_6         : 1;  /* bit[9]    :  */
        unsigned int  reserved_7         : 1;  /* bit[10]   :  */
        unsigned int  gt_clk_hsdt_subsys : 6;  /* bit[11-16]:  */
        unsigned int  reserved_8         : 1;  /* bit[17]   :  */
        unsigned int  reserved_9         : 1;  /* bit[18]   :  */
        unsigned int  reserved_10        : 1;  /* bit[19]   :  */
        unsigned int  reserved_11        : 1;  /* bit[20]   :  */
        unsigned int  reserved_12        : 1;  /* bit[21]   :  */
        unsigned int  reserved_13        : 1;  /* bit[22]   :  */
        unsigned int  reserved_14        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15        : 1;  /* bit[24]   :  */
        unsigned int  reserved_16        : 1;  /* bit[25]   :  */
        unsigned int  reserved_17        : 1;  /* bit[26]   :  */
        unsigned int  reserved_18        : 1;  /* bit[27]   :  */
        unsigned int  reserved_19        : 1;  /* bit[28]   :  */
        unsigned int  reserved_20        : 1;  /* bit[29]   :  */
        unsigned int  reserved_21        : 1;  /* bit[30]   :  */
        unsigned int  gt_clk_mad_axi     : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERCLKEN2_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_isp_snclk3_START   (5)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_isp_snclk3_END     (5)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_isp_snclk2_START   (6)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_isp_snclk2_END     (6)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_isp_snclk0_START   (7)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_isp_snclk0_END     (7)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_hsdt_subsys_START  (11)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_hsdt_subsys_END    (16)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_mad_axi_START      (31)
#define SOC_AO_CRG_SCPERCLKEN2_gt_clk_mad_axi_END        (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT2_UNION
 struct description   : SCPERSTAT2 Register structure definition
                        Address Offset:0x19C Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                        : 2;  /* bit[0-1]  :  */
        unsigned int  reserved_1                        : 1;  /* bit[2]    :  */
        unsigned int  swdone_clk_iomcu_pll_div          : 1;  /* bit[3]    :  */
        unsigned int  reserved_2                        : 1;  /* bit[4]    :  */
        unsigned int  st_clk_isp_snclk3                 : 1;  /* bit[5]    : xian新增 */
        unsigned int  st_clk_isp_snclk2                 : 1;  /* bit[6]    : xian新增 */
        unsigned int  st_clk_isp_snclk0                 : 1;  /* bit[7]    : xian新增 */
        unsigned int  swdone_clk_spi3_ulppll            : 1;  /* bit[8]    :  */
        unsigned int  swdone_clk_spi3                   : 1;  /* bit[9]    :  */
        unsigned int  sw_ack_clk_asp_subsys_ini         : 4;  /* bit[10-13]: asp subsys当前时钟源选择指示。
                                                                             4'b0001: clk_asp_subsys_div
                                                                             4'b0010: clk_fll_src
                                                                             4'b0100: clk_ao_asp
                                                                             4'b1000 :clk_sys */
        unsigned int  reset_dram_n                      : 1;  /* bit[14]   :  */
        unsigned int  swdone_clk_spi5_ulppll            : 1;  /* bit[15]   :  */
        unsigned int  swdone_clk_spi5                   : 1;  /* bit[16]   :  */
        unsigned int  swdone_clk_pcie_aux_32kpll_div    : 1;  /* bit[17]   :  */
        unsigned int  st_clk_hsdt0_pcie_aux             : 1;  /* bit[18]   : clk_hsdt0_pcie_aux时钟门控状态 */
        unsigned int  swdone_clk_dp_audio_pll_ao_div    : 1;  /* bit[19]   : clk_dp_audio通路DIV分频指示信号：
                                                                             0：分频未完成；1：分频完成 */
        unsigned int  st_clk_iomcu_fll_src              : 1;  /* bit[20]   :  */
        unsigned int  st_pclk_ao_wd                     : 1;  /* bit[21]   : 新增pclk_ao_wd门控 */
        unsigned int  st_clk_iomcu_pll_div              : 1;  /* bit[22]   :  */
        unsigned int  st_clk_sif_out                    : 1;  /* bit[23]   :  */
        unsigned int  swdone_clk_mad_fll_div            : 1;  /* bit[24]   :  */
        unsigned int  st_clk_fll_test_src               : 1;  /* bit[25]   :  */
        unsigned int  st_pclkdbg_to_iomcu               : 1;  /* bit[26]   :  */
        unsigned int  st_clk_asp_codec                  : 1;  /* bit[27]   :  */
        unsigned int  swdone_clk_out0_div               : 1;  /* bit[28]   :  */
        unsigned int  swdone_clk_asp_subsys_fll_div     : 1;  /* bit[29]   :  */
        unsigned int  swdone_clk_aonoc_fll_div          : 1;  /* bit[30]   :  */
        unsigned int  swdone_clk_noc_timeout_extref_div : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERSTAT2_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_iomcu_pll_div_START           (3)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_iomcu_pll_div_END             (3)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_isp_snclk3_START                  (5)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_isp_snclk3_END                    (5)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_isp_snclk2_START                  (6)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_isp_snclk2_END                    (6)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_isp_snclk0_START                  (7)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_isp_snclk0_END                    (7)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi3_ulppll_START             (8)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi3_ulppll_END               (8)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi3_START                    (9)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi3_END                      (9)
#define SOC_AO_CRG_SCPERSTAT2_sw_ack_clk_asp_subsys_ini_START          (10)
#define SOC_AO_CRG_SCPERSTAT2_sw_ack_clk_asp_subsys_ini_END            (13)
#define SOC_AO_CRG_SCPERSTAT2_reset_dram_n_START                       (14)
#define SOC_AO_CRG_SCPERSTAT2_reset_dram_n_END                         (14)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi5_ulppll_START             (15)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi5_ulppll_END               (15)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi5_START                    (16)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_spi5_END                      (16)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_pcie_aux_32kpll_div_START     (17)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_pcie_aux_32kpll_div_END       (17)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_hsdt0_pcie_aux_START              (18)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_hsdt0_pcie_aux_END                (18)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_dp_audio_pll_ao_div_START     (19)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_dp_audio_pll_ao_div_END       (19)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_iomcu_fll_src_START               (20)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_iomcu_fll_src_END                 (20)
#define SOC_AO_CRG_SCPERSTAT2_st_pclk_ao_wd_START                      (21)
#define SOC_AO_CRG_SCPERSTAT2_st_pclk_ao_wd_END                        (21)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_iomcu_pll_div_START               (22)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_iomcu_pll_div_END                 (22)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_sif_out_START                     (23)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_sif_out_END                       (23)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_mad_fll_div_START             (24)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_mad_fll_div_END               (24)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_fll_test_src_START                (25)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_fll_test_src_END                  (25)
#define SOC_AO_CRG_SCPERSTAT2_st_pclkdbg_to_iomcu_START                (26)
#define SOC_AO_CRG_SCPERSTAT2_st_pclkdbg_to_iomcu_END                  (26)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_asp_codec_START                   (27)
#define SOC_AO_CRG_SCPERSTAT2_st_clk_asp_codec_END                     (27)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_out0_div_START                (28)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_out0_div_END                  (28)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_asp_subsys_fll_div_START      (29)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_asp_subsys_fll_div_END        (29)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_aonoc_fll_div_START           (30)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_aonoc_fll_div_END             (30)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_noc_timeout_extref_div_START  (31)
#define SOC_AO_CRG_SCPERSTAT2_swdone_clk_noc_timeout_extref_div_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN3_UNION
 struct description   : SCPEREN3 Register structure definition
                        Address Offset:0x1A0 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0] : 外设时钟禁止控制：
                                                  0：写0无效；1：写1使能IP时钟。 */
        unsigned int  reserved_1: 1;  /* bit[1] :  */
        unsigned int  reserved_2: 1;  /* bit[2] :  */
        unsigned int  reserved_3: 1;  /* bit[3] :  */
        unsigned int  reserved_4: 1;  /* bit[4] :  */
        unsigned int  reserved_5: 1;  /* bit[5] :  */
        unsigned int  reserved_6: 1;  /* bit[6] :  */
        unsigned int  reserved_7: 1;  /* bit[7] :  */
        unsigned int  reserved_8: 1;  /* bit[8] :  */
        unsigned int  reserved_9: 1;  /* bit[9] :  */
        unsigned int  reserved_10: 1;  /* bit[10]:  */
        unsigned int  reserved_11: 1;  /* bit[11]:  */
        unsigned int  reserved_12: 1;  /* bit[12]:  */
        unsigned int  reserved_13: 1;  /* bit[13]:  */
        unsigned int  reserved_14: 1;  /* bit[14]:  */
        unsigned int  reserved_15: 1;  /* bit[15]:  */
        unsigned int  reserved_16: 1;  /* bit[16]:  */
        unsigned int  reserved_17: 1;  /* bit[17]:  */
        unsigned int  reserved_18: 1;  /* bit[18]:  */
        unsigned int  reserved_19: 1;  /* bit[19]:  */
        unsigned int  reserved_20: 1;  /* bit[20]:  */
        unsigned int  reserved_21: 1;  /* bit[21]:  */
        unsigned int  reserved_22: 1;  /* bit[22]:  */
        unsigned int  reserved_23: 1;  /* bit[23]:  */
        unsigned int  reserved_24: 1;  /* bit[24]:  */
        unsigned int  reserved_25: 1;  /* bit[25]:  */
        unsigned int  reserved_26: 1;  /* bit[26]:  */
        unsigned int  reserved_27: 1;  /* bit[27]:  */
        unsigned int  reserved_28: 1;  /* bit[28]:  */
        unsigned int  reserved_29: 1;  /* bit[29]:  */
        unsigned int  reserved_30: 1;  /* bit[30]:  */
        unsigned int  reserved_31: 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SCPEREN3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS3_UNION
 struct description   : SCPERDIS3 Register structure definition
                        Address Offset:0x1A4 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0] : 外设时钟禁止控制：
                                                  0：写0无效；
                                                  1：写1关闭IP时钟。 */
        unsigned int  reserved_1: 1;  /* bit[1] :  */
        unsigned int  reserved_2: 1;  /* bit[2] :  */
        unsigned int  reserved_3: 1;  /* bit[3] :  */
        unsigned int  reserved_4: 1;  /* bit[4] :  */
        unsigned int  reserved_5: 1;  /* bit[5] :  */
        unsigned int  reserved_6: 1;  /* bit[6] :  */
        unsigned int  reserved_7: 1;  /* bit[7] :  */
        unsigned int  reserved_8: 1;  /* bit[8] :  */
        unsigned int  reserved_9: 1;  /* bit[9] :  */
        unsigned int  reserved_10: 1;  /* bit[10]:  */
        unsigned int  reserved_11: 1;  /* bit[11]:  */
        unsigned int  reserved_12: 1;  /* bit[12]:  */
        unsigned int  reserved_13: 1;  /* bit[13]:  */
        unsigned int  reserved_14: 1;  /* bit[14]:  */
        unsigned int  reserved_15: 1;  /* bit[15]:  */
        unsigned int  reserved_16: 1;  /* bit[16]:  */
        unsigned int  reserved_17: 1;  /* bit[17]:  */
        unsigned int  reserved_18: 1;  /* bit[18]:  */
        unsigned int  reserved_19: 1;  /* bit[19]:  */
        unsigned int  reserved_20: 1;  /* bit[20]:  */
        unsigned int  reserved_21: 1;  /* bit[21]:  */
        unsigned int  reserved_22: 1;  /* bit[22]:  */
        unsigned int  reserved_23: 1;  /* bit[23]:  */
        unsigned int  reserved_24: 1;  /* bit[24]:  */
        unsigned int  reserved_25: 1;  /* bit[25]:  */
        unsigned int  reserved_26: 1;  /* bit[26]:  */
        unsigned int  reserved_27: 1;  /* bit[27]:  */
        unsigned int  reserved_28: 1;  /* bit[28]:  */
        unsigned int  reserved_29: 1;  /* bit[29]:  */
        unsigned int  reserved_30: 1;  /* bit[30]:  */
        unsigned int  reserved_31: 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SCPERDIS3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN3_UNION
 struct description   : SCPERCLKEN3 Register structure definition
                        Address Offset:0x1A8 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0] : 外设时钟状态：
                                                  0：时钟禁止；
                                                  1：时钟使能。 */
        unsigned int  reserved_1: 1;  /* bit[1] :  */
        unsigned int  reserved_2: 1;  /* bit[2] :  */
        unsigned int  reserved_3: 1;  /* bit[3] :  */
        unsigned int  reserved_4: 1;  /* bit[4] :  */
        unsigned int  reserved_5: 1;  /* bit[5] :  */
        unsigned int  reserved_6: 1;  /* bit[6] :  */
        unsigned int  reserved_7: 1;  /* bit[7] :  */
        unsigned int  reserved_8: 1;  /* bit[8] :  */
        unsigned int  reserved_9: 1;  /* bit[9] :  */
        unsigned int  reserved_10: 1;  /* bit[10]:  */
        unsigned int  reserved_11: 1;  /* bit[11]:  */
        unsigned int  reserved_12: 1;  /* bit[12]:  */
        unsigned int  reserved_13: 1;  /* bit[13]:  */
        unsigned int  reserved_14: 1;  /* bit[14]:  */
        unsigned int  reserved_15: 1;  /* bit[15]:  */
        unsigned int  reserved_16: 1;  /* bit[16]:  */
        unsigned int  reserved_17: 1;  /* bit[17]:  */
        unsigned int  reserved_18: 1;  /* bit[18]:  */
        unsigned int  reserved_19: 1;  /* bit[19]:  */
        unsigned int  reserved_20: 1;  /* bit[20]:  */
        unsigned int  reserved_21: 1;  /* bit[21]:  */
        unsigned int  reserved_22: 1;  /* bit[22]:  */
        unsigned int  reserved_23: 1;  /* bit[23]:  */
        unsigned int  reserved_24: 1;  /* bit[24]:  */
        unsigned int  reserved_25: 1;  /* bit[25]:  */
        unsigned int  reserved_26: 1;  /* bit[26]:  */
        unsigned int  reserved_27: 1;  /* bit[27]:  */
        unsigned int  reserved_28: 1;  /* bit[28]:  */
        unsigned int  reserved_29: 1;  /* bit[29]:  */
        unsigned int  reserved_30: 1;  /* bit[30]:  */
        unsigned int  reserved_31: 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SCPERCLKEN3_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT3_UNION
 struct description   : SCPERSTAT3 Register structure definition
                        Address Offset:0x1AC Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                     : 1;  /* bit[0]    :  */
        unsigned int  st_pclk_ao_gpio1_se            : 1;  /* bit[1]    : 外设时钟最终状态：
                                                                          0：IP时钟禁止状态；
                                                                          1：IP时钟使能状态。 */
        unsigned int  gt_aobus_noc_ini               : 1;  /* bit[2]    : 外设时钟最终状态(投票之后)：
                                                                          0：IP时钟禁止状态；
                                                                          1：IP时钟使能状态。 */
        unsigned int  reserved_1                     : 1;  /* bit[3]    :  */
        unsigned int  autodiv_aonoc_fll_stat         : 1;  /* bit[4]    : aobus_noc_fll自动降频状态指示
                                                                          0:不降频；
                                                                          1：处于降频。 */
        unsigned int  st_pclk_timer17                : 1;  /* bit[5]    : 外设时钟最终状态：
                                                                          0：IP时钟禁止状态；
                                                                          1：IP时钟使能状态。 */
        unsigned int  reserved_2                     : 1;  /* bit[6]    :  */
        unsigned int  st_pclk_timer16                : 1;  /* bit[7]    :  */
        unsigned int  reserved_3                     : 1;  /* bit[8]    :  */
        unsigned int  st_pclk_timer15                : 1;  /* bit[9]    :  */
        unsigned int  reserved_4                     : 1;  /* bit[10]   :  */
        unsigned int  st_pclk_timer14                : 1;  /* bit[11]   :  */
        unsigned int  reserved_5                     : 1;  /* bit[12]   :  */
        unsigned int  st_pclk_timer13                : 1;  /* bit[13]   :  */
        unsigned int  swdone_clk_out2_div            : 1;  /* bit[14]   :  */
        unsigned int  st_pclk_ao_ipc                 : 1;  /* bit[15]   :  */
        unsigned int  st_clk_dp_audio_pll_tp         : 1;  /* bit[16]   : clk_dp_audio_pll_tp门控状态，默认关 */
        unsigned int  sw_ack_clk_pcie_aux            : 2;  /* bit[17-18]: clk_pcie_aux时钟源选择：
                                                                          2'b01:19.2MHz通路
                                                                          2'b10:32KPLL通路 */
        unsigned int  sw_ack_clk_timer_sw            : 2;  /* bit[19-20]: timer时钟源选择状态：
                                                                          2'b01: 32k时钟源
                                                                          2'b10: ULPPLL时钟源 */
        unsigned int  st_atclk_to_iomcu              : 1;  /* bit[21]   :  */
        unsigned int  st_clk_noc_timeout_extref_peri : 1;  /* bit[22]   :  */
        unsigned int  st_timerclk_refh_peri          : 1;  /* bit[23]   :  */
        unsigned int  st_clkin_sys_peri              : 1;  /* bit[24]   :  */
        unsigned int  st_clkin_ref_peri              : 1;  /* bit[25]   :  */
        unsigned int  st_clk_sys_ini_peri            : 1;  /* bit[26]   :  */
        unsigned int  swdone_clk_em_sync_top_div     : 1;  /* bit[27]   :  */
        unsigned int  st_clk_ref_peri                : 1;  /* bit[28]   :  */
        unsigned int  st_clk_sys_peri                : 1;  /* bit[29]   :  */
        unsigned int  st_clk_aobus_peri              : 1;  /* bit[30]   :  */
        unsigned int  st_clk_fll_src_peri            : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERSTAT3_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_ao_gpio1_se_START             (1)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_ao_gpio1_se_END               (1)
#define SOC_AO_CRG_SCPERSTAT3_gt_aobus_noc_ini_START                (2)
#define SOC_AO_CRG_SCPERSTAT3_gt_aobus_noc_ini_END                  (2)
#define SOC_AO_CRG_SCPERSTAT3_autodiv_aonoc_fll_stat_START          (4)
#define SOC_AO_CRG_SCPERSTAT3_autodiv_aonoc_fll_stat_END            (4)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer17_START                 (5)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer17_END                   (5)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer16_START                 (7)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer16_END                   (7)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer15_START                 (9)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer15_END                   (9)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer14_START                 (11)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer14_END                   (11)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer13_START                 (13)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_timer13_END                   (13)
#define SOC_AO_CRG_SCPERSTAT3_swdone_clk_out2_div_START             (14)
#define SOC_AO_CRG_SCPERSTAT3_swdone_clk_out2_div_END               (14)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_ao_ipc_START                  (15)
#define SOC_AO_CRG_SCPERSTAT3_st_pclk_ao_ipc_END                    (15)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_dp_audio_pll_tp_START          (16)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_dp_audio_pll_tp_END            (16)
#define SOC_AO_CRG_SCPERSTAT3_sw_ack_clk_pcie_aux_START             (17)
#define SOC_AO_CRG_SCPERSTAT3_sw_ack_clk_pcie_aux_END               (18)
#define SOC_AO_CRG_SCPERSTAT3_sw_ack_clk_timer_sw_START             (19)
#define SOC_AO_CRG_SCPERSTAT3_sw_ack_clk_timer_sw_END               (20)
#define SOC_AO_CRG_SCPERSTAT3_st_atclk_to_iomcu_START               (21)
#define SOC_AO_CRG_SCPERSTAT3_st_atclk_to_iomcu_END                 (21)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_noc_timeout_extref_peri_START  (22)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_noc_timeout_extref_peri_END    (22)
#define SOC_AO_CRG_SCPERSTAT3_st_timerclk_refh_peri_START           (23)
#define SOC_AO_CRG_SCPERSTAT3_st_timerclk_refh_peri_END             (23)
#define SOC_AO_CRG_SCPERSTAT3_st_clkin_sys_peri_START               (24)
#define SOC_AO_CRG_SCPERSTAT3_st_clkin_sys_peri_END                 (24)
#define SOC_AO_CRG_SCPERSTAT3_st_clkin_ref_peri_START               (25)
#define SOC_AO_CRG_SCPERSTAT3_st_clkin_ref_peri_END                 (25)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_sys_ini_peri_START             (26)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_sys_ini_peri_END               (26)
#define SOC_AO_CRG_SCPERSTAT3_swdone_clk_em_sync_top_div_START      (27)
#define SOC_AO_CRG_SCPERSTAT3_swdone_clk_em_sync_top_div_END        (27)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_ref_peri_START                 (28)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_ref_peri_END                   (28)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_sys_peri_START                 (29)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_sys_peri_END                   (29)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_aobus_peri_START               (30)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_aobus_peri_END                 (30)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_fll_src_peri_START             (31)
#define SOC_AO_CRG_SCPERSTAT3_st_clk_fll_src_peri_END               (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN4_UNION
 struct description   : SCPEREN4 Register structure definition
                        Address Offset:0x1B0 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_memrepair_ao       : 1;  /* bit[0] : 外设时钟使能控制：
                                                                  0：写0无效；
                                                                  1：写1使能IP时钟。 */
        unsigned int  gt_clk_memrepair_peri     : 1;  /* bit[1] :  */
        unsigned int  reserved_0                : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_ao_ipc_ns         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_ao_loadmonitor    : 1;  /* bit[4] :  */
        unsigned int  gt_clk_ao_loadmonitor     : 1;  /* bit[5] :  */
        unsigned int  gt_clk_spi5               : 1;  /* bit[6] :  */
        unsigned int  gt_clk_dp_audio_pll       : 1;  /* bit[7] :  */
        unsigned int  gt_clk_out2               : 1;  /* bit[8] :  */
        unsigned int  reserved_1                : 1;  /* bit[9] :  */
        unsigned int  gt_clk_spi3               : 1;  /* bit[10]:  */
        unsigned int  reserved_2                : 1;  /* bit[11]:  */
        unsigned int  reserved_3                : 1;  /* bit[12]:  */
        unsigned int  reserved_4                : 1;  /* bit[13]:  */
        unsigned int  reserved_5                : 1;  /* bit[14]:  */
        unsigned int  reserved_6                : 1;  /* bit[15]:  */
        unsigned int  reserved_7                : 1;  /* bit[16]:  */
        unsigned int  reserved_8                : 1;  /* bit[17]:  */
        unsigned int  reserved_9                : 1;  /* bit[18]:  */
        unsigned int  reserved_10               : 1;  /* bit[19]:  */
        unsigned int  reserved_11               : 1;  /* bit[20]:  */
        unsigned int  reserved_12               : 1;  /* bit[21]:  */
        unsigned int  reserved_13               : 1;  /* bit[22]:  */
        unsigned int  reserved_14               : 1;  /* bit[23]:  */
        unsigned int  reserved_15               : 1;  /* bit[24]:  */
        unsigned int  gt_clk_ufs_ref_test       : 1;  /* bit[25]:  */
        unsigned int  gt_pclk_ao_loadmonitor_m3 : 1;  /* bit[26]:  */
        unsigned int  reserved_16               : 1;  /* bit[27]:  */
        unsigned int  reserved_17               : 1;  /* bit[28]:  */
        unsigned int  reserved_18               : 1;  /* bit[29]:  */
        unsigned int  reserved_19               : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_20               : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPEREN4_UNION;
#endif
#define SOC_AO_CRG_SCPEREN4_gt_clk_memrepair_ao_START        (0)
#define SOC_AO_CRG_SCPEREN4_gt_clk_memrepair_ao_END          (0)
#define SOC_AO_CRG_SCPEREN4_gt_clk_memrepair_peri_START      (1)
#define SOC_AO_CRG_SCPEREN4_gt_clk_memrepair_peri_END        (1)
#define SOC_AO_CRG_SCPEREN4_gt_pclk_ao_ipc_ns_START          (3)
#define SOC_AO_CRG_SCPEREN4_gt_pclk_ao_ipc_ns_END            (3)
#define SOC_AO_CRG_SCPEREN4_gt_pclk_ao_loadmonitor_START     (4)
#define SOC_AO_CRG_SCPEREN4_gt_pclk_ao_loadmonitor_END       (4)
#define SOC_AO_CRG_SCPEREN4_gt_clk_ao_loadmonitor_START      (5)
#define SOC_AO_CRG_SCPEREN4_gt_clk_ao_loadmonitor_END        (5)
#define SOC_AO_CRG_SCPEREN4_gt_clk_spi5_START                (6)
#define SOC_AO_CRG_SCPEREN4_gt_clk_spi5_END                  (6)
#define SOC_AO_CRG_SCPEREN4_gt_clk_dp_audio_pll_START        (7)
#define SOC_AO_CRG_SCPEREN4_gt_clk_dp_audio_pll_END          (7)
#define SOC_AO_CRG_SCPEREN4_gt_clk_out2_START                (8)
#define SOC_AO_CRG_SCPEREN4_gt_clk_out2_END                  (8)
#define SOC_AO_CRG_SCPEREN4_gt_clk_spi3_START                (10)
#define SOC_AO_CRG_SCPEREN4_gt_clk_spi3_END                  (10)
#define SOC_AO_CRG_SCPEREN4_gt_clk_ufs_ref_test_START        (25)
#define SOC_AO_CRG_SCPEREN4_gt_clk_ufs_ref_test_END          (25)
#define SOC_AO_CRG_SCPEREN4_gt_pclk_ao_loadmonitor_m3_START  (26)
#define SOC_AO_CRG_SCPEREN4_gt_pclk_ao_loadmonitor_m3_END    (26)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS4_UNION
 struct description   : SCPERDIS4 Register structure definition
                        Address Offset:0x1B4 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_memrepair_ao       : 1;  /* bit[0] : 外设时钟禁止控制：
                                                                  0：写0无效；
                                                                  1：写1禁止IP时钟。 */
        unsigned int  gt_clk_memrepair_peri     : 1;  /* bit[1] :  */
        unsigned int  reserved_0                : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_ao_ipc_ns         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_ao_loadmonitor    : 1;  /* bit[4] :  */
        unsigned int  gt_clk_ao_loadmonitor     : 1;  /* bit[5] :  */
        unsigned int  gt_clk_spi5               : 1;  /* bit[6] :  */
        unsigned int  gt_clk_dp_audio_pll       : 1;  /* bit[7] :  */
        unsigned int  gt_clk_out2               : 1;  /* bit[8] :  */
        unsigned int  reserved_1                : 1;  /* bit[9] :  */
        unsigned int  gt_clk_spi3               : 1;  /* bit[10]:  */
        unsigned int  reserved_2                : 1;  /* bit[11]:  */
        unsigned int  reserved_3                : 1;  /* bit[12]:  */
        unsigned int  reserved_4                : 1;  /* bit[13]:  */
        unsigned int  reserved_5                : 1;  /* bit[14]:  */
        unsigned int  reserved_6                : 1;  /* bit[15]:  */
        unsigned int  reserved_7                : 1;  /* bit[16]:  */
        unsigned int  reserved_8                : 1;  /* bit[17]:  */
        unsigned int  reserved_9                : 1;  /* bit[18]:  */
        unsigned int  reserved_10               : 1;  /* bit[19]:  */
        unsigned int  reserved_11               : 1;  /* bit[20]:  */
        unsigned int  reserved_12               : 1;  /* bit[21]:  */
        unsigned int  reserved_13               : 1;  /* bit[22]:  */
        unsigned int  reserved_14               : 1;  /* bit[23]:  */
        unsigned int  reserved_15               : 1;  /* bit[24]:  */
        unsigned int  gt_clk_ufs_ref_test       : 1;  /* bit[25]:  */
        unsigned int  gt_pclk_ao_loadmonitor_m3 : 1;  /* bit[26]:  */
        unsigned int  reserved_16               : 1;  /* bit[27]:  */
        unsigned int  reserved_17               : 1;  /* bit[28]:  */
        unsigned int  reserved_18               : 1;  /* bit[29]:  */
        unsigned int  reserved_19               : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_20               : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPERDIS4_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS4_gt_clk_memrepair_ao_START        (0)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_memrepair_ao_END          (0)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_memrepair_peri_START      (1)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_memrepair_peri_END        (1)
#define SOC_AO_CRG_SCPERDIS4_gt_pclk_ao_ipc_ns_START          (3)
#define SOC_AO_CRG_SCPERDIS4_gt_pclk_ao_ipc_ns_END            (3)
#define SOC_AO_CRG_SCPERDIS4_gt_pclk_ao_loadmonitor_START     (4)
#define SOC_AO_CRG_SCPERDIS4_gt_pclk_ao_loadmonitor_END       (4)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_ao_loadmonitor_START      (5)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_ao_loadmonitor_END        (5)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_spi5_START                (6)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_spi5_END                  (6)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_dp_audio_pll_START        (7)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_dp_audio_pll_END          (7)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_out2_START                (8)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_out2_END                  (8)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_spi3_START                (10)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_spi3_END                  (10)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_ufs_ref_test_START        (25)
#define SOC_AO_CRG_SCPERDIS4_gt_clk_ufs_ref_test_END          (25)
#define SOC_AO_CRG_SCPERDIS4_gt_pclk_ao_loadmonitor_m3_START  (26)
#define SOC_AO_CRG_SCPERDIS4_gt_pclk_ao_loadmonitor_m3_END    (26)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN4_UNION
 struct description   : SCPERCLKEN4 Register structure definition
                        Address Offset:0x1B8 Initial:0x1 Width:32
 register description : 外设时钟使能状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_memrepair_ao       : 1;  /* bit[0] : 外设时钟使能状态：
                                                                  0：IP时钟禁止状态；
                                                                  1：IP时钟使能状态。 */
        unsigned int  gt_clk_memrepair_peri     : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_0                : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  gt_pclk_ao_ipc_ns         : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  gt_pclk_ao_loadmonitor    : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  gt_clk_ao_loadmonitor     : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  gt_clk_spi5               : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  gt_clk_dp_audio_pll       : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  gt_clk_out2               : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  reserved_1                : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  gt_clk_spi3               : 1;  /* bit[10]: 含义同bit0。 */
        unsigned int  reserved_2                : 1;  /* bit[11]: 含义同bit0。 */
        unsigned int  reserved_3                : 1;  /* bit[12]: 含义同bit0。 */
        unsigned int  reserved_4                : 1;  /* bit[13]: 含义同bit0。 */
        unsigned int  reserved_5                : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  reserved_6                : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  reserved_7                : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  reserved_8                : 1;  /* bit[17]: 含义同bit0。 */
        unsigned int  reserved_9                : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  reserved_10               : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  reserved_12               : 1;  /* bit[21]: 含义同bit0。 */
        unsigned int  reserved_13               : 1;  /* bit[22]: 含义同bit0。 */
        unsigned int  reserved_14               : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  reserved_15               : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  gt_clk_ufs_ref_test       : 1;  /* bit[25]: 含义同bit0。 */
        unsigned int  gt_pclk_ao_loadmonitor_m3 : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  reserved_16               : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  reserved_17               : 1;  /* bit[28]: 含义同bit0。 */
        unsigned int  reserved_18               : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  reserved_19               : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_20               : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPERCLKEN4_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_memrepair_ao_START        (0)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_memrepair_ao_END          (0)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_memrepair_peri_START      (1)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_memrepair_peri_END        (1)
#define SOC_AO_CRG_SCPERCLKEN4_gt_pclk_ao_ipc_ns_START          (3)
#define SOC_AO_CRG_SCPERCLKEN4_gt_pclk_ao_ipc_ns_END            (3)
#define SOC_AO_CRG_SCPERCLKEN4_gt_pclk_ao_loadmonitor_START     (4)
#define SOC_AO_CRG_SCPERCLKEN4_gt_pclk_ao_loadmonitor_END       (4)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_ao_loadmonitor_START      (5)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_ao_loadmonitor_END        (5)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_spi5_START                (6)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_spi5_END                  (6)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_dp_audio_pll_START        (7)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_dp_audio_pll_END          (7)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_out2_START                (8)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_out2_END                  (8)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_spi3_START                (10)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_spi3_END                  (10)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_ufs_ref_test_START        (25)
#define SOC_AO_CRG_SCPERCLKEN4_gt_clk_ufs_ref_test_END          (25)
#define SOC_AO_CRG_SCPERCLKEN4_gt_pclk_ao_loadmonitor_m3_START  (26)
#define SOC_AO_CRG_SCPERCLKEN4_gt_pclk_ao_loadmonitor_m3_END    (26)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT4_UNION
 struct description   : SCPERSTAT4 Register structure definition
                        Address Offset:0x1BC Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_memrepair_ao    : 1;  /* bit[0]    : 外设时钟最终状态：
                                                                  0：IP时钟禁止状态；
                                                                  1：IP时钟使能状态。 */
        unsigned int  st_clk_spmi_mst        : 1;  /* bit[1]    :  */
        unsigned int  st_pclk_ao_tzpc        : 1;  /* bit[2]    :  */
        unsigned int  st_pclk_ao_ipc_ns      : 1;  /* bit[3]    :  */
        unsigned int  st_pclk_ao_loadmonitor : 1;  /* bit[4]    :  */
        unsigned int  st_clk_ao_loadmonitor  : 1;  /* bit[5]    :  */
        unsigned int  st_ulppll_1_out        : 1;  /* bit[6]    : ULPPLL1输出门控状态：0：关；1：开 */
        unsigned int  st_clk_dp_audio_pll    : 1;  /* bit[7]    :  */
        unsigned int  reserved_0             : 1;  /* bit[8]    :  */
        unsigned int  st_clk_qic_spidma      : 1;  /* bit[9]    :  */
        unsigned int  st_clk_spi3            : 1;  /* bit[10]   :  */
        unsigned int  st_clk_spi5            : 1;  /* bit[11]   :  */
        unsigned int  reserved_1             : 1;  /* bit[12]   :  */
        unsigned int  reserved_2             : 1;  /* bit[13]   :  */
        unsigned int  reserved_3             : 1;  /* bit[14]   :  */
        unsigned int  sw_ack_clk_mad_sw      : 2;  /* bit[15-16]: mad时钟源选择状态：
                                                                  2'b01 : spll源
                                                                  2'b10 : fll源 */
        unsigned int  reserved_4             : 1;  /* bit[17]   :  */
        unsigned int  st_clk_pll_fsm         : 1;  /* bit[18]   :  */
        unsigned int  reserved_5             : 1;  /* bit[19]   :  */
        unsigned int  reserved_6             : 1;  /* bit[20]   :  */
        unsigned int  st_clk_memrepair_peri  : 1;  /* bit[21]   :  */
        unsigned int  st_pclk_intr_hub       : 1;  /* bit[22]   :  */
        unsigned int  reserved_7             : 1;  /* bit[23]   :  */
        unsigned int  st_clk_aupll_sscg      : 1;  /* bit[24]   : DP AUDIO专用PLL SSCG时钟门控状态；
                                                                  0：关，1：开 */
        unsigned int  st_clk_ufs_ref_test    : 1;  /* bit[25]   :  */
        unsigned int  reserved_8             : 1;  /* bit[26]   :  */
        unsigned int  swdone_clk_timer_div   : 1;  /* bit[27]   :  */
        unsigned int  reserved_9             : 1;  /* bit[28]   :  */
        unsigned int  reserved_10            : 1;  /* bit[29]   :  */
        unsigned int  reserved_11            : 1;  /* bit[30]   :  */
        unsigned int  swdone_clk_ao_332m_div : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERSTAT4_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT4_st_clk_memrepair_ao_START     (0)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_memrepair_ao_END       (0)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_spmi_mst_START         (1)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_spmi_mst_END           (1)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_ao_tzpc_START         (2)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_ao_tzpc_END           (2)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_ao_ipc_ns_START       (3)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_ao_ipc_ns_END         (3)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_ao_loadmonitor_START  (4)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_ao_loadmonitor_END    (4)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_ao_loadmonitor_START   (5)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_ao_loadmonitor_END     (5)
#define SOC_AO_CRG_SCPERSTAT4_st_ulppll_1_out_START         (6)
#define SOC_AO_CRG_SCPERSTAT4_st_ulppll_1_out_END           (6)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_dp_audio_pll_START     (7)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_dp_audio_pll_END       (7)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_qic_spidma_START       (9)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_qic_spidma_END         (9)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_spi3_START             (10)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_spi3_END               (10)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_spi5_START             (11)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_spi5_END               (11)
#define SOC_AO_CRG_SCPERSTAT4_sw_ack_clk_mad_sw_START       (15)
#define SOC_AO_CRG_SCPERSTAT4_sw_ack_clk_mad_sw_END         (16)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_pll_fsm_START          (18)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_pll_fsm_END            (18)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_memrepair_peri_START   (21)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_memrepair_peri_END     (21)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_intr_hub_START        (22)
#define SOC_AO_CRG_SCPERSTAT4_st_pclk_intr_hub_END          (22)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_aupll_sscg_START       (24)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_aupll_sscg_END         (24)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_ufs_ref_test_START     (25)
#define SOC_AO_CRG_SCPERSTAT4_st_clk_ufs_ref_test_END       (25)
#define SOC_AO_CRG_SCPERSTAT4_swdone_clk_timer_div_START    (27)
#define SOC_AO_CRG_SCPERSTAT4_swdone_clk_timer_div_END      (27)
#define SOC_AO_CRG_SCPERSTAT4_swdone_clk_ao_332m_div_START  (31)
#define SOC_AO_CRG_SCPERSTAT4_swdone_clk_ao_332m_div_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN5_UNION
 struct description   : SCPEREN5 Register structure definition
                        Address Offset:0x1C0 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_timer17  : 1;  /* bit[0] : 外设时钟使能控制：
                                                         0：写0无效；
                                                         1：写1使能IP时钟。 */
        unsigned int  reserved_0       : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_timer16  : 1;  /* bit[2] :  */
        unsigned int  reserved_1       : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_timer15  : 1;  /* bit[4] :  */
        unsigned int  reserved_2       : 1;  /* bit[5] :  */
        unsigned int  gt_pclk_timer14  : 1;  /* bit[6] :  */
        unsigned int  reserved_3       : 1;  /* bit[7] :  */
        unsigned int  gt_pclk_timer13  : 1;  /* bit[8] :  */
        unsigned int  reserved_4       : 1;  /* bit[9] :  */
        unsigned int  reserved_5       : 1;  /* bit[10]:  */
        unsigned int  reserved_6       : 1;  /* bit[11]:  */
        unsigned int  reserved_7       : 1;  /* bit[12]:  */
        unsigned int  reserved_8       : 1;  /* bit[13]:  */
        unsigned int  gt_pclk_intr_hub : 1;  /* bit[14]:  */
        unsigned int  reserved_9       : 1;  /* bit[15]:  */
        unsigned int  reserved_10      : 1;  /* bit[16]:  */
        unsigned int  reserved_11      : 1;  /* bit[17]:  */
        unsigned int  reserved_12      : 1;  /* bit[18]:  */
        unsigned int  reserved_13      : 1;  /* bit[19]:  */
        unsigned int  reserved_14      : 1;  /* bit[20]:  */
        unsigned int  reserved_15      : 1;  /* bit[21]:  */
        unsigned int  reserved_16      : 1;  /* bit[22]:  */
        unsigned int  reserved_17      : 1;  /* bit[23]:  */
        unsigned int  reserved_18      : 1;  /* bit[24]:  */
        unsigned int  reserved_19      : 1;  /* bit[25]:  */
        unsigned int  reserved_20      : 1;  /* bit[26]:  */
        unsigned int  reserved_21      : 1;  /* bit[27]:  */
        unsigned int  reserved_22      : 1;  /* bit[28]:  */
        unsigned int  reserved_23      : 1;  /* bit[29]:  */
        unsigned int  reserved_24      : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_25      : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPEREN5_UNION;
#endif
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer17_START   (0)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer17_END     (0)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer16_START   (2)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer16_END     (2)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer15_START   (4)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer15_END     (4)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer14_START   (6)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer14_END     (6)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer13_START   (8)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_timer13_END     (8)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_intr_hub_START  (14)
#define SOC_AO_CRG_SCPEREN5_gt_pclk_intr_hub_END    (14)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS5_UNION
 struct description   : SCPERDIS5 Register structure definition
                        Address Offset:0x1C4 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_timer17  : 1;  /* bit[0] : 外设时钟禁止控制：
                                                         0：写0无效；
                                                         1：写1禁止IP时钟。 */
        unsigned int  reserved_0       : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_timer16  : 1;  /* bit[2] :  */
        unsigned int  reserved_1       : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_timer15  : 1;  /* bit[4] :  */
        unsigned int  reserved_2       : 1;  /* bit[5] :  */
        unsigned int  gt_pclk_timer14  : 1;  /* bit[6] :  */
        unsigned int  reserved_3       : 1;  /* bit[7] :  */
        unsigned int  gt_pclk_timer13  : 1;  /* bit[8] :  */
        unsigned int  reserved_4       : 1;  /* bit[9] :  */
        unsigned int  reserved_5       : 1;  /* bit[10]:  */
        unsigned int  reserved_6       : 1;  /* bit[11]:  */
        unsigned int  reserved_7       : 1;  /* bit[12]:  */
        unsigned int  reserved_8       : 1;  /* bit[13]:  */
        unsigned int  gt_pclk_intr_hub : 1;  /* bit[14]:  */
        unsigned int  reserved_9       : 1;  /* bit[15]:  */
        unsigned int  reserved_10      : 1;  /* bit[16]:  */
        unsigned int  reserved_11      : 1;  /* bit[17]:  */
        unsigned int  reserved_12      : 1;  /* bit[18]:  */
        unsigned int  reserved_13      : 1;  /* bit[19]:  */
        unsigned int  reserved_14      : 1;  /* bit[20]:  */
        unsigned int  reserved_15      : 1;  /* bit[21]:  */
        unsigned int  reserved_16      : 1;  /* bit[22]:  */
        unsigned int  reserved_17      : 1;  /* bit[23]:  */
        unsigned int  reserved_18      : 1;  /* bit[24]:  */
        unsigned int  reserved_19      : 1;  /* bit[25]:  */
        unsigned int  reserved_20      : 1;  /* bit[26]:  */
        unsigned int  reserved_21      : 1;  /* bit[27]:  */
        unsigned int  reserved_22      : 1;  /* bit[28]:  */
        unsigned int  reserved_23      : 1;  /* bit[29]:  */
        unsigned int  reserved_24      : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_25      : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPERDIS5_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer17_START   (0)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer17_END     (0)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer16_START   (2)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer16_END     (2)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer15_START   (4)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer15_END     (4)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer14_START   (6)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer14_END     (6)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer13_START   (8)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_timer13_END     (8)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_intr_hub_START  (14)
#define SOC_AO_CRG_SCPERDIS5_gt_pclk_intr_hub_END    (14)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN5_UNION
 struct description   : SCPERCLKEN5 Register structure definition
                        Address Offset:0x1C8 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_timer17  : 1;  /* bit[0] : 外设时钟使能状态：
                                                         0：IP时钟禁止状态；
                                                         1：IP时钟使能状态。 */
        unsigned int  reserved_0       : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  gt_pclk_timer16  : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  reserved_1       : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  gt_pclk_timer15  : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  reserved_2       : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  gt_pclk_timer14  : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  reserved_3       : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  gt_pclk_timer13  : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  reserved_4       : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  reserved_5       : 1;  /* bit[10]: i2c9 软门控：0：关；1：开 */
        unsigned int  reserved_6       : 1;  /* bit[11]: i2c9 ASP开门控投票 */
        unsigned int  reserved_7       : 1;  /* bit[12]: i2c9 LPMCU开门控投票 */
        unsigned int  reserved_8       : 1;  /* bit[13]: i2c9 MDM开门控投票 */
        unsigned int  gt_pclk_intr_hub : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  reserved_9       : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  reserved_10      : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  reserved_11      : 1;  /* bit[17]: 含义同bit0。 */
        unsigned int  reserved_12      : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  reserved_13      : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  reserved_14      : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  reserved_15      : 1;  /* bit[21]: 含义同bit0。 */
        unsigned int  reserved_16      : 1;  /* bit[22]: 含义同bit0。 */
        unsigned int  reserved_17      : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  reserved_18      : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  reserved_19      : 1;  /* bit[25]: 含义同bit0。 */
        unsigned int  reserved_20      : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  reserved_21      : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  reserved_22      : 1;  /* bit[28]: 含义同bit0。 */
        unsigned int  reserved_23      : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  reserved_24      : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_25      : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPERCLKEN5_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer17_START   (0)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer17_END     (0)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer16_START   (2)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer16_END     (2)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer15_START   (4)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer15_END     (4)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer14_START   (6)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer14_END     (6)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer13_START   (8)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_timer13_END     (8)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_intr_hub_START  (14)
#define SOC_AO_CRG_SCPERCLKEN5_gt_pclk_intr_hub_END    (14)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT5_UNION
 struct description   : SCPERSTAT5 Register structure definition
                        Address Offset:0x1CC Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0              : 2;  /* bit[0-1]  :  */
        unsigned int  ulppll_1_lock           : 1;  /* bit[2]    :  */
        unsigned int  ulppll_lock             : 1;  /* bit[3]    :  */
        unsigned int  swdone_clk_blpwm2_div   : 1;  /* bit[4]    :  */
        unsigned int  st_clk_ao_tcp_h2x       : 1;  /* bit[5]    :  */
        unsigned int  st_clk_ao_tcp           : 1;  /* bit[6]    :  */
        unsigned int  st_clk_ao_tcp_32k       : 1;  /* bit[7]    :  */
        unsigned int  reserved_1              : 1;  /* bit[8]    :  */
        unsigned int  sw_ack_clk_spi3_sw      : 2;  /* bit[9-10] :  */
        unsigned int  sw_ack_clk_spi5_sw      : 2;  /* bit[11-12]:  */
        unsigned int  reserved_2              : 1;  /* bit[13]   :  */
        unsigned int  st_clk_spll_peri        : 1;  /* bit[14]   :  */
        unsigned int  reserved_3              : 1;  /* bit[15]   :  */
        unsigned int  reserved_4              : 2;  /* bit[16-17]:  */
        unsigned int  st_clk_sys_ini          : 1;  /* bit[18]   : clk_sys_ini时钟门控状态回读信号（gt在sys_ctrl/tcxo_ctrl）；
                                                                   0：关，1：开 */
        unsigned int  reserved_5              : 1;  /* bit[19]   : st_clk_ao_tof */
        unsigned int  st_pclk_efusec2         : 1;  /* bit[20]   :  */
        unsigned int  st_clkin_sys_logic_peri : 1;  /* bit[21]   :  */
        unsigned int  st_pclk_ao_wd1          : 1;  /* bit[22]   :  */
        unsigned int  st_pclk_hickm           : 1;  /* bit[23]   :  */
        unsigned int  reserved_6              : 1;  /* bit[24]   :  */
        unsigned int  reserved_7              : 4;  /* bit[25-28]:  */
        unsigned int  reserved_8              : 3;  /* bit[29-31]:  */
    } reg;
} SOC_AO_CRG_SCPERSTAT5_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT5_ulppll_1_lock_START            (2)
#define SOC_AO_CRG_SCPERSTAT5_ulppll_1_lock_END              (2)
#define SOC_AO_CRG_SCPERSTAT5_ulppll_lock_START              (3)
#define SOC_AO_CRG_SCPERSTAT5_ulppll_lock_END                (3)
#define SOC_AO_CRG_SCPERSTAT5_swdone_clk_blpwm2_div_START    (4)
#define SOC_AO_CRG_SCPERSTAT5_swdone_clk_blpwm2_div_END      (4)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_ao_tcp_h2x_START        (5)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_ao_tcp_h2x_END          (5)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_ao_tcp_START            (6)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_ao_tcp_END              (6)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_ao_tcp_32k_START        (7)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_ao_tcp_32k_END          (7)
#define SOC_AO_CRG_SCPERSTAT5_sw_ack_clk_spi3_sw_START       (9)
#define SOC_AO_CRG_SCPERSTAT5_sw_ack_clk_spi3_sw_END         (10)
#define SOC_AO_CRG_SCPERSTAT5_sw_ack_clk_spi5_sw_START       (11)
#define SOC_AO_CRG_SCPERSTAT5_sw_ack_clk_spi5_sw_END         (12)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_spll_peri_START         (14)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_spll_peri_END           (14)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_sys_ini_START           (18)
#define SOC_AO_CRG_SCPERSTAT5_st_clk_sys_ini_END             (18)
#define SOC_AO_CRG_SCPERSTAT5_st_pclk_efusec2_START          (20)
#define SOC_AO_CRG_SCPERSTAT5_st_pclk_efusec2_END            (20)
#define SOC_AO_CRG_SCPERSTAT5_st_clkin_sys_logic_peri_START  (21)
#define SOC_AO_CRG_SCPERSTAT5_st_clkin_sys_logic_peri_END    (21)
#define SOC_AO_CRG_SCPERSTAT5_st_pclk_ao_wd1_START           (22)
#define SOC_AO_CRG_SCPERSTAT5_st_pclk_ao_wd1_END             (22)
#define SOC_AO_CRG_SCPERSTAT5_st_pclk_hickm_START            (23)
#define SOC_AO_CRG_SCPERSTAT5_st_pclk_hickm_END              (23)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN6_UNION
 struct description   : SCPEREN6 Register structure definition
                        Address Offset:0x1D0 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_ufs_mphy_sys  : 1;  /* bit[0] : 外设时钟使能控制：
                                                             0：写0无效；
                                                             1：写1使能IP时钟。 */
        unsigned int  gt_clk_ao_camera1    : 1;  /* bit[1] :  */
        unsigned int  gt_clk_qspi_sfc_x2   : 1;  /* bit[2] :  */
        unsigned int  gt_hclk_qspi         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_autodiv_pcie : 1;  /* bit[4] :  */
        unsigned int  reserved_0           : 1;  /* bit[5] : xian删除 */
        unsigned int  reserved_1           : 1;  /* bit[6] :  */
        unsigned int  reserved_2           : 1;  /* bit[7] : xian删除 */
        unsigned int  gt_clk_swd2jtag_dap  : 1;  /* bit[8] : xian删除 */
        unsigned int  reserved_3           : 1;  /* bit[9] :  */
        unsigned int  reserved_4           : 1;  /* bit[10]:  */
        unsigned int  reserved_5           : 1;  /* bit[11]:  */
        unsigned int  reserved_6           : 1;  /* bit[12]:  */
        unsigned int  reserved_7           : 1;  /* bit[13]:  */
        unsigned int  reserved_8           : 1;  /* bit[14]: 【xian】删除gt_clk_out_hiphy */
        unsigned int  reserved_9           : 1;  /* bit[15]:  */
        unsigned int  reserved_10          : 1;  /* bit[16]:  */
        unsigned int  reserved_11          : 1;  /* bit[17]:  */
        unsigned int  reserved_12          : 1;  /* bit[18]:  */
        unsigned int  reserved_13          : 1;  /* bit[19]:  */
        unsigned int  reserved_14          : 1;  /* bit[20]:  */
        unsigned int  reserved_15          : 1;  /* bit[21]:  */
        unsigned int  reserved_16          : 1;  /* bit[22]:  */
        unsigned int  reserved_17          : 1;  /* bit[23]:  */
        unsigned int  reserved_18          : 1;  /* bit[24]:  */
        unsigned int  reserved_19          : 1;  /* bit[25]:  */
        unsigned int  reserved_20          : 1;  /* bit[26]:  */
        unsigned int  reserved_21          : 1;  /* bit[27]:  */
        unsigned int  reserved_22          : 1;  /* bit[28]:  */
        unsigned int  reserved_23          : 1;  /* bit[29]:  */
        unsigned int  reserved_24          : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_25          : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPEREN6_UNION;
#endif
#define SOC_AO_CRG_SCPEREN6_gt_clk_ufs_mphy_sys_START   (0)
#define SOC_AO_CRG_SCPEREN6_gt_clk_ufs_mphy_sys_END     (0)
#define SOC_AO_CRG_SCPEREN6_gt_clk_ao_camera1_START     (1)
#define SOC_AO_CRG_SCPEREN6_gt_clk_ao_camera1_END       (1)
#define SOC_AO_CRG_SCPEREN6_gt_clk_qspi_sfc_x2_START    (2)
#define SOC_AO_CRG_SCPEREN6_gt_clk_qspi_sfc_x2_END      (2)
#define SOC_AO_CRG_SCPEREN6_gt_hclk_qspi_START          (3)
#define SOC_AO_CRG_SCPEREN6_gt_hclk_qspi_END            (3)
#define SOC_AO_CRG_SCPEREN6_gt_pclk_autodiv_pcie_START  (4)
#define SOC_AO_CRG_SCPEREN6_gt_pclk_autodiv_pcie_END    (4)
#define SOC_AO_CRG_SCPEREN6_gt_clk_swd2jtag_dap_START   (8)
#define SOC_AO_CRG_SCPEREN6_gt_clk_swd2jtag_dap_END     (8)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS6_UNION
 struct description   : SCPERDIS6 Register structure definition
                        Address Offset:0x1D4 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_ufs_mphy_sys  : 1;  /* bit[0] : 外设时钟禁止控制：
                                                             0：写0无效；
                                                             1：写1禁止IP时钟。 */
        unsigned int  gt_clk_ao_camera1    : 1;  /* bit[1] :  */
        unsigned int  gt_clk_qspi_sfc_x2   : 1;  /* bit[2] :  */
        unsigned int  gt_hclk_qspi         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_autodiv_pcie : 1;  /* bit[4] :  */
        unsigned int  reserved_0           : 1;  /* bit[5] :  */
        unsigned int  reserved_1           : 1;  /* bit[6] :  */
        unsigned int  reserved_2           : 1;  /* bit[7] :  */
        unsigned int  gt_clk_swd2jtag_dap  : 1;  /* bit[8] :  */
        unsigned int  reserved_3           : 1;  /* bit[9] :  */
        unsigned int  reserved_4           : 1;  /* bit[10]:  */
        unsigned int  reserved_5           : 1;  /* bit[11]:  */
        unsigned int  reserved_6           : 1;  /* bit[12]:  */
        unsigned int  reserved_7           : 1;  /* bit[13]:  */
        unsigned int  reserved_8           : 1;  /* bit[14]:  */
        unsigned int  reserved_9           : 1;  /* bit[15]:  */
        unsigned int  reserved_10          : 1;  /* bit[16]:  */
        unsigned int  reserved_11          : 1;  /* bit[17]:  */
        unsigned int  reserved_12          : 1;  /* bit[18]:  */
        unsigned int  reserved_13          : 1;  /* bit[19]:  */
        unsigned int  reserved_14          : 1;  /* bit[20]:  */
        unsigned int  reserved_15          : 1;  /* bit[21]:  */
        unsigned int  reserved_16          : 1;  /* bit[22]:  */
        unsigned int  reserved_17          : 1;  /* bit[23]:  */
        unsigned int  reserved_18          : 1;  /* bit[24]:  */
        unsigned int  reserved_19          : 1;  /* bit[25]:  */
        unsigned int  reserved_20          : 1;  /* bit[26]:  */
        unsigned int  reserved_21          : 1;  /* bit[27]:  */
        unsigned int  reserved_22          : 1;  /* bit[28]:  */
        unsigned int  reserved_23          : 1;  /* bit[29]:  */
        unsigned int  reserved_24          : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_25          : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPERDIS6_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS6_gt_clk_ufs_mphy_sys_START   (0)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_ufs_mphy_sys_END     (0)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_ao_camera1_START     (1)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_ao_camera1_END       (1)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_qspi_sfc_x2_START    (2)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_qspi_sfc_x2_END      (2)
#define SOC_AO_CRG_SCPERDIS6_gt_hclk_qspi_START          (3)
#define SOC_AO_CRG_SCPERDIS6_gt_hclk_qspi_END            (3)
#define SOC_AO_CRG_SCPERDIS6_gt_pclk_autodiv_pcie_START  (4)
#define SOC_AO_CRG_SCPERDIS6_gt_pclk_autodiv_pcie_END    (4)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_swd2jtag_dap_START   (8)
#define SOC_AO_CRG_SCPERDIS6_gt_clk_swd2jtag_dap_END     (8)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN6_UNION
 struct description   : SCPERCLKEN6 Register structure definition
                        Address Offset:0x1D8 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_ufs_mphy_sys  : 1;  /* bit[0] : 外设时钟使能状态：
                                                             0：IP时钟禁止状态；
                                                             1：IP时钟使能状态。 */
        unsigned int  gt_clk_ao_camera1    : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  gt_clk_qspi_sfc_x2   : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  gt_hclk_qspi         : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  gt_pclk_autodiv_pcie : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  reserved_0           : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  reserved_1           : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  gt_clk_out_io_die    : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  gt_clk_swd2jtag_dap  : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  reserved_2           : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  reserved_3           : 1;  /* bit[10]:  */
        unsigned int  reserved_4           : 1;  /* bit[11]:  */
        unsigned int  reserved_5           : 1;  /* bit[12]:  */
        unsigned int  reserved_6           : 1;  /* bit[13]:  */
        unsigned int  reserved_7           : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  reserved_8           : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  reserved_9           : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  reserved_10          : 1;  /* bit[17]: 含义同bit0。 */
        unsigned int  reserved_11          : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  reserved_12          : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  reserved_13          : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  reserved_14          : 1;  /* bit[21]: 含义同bit0。 */
        unsigned int  reserved_15          : 1;  /* bit[22]: 含义同bit0。 */
        unsigned int  reserved_16          : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  reserved_17          : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  reserved_18          : 1;  /* bit[25]: 含义同bit0。 */
        unsigned int  reserved_19          : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  reserved_20          : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  reserved_21          : 1;  /* bit[28]: 含义同bit0。 */
        unsigned int  reserved_22          : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  reserved_23          : 1;  /* bit[30]: 含义同bit0。 */
        unsigned int  reserved_24          : 1;  /* bit[31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_SCPERCLKEN6_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_ufs_mphy_sys_START   (0)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_ufs_mphy_sys_END     (0)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_ao_camera1_START     (1)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_ao_camera1_END       (1)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_qspi_sfc_x2_START    (2)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_qspi_sfc_x2_END      (2)
#define SOC_AO_CRG_SCPERCLKEN6_gt_hclk_qspi_START          (3)
#define SOC_AO_CRG_SCPERCLKEN6_gt_hclk_qspi_END            (3)
#define SOC_AO_CRG_SCPERCLKEN6_gt_pclk_autodiv_pcie_START  (4)
#define SOC_AO_CRG_SCPERCLKEN6_gt_pclk_autodiv_pcie_END    (4)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_out_io_die_START     (7)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_out_io_die_END       (7)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_swd2jtag_dap_START   (8)
#define SOC_AO_CRG_SCPERCLKEN6_gt_clk_swd2jtag_dap_END     (8)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTAT6_UNION
 struct description   : SCPERSTAT6 Register structure definition
                        Address Offset:0x1DC Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_ufs_mphy_sys  : 1;  /* bit[0]    :  */
        unsigned int  st_clk_ao_camera1    : 1;  /* bit[1]    :  */
        unsigned int  st_clk_qspi_sfc_x2   : 1;  /* bit[2]    :  */
        unsigned int  st_hclk_qspi         : 1;  /* bit[3]    :  */
        unsigned int  st_pclk_autodiv_pcie : 1;  /* bit[4]    :  */
        unsigned int  reserved_0           : 1;  /* bit[5]    : xian删除 */
        unsigned int  reserved_1           : 1;  /* bit[6]    : st_clk_out_ufs */
        unsigned int  st_clk_out_io_die    : 1;  /* bit[7]    : st_clk_out_io_die */
        unsigned int  st_clk_swd2jtag_dap  : 1;  /* bit[8]    :  */
        unsigned int  reserved_2           : 2;  /* bit[9-10] :  */
        unsigned int  reserved_3           : 2;  /* bit[11-12]:  */
        unsigned int  reserved_4           : 1;  /* bit[13]   :  */
        unsigned int  reserved_5           : 1;  /* bit[14]   :  */
        unsigned int  reserved_6           : 1;  /* bit[15]   :  */
        unsigned int  reserved_7           : 2;  /* bit[16-17]:  */
        unsigned int  reserved_8           : 1;  /* bit[18]   :  */
        unsigned int  reserved_9           : 1;  /* bit[19]   :  */
        unsigned int  reserved_10          : 1;  /* bit[20]   :  */
        unsigned int  reserved_11          : 1;  /* bit[21]   :  */
        unsigned int  reserved_12          : 1;  /* bit[22]   :  */
        unsigned int  reserved_13          : 1;  /* bit[23]   :  */
        unsigned int  reserved_14          : 1;  /* bit[24]   :  */
        unsigned int  reserved_15          : 4;  /* bit[25-28]:  */
        unsigned int  reserved_16          : 3;  /* bit[29-31]:  */
    } reg;
} SOC_AO_CRG_SCPERSTAT6_UNION;
#endif
#define SOC_AO_CRG_SCPERSTAT6_st_clk_ufs_mphy_sys_START   (0)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_ufs_mphy_sys_END     (0)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_ao_camera1_START     (1)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_ao_camera1_END       (1)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_qspi_sfc_x2_START    (2)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_qspi_sfc_x2_END      (2)
#define SOC_AO_CRG_SCPERSTAT6_st_hclk_qspi_START          (3)
#define SOC_AO_CRG_SCPERSTAT6_st_hclk_qspi_END            (3)
#define SOC_AO_CRG_SCPERSTAT6_st_pclk_autodiv_pcie_START  (4)
#define SOC_AO_CRG_SCPERSTAT6_st_pclk_autodiv_pcie_END    (4)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_out_io_die_START     (7)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_out_io_die_END       (7)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_swd2jtag_dap_START   (8)
#define SOC_AO_CRG_SCPERSTAT6_st_clk_swd2jtag_dap_END     (8)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_STATE0_UNION
 struct description   : FNPLL_STATE0 Register structure definition
                        Address Offset:0x1E8 Initial:0x00 Width:32
 register description : fnpll状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_AO_CRG_FNPLL_STATE0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTEN0_UNION
 struct description   : SCPERRSTEN0 Register structure definition
                        Address Offset:0x200 Initial:0x0 Width:32
 register description : 外设软复位使能寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0              : 1;  /* bit[0] : IP软复位使能：
                                                                0：IP软复位使能状态不变；
                                                                1：IP软复位使能。 */
        unsigned int  ip_prst_rtc             : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_1              : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  ip_prst_timer0          : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  ip_prst_blpwm2          : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  reserved_2              : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  reserved_3              : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  ip_rst_i2s_asp2dp_rs    : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  ip_rst_asp_dw_axi       : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  ip_rst_asp_x2h          : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  ip_rst_asp_h2p          : 1;  /* bit[10]: 含义同bit0。 */
        unsigned int  ip_rst_em_sync_top      : 1;  /* bit[11]: 含义同bit0。 */
        unsigned int  reserved_4              : 1;  /* bit[12]: 含义同bit0。 */
        unsigned int  reserved_5              : 1;  /* bit[13]: 含义同bit0。 */
        unsigned int  reserved_6              : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  reserved_7              : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  reserved_8              : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  reserved_9              : 1;  /* bit[17]:  */
        unsigned int  ip_prst_ao_ioc          : 1;  /* bit[18]: 含义同bit0 */
        unsigned int  ip_prst_syscnt          : 1;  /* bit[19]: 含义同bit0 */
        unsigned int  ip_rst_syscnt           : 1;  /* bit[20]: 含义同bit0 */
        unsigned int  reserved_10             : 1;  /* bit[21]:  */
        unsigned int  reserved_11             : 1;  /* bit[22]:  */
        unsigned int  reserved_12             : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  reserved_13             : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  reserved_14             : 1;  /* bit[25]:  */
        unsigned int  ip_rst_noc_aobus2aspbus : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  ip_prst_bbpdrx          : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  ip_prst_ao_ipc_ns       : 1;  /* bit[28]:  */
        unsigned int  reserved_15             : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  sc_rst_src_clr          : 1;  /* bit[30]: 复位源信息清除控制：
                                                                0：写0无效果；
                                                                1：复位源信息清除，并关闭复位源记录功能。 */
        unsigned int  sc_pmurst_ctrl          : 1;  /* bit[31]: PMU复位清除控制：
                                                                0：写0无效果；
                                                                1：关闭PMU复位功能并且开启AP复位功能。 */
    } reg;
} SOC_AO_CRG_SCPERRSTEN0_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_rtc_START              (1)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_rtc_END                (1)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_timer0_START           (3)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_timer0_END             (3)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_blpwm2_START           (4)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_blpwm2_END             (4)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_i2s_asp2dp_rs_START     (7)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_i2s_asp2dp_rs_END       (7)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_asp_dw_axi_START        (8)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_asp_dw_axi_END          (8)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_asp_x2h_START           (9)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_asp_x2h_END             (9)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_asp_h2p_START           (10)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_asp_h2p_END             (10)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_em_sync_top_START       (11)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_em_sync_top_END         (11)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_ao_ioc_START           (18)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_ao_ioc_END             (18)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_syscnt_START           (19)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_syscnt_END             (19)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_syscnt_START            (20)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_syscnt_END              (20)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_noc_aobus2aspbus_START  (26)
#define SOC_AO_CRG_SCPERRSTEN0_ip_rst_noc_aobus2aspbus_END    (26)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_bbpdrx_START           (27)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_bbpdrx_END             (27)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_ao_ipc_ns_START        (28)
#define SOC_AO_CRG_SCPERRSTEN0_ip_prst_ao_ipc_ns_END          (28)
#define SOC_AO_CRG_SCPERRSTEN0_sc_rst_src_clr_START           (30)
#define SOC_AO_CRG_SCPERRSTEN0_sc_rst_src_clr_END             (30)
#define SOC_AO_CRG_SCPERRSTEN0_sc_pmurst_ctrl_START           (31)
#define SOC_AO_CRG_SCPERRSTEN0_sc_pmurst_ctrl_END             (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTDIS0_UNION
 struct description   : SCPERRSTDIS0 Register structure definition
                        Address Offset:0x204 Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0              : 1;  /* bit[0] : IP软复位撤离：
                                                                0：IP软复位使能状态不变；
                                                                1：IP软复位撤离。 */
        unsigned int  ip_prst_rtc             : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_1              : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  ip_prst_timer0          : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  ip_prst_blpwm2          : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  reserved_2              : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  reserved_3              : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  ip_rst_i2s_asp2dp_rs    : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  ip_rst_asp_dw_axi       : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  ip_rst_asp_x2h          : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  ip_rst_asp_h2p          : 1;  /* bit[10]: 含义同bit0。 */
        unsigned int  ip_rst_em_sync_top      : 1;  /* bit[11]: 含义同bit0。 */
        unsigned int  reserved_4              : 1;  /* bit[12]: 含义同bit0。 */
        unsigned int  reserved_5              : 1;  /* bit[13]: 含义同bit0。 */
        unsigned int  reserved_6              : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  reserved_7              : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  reserved_8              : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  reserved_9              : 1;  /* bit[17]: 保留。 */
        unsigned int  ip_prst_ao_ioc          : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  ip_prst_syscnt          : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  ip_rst_syscnt           : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  reserved_10             : 1;  /* bit[21]: 保留。 */
        unsigned int  reserved_11             : 1;  /* bit[22]:  */
        unsigned int  reserved_12             : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  reserved_13             : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  reserved_14             : 1;  /* bit[25]: 保留。 */
        unsigned int  ip_rst_noc_aobus2aspbus : 1;  /* bit[26]: 含义同bit0。 */
        unsigned int  ip_prst_bbpdrx          : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  ip_prst_ao_ipc_ns       : 1;  /* bit[28]: 保留。 */
        unsigned int  reserved_15             : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  sc_rst_src_clr          : 1;  /* bit[30]: 复位源清除信号控制：
                                                                0：写0无效果；
                                                                1：复位源清除撤销，即复位源记录功能启用。 */
        unsigned int  sc_pmurst_ctrl          : 1;  /* bit[31]: PMU复位功能控制：
                                                                0：写0无效果；
                                                                1：开启PMU复位功能并且关闭AP复位功能。 */
    } reg;
} SOC_AO_CRG_SCPERRSTDIS0_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_rtc_START              (1)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_rtc_END                (1)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_timer0_START           (3)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_timer0_END             (3)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_blpwm2_START           (4)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_blpwm2_END             (4)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_i2s_asp2dp_rs_START     (7)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_i2s_asp2dp_rs_END       (7)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_asp_dw_axi_START        (8)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_asp_dw_axi_END          (8)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_asp_x2h_START           (9)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_asp_x2h_END             (9)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_asp_h2p_START           (10)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_asp_h2p_END             (10)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_em_sync_top_START       (11)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_em_sync_top_END         (11)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_ao_ioc_START           (18)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_ao_ioc_END             (18)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_syscnt_START           (19)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_syscnt_END             (19)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_syscnt_START            (20)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_syscnt_END              (20)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_noc_aobus2aspbus_START  (26)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_rst_noc_aobus2aspbus_END    (26)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_bbpdrx_START           (27)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_bbpdrx_END             (27)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_ao_ipc_ns_START        (28)
#define SOC_AO_CRG_SCPERRSTDIS0_ip_prst_ao_ipc_ns_END          (28)
#define SOC_AO_CRG_SCPERRSTDIS0_sc_rst_src_clr_START           (30)
#define SOC_AO_CRG_SCPERRSTDIS0_sc_rst_src_clr_END             (30)
#define SOC_AO_CRG_SCPERRSTDIS0_sc_pmurst_ctrl_START           (31)
#define SOC_AO_CRG_SCPERRSTDIS0_sc_pmurst_ctrl_END             (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTSTAT0_UNION
 struct description   : SCPERRSTSTAT0 Register structure definition
                        Address Offset:0x208 Initial:0x0 Width:32
 register description : 外设软复位状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0              : 1;  /* bit[0] : 外设软复位使能状态：
                                                                0：IP软复位使能撤销；
                                                                1：IP软复位使能； */
        unsigned int  ip_prst_rtc             : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_1              : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  ip_prst_timer0          : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  ip_prst_blpwm2          : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  reserved_2              : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  reserved_3              : 1;  /* bit[6] : 含义同bit0。 */
        unsigned int  ip_rst_i2s_asp2dp_rs    : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  ip_rst_asp_dw_axi       : 1;  /* bit[8] : 含义同bit0。 */
        unsigned int  ip_rst_asp_x2h          : 1;  /* bit[9] : 含义同bit0。 */
        unsigned int  ip_rst_asp_h2p          : 1;  /* bit[10]: 含义同bit0。 */
        unsigned int  ip_rst_em_sync_top      : 1;  /* bit[11]: 含义同bit0。 */
        unsigned int  reserved_4              : 1;  /* bit[12]: 含义同bit0。 */
        unsigned int  reserved_5              : 1;  /* bit[13]: 含义同bit0。 */
        unsigned int  reserved_6              : 1;  /* bit[14]: 含义同bit0。 */
        unsigned int  reserved_7              : 1;  /* bit[15]: 含义同bit0。 */
        unsigned int  reserved_8              : 1;  /* bit[16]: 含义同bit0。 */
        unsigned int  reserved_9              : 1;  /* bit[17]: 含义同bit0。 */
        unsigned int  ip_prst_ao_ioc          : 1;  /* bit[18]: 含义同bit0。 */
        unsigned int  ip_prst_syscnt          : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  ip_rst_syscnt           : 1;  /* bit[20]: 含义同bit0。 */
        unsigned int  reserved_10             : 1;  /* bit[21]:  */
        unsigned int  reserved_11             : 1;  /* bit[22]:  */
        unsigned int  reserved_12             : 1;  /* bit[23]: 含义同bit0。 */
        unsigned int  reserved_13             : 1;  /* bit[24]: 含义同bit0。 */
        unsigned int  reserved_14             : 1;  /* bit[25]:  */
        unsigned int  ip_rst_noc_aobus2aspbus : 1;  /* bit[26]: 含义同bit0 */
        unsigned int  ip_prst_bbpdrx          : 1;  /* bit[27]: 含义同bit0。 */
        unsigned int  ip_prst_ao_ipc_ns       : 1;  /* bit[28]: 保留。 */
        unsigned int  reserved_15             : 1;  /* bit[29]: 含义同bit0。 */
        unsigned int  sc_rst_src_clr          : 1;  /* bit[30]: 复位源清除控制状态：
                                                                0：保留复位源记录功能；
                                                                1：清除复位源记录功能；
                                                                此bit配置为1后需要再配置为0。 */
        unsigned int  sc_pmurst_ctrl          : 1;  /* bit[31]: PMU复位功能控制状态：
                                                                0：开启PMU复位功能并且禁用AP复位功能；
                                                                1：禁用PMU复位功能并且开启AP复位功能。 */
    } reg;
} SOC_AO_CRG_SCPERRSTSTAT0_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_rtc_START              (1)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_rtc_END                (1)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_timer0_START           (3)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_timer0_END             (3)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_blpwm2_START           (4)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_blpwm2_END             (4)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_i2s_asp2dp_rs_START     (7)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_i2s_asp2dp_rs_END       (7)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_asp_dw_axi_START        (8)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_asp_dw_axi_END          (8)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_asp_x2h_START           (9)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_asp_x2h_END             (9)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_asp_h2p_START           (10)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_asp_h2p_END             (10)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_em_sync_top_START       (11)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_em_sync_top_END         (11)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_ao_ioc_START           (18)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_ao_ioc_END             (18)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_syscnt_START           (19)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_syscnt_END             (19)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_syscnt_START            (20)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_syscnt_END              (20)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_noc_aobus2aspbus_START  (26)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_rst_noc_aobus2aspbus_END    (26)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_bbpdrx_START           (27)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_bbpdrx_END             (27)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_ao_ipc_ns_START        (28)
#define SOC_AO_CRG_SCPERRSTSTAT0_ip_prst_ao_ipc_ns_END          (28)
#define SOC_AO_CRG_SCPERRSTSTAT0_sc_rst_src_clr_START           (30)
#define SOC_AO_CRG_SCPERRSTSTAT0_sc_rst_src_clr_END             (30)
#define SOC_AO_CRG_SCPERRSTSTAT0_sc_pmurst_ctrl_START           (31)
#define SOC_AO_CRG_SCPERRSTSTAT0_sc_pmurst_ctrl_END             (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTEN1_UNION
 struct description   : SCPERRSTEN1 Register structure definition
                        Address Offset:0x20C Initial:0x0 Width:32
 register description : 外设软复位使能寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_spmi            : 1;  /* bit[0]    : IP软复位使能：
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_0             : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  ip_prst_dbg_to_ao      : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  ip_prst_timer4         : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  ip_prst_timer5         : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  ip_prst_timer6         : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_1             : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  ip_prst_timer8         : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  reserved_2             : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_3             : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_4             : 1;  /* bit[10]   :  */
        unsigned int  reserved_5             : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  reserved_6             : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  ip_prst_ao_loadmonitor : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  reserved_7             : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  reserved_8             : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  ip_rst_dram_tmp_0      : 2;  /* bit[16-17]: dram复位配置，DALLAS不使用
                                                                  0：无操作
                                                                  1：对SCPERRSTSTAT1.ip_rst_dram_tmp_0对应bit置1。 */
        unsigned int  reserved_9             : 1;  /* bit[18]   :  */
        unsigned int  ip_rst_ufs_crg         : 1;  /* bit[19]   :  */
        unsigned int  reserved_10            : 1;  /* bit[20]   :  */
        unsigned int  reserved_11            : 1;  /* bit[21]   :  */
        unsigned int  reserved_12            : 1;  /* bit[22]   :  */
        unsigned int  reserved_13            : 1;  /* bit[23]   :  */
        unsigned int  reserved_14            : 1;  /* bit[24]   :  */
        unsigned int  reserved_15            : 1;  /* bit[25]   :  */
        unsigned int  reserved_16            : 1;  /* bit[26]   :  */
        unsigned int  reserved_17            : 1;  /* bit[27]   :  */
        unsigned int  reserved_18            : 1;  /* bit[28]   :  */
        unsigned int  reserved_19            : 1;  /* bit[29]   :  */
        unsigned int  reserved_20            : 1;  /* bit[30]   : xian删除 */
        unsigned int  reserved_21            : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTEN1_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTEN1_ip_rst_spmi_START             (0)
#define SOC_AO_CRG_SCPERRSTEN1_ip_rst_spmi_END               (0)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_dbg_to_ao_START       (2)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_dbg_to_ao_END         (2)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer4_START          (3)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer4_END            (3)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer5_START          (4)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer5_END            (4)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer6_START          (5)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer6_END            (5)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer8_START          (7)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_timer8_END            (7)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_ao_loadmonitor_START  (13)
#define SOC_AO_CRG_SCPERRSTEN1_ip_prst_ao_loadmonitor_END    (13)
#define SOC_AO_CRG_SCPERRSTEN1_ip_rst_dram_tmp_0_START       (16)
#define SOC_AO_CRG_SCPERRSTEN1_ip_rst_dram_tmp_0_END         (17)
#define SOC_AO_CRG_SCPERRSTEN1_ip_rst_ufs_crg_START          (19)
#define SOC_AO_CRG_SCPERRSTEN1_ip_rst_ufs_crg_END            (19)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTDIS1_UNION
 struct description   : SCPERRSTDIS1 Register structure definition
                        Address Offset:0x210 Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_spmi            : 1;  /* bit[0]    : IP软复位撤离：
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位撤离。 */
        unsigned int  reserved_0             : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  ip_prst_dbg_to_ao      : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  ip_prst_timer4         : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  ip_prst_timer5         : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  ip_prst_timer6         : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_1             : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  ip_prst_timer8         : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  reserved_2             : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_3             : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_4             : 1;  /* bit[10]   :  */
        unsigned int  reserved_5             : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  reserved_6             : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  ip_prst_ao_loadmonitor : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  reserved_7             : 1;  /* bit[14]   : 含义同bit0。 */
        unsigned int  reserved_8             : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  ip_rst_dram_tmp_0      : 2;  /* bit[16-17]: dram复位配置，DALLAS不使用
                                                                  0：无操作
                                                                  1：对SCPERRSTSTAT1.ip_rst_dram_tmp_0对应bit清0。 */
        unsigned int  reserved_9             : 1;  /* bit[18]   :  */
        unsigned int  ip_rst_ufs_crg         : 1;  /* bit[19]   :  */
        unsigned int  reserved_10            : 1;  /* bit[20]   :  */
        unsigned int  reserved_11            : 1;  /* bit[21]   :  */
        unsigned int  reserved_12            : 1;  /* bit[22]   :  */
        unsigned int  reserved_13            : 1;  /* bit[23]   :  */
        unsigned int  reserved_14            : 1;  /* bit[24]   :  */
        unsigned int  reserved_15            : 1;  /* bit[25]   :  */
        unsigned int  reserved_16            : 1;  /* bit[26]   :  */
        unsigned int  reserved_17            : 1;  /* bit[27]   :  */
        unsigned int  reserved_18            : 1;  /* bit[28]   :  */
        unsigned int  reserved_19            : 1;  /* bit[29]   :  */
        unsigned int  reserved_20            : 1;  /* bit[30]   :  */
        unsigned int  reserved_21            : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTDIS1_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTDIS1_ip_rst_spmi_START             (0)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_rst_spmi_END               (0)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_dbg_to_ao_START       (2)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_dbg_to_ao_END         (2)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer4_START          (3)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer4_END            (3)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer5_START          (4)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer5_END            (4)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer6_START          (5)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer6_END            (5)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer8_START          (7)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_timer8_END            (7)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_ao_loadmonitor_START  (13)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_prst_ao_loadmonitor_END    (13)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_rst_dram_tmp_0_START       (16)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_rst_dram_tmp_0_END         (17)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_rst_ufs_crg_START          (19)
#define SOC_AO_CRG_SCPERRSTDIS1_ip_rst_ufs_crg_END            (19)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTSTAT1_UNION
 struct description   : SCPERRSTSTAT1 Register structure definition
                        Address Offset:0x214 Initial:0x1 Width:32
 register description : 外设软复位状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_spmi            : 1;  /* bit[0]    : IP软复位撤离：
                                                                  0：IP软复位使能撤销；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_0             : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  ip_prst_dbg_to_ao      : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  ip_prst_timer4         : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  ip_prst_timer5         : 1;  /* bit[4]    : 含义同bit0。 */
        unsigned int  ip_prst_timer6         : 1;  /* bit[5]    : 含义同bit0。 */
        unsigned int  reserved_1             : 1;  /* bit[6]    : 含义同bit0。 */
        unsigned int  ip_prst_timer8         : 1;  /* bit[7]    : 含义同bit0。 */
        unsigned int  reserved_2             : 1;  /* bit[8]    : 含义同bit0。 */
        unsigned int  reserved_3             : 1;  /* bit[9]    : 含义同bit0。 */
        unsigned int  reserved_4             : 1;  /* bit[10]   :  */
        unsigned int  reserved_5             : 1;  /* bit[11]   : 含义同bit0。 */
        unsigned int  reserved_6             : 1;  /* bit[12]   : 含义同bit0。 */
        unsigned int  ip_prst_ao_loadmonitor : 1;  /* bit[13]   : 含义同bit0。 */
        unsigned int  reserved_7             : 1;  /* bit[14]   : 0：解复位；1：复位(默认) */
        unsigned int  reserved_8             : 1;  /* bit[15]   : 含义同bit0。 */
        unsigned int  ip_rst_dram_tmp_0      : 2;  /* bit[16-17]: dram复位状态0，DALLAS不使用
                                                                  10：dram复位条件0满足
                                                                  其他：dram不复位
                                                                  仅当softrst_dram_dis=1'b1时，SCPERRSTSTAT2.ip_rst_dram_tmp_0=2'b10且SCPERRSTSTAT2.ip_rst_dram_tmp_1=2'b10同时满足时，对dram进行软复位，
                                                                  当softrst_dram_dis=1'b1时，SCPERRSTSTAT2.ip_rst_dram_tmp_0=2'b10且SCPERRSTSTAT2.ip_rst_dram_tmp_1=2'b10不满足时，对dram进行软件解复位。 */
        unsigned int  reserved_9             : 1;  /* bit[18]   :  */
        unsigned int  ip_rst_ufs_crg         : 1;  /* bit[19]   : 新增UFSCRG软复位：
                                                                  0：解复位；1：复位(默认) */
        unsigned int  reserved_10            : 1;  /* bit[20]   :  */
        unsigned int  reserved_11            : 1;  /* bit[21]   :  */
        unsigned int  reserved_12            : 1;  /* bit[22]   :  */
        unsigned int  reserved_13            : 1;  /* bit[23]   :  */
        unsigned int  reserved_14            : 1;  /* bit[24]   :  */
        unsigned int  reserved_15            : 1;  /* bit[25]   :  */
        unsigned int  reserved_16            : 1;  /* bit[26]   :  */
        unsigned int  reserved_17            : 1;  /* bit[27]   :  */
        unsigned int  reserved_18            : 1;  /* bit[28]   :  */
        unsigned int  reserved_19            : 1;  /* bit[29]   :  */
        unsigned int  reserved_20            : 1;  /* bit[30]   :  */
        unsigned int  reserved_21            : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERRSTSTAT1_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_rst_spmi_START             (0)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_rst_spmi_END               (0)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_dbg_to_ao_START       (2)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_dbg_to_ao_END         (2)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer4_START          (3)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer4_END            (3)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer5_START          (4)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer5_END            (4)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer6_START          (5)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer6_END            (5)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer8_START          (7)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_timer8_END            (7)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_ao_loadmonitor_START  (13)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_prst_ao_loadmonitor_END    (13)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_rst_dram_tmp_0_START       (16)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_rst_dram_tmp_0_END         (17)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_rst_ufs_crg_START          (19)
#define SOC_AO_CRG_SCPERRSTSTAT1_ip_rst_ufs_crg_END            (19)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTEN2_UNION
 struct description   : SCPERRSTEN2 Register structure definition
                        Address Offset:0x218 Initial:0x0 Width:32
 register description : 外设软复位使能寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]  : IP软复位使能：
                                                            0：IP软复位使能状态不变；
                                                            1：IP软复位使能。 */
        unsigned int  reserved_1         : 1;  /* bit[1]  : 含义同bit0。 */
        unsigned int  reserved_2         : 1;  /* bit[2]  :  */
        unsigned int  ip_rst_spi5        : 1;  /* bit[3]  :  */
        unsigned int  soft_rst_dram_dis  : 1;  /* bit[4]  : dram复位配置使能,DALLAS不使用
                                                            0：无操作
                                                            1：对SCPERRSTSTAT2.softrst_dram_dis对应bit置1。 */
        unsigned int  ip_rst_dram_tmp_1  : 2;  /* bit[5-6]: dram复位配置,DALLAS不使用
                                                            0：无操作
                                                            1：对SCPERRSTSTAT2.ip_rst_dram_tmp_0对应bit置1。 */
        unsigned int  reserved_3         : 1;  /* bit[7]  : 含义同bit0。 */
        unsigned int  reserved_4         : 1;  /* bit[8]  : 含义同bit0。 */
        unsigned int  ip_rst_spi3        : 1;  /* bit[9]  : 含义同bit0。 */
        unsigned int  reserved_5         : 1;  /* bit[10] : 含义同bit0。 */
        unsigned int  reserved_6         : 1;  /* bit[11] :  */
        unsigned int  reserved_7         : 1;  /* bit[12] :  */
        unsigned int  reserved_8         : 1;  /* bit[13] :  */
        unsigned int  reserved_9         : 1;  /* bit[14] :  */
        unsigned int  ip_rst_qspi_sfc_x2 : 1;  /* bit[15] :  */
        unsigned int  ip_prst_timer17    : 1;  /* bit[16] :  */
        unsigned int  ip_prst_timer16    : 1;  /* bit[17] :  */
        unsigned int  ip_prst_timer15    : 1;  /* bit[18] :  */
        unsigned int  ip_prst_timer14    : 1;  /* bit[19] :  */
        unsigned int  ip_prst_timer13    : 1;  /* bit[20] :  */
        unsigned int  reserved_10        : 1;  /* bit[21] :  */
        unsigned int  reserved_11        : 1;  /* bit[22] :  */
        unsigned int  reserved_12        : 1;  /* bit[23] :  */
        unsigned int  reserved_13        : 1;  /* bit[24] :  */
        unsigned int  reserved_14        : 1;  /* bit[25] :  */
        unsigned int  reserved_15        : 1;  /* bit[26] :  */
        unsigned int  reserved_16        : 1;  /* bit[27] :  */
        unsigned int  reserved_17        : 1;  /* bit[28] :  */
        unsigned int  reserved_18        : 1;  /* bit[29] :  */
        unsigned int  reserved_19        : 1;  /* bit[30] :  */
        unsigned int  reserved_20        : 1;  /* bit[31] : 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTEN2_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_spi5_START         (3)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_spi5_END           (3)
#define SOC_AO_CRG_SCPERRSTEN2_soft_rst_dram_dis_START   (4)
#define SOC_AO_CRG_SCPERRSTEN2_soft_rst_dram_dis_END     (4)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_dram_tmp_1_START   (5)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_dram_tmp_1_END     (6)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_spi3_START         (9)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_spi3_END           (9)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_qspi_sfc_x2_START  (15)
#define SOC_AO_CRG_SCPERRSTEN2_ip_rst_qspi_sfc_x2_END    (15)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer17_START     (16)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer17_END       (16)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer16_START     (17)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer16_END       (17)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer15_START     (18)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer15_END       (18)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer14_START     (19)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer14_END       (19)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer13_START     (20)
#define SOC_AO_CRG_SCPERRSTEN2_ip_prst_timer13_END       (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTDIS2_UNION
 struct description   : SCPERRSTDIS2 Register structure definition
                        Address Offset:0x21C Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]  : IP软复位撤销：
                                                            0：IP软复位使能状态不变；
                                                            1：IP软复位撤销。 */
        unsigned int  reserved_1         : 1;  /* bit[1]  : 含义同bit0。 */
        unsigned int  reserved_2         : 1;  /* bit[2]  :  */
        unsigned int  ip_rst_spi5        : 1;  /* bit[3]  :  */
        unsigned int  soft_rst_dram_dis  : 1;  /* bit[4]  : dram复位配置使能,DALLAS不使用
                                                            0：无操作
                                                            1：对SCPERRSTSTAT2.softrst_dram_dis对应bit清0。 */
        unsigned int  ip_rst_dram_tmp_1  : 2;  /* bit[5-6]: dram复位配置,DALLAS不使用
                                                            0：无操作
                                                            1：对SCPERRSTSTAT2.ip_rst_dram_tmp_0对应bit清0。 */
        unsigned int  reserved_3         : 1;  /* bit[7]  : 含义同bit0。 */
        unsigned int  reserved_4         : 1;  /* bit[8]  : 含义同bit0。 */
        unsigned int  ip_rst_spi3        : 1;  /* bit[9]  : 含义同bit0。 */
        unsigned int  reserved_5         : 1;  /* bit[10] : 含义同bit0。 */
        unsigned int  reserved_6         : 1;  /* bit[11] :  */
        unsigned int  reserved_7         : 1;  /* bit[12] :  */
        unsigned int  reserved_8         : 1;  /* bit[13] :  */
        unsigned int  reserved_9         : 1;  /* bit[14] :  */
        unsigned int  ip_rst_qspi_sfc_x2 : 1;  /* bit[15] :  */
        unsigned int  ip_prst_timer17    : 1;  /* bit[16] :  */
        unsigned int  ip_prst_timer16    : 1;  /* bit[17] :  */
        unsigned int  ip_prst_timer15    : 1;  /* bit[18] :  */
        unsigned int  ip_prst_timer14    : 1;  /* bit[19] :  */
        unsigned int  ip_prst_timer13    : 1;  /* bit[20] :  */
        unsigned int  reserved_10        : 1;  /* bit[21] :  */
        unsigned int  reserved_11        : 1;  /* bit[22] :  */
        unsigned int  reserved_12        : 1;  /* bit[23] :  */
        unsigned int  reserved_13        : 1;  /* bit[24] :  */
        unsigned int  reserved_14        : 1;  /* bit[25] :  */
        unsigned int  reserved_15        : 1;  /* bit[26] :  */
        unsigned int  reserved_16        : 1;  /* bit[27] :  */
        unsigned int  reserved_17        : 1;  /* bit[28] :  */
        unsigned int  reserved_18        : 1;  /* bit[29] :  */
        unsigned int  reserved_19        : 1;  /* bit[30] :  */
        unsigned int  reserved_20        : 1;  /* bit[31] : 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTDIS2_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_spi5_START         (3)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_spi5_END           (3)
#define SOC_AO_CRG_SCPERRSTDIS2_soft_rst_dram_dis_START   (4)
#define SOC_AO_CRG_SCPERRSTDIS2_soft_rst_dram_dis_END     (4)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_dram_tmp_1_START   (5)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_dram_tmp_1_END     (6)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_spi3_START         (9)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_spi3_END           (9)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_qspi_sfc_x2_START  (15)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_rst_qspi_sfc_x2_END    (15)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer17_START     (16)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer17_END       (16)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer16_START     (17)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer16_END       (17)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer15_START     (18)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer15_END       (18)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer14_START     (19)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer14_END       (19)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer13_START     (20)
#define SOC_AO_CRG_SCPERRSTDIS2_ip_prst_timer13_END       (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTSTAT2_UNION
 struct description   : SCPERRSTSTAT2 Register structure definition
                        Address Offset:0x220 Initial:0x0 Width:32
 register description : 外设软复位状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0         : 1;  /* bit[0]  : IP软复位状态：
                                                            0：IP软复位使能撤销；
                                                            1：IP软复位使能。 */
        unsigned int  reserved_1         : 1;  /* bit[1]  : 含义同bit0。 */
        unsigned int  reserved_2         : 1;  /* bit[2]  :  */
        unsigned int  ip_rst_spi5        : 1;  /* bit[3]  :  */
        unsigned int  soft_rst_dram_dis  : 1;  /* bit[4]  : dram复位配置使能,DALLAS不使用
                                                            0：dram软复位功能状态保持操作
                                                            1：dram软复位功能可修改，仅当softrst_dram_dis=1'b1时，SCPERRSTSTAT2.ip_rst_dram_tmp_0=2'b10且SCPERRSTSTAT2.ip_rst_dram_tmp_1=2'b10同时满足时，对dram进行软复位，
                                                            当softrst_dram_dis=1'b1时，SCPERRSTSTAT2.ip_rst_dram_tmp_0=2'b10且SCPERRSTSTAT2.ip_rst_dram_tmp_1=2'b10不满足时，对dram进行软件解复位。 */
        unsigned int  ip_rst_dram_tmp_1  : 2;  /* bit[5-6]: dram复位状态1,DALLAS不使用
                                                            10：dram复位条件1满足
                                                            其他：dram不复位
                                                            仅当softrst_dram_dis=1'b1时，SCPERRSTSTAT2.ip_rst_dram_tmp_0=2'b10且SCPERRSTSTAT2.ip_rst_dram_tmp_1=2'b10同时满足时，对dram进行软复位，
                                                            当softrst_dram_dis=1'b1时，SCPERRSTSTAT2.ip_rst_dram_tmp_0=2'b10且SCPERRSTSTAT2.ip_rst_dram_tmp_1=2'b10不满足时，对dram进行软件解复位。 */
        unsigned int  reserved_3         : 1;  /* bit[7]  : 含义同bit0。 */
        unsigned int  reserved_4         : 1;  /* bit[8]  : 含义同bit0。 */
        unsigned int  ip_rst_spi3        : 1;  /* bit[9]  : 含义同bit0。 */
        unsigned int  reserved_5         : 1;  /* bit[10] : 含义同bit0。 */
        unsigned int  reserved_6         : 1;  /* bit[11] :  */
        unsigned int  reserved_7         : 1;  /* bit[12] :  */
        unsigned int  reserved_8         : 1;  /* bit[13] :  */
        unsigned int  reserved_9         : 1;  /* bit[14] :  */
        unsigned int  ip_rst_qspi_sfc_x2 : 1;  /* bit[15] :  */
        unsigned int  ip_prst_timer17    : 1;  /* bit[16] :  */
        unsigned int  ip_prst_timer16    : 1;  /* bit[17] :  */
        unsigned int  ip_prst_timer15    : 1;  /* bit[18] :  */
        unsigned int  ip_prst_timer14    : 1;  /* bit[19] :  */
        unsigned int  ip_prst_timer13    : 1;  /* bit[20] :  */
        unsigned int  reserved_10        : 1;  /* bit[21] :  */
        unsigned int  reserved_11        : 1;  /* bit[22] :  */
        unsigned int  reserved_12        : 1;  /* bit[23] :  */
        unsigned int  reserved_13        : 1;  /* bit[24] :  */
        unsigned int  reserved_14        : 1;  /* bit[25] :  */
        unsigned int  reserved_15        : 1;  /* bit[26] :  */
        unsigned int  reserved_16        : 1;  /* bit[27] :  */
        unsigned int  reserved_17        : 1;  /* bit[28] :  */
        unsigned int  reserved_18        : 1;  /* bit[29] :  */
        unsigned int  reserved_19        : 1;  /* bit[30] :  */
        unsigned int  reserved_20        : 1;  /* bit[31] : 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTSTAT2_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_spi5_START         (3)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_spi5_END           (3)
#define SOC_AO_CRG_SCPERRSTSTAT2_soft_rst_dram_dis_START   (4)
#define SOC_AO_CRG_SCPERRSTSTAT2_soft_rst_dram_dis_END     (4)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_dram_tmp_1_START   (5)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_dram_tmp_1_END     (6)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_spi3_START         (9)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_spi3_END           (9)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_qspi_sfc_x2_START  (15)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_rst_qspi_sfc_x2_END    (15)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer17_START     (16)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer17_END       (16)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer16_START     (17)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer16_END       (17)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer15_START     (18)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer15_END       (18)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer14_START     (19)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer14_END       (19)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer13_START     (20)
#define SOC_AO_CRG_SCPERRSTSTAT2_ip_prst_timer13_END       (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV0_UNION
 struct description   : SCCLKDIV0 Register structure definition
                        Address Offset:0x250 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_asp_subsys    : 3;  /* bit[0-2]  : ASP SUBSYS时钟分频比。（源时钟为PPLL0）
                                                             3'h0:1；
                                                             3'h1:2；
                                                             3'h2:3；
                                                             3'h3:4；
                                                             ……
                                                             3'h7:8。 */
        unsigned int  sc_div_hifidsp    : 6;  /* bit[3-8]  : HIFIDSP时钟分频比。
                                                             6'h0:1；
                                                             6'h1:2；
                                                             6'h2:3；
                                                             6'h3:4；
                                                             …
                                                             6'h1F:64。 */
        unsigned int  div_clk_out2      : 4;  /* bit[9-12] : clk_out2的分频比：
                                                             4'd0: 1分频
                                                             …… 
                                                             4'd15: 16分频 */
        unsigned int  sel_ao_asp_32kpll : 1;  /* bit[13]   : ASP SUBSYS 32K 时钟源选择:
                                                             0:ULPPLL 184M;1:ULPPLL_1 260M */
        unsigned int  reserved_0        : 1;  /* bit[14]   :  */
        unsigned int  reserved_1        : 1;  /* bit[15]   :  */
        unsigned int  bitmasken         : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV0_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV0_div_asp_subsys_START     (0)
#define SOC_AO_CRG_SCCLKDIV0_div_asp_subsys_END       (2)
#define SOC_AO_CRG_SCCLKDIV0_sc_div_hifidsp_START     (3)
#define SOC_AO_CRG_SCCLKDIV0_sc_div_hifidsp_END       (8)
#define SOC_AO_CRG_SCCLKDIV0_div_clk_out2_START       (9)
#define SOC_AO_CRG_SCCLKDIV0_div_clk_out2_END         (12)
#define SOC_AO_CRG_SCCLKDIV0_sel_ao_asp_32kpll_START  (13)
#define SOC_AO_CRG_SCCLKDIV0_sel_ao_asp_32kpll_END    (13)
#define SOC_AO_CRG_SCCLKDIV0_bitmasken_START          (16)
#define SOC_AO_CRG_SCCLKDIV0_bitmasken_END            (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV1_UNION
 struct description   : SCCLKDIV1 Register structure definition
                        Address Offset:0x254 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 6;  /* bit[0-5]  : xian删除
                                                                mudp sys的分频比：
                                                                6'd0: 1分频
                                                                6'd1: 2分频
                                                                ……
                                                                6'd63:64分频 */
        unsigned int  sc_gt_clk_mad_spll   : 1;  /* bit[6]    :  */
        unsigned int  reserved_1           : 5;  /* bit[7-11] :  */
        unsigned int  sel_clk_fll_test_src : 4;  /* bit[12-15]: 拓宽至4bit
                                                                ao区域时钟test point时钟源选择（需要通过clk_out0选择输出）：
                                                                4'd0:clk_ulppll_1
                                                                4'd1:clk_aupll
                                                                4'd2:clk_fll_src
                                                                4'd3:clk_spll
                                                                4'd4:clk_ufs_ref_test
                                                                4'd5:clk_aobus_ini
                                                                4'd6:clk_mad
                                                                4'd7:clk_asp_codec
                                                                4'd8~4'd15:clk_dp_audio_pll_tp; */
        unsigned int  bitmasken            : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV1_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV1_sc_gt_clk_mad_spll_START    (6)
#define SOC_AO_CRG_SCCLKDIV1_sc_gt_clk_mad_spll_END      (6)
#define SOC_AO_CRG_SCCLKDIV1_sel_clk_fll_test_src_START  (12)
#define SOC_AO_CRG_SCCLKDIV1_sel_clk_fll_test_src_END    (15)
#define SOC_AO_CRG_SCCLKDIV1_bitmasken_START             (16)
#define SOC_AO_CRG_SCCLKDIV1_bitmasken_END               (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV2_UNION
 struct description   : SCCLKDIV2 Register structure definition
                        Address Offset:0x258 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_asp_subsys : 1;  /* bit[0]    : 时钟门控配置：
                                                                0：时钟关闭；
                                                                1：时钟开启。 */
        unsigned int  gt_hifidsp_clk_div   : 1;  /* bit[1]    : 时钟门控配置：
                                                                0：时钟关闭；
                                                                1：时钟开启。 */
        unsigned int  reserved_0           : 1;  /* bit[2]    :  */
        unsigned int  reserved_1           : 1;  /* bit[3]    :  */
        unsigned int  gt_asp_hclk_div      : 1;  /* bit[4]    : 时钟门控配置：
                                                                0：时钟关闭；
                                                                1：时钟开启。 */
        unsigned int  reserved_2           : 1;  /* bit[5]    :  */
        unsigned int  gt_clk_spmi_mst_vote : 10; /* bit[6-15] : clk_spmi_mst投票，共10个投票源。
                                                                1'b0: 关时钟；1'b1：开时钟。
                                                                bit[15~12]: reserved
                                                                bit[11]: mdm
                                                                bit[10]: acpu_big
                                                                bit[9]: acpu_lillte
                                                                bit[8]: asp
                                                                bit[7]: iomcu
                                                                bit[6]：lpmcu。 */
        unsigned int  bitmasken            : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV2_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV2_sc_gt_clk_asp_subsys_START  (0)
#define SOC_AO_CRG_SCCLKDIV2_sc_gt_clk_asp_subsys_END    (0)
#define SOC_AO_CRG_SCCLKDIV2_gt_hifidsp_clk_div_START    (1)
#define SOC_AO_CRG_SCCLKDIV2_gt_hifidsp_clk_div_END      (1)
#define SOC_AO_CRG_SCCLKDIV2_gt_asp_hclk_div_START       (4)
#define SOC_AO_CRG_SCCLKDIV2_gt_asp_hclk_div_END         (4)
#define SOC_AO_CRG_SCCLKDIV2_gt_clk_spmi_mst_vote_START  (6)
#define SOC_AO_CRG_SCCLKDIV2_gt_clk_spmi_mst_vote_END    (15)
#define SOC_AO_CRG_SCCLKDIV2_bitmasken_START             (16)
#define SOC_AO_CRG_SCCLKDIV2_bitmasken_END               (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV3_UNION
 struct description   : SCCLKDIV3 Register structure definition
                        Address Offset:0x25C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_hsdt_subsys            : 6;  /* bit[0-5]  : hsdt中ip侧总线时钟分频比：
                                                                      6'h0:1分频；
                                                                      6'h1:2分频；
                                                                      …
                                                                      6'h3f：64分频； */
        unsigned int  hsdt_subsys_clk_sw_req_cfg : 2;  /* bit[6-7]  : 系统工作在NORMAL模式时，软件控制hsdt subsys时钟切换配置(高bit 控制切换到32K，低bit控制切换19.2M。软件需要保证先切换到19.2M，再切换到32K；先切回32K，再切换到PLL)：
                                                                      2'b11:时钟切换至32K时钟；
                                                                      2'b01:时钟切换至19.2MHz晶振分频时钟；
                                                                      2'b00:时钟源为PLL时钟；
                                                                      2'b10:不允许 */
        unsigned int  reserved_0                 : 2;  /* bit[8-9]  : clk_ao_tof时钟源选择:
                                                                      2'b00:clkin_sys
                                                                      2'b01:clk_fll_src
                                                                      2'b10:clk_ao_332m_div
                                                                      2'b11:clk_isp_snclk1 */
        unsigned int  reserved_1                 : 6;  /* bit[10-15]: ao_tof的分频比：
                                                                      6'd0: 1分频
                                                                      6'd1: 2分频
                                                                      ……
                                                                      6'd63:64分频 */
        unsigned int  bitmasken                  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV3_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV3_div_hsdt_subsys_START             (0)
#define SOC_AO_CRG_SCCLKDIV3_div_hsdt_subsys_END               (5)
#define SOC_AO_CRG_SCCLKDIV3_hsdt_subsys_clk_sw_req_cfg_START  (6)
#define SOC_AO_CRG_SCCLKDIV3_hsdt_subsys_clk_sw_req_cfg_END    (7)
#define SOC_AO_CRG_SCCLKDIV3_bitmasken_START                   (16)
#define SOC_AO_CRG_SCCLKDIV3_bitmasken_END                     (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV4_UNION
 struct description   : SCCLKDIV4 Register structure definition
                        Address Offset:0x260 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_ao_camera      : 6;  /* bit[0-5]  : ao_camera的分频比：
                                                              6'd0: 1分频
                                                              6'd1: 2分频
                                                              ……
                                                              6'd63:64分频 */
        unsigned int  sel_clk_ao_camera1 : 2;  /* bit[6-7]  : clk_ao_camera1时钟源选择:
                                                              2'b00:clkin_sys
                                                              2'b01:clk_fll_src
                                                              2'b10:clk_ao_332m_div
                                                              2'b11:clk_isp_snclk1 */
        unsigned int  sc_div_asp_hclk    : 2;  /* bit[8-9]  : asp hclk分频比。(源时钟为clk_asp_subsys)
                                                              2'h0:1；
                                                              2'h1:2；
                                                              2'h2:3；
                                                              2'h3:4。 */
        unsigned int  div_ao_camera1     : 6;  /* bit[10-15]: ao_camera1的分频比：
                                                              6'd0: 1分频
                                                              6'd1: 2分频
                                                              ……
                                                              6'd63:64分频 */
        unsigned int  bitmasken          : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV4_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV4_div_ao_camera_START       (0)
#define SOC_AO_CRG_SCCLKDIV4_div_ao_camera_END         (5)
#define SOC_AO_CRG_SCCLKDIV4_sel_clk_ao_camera1_START  (6)
#define SOC_AO_CRG_SCCLKDIV4_sel_clk_ao_camera1_END    (7)
#define SOC_AO_CRG_SCCLKDIV4_sc_div_asp_hclk_START     (8)
#define SOC_AO_CRG_SCCLKDIV4_sc_div_asp_hclk_END       (9)
#define SOC_AO_CRG_SCCLKDIV4_div_ao_camera1_START      (10)
#define SOC_AO_CRG_SCCLKDIV4_div_ao_camera1_END        (15)
#define SOC_AO_CRG_SCCLKDIV4_bitmasken_START           (16)
#define SOC_AO_CRG_SCCLKDIV4_bitmasken_END             (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV5_UNION
 struct description   : SCCLKDIV5 Register structure definition
                        Address Offset:0x264 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_ufs_mphy_sys : 1;  /* bit[0]    :  */
        unsigned int  div_ufs_mphy_sys       : 4;  /* bit[1-4]  :  */
        unsigned int  div_hsdt_subsys_fll    : 6;  /* bit[5-10] : HSDT SUBSYS ULPPLL通路分频比。
                                                                  6'h0:1；
                                                                  6'h1:2；
                                                                  6'h2:3；
                                                                  …
                                                                  6'h3E:63；
                                                                  6'h3F:64； */
        unsigned int  reserved               : 2;  /* bit[11-12]:  */
        unsigned int  sel_syscnt             : 1;  /* bit[13]   : 时钟源选择: 0: clk_sys div
                                                                  1：32K */
        unsigned int  sc_gt_clk_blpwm2       : 1;  /* bit[14]   :  */
        unsigned int  sc_gt_clk_mad_fll      : 1;  /* bit[15]   :  */
        unsigned int  bitmasken              : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV5_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV5_sc_gt_clk_ufs_mphy_sys_START  (0)
#define SOC_AO_CRG_SCCLKDIV5_sc_gt_clk_ufs_mphy_sys_END    (0)
#define SOC_AO_CRG_SCCLKDIV5_div_ufs_mphy_sys_START        (1)
#define SOC_AO_CRG_SCCLKDIV5_div_ufs_mphy_sys_END          (4)
#define SOC_AO_CRG_SCCLKDIV5_div_hsdt_subsys_fll_START     (5)
#define SOC_AO_CRG_SCCLKDIV5_div_hsdt_subsys_fll_END       (10)
#define SOC_AO_CRG_SCCLKDIV5_sel_syscnt_START              (13)
#define SOC_AO_CRG_SCCLKDIV5_sel_syscnt_END                (13)
#define SOC_AO_CRG_SCCLKDIV5_sc_gt_clk_blpwm2_START        (14)
#define SOC_AO_CRG_SCCLKDIV5_sc_gt_clk_blpwm2_END          (14)
#define SOC_AO_CRG_SCCLKDIV5_sc_gt_clk_mad_fll_START       (15)
#define SOC_AO_CRG_SCCLKDIV5_sc_gt_clk_mad_fll_END         (15)
#define SOC_AO_CRG_SCCLKDIV5_bitmasken_START               (16)
#define SOC_AO_CRG_SCCLKDIV5_bitmasken_END                 (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV6_UNION
 struct description   : SCCLKDIV6 Register structure definition
                        Address Offset:0x268 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_mad_spll : 4;  /* bit[0-3]  : mad主时钟spll通路分频比：
                                                        4'd0: 1分频
                                                        4'd1: 2分频
                                                        ……
                                                        4'd15:16分频 */
        unsigned int  div_syscnt   : 4;  /* bit[4-7]  : syscnt分频比。 */
        unsigned int  reserved_0   : 1;  /* bit[8]    : xian删除 
                                                        clk_out_hiphy时钟源选择：
                                                        1'b0：clkin_sys通路；
                                                        1'b1：clkin_sys_div通路； */
        unsigned int  sel_clk_out2 : 2;  /* bit[9-10] : clk_out2时钟源选择:
                                                        2'b0X:clkin_sys通路
                                                        2‘b10:PLL通路
                                                        2’b11:FLL通路 */
        unsigned int  reserved_1   : 5;  /* bit[11-15]:  */
        unsigned int  bitmasken    : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV6_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV6_div_mad_spll_START  (0)
#define SOC_AO_CRG_SCCLKDIV6_div_mad_spll_END    (3)
#define SOC_AO_CRG_SCCLKDIV6_div_syscnt_START    (4)
#define SOC_AO_CRG_SCCLKDIV6_div_syscnt_END      (7)
#define SOC_AO_CRG_SCCLKDIV6_sel_clk_out2_START  (9)
#define SOC_AO_CRG_SCCLKDIV6_sel_clk_out2_END    (10)
#define SOC_AO_CRG_SCCLKDIV6_bitmasken_START     (16)
#define SOC_AO_CRG_SCCLKDIV6_bitmasken_END       (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV7_UNION
 struct description   : SCCLKDIV7 Register structure definition
                        Address Offset:0x26C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_memrepair      : 1;  /* bit[0]    : memrepair 时钟分频前门控： */
        unsigned int  reserved_0               : 1;  /* bit[1]    :  */
        unsigned int  div_memrepair            : 6;  /* bit[2-7]  : memrepair的分频比：
                                                                    6'd0: 1分频
                                                                    6'd1: 2分频
                                                                    ……
                                                                    6'd63:64分频 */
        unsigned int  reserved_1               : 1;  /* bit[8]    : sel_clk_out_ufs xian删除
                                                                    ASP SUBSYS 32K 时钟源选择:
                                                                    0:38.4M;1:19.2M */
        unsigned int  sc_gt_clk_ao_loadmonitor : 1;  /* bit[9]    : aoloadmonitor 时钟分频前门控： */
        unsigned int  div_ao_loadmonitor       : 4;  /* bit[10-13]: aoloadmonitor 时钟分频比配置： */
        unsigned int  reserved_2               : 2;  /* bit[14-15]:  */
        unsigned int  bitmasken                : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV7_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV7_sc_gt_clk_memrepair_START       (0)
#define SOC_AO_CRG_SCCLKDIV7_sc_gt_clk_memrepair_END         (0)
#define SOC_AO_CRG_SCCLKDIV7_div_memrepair_START             (2)
#define SOC_AO_CRG_SCCLKDIV7_div_memrepair_END               (7)
#define SOC_AO_CRG_SCCLKDIV7_sc_gt_clk_ao_loadmonitor_START  (9)
#define SOC_AO_CRG_SCCLKDIV7_sc_gt_clk_ao_loadmonitor_END    (9)
#define SOC_AO_CRG_SCCLKDIV7_div_ao_loadmonitor_START        (10)
#define SOC_AO_CRG_SCCLKDIV7_div_ao_loadmonitor_END          (13)
#define SOC_AO_CRG_SCCLKDIV7_bitmasken_START                 (16)
#define SOC_AO_CRG_SCCLKDIV7_bitmasken_END                   (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV8_UNION
 struct description   : SCCLKDIV8 Register structure definition
                        Address Offset:0x270 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_em_sync_top       : 6;  /* bit[0-5]  :  */
        unsigned int  sc_gt_clk_em_sync_top : 1;  /* bit[6]    :  */
        unsigned int  sel_mad_mux           : 1;  /* bit[7]    : mad主时钟时钟源选择：
                                                                 1'b0: spll通路
                                                                 1'b1: fll通路 */
        unsigned int  sc_gt_clk_spmi_mst    : 1;  /* bit[8]    :  */
        unsigned int  div_spmi_mst          : 6;  /* bit[9-14] :  */
        unsigned int  sel_spmi_mst          : 1;  /* bit[15]   : 1'b0: fll;1'b1: spll;  */
        unsigned int  bitmasken             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV8_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV8_div_em_sync_top_START        (0)
#define SOC_AO_CRG_SCCLKDIV8_div_em_sync_top_END          (5)
#define SOC_AO_CRG_SCCLKDIV8_sc_gt_clk_em_sync_top_START  (6)
#define SOC_AO_CRG_SCCLKDIV8_sc_gt_clk_em_sync_top_END    (6)
#define SOC_AO_CRG_SCCLKDIV8_sel_mad_mux_START            (7)
#define SOC_AO_CRG_SCCLKDIV8_sel_mad_mux_END              (7)
#define SOC_AO_CRG_SCCLKDIV8_sc_gt_clk_spmi_mst_START     (8)
#define SOC_AO_CRG_SCCLKDIV8_sc_gt_clk_spmi_mst_END       (8)
#define SOC_AO_CRG_SCCLKDIV8_div_spmi_mst_START           (9)
#define SOC_AO_CRG_SCCLKDIV8_div_spmi_mst_END             (14)
#define SOC_AO_CRG_SCCLKDIV8_sel_spmi_mst_START           (15)
#define SOC_AO_CRG_SCCLKDIV8_sel_spmi_mst_END             (15)
#define SOC_AO_CRG_SCCLKDIV8_bitmasken_START              (16)
#define SOC_AO_CRG_SCCLKDIV8_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV9_UNION
 struct description   : SCCLKDIV9 Register structure definition
                        Address Offset:0x274 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                 : 6;  /* bit[0-5]  :  */
        unsigned int  sc_gt_clk_hsdt_subsys_fll  : 1;  /* bit[6]    :  */
        unsigned int  sc_gt_clk_hsdt_subsys      : 1;  /* bit[7]    :  */
        unsigned int  sc_gt_clk_asp_codec_ulppll : 1;  /* bit[8]    :  */
        unsigned int  sc_gt_clk_swd2jtag_dap     : 1;  /* bit[9]    :  */
        unsigned int  reserved_1                 : 1;  /* bit[10]   : xian删除 */
        unsigned int  reserved_2                 : 1;  /* bit[11]   :  */
        unsigned int  sc_gt_clk_ao_camera1       : 1;  /* bit[12]   :  */
        unsigned int  gt_clk_dp_audio_pll_tp     : 1;  /* bit[13]   : DP AUDIO PLL测试通路门控，默认关 */
        unsigned int  sc_gt_clk_asp_codec_pll    : 1;  /* bit[14]   :  */
        unsigned int  sc_gt_clk_asp_subsys_fll   : 1;  /* bit[15]   :  */
        unsigned int  bitmasken                  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV9_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_hsdt_subsys_fll_START   (6)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_hsdt_subsys_fll_END     (6)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_hsdt_subsys_START       (7)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_hsdt_subsys_END         (7)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_asp_codec_ulppll_START  (8)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_asp_codec_ulppll_END    (8)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_swd2jtag_dap_START      (9)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_swd2jtag_dap_END        (9)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_ao_camera1_START        (12)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_ao_camera1_END          (12)
#define SOC_AO_CRG_SCCLKDIV9_gt_clk_dp_audio_pll_tp_START      (13)
#define SOC_AO_CRG_SCCLKDIV9_gt_clk_dp_audio_pll_tp_END        (13)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_asp_codec_pll_START     (14)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_asp_codec_pll_END       (14)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_asp_subsys_fll_START    (15)
#define SOC_AO_CRG_SCCLKDIV9_sc_gt_clk_asp_subsys_fll_END      (15)
#define SOC_AO_CRG_SCCLKDIV9_bitmasken_START                   (16)
#define SOC_AO_CRG_SCCLKDIV9_bitmasken_END                     (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_UNION
 struct description   : SC_INTR_NOCBUS_CTRL Register structure definition
                        Address Offset:0x27C Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_mask_asp_nonidle_pend       : 1;  /* bit[0]    :  */
        unsigned int  intr_mask_iomcu_nonidle_pend     : 1;  /* bit[1]    :  */
        unsigned int  intr_mask_hisee_nonidle_pend     : 1;  /* bit[2]    :  */
        unsigned int  intr_mask_hieps_nonidle_pend     : 1;  /* bit[3]    :  */
        unsigned int  intr_mask_hieps_ree_nonidle_pend : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                       : 3;  /* bit[5-7]  :  */
        unsigned int  intr_clr_asp_nonidle_pend        : 1;  /* bit[8]    :  */
        unsigned int  intr_clr_iomcu_nonidle_pend      : 1;  /* bit[9]    :  */
        unsigned int  intr_clr_hisee_nonidle_pend      : 1;  /* bit[10]   :  */
        unsigned int  intr_clr_hieps_nonidle_pend      : 1;  /* bit[11]   :  */
        unsigned int  intr_clr_hieps_ree_nonidle_pend  : 1;  /* bit[12]   :  */
        unsigned int  reserved_1                       : 3;  /* bit[13-15]:  */
        unsigned int  bitmasken                        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_UNION;
#endif
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_asp_nonidle_pend_START        (0)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_asp_nonidle_pend_END          (0)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_iomcu_nonidle_pend_START      (1)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_iomcu_nonidle_pend_END        (1)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_hisee_nonidle_pend_START      (2)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_hisee_nonidle_pend_END        (2)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_hieps_nonidle_pend_START      (3)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_hieps_nonidle_pend_END        (3)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_hieps_ree_nonidle_pend_START  (4)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_mask_hieps_ree_nonidle_pend_END    (4)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_asp_nonidle_pend_START         (8)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_asp_nonidle_pend_END           (8)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_iomcu_nonidle_pend_START       (9)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_iomcu_nonidle_pend_END         (9)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_hisee_nonidle_pend_START       (10)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_hisee_nonidle_pend_END         (10)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_hieps_nonidle_pend_START       (11)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_hieps_nonidle_pend_END         (11)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_hieps_ree_nonidle_pend_START   (12)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_intr_clr_hieps_ree_nonidle_pend_END     (12)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_bitmasken_START                         (16)
#define SOC_AO_CRG_SC_INTR_NOCBUS_CTRL_bitmasken_END                           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV10_UNION
 struct description   : SCCLKDIV10 Register structure definition
                        Address Offset:0x280 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sel_asp_subsys     : 4;  /* bit[0-3]  : asp subsys时钟源选择：
                                                              4'b0001:spll通路
                                                              4'b0010:ULPPLL_1(260MHz)通路
                                                              4'b0100:clk_ao_asp通路
                                                              4'b1000:clk_sys通路 */
        unsigned int  div_asp_subsys_fll : 2;  /* bit[4-5]  : asp subsys时钟ulppll_1通路分频比：
                                                              2'd0: 1分频…… 2'd3: 4分频 */
        unsigned int  reserved           : 5;  /* bit[6-10] :  */
        unsigned int  div_fll_src_tp     : 5;  /* bit[11-15]: ao区域时钟test point的分频比：
                                                              5'd0: 1分频
                                                              5'd1: 2分频
                                                              ……
                                                              5'd31:32分频 */
        unsigned int  bitmasken          : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV10_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV10_sel_asp_subsys_START      (0)
#define SOC_AO_CRG_SCCLKDIV10_sel_asp_subsys_END        (3)
#define SOC_AO_CRG_SCCLKDIV10_div_asp_subsys_fll_START  (4)
#define SOC_AO_CRG_SCCLKDIV10_div_asp_subsys_fll_END    (5)
#define SOC_AO_CRG_SCCLKDIV10_div_fll_src_tp_START      (11)
#define SOC_AO_CRG_SCCLKDIV10_div_fll_src_tp_END        (15)
#define SOC_AO_CRG_SCCLKDIV10_bitmasken_START           (16)
#define SOC_AO_CRG_SCCLKDIV10_bitmasken_END             (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV11_UNION
 struct description   : SCCLKDIV11 Register structure definition
                        Address Offset:0x284 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_asp_codec  : 6;  /* bit[0-5]  : clk_asp_codec的spll通路的分频比：
                                                          6'd0: 1分频
                                                          6'd1: 2分频
                                                          ……
                                                          6'd3F:64分频 */
        unsigned int  sel_clk_out0   : 2;  /* bit[6-7]  : clk_out0时钟源选择:
                                                          2'b00:spll通路
                                                          2’b01:fll通路
                                                          2‘b10:clk_sys通路
                                                          2’b11:test point通路 */
        unsigned int  sc_gt_clk_out0 : 1;  /* bit[8]    :  */
        unsigned int  sc_gt_clk_out2 : 1;  /* bit[9]    :  */
        unsigned int  div_clk_out0   : 4;  /* bit[10-13]: clk_out0的分频比：
                                                          2'd0: 1分频
                                                          ……
                                                          2'd3: 4分频 */
        unsigned int  reserved_0     : 1;  /* bit[14]   :  */
        unsigned int  reserved_1     : 1;  /* bit[15]   :  */
        unsigned int  bitmasken      : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV11_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV11_div_asp_codec_START   (0)
#define SOC_AO_CRG_SCCLKDIV11_div_asp_codec_END     (5)
#define SOC_AO_CRG_SCCLKDIV11_sel_clk_out0_START    (6)
#define SOC_AO_CRG_SCCLKDIV11_sel_clk_out0_END      (7)
#define SOC_AO_CRG_SCCLKDIV11_sc_gt_clk_out0_START  (8)
#define SOC_AO_CRG_SCCLKDIV11_sc_gt_clk_out0_END    (8)
#define SOC_AO_CRG_SCCLKDIV11_sc_gt_clk_out2_START  (9)
#define SOC_AO_CRG_SCCLKDIV11_sc_gt_clk_out2_END    (9)
#define SOC_AO_CRG_SCCLKDIV11_div_clk_out0_START    (10)
#define SOC_AO_CRG_SCCLKDIV11_div_clk_out0_END      (13)
#define SOC_AO_CRG_SCCLKDIV11_bitmasken_START       (16)
#define SOC_AO_CRG_SCCLKDIV11_bitmasken_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV12_UNION
 struct description   : SCCLKDIV12 Register structure definition
                        Address Offset:0x288 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_noc_timeout_extref_peri_bypass : 1;  /* bit[0]    :  */
        unsigned int  gt_timerclk_refh_peri_bypass          : 1;  /* bit[1]    :  */
        unsigned int  gt_clkin_sys_peri_bypass              : 1;  /* bit[2]    :  */
        unsigned int  gt_clkin_ref_peri_bypass              : 1;  /* bit[3]    :  */
        unsigned int  gt_clk_sys_ini_peri_bypass            : 1;  /* bit[4]    :  */
        unsigned int  gt_clk_hsdt_subsys_bypass             : 1;  /* bit[5]    :  */
        unsigned int  gt_clk_ref_peri_bypass                : 1;  /* bit[6]    :  */
        unsigned int  gt_clk_sys_peri_bypass                : 1;  /* bit[7]    :  */
        unsigned int  gt_clk_aobus_peri_bypass              : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_fll_src_peri_bypass            : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                            : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                            : 1;  /* bit[11]   :  */
        unsigned int  gt_clk_spll_peri_bypass               : 1;  /* bit[12]   :  */
        unsigned int  gt_clkin_sys_logic_peri_bypass        : 1;  /* bit[13]   : clk_32kpll_src时钟源选择：
                                                                                 1'b0:ulppll通路
                                                                                 1'b1:lbintJpll通路 */
        unsigned int  sel_clk_pcie_aux                      : 1;  /* bit[14]   : clk_hsdt0_pcie_aux时钟源选择：
                                                                                 1'b0:19.2MHz通路
                                                                                 1'b1:32KPLL通路 */
        unsigned int  reserved_2                            : 1;  /* bit[15]   :  */
        unsigned int  bitmasken                             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV12_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_noc_timeout_extref_peri_bypass_START  (0)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_noc_timeout_extref_peri_bypass_END    (0)
#define SOC_AO_CRG_SCCLKDIV12_gt_timerclk_refh_peri_bypass_START           (1)
#define SOC_AO_CRG_SCCLKDIV12_gt_timerclk_refh_peri_bypass_END             (1)
#define SOC_AO_CRG_SCCLKDIV12_gt_clkin_sys_peri_bypass_START               (2)
#define SOC_AO_CRG_SCCLKDIV12_gt_clkin_sys_peri_bypass_END                 (2)
#define SOC_AO_CRG_SCCLKDIV12_gt_clkin_ref_peri_bypass_START               (3)
#define SOC_AO_CRG_SCCLKDIV12_gt_clkin_ref_peri_bypass_END                 (3)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_sys_ini_peri_bypass_START             (4)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_sys_ini_peri_bypass_END               (4)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_hsdt_subsys_bypass_START              (5)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_hsdt_subsys_bypass_END                (5)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_ref_peri_bypass_START                 (6)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_ref_peri_bypass_END                   (6)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_sys_peri_bypass_START                 (7)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_sys_peri_bypass_END                   (7)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_aobus_peri_bypass_START               (8)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_aobus_peri_bypass_END                 (8)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_fll_src_peri_bypass_START             (9)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_fll_src_peri_bypass_END               (9)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_spll_peri_bypass_START                (12)
#define SOC_AO_CRG_SCCLKDIV12_gt_clk_spll_peri_bypass_END                  (12)
#define SOC_AO_CRG_SCCLKDIV12_gt_clkin_sys_logic_peri_bypass_START         (13)
#define SOC_AO_CRG_SCCLKDIV12_gt_clkin_sys_logic_peri_bypass_END           (13)
#define SOC_AO_CRG_SCCLKDIV12_sel_clk_pcie_aux_START                       (14)
#define SOC_AO_CRG_SCCLKDIV12_sel_clk_pcie_aux_END                         (14)
#define SOC_AO_CRG_SCCLKDIV12_bitmasken_START                              (16)
#define SOC_AO_CRG_SCCLKDIV12_bitmasken_END                                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV13_UNION
 struct description   : SCCLKDIV13 Register structure definition
                        Address Offset:0x28C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                 : 1;  /* bit[0]    :  */
        unsigned int  reserved_1                 : 1;  /* bit[1]    :  */
        unsigned int  reserved_2                 : 1;  /* bit[2]    :  */
        unsigned int  reserved_3                 : 1;  /* bit[3]    :  */
        unsigned int  reserved_4                 : 1;  /* bit[4]    :  */
        unsigned int  reserved_5                 : 1;  /* bit[5]    :  */
        unsigned int  reserved_6                 : 1;  /* bit[6]    :  */
        unsigned int  reserved_7                 : 1;  /* bit[7]    :  */
        unsigned int  reserved_8                 : 1;  /* bit[8]    :  */
        unsigned int  reserved_9                 : 1;  /* bit[9]    :  */
        unsigned int  reserved_10                : 1;  /* bit[10]   :  */
        unsigned int  sleep_mode_spi5_bypass     : 1;  /* bit[11]   :  */
        unsigned int  sleep_mode_dp_audio_bypass : 1;  /* bit[12]   :  */
        unsigned int  sleep_mode_spi3_bypass     : 1;  /* bit[13]   :  */
        unsigned int  sleep_mode_ufs_bypass      : 1;  /* bit[14]   :  */
        unsigned int  sleep_mode_cssys_bypass    : 1;  /* bit[15]   :  */
        unsigned int  bitmasken                  : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV13_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_spi5_bypass_START      (11)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_spi5_bypass_END        (11)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_dp_audio_bypass_START  (12)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_dp_audio_bypass_END    (12)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_spi3_bypass_START      (13)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_spi3_bypass_END        (13)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_ufs_bypass_START       (14)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_ufs_bypass_END         (14)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_cssys_bypass_START     (15)
#define SOC_AO_CRG_SCCLKDIV13_sleep_mode_cssys_bypass_END       (15)
#define SOC_AO_CRG_SCCLKDIV13_bitmasken_START                   (16)
#define SOC_AO_CRG_SCCLKDIV13_bitmasken_END                     (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV14_UNION
 struct description   : SCCLKDIV14 Register structure definition
                        Address Offset:0x290 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_memrepair_fll           : 4;  /* bit[0-3]  : bisr的fll通路分频比：
                                                                       4'd0: 1分频
                                                                       4'd1: 2分频
                                                                       ……
                                                                       4'd15:16分频 */
        unsigned int  sel_clk_ao_camera           : 2;  /* bit[4-5]  : clk_ao_camera时钟源选择:
                                                                       2'b00:clkin_sys
                                                                       2'b01:clk_fll_src
                                                                       2'b10:clk_ao_332m_div
                                                                       2'b11:clk_isp_snclk1 */
        unsigned int  sel_memrepair_peri          : 2;  /* bit[6-7]  : peri bisr时钟源选择:
                                                                       2'b00:spll通路
                                                                       2'b01:clk_sys通路
                                                                       2'b1X:ulppll通路 */
        unsigned int  div_swd2jtag_dap            : 4;  /* bit[8-11] : SWD2JTAG通路分频比：
                                                                       4'd0: 1分频
                                                                       4'd1: 2分频
                                                                       ……
                                                                       4'd15:16分频 */
        unsigned int  sel_memrepair               : 2;  /* bit[12-13]: bisr时钟源选择:
                                                                       2'b00:spll通路
                                                                       2'b01:clk_sys通路
                                                                       2'b1X:ulppll通路 */
        unsigned int  sc_gt_clk_ao_camera         : 1;  /* bit[14]   :  */
        unsigned int  sc_gt_clk_memrepair_fll_div : 1;  /* bit[15]   :  */
        unsigned int  bitmasken                   : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV14_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV14_div_memrepair_fll_START            (0)
#define SOC_AO_CRG_SCCLKDIV14_div_memrepair_fll_END              (3)
#define SOC_AO_CRG_SCCLKDIV14_sel_clk_ao_camera_START            (4)
#define SOC_AO_CRG_SCCLKDIV14_sel_clk_ao_camera_END              (5)
#define SOC_AO_CRG_SCCLKDIV14_sel_memrepair_peri_START           (6)
#define SOC_AO_CRG_SCCLKDIV14_sel_memrepair_peri_END             (7)
#define SOC_AO_CRG_SCCLKDIV14_div_swd2jtag_dap_START             (8)
#define SOC_AO_CRG_SCCLKDIV14_div_swd2jtag_dap_END               (11)
#define SOC_AO_CRG_SCCLKDIV14_sel_memrepair_START                (12)
#define SOC_AO_CRG_SCCLKDIV14_sel_memrepair_END                  (13)
#define SOC_AO_CRG_SCCLKDIV14_sc_gt_clk_ao_camera_START          (14)
#define SOC_AO_CRG_SCCLKDIV14_sc_gt_clk_ao_camera_END            (14)
#define SOC_AO_CRG_SCCLKDIV14_sc_gt_clk_memrepair_fll_div_START  (15)
#define SOC_AO_CRG_SCCLKDIV14_sc_gt_clk_memrepair_fll_div_END    (15)
#define SOC_AO_CRG_SCCLKDIV14_bitmasken_START                    (16)
#define SOC_AO_CRG_SCCLKDIV14_bitmasken_END                      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV15_UNION
 struct description   : SCCLKDIV15 Register structure definition
                        Address Offset:0x294 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_mad_fll               : 5;  /* bit[0-4]  : mad fll的时钟分频比：
                                                                     5'd0: 1分频
                                                                     5'd1: 2分频
                                                                     ……
                                                                     5'd31:32分频 */
        unsigned int  sel_pll_ctrl_tp_ao        : 3;  /* bit[5-7]  : ao区域en/lock的test point输出选择：
                                                                     3'd0: ULPPLL_1 's en/lock
                                                                     3'd2: ULPPLL 's en/lock
                                                                     3'd3: spll 's en/lock
                                                                     3'd4: aupll 's en/lock
                                                                     others: ULPPLL_1 's en/lock */
        unsigned int  reserved_0                : 6;  /* bit[8-13] :  */
        unsigned int  sc_gt_clk_dp_audio_pll_ao : 1;  /* bit[14]   : dp_audio DIV前置门控（clk_dp_audio时钟挪入AOCRG)
                                                                     0：关；1：开（default） */
        unsigned int  reserved_1                : 1;  /* bit[15]   :  */
        unsigned int  bitmasken                 : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV15_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV15_div_mad_fll_START                (0)
#define SOC_AO_CRG_SCCLKDIV15_div_mad_fll_END                  (4)
#define SOC_AO_CRG_SCCLKDIV15_sel_pll_ctrl_tp_ao_START         (5)
#define SOC_AO_CRG_SCCLKDIV15_sel_pll_ctrl_tp_ao_END           (7)
#define SOC_AO_CRG_SCCLKDIV15_sc_gt_clk_dp_audio_pll_ao_START  (14)
#define SOC_AO_CRG_SCCLKDIV15_sc_gt_clk_dp_audio_pll_ao_END    (14)
#define SOC_AO_CRG_SCCLKDIV15_bitmasken_START                  (16)
#define SOC_AO_CRG_SCCLKDIV15_bitmasken_END                    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV16_UNION
 struct description   : SCCLKDIV16 Register structure definition
                        Address Offset:0x298 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved                         : 4;  /* bit[0-3]  :  */
        unsigned int  div_pcie_aux_32kpll              : 6;  /* bit[4-9]  : clk_pcie_aux_32kpll 时钟分频比：
                                                                            6'd0: 1分频
                                                                            6'd1: 2分频
                                                                            ……
                                                                            6'd63:64分频 */
        unsigned int  sc_gt_clk_pcie_aux_32kpll        : 1;  /* bit[10]   : clk_pcie_aux_32kpll DIV前置门控：
                                                                            1'b0:关
                                                                            1'b1:开（默认） */
        unsigned int  div_memrepair_peri_fll           : 4;  /* bit[11-14]: PERI BISR的fll通路分频比：
                                                                            4'd0: 1分频
                                                                            4'd1: 2分频
                                                                            ……
                                                                            4'd15:16分频 */
        unsigned int  sc_gt_clk_memrepair_peri_fll_div : 1;  /* bit[15]   : PERI BISR的fll通路分频前置门控使能：0关；1：开 */
        unsigned int  bitmasken                        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV16_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV16_div_pcie_aux_32kpll_START               (4)
#define SOC_AO_CRG_SCCLKDIV16_div_pcie_aux_32kpll_END                 (9)
#define SOC_AO_CRG_SCCLKDIV16_sc_gt_clk_pcie_aux_32kpll_START         (10)
#define SOC_AO_CRG_SCCLKDIV16_sc_gt_clk_pcie_aux_32kpll_END           (10)
#define SOC_AO_CRG_SCCLKDIV16_div_memrepair_peri_fll_START            (11)
#define SOC_AO_CRG_SCCLKDIV16_div_memrepair_peri_fll_END              (14)
#define SOC_AO_CRG_SCCLKDIV16_sc_gt_clk_memrepair_peri_fll_div_START  (15)
#define SOC_AO_CRG_SCCLKDIV16_sc_gt_clk_memrepair_peri_fll_div_END    (15)
#define SOC_AO_CRG_SCCLKDIV16_bitmasken_START                         (16)
#define SOC_AO_CRG_SCCLKDIV16_bitmasken_END                           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV17_UNION
 struct description   : SCCLKDIV17 Register structure definition
                        Address Offset:0x29C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0            : 4;  /* bit[0-3]  :  */
        unsigned int  reserved_1            : 1;  /* bit[4]    :  */
        unsigned int  reserved_2            : 1;  /* bit[5]    :  */
        unsigned int  reserved_3            : 2;  /* bit[6-7]  :  */
        unsigned int  div_qspi_sfc_x2       : 4;  /* bit[8-11] : qspi sfc通路分频比：
                                                                 4'h0:1分频；4'h1:2分频………4'hF:16分频 */
        unsigned int  sc_gt_clk_qspi_sfc_x2 : 1;  /* bit[12]   :  */
        unsigned int  reserved_4            : 1;  /* bit[13]   :  */
        unsigned int  reserved_5            : 1;  /* bit[14]   :  */
        unsigned int  reserved_6            : 1;  /* bit[15]   :  */
        unsigned int  bitmasken             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV17_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV17_div_qspi_sfc_x2_START        (8)
#define SOC_AO_CRG_SCCLKDIV17_div_qspi_sfc_x2_END          (11)
#define SOC_AO_CRG_SCCLKDIV17_sc_gt_clk_qspi_sfc_x2_START  (12)
#define SOC_AO_CRG_SCCLKDIV17_sc_gt_clk_qspi_sfc_x2_END    (12)
#define SOC_AO_CRG_SCCLKDIV17_bitmasken_START              (16)
#define SOC_AO_CRG_SCCLKDIV17_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV18_UNION
 struct description   : SCCLKDIV18 Register structure definition
                        Address Offset:0x2A0 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0               : 5;  /* bit[0-4]  :  */
        unsigned int  div_memrepair_peri       : 6;  /* bit[5-10] : peri memrepair的分频比：
                                                                    6'd0: 1分频
                                                                    6'd1: 2分频
                                                                    ……
                                                                    6'd31:32分频 */
        unsigned int  sc_gt_clk_memrepair_peri : 1;  /* bit[11]   : peri memrepair的分频前置门控：0：关；1：开 */
        unsigned int  reserved_1               : 4;  /* bit[12-15]:  */
        unsigned int  bitmasken                : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV18_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV18_div_memrepair_peri_START        (5)
#define SOC_AO_CRG_SCCLKDIV18_div_memrepair_peri_END          (10)
#define SOC_AO_CRG_SCCLKDIV18_sc_gt_clk_memrepair_peri_START  (11)
#define SOC_AO_CRG_SCCLKDIV18_sc_gt_clk_memrepair_peri_END    (11)
#define SOC_AO_CRG_SCCLKDIV18_bitmasken_START                 (16)
#define SOC_AO_CRG_SCCLKDIV18_bitmasken_END                   (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV19_UNION
 struct description   : SCCLKDIV19 Register structure definition
                        Address Offset:0x2A4 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_spi5        : 1;  /* bit[0]    :  */
        unsigned int  sc_gt_clk_spi5_ulppll : 1;  /* bit[1]    :  */
        unsigned int  div_spi5              : 6;  /* bit[2-7]  : spi5的时钟分频比：
                                                                 6'd0: 1分频
                                                                 6'd1: 2分频
                                                                 ……
                                                                 6'd31:32分频 */
        unsigned int  div_spi5_ulppll       : 4;  /* bit[8-11] :  */
        unsigned int  sel_spi5              : 1;  /* bit[12]   : 0:clk_splls时钟源；1：32KPLL时钟源 */
        unsigned int  reserved              : 3;  /* bit[13-15]:  */
        unsigned int  bitmasken             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV19_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV19_sc_gt_clk_spi5_START         (0)
#define SOC_AO_CRG_SCCLKDIV19_sc_gt_clk_spi5_END           (0)
#define SOC_AO_CRG_SCCLKDIV19_sc_gt_clk_spi5_ulppll_START  (1)
#define SOC_AO_CRG_SCCLKDIV19_sc_gt_clk_spi5_ulppll_END    (1)
#define SOC_AO_CRG_SCCLKDIV19_div_spi5_START               (2)
#define SOC_AO_CRG_SCCLKDIV19_div_spi5_END                 (7)
#define SOC_AO_CRG_SCCLKDIV19_div_spi5_ulppll_START        (8)
#define SOC_AO_CRG_SCCLKDIV19_div_spi5_ulppll_END          (11)
#define SOC_AO_CRG_SCCLKDIV19_sel_spi5_START               (12)
#define SOC_AO_CRG_SCCLKDIV19_sel_spi5_END                 (12)
#define SOC_AO_CRG_SCCLKDIV19_bitmasken_START              (16)
#define SOC_AO_CRG_SCCLKDIV19_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV20_UNION
 struct description   : SCCLKDIV20 Register structure definition
                        Address Offset:0x2A8 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_dp_audio_pll_ao   : 4;  /* bit[0-3]  : clk_dp_audio通路分频比：
                                                                 4'h0:1分频；4'h1:2分频………4'hF:16分频 */
        unsigned int  gt_clk_aupll_sscg     : 1;  /* bit[4]    : DP AUDIO专用PLL SSCG时钟门控；
                                                                 0：关，1：开 */
        unsigned int  div_blpwm2            : 2;  /* bit[5-6]  : BLPWM2时钟分频比：
                                                                 2'd0: 1分频
                                                                 …… 
                                                                 2'd3: 4分频 */
        unsigned int  div_spi3              : 6;  /* bit[7-12] : spi3的时钟分频比：
                                                                 6'd0: 1分频
                                                                 6'd1: 2分频
                                                                 ……
                                                                 6'd31:32分频 */
        unsigned int  sc_gt_clk_spi3_ulppll : 1;  /* bit[13]   :  */
        unsigned int  sc_gt_clk_spi3        : 1;  /* bit[14]   :  */
        unsigned int  sel_spi3              : 1;  /* bit[15]   : 0:clk_splls时钟源；1：32KPLL时钟源 */
        unsigned int  bitmasken             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV20_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV20_div_dp_audio_pll_ao_START    (0)
#define SOC_AO_CRG_SCCLKDIV20_div_dp_audio_pll_ao_END      (3)
#define SOC_AO_CRG_SCCLKDIV20_gt_clk_aupll_sscg_START      (4)
#define SOC_AO_CRG_SCCLKDIV20_gt_clk_aupll_sscg_END        (4)
#define SOC_AO_CRG_SCCLKDIV20_div_blpwm2_START             (5)
#define SOC_AO_CRG_SCCLKDIV20_div_blpwm2_END               (6)
#define SOC_AO_CRG_SCCLKDIV20_div_spi3_START               (7)
#define SOC_AO_CRG_SCCLKDIV20_div_spi3_END                 (12)
#define SOC_AO_CRG_SCCLKDIV20_sc_gt_clk_spi3_ulppll_START  (13)
#define SOC_AO_CRG_SCCLKDIV20_sc_gt_clk_spi3_ulppll_END    (13)
#define SOC_AO_CRG_SCCLKDIV20_sc_gt_clk_spi3_START         (14)
#define SOC_AO_CRG_SCCLKDIV20_sc_gt_clk_spi3_END           (14)
#define SOC_AO_CRG_SCCLKDIV20_sel_spi3_START               (15)
#define SOC_AO_CRG_SCCLKDIV20_sel_spi3_END                 (15)
#define SOC_AO_CRG_SCCLKDIV20_bitmasken_START              (16)
#define SOC_AO_CRG_SCCLKDIV20_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV21_UNION
 struct description   : SCCLKDIV21 Register structure definition
                        Address Offset:0x2AC Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_spi3_ulppll      : 4;  /* bit[0-3]  :  */
        unsigned int  div_asp_codec_ulppll : 6;  /* bit[4-9]  : clk_asp_codec的ulppll1通路的分频比：
                                                                6'd0: 1分频
                                                                6'd1: 2分频
                                                                ……
                                                                6'd3F:64分频 */
        unsigned int  reserved             : 5;  /* bit[10-14]:  */
        unsigned int  sel_asp_codec        : 1;  /* bit[15]   : 0:clk_spll时钟源；1：ULPPLL时钟源 */
        unsigned int  bitmasken            : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV21_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV21_div_spi3_ulppll_START       (0)
#define SOC_AO_CRG_SCCLKDIV21_div_spi3_ulppll_END         (3)
#define SOC_AO_CRG_SCCLKDIV21_div_asp_codec_ulppll_START  (4)
#define SOC_AO_CRG_SCCLKDIV21_div_asp_codec_ulppll_END    (9)
#define SOC_AO_CRG_SCCLKDIV21_sel_asp_codec_START         (15)
#define SOC_AO_CRG_SCCLKDIV21_sel_asp_codec_END           (15)
#define SOC_AO_CRG_SCCLKDIV21_bitmasken_START             (16)
#define SOC_AO_CRG_SCCLKDIV21_bitmasken_END               (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV22_UNION
 struct description   : SCCLKDIV22 Register structure definition
                        Address Offset:0x2B0 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddr_loopback_en2 : 1;  /* bit[0]    : DDR 复位IO相关 E/IE控制 */
        unsigned int  ddr_reset_en2    : 1;  /* bit[1]    : DDR 复位IO相关 E/IE控制 */
        unsigned int  ddr_loopback_en  : 1;  /* bit[2]    : DDR 复位IO相关 E/IE控制 */
        unsigned int  ddr_reset_en     : 1;  /* bit[3]    : DDR 复位IO相关 E/IE控制 */
        unsigned int  reserved_0       : 6;  /* bit[4-9]  :  */
        unsigned int  reserved_1       : 5;  /* bit[10-14]:  */
        unsigned int  reserved_2       : 1;  /* bit[15]   :  */
        unsigned int  bitmasken        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV22_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV22_ddr_loopback_en2_START  (0)
#define SOC_AO_CRG_SCCLKDIV22_ddr_loopback_en2_END    (0)
#define SOC_AO_CRG_SCCLKDIV22_ddr_reset_en2_START     (1)
#define SOC_AO_CRG_SCCLKDIV22_ddr_reset_en2_END       (1)
#define SOC_AO_CRG_SCCLKDIV22_ddr_loopback_en_START   (2)
#define SOC_AO_CRG_SCCLKDIV22_ddr_loopback_en_END     (2)
#define SOC_AO_CRG_SCCLKDIV22_ddr_reset_en_START      (3)
#define SOC_AO_CRG_SCCLKDIV22_ddr_reset_en_END        (3)
#define SOC_AO_CRG_SCCLKDIV22_bitmasken_START         (16)
#define SOC_AO_CRG_SCCLKDIV22_bitmasken_END           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV23_UNION
 struct description   : SCCLKDIV23 Register structure definition
                        Address Offset:0x2B8 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_isp_cam_gt   : 1;  /* bit[0]    : isp_camera_clk的前门控 */
        unsigned int  sc_gt_clk_isp_cam_0_gt : 1;  /* bit[1]    :  */
        unsigned int  sc_gt_clk_isp_cam_2_gt : 1;  /* bit[2]    :  */
        unsigned int  sc_gt_clk_isp_cam_3_gt : 1;  /* bit[3]    : ISP_CAMERA_CLK0/2/3的前门控 */
        unsigned int  sel_clk_isp_cam_0      : 1;  /* bit[4]    : 0:选择clkin_sys
                                                                  1:选择fnpll分支 */
        unsigned int  sel_clk_isp_cam_2      : 1;  /* bit[5]    : 0:选择clkin_sys
                                                                  1:选择fnpll分支 */
        unsigned int  sel_clk_isp_cam_3      : 1;  /* bit[6]    : 0:选择clkin_sys
                                                                  1:选择fnpll分支 */
        unsigned int  sel_clk_isp_cam_0_pre1 : 1;  /* bit[7]    : 0:选择clkin_sys_div
                                                                  1:pre0分支 */
        unsigned int  sel_clk_isp_cam_2_pre1 : 1;  /* bit[8]    : 0:选择clkin_sys_div
                                                                  1:pre0分支 */
        unsigned int  sel_clk_isp_cam_3_pre1 : 1;  /* bit[9]    : 0:选择clkin_sys_div
                                                                  1:pre0分支 */
        unsigned int  reserved               : 6;  /* bit[10-15]:  */
        unsigned int  bitmasken              : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV23_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_gt_START    (0)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_gt_END      (0)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_0_gt_START  (1)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_0_gt_END    (1)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_2_gt_START  (2)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_2_gt_END    (2)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_3_gt_START  (3)
#define SOC_AO_CRG_SCCLKDIV23_sc_gt_clk_isp_cam_3_gt_END    (3)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_0_START       (4)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_0_END         (4)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_2_START       (5)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_2_END         (5)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_3_START       (6)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_3_END         (6)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_0_pre1_START  (7)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_0_pre1_END    (7)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_2_pre1_START  (8)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_2_pre1_END    (8)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_3_pre1_START  (9)
#define SOC_AO_CRG_SCCLKDIV23_sel_clk_isp_cam_3_pre1_END    (9)
#define SOC_AO_CRG_SCCLKDIV23_bitmasken_START               (16)
#define SOC_AO_CRG_SCCLKDIV23_bitmasken_END                 (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV24_UNION
 struct description   : SCCLKDIV24 Register structure definition
                        Address Offset:0x2BC Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_clk_isp_cam_div   : 4;  /* bit[0-3]  : isp_camera_clk的前div,默认八分频 */
        unsigned int  div_clk_isp_cam_0_div : 4;  /* bit[4-7]  :  */
        unsigned int  div_clk_isp_cam_2_div : 4;  /* bit[8-11] :  */
        unsigned int  div_clk_isp_cam_3_div : 4;  /* bit[12-15]: ISP_CAMERA_CLK0/2/3的后div，默认八分频 */
        unsigned int  bitmasken             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV24_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_div_START    (0)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_div_END      (3)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_0_div_START  (4)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_0_div_END    (7)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_2_div_START  (8)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_2_div_END    (11)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_3_div_START  (12)
#define SOC_AO_CRG_SCCLKDIV24_div_clk_isp_cam_3_div_END    (15)
#define SOC_AO_CRG_SCCLKDIV24_bitmasken_START              (16)
#define SOC_AO_CRG_SCCLKDIV24_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_UNION
 struct description   : SC_PLL_FSM_NS_VOTE0 Register structure definition
                        Address Offset:0x2B4 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aupll_en_fsm_vote : 5;  /* bit[0-4]  : 投票位仅在pll_fsm使能之后有效 */
        unsigned int  reserved_0        : 8;  /* bit[5-12] :  */
        unsigned int  reserved_1        : 3;  /* bit[13-15]:  */
        unsigned int  bitmasken         : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_UNION;
#endif
#define SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_aupll_en_fsm_vote_START  (0)
#define SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_aupll_en_fsm_vote_END    (4)
#define SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_bitmasken_START          (16)
#define SOC_AO_CRG_SC_PLL_FSM_NS_VOTE0_bitmasken_END            (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_INTR_NOCBUS_STAT_UNION
 struct description   : SC_INTR_NOCBUS_STAT Register structure definition
                        Address Offset:0x354 Initial:0x0000 Width:32
 register description : 外设状态寄存器18
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_stat_asp_nonidle_pend       : 1;  /* bit[0]    :  */
        unsigned int  intr_stat_iomcu_nonidle_pend     : 1;  /* bit[1]    :  */
        unsigned int  intr_stat_hisee_nonidle_pend     : 1;  /* bit[2]    :  */
        unsigned int  intr_stat_hieps_nonidle_pend     : 1;  /* bit[3]    :  */
        unsigned int  intr_stat_hieps_ree_nonidle_pend : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                       : 2;  /* bit[5-6]  :  */
        unsigned int  reserved_1                       : 1;  /* bit[7]    :  */
        unsigned int  reserved_2                       : 1;  /* bit[8]    :  */
        unsigned int  reserved_3                       : 3;  /* bit[9-11] :  */
        unsigned int  reserved_4                       : 1;  /* bit[12]   :  */
        unsigned int  reserved_5                       : 2;  /* bit[13-14]:  */
        unsigned int  reserved_6                       : 1;  /* bit[15]   :  */
        unsigned int  reserved_7                       : 1;  /* bit[16]   :  */
        unsigned int  reserved_8                       : 15; /* bit[17-31]:  */
    } reg;
} SOC_AO_CRG_SC_INTR_NOCBUS_STAT_UNION;
#endif
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_asp_nonidle_pend_START        (0)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_asp_nonidle_pend_END          (0)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_iomcu_nonidle_pend_START      (1)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_iomcu_nonidle_pend_END        (1)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_hisee_nonidle_pend_START      (2)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_hisee_nonidle_pend_END        (2)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_hieps_nonidle_pend_START      (3)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_hieps_nonidle_pend_END        (3)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_hieps_ree_nonidle_pend_START  (4)
#define SOC_AO_CRG_SC_INTR_NOCBUS_STAT_intr_stat_hieps_ree_nonidle_pend_END    (4)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTATUS17_UNION
 struct description   : SCPERSTATUS17 Register structure definition
                        Address Offset:0x358 Initial:0x0000 Width:32
 register description : 外设状态寄存器17
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0     : 1;  /* bit[0]    :  */
        unsigned int  reserved_1     : 1;  /* bit[1]    :  */
        unsigned int  reserved_2     : 2;  /* bit[2-3]  :  */
        unsigned int  reserved_3     : 1;  /* bit[4]    :  */
        unsigned int  reserved_4     : 2;  /* bit[5-6]  :  */
        unsigned int  reserved_5     : 1;  /* bit[7]    :  */
        unsigned int  reserved_6     : 1;  /* bit[8]    :  */
        unsigned int  spll_fsm_wsat  : 1;  /* bit[9]    : spll fsm状态机指示信号(需要软件回读至少2次为1,才能判断为正在工作）：
                                                          1'b0 : PLL处于非工作状态(bypass)
                                                          1'b1 : PLL正常工作，输出指定频率 */
        unsigned int  aupll_fsm_wsat : 1;  /* bit[10]   : aupll fsm状态机指示信号(需要软件回读至少2次为1,才能判断为正在工作）：
                                                          1'b0 : PLL处于非工作状态(bypass)
                                                          1'b1 : PLL正常工作，输出指定频率 */
        unsigned int  reserved_7     : 1;  /* bit[11]   :  */
        unsigned int  reserved_8     : 1;  /* bit[12]   :  */
        unsigned int  reserved_9     : 1;  /* bit[13]   :  */
        unsigned int  reserved_10    : 1;  /* bit[14]   :  */
        unsigned int  reserved_11    : 1;  /* bit[15]   :  */
        unsigned int  reserved_12    : 1;  /* bit[16]   :  */
        unsigned int  reserved_13    : 15; /* bit[17-31]:  */
    } reg;
} SOC_AO_CRG_SCPERSTATUS17_UNION;
#endif
#define SOC_AO_CRG_SCPERSTATUS17_spll_fsm_wsat_START   (9)
#define SOC_AO_CRG_SCPERSTATUS17_spll_fsm_wsat_END     (9)
#define SOC_AO_CRG_SCPERSTATUS17_aupll_fsm_wsat_START  (10)
#define SOC_AO_CRG_SCPERSTATUS17_aupll_fsm_wsat_END    (10)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTATUS16_UNION
 struct description   : SCPERSTATUS16 Register structure definition
                        Address Offset:0x35C Initial:0x1 Width:32
 register description : 外设状态寄存器16
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sw_ack_clk_memrepair_sw         : 3;  /* bit[0-2]  : bisr的时钟源选择状态：
                                                                           3'b001:pll通路
                                                                           3'b010:clk_sys通路
                                                                           3'b100:fll通路 */
        unsigned int  sw_ack_clk_aobus_ini            : 2;  /* bit[3-4]  : aobus_ini时钟切换指示信号： 
                                                                           01：同aobus_sw;
                                                                           10：fll div ; */
        unsigned int  swdone_clk_asp_codec_pll_div    : 1;  /* bit[5]    :  */
        unsigned int  swdone_clk_ufs_mphy_sys_div     : 1;  /* bit[6]    :  */
        unsigned int  swdone_clk_asp_codec_ulppll_div : 1;  /* bit[7]    :  */
        unsigned int  swdone_clk_qspi_sfc_x2_div      : 1;  /* bit[8]    :  */
        unsigned int  reserved_0                      : 3;  /* bit[9-11] :  */
        unsigned int  swdone_clk_spmi_mst             : 1;  /* bit[12]   :  */
        unsigned int  sw_ack_clk_aonoc_sw             : 2;  /* bit[13-14]: aonoc当前时钟源选择指示。
                                                                           2'b01:clk_ddrsys_ao_div
                                                                           2'b10 :clk_aonoc_fll_ini */
        unsigned int  swdone_clk_ao_camera_div        : 1;  /* bit[15]   :  */
        unsigned int  swdone_clk_ao_camera1_div       : 1;  /* bit[16]   :  */
        unsigned int  st_pclk_spmi                    : 1;  /* bit[17]   :  */
        unsigned int  sw_ack_clk_hsdt_subsys_sw       : 3;  /* bit[18-20]: clk_hsdt_subsys的时钟源选择状态：
                                                                           3'b001:32K/ulppll
                                                                           3'b010:19.2M
                                                                           3'b100:spll */
        unsigned int  swdone_clk_hsdt_subsys_div      : 1;  /* bit[21]   :  */
        unsigned int  reserved_1                      : 1;  /* bit[22]   : swdone_clk_ao_tof_div */
        unsigned int  sw_ack_clk_asp_codec_sw         : 2;  /* bit[23-24]: asp codec时钟切换指示信号： 
                                                                           2'b01：SPLL时钟源;
                                                                           2'b10：ULPPLL时钟源 ; */
        unsigned int  sw_ack_clk_memrepair_peri_sw    : 3;  /* bit[25-27]: peri bisr的时钟源选择状态：
                                                                           3'b001:pll通路
                                                                           3'b010:clk_sys通路
                                                                           3'b100:clk_fll通路 */
        unsigned int  reserved_2                      : 1;  /* bit[28]   : xian删除 */
        unsigned int  swdone_clk_hsdt_subsys_fll_div  : 1;  /* bit[29]   :  */
        unsigned int  sw_ack_clk_hsdt_subsys_fll_sw   : 2;  /* bit[30-31]: HSDT_SUBSYS ULPPLL通路时钟切换指示信号： 
                                                                           2'b01：32K;
                                                                           2'b10：ULPPLL ; */
    } reg;
} SOC_AO_CRG_SCPERSTATUS16_UNION;
#endif
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_memrepair_sw_START          (0)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_memrepair_sw_END            (2)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_aobus_ini_START             (3)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_aobus_ini_END               (4)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_asp_codec_pll_div_START     (5)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_asp_codec_pll_div_END       (5)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_ufs_mphy_sys_div_START      (6)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_ufs_mphy_sys_div_END        (6)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_asp_codec_ulppll_div_START  (7)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_asp_codec_ulppll_div_END    (7)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_qspi_sfc_x2_div_START       (8)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_qspi_sfc_x2_div_END         (8)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_spmi_mst_START              (12)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_spmi_mst_END                (12)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_aonoc_sw_START              (13)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_aonoc_sw_END                (14)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_ao_camera_div_START         (15)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_ao_camera_div_END           (15)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_ao_camera1_div_START        (16)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_ao_camera1_div_END          (16)
#define SOC_AO_CRG_SCPERSTATUS16_st_pclk_spmi_START                     (17)
#define SOC_AO_CRG_SCPERSTATUS16_st_pclk_spmi_END                       (17)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_hsdt_subsys_sw_START        (18)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_hsdt_subsys_sw_END          (20)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_hsdt_subsys_div_START       (21)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_hsdt_subsys_div_END         (21)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_asp_codec_sw_START          (23)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_asp_codec_sw_END            (24)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_memrepair_peri_sw_START     (25)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_memrepair_peri_sw_END       (27)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_hsdt_subsys_fll_div_START   (29)
#define SOC_AO_CRG_SCPERSTATUS16_swdone_clk_hsdt_subsys_fll_div_END     (29)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_hsdt_subsys_fll_sw_START    (30)
#define SOC_AO_CRG_SCPERSTATUS16_sw_ack_clk_hsdt_subsys_fll_sw_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERSTATUS18_UNION
 struct description   : SCPERSTATUS18 Register structure definition
                        Address Offset:0x360 Initial:0x1 Width:32
 register description : 外设状态寄存器18
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                 : 3;  /* bit[0-2]  :  */
        unsigned int  reserved_1                 : 2;  /* bit[3-4]  :  */
        unsigned int  reserved_2                 : 1;  /* bit[5]    :  */
        unsigned int  swdone_clk_sys_ulppll_div  : 1;  /* bit[6]    :  */
        unsigned int  reserved_3                 : 1;  /* bit[7]    :  */
        unsigned int  reserved_4                 : 1;  /* bit[8]    :  */
        unsigned int  reserved_5                 : 3;  /* bit[9-11] :  */
        unsigned int  reserved_6                 : 1;  /* bit[12]   :  */
        unsigned int  reserved_7                 : 2;  /* bit[13-14]:  */
        unsigned int  swdone_clk_isp_cam_3_div   : 1;  /* bit[15]   :  */
        unsigned int  swdone_clk_isp_cam_2_div   : 1;  /* bit[16]   :  */
        unsigned int  swdone_clk_isp_cam_0_div   : 1;  /* bit[17]   :  */
        unsigned int  reserved_8                 : 3;  /* bit[18-20]:  */
        unsigned int  reserved_9                 : 1;  /* bit[21]   :  */
        unsigned int  reserved_10                : 1;  /* bit[22]   :  */
        unsigned int  reserved_11                : 3;  /* bit[23-25]:  */
        unsigned int  sw_ack_clk_sys_ulppll_sw   : 2;  /* bit[26-27]: CLK_SYS_ULPPLL通路时钟切换指示信号： 
                                                                      2'b01：19.2M;
                                                                      2'b10：ULPPLL分频  */
        unsigned int  sw_ack_clk_sys_pre         : 2;  /* bit[28-29]: CLK_SYS_PRE通路时钟切换指示信号： 
                                                                      2'b01：19.2M;
                                                                      2'b10：ULPPLL分频  */
        unsigned int  sw_ack_clk_ufs_mphy_sys_sw : 2;  /* bit[30-31]: UFS_MPHY_SYS通路时钟切换指示信号： 
                                                                      2'b01：38.4M;
                                                                      2'b10：PLL分频 ; */
    } reg;
} SOC_AO_CRG_SCPERSTATUS18_UNION;
#endif
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_sys_ulppll_div_START   (6)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_sys_ulppll_div_END     (6)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_isp_cam_3_div_START    (15)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_isp_cam_3_div_END      (15)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_isp_cam_2_div_START    (16)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_isp_cam_2_div_END      (16)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_isp_cam_0_div_START    (17)
#define SOC_AO_CRG_SCPERSTATUS18_swdone_clk_isp_cam_0_div_END      (17)
#define SOC_AO_CRG_SCPERSTATUS18_sw_ack_clk_sys_ulppll_sw_START    (26)
#define SOC_AO_CRG_SCPERSTATUS18_sw_ack_clk_sys_ulppll_sw_END      (27)
#define SOC_AO_CRG_SCPERSTATUS18_sw_ack_clk_sys_pre_START          (28)
#define SOC_AO_CRG_SCPERSTATUS18_sw_ack_clk_sys_pre_END            (29)
#define SOC_AO_CRG_SCPERSTATUS18_sw_ack_clk_ufs_mphy_sys_sw_START  (30)
#define SOC_AO_CRG_SCPERSTATUS18_sw_ack_clk_ufs_mphy_sys_sw_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCAUPLLCTRL0_UNION
 struct description   : SCAUPLLCTRL0 Register structure definition
                        Address Offset:0x490 Initial:0x00 Width:32
 register description : SPLL控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aupll_en       : 1;  /* bit[0-0]  : AUPLL软件使能控制（最终的使能由该配置信号、IOMCU侧控制信号IOMCU_pll_ctrl、asp_AUPLL_en_req 做或）
                                                          0：去使能；
                                                          1：使能。 */
        unsigned int  aupll_bp       : 1;  /* bit[1-1]  : AUPLL Bypass控制。
                                                          0：正常工作；
                                                          1：Bypass。 */
        unsigned int  aupll_refdiv   : 6;  /* bit[2-7]  : AUPLL输入时钟分频器。 */
        unsigned int  aupll_fbdiv    : 12; /* bit[8-19] : AUPLL反馈时钟整数分频器。
                                                          。 */
        unsigned int  aupll_postdiv1 : 3;  /* bit[20-22]: AUPLL输出时钟分频器1。 */
        unsigned int  aupll_postdiv2 : 3;  /* bit[23-25]: AUPLL输出时钟分频器2。 */
        unsigned int  reserved       : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCAUPLLCTRL0_UNION;
#endif
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_en_START        (0)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_en_END          (0)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_bp_START        (1)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_bp_END          (1)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_refdiv_START    (2)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_refdiv_END      (7)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_fbdiv_START     (8)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_fbdiv_END       (19)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_postdiv1_START  (20)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_postdiv1_END    (22)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_postdiv2_START  (23)
#define SOC_AO_CRG_SCAUPLLCTRL0_aupll_postdiv2_END    (25)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCAUPLLCTRL1_UNION
 struct description   : SCAUPLLCTRL1 Register structure definition
                        Address Offset:0x494 Initial:0x0 Width:32
 register description : SPLL控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aupll_fracdiv : 24; /* bit[0-23] : AUPLL反馈时钟分数分频器。分频比=配置值+1。 */
        unsigned int  aupll_int_mod : 1;  /* bit[24-24]: AUPLL整数分频模式。
                                                         0：分数分频模式；
                                                         1：整数分频模式。
                                                         注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  aupll_cfg_vld : 1;  /* bit[25-25]: AUPLL配置有效标志。
                                                         0：配置无效；
                                                         1：配置有效。 */
        unsigned int  gt_clk_aupll  : 1;  /* bit[26-26]: AUPLL门控信号。
                                                         0：AUPLL输出时钟门控；
                                                         1：AUPLL输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  reserved_0    : 1;  /* bit[27-27]: 保留。 */
        unsigned int  reserved_1    : 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCAUPLLCTRL1_UNION;
#endif
#define SOC_AO_CRG_SCAUPLLCTRL1_aupll_fracdiv_START  (0)
#define SOC_AO_CRG_SCAUPLLCTRL1_aupll_fracdiv_END    (23)
#define SOC_AO_CRG_SCAUPLLCTRL1_aupll_int_mod_START  (24)
#define SOC_AO_CRG_SCAUPLLCTRL1_aupll_int_mod_END    (24)
#define SOC_AO_CRG_SCAUPLLCTRL1_aupll_cfg_vld_START  (25)
#define SOC_AO_CRG_SCAUPLLCTRL1_aupll_cfg_vld_END    (25)
#define SOC_AO_CRG_SCAUPLLCTRL1_gt_clk_aupll_START   (26)
#define SOC_AO_CRG_SCAUPLLCTRL1_gt_clk_aupll_END     (26)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCAUPLLSSCCTRL_UNION
 struct description   : SCAUPLLSSCCTRL Register structure definition
                        Address Offset:0x498 Initial:0x0000 Width:32
 register description : SPLL SSC控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sscg_enable   : 1;  /* bit[0]    : SSCG模块使能，高有效。
                                                         0：关闭；
                                                         1：正常工作。 */
        unsigned int  sscg_mode     : 1;  /* bit[1]    : SSCG模块展频模式，静态配置。
                                                         0：中心展频；
                                                         1：向下展频。 */
        unsigned int  sscg_rate     : 4;  /* bit[2-5]  : SSCG模块展频速度；展频工作时钟的频率，静态配置。
                                                         freq=(CLKSSCG / (RATE + 1 ))。 */
        unsigned int  sscg_depth    : 3;  /* bit[6-8]  : SSCG模块调制深度(%)，静态配置。
                                                         3'b000 = 0.049%；
                                                         3'b001 = 0.098%；
                                                         3'b010 = 0.195%；
                                                         3'b011 = 0.391%；
                                                         3'b100 = 0.781%；
                                                         3'b101 = 1.563%；
                                                         3'b110 = 3.125%；
                                                         3'b111 = 6.250%。 */
        unsigned int  sscg_fast_dis : 1;  /* bit[9]    : SSCG模块快速退出功能使能，静态配置。
                                                         0：不使能快速退出（推荐）
                                                         1：使能快速退出 */
        unsigned int  reserved      : 6;  /* bit[10-15]:  */
        unsigned int  bitmasken     : 16; /* bit[16-31]: 只有当bitmasken对应的比特位为1'b1，相应的比特位才起作用。bitmasken[0]就是bit[0]的mask使能位。写1有效。 */
    } reg;
} SOC_AO_CRG_SCAUPLLSSCCTRL_UNION;
#endif
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_enable_START    (0)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_enable_END      (0)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_mode_START      (1)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_mode_END        (1)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_rate_START      (2)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_rate_END        (5)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_depth_START     (6)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_depth_END       (8)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_fast_dis_START  (9)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_sscg_fast_dis_END    (9)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_bitmasken_START      (16)
#define SOC_AO_CRG_SCAUPLLSSCCTRL_bitmasken_END        (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_AUFNPLL_CFG0_UNION
 struct description   : AUFNPLL_CFG0 Register structure definition
                        Address Offset:0x49C Initial:0x0F Width:32
 register description : fnpll(dp audio 专用)配置寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sta_reg_sel      : 1;  /* bit[0]    : 寄存器回读选择信号，选择当前读的是哪组状态寄存器：对应寄存器详细描述见“寄存器复用详细说明”页
                                                            0：选择读寄存器SCPPLLSTATE0_0【默认】
                                                            1：选择读寄存器SCPPLLSTATE0_1 */
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
                                                            0：不清零unlock_flag以及历史失锁统计次数；（默认）
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
                                                            00-电源电压1.8V；
                                                            01-电源电压1.2V；
                                                            10-reserved；
                                                            11-reserved； */
        unsigned int  icp_ctrl         : 2;  /* bit[14-15]: fnpll的ICP偏置电流选择信号
                                                            3：reserved
                                                            2：reserved
                                                            1：选择1/4 CP偏置电流【默认】
                                                            0：选择1/2 CP偏置电流 */
        unsigned int  updn_sel         : 1;  /* bit[16]   : fnpll的CP路数选择信号
                                                            1：打开1路CP
                                                            0：打开2路CP【默认】
                                                            当Fpfd<20MHz时，需配置为1 */
        unsigned int  reserved         : 2;  /* bit[17-18]: 保留 */
        unsigned int  int_mask         : 3;  /* bit[19-21]: 中断屏蔽位，为1时中断被屏蔽。中断通过或逻辑连到pll_int。
                                                            xx1：屏蔽unlock_flag；
                                                            x1x：屏蔽bbpd_calib_fail；
                                                            1xx：屏蔽dtc_calib_satu。 */
        unsigned int  pfd_div_ratio    : 4;  /* bit[22-25]: dc_ac模块内pfd分频比设置,默认不分频。建议不要配置。
                                                            4'b0000: 1分频（固定默认值）
                                                            4'b0001: 1分频
                                                            4'b0010: 2分频
                                                            ….
                                                            4'b1111: 15分频 */
        unsigned int  freq_threshold   : 6;  /* bit[26-31]: 判断振荡器输出时钟频率的阈值，推荐配置大于等于4（0/1禁配）,freq_threshold=pfd_div_ratio*1.2GHz/[4*freq(pfd_clk)]-1；
                                                            (当计算值小于4时取4，并且pfd_div_ratio固定为1分频）,例如pfd_clk频率默认为38.4MHz，计算得freq_threshold配置值为7。
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
} SOC_AO_CRG_AUFNPLL_CFG0_UNION;
#endif
#define SOC_AO_CRG_AUFNPLL_CFG0_sta_reg_sel_START       (0)
#define SOC_AO_CRG_AUFNPLL_CFG0_sta_reg_sel_END         (0)
#define SOC_AO_CRG_AUFNPLL_CFG0_read_en_START           (1)
#define SOC_AO_CRG_AUFNPLL_CFG0_read_en_END             (1)
#define SOC_AO_CRG_AUFNPLL_CFG0_input_cfg_en_START      (2)
#define SOC_AO_CRG_AUFNPLL_CFG0_input_cfg_en_END        (2)
#define SOC_AO_CRG_AUFNPLL_CFG0_test_mode_START         (3)
#define SOC_AO_CRG_AUFNPLL_CFG0_test_mode_END           (3)
#define SOC_AO_CRG_AUFNPLL_CFG0_unlock_clear_START      (4)
#define SOC_AO_CRG_AUFNPLL_CFG0_unlock_clear_END        (4)
#define SOC_AO_CRG_AUFNPLL_CFG0_dll_en_START            (5)
#define SOC_AO_CRG_AUFNPLL_CFG0_dll_en_END              (5)
#define SOC_AO_CRG_AUFNPLL_CFG0_pll_lock_ate_sel_START  (6)
#define SOC_AO_CRG_AUFNPLL_CFG0_pll_lock_ate_sel_END    (7)
#define SOC_AO_CRG_AUFNPLL_CFG0_test_data_sel_START     (8)
#define SOC_AO_CRG_AUFNPLL_CFG0_test_data_sel_END       (11)
#define SOC_AO_CRG_AUFNPLL_CFG0_bias_ctrl_START         (12)
#define SOC_AO_CRG_AUFNPLL_CFG0_bias_ctrl_END           (13)
#define SOC_AO_CRG_AUFNPLL_CFG0_icp_ctrl_START          (14)
#define SOC_AO_CRG_AUFNPLL_CFG0_icp_ctrl_END            (15)
#define SOC_AO_CRG_AUFNPLL_CFG0_updn_sel_START          (16)
#define SOC_AO_CRG_AUFNPLL_CFG0_updn_sel_END            (16)
#define SOC_AO_CRG_AUFNPLL_CFG0_int_mask_START          (19)
#define SOC_AO_CRG_AUFNPLL_CFG0_int_mask_END            (21)
#define SOC_AO_CRG_AUFNPLL_CFG0_pfd_div_ratio_START     (22)
#define SOC_AO_CRG_AUFNPLL_CFG0_pfd_div_ratio_END       (25)
#define SOC_AO_CRG_AUFNPLL_CFG0_freq_threshold_START    (26)
#define SOC_AO_CRG_AUFNPLL_CFG0_freq_threshold_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_AUFNPLL_CFG1_UNION
 struct description   : AUFNPLL_CFG1 Register structure definition
                        Address Offset:0x4A0 Initial:0x0000 Width:32
 register description : fnpll(dp audio 专用)配置寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
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
} SOC_AO_CRG_AUFNPLL_CFG1_UNION;
#endif
#define SOC_AO_CRG_AUFNPLL_CFG1_ctinue_lock_num_START  (0)
#define SOC_AO_CRG_AUFNPLL_CFG1_ctinue_lock_num_END    (5)
#define SOC_AO_CRG_AUFNPLL_CFG1_lock_thr_START         (6)
#define SOC_AO_CRG_AUFNPLL_CFG1_lock_thr_END           (7)
#define SOC_AO_CRG_AUFNPLL_CFG1_dc_ac_clk_en_START     (8)
#define SOC_AO_CRG_AUFNPLL_CFG1_dc_ac_clk_en_END       (8)
#define SOC_AO_CRG_AUFNPLL_CFG1_dtc_ctrl_inv_START     (9)
#define SOC_AO_CRG_AUFNPLL_CFG1_dtc_ctrl_inv_END       (9)
#define SOC_AO_CRG_AUFNPLL_CFG1_ana_cfg_fnpll_START    (16)
#define SOC_AO_CRG_AUFNPLL_CFG1_ana_cfg_fnpll_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_AUFNPLL_CFG2_UNION
 struct description   : AUFNPLL_CFG2 Register structure definition
                        Address Offset:0x4A4 Initial:0x20 Width:32
 register description : fnpll(dp audio 专用)配置寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
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
} SOC_AO_CRG_AUFNPLL_CFG2_UNION;
#endif
#define SOC_AO_CRG_AUFNPLL_CFG2_div2_pd_START         (0)
#define SOC_AO_CRG_AUFNPLL_CFG2_div2_pd_END           (0)
#define SOC_AO_CRG_AUFNPLL_CFG2_fbdiv_rst_n_START     (1)
#define SOC_AO_CRG_AUFNPLL_CFG2_fbdiv_rst_n_END       (1)
#define SOC_AO_CRG_AUFNPLL_CFG2_refdiv_rst_n_START    (2)
#define SOC_AO_CRG_AUFNPLL_CFG2_refdiv_rst_n_END      (2)
#define SOC_AO_CRG_AUFNPLL_CFG2_dll_force_en_START    (3)
#define SOC_AO_CRG_AUFNPLL_CFG2_dll_force_en_END      (3)
#define SOC_AO_CRG_AUFNPLL_CFG2_phe_code_a_START      (4)
#define SOC_AO_CRG_AUFNPLL_CFG2_phe_code_a_END        (5)
#define SOC_AO_CRG_AUFNPLL_CFG2_phe_code_b_START      (6)
#define SOC_AO_CRG_AUFNPLL_CFG2_phe_code_b_END        (7)
#define SOC_AO_CRG_AUFNPLL_CFG2_bbpd_calib_byp_START  (8)
#define SOC_AO_CRG_AUFNPLL_CFG2_bbpd_calib_byp_END    (8)
#define SOC_AO_CRG_AUFNPLL_CFG2_k_gain_cfg_en_START   (9)
#define SOC_AO_CRG_AUFNPLL_CFG2_k_gain_cfg_en_END     (9)
#define SOC_AO_CRG_AUFNPLL_CFG2_k_gain_cfg_START      (10)
#define SOC_AO_CRG_AUFNPLL_CFG2_k_gain_cfg_END        (15)
#define SOC_AO_CRG_AUFNPLL_CFG2_k_gain_av_thr_START   (16)
#define SOC_AO_CRG_AUFNPLL_CFG2_k_gain_av_thr_END     (18)
#define SOC_AO_CRG_AUFNPLL_CFG2_dtc_test_START        (19)
#define SOC_AO_CRG_AUFNPLL_CFG2_dtc_test_END          (19)
#define SOC_AO_CRG_AUFNPLL_CFG2_dtc_m_cfg_START       (20)
#define SOC_AO_CRG_AUFNPLL_CFG2_dtc_m_cfg_END         (25)
#define SOC_AO_CRG_AUFNPLL_CFG2_dtc_o_cfg_START       (26)
#define SOC_AO_CRG_AUFNPLL_CFG2_dtc_o_cfg_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_AUFNPLL_CFG3_UNION
 struct description   : AUFNPLL_CFG3 Register structure definition
                        Address Offset:0x4A8 Initial:0x0028627D Width:32
 register description : fnpll(dp audio 专用)配置寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  scppll_cfg3 : 32; /* bit[0-31]: 描述后续待刷新 */
    } reg;
} SOC_AO_CRG_AUFNPLL_CFG3_UNION;
#endif
#define SOC_AO_CRG_AUFNPLL_CFG3_scppll_cfg3_START  (0)
#define SOC_AO_CRG_AUFNPLL_CFG3_scppll_cfg3_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_AUFNPLL_STATE0_UNION
 struct description   : AUFNPLL_STATE0 Register structure definition
                        Address Offset:0x4B4 Initial:0x00 Width:32
 register description : fnpll(dp audio 专用)状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_AO_CRG_AUFNPLL_STATE0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_AO_AUTODIV2_UNION
 struct description   : AO_AUTODIV2 Register structure definition
                        Address Offset:0x500 Initial:0x0 Width:32
 register description : 自动降频控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_div_auto_reduce_bypass : 1;  /* bit[0]    : 硬件自动降频功能bypass配置：
                                                                       0：不bypass；
                                                                       1：bypass。 */
        unsigned int  pcie_auto_waitcfg_in        : 10; /* bit[1-10] : 硬件进入自动降频IDLE等待时钟周期配置 */
        unsigned int  pcie_auto_waitcfg_out       : 10; /* bit[11-20]: 硬件退出自动降频IDLE等待时钟周期配置 */
        unsigned int  pcie_div_auto_cfg           : 6;  /* bit[21-26]: 硬件自动降频倍数配置
                                                                       6'h00:1；
                                                                       6'h01:2；
                                                                       …
                                                                       6'h3F:64； */
        unsigned int  reserved_0                  : 1;  /* bit[27]   :  */
        unsigned int  reserved_1                  : 1;  /* bit[28]   :  */
        unsigned int  reserved_2                  : 1;  /* bit[29]   :  */
        unsigned int  reserved_3                  : 1;  /* bit[30]   :  */
        unsigned int  reserved_4                  : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_AO_AUTODIV2_UNION;
#endif
#define SOC_AO_CRG_AO_AUTODIV2_peri_div_auto_reduce_bypass_START  (0)
#define SOC_AO_CRG_AO_AUTODIV2_peri_div_auto_reduce_bypass_END    (0)
#define SOC_AO_CRG_AO_AUTODIV2_pcie_auto_waitcfg_in_START         (1)
#define SOC_AO_CRG_AO_AUTODIV2_pcie_auto_waitcfg_in_END           (10)
#define SOC_AO_CRG_AO_AUTODIV2_pcie_auto_waitcfg_out_START        (11)
#define SOC_AO_CRG_AO_AUTODIV2_pcie_auto_waitcfg_out_END          (20)
#define SOC_AO_CRG_AO_AUTODIV2_pcie_div_auto_cfg_START            (21)
#define SOC_AO_CRG_AO_AUTODIV2_pcie_div_auto_cfg_END              (26)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN0_SEC_UNION
 struct description   : SCPEREN0_SEC Register structure definition
                        Address Offset:0x900 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器0（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_timer1            : 1;  /* bit[0] : 外设时钟使能控制：
                                                                  0：写0无效果；
                                                                  1：使能IP时钟。 */
        unsigned int  reserved_0                : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  gt_pclk_ao_tzpc           : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  gt_pclk_efusec2           : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  gt_pclk_efusec            : 1;  /* bit[4] : 外设时钟使能控制：
                                                                  0：写0无效果；
                                                                  1：使能IP时钟。 */
        unsigned int  gt_clk_out0               : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  reserved_1                : 1;  /* bit[6] :  */
        unsigned int  gt_pclk_ao_gpio1_se       : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  gt_clk_ao_tcp             : 1;  /* bit[8] :  */
        unsigned int  gt_clk_ao_tcp_32k         : 1;  /* bit[9] :  */
        unsigned int  reserved_2                : 1;  /* bit[10]:  */
        unsigned int  reserved_3                : 1;  /* bit[11]:  */
        unsigned int  reserved_4                : 1;  /* bit[12]:  */
        unsigned int  reserved_5                : 1;  /* bit[13]:  */
        unsigned int  reserved_6                : 1;  /* bit[14]:  */
        unsigned int  reserved_7                : 1;  /* bit[15]:  */
        unsigned int  reserved_8                : 1;  /* bit[16]: 保留。 */
        unsigned int  reserved_9                : 1;  /* bit[17]:  */
        unsigned int  reserved_10               : 1;  /* bit[18]:  */
        unsigned int  gt_pclk_ao_ipc            : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[20]:  */
        unsigned int  reserved_12               : 1;  /* bit[21]:  */
        unsigned int  reserved_13               : 1;  /* bit[22]:  */
        unsigned int  reserved_14               : 1;  /* bit[23]:  */
        unsigned int  gt_pclkdbg_to_iomcu       : 1;  /* bit[24]:  */
        unsigned int  gt_clk_noc_aobus2iomcubus : 1;  /* bit[25]:  */
        unsigned int  reserved_15               : 1;  /* bit[26]:  */
        unsigned int  reserved_16               : 1;  /* bit[27]:  */
        unsigned int  reserved_17               : 1;  /* bit[28]:  */
        unsigned int  reserved_18               : 1;  /* bit[29]:  */
        unsigned int  reserved_19               : 1;  /* bit[30]:  */
        unsigned int  reserved_20               : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPEREN0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_timer1_START             (0)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_timer1_END               (0)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_ao_tzpc_START            (2)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_ao_tzpc_END              (2)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_efusec2_START            (3)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_efusec2_END              (3)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_efusec_START             (4)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_efusec_END               (4)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_out0_START                (5)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_out0_END                  (5)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_ao_gpio1_se_START        (7)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_ao_gpio1_se_END          (7)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_ao_tcp_START              (8)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_ao_tcp_END                (8)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_ao_tcp_32k_START          (9)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_ao_tcp_32k_END            (9)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_ao_ipc_START             (19)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclk_ao_ipc_END               (19)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclkdbg_to_iomcu_START        (24)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_pclkdbg_to_iomcu_END          (24)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_noc_aobus2iomcubus_START  (25)
#define SOC_AO_CRG_SCPEREN0_SEC_gt_clk_noc_aobus2iomcubus_END    (25)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS0_SEC_UNION
 struct description   : SCPERDIS0_SEC Register structure definition
                        Address Offset:0x904 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器0（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_timer1            : 1;  /* bit[0] : 外设时钟禁止控制：
                                                                  0：写0无效果；
                                                                  1：禁止IP时钟。 */
        unsigned int  reserved_0                : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  gt_pclk_ao_tzpc           : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  gt_pclk_efusec2           : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  gt_pclk_efusec            : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  gt_clk_out0               : 1;  /* bit[5] : 含义同bit0。 */
        unsigned int  reserved_1                : 1;  /* bit[6] :  */
        unsigned int  gt_pclk_ao_gpio1_se       : 1;  /* bit[7] : 含义同bit0。 */
        unsigned int  gt_clk_ao_tcp             : 1;  /* bit[8] :  */
        unsigned int  gt_clk_ao_tcp_32k         : 1;  /* bit[9] :  */
        unsigned int  reserved_2                : 1;  /* bit[10]:  */
        unsigned int  reserved_3                : 1;  /* bit[11]:  */
        unsigned int  reserved_4                : 1;  /* bit[12]:  */
        unsigned int  reserved_5                : 1;  /* bit[13]:  */
        unsigned int  reserved_6                : 1;  /* bit[14]:  */
        unsigned int  reserved_7                : 1;  /* bit[15]:  */
        unsigned int  reserved_8                : 1;  /* bit[16]: 保留。 */
        unsigned int  reserved_9                : 1;  /* bit[17]:  */
        unsigned int  reserved_10               : 1;  /* bit[18]:  */
        unsigned int  gt_pclk_ao_ipc            : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[20]:  */
        unsigned int  reserved_12               : 1;  /* bit[21]:  */
        unsigned int  reserved_13               : 1;  /* bit[22]:  */
        unsigned int  reserved_14               : 1;  /* bit[23]:  */
        unsigned int  gt_pclkdbg_to_iomcu       : 1;  /* bit[24]:  */
        unsigned int  gt_clk_noc_aobus2iomcubus : 1;  /* bit[25]:  */
        unsigned int  reserved_15               : 1;  /* bit[26]:  */
        unsigned int  reserved_16               : 1;  /* bit[27]:  */
        unsigned int  reserved_17               : 1;  /* bit[28]:  */
        unsigned int  reserved_18               : 1;  /* bit[29]:  */
        unsigned int  reserved_19               : 1;  /* bit[30]:  */
        unsigned int  reserved_20               : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERDIS0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_timer1_START             (0)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_timer1_END               (0)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_ao_tzpc_START            (2)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_ao_tzpc_END              (2)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_efusec2_START            (3)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_efusec2_END              (3)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_efusec_START             (4)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_efusec_END               (4)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_out0_START                (5)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_out0_END                  (5)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_ao_gpio1_se_START        (7)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_ao_gpio1_se_END          (7)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_ao_tcp_START              (8)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_ao_tcp_END                (8)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_ao_tcp_32k_START          (9)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_ao_tcp_32k_END            (9)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_ao_ipc_START             (19)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclk_ao_ipc_END               (19)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclkdbg_to_iomcu_START        (24)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_pclkdbg_to_iomcu_END          (24)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_noc_aobus2iomcubus_START  (25)
#define SOC_AO_CRG_SCPERDIS0_SEC_gt_clk_noc_aobus2iomcubus_END    (25)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN0_SEC_UNION
 struct description   : SCPERCLKEN0_SEC Register structure definition
                        Address Offset:0x908 Initial:0x1 Width:32
 register description : 外设时钟使能状态寄存器0（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_timer1            : 1;  /* bit[0] : 外设时钟使能状态：
                                                                  0：IP时钟使能撤销状态；
                                                                  1：IP时钟使能状态。 */
        unsigned int  reserved_0                : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_ao_tzpc           : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_efusec2           : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_efusec            : 1;  /* bit[4] :  */
        unsigned int  gt_clk_out0               : 1;  /* bit[5] :  */
        unsigned int  reserved_1                : 1;  /* bit[6] :  */
        unsigned int  gt_pclk_ao_gpio1_se       : 1;  /* bit[7] :  */
        unsigned int  gt_clk_ao_tcp             : 1;  /* bit[8] :  */
        unsigned int  gt_clk_ao_tcp_32k         : 1;  /* bit[9] :  */
        unsigned int  reserved_2                : 1;  /* bit[10]:  */
        unsigned int  reserved_3                : 1;  /* bit[11]:  */
        unsigned int  reserved_4                : 1;  /* bit[12]:  */
        unsigned int  reserved_5                : 1;  /* bit[13]:  */
        unsigned int  reserved_6                : 1;  /* bit[14]:  */
        unsigned int  reserved_7                : 1;  /* bit[15]:  */
        unsigned int  reserved_8                : 1;  /* bit[16]: 保留。 */
        unsigned int  reserved_9                : 1;  /* bit[17]:  */
        unsigned int  reserved_10               : 1;  /* bit[18]:  */
        unsigned int  gt_pclk_ao_ipc            : 1;  /* bit[19]: 含义同bit0。 */
        unsigned int  reserved_11               : 1;  /* bit[20]:  */
        unsigned int  reserved_12               : 1;  /* bit[21]:  */
        unsigned int  reserved_13               : 1;  /* bit[22]:  */
        unsigned int  reserved_14               : 1;  /* bit[23]:  */
        unsigned int  gt_pclkdbg_to_iomcu       : 1;  /* bit[24]:  */
        unsigned int  gt_clk_noc_aobus2iomcubus : 1;  /* bit[25]:  */
        unsigned int  reserved_15               : 1;  /* bit[26]:  */
        unsigned int  reserved_16               : 1;  /* bit[27]:  */
        unsigned int  reserved_17               : 1;  /* bit[28]:  */
        unsigned int  reserved_18               : 1;  /* bit[29]:  */
        unsigned int  reserved_19               : 1;  /* bit[30]:  */
        unsigned int  reserved_20               : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERCLKEN0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_timer1_START             (0)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_timer1_END               (0)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_ao_tzpc_START            (2)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_ao_tzpc_END              (2)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_efusec2_START            (3)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_efusec2_END              (3)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_efusec_START             (4)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_efusec_END               (4)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_out0_START                (5)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_out0_END                  (5)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_ao_gpio1_se_START        (7)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_ao_gpio1_se_END          (7)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_ao_tcp_START              (8)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_ao_tcp_END                (8)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_ao_tcp_32k_START          (9)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_ao_tcp_32k_END            (9)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_ao_ipc_START             (19)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclk_ao_ipc_END               (19)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclkdbg_to_iomcu_START        (24)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_pclkdbg_to_iomcu_END          (24)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_noc_aobus2iomcubus_START  (25)
#define SOC_AO_CRG_SCPERCLKEN0_SEC_gt_clk_noc_aobus2iomcubus_END    (25)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTEN0_SEC_UNION
 struct description   : SCPERRSTEN0_SEC Register structure definition
                        Address Offset:0x910 Initial:0x0 Width:32
 register description : 外设软复位使能寄存器0（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_prst_timer1      : 1;  /* bit[0] : IP软复位使能：
                                                            0：IP软复位使能状态不变；
                                                            1：IP软复位使能。 */
        unsigned int  reserved_0          : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  ip_prst_ao_ipc      : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  reserved_1          : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  ip_prst_ao_gpio1_se : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  ip_rst_aobus        : 1;  /* bit[5] :  */
        unsigned int  ip_rst_aonoc        : 1;  /* bit[6] :  */
        unsigned int  ip_prst_intr_hub    : 1;  /* bit[7] :  */
        unsigned int  reserved_2          : 1;  /* bit[8] :  */
        unsigned int  reserved_3          : 1;  /* bit[9] :  */
        unsigned int  reserved_4          : 1;  /* bit[10]:  */
        unsigned int  reserved_5          : 1;  /* bit[11]:  */
        unsigned int  reserved_6          : 1;  /* bit[12]:  */
        unsigned int  reserved_7          : 1;  /* bit[13]:  */
        unsigned int  reserved_8          : 1;  /* bit[14]:  */
        unsigned int  reserved_9          : 1;  /* bit[15]:  */
        unsigned int  reserved_10         : 1;  /* bit[16]:  */
        unsigned int  reserved_11         : 1;  /* bit[17]:  */
        unsigned int  reserved_12         : 1;  /* bit[18]:  */
        unsigned int  reserved_13         : 1;  /* bit[19]:  */
        unsigned int  reserved_14         : 1;  /* bit[20]:  */
        unsigned int  reserved_15         : 1;  /* bit[21]:  */
        unsigned int  reserved_16         : 1;  /* bit[22]:  */
        unsigned int  reserved_17         : 1;  /* bit[23]:  */
        unsigned int  reserved_18         : 1;  /* bit[24]:  */
        unsigned int  reserved_19         : 1;  /* bit[25]:  */
        unsigned int  reserved_20         : 1;  /* bit[26]:  */
        unsigned int  reserved_21         : 1;  /* bit[27]:  */
        unsigned int  reserved_22         : 1;  /* bit[28]:  */
        unsigned int  reserved_23         : 1;  /* bit[29]:  */
        unsigned int  reserved_24         : 1;  /* bit[30]:  */
        unsigned int  reserved_25         : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTEN0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_timer1_START       (0)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_timer1_END         (0)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_ao_ipc_START       (2)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_ao_ipc_END         (2)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_ao_gpio1_se_START  (4)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_ao_gpio1_se_END    (4)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_rst_aobus_START         (5)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_rst_aobus_END           (5)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_rst_aonoc_START         (6)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_rst_aonoc_END           (6)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_intr_hub_START     (7)
#define SOC_AO_CRG_SCPERRSTEN0_SEC_ip_prst_intr_hub_END       (7)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTDIS0_SEC_UNION
 struct description   : SCPERRSTDIS0_SEC Register structure definition
                        Address Offset:0x914 Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器0（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_prst_timer1      : 1;  /* bit[0] : IP软复位撤离：
                                                            0：IP软复位使能状态不变；
                                                            1：IP软复位撤离。 */
        unsigned int  reserved_0          : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  ip_prst_ao_ipc      : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  reserved_1          : 1;  /* bit[3] : 含义同bit0。 */
        unsigned int  ip_prst_ao_gpio1_se : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  ip_rst_aobus        : 1;  /* bit[5] :  */
        unsigned int  ip_rst_aonoc        : 1;  /* bit[6] :  */
        unsigned int  ip_prst_intr_hub    : 1;  /* bit[7] :  */
        unsigned int  reserved_2          : 1;  /* bit[8] :  */
        unsigned int  reserved_3          : 1;  /* bit[9] :  */
        unsigned int  reserved_4          : 1;  /* bit[10]:  */
        unsigned int  reserved_5          : 1;  /* bit[11]:  */
        unsigned int  reserved_6          : 1;  /* bit[12]:  */
        unsigned int  reserved_7          : 1;  /* bit[13]:  */
        unsigned int  reserved_8          : 1;  /* bit[14]:  */
        unsigned int  reserved_9          : 1;  /* bit[15]:  */
        unsigned int  reserved_10         : 1;  /* bit[16]:  */
        unsigned int  reserved_11         : 1;  /* bit[17]:  */
        unsigned int  reserved_12         : 1;  /* bit[18]:  */
        unsigned int  reserved_13         : 1;  /* bit[19]:  */
        unsigned int  reserved_14         : 1;  /* bit[20]:  */
        unsigned int  reserved_15         : 1;  /* bit[21]:  */
        unsigned int  reserved_16         : 1;  /* bit[22]:  */
        unsigned int  reserved_17         : 1;  /* bit[23]:  */
        unsigned int  reserved_18         : 1;  /* bit[24]:  */
        unsigned int  reserved_19         : 1;  /* bit[25]:  */
        unsigned int  reserved_20         : 1;  /* bit[26]:  */
        unsigned int  reserved_21         : 1;  /* bit[27]:  */
        unsigned int  reserved_22         : 1;  /* bit[28]:  */
        unsigned int  reserved_23         : 1;  /* bit[29]:  */
        unsigned int  reserved_24         : 1;  /* bit[30]:  */
        unsigned int  reserved_25         : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTDIS0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_timer1_START       (0)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_timer1_END         (0)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_ao_ipc_START       (2)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_ao_ipc_END         (2)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_ao_gpio1_se_START  (4)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_ao_gpio1_se_END    (4)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_rst_aobus_START         (5)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_rst_aobus_END           (5)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_rst_aonoc_START         (6)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_rst_aonoc_END           (6)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_intr_hub_START     (7)
#define SOC_AO_CRG_SCPERRSTDIS0_SEC_ip_prst_intr_hub_END       (7)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTSTAT0_SEC_UNION
 struct description   : SCPERRSTSTAT0_SEC Register structure definition
                        Address Offset:0x918 Initial:0x0 Width:32
 register description : 外设软复位状态寄存器0（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_prst_timer1      : 1;  /* bit[0] : 外设软复位使能状态：
                                                            0：IP软复位使能撤销；
                                                            1：IP软复位使能。 */
        unsigned int  reserved_0          : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  ip_prst_ao_ipc      : 1;  /* bit[2] : 含义同bit0。 */
        unsigned int  reserved_1          : 1;  /* bit[3] :  */
        unsigned int  ip_prst_ao_gpio1_se : 1;  /* bit[4] : 含义同bit0。 */
        unsigned int  ip_rst_aobus        : 1;  /* bit[5] :  */
        unsigned int  ip_rst_aonoc        : 1;  /* bit[6] :  */
        unsigned int  ip_prst_intr_hub    : 1;  /* bit[7] :  */
        unsigned int  reserved_2          : 1;  /* bit[8] :  */
        unsigned int  reserved_3          : 1;  /* bit[9] :  */
        unsigned int  reserved_4          : 1;  /* bit[10]:  */
        unsigned int  reserved_5          : 1;  /* bit[11]:  */
        unsigned int  reserved_6          : 1;  /* bit[12]:  */
        unsigned int  reserved_7          : 1;  /* bit[13]:  */
        unsigned int  reserved_8          : 1;  /* bit[14]:  */
        unsigned int  reserved_9          : 1;  /* bit[15]:  */
        unsigned int  reserved_10         : 1;  /* bit[16]:  */
        unsigned int  reserved_11         : 1;  /* bit[17]:  */
        unsigned int  reserved_12         : 1;  /* bit[18]:  */
        unsigned int  reserved_13         : 1;  /* bit[19]:  */
        unsigned int  reserved_14         : 1;  /* bit[20]:  */
        unsigned int  reserved_15         : 1;  /* bit[21]:  */
        unsigned int  reserved_16         : 1;  /* bit[22]:  */
        unsigned int  reserved_17         : 1;  /* bit[23]:  */
        unsigned int  reserved_18         : 1;  /* bit[24]:  */
        unsigned int  reserved_19         : 1;  /* bit[25]:  */
        unsigned int  reserved_20         : 1;  /* bit[26]:  */
        unsigned int  reserved_21         : 1;  /* bit[27]:  */
        unsigned int  reserved_22         : 1;  /* bit[28]:  */
        unsigned int  reserved_23         : 1;  /* bit[29]:  */
        unsigned int  reserved_24         : 1;  /* bit[30]:  */
        unsigned int  reserved_25         : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTSTAT0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_timer1_START       (0)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_timer1_END         (0)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_ao_ipc_START       (2)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_ao_ipc_END         (2)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_ao_gpio1_se_START  (4)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_ao_gpio1_se_END    (4)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_rst_aobus_START         (5)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_rst_aobus_END           (5)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_rst_aonoc_START         (6)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_rst_aonoc_END           (6)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_intr_hub_START     (7)
#define SOC_AO_CRG_SCPERRSTSTAT0_SEC_ip_prst_intr_hub_END       (7)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTEN1_SEC_UNION
 struct description   : SCPERRSTEN1_SEC Register structure definition
                        Address Offset:0x990 Initial:0x0 Width:32
 register description : 外设软复位使能寄存器1（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_asp_subsys     : 1;  /* bit[0] : IP软复位使能：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位使能。 */
        unsigned int  ip_rst_asp_subsys_crg : 1;  /* bit[1] : 保留。 */
        unsigned int  ip_prst_hickm         : 1;  /* bit[2] :  */
        unsigned int  ip_rst_sci0           : 1;  /* bit[3] :  */
        unsigned int  ip_rst_sci1           : 1;  /* bit[4] :  */
        unsigned int  ip_prst_timer2        : 1;  /* bit[5] :  */
        unsigned int  ip_prst_timer3        : 1;  /* bit[6] :  */
        unsigned int  ip_prst_timer7        : 1;  /* bit[7] :  */
        unsigned int  ip_prst_rtc1          : 1;  /* bit[8] :  */
        unsigned int  ip_rst_asp_cfg        : 1;  /* bit[9] :  */
        unsigned int  ip_rst_mad            : 1;  /* bit[10]:  */
        unsigned int  reserved_0            : 1;  /* bit[11]:  */
        unsigned int  ip_rst_hisee          : 1;  /* bit[12]:  */
        unsigned int  reserved_1            : 1;  /* bit[13]:  */
        unsigned int  ip_rst_hieps_tee_crg  : 1;  /* bit[14]:  */
        unsigned int  ip_rst_hieps_tee      : 1;  /* bit[15]:  */
        unsigned int  ip_rst_pll_fsm        : 1;  /* bit[16]:  */
        unsigned int  ip_rst_qic_hieps_tee  : 1;  /* bit[17]:  */
        unsigned int  ip_rst_ao_tcp         : 1;  /* bit[18]:  */
        unsigned int  reserved_2            : 1;  /* bit[19]:  */
        unsigned int  reserved_3            : 1;  /* bit[20]:  */
        unsigned int  reserved_4            : 1;  /* bit[21]:  */
        unsigned int  reserved_5            : 1;  /* bit[22]:  */
        unsigned int  reserved_6            : 1;  /* bit[23]:  */
        unsigned int  reserved_7            : 1;  /* bit[24]:  */
        unsigned int  reserved_8            : 1;  /* bit[25]:  */
        unsigned int  reserved_9            : 1;  /* bit[26]:  */
        unsigned int  reserved_10           : 1;  /* bit[27]:  */
        unsigned int  reserved_11           : 1;  /* bit[28]:  */
        unsigned int  ip_rst_qic_hieps_ree  : 1;  /* bit[29]:  */
        unsigned int  ip_rst_hieps_ree_crg  : 1;  /* bit[30]:  */
        unsigned int  ip_rst_hieps_ree      : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTEN1_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_asp_subsys_START      (0)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_asp_subsys_END        (0)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_asp_subsys_crg_START  (1)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_asp_subsys_crg_END    (1)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_hickm_START          (2)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_hickm_END            (2)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_sci0_START            (3)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_sci0_END              (3)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_sci1_START            (4)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_sci1_END              (4)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_timer2_START         (5)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_timer2_END           (5)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_timer3_START         (6)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_timer3_END           (6)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_timer7_START         (7)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_timer7_END           (7)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_rtc1_START           (8)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_prst_rtc1_END             (8)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_asp_cfg_START         (9)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_asp_cfg_END           (9)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_mad_START             (10)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_mad_END               (10)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hisee_START           (12)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hisee_END             (12)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_tee_crg_START   (14)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_tee_crg_END     (14)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_tee_START       (15)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_tee_END         (15)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_pll_fsm_START         (16)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_pll_fsm_END           (16)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_qic_hieps_tee_START   (17)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_qic_hieps_tee_END     (17)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_ao_tcp_START          (18)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_ao_tcp_END            (18)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_qic_hieps_ree_START   (29)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_qic_hieps_ree_END     (29)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_ree_crg_START   (30)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_ree_crg_END     (30)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_ree_START       (31)
#define SOC_AO_CRG_SCPERRSTEN1_SEC_ip_rst_hieps_ree_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTDIS1_SEC_UNION
 struct description   : SCPERRSTDIS1_SEC Register structure definition
                        Address Offset:0x994 Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器1（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_asp_subsys     : 1;  /* bit[0] : IP软复位撤离：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位撤离。 */
        unsigned int  ip_rst_asp_subsys_crg : 1;  /* bit[1] : 保留。 */
        unsigned int  ip_prst_hickm         : 1;  /* bit[2] :  */
        unsigned int  ip_rst_sci0           : 1;  /* bit[3] :  */
        unsigned int  ip_rst_sci1           : 1;  /* bit[4] :  */
        unsigned int  ip_prst_timer2        : 1;  /* bit[5] :  */
        unsigned int  ip_prst_timer3        : 1;  /* bit[6] :  */
        unsigned int  ip_prst_timer7        : 1;  /* bit[7] :  */
        unsigned int  ip_prst_rtc1          : 1;  /* bit[8] :  */
        unsigned int  ip_rst_asp_cfg        : 1;  /* bit[9] :  */
        unsigned int  ip_rst_mad            : 1;  /* bit[10]:  */
        unsigned int  reserved_0            : 1;  /* bit[11]:  */
        unsigned int  ip_rst_hisee          : 1;  /* bit[12]:  */
        unsigned int  reserved_1            : 1;  /* bit[13]:  */
        unsigned int  ip_rst_hieps_tee_crg  : 1;  /* bit[14]:  */
        unsigned int  ip_rst_hieps_tee      : 1;  /* bit[15]:  */
        unsigned int  ip_rst_pll_fsm        : 1;  /* bit[16]:  */
        unsigned int  ip_rst_qic_hieps_tee  : 1;  /* bit[17]:  */
        unsigned int  ip_rst_ao_tcp         : 1;  /* bit[18]:  */
        unsigned int  reserved_2            : 1;  /* bit[19]:  */
        unsigned int  reserved_3            : 1;  /* bit[20]:  */
        unsigned int  reserved_4            : 1;  /* bit[21]:  */
        unsigned int  reserved_5            : 1;  /* bit[22]:  */
        unsigned int  reserved_6            : 1;  /* bit[23]:  */
        unsigned int  reserved_7            : 1;  /* bit[24]:  */
        unsigned int  reserved_8            : 1;  /* bit[25]:  */
        unsigned int  reserved_9            : 1;  /* bit[26]:  */
        unsigned int  reserved_10           : 1;  /* bit[27]:  */
        unsigned int  reserved_11           : 1;  /* bit[28]:  */
        unsigned int  ip_rst_qic_hieps_ree  : 1;  /* bit[29]:  */
        unsigned int  ip_rst_hieps_ree_crg  : 1;  /* bit[30]:  */
        unsigned int  ip_rst_hieps_ree      : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTDIS1_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_asp_subsys_START      (0)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_asp_subsys_END        (0)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_asp_subsys_crg_START  (1)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_asp_subsys_crg_END    (1)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_hickm_START          (2)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_hickm_END            (2)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_sci0_START            (3)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_sci0_END              (3)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_sci1_START            (4)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_sci1_END              (4)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_timer2_START         (5)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_timer2_END           (5)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_timer3_START         (6)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_timer3_END           (6)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_timer7_START         (7)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_timer7_END           (7)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_rtc1_START           (8)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_prst_rtc1_END             (8)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_asp_cfg_START         (9)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_asp_cfg_END           (9)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_mad_START             (10)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_mad_END               (10)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hisee_START           (12)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hisee_END             (12)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_tee_crg_START   (14)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_tee_crg_END     (14)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_tee_START       (15)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_tee_END         (15)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_pll_fsm_START         (16)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_pll_fsm_END           (16)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_qic_hieps_tee_START   (17)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_qic_hieps_tee_END     (17)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_ao_tcp_START          (18)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_ao_tcp_END            (18)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_qic_hieps_ree_START   (29)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_qic_hieps_ree_END     (29)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_ree_crg_START   (30)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_ree_crg_END     (30)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_ree_START       (31)
#define SOC_AO_CRG_SCPERRSTDIS1_SEC_ip_rst_hieps_ree_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTSTAT1_SEC_UNION
 struct description   : SCPERRSTSTAT1_SEC Register structure definition
                        Address Offset:0x998 Initial:0x1 Width:32
 register description : 外设软复位状态寄存器1（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_asp_subsys     : 1;  /* bit[0] : 外设软复位使能状态：
                                                              0：IP软复位使能撤销；
                                                              1：IP软复位使能。 */
        unsigned int  ip_rst_asp_subsys_crg : 1;  /* bit[1] :  */
        unsigned int  ip_prst_hickm         : 1;  /* bit[2] :  */
        unsigned int  ip_rst_sci0           : 1;  /* bit[3] :  */
        unsigned int  ip_rst_sci1           : 1;  /* bit[4] :  */
        unsigned int  ip_prst_timer2        : 1;  /* bit[5] :  */
        unsigned int  ip_prst_timer3        : 1;  /* bit[6] :  */
        unsigned int  ip_prst_timer7        : 1;  /* bit[7] :  */
        unsigned int  ip_prst_rtc1          : 1;  /* bit[8] :  */
        unsigned int  ip_rst_asp_cfg        : 1;  /* bit[9] :  */
        unsigned int  ip_rst_mad            : 1;  /* bit[10]:  */
        unsigned int  reserved_0            : 1;  /* bit[11]:  */
        unsigned int  ip_rst_hisee          : 1;  /* bit[12]:  */
        unsigned int  reserved_1            : 1;  /* bit[13]:  */
        unsigned int  ip_rst_hieps_tee_crg  : 1;  /* bit[14]:  */
        unsigned int  ip_rst_hieps_tee      : 1;  /* bit[15]:  */
        unsigned int  ip_rst_pll_fsm        : 1;  /* bit[16]:  */
        unsigned int  ip_rst_qic_hieps_tee  : 1;  /* bit[17]:  */
        unsigned int  ip_rst_ao_tcp         : 1;  /* bit[18]:  */
        unsigned int  reserved_2            : 1;  /* bit[19]:  */
        unsigned int  reserved_3            : 1;  /* bit[20]:  */
        unsigned int  reserved_4            : 1;  /* bit[21]:  */
        unsigned int  reserved_5            : 1;  /* bit[22]:  */
        unsigned int  reserved_6            : 1;  /* bit[23]:  */
        unsigned int  reserved_7            : 1;  /* bit[24]:  */
        unsigned int  reserved_8            : 1;  /* bit[25]:  */
        unsigned int  reserved_9            : 1;  /* bit[26]:  */
        unsigned int  reserved_10           : 1;  /* bit[27]:  */
        unsigned int  reserved_11           : 1;  /* bit[28]:  */
        unsigned int  ip_rst_qic_hieps_ree  : 1;  /* bit[29]:  */
        unsigned int  ip_rst_hieps_ree_crg  : 1;  /* bit[30]:  */
        unsigned int  ip_rst_hieps_ree      : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTSTAT1_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_asp_subsys_START      (0)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_asp_subsys_END        (0)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_asp_subsys_crg_START  (1)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_asp_subsys_crg_END    (1)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_hickm_START          (2)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_hickm_END            (2)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_sci0_START            (3)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_sci0_END              (3)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_sci1_START            (4)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_sci1_END              (4)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_timer2_START         (5)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_timer2_END           (5)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_timer3_START         (6)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_timer3_END           (6)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_timer7_START         (7)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_timer7_END           (7)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_rtc1_START           (8)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_prst_rtc1_END             (8)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_asp_cfg_START         (9)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_asp_cfg_END           (9)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_mad_START             (10)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_mad_END               (10)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hisee_START           (12)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hisee_END             (12)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_tee_crg_START   (14)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_tee_crg_END     (14)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_tee_START       (15)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_tee_END         (15)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_pll_fsm_START         (16)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_pll_fsm_END           (16)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_qic_hieps_tee_START   (17)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_qic_hieps_tee_END     (17)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_ao_tcp_START          (18)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_ao_tcp_END            (18)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_qic_hieps_ree_START   (29)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_qic_hieps_ree_END     (29)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_ree_crg_START   (30)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_ree_crg_END     (30)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_ree_START       (31)
#define SOC_AO_CRG_SCPERRSTSTAT1_SEC_ip_rst_hieps_ree_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTEN2_SEC_UNION
 struct description   : SCPERRSTEN2_SEC Register structure definition
                        Address Offset:0x9A0 Initial:0x00000000 Width:32
 register description : 外设软复位使能寄存器2（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_iomcu : 1;  /* bit[0]   : IP软复位使能：
                                                       0：IP软复位使能状态不变；
                                                       1：IP软复位使能。 */
        unsigned int  reserved     : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTEN2_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTEN2_SEC_ip_rst_iomcu_START  (0)
#define SOC_AO_CRG_SCPERRSTEN2_SEC_ip_rst_iomcu_END    (0)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTDIS2_SEC_UNION
 struct description   : SCPERRSTDIS2_SEC Register structure definition
                        Address Offset:0x9A4 Initial:0x00000000 Width:32
 register description : 外设软复位撤离寄存器2（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_iomcu : 1;  /* bit[0]   : IP软复位撤离：
                                                       0：IP软复位使能状态不变；
                                                       1：IP软复位撤离。 */
        unsigned int  reserved     : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTDIS2_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTDIS2_SEC_ip_rst_iomcu_START  (0)
#define SOC_AO_CRG_SCPERRSTDIS2_SEC_ip_rst_iomcu_END    (0)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERRSTSTAT2_SEC_UNION
 struct description   : SCPERRSTSTAT2_SEC Register structure definition
                        Address Offset:0x9A8 Initial:0x7FFFFFFF Width:32
 register description : 外设软复位状态寄存器2（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_iomcu : 1;  /* bit[0]   : 输出控制crg的rsten0_sec[24]
                                                       外设软复位使能状态：
                                                       0：IP软复位使能撤销；
                                                       1：IP软复位使能。 */
        unsigned int  reserved     : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERRSTSTAT2_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERRSTSTAT2_SEC_ip_rst_iomcu_START  (0)
#define SOC_AO_CRG_SCPERRSTSTAT2_SEC_ip_rst_iomcu_END    (0)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN3_SEC_UNION
 struct description   : SCPEREN3_SEC Register structure definition
                        Address Offset:0x9C0 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器3（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_hickm           : 1;  /* bit[0]    : 外设时钟使能控制：
                                                                   0：写0无效果；
                                                                   1：使能IP时钟。 */
        unsigned int  gt_clk_sci0             : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  gt_clk_sci1             : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  gt_pclk_timer2          : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  reserved_0              : 1;  /* bit[4]    :  */
        unsigned int  gt_pclk_timer3          : 1;  /* bit[5]    :  */
        unsigned int  reserved_1              : 1;  /* bit[6]    :  */
        unsigned int  gt_pclk_timer7          : 1;  /* bit[7]    :  */
        unsigned int  reserved_2              : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_ref_crc          : 1;  /* bit[9]    :  */
        unsigned int  gt_pclk_rtc1            : 1;  /* bit[10]   :  */
        unsigned int  gt_pclk_ao_wd1          : 1;  /* bit[11]   :  */
        unsigned int  gt_pclk_ao_wd           : 1;  /* bit[12]   :  */
        unsigned int  gt_clk_asp_subsys_lpmcu : 1;  /* bit[13]   :  */
        unsigned int  gt_clk_asp_codec_lpm3   : 1;  /* bit[14]   :  */
        unsigned int  gt_clk_sif_out          : 1;  /* bit[15]   :  */
        unsigned int  gt_clk_mad_lpm3         : 1;  /* bit[16]   :  */
        unsigned int  reserved_3              : 1;  /* bit[17]   :  */
        unsigned int  reserved_4              : 1;  /* bit[18]   :  */
        unsigned int  reserved_5              : 1;  /* bit[19]   :  */
        unsigned int  reserved_6              : 1;  /* bit[20]   :  */
        unsigned int  reserved_7              : 6;  /* bit[21-26]:  */
        unsigned int  gt_atclk_to_iomcu       : 1;  /* bit[27]   :  */
        unsigned int  reserved_8              : 1;  /* bit[28]   :  */
        unsigned int  reserved_9              : 1;  /* bit[29]   :  */
        unsigned int  reserved_10             : 1;  /* bit[30]   :  */
        unsigned int  reserved_11             : 1;  /* bit[31]   : 安全控制：
                                                                   0：写0无效果；
                                                                   1：写1开钟。 */
    } reg;
} SOC_AO_CRG_SCPEREN3_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_hickm_START            (0)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_hickm_END              (0)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_sci0_START              (1)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_sci0_END                (1)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_sci1_START              (2)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_sci1_END                (2)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_timer2_START           (3)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_timer2_END             (3)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_timer3_START           (5)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_timer3_END             (5)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_timer7_START           (7)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_timer7_END             (7)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_ref_crc_START           (9)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_ref_crc_END             (9)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_rtc1_START             (10)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_rtc1_END               (10)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_ao_wd1_START           (11)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_ao_wd1_END             (11)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_ao_wd_START            (12)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_pclk_ao_wd_END              (12)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_asp_subsys_lpmcu_START  (13)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_asp_subsys_lpmcu_END    (13)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_asp_codec_lpm3_START    (14)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_asp_codec_lpm3_END      (14)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_sif_out_START           (15)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_sif_out_END             (15)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_mad_lpm3_START          (16)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_clk_mad_lpm3_END            (16)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_atclk_to_iomcu_START        (27)
#define SOC_AO_CRG_SCPEREN3_SEC_gt_atclk_to_iomcu_END          (27)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS3_SEC_UNION
 struct description   : SCPERDIS3_SEC Register structure definition
                        Address Offset:0x9C4 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器3（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_hickm           : 1;  /* bit[0]    : 外设时钟使能控制：
                                                                   0：写0无效果；
                                                                   1：关闭IP时钟。 */
        unsigned int  gt_clk_sci0             : 1;  /* bit[1]    : 含义同bit0。 */
        unsigned int  gt_clk_sci1             : 1;  /* bit[2]    : 含义同bit0。 */
        unsigned int  gt_pclk_timer2          : 1;  /* bit[3]    : 含义同bit0。 */
        unsigned int  reserved_0              : 1;  /* bit[4]    :  */
        unsigned int  gt_pclk_timer3          : 1;  /* bit[5]    :  */
        unsigned int  reserved_1              : 1;  /* bit[6]    :  */
        unsigned int  gt_pclk_timer7          : 1;  /* bit[7]    :  */
        unsigned int  reserved_2              : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_ref_crc          : 1;  /* bit[9]    :  */
        unsigned int  gt_pclk_rtc1            : 1;  /* bit[10]   :  */
        unsigned int  gt_pclk_ao_wd1          : 1;  /* bit[11]   :  */
        unsigned int  gt_pclk_ao_wd           : 1;  /* bit[12]   :  */
        unsigned int  gt_clk_asp_subsys_lpmcu : 1;  /* bit[13]   :  */
        unsigned int  gt_clk_asp_codec_lpm3   : 1;  /* bit[14]   :  */
        unsigned int  gt_clk_sif_out          : 1;  /* bit[15]   :  */
        unsigned int  gt_clk_mad_lpm3         : 1;  /* bit[16]   :  */
        unsigned int  reserved_3              : 1;  /* bit[17]   :  */
        unsigned int  reserved_4              : 1;  /* bit[18]   :  */
        unsigned int  reserved_5              : 1;  /* bit[19]   :  */
        unsigned int  reserved_6              : 1;  /* bit[20]   :  */
        unsigned int  reserved_7              : 6;  /* bit[21-26]:  */
        unsigned int  gt_atclk_to_iomcu       : 1;  /* bit[27]   :  */
        unsigned int  reserved_8              : 1;  /* bit[28]   :  */
        unsigned int  reserved_9              : 1;  /* bit[29]   :  */
        unsigned int  reserved_10             : 1;  /* bit[30]   :  */
        unsigned int  reserved_11             : 1;  /* bit[31]   : 安全控制：
                                                                   0：写0无效果；
                                                                   1：写1关钟。 */
    } reg;
} SOC_AO_CRG_SCPERDIS3_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_hickm_START            (0)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_hickm_END              (0)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_sci0_START              (1)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_sci0_END                (1)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_sci1_START              (2)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_sci1_END                (2)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_timer2_START           (3)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_timer2_END             (3)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_timer3_START           (5)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_timer3_END             (5)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_timer7_START           (7)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_timer7_END             (7)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_ref_crc_START           (9)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_ref_crc_END             (9)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_rtc1_START             (10)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_rtc1_END               (10)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_ao_wd1_START           (11)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_ao_wd1_END             (11)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_ao_wd_START            (12)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_pclk_ao_wd_END              (12)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_asp_subsys_lpmcu_START  (13)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_asp_subsys_lpmcu_END    (13)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_asp_codec_lpm3_START    (14)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_asp_codec_lpm3_END      (14)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_sif_out_START           (15)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_sif_out_END             (15)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_mad_lpm3_START          (16)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_clk_mad_lpm3_END            (16)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_atclk_to_iomcu_START        (27)
#define SOC_AO_CRG_SCPERDIS3_SEC_gt_atclk_to_iomcu_END          (27)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN3_SEC_UNION
 struct description   : SCPERCLKEN3_SEC Register structure definition
                        Address Offset:0x9C8 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器3（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_pclk_hickm           : 1;  /* bit[0]    : hickm配置口时钟门控 */
        unsigned int  gt_clk_sci0             : 1;  /* bit[1]    :  */
        unsigned int  gt_clk_sci1             : 1;  /* bit[2]    :  */
        unsigned int  gt_pclk_timer2          : 1;  /* bit[3]    :  */
        unsigned int  reserved_0              : 1;  /* bit[4]    :  */
        unsigned int  gt_pclk_timer3          : 1;  /* bit[5]    :  */
        unsigned int  reserved_1              : 1;  /* bit[6]    :  */
        unsigned int  gt_pclk_timer7          : 1;  /* bit[7]    :  */
        unsigned int  reserved_2              : 1;  /* bit[8]    :  */
        unsigned int  gt_clk_ref_crc          : 1;  /* bit[9]    :  */
        unsigned int  gt_pclk_rtc1            : 1;  /* bit[10]   :  */
        unsigned int  gt_pclk_ao_wd1          : 1;  /* bit[11]   :  */
        unsigned int  gt_pclk_ao_wd           : 1;  /* bit[12]   :  */
        unsigned int  gt_clk_asp_subsys_lpmcu : 1;  /* bit[13]   :  */
        unsigned int  gt_clk_asp_codec_lpm3   : 1;  /* bit[14]   :  */
        unsigned int  gt_clk_sif_out          : 1;  /* bit[15]   :  */
        unsigned int  gt_clk_mad_lpm3         : 1;  /* bit[16]   :  */
        unsigned int  reserved_3              : 1;  /* bit[17]   :  */
        unsigned int  reserved_4              : 1;  /* bit[18]   :  */
        unsigned int  reserved_5              : 1;  /* bit[19]   :  */
        unsigned int  reserved_6              : 1;  /* bit[20]   :  */
        unsigned int  reserved_7              : 6;  /* bit[21-26]:  */
        unsigned int  gt_atclk_to_iomcu       : 1;  /* bit[27]   :  */
        unsigned int  reserved_8              : 1;  /* bit[28]   :  */
        unsigned int  reserved_9              : 1;  /* bit[29]   :  */
        unsigned int  reserved_10             : 1;  /* bit[30]   :  */
        unsigned int  reserved_11             : 1;  /* bit[31]   :  */
    } reg;
} SOC_AO_CRG_SCPERCLKEN3_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_hickm_START            (0)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_hickm_END              (0)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_sci0_START              (1)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_sci0_END                (1)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_sci1_START              (2)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_sci1_END                (2)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_timer2_START           (3)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_timer2_END             (3)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_timer3_START           (5)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_timer3_END             (5)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_timer7_START           (7)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_timer7_END             (7)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_ref_crc_START           (9)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_ref_crc_END             (9)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_rtc1_START             (10)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_rtc1_END               (10)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_ao_wd1_START           (11)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_ao_wd1_END             (11)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_ao_wd_START            (12)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_pclk_ao_wd_END              (12)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_asp_subsys_lpmcu_START  (13)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_asp_subsys_lpmcu_END    (13)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_asp_codec_lpm3_START    (14)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_asp_codec_lpm3_END      (14)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_sif_out_START           (15)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_sif_out_END             (15)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_mad_lpm3_START          (16)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_clk_mad_lpm3_END            (16)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_atclk_to_iomcu_START        (27)
#define SOC_AO_CRG_SCPERCLKEN3_SEC_gt_atclk_to_iomcu_END          (27)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPEREN4_SEC_UNION
 struct description   : SCPEREN4_SEC Register structure definition
                        Address Offset:0x9D0 Initial:0x0 Width:32
 register description : 外设软复位使能寄存器4（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_atclk_to_ao              : 1;  /* bit[0] : IP软复位使能：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位使能。 */
        unsigned int  reserved_0                  : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_1                  : 1;  /* bit[2] :  */
        unsigned int  gt_clk_hieps_tee            : 1;  /* bit[3] :  */
        unsigned int  reserved_2                  : 1;  /* bit[4] :  */
        unsigned int  gt_clk_hieps_tcxo           : 1;  /* bit[5] :  */
        unsigned int  reserved_3                  : 1;  /* bit[6] :  */
        unsigned int  gt_clk_hisee_pll            : 1;  /* bit[7] :  */
        unsigned int  gt_clk_hieps_ree            : 1;  /* bit[8] :  */
        unsigned int  reserved_4                  : 1;  /* bit[9] :  */
        unsigned int  gt_clk_hisee_ref            : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_spidma           : 1;  /* bit[11]:  */
        unsigned int  gt_clk_spll_sscg            : 1;  /* bit[12]:  */
        unsigned int  gt_clk_aobus                : 1;  /* bit[13]:  */
        unsigned int  gt_clk_aonoc2mspcbus        : 1;  /* bit[14]:  */
        unsigned int  gt_clk_noc_aobus2sysbusddrc : 1;  /* bit[15]:  */
        unsigned int  reserved_5                  : 1;  /* bit[16]:  */
        unsigned int  gt_clk_autodiv_aonoc_fll    : 1;  /* bit[17]:  */
        unsigned int  gt_timerclk_refh            : 1;  /* bit[18]:  */
        unsigned int  gt_clk_pll_fsm              : 1;  /* bit[19]:  */
        unsigned int  gt_clk_fll_src_peri         : 1;  /* bit[20]:  */
        unsigned int  reserved_6                  : 1;  /* bit[21]:  */
        unsigned int  reserved_7                  : 1;  /* bit[22]:  */
        unsigned int  reserved_8                  : 1;  /* bit[23]:  */
        unsigned int  reserved_9                  : 1;  /* bit[24]:  */
        unsigned int  reserved_10                 : 1;  /* bit[25]:  */
        unsigned int  reserved_11                 : 1;  /* bit[26]:  */
        unsigned int  reserved_12                 : 1;  /* bit[27]:  */
        unsigned int  reserved_13                 : 1;  /* bit[28]:  */
        unsigned int  reserved_14                 : 1;  /* bit[29]:  */
        unsigned int  reserved_15                 : 1;  /* bit[30]:  */
        unsigned int  reserved_16                 : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPEREN4_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPEREN4_SEC_gt_atclk_to_ao_START               (0)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_atclk_to_ao_END                 (0)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hieps_tee_START             (3)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hieps_tee_END               (3)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hieps_tcxo_START            (5)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hieps_tcxo_END              (5)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hisee_pll_START             (7)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hisee_pll_END               (7)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hieps_ree_START             (8)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hieps_ree_END               (8)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hisee_ref_START             (10)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_hisee_ref_END               (10)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_qic_spidma_START            (11)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_qic_spidma_END              (11)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_spll_sscg_START             (12)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_spll_sscg_END               (12)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_aobus_START                 (13)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_aobus_END                   (13)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_aonoc2mspcbus_START         (14)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_aonoc2mspcbus_END           (14)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_noc_aobus2sysbusddrc_START  (15)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_noc_aobus2sysbusddrc_END    (15)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_autodiv_aonoc_fll_START     (17)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_autodiv_aonoc_fll_END       (17)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_timerclk_refh_START             (18)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_timerclk_refh_END               (18)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_pll_fsm_START               (19)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_pll_fsm_END                 (19)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_fll_src_peri_START          (20)
#define SOC_AO_CRG_SCPEREN4_SEC_gt_clk_fll_src_peri_END            (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERDIS4_SEC_UNION
 struct description   : SCPERDIS4_SEC Register structure definition
                        Address Offset:0x9D4 Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器4（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_atclk_to_ao              : 1;  /* bit[0] : IP软复位使能：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位使能。 */
        unsigned int  reserved_0                  : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_1                  : 1;  /* bit[2] :  */
        unsigned int  gt_clk_hieps_tee            : 1;  /* bit[3] :  */
        unsigned int  reserved_2                  : 1;  /* bit[4] :  */
        unsigned int  gt_clk_hieps_tcxo           : 1;  /* bit[5] :  */
        unsigned int  reserved_3                  : 1;  /* bit[6] :  */
        unsigned int  gt_clk_hisee_pll            : 1;  /* bit[7] :  */
        unsigned int  gt_clk_hieps_ree            : 1;  /* bit[8] :  */
        unsigned int  reserved_4                  : 1;  /* bit[9] :  */
        unsigned int  gt_clk_hisee_ref            : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_spidma           : 1;  /* bit[11]:  */
        unsigned int  gt_clk_spll_sscg            : 1;  /* bit[12]:  */
        unsigned int  gt_clk_aobus                : 1;  /* bit[13]:  */
        unsigned int  gt_clk_aonoc2mspcbus        : 1;  /* bit[14]:  */
        unsigned int  gt_clk_noc_aobus2sysbusddrc : 1;  /* bit[15]:  */
        unsigned int  reserved_5                  : 1;  /* bit[16]:  */
        unsigned int  gt_clk_autodiv_aonoc_fll    : 1;  /* bit[17]:  */
        unsigned int  gt_timerclk_refh            : 1;  /* bit[18]:  */
        unsigned int  gt_clk_pll_fsm              : 1;  /* bit[19]:  */
        unsigned int  gt_clk_fll_src_peri         : 1;  /* bit[20]:  */
        unsigned int  reserved_6                  : 1;  /* bit[21]:  */
        unsigned int  reserved_7                  : 1;  /* bit[22]:  */
        unsigned int  reserved_8                  : 1;  /* bit[23]:  */
        unsigned int  reserved_9                  : 1;  /* bit[24]:  */
        unsigned int  reserved_10                 : 1;  /* bit[25]:  */
        unsigned int  reserved_11                 : 1;  /* bit[26]:  */
        unsigned int  reserved_12                 : 1;  /* bit[27]:  */
        unsigned int  reserved_13                 : 1;  /* bit[28]:  */
        unsigned int  reserved_14                 : 1;  /* bit[29]:  */
        unsigned int  reserved_15                 : 1;  /* bit[30]:  */
        unsigned int  reserved_16                 : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERDIS4_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_atclk_to_ao_START               (0)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_atclk_to_ao_END                 (0)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hieps_tee_START             (3)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hieps_tee_END               (3)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hieps_tcxo_START            (5)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hieps_tcxo_END              (5)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hisee_pll_START             (7)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hisee_pll_END               (7)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hieps_ree_START             (8)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hieps_ree_END               (8)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hisee_ref_START             (10)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_hisee_ref_END               (10)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_qic_spidma_START            (11)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_qic_spidma_END              (11)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_spll_sscg_START             (12)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_spll_sscg_END               (12)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_aobus_START                 (13)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_aobus_END                   (13)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_aonoc2mspcbus_START         (14)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_aonoc2mspcbus_END           (14)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_noc_aobus2sysbusddrc_START  (15)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_noc_aobus2sysbusddrc_END    (15)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_autodiv_aonoc_fll_START     (17)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_autodiv_aonoc_fll_END       (17)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_timerclk_refh_START             (18)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_timerclk_refh_END               (18)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_pll_fsm_START               (19)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_pll_fsm_END                 (19)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_fll_src_peri_START          (20)
#define SOC_AO_CRG_SCPERDIS4_SEC_gt_clk_fll_src_peri_END            (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCPERCLKEN4_SEC_UNION
 struct description   : SCPERCLKEN4_SEC Register structure definition
                        Address Offset:0x9D8 Initial:0x0 Width:32
 register description : 外设软复位状态寄存器4（安全）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_atclk_to_ao              : 1;  /* bit[0] : IP软复位使能：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位使能。 */
        unsigned int  reserved_0                  : 1;  /* bit[1] : 含义同bit0。 */
        unsigned int  reserved_1                  : 1;  /* bit[2] :  */
        unsigned int  gt_clk_hieps_tee            : 1;  /* bit[3] :  */
        unsigned int  reserved_2                  : 1;  /* bit[4] :  */
        unsigned int  gt_clk_hieps_tcxo           : 1;  /* bit[5] :  */
        unsigned int  reserved_3                  : 1;  /* bit[6] :  */
        unsigned int  gt_clk_hisee_pll            : 1;  /* bit[7] :  */
        unsigned int  gt_clk_hieps_ree            : 1;  /* bit[8] :  */
        unsigned int  reserved_4                  : 1;  /* bit[9] :  */
        unsigned int  gt_clk_hisee_ref            : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_spidma           : 1;  /* bit[11]:  */
        unsigned int  gt_clk_spll_sscg            : 1;  /* bit[12]:  */
        unsigned int  gt_clk_aobus                : 1;  /* bit[13]:  */
        unsigned int  gt_clk_aonoc2mspcbus        : 1;  /* bit[14]:  */
        unsigned int  gt_clk_noc_aobus2sysbusddrc : 1;  /* bit[15]:  */
        unsigned int  reserved_5                  : 1;  /* bit[16]:  */
        unsigned int  gt_clk_autodiv_aonoc_fll    : 1;  /* bit[17]:  */
        unsigned int  gt_timerclk_refh            : 1;  /* bit[18]:  */
        unsigned int  gt_clk_pll_fsm              : 1;  /* bit[19]:  */
        unsigned int  gt_clk_fll_src_peri         : 1;  /* bit[20]:  */
        unsigned int  reserved_6                  : 1;  /* bit[21]:  */
        unsigned int  reserved_7                  : 1;  /* bit[22]:  */
        unsigned int  reserved_8                  : 1;  /* bit[23]:  */
        unsigned int  reserved_9                  : 1;  /* bit[24]:  */
        unsigned int  reserved_10                 : 1;  /* bit[25]:  */
        unsigned int  reserved_11                 : 1;  /* bit[26]:  */
        unsigned int  reserved_12                 : 1;  /* bit[27]:  */
        unsigned int  reserved_13                 : 1;  /* bit[28]:  */
        unsigned int  reserved_14                 : 1;  /* bit[29]:  */
        unsigned int  reserved_15                 : 1;  /* bit[30]:  */
        unsigned int  reserved_16                 : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCPERCLKEN4_SEC_UNION;
#endif
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_atclk_to_ao_START               (0)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_atclk_to_ao_END                 (0)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hieps_tee_START             (3)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hieps_tee_END               (3)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hieps_tcxo_START            (5)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hieps_tcxo_END              (5)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hisee_pll_START             (7)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hisee_pll_END               (7)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hieps_ree_START             (8)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hieps_ree_END               (8)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hisee_ref_START             (10)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_hisee_ref_END               (10)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_qic_spidma_START            (11)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_qic_spidma_END              (11)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_spll_sscg_START             (12)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_spll_sscg_END               (12)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_aobus_START                 (13)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_aobus_END                   (13)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_aonoc2mspcbus_START         (14)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_aonoc2mspcbus_END           (14)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_noc_aobus2sysbusddrc_START  (15)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_noc_aobus2sysbusddrc_END    (15)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_autodiv_aonoc_fll_START     (17)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_autodiv_aonoc_fll_END       (17)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_timerclk_refh_START             (18)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_timerclk_refh_END               (18)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_pll_fsm_START               (19)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_pll_fsm_END                 (19)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_fll_src_peri_START          (20)
#define SOC_AO_CRG_SCPERCLKEN4_SEC_gt_clk_fll_src_peri_END            (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV0_SEC_UNION
 struct description   : SCCLKDIV0_SEC Register structure definition
                        Address Offset:0x9E0 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_aobus            : 4;  /* bit[0-3]  :  */
        unsigned int  div_iomcu_fll        : 2;  /* bit[4-5]  : iomcu主时钟的fll通路分频比：
                                                                2'd0: 1分频
                                                                …… 
                                                                2'd3: 4分频 */
        unsigned int  sc_gt_clk_sci        : 1;  /* bit[6]    :  */
        unsigned int  timer_secu_en        : 1;  /* bit[7]    : AO区TIMER1安全控制，使能时TIMER1的时钟有效，复位撤离，timer_clk_en2和timer_clk_en3为32KHZ时钟
                                                                0：安全控制不使能，timer2,3可选择32K和TCXO的4分频时钟；
                                                                1：安全控制使能，timer2,3都只使用32K时钟；并且此时timer2,3无法修改门控使能。 */
        unsigned int  sc_gt_clk_iomcu_fll  : 1;  /* bit[8]    :  */
        unsigned int  sc_gt_clk_iomcu_pll  : 1;  /* bit[9]    :  */
        unsigned int  iomcu_fll_gt_mode    : 1;  /* bit[10]   : crg内部iomcu门控投票方式：
                                                                1'b0:ulppll投票，睡眠模式
                                                                1'b1:ulppll常开，只控制门控模式 */
        unsigned int  ulppll_sleep_en_mode : 1;  /* bit[11]   : ulppll 睡眠模式选择：
                                                                1'b0:使用软件接口
                                                                1'b1:使用硬件投票 */
        unsigned int  ulppll_sleep_en_soft : 1;  /* bit[12]   : ulppll 睡眠模式控制软件配置接口：
                                                                1'b0:正常工作
                                                                1'b1:睡眠模式 */
        unsigned int  div_sci              : 2;  /* bit[13-14]:  */
        unsigned int  sel_sci              : 1;  /* bit[15]   : SIM卡时钟源选择：
                                                                0：clk_aobus_ini时钟源；1：clkin_sys */
        unsigned int  bitmasken            : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV0_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV0_SEC_div_aobus_START             (0)
#define SOC_AO_CRG_SCCLKDIV0_SEC_div_aobus_END               (3)
#define SOC_AO_CRG_SCCLKDIV0_SEC_div_iomcu_fll_START         (4)
#define SOC_AO_CRG_SCCLKDIV0_SEC_div_iomcu_fll_END           (5)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sc_gt_clk_sci_START         (6)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sc_gt_clk_sci_END           (6)
#define SOC_AO_CRG_SCCLKDIV0_SEC_timer_secu_en_START         (7)
#define SOC_AO_CRG_SCCLKDIV0_SEC_timer_secu_en_END           (7)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sc_gt_clk_iomcu_fll_START   (8)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sc_gt_clk_iomcu_fll_END     (8)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sc_gt_clk_iomcu_pll_START   (9)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sc_gt_clk_iomcu_pll_END     (9)
#define SOC_AO_CRG_SCCLKDIV0_SEC_iomcu_fll_gt_mode_START     (10)
#define SOC_AO_CRG_SCCLKDIV0_SEC_iomcu_fll_gt_mode_END       (10)
#define SOC_AO_CRG_SCCLKDIV0_SEC_ulppll_sleep_en_mode_START  (11)
#define SOC_AO_CRG_SCCLKDIV0_SEC_ulppll_sleep_en_mode_END    (11)
#define SOC_AO_CRG_SCCLKDIV0_SEC_ulppll_sleep_en_soft_START  (12)
#define SOC_AO_CRG_SCCLKDIV0_SEC_ulppll_sleep_en_soft_END    (12)
#define SOC_AO_CRG_SCCLKDIV0_SEC_div_sci_START               (13)
#define SOC_AO_CRG_SCCLKDIV0_SEC_div_sci_END                 (14)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sel_sci_START               (15)
#define SOC_AO_CRG_SCCLKDIV0_SEC_sel_sci_END                 (15)
#define SOC_AO_CRG_SCCLKDIV0_SEC_bitmasken_START             (16)
#define SOC_AO_CRG_SCCLKDIV0_SEC_bitmasken_END               (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV1_SEC_UNION
 struct description   : SCCLKDIV1_SEC Register structure definition
                        Address Offset:0x9E4 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0          : 4;  /* bit[0-3]  :  */
        unsigned int  reserved_1          : 1;  /* bit[4]    :  */
        unsigned int  div_hisee_pll       : 6;  /* bit[5-10] : hisee外设时钟分频比选择：
                                                               6'h0:1；
                                                               6'h1:2；
                                                               ...
                                                               6'h3e:63；
                                                               6'h3F:64； */
        unsigned int  sc_gt_clk_hisee_pll : 1;  /* bit[11]   : hisee div 前门控：
                                                               0：时钟关闭；
                                                               1：时钟打开。 */
        unsigned int  reserved_2          : 4;  /* bit[12-15]:  */
        unsigned int  bitmasken           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV1_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV1_SEC_div_hisee_pll_START        (5)
#define SOC_AO_CRG_SCCLKDIV1_SEC_div_hisee_pll_END          (10)
#define SOC_AO_CRG_SCCLKDIV1_SEC_sc_gt_clk_hisee_pll_START  (11)
#define SOC_AO_CRG_SCCLKDIV1_SEC_sc_gt_clk_hisee_pll_END    (11)
#define SOC_AO_CRG_SCCLKDIV1_SEC_bitmasken_START            (16)
#define SOC_AO_CRG_SCCLKDIV1_SEC_bitmasken_END              (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV2_SEC_UNION
 struct description   : SCCLKDIV2_SEC Register structure definition
                        Address Offset:0x9E8 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_hieps_tee : 1;  /* bit[0]    : 分频前门控控制：
                                                               1：时钟开启； 0：时钟关闭 */
        unsigned int  div_hieps_tee       : 6;  /* bit[1-6]  : HIEPS TEE通路分频比。
                                                               6'h0:1；
                                                               6'h1:2；
                                                               6'h2:3；
                                                               …
                                                               6'h3E:63；
                                                               6'h3F:64； */
        unsigned int  reserved_0          : 1;  /* bit[7]    :  */
        unsigned int  reserved_1          : 6;  /* bit[8-13] :  */
        unsigned int  div_aonoc_fll       : 2;  /* bit[14-15]: aonoc的时钟分频比：
                                                               2'd0: 1分频
                                                               …… 
                                                               2'd3: 4分频 */
        unsigned int  bitmasken           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV2_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV2_SEC_sc_gt_clk_hieps_tee_START  (0)
#define SOC_AO_CRG_SCCLKDIV2_SEC_sc_gt_clk_hieps_tee_END    (0)
#define SOC_AO_CRG_SCCLKDIV2_SEC_div_hieps_tee_START        (1)
#define SOC_AO_CRG_SCCLKDIV2_SEC_div_hieps_tee_END          (6)
#define SOC_AO_CRG_SCCLKDIV2_SEC_div_aonoc_fll_START        (14)
#define SOC_AO_CRG_SCCLKDIV2_SEC_div_aonoc_fll_END          (15)
#define SOC_AO_CRG_SCCLKDIV2_SEC_bitmasken_START            (16)
#define SOC_AO_CRG_SCCLKDIV2_SEC_bitmasken_END              (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV3_SEC_UNION
 struct description   : SCCLKDIV3_SEC Register structure definition
                        Address Offset:0x9EC Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 5;  /* bit[0-4]  :  */
        unsigned int  reserved_1      : 1;  /* bit[5]    :  */
        unsigned int  reserved_2      : 4;  /* bit[6-9]  :  */
        unsigned int  sc_gt_clk_aobus : 1;  /* bit[10]   :  */
        unsigned int  reserved_3      : 1;  /* bit[11]   :  */
        unsigned int  sc_gt_aonoc_fll : 1;  /* bit[12]   :  */
        unsigned int  reserved_4      : 1;  /* bit[13]   :  */
        unsigned int  sel_clk_aonoc   : 2;  /* bit[14-15]: aonoc_bus clk选择pll(只能LPM3配置)
                                                           00：TCXO；01：PPLL0；10：DDRPLL
                                                           11: ULPPLL */
        unsigned int  bitmasken       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV3_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV3_SEC_sc_gt_clk_aobus_START  (10)
#define SOC_AO_CRG_SCCLKDIV3_SEC_sc_gt_clk_aobus_END    (10)
#define SOC_AO_CRG_SCCLKDIV3_SEC_sc_gt_aonoc_fll_START  (12)
#define SOC_AO_CRG_SCCLKDIV3_SEC_sc_gt_aonoc_fll_END    (12)
#define SOC_AO_CRG_SCCLKDIV3_SEC_sel_clk_aonoc_START    (14)
#define SOC_AO_CRG_SCCLKDIV3_SEC_sel_clk_aonoc_END      (15)
#define SOC_AO_CRG_SCCLKDIV3_SEC_bitmasken_START        (16)
#define SOC_AO_CRG_SCCLKDIV3_SEC_bitmasken_END          (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV4_SEC_UNION
 struct description   : SCCLKDIV4_SEC Register structure definition
                        Address Offset:0x9F0 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aobusnoc_auto_waitcfg_out : 10; /* bit[0-9]  : SECENG PLL通路分频比。
                                                                     5'h0:1；
                                                                     5'h1:2；
                                                                     5'h2:3；
                                                                     …
                                                                     5'h1E:31；
                                                                     5'h1F:32； */
        unsigned int  aobusnoc_div_auto_cfg     : 6;  /* bit[10-15]: 分频前门控控制：
                                                                     1：时钟开启； 0：时钟关闭 */
        unsigned int  bitmasken                 : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV4_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV4_SEC_aobusnoc_auto_waitcfg_out_START  (0)
#define SOC_AO_CRG_SCCLKDIV4_SEC_aobusnoc_auto_waitcfg_out_END    (9)
#define SOC_AO_CRG_SCCLKDIV4_SEC_aobusnoc_div_auto_cfg_START      (10)
#define SOC_AO_CRG_SCCLKDIV4_SEC_aobusnoc_div_auto_cfg_END        (15)
#define SOC_AO_CRG_SCCLKDIV4_SEC_bitmasken_START                  (16)
#define SOC_AO_CRG_SCCLKDIV4_SEC_bitmasken_END                    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV5_SEC_UNION
 struct description   : SCCLKDIV5_SEC Register structure definition
                        Address Offset:0x9F4 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aobusnoc_div_auto_reduce_bypass : 1;  /* bit[0]    :  */
        unsigned int  aobusnoc_auto_waitcfg_in        : 10; /* bit[1-10] :  */
        unsigned int  aobus_clk_sw_req_cfg            : 2;  /* bit[11-12]:  */
        unsigned int  sc_gt_clk_aobus_fll_bypass      : 1;  /* bit[13]   : clk_aobus时钟fll分支DIV前级门控：
                                                                           0：关；1：开 */
        unsigned int  reserved                        : 2;  /* bit[14-15]:  */
        unsigned int  bitmasken                       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV5_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV5_SEC_aobusnoc_div_auto_reduce_bypass_START  (0)
#define SOC_AO_CRG_SCCLKDIV5_SEC_aobusnoc_div_auto_reduce_bypass_END    (0)
#define SOC_AO_CRG_SCCLKDIV5_SEC_aobusnoc_auto_waitcfg_in_START         (1)
#define SOC_AO_CRG_SCCLKDIV5_SEC_aobusnoc_auto_waitcfg_in_END           (10)
#define SOC_AO_CRG_SCCLKDIV5_SEC_aobus_clk_sw_req_cfg_START             (11)
#define SOC_AO_CRG_SCCLKDIV5_SEC_aobus_clk_sw_req_cfg_END               (12)
#define SOC_AO_CRG_SCCLKDIV5_SEC_sc_gt_clk_aobus_fll_bypass_START       (13)
#define SOC_AO_CRG_SCCLKDIV5_SEC_sc_gt_clk_aobus_fll_bypass_END         (13)
#define SOC_AO_CRG_SCCLKDIV5_SEC_bitmasken_START                        (16)
#define SOC_AO_CRG_SCCLKDIV5_SEC_bitmasken_END                          (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV6_SEC_UNION
 struct description   : SCCLKDIV6_SEC Register structure definition
                        Address Offset:0x9F8 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved               : 4;  /* bit[0-3]  :  */
        unsigned int  sel_noc_timeout_extref : 1;  /* bit[4]    : noc timeout采样源选择： 0：32K 1:19.2M */
        unsigned int  div_noc_timeout_extref : 6;  /* bit[5-10] : noc timeout采样精度的分频比：
                                                                  6'd0: 1分频
                                                                  6'd1: 2分频
                                                                  ……
                                                                  6'd63:64分频 */
        unsigned int  div_aobus_fll          : 5;  /* bit[11-15]:  */
        unsigned int  bitmasken              : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV6_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV6_SEC_sel_noc_timeout_extref_START  (4)
#define SOC_AO_CRG_SCCLKDIV6_SEC_sel_noc_timeout_extref_END    (4)
#define SOC_AO_CRG_SCCLKDIV6_SEC_div_noc_timeout_extref_START  (5)
#define SOC_AO_CRG_SCCLKDIV6_SEC_div_noc_timeout_extref_END    (10)
#define SOC_AO_CRG_SCCLKDIV6_SEC_div_aobus_fll_START           (11)
#define SOC_AO_CRG_SCCLKDIV6_SEC_div_aobus_fll_END             (15)
#define SOC_AO_CRG_SCCLKDIV6_SEC_bitmasken_START               (16)
#define SOC_AO_CRG_SCCLKDIV6_SEC_bitmasken_END                 (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV7_SEC_UNION
 struct description   : SCCLKDIV7_SEC Register structure definition
                        Address Offset:0x9FC Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aupll_en_vote     : 5;  /* bit[0-4]  : DP AUDIO 专用PLL使能投票：
                                                             投1使能，投0关闭； */
        unsigned int  aupll_bypass_vote : 5;  /* bit[5-9]  : DP AUDIO 专用PLLbypass投票：
                                                             投1bypass，投0不bypass； */
        unsigned int  aupll_gt_vote     : 5;  /* bit[10-14]: DP AUDIO 专用PLL输出门控投票：
                                                             投1开门控，投0关门控，PLL不输出； */
        unsigned int  reserved          : 1;  /* bit[15]   :  */
        unsigned int  bitmasken         : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV7_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV7_SEC_aupll_en_vote_START      (0)
#define SOC_AO_CRG_SCCLKDIV7_SEC_aupll_en_vote_END        (4)
#define SOC_AO_CRG_SCCLKDIV7_SEC_aupll_bypass_vote_START  (5)
#define SOC_AO_CRG_SCCLKDIV7_SEC_aupll_bypass_vote_END    (9)
#define SOC_AO_CRG_SCCLKDIV7_SEC_aupll_gt_vote_START      (10)
#define SOC_AO_CRG_SCCLKDIV7_SEC_aupll_gt_vote_END        (14)
#define SOC_AO_CRG_SCCLKDIV7_SEC_bitmasken_START          (16)
#define SOC_AO_CRG_SCCLKDIV7_SEC_bitmasken_END            (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV8_SEC_UNION
 struct description   : SCCLKDIV8_SEC Register structure definition
                        Address Offset:0xA00 Initial:0x0000 Width:32
 register description : 时钟分频比配置
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
        unsigned int  bitmasken : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV8_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV8_SEC_bitmasken_START  (16)
#define SOC_AO_CRG_SCCLKDIV8_SEC_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV9_SEC_UNION
 struct description   : SCCLKDIV9_SEC Register structure definition
                        Address Offset:0xA04 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_clk_pll_fsm   : 6;  /* bit[0-5]  : PLL FSM工作时钟分频比选择：
                                                             6'h0:1；
                                                             6'h1:2；
                                                             …
                                                             6'h3F:64； */
        unsigned int  sc_gt_clk_pll_fsm : 1;  /* bit[6]    :  */
        unsigned int  div_clk_ao_332m   : 3;  /* bit[7-9]  :  */
        unsigned int  ulppll_lp_gt_en   : 1;  /* bit[10]   : 1'b0: bypass Suspend only ULPPLL门控投票关闭LPMCU投票硬件控制
                                                             1'b1: 使能Suspend only ULPPLL门控投票关闭LPMCU投票硬件控制 */
        unsigned int  reserved_0        : 3;  /* bit[11-13]:  */
        unsigned int  reserved_1        : 1;  /* bit[14]   :  */
        unsigned int  reserved_2        : 1;  /* bit[15]   :  */
        unsigned int  bitmasken         : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV9_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV9_SEC_div_clk_pll_fsm_START    (0)
#define SOC_AO_CRG_SCCLKDIV9_SEC_div_clk_pll_fsm_END      (5)
#define SOC_AO_CRG_SCCLKDIV9_SEC_sc_gt_clk_pll_fsm_START  (6)
#define SOC_AO_CRG_SCCLKDIV9_SEC_sc_gt_clk_pll_fsm_END    (6)
#define SOC_AO_CRG_SCCLKDIV9_SEC_div_clk_ao_332m_START    (7)
#define SOC_AO_CRG_SCCLKDIV9_SEC_div_clk_ao_332m_END      (9)
#define SOC_AO_CRG_SCCLKDIV9_SEC_ulppll_lp_gt_en_START    (10)
#define SOC_AO_CRG_SCCLKDIV9_SEC_ulppll_lp_gt_en_END      (10)
#define SOC_AO_CRG_SCCLKDIV9_SEC_bitmasken_START          (16)
#define SOC_AO_CRG_SCCLKDIV9_SEC_bitmasken_END            (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV10_SEC_UNION
 struct description   : SCCLKDIV10_SEC Register structure definition
                        Address Offset:0xA08 Initial:0x0000 Width:32
 register description : 时钟分频比配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_timer               : 6;  /* bit[0-5]  : AO TIMER工作时钟分频比选择：
                                                                   6'h0:1；
                                                                   6'h1:2；
                                                                   …
                                                                   6'h3F:64； */
        unsigned int  sc_gt_clk_timer         : 1;  /* bit[6]    :  */
        unsigned int  sc_sel_clk_timer        : 1;  /* bit[7]    : timer时钟源选择信号（切换过程中请确保ULPPLL已经使能，且两侧都有时钟）：
                                                                   1'b0: 32k时钟源
                                                                   1'b1: ULPPLL时钟源 */
        unsigned int  sc_gt_clk_sys_ulppll_gt : 1;  /* bit[8]    :  */
        unsigned int  sel_sys_ulppll          : 1;  /* bit[9]    : CLK_SYS_ULPPLL时钟源选择信号：
                                                                   1'b0: 19.2M
                                                                   1'b1: ULPPLL时钟源 */
        unsigned int  div_sys_ulppll          : 6;  /* bit[10-15]: CLK_SYS_ULPPLL FLL分支工作时钟分频比选择：
                                                                   6'h0:1；
                                                                   6'h1:2；
                                                                   …
                                                                   6'h3F:64； */
        unsigned int  bitmasken               : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV10_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV10_SEC_div_timer_START                (0)
#define SOC_AO_CRG_SCCLKDIV10_SEC_div_timer_END                  (5)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sc_gt_clk_timer_START          (6)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sc_gt_clk_timer_END            (6)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sc_sel_clk_timer_START         (7)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sc_sel_clk_timer_END           (7)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sc_gt_clk_sys_ulppll_gt_START  (8)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sc_gt_clk_sys_ulppll_gt_END    (8)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sel_sys_ulppll_START           (9)
#define SOC_AO_CRG_SCCLKDIV10_SEC_sel_sys_ulppll_END             (9)
#define SOC_AO_CRG_SCCLKDIV10_SEC_div_sys_ulppll_START           (10)
#define SOC_AO_CRG_SCCLKDIV10_SEC_div_sys_ulppll_END             (15)
#define SOC_AO_CRG_SCCLKDIV10_SEC_bitmasken_START                (16)
#define SOC_AO_CRG_SCCLKDIV10_SEC_bitmasken_END                  (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV11_SEC_UNION
 struct description   : SCCLKDIV11_SEC Register structure definition
                        Address Offset:0xA0C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_hieps_ree       : 6;  /* bit[0-5]  : HIEPS TEE通路分频比。
                                                               6'h0:1；
                                                               6'h1:2；
                                                               6'h2:3；
                                                               …
                                                               6'h3E:63；
                                                               6'h3F:64； */
        unsigned int  sc_gt_clk_hieps_ree : 1;  /* bit[6]    : 分频前门控控制：
                                                               1：时钟开启； 0：时钟关闭 */
        unsigned int  reserved_0          : 6;  /* bit[7-12] :  */
        unsigned int  reserved_1          : 3;  /* bit[13-15]:  */
        unsigned int  bitmasken           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV11_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV11_SEC_div_hieps_ree_START        (0)
#define SOC_AO_CRG_SCCLKDIV11_SEC_div_hieps_ree_END          (5)
#define SOC_AO_CRG_SCCLKDIV11_SEC_sc_gt_clk_hieps_ree_START  (6)
#define SOC_AO_CRG_SCCLKDIV11_SEC_sc_gt_clk_hieps_ree_END    (6)
#define SOC_AO_CRG_SCCLKDIV11_SEC_bitmasken_START            (16)
#define SOC_AO_CRG_SCCLKDIV11_SEC_bitmasken_END              (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCCLKDIV12_SEC_UNION
 struct description   : SCCLKDIV12_SEC Register structure definition
                        Address Offset:0xA1C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_clk_pll_fsm_fll   : 8;  /* bit[0-7]  : 默认215分频，1.2MHz（258/215) */
        unsigned int  sc_gt_clk_pll_fsm_fll : 1;  /* bit[8]    : 分频前门控控制 */
        unsigned int  sel_clk_pll_fsm       : 1;  /* bit[9]    : pll_fsm通路选择：
                                                                 1'b0:19.2MHz通路；默认
                                                                 1'b1:ulppll通路； */
        unsigned int  reserved              : 6;  /* bit[10-15]:  */
        unsigned int  bitmasken             : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_SCCLKDIV12_SEC_UNION;
#endif
#define SOC_AO_CRG_SCCLKDIV12_SEC_div_clk_pll_fsm_fll_START    (0)
#define SOC_AO_CRG_SCCLKDIV12_SEC_div_clk_pll_fsm_fll_END      (7)
#define SOC_AO_CRG_SCCLKDIV12_SEC_sc_gt_clk_pll_fsm_fll_START  (8)
#define SOC_AO_CRG_SCCLKDIV12_SEC_sc_gt_clk_pll_fsm_fll_END    (8)
#define SOC_AO_CRG_SCCLKDIV12_SEC_sel_clk_pll_fsm_START        (9)
#define SOC_AO_CRG_SCCLKDIV12_SEC_sel_clk_pll_fsm_END          (9)
#define SOC_AO_CRG_SCCLKDIV12_SEC_bitmasken_START              (16)
#define SOC_AO_CRG_SCCLKDIV12_SEC_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_UNION
 struct description   : SC_IPCLKRST_BYPASS0_SEC Register structure definition
                        Address Offset:0xA84 Initial:0x0 Width:32
 register description : IP防总线挂死控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  rtc_clkrst_bypass            : 1;  /* bit[0] : 总线防挂死功能bypass
                                                                     0:使能复位&时钟门控防总线挂死功能
                                                                     1:BYPASS复位&时钟门控防总线挂死功能。 */
        unsigned int  rtc1_clkrst_bypass           : 1;  /* bit[1] :  */
        unsigned int  timer0_clkrst_bypass         : 1;  /* bit[2] :  */
        unsigned int  timer1_clkrst_bypass         : 1;  /* bit[3] :  */
        unsigned int  timer2_clkrst_bypass         : 1;  /* bit[4] :  */
        unsigned int  timer3_clkrst_bypass         : 1;  /* bit[5] :  */
        unsigned int  reserved_0                   : 1;  /* bit[6] :  */
        unsigned int  reserved_1                   : 1;  /* bit[7] :  */
        unsigned int  reserved_2                   : 1;  /* bit[8] :  */
        unsigned int  reserved_3                   : 1;  /* bit[9] :  */
        unsigned int  reserved_4                   : 1;  /* bit[10]:  */
        unsigned int  reserved_5                   : 1;  /* bit[11]:  */
        unsigned int  ioc_clkrst_bypass            : 1;  /* bit[12]:  */
        unsigned int  syscnt_clkrst_bypass         : 1;  /* bit[13]:  */
        unsigned int  sci0_clkrst_bypass           : 1;  /* bit[14]:  */
        unsigned int  sci1_clkrst_bypass           : 1;  /* bit[15]:  */
        unsigned int  bbpdrx_clkrst_bypass         : 1;  /* bit[16]:  */
        unsigned int  reserved_6                   : 1;  /* bit[17]:  */
        unsigned int  timer4_clkrst_bypass         : 1;  /* bit[18]:  */
        unsigned int  timer5_clkrst_bypass         : 1;  /* bit[19]:  */
        unsigned int  timer6_clkrst_bypass         : 1;  /* bit[20]:  */
        unsigned int  timer7_clkrst_bypass         : 1;  /* bit[21]:  */
        unsigned int  timer8_clkrst_bypass         : 1;  /* bit[22]:  */
        unsigned int  reserved_7                   : 1;  /* bit[23]:  */
        unsigned int  reserved_8                   : 1;  /* bit[24]:  */
        unsigned int  reserved_9                   : 1;  /* bit[25]:  */
        unsigned int  gpio1_se_clkrst_bypass       : 1;  /* bit[26]:  */
        unsigned int  reserved_10                  : 1;  /* bit[27]:  */
        unsigned int  em_sync_top_clkrst_bypass    : 1;  /* bit[28]:  */
        unsigned int  ao_loadmonitor_clkrst_bypass : 1;  /* bit[29]:  */
        unsigned int  spmi_clkrst_bypass           : 1;  /* bit[30]:  */
        unsigned int  ao_tzpc_clkrst_bypass        : 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_UNION;
#endif
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_rtc_clkrst_bypass_START             (0)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_rtc_clkrst_bypass_END               (0)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_rtc1_clkrst_bypass_START            (1)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_rtc1_clkrst_bypass_END              (1)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer0_clkrst_bypass_START          (2)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer0_clkrst_bypass_END            (2)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer1_clkrst_bypass_START          (3)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer1_clkrst_bypass_END            (3)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer2_clkrst_bypass_START          (4)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer2_clkrst_bypass_END            (4)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer3_clkrst_bypass_START          (5)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer3_clkrst_bypass_END            (5)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ioc_clkrst_bypass_START             (12)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ioc_clkrst_bypass_END               (12)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_syscnt_clkrst_bypass_START          (13)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_syscnt_clkrst_bypass_END            (13)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_sci0_clkrst_bypass_START            (14)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_sci0_clkrst_bypass_END              (14)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_sci1_clkrst_bypass_START            (15)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_sci1_clkrst_bypass_END              (15)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_bbpdrx_clkrst_bypass_START          (16)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_bbpdrx_clkrst_bypass_END            (16)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer4_clkrst_bypass_START          (18)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer4_clkrst_bypass_END            (18)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer5_clkrst_bypass_START          (19)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer5_clkrst_bypass_END            (19)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer6_clkrst_bypass_START          (20)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer6_clkrst_bypass_END            (20)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer7_clkrst_bypass_START          (21)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer7_clkrst_bypass_END            (21)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer8_clkrst_bypass_START          (22)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_timer8_clkrst_bypass_END            (22)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_gpio1_se_clkrst_bypass_START        (26)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_gpio1_se_clkrst_bypass_END          (26)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_em_sync_top_clkrst_bypass_START     (28)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_em_sync_top_clkrst_bypass_END       (28)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ao_loadmonitor_clkrst_bypass_START  (29)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ao_loadmonitor_clkrst_bypass_END    (29)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_spmi_clkrst_bypass_START            (30)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_spmi_clkrst_bypass_END              (30)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ao_tzpc_clkrst_bypass_START         (31)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS0_SEC_ao_tzpc_clkrst_bypass_END           (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_UNION
 struct description   : SC_IPCLKRST_BYPASS1_SEC Register structure definition
                        Address Offset:0xA88 Initial:0x0 Width:32
 register description : IP防总线挂死控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ao_ipc_clkrst_bypass    : 1;  /* bit[0] : 总线防挂死功能bypass
                                                                0:使能复位&时钟门控防总线挂死功能
                                                                1:BYPASS复位&时钟门控防总线挂死功能。 */
        unsigned int  ao_ipc_ns_clkrst_bypass : 1;  /* bit[1] :  */
        unsigned int  reserved_0              : 1;  /* bit[2] :  */
        unsigned int  reserved_1              : 1;  /* bit[3] :  */
        unsigned int  spi3_clkrst_bypass      : 1;  /* bit[4] :  */
        unsigned int  reserved_2              : 1;  /* bit[5] :  */
        unsigned int  reserved_3              : 1;  /* bit[6] :  */
        unsigned int  ao_wd_clkrst_bypass     : 1;  /* bit[7] :  */
        unsigned int  reserved_4              : 1;  /* bit[8] :  */
        unsigned int  reserved_5              : 1;  /* bit[9] :  */
        unsigned int  reserved_6              : 1;  /* bit[10]:  */
        unsigned int  ao_tcp_clkrst_bypass    : 1;  /* bit[11]:  */
        unsigned int  timer17_clkrst_bypass   : 1;  /* bit[12]:  */
        unsigned int  timer16_clkrst_bypass   : 1;  /* bit[13]:  */
        unsigned int  timer15_clkrst_bypass   : 1;  /* bit[14]:  */
        unsigned int  timer14_clkrst_bypass   : 1;  /* bit[15]:  */
        unsigned int  timer13_clkrst_bypass   : 1;  /* bit[16]:  */
        unsigned int  spi5_clkrst_bypass      : 1;  /* bit[17]:  */
        unsigned int  ao_wd1_clkrst_bypass    : 1;  /* bit[18]:  */
        unsigned int  reserved_7              : 1;  /* bit[19]:  */
        unsigned int  blpwm2_clkrst_bypass    : 1;  /* bit[20]:  */
        unsigned int  intr_hub_clkrst_bypass  : 1;  /* bit[21]:  */
        unsigned int  reserved_8              : 1;  /* bit[22]:  */
        unsigned int  reserved_9              : 1;  /* bit[23]:  */
        unsigned int  reserved_10             : 1;  /* bit[24]:  */
        unsigned int  efusec_clkrst_bypass    : 1;  /* bit[25]:  */
        unsigned int  efusec2_clkrst_bypass   : 1;  /* bit[26]:  */
        unsigned int  hickm_clkrst_bypass     : 1;  /* bit[27]:  */
        unsigned int  reserved_11             : 1;  /* bit[28]:  */
        unsigned int  reserved_12             : 1;  /* bit[29]:  */
        unsigned int  reserved_13             : 1;  /* bit[30]:  */
        unsigned int  reserved_14             : 1;  /* bit[31]:  */
    } reg;
} SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_UNION;
#endif
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_ipc_clkrst_bypass_START     (0)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_ipc_clkrst_bypass_END       (0)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_ipc_ns_clkrst_bypass_START  (1)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_ipc_ns_clkrst_bypass_END    (1)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_spi3_clkrst_bypass_START       (4)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_spi3_clkrst_bypass_END         (4)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_wd_clkrst_bypass_START      (7)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_wd_clkrst_bypass_END        (7)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_tcp_clkrst_bypass_START     (11)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_tcp_clkrst_bypass_END       (11)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer17_clkrst_bypass_START    (12)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer17_clkrst_bypass_END      (12)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer16_clkrst_bypass_START    (13)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer16_clkrst_bypass_END      (13)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer15_clkrst_bypass_START    (14)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer15_clkrst_bypass_END      (14)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer14_clkrst_bypass_START    (15)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer14_clkrst_bypass_END      (15)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer13_clkrst_bypass_START    (16)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_timer13_clkrst_bypass_END      (16)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_spi5_clkrst_bypass_START       (17)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_spi5_clkrst_bypass_END         (17)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_wd1_clkrst_bypass_START     (18)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_ao_wd1_clkrst_bypass_END       (18)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_blpwm2_clkrst_bypass_START     (20)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_blpwm2_clkrst_bypass_END       (20)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_intr_hub_clkrst_bypass_START   (21)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_intr_hub_clkrst_bypass_END     (21)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_efusec_clkrst_bypass_START     (25)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_efusec_clkrst_bypass_END       (25)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_efusec2_clkrst_bypass_START    (26)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_efusec2_clkrst_bypass_END      (26)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_hickm_clkrst_bypass_START      (27)
#define SOC_AO_CRG_SC_IPCLKRST_BYPASS1_SEC_hickm_clkrst_bypass_END        (27)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_CLK_STAT0_SEC_UNION
 struct description   : SC_CLK_STAT0_SEC Register structure definition
                        Address Offset:0xA8C Initial:0x00000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_hieps_tee     : 1;  /* bit[0]    :  */
        unsigned int  reserved_0           : 1;  /* bit[1]    :  */
        unsigned int  st_clk_hieps_tcxo    : 1;  /* bit[2]    :  */
        unsigned int  reserved_1           : 1;  /* bit[3]    :  */
        unsigned int  st_clk_hieps_ree     : 1;  /* bit[4]    :  */
        unsigned int  reserved_2           : 1;  /* bit[5]    :  */
        unsigned int  reserved_3           : 1;  /* bit[6]    :  */
        unsigned int  reserved_4           : 1;  /* bit[7]    :  */
        unsigned int  st_clk_hisee_pll     : 1;  /* bit[8]    :  */
        unsigned int  reserved_5           : 1;  /* bit[9]    :  */
        unsigned int  reserved_6           : 1;  /* bit[10]   :  */
        unsigned int  st_clk_hisee_ref     : 1;  /* bit[11]   :  */
        unsigned int  reserved_7           : 1;  /* bit[12]   :  */
        unsigned int  st_clk_aonoc2mspcbus : 1;  /* bit[13]   :  */
        unsigned int  reserved_8           : 18; /* bit[14-31]:  */
    } reg;
} SOC_AO_CRG_SC_CLK_STAT0_SEC_UNION;
#endif
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hieps_tee_START      (0)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hieps_tee_END        (0)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hieps_tcxo_START     (2)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hieps_tcxo_END       (2)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hieps_ree_START      (4)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hieps_ree_END        (4)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hisee_pll_START      (8)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hisee_pll_END        (8)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hisee_ref_START      (11)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_hisee_ref_END        (11)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_aonoc2mspcbus_START  (13)
#define SOC_AO_CRG_SC_CLK_STAT0_SEC_st_clk_aonoc2mspcbus_END    (13)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_CLK_STAT1_SEC_UNION
 struct description   : SC_CLK_STAT1_SEC Register structure definition
                        Address Offset:0xA90 Initial:0x000000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sw_ack_clk_hieps_tee_sw  : 2;  /* bit[0-1] : hieps_tee的时钟源状态
                                                                   2'b01: 38.4M
                                                                   2'b10: PLL时钟源 */
        unsigned int  swdone_clk_hieps_tee_div : 1;  /* bit[2]   :  */
        unsigned int  sw_ack_clk_hisee_sw      : 2;  /* bit[3-4] : hisee的时钟源状态
                                                                   2'b01: 38.4M
                                                                   2'b10: PLL时钟源 */
        unsigned int  sw_ack_clk_hieps_ree_sw  : 2;  /* bit[5-6] : hieps_ree的时钟源状态
                                                                   2'b01: 38.4M
                                                                   2'b10: PLL时钟源(改为19.2M时钟源) */
        unsigned int  swdone_clk_hieps_ree_div : 1;  /* bit[7]   :  */
        unsigned int  reserved_0               : 1;  /* bit[8]   :  */
        unsigned int  reserved_1               : 23; /* bit[9-31]:  */
    } reg;
} SOC_AO_CRG_SC_CLK_STAT1_SEC_UNION;
#endif
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_sw_ack_clk_hieps_tee_sw_START   (0)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_sw_ack_clk_hieps_tee_sw_END     (1)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_swdone_clk_hieps_tee_div_START  (2)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_swdone_clk_hieps_tee_div_END    (2)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_sw_ack_clk_hisee_sw_START       (3)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_sw_ack_clk_hisee_sw_END         (4)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_sw_ack_clk_hieps_ree_sw_START   (5)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_sw_ack_clk_hieps_ree_sw_END     (6)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_swdone_clk_hieps_ree_div_START  (7)
#define SOC_AO_CRG_SC_CLK_STAT1_SEC_swdone_clk_hieps_ree_div_END    (7)


/*****************************************************************************
 struct               : SOC_AO_CRG_PLL_FSM_CTRL0_SEC_UNION
 struct description   : PLL_FSM_CTRL0_SEC Register structure definition
                        Address Offset:0xA94 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aupll_fsm_en        : 1;  /* bit[0]    : PLL投票状态机使能控制：
                                                               0：关闭
                                                               1：使能 */
        unsigned int  aupll_vote_mode_sel : 1;  /* bit[1]    : PLL控制源选择：
                                                               0：选择原有寄存器投票方式
                                                               1：选择PLL投票状态机控制信号 */
        unsigned int  aupll_intr_clr      : 1;  /* bit[2]    : PLL中断清除控制，为0时不清除中断，为1时清除中断，清除中断后需要再次变回0后状态机才能正常工作 */
        unsigned int  spll_fsm_en         : 1;  /* bit[3]    : PLL投票状态机使能控制：
                                                               0：关闭
                                                               1：使能 */
        unsigned int  spll_vote_mode_sel  : 1;  /* bit[4]    : PLL控制源选择：
                                                               0：选择原有寄存器投票方式
                                                               1：选择PLL投票状态机控制信号 */
        unsigned int  spll_intr_clr       : 1;  /* bit[5]    : PLL中断清除控制，为0时不清除中断，为1时清除中断，清除中断后需要再次变回0后状态机才能正常工作 */
        unsigned int  reserved            : 10; /* bit[6-15] :  */
        unsigned int  bitmasken           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_AO_CRG_PLL_FSM_CTRL0_SEC_UNION;
#endif
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_aupll_fsm_en_START         (0)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_aupll_fsm_en_END           (0)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_aupll_vote_mode_sel_START  (1)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_aupll_vote_mode_sel_END    (1)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_aupll_intr_clr_START       (2)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_aupll_intr_clr_END         (2)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_spll_fsm_en_START          (3)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_spll_fsm_en_END            (3)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_spll_vote_mode_sel_START   (4)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_spll_vote_mode_sel_END     (4)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_spll_intr_clr_START        (5)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_spll_intr_clr_END          (5)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_bitmasken_START            (16)
#define SOC_AO_CRG_PLL_FSM_CTRL0_SEC_bitmasken_END              (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_PLL_FSM_CTRL1_SEC_UNION
 struct description   : PLL_FSM_CTRL1_SEC Register structure definition
                        Address Offset:0xA98 Initial:0x000 Width:32
 register description : 
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
        unsigned int  reserved        : 9;  /* bit[23-31]: 含义同bit0。 */
    } reg;
} SOC_AO_CRG_PLL_FSM_CTRL1_SEC_UNION;
#endif
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_lock_dbc_en_START  (0)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_lock_dbc_en_END    (0)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_fsm_intr_en_START  (1)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_fsm_intr_en_END    (1)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_wait_en_START      (2)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_wait_en_END        (2)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_timeout_en_START   (3)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_timeout_en_END     (3)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_intr_lmt_START     (4)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_intr_lmt_END       (6)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_wait_lmt_START     (7)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_wait_lmt_END       (14)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_timeout_lmt_START  (15)
#define SOC_AO_CRG_PLL_FSM_CTRL1_SEC_pll_timeout_lmt_END    (22)


/*****************************************************************************
 struct               : SOC_AO_CRG_PLL_FSM_STAT0_SEC_UNION
 struct description   : PLL_FSM_STAT0_SEC Register structure definition
                        Address Offset:0xA9C Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  aupll_fsm_intr  : 1;  /* bit[0]    : pll_fsm中断指示信号，高有效 */
        unsigned int  aupll_intr_cnt  : 3;  /* bit[1-3]  : pll_fsm触发中断的异常累计次数状态，用于debug */
        unsigned int  aupll_curr_stat : 4;  /* bit[4-7]  : pll_fsm状态指示信号，用于debug */
        unsigned int  spll_fsm_intr   : 1;  /* bit[8]    : pll_fsm中断指示信号，高有效 */
        unsigned int  spll_intr_cnt   : 3;  /* bit[9-11] : pll_fsm触发中断的异常累计次数状态，用于debug */
        unsigned int  spll_curr_stat  : 4;  /* bit[12-15]: pll_fsm状态指示信号，用于debug */
        unsigned int  reserved        : 16; /* bit[16-31]:  */
    } reg;
} SOC_AO_CRG_PLL_FSM_STAT0_SEC_UNION;
#endif
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_aupll_fsm_intr_START   (0)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_aupll_fsm_intr_END     (0)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_aupll_intr_cnt_START   (1)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_aupll_intr_cnt_END     (3)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_aupll_curr_stat_START  (4)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_aupll_curr_stat_END    (7)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_spll_fsm_intr_START    (8)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_spll_fsm_intr_END      (8)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_spll_intr_cnt_START    (9)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_spll_intr_cnt_END      (11)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_spll_curr_stat_START   (12)
#define SOC_AO_CRG_PLL_FSM_STAT0_SEC_spll_curr_stat_END     (15)


/*****************************************************************************
 struct               : SOC_AO_CRG_AO_AUTODIV0_UNION
 struct description   : AO_AUTODIV0 Register structure definition
                        Address Offset:0xB00 Initial:0x0 Width:32
 register description : 自动降频控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mspcqic_fw_tb_nopending_bp      : 1;  /* bit[0] : 是否加入自动降频判断条件：
                                                                        0：加入
                                                                        1：不加入 */
        unsigned int  mspcqic_qcp_tb_nopending_bp     : 1;  /* bit[1] :  */
        unsigned int  mspcqic_mspc_tb_nopending_bp    : 1;  /* bit[2] :  */
        unsigned int  mspcqic_mspc_ib_wr_nopending_bp : 1;  /* bit[3] :  */
        unsigned int  mspcqic_mspc_ib_rd_nopending_bp : 1;  /* bit[4] :  */
        unsigned int  aoqic_aotcp_busy_bp             : 1;  /* bit[5] :  */
        unsigned int  aoqic_aotcp_ib_wr_nopending_bp  : 1;  /* bit[6] :  */
        unsigned int  aoqic_aotcp_ib_rd_nopending_bp  : 1;  /* bit[7] :  */
        unsigned int  aoqic_qspi_idle_bp              : 1;  /* bit[8] :  */
        unsigned int  aoqic_qspi_tb_nopending_bp      : 1;  /* bit[9] :  */
        unsigned int  aoqic_qspi_ib_wr_nopending_bp   : 1;  /* bit[10]:  */
        unsigned int  aoqic_qspi_ib_rd_nopending_bp   : 1;  /* bit[11]:  */
        unsigned int  aoqic_fw_tb_nopending_bp        : 1;  /* bit[12]:  */
        unsigned int  aoqic_qcp_tb_nopending_bp       : 1;  /* bit[13]:  */
        unsigned int  aoqic_ao2asp_tb_nopending_bp    : 1;  /* bit[14]:  */
        unsigned int  aoqic_mad_tb_nopending_bp       : 1;  /* bit[15]:  */
        unsigned int  aoqic_aoapb_tb_nopending_bp     : 1;  /* bit[16]:  */
        unsigned int  aoqic_ao2sys_tb_nopending_bp    : 1;  /* bit[17]:  */
        unsigned int  aoqic_spidma_ib_wr_nopending_bp : 1;  /* bit[18]:  */
        unsigned int  aoqic_spidma_ib_rd_nopending_bp : 1;  /* bit[19]:  */
        unsigned int  aoqic_asp2ao_ib_wr_nopending_bp : 1;  /* bit[20]:  */
        unsigned int  aoqic_asp2ao_ib_rd_nopending_bp : 1;  /* bit[21]:  */
        unsigned int  aoqic_sys2ao_ib_wr_nopending_bp : 1;  /* bit[22]:  */
        unsigned int  aoqic_sys2ao_ib_rd_nopending_bp : 1;  /* bit[23]:  */
        unsigned int  iomcuqic_fw_tb_nopending_bp     : 1;  /* bit[24]:  */
        unsigned int  iomcuqic_qcp_tb_nopending_bp    : 1;  /* bit[25]:  */
        unsigned int  iomcuqic_hts_tb_nopending_bp    : 1;  /* bit[26]:  */
        unsigned int  iomcuqic_apbh_tb_nopending_bp   : 1;  /* bit[27]:  */
        unsigned int  iomcuqic_apbl_tb_nopending_bp   : 1;  /* bit[28]:  */
        unsigned int  iomcuqic_ahb_tb_nopending_bp    : 1;  /* bit[29]:  */
        unsigned int  iomcuqic_hts_ib_wr_nopending_bp : 1;  /* bit[30]:  */
        unsigned int  iomcuqic_hts_ib_rd_nopending_bp : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_AO_AUTODIV0_UNION;
#endif
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_fw_tb_nopending_bp_START       (0)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_fw_tb_nopending_bp_END         (0)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_qcp_tb_nopending_bp_START      (1)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_qcp_tb_nopending_bp_END        (1)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_mspc_tb_nopending_bp_START     (2)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_mspc_tb_nopending_bp_END       (2)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_mspc_ib_wr_nopending_bp_START  (3)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_mspc_ib_wr_nopending_bp_END    (3)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_mspc_ib_rd_nopending_bp_START  (4)
#define SOC_AO_CRG_AO_AUTODIV0_mspcqic_mspc_ib_rd_nopending_bp_END    (4)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aotcp_busy_bp_START              (5)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aotcp_busy_bp_END                (5)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aotcp_ib_wr_nopending_bp_START   (6)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aotcp_ib_wr_nopending_bp_END     (6)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aotcp_ib_rd_nopending_bp_START   (7)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aotcp_ib_rd_nopending_bp_END     (7)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_idle_bp_START               (8)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_idle_bp_END                 (8)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_tb_nopending_bp_START       (9)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_tb_nopending_bp_END         (9)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_ib_wr_nopending_bp_START    (10)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_ib_wr_nopending_bp_END      (10)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_ib_rd_nopending_bp_START    (11)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qspi_ib_rd_nopending_bp_END      (11)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_fw_tb_nopending_bp_START         (12)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_fw_tb_nopending_bp_END           (12)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qcp_tb_nopending_bp_START        (13)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_qcp_tb_nopending_bp_END          (13)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_ao2asp_tb_nopending_bp_START     (14)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_ao2asp_tb_nopending_bp_END       (14)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_mad_tb_nopending_bp_START        (15)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_mad_tb_nopending_bp_END          (15)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aoapb_tb_nopending_bp_START      (16)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_aoapb_tb_nopending_bp_END        (16)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_ao2sys_tb_nopending_bp_START     (17)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_ao2sys_tb_nopending_bp_END       (17)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_spidma_ib_wr_nopending_bp_START  (18)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_spidma_ib_wr_nopending_bp_END    (18)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_spidma_ib_rd_nopending_bp_START  (19)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_spidma_ib_rd_nopending_bp_END    (19)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_asp2ao_ib_wr_nopending_bp_START  (20)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_asp2ao_ib_wr_nopending_bp_END    (20)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_asp2ao_ib_rd_nopending_bp_START  (21)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_asp2ao_ib_rd_nopending_bp_END    (21)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_sys2ao_ib_wr_nopending_bp_START  (22)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_sys2ao_ib_wr_nopending_bp_END    (22)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_sys2ao_ib_rd_nopending_bp_START  (23)
#define SOC_AO_CRG_AO_AUTODIV0_aoqic_sys2ao_ib_rd_nopending_bp_END    (23)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_fw_tb_nopending_bp_START      (24)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_fw_tb_nopending_bp_END        (24)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_qcp_tb_nopending_bp_START     (25)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_qcp_tb_nopending_bp_END       (25)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_hts_tb_nopending_bp_START     (26)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_hts_tb_nopending_bp_END       (26)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_apbh_tb_nopending_bp_START    (27)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_apbh_tb_nopending_bp_END      (27)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_apbl_tb_nopending_bp_START    (28)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_apbl_tb_nopending_bp_END      (28)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_ahb_tb_nopending_bp_START     (29)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_ahb_tb_nopending_bp_END       (29)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_hts_ib_wr_nopending_bp_START  (30)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_hts_ib_wr_nopending_bp_END    (30)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_hts_ib_rd_nopending_bp_START  (31)
#define SOC_AO_CRG_AO_AUTODIV0_iomcuqic_hts_ib_rd_nopending_bp_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_AO_AUTODIV1_UNION
 struct description   : AO_AUTODIV1 Register structure definition
                        Address Offset:0xB04 Initial:0x0 Width:32
 register description : 自动降频控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  iomcuqic_dma_ib_wr_nopending_bp  : 1;  /* bit[0] : 是否加入自动降频判断条件：
                                                                         0：加入
                                                                         1：不加入 */
        unsigned int  iomcuqic_dma_ib_rd_nopending_bp  : 1;  /* bit[1] :  */
        unsigned int  iomcuqic_lx_ib_wr_nopending_bp   : 1;  /* bit[2] :  */
        unsigned int  iomcuqic_lx_ib_rd_nopending_bp   : 1;  /* bit[3] :  */
        unsigned int  iomcuqic_ahb_ib_wr_nopending_bp  : 1;  /* bit[4] :  */
        unsigned int  iomcuqic_ahb_ib_rd_nopending_bp  : 1;  /* bit[5] :  */
        unsigned int  aspqic_tb_nopending_bp           : 1;  /* bit[6] :  */
        unsigned int  reserved_0                       : 1;  /* bit[7] :  */
        unsigned int  reserved_1                       : 1;  /* bit[8] :  */
        unsigned int  reserved_2                       : 1;  /* bit[9] :  */
        unsigned int  reserved_3                       : 1;  /* bit[10]:  */
        unsigned int  reserved_4                       : 1;  /* bit[11]:  */
        unsigned int  reserved_5                       : 1;  /* bit[12]:  */
        unsigned int  reserved_6                       : 1;  /* bit[13]:  */
        unsigned int  reserved_7                       : 1;  /* bit[14]:  */
        unsigned int  reserved_8                       : 1;  /* bit[15]:  */
        unsigned int  reserved_9                       : 1;  /* bit[16]:  */
        unsigned int  reserved_10                      : 1;  /* bit[17]:  */
        unsigned int  reserved_11                      : 1;  /* bit[18]:  */
        unsigned int  reserved_12                      : 1;  /* bit[19]:  */
        unsigned int  reserved_13                      : 1;  /* bit[20]:  */
        unsigned int  reserved_14                      : 1;  /* bit[21]:  */
        unsigned int  reserved_15                      : 1;  /* bit[22]:  */
        unsigned int  reserved_16                      : 1;  /* bit[23]:  */
        unsigned int  reserved_17                      : 1;  /* bit[24]:  */
        unsigned int  reserved_18                      : 1;  /* bit[25]:  */
        unsigned int  reserved_19                      : 1;  /* bit[26]:  */
        unsigned int  reserved_20                      : 1;  /* bit[27]:  */
        unsigned int  reserved_21                      : 1;  /* bit[28]:  */
        unsigned int  reserved_22                      : 1;  /* bit[29]:  */
        unsigned int  reserved_23                      : 1;  /* bit[30]:  */
        unsigned int  aoqic2sys_ao2sys_tb_nopending_bp : 1;  /* bit[31]: 保留。 */
    } reg;
} SOC_AO_CRG_AO_AUTODIV1_UNION;
#endif
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_dma_ib_wr_nopending_bp_START   (0)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_dma_ib_wr_nopending_bp_END     (0)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_dma_ib_rd_nopending_bp_START   (1)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_dma_ib_rd_nopending_bp_END     (1)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_lx_ib_wr_nopending_bp_START    (2)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_lx_ib_wr_nopending_bp_END      (2)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_lx_ib_rd_nopending_bp_START    (3)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_lx_ib_rd_nopending_bp_END      (3)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_ahb_ib_wr_nopending_bp_START   (4)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_ahb_ib_wr_nopending_bp_END     (4)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_ahb_ib_rd_nopending_bp_START   (5)
#define SOC_AO_CRG_AO_AUTODIV1_iomcuqic_ahb_ib_rd_nopending_bp_END     (5)
#define SOC_AO_CRG_AO_AUTODIV1_aspqic_tb_nopending_bp_START            (6)
#define SOC_AO_CRG_AO_AUTODIV1_aspqic_tb_nopending_bp_END              (6)
#define SOC_AO_CRG_AO_AUTODIV1_aoqic2sys_ao2sys_tb_nopending_bp_START  (31)
#define SOC_AO_CRG_AO_AUTODIV1_aoqic2sys_ao2sys_tb_nopending_bp_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SC_DDRC_CTRL_UNION
 struct description   : SC_DDRC_CTRL Register structure definition
                        Address Offset:0xE38 Initial:0x00000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddrc_csysreq_cfg_sc2aocrg     : 4;  /* bit[0-3]  :  */
        unsigned int  ddrc_csysreq_sft_cfg_sc2aocrg : 1;  /* bit[4]    : 此bit写1, bit[3:0]才生效 */
        unsigned int  reserved_0                    : 3;  /* bit[5-7]  :  */
        unsigned int  ddrc_csysreq_stat_aocrg2sc    : 4;  /* bit[8-11] :  */
        unsigned int  ddrc_csysack_stat             : 2;  /* bit[12-13]:  */
        unsigned int  reserved_1                    : 18; /* bit[14-31]:  */
    } reg;
} SOC_AO_CRG_SC_DDRC_CTRL_UNION;
#endif
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysreq_cfg_sc2aocrg_START      (0)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysreq_cfg_sc2aocrg_END        (3)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysreq_sft_cfg_sc2aocrg_START  (4)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysreq_sft_cfg_sc2aocrg_END    (4)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysreq_stat_aocrg2sc_START     (8)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysreq_stat_aocrg2sc_END       (11)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysack_stat_START              (12)
#define SOC_AO_CRG_SC_DDRC_CTRL_ddrc_csysack_stat_END                (13)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLLCTRL0_UNION
 struct description   : SCSPLLCTRL0 Register structure definition
                        Address Offset:0xF14 Initial:0x00 Width:32
 register description : SPLL控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spll_en       : 1;  /* bit[0-0]  : SPLL软件使能控制（最终的使能由该配置信号、IOMCU侧控制信号IOMCU_pll_ctrl、asp_SPLL_en_req 做或）
                                                         0：去使能；
                                                         1：使能。 */
        unsigned int  spll_bp       : 1;  /* bit[1-1]  : SPLL Bypass控制。
                                                         0：正常工作；
                                                         1：Bypass。 */
        unsigned int  spll_refdiv   : 6;  /* bit[2-7]  : SPLL输入时钟分频器。 */
        unsigned int  spll_fbdiv    : 12; /* bit[8-19] : SPLL反馈时钟整数分频器。
                                                         。 */
        unsigned int  spll_postdiv1 : 3;  /* bit[20-22]: SPLL输出时钟分频器1。 */
        unsigned int  spll_postdiv2 : 3;  /* bit[23-25]: SPLL输出时钟分频器2。 */
        unsigned int  reserved      : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCSPLLCTRL0_UNION;
#endif
#define SOC_AO_CRG_SCSPLLCTRL0_spll_en_START        (0)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_en_END          (0)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_bp_START        (1)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_bp_END          (1)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_refdiv_START    (2)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_refdiv_END      (7)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_fbdiv_START     (8)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_fbdiv_END       (19)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_postdiv1_START  (20)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_postdiv1_END    (22)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_postdiv2_START  (23)
#define SOC_AO_CRG_SCSPLLCTRL0_spll_postdiv2_END    (25)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLLCTRL1_UNION
 struct description   : SCSPLLCTRL1 Register structure definition
                        Address Offset:0xF18 Initial:0x0 Width:32
 register description : SPLL控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spll_fracdiv : 24; /* bit[0-23] : SPLL反馈时钟分数分频器。分频比=配置值+1。 */
        unsigned int  spll_int_mod : 1;  /* bit[24-24]: SPLL整数分频模式。
                                                        0：分数分频模式；
                                                        1：整数分频模式。
                                                        注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  spll_cfg_vld : 1;  /* bit[25-25]: SPLL配置有效标志。
                                                        0：配置无效；
                                                        1：配置有效。 */
        unsigned int  gt_clk_spll  : 1;  /* bit[26-26]: SPLL门控信号。
                                                        0：SPLL输出时钟门控；
                                                        1：SPLL输出时钟不门控。（等待PLL稳定后才能配置该bit）。 */
        unsigned int  sel_spll_mux : 1;  /* bit[27-27]: SPLL的时钟源选择(静态配置)。
                                                        0：FNPLL。
                                                        1：FNPLL_LP。 */
        unsigned int  reserved     : 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_AO_CRG_SCSPLLCTRL1_UNION;
#endif
#define SOC_AO_CRG_SCSPLLCTRL1_spll_fracdiv_START  (0)
#define SOC_AO_CRG_SCSPLLCTRL1_spll_fracdiv_END    (23)
#define SOC_AO_CRG_SCSPLLCTRL1_spll_int_mod_START  (24)
#define SOC_AO_CRG_SCSPLLCTRL1_spll_int_mod_END    (24)
#define SOC_AO_CRG_SCSPLLCTRL1_spll_cfg_vld_START  (25)
#define SOC_AO_CRG_SCSPLLCTRL1_spll_cfg_vld_END    (25)
#define SOC_AO_CRG_SCSPLLCTRL1_gt_clk_spll_START   (26)
#define SOC_AO_CRG_SCSPLLCTRL1_gt_clk_spll_END     (26)
#define SOC_AO_CRG_SCSPLLCTRL1_sel_spll_mux_START  (27)
#define SOC_AO_CRG_SCSPLLCTRL1_sel_spll_mux_END    (27)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLLSSCCTRL_UNION
 struct description   : SCSPLLSSCCTRL Register structure definition
                        Address Offset:0xF1C Initial:0x0000 Width:32
 register description : SPLL SSC控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sscg_enable   : 1;  /* bit[0]    : SSCG模块使能，高有效。
                                                         0：关闭；
                                                         1：正常工作。 */
        unsigned int  sscg_mode     : 1;  /* bit[1]    : SSCG模块展频模式，静态配置。
                                                         0：中心展频；
                                                         1：向下展频。 */
        unsigned int  sscg_rate     : 4;  /* bit[2-5]  : SSCG模块展频速度；展频工作时钟的频率，静态配置。
                                                         freq=(CLKSSCG / (RATE + 1 ))。 */
        unsigned int  sscg_depth    : 3;  /* bit[6-8]  : SSCG模块调制深度(%)，静态配置。
                                                         3'b000 = 0.049%；
                                                         3'b001 = 0.098%；
                                                         3'b010 = 0.195%；
                                                         3'b011 = 0.391%；
                                                         3'b100 = 0.781%；
                                                         3'b101 = 1.563%；
                                                         3'b110 = 3.125%；
                                                         3'b111 = 6.250%。 */
        unsigned int  sscg_fast_dis : 1;  /* bit[9]    : SSCG模块快速退出功能使能，静态配置。
                                                         0：不使能快速退出（推荐）
                                                         1：使能快速退出 */
        unsigned int  reserved      : 6;  /* bit[10-15]:  */
        unsigned int  bitmasken     : 16; /* bit[16-31]: 只有当bitmasken对应的比特位为1'b1，相应的比特位才起作用。bitmasken[0]就是bit[0]的mask使能位。写1有效。 */
    } reg;
} SOC_AO_CRG_SCSPLLSSCCTRL_UNION;
#endif
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_enable_START    (0)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_enable_END      (0)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_mode_START      (1)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_mode_END        (1)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_rate_START      (2)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_rate_END        (5)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_depth_START     (6)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_depth_END       (8)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_fast_dis_START  (9)
#define SOC_AO_CRG_SCSPLLSSCCTRL_sscg_fast_dis_END    (9)
#define SOC_AO_CRG_SCSPLLSSCCTRL_bitmasken_START      (16)
#define SOC_AO_CRG_SCSPLLSSCCTRL_bitmasken_END        (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLLCTRL0_LOCK_STAT_UNION
 struct description   : SCSPLLCTRL0_LOCK_STAT Register structure definition
                        Address Offset:0xF20 Initial:0x00000000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 寄存器排布与“SCSPLLCTRL0”相同 */
    } reg;
} SOC_AO_CRG_SCSPLLCTRL0_LOCK_STAT_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLLCTRL1_LOCK_STAT_UNION
 struct description   : SCSPLLCTRL1_LOCK_STAT Register structure definition
                        Address Offset:0xF24 Initial:0x00000000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 寄存器排布与“SCSPLLCTRL1”相同 */
    } reg;
} SOC_AO_CRG_SCSPLLCTRL1_LOCK_STAT_UNION;
#endif


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_UNION
 struct description   : SCSPLLSSCCTRL_LOCK_STAT Register structure definition
                        Address Offset:0xF28 Initial:0x0000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  scspllsscctrl_lock_stat : 16; /* bit[0-15] : 寄存器排布与“SCSPLLSSCCTRL”相同 */
        unsigned int  reserved                : 16; /* bit[16-31]:  */
    } reg;
} SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_UNION;
#endif
#define SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_scspllsscctrl_lock_stat_START  (0)
#define SOC_AO_CRG_SCSPLLSSCCTRL_LOCK_STAT_scspllsscctrl_lock_stat_END    (15)


/*****************************************************************************
 struct               : SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_UNION
 struct description   : SCSPLL_OTHER_LOCK_STAT Register structure definition
                        Address Offset:0xF2C Initial:0x000 Width:32
 register description : 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  scclkdiv1_sec_lock_stat : 16; /* bit[0-15] :  */
        unsigned int  spll_lock_bypass_stat   : 1;  /* bit[16]   :  */
        unsigned int  reserved_0              : 3;  /* bit[17-19]:  */
        unsigned int  spll_lock_bypass        : 1;  /* bit[20]   :  */
        unsigned int  reserved_1              : 11; /* bit[21-31]:  */
    } reg;
} SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_UNION;
#endif
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_scclkdiv1_sec_lock_stat_START  (0)
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_scclkdiv1_sec_lock_stat_END    (15)
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_spll_lock_bypass_stat_START    (16)
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_spll_lock_bypass_stat_END      (16)
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_spll_lock_bypass_START         (20)
#define SOC_AO_CRG_SCSPLL_OTHER_LOCK_STAT_spll_lock_bypass_END           (20)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_CFG0_UNION
 struct description   : FNPLL_CFG0 Register structure definition
                        Address Offset:0xF30 Initial:0x0F Width:32
 register description : fnpll配置寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sta_reg_sel      : 1;  /* bit[0]    : 寄存器回读选择信号，选择当前读的是哪组状态寄存器：对应寄存器详细描述见“寄存器复用详细说明”页
                                                            0：选择读寄存器SCPPLLSTATE0_0【默认】
                                                            1：选择读寄存器SCPPLLSTATE0_1 */
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
                                                            0：不清零unlock_flag以及历史失锁统计次数；（默认）
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
                                                            00-电源电压1.8V；
                                                            01-电源电压1.2V；
                                                            10-reserved；
                                                            11-reserved； */
        unsigned int  icp_ctrl         : 2;  /* bit[14-15]: fnpll的ICP偏置电流选择信号
                                                            3：reserved
                                                            2：reserved
                                                            1：选择1/4 CP偏置电流【默认】
                                                            0：选择1/2 CP偏置电流 */
        unsigned int  updn_sel         : 1;  /* bit[16]   : fnpll的CP路数选择信号
                                                            1：打开1路CP
                                                            0：打开2路CP【默认】
                                                            当Fpfd<20MHz时，需配置为1 */
        unsigned int  reserved         : 2;  /* bit[17-18]: 保留 */
        unsigned int  int_mask         : 3;  /* bit[19-21]: 中断屏蔽位，为1时中断被屏蔽。中断通过或逻辑连到pll_int。
                                                            xx1：屏蔽unlock_flag；
                                                            x1x：屏蔽bbpd_calib_fail；
                                                            1xx：屏蔽dtc_calib_satu。 */
        unsigned int  pfd_div_ratio    : 4;  /* bit[22-25]: dc_ac模块内pfd分频比设置,默认不分频。建议不要配置。
                                                            4'b0000: 1分频（固定默认值）
                                                            4'b0001: 1分频
                                                            4'b0010: 2分频
                                                            ….
                                                            4'b1111: 15分频 */
        unsigned int  freq_threshold   : 6;  /* bit[26-31]: 判断振荡器输出时钟频率的阈值，推荐配置大于等于4（0/1禁配）,freq_threshold=pfd_div_ratio*1.2GHz/[4*freq(pfd_clk)]-1；
                                                            (当计算值小于4时取4，并且pfd_div_ratio固定为1分频）,例如pfd_clk频率默认为38.4MHz，计算得freq_threshold配置值为7。
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
} SOC_AO_CRG_FNPLL_CFG0_UNION;
#endif
#define SOC_AO_CRG_FNPLL_CFG0_sta_reg_sel_START       (0)
#define SOC_AO_CRG_FNPLL_CFG0_sta_reg_sel_END         (0)
#define SOC_AO_CRG_FNPLL_CFG0_read_en_START           (1)
#define SOC_AO_CRG_FNPLL_CFG0_read_en_END             (1)
#define SOC_AO_CRG_FNPLL_CFG0_input_cfg_en_START      (2)
#define SOC_AO_CRG_FNPLL_CFG0_input_cfg_en_END        (2)
#define SOC_AO_CRG_FNPLL_CFG0_test_mode_START         (3)
#define SOC_AO_CRG_FNPLL_CFG0_test_mode_END           (3)
#define SOC_AO_CRG_FNPLL_CFG0_unlock_clear_START      (4)
#define SOC_AO_CRG_FNPLL_CFG0_unlock_clear_END        (4)
#define SOC_AO_CRG_FNPLL_CFG0_dll_en_START            (5)
#define SOC_AO_CRG_FNPLL_CFG0_dll_en_END              (5)
#define SOC_AO_CRG_FNPLL_CFG0_pll_lock_ate_sel_START  (6)
#define SOC_AO_CRG_FNPLL_CFG0_pll_lock_ate_sel_END    (7)
#define SOC_AO_CRG_FNPLL_CFG0_test_data_sel_START     (8)
#define SOC_AO_CRG_FNPLL_CFG0_test_data_sel_END       (11)
#define SOC_AO_CRG_FNPLL_CFG0_bias_ctrl_START         (12)
#define SOC_AO_CRG_FNPLL_CFG0_bias_ctrl_END           (13)
#define SOC_AO_CRG_FNPLL_CFG0_icp_ctrl_START          (14)
#define SOC_AO_CRG_FNPLL_CFG0_icp_ctrl_END            (15)
#define SOC_AO_CRG_FNPLL_CFG0_updn_sel_START          (16)
#define SOC_AO_CRG_FNPLL_CFG0_updn_sel_END            (16)
#define SOC_AO_CRG_FNPLL_CFG0_int_mask_START          (19)
#define SOC_AO_CRG_FNPLL_CFG0_int_mask_END            (21)
#define SOC_AO_CRG_FNPLL_CFG0_pfd_div_ratio_START     (22)
#define SOC_AO_CRG_FNPLL_CFG0_pfd_div_ratio_END       (25)
#define SOC_AO_CRG_FNPLL_CFG0_freq_threshold_START    (26)
#define SOC_AO_CRG_FNPLL_CFG0_freq_threshold_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_CFG1_UNION
 struct description   : FNPLL_CFG1 Register structure definition
                        Address Offset:0xF34 Initial:0x0000 Width:32
 register description : fnpll配置寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
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
} SOC_AO_CRG_FNPLL_CFG1_UNION;
#endif
#define SOC_AO_CRG_FNPLL_CFG1_ctinue_lock_num_START  (0)
#define SOC_AO_CRG_FNPLL_CFG1_ctinue_lock_num_END    (5)
#define SOC_AO_CRG_FNPLL_CFG1_lock_thr_START         (6)
#define SOC_AO_CRG_FNPLL_CFG1_lock_thr_END           (7)
#define SOC_AO_CRG_FNPLL_CFG1_dc_ac_clk_en_START     (8)
#define SOC_AO_CRG_FNPLL_CFG1_dc_ac_clk_en_END       (8)
#define SOC_AO_CRG_FNPLL_CFG1_dtc_ctrl_inv_START     (9)
#define SOC_AO_CRG_FNPLL_CFG1_dtc_ctrl_inv_END       (9)
#define SOC_AO_CRG_FNPLL_CFG1_ana_cfg_fnpll_START    (16)
#define SOC_AO_CRG_FNPLL_CFG1_ana_cfg_fnpll_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_CFG2_UNION
 struct description   : FNPLL_CFG2 Register structure definition
                        Address Offset:0xF38 Initial:0x20 Width:32
 register description : fnpll配置寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
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
} SOC_AO_CRG_FNPLL_CFG2_UNION;
#endif
#define SOC_AO_CRG_FNPLL_CFG2_div2_pd_START         (0)
#define SOC_AO_CRG_FNPLL_CFG2_div2_pd_END           (0)
#define SOC_AO_CRG_FNPLL_CFG2_fbdiv_rst_n_START     (1)
#define SOC_AO_CRG_FNPLL_CFG2_fbdiv_rst_n_END       (1)
#define SOC_AO_CRG_FNPLL_CFG2_refdiv_rst_n_START    (2)
#define SOC_AO_CRG_FNPLL_CFG2_refdiv_rst_n_END      (2)
#define SOC_AO_CRG_FNPLL_CFG2_dll_force_en_START    (3)
#define SOC_AO_CRG_FNPLL_CFG2_dll_force_en_END      (3)
#define SOC_AO_CRG_FNPLL_CFG2_phe_code_a_START      (4)
#define SOC_AO_CRG_FNPLL_CFG2_phe_code_a_END        (5)
#define SOC_AO_CRG_FNPLL_CFG2_phe_code_b_START      (6)
#define SOC_AO_CRG_FNPLL_CFG2_phe_code_b_END        (7)
#define SOC_AO_CRG_FNPLL_CFG2_bbpd_calib_byp_START  (8)
#define SOC_AO_CRG_FNPLL_CFG2_bbpd_calib_byp_END    (8)
#define SOC_AO_CRG_FNPLL_CFG2_k_gain_cfg_en_START   (9)
#define SOC_AO_CRG_FNPLL_CFG2_k_gain_cfg_en_END     (9)
#define SOC_AO_CRG_FNPLL_CFG2_k_gain_cfg_START      (10)
#define SOC_AO_CRG_FNPLL_CFG2_k_gain_cfg_END        (15)
#define SOC_AO_CRG_FNPLL_CFG2_k_gain_av_thr_START   (16)
#define SOC_AO_CRG_FNPLL_CFG2_k_gain_av_thr_END     (18)
#define SOC_AO_CRG_FNPLL_CFG2_dtc_test_START        (19)
#define SOC_AO_CRG_FNPLL_CFG2_dtc_test_END          (19)
#define SOC_AO_CRG_FNPLL_CFG2_dtc_m_cfg_START       (20)
#define SOC_AO_CRG_FNPLL_CFG2_dtc_m_cfg_END         (25)
#define SOC_AO_CRG_FNPLL_CFG2_dtc_o_cfg_START       (26)
#define SOC_AO_CRG_FNPLL_CFG2_dtc_o_cfg_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_CFG3_UNION
 struct description   : FNPLL_CFG3 Register structure definition
                        Address Offset:0xF3C Initial:0x0028627D Width:32
 register description : fnpll配置寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  scppll_cfg3 : 32; /* bit[0-31]: 描述后续待刷新 */
    } reg;
} SOC_AO_CRG_FNPLL_CFG3_UNION;
#endif
#define SOC_AO_CRG_FNPLL_CFG3_scppll_cfg3_START  (0)
#define SOC_AO_CRG_FNPLL_CFG3_scppll_cfg3_END    (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_LP_CFG0_UNION
 struct description   : FNPLL_LP_CFG0 Register structure definition
                        Address Offset:0xF40 Initial:0x0F Width:32
 register description : fnpll配置寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sta_reg_sel      : 1;  /* bit[0]    : 寄存器回读选择信号，选择当前读的是哪组状态寄存器：对应寄存器详细描述见“寄存器复用详细说明”页
                                                            0：选择读寄存器SCPPLLSTATE0_0【默认】
                                                            1：选择读寄存器SCPPLLSTATE0_1 */
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
                                                            0：不清零unlock_flag以及历史失锁统计次数；（默认）
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
                                                            00-电源电压1.8V；
                                                            01-电源电压1.2V；
                                                            10-reserved；
                                                            11-reserved； */
        unsigned int  icp_ctrl         : 2;  /* bit[14-15]: fnpll的ICP偏置电流选择信号
                                                            3：reserved
                                                            2：reserved
                                                            1：选择1/4 CP偏置电流【默认】
                                                            0：选择1/2 CP偏置电流 */
        unsigned int  updn_sel         : 1;  /* bit[16]   : fnpll的CP路数选择信号
                                                            1：打开1路CP
                                                            0：打开2路CP【默认】
                                                            当Fpfd<20MHz时，需配置为1 */
        unsigned int  reserved         : 2;  /* bit[17-18]: 保留 */
        unsigned int  int_mask         : 3;  /* bit[19-21]: 中断屏蔽位，为1时中断被屏蔽。中断通过或逻辑连到pll_int。
                                                            xx1：屏蔽unlock_flag；
                                                            x1x：屏蔽bbpd_calib_fail；
                                                            1xx：屏蔽dtc_calib_satu。 */
        unsigned int  pfd_div_ratio    : 4;  /* bit[22-25]: dc_ac模块内pfd分频比设置,默认不分频。建议不要配置。
                                                            4'b0000: 1分频（固定默认值）
                                                            4'b0001: 1分频
                                                            4'b0010: 2分频
                                                            ….
                                                            4'b1111: 15分频 */
        unsigned int  freq_threshold   : 6;  /* bit[26-31]: 判断振荡器输出时钟频率的阈值，推荐配置大于等于4（0/1禁配）,freq_threshold=pfd_div_ratio*1.2GHz/[4*freq(pfd_clk)]-1；
                                                            (当计算值小于4时取4，并且pfd_div_ratio固定为1分频）,例如pfd_clk频率默认为38.4MHz，计算得freq_threshold配置值为7。
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
} SOC_AO_CRG_FNPLL_LP_CFG0_UNION;
#endif
#define SOC_AO_CRG_FNPLL_LP_CFG0_sta_reg_sel_START       (0)
#define SOC_AO_CRG_FNPLL_LP_CFG0_sta_reg_sel_END         (0)
#define SOC_AO_CRG_FNPLL_LP_CFG0_read_en_START           (1)
#define SOC_AO_CRG_FNPLL_LP_CFG0_read_en_END             (1)
#define SOC_AO_CRG_FNPLL_LP_CFG0_input_cfg_en_START      (2)
#define SOC_AO_CRG_FNPLL_LP_CFG0_input_cfg_en_END        (2)
#define SOC_AO_CRG_FNPLL_LP_CFG0_test_mode_START         (3)
#define SOC_AO_CRG_FNPLL_LP_CFG0_test_mode_END           (3)
#define SOC_AO_CRG_FNPLL_LP_CFG0_unlock_clear_START      (4)
#define SOC_AO_CRG_FNPLL_LP_CFG0_unlock_clear_END        (4)
#define SOC_AO_CRG_FNPLL_LP_CFG0_dll_en_START            (5)
#define SOC_AO_CRG_FNPLL_LP_CFG0_dll_en_END              (5)
#define SOC_AO_CRG_FNPLL_LP_CFG0_pll_lock_ate_sel_START  (6)
#define SOC_AO_CRG_FNPLL_LP_CFG0_pll_lock_ate_sel_END    (7)
#define SOC_AO_CRG_FNPLL_LP_CFG0_test_data_sel_START     (8)
#define SOC_AO_CRG_FNPLL_LP_CFG0_test_data_sel_END       (11)
#define SOC_AO_CRG_FNPLL_LP_CFG0_bias_ctrl_START         (12)
#define SOC_AO_CRG_FNPLL_LP_CFG0_bias_ctrl_END           (13)
#define SOC_AO_CRG_FNPLL_LP_CFG0_icp_ctrl_START          (14)
#define SOC_AO_CRG_FNPLL_LP_CFG0_icp_ctrl_END            (15)
#define SOC_AO_CRG_FNPLL_LP_CFG0_updn_sel_START          (16)
#define SOC_AO_CRG_FNPLL_LP_CFG0_updn_sel_END            (16)
#define SOC_AO_CRG_FNPLL_LP_CFG0_int_mask_START          (19)
#define SOC_AO_CRG_FNPLL_LP_CFG0_int_mask_END            (21)
#define SOC_AO_CRG_FNPLL_LP_CFG0_pfd_div_ratio_START     (22)
#define SOC_AO_CRG_FNPLL_LP_CFG0_pfd_div_ratio_END       (25)
#define SOC_AO_CRG_FNPLL_LP_CFG0_freq_threshold_START    (26)
#define SOC_AO_CRG_FNPLL_LP_CFG0_freq_threshold_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_LP_CFG1_UNION
 struct description   : FNPLL_LP_CFG1 Register structure definition
                        Address Offset:0xF44 Initial:0x0000 Width:32
 register description : fnpll配置寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
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
} SOC_AO_CRG_FNPLL_LP_CFG1_UNION;
#endif
#define SOC_AO_CRG_FNPLL_LP_CFG1_ctinue_lock_num_START  (0)
#define SOC_AO_CRG_FNPLL_LP_CFG1_ctinue_lock_num_END    (5)
#define SOC_AO_CRG_FNPLL_LP_CFG1_lock_thr_START         (6)
#define SOC_AO_CRG_FNPLL_LP_CFG1_lock_thr_END           (7)
#define SOC_AO_CRG_FNPLL_LP_CFG1_dc_ac_clk_en_START     (8)
#define SOC_AO_CRG_FNPLL_LP_CFG1_dc_ac_clk_en_END       (8)
#define SOC_AO_CRG_FNPLL_LP_CFG1_dtc_ctrl_inv_START     (9)
#define SOC_AO_CRG_FNPLL_LP_CFG1_dtc_ctrl_inv_END       (9)
#define SOC_AO_CRG_FNPLL_LP_CFG1_ana_cfg_fnpll_START    (16)
#define SOC_AO_CRG_FNPLL_LP_CFG1_ana_cfg_fnpll_END      (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_LP_CFG2_UNION
 struct description   : FNPLL_LP_CFG2 Register structure definition
                        Address Offset:0xF48 Initial:0x20 Width:32
 register description : fnpll配置寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
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
} SOC_AO_CRG_FNPLL_LP_CFG2_UNION;
#endif
#define SOC_AO_CRG_FNPLL_LP_CFG2_div2_pd_START         (0)
#define SOC_AO_CRG_FNPLL_LP_CFG2_div2_pd_END           (0)
#define SOC_AO_CRG_FNPLL_LP_CFG2_fbdiv_rst_n_START     (1)
#define SOC_AO_CRG_FNPLL_LP_CFG2_fbdiv_rst_n_END       (1)
#define SOC_AO_CRG_FNPLL_LP_CFG2_refdiv_rst_n_START    (2)
#define SOC_AO_CRG_FNPLL_LP_CFG2_refdiv_rst_n_END      (2)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dll_force_en_START    (3)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dll_force_en_END      (3)
#define SOC_AO_CRG_FNPLL_LP_CFG2_phe_code_a_START      (4)
#define SOC_AO_CRG_FNPLL_LP_CFG2_phe_code_a_END        (5)
#define SOC_AO_CRG_FNPLL_LP_CFG2_phe_code_b_START      (6)
#define SOC_AO_CRG_FNPLL_LP_CFG2_phe_code_b_END        (7)
#define SOC_AO_CRG_FNPLL_LP_CFG2_bbpd_calib_byp_START  (8)
#define SOC_AO_CRG_FNPLL_LP_CFG2_bbpd_calib_byp_END    (8)
#define SOC_AO_CRG_FNPLL_LP_CFG2_k_gain_cfg_en_START   (9)
#define SOC_AO_CRG_FNPLL_LP_CFG2_k_gain_cfg_en_END     (9)
#define SOC_AO_CRG_FNPLL_LP_CFG2_k_gain_cfg_START      (10)
#define SOC_AO_CRG_FNPLL_LP_CFG2_k_gain_cfg_END        (15)
#define SOC_AO_CRG_FNPLL_LP_CFG2_k_gain_av_thr_START   (16)
#define SOC_AO_CRG_FNPLL_LP_CFG2_k_gain_av_thr_END     (18)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dtc_test_START        (19)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dtc_test_END          (19)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dtc_m_cfg_START       (20)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dtc_m_cfg_END         (25)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dtc_o_cfg_START       (26)
#define SOC_AO_CRG_FNPLL_LP_CFG2_dtc_o_cfg_END         (31)


/*****************************************************************************
 struct               : SOC_AO_CRG_FNPLL_LP_CFG3_UNION
 struct description   : FNPLL_LP_CFG3 Register structure definition
                        Address Offset:0xF4C Initial:0x0028627D Width:32
 register description : fnpll配置寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  scppll_cfg3 : 32; /* bit[0-31]: 描述后续待刷新 */
    } reg;
} SOC_AO_CRG_FNPLL_LP_CFG3_UNION;
#endif
#define SOC_AO_CRG_FNPLL_LP_CFG3_scppll_cfg3_START  (0)
#define SOC_AO_CRG_FNPLL_LP_CFG3_scppll_cfg3_END    (31)






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

#endif /* end of soc_ao_crg_interface.h */
