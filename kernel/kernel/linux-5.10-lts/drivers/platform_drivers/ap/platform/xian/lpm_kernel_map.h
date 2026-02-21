/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2025. All rights reserved.
 * Description: lpm_kernel_map.h
 * Create: 2019-10-29
 */

#ifndef LPM_KERNEL_MAP_H
#define LPM_KERNEL_MAP_H

#define LPMCU_RAM_PHY_SIZE                   (384 * 1024)

#define EXC_SPECIAL_SAVE_SIZE                0x40
#define INT_TRACK_SIZE                       0x2C0
#define TASK_TRACK_SIZE                      0x2C0
#define LPMCU_TELE_MNTN_DATA_TICKMARK_SIZE   0x3C0
#define LPMCU_TELE_MNTN_DATA_STRUCT_ACTUAL_SIZE	0x600
#define LPMCU_TELE_MNTN_DATA_DDR_MARK_SIZE   0x70
#define IPC_TRACK_SIZE                       (0xA00)
#define SOC_SCTRL_SCBAKDATA10_ADDR(base)     ((base) + (0x434))

#define LPMCU_RAM_SIZE                       (378 * 1024 - 0x40)  /* 0x40 use in MSPC size */

#define RESERVED_LPMX_CORE_PHYMEM_BASE              0x117C0000
#define RESERVED_LPMX_CORE_PHYMEM_SIZE              (0x80000)
#define RESERVED_LPMCU_PHYMEM_BASE                  0x11840000
#define RESERVED_LPMCU_PHYMEM_SIZE                  (0xC0000)

#define SOC_ACPU_SCTRL_BASE_ADDR             (0xFDB23000)
#define SOC_SCTRL_SCBAKDATA8_ADDR(base)      ((base) + 0x42CUL)

#endif /* end of lpm_kernel_map.h */
