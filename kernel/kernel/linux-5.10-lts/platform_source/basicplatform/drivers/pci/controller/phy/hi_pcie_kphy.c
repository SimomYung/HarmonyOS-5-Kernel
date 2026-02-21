/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: kphy implementation.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_kphy.h"

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <asm/memory.h>
#include <securec.h>

#include "../hi_pcie_defines.h"

#define BG_CTRL_REG_0						0x0AC8
#define PLL_UPDATECTRL						0xB0C
#define PMA_CTRL_OTHER_TMR0					0xBA0
#define REG_BG_MT_EN						BIT(16)
#define PLL_CTRL_STATIC						0x0B78
#define PLL_LOCK_DET_RO						0x0B70
#define BYPASS_ASYNCPOWRACK					0x0710
#define PMA_INTF_LANE_SIG_CTRL_EN_REG_5		0x1B24
#define PMA_INTF_LANE_SIG_CTRL_VAL_REG_5	0x1B68
#define PMA_INTF_LANE_SIG_CTRL_EN_REG_2		0x1B18
#define PMA_INTF_LANE_SIG_CTRL_VAL_REG_2	0x1B5C
#define RXOFSTCTRL						0x1524
#define RXUPDTCTRL						0x152C
#define RXEQACTRL						0x1534
#define RXCTRL0							0x1540
#define RXCTRL1							0x1544
#define RXDFECTRL_0						0x1580
#define RXDFECTRL_2						0x1588
#define RXDFECTRL_3						0x15B4
#define RG_RX_CFG_ADDED2                    0x1A08
#define RG_RX_CFG_ADDED4					0x1A14
#define RG_RX_CFG_ADDED6					0x1A1C
#define RG_RX_CFG_ADDED5					0x1A18
#define RG_PMA_CFG_ADDED7					0x1A20
#define PMAC_RX_CONFIG						0x1564
#define RXLOSCTRL							0x1548
#define RXGSACTRL							0x1530
#define RG_RX_CFG_ADDED1					0x1A04
#define RX_EBUF_REG							0x1C04
#define DCCCTRL0							0x15A0
#define TXTERMKCTRL0						0x0A70
#define RXTERMKCTRL0						0x0A84
#define PLL_VCO_OTHER_TMR					0x0B90
#define PCS_STATUS							0x1400
#define TXTERMKCTR2							0x0A78
#define TXDEEMPHCTRL6DB						0x1550
#define TXDEEMPHCTRL3D5DB					0x1554
#define TXDEEMPHCTRL0DB						0x1558
#define PIPE_INTF_SIG_CTRL_VAL_REG_1		0x1B44
#define LOCAL_FS_LF							0x1408
#define PCLK_LANE_INT_SRC_REG				0x1000
#define PCLK_COM_INT_SRC_REG				0x0404

// PLL CFG
#define PLL_CKGCTRLR0						0x0B24
#define PLL_CKGCTRLR1						0x0B2C
#define PLL_CKGCTRLR2						0x0B34
#define PLL_LDO_CTRL						0x0B8C

#define PLL_BANDCTRLR1						0x0B30
#define PLL_BANDCTRLR2						0x0B38
#define PLL_LOCK_DET_RO						0x0B70
#define TERMCTRL						0x154C
#define TXDRVTMR						0x0A40
#define RXTERMKCTRL1						0x0A88
#define PMACTRLSTATUS						0x0A00
#define TERMKCTRL0						0x0A9C
#define PLL_LOCK_DET_DBG					0x0B6C
#define PLL_KBAND_CPLT						0x0B74
#define REG_PV_DEBUG_CFG					0x09B0
#define PMA_PCS_IO_NEW_ADD					0x0E00
#define PMA_INTF_COM_SIG_CTRL_STS_REG_2				0x0F88
#define RXDETCTRL						0x151C
#define PMA_DBG_STS_REG						0x1594
#define PMA_DBG_INFO_SEL_REG					0x158C
#define RG_RX_CFG_ADDED3					0x1A10
#define PIPE_INTF_SIG_CTRL_STS_REG_1				0x1B84
#define PCS_POWERON_STATUS					BIT(7)
#define REG_TX_RTERMK_V48					BIT(8)

/* kphy eye param */
#define QUADRANT1			0x21
#define QUADRANT2			0x2
#define QUADRANT3			0x14
#define QUADRANT4			0x38
#define GRAPH_LEN			32
#define GRAPH_WID			33
#define QUA_LEN				16
#define IMAGE_WIDTH			66
#define FOM_COUNT_MAX			255
#define PHY3_FOM_TIMEOUT		200
#define PHY_PCLK_LANE_INT_SRC_REG_ADDR		0x1000
#define PHY_CRG_PER_LANE_ADDR			0x1300
#define PHY_PCS_POWER_STATUS_ADDR		0x1400
#define PHY_RG_LOCAL_LF_FS_ADDR			0x1408
#define PHY_RXOFSTSTATUS_ADDR			0x1528
#define PHY_RXUPDTCTRL_EN_ADDR			0x152C
#define PHY_RG_RX0_GAIN_VREF_ADDR		0x1530
#define PHY_RX_EQA_CTRL_ADDR			0x1534
#define PHY_RXFOMCTRL0_ADDR			0x1538
#define PHY_RXFOMCTRL1_ADDR			0x153C

#define POWERON_TIMEOUT						70

/* kphy debug trace */
#define DEBUG_TRACE_CONFIG			0x900
#define REG_START_MASK				0x904
#define REG_STOP_MASK				0x908
#define REG_START_TRIGGER_VAL			0x90C
#define REG_STOP_TRIGGER_VAL			0x910
#define REG_TRACE_OUT				0x914
#define REG_STATE_MASK0				0x918
#define REG_STATE_MASK1				0x91C
#define REG_STATE_MASK2				0x920
#define REG_TRACE_RADDR				0x924
#define REG_TRACR_DOUT0				0x928
#define REG_TRACR_DOUT1				0x92C
#define REG_TRACR_DOUT2				0x930
#define REG_TRACER_TRIGGER_MASK_H		0x9A4
#define REG_TRACER_TRIGGER_VALUE_H		0x9A8
#define DFX_CRG					0x604

/* kphy inner loopback */
#define TEST_MODE				0x400
#define FORCE_RATE_MODE				0x700
#define FORCE_POWER_MODE			0x704
#define FORCE_RXSTANDBY_MODE			0x708
#define PRBS_UP_SEL				0x808
#define PRBS_MODE				0x824
#define LPBK_STATUS				0x1200
#define PMA_INTF_LANE_SIG_CTRL_EN_REG_0		0x1B10
#define PMA_INTF_LANE_SIG_CTRL_EN_REG_2		0x1B18
#define PMA_INTF_LANE_SIG_CTRL_VAL_REG_0	0x1B54

#define RXOFSTUPDTCTRL						0x1520
#define SYMBOL_ERR_CNT						0x1010
#define SYMBOL_ERR_CNT_THR					0x1028

static u32 pcie_kphy_readl(struct hi_pcie_phy *phy, u32 reg)
{
	return readl(phy->phy_base + reg);
}

static void pcie_kphy_writel(struct hi_pcie_phy *phy, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, phy->phy_base + reg);
}

void pcie_kphy_dump_status(struct hi_pcie_phy *phy)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *info;
	} dumps[] = {
		{ PCLK_LANE_INT_SRC_REG, "dcc encode buffer: " },
		{ PCLK_COM_INT_SRC_REG, "tx rtermk ovudf int: " },
		{ PLL_BANDCTRLR1, "vco band value in GEN1/2: " },
		{ PLL_BANDCTRLR2, "vco band value in GEN3: " },
		{ TERMCTRL, "dcc calibration result: " },
		{ TXTERMKCTR2, "tx termk result: " },
		{ RXTERMKCTRL1, "rx termk result: " },
		{ PLL_LOCK_DET_RO, "pll lock det: " },
	};

	for (i = 0; i < ARRAY_SIZE(dumps); i++) {
		val = pcie_kphy_readl(phy, dumps[i].reg);
		hi_pcie_pr_i("%s 0x%x", dumps[i].info, val);
	}
}

void pcie_kphy_pcs_clk_sel(struct hi_pcie_phy *phy)
{
	u32 val;
	val = pcie_kphy_readl(phy, PLL_CTRL_STATIC);
	val &= ~BIT(0);
	pcie_kphy_writel(phy, val, PLL_CTRL_STATIC);
}

void pcie_kphy_setup_chs(struct hi_pcie_phy *phy)
{
	int i;
	static struct {
		u32 offset;
		u32 mask;
		u32 value;
	} configs[] = {
		{ BG_CTRL_REG_0, 0, 0x10010 },
		{ PLL_UPDATECTRL, 0, 0x800400FF},
		{ PLL_CKGCTRLR0, 0, 0x60C8000A },
		{ PLL_CKGCTRLR1, 0, 0x60C80005 },
		{ PLL_CKGCTRLR2, 0, 0x41400000 },
		{ PLL_LDO_CTRL, 0, 0x19 },
		{ RXCTRL0, 0, 0x1000 },
		{ RG_RX_CFG_ADDED6, 0, 0x22201ff },
		{ RG_RX_CFG_ADDED5, 0, 0x4440000 },
		{ RXEQACTRL, 0, 0x20180010 },
		{ RG_RX_CFG_ADDED4, 0, 0x410018 },
		{ RXCTRL1, 0, 0x5855500 },
		{ RG_RX_CFG_ADDED2, 0, 0x11102 },
		{ RXDFECTRL_0, 0, 0x40410439 },
		{ RXDFECTRL_2, 0, 0x557 },	
		{ RXGSACTRL, 0, 0x26100030 },		
		{ RG_RX_CFG_ADDED3, 0, 0x25 },
		{ RG_RX_CFG_ADDED1, 0, 0x322354 },
		{ RXUPDTCTRL, 0, 0xEE001B3F },
		{ RG_PMA_CFG_ADDED7, 0, 0x11295 },
		{ PMAC_RX_CONFIG, 0, 0x25A09104 },
		{ RXLOSCTRL, 0, 0x10053 },
		{ RX_EBUF_REG, 0, 0x2010 },
		{ DCCCTRL0, 0, 0x4E150 },
		{ TXDRVTMR, 0, 0x26200780 },
		{ TERMKCTRL0, 0, 0xDB },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_0, 0, 0x6 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_0, 0, 0x0 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_2, 0, 0xF },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_2, 0, 0x600 },
	};
	for (i = 0; i < ARRAY_SIZE(configs); i++)
		pcie_kphy_writel(phy, configs[i].value, configs[i].offset);
}

void pcie_kphy_setup(struct hi_pcie_phy *phy)
{
	int i;
	static struct {
		u32 offset;
		u32 mask;
		u32 value;
	} configs[] = {
		{ BG_CTRL_REG_0, 0, 0x10000 },
		{ PLL_UPDATECTRL, 0, 0x800400ff},
		{ PLL_CTRL_STATIC, 0, 0x0 },
		{ RXCTRL0, 0, 0x1000},
		{ RG_RX_CFG_ADDED6, 0, 0x22201FF },
		{ RG_RX_CFG_ADDED5, 0, 0x2000000 },
		{ RXEQACTRL, 0, 0x20180010 },
		{ RG_RX_CFG_ADDED4, 0, 0x410018 },
		{ RXDFECTRL_0, 0, 0x40410439 },
		{ RXDFECTRL_2, 0, 0x557 },
		{ RXGSACTRL, 0, 0x26100030 },
		{ RG_RX_CFG_ADDED3, 0, 0x25 },
		{ RG_RX_CFG_ADDED1, 0, 0x30A354 },
#if defined(CONFIG_HIPCIE_NSV) || defined(CONFIG_HIPCIE_XN) || defined(CONFIG_HIPCIE_NCH)
		{ RXUPDTCTRL, 0, 0xFE001B7F },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_0, 0, 0x6 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_0, 0, 0x0 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_2, 0, 0xF },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_2, 0, 0x600 },
#else
		{ RXUPDTCTRL, 0, 0xee001b7f },
#endif
		{ RG_PMA_CFG_ADDED7, 0, 0x11295 },
		{ PMAC_RX_CONFIG, 0, 0x25a09104 },
		{ RXLOSCTRL, 0, 0x10053 },
		{ RX_EBUF_REG, 0, 0x2010 },
		{ DCCCTRL0, 0, 0x4e150 },
		{ TXTERMKCTRL0, 0, 0xe66177 },
		{ RXTERMKCTRL0, 0, 0x136e1 },
		{ TXDRVTMR, 0, 0x26200780 },
		{ TERMKCTRL0, 0, 0xbb },
	};

	for (i = 0; i < ARRAY_SIZE(configs); i++)
		pcie_kphy_writel(phy, configs[i].value, configs[i].offset);
}

int pcie_kphy_check_status(struct hi_pcie_phy *phy)
{
	u32 val;
	int timeout = 0;

	while(1) {
		val = pcie_kphy_readl(phy, PCS_STATUS);
		if ((val & PCS_POWERON_STATUS) != 0)
			return 0;

		udelay(5);
		timeout++;
		if (timeout > POWERON_TIMEOUT) {
			hi_pcie_pr_e("pcs status %u invalid", val);
			return -1;
		}
	}

	return 0;
}

int pcie_kphy_cfg_ffe_chs(struct hi_pcie_phy *phy)
{
	pcie_kphy_writel(phy, 0x6001E00, TXDEEMPHCTRL6DB);
	pcie_kphy_writel(phy, 0x4002000, TXDEEMPHCTRL3D5DB);
	pcie_kphy_writel(phy, 0x2F00, TXDEEMPHCTRL0DB);
	/* config FS, LF */
	pcie_kphy_writel(phy, 0x1018, LOCAL_FS_LF);
	pcie_kphy_writel(phy, 0x618, LOCAL_FS_LF);

	return 0;
}

int pcie_kphy_cfg_ffe(struct hi_pcie_phy *phy)
{
	u32 val;

	udelay(350);

	val = pcie_kphy_readl(phy, TXTERMKCTR2);
	if ((val & REG_TX_RTERMK_V48) != 0) {
#if defined(CONFIG_HIPCIE_NSV) || defined(CONFIG_HIPCIE_XN) || defined(CONFIG_HIPCIE_NCH)
		pcie_kphy_writel(phy, 0xc002400, TXDEEMPHCTRL6DB);
		pcie_kphy_writel(phy, 0x8002800, TXDEEMPHCTRL3D5DB);
		pcie_kphy_writel(phy, 0x3000, TXDEEMPHCTRL0DB);
		/* config FS, LF */
		pcie_kphy_writel(phy, 0x1030, LOCAL_FS_LF);
#else
		pcie_kphy_writel(phy, 0x10002f00, TXDEEMPHCTRL6DB);
		pcie_kphy_writel(phy, 0xb003400, TXDEEMPHCTRL3D5DB);
		pcie_kphy_writel(phy, 0x3f00, TXDEEMPHCTRL0DB);
		/* config FS, LF */
		pcie_kphy_writel(phy, 0xf3f, LOCAL_FS_LF);
#endif
	} else {
#if defined(CONFIG_HIPCIE_NSV) || defined(CONFIG_HIPCIE_XN) || defined(CONFIG_HIPCIE_NCH)
		pcie_kphy_writel(phy, 0x9001b00, TXDEEMPHCTRL6DB);
		pcie_kphy_writel(phy, 0xc24, LOCAL_FS_LF);
#else
		/* config FS, LF */
		pcie_kphy_writel(phy, 0xc2f, LOCAL_FS_LF);
#endif
	}

	return (val & REG_TX_RTERMK_V48);
}

static void pcie_kphy_bit_write(struct hi_pcie_phy *phy,
				u32 reg, u32 msb, u32 lsb, u32 val)
{
	u32 data = pcie_kphy_readl(phy, reg);

	data = drv_32bit_set_field(data, lsb, msb, val);
	pcie_kphy_writel(phy, data, reg);
}

static u32 pcie_kphy_bit_read(struct hi_pcie_phy *phy,
			      u32 reg, u32 msb, u32 lsb)
{
	u32 data = pcie_kphy_readl(phy, reg);

	return drv_32bit_read_field(data, lsb, msb);
}

static u32 g_fom_pattern = 0;
void pcie_kphy_set_fom_param(u32 fom_pattern)
{
	g_fom_pattern = fom_pattern;
}

static char pcie_kphy_eye_pattern_cfg(struct hi_pcie_phy *phy, u32 quadrant,
						u32 vref, u32 pi, enum eyefom_mode mode)
{
	u32 reg_val;
	int timeout = PHY3_FOM_TIMEOUT;
	int loop_cnt = 0;
	char ch;

	/* rx_fomrk_en */
	pcie_kphy_bit_write(phy, PHY_RXUPDTCTRL_EN_ADDR, 11, 11, 0x0);

	if (mode == DIG_EYEFORM) {
		/* fom_type_sel */
		pcie_kphy_bit_write(phy, PHY_RXFOMCTRL1_ADDR, 6, 6, 0);
		/* reg_fom_mode */
		pcie_kphy_bit_write(phy, RXDFECTRL_0, 6, 6, 0x1);
		/* reg_fom_data_pattern */
		pcie_kphy_bit_write(phy, RXDFECTRL_2, 5, 3, g_fom_pattern);
	} else {
		pcie_kphy_bit_write(phy, PHY_RXFOMCTRL1_ADDR, 6, 6, 0x0);
	}

	/* quadrant sel */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL1_ADDR, 7, 0, quadrant);
	/* vref */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL1_ADDR, 15, 12, vref);
	/* pi_step */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL1_ADDR, 11, 8, pi);
	/* fom_sw_load */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL0_ADDR, 1, 1, 0x1);
	udelay(1);
	/* fom en */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL0_ADDR, 2, 2, 0x1);
	/* rx_rxg en */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL0_ADDR, 5, 5, 0x1);
	/* PMA_CLK_CG_EN */
	pcie_kphy_bit_write(phy, PHY_CRG_PER_LANE_ADDR, 13, 13, 0x1);

	udelay(1);
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL0_ADDR, 0, 0, 0x1);
	udelay(2);

	/* wait from cpl intr. */
	reg_val = pcie_kphy_bit_read(phy, PHY_PCLK_LANE_INT_SRC_REG_ADDR, 18, 18);
	while (reg_val != 0x1) {
		reg_val = pcie_kphy_bit_read(phy, PHY_PCLK_LANE_INT_SRC_REG_ADDR, 18, 18);
		loop_cnt++;
		udelay(5);
		if (loop_cnt > timeout) {
			hi_pcie_pr_e("Timeout to wait fom cpl intr\n");
			return 'x';
		}
	}
	reg_val = pcie_kphy_bit_read(phy, PHY_RXOFSTSTATUS_ADDR, 15, 8);
	/* fom en = 0 */
	pcie_kphy_bit_write(phy, PHY_RXFOMCTRL0_ADDR, 2, 0, 0);
	pcie_kphy_bit_write(phy, PHY_RXUPDTCTRL_EN_ADDR, 5, 5, 0);

	if (reg_val == FOM_COUNT_MAX)
		ch = ' ';
	else if (reg_val == 0)
		ch = '-';
	else
		ch = '+';

	return ch;
}

static u32 g_quadrant[4] = {QUADRANT1, QUADRANT4, QUADRANT2, QUADRANT3};
static int g_dir[][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
static char g_res[GRAPH_LEN][GRAPH_LEN + 1];
static char g_eye_title[] = "FEDCBA98765432100123456789ABCDEF";
static int pcie_kphy_eye_pattern(struct hi_pcie_phy *phy, enum eyefom_mode mode)
{
	int i, j, pi_step, vref;
	char ch, out[IMAGE_WIDTH];

	if (memset_s(g_res, sizeof(g_res), 'x', sizeof(g_res)))
		return -1;

	for (i = 0; i < 4; i++) {
		for (vref = 0; vref < QUA_LEN; vref++) {
			for (pi_step = 0; pi_step < QUA_LEN; pi_step++) {
				ch = pcie_kphy_eye_pattern_cfg(phy, g_quadrant[i], (u32)vref, (u32)pi_step, mode);
				g_res[QUA_LEN + (vref * g_dir[i][1])][QUA_LEN + (pi_step * g_dir[i][0])] = ch;
			}
		}
	}

	if (memcpy_s(g_res[0], sizeof(g_res), g_eye_title, sizeof(g_eye_title)))
		return -1;

	for (i = 0; i < GRAPH_LEN; i++) {
		g_res[i][GRAPH_LEN - 1] = 0;
		g_res[i][0] = g_eye_title[i];
		if (memset_s(out, sizeof(out), ' ', sizeof(out)))
			return -1;

		for (j = 0; j < GRAPH_LEN; j++)
			out[2 * j] = g_res[i][j];
		out[IMAGE_WIDTH - 1] = 0;
		hi_pcie_pr_e("%s", out);
	}

	return 0;
}

static int pcie_kphy_param_dump(struct hi_pcie_phy *phy)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
		u32 msb;
		u32 lsb;
		u32 data;
	} kphy_info[] = {
		{ BG_CTRL_REG_0, "BandGap: BGR_triming reg_bg_vref_sel", 3, 0, 0xFFFFFFFF },
		{ PLL_LOCK_DET_RO, "PLL lock detect: reg_pll_ad_pll_lock", 0, 0, 0xFFFFFFFF },
		{ PLL_LOCK_DET_RO, "PLL lock detect: reg_pll_dig_pll_lock", 2, 2, 0xFFFFFFFF },
		{ PLL_LOCK_DET_RO, "PLL lock detect: reg_pll_lock_overtime", 3, 3, 0xFFFFFFFF },
		{ REG_PV_DEBUG_CFG, "set rg_pv_en/div", 3, 0, 0x7 },
		{ PMA_PCS_IO_NEW_ADD, "PVSENSOR: pv_out reg_ad_pv_dout_debounce", 15, 8, 0xFFFFFFFF },
		{ PLL_BANDCTRLR1, "GEN2 PLL: reg_pll_kband_r1", 9, 0, 0xFFFFFFFF },
		{ PLL_BANDCTRLR2, "GEN3 PLL: reg_pll_kband_r2", 9, 0, 0xFFFFFFFF },
		{ PLL_KBAND_CPLT, "PLL-kBand: reg_kband_cplt_synzd", 0, 0, 0xFFFFFFFF },
		{ PMA_INTF_COM_SIG_CTRL_STS_REG_2, "PLL0-kBand: reg_da_pll_kband_ctrl_sts", 17, 8, 0xFFFFFFFF },
		{ PMACTRLSTATUS, "PLL-Lock-Status: pll0_dig_pll_lock_status", 8, 8, 0xFFFFFFFF },
		{ TERMCTRL, "TXP impedance: tx_termp", 18, 16, 0xFFFFFFFF },
		{ TERMCTRL, "TXN impedance: tx_termn", 14, 12, 0xFFFFFFFF },
		{ TERMCTRL, "RX impedance: rx_term", 4, 0, 0xFFFFFFFF },
		{ PCS_STATUS, "PCS_STATUS: power_status", 3, 0, 0xFFFFFFFF },
		{ PCS_STATUS, "PCS_STATUS: rate_status", 5, 4, 0xFFFFFFFF },
		{ PIPE_INTF_SIG_CTRL_STS_REG_1, "Tx_Deemph: reg_pipe_txdeemph_ctrl_sts", 24, 7, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x1 },
		{ PMA_DBG_STS_REG, "DA_EQ_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x2 },
		{ PMA_DBG_STS_REG, "DA_SA0_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x3 },
		{ PMA_DBG_STS_REG, "DA_SA1_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x4 },
		{ PMA_DBG_STS_REG, "DA_SA2_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x5 },
		{ PMA_DBG_STS_REG, "DA_SA3_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x6 },
		{ PMA_DBG_STS_REG, "DA_GS0_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x7 },
		{ PMA_DBG_STS_REG, "DA_GS1_OS: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x8 },
		{ PMA_DBG_STS_REG, "DA_RX_ATT: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x9 },
		{ PMA_DBG_STS_REG, "DA_SEL_R: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0xA },
		{ PMA_DBG_STS_REG, "DA_SEL_GAIN: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0xB },
		{ PMA_DBG_STS_REG, "DA_SEL_C: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x1C },
		{ PMA_DBG_STS_REG, "CLTE2_SEL_R: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x1D },
		{ PMA_DBG_STS_REG, "CLTE2_SEL_C: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x18 },
		{ PMA_DBG_STS_REG, "DA_DFE_EQ_TAP1: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x19 },
		{ PMA_DBG_STS_REG, "DA_DFE_EQ_TAP2: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ PMA_DBG_INFO_SEL_REG, "reg_rx_dig_info_sel", 6, 2, 0x1A },
		{ PMA_DBG_STS_REG, "DA_DFE_EQ_TAP3: ad_rx_dig_info", 20, 16, 0xFFFFFFFF },
		{ SYMBOL_ERR_CNT, "SYMBOL_ERR_CNT: reg_symbol_err_cnt", 15, 0, 0xFFFFFFFF },
		{ SYMBOL_ERR_CNT_THR, "SYMBOL_ERR_CNT_THR: reg_symbol_err_cnt_thr", 15, 0, 0xFFFFFFFF },
	};

	for (i = 0; i < ARRAY_SIZE(kphy_info); i++) {
		if (kphy_info[i].data == 0xFFFFFFFF) {
			val = pcie_kphy_bit_read(phy, kphy_info[i].reg, kphy_info[i].msb, kphy_info[i].lsb);
			hi_pcie_pr_e("[%s] reg: 0x%x, bit [%u:%u]:0x%x", kphy_info[i].desc,
				kphy_info[i].reg, kphy_info[i].msb, kphy_info[i].lsb, val);
		} else {
			pcie_kphy_bit_write(phy, kphy_info[i].reg, kphy_info[i].msb,
					kphy_info[i].lsb, kphy_info[i].data);
		}
	}
	return 0;
}

static int pcie_kphy_detect_rx(struct hi_pcie_phy *phy)
{
	u32 val;

	/* pma clk lane en_sel */
	val = pcie_kphy_readl(phy, PHY_CRG_PER_LANE_ADDR);
	val = val | BIT(13);
	pcie_kphy_writel(phy, val, PHY_CRG_PER_LANE_ADDR);

	/* rxn rxp present */
	val = pcie_kphy_readl(phy, RXDETCTRL);
	val = val & 0xFFFFFCFF;
	pcie_kphy_writel(phy, val, RXDETCTRL);

	/* rx np det en */
	val = pcie_kphy_readl(phy, RXDETCTRL);
	val = val | BIT(4) | BIT(5);
	pcie_kphy_writel(phy, val, RXDETCTRL);

	/* rx det req set rx det en */
	val = pcie_kphy_readl(phy, RXDETCTRL);
	val = val | BIT(1);
	val = val & ~BIT(0);
	pcie_kphy_writel(phy, val, RXDETCTRL);

	udelay(15);

	val = pcie_kphy_readl(phy, RXDETCTRL);
	if ((val & (BIT(8) | BIT(9))) != (BIT(8) | BIT(9)))
		hi_pcie_pr_e("detect failed");
	return 0;
}

static int pcie_kphy_param_change(struct hi_pcie_phy *phy)
{
	int size;
	if (phy->phy_change.change_param_size <= 0) {
		hi_pcie_pr_e("phy reg_val is null");
		return 0;
	}

	for (size = 0; size < (int)phy->phy_change.change_param_size; size++)
		pcie_kphy_writel(phy, phy->phy_change.change_param[size],
					phy->phy_change.change_param_addr[size]);

	return 0;
}

#define PCIE_KPHY_FIFO_SIZE 128
static struct {
	u32 roll_addr;
	u32 dout0;
	u32 dout1;
	u32 dout2;
	u32 dout3;
} kphy_trace_data[PCIE_KPHY_FIFO_SIZE] = {
	{0xFFFFFFFF, 0, 0, 0, 0},
};

static int pcie_kphy_trace_data_init(struct hi_pcie_phy *phy,
						u32 msk0, u32 msk1, u32 msk2, u32 msk3)
{
	u32 step;
	struct {
		u32 reg;
		u32 msb;
		u32 lsb;
		u32 data;
	} data_init[] = {
		/* 3.0 kphy default triger lane 0 */
		{ DEBUG_TRACE_CONFIG, 2, 2, 0x0 },
		{ REG_START_MASK, 31, 0, 0x0 },
		{ REG_TRACER_TRIGGER_MASK_H, 31, 16, 0x0 },
		{ REG_START_TRIGGER_VAL, 31, 0, 0x0 },
		{ REG_TRACER_TRIGGER_VALUE_H, 31, 16, 0x0 },
		{ REG_STOP_MASK, 31, 0, 0x0 },
		{ REG_TRACER_TRIGGER_MASK_H, 14, 0, 0x0 },
		{ REG_STOP_TRIGGER_VAL, 31, 0, 0xffffffff },
		{ REG_TRACER_TRIGGER_VALUE_H, 15, 0, 0x7fff },
		{ DEBUG_TRACE_CONFIG, 11, 8, 0x0 },
		{ DEBUG_TRACE_CONFIG, 1, 1, 0x0 },
		{ REG_STATE_MASK0, 31, 0, msk0 },
		{ REG_STATE_MASK1, 31, 0, msk1 },
		{ REG_STATE_MASK2, 31, 0, msk2 },
		{ DEBUG_TRACE_CONFIG, 7, 4, 0x0 },
		{ DEBUG_TRACE_CONFIG, 0, 0, 0x1 },
		{ DFX_CRG, 2, 2, 0x0 },
		{ DFX_CRG, 2, 2, 0x1 },
	};

	for (step = 0; step < ARRAY_SIZE(data_init); step++)
		pcie_kphy_bit_write(phy, data_init[step].reg, data_init[step].msb,
					data_init[step].lsb, data_init[step].data);

	return 0;
}

static int pcie_kphy_trace_data_save(struct hi_pcie_phy *phy)
{
	u32 roll_addr;
	u32 addr;

	addr = pcie_kphy_readl(phy, REG_TRACE_OUT);
	if(addr & BIT(7))
		addr = (addr + 1) % PCIE_KPHY_FIFO_SIZE;
	else
		addr = 0;

	hi_pcie_pr_e("kphy_trace data log start_addr is 0x%x", addr);
	for (roll_addr = 0; roll_addr < PCIE_KPHY_FIFO_SIZE; roll_addr++) {
		pcie_kphy_bit_write(phy, REG_TRACE_RADDR, 6, 0, roll_addr);
		kphy_trace_data[roll_addr].roll_addr = roll_addr;
		kphy_trace_data[roll_addr].dout0 = pcie_kphy_bit_read(phy, REG_TRACR_DOUT0, 32, 0);
		kphy_trace_data[roll_addr].dout1 = pcie_kphy_bit_read(phy, REG_TRACR_DOUT1, 32, 0);
		kphy_trace_data[roll_addr].dout2 = pcie_kphy_bit_read(phy, REG_TRACR_DOUT2, 32, 0);
		kphy_trace_data[roll_addr].dout3 = pcie_kphy_bit_read(phy, REG_TRACE_OUT, 10, 9);
	}

	return 0;
}

#define PCIE_KPHY_TRACER_LOGSIZE		128
static int pcie_kphy_trace_data_dump(struct hi_pcie_phy *phy)
{
	int ret;
	u32 roll_addr;
	size_t size = PCIE_KPHY_TRACER_LOGSIZE;
	char *buff = (char *)kzalloc(PCIE_KPHY_TRACER_LOGSIZE, GFP_KERNEL);
	if (!buff) {
		hi_pcie_pr_e("buff kzalloc failed");
		return -ENOMEM;
	}

	for (roll_addr = 0; roll_addr < PCIE_KPHY_FIFO_SIZE; roll_addr++) {
		ret = snprintf_s(buff, size, size - 1, "kphy_trace_%03u[128:0] %08x%08x%08x%08x\n",
			kphy_trace_data[roll_addr].roll_addr, kphy_trace_data[roll_addr].dout3,
			kphy_trace_data[roll_addr].dout2, kphy_trace_data[roll_addr].dout1,
			kphy_trace_data[roll_addr].dout0);
		if (ret < 0) {
			hi_pcie_pr_e("mem buff overrun, size = %u, roll addr = 0x%x", size, roll_addr);
			kfree(buff);
			return 0;
		}
		hi_pcie_pr_e("%s", buff);
	}

	kfree(buff);
	return 0;
}

static int pcie_kphy_send_patten(struct hi_pcie_phy *phy, u32 patt0, u32 patt1, u32 patt2, u32 patt3)
{
	pcie_kphy_bit_write(phy, 0x400, 6, 0, 0x1);
	pcie_kphy_bit_write(phy, 0x700, 1, 0, 0x2);
	pcie_kphy_bit_write(phy, 0x704, 3, 0, 0x0);
	pcie_kphy_bit_write(phy, 0x808, 0, 0, 0x0);

	pcie_kphy_writel(phy, patt3, 0x818);
	pcie_kphy_writel(phy, patt2, 0x814);
	pcie_kphy_writel(phy, patt1, 0x810);
	pcie_kphy_writel(phy, patt0, 0x80C);
	pcie_kphy_bit_write(phy, 0x1300, 10, 10, 0x1);
	pcie_kphy_bit_write(phy, 0x1300, 13, 13, 0x1);
	pcie_kphy_bit_write(phy, 0x820, 0, 0, 0x1);
	return 0;
}

static int pcie_kphy_inner_loopback(struct hi_pcie_phy *phy)
{
	u32 val;
	u32 step;
	struct {
		u32 reg;
		u32 msb;
		u32 lsb;
		u32 data;
	} lpbk_init[] = {
		{ TEST_MODE, 6, 0, 0x2 },
		{ FORCE_RATE_MODE, 1, 0, 0x2 },
		{ PHY_PCS_POWER_STATUS_ADDR, 5, 4, 0xFFFFFFFF },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_2, 0, 0, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_2, 2, 2, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_2, 1, 1, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_2, 3, 3, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_2, 3, 0, 0x0 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_2, 9, 9, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_2, 8, 4, 0x0 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_2, 10, 10, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_0, 5, 1, 0xB },
		{ PMA_INTF_LANE_SIG_CTRL_VAL_REG_0, 6, 6, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_0, 2, 2, 0x1 },
		{ PMA_INTF_LANE_SIG_CTRL_EN_REG_0, 1, 1, 0x1 },
		{ FORCE_POWER_MODE, 3, 0, 0x0 },
		{ PHY_PCS_POWER_STATUS_ADDR, 3, 0, 0xFFFFFFFF },
		{ PRBS_UP_SEL, 0, 0, 0x0 },
		{ PRBS_MODE, 1, 0, 0x0 },
		{ PHY_CRG_PER_LANE_ADDR, 13, 13, 0x1 },
		{ PHY_CRG_PER_LANE_ADDR, 10, 10, 0x1 },
		{ FORCE_RXSTANDBY_MODE, 0, 0, 0x0 },
		{ PHY_PCS_POWER_STATUS_ADDR, 6, 6, 0xFFFFFFFF },
		{ PHY_CRG_PER_LANE_ADDR, 13, 13, 0x1 },
		{ PHY_CRG_PER_LANE_ADDR, 11, 11, 0x1 },
		{ PHY_CRG_PER_LANE_ADDR, 10, 10, 0x1 },
	};

	for (step = 0; step < ARRAY_SIZE(lpbk_init); step++) {
		if (lpbk_init[step].data == 0xFFFFFFFF) {
			val = pcie_kphy_bit_read(phy, lpbk_init[step].reg, lpbk_init[step].msb,
						lpbk_init[step].lsb);
			hi_pcie_pr_e("reg is 0x%x, val = 0x%x", lpbk_init[step].reg, val);
		} else {
			pcie_kphy_bit_write(phy, lpbk_init[step].reg, lpbk_init[step].msb,
					    lpbk_init[step].lsb, lpbk_init[step].data);
		}
	}

	udelay(50);
	val = pcie_kphy_bit_read(phy, LPBK_STATUS, 2, 0);
	if (val != 0x3) {
		hi_pcie_pr_e("enter inner loopback failed status is 0x%x", val);
		return -EINVAL;
	}

	hi_pcie_pr_e("enter inner loopback succ");
	return 0;
}

static int pcie_kphy_enter_loopback(struct hi_pcie_phy *phy, enum lpbk_mode mode)
{
	int ret = 0;

	switch (mode) {
	case INNER_LOOPBACK:
		ret = pcie_kphy_inner_loopback(phy);
		break;
	case MAC_TX2RX_LPBK:
		break;
	default:
		hi_pcie_pr_e("mode[%d] is not supported", mode);
	}

	return ret;
}

static struct hi_pcie_phy_dfx_ops kphy_dfx_ops = {
	.phy_eye_pattern		= pcie_kphy_eye_pattern,
	.phy_param_dump			= pcie_kphy_param_dump,
	.phy_tx_detect_rx		= pcie_kphy_detect_rx,
	.phy_param_change		= pcie_kphy_param_change,
	.phy_trace_data_init		= pcie_kphy_trace_data_init,
	.phy_trace_data_save		= pcie_kphy_trace_data_save,
	.phy_trace_data_dump		= pcie_kphy_trace_data_dump,
	.phy_enter_loopback		= pcie_kphy_enter_loopback,
	.phy_send_pattern		= pcie_kphy_send_patten,
};

static struct hi_pcie_phy_ops kphy_ops = {
	.phy_bit_read = pcie_kphy_bit_read,
	.phy_bit_write = pcie_kphy_bit_write,
};

int pcie_kphy_dfx_register(struct hi_pcie *pcie)
{
	pcie->phy_ops = &kphy_ops;
	pcie->phy_dfx_ops = &kphy_dfx_ops;
	return 0;
}

