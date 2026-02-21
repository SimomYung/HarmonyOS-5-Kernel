// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022 Qualcomm Innovation Center. All rights reserved.
 *
 */

#include <asm/unaligned.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/msi.h>
#include <linux/fscrypt_common.h>
#include <linux/cpuhotplug.h>

#include "ufshci.h"
#include "ufshcd.h"
#include "hufs_plat.h"
#include "ufstt.h"
#include "ufs_debugfs.h"
#include "ufshcd_debug.h"
#include "ufshcd-crypto.h"
#include "hufs-dump.h"
#include "ufshcd_extend.h"
#include "basicplatform/linux/mas/mas_blk.h"
#include "platform_include/basicplatform/linux/ufs/ufshcd_protocol.h"
#include "platform_include/basicplatform/linux/ufs/ufs_mcq.h"
#include <scsi/scsi_eh.h>
#ifndef CONFIG_MCQ_OHOS
#include <trace/events/ufs.h>
#include <trace/hooks/ufshcd.h>
#endif
#ifdef CONFIG_DFA_MCQ
#include "dfa_mcq.h"
#endif

static void ufshcd_mcq_transfer_req_compl(struct ufs_hba *hba, struct ufs_hw_queue *hwq);

static void ufsmcq_inflt_req_set(struct ufs_hba *hba, int tag)
{
	if (tag == DEV_CMD_QUERY_TAG)
		return;

	atomic_inc(&hba->mcq_inflt_reqs);
	set_bit(mcq_tag_off(tag), mcq_tag_bitmap(hba, tag));
}

void ufsmcq_inflt_req_clear(struct ufs_hba *hba, int tag)
{
	if (tag == DEV_CMD_QUERY_TAG)
		return;

	clear_bit(mcq_tag_off(tag), mcq_tag_bitmap(hba, tag));
	atomic_dec(&hba->mcq_inflt_reqs);
}

int ufsmcq_check_inflt_reqs(struct ufs_hba *hba, unsigned long timeout)
{
	while (atomic_read(&hba->mcq_inflt_reqs)) {
		if (timeout == 0) {
			dev_err(hba->dev, "%s: wait complete timeout! reqs:%d\n",
				__func__, atomic_read(&hba->mcq_inflt_reqs));
			return -ETIMEDOUT;
		}
		timeout--;
		mdelay(1);
	}
	return 0;
}

int __ufsmcq_wait_for_reqs_clr(struct ufs_hba *hba)
{
	u32 wait_timeout_us = 2000000;
	int ret = 0;
	ktime_t start = ktime_get();

	if (!atomic_read(&hba->mcq_inflt_reqs))
		return 0;

	do {
		/*lint -save -e446*/
		if (ktime_to_us(ktime_sub(ktime_get(), start)) > wait_timeout_us) {
			ret = -EIO;
			dev_err(hba->dev, "wait all reqs complete timeout\n");
			hufs_key_reg_dump(hba);
			break;
		}
		/*lint -restore*/
	} while (atomic_read(&hba->mcq_inflt_reqs));

	return ret;
}

void hufs_uic_err_dump(struct ufs_hba *hba)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;
	u32 reg_val = 0;

	dev_err(hba->dev, "UFS_AUTO_H8_STATE_OFF: 0x%x\n", ufshcd_readl(hba, UFS_AUTO_H8_STATE_OFF));
	dev_err(hba->dev, "MPHY_SYS_CLK_EN: 0x%x\n", readl(host->ao_crg + AO_CRG_SCPERSTAT6));
	hufs_uic_read_reg(hba, DME_UNIPRO_STATE, &reg_val);
	dev_err(hba->dev, "DME_UNIPRO_STATE: 0x%x\n", reg_val);
	hufs_uic_read_reg(hba, PA_DBG_TX_TIMER, &reg_val);
	dev_err(hba->dev, "PA_DBG_TX_TIMER: 0x%x\n", reg_val);
	hufs_uic_read_reg(hba, PA_PWRMODE, &reg_val);
	dev_err(hba->dev, "PA_PWRMODE: 0x%x\n", reg_val);
	hufs_uic_read_reg(hba, PA_TXGEAR, &reg_val);
	dev_err(hba->dev, "PA_TXGEAR: 0x%x\n", reg_val);
	hufs_uic_read_reg(hba, PA_RXGEAR, &reg_val);
	dev_err(hba->dev, "PA_RXGEAR: 0x%x\n", reg_val);
}

void ufshcd_mcq_common_dump(struct ufs_hba *hba)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;
	u32 reg = 0;
	u32 index;
	u32 array_size;
	const struct st_register_dump unipro_uic_mcq[] = {
		{ PA_DBG_TX_TIMER, "PA_DBG_TX_TIMER" },
		{ PA_RXGEAR, "PA_RXGEAR" },
		{ PA_MAXRXPWMGEAR, "PA_MAXRXPWMGEAR" },
		{ PA_FSM_STATUS, "PA_FSM_STATUS" },
		{ PA_STATUS, "PA_STATUS" },
		{ DME_UNIPRO_STATE, "DME_UNIPRO_STATE" },
		{ PA_DL_IMPL_STATE, "PA_DL_IMPL_STATE" },
		{ DFX_RMMI_TX_SYM_STATE1, "DFX_RMMI_TX_SYM_STATE1" },
		{ DFX_RMMI_RX_SYM_STATE1, "DFX_RMMI_RX_SYM_STATE1" },
		{ DME_UECPHY, "DME_UECPHY" },
		{ DME_UECPA, "DME_UECPA" },
		{ DME_UECDL, "DME_UECDL" },
		{ DME_UECTL, "DME_UECTL" },
		{ DME_UECNL, "DME_UECNL" },
		{ DME_UECDME, "DME_UECDME" },
	};

	dev_err(hba->dev, "MPHY_SYS_CLK_EN: 0x%x\n", readl(host->ao_crg + AO_CRG_SCPERSTAT6));
	dev_err(hba->dev, "UFS_AUTO_H8_STATE_OFF: 0x%x\n", ufshcd_readl(hba, UFS_AUTO_H8_STATE_OFF));
	array_size = sizeof(unipro_uic_mcq) / sizeof(struct st_register_dump);
	for (index = 0; index < array_size; index++) {
		/* dont print more info if one uic cmd failed */
		hufs_uic_read_reg(hba, unipro_uic_mcq[index].addr, &reg);
		dev_err(hba->dev, "%-27s 0x%x: 0x%08x\n", unipro_uic_mcq[index].name, unipro_uic_mcq[index].addr, reg);
	}

	if (!((host->caps & USE_HUFS_MPHY_TC) || hba->host->is_emulator)) {
		writel(MPHY_SYS_CLK_EN, host->ao_crg + AO_CRG_SCPEREN6);
		dev_err(hba->dev, "PMA_TX_RATE_PWR: 0x%x\n", mphy_pma_readl(host, PMA_TX_RATE_PWR));
		dev_err(hba->dev, "PMA_RX_RATE_PWR: 0x%x\n", mphy_pma_readl(host, PMA_RX_RATE_PWR));
		dev_err(hba->dev, "PMA_TX_TERM: 0x%x\n", mphy_pma_readl(host, PMA_TX_TERM));
		dev_err(hba->dev, "PMA_RX_TERM: 0x%x\n", mphy_pma_readl(host, PMA_RX_TERM));
		dev_err(hba->dev, "PMA_TOP_FSM: 0x%x\n", mphy_pma_readl(host, PMA_TOP_FSM));
		dev_err(hba->dev, "PMA_TX0_FSM: 0x%x\n", mphy_pma_readl(host, PMA_TX0_FSM));
		dev_err(hba->dev, "PMA_RX0_FSM: 0x%x\n", mphy_pma_readl(host, PMA_RX0_FSM));
		dev_err(hba->dev, "PMA_CS_LOCK_FLAG: 0x%x\n", mphy_pma_readl(host, PMA_CS_LOCK_FLAG));
		dev_err(hba->dev, "PMA_API_CSR0: 0x%x\n", mphy_pma_readl(host, PMA_API_CSR0));
		dev_err(hba->dev, "PMA_API_CSR1: 0x%x\n", mphy_pma_readl(host, PMA_API_CSR1));
	}
}

void ufshcd_mcq_dump(struct ufs_hba *hba)
{
	struct ufs_hw_queue *hwq;
	struct hufs_host *host = (struct hufs_host *)hba->priv;
	u32 queue_num;

	if (!hba->use_mcq)
		return;

	for (queue_num = 0; queue_num < hba->nr_hw_queues; queue_num++) {
		hwq = &hba->uhq[queue_num];
		pr_err("----------- ufs mcq soft dump queue_index: %d --------\n", queue_num);
		dev_err(hba->dev, "hwq->id 0x%08x\n", hwq->id);
		dev_err(hba->dev, "hwq->sq_tp_slot 0x%08x\n", hwq->sq_tp_slot);
		dev_err(hba->dev, "hwq->sq_hp_slot 0x%08x\n", hwq->sq_hp_slot);
		dev_err(hba->dev, "hwq->cq_tp_slot 0x%08x\n", hwq->cq_tp_slot);
		dev_err(hba->dev, "hwq->cq_hp_slot 0x%08x\n", hwq->cq_hp_slot);
		pr_err("----------- ufs mcq dump queue_index: %d end --------\n", queue_num);
	}

	for (queue_num = 0; queue_num < MCQ_HW_DEFAULT_NUM; queue_num++) {
		pr_err("----------- ufs mcq hardware dump queue_index: %d --------\n", queue_num);
		dev_err(hba->dev, "REG_SQHP: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_SQD_BASE + queue_num * MCQ_HW_SQD_SCALE + REG_SQHP));
		dev_err(hba->dev, "REG_SQTP: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_SQD_BASE + queue_num * MCQ_HW_SQD_SCALE + REG_SQTP));
		dev_err(hba->dev, "REG_SQRTC: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_SQD_BASE + queue_num * MCQ_HW_SQD_SCALE + REG_SQRTC));
		dev_err(hba->dev, "REG_SQCTI: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_SQD_BASE + queue_num * MCQ_HW_SQD_SCALE + REG_SQCTI));
		dev_err(hba->dev, "REG_SQRTS: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_SQD_BASE + queue_num * MCQ_HW_SQD_SCALE + REG_SQRTS));
		dev_err(hba->dev, "REG_CQHP: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_CQD_BASE + queue_num * MCQ_HW_CQD_SCALE + REG_CQHP));
		dev_err(hba->dev, "REG_CQTP: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_CQD_BASE + queue_num * MCQ_HW_CQD_SCALE + REG_CQTP));
		dev_err(hba->dev, "REG_CQIE: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_CQIS_BASE + queue_num * MCQ_HW_CQIS_SCALE + REG_CQIE));
		dev_err(hba->dev, "REG_CQIS: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_CQIS_BASE + queue_num * MCQ_HW_CQIS_SCALE + REG_CQIS));
		dev_err(hba->dev, "REG_SQATTR: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_MIMO_BASE + queue_num * MCQ_HW_MIMO_SCALE + REG_SQATTR));
		dev_err(hba->dev, "REG_CQATTR: 0x%08x\n", readl(hba->mmio_base + MCQ_HW_MIMO_BASE + queue_num * MCQ_HW_MIMO_SCALE + REG_CQATTR));
		pr_err("----------- ufs mcq dump queue_index: %d end --------\n", queue_num);
	}
	dev_err(hba->dev, "Global Config Register: 0x%x\n", ufshcd_readl(hba, REG_UFS_GCR_CFG));
	dev_err(hba->dev, "REG_UFS_ESIULBA: 0x%x\n", ufshcd_readl(hba, REG_UFS_ESILBA));
	dev_err(hba->dev, "REG_UFS_ESIUBA: 0x%x\n", ufshcd_readl(hba, REG_UFS_ESIUBA));
	dev_err(hba->dev, "MSI_DEVICE_ID: 0x%x\n", ufshcd_readl(hba, MSI_DEVICE_ID));
	dev_err(hba->dev, "UFS_AXI_MST_WR_OTD_LOW: 0x%x\n", ufshcd_readl(hba, UFS_AXI_MST_WR_OTD_LOW));
	dev_err(hba->dev, "UFS_AXI_MST_WR_OTD_HIG: 0x%x\n", ufshcd_readl(hba, UFS_AXI_MST_WR_OTD_HIG));
	dev_err(hba->dev, "UFS_AXI_MST_RD_OTD_LOW: 0x%x\n", ufshcd_readl(hba, UFS_AXI_MST_RD_OTD_LOW));
	dev_err(hba->dev, "UFS_AXI_MST_RD_OTD_HIG: 0x%x\n", ufshcd_readl(hba, UFS_AXI_MST_RD_OTD_HIG));
	dev_err(hba->dev, "UFS_DMA1_RX_AXI_ERR_ADDRL: 0x%x\n", ufshcd_readl(hba, UFS_DMA1_RX_AXI_ERR_ADDRL));
	dev_err(hba->dev, "UFS_DMA1_RX_AXI_ERR_ADDRU: 0x%x\n", ufshcd_readl(hba, UFS_DMA1_RX_AXI_ERR_ADDRU));
	dev_err(hba->dev, "UFS_DMA1_TX_AXI_ERR_ADDRL: 0x%x\n", ufshcd_readl(hba, UFS_DMA1_TX_AXI_ERR_ADDRL));
	dev_err(hba->dev, "UFS_DMA1_TX_AXI_ERR_ADDRU: 0x%x\n", ufshcd_readl(hba, UFS_DMA1_TX_AXI_ERR_ADDRU));
	dev_err(hba->dev, "UFS_DMA0_RX_AXI_ERR_ADDRL: 0x%x\n", ufshcd_readl(hba, UFS_DMA0_RX_AXI_ERR_ADDRL));
	dev_err(hba->dev, "UFS_DMA0_RX_AXI_ERR_ADDRU: 0x%x\n", ufshcd_readl(hba, UFS_DMA0_RX_AXI_ERR_ADDRU));
	dev_err(hba->dev, "UFS_DMA0_TX_AXI_ERR_ADDRL: 0x%x\n", ufshcd_readl(hba, UFS_DMA0_TX_AXI_ERR_ADDRL));
	dev_err(hba->dev, "UFS_DMA0_TX_AXI_ERR_ADDRU: 0x%x\n", ufshcd_readl(hba, UFS_DMA0_TX_AXI_ERR_ADDRU));
#ifdef CONFIG_SCSI_UFS_AQUILA
	dev_err(hba->dev, "----------- bus regist dump --------\n");
	dev_err(hba->dev, "HSDT0_QIC_NOPENDING_COMBO: 0x%x\n", readl(host->hsdt_ctrl + HSDT0_QIC_NOPENDING_COMBO));
	dev_err(hba->dev, "HSDT0_QIC_RD_NOPENDING_DFA_0: 0x%x\n", readl(host->hsdt_ctrl + HSDT0_QIC_RD_NOPENDING_DFA_0));
	dev_err(hba->dev, "HSDT0_QIC_WR_NOPENDING_DFA_0: 0x%x\n", readl(host->hsdt_ctrl + HSDT0_QIC_WR_NOPENDING_DFA_0));
	dev_err(hba->dev, "HSDT0_QIC_RT_NOPENDING_DFA_0: 0x%x\n", readl(host->hsdt_ctrl + HSDT0_QIC_RT_NOPENDING_DFA_0));
	dev_err(hba->dev, "HSDT0_QIC_TB_NOPENDING_DFA_0: 0x%x\n", readl(host->hsdt_ctrl + HSDT0_QIC_TB_NOPENDING_DFA_0));
	dev_err(hba->dev, "PCTRL:PERI_STAT69: 0x%x\n", readl(host->pctrl + PCTRL_PERI_STAT69));
#endif
}

u32 ufshcd_mcq_readl(struct ufs_hba *hba, int i, int reg)
{
	if (hba->uhq && hba->uhq[i].mcq_base)
		return readl(hba->uhq[i].mcq_base + reg);
	return -1;
}

void ufshcd_mcq_writel(struct ufs_hba *hba, int i, u32 val, int reg)
{
	if (hba->uhq && hba->uhq[i].mcq_base)
		writel(val, hba->uhq[i].mcq_base + reg);
}

static u32 ufshcd_mcq_read_cqis(struct ufs_hba *hba, int i)
{
	return readl(hba->uhq[i].mcq_base_cqis + REG_CQIS);
}

static void ufshcd_mcq_write_cqis(struct ufs_hba *hba, u32 val, int i)
{
	writel(val, hba->uhq[i].mcq_base_cqis + REG_CQIS);
}

#ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
static void ufshcd_mcq_enable_esi(struct ufs_hba *hba)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;

	ufshcd_writel(hba, ufshcd_readl(hba, REG_UFS_GCR_CFG) | 0x2, REG_UFS_GCR_CFG);
	if (host->host_id == 1)
		ufshcd_writel(hba, 0x20001, MSI_DEVICE_ID);
	else
		ufshcd_writel(hba, 0x20000, MSI_DEVICE_ID);
}

static void ufshcd_mcq_config_esi_base_address(struct ufs_hba *hba, struct msi_msg *msg)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;

	ufshcd_writel(hba, msg->address_lo, REG_UFS_ESILBA);
	ufshcd_writel(hba, msg->address_hi, REG_UFS_ESIUBA);

	host->esi_address_lo = msg->address_lo;
	host->esi_address_hi = msg->address_hi;
}

static void ufs_mcq_write_msi_msg(struct msi_desc *desc, struct msi_msg *msg)
{
	struct device *dev = msi_desc_to_dev(desc);
	struct ufs_hba *hba = dev_get_drvdata(dev);

	ufshcd_mcq_config_esi_base_address(hba, msg);
}

static irqreturn_t ufs_mcq_esi_handler(int irq, void *data)
{
	u32 id;
	u32 events;
	struct ufs_hw_queue *hwq;
	struct msi_desc *desc = data;
	struct device *dev = msi_desc_to_dev(desc);
	struct ufs_hba *hba = dev_get_drvdata(dev);

	id = desc->platform.msi_index - CQTEPS_MSI_START;
	if (id < 0 || id >= hba->nr_hw_queues) {
		dev_err(hba->dev, "ufs msi irq error irq:%x id:%d, idx:%u\n", irq, id, desc->platform.msi_index);
		return IRQ_HANDLED;
	}

	hwq = &hba->uhq[id];
	events = ufshcd_mcq_read_cqis(hba, id);
	if (events)
		ufshcd_mcq_write_cqis(hba, events, id);

	if (events & UFSHCD_MCQ_CQIS_TEPS)
		ufshcd_mcq_transfer_req_compl(hba, hwq);

	return IRQ_HANDLED;
}

#ifndef CONFIG_MCQ_OHOS
static int ufsmcq_starting_cpu(unsigned int cpu, struct hlist_node *mcq_node)
{
	struct ufs_hba *hba = hlist_entry_safe(mcq_node, struct ufs_hba, mcq_node);
	unsigned int mcq_syncw_irq_idx;

	if (is_mcq_enabled(hba)) {
		mcq_syncw_irq_idx = cpu + num_possible_cpus();

		irq_force_affinity(hba->mcq_irq[cpu], cpumask_of(cpu));
		irq_force_affinity(hba->mcq_irq[mcq_syncw_irq_idx], cpumask_of(cpu));
		return 0;
	}

	return 0;
}

static int ufsmcq_irq_cpuhp_setup(struct ufs_hba *hba)
{
	int ret;

	ret = cpuhp_setup_state_multi(CPUHP_AP_IRQ_UFS_STARTING,
				     "scsi/ufs:starting", ufsmcq_starting_cpu, NULL);
	if (ret) {
		dev_err(hba->dev, "Failed to register cpuhp notifier for UFS: %d\n", ret);
		return ret;
	}

	ret = cpuhp_state_add_instance(CPUHP_AP_IRQ_UFS_STARTING, &hba->mcq_node);
	if (ret) {
		dev_err(hba->dev, "Failed to add cpuhp instace: %d\n", ret);
		goto out_cpuhp;
	}

	return 0;
out_cpuhp:
	cpuhp_remove_multi_state(CPUHP_AP_IRQ_UFS_STARTING);
	return ret;
}

void ufsmcq_irq_cpuhp_remove(struct ufs_hba *hba)
{
	cpuhp_state_remove_instance(CPUHP_AP_IRQ_UFS_STARTING, &hba->mcq_node);
	cpuhp_remove_multi_state(CPUHP_AP_IRQ_UFS_STARTING);
}
#endif

static int ufshcd_mcq_config_esi(struct ufs_hba *hba)
{
	struct hufs_host *host = (struct hufs_host *)hba->priv;
	struct msi_desc *desc = NULL;
	struct msi_desc *failed_desc = NULL;
	unsigned int cpu;
	unsigned int msi_idx;
	int ret;

	if (!hba->esi_sup)
		return -1;

	if (host->esi_enabled) {
		/* reset need open esi again */
		ufshcd_mcq_enable_esi(hba);
		ufshcd_writel(hba, host->esi_address_lo, REG_UFS_ESILBA);
		ufshcd_writel(hba, host->esi_address_hi, REG_UFS_ESIUBA);

		return 0;
	}
	/*
	 * 1. We only handle CQs as of now.
	 * 2. Poll queues do not need ESI.
	 */
	ret = platform_msi_domain_alloc_irqs(hba->dev, 192,
					     ufs_mcq_write_msi_msg);
	if (ret)
		goto out;

	for_each_msi_entry(desc, hba->dev) {
		if (desc->platform.msi_index >= CQTEPS_MSI_START && desc->platform.msi_index <= CQTEPS_MSI_END) {
			ret = devm_request_irq(hba->dev, desc->irq,
						ufs_mcq_esi_handler, IRQF_SHARED, "mcq-esi", desc);
			if (ret) {
				dev_err(hba->dev, "%s: Fail to request IRQ for %d, err = %d\n",
					__func__, desc->irq, ret);
				failed_desc = desc;
				break;
			}

#ifndef CONFIG_MCQ_OHOS
			msi_idx = desc->platform.msi_index - CQTEPS_MSI_START;
			hba->mcq_irq[msi_idx] = desc->irq;
			if (msi_idx >= MCQ_SYNC_QUEUE_NUM)
				continue;
			cpu = msi_idx % num_possible_cpus();
			ret = irq_set_affinity(desc->irq, cpumask_of(cpu));
			if (ret && cpu_online(cpu)) {
				dev_err(hba->dev, "%s, set irq affinity cpu%u fail %d\n", __func__, cpu, ret);
				failed_desc = desc;
				break;
			}
#endif
		}
	}

	if (ret) {
		/* Rewind */
		for_each_msi_entry(desc, hba->dev) {
			if (desc == failed_desc)
				break;
			devm_free_irq(hba->dev, desc->irq, hba);
		}
		platform_msi_domain_free_irqs(hba->dev);
	} else {
		ufshcd_mcq_enable_esi(hba);
	}

out:
	if (ret) {
		hba->esi_sup = false;
		dev_err(hba->dev, "Failed to request Platform MSI %d\n", ret);
	} else {
		host->esi_enabled = true;
#ifndef CONFIG_MCQ_OHOS
		ret = ufsmcq_irq_cpuhp_setup (hba);
#endif
	}

	return ret;
}
#endif

static struct ufs_hw_queue *ufshcd_mcq_find_hwq(struct ufs_hba *hba, unsigned int task_tag, struct ufshcd_lrb *lrbp)
{
	u8 cur_cpu = raw_smp_processor_id();
	if (task_tag == DEV_CMD_QUERY_TAG)
		return &hba->uhq[DEV_CMD_QUEUE];
	else if (task_tag == SYNCCACHE_CMD_TAG)
		return &hba->uhq[SYNCCACHE_QUEUE];

#if defined (CONFIG_MAS_ORDER_PRESERVE) && !defined(CONFIG_MCQ_OHOS)
	if (lrbp->cmd && scsi_order(lrbp->cmd) && scsi_is_order_cmd(lrbp->cmd))
		return &hba->uhq[2 * num_possible_cpus() - 1];
#endif

#if defined (CONFIG_MAS_BLK) && !defined(CONFIG_MCQ_OHOS)
	if (lrbp->cmd && lrbp->cmd->request) {
		if (lrbp->cmd->request->mas_cmd_flags & REQ_VIP) {
			return &hba->uhq[REQ_VIP_QUEUE];
		}
		else if (lrbp->cmd->request->cmd_flags & REQ_FG_META) {
			return &hba->uhq[REQ_HIGH_PRIO_QUEUE];
		}
		else if (!(lrbp->cmd->request->cmd_flags & REQ_SYNC)) {
			return &hba->uhq[REQ_ASYNC_QUEUE];
		}
		else if (lrbp->cmd->cmnd[0] == READ_10) {
			return &hba->uhq[cur_cpu];
		}
		else {
			return &hba->uhq[cur_cpu + num_possible_cpus()];
		}
	}
#endif

#ifdef CONFIG_MCQ_OHOS
	return &hba->uhq[task_tag % (hba->nr_hw_queues - 2)];
#else
	return &hba->uhq[cur_cpu];
#endif
}

inline void ufsmcq_pack_assemble_fail_rollback(void *q)
{
	struct ufs_hw_queue *hwq = (struct ufs_hw_queue *)q;
	hwq->sq_tp_slot = (hwq->sq_tp_slot - 1 + hwq->max_entries) % hwq->max_entries;
	spin_unlock(&hwq->sq_lock);
}

int ufshcd_mcq_config_utrdlp(struct ufs_hba *hba, unsigned int task_tag, struct ufshcd_lrb *lrbp)
{
	struct ufs_hw_queue *hwq = NULL;
	struct hufs_utp_transfer_req_desc *utrdlp = NULL;
	dma_addr_t cmd_desc_addr;
	u16 prdt_offset;
	u16 response_offset;
	int utrd_size = sizeof(struct hufs_utp_transfer_req_desc);

	hwq = ufshcd_mcq_find_hwq(hba, task_tag, lrbp);
	lrbp->hwq = (void *)hwq;
	spin_lock(&hwq->sq_lock);
	utrdlp = (struct hufs_utp_transfer_req_desc *)(hwq->sqe_base_addr + (hwq->sq_tp_slot * utrd_size));
	ufshcd_mcq_update_sq_hp_slot(hwq);
	if (ufshcd_mcq_is_sq_full(hwq)) {
		pr_err("hwq submit entries is full, tag = %d, que id = %d\n", task_tag, hwq->id);
		return -EAGAIN;
	}

	response_offset =
		(u16)offsetof(struct utp_transfer_cmd_desc, response_upiu);
	prdt_offset =
		(u16)offsetof(struct utp_transfer_cmd_desc, prd_table);
#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
	if (task_tag == HI1861_VCMD_QUERY_TAG) {
		response_offset =
			(u16)offsetof(struct vcmd_utp_transfer_cmd_desc, response_upiu);
		prdt_offset =
			(u16)offsetof(struct vcmd_utp_transfer_cmd_desc, prd_table);
	}
#endif

	cmd_desc_addr = hba->ucdl_dma_addr + sizeof_utp_transfer_cmd_desc(hba) * task_tag;
	utrdlp->command_desc_base_addr_lo = cpu_to_le32(lower_32_bits(cmd_desc_addr));
	utrdlp->command_desc_base_addr_hi = cpu_to_le32(upper_32_bits(cmd_desc_addr));
#ifdef CONFIG_UFS_2DW_PRDT
		/* LDB 0x3ff -> 4k byte , CDS 4k granularity, 1 --> 4k */
		utrdlp->header.dword_2 =
			cpu_to_le32((0x3FF << 16) | (1 << 8) | OCS_INVALID_COMMAND_STATUS);
#endif
	ufshcd_hufs_update_upiu_prdt_offset(utrdlp, hba, response_offset, prdt_offset, task_tag);
	lrbp->hufs_utr_descriptor_ptr = utrdlp;
	return 0;
}

void ufshcd_mcq_send_queue(struct ufs_hba *hba, unsigned int task_tag, struct ufshcd_lrb *lrbp)
{
	struct ufs_hw_queue *hwq = NULL;
	unsigned long flags;

	hwq = (struct ufs_hw_queue *)(lrbp->hwq);
	ufsmcq_inflt_req_set(hba, (int)task_tag);
	/* inflt add must before low power vote, avoid h8 interrupt error disable mphy_clk */
#if defined(CONFIG_SCSI_UFS_LOW_POWER_VOTE)
	spin_lock_irqsave(hba->host->host_lock, flags);
	if ((lrbp->opcode == READ_10) || (lrbp->opcode == WRITE_10))
		hba->ufs_inflight_byte += lrbp->cmd->sdb.length;
	/* ccbus vote busy only for large inflight_byte */
	if (hba->ufs_inflight_byte >= hba->ufs_ccbus_threshold)
		ufs_dfa_ccbus_vote(hba, false);
	if (hba->ufs_inflight_byte <= UFS_CCBUS_LIMIT_IDLE)
		ufs_dfa_ccbus_vote(hba, true);
	if (hba->ufs_dfa_lp_voted) {
		ufs_low_power_vote(hba, false);
		hba->ufs_dfa_lp_voted = false;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
#elif defined(CONFIG_UFS_LOW_POWER_VOTE_STATUS_SYNC)
	ufs_send_cmd_status_sync(hba);
#endif

	ufshcd_ring_sqtp(hwq);
	spin_unlock(&hwq->sq_lock);
}

static inline unsigned int ufshcd_mcq_get_tag(struct ufs_hba *hba, struct ufs_hw_queue *hwq,
				     struct cq_entry *cqe)
{
	dma_addr_t dma_addr;

	dma_addr = ((u64)le32_to_cpu(cqe->command_desc_base_addr_hi) << 32) |
		   (le32_to_cpu(cqe->command_desc_base_addr_lo) & 0xffffff80);
	if (unlikely(dma_addr < hba->ucdl_dma_addr)) {
		dev_err(hba->dev, "[UFS] get tag err:%u - %u\n", dma_addr, hba->ucdl_dma_addr);
		return hba->nutrs;
	}
	return (dma_addr - hba->ucdl_dma_addr) /
		sizeof_utp_transfer_cmd_desc(hba);
}

static void ufshcd_mcq_update_ocs_for_utrd(struct ufs_hba *hba, int task_tag, struct cq_entry *cqe)
{
	struct ufshcd_lrb *lrbp;

	lrbp = &hba->lrb[task_tag];
	lrbp->cq_ocs = le32_to_cpu(cqe->status) & MASK_OCS;
	if (lrbp->cq_ocs == OCS_INVALID_COMMAND_STATUS)
		dev_err(hba->dev, "ufs get invalid ocs, tag %d cdb %#02x\n",
				task_tag, lrbp->cmd && lrbp->cmd->cmnd ? lrbp->cmd->cmnd[0] : 0);
}

/* interrup context, need not lock */
static void ufshcd_mcq_transfer_req_compl(struct ufs_hba *hba, struct ufs_hw_queue *hwq)
{
	unsigned int task_tag;
	struct cq_entry *cqe;

	spin_lock(&hwq->cq_lock);
	ufshcd_mcq_update_cq_tp_slot(hwq);
	while (!ufshcd_mcq_is_cq_empty(hwq)) {
		cqe = ufshcd_mcq_cur_cqe(hwq);
		task_tag = ufshcd_mcq_get_tag(hba, hwq, cqe);
		if (task_tag >= hba->nutrs) {
			dev_err(hba->dev, "task_tag:%u - %x nutrs:%d hw_id:%u\n",
				task_tag, cqe->reserved[0], hba->nutrs, hwq->id);
			ufshcd_mcq_inc_cq_hp_slot(hwq);
			continue;
		}

		if (hba->lrb[task_tag].cmd && !hba->lrb[task_tag].req_send)
			dev_err(hba->dev, "%s: mcq request is not sent but completed, tag %d, CQIE:0x%x hw_id:%x, CQHP:0x%x, CQTP:0x%x\n",
				__func__, task_tag, readl(hwq->mcq_base_cqis + REG_CQIE), hwq->id,
				readl(hwq->mcq_base_cqd + REG_CQHP), readl(hwq->mcq_base_cqd + REG_CQTP));
		ufshcd_mcq_update_ocs_for_utrd(hba, task_tag, cqe);
		ufshcd_compl_one_task(hba, task_tag);
		ufshcd_mcq_inc_cq_hp_slot(hwq);
		ufsmcq_inflt_req_clear(hba, task_tag);
	}

	ufshcd_mcq_update_cq_hp(hwq);
	spin_unlock(&hwq->cq_lock);
	/* we might have free'd some tags above */
	wake_up(&hba->dev_cmd.tag_wq);
}

/**
 * ufshcd_handle_mcq_cq_events - handle MCQ completion queue events
 * @hba: per adapter instance
 *
 * Returns
 *  IRQ_HANDLED - If interrupt is valid
 *  IRQ_NONE    - If invalid interrupt
 */
irqreturn_t ufshcd_handle_mcq_cq_events(struct ufs_hba *hba)
{
	struct ufs_hw_queue *hwq;
	u32 events;
	u32 queue_num;

	for (queue_num = 0; queue_num < hba->nr_hw_queues; queue_num++) {
		hwq = &hba->uhq[queue_num];

		events = ufshcd_mcq_read_cqis(hba, queue_num);
		if (events)
			ufshcd_mcq_write_cqis(hba, events, queue_num);

		if (events & UFSHCD_MCQ_CQIS_TEPS)
			ufshcd_mcq_transfer_req_compl(hba, hwq);
	}

	return IRQ_HANDLED;
}

static void ufshcd_mcq_set_sq_entries_abort(struct ufs_hw_queue *hwq)
{
	int i = 0;
	struct hufs_utp_transfer_req_desc *cur_desc = NULL;

	ufshcd_mcq_update_sq_hp_slot(hwq);

	pr_err("%s hwq->sq_hp_slot: %d hwq->sq_tp_slot: %d hwq->max_entries: %d id: %d\n",
			__func__, hwq->sq_hp_slot, hwq->sq_tp_slot, hwq->max_entries, hwq->id);
	while (((hwq->sq_hp_slot + i) % hwq->max_entries) != hwq->sq_tp_slot) {
		cur_desc = (struct hufs_utp_transfer_req_desc *)hwq->sqe_base_addr + hwq->sq_hp_slot + i;
		cur_desc->header.dword_0 |= MCQ_SQ_ENTRY_ABORT;
		i++;
	}
}

static void ufshcd_mcq_check_stop_run_status(struct ufs_hw_queue *hwq, bool stop)
{
	u32 val;

	val = readl(hwq->mcq_base_sqd + REG_SQRTS);
	if (stop && (val & MCQ_SQ_STOP_BIT))
		return;

	if (!stop && ((val & MCQ_SQ_STOP_BIT) != MCQ_SQ_STOP_BIT))
		return;

	pr_err("%s mcq stop run err status sop: %d REG_SQRTS: %x\n", __func__, stop, val);
}

static void ufshcd_mcq_stop_run_sq(struct ufs_hw_queue *hwq, bool stop)
{
	u32 val;
	ktime_t timeout;

	val = readl(hwq->mcq_base_sqd + REG_SQRTC);
	if (stop)
		val |= MCQ_SQ_STOP_BIT;
	else
		val &= ~MCQ_SQ_STOP_BIT;
	writel(val, hwq->mcq_base_sqd + REG_SQRTC);

	timeout = ktime_add_ms(ktime_get(), 1000);
	while (1) {
		bool timedout = ktime_after(ktime_get(), timeout);

		val = readl(hwq->mcq_base_sqis + REG_SQIS);
		if (val & SQ_STOP_STATUS) {
			writel(val, hwq->mcq_base_sqis + REG_SQIS);
			ufshcd_mcq_check_stop_run_status(hwq, stop);
			return;
		}

		if (timedout) {
			pr_err("%s hw queue run stop timeout stop = %d hwq->id: %d\n", __func__, stop, hwq->id);
			return;
		}
		udelay(10);
	}
}

void ufshcd_mcq_cq_ie_enable(struct ufs_hba *hba, bool enable)
{
	int i;

	for (i = 0; i < hba->nr_hw_queues; i++) {
		if (enable)
			writel(1, hba->uhq[i].mcq_base_cqis + REG_CQIE);
		else
			writel(0, hba->uhq[i].mcq_base_cqis + REG_CQIE);
	}
}

void ufshcd_mcq_transfer_req_compl_all(struct ufs_hba *hba, bool force_compl)
{
	u32 i;
	struct ufs_hw_queue *hwq = NULL;
	struct ufshcd_lrb *lrbp = NULL;

	dev_err(hba->dev, "%s hba->mcq_inflt_reqs: %llx, force_comple:%d\n", __func__,
		atomic_read(&hba->mcq_inflt_reqs), force_compl);
	if (!atomic_read(&hba->mcq_inflt_reqs))
		return;
#ifdef CONFIG_DFX_DEBUG_FS
	if ((atomic_read(&hba->mcq_inflt_reqs) < 0) || (atomic_read(&hba->mcq_inflt_reqs) >= hba->nutrs)) {
		dev_err(hba->dev, "%s inflt_reqs: %d\n", __func__, atomic_read(&hba->mcq_inflt_reqs));
		BUG_ON(1);
	}
#endif

	ufshcd_mcq_cq_ie_enable(hba, false);
	mdelay(5);
	if (force_compl) {
		for_each_set_bit(i, ufshcd_outstanding_reqs(hba), hba->nutrs) {
			lrbp = &hba->lrb[i];
			hwq = (struct ufs_hw_queue *)(lrbp->hwq);
			spin_lock(&hwq->cq_lock);
			ufshcd_compl_one_task(hba, i);
			ufsmcq_inflt_req_clear(hba, i);
			spin_unlock(&hwq->cq_lock);
		}
	} else {
		for (i = 0; i < hba->nr_hw_queues; i++) {
			hwq = &hba->uhq[i];
			ufshcd_mcq_transfer_req_compl(hba, hwq);
		}
	}
	ufshcd_mcq_cq_ie_enable(hba, true);
}

static void ufshcd_enable_OoO_mode(struct ufs_hba *hba, bool enable)
{
	if (enable)
		ufshcd_writel(hba, ufshcd_readl(hba, UFS_PRDT_CONFIG) | HOST_CFG_OOO_EN , UFS_PRDT_CONFIG);
	else
		ufshcd_writel(hba, ufshcd_readl(hba, UFS_PRDT_CONFIG) & (~HOST_CFG_OOO_EN) , UFS_PRDT_CONFIG);
}

static void ufshcd_mcq_enable(struct ufs_hba *hba, bool enable)
{
	/* Selct MCQ mode */
	if (enable)
		ufshcd_writel(hba, ufshcd_readl(hba, REG_UFS_GCR_CFG) | QUEUE_TYPE_SELECT, REG_UFS_GCR_CFG);
	else
		ufshcd_writel(hba, ufshcd_readl(hba, REG_UFS_GCR_CFG) & (~QUEUE_TYPE_SELECT) , REG_UFS_GCR_CFG);
	hba->use_mcq = enable;
}
/**
 * ufshcd_mcq_config_mac_as - Set the #Max Activ Cmds & Arbitration scheme.
 * @hba - per adpater instance
 * @max_active_cmds - maximum # of active commands to the device at any time.
 * @as -Select the arbitration scheme to be use for the Multi-Circular Queue.
 *
 * The controller wouldn't send more than the max_active_cmds to the device at
 * any time.
 */
static void ufshcd_mcq_config_mac_as(struct ufs_hba *hba, u32 max_active_cmds, u32 as)
{
	u32 val;

	val = ((max_active_cmds << MCQ_CFG_MAC_OFFSET) & MCQ_CFG_MAC_MASK) |
		(as & MCQ_CFG_AS_MASK);
	ufshcd_writel(hba, val, REG_UFS_MCQ_CFG);
}

static int ufshcd_mcq_memory_alloc(struct ufs_hba *hba)
{
	struct ufs_hw_queue *hwq;
	size_t utrdl_size, cqe_size;
	int i;

	for (i = 0; i < hba->nr_hw_queues; i++) {
		hwq = &hba->uhq[i];

		utrdl_size = sizeof(struct hufs_utp_transfer_req_desc) *
			     hwq->max_entries;
		hwq->sqe_base_addr = dmam_alloc_coherent(hba->dev, utrdl_size,
							 &hwq->sqe_dma_addr,
							 GFP_KERNEL);
		if (!hwq->sqe_dma_addr) {
			dev_err(hba->dev, "SQE allocation failed\n");
			return -ENOMEM;
		}

		cqe_size = sizeof(struct cq_entry) * hwq->max_entries;
		hwq->cqe_base_addr = dmam_alloc_coherent(hba->dev, cqe_size,
							 &hwq->cqe_dma_addr,
							 GFP_KERNEL);
		if (!hwq->cqe_dma_addr) {
			dev_err(hba->dev, "CQE allocation failed\n");
			return -ENOMEM;
		}
	}

	return 0;
}

static void ufshcd_mcq_make_queues_operational(struct ufs_hba *hba)
{
	struct ufs_hw_queue *hwq;
	u16 sqsize;
	u16 cqsize;
	int i;
	unsigned int mcq_pionter;
	u8 priority;

	mcq_pionter = (hba->mcq_capabilities & QCFGPTR_MASK) >> QCFGPTR_OFFSET;
	for (i = 0; i < hba->nr_hw_queues; i++) {
		hwq = &hba->uhq[i];
		hwq->id = i;
		sqsize = hwq->max_entries * MCQ_SQ_ENTRY_SIZE_IN_DWORD - 1;
		cqsize = hwq->max_entries * MCQ_CQ_ENTRY_SIZE_IN_DWORD - 1;

		hwq->mcq_base = hba->mmio_base + mcq_pionter * MCQ_QCFGPTR_UNIT + i * MCQ_QCFG_SIZE;
		hwq->mcq_base_sqd = hba->mmio_base + readl(hwq->mcq_base + REG_SQDAO);
		hwq->mcq_base_sqis = hba->mmio_base + readl(hwq->mcq_base + REG_SQISAO);
		hwq->mcq_base_cqd = hba->mmio_base + readl(hwq->mcq_base + REG_CQDAO);
		hwq->mcq_base_cqis = hba->mmio_base + readl(hwq->mcq_base+ REG_CQISAO);

		/* Submission Queue Lower Base Address */
		writel(lower_32_bits(hwq->sqe_dma_addr), hwq->mcq_base + REG_SQLBA);
		/* Submission Queue Upper Base Address */
		writel(upper_32_bits(hwq->sqe_dma_addr), hwq->mcq_base + REG_SQUBA);

		/* Completion Queue Lower Base Address */
		writel(lower_32_bits(hwq->cqe_dma_addr), hwq->mcq_base + REG_CQLBA);
		/* Completion Queue Upper Base Address */
		writel(upper_32_bits(hwq->cqe_dma_addr), hwq->mcq_base + REG_CQUBA);

		/* Enable Tail Entry Push Status interrupt for queues */
		writel(1, hwq->mcq_base_cqis + REG_CQIE);

		/* Completion Queue Enable|Size to Completion Queue Attribute */
		writel((1 << QUEUE_EN_OFFSET) | cqsize, hwq->mcq_base + REG_CQATTR);

		/*
		 * Submission Qeueue Enable|Size|Completion Queue ID to
		 * Submission Queue Attribute
		 */
		if (i == DEV_CMD_QUEUE || i == SYNCCACHE_QUEUE)
			priority = 0;
#ifndef CONFIG_MCQ_OHOS
		else if (i == REQ_VIP_QUEUE)
			priority = 1;
		else if (i == REQ_HIGH_PRIO_QUEUE)
			priority = 2;
		else if (i == REQ_ASYNC_QUEUE)
			priority = 4;
#endif
		else
			priority = 3;
		writel((1 << QUEUE_EN_OFFSET) | priority << QUEUE_PRIORITY_OFFSET |
			sqsize | (i << QUEUE_ID_OFFSET), hwq->mcq_base + REG_SQATTR);
	}
}

static int ufshcd_mcq_init(struct ufs_hba *hba)
{
	int  i;
	struct ufs_hw_queue *hwq;

	hba->uhq = devm_kzalloc(hba->dev,
				hba->nr_hw_queues * sizeof(struct ufs_hw_queue),
				GFP_KERNEL);
	if (!hba->uhq) {
		dev_err(hba->dev, "ufs hw queue memory allocation failed\n");
		return -ENOMEM;
	}

	for (i = 0; i < hba->nr_hw_queues; i++) {
		hwq = &hba->uhq[i];
		hwq->max_entries = MQ_MAX_ENTRIES;
		spin_lock_init(&hwq->sq_lock);
		spin_lock_init(&hwq->cq_lock);
	}

	/* Give dev_cmd_queue the minimal num of entries */
	hba->uhq[DEV_CMD_QUEUE].max_entries = 2;
	hba->uhq[SYNCCACHE_QUEUE].max_entries = 2;

	ufshcd_enable_intr(hba, MCQ_CQ_EVENT_STATUS);

	return 0;
}

void ufshcd_init_mcq_hwq_slot(struct ufs_hba *hba)
{
	int  i;
	struct ufs_hw_queue *hwq;

	for (i = 0; i < hba->nr_hw_queues; i++) {
		hwq = &hba->uhq[i];
		hwq->sq_hp_slot = 0;
		hwq->sq_tp_slot = 0;
		hwq->cq_hp_slot = 0;
		hwq->cq_tp_slot = 0;
	}
	dev_err(hba->dev, "%s hwq slot init done\n", __func__);
}

int ufshcd_config_mcq(struct ufs_hba *hba, bool async)
{
	int ret = 0;
	u32 max_active_cmds;

#ifdef CONFIG_DFA_MCQ
	if (is_dfa_enabled(hba))
		return dfa_config_mcq(hba, async);
#endif

#ifdef CONFIG_LIBLINUX_UDK_UFS
	hba->nr_hw_queues = MCQ_CORE_QUEUE_NUM;
#else
	hba->nr_hw_queues = (hba->mcq_capabilities & MASK_HW_QUEUE_NUMBER) + 1;
#endif
	dev_err(hba->dev, "%s hba->nr_hw_queues = %d\n", __func__, hba->nr_hw_queues);

	if (hba->dev_info.bqueuedepth > 0)
		max_active_cmds = hba->dev_info.bqueuedepth - 1;
	else
		max_active_cmds = DEFAULT_ACTIVE_CMDS - 1;

	if (async) {
		ret = ufshcd_mcq_init(hba);
		if (ret) {
			dev_err(hba->dev, "MCQ init failed %d\n", ret);
			return ret;
		}

		ret = ufshcd_mcq_memory_alloc(hba);
		if (ret)
			return ret;
	}

	ufshcd_init_mcq_hwq_slot(hba);
#ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
	ret = ufshcd_mcq_config_esi(hba);
	if (ret) {
		dev_info(hba->dev, "MCQ ESI not available %d\n", ret);
		ret = 0;
	}
#endif

	ufshcd_enable_intr(hba, MCQ_CQ_EVENT_STATUS);
	ufshcd_mcq_make_queues_operational(hba);
	ufshcd_mcq_config_mac_as(hba, max_active_cmds, AS_STRICT_PRIORITY);
	ufshcd_mcq_enable(hba, true);

	if (hba->capabilities & MASK_OUT_OF_ORDER_DATA_DELIVERY_SUPPORT)
		ufshcd_enable_OoO_mode(hba, true);

	dev_err(hba->dev, "MCQ configured success, Global Config Register: 0x%x MCQ CONFIG: 0x%x\n",
			ufshcd_readl(hba, REG_UFS_GCR_CFG), ufshcd_readl(hba, REG_UFS_MCQ_CFG));
	return ret;
}

/**
 * ufshcd_mcq_sq_cleanup - Clean up submission queue resources
 * associated with the pending command.
 * @hba - per adapter instance.
 * @task_tag - The command's task tag.
 *
 * Returns 0 for success; error code otherwise.
 */
int ufshcd_mcq_sq_cleanup(struct ufs_hba *hba, int task_tag)
{
	struct ufshcd_lrb *lrbp = &hba->lrb[task_tag];
	struct ufs_hw_queue *hwq;
	u32 nexus;
	u32 val;
	int err = 0;
	ktime_t timeout;
	int i;

	hwq = (struct ufs_hw_queue *)(lrbp->hwq);

	/* stop all the SQs fetching before working on it */
	for (i = 0; i < hba->nr_hw_queues; i++)
		ufshcd_mcq_stop_run_sq(&hba->uhq[i], true);

	/* SQCTI = EXT_IID, IID, LUN, Task Tag */
	nexus = lrbp->lun << 8 | task_tag;
	writel(nexus, hwq->mcq_base_sqd + REG_SQCTI);

	/* SQRTCy.ICU = 1 */
	val = readl(hwq->mcq_base_sqd + REG_SQRTC);
	val |= INITIATE_CLEAN_UP_ICU;
	writel(val, hwq->mcq_base_sqd + REG_SQRTC);
	/* Poll SQRTSy.CUS = 1. Return result from SQRTSy.RTC */
	timeout = ktime_add_ms(ktime_get(), 1000);
	while (1) {
		bool timedout = ktime_after(ktime_get(), timeout);

		val = readl(hwq->mcq_base_sqis + REG_SQIS);
		if (val & SQ_CLEAN_UP_INTR_SUS) {
			writel(val, hwq->mcq_base_sqis + REG_SQIS);
			break;
		}

		if (timedout) {
			pr_err("%s val:0x%x  hwq->id: %d, REG_SQRTC:0x%x\n",
					__func__, val, hwq->id, readl(hwq->mcq_base_sqd + REG_SQRTC));
			err = -ETIMEDOUT;
			break;
		}
		udelay(10);
	}

	val = readl(hwq->mcq_base_sqd + REG_SQRTS);
	dev_err(hba->dev, "%s: clean up result REG_SQRTS: 0x%x\n", __func__, val);
	if (((val & SQ_CLEAN_UP_STATUS_CUS) == 0) || (val & CLEANUP_COMMAND_RETRUN_CODE) != 0)
		err = FAILED;

	udelay(5);

	/* Poll UFS_TRP_DFX3(0x129C) bit8 = 0 */
	timeout = ktime_add_ms(ktime_get(), 1);
	while (1) {
		bool timedout = ktime_after(ktime_get(), timeout);

		val = ufshcd_readl(hba, UFS_TRP_DFX3);
		if ((val & TRP_DFX3_CLEAR_FINISH_REQ) == 0)
			break;

		if (timedout) {
			pr_err("%s UFS_TRP_DFX3:0x%x\n", __func__, ufshcd_readl(hba, UFS_TRP_DFX3));
			break;
		}
	}

	for (i = 0; i < hba->nr_hw_queues; i++)
		ufshcd_mcq_stop_run_sq(&hba->uhq[i], false);

	return err;
}
/**
 * ufshcd_mcq_sqe_search - Search for the command in the submission queue
 * If the command is in the submission queue and not issued to the device yet,
 * nullify the sqe so the host controller will skip fetching the sqe.
 *
 * @hba - per adapter instance.
 * @hwq - Hardware Queue to be searched.
 * @task_tag - The command's task tag.
 *
 * Returns true if the SQE containing the command is present in the SQ
 * (not fetched by the controller); returns false if the SQE is not in the SQ.
 */
static bool ufshcd_mcq_find_set_entry_abort(struct ufs_hba *hba,
				  struct ufs_hw_queue *hwq, int task_tag)
{
	struct ufshcd_lrb *lrbp = &hba->lrb[task_tag];
	struct hufs_utp_transfer_req_desc *cur_utrd = NULL;
	struct hufs_utp_transfer_req_desc *abort_utrd = NULL;
	u32 mask = hwq->max_entries - 1;
	bool ret = false;
	int i = 0;

	ufshcd_mcq_stop_run_sq(hwq, true);

	spin_lock(&hwq->sq_lock);
	ufshcd_mcq_update_sq_hp_slot(hwq);
	pr_err("%s hwq->sq_hp_slot: %d hwq->sq_tp_slot: %d hwq->max_entries: %d id: %d\n",
			__func__, hwq->sq_hp_slot, hwq->sq_tp_slot, hwq->max_entries, hwq->id);

	abort_utrd = lrbp->hufs_utr_descriptor_ptr;
	while (((hwq->sq_hp_slot + i) % hwq->max_entries) != hwq->sq_tp_slot) {
		cur_utrd = (struct hufs_utp_transfer_req_desc *)hwq->sqe_base_addr + hwq->sq_hp_slot + i;
		if ((cur_utrd->command_desc_base_addr_lo == abort_utrd->command_desc_base_addr_lo) &&
			(cur_utrd->command_desc_base_addr_hi == abort_utrd->command_desc_base_addr_hi)) {
			cur_utrd->header.dword_0 |= MCQ_SQ_ENTRY_ABORT;
			ret = true;
			goto out;
		}
		i++;
	}

out:
	spin_unlock(&hwq->sq_lock);
	ufshcd_mcq_stop_run_sq(hwq, false);
	return ret;
}

/**
 * ufshcd_mcq_abort - Abort the command in MCQ.
 * @cmd - The command to be aborted.
 *
 * Returns SUCCESS or FAILED error codes
 */
int ufshcd_mcq_abort(struct scsi_cmnd *cmd)
{
	struct Scsi_Host *host = cmd->device->host;
	struct ufs_hba *hba = shost_priv(host);
	int tag = ufshcd_get_tag_from_cmd(host, hba, cmd);;
	struct ufshcd_lrb *lrbp = &hba->lrb[tag];
	struct ufs_hw_queue *hwq;
	int err = FAILED;

	/* Skip task abort in case previous aborts failed and report failure */
	if (lrbp->req_abort_skip) {
		dev_err(hba->dev, "%s: skip abort. tag %d failed earlier\n",
			__func__, tag);
		goto out;
	}

	hwq = (struct ufs_hw_queue *)lrbp->hwq;
	if (ufshcd_mcq_find_set_entry_abort(hba, hwq, tag)) {
		/*
		 * Failure. The command should not be "stuck" in SQ for
		 * a long time which resulted in command being aborted.
		 */
		dev_err(hba->dev, "%s: cmd found in sq. hwq=%d, tag=%d\n",
			__func__, hwq->id, tag);
		goto out;
	}

	/*
	 * The command is not in the submission queue, and it is not
	 * in the completion queue either. Query the device to see if
	 * the command is being processed in the device.
	 */
	if (ufshcd_try_to_abort_task(hba, tag)) {
		dev_err(hba->dev, "%s: device abort failed %d\n", __func__, err);
		lrbp->req_abort_skip = true;
		goto out;
	}

	err = SUCCESS;

out:
	return err;
}

int ufsmcq_abort(struct scsi_cmnd *cmd)
{
	struct Scsi_Host *host;
	struct ufs_hba *hba;
	unsigned long flags;
	unsigned int tag;
	int err = 0;
	int poll_cnt;
	u8 resp = 0xF;
	struct ufshcd_lrb *lrbp;
	struct ufs_hw_queue *hwq = NULL;

	host = cmd->device->host;
	hba = shost_priv(host);

	tag = ufshcd_get_tag_from_cmd(host, hba, cmd);
	lrbp = &hba->lrb[tag];
	hwq = (struct ufs_hw_queue *)(lrbp->hwq);
	ufshcd_update_error_stats(hba, UFS_ERR_TASK_ABORT);

	/*
	 * Task abort to the device W-LUN is illegal. When this command
	 * will fail, due to spec violation, scsi err handling next step
	 * will be to send LU reset which, again, is a spec violation.
	 * To avoid these unnecessary/illegal step we skip to the last error
	 * handling stage: reset and restore.
	 */
	if (lrbp->lun == UFS_UPIU_UFS_DEVICE_WLUN)
		return host->hostt->eh_host_reset_handler(cmd);

	ufshcd_update_error_stats(hba, UFS_ERR_TASK_ABORT);

	/* If command is already aborted/completed, return SUCCESS */
	if (!(test_bit(mcq_tag_off(tag), mcq_tag_bitmap(hba, tag)))) {
		dev_err(hba->dev,
			"%s: cmd at tag %d already completed, outstanding=0x%lx\n",
			__func__, tag, hba->mcq_outstanding_reqs[mcq_tag_idx(tag)]);
		goto out;
	}

	/* Print Transfer Request of aborted task */
	dev_err(hba->dev, "%s: Device abort task at tag=%d, hwq=%d\n", __func__, tag, hwq->id);
	/*
	 * Print detailed info about aborted request.
	 * As more than one request might get aborted at the same time,
	 * print full information only for the first aborted request in order
	 * to reduce repeated printouts. For other aborted requests only print
	 * basic details.
	 */
	scsi_print_command(hba->lrb[tag].cmd);
	if (!hba->req_abort_count) {
		ufshcd_print_host_regs(hba);
		ufshcd_print_host_state(hba);
		ufshcd_print_pwr_info(hba);
	}
	ufshcd_print_trs(hba, 1UL << tag, !hba->req_abort_count);

	hba->req_abort_count++;

	/* MCQ mode. Branch off to handle abort for mcq mode */
	err = ufshcd_mcq_abort(cmd);
	if (err)
		goto out;

	scsi_dma_unmap(cmd);
	hba->lrb[tag].cmd = NULL;
	ufsmcq_inflt_req_clear(hba, tag);
	ufstt_unprep_handler(hba, lrbp, ktime_get());
	wake_up(&hba->dev_cmd.tag_wq);

out:
	if (!err) {
		err = SUCCESS;
	} else {
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, err);
		for_each_set_bit(tag, ufshcd_outstanding_reqs(hba), hba->nutrs) {
			lrbp = &hba->lrb[tag];
			lrbp->req_abort_skip = true;
		}
		err = FAILED;
	}

	return err;
}

void ufsmcq_send_command(struct ufs_hba *hba, unsigned int task_tag)
{
	struct ufshcd_lrb *lrbp = &hba->lrb[task_tag];

	lrbp->issue_time_stamp = ktime_get();
	lrbp->compl_time_stamp = ktime_set(0, 0);
	lrbp->opcode = 0;
	lrbp->req_tag = 0xFF;
	lrbp->req_send = true;
	lrbp->cq_ocs = OCS_INVALID_COMMAND_STATUS;
	if (lrbp->cmd) {
		lrbp->opcode = lrbp->cmd->cmnd[0];
		if (lrbp->cmd->request)
			lrbp->req_tag = lrbp->cmd->request->tag;
	}

#ifdef CONFIG_RPMB_UFS
		if (lrbp->lun == UFS_UPIU_RPMB_WLUN)
			g_rpmb_ufs_start_time = dfx_getcurtime();
#endif

#ifndef CONFIG_MCQ_OHOS
	trace_android_vh_ufs_send_command(hba, lrbp);
	ufshcd_add_command_trace(hba, task_tag, "send");
#endif

#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
	iomt_host_latency_cmd_start(lrbp->cmd, lrbp->issue_time_stamp);
#endif

	ufshcd_mcq_send_queue(hba, task_tag, lrbp);

	wmb();
	ufshcd_update_tag_stats(hba, task_tag);
}

int ufsmcq_queuecommand(struct Scsi_Host *host, struct scsi_cmnd *cmd)
{
	struct ufshcd_lrb *lrbp;
	struct ufs_hba *hba;
	unsigned long flags;
	int tag;
	int err = 0;

	hba = shost_priv(host);
	cmd->tag = (unsigned char)cmd->request->tag;
	tag = (int)cmd->tag;

#ifndef CONFIG_MCQ_OHOS
	err = fscrypt_lld_protect(cmd->request);
	if (unlikely(err))
		return err;
#endif

	switch (hba->ufshcd_state) {
	case UFSHCD_STATE_OPERATIONAL:
		break;
	case UFSHCD_STATE_FFU:
		if (cmd->cmnd[0] != WRITE_BUFFER ||
			(cmd->cmnd[1] & WB_MODE_MASK) != DOWNLOAD_MODE) {
			dev_dbg(hba->dev, "%s: ffu in progress\n", __func__);
			return SCSI_MLQUEUE_HOST_BUSY;
		}
		break;
	case UFSHCD_STATE_EH_SCHEDULED:
	case UFSHCD_STATE_RESET:
		return SCSI_MLQUEUE_HOST_BUSY;
	case UFSHCD_STATE_ERROR:
		set_host_byte(cmd, DID_ERROR);
		cmd->scsi_done(cmd);
		return err;
	default:
		dev_WARN_ONCE(hba->dev, 1, "%s: invalid state %d\n",
			__func__, hba->ufshcd_state); /*lint !e146 !e665*/
		set_host_byte(cmd, DID_BAD_TARGET);
		cmd->scsi_done(cmd);
		return err;
	}

	if (unlikely(cmd->cmnd[0] == WRITE_BUFFER &&
			    (cmd->cmnd[1] & WB_MODE_MASK) == DOWNLOAD_MODE)) {
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->ufshcd_state = UFSHCD_STATE_FFU;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		/* wait for ufs all complete timeout time 2s */
		err = ufsmcq_check_inflt_reqs(hba, 2000);
		dev_err(hba->dev, "%s: FFU process, wait all complete:%d", __func__, err);
	}

	/* if error handling is in progress, don't issue commands */
	if (ufshcd_eh_in_progress(hba))
		return SCSI_MLQUEUE_HOST_BUSY;

	if (hba->ufshcd_state == UFSHCD_STATE_RESET ||
		hba->ufshcd_state == UFSHCD_STATE_EH_SCHEDULED ||
		hba->force_reset) {
		dev_err(hba->dev, "%s: reset in progress\n", __func__);
		return SCSI_MLQUEUE_HOST_BUSY;
	}

	err = pm_runtime_get_sync(hba->dev);
	if (err < 0) {
		pm_runtime_put_sync(hba->dev);
		return SCSI_MLQUEUE_HOST_BUSY;
	}

	WARN_ON(hba->is_hibernate);
	lrbp = &hba->lrb[tag];
	lrbp->cmd = cmd;
	lrbp->sense_bufflen = UFSHCD_REQ_SENSE_SIZE;
	lrbp->sense_buffer = cmd->sense_buffer;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->lun = ufshcd_scsi_to_upiu_lun(cmd->device->lun);
	lrbp->intr_cmd = !ufshcd_is_intr_aggr_allowed(hba) ? true : false;
	lrbp->req_abort_skip = false;
	lrbp->req_send = false;
	ufshcd_start_delay_work(hba, cmd);

	err = ufshcd_mcq_config_utrdlp(hba, tag, lrbp);
	if (err) {
		lrbp->cmd = NULL;
		goto out_unlock;
	}

	err = ufshcd_comp_scsi_upiu(hba, lrbp);
	if (err) {
		lrbp->cmd = NULL;
		goto out_unlock;
	}

	err = ufshcd_map_sg(hba, lrbp);
	if (err) {
		lrbp->cmd = NULL;
		goto out_unlock;
	}
	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();

#if defined(CONFIG_MAS_ORDER_PRESERVE) || defined(CONFIG_MAS_UNISTORE_PRESERVE)
	/* it has to be put here, without any goto branches behind */
	if (ufshcd_custom_upiu(lrbp->ucd_req_ptr, lrbp->cmd->request, lrbp->cmd, hba)) {
		err = SCSI_MLQUEUE_HOST_BUSY;
		pm_runtime_put_sync(hba->dev);
		goto out_unlock;
	}
#endif

	if ((unlikely((cmd->cmnd[0] == WRITE_BUFFER) &&
			((cmd->cmnd[1] & WB_MODE_MASK) == DOWNLOAD_MODE))) ||
			(cmd->cmnd[0] == UNMAP)) {
		spin_lock_irqsave(hba->host->host_lock, flags);
		ufshcd_disable_dev_tmt_cnt_no_irq(hba);
		spin_unlock_irqrestore(hba->host->host_lock, flags);
	}

	ufsmcq_send_command(hba, tag);
	goto out;

out_unlock:
	ufsmcq_pack_assemble_fail_rollback(lrbp->hwq);
out:
	ufstt_unprep_fn(hba, lrbp);
	return err;
}

bool ufsmcq_get_dev_cmd_tag(struct ufs_hba *hba, int *tag_out)
{
	if (test_and_set_bit_lock(mcq_tag_off(DEV_CMD_QUERY_TAG), mcq_tag_bitmap(hba, DEV_CMD_QUERY_TAG)))
		return false;

	*tag_out = DEV_CMD_QUERY_TAG;
	atomic_inc(&hba->mcq_inflt_reqs);
	return true;
}

void ufsmcq_put_dev_cmd_tag(struct ufs_hba *hba, int tag)
{
	clear_bit_unlock(mcq_tag_off(DEV_CMD_QUERY_TAG), mcq_tag_bitmap(hba, DEV_CMD_QUERY_TAG));
	atomic_dec(&hba->mcq_inflt_reqs);
}

int __ufsmcq_queuecommand_directly(struct ufs_hba *hba, struct scsi_cmnd *cmd, unsigned int timeout)
{
	int err;
	int tag;
	unsigned long flags;
	unsigned long time_left;
	struct ufshcd_lrb *lrbp = NULL;
	struct completion wait;

	wait_event(hba->dev_cmd.tag_wq, ufsmcq_get_dev_cmd_tag(hba, &tag));
	pm_runtime_get_sync(hba->dev);

	init_completion(&wait);
	hba->dev_cmd.complete = &wait;

	lrbp = &hba->lrb[tag];
	WARN_ON(lrbp->cmd); /*lint !e146 !e665*/
	lrbp->cmd = cmd;
	lrbp->sense_buffer = cmd->sense_buffer;
	lrbp->sense_bufflen = SCSI_SENSE_BUFFERSIZE;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->req_send = false;
	lrbp->lun = ufshcd_scsi_to_upiu_lun((unsigned int)cmd->device->lun);
	lrbp->intr_cmd = !ufshcd_is_intr_aggr_allowed(hba) ? true : false;
	lrbp->command_type = UTP_CMD_TYPE_SCSI;
	err = ufshcd_mcq_config_utrdlp(hba, tag, lrbp);
	if (err)
		goto unlock;

	/* form UPIU before issuing the command */
	err = ufshcd_compose_upiu(hba, lrbp);
	if (err)
		goto unlock;

	/* Black Magic, dont touch unless you want a BUG */
	lrbp->command_type = UTP_CMD_TYPE_DEV_MANAGE;
	err = ufshcd_map_sg(hba, lrbp);
	if (err)
		goto unlock;

	wmb();
	ufsmcq_send_command(hba, (unsigned int)tag);

	time_left = wait_for_completion_timeout(hba->dev_cmd.complete,
						msecs_to_jiffies(timeout));
	hba->dev_cmd.complete = NULL;

	if (likely(time_left)) {
		err = ufshcd_transfer_rsp_status(hba, lrbp, false);
		if (err)
			dev_err(hba->dev, "SCSI command result err = 0x%x\n", err);
	} else {
		err = -ETIMEDOUT;
		dev_err(hba->dev, "%s: scsi request timedout, tag %d\n", __func__, lrbp->task_tag);
		if (!ufshcd_clear_cmd(hba, lrbp->task_tag)) {
			err = -EAGAIN;
		} else {
			hba->ufs_dm_timeout++;
			lrbp->command_type = UTP_CMD_TYPE_DM_TIMEOUT;
		}
	}

	goto out;

unlock:
	ufsmcq_pack_assemble_fail_rollback(lrbp->hwq);
out:
	lrbp->cmd = NULL;
	ufsmcq_put_dev_cmd_tag(hba, tag);
	wake_up(&hba->dev_cmd.tag_wq);
	pm_runtime_put_sync(hba->dev);
	return err;
}

int ufsmcq_exec_dev_cmd(struct ufs_hba *hba, enum dev_cmd_type cmd_type, int timeout)
{
	struct ufshcd_lrb *lrbp;
	int err;
	int tag;
	struct completion wait;
	unsigned long flags;
	unsigned long time_left;

	wait_event(hba->dev_cmd.tag_wq, ufsmcq_get_dev_cmd_tag(hba, &tag));

	pm_runtime_get_sync(hba->dev);

	init_completion(&wait);
	hba->dev_cmd.complete = &wait;

	lrbp = &hba->lrb[tag];
	WARN_ON(lrbp->cmd);

	lrbp->cmd = NULL;
	lrbp->sense_bufflen = 0;
	lrbp->sense_buffer = NULL;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->req_send = false;
	lrbp->lun = 0; /* device management cmd is not specific to any LUN */
	lrbp->intr_cmd = true; /* No interrupt aggregation */
	ufshcd_prepare_lrbp_crypto(NULL, lrbp);
	hba->dev_cmd.type = cmd_type;
	/* some 186x vcmd need update lun */
	ufshcd_update_dev_cmd_lun(hba, lrbp);
	err = ufshcd_mcq_config_utrdlp(hba, tag, lrbp);
	if (unlikely(err))
		goto unlock;
	err = ufshcd_compose_devman_upiu(hba, lrbp);
	if (unlikely(err))
		goto unlock;

	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();

	ufsmcq_send_command(hba, tag);
	time_left = wait_for_completion_timeout(hba->dev_cmd.complete,
			msecs_to_jiffies(timeout));
	hba->dev_cmd.complete = NULL;
	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();
	if (likely(time_left)) {
		err = ufshcd_get_tr_ocs(lrbp);
		if (!err)
			err = ufshcd_dev_cmd_completion(hba, lrbp);
	} else {
		err = -ETIMEDOUT;
		dev_err(hba->dev, "%s: dev_cmd request timedout, tag %d\n", __func__, lrbp->task_tag);
		if (!ufshcd_clear_cmd(hba, lrbp->task_tag)) {
			/* successfully cleared the command, retry if needed */
			err = -EAGAIN;
		} else {
			hba->ufs_dm_timeout++;
			lrbp->command_type = UTP_CMD_TYPE_DM_TIMEOUT;
		}
	}

	goto out;

unlock:
	ufsmcq_pack_assemble_fail_rollback(lrbp->hwq);
out:
	ufsmcq_put_dev_cmd_tag(hba, tag);
	wake_up(&hba->dev_cmd.tag_wq);
	pm_runtime_mark_last_busy(hba->dev);
	pm_runtime_put_autosuspend(hba->dev);
	return err;
}
