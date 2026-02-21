// SPDX-License-Identifier: GPL-2.0
#include <asm/page.h>

#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/mm_types.h>
#include <linux/proc_fs.h>
#include <trace/hooks/liblinux.h>

#include <liblinux/page.h>
#include <lnxbase/iofast.h>

#include "iofast.h"
#include "internal.h"  /* mm/internal.h */

#define DEFAULT_NATIVEGFP_WATERLINE	(1UL)
#define MAX_NATIVEGFP_WATERLINE	(10UL)
#define DEFAULT_DMAHEAP_WATERLINE	(1UL << 15) /* 32768 pages, 131,072 KB = 128 MB */
#define MAX_DMAHEAP_WATERLINE		(1UL << 22) /* 4194304 pages, 16,777,216 KB = 16 G */
unsigned long native_gfp_waterline = DEFAULT_NATIVEGFP_WATERLINE;
unsigned long dmaheap_waterline = DEFAULT_DMAHEAP_WATERLINE;

/*
 * The following four numerical statistics are for the use of dfx and are temporarily added.
 * They will be deleted later
 */
atomic_long_t g_native_gfp_alloc = ATOMIC_LONG_INIT(0);
atomic_long_t g_native_gfp_free = ATOMIC_LONG_INIT(0);
atomic_long_t g_iofast_alloc_pages = ATOMIC_LONG_INIT(0);
atomic_long_t g_iofast_free_pages = ATOMIC_LONG_INIT(0);

static struct iofast_stat {
	char *name;
	atomic_long_t value;
} iofast_stat_pages[LIBLINUX_IOFAST_PAGE_TYPE_MAX] = {
	{"buddy", ATOMIC_LONG_INIT(0)},
	{"dma_buf", ATOMIC_LONG_INIT(0)},
	{"init", ATOMIC_LONG_INIT(0)},
	{"pgtbl", ATOMIC_LONG_INIT(0)},
	{"pgd", ATOMIC_LONG_INIT(0)},
	{"fw_cache", ATOMIC_LONG_INIT(0)},
};

void liblinux_iofast_stat_inc(enum liblinux_iofast_page_type page_type, unsigned long nr_pages)
{
	BUG_ON(page_type >= LIBLINUX_IOFAST_PAGE_TYPE_MAX);
	atomic_long_add((long)nr_pages, &iofast_stat_pages[page_type].value);
}

void liblinux_iofast_stat_dec(enum liblinux_iofast_page_type page_type, unsigned long nr_pages)
{
	BUG_ON(page_type >= LIBLINUX_IOFAST_PAGE_TYPE_MAX);
	atomic_long_sub((long)nr_pages, &iofast_stat_pages[page_type].value);
}

void liblinux_iofast_stat_move(enum liblinux_iofast_page_type dst, enum liblinux_iofast_page_type src, unsigned long nr_pages)
{
	BUG_ON((dst >= LIBLINUX_IOFAST_PAGE_TYPE_MAX) || (src >= LIBLINUX_IOFAST_PAGE_TYPE_MAX));
	atomic_long_add((long)nr_pages, &iofast_stat_pages[dst].value);
	atomic_long_sub((long)nr_pages, &iofast_stat_pages[src].value);
}

long liblinux_iofast_stat_get(enum liblinux_iofast_page_type page_type)
{
	BUG_ON(page_type >= LIBLINUX_IOFAST_PAGE_TYPE_MAX);
	return atomic_long_read(&iofast_stat_pages[page_type].value);
}

static void *__mm_map(gfp_t gfp_mask, unsigned int order, unsigned long paddr)
{
	void *vaddr = NULL;
	struct page *page = NULL;
	unsigned long size = 1 << (order + PAGE_SHIFT);

	vaddr = __populate_vmemmap(paddr, size, ZONE_IOFAST);
	if (vaddr == NULL)
		return NULL;

	if (gfp_mask & __GFP_ZERO)
		memset(vaddr, 0, size);

	page = virt_to_page(vaddr);
	if (order && (gfp_mask & __GFP_COMP))
		prep_compound_page(page, order);

	return (void *)page;
}

static void *__liblinux_iofast_alloc_pages(unsigned gfp_mask, unsigned int order,
				  unsigned long length, void **page_array,
				  unsigned int *page_num,  unsigned int gfp_type,
				  enum liblinux_iofast_page_type page_type)
{
	int i;
	int ret;
	void *pret = NULL;
	unsigned int pnum = 0;
	unsigned int pnum_fail = 0;
	unsigned int paddr_num = 0;
	unsigned int flags = __gfp_mask_to_page_pool_flag(gfp_mask) | LIBLINUX_PAL_GFP_NODOWNGRADE
			     | LIBLINUX_PAL_GFP_NORETRY;
	unsigned long long pa_array_impl[MAX_ARRAY_LENGTH] = { 0 };
	unsigned long long *pa_array = &pa_array_impl[0];
	unsigned long nr_pages = 0;

	if (!page_array || !page_num || (length == 0) || (page_type >= LIBLINUX_IOFAST_PAGE_TYPE_MAX)) {
		pr_err("%s invalid parameter\n", __func__);
		return NULL;
	}

	if ((gfp_type == LIBLINUX_PAL_GFP_PGTBL) || (gfp_type == LIBLINUX_PAL_GFP_PGD)) {
		flags |= gfp_type;
	}

	if (length > MAX_ARRAY_LENGTH) {
		pa_array = vzalloc(sizeof(unsigned long) * (PAGE_SIZE - 1));
		if (pa_array == NULL) {
			pr_err("vzalloc failed\n");
			return NULL;
		}
	}

	ret = lnxbase_iofast_alloc_pfn_v(order, pa_array, length, flags, &paddr_num);
	if (ret < 0) {
		goto out;
	}

	for (i = 0; i < paddr_num; i++) {
		page_array[pnum] = __mm_map((gfp_t)gfp_mask, order, pa_array[i] & PAGE_MASK);
		if (page_array[pnum] == NULL) {
			pa_array[pnum_fail++] = pa_array[i];
			continue;
		}
		kasan_alloc_pages(page_array[pnum], order);
		pnum++;
		nr_pages += (1 << order);
	}
	pret = page_array[0];
	*page_num = pnum;
	liblinux_iofast_stat_inc(page_type, nr_pages);
	atomic_long_add(((1 << order) * paddr_num), &g_iofast_alloc_pages);
	if (pnum_fail > 0) {
		ret = lnxbase_iofast_free_pfn_v(pa_array, pnum_fail);
		if (ret != 0)
			pr_warn("liblinux_iofast: free pfn failed\n");
		atomic_long_add(((1 << order) * pnum_fail), &g_iofast_free_pages);
	}

out:
	if (length > MAX_ARRAY_LENGTH) {
		vfree(pa_array);
	}
	return pret;
}

void *liblinux_iofast_alloc_pages(unsigned gfp_mask, unsigned int order,
				  unsigned long length, void **page_array,
				  unsigned int *page_num, enum liblinux_iofast_page_type page_type)
{
	return __liblinux_iofast_alloc_pages(gfp_mask, order, length, page_array, page_num, 0, page_type);
}

void *liblinux_iofast_alloc_page(unsigned gfp_mask, unsigned int order, enum liblinux_iofast_page_type page_type)
{
	int page_num;
	struct page *page;

	return liblinux_iofast_alloc_pages(gfp_mask, order, 1UL, (void **)&page, &page_num, page_type);
}

void *liblinux_iofast_alloc_pt_page(unsigned int order)
{
	int page_num;
	struct page *page;

	return __liblinux_iofast_alloc_pages(GFP_HIGHUSER, order, 1UL, (void **)&page, &page_num,
					     LIBLINUX_PAL_GFP_PGTBL, LIBLINUX_IOFAST_PAGE_PGTBL);
}

void *liblinux_iofast_alloc_pgd_page(unsigned int order)
{
	int page_num;
	struct page *page;

	return __liblinux_iofast_alloc_pages(GFP_HIGHUSER, order, 1UL, (void **)&page, &page_num,
					     LIBLINUX_PAL_GFP_PGD, LIBLINUX_IOFAST_PAGE_PGD);
}

void liblinux_iofast_free_pages(void *page, unsigned int order, enum liblinux_iofast_page_type page_type)
{
	int ret;
	void *vaddr = NULL;
	unsigned long long pa;
	unsigned long long paddr;
	unsigned long size = 1 << (order + PAGE_SHIFT);

	if (page_type >= LIBLINUX_IOFAST_PAGE_TYPE_MAX) {
		pr_err("%s invalid parameter\n", __func__);
		return;
	}

	paddr = page_to_phys((struct page *)page);
	vaddr = page_address((struct page *)page);
	pa = paddr | order;

	kasan_free_pages(page, order);
	liblinux_mm_prepare_unmap(vaddr, size);
	ret = lnxbase_iofast_free_pfn_v(&pa, 1);
	if (ret != 0) {
		pr_err("free pfn failed\n");
		__populate_vmemmap(paddr, size, ZONE_IOFAST);
	} else {
		liblinux_iofast_stat_dec(page_type, (1UL << order));
		atomic_long_add((1 << order), &g_iofast_free_pages);
	}
}

static void hook_free_pages(void *data, void *page, unsigned int order, int *ret)
{
	if (page_zonenum(page) == ZONE_IOFAST) {
		liblinux_iofast_free_pages(page, order, LIBLINUX_IOFAST_PAGE_DMA_BUF);
		*ret = 1;
	}
}
INIT_VENDOR_HOOK(ldk_rvh_free_pages, hook_free_pages);

static int iofast_seq_show(struct seq_file *s, void *d)
{
	struct zone *zone = NULL;
	unsigned long wmark = 0;
	unsigned long free = 0;
	unsigned long present = 0;
	int i;

	for_each_populated_zone(zone) {
		wmark += min_wmark_pages(zone);
		free += zone_page_state(zone, NR_FREE_PAGES);
		present += zone->present_pages;
	}

	seq_printf(s, "native-gfp present %lu, free %lu, wmark %lu \n", present, free, wmark);
	seq_printf(s, "dmaheap: free %lu \n", global_zone_page_state(NR_DMAHEAPCACHE_PAGES));

	seq_printf(s, "native_gfp_waterline %lu \n", native_gfp_waterline);
	seq_printf(s, "dmaheap_waterline %lu \n", dmaheap_waterline);

	seq_printf(s, "%16s %16s %16s\n",
			"name", "alloc", "free");
	seq_printf(s, "%16s %16lld %16lld\n",
			"native_gfp", atomic_long_read(&g_native_gfp_alloc),
			atomic_long_read(&g_native_gfp_free));
	seq_printf(s, "%16s %16lld %16lld\n",
			"liblinux_iofast", atomic_long_read(&g_iofast_alloc_pages),
			atomic_long_read(&g_iofast_free_pages));

	seq_printf(s, " \n");
	seq_printf(s, "ldk iofast pages stat:\n");
	for (i = 0; i < LIBLINUX_IOFAST_PAGE_TYPE_MAX; i++) {
		seq_printf(s, "%16s %s %16lld \n", iofast_stat_pages[i].name, "pages:",
			   atomic_long_read(&iofast_stat_pages[i].value));
	}

	return 0;
}

#define IOFAST_MAX_CMD_SIZE 128
static ssize_t
iofast_write(struct file *file,
				const char __user * buffer,
				size_t count, loff_t * ppos)
{
	char strbuf[IOFAST_MAX_CMD_SIZE];
	char *ptr_native_gfp;
	char *ptr_dmaheap;
	unsigned long native_gfp;
	unsigned long dmaheap;

	if (count == 0 || count > IOFAST_MAX_CMD_SIZE)
		return -EINVAL;

	if (copy_from_user(strbuf, buffer, count))
		return -EFAULT;

	strbuf[count] = '\0';
	ptr_native_gfp = strbuf;
	while (*ptr_native_gfp == ' ') {
		ptr_native_gfp++;
	}
	native_gfp = simple_strtoul(ptr_native_gfp, &ptr_dmaheap, 0);
	if (native_gfp < MAX_DMAHEAP_WATERLINE) {
		native_gfp_waterline = native_gfp;
	}

	while (*ptr_dmaheap == ' ') {
		ptr_dmaheap++;
	}
	dmaheap = simple_strtoul(ptr_dmaheap, NULL, 0);

	if (dmaheap < MAX_DMAHEAP_WATERLINE) {
		dmaheap_waterline = dmaheap;
	}

	return count;
}

static int iofast_open(struct inode *inode, struct file *file)
{
	return single_open(file, iofast_seq_show,
			   PDE_DATA(inode));
}

static const struct proc_ops iofast_proc_ops = {
	.proc_open	= iofast_open,
	.proc_read	= seq_read,
	.proc_write	= iofast_write,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init iofast_proc_init(void)
{
	int i;

	proc_create("iofast", S_IFREG | S_IRUGO | S_IWUSR,
			NULL, &iofast_proc_ops);

	return 0;
}
late_initcall(iofast_proc_init);
