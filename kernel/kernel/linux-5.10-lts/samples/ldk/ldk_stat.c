// SPDX-License-Identifier: GPL-2.0
#include <asm/page.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/types.h>
#include <linux/atomic.h>
#include <liblinux/pal.h>
#include <trace/hooks/liblinux.h>

#include "ldk_stat.h"

/* note dh/ldk use HUGETLB, need sysmgr handle map offset */
raw_stat_t g_ldk_stat __attribute__((aligned(PAGE_SIZE))) = {
	.mem_stat = {
		    .dmaheap_free_pages = ATOMIC_LONG_INIT(0),
		    .buddy_dma_free_pages = ATOMIC_LONG_INIT(0),
		    .system_free_pages = ATOMIC_LONG_INIT(0),
	},
	.sys_mem_stat = {
			.avail_buf_wmark_min = ATOMIC_LONG_INIT(0),
			.avail_buf_wmark_mid = ATOMIC_LONG_INIT(0),
			.avail_buf_wmark_high = ATOMIC_LONG_INIT(0),
			.curr_avail_buf = ATOMIC_LONG_INIT(0),
	},
};

static bool g_is_ldk_stat_registered = false;

void liblinux_set_ldk_stat_registered(void)
{
	g_is_ldk_stat_registered = true;
}

bool liblinux_is_ldk_stat_registered(void)
{
	return g_is_ldk_stat_registered;
}

static inline struct zone *get_zone_by_type(enum zone_type type)
{
	pg_data_t *pgdat = first_online_pgdat();
	return &pgdat->node_zones[type];
}

void liblinux_set_buddy_dma_free_pages(void)
{
	if (likely(g_is_ldk_stat_registered == true)) {
		/* now only dma zone is used and we just want free pages from NR_FREE_PAGES
		 * without watermark, so just set the value.
		 */
		struct zone *zone = get_zone_by_type(ZONE_DMA);
		unsigned long free_pages = global_zone_page_state(NR_FREE_PAGES);
		if (free_pages > high_wmark_pages(zone))
			atomic_long_set(&g_ldk_stat.mem_stat.buddy_dma_free_pages,
					free_pages - high_wmark_pages(zone));
	}
}
EXPORT_SYMBOL(liblinux_set_buddy_dma_free_pages);

void liblinux_mod_dmaheap_free_pages(long val)
{
	if (likely(g_is_ldk_stat_registered == true))
		atomic_long_add(val, &g_ldk_stat.mem_stat.dmaheap_free_pages);
}
EXPORT_SYMBOL(liblinux_mod_dmaheap_free_pages);

long liblinux_get_system_free_pages(void)
{
	long system_free_pages = 0;
	if (likely(g_is_ldk_stat_registered == true)) {
		/* get sys mem free paged from system */
		system_free_pages = atomic_long_read(&g_ldk_stat.mem_stat.system_free_pages);
	}
	return system_free_pages;
}
EXPORT_SYMBOL(liblinux_get_system_free_pages);

void *liblinux_get_ldk_stat_common(void)
{
	return &g_ldk_stat;
}

size_t liblinux_get_ldk_stat_size(void)
{
	return sizeof(g_ldk_stat);
}

static void hook_buddy_dma_free_pages_set(void *data, int item)
{
	liblinux_global_free_pages_set(item);
}
INIT_VENDOR_HOOK(ldk_rvh_buddy_dma_free_pages_set, hook_buddy_dma_free_pages_set);

unsigned long liblinux_get_sys_avail_buf_wmark_min(void)
{
	return (unsigned long)atomic_long_read(&g_ldk_stat.sys_mem_stat.avail_buf_wmark_min);
}

unsigned long liblinux_get_sys_avail_buf_wmark_mid(void)
{
	return (unsigned long)atomic_long_read(&g_ldk_stat.sys_mem_stat.avail_buf_wmark_mid);
}

unsigned long liblinux_get_sys_avail_buf_wmark_high(void)
{
	return (unsigned long)atomic_long_read(&g_ldk_stat.sys_mem_stat.avail_buf_wmark_high);
}

unsigned long liblinux_get_sys_curr_avail_buf(void)
{
	return (unsigned long)atomic_long_read(&g_ldk_stat.sys_mem_stat.curr_avail_buf);
}
