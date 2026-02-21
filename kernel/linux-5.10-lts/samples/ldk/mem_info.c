// SPDX-License-Identifier: GPL-2.0-only
#include <linux/dma-buf.h>
#include <asm/page.h>
#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/sched/debug.h>
#include <linux/slab.h>
#ifdef CONFIG_SLUB
#include <linux/slub_def.h>
#endif
#include <linux/vmstat.h>
#include <linux/of_reserved_mem.h>
#ifdef CONFIG_DMABUF_HEAPS
#include <linux/dmabuf/mm_dma_heap.h>
#endif
#undef CREATE_TRACE_POINTS
#ifdef CONFIG_DFX_MEMCHECK
#include <platform/trace/hooks/memcheck.h>
#endif
#ifdef CONFIG_HISI_GPU
#include "memcheck_log_gpumem_hvgr.h"
#elif defined(CONFIG_DFX_OHOS) && defined(CONFIG_MALI_MIDGARD)
#include "memcheck_log_gpumem_mali.h"
#endif
#include <linux/mm/lowmem_killer.h>
#include "mem_info.h"
#include "iofast.h"
#include <liblinux/pal.h>
#ifdef CONFIG_VCODEC_HISP_DFX
#include <platform_include/basicplatform/linux/mm/mem_trace.h>
#endif

static inline unsigned long get_total_present_nr_pages(void)
{
	struct zone *zone;
	unsigned long total_nr_pages = 0;

	for_each_populated_zone(zone)
		total_nr_pages += zone->present_pages;

	return total_nr_pages;
}

static inline struct zone *get_zone_by_type(enum zone_type type)
{
	pg_data_t *pgdat = first_online_pgdat();
	return &pgdat->node_zones[type];
}

static inline unsigned long get_zone_free_nr_pages(enum zone_type type)
{
	struct zone *zone = get_zone_by_type(type);
	return zone_page_state(zone, NR_FREE_PAGES);
}

static inline unsigned long get_zone_present_nr_pages(enum zone_type type)
{
	struct zone *zone = get_zone_by_type(type);
	return zone->present_pages;
}

static inline unsigned long long liblinux_slab_total_pages(void)
{
	return global_node_page_state_pages(NR_SLAB_RECLAIMABLE_B) +
		   global_node_page_state_pages(NR_SLAB_UNRECLAIMABLE_B);
}

int liblinux_fill_mem_info(struct liblinux_mem_info *mem_info)
{
#ifdef CONFIG_DFX_MEMCHECK
	unsigned long gpu_total = 0;
#endif
	mem_info->ion_total_cache = dma_get_total_cache();
	mem_info->ion_total_used = dma_buf_get_total_size() >> PAGE_SHIFT;
#ifdef CONFIG_DMABUF_HEAPS
	mem_info->dmaheap_total_cache = global_zone_page_state(NR_DMAHEAPCACHE_PAGES);
	mem_info->dmaheap_total_used = mm_dmaheap_total() >> PAGE_SHIFT;
	mem_info->dmaheap_freelist_cache = dmaheap_freelist_total();
#endif
#ifdef CONFIG_DFX_MEMCHECK
	trace_hck_gpu_get_total_used(&gpu_total);
	mem_info->gpu_total_used = gpu_total;
#endif
	mem_info->vmalloc_total = (unsigned long)(VMALLOC_END - VMALLOC_START) >> 10;
	mem_info->vmalloc_used = vmalloc_nr_pages();
	mem_info->slab_total = liblinux_slab_total_pages();
	mem_info->slab_unreclaimable = global_node_page_state_pages(NR_SLAB_UNRECLAIMABLE_B);
	mem_info->slab_reclaimable = global_node_page_state_pages(NR_SLAB_RECLAIMABLE_B);
#ifdef CONFIG_OF_RESERVED_MEM
	mem_info->rsv_total_used = dt_memory_reserved_sizeinfo_get() >> PAGE_SHIFT;
#endif
#ifdef CONFIG_DFX_MEMCHECK
	mem_info->buddy_total = get_total_present_nr_pages();
	mem_info->buddy_normal_total = get_zone_present_nr_pages(ZONE_NORMAL);
	mem_info->buddy_normal_free = get_zone_free_nr_pages(ZONE_NORMAL);
	mem_info->buddy_dma_total = get_zone_present_nr_pages(ZONE_DMA);
	mem_info->buddy_dma_free = get_zone_free_nr_pages(ZONE_DMA);
#endif
	mem_info->ldk_others_total = liblinux_iofast_stat_get(LIBLINUX_IOFAST_PAGE_INIT)
				     + liblinux_iofast_stat_get(LIBLINUX_IOFAST_PAGE_PGTBL)
				     + liblinux_iofast_stat_get(LIBLINUX_IOFAST_PAGE_PGD)
				     + liblinux_iofast_stat_get(LIBLINUX_IOFAST_PAGE_FW_CACHE);
#ifdef CONFIG_VCODEC_HISP_DFX
	mem_info->hisp_total_used = (unsigned long)get_hisp_alloc_pages_num();
	mem_info->vcodec_total_used = (unsigned long)get_vdec_alloc_pages();
#endif
	return 0;
}
EXPORT_SYMBOL(liblinux_fill_mem_info);
#ifdef CONFIG_HISI_GPU
void liblinux_get_gpu_mem_usage(uint32_t pid, unsigned long *gpu_info)
{
	*gpu_info = hvgr_mem_get_gpu_memory_byte_by_pid(pid);
}
#else
void liblinux_get_gpu_mem_usage(uint32_t pid, unsigned long *gpu_info)
{
	*gpu_info = 0U;
}
#endif
EXPORT_SYMBOL(liblinux_get_gpu_mem_usage);
#define LIBLINUX_SLAB_MEMTOTAL_MAX (8ULL * 1024 * 1024 * 1024)

static inline unsigned long long liblinux_slab_threshold(unsigned long long mem_total)
{
	return (mem_total > LIBLINUX_SLAB_MEMTOTAL_MAX ? LIBLINUX_SLAB_MEMTOTAL_MAX : mem_total) >> 3;
}

#ifdef CONFIG_DFX_DMABUF_HEAPS
#define LINLINUX_DMABUF_THRESHOLD_MAX (3ULL * 1024 * 1024 * 1024)
static inline unsigned long long liblinux_dmabuf_threshold(unsigned long long mem_total)
{
	return LINLINUX_DMABUF_THRESHOLD_MAX;
}
#endif

static void dump_kswapd_stack(void)
{
	pg_data_t *pgdat = NODE_DATA(0);
	if (!pgdat || !(pgdat->kswapd))
		return;

	sched_show_task(pgdat->kswapd);
}

/**
 * @brief Dump liblinux memory usage when lowmem.
 *
 * @param oom_score_adj
 */
void liblinux_lowmem_debug(short oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	bool verbose = (oom_score_adj == 0) ? true : false;
	unsigned long long slab_total_used = liblinux_slab_total_pages() << PAGE_SHIFT;
	unsigned long long slab_threshold = liblinux_slab_threshold(mem_total);
	if (!is_emergency) {
		/* dump thread stack may malloc to add symbol */
		dump_kswapd_stack();
		/* cannot spin_lock in show_free_areas when emergency */
		show_mem(SHOW_MEM_FILTER_NODES, NULL);
#ifdef CONFIG_DFX_DMABUF_HEAPS
		unsigned long long dmabuf_total_used = mm_dmaheap_total();
		unsigned long long dmabuf_threshold = liblinux_dmabuf_threshold(mem_total);	
		if (dmabuf_total_used > dmabuf_threshold) {
			memtrack_show();
		}		
#endif
	}
#ifdef CONFIG_SLABINFO
	if (slab_total_used > slab_threshold) {
		pr_info("liblinux slab total usage is %llu KB, threshold is %llu KB, exceeded!\n",
				slab_total_used >> 10, slab_threshold >> 10);
		if (!is_emergency) {
			show_slab(verbose);
		} else {
			show_slab_emergency();
		}
	} else {
		pr_info("liblinux slab total usage is %llu KB, threshold is %llu KB.\n",
				slab_total_used >> 10, slab_threshold >> 10);
	}
#endif
#if defined(CONFIG_HISI_GPU) || (defined(CONFIG_DFX_OHOS) && defined(CONFIG_MALI_MIDGARD) && defined(CONFIG_MALI_BORR_OH))
	if (!is_emergency) {
		/* cannot mutex_lock or kvzalloc when emergency */
		memcheck_gpumem_info_show();
	}
#endif
}
EXPORT_SYMBOL(liblinux_lowmem_debug);
