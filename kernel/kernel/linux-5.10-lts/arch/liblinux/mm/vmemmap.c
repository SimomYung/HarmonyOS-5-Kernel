// SPDX-License-Identifier: GPL-2.0
#include <linux/gfp.h>
#include <linux/page_ref.h>
#include <linux/mm.h>
#include <linux/page-flags.h>
#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <liblinux/pal.h>
#include "vmemmap.h"

static DEFINE_SPINLOCK(vmemmap_lock);

struct __vmemmap {
	unsigned long pg_start;
	unsigned long pg_end;
	unsigned long long *pg_bitmap;
} vmemmap_info;

unsigned long long *find_pg_bitmap(unsigned long align_addr)
{
	unsigned long id = (align_addr - vmemmap_info.pg_start) >> PAGE_SHIFT;
	return &vmemmap_info.pg_bitmap[id];
}

void vmemmap_pg_bitmap_init(unsigned long long phy_start, unsigned long long phy_end)
{
	unsigned long map_size;
	unsigned long page_start = (unsigned long)phys_to_page(phy_start);
	unsigned long page_end = (unsigned long)phys_to_page(phy_end);

	BUG_ON(page_start >= page_end);
	vmemmap_info.pg_start = ALIGN_DOWN(page_start, PAGE_SIZE);
	vmemmap_info.pg_end = ALIGN(page_end, PAGE_SIZE);
	map_size = ((vmemmap_info.pg_end - vmemmap_info.pg_start) >> PAGE_SHIFT);
	printk("vmemmap init pg bitmap size %lu\n", map_size);

	vmemmap_info.pg_bitmap = vzalloc(map_size * sizeof(unsigned long long));
	if (vmemmap_info.pg_bitmap == NULL)
		panic("vmemmap vmalloc pg bitmap failed\n");
}

static void __check_pg_bit(unsigned long mgr_page, unsigned long long *pg_bitmap, unsigned long flag,
			   unsigned long bit_start, unsigned long bit_end)
{
#ifdef CONFIG_LIBLINUX_VMEMMAP_DEBUG
	unsigned long bit_reg;
	unsigned long long bit_val = (flag == PG_POPULATE) ? 0 : ULLONG_MAX;

	if (UPDATE_ALL_BIT(bit_start, bit_end)) {
		if (*pg_bitmap != bit_val)
			panic("The bit of pg:0x%lx is invalid[flag:%lu bit:0x%lx]\n",
			      mgr_page, flag, *pg_bitmap);
	} else {
		for (bit_reg = bit_start; bit_reg < bit_end; bit_reg++) {
			if ((((*pg_bitmap) ^ bit_val) >> bit_reg) & 1ULL)
				panic("The bit of pg:0x%lx is invalid[flag:%lu bit:0x%lx reg:%lu]\n",
				      mgr_page, flag, *pg_bitmap, bit_reg);
		}
	}
#endif
}

static void __set_pg_bit(unsigned long long *pg_bitmap,
			 unsigned long bit_start, unsigned long bit_end)
{
	unsigned long bit_reg;

	if (UPDATE_ALL_BIT(bit_start, bit_end))
		*pg_bitmap = ULLONG_MAX;
	else
		for (bit_reg = bit_start; bit_reg < bit_end; bit_reg++)
			*pg_bitmap |= 1ULL << bit_reg;
}

static void __clear_pg_bit(unsigned long long *pg_bitmap,
			   unsigned long bit_start, unsigned long bit_end)
{
	unsigned long bit_reg;

	if (UPDATE_ALL_BIT(bit_start, bit_end))
		*pg_bitmap = 0;
	else
		for (bit_reg = bit_start; bit_reg < bit_end; bit_reg++)
			*pg_bitmap &= ~(1ULL << bit_reg);
}

static void __update_pg_bit(unsigned long mgr_page, unsigned long start_addr,
			    unsigned long end_addr, unsigned long flag)
{
	unsigned long bit_start = (start_addr - mgr_page) / sizeof(struct page);
	unsigned long bit_end = (end_addr - mgr_page) / sizeof(struct page);
	unsigned long long *pg_bitmap = find_pg_bitmap(mgr_page);

	__check_pg_bit(mgr_page, pg_bitmap, flag, bit_start, bit_end);
	if (flag == PG_POPULATE)
		__set_pg_bit(pg_bitmap, bit_start, bit_end);
	else
		__clear_pg_bit(pg_bitmap, bit_start, bit_end);
}

int vmemmap_pg_bitmap_unpopulate(unsigned long page_start, unsigned long page_end)
{
	int ret = 0;
	unsigned long flags;
	unsigned long unpop_start = page_start;
	unsigned long unpop_end = page_end - PAGE_SIZE;

	spin_lock_irqsave(&vmemmap_lock, flags);
	if ((*find_pg_bitmap(unpop_start)) != 0)
		unpop_start += PAGE_SIZE;

	if ((*find_pg_bitmap(unpop_end)) == 0)
		unpop_end += PAGE_SIZE;
	spin_unlock_irqrestore(&vmemmap_lock, flags);

	if (unpop_start < unpop_end)
		ret = liblinux_vmemmap_pg_unpopulate((void *)unpop_start, unpop_end - unpop_start);

	return ret;
}

bool vmemmap_is_populate(unsigned long pfn)
{
	unsigned long flags;
	unsigned long long *pg_bitmap;
	unsigned long mgr_page, bit_reg;
	unsigned long pg = pfn_to_page(pfn);

	if (pg < vmemmap_info.pg_start || vmemmap_info.pg_end < pg)
		return false;

	mgr_page = ALIGN_DOWN(pg, PAGE_SIZE);
	bit_reg = (pg - mgr_page) / sizeof(struct page);
	pg_bitmap = find_pg_bitmap(mgr_page);

	spin_lock_irqsave(&vmemmap_lock, flags);
	if (((*pg_bitmap) >> bit_reg) & 1ULL) {
		spin_unlock_irqrestore(&vmemmap_lock, flags);
		return true;
	}

	spin_unlock_irqrestore(&vmemmap_lock, flags);
	return false;
}

int vmemmap_pg_bitmap_update(const void *start, unsigned long size, unsigned int flag)
{
	int ret = 0;
	unsigned long flags;
	unsigned long mgr_page, page_start, page_end;
	unsigned long align_start = ALIGN_DOWN((unsigned long)start, PAGE_SIZE);
	unsigned long align_end = ALIGN((unsigned long)start + size, PAGE_SIZE);

	if (align_start < vmemmap_info.pg_start || vmemmap_info.pg_end < align_end || size == 0)
		return -EINVAL;

	spin_lock_irqsave(&vmemmap_lock, flags);
	for (mgr_page = align_start; mgr_page < align_end; mgr_page += PAGE_SIZE) {
		page_end = min((unsigned long)start + size, mgr_page + PAGE_SIZE);
		page_start = max((unsigned long)start, mgr_page);
		__update_pg_bit(mgr_page, page_start, page_end, flag);
	}
	spin_unlock_irqrestore(&vmemmap_lock, flags);

#ifndef CONFIG_LIBLINUX_VMEMMAP_IOFAST
	if (flag == PG_POPULATE)
		ret = liblinux_vmemmap_pg_populate((void *)align_start, align_end - align_start);
	else
		ret = vmemmap_pg_bitmap_unpopulate(align_start, align_end);
#endif

	return ret;
}

#ifdef CONFIG_LIBLINUX_VMEMMAP_DEBUG
#define VMEMMAP_DEBUG_MAXLEN 16
static unsigned long long vmemmap_bitmsg_paddr = 0x0;
static inline ssize_t vmemmap_bitmsg_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos)
{
	char strbuf[VMEMMAP_DEBUG_MAXLEN];

	if (size == 0 || size >= VMEMMAP_DEBUG_MAXLEN)
		return -EINVAL;
	if (copy_from_user(strbuf, buf, size))
		return -EFAULT;

	strbuf[size] = '\0';
	vmemmap_bitmsg_paddr = simple_strtoul(strbuf, NULL, 16);

	return size;
}

static int vmemmap_bitmsg_show(struct seq_file *s, void *d)
{
	unsigned long long pfn = phys_to_pfn(vmemmap_bitmsg_paddr);
	unsigned long page = phys_to_page(vmemmap_bitmsg_paddr);

	if (page < vmemmap_info.pg_start || vmemmap_info.pg_end < page) {
		seq_printf(s, "paddr 0x%llx is invalid\n", vmemmap_bitmsg_paddr);
		return 0;
	}

	seq_printf(s, "paddr 0x%llx is %s\n", vmemmap_bitmsg_paddr, vmemmap_is_populate(pfn) ? "populate" : "unpopulate");
	seq_printf(s, "vaddr 0x%llx page_addr 0x%lx pfn 0x%llx\n", pfn_to_virt(pfn), page,  pfn);
	return 0;
}

static int vmemmap_bitmsg_open(struct inode *inode, struct file *file)
{
	return single_open(file, vmemmap_bitmsg_show, inode->i_private);
}

static const struct file_operations vmemmap_bitmsg_debug_fops = {
	.open = vmemmap_bitmsg_open,
	.read = seq_read,
	.write = vmemmap_bitmsg_write,
	.release = single_release,
};

static int vmemmap_dump_debug_show(struct seq_file *s, void *private)
{
	s->count += liblinux_pal_vmemmap_dump(s->buf, s->size);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(vmemmap_dump_debug);

static int __init vmemmap_debug_init(void)
{
	struct dentry *vmemmap_dentry;

	vmemmap_dentry = debugfs_create_dir("vmemmap_debug", NULL);
	if (vmemmap_dentry == NULL) {
		pr_err("vmemmap debug fs init failed\n");
		return -ENODEV;
	}
	debugfs_create_file("bitmsg", 0600, vmemmap_dentry, NULL, &vmemmap_bitmsg_debug_fops);
	debugfs_create_file("info_dump", 0400, vmemmap_dentry, NULL, &vmemmap_dump_debug_fops);

	return 0;
}
late_initcall(vmemmap_debug_init);
#endif /* CONFIG_LIBLINUX_VMEMMAP_DEBUG */
