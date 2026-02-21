/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LDK_MEM_INFO_H
#define __LDK_MEM_INFO_H

#include <linux/types.h>

struct liblinux_mem_info {
	unsigned long long ion_total_cache;
	unsigned long long ion_total_used;
	unsigned long dmaheap_total_cache;
	unsigned long dmaheap_total_used;
	unsigned long dmaheap_freelist_cache;
	unsigned long gpu_total_used;
	unsigned long vmalloc_total;
	unsigned long vmalloc_used;
	unsigned long long slab_total;
	unsigned long long slab_unreclaimable;
	unsigned long long slab_reclaimable;
	unsigned long rsv_total_used;
	unsigned long buddy_total;
	unsigned long buddy_normal_total;
	unsigned long buddy_normal_free;
	unsigned long buddy_dma_total;
	unsigned long buddy_dma_free;
	unsigned long ldk_others_total;
	unsigned long discretepool_used;
	unsigned long discretepool_free;
	unsigned long vmemmap_rsv_total;
	unsigned long long hisp_total_used;
	unsigned long long vcodec_total_used;
};
unsigned long hvgr_mem_get_gpu_memory_byte_by_pid(uint32_t pid);
#endif /* __LDK_MEM_INFO_H */
