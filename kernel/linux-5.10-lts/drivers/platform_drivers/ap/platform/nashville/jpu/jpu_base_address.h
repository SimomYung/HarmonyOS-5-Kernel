/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2024-2024. All rights reserved.
 * Description: jpu_base_addr.h
 * Create: 2024-10-26
 */
 
#ifndef _HI_JPU_BASE_ADDR_INCLUDE_H_
#define _HI_JPU_BASE_ADDR_INCLUDE_H_

#ifdef CONFIG_JPU_JPEGD
#include "soc_crgperiph_interface.h"
#endif

// part 1: soc_sctrl_interface.h
#ifndef __SOC_H_FOR_ASM__
/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA3_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA3_MSK_ADDR(base)                   ((base) + (0x4CCUL))
#else
/* Register description: 备份寄存器
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA3_MSK_UNION */
#define SOC_SCTRL_SCBAKDATA3_MSK_ADDR(base)                   ((base) + (0x4CC))
#endif

// part2: iomcu_ddr_map.h
#define RESERVED_SENSORHUB_SHMEMEXT_PHYMEM_BASE     0x10000000
#define DDR_SHMEMEXT_ADDR_AP              RESERVED_SENSORHUB_SHMEMEXT_PHYMEM_BASE
#define DDR_FLP_SIZE                      (0x180000)
#define DDR_FLP_END_AP                    (DDR_SHMEMEXT_ADDR_AP + DDR_FLP_SIZE)
#define DDR_DUMP_BASE_AP                   DDR_FLP_END_AP
#define DDR_DUMP_SIZE                      (0x100000)
#define DDR_DUMP_END_AP                    (DDR_DUMP_BASE_AP + DDR_DUMP_SIZE)
#define DDR_JPEGD_LB_BASE_AP               DDR_DUMP_END_AP

// part 3: soc_acpu_baseaddr_interface.h
/* Size: 12K, End at: 0xE0FD6FFF */
#define SOC_ACPU_TBU_BASE_ADDR                                  (0xE0FD4000)
/* Size: 4K, End at: 0xFFB85FFF */
#define SOC_ACPU_PERI_CRG_BASE_ADDR                             (0xFFB85000)
/* Size: 4K, End at: 0xE12F0FFF */
#define SOC_ACPU_MEDIA2_CRG_BASE_ADDR                           (0xE12F0000)

#endif