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
#include "libdh_linux.h"
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

/* alloc/free pfn print threshold time(us) */
#define ALLOC_PFN_TIME_PRINT_THRESHOLD 1000
#define FREE_PFN_TIME_PRINT_THRESHOLD 1000

static int liblinux_iofast_alloc_pfn_v_time(unsigned int order, unsigned long long *pa_array,
					    unsigned long num, unsigned int flags,
					    unsigned int *pa_num, unsigned long *ktime_us)
{
	int ret = 0;
	unsigned long ktime_b, ktime_e = 0;
	unsigned long ktime_used = 0;

	if (!pa_array)
		return -EINVAL;

	ktime_b = liblinux_get_time_us();
	ret = lnxbase_iofast_alloc_pfn_v(order, pa_array, num, flags, pa_num);
	ktime_e = liblinux_get_time_us();
	ktime_used = ktime_e - ktime_b;
	if (ktime_us)
		*ktime_us = ktime_used;

	if (ktime_used > ALLOC_PFN_TIME_PRINT_THRESHOLD) {
		pr_warn_ratelimited("alloc pfn order %u, num %u, flags %#x, ktime %lu us is large\n",
			order, *pa_num, flags, ktime_used);
	}
	return ret;
}

static int liblinux_iofast_free_pfn_v_time(unsigned long long *pa_array, unsigned int pa_num,
					   unsigned long *ktime_us)
{
	int ret = 0;
	unsigned long ktime_b, ktime_e = 0;
	unsigned long ktime_used = 0;
	unsigned int order = 0;

	if (!pa_array)
		return -EINVAL;
	order = pa_array[0] & 0xFFF;

	ktime_b = liblinux_get_time_us();
	ret = lnxbase_iofast_free_pfn_v(pa_array, pa_num);
	ktime_e = liblinux_get_time_us();
	ktime_used = ktime_e - ktime_b;
	if (ktime_us)
		*ktime_us = ktime_used;

	if (ktime_used > FREE_PFN_TIME_PRINT_THRESHOLD) {
		pr_warn_ratelimited("free pfn order %u, num %u, ktime %lu us is large\n",
			order, pa_num, ktime_used);
	}
	return ret;
}


void liblinux_iofast_update_dev_reserve(unsigned long long size)
{
	int ret = lnxbase_iofast_update_dev_reserve(size);
	if (ret) {
		pr_err("iofast add dy reserved size 0x%llx failed, ret %d\n", size, ret);
	}
}

#ifdef CONFIG_LIBLINUX_PAGE_TIME_STAT
static void __iofast_time_stat_get(struct seq_file *s)
{
	page_time_stat_get(IOFAST_ALLOC, s);
	page_time_stat_get(IOFAST_FREE, s);
	return;
}

int liblinux_iofast_alloc_pfn_v(unsigned int order, unsigned long long *pa_array,
				unsigned long num, unsigned int flags,
				unsigned int *pa_num)
{
	int ret;
	unsigned long ktime_us = 0;

	ret = liblinux_iofast_alloc_pfn_v_time(order, pa_array, num, flags, pa_num, &ktime_us);
	if ((ret == 0) && (*pa_num > 0)) {
		page_succ_time_stat_account(IOFAST_ALLOC, ktime_us);
	}
	page_total_time_stat_account(IOFAST_ALLOC, ktime_us);
	page_order_time_stat_account(IOFAST_ALLOC, order, ktime_us);

	return ret;
}

int liblinux_iofast_free_pfn_v(unsigned long long *pa_array, unsigned int pa_num)
{
	int ret;
	unsigned long ktime_us = 0;

	ret = liblinux_iofast_free_pfn_v_time(pa_array, pa_num, &ktime_us);
	if (ret == 0) {
		page_succ_time_stat_account(IOFAST_FREE, ktime_us);
	}
	page_total_time_stat_account(IOFAST_FREE, ktime_us);
	page_order_time_stat_account(IOFAST_FREE, (pa_array[0] & 0xFFF), ktime_us);

	return ret;
}
#else
int liblinux_iofast_alloc_pfn_v(unsigned int order, unsigned long long *pa_array,
				unsigned long num, unsigned int flags,
				unsigned int *pa_num)
{
	return liblinux_iofast_alloc_pfn_v_time(order, pa_array, num, flags, pa_num, NULL);
}

int liblinux_iofast_free_pfn_v(unsigned long long *pa_array, unsigned int pa_num)
{
	return liblinux_iofast_free_pfn_v_time(pa_array, pa_num, NULL);
}
#endif

static atomic_long_t iofast_mm_map_total_time_stat = ATOMIC_LONG_INIT(0);
static atomic_long_t iofast_mm_unmap_total_time_stat = ATOMIC_LONG_INIT(0);
void liblinux_iofast_mm_map_time_stat(unsigned long ktime_us)
{
	atomic_long_add(ktime_us, &iofast_mm_map_total_time_stat);
}

void liblinux_iofast_mm_unmap_time_stat(unsigned long ktime_us)
{
	atomic_long_add(ktime_us, &iofast_mm_unmap_total_time_stat);
}

static unsigned int __mm_map_pa_to_page_array(gfp_t gfp_mask,
	unsigned int order, enum liblinux_iofast_page_type page_type,
	unsigned int paddr_num, unsigned long long *pa_array, void **page_array)
{
	int i;
	int ret;
	unsigned int pnum = 0;
	unsigned int pnum_fail = 0;
	unsigned long nr_pages = 0;
	unsigned long ktime_b, ktime_e = 0;

	dmabuf_systrace_begin("__mm_map order:%u, paddr_num:%u", order, paddr_num);
	ktime_b = liblinux_get_time_us();
	for (i = 0; i < paddr_num; i++) {
		page_array[pnum] = __mm_map(gfp_mask, order, pa_array[i] & PAGE_MASK);
		if (page_array[pnum] == NULL) {
			pa_array[pnum_fail++] = pa_array[i];
			continue;
		}
		kasan_alloc_pages(page_array[pnum], order);
		pnum++;
		nr_pages += (1 << order);
	}

	ktime_e = liblinux_get_time_us();
	liblinux_iofast_mm_map_time_stat(ktime_e - ktime_b);
	dmabuf_systrace_end();

	liblinux_iofast_stat_inc(page_type, nr_pages);
	atomic_long_add(((1 << order) * paddr_num), &g_iofast_alloc_pages);
	if (pnum_fail > 0) {
		ret = liblinux_iofast_free_pfn_v(pa_array, pnum_fail);
		if (ret != 0)
			pr_warn("liblinux_iofast: free pfn failed\n");
		atomic_long_add(((1 << order) * pnum_fail), &g_iofast_free_pages);
	}

	return pnum;
}

static void *__liblinux_iofast_alloc_pages(unsigned gfp_mask, unsigned int order,
				  unsigned long length, void **page_array,
				  unsigned int *page_num,  unsigned int gfp_type,
				  enum liblinux_iofast_page_type page_type)
{
	int i;
	int ret;
	void *pret = NULL;
	unsigned int paddr_num = 0;
	unsigned int flags = __gfp_mask_to_page_pool_flag(gfp_mask) | LIBLINUX_PAL_GFP_NODOWNGRADE
			     | LIBLINUX_PAL_GFP_NORETRY;
	unsigned long long pa_array_impl[MAX_ARRAY_LENGTH] = { 0 };
	unsigned long long *pa_array = &pa_array_impl[0];

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

	dmabuf_systrace_begin("liblinux_iofast_alloc_pfn_v order:%u", order);
	ret = liblinux_iofast_alloc_pfn_v(order, pa_array, length, flags, &paddr_num);
	dmabuf_systrace_end();
	if (ret < 0) {
		goto out;
	}

	*page_num = __mm_map_pa_to_page_array((gfp_t)gfp_mask, order, page_type, paddr_num, pa_array, page_array);
	pret = page_array[0];

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

unsigned int liblinux_iofast_palloc_burst(unsigned int gfp_mask, unsigned int order,
					enum liblinux_iofast_page_type page_type,
					unsigned long length, void **page_array)
{
	int ret;
	unsigned int pnum = 0;
	unsigned int pa_num = 0;
	unsigned int flags = __gfp_mask_to_page_pool_flag(gfp_mask);
	unsigned long long pa_array_impl[MAX_ARRAY_LENGTH] = {0};
	unsigned long long *pa_array = pa_array_impl;

	if (!page_array || (length == 0) || (page_type != LIBLINUX_IOFAST_PAGE_DMA_BUF)) {
		pr_err("%s: invalid length %lu, page_type %u\n", __func__, length, page_type);
		return pnum;
	}

	/*
	 * lnxbase_iofast_alloc_pfn_v uses the lower 12 bits store pa_num to pass to sysmgr,
	 * so the number of pages requested in a batch cannot exceed PAGE_SIZE - 1.
	 */
	length = min(length, PAGE_SIZE - 1);
	if (length > MAX_ARRAY_LENGTH) {
		pa_array = vzalloc(sizeof(unsigned long long) * (length));
		if (!pa_array) {
			pr_err("%s: pa_array vzalloc failed\n", __func__);
			return pnum;
		}
	}

	flags |= LIBLINUX_PAL_GFP_NODOWNGRADE;
	if (order > 0) {
		flags |= LIBLINUX_PAL_GFP_NORETRY;
	} else {
		flags |= LIBLINUX_PAL_GFP_MAYSLOW;
	}

	dmabuf_systrace_begin("liblinux_iofast_alloc_pfn_v order:%u, length:%lu", order, length);
	ret = liblinux_iofast_alloc_pfn_v(order, pa_array, length, flags, &pa_num);
	dmabuf_systrace_end();
	if (ret < 0) {
		pr_err("%s: liblinux_iofast_alloc_pfn_v ret %d, order:%u, length:%lu\n", __func__, ret, order, length);
		goto out;
	}

	pnum = __mm_map_pa_to_page_array(gfp_mask, order, page_type, pa_num, pa_array, page_array);

out:
	if (length > MAX_ARRAY_LENGTH)
		vfree(pa_array);

	return pnum;
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
	ret = liblinux_iofast_free_pfn_v(&pa, 1);
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

#ifdef CONFIG_LIBLINUX_PAGE_TIME_STAT
	__iofast_time_stat_get(s);
#endif

	seq_printf(s, " \n");
	seq_printf(s, "%22s : %16ld(us)\n", "mm_map total time", atomic_long_read(&iofast_mm_map_total_time_stat));
	seq_printf(s, "%22s : %16ld(us)\n", "mm_unmap total time", atomic_long_read(&iofast_mm_unmap_total_time_stat));

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
#ifdef CONFIG_LIBLINUX_PAGE_TIME_STAT
	page_time_stat_init(IOFAST_ALLOC);
	page_time_stat_init(IOFAST_FREE);
#endif
	proc_create("iofast", S_IFREG | S_IRUGO | S_IWUSR,
			NULL, &iofast_proc_ops);

	return 0;
}
late_initcall(iofast_proc_init);
