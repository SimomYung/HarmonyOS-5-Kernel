/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: pmic_mmw_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __PMIC_MMW_MMW_INTERFACE_H__
#define __PMIC_MMW_MMW_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/5) PMU_CTRL
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：版本寄存器0。
   位域定义UNION结构:  PMIC_MMW_VERSION0_UNION */
#define PMIC_MMW_VERSION0_ADDR(base)                              ((base) + (0x000UL))

/* 寄存器说明：版本寄存器1。
   位域定义UNION结构:  PMIC_MMW_VERSION1_UNION */
#define PMIC_MMW_VERSION1_ADDR(base)                              ((base) + (0x001UL))

/* 寄存器说明：版本寄存器2。
   位域定义UNION结构:  PMIC_MMW_VERSION2_UNION */
#define PMIC_MMW_VERSION2_ADDR(base)                              ((base) + (0x002UL))

/* 寄存器说明：版本寄存器3。
   位域定义UNION结构:  PMIC_MMW_VERSION3_UNION */
#define PMIC_MMW_VERSION3_ADDR(base)                              ((base) + (0x003UL))

/* 寄存器说明：版本寄存器4。
   位域定义UNION结构:  PMIC_MMW_VERSION4_UNION */
#define PMIC_MMW_VERSION4_ADDR(base)                              ((base) + (0x004UL))

/* 寄存器说明：版本寄存器5。
   位域定义UNION结构:  PMIC_MMW_VERSION5_UNION */
#define PMIC_MMW_VERSION5_ADDR(base)                              ((base) + (0x005UL))

/* 寄存器说明：状态标志寄存器0。
   位域定义UNION结构:  PMIC_MMW_STATUS0_UNION */
#define PMIC_MMW_STATUS0_ADDR(base)                               ((base) + (0x006UL))

/* 寄存器说明：状态标志寄存器1。
   位域定义UNION结构:  PMIC_MMW_STATUS1_UNION */
#define PMIC_MMW_STATUS1_ADDR(base)                               ((base) + (0x007UL))

/* 寄存器说明：LDO0开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO0_ONOFF_UNION */
#define PMIC_MMW_LDO0_ONOFF_ADDR(base)                            ((base) + (0x00AUL))

/* 寄存器说明：LDO1开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO1_ONOFF_UNION */
#define PMIC_MMW_LDO1_ONOFF_ADDR(base)                            ((base) + (0x00BUL))

/* 寄存器说明：LDO2开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO2_ONOFF_UNION */
#define PMIC_MMW_LDO2_ONOFF_ADDR(base)                            ((base) + (0x00CUL))

/* 寄存器说明：LDO3开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO3_ONOFF_UNION */
#define PMIC_MMW_LDO3_ONOFF_ADDR(base)                            ((base) + (0x00DUL))

/* 寄存器说明：LDO4开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO4_ONOFF_UNION */
#define PMIC_MMW_LDO4_ONOFF_ADDR(base)                            ((base) + (0x00EUL))

/* 寄存器说明：LDO5开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO5_ONOFF_UNION */
#define PMIC_MMW_LDO5_ONOFF_ADDR(base)                            ((base) + (0x00FUL))

/* 寄存器说明：LDO6开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO6_ONOFF_UNION */
#define PMIC_MMW_LDO6_ONOFF_ADDR(base)                            ((base) + (0x010UL))

/* 寄存器说明：LDO7开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO7_ONOFF_ECO_UNION */
#define PMIC_MMW_LDO7_ONOFF_ECO_ADDR(base)                        ((base) + (0x011UL))

/* 寄存器说明：LDO8开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO8_ONOFF_ECO_UNION */
#define PMIC_MMW_LDO8_ONOFF_ECO_ADDR(base)                        ((base) + (0x012UL))

/* 寄存器说明：LDO_CORE开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_ONOFF_ECO_UNION */
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_ADDR(base)                    ((base) + (0x013UL))

/* 寄存器说明：LDO_BUF_RF开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF_RF_ONOFF_UNION */
#define PMIC_MMW_LDO_BUF_RF_ONOFF_ADDR(base)                      ((base) + (0x014UL))

/* 寄存器说明：LDO_BUF0开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF0_ONOFF_UNION */
#define PMIC_MMW_LDO_BUF0_ONOFF_ADDR(base)                        ((base) + (0x015UL))

/* 寄存器说明：LDO_BUF1开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF1_ONOFF_UNION */
#define PMIC_MMW_LDO_BUF1_ONOFF_ADDR(base)                        ((base) + (0x016UL))

/* 寄存器说明：LDO_RFBUF0开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF0_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF0_ONOFF_ADDR(base)                      ((base) + (0x017UL))

/* 寄存器说明：LDO_RFBUF1开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF1_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF1_ONOFF_ADDR(base)                      ((base) + (0x018UL))

/* 寄存器说明：LDO_RFBUF2开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF2_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF2_ONOFF_ADDR(base)                      ((base) + (0x019UL))

/* 寄存器说明：LDO_RFBUF3开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF3_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF3_ONOFF_ADDR(base)                      ((base) + (0x01AUL))

/* 寄存器说明：LDO_RFBUF4开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF4_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF4_ONOFF_ADDR(base)                      ((base) + (0x01BUL))

/* 寄存器说明：PMUH开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_PMUH_ONOFF_UNION */
#define PMIC_MMW_PMUH_ONOFF_ADDR(base)                            ((base) + (0x01CUL))

/* 寄存器说明：XO_CORE开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_ONOFF_UNION */
#define PMIC_MMW_XO_CORE_ONOFF_ADDR(base)                         ((base) + (0x01DUL))

/* 寄存器说明：THSD开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_THSD_ONOFF_ECO_UNION */
#define PMIC_MMW_THSD_ONOFF_ECO_ADDR(base)                        ((base) + (0x01EUL))

/* 寄存器说明：UFS时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_UFS_EN_UNION */
#define PMIC_MMW_CLK_UFS_EN_ADDR(base)                            ((base) + (0x020UL))

/* 寄存器说明：RF0时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_EN_UNION */
#define PMIC_MMW_CLK_RF0_EN_ADDR(base)                            ((base) + (0x021UL))

/* 寄存器说明：RF1时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_EN_UNION */
#define PMIC_MMW_CLK_RF1_EN_ADDR(base)                            ((base) + (0x022UL))

/* 寄存器说明：RF2时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_EN_UNION */
#define PMIC_MMW_CLK_RF2_EN_ADDR(base)                            ((base) + (0x023UL))

/* 寄存器说明：RF3时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_EN_UNION */
#define PMIC_MMW_CLK_RF3_EN_ADDR(base)                            ((base) + (0x024UL))

/* 寄存器说明：RF4时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_EN_UNION */
#define PMIC_MMW_CLK_RF4_EN_ADDR(base)                            ((base) + (0x025UL))

/* 寄存器说明：SERDES时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_EN_UNION */
#define PMIC_MMW_CLK_SERDES_EN_ADDR(base)                         ((base) + (0x026UL))

/* 寄存器说明：WIFI0时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_EN_UNION */
#define PMIC_MMW_CLK_WIFI0_EN_ADDR(base)                          ((base) + (0x027UL))

/* 寄存器说明：WIFI1时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_EN_UNION */
#define PMIC_MMW_CLK_WIFI1_EN_ADDR(base)                          ((base) + (0x028UL))

/* 寄存器说明：NFC时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_EN_UNION */
#define PMIC_MMW_CLK_NFC_EN_ADDR(base)                            ((base) + (0x029UL))

/* 寄存器说明：SYS时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_EN_UNION */
#define PMIC_MMW_CLK_SYS_EN_ADDR(base)                            ((base) + (0x02AUL))

/* 寄存器说明：送给主PMU的时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_EN_UNION */
#define PMIC_MMW_CLK_PMU_EN_ADDR(base)                            ((base) + (0x02BUL))

/* 寄存器说明：32kHz时钟SYS输出使能控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_UNION */
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_ADDR(base)                 ((base) + (0x02CUL))

/* 寄存器说明：LDO0调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO0_VSET_UNION */
#define PMIC_MMW_LDO0_VSET_ADDR(base)                             ((base) + (0x02DUL))

/* 寄存器说明：LDO1调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO1_VSET_UNION */
#define PMIC_MMW_LDO1_VSET_ADDR(base)                             ((base) + (0x02EUL))

/* 寄存器说明：LDO2调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO2_VSET_UNION */
#define PMIC_MMW_LDO2_VSET_ADDR(base)                             ((base) + (0x02FUL))

/* 寄存器说明：LDO3调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO3_VSET_UNION */
#define PMIC_MMW_LDO3_VSET_ADDR(base)                             ((base) + (0x030UL))

/* 寄存器说明：LDO4调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO4_VSET_UNION */
#define PMIC_MMW_LDO4_VSET_ADDR(base)                             ((base) + (0x031UL))

/* 寄存器说明：LDO5调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO5_VSET_UNION */
#define PMIC_MMW_LDO5_VSET_ADDR(base)                             ((base) + (0x032UL))

/* 寄存器说明：LDO6调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO6_VSET_UNION */
#define PMIC_MMW_LDO6_VSET_ADDR(base)                             ((base) + (0x033UL))

/* 寄存器说明：LDO7调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO7_VSET_UNION */
#define PMIC_MMW_LDO7_VSET_ADDR(base)                             ((base) + (0x034UL))

/* 寄存器说明：LDO8调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO8_VSET_UNION */
#define PMIC_MMW_LDO8_VSET_ADDR(base)                             ((base) + (0x035UL))

/* 寄存器说明：LDO_CORE调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_VSET_UNION */
#define PMIC_MMW_LDO_CORE_VSET_ADDR(base)                         ((base) + (0x036UL))

/* 寄存器说明：LDO_CORE ECO调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_VSET_ECO_UNION */
#define PMIC_MMW_LDO_CORE_VSET_ECO_ADDR(base)                     ((base) + (0x037UL))

/* 寄存器说明：LDO_BUF_RF调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF_RF_VSET_UNION */
#define PMIC_MMW_LDO_BUF_RF_VSET_ADDR(base)                       ((base) + (0x038UL))

/* 寄存器说明：LDO_BUF0调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF0_VSET_UNION */
#define PMIC_MMW_LDO_BUF0_VSET_ADDR(base)                         ((base) + (0x039UL))

/* 寄存器说明：LDO_BUF1调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF1_VSET_UNION */
#define PMIC_MMW_LDO_BUF1_VSET_ADDR(base)                         ((base) + (0x03AUL))

/* 寄存器说明：LDO_RFBUF0调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF0_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF0_VSET_ADDR(base)                       ((base) + (0x03BUL))

/* 寄存器说明：LDO_RFBUF1调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF1_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF1_VSET_ADDR(base)                       ((base) + (0x03CUL))

/* 寄存器说明：LDO_RFBUF2调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF2_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF2_VSET_ADDR(base)                       ((base) + (0x03DUL))

/* 寄存器说明：LDO_RFBUF3调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF3_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF3_VSET_ADDR(base)                       ((base) + (0x03EUL))

/* 寄存器说明：LDO_RFBUF4调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF4_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF4_VSET_ADDR(base)                       ((base) + (0x03FUL))

/* 寄存器说明：LDO_PMUH调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_PMUH_VSET_UNION */
#define PMIC_MMW_LDO_PMUH_VSET_ADDR(base)                         ((base) + (0x040UL))

/* 寄存器说明：XO_CORE电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_CURR_UNION */
#define PMIC_MMW_XO_CORE_CURR_ADDR(base)                          ((base) + (0x041UL))

/* 寄存器说明：XO_CORE ECO电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_CURR_ECO_UNION */
#define PMIC_MMW_XO_CORE_CURR_ECO_ADDR(base)                      ((base) + (0x042UL))

/* 寄存器说明：XO_INBUFF电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_INBUFF_CURR_UNION */
#define PMIC_MMW_XO_INBUFF_CURR_ADDR(base)                        ((base) + (0x043UL))

/* 寄存器说明：XO_INBUFF ECO电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_INBUFF_CURR_ECO_UNION */
#define PMIC_MMW_XO_INBUFF_CURR_ECO_ADDR(base)                    ((base) + (0x044UL))

/* 寄存器说明：LDO0_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO0_CFG_REG_0_UNION */
#define PMIC_MMW_LDO0_CFG_REG_0_ADDR(base)                        ((base) + (0x045UL))

/* 寄存器说明：LDO0_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO0_CFG_REG_1_UNION */
#define PMIC_MMW_LDO0_CFG_REG_1_ADDR(base)                        ((base) + (0x046UL))

/* 寄存器说明：LDO0_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO0_CFG_REG_2_UNION */
#define PMIC_MMW_LDO0_CFG_REG_2_ADDR(base)                        ((base) + (0x047UL))

/* 寄存器说明：LDO1_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO1_CFG_REG_0_UNION */
#define PMIC_MMW_LDO1_CFG_REG_0_ADDR(base)                        ((base) + (0x048UL))

/* 寄存器说明：LDO1_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO1_CFG_REG_1_UNION */
#define PMIC_MMW_LDO1_CFG_REG_1_ADDR(base)                        ((base) + (0x049UL))

/* 寄存器说明：LDO1_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO1_CFG_REG_2_UNION */
#define PMIC_MMW_LDO1_CFG_REG_2_ADDR(base)                        ((base) + (0x04AUL))

/* 寄存器说明：LDO2_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO2_CFG_REG_0_UNION */
#define PMIC_MMW_LDO2_CFG_REG_0_ADDR(base)                        ((base) + (0x04BUL))

/* 寄存器说明：LDO2_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO2_CFG_REG_1_UNION */
#define PMIC_MMW_LDO2_CFG_REG_1_ADDR(base)                        ((base) + (0x04CUL))

/* 寄存器说明：LDO2_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO2_CFG_REG_2_UNION */
#define PMIC_MMW_LDO2_CFG_REG_2_ADDR(base)                        ((base) + (0x04DUL))

/* 寄存器说明：LDO3_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO3_CFG_REG_0_UNION */
#define PMIC_MMW_LDO3_CFG_REG_0_ADDR(base)                        ((base) + (0x04EUL))

/* 寄存器说明：LDO3_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO3_CFG_REG_1_UNION */
#define PMIC_MMW_LDO3_CFG_REG_1_ADDR(base)                        ((base) + (0x04FUL))

/* 寄存器说明：LDO3_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO3_CFG_REG_2_UNION */
#define PMIC_MMW_LDO3_CFG_REG_2_ADDR(base)                        ((base) + (0x050UL))

/* 寄存器说明：LDO4_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO4_CFG_REG_0_UNION */
#define PMIC_MMW_LDO4_CFG_REG_0_ADDR(base)                        ((base) + (0x051UL))

/* 寄存器说明：LDO4_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO4_CFG_REG_1_UNION */
#define PMIC_MMW_LDO4_CFG_REG_1_ADDR(base)                        ((base) + (0x052UL))

/* 寄存器说明：LDO4_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO4_CFG_REG_2_UNION */
#define PMIC_MMW_LDO4_CFG_REG_2_ADDR(base)                        ((base) + (0x053UL))

/* 寄存器说明：LDO5_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO5_CFG_REG_0_UNION */
#define PMIC_MMW_LDO5_CFG_REG_0_ADDR(base)                        ((base) + (0x054UL))

/* 寄存器说明：LDO5_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO5_CFG_REG_1_UNION */
#define PMIC_MMW_LDO5_CFG_REG_1_ADDR(base)                        ((base) + (0x055UL))

/* 寄存器说明：LDO6_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO6_CFG_REG_0_UNION */
#define PMIC_MMW_LDO6_CFG_REG_0_ADDR(base)                        ((base) + (0x056UL))

/* 寄存器说明：LDO6_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO6_CFG_REG_1_UNION */
#define PMIC_MMW_LDO6_CFG_REG_1_ADDR(base)                        ((base) + (0x057UL))

/* 寄存器说明：LDO7_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO7_CFG_REG_0_UNION */
#define PMIC_MMW_LDO7_CFG_REG_0_ADDR(base)                        ((base) + (0x058UL))

/* 寄存器说明：LDO7_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO7_CFG_REG_1_UNION */
#define PMIC_MMW_LDO7_CFG_REG_1_ADDR(base)                        ((base) + (0x059UL))

/* 寄存器说明：LDO8_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO8_CFG_REG_0_UNION */
#define PMIC_MMW_LDO8_CFG_REG_0_ADDR(base)                        ((base) + (0x05AUL))

/* 寄存器说明：LDO8_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO8_CFG_REG_1_UNION */
#define PMIC_MMW_LDO8_CFG_REG_1_ADDR(base)                        ((base) + (0x05BUL))

/* 寄存器说明：LDO_CORE_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ADDR(base)                    ((base) + (0x05CUL))

/* 寄存器说明：LDO_BUF0_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_BUF0_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ADDR(base)                    ((base) + (0x05DUL))

/* 寄存器说明：LDO_BUF1_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_BUF1_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ADDR(base)                    ((base) + (0x05EUL))

/* 寄存器说明：LDO_BUF_RF_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_BUF_RF_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ADDR(base)                  ((base) + (0x05FUL))

/* 寄存器说明：LDO_RFBUF0_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF0_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ADDR(base)                  ((base) + (0x060UL))

/* 寄存器说明：LDO_RFBUF1_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF1_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ADDR(base)                  ((base) + (0x061UL))

/* 寄存器说明：LDO_RFBUF2_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF2_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ADDR(base)                  ((base) + (0x062UL))

/* 寄存器说明：LDO_RFBUF3_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF3_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ADDR(base)                  ((base) + (0x063UL))

/* 寄存器说明：LDO_RFBUF4_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF4_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ADDR(base)                  ((base) + (0x064UL))

/* 寄存器说明：PMUH_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_PMUH_CFG_REG_0_UNION */
#define PMIC_MMW_PMUH_CFG_REG_0_ADDR(base)                        ((base) + (0x065UL))

/* 寄存器说明：LDO_RESERVE_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ADDR(base)                 ((base) + (0x066UL))

/* 寄存器说明：LDO_RESERVE_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_1_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ADDR(base)                 ((base) + (0x067UL))

/* 寄存器说明：LDO_RESERVE_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_2_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ADDR(base)                 ((base) + (0x068UL))

/* 寄存器说明：LDO_RESERVE_配置寄存器_3
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_3_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ADDR(base)                 ((base) + (0x069UL))

/* 寄存器说明：UFS时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_UFS_CTRL_UNION */
#define PMIC_MMW_CLK_UFS_CTRL_ADDR(base)                          ((base) + (0x06AUL))

/* 寄存器说明：RF0时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_CTRL_UNION */
#define PMIC_MMW_CLK_RF0_CTRL_ADDR(base)                          ((base) + (0x06BUL))

/* 寄存器说明：RF1时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_CTRL_UNION */
#define PMIC_MMW_CLK_RF1_CTRL_ADDR(base)                          ((base) + (0x06CUL))

/* 寄存器说明：RF2时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_CTRL_UNION */
#define PMIC_MMW_CLK_RF2_CTRL_ADDR(base)                          ((base) + (0x06DUL))

/* 寄存器说明：RF3时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_CTRL_UNION */
#define PMIC_MMW_CLK_RF3_CTRL_ADDR(base)                          ((base) + (0x06EUL))

/* 寄存器说明：RF4时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_CTRL_UNION */
#define PMIC_MMW_CLK_RF4_CTRL_ADDR(base)                          ((base) + (0x06FUL))

/* 寄存器说明：SERDESB时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_CTRL_UNION */
#define PMIC_MMW_CLK_SERDES_CTRL_ADDR(base)                       ((base) + (0x070UL))

/* 寄存器说明：WIFI0时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI0_CTRL_ADDR(base)                        ((base) + (0x071UL))

/* 寄存器说明：WIFI1时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI1_CTRL_ADDR(base)                        ((base) + (0x072UL))

/* 寄存器说明：NFC时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_CTRL_UNION */
#define PMIC_MMW_CLK_NFC_CTRL_ADDR(base)                          ((base) + (0x073UL))

/* 寄存器说明：SYS时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_CTRL_UNION */
#define PMIC_MMW_CLK_SYS_CTRL_ADDR(base)                          ((base) + (0x074UL))

/* 寄存器说明：送给主PMU的时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_CTRL_UNION */
#define PMIC_MMW_CLK_PMU_CTRL_ADDR(base)                          ((base) + (0x075UL))

/* 寄存器说明：XO时钟控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_XO_CTRL0_UNION */
#define PMIC_MMW_XO_CTRL0_ADDR(base)                              ((base) + (0x076UL))

/* 寄存器说明：XO预留寄存器0。
   位域定义UNION结构:  PMIC_MMW_XO_RESERVE_0_UNION */
#define PMIC_MMW_XO_RESERVE_0_ADDR(base)                          ((base) + (0x077UL))

/* 寄存器说明：XO corner检测状态指示寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_DET_STATUS_UNION */
#define PMIC_MMW_XO_DET_STATUS_ADDR(base)                         ((base) + (0x078UL))

/* 寄存器说明：基准控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_BG_THSD_CTRL0_UNION */
#define PMIC_MMW_BG_THSD_CTRL0_ADDR(base)                         ((base) + (0x079UL))

/* 寄存器说明：基准控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_BG_THSD_CTRL1_UNION */
#define PMIC_MMW_BG_THSD_CTRL1_ADDR(base)                         ((base) + (0x07AUL))

/* 寄存器说明：基准预留寄存器0。
   位域定义UNION结构:  PMIC_MMW_BG_RESERVE_0_UNION */
#define PMIC_MMW_BG_RESERVE_0_ADDR(base)                          ((base) + (0x07BUL))

/* 寄存器说明：基准预留寄存器1。
   位域定义UNION结构:  PMIC_MMW_BG_RESERVE_1_UNION */
#define PMIC_MMW_BG_RESERVE_1_ADDR(base)                          ((base) + (0x07CUL))

/* 寄存器说明：基准预留寄存器2。
   位域定义UNION结构:  PMIC_MMW_BG_RESERVE_2_UNION */
#define PMIC_MMW_BG_RESERVE_2_ADDR(base)                          ((base) + (0x07DUL))

/* 寄存器说明：系统控制预留寄存器。
   位域定义UNION结构:  PMIC_MMW_SYS_CTRL_RESERVE_UNION */
#define PMIC_MMW_SYS_CTRL_RESERVE_ADDR(base)                      ((base) + (0x07EUL))

/* 寄存器说明：系统控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_SYS_CTRL0_UNION */
#define PMIC_MMW_SYS_CTRL0_ADDR(base)                             ((base) + (0x07FUL))

/* 寄存器说明：系统控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_SYS_CTRL1_UNION */
#define PMIC_MMW_SYS_CTRL1_ADDR(base)                             ((base) + (0x080UL))

/* 寄存器说明：模拟到数字的保留寄存器0。
   位域定义UNION结构:  PMIC_MMW_A2D_RES0_UNION */
#define PMIC_MMW_A2D_RES0_ADDR(base)                              ((base) + (0x084UL))

/* 寄存器说明：数字到模拟的保留寄存器0。
   位域定义UNION结构:  PMIC_MMW_D2A_RES0_UNION */
#define PMIC_MMW_D2A_RES0_ADDR(base)                              ((base) + (0x085UL))

/* 寄存器说明：数字到模拟的保留寄存器1。
   位域定义UNION结构:  PMIC_MMW_D2A_RES1_UNION */
#define PMIC_MMW_D2A_RES1_ADDR(base)                              ((base) + (0x086UL))

/* 寄存器说明：数字到模拟的保留寄存器2。
   位域定义UNION结构:  PMIC_MMW_D2A_RES2_UNION */
#define PMIC_MMW_D2A_RES2_ADDR(base)                              ((base) + (0x087UL))

/* 寄存器说明：数字到模拟的保留寄存器3。
   位域定义UNION结构:  PMIC_MMW_D2A_RES3_UNION */
#define PMIC_MMW_D2A_RES3_ADDR(base)                              ((base) + (0x088UL))

/* 寄存器说明：OTP回读寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_0_UNION */
#define PMIC_MMW_OTP_0_ADDR(base)                                 ((base) + (0x089UL))

/* 寄存器说明：OTP回读寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_1_UNION */
#define PMIC_MMW_OTP_1_ADDR(base)                                 ((base) + (0x08AUL))

/* 寄存器说明：手动控制读写操作中OTP时钟产生控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OTP_CLK_CTRL_UNION */
#define PMIC_MMW_OTP_CLK_CTRL_ADDR(base)                          ((base) + (0x08BUL))

/* 寄存器说明：OTP控制信号寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_CTRL0_UNION */
#define PMIC_MMW_OTP_CTRL0_ADDR(base)                             ((base) + (0x08CUL))

/* 寄存器说明：OTP控制信号寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_CTRL1_UNION */
#define PMIC_MMW_OTP_CTRL1_ADDR(base)                             ((base) + (0x08DUL))

/* 寄存器说明：OTP控制信号寄存器2。
   位域定义UNION结构:  PMIC_MMW_OTP_CTRL2_UNION */
#define PMIC_MMW_OTP_CTRL2_ADDR(base)                             ((base) + (0x08EUL))

/* 寄存器说明：OTP写操作数据寄存器。
   位域定义UNION结构:  PMIC_MMW_OTP_WDATA_UNION */
#define PMIC_MMW_OTP_WDATA_ADDR(base)                             ((base) + (0x08FUL))

/* 寄存器说明：OTP预修调寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_0_W_UNION */
#define PMIC_MMW_OTP_0_W_ADDR(base)                               ((base) + (0x090UL))

/* 寄存器说明：OTP预修调寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_1_W_UNION */
#define PMIC_MMW_OTP_1_W_ADDR(base)                               ((base) + (0x091UL))

/* 寄存器说明：OTP预修调寄存器2。
   位域定义UNION结构:  PMIC_MMW_OTP_2_W_UNION */
#define PMIC_MMW_OTP_2_W_ADDR(base)                               ((base) + (0x092UL))

/* 寄存器说明：OTP预修调寄存器3。
   位域定义UNION结构:  PMIC_MMW_OTP_3_W_UNION */
#define PMIC_MMW_OTP_3_W_ADDR(base)                               ((base) + (0x093UL))

/* 寄存器说明：OTP预修调寄存器4。
   位域定义UNION结构:  PMIC_MMW_OTP_4_W_UNION */
#define PMIC_MMW_OTP_4_W_ADDR(base)                               ((base) + (0x094UL))

/* 寄存器说明：OTP预修调寄存器5。
   位域定义UNION结构:  PMIC_MMW_OTP_5_W_UNION */
#define PMIC_MMW_OTP_5_W_ADDR(base)                               ((base) + (0x095UL))

/* 寄存器说明：OTP0预修调寄存器6。
   位域定义UNION结构:  PMIC_MMW_OTP_6_W_UNION */
#define PMIC_MMW_OTP_6_W_ADDR(base)                               ((base) + (0x096UL))

/* 寄存器说明：OTP预修调寄存器7。
   位域定义UNION结构:  PMIC_MMW_OTP_7_W_UNION */
#define PMIC_MMW_OTP_7_W_ADDR(base)                               ((base) + (0x097UL))

/* 寄存器说明：OTP预修调寄存器8。
   位域定义UNION结构:  PMIC_MMW_OTP_8_W_UNION */
#define PMIC_MMW_OTP_8_W_ADDR(base)                               ((base) + (0x098UL))

/* 寄存器说明：OTP预修调寄存器9。
   位域定义UNION结构:  PMIC_MMW_OTP_9_W_UNION */
#define PMIC_MMW_OTP_9_W_ADDR(base)                               ((base) + (0x099UL))

/* 寄存器说明：OTP预修调寄存器10。
   位域定义UNION结构:  PMIC_MMW_OTP_10_W_UNION */
#define PMIC_MMW_OTP_10_W_ADDR(base)                              ((base) + (0x09AUL))

/* 寄存器说明：OTP预修调寄存器11。
   位域定义UNION结构:  PMIC_MMW_OTP_11_W_UNION */
#define PMIC_MMW_OTP_11_W_ADDR(base)                              ((base) + (0x09BUL))

/* 寄存器说明：OTP预修调寄存器12。
   位域定义UNION结构:  PMIC_MMW_OTP_12_W_UNION */
#define PMIC_MMW_OTP_12_W_ADDR(base)                              ((base) + (0x09CUL))

/* 寄存器说明：OTP预修调寄存器13。
   位域定义UNION结构:  PMIC_MMW_OTP_13_W_UNION */
#define PMIC_MMW_OTP_13_W_ADDR(base)                              ((base) + (0x09DUL))

/* 寄存器说明：OTP预修调寄存器14。
   位域定义UNION结构:  PMIC_MMW_OTP_14_W_UNION */
#define PMIC_MMW_OTP_14_W_ADDR(base)                              ((base) + (0x09EUL))

/* 寄存器说明：OTP预修调寄存器15。
   位域定义UNION结构:  PMIC_MMW_OTP_15_W_UNION */
#define PMIC_MMW_OTP_15_W_ADDR(base)                              ((base) + (0x09FUL))

/* 寄存器说明：OTP预修调寄存器16。
   位域定义UNION结构:  PMIC_MMW_OTP_16_W_UNION */
#define PMIC_MMW_OTP_16_W_ADDR(base)                              ((base) + (0x0A0UL))

/* 寄存器说明：OTP预修调寄存器17。
   位域定义UNION结构:  PMIC_MMW_OTP_17_W_UNION */
#define PMIC_MMW_OTP_17_W_ADDR(base)                              ((base) + (0x0A1UL))

/* 寄存器说明：OTP预修调寄存器18。
   位域定义UNION结构:  PMIC_MMW_OTP_18_W_UNION */
#define PMIC_MMW_OTP_18_W_ADDR(base)                              ((base) + (0x0A2UL))

/* 寄存器说明：OTP预修调寄存器19。
   位域定义UNION结构:  PMIC_MMW_OTP_19_W_UNION */
#define PMIC_MMW_OTP_19_W_ADDR(base)                              ((base) + (0x0A3UL))

/* 寄存器说明：OTP预修调寄存器20。
   位域定义UNION结构:  PMIC_MMW_OTP_20_W_UNION */
#define PMIC_MMW_OTP_20_W_ADDR(base)                              ((base) + (0x0A4UL))

/* 寄存器说明：OTP预修调寄存器21。
   位域定义UNION结构:  PMIC_MMW_OTP_21_W_UNION */
#define PMIC_MMW_OTP_21_W_ADDR(base)                              ((base) + (0x0A5UL))

/* 寄存器说明：LDO0/1/2/3 OCP自动关断控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO0_3_OCP_CTRL_UNION */
#define PMIC_MMW_LDO0_3_OCP_CTRL_ADDR(base)                       ((base) + (0x0A6UL))

/* 寄存器说明：LCO4/5/6/7 OCP自动关断控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO4_7_OCP_CTRL_UNION */
#define PMIC_MMW_LDO4_7_OCP_CTRL_ADDR(base)                       ((base) + (0x0A7UL))

/* 寄存器说明：LDO8/LDO_CORE/LDO_BUF0/LDO_BUF_RF OCP自动关断控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO8_12_OCP_CTRL_UNION */
#define PMIC_MMW_LDO8_12_OCP_CTRL_ADDR(base)                      ((base) + (0x0A8UL))

/* 寄存器说明：OCP滤波时间控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OCP_DEB_CTRL0_UNION */
#define PMIC_MMW_OCP_DEB_CTRL0_ADDR(base)                         ((base) + (0x0A9UL))

/* 寄存器说明：OCP滤波时间控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OCP_DEB_CTRL1_UNION */
#define PMIC_MMW_OCP_DEB_CTRL1_ADDR(base)                         ((base) + (0x0AAUL))

/* 寄存器说明：过温140度关机滤波时间选择寄存器。
   位域定义UNION结构:  PMIC_MMW_THSD_140_DEB_CTRL_UNION */
#define PMIC_MMW_THSD_140_DEB_CTRL_ADDR(base)                     ((base) + (0x0ABUL))

/* 寄存器说明：OCP和DOCP滤波开关寄存器。
   位域定义UNION结构:  PMIC_MMW_OCP_SCP_ONOFF_UNION */
#define PMIC_MMW_OCP_SCP_ONOFF_ADDR(base)                         ((base) + (0x0ACUL))

/* 寄存器说明：硬线屏蔽寄存器0。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL0_UNION */
#define PMIC_MMW_HARDWIRE_CTRL0_ADDR(base)                        ((base) + (0x0ADUL))

/* 寄存器说明：硬线屏蔽寄存器1。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL1_UNION */
#define PMIC_MMW_HARDWIRE_CTRL1_ADDR(base)                        ((base) + (0x0AEUL))

/* 寄存器说明：硬线屏蔽寄存器2。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL2_UNION */
#define PMIC_MMW_HARDWIRE_CTRL2_ADDR(base)                        ((base) + (0x0AFUL))

/* 寄存器说明：硬线屏蔽寄存器3。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL3_UNION */
#define PMIC_MMW_HARDWIRE_CTRL3_ADDR(base)                        ((base) + (0x0B0UL))

/* 寄存器说明：硬线屏蔽寄存器4。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL4_UNION */
#define PMIC_MMW_HARDWIRE_CTRL4_ADDR(base)                        ((base) + (0x0B1UL))

/* 寄存器说明：硬线屏蔽寄存器5。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL5_UNION */
#define PMIC_MMW_HARDWIRE_CTRL5_ADDR(base)                        ((base) + (0x0B2UL))

/* 寄存器说明：下电区写屏蔽控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LOCK_UNION */
#define PMIC_MMW_LOCK_ADDR(base)                                  ((base) + (0x0B3UL))

/* 寄存器说明：软件在eco模式下对SPMI有读写需求时，提供SPMI时钟以及同步用的19.2M ssi时钟(该寄存器时钟为常开时钟)
   位域定义UNION结构:  PMIC_MMW_SPMI_ECO_GT_BYPASS_UNION */
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_ADDR(base)                    ((base) + (0x0B4UL))

/* 寄存器说明：LDO5和LDO6的缓启完成时间配置寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_SST_END_CFG_UNION */
#define PMIC_MMW_LDO_SST_END_CFG_ADDR(base)                       ((base) + (0x0B5UL))

/* 寄存器说明：非debug模式，配置reg_xo_rf0_en~reg_xo_rf4_en开启时，对应的电源和时钟使能开启时间配置寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_UNION */
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_ADDR(base)                 ((base) + (0x0B6UL))

/* 寄存器说明：ldo_rfbuf_en、ldo_rfbuf0~4_en、xo_rf0~4_en开启和关闭debug模式控制寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_RF_CLK_ON_DEBUG_UNION */
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_ADDR(base)                    ((base) + (0x0B7UL))

/* 寄存器说明：XO CORE电流debug模式配置寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_CURR_DEBUG_UNION */
#define PMIC_MMW_XO_CORE_CURR_DEBUG_ADDR(base)                    ((base) + (0x0B8UL))

/* 寄存器说明：XO INPUT BUFFER电流debug模式配置寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_INBUFF_CURR_DEBUG_UNION */
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_ADDR(base)                  ((base) + (0x0B9UL))

/* 寄存器说明：SPMI debug寄存器0。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG0_UNION */
#define PMIC_MMW_SPMI_DEBUG0_ADDR(base)                           ((base) + (0x0F0UL))

/* 寄存器说明：SPMI debug寄存器1。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG1_UNION */
#define PMIC_MMW_SPMI_DEBUG1_ADDR(base)                           ((base) + (0x0F1UL))

/* 寄存器说明：SPMI debug寄存器2。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG2_UNION */
#define PMIC_MMW_SPMI_DEBUG2_ADDR(base)                           ((base) + (0x0F2UL))

/* 寄存器说明：SPMI debug寄存器3。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG3_UNION */
#define PMIC_MMW_SPMI_DEBUG3_ADDR(base)                           ((base) + (0x0F3UL))

/* 寄存器说明：SPMI debug寄存器4。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG4_UNION */
#define PMIC_MMW_SPMI_DEBUG4_ADDR(base)                           ((base) + (0x0F4UL))

/* 寄存器说明：SPMI debug寄存器5。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG5_UNION */
#define PMIC_MMW_SPMI_DEBUG5_ADDR(base)                           ((base) + (0x0F5UL))

/* 寄存器说明：SPMI debug寄存器6。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG6_UNION */
#define PMIC_MMW_SPMI_DEBUG6_ADDR(base)                           ((base) + (0x0F6UL))

/* 寄存器说明：SPMI debug寄存器7。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG7_UNION */
#define PMIC_MMW_SPMI_DEBUG7_ADDR(base)                           ((base) + (0x0F7UL))


#else


/* 寄存器说明：版本寄存器0。
   位域定义UNION结构:  PMIC_MMW_VERSION0_UNION */
#define PMIC_MMW_VERSION0_ADDR(base)                              ((base) + (0x000))

/* 寄存器说明：版本寄存器1。
   位域定义UNION结构:  PMIC_MMW_VERSION1_UNION */
#define PMIC_MMW_VERSION1_ADDR(base)                              ((base) + (0x001))

/* 寄存器说明：版本寄存器2。
   位域定义UNION结构:  PMIC_MMW_VERSION2_UNION */
#define PMIC_MMW_VERSION2_ADDR(base)                              ((base) + (0x002))

/* 寄存器说明：版本寄存器3。
   位域定义UNION结构:  PMIC_MMW_VERSION3_UNION */
#define PMIC_MMW_VERSION3_ADDR(base)                              ((base) + (0x003))

/* 寄存器说明：版本寄存器4。
   位域定义UNION结构:  PMIC_MMW_VERSION4_UNION */
#define PMIC_MMW_VERSION4_ADDR(base)                              ((base) + (0x004))

/* 寄存器说明：版本寄存器5。
   位域定义UNION结构:  PMIC_MMW_VERSION5_UNION */
#define PMIC_MMW_VERSION5_ADDR(base)                              ((base) + (0x005))

/* 寄存器说明：状态标志寄存器0。
   位域定义UNION结构:  PMIC_MMW_STATUS0_UNION */
#define PMIC_MMW_STATUS0_ADDR(base)                               ((base) + (0x006))

/* 寄存器说明：状态标志寄存器1。
   位域定义UNION结构:  PMIC_MMW_STATUS1_UNION */
#define PMIC_MMW_STATUS1_ADDR(base)                               ((base) + (0x007))

/* 寄存器说明：LDO0开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO0_ONOFF_UNION */
#define PMIC_MMW_LDO0_ONOFF_ADDR(base)                            ((base) + (0x00A))

/* 寄存器说明：LDO1开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO1_ONOFF_UNION */
#define PMIC_MMW_LDO1_ONOFF_ADDR(base)                            ((base) + (0x00B))

/* 寄存器说明：LDO2开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO2_ONOFF_UNION */
#define PMIC_MMW_LDO2_ONOFF_ADDR(base)                            ((base) + (0x00C))

/* 寄存器说明：LDO3开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO3_ONOFF_UNION */
#define PMIC_MMW_LDO3_ONOFF_ADDR(base)                            ((base) + (0x00D))

/* 寄存器说明：LDO4开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO4_ONOFF_UNION */
#define PMIC_MMW_LDO4_ONOFF_ADDR(base)                            ((base) + (0x00E))

/* 寄存器说明：LDO5开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO5_ONOFF_UNION */
#define PMIC_MMW_LDO5_ONOFF_ADDR(base)                            ((base) + (0x00F))

/* 寄存器说明：LDO6开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO6_ONOFF_UNION */
#define PMIC_MMW_LDO6_ONOFF_ADDR(base)                            ((base) + (0x010))

/* 寄存器说明：LDO7开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO7_ONOFF_ECO_UNION */
#define PMIC_MMW_LDO7_ONOFF_ECO_ADDR(base)                        ((base) + (0x011))

/* 寄存器说明：LDO8开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO8_ONOFF_ECO_UNION */
#define PMIC_MMW_LDO8_ONOFF_ECO_ADDR(base)                        ((base) + (0x012))

/* 寄存器说明：LDO_CORE开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_ONOFF_ECO_UNION */
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_ADDR(base)                    ((base) + (0x013))

/* 寄存器说明：LDO_BUF_RF开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF_RF_ONOFF_UNION */
#define PMIC_MMW_LDO_BUF_RF_ONOFF_ADDR(base)                      ((base) + (0x014))

/* 寄存器说明：LDO_BUF0开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF0_ONOFF_UNION */
#define PMIC_MMW_LDO_BUF0_ONOFF_ADDR(base)                        ((base) + (0x015))

/* 寄存器说明：LDO_BUF1开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF1_ONOFF_UNION */
#define PMIC_MMW_LDO_BUF1_ONOFF_ADDR(base)                        ((base) + (0x016))

/* 寄存器说明：LDO_RFBUF0开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF0_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF0_ONOFF_ADDR(base)                      ((base) + (0x017))

/* 寄存器说明：LDO_RFBUF1开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF1_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF1_ONOFF_ADDR(base)                      ((base) + (0x018))

/* 寄存器说明：LDO_RFBUF2开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF2_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF2_ONOFF_ADDR(base)                      ((base) + (0x019))

/* 寄存器说明：LDO_RFBUF3开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF3_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF3_ONOFF_ADDR(base)                      ((base) + (0x01A))

/* 寄存器说明：LDO_RFBUF4开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF4_ONOFF_UNION */
#define PMIC_MMW_LDO_RFBUF4_ONOFF_ADDR(base)                      ((base) + (0x01B))

/* 寄存器说明：PMUH开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_PMUH_ONOFF_UNION */
#define PMIC_MMW_PMUH_ONOFF_ADDR(base)                            ((base) + (0x01C))

/* 寄存器说明：XO_CORE开关使能寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_ONOFF_UNION */
#define PMIC_MMW_XO_CORE_ONOFF_ADDR(base)                         ((base) + (0x01D))

/* 寄存器说明：THSD开关和ECO使能寄存器。
   位域定义UNION结构:  PMIC_MMW_THSD_ONOFF_ECO_UNION */
#define PMIC_MMW_THSD_ONOFF_ECO_ADDR(base)                        ((base) + (0x01E))

/* 寄存器说明：UFS时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_UFS_EN_UNION */
#define PMIC_MMW_CLK_UFS_EN_ADDR(base)                            ((base) + (0x020))

/* 寄存器说明：RF0时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_EN_UNION */
#define PMIC_MMW_CLK_RF0_EN_ADDR(base)                            ((base) + (0x021))

/* 寄存器说明：RF1时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_EN_UNION */
#define PMIC_MMW_CLK_RF1_EN_ADDR(base)                            ((base) + (0x022))

/* 寄存器说明：RF2时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_EN_UNION */
#define PMIC_MMW_CLK_RF2_EN_ADDR(base)                            ((base) + (0x023))

/* 寄存器说明：RF3时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_EN_UNION */
#define PMIC_MMW_CLK_RF3_EN_ADDR(base)                            ((base) + (0x024))

/* 寄存器说明：RF4时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_EN_UNION */
#define PMIC_MMW_CLK_RF4_EN_ADDR(base)                            ((base) + (0x025))

/* 寄存器说明：SERDES时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_EN_UNION */
#define PMIC_MMW_CLK_SERDES_EN_ADDR(base)                         ((base) + (0x026))

/* 寄存器说明：WIFI0时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_EN_UNION */
#define PMIC_MMW_CLK_WIFI0_EN_ADDR(base)                          ((base) + (0x027))

/* 寄存器说明：WIFI1时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_EN_UNION */
#define PMIC_MMW_CLK_WIFI1_EN_ADDR(base)                          ((base) + (0x028))

/* 寄存器说明：NFC时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_EN_UNION */
#define PMIC_MMW_CLK_NFC_EN_ADDR(base)                            ((base) + (0x029))

/* 寄存器说明：SYS时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_EN_UNION */
#define PMIC_MMW_CLK_SYS_EN_ADDR(base)                            ((base) + (0x02A))

/* 寄存器说明：送给主PMU的时钟开关寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_EN_UNION */
#define PMIC_MMW_CLK_PMU_EN_ADDR(base)                            ((base) + (0x02B))

/* 寄存器说明：32kHz时钟SYS输出使能控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_UNION */
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_ADDR(base)                 ((base) + (0x02C))

/* 寄存器说明：LDO0调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO0_VSET_UNION */
#define PMIC_MMW_LDO0_VSET_ADDR(base)                             ((base) + (0x02D))

/* 寄存器说明：LDO1调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO1_VSET_UNION */
#define PMIC_MMW_LDO1_VSET_ADDR(base)                             ((base) + (0x02E))

/* 寄存器说明：LDO2调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO2_VSET_UNION */
#define PMIC_MMW_LDO2_VSET_ADDR(base)                             ((base) + (0x02F))

/* 寄存器说明：LDO3调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO3_VSET_UNION */
#define PMIC_MMW_LDO3_VSET_ADDR(base)                             ((base) + (0x030))

/* 寄存器说明：LDO4调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO4_VSET_UNION */
#define PMIC_MMW_LDO4_VSET_ADDR(base)                             ((base) + (0x031))

/* 寄存器说明：LDO5调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO5_VSET_UNION */
#define PMIC_MMW_LDO5_VSET_ADDR(base)                             ((base) + (0x032))

/* 寄存器说明：LDO6调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO6_VSET_UNION */
#define PMIC_MMW_LDO6_VSET_ADDR(base)                             ((base) + (0x033))

/* 寄存器说明：LDO7调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO7_VSET_UNION */
#define PMIC_MMW_LDO7_VSET_ADDR(base)                             ((base) + (0x034))

/* 寄存器说明：LDO8调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO8_VSET_UNION */
#define PMIC_MMW_LDO8_VSET_ADDR(base)                             ((base) + (0x035))

/* 寄存器说明：LDO_CORE调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_VSET_UNION */
#define PMIC_MMW_LDO_CORE_VSET_ADDR(base)                         ((base) + (0x036))

/* 寄存器说明：LDO_CORE ECO调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_VSET_ECO_UNION */
#define PMIC_MMW_LDO_CORE_VSET_ECO_ADDR(base)                     ((base) + (0x037))

/* 寄存器说明：LDO_BUF_RF调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF_RF_VSET_UNION */
#define PMIC_MMW_LDO_BUF_RF_VSET_ADDR(base)                       ((base) + (0x038))

/* 寄存器说明：LDO_BUF0调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF0_VSET_UNION */
#define PMIC_MMW_LDO_BUF0_VSET_ADDR(base)                         ((base) + (0x039))

/* 寄存器说明：LDO_BUF1调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_BUF1_VSET_UNION */
#define PMIC_MMW_LDO_BUF1_VSET_ADDR(base)                         ((base) + (0x03A))

/* 寄存器说明：LDO_RFBUF0调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF0_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF0_VSET_ADDR(base)                       ((base) + (0x03B))

/* 寄存器说明：LDO_RFBUF1调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF1_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF1_VSET_ADDR(base)                       ((base) + (0x03C))

/* 寄存器说明：LDO_RFBUF2调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF2_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF2_VSET_ADDR(base)                       ((base) + (0x03D))

/* 寄存器说明：LDO_RFBUF3调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF3_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF3_VSET_ADDR(base)                       ((base) + (0x03E))

/* 寄存器说明：LDO_RFBUF4调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF4_VSET_UNION */
#define PMIC_MMW_LDO_RFBUF4_VSET_ADDR(base)                       ((base) + (0x03F))

/* 寄存器说明：LDO_PMUH调压寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_PMUH_VSET_UNION */
#define PMIC_MMW_LDO_PMUH_VSET_ADDR(base)                         ((base) + (0x040))

/* 寄存器说明：XO_CORE电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_CURR_UNION */
#define PMIC_MMW_XO_CORE_CURR_ADDR(base)                          ((base) + (0x041))

/* 寄存器说明：XO_CORE ECO电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_CURR_ECO_UNION */
#define PMIC_MMW_XO_CORE_CURR_ECO_ADDR(base)                      ((base) + (0x042))

/* 寄存器说明：XO_INBUFF电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_INBUFF_CURR_UNION */
#define PMIC_MMW_XO_INBUFF_CURR_ADDR(base)                        ((base) + (0x043))

/* 寄存器说明：XO_INBUFF ECO电流调节寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_INBUFF_CURR_ECO_UNION */
#define PMIC_MMW_XO_INBUFF_CURR_ECO_ADDR(base)                    ((base) + (0x044))

/* 寄存器说明：LDO0_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO0_CFG_REG_0_UNION */
#define PMIC_MMW_LDO0_CFG_REG_0_ADDR(base)                        ((base) + (0x045))

/* 寄存器说明：LDO0_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO0_CFG_REG_1_UNION */
#define PMIC_MMW_LDO0_CFG_REG_1_ADDR(base)                        ((base) + (0x046))

/* 寄存器说明：LDO0_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO0_CFG_REG_2_UNION */
#define PMIC_MMW_LDO0_CFG_REG_2_ADDR(base)                        ((base) + (0x047))

/* 寄存器说明：LDO1_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO1_CFG_REG_0_UNION */
#define PMIC_MMW_LDO1_CFG_REG_0_ADDR(base)                        ((base) + (0x048))

/* 寄存器说明：LDO1_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO1_CFG_REG_1_UNION */
#define PMIC_MMW_LDO1_CFG_REG_1_ADDR(base)                        ((base) + (0x049))

/* 寄存器说明：LDO1_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO1_CFG_REG_2_UNION */
#define PMIC_MMW_LDO1_CFG_REG_2_ADDR(base)                        ((base) + (0x04A))

/* 寄存器说明：LDO2_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO2_CFG_REG_0_UNION */
#define PMIC_MMW_LDO2_CFG_REG_0_ADDR(base)                        ((base) + (0x04B))

/* 寄存器说明：LDO2_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO2_CFG_REG_1_UNION */
#define PMIC_MMW_LDO2_CFG_REG_1_ADDR(base)                        ((base) + (0x04C))

/* 寄存器说明：LDO2_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO2_CFG_REG_2_UNION */
#define PMIC_MMW_LDO2_CFG_REG_2_ADDR(base)                        ((base) + (0x04D))

/* 寄存器说明：LDO3_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO3_CFG_REG_0_UNION */
#define PMIC_MMW_LDO3_CFG_REG_0_ADDR(base)                        ((base) + (0x04E))

/* 寄存器说明：LDO3_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO3_CFG_REG_1_UNION */
#define PMIC_MMW_LDO3_CFG_REG_1_ADDR(base)                        ((base) + (0x04F))

/* 寄存器说明：LDO3_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO3_CFG_REG_2_UNION */
#define PMIC_MMW_LDO3_CFG_REG_2_ADDR(base)                        ((base) + (0x050))

/* 寄存器说明：LDO4_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO4_CFG_REG_0_UNION */
#define PMIC_MMW_LDO4_CFG_REG_0_ADDR(base)                        ((base) + (0x051))

/* 寄存器说明：LDO4_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO4_CFG_REG_1_UNION */
#define PMIC_MMW_LDO4_CFG_REG_1_ADDR(base)                        ((base) + (0x052))

/* 寄存器说明：LDO4_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO4_CFG_REG_2_UNION */
#define PMIC_MMW_LDO4_CFG_REG_2_ADDR(base)                        ((base) + (0x053))

/* 寄存器说明：LDO5_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO5_CFG_REG_0_UNION */
#define PMIC_MMW_LDO5_CFG_REG_0_ADDR(base)                        ((base) + (0x054))

/* 寄存器说明：LDO5_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO5_CFG_REG_1_UNION */
#define PMIC_MMW_LDO5_CFG_REG_1_ADDR(base)                        ((base) + (0x055))

/* 寄存器说明：LDO6_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO6_CFG_REG_0_UNION */
#define PMIC_MMW_LDO6_CFG_REG_0_ADDR(base)                        ((base) + (0x056))

/* 寄存器说明：LDO6_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO6_CFG_REG_1_UNION */
#define PMIC_MMW_LDO6_CFG_REG_1_ADDR(base)                        ((base) + (0x057))

/* 寄存器说明：LDO7_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO7_CFG_REG_0_UNION */
#define PMIC_MMW_LDO7_CFG_REG_0_ADDR(base)                        ((base) + (0x058))

/* 寄存器说明：LDO7_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO7_CFG_REG_1_UNION */
#define PMIC_MMW_LDO7_CFG_REG_1_ADDR(base)                        ((base) + (0x059))

/* 寄存器说明：LDO8_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO8_CFG_REG_0_UNION */
#define PMIC_MMW_LDO8_CFG_REG_0_ADDR(base)                        ((base) + (0x05A))

/* 寄存器说明：LDO8_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO8_CFG_REG_1_UNION */
#define PMIC_MMW_LDO8_CFG_REG_1_ADDR(base)                        ((base) + (0x05B))

/* 寄存器说明：LDO_CORE_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_CORE_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ADDR(base)                    ((base) + (0x05C))

/* 寄存器说明：LDO_BUF0_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_BUF0_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ADDR(base)                    ((base) + (0x05D))

/* 寄存器说明：LDO_BUF1_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_BUF1_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ADDR(base)                    ((base) + (0x05E))

/* 寄存器说明：LDO_BUF_RF_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_BUF_RF_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ADDR(base)                  ((base) + (0x05F))

/* 寄存器说明：LDO_RFBUF0_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF0_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ADDR(base)                  ((base) + (0x060))

/* 寄存器说明：LDO_RFBUF1_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF1_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ADDR(base)                  ((base) + (0x061))

/* 寄存器说明：LDO_RFBUF2_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF2_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ADDR(base)                  ((base) + (0x062))

/* 寄存器说明：LDO_RFBUF3_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF3_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ADDR(base)                  ((base) + (0x063))

/* 寄存器说明：LDO_RFBUF4_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RFBUF4_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ADDR(base)                  ((base) + (0x064))

/* 寄存器说明：PMUH_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_PMUH_CFG_REG_0_UNION */
#define PMIC_MMW_PMUH_CFG_REG_0_ADDR(base)                        ((base) + (0x065))

/* 寄存器说明：LDO_RESERVE_配置寄存器_0
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_0_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ADDR(base)                 ((base) + (0x066))

/* 寄存器说明：LDO_RESERVE_配置寄存器_1
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_1_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ADDR(base)                 ((base) + (0x067))

/* 寄存器说明：LDO_RESERVE_配置寄存器_2
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_2_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ADDR(base)                 ((base) + (0x068))

/* 寄存器说明：LDO_RESERVE_配置寄存器_3
   位域定义UNION结构:  PMIC_MMW_LDO_RESERVE_CFG_REG_3_UNION */
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ADDR(base)                 ((base) + (0x069))

/* 寄存器说明：UFS时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_UFS_CTRL_UNION */
#define PMIC_MMW_CLK_UFS_CTRL_ADDR(base)                          ((base) + (0x06A))

/* 寄存器说明：RF0时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_CTRL_UNION */
#define PMIC_MMW_CLK_RF0_CTRL_ADDR(base)                          ((base) + (0x06B))

/* 寄存器说明：RF1时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_CTRL_UNION */
#define PMIC_MMW_CLK_RF1_CTRL_ADDR(base)                          ((base) + (0x06C))

/* 寄存器说明：RF2时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_CTRL_UNION */
#define PMIC_MMW_CLK_RF2_CTRL_ADDR(base)                          ((base) + (0x06D))

/* 寄存器说明：RF3时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_CTRL_UNION */
#define PMIC_MMW_CLK_RF3_CTRL_ADDR(base)                          ((base) + (0x06E))

/* 寄存器说明：RF4时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_CTRL_UNION */
#define PMIC_MMW_CLK_RF4_CTRL_ADDR(base)                          ((base) + (0x06F))

/* 寄存器说明：SERDESB时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_CTRL_UNION */
#define PMIC_MMW_CLK_SERDES_CTRL_ADDR(base)                       ((base) + (0x070))

/* 寄存器说明：WIFI0时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI0_CTRL_ADDR(base)                        ((base) + (0x071))

/* 寄存器说明：WIFI1时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI1_CTRL_ADDR(base)                        ((base) + (0x072))

/* 寄存器说明：NFC时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_CTRL_UNION */
#define PMIC_MMW_CLK_NFC_CTRL_ADDR(base)                          ((base) + (0x073))

/* 寄存器说明：SYS时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_CTRL_UNION */
#define PMIC_MMW_CLK_SYS_CTRL_ADDR(base)                          ((base) + (0x074))

/* 寄存器说明：送给主PMU的时钟驱动能力配置寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_CTRL_UNION */
#define PMIC_MMW_CLK_PMU_CTRL_ADDR(base)                          ((base) + (0x075))

/* 寄存器说明：XO时钟控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_XO_CTRL0_UNION */
#define PMIC_MMW_XO_CTRL0_ADDR(base)                              ((base) + (0x076))

/* 寄存器说明：XO预留寄存器0。
   位域定义UNION结构:  PMIC_MMW_XO_RESERVE_0_UNION */
#define PMIC_MMW_XO_RESERVE_0_ADDR(base)                          ((base) + (0x077))

/* 寄存器说明：XO corner检测状态指示寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_DET_STATUS_UNION */
#define PMIC_MMW_XO_DET_STATUS_ADDR(base)                         ((base) + (0x078))

/* 寄存器说明：基准控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_BG_THSD_CTRL0_UNION */
#define PMIC_MMW_BG_THSD_CTRL0_ADDR(base)                         ((base) + (0x079))

/* 寄存器说明：基准控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_BG_THSD_CTRL1_UNION */
#define PMIC_MMW_BG_THSD_CTRL1_ADDR(base)                         ((base) + (0x07A))

/* 寄存器说明：基准预留寄存器0。
   位域定义UNION结构:  PMIC_MMW_BG_RESERVE_0_UNION */
#define PMIC_MMW_BG_RESERVE_0_ADDR(base)                          ((base) + (0x07B))

/* 寄存器说明：基准预留寄存器1。
   位域定义UNION结构:  PMIC_MMW_BG_RESERVE_1_UNION */
#define PMIC_MMW_BG_RESERVE_1_ADDR(base)                          ((base) + (0x07C))

/* 寄存器说明：基准预留寄存器2。
   位域定义UNION结构:  PMIC_MMW_BG_RESERVE_2_UNION */
#define PMIC_MMW_BG_RESERVE_2_ADDR(base)                          ((base) + (0x07D))

/* 寄存器说明：系统控制预留寄存器。
   位域定义UNION结构:  PMIC_MMW_SYS_CTRL_RESERVE_UNION */
#define PMIC_MMW_SYS_CTRL_RESERVE_ADDR(base)                      ((base) + (0x07E))

/* 寄存器说明：系统控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_SYS_CTRL0_UNION */
#define PMIC_MMW_SYS_CTRL0_ADDR(base)                             ((base) + (0x07F))

/* 寄存器说明：系统控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_SYS_CTRL1_UNION */
#define PMIC_MMW_SYS_CTRL1_ADDR(base)                             ((base) + (0x080))

/* 寄存器说明：模拟到数字的保留寄存器0。
   位域定义UNION结构:  PMIC_MMW_A2D_RES0_UNION */
#define PMIC_MMW_A2D_RES0_ADDR(base)                              ((base) + (0x084))

/* 寄存器说明：数字到模拟的保留寄存器0。
   位域定义UNION结构:  PMIC_MMW_D2A_RES0_UNION */
#define PMIC_MMW_D2A_RES0_ADDR(base)                              ((base) + (0x085))

/* 寄存器说明：数字到模拟的保留寄存器1。
   位域定义UNION结构:  PMIC_MMW_D2A_RES1_UNION */
#define PMIC_MMW_D2A_RES1_ADDR(base)                              ((base) + (0x086))

/* 寄存器说明：数字到模拟的保留寄存器2。
   位域定义UNION结构:  PMIC_MMW_D2A_RES2_UNION */
#define PMIC_MMW_D2A_RES2_ADDR(base)                              ((base) + (0x087))

/* 寄存器说明：数字到模拟的保留寄存器3。
   位域定义UNION结构:  PMIC_MMW_D2A_RES3_UNION */
#define PMIC_MMW_D2A_RES3_ADDR(base)                              ((base) + (0x088))

/* 寄存器说明：OTP回读寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_0_UNION */
#define PMIC_MMW_OTP_0_ADDR(base)                                 ((base) + (0x089))

/* 寄存器说明：OTP回读寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_1_UNION */
#define PMIC_MMW_OTP_1_ADDR(base)                                 ((base) + (0x08A))

/* 寄存器说明：手动控制读写操作中OTP时钟产生控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OTP_CLK_CTRL_UNION */
#define PMIC_MMW_OTP_CLK_CTRL_ADDR(base)                          ((base) + (0x08B))

/* 寄存器说明：OTP控制信号寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_CTRL0_UNION */
#define PMIC_MMW_OTP_CTRL0_ADDR(base)                             ((base) + (0x08C))

/* 寄存器说明：OTP控制信号寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_CTRL1_UNION */
#define PMIC_MMW_OTP_CTRL1_ADDR(base)                             ((base) + (0x08D))

/* 寄存器说明：OTP控制信号寄存器2。
   位域定义UNION结构:  PMIC_MMW_OTP_CTRL2_UNION */
#define PMIC_MMW_OTP_CTRL2_ADDR(base)                             ((base) + (0x08E))

/* 寄存器说明：OTP写操作数据寄存器。
   位域定义UNION结构:  PMIC_MMW_OTP_WDATA_UNION */
#define PMIC_MMW_OTP_WDATA_ADDR(base)                             ((base) + (0x08F))

/* 寄存器说明：OTP预修调寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_0_W_UNION */
#define PMIC_MMW_OTP_0_W_ADDR(base)                               ((base) + (0x090))

/* 寄存器说明：OTP预修调寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_1_W_UNION */
#define PMIC_MMW_OTP_1_W_ADDR(base)                               ((base) + (0x091))

/* 寄存器说明：OTP预修调寄存器2。
   位域定义UNION结构:  PMIC_MMW_OTP_2_W_UNION */
#define PMIC_MMW_OTP_2_W_ADDR(base)                               ((base) + (0x092))

/* 寄存器说明：OTP预修调寄存器3。
   位域定义UNION结构:  PMIC_MMW_OTP_3_W_UNION */
#define PMIC_MMW_OTP_3_W_ADDR(base)                               ((base) + (0x093))

/* 寄存器说明：OTP预修调寄存器4。
   位域定义UNION结构:  PMIC_MMW_OTP_4_W_UNION */
#define PMIC_MMW_OTP_4_W_ADDR(base)                               ((base) + (0x094))

/* 寄存器说明：OTP预修调寄存器5。
   位域定义UNION结构:  PMIC_MMW_OTP_5_W_UNION */
#define PMIC_MMW_OTP_5_W_ADDR(base)                               ((base) + (0x095))

/* 寄存器说明：OTP0预修调寄存器6。
   位域定义UNION结构:  PMIC_MMW_OTP_6_W_UNION */
#define PMIC_MMW_OTP_6_W_ADDR(base)                               ((base) + (0x096))

/* 寄存器说明：OTP预修调寄存器7。
   位域定义UNION结构:  PMIC_MMW_OTP_7_W_UNION */
#define PMIC_MMW_OTP_7_W_ADDR(base)                               ((base) + (0x097))

/* 寄存器说明：OTP预修调寄存器8。
   位域定义UNION结构:  PMIC_MMW_OTP_8_W_UNION */
#define PMIC_MMW_OTP_8_W_ADDR(base)                               ((base) + (0x098))

/* 寄存器说明：OTP预修调寄存器9。
   位域定义UNION结构:  PMIC_MMW_OTP_9_W_UNION */
#define PMIC_MMW_OTP_9_W_ADDR(base)                               ((base) + (0x099))

/* 寄存器说明：OTP预修调寄存器10。
   位域定义UNION结构:  PMIC_MMW_OTP_10_W_UNION */
#define PMIC_MMW_OTP_10_W_ADDR(base)                              ((base) + (0x09A))

/* 寄存器说明：OTP预修调寄存器11。
   位域定义UNION结构:  PMIC_MMW_OTP_11_W_UNION */
#define PMIC_MMW_OTP_11_W_ADDR(base)                              ((base) + (0x09B))

/* 寄存器说明：OTP预修调寄存器12。
   位域定义UNION结构:  PMIC_MMW_OTP_12_W_UNION */
#define PMIC_MMW_OTP_12_W_ADDR(base)                              ((base) + (0x09C))

/* 寄存器说明：OTP预修调寄存器13。
   位域定义UNION结构:  PMIC_MMW_OTP_13_W_UNION */
#define PMIC_MMW_OTP_13_W_ADDR(base)                              ((base) + (0x09D))

/* 寄存器说明：OTP预修调寄存器14。
   位域定义UNION结构:  PMIC_MMW_OTP_14_W_UNION */
#define PMIC_MMW_OTP_14_W_ADDR(base)                              ((base) + (0x09E))

/* 寄存器说明：OTP预修调寄存器15。
   位域定义UNION结构:  PMIC_MMW_OTP_15_W_UNION */
#define PMIC_MMW_OTP_15_W_ADDR(base)                              ((base) + (0x09F))

/* 寄存器说明：OTP预修调寄存器16。
   位域定义UNION结构:  PMIC_MMW_OTP_16_W_UNION */
#define PMIC_MMW_OTP_16_W_ADDR(base)                              ((base) + (0x0A0))

/* 寄存器说明：OTP预修调寄存器17。
   位域定义UNION结构:  PMIC_MMW_OTP_17_W_UNION */
#define PMIC_MMW_OTP_17_W_ADDR(base)                              ((base) + (0x0A1))

/* 寄存器说明：OTP预修调寄存器18。
   位域定义UNION结构:  PMIC_MMW_OTP_18_W_UNION */
#define PMIC_MMW_OTP_18_W_ADDR(base)                              ((base) + (0x0A2))

/* 寄存器说明：OTP预修调寄存器19。
   位域定义UNION结构:  PMIC_MMW_OTP_19_W_UNION */
#define PMIC_MMW_OTP_19_W_ADDR(base)                              ((base) + (0x0A3))

/* 寄存器说明：OTP预修调寄存器20。
   位域定义UNION结构:  PMIC_MMW_OTP_20_W_UNION */
#define PMIC_MMW_OTP_20_W_ADDR(base)                              ((base) + (0x0A4))

/* 寄存器说明：OTP预修调寄存器21。
   位域定义UNION结构:  PMIC_MMW_OTP_21_W_UNION */
#define PMIC_MMW_OTP_21_W_ADDR(base)                              ((base) + (0x0A5))

/* 寄存器说明：LDO0/1/2/3 OCP自动关断控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO0_3_OCP_CTRL_UNION */
#define PMIC_MMW_LDO0_3_OCP_CTRL_ADDR(base)                       ((base) + (0x0A6))

/* 寄存器说明：LCO4/5/6/7 OCP自动关断控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO4_7_OCP_CTRL_UNION */
#define PMIC_MMW_LDO4_7_OCP_CTRL_ADDR(base)                       ((base) + (0x0A7))

/* 寄存器说明：LDO8/LDO_CORE/LDO_BUF0/LDO_BUF_RF OCP自动关断控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO8_12_OCP_CTRL_UNION */
#define PMIC_MMW_LDO8_12_OCP_CTRL_ADDR(base)                      ((base) + (0x0A8))

/* 寄存器说明：OCP滤波时间控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OCP_DEB_CTRL0_UNION */
#define PMIC_MMW_OCP_DEB_CTRL0_ADDR(base)                         ((base) + (0x0A9))

/* 寄存器说明：OCP滤波时间控制寄存器。
   位域定义UNION结构:  PMIC_MMW_OCP_DEB_CTRL1_UNION */
#define PMIC_MMW_OCP_DEB_CTRL1_ADDR(base)                         ((base) + (0x0AA))

/* 寄存器说明：过温140度关机滤波时间选择寄存器。
   位域定义UNION结构:  PMIC_MMW_THSD_140_DEB_CTRL_UNION */
#define PMIC_MMW_THSD_140_DEB_CTRL_ADDR(base)                     ((base) + (0x0AB))

/* 寄存器说明：OCP和DOCP滤波开关寄存器。
   位域定义UNION结构:  PMIC_MMW_OCP_SCP_ONOFF_UNION */
#define PMIC_MMW_OCP_SCP_ONOFF_ADDR(base)                         ((base) + (0x0AC))

/* 寄存器说明：硬线屏蔽寄存器0。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL0_UNION */
#define PMIC_MMW_HARDWIRE_CTRL0_ADDR(base)                        ((base) + (0x0AD))

/* 寄存器说明：硬线屏蔽寄存器1。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL1_UNION */
#define PMIC_MMW_HARDWIRE_CTRL1_ADDR(base)                        ((base) + (0x0AE))

/* 寄存器说明：硬线屏蔽寄存器2。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL2_UNION */
#define PMIC_MMW_HARDWIRE_CTRL2_ADDR(base)                        ((base) + (0x0AF))

/* 寄存器说明：硬线屏蔽寄存器3。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL3_UNION */
#define PMIC_MMW_HARDWIRE_CTRL3_ADDR(base)                        ((base) + (0x0B0))

/* 寄存器说明：硬线屏蔽寄存器4。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL4_UNION */
#define PMIC_MMW_HARDWIRE_CTRL4_ADDR(base)                        ((base) + (0x0B1))

/* 寄存器说明：硬线屏蔽寄存器5。
   位域定义UNION结构:  PMIC_MMW_HARDWIRE_CTRL5_UNION */
#define PMIC_MMW_HARDWIRE_CTRL5_ADDR(base)                        ((base) + (0x0B2))

/* 寄存器说明：下电区写屏蔽控制寄存器。
   位域定义UNION结构:  PMIC_MMW_LOCK_UNION */
#define PMIC_MMW_LOCK_ADDR(base)                                  ((base) + (0x0B3))

/* 寄存器说明：软件在eco模式下对SPMI有读写需求时，提供SPMI时钟以及同步用的19.2M ssi时钟(该寄存器时钟为常开时钟)
   位域定义UNION结构:  PMIC_MMW_SPMI_ECO_GT_BYPASS_UNION */
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_ADDR(base)                    ((base) + (0x0B4))

/* 寄存器说明：LDO5和LDO6的缓启完成时间配置寄存器。
   位域定义UNION结构:  PMIC_MMW_LDO_SST_END_CFG_UNION */
#define PMIC_MMW_LDO_SST_END_CFG_ADDR(base)                       ((base) + (0x0B5))

/* 寄存器说明：非debug模式，配置reg_xo_rf0_en~reg_xo_rf4_en开启时，对应的电源和时钟使能开启时间配置寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_UNION */
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_ADDR(base)                 ((base) + (0x0B6))

/* 寄存器说明：ldo_rfbuf_en、ldo_rfbuf0~4_en、xo_rf0~4_en开启和关闭debug模式控制寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_RF_CLK_ON_DEBUG_UNION */
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_ADDR(base)                    ((base) + (0x0B7))

/* 寄存器说明：XO CORE电流debug模式配置寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_CORE_CURR_DEBUG_UNION */
#define PMIC_MMW_XO_CORE_CURR_DEBUG_ADDR(base)                    ((base) + (0x0B8))

/* 寄存器说明：XO INPUT BUFFER电流debug模式配置寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_INBUFF_CURR_DEBUG_UNION */
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_ADDR(base)                  ((base) + (0x0B9))

/* 寄存器说明：SPMI debug寄存器0。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG0_UNION */
#define PMIC_MMW_SPMI_DEBUG0_ADDR(base)                           ((base) + (0x0F0))

/* 寄存器说明：SPMI debug寄存器1。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG1_UNION */
#define PMIC_MMW_SPMI_DEBUG1_ADDR(base)                           ((base) + (0x0F1))

/* 寄存器说明：SPMI debug寄存器2。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG2_UNION */
#define PMIC_MMW_SPMI_DEBUG2_ADDR(base)                           ((base) + (0x0F2))

/* 寄存器说明：SPMI debug寄存器3。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG3_UNION */
#define PMIC_MMW_SPMI_DEBUG3_ADDR(base)                           ((base) + (0x0F3))

/* 寄存器说明：SPMI debug寄存器4。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG4_UNION */
#define PMIC_MMW_SPMI_DEBUG4_ADDR(base)                           ((base) + (0x0F4))

/* 寄存器说明：SPMI debug寄存器5。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG5_UNION */
#define PMIC_MMW_SPMI_DEBUG5_ADDR(base)                           ((base) + (0x0F5))

/* 寄存器说明：SPMI debug寄存器6。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG6_UNION */
#define PMIC_MMW_SPMI_DEBUG6_ADDR(base)                           ((base) + (0x0F6))

/* 寄存器说明：SPMI debug寄存器7。
   位域定义UNION结构:  PMIC_MMW_SPMI_DEBUG7_UNION */
#define PMIC_MMW_SPMI_DEBUG7_ADDR(base)                           ((base) + (0x0F7))


#endif


/****************************************************************************
                     (2/5) PMU_IRQ_MASK
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：IRQ_MASK0控制寄存器。
   位域定义UNION结构:  PMIC_MMW_IRQ_MASK_0_UNION */
#define PMIC_MMW_IRQ_MASK_0_ADDR(base)                            ((base) + (0x100UL))

/* 寄存器说明：IRQ_MASK1控制寄存器。
   位域定义UNION结构:  PMIC_MMW_IRQ_MASK_1_UNION */
#define PMIC_MMW_IRQ_MASK_1_ADDR(base)                            ((base) + (0x101UL))

/* 寄存器说明：IRQ_MASK2控制寄存器。
   位域定义UNION结构:  PMIC_MMW_IRQ_MASK_2_UNION */
#define PMIC_MMW_IRQ_MASK_2_ADDR(base)                            ((base) + (0x102UL))

/* 寄存器说明：FAULT_N屏蔽控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_0_UNION */
#define PMIC_MMW_FAULT_N_MASK_0_ADDR(base)                        ((base) + (0x103UL))

/* 寄存器说明：FAULT_N屏蔽控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_1_UNION */
#define PMIC_MMW_FAULT_N_MASK_1_ADDR(base)                        ((base) + (0x104UL))

/* 寄存器说明：FAULT_N屏蔽控制寄存器2。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_2_UNION */
#define PMIC_MMW_FAULT_N_MASK_2_ADDR(base)                        ((base) + (0x105UL))

/* 寄存器说明：FAULT_N屏蔽控制寄存器3。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_3_UNION */
#define PMIC_MMW_FAULT_N_MASK_3_ADDR(base)                        ((base) + (0x106UL))


#else


/* 寄存器说明：IRQ_MASK0控制寄存器。
   位域定义UNION结构:  PMIC_MMW_IRQ_MASK_0_UNION */
#define PMIC_MMW_IRQ_MASK_0_ADDR(base)                            ((base) + (0x100))

/* 寄存器说明：IRQ_MASK1控制寄存器。
   位域定义UNION结构:  PMIC_MMW_IRQ_MASK_1_UNION */
#define PMIC_MMW_IRQ_MASK_1_ADDR(base)                            ((base) + (0x101))

/* 寄存器说明：IRQ_MASK2控制寄存器。
   位域定义UNION结构:  PMIC_MMW_IRQ_MASK_2_UNION */
#define PMIC_MMW_IRQ_MASK_2_ADDR(base)                            ((base) + (0x102))

/* 寄存器说明：FAULT_N屏蔽控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_0_UNION */
#define PMIC_MMW_FAULT_N_MASK_0_ADDR(base)                        ((base) + (0x103))

/* 寄存器说明：FAULT_N屏蔽控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_1_UNION */
#define PMIC_MMW_FAULT_N_MASK_1_ADDR(base)                        ((base) + (0x104))

/* 寄存器说明：FAULT_N屏蔽控制寄存器2。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_2_UNION */
#define PMIC_MMW_FAULT_N_MASK_2_ADDR(base)                        ((base) + (0x105))

/* 寄存器说明：FAULT_N屏蔽控制寄存器3。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_MASK_3_UNION */
#define PMIC_MMW_FAULT_N_MASK_3_ADDR(base)                        ((base) + (0x106))


#endif


/****************************************************************************
                     (3/5) PMU_IRQ
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：中断寄存器0。
   位域定义UNION结构:  PMIC_MMW_IRQ0_UNION */
#define PMIC_MMW_IRQ0_ADDR(base)                                  ((base) + (0x10AUL))

/* 寄存器说明：OCP中断寄存器0。
   位域定义UNION结构:  PMIC_MMW_OCP_IRQ0_UNION */
#define PMIC_MMW_OCP_IRQ0_ADDR(base)                              ((base) + (0x10BUL))

/* 寄存器说明：OCP中断寄存器1。
   位域定义UNION结构:  PMIC_MMW_OCP_IRQ1_UNION */
#define PMIC_MMW_OCP_IRQ1_ADDR(base)                              ((base) + (0x10CUL))

/* 寄存器说明：FAULT_N上报寄存器0。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_0_UNION */
#define PMIC_MMW_FAULT_N_0_ADDR(base)                             ((base) + (0x10DUL))

/* 寄存器说明：FAULT_N上报寄存器1。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_1_UNION */
#define PMIC_MMW_FAULT_N_1_ADDR(base)                             ((base) + (0x10EUL))

/* 寄存器说明：FAULT_N上报寄存器2。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_2_UNION */
#define PMIC_MMW_FAULT_N_2_ADDR(base)                             ((base) + (0x10FUL))

/* 寄存器说明：FAULT_N上报寄存器3。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_3_UNION */
#define PMIC_MMW_FAULT_N_3_ADDR(base)                             ((base) + (0x110UL))


#else


/* 寄存器说明：中断寄存器0。
   位域定义UNION结构:  PMIC_MMW_IRQ0_UNION */
#define PMIC_MMW_IRQ0_ADDR(base)                                  ((base) + (0x10A))

/* 寄存器说明：OCP中断寄存器0。
   位域定义UNION结构:  PMIC_MMW_OCP_IRQ0_UNION */
#define PMIC_MMW_OCP_IRQ0_ADDR(base)                              ((base) + (0x10B))

/* 寄存器说明：OCP中断寄存器1。
   位域定义UNION结构:  PMIC_MMW_OCP_IRQ1_UNION */
#define PMIC_MMW_OCP_IRQ1_ADDR(base)                              ((base) + (0x10C))

/* 寄存器说明：FAULT_N上报寄存器0。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_0_UNION */
#define PMIC_MMW_FAULT_N_0_ADDR(base)                             ((base) + (0x10D))

/* 寄存器说明：FAULT_N上报寄存器1。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_1_UNION */
#define PMIC_MMW_FAULT_N_1_ADDR(base)                             ((base) + (0x10E))

/* 寄存器说明：FAULT_N上报寄存器2。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_2_UNION */
#define PMIC_MMW_FAULT_N_2_ADDR(base)                             ((base) + (0x10F))

/* 寄存器说明：FAULT_N上报寄存器3。
   位域定义UNION结构:  PMIC_MMW_FAULT_N_3_UNION */
#define PMIC_MMW_FAULT_N_3_ADDR(base)                             ((base) + (0x110))


#endif


/****************************************************************************
                     (4/5) NP_PMU_EVENT
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：非下电事件记录寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD0_UNION */
#define PMIC_MMW_NP_RECORD0_ADDR(base)                            ((base) + (0x114UL))

/* 寄存器说明：非下电事件记录寄存器1。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD1_UNION */
#define PMIC_MMW_NP_RECORD1_ADDR(base)                            ((base) + (0x115UL))

/* 寄存器说明：非下电事件记录寄存器2。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD2_UNION */
#define PMIC_MMW_NP_RECORD2_ADDR(base)                            ((base) + (0x116UL))

/* 寄存器说明：非下电事件记录寄存器3。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD3_UNION */
#define PMIC_MMW_NP_RECORD3_ADDR(base)                            ((base) + (0x117UL))

/* 寄存器说明：非下电事件记录寄存器4。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD4_UNION */
#define PMIC_MMW_NP_RECORD4_ADDR(base)                            ((base) + (0x118UL))

/* 寄存器说明：非下电事件记录寄存器5。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD5_UNION */
#define PMIC_MMW_NP_RECORD5_ADDR(base)                            ((base) + (0x119UL))


#else


/* 寄存器说明：非下电事件记录寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD0_UNION */
#define PMIC_MMW_NP_RECORD0_ADDR(base)                            ((base) + (0x114))

/* 寄存器说明：非下电事件记录寄存器1。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD1_UNION */
#define PMIC_MMW_NP_RECORD1_ADDR(base)                            ((base) + (0x115))

/* 寄存器说明：非下电事件记录寄存器2。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD2_UNION */
#define PMIC_MMW_NP_RECORD2_ADDR(base)                            ((base) + (0x116))

/* 寄存器说明：非下电事件记录寄存器3。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD3_UNION */
#define PMIC_MMW_NP_RECORD3_ADDR(base)                            ((base) + (0x117))

/* 寄存器说明：非下电事件记录寄存器4。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD4_UNION */
#define PMIC_MMW_NP_RECORD4_ADDR(base)                            ((base) + (0x118))

/* 寄存器说明：非下电事件记录寄存器5。
   位域定义UNION结构:  PMIC_MMW_NP_RECORD5_UNION */
#define PMIC_MMW_NP_RECORD5_ADDR(base)                            ((base) + (0x119))


#endif


/****************************************************************************
                     (5/5) NP_PMU_CTRL
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：CLK_RF0时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_ADDR(base)                     ((base) + (0x200UL))

/* 寄存器说明：CLK_RF1时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_ADDR(base)                     ((base) + (0x201UL))

/* 寄存器说明：CLK_RF2时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_ADDR(base)                     ((base) + (0x202UL))

/* 寄存器说明：CLK_RF3时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_ADDR(base)                     ((base) + (0x203UL))

/* 寄存器说明：CLK_RF4时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_ADDR(base)                     ((base) + (0x204UL))

/* 寄存器说明：CLK_SERDES时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_ADDR(base)                  ((base) + (0x205UL))

/* 寄存器说明：CLK_WIFI0时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_ADDR(base)                   ((base) + (0x206UL))

/* 寄存器说明：CLK_WIFI1时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_ADDR(base)                   ((base) + (0x207UL))

/* 寄存器说明：CLK_NFC时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_ADDR(base)                     ((base) + (0x208UL))

/* 寄存器说明：CLK_SYS时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_ADDR(base)                     ((base) + (0x209UL))

/* 寄存器说明：CLK_PMU时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_ADDR(base)                     ((base) + (0x20AUL))

/* 寄存器说明：CLK_RF0时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_ADDR(base)                     ((base) + (0x20BUL))

/* 寄存器说明：CLK_RF1时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_ADDR(base)                     ((base) + (0x20CUL))

/* 寄存器说明：CLK_RF2时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_ADDR(base)                     ((base) + (0x20DUL))

/* 寄存器说明：CLK_RF3时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_ADDR(base)                     ((base) + (0x20EUL))

/* 寄存器说明：CLK_RF4时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_ADDR(base)                     ((base) + (0x20FUL))

/* 寄存器说明：CLK_SERDES时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_ADDR(base)                  ((base) + (0x210UL))

/* 寄存器说明：CLK_WIFI0时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_ADDR(base)                   ((base) + (0x211UL))

/* 寄存器说明：CLK_WIFI1时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_ADDR(base)                   ((base) + (0x212UL))

/* 寄存器说明：CLK_NFC时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_ADDR(base)                     ((base) + (0x213UL))

/* 寄存器说明：CLK_SYS时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_ADDR(base)                     ((base) + (0x214UL))

/* 寄存器说明：CLK_PMU时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_ADDR(base)                     ((base) + (0x215UL))

/* 寄存器说明：CLK_UFS时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_UFS_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_ADDR(base)                     ((base) + (0x216UL))

/* 寄存器说明：SYS_CLK BUF所用电源选择寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_BUF_PWR_SEL_UNION */
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_ADDR(base)                   ((base) + (0x217UL))

/* 寄存器说明：EN_PMUD配置寄存器。
   位域定义UNION结构:  PMIC_MMW_EN_PMUD_UNION */
#define PMIC_MMW_EN_PMUD_ADDR(base)                               ((base) + (0x218UL))

/* 寄存器说明：PMUD调压寄存器。
   位域定义UNION结构:  PMIC_MMW_PMUD_VSET_UNION */
#define PMIC_MMW_PMUD_VSET_ADDR(base)                             ((base) + (0x219UL))

/* 寄存器说明：时钟控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_CLK_TOP_CTRL1_0_UNION */
#define PMIC_MMW_CLK_TOP_CTRL1_0_ADDR(base)                       ((base) + (0x21AUL))

/* 寄存器说明：时钟控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_CLK_TOP_CTRL1_1_UNION */
#define PMIC_MMW_CLK_TOP_CTRL1_1_ADDR(base)                       ((base) + (0x21BUL))

/* 寄存器说明：256K RC控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_CLK_256K_CTRL0_UNION */
#define PMIC_MMW_CLK_256K_CTRL0_ADDR(base)                        ((base) + (0x21CUL))

/* 寄存器说明：256K RC控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_CLK_256K_CTRL1_UNION */
#define PMIC_MMW_CLK_256K_CTRL1_ADDR(base)                        ((base) + (0x21DUL))

/* 寄存器说明：XO corner检测数据寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_DET_DATA_UNION */
#define PMIC_MMW_XO_DET_DATA_ADDR(base)                           ((base) + (0x21EUL))

/* 寄存器说明：IN端低压阈值调整寄存器。
   位域定义UNION结构:  PMIC_MMW_VSYS_LOW_SET_UNION */
#define PMIC_MMW_VSYS_LOW_SET_ADDR(base)                          ((base) + (0x21FUL))

/* 寄存器说明：数模接口保留寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_D2A_RES0_UNION */
#define PMIC_MMW_NP_D2A_RES0_ADDR(base)                           ((base) + (0x220UL))

/* 寄存器说明：数模接口保留寄存器1。
   位域定义UNION结构:  PMIC_MMW_NP_D2A_RES1_UNION */
#define PMIC_MMW_NP_D2A_RES1_ADDR(base)                           ((base) + (0x221UL))

/* 寄存器说明：数模接口保留寄存器2。
   位域定义UNION结构:  PMIC_MMW_NP_D2A_RES2_UNION */
#define PMIC_MMW_NP_D2A_RES2_ADDR(base)                           ((base) + (0x222UL))

/* 寄存器说明：滤波时间控制非下电寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_DEB_CTRL0_UNION */
#define PMIC_MMW_NP_DEB_CTRL0_ADDR(base)                          ((base) + (0x223UL))

/* 寄存器说明：滤波开关非下电寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_DEB_ONOFF0_UNION */
#define PMIC_MMW_NP_DEB_ONOFF0_ADDR(base)                         ((base) + (0x224UL))

/* 寄存器说明：PMU_CLK_OUT上电时序和上电时序中等待时间T配置寄存器。
   位域定义UNION结构:  PMIC_MMW_NP_PWRON_TIME_CFG_UNION */
#define PMIC_MMW_NP_PWRON_TIME_CFG_ADDR(base)                     ((base) + (0x225UL))

/* 寄存器说明：XO时钟默认输出频率预修调寄存器。
   位域定义UNION结构:  PMIC_MMW_DEFAULT_FREQ_CFG_UNION */
#define PMIC_MMW_DEFAULT_FREQ_CFG_ADDR(base)                      ((base) + (0x226UL))

/* 寄存器说明：XO时钟默认输出波形预修调寄存器。
   位域定义UNION结构:  PMIC_MMW_DEFAULT_WAVE_CFG_UNION */
#define PMIC_MMW_DEFAULT_WAVE_CFG_ADDR(base)                      ((base) + (0x227UL))

/* 寄存器说明：XO时钟默认使能预修调寄存器。
   位域定义UNION结构:  PMIC_MMW_DEFAULT_ONOFF_CFG_UNION */
#define PMIC_MMW_DEFAULT_ONOFF_CFG_ADDR(base)                     ((base) + (0x228UL))

/* 寄存器说明：数字IO驱动预修调寄存器0。
   位域定义UNION结构:  PMIC_MMW_DIG_IO_DS_CFG0_UNION */
#define PMIC_MMW_DIG_IO_DS_CFG0_ADDR(base)                        ((base) + (0x229UL))

/* 寄存器说明：debug寄存器写屏蔽控制寄存器。
   位域定义UNION结构:  PMIC_MMW_DEBUG_LOCK_UNION */
#define PMIC_MMW_DEBUG_LOCK_ADDR(base)                            ((base) + (0x22AUL))

/* 寄存器说明：数字系统debug预留寄存器0。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG0_UNION */
#define PMIC_MMW_SYS_DEBUG0_ADDR(base)                            ((base) + (0x22BUL))

/* 寄存器说明：数字系统debug预留寄存器1。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG1_UNION */
#define PMIC_MMW_SYS_DEBUG1_ADDR(base)                            ((base) + (0x22CUL))

/* 寄存器说明：RC时钟debug预留寄存器2。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG2_UNION */
#define PMIC_MMW_SYS_DEBUG2_ADDR(base)                            ((base) + (0x22DUL))

/* 寄存器说明：数字系统debug预留寄存器3。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG3_UNION */
#define PMIC_MMW_SYS_DEBUG3_ADDR(base)                            ((base) + (0x22EUL))

/* 寄存器说明：数字系统debug预留寄存器4。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG4_UNION */
#define PMIC_MMW_SYS_DEBUG4_ADDR(base)                            ((base) + (0x22FUL))

/* 寄存器说明：数字系统debug预留寄存器5。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG5_UNION */
#define PMIC_MMW_SYS_DEBUG5_ADDR(base)                            ((base) + (0x230UL))

/* 寄存器说明：OTP回读寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_0_R_UNION */
#define PMIC_MMW_OTP_0_R_ADDR(base)                               ((base) + (0x231UL))

/* 寄存器说明：OTP回读寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_1_R_UNION */
#define PMIC_MMW_OTP_1_R_ADDR(base)                               ((base) + (0x232UL))

/* 寄存器说明：OTP回读寄存器2。
   位域定义UNION结构:  PMIC_MMW_OTP_2_R_UNION */
#define PMIC_MMW_OTP_2_R_ADDR(base)                               ((base) + (0x233UL))

/* 寄存器说明：OTP回读寄存器3。
   位域定义UNION结构:  PMIC_MMW_OTP_3_R_UNION */
#define PMIC_MMW_OTP_3_R_ADDR(base)                               ((base) + (0x234UL))

/* 寄存器说明：OTP回读寄存器4。
   位域定义UNION结构:  PMIC_MMW_OTP_4_R_UNION */
#define PMIC_MMW_OTP_4_R_ADDR(base)                               ((base) + (0x235UL))

/* 寄存器说明：OTP回读寄存器5。
   位域定义UNION结构:  PMIC_MMW_OTP_5_R_UNION */
#define PMIC_MMW_OTP_5_R_ADDR(base)                               ((base) + (0x236UL))

/* 寄存器说明：OTP回读寄存器6。
   位域定义UNION结构:  PMIC_MMW_OTP_6_R_UNION */
#define PMIC_MMW_OTP_6_R_ADDR(base)                               ((base) + (0x237UL))

/* 寄存器说明：OTP回读寄存器7。
   位域定义UNION结构:  PMIC_MMW_OTP_7_R_UNION */
#define PMIC_MMW_OTP_7_R_ADDR(base)                               ((base) + (0x238UL))

/* 寄存器说明：OTP回读寄存器8。
   位域定义UNION结构:  PMIC_MMW_OTP_8_R_UNION */
#define PMIC_MMW_OTP_8_R_ADDR(base)                               ((base) + (0x239UL))

/* 寄存器说明：OTP回读寄存器9。
   位域定义UNION结构:  PMIC_MMW_OTP_9_R_UNION */
#define PMIC_MMW_OTP_9_R_ADDR(base)                               ((base) + (0x23AUL))

/* 寄存器说明：OTP回读寄存器10。
   位域定义UNION结构:  PMIC_MMW_OTP_10_R_UNION */
#define PMIC_MMW_OTP_10_R_ADDR(base)                              ((base) + (0x23BUL))

/* 寄存器说明：OTP回读寄存器11。
   位域定义UNION结构:  PMIC_MMW_OTP_11_R_UNION */
#define PMIC_MMW_OTP_11_R_ADDR(base)                              ((base) + (0x23CUL))

/* 寄存器说明：OTP回读寄存器12。
   位域定义UNION结构:  PMIC_MMW_OTP_12_R_UNION */
#define PMIC_MMW_OTP_12_R_ADDR(base)                              ((base) + (0x23DUL))

/* 寄存器说明：OTP回读寄存器13。
   位域定义UNION结构:  PMIC_MMW_OTP_13_R_UNION */
#define PMIC_MMW_OTP_13_R_ADDR(base)                              ((base) + (0x23EUL))

/* 寄存器说明：OTP回读寄存器14。
   位域定义UNION结构:  PMIC_MMW_OTP_14_R_UNION */
#define PMIC_MMW_OTP_14_R_ADDR(base)                              ((base) + (0x23FUL))

/* 寄存器说明：OTP回读寄存器15。
   位域定义UNION结构:  PMIC_MMW_OTP_15_R_UNION */
#define PMIC_MMW_OTP_15_R_ADDR(base)                              ((base) + (0x240UL))

/* 寄存器说明：OTP回读寄存器16。
   位域定义UNION结构:  PMIC_MMW_OTP_16_R_UNION */
#define PMIC_MMW_OTP_16_R_ADDR(base)                              ((base) + (0x241UL))

/* 寄存器说明：OTP回读寄存器17。
   位域定义UNION结构:  PMIC_MMW_OTP_17_R_UNION */
#define PMIC_MMW_OTP_17_R_ADDR(base)                              ((base) + (0x242UL))

/* 寄存器说明：OTP回读寄存器18。
   位域定义UNION结构:  PMIC_MMW_OTP_18_R_UNION */
#define PMIC_MMW_OTP_18_R_ADDR(base)                              ((base) + (0x243UL))

/* 寄存器说明：OTP回读寄存器19。
   位域定义UNION结构:  PMIC_MMW_OTP_19_R_UNION */
#define PMIC_MMW_OTP_19_R_ADDR(base)                              ((base) + (0x244UL))

/* 寄存器说明：OTP回读寄存器20。
   位域定义UNION结构:  PMIC_MMW_OTP_20_R_UNION */
#define PMIC_MMW_OTP_20_R_ADDR(base)                              ((base) + (0x245UL))

/* 寄存器说明：OTP回读寄存器21。
   位域定义UNION结构:  PMIC_MMW_OTP_21_R_UNION */
#define PMIC_MMW_OTP_21_R_ADDR(base)                              ((base) + (0x246UL))

/* 寄存器说明：OTP回读寄存器22。
   位域定义UNION结构:  PMIC_MMW_OTP_22_R_UNION */
#define PMIC_MMW_OTP_22_R_ADDR(base)                              ((base) + (0x247UL))

/* 寄存器说明：OTP回读寄存器23。
   位域定义UNION结构:  PMIC_MMW_OTP_23_R_UNION */
#define PMIC_MMW_OTP_23_R_ADDR(base)                              ((base) + (0x248UL))

/* 寄存器说明：OTP回读寄存器24。
   位域定义UNION结构:  PMIC_MMW_OTP_24_R_UNION */
#define PMIC_MMW_OTP_24_R_ADDR(base)                              ((base) + (0x249UL))

/* 寄存器说明：OTP回读寄存器25。
   位域定义UNION结构:  PMIC_MMW_OTP_25_R_UNION */
#define PMIC_MMW_OTP_25_R_ADDR(base)                              ((base) + (0x24AUL))

/* 寄存器说明：OTP回读寄存器26。
   位域定义UNION结构:  PMIC_MMW_OTP_26_R_UNION */
#define PMIC_MMW_OTP_26_R_ADDR(base)                              ((base) + (0x24BUL))

/* 寄存器说明：OTP回读寄存器27。
   位域定义UNION结构:  PMIC_MMW_OTP_27_R_UNION */
#define PMIC_MMW_OTP_27_R_ADDR(base)                              ((base) + (0x24CUL))

/* 寄存器说明：OTP回读寄存器28。
   位域定义UNION结构:  PMIC_MMW_OTP_28_R_UNION */
#define PMIC_MMW_OTP_28_R_ADDR(base)                              ((base) + (0x24DUL))

/* 寄存器说明：OTP回读寄存器29。
   位域定义UNION结构:  PMIC_MMW_OTP_29_R_UNION */
#define PMIC_MMW_OTP_29_R_ADDR(base)                              ((base) + (0x24EUL))

/* 寄存器说明：OTP回读寄存器30。
   位域定义UNION结构:  PMIC_MMW_OTP_30_R_UNION */
#define PMIC_MMW_OTP_30_R_ADDR(base)                              ((base) + (0x24FUL))

/* 寄存器说明：OTP回读寄存器31。
   位域定义UNION结构:  PMIC_MMW_OTP_31_R_UNION */
#define PMIC_MMW_OTP_31_R_ADDR(base)                              ((base) + (0x250UL))

/* 寄存器说明：OTP回读寄存器32。
   位域定义UNION结构:  PMIC_MMW_OTP_32_R_UNION */
#define PMIC_MMW_OTP_32_R_ADDR(base)                              ((base) + (0x251UL))

/* 寄存器说明：OTP回读寄存器33。
   位域定义UNION结构:  PMIC_MMW_OTP_33_R_UNION */
#define PMIC_MMW_OTP_33_R_ADDR(base)                              ((base) + (0x252UL))

/* 寄存器说明：OTP回读寄存器34。
   位域定义UNION结构:  PMIC_MMW_OTP_34_R_UNION */
#define PMIC_MMW_OTP_34_R_ADDR(base)                              ((base) + (0x253UL))

/* 寄存器说明：OTP回读寄存器35。
   位域定义UNION结构:  PMIC_MMW_OTP_35_R_UNION */
#define PMIC_MMW_OTP_35_R_ADDR(base)                              ((base) + (0x254UL))

/* 寄存器说明：OTP回读寄存器36。
   位域定义UNION结构:  PMIC_MMW_OTP_36_R_UNION */
#define PMIC_MMW_OTP_36_R_ADDR(base)                              ((base) + (0x255UL))

/* 寄存器说明：OTP回读寄存器37。
   位域定义UNION结构:  PMIC_MMW_OTP_37_R_UNION */
#define PMIC_MMW_OTP_37_R_ADDR(base)                              ((base) + (0x256UL))

/* 寄存器说明：OTP回读寄存器38。
   位域定义UNION结构:  PMIC_MMW_OTP_38_R_UNION */
#define PMIC_MMW_OTP_38_R_ADDR(base)                              ((base) + (0x257UL))

/* 寄存器说明：OTP回读寄存器39。
   位域定义UNION结构:  PMIC_MMW_OTP_39_R_UNION */
#define PMIC_MMW_OTP_39_R_ADDR(base)                              ((base) + (0x258UL))

/* 寄存器说明：OTP回读寄存器40。
   位域定义UNION结构:  PMIC_MMW_OTP_40_R_UNION */
#define PMIC_MMW_OTP_40_R_ADDR(base)                              ((base) + (0x259UL))

/* 寄存器说明：OTP回读寄存器41。
   位域定义UNION结构:  PMIC_MMW_OTP_41_R_UNION */
#define PMIC_MMW_OTP_41_R_ADDR(base)                              ((base) + (0x25AUL))

/* 寄存器说明：OTP回读寄存器42。
   位域定义UNION结构:  PMIC_MMW_OTP_42_R_UNION */
#define PMIC_MMW_OTP_42_R_ADDR(base)                              ((base) + (0x25BUL))

/* 寄存器说明：OTP回读寄存器43。
   位域定义UNION结构:  PMIC_MMW_OTP_43_R_UNION */
#define PMIC_MMW_OTP_43_R_ADDR(base)                              ((base) + (0x25CUL))

/* 寄存器说明：OTP回读寄存器44。
   位域定义UNION结构:  PMIC_MMW_OTP_44_R_UNION */
#define PMIC_MMW_OTP_44_R_ADDR(base)                              ((base) + (0x25DUL))

/* 寄存器说明：OTP回读寄存器45。
   位域定义UNION结构:  PMIC_MMW_OTP_45_R_UNION */
#define PMIC_MMW_OTP_45_R_ADDR(base)                              ((base) + (0x25EUL))

/* 寄存器说明：OTP回读寄存器46。
   位域定义UNION结构:  PMIC_MMW_OTP_46_R_UNION */
#define PMIC_MMW_OTP_46_R_ADDR(base)                              ((base) + (0x25FUL))

/* 寄存器说明：OTP回读寄存器47。
   位域定义UNION结构:  PMIC_MMW_OTP_47_R_UNION */
#define PMIC_MMW_OTP_47_R_ADDR(base)                              ((base) + (0x260UL))

/* 寄存器说明：OTP回读寄存器48。
   位域定义UNION结构:  PMIC_MMW_OTP_48_R_UNION */
#define PMIC_MMW_OTP_48_R_ADDR(base)                              ((base) + (0x261UL))

/* 寄存器说明：OTP回读寄存器49。
   位域定义UNION结构:  PMIC_MMW_OTP_49_R_UNION */
#define PMIC_MMW_OTP_49_R_ADDR(base)                              ((base) + (0x262UL))

/* 寄存器说明：OTP回读寄存器50。
   位域定义UNION结构:  PMIC_MMW_OTP_50_R_UNION */
#define PMIC_MMW_OTP_50_R_ADDR(base)                              ((base) + (0x263UL))

/* 寄存器说明：OTP回读寄存器51。
   位域定义UNION结构:  PMIC_MMW_OTP_51_R_UNION */
#define PMIC_MMW_OTP_51_R_ADDR(base)                              ((base) + (0x264UL))

/* 寄存器说明：OTP回读寄存器52。
   位域定义UNION结构:  PMIC_MMW_OTP_52_R_UNION */
#define PMIC_MMW_OTP_52_R_ADDR(base)                              ((base) + (0x265UL))

/* 寄存器说明：OTP回读寄存器53。
   位域定义UNION结构:  PMIC_MMW_OTP_53_R_UNION */
#define PMIC_MMW_OTP_53_R_ADDR(base)                              ((base) + (0x266UL))

/* 寄存器说明：OTP回读寄存器54。
   位域定义UNION结构:  PMIC_MMW_OTP_54_R_UNION */
#define PMIC_MMW_OTP_54_R_ADDR(base)                              ((base) + (0x267UL))

/* 寄存器说明：OTP回读寄存器55。
   位域定义UNION结构:  PMIC_MMW_OTP_55_R_UNION */
#define PMIC_MMW_OTP_55_R_ADDR(base)                              ((base) + (0x268UL))

/* 寄存器说明：OTP回读寄存器56。
   位域定义UNION结构:  PMIC_MMW_OTP_56_R_UNION */
#define PMIC_MMW_OTP_56_R_ADDR(base)                              ((base) + (0x269UL))

/* 寄存器说明：OTP回读寄存器57。
   位域定义UNION结构:  PMIC_MMW_OTP_57_R_UNION */
#define PMIC_MMW_OTP_57_R_ADDR(base)                              ((base) + (0x26AUL))

/* 寄存器说明：OTP回读寄存器58。
   位域定义UNION结构:  PMIC_MMW_OTP_58_R_UNION */
#define PMIC_MMW_OTP_58_R_ADDR(base)                              ((base) + (0x26BUL))

/* 寄存器说明：OTP回读寄存器59。
   位域定义UNION结构:  PMIC_MMW_OTP_59_R_UNION */
#define PMIC_MMW_OTP_59_R_ADDR(base)                              ((base) + (0x26CUL))

/* 寄存器说明：OTP回读寄存器60。
   位域定义UNION结构:  PMIC_MMW_OTP_60_R_UNION */
#define PMIC_MMW_OTP_60_R_ADDR(base)                              ((base) + (0x26DUL))

/* 寄存器说明：OTP回读寄存器61。
   位域定义UNION结构:  PMIC_MMW_OTP_61_R_UNION */
#define PMIC_MMW_OTP_61_R_ADDR(base)                              ((base) + (0x26EUL))

/* 寄存器说明：OTP回读寄存器62。
   位域定义UNION结构:  PMIC_MMW_OTP_62_R_UNION */
#define PMIC_MMW_OTP_62_R_ADDR(base)                              ((base) + (0x26FUL))

/* 寄存器说明：OTP回读寄存器63。
   位域定义UNION结构:  PMIC_MMW_OTP_63_R_UNION */
#define PMIC_MMW_OTP_63_R_ADDR(base)                              ((base) + (0x270UL))


#else


/* 寄存器说明：CLK_RF0时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_ADDR(base)                     ((base) + (0x200))

/* 寄存器说明：CLK_RF1时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_ADDR(base)                     ((base) + (0x201))

/* 寄存器说明：CLK_RF2时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_ADDR(base)                     ((base) + (0x202))

/* 寄存器说明：CLK_RF3时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_ADDR(base)                     ((base) + (0x203))

/* 寄存器说明：CLK_RF4时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_ADDR(base)                     ((base) + (0x204))

/* 寄存器说明：CLK_SERDES时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_ADDR(base)                  ((base) + (0x205))

/* 寄存器说明：CLK_WIFI0时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_ADDR(base)                   ((base) + (0x206))

/* 寄存器说明：CLK_WIFI1时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_ADDR(base)                   ((base) + (0x207))

/* 寄存器说明：CLK_NFC时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_ADDR(base)                     ((base) + (0x208))

/* 寄存器说明：CLK_SYS时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_ADDR(base)                     ((base) + (0x209))

/* 寄存器说明：CLK_PMU时钟输出波形控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_WAVE_CTRL_UNION */
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_ADDR(base)                     ((base) + (0x20A))

/* 寄存器说明：CLK_RF0时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF0_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_ADDR(base)                     ((base) + (0x20B))

/* 寄存器说明：CLK_RF1时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF1_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_ADDR(base)                     ((base) + (0x20C))

/* 寄存器说明：CLK_RF2时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF2_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_ADDR(base)                     ((base) + (0x20D))

/* 寄存器说明：CLK_RF3时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF3_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_ADDR(base)                     ((base) + (0x20E))

/* 寄存器说明：CLK_RF4时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_RF4_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_ADDR(base)                     ((base) + (0x20F))

/* 寄存器说明：CLK_SERDES时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SERDES_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_ADDR(base)                  ((base) + (0x210))

/* 寄存器说明：CLK_WIFI0时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI0_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_ADDR(base)                   ((base) + (0x211))

/* 寄存器说明：CLK_WIFI1时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_WIFI1_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_ADDR(base)                   ((base) + (0x212))

/* 寄存器说明：CLK_NFC时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_NFC_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_ADDR(base)                     ((base) + (0x213))

/* 寄存器说明：CLK_SYS时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_ADDR(base)                     ((base) + (0x214))

/* 寄存器说明：CLK_PMU时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_PMU_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_ADDR(base)                     ((base) + (0x215))

/* 寄存器说明：CLK_UFS时钟输出频率控制寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_UFS_FREQ_CTRL_UNION */
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_ADDR(base)                     ((base) + (0x216))

/* 寄存器说明：SYS_CLK BUF所用电源选择寄存器。
   位域定义UNION结构:  PMIC_MMW_CLK_SYS_BUF_PWR_SEL_UNION */
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_ADDR(base)                   ((base) + (0x217))

/* 寄存器说明：EN_PMUD配置寄存器。
   位域定义UNION结构:  PMIC_MMW_EN_PMUD_UNION */
#define PMIC_MMW_EN_PMUD_ADDR(base)                               ((base) + (0x218))

/* 寄存器说明：PMUD调压寄存器。
   位域定义UNION结构:  PMIC_MMW_PMUD_VSET_UNION */
#define PMIC_MMW_PMUD_VSET_ADDR(base)                             ((base) + (0x219))

/* 寄存器说明：时钟控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_CLK_TOP_CTRL1_0_UNION */
#define PMIC_MMW_CLK_TOP_CTRL1_0_ADDR(base)                       ((base) + (0x21A))

/* 寄存器说明：时钟控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_CLK_TOP_CTRL1_1_UNION */
#define PMIC_MMW_CLK_TOP_CTRL1_1_ADDR(base)                       ((base) + (0x21B))

/* 寄存器说明：256K RC控制寄存器0。
   位域定义UNION结构:  PMIC_MMW_CLK_256K_CTRL0_UNION */
#define PMIC_MMW_CLK_256K_CTRL0_ADDR(base)                        ((base) + (0x21C))

/* 寄存器说明：256K RC控制寄存器1。
   位域定义UNION结构:  PMIC_MMW_CLK_256K_CTRL1_UNION */
#define PMIC_MMW_CLK_256K_CTRL1_ADDR(base)                        ((base) + (0x21D))

/* 寄存器说明：XO corner检测数据寄存器。
   位域定义UNION结构:  PMIC_MMW_XO_DET_DATA_UNION */
#define PMIC_MMW_XO_DET_DATA_ADDR(base)                           ((base) + (0x21E))

/* 寄存器说明：IN端低压阈值调整寄存器。
   位域定义UNION结构:  PMIC_MMW_VSYS_LOW_SET_UNION */
#define PMIC_MMW_VSYS_LOW_SET_ADDR(base)                          ((base) + (0x21F))

/* 寄存器说明：数模接口保留寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_D2A_RES0_UNION */
#define PMIC_MMW_NP_D2A_RES0_ADDR(base)                           ((base) + (0x220))

/* 寄存器说明：数模接口保留寄存器1。
   位域定义UNION结构:  PMIC_MMW_NP_D2A_RES1_UNION */
#define PMIC_MMW_NP_D2A_RES1_ADDR(base)                           ((base) + (0x221))

/* 寄存器说明：数模接口保留寄存器2。
   位域定义UNION结构:  PMIC_MMW_NP_D2A_RES2_UNION */
#define PMIC_MMW_NP_D2A_RES2_ADDR(base)                           ((base) + (0x222))

/* 寄存器说明：滤波时间控制非下电寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_DEB_CTRL0_UNION */
#define PMIC_MMW_NP_DEB_CTRL0_ADDR(base)                          ((base) + (0x223))

/* 寄存器说明：滤波开关非下电寄存器0。
   位域定义UNION结构:  PMIC_MMW_NP_DEB_ONOFF0_UNION */
#define PMIC_MMW_NP_DEB_ONOFF0_ADDR(base)                         ((base) + (0x224))

/* 寄存器说明：PMU_CLK_OUT上电时序和上电时序中等待时间T配置寄存器。
   位域定义UNION结构:  PMIC_MMW_NP_PWRON_TIME_CFG_UNION */
#define PMIC_MMW_NP_PWRON_TIME_CFG_ADDR(base)                     ((base) + (0x225))

/* 寄存器说明：XO时钟默认输出频率预修调寄存器。
   位域定义UNION结构:  PMIC_MMW_DEFAULT_FREQ_CFG_UNION */
#define PMIC_MMW_DEFAULT_FREQ_CFG_ADDR(base)                      ((base) + (0x226))

/* 寄存器说明：XO时钟默认输出波形预修调寄存器。
   位域定义UNION结构:  PMIC_MMW_DEFAULT_WAVE_CFG_UNION */
#define PMIC_MMW_DEFAULT_WAVE_CFG_ADDR(base)                      ((base) + (0x227))

/* 寄存器说明：XO时钟默认使能预修调寄存器。
   位域定义UNION结构:  PMIC_MMW_DEFAULT_ONOFF_CFG_UNION */
#define PMIC_MMW_DEFAULT_ONOFF_CFG_ADDR(base)                     ((base) + (0x228))

/* 寄存器说明：数字IO驱动预修调寄存器0。
   位域定义UNION结构:  PMIC_MMW_DIG_IO_DS_CFG0_UNION */
#define PMIC_MMW_DIG_IO_DS_CFG0_ADDR(base)                        ((base) + (0x229))

/* 寄存器说明：debug寄存器写屏蔽控制寄存器。
   位域定义UNION结构:  PMIC_MMW_DEBUG_LOCK_UNION */
#define PMIC_MMW_DEBUG_LOCK_ADDR(base)                            ((base) + (0x22A))

/* 寄存器说明：数字系统debug预留寄存器0。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG0_UNION */
#define PMIC_MMW_SYS_DEBUG0_ADDR(base)                            ((base) + (0x22B))

/* 寄存器说明：数字系统debug预留寄存器1。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG1_UNION */
#define PMIC_MMW_SYS_DEBUG1_ADDR(base)                            ((base) + (0x22C))

/* 寄存器说明：RC时钟debug预留寄存器2。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG2_UNION */
#define PMIC_MMW_SYS_DEBUG2_ADDR(base)                            ((base) + (0x22D))

/* 寄存器说明：数字系统debug预留寄存器3。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG3_UNION */
#define PMIC_MMW_SYS_DEBUG3_ADDR(base)                            ((base) + (0x22E))

/* 寄存器说明：数字系统debug预留寄存器4。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG4_UNION */
#define PMIC_MMW_SYS_DEBUG4_ADDR(base)                            ((base) + (0x22F))

/* 寄存器说明：数字系统debug预留寄存器5。
   位域定义UNION结构:  PMIC_MMW_SYS_DEBUG5_UNION */
#define PMIC_MMW_SYS_DEBUG5_ADDR(base)                            ((base) + (0x230))

/* 寄存器说明：OTP回读寄存器0。
   位域定义UNION结构:  PMIC_MMW_OTP_0_R_UNION */
#define PMIC_MMW_OTP_0_R_ADDR(base)                               ((base) + (0x231))

/* 寄存器说明：OTP回读寄存器1。
   位域定义UNION结构:  PMIC_MMW_OTP_1_R_UNION */
#define PMIC_MMW_OTP_1_R_ADDR(base)                               ((base) + (0x232))

/* 寄存器说明：OTP回读寄存器2。
   位域定义UNION结构:  PMIC_MMW_OTP_2_R_UNION */
#define PMIC_MMW_OTP_2_R_ADDR(base)                               ((base) + (0x233))

/* 寄存器说明：OTP回读寄存器3。
   位域定义UNION结构:  PMIC_MMW_OTP_3_R_UNION */
#define PMIC_MMW_OTP_3_R_ADDR(base)                               ((base) + (0x234))

/* 寄存器说明：OTP回读寄存器4。
   位域定义UNION结构:  PMIC_MMW_OTP_4_R_UNION */
#define PMIC_MMW_OTP_4_R_ADDR(base)                               ((base) + (0x235))

/* 寄存器说明：OTP回读寄存器5。
   位域定义UNION结构:  PMIC_MMW_OTP_5_R_UNION */
#define PMIC_MMW_OTP_5_R_ADDR(base)                               ((base) + (0x236))

/* 寄存器说明：OTP回读寄存器6。
   位域定义UNION结构:  PMIC_MMW_OTP_6_R_UNION */
#define PMIC_MMW_OTP_6_R_ADDR(base)                               ((base) + (0x237))

/* 寄存器说明：OTP回读寄存器7。
   位域定义UNION结构:  PMIC_MMW_OTP_7_R_UNION */
#define PMIC_MMW_OTP_7_R_ADDR(base)                               ((base) + (0x238))

/* 寄存器说明：OTP回读寄存器8。
   位域定义UNION结构:  PMIC_MMW_OTP_8_R_UNION */
#define PMIC_MMW_OTP_8_R_ADDR(base)                               ((base) + (0x239))

/* 寄存器说明：OTP回读寄存器9。
   位域定义UNION结构:  PMIC_MMW_OTP_9_R_UNION */
#define PMIC_MMW_OTP_9_R_ADDR(base)                               ((base) + (0x23A))

/* 寄存器说明：OTP回读寄存器10。
   位域定义UNION结构:  PMIC_MMW_OTP_10_R_UNION */
#define PMIC_MMW_OTP_10_R_ADDR(base)                              ((base) + (0x23B))

/* 寄存器说明：OTP回读寄存器11。
   位域定义UNION结构:  PMIC_MMW_OTP_11_R_UNION */
#define PMIC_MMW_OTP_11_R_ADDR(base)                              ((base) + (0x23C))

/* 寄存器说明：OTP回读寄存器12。
   位域定义UNION结构:  PMIC_MMW_OTP_12_R_UNION */
#define PMIC_MMW_OTP_12_R_ADDR(base)                              ((base) + (0x23D))

/* 寄存器说明：OTP回读寄存器13。
   位域定义UNION结构:  PMIC_MMW_OTP_13_R_UNION */
#define PMIC_MMW_OTP_13_R_ADDR(base)                              ((base) + (0x23E))

/* 寄存器说明：OTP回读寄存器14。
   位域定义UNION结构:  PMIC_MMW_OTP_14_R_UNION */
#define PMIC_MMW_OTP_14_R_ADDR(base)                              ((base) + (0x23F))

/* 寄存器说明：OTP回读寄存器15。
   位域定义UNION结构:  PMIC_MMW_OTP_15_R_UNION */
#define PMIC_MMW_OTP_15_R_ADDR(base)                              ((base) + (0x240))

/* 寄存器说明：OTP回读寄存器16。
   位域定义UNION结构:  PMIC_MMW_OTP_16_R_UNION */
#define PMIC_MMW_OTP_16_R_ADDR(base)                              ((base) + (0x241))

/* 寄存器说明：OTP回读寄存器17。
   位域定义UNION结构:  PMIC_MMW_OTP_17_R_UNION */
#define PMIC_MMW_OTP_17_R_ADDR(base)                              ((base) + (0x242))

/* 寄存器说明：OTP回读寄存器18。
   位域定义UNION结构:  PMIC_MMW_OTP_18_R_UNION */
#define PMIC_MMW_OTP_18_R_ADDR(base)                              ((base) + (0x243))

/* 寄存器说明：OTP回读寄存器19。
   位域定义UNION结构:  PMIC_MMW_OTP_19_R_UNION */
#define PMIC_MMW_OTP_19_R_ADDR(base)                              ((base) + (0x244))

/* 寄存器说明：OTP回读寄存器20。
   位域定义UNION结构:  PMIC_MMW_OTP_20_R_UNION */
#define PMIC_MMW_OTP_20_R_ADDR(base)                              ((base) + (0x245))

/* 寄存器说明：OTP回读寄存器21。
   位域定义UNION结构:  PMIC_MMW_OTP_21_R_UNION */
#define PMIC_MMW_OTP_21_R_ADDR(base)                              ((base) + (0x246))

/* 寄存器说明：OTP回读寄存器22。
   位域定义UNION结构:  PMIC_MMW_OTP_22_R_UNION */
#define PMIC_MMW_OTP_22_R_ADDR(base)                              ((base) + (0x247))

/* 寄存器说明：OTP回读寄存器23。
   位域定义UNION结构:  PMIC_MMW_OTP_23_R_UNION */
#define PMIC_MMW_OTP_23_R_ADDR(base)                              ((base) + (0x248))

/* 寄存器说明：OTP回读寄存器24。
   位域定义UNION结构:  PMIC_MMW_OTP_24_R_UNION */
#define PMIC_MMW_OTP_24_R_ADDR(base)                              ((base) + (0x249))

/* 寄存器说明：OTP回读寄存器25。
   位域定义UNION结构:  PMIC_MMW_OTP_25_R_UNION */
#define PMIC_MMW_OTP_25_R_ADDR(base)                              ((base) + (0x24A))

/* 寄存器说明：OTP回读寄存器26。
   位域定义UNION结构:  PMIC_MMW_OTP_26_R_UNION */
#define PMIC_MMW_OTP_26_R_ADDR(base)                              ((base) + (0x24B))

/* 寄存器说明：OTP回读寄存器27。
   位域定义UNION结构:  PMIC_MMW_OTP_27_R_UNION */
#define PMIC_MMW_OTP_27_R_ADDR(base)                              ((base) + (0x24C))

/* 寄存器说明：OTP回读寄存器28。
   位域定义UNION结构:  PMIC_MMW_OTP_28_R_UNION */
#define PMIC_MMW_OTP_28_R_ADDR(base)                              ((base) + (0x24D))

/* 寄存器说明：OTP回读寄存器29。
   位域定义UNION结构:  PMIC_MMW_OTP_29_R_UNION */
#define PMIC_MMW_OTP_29_R_ADDR(base)                              ((base) + (0x24E))

/* 寄存器说明：OTP回读寄存器30。
   位域定义UNION结构:  PMIC_MMW_OTP_30_R_UNION */
#define PMIC_MMW_OTP_30_R_ADDR(base)                              ((base) + (0x24F))

/* 寄存器说明：OTP回读寄存器31。
   位域定义UNION结构:  PMIC_MMW_OTP_31_R_UNION */
#define PMIC_MMW_OTP_31_R_ADDR(base)                              ((base) + (0x250))

/* 寄存器说明：OTP回读寄存器32。
   位域定义UNION结构:  PMIC_MMW_OTP_32_R_UNION */
#define PMIC_MMW_OTP_32_R_ADDR(base)                              ((base) + (0x251))

/* 寄存器说明：OTP回读寄存器33。
   位域定义UNION结构:  PMIC_MMW_OTP_33_R_UNION */
#define PMIC_MMW_OTP_33_R_ADDR(base)                              ((base) + (0x252))

/* 寄存器说明：OTP回读寄存器34。
   位域定义UNION结构:  PMIC_MMW_OTP_34_R_UNION */
#define PMIC_MMW_OTP_34_R_ADDR(base)                              ((base) + (0x253))

/* 寄存器说明：OTP回读寄存器35。
   位域定义UNION结构:  PMIC_MMW_OTP_35_R_UNION */
#define PMIC_MMW_OTP_35_R_ADDR(base)                              ((base) + (0x254))

/* 寄存器说明：OTP回读寄存器36。
   位域定义UNION结构:  PMIC_MMW_OTP_36_R_UNION */
#define PMIC_MMW_OTP_36_R_ADDR(base)                              ((base) + (0x255))

/* 寄存器说明：OTP回读寄存器37。
   位域定义UNION结构:  PMIC_MMW_OTP_37_R_UNION */
#define PMIC_MMW_OTP_37_R_ADDR(base)                              ((base) + (0x256))

/* 寄存器说明：OTP回读寄存器38。
   位域定义UNION结构:  PMIC_MMW_OTP_38_R_UNION */
#define PMIC_MMW_OTP_38_R_ADDR(base)                              ((base) + (0x257))

/* 寄存器说明：OTP回读寄存器39。
   位域定义UNION结构:  PMIC_MMW_OTP_39_R_UNION */
#define PMIC_MMW_OTP_39_R_ADDR(base)                              ((base) + (0x258))

/* 寄存器说明：OTP回读寄存器40。
   位域定义UNION结构:  PMIC_MMW_OTP_40_R_UNION */
#define PMIC_MMW_OTP_40_R_ADDR(base)                              ((base) + (0x259))

/* 寄存器说明：OTP回读寄存器41。
   位域定义UNION结构:  PMIC_MMW_OTP_41_R_UNION */
#define PMIC_MMW_OTP_41_R_ADDR(base)                              ((base) + (0x25A))

/* 寄存器说明：OTP回读寄存器42。
   位域定义UNION结构:  PMIC_MMW_OTP_42_R_UNION */
#define PMIC_MMW_OTP_42_R_ADDR(base)                              ((base) + (0x25B))

/* 寄存器说明：OTP回读寄存器43。
   位域定义UNION结构:  PMIC_MMW_OTP_43_R_UNION */
#define PMIC_MMW_OTP_43_R_ADDR(base)                              ((base) + (0x25C))

/* 寄存器说明：OTP回读寄存器44。
   位域定义UNION结构:  PMIC_MMW_OTP_44_R_UNION */
#define PMIC_MMW_OTP_44_R_ADDR(base)                              ((base) + (0x25D))

/* 寄存器说明：OTP回读寄存器45。
   位域定义UNION结构:  PMIC_MMW_OTP_45_R_UNION */
#define PMIC_MMW_OTP_45_R_ADDR(base)                              ((base) + (0x25E))

/* 寄存器说明：OTP回读寄存器46。
   位域定义UNION结构:  PMIC_MMW_OTP_46_R_UNION */
#define PMIC_MMW_OTP_46_R_ADDR(base)                              ((base) + (0x25F))

/* 寄存器说明：OTP回读寄存器47。
   位域定义UNION结构:  PMIC_MMW_OTP_47_R_UNION */
#define PMIC_MMW_OTP_47_R_ADDR(base)                              ((base) + (0x260))

/* 寄存器说明：OTP回读寄存器48。
   位域定义UNION结构:  PMIC_MMW_OTP_48_R_UNION */
#define PMIC_MMW_OTP_48_R_ADDR(base)                              ((base) + (0x261))

/* 寄存器说明：OTP回读寄存器49。
   位域定义UNION结构:  PMIC_MMW_OTP_49_R_UNION */
#define PMIC_MMW_OTP_49_R_ADDR(base)                              ((base) + (0x262))

/* 寄存器说明：OTP回读寄存器50。
   位域定义UNION结构:  PMIC_MMW_OTP_50_R_UNION */
#define PMIC_MMW_OTP_50_R_ADDR(base)                              ((base) + (0x263))

/* 寄存器说明：OTP回读寄存器51。
   位域定义UNION结构:  PMIC_MMW_OTP_51_R_UNION */
#define PMIC_MMW_OTP_51_R_ADDR(base)                              ((base) + (0x264))

/* 寄存器说明：OTP回读寄存器52。
   位域定义UNION结构:  PMIC_MMW_OTP_52_R_UNION */
#define PMIC_MMW_OTP_52_R_ADDR(base)                              ((base) + (0x265))

/* 寄存器说明：OTP回读寄存器53。
   位域定义UNION结构:  PMIC_MMW_OTP_53_R_UNION */
#define PMIC_MMW_OTP_53_R_ADDR(base)                              ((base) + (0x266))

/* 寄存器说明：OTP回读寄存器54。
   位域定义UNION结构:  PMIC_MMW_OTP_54_R_UNION */
#define PMIC_MMW_OTP_54_R_ADDR(base)                              ((base) + (0x267))

/* 寄存器说明：OTP回读寄存器55。
   位域定义UNION结构:  PMIC_MMW_OTP_55_R_UNION */
#define PMIC_MMW_OTP_55_R_ADDR(base)                              ((base) + (0x268))

/* 寄存器说明：OTP回读寄存器56。
   位域定义UNION结构:  PMIC_MMW_OTP_56_R_UNION */
#define PMIC_MMW_OTP_56_R_ADDR(base)                              ((base) + (0x269))

/* 寄存器说明：OTP回读寄存器57。
   位域定义UNION结构:  PMIC_MMW_OTP_57_R_UNION */
#define PMIC_MMW_OTP_57_R_ADDR(base)                              ((base) + (0x26A))

/* 寄存器说明：OTP回读寄存器58。
   位域定义UNION结构:  PMIC_MMW_OTP_58_R_UNION */
#define PMIC_MMW_OTP_58_R_ADDR(base)                              ((base) + (0x26B))

/* 寄存器说明：OTP回读寄存器59。
   位域定义UNION结构:  PMIC_MMW_OTP_59_R_UNION */
#define PMIC_MMW_OTP_59_R_ADDR(base)                              ((base) + (0x26C))

/* 寄存器说明：OTP回读寄存器60。
   位域定义UNION结构:  PMIC_MMW_OTP_60_R_UNION */
#define PMIC_MMW_OTP_60_R_ADDR(base)                              ((base) + (0x26D))

/* 寄存器说明：OTP回读寄存器61。
   位域定义UNION结构:  PMIC_MMW_OTP_61_R_UNION */
#define PMIC_MMW_OTP_61_R_ADDR(base)                              ((base) + (0x26E))

/* 寄存器说明：OTP回读寄存器62。
   位域定义UNION结构:  PMIC_MMW_OTP_62_R_UNION */
#define PMIC_MMW_OTP_62_R_ADDR(base)                              ((base) + (0x26F))

/* 寄存器说明：OTP回读寄存器63。
   位域定义UNION结构:  PMIC_MMW_OTP_63_R_UNION */
#define PMIC_MMW_OTP_63_R_ADDR(base)                              ((base) + (0x270))


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
                     (1/5) PMU_CTRL
 ****************************************************************************/
/*****************************************************************************
 结构名    : PMIC_MMW_VERSION0_UNION
 结构说明  : VERSION0 寄存器结构定义。地址偏移量:0x000，初值:0x36，宽度:8
 寄存器说明: 版本寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  project_num0 : 8;  /* bit[0-7]: ASIC码36表示项目编号6。 */
    } reg;
} PMIC_MMW_VERSION0_UNION;
#endif
#define PMIC_MMW_VERSION0_project_num0_START  (0)
#define PMIC_MMW_VERSION0_project_num0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_VERSION1_UNION
 结构说明  : VERSION1 寄存器结构定义。地址偏移量:0x001，初值:0x35，宽度:8
 寄存器说明: 版本寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  project_num1 : 8;  /* bit[0-7]: ASIC码35表示项目编号5。 */
    } reg;
} PMIC_MMW_VERSION1_UNION;
#endif
#define PMIC_MMW_VERSION1_project_num1_START  (0)
#define PMIC_MMW_VERSION1_project_num1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_VERSION2_UNION
 结构说明  : VERSION2 寄存器结构定义。地址偏移量:0x002，初值:0x36，宽度:8
 寄存器说明: 版本寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  project_num2 : 8;  /* bit[0-7]: ASIC码36表示项目编号6。 */
    } reg;
} PMIC_MMW_VERSION2_UNION;
#endif
#define PMIC_MMW_VERSION2_project_num2_START  (0)
#define PMIC_MMW_VERSION2_project_num2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_VERSION3_UNION
 结构说明  : VERSION3 寄存器结构定义。地址偏移量:0x003，初值:0x33，宽度:8
 寄存器说明: 版本寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  project_num3 : 8;  /* bit[0-7]: ASIC码34表示项目编号3。 */
    } reg;
} PMIC_MMW_VERSION3_UNION;
#endif
#define PMIC_MMW_VERSION3_project_num3_START  (0)
#define PMIC_MMW_VERSION3_project_num3_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_VERSION4_UNION
 结构说明  : VERSION4 寄存器结构定义。地址偏移量:0x004，初值:0x01，宽度:8
 寄存器说明: 版本寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  version : 8;  /* bit[0-7]: 表示版本为V100的1。 */
    } reg;
} PMIC_MMW_VERSION4_UNION;
#endif
#define PMIC_MMW_VERSION4_version_START  (0)
#define PMIC_MMW_VERSION4_version_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_VERSION5_UNION
 结构说明  : VERSION5 寄存器结构定义。地址偏移量:0x005，初值:0x00，宽度:8
 寄存器说明: 版本寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  chip_id : 8;  /* bit[0-7]: 表示版本为V100的00。 */
    } reg;
} PMIC_MMW_VERSION5_UNION;
#endif
#define PMIC_MMW_VERSION5_chip_id_START  (0)
#define PMIC_MMW_VERSION5_chip_id_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_STATUS0_UNION
 结构说明  : STATUS0 寄存器结构定义。地址偏移量:0x006，初值:0x00，宽度:8
 寄存器说明: 状态标志寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  st_vsys_ov_d200ur        : 1;  /* bit[0]: 过压状态指示位(上升沿去抖200us)。
                                                                 0：VSYS电压小于5.5V；
                                                                 1：VSYS电压大于5.5V。 */
        unsigned char  st_vsys_pwroff_abs_d20nr : 1;  /* bit[1]: 绝对关机电压状态位指示位（没有同步）。
                                                                 0：VSYS电压大于2.3V(可寄存器配置为2.1V/2.2V)；
                                                                 1：VSYS电压小于2.3V(可寄存器配置为2.1V/2.2V)。
                                                                 配置寄存器在非下电区。 */
        unsigned char  st_vsys_pwron_d60ur      : 1;  /* bit[2]: 开机电压状态指示位（上升沿去抖60us）。
                                                                 0：VSYS大于2.9V(可寄存器配置为3.2V)；
                                                                 1：VSYS小于2.9V(可寄存器配置为3.2V)。 */
        unsigned char  st_thsd_otmp140_d540ur   : 1;  /* bit[3]: 过温状态位(上升沿默认去抖540us)。
                                                                 0：温度小于140℃；
                                                                 1：温度大于140℃。 */
        unsigned char  st_thsd_otmp125_d120ur   : 1;  /* bit[4]: 过温状态指示位(上升沿去抖120us)。
                                                                 0：温度小于设置值；
                                                                 1：温度大于设置值(非140关机门限)。
                                                                 温度设置值参见TH_CTRL[thsd_tmp_set]，默认125℃。  */
        unsigned char  st_pmic63_en_deb         : 1;  /* bit[5]: pmic63_EN状态指示位(有双沿去抖)。
                                                                 0：pmic63_EN处于低电平；
                                                                 1：pmic63_EN处于高电平。 */
        unsigned char  st_buf_en_deb            : 1;  /* bit[6]: CLK_BUF_EN管脚状态指示位(有双沿去抖)。
                                                                 0：CLK_BUF_EN处于低电平；
                                                                 1：CLK_BUF_EN处于高电平。 */
        unsigned char  st_vin_ldom_vld_deb      : 1;  /* bit[7]: VIN_LDO_1P95电源检测状态寄存器（有双沿滤波）。
                                                                 0：VIN_LDO_1P95电压不合格；
                                                                 1：VIN_LDO_1P95电压合格。 */
    } reg;
} PMIC_MMW_STATUS0_UNION;
#endif
#define PMIC_MMW_STATUS0_st_vsys_ov_d200ur_START         (0)
#define PMIC_MMW_STATUS0_st_vsys_ov_d200ur_END           (0)
#define PMIC_MMW_STATUS0_st_vsys_pwroff_abs_d20nr_START  (1)
#define PMIC_MMW_STATUS0_st_vsys_pwroff_abs_d20nr_END    (1)
#define PMIC_MMW_STATUS0_st_vsys_pwron_d60ur_START       (2)
#define PMIC_MMW_STATUS0_st_vsys_pwron_d60ur_END         (2)
#define PMIC_MMW_STATUS0_st_thsd_otmp140_d540ur_START    (3)
#define PMIC_MMW_STATUS0_st_thsd_otmp140_d540ur_END      (3)
#define PMIC_MMW_STATUS0_st_thsd_otmp125_d120ur_START    (4)
#define PMIC_MMW_STATUS0_st_thsd_otmp125_d120ur_END      (4)
#define PMIC_MMW_STATUS0_st_pmic63_en_deb_START          (5)
#define PMIC_MMW_STATUS0_st_pmic63_en_deb_END            (5)
#define PMIC_MMW_STATUS0_st_buf_en_deb_START             (6)
#define PMIC_MMW_STATUS0_st_buf_en_deb_END               (6)
#define PMIC_MMW_STATUS0_st_vin_ldom_vld_deb_START       (7)
#define PMIC_MMW_STATUS0_st_vin_ldom_vld_deb_END         (7)


/*****************************************************************************
 结构名    : PMIC_MMW_STATUS1_UNION
 结构说明  : STATUS1 寄存器结构定义。地址偏移量:0x007，初值:0x00，宽度:8
 寄存器说明: 状态标志寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  st_dcxo_clk_sel         : 1;  /* bit[0]  : DCXO模式当前工作时钟源。
                                                                  0：32K RC；
                                                                  1：19.2MHz 586分频时钟。 */
        unsigned char  st_ldo_core_vld_d20nf   : 1;  /* bit[1]  : LDO_CORE电源检测状态寄存器（下降沿去抖20ns）。
                                                                  0：LDO_CORE电压不合格；
                                                                  1：LDO_CORE电压合格。 */
        unsigned char  st_vin_ldo_0p9_vld_d1mf : 1;  /* bit[2]  : VIN_LDO_0P9电源检测状态寄存器（有滤波时间）。
                                                                  0：VIN_LDO_0P9电压不合格；
                                                                  1：VIN_LDO_0P9电压合格。 */
        unsigned char  st_vin_ldo_1p1_vld_d1mf : 1;  /* bit[3]  : VIN_LDO_1P1电源检测状态寄存器（有滤波时间）。
                                                                  0：VIN_LDO_1P1电压不合格；
                                                                  1：VIN_LDO_1P1电压合格。 */
        unsigned char  st_vin_ldo_1p3_vld_d1mf : 1;  /* bit[4]  : VIN_LDO_1P3电源检测状态寄存器（有滤波时间）。
                                                                  0：VIN_LDO_1P3电压不合格；
                                                                  1：VIN_LDO_1P3电压合格。 */
        unsigned char  st_vin_io_vld_d5m       : 1;  /* bit[5]  : VDD_IO电源检测状态寄存器(双沿去抖5ms)
                                                                  0：VDD_IO电压不合格；
                                                                  1：VDD_IO电压合格。 */
        unsigned char  reserved                : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_STATUS1_UNION;
#endif
#define PMIC_MMW_STATUS1_st_dcxo_clk_sel_START          (0)
#define PMIC_MMW_STATUS1_st_dcxo_clk_sel_END            (0)
#define PMIC_MMW_STATUS1_st_ldo_core_vld_d20nf_START    (1)
#define PMIC_MMW_STATUS1_st_ldo_core_vld_d20nf_END      (1)
#define PMIC_MMW_STATUS1_st_vin_ldo_0p9_vld_d1mf_START  (2)
#define PMIC_MMW_STATUS1_st_vin_ldo_0p9_vld_d1mf_END    (2)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p1_vld_d1mf_START  (3)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p1_vld_d1mf_END    (3)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p3_vld_d1mf_START  (4)
#define PMIC_MMW_STATUS1_st_vin_ldo_1p3_vld_d1mf_END    (4)
#define PMIC_MMW_STATUS1_st_vin_io_vld_d5m_START        (5)
#define PMIC_MMW_STATUS1_st_vin_io_vld_d5m_END          (5)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO0_ONOFF_UNION
 结构说明  : LDO0_ONOFF 寄存器结构定义。地址偏移量:0x00A，初值:0x00，宽度:8
 寄存器说明: LDO0开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo0_en : 1;  /* bit[0]  : LDO0使能信号。
                                                      0：关闭；
                                                      1：使能。 */
        unsigned char  st_ldo0_en  : 1;  /* bit[1]  : LDO0开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO0_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO0_ONOFF_reg_ldo0_en_START  (0)
#define PMIC_MMW_LDO0_ONOFF_reg_ldo0_en_END    (0)
#define PMIC_MMW_LDO0_ONOFF_st_ldo0_en_START   (1)
#define PMIC_MMW_LDO0_ONOFF_st_ldo0_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO1_ONOFF_UNION
 结构说明  : LDO1_ONOFF 寄存器结构定义。地址偏移量:0x00B，初值:0x00，宽度:8
 寄存器说明: LDO1开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo1_en : 1;  /* bit[0]  : LDO1使能信号。
                                                      0：关闭；
                                                      1：使能。 */
        unsigned char  st_ldo1_en  : 1;  /* bit[1]  : LDO1开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO1_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO1_ONOFF_reg_ldo1_en_START  (0)
#define PMIC_MMW_LDO1_ONOFF_reg_ldo1_en_END    (0)
#define PMIC_MMW_LDO1_ONOFF_st_ldo1_en_START   (1)
#define PMIC_MMW_LDO1_ONOFF_st_ldo1_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO2_ONOFF_UNION
 结构说明  : LDO2_ONOFF 寄存器结构定义。地址偏移量:0x00C，初值:0x00，宽度:8
 寄存器说明: LDO2开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo2_en : 1;  /* bit[0]  : LDO2使能信号。
                                                      0：关闭；
                                                      1：使能。  */
        unsigned char  st_ldo2_en  : 1;  /* bit[1]  : LDO2开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO2_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO2_ONOFF_reg_ldo2_en_START  (0)
#define PMIC_MMW_LDO2_ONOFF_reg_ldo2_en_END    (0)
#define PMIC_MMW_LDO2_ONOFF_st_ldo2_en_START   (1)
#define PMIC_MMW_LDO2_ONOFF_st_ldo2_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO3_ONOFF_UNION
 结构说明  : LDO3_ONOFF 寄存器结构定义。地址偏移量:0x00D，初值:0x00，宽度:8
 寄存器说明: LDO3开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo3_en : 1;  /* bit[0]  : LDO3使能信号。
                                                      0：关闭；
                                                      1：使能。 */
        unsigned char  st_ldo3_en  : 1;  /* bit[1]  : LDO3开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO3_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO3_ONOFF_reg_ldo3_en_START  (0)
#define PMIC_MMW_LDO3_ONOFF_reg_ldo3_en_END    (0)
#define PMIC_MMW_LDO3_ONOFF_st_ldo3_en_START   (1)
#define PMIC_MMW_LDO3_ONOFF_st_ldo3_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO4_ONOFF_UNION
 结构说明  : LDO4_ONOFF 寄存器结构定义。地址偏移量:0x00E，初值:0x00，宽度:8
 寄存器说明: LDO4开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo4_en : 1;  /* bit[0]  : LDO4使能信号。
                                                      0：关闭；
                                                      1：使能。 */
        unsigned char  st_ldo4_en  : 1;  /* bit[1]  : LDO4开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO4_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO4_ONOFF_reg_ldo4_en_START  (0)
#define PMIC_MMW_LDO4_ONOFF_reg_ldo4_en_END    (0)
#define PMIC_MMW_LDO4_ONOFF_st_ldo4_en_START   (1)
#define PMIC_MMW_LDO4_ONOFF_st_ldo4_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO5_ONOFF_UNION
 结构说明  : LDO5_ONOFF 寄存器结构定义。地址偏移量:0x00F，初值:0x00，宽度:8
 寄存器说明: LDO5开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo5_en : 1;  /* bit[0]  : LDO5使能信号。
                                                      0：关闭；
                                                      1：使能。 */
        unsigned char  st_ldo5_en  : 1;  /* bit[1]  : LDO5开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO5_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO5_ONOFF_reg_ldo5_en_START  (0)
#define PMIC_MMW_LDO5_ONOFF_reg_ldo5_en_END    (0)
#define PMIC_MMW_LDO5_ONOFF_st_ldo5_en_START   (1)
#define PMIC_MMW_LDO5_ONOFF_st_ldo5_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO6_ONOFF_UNION
 结构说明  : LDO6_ONOFF 寄存器结构定义。地址偏移量:0x010，初值:0x00，宽度:8
 寄存器说明: LDO6开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo6_en : 1;  /* bit[0]  : LDO6使能信号。
                                                      0：关闭；
                                                      1：使能。 */
        unsigned char  st_ldo6_en  : 1;  /* bit[1]  : LDO6开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO6_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO6_ONOFF_reg_ldo6_en_START  (0)
#define PMIC_MMW_LDO6_ONOFF_reg_ldo6_en_END    (0)
#define PMIC_MMW_LDO6_ONOFF_st_ldo6_en_START   (1)
#define PMIC_MMW_LDO6_ONOFF_st_ldo6_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO7_ONOFF_ECO_UNION
 结构说明  : LDO7_ONOFF_ECO 寄存器结构定义。地址偏移量:0x011，初值:0x00，宽度:8
 寄存器说明: LDO7开关和ECO使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo7_en     : 1;  /* bit[0]  : LDO7使能信号。
                                                          0：关闭；
                                                          1：使能。 */
        unsigned char  st_ldo7_en      : 1;  /* bit[1]  : LDO7开关状态寄存器。
                                                          0：关闭；
                                                          1：开启。 */
        unsigned char  reserved_0      : 2;  /* bit[2-3]: 保留。 */
        unsigned char  reg_ldo7_eco_en : 1;  /* bit[4]  : LDO7进入低功耗ECO模式控制信号。
                                                          0：normal模式；
                                                          1：ECO模式。 */
        unsigned char  st_ldo7_eco_en  : 1;  /* bit[5]  : LDO7工作状态指示寄存器（指示数模接口处eco_en信号的状态
                                                          0：normal模式；
                                                          1：ECO模式。 */
        unsigned char  reserved_1      : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO7_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_en_START      (0)
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_en_END        (0)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_en_START       (1)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_en_END         (1)
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_eco_en_START  (4)
#define PMIC_MMW_LDO7_ONOFF_ECO_reg_ldo7_eco_en_END    (4)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_eco_en_START   (5)
#define PMIC_MMW_LDO7_ONOFF_ECO_st_ldo7_eco_en_END     (5)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO8_ONOFF_ECO_UNION
 结构说明  : LDO8_ONOFF_ECO 寄存器结构定义。地址偏移量:0x012，初值:0x00，宽度:8
 寄存器说明: LDO8开关和ECO使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo8_en     : 1;  /* bit[0]  : LDO8使能信号。
                                                          0：关闭；
                                                          1：使能。 */
        unsigned char  st_ldo8_en      : 1;  /* bit[1]  : LDO8开关状态寄存器。
                                                          0：关闭；
                                                          1：开启。 */
        unsigned char  reserved_0      : 2;  /* bit[2-3]: 保留。 */
        unsigned char  reg_ldo8_eco_en : 1;  /* bit[4]  : LDO8进入低功耗ECO模式控制信号。
                                                          0：normal模式；
                                                          1：ECO模式。 */
        unsigned char  st_ldo8_eco_en  : 1;  /* bit[5]  : LDO8工作状态指示寄存器（指示数模接口处eco_en信号的状态
                                                          0：normal模式；
                                                          1：ECO模式。 */
        unsigned char  reserved_1      : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO8_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_en_START      (0)
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_en_END        (0)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_en_START       (1)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_en_END         (1)
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_eco_en_START  (4)
#define PMIC_MMW_LDO8_ONOFF_ECO_reg_ldo8_eco_en_END    (4)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_eco_en_START   (5)
#define PMIC_MMW_LDO8_ONOFF_ECO_st_ldo8_eco_en_END     (5)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_CORE_ONOFF_ECO_UNION
 结构说明  : LDO_CORE_ONOFF_ECO 寄存器结构定义。地址偏移量:0x013，初值:0x01，宽度:8
 寄存器说明: LDO_CORE开关和ECO使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_core_en     : 1;  /* bit[0]  : LDO_CORE使能信号。
                                                              0：关闭；
                                                              1：使能。
                                                              注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  st_ldo_core_en      : 1;  /* bit[1]  : LDO_CORE开关状态寄存器。
                                                              0：关闭；
                                                              1：开启。 */
        unsigned char  reserved_0          : 2;  /* bit[2-3]: 保留。 */
        unsigned char  reg_ldo_core_eco_en : 1;  /* bit[4]  : LDO_CORE进入低功耗ECO模式控制信号。
                                                              0：normal模式；
                                                              1：ECO模式。 */
        unsigned char  st_ldo_core_eco_en  : 1;  /* bit[5]  : LDO_CORE工作状态指示寄存器（指示数模接口处eco_en信号的状态
                                                              0：normal模式；
                                                              1：ECO模式。 */
        unsigned char  reserved_1          : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_CORE_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_en_START      (0)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_en_END        (0)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_en_START       (1)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_en_END         (1)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_eco_en_START  (4)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_reg_ldo_core_eco_en_END    (4)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_eco_en_START   (5)
#define PMIC_MMW_LDO_CORE_ONOFF_ECO_st_ldo_core_eco_en_END     (5)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF_RF_ONOFF_UNION
 结构说明  : LDO_BUF_RF_ONOFF 寄存器结构定义。地址偏移量:0x014，初值:0x00，宽度:8
 寄存器说明: LDO_BUF_RF开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_buf_rf_en : 1;  /* bit[0]  : LDO_BUF_RF使能信号。
                                                            0：关闭；
                                                            1：使能。
                                                            注：不对软件开放，该寄存器使用debug模式配置寄存器rfbuf_onoff_debug_mode进行加锁保护，进入debug模式后，才可以进行配置。 */
        unsigned char  st_ldo_buf_rf_en  : 1;  /* bit[1]  : LDO_BUF_RF开关状态寄存器。
                                                            0：关闭；
                                                            1：开启。 */
        unsigned char  reserved          : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_BUF_RF_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_BUF_RF_ONOFF_reg_ldo_buf_rf_en_START  (0)
#define PMIC_MMW_LDO_BUF_RF_ONOFF_reg_ldo_buf_rf_en_END    (0)
#define PMIC_MMW_LDO_BUF_RF_ONOFF_st_ldo_buf_rf_en_START   (1)
#define PMIC_MMW_LDO_BUF_RF_ONOFF_st_ldo_buf_rf_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF0_ONOFF_UNION
 结构说明  : LDO_BUF0_ONOFF 寄存器结构定义。地址偏移量:0x015，初值:0x01，宽度:8
 寄存器说明: LDO_BUF0开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_buf0_en : 1;  /* bit[0]  : LDO_BUF0使能信号。
                                                          0：关闭；
                                                          1：使能。
                                                          注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  st_ldo_buf0_en  : 1;  /* bit[1]  : LDO_BUF0开关状态寄存器。
                                                          0：关闭；
                                                          1：开启。 */
        unsigned char  reserved        : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_BUF0_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_BUF0_ONOFF_reg_ldo_buf0_en_START  (0)
#define PMIC_MMW_LDO_BUF0_ONOFF_reg_ldo_buf0_en_END    (0)
#define PMIC_MMW_LDO_BUF0_ONOFF_st_ldo_buf0_en_START   (1)
#define PMIC_MMW_LDO_BUF0_ONOFF_st_ldo_buf0_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF1_ONOFF_UNION
 结构说明  : LDO_BUF1_ONOFF 寄存器结构定义。地址偏移量:0x016，初值:0x01，宽度:8
 寄存器说明: LDO_BUF1开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_buf1_en : 1;  /* bit[0]  : LDO_BUF1使能信号。
                                                          0：关闭；
                                                          1：使能。
                                                          注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  st_ldo_buf1_en  : 1;  /* bit[1]  : LDO_BUF1开关状态寄存器。
                                                          0：关闭；
                                                          1：开启。 */
        unsigned char  reserved        : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_BUF1_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_BUF1_ONOFF_reg_ldo_buf1_en_START  (0)
#define PMIC_MMW_LDO_BUF1_ONOFF_reg_ldo_buf1_en_END    (0)
#define PMIC_MMW_LDO_BUF1_ONOFF_st_ldo_buf1_en_START   (1)
#define PMIC_MMW_LDO_BUF1_ONOFF_st_ldo_buf1_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF0_ONOFF_UNION
 结构说明  : LDO_RFBUF0_ONOFF 寄存器结构定义。地址偏移量:0x017，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF0开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_rfbuf0_en : 1;  /* bit[0]  : LDO_RFBUF0使能信号。
                                                            0：关闭；
                                                            1：使能。
                                                            注：不对软件开放，该寄存器使用debug模式配置寄存器rfbuf_onoff_debug_mode进行加锁保护，进入debug模式后，才可以进行配置。 */
        unsigned char  st_ldo_rfbuf0_en  : 1;  /* bit[1]  : LDO_RFBUF0开关状态寄存器。
                                                            0：关闭；
                                                            1：开启。 */
        unsigned char  reserved          : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF0_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF0_ONOFF_reg_ldo_rfbuf0_en_START  (0)
#define PMIC_MMW_LDO_RFBUF0_ONOFF_reg_ldo_rfbuf0_en_END    (0)
#define PMIC_MMW_LDO_RFBUF0_ONOFF_st_ldo_rfbuf0_en_START   (1)
#define PMIC_MMW_LDO_RFBUF0_ONOFF_st_ldo_rfbuf0_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF1_ONOFF_UNION
 结构说明  : LDO_RFBUF1_ONOFF 寄存器结构定义。地址偏移量:0x018，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF1开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_rfbuf1_en : 1;  /* bit[0]  : LDO_RFBUF1使能信号。
                                                            0：关闭；
                                                            1：使能。
                                                            注：不对软件开放，该寄存器使用debug模式配置寄存器rfbuf_onoff_debug_mode进行加锁保护，进入debug模式后，才可以进行配置。 */
        unsigned char  st_ldo_rfbuf1_en  : 1;  /* bit[1]  : LDO_RFBUF1开关状态寄存器。
                                                            0：关闭；
                                                            1：开启。 */
        unsigned char  reserved          : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF1_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF1_ONOFF_reg_ldo_rfbuf1_en_START  (0)
#define PMIC_MMW_LDO_RFBUF1_ONOFF_reg_ldo_rfbuf1_en_END    (0)
#define PMIC_MMW_LDO_RFBUF1_ONOFF_st_ldo_rfbuf1_en_START   (1)
#define PMIC_MMW_LDO_RFBUF1_ONOFF_st_ldo_rfbuf1_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF2_ONOFF_UNION
 结构说明  : LDO_RFBUF2_ONOFF 寄存器结构定义。地址偏移量:0x019，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF2开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_rfbuf2_en : 1;  /* bit[0]  : LDO_RFBUF2使能信号。
                                                            0：关闭；
                                                            1：使能。
                                                            注：不对软件开放，该寄存器使用debug模式配置寄存器rfbuf_onoff_debug_mode进行加锁保护，进入debug模式后，才可以进行配置。 */
        unsigned char  st_ldo_rfbuf2_en  : 1;  /* bit[1]  : LDO_RFBUF2开关状态寄存器。
                                                            0：关闭；
                                                            1：开启。 */
        unsigned char  reserved          : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF2_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF2_ONOFF_reg_ldo_rfbuf2_en_START  (0)
#define PMIC_MMW_LDO_RFBUF2_ONOFF_reg_ldo_rfbuf2_en_END    (0)
#define PMIC_MMW_LDO_RFBUF2_ONOFF_st_ldo_rfbuf2_en_START   (1)
#define PMIC_MMW_LDO_RFBUF2_ONOFF_st_ldo_rfbuf2_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF3_ONOFF_UNION
 结构说明  : LDO_RFBUF3_ONOFF 寄存器结构定义。地址偏移量:0x01A，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF3开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_rfbuf3_en : 1;  /* bit[0]  : LDO_RFBUF3使能信号。
                                                            0：关闭；
                                                            1：使能。
                                                            注：不对软件开放，该寄存器使用debug模式配置寄存器rfbuf_onoff_debug_mode进行加锁保护，进入debug模式后，才可以进行配置。 */
        unsigned char  st_ldo_rfbuf3_en  : 1;  /* bit[1]  : LDO_RFBUF3开关状态寄存器。
                                                            0：关闭；
                                                            1：开启。 */
        unsigned char  reserved          : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF3_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF3_ONOFF_reg_ldo_rfbuf3_en_START  (0)
#define PMIC_MMW_LDO_RFBUF3_ONOFF_reg_ldo_rfbuf3_en_END    (0)
#define PMIC_MMW_LDO_RFBUF3_ONOFF_st_ldo_rfbuf3_en_START   (1)
#define PMIC_MMW_LDO_RFBUF3_ONOFF_st_ldo_rfbuf3_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF4_ONOFF_UNION
 结构说明  : LDO_RFBUF4_ONOFF 寄存器结构定义。地址偏移量:0x01B，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF4开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_rfbuf4_en : 1;  /* bit[0]  : LDO_RFBUF4使能信号。
                                                            0：关闭；
                                                            1：使能。
                                                            注：不对软件开放，该寄存器使用debug模式配置寄存器rfbuf_onoff_debug_mode进行加锁保护，进入debug模式后，才可以进行配置。 */
        unsigned char  st_ldo_rfbuf4_en  : 1;  /* bit[1]  : LDO_RFBUF4开关状态寄存器。
                                                            0：关闭；
                                                            1：开启。 */
        unsigned char  reserved          : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF4_ONOFF_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF4_ONOFF_reg_ldo_rfbuf4_en_START  (0)
#define PMIC_MMW_LDO_RFBUF4_ONOFF_reg_ldo_rfbuf4_en_END    (0)
#define PMIC_MMW_LDO_RFBUF4_ONOFF_st_ldo_rfbuf4_en_START   (1)
#define PMIC_MMW_LDO_RFBUF4_ONOFF_st_ldo_rfbuf4_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_PMUH_ONOFF_UNION
 结构说明  : PMUH_ONOFF 寄存器结构定义。地址偏移量:0x01C，初值:0x01，宽度:8
 寄存器说明: PMUH开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_pmuh_en : 1;  /* bit[0]  : PMUH使能信号。
                                                      0：关闭；
                                                      1：使能。 
                                                      注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  st_pmuh_en  : 1;  /* bit[1]  : PMUH开关状态寄存器。
                                                      0：关闭；
                                                      1：开启。 */
        unsigned char  reserved    : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_PMUH_ONOFF_UNION;
#endif
#define PMIC_MMW_PMUH_ONOFF_reg_pmuh_en_START  (0)
#define PMIC_MMW_PMUH_ONOFF_reg_pmuh_en_END    (0)
#define PMIC_MMW_PMUH_ONOFF_st_pmuh_en_START   (1)
#define PMIC_MMW_PMUH_ONOFF_st_pmuh_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_CORE_ONOFF_UNION
 结构说明  : XO_CORE_ONOFF 寄存器结构定义。地址偏移量:0x01D，初值:0x01，宽度:8
 寄存器说明: XO_CORE开关使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_core_en : 1;  /* bit[0]  : XO_CORE使能信号。
                                                         0：关闭；
                                                         1：使能。 
                                                         注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  st_xo_core_en  : 1;  /* bit[1]  : XO_CORE开关状态寄存器。
                                                         0：关闭；
                                                         1：开启。 */
        unsigned char  reserved       : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_CORE_ONOFF_UNION;
#endif
#define PMIC_MMW_XO_CORE_ONOFF_reg_xo_core_en_START  (0)
#define PMIC_MMW_XO_CORE_ONOFF_reg_xo_core_en_END    (0)
#define PMIC_MMW_XO_CORE_ONOFF_st_xo_core_en_START   (1)
#define PMIC_MMW_XO_CORE_ONOFF_st_xo_core_en_END     (1)


/*****************************************************************************
 结构名    : PMIC_MMW_THSD_ONOFF_ECO_UNION
 结构说明  : THSD_ONOFF_ECO 寄存器结构定义。地址偏移量:0x01E，初值:0x01，宽度:8
 寄存器说明: THSD开关和ECO使能寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_thsd_en     : 1;  /* bit[0]  : THSD过温保护模块关闭/开关控制位。
                                                          0：关闭THSD过温保护模块；
                                                          1：开启THSD过温保护模块。
                                                          注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  st_thsd_en      : 1;  /* bit[1]  : THSD开关状态寄存器。
                                                          0：关闭；
                                                          1：开启。 */
        unsigned char  reserved_0      : 2;  /* bit[2-3]: 保留。 */
        unsigned char  reg_thsd_eco_en : 1;  /* bit[4]  : THSD进入ECO模式控制信号。
                                                          0：normal模式；
                                                          1：ECO模式。 */
        unsigned char  st_thsd_eco_en  : 1;  /* bit[5]  : THSD eco工作状态指示寄存器。
                                                          0：normal模式；
                                                          1：ECO模式。 */
        unsigned char  reserved_1      : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_THSD_ONOFF_ECO_UNION;
#endif
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_en_START      (0)
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_en_END        (0)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_en_START       (1)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_en_END         (1)
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_eco_en_START  (4)
#define PMIC_MMW_THSD_ONOFF_ECO_reg_thsd_eco_en_END    (4)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_eco_en_START   (5)
#define PMIC_MMW_THSD_ONOFF_ECO_st_thsd_eco_en_END     (5)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_UFS_EN_UNION
 结构说明  : CLK_UFS_EN 寄存器结构定义。地址偏移量:0x020，初值:0x01，宽度:8
 寄存器说明: UFS时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_ufs_en : 1;  /* bit[0]  : clk_ufs使能信号：
                                                        0：不使能；
                                                        1：使能。
                                                        注：默认是否使能可以通过OTP配置。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_UFS_EN_UNION;
#endif
#define PMIC_MMW_CLK_UFS_EN_reg_xo_ufs_en_START  (0)
#define PMIC_MMW_CLK_UFS_EN_reg_xo_ufs_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF0_EN_UNION
 结构说明  : CLK_RF0_EN 寄存器结构定义。地址偏移量:0x021，初值:0x00，宽度:8
 寄存器说明: RF0时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_rf0_en : 1;  /* bit[0]  : clk_rf0使能信号。
                                                        0：不使能；
                                                        1：使能。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF0_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF0_EN_reg_xo_rf0_en_START  (0)
#define PMIC_MMW_CLK_RF0_EN_reg_xo_rf0_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF1_EN_UNION
 结构说明  : CLK_RF1_EN 寄存器结构定义。地址偏移量:0x022，初值:0x00，宽度:8
 寄存器说明: RF1时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_rf1_en : 1;  /* bit[0]  : clk_rf1使能信号。
                                                        0：不使能；
                                                        1：使能。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF1_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF1_EN_reg_xo_rf1_en_START  (0)
#define PMIC_MMW_CLK_RF1_EN_reg_xo_rf1_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF2_EN_UNION
 结构说明  : CLK_RF2_EN 寄存器结构定义。地址偏移量:0x023，初值:0x00，宽度:8
 寄存器说明: RF2时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_rf2_en : 1;  /* bit[0]  : clk_rf2使能信号。
                                                        0：不使能；
                                                        1：使能。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF2_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF2_EN_reg_xo_rf2_en_START  (0)
#define PMIC_MMW_CLK_RF2_EN_reg_xo_rf2_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF3_EN_UNION
 结构说明  : CLK_RF3_EN 寄存器结构定义。地址偏移量:0x024，初值:0x00，宽度:8
 寄存器说明: RF3时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_rf3_en : 1;  /* bit[0]  : clk_rf3使能信号。
                                                        0：不使能；
                                                        1：使能。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF3_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF3_EN_reg_xo_rf3_en_START  (0)
#define PMIC_MMW_CLK_RF3_EN_reg_xo_rf3_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF4_EN_UNION
 结构说明  : CLK_RF4_EN 寄存器结构定义。地址偏移量:0x025，初值:0x00，宽度:8
 寄存器说明: RF4时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_rf4_en : 1;  /* bit[0]  : clk_rf4使能信号。
                                                        0：不使能；
                                                        1：使能。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF4_EN_UNION;
#endif
#define PMIC_MMW_CLK_RF4_EN_reg_xo_rf4_en_START  (0)
#define PMIC_MMW_CLK_RF4_EN_reg_xo_rf4_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SERDES_EN_UNION
 结构说明  : CLK_SERDES_EN 寄存器结构定义。地址偏移量:0x026，初值:0x01，宽度:8
 寄存器说明: SERDES时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_serdes_en : 1;  /* bit[0]  : clk_serdes使能信号。
                                                           0：不使能；
                                                           1：使能。
                                                           注：默认是否使能可以通过OTP配置。 */
        unsigned char  reserved         : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SERDES_EN_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_EN_reg_xo_serdes_en_START  (0)
#define PMIC_MMW_CLK_SERDES_EN_reg_xo_serdes_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI0_EN_UNION
 结构说明  : CLK_WIFI0_EN 寄存器结构定义。地址偏移量:0x027，初值:0x00，宽度:8
 寄存器说明: WIFI0时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_wifi0_en : 1;  /* bit[0]  : clk_wifi0使能信号。
                                                          0：不使能；
                                                          1：使能。
                                                          注：默认是否使能可以通过OTP配置。 */
        unsigned char  reserved        : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI0_EN_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_EN_reg_xo_wifi0_en_START  (0)
#define PMIC_MMW_CLK_WIFI0_EN_reg_xo_wifi0_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI1_EN_UNION
 结构说明  : CLK_WIFI1_EN 寄存器结构定义。地址偏移量:0x028，初值:0x00，宽度:8
 寄存器说明: WIFI1时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_wifi1_en : 1;  /* bit[0]  : clk_wifi1使能信号。
                                                          0：不使能；
                                                          1：使能。
                                                          注：默认是否使能可以通过OTP配置。 */
        unsigned char  reserved        : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI1_EN_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_EN_reg_xo_wifi1_en_START  (0)
#define PMIC_MMW_CLK_WIFI1_EN_reg_xo_wifi1_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_NFC_EN_UNION
 结构说明  : CLK_NFC_EN 寄存器结构定义。地址偏移量:0x029，初值:0x00，宽度:8
 寄存器说明: NFC时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_nfc_en : 1;  /* bit[0]  : clk_nfc使能信号。
                                                        0：不使能；
                                                        1：使能。
                                                        注：默认是否使能可以通过OTP配置。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_NFC_EN_UNION;
#endif
#define PMIC_MMW_CLK_NFC_EN_reg_xo_nfc_en_START  (0)
#define PMIC_MMW_CLK_NFC_EN_reg_xo_nfc_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SYS_EN_UNION
 结构说明  : CLK_SYS_EN 寄存器结构定义。地址偏移量:0x02A，初值:0x01，宽度:8
 寄存器说明: SYS时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_sys_en : 1;  /* bit[0]  : clk_sys使能信号。
                                                        0：不使能；
                                                        1：使能。
                                                        注：默认是否使能可以通过OTP配置。 */
        unsigned char  reserved      : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SYS_EN_UNION;
#endif
#define PMIC_MMW_CLK_SYS_EN_reg_xo_sys_en_START  (0)
#define PMIC_MMW_CLK_SYS_EN_reg_xo_sys_en_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_PMU_EN_UNION
 结构说明  : CLK_PMU_EN 寄存器结构定义。地址偏移量:0x02B，初值:0x5A，宽度:8
 寄存器说明: 送给主PMU的时钟开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_xo_pmu_en : 8;  /* bit[0-7]: clk_pmu_out使能信号。
                                                        8'hAC：不使能；
                                                        8'h5A：使能（默认）。 */
    } reg;
} PMIC_MMW_CLK_PMU_EN_UNION;
#endif
#define PMIC_MMW_CLK_PMU_EN_reg_xo_pmu_en_START  (0)
#define PMIC_MMW_CLK_PMU_EN_reg_xo_pmu_en_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_UNION
 结构说明  : OSC32K_SYS_ONOFF_CTRL 寄存器结构定义。地址偏移量:0x02C，初值:0x01，宽度:8
 寄存器说明: 32kHz时钟SYS输出使能控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  en_32k_sys : 1;  /* bit[0]  : CLK32_SYS 输出使能位。
                                                     0：不输出；
                                                     1：32kHz时钟输出。 */
        unsigned char  reserved   : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_UNION;
#endif
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_en_32k_sys_START  (0)
#define PMIC_MMW_OSC32K_SYS_ONOFF_CTRL_en_32k_sys_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO0_VSET_UNION
 结构说明  : LDO0_VSET 寄存器结构定义。地址偏移量:0x02D，初值:0x00，宽度:8
 寄存器说明: LDO0调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo0_vset : 4;  /* bit[0-3]: LDO0 输出电压设置：0.91~1.06 10mV/step，默认0.91V。 
                                                    0x0:0.91V； 
                                                    0x1:0.92V； 
                                                    0x2:0.93V； 
                                                    0x3:0.94V； 
                                                    0x4:0.95V； 
                                                    0x5:0.96V； 
                                                    0x6:0.97V； 
                                                    0x7:0.98V； 
                                                    0x8:0.99V； 
                                                    0x9:1.0V； 
                                                    0xA:1.01V； 
                                                    0xB:1.02V； 
                                                    0xC:1.03V； 
                                                    0xD:1.04V； 
                                                    0xE:1.05V； 
                                                    0xF:1.06V； */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO0_VSET_UNION;
#endif
#define PMIC_MMW_LDO0_VSET_ldo0_vset_START  (0)
#define PMIC_MMW_LDO0_VSET_ldo0_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO1_VSET_UNION
 结构说明  : LDO1_VSET 寄存器结构定义。地址偏移量:0x02E，初值:0x00，宽度:8
 寄存器说明: LDO1调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo1_vset : 4;  /* bit[0-3]: LDO1 输出电压设置：0.91~1.06 10mV/step，默认0.91V。 
                                                    0x0:0.91V； 
                                                    0x1:0.92V； 
                                                    0x2:0.93V； 
                                                    0x3:0.94V； 
                                                    0x4:0.95V； 
                                                    0x5:0.96V； 
                                                    0x6:0.97V； 
                                                    0x7:0.98V； 
                                                    0x8:0.99V； 
                                                    0x9:1.0V； 
                                                    0xA:1.01V； 
                                                    0xB:1.02V； 
                                                    0xC:1.03V； 
                                                    0xD:1.04V； 
                                                    0xE:1.05V； 
                                                    0xF:1.06V； */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO1_VSET_UNION;
#endif
#define PMIC_MMW_LDO1_VSET_ldo1_vset_START  (0)
#define PMIC_MMW_LDO1_VSET_ldo1_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO2_VSET_UNION
 结构说明  : LDO2_VSET 寄存器结构定义。地址偏移量:0x02F，初值:0x00，宽度:8
 寄存器说明: LDO2调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo2_vset : 4;  /* bit[0-3]: LDO2 输出电压设置：0.91~1.06 10mV/step，默认0.91V。 
                                                    0x0:0.91V； 
                                                    0x1:0.92V； 
                                                    0x2:0.93V； 
                                                    0x3:0.94V； 
                                                    0x4:0.95V； 
                                                    0x5:0.96V； 
                                                    0x6:0.97V； 
                                                    0x7:0.98V； 
                                                    0x8:0.99V； 
                                                    0x9:1.0V； 
                                                    0xA:1.01V； 
                                                    0xB:1.02V； 
                                                    0xC:1.03V； 
                                                    0xD:1.04V； 
                                                    0xE:1.05V； 
                                                    0xF:1.06V； */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO2_VSET_UNION;
#endif
#define PMIC_MMW_LDO2_VSET_ldo2_vset_START  (0)
#define PMIC_MMW_LDO2_VSET_ldo2_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO3_VSET_UNION
 结构说明  : LDO3_VSET 寄存器结构定义。地址偏移量:0x030，初值:0x00，宽度:8
 寄存器说明: LDO3调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo3_vset : 4;  /* bit[0-3]: LDO3 输出电压设置，1.15~1.3 10mV/step，默认1.15V。
                                                    0x0:1.15V； 
                                                    0x1:1.16V； 
                                                    0x2:1.17V； 
                                                    0x3:1.18V； 
                                                    0x4:1.19V； 
                                                    0x5:1.20V； 
                                                    0x6:1.21V； 
                                                    0x7:1.22V； 
                                                    0x8:1.23V； 
                                                    0x9:1.24V； 
                                                    0xA:1.25V； 
                                                    0xB:1.26V； 
                                                    0xC:1.27V； 
                                                    0xD:1.28V； 
                                                    0xE:1.29V； 
                                                    0xF:1.3V；  */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO3_VSET_UNION;
#endif
#define PMIC_MMW_LDO3_VSET_ldo3_vset_START  (0)
#define PMIC_MMW_LDO3_VSET_ldo3_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO4_VSET_UNION
 结构说明  : LDO4_VSET 寄存器结构定义。地址偏移量:0x031，初值:0x00，宽度:8
 寄存器说明: LDO4调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo4_vset : 4;  /* bit[0-3]: LDO4 输出电压设置，1.15~1.3 10mV/step，默认1.15V。
                                                    0x0:1.15V； 
                                                    0x1:1.16V； 
                                                    0x2:1.17V； 
                                                    0x3:1.18V； 
                                                    0x4:1.19V； 
                                                    0x5:1.20V； 
                                                    0x6:1.21V； 
                                                    0x7:1.22V； 
                                                    0x8:1.23V； 
                                                    0x9:1.24V； 
                                                    0xA:1.25V； 
                                                    0xB:1.26V； 
                                                    0xC:1.27V； 
                                                    0xD:1.28V； 
                                                    0xE:1.29V； 
                                                    0xF:1.3V；  */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO4_VSET_UNION;
#endif
#define PMIC_MMW_LDO4_VSET_ldo4_vset_START  (0)
#define PMIC_MMW_LDO4_VSET_ldo4_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO5_VSET_UNION
 结构说明  : LDO5_VSET 寄存器结构定义。地址偏移量:0x032，初值:0x00，宽度:8
 寄存器说明: LDO5调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo5_vset : 4;  /* bit[0-3]: LDO5 输出电压设置，1.75~1.90 10mV/step，默认电压1.75V。
                                                    0x0:1.75V； 
                                                    0x1:1.76V； 
                                                    0x2:1.77V； 
                                                    0x3:1.78V； 
                                                    0x4:1.79V； 
                                                    0x5:1.8V； 
                                                    0x6:1.81V； 
                                                    0x7:1.82V； 
                                                    0x8:1.83V； 
                                                    0x9:1.84V； 
                                                    0xA:1.85V； 
                                                    0xB:1.86V； 
                                                    0xC:1.87V； 
                                                    0xD:1.88V； 
                                                    0xE:1.89V； 
                                                    0xF:1.90V； */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO5_VSET_UNION;
#endif
#define PMIC_MMW_LDO5_VSET_ldo5_vset_START  (0)
#define PMIC_MMW_LDO5_VSET_ldo5_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO6_VSET_UNION
 结构说明  : LDO6_VSET 寄存器结构定义。地址偏移量:0x033，初值:0x00，宽度:8
 寄存器说明: LDO6调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo6_vset : 4;  /* bit[0-3]: LDO6 输出电压设置，1.75~1.90 10mV/step，默认电压1.75V。
                                                    0x0:1.75V； 
                                                    0x1:1.76V； 
                                                    0x2:1.77V； 
                                                    0x3:1.78V； 
                                                    0x4:1.79V； 
                                                    0x5:1.8V； 
                                                    0x6:1.81V； 
                                                    0x7:1.82V； 
                                                    0x8:1.83V； 
                                                    0x9:1.84V； 
                                                    0xA:1.85V； 
                                                    0xB:1.86V； 
                                                    0xC:1.87V； 
                                                    0xD:1.88V； 
                                                    0xE:1.89V； 
                                                    0xF:1.90V； */
        unsigned char  reserved  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO6_VSET_UNION;
#endif
#define PMIC_MMW_LDO6_VSET_ldo6_vset_START  (0)
#define PMIC_MMW_LDO6_VSET_ldo6_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO7_VSET_UNION
 结构说明  : LDO7_VSET 寄存器结构定义。地址偏移量:0x034，初值:0x00，宽度:8
 寄存器说明: LDO7调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo7_vset : 5;  /* bit[0-4]: LDO7 输出电压设置，0.5~1.12V，32Step，20mV per Step，默认电压0.5V。
                                                    0x0:0.5V； 0x8:0.66V； 0x10:0.82V； 0x18:0.98V；
                                                    0x1:0.52V； 0x9:0.68V； 0x11:0.84V； 0x19:1.0V；
                                                    0x2:0.54V； 0xA:0.7V； 0x12:0.86V； 0x1A:1.02V；
                                                    0x3:0.56V； 0xB:0.72V； 0x13:0.88V； 0x1B:1.04V；
                                                    0x4:0.58V； 0xC:0.74V； 0x14:0.9V； 0x1C:1.06V；
                                                    0x5:0.6V； 0xD:0.76V； 0x15:0.92V； 0x1D:1.08V；
                                                    0x6:0.62V； 0xE:0.78V； 0x16:0.94V； 0x1E:1.1V；
                                                    0x7:0.64V； 0xF:0.8V； 0x17:0.96V； 0x1F:1.12V； */
        unsigned char  reserved  : 3;  /* bit[5-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO7_VSET_UNION;
#endif
#define PMIC_MMW_LDO7_VSET_ldo7_vset_START  (0)
#define PMIC_MMW_LDO7_VSET_ldo7_vset_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO8_VSET_UNION
 结构说明  : LDO8_VSET 寄存器结构定义。地址偏移量:0x035，初值:0x00，宽度:8
 寄存器说明: LDO8调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo8_vset : 5;  /* bit[0-4]: LDO8 输出电压设置，0.5~1.12V，32Step，20mV per Step，默认电压0.5V。
                                                    0x0:0.5V； 0x8:0.66V； 0x10:0.82V； 0x18:0.98V；
                                                    0x1:0.52V； 0x9:0.68V； 0x11:0.84V； 0x19:1.0V；
                                                    0x2:0.54V； 0xA:0.7V； 0x12:0.86V； 0x1A:1.02V；
                                                    0x3:0.56V； 0xB:0.72V； 0x13:0.88V； 0x1B:1.04V；
                                                    0x4:0.58V； 0xC:0.74V； 0x14:0.9V； 0x1C:1.06V；
                                                    0x5:0.6V； 0xD:0.76V； 0x15:0.92V； 0x1D:1.08V；
                                                    0x6:0.62V； 0xE:0.78V； 0x16:0.94V； 0x1E:1.1V；
                                                    0x7:0.64V； 0xF:0.8V； 0x17:0.96V； 0x1F:1.12V； */
        unsigned char  reserved  : 3;  /* bit[5-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO8_VSET_UNION;
#endif
#define PMIC_MMW_LDO8_VSET_ldo8_vset_START  (0)
#define PMIC_MMW_LDO8_VSET_ldo8_vset_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_CORE_VSET_UNION
 结构说明  : LDO_CORE_VSET 寄存器结构定义。地址偏移量:0x036，初值:0x07，宽度:8
 寄存器说明: LDO_CORE调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_core_vset_cfg : 4;  /* bit[0-3]: normal模式下 LDO_CORE输出电压调节，1.15/1.2/1.25/1.3/1.35/1.4/1.5/1.7，默认1.7V。
                                                            0x0: 1.15V； 0x8：1.15V；
                                                            0x1: 1.2V； 0x9：1.2V；
                                                            0x2: 1.25V； 0xA：1.25V；
                                                            0x3：1.3V； 0xB：1.3V；
                                                            0x4：1.35V； 0xC：1.35V；
                                                            0x5：1.4V； 0xD：1.4V； 
                                                            0x6：1.5V； 0xE：1.5V；
                                                            0x7：1.7V； 0xF：1.7V */
        unsigned char  reserved          : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_CORE_VSET_UNION;
#endif
#define PMIC_MMW_LDO_CORE_VSET_ldo_core_vset_cfg_START  (0)
#define PMIC_MMW_LDO_CORE_VSET_ldo_core_vset_cfg_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_CORE_VSET_ECO_UNION
 结构说明  : LDO_CORE_VSET_ECO 寄存器结构定义。地址偏移量:0x037，初值:0x07，宽度:8
 寄存器说明: LDO_CORE ECO调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_core_vset_eco_cfg : 4;  /* bit[0-3]: eco模式下LDO_CORE输出电压调节：
                                                                1.15/1.2/1.25/1.3/1.35/1.4/1.5/1.7，默认1.7V。
                                                                0x0: 1.15V； 0x8：1.15V；
                                                                0x1: 1.2V； 0x9：1.2V；
                                                                0x2: 1.25V； 0xA：1.25V；
                                                                0x3：1.3V； 0xB：1.3V；
                                                                0x4：1.35V； 0xC：1.35V；
                                                                0x5：1.4V； 0xD：1.4V； 
                                                                0x6：1.5V； 0xE：1.5V；
                                                                0x7：1.7V； 0xF：1.7V */
        unsigned char  reserved              : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_CORE_VSET_ECO_UNION;
#endif
#define PMIC_MMW_LDO_CORE_VSET_ECO_ldo_core_vset_eco_cfg_START  (0)
#define PMIC_MMW_LDO_CORE_VSET_ECO_ldo_core_vset_eco_cfg_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF_RF_VSET_UNION
 结构说明  : LDO_BUF_RF_VSET 寄存器结构定义。地址偏移量:0x038，初值:0x00，宽度:8
 寄存器说明: LDO_BUF_RF调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf_vset : 4;  /* bit[0-3]: LDO_BUF_RF输出电压调节，1.1~1.85V，16 step，50mV per Step，默认1.1V。
                                                         0x0: 1.1V； 0x8：1.5V；
                                                         0x1: 1.15V； 0x9: 1.55V； 
                                                         0x2: 1.2V； 0xA：1.6V；
                                                         0x3：1.25V； 0xB：1.65V； 
                                                         0x4：1.3V； 0xC: 1.7V；
                                                         0x5：1.35V； 0xD：1.75V； 
                                                         0x6：1.4V； 0xE：1.8V；
                                                         0x7：1.45V； 0xF：1.85V；  */
        unsigned char  reserved       : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_BUF_RF_VSET_UNION;
#endif
#define PMIC_MMW_LDO_BUF_RF_VSET_ldo_rfbuf_vset_START  (0)
#define PMIC_MMW_LDO_BUF_RF_VSET_ldo_rfbuf_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF0_VSET_UNION
 结构说明  : LDO_BUF0_VSET 寄存器结构定义。地址偏移量:0x039，初值:0x0E，宽度:8
 寄存器说明: LDO_BUF0调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_buf0_vset : 4;  /* bit[0-3]: LDO_BUF0输出电压设置，1.1~1.85V，16 step，50mV per Step
                                                        默认1.8V。
                                                        0x0: 1.1V； 0x8：1.5V；
                                                        0x1: 1.15V； 0x9: 1.55V； 
                                                        0x2: 1.2V； 0xA：1.6V；
                                                        0x3：1.25V； 0xB：1.65V； 
                                                        0x4：1.3V； 0xC: 1.7V；
                                                        0x5：1.35V； 0xD：1.75V； 
                                                        0x6：1.4V； 0xE：1.8V；
                                                        0x7：1.45V； 0xF：1.85V；  */
        unsigned char  reserved      : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_BUF0_VSET_UNION;
#endif
#define PMIC_MMW_LDO_BUF0_VSET_ldo_buf0_vset_START  (0)
#define PMIC_MMW_LDO_BUF0_VSET_ldo_buf0_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF1_VSET_UNION
 结构说明  : LDO_BUF1_VSET 寄存器结构定义。地址偏移量:0x03A，初值:0x03，宽度:8
 寄存器说明: LDO_BUF1调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_buf1_vset : 4;  /* bit[0-3]: LDO_BUF0输出电压设置，1.05~1.8V，16 step，50mV per Step，默认1.2V。
                                                        0x0: 1.05V； 0x8：1.45V；
                                                        0x1: 1.1V； 0x9: 1.5V； 
                                                        0x2: 1.15V； 0xA：1.55V；
                                                        0x3：1.2V； 0xB：1.6V； 
                                                        0x4：1.25V； 0xC: 1.65V；
                                                        0x5：1.3V； 0xD：1.7V； 
                                                        0x6：1.35V； 0xE：1.85V；
                                                        0x7：1.4V； 0xF：1.8V；  */
        unsigned char  reserved      : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_BUF1_VSET_UNION;
#endif
#define PMIC_MMW_LDO_BUF1_VSET_ldo_buf1_vset_START  (0)
#define PMIC_MMW_LDO_BUF1_VSET_ldo_buf1_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF0_VSET_UNION
 结构说明  : LDO_RFBUF0_VSET 寄存器结构定义。地址偏移量:0x03B，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF0调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf0_vset : 4;  /* bit[0-3]: LDO_RFBUF0输出电压设置，1.05~1.8V，16 step，50mV per Step，默认1.05V。 
                                                          0x0: 1.05V； 0x8：1.45V；
                                                          0x1: 1.1V； 0x9: 1.5V； 
                                                          0x2: 1.15V； 0xA：1.55V；
                                                          0x3：1.2V； 0xB：1.6V； 
                                                          0x4：1.25V； 0xC: 1.65V；
                                                          0x5：1.3V； 0xD：1.7V； 
                                                          0x6：1.35V； 0xE：1.75V；
                                                          0x7：1.4V； 0xF：1.8V；  */
        unsigned char  reserved        : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF0_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF0_VSET_ldo_rfbuf0_vset_START  (0)
#define PMIC_MMW_LDO_RFBUF0_VSET_ldo_rfbuf0_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF1_VSET_UNION
 结构说明  : LDO_RFBUF1_VSET 寄存器结构定义。地址偏移量:0x03C，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF1调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf1_vset : 4;  /* bit[0-3]: LDO_RFBUF1输出电压设置，1.05~1.8V，16 step，50mV per Step，默认1.05V。 
                                                          0x0: 1.05V； 0x8：1.45V；
                                                          0x1: 1.1V； 0x9: 1.5V； 
                                                          0x2: 1.15V； 0xA：1.55V；
                                                          0x3：1.2V； 0xB：1.6V； 
                                                          0x4：1.25V； 0xC: 1.65V；
                                                          0x5：1.3V； 0xD：1.7V； 
                                                          0x6：1.35V； 0xE：1.75V；
                                                          0x7：1.4V； 0xF：1.8V；  */
        unsigned char  reserved        : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF1_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF1_VSET_ldo_rfbuf1_vset_START  (0)
#define PMIC_MMW_LDO_RFBUF1_VSET_ldo_rfbuf1_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF2_VSET_UNION
 结构说明  : LDO_RFBUF2_VSET 寄存器结构定义。地址偏移量:0x03D，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF2调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf2_vset : 4;  /* bit[0-3]: LDO_RFBUF2输出电压设置，1.05~1.8V，16 step，50mV per Step，默认1.05V。 
                                                          0x0: 1.05V； 0x8：1.45V；
                                                          0x1: 1.1V； 0x9: 1.5V； 
                                                          0x2: 1.15V； 0xA：1.55V；
                                                          0x3：1.2V； 0xB：1.6V； 
                                                          0x4：1.25V； 0xC: 1.65V；
                                                          0x5：1.3V； 0xD：1.7V； 
                                                          0x6：1.35V； 0xE：1.75V；
                                                          0x7：1.4V； 0xF：1.8V；  */
        unsigned char  reserved        : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF2_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF2_VSET_ldo_rfbuf2_vset_START  (0)
#define PMIC_MMW_LDO_RFBUF2_VSET_ldo_rfbuf2_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF3_VSET_UNION
 结构说明  : LDO_RFBUF3_VSET 寄存器结构定义。地址偏移量:0x03E，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF3调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf3_vset : 4;  /* bit[0-3]: LDO_RFBUF3输出电压设置，1.05~1.8V，16 step，50mV per Step，默认1.05V。 
                                                          0x0: 1.05V； 0x8：1.45V；
                                                          0x1: 1.1V； 0x9: 1.5V； 
                                                          0x2: 1.15V； 0xA：1.55V；
                                                          0x3：1.2V； 0xB：1.6V； 
                                                          0x4：1.25V； 0xC: 1.65V；
                                                          0x5：1.3V； 0xD：1.7V； 
                                                          0x6：1.35V； 0xE：1.75V；
                                                          0x7：1.4V； 0xF：1.8V；  */
        unsigned char  reserved        : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF3_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF3_VSET_ldo_rfbuf3_vset_START  (0)
#define PMIC_MMW_LDO_RFBUF3_VSET_ldo_rfbuf3_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF4_VSET_UNION
 结构说明  : LDO_RFBUF4_VSET 寄存器结构定义。地址偏移量:0x03F，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF4调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf4_vset : 4;  /* bit[0-3]: LDO_RFBUF4输出电压设置，1.05~1.8V，16 step，50mV per Step，默认1.05V。 
                                                          0x0: 1.05V； 0x8：1.45V；
                                                          0x1: 1.1V； 0x9: 1.5V； 
                                                          0x2: 1.15V； 0xA：1.55V；
                                                          0x3：1.2V； 0xB：1.6V； 
                                                          0x4：1.25V； 0xC: 1.65V；
                                                          0x5：1.3V； 0xD：1.7V； 
                                                          0x6：1.35V； 0xE：1.75V；
                                                          0x7：1.4V； 0xF：1.8V；  */
        unsigned char  reserved        : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_RFBUF4_VSET_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF4_VSET_ldo_rfbuf4_vset_START  (0)
#define PMIC_MMW_LDO_RFBUF4_VSET_ldo_rfbuf4_vset_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_PMUH_VSET_UNION
 结构说明  : LDO_PMUH_VSET 寄存器结构定义。地址偏移量:0x040，初值:0x02，宽度:8
 寄存器说明: LDO_PMUH调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  pmuh_vset : 3;  /* bit[0-2]: PMUH输出电压设置。
                                                    000：2.6V；
                                                    001：2.7V；
                                                    010：2.8V；
                                                    011：2.9V；
                                                    100: 3.0V；
                                                    101: 3.0V；
                                                    110: 3.0V；
                                                    111: 3.0V；
                                                    注：译码在模拟处理，数字对配置值不做处理。 */
        unsigned char  reserved  : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_PMUH_VSET_UNION;
#endif
#define PMIC_MMW_LDO_PMUH_VSET_pmuh_vset_START  (0)
#define PMIC_MMW_LDO_PMUH_VSET_pmuh_vset_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_CORE_CURR_UNION
 结构说明  : XO_CORE_CURR 寄存器结构定义。地址偏移量:0x041，初值:0x00，宽度:8
 寄存器说明: XO_CORE电流调节寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_core_curr_cfg : 4;  /* bit[0-3]: normal模式下xo core的电流配置。
                                                           1111:0.1mA 1110:0.2mA 1101:0.3mA 1100:0.4mA
                                                           1011:0.5mA 1010:0.6mA 1001:0.7mA 1000:0.8mA
                                                           0111:0.9mA 0110:1mA 0101:1.1mA 0100:1.2mA
                                                           0011:1.3mA 0010:1.4mA 0001:1.5mA 0000:1.6mA
                                                           注：默认值通过OTP选择，XO启动后软件不可配置该寄存器，按照默认值工作。该寄存器使用debug模式配置寄存器进行加锁保护，进入debug模式后，才可以进行动态配置。 */
        unsigned char  reserved         : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_CORE_CURR_UNION;
#endif
#define PMIC_MMW_XO_CORE_CURR_xo_core_curr_cfg_START  (0)
#define PMIC_MMW_XO_CORE_CURR_xo_core_curr_cfg_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_CORE_CURR_ECO_UNION
 结构说明  : XO_CORE_CURR_ECO 寄存器结构定义。地址偏移量:0x042，初值:0x00，宽度:8
 寄存器说明: XO_CORE ECO电流调节寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_core_curr_eco_cfg : 4;  /* bit[0-3]: eco模式下xo core的电流配置。
                                                               1111:0.1mA 1110:0.2mA 1101:0.3mA 1100:0.4mA
                                                               1011:0.5mA 1010:0.6mA 1001:0.7mA 1000:0.8mA
                                                               0111:0.9mA 0110:1mA 0101:1.1mA 0100:1.2mA
                                                               0011:1.3mA 0010:1.4mA 0001:1.5mA 0000:1.6mA
                                                               注：默认值通过OTP选择，XO启动后软件不可配置该寄存器，按照默认值工作。该寄存器使用debug模式配置寄存器进行加锁保护，进入debug模式后，才可以进行动态配置。 */
        unsigned char  reserved             : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_CORE_CURR_ECO_UNION;
#endif
#define PMIC_MMW_XO_CORE_CURR_ECO_xo_core_curr_eco_cfg_START  (0)
#define PMIC_MMW_XO_CORE_CURR_ECO_xo_core_curr_eco_cfg_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_INBUFF_CURR_UNION
 结构说明  : XO_INBUFF_CURR 寄存器结构定义。地址偏移量:0x043，初值:0x00，宽度:8
 寄存器说明: XO_INBUFF电流调节寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_inbuff_curr_cfg : 3;  /* bit[0-2]: normal模式下xo input的电流配置。
                                                             表示xo input的并联个数，000~111并联个数依次为1/4/7/10/13/16/19/22
                                                             注：默认值通过OTP选择，XO启动后软件不可配置该寄存器，按照默认值工作。该寄存器使用debug模式配置寄存器进行加锁保护，进入debug模式后，才可以进行动态配置。 */
        unsigned char  reserved           : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_INBUFF_CURR_UNION;
#endif
#define PMIC_MMW_XO_INBUFF_CURR_xo_inbuff_curr_cfg_START  (0)
#define PMIC_MMW_XO_INBUFF_CURR_xo_inbuff_curr_cfg_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_INBUFF_CURR_ECO_UNION
 结构说明  : XO_INBUFF_CURR_ECO 寄存器结构定义。地址偏移量:0x044，初值:0x00，宽度:8
 寄存器说明: XO_INBUFF ECO电流调节寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_inbuff_curr_eco_cfg : 3;  /* bit[0-2]: eco模式下xo input的电流配置。
                                                                 表示xo input的并联个数，000~111并联个数依次为1/4/7/10/13/16/19/22
                                                                 注：默认值通过OTP选择，XO启动后软件不可配置该寄存器，按照默认值工作。该寄存器使用debug模式配置寄存器进行加锁保护，进入debug模式后，才可以进行动态配置。 */
        unsigned char  reserved               : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_INBUFF_CURR_ECO_UNION;
#endif
#define PMIC_MMW_XO_INBUFF_CURR_ECO_xo_inbuff_curr_eco_cfg_START  (0)
#define PMIC_MMW_XO_INBUFF_CURR_ECO_xo_inbuff_curr_eco_cfg_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO0_CFG_REG_0_UNION
 结构说明  : LDO0_CFG_REG_0 寄存器结构定义。地址偏移量:0x045，初值:0x60，宽度:8
 寄存器说明: LDO0_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo0_ocpd_vos_set : 1;  /* bit[0-0]: ldo0的ocpd环路offset调节 */
        unsigned char  ldo0_ocpd_set     : 2;  /* bit[1-2]: LDO0 OCPD调节 */
        unsigned char  ldo0_ocp_enn      : 1;  /* bit[3-3]: ldo0限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  ldo0_comp         : 3;  /* bit[4-6]: ldo0的补偿调节 */
        unsigned char  ldo0_bw_en        : 1;  /* bit[7-7]: ldo0 buffer电路电流的调节使能,1表示不随负载变化，0表示随负载变化 */
    } reg;
} PMIC_MMW_LDO0_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_vos_set_START  (0)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_vos_set_END    (0)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_set_START      (1)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocpd_set_END        (2)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocp_enn_START       (3)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_ocp_enn_END         (3)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_comp_START          (4)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_comp_END            (6)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_bw_en_START         (7)
#define PMIC_MMW_LDO0_CFG_REG_0_ldo0_bw_en_END           (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO0_CFG_REG_1_UNION
 结构说明  : LDO0_CFG_REG_1 寄存器结构定义。地址偏移量:0x046，初值:0x06，宽度:8
 寄存器说明: LDO0_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo0_psrr1_set   : 3;  /* bit[0-2]: ldo0 PSRR 提升电路1的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  ldo0_psrr_en     : 1;  /* bit[3-3]: ldo0 PSRR 提升电路使能信号，1代表使能，0代表关闭 */
        unsigned char  ldo0_ocps_set    : 2;  /* bit[4-5]: LDO0 OCPS调节 */
        unsigned char  ldo0_ocps_en     : 1;  /* bit[6-6]: ldo0的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo0_ocps_bw_set : 1;  /* bit[7-7]: ldo0的ocps环路稳定性调节 */
    } reg;
} PMIC_MMW_LDO0_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr1_set_START    (0)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr1_set_END      (2)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr_en_START      (3)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_psrr_en_END        (3)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_set_START     (4)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_set_END       (5)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_en_START      (6)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_en_END        (6)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_bw_set_START  (7)
#define PMIC_MMW_LDO0_CFG_REG_1_ldo0_ocps_bw_set_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO0_CFG_REG_2_UNION
 结构说明  : LDO0_CFG_REG_2 寄存器结构定义。地址偏移量:0x047，初值:0x01，宽度:8
 寄存器说明: LDO0_配置寄存器_2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo0_vrset     : 3;  /* bit[0-2]: ldo0负载调整率补偿调整，5mV / Step */
        unsigned char  ldo0_vgpr_en   : 1;  /* bit[3-3]: ldo0的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  ldo0_psrr2_set : 3;  /* bit[4-6]: ldo0 PSRR 提升电路2的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO0_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vrset_START      (0)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vrset_END        (2)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vgpr_en_START    (3)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_vgpr_en_END      (3)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_psrr2_set_START  (4)
#define PMIC_MMW_LDO0_CFG_REG_2_ldo0_psrr2_set_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO1_CFG_REG_0_UNION
 结构说明  : LDO1_CFG_REG_0 寄存器结构定义。地址偏移量:0x048，初值:0x60，宽度:8
 寄存器说明: LDO1_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo1_ocpd_vos_set : 1;  /* bit[0-0]: ldo1的ocpd环路offset调节 */
        unsigned char  ldo1_ocpd_set     : 2;  /* bit[1-2]: ldo1 OCPD调节 */
        unsigned char  ldo1_ocp_enn      : 1;  /* bit[3-3]: LDO1限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  ldo1_comp         : 3;  /* bit[4-6]: ldo1的补偿调节 */
        unsigned char  ldo1_bw_en        : 1;  /* bit[7-7]: ldo1 buffer电路电流的调节使能,1表示不随负载变化，0表示随负载变化 */
    } reg;
} PMIC_MMW_LDO1_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_vos_set_START  (0)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_vos_set_END    (0)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_set_START      (1)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocpd_set_END        (2)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocp_enn_START       (3)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_ocp_enn_END         (3)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_comp_START          (4)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_comp_END            (6)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_bw_en_START         (7)
#define PMIC_MMW_LDO1_CFG_REG_0_ldo1_bw_en_END           (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO1_CFG_REG_1_UNION
 结构说明  : LDO1_CFG_REG_1 寄存器结构定义。地址偏移量:0x049，初值:0x06，宽度:8
 寄存器说明: LDO1_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo1_psrr1_set   : 3;  /* bit[0-2]: ldo1 PSRR 提升电路1的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  ldo1_psrr_en     : 1;  /* bit[3-3]: ldo1 PSRR 提升电路使能信号，1代表使能，0代表关闭 */
        unsigned char  ldo1_ocps_set    : 2;  /* bit[4-5]: ldo1 OCPS调节 */
        unsigned char  ldo1_ocps_en     : 1;  /* bit[6-6]: ldo1的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo1_ocps_bw_set : 1;  /* bit[7-7]: ldo1的ocps环路稳定性调节 */
    } reg;
} PMIC_MMW_LDO1_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr1_set_START    (0)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr1_set_END      (2)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr_en_START      (3)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_psrr_en_END        (3)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_set_START     (4)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_set_END       (5)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_en_START      (6)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_en_END        (6)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_bw_set_START  (7)
#define PMIC_MMW_LDO1_CFG_REG_1_ldo1_ocps_bw_set_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO1_CFG_REG_2_UNION
 结构说明  : LDO1_CFG_REG_2 寄存器结构定义。地址偏移量:0x04A，初值:0x01，宽度:8
 寄存器说明: LDO1_配置寄存器_2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo1_vrset     : 3;  /* bit[0-2]: LDO1负载调整率补偿调整，5mV / Step */
        unsigned char  ldo1_vgpr_en   : 1;  /* bit[3-3]: ldo1的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  ldo1_psrr2_set : 3;  /* bit[4-6]: ldo1 PSRR 提升电路2的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO1_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vrset_START      (0)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vrset_END        (2)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vgpr_en_START    (3)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_vgpr_en_END      (3)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_psrr2_set_START  (4)
#define PMIC_MMW_LDO1_CFG_REG_2_ldo1_psrr2_set_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO2_CFG_REG_0_UNION
 结构说明  : LDO2_CFG_REG_0 寄存器结构定义。地址偏移量:0x04B，初值:0x60，宽度:8
 寄存器说明: LDO2_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo2_ocpd_vos_set : 1;  /* bit[0-0]: ldo2的ocpd环路offset调节 */
        unsigned char  ldo2_ocpd_set     : 2;  /* bit[1-2]: ldo2 OCPD调节 */
        unsigned char  ldo2_ocp_enn      : 1;  /* bit[3-3]: ldo2限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  ldo2_comp         : 3;  /* bit[4-6]: ldo2的补偿调节 */
        unsigned char  ldo2_bw_en        : 1;  /* bit[7-7]: ldo2 buffer电路电流的调节使能,1表示不随负载变化，0表示随负载变化 */
    } reg;
} PMIC_MMW_LDO2_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_vos_set_START  (0)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_vos_set_END    (0)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_set_START      (1)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocpd_set_END        (2)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocp_enn_START       (3)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_ocp_enn_END         (3)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_comp_START          (4)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_comp_END            (6)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_bw_en_START         (7)
#define PMIC_MMW_LDO2_CFG_REG_0_ldo2_bw_en_END           (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO2_CFG_REG_1_UNION
 结构说明  : LDO2_CFG_REG_1 寄存器结构定义。地址偏移量:0x04C，初值:0x06，宽度:8
 寄存器说明: LDO2_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo2_psrr1_set   : 3;  /* bit[0-2]: ldo2 PSRR 提升电路1的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  ldo2_psrr_en     : 1;  /* bit[3-3]: ldo2 PSRR 提升电路使能信号，1代表使能，0代表关闭 */
        unsigned char  ldo2_ocps_set    : 2;  /* bit[4-5]: ldo2 OCPS调节 */
        unsigned char  ldo2_ocps_en     : 1;  /* bit[6-6]: ldo2的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo2_ocps_bw_set : 1;  /* bit[7-7]: ldo2的ocps环路稳定性调节 */
    } reg;
} PMIC_MMW_LDO2_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr1_set_START    (0)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr1_set_END      (2)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr_en_START      (3)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_psrr_en_END        (3)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_set_START     (4)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_set_END       (5)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_en_START      (6)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_en_END        (6)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_bw_set_START  (7)
#define PMIC_MMW_LDO2_CFG_REG_1_ldo2_ocps_bw_set_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO2_CFG_REG_2_UNION
 结构说明  : LDO2_CFG_REG_2 寄存器结构定义。地址偏移量:0x04D，初值:0x01，宽度:8
 寄存器说明: LDO2_配置寄存器_2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo2_vrset     : 3;  /* bit[0-2]: ldo2负载调整率补偿调整，5mV / Step */
        unsigned char  ldo2_vgpr_en   : 1;  /* bit[3-3]: ldo2的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  ldo2_psrr2_set : 3;  /* bit[4-6]: ldo2 PSRR 提升电路2的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO2_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vrset_START      (0)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vrset_END        (2)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vgpr_en_START    (3)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_vgpr_en_END      (3)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_psrr2_set_START  (4)
#define PMIC_MMW_LDO2_CFG_REG_2_ldo2_psrr2_set_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO3_CFG_REG_0_UNION
 结构说明  : LDO3_CFG_REG_0 寄存器结构定义。地址偏移量:0x04E，初值:0x20，宽度:8
 寄存器说明: LDO3_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo3_ocpd_vos_set : 1;  /* bit[0-0]: ldo3的ocpd环路offset调节 */
        unsigned char  ldo3_ocpd_set     : 2;  /* bit[1-2]: ldo3 OCPD调节 */
        unsigned char  ldo3_ocp_enn      : 1;  /* bit[3-3]: ldo3限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  ldo3_comp         : 3;  /* bit[4-6]: ldo3的补偿调节 */
        unsigned char  ldo3_bw_en        : 1;  /* bit[7-7]: ldo3 buffer电路电流的调节使能,1表示不随负载变化，0表示随负载变化 */
    } reg;
} PMIC_MMW_LDO3_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_vos_set_START  (0)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_vos_set_END    (0)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_set_START      (1)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocpd_set_END        (2)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocp_enn_START       (3)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_ocp_enn_END         (3)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_comp_START          (4)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_comp_END            (6)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_bw_en_START         (7)
#define PMIC_MMW_LDO3_CFG_REG_0_ldo3_bw_en_END           (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO3_CFG_REG_1_UNION
 结构说明  : LDO3_CFG_REG_1 寄存器结构定义。地址偏移量:0x04F，初值:0x06，宽度:8
 寄存器说明: LDO3_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo3_psrr1_set   : 3;  /* bit[0-2]: ldo3 PSRR 提升电路1的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  ldo3_psrr_en     : 1;  /* bit[3-3]: ldo3 PSRR 提升电路使能信号，1代表使能，0代表关闭 */
        unsigned char  ldo3_ocps_set    : 2;  /* bit[4-5]: ldo3 OCPS调节 */
        unsigned char  ldo3_ocps_en     : 1;  /* bit[6-6]: ldo3的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo3_ocps_bw_set : 1;  /* bit[7-7]: ldo3的ocps环路稳定性调节 */
    } reg;
} PMIC_MMW_LDO3_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr1_set_START    (0)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr1_set_END      (2)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr_en_START      (3)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_psrr_en_END        (3)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_set_START     (4)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_set_END       (5)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_en_START      (6)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_en_END        (6)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_bw_set_START  (7)
#define PMIC_MMW_LDO3_CFG_REG_1_ldo3_ocps_bw_set_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO3_CFG_REG_2_UNION
 结构说明  : LDO3_CFG_REG_2 寄存器结构定义。地址偏移量:0x050，初值:0x01，宽度:8
 寄存器说明: LDO3_配置寄存器_2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo3_vrset     : 3;  /* bit[0-2]: ldo3负载调整率补偿调整，5mV / Step */
        unsigned char  ldo3_vgpr_en   : 1;  /* bit[3-3]: ldo3的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  ldo3_psrr2_set : 3;  /* bit[4-6]: ldo3 PSRR 提升电路2的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO3_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vrset_START      (0)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vrset_END        (2)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vgpr_en_START    (3)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_vgpr_en_END      (3)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_psrr2_set_START  (4)
#define PMIC_MMW_LDO3_CFG_REG_2_ldo3_psrr2_set_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO4_CFG_REG_0_UNION
 结构说明  : LDO4_CFG_REG_0 寄存器结构定义。地址偏移量:0x051，初值:0x20，宽度:8
 寄存器说明: LDO4_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo4_ocpd_vos_set : 1;  /* bit[0-0]: ldo4的ocpd环路offset调节 */
        unsigned char  ldo4_ocpd_set     : 2;  /* bit[1-2]: ldo4 OCPD调节 */
        unsigned char  ldo4_ocp_enn      : 1;  /* bit[3-3]: ldo4限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  ldo4_comp         : 3;  /* bit[4-6]: ldo4的补偿调节 */
        unsigned char  ldo4_bw_en        : 1;  /* bit[7-7]: ldo4 buffer电路电流的调节使能,1表示不随负载变化，0表示随负载变化 */
    } reg;
} PMIC_MMW_LDO4_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_vos_set_START  (0)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_vos_set_END    (0)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_set_START      (1)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocpd_set_END        (2)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocp_enn_START       (3)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_ocp_enn_END         (3)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_comp_START          (4)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_comp_END            (6)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_bw_en_START         (7)
#define PMIC_MMW_LDO4_CFG_REG_0_ldo4_bw_en_END           (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO4_CFG_REG_1_UNION
 结构说明  : LDO4_CFG_REG_1 寄存器结构定义。地址偏移量:0x052，初值:0x06，宽度:8
 寄存器说明: LDO4_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo4_psrr1_set   : 3;  /* bit[0-2]: ldo4 PSRR 提升电路1的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  ldo4_psrr_en     : 1;  /* bit[3-3]: ldo4 PSRR 提升电路使能信号，1代表使能，0代表关闭 */
        unsigned char  ldo4_ocps_set    : 2;  /* bit[4-5]: ldo4 OCPS调节 */
        unsigned char  ldo4_ocps_en     : 1;  /* bit[6-6]: ldo4的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo4_ocps_bw_set : 1;  /* bit[7-7]: ldo4的ocps环路稳定性调节 */
    } reg;
} PMIC_MMW_LDO4_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr1_set_START    (0)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr1_set_END      (2)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr_en_START      (3)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_psrr_en_END        (3)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_set_START     (4)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_set_END       (5)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_en_START      (6)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_en_END        (6)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_bw_set_START  (7)
#define PMIC_MMW_LDO4_CFG_REG_1_ldo4_ocps_bw_set_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO4_CFG_REG_2_UNION
 结构说明  : LDO4_CFG_REG_2 寄存器结构定义。地址偏移量:0x053，初值:0x01，宽度:8
 寄存器说明: LDO4_配置寄存器_2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo4_vrset     : 3;  /* bit[0-2]: ldo4负载调整率补偿调整，5mV / Step */
        unsigned char  ldo4_vgpr_en   : 1;  /* bit[3-3]: ldo4的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  ldo4_psrr2_set : 3;  /* bit[4-6]: ldo4 PSRR 提升电路2的档位调节（000表示无提升功能，后续档位影响依次增大） */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO4_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vrset_START      (0)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vrset_END        (2)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vgpr_en_START    (3)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_vgpr_en_END      (3)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_psrr2_set_START  (4)
#define PMIC_MMW_LDO4_CFG_REG_2_ldo4_psrr2_set_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO5_CFG_REG_0_UNION
 结构说明  : LDO5_CFG_REG_0 寄存器结构定义。地址偏移量:0x054，初值:0x06，宽度:8
 寄存器说明: LDO5_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo5_psrr1_set : 3;  /* bit[0-2]: ldo5 PSRR 提升电路1的档位调节（档位影响依次增大） */
        unsigned char  ldo5_psrr_en   : 1;  /* bit[3-3]: ldo5 PSRR 提升电路使能 */
        unsigned char  ldo5_ocp_set   : 2;  /* bit[4-5]: ldo5 ocp大小调节 */
        unsigned char  ldo5_ocp_enn   : 1;  /* bit[6-6]: ldo5限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO5_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr1_set_START  (0)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr1_set_END    (2)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr_en_START    (3)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_psrr_en_END      (3)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_set_START    (4)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_set_END      (5)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_enn_START    (6)
#define PMIC_MMW_LDO5_CFG_REG_0_ldo5_ocp_enn_END      (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO5_CFG_REG_1_UNION
 结构说明  : LDO5_CFG_REG_1 寄存器结构定义。地址偏移量:0x055，初值:0x01，宽度:8
 寄存器说明: LDO5_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo5_vrset     : 3;  /* bit[0-2]: ldo5负载调整率补偿调整，4mV / Step */
        unsigned char  ldo5_psrr2_set : 3;  /* bit[3-5]: ldo5 PSRR 提升电路1的档位调节（档位影响依次增大） */
        unsigned char  reserved       : 2;  /* bit[6-7]: reserved */
    } reg;
} PMIC_MMW_LDO5_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_vrset_START      (0)
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_vrset_END        (2)
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_psrr2_set_START  (3)
#define PMIC_MMW_LDO5_CFG_REG_1_ldo5_psrr2_set_END    (5)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO6_CFG_REG_0_UNION
 结构说明  : LDO6_CFG_REG_0 寄存器结构定义。地址偏移量:0x056，初值:0x06，宽度:8
 寄存器说明: LDO6_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo6_psrr1_set : 3;  /* bit[0-2]: ldo6 PSRR 提升电路1的档位调节（档位影响依次增大） */
        unsigned char  ldo6_psrr_en   : 1;  /* bit[3-3]: ldo6 PSRR 提升电路使能 */
        unsigned char  ldo6_ocp_set   : 2;  /* bit[4-5]: LDO6 ocp大小调节 */
        unsigned char  ldo6_ocp_enn   : 1;  /* bit[6-6]: LDO6限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  reserved       : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_LDO6_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr1_set_START  (0)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr1_set_END    (2)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr_en_START    (3)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_psrr_en_END      (3)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_set_START    (4)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_set_END      (5)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_enn_START    (6)
#define PMIC_MMW_LDO6_CFG_REG_0_ldo6_ocp_enn_END      (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO6_CFG_REG_1_UNION
 结构说明  : LDO6_CFG_REG_1 寄存器结构定义。地址偏移量:0x057，初值:0x01，宽度:8
 寄存器说明: LDO6_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo6_vrset     : 3;  /* bit[0-2]: LDO6负载调整率补偿调整，4mV / Step */
        unsigned char  ldo6_psrr2_set : 3;  /* bit[3-5]: ldo6 PSRR 提升电路1的档位调节（档位影响依次增大） */
        unsigned char  reserved       : 2;  /* bit[6-7]: reserved */
    } reg;
} PMIC_MMW_LDO6_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_vrset_START      (0)
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_vrset_END        (2)
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_psrr2_set_START  (3)
#define PMIC_MMW_LDO6_CFG_REG_1_ldo6_psrr2_set_END    (5)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO7_CFG_REG_0_UNION
 结构说明  : LDO7_CFG_REG_0 寄存器结构定义。地址偏移量:0x058，初值:0x00，宽度:8
 寄存器说明: LDO7_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo7_ocps_set     : 2;  /* bit[0-1]: ldo7 OCPS调节 */
        unsigned char  ldo7_ocps_en      : 1;  /* bit[2-2]: ldo7的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo7_ocps_bw_set  : 1;  /* bit[3-3]: ldo7的ocps环路稳定性调节 */
        unsigned char  ldo7_ocpd_vos_set : 1;  /* bit[4-4]: ldo7的ocpd环路offset调节 */
        unsigned char  ldo7_ocpd_set     : 2;  /* bit[5-6]: ldo7 OCPD调节 */
        unsigned char  ldo7_ocp_enn      : 1;  /* bit[7-7]: ldo7限流模式选择信号，0表示告警限流，1表示告警不限流 */
    } reg;
} PMIC_MMW_LDO7_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_set_START      (0)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_set_END        (1)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_en_START       (2)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_en_END         (2)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_bw_set_START   (3)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocps_bw_set_END     (3)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_vos_set_START  (4)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_vos_set_END    (4)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_set_START      (5)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocpd_set_END        (6)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocp_enn_START       (7)
#define PMIC_MMW_LDO7_CFG_REG_0_ldo7_ocp_enn_END         (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO7_CFG_REG_1_UNION
 结构说明  : LDO7_CFG_REG_1 寄存器结构定义。地址偏移量:0x059，初值:0x01，宽度:8
 寄存器说明: LDO7_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo7_vrset   : 3;  /* bit[0-2]: ldo7负载调整率补偿调整，5mV / Step */
        unsigned char  ldo7_vgpr_en : 1;  /* bit[3-3]: ldo7的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  reserved     : 4;  /* bit[4-7]: reserved */
    } reg;
} PMIC_MMW_LDO7_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vrset_START    (0)
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vrset_END      (2)
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vgpr_en_START  (3)
#define PMIC_MMW_LDO7_CFG_REG_1_ldo7_vgpr_en_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO8_CFG_REG_0_UNION
 结构说明  : LDO8_CFG_REG_0 寄存器结构定义。地址偏移量:0x05A，初值:0x00，宽度:8
 寄存器说明: LDO8_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo8_ocps_set     : 2;  /* bit[0-1]: ldo8 OCPS调节 */
        unsigned char  ldo8_ocps_en      : 1;  /* bit[2-2]: ldo8的S-clamp OCP环路开启的使能信号，1：使能 0：关闭 */
        unsigned char  ldo8_ocps_bw_set  : 1;  /* bit[3-3]: ldo8的ocps环路稳定性调节 */
        unsigned char  ldo8_ocpd_vos_set : 1;  /* bit[4-4]: ldo8的ocpd环路offset调节 */
        unsigned char  ldo8_ocpd_set     : 2;  /* bit[5-6]: ldo8 OCPD调节 */
        unsigned char  ldo8_ocp_enn      : 1;  /* bit[7-7]: ldo8限流模式选择信号，0表示告警限流，1表示告警不限流 */
    } reg;
} PMIC_MMW_LDO8_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_set_START      (0)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_set_END        (1)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_en_START       (2)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_en_END         (2)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_bw_set_START   (3)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocps_bw_set_END     (3)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_vos_set_START  (4)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_vos_set_END    (4)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_set_START      (5)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocpd_set_END        (6)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocp_enn_START       (7)
#define PMIC_MMW_LDO8_CFG_REG_0_ldo8_ocp_enn_END         (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO8_CFG_REG_1_UNION
 结构说明  : LDO8_CFG_REG_1 寄存器结构定义。地址偏移量:0x05B，初值:0x01，宽度:8
 寄存器说明: LDO8_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo8_vrset   : 3;  /* bit[0-2]: ldo8负载调整率补偿调整，5mV / Step */
        unsigned char  ldo8_vgpr_en : 1;  /* bit[3-3]: ldo8的 POWER管gate电压保护， 0：关闭 1：开启 */
        unsigned char  reserved     : 4;  /* bit[4-7]: reserved */
    } reg;
} PMIC_MMW_LDO8_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vrset_START    (0)
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vrset_END      (2)
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vgpr_en_START  (3)
#define PMIC_MMW_LDO8_CFG_REG_1_ldo8_vgpr_en_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_CORE_CFG_REG_0_UNION
 结构说明  : LDO_CORE_CFG_REG_0 寄存器结构定义。地址偏移量:0x05C，初值:0x01，宽度:8
 寄存器说明: LDO_CORE_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_core_vrset   : 3;  /* bit[0-2]: ldo_core负载调整率补偿调整，5mV / Step */
        unsigned char  ldo_core_ocp_set : 2;  /* bit[3-4]: ldo_core ocp大小调节 */
        unsigned char  ldo_core_ocp_enn : 1;  /* bit[5-5]: ldo_core限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  ldo_core_i_sst   : 1;  /* bit[6-6]: ldo_core的缓启电流调整，1表示增大缓启电流，0表示缓启电流为默认值 */
        unsigned char  ldo_core_eco_set : 1;  /* bit[7-7]: ldo_core的ECO偏置电流控制信号，0表示ECO偏置电流保持不变，1表示ECO偏置电流减小1/3 */
    } reg;
} PMIC_MMW_LDO_CORE_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_vrset_START    (0)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_vrset_END      (2)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_set_START  (3)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_set_END    (4)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_enn_START  (5)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_ocp_enn_END    (5)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_i_sst_START    (6)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_i_sst_END      (6)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_eco_set_START  (7)
#define PMIC_MMW_LDO_CORE_CFG_REG_0_ldo_core_eco_set_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF0_CFG_REG_0_UNION
 结构说明  : LDO_BUF0_CFG_REG_0 寄存器结构定义。地址偏移量:0x05D，初值:0x00，宽度:8
 寄存器说明: LDO_BUF0_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_buf0_ocp_set  : 2;  /* bit[0-1]: ldo_buf1 ocp大小调节 */
        unsigned char  ldo_buf0_curr_sel : 2;  /* bit[2-3]: LDO_BUF1的偏置电流控制 */
        unsigned char  reserved          : 4;  /* bit[4-7]: reserved */
    } reg;
} PMIC_MMW_LDO_BUF0_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_ocp_set_START   (0)
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_ocp_set_END     (1)
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_curr_sel_START  (2)
#define PMIC_MMW_LDO_BUF0_CFG_REG_0_ldo_buf0_curr_sel_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF1_CFG_REG_0_UNION
 结构说明  : LDO_BUF1_CFG_REG_0 寄存器结构定义。地址偏移量:0x05E，初值:0x00，宽度:8
 寄存器说明: LDO_BUF1_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_buf1_test     : 1;  /* bit[0-0]: ldo_buf1的测试信号,0表示开关关闭，1表示测试开关闭合 */
        unsigned char  ldo_buf1_sw       : 2;  /* bit[1-2]: ldo_buf0 的直通配置信号，默认0表示正常LDO模式，1表示直通模式 */
        unsigned char  ldo_buf1_curr_sel : 2;  /* bit[3-4]: LDO_BUF1的偏置电流控制 */
        unsigned char  reserved          : 3;  /* bit[5-7]: reserved */
    } reg;
} PMIC_MMW_LDO_BUF1_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_test_START      (0)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_test_END        (0)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_sw_START        (1)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_sw_END          (2)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_curr_sel_START  (3)
#define PMIC_MMW_LDO_BUF1_CFG_REG_0_ldo_buf1_curr_sel_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_BUF_RF_CFG_REG_0_UNION
 结构说明  : LDO_BUF_RF_CFG_REG_0 寄存器结构定义。地址偏移量:0x05F，初值:0x00，宽度:8
 寄存器说明: LDO_BUF_RF_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf_ocp_set  : 2;  /* bit[0-1]: ldo_rfbuf ocp大小调节 */
        unsigned char  ldo_rfbuf_curr_sel : 2;  /* bit[2-3]: ldo_rfbuf的偏置电流控制 */
        unsigned char  reserved           : 4;  /* bit[4-7]: reserved */
    } reg;
} PMIC_MMW_LDO_BUF_RF_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_ocp_set_START   (0)
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_ocp_set_END     (1)
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_curr_sel_START  (2)
#define PMIC_MMW_LDO_BUF_RF_CFG_REG_0_ldo_rfbuf_curr_sel_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF0_CFG_REG_0_UNION
 结构说明  : LDO_RFBUF0_CFG_REG_0 寄存器结构定义。地址偏移量:0x060，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF0_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf0_test     : 1;  /* bit[0-0]: ldo_rfbuf0的测试信号,0表示开关关闭，1表示测试开关闭合 */
        unsigned char  ldo_rfbuf0_sw       : 2;  /* bit[1-2]: ldo_buf0 直通配置信号，默认0表示正常LDO模式，1表示直通模式 */
        unsigned char  ldo_rfbuf0_curr_sel : 2;  /* bit[3-4]: ldo_rfbuf0的偏置电流控制 */
        unsigned char  reserved            : 3;  /* bit[5-7]: reserved */
    } reg;
} PMIC_MMW_LDO_RFBUF0_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_test_START      (0)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_test_END        (0)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_sw_START        (1)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_sw_END          (2)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_curr_sel_START  (3)
#define PMIC_MMW_LDO_RFBUF0_CFG_REG_0_ldo_rfbuf0_curr_sel_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF1_CFG_REG_0_UNION
 结构说明  : LDO_RFBUF1_CFG_REG_0 寄存器结构定义。地址偏移量:0x061，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF1_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf1_test     : 1;  /* bit[0-0]: ldo_rfbuf1的测试信号,0表示开关关闭，1表示测试开关闭合 */
        unsigned char  ldo_rfbuf1_sw       : 2;  /* bit[1-2]: ldo_rfbuf1 直通配置信号，默认0表示正常LDO模式，1表示直通模式 */
        unsigned char  ldo_rfbuf1_curr_sel : 2;  /* bit[3-4]: ldo_rfbuf0的偏置电流控制 */
        unsigned char  reserved            : 3;  /* bit[5-7]: reserved */
    } reg;
} PMIC_MMW_LDO_RFBUF1_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_test_START      (0)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_test_END        (0)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_sw_START        (1)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_sw_END          (2)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_curr_sel_START  (3)
#define PMIC_MMW_LDO_RFBUF1_CFG_REG_0_ldo_rfbuf1_curr_sel_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF2_CFG_REG_0_UNION
 结构说明  : LDO_RFBUF2_CFG_REG_0 寄存器结构定义。地址偏移量:0x062，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF2_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf2_test     : 1;  /* bit[0-0]: ldo_rfbuf2的测试信号,0表示开关关闭，1表示测试开关闭合 */
        unsigned char  ldo_rfbuf2_sw       : 2;  /* bit[1-2]: ldo_rfbuf2 直通配置信号，默认0表示正常LDO模式，1表示直通模式 */
        unsigned char  ldo_rfbuf2_curr_sel : 2;  /* bit[3-4]: ldo_rfbuf2的偏置电流控制 */
        unsigned char  reserved            : 3;  /* bit[5-7]: reserved */
    } reg;
} PMIC_MMW_LDO_RFBUF2_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_test_START      (0)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_test_END        (0)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_sw_START        (1)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_sw_END          (2)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_curr_sel_START  (3)
#define PMIC_MMW_LDO_RFBUF2_CFG_REG_0_ldo_rfbuf2_curr_sel_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF3_CFG_REG_0_UNION
 结构说明  : LDO_RFBUF3_CFG_REG_0 寄存器结构定义。地址偏移量:0x063，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF3_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf3_test     : 1;  /* bit[0-0]: ldo_rfbuf3的测试信号,0表示开关关闭，1表示测试开关闭合 */
        unsigned char  ldo_rfbuf3_sw       : 2;  /* bit[1-2]: ldo_rfbuf3 直通配置信号，默认0表示正常LDO模式，1表示直通模式 */
        unsigned char  ldo_rfbuf3_curr_sel : 2;  /* bit[3-4]: ldo_rfbuf3的偏置电流控制 */
        unsigned char  reserved            : 3;  /* bit[5-7]: reserved */
    } reg;
} PMIC_MMW_LDO_RFBUF3_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_test_START      (0)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_test_END        (0)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_sw_START        (1)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_sw_END          (2)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_curr_sel_START  (3)
#define PMIC_MMW_LDO_RFBUF3_CFG_REG_0_ldo_rfbuf3_curr_sel_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RFBUF4_CFG_REG_0_UNION
 结构说明  : LDO_RFBUF4_CFG_REG_0 寄存器结构定义。地址偏移量:0x064，初值:0x00，宽度:8
 寄存器说明: LDO_RFBUF4_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf4_test     : 1;  /* bit[0-0]: ldo_rfbuf4的测试信号,0表示开关关闭，1表示测试开关闭合 */
        unsigned char  ldo_rfbuf4_sw       : 2;  /* bit[1-2]: ldo_rfbuf4直通配置信号，默认0表示正常LDO模式，1表示直通模式 */
        unsigned char  ldo_rfbuf4_curr_sel : 2;  /* bit[3-4]: ldo_rfbuf4的偏置电流控制 */
        unsigned char  reserved            : 3;  /* bit[5-7]: reserved */
    } reg;
} PMIC_MMW_LDO_RFBUF4_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_test_START      (0)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_test_END        (0)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_sw_START        (1)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_sw_END          (2)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_curr_sel_START  (3)
#define PMIC_MMW_LDO_RFBUF4_CFG_REG_0_ldo_rfbuf4_curr_sel_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_PMUH_CFG_REG_0_UNION
 结构说明  : PMUH_CFG_REG_0 寄存器结构定义。地址偏移量:0x065，初值:0x01，宽度:8
 寄存器说明: PMUH_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  pmuh_vrset    : 3;  /* bit[0-2]: pmuh负载调整率补偿调整，5mV / Step */
        unsigned char  pmuh_ocp_set  : 2;  /* bit[3-4]: pmuh ocp大小调节 */
        unsigned char  pmuh_ocp_enn  : 1;  /* bit[5-5]: pmuh限流模式选择信号，0表示告警限流，1表示告警不限流 */
        unsigned char  pmuh_mode_sel : 1;  /* bit[6-6]: PMUH进入低功耗ECO模式控制信号，1代表eco模式，0代表normal模式 */
        unsigned char  reserved      : 1;  /* bit[7-7]: reserved */
    } reg;
} PMIC_MMW_PMUH_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_vrset_START     (0)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_vrset_END       (2)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_set_START   (3)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_set_END     (4)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_enn_START   (5)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_ocp_enn_END     (5)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_mode_sel_START  (6)
#define PMIC_MMW_PMUH_CFG_REG_0_pmuh_mode_sel_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RESERVE_CFG_REG_0_UNION
 结构说明  : LDO_RESERVE_CFG_REG_0 寄存器结构定义。地址偏移量:0x066，初值:0x00，宽度:8
 寄存器说明: LDO_RESERVE_配置寄存器_0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_reserve0 : 8;  /* bit[0-7]: 备用寄存器 */
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_0_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ldo_reserve0_START  (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_0_ldo_reserve0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RESERVE_CFG_REG_1_UNION
 结构说明  : LDO_RESERVE_CFG_REG_1 寄存器结构定义。地址偏移量:0x067，初值:0x00，宽度:8
 寄存器说明: LDO_RESERVE_配置寄存器_1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_reserve1 : 8;  /* bit[0-7]: 备用寄存器 */
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_1_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ldo_reserve1_START  (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_1_ldo_reserve1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RESERVE_CFG_REG_2_UNION
 结构说明  : LDO_RESERVE_CFG_REG_2 寄存器结构定义。地址偏移量:0x068，初值:0x00，宽度:8
 寄存器说明: LDO_RESERVE_配置寄存器_2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_reserve2 : 8;  /* bit[0-7]: 备用寄存器 */
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_2_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ldo_reserve2_START  (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_2_ldo_reserve2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_RESERVE_CFG_REG_3_UNION
 结构说明  : LDO_RESERVE_CFG_REG_3 寄存器结构定义。地址偏移量:0x069，初值:0x00，宽度:8
 寄存器说明: LDO_RESERVE_配置寄存器_3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_reserve3 : 8;  /* bit[0-7]: 备用寄存器 */
    } reg;
} PMIC_MMW_LDO_RESERVE_CFG_REG_3_UNION;
#endif
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ldo_reserve3_START  (0)
#define PMIC_MMW_LDO_RESERVE_CFG_REG_3_ldo_reserve3_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_UFS_CTRL_UNION
 结构说明  : CLK_UFS_CTRL 寄存器结构定义。地址偏移量:0x06A，初值:0x03，宽度:8
 寄存器说明: UFS时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_ufs_drv : 2;  /* bit[0-1]: clk_ufs驱动能力控制，00表示驱动为5pF//100K,11表示驱动20pF//100K */
        unsigned char  reserved   : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_UFS_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_UFS_CTRL_xo_ufs_drv_START  (0)
#define PMIC_MMW_CLK_UFS_CTRL_xo_ufs_drv_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF0_CTRL_UNION
 结构说明  : CLK_RF0_CTRL 寄存器结构定义。地址偏移量:0x06B，初值:0x04，宽度:8
 寄存器说明: RF0时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_rf0_drv : 3;  /* bit[0-2]: clk_rf0驱动能力控制，00表示驱动为5pF//100K,111表示驱动35pF//100K */
        unsigned char  reserved   : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF0_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF0_CTRL_xo_rf0_drv_START  (0)
#define PMIC_MMW_CLK_RF0_CTRL_xo_rf0_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF1_CTRL_UNION
 结构说明  : CLK_RF1_CTRL 寄存器结构定义。地址偏移量:0x06C，初值:0x04，宽度:8
 寄存器说明: RF1时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_rf1_drv : 3;  /* bit[0-2]: clk_rf1驱动能力控制，00表示驱动为5pF//100K,111表示驱动35pF//100K */
        unsigned char  reserved   : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF1_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF1_CTRL_xo_rf1_drv_START  (0)
#define PMIC_MMW_CLK_RF1_CTRL_xo_rf1_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF2_CTRL_UNION
 结构说明  : CLK_RF2_CTRL 寄存器结构定义。地址偏移量:0x06D，初值:0x04，宽度:8
 寄存器说明: RF2时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_rf2_drv : 3;  /* bit[0-2]: clk_rf2驱动能力控制，00表示驱动为5pF//100K,111表示驱动35pF//100K */
        unsigned char  reserved   : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF2_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF2_CTRL_xo_rf2_drv_START  (0)
#define PMIC_MMW_CLK_RF2_CTRL_xo_rf2_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF3_CTRL_UNION
 结构说明  : CLK_RF3_CTRL 寄存器结构定义。地址偏移量:0x06E，初值:0x04，宽度:8
 寄存器说明: RF3时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_rf3_drv : 3;  /* bit[0-2]: clk_rf3驱动能力控制，00表示驱动为5pF//100K,111表示驱动35pF//100K */
        unsigned char  reserved   : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF3_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF3_CTRL_xo_rf3_drv_START  (0)
#define PMIC_MMW_CLK_RF3_CTRL_xo_rf3_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF4_CTRL_UNION
 结构说明  : CLK_RF4_CTRL 寄存器结构定义。地址偏移量:0x06F，初值:0x04，宽度:8
 寄存器说明: RF4时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_rf4_drv : 3;  /* bit[0-2]: clk_rf4驱动能力控制，00表示驱动为5pF//100K,111表示驱动35pF//100K */
        unsigned char  reserved   : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF4_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF4_CTRL_xo_rf4_drv_START  (0)
#define PMIC_MMW_CLK_RF4_CTRL_xo_rf4_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SERDES_CTRL_UNION
 结构说明  : CLK_SERDES_CTRL 寄存器结构定义。地址偏移量:0x070，初值:0x04，宽度:8
 寄存器说明: SERDESB时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_serdes_drv : 3;  /* bit[0-2]: clk_serdes驱动能力控制，00表示驱动为5pF//100K，111表示驱动35pF//100K */
        unsigned char  reserved      : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SERDES_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_CTRL_xo_serdes_drv_START  (0)
#define PMIC_MMW_CLK_SERDES_CTRL_xo_serdes_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI0_CTRL_UNION
 结构说明  : CLK_WIFI0_CTRL 寄存器结构定义。地址偏移量:0x071，初值:0x04，宽度:8
 寄存器说明: WIFI0时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_wifi0_drv : 3;  /* bit[0-2]: clk_wifi0驱动能力控制，00表示驱动为5pF//100K，111表示驱动35pF//100K */
        unsigned char  reserved     : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI0_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_CTRL_xo_wifi0_drv_START  (0)
#define PMIC_MMW_CLK_WIFI0_CTRL_xo_wifi0_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI1_CTRL_UNION
 结构说明  : CLK_WIFI1_CTRL 寄存器结构定义。地址偏移量:0x072，初值:0x06，宽度:8
 寄存器说明: WIFI1时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_wifi1_drv : 3;  /* bit[0-2]: clk_wifi1驱动能力控制，00表示驱动为5pF//100K,111表示驱动20pF//100K */
        unsigned char  reserved     : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI1_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_CTRL_xo_wifi1_drv_START  (0)
#define PMIC_MMW_CLK_WIFI1_CTRL_xo_wifi1_drv_END    (2)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_NFC_CTRL_UNION
 结构说明  : CLK_NFC_CTRL 寄存器结构定义。地址偏移量:0x073，初值:0x03，宽度:8
 寄存器说明: NFC时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_nfc_drv : 2;  /* bit[0-1]: clk_nfc驱动能力控制，00表示驱动为5pF//100K，11表示驱动20pF//100K */
        unsigned char  reserved   : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_NFC_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_NFC_CTRL_xo_nfc_drv_START  (0)
#define PMIC_MMW_CLK_NFC_CTRL_xo_nfc_drv_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SYS_CTRL_UNION
 结构说明  : CLK_SYS_CTRL 寄存器结构定义。地址偏移量:0x074，初值:0x03，宽度:8
 寄存器说明: SYS时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_sys_drv : 2;  /* bit[0-1]: clk_sys驱动能力控制，00表示驱动为5pF//100K，11表示驱动20pF//100K */
        unsigned char  reserved   : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SYS_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_CTRL_xo_sys_drv_START  (0)
#define PMIC_MMW_CLK_SYS_CTRL_xo_sys_drv_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_PMU_CTRL_UNION
 结构说明  : CLK_PMU_CTRL 寄存器结构定义。地址偏移量:0x075，初值:0x03，宽度:8
 寄存器说明: 送给主PMU的时钟驱动能力配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_pmu_drv : 2;  /* bit[0-1]: clk_pmu_out驱动能力控制，00表示驱动为5pF//100K，11表示驱动20pF//100K */
        unsigned char  reserved   : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_PMU_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_PMU_CTRL_xo_pmu_drv_START  (0)
#define PMIC_MMW_CLK_PMU_CTRL_xo_pmu_drv_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_CTRL0_UNION
 结构说明  : XO_CTRL0 寄存器结构定义。地址偏移量:0x076，初值:0x00，宽度:8
 寄存器说明: XO时钟控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_rf1_ph_sel    : 1;  /* bit[0]  : rf1时钟的相位控制寄存器，可以调节相位变化180度 */
        unsigned char  xo_tri_cap       : 1;  /* bit[1]  : 时钟的三角波电路的电容控制 */
        unsigned char  xo_delay_sel     : 2;  /* bit[2-3]: 时钟输出的Delay时间的控制选择信号。
                                                           00:64个256k时钟周期；01:32个，10：16个，11:0 */
        unsigned char  reserved         : 2;  /* bit[4-5]: 保留。 */
        unsigned char  xo_eco_force_en  : 1;  /* bit[6]  : XO退出eco的屏蔽信号：
                                                           0：不影响normal/eco模式；
                                                           1：强制xo_core进入eco模式。 */
        unsigned char  xo_eco_en_shield : 1;  /* bit[7]  : XO进入eco的屏蔽信号：
                                                           0：不跟随eco_in_n进退eco，保持normal；
                                                           1：跟随eco_in_n进退eco。 */
    } reg;
} PMIC_MMW_XO_CTRL0_UNION;
#endif
#define PMIC_MMW_XO_CTRL0_xo_rf1_ph_sel_START     (0)
#define PMIC_MMW_XO_CTRL0_xo_rf1_ph_sel_END       (0)
#define PMIC_MMW_XO_CTRL0_xo_tri_cap_START        (1)
#define PMIC_MMW_XO_CTRL0_xo_tri_cap_END          (1)
#define PMIC_MMW_XO_CTRL0_xo_delay_sel_START      (2)
#define PMIC_MMW_XO_CTRL0_xo_delay_sel_END        (3)
#define PMIC_MMW_XO_CTRL0_xo_eco_force_en_START   (6)
#define PMIC_MMW_XO_CTRL0_xo_eco_force_en_END     (6)
#define PMIC_MMW_XO_CTRL0_xo_eco_en_shield_START  (7)
#define PMIC_MMW_XO_CTRL0_xo_eco_en_shield_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_RESERVE_0_UNION
 结构说明  : XO_RESERVE_0 寄存器结构定义。地址偏移量:0x077，初值:0x00，宽度:8
 寄存器说明: XO预留寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_reserve0 : 8;  /* bit[0-7]: <7>: no use
                                                      <6:5>:xo osc反馈电阻设置：
                                                      00:200k；01:40k；10:20k；11:10k
                                                      <4:3>: 正弦波buffer三角波电阻值设置：
                                                      00:120ohm 01:240ohm 10:360ohm 11:480ohm
                                                      <2:1>: 正弦波buffer三角波电阻值设置：
                                                      00：6pF 01:4pF 10：2pF 00:0pF
                                                      <0>: 时钟输出是否同步时钟沿设置：
                                                      0：不同步 1：同步 */
    } reg;
} PMIC_MMW_XO_RESERVE_0_UNION;
#endif
#define PMIC_MMW_XO_RESERVE_0_xo_reserve0_START  (0)
#define PMIC_MMW_XO_RESERVE_0_xo_reserve0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_DET_STATUS_UNION
 结构说明  : XO_DET_STATUS 寄存器结构定义。地址偏移量:0x078，初值:0x00，宽度:8
 寄存器说明: XO corner检测状态指示寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_corner_det_valid : 1;  /* bit[0]  : XO corner检测数据输出有效指示位。
                                                              0：检测过程中；
                                                              1：检测完毕。
                                                              (注：OTP bit(xo_corner_det_enb)为0，该bit在检测开始时，被拉低；在检测结束后，变成“1”，并且“1”一直持续到下一次检测开始)。OTP bit(xo_corner_det_enb)为1，该bit一直为低。 */
        unsigned char  reserved            : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_DET_STATUS_UNION;
#endif
#define PMIC_MMW_XO_DET_STATUS_xo_corner_det_valid_START  (0)
#define PMIC_MMW_XO_DET_STATUS_xo_corner_det_valid_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_BG_THSD_CTRL0_UNION
 结构说明  : BG_THSD_CTRL0 寄存器结构定义。地址偏移量:0x079，初值:0x03，宽度:8
 寄存器说明: 基准控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  thsd_tmp_set      : 2;  /* bit[0-1]: 温度保护模块报警温度设置。
                                                            00：95℃；
                                                            01：105℃；
                                                            10：115℃；
                                                            11：125℃；
                                                            默认设置为125℃。 */
        unsigned char  reserved_0        : 1;  /* bit[2]  : 保留。 */
        unsigned char  ref_out_sel       : 2;  /* bit[3-4]: ref_out_sel<0>
                                                            0：0.75V基准输出默认加BUF
                                                            1：0.75V基准输出不加BUF
                                                            ref_out_sel<1>
                                                            0：不打开trim vbg温度系数通道
                                                            1：打开trim vbg温度系数的通道 */
        unsigned char  ref_ibias_trim_en : 2;  /* bit[5-6]: 基准电流Trim使能信号 */
        unsigned char  reserved_1        : 1;  /* bit[7]  : 保留 */
    } reg;
} PMIC_MMW_BG_THSD_CTRL0_UNION;
#endif
#define PMIC_MMW_BG_THSD_CTRL0_thsd_tmp_set_START       (0)
#define PMIC_MMW_BG_THSD_CTRL0_thsd_tmp_set_END         (1)
#define PMIC_MMW_BG_THSD_CTRL0_ref_out_sel_START        (3)
#define PMIC_MMW_BG_THSD_CTRL0_ref_out_sel_END          (4)
#define PMIC_MMW_BG_THSD_CTRL0_ref_ibias_trim_en_START  (5)
#define PMIC_MMW_BG_THSD_CTRL0_ref_ibias_trim_en_END    (6)


/*****************************************************************************
 结构名    : PMIC_MMW_BG_THSD_CTRL1_UNION
 结构说明  : BG_THSD_CTRL1 寄存器结构定义。地址偏移量:0x07A，初值:0x01，宽度:8
 寄存器说明: 基准控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ref_res_sel      : 1;  /* bit[0]  : 1：基准输出RC电阻选大
                                                           0：基准输出RC电阻选小 */
        unsigned char  ref_chop_clk_sel : 2;  /* bit[1-2]: 00：32K
                                                           11：128K */
        unsigned char  ref_chop_sel     : 1;  /* bit[3]  : 0：默认BG没有chop；
                                                           1：使能BG的chop功能； */
        unsigned char  reserved         : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_BG_THSD_CTRL1_UNION;
#endif
#define PMIC_MMW_BG_THSD_CTRL1_ref_res_sel_START       (0)
#define PMIC_MMW_BG_THSD_CTRL1_ref_res_sel_END         (0)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_clk_sel_START  (1)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_clk_sel_END    (2)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_sel_START      (3)
#define PMIC_MMW_BG_THSD_CTRL1_ref_chop_sel_END        (3)


/*****************************************************************************
 结构名    : PMIC_MMW_BG_RESERVE_0_UNION
 结构说明  : BG_RESERVE_0 寄存器结构定义。地址偏移量:0x07B，初值:0x00，宽度:8
 寄存器说明: 基准预留寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ref_reserve0 : 8;  /* bit[0-7]: reserved Register
                                                       ref_reserve<0>：125 140过温保护温度增加5度，默认不加
                                                       ref_reserve<1>：140过温保护温度降低5度，默认不降
                                                       ref_reserve<2>：过温保护与trim码值绑定，默认不绑定
                                                       ref_reserve<3>：npn的trim码值同时trim基级电阻的负温，默认绑定
                                                       ref_reserve<4>：RF0P9_LDO滤波电阻选择 0：50k :1：100k
                                                       ref_reserve<5>：chopper频率选择:0：64k :1：32k */
    } reg;
} PMIC_MMW_BG_RESERVE_0_UNION;
#endif
#define PMIC_MMW_BG_RESERVE_0_ref_reserve0_START  (0)
#define PMIC_MMW_BG_RESERVE_0_ref_reserve0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_BG_RESERVE_1_UNION
 结构说明  : BG_RESERVE_1 寄存器结构定义。地址偏移量:0x07C，初值:0x00，宽度:8
 寄存器说明: 基准预留寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ref_reserve1 : 8;  /* bit[0-7]: 基准预留寄存器 */
    } reg;
} PMIC_MMW_BG_RESERVE_1_UNION;
#endif
#define PMIC_MMW_BG_RESERVE_1_ref_reserve1_START  (0)
#define PMIC_MMW_BG_RESERVE_1_ref_reserve1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_BG_RESERVE_2_UNION
 结构说明  : BG_RESERVE_2 寄存器结构定义。地址偏移量:0x07D，初值:0x00，宽度:8
 寄存器说明: 基准预留寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ref_reserve2 : 8;  /* bit[0-7]: 基准预留寄存器 */
    } reg;
} PMIC_MMW_BG_RESERVE_2_UNION;
#endif
#define PMIC_MMW_BG_RESERVE_2_ref_reserve2_START  (0)
#define PMIC_MMW_BG_RESERVE_2_ref_reserve2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_CTRL_RESERVE_UNION
 结构说明  : SYS_CTRL_RESERVE 寄存器结构定义。地址偏移量:0x07E，初值:0x00，宽度:8
 寄存器说明: 系统控制预留寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  sys_ctrl_reserve : 8;  /* bit[0-7]: 系统控制模块预留寄存器
                                                           sys_ctrl_reserve ：电流检测通道检测选择 */
    } reg;
} PMIC_MMW_SYS_CTRL_RESERVE_UNION;
#endif
#define PMIC_MMW_SYS_CTRL_RESERVE_sys_ctrl_reserve_START  (0)
#define PMIC_MMW_SYS_CTRL_RESERVE_sys_ctrl_reserve_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_CTRL0_UNION
 结构说明  : SYS_CTRL0 寄存器结构定义。地址偏移量:0x07F，初值:0x00，宽度:8
 寄存器说明: 系统控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  pmu_test_buf_en : 1;  /* bit[0]  : ATE复用测试BUF使能信号 */
        unsigned char  pmu_test_en     : 1;  /* bit[1]  : ATE复用测试使能信号 */
        unsigned char  reserved        : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_SYS_CTRL0_UNION;
#endif
#define PMIC_MMW_SYS_CTRL0_pmu_test_buf_en_START  (0)
#define PMIC_MMW_SYS_CTRL0_pmu_test_buf_en_END    (0)
#define PMIC_MMW_SYS_CTRL0_pmu_test_en_START      (1)
#define PMIC_MMW_SYS_CTRL0_pmu_test_en_END        (1)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_CTRL1_UNION
 结构说明  : SYS_CTRL1 寄存器结构定义。地址偏移量:0x080，初值:0x00，宽度:8
 寄存器说明: 系统控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  pmu_test_sel : 8;  /* bit[0-7]: ATE复用测试选择信号 */
    } reg;
} PMIC_MMW_SYS_CTRL1_UNION;
#endif
#define PMIC_MMW_SYS_CTRL1_pmu_test_sel_START  (0)
#define PMIC_MMW_SYS_CTRL1_pmu_test_sel_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_A2D_RES0_UNION
 结构说明  : A2D_RES0 寄存器结构定义。地址偏移量:0x084，初值:0x00，宽度:8
 寄存器说明: 模拟到数字的保留寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  a2d_reserve0 : 8;  /* bit[0-7]: 模拟到数字的保留信号。 */
    } reg;
} PMIC_MMW_A2D_RES0_UNION;
#endif
#define PMIC_MMW_A2D_RES0_a2d_reserve0_START  (0)
#define PMIC_MMW_A2D_RES0_a2d_reserve0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_D2A_RES0_UNION
 结构说明  : D2A_RES0 寄存器结构定义。地址偏移量:0x085，初值:0x00，宽度:8
 寄存器说明: 数字到模拟的保留寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  d2a_reserve0 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_D2A_RES0_UNION;
#endif
#define PMIC_MMW_D2A_RES0_d2a_reserve0_START  (0)
#define PMIC_MMW_D2A_RES0_d2a_reserve0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_D2A_RES1_UNION
 结构说明  : D2A_RES1 寄存器结构定义。地址偏移量:0x086，初值:0x00，宽度:8
 寄存器说明: 数字到模拟的保留寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  d2a_reserve1 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_D2A_RES1_UNION;
#endif
#define PMIC_MMW_D2A_RES1_d2a_reserve1_START  (0)
#define PMIC_MMW_D2A_RES1_d2a_reserve1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_D2A_RES2_UNION
 结构说明  : D2A_RES2 寄存器结构定义。地址偏移量:0x087，初值:0x00，宽度:8
 寄存器说明: 数字到模拟的保留寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  d2a_reserve2 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_D2A_RES2_UNION;
#endif
#define PMIC_MMW_D2A_RES2_d2a_reserve2_START  (0)
#define PMIC_MMW_D2A_RES2_d2a_reserve2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_D2A_RES3_UNION
 结构说明  : D2A_RES3 寄存器结构定义。地址偏移量:0x088，初值:0x00，宽度:8
 寄存器说明: 数字到模拟的保留寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  d2a_reserve3 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_D2A_RES3_UNION;
#endif
#define PMIC_MMW_D2A_RES3_d2a_reserve3_START  (0)
#define PMIC_MMW_D2A_RES3_d2a_reserve3_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_0_UNION
 结构说明  : OTP_0 寄存器结构定义。地址偏移量:0x089，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob0 : 8;  /* bit[0-7]: OTP回读值bit[7:0]。 */
    } reg;
} PMIC_MMW_OTP_0_UNION;
#endif
#define PMIC_MMW_OTP_0_otp_pdob0_START  (0)
#define PMIC_MMW_OTP_0_otp_pdob0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_1_UNION
 结构说明  : OTP_1 寄存器结构定义。地址偏移量:0x08A，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob1 : 8;  /* bit[0-7]: OTP回读值bit[15:8]。 */
    } reg;
} PMIC_MMW_OTP_1_UNION;
#endif
#define PMIC_MMW_OTP_1_otp_pdob1_START  (0)
#define PMIC_MMW_OTP_1_otp_pdob1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_CLK_CTRL_UNION
 结构说明  : OTP_CLK_CTRL 寄存器结构定义。地址偏移量:0x08B，初值:0xAC，宽度:8
 寄存器说明: 手动控制读写操作中OTP时钟产生控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_clk_ctrl : 8;  /* bit[0-7]: 手动控制读写操作中OTP时钟产生控制寄存器。
                                                       该寄存器写入0x5A开启内部otp的32k时钟，写入0xAC，关闭内部otp的32k时钟。 */
    } reg;
} PMIC_MMW_OTP_CLK_CTRL_UNION;
#endif
#define PMIC_MMW_OTP_CLK_CTRL_otp_clk_ctrl_START  (0)
#define PMIC_MMW_OTP_CLK_CTRL_otp_clk_ctrl_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_CTRL0_UNION
 结构说明  : OTP_CTRL0 寄存器结构定义。地址偏移量:0x08C，初值:0x00，宽度:8
 寄存器说明: OTP控制信号寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pwe_int   : 1;  /* bit[0]  : OTP写控制信号。
                                                        0：拉低otp_pwe，结束写操作；
                                                        1：拉高otp_pwe，发起写操作。
                                                        软件约束：需保证otp_por的高电平持续时间大于45us。两次高电平之间持续时间大于45us */
        unsigned char  otp_pwe_pulse : 1;  /* bit[1]  : OTP写脉冲控制。
                                                        0：不发起脉冲；
                                                        1：发起一个310us的高电平脉冲。
                                                        软件约束：软件两次配置之间间隔需大于13个32K时钟周期 */
        unsigned char  otp_por_int   : 1;  /* bit[2]  : OTP读控制信号。
                                                        0：拉低otp_por，结束读操作；
                                                        1：拉高otp_por，发起读操作。
                                                        软件约束：需保证otp_por的高电平持续时间大于45us。两次高电平之间持续时间大于45us */
        unsigned char  otp_por_pulse : 1;  /* bit[3]  : OTP初始化控制信号。
                                                        0：不发起脉冲；
                                                        1：发起一个30us的por高电平脉冲。
                                                        软件约束：软件两次配置之间间隔需大于6个32K时钟周期 */
        unsigned char  reserved      : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_OTP_CTRL0_UNION;
#endif
#define PMIC_MMW_OTP_CTRL0_otp_pwe_int_START    (0)
#define PMIC_MMW_OTP_CTRL0_otp_pwe_int_END      (0)
#define PMIC_MMW_OTP_CTRL0_otp_pwe_pulse_START  (1)
#define PMIC_MMW_OTP_CTRL0_otp_pwe_pulse_END    (1)
#define PMIC_MMW_OTP_CTRL0_otp_por_int_START    (2)
#define PMIC_MMW_OTP_CTRL0_otp_por_int_END      (2)
#define PMIC_MMW_OTP_CTRL0_otp_por_pulse_START  (3)
#define PMIC_MMW_OTP_CTRL0_otp_por_pulse_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_CTRL1_UNION
 结构说明  : OTP_CTRL1 寄存器结构定义。地址偏移量:0x08D，初值:0x00，宽度:8
 寄存器说明: OTP控制信号寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pprog      : 1;  /* bit[0]  : 编程模式使能信号。
                                                         0：关闭otp编程；
                                                         1：使能otp编程。 */
        unsigned char  otp_inctrl_sel : 1;  /* bit[1]  : 0：芯片内部自动读；（默认）
                                                         1：寄存器控制。 
                                                         开机后此寄存器必须配置为1，否则无法通过寄存器发起otp读操作 */
        unsigned char  reserved       : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_OTP_CTRL1_UNION;
#endif
#define PMIC_MMW_OTP_CTRL1_otp_pprog_START       (0)
#define PMIC_MMW_OTP_CTRL1_otp_pprog_END         (0)
#define PMIC_MMW_OTP_CTRL1_otp_inctrl_sel_START  (1)
#define PMIC_MMW_OTP_CTRL1_otp_inctrl_sel_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_CTRL2_UNION
 结构说明  : OTP_CTRL2 寄存器结构定义。地址偏移量:0x08E，初值:0x00，宽度:8
 寄存器说明: OTP控制信号寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pa_cfg : 6;  /* bit[0-5]: OTP的地址信号。
                                                     读模式：（每次读出16bit，地址加2）
                                                     000000：选择otp器件地址为6'b000000的进行读操作；
                                                     000010：选择otp器件地址为6'b000010的进行读操作；
                                                     000100：选择otp器件地址为6'b0000100的进行读操作；
                                                     ……
                                                     111110：选择otp器件地址为6'b111110的进行读操作。
                                                     编程模式：（每次写8bit）
                                                     000000：选择otp器件地址为6'b000000的进行写操作；
                                                     000001：选择otp器件地址为6'b000001的进行写操作；
                                                     000010：选择otp器件地址为6'b0000010的进行写操作；
                                                     ……
                                                     111111：选择otp器件地址为6'b111111的进行写操作。
                                                     软件约束：
                                                     1)采用otp_por_pulse方式读取数据时，向otp_por_pulse写1时刻需保证otp_pa_cfg地址提前4ns准备好，自otp_por_pulse写1时刻起，需保证otp_pa_cfg地址持续时间大于等于192us(6个32k clk);
                                                     2)采用otp_por_int方式读取数据时，otp_por_int写1时刻需保证otp_pa_cfg地址提前4ns准备好，向otp_por_int写0时刻起，需保证otp_pa_cfg地址持续时间大于等于62us(2个32k clk)； */
        unsigned char  otp_ptm    : 2;  /* bit[6-7]: 测试模式使能信号。
                                                     00：用户模式；
                                                     01：未定义；
                                                     10：margin_1读模式；
                                                     11：margin_2读模式。 */
    } reg;
} PMIC_MMW_OTP_CTRL2_UNION;
#endif
#define PMIC_MMW_OTP_CTRL2_otp_pa_cfg_START  (0)
#define PMIC_MMW_OTP_CTRL2_otp_pa_cfg_END    (5)
#define PMIC_MMW_OTP_CTRL2_otp_ptm_START     (6)
#define PMIC_MMW_OTP_CTRL2_otp_ptm_END       (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_WDATA_UNION
 结构说明  : OTP_WDATA 寄存器结构定义。地址偏移量:0x08F，初值:0x00，宽度:8
 寄存器说明: OTP写操作数据寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdin : 8;  /* bit[0-7]: OTP的输入值寄存器。 */
    } reg;
} PMIC_MMW_OTP_WDATA_UNION;
#endif
#define PMIC_MMW_OTP_WDATA_otp_pdin_START  (0)
#define PMIC_MMW_OTP_WDATA_otp_pdin_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_0_W_UNION
 结构说明  : OTP_0_W 寄存器结构定义。地址偏移量:0x090，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob0_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob0相或后送到数模接口otp_d2a_pdob0 */
    } reg;
} PMIC_MMW_OTP_0_W_UNION;
#endif
#define PMIC_MMW_OTP_0_W_otp_pdob0_w_START  (0)
#define PMIC_MMW_OTP_0_W_otp_pdob0_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_1_W_UNION
 结构说明  : OTP_1_W 寄存器结构定义。地址偏移量:0x091，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob1_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob1相或后送到数模接口otp_d2a_pdob0 */
    } reg;
} PMIC_MMW_OTP_1_W_UNION;
#endif
#define PMIC_MMW_OTP_1_W_otp_pdob1_w_START  (0)
#define PMIC_MMW_OTP_1_W_otp_pdob1_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_2_W_UNION
 结构说明  : OTP_2_W 寄存器结构定义。地址偏移量:0x092，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob2_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob2相或后送到数模接口otp_d2a_pdob1 */
    } reg;
} PMIC_MMW_OTP_2_W_UNION;
#endif
#define PMIC_MMW_OTP_2_W_otp_pdob2_w_START  (0)
#define PMIC_MMW_OTP_2_W_otp_pdob2_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_3_W_UNION
 结构说明  : OTP_3_W 寄存器结构定义。地址偏移量:0x093，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob3_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob3相或后送到数模接口otp_d2a_pdob1 */
    } reg;
} PMIC_MMW_OTP_3_W_UNION;
#endif
#define PMIC_MMW_OTP_3_W_otp_pdob3_w_START  (0)
#define PMIC_MMW_OTP_3_W_otp_pdob3_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_4_W_UNION
 结构说明  : OTP_4_W 寄存器结构定义。地址偏移量:0x094，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob4_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob4相或后送到数模接口otp_d2a_pdob2 */
    } reg;
} PMIC_MMW_OTP_4_W_UNION;
#endif
#define PMIC_MMW_OTP_4_W_otp_pdob4_w_START  (0)
#define PMIC_MMW_OTP_4_W_otp_pdob4_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_5_W_UNION
 结构说明  : OTP_5_W 寄存器结构定义。地址偏移量:0x095，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob5_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob5相或后送到数模接口otp_d2a_pdob2 */
    } reg;
} PMIC_MMW_OTP_5_W_UNION;
#endif
#define PMIC_MMW_OTP_5_W_otp_pdob5_w_START  (0)
#define PMIC_MMW_OTP_5_W_otp_pdob5_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_6_W_UNION
 结构说明  : OTP_6_W 寄存器结构定义。地址偏移量:0x096，初值:0x00，宽度:8
 寄存器说明: OTP0预修调寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob6_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob6相或后送到数模接口otp_d2a_pdob3 */
    } reg;
} PMIC_MMW_OTP_6_W_UNION;
#endif
#define PMIC_MMW_OTP_6_W_otp_pdob6_w_START  (0)
#define PMIC_MMW_OTP_6_W_otp_pdob6_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_7_W_UNION
 结构说明  : OTP_7_W 寄存器结构定义。地址偏移量:0x097，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob7_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob7相或后送到数模接口otp_d2a_pdob3 */
    } reg;
} PMIC_MMW_OTP_7_W_UNION;
#endif
#define PMIC_MMW_OTP_7_W_otp_pdob7_w_START  (0)
#define PMIC_MMW_OTP_7_W_otp_pdob7_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_8_W_UNION
 结构说明  : OTP_8_W 寄存器结构定义。地址偏移量:0x098，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器8。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob8_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob8相或后送到数模接口otp_d2a_pdob4 */
    } reg;
} PMIC_MMW_OTP_8_W_UNION;
#endif
#define PMIC_MMW_OTP_8_W_otp_pdob8_w_START  (0)
#define PMIC_MMW_OTP_8_W_otp_pdob8_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_9_W_UNION
 结构说明  : OTP_9_W 寄存器结构定义。地址偏移量:0x099，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器9。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob9_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob9相或后送到数模接口otp_d2a_pdob4 */
    } reg;
} PMIC_MMW_OTP_9_W_UNION;
#endif
#define PMIC_MMW_OTP_9_W_otp_pdob9_w_START  (0)
#define PMIC_MMW_OTP_9_W_otp_pdob9_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_10_W_UNION
 结构说明  : OTP_10_W 寄存器结构定义。地址偏移量:0x09A，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器10。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob10_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob10相或后送到数模接口otp_d2a_pdob5 */
    } reg;
} PMIC_MMW_OTP_10_W_UNION;
#endif
#define PMIC_MMW_OTP_10_W_otp_pdob10_w_START  (0)
#define PMIC_MMW_OTP_10_W_otp_pdob10_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_11_W_UNION
 结构说明  : OTP_11_W 寄存器结构定义。地址偏移量:0x09B，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器11。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob11_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob11相或后送到数模接口otp0_d2a_pdob5 */
    } reg;
} PMIC_MMW_OTP_11_W_UNION;
#endif
#define PMIC_MMW_OTP_11_W_otp_pdob11_w_START  (0)
#define PMIC_MMW_OTP_11_W_otp_pdob11_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_12_W_UNION
 结构说明  : OTP_12_W 寄存器结构定义。地址偏移量:0x09C，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob12_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob12相或后送到数模接口otp0_d2a_pdob6 */
    } reg;
} PMIC_MMW_OTP_12_W_UNION;
#endif
#define PMIC_MMW_OTP_12_W_otp_pdob12_w_START  (0)
#define PMIC_MMW_OTP_12_W_otp_pdob12_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_13_W_UNION
 结构说明  : OTP_13_W 寄存器结构定义。地址偏移量:0x09D，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器13。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob13_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob13相或后送到数模接口otp0_d2a_pdob6 */
    } reg;
} PMIC_MMW_OTP_13_W_UNION;
#endif
#define PMIC_MMW_OTP_13_W_otp_pdob13_w_START  (0)
#define PMIC_MMW_OTP_13_W_otp_pdob13_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_14_W_UNION
 结构说明  : OTP_14_W 寄存器结构定义。地址偏移量:0x09E，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器14。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob14_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob14相或后送到数模接口otp0_d2a_pdob7 */
    } reg;
} PMIC_MMW_OTP_14_W_UNION;
#endif
#define PMIC_MMW_OTP_14_W_otp_pdob14_w_START  (0)
#define PMIC_MMW_OTP_14_W_otp_pdob14_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_15_W_UNION
 结构说明  : OTP_15_W 寄存器结构定义。地址偏移量:0x09F，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器15。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob15_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob15相或后送到数模接口otp0_d2a_pdob7 */
    } reg;
} PMIC_MMW_OTP_15_W_UNION;
#endif
#define PMIC_MMW_OTP_15_W_otp_pdob15_w_START  (0)
#define PMIC_MMW_OTP_15_W_otp_pdob15_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_16_W_UNION
 结构说明  : OTP_16_W 寄存器结构定义。地址偏移量:0x0A0，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器16。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob16_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob16相或后送到数模接口otp0_d2a_pdob8 */
    } reg;
} PMIC_MMW_OTP_16_W_UNION;
#endif
#define PMIC_MMW_OTP_16_W_otp_pdob16_w_START  (0)
#define PMIC_MMW_OTP_16_W_otp_pdob16_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_17_W_UNION
 结构说明  : OTP_17_W 寄存器结构定义。地址偏移量:0x0A1，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器17。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob17_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob17相或后送到数模接口otp0_d2a_pdob8 */
    } reg;
} PMIC_MMW_OTP_17_W_UNION;
#endif
#define PMIC_MMW_OTP_17_W_otp_pdob17_w_START  (0)
#define PMIC_MMW_OTP_17_W_otp_pdob17_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_18_W_UNION
 结构说明  : OTP_18_W 寄存器结构定义。地址偏移量:0x0A2，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器18。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob18_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob18相或后送到数模接口otp0_d2a_pdob9 */
    } reg;
} PMIC_MMW_OTP_18_W_UNION;
#endif
#define PMIC_MMW_OTP_18_W_otp_pdob18_w_START  (0)
#define PMIC_MMW_OTP_18_W_otp_pdob18_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_19_W_UNION
 结构说明  : OTP_19_W 寄存器结构定义。地址偏移量:0x0A3，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器19。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob19_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob19相或后送到数模接口otp0_d2a_pdob9 */
    } reg;
} PMIC_MMW_OTP_19_W_UNION;
#endif
#define PMIC_MMW_OTP_19_W_otp_pdob19_w_START  (0)
#define PMIC_MMW_OTP_19_W_otp_pdob19_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_20_W_UNION
 结构说明  : OTP_20_W 寄存器结构定义。地址偏移量:0x0A4，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器20。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob20_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob20相或后送到数模接口otp0_d2a_pdob10 */
    } reg;
} PMIC_MMW_OTP_20_W_UNION;
#endif
#define PMIC_MMW_OTP_20_W_otp_pdob20_w_START  (0)
#define PMIC_MMW_OTP_20_W_otp_pdob20_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_21_W_UNION
 结构说明  : OTP_21_W 寄存器结构定义。地址偏移量:0x0A5，初值:0x00，宽度:8
 寄存器说明: OTP预修调寄存器21。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  otp_pdob21_w : 8;  /* bit[0-7]: 当烧写识别位为0时将此寄存器与np_otp_pdob21相或后送到数模接口otp0_d2a_pdob10 */
    } reg;
} PMIC_MMW_OTP_21_W_UNION;
#endif
#define PMIC_MMW_OTP_21_W_otp_pdob21_w_START  (0)
#define PMIC_MMW_OTP_21_W_otp_pdob21_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO0_3_OCP_CTRL_UNION
 结构说明  : LDO0_3_OCP_CTRL 寄存器结构定义。地址偏移量:0x0A6，初值:0x55，宽度:8
 寄存器说明: LDO0/1/2/3 OCP自动关断控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo3_ocp_auto_stop : 2;  /* bit[0-1]: LDO3在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo2_ocp_auto_stop : 2;  /* bit[2-3]: LDO2在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo1_ocp_auto_stop : 2;  /* bit[4-5]: LDO1在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo0_ocp_auto_stop : 2;  /* bit[6-7]: LDO0在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
    } reg;
} PMIC_MMW_LDO0_3_OCP_CTRL_UNION;
#endif
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo3_ocp_auto_stop_START  (0)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo3_ocp_auto_stop_END    (1)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo2_ocp_auto_stop_START  (2)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo2_ocp_auto_stop_END    (3)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo1_ocp_auto_stop_START  (4)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo1_ocp_auto_stop_END    (5)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo0_ocp_auto_stop_START  (6)
#define PMIC_MMW_LDO0_3_OCP_CTRL_ldo0_ocp_auto_stop_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO4_7_OCP_CTRL_UNION
 结构说明  : LDO4_7_OCP_CTRL 寄存器结构定义。地址偏移量:0x0A7，初值:0x55，宽度:8
 寄存器说明: LCO4/5/6/7 OCP自动关断控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo7_ocp_auto_stop : 2;  /* bit[0-1]: LDO7在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo6_ocp_auto_stop : 2;  /* bit[2-3]: LDO6在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo5_ocp_auto_stop : 2;  /* bit[4-5]: LDO5在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo4_ocp_auto_stop : 2;  /* bit[6-7]: LDO4在发生OCP时是否自动关闭功能选择位。
                                                             X0：仅上报中断，不自动关闭，PMU不下电；
                                                             01：上报中断，自动关闭，PMU不下电；
                                                             11：上报中断，自动关闭，PMU下电。 */
    } reg;
} PMIC_MMW_LDO4_7_OCP_CTRL_UNION;
#endif
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo7_ocp_auto_stop_START  (0)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo7_ocp_auto_stop_END    (1)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo6_ocp_auto_stop_START  (2)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo6_ocp_auto_stop_END    (3)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo5_ocp_auto_stop_START  (4)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo5_ocp_auto_stop_END    (5)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo4_ocp_auto_stop_START  (6)
#define PMIC_MMW_LDO4_7_OCP_CTRL_ldo4_ocp_auto_stop_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO8_12_OCP_CTRL_UNION
 结构说明  : LDO8_12_OCP_CTRL 寄存器结构定义。地址偏移量:0x0A8，初值:0x7D，宽度:8
 寄存器说明: LDO8/LDO_CORE/LDO_BUF0/LDO_BUF_RF OCP自动关断控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_rfbuf_ocp_auto_stop : 2;  /* bit[0-1]: LDO_BUF_RF在发生OCP时是否自动关闭功能选择位。
                                                                  X0：仅上报中断，不自动关闭，PMU不下电；
                                                                  01：上报中断，自动关闭，PMU不下电；
                                                                  11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo_buf0_ocp_auto_stop  : 2;  /* bit[2-3]: LDO_BUF0在发生OCP时是否自动关闭功能选择位。
                                                                  X0：仅上报中断，不自动关闭，PMU不下电；
                                                                  01：上报中断，自动关闭，PMU不下电；
                                                                  11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo_core_ocp_auto_stop  : 2;  /* bit[4-5]: LDO_CORE在发生OCP时是否自动关闭功能选择位。
                                                                  X0：仅上报中断，不自动关闭，PMU不下电；
                                                                  01：上报中断，自动关闭，PMU不下电；
                                                                  11：上报中断，自动关闭，PMU下电。 */
        unsigned char  ldo8_ocp_auto_stop      : 2;  /* bit[6-7]: LDO8在发生OCP时是否自动关闭功能选择位。
                                                                  X0：仅上报中断，不自动关闭，PMU不下电；
                                                                  01：上报中断，自动关闭，PMU不下电；
                                                                  11：上报中断，自动关闭，PMU下电。 */
    } reg;
} PMIC_MMW_LDO8_12_OCP_CTRL_UNION;
#endif
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_rfbuf_ocp_auto_stop_START  (0)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_rfbuf_ocp_auto_stop_END    (1)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_buf0_ocp_auto_stop_START   (2)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_buf0_ocp_auto_stop_END     (3)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_core_ocp_auto_stop_START   (4)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo_core_ocp_auto_stop_END     (5)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo8_ocp_auto_stop_START       (6)
#define PMIC_MMW_LDO8_12_OCP_CTRL_ldo8_ocp_auto_stop_END         (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OCP_DEB_CTRL0_UNION
 结构说明  : OCP_DEB_CTRL0 寄存器结构定义。地址偏移量:0x0A9，初值:0x01，宽度:8
 寄存器说明: OCP滤波时间控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo_ocp_deb_sel : 2;  /* bit[0-1]: LDO的OCP滤波时间，滤波时间不支持动态切换。。
                                                          00：0.5ms
                                                          01：1ms；
                                                          10：2ms；
                                                          11：4ms。 
                                                          注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  reserved        : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_OCP_DEB_CTRL0_UNION;
#endif
#define PMIC_MMW_OCP_DEB_CTRL0_ldo_ocp_deb_sel_START  (0)
#define PMIC_MMW_OCP_DEB_CTRL0_ldo_ocp_deb_sel_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_OCP_DEB_CTRL1_UNION
 结构说明  : OCP_DEB_CTRL1 寄存器结构定义。地址偏移量:0x0AA，初值:0x15，宽度:8
 寄存器说明: OCP滤波时间控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  vin_ldo_0p9_deb_sel : 2;  /* bit[0-1]: vin_ldo_0p9的滤波时间。滤波时间不支持动态切换。
                                                              00：0.5ms
                                                              01：1ms；
                                                              10：2ms；
                                                              11：4ms。 
                                                               注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  vin_ldo_1p1_deb_sel : 2;  /* bit[2-3]: vin_ldo_1p1的滤波时间。滤波时间不支持动态切换。
                                                              00：0.5ms
                                                              01：1ms；
                                                              10：2ms；
                                                              11：4ms。 
                                                               注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  vin_ldo_1p3_deb_sel : 2;  /* bit[4-5]: vin_ldo_1p3的滤波时间。滤波时间不支持动态切换。
                                                              00：0.5ms
                                                              01：1ms；
                                                              10：2ms；
                                                              11：4ms。 
                                                               注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  reserved            : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_OCP_DEB_CTRL1_UNION;
#endif
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_0p9_deb_sel_START  (0)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_0p9_deb_sel_END    (1)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p1_deb_sel_START  (2)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p1_deb_sel_END    (3)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p3_deb_sel_START  (4)
#define PMIC_MMW_OCP_DEB_CTRL1_vin_ldo_1p3_deb_sel_END    (5)


/*****************************************************************************
 结构名    : PMIC_MMW_THSD_140_DEB_CTRL_UNION
 结构说明  : THSD_140_DEB_CTRL 寄存器结构定义。地址偏移量:0x0AB，初值:0x02，宽度:8
 寄存器说明: 过温140度关机滤波时间选择寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  thsd_otmp140_deb_sel : 2;  /* bit[0-1]: 温度超过140℃指示信号去抖时间。滤波时间不支持动态切换。
                                                               00：60us；
                                                               01：180us；
                                                               10：540us；
                                                               11：1ms； 
                                                               注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  reserved             : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_THSD_140_DEB_CTRL_UNION;
#endif
#define PMIC_MMW_THSD_140_DEB_CTRL_thsd_otmp140_deb_sel_START  (0)
#define PMIC_MMW_THSD_140_DEB_CTRL_thsd_otmp140_deb_sel_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_OCP_SCP_ONOFF_UNION
 结构说明  : OCP_SCP_ONOFF 寄存器结构定义。地址偏移量:0x0AC，初值:0x0F，宽度:8
 寄存器说明: OCP和DOCP滤波开关寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  en_vin_ldo_0p9_deb : 1;  /* bit[0]  : vin_ldo_0p9滤波功能使能位。
                                                             0：vin_ldo_0p9_vld滤波不使能；
                                                             1：vin_ldo_0p9_vld滤波使能。
                                                             注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  en_vin_ldo_1p1_deb : 1;  /* bit[1]  : vin_ldo_1p1滤波功能使能位。
                                                             0：vin_ldo_1p1_vld滤波不使能；
                                                             1：vin_ldo_1p1_vld滤波使能。
                                                             注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  en_vin_ldo_1p3_deb : 1;  /* bit[2]  : vin_ldo_1p3滤波功能使能位。
                                                             0：vin_ldo_1p3_vld滤波不使能；
                                                             1：vin_ldo_1p3_vld滤波使能。
                                                             注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  en_ldo_ocp_deb     : 1;  /* bit[3]  : LDO OCP滤波功能使能位。
                                                             0：LDO OCP 滤波不使能；
                                                             1：LDO OCP 滤波使能。
                                                             注：寄存器需要将寄存器LOCK配置为8'h7D，此寄存器才能配置生效。读不受限。 */
        unsigned char  reserved           : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_OCP_SCP_ONOFF_UNION;
#endif
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_0p9_deb_START  (0)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_0p9_deb_END    (0)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p1_deb_START  (1)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p1_deb_END    (1)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p3_deb_START  (2)
#define PMIC_MMW_OCP_SCP_ONOFF_en_vin_ldo_1p3_deb_END    (2)
#define PMIC_MMW_OCP_SCP_ONOFF_en_ldo_ocp_deb_START      (3)
#define PMIC_MMW_OCP_SCP_ONOFF_en_ldo_ocp_deb_END        (3)


/*****************************************************************************
 结构名    : PMIC_MMW_HARDWIRE_CTRL0_UNION
 结构说明  : HARDWIRE_CTRL0 寄存器结构定义。地址偏移量:0x0AD，初值:0x3F，宽度:8
 寄存器说明: 硬线屏蔽寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_buf0_hd_mask : 1;  /* bit[0]  : 是否屏蔽“SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en”组合逻辑对ldo_buf0的开/关控制作用。
                                                               0：不屏蔽；
                                                               1：屏蔽。 */
        unsigned char  reg_ldo_buf1_hd_mask : 1;  /* bit[1]  : 是否屏蔽“SYS_CLK_EN|reg_xo_ufs_en”组合逻辑对ldo_buf1的开/关控制作用。
                                                               0：不屏蔽；
                                                               1：屏蔽。 */
        unsigned char  reg_pmuh_hd_mask     : 1;  /* bit[2]  : 是否屏蔽“SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN”组合逻辑对pmuh的开/关控制作用。
                                                               0：不屏蔽；
                                                               1：屏蔽。 */
        unsigned char  reg_ldo_core_hd_mask : 1;  /* bit[3]  : 是否屏蔽“SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_ufs_en|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en|reg_xo_sys_en|reg_xo_rf0_en|reg_xo_rf1_en|reg_xo_rf2_en|reg_xo_rf3_en|reg_xo_rf4_en”组合逻辑对ldo_core的eco信号拉低/拉高控制作用。
                                                               0：不屏蔽；
                                                               1：屏蔽。 */
        unsigned char  reg_eco_in_hd_mask   : 1;  /* bit[4]  : 是否屏蔽“SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_ufs_en|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en|reg_xo_sys_en|reg_xo_rf0_en|reg_xo_rf1_en|reg_xo_rf2_en|reg_xo_rf3_en|reg_xo_rf4_en”组合逻辑对eco_in_n的拉低/拉高控制作用。
                                                               0：不屏蔽；
                                                               1：屏蔽。 */
        unsigned char  reg_thsd_eco_hd_mask : 1;  /* bit[5]  : 是否屏蔽“SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_ufs_en|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en|reg_xo_sys_en|reg_xo_rf0_en|reg_xo_rf1_en|reg_xo_rf2_en|reg_xo_rf3_en|reg_xo_rf4_en”组合逻辑对thsd的eco控制作用。
                                                               0：不屏蔽；
                                                               1：屏蔽。 */
        unsigned char  reserved             : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_HARDWIRE_CTRL0_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf0_hd_mask_START  (0)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf0_hd_mask_END    (0)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf1_hd_mask_START  (1)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_buf1_hd_mask_END    (1)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_pmuh_hd_mask_START      (2)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_pmuh_hd_mask_END        (2)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_core_hd_mask_START  (3)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_ldo_core_hd_mask_END    (3)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_eco_in_hd_mask_START    (4)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_eco_in_hd_mask_END      (4)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_thsd_eco_hd_mask_START  (5)
#define PMIC_MMW_HARDWIRE_CTRL0_reg_thsd_eco_hd_mask_END    (5)


/*****************************************************************************
 结构名    : PMIC_MMW_HARDWIRE_CTRL1_UNION
 结构说明  : HARDWIRE_CTRL1 寄存器结构定义。地址偏移量:0x0AE，初值:0x00，宽度:8
 寄存器说明: 硬线屏蔽寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_ldo_core_vset_mask  : 1;  /* bit[0]  : LDO_CORE电压是否受控于组合逻辑进入ECO模式的目标电压。 0：不受控组合逻辑调压； 1：受控于SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_ufs_en|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en|reg_xo_sys_en|reg_xo_rf0_en|reg_xo_rf1_en|reg_xo_rf2_en|reg_xo_rf3_en|reg_xo_rf4_en组合逻辑控制调压；  */
        unsigned char  reg_xo_inbuff_curr_mask : 1;  /* bit[1]  : XO_INBUFF电流是否受控于组合逻辑进入ECO模式的目标电流。
                                                                  0：不受控组合逻辑调电流；
                                                                  1：受控于SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_ufs_en|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en|reg_xo_sys_en|reg_xo_rf0_en|reg_xo_rf1_en|reg_xo_rf2_en|reg_xo_rf3_en|reg_xo_rf4_en组合逻辑控制调电流； */
        unsigned char  reg_xo_core_curr_mask   : 1;  /* bit[2]  : XO_CORE电流是否受控于组合逻辑进入ECO模式的目标电流。
                                                                  0：不受控组合逻辑调电流；
                                                                  1：受控于SYS_CLK_EN|WIFI_CLK_EN|NFC_CLK_EN|reg_xo_ufs_en|reg_xo_serdes_en|reg_xo_wifi0_en|reg_xo_wifi1_en|reg_xo_nfc_en|reg_xo_sys_en|reg_xo_rf0_en|reg_xo_rf1_en|reg_xo_rf2_en|reg_xo_rf3_en|reg_xo_rf4_en组合逻辑控制调电流； */
        unsigned char  reserved                : 5;  /* bit[3-7]: 保留。 */
    } reg;
} PMIC_MMW_HARDWIRE_CTRL1_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL1_reg_ldo_core_vset_mask_START   (0)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_ldo_core_vset_mask_END     (0)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_inbuff_curr_mask_START  (1)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_inbuff_curr_mask_END    (1)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_core_curr_mask_START    (2)
#define PMIC_MMW_HARDWIRE_CTRL1_reg_xo_core_curr_mask_END      (2)


/*****************************************************************************
 结构名    : PMIC_MMW_HARDWIRE_CTRL2_UNION
 结构说明  : HARDWIRE_CTRL2 寄存器结构定义。地址偏移量:0x0AF，初值:0x1F，宽度:8
 寄存器说明: 硬线屏蔽寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_serdes_clk_hd_mask : 1;  /* bit[0]  : 是否屏蔽硬线SYS_CLK_EN对serdes_clk的开/关控制作用。
                                                                 0：不屏蔽；
                                                                 1：屏蔽。 */
        unsigned char  reg_sys_clk_hd_mask    : 1;  /* bit[1]  : 是否屏蔽硬线SYS_CLK_EN对sys_clk的开/关控制作用。
                                                                 0：不屏蔽；
                                                                 1：屏蔽。 */
        unsigned char  reg_wifi0_clk_hd_mask  : 1;  /* bit[2]  : 是否屏蔽硬线WIFI_CLK_EN对wifi0_clk的开/关控制作用。
                                                                 0：不屏蔽；
                                                                 1：屏蔽。 */
        unsigned char  reg_wifi1_clk_hd_mask  : 1;  /* bit[3]  : 是否屏蔽硬线WIFI_CLK_EN对wifi1_clk的开/关控制作用。
                                                                 0：不屏蔽；
                                                                 1：屏蔽。 */
        unsigned char  reg_nfc_clk_hd_mask    : 1;  /* bit[4]  : 是否屏蔽硬线NFC_CLK_EN对nfc_clk的开/关控制作用。
                                                                 0：不屏蔽；
                                                                 1：屏蔽。 */
        unsigned char  reserved               : 3;  /* bit[5-7]: 保留。 */
    } reg;
} PMIC_MMW_HARDWIRE_CTRL2_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL2_reg_serdes_clk_hd_mask_START  (0)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_serdes_clk_hd_mask_END    (0)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_sys_clk_hd_mask_START     (1)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_sys_clk_hd_mask_END       (1)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi0_clk_hd_mask_START   (2)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi0_clk_hd_mask_END     (2)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi1_clk_hd_mask_START   (3)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_wifi1_clk_hd_mask_END     (3)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_nfc_clk_hd_mask_START     (4)
#define PMIC_MMW_HARDWIRE_CTRL2_reg_nfc_clk_hd_mask_END       (4)


/*****************************************************************************
 结构名    : PMIC_MMW_HARDWIRE_CTRL3_UNION
 结构说明  : HARDWIRE_CTRL3 寄存器结构定义。地址偏移量:0x0B0，初值:0x5A，宽度:8
 寄存器说明: 硬线屏蔽寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_sys_pmu_clk_hd_mask : 8;  /* bit[0-7]: 是否屏蔽硬线SYS_CLK_EN对pmu_clk的开/关控制作用。
                                                                  8'hAC：不屏蔽；
                                                                  8'h5A：屏蔽（默认）。 */
    } reg;
} PMIC_MMW_HARDWIRE_CTRL3_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL3_reg_sys_pmu_clk_hd_mask_START  (0)
#define PMIC_MMW_HARDWIRE_CTRL3_reg_sys_pmu_clk_hd_mask_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_HARDWIRE_CTRL4_UNION
 结构说明  : HARDWIRE_CTRL4 寄存器结构定义。地址偏移量:0x0B1，初值:0x5A，宽度:8
 寄存器说明: 硬线屏蔽寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_wifi_pmu_clk_hd_mask : 8;  /* bit[0-7]: 是否屏蔽硬线WIFI_CLK_EN对pmu_clk的开/关控制作用。
                                                                   8'hAC：不屏蔽；
                                                                   8'h5A：屏蔽（默认）。 */
    } reg;
} PMIC_MMW_HARDWIRE_CTRL4_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL4_reg_wifi_pmu_clk_hd_mask_START  (0)
#define PMIC_MMW_HARDWIRE_CTRL4_reg_wifi_pmu_clk_hd_mask_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_HARDWIRE_CTRL5_UNION
 结构说明  : HARDWIRE_CTRL5 寄存器结构定义。地址偏移量:0x0B2，初值:0x5A，宽度:8
 寄存器说明: 硬线屏蔽寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  reg_nfc_pmu_clk_hd_mask : 8;  /* bit[0-7]: 是否屏蔽硬线NFC_CLK_EN对pmu_clk的开/关控制作用。
                                                                  8'hAC：不屏蔽；
                                                                  8'h5A：屏蔽（默认）。 */
    } reg;
} PMIC_MMW_HARDWIRE_CTRL5_UNION;
#endif
#define PMIC_MMW_HARDWIRE_CTRL5_reg_nfc_pmu_clk_hd_mask_START  (0)
#define PMIC_MMW_HARDWIRE_CTRL5_reg_nfc_pmu_clk_hd_mask_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_LOCK_UNION
 结构说明  : LOCK 寄存器结构定义。地址偏移量:0x0B3，初值:0x00，宽度:8
 寄存器说明: 下电区写屏蔽控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  lock : 8;  /* bit[0-7]: 为防止软件误写reg_ldo_buf0_en，reg_ldo_buf1_en，reg_ldo_core_en，reg_pmuh_en，OCP_DEB_CTRL0，OCP_DEB_CTRL1，THSD_140_DEB_CTRL，OCP_SCP_ONOFF，reg_thsd_en导致系统出错。只有当lock写为8'h7D的时候，上述寄存器位才允许被一直写入。 */
    } reg;
} PMIC_MMW_LOCK_UNION;
#endif
#define PMIC_MMW_LOCK_lock_START  (0)
#define PMIC_MMW_LOCK_lock_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_ECO_GT_BYPASS_UNION
 结构说明  : SPMI_ECO_GT_BYPASS 寄存器结构定义。地址偏移量:0x0B4，初值:0x00，宽度:8
 寄存器说明: 软件在eco模式下对SPMI有读写需求时，提供SPMI时钟以及同步用的19.2M ssi时钟(该寄存器时钟为常开时钟)
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_eco_gt_bypass : 1;  /* bit[0]  : 软件在eco模式下对SPMI有读写需求时，提供SPMI时钟以及同步用的19.2Mssi时钟 
                                                             0：无读写需求 
                                                             1：有读写需求
                                                             注：低功耗模式下，该寄存器写1后，回读值也是1，同时内部时钟gating会打开；但写0后内部时钟gating关闭，该寄存器不能正常被回读，其他寄存器也不能正常被回读。  */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_SPMI_ECO_GT_BYPASS_UNION;
#endif
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_spmi_eco_gt_bypass_START  (0)
#define PMIC_MMW_SPMI_ECO_GT_BYPASS_spmi_eco_gt_bypass_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_LDO_SST_END_CFG_UNION
 结构说明  : LDO_SST_END_CFG 寄存器结构定义。地址偏移量:0x0B5，初值:0x00，宽度:8
 寄存器说明: LDO5和LDO6的缓启完成时间配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ldo5_sst_end_sel : 1;  /* bit[0]  : LDO5的缓启完成（ldo5_sst_end拉高）时间配置寄存器。
                                                           0：480us；
                                                           1：720us； */
        unsigned char  ldo6_sst_end_sel : 1;  /* bit[1]  : LDO6的缓启完成（ldo6_sst_end拉高）时间配置寄存器。
                                                           0：480us；
                                                           1：720us； */
        unsigned char  reserved         : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_LDO_SST_END_CFG_UNION;
#endif
#define PMIC_MMW_LDO_SST_END_CFG_ldo5_sst_end_sel_START  (0)
#define PMIC_MMW_LDO_SST_END_CFG_ldo5_sst_end_sel_END    (0)
#define PMIC_MMW_LDO_SST_END_CFG_ldo6_sst_end_sel_START  (1)
#define PMIC_MMW_LDO_SST_END_CFG_ldo6_sst_end_sel_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_UNION
 结构说明  : XO_RF_CLK_ON_TIME_SEL 寄存器结构定义。地址偏移量:0x0B6，初值:0x01，宽度:8
 寄存器说明: 非debug模式，配置reg_xo_rf0_en~reg_xo_rf4_en开启时，对应的电源和时钟使能开启时间配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  rfbuf_pwron_time_sel : 2;  /* bit[0-1]: 非debug模式下，配置reg_xo_rf0_en~reg_xo_rf4_en开启时，各自对应的电源使能信号ldo_rfbuf0_en~ldo_rfbuf4_en打开时序选择寄存器。（t1）
                                                               00：150us；
                                                               01：180us；
                                                               10：210us；
                                                               11：120us。 */
        unsigned char  xo_rf_pwron_time_sel : 2;  /* bit[2-3]: 非debug模式下，配置reg_xo_rf0_en~reg_xo_rf4_en开启时，各自对应的时钟使能信号xo_rf0_en~xo_rf4_en打开时序选择寄存器。（t2）
                                                               00：90us；
                                                               01：120us；
                                                               10：150us；
                                                               11：180us。 */
        unsigned char  reserved             : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_UNION;
#endif
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_rfbuf_pwron_time_sel_START  (0)
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_rfbuf_pwron_time_sel_END    (1)
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_xo_rf_pwron_time_sel_START  (2)
#define PMIC_MMW_XO_RF_CLK_ON_TIME_SEL_xo_rf_pwron_time_sel_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_RF_CLK_ON_DEBUG_UNION
 结构说明  : XO_RF_CLK_ON_DEBUG 寄存器结构定义。地址偏移量:0x0B7，初值:0x00，宽度:8
 寄存器说明: ldo_rfbuf_en、ldo_rfbuf0~4_en、xo_rf0~4_en开启和关闭debug模式控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  rfbuf_onoff_debug_mode : 8;  /* bit[0-7]: ldo_rfbuf_en、ldo_rfbuf0~4_en、xo_rf0~4_en开启和关闭debug模式控制寄存器：
                                                                 8'h5A：debug模式，reg_ldo_rfbuf0_en~reg_ldo_rfbuf4_en、reg_ldo_buf_rf_en寄存器可以被写入，同时reg_ldo_buf_rf_en、reg_ldo_rfbuf0_en~reg_ldo_rfbuf4_en和reg_xo_rf0~4_en只控制对应的模块开启和关闭，不按照时序关系约束。该寄存器写入其他值时退出debug模式，reg_ldo_rfbuf0~4_en、reg_ldo_buf_rf_en寄存器都不能被写入，同时ldo_rfbuf_en、ldo_rfbuf0~4_en和xo_rf0~4_en按时序关系开启和关闭。 */
    } reg;
} PMIC_MMW_XO_RF_CLK_ON_DEBUG_UNION;
#endif
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_rfbuf_onoff_debug_mode_START  (0)
#define PMIC_MMW_XO_RF_CLK_ON_DEBUG_rfbuf_onoff_debug_mode_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_CORE_CURR_DEBUG_UNION
 结构说明  : XO_CORE_CURR_DEBUG 寄存器结构定义。地址偏移量:0x0B8，初值:0x00，宽度:8
 寄存器说明: XO CORE电流debug模式配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_core_curr_debug_mode : 8;  /* bit[0-7]: normal模式和eco模式下，XO CORE电流配置寄存器的写操作控制：
                                                                  8'h5A：进入debug模式，可以对xo_core_curr_cfg和xo_core_curr_eco_cfg寄存器进行写操作。该寄存器写入其他值时退出debug模式，xo_core_curr_cfg和xo_core_curr_eco_cfg寄存器都不能被写入。 */
    } reg;
} PMIC_MMW_XO_CORE_CURR_DEBUG_UNION;
#endif
#define PMIC_MMW_XO_CORE_CURR_DEBUG_xo_core_curr_debug_mode_START  (0)
#define PMIC_MMW_XO_CORE_CURR_DEBUG_xo_core_curr_debug_mode_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_INBUFF_CURR_DEBUG_UNION
 结构说明  : XO_INBUFF_CURR_DEBUG 寄存器结构定义。地址偏移量:0x0B9，初值:0x00，宽度:8
 寄存器说明: XO INPUT BUFFER电流debug模式配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  xo_inbuff_curr_debug_mode : 8;  /* bit[0-7]: normal模式和eco模式下，XO INPUT BUFFER电流配置寄存器的写操作控制：
                                                                    8'h5A：进入debug模式，可以对xo_inbuff_curr_cfg和xo_inbuff_curr_eco_cfg寄存器进行写操作。该寄存器写入其他值时退出debug模式，xo_inbuff_curr_cfg和xo_inbuff_curr_eco_cfg寄存器都不能被写入。 */
    } reg;
} PMIC_MMW_XO_INBUFF_CURR_DEBUG_UNION;
#endif
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_xo_inbuff_curr_debug_mode_START  (0)
#define PMIC_MMW_XO_INBUFF_CURR_DEBUG_xo_inbuff_curr_debug_mode_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG0_UNION
 结构说明  : SPMI_DEBUG0 寄存器结构定义。地址偏移量:0x0F0，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug0 : 8;  /* bit[0-7]: SPMI debug寄存器0。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG0_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG0_spmi_debug0_START  (0)
#define PMIC_MMW_SPMI_DEBUG0_spmi_debug0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG1_UNION
 结构说明  : SPMI_DEBUG1 寄存器结构定义。地址偏移量:0x0F1，初值:0xF0，宽度:8
 寄存器说明: SPMI debug寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug1 : 8;  /* bit[0-7]: SPMI debug寄存器1。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG1_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG1_spmi_debug1_START  (0)
#define PMIC_MMW_SPMI_DEBUG1_spmi_debug1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG2_UNION
 结构说明  : SPMI_DEBUG2 寄存器结构定义。地址偏移量:0x0F2，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug2 : 8;  /* bit[0-7]: SPMI debug寄存器2。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG2_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG2_spmi_debug2_START  (0)
#define PMIC_MMW_SPMI_DEBUG2_spmi_debug2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG3_UNION
 结构说明  : SPMI_DEBUG3 寄存器结构定义。地址偏移量:0x0F3，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug3 : 8;  /* bit[0-7]: SPMI debug寄存器3。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG3_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG3_spmi_debug3_START  (0)
#define PMIC_MMW_SPMI_DEBUG3_spmi_debug3_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG4_UNION
 结构说明  : SPMI_DEBUG4 寄存器结构定义。地址偏移量:0x0F4，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug4 : 8;  /* bit[0-7]: SPMI debug寄存器4。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG4_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG4_spmi_debug4_START  (0)
#define PMIC_MMW_SPMI_DEBUG4_spmi_debug4_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG5_UNION
 结构说明  : SPMI_DEBUG5 寄存器结构定义。地址偏移量:0x0F5，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug5 : 8;  /* bit[0-7]: SPMI debug寄存器5。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG5_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG5_spmi_debug5_START  (0)
#define PMIC_MMW_SPMI_DEBUG5_spmi_debug5_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG6_UNION
 结构说明  : SPMI_DEBUG6 寄存器结构定义。地址偏移量:0x0F6，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug6 : 8;  /* bit[0-7]: SPMI debug寄存器6。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG6_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG6_spmi_debug6_START  (0)
#define PMIC_MMW_SPMI_DEBUG6_spmi_debug6_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SPMI_DEBUG7_UNION
 结构说明  : SPMI_DEBUG7 寄存器结构定义。地址偏移量:0x0F7，初值:0x00，宽度:8
 寄存器说明: SPMI debug寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  spmi_debug7 : 8;  /* bit[0-7]: SPMI debug寄存器7。 */
    } reg;
} PMIC_MMW_SPMI_DEBUG7_UNION;
#endif
#define PMIC_MMW_SPMI_DEBUG7_spmi_debug7_START  (0)
#define PMIC_MMW_SPMI_DEBUG7_spmi_debug7_END    (7)




/****************************************************************************
                     (2/5) PMU_IRQ_MASK
 ****************************************************************************/
/*****************************************************************************
 结构名    : PMIC_MMW_IRQ_MASK_0_UNION
 结构说明  : IRQ_MASK_0 寄存器结构定义。地址偏移量:0x100，初值:0x00，宽度:8
 寄存器说明: IRQ_MASK0控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  irq_mask_0 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                     bit[7:5]：保留使用，可读写；
                                                     bit[4]：开机态vin_ldo_1p3_vld中断屏蔽；
                                                     bit[3]：开机态vin_ldo_1p1_vld中断屏蔽；
                                                     bit[2]：开机态vin_ldo_0p9_vld中断屏蔽；
                                                     bit[1]：开机态过温125°中断屏蔽。
                                                     bit[0]：OCP中断总屏蔽。 */
    } reg;
} PMIC_MMW_IRQ_MASK_0_UNION;
#endif
#define PMIC_MMW_IRQ_MASK_0_irq_mask_0_START  (0)
#define PMIC_MMW_IRQ_MASK_0_irq_mask_0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_IRQ_MASK_1_UNION
 结构说明  : IRQ_MASK_1 寄存器结构定义。地址偏移量:0x101，初值:0x00，宽度:8
 寄存器说明: IRQ_MASK1控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  irq_mask_1 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                     bit[7]：LDO7输出过载中断屏蔽；
                                                     bit[6]：LDO6输出过载中断屏蔽；
                                                     bit[5]：LDO5输出过载中断屏蔽；
                                                     bit[4]：LDO4输出过载中断屏蔽；
                                                     bit[3]：LDO3输出过载中断屏蔽；
                                                     bit[2]：LDO2输出过载中断屏蔽；
                                                     bit[1]：LDO1输出过载中断屏蔽；
                                                     bit[0]：LDO0输出过载中断屏蔽； */
    } reg;
} PMIC_MMW_IRQ_MASK_1_UNION;
#endif
#define PMIC_MMW_IRQ_MASK_1_irq_mask_1_START  (0)
#define PMIC_MMW_IRQ_MASK_1_irq_mask_1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_IRQ_MASK_2_UNION
 结构说明  : IRQ_MASK_2 寄存器结构定义。地址偏移量:0x102，初值:0x00，宽度:8
 寄存器说明: IRQ_MASK2控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  irq_mask_2 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                     bit[7:5]：保留使用，可读写；
                                                     bit[4]：LDO_BUF_RF输出过载中断屏蔽；
                                                     bit[3]：LDO_BUF0输出过载中断屏蔽；
                                                     bit[2]：PMUH输出过载中断屏蔽；
                                                     bit[1]：LDO_CORE输出过载中断屏蔽；
                                                     bit[0]：LDO8输出过载中断屏蔽。 */
    } reg;
} PMIC_MMW_IRQ_MASK_2_UNION;
#endif
#define PMIC_MMW_IRQ_MASK_2_irq_mask_2_START  (0)
#define PMIC_MMW_IRQ_MASK_2_irq_mask_2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_MASK_0_UNION
 结构说明  : FAULT_N_MASK_0 寄存器结构定义。地址偏移量:0x103，初值:0x00，宽度:8
 寄存器说明: FAULT_N屏蔽控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_mask_0 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                       bit[7]：LDO_CORE短路的fault_n屏蔽；
                                                       bit[6]：VDDIO异常的fault_n屏蔽；
                                                       bit[5]：VIN_LDO_1P95异常的fault_n屏蔽；
                                                       bit[4]：过温140°的fault_n屏蔽；
                                                       bit[3]：过温125°的fault_n屏蔽；
                                                       bit[2]：开机电压vsys_pwron的fault_n屏蔽；
                                                       bit[1]：过压vsys_ov的fault_n屏蔽；
                                                       bit[0]：绝对欠压vsys_pwroff_abs的fault_n屏蔽。 */
    } reg;
} PMIC_MMW_FAULT_N_MASK_0_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_0_fault_mask_0_START  (0)
#define PMIC_MMW_FAULT_N_MASK_0_fault_mask_0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_MASK_1_UNION
 结构说明  : FAULT_N_MASK_1 寄存器结构定义。地址偏移量:0x104，初值:0x00，宽度:8
 寄存器说明: FAULT_N屏蔽控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_mask_1 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                       bit[7]：LDO7输出过载的fault_n屏蔽；
                                                       bit[6]：LDO6输出过载的fault_n屏蔽；
                                                       bit[5]：LDO5输出过载的fault_n屏蔽；
                                                       bit[4]：LDO4输出过载的fault_n屏蔽；
                                                       bit[3]：LDO3输出过载的fault_n屏蔽；
                                                       bit[2]：LDO2输出过载的fault_n屏蔽；
                                                       bit[1]：LDO1输出过载的fault_n屏蔽；
                                                       bit[0]：LDO0输出过载的fault_n屏蔽； */
    } reg;
} PMIC_MMW_FAULT_N_MASK_1_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_1_fault_mask_1_START  (0)
#define PMIC_MMW_FAULT_N_MASK_1_fault_mask_1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_MASK_2_UNION
 结构说明  : FAULT_N_MASK_2 寄存器结构定义。地址偏移量:0x105，初值:0x00，宽度:8
 寄存器说明: FAULT_N屏蔽控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_mask_2 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                       bit[7:6]：保留使用，可读写；
                                                       bit[5]：PMUH输出短路的fault_n屏蔽；
                                                       bit[4]：LDO_BUF_RF输出过载的fault_n屏蔽；
                                                       bit[3]：LDO_BUF0输出过载的fault_n屏蔽；
                                                       bit[2]：PMUH输出过载的fault_n屏蔽；
                                                       bit[1]：LDO_CORE输出过载的fault_n屏蔽；
                                                       bit[0]：LDO8输出过载的fault_n屏蔽。 */
    } reg;
} PMIC_MMW_FAULT_N_MASK_2_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_2_fault_mask_2_START  (0)
#define PMIC_MMW_FAULT_N_MASK_2_fault_mask_2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_MASK_3_UNION
 结构说明  : FAULT_N_MASK_3 寄存器结构定义。地址偏移量:0x106，初值:0x00，宽度:8
 寄存器说明: FAULT_N屏蔽控制寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_mask_3 : 8;  /* bit[0-7]: 中断mask，写1屏蔽。
                                                       bit[7:4]：保留使用，可读写；
                                                       bit[3]: vin_ldom_vld在30ms一直没拉高，开机条件不满足的fault_n屏蔽；
                                                       bit[2]：开机态CLK_BUF_EN拉低的fault_n屏蔽；
                                                       bit[1]：19.2MHz时钟一直都没有，计时超时的fault_n屏蔽；
                                                       bit[0]：19.2MHz时钟先有后丢失的fault_n屏蔽； */
    } reg;
} PMIC_MMW_FAULT_N_MASK_3_UNION;
#endif
#define PMIC_MMW_FAULT_N_MASK_3_fault_mask_3_START  (0)
#define PMIC_MMW_FAULT_N_MASK_3_fault_mask_3_END    (7)




/****************************************************************************
                     (3/5) PMU_IRQ
 ****************************************************************************/
/*****************************************************************************
 结构名    : PMIC_MMW_IRQ0_UNION
 结构说明  : IRQ0 寄存器结构定义。地址偏移量:0x10A，初值:0x00，宽度:8
 寄存器说明: 中断寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ocp_r                   : 1;  /* bit[0]  : OCP总中断
                                                                  0：无此中断；
                                                                  1：表示至少有一个LDO发生过流保护，CPU查询到后要去查看下面的过流保存寄存器查看是哪一个发生过流，并将其写1清后，中断消除。 */
        unsigned char  irq_thsd_otmp125_d120ur : 1;  /* bit[1]  : 开机态过温125°中断(上升沿去抖120us)。
                                                                  0：无中断；
                                                                  1：有中断。 */
        unsigned char  vin_ldo_0p9_vld_d1mf    : 1;  /* bit[2]  : 开机态VIN_LDO_0P9异常中断（默认下降沿去抖1ms）。
                                                                  0：无中断；
                                                                  1：有中断。 */
        unsigned char  vin_ldo_1p1_vld_d1mf    : 1;  /* bit[3]  : 开机态VIN_LDO_1P1异常中断（默认下降沿去抖1ms）。
                                                                  0：无中断；
                                                                  1：有中断。 */
        unsigned char  vin_ldo_1p3_vld_d1mf    : 1;  /* bit[4]  : 开机态VIN_LDO_1P3异常中断（默认下降沿去抖1ms）。
                                                                  0：无中断；
                                                                  1：有中断。 */
        unsigned char  reserved                : 3;  /* bit[5-7]: 保留。 */
    } reg;
} PMIC_MMW_IRQ0_UNION;
#endif
#define PMIC_MMW_IRQ0_ocp_r_START                    (0)
#define PMIC_MMW_IRQ0_ocp_r_END                      (0)
#define PMIC_MMW_IRQ0_irq_thsd_otmp125_d120ur_START  (1)
#define PMIC_MMW_IRQ0_irq_thsd_otmp125_d120ur_END    (1)
#define PMIC_MMW_IRQ0_vin_ldo_0p9_vld_d1mf_START     (2)
#define PMIC_MMW_IRQ0_vin_ldo_0p9_vld_d1mf_END       (2)
#define PMIC_MMW_IRQ0_vin_ldo_1p1_vld_d1mf_START     (3)
#define PMIC_MMW_IRQ0_vin_ldo_1p1_vld_d1mf_END       (3)
#define PMIC_MMW_IRQ0_vin_ldo_1p3_vld_d1mf_START     (4)
#define PMIC_MMW_IRQ0_vin_ldo_1p3_vld_d1mf_END       (4)


/*****************************************************************************
 结构名    : PMIC_MMW_OCP_IRQ0_UNION
 结构说明  : OCP_IRQ0 寄存器结构定义。地址偏移量:0x10B，初值:0x00，宽度:8
 寄存器说明: OCP中断寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ocpldo0 : 1;  /* bit[0]: LDO0输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo1 : 1;  /* bit[1]: LDO1输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo2 : 1;  /* bit[2]: LDO2输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo3 : 1;  /* bit[3]: LDO3输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo4 : 1;  /* bit[4]: LDO4输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo5 : 1;  /* bit[5]: LDO5输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo6 : 1;  /* bit[6]: LDO6输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
        unsigned char  ocpldo7 : 1;  /* bit[7]: LDO7输出过载中断。
                                                0：无中断；
                                                1：有中断。 */
    } reg;
} PMIC_MMW_OCP_IRQ0_UNION;
#endif
#define PMIC_MMW_OCP_IRQ0_ocpldo0_START  (0)
#define PMIC_MMW_OCP_IRQ0_ocpldo0_END    (0)
#define PMIC_MMW_OCP_IRQ0_ocpldo1_START  (1)
#define PMIC_MMW_OCP_IRQ0_ocpldo1_END    (1)
#define PMIC_MMW_OCP_IRQ0_ocpldo2_START  (2)
#define PMIC_MMW_OCP_IRQ0_ocpldo2_END    (2)
#define PMIC_MMW_OCP_IRQ0_ocpldo3_START  (3)
#define PMIC_MMW_OCP_IRQ0_ocpldo3_END    (3)
#define PMIC_MMW_OCP_IRQ0_ocpldo4_START  (4)
#define PMIC_MMW_OCP_IRQ0_ocpldo4_END    (4)
#define PMIC_MMW_OCP_IRQ0_ocpldo5_START  (5)
#define PMIC_MMW_OCP_IRQ0_ocpldo5_END    (5)
#define PMIC_MMW_OCP_IRQ0_ocpldo6_START  (6)
#define PMIC_MMW_OCP_IRQ0_ocpldo6_END    (6)
#define PMIC_MMW_OCP_IRQ0_ocpldo7_START  (7)
#define PMIC_MMW_OCP_IRQ0_ocpldo7_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OCP_IRQ1_UNION
 结构说明  : OCP_IRQ1 寄存器结构定义。地址偏移量:0x10C，初值:0x00，宽度:8
 寄存器说明: OCP中断寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  ocpldo8       : 1;  /* bit[0]  : LDO8输出过载中断。
                                                        0：无中断；
                                                        1：有中断。 */
        unsigned char  ocpldo_core   : 1;  /* bit[1]  : LDO_CORE输出过载中断。
                                                        0：无中断；
                                                        1：有中断。 */
        unsigned char  pmuh_ocp      : 1;  /* bit[2]  : PMUH输出过载中断。
                                                        0：无中断；
                                                        1：有中断。 */
        unsigned char  ocpldo_buf0   : 1;  /* bit[3]  : LDO_BUF0输出过载中断。
                                                        0：无中断；
                                                        1：有中断。 */
        unsigned char  ocpldo_buf_rf : 1;  /* bit[4]  : LDO_BUF_RF输出过载中断。
                                                        0：无中断；
                                                        1：有中断。 */
        unsigned char  reserved      : 3;  /* bit[5-7]: 保留。 */
    } reg;
} PMIC_MMW_OCP_IRQ1_UNION;
#endif
#define PMIC_MMW_OCP_IRQ1_ocpldo8_START        (0)
#define PMIC_MMW_OCP_IRQ1_ocpldo8_END          (0)
#define PMIC_MMW_OCP_IRQ1_ocpldo_core_START    (1)
#define PMIC_MMW_OCP_IRQ1_ocpldo_core_END      (1)
#define PMIC_MMW_OCP_IRQ1_pmuh_ocp_START       (2)
#define PMIC_MMW_OCP_IRQ1_pmuh_ocp_END         (2)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf0_START    (3)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf0_END      (3)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf_rf_START  (4)
#define PMIC_MMW_OCP_IRQ1_ocpldo_buf_rf_END    (4)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_0_UNION
 结构说明  : FAULT_N_0 寄存器结构定义。地址偏移量:0x10D，初值:0x00，宽度:8
 寄存器说明: FAULT_N上报寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  vsys_pwroff_abs_d20nr     : 1;  /* bit[0]: vsys_pwroff_abs异常FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  vsys_ov_d200ur            : 1;  /* bit[1]: vsys_ov异常FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  vsys_pwron_d60ur          : 1;  /* bit[2]: vsys_pwron异常FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  fault_thsd_otmp125_d120ur : 1;  /* bit[3]: 开机过程中过温125°异常FAULT_N中断(上升沿去抖120us)。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  thsd_otmp140_d540ur       : 1;  /* bit[4]: 过温140°异常FAULT_N中断(默认上升沿去抖540us)。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  vin_ldom_vld_deb          : 1;  /* bit[5]: vin_ldom_vld异常FAULT_N中断（VDD_LDO_1P95是否拉低）。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  vin_io_vld_d5m            : 1;  /* bit[6]: 开机态vin_io_vld异常FAULT_N中断（VDD_IO是否拉低）。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  ldo_core_vld_d20nf        : 1;  /* bit[7]: LDO_CORE输出电源异常FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
    } reg;
} PMIC_MMW_FAULT_N_0_UNION;
#endif
#define PMIC_MMW_FAULT_N_0_vsys_pwroff_abs_d20nr_START      (0)
#define PMIC_MMW_FAULT_N_0_vsys_pwroff_abs_d20nr_END        (0)
#define PMIC_MMW_FAULT_N_0_vsys_ov_d200ur_START             (1)
#define PMIC_MMW_FAULT_N_0_vsys_ov_d200ur_END               (1)
#define PMIC_MMW_FAULT_N_0_vsys_pwron_d60ur_START           (2)
#define PMIC_MMW_FAULT_N_0_vsys_pwron_d60ur_END             (2)
#define PMIC_MMW_FAULT_N_0_fault_thsd_otmp125_d120ur_START  (3)
#define PMIC_MMW_FAULT_N_0_fault_thsd_otmp125_d120ur_END    (3)
#define PMIC_MMW_FAULT_N_0_thsd_otmp140_d540ur_START        (4)
#define PMIC_MMW_FAULT_N_0_thsd_otmp140_d540ur_END          (4)
#define PMIC_MMW_FAULT_N_0_vin_ldom_vld_deb_START           (5)
#define PMIC_MMW_FAULT_N_0_vin_ldom_vld_deb_END             (5)
#define PMIC_MMW_FAULT_N_0_vin_io_vld_d5m_START             (6)
#define PMIC_MMW_FAULT_N_0_vin_io_vld_d5m_END               (6)
#define PMIC_MMW_FAULT_N_0_ldo_core_vld_d20nf_START         (7)
#define PMIC_MMW_FAULT_N_0_ldo_core_vld_d20nf_END           (7)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_1_UNION
 结构说明  : FAULT_N_1 寄存器结构定义。地址偏移量:0x10E，初值:0x00，宽度:8
 寄存器说明: FAULT_N上报寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_ocpldo0 : 1;  /* bit[0]: LDO0 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo1 : 1;  /* bit[1]: LDO1 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo2 : 1;  /* bit[2]: LDO2 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo3 : 1;  /* bit[3]: LDO3 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo4 : 1;  /* bit[4]: LDO4 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo5 : 1;  /* bit[5]: LDO5 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo6 : 1;  /* bit[6]: LDO6 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo7 : 1;  /* bit[7]: LDO7 OCP为模式三时输出过载FAULT_N中断。
                                                      0：无FAULT_N中断；
                                                      1：有FAULT_N中断。 */
    } reg;
} PMIC_MMW_FAULT_N_1_UNION;
#endif
#define PMIC_MMW_FAULT_N_1_fault_ocpldo0_START  (0)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo0_END    (0)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo1_START  (1)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo1_END    (1)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo2_START  (2)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo2_END    (2)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo3_START  (3)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo3_END    (3)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo4_START  (4)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo4_END    (4)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo5_START  (5)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo5_END    (5)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo6_START  (6)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo6_END    (6)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo7_START  (7)
#define PMIC_MMW_FAULT_N_1_fault_ocpldo7_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_2_UNION
 结构说明  : FAULT_N_2 寄存器结构定义。地址偏移量:0x10F，初值:0x00，宽度:8
 寄存器说明: FAULT_N上报寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_ocpldo8       : 1;  /* bit[0]  : LDO8 OCP为模式三时输出过载FAULT_N中断。
                                                              0：无FAULT_N中断；
                                                              1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo_core   : 1;  /* bit[1]  : LDO_CORE输出过载FAULT_N中断。
                                                              0：无FAULT_N中断；
                                                              1：有FAULT_N中断。 */
        unsigned char  fault_pmuh_ocp      : 1;  /* bit[2]  : PMUH输出过载FAULT_N中断。
                                                              0：无FAULT_N中断；
                                                              1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo_buf0   : 1;  /* bit[3]  : LDO_BUF0输出过载FAULT_N中断。
                                                              0：无FAULT_N中断；
                                                              1：有FAULT_N中断。 */
        unsigned char  fault_ocpldo_buf_rf : 1;  /* bit[4]  : LDO_BUF_RF OCP为模式三时输出过载FAULT_N中断。
                                                              0：无FAULT_N中断；
                                                              1：有FAULT_N中断。 */
        unsigned char  pmuh_short_n_d20nf  : 1;  /* bit[5]  : PMUH输出短路FAULT_N中断。
                                                              0：无FAULT_N中断；
                                                              1：有FAULT_N中断。 */
        unsigned char  reserved            : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_FAULT_N_2_UNION;
#endif
#define PMIC_MMW_FAULT_N_2_fault_ocpldo8_START        (0)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo8_END          (0)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_core_START    (1)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_core_END      (1)
#define PMIC_MMW_FAULT_N_2_fault_pmuh_ocp_START       (2)
#define PMIC_MMW_FAULT_N_2_fault_pmuh_ocp_END         (2)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf0_START    (3)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf0_END      (3)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf_rf_START  (4)
#define PMIC_MMW_FAULT_N_2_fault_ocpldo_buf_rf_END    (4)
#define PMIC_MMW_FAULT_N_2_pmuh_short_n_d20nf_START   (5)
#define PMIC_MMW_FAULT_N_2_pmuh_short_n_d20nf_END     (5)


/*****************************************************************************
 结构名    : PMIC_MMW_FAULT_N_3_UNION
 结构说明  : FAULT_N_3 寄存器结构定义。地址偏移量:0x110，初值:0x00，宽度:8
 寄存器说明: FAULT_N上报寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  fault_xo_19m2_dis       : 1;  /* bit[0]  : 19.2MHz时钟先有后丢失FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。
                                                                  注：下电过程不报（正常下电和异常下电） */
        unsigned char  fault_19m2_dis_time_out : 1;  /* bit[1]  : 19.2MHz时钟一直都没有，计时超时FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。
                                                                  注：下电过程不报（正常下电和异常下电） */
        unsigned char  fault_buf_en_d5mf       : 1;  /* bit[2]  : 开机态CLK_BUF_EN拉低5ms后触发关机的FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  fault_vin_ldom_time_out : 1;  /* bit[3]  : vin_ldom_vld在30ms一直没拉高，开机条件不满足FAULT_N中断。
                                                                  0：无FAULT_N中断；
                                                                  1：有FAULT_N中断。 */
        unsigned char  reserved                : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_FAULT_N_3_UNION;
#endif
#define PMIC_MMW_FAULT_N_3_fault_xo_19m2_dis_START        (0)
#define PMIC_MMW_FAULT_N_3_fault_xo_19m2_dis_END          (0)
#define PMIC_MMW_FAULT_N_3_fault_19m2_dis_time_out_START  (1)
#define PMIC_MMW_FAULT_N_3_fault_19m2_dis_time_out_END    (1)
#define PMIC_MMW_FAULT_N_3_fault_buf_en_d5mf_START        (2)
#define PMIC_MMW_FAULT_N_3_fault_buf_en_d5mf_END          (2)
#define PMIC_MMW_FAULT_N_3_fault_vin_ldom_time_out_START  (3)
#define PMIC_MMW_FAULT_N_3_fault_vin_ldom_time_out_END    (3)




/****************************************************************************
                     (4/5) NP_PMU_EVENT
 ****************************************************************************/
/*****************************************************************************
 结构名    : PMIC_MMW_NP_RECORD0_UNION
 结构说明  : NP_RECORD0 寄存器结构定义。地址偏移量:0x114，初值:0x00，宽度:8
 寄存器说明: 非下电事件记录寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  npirq_thsd_otmp125_d120ur : 1;  /* bit[0]  : 开机态过温125°事件记录寄存器(上升沿去抖120us)。
                                                                    0：没有发生过该事件；
                                                                    1：发生过该事件。 */
        unsigned char  np_vin_ldo_0p9_vld_d1mf   : 1;  /* bit[1]  : 开机态VIN_LDO_0P9异常事件记录寄存器（默认下降沿去抖1ms）。
                                                                    0：没有发生过该事件；
                                                                    1：发生过该事件。 */
        unsigned char  np_vin_ldo_1p1_vld_d1mf   : 1;  /* bit[2]  : 开机态VIN_LDO_1P1异常事件记录寄存器（默认下降沿去抖1ms）。
                                                                    0：没有发生过该事件；
                                                                    1：发生过该事件。 */
        unsigned char  np_vin_ldo_1p3_vld_d1mf   : 1;  /* bit[3]  : 开机态VIN_LDO_1P3异常事件记录寄存器（默认下降沿去抖1ms）。
                                                                    0：没有发生过该事件；
                                                                    1：发生过该事件。 */
        unsigned char  reserved                  : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_NP_RECORD0_UNION;
#endif
#define PMIC_MMW_NP_RECORD0_npirq_thsd_otmp125_d120ur_START  (0)
#define PMIC_MMW_NP_RECORD0_npirq_thsd_otmp125_d120ur_END    (0)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_0p9_vld_d1mf_START    (1)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_0p9_vld_d1mf_END      (1)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p1_vld_d1mf_START    (2)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p1_vld_d1mf_END      (2)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p3_vld_d1mf_START    (3)
#define PMIC_MMW_NP_RECORD0_np_vin_ldo_1p3_vld_d1mf_END      (3)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_RECORD1_UNION
 结构说明  : NP_RECORD1 寄存器结构定义。地址偏移量:0x115，初值:0x00，宽度:8
 寄存器说明: 非下电事件记录寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_vsys_pwroff_abs_d20nr : 1;  /* bit[0]: vsys_pwroff_abs异常事件记录寄存器。
                                                                 0：无FAULT_N中断；
                                                                 1：有FAULT_N中断。 */
        unsigned char  np_vsys_ov_d200ur        : 1;  /* bit[1]: vsys_ov异常事件记录寄存器。
                                                                 0：无FAULT_N中断；
                                                                 1：有FAULT_N中断。 */
        unsigned char  np_vsys_pwron_d60ur      : 1;  /* bit[2]: vsys_pwron异常事件记录寄存器。
                                                                 0：无FAULT_N中断；
                                                                 1：有FAULT_N中断。 */
        unsigned char  np_thsd_otmp125_d120ur   : 1;  /* bit[3]: 上电过程中过温125°异常事件记录寄存器(上升沿去抖120us)。
                                                                 0：没有发生过该事件；
                                                                 1：发生过该事件。 */
        unsigned char  np_thsd_otmp140_d540ur   : 1;  /* bit[4]: 过温140°异常FAULT_N事件记录寄存器(默认上升沿去抖540us)。
                                                                 0：没有发生过该事件；
                                                                 1：发生过该事件。 */
        unsigned char  np_vin_ldom_vld_deb      : 1;  /* bit[5]: 上电条件满足后，vin_ldom_vld异常事件记录寄存器（VDD_LDO_1P95是否拉低）。
                                                                 0：没有发生过该事件；
                                                                 1：发生过该事件。 */
        unsigned char  np_vin_io_vld_d5m        : 1;  /* bit[6]: 开机态vin_io_vld异常事件记录寄存器（VDD_IO是否拉低）。
                                                                 0：没有发生过该事件；
                                                                 1：发生过该事件。 */
        unsigned char  np_ldo_core_vld_d20nf    : 1;  /* bit[7]: LDO_CORE输出电源异常事件记录寄存器。
                                                                 0：没有发生过该事件；
                                                                 1：发生过该事件。 */
    } reg;
} PMIC_MMW_NP_RECORD1_UNION;
#endif
#define PMIC_MMW_NP_RECORD1_np_vsys_pwroff_abs_d20nr_START  (0)
#define PMIC_MMW_NP_RECORD1_np_vsys_pwroff_abs_d20nr_END    (0)
#define PMIC_MMW_NP_RECORD1_np_vsys_ov_d200ur_START         (1)
#define PMIC_MMW_NP_RECORD1_np_vsys_ov_d200ur_END           (1)
#define PMIC_MMW_NP_RECORD1_np_vsys_pwron_d60ur_START       (2)
#define PMIC_MMW_NP_RECORD1_np_vsys_pwron_d60ur_END         (2)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp125_d120ur_START    (3)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp125_d120ur_END      (3)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp140_d540ur_START    (4)
#define PMIC_MMW_NP_RECORD1_np_thsd_otmp140_d540ur_END      (4)
#define PMIC_MMW_NP_RECORD1_np_vin_ldom_vld_deb_START       (5)
#define PMIC_MMW_NP_RECORD1_np_vin_ldom_vld_deb_END         (5)
#define PMIC_MMW_NP_RECORD1_np_vin_io_vld_d5m_START         (6)
#define PMIC_MMW_NP_RECORD1_np_vin_io_vld_d5m_END           (6)
#define PMIC_MMW_NP_RECORD1_np_ldo_core_vld_d20nf_START     (7)
#define PMIC_MMW_NP_RECORD1_np_ldo_core_vld_d20nf_END       (7)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_RECORD2_UNION
 结构说明  : NP_RECORD2 寄存器结构定义。地址偏移量:0x116，初值:0x00，宽度:8
 寄存器说明: 非下电事件记录寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_ocpldo0 : 1;  /* bit[0]: LDO0输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo1 : 1;  /* bit[1]: LDO1输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo2 : 1;  /* bit[2]: LDO2输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo3 : 1;  /* bit[3]: LDO3输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo4 : 1;  /* bit[4]: LDO4输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo5 : 1;  /* bit[5]: LDO5输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo6 : 1;  /* bit[6]: LDO6输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
        unsigned char  np_ocpldo7 : 1;  /* bit[7]: LDO7输出过载事件记录寄存器。
                                                   0：没有发生过该事件；
                                                   1：发生过该事件。 */
    } reg;
} PMIC_MMW_NP_RECORD2_UNION;
#endif
#define PMIC_MMW_NP_RECORD2_np_ocpldo0_START  (0)
#define PMIC_MMW_NP_RECORD2_np_ocpldo0_END    (0)
#define PMIC_MMW_NP_RECORD2_np_ocpldo1_START  (1)
#define PMIC_MMW_NP_RECORD2_np_ocpldo1_END    (1)
#define PMIC_MMW_NP_RECORD2_np_ocpldo2_START  (2)
#define PMIC_MMW_NP_RECORD2_np_ocpldo2_END    (2)
#define PMIC_MMW_NP_RECORD2_np_ocpldo3_START  (3)
#define PMIC_MMW_NP_RECORD2_np_ocpldo3_END    (3)
#define PMIC_MMW_NP_RECORD2_np_ocpldo4_START  (4)
#define PMIC_MMW_NP_RECORD2_np_ocpldo4_END    (4)
#define PMIC_MMW_NP_RECORD2_np_ocpldo5_START  (5)
#define PMIC_MMW_NP_RECORD2_np_ocpldo5_END    (5)
#define PMIC_MMW_NP_RECORD2_np_ocpldo6_START  (6)
#define PMIC_MMW_NP_RECORD2_np_ocpldo6_END    (6)
#define PMIC_MMW_NP_RECORD2_np_ocpldo7_START  (7)
#define PMIC_MMW_NP_RECORD2_np_ocpldo7_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_RECORD3_UNION
 结构说明  : NP_RECORD3 寄存器结构定义。地址偏移量:0x117，初值:0x00，宽度:8
 寄存器说明: 非下电事件记录寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_ocpldo8            : 1;  /* bit[0]  : LDO8 OCP输出过载事件记录寄存器。
                                                                0：没有发生过该事件；
                                                                1：发生过该事件。 */
        unsigned char  np_ocpldo_core        : 1;  /* bit[1]  : LDO_CORE输出过载事件记录寄存器。
                                                                0：没有发生过该事件；
                                                                1：发生过该事件。 */
        unsigned char  np_pmuh_ocp           : 1;  /* bit[2]  : PMUH输出过载事件记录寄存器。
                                                                0：没有发生过该事件；
                                                                1：发生过该事件。 */
        unsigned char  np_ocpldo_buf0        : 1;  /* bit[3]  : LDO_BUF0输出过载事件记录寄存器。
                                                                0：没有发生过该事件；
                                                                1：发生过该事件。 */
        unsigned char  np_ocpldo_buf_rf      : 1;  /* bit[4]  : LDO_BUF_RF输出过载事件记录寄存器。
                                                                0：没有发生过该事件；
                                                                1：发生过该事件。 */
        unsigned char  np_pmuh_short_n_d20nf : 1;  /* bit[5]  : PMUH输出短路事件记录寄存器。
                                                                0：没有发生过该事件；
                                                                1：发生过该事件。 */
        unsigned char  reserved              : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_NP_RECORD3_UNION;
#endif
#define PMIC_MMW_NP_RECORD3_np_ocpldo8_START             (0)
#define PMIC_MMW_NP_RECORD3_np_ocpldo8_END               (0)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_core_START         (1)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_core_END           (1)
#define PMIC_MMW_NP_RECORD3_np_pmuh_ocp_START            (2)
#define PMIC_MMW_NP_RECORD3_np_pmuh_ocp_END              (2)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf0_START         (3)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf0_END           (3)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf_rf_START       (4)
#define PMIC_MMW_NP_RECORD3_np_ocpldo_buf_rf_END         (4)
#define PMIC_MMW_NP_RECORD3_np_pmuh_short_n_d20nf_START  (5)
#define PMIC_MMW_NP_RECORD3_np_pmuh_short_n_d20nf_END    (5)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_RECORD4_UNION
 结构说明  : NP_RECORD4 寄存器结构定义。地址偏移量:0x118，初值:0x00，宽度:8
 寄存器说明: 非下电事件记录寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_19m2_dis       : 1;  /* bit[0]  : 19.2MHz时钟先有然后丢失事件记录寄存器。
                                                               0：没有发生过该事件；
                                                               1：发生过该事件。
                                                               备注：软件需注意，主PMU的BUCK3是Hi6563 LDO_CORE的供电电源，配套Hi6421V700应用，在Hi6421V700下电或校准结束，BUCK3关闭会导致LDO_CORE跟随下电（Hi6563模拟行为），导致模拟输出时钟异常，数字会检测出时钟丢失，因此BUCK3关闭Hi6563每次都会误记录时钟丢失事件。软件约束：Hi6563上电后，应先清除此事件记录，真正的时钟丢失可配合fault_n的事件记录一起查看。 */
        unsigned char  np_19m2_dis_time_out : 1;  /* bit[1]  : 19.2MHz时钟一直都没有，计时超时事件。
                                                               0：没有发生过该事件；
                                                               1：发生过该事件。 
                                                               注：下电过程不记录（正常下电和异常下电） */
        unsigned char  buf_en_d5mf_shutdown : 1;  /* bit[2]  : 开机态CLK_BUF_EN拉低5ms后触发关机事件记录寄存器。（只有不屏蔽该下电功能才会有此事件记录）
                                                               0：没有发生过该事件；
                                                               1：发生过该事件。 */
        unsigned char  np_buf_en_d5mf       : 1;  /* bit[3]  : 开机态CLK_BUF_EN拉低5ms异常事件记录寄存器。（有对应的fault_n上报）
                                                               0：没有发生过该事件；
                                                               1：发生过该事件。 */
        unsigned char  reserved             : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_NP_RECORD4_UNION;
#endif
#define PMIC_MMW_NP_RECORD4_np_xo_19m2_dis_START        (0)
#define PMIC_MMW_NP_RECORD4_np_xo_19m2_dis_END          (0)
#define PMIC_MMW_NP_RECORD4_np_19m2_dis_time_out_START  (1)
#define PMIC_MMW_NP_RECORD4_np_19m2_dis_time_out_END    (1)
#define PMIC_MMW_NP_RECORD4_buf_en_d5mf_shutdown_START  (2)
#define PMIC_MMW_NP_RECORD4_buf_en_d5mf_shutdown_END    (2)
#define PMIC_MMW_NP_RECORD4_np_buf_en_d5mf_START        (3)
#define PMIC_MMW_NP_RECORD4_np_buf_en_d5mf_END          (3)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_RECORD5_UNION
 结构说明  : NP_RECORD5 寄存器结构定义。地址偏移量:0x119，初值:0x00，宽度:8
 寄存器说明: 非下电事件记录寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_dcxo_clk_sel_r     : 1;  /* bit[0]: 32K RC 切换到 19.2MHz/586 时钟。
                                                              0：没有发生过该事件；
                                                              1：发生过该事件。  */
        unsigned char  np_dcxo_clk_sel_f     : 1;  /* bit[1]: 19.2MHz/586 切换到 32K RC时钟。
                                                              0：没有发生过该事件；
                                                              1：发生过该事件。  */
        unsigned char  np_pmic63_en_shutdown : 1;  /* bit[2]: pmic63_EN拉低触发关机。
                                                              0：没有发生过该事件；
                                                              1：发生过该事件。 */
        unsigned char  np_hrstin             : 1;  /* bit[3]: RST_IN热复位事件。
                                                              0：没有RST_IN热复位事件；
                                                              1：发生RST_IN热复位事件。 */
        unsigned char  np_vin_ldom_time_out  : 1;  /* bit[4]: vin_ldom_vld在30ms一直没拉高，开机条件不满足触发关机事件。
                                                              0：没有发生过该事件；
                                                              1：发生过该事件。 */
        unsigned char  np_rstin_time_out     : 1;  /* bit[5]: pmic63_EN拉高触发开机，RST_IN一直未拉高，计时超时事件。
                                                              0：没有发生过该事件；
                                                              1：发生过该事件。  */
        unsigned char  np_fault_n            : 1;  /* bit[6]: FAULT_N管脚上报事件（是否上报给了主PMU）。
                                                              0：没有发生过该事件；
                                                              1：发生过该事件。  */
        unsigned char  reserved              : 1;  /* bit[7]: 保留。 */
    } reg;
} PMIC_MMW_NP_RECORD5_UNION;
#endif
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_r_START      (0)
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_r_END        (0)
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_f_START      (1)
#define PMIC_MMW_NP_RECORD5_np_dcxo_clk_sel_f_END        (1)
#define PMIC_MMW_NP_RECORD5_np_pmic63_en_shutdown_START  (2)
#define PMIC_MMW_NP_RECORD5_np_pmic63_en_shutdown_END    (2)
#define PMIC_MMW_NP_RECORD5_np_hrstin_START              (3)
#define PMIC_MMW_NP_RECORD5_np_hrstin_END                (3)
#define PMIC_MMW_NP_RECORD5_np_vin_ldom_time_out_START   (4)
#define PMIC_MMW_NP_RECORD5_np_vin_ldom_time_out_END     (4)
#define PMIC_MMW_NP_RECORD5_np_rstin_time_out_START      (5)
#define PMIC_MMW_NP_RECORD5_np_rstin_time_out_END        (5)
#define PMIC_MMW_NP_RECORD5_np_fault_n_START             (6)
#define PMIC_MMW_NP_RECORD5_np_fault_n_END               (6)




/****************************************************************************
                     (5/5) NP_PMU_CTRL
 ****************************************************************************/
/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF0_WAVE_CTRL_UNION
 结构说明  : CLK_RF0_WAVE_CTRL 寄存器结构定义。地址偏移量:0x200，初值:0x01，宽度:8
 寄存器说明: CLK_RF0时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_rf0_sel : 1;  /* bit[0]  : RF0_CLK_76M8时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF0_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_np_xo_dig_rf0_sel_START  (0)
#define PMIC_MMW_CLK_RF0_WAVE_CTRL_np_xo_dig_rf0_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF1_WAVE_CTRL_UNION
 结构说明  : CLK_RF1_WAVE_CTRL 寄存器结构定义。地址偏移量:0x201，初值:0x01，宽度:8
 寄存器说明: CLK_RF1时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_rf1_sel : 1;  /* bit[0]  : RF1_CLK_76M8时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF1_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_np_xo_dig_rf1_sel_START  (0)
#define PMIC_MMW_CLK_RF1_WAVE_CTRL_np_xo_dig_rf1_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF2_WAVE_CTRL_UNION
 结构说明  : CLK_RF2_WAVE_CTRL 寄存器结构定义。地址偏移量:0x202，初值:0x01，宽度:8
 寄存器说明: CLK_RF2时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_rf2_sel : 1;  /* bit[0]  : RF2_CLK_76M8时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF2_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_np_xo_dig_rf2_sel_START  (0)
#define PMIC_MMW_CLK_RF2_WAVE_CTRL_np_xo_dig_rf2_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF3_WAVE_CTRL_UNION
 结构说明  : CLK_RF3_WAVE_CTRL 寄存器结构定义。地址偏移量:0x203，初值:0x01，宽度:8
 寄存器说明: CLK_RF3时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_rf3_sel : 1;  /* bit[0]  : RF3_CLK_76M8时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF3_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_np_xo_dig_rf3_sel_START  (0)
#define PMIC_MMW_CLK_RF3_WAVE_CTRL_np_xo_dig_rf3_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF4_WAVE_CTRL_UNION
 结构说明  : CLK_RF4_WAVE_CTRL 寄存器结构定义。地址偏移量:0x204，初值:0x01，宽度:8
 寄存器说明: CLK_RF4时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_rf4_sel : 1;  /* bit[0]  : RF4_CLK_76M8时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF4_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_np_xo_dig_rf4_sel_START  (0)
#define PMIC_MMW_CLK_RF4_WAVE_CTRL_np_xo_dig_rf4_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SERDES_WAVE_CTRL_UNION
 结构说明  : CLK_SERDES_WAVE_CTRL 寄存器结构定义。地址偏移量:0x205，初值:0x01，宽度:8
 寄存器说明: CLK_SERDES时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_serdes_sel : 1;  /* bit[0]  : SERDES_CLK_38M4时钟输出波形选择：
                                                               0：输出方波；
                                                               1：输出正弦波。
                                                               注：默认值通过OTP选择 */
        unsigned char  reserved             : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SERDES_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_np_xo_dig_serdes_sel_START  (0)
#define PMIC_MMW_CLK_SERDES_WAVE_CTRL_np_xo_dig_serdes_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI0_WAVE_CTRL_UNION
 结构说明  : CLK_WIFI0_WAVE_CTRL 寄存器结构定义。地址偏移量:0x206，初值:0x01，宽度:8
 寄存器说明: CLK_WIFI0时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_wifi0_sel : 1;  /* bit[0]  : WIFI0_CLK_38M4时钟输出波形选择：
                                                              0：输出方波；
                                                              1：输出正弦波。
                                                              注：默认值通过OTP选择 */
        unsigned char  reserved            : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI0_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_np_xo_dig_wifi0_sel_START  (0)
#define PMIC_MMW_CLK_WIFI0_WAVE_CTRL_np_xo_dig_wifi0_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI1_WAVE_CTRL_UNION
 结构说明  : CLK_WIFI1_WAVE_CTRL 寄存器结构定义。地址偏移量:0x207，初值:0x01，宽度:8
 寄存器说明: CLK_WIFI1时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_wifi1_sel : 1;  /* bit[0]  : WIFI1_CLK_38M4时钟输出波形选择：
                                                              0：输出方波；
                                                              1：输出正弦波。
                                                              注：默认值通过OTP选择 */
        unsigned char  reserved            : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI1_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_np_xo_dig_wifi1_sel_START  (0)
#define PMIC_MMW_CLK_WIFI1_WAVE_CTRL_np_xo_dig_wifi1_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_NFC_WAVE_CTRL_UNION
 结构说明  : CLK_NFC_WAVE_CTRL 寄存器结构定义。地址偏移量:0x208，初值:0x01，宽度:8
 寄存器说明: CLK_NFC时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_nfc_sel : 1;  /* bit[0]  : NFC_CLK_38M4时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。
                                                            注：默认值通过OTP选择 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_NFC_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_np_xo_dig_nfc_sel_START  (0)
#define PMIC_MMW_CLK_NFC_WAVE_CTRL_np_xo_dig_nfc_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SYS_WAVE_CTRL_UNION
 结构说明  : CLK_SYS_WAVE_CTRL 寄存器结构定义。地址偏移量:0x209，初值:0x00，宽度:8
 寄存器说明: CLK_SYS时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_sys_sel : 1;  /* bit[0]  : SYS_CLK_38M4时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。
                                                            注：默认值通过OTP选择 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SYS_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_np_xo_dig_sys_sel_START  (0)
#define PMIC_MMW_CLK_SYS_WAVE_CTRL_np_xo_dig_sys_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_PMU_WAVE_CTRL_UNION
 结构说明  : CLK_PMU_WAVE_CTRL 寄存器结构定义。地址偏移量:0x20A，初值:0x01，宽度:8
 寄存器说明: CLK_PMU时钟输出波形控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_dig_pmu_sel : 1;  /* bit[0]  : PMU_CLK_OUT时钟输出波形选择：
                                                            0：输出方波；
                                                            1：输出正弦波。
                                                            注：默认值通过OTP选择 */
        unsigned char  reserved          : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_PMU_WAVE_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_np_xo_dig_pmu_sel_START  (0)
#define PMIC_MMW_CLK_PMU_WAVE_CTRL_np_xo_dig_pmu_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF0_FREQ_CTRL_UNION
 结构说明  : CLK_RF0_FREQ_CTRL 寄存器结构定义。地址偏移量:0x20B，初值:0x01，宽度:8
 寄存器说明: CLK_RF0时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_rf0_sel : 1;  /* bit[0]  : RF0_CLK_76M8时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_rf0_sel 时钟频率
                                                              0 0 38.4M
                                                              0 1 76.8M
                                                              1 0 19.2M
                                                              1 1 38.4M */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF0_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_np_xo_freq_rf0_sel_START  (0)
#define PMIC_MMW_CLK_RF0_FREQ_CTRL_np_xo_freq_rf0_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF1_FREQ_CTRL_UNION
 结构说明  : CLK_RF1_FREQ_CTRL 寄存器结构定义。地址偏移量:0x20C，初值:0x01，宽度:8
 寄存器说明: CLK_RF1时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_rf1_sel : 1;  /* bit[0]  : RF1_CLK_76M8时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_rf1_sel 时钟频率
                                                              0 0 38.4M
                                                              0 1 76.8M
                                                              1 0 19.2M
                                                              1 1 38.4M */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF1_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_np_xo_freq_rf1_sel_START  (0)
#define PMIC_MMW_CLK_RF1_FREQ_CTRL_np_xo_freq_rf1_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF2_FREQ_CTRL_UNION
 结构说明  : CLK_RF2_FREQ_CTRL 寄存器结构定义。地址偏移量:0x20D，初值:0x01，宽度:8
 寄存器说明: CLK_RF2时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_rf2_sel : 1;  /* bit[0]  : RF2_CLK_76M8时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_rf2_sel 时钟频率
                                                              0 0 38.4M
                                                              0 1 76.8M
                                                              1 0 19.2M
                                                              1 1 38.4M */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF2_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_np_xo_freq_rf2_sel_START  (0)
#define PMIC_MMW_CLK_RF2_FREQ_CTRL_np_xo_freq_rf2_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF3_FREQ_CTRL_UNION
 结构说明  : CLK_RF3_FREQ_CTRL 寄存器结构定义。地址偏移量:0x20E，初值:0x01，宽度:8
 寄存器说明: CLK_RF3时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_rf3_sel : 1;  /* bit[0]  : RF3_CLK_76M8时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_rf3_sel 时钟频率
                                                              0 0 38.4M
                                                              0 1 76.8M
                                                              1 0 19.2M
                                                              1 1 38.4M */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF3_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_np_xo_freq_rf3_sel_START  (0)
#define PMIC_MMW_CLK_RF3_FREQ_CTRL_np_xo_freq_rf3_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_RF4_FREQ_CTRL_UNION
 结构说明  : CLK_RF4_FREQ_CTRL 寄存器结构定义。地址偏移量:0x20F，初值:0x01，宽度:8
 寄存器说明: CLK_RF4时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_rf4_sel : 1;  /* bit[0]  : RF4_CLK_76M8时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_rf4_sel 时钟频率
                                                              0 0 38.4M
                                                              0 1 76.8M
                                                              1 0 19.2M
                                                              1 1 38.4M */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_RF4_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_np_xo_freq_rf4_sel_START  (0)
#define PMIC_MMW_CLK_RF4_FREQ_CTRL_np_xo_freq_rf4_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SERDES_FREQ_CTRL_UNION
 结构说明  : CLK_SERDES_FREQ_CTRL 寄存器结构定义。地址偏移量:0x210，初值:0x00，宽度:8
 寄存器说明: CLK_SERDES时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_serdes_sel : 1;  /* bit[0]  : SERDES_CLK_38M4时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                                FREQ_SEL_IN np_xo_freq_serdes_sel 时钟频率
                                                                 0 0 38.4M
                                                                 0 1 76.8M
                                                                 1 0 38.4M
                                                                 1 1 19.2M
                                                                注：默认值通过OTP选择 */
        unsigned char  reserved              : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SERDES_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_np_xo_freq_serdes_sel_START  (0)
#define PMIC_MMW_CLK_SERDES_FREQ_CTRL_np_xo_freq_serdes_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI0_FREQ_CTRL_UNION
 结构说明  : CLK_WIFI0_FREQ_CTRL 寄存器结构定义。地址偏移量:0x211，初值:0x00，宽度:8
 寄存器说明: CLK_WIFI0时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_wifi0_sel : 1;  /* bit[0]  : WIFI0_CLK_38M4时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                               FREQ_SEL_IN np_xo_freq_wifi0_sel 时钟频率
                                                                0 0 38.4M
                                                                0 1 76.8M
                                                                1 0 38.4M
                                                                1 1 19.2M
                                                               注：默认值通过OTP选择 */
        unsigned char  reserved             : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI0_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_np_xo_freq_wifi0_sel_START  (0)
#define PMIC_MMW_CLK_WIFI0_FREQ_CTRL_np_xo_freq_wifi0_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_WIFI1_FREQ_CTRL_UNION
 结构说明  : CLK_WIFI1_FREQ_CTRL 寄存器结构定义。地址偏移量:0x212，初值:0x01，宽度:8
 寄存器说明: CLK_WIFI1时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_wifi1_sel : 1;  /* bit[0]  : WIFI1_CLK_38M4时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                               FREQ_SEL_IN np_xo_freq_wifi1_sel 时钟频率
                                                                0 0 19.2M
                                                                0 1 38.4M
                                                                1 0 19.2M
                                                                1 1 38.4M
                                                               注：默认值通过OTP选择 */
        unsigned char  reserved             : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_WIFI1_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_np_xo_freq_wifi1_sel_START  (0)
#define PMIC_MMW_CLK_WIFI1_FREQ_CTRL_np_xo_freq_wifi1_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_NFC_FREQ_CTRL_UNION
 结构说明  : CLK_NFC_FREQ_CTRL 寄存器结构定义。地址偏移量:0x213，初值:0x01，宽度:8
 寄存器说明: CLK_NFC时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_nfc_sel : 1;  /* bit[0]  : NFC_CLK_38M4时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_nfc_sel 时钟频率
                                                              0 0 19.2M
                                                              0 1 38.4M
                                                              1 0 19.2M
                                                              1 1 38.4M
                                                             注：默认值通过OTP选择 */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_NFC_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_np_xo_freq_nfc_sel_START  (0)
#define PMIC_MMW_CLK_NFC_FREQ_CTRL_np_xo_freq_nfc_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SYS_FREQ_CTRL_UNION
 结构说明  : CLK_SYS_FREQ_CTRL 寄存器结构定义。地址偏移量:0x214，初值:0x01，宽度:8
 寄存器说明: CLK_SYS时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_sys_sel : 1;  /* bit[0]  : SYS_CLK_38M4时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_sys_sel 时钟频率
                                                              0 0 19.2M
                                                              0 1 38.4M
                                                              1 0 19.2M
                                                              1 1 38.4M
                                                             注：默认值通过OTP选择 */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SYS_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_np_xo_freq_sys_sel_START  (0)
#define PMIC_MMW_CLK_SYS_FREQ_CTRL_np_xo_freq_sys_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_PMU_FREQ_CTRL_UNION
 结构说明  : CLK_PMU_FREQ_CTRL 寄存器结构定义。地址偏移量:0x215，初值:0x00，宽度:8
 寄存器说明: CLK_PMU时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_pmu_sel : 1;  /* bit[0]  : PMU_CLK_OUT时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_pmu_sel 时钟频率
                                                              0 0 19.2M
                                                              0 1 38.4M
                                                              1 0 19.2M
                                                              1 1 38.4M
                                                             注：默认值通过OTP选择 */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_PMU_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_np_xo_freq_pmu_sel_START  (0)
#define PMIC_MMW_CLK_PMU_FREQ_CTRL_np_xo_freq_pmu_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_UFS_FREQ_CTRL_UNION
 结构说明  : CLK_UFS_FREQ_CTRL 寄存器结构定义。地址偏移量:0x216，初值:0x00，宽度:8
 寄存器说明: CLK_UFS时钟输出频率控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_freq_ufs_sel : 1;  /* bit[0]  : UFS_CLK_19M2时钟输出频率选择，与FREQ_SEL_IN硬线共同控制时钟输出频率：
                                                             FREQ_SEL_IN np_xo_freq_ufs_sel 时钟频率
                                                              0 0 19.2M
                                                              0 1 38.4M
                                                              1 0 19.2M
                                                              1 1 38.4M
                                                             注：默认值通过OTP选择 */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_UFS_FREQ_CTRL_UNION;
#endif
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_np_xo_freq_ufs_sel_START  (0)
#define PMIC_MMW_CLK_UFS_FREQ_CTRL_np_xo_freq_ufs_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_SYS_BUF_PWR_SEL_UNION
 结构说明  : CLK_SYS_BUF_PWR_SEL 寄存器结构定义。地址偏移量:0x217，初值:0x00，宽度:8
 寄存器说明: SYS_CLK BUF所用电源选择寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_sys_buf_sel : 1;  /* bit[0]  : sys_clk buf所用电源选择。
                                                         0：表示选择LDO_BUF0；
                                                         1：表示选择LDO_BUF1。
                                                         注：默认值可以通过OTP选择。 */
        unsigned char  reserved       : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_SYS_BUF_PWR_SEL_UNION;
#endif
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_np_sys_buf_sel_START  (0)
#define PMIC_MMW_CLK_SYS_BUF_PWR_SEL_np_sys_buf_sel_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_EN_PMUD_UNION
 结构说明  : EN_PMUD 寄存器结构定义。地址偏移量:0x218，初值:0x01，宽度:8
 寄存器说明: EN_PMUD配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_pmud_buck_en_cfg : 1;  /* bit[0]  : pmud_buck_en信号时序功能使能信号：
                                                              1'b0:不使能pmud_buck_en时序功能；
                                                              1'b1：使能pmud_buck_en时序功能 */
        unsigned char  reserved            : 7;  /* bit[1-7]: 保留 */
    } reg;
} PMIC_MMW_EN_PMUD_UNION;
#endif
#define PMIC_MMW_EN_PMUD_np_pmud_buck_en_cfg_START  (0)
#define PMIC_MMW_EN_PMUD_np_pmud_buck_en_cfg_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_PMUD_VSET_UNION
 结构说明  : PMUD_VSET 寄存器结构定义。地址偏移量:0x219，初值:0x00，宽度:8
 寄存器说明: PMUD调压寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_pmud_vset : 2;  /* bit[0-1]: pmud电压档位选择：
                                                       00：1.8V；
                                                       01：1.7V；
                                                       10：1.85V；
                                                       11：1.9V。 */
        unsigned char  reserved     : 6;  /* bit[2-7]: 保留。 */
    } reg;
} PMIC_MMW_PMUD_VSET_UNION;
#endif
#define PMIC_MMW_PMUD_VSET_np_pmud_vset_START  (0)
#define PMIC_MMW_PMUD_VSET_np_pmud_vset_END    (1)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_TOP_CTRL1_0_UNION
 结构说明  : CLK_TOP_CTRL1_0 寄存器结构定义。地址偏移量:0x21A，初值:0x05，宽度:8
 寄存器说明: 时钟控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_trim_c2fix : 4;  /* bit[0-3]: 时钟频率精度trim寄存器2（和CLK_TOP_CTRL1[np_xo_c1fix]一起使用）。 */
        unsigned char  reserved         : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_CLK_TOP_CTRL1_0_UNION;
#endif
#define PMIC_MMW_CLK_TOP_CTRL1_0_np_xo_trim_c2fix_START  (0)
#define PMIC_MMW_CLK_TOP_CTRL1_0_np_xo_trim_c2fix_END    (3)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_TOP_CTRL1_1_UNION
 结构说明  : CLK_TOP_CTRL1_1 寄存器结构定义。地址偏移量:0x21B，初值:0x50，宽度:8
 寄存器说明: 时钟控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_trim_c1fix : 8;  /* bit[0-7]: 时钟频率精度trim寄存器1（和CLK_TOP_CTRL0[np_xo_c2fix]一起使用）。 */
    } reg;
} PMIC_MMW_CLK_TOP_CTRL1_1_UNION;
#endif
#define PMIC_MMW_CLK_TOP_CTRL1_1_np_xo_trim_c1fix_START  (0)
#define PMIC_MMW_CLK_TOP_CTRL1_1_np_xo_trim_c1fix_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_256K_CTRL0_UNION
 结构说明  : CLK_256K_CTRL0 寄存器结构定义。地址偏移量:0x21C，初值:0x00，宽度:8
 寄存器说明: 256K RC控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_reg_256k_en0 : 8;  /* bit[0-7]: RC256K关闭控制。
                                                          0x5A：关闭256K时钟（同时reg_256k_en1为0xF0）；
                                                          其他：开启。 */
    } reg;
} PMIC_MMW_CLK_256K_CTRL0_UNION;
#endif
#define PMIC_MMW_CLK_256K_CTRL0_np_reg_256k_en0_START  (0)
#define PMIC_MMW_CLK_256K_CTRL0_np_reg_256k_en0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_CLK_256K_CTRL1_UNION
 结构说明  : CLK_256K_CTRL1 寄存器结构定义。地址偏移量:0x21D，初值:0x00，宽度:8
 寄存器说明: 256K RC控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_reg_256k_en1 : 8;  /* bit[0-7]: RC256K关闭控制。
                                                          0xF0：关闭256K时钟（同时reg_256k_en0为0x5A）；
                                                          其他：开启。 */
    } reg;
} PMIC_MMW_CLK_256K_CTRL1_UNION;
#endif
#define PMIC_MMW_CLK_256K_CTRL1_np_reg_256k_en1_START  (0)
#define PMIC_MMW_CLK_256K_CTRL1_np_reg_256k_en1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_XO_DET_DATA_UNION
 结构说明  : XO_DET_DATA 寄存器结构定义。地址偏移量:0x21E，初值:0x00，宽度:8
 寄存器说明: XO corner检测数据寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_xo_corner_data : 8;  /* bit[0-7]: XO corner检测输出data。 */
    } reg;
} PMIC_MMW_XO_DET_DATA_UNION;
#endif
#define PMIC_MMW_XO_DET_DATA_np_xo_corner_data_START  (0)
#define PMIC_MMW_XO_DET_DATA_np_xo_corner_data_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_VSYS_LOW_SET_UNION
 结构说明  : VSYS_LOW_SET 寄存器结构定义。地址偏移量:0x21F，初值:0x02，宽度:8
 寄存器说明: IN端低压阈值调整寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_vsys_pwroff_abs_set : 2;  /* bit[0-1]: vsys端低电压判断阈值电压调整，低于此电压立刻关机，不滤波。
                                                                 00：2.1V； 
                                                                 01：2.2V；
                                                                 10：2.3V； 
                                                                 11：2.3V； */
        unsigned char  np_vsys_pwron_set      : 2;  /* bit[2-3]: 开机电压阈值调节，高于此电压PMU开机。
                                                                 (默认2.9V)： 
                                                                 00: 2.9V 
                                                                 01: 3.0V
                                                                 10：3.1V
                                                                 11：3.2V  */
        unsigned char  reserved               : 4;  /* bit[4-7]: 保留。 */
    } reg;
} PMIC_MMW_VSYS_LOW_SET_UNION;
#endif
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwroff_abs_set_START  (0)
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwroff_abs_set_END    (1)
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwron_set_START       (2)
#define PMIC_MMW_VSYS_LOW_SET_np_vsys_pwron_set_END         (3)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_D2A_RES0_UNION
 结构说明  : NP_D2A_RES0 寄存器结构定义。地址偏移量:0x220，初值:0x00，宽度:8
 寄存器说明: 数模接口保留寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_d2a_reserve0 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_NP_D2A_RES0_UNION;
#endif
#define PMIC_MMW_NP_D2A_RES0_np_d2a_reserve0_START  (0)
#define PMIC_MMW_NP_D2A_RES0_np_d2a_reserve0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_D2A_RES1_UNION
 结构说明  : NP_D2A_RES1 寄存器结构定义。地址偏移量:0x221，初值:0x00，宽度:8
 寄存器说明: 数模接口保留寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_d2a_reserve1 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_NP_D2A_RES1_UNION;
#endif
#define PMIC_MMW_NP_D2A_RES1_np_d2a_reserve1_START  (0)
#define PMIC_MMW_NP_D2A_RES1_np_d2a_reserve1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_D2A_RES2_UNION
 结构说明  : NP_D2A_RES2 寄存器结构定义。地址偏移量:0x222，初值:0x00，宽度:8
 寄存器说明: 数模接口保留寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_d2a_reserve2 : 8;  /* bit[0-7]: 数字到模拟的保留信号。 */
    } reg;
} PMIC_MMW_NP_D2A_RES2_UNION;
#endif
#define PMIC_MMW_NP_D2A_RES2_np_d2a_reserve2_START  (0)
#define PMIC_MMW_NP_D2A_RES2_np_d2a_reserve2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_DEB_CTRL0_UNION
 结构说明  : NP_DEB_CTRL0 寄存器结构定义。地址偏移量:0x223，初值:0x55，宽度:8
 寄存器说明: 滤波时间控制非下电寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_pmic63_en_deb_sel   : 2;  /* bit[0-1]: pmic63_en的滤波时间。滤波时间不支持动态切换。
                                                                 00：0；
                                                                 01：90us；
                                                                 10：180us；
                                                                 11：540us。 
                                                                  注：寄存器需要将np_debug_lock配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_buf_en_deb_sel      : 2;  /* bit[2-3]: clk_buf_en的滤波时间。滤波时间不支持动态切换。
                                                                 00：0；
                                                                 01：90us；
                                                                 10：180us；
                                                                 11：540us。 
                                                                  注：寄存器需要将np_debug_lock配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_vin_ldom_posdeb_sel : 2;  /* bit[4-5]: vin_ldom_vld上升沿滤波时间。滤波时间不支持动态切换。
                                                                 00：0.2ms；
                                                                 01：0.5ms；
                                                                 10：1ms；
                                                                 11：2ms； 
                                                                  注：寄存器需要将np_debug_lock配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_vin_ldom_negdeb_sel : 2;  /* bit[6-7]: vin_ldom_vld下降沿沿滤波时间。滤波时间不支持动态切换。
                                                                 00：0.5ms；
                                                                 01：1ms；
                                                                 10：2ms；
                                                                 11：4ms； 
                                                                  注：寄存器需要将np_debug_lock配置为8'h6C，此寄存器才能配置生效。读不受限。 */
    } reg;
} PMIC_MMW_NP_DEB_CTRL0_UNION;
#endif
#define PMIC_MMW_NP_DEB_CTRL0_np_pmic63_en_deb_sel_START    (0)
#define PMIC_MMW_NP_DEB_CTRL0_np_pmic63_en_deb_sel_END      (1)
#define PMIC_MMW_NP_DEB_CTRL0_np_buf_en_deb_sel_START       (2)
#define PMIC_MMW_NP_DEB_CTRL0_np_buf_en_deb_sel_END         (3)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_posdeb_sel_START  (4)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_posdeb_sel_END    (5)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_negdeb_sel_START  (6)
#define PMIC_MMW_NP_DEB_CTRL0_np_vin_ldom_negdeb_sel_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_DEB_ONOFF0_UNION
 结构说明  : NP_DEB_ONOFF0 寄存器结构定义。地址偏移量:0x224，初值:0x01，宽度:8
 寄存器说明: 滤波开关非下电寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_en_vin_ldom_deb : 1;  /* bit[0]  : vin_ldom滤波功能使能位。
                                                             0：vin_ldohm 滤波不使能；
                                                             1：vin_ldohm 滤波使能。
                                                             注：寄存器需要将np_debug_lock配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  reserved           : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_NP_DEB_ONOFF0_UNION;
#endif
#define PMIC_MMW_NP_DEB_ONOFF0_np_en_vin_ldom_deb_START  (0)
#define PMIC_MMW_NP_DEB_ONOFF0_np_en_vin_ldom_deb_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_NP_PWRON_TIME_CFG_UNION
 结构说明  : NP_PWRON_TIME_CFG 寄存器结构定义。地址偏移量:0x225，初值:0x00，宽度:8
 寄存器说明: PMU_CLK_OUT上电时序和上电时序中等待时间T配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob0_d_w : 8;  /* bit[0-7]: OTP预修调寄存器0。
                                                           bit[7:5]：保留使用，可读写；
                                                           bit[4]：PMU_CLK_OUT上电时序选择（xo_pmu_en）：
                                                           0：在ldo_core_en后360us拉高；
                                                           1：在ldo_core_en后3ms拉高；
                                                           bit[3:0]：LDO_CORE上电后上电时序中的等待时间T（5~16ms）的预修调寄存器：
                                                           4'b0000:6ms； 4'b0001:5ms； 4'b0010:7ms； 4'b0011:8ms；
                                                           4'b0100:9ms； 4'b0101:10ms；4'b0110:11ms；4'b0111:12ms；
                                                           4'b1000:13ms；4'b1001:14ms；4'b1010:15ms；4'b1011:16ms；
                                                           4'b1100~4'b1111:6ms； */
    } reg;
} PMIC_MMW_NP_PWRON_TIME_CFG_UNION;
#endif
#define PMIC_MMW_NP_PWRON_TIME_CFG_np_otp_pdob0_d_w_START  (0)
#define PMIC_MMW_NP_PWRON_TIME_CFG_np_otp_pdob0_d_w_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_DEFAULT_FREQ_CFG_UNION
 结构说明  : DEFAULT_FREQ_CFG 寄存器结构定义。地址偏移量:0x226，初值:0x1C，宽度:8
 寄存器说明: XO时钟默认输出频率预修调寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_default_freq_serdes_sel : 1;  /* bit[0]: SERDES_CLK_38M4默认频率选择：
                                                                   0：默认为38.4MHz
                                                                   1：默认为76.8MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_freq_wifi0_sel  : 1;  /* bit[1]: WIFI0_CLK_38M4默认频率选择：
                                                                   0：默认为38.4MHz
                                                                   1：默认为76.8MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_freq_wifi1_sel  : 1;  /* bit[2]: WIFI1_CLK_38M4默认频率选择：
                                                                   0：默认为19.2MHz
                                                                   1：默认为38.4MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_freq_nfc_sel    : 1;  /* bit[3]: NFC_CLK_38M4默认频率选择：
                                                                   0：默认为19.2MHz
                                                                   1：默认为38.4MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_freq_sys_sel    : 1;  /* bit[4]: SYS_CLK_38M4默认频率选择：
                                                                   0：默认为19.2MHz
                                                                   1：默认为38.4MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_freq_pmu_sel    : 1;  /* bit[5]: PMU_CLK_OUT默认频率选择：
                                                                   0：默认为19.2MHz
                                                                   1：默认为38.4MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_freq_ufs_sel    : 1;  /* bit[6]: UFS_CLK_19M2默认频率选择：
                                                                   0：默认为19.2MHz
                                                                   1：默认为38.4MHz
                                                                   注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  reserved                   : 1;  /* bit[7]: 保留。 */
    } reg;
} PMIC_MMW_DEFAULT_FREQ_CFG_UNION;
#endif
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_serdes_sel_START  (0)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_serdes_sel_END    (0)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi0_sel_START   (1)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi0_sel_END     (1)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi1_sel_START   (2)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_wifi1_sel_END     (2)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_nfc_sel_START     (3)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_nfc_sel_END       (3)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_sys_sel_START     (4)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_sys_sel_END       (4)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_pmu_sel_START     (5)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_pmu_sel_END       (5)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_ufs_sel_START     (6)
#define PMIC_MMW_DEFAULT_FREQ_CFG_np_default_freq_ufs_sel_END       (6)


/*****************************************************************************
 结构名    : PMIC_MMW_DEFAULT_WAVE_CFG_UNION
 结构说明  : DEFAULT_WAVE_CFG 寄存器结构定义。地址偏移量:0x227，初值:0x2F，宽度:8
 寄存器说明: XO时钟默认输出波形预修调寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_default_dig_serdes_sel : 1;  /* bit[0]: SERDES_CLK_38M4默认波形选择：
                                                                  0：默认方波输出
                                                                  1：默认正弦波输出
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_dig_wifi0_sel  : 1;  /* bit[1]: WIFI0_CLK_38M4默认波形选择：
                                                                  0：默认方波输出
                                                                  1：默认正弦波输出
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_dig_wifi1_sel  : 1;  /* bit[2]: WIFI1_CLK_38M4默认波形选择：
                                                                  0：默认方波输出
                                                                  1：默认正弦波输出
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_dig_nfc_sel    : 1;  /* bit[3]: NFC_CLK_38M4默认波形选择：
                                                                  0：默认方波输出
                                                                  1：默认正弦波输出
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_dig_sys_sel    : 1;  /* bit[4]: SYS_CLK_38M4默认波形选择：
                                                                  0：默认方波输出
                                                                  1：默认正弦波输出
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_dig_pmu_sel    : 1;  /* bit[5]: PMU_CLK_OUT默认波形选择：
                                                                  0：默认方波输出
                                                                  1：默认正弦波输出
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_sys_buf_sel    : 1;  /* bit[6]: SYS_CLK_38M4默认供电电源选择：
                                                                  0：默认用LDO_BUF0供电；
                                                                  1：默认用LDO_BUF1供电。
                                                                  注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  reserved                  : 1;  /* bit[7]: 保留。 */
    } reg;
} PMIC_MMW_DEFAULT_WAVE_CFG_UNION;
#endif
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_serdes_sel_START  (0)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_serdes_sel_END    (0)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi0_sel_START   (1)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi0_sel_END     (1)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi1_sel_START   (2)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_wifi1_sel_END     (2)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_nfc_sel_START     (3)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_nfc_sel_END       (3)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_sys_sel_START     (4)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_sys_sel_END       (4)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_pmu_sel_START     (5)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_dig_pmu_sel_END       (5)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_sys_buf_sel_START     (6)
#define PMIC_MMW_DEFAULT_WAVE_CFG_np_default_sys_buf_sel_END       (6)


/*****************************************************************************
 结构名    : PMIC_MMW_DEFAULT_ONOFF_CFG_UNION
 结构说明  : DEFAULT_ONOFF_CFG 寄存器结构定义。地址偏移量:0x228，初值:0x31，宽度:8
 寄存器说明: XO时钟默认使能预修调寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_default_onoff_serdes_sel : 1;  /* bit[0]  : SERDES_CLK_38M4默认开关选择：
                                                                      0：默认不使能
                                                                      1：默认使能
                                                                      注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_onoff_wifi0_sel  : 1;  /* bit[1]  : WIFI0_CLK_38M4默认开关选择：
                                                                      0：默认不使能
                                                                      1：默认使能
                                                                      注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_onoff_wifi1_sel  : 1;  /* bit[2]  : WIFI1_CLK_38M4默认开关选择：
                                                                      0：默认不使能
                                                                      1：默认使能
                                                                      注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_onoff_nfc_sel    : 1;  /* bit[3]  : NFC_CLK_38M4默认开关选择：
                                                                      0：默认不使能
                                                                      1：默认使能
                                                                      注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_onoff_sys_sel    : 1;  /* bit[4]  : SYS_CLK_38M4默认开关选择：
                                                                      0：默认不使能
                                                                      1：默认使能
                                                                      注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  np_default_onoff_ufs_sel    : 1;  /* bit[5]  : UFS_CLK_19M2默认开关选择：
                                                                      0：默认不使能
                                                                      1：默认使能
                                                                      注：OTP烧写后预修调寄存器对默认值的控制失效。 */
        unsigned char  reserved                    : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_DEFAULT_ONOFF_CFG_UNION;
#endif
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_serdes_sel_START  (0)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_serdes_sel_END    (0)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi0_sel_START   (1)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi0_sel_END     (1)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi1_sel_START   (2)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_wifi1_sel_END     (2)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_nfc_sel_START     (3)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_nfc_sel_END       (3)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_sys_sel_START     (4)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_sys_sel_END       (4)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_ufs_sel_START     (5)
#define PMIC_MMW_DEFAULT_ONOFF_CFG_np_default_onoff_ufs_sel_END       (5)


/*****************************************************************************
 结构名    : PMIC_MMW_DIG_IO_DS_CFG0_UNION
 结构说明  : DIG_IO_DS_CFG0 寄存器结构定义。地址偏移量:0x229，初值:0x29，宽度:8
 寄存器说明: 数字IO驱动预修调寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_irq_n_ds     : 2;  /* bit[0-1]: IRQ_N管脚驱动能力预修调配置寄存器：
                                                          2'b00：4mA
                                                          2'b01：8mA
                                                          2'b10：12mA
                                                          2'b11：16mA
                                                          注：（对于SOC，此寄存器都可读可写） */
        unsigned char  np_clk32_sys_ds : 2;  /* bit[2-3]: CLK32_SYS管脚驱动能力预修调配置寄存器：
                                                          2'b00：4mA
                                                          2'b01：8mA
                                                          2'b10：12mA
                                                          2'b11：16mA
                                                          注：（对于SOC，此寄存器都可读可写） */
        unsigned char  np_spmi_data_ds : 2;  /* bit[4-5]: SPMI_DATA管脚驱动能力预修调配置寄存器：
                                                          2'b00：4mA
                                                          2'b01：8mA
                                                          2'b10：12mA
                                                          2'b11：16mA
                                                          注：（对于SOC，此寄存器都可读可写） */
        unsigned char  reserved        : 2;  /* bit[6-7]: 保留。 */
    } reg;
} PMIC_MMW_DIG_IO_DS_CFG0_UNION;
#endif
#define PMIC_MMW_DIG_IO_DS_CFG0_np_irq_n_ds_START      (0)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_irq_n_ds_END        (1)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_clk32_sys_ds_START  (2)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_clk32_sys_ds_END    (3)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_spmi_data_ds_START  (4)
#define PMIC_MMW_DIG_IO_DS_CFG0_np_spmi_data_ds_END    (5)


/*****************************************************************************
 结构名    : PMIC_MMW_DEBUG_LOCK_UNION
 结构说明  : DEBUG_LOCK 寄存器结构定义。地址偏移量:0x22A，初值:0x00，宽度:8
 寄存器说明: debug寄存器写屏蔽控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_debug_lock : 8;  /* bit[0-7]: 为防止软件误写NP_DEB_CTRL0、NP_DEB_ONOFF0、SYS_DEBUG0/1/2/3/4/5，导致系统出错。只有当debug_lock写为8'h6C的时候，上述寄存器才允许被一直写入； */
    } reg;
} PMIC_MMW_DEBUG_LOCK_UNION;
#endif
#define PMIC_MMW_DEBUG_LOCK_np_debug_lock_START  (0)
#define PMIC_MMW_DEBUG_LOCK_np_debug_lock_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_DEBUG0_UNION
 结构说明  : SYS_DEBUG0 寄存器结构定义。地址偏移量:0x22B，初值:0x00，宽度:8
 寄存器说明: 数字系统debug预留寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_sys_debug0 : 8;  /* bit[0-7]: 数字系统debug预留寄存器。
                                                        该寄存器写入0x7C，数字debug功能打开；写入其他值，数字debug功能关闭。
                                                        注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
    } reg;
} PMIC_MMW_SYS_DEBUG0_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG0_np_sys_debug0_START  (0)
#define PMIC_MMW_SYS_DEBUG0_np_sys_debug0_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_DEBUG1_UNION
 结构说明  : SYS_DEBUG1 寄存器结构定义。地址偏移量:0x22C，初值:0x00，宽度:8
 寄存器说明: 数字系统debug预留寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_sys_debug1 : 8;  /* bit[0-7]: 数字系统debug预留寄存器。
                                                        该寄存器写入0x00~0x2F的不同值可将数模接口的不同信号通过IRQ_N管脚引出观测。
                                                        0x00：ldo0_ocp； 0x01：ldo1_ocp； 0x02：ldo2_ocp；
                                                        0x03：ldo3_ocp； 0x04：ldo4_ocp； 0x05：ldo5_ocp；
                                                        0x06：ldo6_ocp； 0x07：ldo7_ocp； 0x08：ldo8_ocp；
                                                        0x09：ldo_core_ocp； 0x0A：ldo_buf0_ocp； 0x0B：ldo_rfbuf_ocp；
                                                        0x0C：pmuh_ocp； 0x0D：thsd_otmp125； 0x0E：thsd_otmp140；
                                                        0x0F：vsys_ov； 0x10：vsys_pwroff_abs；0x11：vsys_pwron；
                                                        0x12：vin_ldom_vld； 0x13：vin_ldo_0p9_vld；0x14：vin_ldo_1p1_vld；
                                                        0x15：vin_ldo_1p3_vld；0x16：vin_io_vld； 0x17：ldo_core_vld；
                                                        0x18：pmuh_short_n； 0x19：pmic63_en_a2d； 0x1A：clk_buf_en_a2d；
                                                        0x1B：xo_corner_det_valid；0x1C：ldo0_en； 0x1D：ldo1_en；
                                                        0x1E：ldo2_en； 0x1F：ldo3_en； 0x20：ldo4_en；
                                                        0x21：ldo5_en； 0x22：ldo6_en； 0x23：ldo7_en；
                                                        0x24：ldo8_en； 0x25：ldo_core_en； 0x26：ldo_rfbuf_en；
                                                        0x27：ldo_buf0_en； 0x28：ldo_buf1_en； 0x29：ldo_rfbuf0_en：
                                                        0x2A：ldo_rfbuf1_en； 0x2B：ldo_rfbuf2_en； 0x2C：ldo_rfbuf3_en；
                                                        0x2D：ldo_rfbuf4_en； 0x2E：pmuh_en； 0x2F：np_pmud_buck_en；
                                                        default：1'b0
                                                        配置此寄存器前需先配置np_sys_debug0寄存器打开数字debug功能。
                                                        注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
    } reg;
} PMIC_MMW_SYS_DEBUG1_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG1_np_sys_debug1_START  (0)
#define PMIC_MMW_SYS_DEBUG1_np_sys_debug1_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_DEBUG2_UNION
 结构说明  : SYS_DEBUG2 寄存器结构定义。地址偏移量:0x22D，初值:0x00，宽度:8
 寄存器说明: RC时钟debug预留寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_reg_rc_debug : 1;  /* bit[0]  : RC时钟debug使能信号。
                                                          0：正常使用；
                                                          1：将256K时钟引出。
                                                          注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  reserved        : 7;  /* bit[1-7]: 保留。 */
    } reg;
} PMIC_MMW_SYS_DEBUG2_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG2_np_reg_rc_debug_START  (0)
#define PMIC_MMW_SYS_DEBUG2_np_reg_rc_debug_END    (0)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_DEBUG3_UNION
 结构说明  : SYS_DEBUG3 寄存器结构定义。地址偏移量:0x22E，初值:0x60，宽度:8
 寄存器说明: 数字系统debug预留寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_vsys_pwroff_abs_pd_mask : 1;  /* bit[0]: vsys小于2.3v时是否自动关机控制位。
                                                                   0：自动关机(低于2.3v时间超过35us以上才能保证关机成功）；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_vsys_ov_pd_mask         : 1;  /* bit[1]: vsys过压时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_thsd_otmp140_pd_mask    : 1;  /* bit[2]: 温度超过140度时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_vin_ldom_vld_pd_mask    : 1;  /* bit[3]: VIN_LDO_1P95电源不合格时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_vin_io_vld_pd_mask      : 1;  /* bit[4]: 开机态VDD_IO电源不合格时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_xo_19m2_dis_pd_mask     : 1;  /* bit[5]: 19.2MHz时钟先有后丢失时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_clk_buf_en_pd_mask      : 1;  /* bit[6]: 开机态CLK_BUF_EN拉低时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
        unsigned char  np_19m2_dis_timeout_pd_mk  : 1;  /* bit[7]: 19.2MHz时钟一直都没有，计时超时是否自动关机控制位。
                                                                   0：自动关机；
                                                                   1：不自动关机。
                                                                   注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
    } reg;
} PMIC_MMW_SYS_DEBUG3_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG3_np_vsys_pwroff_abs_pd_mask_START  (0)
#define PMIC_MMW_SYS_DEBUG3_np_vsys_pwroff_abs_pd_mask_END    (0)
#define PMIC_MMW_SYS_DEBUG3_np_vsys_ov_pd_mask_START          (1)
#define PMIC_MMW_SYS_DEBUG3_np_vsys_ov_pd_mask_END            (1)
#define PMIC_MMW_SYS_DEBUG3_np_thsd_otmp140_pd_mask_START     (2)
#define PMIC_MMW_SYS_DEBUG3_np_thsd_otmp140_pd_mask_END       (2)
#define PMIC_MMW_SYS_DEBUG3_np_vin_ldom_vld_pd_mask_START     (3)
#define PMIC_MMW_SYS_DEBUG3_np_vin_ldom_vld_pd_mask_END       (3)
#define PMIC_MMW_SYS_DEBUG3_np_vin_io_vld_pd_mask_START       (4)
#define PMIC_MMW_SYS_DEBUG3_np_vin_io_vld_pd_mask_END         (4)
#define PMIC_MMW_SYS_DEBUG3_np_xo_19m2_dis_pd_mask_START      (5)
#define PMIC_MMW_SYS_DEBUG3_np_xo_19m2_dis_pd_mask_END        (5)
#define PMIC_MMW_SYS_DEBUG3_np_clk_buf_en_pd_mask_START       (6)
#define PMIC_MMW_SYS_DEBUG3_np_clk_buf_en_pd_mask_END         (6)
#define PMIC_MMW_SYS_DEBUG3_np_19m2_dis_timeout_pd_mk_START   (7)
#define PMIC_MMW_SYS_DEBUG3_np_19m2_dis_timeout_pd_mk_END     (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_DEBUG4_UNION
 结构说明  : SYS_DEBUG4 寄存器结构定义。地址偏移量:0x22F，初值:0x00，宽度:8
 寄存器说明: 数字系统debug预留寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_sys_debug2 : 8;  /* bit[0-7]: 数字系统debug预留寄存器。
                                                        该寄存器写入0x7C，数字debug功能打开；写入其他值，数字debug功能关闭。
                                                        注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
    } reg;
} PMIC_MMW_SYS_DEBUG4_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG4_np_sys_debug2_START  (0)
#define PMIC_MMW_SYS_DEBUG4_np_sys_debug2_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_SYS_DEBUG5_UNION
 结构说明  : SYS_DEBUG5 寄存器结构定义。地址偏移量:0x230，初值:0x00，宽度:8
 寄存器说明: 数字系统debug预留寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_sys_debug3 : 8;  /* bit[0-7]: 数字系统debug预留寄存器。
                                                        该寄存器写入0x00~0x28的不同值可将数模接口的不同信号通过CLK32_SYS管脚引出观测。
                                                        0x00：ldo0_en； 0x01：ldo1_en； 0x02：ldo2_en；
                                                        0x03：ldo3_en； 0x04：ldo4_en； 0x05：ldo5_en；
                                                        0x06：ldo6_en； 0x07：ldo7_en； 0x08：ldo8_en；
                                                        0x09：ldo_core_en； 0x0A：ldo_rfbuf_en； 0x0B：ldo_buf0_en；
                                                        0x0C：ldo_buf1_en； 0x0D：ldo_rfbuf0_en； 0x0E：ldo_rfbuf1_en；
                                                        0x0F：ldo_rfbuf2_en； 0x10：ldo_rfbuf3_en； 0x11：ldo_rfbuf4_en；
                                                        0x12：pmuh_en； 0x13：np_pmud_buck_en；0x14：ldo7_eco_en；
                                                        0x15：ldo8_eco_en； 0x16：ldo_core_eco_en；0x17：eco_in_n；
                                                        0x18：xo_core_en； 0x19：xo_ufs_en； 0x1A：xo_sys_en；
                                                        0x1B：xo_nfc_en； 0x1C：xo_wifi1_en； 0x1D：xo_wifi0_en；
                                                        0x1E：xo_serdes_en； 0x1F：xo_pmu_en； 0x20：thsd_en；
                                                        0x21：np_rc_clk_en_n；0x22：ref_res_sel； 0x23：ref_en；
                                                        0x24：xo_rf0_en； 0x25：xo_rf1_en； 0x26：xo_rf2_en；
                                                        0x27：xo_rf3_en； 0x28：xo_rf4_en； 
                                                        default：1'b0
                                                        配置此寄存器前需先配置np_sys_debug2寄存器打开数字debug功能。
                                                        注：寄存器需要将寄存器DEBUG_LOCK配置为8'h6C，此寄存器才能配置生效。读不受限。 */
    } reg;
} PMIC_MMW_SYS_DEBUG5_UNION;
#endif
#define PMIC_MMW_SYS_DEBUG5_np_sys_debug3_START  (0)
#define PMIC_MMW_SYS_DEBUG5_np_sys_debug3_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_0_R_UNION
 结构说明  : OTP_0_R 寄存器结构定义。地址偏移量:0x231，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob0_d2a : 8;  /* bit[0-7]: OTP回读寄存器(bit0为烧写识别位) */
    } reg;
} PMIC_MMW_OTP_0_R_UNION;
#endif
#define PMIC_MMW_OTP_0_R_np_otp_pdob0_d2a_START  (0)
#define PMIC_MMW_OTP_0_R_np_otp_pdob0_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_1_R_UNION
 结构说明  : OTP_1_R 寄存器结构定义。地址偏移量:0x232，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob1_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_1_R_UNION;
#endif
#define PMIC_MMW_OTP_1_R_np_otp_pdob1_d2a_START  (0)
#define PMIC_MMW_OTP_1_R_np_otp_pdob1_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_2_R_UNION
 结构说明  : OTP_2_R 寄存器结构定义。地址偏移量:0x233，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob2_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_2_R_UNION;
#endif
#define PMIC_MMW_OTP_2_R_np_otp_pdob2_d2a_START  (0)
#define PMIC_MMW_OTP_2_R_np_otp_pdob2_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_3_R_UNION
 结构说明  : OTP_3_R 寄存器结构定义。地址偏移量:0x234，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob3_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_3_R_UNION;
#endif
#define PMIC_MMW_OTP_3_R_np_otp_pdob3_d2a_START  (0)
#define PMIC_MMW_OTP_3_R_np_otp_pdob3_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_4_R_UNION
 结构说明  : OTP_4_R 寄存器结构定义。地址偏移量:0x235，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob4_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_4_R_UNION;
#endif
#define PMIC_MMW_OTP_4_R_np_otp_pdob4_d2a_START  (0)
#define PMIC_MMW_OTP_4_R_np_otp_pdob4_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_5_R_UNION
 结构说明  : OTP_5_R 寄存器结构定义。地址偏移量:0x236，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob5_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_5_R_UNION;
#endif
#define PMIC_MMW_OTP_5_R_np_otp_pdob5_d2a_START  (0)
#define PMIC_MMW_OTP_5_R_np_otp_pdob5_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_6_R_UNION
 结构说明  : OTP_6_R 寄存器结构定义。地址偏移量:0x237，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob6_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_6_R_UNION;
#endif
#define PMIC_MMW_OTP_6_R_np_otp_pdob6_d2a_START  (0)
#define PMIC_MMW_OTP_6_R_np_otp_pdob6_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_7_R_UNION
 结构说明  : OTP_7_R 寄存器结构定义。地址偏移量:0x238，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob7_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_7_R_UNION;
#endif
#define PMIC_MMW_OTP_7_R_np_otp_pdob7_d2a_START  (0)
#define PMIC_MMW_OTP_7_R_np_otp_pdob7_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_8_R_UNION
 结构说明  : OTP_8_R 寄存器结构定义。地址偏移量:0x239，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器8。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob8_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_8_R_UNION;
#endif
#define PMIC_MMW_OTP_8_R_np_otp_pdob8_d2a_START  (0)
#define PMIC_MMW_OTP_8_R_np_otp_pdob8_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_9_R_UNION
 结构说明  : OTP_9_R 寄存器结构定义。地址偏移量:0x23A，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器9。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob9_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_9_R_UNION;
#endif
#define PMIC_MMW_OTP_9_R_np_otp_pdob9_d2a_START  (0)
#define PMIC_MMW_OTP_9_R_np_otp_pdob9_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_10_R_UNION
 结构说明  : OTP_10_R 寄存器结构定义。地址偏移量:0x23B，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器10。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob10_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_10_R_UNION;
#endif
#define PMIC_MMW_OTP_10_R_np_otp_pdob10_d2a_START  (0)
#define PMIC_MMW_OTP_10_R_np_otp_pdob10_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_11_R_UNION
 结构说明  : OTP_11_R 寄存器结构定义。地址偏移量:0x23C，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器11。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob11_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_11_R_UNION;
#endif
#define PMIC_MMW_OTP_11_R_np_otp_pdob11_d2a_START  (0)
#define PMIC_MMW_OTP_11_R_np_otp_pdob11_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_12_R_UNION
 结构说明  : OTP_12_R 寄存器结构定义。地址偏移量:0x23D，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob12_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_12_R_UNION;
#endif
#define PMIC_MMW_OTP_12_R_np_otp_pdob12_d2a_START  (0)
#define PMIC_MMW_OTP_12_R_np_otp_pdob12_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_13_R_UNION
 结构说明  : OTP_13_R 寄存器结构定义。地址偏移量:0x23E，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器13。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob13_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_13_R_UNION;
#endif
#define PMIC_MMW_OTP_13_R_np_otp_pdob13_d2a_START  (0)
#define PMIC_MMW_OTP_13_R_np_otp_pdob13_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_14_R_UNION
 结构说明  : OTP_14_R 寄存器结构定义。地址偏移量:0x23F，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器14。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob14_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_14_R_UNION;
#endif
#define PMIC_MMW_OTP_14_R_np_otp_pdob14_d2a_START  (0)
#define PMIC_MMW_OTP_14_R_np_otp_pdob14_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_15_R_UNION
 结构说明  : OTP_15_R 寄存器结构定义。地址偏移量:0x240，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器15。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob15_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_15_R_UNION;
#endif
#define PMIC_MMW_OTP_15_R_np_otp_pdob15_d2a_START  (0)
#define PMIC_MMW_OTP_15_R_np_otp_pdob15_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_16_R_UNION
 结构说明  : OTP_16_R 寄存器结构定义。地址偏移量:0x241，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器16。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob16_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_16_R_UNION;
#endif
#define PMIC_MMW_OTP_16_R_np_otp_pdob16_d2a_START  (0)
#define PMIC_MMW_OTP_16_R_np_otp_pdob16_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_17_R_UNION
 结构说明  : OTP_17_R 寄存器结构定义。地址偏移量:0x242，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器17。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob17_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_17_R_UNION;
#endif
#define PMIC_MMW_OTP_17_R_np_otp_pdob17_d2a_START  (0)
#define PMIC_MMW_OTP_17_R_np_otp_pdob17_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_18_R_UNION
 结构说明  : OTP_18_R 寄存器结构定义。地址偏移量:0x243，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器18。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob18_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_18_R_UNION;
#endif
#define PMIC_MMW_OTP_18_R_np_otp_pdob18_d2a_START  (0)
#define PMIC_MMW_OTP_18_R_np_otp_pdob18_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_19_R_UNION
 结构说明  : OTP_19_R 寄存器结构定义。地址偏移量:0x244，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器19。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob19_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_19_R_UNION;
#endif
#define PMIC_MMW_OTP_19_R_np_otp_pdob19_d2a_START  (0)
#define PMIC_MMW_OTP_19_R_np_otp_pdob19_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_20_R_UNION
 结构说明  : OTP_20_R 寄存器结构定义。地址偏移量:0x245，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器20。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob20_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_20_R_UNION;
#endif
#define PMIC_MMW_OTP_20_R_np_otp_pdob20_d2a_START  (0)
#define PMIC_MMW_OTP_20_R_np_otp_pdob20_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_21_R_UNION
 结构说明  : OTP_21_R 寄存器结构定义。地址偏移量:0x246，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器21。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob21_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_21_R_UNION;
#endif
#define PMIC_MMW_OTP_21_R_np_otp_pdob21_d2a_START  (0)
#define PMIC_MMW_OTP_21_R_np_otp_pdob21_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_22_R_UNION
 结构说明  : OTP_22_R 寄存器结构定义。地址偏移量:0x247，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器22。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob22_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_22_R_UNION;
#endif
#define PMIC_MMW_OTP_22_R_np_otp_pdob22_d2a_START  (0)
#define PMIC_MMW_OTP_22_R_np_otp_pdob22_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_23_R_UNION
 结构说明  : OTP_23_R 寄存器结构定义。地址偏移量:0x248，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器23。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob23_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_23_R_UNION;
#endif
#define PMIC_MMW_OTP_23_R_np_otp_pdob23_d2a_START  (0)
#define PMIC_MMW_OTP_23_R_np_otp_pdob23_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_24_R_UNION
 结构说明  : OTP_24_R 寄存器结构定义。地址偏移量:0x249，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器24。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob24_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_24_R_UNION;
#endif
#define PMIC_MMW_OTP_24_R_np_otp_pdob24_d2a_START  (0)
#define PMIC_MMW_OTP_24_R_np_otp_pdob24_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_25_R_UNION
 结构说明  : OTP_25_R 寄存器结构定义。地址偏移量:0x24A，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器25。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob25_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_25_R_UNION;
#endif
#define PMIC_MMW_OTP_25_R_np_otp_pdob25_d2a_START  (0)
#define PMIC_MMW_OTP_25_R_np_otp_pdob25_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_26_R_UNION
 结构说明  : OTP_26_R 寄存器结构定义。地址偏移量:0x24B，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器26。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob26_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_26_R_UNION;
#endif
#define PMIC_MMW_OTP_26_R_np_otp_pdob26_d2a_START  (0)
#define PMIC_MMW_OTP_26_R_np_otp_pdob26_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_27_R_UNION
 结构说明  : OTP_27_R 寄存器结构定义。地址偏移量:0x24C，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器27。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob27_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_27_R_UNION;
#endif
#define PMIC_MMW_OTP_27_R_np_otp_pdob27_d2a_START  (0)
#define PMIC_MMW_OTP_27_R_np_otp_pdob27_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_28_R_UNION
 结构说明  : OTP_28_R 寄存器结构定义。地址偏移量:0x24D，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器28。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob28_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_28_R_UNION;
#endif
#define PMIC_MMW_OTP_28_R_np_otp_pdob28_d2a_START  (0)
#define PMIC_MMW_OTP_28_R_np_otp_pdob28_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_29_R_UNION
 结构说明  : OTP_29_R 寄存器结构定义。地址偏移量:0x24E，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器29。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob29_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_29_R_UNION;
#endif
#define PMIC_MMW_OTP_29_R_np_otp_pdob29_d2a_START  (0)
#define PMIC_MMW_OTP_29_R_np_otp_pdob29_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_30_R_UNION
 结构说明  : OTP_30_R 寄存器结构定义。地址偏移量:0x24F，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器30。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob30_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_30_R_UNION;
#endif
#define PMIC_MMW_OTP_30_R_np_otp_pdob30_d2a_START  (0)
#define PMIC_MMW_OTP_30_R_np_otp_pdob30_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_31_R_UNION
 结构说明  : OTP_31_R 寄存器结构定义。地址偏移量:0x250，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器31。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob31_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_31_R_UNION;
#endif
#define PMIC_MMW_OTP_31_R_np_otp_pdob31_d2a_START  (0)
#define PMIC_MMW_OTP_31_R_np_otp_pdob31_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_32_R_UNION
 结构说明  : OTP_32_R 寄存器结构定义。地址偏移量:0x251，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器32。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob32_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_32_R_UNION;
#endif
#define PMIC_MMW_OTP_32_R_np_otp_pdob32_d2a_START  (0)
#define PMIC_MMW_OTP_32_R_np_otp_pdob32_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_33_R_UNION
 结构说明  : OTP_33_R 寄存器结构定义。地址偏移量:0x252，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器33。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob33_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_33_R_UNION;
#endif
#define PMIC_MMW_OTP_33_R_np_otp_pdob33_d2a_START  (0)
#define PMIC_MMW_OTP_33_R_np_otp_pdob33_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_34_R_UNION
 结构说明  : OTP_34_R 寄存器结构定义。地址偏移量:0x253，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器34。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob34_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_34_R_UNION;
#endif
#define PMIC_MMW_OTP_34_R_np_otp_pdob34_d2a_START  (0)
#define PMIC_MMW_OTP_34_R_np_otp_pdob34_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_35_R_UNION
 结构说明  : OTP_35_R 寄存器结构定义。地址偏移量:0x254，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器35。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob35_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_35_R_UNION;
#endif
#define PMIC_MMW_OTP_35_R_np_otp_pdob35_d2a_START  (0)
#define PMIC_MMW_OTP_35_R_np_otp_pdob35_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_36_R_UNION
 结构说明  : OTP_36_R 寄存器结构定义。地址偏移量:0x255，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器36。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob36_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_36_R_UNION;
#endif
#define PMIC_MMW_OTP_36_R_np_otp_pdob36_d2a_START  (0)
#define PMIC_MMW_OTP_36_R_np_otp_pdob36_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_37_R_UNION
 结构说明  : OTP_37_R 寄存器结构定义。地址偏移量:0x256，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器37。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob37_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_37_R_UNION;
#endif
#define PMIC_MMW_OTP_37_R_np_otp_pdob37_d2a_START  (0)
#define PMIC_MMW_OTP_37_R_np_otp_pdob37_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_38_R_UNION
 结构说明  : OTP_38_R 寄存器结构定义。地址偏移量:0x257，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器38。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob38_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_38_R_UNION;
#endif
#define PMIC_MMW_OTP_38_R_np_otp_pdob38_d2a_START  (0)
#define PMIC_MMW_OTP_38_R_np_otp_pdob38_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_39_R_UNION
 结构说明  : OTP_39_R 寄存器结构定义。地址偏移量:0x258，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器39。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob39_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_39_R_UNION;
#endif
#define PMIC_MMW_OTP_39_R_np_otp_pdob39_d2a_START  (0)
#define PMIC_MMW_OTP_39_R_np_otp_pdob39_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_40_R_UNION
 结构说明  : OTP_40_R 寄存器结构定义。地址偏移量:0x259，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器40。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob40_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_40_R_UNION;
#endif
#define PMIC_MMW_OTP_40_R_np_otp_pdob40_d2a_START  (0)
#define PMIC_MMW_OTP_40_R_np_otp_pdob40_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_41_R_UNION
 结构说明  : OTP_41_R 寄存器结构定义。地址偏移量:0x25A，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器41。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob41_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_41_R_UNION;
#endif
#define PMIC_MMW_OTP_41_R_np_otp_pdob41_d2a_START  (0)
#define PMIC_MMW_OTP_41_R_np_otp_pdob41_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_42_R_UNION
 结构说明  : OTP_42_R 寄存器结构定义。地址偏移量:0x25B，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器42。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob42_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_42_R_UNION;
#endif
#define PMIC_MMW_OTP_42_R_np_otp_pdob42_d2a_START  (0)
#define PMIC_MMW_OTP_42_R_np_otp_pdob42_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_43_R_UNION
 结构说明  : OTP_43_R 寄存器结构定义。地址偏移量:0x25C，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器43。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob43_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_43_R_UNION;
#endif
#define PMIC_MMW_OTP_43_R_np_otp_pdob43_d2a_START  (0)
#define PMIC_MMW_OTP_43_R_np_otp_pdob43_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_44_R_UNION
 结构说明  : OTP_44_R 寄存器结构定义。地址偏移量:0x25D，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器44。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob44_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_44_R_UNION;
#endif
#define PMIC_MMW_OTP_44_R_np_otp_pdob44_d2a_START  (0)
#define PMIC_MMW_OTP_44_R_np_otp_pdob44_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_45_R_UNION
 结构说明  : OTP_45_R 寄存器结构定义。地址偏移量:0x25E，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器45。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob45_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_45_R_UNION;
#endif
#define PMIC_MMW_OTP_45_R_np_otp_pdob45_d2a_START  (0)
#define PMIC_MMW_OTP_45_R_np_otp_pdob45_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_46_R_UNION
 结构说明  : OTP_46_R 寄存器结构定义。地址偏移量:0x25F，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器46。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob46_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_46_R_UNION;
#endif
#define PMIC_MMW_OTP_46_R_np_otp_pdob46_d2a_START  (0)
#define PMIC_MMW_OTP_46_R_np_otp_pdob46_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_47_R_UNION
 结构说明  : OTP_47_R 寄存器结构定义。地址偏移量:0x260，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器47。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob47_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_47_R_UNION;
#endif
#define PMIC_MMW_OTP_47_R_np_otp_pdob47_d2a_START  (0)
#define PMIC_MMW_OTP_47_R_np_otp_pdob47_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_48_R_UNION
 结构说明  : OTP_48_R 寄存器结构定义。地址偏移量:0x261，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器48。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob48_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_48_R_UNION;
#endif
#define PMIC_MMW_OTP_48_R_np_otp_pdob48_d2a_START  (0)
#define PMIC_MMW_OTP_48_R_np_otp_pdob48_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_49_R_UNION
 结构说明  : OTP_49_R 寄存器结构定义。地址偏移量:0x262，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器49。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob49_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_49_R_UNION;
#endif
#define PMIC_MMW_OTP_49_R_np_otp_pdob49_d2a_START  (0)
#define PMIC_MMW_OTP_49_R_np_otp_pdob49_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_50_R_UNION
 结构说明  : OTP_50_R 寄存器结构定义。地址偏移量:0x263，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器50。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob50_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_50_R_UNION;
#endif
#define PMIC_MMW_OTP_50_R_np_otp_pdob50_d2a_START  (0)
#define PMIC_MMW_OTP_50_R_np_otp_pdob50_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_51_R_UNION
 结构说明  : OTP_51_R 寄存器结构定义。地址偏移量:0x264，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器51。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob51_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_51_R_UNION;
#endif
#define PMIC_MMW_OTP_51_R_np_otp_pdob51_d2a_START  (0)
#define PMIC_MMW_OTP_51_R_np_otp_pdob51_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_52_R_UNION
 结构说明  : OTP_52_R 寄存器结构定义。地址偏移量:0x265，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器52。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob52_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_52_R_UNION;
#endif
#define PMIC_MMW_OTP_52_R_np_otp_pdob52_d2a_START  (0)
#define PMIC_MMW_OTP_52_R_np_otp_pdob52_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_53_R_UNION
 结构说明  : OTP_53_R 寄存器结构定义。地址偏移量:0x266，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器53。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob53_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_53_R_UNION;
#endif
#define PMIC_MMW_OTP_53_R_np_otp_pdob53_d2a_START  (0)
#define PMIC_MMW_OTP_53_R_np_otp_pdob53_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_54_R_UNION
 结构说明  : OTP_54_R 寄存器结构定义。地址偏移量:0x267，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器54。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob54_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_54_R_UNION;
#endif
#define PMIC_MMW_OTP_54_R_np_otp_pdob54_d2a_START  (0)
#define PMIC_MMW_OTP_54_R_np_otp_pdob54_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_55_R_UNION
 结构说明  : OTP_55_R 寄存器结构定义。地址偏移量:0x268，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器55。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob55_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_55_R_UNION;
#endif
#define PMIC_MMW_OTP_55_R_np_otp_pdob55_d2a_START  (0)
#define PMIC_MMW_OTP_55_R_np_otp_pdob55_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_56_R_UNION
 结构说明  : OTP_56_R 寄存器结构定义。地址偏移量:0x269，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器56。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob56_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_56_R_UNION;
#endif
#define PMIC_MMW_OTP_56_R_np_otp_pdob56_d2a_START  (0)
#define PMIC_MMW_OTP_56_R_np_otp_pdob56_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_57_R_UNION
 结构说明  : OTP_57_R 寄存器结构定义。地址偏移量:0x26A，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器57。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob57_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_57_R_UNION;
#endif
#define PMIC_MMW_OTP_57_R_np_otp_pdob57_d2a_START  (0)
#define PMIC_MMW_OTP_57_R_np_otp_pdob57_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_58_R_UNION
 结构说明  : OTP_58_R 寄存器结构定义。地址偏移量:0x26B，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器58。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob58_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_58_R_UNION;
#endif
#define PMIC_MMW_OTP_58_R_np_otp_pdob58_d2a_START  (0)
#define PMIC_MMW_OTP_58_R_np_otp_pdob58_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_59_R_UNION
 结构说明  : OTP_59_R 寄存器结构定义。地址偏移量:0x26C，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器59。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob59_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_59_R_UNION;
#endif
#define PMIC_MMW_OTP_59_R_np_otp_pdob59_d2a_START  (0)
#define PMIC_MMW_OTP_59_R_np_otp_pdob59_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_60_R_UNION
 结构说明  : OTP_60_R 寄存器结构定义。地址偏移量:0x26D，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器60。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob60_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_60_R_UNION;
#endif
#define PMIC_MMW_OTP_60_R_np_otp_pdob60_d2a_START  (0)
#define PMIC_MMW_OTP_60_R_np_otp_pdob60_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_61_R_UNION
 结构说明  : OTP_61_R 寄存器结构定义。地址偏移量:0x26E，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器61。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob61_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_61_R_UNION;
#endif
#define PMIC_MMW_OTP_61_R_np_otp_pdob61_d2a_START  (0)
#define PMIC_MMW_OTP_61_R_np_otp_pdob61_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_62_R_UNION
 结构说明  : OTP_62_R 寄存器结构定义。地址偏移量:0x26F，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器62。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob62_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_62_R_UNION;
#endif
#define PMIC_MMW_OTP_62_R_np_otp_pdob62_d2a_START  (0)
#define PMIC_MMW_OTP_62_R_np_otp_pdob62_d2a_END    (7)


/*****************************************************************************
 结构名    : PMIC_MMW_OTP_63_R_UNION
 结构说明  : OTP_63_R 寄存器结构定义。地址偏移量:0x270，初值:0x00，宽度:8
 寄存器说明: OTP回读寄存器63。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned char      value;
    struct
    {
        unsigned char  np_otp_pdob63_d2a : 8;  /* bit[0-7]: OTP回读寄存器 */
    } reg;
} PMIC_MMW_OTP_63_R_UNION;
#endif
#define PMIC_MMW_OTP_63_R_np_otp_pdob63_d2a_START  (0)
#define PMIC_MMW_OTP_63_R_np_otp_pdob63_d2a_END    (7)






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

#endif /* end of PMIC_MMW_interface.h */
