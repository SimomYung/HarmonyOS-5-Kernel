/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2025. All rights reserved.
 * Description: lpm_kernel_map.h
 * Create: 2019-10-29
 */

#ifndef LPM_KERNEL_MAP_H
#define LPM_KERNEL_MAP_H

#define EXC_SPECIAL_SAVE_SIZE                0x40
#define INT_TRACK_SIZE                       0x2C0
#define TASK_TRACK_SIZE                      0x2C0
#define LPMCU_TELE_MNTN_DATA_TICKMARK_SIZE   0x3C0
#define LPMCU_TELE_MNTN_DATA_STRUCT_ACTUAL_SIZE	0x600
#define LPMCU_TELE_MNTN_DATA_DDR_MARK_SIZE   0x70
#define IPC_TRACK_SIZE                       (0xC00)
#define SOC_SCTRL_SCBAKDATA10_ADDR(base)     ((base) + (0x434))
/* BL31 & Kernel share this 64K memory*/
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE     0x10941000
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE     (0x10000)

#define RESERVED_LPMX_CORE_PHYMEM_BASE              0x127C0000
#define RESERVED_LPMX_CORE_PHYMEM_SIZE              (0x80000)
#define RESERVED_LPMCU_PHYMEM_BASE                  0x12840000
#define RESERVED_LPMCU_PHYMEM_SIZE                  (0xC0000)

#define SOC_ACPU_SCTRL_BASE_ADDR                                (0xFDB23000)
#define SOC_SCTRL_SCBAKDATA8_ADDR(base)                       ((base) + (0x42CUL))

#endif /* end of lpm_kernel_map.h */
