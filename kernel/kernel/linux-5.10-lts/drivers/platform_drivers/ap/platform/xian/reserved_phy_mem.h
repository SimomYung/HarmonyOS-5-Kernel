/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2024-2024. All rights reserved.
 * Description: reserved_phy_mem.h
 * Create: 2024-08-23
 */
#ifndef _HI_RESERVED_PHY_MEM_INCLUDE_H_
#define _HI_RESERVED_PHY_MEM_INCLUDE_H_

/*****************Do not modify the following code*********************/
/* kernel must be in this space */
#define RESERVED_KERNEL_PHYMEM_BASE                  0
#define RESERVED_KERNEL_PHYMEM_SIZE                  0x10000000

/* rsv-mem start from 0x10000000 */
#define RESERVED_SENSORHUB_SHMEMEXT_PHYMEM_BASE      0x10000000
#define RESERVED_SENSORHUB_SHMEMEXT_PHYMEM_SIZE      0x300000

#define RESERVED_SENSORHUB_SHMEM_PHYMEM_BASE         0x10300000
#define RESERVED_SENSORHUB_SHMEM_PHYMEM_SIZE         0x40000

#define RESERVED_SENSORHUB_SHARE_MEM_PHYMEM_BASE     0x10340000
#define RESERVED_SENSORHUB_SHARE_MEM_PHYMEM_SIZE     0x380000

#define RESERVED_DRM_PGTABLE_BASE                    0x106C0000
#define RESERVED_DRM_PGTABLE_SIZE                    0x200000

/* only for DDR sec protect */
#define RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE         0x108C0000
#define RESERVED_DRV_RESERVE_MEM_PHYMEM_SIZE         0x100000

#define RESERVED_DP_HDCP2_PHYMEM_BASE                0x10F40000
#define RESERVED_DP_HDCP2_PHYMEM_SIZE                0x80000

#define RESERVED_HHEE_PHYMEM_BASE                    0x10FC0000
#define RESERVED_HHEE_PHYMEM_SIZE                    0x800000

#define RESERVED_LPMX_CORE_PHYMEM_BASE               0x117C0000
#define RESERVED_LPMX_CORE_PHYMEM_SIZE               0x80000

#define RESERVED_LPMCU_PHYMEM_BASE                   0x11840000
#define RESERVED_LPMCU_PHYMEM_SIZE                   0xC0000

#define RESERVED_SENSORHUB_PHYMEM_BASE               0x11900000
#define RESERVED_SENSORHUB_PHYMEM_SIZE               0xD00000

#define RESERVED_BL31_DATA_BASE                      0x12600000
#define RESERVED_BL31_DATA_SIZE                      0x200000

#define RESERVED_CRYPTO_CORE_PHYMEM_BASE             0x12800000
#define RESERVED_CRYPTO_CORE_PHYMEM_SIZE             0x380000

#define RESERVED_TCU_CFG_PHYMEM_BASE                 0x12B80000
#define RESERVED_TCU_CFG_PHYMEM_SIZE                 0x180000

#define RESERVED_NPU_TINY_PHYMEM_BASE                0x12D00000
#define RESERVED_NPU_TINY_PHYMEM_SIZE                0x800000

#define RESERVED_SENSORHUB_SDC_BASE                  0x13500000
#define RESERVED_SENSORHUB_SDC_SIZE                  0x100000

#define RESERVED_SECOS_PHYMEM_BASE                   0x13600000
#define RESERVED_SECOS_PHYMEM_SIZE                   0x3000000

/* 11MB thee work space */
#define RESERVED_THEE_BASE                           0x16600000
#define RESERVED_THEE_SIZE                           0xB00000

#define RESERVED_DICE_BASE                           0x17100000
#define RESERVED_DICE_SIZE                           0x10000

/* for indicator reserved 64K, used by teeos and dss */
#define RESERVED_INDICATOR_BASE                      0x17110000
#define RESERVED_INDICATOR_SIZE                      0x10000

#define RESERVED_CMDLIST_BUFFER_PHYMEM_BASE          0x1D000000
#define RESERVED_CMDLIST_BUFFER_PHYMEM_SIZE          0x100000

#define RESERVED_MODEM_NSRO_SHARE_PHYMEM_BASE        0x1E000000
#define RESERVED_MODEM_NSRO_SHARE_PHYMEM_SIZE        0xE00000

#define RESERVED_MODEM_SHARE_PHYMEM_BASE             0x1EE00000
#define RESERVED_MODEM_SHARE_PHYMEM_SIZE             0x200000

#define RESERVED_MODEM_MNTN_PHYMEM_BASE              0x1F000000
#define RESERVED_MODEM_MNTN_PHYMEM_SIZE              0x200000

#define RESERVED_MODEM_SAN_PHYMEM_BASE               0x20000000
#define RESERVED_MODEM_SAN_PHYMEM_SIZE               0x9E00000

#define RESERVED_SECOS_EX_PHYMEM_BASE                0x2A800000
#define RESERVED_SECOS_EX_PHYMEM_SIZE                0x1080000

#define RESERVED_SUPERSONIC_PHYMEM_BASE              0x2B880000
#define RESERVED_SUPERSONIC_PHYMEM_SIZE              0x9B0000

#define RESERVED_FINGERPRINT_BASE                    0x2C230000
#define RESERVED_FINGERPRINT_SIZE                    0x50000

#define RESERVED_SUPERSONIC_EX_PHYMEM_BASE           0x2C280000
#define RESERVED_SUPERSONIC_EX_PHYMEM_SIZE           0x980000

#define RESERVED_SEC_CAMERA_PHYMEM_BASE              0x2CC00000
#define RESERVED_SEC_CAMERA_PHYMEM_SIZE              0xC00000

#define RESERVED_HIEPS_PHYMEM_BASE                   0x2D800000
#define RESERVED_HIEPS_PHYMEM_SIZE                   0x200000

#define RESERVED_HIFI_PHYMEM_BASE                    0x2DA00000
#define RESERVED_HIFI_PHYMEM_SIZE                    0x1180000

#define RESERVED_NPU_AI_TS_FW_PHYMEM_BASE            0x2EB80000
#define RESERVED_NPU_AI_TS_FW_PHYMEM_SIZE            0x400000

#define RESERVED_NPU_AI_SERVER_PHYMEM_BASE           0x2EF80000
#define RESERVED_NPU_AI_SERVER_PHYMEM_SIZE           0x600000

#define RESERVED_HIFI_DATA_PHYMEM_BASE               0x2F580000
#define RESERVED_HIFI_DATA_PHYMEM_SIZE               0x600000

#define RESERVED_MNTN_PHYMEM_BASE                    0x2FB80000
#define RESERVED_MNTN_PHYMEM_SIZE                    0x620000

#define BL31_NS_LOG_BASE                             0x301A0000
#define BL31_NS_LOG_SIZE                             0x20000

#define RESERVED_PSTORE_PHYMEM_BASE                  0x301C0000
#define RESERVED_PSTORE_PHYMEM_SIZE                  0x100000

#define RESERVED_KEVKBOX_PHYMEM_BASE                 0x302C0000
#define RESERVED_KEVKBOX_PHYMEM_SIZE                 0xB04000

#define RESERVED_COREDUMP_PHYMEM_BASE                0x30DC4000
#define RESERVED_COREDUMP_PHYMEM_SIZE                0x200000

#define RESERVED_FFRT_FFTS_PHYMEM_BASE               0x32F00000
#define RESERVED_FFRT_FFTS_PHYMEM_SIZE               0x480000

#define RESERVED_LOGO_BUFFER_PHYMEM_BASE             0x33380000
#define RESERVED_LOGO_BUFFER_PHYMEM_SIZE             0x7080000

/* for fastboot cma mem */
#define RESERVED_FASTBOOT_CMA_PHYMEM_BASE            0x3A400000
#define RESERVED_FASTBOOT_CMA_PHYMEM_SIZE            0x5C00000

#define RESERVED_DRM_CMA_BASE                        0x40000000
#define RESERVED_DRM_CMA_SIZE                        0x10000000

#define RESERVED_IRIS_CMA_BASE                       0x50000000
#define RESERVED_IRIS_CMA_SIZE                       0xA400000

#define RESERVED_DRM_REDRAW_CMA_BASE                 0x5A400000
#define RESERVED_DRM_REDRAW_CMA_SIZE                 0x4800000

#define RESERVED_TINY_CMA_BASE                       0x60000000
#define RESERVED_TINY_CMA_SIZE                       0x2800000

#define RESERVED_DTB_PHYMEM_BASE                     0x66000000
#define RESERVED_DTB_PHYMEM_SIZE                     0x800000

#define RESERVED_RAMDISK_PHYMEM_BASE                 0x67000000
#define RESERVED_RAMDISK_PHYMEM_SIZE                 0x800000

/* hota eiius: as worksapce of encrypto image increment update */
#define RESERVED_HOTA_PHYMEM_BASE                    0x70000000
#define RESERVED_HOTA_PHYMEM_SIZE                    0x10000000

/* only for DDR sec protect :kernel dump */
#define RESERVED_KERNEL_DUMP_PROTECT_BASE            0x80000000
#define RESERVED_KERNEL_DUMP_PRORECT_SIZE            0x10000000

#define RESERVED_SMEM_CMA_BASE                       0x90000000
#define RESERVED_SMEM_CMA_SIZE                       0x10000000

#define RESERVED_MODEM_PHYMEM_BASE                   0xA0000000
#define RESERVED_MODEM_PHYMEM_SIZE                   0xB100000

#define RESERVED_MODEM_TEE_PHYMEM_BASE               0xAB100000
#define RESERVED_MODEM_TEE_PHYMEM_SIZE               0x100000

#define RESERVED_MODEM_SOCP_PHYMEM_BASE              0xB0F80000
#define RESERVED_MODEM_SOCP_PHYMEM_SIZE              0x8000000

#define RESERVED_AOD_DYNAMIC_CMA_BASE                0xC0000000
#define RESERVED_AOD_DYNAMIC_CMA_SIZE                0xA400000
/*****************Do not modify the preceding code*********************/

#endif
