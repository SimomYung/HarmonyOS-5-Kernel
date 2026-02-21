/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_PAGE_ALLOC_H__
#define __LIBLINUX_PAGE_ALLOC_H__

#include <liblinux/page.h>
#include <asm/pgtable.h>

int page_pool_init(struct liblinux_page_allocator **main_pool);

struct liblinux_page_allocator *default_page_allocator(void);

extern unsigned int dma_zone_only;

void *__alloc_pages_pool_exact(gfp_t gfp_mask, unsigned long size);

void memmap_init_zone_range(struct zone *zone,
			    unsigned long start_pfn,
			    unsigned long end_pfn);

extern unsigned int dmabuf_page_pool_reclaim(unsigned int order, struct page **reclaimed_pages,
					     unsigned int len);
extern unsigned long dmaheap_waterline;
extern atomic_long_t g_native_gfp_alloc;
extern atomic_long_t g_native_gfp_free;
#endif /* __LIBLINUX_PAGE_ALLOC_H__ */
