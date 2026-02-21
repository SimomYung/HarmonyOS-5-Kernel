/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Contexthub 9030 pcie sync driver.
 * Create: 2024-04-22
 */

#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/hwspinlock.h>
#include <securec.h>

#include "hwspinlock_internal.h"

#include <platform_include/smart/drivers/io_die/iodie_pcie.h>

#ifdef CONFIG_HIPCIE_SUPPORT_FFA
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#else
#include <platform_include/see/bl31_smc.h>
#endif
#include <linux/arm-smccc.h>

#define IODIE_FNPLL_LOCK_TIMEOUT					20
#define IODIE_FNPLL_LOCK_BIT						(0x1 << 4)

#define IODIE_PCIEPLL_RETRY_TIME					0x2
#define IODIE_PCIEPLL_STATE_OPEN					0x1
#define IODIE_PCIEPLL_STATE_CLOSE					0x2
#define IODIE_PCIEPLL_STATE_MASK					0x3

#define IODIE_HSDT_CRG_PCIEPLL_PCIE_CTRL0			0x190
#define IODIE_HSDT_CRG_PCIEPLL_PCIE_FCW				0x194
#define IODIE_HSDT_CRG_PCIEPLL_STATE				0x204

#define IODIE_FNPLL_PCIE_ANALOG_CFG0				0x0008
#define IODIE_FNPLL_PCIE_ANALOG_CFG1				0x000C
#define IODIE_FNPLL_PCIE_ANALOG_CFG2				0x0010
#define IODIE_FNPLL_PCIE_ANALOG_CFG3				0x0014
#define IODIE_FNPLL_PCIE_ADPLL_ATUNE_CFG			0x0018
#define IODIE_FNPLL_PCIE_ADPLL_PTUNE_CFG			0x001C
#define IODIE_FNPLL_PCIE_DTC_CFG					0x0024
#define IODIE_FNPLL_PCIE_FTDC_CFG					0x0028
#define IODIE_FNPLL_PCIE_FDTC_LMS_CFG				0x002C
#define IODIE_FNPLL_PCIE_DTC_LMS_CFG				0x0030
#define IODIE_FNPLL_PCIE_LPF_BW_I_T01				0x0034
#define IODIE_FNPLL_PCIE_LPF_BW_P_T01				0x0038
#define IODIE_FNPLL_PCIE_ADPLL_DCO_TMODE			0x003C
#define IODIE_FNPLL_PCIE_ADPLL_LPF_TEST_CFG			0x0040
#define IODIE_FNPLL_PCIE_ADPLL_DSM_CFG				0x0044
#define IODIE_FNPLL_PCIE_ADPLL_FTDC_LOCK_THR		0x0048
#define IODIE_FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR		0x004C
#define IODIE_FNPLL_PCIE_CHAOS_CNT_THR_CFG			0x0050
#define IODIE_FNPLL_PCIE_ADPLL_CLK_EN_CFG0			0x0054
#define IODIE_FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG		0x005C
#define IODIE_FNPLL_PCIE_DTC_LINEAR_TEST_MODE		0x0060
#define IODIE_FNPLL_PCIE_FREZ_LMS_PTHR				0x0064
#define IODIE_FNPLL_PCIE_ACQ_CNT_THR_CFG			0x0068
#define IODIE_FNPLL_PCIE_ADPLL_DLY_CFG				0x006C
#define IODIE_FNPLL_PCIE_PTUNE_FBDIV				0x0070

static struct {
	void __iomem *hsdtcrg_base;
	void __iomem *pcie_fnpll_base;
	void __iomem *sctrl_base;
} iodie_pcie_plat_config = {
	NULL, NULL, NULL
};

static DEFINE_SPINLOCK(resource_vote_lock);
static u32 chip_generation_type = 0;

#if IODIE_PCIE_IDLE
static u32 iodie_pcie_hsdt_crg_reg_readl(u32 reg)
{
	return readl(iodie_pcie_plat_config.hsdtcrg_base + reg);
}

static void iodie_pcie_hsdt_crg_reg_writel(u32 val, u32 reg)
{
	pr_debug("%s, value %x, reg %x", __func__, val, reg);
	writel(val, iodie_pcie_plat_config.hsdtcrg_base + reg);
}

static u32 iodie_pcie_fnpll_reg_readl(u32 reg)
{
	return readl(iodie_pcie_plat_config.pcie_fnpll_base + reg);
}

static void iodie_pcie_fnpll_reg_writel(u32 val, u32 reg)
{
	pr_debug("%s, value %x, reg %x", __func__, val, reg);
	writel(val, iodie_pcie_plat_config.pcie_fnpll_base + reg);
}

u32 iodie_pcie_sctrl_reg_readl(u32 reg)
{
	u32 val = readl(iodie_pcie_plat_config.sctrl_base + reg);
	pr_debug("%s, value 0x%08x, reg 0x%08x.\n", __func__, val, reg);
	return val;
}

void iodie_pcie_sctrl_reg_writel(u32 val, u32 reg)
{
	pr_debug("%s, value 0x%08x, reg 0x%08x.\n", __func__, val, reg);
	writel(val, iodie_pcie_plat_config.sctrl_base + reg);
}

static void iodie_pcie_es_fnpll_cfg_init(void)
{
	iodie_pcie_fnpll_reg_writel(0x0000008A, IODIE_FNPLL_PCIE_ANALOG_CFG0);
	iodie_pcie_fnpll_reg_writel(0x0000000C, IODIE_FNPLL_PCIE_ANALOG_CFG1);
	iodie_pcie_fnpll_reg_writel(0x000002BE, IODIE_FNPLL_PCIE_ANALOG_CFG2);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_ANALOG_CFG3);
	iodie_pcie_fnpll_reg_writel(0x00300000, IODIE_FNPLL_PCIE_ADPLL_ATUNE_CFG);
	iodie_pcie_fnpll_reg_writel(0x026020A0, IODIE_FNPLL_PCIE_ADPLL_PTUNE_CFG);
	iodie_pcie_fnpll_reg_writel(0x00012ABC, IODIE_FNPLL_PCIE_DTC_CFG);
	iodie_pcie_fnpll_reg_writel(0x05866E1F, IODIE_FNPLL_PCIE_FTDC_CFG);

	iodie_pcie_fnpll_reg_writel(0x6420030A, IODIE_FNPLL_PCIE_FDTC_LMS_CFG);
	iodie_pcie_fnpll_reg_writel(0x5620153F, IODIE_FNPLL_PCIE_DTC_LMS_CFG);
	iodie_pcie_fnpll_reg_writel(0x2014008E, IODIE_FNPLL_PCIE_LPF_BW_I_T01);
	iodie_pcie_fnpll_reg_writel(0x00006543, IODIE_FNPLL_PCIE_LPF_BW_P_T01);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_ADPLL_DCO_TMODE);
	iodie_pcie_fnpll_reg_writel(0x00010000, IODIE_FNPLL_PCIE_ADPLL_LPF_TEST_CFG);
	iodie_pcie_fnpll_reg_writel(0x00000003, IODIE_FNPLL_PCIE_ADPLL_DSM_CFG);
	iodie_pcie_fnpll_reg_writel(0x000240FF, IODIE_FNPLL_PCIE_ADPLL_FTDC_LOCK_THR);

	iodie_pcie_fnpll_reg_writel(0x0080103F, IODIE_FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR);
	iodie_pcie_fnpll_reg_writel(0x80800A0A, IODIE_FNPLL_PCIE_CHAOS_CNT_THR_CFG);
	iodie_pcie_fnpll_reg_writel(0x8848D00F, IODIE_FNPLL_PCIE_ADPLL_CLK_EN_CFG0);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG);
	iodie_pcie_fnpll_reg_writel(0x00100010, IODIE_FNPLL_PCIE_DTC_LINEAR_TEST_MODE);
	iodie_pcie_fnpll_reg_writel(0x0020A420, IODIE_FNPLL_PCIE_FREZ_LMS_PTHR);
	iodie_pcie_fnpll_reg_writel(0x01FE1FFF, IODIE_FNPLL_PCIE_ACQ_CNT_THR_CFG);
	iodie_pcie_fnpll_reg_writel(0x000FF102, IODIE_FNPLL_PCIE_ADPLL_DLY_CFG);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_PTUNE_FBDIV);

	udelay(1);
}

static void iodie_pcie_cs_fnpll_cfg_init(void)
{
	iodie_pcie_fnpll_reg_writel(0x000000CA, IODIE_FNPLL_PCIE_ANALOG_CFG0);
	iodie_pcie_fnpll_reg_writel(0x0000000C, IODIE_FNPLL_PCIE_ANALOG_CFG1);
	iodie_pcie_fnpll_reg_writel(0x16204036, IODIE_FNPLL_PCIE_ANALOG_CFG2);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_ANALOG_CFG3);
	iodie_pcie_fnpll_reg_writel(0x0032C000, IODIE_FNPLL_PCIE_ADPLL_ATUNE_CFG);
	iodie_pcie_fnpll_reg_writel(0x026020A0, IODIE_FNPLL_PCIE_ADPLL_PTUNE_CFG);
	iodie_pcie_fnpll_reg_writel(0x00012D3F, IODIE_FNPLL_PCIE_DTC_CFG);
	iodie_pcie_fnpll_reg_writel(0x05366E1F, IODIE_FNPLL_PCIE_FTDC_CFG);

	iodie_pcie_fnpll_reg_writel(0x6420030A, IODIE_FNPLL_PCIE_FDTC_LMS_CFG);
	iodie_pcie_fnpll_reg_writel(0x5620163F, IODIE_FNPLL_PCIE_DTC_LMS_CFG);
	iodie_pcie_fnpll_reg_writel(0x20037015, IODIE_FNPLL_PCIE_LPF_BW_I_T01);
	iodie_pcie_fnpll_reg_writel(0x00003B25, IODIE_FNPLL_PCIE_LPF_BW_P_T01);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_ADPLL_DCO_TMODE);
	iodie_pcie_fnpll_reg_writel(0x00010000, IODIE_FNPLL_PCIE_ADPLL_LPF_TEST_CFG);
	iodie_pcie_fnpll_reg_writel(0x00000003, IODIE_FNPLL_PCIE_ADPLL_DSM_CFG);
	iodie_pcie_fnpll_reg_writel(0x000AA0FF, IODIE_FNPLL_PCIE_ADPLL_FTDC_LOCK_THR);

	iodie_pcie_fnpll_reg_writel(0x0078103F, IODIE_FNPLL_PCIE_ADPLL_FTDC_UNLOCK_THR);
	iodie_pcie_fnpll_reg_writel(0x80780A0A, IODIE_FNPLL_PCIE_CHAOS_CNT_THR_CFG);
	iodie_pcie_fnpll_reg_writel(0x8848D00F, IODIE_FNPLL_PCIE_ADPLL_CLK_EN_CFG0);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_MAN_TRIG_LMS_CNT_CFG);
	iodie_pcie_fnpll_reg_writel(0x00100010, IODIE_FNPLL_PCIE_DTC_LINEAR_TEST_MODE);
	iodie_pcie_fnpll_reg_writel(0x0020A420, IODIE_FNPLL_PCIE_FREZ_LMS_PTHR);
	iodie_pcie_fnpll_reg_writel(0x01FE1FFF, IODIE_FNPLL_PCIE_ACQ_CNT_THR_CFG);
	iodie_pcie_fnpll_reg_writel(0x000FF102, IODIE_FNPLL_PCIE_ADPLL_DLY_CFG);
	iodie_pcie_fnpll_reg_writel(0x00000000, IODIE_FNPLL_PCIE_PTUNE_FBDIV);

	udelay(1);
}

static void iodie_pcie_fnpll_cfg_init(void)
{
	if (chip_generation_type == 0)
		iodie_pcie_es_fnpll_cfg_init();
	else
		iodie_pcie_cs_fnpll_cfg_init();
}

static int iodie_pcie_fnpll_freq_init(u32 cnt)
{
	u32 val;
	u32 val_mask = IODIE_FNPLL_LOCK_BIT;
	u32 timeout = IODIE_FNPLL_LOCK_TIMEOUT;

	// EN=0&&BYPASS=1
	iodie_pcie_hsdt_crg_reg_writel(0x00000002, IODIE_HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	if (chip_generation_type == 0)
		iodie_pcie_hsdt_crg_reg_writel(0x2EE00000, IODIE_HSDT_CRG_PCIEPLL_PCIE_FCW);
	else
		iodie_pcie_hsdt_crg_reg_writel(0x2C455555, IODIE_HSDT_CRG_PCIEPLL_PCIE_FCW);

	// EN=1&&BYPASS=1
	iodie_pcie_hsdt_crg_reg_writel(0x00000003, IODIE_HSDT_CRG_PCIEPLL_PCIE_CTRL0);

	udelay(50);

	val = iodie_pcie_hsdt_crg_reg_readl(IODIE_HSDT_CRG_PCIEPLL_STATE);
	while ((val & val_mask) == 0) {
		udelay(10);
		if (timeout == 0) {
			pr_err("[%s] FNPLL lock timeout, val 0x%x, cnt %u.\n", __func__, val, cnt);
			return -1;
		}
		timeout--;
		val = iodie_pcie_hsdt_crg_reg_readl(IODIE_HSDT_CRG_PCIEPLL_STATE);
	}

	// EN=1&&BYPASS=0
	iodie_pcie_hsdt_crg_reg_writel(0x00000001, IODIE_HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	pr_info("[%s] FNPLL lock succ, cnt %u.\n", __func__, cnt);
	return 0;
}

static void iodie_pcie_fnpll_clk_init(void)
{
	u32 i;

	for (i = 0; i < IODIE_PCIEPLL_RETRY_TIME; i++) {
		iodie_pcie_fnpll_cfg_init();
		if (iodie_pcie_fnpll_freq_init(i) == 0x0)
			break;
	}
}

void iodie_pcie_fnpll_request(void)
{
	u32 val;
	u32 fnpll_val;
	u32 bit = (BIT(HI_PCIE_VOTE_FNPLL_PCIE1_ACPU) | BIT(HI_PCIE_VOTE_FNPLL_PCIE1_IOMCU));

	fnpll_val = iodie_pcie_hsdt_crg_reg_readl(IODIE_HSDT_CRG_PCIEPLL_PCIE_CTRL0);
	pr_info("[%s]fnpll[0x%x]=0x%x.\n", __func__, IODIE_HSDT_CRG_PCIEPLL_PCIE_CTRL0, fnpll_val);

	val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	if ((fnpll_val & IODIE_PCIEPLL_STATE_MASK) == IODIE_PCIEPLL_STATE_CLOSE) { // only pcie1 use
		pr_info("[%s] open pcie fnpll.\n", __func__);
		iodie_pcie_fnpll_clk_init();
	}
	iodie_pcie_sctrl_reg_writel(bit | (bit << 16), SCTRL_PCIE_RESOURCE_VOTE_REG);
	pr_info("[%s]ori_vote=0x%x, vote_val=0x%08x.\n", __func__, val,
		iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG));
}

void iodie_pcie_fnpll_release(void)
{
	u32 bit = HI_PCIE_VOTE_FNPLL_PCIE1_ACPU;
	u32 val = iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG);
	iodie_pcie_sctrl_reg_writel(BIT(bit) << 16, SCTRL_PCIE_RESOURCE_VOTE_REG);
	pr_info("[%s]ori_vote=0x%x, vote_val=0x%08x.\n", __func__, val,
		iodie_pcie_sctrl_reg_readl(SCTRL_PCIE_RESOURCE_VOTE_REG));
}
#endif

static int iodie_pcie_op(struct iodie_pcie_cmd_op *data, uint32_t cmd)
{
	if (data == NULL) {
		pr_err("iodie_pcie_op: data is null\n");
		return -EINVAL;
	}

	data->host_id = 0x1;
	data->port_id = 0x0;
	data->cmd = cmd;

	pr_info("%s: host_id[0x%x], port_id[0x%x], cmd[0x%x], x1[0x%x], x2[0x%x], x3[0x%x], x4[0x%x]\n",
		__func__, data->host_id, data->port_id, data->cmd, data->x1, data->x2, data->x3, data->x4);

	return iodie_commu(IODIE_CMD_PCIE, data, sizeof(struct iodie_pcie_cmd_op));
}

void iodie_pcie_power_config(uint32_t ctrl)
{
	struct iodie_pcie_cmd_op data = {0};
	data.x1 = ctrl;
	iodie_pcie_op(&data, IODIE_PCIE_CMD_POWER);
}

uint32_t iodie_pcie_link_state_get(void)
{
	struct iodie_pcie_cmd_op data = {0};
	iodie_pcie_op(&data, IODIE_PCIE_CMD_LINK_STATE_GET);
	return data.x1;
}

uint32_t iodie_pcie_link_speed_get(void)
{
	struct iodie_pcie_cmd_op data = {0};
	iodie_pcie_op(&data, IODIE_PCIE_CMD_LINK_SPEED_GET);
	return data.x1;
}

uint32_t iodie_pcie_ram_test(void)
{
	struct iodie_pcie_cmd_op data = {0};
	iodie_pcie_op(&data, IODIE_PCIE_CMD_RAM_TEST);
	return data.x1;
}

void iodie_pcie_lp_config(uint32_t mode)
{
	struct iodie_pcie_cmd_op data = {0};
	data.x1 = mode;
	iodie_pcie_op(&data, IODIE_PCIE_CMD_LP);
}

#ifdef CONFIG_DFX_DEBUG_FS
static u32 g_quadrant[4] = {QUADRANT1, QUADRANT4, QUADRANT2, QUADRANT3};
static int g_dir[][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
static char g_res[GRAPH_LEN][GRAPH_LEN + 1];
static char g_eye_title[] = "FEDCBA98765432100123456789ABCDEF";
static char iodie_pcie_phy_eye_pattern_config(u32 quadrant, u32 vref, u32 pi, enum iodie_pcie_eyefom_mode mode)
{
	char str = 0;
	struct iodie_pcie_cmd_op data = {0};

	data.x1 = quadrant;
	data.x2 = vref;
	data.x3 = pi;
	data.x4 = (u32)mode;
	iodie_pcie_op(&data, IODIE_PCIE_CMD_PHY_EYE_PATTERN);

	// ' ' = 0x0, '-' = 0x1, '+' = 0x2, 'x' = 0x3, 's' = 0xf
	switch (data.x1) {
	case 0x0:
		str = ' ';
		break;
	case 0x1:
		str = '-';
		break;
	case 0x2:
		str = '+';
		break;
	case 0x3:
		str = 'x';
		break;
	default:
		str = 's';
		break;
	}

	return str;
}

int iodie_pcie_phy_eye_pattern(uint32_t mode)
{
	int i, j, pi_step, vref;
	char ch, out[IMAGE_WIDTH];

	if (memset_s(g_res, sizeof(g_res), 'x', sizeof(g_res)))
		return -1;

	for (i = 0; i < 4; i++) {
		for (vref = 0; vref < QUA_LEN; vref++) {
			for (pi_step = 0; pi_step < QUA_LEN; pi_step++) {
				ch = iodie_pcie_phy_eye_pattern_config(g_quadrant[i], (u32)vref, (u32)pi_step, mode);
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
		pr_err("%s\n", out);
	}

	return 0;
}

int iodie_pcie_kphy_param_dump(void)
{
	int i;
	struct iodie_pcie_cmd_op data = {0};
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
	};

	for (i = 0; i < ARRAY_SIZE(kphy_info); i++) {
		data.x1 = kphy_info[i].reg;
		data.x2 = kphy_info[i].msb;
		data.x3 = kphy_info[i].lsb;
		data.x4 = kphy_info[i].data;
		iodie_pcie_op(&data, IODIE_PCIE_CMD_PHY_PARAM_DUMP);

		if (kphy_info[i].data == ((u32)0xFFFFFFFF))
			pr_err("[%s] reg: 0x%x, bit [%u:%u]:0x%x.\n", kphy_info[i].desc,
				kphy_info[i].reg, kphy_info[i].msb, kphy_info[i].lsb, data.x1);
	}

	return 0;
}

int iodie_pcie_auth_mid_config(u32 enable)
{
#ifdef CONFIG_HIPCIE_SUPPORT_FFA
	int ret;
	struct ffa_send_direct_data args = {0};

	args.data0 = (u64)FID_PCIE_MID_AUTH_BYPASS;
	args.data1 = (u64)0x1; // pcie id
	if (enable)
		args.data2 =  (u64)0x0; // enable
	else
		args.data2 =  (u64)0x1; // disable

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		pr_err("[%s] send msg fail, enable %u.\n", __func__, enable);
		return -1;
	}
#else
	struct arm_smccc_res res;
	arm_smccc_smc((u64)FID_PCIE_MID_AUTH_BYPASS, (u64)0x1,
		0, 0, 0, 0, 0, 0, &res);
#endif
	pr_info("[%s] send msg succ, enable %u.\n", __func__, enable);
	return 0;
}
#endif

void iodie_pcie_regs_dump(void)
{
	struct iodie_pcie_cmd_op data = {0};
	iodie_pcie_op(&data, IODIE_PCIE_CMD_REG_DUMP);
}

#define PCIE_COMMON_RESOURCE_HWSPINLOCK_ID	66

static struct {
	u32 enable;
	struct hwspinlock *lock;
} hwspinlock_config = {
	0, NULL
};

static int iodie_pcie_hwspinlock_init(void)
{
	int ret;
	struct device_node *np;
	char *compatible = "hi_pcie,hwspinlock";

	np = of_find_compatible_node(NULL, NULL, compatible);
	if (!np) {
		pr_err("[%s]Failed to get %s node", __func__, compatible);
		return -EFAULT;
	}

	ret = of_property_read_u32(np, "hwspinlock_enable", &hwspinlock_config.enable);
	if (ret != 0) {
		pr_err("[%s]hwspinlock_enable not find", __func__);
		hwspinlock_config.enable = 0;
	}

	if (hwspinlock_config.enable == 0x1) {
		pr_err("[%s]hwspinlock is enable", __func__);
		hwspinlock_config.lock = hwspin_lock_request_specific(PCIE_COMMON_RESOURCE_HWSPINLOCK_ID);
		if (!hwspinlock_config.lock) {
			pr_err("[%s]hwspinlock request fail", __func__);
			return -EFAULT;
		}
	}

	ret = of_property_read_u32(np, "asic_type", &chip_generation_type);
	if (ret != 0) {
		pr_err("[%s]asic_type not find", __func__);
		chip_generation_type = 0;
	}

	pr_info("chip_generation_type:0x%x", chip_generation_type);

	pr_err("%s ok", __func__);
	return 0;
}

int iodie_pcie_hwspinlock_request(void)
{
	int ret;
	unsigned long time;
	unsigned long timeout;
	struct hwspinlock *lock = hwspinlock_config.lock;

	if (hwspinlock_config.enable == 0x0)
		return 0;

	timeout = jiffies + msecs_to_jiffies(100);

	do {
		ret = lock->bank->ops->trylock(lock);
		if (ret == 0) {
			time = jiffies;
			if (time_after(time, timeout)) {
				pr_err("[%s]get hwspinlock timeout\n", __func__);
				return -1;
			}
		}
	} while (ret == 0);

	return 0;
}

void iodie_pcie_hwspinlock_release(void)
{
	struct hwspinlock *lock = hwspinlock_config.lock;

	if (hwspinlock_config.enable == 0x0)
		return;

	lock->bank->ops->unlock(lock);
}

spinlock_t *iodie_pcie_resource_vote_lock_get(void)
{
	return &resource_vote_lock;
}

u32 iodie_pcie_asic_tyep_get(void)
{
	return chip_generation_type;
}

static int iodie_pcie_plat_dtsinfo_init(void)
{
	struct device_node *np;
	int i;
	static struct {
		void __iomem **base;
		char *compatible;
		u32 index;
	} iodie_pcie_plat_base_list[] = {
		{ &iodie_pcie_plat_config.hsdtcrg_base, "hisilicon,hsdt_crg", 0 },
		{ &iodie_pcie_plat_config.pcie_fnpll_base, "hi_pcie,plat_base", 0 },
		{ &iodie_pcie_plat_config.sctrl_base, "hisilicon,sysctrl", 0 },
	};

	for (i = 0; i < ARRAY_SIZE(iodie_pcie_plat_base_list); i++) {
		np = of_find_compatible_node(NULL, NULL, iodie_pcie_plat_base_list[i].compatible);
		if (!np) {
			pr_err("Failed to get %s node", iodie_pcie_plat_base_list[i].compatible);
			goto unmap_base;
		}
		*iodie_pcie_plat_base_list[i].base = of_iomap(np, iodie_pcie_plat_base_list[i].index);
		if (*iodie_pcie_plat_base_list[i].base == NULL) {
			pr_err("Failed to iomap %s-%u", iodie_pcie_plat_base_list[i].compatible,
				iodie_pcie_plat_base_list[i].index);
			goto unmap_base;
		}
	}

	pr_err("%s ok", __func__);
	return 0;

unmap_base:
	for (i = 0; i < ARRAY_SIZE(iodie_pcie_plat_base_list); i++) {
		if (*iodie_pcie_plat_base_list[i].base != NULL) {
			iounmap(*iodie_pcie_plat_base_list[i].base);
			*iodie_pcie_plat_base_list[i].base = NULL;
		}
	}
	return -1;
}

static int __init iodie_pcie_sync_init(void)
{
	int ret;

	ret = iodie_pcie_hwspinlock_init();
	if (ret != 0)
		return ret;

	ret = iodie_pcie_plat_dtsinfo_init();
	if (ret != 0) {
		return ret;
	}

	pr_info("%s ok\n", __func__);

	return 0;
}

static void __exit iodie_pcie_sync_exit(void)
{
	if (hwspinlock_config.lock)
		hwspin_lock_free(hwspinlock_config.lock);

	pr_info("exit %s\n", __func__);
}

arch_initcall_sync(iodie_pcie_sync_init);
module_exit(iodie_pcie_sync_exit);

