/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_iomcu_baseaddr_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_IOMCU_BASEADDR_INTERFACE_H__
#define __SOC_IOMCU_BASEADDR_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/


/* =======================  M7 Private Peripheral  ======================== */
/* Size: 4K, End at: 0xE00FFFFF */
#define SOC_IOMCU_PPB_ROM_table_BASE_ADDR                   (0xE00FF000)

/* Size: 4K, End at: 0xE00FEFFF */
#define SOC_IOMCU_Processor_ROM_table_BASE_ADDR             (0xE00FE000)

/* Size: 4K, End at: 0xE0043FFF */
#define SOC_IOMCU_Funnel_BASE_ADDR                          (0xE0043000)

/* Size: 4K, End at: 0xE0042FFF */
#define SOC_IOMCU_CTI_BASE_ADDR                             (0xE0042000)

/* Size: 4K, End at: 0xE0041FFF */
#define SOC_IOMCU_ETM_BASE_ADDR                             (0xE0041000)

/* Size: 4K, End at: 0xE000EFFF */
#define SOC_IOMCU_SCS_BASE_ADDR                             (0xE000E000)

/* Size: 4K, End at: 0xE0002FFF */
#define SOC_IOMCU_FPB_BASE_ADDR                             (0xE0002000)

/* Size: 4K, End at: 0xE0001FFF */
#define SOC_IOMCU_DWT_BASE_ADDR                             (0xE0001000)

/* Size: 4K, End at: 0xE0000FFF */
#define SOC_IOMCU_ITM_BASE_ADDR                             (0xE0000000)


/* ===============================  <N/A>  ================================ */
/* Size: 512M, End at: 0x7FFFFFFF */
#define SOC_IOMCU_DRAM_BASE_ADDR                            (0x60000000)


/* ================================  BOOT  ================================ */
/* Size: 96K, End at: 0x5FFF7FFF */
#define SOC_IOMCU_BOOTROM_BASE_ADDR                         (0x5FFE0000)

/* Size: 192K, End at: 0x5FFA7FFF */
#define SOC_IOMCU_PERI_LPRAM_BASE_ADDR                      (0x5FF78000)

/* Size: 160K, End at: 0x5FF77FFF */
#define SOC_IOMCU_AO_LPRAM_BASE_ADDR                        (0x5FF50000)


/* =========================  LP_APB_PERI (64KB)  ========================= */
/* Size: 4K, End at: 0x5FF0FFFF */
#define SOC_IOMCU_LP_CONFIG_BASE_ADDR                       (0x5FF0F000)

/* Size: 4K, End at: 0x5FF0EFFF */
#define SOC_IOMCU_LP_TIMER_BASE_ADDR                        (0x5FF0E000)

/* Size: 4K, End at: 0x5FF0DFFF */
#define SOC_IOMCU_LP_WDG_BASE_ADDR                          (0x5FF0D000)

/* Size: 4K, End at: 0x5FF09FFF */
#define SOC_IOMCU_CORE_CRG_B_M_BASE_ADDR                    (0x5FF09000)

/* Size: 4K, End at: 0x5FF08FFF */
#define SOC_IOMCU_CORE_CRG_L_L3_BASE_ADDR                   (0x5FF08000)

/* Size: 4K, End at: 0x5FF07FFF */
#define SOC_IOMCU_LP_PCTRL_BASE_ADDR                        (0x5FF07000)

/* Size: 4K, End at: 0x5FF05FFF */
#define SOC_IOMCU_PERI_CRG_BASE_ADDR                        (0x5FF05000)

/* Size: 4K, End at: 0x5FF04FFF */
#define SOC_IOMCU_APB2SPI_BASE_ADDR                         (0x5FF04000)

/* Size: 4K, End at: 0x5FF02FFF */
#define SOC_IOMCU_UART6_BASE_ADDR                           (0x5FF02000)

/* Size: 4K, End at: 0x5FF01FFF */
#define SOC_IOMCU_PMC_BASE_ADDR                             (0x5FF01000)

/* Size: 4K, End at: 0x5FF00FFF */
#define SOC_IOMCU_TSENSORC_BASE_ADDR                        (0x5FF00000)


/* ===========================  DMSS+DMACPACK  ============================ */
/* Size: 256K, End at: 0x5FEBFFFF */
#define SOC_IOMCU_DMSS_BASE_ADDR                            (0x5FE80000)

/* Size: 128K, End at: 0x5FE3FFFF */
#define SOC_IOMCU_DMCPACK1_BASE_ADDR                        (0x5FE20000)

/* Size: 128K, End at: 0x5FE1FFFF */
#define SOC_IOMCU_DMCPACK0_BASE_ADDR                        (0x5FE00000)


/* ==========================  CFG_SUBSYS(4MB)  =========================== */
/* Size: 4K, End at: 0x5ED3FFFF */
#define SOC_IOMCU_CODEC_SSI_BASE_ADDR                       (0x5ED3F000)

/* Size: 64K, End at: 0x5ED2FFFF */
#define SOC_IOMCU_IPC_NS_BASE_ADDR                          (0x5ED20000)

/* Size: 64K, End at: 0x5ED1FFFF */
#define SOC_IOMCU_IPC_BASE_ADDR                             (0x5ED10000)

/* Size: 4K, End at: 0x5ED02FFF */
#define SOC_IOMCU_IOC_BASE_ADDR                             (0x5ED02000)

/* Size: 4K, End at: 0x5EC41FFF */
#define SOC_IOMCU_LoadMonitor1_BASE_ADDR                    (0x5EC41000)

/* Size: 4K, End at: 0x5EC40FFF */
#define SOC_IOMCU_LoadMonitor0_BASE_ADDR                    (0x5EC40000)

/* Size: 4K, End at: 0x5EC3EFFF */
#define SOC_IOMCU_PCTRL_BASE_ADDR                           (0x5EC3E000)

/* Size: 4K, End at: 0x5EC3DFFF */
#define SOC_IOMCU_ATGC_BASE_ADDR                            (0x5EC3D000)

/* Size: 4K, End at: 0x5EC3CFFF */
#define SOC_IOMCU_TZPC_BASE_ADDR                            (0x5EC3C000)

/* Size: 4K, End at: 0x5EC39FFF */
#define SOC_IOMCU_ATGC1_BASE_ADDR                           (0x5EC39000)

/* Size: 4K, End at: 0x5EC37FFF */
#define SOC_IOMCU_WD1_BASE_ADDR                             (0x5EC37000)

/* Size: 4K, End at: 0x5EC36FFF */
#define SOC_IOMCU_WD0_BASE_ADDR                             (0x5EC36000)

/* Size: 4K, End at: 0x5EC34FFF */
#define SOC_IOMCU_BLPWM_BASE_ADDR                           (0x5EC34000)

/* Size: 8K, End at: 0x5EC31FFF */
#define SOC_IOMCU_GPUPCR_BASE_ADDR                          (0x5EC30000)

/* Size: 4K, End at: 0x5EC2FFFF */
#define SOC_IOMCU_TIMER12_BASE_ADDR                         (0x5EC2F000)

/* Size: 4K, End at: 0x5EC2EFFF */
#define SOC_IOMCU_TIMER11_BASE_ADDR                         (0x5EC2E000)

/* Size: 4K, End at: 0x5EC2DFFF */
#define SOC_IOMCU_TIMER10_BASE_ADDR                         (0x5EC2D000)

/* Size: 4K, End at: 0x5EC2CFFF */
#define SOC_IOMCU_TIMER9_BASE_ADDR                          (0x5EC2C000)

/* Size: 4K, End at: 0x5EC25FFF */
#define SOC_IOMCU_GPIO19_BASE_ADDR                          (0x5EC25000)

/* Size: 4K, End at: 0x5EC24FFF */
#define SOC_IOMCU_GPIO18_BASE_ADDR                          (0x5EC24000)

/* Size: 4K, End at: 0x5EC23FFF */
#define SOC_IOMCU_GPIO17_BASE_ADDR                          (0x5EC23000)

/* Size: 4K, End at: 0x5EC22FFF */
#define SOC_IOMCU_GPIO16_BASE_ADDR                          (0x5EC22000)

/* Size: 4K, End at: 0x5EC21FFF */
#define SOC_IOMCU_GPIO15_BASE_ADDR                          (0x5EC21000)

/* Size: 4K, End at: 0x5EC20FFF */
#define SOC_IOMCU_GPIO14_BASE_ADDR                          (0x5EC20000)

/* Size: 4K, End at: 0x5EC1FFFF */
#define SOC_IOMCU_GPIO13_BASE_ADDR                          (0x5EC1F000)

/* Size: 4K, End at: 0x5EC1EFFF */
#define SOC_IOMCU_GPIO12_BASE_ADDR                          (0x5EC1E000)

/* Size: 4K, End at: 0x5EC1DFFF */
#define SOC_IOMCU_GPIO11_BASE_ADDR                          (0x5EC1D000)

/* Size: 4K, End at: 0x5EC1CFFF */
#define SOC_IOMCU_GPIO10_BASE_ADDR                          (0x5EC1C000)

/* Size: 4K, End at: 0x5EC1BFFF */
#define SOC_IOMCU_GPIO9_BASE_ADDR                           (0x5EC1B000)

/* Size: 4K, End at: 0x5EC1AFFF */
#define SOC_IOMCU_GPIO8_BASE_ADDR                           (0x5EC1A000)

/* Size: 4K, End at: 0x5EC19FFF */
#define SOC_IOMCU_GPIO7_BASE_ADDR                           (0x5EC19000)

/* Size: 4K, End at: 0x5EC18FFF */
#define SOC_IOMCU_GPIO6_BASE_ADDR                           (0x5EC18000)

/* Size: 4K, End at: 0x5EC17FFF */
#define SOC_IOMCU_GPIO5_BASE_ADDR                           (0x5EC17000)

/* Size: 4K, End at: 0x5EC16FFF */
#define SOC_IOMCU_GPIO4_BASE_ADDR                           (0x5EC16000)

/* Size: 4K, End at: 0x5EC15FFF */
#define SOC_IOMCU_GPIO3_BASE_ADDR                           (0x5EC15000)

/* Size: 4K, End at: 0x5EC14FFF */
#define SOC_IOMCU_GPIO2_BASE_ADDR                           (0x5EC14000)

/* Size: 4K, End at: 0x5EC13FFF */
#define SOC_IOMCU_GPIO1_BASE_ADDR                           (0x5EC13000)

/* Size: 4K, End at: 0x5EC12FFF */
#define SOC_IOMCU_GPIO0_BASE_ADDR                           (0x5EC12000)

/* Size: 4K, End at: 0x5EC10FFF */
#define SOC_IOMCU_GPIO0_SE_BASE_ADDR                        (0x5EC10000)

/* Size: 64K, End at: 0x5EC0FFFF */
#define SOC_IOMCU_fcm_service_target_BASE_ADDR              (0x5EC00000)

/* Size: 4K, End at: 0x5EB07FFF */
#define SOC_IOMCU_FCM_GIC_HW_SPI1_BASE_ADDR                 (0x5EB07000)

/* Size: 4K, End at: 0x5EB05FFF */
#define SOC_IOMCU_FCM_LOCAL_GIC_BASE_ADDR                   (0x5EB05000)

/* Size: 4K, End at: 0x5EB04FFF */
#define SOC_IOMCU_FCM_GIC_HW_SPI0_BASE_ADDR                 (0x5EB04000)

/* Size: 4K, End at: 0x5EB03FFF */
#define SOC_IOMCU_FCM_LOCAL_PERI_BASE_ADDR                  (0x5EB03000)

/* Size: 4K, End at: 0x5EB02FFF */
#define SOC_IOMCU_FCM_SECURE_BASE_ADDR                      (0x5EB02000)

/* Size: 4K, End at: 0x5EA0CFFF */
#define SOC_IOMCU_FCM_LOCAL_IP_BASE_ADDR                    (0x5EA0C000)

/* Size: 4K, End at: 0x5EA0BFFF */
#define SOC_IOMCU_FCM_CORE7_LOCAL_CFG_BASE_ADDR             (0x5EA0B000)

/* Size: 4K, End at: 0x5EA0AFFF */
#define SOC_IOMCU_FCM_CORE6_LOCAL_CFG_BASE_ADDR             (0x5EA0A000)

/* Size: 4K, End at: 0x5EA09FFF */
#define SOC_IOMCU_FCM_CORE5_LOCAL_CFG_BASE_ADDR             (0x5EA09000)

/* Size: 4K, End at: 0x5EA08FFF */
#define SOC_IOMCU_FCM_CORE4_LOCAL_CFG_BASE_ADDR             (0x5EA08000)

/* Size: 4K, End at: 0x5EA07FFF */
#define SOC_IOMCU_FCM_CORE3_LOCAL_CFG_BASE_ADDR             (0x5EA07000)

/* Size: 4K, End at: 0x5EA06FFF */
#define SOC_IOMCU_FCM_CORE2_LOCAL_CFG_BASE_ADDR             (0x5EA06000)

/* Size: 4K, End at: 0x5EA05FFF */
#define SOC_IOMCU_FCM_CORE1_LOCAL_CFG_BASE_ADDR             (0x5EA05000)

/* Size: 4K, End at: 0x5EA04FFF */
#define SOC_IOMCU_FCM_CORE0_LOCAL_CFG_BASE_ADDR             (0x5EA04000)

/* Size: 4K, End at: 0x5EA01FFF */
#define SOC_IOMCU_FCM_LOCAL_CPU_BASE_ADDR                   (0x5EA01000)

/* Size: 2M, End at: 0x5E9FFFFF */
#define SOC_IOMCU_GIC600_BASE_ADDR                          (0x5E800000)


/* ===============================  <N/A>  ================================ */
/* Size: 64K, End at: 0x5E3AFFFF */
#define SOC_IOMCU_MAA_BASE_ADDR                             (0x5E3A0000)

/* Size: 64K, End at: 0x5E29FFFF */
#define SOC_IOMCU_MDM_5G_Service_Target_BASE_ADDR           (0x5E290000)

/* Size: 64K, End at: 0x5E28FFFF */
#define SOC_IOMCU_FD_BUS_Service_Target_BASE_ADDR           (0x5E280000)

/* Size: 64K, End at: 0x5E27FFFF */
#define SOC_IOMCU_AOBUS_Service_Target_BASE_ADDR            (0x5E270000)

/* Size: 64K, End at: 0x5E26FFFF */
#define SOC_IOMCU_DMA_NOC_Service_Target_BASE_ADDR          (0x5E260000)

/* Size: 64K, End at: 0x5E25FFFF */
#define SOC_IOMCU_CFGBUS_Service_Target_BASE_ADDR           (0x5E250000)

/* Size: 64K, End at: 0x5E24FFFF */
#define SOC_IOMCU_SYS_BUS_Service_Target_BASE_ADDR          (0x5E240000)

/* Size: 64K, End at: 0x5E23FFFF */
#define SOC_IOMCU_ASP_Service_Target_BASE_ADDR              (0x5E230000)

/* Size: 64K, End at: 0x5E22FFFF */
#define SOC_IOMCU_Modem_Service_Target_BASE_ADDR            (0x5E220000)

/* Size: 64K, End at: 0x5E21FFFF */
#define SOC_IOMCU_HSDT_Service_Target_BASE_ADDR             (0x5E210000)

/* Size: 64K, End at: 0x5E20FFFF */
#define SOC_IOMCU_MMC0_Service_target_BASE_ADDR             (0x5E200000)


/* =============================  CSSYS_APB  ============================== */
/* Size: 32M, End at: 0x5DFFFFFF */
#define SOC_IOMCU_CSSYS_APB_BASE_ADDR                       (0x5C000000)


/* ===============================  <N/A>  ================================ */
/* Size: 16M, End at: 0x5BFFFFFF */
#define SOC_IOMCU_G3D_BASE_ADDR                             (0x5B000000)

/* Size: 4K, End at: 0x5ABE0FFF */
#define SOC_IOMCU_UFS_SYS_CTRL_BASE_ADDR                    (0x5ABE0000)

/* Size: 128K, End at: 0x5ABDFFFF */
#define SOC_IOMCU_UFS_MCU_BASE_ADDR                         (0x5ABC0000)


/* =======================  AO_SUBSYS(12MB) PART1  ======================== */
/* Size: 1536K, End at: 0x5AB7FFFF */
#define SOC_IOMCU_UFS_CFG_BASE_ADDR                         (0x5AA00000)

/* Size: 32K, End at: 0x5A987FFF */
#define SOC_IOMCU_AO_TCP_BASE_ADDR                          (0x5A980000)

/* Size: 32K, End at: 0x5A8C7FFF */
#define SOC_IOMCU_BB_DRX_BASE_ADDR                          (0x5A8C0000)

/* Size: 4K, End at: 0x5A8B8FFF */
#define SOC_IOMCU_GPIO36_BASE_ADDR                          (0x5A8B8000)

/* Size: 4K, End at: 0x5A8B7FFF */
#define SOC_IOMCU_GPIO35_BASE_ADDR                          (0x5A8B7000)

/* Size: 4K, End at: 0x5A8B6FFF */
#define SOC_IOMCU_GPIO34_BASE_ADDR                          (0x5A8B6000)

/* Size: 4K, End at: 0x5A8B5FFF */
#define SOC_IOMCU_GPIO33_BASE_ADDR                          (0x5A8B5000)

/* Size: 4K, End at: 0x5A8B4FFF */
#define SOC_IOMCU_GPIO32_BASE_ADDR                          (0x5A8B4000)

/* Size: 4K, End at: 0x5A8B3FFF */
#define SOC_IOMCU_GPIO31_BASE_ADDR                          (0x5A8B3000)

/* Size: 4K, End at: 0x5A8B2FFF */
#define SOC_IOMCU_GPIO30_BASE_ADDR                          (0x5A8B2000)

/* Size: 4K, End at: 0x5A8B1FFF */
#define SOC_IOMCU_GPIO29_BASE_ADDR                          (0x5A8B1000)

/* Size: 4K, End at: 0x5A8B0FFF */
#define SOC_IOMCU_GPIO28_BASE_ADDR                          (0x5A8B0000)

/* Size: 4K, End at: 0x5A8AFFFF */
#define SOC_IOMCU_GPIO27_BASE_ADDR                          (0x5A8AF000)

/* Size: 4K, End at: 0x5A8AEFFF */
#define SOC_IOMCU_GPIO26_BASE_ADDR                          (0x5A8AE000)

/* Size: 4K, End at: 0x5A8ADFFF */
#define SOC_IOMCU_GPIO25_BASE_ADDR                          (0x5A8AD000)

/* Size: 4K, End at: 0x5A8ACFFF */
#define SOC_IOMCU_GPIO24_BASE_ADDR                          (0x5A8AC000)

/* Size: 4K, End at: 0x5A8ABFFF */
#define SOC_IOMCU_GPIO23_BASE_ADDR                          (0x5A8AB000)

/* Size: 4K, End at: 0x5A8AAFFF */
#define SOC_IOMCU_GPIO22_BASE_ADDR                          (0x5A8AA000)

/* Size: 4K, End at: 0x5A8A9FFF */
#define SOC_IOMCU_GPIO21_BASE_ADDR                          (0x5A8A9000)

/* Size: 4K, End at: 0x5A8A8FFF */
#define SOC_IOMCU_GPIO20_BASE_ADDR                          (0x5A8A8000)

/* Size: 4K, End at: 0x5A8A5FFF */
#define SOC_IOMCU_SPI5_BASE_ADDR                            (0x5A8A5000)

/* Size: 4K, End at: 0x5A8A4FFF */
#define SOC_IOMCU_EFUSEC2_BASE_ADDR                         (0x5A8A4000)

/* Size: 4K, End at: 0x5A8A3FFF */
#define SOC_IOMCU_TIMER17_BASE_ADDR                         (0x5A8A3000)

/* Size: 4K, End at: 0x5A8A2FFF */
#define SOC_IOMCU_TIMER16_BASE_ADDR                         (0x5A8A2000)

/* Size: 4K, End at: 0x5A8A1FFF */
#define SOC_IOMCU_GPIO1_SE_BASE_ADDR                        (0x5A8A1000)

/* Size: 4K, End at: 0x5A8A0FFF */
#define SOC_IOMCU_AO_WDG_BASE_ADDR                          (0x5A8A0000)

/* Size: 4K, End at: 0x5A89FFFF */
#define SOC_IOMCU_SPI3_BASE_ADDR                            (0x5A89F000)

/* Size: 4K, End at: 0x5A89EFFF */
#define SOC_IOMCU_AO_TZPC_BASE_ADDR                         (0x5A89E000)

/* Size: 4K, End at: 0x5A89DFFF */
#define SOC_IOMCU_AO_LoadMonitor_BASE_ADDR                  (0x5A89D000)

/* Size: 4K, End at: 0x5A89CFFF */
#define SOC_IOMCU_AO_IOC_BASE_ADDR                          (0x5A89C000)

/* Size: 4K, End at: 0x5A89BFFF */
#define SOC_IOMCU_SCTRL_BASE_ADDR                           (0x5A89B000)

/* Size: 4K, End at: 0x5A89AFFF */
#define SOC_IOMCU_EFUSEC_BASE_ADDR                          (0x5A89A000)

/* Size: 4K, End at: 0x5A899FFF */
#define SOC_IOMCU_AO_IPC_NS_BASE_ADDR                       (0x5A899000)

/* Size: 4K, End at: 0x5A898FFF */
#define SOC_IOMCU_AO_IPC_S_BASE_ADDR                        (0x5A898000)

/* Size: 8K, End at: 0x5A897FFF */
#define SOC_IOMCU_SYS_CNT_BASE_ADDR                         (0x5A896000)

/* Size: 16K, End at: 0x5A893FFF */
#define SOC_IOMCU_SPMI_BASE_ADDR                            (0x5A890000)

/* Size: 4K, End at: 0x5A88FFFF */
#define SOC_IOMCU_SCI1_BASE_ADDR                            (0x5A88F000)

/* Size: 4K, End at: 0x5A88EFFF */
#define SOC_IOMCU_SCI0_BASE_ADDR                            (0x5A88E000)

/* Size: 4K, End at: 0x5A88DFFF */
#define SOC_IOMCU_RTC1_BASE_ADDR                            (0x5A88D000)

/* Size: 4K, End at: 0x5A88CFFF */
#define SOC_IOMCU_RTC0_BASE_ADDR                            (0x5A88C000)

/* Size: 4K, End at: 0x5A88BFFF */
#define SOC_IOMCU_TIMER15_BASE_ADDR                         (0x5A88B000)

/* Size: 4K, End at: 0x5A88AFFF */
#define SOC_IOMCU_TIMER14_BASE_ADDR                         (0x5A88A000)

/* Size: 4K, End at: 0x5A889FFF */
#define SOC_IOMCU_TIMER13_BASE_ADDR                         (0x5A889000)

/* Size: 4K, End at: 0x5A888FFF */
#define SOC_IOMCU_TIMER8_BASE_ADDR                          (0x5A888000)

/* Size: 4K, End at: 0x5A887FFF */
#define SOC_IOMCU_TIMER7_BASE_ADDR                          (0x5A887000)

/* Size: 4K, End at: 0x5A886FFF */
#define SOC_IOMCU_TIMER6_BASE_ADDR                          (0x5A886000)

/* Size: 4K, End at: 0x5A885FFF */
#define SOC_IOMCU_TIMER5_BASE_ADDR                          (0x5A885000)

/* Size: 4K, End at: 0x5A884FFF */
#define SOC_IOMCU_TIMER4_BASE_ADDR                          (0x5A884000)

/* Size: 4K, End at: 0x5A883FFF */
#define SOC_IOMCU_TIMER3_BASE_ADDR                          (0x5A883000)

/* Size: 4K, End at: 0x5A882FFF */
#define SOC_IOMCU_TIMER2_BASE_ADDR                          (0x5A882000)

/* Size: 4K, End at: 0x5A881FFF */
#define SOC_IOMCU_TIMER1_BASE_ADDR                          (0x5A881000)

/* Size: 4K, End at: 0x5A880FFF */
#define SOC_IOMCU_TIMER0_BASE_ADDR                          (0x5A880000)

/* Size: 4K, End at: 0x5A87FFFF */
#define SOC_IOMCU_IOMCU_RTC_BASE_ADDR                       (0x5A87F000)

/* Size: 4K, End at: 0x5A87EFFF */
#define SOC_IOMCU_IOMCU_CONFIG_BASE_ADDR                    (0x5A87E000)

/* Size: 4K, End at: 0x5A87DFFF */
#define SOC_IOMCU_IOMCU_TIMER_BASE_ADDR                     (0x5A87D000)

/* Size: 4K, End at: 0x5A87CFFF */
#define SOC_IOMCU_IOMCU_WDG_BASE_ADDR                       (0x5A87C000)

/* Size: 4K, End at: 0x5A87BFFF */
#define SOC_IOMCU_IOMCU_GPIO3_BASE_ADDR                     (0x5A87B000)

/* Size: 4K, End at: 0x5A87AFFF */
#define SOC_IOMCU_IOMCU_GPIO2_BASE_ADDR                     (0x5A87A000)

/* Size: 4K, End at: 0x5A879FFF */
#define SOC_IOMCU_IOMCU_GPIO1_BASE_ADDR                     (0x5A879000)

/* Size: 4K, End at: 0x5A878FFF */
#define SOC_IOMCU_IOMCU_GPIO0_BASE_ADDR                     (0x5A878000)

/* Size: 4K, End at: 0x5A877FFF */
#define SOC_IOMCU_IOMCU_DMAC_BASE_ADDR                      (0x5A877000)

/* Size: 4K, End at: 0x5A876FFF */
#define SOC_IOMCU_IOMCU_UART7_BASE_ADDR                     (0x5A876000)

/* Size: 4K, End at: 0x5A875FFF */
#define SOC_IOMCU_IOMCU_BLPWM_BASE_ADDR                     (0x5A875000)

/* Size: 4K, End at: 0x5A874FFF */
#define SOC_IOMCU_IOMCU_UART3_BASE_ADDR                     (0x5A874000)

/* Size: 4K, End at: 0x5A873FFF */
#define SOC_IOMCU_IOMCU_I3C1_BASE_ADDR                      (0x5A873000)

/* Size: 4K, End at: 0x5A872FFF */
#define SOC_IOMCU_IOMCU_I2C1_BASE_ADDR                      (0x5A872000)

/* Size: 4K, End at: 0x5A871FFF */
#define SOC_IOMCU_IOMCU_I2C0_BASE_ADDR                      (0x5A871000)

/* Size: 4K, End at: 0x5A870FFF */
#define SOC_IOMCU_IOMCU_SPI0_BASE_ADDR                      (0x5A870000)

/* Size: 4K, End at: 0x5A86FFFF */
#define SOC_IOMCU_DTCM1Remap_BASE_ADDR                (0x5A86F000)

/* Size: 4K, End at: 0x5A86EFFF */
#define SOC_IOMCU_DTCM0Remap_BASE_ADDR                (0x5A86E000)

/* Size: 8K, End at: 0x5A86DFFF */
#define SOC_IOMCU_ITCMRemap_BASE_ADDR                 (0x5A86C000)

/* Size: 4K, End at: 0x5A86BFFF */
#define SOC_IOMCU_RemapCtrl_BASE_ADDR                 (0x5A86B000)

/* Size: 4K, End at: 0x5A86AFFF */
#define SOC_IOMCU_IOMCU_I3C2_BASE_ADDR                      (0x5A86A000)

/* Size: 4K, End at: 0x5A869FFF */
#define SOC_IOMCU_IOMCU_UART8_BASE_ADDR                     (0x5A869000)

/* Size: 4K, End at: 0x5A868FFF */
#define SOC_IOMCU_IOMCU_SPI2_BASE_ADDR                      (0x5A868000)

/* Size: 4K, End at: 0x5A867FFF */
#define SOC_IOMCU_IOMCU_DMMU_BASE_ADDR                      (0x5A867000)

/* Size: 4K, End at: 0x5A866FFF */
#define SOC_IOMCU_IOMCU_TIMER2_BASE_ADDR                    (0x5A866000)

/* Size: 4K, End at: 0x5A865FFF */
#define SOC_IOMCU_IOMCU_I3C_BASE_ADDR                       (0x5A865000)

/* Size: 4K, End at: 0x5A864FFF */
#define SOC_IOMCU_IOMCU_I3C3_BASE_ADDR                      (0x5A864000)

/* Size: 4K, End at: 0x5A863FFF */
#define SOC_IOMCU_IOMCU_I2C5_BASE_ADDR                      (0x5A863000)

/* Size: 48K, End at: 0x5A58BFFF */
#define SOC_IOMCU_DSP_ITCM_BASE_ADDR                        (0x5A580000)

/* Size: 160K, End at: 0x5A57FFFF */
#define SOC_IOMCU_DSP_DTCM_BASE_ADDR                        (0x5A558000)

/* Size: 8K, End at: 0x5A553FFF */
#define SOC_IOMCU_ASP_CODEC_BASE_ADDR                       (0x5A552000)

/* Size: 8K, End at: 0x5A551FFF */
#define SOC_IOMCU_SLIMBUS_BASE_ADDR                         (0x5A550000)

/* Size: 1K, End at: 0x5A54FFFF */
#define SOC_IOMCU_DSD_BASE_ADDR                             (0x5A54FC00)

/* Size: 1K, End at: 0x5A54E3FF */
#define SOC_IOMCU_ASP_CFG_BASE_ADDR                         (0x5A54E000)

/* Size: 4K, End at: 0x5A54DFFF */
#define SOC_IOMCU_ASP_WD_BASE_ADDR                          (0x5A54D000)

/* Size: 4K, End at: 0x5A54CFFF */
#define SOC_IOMCU_ASP_IPC_BASE_ADDR                         (0x5A54C000)

/* Size: 4K, End at: 0x5A54BFFF */
#define SOC_IOMCU_ASP_DMAC_BASE_ADDR                        (0x5A54B000)

/* Size: 4K, End at: 0x5A54AFFF */
#define SOC_IOMCU_ASP_TIMER1_BASE_ADDR                      (0x5A54A000)

/* Size: 4K, End at: 0x5A549FFF */
#define SOC_IOMCU_ASP_TIMER0_BASE_ADDR                      (0x5A549000)

/* Size: 4K, End at: 0x5A548FFF */
#define SOC_IOMCU_ASP_GPIO_BASE_ADDR                        (0x5A548000)

/* Size: 4K, End at: 0x5A547FFF */
#define SOC_IOMCU_ASP_DMMU_BASE_ADDR                        (0x5A547000)

/* Size: 256K, End at: 0x5A4BFFFF */
#define SOC_IOMCU_SECRAM_BASE_ADDR                          (0x5A480000)


/* =======================  DMA_PERI_SUBSYS(17MB)  ======================== */
/* Size: 6K, End at: 0x5A0A17FF */
#define SOC_IOMCU_SOCP_BASE_ADDR                            (0x5A0A0000)

/* Size: 32K, End at: 0x5A097FFF */
#define SOC_IOMCU_EICC_BASE_ADDR                            (0x5A090000)

/* Size: 32K, End at: 0x5A08FFFF */
#define SOC_IOMCU_IPF_BASE_ADDR                             (0x5A088000)

/* Size: 16K, End at: 0x5A087FFF */
#define SOC_IOMCU_SPE_BASE_ADDR                             (0x5A084000)

/* Size: 4K, End at: 0x5A081FFF */
#define SOC_IOMCU_IPC_MDM_NS_BASE_ADDR                      (0x5A081000)

/* Size: 4K, End at: 0x5A080FFF */
#define SOC_IOMCU_IPC_MDM_S_BASE_ADDR                       (0x5A080000)

/* Size: 4K, End at: 0x5A057FFF */
#define SOC_IOMCU_LAT_STAT_BASE_ADDR                        (0x5A057000)

/* Size: 4K, End at: 0x5A056FFF */
#define SOC_IOMCU_PERF_STAT_BASE_ADDR                       (0x5A056000)

/* Size: 4K, End at: 0x5A054FFF */
#define SOC_IOMCU_UART0_BASE_ADDR                           (0x5A054000)

/* Size: 4K, End at: 0x5A050FFF */
#define SOC_IOMCU_I3C4_BASE_ADDR                            (0x5A050000)

/* Size: 4K, End at: 0x5A04FFFF */
#define SOC_IOMCU_I2C7_BASE_ADDR                            (0x5A04F000)

/* Size: 4K, End at: 0x5A04EFFF */
#define SOC_IOMCU_I2C6_BASE_ADDR                            (0x5A04E000)

/* Size: 4K, End at: 0x5A04DFFF */
#define SOC_IOMCU_I2C4_BASE_ADDR                            (0x5A04D000)

/* Size: 4K, End at: 0x5A04CFFF */
#define SOC_IOMCU_I2C3_BASE_ADDR                            (0x5A04C000)

/* Size: 4K, End at: 0x5A04BFFF */
#define SOC_IOMCU_I2C9_BASE_ADDR                            (0x5A04B000)

/* Size: 4K, End at: 0x5A049FFF */
#define SOC_IOMCU_SPI4_BASE_ADDR                            (0x5A049000)

/* Size: 4K, End at: 0x5A048FFF */
#define SOC_IOMCU_SPI1_BASE_ADDR                            (0x5A048000)

/* Size: 4K, End at: 0x5A045FFF */
#define SOC_IOMCU_UART5_BASE_ADDR                           (0x5A045000)

/* Size: 4K, End at: 0x5A044FFF */
#define SOC_IOMCU_UART2_BASE_ADDR                           (0x5A044000)

/* Size: 4K, End at: 0x5A041FFF */
#define SOC_IOMCU_UART4_BASE_ADDR                           (0x5A041000)

/* Size: 4K, End at: 0x5A040FFF */
#define SOC_IOMCU_UART1_BASE_ADDR                           (0x5A040000)

/* Size: 4K, End at: 0x5A000FFF */
#define SOC_IOMCU_PERI_DMAC_BASE_ADDR                       (0x5A000000)


/* =========================  MMC0_SUBSYS (2MB)  ========================== */
/* Size: 1K, End at: 0x585873FF */
#define SOC_IOMCU_USB20PHY_BASE_ADDR                        (0x58587000)

/* Size: 4K, End at: 0x58586FFF */
#define SOC_IOMCU_USB20_SYSCTRL_BASE_ADDR                   (0x58586000)

/* Size: 4K, End at: 0x58585FFF */
#define SOC_IOMCU_PERI_IOCG_R_BASE_ADDR                     (0x58585000)

/* Size: 4K, End at: 0x58584FFF */
#define SOC_IOMCU_PERI_IOCG_L_BASE_ADDR                     (0x58584000)

/* Size: 4K, End at: 0x58583FFF */
#define SOC_IOMCU_SD3_BASE_ADDR                             (0x58583000)

/* Size: 4K, End at: 0x58582FFF */
#define SOC_IOMCU_MMC0_SYS_CTRL_BASE_ADDR                   (0x58582000)

/* Size: 4K, End at: 0x58581FFF */
#define SOC_IOMCU_MMC0_CRG_BASE_ADDR                        (0x58581000)

/* Size: 4K, End at: 0x58580FFF */
#define SOC_IOMCU_IOC_MMC0_BASE_ADDR                        (0x58580000)

/* Size: 512K, End at: 0x5847FFFF */
#define SOC_IOMCU_USB_OTG_BASE_ADDR                         (0x58400000)


/* =====================  HSDT_SUBSYS (98MB) PART 1   ===================== */
/* Size: 128K, End at: 0x581FFFFF */
#define SOC_IOMCU_HSDT_TBU_BASE_ADDR                        (0x581E0000)

/* Size: 384K, End at: 0x581DFFFF */
#define SOC_IOMCU_HSDT_TCU_BASE_ADDR                        (0x58180000)

/* Size: 4K, End at: 0x5817FFFF */
#define SOC_IOMCU_HSDT_SYS_CTRL_BASE_ADDR                   (0x5817F000)

/* Size: 4K, End at: 0x5817EFFF */
#define SOC_IOMCU_HSDT_CRG_BASE_ADDR                        (0x5817E000)

/* Size: 4K, End at: 0x5817DFFF */
#define SOC_IOMCU_PCIE_PLL_BASE_ADDR                        (0x5817D000)

/* Size: 4K, End at: 0x58105FFF */
#define SOC_IOMCU_MMC1_IOC_BASE_ADDR                        (0x58105000)

/* Size: 4K, End at: 0x58104FFF */
#define SOC_IOMCU_MMC1_SDIO_BASE_ADDR                       (0x58104000)

/* Size: 4K, End at: 0x58102FFF */
#define SOC_IOMCU_PCIE0_APB_CFG_BASE_ADDR                   (0x58102000)

/* Size: 4K, End at: 0x58101FFF */
#define SOC_IOMCU_CCP_BASE_ADDR                             (0x58101000)

/* Size: 4K, End at: 0x58100FFF */
#define SOC_IOMCU_CCS_BASE_ADDR                             (0x58100000)

/* Size: 64K, End at: 0x580FFFFF */
#define SOC_IOMCU_PCIE_MCU_BASE_ADDR                        (0x580F0000)

/* Size: 512K, End at: 0x5807FFFF */
#define SOC_IOMCU_PCIEPHY0_BASE_ADDR                        (0x58000000)


/* ===========================  MDM_5G(128MB)  ============================ */
/* Size: 384K, End at: 0x56D5FFFF */
#define SOC_IOMCU_TVP_DSS0_L1TCM_GLOBAL_BASE_ADDR           (0x56D00000)

/* Size: 384K, End at: 0x56C5FFFF */
#define SOC_IOMCU_TVP_L1TCM_LOCAL_BASE_ADDR                 (0x56C00000)

/* Size: 256K, End at: 0x56B3FFFF */
#define SOC_IOMCU_TVP_MPL2M_BASE_ADDR                       (0x56B00000)

/* Size: 1M, End at: 0x566FFFFF */
#define SOC_IOMCU_TVP_REG_BASE_ADDR                         (0x56600000)

/* Size: 4K, End at: 0x56500FFF */
#define SOC_IOMCU_TVP_AMON_BASE_ADDR                        (0x56500000)

/* Size: 32K, End at: 0x56307FFF */
#define SOC_IOMCU_TVP_EICC0_BASE_ADDR                       (0x56300000)

/* Size: 4K, End at: 0x56212FFF */
#define SOC_IOMCU_DW_TVP_BASE_ADDR                      (0x56212000)

/* Size: 4K, End at: 0x5620FFFF */
#define SOC_IOMCU_TVP_EDMA0_BASE_ADDR                       (0x5620F000)

/* Size: 4K, End at: 0x5620EFFF */
#define SOC_IOMCU_TVP_RRT_BASE_ADDR                         (0x5620E000)

/* Size: 4K, End at: 0x5620DFFF */
#define SOC_IOMCU_TVP_TIMER7_BASE_ADDR                      (0x5620D000)

/* Size: 4K, End at: 0x5620CFFF */
#define SOC_IOMCU_TVP_TIMER6_BASE_ADDR                      (0x5620C000)

/* Size: 4K, End at: 0x5620BFFF */
#define SOC_IOMCU_TVP_TIMER5_BASE_ADDR                      (0x5620B000)

/* Size: 4K, End at: 0x5620AFFF */
#define SOC_IOMCU_TVP_TIMER4_BASE_ADDR                      (0x5620A000)

/* Size: 4K, End at: 0x56209FFF */
#define SOC_IOMCU_TVP_TIMER3_BASE_ADDR                      (0x56209000)

/* Size: 4K, End at: 0x56208FFF */
#define SOC_IOMCU_TVP_TIMER2_BASE_ADDR                      (0x56208000)

/* Size: 4K, End at: 0x56207FFF */
#define SOC_IOMCU_TVP_TIMER1_BASE_ADDR                      (0x56207000)

/* Size: 4K, End at: 0x56206FFF */
#define SOC_IOMCU_TVP_TIMER0_BASE_ADDR                      (0x56206000)

/* Size: 4K, End at: 0x56205FFF */
#define SOC_IOMCU_TVP_WDT1_BASE_ADDR                        (0x56205000)

/* Size: 4K, End at: 0x56204FFF */
#define SOC_IOMCU_TVP_WDT0_BASE_ADDR                        (0x56204000)

/* Size: 4K, End at: 0x56201FFF */
#define SOC_IOMCU_SYS_TVP_BASE_ADDR                         (0x56201000)

/* Size: 64K, End at: 0x560CFFFF */
#define SOC_IOMCU_TSP_DSS3_L1TCM_GLOBAL_BASE_ADDR           (0x560C0000)

/* Size: 64K, End at: 0x5608FFFF */
#define SOC_IOMCU_TSP_DSS2_L1TCM_GLOBAL_BASE_ADDR           (0x56080000)

/* Size: 64K, End at: 0x5604FFFF */
#define SOC_IOMCU_TSP_DSS1_L1TCM_GLOBAL_BASE_ADDR           (0x56040000)

/* Size: 64K, End at: 0x5600FFFF */
#define SOC_IOMCU_TSP_DSS0_L1TCM_GLOBAL_BASE_ADDR           (0x56000000)

/* Size: 128K, End at: 0x55F1FFFF */
#define SOC_IOMCU_TSP_L1TCM_LOCAL_BASE_ADDR                 (0x55F00000)

/* Size: 2M, End at: 0x559FFFFF */
#define SOC_IOMCU_TSP_REG_BASE_ADDR                         (0x55800000)

/* Size: 4608K, End at: 0x5547FFFF */
#define SOC_IOMCU_TSP_L2M_BASE_ADDR                         (0x55000000)

/* Size: 64K, End at: 0x54D0FFFF */
#define SOC_IOMCU_NOC_MDMBUS_SERVICE_TARGET_BASE_ADDR       (0x54D00000)

/* Size: 4K, End at: 0x54C00FFF */
#define SOC_IOMCU_MDM_AMON_BASE_ADDR                        (0x54C00000)

/* Size: 4K, End at: 0x54B5BFFF */
#define SOC_IOMCU_MDM_IPCM3_BASE_ADDR                       (0x54B5B000)

/* Size: 4K, End at: 0x54B5AFFF */
#define SOC_IOMCU_MDM_IPCM2_BASE_ADDR                       (0x54B5A000)

/* Size: 4K, End at: 0x54B59FFF */
#define SOC_IOMCU_MDM_IPCM1_BASE_ADDR                       (0x54B59000)

/* Size: 4K, End at: 0x54B58FFF */
#define SOC_IOMCU_MDM_IPCM0_BASE_ADDR                       (0x54B58000)

/* Size: 4K, End at: 0x54B57FFF */
#define SOC_IOMCU_CICOM_BASE_ADDR                           (0x54B57000)

/* Size: 4K, End at: 0x54B54FFF */
#define SOC_IOMCU_UPACC_BASE_ADDR                           (0x54B54000)

/* Size: 16K, End at: 0x54B53FFF */
#define SOC_IOMCU_CIPHER_BASE_ADDR                          (0x54B50000)

/* Size: 64K, End at: 0x54B4FFFF */
#define SOC_IOMCU_L2DLE_BASE_ADDR                           (0x54B40000)

/* Size: 32K, End at: 0x54B07FFF */
#define SOC_IOMCU_MDM_EICC0_BASE_ADDR                       (0x54B00000)

/* Size: 8K, End at: 0x54A3BFFF */
#define SOC_IOMCU_MDM_SYSCNT_BASE_ADDR                      (0x54A3A000)

/* Size: 4K, End at: 0x54A39FFF */
#define SOC_IOMCU_MDM_EDMA1_BASE_ADDR                       (0x54A39000)

/* Size: 4K, End at: 0x54A38FFF */
#define SOC_IOMCU_MDM_EDMA0_BASE_ADDR                       (0x54A38000)

/* Size: 4K, End at: 0x54A33FFF */
#define SOC_IOMCU_LATENCYM_BASE_ADDR                        (0x54A33000)

/* Size: 4K, End at: 0x54A32FFF */
#define SOC_IOMCU_RRT_BASE_ADDR                             (0x54A32000)

/* Size: 4K, End at: 0x54A2DFFF */
#define SOC_IOMCU_MDM_LOAD_MON1_BASE_ADDR                   (0x54A2D000)

/* Size: 4K, End at: 0x54A2CFFF */
#define SOC_IOMCU_MDM_LOAD_MON0_BASE_ADDR                   (0x54A2C000)

/* Size: 4K, End at: 0x54A2AFFF */
#define SOC_IOMCU_MDM_RTC_BASE_ADDR                         (0x54A2A000)

/* Size: 4K, End at: 0x54A29FFF */
#define SOC_IOMCU_MDM_PM_CTRL0_BASE_ADDR                    (0x54A29000)

/* Size: 4K, End at: 0x54A28FFF */
#define SOC_IOMCU_MDM_PM_CTRL1_BASE_ADDR                    (0x54A28000)

/* Size: 4K, End at: 0x54A27FFF */
#define SOC_IOMCU_MDM_UART1_BASE_ADDR                       (0x54A27000)

/* Size: 4K, End at: 0x54A26FFF */
#define SOC_IOMCU_MDM_UART0_BASE_ADDR                       (0x54A26000)

/* Size: 4K, End at: 0x54A24FFF */
#define SOC_IOMCU_MDM_TIMER15_BASE_ADDR                     (0x54A24000)

/* Size: 4K, End at: 0x54A23FFF */
#define SOC_IOMCU_MDM_TIMER14_BASE_ADDR                     (0x54A23000)

/* Size: 4K, End at: 0x54A22FFF */
#define SOC_IOMCU_MDM_TIMER13_BASE_ADDR                     (0x54A22000)

/* Size: 4K, End at: 0x54A21FFF */
#define SOC_IOMCU_MDM_TIMER12_BASE_ADDR                     (0x54A21000)

/* Size: 4K, End at: 0x54A20FFF */
#define SOC_IOMCU_MDM_TIMER11_BASE_ADDR                     (0x54A20000)

/* Size: 4K, End at: 0x54A1FFFF */
#define SOC_IOMCU_MDM_TIMER10_BASE_ADDR                     (0x54A1F000)

/* Size: 4K, End at: 0x54A1EFFF */
#define SOC_IOMCU_MDM_TIMER9_BASE_ADDR                      (0x54A1E000)

/* Size: 4K, End at: 0x54A1DFFF */
#define SOC_IOMCU_MDM_TIMER8_BASE_ADDR                      (0x54A1D000)

/* Size: 4K, End at: 0x54A1CFFF */
#define SOC_IOMCU_MDM_TIMER7_BASE_ADDR                      (0x54A1C000)

/* Size: 4K, End at: 0x54A1BFFF */
#define SOC_IOMCU_MDM_TIMER6_BASE_ADDR                      (0x54A1B000)

/* Size: 4K, End at: 0x54A1AFFF */
#define SOC_IOMCU_MDM_TIMER5_BASE_ADDR                      (0x54A1A000)

/* Size: 4K, End at: 0x54A19FFF */
#define SOC_IOMCU_MDM_TIMER4_BASE_ADDR                      (0x54A19000)

/* Size: 4K, End at: 0x54A18FFF */
#define SOC_IOMCU_MDM_TIMER3_BASE_ADDR                      (0x54A18000)

/* Size: 4K, End at: 0x54A17FFF */
#define SOC_IOMCU_MDM_TIMER2_BASE_ADDR                      (0x54A17000)

/* Size: 4K, End at: 0x54A16FFF */
#define SOC_IOMCU_MDM_TIMER1_BASE_ADDR                      (0x54A16000)

/* Size: 4K, End at: 0x54A15FFF */
#define SOC_IOMCU_MDM_TIMER0_BASE_ADDR                      (0x54A15000)

/* Size: 4K, End at: 0x54A13FFF */
#define SOC_IOMCU_MDM_WDT15_BASE_ADDR                       (0x54A13000)

/* Size: 4K, End at: 0x54A12FFF */
#define SOC_IOMCU_MDM_WDT14_BASE_ADDR                       (0x54A12000)

/* Size: 4K, End at: 0x54A11FFF */
#define SOC_IOMCU_MDM_WDT13_BASE_ADDR                       (0x54A11000)

/* Size: 4K, End at: 0x54A10FFF */
#define SOC_IOMCU_MDM_WDT12_BASE_ADDR                       (0x54A10000)

/* Size: 4K, End at: 0x54A0FFFF */
#define SOC_IOMCU_MDM_WDT11_BASE_ADDR                       (0x54A0F000)

/* Size: 4K, End at: 0x54A0EFFF */
#define SOC_IOMCU_MDM_WDT10_BASE_ADDR                       (0x54A0E000)

/* Size: 4K, End at: 0x54A0DFFF */
#define SOC_IOMCU_MDM_WDT9_BASE_ADDR                        (0x54A0D000)

/* Size: 4K, End at: 0x54A0CFFF */
#define SOC_IOMCU_MDM_WDT8_BASE_ADDR                        (0x54A0C000)

/* Size: 4K, End at: 0x54A0BFFF */
#define SOC_IOMCU_MDM_WDT7_BASE_ADDR                        (0x54A0B000)

/* Size: 4K, End at: 0x54A0AFFF */
#define SOC_IOMCU_MDM_WDT6_BASE_ADDR                        (0x54A0A000)

/* Size: 4K, End at: 0x54A09FFF */
#define SOC_IOMCU_MDM_WDT5_BASE_ADDR                        (0x54A09000)

/* Size: 4K, End at: 0x54A08FFF */
#define SOC_IOMCU_MDM_WDT4_BASE_ADDR                        (0x54A08000)

/* Size: 4K, End at: 0x54A07FFF */
#define SOC_IOMCU_MDM_WDT3_BASE_ADDR                        (0x54A07000)

/* Size: 4K, End at: 0x54A06FFF */
#define SOC_IOMCU_MDM_WDT2_BASE_ADDR                        (0x54A06000)

/* Size: 4K, End at: 0x54A05FFF */
#define SOC_IOMCU_MDM_WDT1_BASE_ADDR                        (0x54A05000)

/* Size: 4K, End at: 0x54A04FFF */
#define SOC_IOMCU_MDM_WDT0_BASE_ADDR                        (0x54A04000)

/* Size: 4K, End at: 0x54A03FFF */
#define SOC_IOMCU_TSP_IMU_BASE_ADDR                         (0x54A03000)

/* Size: 4K, End at: 0x54A02FFF */
#define SOC_IOMCU_MDM_AUTO_DFS_BASE_ADDR                    (0x54A02000)

/* Size: 4K, End at: 0x54A01FFF */
#define SOC_IOMCU_SYS_MDM_BASE_ADDR                         (0x54A01000)

/* Size: 4K, End at: 0x54A00FFF */
#define SOC_IOMCU_CRG_MDM_BASE_ADDR                         (0x54A00000)

/* Size: 4K, End at: 0x542D9FFF */
#define SOC_IOMCU_ALINK0_BASE_ADDR                          (0x542D9000)

/* Size: 4K, End at: 0x542D8FFF */
#define SOC_IOMCU_SYS_BBIC_BASE_ADDR                        (0x542D8000)

/* Size: 4K, End at: 0x542D1FFF */
#define SOC_IOMCU_DW_PDF_CFG_BASE_ADDR                  (0x542D1000)

/* Size: 4K, End at: 0x542D0FFF */
#define SOC_IOMCU_SYS_BBP_APB_BASE_ADDR                     (0x542D0000)

/* Size: 64K, End at: 0x5428FFFF */
#define SOC_IOMCU_NOC_BBPBUS_SERVICE_TARGET_BASE_ADDR       (0x54280000)

/* Size: 64M, End at: 0x53FFFFFF */
#define SOC_IOMCU_BBP5G_BASE_ADDR                           (0x50000000)


/* =====================  HSDT_SUBSYS (100MB) PART 2  ===================== */
/* Size: 4M, End at: 0x4C3FFFFF */
#define SOC_IOMCU_PCIECtrl0_DBI_BASE_ADDR                   (0x4C000000)

/* Size: 32M, End at: 0x4BFFFFFF */
#define SOC_IOMCU_PCIECtrl0_BASE_ADDR                       (0x4A000000)


/* =========================  MEDIA2_SUBSYS(8MB)  ========================= */
/* Size: 64K, End at: 0x493DFFFF */
#define SOC_IOMCU_NOC_EPS_Service_Target_BASE_ADDR          (0x493D0000)

/* Size: 64K, End at: 0x493BFFFF */
#define SOC_IOMCU_NOC_VENC_Service_Target_BASE_ADDR         (0x493B0000)

/* Size: 64K, End at: 0x493AFFFF */
#define SOC_IOMCU_NOC_VDEC_Service_Target_BASE_ADDR         (0x493A0000)

/* Size: 64K, End at: 0x4939FFFF */
#define SOC_IOMCU_NOC_VCODECBUS_Service_Target_BASE_ADDR    (0x49390000)

/* Size: 64K, End at: 0x4938FFFF */
#define SOC_IOMCU_NOC_IVP32_Service_Target_BASE_ADDR        (0x49380000)

/* Size: 4K, End at: 0x49301FFF */
#define SOC_IOMCU_MEDAI2_LoadMonitor_BASE_ADDR              (0x49301000)

/* Size: 4K, End at: 0x49300FFF */
#define SOC_IOMCU_MEDIA2_CRG_BASE_ADDR                      (0x49300000)

/* Size: 256K, End at: 0x492BFFFF */
#define SOC_IOMCU_VENC_BASE_ADDR                            (0x49280000)

/* Size: 256K, End at: 0x4923FFFF */
#define SOC_IOMCU_VDEC_BASE_ADDR                            (0x49200000)

/* Size: 64K, End at: 0x491CFFFF */
#define SOC_IOMCU_IVP32_SMMU_BASE_ADDR                      (0x491C0000)

/* Size: 4K, End at: 0x49183FFF */
#define SOC_IOMCU_IVP32_TIMER1_BASE_ADDR                    (0x49183000)

/* Size: 4K, End at: 0x49182FFF */
#define SOC_IOMCU_IVP32_TIMER0_BASE_ADDR                    (0x49182000)

/* Size: 4K, End at: 0x49181FFF */
#define SOC_IOMCU_IVP32_WDG_BASE_ADDR                       (0x49181000)

/* Size: 4K, End at: 0x49180FFF */
#define SOC_IOMCU_IVP32_CFG_BASE_ADDR                       (0x49180000)

/* Size: 32K, End at: 0x49107FFF */
#define SOC_IOMCU_IVP32_IRAM_BASE_ADDR                      (0x49100000)

/* Size: 144K, End at: 0x49063FFF */
#define SOC_IOMCU_IVP32_DRAM1_BASE_ADDR                     (0x49040000)

/* Size: 144K, End at: 0x49023FFF */
#define SOC_IOMCU_IVP32_DRAM0_BASE_ADDR                     (0x49000000)


/* ============================  MEDIA1(16MB)  ============================ */
/* Size: 64K, End at: 0x486AFFFF */
#define SOC_IOMCU_NOC_VIVO_Service_Target_BASE_ADDR         (0x486A0000)

/* Size: 64K, End at: 0x4869FFFF */
#define SOC_IOMCU_NOC_ISP_Service_Target_BASE_ADDR          (0x48690000)

/* Size: 64K, End at: 0x4868FFFF */
#define SOC_IOMCU_NOC_DSS_Service_Target_BASE_ADDR          (0x48680000)

/* Size: 4K, End at: 0x48608FFF */
#define SOC_IOMCU_CSI_E_BASE_ADDR                           (0x48608000)

/* Size: 4K, End at: 0x48607FFF */
#define SOC_IOMCU_CSI_D_BASE_ADDR                           (0x48607000)

/* Size: 4K, End at: 0x48606FFF */
#define SOC_IOMCU_CSI_C_BASE_ADDR                           (0x48606000)

/* Size: 4K, End at: 0x48605FFF */
#define SOC_IOMCU_CSI_B_BASE_ADDR                           (0x48605000)

/* Size: 4K, End at: 0x48604FFF */
#define SOC_IOMCU_CSI_A_BASE_ADDR                           (0x48604000)

/* Size: 4K, End at: 0x48603FFF */
#define SOC_IOMCU_CSI_adapter_BASE_ADDR                     (0x48603000)

/* Size: 4K, End at: 0x48602FFF */
#define SOC_IOMCU_IDI2AXI_BASE_ADDR                         (0x48602000)

/* Size: 4K, End at: 0x48601FFF */
#define SOC_IOMCU_MEDIA1_CRG_BASE_ADDR                      (0x48601000)

/* Size: 4K, End at: 0x48600FFF */
#define SOC_IOMCU_MEDIA1_LoadMonitor_BASE_ADDR              (0x48600000)

/* Size: 40K, End at: 0x484F7FFF */
#define SOC_IOMCU_GMP_LUT1_BASE_ADDR                        (0x484EE000)

/* Size: 40K, End at: 0x484E7FFF */
#define SOC_IOMCU_GMP_LUT0_BASE_ADDR                        (0x484DE000)

/* Size: 4K, End at: 0x484BEFFF */
#define SOC_IOMCU_CROSSBAR_BASE_ADDR                        (0x484BE000)

/* Size: 112K, End at: 0x484BDFFF */
#define SOC_IOMCU_DISP_CH1_BASE_ADDR                        (0x484A2000)

/* Size: 4K, End at: 0x484A1FFF */
#define SOC_IOMCU_DISP_GLB_BASE_ADDR                        (0x484A1000)

/* Size: 4K, End at: 0x484A0FFF */
#define SOC_IOMCU_WB_BASE_ADDR                              (0x484A0000)

/* Size: 64K, End at: 0x4849FFFF */
#define SOC_IOMCU_DSS_PRO_BASE_ADDR                         (0x48490000)

/* Size: 64K, End at: 0x4848FFFF */
#define SOC_IOMCU_DBCU0_SMMU_BASE_ADDR                      (0x48480000)

/* Size: 112K, End at: 0x4847DFFF */
#define SOC_IOMCU_DISP_CH0_BASE_ADDR                        (0x48462000)

/* Size: 1K, End at: 0x48460BFF */
#define SOC_IOMCU_OV2_BASE_ADDR                             (0x48460800)

/* Size: 1K, End at: 0x484603FF */
#define SOC_IOMCU_OV0_BASE_ADDR                             (0x48460000)

/* Size: 16K, End at: 0x4845FFFF */
#define SOC_IOMCU_WCH1_BASE_ADDR                            (0x4845C000)

/* Size: 4K, End at: 0x48452FFF */
#define SOC_IOMCU_RCH_D0_BASE_ADDR                          (0x48452000)

/* Size: 4K, End at: 0x48451FFF */
#define SOC_IOMCU_RCH_D3_BASE_ADDR                          (0x48451000)

/* Size: 4K, End at: 0x48450FFF */
#define SOC_IOMCU_RCH_D2_BASE_ADDR                          (0x48450000)

/* Size: 32K, End at: 0x4843FFFF */
#define SOC_IOMCU_RCH_G0_BASE_ADDR                          (0x48438000)

/* Size: 32K, End at: 0x4842FFFF */
#define SOC_IOMCU_RCH_V1_BASE_ADDR                          (0x48428000)

/* Size: 32K, End at: 0x48427FFF */
#define SOC_IOMCU_RCH_V0_BASE_ADDR                          (0x48420000)

/* Size: 4K, End at: 0x48412FFF */
#define SOC_IOMCU_GLB_BASE_ADDR                             (0x48412000)

/* Size: 4K, End at: 0x48411FFF */
#define SOC_IOMCU_DBUG_BASE_ADDR                            (0x48411000)

/* Size: 2K, End at: 0x48410FFF */
#define SOC_IOMCU_MCTL_MUTEX_BASE_ADDR                      (0x48410800)

/* Size: 2K, End at: 0x484107FF */
#define SOC_IOMCU_MCTL_SYS_BASE_ADDR                        (0x48410000)

/* Size: 4K, End at: 0x4840AFFF */
#define SOC_IOMCU_DBCU0_MIF_BASE_ADDR                       (0x4840A000)

/* Size: 4K, End at: 0x48409FFF */
#define SOC_IOMCU_DBCU1_AIF_BASE_ADDR                       (0x48409000)

/* Size: 4K, End at: 0x48407FFF */
#define SOC_IOMCU_DBCU0_AIF_BASE_ADDR                       (0x48407000)

/* Size: 5K, End at: 0x484033FF */
#define SOC_IOMCU_CMD_BASE_ADDR                             (0x48402000)

/* Size: 2K, End at: 0x48401FFF */
#define SOC_IOMCU_ASC_BASE_ADDR                             (0x48401800)

/* Size: 1K, End at: 0x484013FF */
#define SOC_IOMCU_DSI0_BASE_ADDR                            (0x48401000)

/* Size: 4K, End at: 0x48400FFF */
#define SOC_IOMCU_MMBUF_CFG_BASE_ADDR                       (0x48400000)

/* Size: 64K, End at: 0x483FFFFF */
#define SOC_IOMCU_SMMUv500_CFG_BASE_ADDR                    (0x483F0000)

/* Size: 8K, End at: 0x483EFFFF */
#define SOC_IOMCU_ISP_R8_PERI_BASE_BASE_ADDR                (0x483EE000)

/* Size: 4K, End at: 0x483E4FFF */
#define SOC_IOMCU_ISP_TCMDMA_BASE_ADDR                      (0x483E4000)

/* Size: 4K, End at: 0x483E3FFF */
#define SOC_IOMCU_ISP_SUB_CTRL_BASE_ADDR                    (0x483E3000)

/* Size: 4K, End at: 0x483E2FFF */
#define SOC_IOMCU_ISP_IPC_BASE_ADDR                         (0x483E2000)

/* Size: 4K, End at: 0x483E1FFF */
#define SOC_IOMCU_ISP_TIMER_BASE_ADDR                       (0x483E1000)

/* Size: 4K, End at: 0x483E0FFF */
#define SOC_IOMCU_ISP_WDT_BASE_ADDR                         (0x483E0000)

/* Size: 64K, End at: 0x483DFFFF */
#define SOC_IOMCU_ISP_R8_DTCM_BASE_ADDR                     (0x483D0000)

/* Size: 32K, End at: 0x483C7FFF */
#define SOC_IOMCU_ISP_R8_ITCM_BASE_ADDR                     (0x483C0000)

/* Size: 1.75M, End at: 0x483BFFFF */
#define SOC_IOMCU_ISP_Core_CFG_BASE_ADDR                    (0x48200000)

/* Size: 128K, End at: 0x4803FFFF */
#define SOC_IOMCU_SMMU_BASE_ADDR                            (0x48020000)

/* Size: 4K, End at: 0x4800CFFF */
#define SOC_IOMCU_ORB_BASE_ADDR                             (0x4800C000)

/* Size: 8K, End at: 0x48007FFF */
#define SOC_IOMCU_CVDR_BASE_ADDR                            (0x48006000)

/* Size: 4K, End at: 0x48005FFF */
#define SOC_IOMCU_CMDLST_BASE_ADDR                          (0x48005000)

/* Size: 4K, End at: 0x48004FFF */
#define SOC_IOMCU_TOP_BASE_ADDR                             (0x48004000)

/* Size: 4K, End at: 0x48000FFF */
#define SOC_IOMCU_JPGENC_BASE_ADDR                          (0x48000000)


/* =============================  NPU(32MB)  ============================== */
/* Size: 256K, End at: 0x45F7FFFF */
#define SOC_IOMCU_NPU_TCU_BASE_ADDR                         (0x45F40000)

/* Size: 128K, End at: 0x45F3FFFF */
#define SOC_IOMCU_SYSDMA_TBU_BASE_ADDR                      (0x45F20000)

/* Size: 128K, End at: 0x45F1FFFF */
#define SOC_IOMCU_AIC_TBU_BASE_ADDR                         (0x45F00000)

/* Size: 128K, End at: 0x45EFFFFF */
#define SOC_IOMCU_sysdma_cfg_BASE_ADDR                      (0x45EE0000)

/* Size: 4K, End at: 0x45E07FFF */
#define SOC_IOMCU_npu_etzpc_BASE_ADDR                       (0x45E07000)

/* Size: 4K, End at: 0x45E06FFF */
#define SOC_IOMCU_npu_hw_exp_irq_BASE_ADDR                  (0x45E06000)

/* Size: 4K, End at: 0x45E05FFF */
#define SOC_IOMCU_npu_uart_BASE_ADDR                        (0x45E05000)

/* Size: 4K, End at: 0x45E04FFF */
#define SOC_IOMCU_npu_sysctrl_BASE_ADDR                     (0x45E04000)

/* Size: 4K, End at: 0x45E03FFF */
#define SOC_IOMCU_npu_crg_BASE_ADDR                         (0x45E03000)

/* Size: 4K, End at: 0x45E02FFF */
#define SOC_IOMCU_npu_tzpc_BASE_ADDR                        (0x45E02000)

/* Size: 4K, End at: 0x45E01FFF */
#define SOC_IOMCU_npu_ipc_ns_BASE_ADDR                      (0x45E01000)

/* Size: 4K, End at: 0x45E00FFF */
#define SOC_IOMCU_npu_ipc_s_BASE_ADDR                       (0x45E00000)

/* Size: 2M, End at: 0x45DFFFFF */
#define SOC_IOMCU_gic_cfg_BASE_ADDR                         (0x45C00000)

/* Size: 1024K, End at: 0x450FFFFF */
#define SOC_IOMCU_aic0_subsys_cfg_BASE_ADDR                 (0x45000000)

/* Size: 64K, End at: 0x44D3FFFF */
#define SOC_IOMCU_npu_bus_service_target_BASE_ADDR          (0x44D30000)

/* Size: 64K, End at: 0x4423FFFF */
#define SOC_IOMCU_SRAM_reg_BASE_ADDR                        (0x44230000)

/* Size: 64K, End at: 0x4422FFFF */
#define SOC_IOMCU_SRAM_atomicdec_BASE_ADDR                  (0x44220000)

/* Size: 64K, End at: 0x4421FFFF */
#define SOC_IOMCU_SRAM_atomicadd_BASE_ADDR                  (0x44210000)

/* Size: 64K, End at: 0x4420FFFF */
#define SOC_IOMCU_SRAM_normal_BASE_ADDR                     (0x44200000)

/* Size: 64K, End at: 0x4411FFFF */
#define SOC_IOMCU_bs_slv_BASE_ADDR                          (0x44110000)

/* Size: 4K, End at: 0x44100FFF */
#define SOC_IOMCU_doorbell_sts_BASE_ADDR                    (0x44100000)

/* Size: 512K, End at: 0x440FFFFF */
#define SOC_IOMCU_doorbell_cfg_BASE_ADDR                    (0x44080000)

/* Size: 4K, End at: 0x44037FFF */
#define SOC_IOMCU_npu_cpu_secure_BASE_ADDR                  (0x44037000)

/* Size: 4K, End at: 0x44036FFF */
#define SOC_IOMCU_axi_mon_soc_BASE_ADDR                     (0x44036000)

/* Size: 4K, End at: 0x44035FFF */
#define SOC_IOMCU_axi_mon_bs_BASE_ADDR                      (0x44035000)

/* Size: 4K, End at: 0x44034FFF */
#define SOC_IOMCU_axi_mon_cpu_BASE_ADDR                     (0x44034000)

/* Size: 4K, End at: 0x44032FFF */
#define SOC_IOMCU_ai_cpu_local_ctrl_BASE_ADDR               (0x44032000)

/* Size: 4K, End at: 0x44031FFF */
#define SOC_IOMCU_ts_cpu_local_ctrl_BASE_ADDR               (0x44031000)

/* Size: 4K, End at: 0x44030FFF */
#define SOC_IOMCU_sysctrl_npu_cpu_BASE_ADDR                 (0x44030000)

/* Size: 4K, End at: 0x44022FFF */
#define SOC_IOMCU_fcm_local_ctrl_BASE_ADDR                  (0x44022000)

/* Size: 4K, End at: 0x44021FFF */
#define SOC_IOMCU_ts_cpu_a55_ctrl_BASE_ADDR                 (0x44021000)

/* Size: 4K, End at: 0x44011FFF */
#define SOC_IOMCU_ts_cpu_timer_BASE_ADDR                    (0x44011000)

/* Size: 4K, End at: 0x44010FFF */
#define SOC_IOMCU_ai_cpu_timer_BASE_ADDR                    (0x44010000)

/* Size: 4K, End at: 0x44001FFF */
#define SOC_IOMCU_ts_cpu_watchdog_BASE_ADDR                 (0x44001000)

/* Size: 4K, End at: 0x44000FFF */
#define SOC_IOMCU_ai_cpu_watchdog_BASE_ADDR                 (0x44000000)


/* ===============================  <N/A>  ================================ */
/* Size: 1M, End at: 0x200FFFFF */
#define SOC_IOMCU_DTCM_BASE_ADDR                            (0x20000000)

/* Size: 1M, End at: 0x000FFFFF */
#define SOC_IOMCU_ITCM_BASE_ADDR                            (0x00000000)



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

#endif /* end of soc_iomcu_axi_baseaddr_interface.h */
