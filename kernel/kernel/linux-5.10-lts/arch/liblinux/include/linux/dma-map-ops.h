/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIBLINUX_DMA_MAP_OPS_H
#define _LIBLINUX_DMA_MAP_OPS_H

#include_next <linux/dma-map-ops.h>

#ifdef CONFIG_LIBLINUX_USER_INVAL_CACHE
void liblinux_sync_dma_for_device(struct device *dev, phys_addr_t paddr, size_t size,
		enum dma_data_direction dir);
void liblinux_sync_dma_for_cpu(struct device *dev, phys_addr_t paddr, size_t size,
		enum dma_data_direction dir);
void liblinux_dma_map_area(struct device *dev, const void *addr, size_t size,
		enum dma_data_direction dir);
#endif

#endif /* _LIBLINUX_DMA_MAP_OPS_H */
