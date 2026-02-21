// SPDX-License-Identifier: GPL-2.0-only
/*
 * liblinux mm subsys init
 */
#include <linux/mm.h>
#include <linux/kmemleak.h>
#include <linux/memblock.h>
#include <linux/page-flags.h>
#include <linux/spinlock.h>
#include <linux/swiotlb.h>
#include <linux/dma-direct.h>
#ifdef CONFIG_LIBLINUX_BUDDY_TRACKER
#include <linux/buddy_tracker.h>
#endif
#include <uapi/linux/mman.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/cacheflush.h>
#include <asm/kasan.h>

#include "page_alloc.h"
#include "vmemmap.h"

/* for dma zone only */
unsigned int dma_zone_only = 0;
unsigned long long kmap_size = (1UL << CONFIG_LIBLINUX_KMAP_BITS);
EXPORT_SYMBOL(kmap_size);

#ifdef CONFIG_ARM64
/*
 * If the corresponding config options are enabled, we create both ZONE_DMA
 * and ZONE_DMA32. By default ZONE_DMA covers the 32-bit addressable memory
 * unless restricted on specific platforms (e.g. 30-bit on Raspberry Pi 4).
 * In such case, ZONE_DMA32 covers the rest of the 32-bit addressable memory,
 * otherwise it is empty.
 */
phys_addr_t arm64_dma_phys_limit __ro_after_init;
#endif

static int __init early_dma_zone_only(char *arg)
{
	if (!arg)
		return -EINVAL;
	if (!strncmp(arg, "true", strlen("true")))
		dma_zone_only = 1;
	else
		dma_zone_only = 0;

	return 0;
}
early_param("dma_zone_only", early_dma_zone_only);

/*
 * Empty_zero_page is a special page that is used for zero-initialized data
 * and COW.
 */
struct page *empty_zero_page;
EXPORT_SYMBOL(empty_zero_page);

/*
 * for native linux-GFP
 */
unsigned long highest_memmap_pfn __read_mostly;
bool node_dirty_ok(struct pglist_data *pgdat)
{
	pr_info("GFP: UN-IMPL: %s: ret=true\n", __func__);
	return true;
}
struct page *__meminit __populate_section_memmap(unsigned long pfn,
		unsigned long nr_pages, int nid, struct vmem_altmap *altmap)
{
	/* no-ops because section memmap already populated outside */
	return pfn_to_page(pfn);
}
void vmemmap_free(unsigned long start, unsigned long end,
		struct vmem_altmap *altmap)
{
	/* no-ops because vmemmap was populated and managed outside */
}

void memmap_init_zone_range(struct zone *zone,
			    unsigned long start_pfn,
			    unsigned long end_pfn)
{
	unsigned long zone_start_pfn = zone->zone_start_pfn;
	unsigned long zone_end_pfn = zone_start_pfn + zone->spanned_pages;
	int nid = zone_to_nid(zone), zone_id = zone_idx(zone);

	start_pfn = clamp(start_pfn, zone_start_pfn, zone_end_pfn);
	end_pfn = clamp(end_pfn, zone_start_pfn, zone_end_pfn);

	if (start_pfn >= end_pfn) {
		pr_warn("liblinux: %s: start_pfn=0x%lx, end_pfn=0x%lx\n",
			__func__, start_pfn, end_pfn);
		return;
	}

	memmap_init_zone(end_pfn - start_pfn, nid, zone_id, start_pfn,
			 zone_end_pfn, MEMINIT_EARLY, NULL, MIGRATE_MOVABLE);
}

void __init memmap_init(void)
{
	unsigned long start_pfn, end_pfn;
	int i, j, nid;

	for_each_mem_pfn_range(i, MAX_NUMNODES, &start_pfn, &end_pfn, &nid) {
		struct pglist_data *node = NODE_DATA(nid);

		for (j = 0; j < MAX_NR_ZONES; j++) {
			struct zone *zone = node->node_zones + j;

			if (!populated_zone(zone))
				continue;

			memmap_init_zone_range(zone, start_pfn, end_pfn);
		}
	}
}
/* ----- END----- */

s64 memstart_addr;
EXPORT_SYMBOL(memstart_addr);

unsigned long kmap_offset __ro_after_init = 0;
EXPORT_SYMBOL(kmap_offset);

#if defined(CONFIG_RANDOMIZE_BASE) && !defined(CONFIG_KASAN)
static void __init init_kmap_random(struct liblinux_pool_base *pool_base)
{
	uint64_t u64_random;
	int ret;
	ret = liblinux_pal_get_random_u64(&u64_random);
	if (ret != 0)
		pr_warn("get random failed\n");
	else
		kmap_offset = (unsigned long)u64_random;

	kmap_offset &= (~(SZ_2M - 1)); /* clear low bits */
	kmap_offset &= ((1UL << (VA_BITS - 2)) - 1); /* clear high bits */
	pool_base->kmap_virt_start = PAGE_OFFSET;
}
#else
static void __init init_kmap_random(struct liblinux_pool_base *pool_base)
{
	return;
}
#endif

/* from mm/memory.c */
void *high_memory;
EXPORT_SYMBOL(high_memory);

/*
 * override mm/page_alloc.c
 * return actual memory ranges include those not allocated to liblinux
 */
void __override get_pfn_range_for_nid(unsigned int nid,
				      unsigned long *start_pfn,
				      unsigned long *end_pfn)
{
	*start_pfn = min_low_pfn;
	*end_pfn = max_pfn;
}
unsigned long __override find_min_pfn_with_active_regions(void)
{
	return min_low_pfn;
}

void __init mem_init(void)
{
#ifdef CONFIG_ARM64
	if (swiotlb_force == SWIOTLB_FORCE ||
	    max_pfn >= PFN_DOWN(arm64_dma_phys_limit)) {
		/*
		 * note: should support `memblock_alloc_low`:
		 * we alloc 64MB DMA mem at `mm_init` to memblock. and `memblock.bottom_up` is top-down.
		 * it's may enough. If it failed, it whill panic when do `swiotlb_map`.
		 */
		swiotlb_init(1);
	} else
		swiotlb_force = SWIOTLB_NO_FORCE;
#endif

	/* free unused memblock to buddy */
	memblock_free_all();
}

/*
 * init mm subsys
 */
static void __init mm_init(unsigned long pfn_start, unsigned long pfn_end,
			   unsigned init_order)
{
	unsigned long max_zone_pfn[MAX_NR_ZONES] = {0};
	unsigned long dma_alloc_size = 0;
	unsigned long normal_alloc_size = 0;
	unsigned long max_order_size = PAGE_SIZE << (MAX_ORDER - 1);
	struct page *pg = NULL;
	void *p = NULL;
	int i;
	unsigned long nr_pages = 0;

	/* init memory layout */
	min_low_pfn = pfn_start;
	max_pfn = max_low_pfn = pfn_end;

	/*
	 * add early pages to memblock:
	 *  - ZONE_DMA from sysmgr: 64MB
	 *  - ZONE_NORMAL form sysmgr: `MAX_ORDER + 5` (a.k.a. 256MB)
	 */
	pg = (struct page *)liblinux_iofast_alloc_page(GFP_DMA, get_order(SZ_64M), LIBLINUX_IOFAST_PAGE_INIT);
	if (pg != NULL) {
		nr_pages = 1 << get_order(SZ_64M);
		memblock_add(page_to_phys(pg), SZ_64M);
	} else {
		for (i = 0; i < SZ_64M / max_order_size; ++i) {
			pg = (struct page *)liblinux_iofast_alloc_page(GFP_DMA, get_order(max_order_size), LIBLINUX_IOFAST_PAGE_INIT);
			if (pg != NULL) {
				nr_pages += (1 << get_order(max_order_size));
				memblock_add(page_to_phys(pg), max_order_size);
				dma_alloc_size += max_order_size;
			} else {
				pr_info("%s: i = %d, alloc 0x%lx failed, dma_alloc_size = 0x%lx\n",
					__func__, i, max_order_size, dma_alloc_size);
				break;
			}
		}
		if ((dma_alloc_size == 0) || (swiotlb_size_or_default() > max_order_size)) {
			panic("liblinux: alloc DMA memory for init memblock failed\n");
		}
	}

	pg = (struct page *)liblinux_iofast_alloc_page(GFP_KERNEL, init_order, LIBLINUX_IOFAST_PAGE_INIT);
	if (pg != NULL) {
		nr_pages += 1 << init_order;
		memblock_add(page_to_phys(pg), 1UL << (init_order + PAGE_SHIFT));
	} else {
		for (i = 0; i < (PAGE_SIZE << init_order) / max_order_size; ++i) {
			pg = (struct page *)liblinux_iofast_alloc_page(GFP_KERNEL, get_order(max_order_size), LIBLINUX_IOFAST_PAGE_INIT);
			if (pg != NULL) {
				nr_pages += (1 << get_order(max_order_size));
				memblock_add(page_to_phys(pg), max_order_size);
				normal_alloc_size += max_order_size;
			} else {
				pr_info("%s: i=%d, alloc failed, size=0x%lx\n",__func__, i, max_order_size);
			}
		}
		if (normal_alloc_size == 0) {
			panic("liblinux: alloc normal memory for init memblock failed\n");
		}
	}

	liblinux_iofast_stat_move(LIBLINUX_IOFAST_PAGE_BUDDY, LIBLINUX_IOFAST_PAGE_INIT, nr_pages);

	/* init free area */
	sparse_init();
	/* same with arch/arm64/mm/init.c */
	zone_dma_bits = 32U;
	if (dma_zone_only) {
		max_zone_pfn[ZONE_DMA] = pfn_end;
#ifdef CONFIG_ARM64
		arm64_dma_phys_limit = pfn_end;
#endif
	} else {
		max_zone_pfn[ZONE_DMA] = PHYS_PFN(SZ_4G);
#ifdef CONFIG_ARM64
		arm64_dma_phys_limit = SZ_4G;
#endif
	}
	for (i = ZONE_NORMAL; i < MAX_NR_ZONES; i++) {
		max_zone_pfn[i] = pfn_end;
	}
	free_area_init(max_zone_pfn);

	/* alloc zero page from memblock */
	p = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
	empty_zero_page = virt_to_page(p);
}

static int __init arch_vmemmap_init(unsigned long long start, unsigned long long end)
{
	static struct liblinux_vmemmap_info info;
	info.page_size = sizeof(struct page);
	info.vmemmap_start = VMEMMAP_START;
	info.vmemmap_size = (unsigned long long)phys_to_page(end) - (unsigned long long)phys_to_page(start);
	info.vmemmap_size = ALIGN(info.vmemmap_size, PAGE_SIZE);

#ifdef CONFIG_LIBLINUX_VMEMMAP_IOFAST
	info.flags = LIBLINUX_VMEMMAP_POPULATE_IOFAST;
#endif
	vmemmap_pg_bitmap_init(start, end);

	return liblinux_pal_vmemmap_init(&info);
}

void __init arch_page_alloc_init(void)
{
	int ret;
#ifdef CONFIG_PCI
	void *pci_vaddr = NULL;
#endif
	struct liblinux_ram_info ram_info;
	struct liblinux_pool_base pool_base = {
		.kmap_virt_start = (unsigned long)PAGE_OFFSET,
		.kmap_virt_end = (unsigned long)PAGE_OFFSET + LIBLINUX_KMAP_SIZE,
		.phys_offset = 0,
		.page_sizeorder = PAGE_SHIFT,
	};
	struct liblinux_page_allocator *page_pool_main = NULL;

	init_kmap_random(&pool_base);

	ret = liblinux_pal_iofast_page_info_init(VMEMMAP_START, sizeof(struct page), PG_slab);
	if (ret) {
		panic("failed to init iofast_page_info: %d\n", ret);
	}

#ifdef CONFIG_PCI
	/* reserve virtual space for `PCI IO SAPCE` with RW */
	pci_vaddr = liblinux_pal_vm_prepare((unsigned long)PCI_IOBASE, PAGE_ALIGN(IO_SPACE_LIMIT),
					    PROT_READ | PROT_WRITE, LIBLINUX_PAL_REMAP_DEVICE);
	if (pci_vaddr == NULL) {
		panic("reserve range for PCI_IO (0x%p,size=0x%x)\n",
		       PCI_IOBASE, IO_SPACE_LIMIT);
	}
#endif

	ret = liblinux_pal_page_alloc_init(&pool_base, &ram_info);
	if (ret < 0) {
		panic("page_alloc init failed: %d\n", ret);
	}

	/* iofast mem will change kampsize in liblinux_pal_page_alloc_init. so reset kmap value */
	if (pool_base.kmap_virt_end - pool_base.kmap_virt_start != kmap_size) {
		kmap_size = pool_base.kmap_virt_end - pool_base.kmap_virt_start;
	}

	memstart_addr = ram_info.memstart;
	high_memory = __va(ram_info.memend);

	BUG_ON(phys_to_page(memstart_addr) != (struct page *)VMEMMAP_START);

	if (ram_info.total_ram > LIBLINUX_KMAP_SIZE) {
		panic("total_ram size(size=0x%lx) out of range reserve for KMAPS (size=0x%lx)\n",
		     (unsigned long)ram_info.total_ram, LIBLINUX_KMAP_SIZE);
	}

	atomic_long_add(ram_info.total_ram >> PAGE_SHIFT, &_totalram_pages);

	kasan_init();

	/* init vmemmap region */
	ret = arch_vmemmap_init(ram_info.memstart, ram_info.memend);
	if (ret	< 0) {
		panic("vmemmap init failed, ret=%d\n", ret);
	}
#ifdef CONFIG_LIBLINUX_BUDDY_TRACKER
	(void)liblinux_buddy_tracker_set_vmemmap_size(
		(unsigned long long)phys_to_page(ram_info.memend) - (unsigned long long)phys_to_page(ram_info.memstart));
#endif
    ret = page_pool_init(&page_pool_main);
	if (ret < 0) {
		panic("init page pool failed: %d\n", ret);
	}

	mm_init(__phys_to_pfn(ram_info.memstart), __phys_to_pfn(ram_info.memend),
		MAX_ORDER + 5);
}

int pfn_valid(unsigned long pfn)
{
	int ret;
	phys_addr_t addr = pfn << PAGE_SHIFT;

	if ((addr >> PAGE_SHIFT) != pfn)
		return 0;

	if ((pfn < PHYS_PFN(memstart_addr)) || (pfn >= max_pfn))
		return 0;

#ifdef CONFIG_SPARSEMEM
	if (pfn_to_section_nr(pfn) >= NR_MEM_SECTIONS)
		return 0;

	if (!valid_section(__pfn_to_section(pfn)))
		return 0;
#endif

	if(!vmemmap_is_populate(pfn))
		return 0;

	return 1;
}
EXPORT_SYMBOL(pfn_valid);

void __override set_zone_contiguous(struct zone *zone)
{
	/* liblinux: always set zone as contiguous */
	zone->contiguous = true;
}

long __override si_mem_available(void)
{
	return totalram_pages();
}

void __override files_maxfiles_init(void)
{
	unsigned long n;
	unsigned long nr_pages = totalram_pages();

	n = (nr_pages * (PAGE_SIZE / 1024)) / 10;
	files_stat.max_files = max_t(unsigned long, n, NR_FILE);
}
