// SPDX-License-Identifier: GPL-2.0-only
#include <asm/page.h>

#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/types.h>


#include "iofast.h"

struct page *alloc_pt_page(unsigned int order)
{
	return liblinux_iofast_alloc_pt_page(order);
}

struct page *alloc_pgd_page(unsigned int order)
{
	return liblinux_iofast_alloc_pgd_page(order);
}

void free_pt_page(struct page *page, unsigned int order)
{
	return liblinux_iofast_free_pages(page, order, LIBLINUX_IOFAST_PAGE_PGTBL);
}

void free_pgd_page(struct page *page, unsigned int order)
{
	return liblinux_iofast_free_pages(page, order, LIBLINUX_IOFAST_PAGE_PGD);
}
EXPORT_SYMBOL(alloc_pt_page);
EXPORT_SYMBOL(free_pt_page);
EXPORT_SYMBOL(alloc_pgd_page);
EXPORT_SYMBOL(free_pgd_page);
