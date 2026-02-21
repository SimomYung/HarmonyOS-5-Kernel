/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2023-2023. All rights reserved.
 * Description: audio_base_addr.h
 * Create: 2023-12-11
 */
#ifndef _HI_AUDIO_BASE_ADDR_INCLUDE_H_
#define _HI_AUDIO_BASE_ADDR_INCLUDE_H_

#define RESERVED_HIFI_PHYMEM_BASE                 0x19E00000
#define RESERVED_HIFI_PHYMEM_SIZE                   0xF80000

#define RESERVED_HIFI_DATA_PHYMEM_BASE            0x1AE90000
#define RESERVED_HIFI_DATA_PHYMEM_SIZE              0x580000

/* for fastboot cma mem */
#define RESERVED_FASTBOOT_CMA_PHYMEM_BASE         0x3A400000

#ifndef __SOC_H_FOR_ASM__

/* Register description: 外设时钟使能状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERCLKEN0_UNION */
#define SOC_SCTRL_SCPERCLKEN0_ADDR(base)                      ((base) + (0x168UL))

/* Register description: 外设时钟最终状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT0_UNION */
#define SOC_SCTRL_SCPERSTAT0_ADDR(base)                       ((base) + (0x16CUL))

/* Register description: 外设时钟使能状态寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERCLKEN1_UNION */
#define SOC_SCTRL_SCPERCLKEN1_ADDR(base)                      ((base) + (0x178UL))

#else
/* Register description: 外设时钟使能状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERCLKEN0_UNION */
#define SOC_SCTRL_SCPERCLKEN0_ADDR(base)                      ((base) + (0x168))

/* Register description: 外设时钟最终状态寄存器0
   Bit domain definition UNION:  SOC_SCTRL_SCPERSTAT0_UNION */
#define SOC_SCTRL_SCPERSTAT0_ADDR(base)                       ((base) + (0x16C))

/* Register description: 外设时钟使能状态寄存器1
   Bit domain definition UNION:  SOC_SCTRL_SCPERCLKEN1_UNION */
#define SOC_SCTRL_SCPERCLKEN1_ADDR(base)                      ((base) + (0x178))
#endif

#define SOC_SCTRL_SCPEREN0_gt_clk_mad_acpu_START              (17)
#define SOC_SCTRL_SCPERSTAT0_st_clk_asp_subsys_START          (26)
#define SOC_SCTRL_SCPERCLKEN1_gt_clk_asp_subsys_acpu_START    (4)

#endif