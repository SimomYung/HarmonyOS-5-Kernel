

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_HSDT1_CRG_INTERFACE_H__
#define __SOC_HSDT1_CRG_INTERFACE_H__

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
   Bit domain definition UNION:  SOC_HSDT1_CRG_PEREN0_UNION */
#define SOC_HSDT1_CRG_PEREN0_ADDR(base)                       (base)

/* Register description: 外设时钟禁止寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERDIS0_UNION */
#define SOC_HSDT1_CRG_PERDIS0_ADDR(base)                      ((base) + 0x004UL)

/* Register description: 外设时钟使能状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERCLKEN0_UNION */
#define SOC_HSDT1_CRG_PERCLKEN0_ADDR(base)                    ((base) + 0x008UL)

/* Register description: 外设时钟最终状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERSTAT0_UNION */
#define SOC_HSDT1_CRG_PERSTAT0_ADDR(base)                     ((base) + 0x00CUL)

/* Register description: 外设时钟使能寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PEREN1_UNION */
#define SOC_HSDT1_CRG_PEREN1_ADDR(base)                       ((base) + 0x010UL)

/* Register description: 外设时钟禁止寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERDIS1_UNION */
#define SOC_HSDT1_CRG_PERDIS1_ADDR(base)                      ((base) + 0x014UL)

/* Register description: 外设时钟使能状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERCLKEN1_UNION */
#define SOC_HSDT1_CRG_PERCLKEN1_ADDR(base)                    ((base) + 0x018UL)

/* Register description: 外设时钟最终状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERSTAT1_UNION */
#define SOC_HSDT1_CRG_PERSTAT1_ADDR(base)                     ((base) + 0x01CUL)

/* Register description: 外设软复位使能寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERRSTEN0_UNION */
#define SOC_HSDT1_CRG_PERRSTEN0_ADDR(base)                    ((base) + 0x020UL)

/* Register description: 外设软复位撤离寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERRSTDIS0_UNION */
#define SOC_HSDT1_CRG_PERRSTDIS0_ADDR(base)                   ((base) + 0x024UL)

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERRSTSTAT0_UNION */
#define SOC_HSDT1_CRG_PERRSTSTAT0_ADDR(base)                  ((base) + 0x028UL)

/* Register description: 时钟分频比控制寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_CLKDIV0_UNION */
#define SOC_HSDT1_CRG_CLKDIV0_ADDR(base)                      ((base) + 0x0A8UL)

/* Register description: 时钟分频比控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_CLKDIV1_UNION */
#define SOC_HSDT1_CRG_CLKDIV1_ADDR(base)                      ((base) + 0x0ACUL)

/* Register description: 时钟分频比控制寄存器2。
   Bit domain definition UNION:  SOC_HSDT1_CRG_CLKDIV2_UNION */
#define SOC_HSDT1_CRG_CLKDIV2_ADDR(base)                      ((base) + 0x0B0UL)

/* Register description: 外设状态寄存器2。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_STAT0_UNION */
#define SOC_HSDT1_CRG_PERI_STAT0_ADDR(base)                   ((base) + 0x0C8UL)

/* Register description: 外设状态寄存器2。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_STAT1_UNION */
#define SOC_HSDT1_CRG_PERI_STAT1_ADDR(base)                   ((base) + 0x0CCUL)

/* Register description: 外设IP防挂死功能byapss控制寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_IPCLKRST_BYPASS0_UNION */
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_ADDR(base)             ((base) + 0x100UL)

/* Register description: SDPLL SSC控制寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_SDPLLSSCCTRL_UNION */
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_ADDR(base)                 ((base) + 0x190UL)

/* Register description: SDPLL控制寄存器0
   Bit domain definition UNION:  SOC_HSDT1_CRG_SDPLLCTRL0_UNION */
#define SOC_HSDT1_CRG_SDPLLCTRL0_ADDR(base)                   ((base) + 0x200UL)

/* Register description: SDPLL控制寄存器1
   Bit domain definition UNION:  SOC_HSDT1_CRG_SDPLLCTRL1_UNION */
#define SOC_HSDT1_CRG_SDPLLCTRL1_ADDR(base)                   ((base) + 0x204UL)

/* Register description: PCIE SCPLL系统状态寄存器0寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_PCIEPLL_STAT_UNION */
#define SOC_HSDT1_CRG_PCIEPLL_STAT_ADDR(base)                 ((base) + 0x208UL)

/* Register description: FNPLL系统控制寄存器0寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG0_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG0_ADDR(base)                   ((base) + 0x224UL)

/* Register description: FNPLL系统控制寄存器1寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG1_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG1_ADDR(base)                   ((base) + 0x228UL)

/* Register description: FNPLL系统控制寄存器2寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG2_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG2_ADDR(base)                   ((base) + 0x22CUL)

/* Register description: FNPLL系统控制寄存器3寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG3_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG3_ADDR(base)                   ((base) + 0x230UL)

/* Register description: 外设自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_AUTODIV0_UNION */
#define SOC_HSDT1_CRG_PERI_AUTODIV0_ADDR(base)                ((base) + 0x350UL)

/* Register description: 外设自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_AUTODIV1_UNION */
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ADDR(base)                ((base) + 0x354UL)

/* Register description: 外设自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_AUTODIV2_UNION */
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ADDR(base)                ((base) + 0x358UL)

/* Register description: 掉电子系统ISO使能寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_ISOEN_UNION */
#define SOC_HSDT1_CRG_ISOEN_ADDR(base)                        ((base) + 0x280UL)

/* Register description: 掉电子系统ISO使能撤销寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_ISODIS_UNION */
#define SOC_HSDT1_CRG_ISODIS_ADDR(base)                       ((base) + 0x284UL)

/* Register description: 掉电子系统ISO使能状态寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_ISOSTAT_UNION */
#define SOC_HSDT1_CRG_ISOSTAT_ADDR(base)                      ((base) + 0x288UL)

/* Register description: 自动降频状态寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_UNION */
#define SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_ADDR(base)           ((base) + 0x400UL)

/* Register description: FNPLL系统状态寄存器0(SD)
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_STAT0_SD_UNION */
#define SOC_HSDT1_CRG_FNPLL_STAT0_SD_ADDR(base)               ((base) + 0x408UL)

/* Register description: NOC总线异常中断屏蔽寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION */
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + 0x500UL)

/* Register description: NOC总线异常中断清除寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION */
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + 0x504UL)

/* Register description: NOC总线异常中断状态寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION */
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + 0x508UL)


#else


/* Register description: 外设时钟使能寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PEREN0_UNION */
#define SOC_HSDT1_CRG_PEREN0_ADDR(base)                       (base)

/* Register description: 外设时钟禁止寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERDIS0_UNION */
#define SOC_HSDT1_CRG_PERDIS0_ADDR(base)                      ((base) + 0x004)

/* Register description: 外设时钟使能状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERCLKEN0_UNION */
#define SOC_HSDT1_CRG_PERCLKEN0_ADDR(base)                    ((base) + 0x008)

/* Register description: 外设时钟最终状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERSTAT0_UNION */
#define SOC_HSDT1_CRG_PERSTAT0_ADDR(base)                     ((base) + 0x00C)

/* Register description: 外设时钟使能寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PEREN1_UNION */
#define SOC_HSDT1_CRG_PEREN1_ADDR(base)                       ((base) + 0x010)

/* Register description: 外设时钟禁止寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERDIS1_UNION */
#define SOC_HSDT1_CRG_PERDIS1_ADDR(base)                      ((base) + 0x014)

/* Register description: 外设时钟使能状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERCLKEN1_UNION */
#define SOC_HSDT1_CRG_PERCLKEN1_ADDR(base)                    ((base) + 0x018)

/* Register description: 外设时钟最终状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERSTAT1_UNION */
#define SOC_HSDT1_CRG_PERSTAT1_ADDR(base)                     ((base) + 0x01C)

/* Register description: 外设软复位使能寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERRSTEN0_UNION */
#define SOC_HSDT1_CRG_PERRSTEN0_ADDR(base)                    ((base) + 0x020)

/* Register description: 外设软复位撤离寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERRSTDIS0_UNION */
#define SOC_HSDT1_CRG_PERRSTDIS0_ADDR(base)                   ((base) + 0x024)

/* Register description: 外设软复位状态寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERRSTSTAT0_UNION */
#define SOC_HSDT1_CRG_PERRSTSTAT0_ADDR(base)                  ((base) + 0x028)

/* Register description: 时钟分频比控制寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_CLKDIV0_UNION */
#define SOC_HSDT1_CRG_CLKDIV0_ADDR(base)                      ((base) + 0x0A8)

/* Register description: 时钟分频比控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_CLKDIV1_UNION */
#define SOC_HSDT1_CRG_CLKDIV1_ADDR(base)                      ((base) + 0x0AC)

/* Register description: 时钟分频比控制寄存器2。
   Bit domain definition UNION:  SOC_HSDT1_CRG_CLKDIV2_UNION */
#define SOC_HSDT1_CRG_CLKDIV2_ADDR(base)                      ((base) + 0x0B0)

/* Register description: 外设状态寄存器2。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_STAT0_UNION */
#define SOC_HSDT1_CRG_PERI_STAT0_ADDR(base)                   ((base) + 0x0C8)

/* Register description: 外设状态寄存器2。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_STAT1_UNION */
#define SOC_HSDT1_CRG_PERI_STAT1_ADDR(base)                   ((base) + 0x0CC)

/* Register description: 外设IP防挂死功能byapss控制寄存器0。
   Bit domain definition UNION:  SOC_HSDT1_CRG_IPCLKRST_BYPASS0_UNION */
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_ADDR(base)             ((base) + 0x100)

/* Register description: SDPLL SSC控制寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_SDPLLSSCCTRL_UNION */
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_ADDR(base)                 ((base) + 0x190)

/* Register description: SDPLL控制寄存器0
   Bit domain definition UNION:  SOC_HSDT1_CRG_SDPLLCTRL0_UNION */
#define SOC_HSDT1_CRG_SDPLLCTRL0_ADDR(base)                   ((base) + 0x200)

/* Register description: SDPLL控制寄存器1
   Bit domain definition UNION:  SOC_HSDT1_CRG_SDPLLCTRL1_UNION */
#define SOC_HSDT1_CRG_SDPLLCTRL1_ADDR(base)                   ((base) + 0x204)

/* Register description: PCIE SCPLL系统状态寄存器0寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_PCIEPLL_STAT_UNION */
#define SOC_HSDT1_CRG_PCIEPLL_STAT_ADDR(base)                 ((base) + 0x208)

/* Register description: FNPLL系统控制寄存器0寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG0_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG0_ADDR(base)                   ((base) + 0x224)

/* Register description: FNPLL系统控制寄存器1寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG1_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG1_ADDR(base)                   ((base) + 0x228)

/* Register description: FNPLL系统控制寄存器2寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG2_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG2_ADDR(base)                   ((base) + 0x22C)

/* Register description: FNPLL系统控制寄存器3寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_CFG3_UNION */
#define SOC_HSDT1_CRG_FNPLL_CFG3_ADDR(base)                   ((base) + 0x230)

/* Register description: 外设自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_AUTODIV0_UNION */
#define SOC_HSDT1_CRG_PERI_AUTODIV0_ADDR(base)                ((base) + 0x350)

/* Register description: 外设自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_AUTODIV1_UNION */
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ADDR(base)                ((base) + 0x354)

/* Register description: 外设自动降频控制寄存器1。
   Bit domain definition UNION:  SOC_HSDT1_CRG_PERI_AUTODIV2_UNION */
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ADDR(base)                ((base) + 0x358)

/* Register description: 掉电子系统ISO使能寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_ISOEN_UNION */
#define SOC_HSDT1_CRG_ISOEN_ADDR(base)                        ((base) + 0x280)

/* Register description: 掉电子系统ISO使能撤销寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_ISODIS_UNION */
#define SOC_HSDT1_CRG_ISODIS_ADDR(base)                       ((base) + 0x284)

/* Register description: 掉电子系统ISO使能状态寄存器。
   Bit domain definition UNION:  SOC_HSDT1_CRG_ISOSTAT_UNION */
#define SOC_HSDT1_CRG_ISOSTAT_ADDR(base)                      ((base) + 0x288)

/* Register description: 自动降频状态寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_UNION */
#define SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_ADDR(base)           ((base) + 0x400)

/* Register description: FNPLL系统状态寄存器0(SD)
   Bit domain definition UNION:  SOC_HSDT1_CRG_FNPLL_STAT0_SD_UNION */
#define SOC_HSDT1_CRG_FNPLL_STAT0_SD_ADDR(base)               ((base) + 0x408)

/* Register description: NOC总线异常中断屏蔽寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION */
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + 0x500)

/* Register description: NOC总线异常中断清除寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION */
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + 0x504)

/* Register description: NOC总线异常中断状态寄存器
   Bit domain definition UNION:  SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION */
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + 0x508)


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
 struct               : SOC_HSDT1_CRG_PEREN0_UNION
 struct description   : PEREN0 Register structure definition
                        Address Offset:0x000 Initial:0x0 Width:32
 register description : 外设时钟使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_hclk_sd             : 1;  /* bit[0] :  */
        unsigned int  gt_pclk_hsdt1_sysctrl  : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_usb2phy        : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_dpctrl         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_hsdt1_ioc      : 1;  /* bit[4] :  */
        unsigned int  reserved_0             : 1;  /* bit[5] :  */
        unsigned int  reserved_1             : 1;  /* bit[6] :  */
        unsigned int  reserved_2             : 1;  /* bit[7] :  */
        unsigned int  reserved_3             : 1;  /* bit[8] :  */
        unsigned int  gt_clk_hi_usb3ctrl_ref : 1;  /* bit[9] :  */
        unsigned int  reserved_4             : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_usb3        : 1;  /* bit[11]:  */
        unsigned int  gt_aclk_hi_usb3        : 1;  /* bit[12]:  */
        unsigned int  reserved_5             : 1;  /* bit[13]:  */
        unsigned int  gt_clk_hi_usb3_suspend : 1;  /* bit[14]:  */
        unsigned int  gt_clk_hi_usb3_sys     : 1;  /* bit[15]:  */
        unsigned int  reserved_6             : 1;  /* bit[16]:  */
        unsigned int  reserved_7             : 1;  /* bit[17]:  */
        unsigned int  gt_clk_usb2phy_ref     : 1;  /* bit[18]:  */
        unsigned int  reserved_8             : 1;  /* bit[19]:  */
        unsigned int  reserved_9             : 1;  /* bit[20]:  */
        unsigned int  reserved_10            : 1;  /* bit[21]:  */
        unsigned int  reserved_11            : 1;  /* bit[22]:  */
        unsigned int  gt_clk_usbdp_aux       : 1;  /* bit[23]:  */
        unsigned int  reserved_12            : 1;  /* bit[24]:  */
        unsigned int  reserved_13            : 1;  /* bit[25]:  */
        unsigned int  gt_clk_usb31phy_apb    : 1;  /* bit[26]:  */
        unsigned int  reserved_14            : 1;  /* bit[27]:  */
        unsigned int  reserved_15            : 1;  /* bit[28]:  */
        unsigned int  gt_pclk_usb_sctrl      : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 外设时钟使能控制：
                                                               0：写0无效果；
                                                               1：使能IP时钟。 */
    } reg;
} SOC_HSDT1_CRG_PEREN0_UNION;
#endif
#define SOC_HSDT1_CRG_PEREN0_gt_hclk_sd_START              (0)
#define SOC_HSDT1_CRG_PEREN0_gt_hclk_sd_END                (0)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_sysctrl_START   (1)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_sysctrl_END     (1)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_usb2phy_START         (2)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_usb2phy_END           (2)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_dpctrl_START          (3)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_dpctrl_END            (3)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_ioc_START       (4)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_ioc_END         (4)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hi_usb3ctrl_ref_START  (9)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hi_usb3ctrl_ref_END    (9)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_qic_usb3_START         (11)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_qic_usb3_END           (11)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_hi_usb3_START         (12)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_hi_usb3_END           (12)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hi_usb3_suspend_START  (14)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hi_usb3_suspend_END    (14)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hi_usb3_sys_START      (15)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hi_usb3_sys_END        (15)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb2phy_ref_START      (18)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb2phy_ref_END        (18)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usbdp_aux_START        (23)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usbdp_aux_END          (23)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb31phy_apb_START     (26)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb31phy_apb_END       (26)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_usb_sctrl_START       (29)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_usb_sctrl_END         (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERDIS0_UNION
 struct description   : PERDIS0 Register structure definition
                        Address Offset:0x004 Initial:0x0 Width:32
 register description : 外设时钟禁止寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_hclk_sd             : 1;  /* bit[0] :  */
        unsigned int  gt_pclk_hsdt1_sysctrl  : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_usb2phy        : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_dpctrl         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_hsdt1_ioc      : 1;  /* bit[4] :  */
        unsigned int  reserved_0             : 1;  /* bit[5] :  */
        unsigned int  reserved_1             : 1;  /* bit[6] :  */
        unsigned int  reserved_2             : 1;  /* bit[7] :  */
        unsigned int  reserved_3             : 1;  /* bit[8] :  */
        unsigned int  gt_clk_hi_usb3ctrl_ref : 1;  /* bit[9] :  */
        unsigned int  reserved_4             : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_usb3        : 1;  /* bit[11]:  */
        unsigned int  gt_aclk_hi_usb3        : 1;  /* bit[12]:  */
        unsigned int  reserved_5             : 1;  /* bit[13]:  */
        unsigned int  gt_clk_hi_usb3_suspend : 1;  /* bit[14]:  */
        unsigned int  gt_clk_hi_usb3_sys     : 1;  /* bit[15]:  */
        unsigned int  reserved_6             : 1;  /* bit[16]:  */
        unsigned int  reserved_7             : 1;  /* bit[17]:  */
        unsigned int  gt_clk_usb2phy_ref     : 1;  /* bit[18]:  */
        unsigned int  reserved_8             : 1;  /* bit[19]:  */
        unsigned int  reserved_9             : 1;  /* bit[20]:  */
        unsigned int  reserved_10            : 1;  /* bit[21]:  */
        unsigned int  reserved_11            : 1;  /* bit[22]:  */
        unsigned int  gt_clk_usbdp_aux       : 1;  /* bit[23]:  */
        unsigned int  reserved_12            : 1;  /* bit[24]:  */
        unsigned int  reserved_13            : 1;  /* bit[25]:  */
        unsigned int  gt_clk_usb31phy_apb    : 1;  /* bit[26]:  */
        unsigned int  reserved_14            : 1;  /* bit[27]:  */
        unsigned int  reserved_15            : 1;  /* bit[28]:  */
        unsigned int  gt_pclk_usb_sctrl      : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 外设时钟禁止控制：
                                                               0：写0无效果；
                                                               1：禁止IP时钟。 */
    } reg;
} SOC_HSDT1_CRG_PERDIS0_UNION;
#endif
#define SOC_HSDT1_CRG_PERDIS0_gt_hclk_sd_START              (0)
#define SOC_HSDT1_CRG_PERDIS0_gt_hclk_sd_END                (0)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_sysctrl_START   (1)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_sysctrl_END     (1)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_usb2phy_START         (2)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_usb2phy_END           (2)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_dpctrl_START          (3)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_dpctrl_END            (3)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_ioc_START       (4)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_ioc_END         (4)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hi_usb3ctrl_ref_START  (9)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hi_usb3ctrl_ref_END    (9)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_qic_usb3_START         (11)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_qic_usb3_END           (11)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_hi_usb3_START         (12)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_hi_usb3_END           (12)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hi_usb3_suspend_START  (14)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hi_usb3_suspend_END    (14)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hi_usb3_sys_START      (15)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hi_usb3_sys_END        (15)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb2phy_ref_START      (18)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb2phy_ref_END        (18)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usbdp_aux_START        (23)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usbdp_aux_END          (23)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb31phy_apb_START     (26)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb31phy_apb_END       (26)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_usb_sctrl_START       (29)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_usb_sctrl_END         (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERCLKEN0_UNION
 struct description   : PERCLKEN0 Register structure definition
                        Address Offset:0x008 Initial:0x0 Width:32
 register description : 外设时钟使能状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_hclk_sd             : 1;  /* bit[0] :  */
        unsigned int  gt_pclk_hsdt1_sysctrl  : 1;  /* bit[1] :  */
        unsigned int  gt_pclk_usb2phy        : 1;  /* bit[2] :  */
        unsigned int  gt_pclk_dpctrl         : 1;  /* bit[3] :  */
        unsigned int  gt_pclk_hsdt1_ioc      : 1;  /* bit[4] :  */
        unsigned int  reserved_0             : 1;  /* bit[5] :  */
        unsigned int  reserved_1             : 1;  /* bit[6] :  */
        unsigned int  reserved_2             : 1;  /* bit[7] :  */
        unsigned int  reserved_3             : 1;  /* bit[8] :  */
        unsigned int  gt_clk_hi_usb3ctrl_ref : 1;  /* bit[9] :  */
        unsigned int  reserved_4             : 1;  /* bit[10]:  */
        unsigned int  gt_clk_qic_usb3        : 1;  /* bit[11]:  */
        unsigned int  gt_aclk_hi_usb3        : 1;  /* bit[12]:  */
        unsigned int  reserved_5             : 1;  /* bit[13]:  */
        unsigned int  gt_clk_hi_usb3_suspend : 1;  /* bit[14]:  */
        unsigned int  gt_clk_hi_usb3_sys     : 1;  /* bit[15]:  */
        unsigned int  reserved_6             : 1;  /* bit[16]:  */
        unsigned int  reserved_7             : 1;  /* bit[17]:  */
        unsigned int  gt_clk_usb2phy_ref     : 1;  /* bit[18]:  */
        unsigned int  reserved_8             : 1;  /* bit[19]:  */
        unsigned int  reserved_9             : 1;  /* bit[20]:  */
        unsigned int  reserved_10            : 1;  /* bit[21]:  */
        unsigned int  reserved_11            : 1;  /* bit[22]:  */
        unsigned int  gt_clk_usbdp_aux       : 1;  /* bit[23]:  */
        unsigned int  reserved_12            : 1;  /* bit[24]:  */
        unsigned int  reserved_13            : 1;  /* bit[25]:  */
        unsigned int  gt_clk_usb31phy_apb    : 1;  /* bit[26]: Baltimore CS USBDP_COMBPHY APB配置口时钟门控 */
        unsigned int  reserved_14            : 1;  /* bit[27]:  */
        unsigned int  reserved_15            : 1;  /* bit[28]:  */
        unsigned int  gt_pclk_usb_sctrl      : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 外设时钟使能状态：
                                                               0：IP时钟使能撤销状态；
                                                               1：IP时钟使能状态。 */
    } reg;
} SOC_HSDT1_CRG_PERCLKEN0_UNION;
#endif
#define SOC_HSDT1_CRG_PERCLKEN0_gt_hclk_sd_START              (0)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_hclk_sd_END                (0)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_sysctrl_START   (1)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_sysctrl_END     (1)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_usb2phy_START         (2)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_usb2phy_END           (2)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_dpctrl_START          (3)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_dpctrl_END            (3)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_ioc_START       (4)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_ioc_END         (4)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hi_usb3ctrl_ref_START  (9)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hi_usb3ctrl_ref_END    (9)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_qic_usb3_START         (11)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_qic_usb3_END           (11)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_hi_usb3_START         (12)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_hi_usb3_END           (12)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hi_usb3_suspend_START  (14)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hi_usb3_suspend_END    (14)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hi_usb3_sys_START      (15)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hi_usb3_sys_END        (15)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb2phy_ref_START      (18)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb2phy_ref_END        (18)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usbdp_aux_START        (23)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usbdp_aux_END          (23)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb31phy_apb_START     (26)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb31phy_apb_END       (26)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_usb_sctrl_START       (29)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_usb_sctrl_END         (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERSTAT0_UNION
 struct description   : PERSTAT0 Register structure definition
                        Address Offset:0x00C Initial:0x0 Width:32
 register description : 外设时钟最终状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_hclk_sd             : 1;  /* bit[0] :  */
        unsigned int  st_pclk_hsdt1_sysctrl  : 1;  /* bit[1] :  */
        unsigned int  st_pclk_usb2phy        : 1;  /* bit[2] :  */
        unsigned int  st_pclk_dpctrl         : 1;  /* bit[3] :  */
        unsigned int  st_pclk_hsdt1_ioc      : 1;  /* bit[4] :  */
        unsigned int  reserved_0             : 1;  /* bit[5] :  */
        unsigned int  reserved_1             : 1;  /* bit[6] :  */
        unsigned int  reserved_2             : 1;  /* bit[7] :  */
        unsigned int  reserved_3             : 1;  /* bit[8] :  */
        unsigned int  st_clk_hi_usb3ctrl_ref : 1;  /* bit[9] :  */
        unsigned int  reserved_4             : 1;  /* bit[10]:  */
        unsigned int  st_clk_qic_usb3        : 1;  /* bit[11]:  */
        unsigned int  st_aclk_hi_usb3        : 1;  /* bit[12]:  */
        unsigned int  reserved_5             : 1;  /* bit[13]:  */
        unsigned int  st_clk_hi_usb3_suspend : 1;  /* bit[14]:  */
        unsigned int  st_clk_hi_usb3_sys     : 1;  /* bit[15]:  */
        unsigned int  reserved_6             : 1;  /* bit[16]:  */
        unsigned int  reserved_7             : 1;  /* bit[17]:  */
        unsigned int  st_clk_usb2phy_ref     : 1;  /* bit[18]:  */
        unsigned int  reserved_8             : 1;  /* bit[19]:  */
        unsigned int  reserved_9             : 1;  /* bit[20]:  */
        unsigned int  reserved_10            : 1;  /* bit[21]:  */
        unsigned int  reserved_11            : 1;  /* bit[22]:  */
        unsigned int  st_clk_usbdp_aux       : 1;  /* bit[23]:  */
        unsigned int  reserved_12            : 1;  /* bit[24]:  */
        unsigned int  reserved_13            : 1;  /* bit[25]:  */
        unsigned int  st_clk_usb31phy_apb    : 1;  /* bit[26]:  */
        unsigned int  reserved_14            : 1;  /* bit[27]:  */
        unsigned int  reserved_15            : 1;  /* bit[28]:  */
        unsigned int  st_pclk_usb_sctrl      : 1;  /* bit[29]:  */
        unsigned int  reserved_16            : 1;  /* bit[30]:  */
        unsigned int  reserved_17            : 1;  /* bit[31]: 外设时钟最终状态：
                                                               0：IP时钟禁止状态；
                                                               1：IP时钟使能状态。 */
    } reg;
} SOC_HSDT1_CRG_PERSTAT0_UNION;
#endif
#define SOC_HSDT1_CRG_PERSTAT0_st_hclk_sd_START              (0)
#define SOC_HSDT1_CRG_PERSTAT0_st_hclk_sd_END                (0)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_sysctrl_START   (1)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_sysctrl_END     (1)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_usb2phy_START         (2)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_usb2phy_END           (2)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_dpctrl_START          (3)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_dpctrl_END            (3)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_ioc_START       (4)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_ioc_END         (4)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hi_usb3ctrl_ref_START  (9)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hi_usb3ctrl_ref_END    (9)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_qic_usb3_START         (11)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_qic_usb3_END           (11)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_hi_usb3_START         (12)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_hi_usb3_END           (12)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hi_usb3_suspend_START  (14)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hi_usb3_suspend_END    (14)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hi_usb3_sys_START      (15)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hi_usb3_sys_END        (15)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb2phy_ref_START      (18)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb2phy_ref_END        (18)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usbdp_aux_START        (23)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usbdp_aux_END          (23)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb31phy_apb_START     (26)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb31phy_apb_END       (26)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_usb_sctrl_START       (29)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_usb_sctrl_END         (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PEREN1_UNION
 struct description   : PEREN1 Register structure definition
                        Address Offset:0x010 Initial:0x00 Width:32
 register description : 外设时钟使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_ufs_mphy_sys_hsdt1            : 1;  /* bit[0]    :  */
        unsigned int  gt_clk_ufs_sys_subsys_hsdt1          : 1;  /* bit[1]    :  */
        unsigned int  gt_pclk_autodiv_ufs_sys_subsys_hsdt1 : 1;  /* bit[2]    :  */
        unsigned int  gt_clk_qic_ufs_trace                 : 1;  /* bit[3]    :  */
        unsigned int  gt_clk_qic_ufs_hci                   : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                           : 1;  /* bit[5]    :  */
        unsigned int  reserved_1                           : 1;  /* bit[6]    :  */
        unsigned int  reserved_2                           : 1;  /* bit[7]    :  */
        unsigned int  reserved_3                           : 1;  /* bit[8]    :  */
        unsigned int  reserved_4                           : 1;  /* bit[9]    :  */
        unsigned int  reserved_5                           : 1;  /* bit[10]   :  */
        unsigned int  reserved_6                           : 1;  /* bit[11]   :  */
        unsigned int  reserved_7                           : 1;  /* bit[12]   :  */
        unsigned int  reserved_8                           : 1;  /* bit[13]   :  */
        unsigned int  reserved_9                           : 1;  /* bit[14]   :  */
        unsigned int  reserved_10                          : 1;  /* bit[15]   :  */
        unsigned int  reserved_11                          : 1;  /* bit[16]   :  */
        unsigned int  reserved_12                          : 1;  /* bit[17]   :  */
        unsigned int  reserved_13                          : 1;  /* bit[18]   :  */
        unsigned int  reserved_14                          : 1;  /* bit[19]   :  */
        unsigned int  gt_clk_sdpll_sscg                    : 1;  /* bit[20]   :  */
        unsigned int  gt_clk_sd                            : 1;  /* bit[21]   :  */
        unsigned int  gt_clk_sdpll_tp                      : 1;  /* bit[22]   :  */
        unsigned int  gt_clk_sd_32k                        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15                          : 1;  /* bit[24]   :  */
        unsigned int  reserved_16                          : 1;  /* bit[25]   :  */
        unsigned int  reserved_17                          : 6;  /* bit[26-31]: 外设时钟使能控制：
                                                                                0：写0无效果；
                                                                                1：使能IP时钟。 */
    } reg;
} SOC_HSDT1_CRG_PEREN1_UNION;
#endif
#define SOC_HSDT1_CRG_PEREN1_gt_clk_ufs_mphy_sys_hsdt1_START             (0)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_ufs_mphy_sys_hsdt1_END               (0)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_ufs_sys_subsys_hsdt1_START           (1)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_ufs_sys_subsys_hsdt1_END             (1)
#define SOC_HSDT1_CRG_PEREN1_gt_pclk_autodiv_ufs_sys_subsys_hsdt1_START  (2)
#define SOC_HSDT1_CRG_PEREN1_gt_pclk_autodiv_ufs_sys_subsys_hsdt1_END    (2)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_qic_ufs_trace_START                  (3)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_qic_ufs_trace_END                    (3)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_qic_ufs_hci_START                    (4)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_qic_ufs_hci_END                      (4)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sdpll_sscg_START                     (20)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sdpll_sscg_END                       (20)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sd_START                             (21)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sd_END                               (21)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sdpll_tp_START                       (22)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sdpll_tp_END                         (22)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sd_32k_START                         (23)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_sd_32k_END                           (23)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERDIS1_UNION
 struct description   : PERDIS1 Register structure definition
                        Address Offset:0x014 Initial:0x00 Width:32
 register description : 外设时钟禁止寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_ufs_mphy_sys_hsdt1            : 1;  /* bit[0]    :  */
        unsigned int  gt_clk_ufs_sys_subsys_hsdt1          : 1;  /* bit[1]    :  */
        unsigned int  gt_pclk_autodiv_ufs_sys_subsys_hsdt1 : 1;  /* bit[2]    :  */
        unsigned int  gt_clk_qic_ufs_trace                 : 1;  /* bit[3]    :  */
        unsigned int  gt_clk_qic_ufs_hci                   : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                           : 1;  /* bit[5]    :  */
        unsigned int  reserved_1                           : 1;  /* bit[6]    :  */
        unsigned int  reserved_2                           : 1;  /* bit[7]    :  */
        unsigned int  reserved_3                           : 1;  /* bit[8]    :  */
        unsigned int  reserved_4                           : 1;  /* bit[9]    :  */
        unsigned int  reserved_5                           : 1;  /* bit[10]   :  */
        unsigned int  reserved_6                           : 1;  /* bit[11]   :  */
        unsigned int  reserved_7                           : 1;  /* bit[12]   :  */
        unsigned int  reserved_8                           : 1;  /* bit[13]   :  */
        unsigned int  reserved_9                           : 1;  /* bit[14]   :  */
        unsigned int  reserved_10                          : 1;  /* bit[15]   :  */
        unsigned int  reserved_11                          : 1;  /* bit[16]   :  */
        unsigned int  reserved_12                          : 1;  /* bit[17]   :  */
        unsigned int  reserved_13                          : 1;  /* bit[18]   :  */
        unsigned int  reserved_14                          : 1;  /* bit[19]   :  */
        unsigned int  gt_clk_sdpll_sscg                    : 1;  /* bit[20]   :  */
        unsigned int  gt_clk_sd                            : 1;  /* bit[21]   :  */
        unsigned int  gt_clk_sdpll_tp                      : 1;  /* bit[22]   :  */
        unsigned int  gt_clk_sd_32k                        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15                          : 1;  /* bit[24]   :  */
        unsigned int  reserved_16                          : 1;  /* bit[25]   :  */
        unsigned int  reserved_17                          : 6;  /* bit[26-31]: 外设时钟禁止控制：
                                                                                0：写0无效果；
                                                                                1：禁止IP时钟。 */
    } reg;
} SOC_HSDT1_CRG_PERDIS1_UNION;
#endif
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_ufs_mphy_sys_hsdt1_START             (0)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_ufs_mphy_sys_hsdt1_END               (0)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_ufs_sys_subsys_hsdt1_START           (1)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_ufs_sys_subsys_hsdt1_END             (1)
#define SOC_HSDT1_CRG_PERDIS1_gt_pclk_autodiv_ufs_sys_subsys_hsdt1_START  (2)
#define SOC_HSDT1_CRG_PERDIS1_gt_pclk_autodiv_ufs_sys_subsys_hsdt1_END    (2)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_qic_ufs_trace_START                  (3)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_qic_ufs_trace_END                    (3)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_qic_ufs_hci_START                    (4)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_qic_ufs_hci_END                      (4)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sdpll_sscg_START                     (20)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sdpll_sscg_END                       (20)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sd_START                             (21)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sd_END                               (21)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sdpll_tp_START                       (22)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sdpll_tp_END                         (22)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sd_32k_START                         (23)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_sd_32k_END                           (23)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERCLKEN1_UNION
 struct description   : PERCLKEN1 Register structure definition
                        Address Offset:0x018 Initial:0x00 Width:32
 register description : 外设时钟使能状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  gt_clk_ufs_mphy_sys_hsdt1            : 1;  /* bit[0]    :  */
        unsigned int  gt_clk_ufs_sys_subsys_hsdt1          : 1;  /* bit[1]    :  */
        unsigned int  gt_pclk_autodiv_ufs_sys_subsys_hsdt1 : 1;  /* bit[2]    :  */
        unsigned int  gt_clk_qic_ufs_trace                 : 1;  /* bit[3]    :  */
        unsigned int  gt_clk_qic_ufs_hci                   : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                           : 1;  /* bit[5]    :  */
        unsigned int  reserved_1                           : 1;  /* bit[6]    :  */
        unsigned int  reserved_2                           : 1;  /* bit[7]    :  */
        unsigned int  reserved_3                           : 1;  /* bit[8]    :  */
        unsigned int  reserved_4                           : 1;  /* bit[9]    :  */
        unsigned int  reserved_5                           : 1;  /* bit[10]   :  */
        unsigned int  reserved_6                           : 1;  /* bit[11]   :  */
        unsigned int  reserved_7                           : 1;  /* bit[12]   :  */
        unsigned int  reserved_8                           : 1;  /* bit[13]   :  */
        unsigned int  reserved_9                           : 1;  /* bit[14]   :  */
        unsigned int  reserved_10                          : 1;  /* bit[15]   :  */
        unsigned int  reserved_11                          : 1;  /* bit[16]   :  */
        unsigned int  reserved_12                          : 1;  /* bit[17]   :  */
        unsigned int  reserved_13                          : 1;  /* bit[18]   :  */
        unsigned int  reserved_14                          : 1;  /* bit[19]   :  */
        unsigned int  gt_clk_sdpll_sscg                    : 1;  /* bit[20]   :  */
        unsigned int  gt_clk_sd                            : 1;  /* bit[21]   :  */
        unsigned int  gt_clk_sdpll_tp                      : 1;  /* bit[22]   :  */
        unsigned int  gt_clk_sd_32k                        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15                          : 1;  /* bit[24]   :  */
        unsigned int  reserved_16                          : 1;  /* bit[25]   :  */
        unsigned int  reserved_17                          : 6;  /* bit[26-31]: 外设时钟使能状态：
                                                                                0：IP时钟使能撤销状态；
                                                                                1：IP时钟使能状态。 */
    } reg;
} SOC_HSDT1_CRG_PERCLKEN1_UNION;
#endif
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_ufs_mphy_sys_hsdt1_START             (0)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_ufs_mphy_sys_hsdt1_END               (0)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_ufs_sys_subsys_hsdt1_START           (1)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_ufs_sys_subsys_hsdt1_END             (1)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_pclk_autodiv_ufs_sys_subsys_hsdt1_START  (2)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_pclk_autodiv_ufs_sys_subsys_hsdt1_END    (2)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_qic_ufs_trace_START                  (3)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_qic_ufs_trace_END                    (3)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_qic_ufs_hci_START                    (4)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_qic_ufs_hci_END                      (4)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sdpll_sscg_START                     (20)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sdpll_sscg_END                       (20)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sd_START                             (21)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sd_END                               (21)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sdpll_tp_START                       (22)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sdpll_tp_END                         (22)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sd_32k_START                         (23)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_sd_32k_END                           (23)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERSTAT1_UNION
 struct description   : PERSTAT1 Register structure definition
                        Address Offset:0x01C Initial:0x00 Width:32
 register description : 外设时钟最终状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  st_clk_ufs_mphy_sys_hsdt1            : 1;  /* bit[0]    :  */
        unsigned int  st_clk_ufs_sys_subsys_hsdt1          : 1;  /* bit[1]    :  */
        unsigned int  st_pclk_autodiv_ufs_sys_subsys_hsdt1 : 1;  /* bit[2]    :  */
        unsigned int  st_clk_qic_ufs_trace                 : 1;  /* bit[3]    :  */
        unsigned int  st_clk_qic_ufs_hci                   : 1;  /* bit[4]    :  */
        unsigned int  reserved_0                           : 1;  /* bit[5]    :  */
        unsigned int  reserved_1                           : 1;  /* bit[6]    :  */
        unsigned int  reserved_2                           : 1;  /* bit[7]    :  */
        unsigned int  reserved_3                           : 1;  /* bit[8]    :  */
        unsigned int  reserved_4                           : 1;  /* bit[9]    :  */
        unsigned int  reserved_5                           : 1;  /* bit[10]   :  */
        unsigned int  reserved_6                           : 1;  /* bit[11]   :  */
        unsigned int  reserved_7                           : 1;  /* bit[12]   :  */
        unsigned int  reserved_8                           : 1;  /* bit[13]   :  */
        unsigned int  reserved_9                           : 1;  /* bit[14]   :  */
        unsigned int  reserved_10                          : 1;  /* bit[15]   :  */
        unsigned int  reserved_11                          : 1;  /* bit[16]   :  */
        unsigned int  reserved_12                          : 1;  /* bit[17]   :  */
        unsigned int  reserved_13                          : 1;  /* bit[18]   :  */
        unsigned int  reserved_14                          : 1;  /* bit[19]   :  */
        unsigned int  st_clk_sdpll_sscg                    : 1;  /* bit[20]   :  */
        unsigned int  st_clk_sd                            : 1;  /* bit[21]   :  */
        unsigned int  st_clk_sdpll_tp                      : 1;  /* bit[22]   :  */
        unsigned int  st_clk_sd_32k                        : 1;  /* bit[23]   :  */
        unsigned int  reserved_15                          : 1;  /* bit[24]   :  */
        unsigned int  reserved_16                          : 1;  /* bit[25]   :  */
        unsigned int  reserved_17                          : 6;  /* bit[26-31]: 外设时钟最终状态：
                                                                                0：IP时钟禁止状态；
                                                                                1：IP时钟使能状态。 */
    } reg;
} SOC_HSDT1_CRG_PERSTAT1_UNION;
#endif
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_ufs_mphy_sys_hsdt1_START             (0)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_ufs_mphy_sys_hsdt1_END               (0)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_ufs_sys_subsys_hsdt1_START           (1)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_ufs_sys_subsys_hsdt1_END             (1)
#define SOC_HSDT1_CRG_PERSTAT1_st_pclk_autodiv_ufs_sys_subsys_hsdt1_START  (2)
#define SOC_HSDT1_CRG_PERSTAT1_st_pclk_autodiv_ufs_sys_subsys_hsdt1_END    (2)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_qic_ufs_trace_START                  (3)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_qic_ufs_trace_END                    (3)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_qic_ufs_hci_START                    (4)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_qic_ufs_hci_END                      (4)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sdpll_sscg_START                     (20)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sdpll_sscg_END                       (20)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sd_START                             (21)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sd_END                               (21)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sdpll_tp_START                       (22)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sdpll_tp_END                         (22)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sd_32k_START                         (23)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_sd_32k_END                           (23)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERRSTEN0_UNION
 struct description   : PERRSTEN0 Register structure definition
                        Address Offset:0x020 Initial:0x0 Width:32
 register description : 外设软复位使能寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_hrst_sd                  : 1;  /* bit[0] : IP软复位使能：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位使能。 */
        unsigned int  ip_rst_sd                   : 1;  /* bit[1] :  */
        unsigned int  ip_prst_hsdt1_sysctrl       : 1;  /* bit[2] :  */
        unsigned int  ip_rst_dpctrl               : 1;  /* bit[3] :  */
        unsigned int  ip_prst_hsdt1_ioc           : 1;  /* bit[4] :  */
        unsigned int  ip_prst_usb31phy_apb        : 1;  /* bit[5] :  */
        unsigned int  reserved_0                  : 1;  /* bit[6] :  */
        unsigned int  ip_rst_usb2phy_por          : 1;  /* bit[7] :  */
        unsigned int  reserved_1                  : 1;  /* bit[8] :  */
        unsigned int  reserved_2                  : 1;  /* bit[9] :  */
        unsigned int  reserved_3                  : 1;  /* bit[10]:  */
        unsigned int  ip_rst_hi_usb3_sys          : 1;  /* bit[11]:  */
        unsigned int  reserved_4                  : 1;  /* bit[12]:  */
        unsigned int  ip_rst_qic_usb3             : 1;  /* bit[13]:  */
        unsigned int  ip_arst_hi_usb3             : 1;  /* bit[14]:  */
        unsigned int  ip_prst_usb2phy             : 1;  /* bit[15]:  */
        unsigned int  reserved_5                  : 1;  /* bit[16]:  */
        unsigned int  reserved_6                  : 1;  /* bit[17]:  */
        unsigned int  reserved_7                  : 1;  /* bit[18]:  */
        unsigned int  reserved_8                  : 1;  /* bit[19]:  */
        unsigned int  ip_rst_combophy             : 1;  /* bit[20]:  */
        unsigned int  reserved_9                  : 1;  /* bit[21]:  */
        unsigned int  ip_rst_combophy_por         : 1;  /* bit[22]:  */
        unsigned int  reserved_10                 : 1;  /* bit[23]:  */
        unsigned int  ip_rst_qic_ufs_trace        : 1;  /* bit[24]:  */
        unsigned int  ip_rst_qic_ufs_hci          : 1;  /* bit[25]:  */
        unsigned int  ip_rst_ufs_sys_subsys_hsdt1 : 1;  /* bit[26]:  */
        unsigned int  ip_rst_ufs_crg_hsdt1        : 1;  /* bit[27]:  */
        unsigned int  reserved_11                 : 1;  /* bit[28]:  */
        unsigned int  ip_prst_usb_sctrl           : 1;  /* bit[29]:  */
        unsigned int  reserved_12                 : 1;  /* bit[30]:  */
        unsigned int  reserved_13                 : 1;  /* bit[31]: IP软复位使能：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位使能。 */
    } reg;
} SOC_HSDT1_CRG_PERRSTEN0_UNION;
#endif
#define SOC_HSDT1_CRG_PERRSTEN0_ip_hrst_sd_START                   (0)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_hrst_sd_END                     (0)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_sd_START                    (1)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_sd_END                      (1)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_sysctrl_START        (2)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_sysctrl_END          (2)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_dpctrl_START                (3)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_dpctrl_END                  (3)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_ioc_START            (4)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_ioc_END              (4)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_usb31phy_apb_START         (5)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_usb31phy_apb_END           (5)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_usb2phy_por_START           (7)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_usb2phy_por_END             (7)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hi_usb3_sys_START           (11)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hi_usb3_sys_END             (11)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_qic_usb3_START              (13)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_qic_usb3_END                (13)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_arst_hi_usb3_START              (14)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_arst_hi_usb3_END                (14)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_usb2phy_START              (15)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_usb2phy_END                (15)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_combophy_START              (20)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_combophy_END                (20)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_combophy_por_START          (22)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_combophy_por_END            (22)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_qic_ufs_trace_START         (24)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_qic_ufs_trace_END           (24)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_qic_ufs_hci_START           (25)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_qic_ufs_hci_END             (25)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_ufs_sys_subsys_hsdt1_START  (26)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_ufs_sys_subsys_hsdt1_END    (26)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_ufs_crg_hsdt1_START         (27)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_ufs_crg_hsdt1_END           (27)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_usb_sctrl_START            (29)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_usb_sctrl_END              (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERRSTDIS0_UNION
 struct description   : PERRSTDIS0 Register structure definition
                        Address Offset:0x024 Initial:0x0 Width:32
 register description : 外设软复位撤离寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_hrst_sd                  : 1;  /* bit[0] : IP软复位撤离：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位撤离。 */
        unsigned int  ip_rst_sd                   : 1;  /* bit[1] :  */
        unsigned int  ip_prst_hsdt1_sysctrl       : 1;  /* bit[2] :  */
        unsigned int  ip_rst_dpctrl               : 1;  /* bit[3] :  */
        unsigned int  ip_prst_hsdt1_ioc           : 1;  /* bit[4] :  */
        unsigned int  ip_prst_usb31phy_apb        : 1;  /* bit[5] :  */
        unsigned int  reserved_0                  : 1;  /* bit[6] :  */
        unsigned int  ip_rst_usb2phy_por          : 1;  /* bit[7] :  */
        unsigned int  reserved_1                  : 1;  /* bit[8] :  */
        unsigned int  reserved_2                  : 1;  /* bit[9] :  */
        unsigned int  reserved_3                  : 1;  /* bit[10]:  */
        unsigned int  ip_rst_hi_usb3_sys          : 1;  /* bit[11]:  */
        unsigned int  reserved_4                  : 1;  /* bit[12]:  */
        unsigned int  ip_rst_qic_usb3             : 1;  /* bit[13]:  */
        unsigned int  ip_arst_hi_usb3             : 1;  /* bit[14]:  */
        unsigned int  ip_prst_usb2phy             : 1;  /* bit[15]:  */
        unsigned int  reserved_5                  : 1;  /* bit[16]:  */
        unsigned int  reserved_6                  : 1;  /* bit[17]:  */
        unsigned int  reserved_7                  : 1;  /* bit[18]:  */
        unsigned int  reserved_8                  : 1;  /* bit[19]:  */
        unsigned int  ip_rst_combophy             : 1;  /* bit[20]:  */
        unsigned int  reserved_9                  : 1;  /* bit[21]:  */
        unsigned int  ip_rst_combophy_por         : 1;  /* bit[22]:  */
        unsigned int  reserved_10                 : 1;  /* bit[23]:  */
        unsigned int  ip_rst_qic_ufs_trace        : 1;  /* bit[24]:  */
        unsigned int  ip_rst_qic_ufs_hci          : 1;  /* bit[25]:  */
        unsigned int  ip_rst_ufs_sys_subsys_hsdt1 : 1;  /* bit[26]:  */
        unsigned int  ip_rst_ufs_crg_hsdt1        : 1;  /* bit[27]:  */
        unsigned int  reserved_11                 : 1;  /* bit[28]:  */
        unsigned int  ip_prst_usb_sctrl           : 1;  /* bit[29]:  */
        unsigned int  reserved_12                 : 1;  /* bit[30]:  */
        unsigned int  reserved_13                 : 1;  /* bit[31]: IP软复位撤离：
                                                                    0：IP软复位使能状态不变；
                                                                    1：IP软复位撤离。 */
    } reg;
} SOC_HSDT1_CRG_PERRSTDIS0_UNION;
#endif
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_hrst_sd_START                   (0)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_hrst_sd_END                     (0)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_sd_START                    (1)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_sd_END                      (1)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_sysctrl_START        (2)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_sysctrl_END          (2)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_dpctrl_START                (3)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_dpctrl_END                  (3)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_ioc_START            (4)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_ioc_END              (4)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_usb31phy_apb_START         (5)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_usb31phy_apb_END           (5)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_usb2phy_por_START           (7)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_usb2phy_por_END             (7)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hi_usb3_sys_START           (11)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hi_usb3_sys_END             (11)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_qic_usb3_START              (13)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_qic_usb3_END                (13)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_arst_hi_usb3_START              (14)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_arst_hi_usb3_END                (14)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_usb2phy_START              (15)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_usb2phy_END                (15)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_combophy_START              (20)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_combophy_END                (20)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_combophy_por_START          (22)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_combophy_por_END            (22)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_qic_ufs_trace_START         (24)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_qic_ufs_trace_END           (24)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_qic_ufs_hci_START           (25)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_qic_ufs_hci_END             (25)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_ufs_sys_subsys_hsdt1_START  (26)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_ufs_sys_subsys_hsdt1_END    (26)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_ufs_crg_hsdt1_START         (27)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_ufs_crg_hsdt1_END           (27)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_usb_sctrl_START            (29)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_usb_sctrl_END              (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERRSTSTAT0_UNION
 struct description   : PERRSTSTAT0 Register structure definition
                        Address Offset:0x028 Initial:0x1 Width:32
 register description : 外设软复位状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ip_hrst_sd                  : 1;  /* bit[0] : 外设软复位使能状态：
                                                                    0：IP软复位使能撤销；
                                                                    1：IP软复位使能； */
        unsigned int  ip_rst_sd                   : 1;  /* bit[1] :  */
        unsigned int  ip_prst_hsdt1_sysctrl       : 1;  /* bit[2] :  */
        unsigned int  ip_rst_dpctrl               : 1;  /* bit[3] :  */
        unsigned int  ip_prst_hsdt1_ioc           : 1;  /* bit[4] :  */
        unsigned int  ip_prst_usb31phy_apb        : 1;  /* bit[5] :  */
        unsigned int  reserved_0                  : 1;  /* bit[6] :  */
        unsigned int  ip_rst_usb2phy_por          : 1;  /* bit[7] :  */
        unsigned int  reserved_1                  : 1;  /* bit[8] :  */
        unsigned int  reserved_2                  : 1;  /* bit[9] :  */
        unsigned int  reserved_3                  : 1;  /* bit[10]:  */
        unsigned int  ip_rst_hi_usb3_sys          : 1;  /* bit[11]:  */
        unsigned int  reserved_4                  : 1;  /* bit[12]:  */
        unsigned int  ip_rst_qic_usb3             : 1;  /* bit[13]:  */
        unsigned int  ip_arst_hi_usb3             : 1;  /* bit[14]:  */
        unsigned int  ip_prst_usb2phy             : 1;  /* bit[15]:  */
        unsigned int  reserved_5                  : 1;  /* bit[16]:  */
        unsigned int  reserved_6                  : 1;  /* bit[17]:  */
        unsigned int  reserved_7                  : 1;  /* bit[18]:  */
        unsigned int  reserved_8                  : 1;  /* bit[19]:  */
        unsigned int  ip_rst_combophy             : 1;  /* bit[20]:  */
        unsigned int  reserved_9                  : 1;  /* bit[21]:  */
        unsigned int  ip_rst_combophy_por         : 1;  /* bit[22]:  */
        unsigned int  reserved_10                 : 1;  /* bit[23]:  */
        unsigned int  ip_rst_qic_ufs_trace        : 1;  /* bit[24]:  */
        unsigned int  ip_rst_qic_ufs_hci          : 1;  /* bit[25]:  */
        unsigned int  ip_rst_ufs_sys_subsys_hsdt1 : 1;  /* bit[26]:  */
        unsigned int  ip_rst_ufs_crg_hsdt1        : 1;  /* bit[27]:  */
        unsigned int  reserved_11                 : 1;  /* bit[28]:  */
        unsigned int  ip_prst_usb_sctrl           : 1;  /* bit[29]:  */
        unsigned int  reserved_12                 : 1;  /* bit[30]:  */
        unsigned int  reserved_13                 : 1;  /* bit[31]: IP软复位使能状态：
                                                                    0：IP处于复位撤离状态；
                                                                    1：IP处于软复位使能状态。 */
    } reg;
} SOC_HSDT1_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_hrst_sd_START                   (0)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_hrst_sd_END                     (0)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_sd_START                    (1)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_sd_END                      (1)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_sysctrl_START        (2)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_sysctrl_END          (2)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_dpctrl_START                (3)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_dpctrl_END                  (3)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_ioc_START            (4)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_ioc_END              (4)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_usb31phy_apb_START         (5)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_usb31phy_apb_END           (5)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_usb2phy_por_START           (7)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_usb2phy_por_END             (7)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hi_usb3_sys_START           (11)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hi_usb3_sys_END             (11)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_qic_usb3_START              (13)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_qic_usb3_END                (13)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_arst_hi_usb3_START              (14)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_arst_hi_usb3_END                (14)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_usb2phy_START              (15)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_usb2phy_END                (15)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_combophy_START              (20)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_combophy_END                (20)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_combophy_por_START          (22)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_combophy_por_END            (22)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_qic_ufs_trace_START         (24)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_qic_ufs_trace_END           (24)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_qic_ufs_hci_START           (25)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_qic_ufs_hci_END             (25)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_ufs_sys_subsys_hsdt1_START  (26)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_ufs_sys_subsys_hsdt1_END    (26)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_ufs_crg_hsdt1_START         (27)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_ufs_crg_hsdt1_END           (27)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_usb_sctrl_START            (29)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_usb_sctrl_END              (29)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_CLKDIV0_UNION
 struct description   : CLKDIV0 Register structure definition
                        Address Offset:0x0A8 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0            : 1;  /* bit[0]    :  */
        unsigned int  sel_usb2phy_ref       : 1;  /* bit[1]    : clk_usb2phy_ref时钟源选择：
                                                                 1'b0:clkin_sys
                                                                 1'b1:clkin_sys_serdes */
        unsigned int  sc_gt_clk_usb2phy_ref : 1;  /* bit[2]    : 分频前门控控制：
                                                                 1：时钟开启； 0：时钟关闭 */
        unsigned int  reserved_1            : 1;  /* bit[3]    :  */
        unsigned int  reserved_2            : 1;  /* bit[4]    :  */
        unsigned int  reserved_3            : 4;  /* bit[5-8]  :  */
        unsigned int  reserved_4            : 2;  /* bit[9-10] :  */
        unsigned int  reserved_5            : 2;  /* bit[11-12]:  */
        unsigned int  reserved_6            : 3;  /* bit[13-15]:  */
        unsigned int  bitmasken             : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                 只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HSDT1_CRG_CLKDIV0_UNION;
#endif
#define SOC_HSDT1_CRG_CLKDIV0_sel_usb2phy_ref_START        (1)
#define SOC_HSDT1_CRG_CLKDIV0_sel_usb2phy_ref_END          (1)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_usb2phy_ref_START  (2)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_usb2phy_ref_END    (2)
#define SOC_HSDT1_CRG_CLKDIV0_bitmasken_START              (16)
#define SOC_HSDT1_CRG_CLKDIV0_bitmasken_END                (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_CLKDIV1_UNION
 struct description   : CLKDIV1 Register structure definition
                        Address Offset:0x0AC Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0               : 1;  /* bit[0]    :  */
        unsigned int  div_sd                   : 4;  /* bit[1-4]  : SD PLL时钟源分频比：
                                                                    4'h0:1分频；
                                                                    4'h1:2分频；
                                                                    …
                                                                    4'hf：16分频； */
        unsigned int  sel_sd_pll               : 2;  /* bit[5-6]  : clk_sd时钟源选择：
                                                                    2'b00:3.2M
                                                                    2'b01:PPLL2时钟源
                                                                    2'b1X:SDPLL时钟源 */
        unsigned int  reserved_1               : 2;  /* bit[7-8]  :  */
        unsigned int  reserved_2               : 1;  /* bit[9]    :  */
        unsigned int  div_ufs_sys_subsys_hsdt1 : 6;  /* bit[10-15]: UFS_SYS_SUBSYS_HSDT1分频比：
                                                                    6'h0:1分频；
                                                                    6'h1:2分频；
                                                                    …
                                                                    6'h3f：64分频； */
        unsigned int  bitmasken                : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                    只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HSDT1_CRG_CLKDIV1_UNION;
#endif
#define SOC_HSDT1_CRG_CLKDIV1_div_sd_START                    (1)
#define SOC_HSDT1_CRG_CLKDIV1_div_sd_END                      (4)
#define SOC_HSDT1_CRG_CLKDIV1_sel_sd_pll_START                (5)
#define SOC_HSDT1_CRG_CLKDIV1_sel_sd_pll_END                  (6)
#define SOC_HSDT1_CRG_CLKDIV1_div_ufs_sys_subsys_hsdt1_START  (10)
#define SOC_HSDT1_CRG_CLKDIV1_div_ufs_sys_subsys_hsdt1_END    (15)
#define SOC_HSDT1_CRG_CLKDIV1_bitmasken_START                 (16)
#define SOC_HSDT1_CRG_CLKDIV1_bitmasken_END                   (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_CLKDIV2_UNION
 struct description   : CLKDIV2 Register structure definition
                        Address Offset:0x0B0 Initial:0x0000 Width:32
 register description : 时钟分频比控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_gt_clk_sd_sys               : 1;  /* bit[0]    : 分频前门控控制：
                                                                          1：时钟开启； 0：时钟关闭 */
        unsigned int  sc_gt_clk_sd                   : 1;  /* bit[1]    :  */
        unsigned int  sc_gt_clk_suspend_div          : 1;  /* bit[2]    :  */
        unsigned int  sc_gt_clk_ufs_sys_subsys_hsdt1 : 1;  /* bit[3]    :  */
        unsigned int  reserved_0                     : 2;  /* bit[4-5]  :  */
        unsigned int  reserved_1                     : 4;  /* bit[6-9]  :  */
        unsigned int  reserved_2                     : 1;  /* bit[10]   :  */
        unsigned int  reserved_3                     : 3;  /* bit[11-13]:  */
        unsigned int  reserved_4                     : 1;  /* bit[14]   :  */
        unsigned int  reserved_5                     : 1;  /* bit[15]   :  */
        unsigned int  bitmasken                      : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                          只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HSDT1_CRG_CLKDIV2_UNION;
#endif
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_sd_sys_START                (0)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_sd_sys_END                  (0)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_sd_START                    (1)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_sd_END                      (1)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_suspend_div_START           (2)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_suspend_div_END             (2)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_ufs_sys_subsys_hsdt1_START  (3)
#define SOC_HSDT1_CRG_CLKDIV2_sc_gt_clk_ufs_sys_subsys_hsdt1_END    (3)
#define SOC_HSDT1_CRG_CLKDIV2_bitmasken_START                       (16)
#define SOC_HSDT1_CRG_CLKDIV2_bitmasken_END                         (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERI_STAT0_UNION
 struct description   : PERI_STAT0 Register structure definition
                        Address Offset:0x0C8 Initial:0x0000000 Width:32
 register description : 外设状态寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 2;  /* bit[0-1] :  */
        unsigned int  reserved_1: 2;  /* bit[2-3] :  */
        unsigned int  reserved_2: 2;  /* bit[4-5] :  */
        unsigned int  reserved_3: 26; /* bit[6-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_PERI_STAT0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERI_STAT1_UNION
 struct description   : PERI_STAT1 Register structure definition
                        Address Offset:0x0CC Initial:0x00000000 Width:32
 register description : 外设状态寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  swdone_clk_sd_div                   : 1;  /* bit[0]   : DIV分频比切换是否完成指示信号：
                                                                              0：未完成；
                                                                              1：完成； */
        unsigned int  sdpll_sscg_idle                     : 1;  /* bit[1]   : SDPLL SSCG模块IDLE指示信号
                                                                              1：IDLE
                                                                              0：BUSY */
        unsigned int  swdone_clk_ufs_sys_subsys_hsdt1_div : 1;  /* bit[2]   : DIV分频比切换是否完成指示信号：
                                                                              0：未完成；
                                                                              1：完成； */
        unsigned int  reserved                            : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_PERI_STAT1_UNION;
#endif
#define SOC_HSDT1_CRG_PERI_STAT1_swdone_clk_sd_div_START                    (0)
#define SOC_HSDT1_CRG_PERI_STAT1_swdone_clk_sd_div_END                      (0)
#define SOC_HSDT1_CRG_PERI_STAT1_sdpll_sscg_idle_START                      (1)
#define SOC_HSDT1_CRG_PERI_STAT1_sdpll_sscg_idle_END                        (1)
#define SOC_HSDT1_CRG_PERI_STAT1_swdone_clk_ufs_sys_subsys_hsdt1_div_START  (2)
#define SOC_HSDT1_CRG_PERI_STAT1_swdone_clk_ufs_sys_subsys_hsdt1_div_END    (2)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_IPCLKRST_BYPASS0_UNION
 struct description   : IPCLKRST_BYPASS0 Register structure definition
                        Address Offset:0x100 Initial:0x0 Width:32
 register description : 外设IP防挂死功能byapss控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sd_clkrst_bypass                 : 1;  /* bit[0] : 防挂死功能byapss控制：
                                                                         0：bypass无效；
                                                                         1：bypass有效。 */
        unsigned int  dpctrl_clkrst_bypass             : 1;  /* bit[1] :  */
        unsigned int  hsdt1_sysctrl_clkrst_bypass      : 1;  /* bit[2] :  */
        unsigned int  hsdt1_ioc_clkrst_bypass          : 1;  /* bit[3] :  */
        unsigned int  usbctrl_clkrst_bypass            : 1;  /* bit[4] :  */
        unsigned int  ufs_apb_clkrst_bypass            : 1;  /* bit[5] :  */
        unsigned int  reserved_0                       : 1;  /* bit[6] :  */
        unsigned int  reserved_1                       : 1;  /* bit[7] :  */
        unsigned int  reserved_2                       : 1;  /* bit[8] :  */
        unsigned int  reserved_3                       : 1;  /* bit[9] :  */
        unsigned int  reserved_4                       : 1;  /* bit[10]:  */
        unsigned int  reserved_5                       : 1;  /* bit[11]:  */
        unsigned int  reserved_6                       : 1;  /* bit[12]:  */
        unsigned int  reserved_7                       : 1;  /* bit[13]:  */
        unsigned int  hsdt1_usb31phy_apb_clkrst_bypass : 1;  /* bit[14]:  */
        unsigned int  reserved_8                       : 1;  /* bit[15]:  */
        unsigned int  reserved_9                       : 1;  /* bit[16]:  */
        unsigned int  reserved_10                      : 1;  /* bit[17]:  */
        unsigned int  reserved_11                      : 1;  /* bit[18]:  */
        unsigned int  usb_sctrl_clkrst_bypass          : 1;  /* bit[19]:  */
        unsigned int  reserved_12                      : 1;  /* bit[20]:  */
        unsigned int  reserved_13                      : 1;  /* bit[21]:  */
        unsigned int  reserved_14                      : 1;  /* bit[22]:  */
        unsigned int  usb2phy_clkrst_bypass            : 1;  /* bit[23]:  */
        unsigned int  reserved_15                      : 1;  /* bit[24]:  */
        unsigned int  reserved_16                      : 1;  /* bit[25]:  */
        unsigned int  reserved_17                      : 1;  /* bit[26]:  */
        unsigned int  reserved_18                      : 1;  /* bit[27]:  */
        unsigned int  reserved_19                      : 1;  /* bit[28]:  */
        unsigned int  reserved_20                      : 1;  /* bit[29]:  */
        unsigned int  reserved_21                      : 1;  /* bit[30]:  */
        unsigned int  reserved_22                      : 1;  /* bit[31]:  */
    } reg;
} SOC_HSDT1_CRG_IPCLKRST_BYPASS0_UNION;
#endif
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_sd_clkrst_bypass_START                  (0)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_sd_clkrst_bypass_END                    (0)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_dpctrl_clkrst_bypass_START              (1)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_dpctrl_clkrst_bypass_END                (1)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_sysctrl_clkrst_bypass_START       (2)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_sysctrl_clkrst_bypass_END         (2)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_ioc_clkrst_bypass_START           (3)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_ioc_clkrst_bypass_END             (3)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usbctrl_clkrst_bypass_START             (4)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usbctrl_clkrst_bypass_END               (4)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_ufs_apb_clkrst_bypass_START             (5)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_ufs_apb_clkrst_bypass_END               (5)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_usb31phy_apb_clkrst_bypass_START  (14)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_usb31phy_apb_clkrst_bypass_END    (14)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usb_sctrl_clkrst_bypass_START           (19)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usb_sctrl_clkrst_bypass_END             (19)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usb2phy_clkrst_bypass_START             (23)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usb2phy_clkrst_bypass_END               (23)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_SDPLLSSCCTRL_UNION
 struct description   : SDPLLSSCCTRL Register structure definition
                        Address Offset:0x190 Initial:0x0000 Width:32
 register description : SDPLL SSC控制寄存器
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
} SOC_HSDT1_CRG_SDPLLSSCCTRL_UNION;
#endif
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_enable_START    (0)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_enable_END      (0)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_mode_START      (1)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_mode_END        (1)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_rate_START      (2)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_rate_END        (5)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_depth_START     (6)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_depth_END       (8)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_fast_dis_START  (9)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_sscg_fast_dis_END    (9)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_bitmasken_START      (16)
#define SOC_HSDT1_CRG_SDPLLSSCCTRL_bitmasken_END        (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_SDPLLCTRL0_UNION
 struct description   : SDPLLCTRL0 Register structure definition
                        Address Offset:0x200 Initial:0x00 Width:32
 register description : SDPLL控制寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sdpll_en       : 1;  /* bit[0-0]  : SDPLL PowerDown控制。
                                                          0：关闭；
                                                          1：使能。 */
        unsigned int  sdpll_bp       : 1;  /* bit[1-1]  : SDPLL Bypass控制。
                                                          0：正常工作；
                                                          1：Bypass。 */
        unsigned int  sdpll_refdiv   : 6;  /* bit[2-7]  : SDPLL输入时钟分频器。 */
        unsigned int  sdpll_fbdiv    : 12; /* bit[8-19] : SDPLL反馈时钟整数分频器。 */
        unsigned int  sdpll_postdiv1 : 3;  /* bit[20-22]: SDPLL输出时钟分频器1。 */
        unsigned int  sdpll_postdiv2 : 3;  /* bit[23-25]: SDPLL输出时钟分频器2。 */
        unsigned int  sdpll_lock     : 1;  /* bit[26-26]: SDPLL Lock指示。
                                                          0：无效；
                                                          1：SDPLL Lock。 */
        unsigned int  reserved       : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_SDPLLCTRL0_UNION;
#endif
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_en_START        (0)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_en_END          (0)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_bp_START        (1)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_bp_END          (1)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_refdiv_START    (2)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_refdiv_END      (7)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_fbdiv_START     (8)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_fbdiv_END       (19)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_postdiv1_START  (20)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_postdiv1_END    (22)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_postdiv2_START  (23)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_postdiv2_END    (25)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_lock_START      (26)
#define SOC_HSDT1_CRG_SDPLLCTRL0_sdpll_lock_END        (26)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_SDPLLCTRL1_UNION
 struct description   : SDPLLCTRL1 Register structure definition
                        Address Offset:0x204 Initial:0x00 Width:32
 register description : SDPLL控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sdpll_fracdiv : 24; /* bit[0-23] : SDPLL反馈时钟分数分频器。 */
        unsigned int  sdpll_int_mod : 1;  /* bit[24-24]: SDPLL整数分频模式。
                                                         0：分数分频模式；
                                                         1：整数分频模式。
                                                         注意：本PLL支持分数分频和整数分频模式，但在SSC打开时，要求软件只使用分数分频模式。 */
        unsigned int  sdpll_cfg_vld : 1;  /* bit[25-25]: SDPLL配置有效标志。
                                                         0：配置无效；
                                                         1：配置有效。 */
        unsigned int  gt_clk_sdpll  : 1;  /* bit[26-26]: SDPLL门控信号。
                                                         0：SDPLL输出时钟门控；
                                                         1：SDPLL输出时钟不门控。（等待PLL稳定后才能配置该bit） */
        unsigned int  reserved      : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_SDPLLCTRL1_UNION;
#endif
#define SOC_HSDT1_CRG_SDPLLCTRL1_sdpll_fracdiv_START  (0)
#define SOC_HSDT1_CRG_SDPLLCTRL1_sdpll_fracdiv_END    (23)
#define SOC_HSDT1_CRG_SDPLLCTRL1_sdpll_int_mod_START  (24)
#define SOC_HSDT1_CRG_SDPLLCTRL1_sdpll_int_mod_END    (24)
#define SOC_HSDT1_CRG_SDPLLCTRL1_sdpll_cfg_vld_START  (25)
#define SOC_HSDT1_CRG_SDPLLCTRL1_sdpll_cfg_vld_END    (25)
#define SOC_HSDT1_CRG_SDPLLCTRL1_gt_clk_sdpll_START   (26)
#define SOC_HSDT1_CRG_SDPLLCTRL1_gt_clk_sdpll_END     (26)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PCIEPLL_STAT_UNION
 struct description   : PCIEPLL_STAT Register structure definition
                        Address Offset:0x208 Initial:0x000000 Width:32
 register description : PCIE SCPLL系统状态寄存器0寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0        : 1;  /* bit[0]   :  */
        unsigned int  reserved_1        : 1;  /* bit[1]   :  */
        unsigned int  reserved_2        : 1;  /* bit[2]   :  */
        unsigned int  reserved_3        : 1;  /* bit[3]   :  */
        unsigned int  sdpll_lock        : 1;  /* bit[4]   : sdpll lock状态：
                                                            0：未锁定；
                                                            1：锁定 */
        unsigned int  sdpll_en_stat     : 1;  /* bit[5]   : sdpll使能状态：
                                                            0：未使能；
                                                            1：使能 */
        unsigned int  sdpll_bypass_stat : 1;  /* bit[6]   : sdpll bypass能状态：
                                                            0：非bypass；
                                                            1：bypass */
        unsigned int  reserved_4        : 1;  /* bit[7]   : 保留。 */
        unsigned int  reserved_5        : 24; /* bit[8-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_PCIEPLL_STAT_UNION;
#endif
#define SOC_HSDT1_CRG_PCIEPLL_STAT_sdpll_lock_START         (4)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_sdpll_lock_END           (4)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_sdpll_en_stat_START      (5)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_sdpll_en_stat_END        (5)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_sdpll_bypass_stat_START  (6)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_sdpll_bypass_stat_END    (6)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_FNPLL_CFG0_UNION
 struct description   : FNPLL_CFG0 Register structure definition
                        Address Offset:0x224 Initial:0x3C831022 Width:32
 register description : FNPLL系统控制寄存器0寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  FNPLL_CFG0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_HSDT1_CRG_FNPLL_CFG0_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG0_FNPLL_CFG0_START  (0)
#define SOC_HSDT1_CRG_FNPLL_CFG0_FNPLL_CFG0_END    (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_FNPLL_CFG1_UNION
 struct description   : FNPLL_CFG1 Register structure definition
                        Address Offset:0x228 Initial:0x0000005F Width:32
 register description : FNPLL系统控制寄存器1寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  FNPLL_CFG1 : 32; /* bit[0-31]:  */
    } reg;
} SOC_HSDT1_CRG_FNPLL_CFG1_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG1_FNPLL_CFG1_START  (0)
#define SOC_HSDT1_CRG_FNPLL_CFG1_FNPLL_CFG1_END    (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_FNPLL_CFG2_UNION
 struct description   : FNPLL_CFG2 Register structure definition
                        Address Offset:0x22C Initial:0x800180A6 Width:32
 register description : FNPLL系统控制寄存器2寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  FNPLL_CFG2 : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_FNPLL_CFG2_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG2_FNPLL_CFG2_START  (0)
#define SOC_HSDT1_CRG_FNPLL_CFG2_FNPLL_CFG2_END    (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_FNPLL_CFG3_UNION
 struct description   : FNPLL_CFG3 Register structure definition
                        Address Offset:0x230 Initial:0x0028627D Width:32
 register description : FNPLL系统控制寄存器3寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  FNPLL_CFG3 : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_FNPLL_CFG3_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG3_FNPLL_CFG3_START  (0)
#define SOC_HSDT1_CRG_FNPLL_CFG3_FNPLL_CFG3_END    (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERI_AUTODIV0_UNION
 struct description   : PERI_AUTODIV0 Register structure definition
                        Address Offset:0x350 Initial:0x0 Width:32
 register description : 外设自动降频控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sysbus_tb_qsp_h12sys_bypass    : 1;  /* bit[0] : 是否加入自动降频判断条件：
                                                                       1'h0:加入；
                                                                       1'h1:不加入； */
        unsigned int  sysbus_tb_h1bus_qcp_bypass     : 1;  /* bit[1] :  */
        unsigned int  sysbus_tb_h1_apb_bypass        : 1;  /* bit[2] :  */
        unsigned int  sysbus_tb_h1_ahb_bypass        : 1;  /* bit[3] :  */
        unsigned int  sysbus_rd_ib_usb3_bypass       : 1;  /* bit[4] :  */
        unsigned int  sysbus_wr_ib_usb3_bypass       : 1;  /* bit[5] :  */
        unsigned int  sysbus_rd_ib_ufs_trace_bypass  : 1;  /* bit[6] :  */
        unsigned int  sysbus_wr_ib_ufs_trace_bypass  : 1;  /* bit[7] :  */
        unsigned int  sysbus_rd_ib_ufs_hci_bypass    : 1;  /* bit[8] :  */
        unsigned int  sysbus_wr_ib_ufs_hci_bypass    : 1;  /* bit[9] :  */
        unsigned int  sysbus_rd_ib_sd3_bypass        : 1;  /* bit[10]:  */
        unsigned int  sysbus_wr_ib_sd3_bypass        : 1;  /* bit[11]:  */
        unsigned int  sysbus_rd_ib_qsp_sys2h1_bypass : 1;  /* bit[12]:  */
        unsigned int  sysbus_wr_ib_qsp_sys2h1_bypass : 1;  /* bit[13]:  */
        unsigned int  sysbus_ufshc_idle_bypass       : 1;  /* bit[14]:  */
        unsigned int  reserved_0                     : 1;  /* bit[15]:  */
        unsigned int  reserved_1                     : 1;  /* bit[16]:  */
        unsigned int  reserved_2                     : 1;  /* bit[17]:  */
        unsigned int  reserved_3                     : 1;  /* bit[18]:  */
        unsigned int  reserved_4                     : 1;  /* bit[19]:  */
        unsigned int  reserved_5                     : 1;  /* bit[20]:  */
        unsigned int  reserved_6                     : 1;  /* bit[21]:  */
        unsigned int  reserved_7                     : 1;  /* bit[22]:  */
        unsigned int  reserved_8                     : 1;  /* bit[23]:  */
        unsigned int  reserved_9                     : 1;  /* bit[24]:  */
        unsigned int  reserved_10                    : 1;  /* bit[25]:  */
        unsigned int  reserved_11                    : 1;  /* bit[26]:  */
        unsigned int  reserved_12                    : 1;  /* bit[27]:  */
        unsigned int  reserved_13                    : 1;  /* bit[28]:  */
        unsigned int  reserved_14                    : 1;  /* bit[29]:  */
        unsigned int  reserved_15                    : 1;  /* bit[30]:  */
        unsigned int  reserved_16                    : 1;  /* bit[31]: 是否加入自动降频判断条件：
                                                                       1'h0:加入；
                                                                       1'h1:不加入； */
    } reg;
} SOC_HSDT1_CRG_PERI_AUTODIV0_UNION;
#endif
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_qsp_h12sys_bypass_START     (0)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_qsp_h12sys_bypass_END       (0)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_h1bus_qcp_bypass_START      (1)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_h1bus_qcp_bypass_END        (1)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_h1_apb_bypass_START         (2)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_h1_apb_bypass_END           (2)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_h1_ahb_bypass_START         (3)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_tb_h1_ahb_bypass_END           (3)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_usb3_bypass_START        (4)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_usb3_bypass_END          (4)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_usb3_bypass_START        (5)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_usb3_bypass_END          (5)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_ufs_trace_bypass_START   (6)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_ufs_trace_bypass_END     (6)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_ufs_trace_bypass_START   (7)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_ufs_trace_bypass_END     (7)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_ufs_hci_bypass_START     (8)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_ufs_hci_bypass_END       (8)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_ufs_hci_bypass_START     (9)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_ufs_hci_bypass_END       (9)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_sd3_bypass_START         (10)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_sd3_bypass_END           (10)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_sd3_bypass_START         (11)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_sd3_bypass_END           (11)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_qsp_sys2h1_bypass_START  (12)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_rd_ib_qsp_sys2h1_bypass_END    (12)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_qsp_sys2h1_bypass_START  (13)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_wr_ib_qsp_sys2h1_bypass_END    (13)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_ufshc_idle_bypass_START        (14)
#define SOC_HSDT1_CRG_PERI_AUTODIV0_sysbus_ufshc_idle_bypass_END          (14)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERI_AUTODIV1_UNION
 struct description   : PERI_AUTODIV1 Register structure definition
                        Address Offset:0x354 Initial:0x0 Width:32
 register description : 外设自动降频控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ufs_sys_subsys_hsdt1_idle_bypass      : 1;  /* bit[0]    : 硬件自动降频功能bypass配置：
                                                                                 0：不bypass；
                                                                                 1：bypass。 */
        unsigned int  ufs_sys_subsys_hsdt1_idle_waitcfg_in  : 10; /* bit[1-10] : 硬件进入自动降频IDLE等待时钟周期配置 */
        unsigned int  ufs_sys_subsys_hsdt1_idle_waitcfg_out : 10; /* bit[11-20]: 硬件退出自动降频IDLE等待时钟周期配置 */
        unsigned int  ufs_sys_subsys_hsdt1_div_auto_cfg     : 6;  /* bit[21-26]: 硬件自动降频倍数配置
                                                                                 6'h00:1；
                                                                                 6'h01:2；
                                                                                 …
                                                                                 6'h3F:64； */
        unsigned int  reserved_0                            : 1;  /* bit[27]   :  */
        unsigned int  reserved_1                            : 1;  /* bit[28]   :  */
        unsigned int  reserved_2                            : 1;  /* bit[29]   :  */
        unsigned int  reserved_3                            : 1;  /* bit[30]   :  */
        unsigned int  reserved_4                            : 1;  /* bit[31]   :  */
    } reg;
} SOC_HSDT1_CRG_PERI_AUTODIV1_UNION;
#endif
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_idle_bypass_START       (0)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_idle_bypass_END         (0)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_idle_waitcfg_in_START   (1)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_idle_waitcfg_in_END     (10)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_idle_waitcfg_out_START  (11)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_idle_waitcfg_out_END    (20)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_div_auto_cfg_START      (21)
#define SOC_HSDT1_CRG_PERI_AUTODIV1_ufs_sys_subsys_hsdt1_div_auto_cfg_END        (26)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_PERI_AUTODIV2_UNION
 struct description   : PERI_AUTODIV2 Register structure definition
                        Address Offset:0x358 Initial:0x0 Width:32
 register description : 外设自动降频控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ufs_ufs4_perf_bypass       : 1;  /* bit[0] : 是否加入自动降频判断条件：
                                                                   1'h0:加入；
                                                                   1'h1:不加入； */
        unsigned int  ufs_ufshc_idle_bypass      : 1;  /* bit[1] :  */
        unsigned int  ufs_wr_ib_ufs_trace_bypass : 1;  /* bit[2] :  */
        unsigned int  ufs_rd_ib_ufs_trace_bypass : 1;  /* bit[3] :  */
        unsigned int  ufs_wr_ib_ufs_hci_bypass   : 1;  /* bit[4] :  */
        unsigned int  ufs_rd_ib_ufs_hci_bypass   : 1;  /* bit[5] :  */
        unsigned int  reserved_0                 : 1;  /* bit[6] :  */
        unsigned int  reserved_1                 : 1;  /* bit[7] :  */
        unsigned int  reserved_2                 : 1;  /* bit[8] :  */
        unsigned int  reserved_3                 : 1;  /* bit[9] :  */
        unsigned int  reserved_4                 : 1;  /* bit[10]:  */
        unsigned int  reserved_5                 : 1;  /* bit[11]:  */
        unsigned int  reserved_6                 : 1;  /* bit[12]:  */
        unsigned int  reserved_7                 : 1;  /* bit[13]:  */
        unsigned int  reserved_8                 : 1;  /* bit[14]:  */
        unsigned int  reserved_9                 : 1;  /* bit[15]:  */
        unsigned int  reserved_10                : 1;  /* bit[16]:  */
        unsigned int  reserved_11                : 1;  /* bit[17]:  */
        unsigned int  reserved_12                : 1;  /* bit[18]:  */
        unsigned int  reserved_13                : 1;  /* bit[19]:  */
        unsigned int  reserved_14                : 1;  /* bit[20]:  */
        unsigned int  reserved_15                : 1;  /* bit[21]:  */
        unsigned int  reserved_16                : 1;  /* bit[22]:  */
        unsigned int  reserved_17                : 1;  /* bit[23]:  */
        unsigned int  reserved_18                : 1;  /* bit[24]:  */
        unsigned int  reserved_19                : 1;  /* bit[25]:  */
        unsigned int  reserved_20                : 1;  /* bit[26]:  */
        unsigned int  reserved_21                : 1;  /* bit[27]:  */
        unsigned int  reserved_22                : 1;  /* bit[28]:  */
        unsigned int  reserved_23                : 1;  /* bit[29]:  */
        unsigned int  reserved_24                : 1;  /* bit[30]:  */
        unsigned int  reserved_25                : 1;  /* bit[31]: 是否加入自动降频判断条件：
                                                                   1'h0:加入；
                                                                   1'h1:不加入； */
    } reg;
} SOC_HSDT1_CRG_PERI_AUTODIV2_UNION;
#endif
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_ufs4_perf_bypass_START        (0)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_ufs4_perf_bypass_END          (0)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_ufshc_idle_bypass_START       (1)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_ufshc_idle_bypass_END         (1)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_wr_ib_ufs_trace_bypass_START  (2)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_wr_ib_ufs_trace_bypass_END    (2)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_rd_ib_ufs_trace_bypass_START  (3)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_rd_ib_ufs_trace_bypass_END    (3)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_wr_ib_ufs_hci_bypass_START    (4)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_wr_ib_ufs_hci_bypass_END      (4)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_rd_ib_ufs_hci_bypass_START    (5)
#define SOC_HSDT1_CRG_PERI_AUTODIV2_ufs_rd_ib_ufs_hci_bypass_END      (5)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_ISOEN_UNION
 struct description   : ISOEN Register structure definition
                        Address Offset:0x280 Initial:0x0 Width:32
 register description : 掉电子系统ISO使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  usbpcie_refclk_iso_en : 1;  /* bit[0] :  */
        unsigned int  reserved_0            : 1;  /* bit[1] :  */
        unsigned int  reserved_1            : 1;  /* bit[2] :  */
        unsigned int  reserved_2            : 1;  /* bit[3] :  */
        unsigned int  reserved_3            : 1;  /* bit[4] :  */
        unsigned int  reserved_4            : 1;  /* bit[5] :  */
        unsigned int  reserved_5            : 1;  /* bit[6] :  */
        unsigned int  reserved_6            : 1;  /* bit[7] :  */
        unsigned int  reserved_7            : 1;  /* bit[8] :  */
        unsigned int  reserved_8            : 1;  /* bit[9] :  */
        unsigned int  reserved_9            : 1;  /* bit[10]:  */
        unsigned int  reserved_10           : 1;  /* bit[11]:  */
        unsigned int  reserved_11           : 1;  /* bit[12]:  */
        unsigned int  reserved_12           : 1;  /* bit[13]:  */
        unsigned int  reserved_13           : 1;  /* bit[14]:  */
        unsigned int  reserved_14           : 1;  /* bit[15]:  */
        unsigned int  reserved_15           : 1;  /* bit[16]:  */
        unsigned int  reserved_16           : 1;  /* bit[17]:  */
        unsigned int  reserved_17           : 1;  /* bit[18]:  */
        unsigned int  reserved_18           : 1;  /* bit[19]:  */
        unsigned int  reserved_19           : 1;  /* bit[20]:  */
        unsigned int  reserved_20           : 1;  /* bit[21]:  */
        unsigned int  reserved_21           : 1;  /* bit[22]:  */
        unsigned int  reserved_22           : 1;  /* bit[23]:  */
        unsigned int  reserved_23           : 1;  /* bit[24]:  */
        unsigned int  reserved_24           : 1;  /* bit[25]:  */
        unsigned int  reserved_25           : 1;  /* bit[26]:  */
        unsigned int  reserved_26           : 1;  /* bit[27]:  */
        unsigned int  reserved_27           : 1;  /* bit[28]:  */
        unsigned int  reserved_28           : 1;  /* bit[29]:  */
        unsigned int  reserved_29           : 1;  /* bit[30]:  */
        unsigned int  reserved_30           : 1;  /* bit[31]:  */
    } reg;
} SOC_HSDT1_CRG_ISOEN_UNION;
#endif
#define SOC_HSDT1_CRG_ISOEN_usbpcie_refclk_iso_en_START  (0)
#define SOC_HSDT1_CRG_ISOEN_usbpcie_refclk_iso_en_END    (0)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_ISODIS_UNION
 struct description   : ISODIS Register structure definition
                        Address Offset:0x284 Initial:0x0 Width:32
 register description : 掉电子系统ISO使能撤销寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  usbpcie_refclk_iso_en : 1;  /* bit[0] :  */
        unsigned int  reserved_0            : 1;  /* bit[1] :  */
        unsigned int  reserved_1            : 1;  /* bit[2] :  */
        unsigned int  reserved_2            : 1;  /* bit[3] :  */
        unsigned int  reserved_3            : 1;  /* bit[4] :  */
        unsigned int  reserved_4            : 1;  /* bit[5] :  */
        unsigned int  reserved_5            : 1;  /* bit[6] :  */
        unsigned int  reserved_6            : 1;  /* bit[7] :  */
        unsigned int  reserved_7            : 1;  /* bit[8] :  */
        unsigned int  reserved_8            : 1;  /* bit[9] :  */
        unsigned int  reserved_9            : 1;  /* bit[10]:  */
        unsigned int  reserved_10           : 1;  /* bit[11]:  */
        unsigned int  reserved_11           : 1;  /* bit[12]:  */
        unsigned int  reserved_12           : 1;  /* bit[13]:  */
        unsigned int  reserved_13           : 1;  /* bit[14]:  */
        unsigned int  reserved_14           : 1;  /* bit[15]:  */
        unsigned int  reserved_15           : 1;  /* bit[16]:  */
        unsigned int  reserved_16           : 1;  /* bit[17]:  */
        unsigned int  reserved_17           : 1;  /* bit[18]:  */
        unsigned int  reserved_18           : 1;  /* bit[19]:  */
        unsigned int  reserved_19           : 1;  /* bit[20]:  */
        unsigned int  reserved_20           : 1;  /* bit[21]:  */
        unsigned int  reserved_21           : 1;  /* bit[22]:  */
        unsigned int  reserved_22           : 1;  /* bit[23]:  */
        unsigned int  reserved_23           : 1;  /* bit[24]:  */
        unsigned int  reserved_24           : 1;  /* bit[25]:  */
        unsigned int  reserved_25           : 1;  /* bit[26]:  */
        unsigned int  reserved_26           : 1;  /* bit[27]:  */
        unsigned int  reserved_27           : 1;  /* bit[28]:  */
        unsigned int  reserved_28           : 1;  /* bit[29]:  */
        unsigned int  reserved_29           : 1;  /* bit[30]:  */
        unsigned int  reserved_30           : 1;  /* bit[31]:  */
    } reg;
} SOC_HSDT1_CRG_ISODIS_UNION;
#endif
#define SOC_HSDT1_CRG_ISODIS_usbpcie_refclk_iso_en_START  (0)
#define SOC_HSDT1_CRG_ISODIS_usbpcie_refclk_iso_en_END    (0)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_ISOSTAT_UNION
 struct description   : ISOSTAT Register structure definition
                        Address Offset:0x288 Initial:0x1 Width:32
 register description : 掉电子系统ISO使能状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  usbpcie_refclk_iso_en : 1;  /* bit[0] : ISO单元使能状态：0：不使能；1：使能。 */
        unsigned int  reserved_0            : 1;  /* bit[1] :  */
        unsigned int  reserved_1            : 1;  /* bit[2] :  */
        unsigned int  reserved_2            : 1;  /* bit[3] :  */
        unsigned int  reserved_3            : 1;  /* bit[4] :  */
        unsigned int  reserved_4            : 1;  /* bit[5] :  */
        unsigned int  reserved_5            : 1;  /* bit[6] :  */
        unsigned int  reserved_6            : 1;  /* bit[7] :  */
        unsigned int  reserved_7            : 1;  /* bit[8] :  */
        unsigned int  reserved_8            : 1;  /* bit[9] :  */
        unsigned int  reserved_9            : 1;  /* bit[10]:  */
        unsigned int  reserved_10           : 1;  /* bit[11]:  */
        unsigned int  reserved_11           : 1;  /* bit[12]:  */
        unsigned int  reserved_12           : 1;  /* bit[13]:  */
        unsigned int  reserved_13           : 1;  /* bit[14]:  */
        unsigned int  reserved_14           : 1;  /* bit[15]:  */
        unsigned int  reserved_15           : 1;  /* bit[16]:  */
        unsigned int  reserved_16           : 1;  /* bit[17]:  */
        unsigned int  reserved_17           : 1;  /* bit[18]:  */
        unsigned int  reserved_18           : 1;  /* bit[19]:  */
        unsigned int  reserved_19           : 1;  /* bit[20]:  */
        unsigned int  reserved_20           : 1;  /* bit[21]:  */
        unsigned int  reserved_21           : 1;  /* bit[22]:  */
        unsigned int  reserved_22           : 1;  /* bit[23]:  */
        unsigned int  reserved_23           : 1;  /* bit[24]:  */
        unsigned int  reserved_24           : 1;  /* bit[25]:  */
        unsigned int  reserved_25           : 1;  /* bit[26]:  */
        unsigned int  reserved_26           : 1;  /* bit[27]:  */
        unsigned int  reserved_27           : 1;  /* bit[28]:  */
        unsigned int  reserved_28           : 1;  /* bit[29]:  */
        unsigned int  reserved_29           : 1;  /* bit[30]:  */
        unsigned int  reserved_30           : 1;  /* bit[31]:  */
    } reg;
} SOC_HSDT1_CRG_ISOSTAT_UNION;
#endif
#define SOC_HSDT1_CRG_ISOSTAT_usbpcie_refclk_iso_en_START  (0)
#define SOC_HSDT1_CRG_ISOSTAT_usbpcie_refclk_iso_en_END    (0)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_UNION
 struct description   : AUTODIV_INUSE_STAT Register structure definition
                        Address Offset:0x400 Initial:0x000 Width:32
 register description : 自动降频状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  autodiv_ufs_sys_subsys_hsdt1_stat : 1;  /* bit[0]    : 自动降频状态回读，默认为0
                                                                             0:处于不降频；
                                                                             1：处于降频态。 */
        unsigned int  reserved_0                        : 1;  /* bit[1]    :  */
        unsigned int  reserved_1                        : 1;  /* bit[2]    :  */
        unsigned int  reserved_2                        : 1;  /* bit[3]    :  */
        unsigned int  reserved_3                        : 1;  /* bit[4]    :  */
        unsigned int  reserved_4                        : 1;  /* bit[5]    :  */
        unsigned int  reserved_5                        : 1;  /* bit[6]    :  */
        unsigned int  reserved_6                        : 1;  /* bit[7]    :  */
        unsigned int  reserved_7                        : 1;  /* bit[8]    :  */
        unsigned int  reserved_8                        : 1;  /* bit[9]    :  */
        unsigned int  reserved_9                        : 1;  /* bit[10]   :  */
        unsigned int  reserved_10                       : 1;  /* bit[11]   :  */
        unsigned int  reserved_11                       : 1;  /* bit[12]   :  */
        unsigned int  reserved_12                       : 1;  /* bit[13]   :  */
        unsigned int  reserved_13                       : 1;  /* bit[14]   :  */
        unsigned int  reserved_14                       : 1;  /* bit[15]   :  */
        unsigned int  reserved_15                       : 1;  /* bit[16]   :  */
        unsigned int  reserved_16                       : 1;  /* bit[17]   :  */
        unsigned int  reserved_17                       : 1;  /* bit[18]   :  */
        unsigned int  reserved_18                       : 1;  /* bit[19]   :  */
        unsigned int  reserved_19                       : 1;  /* bit[20]   :  */
        unsigned int  reserved_20                       : 1;  /* bit[21]   :  */
        unsigned int  reserved_21                       : 10; /* bit[22-31]: 保留 */
    } reg;
} SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_UNION;
#endif
#define SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_autodiv_ufs_sys_subsys_hsdt1_stat_START  (0)
#define SOC_HSDT1_CRG_AUTODIV_INUSE_STAT_autodiv_ufs_sys_subsys_hsdt1_stat_END    (0)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_FNPLL_STAT0_SD_UNION
 struct description   : FNPLL_STAT0_SD Register structure definition
                        Address Offset:0x408 Initial:0x00000000 Width:32
 register description : FNPLL系统状态寄存器0(SD)
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  FNPLL_STATE0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_HSDT1_CRG_FNPLL_STAT0_SD_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_STAT0_SD_FNPLL_STATE0_START  (0)
#define SOC_HSDT1_CRG_FNPLL_STAT0_SD_FNPLL_STATE0_END    (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION
 struct description   : INTR_MASK_NOCBUS_NONIDLE_PEND Register structure definition
                        Address Offset:0x500 Initial:0x0000 Width:32
 register description : NOC总线异常中断屏蔽寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_mask_usb_nonidle_pend       : 1;  /* bit[0]    : 中断屏蔽：
                                                                            0：屏蔽
                                                                            1：不屏蔽 */
        unsigned int  intr_mask_ufs_trace_nonidle_pend : 1;  /* bit[1]    : 中断屏蔽：
                                                                            0：屏蔽
                                                                            1：不屏蔽 */
        unsigned int  intr_mask_ufs_hci_nonidle_pend   : 1;  /* bit[2]    : 中断屏蔽：
                                                                            0：屏蔽
                                                                            1：不屏蔽 */
        unsigned int  reserved                         : 13; /* bit[3-15] : 保留。 */
        unsigned int  bitmasken                        : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                            只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_usb_nonidle_pend_START        (0)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_usb_nonidle_pend_END          (0)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ufs_trace_nonidle_pend_START  (1)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ufs_trace_nonidle_pend_END    (1)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ufs_hci_nonidle_pend_START    (2)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ufs_hci_nonidle_pend_END      (2)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_START                         (16)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_END                           (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION
 struct description   : INTR_CLR_NOCBUS_NONIDLE_PEND Register structure definition
                        Address Offset:0x504 Initial:0x0000 Width:32
 register description : NOC总线异常中断清除寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_clr_usb_nonidle_pend       : 1;  /* bit[0]    : 中断清除：
                                                                           0：不清除
                                                                           1：清除
                                                                           注：该中断清除信号高电平有效，清除信号为高时，一直处于清中断状态，清除中断后需要将该清除信号配置为0 */
        unsigned int  intr_clr_ufs_trace_nonidle_pend : 1;  /* bit[1]    : 中断清除：
                                                                           0：不清除
                                                                           1：清除
                                                                           注：该中断清除信号高电平有效，清除信号为高时，一直处于清中断状态，清除中断后需要将该清除信号配置为0 */
        unsigned int  intr_clr_ufs_hci_nonidle_pend   : 1;  /* bit[2]    : 中断清除：
                                                                           0：不清除
                                                                           1：清除
                                                                           注：该中断清除信号高电平有效，清除信号为高时，一直处于清中断状态，清除中断后需要将该清除信号配置为0 */
        unsigned int  reserved                        : 13; /* bit[3-15] : 保留。 */
        unsigned int  bitmasken                       : 16; /* bit[16-31]: clkdiv每个比特位的使能位：
                                                                           只有当bitmasken对应的比特位为1'b1，clkdiv相应的比特位才起作用。bitmasken[0]就是clkdiv[0]的mask使能位。写1有效。 */
    } reg;
} SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_usb_nonidle_pend_START        (0)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_usb_nonidle_pend_END          (0)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ufs_trace_nonidle_pend_START  (1)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ufs_trace_nonidle_pend_END    (1)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ufs_hci_nonidle_pend_START    (2)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ufs_hci_nonidle_pend_END      (2)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_START                        (16)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_END                          (31)


/*****************************************************************************
 struct               : SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION
 struct description   : INTR_STAT_NOCBUS_NONIDLE_PEND Register structure definition
                        Address Offset:0x508 Initial:0x00000000 Width:32
 register description : NOC总线异常中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_stat_usb_nonidle_pend       : 1;  /* bit[0]   : 中断状态：
                                                                           0：中断无效
                                                                           1：中断有效 */
        unsigned int  intr_stat_ufs_trace_nonidle_pend : 1;  /* bit[1]   : 中断状态：
                                                                           0：中断无效
                                                                           1：中断有效 */
        unsigned int  intr_stat_ufs_hci_nonidle_pend   : 1;  /* bit[2]   : 中断状态：
                                                                           0：中断无效
                                                                           1：中断有效 */
        unsigned int  reserved                         : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_usb_nonidle_pend_START        (0)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_usb_nonidle_pend_END          (0)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ufs_trace_nonidle_pend_START  (1)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ufs_trace_nonidle_pend_END    (1)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ufs_hci_nonidle_pend_START    (2)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ufs_hci_nonidle_pend_END      (2)






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

#endif /* end of soc_hsdt1_crg_interface.h */
