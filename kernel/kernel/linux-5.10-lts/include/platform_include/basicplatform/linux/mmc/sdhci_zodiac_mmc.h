/*
 * zodiac emmc sdhci controller interface.
 * Copyright (c) Zodiac Technologies Co., Ltd. 2017-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __SDHCI_ZODIAC_SCORPIO_H
#define __SDHCI_ZODIAC_SCORPIO_H

#define SCTRL_SCBAKDATA1_MSK (SOC_ACPU_SCTRL_BASE_ADDR + 0x4C4)
#define EMMC_AP_LOWER_STATUS (0x7 << 0)
#define EMMC_SENSOR_LOWER_STATUS (0x7 << 3)
#define EMMC_REAL_LOWER_STATUS (0x7 << 6)
#define AP_USE_FLAG ((0x1 << 9) & (0x0 << 13))
#define AP_DONT_USE_FLAG (0x0 << 9)
#define SENSOR_USE_FLAG (0x1 << 10)
#define NONE_USE_FLAG (0x1 << 11)
#define EMMC_INI_COMOLETE_FLAG (0x1 << 12)
#define AP_WANT_USE_FLAG (0x1 << 13)
#define SENSOR_WANT_USE_FLAG (0x1 << 14)
#define EMMC_POWER_FLAG 0
#define EMMC_USE_FLAG 1
#define INATIAL_FLAG 2

#define PERI_CRG_ADDR 0xFFB05000
#define GT_CLK_SDIO_HF_SD (0x1 << 12)
#define GT_ACLK_SDIO (0x1 << 3)
#define HSDT_CRG_PERSTAT0 0xC
#define PERI_CRG_PEREN6 0x410
#define PERI_CRG_PERSTAT6  0x41C

#define EMMC_AP_POWER_ATTRIBUTE 0
#define EMMC_SENSOR_POWER_ATTRIBUTE 1
#define EMMC_STATUS_ATTRIBUTE 2
#define EMMC_INATIAL_ATTRIBUTE 3
#define EMMC_APPLY_FLAG 4

#define LEGACY_400K 0
#define MMC_HS 1
#define SD_HS  2
#define SDR12  3
#define SDR25  4
#define SDR50  5
#define SDR104 6
#define DDR50  7
#define DDR52  8
#define HS200  9

#define DISABLE	0
#define ENABLE	1

#define EMMC_PHY_PHYINITCTRL 0x004
#define EMMC_PHY_DATENA_DLY 0x23C
#define EMMC_PHY_TUNING_CONFIG 0x218
#define EMMC_PHY_PHYCTRL2 0x248
#define EMMC_PHY_DLY_CTL 0x250
#define EMMC_PHY_DLY_CTL1 0x254
#define EMMC_PHY_DLY_CTL2 0x258

#define HSDT1_CLKDIV1	0xAC
#define HSDT1_CLKDIV1_MASK	(0xFFFF << 16)
#ifdef CONFIG_MMC_SDHCI_ZODIAC_SCORPIO
/* hsdt1 sd reset */
#define PERRSTEN0	0x20
#define PERRSTDIS0	0x24
#define IP_RST_SD	(0x1 << 1)
#define IP_HRST_SD	(0x1 << 0)

#define PERRSTEN1 0x2C
#define PERRSTDIS1 0x30
#define PERRSTSTAT1 0x34

#define IP_HRST_SDIO (0x1 << 2)
#define IP_RST_SDIO (0x1 << 3)

#define CLKDIV1	0xAC
#define SDCLK_3_2 0x0
#define PPLL2	0x1
#define SDPLL	0x2
#define MASK_PLL	(0x3 << 5)
#define MASK_DIV_SDPLL	(0xF << 1)

#define PEREN0	0x0
#define PERDIS0	0x4
#define PERSTAT0	0xC
#define GT_HCLK_SD	0x1

#define PEREN1	0x10
#define PERDIS1	0x14
#define PERSTAT1	0x1C
/* SD SSC */
#define SDPLLSSCCTRL	0x190

#define SSCG_ENABLE	0x1 /* Module enable */
#define SSCG_MODE_DOWN	(0x1 << 1) /* spread spectrum mode of the SSCG module */
#define SSCG_RATE	(0x2 << 2)
#define SSCG_DEPTH_625	(0x4 << 6) /* modulation depth of the SSCG module */
#define SSCG_FAST_DIS	(0x1 << 9) /* enable the fast exit function of the SSCG module */

#define SSCG_ENABLE_BITMASK	(0x1 << 16)
#define SSCG_BITMASK	(0x3FE << 16)
#define SSC_CLK_SDPLL_GT	(0x1 << 20) /* ssc clock gate */

#define SSC_DEPTH_625	(SSCG_BITMASK | SSCG_RATE | SSCG_MODE_DOWN | SSCG_DEPTH_625 | SSCG_FAST_DIS)
#define SSC_ENABLE	(SSCG_ENABLE | SSCG_ENABLE_BITMASK)
#define SSC_DISABLE	SSCG_ENABLE_BITMASK
#endif

#ifdef CONFIG_MMC_SDHCI_ZODIAC_ANDROMEDA
#define PERIPRST1 0x60
#define PERIPDIS1 0x64
#define PERIPSTAT1 0x68
#define IP_RST_EMMC_BIT  (0x1 << 0)
#define IP_PRST_EMMC_BIT (0x1 << 14)
#endif

#ifdef CONFIG_MMC_SDHCI_ZODIAC_APUS
#define PERRSTEN0 0x200
#define PERRSTEN1 0x20C
#define PERRSTEN2 0x218
#define PERRSTEN5 0x09C

#define PERRSTDIS0 0x204
#define PERRSTDIS1 0x210
#define PERRSTDIS2 0x21C
#define PERRSTDIS5 0x0A0

#define PERRSTSTAT0 0x208
#define PERRSTSTAT1 0x214
#define PERRSTSTAT2 0x220
#define PERRSTSTAT5 0x0A4

#define IP_RST_EMMC  (0x1 << 1)
#define IP_RST_EMMC_BUS_BIT (0x1 << 25)
#define IP_RST_SDIO  (0x1 << 3)
#define IP_ARST_SDIO (0x1 << 2)
#endif

#define SDEMMC_CRG_CTRL	0x80
#define TUNING_SAMPLE_MASK	(0x1F << 8)
#define TUNING_SAMPLE_MASKBIT	0x8

#define CLK_CTRL_TIMEOUT_SHIFT 16
#define CLK_CTRL_TIMEOUT_MASK (0xf << CLK_CTRL_TIMEOUT_SHIFT)
#define CLK_CTRL_TIMEOUT_MIN_EXP 13

#define SDHCI_ZODIAC_MIN_FREQ 100000
#define SDHCI_ZODIAC_CLK_FREQ 960000000
#define MUX_SDSIM_VCC_STATUS_2_9_5_V 0
#define MUX_SDSIM_VCC_STATUS_1_8_0_V 1
#define LEVER_SHIFT_GPIO  101
#define LEVER_SHIFT_3_0V 0
#define LEVER_SHIFT_1_8V 1


#define TUNING_PHASE_INVALID (-1)
#define TUNING_LOOP_COUNT 10
#if defined(CONFIG_MMC_SDHCI_ZODIAC_SCORPIO) || defined(CONFIG_MMC_SDHCI_ZODIAC_APUS)
#define MAX_TUNING_LOOP 40
#elif defined(CONFIG_MMC_SDHCI_ZODIAC_ANDROMEDA)
#define MAX_TUNING_LOOP 10
#endif

#define BLKSZ_LARGE 128
#define BLKSZ_SMALL 64
#define SDHCI_MAX_COUNT 0xFFF
#define SDHCI_DMD_ERR_MASK 0xffffffff
#define SDHCI_DSM_ERR_INT_STATUS 16
#define SDHCI_ERR_CMD_INDEX_MASK 0x3f
#define SDHCI_SAM_PHASE_MID 2
#define SDHCI_TUNING_MOVE_STEP 2
#define SDHCI_TUNING_RECORD 0x1
#define SDHCI_TUNING_MAX 63
#define CMDQ_SEND_STATUS_CQSSC1 0x10100
#define SDHCI_ADMA_SUPPORT_BIT 64
#define SDHCI_SDMA_SUPPORT_BIT 32
#define MMC_BLOCK_SIZE 512
#define SDHCI_PINCTL_CLEAR_BIT (0xF << 4)
#define SDHCI_WRITE_PINCTL_FLAG (0x7 << 4)
#define MMC_I2C_ADAPTER_NUM 2
#define SDHCI_I2C_MASTER_LENTH 8
#define SDHCI_ZODIAC_CLK_STABLE_TIME 20

/* SDHCI_CRG_CTRL0 used for zodiac */
#define SDHCI_CRG_CTRL0 0x80
#define SDHCI_ZODIAC_CLK_SAMPLE_MODE_OFFSET 31
#define SDHCI_ZODIAC_CLK_DIV_OFFSET 24
#define SDHCI_ZODIAC_CLK_DLY_DRV_OFFSET 16
#define SDHCI_ZODIAC_CLK_DLY_SAMPLE_OFFSET 8
#define SDHCI_ZODIAC_CLK_DIV_MASK (0xf << 24)
#define SDHCI_ZODIAC_CLK_DLY_DRV_MASK (0x1f << 16)
#define SDHCI_ZODIAC_CLK_DLY_SAMPLE_MASK (0x1f << 8)
#define SDHCI_ZODIAC_CLK_400K 400000
#define SDHCI_SDEMMC_CLK_DIV_10M (0x1 << 24)
#define SDHCI_SDEMMC_CLK_DLY_SAMPLE_180 (0x7 << 8)
#define SDHCI_ZODIAC_TIMING_MODE 11
#define SDHCI_ZODIAC_CONTROLLER_CLK_FREQ 200000000
#define SC_BAKDATA5_MASK 0x4D4
#define SDHCI_ZODIAC_SDIO_AP_SUSPEND 0xFFFF0001
#define SDHCI_ZODIAC_SDIO_AP_RESUME 0xFFFF0002

#define SDEMMC_TIMING_CTRL_1 0x94
#define SDEMMC_NSB_CFG_MASK 0xff0000
#define SDEMMC_NSB_CFG_LONG 0x8C0000
#define SDEMMC_NSB_CFG_SHORT 0x100000

#define ACTRL_BITMSK_NONSEC_CTRL1 0x400
#define SDCARD_SEL18_BITMASK 27
#define SDCARD_SEL18_OFFSET 11

#define ZODIAC_DFX_REG1 0x800
#define ZODIAC_DFX_REG2 0x900
#define SDHCI_CMDQ_REG_BASE 0xE00
#if defined(CONFIG_DFX_DEBUG_FS)
extern void proc_sd_test_init(void);
extern void test_sd_delete_host_caps(struct mmc_host *host);
#endif

#define NONSEC_STATUS20 0x140
#define NONSEC_STATUS22 0x148

#define DFX_BUSY 0x700
#define DFX_OST 0x704
#define TB_LITE_DDR2DMC_RD 0xFA360000
#define TB_LITE_DDR2DMC_WR 0xFA361000
#define IB_MMC 0xFA390000
#define IB_LITE_DDR2DMC_RD 0xFF903000
#define IB_LITE_DDR2DMC_WR 0xFF904000
#define TUNING_RESULT_VALUE 0x88

#ifdef CONFIG_MMC_SDHCI_ZODIAC_APUS
#define EMMC_PHY_ADDR 0xFA064000
#define SDIO_PHY_ADDR 0xEFB0A000
#endif

struct sdhci_crg_ctrl {
	unsigned long clk_src;
	unsigned long clk_dly_sample;
	unsigned long clk_dly_drv;
	unsigned long clk_div;
	unsigned int sample_mode;
	unsigned int max_clk;
};

/**
 * struct sdhci_zodiac_data
 * @clk:						Pointer to the sd clock
 * @clock:					record current clock rate
 * @tuning_current_sample:		record current sample when soft tuning
 * @tuning_init_sample:			record the optimum sample of soft tuning
 * @tuning_sample_count:		record the tuning count for soft tuning
 * @tuning_move_sample:		record the move sample when data or cmd error occor
 * @tuning_move_count:		record the move count
 * @tuning_sample_flag:			record the sample OK or NOT of soft tuning
 * @tuning_strobe_init_sample:	default strobe sample
 * @tuning_strobe_move_sample:	record the strobe move sample when data or cmd error occor
 * @tuning_strobe_move_count:	record the strobe move count
 */
struct sdhci_zodiac_data {
	struct sdhci_host *host;
	struct clk *ciu_clk;
	struct clk *biu_clk;
	struct clk *niu_clk;
	struct clk *tiu_clk; /* AO ulpll clk for tbu_sd */
	unsigned int clock;
	struct sdhci_crg_ctrl *pcrgctrl;
	/* pinctrl handles */
	struct pinctrl		*pinctrl;
	struct pinctrl_state	*pins_default;
	struct pinctrl_state	*pins_idle;
	struct workqueue_struct	*card_workqueue;
	struct work_struct	card_work;
	struct device	*dev;

	unsigned int clk_restore;
	unsigned int tuning_loop_max;
	unsigned int tuning_count;
	unsigned int hw_tuning_phase_best;
	unsigned int tuning_phase_best;
	unsigned int tuning_phase_record;

	unsigned int tuning_move_phase;
	unsigned int tuning_move_count;
	unsigned int tuning_phase_max;
	unsigned int tuning_phase_min;
#ifdef CONFIG_SYS_PM_STR
	unsigned int str_enter_end;
#endif
	unsigned int first_mux_sdsim_probe_init;
	int old_power_mode;
	unsigned int mux_sdsim;
	unsigned int tbu_sd_bypass;
	int mux_vcc_status;
	unsigned int	lever_shift;
	bool delay_measure;
	int lever_shift_ionumb;

	int sdhci_host_id;
#define SDHCI_EMMC_ID	0
#define SDHCI_SD_ID		1
#define SDHCI_SDIO_ID	2
	unsigned int	flags;
#define PINS_DETECT_ENABLE	(1 << 0)	/* NM card 4-pin detect control */
	int card_detect;
#define CARD_REMOVED 0
#define CARD_ON 1
#define CARD_UNDETECT 2
	unsigned int cd_vol;
	int gpio_cd;
#ifdef CONFIG_MMC_SIM_GPIO_EXCHANGE
	int need_get_mmc_regulator;
#endif
#ifdef CONFIG_HUAWEI_EMMC_DSM
	u64 para;
	u32 cmd_data_status;
	void *data;
	struct work_struct dsm_work;
	spinlock_t sdhci_dsm_lock;
#endif
};

#ifdef CONFIG_HUAWEI_EMMC_DSM
void sdhci_dsm_set_host_status(struct sdhci_host *host, u32 error_bits);
void sdhci_dsm_work(struct work_struct *work);
void sdhci_dsm_handle(struct sdhci_host *host, struct mmc_request *mrq);
#endif

extern int check_mntn_switch(int feature);
extern void sdhci_dumpregs(struct sdhci_host *host);
extern int config_sdsim_gpio_mode(enum sdsim_gpio_mode gpio_mode);
extern void sdhci_set_default_irqs(struct sdhci_host *host);
extern int sdhci_send_command_direct(struct mmc_host *mmc, struct mmc_request *mrq);
extern void sdhci_retry_req(struct sdhci_host *host, struct mmc_request *mrq);
extern void sdhci_set_vmmc_power(struct sdhci_host *host, unsigned short vdd);
extern void sdhci_zodiac_gpio_value_set(int value, int gpio_num);
extern void sdhci_zodiac_qic_idle_config(struct sdhci_host *host, u32 enable);
extern void sdhci_zodiac_tbu_config(struct sdhci_host *host, u32 enable);

#ifdef CONFIG_MMC_SDHCI_MUX_SDSIM
extern int gpio_current_number_for_sd_clk;
extern int gpio_current_number_for_sd_cmd;
extern int gpio_current_number_for_sd_data0;
extern int gpio_current_number_for_sd_data1;
extern int gpio_current_number_for_sd_data2;
extern int gpio_current_number_for_sd_data3;
extern void set_sd_sim_group_io_register(
		int gpionumber, int function_select, int pulltype, int driverstrenth);
#endif
#ifdef CONFIG_ZODIAC_SHARE_MMC
extern void mmc_status_mark(u32 attribute_flag, u32 use_flag);
#endif
extern u32 mmc_status_check(u32 flag);

int sdhci_zodiac_get_clock_gt(struct platform_device *pdev, struct sdhci_zodiac_data *sdhci_zodiac);
void sdhci_zodiac_sd_ssc_disconfig(struct sdhci_host *host);
void sdhci_zodiac_sd_ssc_config(struct sdhci_host *host);
void sdhci_zodiac_hardware_reset(struct sdhci_host *host);
void sdhci_zodiac_hardware_disreset(struct sdhci_host *host);
int sdhci_zodiac_get_resource(struct platform_device *pdev, struct sdhci_host *host);
void sdhci_zodiac_qos_config(struct sdhci_host *host);
void sdhci_zodiac_clk_restore(struct sdhci_host *host);
void sdhci_zodiac_clk_save(struct sdhci_host *host);
int ioctl_rpmb_card_status_poll(struct mmc_card *card, u32 *status,
				       u32 retries_max);
int mmc_set_blockcount(struct mmc_card *card, unsigned int blockcount,
			bool is_rel_write);
#ifdef CONFIG_MMC_SIM_GPIO_EXCHANGE
void sdhci_zodiac_put_regulator(struct sdhci_host *host);
void sdhci_zodiac_get_regulator(struct sdhci_host *host);
#endif
#ifdef CONFIG_MMC_SDHCI_ZODIAC_APUS
void sdhci_zodiac_mphy_clk_init(struct sdhci_host *host);
void sdhci_zodiac_mphy_delay_line(struct sdhci_host *host, struct mmc_ios *ios);
int sdhci_try_enable_clk(struct sdhci_host *host, u16 clk);
#endif

void sdhci_zodiac_disable_clk(struct sdhci_host *host);
void sdhci_zodiac_enable_clk(struct sdhci_host *host);
void sdhci_zodiac_phy_disable_clk(struct sdhci_host *host);
#define sdhci_mmc_sys_writel(host, val, reg)                                   \
	writel((val), (host)->mmc_sys + (reg))
#define sdhci_mmc_sys_readl(host, reg) readl((host)->mmc_sys + (reg))

#define sdhci_hsdtcrg_writel(host, val, reg)					\
	writel((val), (host)->hsdtcrg + (reg))
#define sdhci_hsdtcrg_readl(host, reg) readl((host)->hsdtcrg + (reg))

#define sdhci_hsdt1crg_writel(host, val, reg)                                   \
	writel((val), (host)->hsdt1crg + (reg))
#define sdhci_hsdt1crg_readl(host, reg) readl((host)->hsdt1crg + (reg))

#define sdhci_litecrg_writel(host, val, reg)                                   \
	writel((val), (host)->litecrg + (reg))
#define sdhci_litecrg_readl(host, reg) readl((host)->litecrg + (reg))

#define sdhci_actrl_writel(host, val, reg)                                   \
	writel((val), (host)->actrl + (reg))
#define sdhci_actrl_readl(host, reg) readl((host)->actrl + (reg))

#define sdhci_mphy_writel(host, val, reg)                                       \
	writel((val), (host)->mmc_phy + (reg))
#define sdhci_mphy_readl(host, reg) readl((host)->mmc_phy + (reg))

#define sdhci_crgctrl_writel(host, val, reg)                                   \
	writel((val), (host)->crgctrl + (reg))
#define sdhci_crgctrl_readl(host, reg) readl((host)->crgctrl + (reg))

#define sdhci_sysctrl_writel(host, val, reg)                                   \
	writel((val), (host)->sctrl + (reg))
#define sdhci_sysctrl_readl(host, reg) readl((host)->sctrl + (reg))

#endif
