/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: m3_sram_map.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/


#ifndef __M3_SRAM_MAP_H__
#define __M3_SRAM_MAP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef CONFIG_PM_DEL_PLATFORM_ADDR
#include <soc_lpmcu_baseaddr_interface.h>
#include <soc_acpu_baseaddr_interface.h>
#include <m3_ddr_map.h>
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define __PRINT_MACRO(x) #x
#define PRINT_MACRO(x) #x"="__PRINT_MACRO(x)


/************************LPRAM MEMORY MAP************************************/
/*----------------------------------------------------------------------------
|	|		|   | 0x20000 ~ 0x20080    128B isr_vector & magicnumber & seckey
|	|		|   | 0x20080 ~ 0x20320    896B init & runtime
|	|		|   | 0x20320 ~ 0x20F20    3K lpmcu general mntn     0x20320 ~ 0x20F20 NO overlap in fastboot
|AO	|		|   | 0x20F20 ~ 0x21680    1.84375K pmu/peri/ao/thermal init para
|	|		|   | 0x21680 ~ 0x21780    0.25K modem para
|160K			|   | 0x21780 ~ 0x21D00    1408byte telemntn
|	|		|   | 0x21D00 ~ 0x22900    3K IPC trace
|	|		|   | 0x22900 ~ 0x2BC00    36.75K ddr profile
|	|		|   | 0x2BC00 ~ 0x3A100    57.25K alwayson
| 	| 		|   | 0x3A100 ~ 0x44900    34K modem alwayson
------------------------------------------------------------------------------
| 	| 		|   | 0x44900 ~ 0x54100     70K modem peri
|	|		|   | 0x54100 ~ 0x70000     111.75K lpm3 code
|	|		|   | 0x70000 ~ 0x74D00     19.25K bss
|	|		|   | 0x74D00 ~ 0x75900     3K data
|	|		|   | 0x75900 ~ 0x75C00     0.75K lit cluster profile
|NAO			|   | 0x75C00 ~ 0x75F00     0.75K mid cluster profile
|192K			|   | 0x75F00 ~ 0x76200     0.75K big cluster profile
|	|		|   | 0x76200 ~ 0x76500     0.75K l3 cluster profile
|	|		|   | 0x76500 ~ 0x76800     0.75K gpu profile
|	|		|   | 0x76800 ~ 0x76C00     1K stack
|	|		|   | 0x76C00 ~ 0x78000     5K ddr mntn

------------------------------------------------------------------------------*/

#ifndef SOC_LPMCU_LP_RAM_BASE_ADDR
#define SOC_LPMCU_LP_RAM_BASE_ADDR	0x20000
#endif
#ifndef SOC_ACPU_LP_RAM_BASE_ADDR
#define SOC_ACPU_LP_RAM_BASE_ADDR	0xFFF50000
#endif


#if defined(__FASTBOOT__) || defined(CONFIG_BL2_LOAD_LPMCU)
#define M3_SRAM_BASE                            (LPMCU_FIRMWARE_BACKUP_ADDR)
#else
#define M3_SRAM_BASE                            (SOC_LPMCU_LP_RAM_BASE_ADDR)
#endif

#define M3_SRAM_SIZE                            (LPMCU_RAM_SIZE)
#define M3_SRAM_AO_SIZE                         (0x28000) /* 160K */


#define LPMCU_VECTOR_TABLE_BASE                 (M3_SRAM_BASE) /* 0x20000 */
#define LPMCU_VECTOR_TABLE_SIZE                 (0x80)         /* 128Bytes */


#define LPMCU_GEN_DATA_SEC_BASE                 (LPMCU_VECTOR_TABLE_BASE + LPMCU_VECTOR_TABLE_SIZE) /* 0x20080 */
#define LPMCU_GEN_DATA_SEC_SIZE                 (0x2C0)                                             /* 704Bytes */
    #define RUNTIME_VAR_BASE                        (LPMCU_GEN_DATA_SEC_BASE) /* 0x20080 */
    #define RUNTIME_VAR_SIZE                        (0x2C0)                           /* 0.65625K */
#if ((RUNTIME_VAR_BASE + RUNTIME_VAR_SIZE) > (LPMCU_GEN_DATA_SEC_BASE + LPMCU_GEN_DATA_SEC_SIZE))
    #error "lpmcu general data section overflow!!!"
#endif


#define LPMCU_GEN_MNTN_BASE                  (LPMCU_GEN_DATA_SEC_BASE + LPMCU_GEN_DATA_SEC_SIZE) /* 0x20340 */
#define LPMCU_GEN_MNTN_SIZE                  (0xBE0)                                             /* 3k */
	#define EXC_SPECIAL_SAVE_BASE                (LPMCU_GEN_MNTN_BASE) /* 0x20340 */
	#define EXC_SPECIAL_SAVE_SIZE                (0x40)

	/*sys track area*/
	#define INT_TRACK_BASE                       (EXC_SPECIAL_SAVE_BASE + EXC_SPECIAL_SAVE_SIZE) /* 0x20380 */
	#define INT_TRACK_SIZE                       (0x2C0)

	#define TASK_TRACK_BASE                      (INT_TRACK_BASE + INT_TRACK_SIZE) /* 0x20640 */
	#define TASK_TRACK_SIZE                      (0x100)

	/*exc area (ddr unavailable)*/
	#define REG_BACKUP_SRAM_BASE                 (TASK_TRACK_BASE + TASK_TRACK_SIZE) /* 0x20740 */
	#define REG_BACKUP_SRAM_SIZE                 (0x70)

	#define STACK_BACKUP_SRAM_BASE               (REG_BACKUP_SRAM_BASE + REG_BACKUP_SRAM_SIZE) /* 0x207B0 */
	#define STACK_BACKUP_SRAM_SIZE               (0x300)

	#define DMA_REG_BACKUP_BASE                  (STACK_BACKUP_SRAM_BASE + STACK_BACKUP_SRAM_SIZE) /* 0x20AB0 */
	#define DMA_REG_BACKUP_SIZE                  (0x70)

	#define REG_PMU_BASE                         (DMA_REG_BACKUP_BASE + DMA_REG_BACKUP_SIZE) /* 0x20B20 */
	#define REG_PMU_SIZE                         (0x40)  /* PMU 扩大32字节 */

	#define REG_BACKUP_BASE                      (REG_PMU_BASE + REG_PMU_SIZE) /* 0x20B60 */
	#define REG_BACKUP_SIZE                      (0x1A0)

	#define NOC_DUMP_BASE                      (REG_BACKUP_BASE + REG_BACKUP_SIZE) /* 0x20D00 */
	#define NOC_DUMP_SIZE                      (0x14)

	#define INT_LONG_TIME_TRACK_BASE            (NOC_DUMP_BASE + NOC_DUMP_SIZE) /* 0x20D14 */
	#define INT_LONG_TIME_TRACK_SIZE            (0x60)

	#define IRQ_MASK_LONG_TIME_TRACK_BASE       (INT_LONG_TIME_TRACK_BASE + INT_LONG_TIME_TRACK_SIZE) /* 0x20D74 */
	#define IRQ_MASK_LONG_TIME_TRACK_SIZE       (0x60)
#if ((IRQ_MASK_LONG_TIME_TRACK_BASE + IRQ_MASK_LONG_TIME_TRACK_SIZE) > (LPMCU_GEN_MNTN_BASE + LPMCU_GEN_MNTN_SIZE))
	    #error "lpmcu general mntn section overflow!!!"
#endif

#define INIT_PARA_BASE                          (LPMCU_GEN_MNTN_BASE + LPMCU_GEN_MNTN_SIZE) /* 0x20F20 */
#define INIT_PARA_SIZE                          (0x1B0)

#define PERI_AVS_PARA_BASE                        (INIT_PARA_BASE + INIT_PARA_SIZE)  /* 0x210D0 */
#define PERI_AVS_PARA_SIZE                        (0x360)
#define AO_AVS_PARA_BASE                        (PERI_AVS_PARA_BASE + PERI_AVS_PARA_SIZE)  /* 0x21430 */
#define AO_AVS_PARA_SIZE                        (0x120)

#define LPMCU_THERMAL_PARA_BASE                   (AO_AVS_PARA_BASE + AO_AVS_PARA_SIZE) /* 0x21550 */
#define LPMCU_THERMAL_PARA_SIZE                   (0x130)

#define LPMCU_MODEM_SEC_BASE                    (LPMCU_THERMAL_PARA_BASE + LPMCU_THERMAL_PARA_SIZE) /* 0x21680 */
#define LPMCU_MODEM_SEC_SIZE                    (0x100)                                             /* 0.25K */
    #define LPMCU_MDM_MNTN_BASE                 (LPMCU_MODEM_SEC_BASE)
    #define LPMCU_MDM_MNTN_SIZE                 (0x10)
#if ((LPMCU_MDM_MNTN_BASE + LPMCU_MDM_MNTN_SIZE) > (LPMCU_MODEM_SEC_BASE + LPMCU_MODEM_SEC_SIZE))
    #error "lpmcu modem section overflow!!!"
#endif


#define LPMCU_TELE_MNTN_SEC_BASE                (LPMCU_MODEM_SEC_BASE + LPMCU_MODEM_SEC_SIZE) /* 0x21780 */
#define LPMCU_TELE_MNTN_SEC_SIZE                (0x580)
    #define LPMCU_TELE_MNTN_DATA_BASE               (LPMCU_TELE_MNTN_SEC_BASE) /* lpmcu state old */
    #define LPMCU_TELE_MNTN_DATA_SIZE               (0x580)                                      /* 1K */
		#define LPMCU_TELE_MNTN_DATA_TICKMARK_BASE	(LPMCU_TELE_MNTN_DATA_BASE)
		#define LPMCU_TELE_MNTN_DATA_TICKMARK_SIZE	(0x2D0)
#if ((LPMCU_TELE_MNTN_DATA_BASE + LPMCU_TELE_MNTN_DATA_SIZE) > (LPMCU_TELE_MNTN_SEC_BASE + LPMCU_TELE_MNTN_SEC_SIZE))
    #error "lpmcu tele mntn section overflow!!!"
#endif

#define IPC_TRACK_BASE                          (LPMCU_TELE_MNTN_SEC_BASE + LPMCU_TELE_MNTN_SEC_SIZE) /* 0x21D00 */
#define IPC_TRACK_SIZE                          (0xC00)

#define LPMCU_DDR_DATA_SEC_BASE                 (IPC_TRACK_BASE + IPC_TRACK_SIZE) /* 0x22900 */
#define LPMCU_DDR_DATA_SEC_SIZE                 (0x9300)                                        /* 36.75K */
	#define DDR_REG_BACKUP_BASE                     (LPMCU_DDR_DATA_SEC_BASE)
	#define DDR_REG_BACKUP_SIZE                     (0x500)

	#define DDR_SEC_LOCK_REG_BASE			(DDR_REG_BACKUP_BASE + DDR_REG_BACKUP_SIZE)
	#define DDR_SEC_LOCK_REG_SIZE			(0x100)

	#define DDRC_LPRAM_PERI_SEC_RGN0_SAVE_BASE     (DDR_SEC_LOCK_REG_BASE + DDR_SEC_LOCK_REG_SIZE)
	#define DDRC_LPRAM_PERI_SEC_RGN0_SAVE_SIZE     0x10

	#define DDRC_LPRAM_PERI_SEC_RGNX_SAVE_BASE     (DDRC_LPRAM_PERI_SEC_RGN0_SAVE_BASE + DDRC_LPRAM_PERI_SEC_RGN0_SAVE_SIZE)
	#define DDRC_LPRAM_PERI_SEC_RGNX_SAVE_SIZE     0x280

	#define DDR_TRAINNING_BASE                      (DDRC_LPRAM_PERI_SEC_RGNX_SAVE_BASE + DDRC_LPRAM_PERI_SEC_RGNX_SAVE_SIZE)
	#define DDR_TRAINNING_SIZE                      (0x5670)

	#define DDR_PROFILE_BASE                        (DDR_TRAINNING_BASE + DDR_TRAINNING_SIZE)
	#define DDR_PROFILE_SIZE                        (0xF00)

	#define DMSS_PROFILE_BASE                        (DDR_PROFILE_BASE + DDR_PROFILE_SIZE)
	#define DMSS_PROFILE_SIZE                        (0x200)

	#define DDR_IO_PROFILE_BASE                     (DMSS_PROFILE_BASE + DMSS_PROFILE_SIZE)
	#define DDR_IO_PROFILE_SIZE                     (0x200)

	#define DDR_POLICY_BASE                         (DDR_IO_PROFILE_BASE + DDR_IO_PROFILE_SIZE)
	#define DDR_POLICY_SIZE                         (0x200)

	#define DDR_POLICY_LT_BASE                      (DDR_POLICY_BASE + DDR_POLICY_SIZE)
	#define DDR_POLICY_LT_SIZE                      (DDR_POLICY_SIZE)

	#define DDR_DBC_BASE                            (DDR_POLICY_LT_BASE + DDR_POLICY_LT_SIZE)
	#define DDR_DBC_SIZE                            (0x180)

	#define DDR_DMSS_GLB_BASE                       (DDR_DMSS_AXI_BASE + DDR_DMSS_AXI_SIZE)
	#define DDR_DMSS_GLB_SIZE                       (0x180)

	#define DDR_DMSS_AXI_BASE                       (DDR_DBC_BASE + DDR_DBC_SIZE)
	#define DDR_DMSS_AXI_SIZE                       (0xA00)

	#define DDR_DMSS_BYP_BASE                       (DDR_DMSS_GLB_BASE + DDR_DMSS_GLB_SIZE)
	#define DDR_DMSS_BYP_SIZE                       (0x60)
	/* 0X0 number_of_ddr_train 0X4 number_of_ddr_profile */
	/* 0X8 number_of_dmss_axi_cfg 0XC number_of_dmss_global_cfg */
	/* 0X10 number_of_dbc_cfg */
	#define DDR_NUM_CNT_BASE                       (DDR_DMSS_BYP_BASE + DDR_DMSS_BYP_SIZE)
	#define DDR_NUM_CNT_SIZE                       (0x20)

		#define DDR_TRAIN_NUMBER_BASE               (DDR_NUM_CNT_BASE)
		#define DDR_PROFILE_NUMBER_BASE             (DDR_NUM_CNT_BASE + 0x004)
		#define DMSS_AXI_CFG_NUMBER_BASE            (DDR_NUM_CNT_BASE + 0x008)
		#define DMSS_GLOBAL_CFG_NUMBER_BASE         (DDR_NUM_CNT_BASE + 0x00C)
		#define DBC_CFG_NUMBER_BASE                 (DDR_NUM_CNT_BASE + 0x010)
		#define TRAINIG_RESULT_IDX_MAP_SIZE_BASE    (DDR_NUM_CNT_BASE + 0x014)
		#define DMSS_PROFILE_NUMBER_BASE            (DDR_NUM_CNT_BASE + 0x018)

	#define DDR_VOL_BITMAP_BASE                    (DDR_NUM_CNT_BASE + DDR_NUM_CNT_SIZE)
	#define DDR_VOL_BITMAP_SIZE                    (0x40)

	#define DDR_FEATURE_BASE                       (DDR_VOL_BITMAP_BASE + DDR_VOL_BITMAP_SIZE)
	#define DDR_FEATURE_SIZE                       (0x10)

	#define DMSS_FEATURE_BASE                       (DDR_FEATURE_BASE + DDR_FEATURE_SIZE)
	#define DMSS_FEATURE_SIZE                       (0x10)

	#define DDR_TARGET_INFO_BASE                   (DMSS_FEATURE_BASE + DMSS_FEATURE_SIZE)
	#define DDR_TARGET_INFO_SIZE                   (0x140)

	#define UCE_CTRL_CFG_BASE                      (DDR_TARGET_INFO_BASE + DDR_TARGET_INFO_SIZE)
	#define UCE_CTRL_CFG_SIZE                      (0x8)

	#define DDR_INIT_INFO_BASE                     (UCE_CTRL_CFG_BASE + UCE_CTRL_CFG_SIZE)
	#define DDR_INIT_INFO_SIZE                     (0xA0)

	#define DMSS_INIT_INFO_BASE                     (DDR_INIT_INFO_BASE + DDR_INIT_INFO_SIZE)
	#define DMSS_INIT_INFO_SIZE                     (0xA0)

	#define DDR_LATSTAT_DATA_BASE                  (DMSS_INIT_INFO_BASE + DMSS_INIT_INFO_SIZE)
	#define DDR_LATSTAT_DATA_SIZE                  (0xA0)

	#define DDR_TRAINING_UCE_BASE                   (DDR_LATSTAT_DATA_BASE + DDR_LATSTAT_DATA_SIZE)
	#define DDR_TRAINING_UCE_SIZE                   0xBB0
#if ((DDR_TRAINING_UCE_BASE + DDR_TRAINING_UCE_SIZE) > (LPMCU_DDR_DATA_SEC_BASE + LPMCU_DDR_DATA_SEC_SIZE))
    #error "lpmcu ddr data section overflow!!!"
#endif


#define AON_SYS_BASE                            (LPMCU_DDR_DATA_SEC_BASE + LPMCU_DDR_DATA_SEC_SIZE) /* 0x2BC00 */
#define AON_SYS_RESERVED_SIZE                   (0x0)
#define AON_SYS_SIZE                            (0xE500 - AON_SYS_RESERVED_SIZE) /* 57.25K */

#define MODEM_IMAGE_BASE                        (AON_SYS_BASE + AON_SYS_SIZE + AON_SYS_RESERVED_SIZE) /* 0x3A100 */
#define MODEM_IMAGE_AO_SIZE                     (0x8800)  /* modem ao no less than 34K */
#define MODEM_IMAGE_SIZE                        (0x1A000) /* 104K */
/* ao region check */
#if ((MODEM_IMAGE_BASE + MODEM_IMAGE_AO_SIZE) > (M3_SRAM_BASE + M3_SRAM_AO_SIZE))
    #pragma message(PRINT_MACRO(MODEM_IMAGE_BASE + MODEM_IMAGE_AO_SIZE))
    #error "lpmcu ao region overflow!!!"
#endif
#if ((MODEM_IMAGE_BASE & 0xFF) != 0)
    #pragma message(PRINT_MACRO(MODEM_IMAGE_BASE))
    #error "modem image base not align at 0x100"
#endif


#define SYS_CODE_SEC_BASE                      (MODEM_IMAGE_BASE + MODEM_IMAGE_SIZE) /* 0x54100 */
#define SYS_CODE_SEC_RESERVED_SIZE             (0x0)
#define SYS_CODE_SEC_SIZE                      (0x1BF00-SYS_CODE_SEC_RESERVED_SIZE) /* 111.75K */

#define SYS_BSS_SEC_BASE                       (SYS_CODE_SEC_BASE + SYS_CODE_SEC_SIZE + SYS_CODE_SEC_RESERVED_SIZE) /* 0x70000 */
#define SYS_BSS_SEC_RESERVED_SIZE              (0x0)
#define SYS_BSS_SEC_SIZE                       (0x4D00-SYS_BSS_SEC_RESERVED_SIZE) /* 19.25K */

#define SYS_DATA_SEC_BASE                       (SYS_BSS_SEC_BASE + SYS_BSS_SEC_SIZE + SYS_BSS_SEC_RESERVED_SIZE) /* 0x74C00 */
#define SYS_DATA_SEC_RESERVED_SIZE              (0x0)
#define SYS_DATA_SEC_SIZE                       (0xC00-SYS_DATA_SEC_RESERVED_SIZE) /* 3k */

/*reuse bss*/
#define	CFGBUS_REG_SIZE							(4*1024) /* 4K */
#define	CFGBUS_REG_BASE							(SYS_BSS_SEC_BASE + SYS_BSS_SEC_SIZE + SYS_BSS_SEC_RESERVED_SIZE - CFGBUS_REG_SIZE) /* 0x75500 */
	#define CFGBUS_REG_SAVE_SIZE				(1*1024) /* 1K */

	#define CFGBUS_PERI_CRG_ADDR				(CFGBUS_REG_BASE) /* 0x75500 */
	#define CFGBUS_PERI_CRG_SIZE				(CFGBUS_REG_SAVE_SIZE)

	#define CFGBUS_SCTRL_ADDR					(CFGBUS_PERI_CRG_ADDR + CFGBUS_PERI_CRG_SIZE) /* 0x75600 */
	#define CFGBUS_SCTRL_SIZE					(CFGBUS_REG_SAVE_SIZE)

	#define CFGBUS_PMCTRL_ADDR					(CFGBUS_SCTRL_ADDR + CFGBUS_SCTRL_SIZE) /* 0x75700 */
	#define CFGBUS_PMCTRL_SIZE					(CFGBUS_REG_SAVE_SIZE)

	#define CFGBUS_PCTRL_ADDR					(CFGBUS_PMCTRL_ADDR + CFGBUS_PMCTRL_SIZE) /* 0x75800 */
	#define CFGBUS_PCTRL_SIZE					(CFGBUS_REG_SAVE_SIZE)


#define LITTLE_CLUSTER_PROFILE_BASE             (SYS_DATA_SEC_BASE + SYS_DATA_SEC_SIZE + SYS_DATA_SEC_RESERVED_SIZE) /* 0x75900 */
#define LITTLE_CLUSTER_PROFILE_SIZE             (0x300)

#define MIDDLE_CLUSTER_PROFILE_BASE             (LITTLE_CLUSTER_PROFILE_BASE + LITTLE_CLUSTER_PROFILE_SIZE) /* 0x75C00 */
#define MIDDLE_CLUSTER_PROFILE_SIZE             (0x300)

#define BIG_CLUSTER_PROFILE_BASE                (MIDDLE_CLUSTER_PROFILE_BASE + MIDDLE_CLUSTER_PROFILE_SIZE) /* 0x75F00 */
#define BIG_CLUSTER_PROFILE_SIZE                (0x300)

#define L3_CLUSTER_PROFILE_BASE                 (BIG_CLUSTER_PROFILE_BASE + BIG_CLUSTER_PROFILE_SIZE) /* 0x76200 */
#define L3_CLUSTER_PROFILE_SIZE                 (0x300)

/* little cluster profile */
/* middle cluster profile */
/* big cluster profile   */
/* l3 cluster profile   */
#define CPU_BASE_PROFILE_BASE(n)            (LITTLE_CLUSTER_PROFILE_BASE + (n) * LITTLE_CLUSTER_PROFILE_SIZE)

/* GPU profile  */
#define GPU_BASE_PROFILE_BASE                   (L3_CLUSTER_PROFILE_BASE + L3_CLUSTER_PROFILE_SIZE) /* 0x76500 */
#define GPU_BASE_PROFILE_SIZE                   (0x300)

#define SYS_STACK_SEC_BASE                      (GPU_BASE_PROFILE_BASE + GPU_BASE_PROFILE_SIZE) /* 0x76800 */
#define SYS_STACK_SEC_SIZE                      (0x400)                                 /* 1k */

#if ((SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE) != (M3_SRAM_BASE + M3_SRAM_SIZE))
    #pragma message(PRINT_MACRO(SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE))
    #error "lpmcu ram overflow!!!"
#endif


#define LPMCU_DDR_MNTN_BASE                      (SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE) /* 0x76C00 */
#define LPMCU_DDR_MNTN_SIZE                      (0x1400)              /* 5K */
    #define DDRC_SAVE_TMP_BASE                       (LPMCU_DDR_MNTN_BASE)
    #define DDRC_SAVE_TMP_SIZE                       (0x200)

    #define DDRC_SAVE_MEDIA_BASE                       (DDRC_SAVE_TMP_BASE + DDRC_SAVE_TMP_SIZE)
    #define DDRC_SAVE_MEDIA_SIZE                       (0x200)

    #define DDRC_SAVE_DMC_RINT                       (DDRC_SAVE_MEDIA_BASE + DDRC_SAVE_MEDIA_SIZE)
    #define DDRC_SAVE_DMC_RINT_SIZE                  (0x10)

    #define DDRC_SAVE_AVS_PA_BASE                    (DDRC_SAVE_DMC_RINT + DDRC_SAVE_DMC_RINT_SIZE)
    #define DDRC_SAVE_AVS_PA_SIZE                    (0x8)
    /* this area is used for multi purpose */
    #define DDRC_SAVE_BASE		(DDRC_SAVE_AVS_PA_BASE + DDRC_SAVE_AVS_PA_SIZE)
    #define DDRC_TRACKING_BASE                       (DDRC_SAVE_BASE)
    #define DDRC_SAVE_SIZE                           (0x900)
    #define DDRC_TRACKING_SIZE                       (DDRC_SAVE_SIZE)

#if ((DDRC_SAVE_BASE + DDRC_SAVE_SIZE) > (LPMCU_DDR_MNTN_BASE + LPMCU_DDR_MNTN_SIZE))
    #pragma message(PRINT_MACRO(DDRC_SAVE_BASE + DDRC_SAVE_SIZE))
    #error "lpmcu ddr mntn section size overflow!!!"
#endif
#if ((LPMCU_DDR_MNTN_BASE + LPMCU_DDR_MNTN_SIZE) > (M3_SRAM_BASE + 352*1024))
    #pragma message(PRINT_MACRO(LPMCU_DDR_MNTN_BASE + LPMCU_DDR_MNTN_SIZE))
    #error "lpmcu ddr mntn section end overflow!!!"
#endif


#define M3_SRAM_ADDR_OFFSET(addr)               ((addr) - M3_SRAM_BASE)

#define INT_LONG_TIME_TRACK_OFFSET              (INT_LONG_TIME_TRACK_BASE - LPMCU_GEN_MNTN_BASE)
#define IRQ_MASK_LONG_TIME_TRACK_OFFSET         (IRQ_MASK_LONG_TIME_TRACK_BASE - LPMCU_GEN_MNTN_BASE)
#define STACK_BACKUP_SRAM_OFFSET                (STACK_BACKUP_SRAM_BASE - LPMCU_GEN_MNTN_BASE)

#ifndef __FASTBOOT__
#define LPRAM_ADDR_TO_AP_ADDR(addr)             ((addr - SOC_LPMCU_LP_RAM_BASE_ADDR) + SOC_ACPU_LP_RAM_BASE_ADDR)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


#endif /* end of m3_sram_map.h */
