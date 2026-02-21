/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2022-2022. All rights reserved.
 * Description: asp's baseaddr and offset summarizing
 * Create: 2022-04-21
 */

#ifndef __ASP_REGISTER_H__
#define __ASP_REGISTER_H__

/* Size: 4K, End at: 0xFA89BFFF */
#define SOC_ACPU_SCTRL_BASE_ADDR                                (0xFA89B000)

/* Size: 8K, End at: 0xFA553FFF */
#define SOC_ACPU_ASP_CODEC_BASE_ADDR                            (0xFA552000)

/* Size: 1K, End at: 0xFA54FFFF */
#define SOC_ACPU_DSD_BASE_ADDR                                  (0xFA54FC00)

/* Size: 8K, End at: 0xFA551FFF */
#define SOC_ACPU_SLIMBUS_BASE_ADDR                              (0xFA550000)

/* Size: 1K, End at: 0xFA54E3FF */
#define SOC_ACPU_ASP_CFG_BASE_ADDR                              (0xFA54E000)

/* Size: 4K, End at: 0xFA54DFFF */
#define SOC_ACPU_ASP_WD_BASE_ADDR                               (0xFA54D000)

/* Size: 4K, End at: 0xFA54BFFF */
#define SOC_ACPU_ASP_DMAC_BASE_ADDR                             (0xFA54B000)

/* Size: 256K, End at: 0xFA4BFFFF */
#define SOC_ACPU_SECRAM_BASE_ADDR                               (0xFA480000)

#endif
