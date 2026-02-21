/*
 * asp_dma.c
 *
 * asp dma driver
 *
 * Copyright (c) 2015-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <platform_include/audio/platform_drivers/da_combine/asp_dma.h>

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/of_device.h>
#include <linux/hwspinlock.h>
#include <linux/pm_wakeup.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include "audio_log.h"

#define LOG_TAG "asp_dma"

#define HWLOCK_WAIT_TIME 50
#define ASP_DMA_HWLOCK_ID 6
#define ASP_DMA_STOP_RETRY_TIMES 40
#define REG_BIT_CLR 0
#define REG_BIT_SET 1

#ifndef unused_parameter
#define unused_parameter(x) (void)(x)
#endif

enum {
	IRQ_ERR1 = 0,
	IRQ_ERR2,
	IRQ_ERR3,
	IRQ_TC1,
	IRQ_TC2,
	IRQ_TYPE_BUTT
};

struct dma_callback {
	callback_t callback;
	unsigned long para;
};

struct dma_priv {
	struct device *dev;
	int irq[MULTI_DMA_MAX_CNT];
	spinlock_t lock;
	struct resource *res[MULTI_DMA_MAX_CNT];
	struct hwspinlock *hwlock;
	struct wakeup_source *wake_lock;
	void __iomem *reg_base_addr[MULTI_DMA_MAX_CNT];
	struct dma_callback callback[ASP_DMA_MAX_CHANNEL_NUM];
	unsigned int dma_cnt;
};

static struct dma_priv *g_dma_priv;

static unsigned int read_dma_reg(unsigned int reg, unsigned int dma_id)
{
	struct dma_priv *priv = g_dma_priv;
	unsigned long flag = 0;
	unsigned int ret;

	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return 0;
	}

	if (priv->reg_base_addr[dma_id] == NULL) {
		AUDIO_LOGE("reg_base_addr is null");
		return 0;
	}

	if (hwspin_lock_timeout_irqsave(priv->hwlock,
		HWLOCK_WAIT_TIME, &flag) != 0) {
		AUDIO_LOGE("hwspinlock timeout");
		return 0;
	}

	ret = readl(priv->reg_base_addr[dma_id] + reg);

	hwspin_unlock_irqrestore(priv->hwlock, &flag);

	return ret;
}

unsigned int dmac_reg_read(unsigned int reg)
{
	return read_dma_reg(reg, SINGLE_DMA_DEFAULT);
}

static void dmac_reg_write(unsigned int reg, unsigned int value, unsigned int dma_id)
{
	struct dma_priv *priv = g_dma_priv;
	unsigned long flag = 0;

	if (priv->reg_base_addr[dma_id] == NULL) {
		AUDIO_LOGE("reg_base_addr is null");
		return;
	}

	if (hwspin_lock_timeout_irqsave(priv->hwlock,
		HWLOCK_WAIT_TIME, &flag) != 0) {
		AUDIO_LOGE("hwspinlock timeout");
		return;
	}

	writel(value, priv->reg_base_addr[dma_id] + reg);

	hwspin_unlock_irqrestore(priv->hwlock, &flag);
}

static void dmac_reg_write_bit(unsigned int reg, unsigned int offset,
	unsigned int event, unsigned int dma_id)
{
	struct dma_priv *priv = g_dma_priv;
	unsigned int value;
	unsigned long flag_hw = 0;
	unsigned long flag_sft = 0;

	if (priv->reg_base_addr[dma_id] == NULL) {
		AUDIO_LOGE("reg_base_addr is null");
		return;
	}

	if (hwspin_lock_timeout_irqsave(priv->hwlock,
		HWLOCK_WAIT_TIME, &flag_hw) != 0) {
		AUDIO_LOGE("hwspinlock timeout");
		return;
	}

	spin_lock_irqsave(&priv->lock, flag_sft);

	value = readl(priv->reg_base_addr[dma_id] + reg);

	if (event == REG_BIT_CLR)
		value &= ~(BIT(offset));
	else
		value |= BIT(offset);

	writel(value, priv->reg_base_addr[dma_id] + reg);

	spin_unlock_irqrestore(&priv->lock, flag_sft);

	hwspin_unlock_irqrestore(priv->hwlock, &flag_hw);
}

static void dmac_dump(unsigned int channel, unsigned int dma_id)
{
	unused_parameter(channel);
	unused_parameter(dma_id);
	AUDIO_LOGD("a count: %u", read_dma_reg(ASP_DMA_CX_CNT0(channel), dma_id));
	AUDIO_LOGD("src addr: %u", read_dma_reg(ASP_DMA_CX_SRC_ADDR(channel), dma_id));
	AUDIO_LOGD("des addr: %u", read_dma_reg(ASP_DMA_CX_DES_ADDR(channel), dma_id));
	AUDIO_LOGD("lli: 0x%x", read_dma_reg(ASP_DMA_CX_LLI(channel), dma_id));
	AUDIO_LOGD("config: %u", read_dma_reg(ASP_DMA_CX_CONFIG(channel), dma_id));

	AUDIO_LOGD("c count: %u", read_dma_reg(ASP_DMA_CX_CNT1(channel), dma_id));
	AUDIO_LOGD("b index: %u", read_dma_reg(ASP_DMA_CX_BINDX(channel), dma_id));
	AUDIO_LOGD("c index: %u", read_dma_reg(ASP_DMA_CX_CINDX(channel), dma_id));
}

static irqreturn_t irq_handler(unsigned int dma_id)
{
	struct dma_priv *priv = g_dma_priv;
	unsigned int mask;
	unsigned int type;
	unsigned int state;
	unsigned int i;
	unsigned int val[IRQ_TYPE_BUTT] = {0};

	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return IRQ_HANDLED;
	}

	state = read_dma_reg(ASP_DMA_INT_STAT_AP, dma_id);
	if (state == 0)
		return IRQ_HANDLED;

	val[IRQ_ERR1] = read_dma_reg(ASP_DMA_INT_ERR1_AP, dma_id);
	val[IRQ_ERR2] = read_dma_reg(ASP_DMA_INT_ERR2_AP, dma_id);
	val[IRQ_ERR3] = read_dma_reg(ASP_DMA_INT_ERR3_AP, dma_id);
	val[IRQ_TC1] = read_dma_reg(ASP_DMA_INT_TC1_AP, dma_id);
	val[IRQ_TC2] = read_dma_reg(ASP_DMA_INT_TC2_AP, dma_id);

	/* clr interupt states */
	dmac_reg_write(ASP_DMA_INT_TC1_RAW, state, dma_id);
	dmac_reg_write(ASP_DMA_INT_TC2_RAW, state, dma_id);
	dmac_reg_write(ASP_DMA_INT_ERR1_RAW, state, dma_id);
	dmac_reg_write(ASP_DMA_INT_ERR2_RAW, state, dma_id);
	dmac_reg_write(ASP_DMA_INT_ERR3_RAW, state, dma_id);

	for (i = 0; i < ASP_DMA_MAX_CHANNEL_NUM; i++) {
		mask = BIT(i);
		if ((state & mask) == 0)
			continue;

		if (priv->callback[i].callback == NULL)
			continue;

		if (val[IRQ_ERR1] & mask)
			type = ASP_DMA_INT_TYPE_ERR1;
		else if (val[IRQ_ERR2] & mask)
			type = ASP_DMA_INT_TYPE_ERR2;
		else if (val[IRQ_ERR3] & mask)
			type = ASP_DMA_INT_TYPE_ERR3;
		else if (val[IRQ_TC1] & mask)
			type = ASP_DMA_INT_TYPE_TC1;
		else if (val[IRQ_TC2] & mask)
			type = ASP_DMA_INT_TYPE_TC2;
		else
			type = ASP_DMA_INT_TYPE_BUTT;

		priv->callback[i].callback(type, priv->callback[i].para, i);
	}

	return IRQ_HANDLED;
}

static irqreturn_t irq0_handler(int irq, void *data)
{
	unused_parameter(irq);
	unused_parameter(data);
	return irq_handler(MULTI_DMA_SOC);
}

static irqreturn_t irq1_handler(int irq, void *data)
{
	unused_parameter(irq);
	unused_parameter(data);
	return irq_handler(MULTI_DMA_CODEC);
}

static unsigned int get_dmac_id(unsigned short channel)
{
	if (g_dma_priv == NULL) {
		AUDIO_LOGE("g_dma_priv is null");
		return SINGLE_DMA_DEFAULT;
	}

	if (g_dma_priv->dma_cnt < MULTI_DMA_MAX_CNT) {
		return SINGLE_DMA_DEFAULT;
	}
	if (channel < ASP_DMA_MAX_CHANNEL_NUM) {
		return MULTI_DMA_SOC;
	}
	return MULTI_DMA_CODEC;
}

static unsigned short get_dma_channel(unsigned short channel)
{
	return (channel < ASP_DMA_MAX_CHANNEL_NUM)? channel : channel - ASP_DMA_MAX_CHANNEL_NUM;
}

unsigned int asp_dma_get_des(unsigned short channel)
{
	unsigned int dma_id;
	unsigned int addr;
	if (g_dma_priv == NULL) {
		AUDIO_LOGE("asp dma priv is null");
		return 0;
	}

	dma_id = get_dmac_id(channel);
	channel = get_dma_channel(channel);
	if (channel >= ASP_DMA_MAX_CHANNEL_NUM) {
		AUDIO_LOGE("channel %hu is err", channel);
		return 0;
	}
	addr = read_dma_reg(ASP_DMA_CX_DES_ADDR(channel), dma_id);
	return addr;
}

unsigned int asp_dma_get_src(unsigned short channel)
{
	unsigned int dma_id;
	if (g_dma_priv == NULL) {
		AUDIO_LOGE("asp dma priv is null");
		return 0;
	}

	dma_id = get_dmac_id(channel);
	channel = get_dma_channel(channel);
	if (channel >= ASP_DMA_MAX_CHANNEL_NUM) {
		AUDIO_LOGE("channel %hu is err", channel);
		return 0;
	}
	return read_dma_reg(ASP_DMA_CX_SRC_ADDR(channel), dma_id);
}

static int check_dma_para(const struct dma_priv *priv,
	unsigned short channel, const struct dma_lli_cfg *lli_cfg)
{
	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return -EINVAL;
	}

	if (channel >= ASP_DMA_MAX_CHANNEL_NUM) {
		AUDIO_LOGE("channel %hu is err", channel);
		return -EINVAL;
	}

	if (lli_cfg == NULL) {
		AUDIO_LOGE("lli cfg is null");
		return -EINVAL;
	}

	return 0;
}

int asp_dma_config(unsigned short channel, const struct dma_lli_cfg *lli_cfg,
	callback_t callback, unsigned long para)
{
	unsigned int channel_mask;
	struct dma_priv *priv = g_dma_priv;

	unsigned int dma_id = get_dmac_id(channel);
	channel = get_dma_channel(channel);

	channel_mask = BIT(channel);

	if (check_dma_para(priv, channel, lli_cfg) != 0)
		return -EINVAL;

	/* disable dma channel */
	dmac_reg_write_bit(ASP_DMA_CX_CONFIG(channel), 0, REG_BIT_CLR, dma_id);
	dmac_reg_write(ASP_DMA_CX_CNT0(channel), lli_cfg->a_count, dma_id);

	/* c count */
	dmac_reg_write(ASP_DMA_CX_CNT1(channel), 0, dma_id);
	dmac_reg_write(ASP_DMA_CX_BINDX(channel), 0, dma_id);
	dmac_reg_write(ASP_DMA_CX_CINDX(channel), 0, dma_id);

	/* set dma src/des addr */
	dmac_reg_write(ASP_DMA_CX_SRC_ADDR(channel), lli_cfg->src_addr, dma_id);
	dmac_reg_write(ASP_DMA_CX_DES_ADDR(channel), lli_cfg->des_addr, dma_id);

	/* set dma lli config */
	dmac_reg_write(ASP_DMA_CX_LLI(channel), lli_cfg->lli, dma_id);

	/* clr irq status of dma channel */
	dmac_reg_write(ASP_DMA_INT_TC1_RAW, channel_mask, dma_id);
	dmac_reg_write(ASP_DMA_INT_TC2_RAW, channel_mask, dma_id);
	dmac_reg_write(ASP_DMA_INT_ERR1_RAW, channel_mask, dma_id);
	dmac_reg_write(ASP_DMA_INT_ERR2_RAW, channel_mask, dma_id);
	dmac_reg_write(ASP_DMA_INT_ERR3_RAW, channel_mask, dma_id);

	if (callback != NULL) {
		priv->callback[channel].callback = callback;
		priv->callback[channel].para = para;

		/* release irq mask */
		dmac_reg_write_bit(ASP_DMA_INT_ERR1_MASK_AP, channel, REG_BIT_SET, dma_id);
		dmac_reg_write_bit(ASP_DMA_INT_ERR2_MASK_AP, channel, REG_BIT_SET, dma_id);
		dmac_reg_write_bit(ASP_DMA_INT_ERR3_MASK_AP, channel, REG_BIT_SET, dma_id);
		dmac_reg_write_bit(ASP_DMA_INT_TC1_MASK_AP, channel, REG_BIT_SET, dma_id);
		dmac_reg_write_bit(ASP_DMA_INT_TC2_MASK_AP, channel, REG_BIT_SET, dma_id);
	}

	AUDIO_LOGI("dma config succ");

	return 0;
}

int asp_dma_start(unsigned short channel, const struct dma_lli_cfg *lli_cfg)
{
	unsigned int lli_reg;

	unsigned int dma_id = get_dmac_id(channel);
	channel = get_dma_channel(channel);
	if (check_dma_para(g_dma_priv, channel, lli_cfg) != 0)
		return -EINVAL;

	lli_reg = read_dma_reg(ASP_DMA_CX_LLI(channel), dma_id);
	if (lli_reg != lli_cfg->lli) {
		AUDIO_LOGE("lli is changed, lli reg: %u, lli cfg: %u",
			lli_reg, lli_cfg->lli);
		return -EINVAL;
	}

	dmac_reg_write(ASP_DMA_CX_CONFIG(channel), lli_cfg->config, dma_id);

	dmac_dump(channel, dma_id);

	AUDIO_LOGI("dma start succ");

	return 0;
}

void asp_dma_suspend(unsigned short channel)
{
	unsigned int mask;
	unsigned int dma_id = get_dmac_id(channel);
	channel = get_dma_channel(channel);

	mask = BIT(channel);
	unsigned int i = ASP_DMA_STOP_RETRY_TIMES;
	unsigned int state;

	if (g_dma_priv == NULL) {
		AUDIO_LOGE("asp dma priv is null");
		return;
	}

	if (channel >= ASP_DMA_MAX_CHANNEL_NUM) {
		AUDIO_LOGE("dma channel err: %hu", channel);
		return;
	}

	/* disable dma channel */
	dmac_reg_write_bit(ASP_DMA_CX_CONFIG(channel), 0, REG_BIT_CLR, dma_id);

	do {
		state = read_dma_reg(ASP_DMA_CH_STAT, dma_id) & mask;
		if (state == 0)
			break;

		AUDIO_LOGI("stopping dma channel: %hu", channel);
		udelay(250);
	} while (--i);

	if (i == 0) {
		AUDIO_LOGE("dma channel: %hu suspend fail, channel state: %u",
			channel, state);
		return;
	}

	AUDIO_LOGI("dma channel: %hu suspend succ", channel);
}

void asp_dma_stop(unsigned short channel)
{
	unsigned int channel_mask;
	struct dma_priv *priv = g_dma_priv;
	
	unsigned int dma_id = get_dmac_id(channel);
	channel = get_dma_channel(channel);

	channel_mask = BIT(channel);
	unsigned int i = ASP_DMA_STOP_RETRY_TIMES;
	unsigned int state;

	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return;
	}

	if (channel >= ASP_DMA_MAX_CHANNEL_NUM) {
		AUDIO_LOGE("channel err: %hu", channel);
		return;
	}

	/* disable dma channel */
	dmac_reg_write_bit(ASP_DMA_CX_CONFIG(channel), 0, REG_BIT_CLR, dma_id);

	do {
		state = read_dma_reg(ASP_DMA_CH_STAT, dma_id) & channel_mask;
		if (state == 0)
			break;

		AUDIO_LOGI("stopping channel: %hu", channel);
		udelay(250);
	} while (--i);

	dmac_reg_write_bit(ASP_DMA_INT_ERR1_MASK_AP, channel, REG_BIT_CLR, dma_id);
	dmac_reg_write_bit(ASP_DMA_INT_ERR2_MASK_AP, channel, REG_BIT_CLR, dma_id);
	dmac_reg_write_bit(ASP_DMA_INT_ERR3_MASK_AP, channel, REG_BIT_CLR, dma_id);
	dmac_reg_write_bit(ASP_DMA_INT_TC1_MASK_AP, channel, REG_BIT_CLR, dma_id);
	dmac_reg_write_bit(ASP_DMA_INT_TC2_MASK_AP, channel, REG_BIT_CLR, dma_id);

	memset(&priv->callback[channel], 0, sizeof(priv->callback[channel]));

	if (i == 0) {
		AUDIO_LOGE("channel: %hu stop fail, channel state: %u",
			channel, state);
		return;
	}

	AUDIO_LOGI("channel: %hu stop succ", channel);
}

static int remap_base_addr(struct platform_device *pdev,
	struct dma_priv *priv, struct device *dev)
{
	unsigned int i;
	priv->dma_cnt = 1;

	if (of_property_read_bool(dev->of_node, "multi_dma")) {
		int ret;
		ret = of_property_read_u32(pdev->dev.of_node, "reg_count", &priv->dma_cnt);
		if(ret != 0)
			return -EINVAL;
		AUDIO_LOGI("get reg_count: %d", priv->dma_cnt);
	}

	if (priv->dma_cnt > MULTI_DMA_MAX_CNT) {
		AUDIO_LOGE("dma_cnt is invalid, dma_cnt: %u", priv->dma_cnt);
		return -EINVAL;
	}

	for (i = 0; i < priv->dma_cnt; i++) {
		priv->res[i] = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (priv->res[i] == NULL) {
			AUDIO_LOGE("get resource failed");
			return -ENOENT;
		}

		if (!devm_request_mem_region(dev, priv->res[i]->start,
			resource_size(priv->res[i]), "asp dma")) {
			AUDIO_LOGE("request mem region failed");
			return -ENOMEM;
		}

		priv->reg_base_addr[i] =
			devm_ioremap(dev, priv->res[i]->start, resource_size(priv->res[i]));
		if (priv->reg_base_addr[i] == NULL) {
			AUDIO_LOGE("asp dma reg addr ioremap failed");
			return -ENOMEM;
		}
	}
	return 0;
}

static int dma_irq_init(struct platform_device *pdev,
	struct dma_priv *priv, struct device *dev)
{
	int ret;
	unsigned int i;
	if (priv == NULL || dev == NULL) {
		AUDIO_LOGE("dma_priv or dev is null");
		return -ENOENT;
	}
	const char* dma_irq_names[MULTI_DMA_MAX_CNT] = {0};
	irq_handler_t dma_irq_handlers[MULTI_DMA_MAX_CNT]={irq0_handler, irq1_handler};

	if (of_property_read_bool(dev->of_node, "multi_dma")) {
		dma_irq_names[0] = "asp_dma0_irq";
		dma_irq_names[1] = "asp_dma1_irq";
	} else {
		dma_irq_names[0] = "asp_dma_irq";
	}

	for (i = 0; i < priv->dma_cnt; i++) {
		priv->irq[i] = platform_get_irq_byname(pdev, dma_irq_names[i]);
		if (priv->irq[i] < 0) {
			AUDIO_LOGE("get asp dma irq failed %d", priv->irq[i]);
			return -ENOENT;
		}

		if (of_property_read_bool(dev->of_node, "oneshot_irq")) {
			AUDIO_LOGI("oneshot irq mode enable");
			ret = request_threaded_irq((unsigned int)(priv->irq[i]), NULL, dma_irq_handlers[i],
				IRQF_NO_SUSPEND | IRQF_ONESHOT, dma_irq_names[i], dev);
		} else {
			AUDIO_LOGI("shared irq mode enable");
			ret = request_irq(priv->irq[i], dma_irq_handlers[i],
				IRQF_SHARED | IRQF_NO_SUSPEND, dma_irq_names[i], dev);
		}

		if (ret != 0) {
			AUDIO_LOGE("request asp dma irq failed");
			return -ENOENT;
		}
	}

	return 0;
}

static void unremap_base_addr(struct dma_priv *priv)
{
	unsigned int i;
	if (priv->dma_cnt > MULTI_DMA_MAX_CNT) {
		AUDIO_LOGE("dma_cnt is invalid, dma_cnt: %u", priv->dma_cnt);
		return;
	}
	for (i = 0; i < priv->dma_cnt; i++) {
		if (priv->reg_base_addr[i] != NULL) {
			devm_iounmap(priv->dev, priv->reg_base_addr[i]);
			priv->reg_base_addr[i] = NULL;
			devm_release_mem_region(priv->dev, priv->res[i]->start,
				resource_size(priv->res[i]));
		}
	}
}

static void free_dma_irq(struct dma_priv *priv)
{
	unsigned int i;
	if (priv->dma_cnt > MULTI_DMA_MAX_CNT) {
		AUDIO_LOGE("dma_cnt is invalid, dma_cnt: %u", priv->dma_cnt);
		return;
	}
	for (i = 0; i < priv->dma_cnt; i++) {
		free_irq(priv->irq[i], priv);
	}
}

static int asp_dma_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;
	struct dma_priv *priv = NULL;

	AUDIO_LOGI("enter");

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (priv == NULL) {
		AUDIO_LOGE("malloc failed");
		return -ENOMEM;
	}

	ret = remap_base_addr(pdev, priv, dev);
	if (ret != 0) {
		AUDIO_LOGE("remap base addr failed");
		goto remap_err;
	}

	ret = dma_irq_init(pdev, priv, dev);
	if (ret != 0) {
		AUDIO_LOGE("rdma irq init failed");
		goto irq_init_err;
	}

	priv->hwlock = hwspin_lock_request_specific(ASP_DMA_HWLOCK_ID);
	if (priv->hwlock == NULL) {
		AUDIO_LOGE("request hw spinlok failed");
		ret = -ENOENT;
		goto hwspin_lock_err;
	}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0))
	priv->wake_lock = wakeup_source_register(dev, "asp_dma");
#else
	priv->wake_lock = wakeup_source_register("asp_dma");
#endif
	if (priv->wake_lock == NULL) {
		AUDIO_LOGE("request hw spinlok failed");
		ret = -ENOMEM;
		goto wakeup_source_register_err;
	}
	spin_lock_init(&priv->lock);

	priv->dev = dev;

	platform_set_drvdata(pdev, priv);

	g_dma_priv = priv;

	AUDIO_LOGI("exit");

	return 0;

wakeup_source_register_err:
	hwspin_lock_free(priv->hwlock);
	priv->hwlock = NULL;
hwspin_lock_err:
	free_dma_irq(priv);
irq_init_err:
	unremap_base_addr(priv);
remap_err:
	AUDIO_LOGE("probe failed");

	return ret;
}

static int asp_dma_remove(struct platform_device *pdev)
{
	struct dma_priv *priv =
		(struct dma_priv *)platform_get_drvdata(pdev);

	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return 0;
	}

	wakeup_source_unregister(priv->wake_lock);

	if (hwspin_lock_free(priv->hwlock))
		AUDIO_LOGE("hwspinlock free failed");
	priv->hwlock = NULL;

	free_dma_irq(priv);

	unremap_base_addr(priv);

	g_dma_priv = NULL;

	return 0;
}

static const struct of_device_id g_dma_match_tbl[] = {
	{ .compatible = "hisilicon,da_combine-asp-dma", },
	{},
};

MODULE_DEVICE_TABLE(of, g_dma_match_tbl);

static struct platform_driver g_dma_driver = {
	.driver = {
		.name = "da_combine_asp_dma_drv",
		.owner = THIS_MODULE,
		.of_match_table = g_dma_match_tbl,
	},
	.probe = asp_dma_probe,
	.remove = asp_dma_remove,
};

static int __init asp_dma_init(void)
{
	return platform_driver_register(&g_dma_driver);
}
module_init(asp_dma_init);

static void __exit asp_dma_exit(void)
{
	platform_driver_unregister(&g_dma_driver);
}
module_exit(asp_dma_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("ASP DMA Driver");

