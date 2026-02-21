

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_MEDIA2_CRG_INTERFACE_H__
#define __SOC_MEDIA2_CRG_INTERFACE_H__

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


/* Register description: 外设时钟使能寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PEREN0_UNION */
#define SOC_MEDIA2_CRG_PEREN0_ADDR(base)                      (base)

/* Register description: 外设时钟禁止寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERDIS0_UNION */
#define SOC_MEDIA2_CRG_PERDIS0_ADDR(base)                     ((base) + 0x004UL)

/* Register description: 外设时钟使能状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERCLKEN0_UNION */
#define SOC_MEDIA2_CRG_PERCLKEN0_ADDR(base)                   ((base) + 0x008UL)

/* Register description: 外设时钟最终状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERSTAT0_UNION */
#define SOC_MEDIA2_CRG_PERSTAT0_ADDR(base)                    ((base) + 0x00CUL)

/* Register description: 外设软复位使能寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTEN0_UNION */
#define SOC_MEDIA2_CRG_PERRSTEN0_ADDR(base)                   ((base) + 0x030UL)

/* Register description: 外设软复位撤离寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTDIS0_UNION */
#define SOC_MEDIA2_CRG_PERRSTDIS0_ADDR(base)                  ((base) + 0x034UL)

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTSTAT0_UNION */
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ADDR(base)                 ((base) + 0x038UL)

/* Register description: 时钟分频比控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV0_UNION */
#define SOC_MEDIA2_CRG_CLKDIV0_ADDR(base)                     ((base) + 0x080UL)

/* Register description: 时钟分频比控制寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV1_UNION */
#define SOC_MEDIA2_CRG_CLKDIV1_ADDR(base)                     ((base) + 0x084UL)

/* Register description: 时钟分频比控制寄存器2。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV2_UNION */
#define SOC_MEDIA2_CRG_CLKDIV2_ADDR(base)                     ((base) + 0x088UL)

/* Register description: 时钟分频比控制寄存器3。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV3_UNION */
#define SOC_MEDIA2_CRG_CLKDIV3_ADDR(base)                     ((base) + 0x08CUL)

/* Register description: 时钟分频比控制寄存器4。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV4_UNION */
#define SOC_MEDIA2_CRG_CLKDIV4_ADDR(base)                     ((base) + 0x090UL)

/* Register description: 时钟分频比控制寄存器5。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV5_UNION */
#define SOC_MEDIA2_CRG_CLKDIV5_ADDR(base)                     ((base) + 0x094UL)

/* Register description: 时钟分频比控制寄存器6。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV6_UNION */
#define SOC_MEDIA2_CRG_CLKDIV6_ADDR(base)                     ((base) + 0x098UL)

/* Register description: 时钟分频比控制寄存器7。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV7_UNION */
#define SOC_MEDIA2_CRG_CLKDIV7_ADDR(base)                     ((base) + 0x09CUL)

/* Register description: 时钟分频比控制寄存器8。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV8_UNION */
#define SOC_MEDIA2_CRG_CLKDIV8_ADDR(base)                     ((base) + 0x0A0UL)

/* Register description: 时钟分频比控制寄存器9。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV9_UNION */
#define SOC_MEDIA2_CRG_CLKDIV9_ADDR(base)                     ((base) + 0x0A4UL)

/* Register description: 时钟分频比控制寄存器10。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV10_UNION */
#define SOC_MEDIA2_CRG_CLKDIV10_ADDR(base)                    ((base) + 0x0A8UL)

/* Register description: 时钟分频比控制寄存器11。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV11_UNION */
#define SOC_MEDIA2_CRG_CLKDIV11_ADDR(base)                    ((base) + 0x0ACUL)

/* Register description: 时钟分频比控制寄存器12。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV12_UNION */
#define SOC_MEDIA2_CRG_CLKDIV12_ADDR(base)                    ((base) + 0x0B0UL)

/* Register description: 时钟分频比控制寄存器13。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV13_UNION */
#define SOC_MEDIA2_CRG_CLKDIV13_ADDR(base)                    ((base) + 0x0B4UL)

/* Register description: 时钟分频比控制寄存器14。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV14_UNION */
#define SOC_MEDIA2_CRG_CLKDIV14_ADDR(base)                    ((base) + 0x0B8UL)

/* Register description: 时钟分频比控制寄存器15。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV15_UNION */
#define SOC_MEDIA2_CRG_CLKDIV15_ADDR(base)                    ((base) + 0x0BCUL)

/* Register description: 时钟分频比控制寄存器16。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV16_UNION */
#define SOC_MEDIA2_CRG_CLKDIV16_ADDR(base)                    ((base) + 0x0C0UL)

/* Register description: 时钟分频比控制寄存器17。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV17_UNION */
#define SOC_MEDIA2_CRG_CLKDIV17_ADDR(base)                    ((base) + 0x0C4UL)

/* Register description: 时钟分频比控制寄存器18。
            电池欠压降频控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV18_UNION */
#define SOC_MEDIA2_CRG_CLKDIV18_ADDR(base)                    ((base) + 0x0C8UL)

/* Register description: 时钟分频比控制寄存器19。
            电池欠压降频控制寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV19_UNION */
#define SOC_MEDIA2_CRG_CLKDIV19_ADDR(base)                    ((base) + 0x0CCUL)

/* Register description: 外设状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_STAT0_UNION */
#define SOC_MEDIA2_CRG_PERI_STAT0_ADDR(base)                  ((base) + 0x120UL)

/* Register description: 外设状态寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_STAT1_UNION */
#define SOC_MEDIA2_CRG_PERI_STAT1_ADDR(base)                  ((base) + 0x124UL)

/* Register description: 外设状态寄存器2。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_STAT2_UNION */
#define SOC_MEDIA2_CRG_PERI_STAT2_ADDR(base)                  ((base) + 0x128UL)

/* Register description: ISPCPU子系统时钟使能寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_CLKEN_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ADDR(base)                ((base) + 0x140UL)

/* Register description: ISPCPU子系统软复位使能寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_RSTEN_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ADDR(base)                ((base) + 0x144UL)

/* Register description: ISPCPU子系统软复位撤离寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_RSTDIS_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ADDR(base)               ((base) + 0x148UL)

/* Register description: ISPCPU子系统软复位使能状态寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ADDR(base)              ((base) + 0x14CUL)

/* Register description: ISPCPU状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_CPU_STATE0_UNION */
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_ADDR(base)              ((base) + 0x150UL)

/* Register description: ISPCPU状态寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_CPU_STATE1_UNION */
#define SOC_MEDIA2_CRG_ISP_CPU_STATE1_ADDR(base)              ((base) + 0x154UL)

/* Register description: 外设自动降频控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_AUTODIV0_UNION */
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_ADDR(base)               ((base) + 0x160UL)

/* Register description: 外设自动降频控制寄存器3。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_AUTODIV1_UNION */
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ADDR(base)               ((base) + 0x16cUL)

/* Register description: 外设自动降频控制寄存器4。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_ADDR(base) ((base) + 0x170UL)

/* Register description: 外设自动降频控制寄存器5。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_ADDR(base) ((base) + 0x174UL)

/* Register description: 外设自动降频控制寄存器6。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_ADDR(base) ((base) + 0x178UL)

/* Register description: 外设自动降频控制寄存器11。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_ADDR(base) ((base) + 0x18cUL)

/* Register description: 外设自动降频控制寄存器12。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_ADDR(base) ((base) + 0x190UL)

/* Register description: 外设自动降频控制寄存器13。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_ADDR(base) ((base) + 0x194UL)

/* Register description: 自动降频状态寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_UNION */
#define SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_ADDR(base)          ((base) + 0x198UL)

/* Register description: ISP的状态回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_UNION */
#define SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_ADDR(base)        ((base) + 0x19cUL)

/* Register description: 外设自动降频状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_UNION */
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_ADDR(base)           ((base) + 0x1A0UL)

/* Register description: 总线异常中断屏蔽与清除寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_UNION */
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_ADDR(base)       ((base) + 0x1D0UL)

/* Register description: 总线异常中断状态寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_UNION */
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_ADDR(base)  ((base) + 0x1D4UL)

/* Register description: 外设IP防挂死功能byapss控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_IPCLKRST_BYPASS_UNION */
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_ADDR(base)             ((base) + 0x1D8UL)

/* Register description: hpm配置寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPM_CFG_UNION */
#define SOC_MEDIA2_CRG_HPM_CFG_ADDR(base)                     ((base) + 0x200UL)

/* Register description: hpm回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPM_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_HPM_READ_BACK_ADDR(base)               ((base) + 0x204UL)

/* Register description: hpmx配置寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPMX_CFG_UNION */
#define SOC_MEDIA2_CRG_HPMX_CFG_ADDR(base)                    ((base) + 0x208UL)

/* Register description: hpmx回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPMX_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_ADDR(base)              ((base) + 0x20CUL)

/* Register description: himinibus自动门控配置寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_UNION */
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_ADDR(base)            ((base) + 0x400UL)

/* Register description: himinibus idle回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_ADDR(base)    ((base) + 0x404UL)

/* Register description: isp回读寄存器0
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_READ_BACK0_UNION */
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_ADDR(base)              ((base) + 0x408UL)

/* Register description: isp回读寄存器1
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_READ_BACK1_UNION */
#define SOC_MEDIA2_CRG_ISP_READ_BACK1_ADDR(base)              ((base) + 0x40CUL)

/* Register description: isp himinibus控制寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_UNION */
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_ADDR(base)          ((base) + 0x410UL)

/* Register description: himinibus dlock clr寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_UNION */
#define SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_ADDR(base)         ((base) + 0x414UL)

/* Register description: ipp 2to3桥回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_IPP_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ADDR(base)               ((base) + 0x418UL)

/* Register description: 通用安全软复位使能寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ADDR(base)        ((base) + 0xC00UL)

/* Register description: 通用安全软复位撤离寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ADDR(base)       ((base) + 0xC04UL)

/* Register description: 通用安全软复位使能状态寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ADDR(base)      ((base) + 0xC08UL)

/* Register description: 外设时钟安全使能寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_ADDR(base)          ((base) + 0xC30UL)

/* Register description: 外设时钟安全禁止寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_ADDR(base)         ((base) + 0xC34UL)

/* Register description: 外设时钟安全使能状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_ADDR(base)       ((base) + 0xC38UL)

/* Register description: 外设时钟安全最终状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_ADDR(base)        ((base) + 0xC3CUL)

/* Register description: ISP安全软复位使能寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ADDR(base)            ((base) + 0xD00UL)

/* Register description: ISP安全软复位撤离寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ADDR(base)           ((base) + 0xD04UL)

/* Register description: ISP安全软复位状态寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ADDR(base)          ((base) + 0xD08UL)

/* Register description: ISPCPU控制配置寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_ADDR(base)            ((base) + 0xD10UL)

/* Register description: 绝对安全寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_DECPROT0SET_UNION */
#define SOC_MEDIA2_CRG_DECPROT0SET_ADDR(base)                 ((base) + 0xF00UL)


#else


/* Register description: 外设时钟使能寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PEREN0_UNION */
#define SOC_MEDIA2_CRG_PEREN0_ADDR(base)                      (base)

/* Register description: 外设时钟禁止寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERDIS0_UNION */
#define SOC_MEDIA2_CRG_PERDIS0_ADDR(base)                     ((base) + 0x004)

/* Register description: 外设时钟使能状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERCLKEN0_UNION */
#define SOC_MEDIA2_CRG_PERCLKEN0_ADDR(base)                   ((base) + 0x008)

/* Register description: 外设时钟最终状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERSTAT0_UNION */
#define SOC_MEDIA2_CRG_PERSTAT0_ADDR(base)                    ((base) + 0x00C)

/* Register description: 外设软复位使能寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTEN0_UNION */
#define SOC_MEDIA2_CRG_PERRSTEN0_ADDR(base)                   ((base) + 0x030)

/* Register description: 外设软复位撤离寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTDIS0_UNION */
#define SOC_MEDIA2_CRG_PERRSTDIS0_ADDR(base)                  ((base) + 0x034)

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTSTAT0_UNION */
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ADDR(base)                 ((base) + 0x038)

/* Register description: 时钟分频比控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV0_UNION */
#define SOC_MEDIA2_CRG_CLKDIV0_ADDR(base)                     ((base) + 0x080)

/* Register description: 时钟分频比控制寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV1_UNION */
#define SOC_MEDIA2_CRG_CLKDIV1_ADDR(base)                     ((base) + 0x084)

/* Register description: 时钟分频比控制寄存器2。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV2_UNION */
#define SOC_MEDIA2_CRG_CLKDIV2_ADDR(base)                     ((base) + 0x088)

/* Register description: 时钟分频比控制寄存器3。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV3_UNION */
#define SOC_MEDIA2_CRG_CLKDIV3_ADDR(base)                     ((base) + 0x08C)

/* Register description: 时钟分频比控制寄存器4。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV4_UNION */
#define SOC_MEDIA2_CRG_CLKDIV4_ADDR(base)                     ((base) + 0x090)

/* Register description: 时钟分频比控制寄存器5。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV5_UNION */
#define SOC_MEDIA2_CRG_CLKDIV5_ADDR(base)                     ((base) + 0x094)

/* Register description: 时钟分频比控制寄存器6。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV6_UNION */
#define SOC_MEDIA2_CRG_CLKDIV6_ADDR(base)                     ((base) + 0x098)

/* Register description: 时钟分频比控制寄存器7。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV7_UNION */
#define SOC_MEDIA2_CRG_CLKDIV7_ADDR(base)                     ((base) + 0x09C)

/* Register description: 时钟分频比控制寄存器8。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV8_UNION */
#define SOC_MEDIA2_CRG_CLKDIV8_ADDR(base)                     ((base) + 0x0A0)

/* Register description: 时钟分频比控制寄存器9。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV9_UNION */
#define SOC_MEDIA2_CRG_CLKDIV9_ADDR(base)                     ((base) + 0x0A4)

/* Register description: 时钟分频比控制寄存器10。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV10_UNION */
#define SOC_MEDIA2_CRG_CLKDIV10_ADDR(base)                    ((base) + 0x0A8)

/* Register description: 时钟分频比控制寄存器11。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV11_UNION */
#define SOC_MEDIA2_CRG_CLKDIV11_ADDR(base)                    ((base) + 0x0AC)

/* Register description: 时钟分频比控制寄存器12。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV12_UNION */
#define SOC_MEDIA2_CRG_CLKDIV12_ADDR(base)                    ((base) + 0x0B0)

/* Register description: 时钟分频比控制寄存器13。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV13_UNION */
#define SOC_MEDIA2_CRG_CLKDIV13_ADDR(base)                    ((base) + 0x0B4)

/* Register description: 时钟分频比控制寄存器14。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV14_UNION */
#define SOC_MEDIA2_CRG_CLKDIV14_ADDR(base)                    ((base) + 0x0B8)

/* Register description: 时钟分频比控制寄存器15。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV15_UNION */
#define SOC_MEDIA2_CRG_CLKDIV15_ADDR(base)                    ((base) + 0x0BC)

/* Register description: 时钟分频比控制寄存器16。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV16_UNION */
#define SOC_MEDIA2_CRG_CLKDIV16_ADDR(base)                    ((base) + 0x0C0)

/* Register description: 时钟分频比控制寄存器17。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV17_UNION */
#define SOC_MEDIA2_CRG_CLKDIV17_ADDR(base)                    ((base) + 0x0C4)

/* Register description: 时钟分频比控制寄存器18。
            电池欠压降频控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV18_UNION */
#define SOC_MEDIA2_CRG_CLKDIV18_ADDR(base)                    ((base) + 0x0C8)

/* Register description: 时钟分频比控制寄存器19。
            电池欠压降频控制寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_CLKDIV19_UNION */
#define SOC_MEDIA2_CRG_CLKDIV19_ADDR(base)                    ((base) + 0x0CC)

/* Register description: 外设状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_STAT0_UNION */
#define SOC_MEDIA2_CRG_PERI_STAT0_ADDR(base)                  ((base) + 0x120)

/* Register description: 外设状态寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_STAT1_UNION */
#define SOC_MEDIA2_CRG_PERI_STAT1_ADDR(base)                  ((base) + 0x124)

/* Register description: 外设状态寄存器2。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_STAT2_UNION */
#define SOC_MEDIA2_CRG_PERI_STAT2_ADDR(base)                  ((base) + 0x128)

/* Register description: ISPCPU子系统时钟使能寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_CLKEN_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ADDR(base)                ((base) + 0x140)

/* Register description: ISPCPU子系统软复位使能寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_RSTEN_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ADDR(base)                ((base) + 0x144)

/* Register description: ISPCPU子系统软复位撤离寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_RSTDIS_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ADDR(base)               ((base) + 0x148)

/* Register description: ISPCPU子系统软复位使能状态寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ADDR(base)              ((base) + 0x14C)

/* Register description: ISPCPU状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_CPU_STATE0_UNION */
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_ADDR(base)              ((base) + 0x150)

/* Register description: ISPCPU状态寄存器1。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_CPU_STATE1_UNION */
#define SOC_MEDIA2_CRG_ISP_CPU_STATE1_ADDR(base)              ((base) + 0x154)

/* Register description: 外设自动降频控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_AUTODIV0_UNION */
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_ADDR(base)               ((base) + 0x160)

/* Register description: 外设自动降频控制寄存器3。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_AUTODIV1_UNION */
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ADDR(base)               ((base) + 0x16c)

/* Register description: 外设自动降频控制寄存器4。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_ADDR(base) ((base) + 0x170)

/* Register description: 外设自动降频控制寄存器5。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_ADDR(base) ((base) + 0x174)

/* Register description: 外设自动降频控制寄存器6。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_ADDR(base) ((base) + 0x178)

/* Register description: 外设自动降频控制寄存器11。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_ADDR(base) ((base) + 0x18c)

/* Register description: 外设自动降频控制寄存器12。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_ADDR(base) ((base) + 0x190)

/* Register description: 外设自动降频控制寄存器13。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_UNION */
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_ADDR(base) ((base) + 0x194)

/* Register description: 自动降频状态寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_UNION */
#define SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_ADDR(base)          ((base) + 0x198)

/* Register description: ISP的状态回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_UNION */
#define SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_ADDR(base)        ((base) + 0x19c)

/* Register description: 外设自动降频状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_UNION */
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_ADDR(base)           ((base) + 0x1A0)

/* Register description: 总线异常中断屏蔽与清除寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_UNION */
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_ADDR(base)       ((base) + 0x1D0)

/* Register description: 总线异常中断状态寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_UNION */
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_ADDR(base)  ((base) + 0x1D4)

/* Register description: 外设IP防挂死功能byapss控制寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_IPCLKRST_BYPASS_UNION */
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_ADDR(base)             ((base) + 0x1D8)

/* Register description: hpm配置寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPM_CFG_UNION */
#define SOC_MEDIA2_CRG_HPM_CFG_ADDR(base)                     ((base) + 0x200)

/* Register description: hpm回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPM_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_HPM_READ_BACK_ADDR(base)               ((base) + 0x204)

/* Register description: hpmx配置寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPMX_CFG_UNION */
#define SOC_MEDIA2_CRG_HPMX_CFG_ADDR(base)                    ((base) + 0x208)

/* Register description: hpmx回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HPMX_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_ADDR(base)              ((base) + 0x20C)

/* Register description: himinibus自动门控配置寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_UNION */
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_ADDR(base)            ((base) + 0x400)

/* Register description: himinibus idle回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_ADDR(base)    ((base) + 0x404)

/* Register description: isp回读寄存器0
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_READ_BACK0_UNION */
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_ADDR(base)              ((base) + 0x408)

/* Register description: isp回读寄存器1
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_READ_BACK1_UNION */
#define SOC_MEDIA2_CRG_ISP_READ_BACK1_ADDR(base)              ((base) + 0x40C)

/* Register description: isp himinibus控制寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_UNION */
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_ADDR(base)          ((base) + 0x410)

/* Register description: himinibus dlock clr寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_UNION */
#define SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_ADDR(base)         ((base) + 0x414)

/* Register description: ipp 2to3桥回读寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_IPP_READ_BACK_UNION */
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ADDR(base)               ((base) + 0x418)

/* Register description: 通用安全软复位使能寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ADDR(base)        ((base) + 0xC00)

/* Register description: 通用安全软复位撤离寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ADDR(base)       ((base) + 0xC04)

/* Register description: 通用安全软复位使能状态寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ADDR(base)      ((base) + 0xC08)

/* Register description: 外设时钟安全使能寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_ADDR(base)          ((base) + 0xC30)

/* Register description: 外设时钟安全禁止寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_ADDR(base)         ((base) + 0xC34)

/* Register description: 外设时钟安全使能状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_ADDR(base)       ((base) + 0xC38)

/* Register description: 外设时钟安全最终状态寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_UNION */
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_ADDR(base)        ((base) + 0xC3C)

/* Register description: ISP安全软复位使能寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ADDR(base)            ((base) + 0xD00)

/* Register description: ISP安全软复位撤离寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ADDR(base)           ((base) + 0xD04)

/* Register description: ISP安全软复位状态寄存器。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_UNION */
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ADDR(base)          ((base) + 0xD08)

/* Register description: ISPCPU控制配置寄存器0。
   Bit domain definition UNION:  SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_UNION */
#define SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_ADDR(base)            ((base) + 0xD10)

/* Register description: 绝对安全寄存器
   Bit domain definition UNION:  SOC_MEDIA2_CRG_DECPROT0SET_UNION */
#define SOC_MEDIA2_CRG_DECPROT0SET_ADDR(base)                 ((base) + 0xF00)


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
 struct               : SOC_MEDIA2_CRG_PEREN0_UNION
 struct description   : PEREN0 Register structure definition
                        Address Offset:0x000 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_hpm                   : 1;  /* bit[0] :  */
        unsigned int  gt_clk_csi_idi               : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_atgm_isp             : 1;  /* bit[2] :  */
        unsigned int  gt_atclk_isp_cpu             : 1;  /* bit[3] :  */
        unsigned int  gt_clk_isp_in_sys            : 1;  /* bit[4] :  */
        unsigned int  gt_pclk_ipp                  : 1;  /* bit[5] :  */
        unsigned int  gt_pclk_p2j                  : 1;  /* bit[6] :  */
        unsigned int  gt_pclk_csi_wrapper_cfg      : 1;  /* bit[7] :  */
        unsigned int  gt_pclk_isp_qic_subsys       : 1;  /* bit[8] :  */
        unsigned int  gt_clk_qic_tcu_m2            : 1;  /* bit[9] :  */
        unsigned int  gt_aclk_isp_qic_subsys       : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_m2                : 1;  /* bit[11]:  */
        unsigned int  gt_clk_isp_i3c               : 1;  /* bit[12]:  */
        unsigned int  gt_clk_ipp_func              : 1;  /* bit[13]:  */
        unsigned int  gt_clk_isp_cpu               : 1;  /* bit[14]:  */
        unsigned int  gt_clk_isp_func              : 1;  /* bit[15]:  */
        unsigned int  gt_clk_isp_i2c               : 1;  /* bit[16]:  */
        unsigned int  gt_aclk_ipp                  : 1;  /* bit[17]:  */
        unsigned int  gt_aclk_isp                  : 1;  /* bit[18]:  */
        unsigned int  gt_aclk_qic_isp              : 1;  /* bit[19]:  */
        unsigned int  gt_clk_qice_isp              : 1;  /* bit[20]:  */
        unsigned int  gt_clk_qice_data_media2      : 1;  /* bit[21]:  */
        unsigned int  gt_pclk_autodiv_aclk_qic_tcu : 1;  /* bit[22]:  */
        unsigned int  gt_clk_miniisp_csi           : 1;  /* bit[23]:  */
        unsigned int  gt_pclk_dbg                  : 1;  /* bit[24]:  */
        unsigned int  gt_clk_isp_cputocfg          : 1;  /* bit[25]:  */
        unsigned int  gt_pclk_qic_isp              : 1;  /* bit[26]:  */
        unsigned int  reserved_0                   : 1;  /* bit[27]:  */
        unsigned int  reserved_1                   : 1;  /* bit[28]:  */
        unsigned int  reserved_2                   : 1;  /* bit[29]:  */
        unsigned int  reserved_3                   : 1;  /* bit[30]:  */
        unsigned int  reserved_4                   : 1;  /* bit[31]: 外设时钟使能控制：
                                                                     0：写0无效果；
                                                                     1：使能IP时钟。 */
    } reg;
} SOC_MEDIA2_CRG_PEREN0_UNION;
#endif
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_hpm_START                    (0)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_hpm_END                      (0)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_csi_idi_START                (1)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_csi_idi_END                  (1)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_atgm_isp_START              (2)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_atgm_isp_END                (2)
#define SOC_MEDIA2_CRG_PEREN0_gt_atclk_isp_cpu_START              (3)
#define SOC_MEDIA2_CRG_PEREN0_gt_atclk_isp_cpu_END                (3)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_in_sys_START             (4)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_in_sys_END               (4)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_ipp_START                   (5)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_ipp_END                     (5)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_p2j_START                   (6)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_p2j_END                     (6)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_csi_wrapper_cfg_START       (7)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_csi_wrapper_cfg_END         (7)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_isp_qic_subsys_START        (8)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_isp_qic_subsys_END          (8)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qic_tcu_m2_START             (9)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qic_tcu_m2_END               (9)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_isp_qic_subsys_START        (10)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_isp_qic_subsys_END          (10)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qic_m2_START                 (11)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qic_m2_END                   (11)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_i3c_START                (12)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_i3c_END                  (12)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ipp_func_START               (13)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ipp_func_END                 (13)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_cpu_START                (14)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_cpu_END                  (14)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_func_START               (15)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_func_END                 (15)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_i2c_START                (16)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_i2c_END                  (16)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_ipp_START                   (17)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_ipp_END                     (17)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_isp_START                   (18)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_isp_END                     (18)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_qic_isp_START               (19)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_qic_isp_END                 (19)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qice_isp_START               (20)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qice_isp_END                 (20)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qice_data_media2_START       (21)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qice_data_media2_END         (21)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_aclk_qic_tcu_START  (22)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_aclk_qic_tcu_END    (22)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_miniisp_csi_START            (23)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_miniisp_csi_END              (23)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_dbg_START                   (24)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_dbg_END                     (24)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_cputocfg_START           (25)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_isp_cputocfg_END             (25)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_qic_isp_START               (26)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_qic_isp_END                 (26)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERDIS0_UNION
 struct description   : PERDIS0 Register structure definition
                        Address Offset:0x004 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_hpm                   : 1;  /* bit[0] :  */
        unsigned int  gt_clk_csi_idi               : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_atgm_isp             : 1;  /* bit[2] :  */
        unsigned int  gt_atclk_isp_cpu             : 1;  /* bit[3] :  */
        unsigned int  gt_clk_isp_in_sys            : 1;  /* bit[4] :  */
        unsigned int  gt_pclk_ipp                  : 1;  /* bit[5] :  */
        unsigned int  gt_pclk_p2j                  : 1;  /* bit[6] :  */
        unsigned int  gt_pclk_csi_wrapper_cfg      : 1;  /* bit[7] :  */
        unsigned int  gt_pclk_isp_qic_subsys       : 1;  /* bit[8] :  */
        unsigned int  gt_clk_qic_tcu_m2            : 1;  /* bit[9] :  */
        unsigned int  gt_aclk_isp_qic_subsys       : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_m2                : 1;  /* bit[11]:  */
        unsigned int  gt_clk_isp_i3c               : 1;  /* bit[12]:  */
        unsigned int  gt_clk_ipp_func              : 1;  /* bit[13]:  */
        unsigned int  gt_clk_isp_cpu               : 1;  /* bit[14]:  */
        unsigned int  gt_clk_isp_func              : 1;  /* bit[15]:  */
        unsigned int  gt_clk_isp_i2c               : 1;  /* bit[16]:  */
        unsigned int  gt_aclk_ipp                  : 1;  /* bit[17]:  */
        unsigned int  gt_aclk_isp                  : 1;  /* bit[18]:  */
        unsigned int  gt_aclk_qic_isp              : 1;  /* bit[19]:  */
        unsigned int  gt_clk_qice_isp              : 1;  /* bit[20]:  */
        unsigned int  gt_clk_qice_data_media2      : 1;  /* bit[21]:  */
        unsigned int  gt_pclk_autodiv_aclk_qic_tcu : 1;  /* bit[22]:  */
        unsigned int  gt_clk_miniisp_csi           : 1;  /* bit[23]:  */
        unsigned int  gt_pclk_dbg                  : 1;  /* bit[24]:  */
        unsigned int  gt_clk_isp_cputocfg          : 1;  /* bit[25]:  */
        unsigned int  gt_pclk_qic_isp              : 1;  /* bit[26]:  */
        unsigned int  reserved_0                   : 1;  /* bit[27]:  */
        unsigned int  reserved_1                   : 1;  /* bit[28]:  */
        unsigned int  reserved_2                   : 1;  /* bit[29]:  */
        unsigned int  reserved_3                   : 1;  /* bit[30]:  */
        unsigned int  reserved_4                   : 1;  /* bit[31]: 外设时钟禁止控制：
                                                                     0：写0无效果；
                                                                     1：禁止IP时钟。 */
    } reg;
} SOC_MEDIA2_CRG_PERDIS0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_hpm_START                    (0)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_hpm_END                      (0)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_csi_idi_START                (1)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_csi_idi_END                  (1)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_atgm_isp_START              (2)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_atgm_isp_END                (2)
#define SOC_MEDIA2_CRG_PERDIS0_gt_atclk_isp_cpu_START              (3)
#define SOC_MEDIA2_CRG_PERDIS0_gt_atclk_isp_cpu_END                (3)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_in_sys_START             (4)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_in_sys_END               (4)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_ipp_START                   (5)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_ipp_END                     (5)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_p2j_START                   (6)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_p2j_END                     (6)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_csi_wrapper_cfg_START       (7)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_csi_wrapper_cfg_END         (7)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_isp_qic_subsys_START        (8)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_isp_qic_subsys_END          (8)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qic_tcu_m2_START             (9)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qic_tcu_m2_END               (9)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_isp_qic_subsys_START        (10)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_isp_qic_subsys_END          (10)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qic_m2_START                 (11)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qic_m2_END                   (11)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_i3c_START                (12)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_i3c_END                  (12)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ipp_func_START               (13)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ipp_func_END                 (13)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_cpu_START                (14)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_cpu_END                  (14)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_func_START               (15)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_func_END                 (15)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_i2c_START                (16)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_i2c_END                  (16)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_ipp_START                   (17)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_ipp_END                     (17)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_isp_START                   (18)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_isp_END                     (18)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_qic_isp_START               (19)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_qic_isp_END                 (19)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qice_isp_START               (20)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qice_isp_END                 (20)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qice_data_media2_START       (21)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qice_data_media2_END         (21)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_aclk_qic_tcu_START  (22)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_aclk_qic_tcu_END    (22)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_miniisp_csi_START            (23)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_miniisp_csi_END              (23)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_dbg_START                   (24)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_dbg_END                     (24)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_cputocfg_START           (25)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_isp_cputocfg_END             (25)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_qic_isp_START               (26)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_qic_isp_END                 (26)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERCLKEN0_UNION
 struct description   : PERCLKEN0 Register structure definition
                        Address Offset:0x008 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_hpm                   : 1;  /* bit[0] :  */
        unsigned int  gt_clk_csi_idi               : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_atgm_isp             : 1;  /* bit[2] :  */
        unsigned int  gt_atclk_isp_cpu             : 1;  /* bit[3] :  */
        unsigned int  gt_clk_isp_in_sys            : 1;  /* bit[4] :  */
        unsigned int  gt_pclk_ipp                  : 1;  /* bit[5] :  */
        unsigned int  gt_pclk_p2j                  : 1;  /* bit[6] :  */
        unsigned int  gt_pclk_csi_wrapper_cfg      : 1;  /* bit[7] :  */
        unsigned int  gt_pclk_isp_qic_subsys       : 1;  /* bit[8] :  */
        unsigned int  gt_clk_qic_tcu_m2            : 1;  /* bit[9] :  */
        unsigned int  gt_aclk_isp_qic_subsys       : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_m2                : 1;  /* bit[11]:  */
        unsigned int  gt_clk_isp_i3c               : 1;  /* bit[12]:  */
        unsigned int  gt_clk_ipp_func              : 1;  /* bit[13]:  */
        unsigned int  gt_clk_isp_cpu               : 1;  /* bit[14]:  */
        unsigned int  gt_clk_isp_func              : 1;  /* bit[15]:  */
        unsigned int  gt_clk_isp_i2c               : 1;  /* bit[16]:  */
        unsigned int  gt_aclk_ipp                  : 1;  /* bit[17]:  */
        unsigned int  gt_aclk_isp                  : 1;  /* bit[18]:  */
        unsigned int  gt_aclk_qic_isp              : 1;  /* bit[19]:  */
        unsigned int  gt_clk_qice_isp              : 1;  /* bit[20]:  */
        unsigned int  gt_clk_qice_data_media2      : 1;  /* bit[21]:  */
        unsigned int  gt_pclk_autodiv_aclk_qic_tcu : 1;  /* bit[22]:  */
        unsigned int  gt_clk_miniisp_csi           : 1;  /* bit[23]:  */
        unsigned int  gt_pclk_dbg                  : 1;  /* bit[24]:  */
        unsigned int  gt_clk_isp_cputocfg          : 1;  /* bit[25]:  */
        unsigned int  gt_pclk_qic_isp              : 1;  /* bit[26]:  */
        unsigned int  reserved_0                   : 1;  /* bit[27]:  */
        unsigned int  reserved_1                   : 1;  /* bit[28]:  */
        unsigned int  reserved_2                   : 1;  /* bit[29]:  */
        unsigned int  reserved_3                   : 1;  /* bit[30]:  */
        unsigned int  reserved_4                   : 1;  /* bit[31]: 外设时钟使能状态：
                                                                     0：IP时钟使能撤销状态；
                                                                     1：IP时钟使能状态。 */
    } reg;
} SOC_MEDIA2_CRG_PERCLKEN0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_hpm_START                    (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_hpm_END                      (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_csi_idi_START                (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_csi_idi_END                  (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_atgm_isp_START              (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_atgm_isp_END                (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_atclk_isp_cpu_START              (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_atclk_isp_cpu_END                (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_in_sys_START             (4)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_in_sys_END               (4)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_ipp_START                   (5)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_ipp_END                     (5)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_p2j_START                   (6)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_p2j_END                     (6)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_csi_wrapper_cfg_START       (7)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_csi_wrapper_cfg_END         (7)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_isp_qic_subsys_START        (8)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_isp_qic_subsys_END          (8)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qic_tcu_m2_START             (9)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qic_tcu_m2_END               (9)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_isp_qic_subsys_START        (10)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_isp_qic_subsys_END          (10)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qic_m2_START                 (11)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qic_m2_END                   (11)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_i3c_START                (12)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_i3c_END                  (12)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ipp_func_START               (13)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ipp_func_END                 (13)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_cpu_START                (14)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_cpu_END                  (14)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_func_START               (15)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_func_END                 (15)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_i2c_START                (16)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_i2c_END                  (16)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_ipp_START                   (17)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_ipp_END                     (17)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_isp_START                   (18)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_isp_END                     (18)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_qic_isp_START               (19)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_qic_isp_END                 (19)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qice_isp_START               (20)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qice_isp_END                 (20)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qice_data_media2_START       (21)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qice_data_media2_END         (21)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_aclk_qic_tcu_START  (22)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_aclk_qic_tcu_END    (22)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_miniisp_csi_START            (23)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_miniisp_csi_END              (23)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_dbg_START                   (24)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_dbg_END                     (24)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_cputocfg_START           (25)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_isp_cputocfg_END             (25)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_qic_isp_START               (26)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_qic_isp_END                 (26)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERSTAT0_UNION
 struct description   : PERSTAT0 Register structure definition
                        Address Offset:0x00C Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_hpm                   : 1;  /* bit[0] : Charlotte 不支持 */
        unsigned int  st_clk_csi_idi               : 1;  /* bit[1] :  */
        unsigned int  st_pclk_atgm_isp             : 1;  /* bit[2] :  */
        unsigned int  st_atclk_isp_cpu             : 1;  /* bit[3] :  */
        unsigned int  st_clk_isp_in_sys            : 1;  /* bit[4] :  */
        unsigned int  st_pclk_ipp                  : 1;  /* bit[5] :  */
        unsigned int  st_pclk_p2j                  : 1;  /* bit[6] :  */
        unsigned int  st_pclk_csi_wrapper_cfg      : 1;  /* bit[7] :  */
        unsigned int  st_pclk_isp_qic_subsys       : 1;  /* bit[8] :  */
        unsigned int  st_clk_qic_tcu_m2            : 1;  /* bit[9] :  */
        unsigned int  st_aclk_isp_qic_subsys       : 1;  /* bit[10]:  */
        unsigned int  st_clk_qic_m2                : 1;  /* bit[11]:  */
        unsigned int  st_clk_isp_i3c               : 1;  /* bit[12]:  */
        unsigned int  st_clk_ipp_func              : 1;  /* bit[13]:  */
        unsigned int  st_clk_isp_cpu               : 1;  /* bit[14]:  */
        unsigned int  st_clk_isp_func              : 1;  /* bit[15]:  */
        unsigned int  st_clk_isp_i2c               : 1;  /* bit[16]:  */
        unsigned int  st_aclk_ipp                  : 1;  /* bit[17]:  */
        unsigned int  st_aclk_isp                  : 1;  /* bit[18]:  */
        unsigned int  st_aclk_qic_isp              : 1;  /* bit[19]:  */
        unsigned int  st_clk_qice_isp              : 1;  /* bit[20]:  */
        unsigned int  st_clk_qice_data_media2      : 1;  /* bit[21]:  */
        unsigned int  st_pclk_autodiv_aclk_qic_tcu : 1;  /* bit[22]:  */
        unsigned int  st_clk_miniisp_csi           : 1;  /* bit[23]:  */
        unsigned int  st_pclk_dbg                  : 1;  /* bit[24]:  */
        unsigned int  st_clk_isp_cputocfg          : 1;  /* bit[25]:  */
        unsigned int  st_pclk_qic_isp              : 1;  /* bit[26]:  */
        unsigned int  reserved_0                   : 1;  /* bit[27]:  */
        unsigned int  reserved_1                   : 1;  /* bit[28]:  */
        unsigned int  reserved_2                   : 1;  /* bit[29]:  */
        unsigned int  reserved_3                   : 1;  /* bit[30]:  */
        unsigned int  reserved_4                   : 1;  /* bit[31]: 外设时钟最终状态：
                                                                     0：IP时钟禁止状态；
                                                                     1：IP时钟使能状态。 */
    } reg;
} SOC_MEDIA2_CRG_PERSTAT0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_hpm_START                    (0)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_hpm_END                      (0)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_csi_idi_START                (1)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_csi_idi_END                  (1)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_atgm_isp_START              (2)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_atgm_isp_END                (2)
#define SOC_MEDIA2_CRG_PERSTAT0_st_atclk_isp_cpu_START              (3)
#define SOC_MEDIA2_CRG_PERSTAT0_st_atclk_isp_cpu_END                (3)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_in_sys_START             (4)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_in_sys_END               (4)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_ipp_START                   (5)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_ipp_END                     (5)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_p2j_START                   (6)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_p2j_END                     (6)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_csi_wrapper_cfg_START       (7)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_csi_wrapper_cfg_END         (7)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_isp_qic_subsys_START        (8)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_isp_qic_subsys_END          (8)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qic_tcu_m2_START             (9)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qic_tcu_m2_END               (9)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_isp_qic_subsys_START        (10)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_isp_qic_subsys_END          (10)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qic_m2_START                 (11)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qic_m2_END                   (11)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_i3c_START                (12)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_i3c_END                  (12)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ipp_func_START               (13)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ipp_func_END                 (13)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_cpu_START                (14)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_cpu_END                  (14)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_func_START               (15)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_func_END                 (15)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_i2c_START                (16)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_i2c_END                  (16)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_ipp_START                   (17)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_ipp_END                     (17)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_isp_START                   (18)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_isp_END                     (18)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_qic_isp_START               (19)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_qic_isp_END                 (19)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qice_isp_START               (20)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qice_isp_END                 (20)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qice_data_media2_START       (21)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qice_data_media2_END         (21)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_aclk_qic_tcu_START  (22)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_aclk_qic_tcu_END    (22)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_miniisp_csi_START            (23)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_miniisp_csi_END              (23)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_dbg_START                   (24)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_dbg_END                     (24)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_cputocfg_START           (25)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_isp_cputocfg_END             (25)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_qic_isp_START               (26)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_qic_isp_END                 (26)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTEN0_UNION
 struct description   : PERRSTEN0 Register structure definition
                        Address Offset:0x030 Initial:0x000 Width:32
 register description : 外设软复位使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_prst_atgm_isp       : 1;  /* bit[0]    :  */
        unsigned int  ip_rst_qice_data       : 1;  /* bit[1]    :  */
        unsigned int  ip_rst_media2_qic_data : 1;  /* bit[2]    :  */
        unsigned int  ip_rst_tcu             : 1;  /* bit[3]    :  */
        unsigned int  ip_rst_hpm             : 1;  /* bit[4]    :  */
        unsigned int  ip_rst_p2j             : 1;  /* bit[5]    :  */
        unsigned int  ip_rst_dbg_cs          : 1;  /* bit[6]    :  */
        unsigned int  reserved_0             : 1;  /* bit[7]    :  */
        unsigned int  reserved_1             : 1;  /* bit[8]    :  */
        unsigned int  reserved_2             : 1;  /* bit[9]    :  */
        unsigned int  reserved_3             : 1;  /* bit[10]   :  */
        unsigned int  reserved_4             : 1;  /* bit[11]   :  */
        unsigned int  reserved_5             : 1;  /* bit[12]   :  */
        unsigned int  reserved_6             : 1;  /* bit[13]   :  */
        unsigned int  reserved_7             : 1;  /* bit[14]   :  */
        unsigned int  reserved_8             : 1;  /* bit[15]   :  */
        unsigned int  reserved_9             : 1;  /* bit[16]   :  */
        unsigned int  reserved_10            : 1;  /* bit[17]   :  */
        unsigned int  reserved_11            : 1;  /* bit[18]   :  */
        unsigned int  reserved_12            : 1;  /* bit[19]   :  */
        unsigned int  reserved_13            : 12; /* bit[20-31]: IP软复位使能：
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_isp_START        (0)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_isp_END          (0)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_qice_data_START        (1)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_qice_data_END          (1)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_media2_qic_data_START  (2)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_media2_qic_data_END    (2)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_tcu_START              (3)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_tcu_END                (3)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_hpm_START              (4)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_hpm_END                (4)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_p2j_START              (5)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_p2j_END                (5)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_dbg_cs_START           (6)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_dbg_cs_END             (6)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTDIS0_UNION
 struct description   : PERRSTDIS0 Register structure definition
                        Address Offset:0x034 Initial:0x000 Width:32
 register description : 外设软复位撤离寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_prst_atgm_isp       : 1;  /* bit[0]    :  */
        unsigned int  ip_rst_qice_data       : 1;  /* bit[1]    :  */
        unsigned int  ip_rst_media2_qic_data : 1;  /* bit[2]    :  */
        unsigned int  ip_rst_tcu             : 1;  /* bit[3]    :  */
        unsigned int  ip_rst_hpm             : 1;  /* bit[4]    :  */
        unsigned int  ip_rst_p2j             : 1;  /* bit[5]    :  */
        unsigned int  ip_rst_dbg_cs          : 1;  /* bit[6]    :  */
        unsigned int  reserved_0             : 1;  /* bit[7]    :  */
        unsigned int  reserved_1             : 1;  /* bit[8]    :  */
        unsigned int  reserved_2             : 1;  /* bit[9]    :  */
        unsigned int  reserved_3             : 1;  /* bit[10]   :  */
        unsigned int  reserved_4             : 1;  /* bit[11]   :  */
        unsigned int  reserved_5             : 1;  /* bit[12]   :  */
        unsigned int  reserved_6             : 1;  /* bit[13]   :  */
        unsigned int  reserved_7             : 1;  /* bit[14]   :  */
        unsigned int  reserved_8             : 1;  /* bit[15]   :  */
        unsigned int  reserved_9             : 1;  /* bit[16]   :  */
        unsigned int  reserved_10            : 1;  /* bit[17]   :  */
        unsigned int  reserved_11            : 1;  /* bit[18]   :  */
        unsigned int  reserved_12            : 1;  /* bit[19]   :  */
        unsigned int  reserved_13            : 12; /* bit[20-31]: IP软复位撤离：
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位撤离。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_isp_START        (0)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_isp_END          (0)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_qice_data_START        (1)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_qice_data_END          (1)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_media2_qic_data_START  (2)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_media2_qic_data_END    (2)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_tcu_START              (3)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_tcu_END                (3)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_hpm_START              (4)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_hpm_END                (4)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_p2j_START              (5)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_p2j_END                (5)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_dbg_cs_START           (6)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_dbg_cs_END             (6)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTSTAT0_UNION
 struct description   : PERRSTSTAT0 Register structure definition
                        Address Offset:0x038 Initial:0x000 Width:32
 register description : 外设软复位状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_prst_atgm_isp       : 1;  /* bit[0]    :  */
        unsigned int  ip_rst_qice_data       : 1;  /* bit[1]    :  */
        unsigned int  ip_rst_media2_qic_data : 1;  /* bit[2]    :  */
        unsigned int  ip_rst_tcu             : 1;  /* bit[3]    :  */
        unsigned int  ip_rst_hpm             : 1;  /* bit[4]    :  */
        unsigned int  ip_rst_p2j             : 1;  /* bit[5]    :  */
        unsigned int  ip_rst_dbg_cs          : 1;  /* bit[6]    :  */
        unsigned int  reserved_0             : 1;  /* bit[7]    :  */
        unsigned int  reserved_1             : 1;  /* bit[8]    :  */
        unsigned int  reserved_2             : 1;  /* bit[9]    :  */
        unsigned int  reserved_3             : 1;  /* bit[10]   :  */
        unsigned int  reserved_4             : 1;  /* bit[11]   :  */
        unsigned int  reserved_5             : 1;  /* bit[12]   :  */
        unsigned int  reserved_6             : 1;  /* bit[13]   :  */
        unsigned int  reserved_7             : 1;  /* bit[14]   :  */
        unsigned int  reserved_8             : 1;  /* bit[15]   :  */
        unsigned int  reserved_9             : 1;  /* bit[16]   :  */
        unsigned int  reserved_10            : 1;  /* bit[17]   :  */
        unsigned int  reserved_11            : 1;  /* bit[18]   :  */
        unsigned int  reserved_12            : 1;  /* bit[19]   :  */
        unsigned int  reserved_13            : 12; /* bit[20-31]: IP软复位使能状态：
                                                                  0：IP处于复位撤离状态；
                                                                  1：IP处于软复位使能状态。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_isp_START        (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_isp_END          (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_qice_data_START        (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_qice_data_END          (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_media2_qic_data_START  (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_media2_qic_data_END    (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_tcu_START              (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_tcu_END                (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_hpm_START              (4)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_hpm_END                (4)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_p2j_START              (5)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_p2j_END                (5)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_dbg_cs_START           (6)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_dbg_cs_END             (6)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV0_UNION
 struct description   : CLKDIV0 Register structure definition
                        Address Offset:0x080 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sel_isp_func_pll : 4;  /* bit[0-3]  : isp func时钟源PLL选择控制：
                                                            4'b0001：PPLL2_B
                                                            4'b0010：PPLL0
                                                            4'b0100：PPLL1
                                                            4'b1000：PPLL2 */
        unsigned int  reserved_0       : 4;  /* bit[4-7]  : reserved */
        unsigned int  reserved_1       : 4;  /* bit[8-11] : reserved */
        unsigned int  reserved_2       : 4;  /* bit[12-15]: reserved */
        unsigned int  bitmasken        : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                            只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV0_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV0_sel_isp_func_pll_START  (0)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_isp_func_pll_END    (3)
#define SOC_MEDIA2_CRG_CLKDIV0_bitmasken_START         (16)
#define SOC_MEDIA2_CRG_CLKDIV0_bitmasken_END           (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV1_UNION
 struct description   : CLKDIV1 Register structure definition
                        Address Offset:0x084 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sel_miniisp_pll      : 4;  /* bit[0-3]  : clk_miniisp工作时钟时钟源PLL选择控制：100 03新增
                                                                
                                                                4'b0001：PPLL0
                                                                4'b0010：ULPPLL
                                                                4'b0100：PPLL1
                                                                4'b1000：PPLL2 */
        unsigned int  sel_clk_csi_idi_pll  : 4;  /* bit[4-7]  : clk_csi_idi工作时钟时钟源PLL选择控制：
                                                                4'b0001：PPLL0
                                                                4'b0010：ULPPLL
                                                                4'b0100：PPLL1
                                                                4'b1000：PPLL2 */
        unsigned int  sel_clk_idi2axi_csid : 4;  /* bit[8-11] : clk_idi2axi_csid工作时钟时钟源PLL选择控制：
                                                                4'b0001：PPLL0
                                                                4'b0010：ULPPLL
                                                                4'b0100：PPLL1
                                                                4'b1000：PPLL2 */
        unsigned int  reserved             : 4;  /* bit[12-15]: reserved */
        unsigned int  bitmasken            : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV1_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV1_sel_miniisp_pll_START       (0)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_miniisp_pll_END         (3)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_clk_csi_idi_pll_START   (4)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_clk_csi_idi_pll_END     (7)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_clk_idi2axi_csid_START  (8)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_clk_idi2axi_csid_END    (11)
#define SOC_MEDIA2_CRG_CLKDIV1_bitmasken_START             (16)
#define SOC_MEDIA2_CRG_CLKDIV1_bitmasken_END               (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV2_UNION
 struct description   : CLKDIV2 Register structure definition
                        Address Offset:0x088 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sel_isp_cpu_pll     : 4;  /* bit[0-3]  : ISPCPU工作时钟时钟源PLL选择控制：
                                                               4'b0001：PPLL1
                                                               4'b0010：PPLL0
                                                               4'b0100：PPLL2
                                                               4'b1000：PPLL6 */
        unsigned int  sel_qic_m2_data_pll : 4;  /* bit[4-7]  : media2 QIC data工作时钟时钟源PLL选择控制：
                                                               4'b0001：PPLL2_B
                                                               4'b0010：ULPLL
                                                               4'b0100：PPLL0
                                                               4'b1000：PPLL1 */
        unsigned int  reserved_0          : 4;  /* bit[8-11] : reserved */
        unsigned int  reserved_1          : 4;  /* bit[12-15]: reserved */
        unsigned int  bitmasken           : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                               只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV2_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV2_sel_isp_cpu_pll_START      (0)
#define SOC_MEDIA2_CRG_CLKDIV2_sel_isp_cpu_pll_END        (3)
#define SOC_MEDIA2_CRG_CLKDIV2_sel_qic_m2_data_pll_START  (4)
#define SOC_MEDIA2_CRG_CLKDIV2_sel_qic_m2_data_pll_END    (7)
#define SOC_MEDIA2_CRG_CLKDIV2_bitmasken_START            (16)
#define SOC_MEDIA2_CRG_CLKDIV2_bitmasken_END              (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV3_UNION
 struct description   : CLKDIV3 Register structure definition
                        Address Offset:0x08C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sel_hpm_pll      : 4;  /* bit[0-3]  : clk_hpm工作时钟时钟源PLL选择控制：新增
                                                            4'b0001：PPLL2_B
                                                            4'b0010：PPLL0
                                                            4'b0100：PPLL1
                                                            4'b1000：PPLL2 */
        unsigned int  sel_ipp_func_pll : 4;  /* bit[4-7]  : clk_ipp工作时钟时钟源PLL选择控制：新增
                                                            4'b0001：PPLL2_B
                                                            4'b0010：PPLL0
                                                            4'b0100：PPLL1
                                                            4'b1000：PPLL2 */
        unsigned int  reserved_0       : 4;  /* bit[8-11] : reserved */
        unsigned int  reserved_1       : 4;  /* bit[12-15]: reserved */
        unsigned int  bitmasken        : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                            只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV3_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV3_sel_hpm_pll_START       (0)
#define SOC_MEDIA2_CRG_CLKDIV3_sel_hpm_pll_END         (3)
#define SOC_MEDIA2_CRG_CLKDIV3_sel_ipp_func_pll_START  (4)
#define SOC_MEDIA2_CRG_CLKDIV3_sel_ipp_func_pll_END    (7)
#define SOC_MEDIA2_CRG_CLKDIV3_bitmasken_START         (16)
#define SOC_MEDIA2_CRG_CLKDIV3_bitmasken_END           (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV4_UNION
 struct description   : CLKDIV4 Register structure definition
                        Address Offset:0x090 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 4;  /* bit[0-3]  : reserved */
        unsigned int  reserved_1: 4;  /* bit[4-7]  : reserved */
        unsigned int  reserved_2: 4;  /* bit[8-11] : reserved */
        unsigned int  reserved_3: 4;  /* bit[12-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV4_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV4_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV4_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV5_UNION
 struct description   : CLKDIV5 Register structure definition
                        Address Offset:0x094 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 4;  /* bit[0-3]  : reserved */
        unsigned int  reserved_1: 4;  /* bit[4-7]  : reserved */
        unsigned int  reserved_2: 4;  /* bit[8-11] : reserved */
        unsigned int  reserved_3: 4;  /* bit[12-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV5_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV5_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV5_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV6_UNION
 struct description   : CLKDIV6 Register structure definition
                        Address Offset:0x098 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  : reserved */
        unsigned int  reserved_1: 2;  /* bit[6-7]  : reserved */
        unsigned int  reserved_2: 6;  /* bit[8-13] : reserved */
        unsigned int  reserved_3: 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV6_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV6_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV6_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV7_UNION
 struct description   : CLKDIV7 Register structure definition
                        Address Offset:0x09C Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_qic_m2  : 6;  /* bit[0-5]  : qic m2工作时钟分频比选择：
                                                       6'h0:1；
                                                       6'h1:2；
                                                       …
                                                       6'h3F:64； */
        unsigned int  reserved_0  : 2;  /* bit[6-7]  : reserved */
        unsigned int  div_miniisp : 6;  /* bit[8-13] : miniisp工作时钟分频比选择：
                                                       6'h0:1；
                                                       6'h1:2；
                                                       …
                                                       6'h3F:64； */
        unsigned int  reserved_1  : 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken   : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                       只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV7_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV7_div_qic_m2_START   (0)
#define SOC_MEDIA2_CRG_CLKDIV7_div_qic_m2_END     (5)
#define SOC_MEDIA2_CRG_CLKDIV7_div_miniisp_START  (8)
#define SOC_MEDIA2_CRG_CLKDIV7_div_miniisp_END    (13)
#define SOC_MEDIA2_CRG_CLKDIV7_bitmasken_START    (16)
#define SOC_MEDIA2_CRG_CLKDIV7_bitmasken_END      (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV8_UNION
 struct description   : CLKDIV8 Register structure definition
                        Address Offset:0x0A0 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器8。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_clk_idi2axi_csid : 6;  /* bit[0-5]  : clk_idi2axi_csid工作时钟分频比选择：
                                                                6'h0:1；
                                                                6'h1:2；
                                                                …
                                                                6'h3F:64； */
        unsigned int  reserved_0           : 2;  /* bit[6-7]  : reserved */
        unsigned int  div_isp_i3c          : 6;  /* bit[8-13] : isp_i3c工作时钟分频比选择：
                                                                6'h0:1；
                                                                6'h1:2；
                                                                …
                                                                6'h3F:64； */
        unsigned int  reserved_1           : 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken            : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV8_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV8_div_clk_idi2axi_csid_START  (0)
#define SOC_MEDIA2_CRG_CLKDIV8_div_clk_idi2axi_csid_END    (5)
#define SOC_MEDIA2_CRG_CLKDIV8_div_isp_i3c_START           (8)
#define SOC_MEDIA2_CRG_CLKDIV8_div_isp_i3c_END             (13)
#define SOC_MEDIA2_CRG_CLKDIV8_bitmasken_START             (16)
#define SOC_MEDIA2_CRG_CLKDIV8_bitmasken_END               (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV9_UNION
 struct description   : CLKDIV9 Register structure definition
                        Address Offset:0x0A4 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器9。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  : reserved */
        unsigned int  reserved_1: 2;  /* bit[6-7]  : reserved */
        unsigned int  reserved_2: 6;  /* bit[8-13] : reserved */
        unsigned int  reserved_3: 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV9_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV9_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV9_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV10_UNION
 struct description   : CLKDIV10 Register structure definition
                        Address Offset:0x0A8 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器10。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_clk_isp_i2c : 6;  /* bit[0-5]  : clk_isp_i2c工作时钟分频比选择：
                                                           6'h0:1；
                                                           6'h1:2；
                                                           …
                                                           6'h3F:64； */
        unsigned int  reserved_0      : 2;  /* bit[6-7]  : reserved */
        unsigned int  div_clk_isp_cpu : 6;  /* bit[8-13] : clk_isp_cpu工作时钟分频比选择：
                                                           6'h0:1；
                                                           6'h1:2；
                                                           …
                                                           6'h3F:64； */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken       : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                           只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV10_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV10_div_clk_isp_i2c_START  (0)
#define SOC_MEDIA2_CRG_CLKDIV10_div_clk_isp_i2c_END    (5)
#define SOC_MEDIA2_CRG_CLKDIV10_div_clk_isp_cpu_START  (8)
#define SOC_MEDIA2_CRG_CLKDIV10_div_clk_isp_cpu_END    (13)
#define SOC_MEDIA2_CRG_CLKDIV10_bitmasken_START        (16)
#define SOC_MEDIA2_CRG_CLKDIV10_bitmasken_END          (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV11_UNION
 struct description   : CLKDIV11 Register structure definition
                        Address Offset:0x0AC Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器11。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_csi_idi  : 6;  /* bit[0-5]  : clk_csi_idi工作时钟分频比选择：
                                                        6'h0:1；
                                                        6'h1:2；
                                                        …
                                                        6'h3F:64； */
        unsigned int  reserved_0   : 2;  /* bit[6-7]  : reserved */
        unsigned int  div_isp_func : 6;  /* bit[8-13] : clk_isp_top工作时钟分频比选择：
                                                        6'h0:1；
                                                        6'h1:2；
                                                        …
                                                        6'h3F:64； */
        unsigned int  reserved_1   : 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken    : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                        只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV11_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV11_div_csi_idi_START   (0)
#define SOC_MEDIA2_CRG_CLKDIV11_div_csi_idi_END     (5)
#define SOC_MEDIA2_CRG_CLKDIV11_div_isp_func_START  (8)
#define SOC_MEDIA2_CRG_CLKDIV11_div_isp_func_END    (13)
#define SOC_MEDIA2_CRG_CLKDIV11_bitmasken_START     (16)
#define SOC_MEDIA2_CRG_CLKDIV11_bitmasken_END       (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV12_UNION
 struct description   : CLKDIV12 Register structure definition
                        Address Offset:0x0B0 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  div_hpm      : 6;  /* bit[0-5]  : hpm工作时钟分频比选择：新增
                                                        6'h0:1；
                                                        6'h1:2；
                                                        …
                                                        6'h3F:64； */
        unsigned int  reserved_0   : 2;  /* bit[6-7]  : reserved */
        unsigned int  div_ipp_func : 6;  /* bit[8-13] : ipp工作时钟分频比选择：新增
                                                        6'h0:1；
                                                        6'h1:2；
                                                        …
                                                        6'h3F:64； */
        unsigned int  reserved_1   : 2;  /* bit[14-15]:  */
        unsigned int  bitmasken    : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                        只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV12_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV12_div_hpm_START       (0)
#define SOC_MEDIA2_CRG_CLKDIV12_div_hpm_END         (5)
#define SOC_MEDIA2_CRG_CLKDIV12_div_ipp_func_START  (8)
#define SOC_MEDIA2_CRG_CLKDIV12_div_ipp_func_END    (13)
#define SOC_MEDIA2_CRG_CLKDIV12_bitmasken_START     (16)
#define SOC_MEDIA2_CRG_CLKDIV12_bitmasken_END       (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV13_UNION
 struct description   : CLKDIV13 Register structure definition
                        Address Offset:0x0B4 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器13。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  : reserved */
        unsigned int  reserved_1: 2;  /* bit[6-7]  : reserved */
        unsigned int  reserved_2: 6;  /* bit[8-13] : reserved */
        unsigned int  reserved_3: 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV13_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV13_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV13_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV14_UNION
 struct description   : CLKDIV14 Register structure definition
                        Address Offset:0x0B8 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器14。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  : reserved */
        unsigned int  reserved_1: 2;  /* bit[6-7]  : reserved */
        unsigned int  reserved_2: 6;  /* bit[8-13] : reserved */
        unsigned int  reserved_3: 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV14_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV14_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV14_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV15_UNION
 struct description   : CLKDIV15 Register structure definition
                        Address Offset:0x0BC Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器15。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 6;  /* bit[0-5]  : reserved */
        unsigned int  reserved_1: 2;  /* bit[6-7]  : reserved */
        unsigned int  reserved_2: 6;  /* bit[8-13] : reserved */
        unsigned int  reserved_3: 2;  /* bit[14-15]: reserved */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV15_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV15_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV15_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV16_UNION
 struct description   : CLKDIV16 Register structure definition
                        Address Offset:0x0C0 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器16。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_csi_idi      : 1;  /* bit[0]    :  */
        unsigned int  sc_gt_clk_i3c          : 1;  /* bit[1]    :  */
        unsigned int  sc_gt_clk_hpm          : 1;  /* bit[2]    :  */
        unsigned int  sc_gt_clk_miniisp      : 1;  /* bit[3]    :  */
        unsigned int  sc_gt_clk_isp_cpu      : 1;  /* bit[4]    :  */
        unsigned int  sc_gt_clk_qic_m2       : 1;  /* bit[5]    :  */
        unsigned int  sc_gt_clk_isp_i2c      : 1;  /* bit[6]    :  */
        unsigned int  sc_gt_clk_ipp_func     : 1;  /* bit[7]    :  */
        unsigned int  sc_gt_clk_isp_func     : 1;  /* bit[8]    :  */
        unsigned int  sc_gt_clk_idi2axi_csid : 1;  /* bit[9]    :  */
        unsigned int  reserved_0             : 1;  /* bit[10]   :  */
        unsigned int  reserved_1             : 1;  /* bit[11]   :  */
        unsigned int  reserved_2             : 1;  /* bit[12]   :  */
        unsigned int  reserved_3             : 3;  /* bit[13-15]:  */
        unsigned int  bitmasken              : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                  只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV16_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_csi_idi_START       (0)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_csi_idi_END         (0)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_i3c_START           (1)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_i3c_END             (1)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_hpm_START           (2)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_hpm_END             (2)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_miniisp_START       (3)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_miniisp_END         (3)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_isp_cpu_START       (4)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_isp_cpu_END         (4)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_qic_m2_START        (5)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_qic_m2_END          (5)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_isp_i2c_START       (6)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_isp_i2c_END         (6)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_ipp_func_START      (7)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_ipp_func_END        (7)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_isp_func_START      (8)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_isp_func_END        (8)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_idi2axi_csid_START  (9)
#define SOC_MEDIA2_CRG_CLKDIV16_sc_gt_clk_idi2axi_csid_END    (9)
#define SOC_MEDIA2_CRG_CLKDIV16_bitmasken_START               (16)
#define SOC_MEDIA2_CRG_CLKDIV16_bitmasken_END                 (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV17_UNION
 struct description   : CLKDIV17 Register structure definition
                        Address Offset:0x0C4 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器17。
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
        unsigned int  reserved_8: 8;  /* bit[8-15] :  */
        unsigned int  bitmasken : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV17_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV17_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV17_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV18_UNION
 struct description   : CLKDIV18 Register structure definition
                        Address Offset:0x0C8 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器18。
            电池欠压降频控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vdbat_drop_freq_down_isp_cpu_bypass  : 1;  /* bit[0]    : ATLANTA ES新增
                                                                                电池保护方案ispcpu自动降频功能bypass 逻辑：
                                                                                1'b0:bypass该功能，不降频；
                                                                                1'b1:使能该功能，降频； */
        unsigned int  div_isp_cpu_vbat_drop_freq_down      : 5;  /* bit[1-5]  : ATLANTA ES新增
                                                                                电池保护方案ispcpu自动降频分频比：
                                                                                5'h0:1；
                                                                                5'h1:2；
                                                                                5'h2:3；
                                                                                …
                                                                                5'h1E:31；
                                                                                5'h1F:32； */
        unsigned int  vdbat_drop_freq_down_isp_func_bypass : 1;  /* bit[6]    : ATLANTA ES新增
                                                                                电池保护方案isp func自动降频功能bypass 逻辑：
                                                                                1'b0:bypass该功能，不降频；
                                                                                1'b1:使能该功能，降频； */
        unsigned int  div_isp_func_vbat_drop_freq_down     : 5;  /* bit[7-11] : ATLANTA ES新增
                                                                                电池保护方案isp func自动降频分频比：
                                                                                5'h0:1；
                                                                                5'h1:2；
                                                                                5'h2:3；
                                                                                …
                                                                                5'h1E:31；
                                                                                5'h1F:32； */
        unsigned int  reserved                             : 4;  /* bit[12-15]:  */
        unsigned int  bitmasken                            : 16; /* bit[16-31]: bitmasken位，为clkdiv每个比特位的使能位：
                                                                                只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV18_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV18_vdbat_drop_freq_down_isp_cpu_bypass_START   (0)
#define SOC_MEDIA2_CRG_CLKDIV18_vdbat_drop_freq_down_isp_cpu_bypass_END     (0)
#define SOC_MEDIA2_CRG_CLKDIV18_div_isp_cpu_vbat_drop_freq_down_START       (1)
#define SOC_MEDIA2_CRG_CLKDIV18_div_isp_cpu_vbat_drop_freq_down_END         (5)
#define SOC_MEDIA2_CRG_CLKDIV18_vdbat_drop_freq_down_isp_func_bypass_START  (6)
#define SOC_MEDIA2_CRG_CLKDIV18_vdbat_drop_freq_down_isp_func_bypass_END    (6)
#define SOC_MEDIA2_CRG_CLKDIV18_div_isp_func_vbat_drop_freq_down_START      (7)
#define SOC_MEDIA2_CRG_CLKDIV18_div_isp_func_vbat_drop_freq_down_END        (11)
#define SOC_MEDIA2_CRG_CLKDIV18_bitmasken_START                             (16)
#define SOC_MEDIA2_CRG_CLKDIV18_bitmasken_END                               (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_CLKDIV19_UNION
 struct description   : CLKDIV19 Register structure definition
                        Address Offset:0x0CC Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器19。
            电池欠压降频控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    : reserve */
        unsigned int  reserved_1: 5;  /* bit[1-5]  : reserve */
        unsigned int  reserved_2: 1;  /* bit[6]    : reserve */
        unsigned int  reserved_3: 6;  /* bit[7-12] :  */
        unsigned int  reserved_4: 3;  /* bit[13-15]:  */
        unsigned int  bitmasken : 16; /* bit[16-31]: bitmasken位，为clkdiv每个比特位的使能位：
                                                     只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_CLKDIV19_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV19_bitmasken_START  (16)
#define SOC_MEDIA2_CRG_CLKDIV19_bitmasken_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERI_STAT0_UNION
 struct description   : PERI_STAT0 Register structure definition
                        Address Offset:0x120 Initial:0x000 Width:32
 register description : 外设状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  swdone_clk_hpm_div          : 1;  /* bit[0]    :  */
        unsigned int  swdone_clk_isp_i2c_div      : 1;  /* bit[1]    :  */
        unsigned int  swdone_clk_csi_idi_div      : 1;  /* bit[2]    :  */
        unsigned int  swdone_clk_isp_i3c_div      : 1;  /* bit[3]    :  */
        unsigned int  swdone_clk_idi2axi_csid_div : 1;  /* bit[4]    :  */
        unsigned int  swdone_clk_miniisp_div      : 1;  /* bit[5]    :  */
        unsigned int  swdone_clk_qic_m2_data_div  : 1;  /* bit[6]    :  */
        unsigned int  swdone_clk_isp_cpu_div      : 1;  /* bit[7]    :  */
        unsigned int  swdone_clk_isp_func_div     : 1;  /* bit[8]    :  */
        unsigned int  swdone_clk_ipp_func_div     : 1;  /* bit[9]    :  */
        unsigned int  reserved_0                  : 1;  /* bit[10]   :  */
        unsigned int  reserved_1                  : 1;  /* bit[11]   :  */
        unsigned int  reserved_2                  : 1;  /* bit[12]   :  */
        unsigned int  reserved_3                  : 1;  /* bit[13]   :  */
        unsigned int  reserved_4                  : 1;  /* bit[14]   :  */
        unsigned int  reserved_5                  : 1;  /* bit[15]   :  */
        unsigned int  reserved_6                  : 1;  /* bit[16]   :  */
        unsigned int  reserved_7                  : 1;  /* bit[17]   :  */
        unsigned int  reserved_8                  : 1;  /* bit[18]   :  */
        unsigned int  reserved_9                  : 1;  /* bit[19]   :  */
        unsigned int  reserved_10                 : 12; /* bit[20-31]:  */
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_hpm_div_START           (0)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_hpm_div_END             (0)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_i2c_div_START       (1)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_i2c_div_END         (1)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_csi_idi_div_START       (2)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_csi_idi_div_END         (2)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_i3c_div_START       (3)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_i3c_div_END         (3)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_idi2axi_csid_div_START  (4)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_idi2axi_csid_div_END    (4)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_miniisp_div_START       (5)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_miniisp_div_END         (5)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_qic_m2_data_div_START   (6)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_qic_m2_data_div_END     (6)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_cpu_div_START       (7)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_cpu_div_END         (7)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_func_div_START      (8)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_isp_func_div_END        (8)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_ipp_func_div_START      (9)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_ipp_func_div_END        (9)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERI_STAT1_UNION
 struct description   : PERI_STAT1 Register structure definition
                        Address Offset:0x124 Initial:0x2 Width:32
 register description : 外设状态寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sw_ack_clk_qic_m2_data_sw  : 4;  /* bit[0-3]  : qic m2 data时钟源PLL选择控制：
                                                                      4'b0001：PPLL2_B
                                                                      4'b0010：ULPPLL
                                                                      4'b0100：PPLL0
                                                                      4'b1000：PPLL1 */
        unsigned int  sw_ack_clk_idi2axi_csid_sw : 4;  /* bit[4-7]  : IDI2AXI CSID工作时钟时钟源PLL选择控制：
                                                                      4'b0001：PPLL0
                                                                      4'b0010：ULPPLL
                                                                      4'b0100：PPLL1
                                                                      4'b1000：PPLL2 */
        unsigned int  sw_ack_clk_csi_idi_sw      : 4;  /* bit[8-11] : CSI IDI 工作时钟时钟源PLL选择控制：
                                                                      4'b0001：PPLL0
                                                                      4'b0010：ULPPLL
                                                                      4'b0100：PPLL1
                                                                      4'b1000：PPLL2 */
        unsigned int  sw_ack_clk_miniisp_sw      : 4;  /* bit[12-15]: miniisp 工作时钟时钟源PLL选择控制：
                                                                      4'b0001：PPLL0
                                                                      4'b0010：ULPPLL
                                                                      4'b0100：PPLL1
                                                                      4'b1000：PPLL2 */
        unsigned int  sw_ack_clk_isp_cpu_sw      : 4;  /* bit[16-19]: ISP CPU工作时钟时钟源PLL选择控制：
                                                                      4'b0001：PPLL1
                                                                      4'b0010：PPLL0
                                                                      4'b0100：PPLL2
                                                                      4'b1000：PPLL6 */
        unsigned int  sw_ack_clk_ipp_sw          : 4;  /* bit[20-23]: ISP RT工作时钟时钟源PLL选择控制：
                                                                      4'b0001：PPLL2_B
                                                                      4'b0010：PPLL0
                                                                      4'b0100：PPLL1
                                                                      4'b1000：PPLL2 */
        unsigned int  sw_ack_clk_isp_func_sw     : 4;  /* bit[24-27]: ISP SRT工作时钟时钟源PLL选择控制：
                                                                      4'b0001：PPLL2_B
                                                                      4'b0010：PPLL0
                                                                      4'b0100：PPLL1
                                                                      4'b1000：PPLL2 */
        unsigned int  reserved                   : 4;  /* bit[28-31]: reserved */
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_qic_m2_data_sw_START   (0)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_qic_m2_data_sw_END     (3)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_idi2axi_csid_sw_START  (4)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_idi2axi_csid_sw_END    (7)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_csi_idi_sw_START       (8)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_csi_idi_sw_END         (11)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_miniisp_sw_START       (12)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_miniisp_sw_END         (15)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_isp_cpu_sw_START       (16)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_isp_cpu_sw_END         (19)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_ipp_sw_START           (20)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_ipp_sw_END             (23)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_isp_func_sw_START      (24)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_isp_func_sw_END        (27)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERI_STAT2_UNION
 struct description   : PERI_STAT2 Register structure definition
                        Address Offset:0x128 Initial:0x0 Width:32
 register description : 外设状态寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sw_ack_clk_hpm_sw : 4;  /* bit[0-3]  : HPM时钟源PLL选择控制：
                                                             4'b0001：PPLL2_B
                                                             4'b0010：PPLL0
                                                             4'b0100：PPLL1
                                                             4'b1000：PPLL2 */
        unsigned int  reserved_0        : 4;  /* bit[4-7]  : reserved */
        unsigned int  reserved_1        : 4;  /* bit[8-11] : reserved */
        unsigned int  reserved_2        : 4;  /* bit[12-15]: reserved */
        unsigned int  reserved_3        : 4;  /* bit[16-19]: reserved */
        unsigned int  reserved_4        : 4;  /* bit[20-23]: reserved */
        unsigned int  reserved_5        : 4;  /* bit[24-27]: reserved */
        unsigned int  reserved_6        : 4;  /* bit[28-31]: reserved */
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT2_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT2_sw_ack_clk_hpm_sw_START  (0)
#define SOC_MEDIA2_CRG_PERI_STAT2_sw_ack_clk_hpm_sw_END    (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISPCPU_CLKEN_UNION
 struct description   : ISPCPU_CLKEN Register structure definition
                        Address Offset:0x140 Initial:0x0000 Width:32
 register description : ISPCPU子系统时钟使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ccpu_pd_clk_en    : 1;  /* bit[0]    : CPU时钟使能系统控制：
                                                             0：关闭时钟
                                                             1：开启时钟 */
        unsigned int  ccpu_core_clk_en  : 4;  /* bit[1-4]  : 每个核单独的时钟门控系统控制：
                                                             0：关闭时钟
                                                             1：开启时钟 */
        unsigned int  ccpu_dbg_clk_en   : 4;  /* bit[5-8]  : 每个核单独的核内部dbg 时钟使能：
                                                             0：关闭时钟
                                                             1：开启时钟 */
        unsigned int  ccpu_ct_clk_en    : 1;  /* bit[9]    : Individual CTM clock control signal：
                                                             0：关闭时钟
                                                             1：开启时钟 */
        unsigned int  ccpu_atbclk_en    : 1;  /* bit[10]   : cpu外部的coresight trace逻辑时钟使能控制：
                                                             0：关闭时钟
                                                             1：开启时钟 */
        unsigned int  ccpu_dbgclk_en    : 1;  /* bit[11]   : cpu外部coresight dbg逻辑时钟使能系统控制：
                                                             0：关闭时钟
                                                             1：开启时钟 */
        unsigned int  ccpu_m0clk_divnum : 2;  /* bit[12-13]: CPU CBU和DMI接口时钟频率与core时钟频率比选择(默认二分频)：
                                                             0：1:1
                                                             1：1:2
                                                             2：1:3
                                                             3：1:4 */
        unsigned int  reserved          : 2;  /* bit[14-15]: 保留。 */
        unsigned int  bitmasken         : 16; /* bit[16-31]: clken每个比特位的使能位：
                                                             只有当bitmasken对应的比特位为1'b1，clken相应的比特位才起作用。bitmasken[0]就是clken[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_ISPCPU_CLKEN_UNION;
#endif
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_pd_clk_en_START     (0)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_pd_clk_en_END       (0)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_core_clk_en_START   (1)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_core_clk_en_END     (4)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_dbg_clk_en_START    (5)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_dbg_clk_en_END      (8)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_ct_clk_en_START     (9)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_ct_clk_en_END       (9)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_atbclk_en_START     (10)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_atbclk_en_END       (10)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_dbgclk_en_START     (11)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_dbgclk_en_END       (11)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_m0clk_divnum_START  (12)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_ccpu_m0clk_divnum_END    (13)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_bitmasken_START          (16)
#define SOC_MEDIA2_CRG_ISPCPU_CLKEN_bitmasken_END            (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISPCPU_RSTEN_UNION
 struct description   : ISPCPU_RSTEN Register structure definition
                        Address Offset:0x144 Initial:0x00000 Width:32
 register description : ISPCPU子系统软复位使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ccpu_core_srst_req        : 4;  /* bit[0-3]  : Core reset. Resets all non-debug logic. */
        unsigned int  ccpu_scu_srst_req         : 1;  /* bit[4]    : Snoop Control Unit (SCU) global reset. */
        unsigned int  ccpu_ct_srst_req          : 1;  /* bit[5]    : Reset for CTI, CTM, and ROM table */
        unsigned int  ccpu_wdt_dbg_etm_srst_req : 4;  /* bit[6-9]  : atb coresight侧时钟复位/每个核内部dbg逻辑软复位/ ETM复位，电平复位 */
        unsigned int  ccpu_l2c_srst_req         : 1;  /* bit[10]   : 暂未使用 */
        unsigned int  ccpu_periph_srst_req      : 1;  /* bit[11]   : Timer and interrupt controller reset. */
        unsigned int  cs_ccpu_dbg_srst_req      : 1;  /* bit[12]   : cpu外部dbg逻辑软复位请求 */
        unsigned int  ccpu_pd_srst_req          : 1;  /* bit[13]   : cpu整体软复位请求 */
        unsigned int  reserved                  : 18; /* bit[14-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_ISPCPU_RSTEN_UNION;
#endif
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_core_srst_req_START         (0)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_core_srst_req_END           (3)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_scu_srst_req_START          (4)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_scu_srst_req_END            (4)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_ct_srst_req_START           (5)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_ct_srst_req_END             (5)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_wdt_dbg_etm_srst_req_START  (6)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_wdt_dbg_etm_srst_req_END    (9)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_l2c_srst_req_START          (10)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_l2c_srst_req_END            (10)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_periph_srst_req_START       (11)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_periph_srst_req_END         (11)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_cs_ccpu_dbg_srst_req_START       (12)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_cs_ccpu_dbg_srst_req_END         (12)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_pd_srst_req_START           (13)
#define SOC_MEDIA2_CRG_ISPCPU_RSTEN_ccpu_pd_srst_req_END             (13)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISPCPU_RSTDIS_UNION
 struct description   : ISPCPU_RSTDIS Register structure definition
                        Address Offset:0x148 Initial:0x00000 Width:32
 register description : ISPCPU子系统软复位撤离寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ccpu_core_srst_req        : 4;  /* bit[0-3]  : Core reset. Resets all non-debug logic. */
        unsigned int  ccpu_scu_srst_req         : 1;  /* bit[4]    : Snoop Control Unit (SCU) global reset. */
        unsigned int  ccpu_ct_srst_req          : 1;  /* bit[5]    : Reset for CTI, CTM, and ROM table */
        unsigned int  ccpu_wdt_dbg_etm_srst_req : 4;  /* bit[6-9]  : atb coresight侧时钟复位/每个核内部dbg逻辑软复位/ ETM复位，电平复位 */
        unsigned int  ccpu_l2c_srst_req         : 1;  /* bit[10]   : 暂未使用 */
        unsigned int  ccpu_periph_srst_req      : 1;  /* bit[11]   : Timer and interrupt controller reset. */
        unsigned int  cs_ccpu_dbg_srst_req      : 1;  /* bit[12]   : cpu外部dbg逻辑软复位请求 */
        unsigned int  ccpu_pd_srst_req          : 1;  /* bit[13]   : cpu整体软复位请求 */
        unsigned int  reserved                  : 18; /* bit[14-31]: IP软复位请求撤离：
                                                                     0：无效果；
                                                                     1：IP软复位请求撤离。 */
    } reg;
} SOC_MEDIA2_CRG_ISPCPU_RSTDIS_UNION;
#endif
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_core_srst_req_START         (0)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_core_srst_req_END           (3)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_scu_srst_req_START          (4)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_scu_srst_req_END            (4)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_ct_srst_req_START           (5)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_ct_srst_req_END             (5)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_wdt_dbg_etm_srst_req_START  (6)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_wdt_dbg_etm_srst_req_END    (9)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_l2c_srst_req_START          (10)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_l2c_srst_req_END            (10)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_periph_srst_req_START       (11)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_periph_srst_req_END         (11)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_cs_ccpu_dbg_srst_req_START       (12)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_cs_ccpu_dbg_srst_req_END         (12)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_pd_srst_req_START           (13)
#define SOC_MEDIA2_CRG_ISPCPU_RSTDIS_ccpu_pd_srst_req_END             (13)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_UNION
 struct description   : ISPCPU_RSTSTAT Register structure definition
                        Address Offset:0x14C Initial:0x00000 Width:32
 register description : ISPCPU子系统软复位使能状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ccpu_core_srst_req        : 4;  /* bit[0-3]  : Core reset. Resets all non-debug logic. */
        unsigned int  ccpu_scu_srst_req         : 1;  /* bit[4]    : Snoop Control Unit (SCU) global reset. */
        unsigned int  ccpu_ct_srst_req          : 1;  /* bit[5]    : Reset for CTI, CTM, and ROM table */
        unsigned int  ccpu_wdt_dbg_etm_srst_req : 4;  /* bit[6-9]  : atb coresight侧时钟复位/每个核内部dbg逻辑软复位/ ETM复位，电平复位 */
        unsigned int  ccpu_l2c_srst_req         : 1;  /* bit[10]   : 暂未使用 */
        unsigned int  ccpu_periph_srst_req      : 1;  /* bit[11]   : Timer and interrupt controller reset. */
        unsigned int  cs_ccpu_dbg_srst_req      : 1;  /* bit[12]   : cpu外部dbg逻辑软复位请求 */
        unsigned int  ccpu_pd_srst_req          : 1;  /* bit[13]   : cpu整体软复位请求 */
        unsigned int  reserved                  : 18; /* bit[14-31]: IP软复位请求使能状态：
                                                                     0：IP处于复位请求撤离状态；
                                                                     1：IP处于软复位请求使能状态。 */
    } reg;
} SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_UNION;
#endif
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_core_srst_req_START         (0)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_core_srst_req_END           (3)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_scu_srst_req_START          (4)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_scu_srst_req_END            (4)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_ct_srst_req_START           (5)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_ct_srst_req_END             (5)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_wdt_dbg_etm_srst_req_START  (6)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_wdt_dbg_etm_srst_req_END    (9)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_l2c_srst_req_START          (10)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_l2c_srst_req_END            (10)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_periph_srst_req_START       (11)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_periph_srst_req_END         (11)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_cs_ccpu_dbg_srst_req_START       (12)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_cs_ccpu_dbg_srst_req_END         (12)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_pd_srst_req_START           (13)
#define SOC_MEDIA2_CRG_ISPCPU_RSTSTAT_ccpu_pd_srst_req_END             (13)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISP_CPU_STATE0_UNION
 struct description   : ISP_CPU_STATE0 Register structure definition
                        Address Offset:0x150 Initial:0x0000 Width:32
 register description : ISPCPU状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  c0_sys_halt_r     : 1;  /* bit[0]    : Indicates that processor is halted */
        unsigned int  c0_sys_sleep_r    : 1;  /* bit[1]    : Indicates that processor is sleeping */
        unsigned int  c1_sys_halt_r     : 1;  /* bit[2]    : Indicates that processor is halted */
        unsigned int  c1_sys_sleep_r    : 1;  /* bit[3]    : Indicates that processor is sleeping */
        unsigned int  c0_sys_tf_halt_r  : 1;  /* bit[4]    : Asserted when the core is halted due to a triple fault exception */
        unsigned int  c0_sys_sleep_mode : 3;  /* bit[5-7]  : Indicates the sleep mode entered by the processor */
        unsigned int  c1_sys_tf_halt_r  : 1;  /* bit[8]    : Asserted when the core is halted due to a triple fault exception */
        unsigned int  c1_sys_sleep_mode : 3;  /* bit[9-11] : Indicates the sleep mode entered by the processor */
        unsigned int  c0_arc_halt_ack   : 1;  /* bit[12]   : Core acknowledge to halt request */
        unsigned int  c0_arc_run_ack    : 1;  /* bit[13]   : Core acknowledge to run request */
        unsigned int  c1_arc_halt_ack   : 1;  /* bit[14]   : Core acknowledge to halt request */
        unsigned int  c1_arc_run_ack    : 1;  /* bit[15]   : Core acknowledge to run request */
        unsigned int  reserved          : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_ISP_CPU_STATE0_UNION;
#endif
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_halt_r_START      (0)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_halt_r_END        (0)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_sleep_r_START     (1)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_sleep_r_END       (1)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_halt_r_START      (2)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_halt_r_END        (2)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_sleep_r_START     (3)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_sleep_r_END       (3)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_tf_halt_r_START   (4)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_tf_halt_r_END     (4)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_sleep_mode_START  (5)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_sys_sleep_mode_END    (7)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_tf_halt_r_START   (8)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_tf_halt_r_END     (8)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_sleep_mode_START  (9)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_sys_sleep_mode_END    (11)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_arc_halt_ack_START    (12)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_arc_halt_ack_END      (12)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_arc_run_ack_START     (13)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c0_arc_run_ack_END       (13)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_arc_halt_ack_START    (14)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_arc_halt_ack_END      (14)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_arc_run_ack_START     (15)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE0_c1_arc_run_ack_END       (15)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISP_CPU_STATE1_UNION
 struct description   : ISP_CPU_STATE1 Register structure definition
                        Address Offset:0x154 Initial:0x00000000 Width:32
 register description : ISPCPU状态寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_cpu_state1 : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_ISP_CPU_STATE1_UNION;
#endif
#define SOC_MEDIA2_CRG_ISP_CPU_STATE1_isp_cpu_state1_START  (0)
#define SOC_MEDIA2_CRG_ISP_CPU_STATE1_isp_cpu_state1_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERI_AUTODIV0_UNION
 struct description   : PERI_AUTODIV0 Register structure definition
                        Address Offset:0x160 Initial:0x00 Width:32
 register description : 外设自动降频控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_m2_div_auto_reduce_bypass : 1;  /* bit[0]    : 硬件自动降频功能bypass配置：
                                                                         0：不bypass；
                                                                         1：bypass。 */
        unsigned int  qic_m2_auto_waitcfg_in        : 10; /* bit[1-10] : 硬件进入自动降频IDLE等待时钟周期配置 */
        unsigned int  qic_m2_auto_waitcfg_out       : 10; /* bit[11-20]: 硬件退出自动降频IDLE等待时钟周期配置 */
        unsigned int  qic_m2_div_auto_cfg           : 6;  /* bit[21-26]: 硬件自动降频倍数配置
                                                                         6'h00:1；
                                                                         6'h01:2；
                                                                         …
                                                                         6'h3F:64； */
        unsigned int  reserved                      : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_div_auto_reduce_bypass_START  (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_div_auto_reduce_bypass_END    (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_auto_waitcfg_in_START         (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_auto_waitcfg_in_END           (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_auto_waitcfg_out_START        (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_auto_waitcfg_out_END          (20)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_div_auto_cfg_START            (21)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_qic_m2_div_auto_cfg_END              (26)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERI_AUTODIV1_UNION
 struct description   : PERI_AUTODIV1 Register structure definition
                        Address Offset:0x16c Initial:0x0000 Width:32
 register description : 外设自动降频控制寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_idle_flag_bypass                       : 1;  /* bit[0]    :  */
        unsigned int  tcu_idle_flag_bypass                       : 1;  /* bit[1]    :  */
        unsigned int  isp_dma_64to128_idle_bypass                : 1;  /* bit[2]    :  */
        unsigned int  perf_idle_isp_0_bypass                     : 1;  /* bit[3]    :  */
        unsigned int  perf_idle_isp_1_bypass                     : 1;  /* bit[4]    :  */
        unsigned int  perf_idle_isp_2_bypass                     : 1;  /* bit[5]    :  */
        unsigned int  perf_idle_isp_3_bypass                     : 1;  /* bit[6]    :  */
        unsigned int  perf_idle_isp_4_bypass                     : 1;  /* bit[7]    :  */
        unsigned int  himinibus_isp_2to5_hiaxi_idle_bypass       : 1;  /* bit[8]    :  */
        unsigned int  perf_idle_ipp_bypass                       : 1;  /* bit[9]    :  */
        unsigned int  perf_idle_tcu_bypass                       : 1;  /* bit[10]   :  */
        unsigned int  perf_idle_ibqe_bypass                      : 1;  /* bit[11]   :  */
        unsigned int  qice_bus_media2_nopending_comb_bypass      : 1;  /* bit[12]   :  */
        unsigned int  perf_idle_miniisp_bypass                   : 1;  /* bit[13]   :  */
        unsigned int  perf_idle_idi2axi_bypass                   : 1;  /* bit[14]   :  */
        unsigned int  isp_cpu_idle_flag_bypass                   : 1;  /* bit[15]   :  */
        unsigned int  himinibus_isp_axi_s4_x2p_async_idle_bypass : 1;  /* bit[16]   :  */
        unsigned int  ipp_himinibus_x2p_idle_bypass              : 1;  /* bit[17]   :  */
        unsigned int  ipp_himinibus_x2x_idle_byapss              : 1;  /* bit[18]   :  */
        unsigned int  reserved                                   : 13; /* bit[19-31]:  */
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_isp_idle_flag_bypass_START                        (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_isp_idle_flag_bypass_END                          (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_tcu_idle_flag_bypass_START                        (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_tcu_idle_flag_bypass_END                          (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_isp_dma_64to128_idle_bypass_START                 (2)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_isp_dma_64to128_idle_bypass_END                   (2)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_0_bypass_START                      (3)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_0_bypass_END                        (3)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_1_bypass_START                      (4)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_1_bypass_END                        (4)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_2_bypass_START                      (5)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_2_bypass_END                        (5)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_3_bypass_START                      (6)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_3_bypass_END                        (6)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_4_bypass_START                      (7)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_isp_4_bypass_END                        (7)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_himinibus_isp_2to5_hiaxi_idle_bypass_START        (8)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_himinibus_isp_2to5_hiaxi_idle_bypass_END          (8)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_ipp_bypass_START                        (9)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_ipp_bypass_END                          (9)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_tcu_bypass_START                        (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_tcu_bypass_END                          (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_ibqe_bypass_START                       (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_ibqe_bypass_END                         (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_qice_bus_media2_nopending_comb_bypass_START       (12)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_qice_bus_media2_nopending_comb_bypass_END         (12)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_miniisp_bypass_START                    (13)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_miniisp_bypass_END                      (13)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_idi2axi_bypass_START                    (14)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_perf_idle_idi2axi_bypass_END                      (14)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_isp_cpu_idle_flag_bypass_START                    (15)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_isp_cpu_idle_flag_bypass_END                      (15)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_himinibus_isp_axi_s4_x2p_async_idle_bypass_START  (16)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_himinibus_isp_axi_s4_x2p_async_idle_bypass_END    (16)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ipp_himinibus_x2p_idle_bypass_START               (17)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ipp_himinibus_x2p_idle_bypass_END                 (17)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ipp_himinibus_x2x_idle_byapss_START               (18)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ipp_himinibus_x2x_idle_byapss_END                 (18)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_UNION
 struct description   : QIC_BUS_ISP_NOPENDING_GRP0_BYPASS Register structure definition
                        Address Offset:0x170 Initial:0x00000000 Width:32
 register description : 外设自动降频控制寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_bus_isp_nopending_grp0_bypass : 32; /* bit[0-31]:  */
    } reg;
} SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_qic_bus_isp_nopending_grp0_bypass_START  (0)
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP0_BYPASS_qic_bus_isp_nopending_grp0_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_UNION
 struct description   : QIC_BUS_ISP_NOPENDING_GRP1_BYPASS Register structure definition
                        Address Offset:0x174 Initial:0x00000000 Width:32
 register description : 外设自动降频控制寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_bus_isp_nopending_grp1_bypass : 32; /* bit[0-31]:  */
    } reg;
} SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_qic_bus_isp_nopending_grp1_bypass_START  (0)
#define SOC_MEDIA2_CRG_QIC_BUS_ISP_NOPENDING_GRP1_BYPASS_qic_bus_isp_nopending_grp1_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_UNION
 struct description   : QIC_BUS_IPP_NOPENDING_GRP2_BYPASS Register structure definition
                        Address Offset:0x178 Initial:0x00000000 Width:32
 register description : 外设自动降频控制寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_bus_ipp_nopending_grp0_bypass : 32; /* bit[0-31]:  */
    } reg;
} SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_qic_bus_ipp_nopending_grp0_bypass_START  (0)
#define SOC_MEDIA2_CRG_QIC_BUS_IPP_NOPENDING_GRP2_BYPASS_qic_bus_ipp_nopending_grp0_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_UNION
 struct description   : QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS Register structure definition
                        Address Offset:0x18c Initial:0x00000000 Width:32
 register description : 外设自动降频控制寄存器11。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_bus_media2_nopending_grp0_bypass : 32; /* bit[0-31]:  */
    } reg;
} SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_qic_bus_media2_nopending_grp0_bypass_START  (0)
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP0_BYPASS_qic_bus_media2_nopending_grp0_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_UNION
 struct description   : QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS Register structure definition
                        Address Offset:0x190 Initial:0x00000000 Width:32
 register description : 外设自动降频控制寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_bus_media2_nopending_grp1_bypass : 32; /* bit[0-31]:  */
    } reg;
} SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_qic_bus_media2_nopending_grp1_bypass_START  (0)
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP1_BYPASS_qic_bus_media2_nopending_grp1_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_UNION
 struct description   : QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS Register structure definition
                        Address Offset:0x194 Initial:0x00000000 Width:32
 register description : 外设自动降频控制寄存器13。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_bus_media2_nopending_grp2_bypass : 32; /* bit[0-31]:  */
    } reg;
} SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_qic_bus_media2_nopending_grp2_bypass_START  (0)
#define SOC_MEDIA2_CRG_QIC_BUS_MEDIA2_NOPENDING_GRP2_BYPASS_qic_bus_media2_nopending_grp2_bypass_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_UNION
 struct description   : MEDIA_AUTODIV_STAT Register structure definition
                        Address Offset:0x198 Initial:0x000000 Width:32
 register description : 自动降频状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  vbat_drop_freq_down_isp_cpu_stat  : 1;  /* bit[0]   : 自动降频状态回读，默认为0
                                                                            0:处于不降频；
                                                                            1：处于降频态。 */
        unsigned int  vbat_drop_freq_down_isp_func_stat : 1;  /* bit[1]   : ATLANTA ES新增
                                                                            电池欠压降频状态回读，默认为0
                                                                            0:处于不降频
                                                                            1：处于降频态 */
        unsigned int  reserved_0                        : 1;  /* bit[2]   :  */
        unsigned int  reserved_1                        : 1;  /* bit[3]   :  */
        unsigned int  reserved_2                        : 1;  /* bit[4]   :  */
        unsigned int  reserved_3                        : 1;  /* bit[5]   :  */
        unsigned int  reserved_4                        : 1;  /* bit[6]   :  */
        unsigned int  reserved_5                        : 1;  /* bit[7]   :  */
        unsigned int  reserved_6                        : 1;  /* bit[8]   :  */
        unsigned int  reserved_7                        : 23; /* bit[9-31]: 保留 */
    } reg;
} SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_UNION;
#endif
#define SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_isp_cpu_stat_START   (0)
#define SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_isp_cpu_stat_END     (0)
#define SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_isp_func_stat_START  (1)
#define SOC_MEDIA2_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_isp_func_stat_END    (1)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_UNION
 struct description   : ISPTOMED_RESERVE_OUT Register structure definition
                        Address Offset:0x19c Initial:0x000000 Width:32
 register description : ISP的状态回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isptomed_reserve_stat : 8;  /* bit[0-7] : 对应比特意义
                                                                [7:0]:reserverd */
        unsigned int  reserved              : 24; /* bit[8-31]: 保留 */
    } reg;
} SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_UNION;
#endif
#define SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_isptomed_reserve_stat_START  (0)
#define SOC_MEDIA2_CRG_ISPTOMED_RESERVE_OUT_isptomed_reserve_stat_END    (7)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_UNION
 struct description   : PERI_AUTODIV_STAT Register structure definition
                        Address Offset:0x1A0 Initial:0x0000000 Width:32
 register description : 外设自动降频状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  autodiv_qic_m2_stat : 1;  /* bit[0]   :  */
        unsigned int  reserved_0          : 1;  /* bit[1]   :  */
        unsigned int  reserved_1          : 1;  /* bit[2]   :  */
        unsigned int  reserved_2          : 1;  /* bit[3]   :  */
        unsigned int  reserved_3          : 1;  /* bit[4]   :  */
        unsigned int  reserved_4          : 27; /* bit[5-31]: 自动降频状态回读，默认为0
                                                              0:处于不降频；
                                                              1：处于降频态。 */
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_qic_m2_stat_START  (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_qic_m2_stat_END    (0)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_UNION
 struct description   : INTR_BUS_NONIDLE_PEND Register structure definition
                        Address Offset:0x1D0 Initial:0x0000 Width:32
 register description : 总线异常中断屏蔽与清除寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_clr_media2bus_nonidle_pend   : 1;  /* bit[0]    : 中断清除：
                                                                             0：不清除
                                                                             1：清除
                                                                             注：该中断清除信号高电平有效，清除信号为高时，一直处于清中断状态，清除中断后需要将该清除信号配置为0 */
        unsigned int  intr_mask_media2qic_nonidle_pend  : 1;  /* bit[1]    : 中断屏蔽：
                                                                             0：屏蔽
                                                                             1：不屏蔽 */
        unsigned int  intr_mask_media2ibqe_nonidle_pend : 1;  /* bit[2]    : 中断屏蔽：
                                                                             0：屏蔽
                                                                             1：不屏蔽 */
        unsigned int  intr_mask_ispbus_nonidle_pend     : 1;  /* bit[3]    : 中断屏蔽：
                                                                             0：屏蔽
                                                                             1：不屏蔽 */
        unsigned int  intr_mask_ippbus_nonidle_pend     : 1;  /* bit[4]    : 中断屏蔽：
                                                                             0：屏蔽
                                                                             1：不屏蔽 */
        unsigned int  reserved_0                        : 1;  /* bit[5]    : 中断屏蔽：
                                                                             0：屏蔽
                                                                             1：不屏蔽 */
        unsigned int  reserved_1                        : 1;  /* bit[6]    : 保留。 */
        unsigned int  reserved_2                        : 1;  /* bit[7]    : 保留。 */
        unsigned int  reserved_3                        : 1;  /* bit[8]    : 保留。 */
        unsigned int  reserved_4                        : 7;  /* bit[9-15] : 保留。 */
        unsigned int  bitmasken                         : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                             只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_UNION;
#endif
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_clr_media2bus_nonidle_pend_START    (0)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_clr_media2bus_nonidle_pend_END      (0)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_media2qic_nonidle_pend_START   (1)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_media2qic_nonidle_pend_END     (1)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_media2ibqe_nonidle_pend_START  (2)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_media2ibqe_nonidle_pend_END    (2)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ispbus_nonidle_pend_START      (3)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ispbus_nonidle_pend_END        (3)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ippbus_nonidle_pend_START      (4)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ippbus_nonidle_pend_END        (4)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_bitmasken_START                          (16)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_bitmasken_END                            (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_UNION
 struct description   : INTR_STAT_BUS_NONIDLE_PEND Register structure definition
                        Address Offset:0x1D4 Initial:0x0000 Width:32
 register description : 总线异常中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_stat_media2qic_nonidle_pend  : 1;  /* bit[0]    : 中断状态：
                                                                             0：中断无效
                                                                             1：中断有效 */
        unsigned int  intr_stat_media2ibqe_nonidle_pend : 1;  /* bit[1]    : 中断状态：
                                                                             0：中断无效
                                                                             1：中断有效 */
        unsigned int  intr_stat_ispbus_nonidle_pend     : 1;  /* bit[2]    : 中断状态：
                                                                             0：中断无效
                                                                             1：中断有效 */
        unsigned int  intr_stat_ippbus_nonidle_pend     : 1;  /* bit[3]    : 中断状态：
                                                                             0：中断无效
                                                                             1：中断有效 */
        unsigned int  reserved_0                        : 1;  /* bit[4]    : 中断状态：
                                                                             0：中断无效
                                                                             1：中断有效 */
        unsigned int  reserved_1                        : 1;  /* bit[5]    : 保留。 */
        unsigned int  reserved_2                        : 1;  /* bit[6]    : 保留。 */
        unsigned int  reserved_3                        : 1;  /* bit[7]    : 保留。 */
        unsigned int  reserved_4                        : 8;  /* bit[8-15] : 保留。 */
        unsigned int  reserved_5                        : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                             只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_UNION;
#endif
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_media2qic_nonidle_pend_START   (0)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_media2qic_nonidle_pend_END     (0)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_media2ibqe_nonidle_pend_START  (1)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_media2ibqe_nonidle_pend_END    (1)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ispbus_nonidle_pend_START      (2)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ispbus_nonidle_pend_END        (2)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ippbus_nonidle_pend_START      (3)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ippbus_nonidle_pend_END        (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_IPCLKRST_BYPASS_UNION
 struct description   : IPCLKRST_BYPASS Register structure definition
                        Address Offset:0x1D8 Initial:0x0000000 Width:32
 register description : 外设IP防挂死功能byapss控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tcu_cfg_clkrst_bypass         : 1;  /* bit[0]   :  */
        unsigned int  csi_wrapper_cfg_clkrst_bypass : 1;  /* bit[1]   :  */
        unsigned int  idi2axi_cfg_clkrst_bypass     : 1;  /* bit[2]   :  */
        unsigned int  miniisp_cfg_clkrst_bypass     : 1;  /* bit[3]   :  */
        unsigned int  p2j_clkrst_bypass             : 1;  /* bit[4]   : Charlotte 不支持 */
        unsigned int  reserved                      : 27; /* bit[5-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_IPCLKRST_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_tcu_cfg_clkrst_bypass_START          (0)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_tcu_cfg_clkrst_bypass_END            (0)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_csi_wrapper_cfg_clkrst_bypass_START  (1)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_csi_wrapper_cfg_clkrst_bypass_END    (1)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_idi2axi_cfg_clkrst_bypass_START      (2)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_idi2axi_cfg_clkrst_bypass_END        (2)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_miniisp_cfg_clkrst_bypass_START      (3)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_miniisp_cfg_clkrst_bypass_END        (3)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_p2j_clkrst_bypass_START              (4)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_p2j_clkrst_bypass_END                (4)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HPM_CFG_UNION
 struct description   : HPM_CFG Register structure definition
                        Address Offset:0x200 Initial:0x0000 Width:32
 register description : hpm配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hpm_en         : 1;  /* bit[0]    : hpm计算最小值使能 */
        unsigned int  hpm_opc_min_en : 1;  /* bit[1]    : hpm使能 */
        unsigned int  reserved_0     : 1;  /* bit[2]    : 保留。 */
        unsigned int  reserved_1     : 1;  /* bit[3]    : 保留。 */
        unsigned int  hpm_clk_div    : 6;  /* bit[4-9]  : hpm_clk_div分频比 */
        unsigned int  reserved_2     : 2;  /* bit[10-11]: 保留。 */
        unsigned int  reserved_3     : 4;  /* bit[12-15]: 保留。 */
        unsigned int  reserved_4     : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HPM_CFG_UNION;
#endif
#define SOC_MEDIA2_CRG_HPM_CFG_hpm_en_START          (0)
#define SOC_MEDIA2_CRG_HPM_CFG_hpm_en_END            (0)
#define SOC_MEDIA2_CRG_HPM_CFG_hpm_opc_min_en_START  (1)
#define SOC_MEDIA2_CRG_HPM_CFG_hpm_opc_min_en_END    (1)
#define SOC_MEDIA2_CRG_HPM_CFG_hpm_clk_div_START     (4)
#define SOC_MEDIA2_CRG_HPM_CFG_hpm_clk_div_END       (9)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HPM_READ_BACK_UNION
 struct description   : HPM_READ_BACK Register structure definition
                        Address Offset:0x204 Initial:0x00 Width:32
 register description : hpm回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hpm_opc_vld : 1;  /* bit[0]    : hpm OPC有效标志 */
        unsigned int  hpm_opc     : 10; /* bit[1-10] : hpm OPC码字 */
        unsigned int  hpm_opc_min : 10; /* bit[11-20]: hpm OPC_min */
        unsigned int  reserved_0  : 3;  /* bit[21-23]: 保留。 */
        unsigned int  reserved_1  : 8;  /* bit[24-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HPM_READ_BACK_UNION;
#endif
#define SOC_MEDIA2_CRG_HPM_READ_BACK_hpm_opc_vld_START  (0)
#define SOC_MEDIA2_CRG_HPM_READ_BACK_hpm_opc_vld_END    (0)
#define SOC_MEDIA2_CRG_HPM_READ_BACK_hpm_opc_START      (1)
#define SOC_MEDIA2_CRG_HPM_READ_BACK_hpm_opc_END        (10)
#define SOC_MEDIA2_CRG_HPM_READ_BACK_hpm_opc_min_START  (11)
#define SOC_MEDIA2_CRG_HPM_READ_BACK_hpm_opc_min_END    (20)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HPMX_CFG_UNION
 struct description   : HPMX_CFG Register structure definition
                        Address Offset:0x208 Initial:0x0000 Width:32
 register description : hpmx配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hpmx_en         : 1;  /* bit[0]    : hpmx计算最小值使能 */
        unsigned int  hpmx_opc_min_en : 1;  /* bit[1]    : hpmx使能 */
        unsigned int  reserved_0      : 1;  /* bit[2]    : 保留。 */
        unsigned int  reserved_1      : 1;  /* bit[3]    : 保留。 */
        unsigned int  reserved_2      : 12; /* bit[4-15] : 保留。 */
        unsigned int  reserved_3      : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HPMX_CFG_UNION;
#endif
#define SOC_MEDIA2_CRG_HPMX_CFG_hpmx_en_START          (0)
#define SOC_MEDIA2_CRG_HPMX_CFG_hpmx_en_END            (0)
#define SOC_MEDIA2_CRG_HPMX_CFG_hpmx_opc_min_en_START  (1)
#define SOC_MEDIA2_CRG_HPMX_CFG_hpmx_opc_min_en_END    (1)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HPMX_READ_BACK_UNION
 struct description   : HPMX_READ_BACK Register structure definition
                        Address Offset:0x20C Initial:0x00 Width:32
 register description : hpmx回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hpmx_opc_vld : 1;  /* bit[0]    : hpmx OPC有效标志 */
        unsigned int  hpmx_opc     : 10; /* bit[1-10] : hpmx OPC码字 */
        unsigned int  hpmx_opc_min : 10; /* bit[11-20]: hpmx OPC_min */
        unsigned int  reserved_0   : 3;  /* bit[21-23]: 保留。 */
        unsigned int  reserved_1   : 8;  /* bit[24-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HPMX_READ_BACK_UNION;
#endif
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_hpmx_opc_vld_START  (0)
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_hpmx_opc_vld_END    (0)
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_hpmx_opc_START      (1)
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_hpmx_opc_END        (10)
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_hpmx_opc_min_START  (11)
#define SOC_MEDIA2_CRG_HPMX_READ_BACK_hpmx_opc_min_END    (20)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_UNION
 struct description   : HIMINIBUS_CKG_EN Register structure definition
                        Address Offset:0x400 Initial:0x0000 Width:32
 register description : himinibus自动门控配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                    : 1;  /* bit[0]    : 保留。 */
        unsigned int  reserved_1                    : 1;  /* bit[1]    : 保留。 */
        unsigned int  himinibus_isp_axi_2to5_ckg_en : 1;  /* bit[2]    : 0：无自动时钟门控100 06新增
                                                                         1：有自动时钟门控
                                                                         软件配置该寄存器为1 时需要在初始化时，确保任何操作均没有发到桥上； */
        unsigned int  himinibus_ipp_x2p_ckg_en      : 1;  /* bit[3]    : 0：无自动时钟门控100 06新增
                                                                         1：有自动时钟门控
                                                                         软件配置该寄存器为1 时需要在初始化时，确保任何操作均没有发到桥上； */
        unsigned int  ipp_2to3_hiaxi_ckg_en         : 1;  /* bit[4]    : 0：无自动时钟门控100 06新增
                                                                         1：有自动时钟门控
                                                                         软件配置该寄存器为1 时需要在初始化时，确保任何操作均没有发到桥上； */
        unsigned int  reserved_2                    : 1;  /* bit[5]    : 保留。 */
        unsigned int  reserved_3                    : 1;  /* bit[6]    : 保留。 */
        unsigned int  reserved_4                    : 1;  /* bit[7]    : 保留。 */
        unsigned int  reserved_5                    : 8;  /* bit[8-15] : 保留。 */
        unsigned int  reserved_6                    : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_UNION;
#endif
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_himinibus_isp_axi_2to5_ckg_en_START  (2)
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_himinibus_isp_axi_2to5_ckg_en_END    (2)
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_himinibus_ipp_x2p_ckg_en_START       (3)
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_himinibus_ipp_x2p_ckg_en_END         (3)
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_ipp_2to3_hiaxi_ckg_en_START          (4)
#define SOC_MEDIA2_CRG_HIMINIBUS_CKG_EN_ipp_2to3_hiaxi_ckg_en_END            (4)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_UNION
 struct description   : HIMINIBUS_IDLE_READ_BACK Register structure definition
                        Address Offset:0x404 Initial:0x0000 Width:32
 register description : himinibus idle回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                          : 1;  /* bit[0]    : 保留。 */
        unsigned int  reserved_1                          : 1;  /* bit[1]    : 保留。 */
        unsigned int  himinibus_isp_2to5_hiaxi_idle       : 1;  /* bit[2]    : himinibus_isp_2to5_hiaxi_idle 100 06新增 */
        unsigned int  reserved_2                          : 1;  /* bit[3]    : 保留。 */
        unsigned int  reserved_3                          : 1;  /* bit[4]    : 保留。 */
        unsigned int  isp_dma_64to128_idle                : 1;  /* bit[5]    : isp_dma_64to128_idle */
        unsigned int  himinibus_isp_axi_s3_xp2_sync_idle  : 1;  /* bit[6]    : himinibus_isp_axi_s3_xp2_sync_idle 100 06新增 */
        unsigned int  himinibus_isp_axi_s4_x2p_async_idle : 1;  /* bit[7]    : himinibus_isp_axi_s4_x2p_async_idle 100 06新增 */
        unsigned int  reserved_4                          : 8;  /* bit[8-15] : 保留。 */
        unsigned int  reserved_5                          : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_UNION;
#endif
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_himinibus_isp_2to5_hiaxi_idle_START        (2)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_himinibus_isp_2to5_hiaxi_idle_END          (2)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_isp_dma_64to128_idle_START                 (5)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_isp_dma_64to128_idle_END                   (5)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_himinibus_isp_axi_s3_xp2_sync_idle_START   (6)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_himinibus_isp_axi_s3_xp2_sync_idle_END     (6)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_himinibus_isp_axi_s4_x2p_async_idle_START  (7)
#define SOC_MEDIA2_CRG_HIMINIBUS_IDLE_READ_BACK_himinibus_isp_axi_s4_x2p_async_idle_END    (7)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISP_READ_BACK0_UNION
 struct description   : ISP_READ_BACK0 Register structure definition
                        Address Offset:0x408 Initial:0x0 Width:32
 register description : isp回读寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_himinibus_2to5_dlock_mst              : 4;  /* bit[0-3]  : isp_himinibus_2to5_dlock_mst */
        unsigned int  isp_himinibus_2to5_dlock_slv              : 10; /* bit[4-13] : isp_himinibus_2to5_dlock_slv */
        unsigned int  isp_himinibus_tcmdma_1to2_dlock_mst       : 2;  /* bit[14-15]: isp_himinibus_tcmdma_1to2_dlock_mst */
        unsigned int  isp_himinibus_tcmdma_1to2_dlock_slv       : 4;  /* bit[16-19]: isp_himinibus_tcmdma_1to2_dlock_slv */
        unsigned int  isp_himinibus_tcmdma_mport_2to1_dlock_mst : 4;  /* bit[20-23]: isp_himinibus_tcmdma_mport_1to2_dlock_mst */
        unsigned int  isp_himinibus_tcmdma_mport_2to1_dlock_slv : 2;  /* bit[24-25]: isp_himinibus_tcmdma_mport_1to2_dlock_slv */
        unsigned int  isp_himinibus_tcmdma_sport_2to1_dlock_mst : 4;  /* bit[26-29]: isp_himinibus_tcmdma_sport_1to2_dlock_mst */
        unsigned int  isp_himinibus_tcmdma_sport_2to1_dlock_slv : 2;  /* bit[30-31]: isp_himinibus_tcmdma_sport_1to2_dlock_slv  */
    } reg;
} SOC_MEDIA2_CRG_ISP_READ_BACK0_UNION;
#endif
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_2to5_dlock_mst_START               (0)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_2to5_dlock_mst_END                 (3)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_2to5_dlock_slv_START               (4)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_2to5_dlock_slv_END                 (13)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_1to2_dlock_mst_START        (14)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_1to2_dlock_mst_END          (15)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_1to2_dlock_slv_START        (16)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_1to2_dlock_slv_END          (19)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_mport_2to1_dlock_mst_START  (20)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_mport_2to1_dlock_mst_END    (23)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_mport_2to1_dlock_slv_START  (24)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_mport_2to1_dlock_slv_END    (25)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_sport_2to1_dlock_mst_START  (26)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_sport_2to1_dlock_mst_END    (29)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_sport_2to1_dlock_slv_START  (30)
#define SOC_MEDIA2_CRG_ISP_READ_BACK0_isp_himinibus_tcmdma_sport_2to1_dlock_slv_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISP_READ_BACK1_UNION
 struct description   : ISP_READ_BACK1 Register structure definition
                        Address Offset:0x40C Initial:0x0000 Width:32
 register description : isp回读寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_himinibus_2to5_dbg_aw_err : 4;  /* bit[0-3]  : isp_himinibus_2to5_dbg_aw_err 100 06新增 */
        unsigned int  isp_himinibus_2to5_dbg_ar_err : 4;  /* bit[4-7]  : isp_himinibus_2to5_dbg_ar_err 100 06新增 */
        unsigned int  reserved_0                    : 8;  /* bit[8-15] : 保留。 */
        unsigned int  reserved_1                    : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_ISP_READ_BACK1_UNION;
#endif
#define SOC_MEDIA2_CRG_ISP_READ_BACK1_isp_himinibus_2to5_dbg_aw_err_START  (0)
#define SOC_MEDIA2_CRG_ISP_READ_BACK1_isp_himinibus_2to5_dbg_aw_err_END    (3)
#define SOC_MEDIA2_CRG_ISP_READ_BACK1_isp_himinibus_2to5_dbg_ar_err_START  (4)
#define SOC_MEDIA2_CRG_ISP_READ_BACK1_isp_himinibus_2to5_dbg_ar_err_END    (7)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_UNION
 struct description   : ISP_HIMINIBUS_CTRL Register structure definition
                        Address Offset:0x410 Initial:0x0000 Width:32
 register description : isp himinibus控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_himinibus_2to5_slv_lp_en        : 5;  /* bit[0-4]  : isp_himinibus_2to5_slv_lp_en 100 07新增 */
        unsigned int  isp_himinibus_2to5_dec_lp_en        : 1;  /* bit[5]    : isp_himinibus_2to5_dec_lp_en 100 07新增 */
        unsigned int  isp_himinibus_2to5_mst_err_srst_req : 1;  /* bit[6]    : isp_himinibus_2to5_mst_err_srst_req 100 07新增 */
        unsigned int  isp_himinibus_2to5_mst_pri_m1       : 1;  /* bit[7]    : isp_himinibus_2to5_mst_pri_m1 100 07新增 */
        unsigned int  isp_himinibus_2to5_mst_pri_m2       : 1;  /* bit[8]    : isp_himinibus_2to5_mst_pri_m2 100 07新增 */
        unsigned int  isp_himinibus_2to5_mst_pri_upd      : 1;  /* bit[9]    : isp_himinibus_2to5_mst_pri_upd 100 07新增 */
        unsigned int  reserved_0                          : 1;  /* bit[10]   : 保留。 */
        unsigned int  reserved_1                          : 1;  /* bit[11]   : 保留。 */
        unsigned int  reserved_2                          : 4;  /* bit[12-15]: 保留。 */
        unsigned int  reserved_3                          : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_UNION;
#endif
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_slv_lp_en_START         (0)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_slv_lp_en_END           (4)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_dec_lp_en_START         (5)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_dec_lp_en_END           (5)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_err_srst_req_START  (6)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_err_srst_req_END    (6)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_pri_m1_START        (7)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_pri_m1_END          (7)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_pri_m2_START        (8)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_pri_m2_END          (8)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_pri_upd_START       (9)
#define SOC_MEDIA2_CRG_ISP_HIMINIBUS_CTRL_isp_himinibus_2to5_mst_pri_upd_END         (9)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_UNION
 struct description   : HIMINIBUS_DLOCK_CLR Register structure definition
                        Address Offset:0x414 Initial:0x0000 Width:32
 register description : himinibus dlock clr寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipp_2to3_hiaxi_dlock_clr : 1;  /* bit[0]    : ipp_2to3_hiaxi_dlock_clr信号100 06新增 */
        unsigned int  reserved_0               : 15; /* bit[1-15] : 保留。 */
        unsigned int  reserved_1               : 16; /* bit[16-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_UNION;
#endif
#define SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_ipp_2to3_hiaxi_dlock_clr_START  (0)
#define SOC_MEDIA2_CRG_HIMINIBUS_DLOCK_CLR_ipp_2to3_hiaxi_dlock_clr_END    (0)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_IPP_READ_BACK_UNION
 struct description   : IPP_READ_BACK Register structure definition
                        Address Offset:0x418 Initial:0x000 Width:32
 register description : ipp 2to3桥回读寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipp_dlock_mst             : 4;  /* bit[0-3]  : ipp_dlock_mst */
        unsigned int  ipp_dlock_slv             : 6;  /* bit[4-9]  : ipp_dlock_slv */
        unsigned int  ipp_hiaxi_idle            : 1;  /* bit[10]   : ipp_hiaxi_idle */
        unsigned int  ipp_himinibus_x2p_idle    : 1;  /* bit[11]   : ipp_himinibus_x2p_idle 100 06新增 */
        unsigned int  ipp_2to3_himinibus_dbg_ar : 4;  /* bit[12-15]: ipp_2to3_himinibus_dbg_ar 100 06新增 */
        unsigned int  ipp_2to3_himinibus_dbg_aw : 4;  /* bit[16-19]: ipp_2to3_himinibus_dbg_aw 100 06新增 */
        unsigned int  ipp_himinibus_x2x_idle    : 1;  /* bit[20]   : ipp_himinibus_x2x_idle */
        unsigned int  reserved                  : 11; /* bit[21-31]: reserve */
    } reg;
} SOC_MEDIA2_CRG_IPP_READ_BACK_UNION;
#endif
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_dlock_mst_START              (0)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_dlock_mst_END                (3)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_dlock_slv_START              (4)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_dlock_slv_END                (9)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_hiaxi_idle_START             (10)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_hiaxi_idle_END               (10)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_himinibus_x2p_idle_START     (11)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_himinibus_x2p_idle_END       (11)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_2to3_himinibus_dbg_ar_START  (12)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_2to3_himinibus_dbg_ar_END    (15)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_2to3_himinibus_dbg_aw_START  (16)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_2to3_himinibus_dbg_aw_END    (19)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_himinibus_x2x_idle_START     (20)
#define SOC_MEDIA2_CRG_IPP_READ_BACK_ipp_himinibus_x2x_idle_END       (20)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_UNION
 struct description   : PERRSTEN_GENERAL_SEC Register structure definition
                        Address Offset:0xC00 Initial:0x0000000 Width:32
 register description : 通用安全软复位使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_miniisp      : 1;  /* bit[0]   : IP软复位使能：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位使能。 */
        unsigned int  ip_rst_idi2axi_csid : 1;  /* bit[1]   : IP软复位使能：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位使能。
                                                              只允许上下电时配置 */
        unsigned int  ip_rst_fp_idi       : 1;  /* bit[2]   : IP软复位使能：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位使能。 */
        unsigned int  ip_rst_miniisp_cfg  : 1;  /* bit[3]   : IP软复位使能：新增
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位使能。 */
        unsigned int  reserved            : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_miniisp_START       (0)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_miniisp_END         (0)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_idi2axi_csid_START  (1)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_idi2axi_csid_END    (1)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_fp_idi_START        (2)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_fp_idi_END          (2)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_miniisp_cfg_START   (3)
#define SOC_MEDIA2_CRG_PERRSTEN_GENERAL_SEC_ip_rst_miniisp_cfg_END     (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_UNION
 struct description   : PERRSTDIS_GENERAL_SEC Register structure definition
                        Address Offset:0xC04 Initial:0x0000000 Width:32
 register description : 通用安全软复位撤离寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_miniisp      : 1;  /* bit[0]   : IP软复位撤离：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位撤离。 */
        unsigned int  ip_rst_idi2axi_csid : 1;  /* bit[1]   : IP软复位撤离：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位撤离。
                                                              只允许上下电时配置 */
        unsigned int  ip_rst_fp_idi       : 1;  /* bit[2]   : IP软复位撤离：
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位撤离。 */
        unsigned int  ip_rst_miniisp_cfg  : 1;  /* bit[3]   : IP软复位使能：新增
                                                              0：IP软复位使能状态不变；
                                                              1：IP软复位使能。 */
        unsigned int  reserved            : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_miniisp_START       (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_miniisp_END         (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_idi2axi_csid_START  (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_idi2axi_csid_END    (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_fp_idi_START        (2)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_fp_idi_END          (2)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_miniisp_cfg_START   (3)
#define SOC_MEDIA2_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_miniisp_cfg_END     (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_UNION
 struct description   : PERRSTSTAT_GENERAL_SEC Register structure definition
                        Address Offset:0xC08 Initial:0x0000000 Width:32
 register description : 通用安全软复位使能状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_miniisp      : 1;  /* bit[0]   : IP软复位使能状态：
                                                              0：IP处于复位撤离状态；
                                                              1：IP处于软复位使能状态。 */
        unsigned int  ip_rst_idi2axi_csid : 1;  /* bit[1]   : IP软复位使能状态：
                                                              0：IP处于复位撤离状态；
                                                              1：IP处于软复位使能状态。 */
        unsigned int  ip_rst_fp_idi       : 1;  /* bit[2]   : IP软复位使能状态：
                                                              0：IP处于复位撤离状态；
                                                              1：IP处于软复位使能状态。 */
        unsigned int  ip_rst_miniisp_cfg  : 1;  /* bit[3]   : IP软复位使能状态：新增
                                                              0：IP处于复位撤离状态；
                                                              1：IP处于软复位使能状态。 */
        unsigned int  reserved            : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_miniisp_START       (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_miniisp_END         (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_idi2axi_csid_START  (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_idi2axi_csid_END    (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_fp_idi_START        (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_fp_idi_END          (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_miniisp_cfg_START   (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_miniisp_cfg_END     (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_UNION
 struct description   : PEREN0_GENERAL_SEC Register structure definition
                        Address Offset:0xC30 Initial:0x0000000 Width:32
 register description : 外设时钟安全使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_idi2axi_csid : 1;  /* bit[0]   :  */
        unsigned int  gt_clk_fp_idi       : 1;  /* bit[1]   :  */
        unsigned int  gt_clk_miniisp      : 1;  /* bit[2]   :  */
        unsigned int  gt_pclk_miniisp     : 1;  /* bit[3]   :  */
        unsigned int  reserved            : 28; /* bit[4-31]: 外设时钟使能控制：
                                                              0：写0无效果；
                                                              1：使能IP时钟。 */
    } reg;
} SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_clk_idi2axi_csid_START  (0)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_clk_idi2axi_csid_END    (0)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_clk_fp_idi_START        (1)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_clk_fp_idi_END          (1)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_clk_miniisp_START       (2)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_clk_miniisp_END         (2)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_pclk_miniisp_START      (3)
#define SOC_MEDIA2_CRG_PEREN0_GENERAL_SEC_gt_pclk_miniisp_END        (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_UNION
 struct description   : PERDIS0_GENERAL_SEC Register structure definition
                        Address Offset:0xC34 Initial:0x0000000 Width:32
 register description : 外设时钟安全禁止寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_idi2axi_csid : 1;  /* bit[0]   :  */
        unsigned int  gt_clk_fp_idi       : 1;  /* bit[1]   :  */
        unsigned int  gt_clk_miniisp      : 1;  /* bit[2]   :  */
        unsigned int  gt_pclk_miniisp     : 1;  /* bit[3]   :  */
        unsigned int  reserved            : 28; /* bit[4-31]: 外设时钟禁止控制：
                                                              0：写0无效果；
                                                              1：禁止IP时钟。 */
    } reg;
} SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_clk_idi2axi_csid_START  (0)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_clk_idi2axi_csid_END    (0)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_clk_fp_idi_START        (1)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_clk_fp_idi_END          (1)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_clk_miniisp_START       (2)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_clk_miniisp_END         (2)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_pclk_miniisp_START      (3)
#define SOC_MEDIA2_CRG_PERDIS0_GENERAL_SEC_gt_pclk_miniisp_END        (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_UNION
 struct description   : PERCLKEN0_GENERAL_SEC Register structure definition
                        Address Offset:0xC38 Initial:0x0000000 Width:32
 register description : 外设时钟安全使能状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_idi2axi_csid : 1;  /* bit[0]   :  */
        unsigned int  gt_clk_fp_idi       : 1;  /* bit[1]   :  */
        unsigned int  gt_clk_miniisp      : 1;  /* bit[2]   :  */
        unsigned int  gt_pclk_miniisp     : 1;  /* bit[3]   :  */
        unsigned int  reserved            : 28; /* bit[4-31]: 外设时钟使能状态：
                                                              0：IP时钟使能撤销状态；
                                                              1：IP时钟使能状态。 */
    } reg;
} SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_idi2axi_csid_START  (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_idi2axi_csid_END    (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_fp_idi_START        (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_fp_idi_END          (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_miniisp_START       (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_miniisp_END         (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_pclk_miniisp_START      (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_GENERAL_SEC_gt_pclk_miniisp_END        (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_UNION
 struct description   : PERSTAT0_GENERAL_SEC Register structure definition
                        Address Offset:0xC3C Initial:0x0000000 Width:32
 register description : 外设时钟安全最终状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_idi2axi_csid : 1;  /* bit[0]   :  */
        unsigned int  st_clk_fp_idi       : 1;  /* bit[1]   :  */
        unsigned int  st_clk_miniisp      : 1;  /* bit[2]   :  */
        unsigned int  st_pclk_miniisp     : 1;  /* bit[3]   :  */
        unsigned int  reserved            : 28; /* bit[4-31]: 外设时钟最终状态：
                                                              0：IP时钟禁止状态；
                                                              1：IP时钟使能状态。 */
    } reg;
} SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_clk_idi2axi_csid_START  (0)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_clk_idi2axi_csid_END    (0)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_clk_fp_idi_START        (1)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_clk_fp_idi_END          (1)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_clk_miniisp_START       (2)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_clk_miniisp_END         (2)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_pclk_miniisp_START      (3)
#define SOC_MEDIA2_CRG_PERSTAT0_GENERAL_SEC_st_pclk_miniisp_END        (3)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_UNION
 struct description   : PERRSTEN_ISP_SEC Register structure definition
                        Address Offset:0xD00 Initial:0x0000 Width:32
 register description : ISP安全软复位使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_isp             : 1;  /* bit[0]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_arst_isp            : 1;  /* bit[1]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_csi_idi         : 1;  /* bit[2]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_arst_ipp            : 1;  /* bit[3]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_prst_ipp            : 1;  /* bit[4]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_qic_cfg     : 1;  /* bit[5]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_i2c         : 1;  /* bit[6]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_i3c         : 1;  /* bit[7]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_csi_wrapper_cfg : 1;  /* bit[8]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_ipp             : 1;  /* bit[9]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_ispcpu          : 1;  /* bit[10]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_subsys_cfg  : 1;  /* bit[11]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_0             : 1;  /* bit[12]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_1             : 1;  /* bit[13]   :  */
        unsigned int  reserved_2             : 1;  /* bit[14]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_3             : 1;  /* bit[15]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_4             : 1;  /* bit[16]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_5             : 1;  /* bit[17]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_6             : 14; /* bit[18-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_START              (0)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_END                (0)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_arst_isp_START             (1)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_arst_isp_END               (1)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_csi_idi_START          (2)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_csi_idi_END            (2)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_arst_ipp_START             (3)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_arst_ipp_END               (3)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_prst_ipp_START             (4)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_prst_ipp_END               (4)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_qic_cfg_START      (5)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_qic_cfg_END        (5)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i2c_START          (6)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i2c_END            (6)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i3c_START          (7)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i3c_END            (7)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_csi_wrapper_cfg_START  (8)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_csi_wrapper_cfg_END    (8)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_ipp_START              (9)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_ipp_END                (9)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_ispcpu_START           (10)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_ispcpu_END             (10)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_subsys_cfg_START   (11)
#define SOC_MEDIA2_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_subsys_cfg_END     (11)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_UNION
 struct description   : PERRSTDIS_ISP_SEC Register structure definition
                        Address Offset:0xD04 Initial:0x0000 Width:32
 register description : ISP安全软复位撤离寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_isp             : 1;  /* bit[0]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_arst_isp            : 1;  /* bit[1]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_csi_idi         : 1;  /* bit[2]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_arst_ipp            : 1;  /* bit[3]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_prst_ipp            : 1;  /* bit[4]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_qic_cfg     : 1;  /* bit[5]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_i2c         : 1;  /* bit[6]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_i3c         : 1;  /* bit[7]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_csi_wrapper_cfg : 1;  /* bit[8]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_ipp             : 1;  /* bit[9]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_ispcpu          : 1;  /* bit[10]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_subsys_cfg  : 1;  /* bit[11]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_0             : 1;  /* bit[12]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_1             : 1;  /* bit[13]   :  */
        unsigned int  reserved_2             : 1;  /* bit[14]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_3             : 1;  /* bit[15]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_4             : 1;  /* bit[16]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_5             : 1;  /* bit[17]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_6             : 14; /* bit[18-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_START              (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_END                (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_arst_isp_START             (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_arst_isp_END               (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_csi_idi_START          (2)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_csi_idi_END            (2)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_arst_ipp_START             (3)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_arst_ipp_END               (3)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_prst_ipp_START             (4)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_prst_ipp_END               (4)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_qic_cfg_START      (5)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_qic_cfg_END        (5)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i2c_START          (6)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i2c_END            (6)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i3c_START          (7)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i3c_END            (7)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_csi_wrapper_cfg_START  (8)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_csi_wrapper_cfg_END    (8)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_ipp_START              (9)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_ipp_END                (9)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_ispcpu_START           (10)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_ispcpu_END             (10)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_subsys_cfg_START   (11)
#define SOC_MEDIA2_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_subsys_cfg_END     (11)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_UNION
 struct description   : PERRSTSTAT_ISP_SEC Register structure definition
                        Address Offset:0xD08 Initial:0x0000 Width:32
 register description : ISP安全软复位状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_rst_isp             : 1;  /* bit[0]    : IP软复位使能状态：
                                                                  0：IP处于复位撤离状态；
                                                                  1：IP处于软复位使能状态。 */
        unsigned int  ip_arst_isp            : 1;  /* bit[1]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_csi_idi         : 1;  /* bit[2]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_arst_ipp            : 1;  /* bit[3]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_prst_ipp            : 1;  /* bit[4]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_qic_cfg     : 1;  /* bit[5]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_i2c         : 1;  /* bit[6]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_i3c         : 1;  /* bit[7]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_csi_wrapper_cfg : 1;  /* bit[8]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_ipp             : 1;  /* bit[9]    : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_ispcpu          : 1;  /* bit[10]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  ip_rst_isp_subsys_cfg  : 1;  /* bit[11]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_0             : 1;  /* bit[12]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_1             : 1;  /* bit[13]   :  */
        unsigned int  reserved_2             : 1;  /* bit[14]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_3             : 1;  /* bit[15]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_4             : 1;  /* bit[16]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_5             : 1;  /* bit[17]   : IP软复位使能：新增
                                                                  0：IP软复位使能状态不变；
                                                                  1：IP软复位使能。 */
        unsigned int  reserved_6             : 14; /* bit[18-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_START              (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_END                (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_arst_isp_START             (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_arst_isp_END               (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_csi_idi_START          (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_csi_idi_END            (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_arst_ipp_START             (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_arst_ipp_END               (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_prst_ipp_START             (4)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_prst_ipp_END               (4)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_qic_cfg_START      (5)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_qic_cfg_END        (5)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i2c_START          (6)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i2c_END            (6)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i3c_START          (7)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i3c_END            (7)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_csi_wrapper_cfg_START  (8)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_csi_wrapper_cfg_END    (8)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_ipp_START              (9)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_ipp_END                (9)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_ispcpu_START           (10)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_ispcpu_END             (10)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_subsys_cfg_START   (11)
#define SOC_MEDIA2_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_subsys_cfg_END     (11)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_UNION
 struct description   : ISPCPU_CTRL0_SEC Register structure definition
                        Address Offset:0xD10 Initial:0x00015858 Width:32
 register description : ISPCPU控制配置寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  isp_arc_mem_ctrl_s : 32; /* bit[0-31]: Charlotte ES修改
                                                             CPU 内部RAM控制信号。
                                                             [6] LS Light sleep mode,active high
                                                             [7] DS Deep sleep mode,active high
                                                             [8] SD Shut down mode,active high
                                                             [11:9] TSELR 调节读，默认值011
                                                             [13:12] TSELW 调节写，默认值01
                                                             [16:14] TEST 测试pin，默认值000
                                                             [19:17] TSELR 调节读，默认值011
                                                             [21:20] TSELW 调节写，默认值01
                                                             [23:22] TRA 读assist，仅针对DR类型mem，默认值00 */
    } reg;
} SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_UNION;
#endif
#define SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_isp_arc_mem_ctrl_s_START  (0)
#define SOC_MEDIA2_CRG_ISPCPU_CTRL0_SEC_isp_arc_mem_ctrl_s_END    (31)


/*****************************************************************************
 struct               : SOC_MEDIA2_CRG_DECPROT0SET_UNION
 struct description   : DECPROT0SET Register structure definition
                        Address Offset:0xF00 Initial:0x00000000 Width:32
 register description : 绝对安全寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   : 配置的安全位
                                                    0：安全配置；
                                                    1：非安全配置。 */
        unsigned int  reserved_1: 1;  /* bit[1]   : 配置的安全位
                                                    0：安全配置；
                                                    1：非安全配置。 */
        unsigned int  reserved_2: 1;  /* bit[2]   : 配置的安全位
                                                    0：安全配置；
                                                    1：非安全配置。 */
        unsigned int  reserved_3: 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_MEDIA2_CRG_DECPROT0SET_UNION;
#endif






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

#endif /* end of soc_media2_crg_interface.h */
