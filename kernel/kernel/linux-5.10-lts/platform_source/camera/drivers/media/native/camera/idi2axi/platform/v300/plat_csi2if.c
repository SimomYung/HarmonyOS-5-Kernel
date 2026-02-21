/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: platform dependent csi2if interface implementations
 *
 * This file is released under the GPLv2.
 */
#include "plat_csi2if.h"
#include <linux/delay.h>
#include <soc_csi2if_interface.h>
#include <soc_csi_wrapper_interface.h>
#include <reg_helper.h>
#include "i2a_csi2if.h"
#include "plat_idi2axi.h"
#include "i2a_debug.h"

/*  enum CSI2IF_REG_TYPE
 *
 *  For common config, different csi a/b/c/d/e... may use same layout register.
 *  provide a reg type to get register addr for different csi instance.
 */
enum csi2if_reg_type {
	CSI2IF_PHY_TEST_CTRL,
	CSI2IF_PHY_TEST_DOUT,
	CSI2IF_PHY_RST_CTRL,
	CSI2IF_PHY_CTRL,
	CSI2IF_CSI_CTRL,
	CSI2IF_CSI_CGR_RESET, // reset = cgr + 12bytes
	CSI2IF_CSI_DGB,
	CSI2IF_REG_MAX,
};
#define MAX_CSI_COUNT 7
#define MAX_PHY_COUNT 6
#define RESET_CGR_OFFSET (SOC_CSI_WRAPPER_CSI_RESET_0_ADDR(0) - SOC_CSI_WRAPPER_CSI_CGR_0_SET_ADDR(0))

#define csi2if_irq_bit(regtype, bitname) def_irq_bit(SOC_CSI2IF_INT_ ## regtype, bitname)
// IRQ
static struct irq_bit_info csi2if_int_phy_fatal[] = {
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_errsotsynchs_0),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_errsotsynchs_1),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_errsotsynchs_2),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_errsotsynchs_3),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_rxinvalidcodehs_0),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_rxinvalidcodehs_1),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_rxinvalidcodehs_2),
	csi2if_irq_bit(MSK_PHY_FATAL, mask_phy_rxinvalidcodehs_3),
};

static struct irq_bit_info csi2if_int_frame_fatal[] = {
	csi2if_irq_bit(MSK_FRAME_FATAL, mask_err_frame_number_nomatch),
	csi2if_irq_bit(MSK_FRAME_FATAL, mask_err_two_con_fend),
	csi2if_irq_bit(MSK_FRAME_FATAL, mask_err_two_con_fstart),
	csi2if_irq_bit(MSK_FRAME_FATAL, mask_err_f_seq),
};

static struct irq_bit_info csi2if_int_phy[] = {
	csi2if_irq_bit(MSK_PHY, mask_phy_errsoths_0),
	csi2if_irq_bit(MSK_PHY, mask_phy_errsoths_1),
	csi2if_irq_bit(MSK_PHY, mask_phy_errsoths_2),
	csi2if_irq_bit(MSK_PHY, mask_phy_errsoths_3),
};

static struct irq_bit_info csi2if_int_skew_fifo[] = {
	csi2if_irq_bit(SKEW_FIFO, fifo_full_push),
	csi2if_irq_bit(SKEW_FIFO, fifo_empty_pop),
	csi2if_irq_bit(SKEW_FIFO, skew_err),
};

static struct irq_bit_info csi2if_int_skew_fifo_force[] = {
	csi2if_irq_bit(SKEW_FIFO_FORCE, force_fifo_full_push),
	csi2if_irq_bit(SKEW_FIFO_FORCE, force_fifo_empty_pop),
	csi2if_irq_bit(SKEW_FIFO_FORCE, force_skew_err),
};

static struct irq_bit_info csi2if_int_st_pkt[] = {
	csi2if_irq_bit(ST_PKT_MASK, mask_ecc_double),
	csi2if_irq_bit(ST_PKT_MASK, mask_cphy_header_crc),
	csi2if_irq_bit(ST_PKT_MASK, mask_payload_crc_err),
	csi2if_irq_bit(ST_PKT_MASK, mask_data_type),
	csi2if_irq_bit(ST_PKT_MASK, mask_ecc_corrected),
};

static struct irq_bit_info csi2if_int_st_main[] = {
	csi2if_irq_bit(ST_MAIN, status_int_phy_fatal),
	csi2if_irq_bit(ST_MAIN, status_int_frame_fatal),
	csi2if_irq_bit(ST_MAIN, status_int_phy),
	csi2if_irq_bit(ST_MAIN, status_int_pkt),
	csi2if_irq_bit(ST_MAIN, status_int_skew_fifo),
};

static struct irq_bit_info csi2if_int_st_pkt_dt[] = {
	csi2if_irq_bit(ST_PKT_DT, err_pkt_channel_id),
	csi2if_irq_bit(ST_PKT_DT, err_pkt_data_type),
	csi2if_irq_bit(ST_PKT_DT, err_pkt_f_seq_numb),
};

static struct irq_reg_info csi2if_irq_regs[] = {
	{ 0x0c, 0, csi2if_int_st_main,         ARRAY_SIZE(csi2if_int_st_main) },
	{ 0x58, 0, csi2if_int_phy_fatal,       ARRAY_SIZE(csi2if_int_phy_fatal) },
	{ 0x64, 0, csi2if_int_frame_fatal,     ARRAY_SIZE(csi2if_int_frame_fatal) },
	{ 0x74, 0, csi2if_int_phy,             ARRAY_SIZE(csi2if_int_phy) },
	{ 0x80, 0, csi2if_int_skew_fifo,       ARRAY_SIZE(csi2if_int_skew_fifo) },
	{ 0x88, 0, csi2if_int_skew_fifo_force, ARRAY_SIZE(csi2if_int_skew_fifo_force) },
	{ 0x8c, 0, csi2if_int_st_pkt,          ARRAY_SIZE(csi2if_int_st_pkt) },
	{ 0x90, 0, csi2if_int_st_pkt_dt,       ARRAY_SIZE(csi2if_int_st_pkt_dt) },
};

static uint32_t csi2if_debug_offset[] = {
	0x1C, /* SOC_CSI2IF_PHY_RX_ADDR */
	0x20, /* SOC_CSI2IF_PHY_STOPSTATE_ADDR */
	0x2c, /* SOC_CSI2IF_PHY_CAL_ADDR */
	0x44, /* SOC_CSI2IF_SKEW_FIFO_CFG_ADDR */
	0x4c, /* SOC_CSI2IF_PHY_DEBUG_OUT_ADDR */
	0x50, /* SOC_CSI2IF_DBG_TOTAL_FRAME_NUM_ADDR */
	0x54, /* SOC_CSI2IF_DBG_TOTAL_PACKAGE_NUM_ADDR */
	0x68, /* SOC_CSI2IF_FRAME_FATAL_TYPE_ADDR */
};

static void csi2if_int_mask_set(void __iomem *csi_base, uint32_t value)
{
	pr_info("csi2if: setting int mask:%#x", value);
	write_reg(value, SOC_CSI2IF_INT_MSK_PHY_FATAL_ADDR(csi_base));
	write_reg(value, SOC_CSI2IF_INT_MSK_FRAME_FATAL_ADDR(csi_base));
	write_reg(value, SOC_CSI2IF_INT_MSK_PHY_ADDR(csi_base));
	write_reg(value, SOC_CSI2IF_INT_SKEW_FIFO_MASK_ADDR(csi_base));
	write_reg(value, SOC_CSI2IF_INT_ST_PKT_MASK_ADDR(csi_base));
}

static void dump_irq_reg_info(void __iomem *csi_base, struct irq_reg_info *reginfo)
{
	uint32_t i, value;
	struct irq_bit_info *bitinfo;

	value = read_reg(csi_base + reginfo->irq_stat_offset);
	if (!value)
		return;

	pr_info("csi2if: irq offset:%#x, val:%#010x", reginfo->irq_stat_offset, value);
	bitinfo = reginfo->irq_bit_info;
	for (i = 0; i < reginfo->irq_bit_count; ++i) {
		if (bitinfo[i].bit_mask & value) {
			pr_info("csi2if:    reg:%s bit:%s set",
				bitinfo[i].reg_name, bitinfo[i].bit_name);
		}
	}
}

static inline bool is_cphy(struct csi_phy_info *info)
{
	return info->phy_mode == IDI_PHY_MODE_CPHY;
}

static uint32_t reg_type_offset[CSI2IF_REG_MAX][MAX_CSI_COUNT] = {
	[CSI2IF_CSI_CGR_RESET] = {
		SOC_CSI_WRAPPER_CSI_CGR_0_ADDR(0),
		SOC_CSI_WRAPPER_CSI_CGR_0_ADDR(0),
		SOC_CSI_WRAPPER_CSI_CGR_0_ADDR(0),
		SOC_CSI_WRAPPER_CSI_CGR_1_ADDR(0),
		SOC_CSI_WRAPPER_CSI_CGR_2_ADDR(0),
		SOC_CSI_WRAPPER_CSIF_CGR_ADDR(0),
		SOC_CSI_WRAPPER_CSIG_CGR_ADDR(0),
	},
	[CSI2IF_CSI_DGB] = {
		SOC_CSI_WRAPPER_PHY_CSI_A_DBG_ADDR(0),
		SOC_CSI_WRAPPER_PHY_CSI_B_DBG_ADDR(0),
		SOC_CSI_WRAPPER_PHY_CSI_C_DBG_ADDR(0),
		SOC_CSI_WRAPPER_PHY_CSI_D_DBG_ADDR(0),
		SOC_CSI_WRAPPER_PHY_CSI_E_DBG_ADDR(0),
		SOC_CSI_WRAPPER_PHY_CSI_F_DBG_ADDR(0),
		SOC_CSI_WRAPPER_PHY_CSI_G_DBG_ADDR(0),
	},
};

static inline void __iomem *csi2if_get_reg(struct csi_phy_info *info, uint32_t rtype)
{
	uint32_t offset = 0;

	switch (rtype) {
	case CSI2IF_PHY_TEST_CTRL:
		offset = SOC_CSI_WRAPPER_CDPHY_A_TEST_CTRL_ADDR(0) + info->phy_id * 8 * 8;
		break;
	case CSI2IF_PHY_TEST_DOUT:
		offset = SOC_CSI_WRAPPER_CDPHY_A_TEST_DOUT_ADDR(0) + info->phy_id * 8 * 8;
		break;
	case CSI2IF_PHY_RST_CTRL:
		offset = SOC_CSI_WRAPPER_CDPHY_A_SHUTDOWN_RST_CTRL_ADDR(0) + info->phy_id * 8 * 8;
		break;
	case CSI2IF_PHY_CTRL:
		offset = SOC_CSI_WRAPPER_CDPHY_A_CTRL_ADDR(0) + info->phy_id * 8 * 8;
		break;
	case CSI2IF_CSI_CTRL:
		offset = SOC_CSI_WRAPPER_CSIA_CTRL_ADDR(0) + info->csi_index * 4;
		break;
	default:
		offset = reg_type_offset[rtype][info->csi_index];
		break;
	}

	return (uint8_t *)info->wrapper_base + offset;
}

// phy_mode: works in cphy or dphy
// phy_workmode: single or dual_a or dual_b
static void csi_phy_connect(struct csi_phy_info *info)
{
	SOC_CSI_WRAPPER_CSIA_CTRL_UNION ctrl;
	void __iomem *addr = csi2if_get_reg(info, CSI2IF_CSI_CTRL);

	ctrl.value = read_reg(addr);
	ctrl.reg.csia_cdphy_source = info->phy_id;
	switch (info->phy_workmode) {
	case IDI_PHY_WORKMODE_SINGLE:
		ctrl.reg.csia_cdphy_sensor_connection = 0;
		break;
	case IDI_PHY_WORKMODE_DUAL_SENSORA:
		ctrl.reg.csia_cdphy_sensor_connection = 1;
		ctrl.reg.csia_sensor = 0;
		break;
	case IDI_PHY_WORKMODE_DUAL_SENSORB:
		ctrl.reg.csia_cdphy_sensor_connection = 1;
		ctrl.reg.csia_sensor = 1;
		break;
	default:
		pr_err("invalid phy_workmode for csi ctrl");
		break;
	}
	pr_info("ctrl connected to phy:%u with workmode:%u",
		info->phy_id, info->phy_workmode);
	write_reg(ctrl.value, addr);
}

static inline void phy_reset_shutdown(struct csi_phy_info *info)
{
	SOC_CSI_WRAPPER_CDPHY_A_SHUTDOWN_RST_CTRL_UNION ctrl;
	void __iomem *addr = csi2if_get_reg(info, CSI2IF_PHY_RST_CTRL);

	ctrl.value = read_reg(addr);
	ctrl.reg.phya_rstz = 0;
	ctrl.reg.phya_shutdownz = 0;
	write_reg(ctrl.value, addr);
}

static inline void phy_disreset(struct csi_phy_info *info)
{
	SOC_CSI_WRAPPER_CDPHY_A_SHUTDOWN_RST_CTRL_UNION ctrl;
	void __iomem *addr = csi2if_get_reg(info, CSI2IF_PHY_RST_CTRL);

	ctrl.value = read_reg(addr);
	ctrl.reg.phya_rstz = 1;
	ctrl.reg.phya_shutdownz = 1;
	write_reg(ctrl.value, addr);
}

static void phy_ctrl_init(struct csi_phy_info *info)
{
	SOC_CSI_WRAPPER_CDPHY_A_CTRL_UNION ctrl;
	void __iomem *addr = csi2if_get_reg(info, CSI2IF_PHY_CTRL);

	ctrl.value = read_reg(addr);

	ctrl.reg.phya_enableclk_0 = 1;
	if (info->phy_workmode != IDI_PHY_WORKMODE_SINGLE) {
		ctrl.reg.phya_enableclk_1 = 1;
		ctrl.reg.phya_lane1_clk_en = 1;
	} else {
		ctrl.reg.phya_enableclk_1 = 0;
		ctrl.reg.phya_lane1_clk_en = 0;
	}

	ctrl.reg.phya_enable_0 = 1;
	ctrl.reg.phya_enable_1 = 1;
	ctrl.reg.phya_enable_2 = 1;
	ctrl.reg.phya_enable_3 = 1;

	ctrl.reg.phya_cdphy_rx_sel = info->phy_mode == IDI_PHY_MODE_CPHY ? 1 : 0;
	if (info->phy_mode == IDI_PHY_MODE_DPHY) {
		ctrl.reg.phya_rxdatawidthhs_0_dphy = 0;
		ctrl.reg.phya_rxdatawidthhs_1_dphy = 0;
	} else {
		ctrl.reg.phya_rxdatawidthhs_0_cphy = 1;
		ctrl.reg.phya_rxdatawidthhs_1_cphy = 1;
		ctrl.reg.phya_rxdatawidthhs_2_cphy = 1;
	}

	write_reg(ctrl.value, addr);
}

/**
 * accord to phy info, do phy init:
 * cphy or dphy...
 **/
static int phy_dump_reg_status(struct csi_phy_info *info)
{
	#define MAX_PHY_REGS 0xFF
	uint32_t i;
	uint8_t arr[MAX_PHY_REGS + 1];
	SOC_CSI_WRAPPER_CDPHY_A_TEST_CTRL_UNION ctrl;
	void __iomem *addr = csi2if_get_reg(info, CSI2IF_PHY_TEST_CTRL);

	SOC_CSI_WRAPPER_CDPHY_A_TEST_DOUT_UNION dout;
	void __iomem *read_addr = csi2if_get_reg(info, CSI2IF_PHY_TEST_DOUT);

	if (!addr || !read_addr)
		return -EINVAL;

	for (i = 0; i <= MAX_PHY_REGS; ++i) {
		ctrl.value = 0;
		ctrl.reg.phya_testdin = i;
		ctrl.reg.phya_testen = 1; // 1: addr
		ctrl.reg.phya_testclk = 1;
		ctrl.reg.bitmasken = 0x5ff;
		write_reg(ctrl.value, addr);
		ctrl.reg.phya_testclk = 0; // 1->0 latch addr
		write_reg(ctrl.value, addr);

		dout.value = read_reg(read_addr);
		arr[i] = dout.reg.phya_testdout;
	}

	print_hex_dump(KERN_INFO, "PHYStatus: ", DUMP_PREFIX_OFFSET,
		16, 1, arr, ARRAY_SIZE(arr), false);
	return 0;
}

static int phy_dump_rx_state(struct csi_phy_info *info)
{
	uint32_t phy_rx, phy_state;

	phy_rx = read_reg(SOC_CSI2IF_PHY_RX_ADDR(info->csi2if_base));
	phy_state = read_reg(SOC_CSI2IF_PHY_STOPSTATE_ADDR(info->csi2if_base));
	pr_info("CDPHY state: phy_rx = %#x, phy_state = %#x", phy_rx, phy_state);
	return 0;
}

static int phy_config(struct csi_phy_info *info)
{
	phy_reset_shutdown(info);
	phy_ctrl_init(info);
	phy_disreset(info);
	phy_dump_reg_status(info);
	phy_dump_rx_state(info);
	return 0;
}

static inline void csi_wrapper_csi_en_config(struct csi_phy_info *info, bool enable)
{
	SOC_CSI_WRAPPER_CSIA_CTRL_UNION ctrl;
	void __iomem *ctrl_addr = csi2if_get_reg(info, CSI2IF_CSI_CTRL);

	ctrl.value = read_reg(ctrl_addr);
	ctrl.reg.csia_en = enable ? 1 : 0;
	write_reg(ctrl.value, ctrl_addr);
}

static inline void csi_wrapper_csi_debug_config(struct csi_phy_info *info, bool enable)
{
	SOC_CSI_WRAPPER_CSI_DBG_UNION dbg;
	void __iomem *dbg_addr = SOC_CSI_WRAPPER_CSI_DBG_ADDR(info->wrapper_base);

	dbg.value = read_reg(dbg_addr);
	dbg.reg.csi_debug_en = enable ? 1 : 0;
	write_reg(dbg.value, dbg_addr);
}

static void csi_clk_enable_disreset(struct csi_phy_info *info)
{
	uint32_t bit, value;
	void __iomem *cgr_base = csi2if_get_reg(info, CSI2IF_CSI_CGR_RESET);
	void __iomem *cgren_addr = cgr_base + 0x4;
	void __iomem *reset_addr = cgr_base + 0xC;

	if (info->csi_index < 3) // csi a/b/c
		bit = BIT(info->csi_index);
	else
		bit = BIT(0);

	value = read_reg(cgren_addr);
	write_reg(value | bit, cgren_addr); // enable clk

	value = read_reg(reset_addr);
	write_reg(value & ~bit, reset_addr); // disreset

	csi_wrapper_csi_en_config(info, true); // enable csi
	csi_wrapper_csi_debug_config(info, true);
}

static void csi_clk_disable_reset(struct csi_phy_info *info)
{
	uint32_t bit, value;
	void __iomem *cgr_base = csi2if_get_reg(info, CSI2IF_CSI_CGR_RESET);
	void __iomem *cgrdis_addr = cgr_base + 0x8;
	void __iomem *reset_addr = cgr_base + 0xC;

	if (info->csi_index < 3) // csi a/b/c
		bit = BIT(info->csi_index);
	else
		bit = BIT(0);

	csi_wrapper_csi_en_config(info, false); // disable csi

	value = read_reg(reset_addr);
	write_reg(value | bit, reset_addr); // reset

	value = read_reg(cgrdis_addr);
	write_reg(value | bit, cgrdis_addr); // disable clk
}

static void csi_wrapper_dump_debug_status(struct csi_phy_info *info)
{
	uint32_t i, value;
	void __iomem *debug_addr = csi2if_get_reg(info, CSI2IF_CSI_DGB);
	void __iomem *addr = csi2if_get_reg(info, CSI2IF_PHY_CTRL);

	value = read_reg(debug_addr);
	pr_info("csi:%u phy:%u status:%#x", info->csi_index, info->phy_id, value);

	for (i = 0; i < MAX_CSI_COUNT; i++) {
		value = read_reg(SOC_CSI_WRAPPER_CSIA_CTRL_ADDR(info->wrapper_base) + i * 4);
		pr_info("csi:%d ctrls:%#x", 'A' + i, value);

		value = read_reg(SOC_CSI_WRAPPER_CSIA_DBG_ADDR(info->wrapper_base) + i * 4);
		pr_info("csi:%d debug:%#x", 'A' + i, value);
	}

	value = read_reg(addr);
	pr_info("phy:%u ctrl:%#x", info->phy_id, value);
	for (i = 0; i < MAX_PHY_COUNT; i++) {
		value = read_reg(SOC_CSI_WRAPPER_CDPHY_A_CTRL_ADDR(info->wrapper_base) + i * 4);
		pr_info("other phy:%u ctrl:%#x", 'A' + i, value);
	}
}

void plat_dump_csi_status(struct csi_phy_info *info)
{
	if (!info)
		return;
	uint32_t value;
	void __iomem *dbg_addr = SOC_CSI_WRAPPER_CSI_DBG_ADDR(info->wrapper_base);

	value = read_reg(dbg_addr);
	pr_info("csi:%u phy:%u dgb status:%#x", info->csi_index, info->phy_id, value);
	csi_wrapper_dump_debug_status(info);
}

static inline void csi_reset(struct csi_phy_info *info)
{
	SOC_CSI2IF_CSI2_RESETN_UNION resetn;
	void __iomem *addr = SOC_CSI2IF_CSI2_RESETN_ADDR(info->csi2if_base);

	resetn.value = read_reg(addr);
	resetn.reg.csi2_resetn = 0;
	write_reg(resetn.value, addr);

	pr_info("csi:%u reset", info->csi_index);
}

static inline void csi_disreset(struct csi_phy_info *info)
{
	SOC_CSI2IF_CSI2_RESETN_UNION resetn;
	void __iomem *addr = SOC_CSI2IF_CSI2_RESETN_ADDR(info->csi2if_base);

	resetn.value = read_reg(addr);
	resetn.reg.csi2_resetn = 1;
	write_reg(resetn.value, addr);

	pr_info("csi:%u disreset", info->csi_index);
}

static inline void csi_set_lanes(struct csi_phy_info *info)
{
	SOC_CSI2IF_N_LANES_UNION nlanes;
	void __iomem *addr = SOC_CSI2IF_N_LANES_ADDR(info->csi2if_base);

	nlanes.value = read_reg(addr);
	nlanes.reg.n_lanes = 3;  // 4 lanes
	write_reg(nlanes.value, addr);

	pr_info("csi nlanes:%u", nlanes.reg.n_lanes + 1);
}

static inline void csi_cfg_set(struct csi_phy_info *info)
{
	SOC_CSI2IF_CSI_CFG_UNION cfg;
	void __iomem *addr = SOC_CSI2IF_CSI_CFG_ADDR(info->csi2if_base);

	cfg.value = read_reg(addr);
	cfg.reg.phy_mode = info->phy_mode == IDI_PHY_MODE_DPHY ? 0 : 1;
	cfg.reg.cphy_data_width = 0;
	cfg.reg.debug_cnt_en = 1; // ADD NEW DEBUG
	cfg.reg.legacy_yuv_8b_swap_disable = 1;   // u y reverse
	write_reg(cfg.value, addr);
}

static void csi_config(struct csi_phy_info *info)
{
	csi_reset(info);
	csi_disreset(info);

	csi_set_lanes(info);
	csi_cfg_set(info);
}

static int info_invalid(struct csi_phy_info *info)
{
	if (!info) {
       pr_err("info is nullptr");
       return -EINVAL;
	}
	if (info->csi_index > MAX_CSI_COUNT ||
		info->phy_id > MAX_PHY_COUNT ||
		info->phy_mode >= IDI_PHY_MODE_MAX ||
		info->phy_workmode >= IDI_PHY_WORKMODE_MAX) {
		pr_err("csi_index:%u, phy_id:%u, phy_mode:%u, phy_workmode:%u",
			info->csi_index, info->phy_id,
			info->phy_mode, info->phy_workmode);
		return -EINVAL;
	}

	return 0;
}

static void dump_debug_info(void __iomem *csi_base)
{
	uint32_t i, value;
	for (i = 0; i < ARRAY_SIZE(csi2if_debug_offset); i++) {
		value = read_reg(csi_base + csi2if_debug_offset[i]);
		pr_info("csi2if: debug offset:%#x, val:%#010x", csi2if_debug_offset[i], value);
	}
}

void plat_csi2if_dump_debug_info(struct csi_phy_info *info)
{
	if (!info)
		return;

	uint32_t i;
	for (i = 0; i < ARRAY_SIZE(csi2if_irq_regs); ++i)
		dump_irq_reg_info(info->csi2if_base, &csi2if_irq_regs[i]);
	dump_debug_info(info->csi2if_base);
}

void plat_csi2if_irq_handler(int irq, void *dev_id)
{
	if (!dev_id)
		return;

	struct csi_phy_info *info = dev_id;
	pr_info("idi2axi: got here for csi2if interrupt");
	for (uint32_t i = 0; i < ARRAY_SIZE(csi2if_irq_regs); ++i)
		dump_irq_reg_info(info->csi2if_base, &csi2if_irq_regs[i]);
}


int plat_csi2if_enable(struct csi_phy_info *info)
{
	uint32_t intmask = 0;

	if (info_invalid(info))
		return -EINVAL;

	csi_clk_enable_disreset(info);
	csi_config(info);
	phy_config(info);
	csi_phy_connect(info);
	if (csi_int_enable()) {
		intmask = 0xffffffff;
	}
	csi2if_int_mask_set(info->csi2if_base, intmask);
	pr_info("plat_csi2if_enable finish");
	return 0;
}

int plat_csi2if_disable(struct csi_phy_info *info)
{
	if (info_invalid(info))
		return -EINVAL;

	csi2if_int_mask_set(info->csi2if_base, 0);
	phy_reset_shutdown(info);
	csi_reset(info);
	csi_clk_disable_reset(info);
	return 0;
}
