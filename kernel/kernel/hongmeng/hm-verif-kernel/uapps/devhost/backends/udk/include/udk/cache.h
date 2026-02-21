/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of cache in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:00:09 2019
 */
#ifndef	__UDK_FLUSH_CACHE_H__
#define	__UDK_FLUSH_CACHE_H__

extern unsigned long fusion_flush_cache_range_by_pa(unsigned long start, unsigned long end,
						    unsigned int cmd);
void udk_flush_dcache_range(unsigned long start,
			    unsigned long end);
void udk_dma_sync_cpu_to_dev(unsigned long start,
			     unsigned long end);
void udk_dma_sync_dev_to_cpu(unsigned long start,
			     unsigned long end);
void udk_dma_sync_cpu_to_dev_by_pa(unsigned long start,
				   unsigned long end);
void udk_dma_sync_dev_to_cpu_by_pa(unsigned long start,
				   unsigned long end);
void udk_flush_cache_all(void);

#endif /* __UDK_FLUSH_CACHE_H__ */
