/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: global_ddr_map.h
 * Create: 2019-10-29
 */
#ifndef _HI_GLOBAL_MEM_MAP_INCLUDE_H_
#define _HI_GLOBAL_MEM_MAP_INCLUDE_H_

#include <reserved_phy_mem.h>

#define RESERVED_FASTBOOT_USED_PHYMEM_SIZE            0x4000000

#define ATF_RESERVED_BL2_PHYMEM_BASE                  (RESERVED_FASTBOOT_CMA_PHYMEM_BASE + RESERVED_FASTBOOT_USED_PHYMEM_SIZE)
#define ATF_RESERVED_BL2_PHYMEM_SIZE                  0x400000

#define RESERVED_BL2_FST_BUFF_BASE                    (ATF_RESERVED_BL2_PHYMEM_BASE + ATF_RESERVED_BL2_PHYMEM_SIZE)
#define RESERVED_BL2_FST_BUFF_SIZE                    0x01400000

/* alloc 64M for bl2 verify buf */
#define RESERVED_BL2_VERIFY_BUFF_BASE                 (RESERVED_BL2_FST_BUFF_BASE + RESERVED_BL2_FST_BUFF_SIZE)
#define RESERVED_BL2_VERIFY_BUFF_SIZE                 0x04000000

/* kernel must be in this space */
#define RESERVED_KERNEL_CAN_RUN_BASE                  RESERVED_KERNEL_PHYMEM_BASE
#define RESERVED_KERNEL_CAN_RUN_END                   RESERVED_KERNEL_PHYMEM_SIZE

/*
 * DTS_BASE
 * Ramdisk_base
 * the address define in Boardconfig.mk
 */

/* dts must be in this space */
#define RESERVED_DTB_CAN_RUN_BASE                      RESERVED_DTB_PHYMEM_BASE
#define RESERVED_DTB_CAN_RUN_END                       (RESERVED_DTB_PHYMEM_BASE + RESERVED_DTB_PHYMEM_SIZE)

/* ramdisk must be in this space */
#define RESERVED_NORMAL_RAMDISK_CAN_RUN_BASE           RESERVED_RAMDISK_PHYMEM_BASE
#define RESERVED_NORMAL_RAMDISK_CAN_RUN_SIZE           RESERVED_RAMDISK_PHYMEM_SIZE
#define RESERVED_NORMAL_RAMDISK_CAN_RUN_END            (RESERVED_NORMAL_RAMDISK_CAN_RUN_BASE + \
                                                        RESERVED_NORMAL_RAMDISK_CAN_RUN_SIZE)

/* reuse RESERVED_KERNEL_DUMP_PROTECT_BASE + RESERVED_SMEM_CMA_BASE */
#define RESERVED_RECOVERY_RAMDISK_CAN_RUN_BASE         RESERVED_KERNEL_DUMP_PROTECT_BASE
#define RESERVED_RECOVERY_RAMDISK_CAN_RUN_SIZE         (RESERVED_KERNEL_DUMP_PRORECT_SIZE + RESERVED_SMEM_CMA_SIZE)
#define RESERVED_RECOVERY_RAMDISK_CAN_RUN_END          (RESERVED_RECOVERY_RAMDISK_CAN_RUN_BASE + \
                                                        RESERVED_RECOVERY_RAMDISK_CAN_RUN_SIZE)

/* this memory is submemory of FAST_KER_AND start */
#define RESERVED_FAST_KER_AND_PHYMEM_BASE               RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE
#define RESERVED_FAST_KER_AND_PHYMEM_SIZE               0x40000

#define SUB_RESERVED_FASTBOOT_LOG_PYHMEM_BASE           (RESERVED_FAST_KER_AND_PHYMEM_BASE + \
                                                         RESERVED_FAST_KER_AND_PHYMEM_SIZE)
#define SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE           0x40000

#define SUB_RESERVED_SCHARGE_PYHMEM_BASE                (SUB_RESERVED_FASTBOOT_LOG_PYHMEM_BASE + \
                                                         SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE)
#define SUB_RESERVED_SCHARGE_PYHMEM_SIZE                0x1000

/* BL31 & Kernel share this 64K memory*/
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE     (SUB_RESERVED_SCHARGE_PYHMEM_BASE + \
                                                         SUB_RESERVED_SCHARGE_PYHMEM_SIZE)
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE     0x10000

/* for lcd gamma data 4k */
#define SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_BASE          (ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE + \
                                                         ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_SIZE          0x1000

/* for him data 4k */
#define SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_BASE  (SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_BASE + \
                                                            SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_SIZE  0x1000

/* for security random number inject 4k */
#define SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_BASE     (SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_BASE + \
                                                            SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_SIZE     0x1000

/* for BL2 log 64k*/
#define ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_BASE        (SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_BASE + \
                                                            SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_SIZE)
#define ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_SIZE        0x10000

#define SUB_RESERVED_DDR_REPAIR_PHYMEM_BASE             (ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_BASE + \
                                                            ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_DDR_REPAIR_PHYMEM_SIZE             0x1000

#define HISI_SUB_RESERVED_UEFI_DDRINFO_SHARE_BASE       SUB_RESERVED_DDR_REPAIR_PHYMEM_BASE
#define HISI_SUB_RESERVED_UEFI_DDRINFO_SHARE_SIZE       SUB_RESERVED_DDR_REPAIR_PHYMEM_SIZE

#define SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_BASE         (SUB_RESERVED_DDR_REPAIR_PHYMEM_BASE + \
                                                            SUB_RESERVED_DDR_REPAIR_PHYMEM_SIZE)
#define SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_SIZE         0x2000

/* for BL2 USB forcible loading 4K + 200k */
#define SUB_RESERVED_USB_BL2_SHARE_BASE                 (SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_BASE + \
                                                            SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_USB_BL2_SHARE_SIZE                 0x2000

#define RESERVED_SOC_SPEC_INFO_PHYMEM_BASE              (SUB_RESERVED_USB_BL2_SHARE_BASE + \
                                                            SUB_RESERVED_USB_BL2_SHARE_SIZE)
#define RESERVED_SOC_SPEC_INFO_PHYMEM_SIZE              0x1000


#define SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_BASE      (RESERVED_SOC_SPEC_INFO_PHYMEM_BASE + \
                                                            RESERVED_SOC_SPEC_INFO_PHYMEM_SIZE)
#define SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_SIZE      0x1000

#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE     (SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_BASE+ \
                                                            SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_SIZE)
#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE     (0x2000)

/* FORM RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE END */
#define SUB_RESERVED_MNTN_DUMP_PHYMEM_SIZE              0x1000
#define SUB_RESERVED_MNTN_DUMP_PHYMEM_BASE              (RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE + \
                                                            RESERVED_DRV_RESERVE_MEM_PHYMEM_SIZE - \
                                                            SUB_RESERVED_MNTN_DUMP_PHYMEM_SIZE)

#define RESERVED_SECOS_CMDMEM_SIZE                      0x1000
#define RESERVED_SECOS_CMDMEM_BASE                      (SUB_RESERVED_MNTN_DUMP_PHYMEM_BASE - \
                                                            RESERVED_SECOS_CMDMEM_SIZE)
#ifndef SUB_RESERVED_UNUSED_PHYMEM_BASE
#define SUB_RESERVED_UNUSED_PHYMEM_BASE                 (SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE + \
                                                            SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE)
#endif
#ifndef SUB_RESERVED_UNUSED_PHYMEM_SIZE
#define SUB_RESERVED_UNUSED_PHYMEM_SIZE                 (RESERVED_SECOS_CMDMEM_BASE - \
                                                            SUB_RESERVED_UNUSED_PHYMEM_BASE)
#endif

#if SUB_RESERVED_UNUSED_PHYMEM_SIZE < 0
#error "SUB_RESERVED_UNUSED_PHYMEM_SIZE < 0"
#endif
/* this memory is submemory of FAST_KER_AND end*/

#if defined(CONFIG_GCOV_ATF) || defined(CONFIG_HISI_GCOV_FASTBOOT)
#define ATF_RESERVED_BL31_PHYMEM_BASE               0x64000000
#define ATF_RESERVED_BL31_PHYMEM_SIZE               (0x1000000) /* 16MB for gocv use */
#else
#define ATF_RESERVED_BL31_PHYMEM_BASE               RESERVED_BL31_DATA_BASE
#define ATF_RESERVED_BL31_PHYMEM_SIZE               RESERVED_BL31_DATA_SIZE /* 2MB for normal use */
#endif

#define RESERVED_HIEPS_TEE_PHYMEM_BASE              RESERVED_HIEPS_PHYMEM_BASE
#define RESERVED_HIEPS_TEE_PHYMEM_SIZE              0x100000
#define RESERVED_HIEPS_REE_PHYMEM_BASE              (RESERVED_HIEPS_PHYMEM_BASE + RESERVED_HIEPS_TEE_PHYMEM_SIZE)
#define RESERVED_HIEPS_REE_PHYMEM_SIZE              0x100000

/* for metalbox img reserved, only used when enable_metalbox is set to true */
#ifdef CONFIG_BOOT_METALBOX
#define RESERVED_METALBOX_PHYMEM_BASE               0x314C0000
#define RESERVED_METALBOX_PHYMEM_SIZE               (0x400000)
#endif

/* Unique name across platforms.
 * Same name points to different value on different platform.
 */
#define RESERVED_LPMX_CORE_PHYMEM_BASE_UNIQUE       RESERVED_LPMX_CORE_PHYMEM_BASE
#define RESERVED_LPMCU_PHYMEM_BASE_UNIQUE           RESERVED_LPMCU_PHYMEM_BASE
#define RESERVED_MNTN_PHYMEM_BASE_UNIQUE            (RESERVED_MNTN_PHYMEM_BASE)

#ifdef CONFIG_HISI_GCOV_FASTBOOT
#define RESERVED_4G_DDR_FASTBOOT_GCDA_PHYMEM_BASE          (0xC0000000)
#define RESERVED_6G_DDR_FASTBOOT_GCDA_PHYMEM_BASE          (0x100000000ULL)
#define RESERVED_DDR_FASTBOOT_GCDA_PHYMEM_SIZE             (0x200000)
#endif

#ifdef CONFIG_HISI_GCOV_SLT_HIBENCH
#define RESERVED_4G_DDR_SLT_HIBENCH_GCDA_PHYMEM_BASE       (0xC0200000)
#define RESERVED_6G_DDR_SLT_HIBENCH_GCDA_PHYMEM_BASE       (0x100200000ULL)
#define RESERVED_SLT_HIBENCH_GCDA_PHYMEM_SIZE              (0x100000)
#endif

/* for UEFI encrypted boot mem, reuse RESERVED_DRM_CMA_BASE */
#define RESERVED_ENCRYPTED_BOOT_BASE                    RESERVED_DRM_CMA_BASE
#define RESERVED_ENCRYPTED_BOOT_SIZE                    0x4000000

/* for bl2 usb start 128MB, reuse  RESERVED_IRIS_CMA_BASE */
#define RESERVED_USBLOADER_BL2_BASE                     RESERVED_IRIS_CMA_BASE
#define RESERVED_USBLOADER_BL2_SIZE                     0x8000000

#ifndef HIBENCH_VERSION
/* UEFI system memory region (HOB,BootCode,BootData) */
#define HISI_RESERVED_UEFI_SIZE                     0x8000000ULL
#define HISI_RESERVED_UEFI_END                      (RESERVED_DRM_CMA_BASE + RESERVED_DRM_CMA_SIZE)
#define HISI_RESERVED_UEFI_BASE                     (HISI_RESERVED_UEFI_END - HISI_RESERVED_UEFI_SIZE)

/* UEFI Runtime memory */
#define HISI_RESERVED_UEFI_RT_SIZE                  0x800000ULL
#define HISI_RESERVED_UEFI_RT_END                   HISI_RESERVED_UEFI_BASE
#define HISI_RESERVED_UEFI_RT_BASE                  (HISI_RESERVED_UEFI_RT_END - HISI_RESERVED_UEFI_RT_SIZE)

#if HISI_RESERVED_UEFI_RT_BASE < RESERVED_ENCRYPTED_BOOT_BASE + RESERVED_ENCRYPTED_BOOT_SIZE
#error "RESERVED_DRM_CMA_BASE over flow!"
#endif

/* UEFI reserved memory region*/
#define HISI_RESERVED_UEFI_RSV_BASE                 RESERVED_IRIS_CMA_BASE
#define HISI_RESERVED_UEFI_RSV_SIZE                 0x4000000ULL
#define HISI_RESERVED_UEFI_RSV_END                  (HISI_RESERVED_UEFI_RSV_BASE + HISI_RESERVED_UEFI_RSV_SIZE)
#else
/* UEFI Runtime memory */
#define HISI_RESERVED_UEFI_RT_BASE                  0x43800000
#define HISI_RESERVED_UEFI_RT_SIZE                  0x800000ULL
#define HISI_RESERVED_UEFI_RT_END                   (HISI_RESERVED_UEFI_RT_BASE + HISI_RESERVED_UEFI_RT_SIZE)

/* UEFI reserved memory region */
#define HISI_RESERVED_UEFI_RSV_BASE                 0x44000000
#define HISI_RESERVED_UEFI_RSV_SIZE                 0x4000000ULL
#define HISI_RESERVED_UEFI_RSV_END                  (HISI_RESERVED_UEFI_RSV_BASE + HISI_RESERVED_UEFI_RSV_SIZE)

/* UEFI system memory region (HOB,BootCode,BootData) */
#define HISI_RESERVED_UEFI_BASE                     0x90000000
#define HISI_RESERVED_UEFI_SIZE                     0x8000000ULL
#define HISI_RESERVED_UEFI_END                      (HISI_RESERVED_UEFI_BASE + HISI_RESERVED_UEFI_SIZE)
#endif

#define HISI_SENSORHUB_IMAGE_SIZE                   0x380000
#define HISI_TEEOS_IMAGE_SIZE                       0x600000

#define HISI_RESERVED_BL2_UFS_DATA_BASE             RESERVED_SMEM_CMA_BASE
#define HISI_RESERVED_BL2_UFS_DATA_SIZE             0x1400000

/* UEFI & BL31 share this 64K memory*/
#define RESERVED_UEFI_BL31_SHARE_PHYMEM_BASE        (HISI_RESERVED_BL2_UFS_DATA_BASE + \
                                                        HISI_RESERVED_BL2_UFS_DATA_SIZE)
#define RESERVED_UEFI_BL31_SHARE_PHYMEM_SIZE        0xA00000
#endif
