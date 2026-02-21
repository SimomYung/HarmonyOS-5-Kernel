/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_usb_misc_ctrl_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_USB_MISC_CTRL_INTERFACE_H__
#define __SOC_USB_MISC_CTRL_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) register_define
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_ADDR(base)   ((base) + (0x00UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_ADDR(base)   ((base) + (0x04UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ADDR(base)   ((base) + (0x08UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_ADDR(base)   ((base) + (0x0cUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_ADDR(base)   ((base) + (0x10UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_ADDR(base)   ((base) + (0x14UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ADDR(base)   ((base) + (0x18UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_ADDR(base)   ((base) + (0x1cUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_ADDR(base)   ((base) + (0x20UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_ADDR(base)   ((base) + (0x24UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_ADDR(base)  ((base) + (0x70UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_ADDR(base)     ((base) + (0x2cUL))

/* 寄存器说明：选择控制寄存器1
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_CTRL1_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_ADDR(base)            ((base) + (0x30UL))

/* 寄存器说明：状态寄存器0
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS0_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_ADDR(base)             ((base) + (0x34UL))

/* 寄存器说明：状态寄存器1
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS1_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_ADDR(base)             ((base) + (0x38UL))

/* 寄存器说明：状态寄存器2
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS2_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_ADDR(base)             ((base) + (0x3cUL))

/* 寄存器说明：状态寄存器3
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS3_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_ADDR(base)             ((base) + (0x40UL))

/* 寄存器说明：BC控制器寄存器0
   位域定义UNION结构:  SOC_USB_MISC_CTRL_BC_CTRL0_UNION */
#define SOC_USB_MISC_CTRL_BC_CTRL0_ADDR(base)                 ((base) + (0x44UL))

/* 寄存器说明：BC控制器寄存器1
   位域定义UNION结构:  SOC_USB_MISC_CTRL_BC_CTRL1_UNION */
#define SOC_USB_MISC_CTRL_BC_CTRL1_ADDR(base)                 ((base) + (0x48UL))

/* 寄存器说明：RAM控制寄存器
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG4c_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_ADDR(base)           ((base) + (0x4cUL))

/* 寄存器说明：状态寄存器4
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS4_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_ADDR(base)             ((base) + (0x50UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG54_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_ADDR(base)           ((base) + (0x54UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG58_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_ADDR(base)           ((base) + (0x58UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG5C_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_ADDR(base)           ((base) + (0x5cUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG60_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_ADDR(base)           ((base) + (0x60UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG64_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_ADDR(base)           ((base) + (0x64UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG68_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_ADDR(base)           ((base) + (0x68UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG6C_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_ADDR(base)           ((base) + (0x6cUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFGA0_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_ADDR(base)           ((base) + (0xA0UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ADDR(base)  ((base) + (0x0074UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_ADDR(base)  ((base) + (0x0078UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ADDR(base)  ((base) + (0x007cUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ADDR(base)  ((base) + (0x0080UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ADDR(base)  ((base) + (0x0084UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ADDR(base)  ((base) + (0x0088UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_usb_pipe_rst_n_UNION */
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_ADDR(base)           ((base) + (0x008cUL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_dwc_usb_ctrl0_UNION */
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ADDR(base)            ((base) + (0x0090UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_dwc_usb_ctrl1_UNION */
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_ADDR(base)            ((base) + (0x0094UL))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_dwc_usb_ctrl2_UNION */
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_ADDR(base)            ((base) + (0x0098UL))


#else


/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_ADDR(base)   ((base) + (0x00))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_ADDR(base)   ((base) + (0x04))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ADDR(base)   ((base) + (0x08))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_ADDR(base)   ((base) + (0x0c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_ADDR(base)   ((base) + (0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_ADDR(base)   ((base) + (0x14))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ADDR(base)   ((base) + (0x18))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_ADDR(base)   ((base) + (0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_ADDR(base)   ((base) + (0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_ADDR(base)   ((base) + (0x24))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_ADDR(base)  ((base) + (0x70))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_ADDR(base)     ((base) + (0x2c))

/* 寄存器说明：选择控制寄存器1
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_CTRL1_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_ADDR(base)            ((base) + (0x30))

/* 寄存器说明：状态寄存器0
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS0_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_ADDR(base)             ((base) + (0x34))

/* 寄存器说明：状态寄存器1
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS1_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_ADDR(base)             ((base) + (0x38))

/* 寄存器说明：状态寄存器2
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS2_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_ADDR(base)             ((base) + (0x3c))

/* 寄存器说明：状态寄存器3
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS3_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_ADDR(base)             ((base) + (0x40))

/* 寄存器说明：BC控制器寄存器0
   位域定义UNION结构:  SOC_USB_MISC_CTRL_BC_CTRL0_UNION */
#define SOC_USB_MISC_CTRL_BC_CTRL0_ADDR(base)                 ((base) + (0x44))

/* 寄存器说明：BC控制器寄存器1
   位域定义UNION结构:  SOC_USB_MISC_CTRL_BC_CTRL1_UNION */
#define SOC_USB_MISC_CTRL_BC_CTRL1_ADDR(base)                 ((base) + (0x48))

/* 寄存器说明：RAM控制寄存器
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG4c_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_ADDR(base)           ((base) + (0x4c))

/* 寄存器说明：状态寄存器4
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USBOTG3_STS4_UNION */
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_ADDR(base)             ((base) + (0x50))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG54_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_ADDR(base)           ((base) + (0x54))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG58_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_ADDR(base)           ((base) + (0x58))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG5C_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_ADDR(base)           ((base) + (0x5c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG60_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_ADDR(base)           ((base) + (0x60))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG64_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_ADDR(base)           ((base) + (0x64))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG68_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_ADDR(base)           ((base) + (0x68))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFG6C_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_ADDR(base)           ((base) + (0x6c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_USB_MISC_CFGA0_UNION */
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_ADDR(base)           ((base) + (0xA0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ADDR(base)  ((base) + (0x0074))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_ADDR(base)  ((base) + (0x0078))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ADDR(base)  ((base) + (0x007c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ADDR(base)  ((base) + (0x0080))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ADDR(base)  ((base) + (0x0084))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_UNION */
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ADDR(base)  ((base) + (0x0088))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_usb_pipe_rst_n_UNION */
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_ADDR(base)           ((base) + (0x008c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_dwc_usb_ctrl0_UNION */
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ADDR(base)            ((base) + (0x0090))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_dwc_usb_ctrl1_UNION */
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_ADDR(base)            ((base) + (0x0094))

/* 寄存器说明：
   位域定义UNION结构:  SOC_USB_MISC_CTRL_dwc_usb_ctrl2_UNION */
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_ADDR(base)            ((base) + (0x0098))


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
                     (1/1) register_define
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_UNION
 结构说明  : sc_usb20phy_misc_ctrl0 寄存器结构定义。地址偏移量:0x00，初值:0x00000180，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_cfg_cr_clk_sel          : 1;  /* bit[0-0]  : eUSB register configrate clock Select:
                                                                       1 : apb_clk
                                                                       0 ：internal pclk
                                                                       don't care,暂时不使用 */
        unsigned int  phy_cfg_por_in_lx           : 1;  /* bit[1-1]  : after power-on reset : eUSB will enter:
                                                                       1：low power mode
                                                                       0：normal power on mode
                                                                       don't care,暂时不使用 */
        unsigned int  phy_cfg_rcal_bypass         : 1;  /* bit[2-2]  : eUSB transmitter calibration Bypass：
                                                                       1： calibration bypassed
                                                                       0： calibration enable
                                                                       don't care,暂时不使用 */
        unsigned int  phy_cfg_rcal_code           : 4;  /* bit[3-6]  : eUSB termination control
                                                                       don't care,暂时不使用 */
        unsigned int  phy_cfg_rptr_mode           : 1;  /* bit[7-7]  : eUSB mode Selection：
                                                                       1：repeator mode
                                                                       0： native mode
                                                                       don't care,暂时不使用 */
        unsigned int  phy_cfg_rx_hs_term_en       : 1;  /* bit[8-8]  : eUSB HS RX termination enbale
                                                                       1: enable in HS mode
                                                                       0：disbale in HS Mode
                                                                       don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_fsls_vreg_bypass : 1;  /* bit[9-9]  : don't care,暂时不使用 */
        unsigned int  reserved                    : 22; /* bit[10-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_cr_clk_sel_START           (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_cr_clk_sel_END             (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_por_in_lx_START            (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_por_in_lx_END              (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_bypass_START          (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_bypass_END            (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_code_START            (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_code_END              (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rptr_mode_START            (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rptr_mode_END              (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rx_hs_term_en_START        (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rx_hs_term_en_END          (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_tx_fsls_vreg_bypass_START  (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_tx_fsls_vreg_bypass_END    (9)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_UNION
 结构说明  : sc_usb20phy_misc_ctrl1 寄存器结构定义。地址偏移量:0x04，初值:0x0100C800，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_cfg_pll_cpbias_cntrl : 7;  /* bit[0-6]  : PHY PLL Configuration， don't change
                                                                    don't care,暂时不使用 */
        unsigned int  phy_cfg_pll_fb_div       : 12; /* bit[7-18] : PHY PLL Configuration， don't change
                                                                    don't care,暂时不使用 */
        unsigned int  phy_cfg_pll_gmp_cntrl    : 2;  /* bit[19-20]: PHY PLL Configuration， don't change
                                                                    don't care,暂时不使用 */
        unsigned int  phy_cfg_pll_int_cntrl    : 6;  /* bit[21-26]: PHY PLL Configuration， don't change
                                                                    don't care,暂时不使用 */
        unsigned int  reserved                 : 5;  /* bit[27-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_cpbias_cntrl_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_cpbias_cntrl_END    (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_fb_div_START        (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_fb_div_END          (18)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_gmp_cntrl_START     (19)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_gmp_cntrl_END       (20)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_int_cntrl_START     (21)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_int_cntrl_END       (26)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_UNION
 结构说明  : sc_usb20phy_misc_ctrl2 寄存器结构定义。地址偏移量:0x08，初值:0x0000200A，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_cfg_pll_prop_cntrl : 6;  /* bit[0-5]  : PHY PLL Configuration， don't change
                                                                  don't care,暂时不使用 */
        unsigned int  phy_cfg_pll_ref_div    : 4;  /* bit[6-9]  : PHY PLL Configuration， don't change
                                                                  don't care,暂时不使用 */
        unsigned int  phy_cfg_pll_vco_cntrl  : 3;  /* bit[10-12]: PHY PLL Configuration， don't change
                                                                  don't care,暂时不使用 */
        unsigned int  phy_cfg_pll_vref_tune  : 2;  /* bit[13-14]: PHY PLL Configuration， don't change
                                                                  don't care,暂时不使用 */
        unsigned int  ref_freq_sel           : 3;  /* bit[15-17]: PHY PLL Configuration， don't change
                                                                  don't care,暂时不使用 */
        unsigned int  reserved               : 14; /* bit[18-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_prop_cntrl_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_prop_cntrl_END    (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_ref_div_START     (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_ref_div_END       (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vco_cntrl_START   (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vco_cntrl_END     (12)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vref_tune_START   (13)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vref_tune_END     (14)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ref_freq_sel_START            (15)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ref_freq_sel_END              (17)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_UNION
 结构说明  : sc_usb20phy_misc_ctrl3 寄存器结构定义。地址偏移量:0x0c，初值:0x0000000D，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_cfg_rx_eq_ctle : 2;  /* bit[0-1] : HS RX CTLE BOOST配置
                                                             11:4.62dB
                                                             10:3.17dB
                                                             01:0dB
                                                             00：Reserved
                                                             don't care,暂时不使用 */
        unsigned int  phy_cfg_rx_hs_tune : 3;  /* bit[2-4] : squelch Threshold 校准
                                                             111： -32.84%
                                                             110： -24.31%
                                                             101: -16.07%
                                                             100： -8.21%
                                                             011： default
                                                             010： 8.34%
                                                             001： 17.44%
                                                             000： 25.25%
                                                             don't care,暂时不使用 */
        unsigned int  reserved           : 27; /* bit[5-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_eq_ctle_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_eq_ctle_END    (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_hs_tune_START  (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_hs_tune_END    (4)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_UNION
 结构说明  : sc_usb20phy_misc_ctrl4 寄存器结构定义。地址偏移量:0x10，初值:0x0000481C，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_cfg_tx_fsls_slew_rate_tune : 1;  /* bit[0-0]  : FS/LS slew rate control
                                                                          0:7.86ns
                                                                          1:8.70ns, default
                                                                          don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_fsls_vref_tune      : 2;  /* bit[1-2]  : default 2'b0
                                                                          don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_hs_vref_tune        : 3;  /* bit[3-5]  : HS DC Voltage Level 校准
                                                                          000：-17.99%
                                                                          001：-12.06%
                                                                          010：-6.08%
                                                                          011： default
                                                                          100: 6.21%
                                                                          101： 12.34%
                                                                          110： 18.48%
                                                                          111： 24.63%
                                                                          don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_hs_xv_tune          : 2;  /* bit[6-7]  : Transmitter HS Crossover 校准
                                                                          00 ： 0， default
                                                                          01 ： Reserved
                                                                          10 ： 26.2mV
                                                                          01 ： -23.38%
                                                                          don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_preemp_tune         : 3;  /* bit[8-10] : HS Transmitter Pre-Emphasis Control：
                                                                          000 ： default pre-emphasis 关闭
                                                                          001 ： pre-emphasis 1.6dB
                                                                          010 ： pre-emphasis 2.5dB
                                                                          011 ： pre-emphasis 3.1dB
                                                                          100 ： pre-emphasis 3.6dB
                                                                          Others： Reserved
                                                                          don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_res_tune            : 2;  /* bit[11-12]: USB Source Impedance Adjustment：
                                                                          11 ： -2.95 Ohm
                                                                          10 : -2.95 Ohm
                                                                          01 : default
                                                                          00 : 1.67 Ohm
                                                                          don't care,暂时不使用 */
        unsigned int  phy_cfg_tx_rise_tune           : 2;  /* bit[13-14]: HS Transmitter Rise/Fall Time Adjustment:
                                                                          11 : -3.23%
                                                                          10 : default 
                                                                          01 : 7.54 %
                                                                          00 : 14.64%
                                                                          don't care,暂时不使用 */
        unsigned int  reserved                       : 17; /* bit[15-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_slew_rate_tune_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_slew_rate_tune_END    (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_vref_tune_START       (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_vref_tune_END         (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_vref_tune_START         (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_vref_tune_END           (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_xv_tune_START           (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_xv_tune_END             (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_preemp_tune_START          (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_preemp_tune_END            (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_res_tune_START             (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_res_tune_END               (12)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_rise_tune_START            (13)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_rise_tune_END              (14)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_UNION
 结构说明  : sc_usb20phy_misc_ctrl5 寄存器结构定义。地址偏移量:0x14，初值:0x00000040，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  res_req_in        : 1;  /* bit[0-0] : don't care,暂时不使用 */
        unsigned int  res_ack_in        : 1;  /* bit[1-1] : don't care,暂时不使用 */
        unsigned int  test_burin        : 1;  /* bit[2-2] : test-burin mode, don't change
                                                            don't care,暂时不使用 */
        unsigned int  test_iddq         : 1;  /* bit[3-3] : IDDQ mode enable
                                                            don't care,暂时不使用 */
        unsigned int  test_loopback_en  : 1;  /* bit[4-4] : loopback enable
                                                            don't care,暂时不使用 */
        unsigned int  test_stop_clk_en  : 1;  /* bit[5-5] : default 0, don't change
                                                            don't care,暂时不使用 */
        unsigned int  retenable_n       : 1;  /* bit[6-6] : don't change
                                                            don't care,暂时不使用 */
        unsigned int  utmi_clk_force_en : 1;  /* bit[7-7] : utmi free clock force enable
                                                            don't care,暂时不使用 */
        unsigned int  utmi_txbitstuffen : 1;  /* bit[8-8] : utmi bit stuff enable
                                                            don't care,暂时不使用 */
        unsigned int  reserved          : 23; /* bit[9-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_req_in_START         (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_req_in_END           (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_ack_in_START         (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_ack_in_END           (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_burin_START         (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_burin_END           (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_iddq_START          (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_iddq_END            (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_loopback_en_START   (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_loopback_en_END     (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_stop_clk_en_START   (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_stop_clk_en_END     (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_retenable_n_START        (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_retenable_n_END          (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_clk_force_en_START  (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_clk_force_en_END    (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_txbitstuffen_START  (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_txbitstuffen_END    (8)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_UNION
 结构说明  : sc_usb20phy_misc_ctrl6 寄存器结构定义。地址偏移量:0x18，初值:0x00000001，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_reset      : 1;  /* bit[0-0] : 高有效,don't care,暂时不使用 */
        unsigned int  repeator_reset : 1;  /* bit[1-1] : don't care,暂时不使用 */
        unsigned int  ulpi_reset     : 1;  /* bit[2-2] : 低有效 */
        unsigned int  reserved       : 29; /* bit[3-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_phy_reset_START       (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_phy_reset_END         (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_repeator_reset_START  (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_repeator_reset_END    (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ulpi_reset_START      (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ulpi_reset_END        (2)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_UNION
 结构说明  : sc_usb20phy_misc_ctrl7 寄存器结构定义。地址偏移量:0x1c，初值:0x000001C0，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  utmi_vbus_valid_ext  : 1;  /* bit[0-0]  : (vbusvalid)bvalid信号，默认为0， 工作时候必须配置为1 */
        unsigned int  hsdt1_utmi_word_if   : 1;  /* bit[1-1]  : don't care,暂时不使用 */
        unsigned int  audio_utmi_word_if   : 1;  /* bit[2-2]  : don't care,暂时不使用 */
        unsigned int  asp_use_phy          : 1;  /* bit[3-3]  : don't care,暂时不使用 */
        unsigned int  dwc_bvalid           : 1;  /* bit[4-4]  : 控制器bvalid信号，默认为0， 工作时候必须配置为1，且必须在utmi_vbus_valid_ext配置为1之后配置 */
        unsigned int  dwc_utmi_vbusvld_sel : 1;  /* bit[5-5]  : 选择输入给control的vbusvald是 eusb的还是 寄存器控制的 */
        unsigned int  combophy_ready       : 1;  /* bit[6-6]  : 默认打开，保证onchip阶段不影响。 */
        unsigned int  combophy_ready_sel   : 1;  /* bit[7-7]  : 默认打开，保证onchip阶段不影响。 */
        unsigned int  power_prsent_sel     : 2;  /* bit[8-9]  : 控制器pipe power prsent 选择：
                                                                00： power_prsent = 0
                                                                01： power_prsent = 1
                                                                10： usb2 vbusvalid 
                                                                11: usb2 vbusvalid 与combophy_ready */
        unsigned int  reserved             : 22; /* bit[10-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_utmi_vbus_valid_ext_START   (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_utmi_vbus_valid_ext_END     (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_hsdt1_utmi_word_if_START    (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_hsdt1_utmi_word_if_END      (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_audio_utmi_word_if_START    (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_audio_utmi_word_if_END      (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_asp_use_phy_START           (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_asp_use_phy_END             (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_bvalid_START            (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_bvalid_END              (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_utmi_vbusvld_sel_START  (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_utmi_vbusvld_sel_END    (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_START        (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_END          (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_sel_START    (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_sel_END      (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_power_prsent_sel_START      (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_power_prsent_sel_END        (9)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_UNION
 结构说明  : sc_usb20phy_misc_ctrl8 寄存器结构定义。地址偏移量:0x20，初值:0x00000003，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cfg_utmi_dppulldown : 1;  /* bit[0-0] : only audio usb , host mode use
                                                              don't care,暂时不使用 */
        unsigned int  cfg_utmi_dmpulldown : 1;  /* bit[1-1] : only audio usb , host mode use
                                                              don't care,暂时不使用 */
        unsigned int  reserved            : 30; /* bit[2-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dppulldown_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dppulldown_END    (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dmpulldown_START  (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dmpulldown_END    (1)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_UNION
 结构说明  : sc_usb20phy_misc_ctrl9 寄存器结构定义。地址偏移量:0x24，初值:0x00000002，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bvalid_cfg               : 1;  /* bit[0-0]  : ULPI interface 强配bvalid */
        unsigned int  sessend_cfg              : 1;  /* bit[1-1]  : ULPI interface 强配Sessend */
        unsigned int  vbusvalid_cfg            : 1;  /* bit[2-2]  : ULPI interface 强配vbusvalid */
        unsigned int  reserved_0               : 7;  /* bit[3-9]  :  */
        unsigned int  logic_debug_trace_en     : 1;  /* bit[10-10]: logic trace 开始使能 */
        unsigned int  trace_sample_clear_start : 1;  /* bit[11-11]: 每次trace都必须要将该信号拉高 */
        unsigned int  reserved_1               : 2;  /* bit[12-13]:  */
        unsigned int  sample_ctrl              : 4;  /* bit[14-17]: control trace start and data select
                                                                    don't care,暂时不使用 */
        unsigned int  reserved_2               : 14; /* bit[18-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_bvalid_cfg_START                (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_bvalid_cfg_END                  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sessend_cfg_START               (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sessend_cfg_END                 (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_vbusvalid_cfg_START             (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_vbusvalid_cfg_END               (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_logic_debug_trace_en_START      (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_logic_debug_trace_en_END        (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_trace_sample_clear_start_START  (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_trace_sample_clear_start_END    (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sample_ctrl_START               (14)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sample_ctrl_END                 (17)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_UNION
 结构说明  : sc_usb20phy_misc_ctrl10 寄存器结构定义。地址偏移量:0x70，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cfg_scan_apb_in   : 1;  /* bit[0-0]  : don't care,暂时不使用 */
        unsigned int  cfg_scan_mode     : 1;  /* bit[1-1]  : don't care,暂时不使用 */
        unsigned int  cfg_scan_ocla_clk : 1;  /* bit[2-2]  : don't care,暂时不使用 */
        unsigned int  cfg_scan_ocla_in  : 1;  /* bit[3-3]  : don't care,暂时不使用 */
        unsigned int  cfg_scan_pclk_clk : 1;  /* bit[4-4]  : don't care,暂时不使用 */
        unsigned int  cfg_scan_pclk_in  : 17; /* bit[5-21] : don't care,暂时不使用 */
        unsigned int  cfg_scan_pll_clk  : 1;  /* bit[22-22]: don't care,暂时不使用 */
        unsigned int  cfg_scan_pll_in   : 3;  /* bit[23-25]: don't care,暂时不使用 */
        unsigned int  cfg_scan_ref_clk  : 1;  /* bit[26-26]: don't care,暂时不使用 */
        unsigned int  cfg_scan_ref_in   : 1;  /* bit[27-27]: don't care,暂时不使用 */
        unsigned int  cfg_scan_sclk_clk : 1;  /* bit[28-28]: don't care,暂时不使用 */
        unsigned int  cfg_scan_set_rst  : 1;  /* bit[29-29]: don't care,暂时不使用 */
        unsigned int  cfg_scan_shift    : 1;  /* bit[30-30]: don't care,暂时不使用 */
        unsigned int  cfg_scan_shift_cg : 1;  /* bit[31-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_apb_in_START    (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_apb_in_END      (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_mode_START      (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_mode_END        (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_clk_START  (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_clk_END    (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_in_START   (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_in_END     (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_clk_START  (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_clk_END    (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_in_START   (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_in_END     (21)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_clk_START   (22)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_clk_END     (22)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_in_START    (23)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_in_END      (25)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_clk_START   (26)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_clk_END     (26)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_in_START    (27)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_in_END      (27)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_sclk_clk_START  (28)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_sclk_clk_END    (28)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_set_rst_START   (29)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_set_rst_END     (29)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_START     (30)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_END       (30)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_cg_START  (31)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_cg_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_UNION
 结构说明  : sc_usb20phy_misc_st0 寄存器结构定义。地址偏移量:0x2c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  phy_dtb_out       : 2;  /* bit[0-1]  : Digital test bus outut
                                                             don't care,暂时不使用 */
        unsigned int  test_utmi_rx_data : 8;  /* bit[2-9]  : don't care,暂时不使用 */
        unsigned int  reserved          : 22; /* bit[10-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_phy_dtb_out_START        (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_phy_dtb_out_END          (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_test_utmi_rx_data_START  (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_test_utmi_rx_data_END    (9)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USBOTG3_CTRL1_UNION
 结构说明  : USBOTG3_CTRL1 寄存器结构定义。地址偏移量:0x30，初值:0x00000080，宽度:32
 寄存器说明: 选择控制寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0           : 2;  /* bit[0-1] :  */
        unsigned int  hub_port_overcurrent : 2;  /* bit[2-3] : This is the per port Overcurrent indication of the root-hub ports:
                                                               ■ 0 - No Overcurrent
                                                               ■ 1 - Overcurrent  */
        unsigned int  reserved_1           : 3;  /* bit[4-6] :  */
        unsigned int  xhc_bme              : 1;  /* bit[7]   : This signal is used to disable the bus mastering 
                                                               capability of the xHC. */
        unsigned int  reserved_2           : 24; /* bit[8-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_CTRL1_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_hub_port_overcurrent_START  (2)
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_hub_port_overcurrent_END    (3)
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_xhc_bme_START               (7)
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_xhc_bme_END                 (7)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USBOTG3_STS0_UNION
 结构说明  : USBOTG3_STS0 寄存器结构定义。地址偏移量:0x34，初值:0x00000040，宽度:32
 寄存器说明: 状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  debug : 32; /* bit[0-31]: debug信号低32位，寄存器默认读出值不用care */
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS0_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_debug_START  (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_debug_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USBOTG3_STS1_UNION
 结构说明  : USBOTG3_STS1 寄存器结构定义。地址偏移量:0x38，初值:0x00188000，宽度:32
 寄存器说明: 状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  debugh : 32; /* bit[0-31]: debug信号高32位，寄存器默认读出值不用care */
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS1_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_debugh_START  (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_debugh_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USBOTG3_STS2_UNION
 结构说明  : USBOTG3_STS2 寄存器结构定义。地址偏移量:0x3c，初值:0x00000000，宽度:32
 寄存器说明: 状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  logic_analyzer_trace : 32; /* bit[0-31]: logic_analyzer_trace信号低32位，寄存器默认读出值不用care */
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS2_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_logic_analyzer_trace_START  (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_logic_analyzer_trace_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USBOTG3_STS3_UNION
 结构说明  : USBOTG3_STS3 寄存器结构定义。地址偏移量:0x40，初值:0x00000000，宽度:32
 寄存器说明: 状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  logic_analyzer_traceh : 32; /* bit[0-31]: logic_analyzer_trace信号高32位，寄存器默认读出值不用care */
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS3_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_logic_analyzer_traceh_START  (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_logic_analyzer_traceh_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_BC_CTRL0_UNION
 结构说明  : BC_CTRL0 寄存器结构定义。地址偏移量:0x44，初值:0x00000205，宽度:32
 寄存器说明: BC控制器寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 2;  /* bit[0-1]  : reserved */
        unsigned int  reserved_1: 2;  /* bit[2-3]  : reserved */
        unsigned int  reserved_2: 1;  /* bit[4]    : reserved */
        unsigned int  reserved_3: 1;  /* bit[5]    : reserved */
        unsigned int  reserved_4: 1;  /* bit[6]    : reserved */
        unsigned int  reserved_5: 1;  /* bit[7]    : reserved */
        unsigned int  reserved_6: 1;  /* bit[8]    : reserved */
        unsigned int  reserved_7: 2;  /* bit[9-10] : reserved */
        unsigned int  reserved_8: 21; /* bit[11-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_BC_CTRL0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_BC_CTRL1_UNION
 结构说明  : BC_CTRL1 寄存器结构定义。地址偏移量:0x48，初值:0x00000000，宽度:32
 寄存器说明: BC控制器寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_BC_CTRL1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG4c_UNION
 结构说明  : USB_MISC_CFG4c 寄存器结构定义。地址偏移量:0x4c，初值:0x00004058，宽度:32
 寄存器说明: RAM控制寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mem_ctrl : 16; /* bit[0-15] : USB31_DP_combo_PHY下面U_USB_PHY_RAM和U_USB31_MEM_WRAP下面*RAM*的控制 */
        unsigned int  reserved : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG4c_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_mem_ctrl_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_mem_ctrl_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USBOTG3_STS4_UNION
 结构说明  : USBOTG3_STS4 寄存器结构定义。地址偏移量:0x50，初值:0x00000000，宽度:32
 寄存器说明: 状态寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  host_legacy_smi_interrupt : 1;  /* bit[0]   :  */
        unsigned int  reserved_0                : 3;  /* bit[1-3] :  */
        unsigned int  usb31_debug_i1            : 2;  /* bit[4-5] :  */
        unsigned int  sc_usb31_host_system_err  : 1;  /* bit[6]   :  */
        unsigned int  reserved_1                : 25; /* bit[7-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS4_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_host_legacy_smi_interrupt_START  (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_host_legacy_smi_interrupt_END    (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_usb31_debug_i1_START             (4)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_usb31_debug_i1_END               (5)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_sc_usb31_host_system_err_START   (6)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_sc_usb31_host_system_err_END     (6)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG54_UNION
 结构说明  : USB_MISC_CFG54 寄存器结构定义。地址偏移量:0x54，初值:0x00800004，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  host_legacy_smi_pci_cmd_reg_wr : 1;  /* bit[0]    : host_legacy_smi_pci_cmd_reg_wr */
        unsigned int  host_legacy_smi_bar_wr         : 1;  /* bit[1]    : host_legacy_smi_bar_wr */
        unsigned int  pme_en                         : 1;  /* bit[2]    : pme_en */
        unsigned int  reserved_0                     : 1;  /* bit[3]    :  */
        unsigned int  usb3_typec_flip_invert         : 1;  /* bit[4]    : type-c connector flip orientation sense
                                                                          0: usb3_typec_flip active high
                                                                          1: usb3_typec_flip active low
                                                                          don't care,暂时不使用 */
        unsigned int  reserved_1                     : 18; /* bit[5-22] :  */
        unsigned int  usb3_phy_test_powerdown        : 1;  /* bit[23]   : usb3_phy_test_powerdown
                                                                          1: power down mode
                                                                          0: normal mode
                                                                          don't care,暂时不使用 */
        unsigned int  reserved_2                     : 8;  /* bit[24-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG54_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_pci_cmd_reg_wr_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_pci_cmd_reg_wr_END    (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_bar_wr_START          (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_bar_wr_END            (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_pme_en_START                          (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_pme_en_END                            (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_typec_flip_invert_START          (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_typec_flip_invert_END            (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_phy_test_powerdown_START         (23)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_phy_test_powerdown_END           (23)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG58_UNION
 结构说明  : USB_MISC_CFG58 寄存器结构定义。地址偏移量:0x58，初值:0x000001FF，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  usb3_upcs_pwr_stable   : 1;  /* bit[0]    : don't care,暂时不使用 */
        unsigned int  usb3_upcs_pwr_en       : 1;  /* bit[1]    : don't care,暂时不使用 */
        unsigned int  phy0_ana_pwr_en        : 1;  /* bit[2]    : don't care,暂时不使用 */
        unsigned int  phy0_ana_pwr_stable    : 1;  /* bit[3]    : don't care,暂时不使用 */
        unsigned int  phy0_pcs_pwr_en        : 1;  /* bit[4]    : don't care,暂时不使用 */
        unsigned int  phy0_pcs_pwr_stable    : 1;  /* bit[5]    : don't care,暂时不使用 */
        unsigned int  phy0_pma_pwr_en        : 1;  /* bit[6]    : don't care,暂时不使用 */
        unsigned int  phy0_pma_pwr_stable    : 1;  /* bit[7]    : don't care,暂时不使用 */
        unsigned int  usb31_pmgt_ref_clk_ok  : 1;  /* bit[8]    : don't care,暂时不使用 */
        unsigned int  usb31_pmgt_ref_clk_off : 1;  /* bit[9]    : don't care,暂时不使用 */
        unsigned int  reserved               : 22; /* bit[10-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG58_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_stable_START    (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_stable_END      (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_en_START        (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_en_END          (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_en_START         (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_en_END           (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_stable_START     (3)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_stable_END       (3)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_en_START         (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_en_END           (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_stable_START     (5)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_stable_END       (5)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_en_START         (6)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_en_END           (6)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_stable_START     (7)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_stable_END       (7)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_ok_START   (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_ok_END     (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_off_START  (9)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_off_END    (9)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG5C_UNION
 结构说明  : USB_MISC_CFG5C 寄存器结构定义。地址偏移量:0x5c，初值:0x00004058，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_usb3_mem_pwr_ram0_half : 16; /* bit[0-15] : USB31_MEM_WRAP下面U_USB_RAM0.U_RAM0_HALF的控制 */
        unsigned int  reserved                  : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG5C_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_sc_usb3_mem_pwr_ram0_half_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_sc_usb3_mem_pwr_ram0_half_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG60_UNION
 结构说明  : USB_MISC_CFG60 寄存器结构定义。地址偏移量:0x60，初值:0x00004058，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_usb3_mem_pwr_ram0_half2 : 16; /* bit[0-15] : USB31_MEM_WRAP下面U_USB_RAM0.U_RAM0_HALF_2的控制 */
        unsigned int  reserved                   : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG60_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_sc_usb3_mem_pwr_ram0_half2_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_sc_usb3_mem_pwr_ram0_half2_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG64_UNION
 结构说明  : USB_MISC_CFG64 寄存器结构定义。地址偏移量:0x64，初值:0x00004058，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_usb3_mem_pwr_ram2 : 16; /* bit[0-15] : USB31_MEM_WRAP下面U_USB_RAM1的控制 */
        unsigned int  reserved             : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG64_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_sc_usb3_mem_pwr_ram2_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_sc_usb3_mem_pwr_ram2_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG68_UNION
 结构说明  : USB_MISC_CFG68 寄存器结构定义。地址偏移量:0x68，初值:0x00004058，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_usb3_mem_pwr_ram1_half : 16; /* bit[0-15] : USB31_MEM_WRAP下面U_USB_RAM2.U_RAM0_HALF的控制 */
        unsigned int  reserved                  : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG68_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_sc_usb3_mem_pwr_ram1_half_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_sc_usb3_mem_pwr_ram1_half_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFG6C_UNION
 结构说明  : USB_MISC_CFG6C 寄存器结构定义。地址偏移量:0x6c，初值:0x00004058，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_usb3_mem_pwr_ram1_half2 : 16; /* bit[0-15] : USB31_MEM_WRAP下面U_USB_RAM2.U_RAM0_HALF_2的控制 */
        unsigned int  reserved                   : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG6C_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_sc_usb3_mem_pwr_ram1_half2_START  (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_sc_usb3_mem_pwr_ram1_half2_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_USB_MISC_CFGA0_UNION
 结构说明  : USB_MISC_CFGA0 寄存器结构定义。地址偏移量:0xA0，初值:0x00000002，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  usb2phy_por_n   : 1;  /* bit[0]    : FPGA版本用，asic版本不用.
                                                           cfg_usb2phy_por_n
                                                           0: reset
                                                           1: release
                                                           'don't care,暂时不使用 */
        unsigned int  usb3phy_reset_n : 1;  /* bit[1]    : usb3phy reset:
                                                           1: reset
                                                           0: release */
        unsigned int  reserved_0      : 6;  /* bit[2-7]  :  */
        unsigned int  usb31c_resetn   : 1;  /* bit[8]    : usb31 controller reset:
                                                           0: reset
                                                           1: release */
        unsigned int  reserved_1      : 3;  /* bit[9-11] :  */
        unsigned int  tca_dp4_por     : 1;  /* bit[12]   : ComboPHY PoR as DP4:
                                                           0: reset as usb
                                                           1: reset as dp4
                                                           'don't care,暂时不使用 */
        unsigned int  tca_usb_dev_por : 1;  /* bit[13]   : PoR as USB Device:
                                                           0: Normal
                                                           1: reset as usb device
                                                           'don't care,暂时不使用 */
        unsigned int  tca_mux_enable  : 1;  /* bit[14]   : tca mux enable:
                                                           0: disable
                                                           1: enable
                                                           'don't care,暂时不使用 */
        unsigned int  testpoint_mux   : 2;  /* bit[15-16]: debug port 选择信号：
                                                           00： 控制器logic_analyzer_trace[31:0]
                                                           01： 控制器logic_analyzer_trace[63:32]
                                                           10: {23'b0, usb31_host_system_err,combophy_debug_port[7:0]
                                                           
                                                           other: {24'd0, pcie1 phy debug port} */
        unsigned int  reserved_2      : 15; /* bit[17-31]: reserved */
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFGA0_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb2phy_por_n_START    (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb2phy_por_n_END      (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb3phy_reset_n_START  (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb3phy_reset_n_END    (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb31c_resetn_START    (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb31c_resetn_END      (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_dp4_por_START      (12)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_dp4_por_END        (12)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_usb_dev_por_START  (13)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_usb_dev_por_END    (13)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_mux_enable_START   (14)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_mux_enable_END     (14)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_testpoint_mux_START    (15)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_testpoint_mux_END      (16)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_UNION
 结构说明  : sc_usb20phy_misc_ctrl11 寄存器结构定义。地址偏移量:0x0074，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cfg_scan_sclk_in : 7;  /* bit[0-6]  : don't care,暂时不使用 */
        unsigned int  reserved         : 24; /* bit[7-30] :  */
        unsigned int  ate_apb_test     : 1;  /* bit[31-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_cfg_scan_sclk_in_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_cfg_scan_sclk_in_END    (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ate_apb_test_START      (31)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ate_apb_test_END        (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_UNION
 结构说明  : sc_usb20phy_misc_ctrl12 寄存器结构定义。地址偏移量:0x0078，初值:0x000000C0，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cfg_eusb2_utmi_opmode     : 2;  /* bit[0-1]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb2_utmi_termselect : 1;  /* bit[2-2]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb2_utmi_xcvrselect : 2;  /* bit[3-4]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb_utmi_portreset   : 1;  /* bit[5-5]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb2_utmi_sleep_n    : 1;  /* bit[6-6]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb2_utmi_suspend_n  : 1;  /* bit[7-7]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb2_utmi_txvalid    : 1;  /* bit[8-8]  : don't care,暂时不使用 */
        unsigned int  cfg_eusb2_utmi_tx_data    : 8;  /* bit[9-16] : don't care,暂时不使用 */
        unsigned int  reserved                  : 15; /* bit[17-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_opmode_START      (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_opmode_END        (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_termselect_START  (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_termselect_END    (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_xcvrselect_START  (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_xcvrselect_END    (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb_utmi_portreset_START    (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb_utmi_portreset_END      (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_sleep_n_START     (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_sleep_n_END       (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_suspend_n_START   (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_suspend_n_END     (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_txvalid_START     (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_txvalid_END       (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_tx_data_START     (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_tx_data_END       (16)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_UNION
 结构说明  : sc_usb20phy_misc_ctrl13 寄存器结构定义。地址偏移量:0x007c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  eusb_ram_clk_sel : 1;  /* bit[0-0]  : don't care,暂时不使用 */
        unsigned int  eusb_read_en     : 1;  /* bit[1-1]  : don't care,暂时不使用 */
        unsigned int  eusb_read_addr   : 8;  /* bit[2-9]  : don't care,暂时不使用 */
        unsigned int  ram_ful_stop     : 1;  /* bit[10-10]:  */
        unsigned int  ram_clear        : 1;  /* bit[11-11]:  */
        unsigned int  reserved         : 20; /* bit[12-31]: 'don't care,暂时不使用 */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_ram_clk_sel_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_ram_clk_sel_END    (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_en_START      (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_en_END        (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_addr_START    (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_addr_END      (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_ful_stop_START      (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_ful_stop_END        (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_clear_START         (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_clear_END           (11)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_UNION
 结构说明  : sc_usb20phy_misc_ctrl14 寄存器结构定义。地址偏移量:0x0080，初值:0x00004058，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ocla_mem_ctrl : 16; /* bit[0-15] : don't care,暂时不使用 */
        unsigned int  reserved      : 16; /* bit[16-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ocla_mem_ctrl_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ocla_mem_ctrl_END    (15)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_UNION
 结构说明  : sc_usb20phy_misc_ctrl15 寄存器结构定义。地址偏移量:0x0084，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ocla_ram_data_31_0 : 32; /* bit[0-31]: don't care,暂时不使用 */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ocla_ram_data_31_0_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ocla_ram_data_31_0_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_UNION
 结构说明  : sc_usb20phy_misc_ctrl16 寄存器结构定义。地址偏移量:0x0088，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ocla_ram_data_63_32 : 32; /* bit[0-31]: don't care,暂时不使用 */
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ocla_ram_data_63_32_START  (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ocla_ram_data_63_32_END    (31)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_usb_pipe_rst_n_UNION
 结构说明  : usb_pipe_rst_n 寄存器结构定义。地址偏移量:0x008c，初值:0x00000001，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  usb_pipe_rst_n   : 1;  /* bit[0-0] : 软件控制的combophy pipe reset */
        unsigned int  usb_pipe_rst_sel : 1;  /* bit[1-1] : combophy pipe reset 选择信号
                                                           1: 软件控制
                                                           0：控制器其控制 */
        unsigned int  reserved         : 30; /* bit[2-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_usb_pipe_rst_n_UNION;
#endif
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_n_START    (0)
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_n_END      (0)
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_sel_START  (1)
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_sel_END    (1)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_dwc_usb_ctrl0_UNION
 结构说明  : dwc_usb_ctrl0 寄存器结构定义。地址偏移量:0x0090，初值:0x00000024，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ls_eop_timer : 18; /* bit[0-17] : 控制器ls 模式下EOP的长度参数 */
        unsigned int  reserved     : 14; /* bit[18-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_dwc_usb_ctrl0_UNION;
#endif
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ls_eop_timer_START  (0)
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ls_eop_timer_END    (17)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_dwc_usb_ctrl1_UNION
 结构说明  : dwc_usb_ctrl1 寄存器结构定义。地址偏移量:0x0094，初值:0x00000004，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  nols_eop_timer : 18; /* bit[0-17] : 控制器非ls 模式下EOP的长度参数 */
        unsigned int  reserved       : 14; /* bit[18-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_dwc_usb_ctrl1_UNION;
#endif
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_nols_eop_timer_START  (0)
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_nols_eop_timer_END    (17)


/*****************************************************************************
 结构名    : SOC_USB_MISC_CTRL_dwc_usb_ctrl2_UNION
 结构说明  : dwc_usb_ctrl2 寄存器结构定义。地址偏移量:0x0098，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  eop_timer_capture : 1;  /* bit[0-0] : 控制器EOP 长度参数采样使能 */
        unsigned int  reserved          : 31; /* bit[1-31]:  */
    } reg;
} SOC_USB_MISC_CTRL_dwc_usb_ctrl2_UNION;
#endif
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_eop_timer_capture_START  (0)
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_eop_timer_capture_END    (0)






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

#endif /* end of soc_usb_misc_ctrl_interface.h */
