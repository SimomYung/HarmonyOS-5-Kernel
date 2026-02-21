/* Copyright (c) Hisilicon Technologies Co., Ltd. 2001-2021. All rights reserved.
 * FileName: mm_dmaheap_alloc_debug.h
 * Description: This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;
 * either version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef _PERI_DMA_H
#define _PERI_DMA_H
#include <linux/dmaengine.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#ifdef CONFIG_LIBLINUX_PERI_DMA_USE_HRTIMER
#include <linux/hrtimer.h>
#endif

struct peri_dma_dev {
	struct dma_device slave;
	void __iomem *base;
	struct tasklet_struct task;
	spinlock_t lock;
	struct list_head chan_pending;
	struct peri_dma_phy *phy;
	struct peri_dma_chan *chans;
	struct clk *clk;
	u32 dma_channels;
	u32 dma_requests;
	u32 dma_min_chan;
	u32 dma_used_chans;
	u32 dma_cpuid;
	int dma_irq;
	bool dma_share;
	u32 dma_pipeline_en;
	u32 dma_smmu;
	u32 tbu_detect_irq;
	u32 tbu_multpl_irq;
	void __iomem *tbu_base;
	bool dma_irq_flag;
#ifdef CONFIG_LIBLINUX_PERI_DMA_USE_HRTIMER
	struct hrtimer dma_timer;
#endif
};

void peri_dma_smmu_init(struct platform_device *op, struct peri_dma_dev *d);
void dma_smmu_suspend(struct platform_device *op, struct peri_dma_dev *d);
void dma_smmu_resume(struct platform_device *op, struct peri_dma_dev *d);
void dma_smmu_remove(struct platform_device *op, struct peri_dma_dev *d);

#endif
