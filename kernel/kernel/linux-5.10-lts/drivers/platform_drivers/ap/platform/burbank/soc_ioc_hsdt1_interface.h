/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_ioc_hsdt1_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_IOC_HSDT1_INTERFACE_H__
#define __SOC_IOC_HSDT1_INTERFACE_H__

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


/* 寄存器说明：SD_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_000_UNION */
#define SOC_IOC_HSDT1_IOMG_000_ADDR(base)                     ((base) + (0x000UL))

/* 寄存器说明：SD_CMD管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_001_UNION */
#define SOC_IOC_HSDT1_IOMG_001_ADDR(base)                     ((base) + (0x004UL))

/* 寄存器说明：SD_DATA0管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_002_UNION */
#define SOC_IOC_HSDT1_IOMG_002_ADDR(base)                     ((base) + (0x008UL))

/* 寄存器说明：SD_DATA1管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_003_UNION */
#define SOC_IOC_HSDT1_IOMG_003_ADDR(base)                     ((base) + (0x00CUL))

/* 寄存器说明：SD_DATA2管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_004_UNION */
#define SOC_IOC_HSDT1_IOMG_004_ADDR(base)                     ((base) + (0x010UL))

/* 寄存器说明：SD_DATA3管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_005_UNION */
#define SOC_IOC_HSDT1_IOMG_005_ADDR(base)                     ((base) + (0x014UL))

/* 寄存器说明：USIM0_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_006_UNION */
#define SOC_IOC_HSDT1_IOMG_006_ADDR(base)                     ((base) + (0x018UL))

/* 寄存器说明：USIM0_RST管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_007_UNION */
#define SOC_IOC_HSDT1_IOMG_007_ADDR(base)                     ((base) + (0x01CUL))

/* 寄存器说明：USIM0_DATA管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_008_UNION */
#define SOC_IOC_HSDT1_IOMG_008_ADDR(base)                     ((base) + (0x020UL))

/* 寄存器说明：USIM1_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_009_UNION */
#define SOC_IOC_HSDT1_IOMG_009_ADDR(base)                     ((base) + (0x024UL))

/* 寄存器说明：USIM1_RST管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_010_UNION */
#define SOC_IOC_HSDT1_IOMG_010_ADDR(base)                     ((base) + (0x028UL))

/* 寄存器说明：USIM1_DATA管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_011_UNION */
#define SOC_IOC_HSDT1_IOMG_011_ADDR(base)                     ((base) + (0x02CUL))

/* 寄存器说明：USB2_ULPI_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_012_UNION */
#define SOC_IOC_HSDT1_IOMG_012_ADDR(base)                     ((base) + (0x030UL))

/* 寄存器说明：USB2_ULPI_DATA0管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_013_UNION */
#define SOC_IOC_HSDT1_IOMG_013_ADDR(base)                     ((base) + (0x034UL))

/* 寄存器说明：USB2_ULPI_DATA1管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_014_UNION */
#define SOC_IOC_HSDT1_IOMG_014_ADDR(base)                     ((base) + (0x038UL))

/* 寄存器说明：USB2_ULPI_DATA2管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_015_UNION */
#define SOC_IOC_HSDT1_IOMG_015_ADDR(base)                     ((base) + (0x03CUL))

/* 寄存器说明：USB2_ULPI_DATA3管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_016_UNION */
#define SOC_IOC_HSDT1_IOMG_016_ADDR(base)                     ((base) + (0x040UL))

/* 寄存器说明：USB2_ULPI_DATA4管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_017_UNION */
#define SOC_IOC_HSDT1_IOMG_017_ADDR(base)                     ((base) + (0x044UL))

/* 寄存器说明：USB2_ULPI_DATA5管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_018_UNION */
#define SOC_IOC_HSDT1_IOMG_018_ADDR(base)                     ((base) + (0x048UL))

/* 寄存器说明：USB2_ULPI_DATA6管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_019_UNION */
#define SOC_IOC_HSDT1_IOMG_019_ADDR(base)                     ((base) + (0x04CUL))

/* 寄存器说明：USB2_ULPI_DATA7管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_020_UNION */
#define SOC_IOC_HSDT1_IOMG_020_ADDR(base)                     ((base) + (0x050UL))

/* 寄存器说明：USB2_ULPI_DIR管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_021_UNION */
#define SOC_IOC_HSDT1_IOMG_021_ADDR(base)                     ((base) + (0x054UL))

/* 寄存器说明：USB2_ULPI_NXT管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_022_UNION */
#define SOC_IOC_HSDT1_IOMG_022_ADDR(base)                     ((base) + (0x058UL))

/* 寄存器说明：USB2_ULPI_STP管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_023_UNION */
#define SOC_IOC_HSDT1_IOMG_023_ADDR(base)                     ((base) + (0x05CUL))

/* 寄存器说明：USB2_ULPI_RST管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_024_UNION */
#define SOC_IOC_HSDT1_IOMG_024_ADDR(base)                     ((base) + (0x060UL))

/* 寄存器说明：ULPI_REF_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_025_UNION */
#define SOC_IOC_HSDT1_IOMG_025_ADDR(base)                     ((base) + (0x064UL))

/* 寄存器说明：SD_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_000_UNION */
#define SOC_IOC_HSDT1_IOCG_000_ADDR(base)                     ((base) + (0x800UL))

/* 寄存器说明：SD_CMD功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_001_UNION */
#define SOC_IOC_HSDT1_IOCG_001_ADDR(base)                     ((base) + (0x804UL))

/* 寄存器说明：SD_DATA0功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_002_UNION */
#define SOC_IOC_HSDT1_IOCG_002_ADDR(base)                     ((base) + (0x808UL))

/* 寄存器说明：SD_DATA1功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_003_UNION */
#define SOC_IOC_HSDT1_IOCG_003_ADDR(base)                     ((base) + (0x80CUL))

/* 寄存器说明：SD_DATA2功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_004_UNION */
#define SOC_IOC_HSDT1_IOCG_004_ADDR(base)                     ((base) + (0x810UL))

/* 寄存器说明：SD_DATA3功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_005_UNION */
#define SOC_IOC_HSDT1_IOCG_005_ADDR(base)                     ((base) + (0x814UL))

/* 寄存器说明：USIM0_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_006_UNION */
#define SOC_IOC_HSDT1_IOCG_006_ADDR(base)                     ((base) + (0x818UL))

/* 寄存器说明：USIM0_RST功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_007_UNION */
#define SOC_IOC_HSDT1_IOCG_007_ADDR(base)                     ((base) + (0x81CUL))

/* 寄存器说明：USIM0_DATA功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_008_UNION */
#define SOC_IOC_HSDT1_IOCG_008_ADDR(base)                     ((base) + (0x820UL))

/* 寄存器说明：USIM1_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_009_UNION */
#define SOC_IOC_HSDT1_IOCG_009_ADDR(base)                     ((base) + (0x824UL))

/* 寄存器说明：USIM1_RST功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_010_UNION */
#define SOC_IOC_HSDT1_IOCG_010_ADDR(base)                     ((base) + (0x828UL))

/* 寄存器说明：USIM1_DATA功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_011_UNION */
#define SOC_IOC_HSDT1_IOCG_011_ADDR(base)                     ((base) + (0x82CUL))

/* 寄存器说明：USB2_ULPI_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_012_UNION */
#define SOC_IOC_HSDT1_IOCG_012_ADDR(base)                     ((base) + (0x830UL))

/* 寄存器说明：USB2_ULPI_DATA0功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_013_UNION */
#define SOC_IOC_HSDT1_IOCG_013_ADDR(base)                     ((base) + (0x834UL))

/* 寄存器说明：USB2_ULPI_DATA1功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_014_UNION */
#define SOC_IOC_HSDT1_IOCG_014_ADDR(base)                     ((base) + (0x838UL))

/* 寄存器说明：USB2_ULPI_DATA2功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_015_UNION */
#define SOC_IOC_HSDT1_IOCG_015_ADDR(base)                     ((base) + (0x83CUL))

/* 寄存器说明：USB2_ULPI_DATA3功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_016_UNION */
#define SOC_IOC_HSDT1_IOCG_016_ADDR(base)                     ((base) + (0x840UL))

/* 寄存器说明：USB2_ULPI_DATA4功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_017_UNION */
#define SOC_IOC_HSDT1_IOCG_017_ADDR(base)                     ((base) + (0x844UL))

/* 寄存器说明：USB2_ULPI_DATA5功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_018_UNION */
#define SOC_IOC_HSDT1_IOCG_018_ADDR(base)                     ((base) + (0x848UL))

/* 寄存器说明：USB2_ULPI_DATA6功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_019_UNION */
#define SOC_IOC_HSDT1_IOCG_019_ADDR(base)                     ((base) + (0x84CUL))

/* 寄存器说明：USB2_ULPI_DATA7功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_020_UNION */
#define SOC_IOC_HSDT1_IOCG_020_ADDR(base)                     ((base) + (0x850UL))

/* 寄存器说明：USB2_ULPI_DIR功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_021_UNION */
#define SOC_IOC_HSDT1_IOCG_021_ADDR(base)                     ((base) + (0x854UL))

/* 寄存器说明：USB2_ULPI_NXT功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_022_UNION */
#define SOC_IOC_HSDT1_IOCG_022_ADDR(base)                     ((base) + (0x858UL))

/* 寄存器说明：USB2_ULPI_STP功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_023_UNION */
#define SOC_IOC_HSDT1_IOCG_023_ADDR(base)                     ((base) + (0x85CUL))

/* 寄存器说明：USB2_ULPI_RST功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_024_UNION */
#define SOC_IOC_HSDT1_IOCG_024_ADDR(base)                     ((base) + (0x860UL))

/* 寄存器说明：ULPI_REF_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_025_UNION */
#define SOC_IOC_HSDT1_IOCG_025_ADDR(base)                     ((base) + (0x864UL))

/* 寄存器说明：CLK_SYS_HILINK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_026_UNION */
#define SOC_IOC_HSDT1_IOCG_026_ADDR(base)                     ((base) + (0x868UL))


#else


/* 寄存器说明：SD_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_000_UNION */
#define SOC_IOC_HSDT1_IOMG_000_ADDR(base)                     ((base) + (0x000))

/* 寄存器说明：SD_CMD管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_001_UNION */
#define SOC_IOC_HSDT1_IOMG_001_ADDR(base)                     ((base) + (0x004))

/* 寄存器说明：SD_DATA0管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_002_UNION */
#define SOC_IOC_HSDT1_IOMG_002_ADDR(base)                     ((base) + (0x008))

/* 寄存器说明：SD_DATA1管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_003_UNION */
#define SOC_IOC_HSDT1_IOMG_003_ADDR(base)                     ((base) + (0x00C))

/* 寄存器说明：SD_DATA2管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_004_UNION */
#define SOC_IOC_HSDT1_IOMG_004_ADDR(base)                     ((base) + (0x010))

/* 寄存器说明：SD_DATA3管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_005_UNION */
#define SOC_IOC_HSDT1_IOMG_005_ADDR(base)                     ((base) + (0x014))

/* 寄存器说明：USIM0_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_006_UNION */
#define SOC_IOC_HSDT1_IOMG_006_ADDR(base)                     ((base) + (0x018))

/* 寄存器说明：USIM0_RST管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_007_UNION */
#define SOC_IOC_HSDT1_IOMG_007_ADDR(base)                     ((base) + (0x01C))

/* 寄存器说明：USIM0_DATA管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_008_UNION */
#define SOC_IOC_HSDT1_IOMG_008_ADDR(base)                     ((base) + (0x020))

/* 寄存器说明：USIM1_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_009_UNION */
#define SOC_IOC_HSDT1_IOMG_009_ADDR(base)                     ((base) + (0x024))

/* 寄存器说明：USIM1_RST管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_010_UNION */
#define SOC_IOC_HSDT1_IOMG_010_ADDR(base)                     ((base) + (0x028))

/* 寄存器说明：USIM1_DATA管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_011_UNION */
#define SOC_IOC_HSDT1_IOMG_011_ADDR(base)                     ((base) + (0x02C))

/* 寄存器说明：USB2_ULPI_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_012_UNION */
#define SOC_IOC_HSDT1_IOMG_012_ADDR(base)                     ((base) + (0x030))

/* 寄存器说明：USB2_ULPI_DATA0管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_013_UNION */
#define SOC_IOC_HSDT1_IOMG_013_ADDR(base)                     ((base) + (0x034))

/* 寄存器说明：USB2_ULPI_DATA1管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_014_UNION */
#define SOC_IOC_HSDT1_IOMG_014_ADDR(base)                     ((base) + (0x038))

/* 寄存器说明：USB2_ULPI_DATA2管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_015_UNION */
#define SOC_IOC_HSDT1_IOMG_015_ADDR(base)                     ((base) + (0x03C))

/* 寄存器说明：USB2_ULPI_DATA3管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_016_UNION */
#define SOC_IOC_HSDT1_IOMG_016_ADDR(base)                     ((base) + (0x040))

/* 寄存器说明：USB2_ULPI_DATA4管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_017_UNION */
#define SOC_IOC_HSDT1_IOMG_017_ADDR(base)                     ((base) + (0x044))

/* 寄存器说明：USB2_ULPI_DATA5管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_018_UNION */
#define SOC_IOC_HSDT1_IOMG_018_ADDR(base)                     ((base) + (0x048))

/* 寄存器说明：USB2_ULPI_DATA6管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_019_UNION */
#define SOC_IOC_HSDT1_IOMG_019_ADDR(base)                     ((base) + (0x04C))

/* 寄存器说明：USB2_ULPI_DATA7管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_020_UNION */
#define SOC_IOC_HSDT1_IOMG_020_ADDR(base)                     ((base) + (0x050))

/* 寄存器说明：USB2_ULPI_DIR管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_021_UNION */
#define SOC_IOC_HSDT1_IOMG_021_ADDR(base)                     ((base) + (0x054))

/* 寄存器说明：USB2_ULPI_NXT管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_022_UNION */
#define SOC_IOC_HSDT1_IOMG_022_ADDR(base)                     ((base) + (0x058))

/* 寄存器说明：USB2_ULPI_STP管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_023_UNION */
#define SOC_IOC_HSDT1_IOMG_023_ADDR(base)                     ((base) + (0x05C))

/* 寄存器说明：USB2_ULPI_RST管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_024_UNION */
#define SOC_IOC_HSDT1_IOMG_024_ADDR(base)                     ((base) + (0x060))

/* 寄存器说明：ULPI_REF_CLK管脚复用控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOMG_025_UNION */
#define SOC_IOC_HSDT1_IOMG_025_ADDR(base)                     ((base) + (0x064))

/* 寄存器说明：SD_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_000_UNION */
#define SOC_IOC_HSDT1_IOCG_000_ADDR(base)                     ((base) + (0x800))

/* 寄存器说明：SD_CMD功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_001_UNION */
#define SOC_IOC_HSDT1_IOCG_001_ADDR(base)                     ((base) + (0x804))

/* 寄存器说明：SD_DATA0功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_002_UNION */
#define SOC_IOC_HSDT1_IOCG_002_ADDR(base)                     ((base) + (0x808))

/* 寄存器说明：SD_DATA1功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_003_UNION */
#define SOC_IOC_HSDT1_IOCG_003_ADDR(base)                     ((base) + (0x80C))

/* 寄存器说明：SD_DATA2功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_004_UNION */
#define SOC_IOC_HSDT1_IOCG_004_ADDR(base)                     ((base) + (0x810))

/* 寄存器说明：SD_DATA3功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_005_UNION */
#define SOC_IOC_HSDT1_IOCG_005_ADDR(base)                     ((base) + (0x814))

/* 寄存器说明：USIM0_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_006_UNION */
#define SOC_IOC_HSDT1_IOCG_006_ADDR(base)                     ((base) + (0x818))

/* 寄存器说明：USIM0_RST功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_007_UNION */
#define SOC_IOC_HSDT1_IOCG_007_ADDR(base)                     ((base) + (0x81C))

/* 寄存器说明：USIM0_DATA功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_008_UNION */
#define SOC_IOC_HSDT1_IOCG_008_ADDR(base)                     ((base) + (0x820))

/* 寄存器说明：USIM1_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_009_UNION */
#define SOC_IOC_HSDT1_IOCG_009_ADDR(base)                     ((base) + (0x824))

/* 寄存器说明：USIM1_RST功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_010_UNION */
#define SOC_IOC_HSDT1_IOCG_010_ADDR(base)                     ((base) + (0x828))

/* 寄存器说明：USIM1_DATA功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_011_UNION */
#define SOC_IOC_HSDT1_IOCG_011_ADDR(base)                     ((base) + (0x82C))

/* 寄存器说明：USB2_ULPI_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_012_UNION */
#define SOC_IOC_HSDT1_IOCG_012_ADDR(base)                     ((base) + (0x830))

/* 寄存器说明：USB2_ULPI_DATA0功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_013_UNION */
#define SOC_IOC_HSDT1_IOCG_013_ADDR(base)                     ((base) + (0x834))

/* 寄存器说明：USB2_ULPI_DATA1功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_014_UNION */
#define SOC_IOC_HSDT1_IOCG_014_ADDR(base)                     ((base) + (0x838))

/* 寄存器说明：USB2_ULPI_DATA2功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_015_UNION */
#define SOC_IOC_HSDT1_IOCG_015_ADDR(base)                     ((base) + (0x83C))

/* 寄存器说明：USB2_ULPI_DATA3功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_016_UNION */
#define SOC_IOC_HSDT1_IOCG_016_ADDR(base)                     ((base) + (0x840))

/* 寄存器说明：USB2_ULPI_DATA4功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_017_UNION */
#define SOC_IOC_HSDT1_IOCG_017_ADDR(base)                     ((base) + (0x844))

/* 寄存器说明：USB2_ULPI_DATA5功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_018_UNION */
#define SOC_IOC_HSDT1_IOCG_018_ADDR(base)                     ((base) + (0x848))

/* 寄存器说明：USB2_ULPI_DATA6功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_019_UNION */
#define SOC_IOC_HSDT1_IOCG_019_ADDR(base)                     ((base) + (0x84C))

/* 寄存器说明：USB2_ULPI_DATA7功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_020_UNION */
#define SOC_IOC_HSDT1_IOCG_020_ADDR(base)                     ((base) + (0x850))

/* 寄存器说明：USB2_ULPI_DIR功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_021_UNION */
#define SOC_IOC_HSDT1_IOCG_021_ADDR(base)                     ((base) + (0x854))

/* 寄存器说明：USB2_ULPI_NXT功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_022_UNION */
#define SOC_IOC_HSDT1_IOCG_022_ADDR(base)                     ((base) + (0x858))

/* 寄存器说明：USB2_ULPI_STP功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_023_UNION */
#define SOC_IOC_HSDT1_IOCG_023_ADDR(base)                     ((base) + (0x85C))

/* 寄存器说明：USB2_ULPI_RST功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_024_UNION */
#define SOC_IOC_HSDT1_IOCG_024_ADDR(base)                     ((base) + (0x860))

/* 寄存器说明：ULPI_REF_CLK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_025_UNION */
#define SOC_IOC_HSDT1_IOCG_025_ADDR(base)                     ((base) + (0x864))

/* 寄存器说明：CLK_SYS_HILINK功能管脚控制寄存器。
   位域定义UNION结构:  SOC_IOC_HSDT1_IOCG_026_UNION */
#define SOC_IOC_HSDT1_IOCG_026_ADDR(base)                     ((base) + (0x868))


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
 结构名    : SOC_IOC_HSDT1_IOMG_000_UNION
 结构说明  : IOMG_000 寄存器结构定义。地址偏移量:0x000，初值:0x00000000，宽度:32
 寄存器说明: SD_CLK管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_000 : 3;  /* bit[0-2] : SD_CLK管脚的具体复用情况。
                                                   000：GPIO_120；
                                                   001：SD_CLK；
                                                   011：SD_CLK_TEST；
                                                   100：USIM1_CLK；
                                                   101：CLK_PCIEIO_TEST；
                                                   110：SD_CLK_HTOL_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_000_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_000_iomg_000_START  (0)
#define SOC_IOC_HSDT1_IOMG_000_iomg_000_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_001_UNION
 结构说明  : IOMG_001 寄存器结构定义。地址偏移量:0x004，初值:0x00000000，宽度:32
 寄存器说明: SD_CMD管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_001 : 3;  /* bit[0-2] : SD_CMD管脚的具体复用情况。
                                                   000：GPIO_121；
                                                   001：SD_CMD；
                                                   010：JTAG_TMS_SD；
                                                   011：SD_CMD_TEST；
                                                   110：SD_CMD_HTOL_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_001_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_001_iomg_001_START  (0)
#define SOC_IOC_HSDT1_IOMG_001_iomg_001_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_002_UNION
 结构说明  : IOMG_002 寄存器结构定义。地址偏移量:0x008，初值:0x00000000，宽度:32
 寄存器说明: SD_DATA0管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_002 : 3;  /* bit[0-2] : SD_DATA0管脚的具体复用情况。
                                                   000：GPIO_122；
                                                   001：SD_DATA0；
                                                   010：JTAG_TCK_SD；
                                                   011：SD_DATA0_TEST；
                                                   100：USIM1_RST；
                                                   110：SD_DATA0_HTOL_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_002_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_002_iomg_002_START  (0)
#define SOC_IOC_HSDT1_IOMG_002_iomg_002_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_003_UNION
 结构说明  : IOMG_003 寄存器结构定义。地址偏移量:0x00C，初值:0x00000000，宽度:32
 寄存器说明: SD_DATA1管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_003 : 3;  /* bit[0-2] : SD_DATA1管脚的具体复用情况。
                                                   000：GPIO_123；
                                                   001：SD_DATA1；
                                                   010：JTAG_TDI_SD；
                                                   011：SD_DATA1_TEST；
                                                   100：USIM1_DATA；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_003_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_003_iomg_003_START  (0)
#define SOC_IOC_HSDT1_IOMG_003_iomg_003_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_004_UNION
 结构说明  : IOMG_004 寄存器结构定义。地址偏移量:0x010，初值:0x00000000，宽度:32
 寄存器说明: SD_DATA2管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_004 : 2;  /* bit[0-1] : SD_DATA2管脚的具体复用情况。
                                                   00：GPIO_124；
                                                   01：SD_DATA2；
                                                   10：JTAG_TDO_SD；
                                                   11：SD_DATA2_TEST。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_004_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_004_iomg_004_START  (0)
#define SOC_IOC_HSDT1_IOMG_004_iomg_004_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_005_UNION
 结构说明  : IOMG_005 寄存器结构定义。地址偏移量:0x014，初值:0x00000000，宽度:32
 寄存器说明: SD_DATA3管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_005 : 2;  /* bit[0-1] : SD_DATA3管脚的具体复用情况。
                                                   00：GPIO_125；
                                                   01：SD_DATA3；
                                                   10：JTAG_TRST_N_SD；
                                                   11：SD_DATA3_TEST。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_005_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_005_iomg_005_START  (0)
#define SOC_IOC_HSDT1_IOMG_005_iomg_005_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_006_UNION
 结构说明  : IOMG_006 寄存器结构定义。地址偏移量:0x018，初值:0x00000000，宽度:32
 寄存器说明: USIM0_CLK管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_006 : 3;  /* bit[0-2] : USIM0_CLK管脚的具体复用情况。
                                                   000：GPIO_126；
                                                   001：USIM0_CLK；
                                                   010：JTAG_TDO_SD；
                                                   111：GROUND；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_006_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_006_iomg_006_START  (0)
#define SOC_IOC_HSDT1_IOMG_006_iomg_006_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_007_UNION
 结构说明  : IOMG_007 寄存器结构定义。地址偏移量:0x01C，初值:0x00000000，宽度:32
 寄存器说明: USIM0_RST管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_007 : 3;  /* bit[0-2] : USIM0_RST管脚的具体复用情况。
                                                   000：GPIO_127；
                                                   001：USIM0_RST；
                                                   010：JTAG_TRST_N_SD；
                                                   111：GROUND；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_007_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_007_iomg_007_START  (0)
#define SOC_IOC_HSDT1_IOMG_007_iomg_007_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_008_UNION
 结构说明  : IOMG_008 寄存器结构定义。地址偏移量:0x020，初值:0x00000000，宽度:32
 寄存器说明: USIM0_DATA管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_008 : 3;  /* bit[0-2] : USIM0_DATA管脚的具体复用情况。
                                                   000：GPIO_128；
                                                   001：USIM0_DATA；
                                                   010：JTAG_TCK_SD；
                                                   111：GROUND；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_008_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_008_iomg_008_START  (0)
#define SOC_IOC_HSDT1_IOMG_008_iomg_008_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_009_UNION
 结构说明  : IOMG_009 寄存器结构定义。地址偏移量:0x024，初值:0x00000000，宽度:32
 寄存器说明: USIM1_CLK管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_009 : 3;  /* bit[0-2] : USIM1_CLK管脚的具体复用情况。
                                                   000：GPIO_129；
                                                   001：USIM1_CLK；
                                                   111：GROUND；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_009_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_009_iomg_009_START  (0)
#define SOC_IOC_HSDT1_IOMG_009_iomg_009_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_010_UNION
 结构说明  : IOMG_010 寄存器结构定义。地址偏移量:0x028，初值:0x00000000，宽度:32
 寄存器说明: USIM1_RST管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_010 : 3;  /* bit[0-2] : USIM1_RST管脚的具体复用情况。
                                                   000：GPIO_130；
                                                   001：USIM1_RST；
                                                   010：JTAG_TDI_SD；
                                                   111：GROUND；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_010_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_010_iomg_010_START  (0)
#define SOC_IOC_HSDT1_IOMG_010_iomg_010_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_011_UNION
 结构说明  : IOMG_011 寄存器结构定义。地址偏移量:0x02C，初值:0x00000000，宽度:32
 寄存器说明: USIM1_DATA管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_011 : 3;  /* bit[0-2] : USIM1_DATA管脚的具体复用情况。
                                                   000：GPIO_131；
                                                   001：USIM1_DATA；
                                                   010：JTAG_TMS_SD；
                                                   111：GROUND；
                                                   其他：保留。 */
        unsigned int  reserved : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_011_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_011_iomg_011_START  (0)
#define SOC_IOC_HSDT1_IOMG_011_iomg_011_END    (2)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_012_UNION
 结构说明  : IOMG_012 寄存器结构定义。地址偏移量:0x030，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_CLK管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_012 : 2;  /* bit[0-1] : USB2_ULPI_CLK管脚的具体复用情况。
                                                   00：GPIO_132；
                                                   01：USB2_ULPI_CLK；
                                                   10：USB2_ULPI_CLK_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_012_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_012_iomg_012_START  (0)
#define SOC_IOC_HSDT1_IOMG_012_iomg_012_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_013_UNION
 结构说明  : IOMG_013 寄存器结构定义。地址偏移量:0x034，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA0管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_013 : 2;  /* bit[0-1] : USB2_ULPI_DATA0管脚的具体复用情况。
                                                   00：GPIO_133；
                                                   01：USB2_ULPI_DATA0；
                                                   10：USB2_ULPI_DATA0_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_013_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_013_iomg_013_START  (0)
#define SOC_IOC_HSDT1_IOMG_013_iomg_013_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_014_UNION
 结构说明  : IOMG_014 寄存器结构定义。地址偏移量:0x038，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA1管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_014 : 2;  /* bit[0-1] : USB2_ULPI_DATA1管脚的具体复用情况。
                                                   00：GPIO_134；
                                                   01：USB2_ULPI_DATA1；
                                                   10：USB2_ULPI_DATA1_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_014_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_014_iomg_014_START  (0)
#define SOC_IOC_HSDT1_IOMG_014_iomg_014_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_015_UNION
 结构说明  : IOMG_015 寄存器结构定义。地址偏移量:0x03C，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA2管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_015 : 2;  /* bit[0-1] : USB2_ULPI_DATA2管脚的具体复用情况。
                                                   00：GPIO_135；
                                                   01：USB2_ULPI_DATA2；
                                                   10：USB2_ULPI_DATA2_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_015_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_015_iomg_015_START  (0)
#define SOC_IOC_HSDT1_IOMG_015_iomg_015_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_016_UNION
 结构说明  : IOMG_016 寄存器结构定义。地址偏移量:0x040，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA3管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_016 : 2;  /* bit[0-1] : USB2_ULPI_DATA3管脚的具体复用情况。
                                                   00：GPIO_136；
                                                   01：USB2_ULPI_DATA3；
                                                   10：USB2_ULPI_DATA3_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_016_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_016_iomg_016_START  (0)
#define SOC_IOC_HSDT1_IOMG_016_iomg_016_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_017_UNION
 结构说明  : IOMG_017 寄存器结构定义。地址偏移量:0x044，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA4管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_017 : 2;  /* bit[0-1] : USB2_ULPI_DATA4管脚的具体复用情况。
                                                   00：GPIO_137；
                                                   01：USB2_ULPI_DATA4；
                                                   10：USB2_ULPI_DATA4_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_017_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_017_iomg_017_START  (0)
#define SOC_IOC_HSDT1_IOMG_017_iomg_017_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_018_UNION
 结构说明  : IOMG_018 寄存器结构定义。地址偏移量:0x048，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA5管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_018 : 2;  /* bit[0-1] : USB2_ULPI_DATA5管脚的具体复用情况。
                                                   00：GPIO_138；
                                                   01：USB2_ULPI_DATA5；
                                                   10：USB2_ULPI_DATA5_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_018_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_018_iomg_018_START  (0)
#define SOC_IOC_HSDT1_IOMG_018_iomg_018_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_019_UNION
 结构说明  : IOMG_019 寄存器结构定义。地址偏移量:0x04C，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA6管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_019 : 2;  /* bit[0-1] : USB2_ULPI_DATA6管脚的具体复用情况。
                                                   00：GPIO_139；
                                                   01：USB2_ULPI_DATA6；
                                                   10：USB2_ULPI_DATA6_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_019_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_019_iomg_019_START  (0)
#define SOC_IOC_HSDT1_IOMG_019_iomg_019_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_020_UNION
 结构说明  : IOMG_020 寄存器结构定义。地址偏移量:0x050，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DATA7管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_020 : 2;  /* bit[0-1] : USB2_ULPI_DATA7管脚的具体复用情况。
                                                   00：GPIO_140；
                                                   01：USB2_ULPI_DATA7；
                                                   10：USB2_ULPI_DATA7_TEST；
                                                   其他：保留。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_020_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_020_iomg_020_START  (0)
#define SOC_IOC_HSDT1_IOMG_020_iomg_020_END    (1)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_021_UNION
 结构说明  : IOMG_021 寄存器结构定义。地址偏移量:0x054，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_DIR管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_021 : 1;  /* bit[0]   : USB2_ULPI_DIR管脚的具体复用情况。
                                                   0：GPIO_141；
                                                   1：USB2_ULPI_DIR。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_021_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_021_iomg_021_START  (0)
#define SOC_IOC_HSDT1_IOMG_021_iomg_021_END    (0)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_022_UNION
 结构说明  : IOMG_022 寄存器结构定义。地址偏移量:0x058，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_NXT管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_022 : 1;  /* bit[0]   : USB2_ULPI_NXT管脚的具体复用情况。
                                                   0：GPIO_142；
                                                   1：USB2_ULPI_NXT。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_022_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_022_iomg_022_START  (0)
#define SOC_IOC_HSDT1_IOMG_022_iomg_022_END    (0)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_023_UNION
 结构说明  : IOMG_023 寄存器结构定义。地址偏移量:0x05C，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_STP管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_023 : 1;  /* bit[0]   : USB2_ULPI_STP管脚的具体复用情况。
                                                   0：GPIO_143；
                                                   1：USB2_ULPI_STP。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_023_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_023_iomg_023_START  (0)
#define SOC_IOC_HSDT1_IOMG_023_iomg_023_END    (0)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_024_UNION
 结构说明  : IOMG_024 寄存器结构定义。地址偏移量:0x060，初值:0x00000000，宽度:32
 寄存器说明: USB2_ULPI_RST管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_024 : 1;  /* bit[0]   : USB2_ULPI_RST管脚的具体复用情况。
                                                   0：GPIO_144；
                                                   1：USB2_ULPI_RST。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_024_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_024_iomg_024_START  (0)
#define SOC_IOC_HSDT1_IOMG_024_iomg_024_END    (0)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOMG_025_UNION
 结构说明  : IOMG_025 寄存器结构定义。地址偏移量:0x064，初值:0x00000000，宽度:32
 寄存器说明: ULPI_REF_CLK管脚复用控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iomg_025 : 1;  /* bit[0]   : ULPI_REF_CLK管脚的具体复用情况。
                                                   0：GPIO_145；
                                                   1：ULPI_REF_CLK。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOMG_025_UNION;
#endif
#define SOC_IOC_HSDT1_IOMG_025_iomg_025_START  (0)
#define SOC_IOC_HSDT1_IOMG_025_iomg_025_END    (0)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_000_UNION
 结构说明  : IOCG_000 寄存器结构定义。地址偏移量:0x800，初值:0x00000022，宽度:32
 寄存器说明: SD_CLK功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_000_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_000_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_000_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_000_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_000_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_000_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_000_ds3 : 1;  /* bit[7]   : DS3,DS2,DS1,DS0配置驱动力。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_000_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds3_START  (7)
#define SOC_IOC_HSDT1_IOCG_000_iocg_000_ds3_END    (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_001_UNION
 结构说明  : IOCG_001 寄存器结构定义。地址偏移量:0x804，初值:0x00000012，宽度:32
 寄存器说明: SD_CMD功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_001_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_001_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_001_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_001_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_001_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_001_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_001_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_001_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_001_iocg_001_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_002_UNION
 结构说明  : IOCG_002 寄存器结构定义。地址偏移量:0x808，初值:0x00000012，宽度:32
 寄存器说明: SD_DATA0功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_002_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_002_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_002_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_002_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_002_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_002_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_002_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_002_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_002_iocg_002_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_003_UNION
 结构说明  : IOCG_003 寄存器结构定义。地址偏移量:0x80C，初值:0x00000012，宽度:32
 寄存器说明: SD_DATA1功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_003_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_003_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_003_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_003_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_003_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_003_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_003_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_003_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_003_iocg_003_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_004_UNION
 结构说明  : IOCG_004 寄存器结构定义。地址偏移量:0x810，初值:0x00000012，宽度:32
 寄存器说明: SD_DATA2功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_004_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_004_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_004_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_004_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_004_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_004_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_004_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_004_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_004_iocg_004_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_005_UNION
 结构说明  : IOCG_005 寄存器结构定义。地址偏移量:0x814，初值:0x00000012，宽度:32
 寄存器说明: SD_DATA3功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_005_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_005_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_005_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_005_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_005_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_005_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_005_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_005_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_005_iocg_005_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_006_UNION
 结构说明  : IOCG_006 寄存器结构定义。地址偏移量:0x818，初值:0x00000012，宽度:32
 寄存器说明: USIM0_CLK功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_006_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_006_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_006_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_006_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_006_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_006_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_006_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_006_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_006_iocg_006_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_007_UNION
 结构说明  : IOCG_007 寄存器结构定义。地址偏移量:0x81C，初值:0x00000012，宽度:32
 寄存器说明: USIM0_RST功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_007_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_007_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_007_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_007_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_007_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_007_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_007_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_007_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_007_iocg_007_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_008_UNION
 结构说明  : IOCG_008 寄存器结构定义。地址偏移量:0x820，初值:0x00000012，宽度:32
 寄存器说明: USIM0_DATA功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_008_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_008_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_008_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_008_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_008_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_008_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_008_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_008_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_008_iocg_008_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_009_UNION
 结构说明  : IOCG_009 寄存器结构定义。地址偏移量:0x824，初值:0x00000012，宽度:32
 寄存器说明: USIM1_CLK功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_009_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_009_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_009_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_009_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_009_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_009_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_009_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_009_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_009_iocg_009_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_010_UNION
 结构说明  : IOCG_010 寄存器结构定义。地址偏移量:0x828，初值:0x00000012，宽度:32
 寄存器说明: USIM1_RST功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_010_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_010_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_010_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_010_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_010_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_010_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_010_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_010_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_010_iocg_010_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_011_UNION
 结构说明  : IOCG_011 寄存器结构定义。地址偏移量:0x82C，初值:0x00000012，宽度:32
 寄存器说明: USIM1_DATA功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_011_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_011_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_011_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_011_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_011_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_011_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_011_sl  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_011_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_sl_START   (7)
#define SOC_IOC_HSDT1_IOCG_011_iocg_011_sl_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_012_UNION
 结构说明  : IOCG_012 寄存器结构定义。地址偏移量:0x830，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_CLK功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_012_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_012_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_012_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_012_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_012_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_012_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_012_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_012_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_012_iocg_012_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_013_UNION
 结构说明  : IOCG_013 寄存器结构定义。地址偏移量:0x834，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA0功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_013_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_013_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_013_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_013_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_013_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_013_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_013_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_013_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_013_iocg_013_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_014_UNION
 结构说明  : IOCG_014 寄存器结构定义。地址偏移量:0x838，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA1功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_014_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_014_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_014_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_014_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_014_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_014_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_014_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_014_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_014_iocg_014_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_015_UNION
 结构说明  : IOCG_015 寄存器结构定义。地址偏移量:0x83C，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA2功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_015_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_015_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_015_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_015_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_015_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_015_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_015_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_015_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_015_iocg_015_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_016_UNION
 结构说明  : IOCG_016 寄存器结构定义。地址偏移量:0x840，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA3功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_016_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_016_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_016_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_016_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_016_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_016_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_016_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_016_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_016_iocg_016_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_017_UNION
 结构说明  : IOCG_017 寄存器结构定义。地址偏移量:0x844，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA4功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_017_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_017_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_017_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_017_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_017_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_017_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_017_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_017_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_017_iocg_017_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_018_UNION
 结构说明  : IOCG_018 寄存器结构定义。地址偏移量:0x848，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA5功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_018_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_018_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_018_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_018_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_018_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_018_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_018_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_018_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_018_iocg_018_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_019_UNION
 结构说明  : IOCG_019 寄存器结构定义。地址偏移量:0x84C，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA6功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_019_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_019_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_019_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_019_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_019_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_019_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_019_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_019_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_019_iocg_019_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_020_UNION
 结构说明  : IOCG_020 寄存器结构定义。地址偏移量:0x850，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DATA7功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_020_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_020_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_020_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_020_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_020_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_020_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_020_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_020_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_020_iocg_020_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_021_UNION
 结构说明  : IOCG_021 寄存器结构定义。地址偏移量:0x854，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_DIR功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_021_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_021_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_021_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_021_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_021_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_021_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_021_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_021_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_021_iocg_021_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_022_UNION
 结构说明  : IOCG_022 寄存器结构定义。地址偏移量:0x858，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_NXT功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_022_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_022_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_022_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_022_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_022_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_022_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_022_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_022_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_022_iocg_022_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_023_UNION
 结构说明  : IOCG_023 寄存器结构定义。地址偏移量:0x85C，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_STP功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_023_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_023_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_023_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_023_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_023_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_023_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_023_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_023_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_023_iocg_023_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_024_UNION
 结构说明  : IOCG_024 寄存器结构定义。地址偏移量:0x860，初值:0x0000004A，宽度:32
 寄存器说明: USB2_ULPI_RST功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_024_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_024_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_024_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_024_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_024_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_024_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_024_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_024_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_024_iocg_024_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_025_UNION
 结构说明  : IOCG_025 寄存器结构定义。地址偏移量:0x864，初值:0x0000004A，宽度:32
 寄存器说明: ULPI_REF_CLK功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_025_pu  : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_025_pd  : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0   : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_025_st  : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  iocg_025_ds0 : 1;  /* bit[4]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_025_ds1 : 1;  /* bit[5]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_025_ds2 : 1;  /* bit[6]   : DS2,DS1,DS0配置驱动力。 */
        unsigned int  iocg_025_sr  : 1;  /* bit[7]   : Slew rate控制使能，0：正常模式，1：增加输出端信号上升下降时间。 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_025_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_pu_START   (0)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_pu_END     (0)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_pd_START   (1)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_pd_END     (1)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_st_START   (3)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_st_END     (3)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_ds0_START  (4)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_ds0_END    (4)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_ds1_START  (5)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_ds1_END    (5)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_ds2_START  (6)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_ds2_END    (6)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_sr_START   (7)
#define SOC_IOC_HSDT1_IOCG_025_iocg_025_sr_END     (7)


/*****************************************************************************
 结构名    : SOC_IOC_HSDT1_IOCG_026_UNION
 结构说明  : IOCG_026 寄存器结构定义。地址偏移量:0x868，初值:0x00000000，宽度:32
 寄存器说明: CLK_SYS_HILINK功能管脚控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  iocg_026_pu : 1;  /* bit[0]   : 输入上拉控制，高有效。 */
        unsigned int  iocg_026_pd : 1;  /* bit[1]   : 输入下拉控制，高有效。 */
        unsigned int  reserved_0  : 1;  /* bit[2]   : 保留。 */
        unsigned int  iocg_026_st : 1;  /* bit[3]   : 施密特控制，高有效。 */
        unsigned int  reserved_1  : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_IOC_HSDT1_IOCG_026_UNION;
#endif
#define SOC_IOC_HSDT1_IOCG_026_iocg_026_pu_START  (0)
#define SOC_IOC_HSDT1_IOCG_026_iocg_026_pu_END    (0)
#define SOC_IOC_HSDT1_IOCG_026_iocg_026_pd_START  (1)
#define SOC_IOC_HSDT1_IOCG_026_iocg_026_pd_END    (1)
#define SOC_IOC_HSDT1_IOCG_026_iocg_026_st_START  (3)
#define SOC_IOC_HSDT1_IOCG_026_iocg_026_st_END    (3)






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

#endif /* end of soc_ioc_hsdt1_interface.h */
