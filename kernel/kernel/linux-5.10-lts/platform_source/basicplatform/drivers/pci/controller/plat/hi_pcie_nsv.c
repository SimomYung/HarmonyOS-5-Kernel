/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: PCIe nsv platform functions.
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
#include <linux/hwspinlock.h>
#include <linux/pinctrl/consumer.h>
#include <securec.h>
#ifdef CONFIG_HIPCIE_IDLE
#include <doze.h>
#endif

#include "hwspinlock_internal.h"

#include "../hi_pcie_defines.h"
#include "../ctrl/hi_pcie_ctrl_common.h"
#include "../phy/hi_pcie_combphy.h"
#include "../phy/hi_pcie_kphy.h"
#include "../phy/hi_pcie_combphy_firmware.h"
#include "../ctrl/hi_pcie_ctrl_dfx.h"
#include "../hi_pcie_external_api.h"
#include "hi_pcie_idle.h"
#ifndef CONFIG_HIPCIE_ARMPC
#include <platform_include/smart/drivers/io_die/iodie_pcie.h>
#endif

atomic_t pcie_tcu_sd_cnt = ATOMIC_INIT(0);
atomic_t pcie_tcu_pwr_cnt = ATOMIC_INIT(0);
atomic_t plat_config_cnt = ATOMIC_INIT(0);

#define ENABLE 1
#define DISABLE 0

#define AOCRGSC_PLL_STAT					0x0CC
#define AOCRGSCPERCLKEN1					0x178
#define AOCRGSCPERSTAT1						0x17C
#define AOCRGSCPEREN2						0x190
#define AOCRGSCPERDIS2						0x194
#define AOCRGSCPERCLKEN2					0x198
#define AOCRGSCPERSTAT5						0x1CC
#define AOCRGSCPEREN6						0x1D0
#define AOCRGSCCLKDIV3						0x25C
#define AOCRGSCCLKDIV5						0x264
#define AOCRGSCCLKDIV9						0x274
#define AOCRGSCCLKDIV12						0x288
#define AOCRGSCCLKDIV16						0x298
#define AOCRGSCPERSTATUS17					0x358
#define AOCRGSCPERSTATUS16					0x35C
#define AOCRGSCPERSTATUS18					0x360
#define AOCRGAO_AUTODIV2					0x500
#define AOCRGPLL_FSM_STAT0_SEC				0xA9C
#define AOCRGSCSPLLCTRL0					0xF14
#define AOCRGSCSPLLCTRL1					0xF18

#define HSDT_PEREN0							0x000
#define HSDT_PERDIS0						0x004
#define HSDT_PERCLKEN0						0x008
#define HSDT_PERSTAT0						0x00C
#define HSDT_PEREN1							0x010
#define HSDT_PERDIS1						0x014
#define HSDT_PERCLKEN1						0x018
#define HSDT_PERSTAT1						0x01C
#define HSDT_PEREN4							0x040
#define HSDT_PERDIS4						0x044
#define HSDT_PERRSTEN0						0x060
#define HSDT_PERRSTDIS0						0x064
#define HSDT_PERRSTSTAT0					0x068
#define HSDT_CRG_CLKDIV0					0x0A8
#define HSDT_PERISTAT1						0x104
#define HSDT_PERISTAT2						0x108
#define HSDT_CRG_PCIECTRL0					0x300
#define HSDT_CRG_PCIECTRL1					0x304
#define HSDT_CRG_PERI_AUTODIV3				0x35C
#define HSDT_CRG_PCIE_HARD_FSM0				0x600
#define HSDT_CRG_PCIE_HARD_FSM_STATE		0x610

#define HSDT0_QIC_WR_NOPENDING_0			0x0C
#define HSDT0_QIC_RD_NOPENDING_0			0x10
#define HSDT0_QIC_TB_NOPENDING_0			0x14

#define HIPCIE_RSTEN0_X1					0x190
#define HIPCIE_RSTDIS0_X1					0x194
#define HIPCIE_ICG_EN0_X1					0x160
#define HIPCIE_ICG_EN1_X1					0x170
#define HIPCIE_ICG_DIS0_X1					0x164
#define HIPCIE_ICG_DIS1_X1					0x174

#define PCIE_CLKREQ_LOW						0

#define HIPCIE_SOC2PHY_ISOEN				0x000
#define HIPCIE_PHY_CFG0						0x034
#define HIPCIE_PHY_CFG1						0x038
#define HIPCIE_MISC_CFG9_V3					0x05C
#define HIPCIE_MISC_STATE0					0x100
#define HIPCIE_CTRL_STAT2					0x408
#define HIPCIE_CTRL_REG31					0x7C
#define HIPCIE_CLKREQ_IN_N_CTRL				0x90
#define HIPCIE_CON_CLKREQ_IN_N_CNT_BASE		0x98
#define HIPCIE_SC_PCIE_CTRL_STAT19			0x290
#define HIPCIE_SC_PCIE_CTRL_STAT20			0x294
#define HIPCIE_SC_PCIE_CTRL_STAT17			0x444
#define HIPCIE_SC_PCIE_CTRL_STAT18			0x448

#define DEBOUNCE_CYCLES						0x80000240

#define FNPLL_PCIE_ANALOG_CFG0				0x0008
#define FNPLL_PCIE_ANALOG_CFG1				0x000C
#define FNPLL_PCIE_ANALOG_CFG2				0x0010
#define FNPLL_PCIE_ANALOG_CFG3				0x0014
#define FNPLL_PCIE_ADPLL_ATUNE_CFG			0x0018
#define FNPLL_PCIE_ADPLL_PTUNE_CFG			0x001C
#define FNPLL_PCIE_DTC_CFG					0x0024
#define FNPLL_PCIE_FTDC_CFG					0x0028
#define FNPLL_PCIE_FDTC_LMS_CFG				0x002C
#define FNPLL_PCIE_DTC_LMS_CFG				0x0030
#define FNPLL_PCIE_LPF_BW_I_T01				0x0034
#define FNPLL_PCIE_LPF_BW_P_T01				0x0038
#define FNPLL_PCIE_ADPLL_DCO_TMODE			0x003C
#define FNPLL_PCIE_ADPLL_LPF_TEST_CFG		0x0040
#define FNPLL_PCIE_ADPLL_DSM_CFG			0x0044
#define FNPLL_PCIE_ADPLL_FTDC_LOCK_THR		0x0048
#define FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR	0x004C
#define FNPLL_PCIE_CHAOS_CNT_THR_CFG		0x0050
#define FNPLL_PCIE_ADPLL_CLK_EN_CFG0		0x0054
#define FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG		0x005C
#define FNPLL_PCIE_DTC_LINEAR_TEST_MODE		0x0060
#define FNPLL_PCIE_FREZ_LMS_PTHR			0x0064
#define FNPLL_PCIE_ACQ_CNT_THR_CFG			0x0068
#define FNPLL_PCIE_ADPLL_DLY_CFG			0x006C
#define FNPLL_PCIE_PTUNE_FBDIV				0x0070

#define FNPLL_PCIE_RETRY_TIME				0x2
#define PCIEPLL_STATE_OPEN					0x1
#define PCIEPLL_STATE_CLOSE					0x2
#define PCIEPLL_STATE_MASK					0x3

#define HSDT_CRG_PCIEPLL_PCIE_CTRL0			0x190
#define HSDT_CRG_PCIEPLL_PCIE_FCW			0x194
#define HSDT_CRG_PCIEPLL_STATE				0x204

#define FNPLL_LOCK_TIMEOUT					20
#define FNPLL_LOCK_BIT						(0x1 << 4)

#define HSDT_SCTRL_SMMU_SD					0x094

#define BASE_ADDR_NUM						32
#define RC_ID0								0
#define RC_ID1								1

#define SC_PCIE_SMMU_CTRL_AW_NS				0x200
#define SC_PCIE_SMMU_CTRL_AR_NS				0x204
#define SC_PCIE_BDF0_NS						0x24C
#define SC_PCIE_NSSID_CTRL_NS				0x28C

#define MAX_SSID_NUM						16

static struct {
	void __iomem *aocrg_base;
	void __iomem *hsdtcrg_base;
	void __iomem *hsdtsctrl_base;
	void __iomem *hsdtxctrl_base;
	void __iomem *pcie_fnpll_base;
	void __iomem *sctrl_base;
} plat_config = {
	NULL, NULL, NULL, NULL, NULL, NULL
};

struct hi_pcie_nsv {
	struct hi_pcie *pcie;
	struct hi_pcie_ctrl *ctrl;

	void __iomem *sysctrl_base;
	void __iomem *ctrl_base;
	void __iomem *phy_base;
	void __iomem *tbu_base;
	void __iomem *phy_pcs_base;
	struct pinctrl *pinctrl;
	struct pinctrl_state *pinctrls_default;
	struct pinctrl_state *pinctrls_idle;

	u32 sub_stream_num;
	u32 ssid_map_list[MAX_SSID_NUM];
	bool fnpll_hw_auto_open;
	int level2_irq;
	u32 clkreq_gpio;
};

struct {
	int smmu_id;
	u32 sid;
	u32 mode_reg;
	u32 aw_reg;
	u32 ar_reg;
	u32 bdf_reg;
	atomic_t *tcu_pwr_cnt;
} nsv_pcie_smmu[] = {
	{
		3, 14,
		SC_PCIE_NSSID_CTRL_NS,
		SC_PCIE_SMMU_CTRL_AW_NS,
		SC_PCIE_SMMU_CTRL_AR_NS,
		SC_PCIE_BDF0_NS,
		&pcie_tcu_pwr_cnt,
	},
	{
		3, 15,
		SC_PCIE_NSSID_CTRL_NS,
		SC_PCIE_SMMU_CTRL_AW_NS,
		SC_PCIE_SMMU_CTRL_AR_NS,
		SC_PCIE_BDF0_NS,
		&pcie_tcu_pwr_cnt,
	},
};

struct pcie_behavior {
	int behavior_id;
	u32 size;
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
	CLKREQ_LOW,
	CTRL_SD_EXIT,
	SMMU_SD_EXIT,
	PHY_RST_N_SEL,
	PHY_PORN_RST,
	PHY_APB_UNRESET,
	PHY_APB_RESET,
	PHY_RESET_N,
	PIPE_RESET_N,
	CTRL_PORN_UNRST,
	CTRL_PORN_RST,
	PIPE_CLK_STABLE,
	PU_RX_TERMIANTION,
	NOC_POWER_EXIT,
	NOC_POWER_ENTER,
	CLKREQ_IDLE_TIMEOUT,
	CLKREQ_NO_DELAY,
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
				{ (BIT(30) | BIT(28) | BIT(19) | BIT(11) | BIT(9) | BIT(8) | BIT(7) | BIT(5) | BIT(0)) } },
			{ PCIE_UNRST, 1, { HSDT_PERRSTDIS0 }, { (BIT(30) | BIT(28) | BIT(19) | BIT(11) | BIT(8) | BIT(5) | BIT(0)) } },
			// pciephy_ref_soft bit6/pcieio_soft bit7/tcu bit10/tbu bit11/mudp bit14 bit16/int_par bit15
			{ OPEN_SYSCLK, 1, { HSDT_PEREN1 }, { 0xFFFE1B3F } },
			{ CLOSE_SYSCLK, 1, { HSDT_PERDIS1 }, { 0xFFFE1BFF } },
			{ SC_RST, 1, { HIPCIE_RSTEN0_X1 }, { 0xFFFFFFFF } },
			{ SC_UNRST, 1, { HIPCIE_RSTDIS0_X1 }, { 0xFFFFFFFF } },
			{ EN_SC_CLK, 2, { HIPCIE_ICG_EN0_X1, HIPCIE_ICG_EN1_X1 }, { 0xFFFFFFFF, 0xFFFFFFFF } },
			{ DIS_SC_CLK, 2, { HIPCIE_ICG_DIS0_X1, HIPCIE_ICG_DIS1_X1 }, { 0xFFFFFFFF, 0xFFFFFFFF } },
			{ OE_IE_CFG_RC, 1, { HSDT_CRG_PCIECTRL0 }, { BIT(27) } }, // hard ctrl
			{ OE_IE_CFG_EP, 1, { HSDT_CRG_PCIECTRL0 }, { (BIT(11) | BIT(9) | BIT(6)) } }, // hard ctrl
			{ CLKREQ_LOW, 0, {}, {} },
			{ CTRL_SD_EXIT, 0, {}, {} },
			{ SMMU_SD_EXIT, 1, { HSDT_SCTRL_SMMU_SD }, { BIT(1) } },
			{ PHY_RST_N_SEL, 1, { HIPCIE_PHY_CFG0 }, { BIT(5) } },
			{ PHY_PORN_RST, 1, { HIPCIE_PHY_CFG0 }, { (BIT(2)) } },
			{ PHY_APB_UNRESET, 1, { HSDT_PERRSTDIS0 }, { BIT(9) } },
			{ PHY_APB_RESET, 1, { HSDT_PERRSTEN0 }, { BIT(9) } },
			{ PHY_RESET_N, 1, { HIPCIE_PHY_CFG0 }, { (BIT(4)) } },
			{ PIPE_RESET_N, 0, { HIPCIE_PHY_CFG0 }, { (BIT(6) | BIT(7)) } },
			{ CTRL_PORN_UNRST, 1, { HSDT_PERRSTDIS0 }, { (BIT(7)) } },
			{ CTRL_PORN_RST, 1, { HSDT_PERRSTEN0 }, { (BIT(7)) } },
			{ PIPE_CLK_STABLE, 1, { HIPCIE_CTRL_STAT2 }, { (BIT(19)) } },
			{ PU_RX_TERMIANTION, 1, { HIPCIE_PHY_CFG1 }, { (BIT(0) | BIT(16)) } },
			{ NOC_POWER_EXIT, 0, {}, {} },
			{ NOC_POWER_ENTER, 0, {}, {} },
			{ CLKREQ_IDLE_TIMEOUT, 0, {},{} },
			{ CLKREQ_NO_DELAY, 0, {}, {} },
			{ QIC_POWER_DOMAIN_ENTER, 2, { 0x424, 0x428 }, { BIT(4), 4 } },
			{ QIC_POWER_DOMAIN_EXIT, 2, { 0x424, 0x428 }, { BIT(4), 4 } },
		}
	}, { RC_ID1,
		{
			{ PCIE_RST, 1, { HSDT_PERRSTEN0 },
				{ (BIT(29) | BIT(27) | BIT(25) | BIT(23) | BIT(22) | BIT(21) | BIT(20) | BIT(18) | BIT(10)) } },
			{ PCIE_UNRST, 1, { HSDT_PERRSTDIS0 }, { (BIT(29) | BIT(27) | BIT(25) | BIT(22) | BIT(20) | BIT(18) | BIT(10)) } },
			// hsdt_ioc bit2/pciephy_ref_soft bit4/pcieio_soft bit5/Sctrl bit11/dfa bit15/
			{ OPEN_SYSCLK, 1, { HSDT_PEREN0 }, { 0xFFFF3FCF } },
			{ CLOSE_SYSCLK, 1, { HSDT_PERDIS0 }, { 0xFFFF37FB } },
			{ SC_RST, 1, { HIPCIE_RSTEN0_X1 }, { 0xFFFFFFFF } },
			{ SC_UNRST, 1, { HIPCIE_RSTDIS0_X1 }, { 0xFFFFFFFF } },
			{ EN_SC_CLK, 2, { HIPCIE_ICG_EN0_X1, HIPCIE_ICG_EN1_X1 }, { 0xFFFFFFFF, 0xFFFFFFFF } },
			{ DIS_SC_CLK, 2, { HIPCIE_ICG_DIS0_X1, HIPCIE_ICG_DIS1_X1 }, { 0xFFFFFFFF, 0xFFFFFFFF } },
			{ OE_IE_CFG_RC, 1, { HSDT_CRG_PCIECTRL1 }, { BIT(27) } }, // hard ctrl
			{ OE_IE_CFG_EP, 1, { HSDT_CRG_PCIECTRL1 }, { (BIT(11) | BIT(9) | BIT(6)) } }, // hard ctrl
			{ CLKREQ_LOW, 0, {}, {} },
			{ CTRL_SD_EXIT, 0, {}, {} },
			{ SMMU_SD_EXIT, 1, { HSDT_SCTRL_SMMU_SD }, { BIT(2) } },
			{ PHY_RST_N_SEL, 1, { HIPCIE_PHY_CFG0 }, { BIT(5) } },
			{ PHY_PORN_RST, 1, { HIPCIE_PHY_CFG0 }, { (BIT(2)) } },
			{ PHY_APB_UNRESET, 1, { HSDT_PERRSTDIS0 }, { BIT(23) } },
			{ PHY_APB_RESET, 1, { HSDT_PERRSTEN0 }, { BIT(23) } },
			{ PHY_RESET_N, 1, { HIPCIE_PHY_CFG0 }, { (BIT(4)) } },
			{ PIPE_RESET_N, 0, { HIPCIE_PHY_CFG0 }, { (BIT(6) | BIT(7)) } },
			{ CTRL_PORN_UNRST, 1, { HSDT_PERRSTDIS0 }, { (BIT(21)) } },
			{ CTRL_PORN_RST, 1, { HSDT_PERRSTEN0 }, { (BIT(21)) } },
			{ PIPE_CLK_STABLE, 1, { HIPCIE_CTRL_STAT2 }, { (BIT(19)) } },
			{ PU_RX_TERMIANTION, 1, { HIPCIE_PHY_CFG1 }, { (BIT(0) | BIT(16)) } },
			{ NOC_POWER_EXIT, 0, {}, {} },
			{ NOC_POWER_ENTER, 0, {}, {} },
			{ CLKREQ_IDLE_TIMEOUT, 0, {},{} },
			{ CLKREQ_NO_DELAY, 0, {}, {} },
			{ QIC_POWER_DOMAIN_ENTER, 2, { 0x424, 0x428 }, { BIT(5), 5 } },
			{ QIC_POWER_DOMAIN_EXIT, 2, { 0x424, 0x428 }, { BIT(5), 5 } },
		}
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
	}, { RC_ID1, 0,
		{
			{ OPEN_RFF_CLK, 1, { HSDT_PEREN0 }, { (BIT(7) | BIT(6)) } }, // debounce/hp/phy_ref hardware control
			{ CLOSE_RFF_CLK, 1, { HSDT_PERDIS0 }, { (BIT(7) | BIT(6)) } }, // debounce/hp/phy_ref hardware control
			// OPEN_IO_CLK/CLOSE_IO_CLK hardware control
		}
	}
};
#endif

static u32 fnpll_count = 0;
static u32 pcie_share_st = 0;

#ifdef CONFIG_HIPCIE_ARMPC

#define SCTRL_PCIE_RESOURCE_VOTE_REG			0x4D0

#define SCTRL_PCIE_RESOURCE_VOTE_RST_MASK		0xC0
#define SCTRL_PCIE_RESOURCE_VOTE_CLK_MASK		0x300
#define SCTRL_PCIE_RESOURCE_VOTE_MEM_SD_MASK	0xC00
#define SCTRL_PCIE_RESOURCE_VOTE_FNPLL_MASK		0x7000

#define SCTRL_SCPERCTRL17_REG					0x344

enum {
	HI_PCIE_VOTE_COMMON_RST_PCIE0 = 0x6,
	HI_PCIE_VOTE_COMMON_RST_PCIE1,
	HI_PCIE_VOTE_COMMON_CLK_PCIE0,
	HI_PCIE_VOTE_COMMON_CLK_PCIE1,
	HI_PCIE_VOTE_MEM_SD_PCIE0,
	HI_PCIE_VOTE_MEM_SD_PCIE1,
	HI_PCIE_VOTE_FNPLL_PCIE0,
	HI_PCIE_VOTE_FNPLL_PCIE1_ACPU,
	HI_PCIE_VOTE_FNPLL_PCIE1_IOMCU,
	HI_PCIE_VOTE_MAX,
};

static DEFINE_SPINLOCK(resource_vote_lock);
static u32 chip_generation_type = 0;

static spinlock_t *iodie_pcie_resource_vote_lock_get(void)
{
	return &resource_vote_lock;
}

static inline int iodie_pcie_hwspinlock_request(void)
{
	return 0;
}

static inline void iodie_pcie_hwspinlock_release(void)
{
	return;
}

static u32 iodie_pcie_asic_tyep_get(void)
{
	return chip_generation_type;
}

static int iodie_pcie_hwspinlock_init(void)
{
	int ret;
	struct device_node *np;
	char *compatible = "hi_pcie,hwspinlock";

	np = of_find_compatible_node(NULL, NULL, compatible);
	if (!np) {
		hi_pcie_pr_e("[%s]Failed to get %s node", __func__, compatible);
		return -1;
	}

	ret = of_property_read_u32(np, "asic_type", &chip_generation_type);
	if (ret != 0) {
		hi_pcie_pr_e("[%s]asic_type not find", __func__);
		chip_generation_type = 0;
	}

	hi_pcie_pr_i("chip_generation_type:0x%x", chip_generation_type);

	hi_pcie_pr_i("%s ok", __func__);
	return 0;
}

static u32 iodie_pcie_sctrl_reg_readl(u32 reg)
{
	return readl(plat_config.sctrl_base + reg);
}

static void iodie_pcie_sctrl_reg_writel(u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat_config.sctrl_base + reg);
}
#endif

static u32 nsv_pcie_ao_crg_reg_readl(struct hi_pcie_nsv *plat, u32 reg)
{
	return readl(plat_config.aocrg_base + reg);
}

static void nsv_pcie_ao_crg_reg_writel(struct hi_pcie_nsv *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat_config.aocrg_base + reg);
}

static u32 nsv_pcie_hsdt_crg_reg_readl(struct hi_pcie_nsv *plat, u32 reg)
{
	return readl(plat_config.hsdtcrg_base + reg);
}

static void nsv_pcie_hsdt_crg_reg_writel(struct hi_pcie_nsv *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat_config.hsdtcrg_base + reg);
}

static u32 nsv_pcie_sysctrl_readl(struct hi_pcie_nsv *plat, u32 reg)
{
	return readl(plat->sysctrl_base + reg);
}

static void nsv_pcie_sysctrl_writel(struct hi_pcie_nsv *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat->sysctrl_base + reg);
}

static u32 nsv_pcie_hsdt_sctrl_reg_readl(struct hi_pcie_nsv *plat, u32 reg)
{
	return readl(plat_config.hsdtsctrl_base + reg);
}

static void nsv_pcie_hsdt_sctrl_reg_writel(struct hi_pcie_nsv *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat_config.hsdtsctrl_base + reg);
}

static u32 nsv_pcie_hsdt_xctrl_reg_readl(struct hi_pcie_nsv *plat, u32 reg)
{
	return readl(plat_config.hsdtxctrl_base + reg);
}

static void nsv_pcie_hsdt_xctrl_reg_writel(struct hi_pcie_nsv *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat_config.hsdtxctrl_base + reg);
}

static void nsv_pcie_fnpll_reg_writel(struct hi_pcie_nsv *plat, u32 val, u32 reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, plat_config.pcie_fnpll_base + reg);
}

static int nsv_pcie_clk_config_swdone_check(struct hi_pcie_nsv *plat)
{
	u32 val;
	u32 timeout = 100; /* try 100 times */

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_PERISTAT1);
	if ((val & BIT(0)) != 0x0) {
		hi_pcie_pr_i("clk config swdone pcie div4=0x%x is success", val);
		return 0;
	}

	hi_pcie_pr_e("clk config swdone pcie div4=0x%x, clk_hsdt_subsys need switch 64div", val);
	nsv_pcie_ao_crg_reg_writel(plat, 0x003F003F, AOCRGSCCLKDIV3);

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_PERISTAT1);
	while ((val & BIT(0)) == 0x0) {
		udelay(1);
		if (timeout == 0) {
			hi_pcie_pr_e("clk config swdone pcie div4 64div timeout, val 0x%x", val);
			return -1;
		}
		timeout--;
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_PERISTAT1);
	}

	hi_pcie_pr_e("clk config swdone pcie div4=0x%x, clk_hsdt_subsys need switch 7div", val);
	nsv_pcie_ao_crg_reg_writel(plat, 0x003F0006, AOCRGSCCLKDIV3);

	timeout = 100;
	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_PERISTAT1);
	while ((val & BIT(0)) == 0x0) {
		udelay(1);
		if (timeout == 0) {
			hi_pcie_pr_e("clk config swdone pcie div4 7div timeout, val 0x%x", val);
			return -1;
		}
		timeout--;
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_PERISTAT1);
	}

	return 0;
}

static int nsv_pcie_clk_config(struct hi_pcie_nsv *plat)
{
	int ret;
	u32 val;

	// sel_suspend_ao2syscache_hsdt_subsys_inuse to fll clk
	iodie_pcie_sctrl_reg_writel(BIT(10) | BIT(26), SCTRL_SCPERCTRL17_REG);

	ret = iodie_pcie_hwspinlock_request();
	if (ret != 0)
		return -1;

	// clk_hsdt_subsys set to 238.74M
	nsv_pcie_ao_crg_reg_writel(plat, 0x003F0006, AOCRGSCCLKDIV3);

	if ((nsv_pcie_clk_config_swdone_check(plat)) != 0x0) {
		iodie_pcie_hwspinlock_release();
		pcie_dsm_client_report_type(DSM_REPORT_TYPE_SWDOWN_ACLK_PCIE_DIV4_INACTIVE);
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_PERISTAT1);
		pcie_dsm_report(val);
		return -1;
	}
	iodie_pcie_hwspinlock_release();

	// autofs PCIE_on
	nsv_pcie_ao_crg_reg_writel(plat, 0x07E0283F, AOCRGAO_AUTODIV2);
	nsv_pcie_hsdt_crg_reg_writel(plat, 0x0, HSDT_CRG_PERI_AUTODIV3);

	val = nsv_pcie_ao_crg_reg_readl(plat, AOCRGAO_AUTODIV2);
	val &= ~BIT(0); // peri_div_auto_reduce_bypass
	nsv_pcie_ao_crg_reg_writel(plat, val, AOCRGAO_AUTODIV2);

	// gt_pclk_autodiv_pcie
	nsv_pcie_ao_crg_reg_writel(plat, BIT(4), AOCRGSCPEREN6);

	return 0;
}

static void nsv_pcie_qic_power_domain(struct hi_pcie_nsv *plat, int behavior)
{
	u32 val = 0;
	u32 val_mask ;
	u32 reg;
	u32 timeout = 100; /* try 100 times */
	u32 bitmask;

	reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[0];
	val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[0];
	if (behavior == QIC_POWER_DOMAIN_ENTER) {
		val |= (val_mask | (val_mask << 16));
		bitmask = 0x0;
	} else if (behavior == QIC_POWER_DOMAIN_EXIT) {
		val |= (val_mask << 16);
		bitmask = 0x1;
	} else {
		return;
	}
	nsv_pcie_hsdt_xctrl_reg_writel(plat, val, reg);

	reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[1];
	val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[1];
	val = nsv_pcie_hsdt_xctrl_reg_readl(plat, reg);
	while (((val >> val_mask) & 0x1) == bitmask) {
		udelay(10);
		if (timeout == 0) {
			hi_pcie_pr_e("%s, qic idle ack timeout, val 0x%x", __func__, val);
			return;
		}
		timeout--;
		val = nsv_pcie_hsdt_xctrl_reg_readl(plat, reg);
	}
}

static void nsv_pcie_rst_cfg(struct hi_pcie_nsv *plat, int behavior)
{
	u32 val_mask;
	u32 reg;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static void nsv_pcie_common_rst_cfg(struct hi_pcie_nsv *plat)
{
	int ret;
	u32 val;
	u32 bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_COMMON_RST_PCIE0 : HI_PCIE_VOTE_COMMON_RST_PCIE1);

	iodie_pcie_sctrl_reg_writel(BIT(bit) << 16, SCTRL_PCIE_RESOURCE_VOTE_REG);
	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((val & SCTRL_PCIE_RESOURCE_VOTE_RST_MASK) == 0x0) {
		hi_pcie_pr_d("pcie common rst_cfg");
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(3) | BIT(15), HSDT_PERRSTEN0); // bit3 tcu, bit15 fsm
	}
}

static void nsv_pcie_common_unrst_cfg(struct hi_pcie_nsv *plat)
{
	int ret;
	u32 val;
	u32 bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_COMMON_RST_PCIE0 : HI_PCIE_VOTE_COMMON_RST_PCIE1);

	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((val & SCTRL_PCIE_RESOURCE_VOTE_RST_MASK) == 0x0) {
		hi_pcie_pr_d("pcie common unrst_cfg");
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(3) | BIT(15), HSDT_PERRSTDIS0); // bit3 tcu, bit15 fsm
	}
	iodie_pcie_sctrl_reg_writel(BIT(bit) | (BIT(bit) << 16), SCTRL_PCIE_RESOURCE_VOTE_REG);
}

static void nsv_pcie_common_clk_cfg(struct hi_pcie_nsv *plat)
{
	int ret;
	u32 val;
	u32 bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_COMMON_CLK_PCIE0 : HI_PCIE_VOTE_COMMON_CLK_PCIE1);

	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((val & SCTRL_PCIE_RESOURCE_VOTE_CLK_MASK) == 0x0) { // bit10 tcu, bit14 fsm
		hi_pcie_pr_d("pcie common clk_cfg");
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(10), HSDT_PEREN1);
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(14), HSDT_PEREN0);
	}
	iodie_pcie_sctrl_reg_writel(BIT(bit) | (BIT(bit) << 16), SCTRL_PCIE_RESOURCE_VOTE_REG);
}

static void nsv_pcie_common_clk_uncfg(struct hi_pcie_nsv *plat)
{
	int ret;
	u32 val;
	u32 bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_COMMON_CLK_PCIE0 : HI_PCIE_VOTE_COMMON_CLK_PCIE1);

	iodie_pcie_sctrl_reg_writel(BIT(bit) << 16, SCTRL_PCIE_RESOURCE_VOTE_REG);
	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((val & SCTRL_PCIE_RESOURCE_VOTE_CLK_MASK) == 0x0) { // bit10 tcu, bit14 fsm
		hi_pcie_pr_d("pcie common clk_uncfg");
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(10), HSDT_PERDIS1);
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(14), HSDT_PERDIS0);
	}
}

static void nsv_pcie_sysclk_ctrl(struct hi_pcie_nsv *plat, bool open)
{
	u32 val_mask;
	u32 reg;
	int behavior = open ? OPEN_SYSCLK : CLOSE_SYSCLK;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}

	if (open) {
		// close phy_ref_soft/pcieio_soft
		reg = ((plat->pcie->ctrl_id == RC_ID0) ? HSDT_PERDIS1 : HSDT_PERDIS0);
		val_mask = ((plat->pcie->ctrl_id == RC_ID0) ? (BIT(6) | BIT(7)) : (BIT(4) | BIT(5)));
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);

		reg = ((plat->pcie->ctrl_id == RC_ID0) ? HSDT_CRG_PCIECTRL0 : HSDT_CRG_PCIECTRL1);
		val_mask = nsv_pcie_hsdt_crg_reg_readl(plat, reg);
		val_mask &= ~(BIT(1) | BIT(0)); // phy_ref_hard_bypass=0x0/pcieio_soft=0x0
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	} else {
		reg = ((plat->pcie->ctrl_id == RC_ID0) ? HSDT_CRG_PCIECTRL0 : HSDT_CRG_PCIECTRL1);
		val_mask = nsv_pcie_hsdt_crg_reg_readl(plat, reg);
		val_mask |= (BIT(1) | BIT(0)); // phy_ref_hard_bypass=0x1/pcieio_soft=0x1
		val_mask |= BIT(11); // pcieio_oe_en_hard bypass
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static void nsv_pcie_sc_rst_cfg(struct hi_pcie_nsv *plat, bool reset)
{
	u32 step;
	u32 val;
	u32 val_mask;
	u32 reg;
	int behavior = reset ? SC_RST : SC_UNRST;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		val = nsv_pcie_sysctrl_readl(plat, reg);
		val = val | val_mask;
		nsv_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void nsv_pcie_sc_clk_ctrl(struct hi_pcie_nsv *plat, bool open)
{
	u32 val;
	u32 val_mask;
	u32 reg;
	int behavior = open ? EN_SC_CLK : DIS_SC_CLK;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 step;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		val = nsv_pcie_sysctrl_readl(plat, reg);
		val = val | val_mask;
		nsv_pcie_sysctrl_writel(plat, val, reg);
	}
}
static void nsv_pcie_es_fnpll_cfg_init(struct hi_pcie_nsv *plat)
{
	nsv_pcie_fnpll_reg_writel(plat, 0x0000008A, FNPLL_PCIE_ANALOG_CFG0);
	nsv_pcie_fnpll_reg_writel(plat, 0x0000000C, FNPLL_PCIE_ANALOG_CFG1);
	nsv_pcie_fnpll_reg_writel(plat, 0x000002BE, FNPLL_PCIE_ANALOG_CFG2);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_ANALOG_CFG3);
	nsv_pcie_fnpll_reg_writel(plat, 0x00300000, FNPLL_PCIE_ADPLL_ATUNE_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x026020A0, FNPLL_PCIE_ADPLL_PTUNE_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00012ABC, FNPLL_PCIE_DTC_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x05866E1F, FNPLL_PCIE_FTDC_CFG);

	nsv_pcie_fnpll_reg_writel(plat, 0x6420030A, FNPLL_PCIE_FDTC_LMS_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x5620153F, FNPLL_PCIE_DTC_LMS_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x2014008E, FNPLL_PCIE_LPF_BW_I_T01);
	nsv_pcie_fnpll_reg_writel(plat, 0x00006543, FNPLL_PCIE_LPF_BW_P_T01);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_ADPLL_DCO_TMODE);
	nsv_pcie_fnpll_reg_writel(plat, 0x00010000, FNPLL_PCIE_ADPLL_LPF_TEST_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000003, FNPLL_PCIE_ADPLL_DSM_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x000240FF, FNPLL_PCIE_ADPLL_FTDC_LOCK_THR);

	nsv_pcie_fnpll_reg_writel(plat, 0x0080103F, FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR);
	nsv_pcie_fnpll_reg_writel(plat, 0x80800A0A, FNPLL_PCIE_CHAOS_CNT_THR_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x8848D00F, FNPLL_PCIE_ADPLL_CLK_EN_CFG0);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00100010, FNPLL_PCIE_DTC_LINEAR_TEST_MODE);
	nsv_pcie_fnpll_reg_writel(plat, 0x0020A420, FNPLL_PCIE_FREZ_LMS_PTHR);
	nsv_pcie_fnpll_reg_writel(plat, 0x01FE1FFF, FNPLL_PCIE_ACQ_CNT_THR_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x000FF102, FNPLL_PCIE_ADPLL_DLY_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_PTUNE_FBDIV);

	udelay(1);
}

static void nsv_pcie_cs_fnpll_cfg_init(struct hi_pcie_nsv *plat)
{
	nsv_pcie_fnpll_reg_writel(plat, 0x000000CA, FNPLL_PCIE_ANALOG_CFG0);
	nsv_pcie_fnpll_reg_writel(plat, 0x0000000C, FNPLL_PCIE_ANALOG_CFG1);
	nsv_pcie_fnpll_reg_writel(plat, 0x16204036, FNPLL_PCIE_ANALOG_CFG2);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_ANALOG_CFG3);
	nsv_pcie_fnpll_reg_writel(plat, 0x0032C000, FNPLL_PCIE_ADPLL_ATUNE_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x026020A0, FNPLL_PCIE_ADPLL_PTUNE_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00012D3F, FNPLL_PCIE_DTC_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x05366E1F, FNPLL_PCIE_FTDC_CFG);

	nsv_pcie_fnpll_reg_writel(plat, 0x6420030A, FNPLL_PCIE_FDTC_LMS_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x5620163F, FNPLL_PCIE_DTC_LMS_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x20037015, FNPLL_PCIE_LPF_BW_I_T01);
	nsv_pcie_fnpll_reg_writel(plat, 0x00003B25, FNPLL_PCIE_LPF_BW_P_T01);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_ADPLL_DCO_TMODE);
	nsv_pcie_fnpll_reg_writel(plat, 0x00010000, FNPLL_PCIE_ADPLL_LPF_TEST_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000003, FNPLL_PCIE_ADPLL_DSM_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x000AA0FF, FNPLL_PCIE_ADPLL_FTDC_LOCK_THR);

	nsv_pcie_fnpll_reg_writel(plat, 0x0078103F, FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR);
	nsv_pcie_fnpll_reg_writel(plat, 0x80780A0A, FNPLL_PCIE_CHAOS_CNT_THR_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x8848D00F, FNPLL_PCIE_ADPLL_CLK_EN_CFG0);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00100010, FNPLL_PCIE_DTC_LINEAR_TEST_MODE);
	nsv_pcie_fnpll_reg_writel(plat, 0x0020A420, FNPLL_PCIE_FREZ_LMS_PTHR);
	nsv_pcie_fnpll_reg_writel(plat, 0x01FE1FFF, FNPLL_PCIE_ACQ_CNT_THR_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x000FF102, FNPLL_PCIE_ADPLL_DLY_CFG);
	nsv_pcie_fnpll_reg_writel(plat, 0x00000000, FNPLL_PCIE_PTUNE_FBDIV);

	udelay(1);
}

static void nsv_pcie_fnpll_cfg_init(struct hi_pcie_nsv *plat)
{
	if (iodie_pcie_asic_tyep_get() == 0)
		nsv_pcie_es_fnpll_cfg_init(plat);
	else
		nsv_pcie_cs_fnpll_cfg_init(plat);
}

static void nsv_pcie_fnpll_fsm_startup(struct hi_pcie_nsv *plat)
{
	u32 val;

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	val &= (~BIT(0));
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIEPLL_PCIE_CTRL0);

	nsv_pcie_hsdt_crg_reg_writel(plat, BIT(14) , HSDT_PERDIS0);

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM0);
	val &= (~BIT(31));
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIE_HARD_FSM0);

	nsv_pcie_hsdt_crg_reg_writel(plat, BIT(6) | BIT(7), HSDT_PERDIS1);
	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIECTRL0);
	val &= (~(BIT(1) | BIT(0) | BIT(6) | BIT(11) | BIT(28) | BIT(27)));
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIECTRL0);

	if (plat->pcie->fnpll_fsm_ctrl == PCIE_PLL_FSM_EXCLUSIVE_OPEN) {
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM0);
		val |= (BIT(29) | BIT(27) | BIT(25) | BIT(23) | BIT(21) | BIT(19));
		nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIE_HARD_FSM0);
	} else if (plat->pcie->fnpll_fsm_ctrl == PCIE_PLL_FSM_SHARED_OPEN) {
		nsv_pcie_hsdt_crg_reg_writel(plat, BIT(4) | BIT(5), HSDT_PERDIS0);
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIECTRL1);
		val &= (~(BIT(1) | BIT(0) | BIT(6) | BIT(11) | BIT(28) | BIT(27)));
		nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIECTRL1);
	} else {
		hi_pcie_pr_e("fnpll_fsm_ctrl 0x%x invalid", plat->pcie->fnpll_fsm_ctrl);
	}

	nsv_pcie_hsdt_crg_reg_writel(plat, BIT(14) , HSDT_PEREN0);

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM0);
	val &= (~BIT(18));
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIE_HARD_FSM0);
}

static int nsv_pcie_fnpll_freq_init(struct hi_pcie_nsv *plat, u32 cnt)
{
	u32 val;
	u32 val_mask = FNPLL_LOCK_BIT;
	u32 timeout = FNPLL_LOCK_TIMEOUT;

	// EN=0&&BYPASS=1
	nsv_pcie_hsdt_crg_reg_writel(plat, 0x00000002, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	if (iodie_pcie_asic_tyep_get() == 0)
		nsv_pcie_hsdt_crg_reg_writel(plat, 0x2EE00000, HSDT_CRG_PCIEPLL_PCIE_FCW);
	else
		nsv_pcie_hsdt_crg_reg_writel(plat, 0x2C455555, HSDT_CRG_PCIEPLL_PCIE_FCW);

	if (plat->pcie->fnpll_fsm_ctrl != PCIE_PLL_FSM_CLOSE) {
		nsv_pcie_fnpll_fsm_startup(plat);
	} else {
		// EN=1&&BYPASS=1
		nsv_pcie_hsdt_crg_reg_writel(plat, 0x00000003, HSDT_CRG_PCIEPLL_PCIE_CTRL0);

		udelay(50);

		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIEPLL_STATE);
		while ((val & val_mask) == 0) {
			udelay(10);
			if (timeout == 0) {
				hi_pcie_pr_e("FNPLL lock timeout, val 0x%x, cnt %u",val, cnt);
				return -1;
			}
			timeout--;
			val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIEPLL_STATE);
		}

		// EN=1&&BYPASS=0
		nsv_pcie_hsdt_crg_reg_writel(plat, 0x00000001, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	}
	hi_pcie_pr_i("FNPLL lock succ, cnt %u", cnt);
	return 0;
}

static void nsv_pcie_fnpll_clk_init(struct hi_pcie_nsv *plat)
{
	u32 i;

	for (i = 0; i < FNPLL_PCIE_RETRY_TIME; i++) {
		nsv_pcie_fnpll_cfg_init(plat);
		if (nsv_pcie_fnpll_freq_init(plat, i) == 0x0)
			break;
	}
}

static int nsv_pcie_fnpll_clk_cfg(struct hi_pcie_nsv *plat)
{
	int ret;
	u32 val;
	u32 fnpll_val;
	u32 bit;
	unsigned long flags;
	spinlock_t *resource_vote_lock;

	bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_FNPLL_PCIE0 : HI_PCIE_VOTE_FNPLL_PCIE1_ACPU);
	resource_vote_lock = iodie_pcie_resource_vote_lock_get();
	ret = iodie_pcie_hwspinlock_request();
	if (ret != 0)
		return -1;

	spin_lock_irqsave(resource_vote_lock, flags);
	fnpll_val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	hi_pcie_pr_i("[%s]fnpll[0x%x]=0x%x", __func__, HSDT_CRG_PCIEPLL_PCIE_CTRL0, fnpll_val);
	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((fnpll_val & PCIEPLL_STATE_MASK) == PCIEPLL_STATE_CLOSE) { // only pcie0 use
		hi_pcie_pr_e("[%s] open pcie fnpll", __func__);
		nsv_pcie_fnpll_clk_init(plat);
	}
	iodie_pcie_sctrl_reg_writel(BIT(bit) | (BIT(bit) << 16), SCTRL_PCIE_RESOURCE_VOTE_REG);
	spin_unlock_irqrestore(resource_vote_lock, flags);
	iodie_pcie_hwspinlock_release();
	hi_pcie_pr_i("[%s]ori_vote=0x%x, vote=0x%x", __func__,
		val, iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG));
	return 0;
}

static int nsv_pcie_fnpll_fsm_shutdown(struct hi_pcie_nsv *plat)
{
	u32 val;
	u32 timeout = 10;

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM0);
	val |= BIT(18);
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIE_HARD_FSM0);

	udelay(5);

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM_STATE);
	while (((val >> 29) & 0x3) == 0x0) {
		hi_pcie_pr_e("pcie pll fsm state val 0x%x", val);
		udelay(1);
		if (timeout == 0) {
			hi_pcie_pr_e("pcie pll fsm close fail");
			return -1;
		}
		timeout--;
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM_STATE);
	}

	nsv_pcie_hsdt_crg_reg_writel(plat, BIT(14) , HSDT_PERDIS0);

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIECTRL0);
	val |= (BIT(1) | BIT(0) | BIT(11) | BIT(27));
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIECTRL0);

	if (plat->pcie->fnpll_fsm_ctrl == PCIE_PLL_FSM_SHARED_OPEN) {
		val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIECTRL1);
		val |= (BIT(1) | BIT(0) | BIT(11) | BIT(27));
		nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIECTRL1);
	}

	val = nsv_pcie_hsdt_crg_reg_readl(plat, HSDT_CRG_PCIE_HARD_FSM0);
	val |= BIT(31);
	nsv_pcie_hsdt_crg_reg_writel(plat, val , HSDT_CRG_PCIE_HARD_FSM0);

	return 0;
}

static int nsv_pcie_fnpll_clk_uncfg(struct hi_pcie_nsv *plat, bool is_idle)
{
	int ret = 0;
	u32 val;
	u32 ori_val;
	u32 bit;
	unsigned long flags;
	spinlock_t *resource_vote_lock;

	if (is_idle) {
		hi_pcie_pr_e("[%s]sysctrl19=0x%x, sysctrl20=0x%x", __func__,
			(nsv_pcie_sysctrl_readl(plat, HIPCIE_SC_PCIE_CTRL_STAT19) & 0x3F),
			nsv_pcie_sysctrl_readl(plat, HIPCIE_SC_PCIE_CTRL_STAT20));
		if (((nsv_pcie_sysctrl_readl(plat, HIPCIE_SC_PCIE_CTRL_STAT19) & 0x3F) != HIPCIE_LTSSM_L1_STATE) ||
			((nsv_pcie_sysctrl_readl(plat, HIPCIE_SC_PCIE_CTRL_STAT20) & BIT(5)) == 0x0)) {
			ori_val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
			goto end;
		}
	}

	bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_FNPLL_PCIE0 : HI_PCIE_VOTE_FNPLL_PCIE1_ACPU);
	resource_vote_lock = iodie_pcie_resource_vote_lock_get();
	ret = iodie_pcie_hwspinlock_request();
	if (ret != 0)
		return -1;

	spin_lock_irqsave(resource_vote_lock, flags);
	ori_val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	iodie_pcie_sctrl_reg_writel(BIT(bit) << 16, SCTRL_PCIE_RESOURCE_VOTE_REG);
	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((val & SCTRL_PCIE_RESOURCE_VOTE_FNPLL_MASK) == 0x0) { // only pcie0 use
		hi_pcie_pr_e("[%s] close pcie fnpll", __func__);
		if (plat->pcie->fnpll_fsm_ctrl != PCIE_PLL_FSM_CLOSE) {
			ret = nsv_pcie_fnpll_fsm_shutdown(plat);
		} else {
			nsv_pcie_hsdt_crg_reg_writel(plat, 0x00000002, HSDT_CRG_PCIEPLL_PCIE_CTRL0);
			ret = 0;
		}
	}
	spin_unlock_irqrestore(resource_vote_lock, flags);
	iodie_pcie_hwspinlock_release();
end:
	hi_pcie_pr_i("[%s]ori_vote=0x%x, vote=0x%x", __func__,
		ori_val, iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG));
	return ret;
}

static void nsv_pcie_oe_ie_cfg(struct hi_pcie_nsv *plat, int behavior)
{
	u32 step;
	u32 val;
	u32 val_mask;
	u32 reg;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	if (plat->pcie->fnpll_fsm_ctrl != PCIE_PLL_FSM_CLOSE)
		return;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];

		val = nsv_pcie_hsdt_crg_reg_readl(plat, reg);
		val = val | val_mask;
		if (plat->pcie->mode == PCIE_MODE_RC)
			val &= 0xCFFFF5BF;
		else
			val &= 0xC7FFFFFF;

		nsv_pcie_hsdt_crg_reg_writel(plat, val, reg);
	}
}

static void nsv_pcie_clkreq_idle_timeout_cfg(struct hi_pcie_nsv *plat)
{
	u32 val;
	u32 reg;
	u32 size;
	u32 step;
	int behavior = CLKREQ_IDLE_TIMEOUT;

	size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		nsv_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void nsv_pcie_clkreq_debounce_cfg(struct hi_pcie_nsv *plat)
{
	// 2.5.1.2 CLKREQ# debounce cfg
	nsv_pcie_sysctrl_writel(plat, DEBOUNCE_CYCLES, HIPCIE_CTRL_REG31);

	// 2.5.1.3 CLKREQ# delay send controller
	if (!plat->fnpll_hw_auto_open) {
		nsv_pcie_sysctrl_writel(plat, BIT(4) | BIT(20), HIPCIE_CLKREQ_IN_N_CTRL); // bit4=0x1
		return;
	}
	nsv_pcie_sysctrl_writel(plat, BIT(20), HIPCIE_CLKREQ_IN_N_CTRL); // bit4=0x0
	nsv_pcie_sysctrl_writel(plat, 0x0, HIPCIE_CON_CLKREQ_IN_N_CNT_BASE); // @todo 0x0
}

static void nsv_pcie_iso_ctrl_cfg(struct hi_pcie_nsv *plat, bool enable)
{
	u32 val;
	u32 reg = HIPCIE_SOC2PHY_ISOEN;
	if (!enable) {
		val = nsv_pcie_sysctrl_readl(plat, reg);
		val = val & 0xFFFFFFFC;
		nsv_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void nsv_pcie_ctrl_memsd_exit(struct hi_pcie_nsv *plat, int behavior)
{
	u32 step;
	u32 val_mask;
	u32 reg;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		nsv_pcie_sysctrl_writel(plat, val_mask, reg);
	}
}

static void nsv_pcie_smmu_memsd_exit(struct hi_pcie_nsv *plat, int behavior, bool enable)
{
	int ret;
	u32 step;
	u32 val;
	u32 val_mask;
	u32 reg;
	u32 ori_val;
	unsigned long flags;
	spinlock_t *resource_vote_lock;

	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;
	u32 bit = ((plat->pcie->ctrl_id == RC_ID0) ? HI_PCIE_VOTE_MEM_SD_PCIE0 : HI_PCIE_VOTE_MEM_SD_PCIE1);
	resource_vote_lock = iodie_pcie_resource_vote_lock_get();

	ret = iodie_pcie_hwspinlock_request();
	if (ret != 0)
		return;

	spin_lock_irqsave(resource_vote_lock, flags);
	if (enable) {
		val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
		if ((atomic_add_return(1, &pcie_tcu_sd_cnt) == 1) && ((val & SCTRL_PCIE_RESOURCE_VOTE_MEM_SD_MASK) == 0x0))
			nsv_pcie_hsdt_sctrl_reg_writel(plat, 0x10000, HSDT_SCTRL_SMMU_SD);
		iodie_pcie_sctrl_reg_writel(BIT(bit) | (BIT(bit) << 16), SCTRL_PCIE_RESOURCE_VOTE_REG);
	} else if (!enable) {
		ori_val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
		iodie_pcie_sctrl_reg_writel(BIT(bit) << 16, SCTRL_PCIE_RESOURCE_VOTE_REG);
		val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
		if ((atomic_sub_return(1, &pcie_tcu_sd_cnt) == 0) && ((val & SCTRL_PCIE_RESOURCE_VOTE_MEM_SD_MASK) == 0x0))
			nsv_pcie_hsdt_sctrl_reg_writel(plat, 0x10001, HSDT_SCTRL_SMMU_SD);
	} else {
		spin_unlock_irqrestore(resource_vote_lock, flags);
		iodie_pcie_hwspinlock_release();
		return;
	}
	spin_unlock_irqrestore(resource_vote_lock, flags);
	iodie_pcie_hwspinlock_release();

	if (enable) {
		hi_pcie_pr_i("smmu memsd exit ori_vote=0x%x, vote=0x%x",
			val, iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG));
	} else {
		hi_pcie_pr_i("smmu memsd enter ori_vote=0x%x, vote=0x%x", ori_val, val);
	}

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		val = 0;
		if (enable) {
			val &= ~val_mask;
		} else {
			val |= val_mask;
		}
		val |= (val_mask << 16);
		nsv_pcie_hsdt_sctrl_reg_writel(plat, val, reg);
	}
}

static void nsv_pcie_mem_sd_exit(struct hi_pcie_nsv *plat, bool enable)
{
	nsv_pcie_ctrl_memsd_exit(plat, CTRL_SD_EXIT);

	nsv_pcie_smmu_memsd_exit(plat, SMMU_SD_EXIT, enable);
}

static void nsv_pcie_do_phy_reset(struct hi_pcie_nsv *plat, int behavior, bool reset)
{
	u32 step;
	u32 reg;
	u32 val;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		if (reset)
			val <<= 16;
		else
			val |= (val << 16);
		nsv_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void nsv_pcie_phy_apb_reset(struct hi_pcie_nsv *plat, int behavior)
{
	u32 step;
	u32 reg;
	u32 val_mask;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static void nsv_pcie_pcs_sel_clk(struct hi_pcie_nsv *plat)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = plat->ctrl;

	if (ctrl->pcie->mode == PCIE_MODE_RC)
		return;

	val = nsv_pcie_sysctrl_readl(plat, HIPCIE_PHY_CFG0);
	val |= (BIT(10) | BIT(26));
	nsv_pcie_sysctrl_writel(plat, val, HIPCIE_PHY_CFG0);
}

static void nsv_pcie_phy_cfg_x1(struct hi_pcie_nsv *plat)
{
	struct hi_pcie_phy *phy = &plat->pcie->phy;

	pcie_kphy_setup(phy);

	if (plat->pcie->phy_dfx_ops && plat->pcie->phy_dfx_ops->phy_param_change)
		plat->pcie->phy_dfx_ops->phy_param_change(phy);

	nsv_pcie_do_phy_reset(plat, PHY_RESET_N, false);
	nsv_pcie_do_phy_reset(plat, PIPE_RESET_N, false);
}

static void nsv_pcie_phy_cfg(struct hi_pcie_nsv *plat)
{
	if (plat->pcie->chip_type == BOARD_FPGA) {
		nsv_pcie_do_phy_reset(plat, PHY_RESET_N, false);
		nsv_pcie_do_phy_reset(plat, PIPE_RESET_N, false);
		return;
	}

	nsv_pcie_phy_cfg_x1(plat);
}

static void nsv_pcie_phy_init(struct hi_pcie_nsv *plat)
{
	nsv_pcie_do_phy_reset(plat, PHY_RST_N_SEL, false); // 2.7.1
	nsv_pcie_do_phy_reset(plat, PHY_PORN_RST, false); // 2.7.2
	nsv_pcie_phy_apb_reset(plat, PHY_APB_UNRESET); // 2.7.3
	nsv_pcie_pcs_sel_clk(plat);

	nsv_pcie_phy_cfg(plat);
}

static void nsv_pcie_reset_phy(struct hi_pcie_nsv *plat)
{
	nsv_pcie_do_phy_reset(plat, PIPE_RESET_N, true);
	nsv_pcie_do_phy_reset(plat, PHY_RESET_N, true);
	nsv_pcie_phy_apb_reset(plat, PHY_APB_RESET);
	nsv_pcie_do_phy_reset(plat, PHY_PORN_RST, true);
}

static int nsv_pcie_perst_from_gpio(struct hi_pcie_nsv *plat, int pull_up)
{
	int ret = 0;
	struct hi_pcie_port *port = NULL;
	list_for_each_entry(port, &plat->pcie->ports, node) {
		if ((pull_up == ENABLE)  && port->poweron_callback)
			ret = port->poweron_callback(port->power_callback_data);
		else if ((pull_up == DISABLE) && port->poweroff_callback)
			ret = port->poweroff_callback(port->power_callback_data);
		if (ret != 0)
			return ret;
	}
	return 0;
}

#define HIPCIE_MISC_CFG4_X4 0x044
#define SC_PCIE_CTRL_12		0x030

static int nsv_pcie_perst_from_pinctrl(struct hi_pcie_nsv *plat, int pull_up)
{
	u32 val;

	val = nsv_pcie_sysctrl_readl(plat, SC_PCIE_CTRL_12);
	if (pull_up) {
		// bit 0 set 1, bit 1 set 1
		val = val | BIT(0) | BIT(1);
	} else {
		// bit 0 set 0, bit 1 set 1
		val = val & 0xFFFFFFFE;
		val = val | BIT(1);
	}
	nsv_pcie_sysctrl_writel(plat, val, SC_PCIE_CTRL_12);

	return 0;
}

static int nsv_pcie_perst_config(struct hi_pcie_nsv *plat, int pull_up)
{
	int ret;

	if (plat->pcie->mode == PCIE_MODE_EP)
		return 0;

	if (pull_up)
		usleep_range(10, 11);

	if (plat->pcie->chip_type == BOARD_FPGA) {
		ret = nsv_pcie_perst_from_pinctrl(plat, pull_up);
	} else {
		ret = nsv_pcie_perst_from_gpio(plat, pull_up);
	}

	if (ret)
		hi_pcie_pr_e("Failed to pulse perst signal");

	usleep_range(10, 11);
	return ret;
}

static void nsv_pcie_ctrl_por_n_rst(struct hi_pcie_nsv *plat, bool reset)
{
	u32 val_mask;
	u32 reg;
	u32 step;
	int behavior = reset ? CTRL_PORN_RST : CTRL_PORN_UNRST;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		nsv_pcie_hsdt_crg_reg_writel(plat, val_mask, reg);
	}
}

static bool nsv_pcie_is_pipe_clk_stable(struct hi_pcie_nsv *plat, int behavior)
{
	u32 step;
	u32 val_mask;
	u32 reg;
	u32 val;
	u32 time = 100; /* try 100 times */
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		val = nsv_pcie_sysctrl_readl(plat, reg);
		while ((val & val_mask) != 0) {
			mdelay(1);
			if (time == 0) {
				hi_pcie_pr_e("PIPE clk is not stable, val = 0x%x", val);
				pcie_dsm_record_pipe_status(val);
				return false;
			}
			time--;
			val = nsv_pcie_sysctrl_readl(plat, reg);
		}
	}

	hi_pcie_pr_d("PIPE clk is stable in (1000-%u)*100 us", time);
	return true;
}

static void nsv_pcie_rx_term_cfg(struct hi_pcie_nsv *plat, int behavior)
{
	u32 step;
	u32 val_mask;
	u32 reg;
	u32 val;
	u32 size = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].size;

	for (step = 0; step < size; step++) {
		reg = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].base_addr[step];
		val_mask = pcie_init_step[plat->pcie->ctrl_id].behavior[behavior].val[step];
		val = nsv_pcie_sysctrl_readl(plat, reg);
		val = val | val_mask;
		nsv_pcie_sysctrl_writel(plat, val, reg);
	}
}

static void nsv_pcie_mid_auth_config(struct hi_pcie_nsv *plat, bool enable)
{
	if (plat->pcie->ctrl_id == RC_ID0)
		return;

#ifdef CONFIG_HIPCIE_SUPPORT_FFA
	int ret;
	struct ffa_send_direct_data args = {0};

	args.data0 = (u64)FID_PCIE_MID_AUTH_BYPASS;
	args.data1 = (u64)plat->pcie->ctrl_id;
	if (enable)
		args.data2 =  (u64)0x0; // enable
	else
		args.data2 =  (u64)0x1; // disable

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		hi_pcie_pr_e("mid auth config failed");
		return;
	}
#else
	struct arm_smccc_res res;
	arm_smccc_smc((u64)FID_PCIE_MID_AUTH_BYPASS, (u64)plat->pcie->ctrl_id,
		0, 0, 0, 0, 0, 0, &res);
#endif
	hi_pcie_pr_d("mid auth config");
}

static void nsv_pcie_firewall_fix(struct hi_pcie_nsv *plat)
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
	hi_pcie_pr_d("fix firewall");
}

static noinline uint64_t nsv_pcie_tbu_bypass(struct hi_pcie_nsv *plat)
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

static bool nsv_pcie_is_smmu_bypass(struct hi_pcie_nsv *plat)
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

static void nsv_pcie_init_smmu_map(struct hi_pcie_nsv *plat)
{
	u32 i;
	u32 reg_val;
	u32 cdx = plat->pcie->ctrl_id;

	if (plat->sub_stream_num > 0) {
		reg_val = nsv_pcie_sysctrl_readl(plat, nsv_pcie_smmu[cdx].mode_reg);
		reg_val &= ~BIT(0);
		nsv_pcie_sysctrl_writel(plat, reg_val, nsv_pcie_smmu[cdx].mode_reg);

		for (i = 0; i < plat->sub_stream_num; i++)
			nsv_pcie_sysctrl_writel(plat, plat->ssid_map_list[i],
				nsv_pcie_smmu[cdx].bdf_reg + i * sizeof(u32));
	} else {
		reg_val = nsv_pcie_sysctrl_readl(plat, nsv_pcie_smmu[cdx].mode_reg);
		reg_val |= BIT(0);
		nsv_pcie_sysctrl_writel(plat, reg_val, nsv_pcie_smmu[cdx].mode_reg);
	}

	reg_val = nsv_pcie_sysctrl_readl(plat, nsv_pcie_smmu[cdx].aw_reg);
	reg_val |= nsv_pcie_smmu[cdx].sid << 8;
	reg_val |= 1 << 28;
	nsv_pcie_sysctrl_writel(plat, reg_val, nsv_pcie_smmu[cdx].aw_reg);

	reg_val = nsv_pcie_sysctrl_readl(plat, nsv_pcie_smmu[cdx].ar_reg);
	reg_val |= nsv_pcie_smmu[cdx].sid << 8;
	reg_val |= 1 << 28;
	nsv_pcie_sysctrl_writel(plat, reg_val, nsv_pcie_smmu[cdx].ar_reg);
}

static int nsv_pcie_tbu_config(struct hi_pcie_nsv *plat, u32 enable)
{
	u32 reg_val;
	u32 tok_trans_gnt;
	u32 time = TBU_CONFIG_TIMEOUT;

	nsv_pcie_init_smmu_map(plat);

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

static u32 nsv_pcie_get_smmuid(struct hi_pcie *pcie)
{
	return nsv_pcie_smmu[pcie->ctrl_id].smmu_id;
}
static atomic_t *nsv_pcie_get_tcu_pwrcnt(struct hi_pcie *pcie)
{
	return nsv_pcie_smmu[pcie->ctrl_id].tcu_pwr_cnt;
}

static int nsv_pcie_smmu_tcu_cfg(struct hi_pcie_nsv *plat, bool enable)
{
	int ret = 0;
	unsigned long flag;
	u32 smmu_id = nsv_pcie_get_smmuid(plat->pcie);
	atomic_t *tcu_pwr_cnt = nsv_pcie_get_tcu_pwrcnt(plat->pcie);
	spinlock_t *resource_vote_lock = iodie_pcie_resource_vote_lock_get();
	u32 old_val = 0;
	u32 val = 0;

	ret = iodie_pcie_hwspinlock_request();
	if (ret != 0)
		return ret;

	spin_lock_irqsave(resource_vote_lock, flag);

	old_val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);

	if(enable) {
		nsv_pcie_common_unrst_cfg(plat);
		nsv_pcie_common_clk_cfg(plat);

		ret = mm_smmu_nonsec_tcuctl(smmu_id);
		if (ret) {
			hi_pcie_pr_e("TCU nonsec cfg Failed");
			goto CFG_END;
		}

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
		nsv_pcie_common_clk_uncfg(plat);
		nsv_pcie_common_rst_cfg(plat);
	}

CFG_END:
	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	spin_unlock_irqrestore(resource_vote_lock, flag);
	iodie_pcie_hwspinlock_release();

	hi_pcie_pr_i("tcu_cfg ori_vote=0x%x, vote=0x%x, tcu_pwr_cnt=%d, enable=%u",
		old_val, val, atomic_read(tcu_pwr_cnt), enable);

	return ret;
}

static int nsv_pcie_enable_smmu(struct hi_pcie_nsv *plat)
{
	int ret;

	ret = nsv_pcie_smmu_tcu_cfg(plat, true);
	if(ret)
		return ret;

	ret = nsv_pcie_tbu_config(plat, ENABLE);
	if (ret) {
		hi_pcie_pr_e("TBU config Failed");
		return -1;
	}
	hi_pcie_pr_d("TBU init Done");

	return 0;
}

static int nsv_pcie_disable_smmu(struct hi_pcie_nsv *plat)
{
	atomic_t *tcu_pwr_cnt;
	if (nsv_pcie_is_smmu_bypass(plat)) {
		hi_pcie_pr_e("smmu bypassed");
		return 0;
	}

	tcu_pwr_cnt = nsv_pcie_get_tcu_pwrcnt(plat->pcie);
	if ((atomic_read(tcu_pwr_cnt) && BIT(plat->pcie->ctrl_id)) == 0)
		return 0;

	(void)nsv_pcie_tbu_config(plat, DISABLE);

	return nsv_pcie_smmu_tcu_cfg(plat, false);
}

static int nsv_pcie_smmu_init(struct hi_pcie_nsv *plat)
{
	if (nsv_pcie_is_smmu_bypass(plat)) {
		if (nsv_pcie_tbu_bypass(plat)) {
			hi_pcie_pr_e("Bypass TBU failed");
			return -1;
		}

		return 0;
	}

	return nsv_pcie_enable_smmu(plat);
}

static void nsv_pcie_axi_timeout_set(struct hi_pcie_nsv *plat)
{
	u32 val;

	val = nsv_pcie_sysctrl_readl(plat, HIPCIE_MISC_CFG9_V3);
	val &= ~(0x3F << 16);
	val |= (0x2F << 16);
	nsv_pcie_sysctrl_writel(plat, val, HIPCIE_MISC_CFG9_V3);
}

static int nsv_pcie_turn_on(struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;
	unsigned long flags = 0;

	if (atomic_read(&(pcie->is_power_on))) {
		hi_pcie_pr_e("PCIe%u already power on", pcie->ctrl_id);
		return 0;
	}

	ret = pinctrl_select_state(plat->pinctrl, plat->pinctrls_default);
	if (ret)
		hi_pcie_pr_e("can not set pins to default state");

	ret = nsv_pcie_clk_config(plat);
	if (ret != 0) {
		hi_pcie_pr_e("PCIe%u clk config fail", pcie->ctrl_id);
		goto clk_config_fail;
	}

	nsv_pcie_qic_power_domain(plat, QIC_POWER_DOMAIN_ENTER); // 2.3.1

	nsv_pcie_rst_cfg(plat, PCIE_RST); // 2.3.2
	nsv_pcie_rst_cfg(plat, PCIE_UNRST); // 2.3.3
	nsv_pcie_sysclk_ctrl(plat, true); // 2.3.4

	nsv_pcie_mid_auth_config(plat, true);

	nsv_pcie_sc_rst_cfg(plat, false); // 2.3.6
	nsv_pcie_sc_clk_ctrl(plat, true); // 2.3.7

	nsv_pcie_fnpll_clk_cfg(plat); // 2.3.8

	if (plat->pcie->mode == PCIE_MODE_RC)
		nsv_pcie_oe_ie_cfg(plat, OE_IE_CFG_RC);
	else
		nsv_pcie_oe_ie_cfg(plat, OE_IE_CFG_EP);

	nsv_pcie_clkreq_idle_timeout_cfg(plat); // @todo check
	nsv_pcie_clkreq_debounce_cfg(plat); // 2.5

	nsv_pcie_iso_ctrl_cfg(plat, DISABLE); // 2.6

	nsv_pcie_phy_init(plat); // 2.7 FPGA or X1

	ret = nsv_pcie_perst_config(plat, ENABLE);
	if (ret != 0) {
		hi_pcie_pr_e("power on perst fail");
		goto perst_fail;
	}

	nsv_pcie_ctrl_por_n_rst(plat, false); // 2.10

	nsv_pcie_mem_sd_exit(plat, ENABLE);

	nsv_pcie_axi_timeout_set(plat);

	flags = hi_pcie_idle_spin_lock(pcie->idle, flags);
	atomic_add(1, &(pcie->is_power_on));
	hi_pcie_idle_spin_unlock(pcie->idle, flags);

	pcie_ctrl_enable_irq(pcie->ctrl);
	enable_irq((u32)plat->level2_irq);
	hi_pcie_idle_mask_int(pcie->idle);

	return 0;

perst_fail:
	nsv_pcie_sc_clk_ctrl(plat, false);

	nsv_pcie_reset_phy(plat);

	nsv_pcie_sc_rst_cfg(plat, true);

	nsv_pcie_sysclk_ctrl(plat, false);
	nsv_pcie_rst_cfg(plat, PCIE_RST);

	nsv_pcie_fnpll_clk_uncfg(plat, false);

clk_config_fail:
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

	return -1;
}

static int nsv_pcie_turn_off(struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;
	unsigned long flags = 0;

	if (atomic_read(&(pcie->is_power_on)) == 0) {
		hi_pcie_pr_e("PCIe%u already power down", pcie->ctrl_id);
		return 0;
	}

	hi_pcie_idle_mask_int(pcie->idle);
	hi_pcie_idle_clear_irq(pcie->idle);
	udelay(5);

	pcie_ctrl_disable_irq(pcie->ctrl);
	disable_irq((u32)plat->level2_irq);

	hi_pcie_idle_disable_irq(pcie->idle);

	pcie_core_flush_work(pcie);

	flags = hi_pcie_idle_spin_lock(pcie->idle, flags);
	atomic_sub(1, &(pcie->is_power_on));
	hi_pcie_idle_spin_unlock(pcie->idle, flags);

	hi_pcie_32k_idle_exit(pcie->idle);

	nsv_pcie_qic_power_domain(plat, QIC_POWER_DOMAIN_ENTER);

	ret = pcie_ctrl_reset(pcie->ctrl);
	if (ret)
		return ret;

	nsv_pcie_disable_smmu(plat);

	nsv_pcie_ctrl_por_n_rst(plat, true);

	nsv_pcie_sc_clk_ctrl(plat, false);

	nsv_pcie_reset_phy(plat);

	nsv_pcie_sc_rst_cfg(plat, true);

	nsv_pcie_mid_auth_config(plat, false);

	ret = nsv_pcie_perst_config(plat, DISABLE);
	if (ret != 0)
		hi_pcie_pr_e("power off perst fail");

	nsv_pcie_mem_sd_exit(plat, DISABLE);

	nsv_pcie_sysclk_ctrl(plat, false);
	nsv_pcie_rst_cfg(plat, PCIE_RST);

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

	nsv_pcie_fnpll_clk_uncfg(plat, false);

	return 0;
}

static int nsv_pcie_parse_base_config(struct platform_device *pdev, struct hi_pcie_nsv *plat)
{
	int ret;

	ret = device_property_read_u32(&pdev->dev, "clkreq_gpio", &plat->clkreq_gpio);
	if (ret != 0)
		hi_pcie_pr_e("clkreq_gpio not find");

	hi_pcie_pr_i("clkreq_gpio = %u", plat->clkreq_gpio);

	ret = device_property_read_u32(&pdev->dev, "ssid_num", &plat->sub_stream_num);
	if (ret != 0) {
		hi_pcie_pr_e("sub_stream_num not find");
		return 0;
	}

	if (plat->sub_stream_num != 0) {
		ret = device_property_read_u32_array(&pdev->dev, "ssid_map_list", plat->ssid_map_list, plat->sub_stream_num);
		if (ret != 0) {
			hi_pcie_pr_e("failed to get sub_stream_num map list, num %u", plat->sub_stream_num);
			return -1;
		}
	}

	return 0;
}

static int nsv_pcie_parse_pcie_base(struct platform_device *pdev, struct hi_pcie_nsv *plat)
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
	plat->pcie->phy.lane_num = PCIE_LANE_X1;
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

static int nsv_pcie_parse_plat_base(struct platform_device *pdev, struct hi_pcie_nsv *plat)
{
	struct device_node *np;
	int i;
	static struct {
		void __iomem **base;
		char *compatible;
		int index;
	} plat_base_list[] = {
		{ &plat_config.aocrg_base, "hisilicon,ao_crg", 0 },
		{ &plat_config.hsdtcrg_base, "hisilicon,hsdt_crg", 0 },
		{ &plat_config.hsdtsctrl_base, "hisilicon,hsdt_sysctrl", 0 },
		{ &plat_config.hsdtxctrl_base, "hisilicon,hsdt_ctrl", 0 },
		{ &plat_config.pcie_fnpll_base, "hi_pcie,plat_base", 0 },
		{ &plat_config.sctrl_base, "hisilicon,sysctrl", 0 },
	};

	if (atomic_add_return(1, &plat_config_cnt) != 1)
		return 0;

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

static struct {
	u32 ctrl_id;
	char *phy_compatibal;
} nsv_phy_compatible[] = {
	{
		RC_ID0, "pcie0,kphy",
	},
	{
		RC_ID1, "pcie1,kphy",
	},
};

static int nsv_pcie_parse_phy_param(struct platform_device *pdev, struct hi_pcie_nsv *plat)
{
	int ret;
	char *phy_compatibal_name;
	struct device_node *node;
	u32 size;
	u32 *reg = NULL;
	u32 *val = NULL;

	phy_compatibal_name = nsv_phy_compatible[plat->pcie->ctrl_id].phy_compatibal;
	hi_pcie_pr_e("get node phy_node_name %s", phy_compatibal_name);

	node = of_find_compatible_node(plat->pcie->dev->of_node, NULL, phy_compatibal_name);
	if (node == NULL) {
		hi_pcie_pr_e("get plat(%s) node failed", phy_compatibal_name);
		return -EINVAL;
	}

	ret = of_property_read_u32(node, "change-param-size", &size);
	if ((ret != 0) || size == 0) {
		hi_pcie_pr_e("need not change phy param, ret = %d, size = %u", ret, size);
		return 0;
	}
	hi_pcie_pr_e("get phy param size = %u", size);

	plat->pcie->phy.phy_change.change_param_size = size;
	reg = (u32 *)devm_kzalloc(&pdev->dev, sizeof(u32) * size, GFP_KERNEL);
	if (!reg) {
		hi_pcie_pr_e("reg malloc failed");
		return -EINVAL;
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

static int nsv_pcie_parse_plat_config(struct platform_device *pdev, struct hi_pcie_nsv *plat)
{
	int ret;

	ret = nsv_pcie_parse_base_config(pdev, plat);
	if (ret != 0)
		return ret;

	ret = nsv_pcie_parse_pcie_base(pdev, plat);
	if (ret != 0)
		return ret;

	ret = nsv_pcie_parse_plat_base(pdev, plat);
	if (ret != 0)
		return ret;

	nsv_pcie_parse_phy_param(pdev, plat);

	return 0;
}

static irqreturn_t nsv_pcie_pcs_handler(int irq, void *arg)
{
	hi_pcie_pr_e("pcie plat pcs handler enter");
	return IRQ_HANDLED;
}

#define AXI_TIMEOUT_MASK	BIT(11)
#define AXI_TIMEOUT_CLR		BIT(9)
#define AXI_TIMEOUT_STATUS	BIT(19)

static irqreturn_t nsv_pcie_level2_handler(int irq, void *arg)
{
	u32 val;
	struct hi_pcie_nsv *plat = arg;
	struct hi_pcie_port *port = NULL;
	struct hi_pcie *pcie = plat->pcie;

	if (atomic_read(&(plat->pcie->is_power_on)) == 0) {
		hi_pcie_pr_d("PCIe%u already power down, not support irq handler", plat->pcie->ctrl_id);
		return IRQ_HANDLED;
	}

	hi_pcie_pr_e("pcie plat level2 handler enter");

	val = nsv_pcie_sysctrl_readl(plat, HIPCIE_MISC_STATE0);
	if ((val & AXI_TIMEOUT_STATUS) == AXI_TIMEOUT_STATUS) {
		hi_pcie_pr_e("pcie axi timeout happended");
		list_for_each_entry(port, &pcie->ports, node)
			schedule_work(&port->events_work[HI_PCIE_EVENT_AXI_TIMEOUT]);
	}

	val = nsv_pcie_sysctrl_readl(plat, HIPCIE_PHY_CFG0);
	val |= AXI_TIMEOUT_MASK | AXI_TIMEOUT_CLR | (AXI_TIMEOUT_MASK << 16) | (AXI_TIMEOUT_CLR << 16);
	nsv_pcie_sysctrl_writel(plat, val, HIPCIE_PHY_CFG0);

	hi_pcie_handle_int(plat->ctrl);

	val = nsv_pcie_sysctrl_readl(plat, HIPCIE_PHY_CFG0);
	val &= ~AXI_TIMEOUT_MASK;
	nsv_pcie_sysctrl_writel(plat, val, HIPCIE_PHY_CFG0);

	return IRQ_HANDLED;
}

static int nsv_pcie_register_int(struct platform_device *pdev, struct hi_pcie_nsv *plat)
{
	int irq;
	int ret;

	irq = platform_get_irq(pdev, INT_INDEX_PCS);
	if (irq <= 0) {
		hi_pcie_pr_e("get pcs irq-%dfailed", irq);
		return -1;
	}
	ret = devm_request_irq(&pdev->dev, (u32)irq, nsv_pcie_pcs_handler,
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
	ret = devm_request_irq(&pdev->dev, (u32)irq, nsv_pcie_level2_handler,
			(unsigned long)IRQF_TRIGGER_HIGH, "pcie_level2", plat);
	if (ret != 0) {
		hi_pcie_pr_e("request level2 irq-%dfailed", irq);
		return ret;
	}
	plat->level2_irq = irq;
	disable_irq((u32)irq);

	return 0;
}

static int nsv_pcie_phy_register(struct hi_pcie_nsv *plat)
{
	pcie_kphy_dfx_register(plat->pcie);
	return 0;
}

static int nsv_pcie_pwron_callback(void *data)
{
	int ret;
	struct hi_pcie_port *port = data;
	usleep_range(port->t_ref2perst[0], port->t_ref2perst[1]);
	ret = gpio_direction_output(port->reset_gpio, ENABLE);
	usleep_range(port->t_perst2access[0], port->t_perst2access[1]);
	return ret;
}

static int nsv_pcie_pwroff_callback(void *data)
{
	struct hi_pcie_port *port = data;
	return gpio_direction_output(port->reset_gpio, DISABLE);
}

static void nsv_pcie_pwr_callback_register(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable)
			continue;
		if (port->ep_device_type != PCIE_DEVICE_HI110X) {
			hi_pcie_pr_i("%s[rc-%d]: ep_device_type = %d, not HI110X",
				__func__, port->pcie->ctrl_id, port->ep_device_type);
			port->poweron_callback = nsv_pcie_pwron_callback;
			port->poweroff_callback = nsv_pcie_pwroff_callback;
			port->power_callback_data = port;
		}
	}
}

static int nsv_pcie_pinctrl(struct platform_device *pdev, struct hi_pcie_nsv *plat)
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

static int nsv_pcie_init(struct platform_device *pdev, struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_nsv *plat;

	plat = devm_kzalloc(&pdev->dev, sizeof(struct hi_pcie_nsv), GFP_KERNEL);
	if (plat == NULL)
		return -ENOMEM;

	pcie->plat = (void *)plat;
	plat->pcie = pcie;
	plat->ctrl = pcie->ctrl;

	ret = nsv_pcie_parse_plat_config(pdev, plat);
	if (ret != 0) {
		devm_kfree(&pdev->dev, plat);
		return ret;
	}

	if (plat->pcie->mode == PCIE_MODE_RC) {
		ret = nsv_pcie_register_int(pdev, plat);
		if (ret != 0) {
			devm_kfree(&pdev->dev, plat);
			return ret;
		}
	}

	ret = nsv_pcie_phy_register(plat);

	nsv_pcie_pwr_callback_register(plat->pcie);

	ret = nsv_pcie_pinctrl(pdev, plat);
	if (ret) {
		hi_pcie_pr_e("pinctrl failed\n");
		return ret;
	}

#ifdef CONFIG_HIPCIE_ARMPC
	ret = iodie_pcie_hwspinlock_init();
	if (ret != 0)
		return ret;
#endif

	return ret;
}

static void nsv_pcie_phy_recfg_controller_ffe(struct hi_pcie *pcie)
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

static int nsv_pcie_phy_chk_recfg_ffe(struct hi_pcie_port *port)
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

	hi_pcie_pr_d("phy check succ");

	ret = pcie_kphy_cfg_ffe(phy);
	if (ret) {
		hi_pcie_pr_e("need to recfg ffe");
		nsv_pcie_phy_recfg_controller_ffe(port->pcie);
	}

	return 0;
}

static int nsv_pcie_start_link(struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;
	struct hi_pcie_port *port = NULL;

	hi_pcie_pr_d("+%s+", __func__);

	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable)
			continue;
		nsv_pcie_phy_chk_recfg_ffe(port);
	}

	if (!nsv_pcie_is_pipe_clk_stable(plat, PIPE_CLK_STABLE)) { // 2.13.1
		hi_pcie_pr_e("PIPE clk is not stable");
		return -1;
	}

	nsv_pcie_rx_term_cfg(plat, PU_RX_TERMIANTION); // 2.13.4

	nsv_pcie_qic_power_domain(plat, QIC_POWER_DOMAIN_EXIT); // 2.14

	nsv_pcie_firewall_fix(plat); // 2.16

	ret = nsv_pcie_smmu_init(plat);
	if (ret)
		return ret;

	hi_pcie_idle_clear_irq(pcie->idle);
	hi_pcie_32k_idle_init(pcie->idle);
	hi_pcie_idle_enable_irq(pcie->idle);

	return 0;
}

#ifdef CONFIG_HIPCIE_IDLE
static int nsv_pcie_idle_init(struct hi_pcie *pcie)
{
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;
	pcie->idle->idle_reg_base = plat->sysctrl_base;
	return 0;
}

static void nsv_pcie_idle_reg_cfg(struct hi_pcie *pcie, int index, u32 step)
{
	u32 base_step;
	u32 reg;
	u32 val;

	for (base_step = 0; base_step < pcie_idle_step[index].behavior[step].size; base_step++) {
		reg = pcie_idle_step[index].behavior[step].base_addr[base_step];
		val = pcie_idle_step[index].behavior[step].val[base_step];
		nsv_pcie_hsdt_crg_reg_writel(pcie->plat, val, reg);
	}
	return;
}

static int nsv_pcie_enter_idle(struct hi_pcie *pcie, u32 port_id)
{
	int ret;
	int i;
	int vote_id = ((pcie->ctrl_id == RC_ID0) ? DOZE_CLIENT_PCIE0 : DOZE_CLIENT_PCIE1);

	hi_pcie_pr_d("++%s++", __func__);

	if (atomic_read(&(pcie->is_power_on)) == 0)
		goto end;

	if (pcie->fnpll_fsm_ctrl == PCIE_PLL_FSM_CLOSE)
		nsv_pcie_fnpll_clk_uncfg(pcie->plat, true);

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

	nsv_pcie_idle_reg_cfg(pcie, i, CLOSE_RFF_CLK);

	ret = vote_enable_doze(vote_id);
	if (ret < 0)
		hi_pcie_pr_e("[%s], port %u/%u enable doze fail", __func__, pcie->ctrl_id, port_id);

end:
	hi_pcie_pr_d("--%s--", __func__);
	return 0;
}

static int nsv_pcie_exit_idle(struct hi_pcie *pcie, u32 port_id)
{
	int ret;
	int i;
	int vote_id = ((pcie->ctrl_id == RC_ID0) ? DOZE_CLIENT_PCIE0 : DOZE_CLIENT_PCIE1);

	hi_pcie_pr_d("++%s++", __func__);

	if (atomic_read(&(pcie->is_power_on)) == 0)
		goto end;

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

	/* fnpll must be locked before io enable */
	if (pcie->fnpll_fsm_ctrl == PCIE_PLL_FSM_CLOSE)
		nsv_pcie_fnpll_clk_cfg(pcie->plat);

	nsv_pcie_idle_reg_cfg(pcie, i, OPEN_RFF_CLK);

end:
	hi_pcie_pr_d("--%s--", __func__);
	return 0;
}
#endif

static int nsv_pcie_get_ssid_by_dev(struct hi_pcie *pcie, struct pci_dev *pdev)
{
	u32 i;
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;

	hi_pcie_pr_e("sub_stream_num %u, dev id %u", plat->sub_stream_num, pci_dev_id(pdev));

	for (i = 0; i < plat->sub_stream_num; i++) {
		if (plat->ssid_map_list[i] == pci_dev_id(pdev))
			return (int)i + 1;
	}

	return 0;
}

static int nsv_pcie_dump_aocrg_info(struct hi_pcie *pcie)
{
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;

	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} aocrg_info[] = {
		{ AOCRGSC_PLL_STAT, "SC_PLL_STAT" },
		{ AOCRGSCPERCLKEN1, "PERCLKEN1" },
		{ AOCRGSCPERSTAT1, "PERSTAT1" },
		{ AOCRGSCPERCLKEN2, "PERCLKEN2" },
		{ AOCRGSCPERSTAT5, "RSTSTAT5" },
		{ AOCRGSCPEREN6, "PER CLKEN6" },
		{ AOCRGSCCLKDIV3, "CLK DIV3" },
		{ AOCRGSCCLKDIV5, "CLK DIV5" },
		{ AOCRGSCCLKDIV9, "CLK DIV9" },
		{ AOCRGSCCLKDIV12, "CLK DIV12" },
		{ AOCRGSCCLKDIV16, "CLK DIV16" },
		{ AOCRGSCPERSTATUS17, "PERSTATUS17" },
		{ AOCRGSCPERSTATUS16, "PERSTATUS16" },
		{ AOCRGSCPERSTATUS18, "PERSTATUS18" },
		{ AOCRGAO_AUTODIV2, "PER RST STAT0" },
		{ AOCRGPLL_FSM_STAT0_SEC, "PLL_FSM_STAT0_SEC" },
		{ AOCRGSCSPLLCTRL0, "SCSPLLCTRL0" },
		{ AOCRGSCSPLLCTRL1, "SCSPLLCTRL1" },
	};

	hi_pcie_pr_i("PCIe Aocrg info");

	for (i = 0; i < ARRAY_SIZE(aocrg_info); i++) {
		val = nsv_pcie_ao_crg_reg_readl(plat, aocrg_info[i].reg);
		hi_pcie_pr_i("Aocrg %s reg [0x%x] is [0x%x]", aocrg_info[i].desc, aocrg_info[i].reg, val);
	}

	return 0;
}

static int nsv_pcie_dump_hsdt_xctrl_info(struct hi_pcie *pcie)
{
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;

	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} hsdt_xctrl_info[] = {
		{ HSDT0_QIC_WR_NOPENDING_0, "QIC_WR_NOPENDING" },
		{ HSDT0_QIC_RD_NOPENDING_0, "QIC_RD_NOPENDING" },
		{ HSDT0_QIC_TB_NOPENDING_0, "QIC_TB_NOPENDING" },
	};

	hi_pcie_pr_i("PCIe HSDT0 XCTRL info");

	for (i = 0; i < ARRAY_SIZE(hsdt_xctrl_info); i++) {
		val = nsv_pcie_hsdt_xctrl_reg_readl(plat, hsdt_xctrl_info[i].reg);
		hi_pcie_pr_i("HSDT0 XCTRL %s reg [0x%x] is [0x%x]", hsdt_xctrl_info[i].desc, hsdt_xctrl_info[i].reg, val);
	}

	return 0;
}

static int nsv_pcie_dump_hsdtcrg_info(struct hi_pcie *pcie)
{
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;

	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} hsdtcrg_info[] = {
		{ HSDT_PERCLKEN0, "PER CLKEN0" },
		{ HSDT_PERSTAT0, "PER STAT0" },
		{ HSDT_PERCLKEN1, "PER CLKEN1" },
		{ HSDT_PERSTAT1, "PER STAT1" },
		{ HSDT_PERRSTSTAT0, "PER RST STAT0" },
		{ HSDT_CRG_CLKDIV0, "CLK DIV0" },
		{ HSDT_PERISTAT1, "PER STAT1" },
		{ HSDT_PERISTAT2, "PER STAT2" },
		{ HSDT_CRG_PCIEPLL_PCIE_CTRL0, "PCIEPLL PCIE CTRL0" },
		{ HSDT_CRG_PCIEPLL_STATE, "PCIEPLL STATE" },
		{ HSDT_CRG_PCIECTRL0, "PCIECTRL0" },
		{ HSDT_CRG_PCIECTRL1, "PCIECTRL1" },
	};

	hi_pcie_pr_i("PCIe Hsdtcrg info");

	for (i = 0; i < ARRAY_SIZE(hsdtcrg_info); i++) {
		val = nsv_pcie_hsdt_crg_reg_readl(plat, hsdtcrg_info[i].reg);
		hi_pcie_pr_i("Hsdtcrg %s reg [0x%x] is [0x%x]", hsdtcrg_info[i].desc, hsdtcrg_info[i].reg, val);
	}

	return 0;
}

static int nsv_pcie_dump_sysctrl_info(struct hi_pcie *pcie)
{
	struct hi_pcie_nsv *plat = (struct hi_pcie_nsv *)pcie->plat;
	
	int i;
	u32 val;
	static struct {
		u32 reg;
		char *desc;
	} sysctrl_info[] = {
		{ HIPCIE_MISC_STATE0, "pcie misc state0" },
		{ HIPCIE_SC_PCIE_CTRL_STAT17, "sc pcie ctrl stat17" },
		{ HIPCIE_SC_PCIE_CTRL_STAT18, "sc pcie ctrl stat18" },
		{ HIPCIE_SC_PCIE_CTRL_STAT19, "sc pcie ctrl stat19" },
		{ HIPCIE_SC_PCIE_CTRL_STAT20, "sc pcie ctrl stat20" },
	};

	hi_pcie_pr_i("PCIe Sysctrl info");

	for (i = 0; i < ARRAY_SIZE(sysctrl_info); i++) {
		val = nsv_pcie_sysctrl_readl(plat, sysctrl_info[i].reg);
		hi_pcie_pr_i("Sysctrl %s reg [0x%x] is [0x%x]", sysctrl_info[i].desc, sysctrl_info[i].reg, val);
	}

	return 0;
}

static int nsv_pcie_dump_plat_info(struct hi_pcie *pcie)
{
	u32 val;

	nsv_pcie_dump_aocrg_info(pcie);
	nsv_pcie_dump_hsdt_xctrl_info(pcie);
	nsv_pcie_dump_hsdtcrg_info(pcie);
	nsv_pcie_dump_sysctrl_info(pcie);

	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	hi_pcie_pr_i("sctrl SCBAKDATA4_MSK=0x%x", val);

	if (pcie->phy_dfx_ops != NULL && pcie->phy_dfx_ops->phy_param_dump != NULL)
		pcie->phy_dfx_ops->phy_param_dump(&pcie->phy);

	return 0;
}

struct hi_pcie_platform_ops platform_ops = {
	.plat_init			= nsv_pcie_init,
	.plat_on			= nsv_pcie_turn_on,
	.plat_off			= nsv_pcie_turn_off,
	.start_link			= nsv_pcie_start_link,
#ifdef CONFIG_HIPCIE_IDLE
	.idle_init			= nsv_pcie_idle_init,
	.enter_idle			= nsv_pcie_enter_idle,
	.exit_idle			= nsv_pcie_exit_idle,
#endif
	.init_phy			= NULL,
	.get_ssid_by_dev	= nsv_pcie_get_ssid_by_dev,
	.dump_plat_info		= nsv_pcie_dump_plat_info,
};

int hi_pcie_register_ops(struct platform_device *pdev, struct hi_pcie *pcie)
{
	pcie->plat_ops = &platform_ops;

	hi_pcie_ctrl_register(pcie);
	hi_pcie_dfx_register(pcie);
	return 0;
}
