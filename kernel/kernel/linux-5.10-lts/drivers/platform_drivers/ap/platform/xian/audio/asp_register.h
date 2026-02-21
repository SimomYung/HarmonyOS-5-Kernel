/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2023. All rights reserved.
 * Description: asp's baseaddr and offset summarizing
 * Create: 2023-09-20
 */

#ifndef __ASP_REGISTER_H__
#define __ASP_REGISTER_H__

/* Size: 4K, End at: 0xFDB4FFFF */
#define SOC_ACPU_intr_hub_ao_BASE_ADDR                          (0xFDB4F000)
/* Size: 4K, End at: 0xFDB4DFFF */
#define SOC_ACPU_AO_CRG_BASE_ADDR                               (0xFDB4D000)
/* Size: 4K, End at: 0xFDB29FFF */
#define SOC_ACPU_PAD_AO_IOMG_IOCG_BASE_ADDR                     (0xFDB29000)
/* Size: 4K, End at: 0xFDB23FFF */
#define SOC_ACPU_SCTRL_BASE_ADDR                                (0xFDB23000)
/* Size: 4K, End at: 0xFDB22FFF */
#define SOC_ACPU_ACTRL_BASE_ADDR                                (0xFDB22000)
/* Size: 24K, End at: 0xFD155FFF */
#define SOC_ACPU_ASP_CODEC_BASE_ADDR                            (0xFD150000)
/* Size: 4K, End at: 0xFD14FFFF */
#define SOC_ACPU_ASP_CFG_BASE_ADDR                              (0xFD14F000)
/* Size: 4K, End at: 0xFD14DFFF */
#define SOC_ACPU_ASP_WD_BASE_ADDR                               (0xFD14D000)
/* Size: 4K, End at: 0xFD14BFFF */
#define SOC_ACPU_ASP_DMAC_BASE_ADDR                             (0xFD14B000)
/* Size: 256K, End at: 0xFD0BFFFF */
#define SOC_ACPU_SECRAM_BASE_ADDR                               (0xFD080000)

#define SOC_ASP_DMA_USB_HDMI_MID	                            0xf
#define SOC_ASP_HIFI_MID	                                    0x11
#endif
