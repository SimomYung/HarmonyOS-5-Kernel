/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: platform dependent idi2axi interface implementations
 *
 * This file is released under the GPLv2.
 */
#include "plat_idi2axi.h"
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <soc_media2_crg_interface.h>
#include <soc_idi2axi_interface.h>
#include "idi2axi_internal.h"
#include "i2a_atf.h"
#include "plat_csi2if.h"
#include "i2a_debug.h"
#include "i2a_csi2if.h"
#include "reg_helper.h"
#include "cam_intf.h"
#include "idi2axi_intf.h"
#include "i2a_ffa.h"

#define DEFAULT_TIMEOUT 5000  // ms
#define IDI2AXI_CLK_CYCLES_PER_MS    104000  // idi freq: 104M

enum idi_reg_type {
	IDI_REG_ADDR_LOW,
	IDI_REG_ADDR_HIGH,
	IDI_REG_INT_EN,
	IDI_REG_INT_MSK,
	IDI_REG_INT_CLR,
	IDI_REG_MAX,
};

static uint32_t reg_type_offset[IDI_REG_MAX][I2A_MAX_CHANNELS] = {
	[IDI_REG_ADDR_LOW] = {
		SOC_IDI2AXI_WR_START_ADDR0_LOW_ADDR(0),
		SOC_IDI2AXI_WR_START_ADDR1_LOW_ADDR(0),
		SOC_IDI2AXI_WR_START_ADDR2_LOW_ADDR(0),
		SOC_IDI2AXI_WR_START_ADDR3_LOW_ADDR(0),
	},
	[IDI_REG_ADDR_HIGH] = {
		SOC_IDI2AXI_WR_START_ADDR0_HIGH_ADDR(0),
		SOC_IDI2AXI_WR_START_ADDR1_HIGH_ADDR(0),
		SOC_IDI2AXI_WR_START_ADDR2_HIGH_ADDR(0),
		SOC_IDI2AXI_WR_START_ADDR3_HIGH_ADDR(0),
	},
	[IDI_REG_INT_EN] = {
		SOC_IDI2AXI_INT_EN_ADDR(0),
		SOC_IDI2AXI_INT_EN_ADDR(0),
		SOC_IDI2AXI_INT_EN_ADDR(0),
		SOC_IDI2AXI_INT_EN_ADDR(0),
	},
	[IDI_REG_INT_MSK] = {
		SOC_IDI2AXI_INT_MSK_ADDR(0),
		SOC_IDI2AXI_INT_MSK_ADDR(0),
		SOC_IDI2AXI_INT_MSK_ADDR(0),
		SOC_IDI2AXI_INT_MSK_ADDR(0),
	},
	[IDI_REG_INT_CLR] = {
		SOC_IDI2AXI_INT_CLR_ADDR(0),
		SOC_IDI2AXI_INT_CLR_ADDR(0),
		SOC_IDI2AXI_INT_CLR_ADDR(0),
		SOC_IDI2AXI_INT_CLR_ADDR(0),
	},
};

static int check_base_info(struct i2a_dev_base_t *i2a_base_info)
{
	if (!i2a_base_info || !i2a_base_info->dev || !i2a_base_info->i2a_base) {
		pr_err("%s input i2a_base_info is null", __func__);
		return -1;
	}
	return 0;
}

static int check_channel_param(struct idi2axi_channel *ich)
{
	if (!ich) {
		pr_err("%s input channel null", __func__);
		return -1;
	}
	return check_base_info(ich->i2a_base_info);
}

static void timeout_config(struct device *dev, void __iomem *i2a_base, uint32_t cfg_timeout)
{
	// should use channel vc(cid) to get inten addr if support 4 vc
	void __iomem *addr = SOC_IDI2AXI_TIMER_ADDR(i2a_base);
	uint32_t timeout, max_timeout;

	timeout = cfg_timeout ? cfg_timeout : DEFAULT_TIMEOUT;
	if (get_i2a_timeout())
		timeout = get_i2a_timeout();
	max_timeout = UINT_MAX / IDI2AXI_CLK_CYCLES_PER_MS;
	if (timeout > max_timeout) {
		dev_warn(dev, "timeout: %u exceeds max timeout: %u",
			timeout, max_timeout);
		timeout = max_timeout;
	}

	timeout *= IDI2AXI_CLK_CYCLES_PER_MS;
	timeout = 0xffffffff;
	write_reg(timeout, addr);
	dev_info(dev, "timeout config value:%#x", read_reg(addr));
}

int plat_idi2axi_disable(struct i2a_dev_base_t *i2a_base_info)
{
	if (check_base_info(i2a_base_info) != 0)
		return -1;
	return ffa_idi2axi_disable(i2a_base_info->id);
}

int plat_idi2axi_enable(struct i2a_dev_base_t *i2a_base_info)
{
	if (check_base_info(i2a_base_info) != 0)
		return -1;

	int ret = ffa_idi2axi_enable(i2a_base_info->id);
	if (ret) {
		pr_info("ffa_tzsp_idi2axi_enable fail");
		goto disreset_fail;
	}

	timeout_config(i2a_base_info->dev, i2a_base_info->i2a_base, i2a_base_info->timeout);
	pr_info("%s exit", __func__);
	return 0;

disreset_fail:
	plat_idi2axi_disable(i2a_base_info);
	return ret;
}

static void __iomem *idi_channel_reg(struct idi2axi_channel *ich, uint32_t reg_type)
{
	if (reg_type >= IDI_REG_MAX || ich->vc >= I2A_MAX_CHANNELS) {
		dev_err(ich->i2a_base_info->dev, "fail reg_type=%d ch_id=%d", __func__, reg_type);
		return NULL;
	}

	void __iomem *i2a_base = ich->i2a_base_info->i2a_base;
	if (!i2a_base) {
		dev_err(ich->i2a_base_info->dev, "%s i2a_base addr is null", __func__);
		return NULL;
	}
	return i2a_base + reg_type_offset[reg_type][ich->vc];
}

static int enable_channel_irq(struct idi2axi_channel *ich)
{
	SOC_IDI2AXI_INT_EN_UNION inten;
	// should use channel vc(cid) to get inten addr if support 4 vc
	void __iomem *addr = idi_channel_reg(ich, IDI_REG_INT_EN);
	if (!addr) {
		dev_err(ich->i2a_base_info->dev, "%s channel IDI_REG_INT_EN addr is null", __func__);
		return -1;
	}

	inten.value = read_reg(addr);
	
	pr_info("idi2axi_channel ich->vc = %d", ich->vc);
	if (ich->vc == 0) {
		inten.reg.trans_end_int_en = 1;
	}
	inten.reg.dt_error_int_en = 1;
	inten.reg.vc_error_int_en = 1;
	inten.reg.timeout_int_en = 1;
	inten.reg.axi_wr_error_int_en = 1;
	inten.reg.sfifo_overflow_error_int_en = 1;
	inten.reg.frame_cnt_error_int_en = 1;
	inten.reg.pslverr_int_en = 1;
	write_reg(inten.value, addr);
	return 0;
}

static void disable_channel_irq(struct idi2axi_channel *ich)
{
	SOC_IDI2AXI_INT_EN_UNION inten;
	// should use channel vc(cid) to get inten addr if support 4 vc
	void __iomem *addr = idi_channel_reg(ich, IDI_REG_INT_EN);
	if (!addr) {
		dev_err(ich->i2a_base_info->dev, "%s channel IDI_REG_INT_EN addr is null", __func__);
		return;
	}

	inten.value = read_reg(addr);
	if (ich->vc == 0) {
		inten.reg.trans_end_int_en = 0;
	}
	write_reg(inten.value, addr);
	dev_info(ich->i2a_base_info->dev, "disable channel[%u] irq", ich->vc);
}

void disable_channel_base_irq(struct i2a_dev_base_t *i2a_base_info)
{
	if (check_base_info(i2a_base_info) != 0)
		return;
	dev_info(i2a_base_info->dev, "disable other channel irq");
	void __iomem *addr = SOC_IDI2AXI_INT_EN_ADDR(i2a_base_info->i2a_base);
	write_reg(0, addr);
}

static void channel_enable(struct idi2axi_channel *ich)
{
	SOC_IDI2AXI_EN_UNION en;
	void __iomem *i2a_base = ich->i2a_base_info->i2a_base;
	void __iomem *addr = SOC_IDI2AXI_EN_ADDR(i2a_base);

	en.value = read_reg(addr);
	en.reg.idi2axi_en = 1;

	write_reg(en.value, addr);

	write_reg(en.value, addr);
	pr_info("i2a_id = %u  channel_enable[%d]", ich->i2a_base_info->id, ich->vc);
}

// when buffer pushed to idi2axi, enable that channel
void plat_idi2axi_push_buffer(struct idi2axi_channel *ich, uint64_t va)
{
	if (check_channel_param(ich) != 0)
		return;
	void __iomem *addr_low = idi_channel_reg(ich, IDI_REG_ADDR_LOW);
	void __iomem *addr_high = idi_channel_reg(ich, IDI_REG_ADDR_HIGH);
	uint32_t low = va & 0xFFFFFFFFu;
	uint32_t high = (va  >> 32u) & 0xFFFFFFFFu;

	write_reg(low, addr_low);
	write_reg(high, addr_high);
	channel_enable(ich);
	dev_info(ich->i2a_base_info->dev, "buffer: %#lx pushed to channel: %u", va, ich->vc);
}

uint64_t plat_idi2axi_pop_buffer(struct idi2axi_channel *ich)
{
	if (check_channel_param(ich) != 0)
		return 0;
	void __iomem *addr_low = idi_channel_reg(ich, IDI_REG_ADDR_LOW);
	void __iomem *addr_high = idi_channel_reg(ich, IDI_REG_ADDR_HIGH);
	uint32_t low, high;
	uint64_t va;

	low = read_reg(addr_low);
	high = read_reg(addr_high);

	va = ((uint64_t)high << 32u) | low;
	dev_info(ich->i2a_base_info->dev, "buffer: %#lx poped from channel: %u", va, ich->vc);
	return va;
}

static struct irq_bit_info idi2axi_irq_bit[] = {
	def_idi2axi_irq_bit(timeout_int_msk),
	def_idi2axi_irq_bit(dt_error_int_msk),
	def_idi2axi_irq_bit(vc_error_int_msk),
	def_idi2axi_irq_bit(axi_wr_error_int_msk),
	def_idi2axi_irq_bit(sfifo_overflow_error_int_msk),
	def_idi2axi_irq_bit(frame_cnt_error_int_msk),
	def_idi2axi_irq_bit(pslverr_int_msk),
};

static struct irq_reg_info idi2axi_irq_reg = {
	SOC_IDI2AXI_INT_MSK_ADDR(0), SOC_IDI2AXI_INT_CLR_ADDR(0),
	idi2axi_irq_bit, ARRAY_SIZE(idi2axi_irq_bit),
};

static void plat_common_irq_handler(struct i2a_dev_base_t *i2a_base_info)
{
	uint32_t i;
	struct irq_bit_info *bitinfo = idi2axi_irq_reg.irq_bit_info;
	uint32_t bitcount = idi2axi_irq_reg.irq_bit_count;
	uint32_t irq_value = read_reg(i2a_base_info->i2a_base + idi2axi_irq_reg.irq_stat_offset);
	uint32_t irq_bit_handled = 0;

	for (i = 0; i < bitcount; ++i) {
		if (bitinfo[i].bit_mask & irq_value) {
			if (log_i2a_status())
				dev_info(i2a_base_info->dev, "got irq: %s", bitinfo[i].bit_name);
			irq_bit_handled |= bitinfo[i].bit_mask;
		}
	}

	write_reg(irq_bit_handled, i2a_base_info->i2a_base + idi2axi_irq_reg.irq_clear_offset);
}

void plat_dump_status(struct i2a_dev_base_t *i2a_base_info)
{
	if (check_base_info(i2a_base_info) != 0)
		return;
	uint32_t i;
	void __iomem *i2a_base = i2a_base_info->i2a_base;

	for (i = 0; i <= 3; ++i)
		dev_info(i2a_base_info->dev, "stat %u: %#x", i,
			read_reg(i2a_base + SOC_IDI2AXI_STAT0_ADDR(0) + i * 4));
	for (; i <= 9; ++i)
		dev_info(i2a_base_info->dev, "stat %u: %#x", i,
			read_reg(i2a_base + SOC_IDI2AXI_STAT4_ADDR(0) + (i - 4) * 4));
}

/* plat_idi2axi_get_eof_channels - read interrupt masked register,
 * to find which channel comes eof

 * @idev: the idi2axi device to look for
 * Returns bit indexed mask, if bit0 is 1, then channel 0 has eof, etc.
 * at max four bit will be set, which means four channel eof come simultaneously.
 */
uint32_t plat_idi2axi_get_eof_channels(struct idi2axi_channel *ich)
{
	if (check_channel_param(ich) != 0)
		return 0;
	uint32_t msk = 0;
	unsigned int i2a_id = ich->i2a_base_info->id;
	SOC_IDI2AXI_INT_MSK_UNION intmsk;
	void __iomem *addr;
	plat_common_irq_handler(ich->i2a_base_info);
	addr = idi_channel_reg(ich, IDI_REG_INT_MSK);
	intmsk.value = read_reg(addr);
	if (intmsk.value != 0)
		pr_info("i2a_id = %u : irq mask value:%#x", i2a_id, intmsk.value);

	if (intmsk.reg.trans_end_int_msk) {
		pr_info("i2a_id = %u  vc0 irq", i2a_id);
		msk |= BIT(0);
	}

	addr = idi_channel_reg(ich, IDI_REG_INT_CLR);
	write_reg(intmsk.value, addr);
	return msk;
}

int plat_idi2axi_channel_working(struct idi2axi_channel *ich)
{
	if (check_channel_param(ich) != 0)
		return -1;
	SOC_IDI2AXI_EN_UNION en;
	void __iomem *i2a_base = ich->i2a_base_info->i2a_base;
	void __iomem *addr = SOC_IDI2AXI_EN_ADDR(i2a_base);

	en.value = read_reg(addr);
	return en.reg.idi2axi_en;
}

int plat_idi2axi_channel_config(struct idi2axi_channel *ich)
{
	if (check_channel_param(ich) != 0)
		return -1;
	struct device *dev = ich->i2a_base_info->dev;
	dev_info(dev, "%s: enter config channel[%u]", __func__, ich->vc);
	SOC_IDI2AXI_CFG_UNION cfg;
	void __iomem *i2a_base = ich->i2a_base_info->i2a_base;
	if (!i2a_base) {
		dev_err(dev, "%s i2a_base is null", __func__);
		return -1;
	}
	void __iomem *cfg_addr = SOC_IDI2AXI_CFG_ADDR(i2a_base);
	if (!cfg_addr) {
		dev_err(dev, "%s cfg_addr is null", __func__);
		return -1;
	}

	cfg.value = read_reg(cfg_addr);
	if (ich->vc == 0) {
		cfg.reg.vld_vc = ich->vc;
	}
	cfg.reg.vld_dt = ich->dt;
	cfg.reg.max_wr_outstanding = 0xF;
	write_reg(cfg.value, cfg_addr);
	read_reg(cfg_addr);
	return enable_channel_irq(ich);
}

void plat_idi2axi_channel_deconfig(struct idi2axi_channel *ich)
{
	if (check_channel_param(ich) != 0)
		return;
	disable_channel_irq(ich);
}
