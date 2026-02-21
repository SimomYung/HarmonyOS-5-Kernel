/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_gpio_interface.h
 * Create: 2019-10-29
 */

#ifndef __SOC_HI_GPIO_V500_INTERFACE_H__
#define __SOC_HI_GPIO_V500_INTERFACE_H__

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


/* 寄存器说明：数据寄存器1
   位域定义UNION结构:  SOC_HI_GPIO_V500_DATA_REG_HIGH_UNION */
#define SOC_HI_GPIO_V500_DATA_REG_HIGH_ADDR(base)             ((base) + (0x000UL))

/* 寄存器说明：数据寄存器0
   位域定义UNION结构:  SOC_HI_GPIO_V500_DATA_REG_LOW_UNION */
#define SOC_HI_GPIO_V500_DATA_REG_LOW_ADDR(base)              ((base) + (0x004UL))

/* 寄存器说明：中断配置寄存器0
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT0_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_ADDR(base)            ((base) + (0x008UL))

/* 寄存器说明：中断配置寄存器1
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT1_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_ADDR(base)            ((base) + (0x00CUL))

/* 寄存器说明：中断配置寄存器2
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT2_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_ADDR(base)            ((base) + (0x010UL))

/* 寄存器说明：中断配置寄存器3
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT3_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_ADDR(base)            ((base) + (0x014UL))

/* 寄存器说明：中断配置寄存器4
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT4_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_ADDR(base)            ((base) + (0x018UL))

/* 寄存器说明：中断配置寄存器5
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT5_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_ADDR(base)            ((base) + (0x01CUL))

/* 寄存器说明：中断配置寄存器6
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT6_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_ADDR(base)            ((base) + (0x020UL))

/* 寄存器说明：中断配置寄存器7
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT7_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_ADDR(base)            ((base) + (0x024UL))

/* 寄存器说明：中断配置寄存器8
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT8_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_ADDR(base)            ((base) + (0x028UL))

/* 寄存器说明：中断配置寄存器9
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT9_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_ADDR(base)            ((base) + (0x02CUL))

/* 寄存器说明：中断配置寄存器10
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT10_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_ADDR(base)           ((base) + (0x030UL))

/* 寄存器说明：中断配置寄存器11
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT11_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_ADDR(base)           ((base) + (0x034UL))

/* 寄存器说明：中断配置寄存器12
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT12_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_ADDR(base)           ((base) + (0x038UL))

/* 寄存器说明：中断配置寄存器13
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT13_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_ADDR(base)           ((base) + (0x03CUL))

/* 寄存器说明：中断配置寄存器14
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT14_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_ADDR(base)           ((base) + (0x040UL))

/* 寄存器说明：中断配置寄存器15
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT15_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_ADDR(base)           ((base) + (0x044UL))

/* 寄存器说明：中断配置寄存器16
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT16_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_ADDR(base)           ((base) + (0x048UL))

/* 寄存器说明：中断配置寄存器17
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT17_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_ADDR(base)           ((base) + (0x04CUL))

/* 寄存器说明：中断配置寄存器18
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT18_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_ADDR(base)           ((base) + (0x050UL))

/* 寄存器说明：中断配置寄存器19
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT19_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_ADDR(base)           ((base) + (0x054UL))

/* 寄存器说明：中断配置寄存器20
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT20_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_ADDR(base)           ((base) + (0x058UL))

/* 寄存器说明：中断配置寄存器21
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT21_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_ADDR(base)           ((base) + (0x05CUL))

/* 寄存器说明：中断配置寄存器22
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT22_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_ADDR(base)           ((base) + (0x060UL))

/* 寄存器说明：中断配置寄存器23
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT23_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_ADDR(base)           ((base) + (0x064UL))

/* 寄存器说明：中断配置寄存器24
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT24_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_ADDR(base)           ((base) + (0x068UL))

/* 寄存器说明：中断配置寄存器25
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT25_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_ADDR(base)           ((base) + (0x06CUL))

/* 寄存器说明：中断配置寄存器26
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT26_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_ADDR(base)           ((base) + (0x070UL))

/* 寄存器说明：中断配置寄存器27
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT27_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_ADDR(base)           ((base) + (0x074UL))

/* 寄存器说明：中断配置寄存器28
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT28_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_ADDR(base)           ((base) + (0x078UL))

/* 寄存器说明：中断配置寄存器29
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT29_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_ADDR(base)           ((base) + (0x07CUL))

/* 寄存器说明：中断配置寄存器30
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT30_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_ADDR(base)           ((base) + (0x080UL))

/* 寄存器说明：中断配置寄存器31
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT31_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_ADDR(base)           ((base) + (0x084UL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_0_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_0_ADDR(base)                ((base) + (0x088UL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_1_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_1_ADDR(base)                ((base) + (0x08CUL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_2_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_2_ADDR(base)                ((base) + (0x090UL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_3_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_3_ADDR(base)                ((base) + (0x094UL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_4_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_4_ADDR(base)                ((base) + (0x098UL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_5_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_5_ADDR(base)                ((base) + (0x09CUL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_6_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_6_ADDR(base)                ((base) + (0x0A0UL))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_7_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_7_ADDR(base)                ((base) + (0x0A4UL))

/* 寄存器说明：中断清除寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CRL_UNION */
#define SOC_HI_GPIO_V500_INTR_CRL_ADDR(base)                  ((base) + (0x0A8UL))

/* 寄存器说明：原始中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_RAW_STAT_UNION */
#define SOC_HI_GPIO_V500_INTR_RAW_STAT_ADDR(base)             ((base) + (0x0ACUL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_ADDR(base)          ((base) + (0x0B0UL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_ADDR(base)          ((base) + (0x0B4UL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_ADDR(base)          ((base) + (0x0B8UL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_ADDR(base)          ((base) + (0x0BCUL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_ADDR(base)          ((base) + (0x0C0UL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_ADDR(base)          ((base) + (0x0C4UL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_ADDR(base)          ((base) + (0x0C8UL))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_ADDR(base)          ((base) + (0x0CCUL))

/* 寄存器说明：管脚输入数据寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_IN_DATA_UNION */
#define SOC_HI_GPIO_V500_GPIO_IN_DATA_ADDR(base)              ((base) + (0x0D0UL))

/* 寄存器说明：管脚输出数据寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_OUT_DATA_UNION */
#define SOC_HI_GPIO_V500_GPIO_OUT_DATA_ADDR(base)             ((base) + (0x0D4UL))

/* 寄存器说明：管脚输出使能寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_OEN_UNION */
#define SOC_HI_GPIO_V500_GPIO_OEN_ADDR(base)                  ((base) + (0x0D8UL))

/* 寄存器说明：数据寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_SOURCE_DATA_UNION */
#define SOC_HI_GPIO_V500_SOURCE_DATA_ADDR(base)               ((base) + (0x0DCUL))

/* 寄存器说明：ID状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_ID_UNION */
#define SOC_HI_GPIO_V500_GPIO_ID_ADDR(base)                   ((base) + (0xFFCUL))


#else


/* 寄存器说明：数据寄存器1
   位域定义UNION结构:  SOC_HI_GPIO_V500_DATA_REG_HIGH_UNION */
#define SOC_HI_GPIO_V500_DATA_REG_HIGH_ADDR(base)             ((base) + (0x000))

/* 寄存器说明：数据寄存器0
   位域定义UNION结构:  SOC_HI_GPIO_V500_DATA_REG_LOW_UNION */
#define SOC_HI_GPIO_V500_DATA_REG_LOW_ADDR(base)              ((base) + (0x004))

/* 寄存器说明：中断配置寄存器0
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT0_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_ADDR(base)            ((base) + (0x008))

/* 寄存器说明：中断配置寄存器1
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT1_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_ADDR(base)            ((base) + (0x00C))

/* 寄存器说明：中断配置寄存器2
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT2_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_ADDR(base)            ((base) + (0x010))

/* 寄存器说明：中断配置寄存器3
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT3_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_ADDR(base)            ((base) + (0x014))

/* 寄存器说明：中断配置寄存器4
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT4_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_ADDR(base)            ((base) + (0x018))

/* 寄存器说明：中断配置寄存器5
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT5_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_ADDR(base)            ((base) + (0x01C))

/* 寄存器说明：中断配置寄存器6
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT6_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_ADDR(base)            ((base) + (0x020))

/* 寄存器说明：中断配置寄存器7
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT7_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_ADDR(base)            ((base) + (0x024))

/* 寄存器说明：中断配置寄存器8
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT8_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_ADDR(base)            ((base) + (0x028))

/* 寄存器说明：中断配置寄存器9
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT9_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_ADDR(base)            ((base) + (0x02C))

/* 寄存器说明：中断配置寄存器10
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT10_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_ADDR(base)           ((base) + (0x030))

/* 寄存器说明：中断配置寄存器11
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT11_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_ADDR(base)           ((base) + (0x034))

/* 寄存器说明：中断配置寄存器12
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT12_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_ADDR(base)           ((base) + (0x038))

/* 寄存器说明：中断配置寄存器13
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT13_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_ADDR(base)           ((base) + (0x03C))

/* 寄存器说明：中断配置寄存器14
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT14_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_ADDR(base)           ((base) + (0x040))

/* 寄存器说明：中断配置寄存器15
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT15_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_ADDR(base)           ((base) + (0x044))

/* 寄存器说明：中断配置寄存器16
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT16_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_ADDR(base)           ((base) + (0x048))

/* 寄存器说明：中断配置寄存器17
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT17_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_ADDR(base)           ((base) + (0x04C))

/* 寄存器说明：中断配置寄存器18
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT18_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_ADDR(base)           ((base) + (0x050))

/* 寄存器说明：中断配置寄存器19
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT19_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_ADDR(base)           ((base) + (0x054))

/* 寄存器说明：中断配置寄存器20
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT20_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_ADDR(base)           ((base) + (0x058))

/* 寄存器说明：中断配置寄存器21
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT21_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_ADDR(base)           ((base) + (0x05C))

/* 寄存器说明：中断配置寄存器22
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT22_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_ADDR(base)           ((base) + (0x060))

/* 寄存器说明：中断配置寄存器23
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT23_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_ADDR(base)           ((base) + (0x064))

/* 寄存器说明：中断配置寄存器24
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT24_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_ADDR(base)           ((base) + (0x068))

/* 寄存器说明：中断配置寄存器25
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT25_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_ADDR(base)           ((base) + (0x06C))

/* 寄存器说明：中断配置寄存器26
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT26_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_ADDR(base)           ((base) + (0x070))

/* 寄存器说明：中断配置寄存器27
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT27_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_ADDR(base)           ((base) + (0x074))

/* 寄存器说明：中断配置寄存器28
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT28_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_ADDR(base)           ((base) + (0x078))

/* 寄存器说明：中断配置寄存器29
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT29_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_ADDR(base)           ((base) + (0x07C))

/* 寄存器说明：中断配置寄存器30
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT30_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_ADDR(base)           ((base) + (0x080))

/* 寄存器说明：中断配置寄存器31
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CTRL_BIT31_UNION */
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_ADDR(base)           ((base) + (0x084))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_0_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_0_ADDR(base)                ((base) + (0x088))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_1_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_1_ADDR(base)                ((base) + (0x08C))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_2_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_2_ADDR(base)                ((base) + (0x090))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_3_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_3_ADDR(base)                ((base) + (0x094))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_4_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_4_ADDR(base)                ((base) + (0x098))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_5_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_5_ADDR(base)                ((base) + (0x09C))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_6_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_6_ADDR(base)                ((base) + (0x0A0))

/* 寄存器说明：中断屏蔽状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_MSK_7_UNION */
#define SOC_HI_GPIO_V500_INTR_MSK_7_ADDR(base)                ((base) + (0x0A4))

/* 寄存器说明：中断清除寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_CRL_UNION */
#define SOC_HI_GPIO_V500_INTR_CRL_ADDR(base)                  ((base) + (0x0A8))

/* 寄存器说明：原始中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_RAW_STAT_UNION */
#define SOC_HI_GPIO_V500_INTR_RAW_STAT_ADDR(base)             ((base) + (0x0AC))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_ADDR(base)          ((base) + (0x0B0))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_ADDR(base)          ((base) + (0x0B4))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_ADDR(base)          ((base) + (0x0B8))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_ADDR(base)          ((base) + (0x0BC))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_ADDR(base)          ((base) + (0x0C0))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_ADDR(base)          ((base) + (0x0C4))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_ADDR(base)          ((base) + (0x0C8))

/* 寄存器说明：中断状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_UNION */
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_ADDR(base)          ((base) + (0x0CC))

/* 寄存器说明：管脚输入数据寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_IN_DATA_UNION */
#define SOC_HI_GPIO_V500_GPIO_IN_DATA_ADDR(base)              ((base) + (0x0D0))

/* 寄存器说明：管脚输出数据寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_OUT_DATA_UNION */
#define SOC_HI_GPIO_V500_GPIO_OUT_DATA_ADDR(base)             ((base) + (0x0D4))

/* 寄存器说明：管脚输出使能寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_OEN_UNION */
#define SOC_HI_GPIO_V500_GPIO_OEN_ADDR(base)                  ((base) + (0x0D8))

/* 寄存器说明：数据寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_SOURCE_DATA_UNION */
#define SOC_HI_GPIO_V500_SOURCE_DATA_ADDR(base)               ((base) + (0x0DC))

/* 寄存器说明：ID状态寄存器
   位域定义UNION结构:  SOC_HI_GPIO_V500_GPIO_ID_UNION */
#define SOC_HI_GPIO_V500_GPIO_ID_ADDR(base)                   ((base) + (0xFFC))


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
 结构名    : SOC_HI_GPIO_V500_DATA_REG_HIGH_UNION
 结构说明  : DATA_REG_HIGH 寄存器结构定义。地址偏移量:0x000，初值:0x00000000，宽度:32
 寄存器说明: 数据寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data_reg_high  : 16; /* bit[0-15] : data数据寄存器的高16bit[31:16] */
        unsigned int  gpio_data_msk1 : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_DATA_REG_HIGH_UNION;
#endif
#define SOC_HI_GPIO_V500_DATA_REG_HIGH_data_reg_high_START   (0)
#define SOC_HI_GPIO_V500_DATA_REG_HIGH_data_reg_high_END     (15)
#define SOC_HI_GPIO_V500_DATA_REG_HIGH_gpio_data_msk1_START  (16)
#define SOC_HI_GPIO_V500_DATA_REG_HIGH_gpio_data_msk1_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_DATA_REG_LOW_UNION
 结构说明  : DATA_REG_LOW 寄存器结构定义。地址偏移量:0x004，初值:0x00000000，宽度:32
 寄存器说明: 数据寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data_reg_low   : 16; /* bit[0-15] : data数据寄存器的低16bit[15:0] */
        unsigned int  gpio_data_msk1 : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_DATA_REG_LOW_UNION;
#endif
#define SOC_HI_GPIO_V500_DATA_REG_LOW_data_reg_low_START    (0)
#define SOC_HI_GPIO_V500_DATA_REG_LOW_data_reg_low_END      (15)
#define SOC_HI_GPIO_V500_DATA_REG_LOW_gpio_data_msk1_START  (16)
#define SOC_HI_GPIO_V500_DATA_REG_LOW_gpio_data_msk1_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT0_UNION
 结构说明  : INTR_CTRL_BIT0 寄存器结构定义。地址偏移量:0x008，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_0 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_0      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_0     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_0      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT0_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_intr_trig_type_0_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_intr_trig_type_0_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_edge_type_0_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_edge_type_0_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_level_type_0_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_level_type_0_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_hw_sw_sel_0_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT0_hw_sw_sel_0_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT1_UNION
 结构说明  : INTR_CTRL_BIT1 寄存器结构定义。地址偏移量:0x00C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_1 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_1      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_1     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_1      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT1_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_intr_trig_type_1_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_intr_trig_type_1_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_edge_type_1_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_edge_type_1_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_level_type_1_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_level_type_1_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_hw_sw_sel_1_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT1_hw_sw_sel_1_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT2_UNION
 结构说明  : INTR_CTRL_BIT2 寄存器结构定义。地址偏移量:0x010，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_2 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_2      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_2     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_2      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT2_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_intr_trig_type_2_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_intr_trig_type_2_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_edge_type_2_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_edge_type_2_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_level_type_2_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_level_type_2_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_hw_sw_sel_2_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT2_hw_sw_sel_2_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT3_UNION
 结构说明  : INTR_CTRL_BIT3 寄存器结构定义。地址偏移量:0x014，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_3 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_3      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_3     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_3      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT3_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_intr_trig_type_3_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_intr_trig_type_3_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_edge_type_3_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_edge_type_3_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_level_type_3_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_level_type_3_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_hw_sw_sel_3_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT3_hw_sw_sel_3_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT4_UNION
 结构说明  : INTR_CTRL_BIT4 寄存器结构定义。地址偏移量:0x018，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器4
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_4 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_4      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_4     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_4      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT4_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_intr_trig_type_4_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_intr_trig_type_4_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_edge_type_4_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_edge_type_4_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_level_type_4_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_level_type_4_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_hw_sw_sel_4_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT4_hw_sw_sel_4_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT5_UNION
 结构说明  : INTR_CTRL_BIT5 寄存器结构定义。地址偏移量:0x01C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器5
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_5 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_5      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_5     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_5      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT5_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_intr_trig_type_5_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_intr_trig_type_5_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_edge_type_5_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_edge_type_5_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_level_type_5_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_level_type_5_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_hw_sw_sel_5_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT5_hw_sw_sel_5_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT6_UNION
 结构说明  : INTR_CTRL_BIT6 寄存器结构定义。地址偏移量:0x020，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器6
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_6 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_6      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_6     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_6      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT6_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_intr_trig_type_6_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_intr_trig_type_6_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_edge_type_6_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_edge_type_6_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_level_type_6_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_level_type_6_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_hw_sw_sel_6_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT6_hw_sw_sel_6_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT7_UNION
 结构说明  : INTR_CTRL_BIT7 寄存器结构定义。地址偏移量:0x024，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器7
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_7 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_7      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_7     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_7      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT7_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_intr_trig_type_7_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_intr_trig_type_7_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_edge_type_7_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_edge_type_7_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_level_type_7_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_level_type_7_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_hw_sw_sel_7_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT7_hw_sw_sel_7_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT8_UNION
 结构说明  : INTR_CTRL_BIT8 寄存器结构定义。地址偏移量:0x028，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器8
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_8 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_8      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_8     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_8      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT8_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_intr_trig_type_8_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_intr_trig_type_8_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_edge_type_8_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_edge_type_8_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_level_type_8_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_level_type_8_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_hw_sw_sel_8_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT8_hw_sw_sel_8_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT9_UNION
 结构说明  : INTR_CTRL_BIT9 寄存器结构定义。地址偏移量:0x02C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器9
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_9 : 1;  /* bit[0]    : 0:沿触发中断；
                                                            1：电平触发中断 */
        unsigned int  edge_type_9      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                            00:下降沿触发中断; 
                                                            01:上升沿触发中断;
                                                            1X:双沿触发中断 */
        unsigned int  level_type_9     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                            0：低电平触发中断；
                                                            1：高电平触发中断。 */
        unsigned int  hw_sw_sel_9      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                            0：数据来源是引脚；
                                                            1：数据来源是apb软件配置。 */
        unsigned int  reserved_0       : 11; /* bit[5-15] :  */
        unsigned int  reserved_1       : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT9_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_intr_trig_type_9_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_intr_trig_type_9_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_edge_type_9_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_edge_type_9_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_level_type_9_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_level_type_9_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_hw_sw_sel_9_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT9_hw_sw_sel_9_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT10_UNION
 结构说明  : INTR_CTRL_BIT10 寄存器结构定义。地址偏移量:0x030，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器10
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_10 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_10      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_10     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_10      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT10_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_intr_trig_type_10_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_intr_trig_type_10_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_edge_type_10_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_edge_type_10_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_level_type_10_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_level_type_10_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_hw_sw_sel_10_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT10_hw_sw_sel_10_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT11_UNION
 结构说明  : INTR_CTRL_BIT11 寄存器结构定义。地址偏移量:0x034，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器11
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_11 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_11      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_11     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_11      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT11_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_intr_trig_type_11_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_intr_trig_type_11_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_edge_type_11_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_edge_type_11_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_level_type_11_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_level_type_11_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_hw_sw_sel_11_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT11_hw_sw_sel_11_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT12_UNION
 结构说明  : INTR_CTRL_BIT12 寄存器结构定义。地址偏移量:0x038，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器12
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_12 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_12      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_12     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_12      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT12_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_intr_trig_type_12_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_intr_trig_type_12_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_edge_type_12_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_edge_type_12_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_level_type_12_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_level_type_12_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_hw_sw_sel_12_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT12_hw_sw_sel_12_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT13_UNION
 结构说明  : INTR_CTRL_BIT13 寄存器结构定义。地址偏移量:0x03C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器13
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_13 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_13      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_13     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_13      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT13_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_intr_trig_type_13_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_intr_trig_type_13_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_edge_type_13_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_edge_type_13_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_level_type_13_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_level_type_13_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_hw_sw_sel_13_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT13_hw_sw_sel_13_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT14_UNION
 结构说明  : INTR_CTRL_BIT14 寄存器结构定义。地址偏移量:0x040，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器14
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_14 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_14      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_14     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_14      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT14_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_intr_trig_type_14_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_intr_trig_type_14_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_edge_type_14_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_edge_type_14_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_level_type_14_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_level_type_14_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_hw_sw_sel_14_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT14_hw_sw_sel_14_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT15_UNION
 结构说明  : INTR_CTRL_BIT15 寄存器结构定义。地址偏移量:0x044，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器15
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_15 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_15      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_15     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_15      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT15_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_intr_trig_type_15_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_intr_trig_type_15_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_edge_type_15_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_edge_type_15_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_level_type_15_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_level_type_15_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_hw_sw_sel_15_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT15_hw_sw_sel_15_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT16_UNION
 结构说明  : INTR_CTRL_BIT16 寄存器结构定义。地址偏移量:0x048，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器16
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_16 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_16      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_16     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_16      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT16_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_intr_trig_type_16_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_intr_trig_type_16_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_edge_type_16_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_edge_type_16_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_level_type_16_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_level_type_16_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_hw_sw_sel_16_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT16_hw_sw_sel_16_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT17_UNION
 结构说明  : INTR_CTRL_BIT17 寄存器结构定义。地址偏移量:0x04C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器17
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_17 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_17      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_17     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_17      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT17_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_intr_trig_type_17_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_intr_trig_type_17_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_edge_type_17_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_edge_type_17_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_level_type_17_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_level_type_17_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_hw_sw_sel_17_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT17_hw_sw_sel_17_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT18_UNION
 结构说明  : INTR_CTRL_BIT18 寄存器结构定义。地址偏移量:0x050，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器18
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_18 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_18      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_18     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_18      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT18_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_intr_trig_type_18_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_intr_trig_type_18_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_edge_type_18_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_edge_type_18_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_level_type_18_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_level_type_18_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_hw_sw_sel_18_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT18_hw_sw_sel_18_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT19_UNION
 结构说明  : INTR_CTRL_BIT19 寄存器结构定义。地址偏移量:0x054，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器19
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_19 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_19      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_19     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_19      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT19_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_intr_trig_type_19_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_intr_trig_type_19_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_edge_type_19_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_edge_type_19_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_level_type_19_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_level_type_19_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_hw_sw_sel_19_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT19_hw_sw_sel_19_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT20_UNION
 结构说明  : INTR_CTRL_BIT20 寄存器结构定义。地址偏移量:0x058，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器20
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_20 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_20      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_20     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_20      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT20_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_intr_trig_type_20_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_intr_trig_type_20_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_edge_type_20_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_edge_type_20_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_level_type_20_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_level_type_20_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_hw_sw_sel_20_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT20_hw_sw_sel_20_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT21_UNION
 结构说明  : INTR_CTRL_BIT21 寄存器结构定义。地址偏移量:0x05C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器21
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_21 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_21      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_21     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_21      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT21_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_intr_trig_type_21_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_intr_trig_type_21_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_edge_type_21_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_edge_type_21_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_level_type_21_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_level_type_21_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_hw_sw_sel_21_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT21_hw_sw_sel_21_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT22_UNION
 结构说明  : INTR_CTRL_BIT22 寄存器结构定义。地址偏移量:0x060，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器22
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_22 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_22      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_22     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_22      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT22_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_intr_trig_type_22_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_intr_trig_type_22_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_edge_type_22_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_edge_type_22_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_level_type_22_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_level_type_22_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_hw_sw_sel_22_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT22_hw_sw_sel_22_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT23_UNION
 结构说明  : INTR_CTRL_BIT23 寄存器结构定义。地址偏移量:0x064，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器23
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_23 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_23      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_23     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_23      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT23_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_intr_trig_type_23_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_intr_trig_type_23_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_edge_type_23_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_edge_type_23_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_level_type_23_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_level_type_23_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_hw_sw_sel_23_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT23_hw_sw_sel_23_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT24_UNION
 结构说明  : INTR_CTRL_BIT24 寄存器结构定义。地址偏移量:0x068，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器24
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_24 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_24      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_24     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_24      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT24_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_intr_trig_type_24_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_intr_trig_type_24_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_edge_type_24_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_edge_type_24_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_level_type_24_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_level_type_24_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_hw_sw_sel_24_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT24_hw_sw_sel_24_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT25_UNION
 结构说明  : INTR_CTRL_BIT25 寄存器结构定义。地址偏移量:0x06C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器25
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_25 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_25      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_25     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_25      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT25_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_intr_trig_type_25_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_intr_trig_type_25_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_edge_type_25_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_edge_type_25_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_level_type_25_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_level_type_25_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_hw_sw_sel_25_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT25_hw_sw_sel_25_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT26_UNION
 结构说明  : INTR_CTRL_BIT26 寄存器结构定义。地址偏移量:0x070，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器26
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_26 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_26      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_26     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_26      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT26_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_intr_trig_type_26_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_intr_trig_type_26_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_edge_type_26_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_edge_type_26_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_level_type_26_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_level_type_26_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_hw_sw_sel_26_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT26_hw_sw_sel_26_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT27_UNION
 结构说明  : INTR_CTRL_BIT27 寄存器结构定义。地址偏移量:0x074，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器27
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_27 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_27      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_27     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_27      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT27_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_intr_trig_type_27_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_intr_trig_type_27_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_edge_type_27_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_edge_type_27_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_level_type_27_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_level_type_27_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_hw_sw_sel_27_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT27_hw_sw_sel_27_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT28_UNION
 结构说明  : INTR_CTRL_BIT28 寄存器结构定义。地址偏移量:0x078，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器28
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_28 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_28      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_28     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_28      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT28_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_intr_trig_type_28_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_intr_trig_type_28_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_edge_type_28_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_edge_type_28_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_level_type_28_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_level_type_28_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_hw_sw_sel_28_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT28_hw_sw_sel_28_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT29_UNION
 结构说明  : INTR_CTRL_BIT29 寄存器结构定义。地址偏移量:0x07C，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器29
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_29 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_29      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_29     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_29      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT29_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_intr_trig_type_29_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_intr_trig_type_29_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_edge_type_29_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_edge_type_29_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_level_type_29_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_level_type_29_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_hw_sw_sel_29_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT29_hw_sw_sel_29_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT30_UNION
 结构说明  : INTR_CTRL_BIT30 寄存器结构定义。地址偏移量:0x080，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器30
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_30 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_30      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_30     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_30      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT30_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_intr_trig_type_30_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_intr_trig_type_30_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_edge_type_30_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_edge_type_30_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_level_type_30_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_level_type_30_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_hw_sw_sel_30_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT30_hw_sw_sel_30_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CTRL_BIT31_UNION
 结构说明  : INTR_CTRL_BIT31 寄存器结构定义。地址偏移量:0x084，初值:0x00000000，宽度:32
 寄存器说明: 中断配置寄存器31
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_trig_type_31 : 1;  /* bit[0]    : 0:沿触发中断；
                                                             1：电平触发中断 */
        unsigned int  edge_type_31      : 2;  /* bit[1-2]  : 沿触发方式选择:
                                                             00:下降沿触发中断; 
                                                             01:上升沿触发中断;
                                                             1X:双沿触发中断 */
        unsigned int  level_type_31     : 1;  /* bit[3]    : 高电平/低电平选择。
                                                             0：低电平触发中断；
                                                             1：高电平触发中断。 */
        unsigned int  hw_sw_sel_31      : 1;  /* bit[4]    : 软件硬件来源选择。
                                                             0：数据来源是引脚；
                                                             1：数据来源是apb软件配置。 */
        unsigned int  reserved_0        : 11; /* bit[5-15] :  */
        unsigned int  reserved_1        : 16; /* bit[16-31]: 写mask bit，分别mask bit[15；0]，只有当mask bit[x+16]为1时，bit[x]的写才有效， */
    } reg;
} SOC_HI_GPIO_V500_INTR_CTRL_BIT31_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_intr_trig_type_31_START  (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_intr_trig_type_31_END    (0)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_edge_type_31_START       (1)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_edge_type_31_END         (2)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_level_type_31_START      (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_level_type_31_END        (3)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_hw_sw_sel_31_START       (4)
#define SOC_HI_GPIO_V500_INTR_CTRL_BIT31_hw_sw_sel_31_END         (4)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_0_UNION
 结构说明  : INTR_MSK_0 寄存器结构定义。地址偏移量:0x088，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_0 : 32; /* bit[0-31]: 中断0的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_0_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_0_intr_msk_0_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_0_intr_msk_0_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_1_UNION
 结构说明  : INTR_MSK_1 寄存器结构定义。地址偏移量:0x08C，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_1 : 32; /* bit[0-31]: 中断1的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_1_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_1_intr_msk_1_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_1_intr_msk_1_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_2_UNION
 结构说明  : INTR_MSK_2 寄存器结构定义。地址偏移量:0x090，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_2 : 32; /* bit[0-31]: 中断2的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_2_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_2_intr_msk_2_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_2_intr_msk_2_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_3_UNION
 结构说明  : INTR_MSK_3 寄存器结构定义。地址偏移量:0x094，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_3 : 32; /* bit[0-31]: 中断3的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_3_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_3_intr_msk_3_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_3_intr_msk_3_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_4_UNION
 结构说明  : INTR_MSK_4 寄存器结构定义。地址偏移量:0x098，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_4 : 32; /* bit[0-31]: 中断4的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_4_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_4_intr_msk_4_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_4_intr_msk_4_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_5_UNION
 结构说明  : INTR_MSK_5 寄存器结构定义。地址偏移量:0x09C，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_5 : 32; /* bit[0-31]: 中断5的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_5_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_5_intr_msk_5_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_5_intr_msk_5_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_6_UNION
 结构说明  : INTR_MSK_6 寄存器结构定义。地址偏移量:0x0A0，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_6 : 32; /* bit[0-31]: 中断6的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_6_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_6_intr_msk_6_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_6_intr_msk_6_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_MSK_7_UNION
 结构说明  : INTR_MSK_7 寄存器结构定义。地址偏移量:0x0A4，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 中断屏蔽状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_msk_7 : 32; /* bit[0-31]: 中断7的屏蔽。
                                                     1：屏蔽相应管脚的中断；
                                                     0：使能相应管脚的中断。 */
    } reg;
} SOC_HI_GPIO_V500_INTR_MSK_7_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_MSK_7_intr_msk_7_START  (0)
#define SOC_HI_GPIO_V500_INTR_MSK_7_intr_msk_7_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_CLR_UNION
 结构说明  : INTR_CLR 寄存器结构定义。地址偏移量:0x0A8，初值:0x00000000，宽度:32
 寄存器说明: 中断清除寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_clr_0  : 1;  /* bit[0] : 中断清除。
                                                    0：对中断无影响；
                                                    1：清除相应中断。 */
        unsigned int  intr_clr_1  : 1;  /* bit[1] :  */
        unsigned int  intr_clr_2  : 1;  /* bit[2] :  */
        unsigned int  intr_clr_3  : 1;  /* bit[3] :  */
        unsigned int  intr_clr_4  : 1;  /* bit[4] :  */
        unsigned int  intr_clr_5  : 1;  /* bit[5] :  */
        unsigned int  intr_clr_6  : 1;  /* bit[6] :  */
        unsigned int  intr_clr_7  : 1;  /* bit[7] :  */
        unsigned int  intr_clr_8  : 1;  /* bit[8] :  */
        unsigned int  intr_clr_9  : 1;  /* bit[9] :  */
        unsigned int  intr_clr_10 : 1;  /* bit[10]:  */
        unsigned int  intr_clr_11 : 1;  /* bit[11]:  */
        unsigned int  intr_clr_12 : 1;  /* bit[12]:  */
        unsigned int  intr_clr_13 : 1;  /* bit[13]:  */
        unsigned int  intr_clr_14 : 1;  /* bit[14]:  */
        unsigned int  intr_clr_15 : 1;  /* bit[15]:  */
        unsigned int  intr_clr_16 : 1;  /* bit[16]:  */
        unsigned int  intr_clr_17 : 1;  /* bit[17]:  */
        unsigned int  intr_clr_18 : 1;  /* bit[18]:  */
        unsigned int  intr_clr_19 : 1;  /* bit[19]:  */
        unsigned int  intr_clr_20 : 1;  /* bit[20]:  */
        unsigned int  intr_clr_21 : 1;  /* bit[21]:  */
        unsigned int  intr_clr_22 : 1;  /* bit[22]:  */
        unsigned int  intr_clr_23 : 1;  /* bit[23]:  */
        unsigned int  intr_clr_24 : 1;  /* bit[24]:  */
        unsigned int  intr_clr_25 : 1;  /* bit[25]:  */
        unsigned int  intr_clr_26 : 1;  /* bit[26]:  */
        unsigned int  intr_clr_27 : 1;  /* bit[27]:  */
        unsigned int  intr_clr_28 : 1;  /* bit[28]:  */
        unsigned int  intr_clr_29 : 1;  /* bit[29]:  */
        unsigned int  intr_clr_30 : 1;  /* bit[30]:  */
        unsigned int  intr_clr_31 : 1;  /* bit[31]:  */
    } reg;
} SOC_HI_GPIO_V500_INTR_CRL_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_0_START   (0)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_0_END     (0)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_1_START   (1)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_1_END     (1)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_2_START   (2)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_2_END     (2)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_3_START   (3)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_3_END     (3)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_4_START   (4)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_4_END     (4)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_5_START   (5)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_5_END     (5)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_6_START   (6)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_6_END     (6)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_7_START   (7)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_7_END     (7)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_8_START   (8)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_8_END     (8)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_9_START   (9)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_9_END     (9)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_10_START  (10)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_10_END    (10)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_11_START  (11)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_11_END    (11)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_12_START  (12)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_12_END    (12)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_13_START  (13)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_13_END    (13)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_14_START  (14)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_14_END    (14)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_15_START  (15)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_15_END    (15)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_16_START  (16)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_16_END    (16)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_17_START  (17)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_17_END    (17)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_18_START  (18)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_18_END    (18)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_19_START  (19)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_19_END    (19)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_20_START  (20)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_20_END    (20)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_21_START  (21)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_21_END    (21)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_22_START  (22)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_22_END    (22)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_23_START  (23)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_23_END    (23)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_24_START  (24)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_24_END    (24)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_25_START  (25)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_25_END    (25)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_26_START  (26)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_26_END    (26)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_27_START  (27)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_27_END    (27)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_28_START  (28)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_28_END    (28)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_29_START  (29)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_29_END    (29)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_30_START  (30)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_30_END    (30)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_31_START  (31)
#define SOC_HI_GPIO_V500_INTR_CRL_intr_clr_31_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_RAW_STAT_UNION
 结构说明  : INTR_RAW_STAT 寄存器结构定义。地址偏移量:0x0AC，初值:0x00000000，宽度:32
 寄存器说明: 原始中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_raw_stat : 32; /* bit[0-31]: 原始中断的回读，屏蔽前的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_RAW_STAT_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_RAW_STAT_intr_raw_stat_START  (0)
#define SOC_HI_GPIO_V500_INTR_RAW_STAT_intr_raw_stat_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_UNION
 结构说明  : INTR_GPIO_STAT_0 寄存器结构定义。地址偏移量:0x0B0，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_0 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_0屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_intr_gpio_stat_0_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_0_intr_gpio_stat_0_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_UNION
 结构说明  : INTR_GPIO_STAT_1 寄存器结构定义。地址偏移量:0x0B4，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_1 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_1屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_intr_gpio_stat_1_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_1_intr_gpio_stat_1_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_UNION
 结构说明  : INTR_GPIO_STAT_2 寄存器结构定义。地址偏移量:0x0B8，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_2 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_2屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_intr_gpio_stat_2_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_2_intr_gpio_stat_2_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_UNION
 结构说明  : INTR_GPIO_STAT_3 寄存器结构定义。地址偏移量:0x0BC，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_3 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_3屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_intr_gpio_stat_3_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_3_intr_gpio_stat_3_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_UNION
 结构说明  : INTR_GPIO_STAT_4 寄存器结构定义。地址偏移量:0x0C0，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_4 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_4屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_intr_gpio_stat_4_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_4_intr_gpio_stat_4_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_UNION
 结构说明  : INTR_GPIO_STAT_5 寄存器结构定义。地址偏移量:0x0C4，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_5 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_5屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_intr_gpio_stat_5_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_5_intr_gpio_stat_5_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_UNION
 结构说明  : INTR_GPIO_STAT_6 寄存器结构定义。地址偏移量:0x0C8，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_6 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_6屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_intr_gpio_stat_6_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_6_intr_gpio_stat_6_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_UNION
 结构说明  : INTR_GPIO_STAT_7 寄存器结构定义。地址偏移量:0x0CC，初值:0x00000000，宽度:32
 寄存器说明: 中断状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_gpio_stat_7 : 32; /* bit[0-31]: 经过屏蔽寄存器INTR_MSK_7屏蔽后的中断状态 */
    } reg;
} SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_UNION;
#endif
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_intr_gpio_stat_7_START  (0)
#define SOC_HI_GPIO_V500_INTR_GPIO_STAT_7_intr_gpio_stat_7_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_GPIO_IN_DATA_UNION
 结构说明  : GPIO_IN_DATA 寄存器结构定义。地址偏移量:0x0D0，初值:0x00000000，宽度:32
 寄存器说明: 管脚输入数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpio_in_data : 32; /* bit[0-31]: GPIO的输入数据。用于维测，是管脚直接输入数据，未同步 */
    } reg;
} SOC_HI_GPIO_V500_GPIO_IN_DATA_UNION;
#endif
#define SOC_HI_GPIO_V500_GPIO_IN_DATA_gpio_in_data_START  (0)
#define SOC_HI_GPIO_V500_GPIO_IN_DATA_gpio_in_data_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_GPIO_OUT_DATA_UNION
 结构说明  : GPIO_OUT_DATA 寄存器结构定义。地址偏移量:0x0D4，初值:0x00000000，宽度:32
 寄存器说明: 管脚输出数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpio_out_data : 32; /* bit[0-31]: GPIO输出数据。 */
    } reg;
} SOC_HI_GPIO_V500_GPIO_OUT_DATA_UNION;
#endif
#define SOC_HI_GPIO_V500_GPIO_OUT_DATA_gpio_out_data_START  (0)
#define SOC_HI_GPIO_V500_GPIO_OUT_DATA_gpio_out_data_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_GPIO_OEN_UNION
 结构说明  : GPIO_OEN 寄存器结构定义。地址偏移量:0x0D8，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 管脚输出使能寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpio_oen : 32; /* bit[0-31]: GPIO输出管脚使能，低有效 */
    } reg;
} SOC_HI_GPIO_V500_GPIO_OEN_UNION;
#endif
#define SOC_HI_GPIO_V500_GPIO_OEN_gpio_oen_START  (0)
#define SOC_HI_GPIO_V500_GPIO_OEN_gpio_oen_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_SOURCE_DATA_UNION
 结构说明  : SOURCE_DATA 寄存器结构定义。地址偏移量:0x0DC，初值:0x00000000，宽度:32
 寄存器说明: 数据寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  source_data : 32; /* bit[0-31]: bitN配置寄存器的hw_sw_sel_n为1,对应source_data[n]数据来自data数据寄存器的[n];
                                                      bitN配置寄存器的hw_sw_sel_n为0,对应source_data[n]数据来自同步后的管脚输入数据寄存器的[n];
                                                      可以用于直接回读管脚输入数据 */
    } reg;
} SOC_HI_GPIO_V500_SOURCE_DATA_UNION;
#endif
#define SOC_HI_GPIO_V500_SOURCE_DATA_source_data_START  (0)
#define SOC_HI_GPIO_V500_SOURCE_DATA_source_data_END    (31)


/*****************************************************************************
 结构名    : SOC_HI_GPIO_V500_GPIO_ID_UNION
 结构说明  : GPIO_ID 寄存器结构定义。地址偏移量:0xFFC，初值:0x00005500，宽度:32
 寄存器说明: ID状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gpio_id : 32; /* bit[0-31]:  */
    } reg;
} SOC_HI_GPIO_V500_GPIO_ID_UNION;
#endif
#define SOC_HI_GPIO_V500_GPIO_ID_gpio_id_START  (0)
#define SOC_HI_GPIO_V500_GPIO_ID_gpio_id_END    (31)






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

#endif /* end of soc_hi_gpio_v500_interface.h */
