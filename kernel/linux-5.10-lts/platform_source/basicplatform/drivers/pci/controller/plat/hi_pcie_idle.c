/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe Idle.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_idle.h"

#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <securec.h>
#include "../hi_pcie_external_api.h"
#include "../hi_pcie_defines.h"
#include "../core/hi_pcie_core.h"

#define  PCIE_SUPPORT_IDLE_FEATURE	"support_32k_idle"
#define  PCIE_IDLE_TIMEOUT_IRQ		"pcie_idle_timeout"
#define  PCIE_IDLE_WAKE_IRQ		"pcie_idle_wake"
#define  PCIE_IDLE_TIMEOUT_IRQ_1	"pcie_idle_timeout_1"
#define  PCIE_IDLE_WAKE_IRQ_1		"pcie_idle_wake_1"

static inline void idle_reg_writel(struct hi_pcie_idle *idle, u32 val, u32 reg)
{
	hi_pcie_pr_d("idle write reg: 0x%x val: 0x%x", reg, val);
	writel(val, idle->idle_reg_base + reg);
}

static inline uint32_t idle_reg_readl(struct hi_pcie_idle *idle, u32 reg)
{
	return readl(idle->idle_reg_base + reg);
}

static int hi_pcie_idle_dtsinfo_port_get(struct hi_pcie *pcie, struct hi_pcie_idle *idle, u32 port_id)
{
	char node_name[32] = {0};
	struct device_node *node;
	u32 val[3] = {0};

	(void)snprintf_s(node_name, sizeof(node_name), sizeof(node_name) - 1,
		"hi_pcie,port%u%u", pcie->ctrl_id, port_id);

	hi_pcie_pr_d("add port(%s) idle dtsinfo", node_name);

	node = of_find_compatible_node(pcie->dev->of_node, NULL, node_name);
	if (node == NULL) {
		hi_pcie_pr_e("get port(%s) node failed", node_name);
		return -EINVAL;
	}

	/* get dts info: 32k l1ss en bit && en bit masken && en addr */
	if (of_property_read_u32_array(node, "idle_l1ss_cnt_info", val, 3)) {
		hi_pcie_pr_e("idle l1ss cnt en info get failed");
		return -1;
	}
	idle->port[port_id].idle_l1ss_cnt_en = val[0];
	idle->port[port_id].idle_l1ss_cnt_en_bitmask = val[1];
	idle->port[port_id].idle_l1ss_cnt_en_addr = val[2];

	/* get dts info: timeout msk bit && msk bitmasken && msk addr */
	if (of_property_read_u32_array(node, "idle_timeout_mask_info", val, 3)) {
		hi_pcie_pr_e("idle timeout mask info get failed");
		return -1;
	}
	idle->port[port_id].idle_timeout_mask_bit = val[0];
	idle->port[port_id].idle_timeout_mask_bitmask = val[1];
	idle->port[port_id].idle_timeout_mask_addr = val[2];

	/* get dts info: timeout clr bit && clr bit masken && clr addr */
	if (of_property_read_u32_array(node, "idle_timeout_clr_info", val, 3)) {
		hi_pcie_pr_e("idle timeout clr info get failed");
		return -1;
	}
	idle->port[port_id].idle_timeout_clr_bit = val[0];
	idle->port[port_id].idle_timeout_clr_bitmask = val[1];
	idle->port[port_id].idle_timeout_clr_addr = val[2];

	/* get dts info: wake msk bit && msk bitmasken && sk_addr */
	if (of_property_read_u32_array(node, "idle_wake_mask_info", val, 3)) {
		hi_pcie_pr_e("idle wake mask info get failed");
		return -1;
	}
	idle->port[port_id].idle_wake_mask_bit = val[0];
	idle->port[port_id].idle_wake_mask_bitmask = val[1];
	idle->port[port_id].idle_wake_mask_addr = val[2];

	/* get dts info: wake clr bit && clr bitmasken && clr_addr */
	if (of_property_read_u32_array(node, "idle_wake_clr_info", val, 3)) {
		hi_pcie_pr_e("idle wake clr info get failed");
		return -1;
	}
	idle->port[port_id].idle_wake_clr_bit = val[0];
	idle->port[port_id].idle_wake_clr_bitmask = val[1];
	idle->port[port_id].idle_wake_clr_addr = val[2];

	/* get dts info: l1ss cnt time set val(if 0, then use default), l1ss cnt time reg addr */
	if (of_property_read_u32_array(node, "idle_l1ss_cnt_time_set", val, 2)) {
		hi_pcie_pr_e("idle l1ss cnt time set get failed");
		return -1;
	}
	idle->port[port_id].idle_l1ss_cnt_time_set = val[0];
	idle->port[port_id].idle_l1ss_cnt_time_set_addr = val[1];

	return 0;
}

static int hi_pcie_idle_dtsinfo_get(struct hi_pcie *pcie, struct hi_pcie_idle *idle)
{
	int ret;
	u32 i = 0;

	idle->idle_enable = 1;
	idle->host_id = pcie->ctrl_id;
	idle->port_num = pcie->port_num;

	for (i = 0; i < idle->port_num; i++) {
		hi_pcie_pr_d("add port(%u/%u) idle dtsinfo", pcie->ctrl_id, i);
		idle->port[i].id = i;
		idle->port[i].pcie = pcie;
		idle->port[i].pcie_port = pcie_core_port_get(idle->host_id, i);
		hi_pcie_pr_e("port(%u/%u) dev_ven_id 0x%x", pcie->ctrl_id, i, idle->port[i].pcie_port->dev_ven_id);
		ret = hi_pcie_idle_dtsinfo_port_get(pcie, idle, i);
		if (ret) {
			hi_pcie_pr_e("get port(%u/%u) idle dtsinfo fail", pcie->ctrl_id, i);
			return ret;
		}
	}

	return 0;
}

static void hi_pcie_idle_startup_port(struct hi_pcie_idle *idle, u32 port_id)
{
	u32 val;

	hi_pcie_pr_d("+%s+", __func__);

	if (idle->idle_enable == 0)
		goto SKIP_IDLE_SLEEP_INIT;

	/* enable idle l1ss cnt machine */
	val = idle_reg_readl(idle, idle->port[port_id].idle_l1ss_cnt_en_addr);
	if (idle->port[port_id].idle_l1ss_cnt_en_bitmask)
		val = val | BIT(idle->port[port_id].idle_l1ss_cnt_en) |
			BIT(idle->port[port_id].idle_l1ss_cnt_en_bitmask);
	else
		val = val | BIT(idle->port[port_id].idle_l1ss_cnt_en);
	idle_reg_writel(idle, val, idle->port[port_id].idle_l1ss_cnt_en_addr);

	/* enable pcie idle timeout msk interrupt */
	val = idle_reg_readl(idle, idle->port[port_id].idle_timeout_mask_addr);
	if (idle->port[port_id].idle_timeout_mask_bitmask)
		val = BIT(idle->port[port_id].idle_timeout_mask_bitmask);
	else
		val &= ~(BIT(idle->port[port_id].idle_timeout_mask_bit));
	idle_reg_writel(idle, val, idle->port[port_id].idle_timeout_mask_addr);

	/* disable pcie idle wake msk interrupt, will enable while timeout interrupt handller */
	val = idle_reg_readl(idle, idle->port[port_id].idle_wake_mask_addr);
	if (idle->port[port_id].idle_wake_mask_bitmask)
		val |= BIT(idle->port[port_id].idle_wake_mask_bit) |
			BIT(idle->port[port_id].idle_wake_mask_bitmask);
	else
		val |= BIT(idle->port[port_id].idle_wake_mask_bit);
	idle_reg_writel(idle, val, idle->port[port_id].idle_wake_mask_addr);

	/* init pcie l1ss cnt time set */
	if (idle->port[port_id].idle_l1ss_cnt_time_set)
		idle_reg_writel(idle,
		idle->port[port_id].idle_l1ss_cnt_time_set, idle->port[port_id].idle_l1ss_cnt_time_set_addr);

SKIP_IDLE_SLEEP_INIT:
	/* set refclk state to 1 */
	atomic_set(&(idle->port[port_id].idle_state), 0);

	hi_pcie_pr_d("idle sleep enable state %u", idle->idle_enable);
	hi_pcie_pr_d("idle timeout mask bit state[0x%x]: 0x%x",
		idle->port[port_id].idle_timeout_mask_addr,
		idle_reg_readl(idle, idle->port[port_id].idle_timeout_mask_addr));

	hi_pcie_pr_d("idle wake mask bit state[0x%x]: 0x%x",
		idle->port[port_id].idle_wake_mask_addr,
		idle_reg_readl(idle, idle->port[port_id].idle_wake_mask_addr));

	hi_pcie_pr_d("L1ss cnt base[0x%x]: 0x%x", idle->port[port_id].idle_l1ss_cnt_en,
		idle_reg_readl(idle, idle->port[port_id].idle_l1ss_cnt_en_addr));

	hi_pcie_pr_d("-%s-", __func__);
}

static void hi_pcie_idle_startup(struct hi_pcie_idle *idle)
{
	u32 i;

	for (i = 0; i < idle->port_num; i++) {
		hi_pcie_pr_d("startup port(%u/%u) idle", idle->host_id, i);
		hi_pcie_idle_startup_port(idle, i);
	}
}

static void hi_pcie_idle_shutdown_port(struct hi_pcie_idle *idle, u32 port_id)
{
	u32 val;

	hi_pcie_pr_d("+%s+", __func__);

	if (idle->idle_enable == 0)
		goto SKIP_IDLE_SLEEP_DEINIT;

	/* disable idle l1ss cnt machine */
	val = idle_reg_readl(idle, idle->port[port_id].idle_l1ss_cnt_en_addr);
	if (idle->port[port_id].idle_l1ss_cnt_en_bitmask)
		val |= BIT(idle->port[port_id].idle_l1ss_cnt_en_bitmask);
	val &= ~(BIT(idle->port[port_id].idle_l1ss_cnt_en));
	idle_reg_writel(idle, val, idle->port[port_id].idle_l1ss_cnt_en_addr);

	/* clear pcie idle timeout interrupt */
	val = idle_reg_readl(idle, idle->port[port_id].idle_timeout_clr_addr);
	if (idle->port[port_id].idle_timeout_clr_bitmask)
		val |= BIT(idle->port[port_id].idle_timeout_clr_bit) |
			BIT(idle->port[port_id].idle_timeout_clr_bitmask);
	else
		val |= BIT(idle->port[port_id].idle_timeout_clr_bit);
	idle_reg_writel(idle, val, idle->port[port_id].idle_timeout_clr_addr);

	/* enable pcie idle timeout msk interrupt */
	val = idle_reg_readl(idle, idle->port[port_id].idle_timeout_mask_addr);
	if (idle->port[port_id].idle_timeout_mask_bitmask)
		val |= BIT(idle->port[port_id].idle_timeout_mask_bit) |
			BIT(idle->port[port_id].idle_timeout_mask_bitmask);
	else
		val |= BIT(idle->port[port_id].idle_timeout_mask_bit);
	idle_reg_writel(idle, val, idle->port[port_id].idle_timeout_mask_addr);

	/* clear pcie idle wake interrupt */
	val = idle_reg_readl(idle, idle->port[port_id].idle_wake_clr_addr);
	if (idle->port[port_id].idle_wake_clr_bitmask)
		val |= BIT(idle->port[port_id].idle_wake_clr_bit) |
			BIT(idle->port[port_id].idle_wake_clr_bitmask);
	else
		val |= BIT(idle->port[port_id].idle_wake_clr_bit);
	idle_reg_writel(idle, val, idle->port[port_id].idle_wake_clr_addr);

	/* enable pcie idle wake msk interrupt */
	val = idle_reg_readl(idle, idle->port[port_id].idle_wake_mask_addr);
	if (idle->port[port_id].idle_wake_mask_bitmask)
		val |= BIT(idle->port[port_id].idle_wake_mask_bit) |
			BIT(idle->port[port_id].idle_wake_mask_bitmask);
	else
		val |= BIT(idle->port[port_id].idle_wake_mask_bit);
	idle_reg_writel(idle, val, idle->port[port_id].idle_wake_mask_addr);

SKIP_IDLE_SLEEP_DEINIT:
	/* clear refclk vote state */
	atomic_set(&(idle->port[port_id].idle_state), 1);
	atomic_set(&(idle->host_idle_vote), 0);
	hi_pcie_pr_d("idle sleep enable state %u", idle->idle_enable);
	hi_pcie_pr_d("idle timeout mask bit state[0x%x]: 0x%x",
		idle->port[port_id].idle_timeout_mask_addr,
		idle_reg_readl(idle, idle->port[port_id].idle_timeout_mask_addr));

	hi_pcie_pr_d("idle wake mask bit state[0x%x]: 0x%x",
		idle->port[port_id].idle_wake_mask_addr,
		idle_reg_readl(idle, idle->port[port_id].idle_wake_mask_addr));

	hi_pcie_pr_d("L1ss cnt base[0x%x]: 0x%x", idle->port[port_id].idle_l1ss_cnt_en,
		idle_reg_readl(idle, idle->port[port_id].idle_l1ss_cnt_en_addr));

	hi_pcie_pr_d("-%s-", __func__);
}

static void hi_pcie_idle_shutdown(struct hi_pcie_idle *idle)
{
	u32 i = 0;

	for (i = 0; i < idle->port_num; i++) {
		hi_pcie_pr_d("shutdown port(%u/%u) idle", idle->host_id, i);
		hi_pcie_idle_shutdown_port(idle, i);
	}
}

static inline void hi_pcie_idle_sleep_init_ops_register(struct hi_pcie_idle *idle)
{
	idle->startup = hi_pcie_idle_startup;
	idle->shutdown = hi_pcie_idle_shutdown;
}

static void hi_pcie_idle_bind_host(struct hi_pcie *pcie,
			struct hi_pcie_idle *idle)
{
	hi_pcie_idle_sleep_init_ops_register(idle);

	pcie->idle = idle;

	if (pcie->plat_ops && pcie->plat_ops->idle_init) {
		hi_pcie_pr_d("Idle init");
		pcie->plat_ops->idle_init(pcie);
	}
}

static inline void hi_pcie_idle_vote_init(struct hi_pcie_idle *idle)
{
	u32 i;

	if (idle) {
		for (i = 0; i < idle->port_num; i++)
			atomic_set(&(idle->port[i].device_idle_vote), 1);
	}
}

static int hi_pcie_get_idle_irq(struct hi_pcie_idle *idle,
			struct device_node *np)
{
	int irq;

	irq = of_irq_get_byname(np, PCIE_IDLE_TIMEOUT_IRQ);
	if (irq < 0) {
		hi_pcie_pr_e("Failed to get [%s] irq ", PCIE_IDLE_TIMEOUT_IRQ);
		return -ENODEV;
	}
	idle->port[0].irq.timeout_irq = (unsigned int)irq;

	irq = of_irq_get_byname(np, PCIE_IDLE_WAKE_IRQ);
	if (irq < 0) {
		hi_pcie_pr_e("Failed to get [%s] irq ", PCIE_IDLE_WAKE_IRQ);
		return -ENODEV;
	}
	idle->port[0].irq.wake_irq = (unsigned int)irq;

	hi_pcie_pr_i("irq nums, idle_timeout:%u, idle_wake:%u",
		idle->port[0].irq.timeout_irq, idle->port[0].irq.wake_irq);

	if (idle->port_num == 2) {
		irq = of_irq_get_byname(np, PCIE_IDLE_TIMEOUT_IRQ_1);
		if (irq < 0) {
			hi_pcie_pr_e("Failed to get [%s] irq ", PCIE_IDLE_TIMEOUT_IRQ_1);
			return -ENODEV;
		}
		idle->port[1].irq.timeout_irq = (unsigned int)irq;

		irq = of_irq_get_byname(np, PCIE_IDLE_WAKE_IRQ_1);
		if (irq < 0) {
			hi_pcie_pr_e("Failed to get [%s] irq ", PCIE_IDLE_WAKE_IRQ_1);
			return -ENODEV;
		}
		idle->port[1].irq.wake_irq = (unsigned int)irq;

		hi_pcie_pr_i("irq nums, idle_timeout_1:%u, idle_wake_1:%u",
			idle->port[1].irq.timeout_irq, idle->port[1].irq.wake_irq);
	}

	return 0;
}

static void hi_pcie_idle_exit(struct hi_pcie_idle_port *port)
{
	struct hi_pcie *pcie = port->pcie;
	if (unlikely(!pcie)) {
		hi_pcie_pr_e("[%s], pcie point is null", __func__);
		return;
	}

	hi_pcie_pr_d("++%s++", __func__);
	if (atomic_cmpxchg(&(port->idle_state), 1, 0) == 0)
		return;

	if (pcie->plat_ops && pcie->plat_ops->exit_idle) {
		hi_pcie_pr_i("Exit idle, turn on refclk");
		(void)pcie->plat_ops->exit_idle(pcie, port->id);
	}
}

static void hi_pcie_idle_enter(struct hi_pcie_idle_port *port)
{
	struct hi_pcie *pcie = port->pcie;
	if (unlikely(!pcie)) {
		hi_pcie_pr_e("[%s], pcie point is null", __func__);
		return;
	}

	hi_pcie_pr_d("++%s++", __func__);
	if (atomic_cmpxchg(&(port->idle_state), 0, 1) != 0)
		return;

	if (pcie->plat_ops && pcie->plat_ops->enter_idle) {
		hi_pcie_pr_i("Enter idle, turn off refclk");
		(void)pcie->plat_ops->enter_idle(pcie, port->id);
	}
}

static bool hi_pcie_idle_vote_count(struct hi_pcie_port *pcie_port, struct hi_pcie_idle_port *port)
{
	if (atomic_read(&(pcie_port->pcie->idle->host_idle_vote)))
		return true;

	if ((pcie_port->disable) || (atomic_read(&(pcie_port->is_link_up)) == 0x0))
		return false;

	if (atomic_read(&(port->device_idle_vote)))
		return true;

	return false;
}

static irqreturn_t hi_pcie_idle_timeout_irq_handler(int irq, void *arg)
{
	struct hi_pcie_idle_port *port = (struct hi_pcie_idle_port *)arg;
	struct hi_pcie_port *pcie_port = port->pcie_port;
	struct hi_pcie_idle *idle =  port->pcie->idle;
	unsigned long flags;
	u32 val;

	spin_lock_irqsave(&(idle->idle_lock), flags);
	hi_pcie_pr_i("+%s+ cdx %u irq %d", __func__, pcie_port->pcie->ctrl_id, irq);

	if (atomic_read(&(pcie_port->pcie->is_power_on)) == 0)
		goto PWR_OFF;

	if (!hi_pcie_idle_vote_count(pcie_port, port)) {
		hi_pcie_pr_i("Allow idle sleep! Ref clk will off");
		hi_pcie_idle_enter(port);
	}

	/* enable pcie idle timeout mask */
	val = idle_reg_readl(idle, port->idle_timeout_mask_addr);
	if (port->idle_timeout_mask_bitmask)
		val = BIT(port->idle_timeout_mask_bit) |
			BIT(port->idle_timeout_mask_bitmask);
	else
		val |= BIT(port->idle_timeout_mask_bit);
	idle_reg_writel(idle, val, port->idle_timeout_mask_addr);

	/* clear pcie idle timeout int */
	val = idle_reg_readl(idle, port->idle_timeout_clr_addr);
	if (port->idle_timeout_clr_bitmask)
		val |= BIT(port->idle_timeout_clr_bit) |
			BIT(port->idle_timeout_clr_bitmask);
	else
		val |= BIT(port->idle_timeout_clr_bit);
	idle_reg_writel(idle, val, port->idle_timeout_clr_addr);

	/* clear pcie idle wake int */
	val = idle_reg_readl(idle, port->idle_wake_clr_addr);
	if (port->idle_wake_clr_bitmask)
		val |= BIT(port->idle_wake_clr_bit) |
			BIT(port->idle_wake_clr_bitmask);
	else
		val |= BIT(port->idle_wake_clr_bit);
	idle_reg_writel(idle, val, port->idle_wake_clr_addr);

	/* disable wake interrupt mask */
	val = idle_reg_readl(idle, port->idle_wake_mask_addr);
	if (port->idle_wake_mask_bitmask)
		val = BIT(port->idle_wake_mask_bitmask);
	else
		val &= ~(BIT(port->idle_wake_mask_bit));
	idle_reg_writel(idle, val, port->idle_wake_mask_addr);

PWR_OFF:
	spin_unlock_irqrestore(&(idle->idle_lock), flags);

	return IRQ_HANDLED;
}

static irqreturn_t hi_pcie_idle_wake_irq_handler(int irq, void *arg)
{
	struct hi_pcie_idle_port *port = (struct hi_pcie_idle_port *)arg;
	struct hi_pcie_port *pcie_port = port->pcie_port;
	struct hi_pcie_idle *idle =  port->pcie->idle;
	unsigned long flags;
	u32 val;
	hi_pcie_pr_i("+%s+ cdx %u irq %d", __func__, pcie_port->pcie->ctrl_id, irq);
	spin_lock_irqsave(&(idle->idle_lock), flags);

	if (atomic_read(&(pcie_port->pcie->is_power_on)) == 0)
		goto PWR_OFF;

	/* enable pcie idle wake mask */
	val = idle_reg_readl(idle, port->idle_wake_mask_addr);
	if (port->idle_wake_mask_bitmask)
		val |= BIT(port->idle_wake_mask_bit) |
			BIT(port->idle_wake_mask_bitmask);
	else
		val |= BIT(port->idle_wake_mask_bit);
	idle_reg_writel(idle, val, port->idle_wake_mask_addr);

	/* clear pcie idle wake interrupt */
	val = idle_reg_readl(idle, port->idle_wake_clr_addr);
	if (port->idle_wake_clr_bitmask)
		val = BIT(port->idle_wake_clr_bit) |
			BIT(port->idle_wake_clr_bitmask);
	else
		val |= BIT(port->idle_wake_clr_bit);
	idle_reg_writel(idle, val, port->idle_wake_clr_addr);

	/* clear pcie idle timeout interrupt */
	val = idle_reg_readl(idle, port->idle_timeout_clr_addr);
	if (port->idle_wake_clr_bit)
		val |= BIT(port->idle_timeout_clr_bit) |
			BIT(port->idle_timeout_clr_bitmask);
	else
		val |= BIT(port->idle_wake_clr_bit);
	idle_reg_writel(idle, val, port->idle_timeout_clr_addr);

	/* disable timeout interrupt mask */
	val = idle_reg_readl(idle, port->idle_timeout_mask_addr);
	if (port->idle_timeout_mask_bitmask)
		val = BIT(port->idle_timeout_mask_bitmask);
	else
		val &= ~(BIT(port->idle_timeout_mask_bit));
	idle_reg_writel(idle, val, port->idle_timeout_mask_addr);

	hi_pcie_idle_exit(port);

PWR_OFF:
	spin_unlock_irqrestore(&(idle->idle_lock), flags);

	return IRQ_HANDLED;
}

static int hi_pcie_register_idle_irq(struct hi_pcie_idle *idle,
			struct device *dev)
{
	int ret;

	ret = devm_request_irq(dev,
			idle->port[0].irq.timeout_irq,
			hi_pcie_idle_timeout_irq_handler,
			IRQF_TRIGGER_RISING,
			PCIE_IDLE_TIMEOUT_IRQ,
			&(idle->port[0]));
	if (ret) {
		hi_pcie_pr_e("Register idle timeout irq");
		return ret;
	}

	ret = devm_request_irq(dev,
			idle->port[0].irq.wake_irq,
			hi_pcie_idle_wake_irq_handler,
			IRQF_TRIGGER_RISING,
			PCIE_IDLE_WAKE_IRQ,
			&(idle->port[0]));
	if (ret) {
		hi_pcie_pr_e("Register idle wake irq");
		return ret;
	}

	if (idle->port_num == 2) {
		ret = devm_request_irq(dev,
				idle->port[1].irq.timeout_irq,
				hi_pcie_idle_timeout_irq_handler,
				IRQF_TRIGGER_RISING,
				PCIE_IDLE_TIMEOUT_IRQ_1,
				&(idle->port[1]));
		if (ret) {
			hi_pcie_pr_e("Register idle timeout_1 irq");
			return ret;
		}

		ret = devm_request_irq(dev,
				idle->port[1].irq.wake_irq,
				hi_pcie_idle_wake_irq_handler,
				IRQF_TRIGGER_RISING,
				PCIE_IDLE_WAKE_IRQ_1,
				&(idle->port[1]));
		if (ret) {
			hi_pcie_pr_e("Register idle wake_1 irq");
			return ret;
		}
	}

	return 0;
}

int hi_pcie_idle_hook_register(struct platform_device *pdev)
{
	int ret;

	struct hi_pcie *pcie = dev_get_drvdata(&pdev->dev);
	struct device *dev = &(pdev->dev);
	struct device_node *np = NULL;
	struct hi_pcie_idle *idle = NULL;

	hi_pcie_pr_i("+%s+", __func__);

	if (!pcie || !dev) {
		hi_pcie_pr_e("Null platform");
		return 0;
	}

	np = dev->of_node;
	if (!np) {
		hi_pcie_pr_e("Null np");
		return 0;
	}

	if (!of_find_property(np, PCIE_SUPPORT_IDLE_FEATURE, NULL)) {
		hi_pcie_pr_e("Not Support 32K idle");
		return 0;
	}

	idle = devm_kzalloc(dev, sizeof(struct hi_pcie_idle), GFP_KERNEL);
	if (!idle) {
		hi_pcie_pr_e("Alloc idle info");
		return -ENOMEM;
	}

	ret = hi_pcie_idle_dtsinfo_get(pcie, idle);
	if (ret) {
		devm_kfree(dev, idle);
		return ret;
	}

	hi_pcie_idle_bind_host(pcie, idle);
	spin_lock_init(&(idle->idle_lock));
	hi_pcie_idle_vote_init(idle);

	ret = hi_pcie_get_idle_irq(idle, np);
	if (ret) {
		devm_kfree(dev, idle);
		return ret;
	}

	ret = hi_pcie_register_idle_irq(idle, dev);
	if (ret) {
		devm_kfree(dev, idle);
		return ret;
	}

	hi_pcie_pr_i("-%s-", __func__);
	return 0;
}

void hi_pcie_idle_host_vote(struct hi_pcie_idle *idle, u32 id, u32 vote)
{
	unsigned long flags;

	if ((idle == NULL) || unlikely(idle->idle_enable == 0))
		return;

	spin_lock_irqsave(&(idle->idle_lock), flags);
	if (vote) {
		if (atomic_read(&(idle->host_idle_vote)) == 0)
			hi_pcie_idle_exit(&(idle->port[id]));
		atomic_inc(&(idle->host_idle_vote));
	} else {
		if (atomic_read(&(idle->host_idle_vote)) == 0) {
			/* should NOT run to this flow */
			hi_pcie_pr_e("pcie ref clk vote already 0!");
			goto SPIN_UNLOCK;
		}
		atomic_dec(&(idle->host_idle_vote));
	}
SPIN_UNLOCK:
	spin_unlock_irqrestore(&(idle->idle_lock), flags);
}

void hi_pcie_idle_device_vote(u32 cdx, u32 pdx, u32 vote)
{
	unsigned long flags;
	struct hi_pcie *pcie = NULL;

	pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("pcie (%u-%u) pointer is null", cdx, pdx);
		return;
	}

	if (unlikely(!pcie))
		return;

	if ((pcie->idle == NULL) || unlikely(pcie->idle->idle_enable == 0))
		return;
	spin_lock_irqsave(&(pcie->idle->idle_lock), flags);
	if (vote) {
		atomic_set(&(pcie->idle->port[pdx].device_idle_vote), 1);
		hi_pcie_idle_exit(&(pcie->idle->port[pdx]));
	} else {
		atomic_set(&(pcie->idle->port[pdx].device_idle_vote), 0);
	}
	spin_unlock_irqrestore(&(pcie->idle->idle_lock), flags);
}

void hi_pcie_idle_enable_irq(struct hi_pcie_idle *idle)
{
	u32 i;

	if (idle == NULL)
		return;

	for (i = 0; i < idle->port_num; i++) {
		enable_irq(idle->port[i].irq.timeout_irq);
		enable_irq(idle->port[i].irq.wake_irq);
	}
}

void hi_pcie_idle_disable_irq(struct hi_pcie_idle *idle)
{
	u32 i;

	if (idle == NULL)
		return;

	for (i = 0; i < idle->port_num; i++) {
		disable_irq(idle->port[i].irq.timeout_irq);
		disable_irq(idle->port[i].irq.wake_irq);
	}
}

void hi_pcie_idle_mask_int(struct hi_pcie_idle *idle)
{
	u32 i;
	u32 val;

	if (idle == NULL)
		return;

	for (i = 0; i < idle->port_num; i++) {
		/* enable pcie idle timeout mask */
		val = idle_reg_readl(idle, idle->port[i].idle_timeout_mask_addr);
		if (idle->port[i].idle_timeout_mask_bitmask)
			val = BIT(idle->port[i].idle_timeout_mask_bit) |
				BIT(idle->port[i].idle_timeout_mask_bitmask);
		else
			val |= BIT(idle->port[i].idle_timeout_mask_bit);
		idle_reg_writel(idle, val, idle->port[i].idle_timeout_mask_addr);

		/* enable pcie idle wake mask */
		val = idle_reg_readl(idle, idle->port[i].idle_wake_mask_addr);
		if (idle->port[i].idle_wake_mask_bitmask)
			val |= BIT(idle->port[i].idle_wake_mask_bit) |
				BIT(idle->port[i].idle_wake_mask_bitmask);
		else
			val |= BIT(idle->port[i].idle_wake_mask_bit);
		idle_reg_writel(idle, val, idle->port[i].idle_wake_mask_addr);
	}
}

void hi_pcie_idle_clear_irq(struct hi_pcie_idle *idle)
{
	u32 i;
	u32 val;

	if (idle == NULL)
		return;

	for (i = 0; i < idle->port_num; i++) {
		/* clear pcie idle timeout int */
		val = idle_reg_readl(idle, idle->port[i].idle_timeout_clr_addr);
		if (idle->port[i].idle_timeout_clr_bitmask)
			val |= BIT(idle->port[i].idle_timeout_clr_bit) |
				BIT(idle->port[i].idle_timeout_clr_bitmask);
		else
			val |= BIT(idle->port[i].idle_timeout_clr_bit);
		idle_reg_writel(idle, val, idle->port[i].idle_timeout_clr_addr);

		/* clear pcie idle wake int */
		val = idle_reg_readl(idle, idle->port[i].idle_wake_clr_addr);
		if (idle->port[i].idle_wake_clr_bitmask)
			val |= BIT(idle->port[i].idle_wake_clr_bit) |
				BIT(idle->port[i].idle_wake_clr_bitmask);
		else
			val |= BIT(idle->port[i].idle_wake_clr_bit);
		idle_reg_writel(idle, val, idle->port[i].idle_wake_clr_addr);
	}
}

