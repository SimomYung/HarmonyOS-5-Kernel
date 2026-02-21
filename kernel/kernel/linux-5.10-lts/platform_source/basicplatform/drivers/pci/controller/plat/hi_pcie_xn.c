/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: PCIe xn platform functions.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
#include <linux/delay.h>
#include <linux/gpio.h>
#ifdef CONFIG_HIPCIE_SUPPORT_FFA
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#else
#include <platform_include/see/bl31_smc.h>
#endif
#include <linux/iommu/mm_svm.h>
#include <linux/arm-smccc.h>
#include <linux/of_address.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/of.h>
#include <linux/jiffies.h>
#include <linux/pinctrl/consumer.h>
#include <linux/clk-provider.h>
#include <securec.h>
#include <doze.h>

#include "../hi_pcie_defines.h"
#include "../ctrl/hi_pcie_ctrl_common.h"
#include "../phy/hi_pcie_kphy.h"
#include "../ctrl/hi_pcie_ctrl_dfx.h"
#include "../hi_pcie_external_api.h"
#include "hi_pcie_idle.h"

atomic_t pcie_phy_clk_cnt = ATOMIC_INIT(0);
atomic_t pcie_tcu_pwr_cnt = ATOMIC_INIT(0);

static DEFINE_SPINLOCK(pcie_fnpll_lock);

#define ENABLE 1
#define DISABLE 0

#define AOCRGSCCLKDIV3						0x25C
#define AOCRGAO_ATUODIV2                    0X500
#define AOCRGSCPEREN6                       0X1D0
#define HSDT_CRG_PERI_AUTODIV3              0X35C
#define HSDT_PERRSTEN0                      0x060
#define HSDT_PERRSTDIS0                     0x064
#define HSDT_PERRSTSTAT0                    0x068
#define HSDT_PEREN1                         0x010
#define HSDT_PERDIS1                        0x014
#define HSDT_PERCLKEN1                      0x018
#define HSDT_PERSTAT1                       0x01C
#define HSDT_CRG_PCIEPLL_PCIE_CTRL0         0x190
#define HSDT_CRG_PCIEPLL_PCIE_FCW           0x194
#define HSDT_CRG_PCIEPLL_STAT               0x204
#define HSDT_CRG_PCIECTRL0                  0x300
#define HSDT_SCTRL_SMMU_SD                  0x094

#define HIPCIE_RST_EN0                      0x190
#define HIPCIE_RST_DIS0                     0x194
#define HIPCIE_ICG_EN0                      0x160
#define HIPCIE_ICG_DIS0                     0x164
#define HIPCIE_ICG_EN1                      0x170
#define HIPCIE_ICG_DIS1                     0x174
#define HIPCIE_PHY_CFG0                     0x034
#define HIPCIE_PHY_CFG1                     0X038
#define HIPCIE_MISC_STATE0                  0x100
#define HIPCIE_CTRL_STAT2                   0x408
#define HIPCIE_SC_PCIE_CTRL_STAT19			0x290
#define HIPCIE_SC_PCIE_CTRL_STAT20			0x294
#define HIPCIE_MISC_CFG9_V3					0x05C

#define PCIE_SRAM_CTRL_BUS_SP               0x424
#define PCIE_SRAM_CTRL_BUS_TP               0x428
#define GLB_SRAM_CTRL_BUS_EN                0x42C

#define SC_PCIECTRL_CTRL31                  0x07C
#define CLKREQ_IN_N_CTRL                    0x090
#define SOC2PHY_ISO_EN                      0x000
#define PCLK_COM_INT_SRC_REG                0x404
#define RXOFSTUPDTCTRL                      0x1520

#define FNPLL_LOCK_TIMEOUT                  20
#define FNPLL_LOCK_BIT						(0x1 << 4)
#define FNPLL_PCIE_ANALOG_CFG0              0x0008
#define FNPLL_PCIE_ANALOG_CFG1              0x000C
#define FNPLL_PCIE_ANALOG_CFG2              0x0010
#define FNPLL_PCIE_ANALOG_CFG3              0x0014
#define FNPLL_PCIE_ADPLL_ATUNE_CFG          0x0018
#define FNPLL_PCIE_ADPLL_PTUNE_CFG          0x001C
#define FNPLL_PCIE_DTC_CFG                  0x0024
#define FNPLL_PCIE_FTDC_CFG                 0x0028
#define FNPLL_PCIE_FDTC_LMS_CFG             0x002C
#define FNPLL_PCIE_DTC_LMS_CFG              0x0030
#define FNPLL_PCIE_LPF_BW_I_T01             0x0034
#define FNPLL_PCIE_LPF_BW_P_T01             0x0038
#define FNPLL_PCIE_ADPLL_DCO_TMODE          0x003C
#define FNPLL_PCIE_ADPLL_LPF_TEST_CFG       0x0040
#define FNPLL_PCIE_ADPLL_DSM_CFG            0x0044
#define FNPLL_PCIE_ADPLL_FTDC_LOCK_THR      0x0048
#define FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR    0x004C
#define FNPLL_PCIE_CHAOS_CNT_THR_CFG        0x0050
#define FNPLL_PCIE_ADPLL_CLK_EN_CFG0        0x0054
#define FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG     0x005C
#define FNPLL_PCIE_DTC_LINEAR_TEST_MODE     0x0060
#define FNPLL_PCIE_FREZ_LMS_PTHR            0x0064
#define FNPLL_PCIE_ACQ_CNT_THR_CFG          0x0068
#define FNPLL_PCIE_ADPLL_DLY_CFG            0x006C
#define FNPLL_PCIE_PTUNE_FBDIV              0x0070

#define BASE_ADDR_NUM						32
#define RC_ID0								0

#define SC_PCIE_SMMU_CTRL_AW_NS				0x200
#define SC_PCIE_SMMU_CTRL_AR_NS				0x204
#define SC_PCIE_NSSID_CTRL_NS				0x28C

#define PCIEPLL_STATE_OPEN					0x1
#define PCIEPLL_STATE_CLOSE					0x2
#define PCIEPLL_STATE_MASK					0x3

#define HIPCIE_LTSSM_L1_STATE				0x17

static struct {
	void __iomem *aocrg_base;
	void __iomem *hsdtcrg_base;
	void __iomem *pcie_fnpll_base;
	void __iomem *hsdtsctrl_base;
	void __iomem *hsdtxctrl_base;
} plat_config = {
	NULL, NULL, NULL, NULL, NULL
};

struct hi_pcie_xn {
	struct hi_pcie *pcie;
	struct hi_pcie_ctrl *ctrl;

	void __iomem *sysctrl_base;
	void __iomem *ctrl_base;
	void __iomem *phy_base;
	void __iomem *tbu_base;
	void __iomem *phy_pcs_base;
	struct clk *phy_ref_clk;
	struct pinctrl *pinctrl;
	struct pinctrl_state *pinctrls_default;
	struct pinctrl_state *pinctrls_idle;

	int level2_irq;
	u32 clkreq_gpio;
};

struct {
	u32 smmu_id;
	u32 sid;
	u32 mode_reg;
	u32 aw_reg;
	u32 ar_reg;
	atomic_t *tcu_pwr_cnt;
} xn_pcie_smmu[] = {
	{
		3, 14,
		SC_PCIE_NSSID_CTRL_NS,
		SC_PCIE_SMMU_CTRL_AW_NS,
		SC_PCIE_SMMU_CTRL_AR_NS,
		&pcie_tcu_pwr_cnt,
	}
};

struct pcie_behavior {
	int behavior_id;
	int size;
	u32 base_addr[BASE_ADDR_NUM];
	u32 val[BASE_ADDR_NUM];
};

enum {
	PCIE_RST = 0,
	PCIE_UNRST,
	OPEN_SYSCLK,
	CLOSE_SYSCLK,
	SC_RST,
	SC_UNRST,
	EN_SC_CLK,
	DIS_SC_CLK,
	OE_IE_CFG_RC,
	OE_IE_CFG_EP,
	PHY_RST_N_SEL,
	PHY_PORN_RST,
	PHY_APB_UNRESET,
	PHY_APB_RESET,
	PHY_RESET_N,
	PIPE_RESET_N,
	CTRL_PORN_UNRST,
	CTRL_PORN_RST,
	CTRL_SD_EXIT,
	SMMU_SD_EXIT,
	PIPE_CLK_STABLE,
	PU_RX_TERMIANTION,
	QIC_POWER_DOMAIN_ENTER,
	QIC_POWER_DOMAIN_EXIT,
	BEHAVIOR_MAX,
};

struct hi_pcie_behavior {
	int id;
	struct pcie_behavior behavior[BEHAVIOR_MAX];
};

const struct hi_pcie_behavior pcie_init_step[] = {
	{ RC_ID0,
		{
			{ PCIE_RST, 1, { HSDT_PERRSTEN0 },
				{ (BIT(28) | BIT(19) | BIT(15) | BIT(11) | BIT(9) | BIT(8) | BIT(7) | BIT(5) | BIT(3) | BIT(0)) } },
			{ PCIE_UNRST, 1, { HSDT_PERRSTDIS0 },
				{ BIT(28) | BIT(19) | BIT(15) | BIT(11) | BIT(8) | BIT(5) | BIT(3) | BIT(0) } },
			{ OPEN_SYSCLK, 1, { HSDT_PEREN1 }, 0xFFFE1F3F }, // hard ctrl
			{ CLOSE_SYSCLK, 1, { HSDT_PERDIS1 }, 0XFFFE1FFF }, // hard ctrl
			{ SC_RST, 1, { HIPCIE_RST_EN0 }, { 0xFFFFFFFF } },
			{ SC_UNRST, 1, { HIPCIE_RST_DIS0 }, { 0xFFFFFFFF } },
			{ EN_SC_CLK, 2, { HIPCIE_ICG_EN0, HIPCIE_ICG_EN1 }, { 0xFFFFFFFF, 0xFFFFFFFF } },
			{ DIS_SC_CLK, 2, { HIPCIE_ICG_DIS0, HIPCIE_ICG_DIS1 }, { 0xFFFFFFFF, 0xFFFFFFFF } },
			{ OE_IE_CFG_RC, 1, { HSDT_CRG_PCIECTRL0 }, { BIT(27) } }, // hard ctrl
			{ OE_IE_CFG_EP, 1, { HSDT_CRG_PCIECTRL0 }, { BIT(11) | BIT(9) | BIT(6) } }, // soft ctrl
			{ PHY_RST_N_SEL, 1, { HIPCIE_PHY_CFG0 }, { BIT(5) } },
			{ PHY_PORN_RST, 1, { HIPCIE_PHY_CFG0 }, { BIT(2) } },
			{ PHY_APB_UNRESET, 1, { HSDT_PERRSTDIS0 }, { BIT(9) } },
			{ PHY_APB_RESET, 1, { HSDT_PERRSTEN0 }, { BIT(9) } },
			{ PHY_RESET_N, 1, { HIPCIE_PHY_CFG0 }, { (BIT(4)) } },
			{ PIPE_RESET_N, 0, {}, {} },
			{ CTRL_PORN_UNRST, 1, { HSDT_PERRSTDIS0 }, { (BIT(7)) } },
			{ CTRL_PORN_RST, 1, { HSDT_PERRSTEN0 }, { (BIT(7)) } },
			{ CTRL_SD_EXIT, 3, { PCIE_SRAM_CTRL_BUS_SP, PCIE_SRAM_CTRL_BUS_TP, GLB_SRAM_CTRL_BUS_EN },
				{ 0x00005858, 0x00000850, 0x1 } },
			{ SMMU_SD_EXIT, 1, { HSDT_SCTRL_SMMU_SD }, { BIT(1) | BIT(0) } },
			{ PIPE_CLK_STABLE, 1, { HIPCIE_CTRL_STAT2 }, { BIT(19) } },
			{ PU_RX_TERMIANTION, 1, { HIPCIE_PHY_CFG1 }, {BIT(0) | BIT(16)} },
			{ QIC_POWER_DOMAIN_ENTER, 2, { 0x424, 0x428 }, { BIT(4), 4 } },
			{ QIC_POWER_DOMAIN_EXIT, 2, { 0x424, 0x428 }, { BIT(4), 4 }},
		},
	}
};

#ifdef CONFIG_HIPCIE_IDLE
enum {
	OPEN_RFF_CLK = 0,
	CLOSE_RFF_CLK,
	OPEN_IO_CLK,
	CLOSE_IO_CLK,
	IDLE_BEHAVIOR_MAX,
};

struct hi_pcie_idle_behavior {
	u32 ctrl_id;
	u32 port_id;
	struct pcie_behavior behavior[IDLE_BEHAVIOR_MAX];
};

const struct hi_pcie_idle_behavior pcie_idle_step[] = {
	{ RC_ID0, 0,
		{
			{ OPEN_RFF_CLK, 1, { HSDT_PEREN1 }, { (BIT(2) | BIT(1)) } }, // debounce/hp/phy_ref hardware control
			{ CLOSE_RFF_CLK, 1, { HSDT_PERDIS1 }, { (BIT(2) | BIT(1)) } }, // debounce/hp/phy_ref hardware control
			// OPEN_IO_CLK/CLOSE_IO_CLK hardware control
		}
	}
};
#endif

static u32 xn_pcie_ao_crg_reg_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat_config.aocrg_base + reg);
}

static void xn_pcie_ao_crg_reg_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val: %x, reg: %x", __func__, val, reg);
	writel(val, plat_config.aocrg_base + reg);
}

static u32 xn_pcie_hsdt_crg_reg_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat_config.hsdtcrg_base + reg);
}

static void xn_pcie_hsdt_crg_reg_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val: %x, reg: %x", __func__, val, reg);
	writel(val, plat_config.hsdtcrg_base + reg);
}

static u32 xn_pcie_sysctrl_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat->sysctrl_base + reg);
}

static void xn_pcie_sysctrl_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val: %x, reg: %x", __func__, val, reg);
	writel(val, plat->sysctrl_base + reg);
}

static u32 xn_pcie_phy_pcs_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat->phy_pcs_base + reg);
}

static void xn_pcie_phy_pcs_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val: %x, reg: %x", __func__, val, reg);
	writel(val, plat->phy_pcs_base + reg);
}

static u32 xn_pcie_fnpll_reg_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat_config.pcie_fnpll_base + reg);
}

static void xn_pcie_fnpll_reg_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val: %x, reg: %x", __func__, val, reg);
	writel(val, plat_config.pcie_fnpll_base + reg);
}

static u32 xn_pcie_hsdt_sctrl_reg_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat_config.hsdtsctrl_base + reg);
}

static void xn_pcie_hsdt_sctrl_reg_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val %x, reg %x", __func__, val, reg);
	writel(val, plat_config.hsdtsctrl_base + reg);
}

static u32 xn_pcie_hsdt_xctrl_reg_readl(struct hi_pcie_xn *plat, u32 reg)
{
	return readl(plat_config.hsdtxctrl_base + reg);
}

static void xn_pcie_hsdt_xctrl_reg_writel(struct hi_pcie_xn *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, val %x, reg %x", __func__, val, reg);
	writel(val, plat_config.hsdtxctrl_base + reg);
}

static void xn_pcie_clk_config(struct hi_pcie_xn *plat)
{
	u32 val = 0;
	xn_pcie_ao_crg_reg_writel(plat, 0x003F0006, AOCRGSCCLKDIV3);

	// autofs open
	xn_pcie_ao_crg_reg_writel(plat, 0x07E0283F, AOCRGAO_ATUODIV2);
	xn_pcie_hsdt_crg_reg_writel(plat, 0x0, HSDT_CRG_PERI_AUTODIV3);

	// autofs enable
	val = xn_pcie_ao_crg_reg_readl(plat, AOCRGAO_ATUODIV2);
	val &= ~BIT(0);
	xn_pcie_ao_crg_reg_writel(plat, val, AOCRGAO_ATUODIV2);

	xn_pcie_hsdt_crg_reg_writel(plat, BIT(4), AOCRGSCPEREN6);
}

static void xn_pcie_qic_power_domain(struct hi_pcie_xn *plat, int behavior)
{
	u32 reg;
	u32 val_mask;
	u32 val = 0;
	u32 timeout = 100;
	u32 bitmask;

	val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[0];
	reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[0];
	if (behavior == QIC_POWER_DOMAIN_ENTER) {
		val |= (val_mask | (val_mask << 16));
		bitmask = 0x0;
	} else if (behavior == QIC_POWER_DOMAIN_EXIT) {
		val |= (val_mask << 16);
		bitmask = 0x1;
	} else {
		return;
	}
	xn_pcie_hsdt_xctrl_reg_writel(plat, val, reg);

	val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[1];
	reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[1];
	val = xn_pcie_hsdt_xctrl_reg_readl(plat, reg);
	while(((val >> val_mask) & 0x1) == bitmask) {
		udelay(10);
		if (timeout == 0) {
			hi_pcie_pr_e("%s, qic idle ack timeout, val 0x%x", __func__, val);
			return;
		}
		timeout--;
		val = xn_pcie_hsdt_xctrl_reg_readl(plat, reg);
	}
}

static void xn_pcie_rst_cfg(struct hi_pcie_xn *plat, u32 behavior)
{
	u32 val_mask;
	u32 reg;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		xn_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static void xn_pcie_sysclk_ctrl(struct hi_pcie_xn *plat, bool open)
{
	u32 val_mask;
	u32 reg;
	int behavior = open ? OPEN_SYSCLK : CLOSE_SYSCLK;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		xn_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}

	if (open) {
		// sysclk_hard/pcieio_hard
		xn_pcie_hsdt_crg_reg_writel(plat, BIT(6) | BIT(7), HSDT_PERDIS1);

		val_mask = xn_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIECTRL0);
		val_mask &= ~(BIT(0) | BIT(1)); // pcieio_hard
		xn_pcie_hsdt_crg_reg_writel(plat, val_mask, HSDT_CRG_PCIECTRL0);
	} else {
		val_mask = xn_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIECTRL0);
		val_mask |= (BIT(0) | BIT(1));
		xn_pcie_hsdt_crg_reg_writel(plat, val_mask, HSDT_CRG_PCIECTRL0);
	}
}

static void xn_pcie_sc_rst_cfg(struct hi_pcie_xn *plat, bool reset)
{
	u32 val_mask;
	u32 val;
	u32 reg;
	u32 behavior = reset ? SC_RST : SC_UNRST;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		val = xn_pcie_sysctrl_readl(plat, reg);
		val = val | val_mask;
		xn_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void xn_pcie_sc_clk_ctrl(struct hi_pcie_xn *plat, bool open)
{
	u32 val_mask;
	u32 val;
	u32 reg;
	u32 behavior = open ? EN_SC_CLK : DIS_SC_CLK;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		val = xn_pcie_sysctrl_readl(plat, reg);
		val = val | val_mask;
		xn_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void xn_pcie_fnpll_cfg_init(struct hi_pcie_xn *plat)
{
	xn_pcie_fnpll_reg_writel(plat, 0x000000CA, FNPLL_PCIE_ANALOG_CFG0);
	xn_pcie_fnpll_reg_writel(plat, 0x0000000C, FNPLL_PCIE_ANALOG_CFG1);
	xn_pcie_fnpll_reg_writel(plat, 0x020001FE, FNPLL_PCIE_ANALOG_CFG2);
	xn_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_ANALOG_CFG3);
	xn_pcie_fnpll_reg_writel(plat, 0x00354000, FNPLL_PCIE_ADPLL_ATUNE_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x026020A0, FNPLL_PCIE_ADPLL_PTUNE_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x00012D3F, FNPLL_PCIE_DTC_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x05366E1F, FNPLL_PCIE_FTDC_CFG);

	xn_pcie_fnpll_reg_writel(plat, 0x6420030A, FNPLL_PCIE_FDTC_LMS_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x5620163F, FNPLL_PCIE_DTC_LMS_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x20037015, FNPLL_PCIE_LPF_BW_I_T01);
	xn_pcie_fnpll_reg_writel(plat, 0x00003B25, FNPLL_PCIE_LPF_BW_P_T01);
	xn_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_ADPLL_DCO_TMODE);
	xn_pcie_fnpll_reg_writel(plat, 0x00010000, FNPLL_PCIE_ADPLL_LPF_TEST_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x00000003, FNPLL_PCIE_ADPLL_DSM_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x000AA0FF, FNPLL_PCIE_ADPLL_FTDC_LOCK_THR);
	xn_pcie_fnpll_reg_writel(plat, 0x0078103F, FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR);

	xn_pcie_fnpll_reg_writel(plat, 0x80780A0A, FNPLL_PCIE_CHAOS_CNT_THR_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x8848D00F, FNPLL_PCIE_ADPLL_CLK_EN_CFG0);
	xn_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x00100010, FNPLL_PCIE_DTC_LINEAR_TEST_MODE);
	xn_pcie_fnpll_reg_writel(plat, 0x0020A420, FNPLL_PCIE_FREZ_LMS_PTHR);
	xn_pcie_fnpll_reg_writel(plat, 0x01FE1FFF, FNPLL_PCIE_ACQ_CNT_THR_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x000FF102, FNPLL_PCIE_ADPLL_DLY_CFG);
	xn_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_PTUNE_FBDIV);

	udelay(1);
}

static void xn_pcie_fnpll_freq_init(struct hi_pcie_xn *plat)
{
	xn_pcie_hsdt_crg_reg_writel(plat, 0x00000002, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	xn_pcie_hsdt_crg_reg_writel(plat, 0x29AAAAAA, HSDT_CRG_PCIEPLL_PCIE_FCW);
}

static void xn_pcie_fnpll_clk_cfg(struct hi_pcie_xn *plat)
{
	u32 fnpll_val;
	unsigned long flag;

	spin_lock_irqsave(&pcie_fnpll_lock, flag);

	fnpll_val = xn_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	hi_pcie_pr_i("[%s]fnpll[0x%x]=0x%x", __func__, HSDT_CRG_PCIEPLL_PCIE_CTRL0, fnpll_val);
	if ((fnpll_val & PCIEPLL_STATE_MASK) != PCIEPLL_STATE_CLOSE) {
		hi_pcie_pr_i("FNPLL already locked ");
		goto FNPLL_END;
	}
	xn_pcie_fnpll_cfg_init(plat);
	xn_pcie_fnpll_freq_init(plat);

FNPLL_END:
	atomic_or((1 << plat->pcie->ctrl_id), &pcie_phy_clk_cnt);
	hi_pcie_pr_i("+%s+ pcie_phy_clk_cnt = %d ", __func__, atomic_read(&pcie_phy_clk_cnt));
	spin_unlock_irqrestore(&pcie_fnpll_lock, flag);
}

static int xn_pcie_phy_clk_cfg(struct hi_pcie_xn *plat)
{
	hi_pcie_pr_d("[%s] open pcie fnpll", __func__);

	xn_pcie_fnpll_clk_cfg(plat);

	if (plat->phy_ref_clk != NULL) {
		if (clk_prepare_enable(plat->phy_ref_clk)) {
			hi_pcie_pr_e("Failed to prepare phy_ref_clk");
			return -EINVAL;
		}
	}

	return 0;
}

static int xn_pcie_32k_clk_cfg(struct hi_pcie_xn *plat)
{
	u32 count_val;
	if (atomic_read(&(plat->pcie->is_power_on)) == 0)
		return 0;

	xn_pcie_fnpll_clk_cfg(plat);

	if (plat->phy_ref_clk != NULL) {
		count_val = __clk_get_enable_count(plat->phy_ref_clk);
		if (count_val > 0) {
			hi_pcie_pr_e("phy_ref_clk already enable: %u", count_val);
			return 0;
		}

		if (clk_enable(plat->phy_ref_clk)) {
			hi_pcie_pr_e("Failed to enable phy_ref_clk");
			return -EINVAL;
		}
	}

	return 0;
}

static void xn_pcie_fnpll_clk_uncfg(struct hi_pcie_xn *plat)
{
	unsigned long flag;
	spin_lock_irqsave(&pcie_fnpll_lock, flag);
	atomic_andnot(1 << plat->pcie->ctrl_id, &pcie_phy_clk_cnt);
	hi_pcie_pr_i("+%s+ pcie_phy_clk_cnt = %d ", __func__, atomic_read(&pcie_phy_clk_cnt));
	// EN=0 && BYPASS=1
	if (atomic_read(&pcie_phy_clk_cnt) == 0) 
		xn_pcie_hsdt_crg_reg_writel(plat, 0x00000002, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	spin_unlock_irqrestore(&pcie_fnpll_lock, flag);
}

static int xn_pcie_phy_clk_uncfg(struct hi_pcie_xn *plat)
{
	hi_pcie_pr_d("[%s] close pcie fnpll", __func__);

	if (plat->phy_ref_clk != NULL)
		clk_disable_unprepare(plat->phy_ref_clk);

	xn_pcie_fnpll_clk_uncfg(plat);

	return 0;
}

static int xn_pcie_32k_clk_uncfg(struct hi_pcie_xn *plat)
{
	if (atomic_read(&(plat->pcie->is_power_on)) == 0)
		return 0;

	if (plat->phy_ref_clk != NULL)
		clk_disable(plat->phy_ref_clk);

	xn_pcie_fnpll_clk_uncfg(plat);

	return 0;
}

static void xn_pcie_oe_ie_cfg(struct hi_pcie_xn *plat, u32 behavior)
{
	u32 val;
	u32 val_mask;
	u32 reg;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		val = xn_pcie_hsdt_crg_reg_readl(plat, reg);
		val = val | val_mask;
		if (plat->pcie->mode == PCIE_MODE_RC)
			val &= 0xCFFFF5BF;
		else
			val &= 0xC7FFFFFF;
		xn_pcie_hsdt_crg_reg_writel(plat, val, reg);
	}
}

static void xn_pcie_clkreq_debounce_cfg(struct hi_pcie_xn *plat)
{
	// 2.7.1.2 cfg clkreq debounce
	xn_pcie_sysctrl_writel(plat, 0x80000240, SC_PCIECTRL_CTRL31);

	// 2.7.1.3 delay send controller
	xn_pcie_sysctrl_writel(plat, BIT(20) | BIT(4), CLKREQ_IN_N_CTRL);
}

static void xn_pcie_phy_rst_iso(struct hi_pcie_xn *plat, bool enable)
{
	u32 val;
	u32 reg = SOC2PHY_ISO_EN;

	if (!enable) {
		val = xn_pcie_sysctrl_readl(plat, reg);
		val = val & 0xFFFFFFFC;
		xn_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void xn_pcie_do_phy_reset(struct hi_pcie_xn *plat, u32 behavior, bool reset)
{
	u32 reg;
	u32 val_mask;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		if (reset)
			val_mask <<= 16;
		else
			val_mask |= (val_mask << 16);

		xn_pcie_sysctrl_writel(plat, val_mask, reg);
	}
}

static void xn_pcie_phy_apb_reset(struct hi_pcie_xn *plat, u32 behavior)
{
	u32 reg;
	u32 val_mask;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		xn_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static void xn_pcie_phy_refclk_sel(struct hi_pcie_xn *plat)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = plat->ctrl;

	if (ctrl->pcie->mode == PCIE_MODE_RC)
		return;

	val = xn_pcie_sysctrl_readl(plat, HIPCIE_PHY_CFG0);
	val |= BIT(26) | BIT(10);
	xn_pcie_sysctrl_writel(plat, val, HIPCIE_PHY_CFG0);
}

static void xn_pcie_phy_cfg_x1(struct hi_pcie_xn *plat)
{
	struct hi_pcie_phy *phy = &plat->pcie->phy;

	pcie_kphy_setup(phy);

	if (plat->pcie->phy_dfx_ops && plat->pcie->phy_dfx_ops->phy_param_change)
		plat->pcie->phy_dfx_ops->phy_param_change(phy);

	xn_pcie_do_phy_reset(plat, PHY_RESET_N, false); // 2.9.6
	xn_pcie_do_phy_reset(plat, PIPE_RESET_N, false); // 2.9.7
}

static void xn_pcie_phy_init(struct hi_pcie_xn *plat)
{
	xn_pcie_do_phy_reset(plat, PHY_RST_N_SEL, false); // 2.9.1
	xn_pcie_do_phy_reset(plat, PHY_PORN_RST, false); // 2.9.2
	xn_pcie_phy_apb_reset(plat, PHY_APB_UNRESET); // 2.9.3
	xn_pcie_phy_refclk_sel(plat); // 2.9.4

	xn_pcie_phy_cfg_x1(plat);
}

static void xn_pcie_reset_phy(struct hi_pcie_xn *plat)
{
	xn_pcie_do_phy_reset(plat, PIPE_RESET_N, true);
	xn_pcie_do_phy_reset(plat, PHY_RESET_N, true);
	xn_pcie_phy_apb_reset(plat, PHY_APB_RESET);
	xn_pcie_do_phy_reset(plat, PHY_PORN_RST, true);
}

static int xn_pcie_perst_from_gpio(struct hi_pcie_xn *plat, int pull_up)
{
	int ret = 0;
	struct hi_pcie_port *port = NULL;
	list_for_each_entry(port, &plat->pcie->ports, node) {
		if (pull_up && port->poweron_callback)
			ret = port->poweron_callback(port->power_callback_data);
		else if (!pull_up && port->poweroff_callback)
			ret = port->poweroff_callback(port->power_callback_data);
		if (ret != 0)
			return ret;
	}
	return 0;
}

static int xn_pcie_perst_config(struct hi_pcie_xn *plat, int pull_up)
{
	int ret;

	if (plat->pcie->mode == PCIE_MODE_EP)
		return 0;

	if (pull_up)
		usleep_range(10, 11);

	ret = xn_pcie_perst_from_gpio(plat, pull_up);
	if (ret)
		hi_pcie_pr_e("Failed to pulse perst signal");

	usleep_range(10, 11);
	return ret;
}

static void xn_pcie_ctrl_por_n_rst(struct hi_pcie_xn *plat, bool reset)
{
	u32 val_mask;
	u32 reg;
	u32 behavior = reset ? CTRL_PORN_RST : CTRL_PORN_UNRST;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		xn_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static void xn_pcie_ctrl_memsd_exit(struct hi_pcie_xn *plat, int behavior)
{
	u32 reg;
	u32 val_mask;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		xn_pcie_sysctrl_writel(plat, val_mask, reg);
	}
}

static void xn_pcie_smmu_memsd_exit(struct hi_pcie_xn *plat, u32 behavior, bool enable)
{
	u32 reg;
	u32 val_mask;
	u32 val;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		val = 0;
		if (enable)
			val &= ~val_mask;
		else
			val |= val_mask;
		val |= (val_mask << 16);

		xn_pcie_hsdt_sctrl_reg_writel(plat, val, reg);
	}
}

static void xn_pcie_mem_sd_exit(struct hi_pcie_xn *plat, bool enable)
{
	xn_pcie_ctrl_memsd_exit(plat, CTRL_SD_EXIT);
	xn_pcie_smmu_memsd_exit(plat, SMMU_SD_EXIT, enable);
}

static bool xn_pcie_is_pipe_clk_stable(struct hi_pcie_xn *plat, u32 behavior)
{
	u32 reg;
	u32 val_mask;
	u32 val;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;
	u32 timeout = 100; /* try 100 times */

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		val = xn_pcie_sysctrl_readl(plat, reg);

		while((val & val_mask) != 0) {
			mdelay(1);
			if (timeout == 0) {
				hi_pcie_pr_e("PIPE clk is not stable, val = 0x%x", val);
				pcie_dsm_record_pipe_status(val);
				return false;
			}
			timeout--;
			val = xn_pcie_sysctrl_readl(plat, reg);
		}
	}

	hi_pcie_pr_d("PIPE clk is stable in (1000-%u)*100 us", timeout);
	return true;
}

static void xn_pcie_rx_term_cfg(struct hi_pcie_xn *plat, int behavior)
{
	u32 step;
	u32 val_mask;
	u32 reg;
	u32 val;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		val = xn_pcie_sysctrl_readl(plat, reg);
		val = val | val_mask;
		xn_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void xn_pcie_firewall_fix(struct hi_pcie_xn *plat)
{
#ifdef CONFIG_HIPCIE_SUPPORT_FFA
	int ret;
	struct ffa_send_direct_data args = {0};

	args.data0 = (u64)FID_PCIE_SET_MEM_UNSEC;
	args.data1 = (u64)plat->pcie->ctrl_id;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		hi_pcie_pr_e("fix firewall failed");
		return;
	}
#else
	struct arm_smccc_res res;
	arm_smccc_smc((u64)FID_PCIE_SET_MEM_UNSEC, (u64)plat->pcie->ctrl_id,
		0, 0, 0, 0, 0, 0, &res);
#endif
	hi_pcie_pr_e("fix firewall");
}

static noinline uint64_t xn_pcie_tbu_bypass(struct hi_pcie_xn *plat)
{
#ifdef CONFIG_HIPCIE_SUPPORT_FFA
	int ret;
	struct ffa_send_direct_data args = {0};
 
	args.data0 = (u64)FID_PCIE_SET_TBU_BYPASS;
	args.data1 = (u64)plat->pcie->ctrl_id;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		hi_pcie_pr_e("Bypass TBU failed, ret = %d", ret);
	}
	hi_pcie_pr_e("Bypass TBU");
	return args.data0;
#else
	struct arm_smccc_res res;
	arm_smccc_smc((u64)FID_PCIE_SET_TBU_BYPASS, (u64)plat->pcie->ctrl_id,
		0, 0, 0, 0, 0, 0, &res);
	hi_pcie_pr_e("Bypass TBU");
	return res.a0;
#endif
}

static bool xn_pcie_is_smmu_bypass(struct hi_pcie_xn *plat)
{
	if (IS_ENABLED(CONFIG_MM_IOMMU_DMA) && !plat->pcie->bypass_smmu)
		return false;
	return true;
}

#define TBU_CONFIG_TIMEOUT			1000
#define PCIE_TBU_CR					0x0
#define PCIE_TBU_CRACK				0x4
#define PCIE_TBU_EN_REQ_BIT			BIT(0)
#define PCIE_TBU_EN_ACK_BIT			BIT(0)
#define PCIE_TBU_CONNCT_STATUS_BIT	BIT(1)
#define PCIE_TBU_TOK_TRANS_MSK		(0xFF << 8)
#define PCIE_TBU_TOK_TRANS			0x8
#define PCIE_TBU_DEFALUT_TOK_TRANS	0xF

static void xn_pcie_init_smmu_map(struct hi_pcie_xn *plat)
{
	u32 reg_val;
	u32 cdx = plat->pcie->ctrl_id;

	reg_val = xn_pcie_sysctrl_readl(plat, xn_pcie_smmu[cdx].mode_reg);
	reg_val |= BIT(0);
	xn_pcie_sysctrl_writel(plat, reg_val, xn_pcie_smmu[cdx].mode_reg);

	reg_val = xn_pcie_sysctrl_readl(plat, xn_pcie_smmu[cdx].aw_reg);
	reg_val |= xn_pcie_smmu[cdx].sid << 8;
	reg_val |= 1 << 28;
	xn_pcie_sysctrl_writel(plat, reg_val, xn_pcie_smmu[cdx].aw_reg);

	reg_val = xn_pcie_sysctrl_readl(plat, xn_pcie_smmu[cdx].ar_reg);
	reg_val |= xn_pcie_smmu[cdx].sid << 8;
	reg_val |= 1 << 28;
	xn_pcie_sysctrl_writel(plat, reg_val, xn_pcie_smmu[cdx].ar_reg);
}

static int xn_pcie_tbu_config(struct hi_pcie_xn *plat, u32 enable)
{
	u32 reg_val;
	u32 tok_trans_gnt;
	u32 time = TBU_CONFIG_TIMEOUT;

	xn_pcie_init_smmu_map(plat);

	reg_val = readl(plat->tbu_base + PCIE_TBU_CR); // enable req
	if (enable)
		reg_val |= PCIE_TBU_EN_REQ_BIT;
	else
		reg_val &= ~PCIE_TBU_EN_REQ_BIT;
	writel(reg_val, plat->tbu_base + PCIE_TBU_CR);

	reg_val = readl(plat->tbu_base + PCIE_TBU_CRACK);
	while ((reg_val & PCIE_TBU_EN_ACK_BIT) == 0) {
		udelay(1);
		if (time == 0) {
			hi_pcie_pr_e("TBU req(en/dis) not been acknowledged");
			return -1;
		}
		time--;
		reg_val = readl(plat->tbu_base + PCIE_TBU_CRACK);
	}

	reg_val = readl(plat->tbu_base + PCIE_TBU_CRACK);
	if (enable) {
		if ((reg_val & PCIE_TBU_CONNCT_STATUS_BIT) == 0) {
			hi_pcie_pr_e("TBU connecting failed");
			return -1;
		}
	} else {
		if ((reg_val & PCIE_TBU_CONNCT_STATUS_BIT) != 0) {
			hi_pcie_pr_e("TBU is disconnect from TCU fail");
			return -1;
		}
	}

	if (enable) {
		reg_val = readl(plat->tbu_base + PCIE_TBU_CRACK);
		tok_trans_gnt = (reg_val & PCIE_TBU_TOK_TRANS_MSK) >> PCIE_TBU_TOK_TRANS;
		if (tok_trans_gnt < PCIE_TBU_DEFALUT_TOK_TRANS) {
			hi_pcie_pr_e("tok_trans_gnt[%d] is less than setting", tok_trans_gnt);
			return -1;
		}
	}

	return 0;
}

static u32 xn_pcie_get_smmuid(struct hi_pcie *pcie)
{
	return xn_pcie_smmu[pcie->ctrl_id].smmu_id;
}
static atomic_t *xn_pcie_get_tcu_pwrcnt(struct hi_pcie *pcie)
{
	return xn_pcie_smmu[pcie->ctrl_id].tcu_pwr_cnt;
}

static int xn_pcie_smmu_tcu_cfg(struct hi_pcie_xn *plat, bool enable)
{
	int ret = 0;
	atomic_t *tcu_pwr_cnt = xn_pcie_get_tcu_pwrcnt(plat->pcie);
 
	if(enable) {
		if (atomic_read(tcu_pwr_cnt) == 0) {
			ret = mm_smmu_poweron(plat->pcie->dev);
			if (ret) {
				hi_pcie_pr_e("TCU power on Failed");
				goto CFG_END;
			}
		}
		atomic_or((1 << plat->pcie->ctrl_id), tcu_pwr_cnt);
	} else {
		atomic_andnot(1 << plat->pcie->ctrl_id, tcu_pwr_cnt);
		if (atomic_read(tcu_pwr_cnt) == 0) {
			ret = mm_smmu_poweroff(plat->pcie->dev);
			if (ret) {
				hi_pcie_pr_e("TCU power off failed");
				goto CFG_END;
			}	
		}
	}
 
CFG_END:
	hi_pcie_pr_e("tcu_pwr_cnt=%d, enable=%d", *tcu_pwr_cnt, enable);
	return ret;
}

static int xn_pcie_enable_smmu(struct hi_pcie_xn *plat)
{
	int ret;
	u32 smmu_id = xn_pcie_get_smmuid(plat->pcie);

	ret = mm_smmu_nonsec_tcuctl(smmu_id);
	if (ret) {
		hi_pcie_pr_e("TCU cfg Failed");
		return -1;
	}

	ret = xn_pcie_smmu_tcu_cfg(plat, true);
	if(ret)
		return ret;

	ret = xn_pcie_tbu_config(plat, ENABLE);
	if (ret) {
		hi_pcie_pr_e("TBU config Failed");
		return -1;
	}
	hi_pcie_pr_e("TBU init Done");

	return 0;
}

static int xn_pcie_disable_smmu(struct hi_pcie_xn *plat)
{
	atomic_t *tcu_pwr_cnt;
	if (xn_pcie_is_smmu_bypass(plat)) {
		hi_pcie_pr_e("smmu bypassed");
		return 0;
	}

	tcu_pwr_cnt = xn_pcie_get_tcu_pwrcnt(plat->pcie);
	if ((atomic_read(tcu_pwr_cnt) && BIT(plat->pcie->ctrl_id)) == 0)
		return 0;

	(void)xn_pcie_tbu_config(plat, DISABLE);

	return xn_pcie_smmu_tcu_cfg(plat, false);
}

static int xn_pcie_smmu_init(struct hi_pcie_xn *plat)
{
	if (xn_pcie_is_smmu_bypass(plat)) {
		if (xn_pcie_tbu_bypass(plat)) {
			hi_pcie_pr_e("Bypass TBU failed");
			return -1;
		}

		return 0;
	}

	return xn_pcie_enable_smmu(plat);
}

static int xn_pcie_parse_base_config(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	int ret;

	ret = device_property_read_u32(&pdev->dev, "clkreq_gpio", &plat->clkreq_gpio);
	if (ret != 0)
		hi_pcie_pr_e("clkreq_gpio not find");

	hi_pcie_pr_d("clkreq_gpio = %u", plat->clkreq_gpio);

	plat->phy_ref_clk = devm_clk_get(&pdev->dev, "phy_ref_clk");
	if (IS_ERR(plat->phy_ref_clk)) {
		hi_pcie_pr_e("Failed to get pcie_aclk clock");
		return (int)PTR_ERR(plat->phy_ref_clk);
	}

	return 0;
}

static int xn_pcie_parse_pcie_base(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	struct resource *phy_pcs;
	struct resource *sysctrl;
	struct resource *tbu;

	phy_pcs = platform_get_resource_byname(pdev, IORESOURCE_MEM, "phy");
	plat->phy_pcs_base = devm_ioremap_resource(&pdev->dev, phy_pcs);
	if (IS_ERR(plat->phy_pcs_base)) {
		hi_pcie_pr_e("Failed to get PCIe pcs base");
		return -1;
	}

	plat->pcie->phy.phy_base = plat->phy_pcs_base;
	plat->pcie->phy.lane_num = 1;
	plat->pcie->phy.link_num = plat->pcie->port_num;

	sysctrl = platform_get_resource_byname(pdev, IORESOURCE_MEM, "apb");
	plat->sysctrl_base = devm_ioremap_resource(&pdev->dev, sysctrl);
	if (IS_ERR(plat->sysctrl_base)) {
		hi_pcie_pr_e("Failed to get PCIe sysctrl base");
		return -1;
	}

	tbu = platform_get_resource_byname(pdev, IORESOURCE_MEM, "tbu");
	plat->tbu_base = devm_ioremap_resource(&pdev->dev, tbu);
	if (IS_ERR(plat->tbu_base)) {
		hi_pcie_pr_e("Failed to get PCIe tbu base");
		return -1;
	}

	return 0;
}

static int xn_pcie_parse_plat_base(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	int ret;
	struct device_node *np;
	int i;
	static struct {
		void __iomem **base;
		char *compatible;
		u32 index;
	} plat_base_list[] = {
		{ &plat_config.aocrg_base, "hisilicon,ao_crg", 0 },
		{ &plat_config.hsdtcrg_base, "hisilicon,hsdt_crg", 0 },
		{ &plat_config.hsdtsctrl_base, "hisilicon,hsdt_sysctrl", 0 },
		{ &plat_config.hsdtxctrl_base, "hisilicon,hsdt_ctrl", 0 },
		{ &plat_config.pcie_fnpll_base, "hi_pcie,plat_base", 0 },
	};

	for (i = 0; i < ARRAY_SIZE(plat_base_list); i++) {
		np = of_find_compatible_node(NULL, NULL, plat_base_list[i].compatible);
		if (!np) {
			hi_pcie_pr_e("Failed to get %s node", plat_base_list[i].compatible);
			goto unmap_base;
		}
		*plat_base_list[i].base = of_iomap(np, plat_base_list[i].index);
		if (*plat_base_list[i].base == NULL) {
			hi_pcie_pr_e("Failed to iomap %s-%u", plat_base_list[i].compatible,
				plat_base_list[i].index);
			goto unmap_base;
		}
	}

	return 0;

unmap_base:
	for (i = 0; i < ARRAY_SIZE(plat_base_list); i++) {
		if (*plat_base_list[i].base != NULL) {
			iounmap(*plat_base_list[i].base);
			*plat_base_list[i].base = NULL;
		}
	}
	return -1;
}

static int xn_pcie_parse_phy_param(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	int ret;
	struct device_node *node;
	u32 size = 0;
	u32 *reg = NULL;
	u32 *val = NULL;

	node = of_find_compatible_node(plat->pcie->dev->of_node, NULL, "pcie0,kphy");
	if (node == NULL) {
		hi_pcie_pr_e("get plat(%s) node failed", "pcie0,kphy");
		return -EINVAL;
	}

	ret = of_property_read_u32(node, "change-param-size", &size);
	if ((ret != 0) || size == 0) {
		hi_pcie_pr_e("need not change phy param, ret = %d, size = %u", ret, size);
		return 0;
	}
	hi_pcie_pr_d("get phy param size = %u", size);

	plat->pcie->phy.phy_change.change_param_size = size;
	reg = (u32 *)devm_kzalloc(&pdev->dev, sizeof(u32) * size, GFP_KERNEL);
	if (!reg) {
		hi_pcie_pr_e("reg malloc failed");
		return -ENOMEM;
	}
	val = (u32 *)devm_kzalloc(&pdev->dev, sizeof(u32) * size, GFP_KERNEL);
	if (!val) {
		hi_pcie_pr_e("val malloc failed");
		goto free_reg;
	}

	ret = of_property_read_u32_array(node, "change-param", val, size);
	if (ret) {
		hi_pcie_pr_e("get change phy param failed, ret = %d", ret);
		goto free_val;
	}

	ret = of_property_read_u32_array(node, "change-param-reg", reg, size);
	if (ret) {
		hi_pcie_pr_e("get change phy param reg failed, ret = %d", ret);
		goto free_val;
	}

	plat->pcie->phy.phy_change.change_param = val;
	plat->pcie->phy.phy_change.change_param_addr = reg;

	return ret;

free_val:
	devm_kfree(&pdev->dev, val);

free_reg:
	devm_kfree(&pdev->dev, reg);
	return ret;
}

static int xn_pcie_parse_plat_config(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	int ret;

	ret = xn_pcie_parse_base_config(pdev, plat);
	if (ret != 0)
		return ret;

	ret = xn_pcie_parse_pcie_base(pdev, plat);
	if (ret != 0)
		return ret;

	ret = xn_pcie_parse_plat_base(pdev, plat);
	if (ret != 0)
		return ret;

	xn_pcie_parse_phy_param(pdev, plat);

	return 0;
}

static irqreturn_t xn_pcie_pcs_handler(int irq, void *arg)
{
	hi_pcie_pr_d("pcie plat pcs handler enter");
	return IRQ_HANDLED;
}

#define AXI_TIMEOUT_MASK	BIT(11)
#define AXI_TIMEOUT_CLR		BIT(9)
#define AXI_TIMEOUT_STATUS	BIT(19)

static irqreturn_t xn_pcie_level2_handler(int irq, void *arg)
{
	u32 val;
	struct hi_pcie_xn *plat = arg;

	if (atomic_read(&(plat->pcie->is_power_on)) == 0) {
		hi_pcie_pr_d("PCIe%u already power down, not support irq handler", plat->pcie->ctrl_id);
		return IRQ_HANDLED;
	}

	hi_pcie_pr_e("pcie plat level2 handler enter");

	val = xn_pcie_sysctrl_readl(plat, HIPCIE_MISC_STATE0);
	if ((val & AXI_TIMEOUT_STATUS) == AXI_TIMEOUT_STATUS)
		hi_pcie_pr_e("pcie axi timeout happended");

	val = xn_pcie_sysctrl_readl(plat, HIPCIE_PHY_CFG0);
	val |= AXI_TIMEOUT_MASK | AXI_TIMEOUT_CLR | (AXI_TIMEOUT_MASK << 16) | (AXI_TIMEOUT_CLR << 16);
	xn_pcie_sysctrl_writel(plat, val, HIPCIE_PHY_CFG0);

	hi_pcie_handle_int(plat->ctrl);

	val = xn_pcie_sysctrl_readl(plat, HIPCIE_PHY_CFG0);
	val &= ~AXI_TIMEOUT_MASK;
	xn_pcie_sysctrl_writel(plat, val, HIPCIE_PHY_CFG0);

	return IRQ_HANDLED;
}

static int xn_pcie_register_int(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	int irq;
	int ret;

	irq = platform_get_irq(pdev, INT_INDEX_PCS);
	if (irq <= 0) {
		hi_pcie_pr_e("get pcs irq-%dfailed", irq);
		return -1;
	}
	ret = devm_request_irq(&pdev->dev, irq, xn_pcie_pcs_handler,
			(unsigned long)IRQF_TRIGGER_HIGH, "pcie_pcs", plat);
	if (ret != 0) {
		hi_pcie_pr_e("request pcs irq-%dfailed", irq);
		return ret;
	}
	disable_irq((u32)irq);

	irq = platform_get_irq(pdev, INT_INDEX_LEVEL2);
	if (irq <= 0) {
		hi_pcie_pr_e("get level2 irq-%dfailed", irq);
		return -1;
	}
	ret = devm_request_irq(&pdev->dev, irq, xn_pcie_level2_handler,
			(unsigned long)IRQF_TRIGGER_HIGH, "pcie_level2", plat);
	if (ret != 0) {
		hi_pcie_pr_e("request level2 irq-%dfailed", irq);
		return ret;
	}
	plat->level2_irq = irq;
	disable_irq((u32)irq);

	return 0;
}

static void xn_pcie_phy_register(struct hi_pcie_xn *plat)
{
	pcie_kphy_dfx_register(plat->pcie);
}

static int xn_pcie_pwron_callback(void *data)
{
	struct hi_pcie_port *port = data;
	return gpio_direction_output(port->reset_gpio, ENABLE);
}

static int xn_pcie_pwroff_callback(void *data)
{
	struct hi_pcie_port *port = data;
	return gpio_direction_output(port->reset_gpio, DISABLE);
}

static void xn_pcie_pwr_callback_register(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable)
			continue;
		if (port->ep_device_type != PCIE_DEVICE_HI110X) {
			port->poweron_callback = xn_pcie_pwron_callback;
			port->poweroff_callback = xn_pcie_pwroff_callback;
			port->power_callback_data = port;
		}
	}
}

static int xn_pcie_pinctrl(struct platform_device *pdev, struct hi_pcie_xn *plat)
{
	plat->pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(plat->pinctrl)) {
		hi_pcie_pr_e("pinctrl get failed\n");
		return -1;
	}

	plat->pinctrls_default = pinctrl_lookup_state(plat->pinctrl, PINCTRL_STATE_DEFAULT);
	if (IS_ERR(plat->pinctrls_default))
		hi_pcie_pr_e("could not get default pinstate\n");

	plat->pinctrls_idle = pinctrl_lookup_state(plat->pinctrl, PINCTRL_STATE_IDLE);
	if (IS_ERR(plat->pinctrls_idle))
		hi_pcie_pr_e("could not get sleep pinstate\n");

	return 0;
}

static int xn_pcie_init(struct platform_device *pdev, struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_xn *plat;

	hi_pcie_pr_d("+%s+", __func__);

	plat = devm_kzalloc(&pdev->dev, sizeof(struct hi_pcie_xn), GFP_KERNEL);
	if (plat == NULL)
		return -ENOMEM;

	pcie->plat = (void *)plat;
	plat->pcie = pcie;
	plat->ctrl = pcie->ctrl;

	ret = xn_pcie_parse_plat_config(pdev, plat);
	if (ret != 0) {
		devm_kfree(&pdev->dev, plat);
		return ret;
	}

	if (plat->pcie->mode == PCIE_MODE_RC) {
		ret = xn_pcie_register_int(pdev, plat);
		if (ret != 0) {
			devm_kfree(&pdev->dev, plat);
			return ret;
		}
	}

	xn_pcie_phy_register(plat);

	xn_pcie_pwr_callback_register(plat->pcie);

	ret = xn_pcie_pinctrl(pdev, plat);
	if (ret) {
		hi_pcie_pr_e("pinctrl failed\n");
		return ret;
	}

	hi_pcie_pr_d("-%s-", __func__);
	return ret;
}

static void xn_pcie_phy_recfg_controller_ffe(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	u32 val;
	u32 step;

	static struct pcie_phy_v48_one_table {
		u32 prest_cm1;
		u32 prest_c0;
		u32 prest_c1;
		u32 reg;
	} v48_one_table[] = {
		{ 0, 47, 16, MAC_REG_PRESET0 },
		{ 0, 52, 11, MAC_REG_PRESET1 },
		{ 0, 50, 13, MAC_REG_PRESET2 },
		{ 0, 55, 8, MAC_REG_PRESET3 },
		{ 0, 63, 0, MAC_REG_PRESET4 },
		{ 6, 57, 0, MAC_REG_PRESET5 },
		{ 8, 55, 0, MAC_REG_PRESET6 },
		{ 6, 44, 13, MAC_REG_PRESET7 },
		{ 8, 47, 8, MAC_REG_PRESET8 },
		{ 11, 52, 0, MAC_REG_PRESET9 },
		{ 0, 42, 21, MAC_REG_PRESET10 },
		{ 0, 63, 0, MAC_REG_PRESET11 },
		{ 0, 63, 0, MAC_REG_PRESET12 },
		{ 0, 63, 0, MAC_REG_PRESET13 },
		{ 0, 63, 0, MAC_REG_PRESET14 },
		{ 0, 63, 0, MAC_REG_PRESET15 },
	};

	list_for_each_entry(port, &pcie->ports, node) {
		/* coeff table set 0 */
		val = pcie_ctrl_mac_readl(port->priv, MAC_REG_EQ_INIT_COEFF_CFG);
		val = val & 0xF0FFFFFF;
		pcie_ctrl_mac_writel(port->priv, val, MAC_REG_EQ_INIT_COEFF_CFG);
		/* recfg table 1 */
		for (step = 0; step < ARRAY_SIZE(v48_one_table); step++) {
			val = pcie_ctrl_mac_readl(port->priv, v48_one_table[step].reg);
			val &= 0xFFC0C0C0;
			val = val | v48_one_table[step].prest_cm1 |
				(v48_one_table[step].prest_c0 << 8) |
				(v48_one_table[step].prest_c1 << 16);

			pcie_ctrl_mac_writel(port->priv, val, v48_one_table[step].reg);
		}
	}
}

static int xn_pcie_phy_chk_recfg_ffe(struct hi_pcie_port *port)
{
	int ret;
	struct hi_pcie_phy *phy = &port->pcie->phy;

	if (port->pcie->chip_type == BOARD_FPGA)
		return 0;

	ret = pcie_kphy_check_status(phy);
	if (ret) {
		hi_pcie_pr_e("phy check fail");
		pcie_kphy_dump_status(phy);
		return -1;
	}

	hi_pcie_pr_e("phy check succ");

	ret = pcie_kphy_cfg_ffe(phy);
	if (ret) {
		hi_pcie_pr_e("need to recfg ffe");
		xn_pcie_phy_recfg_controller_ffe(port->pcie);
	}

	return 0;
}

static void xn_pcie_axi_timeout_set(struct hi_pcie_xn *plat)
{
	u32 val;

	val = xn_pcie_sysctrl_readl(plat, HIPCIE_MISC_CFG9_V3);
	val &= ~(0x3F << 16);
	val |= (0x2F << 16);
	xn_pcie_sysctrl_writel(plat, val, HIPCIE_MISC_CFG9_V3);
}

static int xn_pcie_turn_on(struct hi_pcie *pcie)
{
	int ret;
	unsigned long flags = 0;

	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;
	hi_pcie_pr_d("+%s+", __func__);

	if (atomic_read(&(pcie->is_power_on))) {
		hi_pcie_pr_e("PCIe%u already power on", pcie->ctrl_id);
		return 0;
	}

	ret = pinctrl_select_state(plat->pinctrl, plat->pinctrls_default);
	if (ret)
		hi_pcie_pr_e("can not set pins to default state");

	xn_pcie_clk_config(plat);

	xn_pcie_qic_power_domain(plat, QIC_POWER_DOMAIN_ENTER);

	xn_pcie_rst_cfg(plat, PCIE_RST);  // 2.5.2
	xn_pcie_rst_cfg(plat, PCIE_UNRST); // 2.5.3
	xn_pcie_sysclk_ctrl(plat, true); // 2.5.4

	xn_pcie_sc_rst_cfg(plat, false); // 2.5.6
	xn_pcie_sc_clk_ctrl(plat, true); // 2.5.7

	xn_pcie_phy_clk_cfg(plat); // 2.5.8

	if (plat->pcie->mode == PCIE_MODE_RC)  // 2.5.9
		xn_pcie_oe_ie_cfg(plat, OE_IE_CFG_RC);
	else
		xn_pcie_oe_ie_cfg(plat, OE_IE_CFG_EP);

	xn_pcie_clkreq_debounce_cfg(plat); // 2.7
	xn_pcie_phy_rst_iso(plat, DISABLE); // 2.8

	xn_pcie_phy_init(plat); // 2.9

	ret = xn_pcie_perst_config(plat, ENABLE); // 2.11
	if (ret != 0) {
		hi_pcie_pr_e("power on perst fail");
		goto perst_fail;
	}

	xn_pcie_ctrl_por_n_rst(plat, false); // 2.12

	xn_pcie_mem_sd_exit(plat, ENABLE); // 2.13

	xn_pcie_axi_timeout_set(plat);

	flags = hi_pcie_idle_spin_lock(pcie->idle, flags);
	atomic_add(1, &(pcie->is_power_on));
	hi_pcie_idle_spin_unlock(pcie->idle, flags);

	pcie_ctrl_enable_irq(pcie->ctrl);
	enable_irq(plat->level2_irq);
	hi_pcie_idle_mask_int(pcie->idle);

	hi_pcie_pr_d("-%s-", __func__);
	return 0;

perst_fail:
	xn_pcie_sc_clk_ctrl(plat, false);
 
	xn_pcie_reset_phy(plat);
 
	xn_pcie_sc_rst_cfg(plat, true);

	xn_pcie_sysclk_ctrl(plat, false);
	xn_pcie_rst_cfg(plat, PCIE_RST);
 
	ret = pinctrl_select_state(plat->pinctrl, plat->pinctrls_idle);
	if (ret)
		hi_pcie_pr_e("can not set pins to sleep state");
 
	ret = gpio_request(plat->clkreq_gpio, "clkreq_gpio");
	if (ret)
		hi_pcie_pr_e("clkreq_gpio request fail");
	ret = gpio_direction_input(plat->clkreq_gpio);
	if (ret)
		hi_pcie_pr_e("clkreq_gpio direction input fail");
	gpio_free(plat->clkreq_gpio);
 
	xn_pcie_phy_clk_uncfg(plat);
	return -1;
}

static int xn_pcie_turn_off(struct hi_pcie *pcie)
{
	int ret;
	unsigned long flags = 0;

	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;
	hi_pcie_pr_d("+%s+", __func__);

	if (atomic_read(&(pcie->is_power_on)) == 0) {
		hi_pcie_pr_e("PCIe%u already power down", pcie->ctrl_id);
		return 0;
	}

	hi_pcie_idle_mask_int(pcie->idle);
	hi_pcie_idle_clear_irq(pcie->idle);
	udelay(5);

	pcie_ctrl_disable_irq(pcie->ctrl);
	disable_irq(plat->level2_irq);

	hi_pcie_idle_disable_irq(pcie->idle);

	pcie_core_flush_work(pcie);

	flags = hi_pcie_idle_spin_lock(pcie->idle, flags);
	atomic_sub(1, &(pcie->is_power_on));
	hi_pcie_idle_spin_unlock(pcie->idle, flags);

	hi_pcie_32k_idle_exit(pcie->idle);

	xn_pcie_qic_power_domain(plat, QIC_POWER_DOMAIN_ENTER);

	ret = pcie_ctrl_reset(pcie->ctrl);
	if (ret)
		return ret;

	xn_pcie_disable_smmu(plat);

	xn_pcie_ctrl_por_n_rst(plat, true);

	xn_pcie_sc_clk_ctrl(plat, false);

	xn_pcie_reset_phy(plat);

	xn_pcie_sc_rst_cfg(plat, true);

	ret = xn_pcie_perst_config(plat, DISABLE);
	if (ret != 0)
		hi_pcie_pr_e("power off perst fail");

	xn_pcie_phy_clk_uncfg(plat);

	xn_pcie_mem_sd_exit(plat, DISABLE);

	xn_pcie_sysclk_ctrl(plat, false);

	xn_pcie_rst_cfg(plat, PCIE_RST);

	ret = pinctrl_select_state(plat->pinctrl, plat->pinctrls_idle);
	if (ret)
		hi_pcie_pr_e("can not set pins to sleep state");

	ret = gpio_request(plat->clkreq_gpio, "clkreq_gpio");
	if (ret)
		hi_pcie_pr_e("clkreq_gpio request fail");
	ret = gpio_direction_input(plat->clkreq_gpio);
	if (ret)
		hi_pcie_pr_e("clkreq_gpio direction input fail");
	gpio_free(plat->clkreq_gpio);

	hi_pcie_pr_d("-%s-", __func__);
	return 0;
}

static int xn_pcie_start_link(struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;
	struct hi_pcie_port *port = NULL;
	hi_pcie_pr_d("+%s+", __func__);

	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable)
			continue;
		xn_pcie_phy_chk_recfg_ffe(port);
	}

	if (!xn_pcie_is_pipe_clk_stable(plat, PIPE_CLK_STABLE)) {
		hi_pcie_pr_e("PIPE clk is not stable");
		return -1;
	}
	hi_pcie_pr_d("PIPE clk is stable");

	xn_pcie_rx_term_cfg(plat, PU_RX_TERMIANTION);

	xn_pcie_qic_power_domain(plat, QIC_POWER_DOMAIN_EXIT);

	xn_pcie_firewall_fix(plat);

	ret = xn_pcie_smmu_init(plat);
	if (ret)
		return ret;

	hi_pcie_idle_clear_irq(pcie->idle);
	hi_pcie_32k_idle_init(pcie->idle);
	hi_pcie_idle_enable_irq(pcie->idle);

	hi_pcie_pr_d("-%s-", __func__);
	return 0;
}

#ifdef CONFIG_HIPCIE_IDLE
static int xn_pcie_idle_init(struct hi_pcie *pcie)
{
	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;
	pcie->idle->idle_reg_base = plat->sysctrl_base;
	return 0;
}

static void xn_pcie_idle_reg_cfg(struct hi_pcie *pcie, int index, u32 step)
{
	u32 base_step;
	u32 reg;
	u32 val;

	for (base_step = 0; base_step < pcie_idle_step[index].behavior[step].size; base_step++) {
		reg = pcie_idle_step[index].behavior[step].base_addr[base_step];
		val = pcie_idle_step[index].behavior[step].val[base_step];
		xn_pcie_hsdt_crg_reg_writel(pcie->plat, val, reg);
	}
	return;
}

static int xn_pcie_enter_idle(struct hi_pcie *pcie, u32 port_id)
{
	int ret;
	int i;
	u32 ltssm_state;
	u32 clkreq_state;
	int vote_id = DOZE_CLIENT_PCIE0;
	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;

	hi_pcie_pr_d("++%s++\n", __func__);

	ltssm_state = xn_pcie_sysctrl_readl(plat, HIPCIE_SC_PCIE_CTRL_STAT19) & 0x3F;
	clkreq_state = xn_pcie_sysctrl_readl(plat, HIPCIE_SC_PCIE_CTRL_STAT20);

	hi_pcie_pr_e("[%s]ltssm_state=0x%x, clkreq_state=0x%x", __func__, ltssm_state, clkreq_state);
	if ((ltssm_state != HIPCIE_LTSSM_L1_STATE) || ((clkreq_state & BIT(5)) == 0x0))
		goto end;

	xn_pcie_32k_clk_uncfg(pcie->plat);

	for (i = 0; i < ARRAY_SIZE(pcie_idle_step); i++) {
		if (pcie_idle_step[i].ctrl_id == pcie->ctrl_id &&
			pcie_idle_step[i].port_id == port_id) {
			break;
		}
	}

	if (i == ARRAY_SIZE(pcie_idle_step)) {
		hi_pcie_pr_e("[%s], port %u/%u enter idle fail", __func__, pcie->ctrl_id, port_id);
		return -1;
	}

	xn_pcie_idle_reg_cfg(pcie, i, CLOSE_RFF_CLK);

	ret = vote_enable_doze(vote_id);
	if (ret < 0)
		hi_pcie_pr_e("[%s], port %u/%u enable doze fail", __func__, pcie->ctrl_id, port_id);

end:
	hi_pcie_pr_d("--%s--\n", __func__);
	return 0;
}

static int xn_pcie_exit_idle(struct hi_pcie *pcie, u32 port_id)
{
	int ret;
	int i;
	int vote_id = DOZE_CLIENT_PCIE0;

	hi_pcie_pr_d("++%s++\n", __func__);

	for (i = 0; i < ARRAY_SIZE(pcie_idle_step); i++) {
		if (pcie_idle_step[i].ctrl_id == pcie->ctrl_id &&
			pcie_idle_step[i].port_id == port_id) {
			break;
		}
	}

	if (i == ARRAY_SIZE(pcie_idle_step)) {
		hi_pcie_pr_e("[%s], port %u/%u exit idle fail", __func__, pcie->ctrl_id, port_id);
		return -1;
	}

	ret = vote_disable_doze(vote_id);
	if (ret < 0)
		hi_pcie_pr_e("[%s], port %u/%u disable doze fail", __func__, pcie->ctrl_id, port_id);

	xn_pcie_32k_clk_cfg(pcie->plat);
	
	xn_pcie_idle_reg_cfg(pcie, i, OPEN_RFF_CLK);

	hi_pcie_pr_d("--%s--\n", __func__);
	return 0;
}
#endif

static int xn_pcie_dump_hsdtcrg_info(struct hi_pcie *pcie)
{
	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;

	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} hsdtcrg_info[] = {
		{ HSDT_PERCLKEN1, "PER CLKEN1" },
		{ HSDT_PERSTAT1, "PER STAT1" },
		{ HSDT_PERRSTSTAT0, "PER RST STAT0" },
		{ HSDT_CRG_PCIEPLL_PCIE_CTRL0, "PCIEPLL PCIE CTRL0" },
		{ HSDT_CRG_PCIEPLL_STAT, "PCIEPLL_STAT" },
		{ HSDT_CRG_PCIECTRL0, "PCIECTRL0" },
	};

	hi_pcie_pr_d("PCIe Hsdtcrg info");

	for (i = 0; i < ARRAY_SIZE(hsdtcrg_info); i++) {
		val = xn_pcie_hsdt_crg_reg_readl(plat, hsdtcrg_info[i].reg);
		hi_pcie_pr_e("Hsdtcrg %s reg [0x%x] is [0x%x]", hsdtcrg_info[i].desc, hsdtcrg_info[i].reg, val);
	}

	return 0;
}

static int xn_pcie_dump_sysctrl_info(struct hi_pcie *pcie)
{
	struct hi_pcie_xn *plat = (struct hi_pcie_xn *)pcie->plat;
	
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} sysctrl_info[] = {
		{ HIPCIE_MISC_STATE0, "pcie misc state0" },
		{ HIPCIE_SC_PCIE_CTRL_STAT19, "sc pcie ctrl stat19" },
		{ HIPCIE_SC_PCIE_CTRL_STAT20, "sc pcie ctrl stat20" },
	};

	hi_pcie_pr_d("PCIe Sysctrl info");

	for (i = 0; i < ARRAY_SIZE(sysctrl_info); i++) {
		val = xn_pcie_sysctrl_readl(plat, sysctrl_info[i].reg);
		hi_pcie_pr_i("Sysctrl %s reg [0x%x] is [0x%x]", sysctrl_info[i].desc, sysctrl_info[i].reg, val);
	}

	return 0;
}

static int xn_pcie_dump_plat_info(struct hi_pcie *pcie)
{
	xn_pcie_dump_hsdtcrg_info(pcie);
	xn_pcie_dump_sysctrl_info(pcie);
	return 0;
}


static struct hi_pcie_platform_ops platform_ops = {
	.plat_init			= xn_pcie_init,
	.plat_on			= xn_pcie_turn_on,
	.plat_off			= xn_pcie_turn_off,
	.start_link			= xn_pcie_start_link,
#ifdef CONFIG_HIPCIE_IDLE
	.idle_init			= xn_pcie_idle_init,
	.enter_idle			= xn_pcie_enter_idle,
	.exit_idle			= xn_pcie_exit_idle,
#endif
	.init_phy			= NULL,
	.get_ssid_by_dev	= NULL,
	.dump_plat_info		= xn_pcie_dump_plat_info,
};

int hi_pcie_register_ops(struct platform_device *pdev, struct hi_pcie *pcie)
{
	pcie->plat_ops = &platform_ops;

	hi_pcie_ctrl_register(pcie);
	hi_pcie_dfx_register(pcie);
	return 0;
}