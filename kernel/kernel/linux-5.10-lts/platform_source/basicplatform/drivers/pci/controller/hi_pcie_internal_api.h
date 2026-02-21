/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe internal api.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_INTERNAL_API_H
#define HI_PCIE_INTERNAL_API_H

#include <linux/types.h>

struct hi_pcie_dma_para {
	dma_addr_t src_addr;
	dma_addr_t dest_addr;
	u32 length;
	void *data;
	void (*callback)(void *data, int result);
};

#ifdef CONFIG_HIPCIE_CORE
int hi_pcie_config_l0sl1(u32 cdx, u32 pdx, u32 aspm);
int hi_pcie_config_l1ss(u32 cdx, u32 pdx, u32 aspm);
int hi_pcie_set_power_state(u32 cdx, u32 pdx, u32 state);
int hi_pcie_setup_outbound(u32 cdx, u32 index, u32 base, u32 target, u32 size);
int hi_pcie_setup_inbound(u32 cdx, u32 index, u32 base, u32 target, u32 size);
int hi_pcie_dma_read(u32 cdx, u32 pdx, u32 queue, struct hi_pcie_dma_para *para);
int hi_pcie_dma_write(u32 cdx, u32 pdx, u32 queue, struct hi_pcie_dma_para *para);
void hi_pcie_refclk_host_vote(u32 cdx, bool vote);
#else
static inline int hi_pcie_config_l0sl1(u32 cdx, u32 pdx, u32 aspm);
{
	return 0;
}

static inline int hi_pcie_config_l1ss(u32 cdx, u32 pdx, u32 aspm)
{
	return 0;
}

static inline int hi_pcie_set_power_state(u32 cdx, u32 pdx, u32 state)
{
	return 0;
}

static inline int hi_pcie_setup_outbound(u32 cdx, u32 index, u32 base, u32 target, u32 size)
{
	return;
}

static inline int hi_pcie_setup_inbound(u32 cdx, u32 index, u32 base, u32 target, u32 size)
{
	return;
}

int hi_pcie_dma_read(u32 cdx, u32 pdx, u32 queue, struct hi_pcie_dma_para *para)
{
	return 0;
}

int hi_pcie_dma_write(u32 cdx, u32 pdx, u32 queue, struct hi_pcie_dma_para *para)
{
	return 0;
}

static inline void hi_pcie_refclk_host_vote(u32 cdx, bool vote)
{
	return;
}
#endif
#endif
