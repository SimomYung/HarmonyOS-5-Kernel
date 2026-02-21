/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: DDR memory to store data.
 * The following code describes the specific partition of the DDR.
 */

#ifndef __VDEC_MCU_DDR_MAP_H__
#define __VDEC_MCU_DDR_MAP_H__
#include "vcodec_vdec.h"

uint32_t vdec_plat_reserved_ion_base(void);
// segbuffer,size:256M
#define VDEC_SEG_BUF_BASE vdec_plat_reserved_ion_base()
#define VDEC_SEG_BUF_ONE_BLOCK_SIZE (16 * 1024 * 1024)
#define VDEC_SEG_BUF_TOTAL_SIZE (512 * 1024 * 1024)

// mcu, size:9M
#define VDEC_MCU_BASE (vdec_plat_reserved_ion_base() + VDEC_SEG_BUF_TOTAL_SIZE + 0x1000000)
#define VDEC_MCU_SIZE (11 * 1024 * 1024)

// ringbuffer, size:160M
#define VDEC_BSP_RING_BUFFER_BASE (vdec_plat_reserved_ion_base() + VDEC_SEG_BUF_TOTAL_SIZE + 0x2000000)
#define BSP_CTU_INFO_RING_SIZE (45 * 1024 * 1024)
#define BSP_CTU_RES_RING_SIZE  (35 * 1024 * 1024)
#define VDEC_BSP_RING_BUFFER_SIZE (2 * (BSP_CTU_INFO_RING_SIZE + BSP_CTU_RES_RING_SIZE))

// msg pool, size:12M
#define VDEC_MSG_POOL_BASE  (vdec_plat_reserved_ion_base() + VDEC_SEG_BUF_TOTAL_SIZE + 0xCC00000)
#define VDEC_MSG_POOL_SIZE  (12 * 1024 * 1024)

// bsp sw, size:16K
#define BSP_SW_BUFFER_BASE     (vdec_plat_reserved_ion_base() + VDEC_SEG_BUF_TOTAL_SIZE + 0xE000000)
#define BSP_SW_BUFFER_SIZE     (16 * 1024)

// heap, size:128M
#define VDEC_MCU_HEAP_BASE (vdec_plat_reserved_ion_base() + VDEC_SEG_BUF_TOTAL_SIZE + 0x10000000)
#define VDEC_MCU_HEAP_ONE_BLOCK_SIZE (16 * 1024 * 1024)
#define VDEC_MCU_HEAP_TOTAL_SIZE (128 * 1024 * 1024)


// ===========  mcu memory detail  ============
#define MCU_TEXT_BASE             (VDEC_MCU_BASE)
#define MCU_TEXT_SIZE             (1 * 1024 * 1024)
#define MCU_ENTRY_ADDR            (MCU_TEXT_BASE)

#define MCU_DATA_BASE             (MCU_TEXT_BASE + MCU_TEXT_SIZE)
#define MCU_DATA_SIZE             (2 * 1024 * 1024)
#define MCU_IMAGE_SIZE            (MCU_DATA_SIZE + MCU_TEXT_SIZE)

#define MCU_OS_MEM_BASE           (MCU_DATA_BASE + MCU_DATA_SIZE)
#define MCU_OS_MEM_SIZE           (1 * 1024 * 1024)

#define MCU_LOG_BUFFER_BASE         (MCU_OS_MEM_BASE + MCU_OS_MEM_SIZE)
#define MCU_LOG_BUFFER_SIZE         (1 * 1024 * 1024)
#define MCU_LOG_BUFFER_OFFSET       (MCU_LOG_BUFFER_BASE - VDEC_MCU_BASE)

#define INPUT_MSG_QUEUE_BASE       (MCU_LOG_BUFFER_BASE + MCU_LOG_BUFFER_SIZE)
#define INPUT_MSG_QUEUE_SIZE       (1 * 1024 * 1024)
#define INPUT_MSG_QUEUE_OFFSET     (INPUT_MSG_QUEUE_BASE - VDEC_MCU_BASE)

#define OUTPUT_MSG_QUEUE_ONE_CHAN_SIZE (128 * 1024)
#define OUTPUT_MSG_QUEUE_BASE     (INPUT_MSG_QUEUE_BASE + INPUT_MSG_QUEUE_SIZE)
#define OUTPUT_MSG_QUEUE_SIZE     (OUTPUT_MSG_QUEUE_ONE_CHAN_SIZE * 32)  // MAX_CHAN_ID
#define OUTPUT_MSG_QUEUE_OFFSET   (OUTPUT_MSG_QUEUE_BASE - VDEC_MCU_BASE)

#define IPC_AP2MCU_MEMORY_BASE    (OUTPUT_MSG_QUEUE_BASE + OUTPUT_MSG_QUEUE_SIZE)
#define IPC_AP2MCU_MEMORY_SIZE    (512 * 1024)
#define IPC_AP2MCU_MEMORY_OFFSET  (IPC_AP2MCU_MEMORY_BASE - VDEC_MCU_BASE)

#define IPC_MCU2AP_MEMORY_BASE    (IPC_AP2MCU_MEMORY_BASE + IPC_AP2MCU_MEMORY_SIZE)
#define IPC_MCU2AP_MEMORY_SIZE    (512 * 1024)
#define IPC_MCU2AP_MEMORY_OFFSET  (IPC_MCU2AP_MEMORY_BASE - VDEC_MCU_BASE)

#endif