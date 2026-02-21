

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


/* =============================  BOOT 768KB  ============================= */
/* Size: 384K, End at: 0xFFFFFFFF */
#define SOC_IOMCU_LP_RAM_BASE_ADDR                              (0xFFFA0000)

/* Size: 128K, End at: 0xFFF9FFFF */
#define SOC_IOMCU_BOOTROM_BASE_ADDR                             (0xFFF80000)


/* ===  LP_APB_PERI （2MB LPCTRL+512KB PERI+ 80MB CSSYS+  256KB GPU_APB+2MB FCM_APB) 注意GPU/FCM_APB仅LPMCU通过内部BUS访问，其他IP通过SYSBUS访问  === */
/* Size: 4K, End at: 0xFFB8FFFF */
#define SOC_IOMCU_LP_CONFIG_BASE_ADDR                           (0xFFB8F000)

/* Size: 4K, End at: 0xFFB8EFFF */
#define SOC_IOMCU_LP_TIMER_BASE_ADDR                            (0xFFB8E000)

/* Size: 4K, End at: 0xFFB8DFFF */
#define SOC_IOMCU_LP_WDG_BASE_ADDR                              (0xFFB8D000)

/* Size: 4K, End at: 0xFFB8BFFF */
#define SOC_IOMCU_CORE_CRG_BASE_ADDR                            (0xFFB8B000)

/* Size: 4K, End at: 0xFFB87FFF */
#define SOC_IOMCU_func_mbist_BASE_ADDR                          (0xFFB87000)

/* Size: 4K, End at: 0xFFB85FFF */
#define SOC_IOMCU_PERI_CRG_BASE_ADDR                            (0xFFB85000)

/* Size: 4K, End at: 0xFFB84FFF */
#define SOC_IOMCU_digital_tsensorc_BASE_ADDR                    (0xFFB84000)

/* Size: 4K, End at: 0xFFB82FFF */
#define SOC_IOMCU_UART6_BASE_ADDR                               (0xFFB82000)

/* Size: 4K, End at: 0xFFB81FFF */
#define SOC_IOMCU_PMC_BASE_ADDR                                 (0xFFB81000)

/* Size: 4K, End at: 0xFFB80FFF */
#define SOC_IOMCU_TSENSORC_BASE_ADDR                            (0xFFB80000)


/* ==========================  LP_AHB_PERI 3MB  =========================== */
/* Size: 512K, End at: 0xFFAFFFFF */
#define SOC_IOMCU_DMCPACK3_BASE_ADDR                            (0xFFA80000)

/* Size: 512K, End at: 0xFFA7FFFF */
#define SOC_IOMCU_DMCPACK2_BASE_ADDR                            (0xFFA00000)

/* Size: 512K, End at: 0xFF9FFFFF */
#define SOC_IOMCU_DMCPACK1_BASE_ADDR                            (0xFF980000)

/* Size: 512K, End at: 0xFF97FFFF */
#define SOC_IOMCU_DMCPACK0_BASE_ADDR                            (0xFF900000)

/* Size: 512K, End at: 0xFF8FFFFF */
#define SOC_IOMCU_AHB_RADIO_VIRTUAL_BASE_ADDR                   (0xFF880000)

/* Size: 4K, End at: 0xFF802FFF */
#define SOC_IOMCU_exmbist0_BASE_ADDR                            (0xFF802000)

/* Size: 4K, End at: 0xFF801FFF */
#define SOC_IOMCU_exmbist1_BASE_ADDR                            (0xFF801000)

/* Size: 4K, End at: 0xFF800FFF */
#define SOC_IOMCU_CHI2QSP_BASE_ADDR                             (0xFF800000)

/* Size: 1M, End at: 0xFF7FFFFF */
#define SOC_IOMCU_QICE_BASE_ADDR                                (0xFF700000)


/* =========================  CFGDMA SUBSYS 8MB  ========================== */
/* Size: 4K, End at: 0xFF462FFF */
#define SOC_IOMCU_PERI_DMAC_BASE_ADDR                           (0xFF462000)

/* Size: 8K, End at: 0xFF461FFF */
#define SOC_IOMCU_FW_CFGDMA_BASE_ADDR                           (0xFF460000)

/* Size: 128K, End at: 0xFF45FFFF */
#define SOC_IOMCU_DDR_XCTRL_APB_BASE_ADDR                       (0xFF440000)

/* Size: 256K, End at: 0xFF43FFFF */
#define SOC_IOMCU_DDR_XCTRL_AHB_BASE_ADDR                       (0xFF400000)

/* Size: 1M, End at: 0xFF3FFFFF */
#define SOC_IOMCU_QCP_CFGDMABUS_BASE_ADDR                       (0xFF300000)

/* Size: 64K, End at: 0xFF2FFFFF */
#define SOC_IOMCU_IPC_NS1_BASE_ADDR                             (0xFF2F0000)

/* Size: 64K, End at: 0xFF2EFFFF */
#define SOC_IOMCU_IPC_NS_BASE_ADDR                              (0xFF2E0000)

/* Size: 64K, End at: 0xFF2DFFFF */
#define SOC_IOMCU_IPC_BASE_ADDR                                 (0xFF2D0000)

/* Size: 4K, End at: 0xFF292FFF */
#define SOC_IOMCU_GPIO2_BASE_ADDR                               (0xFF292000)

/* Size: 4K, End at: 0xFF291FFF */
#define SOC_IOMCU_PAD_DSI_IOMG_IOCG_BASE_ADDR                   (0xFF291000)

/* Size: 4K, End at: 0xFF290FFF */
#define SOC_IOMCU_IOC_DSI_IOMG_BASE_ADDR                        (0xFF290000)

/* Size: 4K, End at: 0xFF286FFF */
#define SOC_IOMCU_GPIO1_BASE_ADDR                               (0xFF286000)

/* Size: 4K, End at: 0xFF282FFF */
#define SOC_IOMCU_GPIO0_BASE_ADDR                               (0xFF282000)

/* Size: 4K, End at: 0xFF281FFF */
#define SOC_IOMCU_PAD_PERI_IOMG_IOCG_BASE_ADDR                  (0xFF281000)

/* Size: 4K, End at: 0xFF280FFF */
#define SOC_IOMCU_PERI_IOC_BASE_ADDR                            (0xFF280000)

/* Size: 4K, End at: 0xFF258FFF */
#define SOC_IOMCU_PWR_STAT_BASE_ADDR                            (0xFF258000)

/* Size: 4K, End at: 0xFF256FFF */
#define SOC_IOMCU_PERF_STAT_BASE_ADDR                           (0xFF256000)

/* Size: 4K, End at: 0xFF254FFF */
#define SOC_IOMCU_UART0_BASE_ADDR                               (0xFF254000)

/* Size: 4K, End at: 0xFF250FFF */
#define SOC_IOMCU_I3C4_BASE_ADDR                                (0xFF250000)

/* Size: 4K, End at: 0xFF24FFFF */
#define SOC_IOMCU_I2C7_BASE_ADDR                                (0xFF24F000)

/* Size: 4K, End at: 0xFF24EFFF */
#define SOC_IOMCU_I2C6_BASE_ADDR                                (0xFF24E000)

/* Size: 4K, End at: 0xFF24DFFF */
#define SOC_IOMCU_I2C4_BASE_ADDR                                (0xFF24D000)

/* Size: 4K, End at: 0xFF24CFFF */
#define SOC_IOMCU_I2C3_BASE_ADDR                                (0xFF24C000)

/* Size: 4K, End at: 0xFF24BFFF */
#define SOC_IOMCU_I2C2_BASE_ADDR                                (0xFF24B000)

/* Size: 4K, End at: 0xFF24AFFF */
#define SOC_IOMCU_SPI6_BASE_ADDR                                (0xFF24A000)

/* Size: 4K, End at: 0xFF249FFF */
#define SOC_IOMCU_MDM_GID_CTRL_PERI_BASE_ADDR                   (0xFF249000)

/* Size: 4K, End at: 0xFF248FFF */
#define SOC_IOMCU_SPI1_BASE_ADDR                                (0xFF248000)

/* Size: 4K, End at: 0xFF245FFF */
#define SOC_IOMCU_UART5_BASE_ADDR                               (0xFF245000)

/* Size: 4K, End at: 0xFF241FFF */
#define SOC_IOMCU_UART4_BASE_ADDR                               (0xFF241000)

/* Size: 4K, End at: 0xFF240FFF */
#define SOC_IOMCU_UART8_BASE_ADDR                               (0xFF240000)

/* Size: 4K, End at: 0xFF228FFF */
#define SOC_IOMCU_LoadMonitor1_BASE_ADDR                        (0xFF228000)

/* Size: 4K, End at: 0xFF227FFF */
#define SOC_IOMCU_LoadMonitor0_BASE_ADDR                        (0xFF227000)

/* Size: 4K, End at: 0xFF226FFF */
#define SOC_IOMCU_ATGC_BASE_ADDR                                (0xFF226000)

/* Size: 4K, End at: 0xFF225FFF */
#define SOC_IOMCU_TZPC_BASE_ADDR                                (0xFF225000)

/* Size: 4K, End at: 0xFF222FFF */
#define SOC_IOMCU_WD1_BASE_ADDR                                 (0xFF222000)

/* Size: 4K, End at: 0xFF221FFF */
#define SOC_IOMCU_WD0_BASE_ADDR                                 (0xFF221000)

/* Size: 4K, End at: 0xFF220FFF */
#define SOC_IOMCU_BLPWM1_BASE_ADDR                              (0xFF220000)

/* Size: 4K, End at: 0xFF21FFFF */
#define SOC_IOMCU_TIMER12_BASE_ADDR                             (0xFF21F000)

/* Size: 4K, End at: 0xFF21EFFF */
#define SOC_IOMCU_TIMER11_BASE_ADDR                             (0xFF21E000)

/* Size: 4K, End at: 0xFF21DFFF */
#define SOC_IOMCU_TIMER10_BASE_ADDR                             (0xFF21D000)

/* Size: 4K, End at: 0xFF21CFFF */
#define SOC_IOMCU_TIMER9_BASE_ADDR                              (0xFF21C000)

/* Size: 16K, End at: 0xFF207FFF */
#define SOC_IOMCU_SPMI1_BASE_ADDR                               (0xFF204000)

/* Size: 4K, End at: 0xFF201FFF */
#define SOC_IOMCU_MSPC_LOCK_BASE_ADDR                           (0xFF201000)

/* Size: 4K, End at: 0xFF200FFF */
#define SOC_IOMCU_GPIO0_SE_BASE_ADDR                            (0xFF200000)

/* Size: 4K, End at: 0xFF00FFFF */
#define SOC_IOMCU_intr_hub_hsdt0_BASE_ADDR                      (0xFF00F000)

/* Size: 4K, End at: 0xFF00EFFF */
#define SOC_IOMCU_hsdt0_ctrl_BASE_ADDR                          (0xFF00E000)

/* Size: 8K, End at: 0xFF00DFFF */
#define SOC_IOMCU_PCTRL_BASE_ADDR                               (0xFF00C000)

/* Size: 4K, End at: 0xFF00BFFF */
#define SOC_IOMCU_sourcelock_BASE_ADDR                          (0xFF00B000)

/* Size: 4K, End at: 0xFF00AFFF */
#define SOC_IOMCU_intr_hub_med1_BASE_ADDR                       (0xFF00A000)

/* Size: 4K, End at: 0xFF009FFF */
#define SOC_IOMCU_MEDIA1_CTRL_BASE_ADDR                         (0xFF009000)

/* Size: 4K, End at: 0xFF008FFF */
#define SOC_IOMCU_MEDIA2_CTRL_BASE_ADDR                         (0xFF008000)

/* Size: 4K, End at: 0xFF007FFF */
#define SOC_IOMCU_intr_hub_med2_BASE_ADDR                       (0xFF007000)

/* Size: 4K, End at: 0xFF006FFF */
#define SOC_IOMCU_hsdt1_ctrl_BASE_ADDR                          (0xFF006000)

/* Size: 4K, End at: 0xFF005FFF */
#define SOC_IOMCU_intr_hub_hsdt1_BASE_ADDR                      (0xFF005000)

/* Size: 4K, End at: 0xFF003FFF */
#define SOC_IOMCU_intr_hub_peri_BASE_ADDR                       (0xFF003000)

/* Size: 4K, End at: 0xFF001FFF */
#define SOC_IOMCU_intr_hub_npu_BASE_ADDR                        (0xFF001000)

/* Size: 4K, End at: 0xFF000FFF */
#define SOC_IOMCU_NPU_CTRL_BASE_ADDR                            (0xFF000000)


/* ==========================  FCM SUBSYS 16MB  =========================== */
/* Size: 256K, End at: 0xFEE3FFFF */
#define SOC_IOMCU_CPU_LPCTRL_AHB_BASE_ADDR                      (0xFEE00000)

/* Size: 1M, End at: 0xFEDFFFFF */
#define SOC_IOMCU_FCMQIC_QCP_BASE_ADDR                          (0xFED00000)

/* Size: 4K, End at: 0xFEC41FFF */
#define SOC_IOMCU_FCM_LP_MON_CFG_BASE_ADDR                      (0xFEC41000)

/* Size: 4K, End at: 0xFEC40FFF */
#define SOC_IOMCU_FCM_P2J_BASE_ADDR                             (0xFEC40000)

/* Size: 128K, End at: 0xFEC3FFFF */
#define SOC_IOMCU_CPU_LPCTRL_APB_BASE_ADDR                      (0xFEC20000)

/* Size: 4K, End at: 0xFEC0AFFF */
#define SOC_IOMCU_RTM_CPU_CTRL_BASE_ADDR                        (0xFEC0A000)

/* Size: 4K, End at: 0xFEC07FFF */
#define SOC_IOMCU_CPU_SUB_CTRL_BASE_ADDR                        (0xFEC07000)

/* Size: 4K, End at: 0xFEC06FFF */
#define SOC_IOMCU_CPU_CRG_BASE_ADDR                             (0xFEC06000)

/* Size: 4K, End at: 0xFEC05FFF */
#define SOC_IOMCU_FCM_LOCAL_PERI_BASE_ADDR                      (0xFEC05000)

/* Size: 4K, End at: 0xFEC04FFF */
#define SOC_IOMCU_CPU_PDC_BASE_ADDR                             (0xFEC04000)

/* Size: 4K, End at: 0xFEC03FFF */
#define SOC_IOMCU_FCM_GIC_HW_SPI1_BASE_ADDR                     (0xFEC03000)

/* Size: 4K, End at: 0xFEC02FFF */
#define SOC_IOMCU_FCM_LOCAL_GIC_BASE_ADDR                       (0xFEC02000)

/* Size: 4K, End at: 0xFEC01FFF */
#define SOC_IOMCU_FCM_GIC_HW_SPI0_BASE_ADDR                     (0xFEC01000)

/* Size: 4K, End at: 0xFEC00FFF */
#define SOC_IOMCU_FCM_SECURE_BASE_ADDR                          (0xFEC00000)

/* Size: 64K, End at: 0xFEB4FFFF */
#define SOC_IOMCU_HC_MPAM_NS_BASE_ADDR                          (0xFEB40000)

/* Size: 16K, End at: 0xFEB3BFFF */
#define SOC_IOMCU_HC_MEM_BASE_ADDR                              (0xFEB38000)

/* Size: 16K, End at: 0xFEB37FFF */
#define SOC_IOMCU_HC_PWE_BASE_ADDR                              (0xFEB34000)

/* Size: 16K, End at: 0xFEB33FFF */
#define SOC_IOMCU_HC_SECURE_BASE_ADDR                           (0xFEB30000)

/* Size: 16K, End at: 0xFEB2FFFF */
#define SOC_IOMCU_HC_RAS_BASE_ADDR                              (0xFEB2C000)

/* Size: 16K, End at: 0xFEB2BFFF */
#define SOC_IOMCU_HC_FLUSH_BASE_ADDR                            (0xFEB28000)

/* Size: 16K, End at: 0xFEB27FFF */
#define SOC_IOMCU_HC_PMU_BASE_ADDR                              (0xFEB24000)

/* Size: 16K, End at: 0xFEB23FFF */
#define SOC_IOMCU_HC_PWRCTL_BASE_ADDR                           (0xFEB20000)

/* Size: 64K, End at: 0xFEB0FFFF */
#define SOC_IOMCU_CORE7_RAS_BASE_ADDR                           (0xFEB00000)

/* Size: 64K, End at: 0xFEAFFFFF */
#define SOC_IOMCU_CORE6_RAS_BASE_ADDR                           (0xFEAF0000)

/* Size: 64K, End at: 0xFEAEFFFF */
#define SOC_IOMCU_CORE5_RAS_BASE_ADDR                           (0xFEAE0000)

/* Size: 64K, End at: 0xFEADFFFF */
#define SOC_IOMCU_CORE4_RAS_BASE_ADDR                           (0xFEAD0000)

/* Size: 64K, End at: 0xFEACFFFF */
#define SOC_IOMCU_CORE3_RAS_BASE_ADDR                           (0xFEAC0000)

/* Size: 64K, End at: 0xFEABFFFF */
#define SOC_IOMCU_CORE2_RAS_BASE_ADDR                           (0xFEAB0000)

/* Size: 64K, End at: 0xFEAAFFFF */
#define SOC_IOMCU_CORE1_RAS_BASE_ADDR                           (0xFEAA0000)

/* Size: 64K, End at: 0xFEA9FFFF */
#define SOC_IOMCU_CORE0_RAS_BASE_ADDR                           (0xFEA90000)

/* Size: 64K, End at: 0xFEA8FFFF */
#define SOC_IOMCU_CORE7_MPAM_BASE_ADDR                          (0xFEA80000)

/* Size: 64K, End at: 0xFEA7FFFF */
#define SOC_IOMCU_CORE6_MPAM_BASE_ADDR                          (0xFEA70000)

/* Size: 64K, End at: 0xFEA6FFFF */
#define SOC_IOMCU_CORE5_MPAM_BASE_ADDR                          (0xFEA60000)

/* Size: 64K, End at: 0xFEA5FFFF */
#define SOC_IOMCU_CORE4_MPAM_BASE_ADDR                          (0xFEA50000)

/* Size: 64K, End at: 0xFEA4FFFF */
#define SOC_IOMCU_CORE3_MPAM_BASE_ADDR                          (0xFEA40000)

/* Size: 64K, End at: 0xFEA3FFFF */
#define SOC_IOMCU_CORE2_MPAM_BASE_ADDR                          (0xFEA30000)

/* Size: 64K, End at: 0xFEA2FFFF */
#define SOC_IOMCU_CORE1_MPAM_BASE_ADDR                          (0xFEA20000)

/* Size: 64K, End at: 0xFEA1FFFF */
#define SOC_IOMCU_CORE0_MPAM_BASE_ADDR                          (0xFEA10000)

/* Size: 4K, End at: 0xFEA0FFFF */
#define SOC_IOMCU_CORE7_T1_AMU_BASE_ADDR                        (0xFEA0F000)

/* Size: 4K, End at: 0xFEA0EFFF */
#define SOC_IOMCU_CORE7_T0_AMU_BASE_ADDR                        (0xFEA0E000)

/* Size: 4K, End at: 0xFEA0DFFF */
#define SOC_IOMCU_CORE6_T1_AMU_BASE_ADDR                        (0xFEA0D000)

/* Size: 4K, End at: 0xFEA0CFFF */
#define SOC_IOMCU_CORE6_T0_AMU_BASE_ADDR                        (0xFEA0C000)

/* Size: 4K, End at: 0xFEA0BFFF */
#define SOC_IOMCU_CORE5_T1_AMU_BASE_ADDR                        (0xFEA0B000)

/* Size: 4K, End at: 0xFEA0AFFF */
#define SOC_IOMCU_CORE5_T0_AMU_BASE_ADDR                        (0xFEA0A000)

/* Size: 4K, End at: 0xFEA09FFF */
#define SOC_IOMCU_CORE4_T1_AMU_BASE_ADDR                        (0xFEA09000)

/* Size: 4K, End at: 0xFEA08FFF */
#define SOC_IOMCU_CORE4_T0_AMU_BASE_ADDR                        (0xFEA08000)

/* Size: 4K, End at: 0xFEA06FFF */
#define SOC_IOMCU_CORE3_T0_AMU_BASE_ADDR                        (0xFEA06000)

/* Size: 4K, End at: 0xFEA04FFF */
#define SOC_IOMCU_CORE2_T0_AMU_BASE_ADDR                        (0xFEA04000)

/* Size: 4K, End at: 0xFEA02FFF */
#define SOC_IOMCU_CORE1_T0_AMU_BASE_ADDR                        (0xFEA02000)

/* Size: 4K, End at: 0xFEA00FFF */
#define SOC_IOMCU_CORE0_T0_AMU_BASE_ADDR                        (0xFEA00000)

/* Size: 2M, End at: 0xFE9FFFFF */
#define SOC_IOMCU_GIC600_CFG_BASE_ADDR                          (0xFE800000)

/* Size: 64K, End at: 0xFE52FFFF */
#define SOC_IOMCU_linx_cluster_djtag_lit_BASE_ADDR              (0xFE520000)

/* Size: 4K, End at: 0xFE409FFF */
#define SOC_IOMCU_FCM_LOCAL_IP_LIT_BASE_ADDR                    (0xFE409000)

/* Size: 4K, End at: 0xFE404FFF */
#define SOC_IOMCU_CORE3_LOCAL_CFG_BASE_ADDR                     (0xFE404000)

/* Size: 4K, End at: 0xFE403FFF */
#define SOC_IOMCU_CORE2_LOCAL_CFG_BASE_ADDR                     (0xFE403000)

/* Size: 4K, End at: 0xFE402FFF */
#define SOC_IOMCU_CORE1_LOCAL_CFG_BASE_ADDR                     (0xFE402000)

/* Size: 4K, End at: 0xFE401FFF */
#define SOC_IOMCU_CORE0_LOCAL_CFG_BASE_ADDR                     (0xFE401000)

/* Size: 4K, End at: 0xFE400FFF */
#define SOC_IOMCU_FCM_LOCAL_CPU_LIT_BASE_ADDR                   (0xFE400000)

/* Size: 64K, End at: 0xFE12FFFF */
#define SOC_IOMCU_linx_cluster_djtag_hc_BASE_ADDR               (0xFE120000)

/* Size: 4K, End at: 0xFE009FFF */
#define SOC_IOMCU_FCM_LOCAL_IP_HC_BASE_ADDR                     (0xFE009000)

/* Size: 4K, End at: 0xFE004FFF */
#define SOC_IOMCU_CORE7_LOCAL_CFG_BASE_ADDR                     (0xFE004000)

/* Size: 4K, End at: 0xFE003FFF */
#define SOC_IOMCU_CORE6_LOCAL_CFG_BASE_ADDR                     (0xFE003000)

/* Size: 4K, End at: 0xFE002FFF */
#define SOC_IOMCU_CORE5_LOCAL_CFG_BASE_ADDR                     (0xFE002000)

/* Size: 4K, End at: 0xFE001FFF */
#define SOC_IOMCU_CORE4_LOCAL_CFG_BASE_ADDR                     (0xFE001000)

/* Size: 4K, End at: 0xFE000FFF */
#define SOC_IOMCU_FCM_LOCAL_CPU_HC_BASE_ADDR                    (0xFE000000)


/* ==========================  AO_SUBSYS 5.5MB  =========================== */
/* Size: 1M, End at: 0xFDEFFFFF */
#define SOC_IOMCU_QCP_IOMCUBUS_BASE_ADDR                        (0xFDE00000)

/* Size: 1M, End at: 0xFDDFFFFF */
#define SOC_IOMCU_QCP_MSPCBUS_BASE_ADDR                         (0xFDD00000)

/* Size: 1M, End at: 0xFDCFFFFF */
#define SOC_IOMCU_QCP_AOBUS_BASE_ADDR                           (0xFDC00000)

/* Size: 8K, End at: 0xFDB87FFF */
#define SOC_IOMCU_QSPI_BASE_ADDR                                (0xFDB86000)

/* Size: 8K, End at: 0xFDB85FFF */
#define SOC_IOMCU_FW_AO_BASE_ADDR                               (0xFDB84000)

/* Size: 8K, End at: 0xFDB83FFF */
#define SOC_IOMCU_FW_MSPC_BASE_ADDR                             (0xFDB82000)

/* Size: 8K, End at: 0xFDB81FFF */
#define SOC_IOMCU_FW_IOMCU_BASE_ADDR                            (0xFDB80000)

/* Size: 32K, End at: 0xFDB57FFF */
#define SOC_IOMCU_AO_TCP_BASE_ADDR                              (0xFDB50000)

/* Size: 4K, End at: 0xFDB4FFFF */
#define SOC_IOMCU_intr_hub_ao_BASE_ADDR                         (0xFDB4F000)

/* Size: 4K, End at: 0xFDB4EFFF */
#define SOC_IOMCU_BISR_BASE_ADDR                                (0xFDB4E000)

/* Size: 4K, End at: 0xFDB4DFFF */
#define SOC_IOMCU_AO_CRG_BASE_ADDR                              (0xFDB4D000)

/* Size: 4K, End at: 0xFDB4CFFF */
#define SOC_IOMCU_BLPWM2_BASE_ADDR                              (0xFDB4C000)

/* Size: 8K, End at: 0xFDB4BFFF */
#define SOC_IOMCU_SYS_CNT_BASE_ADDR                             (0xFDB4A000)

/* Size: 4K, End at: 0xFDB48FFF */
#define SOC_IOMCU_hisec_timer_BASE_ADDR                         (0xFDB48000)

/* Size: 32K, End at: 0xFDB47FFF */
#define SOC_IOMCU_BB_DRX_BASE_ADDR                              (0xFDB40000)

/* Size: 4K, End at: 0xFDB37FFF */
#define SOC_IOMCU_dft_clamp_auth_BASE_ADDR                      (0xFDB37000)

/* Size: 4K, End at: 0xFDB36FFF */
#define SOC_IOMCU_GPIO8_BASE_ADDR                               (0xFDB36000)

/* Size: 4K, End at: 0xFDB32FFF */
#define SOC_IOMCU_GPIO7_BASE_ADDR                               (0xFDB32000)

/* Size: 4K, End at: 0xFDB2EFFF */
#define SOC_IOMCU_GPIO6_BASE_ADDR                               (0xFDB2E000)

/* Size: 4K, End at: 0xFDB2AFFF */
#define SOC_IOMCU_GPIO5_BASE_ADDR                               (0xFDB2A000)

/* Size: 4K, End at: 0xFDB29FFF */
#define SOC_IOMCU_PAD_AO_IOMG_IOCG_BASE_ADDR                    (0xFDB29000)

/* Size: 4K, End at: 0xFDB28FFF */
#define SOC_IOMCU_IOC_AO_IOMG_BASE_ADDR                         (0xFDB28000)

/* Size: 16K, End at: 0xFDB27FFF */
#define SOC_IOMCU_SPMI_BASE_ADDR                                (0xFDB24000)

/* Size: 4K, End at: 0xFDB23FFF */
#define SOC_IOMCU_SCTRL_BASE_ADDR                               (0xFDB23000)

/* Size: 4K, End at: 0xFDB22FFF */
#define SOC_IOMCU_ACTRL_BASE_ADDR                               (0xFDB22000)

/* Size: 4K, End at: 0xFDB21FFF */
#define SOC_IOMCU_AO_IPC_S_BASE_ADDR                            (0xFDB21000)

/* Size: 4K, End at: 0xFDB20FFF */
#define SOC_IOMCU_TIMER17_BASE_ADDR                             (0xFDB20000)

/* Size: 4K, End at: 0xFDB1FFFF */
#define SOC_IOMCU_TIMER16_BASE_ADDR                             (0xFDB1F000)

/* Size: 4K, End at: 0xFDB1EFFF */
#define SOC_IOMCU_GPIO1_SE_BASE_ADDR                            (0xFDB1E000)

/* Size: 4K, End at: 0xFDB1DFFF */
#define SOC_IOMCU_AO_WDG_BASE_ADDR                              (0xFDB1D000)

/* Size: 4K, End at: 0xFDB1CFFF */
#define SOC_IOMCU_SPI3_BASE_ADDR                                (0xFDB1C000)

/* Size: 4K, End at: 0xFDB1BFFF */
#define SOC_IOMCU_AO_TZPC_BASE_ADDR                             (0xFDB1B000)

/* Size: 4K, End at: 0xFDB1AFFF */
#define SOC_IOMCU_AO_LoadMonitor_BASE_ADDR                      (0xFDB1A000)

/* Size: 4K, End at: 0xFDB19FFF */
#define SOC_IOMCU_EM_SYNC_TOP_BASE_ADDR                         (0xFDB19000)

/* Size: 4K, End at: 0xFDB18FFF */
#define SOC_IOMCU_AO_WDG1_BASE_ADDR                             (0xFDB18000)

/* Size: 4K, End at: 0xFDB17FFF */
#define SOC_IOMCU_EFUSEC_BASE_ADDR                              (0xFDB17000)

/* Size: 4K, End at: 0xFDB16FFF */
#define SOC_IOMCU_EFUSEC2_BASE_ADDR                             (0xFDB16000)

/* Size: 4K, End at: 0xFDB15FFF */
#define SOC_IOMCU_AO_IPC_NS_BASE_ADDR                           (0xFDB15000)

/* Size: 4K, End at: 0xFDB12FFF */
#define SOC_IOMCU_SPI5_BASE_ADDR                                (0xFDB12000)

/* Size: 4K, End at: 0xFDB10FFF */
#define SOC_IOMCU_CLK_MONITOR_BASE_ADDR                         (0xFDB10000)

/* Size: 4K, End at: 0xFDB0FFFF */
#define SOC_IOMCU_HISIMC1_BASE_ADDR                             (0xFDB0F000)

/* Size: 4K, End at: 0xFDB0EFFF */
#define SOC_IOMCU_HISIMC0_BASE_ADDR                             (0xFDB0E000)

/* Size: 4K, End at: 0xFDB0DFFF */
#define SOC_IOMCU_RTC1_BASE_ADDR                                (0xFDB0D000)

/* Size: 4K, End at: 0xFDB0CFFF */
#define SOC_IOMCU_RTC0_BASE_ADDR                                (0xFDB0C000)

/* Size: 4K, End at: 0xFDB0BFFF */
#define SOC_IOMCU_TIMER15_BASE_ADDR                             (0xFDB0B000)

/* Size: 4K, End at: 0xFDB0AFFF */
#define SOC_IOMCU_TIMER14_BASE_ADDR                             (0xFDB0A000)

/* Size: 4K, End at: 0xFDB09FFF */
#define SOC_IOMCU_TIMER13_BASE_ADDR                             (0xFDB09000)

/* Size: 4K, End at: 0xFDB08FFF */
#define SOC_IOMCU_TIMER8_BASE_ADDR                              (0xFDB08000)

/* Size: 4K, End at: 0xFDB07FFF */
#define SOC_IOMCU_TIMER7_BASE_ADDR                              (0xFDB07000)

/* Size: 4K, End at: 0xFDB06FFF */
#define SOC_IOMCU_TIMER6_BASE_ADDR                              (0xFDB06000)

/* Size: 4K, End at: 0xFDB05FFF */
#define SOC_IOMCU_TIMER5_BASE_ADDR                              (0xFDB05000)

/* Size: 4K, End at: 0xFDB04FFF */
#define SOC_IOMCU_TIMER4_BASE_ADDR                              (0xFDB04000)

/* Size: 4K, End at: 0xFDB03FFF */
#define SOC_IOMCU_TIMER3_BASE_ADDR                              (0xFDB03000)

/* Size: 4K, End at: 0xFDB02FFF */
#define SOC_IOMCU_TIMER2_BASE_ADDR                              (0xFDB02000)

/* Size: 4K, End at: 0xFDB01FFF */
#define SOC_IOMCU_TIMER1_BASE_ADDR                              (0xFDB01000)

/* Size: 4K, End at: 0xFDB00FFF */
#define SOC_IOMCU_TIMER0_BASE_ADDR                              (0xFDB00000)

/* Size: 4K, End at: 0xFDA82FFF */
#define SOC_IOMCU_MAD_DSPIF_BASE_ADDR                           (0xFDA82000)

/* Size: 4K, End at: 0xFDA81FFF */
#define SOC_IOMCU_MAD_TIMER_BASE_ADDR                           (0xFDA81000)

/* Size: 4K, End at: 0xFDA80FFF */
#define SOC_IOMCU_MAD_CFG_BASE_ADDR                             (0xFDA80000)


/* =========================  IOMCU_SUBSYS 4.5MB  ========================= */
/* Size: 4K, End at: 0xFDA52FFF */
#define SOC_IOMCU_IOMCU_P2J_BASE_ADDR                                 (0xFDA52000)

/* Size: 4K, End at: 0xFDA51FFF */
#define SOC_IOMCU_RemapCtrl_BASE_ADDR                           (0xFDA51000)

/* Size: 4K, End at: 0xFDA50FFF */
#define SOC_IOMCU_IOMCU_DMMU_BASE_ADDR                                (0xFDA50000)

/* Size: 4K, End at: 0xFDA4EFFF */
#define SOC_IOMCU_IOMCU_HTS_slv_adapt_BASE_ADDR                       (0xFDA4E000)

/* Size: 4K, End at: 0xFDA4DFFF */
#define SOC_IOMCU_IOMCU_HTS_mst_adapt_BASE_ADDR                       (0xFDA4D000)

/* Size: 4K, End at: 0xFDA4CFFF */
#define SOC_IOMCU_IOMCU_SPI0_BASE_ADDR                                (0xFDA4C000)

/* Size: 4K, End at: 0xFDA4BFFF */
#define SOC_IOMCU_IOMCU_SPI8_BASE_ADDR                                (0xFDA4B000)

/* Size: 4K, End at: 0xFDA4AFFF */
#define SOC_IOMCU_IOMCU_DMAC_BASE_ADDR                                (0xFDA4A000)

/* Size: 4K, End at: 0xFDA49FFF */
#define SOC_IOMCU_IOMCU_GPIO_BASE_ADDR                                (0xFDA49000)

/* Size: 4K, End at: 0xFDA48FFF */
#define SOC_IOMCU_IOMCU_I3C1_BASE_ADDR                                (0xFDA48000)

/* Size: 4K, End at: 0xFDA47FFF */
#define SOC_IOMCU_IOMCU_I2C1_BASE_ADDR                                (0xFDA47000)

/* Size: 4K, End at: 0xFDA46FFF */
#define SOC_IOMCU_IOMCU_BLPWM0_BASE_ADDR                              (0xFDA46000)

/* Size: 4K, End at: 0xFDA45FFF */
#define SOC_IOMCU_IOMCU_Ftimer0_BASE_ADDR                             (0xFDA45000)

/* Size: 4K, End at: 0xFDA44FFF */
#define SOC_IOMCU_IOMCU_Ftimer1_BASE_ADDR                             (0xFDA44000)

/* Size: 4K, End at: 0xFDA43FFF */
#define SOC_IOMCU_IOMCU_TIMER_BASE_ADDR                               (0xFDA43000)

/* Size: 4K, End at: 0xFDA42FFF */
#define SOC_IOMCU_IOMCU_WDG_BASE_ADDR                                 (0xFDA42000)

/* Size: 4K, End at: 0xFDA41FFF */
#define SOC_IOMCU_IOMCU_RTC_BASE_ADDR                                 (0xFDA41000)

/* Size: 4K, End at: 0xFDA40FFF */
#define SOC_IOMCU_IOMCU_CONFIG_BASE_ADDR                              (0xFDA40000)

/* Size: 4K, End at: 0xFDA20FFF */
#define SOC_IOMCU_IOMCU_HTS_SLV_MSG_BASE_ADDR                         (0xFDA20000)

/* Size: 4K, End at: 0xFDA0CFFF */
#define SOC_IOMCU_IOMCU_I2C11_BASE_ADDR                               (0xFDA0C000)

/* Size: 4K, End at: 0xFDA0BFFF */
#define SOC_IOMCU_IOMCU_I2C10_BASE_ADDR                               (0xFDA0B000)

/* Size: 4K, End at: 0xFDA0AFFF */
#define SOC_IOMCU_IOMCU_SPI2_BASE_ADDR                                (0xFDA0A000)

/* Size: 4K, End at: 0xFDA09FFF */
#define SOC_IOMCU_IOMCU_SPI4_BASE_ADDR                                (0xFDA09000)

/* Size: 4K, End at: 0xFDA08FFF */
#define SOC_IOMCU_IOMCU_I2C8_BASE_ADDR                                (0xFDA08000)

/* Size: 4K, End at: 0xFDA07FFF */
#define SOC_IOMCU_IOMCU_I2C5_BASE_ADDR                                (0xFDA07000)

/* Size: 4K, End at: 0xFDA06FFF */
#define SOC_IOMCU_IOMCU_I2C0_BASE_ADDR                                (0xFDA06000)

/* Size: 4K, End at: 0xFDA05FFF */
#define SOC_IOMCU_IOMCU_I3C3_BASE_ADDR                                (0xFDA05000)

/* Size: 4K, End at: 0xFDA04FFF */
#define SOC_IOMCU_IOMCU_I3C2_BASE_ADDR                                (0xFDA04000)

/* Size: 4K, End at: 0xFDA03FFF */
#define SOC_IOMCU_IOMCU_I3C_BASE_ADDR                                 (0xFDA03000)

/* Size: 4K, End at: 0xFDA01FFF */
#define SOC_IOMCU_IOMCU_UART7_BASE_ADDR                               (0xFDA01000)

/* Size: 4K, End at: 0xFDA00FFF */
#define SOC_IOMCU_IOMCU_UART3_BASE_ADDR                               (0xFDA00000)


/* ==========================  MSPC SUBSYS 2MB  =========================== */
/* Size: 4K, End at: 0xFD430FFF */
#define SOC_IOMCU_HISEE_IPC_BASE_ADDR                           (0xFD430000)

/* Size: 1K, End at: 0xFD4203FF */
#define SOC_IOMCU_HISEE_MAILBOX_BASE_ADDR                       (0xFD420000)


/* ===========================  ASP SUBSYS 4MB  =========================== */
/* Size: 4K, End at: 0xFD302FFF */
#define SOC_IOMCU_ASP_HTS_SLV_MSG_BASE_ADDR                     (0xFD302000)

/* Size: 1M, End at: 0xFD2FFFFF */
#define SOC_IOMCU_QCP_ASP_BASE_ADDR                             (0xFD200000)

/* Size: 4K, End at: 0xFD1E1FFF */
#define SOC_IOMCU_AUDIO_MULTI_ACC_CFG_BASE_ADDR                 (0xFD1E1000)

/* Size: 4K, End at: 0xFD1E0FFF */
#define SOC_IOMCU_ASP_HTS_SLV_ADAPT_BASE_ADDR                   (0xFD1E0000)

/* Size: 4K, End at: 0xFD1DFFFF */
#define SOC_IOMCU_ASP_HTS_MST_ADAPT_BASE_ADDR                   (0xFD1DF000)

/* Size: 4K, End at: 0xFD1DEFFF */
#define SOC_IOMCU_ASP_UART_BASE_ADDR                            (0xFD1DE000)

/* Size: 4K, End at: 0xFD1DDFFF */
#define SOC_IOMCU_ACCESS_REGION_HDMI_BASE_ADDR                  (0xFD1DD000)

/* Size: 4K, End at: 0xFD1DCFFF */
#define SOC_IOMCU_ACCESS_REGION_EDMAC_BASE_ADDR                 (0xFD1DC000)

/* Size: 4K, End at: 0xFD1DBFFF */
#define SOC_IOMCU_ACCESS_REGION_DSP_BASE_ADDR                   (0xFD1DB000)

/* Size: 4K, End at: 0xFD1DAFFF */
#define SOC_IOMCU_AXI2TDM_CFG_BASE_ADDR                         (0xFD1DA000)

/* Size: 4K, End at: 0xFD1D9FFF */
#define SOC_IOMCU_DMMU_HDMI_BASE_ADDR                           (0xFD1D9000)

/* Size: 4K, End at: 0xFD1D8FFF */
#define SOC_IOMCU_DMMU_EDMAC_BASE_ADDR                          (0xFD1D8000)

/* Size: 48K, End at: 0xFD19CFFF */
#define SOC_IOMCU_dss_itcm_BASE_ADDR                            (0xFD191000)

/* Size: 160K, End at: 0xFD190FFF */
#define SOC_IOMCU_dss_dtcm_BASE_ADDR                            (0xFD169000)

/* Size: 4K, End at: 0xFD168FFF */
#define SOC_IOMCU_dsp_mp_mpscu_registers_BASE_ADDR              (0xFD168000)

/* Size: 64K, End at: 0xFD167FFF */
#define SOC_IOMCU_dsp_mp_dss0_registers_BASE_ADDR               (0xFD158000)

/* Size: 16K, End at: 0xFD157FFF */
#define SOC_IOMCU_soundwire_BASE_ADDR                           (0xFD154000)

/* Size: 16K, End at: 0xFD153FFF */
#define SOC_IOMCU_ASP_CODEC_BASE_ADDR                           (0xFD150000)

/* Size: 4K, End at: 0xFD14FFFF */
#define SOC_IOMCU_ASP_CFG_BASE_ADDR                             (0xFD14F000)

/* Size: 1K, End at: 0xFD14EFFF */
#define SOC_IOMCU_ASP_HDMI_SPDIF_BASE_ADDR                      (0xFD14EC00)

/* Size: 1K, End at: 0xFD14EBFF */
#define SOC_IOMCU_ASP_HDMI_SIO_BASE_ADDR                        (0xFD14E800)

/* Size: 1K, End at: 0xFD14E7FF */
#define SOC_IOMCU_ASP_HDMI_ASP_BASE_ADDR                        (0xFD14E400)

/* Size: 4K, End at: 0xFD14DFFF */
#define SOC_IOMCU_ASP_WD_BASE_ADDR                              (0xFD14D000)

/* Size: 4K, End at: 0xFD14CFFF */
#define SOC_IOMCU_ASP_IPC_BASE_ADDR                             (0xFD14C000)

/* Size: 4K, End at: 0xFD14BFFF */
#define SOC_IOMCU_ASP_DMAC_BASE_ADDR                            (0xFD14B000)

/* Size: 4K, End at: 0xFD14AFFF */
#define SOC_IOMCU_ASP_TIMER1_BASE_ADDR                          (0xFD14A000)

/* Size: 4K, End at: 0xFD149FFF */
#define SOC_IOMCU_ASP_TIMER0_BASE_ADDR                          (0xFD149000)

/* Size: 4K, End at: 0xFD148FFF */
#define SOC_IOMCU_ASP_GPIO_BASE_ADDR                            (0xFD148000)

/* Size: 4K, End at: 0xFD147FFF */
#define SOC_IOMCU_ASP_DMMU_SRT_BASE_ADDR                        (0xFD147000)

/* Size: 64K, End at: 0xFD12FFFF */
#define SOC_IOMCU_AXI2TDM_DATA_BASE_ADDR                        (0xFD120000)

/* Size: 4K, End at: 0xFD100FFF */
#define SOC_IOMCU_AUDIO_MULTI_ACC_DATA_BASE_ADDR                (0xFD100000)

/* Size: 256K, End at: 0xFD0BFFFF */
#define SOC_IOMCU_SECRAM_BASE_ADDR                              (0xFD080000)


/* =================  LPMCU SUBSYS PART2 80MB;TOTAL 88MB  ================= */
/* Size: 80M, End at: 0xFCFFFFFF */
#define SOC_IOMCU_CSSYS_APB_BASE_ADDR                           (0xF8000000)


/* ==========================  MDM SUBSYS 128MB  ========================== */
/* Size: 192K, End at: 0xF6A2FFFF */
#define SOC_IOMCU_TSP_DSS0_L1ITCM_GLOBAL_BASE_ADDR              (0xF6A00000)

/* Size: 256K, End at: 0xF683FFFF */
#define SOC_IOMCU_TSP_DSS0_L1DTCM_GLOBAL_BASE_ADDR              (0xF6800000)

/* Size: 192K, End at: 0xF65AFFFF */
#define SOC_IOMCU_TSP_L1ITCM_LOCAL_BASE_ADDR                    (0xF6580000)

/* Size: 256K, End at: 0xF653FFFF */
#define SOC_IOMCU_TSP_L1DTCM_LOCAL_BASE_ADDR                    (0xF6500000)

/* Size: 2M, End at: 0xF5FFFFFF */
#define SOC_IOMCU_TSP_REG_BASE_ADDR                             (0xF5E00000)

/* Size: 1M, End at: 0xF51FFFFF */
#define SOC_IOMCU_TSP_QIC_QCP_BASE_ADDR                         (0xF5100000)

/* Size: 4K, End at: 0xF501DFFF */
#define SOC_IOMCU_TSP_TIMER_9_BASE_ADDR                         (0xF501D000)

/* Size: 4K, End at: 0xF501CFFF */
#define SOC_IOMCU_TSP_TIMER_8_BASE_ADDR                         (0xF501C000)

/* Size: 4K, End at: 0xF501BFFF */
#define SOC_IOMCU_TSP_TIMER_7_BASE_ADDR                         (0xF501B000)

/* Size: 4K, End at: 0xF501AFFF */
#define SOC_IOMCU_TSP_TIMER_6_BASE_ADDR                         (0xF501A000)

/* Size: 4K, End at: 0xF5019FFF */
#define SOC_IOMCU_TSP_TIMER_5_BASE_ADDR                         (0xF5019000)

/* Size: 4K, End at: 0xF5018FFF */
#define SOC_IOMCU_TSP_TIMER_4_BASE_ADDR                         (0xF5018000)

/* Size: 4K, End at: 0xF5017FFF */
#define SOC_IOMCU_TSP_TIMER_3_BASE_ADDR                         (0xF5017000)

/* Size: 4K, End at: 0xF5016FFF */
#define SOC_IOMCU_TSP_TIMER_2_BASE_ADDR                         (0xF5016000)

/* Size: 4K, End at: 0xF5015FFF */
#define SOC_IOMCU_TSP_TIMER_1_BASE_ADDR                         (0xF5015000)

/* Size: 4K, End at: 0xF5014FFF */
#define SOC_IOMCU_TSP_TIMER_0_BASE_ADDR                         (0xF5014000)

/* Size: 4K, End at: 0xF5001FFF */
#define SOC_IOMCU_TSP_INT_CTRL_BASE_ADDR                        (0xF5001000)

/* Size: 4K, End at: 0xF5000FFF */
#define SOC_IOMCU_TSP_SYSCTRL_BASE_ADDR                         (0xF5000000)

/* Size: 1M, End at: 0xF45FFFFF */
#define SOC_IOMCU_MDM_QIC_QCP_BASE_ADDR                         (0xF4500000)

/* Size: 40K, End at: 0xF44C9FFF */
#define SOC_IOMCU_MDM_MAXS_BASE_ADDR                            (0xF44C0000)

/* Size: 512K, End at: 0xF447FFFF */
#define SOC_IOMCU_MDM_L2DLE_BASE_ADDR                           (0xF4400000)

/* Size: 16K, End at: 0xF4243FFF */
#define SOC_IOMCU_MDM_PM_CTRL_BASE_ADDR                         (0xF4240000)

/* Size: 8K, End at: 0xF423BFFF */
#define SOC_IOMCU_MDM_SYSCNT_BASE_ADDR                          (0xF423A000)

/* Size: 8K, End at: 0xF4235FFF */
#define SOC_IOMCU_MDM_EAGLE_EYE_BASE_ADDR                       (0xF4234000)

/* Size: 4K, End at: 0xF4232FFF */
#define SOC_IOMCU_MDM_TRACE_BASE_ADDR                           (0xF4232000)

/* Size: 4K, End at: 0xF4230FFF */
#define SOC_IOMCU_MDM_ADDR_MONITOR_BASE_ADDR                    (0xF4230000)

/* Size: 4K, End at: 0xF422CFFF */
#define SOC_IOMCU_MDM_LOAD_MONITOR_BASE_ADDR                    (0xF422C000)

/* Size: 4K, End at: 0xF422AFFF */
#define SOC_IOMCU_MDM_GID_CTRL_MDM_BASE_ADDR                    (0xF422A000)

/* Size: 4K, End at: 0xF4226FFF */
#define SOC_IOMCU_MDM_UART_BASE_ADDR                            (0xF4226000)

/* Size: 4K, End at: 0xF4205FFF */
#define SOC_IOMCU_MDM_WATCHDOG_1_BASE_ADDR                      (0xF4205000)

/* Size: 4K, End at: 0xF4204FFF */
#define SOC_IOMCU_MDM_WATCHDOG_0_BASE_ADDR                      (0xF4204000)

/* Size: 4K, End at: 0xF4202FFF */
#define SOC_IOMCU_MDM_FIX_CRG_BASE_ADDR                         (0xF4202000)

/* Size: 4K, End at: 0xF4201FFF */
#define SOC_IOMCU_MDM_SYSCTRL_BASE_ADDR                         (0xF4201000)

/* Size: 4K, End at: 0xF4200FFF */
#define SOC_IOMCU_MDM_CRG_BASE_ADDR                             (0xF4200000)

/* Size: 8M, End at: 0xF37FFFFF */
#define SOC_IOMCU_SOC2PDEA_BASE_ADDR                            (0xF3000000)

/* Size: 16M, End at: 0xF1FFFFFF */
#define SOC_IOMCU_SOC2FPDF_BASE_ADDR                            (0xF1000000)

/* Size: 1M, End at: 0xF08FFFFF */
#define SOC_IOMCU_BBA_GUS_BASE_ADDR                             (0xF0800000)

/* Size: 2M, End at: 0xF01FFFFF */
#define SOC_IOMCU_FPDT_BASE_ADDR                                (0xF0000000)


/* =========================  HSDT0 SUBSYS 128MB  ========================= */
/* Size: 8K, End at: 0xEEC03FFF */
#define SOC_IOMCU_FW_HSDT0_BASE_ADDR                            (0xEEC02000)

/* Size: 256K, End at: 0xEEABFFFF */
#define SOC_IOMCU_MUDP_PHY_BASE_ADDR                            (0xEEA80000)

/* Size: 64K, End at: 0xEEA7FFFF */
#define SOC_IOMCU_MUDP_CTRL_BASE_ADDR                           (0xEEA70000)

/* Size: 24K, End at: 0xEEA6DFFF */
#define SOC_IOMCU_INTR_PAR_BASE_ADDR                            (0xEEA68000)

/* Size: 4K, End at: 0xEEA67FFF */
#define SOC_IOMCU_GPIO3_BASE_ADDR                               (0xEEA67000)

/* Size: 2K, End at: 0xEEA66FFF */
#define SOC_IOMCU_HSDT0_PAD_CTRL_BASE_ADDR                      (0xEEA66800)

/* Size: 2K, End at: 0xEEA667FF */
#define SOC_IOMCU_HSDT0_PAD_MUX_BASE_ADDR                       (0xEEA66000)

/* Size: 4K, End at: 0xEEA65FFF */
#define SOC_IOMCU_HSDT0_IOC_BASE_ADDR                           (0xEEA65000)

/* Size: 4K, End at: 0xEEA64FFF */
#define SOC_IOMCU_HSDT0_SCTRL_BASE_ADDR                         (0xEEA64000)

/* Size: 4K, End at: 0xEEA63FFF */
#define SOC_IOMCU_hsdt0_crg_fnpll_pcie_BASE_ADDR                (0xEEA63000)

/* Size: 4K, End at: 0xEEA62FFF */
#define SOC_IOMCU_HSDT0_CRG_BASE_ADDR                           (0xEEA62000)

/* Size: 256K, End at: 0xEEA3FFFF */
#define SOC_IOMCU_PCIE_TCU_BASE_ADDR                            (0xEEA00000)

/* Size: 4K, End at: 0xEE9A3FFF */
#define SOC_IOMCU_PCIE1_APB_CTRL_BASE_ADDR                      (0xEE9A3000)

/* Size: 12K, End at: 0xEE9A2FFF */
#define SOC_IOMCU_PCIE1_PHY_BASE_ADDR                           (0xEE9A0000)

/* Size: 128K, End at: 0xEE99FFFF */
#define SOC_IOMCU_PCIE1_TBU_BASE_ADDR                           (0xEE980000)

/* Size: 512K, End at: 0xEE97FFFF */
#define SOC_IOMCU_PCIECtrl1_DBI_BASE_ADDR                       (0xEE900000)

/* Size: 4K, End at: 0xEE8A3FFF */
#define SOC_IOMCU_PCIE0_APB_CTRL_BASE_ADDR                      (0xEE8A3000)

/* Size: 12K, End at: 0xEE8A2FFF */
#define SOC_IOMCU_PCIE0_PHY_BASE_ADDR                           (0xEE8A0000)

/* Size: 128K, End at: 0xEE89FFFF */
#define SOC_IOMCU_PCIE0_TBU_BASE_ADDR                           (0xEE880000)

/* Size: 512K, End at: 0xEE87FFFF */
#define SOC_IOMCU_PCIECtrl0_DBI_BASE_ADDR                       (0xEE800000)

/* Size: 1M, End at: 0xEE6FFFFF */
#define SOC_IOMCU_UFS_UNIPRO_BASE_ADDR                          (0xEE600000)

/* Size: 512K, End at: 0xEE5FFFFF */
#define SOC_IOMCU_UFS_MPHY_BASE_ADDR                            (0xEE580000)

/* Size: 128K, End at: 0xEE57FFFF */
#define SOC_IOMCU_UFS_HCI_BASE_ADDR                             (0xEE560000)

/* Size: 4K, End at: 0xEE55FFFF */
#define SOC_IOMCU_UFS_SYS_CTRL_BASE_ADDR                        (0xEE55F000)

/* Size: 4K, End at: 0xEE55EFFF */
#define SOC_IOMCU_UFS_MPHY_DSC_BASE_ADDR                        (0xEE55E000)

/* Size: 8K, End at: 0xEE55DFFF */
#define SOC_IOMCU_UFS_TRANC_UNIT_BASE_ADDR                      (0xEE55C000)

/* Size: 8K, End at: 0xEE55BFFF */
#define SOC_IOMCU_DFA_QUE_PROC_BASE_ADDR                        (0xEE55A000)

/* Size: 4K, End at: 0xEE559FFF */
#define SOC_IOMCU_DFA_SYS_CTRL_BASE_ADDR                        (0xEE559000)

/* Size: 4K, End at: 0xEE550FFF */
#define SOC_IOMCU_UFS_KM_BASE_ADDR                              (0xEE550000)

/* Size: 128K, End at: 0xEE4FFFFF */
#define SOC_IOMCU_DFA_QCP_BASE_ADDR                             (0xEE4E0000)

/* Size: 384K, End at: 0xEE4DFFFF */
#define SOC_IOMCU_DFA_DTCM_BASE_ADDR                            (0xEE480000)

/* Size: 1M, End at: 0xEE3FFFFF */
#define SOC_IOMCU_QCP_HSDT0BUS_BASE_ADDR                        (0xEE300000)

/* Size: 4K, End at: 0xEE282FFF */
#define SOC_IOMCU_TB_EPS_SCE_REE_BASE_ADDR                      (0xEE282000)

/* Size: 4K, End at: 0xEE281FFF */
#define SOC_IOMCU_TB_EPS_KM_REE_BASE_ADDR                       (0xEE281000)

/* Size: 4K, End at: 0xEE280FFF */
#define SOC_IOMCU_TB_EPS_CONFIG_REE_BASE_ADDR                   (0xEE280000)

/* Size: 160K, End at: 0xEE239FFF */
#define SOC_IOMCU_TB_EPS_PKE_TEE_BASE_ADDR                      (0xEE212000)

/* Size: 4K, End at: 0xEE211FFF */
#define SOC_IOMCU_TB_EPS_SCE_TEE_BASE_ADDR                      (0xEE211000)

/* Size: 4K, End at: 0xEE210FFF */
#define SOC_IOMCU_TB_EPS_KM_TEE_BASE_ADDR                       (0xEE210000)

/* Size: 4K, End at: 0xEE20FFFF */
#define SOC_IOMCU_TB_EPS_SCE2_TEE_BASE_ADDR                     (0xEE20F000)

/* Size: 8K, End at: 0xEE203FFF */
#define SOC_IOMCU_TB_EPS_TRNG_TEE_BASE_ADDR                     (0xEE202000)

/* Size: 4K, End at: 0xEE201FFF */
#define SOC_IOMCU_TB_EPS_TZPC_TEE_BASE_ADDR                     (0xEE201000)

/* Size: 4K, End at: 0xEE200FFF */
#define SOC_IOMCU_TB_EPS_CONFIG_TEE_BASE_ADDR                   (0xEE200000)

/* Size: 32M, End at: 0xEDFFFFFF */
#define SOC_IOMCU_PCIECtrl0_BASE_ADDR                           (0xEC000000)

/* Size: 64M, End at: 0xEBFFFFFF */
#define SOC_IOMCU_PCIECtrl1_BASE_ADDR                           (0xE8000000)


/* ==========================  HSDT1 SUBSYS 8MB  ========================== */
/* Size: 8K, End at: 0xE6601FFF */
#define SOC_IOMCU_FW_HSDT1_BASE_ADDR                            (0xE6600000)

/* Size: 4K, End at: 0xE6500FFF */
#define SOC_IOMCU_HSDT1_UFS_KM_BASE_ADDR                        (0xE6500000)

/* Size: 1M, End at: 0xE64FFFFF */
#define SOC_IOMCU_USB_CTRL_BASE_ADDR                            (0xE6400000)

/* Size: 1M, End at: 0xE63FFFFF */
#define SOC_IOMCU_QCP_HSDT1BUS_BASE_ADDR                        (0xE6300000)

/* Size: 512K, End at: 0xE62FFFFF */
#define SOC_IOMCU_HSDT1_UFS_MPHY_BASE_ADDR                      (0xE6280000)

/* Size: 4K, End at: 0xE627FFFF */
#define SOC_IOMCU_HSDT1_UFS_MPHY_DSC_BASE_ADDR                  (0xE627F000)

/* Size: 4K, End at: 0xE6206FFF */
#define SOC_IOMCU_GPIO11_BASE_ADDR                              (0xE6206000)

/* Size: 4K, End at: 0xE6205FFF */
#define SOC_IOMCU_GPIO10_BASE_ADDR                              (0xE6205000)

/* Size: 2K, End at: 0xE6204FFF */
#define SOC_IOMCU_HSDT1_PAD_CTRL_BASE_ADDR                      (0xE6204800)

/* Size: 2K, End at: 0xE62047FF */
#define SOC_IOMCU_HSDT1_PAD_MUX_BASE_ADDR                       (0xE6204000)

/* Size: 4K, End at: 0xE6203FFF */
#define SOC_IOMCU_IOC_HSDT1_BASE_ADDR                           (0xE6203000)

/* Size: 4K, End at: 0xE6202FFF */
#define SOC_IOMCU_HSDT1_CRG_BASE_ADDR                           (0xE6202000)

/* Size: 4K, End at: 0xE6201FFF */
#define SOC_IOMCU_SD3_BASE_ADDR                                 (0xE6201000)

/* Size: 4K, End at: 0xE6200FFF */
#define SOC_IOMCU_HSDT1_SCTRL_BASE_ADDR                         (0xE6200000)

/* Size: 4K, End at: 0xE614DFFF */
#define SOC_IOMCU_USBDP_SYS_CTRL_BASE_ADDR                      (0xE614D000)

/* Size: 4K, End at: 0xE614BFFF */
#define SOC_IOMCU_USBDP_DPTX_CTRL_BASE_ADDR                     (0xE614B000)

/* Size: 3K, End at: 0xE6149FFF */
#define SOC_IOMCU_USBDP_USB2PHY_SCTRL_BASE_ADDR                 (0xE6149400)

/* Size: 1K, End at: 0xE61493FF */
#define SOC_IOMCU_USBDP_USB2PHY_BASE_ADDR                       (0xE6149000)

/* Size: 32K, End at: 0xE6147FFF */
#define SOC_IOMCU_USBDP_PHY_BASE_ADDR                           (0xE6140000)

/* Size: 8K, End at: 0xE6123FFF */
#define SOC_IOMCU_HSDT1_UFS_TRACE_BASE_ADDR                     (0xE6122000)

/* Size: 4K, End at: 0xE6120FFF */
#define SOC_IOMCU_HSDT1_UFS_SYS_CTRL_BASE_ADDR                  (0xE6120000)

/* Size: 128K, End at: 0xE611FFFF */
#define SOC_IOMCU_HSDT1_UFS_UFSHC_BASE_ADDR                     (0xE6100000)

/* Size: 1M, End at: 0xE60FFFFF */
#define SOC_IOMCU_HSDT1_UFS_UNIPRO_BASE_ADDR                    (0xE6000000)


/* =========================  NPU SUBSYS （32MB)  ========================= */
/* Size: 256K, End at: 0xE5FFFFFF */
#define SOC_IOMCU_smmu_tcu_cfg_BASE_ADDR                        (0xE5FC0000)

/* Size: 128K, End at: 0xE5F5FFFF */
#define SOC_IOMCU_aic0_smmu_cfg_BASE_ADDR                       (0xE5F40000)

/* Size: 4K, End at: 0xE5F33FFF */
#define SOC_IOMCU_easc1_cfg_BASE_ADDR                           (0xE5F33000)

/* Size: 4K, End at: 0xE5F32FFF */
#define SOC_IOMCU_easc0_cfg_BASE_ADDR                           (0xE5F32000)

/* Size: 4K, End at: 0xE5F31FFF */
#define SOC_IOMCU_dmmu1_cfg_BASE_ADDR                           (0xE5F31000)

/* Size: 4K, End at: 0xE5F30FFF */
#define SOC_IOMCU_dmmu0_cfg_BASE_ADDR                           (0xE5F30000)

/* Size: 4K, End at: 0xE5E09FFF */
#define SOC_IOMCU_npu_sourcelock_BASE_ADDR                      (0xE5E09000)

/* Size: 4K, End at: 0xE5E08FFF */
#define SOC_IOMCU_npu_tiny_sctrl_BASE_ADDR                      (0xE5E08000)

/* Size: 4K, End at: 0xE5E07FFF */
#define SOC_IOMCU_npu_lite_sctrl_BASE_ADDR                      (0xE5E07000)

/* Size: 4K, End at: 0xE5E06FFF */
#define SOC_IOMCU_npu_pcr_BASE_ADDR                             (0xE5E06000)

/* Size: 8K, End at: 0xE5E04FFF */
#define SOC_IOMCU_npu_dpm_BASE_ADDR                             (0xE5E03000)

/* Size: 4K, End at: 0xE5E02FFF */
#define SOC_IOMCU_npu_sysctrl_BASE_ADDR                         (0xE5E02000)

/* Size: 4K, End at: 0xE5E01FFF */
#define SOC_IOMCU_npu_crg_BASE_ADDR                             (0xE5E01000)

/* Size: 4K, End at: 0xE5E00FFF */
#define SOC_IOMCU_npu_tzpc_BASE_ADDR                            (0xE5E00000)

/* Size: 1M, End at: 0xE51FFFFF */
#define SOC_IOMCU_AICORE1_CFG_BASE_ADDR                         (0xE5100000)

/* Size: 512K, End at: 0xE50FFFFF */
#define SOC_IOMCU_aiv0_subsys_cfg_BASE_ADDR                     (0xE5080000)

/* Size: 512K, End at: 0xE507FFFF */
#define SOC_IOMCU_aic0_subsys_cfg_BASE_ADDR                     (0xE5000000)

/* Size: 1M, End at: 0xE4CFFFFF */
#define SOC_IOMCU_QCP_NPUBUS_BASE_ADDR                          (0xE4C00000)

/* Size: 8K, End at: 0xE4505FFF */
#define SOC_IOMCU_FFAP1_NPU_BASE_ADDR                           (0xE4504000)

/* Size: 4K, End at: 0xE4503FFF */
#define SOC_IOMCU_NPU_TSCPU_HTS_SLV_ADAPT_BASE_ADDR             (0xE4503000)

/* Size: 4K, End at: 0xE4502FFF */
#define SOC_IOMCU_NPU_TSCPU_HTS_MST_ADAPT_BASE_ADDR             (0xE4502000)

/* Size: 4K, End at: 0xE4501FFF */
#define SOC_IOMCU_NPU_TSCPU_HTS_SLV_MSG_BASE_ADDR               (0xE4501000)

/* Size: 4K, End at: 0xE4500FFF */
#define SOC_IOMCU_NPU_HWTS_HTS_SLV_MSG_BASE_ADDR                (0xE4500000)

/* Size: 1M, End at: 0xE44FFFFF */
#define SOC_IOMCU_QCP_TSQIC_BASE_ADDR                           (0xE4400000)

/* Size: 4K, End at: 0xE43A1FFF */
#define SOC_IOMCU_ts_secure_BASE_ADDR                           (0xE43A1000)

/* Size: 4K, End at: 0xE43A0FFF */
#define SOC_IOMCU_tscpu_sysctrl_BASE_ADDR                       (0xE43A0000)

/* Size: 128K, End at: 0xE439FFFF */
#define SOC_IOMCU_ts_tbu_BASE_ADDR                              (0xE4380000)

/* Size: 4K, End at: 0xE430BFFF */
#define SOC_IOMCU_hw_exp_irq_ns3_BASE_ADDR                      (0xE430B000)

/* Size: 4K, End at: 0xE430AFFF */
#define SOC_IOMCU_hw_exp_irq_ns2_BASE_ADDR                      (0xE430A000)

/* Size: 4K, End at: 0xE4309FFF */
#define SOC_IOMCU_hw_exp_irq_ns1_BASE_ADDR                      (0xE4309000)

/* Size: 4K, End at: 0xE4308FFF */
#define SOC_IOMCU_hw_exp_irq_ns_BASE_ADDR                       (0xE4308000)

/* Size: 4K, End at: 0xE4307FFF */
#define SOC_IOMCU_hw_exp_irq_s_BASE_ADDR                        (0xE4307000)

/* Size: 4K, End at: 0xE4305FFF */
#define SOC_IOMCU_tscpu_timer_BASE_ADDR                         (0xE4305000)

/* Size: 4K, End at: 0xE4304FFF */
#define SOC_IOMCU_tscpu_watchdog_BASE_ADDR                      (0xE4304000)

/* Size: 4K, End at: 0xE4301FFF */
#define SOC_IOMCU_npu_uart_BASE_ADDR                            (0xE4301000)

/* Size: 4K, End at: 0xE4300FFF */
#define SOC_IOMCU_ts_dmmu_BASE_ADDR                             (0xE4300000)

/* Size: 4K, End at: 0xE4280FFF */
#define SOC_IOMCU_doorbell_sts_BASE_ADDR                        (0xE4280000)

/* Size: 512K, End at: 0xE427FFFF */
#define SOC_IOMCU_doorbell_cfg_BASE_ADDR                        (0xE4200000)

/* Size: 2M, End at: 0xE41FFFFF */
#define SOC_IOMCU_hwts_BASE_ADDR                                (0xE4000000)


/* ===========================  GPU SUBSYS 4MB  =========================== */
/* Size: 256K, End at: 0xE2F3FFFF */
#define SOC_IOMCU_GPU_LPCTRL_AHB_BASE_ADDR                      (0xE2F00000)

/* Size: 256K, End at: 0xE2EFFFFF */
#define SOC_IOMCU_G3D_BASE_ADDR                                 (0xE2EC0000)

/* Size: 4K, End at: 0xE2E82FFF */
#define SOC_IOMCU_GPU_HTS_SLV_MSG_BASE_ADDR                     (0xE2E82000)

/* Size: 8K, End at: 0xE2E81FFF */
#define SOC_IOMCU_FFAP1_GPU_BASE_ADDR                           (0xE2E80000)

/* Size: 4K, End at: 0xE2E22FFF */
#define SOC_IOMCU_SC_SCHEDULER_SLV_BASE_ADDR                    (0xE2E22000)

/* Size: 4K, End at: 0xE2E21FFF */
#define SOC_IOMCU_GPU_CTRL_BASE_ADDR                            (0xE2E21000)

/* Size: 4K, End at: 0xE2E20FFF */
#define SOC_IOMCU_GPU_CRG_BASE_ADDR                             (0xE2E20000)

/* Size: 128K, End at: 0xE2E1FFFF */
#define SOC_IOMCU_GPU_LPCTRL_APB_BASE_ADDR                      (0xE2E00000)

/* Size: 1M, End at: 0xE2DFFFFF */
#define SOC_IOMCU_QCP_GPUDATA_BASE_ADDR                         (0xE2D00000)

/* Size: 1M, End at: 0xE2CFFFFF */
#define SOC_IOMCU_QCP_GPUCFG_BASE_ADDR                          (0xE2C00000)


/* ===============================  <N/A>  ================================ */
/* Size: 320K, End at: 0xE2BFFFFF */
#define SOC_IOMCU_onchiprom_cpu_BASE_ADDR                       (0xE2BB0000)

/* Size: 8K, End at: 0xE2B13FFF */
#define SOC_IOMCU_FW_CROSSBAR_1_BASE_ADDR                       (0xE2B12000)

/* Size: 8K, End at: 0xE2B11FFF */
#define SOC_IOMCU_FW_CROSSBAR_0_BASE_ADDR                       (0xE2B10000)

/* Size: 4K, End at: 0xE2B0FFFF */
#define SOC_IOMCU_DJTAG_ROUTER_BASE_ADDR                        (0xE2B0F000)

/* Size: 2K, End at: 0xE2B0EFFF */
#define SOC_IOMCU_HTS_WRAP_CFG_BASE_ADDR                        (0xE2B0E800)

/* Size: 2K, End at: 0xE2B0E7FF */
#define SOC_IOMCU_HTS_MSG_BASE_ADDR                             (0xE2B0E000)

/* Size: 24K, End at: 0xE2B0DFFF */
#define SOC_IOMCU_HTS_CFG_BASE_ADDR                             (0xE2B08000)

/* Size: 32K, End at: 0xE2B07FFF */
#define SOC_IOMCU_FFTS_BASE_ADDR                                (0xE2B00000)


/* ========================  MDM PERI SUBSYS 1MB  ========================= */
/* Size: 512K, End at: 0xE2AFFFFF */
#define SOC_IOMCU_QCP_MDMPERI_BASE_ADDR                         (0xE2A80000)

/* Size: 16K, End at: 0xE2A43FFF */
#define SOC_IOMCU_SPE_BASE_ADDR                                 (0xE2A40000)

/* Size: 32K, End at: 0xE2A3FFFF */
#define SOC_IOMCU_EICC_BASE_ADDR                                (0xE2A38000)

/* Size: 32K, End at: 0xE2A37FFF */
#define SOC_IOMCU_IPF_BASE_ADDR                                 (0xE2A30000)

/* Size: 64K, End at: 0xE2A2FFFF */
#define SOC_IOMCU_MAA_BASE_ADDR                                 (0xE2A20000)

/* Size: 128K, End at: 0xE2A1FFFF */
#define SOC_IOMCU_SOCP_BASE_ADDR                                (0xE2A00000)


/* ===============================  <N/A>  ================================ */
/* Size: 1M, End at: 0xE29FFFFF */
#define SOC_IOMCU_QCP_CROSSBAR_BASE_ADDR                        (0xE2900000)

/* Size: 1M, End at: 0xE28FFFFF */
#define SOC_IOMCU_SC_CFG_BASE_ADDR                              (0xE2800000)


/* =========================  MEDIA1 SUBSYS 12MB  ========================= */
/* Size: 256K, End at: 0xE24FFFFF */
#define SOC_IOMCU_MEDIA1_QCP_BASE_ADDR                          (0xE24C0000)

/* Size: 64K, End at: 0xE24BFFFF */
#define SOC_IOMCU_MEDIA1_TCU_QCP_BASE_ADDR                      (0xE24B0000)

/* Size: 32K, End at: 0xE24AFFFF */
#define SOC_IOMCU_VENC_QCP_BASE_ADDR                            (0xE24A8000)

/* Size: 32K, End at: 0xE24A7FFF */
#define SOC_IOMCU_VDEC_QCP_BASE_ADDR                            (0xE24A0000)

/* Size: 64K, End at: 0xE249FFFF */
#define SOC_IOMCU_DSS_QCP_BASE_ADDR                             (0xE2490000)

/* Size: 8K, End at: 0xE2483FFF */
#define SOC_IOMCU_FW_MEDIA1_BASE_ADDR                           (0xE2482000)

/* Size: 4K, End at: 0xE2481FFF */
#define SOC_IOMCU_DSS_HTS_SLV_MSG_BASE_ADDR                     (0xE2481000)

/* Size: 4K, End at: 0xE2480FFF */
#define SOC_IOMCU_VENC_HTS_SLV_MSG_BASE_ADDR                    (0xE2480000)

/* Size: 384K, End at: 0xE2462FFF */
#define SOC_IOMCU_MEDIA1_TCU_BASE_ADDR                          (0xE2403000)

/* Size: 4K, End at: 0xE2401FFF */
#define SOC_IOMCU_MEDIA1_CRG_BASE_ADDR                          (0xE2401000)

/* Size: 156K, End at: 0xE22FFFFF */
#define SOC_IOMCU_DSS_DISP_CH1_BASE_ADDR                        (0xE22D9000)

/* Size: 4K, End at: 0xE22D8FFF */
#define SOC_IOMCU_DSS_GLB_DISP_BASE_ADDR                        (0xE22D8000)

/* Size: 1K, End at: 0xE22D7FFF */
#define SOC_IOMCU_DSS_RD_CTRL_BASE_ADDR                         (0xE22D7C00)

/* Size: 4K, End at: 0xE22D7BFF */
#define SOC_IOMCU_DSS_PIPE3_BASE_ADDR                           (0xE22D6C00)

/* Size: 280K, End at: 0xE22D5FFF */
#define SOC_IOMCU_DSS_DISP_CH0_BASE_ADDR                        (0xE2290000)

/* Size: 4K, End at: 0xE228FFFF */
#define SOC_IOMCU_DSS_UVDP_WRAP_REG_BASE_ADDR                   (0xE228F000)

/* Size: 4K, End at: 0xE228DFFF */
#define SOC_IOMCU_DSS_CLD0_WRAP_REG_BASE_ADDR                   (0xE228D000)

/* Size: 4K, End at: 0xE2288FFF */
#define SOC_IOMCU_DSS_GLB_AVHR_BASE_ADDR                        (0xE2288000)

/* Size: 12K, End at: 0xE2286FFF */
#define SOC_IOMCU_DSS_VSCF1_VSCF_LUT_BASE_ADDR                  (0xE2284000)

/* Size: 4K, End at: 0xE2283FFF */
#define SOC_IOMCU_DSS_VSCF1_WRAP_REG_BASE_ADDR                  (0xE2283000)

/* Size: 12K, End at: 0xE2282FFF */
#define SOC_IOMCU_DSS_VSCF0_VSCF_LUT_BASE_ADDR                  (0xE2280000)

/* Size: 4K, End at: 0xE227FFFF */
#define SOC_IOMCU_DSS_VSCF0_WRAP_REG_BASE_ADDR                  (0xE227F000)

/* Size: 4K, End at: 0xE227DFFF */
#define SOC_IOMCU_DSS_ARSR1_ARSR_LUT_BASE_ADDR                  (0xE227D000)

/* Size: 16K, End at: 0xE227CFFF */
#define SOC_IOMCU_DSS_ARSR1_VSCF_LUT_BASE_ADDR                  (0xE2279000)

/* Size: 4K, End at: 0xE2278FFF */
#define SOC_IOMCU_DSS_ARSR1_WRAP_REG_BASE_ADDR                  (0xE2278000)

/* Size: 4K, End at: 0xE2271FFF */
#define SOC_IOMCU_DSS_WCH2_BASE_ADDR                            (0xE2271000)

/* Size: 12K, End at: 0xE2270FFF */
#define SOC_IOMCU_DSS_VSCF_LUT_BASE_ADDR                        (0xE226E000)

/* Size: 4K, End at: 0xE226DFFF */
#define SOC_IOMCU_DSS_WCH1_BASE_ADDR                            (0xE226D000)

/* Size: 4K, End at: 0xE226CFFF */
#define SOC_IOMCU_DSS_WCH0_BASE_ADDR                            (0xE226C000)

/* Size: 8K, End at: 0xE226AFFF */
#define SOC_IOMCU_DSS_HDR_GMP_LUT_BASE_ADDR                     (0xE2269000)

/* Size: 4K, End at: 0xE2268FFF */
#define SOC_IOMCU_DSS_HDR_GAMMA_LUT_BASE_ADDR                   (0xE2268000)

/* Size: 4K, End at: 0xE2267FFF */
#define SOC_IOMCU_DSS_HDR_DEGAMMA_LUT_BASE_ADDR                 (0xE2267000)

/* Size: 24K, End at: 0xE2266FFF */
#define SOC_IOMCU_DSS_HDR_CORE_LUT_BASE_ADDR                    (0xE2261000)

/* Size: 4K, End at: 0xE2260FFF */
#define SOC_IOMCU_DSS_HDR_WRAP_REG_BASE_ADDR                    (0xE2260000)

/* Size: 8K, End at: 0xE224DFFF */
#define SOC_IOMCU_DSS_DACC_LB_CTRL_BASE_ADDR                    (0xE224C000)

/* Size: 4K, End at: 0xE2249FFF */
#define SOC_IOMCU_DSS_HTS_SLAVE_ADAPTER_BASE_ADDR               (0xE2249000)

/* Size: 4K, End at: 0xE2248FFF */
#define SOC_IOMCU_DSS_HTS_MASTER_ADAPTER_BASE_ADDR              (0xE2248000)

/* Size: 32K, End at: 0xE2247FFF */
#define SOC_IOMCU_DSS_DACC_CFG_BASE_ADDR                        (0xE2240000)

/* Size: 4K, End at: 0xE223FFFF */
#define SOC_IOMCU_DSS_DACC_WDT_BASE_ADDR                        (0xE223F000)

/* Size: 4K, End at: 0xE223EFFF */
#define SOC_IOMCU_DSS_DACC_TIMER_BASE_ADDR                      (0xE223E000)

/* Size: 28K, End at: 0xE2234FFF */
#define SOC_IOMCU_DSS_DACC_DM_BASE_ADDR                         (0xE222E000)

/* Size: 64K, End at: 0xE222DFFF */
#define SOC_IOMCU_DSS_DACC_IM_BASE_ADDR                         (0xE221E000)

/* Size: 8K, End at: 0xE221DFFF */
#define SOC_IOMCU_DSS_RCH_OV_BASE_ADDR                          (0xE221C000)

/* Size: 4K, End at: 0xE221BFFF */
#define SOC_IOMCU_DSS_PIPE2_BASE_ADDR                           (0xE221B000)

/* Size: 4K, End at: 0xE221AFFF */
#define SOC_IOMCU_DSS_PIPE1_BASE_ADDR                           (0xE221A000)

/* Size: 4K, End at: 0xE2219FFF */
#define SOC_IOMCU_DBCU_BASE_ADDR                                (0xE2219000)

/* Size: 1K, End at: 0xE2218FFF */
#define SOC_IOMCU_DSS_RD_CTRL_OV_BASE_ADDR                      (0xE2218C00)

/* Size: 1K, End at: 0xE2218BFF */
#define SOC_IOMCU_DSS_RD_CTRL_PRE_WRAP_BASE_ADDR                (0xE2218800)

/* Size: 1K, End at: 0xE22177FF */
#define SOC_IOMCU_DSS_LBUF_CMN_BASE_ADDR                        (0xE2217400)

/* Size: 1K, End at: 0xE22173FF */
#define SOC_IOMCU_DSS_RD_CTRL_ITF_BASE_ADDR                     (0xE2217000)

/* Size: 4K, End at: 0xE2216FFF */
#define SOC_IOMCU_DSS_PIPE4_BASE_ADDR                           (0xE2216000)

/* Size: 4K, End at: 0xE2215FFF */
#define SOC_IOMCU_DSS_PIPE0_BASE_ADDR                           (0xE2215000)

/* Size: 8K, End at: 0xE2214FFF */
#define SOC_IOMCU_DSS_GLB_BASE_ADDR                             (0xE2213000)

/* Size: 4K, End at: 0xE2212FFF */
#define SOC_IOMCU_DSS_CMD_BASE_ADDR                             (0xE2212000)

/* Size: 4K, End at: 0xE2211FFF */
#define SOC_IOMCU_DSS_ITF_BASE_ADDR                             (0xE2211000)

/* Size: 16K, End at: 0xE2209FFF */
#define SOC_IOMCU_DSS_SRT_TBU_BASE_ADDR                         (0xE2206000)

/* Size: 16K, End at: 0xE2205FFF */
#define SOC_IOMCU_DSS_RT_TBU_BASE_ADDR                          (0xE2202000)

/* Size: 4K, End at: 0xE2200FFF */
#define SOC_IOMCU_DSI_BASE_ADDR                                 (0xE2200000)

/* Size: 64K, End at: 0xE213FFFF */
#define SOC_IOMCU_MCU_ITCM_BASE_ADDR                            (0xE2130000)

/* Size: 4K, End at: 0xE2129FFF */
#define SOC_IOMCU_VENC_IPC_BASE_ADDR                            (0xE2129000)

/* Size: 4K, End at: 0xE2127FFF */
#define SOC_IOMCU_VENC_UART_BASE_ADDR                           (0xE2127000)

/* Size: 4K, End at: 0xE2126FFF */
#define SOC_IOMCU_VENC_TIMER_BASE_ADDR                          (0xE2126000)

/* Size: 4K, End at: 0xE2125FFF */
#define SOC_IOMCU_VENC_WDG_BASE_ADDR                            (0xE2125000)

/* Size: 4K, End at: 0xE2124FFF */
#define SOC_IOMCU_SUB_CTRL_VENC_BASE_ADDR                       (0xE2124000)

/* Size: 16K, End at: 0xE2123FFF */
#define SOC_IOMCU_venc_smmu_tbu_BASE_ADDR                       (0xE2120000)

/* Size: 4K, End at: 0xE211AFFF */
#define SOC_IOMCU_SMMU_PRE_CDC_BASE_ADDR                        (0xE211A000)

/* Size: 4K, End at: 0xE2119FFF */
#define SOC_IOMCU_axidfx_BASE_ADDR                              (0xE2119000)

/* Size: 4K, End at: 0xE2118FFF */
#define SOC_IOMCU_venc_smmu_pre_BASE_ADDR                       (0xE2118000)

/* Size: 2K, End at: 0xE210B7FF */
#define SOC_IOMCU_core_lite2_BASE_ADDR                          (0xE210B000)

/* Size: 2K, End at: 0xE210AFFF */
#define SOC_IOMCU_core_lite1_BASE_ADDR                          (0xE210A800)

/* Size: 2K, End at: 0xE210A7FF */
#define SOC_IOMCU_core_lite0_BASE_ADDR                          (0xE210A000)

/* Size: 4K, End at: 0xE2109FFF */
#define SOC_IOMCU_VENC_HTS_SLV_ADAPTER_BASE_ADDR                (0xE2109000)

/* Size: 4K, End at: 0xE2108FFF */
#define SOC_IOMCU_MCU_CTRL_BASE_ADDR                            (0xE2108000)

/* Size: 4K, End at: 0xE2107FFF */
#define SOC_IOMCU_cdmlst_BASE_ADDR                              (0xE2107000)

/* Size: 12K, End at: 0xE2106FFF */
#define SOC_IOMCU_debug_ro_reg_BASE_ADDR                        (0xE2104000)

/* Size: 4K, End at: 0xE2103FFF */
#define SOC_IOMCU_func_read_back_reg_BASE_ADDR                  (0xE2103000)

/* Size: 4K, End at: 0xE2100FFF */
#define SOC_IOMCU_VENC_core_cfg_BASE_ADDR                       (0xE2100000)

/* Size: 128K, End at: 0xE20FFFFF */
#define SOC_IOMCU_PXP_HW_QIC_VDEC_BASE_ADDR                     (0xE20E0000)

/* Size: 128K, End at: 0xE20DFFFF */
#define SOC_IOMCU_CFG_QIC_VDEC_BASE_ADDR                        (0xE20C0000)

/* Size: 128K, End at: 0xE20BFFFF */
#define SOC_IOMCU_CORE_QIC_VDEC_BASE_ADDR                       (0xE20A0000)

/* Size: 4K, End at: 0xE209FFFF */
#define SOC_IOMCU_VDEC_HTS_MST_ADAPTER_BASE_ADDR                (0xE209F000)

/* Size: 8K, End at: 0xE209EFFF */
#define SOC_IOMCU_FW_CORE_CFG_BASE_ADDR                         (0xE209D000)

/* Size: 8K, End at: 0xE209CFFF */
#define SOC_IOMCU_FW_VDECCORE_CFG_BASE_ADDR                     (0xE209B000)

/* Size: 8K, End at: 0xE2033FFF */
#define SOC_IOMCU_JPDG_VDEC_BASE_ADDR                           (0xE2032000)

/* Size: 64K, End at: 0xE2031FFF */
#define SOC_IOMCU_RISCV_ITCM_TBD_VDEC_BASE_ADDR                 (0xE2022000)

/* Size: 60K, End at: 0xE201DFFF */
#define SOC_IOMCU_RISCV_TBD_VDEC_BASE_ADDR                      (0xE200F000)

/* Size: 8K, End at: 0xE200DFFF */
#define SOC_IOMCU_IPC_VDEC_BASE_ADDR                            (0xE200C000)

/* Size: 4K, End at: 0xE200BFFF */
#define SOC_IOMCU_UART_VDEC_BASE_ADDR                           (0xE200B000)

/* Size: 4K, End at: 0xE200AFFF */
#define SOC_IOMCU_Timer_VDEC_BASE_ADDR                          (0xE200A000)

/* Size: 4K, End at: 0xE2009FFF */
#define SOC_IOMCU_WatchDog_VDEC_BASE_ADDR                       (0xE2009000)

/* Size: 4K, End at: 0xE2008FFF */
#define SOC_IOMCU_SUB_CTRL_VDEC_BASE_ADDR                       (0xE2008000)

/* Size: 4K, End at: 0xE2006FFF */
#define SOC_IOMCU_SMMU_PRE_VDEC_BASE_ADDR                       (0xE2006000)

/* Size: 16K, End at: 0xE2004FFF */
#define SOC_IOMCU_SMMU_TBU_VDEC_BASE_ADDR                       (0xE2001000)

/* Size: 1K, End at: 0xE20003FF */
#define SOC_IOMCU_SCD_VDEC_BASE_ADDR                            (0xE2000000)

/* Size: 2M, End at: 0xE1FFFFFF */
#define SOC_IOMCU_PXPC_VDEC_BASE_ADDR                           (0xE1E00000)

/* Size: 2M, End at: 0xE1DFFFFF */
#define SOC_IOMCU_BSP_VDEC_BASE_ADDR                            (0xE1C00000)


/* =========================  MEDIA2 SUBSYS 24MB  ========================= */
/* Size: 4K, End at: 0xE1363FFF */
#define SOC_IOMCU_IPP_HTS_SLV_MSG_BASE_ADDR                     (0xE1363000)

/* Size: 4K, End at: 0xE1362FFF */
#define SOC_IOMCU_ISP_HTS_SLV_MSG_BASE_ADDR                     (0xE1362000)

/* Size: 8K, End at: 0xE1361FFF */
#define SOC_IOMCU_FW_MEDIA2_BASE_ADDR                           (0xE1360000)

/* Size: 128k, End at: 0xE135FFFF */
#define SOC_IOMCU_QCP_ISP_BASE_ADDR                             (0xE1340000)

/* Size: 256K, End at: 0xE133FFFF */
#define SOC_IOMCU_QCP_M2_BASE_ADDR                              (0xE1300000)

/* Size: 8K, End at: 0xE12F3FFF */
#define SOC_IOMCU_IDI2AXI_BASE_ADDR                             (0xE12F2000)

/* Size: 4K, End at: 0xE12F1FFF */
#define SOC_IOMCU_MEDIA2_P2J_BASE_ADDR                          (0xE12F1000)

/* Size: 4K, End at: 0xE12F0FFF */
#define SOC_IOMCU_MEDIA2_CRG_BASE_ADDR                          (0xE12F0000)

/* Size: 64K, End at: 0xE12EFFFF */
#define SOC_IOMCU_mini_isp_BASE_ADDR                            (0xE12E0000)

/* Size: 384K, End at: 0xE12DFFFF */
#define SOC_IOMCU_MEDIA2_TCU_BASE_ADDR                          (0xE1280000)

/* Size: 2K, End at: 0xE120CFFF */
#define SOC_IOMCU_RX_CDPHY_F_BASE_ADDR                          (0xE120C800)

/* Size: 2K, End at: 0xE120C7FF */
#define SOC_IOMCU_RX_CDPHY_E_BASE_ADDR                          (0xE120C000)

/* Size: 2K, End at: 0xE120BFFF */
#define SOC_IOMCU_RX_CDPHY_D_BASE_ADDR                          (0xE120B800)

/* Size: 2K, End at: 0xE120B7FF */
#define SOC_IOMCU_RX_CDPHY_C_BASE_ADDR                          (0xE120B000)

/* Size: 2K, End at: 0xE120AFFF */
#define SOC_IOMCU_RX_CDPHY_B_BASE_ADDR                          (0xE120A800)

/* Size: 2K, End at: 0xE120A7FF */
#define SOC_IOMCU_RX_CDPHY_A_BASE_ADDR                          (0xE120A000)

/* Size: 4K, End at: 0xE1209FFF */
#define SOC_IOMCU_CSI_G_BASE_ADDR                               (0xE1209000)

/* Size: 4K, End at: 0xE1208FFF */
#define SOC_IOMCU_CSI_F_BASE_ADDR                               (0xE1208000)

/* Size: 4K, End at: 0xE1205FFF */
#define SOC_IOMCU_CSI_E_BASE_ADDR                               (0xE1205000)

/* Size: 4K, End at: 0xE1204FFF */
#define SOC_IOMCU_CSI_D_BASE_ADDR                               (0xE1204000)

/* Size: 4K, End at: 0xE1203FFF */
#define SOC_IOMCU_CSI_C_BASE_ADDR                               (0xE1203000)

/* Size: 4K, End at: 0xE1202FFF */
#define SOC_IOMCU_CSI_B_BASE_ADDR                               (0xE1202000)

/* Size: 4K, End at: 0xE1201FFF */
#define SOC_IOMCU_CSI_A_BASE_ADDR                               (0xE1201000)

/* Size: 4K, End at: 0xE1200FFF */
#define SOC_IOMCU_CSI_adapter_BASE_ADDR                         (0xE1200000)

/* Size: 4K, End at: 0xE1163FFF */
#define SOC_IOMCU_SMMU_SEC_ADAPTOR_BASE_ADDR                    (0xE1163000)

/* Size: 12K, End at: 0xE1162FFF */
#define SOC_IOMCU_CVDR_TBU_BASE_ADDR                            (0xE1160000)

/* Size: 32K, End at: 0xE111FFFF */
#define SOC_IOMCU_MC_BASE_ADDR                                  (0xE1118000)

/* Size: 16K, End at: 0xE1117FFF */
#define SOC_IOMCU_CMP_BASE_ADDR                                 (0xE1114000)

/* Size: 4K, End at: 0xE1110FFF */
#define SOC_IOMCU_ORB_ENH_BASE_ADDR                             (0xE1110000)

/* Size: 4K, End at: 0xE110EFFF */
#define SOC_IOMCU_RDR_BASE_ADDR                                 (0xE110E000)

/* Size: 8K, End at: 0xE110DFFF */
#define SOC_IOMCU_AR_FEATURE_BASE_ADDR                          (0xE110C000)

/* Size: 8K, End at: 0xE110BFFF */
#define SOC_IOMCU_KLT_BASE_ADDR                                 (0xE110A000)

/* Size: 8K, End at: 0xE1107FFF */
#define SOC_IOMCU_CVDR_BASE_ADDR                                (0xE1106000)

/* Size: 4K, End at: 0xE1105FFF */
#define SOC_IOMCU_CMDLIST_BASE_ADDR                             (0xE1105000)

/* Size: 4K, End at: 0xE1104FFF */
#define SOC_IOMCU_TOP_BASE_ADDR                                 (0xE1104000)

/* Size: 4K, End at: 0xE1100FFF */
#define SOC_IOMCU_JPGE_BASE_ADDR                                (0xE1100000)

/* Size: 4K, End at: 0xE0FFCFFF */
#define SOC_IOMCU_OISP_CTRL_BASE_ADDR                           (0xE0FFC000)

/* Size: 8K, End at: 0xE0FFBFFF */
#define SOC_IOMCU_SPI0_BASE_ADDR                                (0xE0FFA000)

/* Size: 4K, End at: 0xE0FF9FFF */
#define SOC_IOMCU_OISP_Timer_BASE_ADDR                          (0xE0FF9000)

/* Size: 4K, End at: 0xE0FF8FFF */
#define SOC_IOMCU_OISP_WDT_BASE_ADDR                            (0xE0FF8000)

/* Size: 32K, End at: 0xE0FF7FFF */
#define SOC_IOMCU_OIS_FILTER_BASE_ADDR                          (0xE0FF0000)

/* Size: 4K, End at: 0xE0FEFFFF */
#define SOC_IOMCU_ISP_HiDMA_BASE_ADDR                           (0xE0FEF000)

/* Size: 4K, End at: 0xE0FEEFFF */
#define SOC_IOMCU_I3C_HW6_BASE_ADDR                             (0xE0FEE000)

/* Size: 4K, End at: 0xE0FEDFFF */
#define SOC_IOMCU_I3C6_ISP_BASE_ADDR                            (0xE0FED000)

/* Size: 4K, End at: 0xE0FECFFF */
#define SOC_IOMCU_I2C6_ISP_BASE_ADDR                            (0xE0FEC000)

/* Size: 4K, End at: 0xE0FEBFFF */
#define SOC_IOMCU_I3C_HW5_BASE_ADDR                             (0xE0FEB000)

/* Size: 4K, End at: 0xE0FEAFFF */
#define SOC_IOMCU_I3C5_ISP_BASE_ADDR                            (0xE0FEA000)

/* Size: 4K, End at: 0xE0FE9FFF */
#define SOC_IOMCU_I2C5_ISP_BASE_ADDR                            (0xE0FE9000)

/* Size: 4K, End at: 0xE0FE8FFF */
#define SOC_IOMCU_I3C_HW4_BASE_ADDR                             (0xE0FE8000)

/* Size: 4K, End at: 0xE0FE7FFF */
#define SOC_IOMCU_I3C4_ISP_BASE_ADDR                            (0xE0FE7000)

/* Size: 4K, End at: 0xE0FE6FFF */
#define SOC_IOMCU_I2C4_ISP_BASE_ADDR                            (0xE0FE6000)

/* Size: 4K, End at: 0xE0FE5FFF */
#define SOC_IOMCU_I3C_HW3_BASE_ADDR                             (0xE0FE5000)

/* Size: 4K, End at: 0xE0FE4FFF */
#define SOC_IOMCU_I3C3_ISP_BASE_ADDR                            (0xE0FE4000)

/* Size: 4K, End at: 0xE0FE3FFF */
#define SOC_IOMCU_I2C3_ISP_BASE_ADDR                            (0xE0FE3000)

/* Size: 4K, End at: 0xE0FE2FFF */
#define SOC_IOMCU_I3C_HW2_BASE_ADDR                             (0xE0FE2000)

/* Size: 4K, End at: 0xE0FE1FFF */
#define SOC_IOMCU_I3C2_ISP_BASE_ADDR                            (0xE0FE1000)

/* Size: 4K, End at: 0xE0FE0FFF */
#define SOC_IOMCU_I2C2_ISP_BASE_ADDR                            (0xE0FE0000)

/* Size: 4K, End at: 0xE0FDFFFF */
#define SOC_IOMCU_I3C_HW1_BASE_ADDR                             (0xE0FDF000)

/* Size: 4K, End at: 0xE0FDEFFF */
#define SOC_IOMCU_I3C1_ISP_BASE_ADDR                            (0xE0FDE000)

/* Size: 4K, End at: 0xE0FDDFFF */
#define SOC_IOMCU_I2C1_ISP_BASE_ADDR                            (0xE0FDD000)

/* Size: 4K, End at: 0xE0FDCFFF */
#define SOC_IOMCU_I3C_HW0_BASE_ADDR                             (0xE0FDC000)

/* Size: 4K, End at: 0xE0FDBFFF */
#define SOC_IOMCU_I3C0_ISP_BASE_ADDR                            (0xE0FDB000)

/* Size: 4K, End at: 0xE0FDAFFF */
#define SOC_IOMCU_I2C0_ISP_BASE_ADDR                            (0xE0FDA000)

/* Size: 4K, End at: 0xE0FD9FFF */
#define SOC_IOMCU_ISP_IPC_BASE_ADDR                             (0xE0FD9000)

/* Size: 4K, End at: 0xE0FD7FFF */
#define SOC_IOMCU_SEC_ADPT_BASE_ADDR                            (0xE0FD7000)

/* Size: 12K, End at: 0xE0FD6FFF */
#define SOC_IOMCU_TBU_BASE_ADDR                                 (0xE0FD4000)

/* Size: 4K, End at: 0xE0FD3FFF */
#define SOC_IOMCU_ISP_SUB_CTRL_BASE_ADDR                        (0xE0FD3000)

/* Size: 4K, End at: 0xE0FD1FFF */
#define SOC_IOMCU_ISP_TIMER_BASE_ADDR                           (0xE0FD1000)

/* Size: 4K, End at: 0xE0FD0FFF */
#define SOC_IOMCU_ISP_WDT_BASE_ADDR                             (0xE0FD0000)

/* Size: 64K, End at: 0xE0F8FFFF */
#define SOC_IOMCU_ISP_CORE_TBU_BASE_ADDR                        (0xE0F80000)

/* Size: 16K, End at: 0xE0F63FFF */
#define SOC_IOMCU_OISP_DTCM_BASE_ADDR                           (0xE0F60000)

/* Size: 64K, End at: 0xE0F5FFFF */
#define SOC_IOMCU_OISP_QCP_BASE_ADDR                            (0xE0F50000)

/* Size: 8K, End at: 0xE0F41FFF */
#define SOC_IOMCU_ISP_FW_BASE_ADDR                              (0xE0F40000)

/* Size: 256K, End at: 0xE0F3FFFF */
#define SOC_IOMCU_ISP_QCP_BASE_ADDR                             (0xE0F00000)

/* Size: 3M, End at: 0xE0EFFFFF */
#define SOC_IOMCU_ISP_CORE_CFG_BASE_ADDR                        (0xE0C00000)

/* Size: 2M, End at: 0xE0BFFFFF */
#define SOC_IOMCU_ISP_CPU_local_peri_BASE_ADDR                  (0xE0A00000)

/* Size: 8M, End at: 0xE09FFFFF */
#define SOC_IOMCU_ISP_CPU_local_apb_BASE_ADDR                   (0xE0200000)

/* Size: 2M, End at: 0xE01FFFFF */
#define SOC_IOMCU_ISP_CPU_GIC_BASE_ADDR                         (0xE0000000)


/* ===============================  <N/A>  ================================ */
/* Size: 256M, End at: 0x8FFFFFFF */
#define SOC_IOMCU_PMEM_BASE_ADDR                                (0x80000000)

/* Size: 512M, End at: 0x7FFFFFFF */
#define SOC_IOMCU_SYSTEM_BASE_ADDR                              (0x60000000)


/* ===  通过窗口滑动实现对3.5G DDR空间的访问,DRAM1和DRAM2分别提供滑窗  ==== */
/* Size: 256M, End at: 0x5FFFFFFF */
#define SOC_IOMCU_DRAM1_BASE_ADDR                               (0x50000000)

/* Size: 256M, End at: 0x4FFFFFFF */
#define SOC_IOMCU_DRAM0_BASE_ADDR                               (0x40000000)


/* ===============================  <N/A>  ================================ */
/* Size: 1280K, End at: 0x2013FFFF */
#define SOC_IOMCU_DTCM_BASE_ADDR                                (0x20000000)

/* Size: 1280K, End at: 0x0013FFFF */
#define SOC_IOMCU_ITCM_BASE_ADDR                                (0x00000000)



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

#endif /* end of soc_iomcu_baseaddr_interface.h */
