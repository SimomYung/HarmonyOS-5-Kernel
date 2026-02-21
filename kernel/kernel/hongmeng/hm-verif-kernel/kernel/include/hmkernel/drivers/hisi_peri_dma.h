/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: General interfaces and structs of uart
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 01:29:43 2018
 */
#ifndef HMKERNEL_HISI_PERI_DMA_H
#define HMKERNEL_HISI_PERI_DMA_H

#include <lib/dlist.h>
#include <hmkernel/interrupt/kvic.h>

#define PERI_DMA_WRITE_INFO_MAX 8
struct peri_dma_write_info {
	unsigned int val[PERI_DMA_WRITE_INFO_MAX];
	unsigned int offset[PERI_DMA_WRITE_INFO_MAX];
	unsigned int count;
};

void hm_peri_dma_write(struct peri_dma_write_info *info);

#endif
