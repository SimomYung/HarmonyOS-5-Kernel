/*
 * drv_venc_osal.h
 *
 * This is for venc drv.
 *
 * Copyright (c) 2009-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __VDEC_MEMORY_MAP_H__
#define __VDEC_MEMORY_MAP_H__

#include "vcodec_types.h"

uint32_t vdec_plat_reg_base_addr(void);
// vdec reg base addr
#define VDEC_REG_BASE                vdec_plat_reg_base_addr()
#define VDEC_REG_RANGE               (5 * 1024 * 1024)

// bsp reg addr
#define BSP_REG_BASE                (VDEC_REG_BASE + 0x00000000)
#define BSP_REG_RANGE                (2 * 1024 * 1024)
#define BSP_TOP_SCTRL                (VDEC_REG_BASE + 0x00000000)
#define BSPS_SCTRL                   (VDEC_REG_BASE + 0x00064000)
#define BSP_TASK_QUEUE_REG_NSF       (VDEC_REG_BASE + 0x0007B400)
#define BSP_TASK_QUEUE_REG_SF        (VDEC_REG_BASE + 0x0007B800)
#define BSP_CMTAB_ADDR               (VDEC_REG_BASE + 0x00044000)

// pxp reg addr
#define PXP_REG_BASE                (VDEC_REG_BASE + 0x00200000)
#define PXP_REG_RANGE               (2 * 1024 * 1024)
#define PXPC_TASK_QUEUE_SF          (VDEC_REG_BASE + 0x00249850)
#define PXPC_TASK_QUEUE_NSF         (VDEC_REG_BASE + 0x0024FC00)
#define PXPC_SCTRL                  (VDEC_REG_BASE + 0x00280000)
#define PXPC_DMA                    (VDEC_REG_BASE + 0x00290000)
#define PXP_HW                      (VDEC_REG_BASE + 0x002A0000)

// scd reg addr
#define SCD_REG_BASE                (VDEC_REG_BASE + 0x00400000)
#define SCD_REG_RANGE                1024

// smmu tbu reg addr
#define SMMU_TBU_REG_BASE                (VDEC_REG_BASE + 0x00401000)
#define SMMU_TBU_REG_RANGE                (16 * 1024)

// smmu pre reg addr
#define SMMU_PRE_REG_BASE                (VDEC_REG_BASE + 0x00406000)
#define SMMU_PRE_REG_RANGE                (4 * 1024)

// subctrl reg addr
#define SUB_CTRL_REG_BASE                (VDEC_REG_BASE + 0x00408000)
#define SUB_CTRL_REG_RANGE                (3 * 1024)

// subctrl secure reg addr
#define SUB_CTRL_S_REG_BASE                (VDEC_REG_BASE + 0x00408c00)
#define SUB_CTRL_S_REG_RANGE            (1024)

// ipc reg addr
#define IPC_REG_BASE                (VDEC_REG_BASE + 0x0040C000)
#define IPC_REG_RANGE               (8 * 1024)

#define MCU_TCM_BASE                (VDEC_REG_BASE + 0x004E0000)
#define MCU_TCM_RANGE               (32*1024)

// RISCV reg addr
// 4K	0x00409000	0x00409FFF	WatchDog
// 4K	0x0040A000	0x0040AFFF	Timer
// 4K	0x0040B000	0x0040BFFF	UART
// 8K	0x0040C000	0x0040DFFF	IPC
// 60K	0x0040F000	0x0041DFFF	RISCV(TBD)
// 64K	0x00422000	0x00431FFF	RISCV_ITCM(TBD)


#define OFFSET_OF(base, addr)    ((addr) - (base))
#define OFFSET_OF_VDEC_BASE(addr)    ((addr) - VDEC_REG_BASE)

#endif
