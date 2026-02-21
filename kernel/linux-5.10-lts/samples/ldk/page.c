// SPDX-License-Identifier: GPL-2.0
#include <linux/gfp.h>
#include <linux/page_ref.h>
#include <linux/mm.h>
#include <linux/page-flags.h>

void *dh_backend_alloc_page(unsigned int order)
{
	return (void *)__get_free_pages(GFP_ATOMIC | __GFP_NOWARN | __GFP_ZERO, order);
}
