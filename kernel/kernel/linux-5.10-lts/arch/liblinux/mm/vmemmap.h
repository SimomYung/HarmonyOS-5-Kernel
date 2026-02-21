/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_VMEMMAP_H__
#define __LIBLINUX_VMEMMAP_H__

#include <linux/types.h>

enum {
	PG_POPULATE = 0,
	PG_UNPOPULATE,
};

#define VMEMMAP_MAX_BITS 64
#define UPDATE_ALL_BIT(start, end) (start == 0 && end == VMEMMAP_MAX_BITS)
static_assert(PAGE_SIZE % sizeof(struct page) == 0);
static_assert((PAGE_SIZE / sizeof(struct page)) == VMEMMAP_MAX_BITS);

void vmemmap_pg_bitmap_init(unsigned long long phy_start, unsigned long long phy_end);
int vmemmap_pg_bitmap_update(const void *start, unsigned long size, unsigned int flag);

bool vmemmap_is_populate(unsigned long pfn);

#endif
