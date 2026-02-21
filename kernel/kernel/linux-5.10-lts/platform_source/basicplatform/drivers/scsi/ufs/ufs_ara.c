/*
 * ufs_scorpio.c
 *
 * ufs configuration for scorpio
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define pr_fmt(fmt) "ufshcd :" fmt
#include "dsm_ufs.h"
#include "hufs_hcd.h"
#include "hufs_plat.h"
#include "ufshcd.h"
#include "unipro.h"
#include "ufshcd_extend.h"
#include <linux/platform_drivers/lpcpu_idle_sleep.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <platform_include/see/fbe_ctrl.h>
#include <platform_include/see/bl31_smc.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include "ufs_trap.h"

#define POLLING_RETRY 2000
#define POLLING_RETRY_4W 40000
#define MPHY_BIT_REG_VCO_SW_VCAL BIT(7)
#define UNIPRO_CLK_AUTO_GATE_DISABLE 0x3000000
#define UNIPRO_CLK_AUTO_GATE_CONFIG 0x30067FF
#define UFSHC_CLOCK_GATING_CONFIG 0x3FFF
#define UFS_SYSCTRL_RESET_CTRL_OFFSET 0x8
#define UFS_SYSCTRL_CRG_CLK_OFFSET 0x0

#define MPHY_SR_FIX_SIZE 4

#define UFS_TP_MEMORY_TRA BIT(20)
#define UFS_SP_MEMORY_TRA BIT(15)
#define UFS_SYSQIC_IB_RAM_TMOD_OFFSET 0x104
#define UFS_SYSCTRL_UFS_AXI_CTRL_OFFSET 0x210
#define UFS_SYSCTRL_MEMORY_CTRL_TP_OFFSET 0x280
#define UFS_SYSCTRL_AO_MEMORY_CTRL_OFFSET 0x284
#define UFS_SYSCTRL_MEMORY_CTRL_OFFSET 0x288
#define UFS_MPHYMCU_SC_REG_UCE_M3_MEMCTRL_OFFSET 0x00C

/****** Unipro Registers Offsets ******/
#define TXTRAILINGCLOCKSVALUE   0xFF

/****** MPHY Registers ******/
#define PCS_LAN0_MPHY_TX_EN 0x120
#define PCS_LAN1_MPHY_TX_EN 0x0520
#define PCS_LAN0_MPHY_RX_EN 0x0808
#define PCS_LAN1_MPHY_RX_EN 0x0C08
#define MPHY_TX_EN_BIT (0x1 << 0)
#define MPHY_RX_EN_BIT (0x1 << 0)
#define PCS_RX0_STALL_LOW_PWR_EN 0x0910
#define PCS_RX1_STALL_LOW_PWR_EN 0x0D10
#define STALL_LOW_PWR_EN_BIT 0x1
#define PCS_LAN0_DIFN_TIMEOUT 0x878
#define PCS_LAN1_DIFN_TIMEOUT 0xC78
#define DIFN_TIMEOUT_VAL 0x19

/****** MPHY PMA Registers ******/
#define PMA_API_CSR0 0x80C4
#define TOP_REFCLK_SEL_API_PIN (0x1 << 0)
#define TOP_INIT_TARGET_RATE_API_HG1 (0x1 << 1)
#define TOP_REFCLK_FREQ_SEL_API_38_4 (0x2 << 4)
#define PMA_API_CSR1 0x80C8
#define PMA_API_CSR7 0x80E0
#define TOP_FSM_INIT_STATUS_API_SUCCESS 9
#define TOP_FSM_INIT_STATUS_API_MASK 0x1F
#define PMA_API_CSR13 0x80F8
#define EFUSE_UFS0_MPHY_MASK 0x1F
#define PMA_EFUSE 0x80fc
#define PMA_TOP_FSM_CTRL 0x20054
#define FSM_SRAM_MAX_ADDR (0xA000 << 5)
#define CORE_FREQ_38_4 0x2
#define CORE_FREQ_417 0x10
#define PMA_CP_CSR17 0x20044
#define TOP_FSM_START_EN 0x1
#define TOP_FSM_START_ADDR (0x30 << 1)
#define PMA_CP_CSR8 0x20020
#define TOP_FSM_DONE (0x1 << 22)
#define PMA_API_CSR3 0x80D0
#define CSR3_DS_STALL_EN_API_BIT (0x1 << 18)

#define EFUSE_UFS1_DATA8_MASK 0xFFC000FF
#define EFUSE_UFS1_DATA9_MASK 0x3FFF
#define EFUSE_UFS1_DATA9_OFFSET 0x8
#define EFUSE_UFS1_DATA2_MASK 0x1E0

#define MPHY_FSM_WAIT_TIME 500000

/****** MPHY PCS Registers ******/
#define PCS_SYS_CLK_SW_MODE 0x8620
#define SYS_CLK_SW_MODE_PMA (0x1 << 0)
#define SYS_CLK_SW_MODE_SOC (0x0 << 0)
#define PCS_SC_UFS_CLK_DIV_MODE 0x81D0
#define SC_UFS_CLK_DIV_1 0x0
#define SC_UFS_CLK_DIV_2 0x1

#define PCS_TX0_CAP_WP 0x13C
#define PCS_TX1_CAP_WP 0x53C
#define PCS_RX0_CAP_WP 0x804
#define PCS_RX1_CAP_WP 0xC04
#define PCS_RX0_HS_G1_SYNC_LENGTH_CAPABILITY 0xA2C
#define PCS_RX1_HS_G1_SYNC_LENGTH_CAPABILITY 0xE2C
#define PCS_RX0_HS_G2_SYNC_LENGTH_CAPABILITY 0xA50
#define PCS_RX1_HS_G2_SYNC_LENGTH_CAPABILITY 0xE50
#define PCS_RX0_HS_G3_SYNC_LENGTH_CAPABILITY 0xA54
#define PCS_RX1_HS_G3_SYNC_LENGTH_CAPABILITY 0xE54
#define PCS_RX0_HS_G4_SYNC_LENGTH_CAPABILITY 0xA6C
#define PCS_RX1_HS_G4_SYNC_LENGTH_CAPABILITY 0xE6C
#define PCS_RX0_HS_G5_SYNC_LENGTH_CAPABILITY 0x9C4
#define PCS_RX1_HS_G5_SYNC_LENGTH_CAPABILITY 0xDC4
#define PCS_RX0_REG_HS_DATA_VALID_TIMEOUT_VAL_LOW 0x838
#define PCS_RX1_REG_HS_DATA_VALID_TIMEOUT_VAL_LOW 0xC38
#define PCS_RX0_REG_HS_DATA_VALID_TIMEOUT_VAL_HIGH 0x83C
#define PCS_RX1_REG_HS_DATA_VALID_TIMEOUT_VAL_HIGH 0xC3C
#define REG_CAP_WR_EN_VALUE 0x5a
#define PCS_TX0_HIBERN8TIME_CAPABILITY 0x3C
#define PCS_TX1_HIBERN8TIME_CAPABILITY 0x43C
#define PCS_RX0_HIBERN8TIME_CAPABILITY 0xA48
#define PCS_RX1_HIBERN8TIME_CAPABILITY 0xE48
#define HIBERN8TIME_CAPABILITY_VALUE 0x1
#define PCS_RX0_REG_H8_TIMEOUT_VAL 0x874
#define PCS_RX1_REG_H8_TIMEOUT_VAL 0xC74
#define REG_H8_TIMEOUT_VAL_SET 0x2
#define PCS_RX0_MIN_ACTIVATETIME_CAPABILITY 0xA3C
#define PCS_RX1_MIN_ACTIVATETIME_CAPABILITY 0xE3C
#define MIN_ACTIVATETIME_CAPABILITY_VAL 0x2
#define ACTIVATETIME_1_5MS_CAPABILITY_VAL 0xF

#define HSDT0_SCTRL_UFS_SEL_IN_ADDR 0x410
#define HSDT1_SCTRL_UFS_SEL_IN_ADDR 0x110
#define SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(base)              ((base) + 0x45CUL)

static const struct ufs_attr_cfg hufs_mphy_v200_pre_link_attr[] = {
	{ 0x0000156A, 0x00000002 }, /* PA_HSSeries */
	{ 0x00020009, 0x00000001 }, /* Rx SKP_DET_SEL, lane0 */
	{ 0x00030009, 0x00000001 }, /* Rx SKP_DET_SEL, lane1 */
	{ 0x000000DF, 0x00000003 }, /* VCO_AUTO_CHG */
	/* FPGA can only support HS Gear 1, Gear 4 for only test chip */
	{ 0x00000023, 0x00000004 }, /* TX_HSGEAR */
	{ 0x00010023, 0x00000004 }, /* TX_HSGEAR */
	{ 0x000200a3, 0x00000004 }, /* RX_HSGEAR */
	{ 0x000300a3, 0x00000004 }, /* RX_HSGEAR */
	{ 0x000200F1, 0x00000004 }, /* RX_SQ_VREF, lane0 ,RX_SQ_VREF_140mv */
	{ 0x000300F1, 0x00000004 }, /* RX_SQ_VREF, lane1 ,RX_SQ_VREF_140mv */
	{ 0x00020003, 0x0000000A }, /* AD_DIF_P_LS_TIMEOUT_VAL, lane0 */
	{ 0x00030003, 0x0000000A }, /* AD_DIF_P_LS_TIMEOUT_VAL, lane1 */
	{ 0x00020004, 0x00000064 }, /* AD_DIF_N_LS_TIMEOUT_VAL, lane0 */
	{ 0x00030004, 0x00000064 }, /* AD_DIF_N_LS_TIMEOUT_VAL, lane1 */
	{ 0x000200cf, 0x00000002 }, /* RX_STALL */
	{ 0x000300cf, 0x00000002 }, /* RX_STALL */
	{ 0x000200d0, 0x00000002 }, /* RX_SLEEP */
	{ 0x000300d0, 0x00000002 }, /* RX_SLEEP */
	{ 0x000200F0, 0x00000001 }, /* RX enable, lane0 */
	{ 0x000300F0, 0x00000001 }, /* RX enable, lane1 */
	{ 0x000200C7, 0x00000001 }, /* enable override */
	{ 0x000300C7, 0x00000001 }, /* enable override */
	{ 0x0002008d, 0x0000000a }, /* TT */
	{ 0x0003008d, 0x0000000a }, /* TT */
	{ 0x000200C7, 0x00000000 }, /* disable override */
	{ 0x000300C7, 0x00000000 }, /* disable override */
	{ 0x00000059, 0x0000000f }, /* reg 0x59,TX0 */
	{ 0x00010059, 0x0000000f }, /* reg 0x59,TX1 */
	{ 0x0000005A, 0x00000000 }, /* reg 0x5A,TX0 */
	{ 0x0001005A, 0x00000000 }, /* reg 0x5A,TX1 */
	{ 0x0000005B, 0x0000000f }, /* reg 0x5B,TX0 */
	{ 0x0001005B, 0x0000000f }, /* reg 0x5B,TX1 */
	{ 0x0000005C, 0x00000000 }, /* reg 0x5C,TX0 */
	{ 0x0001005C, 0x00000000 }, /* reg 0x5C,TX1 */
	{ 0x0000005D, 0x00000000 }, /* reg 0x5D,TX0 */
	{ 0x0001005D, 0x00000000 }, /* reg 0x5D,TX1 */
	{ 0x0000005E, 0x0000000a }, /* reg 0x5E,TX0 */
	{ 0x0001005E, 0x0000000a }, /* reg 0x5E,TX1 */
	{ 0x0000005F, 0x0000000a }, /* reg 0x5F,TX0 */
	{ 0x0001005F, 0x0000000a }, /* reg 0x5F,TX1 */
	{ 0x0000007A, 0x00000000 }, /* reg 0x7A,TX0 */
	{ 0x0001007A, 0x00000000 }, /* reg 0x7A,TX1 */
	{ 0x0000007B, 0x00000000 }, /* reg 0x7B,TX0 */
	{ 0x0001007B, 0x00000000 }, /* reg 0x7B,TX1 */
	/* Tell UFS that RX can exit H8 only when host TX exits H8 first */
	{ 0x000200C3, 0x00000004 }, /* RX_EXTERNAL_H8_EXIT_EN, lane0 */
	{ 0x000300C3, 0x00000004 }, /* RX_EXTERNAL_H8_EXIT_EN, lane1 */
	{ 0x000200f6, 0x00000001 }, /* RX_DLF Lane 0 */
	{ 0x000300f6, 0x00000001 }, /* RX_DLF Lane 1 */
	{ 0x000000C7, 0x00000003 }, /* measure the power, can close it */
	{ 0x000000C8, 0x00000003 }, /* measure the power, can close it */
	{ 0x000000c5, 0x00000003 }, /* RG_PLL_RXHS_EN */
	{ 0x000000c6, 0x00000003 }, /* RG_PLL_RXLS_EN */

	{ 0x000200E9, 0x00000000 }, /* RX_HS_DATA_VALID_TIMER_VAL0 */
	{ 0x000300E9, 0x00000000 }, /* RX_HS_DATA_VALID_TIMER_VAL0 */
	{ 0x000200EA, 0x00000010 }, /* RX_HS_DATA_VALID_TIMER_VAL1 */
	{ 0x000300EA, 0x00000010 }, /* RX_HS_DATA_VALID_TIMER_VAL1 */

	/* rx_setting_better_perform */
	{ 0x000200f4, 0x00000000 },
	{ 0x000300f4, 0x00000000 },
	{ 0x000200f3, 0x00000000 },
	{ 0x000300f3, 0x00000000 },
	{ 0x000200f2, 0x00000003 },
	{ 0x000300f2, 0x00000003 },
	{ 0x000200f6, 0x00000002 },
	{ 0x000300f6, 0x00000002 },
	{ 0x000200f5, 0x00000000 },
	{ 0x000300f5, 0x00000000 },
	{ 0x000200fc, 0x0000001f },
	{ 0x000300fc, 0x0000001f },
	{ 0x000200fd, 0x00000000 },
	{ 0x000300fd, 0x00000000 },
	{ 0x000200fb, 0x00000005 },
	{ 0x000300fb, 0x00000005 },
	{ 0x00020011, 0x00000011 },
	{ 0x00030011, 0x00000011 },
	/* need to check the value of 0x9529 in the next version of test chip */
	{ 0x00009529, 0x00000006 }, /* VS_DebugSaveConfigTime */
	{ 0x0000D014, 0x00000001 },
	/* update */ /* Hufs UniPro */
};

static const struct ufs_attr_cfg hufs_mphy_v200_post_link_attr[] = {
	{ 0x00003000, 0x00000000 }, /* N_ DeviceID */
	{ 0x00003001, 0x00000001 }, /* N_DeviceID_valid */
	{ 0x00004020, 0x00000000 }, /* T_ConnectionState */
	{ 0x00004022, 0x00000000 }, /* T_PeerCPortID */
	{ 0x00004021, 0x00000001 }, /* T_PeerDeviceID */
	{ 0x00004023, 0x00000000 }, /* T_TrafficClass */
	{ 0x00004020, 0x00000001 }, /* T_ConnectionState */
	{ 0x0002000a, 0x0000001E }, /* RX H8_TIMEOUT_VAL, Lane 0 */
	{ 0x0003000a, 0x0000001E }, /* RX H8_TIMEOUT_VAL, Lane 1 */
	{ 0x000200EB, 0x00000064 }, /* AD_DIF_N_TIMEOUT_VAL */
	{ 0x000300EB, 0x00000064 }, /* AD_DIF_N_TIMEOUT_VAL */
	{ 0x0002000E, 0x000000F0 }, /* RX_H8_EXIT_TIME */
	{ 0x0003000E, 0x000000F0 }, /* RX_H8_EXIT_TIME */
	{ 0x000000da, 0x0000004B }, /* PLL_LOCK_TIME_VAL */
	{ 0x000000dd, 0x000000CB }, /* PLL_RSTB_TIME_VAL */
};

static const struct ufs_attr_cfg hufs_mphy_v120_pre_link_attr[] = {
	{ 0x0000156A, 0x2 }, /* PA_HSSeries */

	{ 0x00000023, 0x00000004 },
	{ 0x00010023, 0x00000004 },
	{ 0x000200a3, 0x00000004 },
	{ 0x000300a3, 0x00000004 },
	{ 0x000200f1, 0x00000004 },
	{ 0x000300f1, 0x00000004 },
	{ 0x00020004, 0x00000064 },
	{ 0x00030004, 0x00000064 },
	{ 0x00020003, 0x0000000A },
	{ 0x00030003, 0x0000000A },
	{ 0x000200f0, 0x00000001 },
	{ 0x000300f0, 0x00000001 },

	{ 0x00020009, 0x00000001 }, /* Rx SKP_DET_SEL, lane0 */
	{ 0x00030009, 0x00000001 }, /* Rx SKP_DET_SEL, lane1 */
	{ 0x000000DF, 0x00000003 }, /* VCO_AUTO_CHG */
	{ 0x000000C7, 0x3 }, /* measure the power, can close it */
	{ 0x000000C8, 0x3 }, /* measure the power, can close it */
	{ 0x000200cf, 0x00000002 }, /* RX_STALL */
	{ 0x000300cf, 0x00000002 }, /* RX_STALL */
	{ 0x000200d0, 0x00000002 }, /* RX_SLEEP */
	{ 0x000300d0, 0x00000002 }, /* RX_SLEEP */
	{ 0x000200cc, 0x00000003 }, /* RX_HS_CLK_EN */
	{ 0x000300cc, 0x00000003 }, /* RX_HS_CLK_EN */
	{ 0x000200cd, 0x00000003 }, /* RX_LS_CLK_EN */
	{ 0x000300cd, 0x00000003 }, /* RX_LS_CLK_EN */
	{ 0x000000c5, 0x00000003 }, /* RG_PLL_RXHS_EN */
	{ 0x000000c6, 0x00000003 }, /* RG_PLL_RXLS_EN */
	{ 0x000200E9, 0x00000000 }, /* RX_HS_DATA_VALID_TIMER_VAL0 */
	{ 0x000300E9, 0x00000000 }, /* RX_HS_DATA_VALID_TIMER_VAL0 */
	{ 0x000200EA, 0x00000010 }, /* RX_HS_DATA_VALID_TIMER_VAL1 */
	{ 0x000300EA, 0x00000010 }, /* RX_HS_DATA_VALID_TIMER_VAL1 */
	{ 0x00001552, 0x0000004F }, /* PA_TxHsG1SyncLength */
	{ 0x00001554, 0x0000004F }, /* PA_TxHsG2SyncLength */
	{ 0x00001556, 0x0000004F }, /* PA_TxHsG3SyncLength */
	{ 0x00009509, 0x0000004F },
	{ 0x0000950A, 0x0000000F },
	{ 0x000200F4, 0x1 }, /* RX_EQ_SEL_R */
	{ 0x000300F4, 0x1 }, /* RX_EQ_SEL_R */
	{ 0x000200F2, 0x3 }, /* RX_EQ_SEL_C */
	{ 0x000300F2, 0x3 }, /* RX_EQ_SEL_C */
	{ 0x000200FB, 0x3 }, /* RX_VSEL */
	{ 0x000300FB, 0x3 }, /* RX_VSEL */
	{ 0x000200F6, 0x3 }, /* RX_DLF Lane 0 */
	{ 0x000300F6, 0x3 }, /* RX_DLF Lane 1 */
	{ 0x0002000a, 0x00000002 }, /* RX H8_TIMEOUT_VAL, Lane 0 */
	{ 0x0003000a, 0x00000002 }, /* RX H8_TIMEOUT_VAL, Lane 1 */
	{ 0x000000d4, 0x00000031 }, /* RG_PLL_DMY0 */
	{ 0x00000073, 0x00000004 }, /* TX_PHY_CONFIG II */
	{ 0x00010073, 0x00000004 }, /* TX_PHY_CONFIG II */
	{ 0x000200F0, 0x00000001 }, /* RX enable, lane0 */
	{ 0x000300F0, 0x00000001 }, /* RX enable, lane1 */
	{ 0x0000D014, 0x1 }, /* Unipro VS_MphyCfgUpdt */
};

static const struct ufs_attr_cfg hufs_mphy_v120_post_link_attr[] = {
	{ 0x00003000, 0x0 }, /* N_ DeviceID */
	{ 0x00003001, 0x1 }, /* N_DeviceID_valid */
	{ 0x00004020, 0x0 }, /* T_ConnectionState */
	{ 0x00004022, 0x0 }, /* T_PeerCPortID */
	{ 0x00004021, 0x1 }, /* T_PeerDeviceID */
	{ 0x00004023, 0x0 }, /* T_TrafficClass */
	{ 0x00004020, 0x1 }, /* T_ConnectionState */
	{ 0x0002000a, 0x0000001E }, /* RX H8_TIMEOUT_VAL, Lane 0 */
	{ 0x0003000a, 0x0000001E }, /* RX H8_TIMEOUT_VAL, Lane 1 */
	{ 0x000200EB, 0x64 }, /* AD_DIF_N_TIMEOUT_VAL */
	{ 0x000300EB, 0x64 }, /* AD_DIF_N_TIMEOUT_VAL */
	{ 0x0002000E, 0xF0 }, /* RX_H8_EXIT_TIME */
	{ 0x0003000E, 0xF0 }, /* RX_H8_EXIT_TIME */
	{ 0x000000ca, 0x3 },
	{ 0x000015a8, 10 }, /* PA_TActivate */
};

/* ASIC */
static const struct ufs_attr_cfg hufs_mphy_post_link_attr[] = {
	{ 0x40210000, 0x00000001 }, /* T_PeerDeviceID */
	{ 0x30000000, 0x00000000 }, /* N_DeviceID */
	{ 0x30010000, 0x00000001 }, /* N_DeviceID_valid */
	{ 0x40220000, 0x00000000 }, /* T_PeerCPortID */
	{ 0x40230000, 0x00000000 }, /* T_TrafficClass */
	{ 0x40200000, 0x00000001 }, /* T_ConnectionState */
};

static const struct ufs_attr_cfg hisi_fw_config_attr[] = {
	{ 0x000080C8, 0x00000000 },
	{ 0x000080CC, 0x0000001D },
	{ 0x000080D0, 0x10000000 },
	{ 0x000080D4, 0x01C00000 },
	{ 0x000080D8, 0x05400000 },
	{ 0x000080DC, 0x00000001 },
	{ 0x000080E0, 0x00000003 },
	{ 0x000080E4, 0x0010B800 },
	{ 0x000080E8, 0x00000000 },
	{ 0x000080EC, 0x00000000 },
};


void hufs_regulator_init(struct ufs_hba *hba)
{
}

static uint32_t ufs0_get_ufs_version(struct hufs_host *host)
{
	u32 reg = 0;
	reg = readl(host->sysctrl + SCTRL_SCEFUSEDATA2_OFFSET);
	if ((reg & BIT(21)) == 0) {
		if ((readl(host->hsdt_sysctrl + HSDT0_SCTRL_UFS_SEL_IN_ADDR) & BIT(0)) == 0)
			return 0;
		else
			return 1;
	}

	if ((reg & BIT(22)) == 0)
		return 0;
	else
		return 1;
}

static uint32_t ufs1_get_ufs_version(struct hufs_host *host)
{
	u32 reg = 0;
	reg = readl(host->sysctrl + SCTRL_SCEFUSEDATA3_OFFSET);
	if ((reg & BIT(27)) == 0) {
		if ((readl(host->hsdt1_sysctrl + HSDT1_SCTRL_UFS_SEL_IN_ADDR) & BIT(0)) == 0)
			return 0;
		else
			return 1;
	}

	if ((reg & BIT(28)) == 0)
		return 0;
	else
		return 1;
}

void hufs_pwr_change_post_change(struct ufs_hba *hba)
{
	struct hufs_host *host = NULL;

	host = (struct hufs_host *)hba->priv;
	/* sctrl scbakdate1 bit 2, ufs1 worked success in kernel */
	if (host->host_id == 1) {
		writel(0x40004, host->sysctrl + SCTRL_SCBAKDATA1_MASK_OFFSET);
		dev_err(hba->dev, "ufs post change reseved reg: 0x%x\n", readl(host->sysctrl + SCTRL_SCBAKDATA1_MASK_OFFSET));
	}
}

void hufs_set_buck_voltage(struct ufs_hba *hba)
{
}

void hufs_mphy_config_update(struct ufs_hba *hba)
{
}

#define PLL0_FREQ 239000000
#define PLL0_4_0_FREQ 418000000
#define PLL1_FREQ 480000000
#define PLL2_FREQ 964000000

void ufs_clk_init(struct ufs_hba *hba)
{
	uint32_t value = 0;
	int retry = POLLING_RETRY;
	int ret;
	struct hufs_host *host = NULL;

	host = (struct hufs_host *)hba->priv;
	dev_info(hba->dev, "%s ++ host_id: %d\n", __func__, host->host_id);

	if (host->host_id == 1) {
		/* 0. reset device first */
		writel(0x00020000, SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(host->actrl));
		/* 1. close mphy clk enable */
		writel(MPHY_SYS_CLK_EN, host->hsdt1_crg + HSDT1_CRG_PERDIS1_OFFSET);
		/* 2. enable MPHY ISO */
		if (!((host->caps & USE_HUFS_MPHY_TC) || hba->host->is_emulator))
			writel(0xC000C000, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);
		/* 3. reset ufs subsys */
		writel(IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1,
				host->hsdt1_crg + HSDT1_CRG_PERRSTEN0_OFFSET);
		/* fpga delay 200us */
		if (host->caps & USE_HUFS_MPHY_TC)
			udelay(200);
		else
			udelay(20);
		/* 4. config ufs1 retention dual -rail ram exit deep sleep */
		ret = atfd_ufs_smc(FID_ACTRL_HSDT1_MEM_CONFIG, 0, 0, 0);
		if (ret)
			pr_err("%s: config ufs1 retention dual -rail ram exit deep sleep fail ret = 0x%llx\n", __func__, ret);

		/* 5. disreset ufs1 ip */
		writel(IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1 | IP_RST_QIC_UFS_HCI,
				host->hsdt1_crg + HSDT1_CRG_PERRSTDIS0_OFFSET);

		/* 6. check ufs1 reset state */
		while (retry--) {
			value = readl(host->hsdt1_crg + HSDT1_CRG_PERRSTSTAT0_OFFSET);
			if (!(value & (IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1 | IP_RST_QIC_UFS_HCI)))
				break;
			udelay(1);
		}
		if (retry < 0)
			dev_err(hba->dev, "UFS1 Sub-sys CRG disreset failed 0x%x\n", value);

		/* 4.0 ufs device */
		if (ufs1_get_ufs_version(host)) {
			/* 4. clk ufs subsys 417M */
			writel(DIV_UFS_SYS_SUBSYS_HSDT1_1FREQ, host->hsdt1_crg + HSDT1_CRG_CLKDIV1_OFFSET);
			host->device_4_0 = true;
			dev_err(hba->dev, "UFS1 4.0 clk set\n");
		} else {
			/* 4. clk ufs subsys 208.9M */
			writel(UFS1_SUBSYS_CLK_DIV, host->hsdt1_crg + HSDT1_CRG_CLKDIV1_OFFSET);
			host->device_4_0 = false;
			dev_err(hba->dev, "UFS1 3.1 clk set\n");
		}

		/* 5. clk ufs mphy 417M */
		writel(UFS1_MPHY_CLK_DIV, host->pericrg + CRGPERIPH_CLKDIV1_OFFSET);

		/* 6. open qic ufs hic, ufs subsys, mphy clk */
		writel(GT_CLK_UFS_MPHY_SYS_HSDT1 | GT_CLK_UFS_SYS_SUBSYS_HSDT1 | GT_CLK_QIC_UFS_HCI_HSDT1,
				host->hsdt1_crg + HSDT1_CRG_PEREN1_OFFSET);
	} else {
		/* 0. reset device first */
		writel(0x00010000, SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(host->actrl));
		/* 1. close mphy clk enable */
		writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPERDIS6);
		/* 2. enable MPHY ISO */
		if (!((host->caps & USE_HUFS_MPHY_TC) || hba->host->is_emulator))
			writel(0x10201020, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);

		/* 3. reset disreset dfa subsys */
		writel(IP_RST_UFS_DFA_SYS, host->pericrg + PERRSTEN0_OFFSET);

		/* 4. fpga delay 200us */
		if (host->caps & USE_HUFS_MPHY_TC)
			udelay(200);
		else
			udelay(20);
		/* 5. disreset and dfa por */
		writel(IP_RST_DFA_POR, host->hsdt_crg + SOC_HSDT_CRG_PERRSTDIS0);

		/* 6. disreset ufs dfa sys */
		writel(IP_RST_UFS_DFA_SYS, host->pericrg + PERRSTDIS0_OFFSET);

		/* 4.0 ufs device */
		if (ufs0_get_ufs_version(host)) {
			/* select pll0, dfa sys clk 417M */
			ret = clk_set_rate(host->clk_ufs_dfa_sys, PLL0_4_0_FREQ);
			if (ret)
				dev_err(hba->dev, "clk_ufs_dfa_sys set clk failed\n");
			ret = clk_prepare_enable(host->clk_ufs_dfa_sys);
			if (ret)
				dev_err(hba->dev, "clk_ufs_dfa_sys clk_prepare_enable failed\n");
			/* mphy sys clk 417M */
			writel(MPHY_SYS_CLK_DIV, host->ao_crg + AO_CRG_SCCLKDIV5);
			/* mphy sys clk enable */
			writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPEREN6);
			host->device_4_0 = true;
			dev_err(hba->dev, "UFS 4.0 clk set\n");
		} else {
			ret = clk_set_rate(host->clk_ufs_dfa_sys, PLL0_FREQ);
			if (ret)
				dev_err(hba->dev, "clk_ufs_dfa_sys set clk failed\n");
			ret = clk_prepare_enable(host->clk_ufs_dfa_sys);
			if (ret)
				dev_err(hba->dev, "clk_ufs_dfa_sys clk_prepare_enable failed\n");
			/* mphy sys clk 417M */
			writel(MPHY_SYS_CLK_DIV, host->ao_crg + AO_CRG_SCCLKDIV5);
			/* mphy sys clk enable */
			writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPEREN6);
			dev_err(hba->dev, "UFS 3.1 clk set\n");
		}

		/* 4. enable ip_rst_ufs_subsys, reset ufs sub-system */
		writel(IP_RST_UFS_CRG, host->ao_crg + AO_CRG_SCPERRSTDIS1);
		retry = POLLING_RETRY;
		while (retry--) {
			value = readl(host->ao_crg + AO_CRG_SCPERRSTSTAT1);
			if (!(value & IP_RST_UFS_CRG))
				break;
			udelay(1);
		}
		if (retry < 0)
			dev_err(hba->dev, "UFS Sub-sys CRG reset failed 0x%x\n", value);

		/* 9. ufs0 need open clkin_sedes buf share with modem/pcie */
		ret = clk_prepare_enable(host->clk_ufs0phy_ref);
		if (ret)
			dev_err(hba->dev, "clk_ufs0phy_ref clk_prepare_enable failed\n");
	}

	dev_info(hba->dev, "%s device version: %d--\n", __func__, host->device_4_0);
}

void ufs_soc_init(struct ufs_hba *hba)
{
	struct hufs_host *host = NULL;

	host = (struct hufs_host *)hba->priv;

	dev_info(hba->dev, "%s ++ host_id: %d\n", __func__, host->host_id);
	/* device reset */
	hufs_device_hw_reset(hba);

	/* 1. release internal resets */
	writel(0xFFFF0683, host->ufs_sys_ctrl + UFS_SYSCTRL_RESET_CTRL_OFFSET);
	/* 5. mphy iso en bit 5, mphy lvr iso en bit12 */
	if (host->host_id == 1)
		writel(0xC0000000, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);
	else
		/* 5. mphy iso en bit 5, mphy lvr iso en bit12 */
		writel(0x10200000, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);

	/* 2. wait 1 ms */
	mdelay(1);

	/* open ufs address io consisten access */
	if (host->caps & HUFS_CONSISTENT_ACCESS)
		writel(0x0, host->ufs_sys_ctrl + UFS_SYSCTRL_UFS_AXI_CTRL_OFFSET);

	dev_info(hba->dev, "%s --\n", __func__);
}

void unipro_clk_auto_gate_enable(struct ufs_hba *hba)
{
	ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(DME_CTRL1, 0x0), UNIPRO_CLK_AUTO_GATE_CONFIG);
}

void unipro_clk_auto_gate_disable(struct ufs_hba *hba)
{
	ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(DME_CTRL1, 0x0), UNIPRO_CLK_AUTO_GATE_DISABLE);
}

#define H8_CLK_GT_EN (0x1 << 1)
void unipro_h8_clk_gate_enable(struct ufs_hba *hba)
{
	u32 reg = 0;

	ufshcd_dme_get(hba, UIC_ARG_MIB(DME_CTRL0), &reg);
	reg |= H8_CLK_GT_EN;
	ufshcd_dme_set(hba, UIC_ARG_MIB(DME_CTRL0), reg);
}

void unipro_h8_clk_gate_disable(struct ufs_hba *hba)
{
	u32 reg = 0;

	ufshcd_dme_get(hba, UIC_ARG_MIB(DME_CTRL0), &reg);
	reg &= ~H8_CLK_GT_EN;
	ufshcd_dme_set(hba, UIC_ARG_MIB(DME_CTRL0), reg);
}

int hufs_suspend_before_set_link_state(
	struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	unipro_clk_auto_gate_disable(hba);
	unipro_h8_clk_gate_disable(hba);
	return 0;
}

int hufs_resume_after_set_link_state(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	unipro_h8_clk_gate_enable(hba);
	unipro_clk_auto_gate_enable(hba);

	return 0;
}

int hufs1_suspend(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	int ret;
	u32 value;
	int retry = POLLING_RETRY;
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (ufshcd_is_runtime_pm(pm_op))
		return 0;

	if (host->in_suspend)
		return 0;
	udelay(10); /* wait 10 us */

	/* Step 6~7. check if the store is complete */
	ret = ufs_access_register_and_check(hba, UFS_CFG_SUSPEND);
	if (ret)
		return ret;

	hufs_disable_all_irq(hba);
	/* Step 8. clear bit0 of ufs_cfg_ram_ctrl */
	value = ufshcd_readl(hba, UFS_CFG_RAM_CTRL);
	value &= (~0x01);
	ufshcd_writel(hba, value, UFS_CFG_RAM_CTRL);

	/* Step 9. hsdt1 hci bus enter low power */
	writel(0x40004, host->hsdt1_ctrl + HSDT1_QIC_PDC_REQ_CTRL);

	/* Step 10. wait hsdt1 QIC_PDC_ACK */
	retry = POLLING_RETRY;
	while (retry--) {
		value = readl(host->hsdt1_ctrl + HSDT1_QIC_PDC_ACK_CTRL);
		if (value & PDC_UFS_HCI_SYSPDC_PWRD_ACK)
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS1 entry pdc req timeout 0x%x\n", value);

	/* Step11. close qic ufs hic, ufs subsys, mphy clk */
	writel(GT_CLK_UFS_MPHY_SYS_HSDT1 | GT_CLK_UFS_SYS_SUBSYS_HSDT1 | GT_CLK_QIC_UFS_HCI_HSDT1,
				host->hsdt1_crg + HSDT1_CRG_PERDIS1_OFFSET);

	/* Step 12. enable MPHY ISO */
	writel(0xC000C000, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);

	/* Step 13. reset  dfa por */
	writel(IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1 | IP_RST_QIC_UFS_HCI,
				host->hsdt1_crg + HSDT1_CRG_PERRSTEN0_OFFSET);

	/* Step 14. check ufs1 reset state */
	retry = POLLING_RETRY;
	while (retry--) {
		value = readl(host->hsdt1_crg + HSDT1_CRG_PERRSTSTAT0_OFFSET);
		if ((value & (IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1 | IP_RST_QIC_UFS_HCI)) == 0xE000000)
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS1 Sub-sys CRG reset timeout 0x%x\n", value);

	/* Step 15. enable RET of dual-rail RAM, set DS =1 , SD = 0 */
	ret = atfd_ufs_smc(FID_ACTRL_HSDT1_MEM_CONFIG, 1, 0, 0);
		if (ret)
			pr_err("%s: enable RET of dual-rail RAM, set DS =1 , SD = 0 fail ret = 0x%llx\n", __func__, ret);

	pmic_write_reg(PMIC_CLK_UFS_A_EN_ADDR(0), 0);

	host->in_suspend = true;

	return 0;
}

int hufs_suspend(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	int ret;
	u32 value;
	int retry = POLLING_RETRY;
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (host->host_id == 1)
		return hufs1_suspend(hba, pm_op);

	if (ufshcd_is_runtime_pm(pm_op))
		return 0;

	if (host->in_suspend)
		return 0;
	udelay(10); /* wait 10 us */

	/* Step 6~7. check if the store is complete */
	ret = ufs_access_register_and_check(hba, UFS_CFG_SUSPEND);
	if (ret)
		return ret;

	hufs_disable_all_irq(hba);
	/* Step 8. clear bit0 of ufs_cfg_ram_ctrl */
	value = ufshcd_readl(hba, UFS_CFG_RAM_CTRL);
	value &= (~0x01);
	ufshcd_writel(hba, value, UFS_CFG_RAM_CTRL);

	clk_disable_unprepare(host->clk_ufs0phy_ref);

	/* step 9. close dfa clk */
	clk_disable_unprepare(host->clk_ufs_dfa_sys);
	/* close sys clk enable */
	writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPERDIS6);

	/* Step 10. enable MPHY ISO */
	writel(0x10201020, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);

	/* Step 11. hsdt0 pd pull up */
	writel(0x40004, host->hsdt_ctrl + HSDT0_QIC_SYSPDC_PWRD_REQ);

	/* Step 12. check hsdt0 pd pull up state */
	while (retry--) {
		value = readl(host->hsdt_ctrl + HSDT0_QIC_SYSPDC_PWRD_ACK);
		if (value & PDC_DFA_SYSPDC_PWRD_ACK)
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS enter pdc req timeout 0x%x\n", value);

	/* step 13. reset  dfa por */
	writel(IP_RST_DFA_POR, host->hsdt_crg + SOC_HSDT_CRG_PERRSTEN0);
	/* reset dfa subsys */
	writel(IP_RST_UFS_DFA_SYS, host->pericrg + PERRSTEN0_OFFSET);

	/* Step 14. enable ip_rst_ufs_subsys, reset ufs sub-system */
	writel(IP_RST_UFS_CRG, host->ao_crg + AO_CRG_SCPERRSTEN1);

	/* Step 15. enable RET of dual-rail RAM, set DS =1 , SD = 0 */
	ret = atfd_ufs_smc(FID_ACTRL_AO_MEM_CONFIG, 1, 0, 0);
	if (ret)
		pr_err("%s: enable RET of dual-rail RAM, set DS =1 , SD = 0 fail ret = 0x%llx\n", __func__, ret);

	pmic_write_reg(PMIC_CLK_UFS_A_EN_ADDR(0), 0);

	host->in_suspend = true;

	return 0;
}

static void ufs_sub_sys_crg_reset_release(struct ufs_hba *hba)
{
	u32 value;
	int retry = POLLING_RETRY;
	struct hufs_host *host = hba->priv;

	writel(IP_RST_UFS_CRG, host->ao_crg + AO_CRG_SCPERRSTDIS1);
	retry = POLLING_RETRY;
	while (retry--) {
		value = readl(host->ao_crg + AO_CRG_SCPERRSTSTAT1);
		if (!(value & IP_RST_UFS_CRG))
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS Sub-sys CRG reset failed 0x%x\n", value);
}

int hufs1_resume(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	int ret;
	u32 value;
	int retry = POLLING_RETRY;
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (!host->in_suspend)
		return 0;

	/* Step 1. ufs ref clk enable */
	pmic_write_reg(PMIC_CLK_UFS_A_EN_ADDR(0), 1);
	/* Step 2. 250us to ensure the clk stable */
	udelay(PMIC_CLK_EN_WAIT_TIME);

	/* Step 3. disable RET of dual-rail RAM, DS = 0, SD = 0 */
	ret = atfd_ufs_smc(FID_ACTRL_HSDT1_MEM_CONFIG, 0, 0, 0);
		if (ret)
			pr_err("%s: disable RET of dual-rail RAM, DS = 0, SD = 0 fail ret = 0x%llx\n", __func__, ret);
	/* 4. disreset ufs1 ip */
	writel(IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1 | IP_RST_QIC_UFS_HCI,
			host->hsdt1_crg + HSDT1_CRG_PERRSTDIS0_OFFSET);

	/* 5. check ufs1 reset state */
	while (retry--) {
		value = readl(host->hsdt1_crg + HSDT1_CRG_PERRSTSTAT0_OFFSET);
		if (!(value & (IP_RST_UFS_CRG_HSDT1 | IP_RST_UFS_SYS_SUBSYS_HSDT1 | IP_RST_QIC_UFS_HCI)))
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS1 Sub-sys CRG disreset failed 0x%x\n", value);

	if (ufs1_get_ufs_version(host)) {
		/* 6. clk ufs subsys 417M */
		writel(DIV_UFS_SYS_SUBSYS_HSDT1_1FREQ, host->hsdt1_crg + HSDT1_CRG_CLKDIV1_OFFSET);
	} else {
		/* 6. clk ufs subsys 208.9M */
		writel(UFS1_SUBSYS_CLK_DIV, host->hsdt1_crg + HSDT1_CRG_CLKDIV1_OFFSET);
	}

	/* 7. clk ufs mphy 417M */
	writel(UFS1_MPHY_CLK_DIV, host->pericrg + CRGPERIPH_CLKDIV1_OFFSET);

	/* 8. open qic ufs hic, ufs subsys, mphy clk */
	writel(GT_CLK_UFS_MPHY_SYS_HSDT1 | GT_CLK_UFS_SYS_SUBSYS_HSDT1 | GT_CLK_QIC_UFS_HCI_HSDT1,
			host->hsdt1_crg + HSDT1_CRG_PEREN1_OFFSET);
	udelay(1);

	/* 10~11. ufs1 busy exit sleep */
	writel(PDC_UFS_HCI_SYSPDC_PWRD_REQ, host->hsdt1_ctrl + HSDT1_QIC_PDC_REQ_CTRL);
	retry = POLLING_RETRY;
	while (retry--) {
		value = readl(host->hsdt1_ctrl + HSDT1_QIC_PDC_ACK_CTRL);
		if (!(value & PDC_UFS_HCI_SYSPDC_PWRD_ACK))
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS1 exit pdc req failure 0x%x\n", value);

	/* Step 12. release internal release of UFS Subsys */
	writel(0xFFFF0683, host->ufs_sys_ctrl + UFS_SYSCTRL_RESET_CTRL_OFFSET);

	/* Step 13. disable MPHY ISO */
	writel(0xC0000000, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);

	/* Step 14. wait 500us */
	udelay(500);

	hufs_enable_all_irq(hba);

	/* Step 1. Hibernate Exit Mode : power down mode */
	hufs_uic_write_reg(hba, 0xD000, 0x1);
	/* Step 2. wait 10us */
	udelay(10);
	/*
	 * Step 3. and Step 8.
	 * request UFSHCI and MPHY to restore register from dual-rail RAM and
	 * check if the restore is complete
	 */
	ret = ufs_access_register_and_check(hba, UFS_CFG_RESUME);
	if (ret)
		return ret;

	/* Step 9. */
	value = ufshcd_readl(hba, UFS_CFG_RAM_CTRL);
	value &= (~UFS_CFG_RESUME);
	ufshcd_writel(hba, value, UFS_CFG_RAM_CTRL);

	/* Step 10. disable all unipro irq, use uic irq */
	ufshcd_hufs_disable_unipro_intr(hba, 0x1FFFFFF);

#ifdef CONFIG_UFS_4_0_HC
	ufshcd_init_mcq_hwq_slot(hba);
#endif

	host->in_suspend = false;

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_KM
	unsigned int mspc_flag = 0;
	/* inline restore SK */
	ret = fbe_ca_msp_available(&mspc_flag);
	if (ret)
		BUG();

	if (mspc_flag == FBE_CA_MSP_AVAILABLE) {
		ret = fbex_gen_sk(FILE_ENCRY_RESTORE_SK_TYPE);
		dev_err(hba->dev, "%s: km sk restore end\n", __func__);
		if (ret)
			BUG();
	}
#endif

	return 0;
}

int hufs_resume(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	int ret;
	u32 value;
	int retry = POLLING_RETRY;
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (host->host_id == 1)
		return hufs1_resume(hba, pm_op);

	if (!host->in_suspend)
		return 0;

	/* Step 0. hsdt0 pd pull down */
	writel(0x40000, host->hsdt_ctrl + HSDT0_QIC_SYSPDC_PWRD_REQ);

	/* Step 0.1. check hsdt0 pd pull up state */
	while (retry--) {
		value = readl(host->hsdt_ctrl + HSDT0_QIC_SYSPDC_PWRD_ACK);
		if (!(value & PDC_DFA_SYSPDC_PWRD_ACK))
			break;
		udelay(1);
	}
	if (retry < 0)
		dev_err(hba->dev, "UFS exit pdc req timeout 0x%x\n", value);

	ret = clk_prepare_enable(host->clk_ufs0phy_ref);
	if (ret)
		dev_err(hba->dev, "clk_ufs0phy_ref clk_prepare_enable failed\n");

	/* Step 1. ufs ref clk enable */
	pmic_write_reg(PMIC_CLK_UFS_A_EN_ADDR(0), 1);
	/* Step 2. 250us to ensure the clk stable */
	udelay(PMIC_CLK_EN_WAIT_TIME);

	/* Step 3. disable RET of dual-rail RAM, DS = 0, SD = 0 */
	ret = atfd_ufs_smc(FID_ACTRL_AO_MEM_CONFIG, 0, 0, 0);
	if (ret)
		pr_err("%s: disable RET of dual-rail RAM, DS = 0, SD = 0 fail ret = 0x%llx\n", __func__, ret);

	/* Step4-5 4.0 ufs device */
	if (ufs0_get_ufs_version(host)) {
		/* step8-1. ufs dfa  sys clk 417M enable */
		ret = clk_set_rate(host->clk_ufs_dfa_sys, PLL0_4_0_FREQ);
		if (ret)
			dev_err(hba->dev, "clk_ufs_dfa_sys set clk failed\n");
		/* mphy sys clk 417M */
		writel(MPHY_SYS_CLK_DIV, host->ao_crg + AO_CRG_SCCLKDIV5);
	} else {
		/* step8-1. ufs dfa  sys clk enable */
		ret = clk_set_rate(host->clk_ufs_dfa_sys, PLL0_FREQ);
		if (ret)
			dev_err(hba->dev, "clk_ufs_dfa_sys set clk failed\n");
		/* mphy sys clk 417M */
		writel(MPHY_SYS_CLK_DIV, host->ao_crg + AO_CRG_SCCLKDIV5);
	}
	udelay(1);

	/* Step6. enable ip_rst_ufs_subsys, reset ufs sub-system */
	ufs_sub_sys_crg_reset_release(hba);

	/* Step7 . disreset and dfa por */
	writel(IP_RST_DFA_POR, host->hsdt_crg + SOC_HSDT_CRG_PERRSTDIS0);
	/* disreset dfa subsys */
	writel(IP_RST_UFS_DFA_SYS, host->pericrg + PERRSTDIS0_OFFSET);

	ret = clk_prepare_enable(host->clk_ufs_dfa_sys);
	if (ret)
		dev_err(hba->dev, "clk_ufs_dfa_sys clk_prepare_enable failed\n");
	/* Setp8-2 open dfa  mphy clk */
	writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPEREN6);

	udelay(200);
	/* Step 9. release internal release of UFS Subsys */
	writel(0xFFFF0683, host->ufs_sys_ctrl + UFS_SYSCTRL_RESET_CTRL_OFFSET);

	/* Step 10. disable MPHY ISO */
	writel(0x10200000, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);

	/* Step 11. wait 500us */
	udelay(500);

	hufs_enable_all_irq(hba);

	/* Step 1. Hibernate Exit Mode : power down mode */
	hufs_uic_write_reg(hba, 0xD000, 0x1);
	/* Step 2. wait 10us */
	udelay(10);
	/*
	 * Step 3. and Step 8.
	 * request UFSHCI and MPHY to restore register from dual-rail RAM and
	 * check if the restore is complete
	 */
	ret = ufs_access_register_and_check(hba, UFS_CFG_RESUME);
	if (ret)
		return ret;

	/* Step 9. */
	value = ufshcd_readl(hba, UFS_CFG_RAM_CTRL);
	value &= (~UFS_CFG_RESUME);
	ufshcd_writel(hba, value, UFS_CFG_RAM_CTRL);

	/* Step 10. disable all unipro irq, use uic irq */
	ufshcd_hufs_disable_unipro_intr(hba, 0x1FFFFFF);

#ifdef CONFIG_UFS_4_0_HC
	ufshcd_init_mcq_hwq_slot(hba);
#endif

	host->in_suspend = false;

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_KM
	unsigned int mspc_flag = 0;
	/* inline restore SK */
	ret = fbe_ca_msp_available(&mspc_flag);
	if (ret)
		BUG();

	if (mspc_flag == FBE_CA_MSP_AVAILABLE) {
		ret = fbex_gen_sk(FILE_ENCRY_RESTORE_SK_TYPE);
		dev_err(hba->dev, "%s: km sk restore end\n", __func__);
		if (ret)
			BUG();
	}
#endif

	return 0;
}

void hufs_device_hw_reset(struct ufs_hba *hba)
{
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (likely(!(host->caps & USE_HUFS_MPHY_TC))) {
		/*
		 * Enable device reset, bit[0] is rst_device_rst_n
		 * bit[16] is bitmask
		 */
		if (host->host_id == 1) {
			writel(0x00020000, SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(host->actrl));
			/* wait 2 ms */
			mdelay(2);
			/* Disable device reset */
			writel(0x00020002, SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(host->actrl));
		} else {
			writel(0x00010000, SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(host->actrl));
			/* wait 2 ms */
			mdelay(2);
			/* Disable device reset */
			writel(0x00010001, SOC_ACTRL_BITMSK_NONSEC_CTRL7_ADDR(host->actrl));
		}
	} else {
		if (!is_v200_mphy(hba)) {
			ufs_i2c_writel(hba, (unsigned int)UFS_BIT_DEVICE_RSTDIS,
				       SC_RSTDIS);
			mdelay(1);
			ufs_i2c_writel(hba, (unsigned int)UFS_BIT_DEVICE_RSTEN,
				       SC_RSTEN);
		} else {
			ufs_i2c_writel(hba, 0x20000, SC_RSTEN_V200);
			mdelay(2); /* wait 2 ms */
			ufs_i2c_writel(hba, 0x20000, SC_RSTDIS_V200);
		}
	}

	mdelay(10); /* wait 10 ms */
	dev_err(hba->dev, "device reset done hostid: %d\n", host->host_id);
}

void mphy_set_pcs_init(struct hufs_host *host)
{
	mphy_pcs_writel(host, MPHY_TX_EN_BIT, PCS_LAN0_MPHY_TX_EN);
	mphy_pcs_writel(host, MPHY_TX_EN_BIT, PCS_LAN1_MPHY_TX_EN);
	mphy_pcs_writel(host, MPHY_RX_EN_BIT, PCS_LAN0_MPHY_RX_EN);
	mphy_pcs_writel(host, MPHY_RX_EN_BIT, PCS_LAN1_MPHY_RX_EN);
}

void mphy_set_pcs_time_escape_trunk(struct hufs_host *host)
{
	mphy_pcs_writel(host, SYS_CLK_SW_MODE_SOC, PCS_SYS_CLK_SW_MODE);
	mphy_pcs_writel(host, SC_UFS_CLK_DIV_2, PCS_SC_UFS_CLK_DIV_MODE);
	/* rx sync length */
	mphy_pcs_writel(host, 0x5a, PCS_RX0_CAP_WP);
	mphy_pcs_writel(host, 0x5a, PCS_RX1_CAP_WP);
	mphy_pcs_writel(host, 0x4b, PCS_RX0_HS_G1_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4b, PCS_RX1_HS_G1_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4b, PCS_RX0_HS_G2_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4b, PCS_RX1_HS_G2_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4b, PCS_RX0_HS_G3_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4b, PCS_RX1_HS_G3_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4c, PCS_RX0_HS_G4_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4c, PCS_RX1_HS_G4_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4c, PCS_RX0_HS_G5_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0x4c, PCS_RX1_HS_G5_SYNC_LENGTH_CAPABILITY);
	mphy_pcs_writel(host, 0xb8, PCS_RX0_REG_HS_DATA_VALID_TIMEOUT_VAL_LOW);
	mphy_pcs_writel(host, 0xb8, PCS_RX1_REG_HS_DATA_VALID_TIMEOUT_VAL_LOW);
	mphy_pcs_writel(host, 0x1, PCS_RX0_REG_HS_DATA_VALID_TIMEOUT_VAL_HIGH);
	mphy_pcs_writel(host, 0x1, PCS_RX1_REG_HS_DATA_VALID_TIMEOUT_VAL_HIGH);
}

static void mphy_reg_cap_enable(struct hufs_host *host, bool enable)
{
	u32 val = 0;

	if (enable)
		val = REG_CAP_WR_EN_VALUE;
	mphy_pcs_writel(host, val, PCS_TX0_CAP_WP);
	mphy_pcs_writel(host, val, PCS_TX1_CAP_WP);
	mphy_pcs_writel(host, val, PCS_RX0_CAP_WP);
	mphy_pcs_writel(host, val, PCS_RX1_CAP_WP);
}

static void mphy_hibern8_config(struct hufs_host *host)
{
	u32 val = MIN_ACTIVATETIME_CAPABILITY_VAL;
	mphy_reg_cap_enable(host, true);

	mphy_pcs_writel(host, HIBERN8TIME_CAPABILITY_VALUE, PCS_RX0_HIBERN8TIME_CAPABILITY);
	mphy_pcs_writel(host, HIBERN8TIME_CAPABILITY_VALUE, PCS_RX1_HIBERN8TIME_CAPABILITY);
	mphy_pcs_writel(host, HIBERN8TIME_CAPABILITY_VALUE, PCS_TX0_HIBERN8TIME_CAPABILITY);
	mphy_pcs_writel(host, HIBERN8TIME_CAPABILITY_VALUE, PCS_TX1_HIBERN8TIME_CAPABILITY);

	mphy_pcs_writel(host, REG_H8_TIMEOUT_VAL_SET, PCS_RX0_REG_H8_TIMEOUT_VAL);
	mphy_pcs_writel(host, REG_H8_TIMEOUT_VAL_SET, PCS_RX1_REG_H8_TIMEOUT_VAL);

	if (readl(host->sysctrl + SCTRL_SCBAKDATA1_MASK_OFFSET) & BIT(11))
		val = ACTIVATETIME_1_5MS_CAPABILITY_VAL;

	mphy_pcs_writel(host, val, PCS_RX0_MIN_ACTIVATETIME_CAPABILITY);
	mphy_pcs_writel(host, val, PCS_RX1_MIN_ACTIVATETIME_CAPABILITY);

	mphy_reg_cap_enable(host, false);
}

static int mphy_fsm_calibration(struct ufs_hba *hba)
{
	int retry = MPHY_FSM_WAIT_TIME;
	u32 reg = 0;
	struct hufs_host *host = NULL;

	host = hba->priv;

	mphy_pma_writel(host, TOP_FSM_START_EN | TOP_FSM_START_ADDR, PMA_CP_CSR17);
	while (retry--) {
		reg = mphy_pma_readl(host, PMA_CP_CSR8);
		if (reg & TOP_FSM_DONE)
			break;
		udelay(2);
	}

	if (retry < 0) {
		dev_err(hba->dev, "UFS calibration timeout reg: 0x%x\n", reg);
		return -1;
	}

	if ((mphy_pma_readl(host, PMA_API_CSR1) & TOP_FSM_INIT_STATUS_API_MASK) != TOP_FSM_INIT_STATUS_API_SUCCESS) {
		dev_err(hba->dev, "UFS calibration falure result: 0x%x\n", mphy_pma_readl(host, PMA_API_CSR1));
		return -1;
	}

	dev_err(hba->dev, "UFS fsm calibration success\n");
	return 0;
}

void mphy_config_efuse(struct hufs_host *host)
{
	u32 reg;
	u32 reg1;
	if (host->host_id == 1) {
		reg = readl(host->sysctrl + SCTRL_SCEFUSEDATA8_OFFSET);
		reg &= EFUSE_UFS1_DATA8_MASK;
		reg1 = readl(host->sysctrl + SCTRL_SCEFUSEDATA9_OFFSET);
		reg1 &= EFUSE_UFS1_DATA9_MASK;
		reg1 <<= EFUSE_UFS1_DATA9_OFFSET;
		mphy_pma_writel(host, reg | reg1, PMA_EFUSE);

		reg = readl(host->sysctrl + SCTRL_SCEFUSEDATA2_OFFSET);
		reg1 = reg & BIT(23);
		reg &= EFUSE_UFS1_DATA2_MASK;
		reg >>= 5;
		reg1 >>= 19;
		mphy_pma_writel(host, reg | reg1, PMA_API_CSR13);
	} else {
		reg = readl(host->sysctrl + SCTRL_SCEFUSEDATA8_OFFSET);
		mphy_pma_writel(host, reg, PMA_EFUSE);

		reg = readl(host->sysctrl + SCTRL_SCEFUSEDATA2_OFFSET);
		reg &= EFUSE_UFS0_MPHY_MASK;
		mphy_pma_writel(host, reg, PMA_API_CSR13);
	}
}

int hufs_link_startup_pre_change(struct ufs_hba *hba)
{
	int err = 0;
	struct hufs_host *host = NULL;
	u32 i;

	host = hba->priv;

	dev_info(hba->dev, "%s ++\n", __func__);

#ifndef CONFIG_UFS_2DW_PRDT
	ufshcd_writel(hba, 0x0, REG_UFS_GCR_CFG);
	dev_err(hba->dev, "use 4DW PRDT set REG_UFS_GCR_CFG: %x.\n", ufshcd_readl(hba, REG_UFS_GCR_CFG));
#endif

	if (hba->host->is_emulator)
		return 0;

	/* for FPGA hufs MPHY */
	if ((host->caps & USE_HUFS_MPHY_TC)) {
		if (is_v200_mphy(hba)) {
			hufs_set_each_cfg_attr(
				hba, hufs_mphy_v200_pre_link_attr,
				ARRAY_SIZE(hufs_mphy_v200_pre_link_attr));
			mdelay(40); /* wait 40 ms */
		} else {
			hufs_set_each_cfg_attr(
				hba, hufs_mphy_v120_pre_link_attr,
				ARRAY_SIZE(hufs_mphy_v120_pre_link_attr));
		}
	} else {
		/* for ASIC hufs MPHY and emu */
		/* 1. csr0: slow link: 0x29-> API_PIN, 38.4M, clear api; 0x2b-> API_PIN, 38.4M, clear api, high mode */
		mphy_pma_writel(host, 0x29, PMA_API_CSR0);

		/* 2. mphy5 api config */
		for (i = 0; i < sizeof(hisi_fw_config_attr) / sizeof (struct ufs_attr_cfg); i++)
			mphy_pma_writel(host, hisi_fw_config_attr[i].val, hisi_fw_config_attr[i].addr);

		/* 3. core req config */
		mphy_pma_writel(host, CORE_FREQ_417 | FSM_SRAM_MAX_ADDR, PMA_TOP_FSM_CTRL);
		/* 4. load efuse config */
		mphy_config_efuse(host);
		/* 5. mphy calibration */
		err = mphy_fsm_calibration(hba);
		if (err)
			return err;
		/* 6. escape config */
		mphy_set_pcs_time_escape_trunk(host);
		/* 7. pcs init */
		mphy_set_pcs_init(host);

		/* 9. pcs hibern8 config */
		mphy_hibern8_config(host);
	}

	/* for hufs MPHY */
	if ((host->caps & USE_HUFS_MPHY_TC)) {
		if (is_v200_mphy(hba))
			hufs_mphy_v200_updata_fsm(hba, host);
		else
			hufs_mphy_updata_fsm_ocs5(hba, host);

		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL((u32)PA_ADAPTAFTERLRSTINPA_INIT, 0x0), 0x1);

		pr_info("%s --\n", __func__);

		return err;
	}
	dev_info(hba->dev, "%s --\n", __func__);

	return err;
}

static void hufs_mphy_link_post_config(
	struct ufs_hba *hba, struct hufs_host *host)
{
	u32 tx_lane_num = 1;
	u32 rx_lane_num = 1;

	if (host->caps & USE_HUFS_MPHY_TC) {
		/*
		 * set the PA_TActivate to 128. need to check in ASIC
		 * H8's workaround
		 */
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TACTIVATE, 0x0), 0x5);
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(0x80da, 0x0), 0x2d);

		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDTXDATALANES), &tx_lane_num);
		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDRXDATALANES), &rx_lane_num);

		/* RX_MC_PRESENT */
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(0x00c2, 0x4), 0x0);
		if (tx_lane_num > 1 && rx_lane_num > 1)
			/* RX_MC_PRESENT */
			ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(0x00c2, 0x5), 0x0);
	}
}

void set_device_clk(struct ufs_hba *hba)
{
}

static void ufs_hcd_dme_set_value(struct ufs_hba *hba)
{
	u32 val = 0;

	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG1SYNCLENGTH), &val);
	/* sync length devided into four step 4B 4C 4D 4E */
	if (val < 0x4B)
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TXHSG1SYNCLENGTH, 0x0), 0x4B);

	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG2SYNCLENGTH), &val);
	if (val < 0x4C)
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TXHSG2SYNCLENGTH, 0x0), 0x4C);

	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG3SYNCLENGTH), &val);
	if (val < 0x4D)
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TXHSG3SYNCLENGTH, 0x0), 0x4D);

	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG4SYNCLENGTH), &val);
	if (val < 0x4E)
		ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TXHSG4SYNCLENGTH, 0x0), 0x4E);
}

void mphy5_idle_set(struct ufs_hba *hba)
{
	u32 reg = 0;
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (hba->host->is_emulator || (host->caps & USE_HUFS_MPHY_TC))
		return;

	mphy_pcs_writel(host, STALL_LOW_PWR_EN_BIT, PCS_RX0_STALL_LOW_PWR_EN);
	mphy_pcs_writel(host, STALL_LOW_PWR_EN_BIT, PCS_RX1_STALL_LOW_PWR_EN);

	udelay(2);
	reg = mphy_pma_readl(host, PMA_API_CSR3);
	mphy_pma_writel(host, reg | CSR3_DS_STALL_EN_API_BIT, PMA_API_CSR3);
}

void mphy_config_tactivate(struct ufs_hba *hba)
{
	u32 pa_activate;
	u32 pa_granularity;
	u32 value;

	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TACTIVATE), &pa_activate);
	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_GRANULARITY), &pa_granularity);
	if ((pa_granularity == 0) || (pa_granularity > 0x6)) {
		dev_err(hba->dev, "pa_granularity value err\n");
		return;
	}

	if (pa_granularity == 0x1) {
		value = 0xA;
	} else if (pa_granularity == 0x2) {
		value = 0x3;
	} else if (pa_granularity == 0x3) {
		value = 0x2;
	} else {
		value = 0x1;
	}
	value += pa_activate;
	ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TACTIVATE, 0x0), value);
	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXTRAILINGCLOCKS), &value);
	ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(PA_TXTRAILINGCLOCKS, 0x0), value | TXTRAILINGCLOCKSVALUE);
}

int hufs_link_startup_post_change(struct ufs_hba *hba)
{
	struct hufs_host *host = NULL;
	u32 value = 0;
	u32 value_bak = 0;
	u32 i;

	host = hba->priv;

	dev_info(hba->dev, "%s ++\n", __func__);

	/* for hufs asic mphy and emu, use USE_HUFS_MPHY_ASIC on ASIC later */
	if (!(host->caps & USE_HUFS_MPHY_TC)) {
		mphy_config_tactivate(hba);
		for (i = 0; i < ARRAY_SIZE(hufs_mphy_post_link_attr); i++)
			ufshcd_dme_set(hba, hufs_mphy_post_link_attr[i].addr, hufs_mphy_post_link_attr[i].val);
	}

	if ((host->caps & USE_HUFS_MPHY_TC)) {
		if (is_v200_mphy(hba)) {
			hufs_mphy_v200_link_post_config(hba, host);
			hufs_set_each_cfg_attr(
				hba, hufs_mphy_v200_post_link_attr,
				ARRAY_SIZE(hufs_mphy_v200_post_link_attr));

			ufs_hcd_dme_set_value(hba);

			ufshcd_dme_get(hba, UIC_ARG_MIB_SEL(0x00ba, 0x0),
				       &value_bak);
			ufshcd_dme_get(hba, UIC_ARG_MIB_SEL(0x00ae, 0x0),
				       &value);
			ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(0x00ae, 0x0),
				       value | MPHY_BIT_REG_VCO_SW_VCAL);
			ufshcd_dme_set(hba, UIC_ARG_MIB_SEL(0x00ba, 0x0),
				       value_bak);
		} else {
			hufs_mphy_link_post_config(hba, host);
			hufs_set_each_cfg_attr(
				hba, hufs_mphy_v120_post_link_attr,
				ARRAY_SIZE(hufs_mphy_v120_post_link_attr));
		}
	}

	unipro_clk_auto_gate_enable(hba);
	mphy5_idle_set(hba);

	/* select debug counter0 mask0, bit 0: 1K symbol received */
	ufshcd_dme_set(hba, UIC_ARG_MIB((u32)VS_DEBUG_COUNTER0_MASK0), BIT(0));
	/* clear debug counter0 and enable it */
	ufshcd_dme_set(hba, UIC_ARG_MIB((u32)DEBUGCOUNTER_CLR),
		       BIT_DBG_CNT0_CLR);
	ufshcd_dme_set(hba, UIC_ARG_MIB((u32)DEBUGCOUNTER_EN), BIT_DBG_CNT0_EN);

	dev_info(hba->dev, "%s --\n", __func__);

	return 0;
}

/**
 * Soc init will reset host controller, all register value will lost
 * including memory address, doorbell and AH8 AGGR
 */
void hufs_full_reset(struct ufs_hba *hba)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;

#ifdef CONFIG_HUAWEI_UFS_DSM
	dsm_ufs_disable_volt_irq(hba);
#endif
	disable_irq(hba->irq);

	/* mphy sys clk close */
	writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPERDIS6);
	/* enable MPHY ISO */
	if (!((host->caps & USE_HUFS_MPHY_TC) || hba->host->is_emulator))
		writel(0x10201020, host->actrl + ACTRL_ISO_EN_GROUP0_PERI_OFFSET);
	mdelay(5);
	/* disable hba before device reset */
	ufshcd_hba_stop(hba);
	/* close dfa clk */
	if (host->host_id == 0) {
		clk_disable_unprepare(host->clk_ufs_dfa_sys);
		clk_disable_unprepare(host->clk_ufs0phy_ref);
	}

	/*
	 * Cancer platform need a full reset when error handler occurs.
	 * If a request sending in ufshcd_queuecommand passed through
	 * ufshcd_state check. And eh may reset host controller, a NOC
	 * error happens.
	 */
	msleep(1000); /* 1000ms sleep is enough for waiting those requests. */

	ufs_clk_init(hba);
	ufs_soc_init(hba);

	enable_irq(hba->irq);
#ifdef CONFIG_HUAWEI_UFS_DSM
	dsm_ufs_enable_volt_irq(hba);
#endif
}

/* compatible with old platform */
void hufs_pwr_change_pre_change(
	struct ufs_hba *hba, struct ufs_pa_layer_attr *dev_req_params)
{
	struct hufs_host *host = NULL;
	u32 value = 0;
	u32 peer_value = 0;

	host = hba->priv;
	hba->scramble = true;

	/* solve nac problem on samsung for nsv/xian */
	if ((hba->manufacturer_id == UFS_VENDOR_SAMSUNG) && (hba->ufs_device_spec_version == UFS_DEVICE_SPEC_3_1)) {
		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG1SYNCLENGTH), &value);
		if (value < 0x4f) {
			value += 1;
			ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG1SYNCLENGTH), value);
		}
		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG2SYNCLENGTH), &value);
		if (value < 0x4f) {
			value += 1;
			ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG2SYNCLENGTH), value);
		}
		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TXHSG3SYNCLENGTH), &value);
		if (value < 0x4f) {
			value += 1;
			ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG3SYNCLENGTH), value);
		}
	}

	if ((hba->manufacturer_id == UFS_VENDOR_SAMSUNG) && (hba->ufs_device_spec_version == UFS_DEVICE_SPEC_4_0)) {
		/* tx sync length */
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG1SYNCLENGTH), 0x4B);
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG2SYNCLENGTH), 0x4C);
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG3SYNCLENGTH), 0x4D);
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG4SYNCLENGTH), 0x4E);
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSG5SYNCLENGTH), 0x4F);
	}

	/* squelch detect diff_n config on THR925 & HH3.1 for nsv/xian */
	if ((!(host->caps & USE_HUFS_MPHY_TC)) && (host->caps & USE_HUFS_MPHY_5) &&
		(((hba->manufacturer_id == UFS_VENDOR_SKHYNIX) && (hba->ufs_device_spec_version <= UFS_DEVICE_SPEC_3_1)) ||
		strstarts(hba->model, UFS_MODEL_THOR925))) {
		mphy_pcs_writel(host, DIFN_TIMEOUT_VAL, PCS_LAN0_DIFN_TIMEOUT);
		mphy_pcs_writel(host, DIFN_TIMEOUT_VAL, PCS_LAN1_DIFN_TIMEOUT);
	}

	if (!(host->caps & BROKEN_FASTAUTO) && (hba->ufs_device_spec_version == UFS_DEVICE_SPEC_4_0)) {
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_EOBDELAY), 0xFFFF);
		ufshcd_dme_peer_set(hba, UIC_ARG_MIB(PA_EOBDELAY), 0xFFFF);

		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_EOBDELAY), &value);
		ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_EOBDELAY), &peer_value);
		dev_err(host->hba->dev, "PA_EOBDELAY:0x%x PEER PA_EOBDELAY:0x%x\n", value, peer_value);
	}

	if ((hba->manufacturer_id == UFS_VENDOR_SAMSUNG) && (hba->ufs_device_spec_version == UFS_DEVICE_SPEC_2_2)) {
		/* ss2.2 Changing the CDR Bandwidth avoid H8 bit errors */
		value = mphy_pma_readl(host, PMA_EFUSE);
		value &= 0xFFFFFFFC;
		value |= 0x2;
		mphy_pma_writel(host, value, PMA_EFUSE);
	}

	/* hi1865 G5 Eye improve */
	if (strstarts(hba->model, UFS_MODEL_THOR850) || strstarts(hba->model, UFS_MODEL_THOR950))
		mphy_pma_writel(host, 0x0, PMA_API_CSR7);

	ufs_pwr_change_pre_change(hba, dev_req_params);
}

void hufs_pre_hce_notify(struct ufs_hba *hba)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;
	u32 set;

	BUG_ON(!host->pericrg || !host->ufs_sys_ctrl || !host->pctrl ||
		!host->sysctrl || !host->pmctrl);

	if (ufshcd_is_hufs_hc(host->hba))
		BUG_ON(!host->actrl);

	set = ufshcd_readl(hba, REG_INTERRUPT_ENABLE);
	set &= ~UIC_COMMAND_COMPL;
	ufshcd_writel(hba, set, REG_INTERRUPT_ENABLE);

	return;
}

int ufs_get_hufs_hc(struct hufs_host *host)
{
	struct device_node *np = NULL;

	np = host->hba->dev->of_node;
	if (!np) {
		dev_err(host->hba->dev, "can't find device node hisilicon,hufs\n");
		return -ENXIO;
	}
	host->mphy_pcs = of_iomap(np, 3);
	if (!host->mphy_pcs) {
		dev_err(host->hba->dev, "mphy_pcs iomap error!\n");
		return -ENOMEM;
	}
	host->mphy_pma = of_iomap(np, 4);
	if (!host->mphy_pma) {
		dev_err(host->hba->dev, "mphy_pma iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,actrl");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,actrl\n");
		return -ENXIO;
	}
	host->actrl = of_iomap(np, 0);
	if (!host->actrl) {
		dev_err(host->hba->dev, "actrl iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt_crg");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt_crg\n");
		return -ENXIO;
	}

	host->hsdt_crg = of_iomap(np, 0);
	if (!host->hsdt_crg) {
		dev_err(host->hba->dev, "hsdt_crg iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt_ctrl");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt_ctrl\n");
		return -ENXIO;
	}

	host->hsdt_ctrl = of_iomap(np, 0);
	if (!host->hsdt_ctrl) {
		dev_err(host->hba->dev, "hsdt_ctrl iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt_sysctrl");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt_sysctrl\n");
		return -ENXIO;
	}

	host->hsdt_sysctrl = of_iomap(np, 0);
	if (!host->hsdt_sysctrl) {
		dev_err(host->hba->dev, "hsdt_sysctrl iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt1_sysctrl");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt1_sysctrl\n");
		return -ENXIO;
	}

	host->hsdt1_sysctrl = of_iomap(np, 0);
	if (!host->hsdt1_sysctrl) {
		dev_err(host->hba->dev, "hsdt1_sysctrl iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,ao_crg");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,ao_crg\n");
		return -ENXIO;
	}

	host->ao_crg = of_iomap(np, 0);
	if (!host->ao_crg) {
		dev_err(host->hba->dev, "ao_crg iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,peri_crg");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,peri_crg\n");
		return -ENXIO;
	}

	host->pericrg = of_iomap(np, 0);
	if (!host->pericrg) {
		dev_err(host->hba->dev, "peri_crg iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt1_crg");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt1_crg\n");
		return -ENXIO;
	}

	host->hsdt1_crg = of_iomap(np, 0);
	if (!host->hsdt1_crg) {
		dev_err(host->hba->dev, "hsdt1_crg iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt1_ctrl");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt1_ctrl\n");
		return -ENXIO;
	}

	host->hsdt1_ctrl = of_iomap(np, 0);
	if (!host->hsdt1_ctrl) {
		dev_err(host->hba->dev, "hsdt1_ctrl iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,pmctrl");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,pmctrl\n");
		return -ENXIO;
	}

	host->pmc_reg = of_iomap(np, 0);
	if (!host->pmc_reg) {
		dev_err(host->hba->dev, "pmctrl iomap error!\n");
		return -ENOMEM;
	}

#ifdef CONFIG_SCSI_UFS_INTR_HUB
	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt0_intr_hub");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt0_intr_hub\n");
		return -ENXIO;
	}

	host->hsdt0_intr_hub = of_iomap(np, 0);
	if (!host->hsdt0_intr_hub) {
		dev_err(host->hba->dev, "hsdt0_intr_hub iomap error!\n");
		return -ENOMEM;
	}

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt1_intr_hub");
	if (!np) {
		dev_err(host->hba->dev,
			"can't find device node hisilicon,hsdt1_intr_hub\n");
		return -ENXIO;
	}

	host->hsdt1_intr_hub = of_iomap(np, 0);
	if (!host->hsdt1_intr_hub) {
		dev_err(host->hba->dev, "hsdt1_intr_hub iomap error!\n");
		return -ENOMEM;
	}
#endif

	if (host->host_id == 0) {
		host->clk_ufs0phy_ref = devm_clk_get(host->hba->dev, "clk_ufs0phy_ref");
		if (IS_ERR(host->clk_ufs0phy_ref)) {
			dev_err(host->hba->dev, "clk_ufs0phy_ref not found.\n");
			return -ENOMEM;
		}

		return 0;
	}

	host->clk_ufs_dfa_sys = devm_clk_get(host->hba->dev, "clk_ufs_dfa_sys");
	if (IS_ERR(host->clk_ufs_dfa_sys)) {
		dev_err(host->hba->dev, "clk_ufs_dfa_sys not found.\n");
		return -ENOMEM;
	}

	return 0;
}

void ufshcd_idle_auto_gating(struct ufs_hba *hba)
{
	if (hba->use_mcq)
		ufshcd_writel(hba, UFSHC_CLOCK_GATING_CONFIG, UFS_BLOCK_CG_CFG);
}

void hufs_txpost_enable(struct ufs_pa_layer_attr *dev_req_params,u32 *value)
{
	UNUSED(dev_req_params);
	UNUSED(value);
	return;
}

void hufs_mphy_tactivate_adapt(struct ufs_hba *hba)
{
	check_pa_granularity(hba);
}

void ufshcd_enable_autodiv(struct ufs_hba *hba)
{
	u32 reg;
	struct hufs_host *host = NULL;

	host = hba->priv;

	if (host->host_id == 1) {
		writel(GT_PCLK_AUTODIV_UFS_SYS_SUBSYS_HSDT1, host->hsdt1_crg + HSDT1_CRG_PEREN1_OFFSET);

		reg = readl(host->hsdt1_crg + UFS1_PERI_AUTODIV1_OFFSET);
		reg &= (~UFS_SYS_SUBSYS_HSDT1_DIV_AUTO_CFG_MASK);
		reg |= UFS_SYS_SUBSYS_HSDT1_DIV_AUTO_CFG_UFS;
		writel(reg, host->hsdt1_crg + UFS1_PERI_AUTODIV1_OFFSET);

		reg = readl(host->hsdt1_crg + UFS1_PERI_AUTODIV1_OFFSET);
		reg &= (~UFS_SYS_SUBSYS_HSDT1_IDLE_BYPASS);
		writel(reg, host->hsdt1_crg + UFS1_PERI_AUTODIV1_OFFSET);

		dev_info(hba->dev, "ufs1 PERI_AUTODIV1:0x%x\n", readl(host->hsdt1_crg + UFS1_PERI_AUTODIV1_OFFSET));
	} else {
		writel(GT_PCLK_AUTODIV_UFS_DFA_SYS, host->pericrg + PEREN0_OFFSET);

		reg = readl(host->pericrg + PERI_AUTODIV1_OFFSET);
		reg &= (~UFS_DFA_SYS_DIV_AUTO_CFG_MASK);
		reg |= UFS_DFA_SYS_DIV_AUTO_CFG_UFS;
		writel(reg, host->pericrg + PERI_AUTODIV1_OFFSET);

		reg = readl(host->pericrg + PERI_AUTODIV1_OFFSET);
		reg &= (~UFS_DFA_SYS_IDLE_BYPASS_DIS);
		writel(reg, host->pericrg + PERI_AUTODIV1_OFFSET);

		dev_info(hba->dev, "ufs0 PERI_AUTODIV1:0x%x\n", readl(host->pericrg + PERI_AUTODIV1_OFFSET));
	}
}
void v300_gears_config(struct ufs_hba *hba, u32 gear_rx)
{
}

