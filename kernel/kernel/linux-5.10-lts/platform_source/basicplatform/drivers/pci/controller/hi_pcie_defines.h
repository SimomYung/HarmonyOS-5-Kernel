/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe type defines.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_DEFINES_H
#define HI_PCIE_DEFINES_H

#include <linux/printk.h>

enum {
	PCIE_MODE_RC = 0,
	PCIE_MODE_EP,
};

enum {
	RC_ID0 = 0,
	RC_ID1,
	RC_ID2,
	RC_ID3,
	RC_ID4,
	RC_ID_MAX,
};

enum {
	PCIE_POWER_OFF = 0,
	PCIE_POWER_ON,
	PCIE_POWER_OFF_NO_PME,
	PCIE_POWER_OFF_NO_LINK,
	PCIE_POWER_SUSPEND,
	PCIE_POWER_RESUME,
	PCIE_POWER_INVALID,
};

enum {
	PCIE_LINK_SPEED_LIMIT = 0,
	PCIE_LINK_SPEED_GEN1,
	PCIE_LINK_SPEED_GEN2,
	PCIE_LINK_SPEED_GEN3,
	PCIE_LINK_SPEED_GEN4,
	PCIE_LINK_SPEED_GEN5,
	PCIE_LINK_SPEED_BOTTEOM,
};

enum {
	PCIE_ASPM_CLOSE = 0,
	PCIE_ASPM_L0S,
	PCIE_ASPM_L1,
	PCIE_ASPM_L0S_L1,
	PCIE_ASPM_STATE_MAX,
};

enum {
	PCIE_L1SS_CLOSE		= 0x0,
	PCIE_L1SS_PM_1_2	= 0x1,
	PCIE_L1SS_PM_1_1	= 0x2,
	PCIE_L1SS_PM_ALL	= 0x3,
	PCIE_L1SS_ASPM_1_2	= 0x4,
	PCIE_L1SS_ASPM_1_1	= 0x8,
	PCIE_L1SS_ASPM_ALL	= 0xC,
	PCIE_L1SS_PM_ASPM_ALL	= 0xF,
};

enum {
	PCIE_LINKSTATE_UNKNOWN = 0,
	PCIE_LINKSTATE_DETECT,
	PCIE_LINKSTATE_POLLING,
	PCIE_LINKSTATE_CONFIG,
	PCIE_LINKSTATE_L0,
	PCIE_LINKSTATE_L0S,
	PCIE_LINKSTATE_L1,
	PCIE_LINKSTATE_L2,
	PCIE_LINKSTATE_L1D1 = 0x21,
	PCIE_LINKSTATE_L1D2 = 0x24,
};

enum {
	PCIE_IRQ_LEGACY = 0,
	PCIE_IRQ_MSI,
	PCIE_IRQ_MSIX,
};

enum {
	PCIE_SIZE_BYTE	= 1,
	PCIE_SIZE_WORD	= 2,
	PCIE_SIZE_DWORD	= 4,
};

#ifndef unused
#define unused(x) (void)(x)
#endif

#define PCIE_SUCCESS	0
#define PCIE_ERR	0xFFFFFFFF

#define PCIE_MAX_PORT_NUM 32
#define PCIE_CTRL_ID_SHIFT 8
#define PCIE_DEFAULT_DEVICE_ID 0x36A0
#define PCIE_DEFAULT_VENDOR_ID 0x19E5
#define PCIE_DEFAULT_EP_ID 0xA120FFFF
#define PCIE_REF2PERST1_DELAY_MIN	0
#define PCIE_REF2PERST2_DELAY_MAX	0
#define PCIE_PERST2ACCESS1_DELAY_MIN	0
#define PCIE_PERST2ACCESS2_DELAY_MAX	0
#define PCI_DEVCTL_SUP_PAYLOAD_128B            128
#define PCI_DEVCTL_SUP_PAYLOAD_512B            512
#define PCI_EXP_DEVCAP_PAYLOAD                 0x00000007
#define PCI_EXP_DEVCTL_PAYLOAD 0x00E0
#define HIPCIE_LTSSM_L1_STATE				0x17
#define MPS_CTRL_STATUS			0x48
#define EP_BAR0_OFFSET (4 * 1024)
#define EP_BAR0_SIZE (8 * 1024)

#ifndef HIPCIE_LOG_LEVEL
#define HIPCIE_LOG_LEVEL 0
#endif

#if HIPCIE_LOG_LEVEL
#define hi_pcie_pr_d(fmt, args ...) pr_err("[hi_pcie][DBG]:" fmt "\n", ##args)
#define hi_pcie_pr_i(fmt, args ...) pr_err("[hi_pcie][INF]:" fmt "\n", ##args)
#else
#define hi_pcie_pr_d(fmt, args ...)
#define hi_pcie_pr_i(fmt, args ...) pr_info("[hi_pcie][INF]:" fmt "\n", ##args)
#endif

#define hi_pcie_pr_e(fmt, args ...) pr_err("[hi_pcie][ERR]:" fmt "\n", ##args)
#endif
