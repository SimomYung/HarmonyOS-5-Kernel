/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller interrupt.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_ctrl_dfx.h"

#include <linux/delay.h>
#include "../core/hi_pcie_core.h"
#include "hi_pcie_ctrl_common.h"
#include "hi_pcie_ctrl_regs.h"
#include "../hi_pcie_external_api.h"

enum {
	PCIE_LTSSM_INIT = 0b000000,
	PCIE_LTSSM_RESET_PIPE_AFIFO = 0b000001,
	PCIE_LTSSM_DETEC_QUIET = 0b000010,
	PCIE_LTSSM_DETECT_ACTIVE = 0b000011,
	PCIE_LTSSM_DETECT_WAIT = 0b000100,
	PCIE_LTSSM_DETECT_PWR_P0 = 0b000101,
	PCIE_LTSSM_POLL_ACTIVE = 0b000110,
	PCIE_LTSSM_POLL_CONFIG = 0b000111,
	PCIE_LTSSM_POLL_COMP = 0b001000,
	PCIE_LTSSM_CONFIG_LW_STR = 0b001001,
	PCIE_LTSSM_CONFIG_LW_ACC = 0b001010,
	PCIE_LTSSM_CONFIG_LN_WAIT = 0b001011,
	PCIE_LTSSM_CONFIG_LN_ACC = 0b001100,
	PCIE_LTSSM_CONFIG_COMPLETE = 0b001101,
	PCIE_LTSSM_CONFIG_IDLE1 = 0b001110,
	PCIE_LTSSM_CONFIG_IDLE2 = 0b001111,
	PCIE_LTSSM_L0 = 0b010000,
	PCIE_LTSSM_RX_L0S = 0b010001,
	PCIE_LTSSM_TX_L0S_ENTRY = 0b010100,
	PCIE_LTSSM_TX_L0S_IDLE = 0b010101,
	PCIE_LTSSM_TX_L0S_FTS = 0b010110,
	PCIE_LTSSM_L1 = 0b010111,
	PCIE_LTSSM_L2 = 0b011000,
	PCIE_LTSSM_TX_BEACON_BEGIN = 0b011001,
	PCIE_LTSSM_TX_BEACON_END = 0b011010,
	PCIE_LTSSM_TX_EIOS_ST = 0b110000,
	PCIE_LTSSM_CHANGE_RATE_GEN1 = 0b110001,
	PCIE_LTSSM_CHANGE_POWER = 0b110010,
	PCIE_LTSSM_HOT_RESET = 0b110011,
	PCIE_LTSSM_DISABLE_PHASE1 = 0b110100,
	PCIE_LTSSM_DISABLE_PHASE2 = 0b110101,
	PCIE_LTSSM_LOOPBACK_ENTRY = 0b111000,
	PCIE_LTSSM_LOOPBACK_ACTIVE = 0b111001,
	PCIE_LTSSM_LOOPBACK_EXIT = 0b111010,
	PCIE_LTSSM_RECOVERY_LOCK = 0b100000,
	PCIE_LTSSM_RECOVERY_CONFIG = 0b100001,
	PCIE_LTSSM_RECOVERY_SPEED = 0b100010,
	PCIE_LTSSM_RECOVERY_IDLE1 = 0b100011,
	PCIE_LTSSM_RECOVERY_IDLE2 = 0b100100,
	PCIE_LTSSM_RECOVERY_EQ_P0 = 0b100101,
	PCIE_LTSSM_RECOVERY_EQ_P1 = 0b100110,
	PCIE_LTSSM_RECOVERY_EQ_P2 = 0b100111,
	PCIE_LTSSM_RECOVERY_EQ_P3 = 0b101000,
};

struct {
	u32 state;
	char *desc;
	u32 official;
} pcie_ltssm[] = {
	{PCIE_LTSSM_INIT, "LTSSM_INIT", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RESET_PIPE_AFIFO, "LTSSM_RESET_PIPE_AFIFO", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_DETEC_QUIET, "LTSSM_DETEC_QUIET", PCIE_LINKSTATE_DETECT},
	{PCIE_LTSSM_DETECT_ACTIVE, "LTSSM_DETECT_ACTIVE", PCIE_LINKSTATE_DETECT},
	{PCIE_LTSSM_DETECT_WAIT, "LTSSM_DETECT_WAIT", PCIE_LINKSTATE_DETECT},
	{PCIE_LTSSM_DETECT_PWR_P0, "LTSSM_DETECT_PWR_P0", PCIE_LINKSTATE_DETECT},
	{PCIE_LTSSM_POLL_ACTIVE, "LTSSM_POLL_ACTIVE", PCIE_LINKSTATE_POLLING},
	{PCIE_LTSSM_POLL_CONFIG, "LTSSM_POLL_CONFIG", PCIE_LINKSTATE_POLLING},
	{PCIE_LTSSM_POLL_COMP, "LTSSM_POLL_COMP", PCIE_LINKSTATE_POLLING},
	{PCIE_LTSSM_CONFIG_LW_STR, "LTSSM_CONFIG_LW_STR", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_CONFIG_LW_ACC, "LTSSM_CONFIG_LW_ACC", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_CONFIG_LN_WAIT, "LTSSM_CONFIG_LN_WAIT", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_CONFIG_LN_ACC, "LTSSM_CONFIG_LN_ACC", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_CONFIG_COMPLETE, "LTSSM_CONFIG_COMPLETE", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_CONFIG_IDLE1, "LTSSM_CONFIG_IDLE1", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_CONFIG_IDLE2, "LTSSM_CONFIG_IDLE2", PCIE_LINKSTATE_CONFIG},
	{PCIE_LTSSM_L0, "LTSSM_L0", PCIE_LINKSTATE_L0},
	{PCIE_LTSSM_RX_L0S, "LTSSM_RX_L0s", PCIE_LINKSTATE_L0S},
	{PCIE_LTSSM_TX_L0S_ENTRY, "LTSSM_TX_L0S_ENTRY", PCIE_LINKSTATE_L0S},
	{PCIE_LTSSM_TX_L0S_IDLE, "LTSSM_TX_L0S_IDLE", PCIE_LINKSTATE_L0S},
	{PCIE_LTSSM_TX_L0S_FTS, "LTSSM_TX_L0S_FTS", PCIE_LINKSTATE_L0S},
	{PCIE_LTSSM_L1, "LTSSM_L1", PCIE_LINKSTATE_L1},
	{PCIE_LTSSM_L2, "LTSSM_L2", PCIE_LINKSTATE_L2},
	{PCIE_LTSSM_TX_BEACON_BEGIN, "LTSSM_TX_BEACON_BEGIN", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_TX_BEACON_END, "LTSSM_TX_BEACON_END", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_TX_EIOS_ST, "LTSSM_TX_EIOS_ST", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_CHANGE_RATE_GEN1, "LTSSM_CHANGE_RATE_GEN1", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_CHANGE_POWER, "LTSSM_CHANGE_POWER", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_HOT_RESET, "LTSSM_HOT_RESET", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_DISABLE_PHASE1, "LTSSM_DISABLE_PHASE1", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_DISABLE_PHASE2, "LTSSM_DISABLE_PHASE2", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_LOOPBACK_ENTRY, "LTSSM_LOOPBACK_ENTRY", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_LOOPBACK_ACTIVE, "LTSSM_LOOPBACK_ACTIVE", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_LOOPBACK_EXIT, "LTSSM_LOOPBACK_EXIT", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_LOCK, "LTSSM_RECOVERY_LOCK", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_CONFIG, "LTSSM_RECOVERY_CONFIG", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_SPEED, "LTSSM_RECOVERY_SPEED", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_IDLE1, "LTSSM_RECOVERY_IDLE1", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_IDLE2, "LTSSM_RECOVERY_IDLE2", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_EQ_P0, "LTSSM_RECOVERY_EQ_P0", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_EQ_P1, "LTSSM_RECOVERY_EQ_P1", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_EQ_P2, "LTSSM_RECOVERY_EQ_P2", PCIE_LINKSTATE_UNKNOWN},
	{PCIE_LTSSM_RECOVERY_EQ_P3, "LTSSM_RECOVERY_EQ_P3", PCIE_LINKSTATE_UNKNOWN},
};

enum {
	PCIE_PME_IDLE = 0x0,
	WAIT_DC_PME_ACK = 0x1,
	WAIT_DC_TL_EN_L2 = 0x2,
	WAIT_DC_DL_EN_L2 = 0x3,
	WAIT_DC_MAC_EN_L2 = 0x4,
	DEVICE_INTO_L2 = 0x5,
	WAIT_DC_TL_EN_PM_L1 = 0x6,
	WAIT_DC_DL_EN_PM_L1 = 0x7,
	WAIT_DC_TL_EN_ASPM_L1 = 0x8,
	WAIT_DC_DL_EN_ASPM_L1 = 0x9,
	WAIT_TL_EN_ASPM_L0S = 0xA,
	WAIT_DL_EN_ASPM_L0S = 0xB,
	WAIT_DC_MAC_EN_L1 = 0xC,
	WAIT_MAC_EN_L0S = 0xD,
	DEVICE_L0S = 0xE,
	DC_DEVICE_IN_L1 = 0xF,
	WAIT_DC_EN_L0 = 0x10,
	WAIT_UC_TL_EN_L2 = 0x11,
	WAIT_UC_DL_EN_L2 = 0x12,
	WAIT_UC_MAC_EN_L2 = 0x13,
	WAIT_UC_TL_EN_PM_L1 = 0x15,
	WAIT_UC_DL_EN_ASPM_L1 = 0x17,
	WAIT_UC_TL_EN_ASPM_L1 = 0x18,
	WAIT_UC_DL_EN_PM_L1 = 0x1A,
	WAIT_UC_MAC_EN_L1 = 0x1C,
	WAIT_UC_PME_EN_L1_NAK = 0x1D,
	WAIT_UC_EN_L0 = 0x1E,
	DEVICE_EN_L1SS = 0x20,
	DEVICE_IN_L1D1 = 0x21,
	DEVICE_EXIT_L1SS = 0x22,
	DEVICE_IN_L1D2_ENTRY = 0x23,
	DEVICE_IN_L1D2_IDLE = 0x24,
	DEVICE_IN_L1D2_EXIT = 0x25,
};

static struct {
	u32 state;
	char *desc;
} pcie_tlpm[] = {
	{PCIE_PME_IDLE, "PME IDLE"},
	{WAIT_DC_PME_ACK, "Wait for DC PME_TO_ACK MSG send out"},
	{WAIT_DC_TL_EN_L2, "Wait for DC TL Enter L2"},
	{WAIT_DC_DL_EN_L2, "Wait for DC DL Enter L2"},
	{WAIT_DC_MAC_EN_L2, "Wait for DC MAC Enter L2"},
	{DEVICE_INTO_L2, "DC enter into L2 state"},
	{WAIT_DC_TL_EN_PM_L1, "DC TL Enter PM L1 state"},
	{WAIT_DC_DL_EN_PM_L1, "DC DL Enter PM L1 state"},
	{WAIT_DC_TL_EN_ASPM_L1, "DC TL Enter ASPM L1 state"},
	{WAIT_DC_DL_EN_ASPM_L1, "DC DL Enter ASPM L1 state"},
	{WAIT_TL_EN_ASPM_L0S, "Wait for TL enter ASPM L0s"},
	{WAIT_DL_EN_ASPM_L0S, "Wait for DL enter ASPM L0s"},
	{WAIT_DC_MAC_EN_L1, "Wait for DC MAC enter L1"},
	{WAIT_MAC_EN_L0S, "Wait for MAC enter L0s"},
	{DEVICE_L0S, "Device in L0s"},
	{DC_DEVICE_IN_L1, "DC Device in L1"},
	{WAIT_DC_EN_L0, "Wait for DC enter L0"},
	{WAIT_UC_TL_EN_L2, "Wait for UC TL enter L2"},
	{WAIT_UC_DL_EN_L2, "Wait for UC DL enter L2"},
	{WAIT_UC_MAC_EN_L2, "Wait for UC MAC enter L2"},
	{WAIT_UC_TL_EN_PM_L1, "Wait UC TL enter PCIPM L1"},
	{WAIT_UC_DL_EN_ASPM_L1, "Wait UC DL enter ASPM L1"},
	{WAIT_UC_TL_EN_ASPM_L1, "Wait UC TL enter ASPM L1"},
	{WAIT_UC_DL_EN_PM_L1, "Wait for UC DL enter PCIPM L1"},
	{WAIT_UC_MAC_EN_L1, "Wait for UC MAC enter L1"},
	{WAIT_UC_PME_EN_L1_NAK, "Wait for UC PME_ENTER_L1_NAK send out"},
	{WAIT_UC_EN_L0, "Wait for UC enter L0"},
	{DEVICE_EN_L1SS, "Device will enter into L1 substate"},
	{DEVICE_IN_L1D1, "Device in L1.1"},
	{DEVICE_EXIT_L1SS, "Device will exit L1 substate"},
	{DEVICE_IN_L1D2_ENTRY, "Device in L1.2.ENTRY"},
	{DEVICE_IN_L1D2_IDLE, "Device in L1.2.IDLE"},
	{DEVICE_IN_L1D2_EXIT, "Device in L1.2.EXIT"},
};

/* for rc dfx */
static char *pcie_ctrl_get_ltssm_desc(u32 ltssm)
{
	int i;
	char *desc = "unknown";
	for (i = 0; i < ARRAY_SIZE(pcie_ltssm); i++) {
		if (pcie_ltssm[i].state == ltssm)
			desc = pcie_ltssm[i].desc;
	}
	return desc;
}

void pcie_ctrl_show_link_state(struct hi_pcie_port *link)
{
	u32 info;
	u32 ltssm;
	u32 cur_speed;
	u32 cur_width;
	char *ltssm_desc = "unknown";
	char *state = "down";

	info = pcie_ctrl_mac_readl(link->priv, MAC_REG_LINK_INFO);
	ltssm = info >> MAC_LTSSM_ST_SHIFT;
	ltssm &= MAC_LTSSM_ST_MASK;
	ltssm_desc = pcie_ctrl_get_ltssm_desc(ltssm);

	if (info & MAC_LINK_UP)
		state = "up";

	cur_speed = info >> MAC_CUR_LINK_SPEED_SHIFT;
	cur_speed &= MAC_CUR_LINK_SPEED_MASK;

	cur_width = info >> MAC_CUR_LINK_WIDTH_SHIFT;
	cur_width &= MAC_CUR_LINK_WIDTH_MASK;

	hi_pcie_pr_e("link-%u is %s(%s), cur speed %u, cur link width %u",
		link->port_id, state, ltssm_desc, cur_speed, cur_width);
}

static void pcie_ctrl_reset_ltssm_tracer(struct hi_pcie_ctrl_port *link)
{
	u32 val = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACER_INPUT);
	val |= MAC_LTSSM_TRACER_RECAP;
	pcie_ctrl_mac_writel(link, val, MAC_REG_LTSSM_TRACER_INPUT);
}

void pcie_ctrl_dump_ltssm_tracer(struct hi_pcie_ctrl_port *link, bool reset)
{
	u32 addr;
	u32 val;
	u32 val2;
	u32 ltssm;
	u32 last_addr;
	u32 idx;

	hi_pcie_pr_e("	ltssm[ii]:  63:48  47:32    31   30   29   28   27   26   25   24   23   22  21:20  19:12  11:10 9:6   5:0   LTSSM");
	last_addr = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACER_LAST_ADDR);
	if ((last_addr & MAC_LTSSM_TRACER_LAST_ADDR_ROLLBACK) != 0)
		last_addr = 64;
	else
		last_addr &= MAC_LTSSM_TRACER_LAST_WADDR_MASK;
	for (idx = 1; idx <= last_addr; idx++) {
		addr = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACER_INPUT);
		addr |= MAC_LTSSM_TRACER_CAP_MODE;
		pcie_ctrl_mac_writel(link, addr, MAC_REG_LTSSM_TRACER_INPUT);

		addr = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACERADDR);
		addr &= ~MAC_LTSSM_TRACER_RADDR_MASK;
		addr |= ((idx - 1) & MAC_LTSSM_TRACER_RADDR_MASK);
		pcie_ctrl_mac_writel(link, addr, MAC_REG_LTSSM_TRACERADDR);

		val = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACER_OUTPUT_1);
		ltssm = (val & MAC_LTSSM_TRACER_OUTPUT_1_STATE_MASK);

		val2 = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACER_OUTPUT_2);

		hi_pcie_pr_e("	ltssm[%02u]: 0x%04x 0x%04x   0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x   0x%02x   0x%x   0x%x   0x%02x  %s", idx,
			(val2 >> 16) & 0xffff,
			(val2 & 0xffff),
			(val >> 31) & 0x1,
			(val >> 30) & 0x1,
			(val >> 29) & 0x1,
			(val >> 28) & 0x1,
			(val >> 27) & 0x1,
			(val >> 26) & 0x1,
			(val >> 25) & 0x1,
			(val >> 24) & 0x1,
			(val >> 23) & 0x1,
			(val >> 22) & 0x1,
			(val >> 20) & 0x3,
			(val >> 12) & 0xFF,
			(((val >> 20) & 0x3) << 2) | ((val >> 10) & 0x3),
			(val >> 6) & 0xF,
			(val & 0x3F),
			pcie_ctrl_get_ltssm_desc(ltssm));
	}

	last_addr = pcie_ctrl_mac_readl(link, MAC_REG_LTSSM_TRACER_LAST_ADDR);
	last_addr &= MAC_LTSSM_TRACER_OUTPUT_1_STATE_MASK;
	hi_pcie_pr_e("last addr is:(0x%x)", last_addr);
	if (reset)
		pcie_ctrl_reset_ltssm_tracer(link);
}

static char *pcie_ctrl_get_tlpm_desc(u32 tlpm)
{
	int i;
	char *desc = "unknown";
	for (i = 0; i < ARRAY_SIZE(pcie_tlpm); i++) {
		if (pcie_tlpm[i].state == tlpm) {
			desc = pcie_tlpm[i].desc;
			break;
		}
	}
	return desc;
}

static void pcie_ctrl_dump_tlpm_tracer(struct hi_pcie_ctrl_port *link)
{
	u32 addr;
	u32 val;
	u32 last_addr;
	u32 idx;
	u32 tlpm;

	last_addr = pcie_ctrl_tl_readl(link, TL_PM_STATUS_FIFO_CONTROL);
	if ((last_addr & TL_DFX_PM_TRACER_CAP_MODEBIT) != 0) {
		last_addr = 32;
	} else {
		last_addr &= TL_DFX_PM_TRACER_LAST_WADDR_MASK;
		last_addr = last_addr >> 8;
	}
	for (idx = 1; idx <= last_addr; idx++) {
		addr = pcie_ctrl_tl_readl(link, TL_PM_STATUS_FIFO_CONTROL);
		addr |= BIT(1);

		/* set dfx_pm_tracer_raddr */
		addr &= ~TL_DFX_PM_TRACER_RADDR_MASK;
		addr |= (((idx - 1) << 0x3) & TL_DFX_PM_TRACER_RADDR_MASK);
		pcie_ctrl_tl_writel(link, addr, TL_PM_STATUS_FIFO_CONTROL);

		/* please set the dfx_pm_tracer_raddr before set this bit valid */
		addr = pcie_ctrl_tl_readl(link, TL_PM_STATUS_FIFO_CONTROL);
		addr |= BIT(2);
		pcie_ctrl_tl_writel(link, addr, TL_PM_STATUS_FIFO_CONTROL);
		val = pcie_ctrl_tl_readl(link, TL_PM_STATUS);
		tlpm = val & TL_PM_STATUS_MSK;
		hi_pcie_pr_e("tlpm:%s(%x)", pcie_ctrl_get_tlpm_desc(tlpm), val);
	}

	last_addr = pcie_ctrl_tl_readl(link, TL_PM_STATUS_FIFO_CONTROL);
	last_addr &= TL_PM_TRACER_WADDR_MASK;
	last_addr = last_addr >> 8;
	hi_pcie_pr_e("last addr is:(0x%x)", last_addr);
}

static void pcie_ctrl_dump_dl_info(struct hi_pcie_port *port)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} dl_info[] = {
		{ DL_INT_STATUS, "original interrupts status" },
		{ DFX_RX_NAK_COUNT, "rx reply nak cnt number" },
		{ ECC_ERR_CNT_1BIT, "1bit ecc err cnt" },
		{ ECC_ERR_CNT_2BIT, "2bit ecc err cnt" },
		{ DFX_LCRC_ERR_NUM, "the lcrc error number" },
		{ DFX_DCRC_ERR_NUM, "the crc error number" },
		{ DFX_DLLP_RX_COUNT_NUM, "cnt rx dllp number" },
		{ DFX_RX_BAD_DLLP_TYPE, "bad dllp type indicate" },
		{ DFX_FSM_STATE, "dfx signal for fsm state" },
	};

	hi_pcie_pr_e("PCIe dump dl info");

	/* dl info status dump */
	for (i = 0; i < ARRAY_SIZE(dl_info); i++) {
		val = pcie_ctrl_dl_readl(port->priv, dl_info[i].reg);
		hi_pcie_pr_e("DL %s reg [0x%x] is [0x%x]",
			     dl_info[i].desc, dl_info[i].reg, val);
	}
}

static void pcie_ctrl_dump_mac_info(struct hi_pcie_port *port)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} mac_info[] = {
		{ MAC_REG_MAC_INT_STATUS, "interrupt status" },
		{ MAC_REG_TEST_COUNTER, "phy lane err counter" },
		{ MAC_REG_DEBUG_PIPE2, "mac pipe lane 0-7 powerdown status" },
		{ MAC_REG_DEBUG_PIPE3, "phy status chk" },
		{ MAC_REG_DEBUG_PIPE7, "mac pipe lane 8-15 powerdown status" },
		{ MAC_REG_SYMBOL_UNLOCK_CNT, "rx symbol unlock counter" },
		{ MAC_PCS_RX_ERR_CNT_REG, "pcs rx err cnt" },
		{ MAC_FRAMING_ERR_CNT, "rx framing err counter val" },
		{ MAC_TRACE_2BIT_ECC_CNT, "2bit ecc err cnt" },
		{ MAC_TRACE_1BIT_ECC_CNT, "1bit ecc err cnt" },
		{ DFX_APB_LANE_ERROR_STATUS, "pcs layer receive error status" },
		{ DFX_APB_LANE_ERROR_STATUS1, "lane err status" },
		{ MAC_REG_RPT_MBUS_ERR_CNT, "the err cnt of mbus" },
		{ MAC_LEAVE_L0_INFO, "mac leave l0 info" },
		{ MAC_REG_LINK_INFO, "mac link information" },
		{ MAC_TIMEOUT_ERR, "mac timeout err" },
	};

	static struct {
		u32 reg;
		u32 val;
		char *desc;
	} mac_clr_info[] = {
		{ MAC_REG_TEST_COUNTER, 0xFFFF, "phy lane err counter" },
		{ MAC_REG_SYMBOL_UNLOCK_CNT, 0xFFFF, "rx symbol unlock counter" },
		{ MAC_PCS_RX_ERR_CNT_REG, 0xFFFF, "pcs rx err cnt" },
		{ MAC_FRAMING_ERR_CNT, 0xFFFF, "rx framing err counter val" },
	};

	hi_pcie_pr_e("PCIe dump MAC info");

	/* mac info status dump */
	for (i = 0; i < ARRAY_SIZE(mac_info); i++) {
		val = pcie_ctrl_mac_readl(port->priv, mac_info[i].reg);
		hi_pcie_pr_e("MAC %s reg [0x%x] is [0x%x]",
			     mac_info[i].desc, mac_info[i].reg, val);
	}
	/* mac error count clr */
	for (i = 0; i < ARRAY_SIZE(mac_clr_info); i++) {
		pcie_ctrl_mac_writel(port->priv, mac_clr_info[i].val, mac_clr_info[i].reg);
	}
}

static void pcie_ctrl_dump_tl_info(struct hi_pcie_port *port)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} tl_info[] = {
		{ TL_INT_STATUS0, "interrupts status0" },
		{ TL_INT_STATUS1, "interrupts status1" },
		{ TL_INT_STATUS2, "interrupts status2" },
		{ TL_RX_VC0_POST_CREDIT_LEFT, "rx post credit" },
		{ TL_RX_VC0_NONPOST_CREDIT_LEFT, "rx non-port credit" },
		{ TL_RX_VC0_CPL_CREDIT_LEFT, "rx cpl credit" },
		{ TL_TX_VC0_P_FC_LEFT, "tx post credit" },
		{ TL_TX_VC0_NP_FC_LEFT, "tx non-post credit" },
		{ TL_TX_VC0_CPL_FC_LEFT, "tx cpl credit" },
		{ TX_CRD_CTRL, "tx crd ctrl info" },
		{ TL_PORT_IDLE_CTRL_STATUS, "tl port idle ctrl status" },
		{ TL_RX_TOTAL_TR_CNT, "rx total sending tlps counter" },
		{ TL_RX_DROP_CNT, "rx drop tlps counter" },
		{ TL_RX_POST_CNT, "rx post tlp counter" },
		{ TL_RX_NONPOST_CNT, "rx non-post tlp counter" },
		{ TL_RX_CPL_CNT, "rx completion tlp counter" },
	};

	hi_pcie_pr_e("PCIe dump TL info");

	/* tl info status dump */
	for (i = 0; i < ARRAY_SIZE(tl_info); i++) {
		val = pcie_ctrl_tl_readl(port->priv, tl_info[i].reg);
		hi_pcie_pr_e("TL %s reg [0x%x] is [0x%x]",
			     tl_info[i].desc, tl_info[i].reg, val);
	}
}

static void pcie_ctrl_dump_local_info(struct hi_pcie_port *port)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} rc_cfg[] = {
		{ LINK_CTRL_STATUS, "link ctrl status register" },
		{ LINK_CTRL_STATUS2, "link ctrl status2 register" },
		{ DEVICE_CTRL_STATUS, "device ctrl register" },
		{ L1PM_CONTROL1, "l1 pm substate control1 register" },
		{ L1PM_CONTROL2, "l1 pm substate control2 register" },
		{ AER_UNCR_STATUS, "uncorrectable error register"},
		{ AER_COR_STATUS, "correctable error register"},
		{ FIRST_HEADER_LOG, "First DW header log"},
		{ SECOND_HEADER_LOG, "Second DW header log"},
		{ THIRD_HEADER_LOG, "Third DW header log"},
		{ FOUR_HEADER_LOG, "Forth DW header log"},
	};

	hi_pcie_pr_e("PCIe dump RC Cfg info");

	/* RC cfg info dump */
	for (i = 0; i < ARRAY_SIZE(rc_cfg); i++) {
		val = pcie_ctrl_cfg_readl(port->priv, rc_cfg[i].reg);
		hi_pcie_pr_e("RC Cfg %s is 0x%x", rc_cfg[i].desc, val);
	}
}

static void pcie_ctrl_dump_plat_info(struct hi_pcie *pcie)
{
	if (pcie->plat_ops && pcie->plat_ops->dump_plat_info) {
		(void)pcie->plat_ops->dump_plat_info(pcie);
	}
}

void pcie_ctrl_dump_port_info(struct hi_pcie_port *port, u32 type)
{
	if (atomic_read(&(port->pcie->is_power_on)) == 0) {
		hi_pcie_pr_e("pcie %u port %u is power down",
			      port->pcie->ctrl_id, port->id);
		return;
	}

	pcie_ctrl_dump_dl_info(port);

	pcie_ctrl_dump_mac_info(port);

	pcie_ctrl_dump_tl_info(port);

	pcie_ctrl_dump_local_info(port);

	pcie_ctrl_dump_plat_info(port->pcie);
}
/* end */

static u32 pcie_ctrl_get_official_state(u32 ltssm)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(pcie_ltssm); i++) {
		if (pcie_ltssm[i].state == ltssm)
			return pcie_ltssm[i].official;
	}
	return PCIE_LINKSTATE_UNKNOWN;
}

static void pcie_ctrl_setup_mac_loopback(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	u32 lanes;

	switch (port->port->num_lanes) {
	case 0x4:
		lanes = 0xf;
		break;
	case 0x2:
		lanes = 0x3;
		break;
	case 0x1:
		lanes = 0x1;
		break;
	default:
		lanes = 0x0;
		break;
	}

	val = pcie_ctrl_core_gbl_readl(port->ctrl, LOCAL_LOOPBACK_MODE);
	if (port->ctrl->pcie->loopback_type == PCIE_LOOPBACK_LOCAL)
		val |= lanes;
	else
		val &= (~lanes);
	pcie_ctrl_core_gbl_writel(port->ctrl, val, LOCAL_LOOPBACK_MODE);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ENTER_LOOPBACK);
	val |= (MAC_ENTER_LOOPBACK | MAC_LOOPBACK_CHECK_EN);
	pcie_ctrl_mac_writel(port, val, MAC_REG_ENTER_LOOPBACK);
}

static void pcie_ctrl_setup_ap_loopback(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	u32 lanes;

	switch (port->port->num_lanes) {
	case 0x4:
		lanes = 0xf;
		break;
	case 0x2:
		lanes = 0x3;
		break;
	case 0x1:
		lanes = 0x1;
		break;
	default:
		lanes = 0x0;
		break;
	}
	val = pcie_ctrl_core_gbl_readl(port->ctrl, LOCAL_LOOPBACK_MODE);
	val |= lanes;
	pcie_ctrl_core_gbl_writel(port->ctrl, val, LOCAL_LOOPBACK_MODE);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ENTER_LOOPBACK);
	val |= MAC_DS_L0_LOOPBACK_EN;
	pcie_ctrl_mac_writel(port, val, MAC_REG_ENTER_LOOPBACK);
	val = pcie_ctrl_mac_readl(port, MAC_REG_EQ_DISABLE);
	val |= MAC_EQUALIZATION_DISABLE;
	pcie_ctrl_mac_writel(port, val, MAC_REG_EQ_DISABLE);
}

static void pcie_ctrl_setup_none_loopback(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_mac_readl(port, MAC_REG_ENTER_LOOPBACK);
	val &= ~(MAC_ENTER_LOOPBACK | MAC_DS_L0_LOOPBACK_EN);
	pcie_ctrl_mac_writel(port, val, MAC_REG_ENTER_LOOPBACK);
	val = pcie_ctrl_mac_readl(port, MAC_REG_EQ_DISABLE);
	val &= ~MAC_EQUALIZATION_DISABLE;
	pcie_ctrl_mac_writel(port, val, MAC_REG_EQ_DISABLE);
}

static void pcie_ctrl_setup_loopback(struct hi_pcie_ctrl_port *port)
{
	u32 type = port->ctrl->pcie->loopback_type;
	if (type == PCIE_LOOPBACK_LOCAL || type == PCIE_LOOPBACK_REMOTE)
		pcie_ctrl_setup_mac_loopback(port);
	else if (type == PCIE_LOOPBACK_AP)
		pcie_ctrl_setup_ap_loopback(port);
	else if (type == PCIE_LOOPBACK_NONE)
		pcie_ctrl_setup_none_loopback(port);
}

u32 pcie_ctrl_get_link_state(struct hi_pcie_port *port)
{
	u32 ltssm;
	u32 state;
	ltssm = pcie_ctrl_mac_readl(port->priv, MAC_REG_LINK_INFO);
	ltssm >>= MAC_LTSSM_ST_SHIFT;
	ltssm &= MAC_LTSSM_ST_MASK;
	state = pcie_ctrl_get_official_state(ltssm);
	if (state == PCIE_LINKSTATE_L1) {
		state = pcie_ctrl_tl_readl(port->priv, TL_PM_STATE);
		state = state & TL_PM_STATE_MSK;
		if (state != PCIE_LINKSTATE_L1D1 && state != PCIE_LINKSTATE_L1D2)
			return PCIE_LINKSTATE_L1;
	}
	return state;
}

static u32 pcie_ctrl_get_link_speed(struct hi_pcie_port *port)
{
	u32 speed;
	speed = pcie_ctrl_mac_readl(port->priv, MAC_REG_LINK_INFO);
	speed >>= MAC_CUR_LINK_SPEED_SHIFT;
	speed &= MAC_CUR_LINK_SPEED_MASK;
	return speed;
}

static u32 pcie_ctrl_get_link_lane(struct hi_pcie_port *port)
{
	u32 lane;
	lane = pcie_ctrl_mac_readl(port->priv, MAC_REG_LINK_INFO);
	lane >>= MAC_CUR_LINK_WIDTH_SHIFT;
	lane &= MAC_CUR_LINK_WIDTH_MASK;
	return lane;
}

static int pcie_ctrl_check_loopback_st(struct hi_pcie_ctrl_port *port, u32 type)
{
	u32 timeout = 100;
	u32 val;

	while (timeout > 0) {
		val = pcie_ctrl_mac_readl(port, MAC_REG_ENTER_LOOPBACK);
		val >>= MAC_LOOPBACK_MASTER_CHECK_ST_SHIFT;
		val &= MAC_LOOPBACK_MASTER_CHECK_ST_MASK;
		if (0b10 == val) {
			hi_pcie_pr_e("port %u enter loopback success", port->port->port_id);
			return 0;
		}
		mdelay(1);
		timeout--;
	}

	hi_pcie_pr_e("port %u enter loopback failed", port->port->port_id);

	return -1;
}

int pcie_ctrl_enter_loopback(struct hi_pcie_port *port, u32 type)
{
	port->pcie->loopback_type = type;
	if (type == PCIE_LOOPBACK_AP)
		pcie_ctrl_disable_port_ap(port->priv);
	pcie_ctrl_disable_port(port->priv);
	pcie_ctrl_setup_loopback(port->priv);
	pcie_ctrl_enable_port(port->priv);
	if (type == PCIE_LOOPBACK_AP)
		pcie_ctrl_enable_port_ap(port->priv);
	if ((type == PCIE_LOOPBACK_LOCAL) || (type == PCIE_LOOPBACK_REMOTE))
		return pcie_ctrl_check_loopback_st(port->priv, type);
	return 0;
}

int pcie_ctrl_exit_loopback(struct hi_pcie_port *port)
{
	u32 type = port->pcie->loopback_type;
	port->pcie->loopback_type = PCIE_LOOPBACK_NONE;
	if (type == PCIE_LOOPBACK_AP)
		pcie_ctrl_disable_port_ap(port->priv);
	pcie_ctrl_disable_port(port->priv);
	pcie_ctrl_setup_loopback(port->priv);
	pcie_ctrl_enable_port(port->priv);
	if (type == PCIE_LOOPBACK_AP)
		pcie_ctrl_enable_port_ap(port->priv);
	return 0;
}

static void pcie_ctrl_dump_ap_info(struct hi_pcie *pcie, u32 type)
{
	int i;
	u32 val = 0;
	static struct {
		u32 reg;
		char *desc;
		u32 type;
	} ap_info[] = {
		{ DFX_IOB_RX_CNT_RX_REQ , "counter for RX received request from TL", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_LOC_REQ , "counter for local request from inner module", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_SEND_AM , "counter of the request sent to AXI Master", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_SEND_LOC , "counter of the request not sent to AXI Master", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_DMA_CPL, "counter of cpl belong to dma", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_RESP_RX, "counter of the response to TL", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_RESP_LOC, "counter of the response to inner module", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_RESP_RECV, "counter of the response from several destination", AP_IOB_RX_REG },
		{ DFX_IOB_RX_CNT_CTRL, "conter control for IOB_RX", AP_IOB_RX_REG },
		{ IOB_RX_DAT_BUF_ECC_ERR_CNT, "rx data buffer ECC error counter", AP_IOB_RX_REG },
		{ IOB_TX_INT_STATUS0, "tx interrupt status0", AP_IOB_TX_REG },
		{ IOB_TX_INT_STATUS1, "tx interrupt status1", AP_IOB_TX_REG },
		{ IOB_TX_INT_STATUS2, "tx interrupt status2", AP_IOB_TX_REG },
		{ IOB_TX_INT_STATUS3, "tx interrupt status3", AP_IOB_TX_REG },
		{ IOB_TX_INT_STATUS4, "tx interrupt status4", AP_IOB_TX_REG },
		{ DFX_IOB_RX_AML_IDLE_STATE, "Idle state of the IOB_RX AML", AP_IOB_RX_REG },
		{ DFX_IOB_RX_AML_WR_CNT_0, "counter0 for iob rx aw cnt", AP_IOB_RX_REG },
		{ DFX_IOB_RX_AML_WR_CNT_1, "counter1 for iob rx w cnt", AP_IOB_RX_REG },
		{ DFX_IOB_RX_AML_RD_CNT, "counter for iob rx aw cnt", AP_IOB_RX_REG },
		{ IOB_TX_INT_STATUS0, "iob tx intr status0", AP_IOB_TX_REG },
		{ IOB_TX_INT_STATUS1, "iob tx intr status1", AP_IOB_TX_REG },
		{ IOB_TX_INT_STATUS2, "iob tx intr status2", AP_IOB_TX_REG },
		{ DFX_IOB_TX_IDLE, " iob_tx_axis wr counter", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_WR_CNT0, "iob_tx_axis wr counter0", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_WR_CNT1, "iob_tx_axis wr counter1", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_WR_CNT2, "iob_tx_axis wr counter2", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_WR_CNT3, "iob_tx_axis wr counter3", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_WR_CNT4, "iob_tx_axis wr counter4", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_WR_CNT5, "iob_tx_axis wr counter5", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_RD_CNT0, "iob_tx_axis rd counter0", AP_IOB_TX_REG },
		{ DFX_IOB_TX_AXIS_RD_CNT1, "iob_tx_axis rd counter1", AP_IOB_TX_REG },
		{ DFX_IOB_TX_P_CNT0, "statistic DFX0 of Post TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_P_CNT1, "statistic DFX1 of Post TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_NP_CNT0, "statistic DFX0 of NonPost TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_NP_CNT1, "statistic DFX1 of NonPost TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_NP_CNT2, "statistic DFX2 of NonPost TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_NP_CNT3, "statistic DFX3 of NonPost TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_NP_CNT4, "statistic DFX4 of NonPost TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_NP_CNT5, "statistic DFX5 of NonPost TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_CPL_CNT0, "statistic DFX0 of Completion TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_CPL_CNT1, "statistic DFX1 of Completion TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_CPL_CNT2, "statistic DFX2 of Completion TLP", AP_IOB_TX_REG },
		{ DFX_IOB_TX_STATUS2, "The status2 of TQM", AP_IOB_TX_REG },
		{ DFX_IOB_TX_STATUS3, "The status3 of TQM", AP_IOB_TX_REG },
		{ DFX_IOB_TX_STATUS4, "The status4 of TQM", AP_IOB_TX_REG },
	};

	hi_pcie_pr_e("PCIe dump AP info");

	/* Ap info dump */
	for (i = 0; i < ARRAY_SIZE(ap_info); i++) {
		switch (ap_info[i].type) {
		case AP_IOB_RX_REG:
			val = pcie_ctrl_ap_iobrx_readl(pcie->ctrl, ap_info[i].reg);
			break;

		case AP_IOB_TX_REG:
			val = pcie_ctrl_ap_iobtx_readl(pcie->ctrl, ap_info[i].reg);
			break;

		default:
			break;
		}
		hi_pcie_pr_e("AP %s reg [0x%x] is [0x%x]",
			     ap_info[i].desc, ap_info[i].reg, val);
	}
}

void pcie_ctrl_dump_ctrl_info(struct hi_pcie *pcie, u32 type)
{
	pcie_ctrl_dump_ap_info(pcie, type);
}

static void pcie_ctrl_dump_ltssm_trace(struct hi_pcie_port *port, u32 reset)
{
	pcie_ctrl_dump_ltssm_tracer(port->priv, (bool)reset);
}

static void pcie_ctrl_dump_tlpm_trace(struct hi_pcie_port *port)
{
	pcie_ctrl_dump_tlpm_tracer(port->priv);
}

static int pcie_ctrl_enter_compliance(struct hi_pcie_port *port, u32 speed, u32 deemp)
{
	u32 ret;
	u32 val;

	/* detect ep: the interface is used after powerup soc && controller */
	port->pcie->phy_dfx_ops->phy_tx_detect_rx(&(port->pcie->phy));

	/* close port en */
	pcie_ctrl_disable_port(port->priv);

	/* set link spped */
	val = pcie_ctrl_cfg_readl(port->priv, LINK_CTRL_STATUS2);
	val = val & 0xFFFFFFF0;
	val |= speed;
	pcie_ctrl_cfg_writel(port->priv, val, LINK_CTRL_STATUS2);

	/* cfg tx fee link ctrl2[15:12] */
	val = pcie_ctrl_cfg_readl(port->priv, LINK_CTRL_STATUS2);
	val = val & 0xFFFF0FFF;
	val = val | (deemp << 12);
	pcie_ctrl_cfg_writel(port->priv, val, LINK_CTRL_STATUS2);

	/* compliance enable */
	val = pcie_ctrl_cfg_readl(port->priv, LINK_CTRL_STATUS2);
	val |= BIT(4);
	pcie_ctrl_cfg_writel(port->priv, val, LINK_CTRL_STATUS2);

	/* open port en */
	pcie_ctrl_enable_port(port->priv);
	mdelay(50);

	/* chk compliance status */
	ret = pcie_ctrl_mac_readl(port->priv, MAC_REG_LINK_INFO);
	if (ret & BIT(27)) {
		pcie_ctrl_show_link_state(port);
		return 0;
	}

	pcie_ctrl_show_link_state(port);
	hi_pcie_pr_e("enter compliance fail, status = 0x%x", ret);
	return -EINVAL;
}

static int pcie_ctrl_exit_compliance(struct hi_pcie_port *port)
{
	u32 val;

	/* compliance disable */
	val = pcie_ctrl_cfg_readl(port->priv, LINK_CTRL_STATUS2);
	val &= ~BIT(4);
	pcie_ctrl_cfg_writel(port->priv, val, LINK_CTRL_STATUS2);

	/* close port en */
	pcie_ctrl_disable_port(port->priv);

	/* open port en */
	pcie_ctrl_enable_port(port->priv);

	/* retrain link */
	val = pcie_ctrl_cfg_readl(port->priv, LINK_CTRL_STATUS);
	val = val | BIT(5);
	pcie_ctrl_cfg_writel(port->priv, val, LINK_CTRL_STATUS);

	mdelay(50);
	pcie_ctrl_show_link_state(port);
	return 0;
}

static struct hi_pcie_dfx_ops dfx_ops = {
	.show_link_state	= pcie_ctrl_show_link_state,
	.get_link_state		= pcie_ctrl_get_link_state,
	.get_link_speed		= pcie_ctrl_get_link_speed,
	.get_link_lane		= pcie_ctrl_get_link_lane,
	.enter_loopback		= pcie_ctrl_enter_loopback,
	.exit_loopback		= pcie_ctrl_exit_loopback,
	.dump_port_info		= pcie_ctrl_dump_port_info,
	.dump_ctrl_info		= pcie_ctrl_dump_ctrl_info,
	.dump_ltssm_trace	= pcie_ctrl_dump_ltssm_trace,
	.dump_tlpm_trace	= pcie_ctrl_dump_tlpm_trace,
	.enter_compliance	= pcie_ctrl_enter_compliance,
	.exit_compliance	= pcie_ctrl_exit_compliance,
};

void hi_pcie_dfx_register(struct hi_pcie *pcie)
{
	pcie->dfx_ops = &dfx_ops;
}
