/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_acpu_baseaddr_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_ACPU_BASEADDR_INTERFACE_H__
#define __SOC_ACPU_BASEADDR_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/


/* ================================  BOOT  ================================ */
/* Size: 96K, End at: 0xFFFF7FFF */
#define SOC_ACPU_BOOTROM_BASE_ADDR                              (0xFFFE0000)

/* Size: 192K, End at: 0xFFFA7FFF */
#define SOC_ACPU_PERI_LPRAM_BASE_ADDR                           (0xFFF78000)

/* Size: 160K, End at: 0xFFF77FFF */
#define SOC_ACPU_AO_LPRAM_BASE_ADDR                             (0xFFF50000)


/* =========================  LP_APB_PERI (64KB)  ========================= */
/* Size: 4K, End at: 0xFFF0FFFF */
#define SOC_ACPU_LP_CONFIG_BASE_ADDR                            (0xFFF0F000)

/* Size: 4K, End at: 0xFFF0EFFF */
#define SOC_ACPU_LP_TIMER_BASE_ADDR                             (0xFFF0E000)

/* Size: 4K, End at: 0xFFF0DFFF */
#define SOC_ACPU_LP_WDG_BASE_ADDR                               (0xFFF0D000)

/* Size: 4K, End at: 0xFFF09FFF */
#define SOC_ACPU_CORE_CRG_B_M_BASE_ADDR                         (0xFFF09000)

/* Size: 4K, End at: 0xFFF08FFF */
#define SOC_ACPU_CORE_CRG_L_L3_BASE_ADDR                        (0xFFF08000)

/* Size: 4K, End at: 0xFFF07FFF */
#define SOC_ACPU_LP_PCTRL_BASE_ADDR                             (0xFFF07000)

/* Size: 4K, End at: 0xFFF05FFF */
#define SOC_ACPU_PERI_CRG_BASE_ADDR                             (0xFFF05000)

/* Size: 4K, End at: 0xFFF04FFF */
#define SOC_ACPU_APB2SPI_BASE_ADDR                              (0xFFF04000)

/* Size: 4K, End at: 0xFFF02FFF */
#define SOC_ACPU_UART6_BASE_ADDR                                (0xFFF02000)

/* Size: 4K, End at: 0xFFF01FFF */
#define SOC_ACPU_PMC_BASE_ADDR                                  (0xFFF01000)

/* Size: 4K, End at: 0xFFF00FFF */
#define SOC_ACPU_TSENSORC_BASE_ADDR                             (0xFFF00000)


/* ===============================  <N/A>  ================================ */
/* Size: 128K, End at: 0xFFEDFFFF */
#define SOC_ACPU_AHB_RADIO_VIRTUAL_BASE_ADDR                    (0xFFEC0000)


/* ===========================  DMSS+DMACPACK  ============================ */
/* Size: 256K, End at: 0xFFEBFFFF */
#define SOC_ACPU_DMSS_BASE_ADDR                                 (0xFFE80000)

/* Size: 128K, End at: 0xFFE3FFFF */
#define SOC_ACPU_DMCPACK1_BASE_ADDR                             (0xFFE20000)

/* Size: 128K, End at: 0xFFE1FFFF */
#define SOC_ACPU_DMCPACK0_BASE_ADDR                             (0xFFE00000)


/* ===========================  CFG_SUBSYS 8MB  =========================== */
/* Size: 4K, End at: 0xFED3FFFF */
#define SOC_ACPU_CODEC_SSI_BASE_ADDR                            (0xFED3F000)

/* Size: 64K, End at: 0xFED2FFFF */
#define SOC_ACPU_IPC_NS_BASE_ADDR                               (0xFED20000)

/* Size: 64K, End at: 0xFED1FFFF */
#define SOC_ACPU_IPC_BASE_ADDR                                  (0xFED10000)

/* Size: 4K, End at: 0xFED02FFF */
#define SOC_ACPU_IOC_BASE_ADDR                                  (0xFED02000)

/* Size: 4K, End at: 0xFEC41FFF */
#define SOC_ACPU_LoadMonitor1_BASE_ADDR                         (0xFEC41000)

/* Size: 4K, End at: 0xFEC40FFF */
#define SOC_ACPU_LoadMonitor0_BASE_ADDR                         (0xFEC40000)

/* Size: 4K, End at: 0xFEC3EFFF */
#define SOC_ACPU_PCTRL_BASE_ADDR                                (0xFEC3E000)

/* Size: 4K, End at: 0xFEC3DFFF */
#define SOC_ACPU_ATGC_BASE_ADDR                                 (0xFEC3D000)

/* Size: 4K, End at: 0xFEC3CFFF */
#define SOC_ACPU_TZPC_BASE_ADDR                                 (0xFEC3C000)

/* Size: 4K, End at: 0xFEC39FFF */
#define SOC_ACPU_ATGC1_BASE_ADDR                                (0xFEC39000)

/* Size: 4K, End at: 0xFEC37FFF */
#define SOC_ACPU_WD1_BASE_ADDR                                  (0xFEC37000)

/* Size: 4K, End at: 0xFEC36FFF */
#define SOC_ACPU_WD0_BASE_ADDR                                  (0xFEC36000)

/* Size: 4K, End at: 0xFEC34FFF */
#define SOC_ACPU_BLPWM_BASE_ADDR                                (0xFEC34000)

/* Size: 8K, End at: 0xFEC31FFF */
#define SOC_ACPU_GPUPCR_BASE_ADDR                               (0xFEC30000)

/* Size: 4K, End at: 0xFEC2FFFF */
#define SOC_ACPU_TIMER12_BASE_ADDR                              (0xFEC2F000)

/* Size: 4K, End at: 0xFEC2EFFF */
#define SOC_ACPU_TIMER11_BASE_ADDR                              (0xFEC2E000)

/* Size: 4K, End at: 0xFEC2DFFF */
#define SOC_ACPU_TIMER10_BASE_ADDR                              (0xFEC2D000)

/* Size: 4K, End at: 0xFEC2CFFF */
#define SOC_ACPU_TIMER9_BASE_ADDR                               (0xFEC2C000)

/* Size: 4K, End at: 0xFEC25FFF */
#define SOC_ACPU_GPIO19_BASE_ADDR                               (0xFEC25000)

/* Size: 4K, End at: 0xFEC24FFF */
#define SOC_ACPU_GPIO18_BASE_ADDR                               (0xFEC24000)

/* Size: 4K, End at: 0xFEC23FFF */
#define SOC_ACPU_GPIO17_BASE_ADDR                               (0xFEC23000)

/* Size: 4K, End at: 0xFEC22FFF */
#define SOC_ACPU_GPIO16_BASE_ADDR                               (0xFEC22000)

/* Size: 4K, End at: 0xFEC21FFF */
#define SOC_ACPU_GPIO15_BASE_ADDR                               (0xFEC21000)

/* Size: 4K, End at: 0xFEC20FFF */
#define SOC_ACPU_GPIO14_BASE_ADDR                               (0xFEC20000)

/* Size: 4K, End at: 0xFEC1FFFF */
#define SOC_ACPU_GPIO13_BASE_ADDR                               (0xFEC1F000)

/* Size: 4K, End at: 0xFEC1EFFF */
#define SOC_ACPU_GPIO12_BASE_ADDR                               (0xFEC1E000)

/* Size: 4K, End at: 0xFEC1DFFF */
#define SOC_ACPU_GPIO11_BASE_ADDR                               (0xFEC1D000)

/* Size: 4K, End at: 0xFEC1CFFF */
#define SOC_ACPU_GPIO10_BASE_ADDR                               (0xFEC1C000)

/* Size: 4K, End at: 0xFEC1BFFF */
#define SOC_ACPU_GPIO9_BASE_ADDR                                (0xFEC1B000)

/* Size: 4K, End at: 0xFEC1AFFF */
#define SOC_ACPU_GPIO8_BASE_ADDR                                (0xFEC1A000)

/* Size: 4K, End at: 0xFEC19FFF */
#define SOC_ACPU_GPIO7_BASE_ADDR                                (0xFEC19000)

/* Size: 4K, End at: 0xFEC18FFF */
#define SOC_ACPU_GPIO6_BASE_ADDR                                (0xFEC18000)

/* Size: 4K, End at: 0xFEC17FFF */
#define SOC_ACPU_GPIO5_BASE_ADDR                                (0xFEC17000)

/* Size: 4K, End at: 0xFEC16FFF */
#define SOC_ACPU_GPIO4_BASE_ADDR                                (0xFEC16000)

/* Size: 4K, End at: 0xFEC15FFF */
#define SOC_ACPU_GPIO3_BASE_ADDR                                (0xFEC15000)

/* Size: 4K, End at: 0xFEC14FFF */
#define SOC_ACPU_GPIO2_BASE_ADDR                                (0xFEC14000)

/* Size: 4K, End at: 0xFEC13FFF */
#define SOC_ACPU_GPIO1_BASE_ADDR                                (0xFEC13000)

/* Size: 4K, End at: 0xFEC12FFF */
#define SOC_ACPU_GPIO0_BASE_ADDR                                (0xFEC12000)

/* Size: 4K, End at: 0xFEC10FFF */
#define SOC_ACPU_GPIO0_SE_BASE_ADDR                             (0xFEC10000)

/* Size: 64K, End at: 0xFEC0FFFF */
#define SOC_ACPU_fcm_service_target_BASE_ADDR                   (0xFEC00000)

/* Size: 4K, End at: 0xFEB07FFF */
#define SOC_ACPU_FCM_GIC_HW_SPI1_BASE_ADDR                      (0xFEB07000)

/* Size: 4K, End at: 0xFEB05FFF */
#define SOC_ACPU_FCM_LOCAL_GIC_BASE_ADDR                        (0xFEB05000)

/* Size: 4K, End at: 0xFEB04FFF */
#define SOC_ACPU_FCM_GIC_HW_SPI0_BASE_ADDR                      (0xFEB04000)

/* Size: 4K, End at: 0xFEB03FFF */
#define SOC_ACPU_FCM_LOCAL_PERI_BASE_ADDR                       (0xFEB03000)

/* Size: 4K, End at: 0xFEB02FFF */
#define SOC_ACPU_FCM_SECURE_BASE_ADDR                           (0xFEB02000)

/* Size: 4K, End at: 0xFEA0CFFF */
#define SOC_ACPU_FCM_LOCAL_IP_BASE_ADDR                         (0xFEA0C000)

/* Size: 4K, End at: 0xFEA0BFFF */
#define SOC_ACPU_FCM_CORE7_LOCAL_CFG_BASE_ADDR                  (0xFEA0B000)

/* Size: 4K, End at: 0xFEA0AFFF */
#define SOC_ACPU_FCM_CORE6_LOCAL_CFG_BASE_ADDR                  (0xFEA0A000)

/* Size: 4K, End at: 0xFEA09FFF */
#define SOC_ACPU_FCM_CORE5_LOCAL_CFG_BASE_ADDR                  (0xFEA09000)

/* Size: 4K, End at: 0xFEA08FFF */
#define SOC_ACPU_FCM_CORE4_LOCAL_CFG_BASE_ADDR                  (0xFEA08000)

/* Size: 4K, End at: 0xFEA07FFF */
#define SOC_ACPU_FCM_CORE3_LOCAL_CFG_BASE_ADDR                  (0xFEA07000)

/* Size: 4K, End at: 0xFEA06FFF */
#define SOC_ACPU_FCM_CORE2_LOCAL_CFG_BASE_ADDR                  (0xFEA06000)

/* Size: 4K, End at: 0xFEA05FFF */
#define SOC_ACPU_FCM_CORE1_LOCAL_CFG_BASE_ADDR                  (0xFEA05000)

/* Size: 4K, End at: 0xFEA04FFF */
#define SOC_ACPU_FCM_CORE0_LOCAL_CFG_BASE_ADDR                  (0xFEA04000)

/* Size: 4K, End at: 0xFEA02FFF */
#define SOC_ACPU_PCR_MID_WRAP_BASE_ADDR                         (0xFEA02000)

/* Size: 4K, End at: 0xFEA01FFF */
#define SOC_ACPU_FCM_LOCAL_CPU_BASE_ADDR                        (0xFEA01000)

/* Size: 2M, End at: 0xFE9FFFFF */
#define SOC_ACPU_GIC600_BASE_ADDR                               (0xFE800000)


/* ===============================  <N/A>  ================================ */
/* Size: 64K, End at: 0xFE3AFFFF */
#define SOC_ACPU_MAA_BASE_ADDR                                  (0xFE3A0000)

/* Size: 64K, End at: 0xFE29FFFF */
#define SOC_ACPU_MDM_5G_Service_Target_BASE_ADDR                (0xFE290000)

/* Size: 64K, End at: 0xFE28FFFF */
#define SOC_ACPU_FD_BUS_Service_Target_BASE_ADDR                (0xFE280000)

/* Size: 64K, End at: 0xFE27FFFF */
#define SOC_ACPU_AOBUS_Service_Target_BASE_ADDR                 (0xFE270000)

/* Size: 64K, End at: 0xFE26FFFF */
#define SOC_ACPU_DMA_NOC_Service_Target_BASE_ADDR               (0xFE260000)

/* Size: 64K, End at: 0xFE25FFFF */
#define SOC_ACPU_CFGBUS_Service_Target_BASE_ADDR                (0xFE250000)

/* Size: 64K, End at: 0xFE24FFFF */
#define SOC_ACPU_SYS_BUS_Service_Target_BASE_ADDR               (0xFE240000)

/* Size: 64K, End at: 0xFE23FFFF */
#define SOC_ACPU_ASP_Service_Target_BASE_ADDR                   (0xFE230000)

/* Size: 64K, End at: 0xFE22FFFF */
#define SOC_ACPU_Modem_Service_Target_BASE_ADDR                 (0xFE220000)

/* Size: 64K, End at: 0xFE21FFFF */
#define SOC_ACPU_HSDT_Service_Target_BASE_ADDR                  (0xFE210000)

/* Size: 64K, End at: 0xFE20FFFF */
#define SOC_ACPU_MMC0_Service_target_BASE_ADDR                  (0xFE200000)


/* =============================  CSSYS_APB  ============================== */
/* Size: 32M, End at: 0xFDFFFFFF */
#define SOC_ACPU_CSSYS_APB_BASE_ADDR                            (0xFC000000)


/* ===============================  <N/A>  ================================ */
/* Size: 16M, End at: 0xFBFFFFFF */
#define SOC_ACPU_G3D_BASE_ADDR                                  (0xFB000000)


/* ==========================  AO_SUBSYS(12MB)  =========================== */
/* Size: 4K, End at: 0xFABE0FFF */
#define SOC_ACPU_UFS_SYS_CTRL_BASE_ADDR                         (0xFABE0000)

/* Size: 128K, End at: 0xFABDFFFF */
#define SOC_ACPU_UFS_MCU_BASE_ADDR                              (0xFABC0000)

/* Size: 1536K, End at: 0xFAB7FFFF */
#define SOC_ACPU_UFS_CFG_BASE_ADDR                              (0xFAA00000)

/* Size: 32K, End at: 0xFA987FFF */
#define SOC_ACPU_AO_TCP_BASE_ADDR                               (0xFA980000)

/* Size: 4K, End at: 0xFA902FFF */
#define SOC_ACPU_DSF_IF_BASE_ADDR                               (0xFA902000)

/* Size: 4K, End at: 0xFA901FFF */
#define SOC_ACPU_MAD_Timer_BASE_ADDR                            (0xFA901000)

/* Size: 4K, End at: 0xFA900FFF */
#define SOC_ACPU_MAD_REG_BASE_ADDR                              (0xFA900000)

/* Size: 32K, End at: 0xFA8C7FFF */
#define SOC_ACPU_BB_DRX_BASE_ADDR                               (0xFA8C0000)

/* Size: 4K, End at: 0xFA8B8FFF */
#define SOC_ACPU_GPIO36_BASE_ADDR                               (0xFA8B8000)

/* Size: 4K, End at: 0xFA8B7FFF */
#define SOC_ACPU_GPIO35_BASE_ADDR                               (0xFA8B7000)

/* Size: 4K, End at: 0xFA8B6FFF */
#define SOC_ACPU_GPIO34_BASE_ADDR                               (0xFA8B6000)

/* Size: 4K, End at: 0xFA8B5FFF */
#define SOC_ACPU_GPIO33_BASE_ADDR                               (0xFA8B5000)

/* Size: 4K, End at: 0xFA8B4FFF */
#define SOC_ACPU_GPIO32_BASE_ADDR                               (0xFA8B4000)

/* Size: 4K, End at: 0xFA8B3FFF */
#define SOC_ACPU_GPIO31_BASE_ADDR                               (0xFA8B3000)

/* Size: 4K, End at: 0xFA8B2FFF */
#define SOC_ACPU_GPIO30_BASE_ADDR                               (0xFA8B2000)

/* Size: 4K, End at: 0xFA8B1FFF */
#define SOC_ACPU_GPIO29_BASE_ADDR                               (0xFA8B1000)

/* Size: 4K, End at: 0xFA8B0FFF */
#define SOC_ACPU_GPIO28_BASE_ADDR                               (0xFA8B0000)

/* Size: 4K, End at: 0xFA8AFFFF */
#define SOC_ACPU_GPIO27_BASE_ADDR                               (0xFA8AF000)

/* Size: 4K, End at: 0xFA8AEFFF */
#define SOC_ACPU_GPIO26_BASE_ADDR                               (0xFA8AE000)

/* Size: 4K, End at: 0xFA8ADFFF */
#define SOC_ACPU_GPIO25_BASE_ADDR                               (0xFA8AD000)

/* Size: 4K, End at: 0xFA8ACFFF */
#define SOC_ACPU_GPIO24_BASE_ADDR                               (0xFA8AC000)

/* Size: 4K, End at: 0xFA8ABFFF */
#define SOC_ACPU_GPIO23_BASE_ADDR                               (0xFA8AB000)

/* Size: 4K, End at: 0xFA8AAFFF */
#define SOC_ACPU_GPIO22_BASE_ADDR                               (0xFA8AA000)

/* Size: 4K, End at: 0xFA8A9FFF */
#define SOC_ACPU_GPIO21_BASE_ADDR                               (0xFA8A9000)

/* Size: 4K, End at: 0xFA8A8FFF */
#define SOC_ACPU_GPIO20_BASE_ADDR                               (0xFA8A8000)

/* Size: 4K, End at: 0xFA8A5FFF */
#define SOC_ACPU_SPI5_BASE_ADDR                                 (0xFA8A5000)

/* Size: 4K, End at: 0xFA8A4FFF */
#define SOC_ACPU_EFUSEC2_BASE_ADDR                              (0xFA8A4000)

/* Size: 4K, End at: 0xFA8A3FFF */
#define SOC_ACPU_TIMER17_BASE_ADDR                              (0xFA8A3000)

/* Size: 4K, End at: 0xFA8A2FFF */
#define SOC_ACPU_TIMER16_BASE_ADDR                              (0xFA8A2000)

/* Size: 4K, End at: 0xFA8A1FFF */
#define SOC_ACPU_GPIO1_SE_BASE_ADDR                             (0xFA8A1000)

/* Size: 4K, End at: 0xFA8A0FFF */
#define SOC_ACPU_AO_WDG_BASE_ADDR                               (0xFA8A0000)

/* Size: 4K, End at: 0xFA89FFFF */
#define SOC_ACPU_SPI3_BASE_ADDR                                 (0xFA89F000)

/* Size: 4K, End at: 0xFA89EFFF */
#define SOC_ACPU_AO_TZPC_BASE_ADDR                              (0xFA89E000)

/* Size: 4K, End at: 0xFA89DFFF */
#define SOC_ACPU_AO_LoadMonitor_BASE_ADDR                       (0xFA89D000)

/* Size: 4K, End at: 0xFA89CFFF */
#define SOC_ACPU_AO_IOC_BASE_ADDR                               (0xFA89C000)

/* Size: 4K, End at: 0xFA89BFFF */
#define SOC_ACPU_SCTRL_BASE_ADDR                                (0xFA89B000)

/* Size: 4K, End at: 0xFA89AFFF */
#define SOC_ACPU_EFUSEC_BASE_ADDR                               (0xFA89A000)

/* Size: 4K, End at: 0xFA899FFF */
#define SOC_ACPU_AO_IPC_NS_BASE_ADDR                            (0xFA899000)

/* Size: 4K, End at: 0xFA898FFF */
#define SOC_ACPU_AO_IPC_S_BASE_ADDR                             (0xFA898000)

/* Size: 8K, End at: 0xFA897FFF */
#define SOC_ACPU_SYS_CNT_BASE_ADDR                              (0xFA896000)

/* Size: 16K, End at: 0xFA893FFF */
#define SOC_ACPU_SPMI_BASE_ADDR                                 (0xFA890000)

/* Size: 4K, End at: 0xFA88FFFF */
#define SOC_ACPU_SCI1_BASE_ADDR                                 (0xFA88F000)

/* Size: 4K, End at: 0xFA88EFFF */
#define SOC_ACPU_SCI0_BASE_ADDR                                 (0xFA88E000)

/* Size: 4K, End at: 0xFA88DFFF */
#define SOC_ACPU_RTC1_BASE_ADDR                                 (0xFA88D000)

/* Size: 4K, End at: 0xFA88CFFF */
#define SOC_ACPU_RTC0_BASE_ADDR                                 (0xFA88C000)

/* Size: 4K, End at: 0xFA88BFFF */
#define SOC_ACPU_TIMER15_BASE_ADDR                              (0xFA88B000)

/* Size: 4K, End at: 0xFA88AFFF */
#define SOC_ACPU_TIMER14_BASE_ADDR                              (0xFA88A000)

/* Size: 4K, End at: 0xFA889FFF */
#define SOC_ACPU_TIMER13_BASE_ADDR                              (0xFA889000)

/* Size: 4K, End at: 0xFA888FFF */
#define SOC_ACPU_TIMER8_BASE_ADDR                               (0xFA888000)

/* Size: 4K, End at: 0xFA887FFF */
#define SOC_ACPU_TIMER7_BASE_ADDR                               (0xFA887000)

/* Size: 4K, End at: 0xFA886FFF */
#define SOC_ACPU_TIMER6_BASE_ADDR                               (0xFA886000)

/* Size: 4K, End at: 0xFA885FFF */
#define SOC_ACPU_TIMER5_BASE_ADDR                               (0xFA885000)

/* Size: 4K, End at: 0xFA884FFF */
#define SOC_ACPU_TIMER4_BASE_ADDR                               (0xFA884000)

/* Size: 4K, End at: 0xFA883FFF */
#define SOC_ACPU_TIMER3_BASE_ADDR                               (0xFA883000)

/* Size: 4K, End at: 0xFA882FFF */
#define SOC_ACPU_TIMER2_BASE_ADDR                               (0xFA882000)

/* Size: 4K, End at: 0xFA881FFF */
#define SOC_ACPU_TIMER1_BASE_ADDR                               (0xFA881000)

/* Size: 4K, End at: 0xFA880FFF */
#define SOC_ACPU_TIMER0_BASE_ADDR                               (0xFA880000)

/* Size: 4K, End at: 0xFA87FFFF */
#define SOC_ACPU_IOMCU_RTC_BASE_ADDR                            (0xFA87F000)

/* Size: 4K, End at: 0xFA87EFFF */
#define SOC_ACPU_IOMCU_CONFIG_BASE_ADDR                         (0xFA87E000)

/* Size: 4K, End at: 0xFA87DFFF */
#define SOC_ACPU_IOMCU_TIMER_BASE_ADDR                          (0xFA87D000)

/* Size: 4K, End at: 0xFA87CFFF */
#define SOC_ACPU_IOMCU_WDG_BASE_ADDR                            (0xFA87C000)

/* Size: 4K, End at: 0xFA87BFFF */
#define SOC_ACPU_IOMCU_GPIO3_BASE_ADDR                          (0xFA87B000)

/* Size: 4K, End at: 0xFA87AFFF */
#define SOC_ACPU_IOMCU_GPIO2_BASE_ADDR                          (0xFA87A000)

/* Size: 4K, End at: 0xFA879FFF */
#define SOC_ACPU_IOMCU_GPIO1_BASE_ADDR                          (0xFA879000)

/* Size: 4K, End at: 0xFA878FFF */
#define SOC_ACPU_IOMCU_GPIO0_BASE_ADDR                          (0xFA878000)

/* Size: 4K, End at: 0xFA877FFF */
#define SOC_ACPU_IOMCU_DMAC_BASE_ADDR                           (0xFA877000)

/* Size: 4K, End at: 0xFA876FFF */
#define SOC_ACPU_IOMCU_UART7_BASE_ADDR                          (0xFA876000)

/* Size: 4K, End at: 0xFA875FFF */
#define SOC_ACPU_IOMCU_BLPWM_BASE_ADDR                          (0xFA875000)

/* Size: 4K, End at: 0xFA874FFF */
#define SOC_ACPU_IOMCU_UART3_BASE_ADDR                          (0xFA874000)

/* Size: 4K, End at: 0xFA873FFF */
#define SOC_ACPU_IOMCU_I3C1_BASE_ADDR                           (0xFA873000)

/* Size: 4K, End at: 0xFA872FFF */
#define SOC_ACPU_IOMCU_I2C1_BASE_ADDR                           (0xFA872000)

/* Size: 4K, End at: 0xFA871FFF */
#define SOC_ACPU_IOMCU_I2C0_BASE_ADDR                           (0xFA871000)

/* Size: 4K, End at: 0xFA870FFF */
#define SOC_ACPU_IOMCU_SPI0_BASE_ADDR                           (0xFA870000)

/* Size: 4K, End at: 0xFA86FFFF */
#define SOC_ACPU_IOMCU_DTCM1Remap_BASE_ADDR                     (0xFA86F000)

/* Size: 4K, End at: 0xFA86EFFF */
#define SOC_ACPU_IOMCU_DTCM0Remap_BASE_ADDR                     (0xFA86E000)

/* Size: 8K, End at: 0xFA86DFFF */
#define SOC_ACPU_IOMCU_ITCMRemap_BASE_ADDR                      (0xFA86C000)

/* Size: 4K, End at: 0xFA86BFFF */
#define SOC_ACPU_IOMCU_Remapctrl_BASE_ADDR                      (0xFA86B000)

/* Size: 4K, End at: 0xFA86AFFF */
#define SOC_ACPU_IOMCU_I3C2_BASE_ADDR                           (0xFA86A000)

/* Size: 4K, End at: 0xFA869FFF */
#define SOC_ACPU_IOMCU_UART8_BASE_ADDR                          (0xFA869000)

/* Size: 4K, End at: 0xFA868FFF */
#define SOC_ACPU_IOMCU_SPI2_BASE_ADDR                           (0xFA868000)

/* Size: 4K, End at: 0xFA867FFF */
#define SOC_ACPU_IOMCU_DMMU_BASE_ADDR                           (0xFA867000)

/* Size: 4K, End at: 0xFA866FFF */
#define SOC_ACPU_IOMCU_TIMER2_BASE_ADDR                         (0xFA866000)

/* Size: 4K, End at: 0xFA865FFF */
#define SOC_ACPU_IOMCU_I3C_BASE_ADDR                            (0xFA865000)

/* Size: 4K, End at: 0xFA864FFF */
#define SOC_ACPU_IOMCU_I3C3_BASE_ADDR                           (0xFA864000)

/* Size: 4K, End at: 0xFA863FFF */
#define SOC_ACPU_IOMCU_I2C5_BASE_ADDR                           (0xFA863000)

/* Size: 1M, End at: 0xFA7FFFFF */
#define SOC_ACPU_IOMCU_DTCM_BASE_ADDR                           (0xFA700000)

/* Size: 1M, End at: 0xFA6FFFFF */
#define SOC_ACPU_IOMCU_ITCM_BASE_ADDR                           (0xFA600000)

/* Size: 48K, End at: 0xFA58BFFF */
#define SOC_ACPU_DSP_ITCM_BASE_ADDR                             (0xFA580000)

/* Size: 160K, End at: 0xFA57FFFF */
#define SOC_ACPU_DSP_DTCM_BASE_ADDR                             (0xFA558000)

/* Size: 8K, End at: 0xFA553FFF */
#define SOC_ACPU_ASP_CODEC_BASE_ADDR                            (0xFA552000)

/* Size: 8K, End at: 0xFA551FFF */
#define SOC_ACPU_SLIMBUS_BASE_ADDR                              (0xFA550000)

/* Size: 1K, End at: 0xFA54FFFF */
#define SOC_ACPU_DSD_BASE_ADDR                                  (0xFA54FC00)

/* Size: 1K, End at: 0xFA54E3FF */
#define SOC_ACPU_ASP_CFG_BASE_ADDR                              (0xFA54E000)

/* Size: 4K, End at: 0xFA54DFFF */
#define SOC_ACPU_ASP_WD_BASE_ADDR                               (0xFA54D000)

/* Size: 4K, End at: 0xFA54CFFF */
#define SOC_ACPU_ASP_IPC_BASE_ADDR                              (0xFA54C000)

/* Size: 4K, End at: 0xFA54BFFF */
#define SOC_ACPU_ASP_DMAC_BASE_ADDR                             (0xFA54B000)

/* Size: 4K, End at: 0xFA54AFFF */
#define SOC_ACPU_ASP_TIMER1_BASE_ADDR                           (0xFA54A000)

/* Size: 4K, End at: 0xFA549FFF */
#define SOC_ACPU_ASP_TIMER0_BASE_ADDR                           (0xFA549000)

/* Size: 4K, End at: 0xFA548FFF */
#define SOC_ACPU_ASP_GPIO_BASE_ADDR                             (0xFA548000)

/* Size: 4K, End at: 0xFA547FFF */
#define SOC_ACPU_ASP_DMMU_BASE_ADDR                             (0xFA547000)

/* Size: 256K, End at: 0xFA4BFFFF */
#define SOC_ACPU_SECRAM_BASE_ADDR                               (0xFA480000)


/* =======================  DMA_PERI_SUBSYS(17MB)  ======================== */
/* Size: 6K, End at: 0xFA0A17FF */
#define SOC_ACPU_SOCP_BASE_ADDR                                 (0xFA0A0000)

/* Size: 32K, End at: 0xFA097FFF */
#define SOC_ACPU_EICC_BASE_ADDR                                 (0xFA090000)

/* Size: 32K, End at: 0xFA08FFFF */
#define SOC_ACPU_IPF_BASE_ADDR                                  (0xFA088000)

/* Size: 16K, End at: 0xFA087FFF */
#define SOC_ACPU_SPE_BASE_ADDR                                  (0xFA084000)

/* Size: 4K, End at: 0xFA081FFF */
#define SOC_ACPU_IPC_MDM_NS_BASE_ADDR                           (0xFA081000)

/* Size: 4K, End at: 0xFA080FFF */
#define SOC_ACPU_IPC_MDM_S_BASE_ADDR                            (0xFA080000)

/* Size: 4K, End at: 0xFA057FFF */
#define SOC_ACPU_LAT_STAT_BASE_ADDR                             (0xFA057000)

/* Size: 4K, End at: 0xFA056FFF */
#define SOC_ACPU_PERF_STAT_BASE_ADDR                            (0xFA056000)

/* Size: 4K, End at: 0xFA054FFF */
#define SOC_ACPU_UART0_BASE_ADDR                                (0xFA054000)

/* Size: 4K, End at: 0xFA050FFF */
#define SOC_ACPU_I3C4_BASE_ADDR                                 (0xFA050000)

/* Size: 4K, End at: 0xFA04FFFF */
#define SOC_ACPU_I2C7_BASE_ADDR                                 (0xFA04F000)

/* Size: 4K, End at: 0xFA04EFFF */
#define SOC_ACPU_I2C6_BASE_ADDR                                 (0xFA04E000)

/* Size: 4K, End at: 0xFA04DFFF */
#define SOC_ACPU_I2C4_BASE_ADDR                                 (0xFA04D000)

/* Size: 4K, End at: 0xFA04CFFF */
#define SOC_ACPU_I2C3_BASE_ADDR                                 (0xFA04C000)

/* Size: 4K, End at: 0xFA04BFFF */
#define SOC_ACPU_I2C9_BASE_ADDR                                 (0xFA04B000)

/* Size: 4K, End at: 0xFA049FFF */
#define SOC_ACPU_SPI4_BASE_ADDR                                 (0xFA049000)

/* Size: 4K, End at: 0xFA048FFF */
#define SOC_ACPU_SPI1_BASE_ADDR                                 (0xFA048000)

/* Size: 4K, End at: 0xFA045FFF */
#define SOC_ACPU_UART5_BASE_ADDR                                (0xFA045000)

/* Size: 4K, End at: 0xFA044FFF */
#define SOC_ACPU_UART2_BASE_ADDR                                (0xFA044000)

/* Size: 4K, End at: 0xFA041FFF */
#define SOC_ACPU_UART4_BASE_ADDR                                (0xFA041000)

/* Size: 4K, End at: 0xFA040FFF */
#define SOC_ACPU_UART1_BASE_ADDR                                (0xFA040000)

/* Size: 4K, End at: 0xFA000FFF */
#define SOC_ACPU_PERI_DMAC_BASE_ADDR                            (0xFA000000)


/* =========================  MMC0_SUBSYS (2MB)  ========================== */
/* Size: 1K, End at: 0xF85873FF */
#define SOC_ACPU_USB20PHY_BASE_ADDR                             (0xF8587000)

/* Size: 4K, End at: 0xF8586FFF */
#define SOC_ACPU_USB20_SYSCTRL_BASE_ADDR                        (0xF8586000)

/* Size: 4K, End at: 0xF8585FFF */
#define SOC_ACPU_PERI_IOCG_R_BASE_ADDR                          (0xF8585000)

/* Size: 4K, End at: 0xF8584FFF */
#define SOC_ACPU_PERI_IOCG_L_BASE_ADDR                          (0xF8584000)

/* Size: 4K, End at: 0xF8583FFF */
#define SOC_ACPU_SD3_BASE_ADDR                                  (0xF8583000)

/* Size: 4K, End at: 0xF8582FFF */
#define SOC_ACPU_MMC0_SYS_CTRL_BASE_ADDR                        (0xF8582000)

/* Size: 4K, End at: 0xF8581FFF */
#define SOC_ACPU_MMC0_CRG_BASE_ADDR                             (0xF8581000)

/* Size: 4K, End at: 0xF8580FFF */
#define SOC_ACPU_IOC_MMC0_BASE_ADDR                             (0xF8580000)

/* Size: 512K, End at: 0xF847FFFF */
#define SOC_ACPU_USB_OTG_BASE_ADDR                              (0xF8400000)


/* =====================  HSDT_SUBSYS (98MB) PART 1   ===================== */
/* Size: 128K, End at: 0xF81FFFFF */
#define SOC_ACPU_HSDT_TBU_BASE_ADDR                             (0xF81E0000)

/* Size: 384K, End at: 0xF81DFFFF */
#define SOC_ACPU_HSDT_TCU_BASE_ADDR                             (0xF8180000)

/* Size: 4K, End at: 0xF817FFFF */
#define SOC_ACPU_HSDT_SYS_CTRL_BASE_ADDR                        (0xF817F000)

/* Size: 4K, End at: 0xF817EFFF */
#define SOC_ACPU_HSDT_CRG_BASE_ADDR                             (0xF817E000)

/* Size: 4K, End at: 0xF817DFFF */
#define SOC_ACPU_PCIE_PLL_BASE_ADDR                             (0xF817D000)

/* Size: 4K, End at: 0xF8105FFF */
#define SOC_ACPU_MMC1_IOC_BASE_ADDR                             (0xF8105000)

/* Size: 4K, End at: 0xF8104FFF */
#define SOC_ACPU_MMC1_SDIO_BASE_ADDR                            (0xF8104000)

/* Size: 4K, End at: 0xF8102FFF */
#define SOC_ACPU_PCIE0_APB_CFG_BASE_ADDR                        (0xF8102000)

/* Size: 4K, End at: 0xF8101FFF */
#define SOC_ACPU_CCP_BASE_ADDR                                  (0xF8101000)

/* Size: 4K, End at: 0xF8100FFF */
#define SOC_ACPU_CCS_BASE_ADDR                                  (0xF8100000)

/* Size: 64K, End at: 0xF80FFFFF */
#define SOC_ACPU_PCIE_MCU_BASE_ADDR                             (0xF80F0000)

/* Size: 512K, End at: 0xF807FFFF */
#define SOC_ACPU_PCIEPHY0_BASE_ADDR                             (0xF8000000)


/* ===========================  MDM_5G(128MB)  ============================ */
/* Size: 64K, End at: 0xF604FFFF */
#define SOC_ACPU_TSP_DSS1_L1TCM_GLOBAL_BASE_ADDR                (0xF6040000)

/* Size: 64K, End at: 0xF600FFFF */
#define SOC_ACPU_TSP_DSS0_L1TCM_GLOBAL_BASE_ADDR                (0xF6000000)

/* Size: 64K, End at: 0xF5F0FFFF */
#define SOC_ACPU_TSP_L1TCM_LOCAL_BASE_ADDR                      (0xF5F00000)

/* Size: 2M, End at: 0xF59FFFFF */
#define SOC_ACPU_TSP_REG_BASE_ADDR                              (0xF5800000)

/* Size: 4352K, End at: 0xF543FFFF */
#define SOC_ACPU_TSP_L2M_BASE_ADDR                              (0xF5000000)

/* Size: 64K, End at: 0xF4D0FFFF */
#define SOC_ACPU_NOC_MDMBUS_SERVICE_TARGET_BASE_ADDR            (0xF4D00000)

/* Size: 4K, End at: 0xF4C00FFF */
#define SOC_ACPU_MDM_AMON_BASE_ADDR                             (0xF4C00000)

/* Size: 4K, End at: 0xF4B5BFFF */
#define SOC_ACPU_MDM_IPCM3_BASE_ADDR                            (0xF4B5B000)

/* Size: 4K, End at: 0xF4B5AFFF */
#define SOC_ACPU_MDM_IPCM2_BASE_ADDR                            (0xF4B5A000)

/* Size: 4K, End at: 0xF4B59FFF */
#define SOC_ACPU_MDM_IPCM1_BASE_ADDR                            (0xF4B59000)

/* Size: 4K, End at: 0xF4B58FFF */
#define SOC_ACPU_MDM_IPCM0_BASE_ADDR                            (0xF4B58000)

/* Size: 4K, End at: 0xF4B57FFF */
#define SOC_ACPU_CICOM_BASE_ADDR                                (0xF4B57000)

/* Size: 4K, End at: 0xF4B54FFF */
#define SOC_ACPU_UPACC_BASE_ADDR                                (0xF4B54000)

/* Size: 16K, End at: 0xF4B53FFF */
#define SOC_ACPU_CIPHER_BASE_ADDR                               (0xF4B50000)

/* Size: 64K, End at: 0xF4B4FFFF */
#define SOC_ACPU_L2DLE_BASE_ADDR                                (0xF4B40000)

/* Size: 32K, End at: 0xF4B0FFFF */
#define SOC_ACPU_MDM_EICC1_BASE_ADDR                            (0xF4B08000)

/* Size: 32K, End at: 0xF4B07FFF */
#define SOC_ACPU_MDM_EICC0_BASE_ADDR                            (0xF4B00000)

/* Size: 8K, End at: 0xF4A3BFFF */
#define SOC_ACPU_MDM_SYSCNT_BASE_ADDR                           (0xF4A3A000)

/* Size: 4K, End at: 0xF4A39FFF */
#define SOC_ACPU_MDM_EDMA1_BASE_ADDR                            (0xF4A39000)

/* Size: 4K, End at: 0xF4A38FFF */
#define SOC_ACPU_MDM_EDMA0_BASE_ADDR                            (0xF4A38000)

/* Size: 4K, End at: 0xF4A33FFF */
#define SOC_ACPU_LATENCYM_BASE_ADDR                             (0xF4A33000)

/* Size: 4K, End at: 0xF4A32FFF */
#define SOC_ACPU_RRT_BASE_ADDR                                  (0xF4A32000)

/* Size: 4K, End at: 0xF4A2DFFF */
#define SOC_ACPU_MDM_LOAD_MON1_BASE_ADDR                        (0xF4A2D000)

/* Size: 4K, End at: 0xF4A2CFFF */
#define SOC_ACPU_MDM_LOAD_MON0_BASE_ADDR                        (0xF4A2C000)

/* Size: 4K, End at: 0xF4A2AFFF */
#define SOC_ACPU_MDM_RTC_BASE_ADDR                              (0xF4A2A000)

/* Size: 4K, End at: 0xF4A29FFF */
#define SOC_ACPU_MDM_PM_CTRL0_BASE_ADDR                         (0xF4A29000)

/* Size: 4K, End at: 0xF4A28FFF */
#define SOC_ACPU_MDM_PM_CTRL1_BASE_ADDR                         (0xF4A28000)

/* Size: 4K, End at: 0xF4A27FFF */
#define SOC_ACPU_MDM_UART1_BASE_ADDR                            (0xF4A27000)

/* Size: 4K, End at: 0xF4A26FFF */
#define SOC_ACPU_MDM_UART0_BASE_ADDR                            (0xF4A26000)

/* Size: 4K, End at: 0xF4A24FFF */
#define SOC_ACPU_MDM_TIMER15_BASE_ADDR                          (0xF4A24000)

/* Size: 4K, End at: 0xF4A23FFF */
#define SOC_ACPU_MDM_TIMER14_BASE_ADDR                          (0xF4A23000)

/* Size: 4K, End at: 0xF4A22FFF */
#define SOC_ACPU_MDM_TIMER13_BASE_ADDR                          (0xF4A22000)

/* Size: 4K, End at: 0xF4A21FFF */
#define SOC_ACPU_MDM_TIMER12_BASE_ADDR                          (0xF4A21000)

/* Size: 4K, End at: 0xF4A20FFF */
#define SOC_ACPU_MDM_TIMER11_BASE_ADDR                          (0xF4A20000)

/* Size: 4K, End at: 0xF4A1FFFF */
#define SOC_ACPU_MDM_TIMER10_BASE_ADDR                          (0xF4A1F000)

/* Size: 4K, End at: 0xF4A1EFFF */
#define SOC_ACPU_MDM_TIMER9_BASE_ADDR                           (0xF4A1E000)

/* Size: 4K, End at: 0xF4A1DFFF */
#define SOC_ACPU_MDM_TIMER8_BASE_ADDR                           (0xF4A1D000)

/* Size: 4K, End at: 0xF4A1CFFF */
#define SOC_ACPU_MDM_TIMER7_BASE_ADDR                           (0xF4A1C000)

/* Size: 4K, End at: 0xF4A1BFFF */
#define SOC_ACPU_MDM_TIMER6_BASE_ADDR                           (0xF4A1B000)

/* Size: 4K, End at: 0xF4A1AFFF */
#define SOC_ACPU_MDM_TIMER5_BASE_ADDR                           (0xF4A1A000)

/* Size: 4K, End at: 0xF4A19FFF */
#define SOC_ACPU_MDM_TIMER4_BASE_ADDR                           (0xF4A19000)

/* Size: 4K, End at: 0xF4A18FFF */
#define SOC_ACPU_MDM_TIMER3_BASE_ADDR                           (0xF4A18000)

/* Size: 4K, End at: 0xF4A17FFF */
#define SOC_ACPU_MDM_TIMER2_BASE_ADDR                           (0xF4A17000)

/* Size: 4K, End at: 0xF4A16FFF */
#define SOC_ACPU_MDM_TIMER1_BASE_ADDR                           (0xF4A16000)

/* Size: 4K, End at: 0xF4A15FFF */
#define SOC_ACPU_MDM_TIMER0_BASE_ADDR                           (0xF4A15000)

/* Size: 4K, End at: 0xF4A13FFF */
#define SOC_ACPU_MDM_WDT15_BASE_ADDR                            (0xF4A13000)

/* Size: 4K, End at: 0xF4A12FFF */
#define SOC_ACPU_MDM_WDT14_BASE_ADDR                            (0xF4A12000)

/* Size: 4K, End at: 0xF4A11FFF */
#define SOC_ACPU_MDM_WDT13_BASE_ADDR                            (0xF4A11000)

/* Size: 4K, End at: 0xF4A10FFF */
#define SOC_ACPU_MDM_WDT12_BASE_ADDR                            (0xF4A10000)

/* Size: 4K, End at: 0xF4A0FFFF */
#define SOC_ACPU_MDM_WDT11_BASE_ADDR                            (0xF4A0F000)

/* Size: 4K, End at: 0xF4A0EFFF */
#define SOC_ACPU_MDM_WDT10_BASE_ADDR                            (0xF4A0E000)

/* Size: 4K, End at: 0xF4A0DFFF */
#define SOC_ACPU_MDM_WDT9_BASE_ADDR                             (0xF4A0D000)

/* Size: 4K, End at: 0xF4A0CFFF */
#define SOC_ACPU_MDM_WDT8_BASE_ADDR                             (0xF4A0C000)

/* Size: 4K, End at: 0xF4A0BFFF */
#define SOC_ACPU_MDM_WDT7_BASE_ADDR                             (0xF4A0B000)

/* Size: 4K, End at: 0xF4A0AFFF */
#define SOC_ACPU_MDM_WDT6_BASE_ADDR                             (0xF4A0A000)

/* Size: 4K, End at: 0xF4A09FFF */
#define SOC_ACPU_MDM_WDT5_BASE_ADDR                             (0xF4A09000)

/* Size: 4K, End at: 0xF4A08FFF */
#define SOC_ACPU_MDM_WDT4_BASE_ADDR                             (0xF4A08000)

/* Size: 4K, End at: 0xF4A07FFF */
#define SOC_ACPU_MDM_WDT3_BASE_ADDR                             (0xF4A07000)

/* Size: 4K, End at: 0xF4A06FFF */
#define SOC_ACPU_MDM_WDT2_BASE_ADDR                             (0xF4A06000)

/* Size: 4K, End at: 0xF4A05FFF */
#define SOC_ACPU_MDM_WDT1_BASE_ADDR                             (0xF4A05000)

/* Size: 4K, End at: 0xF4A04FFF */
#define SOC_ACPU_MDM_WDT0_BASE_ADDR                             (0xF4A04000)

/* Size: 4K, End at: 0xF4A02FFF */
#define SOC_ACPU_MDM_AUTO_DFS_BASE_ADDR                         (0xF4A02000)

/* Size: 4K, End at: 0xF4A01FFF */
#define SOC_ACPU_SYS_MDM_BASE_ADDR                              (0xF4A01000)

/* Size: 4K, End at: 0xF4A00FFF */
#define SOC_ACPU_CRG_MDM_BASE_ADDR                              (0xF4A00000)

/* Size: 4K, End at: 0xF42D0FFF */
#define SOC_ACPU_SYS_BBP_APB_BASE_ADDR                          (0xF42D0000)

/* Size: 64K, End at: 0xF428FFFF */
#define SOC_ACPU_NOC_BBPBUS_SERVICE_TARGET_BASE_ADDR            (0xF4280000)

/* Size: 64M, End at: 0xF3FFFFFF */
#define SOC_ACPU_BBP_BASE_ADDR                                  (0xF0000000)


/* =====================  HSDT_SUBSYS (100MB) PART 2  ===================== */
/* Size: 4M, End at: 0xEC3FFFFF */
#define SOC_ACPU_PCIECtrl0_DBI_BASE_ADDR                        (0xEC000000)

/* Size: 32M, End at: 0xEBFFFFFF */
#define SOC_ACPU_PCIECtrl0_BASE_ADDR                            (0xEA000000)


/* =========================  MEDIA2_SUBSYS(8MB)  ========================= */
/* Size: 1M, End at: 0xE95FFFFF */
#define SOC_ACPU_HIEPS_BASE_ADDR                                (0xE9500000)

/* Size: 64K, End at: 0xE93DFFFF */
#define SOC_ACPU_NOC_EPS_Service_Target_BASE_ADDR               (0xE93D0000)

/* Size: 64K, End at: 0xE93BFFFF */
#define SOC_ACPU_NOC_VENC_Service_Target_BASE_ADDR              (0xE93B0000)

/* Size: 64K, End at: 0xE93AFFFF */
#define SOC_ACPU_NOC_VDEC_Service_Target_BASE_ADDR              (0xE93A0000)

/* Size: 64K, End at: 0xE939FFFF */
#define SOC_ACPU_NOC_VCODECBUS_Service_Target_BASE_ADDR         (0xE9390000)

/* Size: 64K, End at: 0xE938FFFF */
#define SOC_ACPU_NOC_IVP32_Service_Target_BASE_ADDR             (0xE9380000)

/* Size: 4K, End at: 0xE9301FFF */
#define SOC_ACPU_MEDAI2_LoadMonitor_BASE_ADDR                   (0xE9301000)

/* Size: 4K, End at: 0xE9300FFF */
#define SOC_ACPU_MEDIA2_CRG_BASE_ADDR                           (0xE9300000)

/* Size: 256K, End at: 0xE92BFFFF */
#define SOC_ACPU_VENC_BASE_ADDR                                 (0xE9280000)

/* Size: 256K, End at: 0xE923FFFF */
#define SOC_ACPU_VDEC_BASE_ADDR                                 (0xE9200000)

/* Size: 64K, End at: 0xE91CFFFF */
#define SOC_ACPU_IVP32_SMMU_BASE_ADDR                           (0xE91C0000)

/* Size: 4K, End at: 0xE9183FFF */
#define SOC_ACPU_IVP32_TIMER1_BASE_ADDR                         (0xE9183000)

/* Size: 4K, End at: 0xE9182FFF */
#define SOC_ACPU_IVP32_TIMER0_BASE_ADDR                         (0xE9182000)

/* Size: 4K, End at: 0xE9181FFF */
#define SOC_ACPU_IVP32_WDG_BASE_ADDR                            (0xE9181000)

/* Size: 4K, End at: 0xE9180FFF */
#define SOC_ACPU_IVP32_CFG_BASE_ADDR                            (0xE9180000)

/* Size: 32K, End at: 0xE9107FFF */
#define SOC_ACPU_IVP32_IRAM_BASE_ADDR                           (0xE9100000)

/* Size: 144K, End at: 0xE9063FFF */
#define SOC_ACPU_IVP32_DRAM1_BASE_ADDR                          (0xE9040000)

/* Size: 144K, End at: 0xE9023FFF */
#define SOC_ACPU_IVP32_DRAM0_BASE_ADDR                          (0xE9000000)


/* ============================  MEDIA1(16MB)  ============================ */
/* Size: 64K, End at: 0xE86AFFFF */
#define SOC_ACPU_NOC_VIVO_Service_Target_BASE_ADDR              (0xE86A0000)

/* Size: 64K, End at: 0xE869FFFF */
#define SOC_ACPU_NOC_ISP_Service_Target_BASE_ADDR               (0xE8690000)

/* Size: 64K, End at: 0xE868FFFF */
#define SOC_ACPU_NOC_DSS_Service_Target_BASE_ADDR               (0xE8680000)

/* Size: 4K, End at: 0xE8608FFF */
#define SOC_ACPU_CSI_E_BASE_ADDR                                (0xE8608000)

/* Size: 4K, End at: 0xE8607FFF */
#define SOC_ACPU_CSI_D_BASE_ADDR                                (0xE8607000)

/* Size: 4K, End at: 0xE8606FFF */
#define SOC_ACPU_CSI_C_BASE_ADDR                                (0xE8606000)

/* Size: 4K, End at: 0xE8605FFF */
#define SOC_ACPU_CSI_B_BASE_ADDR                                (0xE8605000)

/* Size: 4K, End at: 0xE8604FFF */
#define SOC_ACPU_CSI_A_BASE_ADDR                                (0xE8604000)

/* Size: 4K, End at: 0xE8603FFF */
#define SOC_ACPU_CSI_adapter_BASE_ADDR                          (0xE8603000)

/* Size: 4K, End at: 0xE8601FFF */
#define SOC_ACPU_MEDIA1_CRG_BASE_ADDR                           (0xE8601000)

/* Size: 4K, End at: 0xE8600FFF */
#define SOC_ACPU_MEDIA1_LoadMonitor_BASE_ADDR                   (0xE8600000)

/* Size: 40K, End at: 0xE84F7FFF */
#define SOC_ACPU_GMP_LUT1_BASE_ADDR                             (0xE84EE000)

/* Size: 40K, End at: 0xE84E7FFF */
#define SOC_ACPU_GMP_LUT0_BASE_ADDR                             (0xE84DE000)

/* Size: 4K, End at: 0xE84BEFFF */
#define SOC_ACPU_CROSSBAR_BASE_ADDR                             (0xE84BE000)

/* Size: 112K, End at: 0xE84BDFFF */
#define SOC_ACPU_DISP_CH1_BASE_ADDR                             (0xE84A2000)

/* Size: 4K, End at: 0xE84A1FFF */
#define SOC_ACPU_DISP_GLB_BASE_ADDR                             (0xE84A1000)

/* Size: 4K, End at: 0xE84A0FFF */
#define SOC_ACPU_WB_BASE_ADDR                                   (0xE84A0000)

/* Size: 64K, End at: 0xE849FFFF */
#define SOC_ACPU_DSS_PRO_BASE_ADDR                              (0xE8490000)

/* Size: 64K, End at: 0xE848FFFF */
#define SOC_ACPU_DBCU0_SMMU_BASE_ADDR                           (0xE8480000)

/* Size: 112K, End at: 0xE847DFFF */
#define SOC_ACPU_DISP_CH0_BASE_ADDR                             (0xE8462000)

/* Size: 1K, End at: 0xE8460BFF */
#define SOC_ACPU_OV2_BASE_ADDR                                  (0xE8460800)

/* Size: 1K, End at: 0xE84603FF */
#define SOC_ACPU_OV0_BASE_ADDR                                  (0xE8460000)

/* Size: 16K, End at: 0xE845FFFF */
#define SOC_ACPU_WCH1_BASE_ADDR                                 (0xE845C000)

/* Size: 4K, End at: 0xE8452FFF */
#define SOC_ACPU_RCH_D0_BASE_ADDR                               (0xE8452000)

/* Size: 4K, End at: 0xE8451FFF */
#define SOC_ACPU_RCH_D3_BASE_ADDR                               (0xE8451000)

/* Size: 4K, End at: 0xE8450FFF */
#define SOC_ACPU_RCH_D2_BASE_ADDR                               (0xE8450000)

/* Size: 32K, End at: 0xE843FFFF */
#define SOC_ACPU_RCH_G0_BASE_ADDR                               (0xE8438000)

/* Size: 32K, End at: 0xE842FFFF */
#define SOC_ACPU_RCH_V1_BASE_ADDR                               (0xE8428000)

/* Size: 32K, End at: 0xE8427FFF */
#define SOC_ACPU_RCH_V0_BASE_ADDR                               (0xE8420000)

/* Size: 4K, End at: 0xE8412FFF */
#define SOC_ACPU_GLB_BASE_ADDR                                  (0xE8412000)

/* Size: 4K, End at: 0xE8411FFF */
#define SOC_ACPU_DBUG_BASE_ADDR                                 (0xE8411000)

/* Size: 2K, End at: 0xE8410FFF */
#define SOC_ACPU_MCTL_MUTEX_BASE_ADDR                           (0xE8410800)

/* Size: 2K, End at: 0xE84107FF */
#define SOC_ACPU_MCTL_SYS_BASE_ADDR                             (0xE8410000)

/* Size: 4K, End at: 0xE840AFFF */
#define SOC_ACPU_DBCU0_MIF_BASE_ADDR                            (0xE840A000)

/* Size: 4K, End at: 0xE8409FFF */
#define SOC_ACPU_DBCU1_AIF_BASE_ADDR                            (0xE8409000)

/* Size: 4K, End at: 0xE8407FFF */
#define SOC_ACPU_DBCU0_AIF_BASE_ADDR                            (0xE8407000)

/* Size: 5K, End at: 0xE84033FF */
#define SOC_ACPU_CMD_BASE_ADDR                                  (0xE8402000)

/* Size: 2K, End at: 0xE8401FFF */
#define SOC_ACPU_ASC_BASE_ADDR                                  (0xE8401800)

/* Size: 1K, End at: 0xE84013FF */
#define SOC_ACPU_DSI0_BASE_ADDR                                 (0xE8401000)

/* Size: 4K, End at: 0xE8400FFF */
#define SOC_ACPU_MMBUF_CFG_BASE_ADDR                            (0xE8400000)

/* Size: 64K, End at: 0xE83FFFFF */
#define SOC_ACPU_SMMUv500_CFG_BASE_ADDR                         (0xE83F0000)

/* Size: 8K, End at: 0xE83EFFFF */
#define SOC_ACPU_ISP_R8_PERI_BASE_BASE_ADDR                     (0xE83EE000)

/* Size: 4K, End at: 0xE83E4FFF */
#define SOC_ACPU_ISP_TCMDMA_BASE_ADDR                           (0xE83E4000)

/* Size: 4K, End at: 0xE83E3FFF */
#define SOC_ACPU_ISP_SUB_CTRL_BASE_ADDR                         (0xE83E3000)

/* Size: 4K, End at: 0xE83E2FFF */
#define SOC_ACPU_ISP_IPC_BASE_ADDR                              (0xE83E2000)

/* Size: 4K, End at: 0xE83E1FFF */
#define SOC_ACPU_ISP_TIMER_BASE_ADDR                            (0xE83E1000)

/* Size: 4K, End at: 0xE83E0FFF */
#define SOC_ACPU_ISP_WDT_BASE_ADDR                              (0xE83E0000)

/* Size: 64K, End at: 0xE83DFFFF */
#define SOC_ACPU_ISP_R8_DTCM_BASE_ADDR                          (0xE83D0000)

/* Size: 32K, End at: 0xE83C7FFF */
#define SOC_ACPU_ISP_R8_ITCM_BASE_ADDR                          (0xE83C0000)

/* Size: 1.75M, End at: 0xE83BFFFF */
#define SOC_ACPU_ISP_Core_CFG_BASE_ADDR                         (0xE8200000)

/* Size: 128K, End at: 0xE803FFFF */
#define SOC_ACPU_SMMU_BASE_ADDR                                 (0xE8020000)

/* Size: 4K, End at: 0xE800CFFF */
#define SOC_ACPU_ORB_BASE_ADDR                                  (0xE800C000)

/* Size: 8K, End at: 0xE8007FFF */
#define SOC_ACPU_CVDR_BASE_ADDR                                 (0xE8006000)

/* Size: 4K, End at: 0xE8005FFF */
#define SOC_ACPU_CMDLST_BASE_ADDR                               (0xE8005000)

/* Size: 4K, End at: 0xE8004FFF */
#define SOC_ACPU_TOP_BASE_ADDR                                  (0xE8004000)

/* Size: 4K, End at: 0xE8000FFF */
#define SOC_ACPU_JPGENC_BASE_ADDR                               (0xE8000000)


/* =============================  NPU(32MB)  ============================== */
/* Size: 256K, End at: 0xE5F7FFFF */
#define SOC_ACPU_NPU_TCU_BASE_ADDR                              (0xE5F40000)

/* Size: 128K, End at: 0xE5F3FFFF */
#define SOC_ACPU_SYSDMA_TBU_BASE_ADDR                           (0xE5F20000)

/* Size: 128K, End at: 0xE5F1FFFF */
#define SOC_ACPU_AIC_TBU_BASE_ADDR                              (0xE5F00000)

/* Size: 128K, End at: 0xE5EFFFFF */
#define SOC_ACPU_sysdma_cfg_BASE_ADDR                           (0xE5EE0000)

/* Size: 4K, End at: 0xE5E07FFF */
#define SOC_ACPU_npu_etzpc_BASE_ADDR                            (0xE5E07000)

/* Size: 4K, End at: 0xE5E06FFF */
#define SOC_ACPU_npu_hw_exp_irq_BASE_ADDR                       (0xE5E06000)

/* Size: 4K, End at: 0xE5E05FFF */
#define SOC_ACPU_npu_uart_BASE_ADDR                             (0xE5E05000)

/* Size: 4K, End at: 0xE5E04FFF */
#define SOC_ACPU_npu_sysctrl_BASE_ADDR                          (0xE5E04000)

/* Size: 4K, End at: 0xE5E03FFF */
#define SOC_ACPU_npu_crg_BASE_ADDR                              (0xE5E03000)

/* Size: 4K, End at: 0xE5E02FFF */
#define SOC_ACPU_npu_tzpc_BASE_ADDR                             (0xE5E02000)

/* Size: 4K, End at: 0xE5E01FFF */
#define SOC_ACPU_npu_ipc_ns_BASE_ADDR                           (0xE5E01000)

/* Size: 4K, End at: 0xE5E00FFF */
#define SOC_ACPU_npu_ipc_s_BASE_ADDR                            (0xE5E00000)

/* Size: 2M, End at: 0xE5DFFFFF */
#define SOC_ACPU_gic_cfg_BASE_ADDR                              (0xE5C00000)

/* Size: 1024K, End at: 0xE50FFFFF */
#define SOC_ACPU_aic0_subsys_cfg_BASE_ADDR                      (0xE5000000)

/* Size: 64K, End at: 0xE4D3FFFF */
#define SOC_ACPU_npu_bus_service_target_BASE_ADDR               (0xE4D30000)

/* Size: 64K, End at: 0xE423FFFF */
#define SOC_ACPU_SRAM_reg_BASE_ADDR                             (0xE4230000)

/* Size: 64K, End at: 0xE422FFFF */
#define SOC_ACPU_SRAM_atomicdec_BASE_ADDR                       (0xE4220000)

/* Size: 64K, End at: 0xE421FFFF */
#define SOC_ACPU_SRAM_atomicadd_BASE_ADDR                       (0xE4210000)

/* Size: 64K, End at: 0xE420FFFF */
#define SOC_ACPU_SRAM_normal_BASE_ADDR                          (0xE4200000)

/* Size: 64K, End at: 0xE411FFFF */
#define SOC_ACPU_bs_slv_BASE_ADDR                               (0xE4110000)

/* Size: 4K, End at: 0xE4100FFF */
#define SOC_ACPU_doorbell_sts_BASE_ADDR                         (0xE4100000)

/* Size: 512K, End at: 0xE40FFFFF */
#define SOC_ACPU_doorbell_cfg_BASE_ADDR                         (0xE4080000)

/* Size: 4K, End at: 0xE4037FFF */
#define SOC_ACPU_npu_cpu_secure_BASE_ADDR                       (0xE4037000)

/* Size: 4K, End at: 0xE4036FFF */
#define SOC_ACPU_axi_mon_soc_BASE_ADDR                          (0xE4036000)

/* Size: 4K, End at: 0xE4035FFF */
#define SOC_ACPU_axi_mon_bs_BASE_ADDR                           (0xE4035000)

/* Size: 4K, End at: 0xE4034FFF */
#define SOC_ACPU_axi_mon_cpu_BASE_ADDR                          (0xE4034000)

/* Size: 4K, End at: 0xE4032FFF */
#define SOC_ACPU_ai_cpu_local_ctrl_BASE_ADDR                    (0xE4032000)

/* Size: 4K, End at: 0xE4031FFF */
#define SOC_ACPU_ts_cpu_local_ctrl_BASE_ADDR                    (0xE4031000)

/* Size: 4K, End at: 0xE4030FFF */
#define SOC_ACPU_sysctrl_npu_cpu_BASE_ADDR                      (0xE4030000)

/* Size: 4K, End at: 0xE4022FFF */
#define SOC_ACPU_fcm_local_ctrl_BASE_ADDR                       (0xE4022000)

/* Size: 4K, End at: 0xE4021FFF */
#define SOC_ACPU_ts_cpu_a55_ctrl_BASE_ADDR                      (0xE4021000)

/* Size: 4K, End at: 0xE4011FFF */
#define SOC_ACPU_ts_cpu_timer_BASE_ADDR                         (0xE4011000)

/* Size: 4K, End at: 0xE4010FFF */
#define SOC_ACPU_ai_cpu_timer_BASE_ADDR                         (0xE4010000)

/* Size: 4K, End at: 0xE4001FFF */
#define SOC_ACPU_ts_cpu_watchdog_BASE_ADDR                      (0xE4001000)

/* Size: 4K, End at: 0xE4000FFF */
#define SOC_ACPU_ai_cpu_watchdog_BASE_ADDR                      (0xE4000000)


/* ================================  DRAM  ================================ */
/* Size: 3584M, End at: 0xDFFFFFFF */
#define SOC_ACPU_DRAM_BASE_ADDR                                 (0x00000000)



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

#endif /* end of soc_acpu_baseaddr_interface.h */
