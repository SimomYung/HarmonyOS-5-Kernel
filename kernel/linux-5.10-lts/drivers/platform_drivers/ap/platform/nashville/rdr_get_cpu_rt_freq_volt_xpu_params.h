/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: get cpu rt freq and volt info from rdr
 * Create: 2025-2-118
 */
#ifndef __RDR_CPU_RT_FV_XPU_PARAMS_H__
#define __RDR_CPU_RT_FV_XPU_PARAMS_H__
 
#if defined(CONFIG_ACPU_DIRECT_ACCESS_CXPU) /* read from acpu */
#define CXPU_SRAM_SOC_ACPU_CPU_LPCTRL_AHB_BASE_ADDR                       (0xFEE00000)
#define CXPU_SRAM_BASE		CXPU_SRAM_SOC_ACPU_CPU_LPCTRL_AHB_BASE_ADDR
#elif defined(CONFIG_LPMCU_DIRECT_ACCESS_CXPU) /* read from lpmcu */
#define CXPU_SRAM_SOC_LPMCU_CPU_LPCTRL_AHB_BASE_ADDR                      (0xFEE00000)
#define CXPU_SRAM_BASE		CXPU_SRAM_SOC_LPMCU_CPU_LPCTRL_AHB_BASE_ADDR
#else /* read from cpu_lpctrl(xpu cpu itself) or ILLEGAL */
#define CXPU_SRAM_BASE		0
#endif
#define CXPU_SRAM_SIZE		0x10000U
 
/* LPRAM MEMORY MAP */
#define CXPU_PROG_HEAD_BASE	(CXPU_SRAM_BASE)
#define CXPU_PROG_HEAD_SIZE	0x40
 
#define CXPU_PROFILE_BASE	(CXPU_PROG_HEAD_BASE + CXPU_PROG_HEAD_SIZE)
#define CXPU_PROFILE_SIZE	(0x3D0 * 3) /* single profile size decrease from 0x440 to 0x3D0 */
 
#define CXPU_SEC_DATA_BASE	(CXPU_PROFILE_BASE + CXPU_PROFILE_SIZE)
#define CXPU_SEC_DATA_RSV_SIZE	0x0
#define CXPU_SEC_DATA_SIZE	(0x2660 - CXPU_SEC_DATA_RSV_SIZE) /* sec sdata region size decrease from 0x2D00 to 0x2660, left 400 bytes */
 
#endif /* __RDR_CPU_RT_FV_XPU_PARAMS_H__ */