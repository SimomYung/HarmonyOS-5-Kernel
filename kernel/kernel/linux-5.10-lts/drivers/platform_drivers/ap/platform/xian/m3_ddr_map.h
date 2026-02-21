/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2023. All rights reserved.
 * Description: m3_ddr_map.h
 * Create: 2019-10-29
 */

#ifndef M3_DDR_MAP_H
#define M3_DDR_MAP_H

#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
#include <lpm_kernel_map.h>
#include <reserved_phy_mem.h>
#else
#include <global_ddr_map.h>
#endif

/* RAM SIZE */
#define LPMCU_RAM_PHY_SIZE                      (384 * 1024)
#define LPMCU_RAM_SIZE                          (378 * 1024 - 0x40)  /* 0x40 use in MSPC size */
#define UCE_RAM_SIZE                            (32 * 1024)

/* ddr distribution of macro definition */
#define M3_DDR_MEM_BASE_ADDR                    (RESERVED_LPMX_CORE_PHYMEM_BASE)
#define M3_DDR_MEM_BASE_SIZE                    (RESERVED_LPMX_CORE_PHYMEM_SIZE)

#define LPMCU_SECURE_DDR_BASE_ADDR                    (RESERVED_LPMCU_PHYMEM_BASE)
#define LPMCU_SECURE_DDR_BASE_SIZE                    (RESERVED_LPMCU_PHYMEM_SIZE)

/*
 * RESERVED_LPMX_CORE_PHYMEM_BASE share with acpu
 * TELE MNTN memory area, must be placed in the beginning of the whole space
 */
#define TELE_MNTN_AREA_ADDR                     (M3_DDR_MEM_BASE_ADDR)
#define TELE_MNTN_AREA_SIZE                     0x78000 /* 480K */

/* share ddr */
#define M3_DDR_SHARE_MEM_DDR2FASTBOOT_BASE      (TELE_MNTN_AREA_ADDR + TELE_MNTN_AREA_SIZE)
#define M3_DDR_SHARE_MEM_DDR2FASTBOOT_SIZE      0x4000 /* 16K */

/* used by save lpram mntn not overwrite by xloader */
#define M3_MNTN_NOT_OVERWRITE_ADDR    (M3_DDR_SHARE_MEM_DDR2FASTBOOT_BASE + M3_DDR_SHARE_MEM_DDR2FASTBOOT_SIZE)
#define M3_MNTN_NOT_OVERWRITE_SIZE    0x1400 /* 5K */

/* used before load lpm3 */
#define PMU_DUMP_ADDR             (M3_MNTN_NOT_OVERWRITE_ADDR + M3_MNTN_NOT_OVERWRITE_SIZE)
#define PMU_DUMP_SIZE             0x400 /* 1K */

/* increase the relevant memory allocation here, modify M3_DDR_MEM_USED_SIZE value at the same time */
#define M3_DDR_MEM_USED_SIZE      (PMU_DUMP_ADDR + PMU_DUMP_SIZE - M3_DDR_MEM_BASE_ADDR)

#if (M3_DDR_MEM_USED_SIZE > (M3_DDR_MEM_BASE_SIZE))
#error m3_ddr_memory_size used beyond (M3_DDR_MEM_USED_SIZE)
#endif

/* Reserved_LPMCU_PHYMEM_SIZE must be 4K aligned. Otherwise, page table loading fails. */
#define LPMCU_FIRMWARE_PACK_BACKUP_ADDR         (LPMCU_SECURE_DDR_BASE_ADDR)
#define LPMCU_FIRMWARE_PACK_BACKUP_SIZE         (456 * 1024)

	/* sram image backup */
	#define LPMCU_FIRMWARE_BACKUP_ADDR              (LPMCU_FIRMWARE_PACK_BACKUP_ADDR)
	#define LPMCU_FIRMWARE_BACKUP_SIZE              (LPMCU_RAM_SIZE)
	#define LPM3_MAGIC_ADDR                         (LPMCU_FIRMWARE_BACKUP_ADDR + 0x044)

	/* m3 ddr runspace */
	#define LPMCU_DDR_RUNSPACE_ADDR                 (LPMCU_FIRMWARE_BACKUP_ADDR + LPMCU_FIRMWARE_BACKUP_SIZE)
	#define LPMCU_DDR_RUNSPACE_SIZE                 0x10000 /* 64K */

	/* m3 secimage cert */
	#define LPMCU_SECIMG_CERT_ADDR			(LPMCU_DDR_RUNSPACE_ADDR + LPMCU_DDR_RUNSPACE_SIZE)
	#define LPMCU_SECIMG_CERT_SIZE			0x2000 /* 8K */

	#define M3_AO_TCP_ADDR                          (LPMCU_DDR_RUNSPACE_ADDR + LPMCU_DDR_RUNSPACE_SIZE)
	#define M3_AO_TCP_ADDR_SIZE                     0x1000 /* 4K */

	/* increase the relevant memory allocation here, modify M3_DDR_SYS_MEM_USED_SIZE value at the same time */
	#define LPMCU_FIRMWARE_PACK_BACKUP_USED_SIZE      (M3_AO_TCP_ADDR + M3_AO_TCP_ADDR_SIZE - LPMCU_FIRMWARE_PACK_BACKUP_ADDR)

#if (LPMCU_FIRMWARE_PACK_BACKUP_USED_SIZE > (LPMCU_FIRMWARE_PACK_BACKUP_SIZE))
#error m3_system_memory_size(LPMCU_FIRMWARE_PACK_BACKUP_SIZE) used beyond (LPMCU_FIRMWARE_PACK_BACKUP_SIZE)
#endif

#if ((LPMCU_FIRMWARE_PACK_BACKUP_SIZE % (4 * 1024)) != 0)
#error LPMCU_FIRMWARE_PACK_BACKUP_SIZE should be an integral multiple of 4K
#endif

/* used by perchip avs */
#define PER_CHIP_AVS_FLAG_ADDR                  (LPMCU_FIRMWARE_PACK_BACKUP_ADDR + LPMCU_FIRMWARE_PACK_BACKUP_SIZE)
#define PER_CHIP_AVS_FLAG_SIZE                  1024

/* reserved */
#define M3_DDR_RESERVED_ADDR                    (PER_CHIP_AVS_FLAG_ADDR + PER_CHIP_AVS_FLAG_SIZE)
#define M3_DDR_RESERVED_SIZE                    0x13c00 /* 79K */

/* ddr private */
#define M3_DDR_SHARE_MEM_ADDR                   (M3_DDR_RESERVED_ADDR + M3_DDR_RESERVED_SIZE)
#define M3_DDR_SHARE_MEM_SIZE                   0x3A000 /* 224K */

	#define M3_DDR_INIT_UCE_MNTN_BASE               (M3_DDR_SHARE_MEM_ADDR)
	#define M3_DDR_INIT_UCE_MNTN_SIZE               0x24400 /* 145K */

	#define M3_DDR_INIT_XLOADER_TO_FASTBOOT_BASE    (M3_DDR_INIT_UCE_MNTN_BASE + M3_DDR_INIT_UCE_MNTN_SIZE)
	#define M3_DDR_INIT_XLOADER_TO_FASTBOOT_SIZE    0x8000 /* 32K */

	#define M3_DDR_STORAGE_MEM_DDR_MPU_ADDR         (M3_DDR_INIT_XLOADER_TO_FASTBOOT_BASE + M3_DDR_INIT_XLOADER_TO_FASTBOOT_SIZE)
	#define M3_DDR_STORAGE_MEM_DDR_MPU_SIZE         0x200 /* 0.5k */

	#define M3_DDR_STORAGE_MEM_DDR_SECURITY_ADDR    (M3_DDR_STORAGE_MEM_DDR_MPU_ADDR + M3_DDR_STORAGE_MEM_DDR_MPU_SIZE)
	#define M3_DDR_STORAGE_MEM_DDR_SECURITY_SIZE    0x3A00 /* 14.5K */

	#define UCE_FIRMWARE_BACKUP_ADDR                (M3_DDR_STORAGE_MEM_DDR_SECURITY_ADDR + M3_DDR_STORAGE_MEM_DDR_SECURITY_SIZE)
	#define UCE_FIRMWARE_BACKUP_SIZE                (UCE_RAM_SIZE)

	#define M3_DDR_SHARE_MEM_AGE_ADDR               (UCE_FIRMWARE_BACKUP_ADDR + UCE_FIRMWARE_BACKUP_SIZE)
	#define M3_DDR_SHARE_MEM_AGE_SIZE               0x400 /* 1k use for aging */
	/* increase the relevant memory allocation here, modify M3_SHARE_MEM_RESERVED_ADDR value at the same time */
	#define M3_DDR_SHARE_MEM_RESERVED_ADDR          (M3_DDR_SHARE_MEM_AGE_ADDR + M3_DDR_SHARE_MEM_AGE_SIZE)

#if (M3_DDR_SHARE_MEM_RESERVED_ADDR > (M3_DDR_SHARE_MEM_ADDR + M3_DDR_SHARE_MEM_SIZE))
#error m3_share_memory_addr(M3_DDR_SHARE_MEM_RESERVED_ADDR) used beyond (M3_DDR_SHARE_MEM_ADDR + M3_DDR_SHARE_MEM_SIZE)
#endif

#define M3_DDR_MEM_RESERVED_ADDR                (M3_DDR_SHARE_MEM_ADDR + M3_DDR_SHARE_MEM_SIZE)

#if (M3_DDR_MEM_RESERVED_ADDR > (LPMCU_SECURE_DDR_BASE_ADDR + LPMCU_SECURE_DDR_BASE_SIZE))
#error m3_memory_addr(M3_DDR_MEM_RESERVED_ADDR) used beyond (LPMCU_SECURE_DDR_BASE_ADDR + LPMCU_SECURE_DDR_BASE_SIZE)
#endif

/* lpm3 modem image load addr */
#define LPMCU_MODEM_DDR_ADDR_START RESERVED_MODEM_NSRO_SHARE_PHYMEM_BASE
#define LPMCU_MODEM_DDR_ADDR_END (RESERVED_MODEM_NSRO_SHARE_PHYMEM_BASE + RESERVED_MODEM_NSRO_SHARE_PHYMEM_SIZE)

/* ddr virtual physical conversion */
#define DDR_WIN_MASK                          0xF0000000
#define DDR_PHY_TO_WIN(phy_addr)              ((phy_addr) & DDR_WIN_MASK)
#define DDR_VIRT_TO_PHY(virt_addr, win_addr)  (((virt_addr) & 0x0FFFFFFF) | (win_addr))
#define DDR_PHY_TO_VIRT(phy_addr)             (((phy_addr) & 0x0FFFFFFF) | 0x10000000)

#endif
