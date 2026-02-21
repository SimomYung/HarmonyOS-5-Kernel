/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_lpmcu_baseaddr_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_LPMCU_BASEADDR_INTERFACE_H__
#define __SOC_LPMCU_BASEADDR_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/


/* ==========================  M3core 内部访问  =========================== */
/* Size: 4K, End at: 0xE00FFFFF */
#define SOC_LPMCU_M3_CS_ROM_table_BASE_ADDR                     (0xE00FF000)

/* Size: 4K, End at: 0xE00FEFFF */
#define SOC_LPMCU_M3_CS_CTI_BASE_ADDR                           (0xE00FE000)

/* Size: 4K, End at: 0xE000EFFF */
#define SOC_LPMCU_M3_CS_SCS_BASE_ADDR                           (0xE000E000)

/* Size: 4K, End at: 0xE0002FFF */
#define SOC_LPMCU_M3_CS_FPB_BASE_ADDR                           (0xE0002000)

/* Size: 4K, End at: 0xE0001FFF */
#define SOC_LPMCU_M3_CS_DWT_BASE_ADDR                           (0xE0001000)


/* ===========================  CFG_SUBSYS 8MB  =========================== */
/* Size: 4K, End at: 0xBED3FFFF */
#define SOC_LPMCU_CODEC_SSI_BASE_ADDR                           (0xBED3F000)

/* Size: 64K, End at: 0xBED2FFFF */
#define SOC_LPMCU_IPC_NS_BASE_ADDR                              (0xBED20000)

/* Size: 64K, End at: 0xBED1FFFF */
#define SOC_LPMCU_IPC_BASE_ADDR                                 (0xBED10000)

/* Size: 4K, End at: 0xBED02FFF */
#define SOC_LPMCU_IOC_BASE_ADDR                                 (0xBED02000)

/* Size: 4K, End at: 0xBEC41FFF */
#define SOC_LPMCU_LoadMonitor1_BASE_ADDR                        (0xBEC41000)

/* Size: 4K, End at: 0xBEC40FFF */
#define SOC_LPMCU_LoadMonitor0_BASE_ADDR                        (0xBEC40000)

/* Size: 4K, End at: 0xBEC3EFFF */
#define SOC_LPMCU_PCTRL_BASE_ADDR                               (0xBEC3E000)

/* Size: 4K, End at: 0xBEC3DFFF */
#define SOC_LPMCU_ATGC_BASE_ADDR                                (0xBEC3D000)

/* Size: 4K, End at: 0xBEC3CFFF */
#define SOC_LPMCU_TZPC_BASE_ADDR                                (0xBEC3C000)

/* Size: 4K, End at: 0xBEC39FFF */
#define SOC_LPMCU_ATGC1_BASE_ADDR                               (0xBEC39000)

/* Size: 4K, End at: 0xBEC37FFF */
#define SOC_LPMCU_WD1_BASE_ADDR                                 (0xBEC37000)

/* Size: 4K, End at: 0xBEC36FFF */
#define SOC_LPMCU_WD0_BASE_ADDR                                 (0xBEC36000)

/* Size: 4K, End at: 0xBEC34FFF */
#define SOC_LPMCU_BLPWM_BASE_ADDR                               (0xBEC34000)

/* Size: 8K, End at: 0xBEC31FFF */
#define SOC_LPMCU_GPUPCR_BASE_ADDR                              (0xBEC30000)

/* Size: 4K, End at: 0xBEC2FFFF */
#define SOC_LPMCU_TIMER12_BASE_ADDR                             (0xBEC2F000)

/* Size: 4K, End at: 0xBEC2EFFF */
#define SOC_LPMCU_TIMER11_BASE_ADDR                             (0xBEC2E000)

/* Size: 4K, End at: 0xBEC2DFFF */
#define SOC_LPMCU_TIMER10_BASE_ADDR                             (0xBEC2D000)

/* Size: 4K, End at: 0xBEC2CFFF */
#define SOC_LPMCU_TIMER9_BASE_ADDR                              (0xBEC2C000)

/* Size: 4K, End at: 0xBEC25FFF */
#define SOC_LPMCU_GPIO19_BASE_ADDR                              (0xBEC25000)

/* Size: 4K, End at: 0xBEC24FFF */
#define SOC_LPMCU_GPIO18_BASE_ADDR                              (0xBEC24000)

/* Size: 4K, End at: 0xBEC23FFF */
#define SOC_LPMCU_GPIO17_BASE_ADDR                              (0xBEC23000)

/* Size: 4K, End at: 0xBEC22FFF */
#define SOC_LPMCU_GPIO16_BASE_ADDR                              (0xBEC22000)

/* Size: 4K, End at: 0xBEC21FFF */
#define SOC_LPMCU_GPIO15_BASE_ADDR                              (0xBEC21000)

/* Size: 4K, End at: 0xBEC20FFF */
#define SOC_LPMCU_GPIO14_BASE_ADDR                              (0xBEC20000)

/* Size: 4K, End at: 0xBEC1FFFF */
#define SOC_LPMCU_GPIO13_BASE_ADDR                              (0xBEC1F000)

/* Size: 4K, End at: 0xBEC1EFFF */
#define SOC_LPMCU_GPIO12_BASE_ADDR                              (0xBEC1E000)

/* Size: 4K, End at: 0xBEC1DFFF */
#define SOC_LPMCU_GPIO11_BASE_ADDR                              (0xBEC1D000)

/* Size: 4K, End at: 0xBEC1CFFF */
#define SOC_LPMCU_GPIO10_BASE_ADDR                              (0xBEC1C000)

/* Size: 4K, End at: 0xBEC1BFFF */
#define SOC_LPMCU_GPIO9_BASE_ADDR                               (0xBEC1B000)

/* Size: 4K, End at: 0xBEC1AFFF */
#define SOC_LPMCU_GPIO8_BASE_ADDR                               (0xBEC1A000)

/* Size: 4K, End at: 0xBEC19FFF */
#define SOC_LPMCU_GPIO7_BASE_ADDR                               (0xBEC19000)

/* Size: 4K, End at: 0xBEC18FFF */
#define SOC_LPMCU_GPIO6_BASE_ADDR                               (0xBEC18000)

/* Size: 4K, End at: 0xBEC17FFF */
#define SOC_LPMCU_GPIO5_BASE_ADDR                               (0xBEC17000)

/* Size: 4K, End at: 0xBEC16FFF */
#define SOC_LPMCU_GPIO4_BASE_ADDR                               (0xBEC16000)

/* Size: 4K, End at: 0xBEC15FFF */
#define SOC_LPMCU_GPIO3_BASE_ADDR                               (0xBEC15000)

/* Size: 4K, End at: 0xBEC14FFF */
#define SOC_LPMCU_GPIO2_BASE_ADDR                               (0xBEC14000)

/* Size: 4K, End at: 0xBEC13FFF */
#define SOC_LPMCU_GPIO1_BASE_ADDR                               (0xBEC13000)

/* Size: 4K, End at: 0xBEC12FFF */
#define SOC_LPMCU_GPIO0_BASE_ADDR                               (0xBEC12000)

/* Size: 4K, End at: 0xBEC10FFF */
#define SOC_LPMCU_GPIO0_SE_BASE_ADDR                            (0xBEC10000)

/* Size: 64K, End at: 0xBEC0FFFF */
#define SOC_LPMCU_fcm_service_target_BASE_ADDR                  (0xBEC00000)

/* Size: 4K, End at: 0xBEB07FFF */
#define SOC_LPMCU_FCM_GIC_HW_SPI1_BASE_ADDR                     (0xBEB07000)

/* Size: 4K, End at: 0xBEB05FFF */
#define SOC_LPMCU_FCM_LOCAL_GIC_BASE_ADDR                       (0xBEB05000)

/* Size: 4K, End at: 0xBEB04FFF */
#define SOC_LPMCU_FCM_GIC_HW_SPI0_BASE_ADDR                     (0xBEB04000)

/* Size: 4K, End at: 0xBEB03FFF */
#define SOC_LPMCU_FCM_LOCAL_PERI_BASE_ADDR                      (0xBEB03000)

/* Size: 4K, End at: 0xBEB02FFF */
#define SOC_LPMCU_FCM_SECURE_BASE_ADDR                          (0xBEB02000)

/* Size: 4K, End at: 0xBEA0CFFF */
#define SOC_LPMCU_FCM_LOCAL_IP_BASE_ADDR                        (0xBEA0C000)

/* Size: 4K, End at: 0xBEA0BFFF */
#define SOC_LPMCU_FCM_CORE7_LOCAL_CFG_BASE_ADDR                 (0xBEA0B000)

/* Size: 4K, End at: 0xBEA0AFFF */
#define SOC_LPMCU_FCM_CORE6_LOCAL_CFG_BASE_ADDR                 (0xBEA0A000)

/* Size: 4K, End at: 0xBEA09FFF */
#define SOC_LPMCU_FCM_CORE5_LOCAL_CFG_BASE_ADDR                 (0xBEA09000)

/* Size: 4K, End at: 0xBEA08FFF */
#define SOC_LPMCU_FCM_CORE4_LOCAL_CFG_BASE_ADDR                 (0xBEA08000)

/* Size: 4K, End at: 0xBEA07FFF */
#define SOC_LPMCU_FCM_CORE3_LOCAL_CFG_BASE_ADDR                 (0xBEA07000)

/* Size: 4K, End at: 0xBEA06FFF */
#define SOC_LPMCU_FCM_CORE2_LOCAL_CFG_BASE_ADDR                 (0xBEA06000)

/* Size: 4K, End at: 0xBEA05FFF */
#define SOC_LPMCU_FCM_CORE1_LOCAL_CFG_BASE_ADDR                 (0xBEA05000)

/* Size: 4K, End at: 0xBEA04FFF */
#define SOC_LPMCU_FCM_CORE0_LOCAL_CFG_BASE_ADDR                 (0xBEA04000)

/* Size: 4K, End at: 0xBEA03FFF */
#define SOC_LPMCU_PCR_MID_WRAP_BASE_ADDR                        (0xBEA03000)

/* Size: 4K, End at: 0xBEA02FFF */
#define SOC_LPMCU_PCR_BIG_WRAP_BASE_ADDR                        (0xBEA02000)

/* Size: 4K, End at: 0xBEA01FFF */
#define SOC_LPMCU_FCM_LOCAL_CPU_BASE_ADDR                       (0xBEA01000)

/* Size: 2M, End at: 0xBE9FFFFF */
#define SOC_LPMCU_GIC600_BASE_ADDR                              (0xBE800000)


/* ===============================  <N/A>  ================================ */
/* Size: 64K, End at: 0xBE3AFFFF */
#define SOC_LPMCU_MAA_BASE_ADDR                                 (0xBE3A0000)

/* Size: 64K, End at: 0xBE29FFFF */
#define SOC_LPMCU_MDM_5G_Service_Target_BASE_ADDR               (0xBE290000)

/* Size: 64K, End at: 0xBE28FFFF */
#define SOC_LPMCU_FD_BUS_Service_Target_BASE_ADDR               (0xBE280000)

/* Size: 64K, End at: 0xBE27FFFF */
#define SOC_LPMCU_AOBUS_Service_Target_BASE_ADDR                (0xBE270000)

/* Size: 64K, End at: 0xBE26FFFF */
#define SOC_LPMCU_DMA_NOC_Service_Target_BASE_ADDR              (0xBE260000)

/* Size: 64K, End at: 0xBE25FFFF */
#define SOC_LPMCU_CFGBUS_Service_Target_BASE_ADDR               (0xBE250000)

/* Size: 64K, End at: 0xBE24FFFF */
#define SOC_LPMCU_SYS_BUS_Service_Target_BASE_ADDR              (0xBE240000)

/* Size: 64K, End at: 0xBE23FFFF */
#define SOC_LPMCU_ASP_Service_Target_BASE_ADDR                  (0xBE230000)

/* Size: 64K, End at: 0xBE22FFFF */
#define SOC_LPMCU_Modem_Service_Target_BASE_ADDR                (0xBE220000)

/* Size: 64K, End at: 0xBE21FFFF */
#define SOC_LPMCU_HSDT_Service_Target_BASE_ADDR                 (0xBE210000)

/* Size: 64K, End at: 0xBE20FFFF */
#define SOC_LPMCU_MMC0_Service_target_BASE_ADDR                 (0xBE200000)

/* Size: 16M, End at: 0xBBFFFFFF */
#define SOC_LPMCU_G3D_BASE_ADDR                                 (0xBB000000)


/* =======================  AO_SUBSYS (7MB) PART1  ======================== */
/* Size: 4K, End at: 0xBABE0FFF */
#define SOC_LPMCU_UFS_SYS_CTRL_BASE_ADDR                        (0xBABE0000)

/* Size: 128K, End at: 0xBABDFFFF */
#define SOC_LPMCU_UFS_MCU_BASE_ADDR                             (0xBABC0000)

/* Size: 1536K, End at: 0xBAB7FFFF */
#define SOC_LPMCU_UFS_CFG_BASE_ADDR                             (0xBAA00000)

/* Size: 32K, End at: 0xBA987FFF */
#define SOC_LPMCU_AO_TCP_BASE_ADDR                              (0xBA980000)

/* Size: 4K, End at: 0xBA902FFF */
#define SOC_LPMCU_DSF_IF_BASE_ADDR                              (0xBA902000)

/* Size: 4K, End at: 0xBA901FFF */
#define SOC_LPMCU_MAD_Timer_BASE_ADDR                           (0xBA901000)

/* Size: 4K, End at: 0xBA900FFF */
#define SOC_LPMCU_MAD_REG_BASE_ADDR                             (0xBA900000)


/* =======================  AO_SUBSYS(4.5MB) PART2  ======================= */
/* Size: 4K, End at: 0xBA87FFFF */
#define SOC_LPMCU_IOMCU_RTC_BASE_ADDR                           (0xBA87F000)

/* Size: 4K, End at: 0xBA87EFFF */
#define SOC_LPMCU_IOMCU_CONFIG_BASE_ADDR                        (0xBA87E000)

/* Size: 4K, End at: 0xBA87DFFF */
#define SOC_LPMCU_IOMCU_TIMER_BASE_ADDR                         (0xBA87D000)

/* Size: 4K, End at: 0xBA87CFFF */
#define SOC_LPMCU_IOMCU_WDG_BASE_ADDR                           (0xBA87C000)

/* Size: 4K, End at: 0xBA87BFFF */
#define SOC_LPMCU_IOMCU_GPIO3_BASE_ADDR                         (0xBA87B000)

/* Size: 4K, End at: 0xBA87AFFF */
#define SOC_LPMCU_IOMCU_GPIO2_BASE_ADDR                         (0xBA87A000)

/* Size: 4K, End at: 0xBA879FFF */
#define SOC_LPMCU_IOMCU_GPIO1_BASE_ADDR                         (0xBA879000)

/* Size: 4K, End at: 0xBA878FFF */
#define SOC_LPMCU_IOMCU_GPIO0_BASE_ADDR                         (0xBA878000)

/* Size: 4K, End at: 0xBA877FFF */
#define SOC_LPMCU_IOMCU_DMAC_BASE_ADDR                          (0xBA877000)

/* Size: 4K, End at: 0xBA876FFF */
#define SOC_LPMCU_IOMCU_UART7_BASE_ADDR                         (0xBA876000)

/* Size: 4K, End at: 0xBA875FFF */
#define SOC_LPMCU_IOMCU_BLPWM_BASE_ADDR                         (0xBA875000)

/* Size: 4K, End at: 0xBA874FFF */
#define SOC_LPMCU_IOMCU_UART3_BASE_ADDR                         (0xBA874000)

/* Size: 4K, End at: 0xBA873FFF */
#define SOC_LPMCU_IOMCU_I3C1_BASE_ADDR                          (0xBA873000)

/* Size: 4K, End at: 0xBA872FFF */
#define SOC_LPMCU_IOMCU_I2C1_BASE_ADDR                          (0xBA872000)

/* Size: 4K, End at: 0xBA871FFF */
#define SOC_LPMCU_IOMCU_I2C0_BASE_ADDR                          (0xBA871000)

/* Size: 4K, End at: 0xBA870FFF */
#define SOC_LPMCU_IOMCU_SPI0_BASE_ADDR                          (0xBA870000)

/* Size: 4K, End at: 0xBA86FFFF */
#define SOC_LPMCU_IOMCU_DTCM1Remap_BASE_ADDR                    (0xBA86F000)

/* Size: 4K, End at: 0xBA86EFFF */
#define SOC_LPMCU_IOMCU_DTCM0Remap_BASE_ADDR                    (0xBA86E000)

/* Size: 8K, End at: 0xBA86DFFF */
#define SOC_LPMCU_IOMCU_ITCMRemap_BASE_ADDR                     (0xBA86C000)

/* Size: 4K, End at: 0xBA86BFFF */
#define SOC_LPMCU_IOMCU_Remapctrl_BASE_ADDR                     (0xBA86B000)

/* Size: 4K, End at: 0xBA86AFFF */
#define SOC_LPMCU_IOMCU_I3C2_BASE_ADDR                          (0xBA86A000)

/* Size: 4K, End at: 0xBA869FFF */
#define SOC_LPMCU_IOMCU_UART8_BASE_ADDR                         (0xBA869000)

/* Size: 4K, End at: 0xBA868FFF */
#define SOC_LPMCU_IOMCU_SPI2_BASE_ADDR                          (0xBA868000)

/* Size: 4K, End at: 0xBA867FFF */
#define SOC_LPMCU_IOMCU_DMMU_BASE_ADDR                          (0xBA867000)

/* Size: 4K, End at: 0xBA866FFF */
#define SOC_LPMCU_IOMCU_TIMER2_BASE_ADDR                        (0xBA866000)

/* Size: 4K, End at: 0xBA865FFF */
#define SOC_LPMCU_IOMCU_I3C_BASE_ADDR                           (0xBA865000)

/* Size: 4K, End at: 0xBA864FFF */
#define SOC_LPMCU_IOMCU_I3C3_BASE_ADDR                          (0xBA864000)

/* Size: 4K, End at: 0xBA863FFF */
#define SOC_LPMCU_IOMCU_I2C5_BASE_ADDR                          (0xBA863000)

/* Size: 1M, End at: 0xBA7FFFFF */
#define SOC_LPMCU_IOMCU_DTCM_BASE_ADDR                          (0xBA700000)

/* Size: 1M, End at: 0xBA6FFFFF */
#define SOC_LPMCU_IOMCU_ITCM_BASE_ADDR                          (0xBA600000)

/* Size: 48K, End at: 0xBA58BFFF */
#define SOC_LPMCU_DSP_ITCM_BASE_ADDR                            (0xBA580000)

/* Size: 160K, End at: 0xBA57FFFF */
#define SOC_LPMCU_DSP_DTCM_BASE_ADDR                            (0xBA558000)

/* Size: 8K, End at: 0xBA553FFF */
#define SOC_LPMCU_ASP_CODEC_BASE_ADDR                           (0xBA552000)

/* Size: 8K, End at: 0xBA551FFF */
#define SOC_LPMCU_SLIMBUS_BASE_ADDR                             (0xBA550000)

/* Size: 1K, End at: 0xBA54FFFF */
#define SOC_LPMCU_DSD_BASE_ADDR                                 (0xBA54FC00)

/* Size: 1K, End at: 0xBA54E3FF */
#define SOC_LPMCU_ASP_CFG_BASE_ADDR                             (0xBA54E000)

/* Size: 4K, End at: 0xBA54DFFF */
#define SOC_LPMCU_ASP_WD_BASE_ADDR                              (0xBA54D000)

/* Size: 4K, End at: 0xBA54CFFF */
#define SOC_LPMCU_ASP_IPC_BASE_ADDR                             (0xBA54C000)

/* Size: 4K, End at: 0xBA54BFFF */
#define SOC_LPMCU_ASP_DMAC_BASE_ADDR                            (0xBA54B000)

/* Size: 4K, End at: 0xBA54AFFF */
#define SOC_LPMCU_ASP_TIMER1_BASE_ADDR                          (0xBA54A000)

/* Size: 4K, End at: 0xBA549FFF */
#define SOC_LPMCU_ASP_TIMER0_BASE_ADDR                          (0xBA549000)

/* Size: 4K, End at: 0xBA548FFF */
#define SOC_LPMCU_ASP_GPIO_BASE_ADDR                            (0xBA548000)

/* Size: 4K, End at: 0xBA547FFF */
#define SOC_LPMCU_ASP_DMMU_BASE_ADDR                            (0xBA547000)

/* Size: 256K, End at: 0xBA4BFFFF */
#define SOC_LPMCU_SECRAM_BASE_ADDR                              (0xBA480000)


/* =======================  DMA_PERI_SUBSYS(17MB)  ======================== */
/* Size: 6K, End at: 0xBA0A17FF */
#define SOC_LPMCU_SOCP_BASE_ADDR                                (0xBA0A0000)

/* Size: 32K, End at: 0xBA097FFF */
#define SOC_LPMCU_EICC_BASE_ADDR                                (0xBA090000)

/* Size: 32K, End at: 0xBA08FFFF */
#define SOC_LPMCU_IPF_BASE_ADDR                                 (0xBA088000)

/* Size: 16K, End at: 0xBA087FFF */
#define SOC_LPMCU_SPE_BASE_ADDR                                 (0xBA084000)

/* Size: 4K, End at: 0xBA081FFF */
#define SOC_LPMCU_IPC_MDM_NS_BASE_ADDR                          (0xBA081000)

/* Size: 4K, End at: 0xBA080FFF */
#define SOC_LPMCU_IPC_MDM_S_BASE_ADDR                           (0xBA080000)

/* Size: 4K, End at: 0xBA057FFF */
#define SOC_LPMCU_LAT_STAT_BASE_ADDR                            (0xBA057000)

/* Size: 4K, End at: 0xBA056FFF */
#define SOC_LPMCU_PERF_STAT_BASE_ADDR                           (0xBA056000)

/* Size: 4K, End at: 0xBA054FFF */
#define SOC_LPMCU_UART0_BASE_ADDR                               (0xBA054000)

/* Size: 4K, End at: 0xBA050FFF */
#define SOC_LPMCU_I3C4_BASE_ADDR                                (0xBA050000)

/* Size: 4K, End at: 0xBA04FFFF */
#define SOC_LPMCU_I2C7_BASE_ADDR                                (0xBA04F000)

/* Size: 4K, End at: 0xBA04EFFF */
#define SOC_LPMCU_I2C6_BASE_ADDR                                (0xBA04E000)

/* Size: 4K, End at: 0xBA04DFFF */
#define SOC_LPMCU_I2C4_BASE_ADDR                                (0xBA04D000)

/* Size: 4K, End at: 0xBA04CFFF */
#define SOC_LPMCU_I2C3_BASE_ADDR                                (0xBA04C000)

/* Size: 4K, End at: 0xBA04BFFF */
#define SOC_LPMCU_I2C9_BASE_ADDR                                (0xBA04B000)

/* Size: 4K, End at: 0xBA049FFF */
#define SOC_LPMCU_SPI4_BASE_ADDR                                (0xBA049000)

/* Size: 4K, End at: 0xBA048FFF */
#define SOC_LPMCU_SPI1_BASE_ADDR                                (0xBA048000)

/* Size: 4K, End at: 0xBA045FFF */
#define SOC_LPMCU_UART5_BASE_ADDR                               (0xBA045000)

/* Size: 4K, End at: 0xBA044FFF */
#define SOC_LPMCU_UART2_BASE_ADDR                               (0xBA044000)

/* Size: 4K, End at: 0xBA041FFF */
#define SOC_LPMCU_UART4_BASE_ADDR                               (0xBA041000)

/* Size: 4K, End at: 0xBA040FFF */
#define SOC_LPMCU_UART1_BASE_ADDR                               (0xBA040000)

/* Size: 4K, End at: 0xBA000FFF */
#define SOC_LPMCU_PERI_DMAC_BASE_ADDR                           (0xBA000000)


/* =========================  MMC0_SUBSYS (2MB)  ========================== */
/* Size: 1K, End at: 0xB85873FF */
#define SOC_LPMCU_USB20PHY_BASE_ADDR                            (0xB8587000)

/* Size: 4K, End at: 0xB8586FFF */
#define SOC_LPMCU_USB20_SYSCTRL_BASE_ADDR                       (0xB8586000)

/* Size: 4K, End at: 0xB8585FFF */
#define SOC_LPMCU_PERI_IOCG_R_BASE_ADDR                         (0xB8585000)

/* Size: 4K, End at: 0xB8584FFF */
#define SOC_LPMCU_PERI_IOCG_L_BASE_ADDR                         (0xB8584000)

/* Size: 4K, End at: 0xB8583FFF */
#define SOC_LPMCU_SD3_BASE_ADDR                                 (0xB8583000)

/* Size: 4K, End at: 0xB8582FFF */
#define SOC_LPMCU_MMC0_SYS_CTRL_BASE_ADDR                       (0xB8582000)

/* Size: 4K, End at: 0xB8581FFF */
#define SOC_LPMCU_MMC0_CRG_BASE_ADDR                            (0xB8581000)

/* Size: 4K, End at: 0xB8580FFF */
#define SOC_LPMCU_IOC_MMC0_BASE_ADDR                            (0xB8580000)

/* Size: 512K, End at: 0xB847FFFF */
#define SOC_LPMCU_USB_OTG_BASE_ADDR                             (0xB8400000)


/* =====================  HSDT_SUBSYS (98MB) PART 1   ===================== */
/* Size: 128K, End at: 0xB81FFFFF */
#define SOC_LPMCU_HSDT_TBU_BASE_ADDR                            (0xB81E0000)

/* Size: 384K, End at: 0xB81DFFFF */
#define SOC_LPMCU_HSDT_TCU_BASE_ADDR                            (0xB8180000)

/* Size: 4K, End at: 0xB817FFFF */
#define SOC_LPMCU_HSDT_SYS_CTRL_BASE_ADDR                       (0xB817F000)

/* Size: 4K, End at: 0xB817EFFF */
#define SOC_LPMCU_HSDT_CRG_BASE_ADDR                            (0xB817E000)

/* Size: 4K, End at: 0xB817DFFF */
#define SOC_LPMCU_PCIE_PLL_BASE_ADDR                            (0xB817D000)

/* Size: 4K, End at: 0xB8105FFF */
#define SOC_LPMCU_MMC1_IOC_BASE_ADDR                            (0xB8105000)

/* Size: 4K, End at: 0xB8104FFF */
#define SOC_LPMCU_MMC1_SDIO_BASE_ADDR                           (0xB8104000)

/* Size: 4K, End at: 0xB8102FFF */
#define SOC_LPMCU_PCIE0_APB_CFG_BASE_ADDR                       (0xB8102000)

/* Size: 4K, End at: 0xB8101FFF */
#define SOC_LPMCU_CCP_BASE_ADDR                                 (0xB8101000)

/* Size: 4K, End at: 0xB8100FFF */
#define SOC_LPMCU_CCS_BASE_ADDR                                 (0xB8100000)

/* Size: 64K, End at: 0xB80FFFFF */
#define SOC_LPMCU_PCIE_MCU_BASE_ADDR                            (0xB80F0000)

/* Size: 512K, End at: 0xB807FFFF */
#define SOC_LPMCU_PCIEPHY0_BASE_ADDR                            (0xB8000000)


/* ===========================  MDM_5G(128MB)  ============================ */
/* Size: 64K, End at: 0xB604FFFF */
#define SOC_LPMCU_TSP_DSS1_L1TCM_GLOBAL_BASE_ADDR               (0xB6040000)

/* Size: 64K, End at: 0xB600FFFF */
#define SOC_LPMCU_TSP_DSS0_L1TCM_GLOBAL_BASE_ADDR               (0xB6000000)

/* Size: 64K, End at: 0xB5F0FFFF */
#define SOC_LPMCU_TSP_L1TCM_LOCAL_BASE_ADDR                     (0xB5F00000)

/* Size: 2M, End at: 0xB59FFFFF */
#define SOC_LPMCU_TSP_REG_BASE_ADDR                             (0xB5800000)

/* Size: 4352K, End at: 0xB543FFFF */
#define SOC_LPMCU_TSP_L2M_BASE_ADDR                             (0xB5000000)

/* Size: 64K, End at: 0xB4D0FFFF */
#define SOC_LPMCU_NOC_MDMBUS_SERVICE_TARGET_BASE_ADDR           (0xB4D00000)

/* Size: 4K, End at: 0xB4C00FFF */
#define SOC_LPMCU_MDM_AMON_BASE_ADDR                            (0xB4C00000)

/* Size: 4K, End at: 0xB4B5BFFF */
#define SOC_LPMCU_MDM_IPCM3_BASE_ADDR                           (0xB4B5B000)

/* Size: 4K, End at: 0xB4B5AFFF */
#define SOC_LPMCU_MDM_IPCM2_BASE_ADDR                           (0xB4B5A000)

/* Size: 4K, End at: 0xB4B59FFF */
#define SOC_LPMCU_MDM_IPCM1_BASE_ADDR                           (0xB4B59000)

/* Size: 4K, End at: 0xB4B58FFF */
#define SOC_LPMCU_MDM_IPCM0_BASE_ADDR                           (0xB4B58000)

/* Size: 4K, End at: 0xB4B57FFF */
#define SOC_LPMCU_CICOM_BASE_ADDR                               (0xB4B57000)

/* Size: 4K, End at: 0xB4B54FFF */
#define SOC_LPMCU_UPACC_BASE_ADDR                               (0xB4B54000)

/* Size: 16K, End at: 0xB4B53FFF */
#define SOC_LPMCU_CIPHER_BASE_ADDR                              (0xB4B50000)

/* Size: 64K, End at: 0xB4B4FFFF */
#define SOC_LPMCU_L2DLE_BASE_ADDR                               (0xB4B40000)

/* Size: 32K, End at: 0xB4B0FFFF */
#define SOC_LPMCU_MDM_EICC1_BASE_ADDR                           (0xB4B08000)

/* Size: 32K, End at: 0xB4B07FFF */
#define SOC_LPMCU_MDM_EICC0_BASE_ADDR                           (0xB4B00000)

/* Size: 8K, End at: 0xB4A3BFFF */
#define SOC_LPMCU_MDM_SYSCNT_BASE_ADDR                          (0xB4A3A000)

/* Size: 4K, End at: 0xB4A39FFF */
#define SOC_LPMCU_MDM_EDMA1_BASE_ADDR                           (0xB4A39000)

/* Size: 4K, End at: 0xB4A38FFF */
#define SOC_LPMCU_MDM_EDMA0_BASE_ADDR                           (0xB4A38000)

/* Size: 4K, End at: 0xB4A33FFF */
#define SOC_LPMCU_LATENCYM_BASE_ADDR                            (0xB4A33000)

/* Size: 4K, End at: 0xB4A32FFF */
#define SOC_LPMCU_RRT_BASE_ADDR                                 (0xB4A32000)

/* Size: 4K, End at: 0xB4A2DFFF */
#define SOC_LPMCU_MDM_LOAD_MON1_BASE_ADDR                       (0xB4A2D000)

/* Size: 4K, End at: 0xB4A2CFFF */
#define SOC_LPMCU_MDM_LOAD_MON0_BASE_ADDR                       (0xB4A2C000)

/* Size: 4K, End at: 0xB4A2AFFF */
#define SOC_LPMCU_MDM_RTC_BASE_ADDR                             (0xB4A2A000)

/* Size: 4K, End at: 0xB4A29FFF */
#define SOC_LPMCU_MDM_PM_CTRL0_BASE_ADDR                        (0xB4A29000)

/* Size: 4K, End at: 0xB4A28FFF */
#define SOC_LPMCU_MDM_PM_CTRL1_BASE_ADDR                        (0xB4A28000)

/* Size: 4K, End at: 0xB4A27FFF */
#define SOC_LPMCU_MDM_UART1_BASE_ADDR                           (0xB4A27000)

/* Size: 4K, End at: 0xB4A26FFF */
#define SOC_LPMCU_MDM_UART0_BASE_ADDR                           (0xB4A26000)

/* Size: 4K, End at: 0xB4A24FFF */
#define SOC_LPMCU_MDM_TIMER15_BASE_ADDR                         (0xB4A24000)

/* Size: 4K, End at: 0xB4A23FFF */
#define SOC_LPMCU_MDM_TIMER14_BASE_ADDR                         (0xB4A23000)

/* Size: 4K, End at: 0xB4A22FFF */
#define SOC_LPMCU_MDM_TIMER13_BASE_ADDR                         (0xB4A22000)

/* Size: 4K, End at: 0xB4A21FFF */
#define SOC_LPMCU_MDM_TIMER12_BASE_ADDR                         (0xB4A21000)

/* Size: 4K, End at: 0xB4A20FFF */
#define SOC_LPMCU_MDM_TIMER11_BASE_ADDR                         (0xB4A20000)

/* Size: 4K, End at: 0xB4A1FFFF */
#define SOC_LPMCU_MDM_TIMER10_BASE_ADDR                         (0xB4A1F000)

/* Size: 4K, End at: 0xB4A1EFFF */
#define SOC_LPMCU_MDM_TIMER9_BASE_ADDR                          (0xB4A1E000)

/* Size: 4K, End at: 0xB4A1DFFF */
#define SOC_LPMCU_MDM_TIMER8_BASE_ADDR                          (0xB4A1D000)

/* Size: 4K, End at: 0xB4A1CFFF */
#define SOC_LPMCU_MDM_TIMER7_BASE_ADDR                          (0xB4A1C000)

/* Size: 4K, End at: 0xB4A1BFFF */
#define SOC_LPMCU_MDM_TIMER6_BASE_ADDR                          (0xB4A1B000)

/* Size: 4K, End at: 0xB4A1AFFF */
#define SOC_LPMCU_MDM_TIMER5_BASE_ADDR                          (0xB4A1A000)

/* Size: 4K, End at: 0xB4A19FFF */
#define SOC_LPMCU_MDM_TIMER4_BASE_ADDR                          (0xB4A19000)

/* Size: 4K, End at: 0xB4A18FFF */
#define SOC_LPMCU_MDM_TIMER3_BASE_ADDR                          (0xB4A18000)

/* Size: 4K, End at: 0xB4A17FFF */
#define SOC_LPMCU_MDM_TIMER2_BASE_ADDR                          (0xB4A17000)

/* Size: 4K, End at: 0xB4A16FFF */
#define SOC_LPMCU_MDM_TIMER1_BASE_ADDR                          (0xB4A16000)

/* Size: 4K, End at: 0xB4A15FFF */
#define SOC_LPMCU_MDM_TIMER0_BASE_ADDR                          (0xB4A15000)

/* Size: 4K, End at: 0xB4A13FFF */
#define SOC_LPMCU_MDM_WDT15_BASE_ADDR                           (0xB4A13000)

/* Size: 4K, End at: 0xB4A12FFF */
#define SOC_LPMCU_MDM_WDT14_BASE_ADDR                           (0xB4A12000)

/* Size: 4K, End at: 0xB4A11FFF */
#define SOC_LPMCU_MDM_WDT13_BASE_ADDR                           (0xB4A11000)

/* Size: 4K, End at: 0xB4A10FFF */
#define SOC_LPMCU_MDM_WDT12_BASE_ADDR                           (0xB4A10000)

/* Size: 4K, End at: 0xB4A0FFFF */
#define SOC_LPMCU_MDM_WDT11_BASE_ADDR                           (0xB4A0F000)

/* Size: 4K, End at: 0xB4A0EFFF */
#define SOC_LPMCU_MDM_WDT10_BASE_ADDR                           (0xB4A0E000)

/* Size: 4K, End at: 0xB4A0DFFF */
#define SOC_LPMCU_MDM_WDT9_BASE_ADDR                            (0xB4A0D000)

/* Size: 4K, End at: 0xB4A0CFFF */
#define SOC_LPMCU_MDM_WDT8_BASE_ADDR                            (0xB4A0C000)

/* Size: 4K, End at: 0xB4A0BFFF */
#define SOC_LPMCU_MDM_WDT7_BASE_ADDR                            (0xB4A0B000)

/* Size: 4K, End at: 0xB4A0AFFF */
#define SOC_LPMCU_MDM_WDT6_BASE_ADDR                            (0xB4A0A000)

/* Size: 4K, End at: 0xB4A09FFF */
#define SOC_LPMCU_MDM_WDT5_BASE_ADDR                            (0xB4A09000)

/* Size: 4K, End at: 0xB4A08FFF */
#define SOC_LPMCU_MDM_WDT4_BASE_ADDR                            (0xB4A08000)

/* Size: 4K, End at: 0xB4A07FFF */
#define SOC_LPMCU_MDM_WDT3_BASE_ADDR                            (0xB4A07000)

/* Size: 4K, End at: 0xB4A06FFF */
#define SOC_LPMCU_MDM_WDT2_BASE_ADDR                            (0xB4A06000)

/* Size: 4K, End at: 0xB4A05FFF */
#define SOC_LPMCU_MDM_WDT1_BASE_ADDR                            (0xB4A05000)

/* Size: 4K, End at: 0xB4A04FFF */
#define SOC_LPMCU_MDM_WDT0_BASE_ADDR                            (0xB4A04000)

/* Size: 4K, End at: 0xB4A02FFF */
#define SOC_LPMCU_MDM_AUTO_DFS_BASE_ADDR                        (0xB4A02000)

/* Size: 4K, End at: 0xB4A01FFF */
#define SOC_LPMCU_SYS_MDM_BASE_ADDR                             (0xB4A01000)

/* Size: 4K, End at: 0xB4A00FFF */
#define SOC_LPMCU_CRG_MDM_BASE_ADDR                             (0xB4A00000)

/* Size: 4K, End at: 0xB42D0FFF */
#define SOC_LPMCU_SYS_BBP_APB_BASE_ADDR                         (0xB42D0000)

/* Size: 64K, End at: 0xB428FFFF */
#define SOC_LPMCU_NOC_BBPBUS_SERVICE_TARGET_BASE_ADDR           (0xB4280000)

/* Size: 64M, End at: 0xB3FFFFFF */
#define SOC_LPMCU_BBP_BASE_ADDR                                 (0xB0000000)


/* =====================  HSDT_SUBSYS (100MB) PART 2  ===================== */
/* Size: 4M, End at: 0xAC3FFFFF */
#define SOC_LPMCU_PCIECtrl0_DBI_BASE_ADDR                       (0xAC000000)

/* Size: 32M, End at: 0xABFFFFFF */
#define SOC_LPMCU_PCIECtrl0_BASE_ADDR                           (0xAA000000)


/* =========================  MEDIA2_SUBSYS(8MB)  ========================= */
/* Size: 1M, End at: 0xA95FFFFF */
#define SOC_LPMCU_HIEPS_BASE_ADDR                               (0xA9500000)

/* Size: 64K, End at: 0xA93DFFFF */
#define SOC_LPMCU_NOC_EPS_Service_Target_BASE_ADDR              (0xA93D0000)

/* Size: 64K, End at: 0xA93BFFFF */
#define SOC_LPMCU_NOC_VENC_Service_Target_BASE_ADDR             (0xA93B0000)

/* Size: 64K, End at: 0xA93AFFFF */
#define SOC_LPMCU_NOC_VDEC_Service_Target_BASE_ADDR             (0xA93A0000)

/* Size: 64K, End at: 0xA939FFFF */
#define SOC_LPMCU_NOC_VCODECBUS_Service_Target_BASE_ADDR        (0xA9390000)

/* Size: 64K, End at: 0xA938FFFF */
#define SOC_LPMCU_NOC_IVP32_Service_Target_BASE_ADDR            (0xA9380000)

/* Size: 4K, End at: 0xA9301FFF */
#define SOC_LPMCU_MEDAI2_LoadMonitor_BASE_ADDR                  (0xA9301000)

/* Size: 4K, End at: 0xA9300FFF */
#define SOC_LPMCU_MEDIA2_CRG_BASE_ADDR                          (0xA9300000)

/* Size: 256K, End at: 0xA92BFFFF */
#define SOC_LPMCU_VENC_BASE_ADDR                                (0xA9280000)

/* Size: 256K, End at: 0xA923FFFF */
#define SOC_LPMCU_VDEC_BASE_ADDR                                (0xA9200000)

/* Size: 64K, End at: 0xA91CFFFF */
#define SOC_LPMCU_IVP32_SMMU_BASE_ADDR                          (0xA91C0000)

/* Size: 4K, End at: 0xA9183FFF */
#define SOC_LPMCU_IVP32_TIMER1_BASE_ADDR                        (0xA9183000)

/* Size: 4K, End at: 0xA9182FFF */
#define SOC_LPMCU_IVP32_TIMER0_BASE_ADDR                        (0xA9182000)

/* Size: 4K, End at: 0xA9181FFF */
#define SOC_LPMCU_IVP32_WDG_BASE_ADDR                           (0xA9181000)

/* Size: 4K, End at: 0xA9180FFF */
#define SOC_LPMCU_IVP32_CFG_BASE_ADDR                           (0xA9180000)

/* Size: 32K, End at: 0xA9107FFF */
#define SOC_LPMCU_IVP32_IRAM_BASE_ADDR                          (0xA9100000)

/* Size: 144K, End at: 0xA9063FFF */
#define SOC_LPMCU_IVP32_DRAM1_BASE_ADDR                         (0xA9040000)

/* Size: 144K, End at: 0xA9023FFF */
#define SOC_LPMCU_IVP32_DRAM0_BASE_ADDR                         (0xA9000000)


/* ============================  MEDIA1(16MB)  ============================ */
/* Size: 64K, End at: 0xA86AFFFF */
#define SOC_LPMCU_NOC_VIVO_Service_Target_BASE_ADDR             (0xA86A0000)

/* Size: 64K, End at: 0xA869FFFF */
#define SOC_LPMCU_NOC_ISP_Service_Target_BASE_ADDR              (0xA8690000)

/* Size: 64K, End at: 0xA868FFFF */
#define SOC_LPMCU_NOC_DSS_Service_Target_BASE_ADDR              (0xA8680000)

/* Size: 4K, End at: 0xA8608FFF */
#define SOC_LPMCU_CSI_E_BASE_ADDR                               (0xA8608000)

/* Size: 4K, End at: 0xA8607FFF */
#define SOC_LPMCU_CSI_D_BASE_ADDR                               (0xA8607000)

/* Size: 4K, End at: 0xA8606FFF */
#define SOC_LPMCU_CSI_C_BASE_ADDR                               (0xA8606000)

/* Size: 4K, End at: 0xA8605FFF */
#define SOC_LPMCU_CSI_B_BASE_ADDR                               (0xA8605000)

/* Size: 4K, End at: 0xA8604FFF */
#define SOC_LPMCU_CSI_A_BASE_ADDR                               (0xA8604000)

/* Size: 4K, End at: 0xA8603FFF */
#define SOC_LPMCU_CSI_adapter_BASE_ADDR                         (0xA8603000)

/* Size: 4K, End at: 0xA8601FFF */
#define SOC_LPMCU_MEDIA1_CRG_BASE_ADDR                          (0xA8601000)

/* Size: 4K, End at: 0xA8600FFF */
#define SOC_LPMCU_MEDIA1_LoadMonitor_BASE_ADDR                  (0xA8600000)

/* Size: 40K, End at: 0xA84F7FFF */
#define SOC_LPMCU_GMP_LUT1_BASE_ADDR                            (0xA84EE000)

/* Size: 40K, End at: 0xA84E7FFF */
#define SOC_LPMCU_GMP_LUT0_BASE_ADDR                            (0xA84DE000)

/* Size: 4K, End at: 0xA84BEFFF */
#define SOC_LPMCU_CROSSBAR_BASE_ADDR                            (0xA84BE000)

/* Size: 112K, End at: 0xA84BDFFF */
#define SOC_LPMCU_DISP_CH1_BASE_ADDR                            (0xA84A2000)

/* Size: 4K, End at: 0xA84A1FFF */
#define SOC_LPMCU_DISP_GLB_BASE_ADDR                            (0xA84A1000)

/* Size: 4K, End at: 0xA84A0FFF */
#define SOC_LPMCU_WB_BASE_ADDR                                  (0xA84A0000)

/* Size: 64K, End at: 0xA849FFFF */
#define SOC_LPMCU_DSS_PRO_BASE_ADDR                             (0xA8490000)

/* Size: 64K, End at: 0xA848FFFF */
#define SOC_LPMCU_DBCU0_SMMU_BASE_ADDR                          (0xA8480000)

/* Size: 112K, End at: 0xA847DFFF */
#define SOC_LPMCU_DISP_CH0_BASE_ADDR                            (0xA8462000)

/* Size: 1K, End at: 0xA8460BFF */
#define SOC_LPMCU_OV2_BASE_ADDR                                 (0xA8460800)

/* Size: 1K, End at: 0xA84603FF */
#define SOC_LPMCU_OV0_BASE_ADDR                                 (0xA8460000)

/* Size: 16K, End at: 0xA845FFFF */
#define SOC_LPMCU_WCH1_BASE_ADDR                                (0xA845C000)

/* Size: 4K, End at: 0xA8452FFF */
#define SOC_LPMCU_RCH_D0_BASE_ADDR                              (0xA8452000)

/* Size: 4K, End at: 0xA8451FFF */
#define SOC_LPMCU_RCH_D3_BASE_ADDR                              (0xA8451000)

/* Size: 4K, End at: 0xA8450FFF */
#define SOC_LPMCU_RCH_D2_BASE_ADDR                              (0xA8450000)

/* Size: 32K, End at: 0xA843FFFF */
#define SOC_LPMCU_RCH_G0_BASE_ADDR                              (0xA8438000)

/* Size: 32K, End at: 0xA842FFFF */
#define SOC_LPMCU_RCH_V1_BASE_ADDR                              (0xA8428000)

/* Size: 32K, End at: 0xA8427FFF */
#define SOC_LPMCU_RCH_V0_BASE_ADDR                              (0xA8420000)

/* Size: 4K, End at: 0xA8412FFF */
#define SOC_LPMCU_GLB_BASE_ADDR                                 (0xA8412000)

/* Size: 4K, End at: 0xA8411FFF */
#define SOC_LPMCU_DBUG_BASE_ADDR                                (0xA8411000)

/* Size: 2K, End at: 0xA8410FFF */
#define SOC_LPMCU_MCTL_MUTEX_BASE_ADDR                          (0xA8410800)

/* Size: 2K, End at: 0xA84107FF */
#define SOC_LPMCU_MCTL_SYS_BASE_ADDR                            (0xA8410000)

/* Size: 4K, End at: 0xA840AFFF */
#define SOC_LPMCU_DBCU0_MIF_BASE_ADDR                           (0xA840A000)

/* Size: 4K, End at: 0xA8409FFF */
#define SOC_LPMCU_DBCU1_AIF_BASE_ADDR                           (0xA8409000)

/* Size: 4K, End at: 0xA8407FFF */
#define SOC_LPMCU_DBCU0_AIF_BASE_ADDR                           (0xA8407000)

/* Size: 5K, End at: 0xA84033FF */
#define SOC_LPMCU_CMD_BASE_ADDR                                 (0xA8402000)

/* Size: 2K, End at: 0xA8401FFF */
#define SOC_LPMCU_ASC_BASE_ADDR                                 (0xA8401800)

/* Size: 1K, End at: 0xA84013FF */
#define SOC_LPMCU_DSI0_BASE_ADDR                                (0xA8401000)

/* Size: 4K, End at: 0xA8400FFF */
#define SOC_LPMCU_MMBUF_CFG_BASE_ADDR                           (0xA8400000)

/* Size: 64K, End at: 0xA83FFFFF */
#define SOC_LPMCU_SMMUv500_CFG_BASE_ADDR                        (0xA83F0000)

/* Size: 8K, End at: 0xA83EFFFF */
#define SOC_LPMCU_ISP_R8_PERI_BASE_BASE_ADDR                    (0xA83EE000)

/* Size: 4K, End at: 0xA83E4FFF */
#define SOC_LPMCU_ISP_TCMDMA_BASE_ADDR                          (0xA83E4000)

/* Size: 4K, End at: 0xA83E3FFF */
#define SOC_LPMCU_ISP_SUB_CTRL_BASE_ADDR                        (0xA83E3000)

/* Size: 4K, End at: 0xA83E2FFF */
#define SOC_LPMCU_ISP_IPC_BASE_ADDR                             (0xA83E2000)

/* Size: 4K, End at: 0xA83E1FFF */
#define SOC_LPMCU_ISP_TIMER_BASE_ADDR                           (0xA83E1000)

/* Size: 4K, End at: 0xA83E0FFF */
#define SOC_LPMCU_ISP_WDT_BASE_ADDR                             (0xA83E0000)

/* Size: 64K, End at: 0xA83DFFFF */
#define SOC_LPMCU_ISP_R8_DTCM_BASE_ADDR                         (0xA83D0000)

/* Size: 32K, End at: 0xA83C7FFF */
#define SOC_LPMCU_ISP_R8_ITCM_BASE_ADDR                         (0xA83C0000)

/* Size: 1.75M, End at: 0xA83BFFFF */
#define SOC_LPMCU_ISP_Core_CFG_BASE_ADDR                        (0xA8200000)

/* Size: 128K, End at: 0xA803FFFF */
#define SOC_LPMCU_SMMU_BASE_ADDR                                (0xA8020000)

/* Size: 4K, End at: 0xA800CFFF */
#define SOC_LPMCU_ORB_BASE_ADDR                                 (0xA800C000)

/* Size: 8K, End at: 0xA8007FFF */
#define SOC_LPMCU_CVDR_BASE_ADDR                                (0xA8006000)

/* Size: 4K, End at: 0xA8005FFF */
#define SOC_LPMCU_CMDLST_BASE_ADDR                              (0xA8005000)

/* Size: 4K, End at: 0xA8004FFF */
#define SOC_LPMCU_TOP_BASE_ADDR                                 (0xA8004000)

/* Size: 4K, End at: 0xA8000FFF */
#define SOC_LPMCU_JPGENC_BASE_ADDR                              (0xA8000000)


/* =============================  NPU(32MB)  ============================== */
/* Size: 256K, End at: 0xA5F7FFFF */
#define SOC_LPMCU_NPU_TCU_BASE_ADDR                             (0xA5F40000)

/* Size: 128K, End at: 0xA5F3FFFF */
#define SOC_LPMCU_SYSDMA_TBU_BASE_ADDR                          (0xA5F20000)

/* Size: 128K, End at: 0xA5F1FFFF */
#define SOC_LPMCU_AIC_TBU_BASE_ADDR                             (0xA5F00000)

/* Size: 128K, End at: 0xA5EFFFFF */
#define SOC_LPMCU_sysdma_cfg_BASE_ADDR                          (0xA5EE0000)

/* Size: 4K, End at: 0xA5E07FFF */
#define SOC_LPMCU_npu_etzpc_BASE_ADDR                           (0xA5E07000)

/* Size: 4K, End at: 0xA5E06FFF */
#define SOC_LPMCU_npu_hw_exp_irq_BASE_ADDR                      (0xA5E06000)

/* Size: 4K, End at: 0xA5E05FFF */
#define SOC_LPMCU_npu_uart_BASE_ADDR                            (0xA5E05000)

/* Size: 4K, End at: 0xA5E04FFF */
#define SOC_LPMCU_npu_sysctrl_BASE_ADDR                         (0xA5E04000)

/* Size: 4K, End at: 0xA5E03FFF */
#define SOC_LPMCU_npu_crg_BASE_ADDR                             (0xA5E03000)

/* Size: 4K, End at: 0xA5E02FFF */
#define SOC_LPMCU_npu_tzpc_BASE_ADDR                            (0xA5E02000)

/* Size: 4K, End at: 0xA5E01FFF */
#define SOC_LPMCU_npu_ipc_ns_BASE_ADDR                          (0xA5E01000)

/* Size: 4K, End at: 0xA5E00FFF */
#define SOC_LPMCU_npu_ipc_s_BASE_ADDR                           (0xA5E00000)

/* Size: 2M, End at: 0xA5DFFFFF */
#define SOC_LPMCU_gic_cfg_BASE_ADDR                             (0xA5C00000)

/* Size: 1024K, End at: 0xA50FFFFF */
#define SOC_LPMCU_aic0_subsys_cfg_BASE_ADDR                     (0xA5000000)

/* Size: 64K, End at: 0xA4D3FFFF */
#define SOC_LPMCU_npu_bus_service_target_BASE_ADDR              (0xA4D30000)

/* Size: 64K, End at: 0xA423FFFF */
#define SOC_LPMCU_SRAM_reg_BASE_ADDR                            (0xA4230000)

/* Size: 64K, End at: 0xA422FFFF */
#define SOC_LPMCU_SRAM_atomicdec_BASE_ADDR                      (0xA4220000)

/* Size: 64K, End at: 0xA421FFFF */
#define SOC_LPMCU_SRAM_atomicadd_BASE_ADDR                      (0xA4210000)

/* Size: 64K, End at: 0xA420FFFF */
#define SOC_LPMCU_SRAM_normal_BASE_ADDR                         (0xA4200000)

/* Size: 64K, End at: 0xA411FFFF */
#define SOC_LPMCU_bs_slv_BASE_ADDR                              (0xA4110000)

/* Size: 4K, End at: 0xA4100FFF */
#define SOC_LPMCU_doorbell_sts_BASE_ADDR                        (0xA4100000)

/* Size: 512K, End at: 0xA40FFFFF */
#define SOC_LPMCU_doorbell_cfg_BASE_ADDR                        (0xA4080000)

/* Size: 4K, End at: 0xA4037FFF */
#define SOC_LPMCU_npu_cpu_secure_BASE_ADDR                      (0xA4037000)

/* Size: 4K, End at: 0xA4036FFF */
#define SOC_LPMCU_axi_mon_soc_BASE_ADDR                         (0xA4036000)

/* Size: 4K, End at: 0xA4035FFF */
#define SOC_LPMCU_axi_mon_bs_BASE_ADDR                          (0xA4035000)

/* Size: 4K, End at: 0xA4034FFF */
#define SOC_LPMCU_axi_mon_cpu_BASE_ADDR                         (0xA4034000)

/* Size: 4K, End at: 0xA4032FFF */
#define SOC_LPMCU_ai_cpu_local_ctrl_BASE_ADDR                   (0xA4032000)

/* Size: 4K, End at: 0xA4031FFF */
#define SOC_LPMCU_ts_cpu_local_ctrl_BASE_ADDR                   (0xA4031000)

/* Size: 4K, End at: 0xA4030FFF */
#define SOC_LPMCU_sysctrl_npu_cpu_BASE_ADDR                     (0xA4030000)

/* Size: 4K, End at: 0xA4022FFF */
#define SOC_LPMCU_fcm_local_ctrl_BASE_ADDR                      (0xA4022000)

/* Size: 4K, End at: 0xA4021FFF */
#define SOC_LPMCU_ts_cpu_a55_ctrl_BASE_ADDR                     (0xA4021000)

/* Size: 4K, End at: 0xA4011FFF */
#define SOC_LPMCU_ts_cpu_timer_BASE_ADDR                        (0xA4011000)

/* Size: 4K, End at: 0xA4010FFF */
#define SOC_LPMCU_ai_cpu_timer_BASE_ADDR                        (0xA4010000)

/* Size: 4K, End at: 0xA4001FFF */
#define SOC_LPMCU_ts_cpu_watchdog_BASE_ADDR                     (0xA4001000)

/* Size: 4K, End at: 0xA4000FFF */
#define SOC_LPMCU_ai_cpu_watchdog_BASE_ADDR                     (0xA4000000)


/* ===============================  SECRAM  =============================== */
/* Size: 256K, End at: 0x6003FFFF */
#define SOC_LPMCU_SECRAM_remap_BASE_ADDR                        (0x60000000)


/* ===============================  <N/A>  ================================ */
/* Size: 128K, End at: 0x5A9DFFFF */
#define SOC_LPMCU_AHB_RADIO_VIRTUAL_BASE_ADDR                   (0x5A9C0000)


/* ===========================  DMSS+DMACPACK  ============================ */
/* Size: 256K, End at: 0x5A9BFFFF */
#define SOC_LPMCU_DMSS_BASE_ADDR                                (0x5A980000)

/* Size: 128K, End at: 0x5A93FFFF */
#define SOC_LPMCU_DMCPACK1_BASE_ADDR                            (0x5A920000)

/* Size: 128K, End at: 0x5A91FFFF */
#define SOC_LPMCU_DMCPACK0_BASE_ADDR                            (0x5A900000)


/* =============================  CSSYS_APB  ============================== */
/* Size: 32M, End at: 0x51FFFFFF */
#define SOC_LPMCU_CSSYS_APB_BASE_ADDR                           (0x50000000)


/* =========================  LP_APB_PERI (64KB)  ========================= */
/* Size: 4K, End at: 0x4028FFFF */
#define SOC_LPMCU_LP_CONFIG_BASE_ADDR                           (0x4028F000)

/* Size: 4K, End at: 0x4028EFFF */
#define SOC_LPMCU_LP_TIMER_BASE_ADDR                            (0x4028E000)

/* Size: 4K, End at: 0x4028DFFF */
#define SOC_LPMCU_LP_WDG_BASE_ADDR                              (0x4028D000)

/* Size: 4K, End at: 0x40289FFF */
#define SOC_LPMCU_CORE_CRG_B_M_BASE_ADDR                        (0x40289000)

/* Size: 4K, End at: 0x40288FFF */
#define SOC_LPMCU_CORE_CRG_L_L3_BASE_ADDR                       (0x40288000)

/* Size: 4K, End at: 0x40287FFF */
#define SOC_LPMCU_LP_PCTRL_BASE_ADDR                            (0x40287000)

/* Size: 4K, End at: 0x40285FFF */
#define SOC_LPMCU_PERI_CRG_BASE_ADDR                            (0x40285000)

/* Size: 4K, End at: 0x40284FFF */
#define SOC_LPMCU_APB2SPI_BASE_ADDR                             (0x40284000)

/* Size: 4K, End at: 0x40282FFF */
#define SOC_LPMCU_UART6_BASE_ADDR                               (0x40282000)

/* Size: 4K, End at: 0x40281FFF */
#define SOC_LPMCU_PMC_BASE_ADDR                                 (0x40281000)

/* Size: 4K, End at: 0x40280FFF */
#define SOC_LPMCU_TSENSORC_BASE_ADDR                            (0x40280000)


/* ======================  AO_SUBSYS (512KB) PART 3  ====================== */
/* Size: 32K, End at: 0x40247FFF */
#define SOC_LPMCU_BB_DRX_BASE_ADDR                              (0x40240000)

/* Size: 4K, End at: 0x40238FFF */
#define SOC_LPMCU_GPIO36_BASE_ADDR                              (0x40238000)

/* Size: 4K, End at: 0x40237FFF */
#define SOC_LPMCU_GPIO35_BASE_ADDR                              (0x40237000)

/* Size: 4K, End at: 0x40236FFF */
#define SOC_LPMCU_GPIO34_BASE_ADDR                              (0x40236000)

/* Size: 4K, End at: 0x40235FFF */
#define SOC_LPMCU_GPIO33_BASE_ADDR                              (0x40235000)

/* Size: 4K, End at: 0x40234FFF */
#define SOC_LPMCU_GPIO32_BASE_ADDR                              (0x40234000)

/* Size: 4K, End at: 0x40233FFF */
#define SOC_LPMCU_GPIO31_BASE_ADDR                              (0x40233000)

/* Size: 4K, End at: 0x40232FFF */
#define SOC_LPMCU_GPIO30_BASE_ADDR                              (0x40232000)

/* Size: 4K, End at: 0x40231FFF */
#define SOC_LPMCU_GPIO29_BASE_ADDR                              (0x40231000)

/* Size: 4K, End at: 0x40230FFF */
#define SOC_LPMCU_GPIO28_BASE_ADDR                              (0x40230000)

/* Size: 4K, End at: 0x4022FFFF */
#define SOC_LPMCU_GPIO27_BASE_ADDR                              (0x4022F000)

/* Size: 4K, End at: 0x4022EFFF */
#define SOC_LPMCU_GPIO26_BASE_ADDR                              (0x4022E000)

/* Size: 4K, End at: 0x4022DFFF */
#define SOC_LPMCU_GPIO25_BASE_ADDR                              (0x4022D000)

/* Size: 4K, End at: 0x4022CFFF */
#define SOC_LPMCU_GPIO24_BASE_ADDR                              (0x4022C000)

/* Size: 4K, End at: 0x4022BFFF */
#define SOC_LPMCU_GPIO23_BASE_ADDR                              (0x4022B000)

/* Size: 4K, End at: 0x4022AFFF */
#define SOC_LPMCU_GPIO22_BASE_ADDR                              (0x4022A000)

/* Size: 4K, End at: 0x40229FFF */
#define SOC_LPMCU_GPIO21_BASE_ADDR                              (0x40229000)

/* Size: 4K, End at: 0x40228FFF */
#define SOC_LPMCU_GPIO20_BASE_ADDR                              (0x40228000)

/* Size: 4K, End at: 0x40225FFF */
#define SOC_LPMCU_SPI5_BASE_ADDR                                (0x40225000)

/* Size: 4K, End at: 0x40224FFF */
#define SOC_LPMCU_EFUSEC2_BASE_ADDR                             (0x40224000)

/* Size: 4K, End at: 0x40223FFF */
#define SOC_LPMCU_TIMER17_BASE_ADDR                             (0x40223000)

/* Size: 4K, End at: 0x40222FFF */
#define SOC_LPMCU_TIMER16_BASE_ADDR                             (0x40222000)

/* Size: 4K, End at: 0x40221FFF */
#define SOC_LPMCU_GPIO1_SE_BASE_ADDR                            (0x40221000)

/* Size: 4K, End at: 0x40220FFF */
#define SOC_LPMCU_AO_WDG_BASE_ADDR                              (0x40220000)

/* Size: 4K, End at: 0x4021FFFF */
#define SOC_LPMCU_SPI3_BASE_ADDR                                (0x4021F000)

/* Size: 4K, End at: 0x4021EFFF */
#define SOC_LPMCU_AO_TZPC_BASE_ADDR                             (0x4021E000)

/* Size: 4K, End at: 0x4021DFFF */
#define SOC_LPMCU_AO_LoadMonitor_BASE_ADDR                      (0x4021D000)

/* Size: 4K, End at: 0x4021CFFF */
#define SOC_LPMCU_AO_IOC_BASE_ADDR                              (0x4021C000)

/* Size: 4K, End at: 0x4021BFFF */
#define SOC_LPMCU_SCTRL_BASE_ADDR                               (0x4021B000)

/* Size: 4K, End at: 0x4021AFFF */
#define SOC_LPMCU_EFUSEC_BASE_ADDR                              (0x4021A000)

/* Size: 4K, End at: 0x40219FFF */
#define SOC_LPMCU_AO_IPC_NS_BASE_ADDR                           (0x40219000)

/* Size: 4K, End at: 0x40218FFF */
#define SOC_LPMCU_AO_IPC_S_BASE_ADDR                            (0x40218000)

/* Size: 8K, End at: 0x40217FFF */
#define SOC_LPMCU_SYS_CNT_BASE_ADDR                             (0x40216000)

/* Size: 16K, End at: 0x40213FFF */
#define SOC_LPMCU_SPMI_BASE_ADDR                                (0x40210000)

/* Size: 4K, End at: 0x4020FFFF */
#define SOC_LPMCU_SCI1_BASE_ADDR                                (0x4020F000)

/* Size: 4K, End at: 0x4020EFFF */
#define SOC_LPMCU_SCI0_BASE_ADDR                                (0x4020E000)

/* Size: 4K, End at: 0x4020DFFF */
#define SOC_LPMCU_RTC1_BASE_ADDR                                (0x4020D000)

/* Size: 4K, End at: 0x4020CFFF */
#define SOC_LPMCU_RTC0_BASE_ADDR                                (0x4020C000)

/* Size: 4K, End at: 0x4020BFFF */
#define SOC_LPMCU_TIMER15_BASE_ADDR                             (0x4020B000)

/* Size: 4K, End at: 0x4020AFFF */
#define SOC_LPMCU_TIMER14_BASE_ADDR                             (0x4020A000)

/* Size: 4K, End at: 0x40209FFF */
#define SOC_LPMCU_TIMER13_BASE_ADDR                             (0x40209000)

/* Size: 4K, End at: 0x40208FFF */
#define SOC_LPMCU_TIMER8_BASE_ADDR                              (0x40208000)

/* Size: 4K, End at: 0x40207FFF */
#define SOC_LPMCU_TIMER7_BASE_ADDR                              (0x40207000)

/* Size: 4K, End at: 0x40206FFF */
#define SOC_LPMCU_TIMER6_BASE_ADDR                              (0x40206000)

/* Size: 4K, End at: 0x40205FFF */
#define SOC_LPMCU_TIMER5_BASE_ADDR                              (0x40205000)

/* Size: 4K, End at: 0x40204FFF */
#define SOC_LPMCU_TIMER4_BASE_ADDR                              (0x40204000)

/* Size: 4K, End at: 0x40203FFF */
#define SOC_LPMCU_TIMER3_BASE_ADDR                              (0x40203000)

/* Size: 4K, End at: 0x40202FFF */
#define SOC_LPMCU_TIMER2_BASE_ADDR                              (0x40202000)

/* Size: 4K, End at: 0x40201FFF */
#define SOC_LPMCU_TIMER1_BASE_ADDR                              (0x40201000)

/* Size: 4K, End at: 0x40200FFF */
#define SOC_LPMCU_TIMER0_BASE_ADDR                              (0x40200000)


/* ================================  DRAM  ================================ */
/* Size: 256M, End at: 0x1FFFFFFF */
#define SOC_LPMCU_DRAM_BASE_ADDR                                (0x10000000)


/* ================================  BOOT  ================================ */
/* Size: 192K, End at: 0x00077FFF */
#define SOC_LPMCU_PERI_LPRAM_BASE_ADDR                          (0x00048000)

/* Size: 160K, End at: 0x00047FFF */
#define SOC_LPMCU_AO_LPRAM_BASE_ADDR                            (0x00020000)

/* Size: 96K, End at: 0x00017FFF */
#define SOC_LPMCU_BOOTROM_BASE_ADDR                             (0x00000000)



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

#endif /* end of soc_lpmcu_baseaddr_interface.h */
